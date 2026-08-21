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
using log4net;
using log4net.Config;
using System.Diagnostics;

namespace WCS_TASK_CV
{
    public delegate void SampleEventDelegate(object sender, string msg);
	public delegate void DelPsMsgLog(DateTime LogDate, string strMsg, cDefApp.eLogWriteGbn eLogGbn);
    public partial class SYS_MAIN : Form
    {
        private int m_nProcessCnt;
        public string m_strConnectString;
        public CvThread[] m_thCvThread     = new CvThread[200];
        // [LGLS 2026-07-21] 물리 이관: SC/RTV 통신 스레드 (Vehicle 프로토콜 ↔ SC_DATA/RTV_DATA)
        public VehThread m_thVehSc;
        public VehThread m_thVehRtv;
        public cLogThread[] m_thLogging = new cLogThread[200];
        public string[] m_strCOMM_IP     = new string[200];
        public string[] m_strPLC_NO = new string[200];
        public string[] m_strEQMT_TYP = new string[200];
        public int[] m_nCOMM_CUR_PORT      = new int[200];
        public int[] m_nCOMM_FROM_PORT = new int[200];
        public int[] m_nCOMM_TO_PORT = new int[200];
        public int[] m_nCOMM_PORT_CNT = new int[200];
        public int[] m_nCOMM_CNT       = new int[200];
        public int[] m_nFrTrackNo = new int[200];
        public int[] m_nToTrackNo = new int[200];
        public string[] m_strLogPath = new string[200];
        public string[] m_strLogFileNm = new string[200];
        private string m_strRtnMsg          = "";
        private maindefine m_mfgClass = new maindefine();
        private bool m_bHex = true;
        private bool m_bAscii = false;
        private int m_nPlcMaker = 1;       // PLC 제조사 (ini [PLC] MAKER : 1=XGT, 0=Melsec)
        public bool IsHex { get { return m_bHex; } set { m_bHex = value; } }
        public bool IsAscii { get { return m_bAscii; } set { m_bAscii = value; } }

        #region@@@.생성자
        public SYS_MAIN()
        {
            InitializeComponent();

            // [LGLS 2026-07-27] CV_TASK [시나리오 테스트] 버튼 (동적 추가) — EQP_SIM 시나리오 테스트의 역할 반대판
            try
            {
                var btnScenario = new System.Windows.Forms.Button();
                btnScenario.Name = "btnScenarioTest";
                btnScenario.Text = "시나리오 테스트";
                btnScenario.Size = new System.Drawing.Size(120, 23);
                btnScenario.Location = new System.Drawing.Point(960, 28);
                btnScenario.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
                btnScenario.BackColor = System.Drawing.Color.MistyRose;
                btnScenario.Click += btnScenarioTest_Click;
                this.pnlTop.Controls.Add(btnScenario);
                btnScenario.BringToFront();

                // [LGLS 2026-07-28] [정리] 버튼 — 꼬인 출고 잔류(JOB_STATUS=15) + 유령 CV 파렛트 DB 정리
                var btnCleanup = new System.Windows.Forms.Button();
                btnCleanup.Name = "btnCleanupStuck";
                btnCleanup.Text = "정리";
                btnCleanup.Size = new System.Drawing.Size(70, 23);
                btnCleanup.Location = new System.Drawing.Point(884, 28);
                btnCleanup.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
                btnCleanup.BackColor = System.Drawing.Color.LightYellow;
                btnCleanup.Click += btnCleanupStuck_Click;
                this.pnlTop.Controls.Add(btnCleanup);
                btnCleanup.BringToFront();
            }
            catch { }
        }

        private void btnScenarioTest_Click(object sender, EventArgs e)
        {
            try { var frm = new FRM_SCENARIO_TEST(this); frm.Show(); }
            catch (Exception ex)
            { System.Windows.Forms.MessageBox.Show("시나리오 테스트 열기 실패: " + ex.Message, "CV_TASK", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error); }
        }

        // [LGLS 2026-07-28] 꼬인 출고 잔류 + 유령 CV 파렛트 정리 (이중입고/공출고 테스트 후 상태 불일치 해소)
        private void btnCleanupStuck_Click(object sender, EventArgs e)
        {
            try
            {
                if (System.Windows.Forms.MessageBox.Show(
                        "꼬인 출고 잔류(JOB_STATUS=15 CV구동중, JOB_TYP=2 출고)와\n유령 CV 파렛트(JOB_MST 미존재)를 정리합니다.\n계속하시겠습니까?",
                        "DB 정리", System.Windows.Forms.MessageBoxButtons.YesNo,
                        System.Windows.Forms.MessageBoxIcon.Warning) != System.Windows.Forms.DialogResult.Yes)
                    return;
                string r = (m_thCvThread[0] != null) ? m_thCvThread[0].CleanupStuckOutgo() : "CvThread 미기동 — 정리 불가";
                System.Windows.Forms.MessageBox.Show(r, "DB 정리 결과");
            }
            catch (Exception ex)
            { System.Windows.Forms.MessageBox.Show("정리 실패: " + ex.Message, "DB 정리", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error); }
        }
        #endregion

