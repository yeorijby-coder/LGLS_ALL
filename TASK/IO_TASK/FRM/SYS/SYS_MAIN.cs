//프로그램작성자	: 주영기
//작성일		: 20190722
//화면개요	    : 입/출고 스케쥴러 작업처리
//변경이력	    : 이승범 2019.09/19 S/C의 작업처리상태 LEVEL정보를 해당 상태에 맞게 변경.
//              : 2019.11.01 입고분과 변경추가.
//              : 2019.11.02 화면감시기능 변경추가. / FireRet = 8 (추가완료) 상태값..
//              : 2019.11.12 입고분과 빈 셀에러 발생 시 CELL_STA에 MARKING 후 화면감시에서 상태를 자동으로 처리하도록 보완.

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.CompilerServices;

using System.Threading;
using System.Net.Sockets;
using System.Data.OleDb;
using Samoh_Lib;
using log4net;
using log4net.Config;
using System.Collections;
using System.Diagnostics;


namespace TSK_COMM_IOSCH
{
    public delegate void PsMsgView(string pMsg, string pObjID, string pCommTyp, string pTgm, int nId, cDefApp.eLogMsgType pMsgTyp, string pFile, string pFunc);
	public delegate void DelPsMsgLog(DateTime LogDate, string strMsg, int nId);

	public partial class SYS_MAIN : Form
	{
		private string m_StrConnecString; //DataBase Connection 문자열.
        // LG생명과학 자동 반송 Scheduler THREAD
        // (기존 설비 스레드 cThread_CV/cThread_SC/cThread_R 은 현장 구성에서 제외됨 - 참고용으로만 보존)
        private cThread_SCH[] m_Thread_SCH = new cThread_SCH[1];

        private cLogThread[] m_thLogging = new cLogThread[200]; //Thread 객체.

		private MainClass m_Maindefine = new MainClass();
		private string m_strRtnMsg = ""; //리턴 문자열.
        private Object thisLock = new object();

		#region@@@.생성자
		public SYS_MAIN()
		{
			InitializeComponent();
		}
		#endregion

