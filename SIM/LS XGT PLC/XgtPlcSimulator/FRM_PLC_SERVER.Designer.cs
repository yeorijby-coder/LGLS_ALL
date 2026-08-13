namespace XgtPlcSimulator
{
    partial class FRM_PLC_SERVER
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && components != null) components.Dispose();
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            // ── Declare ───────────────────────────────────────────────────────
            this.pnlConn         = new System.Windows.Forms.Panel();
            this.lblPort         = new System.Windows.Forms.Label();
            this.txtPort         = new System.Windows.Forms.TextBox();
            this.btnStart        = new System.Windows.Forms.Button();
            this.btnStop         = new System.Windows.Forms.Button();
            this.lblSrvStatus    = new System.Windows.Forms.Label();

            this.pnlClients      = new System.Windows.Forms.Panel();
            this.lblClientsTitle = new System.Windows.Forms.Label();
            this.lstClients      = new System.Windows.Forms.ListBox();

            this.tabMain         = new System.Windows.Forms.TabControl();
            this.tabBit          = new System.Windows.Forms.TabPage();
            this.dgvBit          = new System.Windows.Forms.DataGridView();
            this.pnlBitWrite     = new System.Windows.Forms.Panel();
            this.lblBitAddr      = new System.Windows.Forms.Label();
            this.txtBitAddr      = new System.Windows.Forms.TextBox();
            this.lblBitVal       = new System.Windows.Forms.Label();
            this.txtBitVal       = new System.Windows.Forms.TextBox();
            this.btnBitWrite     = new System.Windows.Forms.Button();

            this.tabWord         = new System.Windows.Forms.TabPage();
            this.dgvWord         = new System.Windows.Forms.DataGridView();
            this.pnlWordWrite    = new System.Windows.Forms.Panel();
            this.lblWordAddr     = new System.Windows.Forms.Label();
            this.txtWordAddr     = new System.Windows.Forms.TextBox();
            this.lblWordVal      = new System.Windows.Forms.Label();
            this.txtWordVal      = new System.Windows.Forms.TextBox();
            this.btnWordWrite    = new System.Windows.Forms.Button();

            this.tabTracking     = new System.Windows.Forms.TabPage();
            this.dgvTracking     = new System.Windows.Forms.DataGridView();
            this.pnlTrackWrite   = new System.Windows.Forms.Panel();
            this.lblTrackAddr    = new System.Windows.Forms.Label();
            this.txtTrackAddr    = new System.Windows.Forms.TextBox();
            this.lblTrackVal     = new System.Windows.Forms.Label();
            this.txtTrackVal     = new System.Windows.Forms.TextBox();
            this.btnTrackWrite   = new System.Windows.Forms.Button();

            this.tabB            = new System.Windows.Forms.TabPage();
            this.dgvB            = new System.Windows.Forms.DataGridView();
            this.pnlBWrite       = new System.Windows.Forms.Panel();
            this.lblBAddr        = new System.Windows.Forms.Label();
            this.txtBAddr        = new System.Windows.Forms.TextBox();
            this.lblBVal         = new System.Windows.Forms.Label();
            this.txtBVal         = new System.Windows.Forms.TextBox();
            this.btnBWrite       = new System.Windows.Forms.Button();

            this.tabW            = new System.Windows.Forms.TabPage();
            this.dgvW            = new System.Windows.Forms.DataGridView();
            this.pnlWWrite       = new System.Windows.Forms.Panel();
            this.lblWAddr        = new System.Windows.Forms.Label();
            this.txtWAddr        = new System.Windows.Forms.TextBox();
            this.lblWVal         = new System.Windows.Forms.Label();
            this.txtWVal         = new System.Windows.Forms.TextBox();
            this.btnWWrite       = new System.Windows.Forms.Button();

            this.tabManual       = new System.Windows.Forms.TabPage();
            this.pnlManualTop    = new System.Windows.Forms.Panel();
            this.lblManualDev    = new System.Windows.Forms.Label();
            this.cmbManualDev    = new System.Windows.Forms.ComboBox();
            this.lblManualAddr   = new System.Windows.Forms.Label();
            this.txtManualAddr   = new System.Windows.Forms.TextBox();
            this.lblManualCnt    = new System.Windows.Forms.Label();
            this.txtManualCnt    = new System.Windows.Forms.TextBox();
            this.btnManualView   = new System.Windows.Forms.Button();
            this.lblManualVal    = new System.Windows.Forms.Label();
            this.txtManualVal    = new System.Windows.Forms.TextBox();
            this.btnManualWrite  = new System.Windows.Forms.Button();
            this.dgvManual       = new System.Windows.Forms.DataGridView();

            this.txtLog          = new System.Windows.Forms.TextBox();

            // columns – Bit
            this.colBitAddr      = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBitHex       = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBitDec       = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBitBin       = new System.Windows.Forms.DataGridViewTextBoxColumn();
            // columns – Word
            this.colWordAddr     = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colWordHex      = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colWordDec      = new System.Windows.Forms.DataGridViewTextBoxColumn();
            // columns – Tracking
            this.colTrackAddr    = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTrackHex     = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTrackDec     = new System.Windows.Forms.DataGridViewTextBoxColumn();
            // columns – B (Link Relay)
            this.colBAddr        = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBHex         = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBDec         = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBBin         = new System.Windows.Forms.DataGridViewTextBoxColumn();
            // columns – W (Link Register)
            this.colWAddr        = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colWHex         = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colWDec         = new System.Windows.Forms.DataGridViewTextBoxColumn();
            // columns – Manual
            this.colManAddr      = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colManHex       = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colManDec       = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colManBin       = new System.Windows.Forms.DataGridViewTextBoxColumn();

            this.splitMain       = new System.Windows.Forms.SplitContainer();
            this.splitTop        = new System.Windows.Forms.SplitContainer();

            // ── Suspend ───────────────────────────────────────────────────────
            ((System.ComponentModel.ISupportInitialize)this.splitMain).BeginInit();
            this.splitMain.Panel1.SuspendLayout();
            this.splitMain.Panel2.SuspendLayout();
            this.splitMain.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)this.splitTop).BeginInit();
            this.splitTop.Panel1.SuspendLayout();
            this.splitTop.Panel2.SuspendLayout();
            this.splitTop.SuspendLayout();
            this.pnlConn.SuspendLayout();
            this.pnlClients.SuspendLayout();
            this.tabMain.SuspendLayout();
            this.tabBit.SuspendLayout();
            this.tabWord.SuspendLayout();
            this.tabTracking.SuspendLayout();
            this.tabB.SuspendLayout();
            this.tabW.SuspendLayout();
            this.tabManual.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)this.dgvBit).BeginInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvWord).BeginInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvTracking).BeginInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvB).BeginInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvW).BeginInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvManual).BeginInit();
            this.SuspendLayout();

            // ── pnlConn ───────────────────────────────────────────────────────
            this.pnlConn.Dock      = System.Windows.Forms.DockStyle.Top;
            this.pnlConn.Height    = 40;
            this.pnlConn.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.chkHexDump  = new System.Windows.Forms.CheckBox();
            this.btnClearLog = new System.Windows.Forms.Button();
            this.rdoSPL      = new System.Windows.Forms.RadioButton();
            this.rdoLGLS     = new System.Windows.Forms.RadioButton();
            this.lblBindIp   = new System.Windows.Forms.Label();
            this.txtBindIp   = new System.Windows.Forms.TextBox();

            this.pnlConn.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblBindIp, this.txtBindIp,
                this.lblPort, this.txtPort, this.btnStart, this.btnStop, this.lblSrvStatus });

            L(this.lblBindIp,   "IP:",        8,  12, 24);
            T(this.txtBindIp,   "0.0.0.0",  34,   8, 100);
            L(this.lblPort,     "Port:",    142,  12, 32);
            T(this.txtPort,     "3001",     178,   8, 60);
            B(this.btnStart,    "서버 실행", 246,  7, 90, System.Drawing.Color.FromArgb(0, 130, 60));
            B(this.btnStop,     "서버 종료", 344,  7, 90, System.Drawing.Color.FromArgb(180, 50, 50));
            this.btnStop.Enabled = false;

            this.lblSrvStatus.Text      = "● 중지";
            this.lblSrvStatus.ForeColor = System.Drawing.Color.OrangeRed;
            this.lblSrvStatus.Font      = new System.Drawing.Font("맑은 고딕", 10f,
                                            System.Drawing.FontStyle.Bold);
            this.lblSrvStatus.Location  = new System.Drawing.Point(444, 11);
            this.lblSrvStatus.AutoSize  = true;

            this.chkHexDump.Text      = "HEX 덤프";
            this.chkHexDump.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.chkHexDump.Location  = new System.Drawing.Point(574, 11);
            this.chkHexDump.AutoSize  = true;
            this.chkHexDump.Checked   = true;
            this.chkHexDump.CheckedChanged += new System.EventHandler(this.ChkHexDump_CheckedChanged);

            this.btnClearLog.Text      = "로그 지우기";
            this.btnClearLog.Location  = new System.Drawing.Point(664, 7);
            this.btnClearLog.Size      = new System.Drawing.Size(80, 26);
            this.btnClearLog.BackColor = System.Drawing.Color.FromArgb(70, 70, 75);
            this.btnClearLog.ForeColor = System.Drawing.Color.White;
            this.btnClearLog.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnClearLog.FlatAppearance.BorderSize = 0;
            this.btnClearLog.Click += new System.EventHandler(this.BtnClearLog_Click);

            this.rdoSPL.Text      = "SPL";
            this.rdoSPL.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.rdoSPL.Location  = new System.Drawing.Point(752, 11);
            this.rdoSPL.AutoSize  = true;
            this.rdoSPL.Checked   = true;
            this.rdoSPL.CheckedChanged += new System.EventHandler(this.RdoProtocol_CheckedChanged);

            this.rdoLGLS.Text      = "LGLS";
            this.rdoLGLS.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.rdoLGLS.Location  = new System.Drawing.Point(800, 11);
            this.rdoLGLS.AutoSize  = true;
            this.rdoLGLS.CheckedChanged += new System.EventHandler(this.RdoProtocol_CheckedChanged);

            this.pnlConn.Controls.Add(this.chkHexDump);
            this.pnlConn.Controls.Add(this.btnClearLog);
            this.pnlConn.Controls.Add(this.rdoSPL);
            this.pnlConn.Controls.Add(this.rdoLGLS);

            this.btnStart.Click += new System.EventHandler(this.BtnStart_Click);
            this.btnStop.Click  += new System.EventHandler(this.BtnStop_Click);

            // ── pnlClients (어드레스 패널 – splitTop.Panel1 에 배치) ────────────
            this.pnlClients.Dock      = System.Windows.Forms.DockStyle.Fill;
            this.pnlClients.BackColor = System.Drawing.Color.FromArgb(37, 37, 40);
            this.pnlClients.Padding   = new System.Windows.Forms.Padding(4, 22, 4, 2);
            this.pnlClients.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblClientsTitle, this.lstClients });

            this.lblClientsTitle.Text      = "접속 클라이언트";
            this.lblClientsTitle.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblClientsTitle.Font      = new System.Drawing.Font("맑은 고딕", 9f,
                                               System.Drawing.FontStyle.Bold);
            this.lblClientsTitle.Location  = new System.Drawing.Point(6, 4);
            this.lblClientsTitle.AutoSize  = true;

            this.lstClients.Dock            = System.Windows.Forms.DockStyle.Fill;
            this.lstClients.BackColor       = System.Drawing.Color.FromArgb(30, 30, 30);
            this.lstClients.ForeColor       = System.Drawing.Color.LightGreen;
            this.lstClients.Font            = new System.Drawing.Font("Consolas", 9f);
            this.lstClients.BorderStyle     = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lstClients.SelectionMode   = System.Windows.Forms.SelectionMode.None;

            // ── tabMain ───────────────────────────────────────────────────────
            this.tabMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabMain.Controls.AddRange(new System.Windows.Forms.TabPage[] {
                this.tabBit, this.tabWord, this.tabTracking, this.tabB, this.tabW, this.tabManual });
            this.tabMain.Font = new System.Drawing.Font("맑은 고딕", 9f);

            // ── tabBit ────────────────────────────────────────────────────────
            this.tabBit.Text    = "Bit 영역 (%MX)";
            this.tabBit.Padding = new System.Windows.Forms.Padding(3);
            this.tabBit.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.pnlBitWrite, this.dgvBit });

            this.pnlBitWrite.Dock   = System.Windows.Forms.DockStyle.Bottom;
            this.pnlBitWrite.Height = 38;
            this.pnlBitWrite.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlBitWrite.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblBitAddr, this.txtBitAddr, this.lblBitVal, this.txtBitVal, this.btnBitWrite });
            L(this.lblBitAddr, "주소 (MW#):", 6, 10, 80);
            T(this.txtBitAddr, "0",          90,  6, 60);
            L(this.lblBitVal,  "값 (Hex/Dec):", 160, 10, 90);
            T(this.txtBitVal,  "0x0000",     254,  6, 100);
            B(this.btnBitWrite, "강제 Write", 362, 5, 90,
              System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnBitWrite.Click += new System.EventHandler(this.BtnBitWrite_Click);

            this.dgvBit.Dock = System.Windows.Forms.DockStyle.Fill;
            DgvStyle(this.dgvBit);
            Col(this.colBitAddr, "colBitAddr", "주소(MW#)",      80,  false);
            Col(this.colBitHex,  "colBitHex",  "Hex",            80,  false);
            Col(this.colBitDec,  "colBitDec",  "Dec",            80,  false);
            Col(this.colBitBin,  "colBitBin",  "Binary (B15..B0)", 0, true);
            this.dgvBit.Columns.AddRange(this.colBitAddr, this.colBitHex,
                                          this.colBitDec,  this.colBitBin);

            // ── tabWord ───────────────────────────────────────────────────────
            this.tabWord.Text    = "Word 영역 (%DW)";
            this.tabWord.Padding = new System.Windows.Forms.Padding(3);
            this.tabWord.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.pnlWordWrite, this.dgvWord });

            this.pnlWordWrite.Dock      = System.Windows.Forms.DockStyle.Bottom;
            this.pnlWordWrite.Height    = 38;
            this.pnlWordWrite.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlWordWrite.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblWordAddr, this.txtWordAddr, this.lblWordVal, this.txtWordVal, this.btnWordWrite });
            L(this.lblWordAddr, "주소 (DW#):", 6, 10, 80);
            T(this.txtWordAddr, "0",           90,  6, 60);
            L(this.lblWordVal,  "값 (Hex/Dec):", 160, 10, 90);
            T(this.txtWordVal,  "0x0000",      254,  6, 100);
            B(this.btnWordWrite, "강제 Write", 362, 5, 90,
              System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnWordWrite.Click += new System.EventHandler(this.BtnWordWrite_Click);

            this.dgvWord.Dock = System.Windows.Forms.DockStyle.Fill;
            DgvStyle(this.dgvWord);
            Col(this.colWordAddr, "colWordAddr", "주소(DW#)", 80,  false);
            Col(this.colWordHex,  "colWordHex",  "Hex",       100, false);
            Col(this.colWordDec,  "colWordDec",  "Dec",       0,   true);
            this.dgvWord.Columns.AddRange(this.colWordAddr, this.colWordHex, this.colWordDec);

            // ── tabTracking ───────────────────────────────────────────────────
            this.tabTracking.Text    = "Tracking 영역 (%RW)";
            this.tabTracking.Padding = new System.Windows.Forms.Padding(3);
            this.tabTracking.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.pnlTrackWrite, this.dgvTracking });

            this.pnlTrackWrite.Dock      = System.Windows.Forms.DockStyle.Bottom;
            this.pnlTrackWrite.Height    = 38;
            this.pnlTrackWrite.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlTrackWrite.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblTrackAddr, this.txtTrackAddr, this.lblTrackVal, this.txtTrackVal, this.btnTrackWrite });
            L(this.lblTrackAddr, "주소 (RW#):", 6, 10, 80);
            T(this.txtTrackAddr, "0",           90,  6, 60);
            L(this.lblTrackVal,  "값 (Hex/Dec):", 160, 10, 90);
            T(this.txtTrackVal,  "0x0000",      254,  6, 100);
            B(this.btnTrackWrite, "강제 Write", 362, 5, 90,
              System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnTrackWrite.Click += new System.EventHandler(this.BtnTrackWrite_Click);

            this.dgvTracking.Dock = System.Windows.Forms.DockStyle.Fill;
            DgvStyle(this.dgvTracking);
            Col(this.colTrackAddr, "colTrackAddr", "주소(RW#)", 80,  false);
            Col(this.colTrackHex,  "colTrackHex",  "Hex",       100, false);
            Col(this.colTrackDec,  "colTrackDec",  "Dec",       0,   true);
            this.dgvTracking.Columns.AddRange(this.colTrackAddr, this.colTrackHex, this.colTrackDec);

            // ── tabB (Link Relay) ─────────────────────────────────────────────
            this.tabB.Text    = "Link Relay (%BW)";
            this.tabB.Padding = new System.Windows.Forms.Padding(3);
            this.tabB.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.pnlBWrite, this.dgvB });

            this.pnlBWrite.Dock      = System.Windows.Forms.DockStyle.Bottom;
            this.pnlBWrite.Height    = 38;
            this.pnlBWrite.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlBWrite.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblBAddr, this.txtBAddr, this.lblBVal, this.txtBVal, this.btnBWrite });
            L(this.lblBAddr, "주소 (BW#):", 6, 10, 80);
            T(this.txtBAddr, "0",           90,  6, 60);
            L(this.lblBVal,  "값 (Hex/Dec):", 160, 10, 90);
            T(this.txtBVal,  "0x0000",      254,  6, 100);
            B(this.btnBWrite, "강제 Write", 362, 5, 90,
              System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnBWrite.Click += new System.EventHandler(this.BtnBWrite_Click);

            this.dgvB.Dock = System.Windows.Forms.DockStyle.Fill;
            DgvStyle(this.dgvB);
            Col(this.colBAddr, "colBAddr", "주소(BW#)",       80, false);
            Col(this.colBHex,  "colBHex",  "Hex",             80, false);
            Col(this.colBDec,  "colBDec",  "Dec",             80, false);
            Col(this.colBBin,  "colBBin",  "Binary (B15..B0)", 0, true);
            this.dgvB.Columns.AddRange(this.colBAddr, this.colBHex,
                                        this.colBDec,  this.colBBin);

            // ── tabW (Link Register) ──────────────────────────────────────────
            this.tabW.Text    = "Link Register (%WW)";
            this.tabW.Padding = new System.Windows.Forms.Padding(3);
            this.tabW.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.pnlWWrite, this.dgvW });

            this.pnlWWrite.Dock      = System.Windows.Forms.DockStyle.Bottom;
            this.pnlWWrite.Height    = 38;
            this.pnlWWrite.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlWWrite.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblWAddr, this.txtWAddr, this.lblWVal, this.txtWVal, this.btnWWrite });
            L(this.lblWAddr, "주소 (WW#):", 6, 10, 80);
            T(this.txtWAddr, "0",           90,  6, 60);
            L(this.lblWVal,  "값 (Hex/Dec):", 160, 10, 90);
            T(this.txtWVal,  "0x0000",      254,  6, 100);
            B(this.btnWWrite, "강제 Write", 362, 5, 90,
              System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnWWrite.Click += new System.EventHandler(this.BtnWWrite_Click);

            this.dgvW.Dock = System.Windows.Forms.DockStyle.Fill;
            DgvStyle(this.dgvW);
            Col(this.colWAddr, "colWAddr", "주소(WW#)", 80, false);
            Col(this.colWHex,  "colWHex",  "Hex",       100, false);
            Col(this.colWDec,  "colWDec",  "Dec",       0,   true);
            this.dgvW.Columns.AddRange(this.colWAddr, this.colWHex, this.colWDec);

            // ── tabManual (직접 읽기/쓰기) ─────────────────────────────────────
            this.tabManual.Text    = "직접 읽기/쓰기";
            this.tabManual.Padding = new System.Windows.Forms.Padding(3);
            this.tabManual.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.pnlManualTop, this.dgvManual });

            this.pnlManualTop.Dock      = System.Windows.Forms.DockStyle.Top;
            this.pnlManualTop.Height    = 42;
            this.pnlManualTop.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlManualTop.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblManualDev, this.cmbManualDev,
                this.lblManualAddr, this.txtManualAddr,
                this.lblManualCnt,  this.txtManualCnt,
                this.btnManualView,
                this.lblManualVal,  this.txtManualVal,
                this.btnManualWrite });

            // Device: [%MB ▼]  시작 주소: [0]  워드 수: [10]  [Read]  Write 값: [0x0000]  [Write]
            L(this.lblManualDev, "Device:", 4, 13, 50);
            this.cmbManualDev.Items.AddRange(new object[] {
                "%MB", "%MW", "%DB", "%DW", "%RW", "%RB", "%BW", "%WW" });
            this.cmbManualDev.SelectedIndex = 0;
            this.cmbManualDev.Location      = new System.Drawing.Point(54, 10);
            this.cmbManualDev.Size          = new System.Drawing.Size(72, 22);
            this.cmbManualDev.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbManualDev.BackColor     = System.Drawing.Color.FromArgb(60, 60, 63);
            this.cmbManualDev.ForeColor     = System.Drawing.Color.WhiteSmoke;
            this.cmbManualDev.Font          = new System.Drawing.Font("Consolas", 9f);

            L(this.lblManualAddr, "시작 주소:", 134, 13, 64);
            T(this.txtManualAddr, "0",          202, 10, 70);
            L(this.lblManualCnt,  "워드 수:",   280, 13, 52);
            T(this.txtManualCnt,  "10",         336, 10, 52);
            B(this.btnManualView, "Read",        396, 9, 65,
              System.Drawing.Color.FromArgb(0, 102, 204));
            L(this.lblManualVal,  "Write 값:",  470, 13, 62);
            T(this.txtManualVal,  "0x0000",     536, 10, 90);
            B(this.btnManualWrite, "Write",     634, 9, 65,
              System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnManualView.Click  += new System.EventHandler(this.BtnManualView_Click);
            this.btnManualWrite.Click += new System.EventHandler(this.BtnManualWrite_Click);

            this.dgvManual.Dock = System.Windows.Forms.DockStyle.Fill;
            DgvStyle(this.dgvManual);
            Col(this.colManAddr, "colManAddr", "주소",            90,  false);
            Col(this.colManHex,  "colManHex",  "Hex",             90,  false);
            Col(this.colManDec,  "colManDec",  "Dec",             90,  false);
            Col(this.colManBin,  "colManBin",  "Binary(B15..B0)", 0,   true);
            this.dgvManual.Columns.AddRange(this.colManAddr, this.colManHex,
                                             this.colManDec,  this.colManBin);

            // ── SplitContainer 구성 ───────────────────────────────────────────
            // splitTop : 어드레스(클라이언트) 패널 ↕ 탭 콘텐츠
            this.splitTop.Dock             = System.Windows.Forms.DockStyle.Fill;
            this.splitTop.Orientation      = System.Windows.Forms.Orientation.Horizontal;
            this.splitTop.SplitterDistance = 80;
            this.splitTop.SplitterWidth    = 6;
            this.splitTop.Panel1MinSize    = 40;
            this.splitTop.Panel2MinSize    = 100;
            this.splitTop.IsSplitterFixed  = false;
            this.splitTop.FixedPanel       = System.Windows.Forms.FixedPanel.None;
            this.splitTop.BackColor        = System.Drawing.Color.FromArgb(63, 63, 70);
            this.splitTop.Panel1.Controls.Add(this.pnlClients);
            this.splitTop.Panel2.Controls.Add(this.tabMain);

            // splitMain : (어드레스+탭) ↕ 로그 패널
            this.splitMain.Dock             = System.Windows.Forms.DockStyle.Fill;
            this.splitMain.Orientation      = System.Windows.Forms.Orientation.Horizontal;
            this.splitMain.SplitterDistance = 530;
            this.splitMain.SplitterWidth    = 6;
            this.splitMain.Panel1MinSize    = 100;
            this.splitMain.Panel2MinSize    = 40;
            this.splitMain.IsSplitterFixed  = false;
            this.splitMain.FixedPanel       = System.Windows.Forms.FixedPanel.None;
            this.splitMain.BackColor        = System.Drawing.Color.FromArgb(63, 63, 70);
            this.splitMain.Panel1.Controls.Add(this.splitTop);
            this.splitMain.Panel2.Controls.Add(this.txtLog);

            // ── txtLog (로그 패널 – splitMain.Panel2 에 배치) ────────────────
            this.txtLog.Dock       = System.Windows.Forms.DockStyle.Fill;
            this.txtLog.Multiline  = true;
            this.txtLog.ReadOnly   = true;
            this.txtLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtLog.BackColor  = System.Drawing.Color.FromArgb(20, 20, 20);
            this.txtLog.ForeColor  = System.Drawing.Color.LightGreen;
            this.txtLog.Font       = new System.Drawing.Font("Consolas", 9f);

            // ── Form ──────────────────────────────────────────────────────────
            this.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.splitMain, this.pnlConn });
            this.Text          = "LS XGT PLC 시뮬레이터 [LG생명과학]";
            this.Size          = new System.Drawing.Size(1100, 760);
            this.MinimumSize   = new System.Drawing.Size(800, 600);
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.BackColor     = System.Drawing.Color.FromArgb(37, 37, 38);
            this.ForeColor     = System.Drawing.Color.WhiteSmoke;
            this.Font          = new System.Drawing.Font("맑은 고딕", 9f);

            // ── Resume ────────────────────────────────────────────────────────
            this.splitTop.Panel1.ResumeLayout(false);
            this.splitTop.Panel2.ResumeLayout(false);
            this.splitTop.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)this.splitTop).EndInit();
            this.splitMain.Panel1.ResumeLayout(false);
            this.splitMain.Panel2.ResumeLayout(false);
            this.splitMain.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)this.splitMain).EndInit();
            this.pnlConn.ResumeLayout(false);
            this.pnlClients.ResumeLayout(false);
            this.tabMain.ResumeLayout(false);
            this.tabBit.ResumeLayout(false);
            this.tabWord.ResumeLayout(false);
            this.tabTracking.ResumeLayout(false);
            this.tabB.ResumeLayout(false);
            this.tabW.ResumeLayout(false);
            this.tabManual.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)this.dgvBit).EndInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvWord).EndInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvTracking).EndInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvB).EndInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvW).EndInit();
            ((System.ComponentModel.ISupportInitialize)this.dgvManual).EndInit();
            this.ResumeLayout(false);
        }

        // ── helper builders ────────────────────────────────────────────────────

        private static void L(System.Windows.Forms.Label l, string text, int x, int y, int w)
        {
            l.Text      = text;
            l.Location  = new System.Drawing.Point(x, y);
            l.Size      = new System.Drawing.Size(w, 18);
            l.ForeColor = System.Drawing.Color.WhiteSmoke;
            l.AutoSize  = false;
        }
        private static void T(System.Windows.Forms.TextBox t, string def, int x, int y, int w)
        {
            t.Text     = def;
            t.Location = new System.Drawing.Point(x, y);
            t.Size     = new System.Drawing.Size(w, 22);
        }
        private static void B(System.Windows.Forms.Button b, string text,
            int x, int y, int w, System.Drawing.Color bc)
        {
            b.Text      = text;
            b.Location  = new System.Drawing.Point(x, y);
            b.Size      = new System.Drawing.Size(w, 26);
            b.BackColor = bc;
            b.ForeColor = System.Drawing.Color.White;
            b.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            b.FlatAppearance.BorderSize = 0;
        }
        private static void DgvStyle(System.Windows.Forms.DataGridView g)
        {
            g.ReadOnly              = true;
            g.SelectionMode         = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            g.MultiSelect           = false;
            g.AllowUserToAddRows    = false;
            g.AllowUserToDeleteRows = false;
            g.RowHeadersWidth       = 30;
            g.BackgroundColor       = System.Drawing.Color.FromArgb(37, 37, 38);
            g.ForeColor             = System.Drawing.Color.WhiteSmoke;
            g.GridColor             = System.Drawing.Color.FromArgb(80, 80, 80);
            g.DefaultCellStyle.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            g.DefaultCellStyle.ForeColor = System.Drawing.Color.WhiteSmoke;
            g.ColumnHeadersDefaultCellStyle.BackColor = System.Drawing.Color.FromArgb(63, 63, 70);
            g.ColumnHeadersDefaultCellStyle.ForeColor = System.Drawing.Color.WhiteSmoke;
            g.ColumnHeadersDefaultCellStyle.Font =
                new System.Drawing.Font("맑은 고딕", 9f, System.Drawing.FontStyle.Bold);
            g.AlternatingRowsDefaultCellStyle.BackColor = System.Drawing.Color.FromArgb(55, 55, 58);
            g.EnableHeadersVisualStyles = false;
            g.AutoSizeRowsMode          = System.Windows.Forms.DataGridViewAutoSizeRowsMode.None;
            g.RowTemplate.Height        = 22;
        }
        private static void Col(System.Windows.Forms.DataGridViewTextBoxColumn c,
            string name, string header, int width, bool fill)
        {
            c.Name       = name;
            c.HeaderText = header;
            if (fill) c.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            else       c.Width       = width;
        }

        #endregion

        // Controls
        private System.Windows.Forms.CheckBox     chkHexDump;
        private System.Windows.Forms.Button       btnClearLog;
        private System.Windows.Forms.RadioButton  rdoSPL;
        private System.Windows.Forms.RadioButton  rdoLGLS;
        private System.Windows.Forms.Label        lblBindIp;
        private System.Windows.Forms.TextBox      txtBindIp;
        private System.Windows.Forms.Panel    pnlConn;
        private System.Windows.Forms.Label    lblPort;
        private System.Windows.Forms.TextBox  txtPort;
        private System.Windows.Forms.Button   btnStart;
        private System.Windows.Forms.Button   btnStop;
        private System.Windows.Forms.Label    lblSrvStatus;

        private System.Windows.Forms.Panel    pnlClients;
        private System.Windows.Forms.Label    lblClientsTitle;
        private System.Windows.Forms.ListBox  lstClients;

        private System.Windows.Forms.TabControl tabMain;

        private System.Windows.Forms.TabPage    tabBit;
        private System.Windows.Forms.DataGridView dgvBit;
        private System.Windows.Forms.Panel      pnlBitWrite;
        private System.Windows.Forms.Label      lblBitAddr;
        private System.Windows.Forms.TextBox    txtBitAddr;
        private System.Windows.Forms.Label      lblBitVal;
        private System.Windows.Forms.TextBox    txtBitVal;
        private System.Windows.Forms.Button     btnBitWrite;

        private System.Windows.Forms.TabPage    tabWord;
        private System.Windows.Forms.DataGridView dgvWord;
        private System.Windows.Forms.Panel      pnlWordWrite;
        private System.Windows.Forms.Label      lblWordAddr;
        private System.Windows.Forms.TextBox    txtWordAddr;
        private System.Windows.Forms.Label      lblWordVal;
        private System.Windows.Forms.TextBox    txtWordVal;
        private System.Windows.Forms.Button     btnWordWrite;

        private System.Windows.Forms.TabPage    tabTracking;
        private System.Windows.Forms.DataGridView dgvTracking;
        private System.Windows.Forms.Panel      pnlTrackWrite;
        private System.Windows.Forms.Label      lblTrackAddr;
        private System.Windows.Forms.TextBox    txtTrackAddr;
        private System.Windows.Forms.Label      lblTrackVal;
        private System.Windows.Forms.TextBox    txtTrackVal;
        private System.Windows.Forms.Button     btnTrackWrite;

        private System.Windows.Forms.TabPage    tabB;
        private System.Windows.Forms.DataGridView dgvB;
        private System.Windows.Forms.Panel      pnlBWrite;
        private System.Windows.Forms.Label      lblBAddr;
        private System.Windows.Forms.TextBox    txtBAddr;
        private System.Windows.Forms.Label      lblBVal;
        private System.Windows.Forms.TextBox    txtBVal;
        private System.Windows.Forms.Button     btnBWrite;

        private System.Windows.Forms.TabPage    tabW;
        private System.Windows.Forms.DataGridView dgvW;
        private System.Windows.Forms.Panel      pnlWWrite;
        private System.Windows.Forms.Label      lblWAddr;
        private System.Windows.Forms.TextBox    txtWAddr;
        private System.Windows.Forms.Label      lblWVal;
        private System.Windows.Forms.TextBox    txtWVal;
        private System.Windows.Forms.Button     btnWWrite;

        private System.Windows.Forms.TabPage    tabManual;
        private System.Windows.Forms.Panel      pnlManualTop;
        private System.Windows.Forms.Label      lblManualDev;
        private System.Windows.Forms.ComboBox   cmbManualDev;
        private System.Windows.Forms.Label      lblManualAddr;
        private System.Windows.Forms.TextBox    txtManualAddr;
        private System.Windows.Forms.Label      lblManualCnt;
        private System.Windows.Forms.TextBox    txtManualCnt;
        private System.Windows.Forms.Button     btnManualView;
        private System.Windows.Forms.Label      lblManualVal;
        private System.Windows.Forms.TextBox    txtManualVal;
        private System.Windows.Forms.Button     btnManualWrite;
        private System.Windows.Forms.DataGridView dgvManual;

        private System.Windows.Forms.TextBox    txtLog;

        private System.Windows.Forms.SplitContainer splitMain;
        private System.Windows.Forms.SplitContainer splitTop;

        // DGV Columns
        private System.Windows.Forms.DataGridViewTextBoxColumn colBitAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBitHex;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBitDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBitBin;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWordAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWordHex;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWordDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTrackAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTrackHex;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTrackDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBHex;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBBin;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWHex;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colManAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colManHex;
        private System.Windows.Forms.DataGridViewTextBoxColumn colManDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colManBin;
    }
}
