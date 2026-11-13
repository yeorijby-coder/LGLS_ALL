using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Threading;
using System.Data;

namespace TSK_HostCom
{
	public struct LogMsgInfo
	{
		public string g_strTime;
		public string g_strType;
		public string g_strMsg;
	}
	class modWorkThread
	{

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: Listen Socket Thread
		// [LGLS 2026-09-29] ★작업 스레드 안전망★ (사용자 지시 - 현장 CLR20r3).
		//   .NET 2.0 부터 작업 스레드의 미처리 예외는 프로세스를 통째로 끝낸다.
		//   AppDomain.UnhandledException 으로는 막지 못하고 로그만 남길 수 있다.
		//   단독 EXE 때는 HOST 만 죽었지만 ALL_TASK 는 한 프로세스라 EQP·IO 까지 함께 내려간다.
		//   그래서 스레드 바깥을 감싸 ★그 스레드만★ 끝내고 프로세스는 살려 둔다.
		//   스레드 본체(아래 함수들)는 한 줄도 바꾸지 않았다.
		// [LGLS 2026-09-30] ★수신 접속을 여러 개 받을지★ (기본 1). 구 ECP 의 TcpServer 는 접속마다 따로 받았다.
		//   0 이면 종전처럼 접속 1개만 두고 새 접속이 오면 이전 접속을 닫는다(단, 소켓을 서로 닫는 경합은 고쳤다).
		private static bool SrvMultiConn
		{
			get
			{
				try
				{
					string strIni = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "EcsComA.ini");
					return modDefAPI.GetPrivateProfileInt("Host", "SRV_MULTI_CONN", 1, strIni) != 0;
				}
				catch { return true; }
			}
		}