		#region[Event]SYS_MAIN_Load
		private void SYS_MAIN_Load(object sender, EventArgs e)
		{
            //MainThread가 시작되었다는것을 나타내는 Bool값.
			cDefApp.GM_STAT_MAIN = true;


			// [LGLS 2026-08-21] 로그 헤더 우클릭 → 열 표시/숨김 메뉴
			WcsCommon.cLogCols.Attach(lsvR);

            //중복실행을 방지하는 함수.
            if (cCmLib.GfPrevInstance() == true)
            {
                cDefApp.GM_RE_START = true;
                Application.Exit();
            }

           // this.Text = Process.GetCurrentProcess().ProcessName;

            //설정파일 불러오기
#if ORACLE
            cDefApi.GsGetInitPorFileDB(ref cDefApp.GM_DB_PROVIDER, ref cDefApp.GM_DB_ALIAS, ref cDefApp.GM_DB_USERID, ref cDefApp.GM_DB_PASSWORD, ref cDefApp.GM_LOG_PATH, ref cDefApp.GM_FILENAME, ref m_strRtnMsg);
            m_StrConnecString = "Provider=" + cDefApp.GM_DB_PROVIDER + "; Data Source=" + cDefApp.GM_DB_ALIAS + "; User ID=" + cDefApp.GM_DB_USERID + "; Password =" + cDefApp.GM_DB_PASSWORD;
#endif
#if POSTGRESQL
            cDefApi.GsGetInitPorFilePDB(ref cDefApp.GM_PDB_IP, ref cDefApp.GM_PDB_PORT, ref cDefApp.GM_PDB_DATABASE, ref cDefApp.GM_PDB_USER, ref cDefApp.GM_PDB_USER_PW, ref cDefApp.GM_LOG_PATH, ref cDefApp.GM_FILENAME, ref m_strRtnMsg);
            m_StrConnecString = "Server=" + cDefApp.GM_PDB_IP + ";Port=" + cDefApp.GM_PDB_PORT + ";User ID=" + cDefApp.GM_PDB_USER + ";Password=" + cDefApp.GM_PDB_USER_PW + ";Database=" + cDefApp.GM_PDB_DATABASE + ";MaxPoolSize=50;SSL=false;";
#endif
#if SQL
            // MS-SQL 연결정보는 PostgreSQL과 동일하게 ENV_IOSCH.INI [P_DB] 에서 읽는다.
            //   IP=Server(예: localhost\SQLEXPRESS), DATABASE=DB명, USER/USER_PW=SQL 로그인
            cDefApi.GsGetInitPorFilePDB(ref cDefApp.GM_PDB_IP, ref cDefApp.GM_PDB_PORT, ref cDefApp.GM_PDB_DATABASE, ref cDefApp.GM_PDB_USER, ref cDefApp.GM_PDB_USER_PW, ref cDefApp.GM_LOG_PATH, ref cDefApp.GM_FILENAME, ref m_strRtnMsg);
            m_StrConnecString = "Server=" + cDefApp.GM_PDB_IP + ";Database=" + cDefApp.GM_PDB_DATABASE + ";User ID=" + cDefApp.GM_PDB_USER + ";Password=" + cDefApp.GM_PDB_USER_PW + ";";
#endif
            // [LGLS 2026-08-21] 접속 DB 정보를 타이틀에 표시 (CV TASK 와 동일 형식)
            try
            {
#if ORACLE
                this.Text = this.Text + " [DB:" + cDefApp.GM_DB_ALIAS + "]";
#else
                this.Text = this.Text + " [DB:" + cDefApp.GM_PDB_DATABASE + "@" + cDefApp.GM_PDB_IP + "]";
#endif
            }
            catch { }

			//Main에서 동작중 상태를 나타내기위해 표시.
            // Scheduler 상태 LED (picDbCn0)
            SetVisable(pnlTop, 0, "picDbCn0", "Scheduler STATUS");
            SetDisplay(pnlTop, 0, "picDbCn0", "D");

			WrkThStart();   // @.스레드 시작
			Thread_Timer.Enabled = true;
		}


		#endregion

		#region Thread 동작상태를 가져온다.
		private void WrkThStart()
		{
			//CrossThread Check False.
			CheckForIllegalCrossThreadCalls = false;

            // LG생명과학 자동 반송 Scheduler THREAD (단일 스케줄러)
            int schId = (int)cDefApp.eThGbn.SCH_GR01;
            m_Thread_SCH[0] = new cThread_SCH(schId);
            m_Thread_SCH[0].ConnectionString = m_StrConnecString;
            m_Thread_SCH[0].callPsMsgView = new PsMsgView(PsMsgView);

            // Scheduler 로그 큐 및 로깅 스레드 초기화
            cDefApp.m_LogQ[schId] = new Queue<LogParam>();
            m_thLogging[schId] = new cLogThread(cDefApp.GM_LOG_PATH
                                              , cDefApp.GM_FILENAME + getFileName(schId)
                                              , schId);

            m_Thread_SCH[0].m_Thread = new Thread(new ParameterizedThreadStart(m_Thread_SCH[0].Thread_Doing));
            m_Thread_SCH[0].m_Thread.IsBackground = true;
            m_Thread_SCH[0].m_Thread.Start(schId);

		}
		#endregion

		/*
		 * 화면 상태 표시 관련
		 */
		#region SetVisable, SetDisplay
		private void SetVisable(Panel obj, int ii, string ctrName, string tipname)
		{
			Control ctrl;
			PictureBox FindPictureBox = null;

			ctrl = m_Maindefine.PfCtlFind(ref obj, ctrName);
			if (ctrl == null)
			{
				return;
			}

			FindPictureBox = ctrl as PictureBox;
			this.ToolTip.SetToolTip(FindPictureBox, tipname);
			FindPictureBox.Visible = true;
		}
		private void SetDisplay(Panel obj, int ii, string ctrName, params string[] opt)
		{
			Control ctrl;
			PictureBox FindPictureBox = null;


			ctrl = m_Maindefine.PfCtlFind(ref obj, ctrName);
			if (ctrl == null)
			{
				return;
			}

			FindPictureBox = ctrl as PictureBox;

			if (opt.Length == 1)
				PfSetStatImgView(FindPictureBox, opt[0]);
			else
				PfSetStatImgView(FindPictureBox, opt[0], opt[1]);
		}
		private void SetDisplay(Panel obj,int ii, string ctrName, string opt)
		{
			Control ctrl;
			PictureBox FindPictureBox = null;

			ctrl = m_Maindefine.PfCtlFind(ref obj, ctrName);
			if (ctrl == null)
			{
				return;
			}

			FindPictureBox = ctrl as PictureBox;

			PfSetStatImgView(FindPictureBox, opt);
		}
		#endregion SetVisable, SetDisplay