        /* 
         * SYS_MAIN_Load
         */
        #region[Event]SYS_MAIN_Load

        private void SYS_MAIN_Load(object sender, EventArgs e)
        {
			m_nProcessCnt = 0;

            // [LGLS 2026-08-19] 상단 컨트롤(R주소 라디오 ~780, 메모리맵/정리/시나리오 버튼 ~1080)이
            //   전부 보이도록 기본 창폭 확대 (종전 600 에서는 우측 버튼들이 잘려 있었다)
            this.Width = 1110;
            this.Height = 600;

            //중복실행을 방지하는 함수.
            if (cCmLib.GfPrevInstance() == true)
            {
                cDefApp.GM_RE_START = true;
                Application.Exit();
            }
            
            //this.Text = Process.GetCurrentProcess().ProcessName;

            this.IsAscii = checkBox1.Checked;
            this.IsHex = checkBox2.Checked;

            // PLC 제조사 설정 (ini [PLC] MAKER : 1=XGT, 0=Melsec)
            m_nPlcMaker = cDefApi.GsReadInitProfilePlcMaker();
            if (m_nPlcMaker == 1)
            {
                // XGT : 타이틀에 XGT 표시, 라디오(Display) 버튼/맵보기 버튼 표시, XML 파싱 버튼 숨김
                this.Text = this.Text + " [XGT]";
                checkBox1.Visible = true;
                checkBox2.Visible = true;
                btnMemMap.Visible = true;
                btnXmlSync.Visible = false;
            }
            else
            {
                // Melsec : 타이틀에 Melsec 표시, 라디오(Display) 버튼/맵보기 버튼 숨김, XML 파싱 버튼 표시
                this.Text = this.Text + " [Melsec]";
                checkBox1.Visible = false;
                checkBox2.Visible = false;
                btnMemMap.Visible = false;
                btnXmlSync.Visible = true;
                btnXmlSync.Location = btnMemMap.Location; // 맵보기 자리로 이동
            }

            // [LGLS 2026-08-19] R(트래킹) 주소 해석 모드 로드 (ini [PLC] R_ADDR_MODE : HEX=구 ECS 호환, DEC=현행 10진)
            //   R 영역을 쓰는 전 지점(CvThread 통신 / VehThread 관측 / 메모리맵 직접 읽기·쓰기 / 시나리오 테스트)이
            //   cDefApp.GsRTrackWord() 를 거치므로 이 라디오 하나로 일괄 전환된다.
            // [LGLS 2026-08-21] 단일 기준 = XML(rAddrMode). XML 이 없을 때만 INI 폴백.
            //   라디오 전환은 XML+INI 양쪽에 기록되므로 정상 상태에서 둘은 항상 같다.
            if (cPlcAddrMap.IsLoaded)
            {
                cDefApp.GM_R_ADDR_HEX = cPlcAddrMap.RAddrModeHex;
                cDefApi.GsWriteInitProfileRAddrHex(cDefApp.GM_R_ADDR_HEX);   // INI 동기화
            }
            else
                cDefApp.GM_R_ADDR_HEX = cDefApi.GsReadInitProfileRAddrHex();
            // 주소맵 파서(EQP_SIM 과 공유하는 파일)에도 같은 변환기를 꽂아 라디오와 연동한다.
            cPlcAddrMap.RTrackWordFn = cDefApp.GsRTrackWord;
            // [LGLS 2026-08-21] 로그 헤더 우클릭 → 열 표시/숨김 메뉴
            WcsCommon.cLogCols.Attach(lsvCOMM1);

            m_bRAddrLoading = true;
            rdoRHex.Checked = cDefApp.GM_R_ADDR_HEX;
            rdoRDec.Checked = !cDefApp.GM_R_ADDR_HEX;
            m_bRAddrLoading = false;
            // XGT 전용 설정 - Melsec 은 R 트래킹 규약이 다르므로 숨김
            rdoRHex.Visible = (m_nPlcMaker == 1);
            rdoRDec.Visible = (m_nPlcMaker == 1);
            rdoRHex.BackColor = System.Drawing.Color.LightYellow;
            rdoRDec.BackColor = System.Drawing.Color.LightYellow;
            string strTip = "R(트래킹) 영역 주소 해석 방식\r\n"
                          + " · 16진(구ECS) : 문서표기를 16진 파싱  (C/V#11 R0100 → 워드 256 / %RB512)\r\n"
                          + " · 10진(현행)  : 문서표기를 그대로 사용 (C/V#11 R0100 → 워드 100 / %RB200)\r\n"
                          + "통신(CvThread/VehThread)·메모리맵 직접 읽기/쓰기·시나리오 테스트에 모두 적용됩니다.";
            try
            {
                System.Windows.Forms.ToolTip tt = new System.Windows.Forms.ToolTip();
                tt.SetToolTip(rdoRHex, strTip);
                tt.SetToolTip(rdoRDec, strTip);
            }
            catch { }

#if ORACLE
            cDefApi.GsGetInitPorFileDB_1(ref cDefApp.GM_DB1_PROVIDER, ref cDefApp.GM_DB1_ALIAS, ref cDefApp.GM_DB1_USERID, ref cDefApp.GM_DB1_PASSWORD, ref m_strRtnMsg);
            m_strConnectString = "Provider=" + cDefApp.GM_DB1_PROVIDER + "; Data Source=" + cDefApp.GM_DB1_ALIAS + "; User ID=" + cDefApp.GM_DB1_USERID + "; Password =" + cDefApp.GM_DB1_PASSWORD;
            this.Text = this.Text + " [DB:" + cDefApp.GM_DB1_ALIAS + "]";   // 접속 DB명 타이틀 표시
#endif
#if POSTGRESQL
            cDefApi.GsGetInitPorFileDB_2(ref cDefApp.GM_DB2_IP, ref cDefApp.GM_DB2_DATABASE, ref cDefApp.GM_DB2_PORT, ref cDefApp.GM_DB2_USER, ref cDefApp.GM_DB2_USER_PW, ref m_strRtnMsg);
            m_strConnectString = "host=" + cDefApp.GM_DB2_IP + ";username=" + cDefApp.GM_DB2_USER + ";password=" + cDefApp.GM_DB2_USER_PW + ";database=" + cDefApp.GM_DB2_DATABASE + ";MAXPOOLSIZE=50;";
            this.Text = this.Text + " [DB:" + cDefApp.GM_DB2_DATABASE + "@" + cDefApp.GM_DB2_IP + "]"; // 접속 DB명 타이틀 표시
#endif
#if SQL
            // [LGLS] MS SQL Server 접속문자열 구성 (INI [DB_2] 재사용)
            cDefApi.GsGetInitPorFileDB_2(ref cDefApp.GM_DB2_IP, ref cDefApp.GM_DB2_DATABASE, ref cDefApp.GM_DB2_PORT, ref cDefApp.GM_DB2_USER, ref cDefApp.GM_DB2_USER_PW, ref m_strRtnMsg);
            m_strConnectString = "Server=" + cDefApp.GM_DB2_IP + ";Database=" + cDefApp.GM_DB2_DATABASE + ";User ID=" + cDefApp.GM_DB2_USER + ";Password=" + cDefApp.GM_DB2_USER_PW + ";";
            this.Text = this.Text + " [DB:" + cDefApp.GM_DB2_DATABASE + "@" + cDefApp.GM_DB2_IP + "]"; // 접속 DB명 타이틀 표시
#endif

            cDefApi.GsGetInitPorFileCNF(ref cDefApp.GM_WH_TYP, ref cDefApp.GM_USERID, ref m_strRtnMsg);
            cDefApi.GsReadInitProfileProcessCnt("PROCESS", ref m_nProcessCnt, ref m_strRtnMsg);

            //@@.CV #1 접속정보ini 읽어오기
            for (int ii = 0; ii < m_nProcessCnt; ii++)
            {
                string Name=null;

                Name =  "COMM" + ii.ToString();
                //@@.CV #1 접속정보ini 읽어오기
                cDefApi.GsReadInitProfileCom(Name,
                                             ref m_strPLC_NO[ii],
                                             ref m_strCOMM_IP[ii],
                                             ref m_nCOMM_CUR_PORT[ii],
                                             ref m_nCOMM_FROM_PORT[ii],
                                             ref m_nCOMM_TO_PORT[ii],
                                             ref m_nCOMM_PORT_CNT[ii],
                                             ref m_nCOMM_CNT[ii],
                                             ref ii,
                                             ref m_nFrTrackNo[ii],
                                             ref m_nToTrackNo[ii],
                                             ref m_strLogPath[ii],
                                             ref m_strLogFileNm[ii],
                                             ref m_strEQMT_TYP[ii],
                                             ref m_strRtnMsg);

                if (m_strPLC_NO[ii] == "")
                {
                    m_strPLC_NO[ii] = null;
                    break;
                }

                SetVisable(pnlTop, ii, "picCvDbCn" + ii.ToString(), "DB  Status #" + ii.ToString("00"));
                SetVisable(pnlTop, ii, "picCvSkt" + ii.ToString(), "Socket  Status #" + ii.ToString("00"));

                SetDisplay(pnlTop, ii, "picCvDbCn" + ii.ToString(), "D");
                SetDisplay(pnlTop, ii, "picCvSkt" + ii.ToString(), "D", "E");

            }
         
            // @@.통신 딜레이 타임읽어오기
            cDefApi.GsReadInitProfileDelay("SND", ref cDefApp.GM_COMM_SND_TIME_OUT, ref m_strRtnMsg); // @.전송
            cDefApi.GsReadInitProfileDelay("RCV", ref cDefApp.GM_COMM_RCV_TIME_OUT, ref m_strRtnMsg); // @.수신

            // Initialize log queues to avoid NullReference when threads enqueue log messages
            int logInitCount = Math.Min(m_nProcessCnt, cDefApp.m_LogQ.Length);
            for (int ii = 0; ii < logInitCount; ii++)
            {
                if (cDefApp.m_LogQ[ii] == null)
                    cDefApp.m_LogQ[ii] = new Queue<LogParam>();
            }

            // @@.여기서 부터 쓰레드 시작
            cDefApp.GM_STAT_MAIN  = true; // @.메인 시스템 동작상태
            WrkThStart();   // @.쓰레드 시작

            // [LGLS 2026-07-21] 물리 이관: SC/RTV 통신 스레드 기동 (마스터 PLC = COMM0 IP:2004)
            try
            {
                string vehIp = string.IsNullOrEmpty(m_strCOMM_IP[0]) ? "127.0.0.1" : m_strCOMM_IP[0];
                m_thVehSc  = new VehThread("SC",  m_strConnectString, cDefApp.GM_WH_TYP, vehIp, 2004);
                m_thVehRtv = new VehThread("RTV", m_strConnectString, cDefApp.GM_WH_TYP, vehIp, 2004);
                m_thVehSc.Start();
                m_thVehRtv.Start();
            }
            catch (Exception exVeh)
            {
                MessageBox.Show("SC/RTV 통신 스레드 기동 실패: " + exVeh.Message);
            }
        }
        #endregion

