using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ALL_TASK
{
    // ─────────────────────────────────────────────────────────────────────────
    // [LGLS 2026-09-09] ALL_TASK 메인 창
    //
    //   WCS_TASK_CV(EQP) / IO_TASK(IO) / WCS_TASK_HOST(HOST) 세 프로그램을 한
    //   프로세스로 합친 껍데기. 각 태스크의 기존 화면을 탭에 그대로 끼우고,
    //   [제어판] 탭에서 태스크별로 스레드를 끄고 켠다.
    //
    //   - 기동 : 원래 폼을 새로 만들어 Load 를 태운다(=단독 EXE 를 켠 것과 같다)
    //   - 정지 : TaskStop() 으로 스레드 종료 + 소켓/DB 반납 후 폼 폐기
    //   - 로그 : WcsCommon.cTaskLog 가 스레드별 파일(날짜별/1년)에 적재(운전 이력은 종전 로그 테이블)
    //
    //   설정은 ALL_TASK.INI 를 쓴다(각 태스크의 INI 는 종전 그대로 각자 읽는다).
    // ─────────────────────────────────────────────────────────────────────────
    public partial class AllTaskMain : Form
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern int GetPrivateProfileString(string sec, string key, string def,
                                                          System.Text.StringBuilder ret, int size, string file);

        private const string INI = "./ALL_TASK.INI";

        private readonly List<TaskHost> m_lstTask = new List<TaskHost>();
        private readonly Dictionary<TaskKind, Button> m_dicBtnStart = new Dictionary<TaskKind, Button>();
        private readonly Dictionary<TaskKind, Button> m_dicBtnStop  = new Dictionary<TaskKind, Button>();
        private readonly Dictionary<TaskKind, Label>  m_dicLblState = new Dictionary<TaskKind, Label>();

        private Label m_lblLogDir, m_lblLogKeep, m_lblLogDb, m_lblLogQue;

        public AllTaskMain()
        {
            InitializeComponent();
            LoadAppIcon();
        }

        // 실행 파일에 넣어 둔 ALL_TASK 아이콘을 창/작업표시줄 아이콘으로 쓴다.
        private void LoadAppIcon()
        {
            try
            {
                System.Reflection.Assembly asm = System.Reflection.Assembly.GetExecutingAssembly();
                using (Stream st = asm.GetManifestResourceStream("ALL_TASK.ALL_TASK.ico"))
                {
                    if (st != null) { this.Icon = new Icon(st); return; }
                }
                this.Icon = Icon.ExtractAssociatedIcon(Application.ExecutablePath);
            }
            catch { }
        }

        // ─────────────────────────────────────────────────────────────────
        private static string IniStr(string pSec, string pKey, string pDef)
        {
            System.Text.StringBuilder sb = new System.Text.StringBuilder(512);
            GetPrivateProfileString(pSec, pKey, pDef, sb, sb.Capacity, INI);
            return sb.ToString().Trim();
        }

        private static int IniInt(string pSec, string pKey, int pDef)
        {
            int n;
            if (int.TryParse(IniStr(pSec, pKey, pDef.ToString()), out n)) return n;
            return pDef;
        }

        // ─────────────────────────────────────────────────────────────────
        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            this.Text = WcsCommon.cWcsVer.Title + "  -  ALL_TASK (EQP / IO / HOST 통합)";
            CheckForIllegalCrossThreadCalls = false;   // 세 태스크가 모두 이 전제로 만들어져 있다

            BuildControlPanel();
            StartLogSink();

            m_lstTask.Add(new TaskHost(TaskKind.EQP,  "EQP",  tabEqp));
            m_lstTask.Add(new TaskHost(TaskKind.IO,   "IO",   tabIo));
            m_lstTask.Add(new TaskHost(TaskKind.HOST, "HOST", tabHost));

            tmrState.Tick += tmrState_Tick;
            tmrState.Enabled = true;

            WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "IMP", "ALL_TASK 기동");

            // 자동 기동 (ALL_TASK.INI [AUTOSTART]) - 기동 순서는 EQP → IO → HOST
            foreach (TaskHost th in m_lstTask)
            {
                if (IniInt("AUTOSTART", th.Name, 0) != 0)
                    DoStart(th);
            }

            RefreshState();
        }

        // ─────────────────────────────────────────────────────────────────
        private void StartLogSink()
        {
            string strDir = IniStr("LOG", "DIR", "");
            if (strDir.Length == 0)
                strDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "LOG");
            else if (!Path.IsPathRooted(strDir))
                strDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, strDir);

            int nKeep = IniInt("LOG", "KEEP_DAYS", 365);

            WcsCommon.cTaskLog.Start(strDir, nKeep, "", false);   // [LGLS 2026-09-09] DB 적재 없음

            m_lblLogDir.Text  = "폴더 : " + WcsCommon.cTaskLog.RootDir;
            m_lblLogKeep.Text = "보관 : " + WcsCommon.cTaskLog.KeepDays + "일 (스레드별 폴더 / 날짜별 파일)";
            m_lblLogDb.Text   = "DB   : 적재 없음 - 운전 이력은 종전대로 WCS_LOG_PGR 에 남는다";
        }

        // ─────────────────────────────────────────────────────────────────
        private void BuildControlPanel()
        {
            string[] arrName = { "EQP  (설비통신)", "IO   (스케줄러)", "HOST (상위통신)" };
            TaskKind[] arrKind = { TaskKind.EQP, TaskKind.IO, TaskKind.HOST };

            for (int i = 0; i < 3; i++)
            {
                TaskKind kind = arrKind[i];
                int y = 22 + i * 38;

                Label lblNm = new Label();
                lblNm.Text = arrName[i];
                lblNm.Font = new Font("맑은 고딕", 9F, FontStyle.Bold);
                lblNm.Location = new Point(12, y + 4);
                lblNm.Size = new Size(120, 20);
                grpTask.Controls.Add(lblNm);

                Label lblSt = new Label();
                lblSt.Text = "정지";
                lblSt.TextAlign = ContentAlignment.MiddleCenter;
                lblSt.BorderStyle = BorderStyle.FixedSingle;
                lblSt.Location = new Point(140, y + 2);
                lblSt.Size = new Size(70, 24);
                grpTask.Controls.Add(lblSt);
                m_dicLblState[kind] = lblSt;

                Button btnGo = new Button();
                btnGo.Text = "기동";
                btnGo.Location = new Point(224, y);
                btnGo.Size = new Size(80, 28);
                btnGo.Tag = kind;
                btnGo.Click += btnStart_Click;
                grpTask.Controls.Add(btnGo);
                m_dicBtnStart[kind] = btnGo;

                Button btnStop = new Button();
                btnStop.Text = "정지";
                btnStop.Location = new Point(310, y);
                btnStop.Size = new Size(80, 28);
                btnStop.Tag = kind;
                btnStop.Click += btnStop_Click;
                grpTask.Controls.Add(btnStop);
                m_dicBtnStop[kind] = btnStop;

                Button btnRe = new Button();
                btnRe.Text = "재기동";
                btnRe.Location = new Point(396, y);
                btnRe.Size = new Size(80, 28);
                btnRe.Tag = kind;
                btnRe.Click += btnRestart_Click;
                grpTask.Controls.Add(btnRe);
            }

            // 로그 정보
            m_lblLogDir  = NewInfoLabel(20);
            m_lblLogKeep = NewInfoLabel(44);
            m_lblLogDb   = NewInfoLabel(68);
            m_lblLogQue  = NewInfoLabel(92);

            Button btnOpen = new Button();
            btnOpen.Text = "로그 폴더 열기";
            btnOpen.Location = new Point(12, 116);
            btnOpen.Size = new Size(120, 26);
            btnOpen.Click += delegate
            {
                try { System.Diagnostics.Process.Start("explorer.exe", WcsCommon.cTaskLog.RootDir); }
                catch (Exception ex) { MessageBox.Show(ex.Message); }
            };
            grpLog.Controls.Add(btnOpen);

            // 스레드 상태 목록
            lsvThread.Columns.Add("태스크", 90);
            lsvThread.Columns.Add("스레드", 140);
            lsvThread.Columns.Add("상태", 70);
            lsvThread.Columns.Add("로그 파일", 640);
            WcsCommon.cLogCols.Attach(lsvThread);
        }

        private Label NewInfoLabel(int pY)
        {
            Label lbl = new Label();
            lbl.Location = new Point(12, pY);
            lbl.Size = new Size(566, 20);
            lbl.AutoEllipsis = true;
            grpLog.Controls.Add(lbl);
            return lbl;
        }

        // ─────────────────────────────────────────────────────────────────
        private TaskHost Find(TaskKind pKind)
        {
            foreach (TaskHost th in m_lstTask)
                if (th.Kind == pKind) return th;
            return null;
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            DoStart(Find((TaskKind)((Button)sender).Tag));
            RefreshState();
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            DoStop(Find((TaskKind)((Button)sender).Tag));
            RefreshState();
        }

        private void btnRestart_Click(object sender, EventArgs e)
        {
            TaskHost th = Find((TaskKind)((Button)sender).Tag);
            DoStop(th);
            DoStart(th);
            RefreshState();
        }

        private void DoStart(TaskHost pTh)
        {
            if (pTh == null || pTh.IsRunning) return;

            Cursor = Cursors.WaitCursor;
            try
            {
                WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "IMP", "[" + pTh.Name + "] 기동 요청");
                if (!pTh.Start())
                    MessageBox.Show(this, "[" + pTh.Name + "] 기동 실패\r\n" + pTh.LastError,
                                    "ALL_TASK", MessageBoxButtons.OK, MessageBoxIcon.Error);
                else
                    WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "IMP", "[" + pTh.Name + "] 기동 완료");
            }
            finally { Cursor = Cursors.Default; }
        }

        private void DoStop(TaskHost pTh)
        {
            if (pTh == null || !pTh.IsRunning) return;

            Cursor = Cursors.WaitCursor;
            try
            {
                WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "IMP", "[" + pTh.Name + "] 정지 요청");
                pTh.Stop();
                WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "IMP", "[" + pTh.Name + "] 정지 완료");
            }
            finally { Cursor = Cursors.Default; }
        }

        // ─────────────────────────────────────────────────────────────────
        private void tmrState_Tick(object sender, EventArgs e)
        {
            tmrState.Enabled = false;
            try
            {
                RefreshState();

            }
            catch { }
            tmrState.Enabled = true;
        }

        private void RefreshState()
        {
            foreach (TaskHost th in m_lstTask)
            {
                bool bRun = th.IsRunning;
                Label lbl = m_dicLblState[th.Kind];
                lbl.Text = bRun ? "동작중" : "정지";
                lbl.BackColor = bRun ? Color.LimeGreen : Color.LightGray;
                lbl.ForeColor = bRun ? Color.White : Color.Black;
                m_dicBtnStart[th.Kind].Enabled = !bRun;
                m_dicBtnStop[th.Kind].Enabled = bRun;
            }

            // 스레드 목록
            lsvThread.BeginUpdate();
            try
            {
                lsvThread.Items.Clear();
                AddThreadRow("ALL", "ALL_TASK", true);
                foreach (TaskHost th in m_lstTask)
                {
                    string[] arr = th.ThreadStates();
                    if (arr.Length == 0)
                    {
                        AddThreadRow(th.Name, "(정지)", false);
                        continue;
                    }
                    foreach (string s in arr)
                    {
                        string[] kv = s.Split('|');
                        AddThreadRow(th.Name, kv[0], (kv.Length > 1 && kv[1] == "RUN"));
                    }
                }
            }
            finally { lsvThread.EndUpdate(); }

            m_lblLogQue.Text = string.Format("큐 {0}건 / 버림 {1}건",
                WcsCommon.cTaskLog.QueueCount, WcsCommon.cTaskLog.DroppedCount);

            int nRun = 0;
            foreach (TaskHost th in m_lstTask) if (th.IsRunning) nRun++;
            lblStatus.Text = string.Format("동작 태스크 {0}/3    로그 {1}", nRun, WcsCommon.cTaskLog.RootDir);
        }

        private void AddThreadRow(string pTask, string pThread, bool pRun)
        {
            ListViewItem it = new ListViewItem(pTask);
            it.SubItems.Add(pThread);
            it.SubItems.Add(pRun ? "RUN" : "STOP");

            string strFile = "";
            if (pThread.Length > 0 && pThread[0] != '(')
                strFile = Path.Combine(Path.Combine(WcsCommon.cTaskLog.RootDir, pThread),
                                       pThread + "_" + DateTime.Now.ToString("yyyyMMdd") + ".log");
            it.SubItems.Add(strFile);

            it.BackColor = pRun ? Color.FromArgb(230, 255, 230) : Color.FromArgb(245, 245, 245);
            lsvThread.Items.Add(it);
        }

        // ─────────────────────────────────────────────────────────────────
        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                int nRun = 0;
                foreach (TaskHost th in m_lstTask) if (th.IsRunning) nRun++;
                if (nRun > 0 &&
                    MessageBox.Show(this, "동작 중인 태스크 " + nRun + "개를 정지하고 종료합니다.\r\n계속하시겠습니까?",
                                    "ALL_TASK", MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes)
                {
                    e.Cancel = true;
                    return;
                }
            }

            tmrState.Enabled = false;
            foreach (TaskHost th in m_lstTask)
            {
                try { th.Stop(); } catch { }
            }

            WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "IMP", "ALL_TASK 종료");
            try { WcsCommon.cTaskLog.Shutdown(); } catch { }

            base.OnFormClosing(e);
        }
    }
}