		/*
         * Thread Timer
         */
		#region Thread_Tick
		private void Thread_Tick(object sender, EventArgs e)
		{
			Thread_Timer.Enabled = false;
			Thread_Timer.Interval = 5000;

			try
			{
				#region IOTASK
                // --- LG생명과학 자동 반송 Scheduler 상태 표시 ---
                int schId = (int)cDefApp.eThGbn.SCH_GR01;
                if (m_Thread_SCH[0] == null) { Thread_Timer.Enabled = true; return; }

                if (m_Thread_SCH[0].m_Thread == null || !m_Thread_SCH[0].m_Thread.IsAlive)
                {
                    //@@.스레드 상태표시[C:연결, T:시도, D:비연결]
                    SetDisplay(pnlTop, 0, "picDbCn0", "T");
                }
                else
                {
                    SetDisplay(pnlTop, 0, "picDbCn0", "C");
                }

                // Scheduler 로깅 스레드 기동
                if (m_thLogging[schId] != null && m_thLogging[schId].m_thThread == null)
                {
                    m_thLogging[schId].m_thThread = new Thread(m_thLogging[schId].LogQueThread);
                    m_thLogging[schId].m_thThread.IsBackground = true;
                    m_thLogging[schId].m_thThread.Start();
                    Thread.Sleep(10);
                }
				#endregion

                long mem = GC.GetTotalMemory(true);
                Console.WriteLine("MAIN-Current Memory : {0}", mem);
                //GC.Collect(0, GCCollectionMode.Forced);

			}
			catch (Exception ex)
			{
			}

			//다시 타이머 Enable을 true로 한다.
			Thread_Timer.Enabled = true;
		}
		#endregion Thread_Tick

        public string getFileName(int nThNo)
        {
            string strLogName = "";

            switch (nThNo)
            {
                //case (int) cDefApp.eThGbn.R_GR01:
                //    strLogName = "RETRY_JOB";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR01:
                //    strLogName = "CONVEYOR1F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR02:
                //    strLogName = "CONVEYOR2F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR03:
                //    strLogName = "CONVEYOR3F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR04:
                //    strLogName = "CONVEYOR4F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR05:
                //    strLogName = "CONVEYOR5F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR06:
                //    strLogName = "CONVEYOR6F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR07:
                //    strLogName = "CONVEYOR7F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR08:
                //    strLogName = "CONVEYOR8F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR09:
                //    strLogName = "CONVEYOR9F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR10:
                //    strLogName = "CONVEYOR10F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR11:
                //    strLogName = "CONVEYOR11F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR12:
                //    strLogName = "CONVEYOR12F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR13:
                //    strLogName = "CONVEYOR13F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR14:
                //    strLogName = "CONVEYOR14F";
                //    break;
                //case (int)cDefApp.eThGbn.CV_GR15:
                //    strLogName = "CONVEYOR15F";
                //    break;
                //case (int)cDefApp.eThGbn.SC_GR22:
                //    strLogName = "STACKER CRANE";
                //    break;
                case (int)cDefApp.eThGbn.SCH_GR01:
                    strLogName = "SCHEDULER";
                    break;
                default:
                    strLogName = "NO_DEFINE";
                    break;
            }

            return strLogName;
        }