        /*
         * 화면 통신 표시 제어
         */
        #region
        private void SetVisable(Panel obj, int ii, string ctrName, string tipname)
        {
            Control ctrl;
            PictureBox FindPictureBox = null;



            string msg = null;
            ctrl = m_mfgClass.PfCtlFind(ref obj, ctrName, ref msg);
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

            string msg = null;
            ctrl = m_mfgClass.PfCtlFind(ref obj, ctrName, ref msg);
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
        private void SetDisplay(Panel obj, int ii, string ctrName, string opt)
        {
            Control ctrl;
            PictureBox FindPictureBox = null;



            string msg = null;
            ctrl = m_mfgClass.PfCtlFind(ref obj, ctrName, ref msg);
            if (ctrl == null)
            {
                return;
            }

            FindPictureBox = ctrl as PictureBox;

            PfSetStatImgView(FindPictureBox, opt);
        }
        #endregion

        /*
         * @@@.스레드 실행
         */
        #region
        private void WrkThStart()
        {
            CheckForIllegalCrossThreadCalls = false;
            Thread_Timer.Enabled = true;

            // [LGLS 2026-07-27] 마스터 PLC 1소켓 통합: CvThread 를 1개만 생성(COMM0 소켓)하고,
            //   나머지 COMM(설비)들은 AddEquip 으로 같은 스레드의 처리 슬롯에 등록한다.
            //   (기존: COMM 개수만큼 스레드/소켓 15개 → 실제 마스터 PLC 1대이므로 1소켓으로 통합)
            if (m_nProcessCnt <= 0) return;

            m_thCvThread[0] = new CvThread(0,
                                            cDefApp.GM_WH_TYP,
                                            m_strEQMT_TYP[0],
                                            m_strPLC_NO[0],
                                            m_strCOMM_IP[0],
                                            Convert.ToInt16("0" + m_nCOMM_CUR_PORT[0]),
                                            Convert.ToInt16("0" + m_nCOMM_FROM_PORT[0]),
                                            Convert.ToInt16("0" + m_nCOMM_TO_PORT[0]),
                                            m_nCOMM_PORT_CNT[0],
                                            m_nCOMM_CNT[0],
                                            m_nFrTrackNo[0],
                                            m_nToTrackNo[0],
                                            m_strConnectString,
                                            m_strLogFileNm[0]);

            for (int ii = 1; ii < m_nProcessCnt; ii++)
            {
                m_thCvThread[0].AddEquip(m_strPLC_NO[ii],
                                         m_strEQMT_TYP[ii],
                                         m_nFrTrackNo[ii],
                                         m_nToTrackNo[ii],
                                         m_nCOMM_CNT[ii]);
            }

            // [LGLS 2026-07-30] 화물 소실 수정: [PROCESS] CNT=1([COMM0]=PLC11만) 운용에서는 라인(PLC01~10)·
            //   나머지 스테이션(PLC12~15) 트랙이 슬롯에 등록되지 않아 CV_DATA 의 SENSOR0/LUGG_NO 가 영영
            //   갱신되지 않는다 → IO_TASK DriveSC 가 SC 하역트랙(예: 904→115/116)을 빈 트랙으로 판정해
            //   입고 SC 배정을 무기한 보류(작업 20 정체, 화물은 EQP_SIM 에만 존재).
            //   소켓은 위에서 이미 1개(COMM0)로 통합돼 있고 CNT 는 처리 슬롯 수일 뿐이므로,
            //   INI 에 없는 설비 그룹을 고정 레이아웃(CV_DATA PLC_NO/MC_NO 와 동일)으로 자동 등록한다.
            {
                string[] plcs = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15" };
                int[]    frs  = { 101,  103,  105,  107,  109,  111,  113,  115,  117,  119,  121,  123,  125,  127,  130 };
                int[]    tos  = { 102,  104,  106,  108,  110,  112,  114,  116,  118,  120,  122,  124,  126,  129,  132 };
                for (int gi = 0; gi < plcs.Length; gi++)
                {
                    bool bHave = false;
                    for (int ii = 0; ii < m_nProcessCnt; ii++)
                    {
                        int a, b;
                        if (m_strPLC_NO[ii] != null &&
                            int.TryParse(m_strPLC_NO[ii], out a) && int.TryParse(plcs[gi], out b) && a == b)
                        { bHave = true; break; }
                    }
                    if (!bHave)
                        m_thCvThread[0].AddEquip(plcs[gi], m_strEQMT_TYP[0], frs[gi], tos[gi], 1);
                }
            }
        }


        //public event SampleEventDelegate DspMsg;

        private void Thread_Tick(object sender, EventArgs e)
        {
			try
			{
				Thread_Timer.Enabled = false;

                // [LGLS 2026-07-27] 마스터 PLC 1소켓 통합: CvThread 1개(index 0)만 시작/모니터.
                for (int ii = 0; ii < 1; ii++)
                {
                    if (m_thCvThread[ii].m_thThread == null)
                    {
                        SetDisplay(pnlTop, ii, "picCvSkt" + ii.ToString(), "T");
                        SetDisplay(pnlTop, ii, "picCvDbCn" + ii.ToString(), "T");

                        m_thCvThread[ii].m_thThread = new Thread(m_thCvThread[ii].Thread_Doing);
                        m_thCvThread[ii].m_thThread.IsBackground = true;
                        m_thCvThread[ii].m_frmMain = this;
                        m_thCvThread[ii].m_thThread.Start(ii);

                        Thread.Sleep(100);
                    }
                    else
                    {
                        if (m_thCvThread[ii].IsOpen)
                        {
                            SetDisplay(pnlTop, ii, "picCvSkt" + ii.ToString(), "C");
                            SetDisplay(pnlTop, ii, "picCvDbCn" + ii.ToString(), "C");
                        }
                    }

                }

				Thread_Timer.Enabled = true;

			}
			catch (Exception ex)
			{
				Thread_Timer.Enabled = true;
			}

        }
        #endregion


        #region[Motod] @@@.쓰레드 상태를 화면에 표시
        private bool PfSetStatImgView(PictureBox  pPic, 
                                          string  pStatSkt, 
                                          string pStatOp) {
            // @.Stat Connection : C:연결, T:시도, D:비연결
            // @.Stat Operation : N:정상, W:대기, E:에러
            try {
                switch (pStatSkt + pStatOp)
                {
                    case "CN":if (pPic.Tag.ToString() != "0") pPic.Image = this.imgLstStat.Images[0]; pPic.Tag = "0"; break;
                    case "CW":if (pPic.Tag.ToString() != "1") pPic.Image = this.imgLstStat.Images[1]; pPic.Tag = "1"; break;
                    case "CE":if (pPic.Tag.ToString() != "2") pPic.Image = this.imgLstStat.Images[2]; pPic.Tag = "2"; break;
                    case "TN":if (pPic.Tag.ToString() != "3") pPic.Image = this.imgLstStat.Images[3]; pPic.Tag = "3"; break;
                    case "TW":if (pPic.Tag.ToString() != "4") pPic.Image = this.imgLstStat.Images[4]; pPic.Tag = "4"; break;
                    case "TE":if (pPic.Tag.ToString() != "5") pPic.Image = this.imgLstStat.Images[5]; pPic.Tag = "5"; break;
                    case "DN":if (pPic.Tag.ToString() != "6") pPic.Image = this.imgLstStat.Images[6]; pPic.Tag = "6"; break;
                    case "DW":if (pPic.Tag.ToString() != "7") pPic.Image = this.imgLstStat.Images[7]; pPic.Tag = "7"; break;
                    case "DE":if (pPic.Tag.ToString() != "8") pPic.Image = this.imgLstStat.Images[8]; pPic.Tag = "8"; break;
                    default: break;
                }
                return true;
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
                //MsgBox(ex.Message & pPic.Name)
            }
            return false;
        }
        #endregion

        #region[Motod] @@@.DB연결 상태를 화면에 표시
        private bool PfSetStatImgView(PictureBox pPic,
                                      string pStatDbCn)
        {
            // @.Stat Connection : C:연결, T:시도, D:비연결

            try 
            {
                switch(pStatDbCn)
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
                string msg = ex.Message;
                //MsgBox(ex.Message & pPic.Name)
            }
            return false;
        }
        #endregion

        #region@@@.ListView에 로깅[PsMsgView();]
        // @@@.대리자 선언
		delegate void DelegateListViewItem(ListViewItem item, cDefApp.eLogWriteGbn eThGbn);

        // @@@.Client 메세지 Listview Invoke 선언
		private void PsSetMsg(ListViewItem item, cDefApp.eLogWriteGbn eThGbn)
		{
			try
			{
				string strCtrlName = "";
				if (eThGbn == cDefApp.eLogWriteGbn.COMM1)
					strCtrlName = "lsvCOMM1";
				else if (eThGbn == cDefApp.eLogWriteGbn.COMM2)
					strCtrlName = "lsvCOMM2";
                else if (eThGbn == cDefApp.eLogWriteGbn.COMM3)
                    strCtrlName = "lsvCOMM3";
                else if (eThGbn == cDefApp.eLogWriteGbn.COMM4)
                    strCtrlName = "lsvCOMM4";
                else if (eThGbn == cDefApp.eLogWriteGbn.COMM5)
                    strCtrlName = "lsvCOMM5";
                else if (eThGbn == cDefApp.eLogWriteGbn.COMM6)
                    strCtrlName = "lsvCOMM6";
                else if (eThGbn == cDefApp.eLogWriteGbn.COMM7)
                    strCtrlName = "lsvCOMM7";
                else if (eThGbn == cDefApp.eLogWriteGbn.COMM8)
                    strCtrlName = "lsvCOMM8";
                else if (eThGbn == cDefApp.eLogWriteGbn.COMM9)
                    strCtrlName = "lsvCOMM9";
				else
					strCtrlName = "";

				Control Ctrl = PfCtlFind1(splBodySkt.Panel1, strCtrlName);

				if (Ctrl == null) return;

				ListView lstView = (ListView)Ctrl;

				if (lstView.InvokeRequired == true)
				{
					DelegateListViewItem d = new DelegateListViewItem(this.PsSetMsg); // SetListview
					this.Invoke(d, item, eThGbn);
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
				MessageBox.Show(ex.Message);
			}
		}

        //@@@.PsMsgView[화면에 로깅...]
		public void PsMsgView(string pMsg, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, "", "", "", cDefApp.eLogMsgType.MSG_NOR, nThGbn, pFile, pFunc);
        }
		public void PsMsgView_Error(string pMsg, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, "", "", "", cDefApp.eLogMsgType.MSG_ERR, nThGbn, pFile, pFunc);
        }
		public void PsMsgView_IMP(string pMsg, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, "", "", "", cDefApp.eLogMsgType.MSG_IMP, nThGbn, pFile, pFunc);
        }
		public void PsMsgView(string pMsg, string pObjID, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, pObjID, "", "", cDefApp.eLogMsgType.MSG_NOR, nThGbn, pFile, pFunc);
        }
		public void PsMsgView_Error(string pMsg, string pObjID, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, pObjID, "", "", cDefApp.eLogMsgType.MSG_ERR, nThGbn, pFile, pFunc);
        }
		public void PsMsgView_IMP(string pMsg, string pObjID, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, pObjID, "", "", cDefApp.eLogMsgType.MSG_IMP, nThGbn, pFile, pFunc);
        }
        public void PsMsgView(string pMsg, string pObjID, string pCommTyp, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, pObjID, pCommTyp, "", cDefApp.eLogMsgType.MSG_NOR, nThGbn, pFile, pFunc);
        }
        public void PsMsgView_Error(string pMsg, string pObjID, string pCommTyp, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, pObjID, pCommTyp, "", cDefApp.eLogMsgType.MSG_ERR, nThGbn, pFile, pFunc);
        }
		public void PsMsgView(string pMsg, string pObjID, string pCommTyp, string pTgm, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, pObjID, pCommTyp, pTgm, cDefApp.eLogMsgType.MSG_NOR, nThGbn, pFile, pFunc);
        }
		public void PsMsgView_Error(string pMsg, string pObjID, string pCommTyp, string pTgm, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, pObjID, pCommTyp, pTgm, cDefApp.eLogMsgType.MSG_ERR, nThGbn, pFile, pFunc);
        }
		public void PsMsgView_IMP(string pMsg, string pObjID, string pCommTyp, string pTgm, int nThGbn, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
			PsMsgView(pMsg, pObjID, pCommTyp, pTgm, cDefApp.eLogMsgType.MSG_IMP, nThGbn, pFile, pFunc);
        }
        private void PsMsgView(string pMsg, 
                               string pObjID, 
                               string pCommTyp, 
                               string pTgm, 
                  cDefApp.eLogMsgType pMsgTyp,
							   int nThGbn,
                               string pFile = "",
                               string pFunc = "")
        {
            try
            {

                if (chkStopLog.Checked) return;

                cDefApp.stutLogMsgInfo LogMsg ;
                LogMsg.Time = DateTime.Now.ToString( "yyyy/MM/dd HH:mm:ss:ffffff");
                LogMsg.MsgTyp =  pMsgTyp.ToString(); 
                LogMsg.ID = pObjID;
                LogMsg.Com = pCommTyp;
                LogMsg.Msg = pMsg;
                LogMsg.Tgm = pTgm;
                if( chkStopLog.Checked) return;
                ListViewItem vItem = new ListViewItem(LogMsg.Time, 0);
                vItem.SubItems.Add(LogMsg.ID);
                vItem.SubItems.Add(LogMsg.Com);
                // [LGLS 2026-08-21] 호출 위치 (Message 앞 2열 - 헤더 우클릭으로 표시/숨김)
                vItem.SubItems.Add(WcsCommon.cLogCols.ShortFile(pFile));
                vItem.SubItems.Add(pFunc ?? "");
                vItem.SubItems.Add(LogMsg.Msg);
                vItem.SubItems.Add(LogMsg.Tgm);
                switch (pMsgTyp)
                {
                    case cDefApp.eLogMsgType.MSG_IMP : vItem.BackColor = Color.Blue; vItem.ForeColor = Color.White; break; 
                    case cDefApp.eLogMsgType.MSG_ERR: vItem.BackColor = Color.Red; vItem.ForeColor = Color.White; break; 
                    default:  vItem.BackColor = Color.White; vItem.ForeColor = Color.Black; break; 

                }
				this.PsSetMsg(vItem, (cDefApp.eLogWriteGbn)nThGbn);
                return;
             }
            catch (Exception ex) 
            {
                MessageBox.Show(ex.Message);
            }
        }
        #endregion

        #region[Event]btnMemMap_Click
        private void btnMemMap_Click(object sender, EventArgs e)
        {
            FRM_PLC_MEMMAP frm = new FRM_PLC_MEMMAP();
            frm.Show();
        }
        #endregion

        #region[Event]btnXmlSync_Click
        private void btnXmlSync_Click(object sender, EventArgs e)
        {
            FRM_XML_FIELD_SYNC frm = new FRM_XML_FIELD_SYNC(m_strConnectString);
            frm.Show();
        }
        #endregion

        #region[Event]btnDelLog_Click
        private void btnDelLog_Click(object sender, EventArgs e)
        {
            this.lsvCOMM1.Items.Clear();
            this.txtMsg.Text = "";
            this.txtTgm.Text = "";
        }
        #endregion

        #region[Event]btnDelLog_Click
        private void lsvMsg_Click(object sender, EventArgs e)
        {
            try
            {
                 // [LGLS 2026-08-21] 파일/함수 2열 삽입으로 Msg=5, Tgm=6
                 this.txtMsg.Text = this.lsvCOMM1.SelectedItems[0].SubItems[5].Text;
                 this.txtTgm.Text = this.lsvCOMM1.SelectedItems[0].SubItems[6].Text;
            }
            catch(Exception ex)
            {
                string msg = ex.Message;
            }
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
                    if (MessageBox.Show(this, "종료하시겠습니까?", "질문", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                    {
                        LogManager.Shutdown();
                        cDefApp.GM_STAT_MAIN = false;
                        return;
                    }
                }
                e.Cancel = true;

                //if (e.CloseReason == CloseReason.UserClosing)
                //{
                //    e.Cancel = true;
                //}
                //else
                //{
                //    return;
                //}
            }
            else
            {
                MessageBox.Show(this, "프로그램 : WCS_TASK_CV \n프로그램이 이미 실행 중 입니다.", "WCS_TASK_CV");
                LogManager.Shutdown();
                cDefApp.GM_STAT_MAIN = false;
                return;
            }
        }
        #endregion

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            IsHex = checkBox2.Checked;
        }

        // [LGLS 2026-08-19] R(트래킹) 주소 해석 모드 전환
        //   HEX = 구 ECS 호환 : 문서 표기를 16진 파싱 (C/V#11 R0100 → 워드 256)
        //   DEC = 현행       : 문서 표기를 10진 워드주소로 (C/V#11 R0100 → 워드 100)
        //   ※ 통신 중 전환하면 다음 사이클부터 새 주소로 읽고 쓴다.
        private bool m_bRAddrLoading = false;
        private void rdoRAddr_CheckedChanged(object sender, EventArgs e)
        {
            if (m_bRAddrLoading) return;
            RadioButton rdo = sender as RadioButton;
            if (rdo == null || !rdo.Checked) return;

            cDefApp.GM_R_ADDR_HEX = rdoRHex.Checked;
            cDefApi.GsWriteInitProfileRAddrHex(cDefApp.GM_R_ADDR_HEX);
            // [LGLS 2026-08-21] XML(rAddrMode)이 단일 기준 - 라디오 전환을 XML 에도 기록해
            //   EQP_SIM 등 다른 프로그램이 같은 해석을 따라오게 한다
            cPlcAddrMap.WriteRAddrMode(cDefApp.GM_R_ADDR_HEX);
            // S/C·RGV 통신(VehThread)은 주소표를 생성 시 1회 만들므로 재구성을 요청한다
            try
            {
                if (m_thVehSc  != null) m_thVehSc.RequestReloadObservables();
                if (m_thVehRtv != null) m_thVehRtv.RequestReloadObservables();
            }
            catch { }

            string strMsg = "[R주소모드] " + cDefApp.GsRAddrModeText()
                          + "  (예: C/V#11 R0100 → 워드 " + cDefApp.GsRTrackWord(100).ToString()
                          + " / %RB" + (cDefApp.GsRTrackWord(100) * 2).ToString() + ")";
            try { PsMsgView_IMP(strMsg, 0); }
            catch { }

            // [LGLS 2026-08-19] 이미 열려 있는 PLC 메모리 맵 창을 즉시 갱신한다
            //   (창 활성화 이벤트에만 기대면 포그라운드 전환이 막힐 때 옛 주소가 그대로 남는다)
            try
            {
                foreach (Form f in Application.OpenForms)
                {
                    FRM_PLC_MEMMAP frm = f as FRM_PLC_MEMMAP;
                    if (frm != null) frm.RefreshRMode();
                }
            }
            catch { }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            IsAscii = checkBox1.Checked;
        }

		#region 컨트롤 찾기.
		public Control PfCtlFind(ref Panel pPnl, string pCtlNm)
		{
			Control[] ctl;
			try
			{
				ctl = pPnl.Controls.Find(pCtlNm, true);

				if (ctl.Length == 0)
				{
					return null;
				}
				else
				{
					return ctl[0];
				}
			}
			catch (Exception ex)
			{
			}
			return null;
		}

		public Control PfCtlFind1(SplitterPanel pPnl, string pCtlNm)
		{
			Control[] ctl;
			try
			{
				ctl = pPnl.Controls.Find(pCtlNm, true);

				if (ctl.Length == 0)
				{
					return null;
				}
				else
				{
					return ctl[0];
				}
			}
			catch (Exception ex)
			{
			}
			return null;
		}
		#endregion



    }
}
