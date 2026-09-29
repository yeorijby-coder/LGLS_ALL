using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace TSK_HostCom
{
	static class modCmWork
	{
		//*** 메세지 표시 ***
		public static void ShowMsgClient(string p_strMsg, string p_strMsgKind = modDefApp.MSG_NOR, bool p_blWriteLog = true, [CallerFilePath] string p_strFile = "", [CallerMemberName] string p_strFunc = "")
		{
			// [LGLS 2026-09-09] ALL_TASK 공용 로그 싱크(스레드별 파일 + DB)
			try { WcsCommon.cTaskLog.Write("HOST", "HOST_CLI", p_strMsgKind,
					WcsCommon.cLogCols.ShortFile(p_strFile) + "::" + p_strFunc + " " + p_strMsg); } catch { }

            try
            {
                LogMsgInfo LogMsg = default(LogMsgInfo);

                //임시
                if (!modDefApp.g_frmForm.chkStatus.Checked & p_strMsgKind == modDefApp.MSG_NOR)
                {
                    if (p_strMsg.Length > modDefApp.MSG_HEAD_CNT + 2)
                    {
                        if (p_strMsg.Substring(modDefApp.MSG_HEAD_CNT + 1, 1).ToUpper().Equals("S"))
                        {
                            return;
                        }
                    }
                } 
                
                LogMsg.g_strTime = string.Format(@"{0:HH:mm:ss}", DateTime.Now) + ":" + string.Format(@"{0:000}", DateTime.Now.Millisecond);
                LogMsg.g_strType = p_strMsgKind;
                LogMsg.g_strMsg = p_strMsg;
                if (p_blWriteLog)
                {
                    modDefApp.g_arrlstLogList[0].Add(LogMsg);
                }

                if (modDefApp.g_blIsAppExit | modDefApp.g_frmForm.chkStopLogClient.Checked)
                {
                    return;
                }

                ListViewItem lvitem = new ListViewItem(LogMsg.g_strTime, 0);

                // [LGLS 2026-08-21] 호출 위치 (내용 앞 2열 - 헤더 우클릭으로 표시/숨김)
                lvitem.SubItems.Add(WcsCommon.cLogCols.ShortFile(p_strFile));
                lvitem.SubItems.Add(p_strFunc ?? "");
                lvitem.SubItems.Add(p_strMsg);

                // [LGLS] 통신 전문(원문) 판별: STX 포함 또는 메일박스 헤더로 시작하면 전문
                bool bCommMsg = p_strMsg.IndexOf((char)0x02) >= 0
                             || p_strMsg.StartsWith(modDefApp.MSG_MAILBOX_SEND)
                             || p_strMsg.StartsWith(modDefApp.MSG_MAILBOX_RECV);

                if (bCommMsg)
                {
                    // 통신 메세지는 기존 색상 규칙 그대로
                    switch (p_strMsgKind)
                    {
                        case modDefApp.MSG_IMP:
                            lvitem.BackColor = Color.Blue;
                            lvitem.ForeColor = Color.White;
                            break;
                        case modDefApp.MSG_ERR:
                            lvitem.BackColor = Color.Red;
                            lvitem.ForeColor = Color.White;
                            break;
                    }
                }
                else
                {
                    // [LGLS] 로그성 메세지는 IMP 와 동일한 스타일(파란 배경 + 흰 글씨)로 표시
                    //        (에러는 시인성 위해 기존 빨강 유지)
                    if (p_strMsgKind == modDefApp.MSG_ERR)
                    {
                        lvitem.BackColor = Color.Red;
                        lvitem.ForeColor = Color.White;
                    }
                    else
                    {
                        lvitem.BackColor = Color.Blue;
                        lvitem.ForeColor = Color.White;
                    }
                }

                modDefApp.g_frmForm.lsvMsgClient.Items.Add(lvitem);

                if (modDefApp.g_frmForm.lsvMsgClient.Items.Count > 50)
                {
                    modDefApp.g_frmForm.lsvMsgClient.Items.Clear();
                }

                if (modDefApp.g_frmForm.chkShowClient.Checked)
                {
                    modDefApp.g_frmForm.lsvMsgClient.EnsureVisible(modDefApp.g_frmForm.lsvMsgClient.Items.Count - 1);
                }

                if (p_strMsgKind != modDefApp.MSG_NOR)
                {
                    modDefApp.g_frmForm.lsvMsgClient.Refresh();
                }
            }
            catch(Exception Ex)
            {

            }
			
		}


		public static void ShowMsgServer(string p_strMsg, string p_strMsgKind = modDefApp.MSG_NOR, bool p_blWriteLog = true, [CallerFilePath] string p_strFile = "", [CallerMemberName] string p_strFunc = "")
		{
			// [LGLS 2026-09-09] ALL_TASK 공용 로그 싱크(스레드별 파일 + DB)
			try { WcsCommon.cTaskLog.Write("HOST", "HOST_SRV", p_strMsgKind,
					WcsCommon.cLogCols.ShortFile(p_strFile) + "::" + p_strFunc + " " + p_strMsg); } catch { }

            try
            {
                LogMsgInfo LogMsg = default(LogMsgInfo);

                //임시
                if (!modDefApp.g_frmForm.chkStatus.Checked & p_strMsgKind == modDefApp.MSG_NOR)
                {
                    if (p_strMsg.Length > modDefApp.MSG_HEAD_CNT + 2)
                    {
                        if (p_strMsg.Substring(modDefApp.MSG_HEAD_CNT + 1, 1).ToUpper().Equals("S"))
                        {
                            return;
                        }
                    }
                }

                LogMsg.g_strTime = string.Format(@"{0:HH:mm:ss}", DateTime.Now) + ":" + string.Format(@"{0:000}", DateTime.Now.Millisecond);
                LogMsg.g_strType = p_strMsgKind;
                LogMsg.g_strMsg = p_strMsg;
                if (p_blWriteLog)
                {
                    modDefApp.g_arrlstLogList[1].Add(LogMsg);
                }

                if (modDefApp.g_blIsAppExit | modDefApp.g_frmForm.chkStopLogServer.Checked)
                {
                    return;
                }

                ListViewItem lvitem = new ListViewItem(LogMsg.g_strTime, 0);

                // [LGLS 2026-08-21] 호출 위치 (내용 앞 2열 - 헤더 우클릭으로 표시/숨김)
                lvitem.SubItems.Add(WcsCommon.cLogCols.ShortFile(p_strFile));
                lvitem.SubItems.Add(p_strFunc ?? "");
                lvitem.SubItems.Add(p_strMsg);
                switch (p_strMsgKind)
                {
                    case modDefApp.MSG_IMP:
                        lvitem.BackColor = Color.Blue;
                        lvitem.ForeColor = Color.White;
                        break;
                    case modDefApp.MSG_ERR:
                        lvitem.BackColor = Color.Red;
                        lvitem.ForeColor = Color.White;
                        break;
                }

                modDefApp.g_frmForm.lsvMsgServer.Items.Add(lvitem);

                if (modDefApp.g_frmForm.lsvMsgServer.Items.Count > 50)
                {
                    modDefApp.g_frmForm.lsvMsgServer.Items.Clear();
                }

                if (modDefApp.g_frmForm.chkShowServer.Checked)
                {
                    modDefApp.g_frmForm.lsvMsgServer.EnsureVisible(modDefApp.g_frmForm.lsvMsgServer.Items.Count - 1);
                }
                if (p_strMsgKind != modDefApp.MSG_NOR)
                {
                    modDefApp.g_frmForm.lsvMsgServer.Refresh();
                }

                modDefApp.g_frmForm.lsvMsgServer.Refresh();

            }
            catch (Exception Ex)
            {

            }
			
		}

		public static void CloseSocket(ref System.Net.Sockets.Socket p_sktSocket)
		{
			// 소켓을 ManualClose 시키면 쓰레드가 종료하면서 다시 closesocket을 호출하면서 동기화가 이루어지지 않는다.
			// [LGLS 2026-09-29] ★여기가 현장 CLR20r3 의 실제 자리다★ (이벤트 로그 확인).
			//   2026-09-28 16:03 TASK_LFC10_G1_ECSCOM.exe
			//     System.Net.Sockets.SocketException
			//       Socket.Shutdown() ← modCmWork.CloseSocket ← modWorkThread.CliWorkThread
			//   Connected 는 "마지막 I/O 시점의 상태" 라, 확인과 Shutdown 사이에 상대가 끊으면
			//   그대로 예외가 난다. 상대가 먼저 끊는 것은 통신에서 늘 있는 일이지 고장이 아니다.
			//   또 Monitor.Enter/Exit 가 try/finally 로 감싸여 있지 않아, 예외가 나면
			//   ★락이 영영 풀리지 않는다★ - 죽지 않더라도 이후 소켓 종료가 모두 멈춘다.
			System.Threading.Monitor.Enter(modDefApp.g_objSockSync);
			try
			{
				if ((p_sktSocket != null))
				{
					// 이미 끊긴 소켓의 Shutdown 실패는 정상적인 상황이다 - 닫기만 하면 된다
					try
					{
						if (p_sktSocket.Connected)
							p_sktSocket.Shutdown(System.Net.Sockets.SocketShutdown.Both);
					}
					catch (System.Net.Sockets.SocketException) { }
					catch (ObjectDisposedException) { }

					try { p_sktSocket.Close(); }
					catch (Exception) { }

					p_sktSocket = null;// 프로그램내에서 강제종료시 소켓 객체 사용여부 판단
				}
			}
			finally
			{
				System.Threading.Monitor.Exit(modDefApp.g_objSockSync);
			}
		}

		public static void CloseSocket(ref System.Net.Sockets.TcpClient p_tcpcli, ref System.Net.Sockets.NetworkStream p_ntstrm)
		{
			if ((p_ntstrm != null))
			{
				p_ntstrm.Close();
				p_tcpcli.Close();
				p_ntstrm = null;// 프로그램내에서 강제종료시 소켓 객체 사용여부 판단
				p_tcpcli = null;
			}
		}

		// Socket 연결종료표시
		// [LGLS 2026-08-31] ★종료 중 경합으로 프로세스가 죽던 자리★
		//   통신 스레드가 연결 상태를 아이콘으로 표시하는데, 폼이 이미 정리(Dispose)된 뒤에
		//   호출되면 ImageList.Images 가 비어 ArgumentOutOfRangeException 이 난다.
		//   그 예외가 미처리로 올라가 ★TASK_LFC10_G1_ECSCOM.exe 가 강제 종료★됐다.
		//   (이벤트로그 : System.Windows.Forms.ImageList+ImageCollection.get_Item → SetSocketCon)
		//   실측 : 상위(HOST_SIM)를 내리자 10061 재연결을 반복하다 종료 시점에 크래시.
		//   표시가 실패해도 통신은 계속되어야 한다 - 조용히 넘긴다.
		public static void SetSocketCon(ref PictureBox p_picObj, modDefApp.ComSts p_Status)
		{
			try
			{
				if (p_picObj == null || p_picObj.IsDisposed) return;
				if (modDefApp.g_frmForm == null || modDefApp.g_frmForm.IsDisposed) return;

				ImageList iml = modDefApp.g_frmForm.imlConnect;
				if (iml == null || iml.Images == null) return;

				int nIdx = Convert.ToInt32(p_Status);
				if (nIdx < 0 || nIdx >= iml.Images.Count) return;

				p_picObj.Image = iml.Images[nIdx];
			}
			catch { }   // 표시 실패로 통신 프로세스를 죽이지 않는다
		}
	}
}