		#region[Method] @@@.스레드 상태를 화면에 표시
		private bool PfSetStatImgView(PictureBox pPic,
										  string pStatSkt,
										  string pStatOp)
		{
			// @.Stat Connection : C:연결, T:시도, D:비연결
			// @.Stat Operation : N:정상, W:대기, E:에러
			try
			{
				switch (pStatSkt + pStatOp)
				{
					case "CN": if (pPic.Tag.ToString() != "0") pPic.Image = this.imgLstStat.Images[0]; pPic.Tag = "0"; break;
					case "CW": if (pPic.Tag.ToString() != "1") pPic.Image = this.imgLstStat.Images[1]; pPic.Tag = "1"; break;
					case "CE": if (pPic.Tag.ToString() != "2") pPic.Image = this.imgLstStat.Images[2]; pPic.Tag = "2"; break;
					case "TN": if (pPic.Tag.ToString() != "3") pPic.Image = this.imgLstStat.Images[3]; pPic.Tag = "3"; break;
					case "TW": if (pPic.Tag.ToString() != "4") pPic.Image = this.imgLstStat.Images[4]; pPic.Tag = "4"; break;
					case "TE": if (pPic.Tag.ToString() != "5") pPic.Image = this.imgLstStat.Images[5]; pPic.Tag = "5"; break;
					case "DN": if (pPic.Tag.ToString() != "6") pPic.Image = this.imgLstStat.Images[6]; pPic.Tag = "6"; break;
					case "DW": if (pPic.Tag.ToString() != "7") pPic.Image = this.imgLstStat.Images[7]; pPic.Tag = "7"; break;
					case "DE": if (pPic.Tag.ToString() != "8") pPic.Image = this.imgLstStat.Images[8]; pPic.Tag = "8"; break;
					default: break;
				}
				return true;
			}
			catch (Exception ex)
			{
				string msg;
				msg = ex.Message;
			}
			return false;
		}
		#endregion

		#region[Method] @@@.DB연결 상태를 화면에 표시
		private bool PfSetStatImgView(PictureBox pPic,
									  string pStatDbCn)
		{
			// @.Stat Connection : C:연결, T:시도, D:비연결

			try
			{
				switch (pStatDbCn)
				{
					case "C": if (pPic.Tag.ToString() != "0") pPic.Image = this.ImgLstBkgStat.Images[0]; pPic.Tag = "0"; break;
					case "T": if (pPic.Tag.ToString() != "1") pPic.Image = this.ImgLstBkgStat.Images[1]; pPic.Tag = "1"; break;
					case "D": if (pPic.Tag.ToString() != "2") pPic.Image = this.ImgLstBkgStat.Images[2]; pPic.Tag = "2"; break;
					default: break;
				}

				return true;
			}
			catch (Exception ex)
			{
				string msg;
				msg = ex.Message;
			}
			return false;
		}
		#endregion

		#region@@@.ListView에 로그[PsMsgView();]
		// @@@.대리자 선언
		delegate void DelegateListViewItem(ListViewItem item, int nId);

		// @@@.Client 메세지 Listview Invoke 선언
		private void PsSetMsg(ListViewItem item,int nId)
		{
			try
			{
				string strCtrlName = "";
                if (nId == (int)cDefApp.eThGbn.SCH_GR01) // LG생명과학 자동 반송 Scheduler
                    strCtrlName = "lsvR";
				else
					strCtrlName = "";

				Control Ctrl = m_Maindefine.PfCtlFind1(splitContainer1.Panel1, strCtrlName);

				if (Ctrl == null) return;

				ListView lstView = (ListView)Ctrl;

				if (lstView.InvokeRequired == true)
				{
					DelegateListViewItem d = new DelegateListViewItem(this.PsSetMsg); // SetListview
					this.Invoke(d, item, nId);
				}
				else
				{
                    
					lstView.Items.Add(item);
					if (lstView.Items.Count > 500)
					{
						lstView.Items.RemoveAt(0);
					}
					if (this.chkShow.Checked == true)
					{
						lstView.EnsureVisible(lstView.Items.Count - 1);
					}
				}
				return;
			}
			catch (Exception ex)
			{
				//MessageBox.Show(ex.Message);
			}
		}

