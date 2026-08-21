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
			System.Threading.Monitor.Enter(modDefApp.g_objSockSync);
			if ((p_sktSocket != null))
			{
				if (p_sktSocket.Connected)
				{
					p_sktSocket.Shutdown(System.Net.Sockets.SocketShutdown.Both);
				}
				p_sktSocket.Close();
				p_sktSocket = null;// 프로그램내에서 강제종료시 소켓 객체 사용여부 판단
			}
			System.Threading.Monitor.Exit(modDefApp.g_objSockSync);
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
		public static void SetSocketCon(ref PictureBox p_picObj, modDefApp.ComSts p_Status)
		{
			p_picObj.Image = modDefApp.g_frmForm.imlConnect.Images[Convert.ToInt32(p_Status)];
		}
	}
}
