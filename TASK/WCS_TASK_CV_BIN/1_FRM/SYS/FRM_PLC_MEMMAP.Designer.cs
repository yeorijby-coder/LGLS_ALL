namespace WCS_TASK_CV
{
    partial class FRM_PLC_MEMMAP
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
                components.Dispose();
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code
        private void InitializeComponent()
        {
            this.pnlConn        = new System.Windows.Forms.Panel();
            this.lblIp          = new System.Windows.Forms.Label();
            this.txtIp          = new System.Windows.Forms.TextBox();
            this.lblPort        = new System.Windows.Forms.Label();
            this.txtPort        = new System.Windows.Forms.TextBox();
            this.btnConnect     = new System.Windows.Forms.Button();
            this.btnDisconnect  = new System.Windows.Forms.Button();
            this.lblConnStatus  = new System.Windows.Forms.Label();
            this.chkProtoLog    = new System.Windows.Forms.CheckBox();
            this.tabMain        = new System.Windows.Forms.TabControl();
            this.tabBit         = new System.Windows.Forms.TabPage();
            this.dgvBit         = new System.Windows.Forms.DataGridView();
            this.pnlBitBtn      = new System.Windows.Forms.Panel();
            this.btnReadBit     = new System.Windows.Forms.Button();
            this.lblWriteBitVal = new System.Windows.Forms.Label();
            this.txtWriteBitVal = new System.Windows.Forms.TextBox();
            this.btnWriteBit    = new System.Windows.Forms.Button();
            this.tabWord        = new System.Windows.Forms.TabPage();
            this.dgvWord        = new System.Windows.Forms.DataGridView();
            this.pnlWordBtn     = new System.Windows.Forms.Panel();
            this.btnReadWord    = new System.Windows.Forms.Button();
            this.lblWriteWordVal= new System.Windows.Forms.Label();
            this.txtWriteWordVal= new System.Windows.Forms.TextBox();
            this.btnWriteWord   = new System.Windows.Forms.Button();
            this.tabTracking    = new System.Windows.Forms.TabPage();
            this.dgvTracking    = new System.Windows.Forms.DataGridView();
            this.pnlTrackBtn    = new System.Windows.Forms.Panel();
            this.btnReadTracking = new System.Windows.Forms.Button();
            this.lblWriteTrackVal= new System.Windows.Forms.Label();
            this.txtWriteTrackVal= new System.Windows.Forms.TextBox();
            this.btnWriteTracking = new System.Windows.Forms.Button();
            this.tabCvWrite     = new System.Windows.Forms.TabPage();
            this.btnCvWrite11   = new System.Windows.Forms.Button();
            this.btnCvWrite12   = new System.Windows.Forms.Button();
            this.btnCvWrite15   = new System.Windows.Forms.Button();
            this.tabManual      = new System.Windows.Forms.TabPage();
            this.pnlManualTop   = new System.Windows.Forms.Panel();
            this.lblManualDev   = new System.Windows.Forms.Label();
            this.cmbManualDev   = new System.Windows.Forms.ComboBox();
            this.lblManualAddr  = new System.Windows.Forms.Label();
            this.txtManualAddr  = new System.Windows.Forms.TextBox();
            this.lblManualCnt   = new System.Windows.Forms.Label();
            this.txtManualCnt   = new System.Windows.Forms.TextBox();
            this.btnManualRead  = new System.Windows.Forms.Button();
            this.lblManualWriteVal = new System.Windows.Forms.Label();
            this.txtManualWriteVal = new System.Windows.Forms.TextBox();
            this.btnManualWrite = new System.Windows.Forms.Button();
            this.dgvManual      = new System.Windows.Forms.DataGridView();
            this.lblPptCap      = new System.Windows.Forms.Label();
            this.cmbPptDev      = new System.Windows.Forms.ComboBox();
            this.txtPptAddr     = new System.Windows.Forms.TextBox();
            this.btnPptCalc     = new System.Windows.Forms.Button();
            this.lblPptResult   = new System.Windows.Forms.Label();
            this.lblPptLen      = new System.Windows.Forms.Label();
            this.txtPptLen      = new System.Windows.Forms.TextBox();
            this.btnPptRead     = new System.Windows.Forms.Button();
            this.txtPptVal      = new System.Windows.Forms.TextBox();
            this.btnPptWrite    = new System.Windows.Forms.Button();
            this.txtLog         = new System.Windows.Forms.TextBox();
            this.splLog         = new System.Windows.Forms.Splitter();
            // columns
            this.colBitAddr    = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBitDesc    = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBitRaw     = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBitDec     = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBitBinary  = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colWordAddr   = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colWordDesc   = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colWordRaw    = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colWordDec    = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTrackAddr  = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTrackDesc  = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTrackRaw   = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colTrackDec   = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colManAddr    = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colManRaw     = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colManDec     = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colManBin     = new System.Windows.Forms.DataGridViewTextBoxColumn();

            this.pnlConn.SuspendLayout();
            this.tabMain.SuspendLayout();
            this.tabBit.SuspendLayout();
            this.tabWord.SuspendLayout();
            this.tabTracking.SuspendLayout();
            this.tabManual.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvBit)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvWord)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvTracking)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvManual)).BeginInit();
            this.SuspendLayout();

            // ── pnlConn ──────────────────────────────────────────────────────
            this.pnlConn.Dock      = System.Windows.Forms.DockStyle.Top;
            this.pnlConn.Height    = 40;
            this.pnlConn.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlConn.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblIp, this.txtIp, this.lblPort, this.txtPort,
                this.btnConnect, this.btnDisconnect, this.lblConnStatus, this.chkProtoLog });

            SetLabel(this.lblIp, "PLC IP:", 8, 10, 55);
            SetTextBox(this.txtIp, "127.0.0.1", 66, 8, 120);
            SetLabel(this.lblPort, "Port:", 200, 10, 40);
            SetTextBox(this.txtPort, "3001", 244, 8, 60);
            SetButton(this.btnConnect, "연결", 318, 7, 70, System.Drawing.Color.FromArgb(0, 122, 204));
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            SetButton(this.btnDisconnect, "해제", 398, 7, 70, System.Drawing.Color.FromArgb(180, 60, 60));
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            this.btnDisconnect.Enabled = false;

            this.lblConnStatus.Text      = "● 미연결";
            this.lblConnStatus.ForeColor = System.Drawing.Color.Red;
            this.lblConnStatus.Font      = new System.Drawing.Font("맑은 고딕", 10f, System.Drawing.FontStyle.Bold);
            this.lblConnStatus.Location  = new System.Drawing.Point(480, 10);
            this.lblConnStatus.AutoSize  = true;

            this.chkProtoLog.Text      = "프로토콜 로그";
            this.chkProtoLog.Location  = new System.Drawing.Point(600, 10);
            this.chkProtoLog.Size      = new System.Drawing.Size(110, 20);
            this.chkProtoLog.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.chkProtoLog.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);

            // ── tabMain ───────────────────────────────────────────────────────
            this.tabMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabMain.Controls.AddRange(new System.Windows.Forms.TabPage[] {
                this.tabBit, this.tabWord, this.tabTracking, this.tabManual, this.tabCvWrite });
            this.tabMain.Font = new System.Drawing.Font("맑은 고딕", 9f);

            // ── tabBit ───────────────────────────────────────────────────────
            this.tabBit.Text    = "Bit 영역 (%MX)";
            this.tabBit.Padding = new System.Windows.Forms.Padding(3);
            this.tabBit.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.dgvBit, this.pnlBitBtn });

            this.pnlBitBtn.Dock      = System.Windows.Forms.DockStyle.Bottom;
            this.pnlBitBtn.Height    = 38;
            this.pnlBitBtn.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlBitBtn.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.btnReadBit, this.lblWriteBitVal, this.txtWriteBitVal, this.btnWriteBit });
            SetButton(this.btnReadBit, "전체 Read", 4, 5, 90, System.Drawing.Color.FromArgb(0, 122, 204));
            this.btnReadBit.Click += new System.EventHandler(this.btnReadBit_Click);
            SetLabel(this.lblWriteBitVal, "선택행 Write 값(Hex/Dec):", 108, 10, 160);
            SetTextBox(this.txtWriteBitVal, "0x0001", 272, 6, 100);
            SetButton(this.btnWriteBit, "Write", 380, 5, 70, System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnWriteBit.Click += new System.EventHandler(this.btnWriteBit_Click);

            this.dgvBit.Dock = System.Windows.Forms.DockStyle.Fill;
            ConfigDgv(this.dgvBit);
            this.colBitAddr.Name       = "colBitAddr";   this.colBitAddr.HeaderText   = "주소";   this.colBitAddr.Width  = 70;
            this.colBitDesc.Name       = "colBitDesc";   this.colBitDesc.HeaderText   = "설명";   this.colBitDesc.Width  = 360; this.colBitDesc.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colBitRaw.Name        = "colBitRaw";    this.colBitRaw.HeaderText    = "Hex";    this.colBitRaw.Width   = 70;
            this.colBitDec.Name        = "colBitDec";    this.colBitDec.HeaderText    = "Dec";    this.colBitDec.Width   = 60;
            this.colBitBinary.Name     = "colBitBinary"; this.colBitBinary.HeaderText = "Binary(B15..B0)"; this.colBitBinary.Width = 140;
            this.dgvBit.Columns.AddRange(this.colBitAddr, this.colBitDesc, this.colBitRaw, this.colBitDec, this.colBitBinary);

            // ── tabWord ──────────────────────────────────────────────────────
            this.tabWord.Text    = "Word 영역 (%DB)";
            this.tabWord.Padding = new System.Windows.Forms.Padding(3);
            this.tabWord.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.dgvWord, this.pnlWordBtn });

            this.pnlWordBtn.Dock      = System.Windows.Forms.DockStyle.Bottom;
            this.pnlWordBtn.Height    = 38;
            this.pnlWordBtn.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlWordBtn.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.btnReadWord, this.lblWriteWordVal, this.txtWriteWordVal, this.btnWriteWord });
            SetButton(this.btnReadWord, "전체 Read", 4, 5, 90, System.Drawing.Color.FromArgb(0, 122, 204));
            this.btnReadWord.Click += new System.EventHandler(this.btnReadWord_Click);
            SetLabel(this.lblWriteWordVal, "선택행 Write 값(Hex/Dec):", 108, 10, 160);
            SetTextBox(this.txtWriteWordVal, "0x0000", 272, 6, 100);
            SetButton(this.btnWriteWord, "Write", 380, 5, 70, System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnWriteWord.Click += new System.EventHandler(this.btnWriteWord_Click);

            this.dgvWord.Dock = System.Windows.Forms.DockStyle.Fill;
            ConfigDgv(this.dgvWord);
            this.colWordAddr.Name = "colWordAddr"; this.colWordAddr.HeaderText = "주소"; this.colWordAddr.Width = 70;
            this.colWordDesc.Name = "colWordDesc"; this.colWordDesc.HeaderText = "설명"; this.colWordDesc.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colWordRaw.Name  = "colWordRaw";  this.colWordRaw.HeaderText  = "Hex";  this.colWordRaw.Width  = 70;
            this.colWordDec.Name  = "colWordDec";  this.colWordDec.HeaderText  = "Dec";  this.colWordDec.Width  = 70;
            this.dgvWord.Columns.AddRange(this.colWordAddr, this.colWordDesc, this.colWordRaw, this.colWordDec);

            // ── tabTracking ──────────────────────────────────────────────────
            this.tabTracking.Text    = "Tracking 영역 (R→%RB)";
            this.tabTracking.Padding = new System.Windows.Forms.Padding(3);
            this.tabTracking.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.dgvTracking, this.pnlTrackBtn });

            this.pnlTrackBtn.Dock      = System.Windows.Forms.DockStyle.Bottom;
            this.pnlTrackBtn.Height    = 38;
            this.pnlTrackBtn.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlTrackBtn.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.btnReadTracking, this.lblWriteTrackVal, this.txtWriteTrackVal, this.btnWriteTracking });
            SetButton(this.btnReadTracking, "전체 Read", 4, 5, 90, System.Drawing.Color.FromArgb(0, 122, 204));
            this.btnReadTracking.Click += new System.EventHandler(this.btnReadTracking_Click);
            SetLabel(this.lblWriteTrackVal, "선택행 Write 값(Hex/Dec):", 108, 10, 160);
            SetTextBox(this.txtWriteTrackVal, "0x0000", 272, 6, 100);
            SetButton(this.btnWriteTracking, "Write", 380, 5, 70, System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnWriteTracking.Click += new System.EventHandler(this.btnWriteTracking_Click);

            this.dgvTracking.Dock = System.Windows.Forms.DockStyle.Fill;
            ConfigDgv(this.dgvTracking);
            // [LGLS 2026-08-19] R 주소모드(16진)에서 "R0300 (%RB1536)" 처럼 길어져 잘리므로 폭 확대
            this.colTrackAddr.Name = "colTrackAddr"; this.colTrackAddr.HeaderText = "주소"; this.colTrackAddr.Width = 150;
            this.colTrackDesc.Name = "colTrackDesc"; this.colTrackDesc.HeaderText = "설명"; this.colTrackDesc.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.colTrackRaw.Name  = "colTrackRaw";  this.colTrackRaw.HeaderText  = "Hex";  this.colTrackRaw.Width  = 70;
            this.colTrackDec.Name  = "colTrackDec";  this.colTrackDec.HeaderText  = "Dec";  this.colTrackDec.Width  = 70;
            this.dgvTracking.Columns.AddRange(this.colTrackAddr, this.colTrackDesc, this.colTrackRaw, this.colTrackDec);

            // ── tabManual ────────────────────────────────────────────────────
            this.tabManual.Text    = "직접 읽기/쓰기";
            this.tabManual.Padding = new System.Windows.Forms.Padding(3);
            this.tabManual.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.dgvManual, this.pnlManualTop });

            this.pnlManualTop.Dock      = System.Windows.Forms.DockStyle.Top;
            this.pnlManualTop.Height    = 78;
            this.pnlManualTop.BackColor = System.Drawing.Color.FromArgb(45, 45, 48);
            this.pnlManualTop.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.lblManualDev, this.cmbManualDev, this.lblManualAddr, this.txtManualAddr,
                this.lblManualCnt, this.txtManualCnt, this.btnManualRead,
                this.lblManualWriteVal, this.txtManualWriteVal, this.btnManualWrite,
                this.lblPptCap, this.cmbPptDev, this.txtPptAddr, this.btnPptCalc,
                this.lblPptResult, this.lblPptLen, this.txtPptLen, this.btnPptRead,
                this.txtPptVal, this.btnPptWrite });

            SetLabel(this.lblManualDev, "Device:", 4, 12, 52);
            // 다른 탭과 동일하게 두 가지 표기 병기: 입력표기 + 실제 전송영역
            this.cmbManualDev.Items.AddRange(new object[] { "%DB (D영역 바이트)", "%DW (D영역 워드)", "%MX (M영역 비트)", "%RW (R영역 트래킹)", "%RB (R영역 바이트)" });
            this.cmbManualDev.SelectedIndex = 0;
            this.cmbManualDev.Location  = new System.Drawing.Point(58, 8);
            this.cmbManualDev.Size      = new System.Drawing.Size(132, 22);
            this.cmbManualDev.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;

            SetLabel(this.lblManualAddr, "시작 주소:", 196, 12, 68);
            SetTextBox(this.txtManualAddr, "0", 268, 8, 80);
            SetLabel(this.lblManualCnt, "워드 수:", 356, 12, 55);
            SetTextBox(this.txtManualCnt, "10", 414, 8, 50);
            SetButton(this.btnManualRead, "Read", 472, 7, 65, System.Drawing.Color.FromArgb(0, 122, 204));
            this.btnManualRead.Click += new System.EventHandler(this.btnManualRead_Click);
            SetLabel(this.lblManualWriteVal, "Write 값:", 548, 12, 62);
            SetTextBox(this.txtManualWriteVal, "0x0000", 614, 8, 80);
            SetButton(this.btnManualWrite, "Write", 702, 7, 65, System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnManualWrite.Click += new System.EventHandler(this.btnManualWrite_Click);

            // ── 2행: PPT(구 표기) 주소 계산 + 읽기/쓰기 ──────────────────
            //   예) M492 = M워드49 비트2 → %MX786 = (49*16)+2, 워드 읽기값 0x0004
            SetLabel(this.lblPptCap, "PPT표기:", 4, 48, 60);
            this.cmbPptDev.Items.AddRange(new object[] { "M", "D", "R" });
            this.cmbPptDev.SelectedIndex = 0;
            this.cmbPptDev.Location  = new System.Drawing.Point(66, 44);
            this.cmbPptDev.Size      = new System.Drawing.Size(44, 22);
            this.cmbPptDev.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            SetTextBox(this.txtPptAddr, "492", 114, 44, 60);
            SetButton(this.btnPptCalc, "계산", 180, 43, 52, System.Drawing.Color.FromArgb(0, 122, 204));
            this.btnPptCalc.Click += new System.EventHandler(this.btnPptCalc_Click);
            SetLabel(this.lblPptResult, "-", 238, 48, 190);
            this.lblPptResult.ForeColor = System.Drawing.Color.Gold;
            SetLabel(this.lblPptLen, "길이:", 430, 48, 36);
            SetTextBox(this.txtPptLen, "1", 468, 44, 40);
            SetButton(this.btnPptRead, "읽기", 514, 43, 52, System.Drawing.Color.FromArgb(0, 122, 204));
            this.btnPptRead.Click += new System.EventHandler(this.btnPptRead_Click);
            SetTextBox(this.txtPptVal, "0x0000", 572, 44, 78);
            SetButton(this.btnPptWrite, "쓰기", 656, 43, 52, System.Drawing.Color.FromArgb(180, 100, 0));
            this.btnPptWrite.Click += new System.EventHandler(this.btnPptWrite_Click);

            this.dgvManual.Dock = System.Windows.Forms.DockStyle.Fill;
            ConfigDgv(this.dgvManual);
            this.colManAddr.Name = "colManAddr"; this.colManAddr.HeaderText = "주소 (전송표기)"; this.colManAddr.Width = 170;
            this.colManRaw.Name  = "colManRaw";  this.colManRaw.HeaderText  = "Hex";    this.colManRaw.Width  = 80;
            this.colManDec.Name  = "colManDec";  this.colManDec.HeaderText  = "Dec";    this.colManDec.Width  = 80;
            this.colManBin.Name  = "colManBin";  this.colManBin.HeaderText  = "Binary(B15..B0)"; this.colManBin.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.dgvManual.Columns.AddRange(this.colManAddr, this.colManRaw, this.colManDec, this.colManBin);

            // ── tabCvWrite ───────────────────────────────────────────────────
            this.tabCvWrite.Text    = "CV 데이터 쓰기";
            this.tabCvWrite.Padding = new System.Windows.Forms.Padding(3);
            this.tabCvWrite.BackColor = System.Drawing.Color.FromArgb(37, 37, 38);
            this.tabCvWrite.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.btnCvWrite11, this.btnCvWrite12, this.btnCvWrite15 });

            this.btnCvWrite11.Location  = new System.Drawing.Point(20, 20);
            this.btnCvWrite11.Size      = new System.Drawing.Size(280, 100);
            this.btnCvWrite11.BackColor = System.Drawing.Color.FromArgb(0, 122, 204);
            this.btnCvWrite11.ForeColor = System.Drawing.Color.White;
            this.btnCvWrite11.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCvWrite11.FlatAppearance.BorderSize = 0;
            this.btnCvWrite11.Font      = new System.Drawing.Font("맑은 고딕", 9f);
            this.btnCvWrite11.Text      = "CV#11 데이터 쓰기\r\n\r\n%DB980 ← 0 (방향: 입고)\r\nR0100(%RB200) ← 2143 (JOB 1234)\r\nR0101(%RB202) ← 0";
            this.btnCvWrite11.Click    += new System.EventHandler(this.btnCvWrite11_Click);

            this.btnCvWrite12.Location  = new System.Drawing.Point(320, 20);
            this.btnCvWrite12.Size      = new System.Drawing.Size(280, 100);
            this.btnCvWrite12.BackColor = System.Drawing.Color.FromArgb(0, 122, 204);
            this.btnCvWrite12.ForeColor = System.Drawing.Color.White;
            this.btnCvWrite12.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCvWrite12.FlatAppearance.BorderSize = 0;
            this.btnCvWrite12.Font      = new System.Drawing.Font("맑은 고딕", 9f);
            this.btnCvWrite12.Text      = "CV#12 데이터 쓰기\r\n\r\n%DB982 ← 0 (방향: 입고)\r\nR0110(%RB220) ← 2143 (JOB 1234)\r\nR0111(%RB222) ← 0";
            this.btnCvWrite12.Click    += new System.EventHandler(this.btnCvWrite12_Click);

            this.btnCvWrite15.Location  = new System.Drawing.Point(620, 20);
            this.btnCvWrite15.Size      = new System.Drawing.Size(280, 100);
            this.btnCvWrite15.BackColor = System.Drawing.Color.FromArgb(0, 122, 204);
            this.btnCvWrite15.ForeColor = System.Drawing.Color.White;
            this.btnCvWrite15.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCvWrite15.FlatAppearance.BorderSize = 0;
            this.btnCvWrite15.Font      = new System.Drawing.Font("맑은 고딕", 9f);
            this.btnCvWrite15.Text      = "CV#15 데이터 쓰기\r\n\r\n%DB988 ← 0 (방향: 입고)\r\nR0140(%RB280) ← 2143 (JOB 1234)\r\nR0141(%RB282) ← 0";
            this.btnCvWrite15.Click    += new System.EventHandler(this.btnCvWrite15_Click);

            // ── splLog (로그 패널 크기 조절 스플리터) ─────────────────────────
            this.splLog.Dock        = System.Windows.Forms.DockStyle.Bottom;
            this.splLog.Height      = 5;
            this.splLog.BackColor   = System.Drawing.Color.FromArgb(63, 63, 70);
            this.splLog.MinExtra    = 200;   // 탭 영역 최소 높이
            this.splLog.MinSize     = 40;    // 로그 영역 최소 높이

            // ── txtLog ───────────────────────────────────────────────────────
            this.txtLog.Dock        = System.Windows.Forms.DockStyle.Bottom;
            this.txtLog.Height      = 90;
            this.txtLog.Multiline   = true;
            this.txtLog.ReadOnly    = true;
            this.txtLog.ScrollBars  = System.Windows.Forms.ScrollBars.Vertical;
            this.txtLog.BackColor   = System.Drawing.Color.FromArgb(30, 30, 30);
            this.txtLog.ForeColor   = System.Drawing.Color.LightGreen;
            this.txtLog.Font        = new System.Drawing.Font("Consolas", 9f);

            // ── Form ─────────────────────────────────────────────────────────
            this.Controls.AddRange(new System.Windows.Forms.Control[] {
                this.tabMain, this.splLog, this.txtLog, this.pnlConn });
            this.Text            = "LS XGT PLC 메모리 맵 읽기/쓰기 [LG생명과학]";
            this.Size            = new System.Drawing.Size(1000, 680);
            this.MinimumSize     = new System.Drawing.Size(800, 500);
            this.StartPosition   = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.BackColor       = System.Drawing.Color.FromArgb(37, 37, 38);
            this.ForeColor       = System.Drawing.Color.WhiteSmoke;
            this.Font            = new System.Drawing.Font("맑은 고딕", 9f);

            this.pnlConn.ResumeLayout(false);
            this.tabMain.ResumeLayout(false);
            this.tabBit.ResumeLayout(false);
            this.tabWord.ResumeLayout(false);
            this.tabTracking.ResumeLayout(false);
            this.tabManual.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvBit)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvWord)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvTracking)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvManual)).EndInit();
            this.ResumeLayout(false);
        }

        // ── 헬퍼 ─────────────────────────────────────────────────────────────
        private static void SetLabel(System.Windows.Forms.Label lbl, string text, int x, int y, int w)
        {
            lbl.Text      = text;
            lbl.Location  = new System.Drawing.Point(x, y);
            lbl.Size      = new System.Drawing.Size(w, 18);
            lbl.ForeColor = System.Drawing.Color.WhiteSmoke;
            lbl.AutoSize  = false;
        }
        private static void SetTextBox(System.Windows.Forms.TextBox txt, string def, int x, int y, int w)
        {
            txt.Text      = def;
            txt.Location  = new System.Drawing.Point(x, y);
            txt.Size      = new System.Drawing.Size(w, 22);
        }
        private static void SetButton(System.Windows.Forms.Button btn, string text, int x, int y, int w, System.Drawing.Color bc)
        {
            btn.Text      = text;
            btn.Location  = new System.Drawing.Point(x, y);
            btn.Size      = new System.Drawing.Size(w, 26);
            btn.BackColor = bc;
            btn.ForeColor = System.Drawing.Color.White;
            btn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            btn.FlatAppearance.BorderSize = 0;
        }
        private static void ConfigDgv(System.Windows.Forms.DataGridView dgv)
        {
            dgv.ReadOnly               = true;
            dgv.SelectionMode          = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            dgv.MultiSelect            = false;
            dgv.AllowUserToAddRows     = false;
            dgv.AllowUserToDeleteRows  = false;
            dgv.RowHeadersWidth        = 30;
            dgv.BackgroundColor        = System.Drawing.Color.FromArgb(37, 37, 38);
            dgv.ForeColor              = System.Drawing.Color.WhiteSmoke;
            dgv.GridColor              = System.Drawing.Color.FromArgb(80, 80, 80);
            dgv.DefaultCellStyle.BackColor       = System.Drawing.Color.FromArgb(45, 45, 48);
            dgv.DefaultCellStyle.ForeColor       = System.Drawing.Color.WhiteSmoke;
            dgv.ColumnHeadersDefaultCellStyle.BackColor = System.Drawing.Color.FromArgb(63, 63, 70);
            dgv.ColumnHeadersDefaultCellStyle.ForeColor = System.Drawing.Color.WhiteSmoke;
            dgv.ColumnHeadersDefaultCellStyle.Font = new System.Drawing.Font("맑은 고딕", 9f, System.Drawing.FontStyle.Bold);
            dgv.AlternatingRowsDefaultCellStyle.BackColor = System.Drawing.Color.FromArgb(55, 55, 58);
            dgv.EnableHeadersVisualStyles = false;
            dgv.AutoSizeRowsMode  = System.Windows.Forms.DataGridViewAutoSizeRowsMode.None;
            dgv.RowTemplate.Height = 22;
        }
        #endregion

        // Controls
        private System.Windows.Forms.Panel  pnlConn;
        private System.Windows.Forms.Label  lblIp;
        private System.Windows.Forms.TextBox txtIp;
        private System.Windows.Forms.Label  lblPort;
        private System.Windows.Forms.TextBox txtPort;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.Label    lblConnStatus;
        private System.Windows.Forms.CheckBox chkProtoLog;
        private System.Windows.Forms.TabControl tabMain;
        private System.Windows.Forms.TabPage tabCvWrite;
        private System.Windows.Forms.Button btnCvWrite11;
        private System.Windows.Forms.Button btnCvWrite12;
        private System.Windows.Forms.Button btnCvWrite15;
        private System.Windows.Forms.TabPage tabBit;
        private System.Windows.Forms.DataGridView dgvBit;
        private System.Windows.Forms.Panel  pnlBitBtn;
        private System.Windows.Forms.Button btnReadBit;
        private System.Windows.Forms.Label  lblWriteBitVal;
        private System.Windows.Forms.TextBox txtWriteBitVal;
        private System.Windows.Forms.Button btnWriteBit;
        private System.Windows.Forms.TabPage tabWord;
        private System.Windows.Forms.DataGridView dgvWord;
        private System.Windows.Forms.Panel  pnlWordBtn;
        private System.Windows.Forms.Button btnReadWord;
        private System.Windows.Forms.Label  lblWriteWordVal;
        private System.Windows.Forms.TextBox txtWriteWordVal;
        private System.Windows.Forms.Button btnWriteWord;
        private System.Windows.Forms.TabPage tabTracking;
        private System.Windows.Forms.DataGridView dgvTracking;
        private System.Windows.Forms.Panel  pnlTrackBtn;
        private System.Windows.Forms.Button btnReadTracking;
        private System.Windows.Forms.Label  lblWriteTrackVal;
        private System.Windows.Forms.TextBox txtWriteTrackVal;
        private System.Windows.Forms.Button btnWriteTracking;
        private System.Windows.Forms.TabPage tabManual;
        private System.Windows.Forms.Panel  pnlManualTop;
        private System.Windows.Forms.Label  lblManualDev;
        private System.Windows.Forms.ComboBox cmbManualDev;
        private System.Windows.Forms.Label  lblManualAddr;
        private System.Windows.Forms.TextBox txtManualAddr;
        private System.Windows.Forms.Label  lblManualCnt;
        private System.Windows.Forms.TextBox txtManualCnt;
        private System.Windows.Forms.Button btnManualRead;
        private System.Windows.Forms.Label  lblManualWriteVal;
        private System.Windows.Forms.TextBox txtManualWriteVal;
        private System.Windows.Forms.Button btnManualWrite;
        private System.Windows.Forms.Label   lblPptCap;
        private System.Windows.Forms.ComboBox cmbPptDev;
        private System.Windows.Forms.TextBox txtPptAddr;
        private System.Windows.Forms.Button  btnPptCalc;
        private System.Windows.Forms.Label   lblPptResult;
        private System.Windows.Forms.Label   lblPptLen;
        private System.Windows.Forms.TextBox txtPptLen;
        private System.Windows.Forms.Button  btnPptRead;
        private System.Windows.Forms.TextBox txtPptVal;
        private System.Windows.Forms.Button  btnPptWrite;
        private System.Windows.Forms.DataGridView dgvManual;
        private System.Windows.Forms.TextBox txtLog;
        private System.Windows.Forms.Splitter splLog;
        // DGV columns
        private System.Windows.Forms.DataGridViewTextBoxColumn colBitAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBitDesc;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBitRaw;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBitDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBitBinary;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWordAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWordDesc;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWordRaw;
        private System.Windows.Forms.DataGridViewTextBoxColumn colWordDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTrackAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTrackDesc;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTrackRaw;
        private System.Windows.Forms.DataGridViewTextBoxColumn colTrackDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colManAddr;
        private System.Windows.Forms.DataGridViewTextBoxColumn colManRaw;
        private System.Windows.Forms.DataGridViewTextBoxColumn colManDec;
        private System.Windows.Forms.DataGridViewTextBoxColumn colManBin;
    }
}