		private void PsMsgView(string pMsg,
							   string pObjID,
							   string pCommTyp,
							   string pTgm,
							   int nId,
				  cDefApp.eLogMsgType pMsgTyp,
							   string pFile,
							   string pFunc)
		{
			try
			{

				if (chkStopLog.Checked) return;

				cDefApp.stutLogMsgInfo LogMsg;
				LogMsg.Time = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss:ffffff");
				// (작업상태 표기는 아래 JobStatusText 참고 - 숫자만으로는 뜻을 알 수 없어 [번호]내용 으로 적는다)
				LogMsg.MsgTyp = pMsgTyp.ToString();
				LogMsg.ID = pObjID;
				LogMsg.Com = pCommTyp;
				LogMsg.Msg = pMsg;
				LogMsg.Tgm = pTgm;
				if (chkStopLog.Checked) return;
				// [LGLS 2026-08-23] 메시지에서 구분/작업번호/작업상태를 뽑아 별도 열로 보낸다.
				//   맨 앞의 [SCH] 는 모든 줄에 붙어 정보가 없으므로 떼어낸다.
				string strKind = LogMsg.Com ?? "";
				string strJob  = "";
				string strSta  = "";
				string strBody = LogMsg.Msg ?? "";
				if (strBody.StartsWith("[SCH]")) strBody = strBody.Substring(5);
				System.Text.RegularExpressions.Match mk =
					System.Text.RegularExpressions.Regex.Match(strBody, @"^\[([^\]]{1,12})\]");
				if (mk.Success)
				{
					if (strKind.Length == 0) strKind = mk.Groups[1].Value;
					strBody = strBody.Substring(mk.Length);
				}
				System.Text.RegularExpressions.Match mj =
					System.Text.RegularExpressions.Regex.Match(strBody, @"작업\s*[:]?\s*(\d{3,4})");
				if (mj.Success) strJob = mj.Groups[1].Value;
				System.Text.RegularExpressions.Match ms2 =
					System.Text.RegularExpressions.Regex.Match(strBody, @"상태\s*[']?(\d{1,2})[']?");
				if (ms2.Success) strSta = JobStatusText(ms2.Groups[1].Value);
				strBody = strBody.TrimStart();

				ListViewItem vItem = new ListViewItem(LogMsg.Time, 0);
				vItem.SubItems.Add(LogMsg.ID);
				vItem.SubItems.Add(strKind);
				vItem.SubItems.Add(strJob);
				vItem.SubItems.Add(strSta);
				// [LGLS 2026-08-21] 호출 위치 (Message 앞 2열 - 헤더 우클릭으로 표시/숨김)
				vItem.SubItems.Add(WcsCommon.cLogCols.ShortFile(pFile));
				vItem.SubItems.Add(pFunc ?? "");
				vItem.SubItems.Add(strBody);
				vItem.SubItems.Add(LogMsg.Tgm);

				switch (pMsgTyp)
				{
					case cDefApp.eLogMsgType.MSG_IMP: vItem.BackColor = Color.YellowGreen; vItem.ForeColor = Color.White; break;
					case cDefApp.eLogMsgType.MSG_ERR: vItem.BackColor = Color.Red; vItem.ForeColor = Color.White; break;
					default: vItem.BackColor = Color.White; vItem.ForeColor = Color.Black; break;
				}

				this.PsSetMsg(vItem, nId);

				return;

			}
			catch (Exception ex)
			{
				//MessageBox.Show(ex.Message);
			}
		}