		// [LGLS 2026-11-10] ★수신 접속을 하나만 허용할지★ (기본 1). 이미 접속이 살아 있으면 다른 접속은 받자마자 닫는다(사용자 지시).
		//   0 이면 종전대로(SRV_MULTI_CONN 규칙) 새 접속을 그대로 받는다.
		private static bool SrvSingleConn
		{
			get
			{
				try
				{
					string strIni = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "EcsComA.ini");
					return modDefAPI.GetPrivateProfileInt("Host", "SRV_SINGLE_CONN", 1, strIni) != 0;
				}
				catch { return true; }
			}
		}

		// [LGLS 2026-11-11] EcsComA.ini [Host] 정수 키 (접속 때마다 읽는다)
		private static int HostIniInt(string pKey, int pDef)
		{
			try { return modDefAPI.GetPrivateProfileInt("Host", pKey, pDef, System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "EcsComA.ini")); }
			catch { return pDef; }
		}

		// [LGLS 2026-11-11] ★상대가 흔적 없이 사라진 접속(FIN/RST 없이 망이 끊김, 상대 전원 차단)을 걸러 낸다★
		//   수신 소켓에 연결 유지 확인(TCP keep-alive)이 없어 그런 접속의 스레드가 Receive 에서 영원히 대기했다.
		//   그 스레드가 접속 수만큼 쌓이면(32비트 프로세스, 스레드 약 1,500~1,900개) 스레드를 더 못 만들어 수신이 멈춘다.
		//   [Host] SRV_KEEPALIVE_SEC (기본 60초, 0 이면 끔) 동안 조용하면 10초 간격으로 확인하고, 응답이 없으면 Receive 가 예외로 끝나
		//   그 접속의 스레드가 스스로 정리된다.
		private static void SetKeepAlive(Socket pSock)
		{
			try
			{
				int nSec = HostIniInt("SRV_KEEPALIVE_SEC", 60);
				if (nSec <= 0) return;
				byte[] b = new byte[12];
				BitConverter.GetBytes((uint)1).CopyTo(b, 0);                    // on
				BitConverter.GetBytes((uint)(nSec * 1000)).CopyTo(b, 4);        // 조용한 시간(ms)
				BitConverter.GetBytes((uint)10000).CopyTo(b, 8);                // 확인 간격(ms)
				pSock.IOControl(IOControlCode.KeepAliveValues, b, null);
			}
			catch { }
		}

		// [LGLS 2026-11-10] 목록의 접속이 정말 살아 있는지 - 스레드가 돌고 소켓이 열려 있고, 상대가 FIN/RST 를 보내지 않았을 것.
		//   (반쯤 끊긴 접속이 새 접속을 영영 막지 않도록, 죽은 것으로 보이면 여기서 닫고 목록에서 뺀다)
		private static CSrvWork FindAliveSrvWork()
		{
			CSrvWork[] arr;
			lock (modDefApp.g_lstSrvWork) { arr = modDefApp.g_lstSrvWork.ToArray(); }
			foreach (CSrvWork x in arr)
			{
				bool bAlive = false;
				try
				{
					Socket s = x.m_sktSock;
					bAlive = (s != null && x.m_thrThreadObj != null && x.m_thrThreadObj.IsAlive && s.Connected
							  && !(s.Poll(0, SelectMode.SelectRead) && s.Available == 0));
				}
				catch { bAlive = false; }
				if (bAlive) return x;
				// 죽은 접속 정리 (제 소켓만 닫는다 - 스레드는 스스로 끝나며 목록에서도 뺀다)
				try { x.m_blSockConnected = false; modCmWork.CloseSocket(ref x.m_sktSock); } catch { }
			}
			return null;
		}

		// [LGLS 2026-11-10] ★ini [Network] RemoteIP 가 아닌 IP 에서 온 접속은 거절★ (기본 1, [Host] SRV_ALLOW_ONLY_REMOTEIP).
		//   RemoteIP 는 상위(WMS) 주소다 - 그 외 곳에서 붙는 것을 막는다. 같은 PC 시험(루프백)은 RemoteIP 가 이 PC 의 주소일 때만 통과.
		//   RemoteIP 를 해석할 수 없으면(호스트명 등) 거르지 않는다. 0 이면 거르지 않는다.
		private static bool SrvAllowOnlyRemoteIp
		{
			get
			{
				try
				{
					string strIni = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "EcsComA.ini");
					return modDefAPI.GetPrivateProfileInt("Host", "SRV_ALLOW_ONLY_REMOTEIP", 1, strIni) != 0;
				}
				catch { return true; }
			}
		}

		private static bool IsAllowedPeer(Socket pSock, out string pAllowIp)
		{
			pAllowIp = modDefApp.g_strRemoteIP;
			if (!SrvAllowOnlyRemoteIp) return true;
			System.Net.IPAddress allow;
			if (!System.Net.IPAddress.TryParse((pAllowIp ?? "").Trim(), out allow)) return true;
			// [LGLS 2026-11-13] ★상위 IP 가 두 개인 현장★ (사용자 지시) - ini [Network] RemoteIP2 에 적은 IP 도 받는다.
			//   쉼표로 여러 개를 적을 수 있다. 접속을 받을 때마다 읽으므로 저장만 하면 된다(재기동 불필요).
			//   보내는 쪽(상태·완료 보고)은 종전대로 RemoteIP 한 곳이다. 비어 있으면 종전과 같다.
			List<System.Net.IPAddress> lstAllow = new List<System.Net.IPAddress>();
			lstAllow.Add(allow);
			try
			{
				System.Text.StringBuilder sb = new System.Text.StringBuilder(256);
				modDefAPI.GetPrivateProfileString("Network", "RemoteIP2", "", sb, sb.Capacity, System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "EcsComA.ini"));
				foreach (string s in sb.ToString().Split(new char[] { ',', ';', ' ' }, StringSplitOptions.RemoveEmptyEntries))
				{
					System.Net.IPAddress a;
					if (System.Net.IPAddress.TryParse(s.Trim(), out a))
					{
						lstAllow.Add(a);
						pAllowIp += ", " + a.ToString();
					}
				}
			}
			catch { }
			System.Net.IPEndPoint ep = pSock.RemoteEndPoint as System.Net.IPEndPoint;
			if (ep == null) return true;
			System.Net.IPAddress peer = ep.Address;
			foreach (System.Net.IPAddress a in lstAllow)
				if (peer.Equals(a)) return true;
			// 이 PC 안에서 붙은 접속(루프백)은 허용 IP 가 이 PC 자신의 주소(127.x 포함)일 때만
			if (System.Net.IPAddress.IsLoopback(peer))
			{
				foreach (System.Net.IPAddress a in lstAllow)
					if (System.Net.IPAddress.IsLoopback(a)) return true;
				try
				{
					foreach (System.Net.IPAddress my in System.Net.Dns.GetHostAddresses(System.Net.Dns.GetHostName()))
						foreach (System.Net.IPAddress a in lstAllow)
							if (my.Equals(a)) return true;
				}
				catch { }
			}
			return false;
		}

		// [LGLS 2026-11-10] 상대(Accept 한 쪽) 표기. 같은 PC 에서 127.0.0.1 로 붙은 접속은 이 PC 의 실제 IP 를 함께 적는다
		//   (루프백 주소는 "내 PC" 라는 뜻일 뿐이라 어느 PC 인지 알 수 없다 - 예 192.168.0.10(127.0.0.1):57412).
		private static string PeerText(Socket pSock)
		{
			System.Net.IPEndPoint ep = pSock.RemoteEndPoint as System.Net.IPEndPoint;
			if (ep == null) return pSock.RemoteEndPoint.ToString();
			string strIp = ep.Address.ToString();
			if (System.Net.IPAddress.IsLoopback(ep.Address))
			{
				string strMine = MyRealIp();
				if (strMine.Length > 0) strIp = strMine + "(" + strIp + ")";
			}
			return strIp + ":" + ep.Port;
		}

		// 이 PC 의 실제 IPv4 - 켜져 있는 어댑터 중 게이트웨이가 있는 것을 먼저, 없으면 처음 보이는 것. 못 찾으면 빈 문자열.
		private static string MyRealIp()
		{
			try
			{
				string strFirst = "";
				foreach (System.Net.NetworkInformation.NetworkInterface ni in System.Net.NetworkInformation.NetworkInterface.GetAllNetworkInterfaces())
				{
					if (ni.OperationalStatus != System.Net.NetworkInformation.OperationalStatus.Up) continue;
					if (ni.NetworkInterfaceType == System.Net.NetworkInformation.NetworkInterfaceType.Loopback) continue;
					System.Net.NetworkInformation.IPInterfaceProperties ip = ni.GetIPProperties();
					foreach (System.Net.NetworkInformation.UnicastIPAddressInformation ua in ip.UnicastAddresses)
					{
						if (ua.Address.AddressFamily != System.Net.Sockets.AddressFamily.InterNetwork) continue;
						if (ip.GatewayAddresses.Count > 0 && ip.GatewayAddresses[0].Address.ToString() != "0.0.0.0") return ua.Address.ToString();
						if (strFirst.Length == 0) strFirst = ua.Address.ToString();
					}
				}
				return strFirst;
			}
			catch { return ""; }
		}

		// [LGLS 2026-11-10] 제목줄에 현재 접속한 상대(Accept 한 쪽)를 보인다. null 이면 "접속 없음".
		private static void ShowPeerInTitle(string pPeer)
		{
			try
			{
				frmMain f = modDefApp.g_frmForm;
				if (f == null || f.IsDisposed) return;
				f.BeginInvoke(new System.Windows.Forms.MethodInvoker(delegate { f.SetPeerTitle(pPeer); }));
			}
			catch { }
		}

		// [LGLS 2026-09-30] 수신 스레드가 송신 쪽 DB 오류로도 끊던 종전 판정을 쓸지 (기본 0 = 쓰지 않음)
		private static bool SrvStopOnCliDbErr
		{
			get
			{
				try
				{
					string strIni = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "EcsComA.ini");
					return modDefAPI.GetPrivateProfileInt("Host", "SRV_STOP_ON_CLI_DBERR", 0, strIni) != 0;
				}
				catch { return false; }
			}
		}

		public static void SafeRun(string pName, ThreadStart pBody)
		{
			try
			{
				pBody();
			}
			catch (ThreadAbortException)
			{
				throw;		// 정지 절차(Abort)는 종전 그대로 흘려보낸다
			}
			catch (Exception ex)
			{
				try { WcsCommon.cTaskLog.Write("HOST", pName, modDefApp.MSG_ERR,
						"[스레드 예외] 이 스레드만 끝냅니다 - " + ex.ToString()); } catch { }
				try { modCmWork.ShowMsgServer("[" + pName + "] 스레드 예외 : " + ex.Message,
						modDefApp.MSG_ERR); } catch { }
				try { CrashReporter.ReportHandled(pName, ex); } catch { }   // [LGLS 2026-11-11] 호출 경로·자원·최근 로그를 CrashReport\ 에 남긴다
			}
		}

		public static void ListenThread()
		{
			string strLog = null;

			// [LGLS 2026-09-29] ★Bind 실패로 프로세스가 죽던 자리★ (이벤트 로그 확인).
			//   2026-09-22 19:16 ALL_TASK.exe
			//     System.Net.Sockets.SocketException
			//       Socket.Bind() → TcpListener.Start() ← modWorkThread.ListenThread
			//   수신 포트를 이미 다른 인스턴스가 쓰고 있으면 난다(HOST_TASK 를 두 번 띄운 경우 등).
			//   그건 프로그램을 끝낼 일이 아니라 사람이 알아야 할 상황이므로,
			//   사유를 남기고 기다렸다 다시 시도한다.
			while (modDefApp.g_blListenThread)
			{
				try
				{
					modDefApp.g_tcplsn = new TcpListener(System.Net.IPAddress.Any, modDefApp.g_iListenPort);
					modDefApp.g_tcplsn.Start();
					break;				// 열렸다
				}
				catch (SocketException se)
				{
					modCmWork.ShowMsgServer(
						"수신 포트 " + modDefApp.g_iListenPort + " 를 열지 못했습니다 - "
						+ se.Message + "(" + se.ErrorCode + "). "
						+ "이미 떠 있는 HOST_TASK 가 없는지 확인하세요. 10초 뒤 다시 시도합니다.",
						modDefApp.MSG_ERR);
					modDefAPI.SleepA(10000);
				}
			}
			if (!modDefApp.g_blListenThread) return;

			while (modDefApp.g_blListenThread)
			{
				modDefAPI.SleepA(1000);
				if (modDefApp.g_tcplsn.Pending())
				{
					// [LGLS 2026-09-30] ★현장 "접속되자마자 0.1초 만에 끊김" 의 원인★ (재현·확인).
					//   종전에는 접속 1개용 객체(g_SrvWork) 하나를 모든 접속이 같이 썼다. 붙어 있는 접속이 있는 채로
					//   새 접속이 오면 여기서 옛 소켓을 닫고 새 소켓을 같은 필드에 넣는데, 그 순간 깨어난 옛 스레드의
					//   정리 코드가 같은 필드(= 새 소켓)를 닫아 버렸다. 상위가 30초마다 접속을 겹쳐 열면 매번 그랬다.
					//   이제 접속마다 CSrvWork 를 하나씩 만들어(구 ECP TcpServer.Clients 와 같은 꼴) 제 소켓만 닫는다.
					CSrvWork w;
					bool bSingle = SrvSingleConn;   // [LGLS 2026-11-10]
					if (bSingle)
					{
						// 접속 하나만 : 새 객체에 받아 보고, 이미 살아 있는 접속이 있으면 아래에서 거부한다.
						w = new CSrvWork();
					}
					else if (SrvMultiConn)
					{
						w = new CSrvWork();
					}
					else
					{
						// 접속 1개 모드 : 이전 접속을 닫는다(종전 동작). 옛 스레드가 아직 살아 있으면 새 객체를 써서 경합을 피한다.
						w = modDefApp.g_SrvWork;
						if ((w.m_sktSock != null) | w.m_blSockConnected)
						{
							w.m_blSockConnected = false;
							modCmWork.CloseSocket(ref w.m_sktSock);
						}
						if (w.m_thrThreadObj != null && w.m_thrThreadObj.IsAlive) w = new CSrvWork();
					}

					// 소켓연결
					try
					{
						w.m_sktSock = modDefApp.g_tcplsn.AcceptSocket();
					}
					catch (SocketException se)
					{
						strLog = se.Message + "(" + se.ErrorCode.ToString() + ")";
						modCmWork.ShowMsgServer(strLog, modDefApp.MSG_ERR);
					}
					catch (Exception ex)
					{
						modCmWork.ShowMsgServer(ex.ToString(), modDefApp.MSG_ERR);
					}
					if (w.m_sktSock == null) continue;
					SetKeepAlive(w.m_sktSock);          // [LGLS 2026-11-11]
					w.m_dtLastRx = DateTime.Now;

					string strPeer = "";
					try { strPeer = PeerText(w.m_sktSock); } catch { }
					// [LGLS 2026-11-10] RemoteIP 가 아닌 IP 에서 온 접속은 받자마자 닫고 로그만 남긴다
					string strAllowIp;
					if (!IsAllowedPeer(w.m_sktSock, out strAllowIp))
					{
						modCmWork.ShowMsgServer("접속이 거부되었습니다. 허용되지 않은 IP 에서 접속을 요청한 상대 [" + strPeer + "] (허용 IP = ini RemoteIP/RemoteIP2 " + strAllowIp + ")", modDefApp.MSG_ERR);
						try { modCmWork.CloseSocket(ref w.m_sktSock); } catch { }
						continue;
					}
					// [LGLS 2026-11-10] 이미 한 접속이 살아 있으면 다른 접속은 받지 않는다 - 받자마자 닫고 로그만 남긴다.
					if (bSingle)
					{
						CSrvWork wAlive = FindAliveSrvWork();
						if (wAlive != null)
						{
							string strHave = "";
							try { strHave = PeerText(wAlive.m_sktSock); } catch { }
							// [LGLS 2026-11-11] 조용한 옛 접속 교체(SRV_STALE_SEC)는 사용자 지시로 삭제했다 - 이미 접속이 있으면 항상 거부한다.
							modCmWork.ShowMsgServer("접속이 거부되었습니다. 접속을 요청한 상대 [" + strPeer + "] (이미 접속 중인 상대 [" + strHave + "] 가 있음)", modDefApp.MSG_ERR);
							try { modCmWork.CloseSocket(ref w.m_sktSock); } catch { }
							continue;
						}
					}
					int nConn;
					lock (modDefApp.g_lstSrvWork)
					{
						if (!modDefApp.g_lstSrvWork.Contains(w)) modDefApp.g_lstSrvWork.Add(w);
						nConn = modDefApp.g_lstSrvWork.Count;
					}
					modDefApp.g_SrvWork = w;

					// [LGLS 2026-11-10] Accept 해 준 상대의 IP:포트 (내 수신 포트와 구분해서 적는다)
					strLog = string.Format("통신이 연결되었습니다.. 상대 [{0}] (내 수신 포트 {1}, 접속 {2}개)", strPeer, modDefApp.g_iListenPort, nConn);
					modCmWork.ShowMsgServer(strLog, modDefApp.MSG_IMP);
					ShowPeerInTitle(strPeer);

					// 서버 쓰레드 시작 - 이 접속의 객체를 넘긴다
					CSrvWork wRun = w;
					w.m_thrThreadObj = new Thread(delegate() { SafeRun("HOST_SRV", delegate() { SrvWorkThread(wRun); }); });
					w.m_thrThreadObj.Name = "Server Thread";
					w.m_thrThreadObj.Start();
				}
			}

			modDefApp.g_tcplsn.Stop();

			modCmWork.ShowMsgServer("Socket Listener 종료.");

            // Log 쓰레드 종료
			modDefApp.g_frmForm.LogThreadEnd();
		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 서버 작업 Thread
		// [LGLS 2026-09-30] 종전 호출 호환용 - 가장 최근 접속으로 돈다
		public static void SrvWorkThread() { SrvWorkThread(modDefApp.g_SrvWork); }

		public static void SrvWorkThread(CSrvWork w)
		{
			string strLog = null;
			bool blResult = false;
			int iBodyLen = 0;
			string strErrMsg = null;

			modDefApp.g_blSrvThread = true;
			// 설비상태가 연속으로 많은량 수신 될 떄 SKIP을 위한 처리
			w.m_tmSCMD_RecvTime = DateTime.Now.AddSeconds(-10);

			modCmWork.SetSocketCon(ref modDefApp.g_frmForm.picSrvCom, modDefApp.ComSts.ComNor);
            // DB 연결,연결이 끊어지면 소켓도 Close한다. 자동 재접속은 무한루프 및 부하증가

            if ((w.m_BDb.conMain != null))
            {
                w.m_BDb.conMain.Close();
                modDefAPI.SleepA(3000);
            }

            modCmWork.ShowMsgServer("DB 로그인 중...", modDefApp.MSG_IMP);

            if (!modCmLib.DBLogIn(ref w.m_BDb.conMain, ref strErrMsg))
            {
                w.m_blDbConnted = false;
                strLog = string.Format("DB 연결 실패-{0}", strErrMsg);
                modCmWork.ShowMsgServer(strLog, modDefApp.MSG_ERR);
            }
            else
            {
                w.m_blDbConnted = true;
                modCmWork.ShowMsgServer("DB 로그인 성공.", modDefApp.MSG_IMP);
                w.m_BDb.Init();
            }

            //While SrvWork.DbConnted
            while (modDefApp.g_blSrvThread)
			{
                try
                {
                    blResult = w.ReadRequest(ref iBodyLen);
                    if (!blResult)
                    {
                        break; // TODO: might not be correct. Was : Exit While
                    }
                    w.m_dtLastRx = DateTime.Now;   // [LGLS 2026-11-11]

                    w.Parsing(iBodyLen);
                    // Response
                    w.SendSock();

#if ORACLE
				    //--LKM[20140430]DB연결상태체크
				    if (!string.IsNullOrEmpty(modDefApp.g_CliWork.m_BDb.ErrMsg) || modDefApp.g_CliWork.m_BDb.conMain.State == ConnectionState.Closed)
				    {
					    w.m_blDbConnted = false;
                        w.m_BDb.ErrMsg = "";
                        break; // TODO: might not be correct. Was : Exit While
				    }
				    else
				    {
					    w.m_blDbConnted = true;
				    }
				    //==LKM[20140430]DB연결상태체크
#endif
#if SQL
				    // DB연결상태체크
				    // [LGLS 2026-09-30] ★현장 "접속 직후 0.1초 만에 통신 쓰레드를 종료합니다" 의 자리★ (사용자 확인).
				    //   종전에는 ★송신(Client) 쪽★ DB 객체의 오류 문구를 보고 ★수신★ 소켓을 끊었고,
				    //   지우는 대상은 수신 쪽 객체라 원인이 남아 접속마다 되풀이됐다. 상대(IMS)는
				    //   전문을 보내고 응답까지 받은 뒤 우리가 끊는 것을 30초마다 겪었다.
				    //   이제 수신 스레드는 ★자기 DB(g_SrvWork.m_BDb)★ 만 보고, 끊을 때는 사유를 남긴다.
				    //   EcsComA.ini [Host] SRV_STOP_ON_CLI_DBERR=1 이면 종전 판정(비교용). 기본 0.
				    string strSrvDbErr = w.m_BDb.ErrMsg;
				    string strCliDbErr = modDefApp.g_CliWork.m_BDb.ErrMsg;
				    bool   blStopOld   = SrvStopOnCliDbErr && !string.IsNullOrEmpty(strCliDbErr);
				    if (!string.IsNullOrEmpty(strSrvDbErr) || blStopOld)
				    {
					    w.m_blDbConnted = false;
                        w.m_BDb.ErrMsg = "";
                        modCmWork.ShowMsgServer("DB 오류로 수신을 종료합니다(재접속 대기) : "
                            + (blStopOld ? "[송신측] " + strCliDbErr : strSrvDbErr), modDefApp.MSG_ERR);
                        break; // TODO: might not be correct. Was : Exit While
				    }
				    else
				    {
					    w.m_blDbConnted = true;
				    }
				    // DB연결상태체크
#endif
#if POSTGRESSQL
                    // DB연결상태체크
                    if (!string.IsNullOrEmpty(modDefApp.g_CliWork.m_BDb.ErrMsg))
                    {
                        w.m_blDbConnted = false;
                        w.m_BDb.ErrMsg = "";
                        break; // TODO: might not be correct. Was : Exit While
                    }
                    else
                    {
                        w.m_blDbConnted = true;
                    }
                    // DB연결상태체크
#endif
                }
                catch (SocketException se)
                {
                    //strLog = se.Message & "(" & se.ErrorCode.ToString & ")"
                    strLog = "리모트 시스템과 연결 실패 !" + "(" + se.ErrorCode.ToString() + ")";
                    modCmWork.ShowMsgServer(strLog, modDefApp.MSG_IMP);
                    break; // TODO: might not be correct. Was : Exit While

                }
                catch (Exception ex)
                {
                    modCmWork.ShowMsgServer(ex.ToString(), modDefApp.MSG_ERR);
                    break; // TODO: might not be correct. Was : Exit While
                }
                

			}

			if ((w.m_BDb.conMain != null))
			{
				// DB Close
				w.m_BDb.conMain.Close();
			}
			w.ReleaseDb();   // [LGLS 2026-11-11] 접속이 끝나면 이 접속의 DB 개체를 반납한다

			w.m_blSockConnected = false;
			modCmWork.CloseSocket(ref w.m_sktSock);          // ★제 소켓만★ 닫는다

			// Remote 에서 Close 할 시( Remote Close), return 0 시 로그
			if (!blResult)
			{
				strLog = "리모트 시스템과 연결을 종료합니다.";
				modCmWork.ShowMsgServer(strLog, modDefApp.MSG_IMP);
			}

			// [LGLS 2026-09-30] 목록에서 빼고, 남은 접속이 하나도 없을 때만 신호등을 끈다
			int nLeft = 0;
			lock (modDefApp.g_lstSrvWork)
			{
				modDefApp.g_lstSrvWork.Remove(w);
				foreach (CSrvWork x in modDefApp.g_lstSrvWork)
					if (x.m_sktSock != null && x.m_sktSock.Connected) nLeft++;
			}
			strLog = "통신 쓰레드를 종료합니다." + (nLeft > 0 ? " (남은 접속 " + nLeft + "개)" : "");
			modCmWork.ShowMsgServer(strLog, modDefApp.MSG_IMP);
			if (nLeft == 0) ShowPeerInTitle(null);   // [LGLS 2026-11-10] 제목줄 "접속 없음"
			//소켓 연결종료표시
			if (nLeft == 0)
				modCmWork.SetSocketCon(ref modDefApp.g_frmForm.picSrvCom, modDefApp.ComSts.ComErr);

            //--LKM[20140509]프로그램종료시 서버쓰레드종료 후 리슨쓰레드종료
            if (modDefApp.g_blSrvThread == false)
            {
                modDefApp.g_blListenThread = false;
            }
        }

        //최초작성자	: BASE(이길문)
        //작성일		: 20160829
        //설명		: 로그 Thread
        public static void LogThread()
		{
			int iDelCnt = 0;
			int iID = 0;
			CLog Log = default(CLog);
			LogMsgInfo LogMsg1, LogMsg2;

			//----------------------------------
			// Log Dir이 없으면 생성함
			//----------------------------------
			System.IO.DirectoryInfo diLog = new System.IO.DirectoryInfo(modDefApp.LOG_DIR);
			if (!diLog.Exists)
				diLog.Create();
			diLog = null;

			iID = Convert.ToInt32(Thread.CurrentThread.Name.Substring(0, 3));
			if (iID == 0)
			{
				Log = new CLog("Cli\\\\");
			}
			else
			{
				Log = new CLog("Srv\\\\");
			}


			while (!modDefApp.g_areLogExitEvent[iID].WaitOne(1000, false))
			{
				while ((modDefApp.g_arrlstLogList[iID].Count > 0))
				{
					//20170919 권혁찬 Object형식으로는 파라메터를 넘길 수 없어 변수에 캐스팅 후 값을 넘김.
					LogMsg1 = (LogMsgInfo)modDefApp.g_arrlstLogList[iID][0];
					Log.WriteLog(ref LogMsg1);
					modDefApp.g_arrlstLogList[iID].RemoveAt(0);
				}

				//삭제
				iDelCnt += 1;
				if (iDelCnt > 3600)
				{
					modCmLib.DelLog();
					iDelCnt = 0;
				}

			}

			if (iID == 0)
			{
				modCmWork.ShowMsgClient("Write Log Thread 종료.");
			}
			else
			{
				modCmWork.ShowMsgServer("Write Log Thread 종료.");
			}

			//이전로그 기록
			while ((modDefApp.g_arrlstLogList[iID].Count > 0))
			{
				//20170919 권혁찬 Object형식으로는 파라메터를 넘길 수 없어 변수에 캐스팅 후 값을 넘김.
				LogMsg2 = (LogMsgInfo)modDefApp.g_arrlstLogList[iID][0];
				Log.WriteLog(ref LogMsg2);
				modDefApp.g_arrlstLogList[iID].RemoveAt(0);
			}

		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 클라이언트 작업 Thread
		public static void CliWorkThread()
		{
			string strLog = null;
			int iCnt = 0;
			string strErrMsg = null;

			while (!modDefApp.g_CliWork.m_areCliExitEvent.WaitOne(1000, false))
			{
				try
				{
					//--LKM[20140430]DB연결
					if (modDefApp.g_CliWork.m_blDbConnted == false)
					{
						modCmWork.ShowMsgClient("DB 로그인 중...", modDefApp.MSG_IMP);

						if (!modCmLib.DBLogIn(ref modDefApp.g_CliWork.m_BDb.conMain, ref strErrMsg))
						{
							modDefApp.g_CliWork.m_blDbConnted = false;
							strLog = string.Format("DB 연결 실패-{0}", strErrMsg);
							modCmWork.ShowMsgClient(strLog, modDefApp.MSG_ERR);
							modDefAPI.SleepA(3000);
                            continue;
						}
						else
						{
							modDefApp.g_CliWork.m_blDbConnted = true;
							modCmWork.ShowMsgClient("DB 로그인 성공.", modDefApp.MSG_IMP);
							modDefApp.g_CliWork.m_BDb.Init();
						}
					}
					//==LKM[20140430]DB연결

                    if (!modDefApp.g_CliWork.m_blSockConnected && modDefApp.g_frmForm.chkSimMode.Checked == false)
					{
						iCnt += 1;
						// 3초
						if (iCnt == 3)      // 15
						{
							iCnt = 0;
							modDefApp.g_CliWork.ConnectSock();
						}
					}

					if (modDefApp.g_blSTOP_REQ == false)
					{
						modDefApp.g_CliWork.GetSendData();
					}


#if ORACLE
					//--LKM[20140430]DB연결상태체크
					if (!string.IsNullOrEmpty(modDefApp.g_CliWork.m_BDb.ErrMsg) || modDefApp.g_CliWork.m_BDb.conMain.State == ConnectionState.Closed)
					{
						modDefApp.g_CliWork.m_blDbConnted = false;
					}
					else
					{
						modDefApp.g_CliWork.m_blDbConnted = true;
					}
					//==LKM[20140430]DB연결상태체크
#endif
#if SQL
					// DB연결상태체크
					if (!string.IsNullOrEmpty(modDefApp.g_CliWork.m_BDb.ErrMsg))
					{
						modDefApp.g_CliWork.m_blDbConnted = false;
					}
					else
					{
						modDefApp.g_CliWork.m_blDbConnted = true;
					}
					// DB연결상태체크
#endif
#if POSTGRESSQL
                    // DB연결상태체크
					if (!string.IsNullOrEmpty(modDefApp.g_CliWork.m_BDb.ErrMsg))
					{
						modDefApp.g_CliWork.m_blDbConnted = false;
					}
					else
					{
						modDefApp.g_CliWork.m_blDbConnted = true;
					}
					// DB연결상태체크
#endif


                }
				catch (Exception ex)
				{
					modCmWork.ShowMsgClient(ex.Message, modDefApp.MSG_ERR);
					modDefApp.g_CliWork.m_blDbConnted = false;
				}

				//--LKM[20140430]DB연결해제, 통신종료
				if (modDefApp.g_CliWork.m_blDbConnted == false)
				{
					modDefApp.g_CliWork.m_BDb.ErrMsg = "";
					modCmWork.ShowMsgClient("이상 발생.DB Logout...", modDefApp.MSG_IMP);
					if ((modDefApp.g_CliWork.m_BDb.conMain != null))
					{
						// DB Close
						modDefApp.g_CliWork.m_BDb.conMain.Close();
					}

					modDefApp.g_CliWork.m_blSockConnected = false;
					modCmWork.CloseSocket(ref modDefApp.g_CliWork.m_sktSock);

					strLog = "통신 쓰레드를 종료합니다.";
					modCmWork.ShowMsgClient(strLog, modDefApp.MSG_IMP);
					//소켓 연결종료표시
					modCmWork.SetSocketCon(ref modDefApp.g_frmForm.picCliCom, modDefApp.ComSts.ComErr);

					modDefAPI.SleepA(3000);
				}
				// DB연결해제, 통신종료
			}

			modDefApp.g_CliWork.m_blSockConnected = false;
			modCmWork.CloseSocket(ref modDefApp.g_CliWork.m_sktSock);

			strLog = "통신 쓰레드를 종료합니다.";
			modCmWork.ShowMsgClient(strLog, modDefApp.MSG_IMP);
			//소켓 연결종료표시
			modCmWork.SetSocketCon(ref modDefApp.g_frmForm.picCliCom, modDefApp.ComSts.ComErr);
		}
	}
}
