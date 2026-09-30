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
					if (SrvMultiConn)
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

					string strPeer = "";
					try { strPeer = w.m_sktSock.RemoteEndPoint.ToString(); } catch { }
					int nConn;
					lock (modDefApp.g_lstSrvWork)
					{
						if (!modDefApp.g_lstSrvWork.Contains(w)) modDefApp.g_lstSrvWork.Add(w);
						nConn = modDefApp.g_lstSrvWork.Count;
					}
					modDefApp.g_SrvWork = w;

					strLog = string.Format("통신이 연결되었습니다.. [{0}] (접속 {1}개)", strPeer, nConn);
					modCmWork.ShowMsgServer(strLog, modDefApp.MSG_IMP);

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