		/// <summary>
		/// [LGLS 2026-08-23] 작업상태를 "[번호]내용" 으로 적는다.
		///   숫자만 있으면 10/20/30 이 CV/SC/RGV 중 무엇의 대기인지 로그만 보고는 알 수 없다.
		///   번호 체계는 cThread_SCH 의 ST_* 상수와 같다 (CV 1x / SC 2x / RGV 3x, x = 0대기 1지시 5중 9완료).
		/// </summary>
		private static string JobStatusText(string pSta)
		{
			if (string.IsNullOrEmpty(pSta)) return "";
			string s = pSta.TrimStart('0');
			if (s.Length == 0) s = "0";
			string t;
			switch (s)
			{
				case "99": t = "작업생성";     break;
				case "10": t = "CV 구동대기";  break;
				case "11": t = "CV 구동지시";  break;
				case "15": t = "CV 구동중";    break;
				case "19": t = "CV 구동완료";  break;
				case "20": t = "SC 구동대기";  break;
				case "21": t = "SC 구동지시";  break;
				case "25": t = "SC 구동중";    break;
				case "29": t = "SC 구동완료";  break;
				case "30": t = "RGV 구동대기"; break;
				case "31": t = "RGV 구동지시"; break;
				case "35": t = "RGV 구동중";   break;
				case "39": t = "RGV 구동완료"; break;
				default:   t = "";             break;   // 모르는 값은 번호만
			}
			return "[" + s + "]" + t;
		}
		#endregion

		#region[Event]btnDelLog_Click
		private void btnDelLog_Click(object sender, EventArgs e)
		{
            // Scheduler 로그 CLEAR 처리.
            this.lsvR.Items.Clear();
            this.txtMsg.Text = "";
		}
		#endregion

		#region 종료
		private void tsbEnd_Click(object sender, EventArgs e)
		{
			this.Close();
		}

		private void SYS_MAIN_FormClosing(object sender, FormClosingEventArgs e)
		{
            if (cDefApp.GM_RE_START == false)
            {
                if (cDefApp.GM_STAT_MAIN == true)
                {
                    if (MessageBox.Show(this, "종료하시겠습니까?", "종료", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                    {
                        LogManager.Shutdown();
                        cDefApp.GM_STAT_MAIN = false;
                        return;
                    }
                }
                e.Cancel = true;

                if (e.CloseReason == CloseReason.UserClosing)
                {
                    e.Cancel = true;
                }
                else
                {
                    return;
                }
            }
            else
            {
                MessageBox.Show(this, "프로그램 : IO_TASK_SEMI_FINISH \n프로그램이 이미 종료 중 입니다.", "IO_TASK_SEMI_FINISH");
                LogManager.Shutdown();
                cDefApp.GM_STAT_MAIN = false;
                return;
            }
		}
		#endregion

		//LIST VIEW를 클릭했을때 텍스트박스 하단에 표시하는것.
		private void lsvMsg_Click(object sender, EventArgs e)
		{
			try
			{
				Control Ctrl = m_Maindefine.PfCtlFind1(splitContainer1.Panel1, ((ListView)sender).Name);

				ListView LvCtrl = (ListView)Ctrl;

				// [LGLS 2026-09-01] 파일/함수 2열 삽입 후 인덱스가 밀려 [5]=파일명만 표시됐다(사용자 지적).
				//   Message 는 마지막 열이므로 인덱스 고정 대신 ★마지막 SubItem★ 을 쓰고,
				//   파일::함수를 머리에 붙여 어디서 난 로그인지도 함께 보인다.
				// [LGLS 2026-09-01] 마지막 열은 Message 가 아니라 Tgm(전문, 대부분 빈 값)이었다.
				//   열 구성 : 0시각 1스레드 2구분 3작업 4상태 5파일 6함수 7Message 8Tgm
				var it = LvCtrl.SelectedItems[0];
				string strFile = (it.SubItems.Count > 5) ? it.SubItems[5].Text : "";
				string strFunc = (it.SubItems.Count > 6) ? it.SubItems[6].Text : "";
				string strBody = (it.SubItems.Count > 7) ? it.SubItems[7].Text : "";
				string strTgm  = (it.SubItems.Count > 8) ? it.SubItems[8].Text : "";
				string strHead = (strFile.Length > 0 || strFunc.Length > 0) ? ("[" + strFile + " :: " + strFunc + "]  ") : "";
				this.txtMsg.Text = strHead + strBody + (strTgm.Trim().Length > 0 ? ("
TGM : " + strTgm) : "");
			}
			catch (Exception ex)
			{
				string msg = ex.Message;
			}
		}
	}
}