namespace EQP_SIM
{
    partial class MainForm
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null)) components.Dispose();
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.pnlTop = new System.Windows.Forms.Panel();
            this.lblServer = new System.Windows.Forms.Label();
            this.chkAutoFeed = new System.Windows.Forms.CheckBox();
            this.chkInjectDouble = new System.Windows.Forms.CheckBox();
            this.chkInjectEmpty = new System.Windows.Forms.CheckBox();
            this.btnClearErr = new System.Windows.Forms.Button();
            this.btnFeed11 = new System.Windows.Forms.Button();
            this.btnFeed12 = new System.Windows.Forms.Button();
            this.btnFeed15 = new System.Windows.Forms.Button();
            this.btnReset = new System.Windows.Forms.Button();
            this.txtClearTrack = new System.Windows.Forms.TextBox();
            this.btnClearPallet = new System.Windows.Forms.Button();
            this.btnScenarioTest = new System.Windows.Forms.Button();
            this.splitMain = new System.Windows.Forms.SplitContainer();
            this.lvDevices = new System.Windows.Forms.ListView();
            this.colDev = new System.Windows.Forms.ColumnHeader();
            this.colState = new System.Windows.Forms.ColumnHeader();
            this.lvLog = new System.Windows.Forms.ListView();
            this.colTime = new System.Windows.Forms.ColumnHeader();
            this.colMsg = new System.Windows.Forms.ColumnHeader();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.timerUi = new System.Windows.Forms.Timer(this.components = new System.ComponentModel.Container());
            this.pnlTop.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitMain)).BeginInit();
            this.splitMain.Panel1.SuspendLayout();
            this.splitMain.Panel2.SuspendLayout();
            this.splitMain.SuspendLayout();
            this.SuspendLayout();
            //
            // pnlTop
            //
            this.pnlTop.Controls.Add(this.lblServer);
            this.pnlTop.Controls.Add(this.chkAutoFeed);
            this.pnlTop.Controls.Add(this.chkInjectDouble);
            this.pnlTop.Controls.Add(this.chkInjectEmpty);
            this.pnlTop.Controls.Add(this.btnClearErr);
            this.pnlTop.Controls.Add(this.btnFeed11);
            this.pnlTop.Controls.Add(this.btnFeed12);
            this.pnlTop.Controls.Add(this.btnFeed15);
            this.pnlTop.Controls.Add(this.btnReset);
            this.pnlTop.Controls.Add(this.txtClearTrack);
            this.pnlTop.Controls.Add(this.btnClearPallet);
            this.pnlTop.Controls.Add(this.btnScenarioTest);
            this.pnlTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlTop.Height = 86;
            //
            // lblServer
            //
            this.lblServer.Location = new System.Drawing.Point(12, 8);
            this.lblServer.Size = new System.Drawing.Size(280, 18);
            this.lblServer.Text = "XGT 서버: -";
            //
            // chkAutoFeed
            //
            this.chkAutoFeed.Location = new System.Drawing.Point(12, 28);
            this.chkAutoFeed.Size = new System.Drawing.Size(200, 22);
            this.chkAutoFeed.Text = "입고대 자동 투입 (작업자 자동화)";
            this.chkAutoFeed.CheckedChanged += new System.EventHandler(this.chkAutoFeed_CheckedChanged);
            //
            // chkInjectDouble / chkInjectEmpty (이중입고/공출고 에러 주입)
            //
            this.chkInjectDouble.Location = new System.Drawing.Point(12, 56);
            this.chkInjectDouble.Size = new System.Drawing.Size(190, 22);
            this.chkInjectDouble.Text = "이중입고 에러 (다음 입고 크레인)";
            this.chkInjectDouble.ForeColor = System.Drawing.Color.DarkRed;
            this.chkInjectDouble.CheckedChanged += new System.EventHandler(this.chkInjectDouble_CheckedChanged);
            this.chkInjectEmpty.Location = new System.Drawing.Point(210, 56);
            this.chkInjectEmpty.Size = new System.Drawing.Size(190, 22);
            this.chkInjectEmpty.Text = "공출고 에러 (다음 출고 크레인)";
            this.chkInjectEmpty.ForeColor = System.Drawing.Color.DarkBlue;
            this.chkInjectEmpty.CheckedChanged += new System.EventHandler(this.chkInjectEmpty_CheckedChanged);
            // [LGLS 2026-09-05] [설비 에러 해제] - 현장 조작반에서 사람이 에러를 푸는 동작.
            //   공출고(58)는 재지정이 없어 작업을 삭제하는데, 그러면 스케줄러가 에러난 크레인에
            //   새 지시를 주지 않아 에러가 영영 남는다. 그 상황을 현장처럼 풀어 준다.
            this.btnClearErr.Location = new System.Drawing.Point(430, 52);
            this.btnClearErr.Name = "btnClearErr";
            this.btnClearErr.Size = new System.Drawing.Size(120, 23);
            this.btnClearErr.Text = "설비 에러 해제";
            this.btnClearErr.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
            this.btnClearErr.BackColor = System.Drawing.Color.LightYellow;
            this.btnClearErr.UseVisualStyleBackColor = false;
            this.btnClearErr.Click += new System.EventHandler(this.btnClearErr_Click);
            //
            // btnFeed11 / 12 / 15
            //
            this.btnFeed11.Location = new System.Drawing.Point(320, 14);
            this.btnFeed11.Size = new System.Drawing.Size(130, 30);
            this.btnFeed11.Text = "CV#11 파렛트 투입";
            this.btnFeed11.Click += new System.EventHandler(this.btnFeed11_Click);
            this.btnFeed12.Location = new System.Drawing.Point(456, 14);
            this.btnFeed12.Size = new System.Drawing.Size(130, 30);
            this.btnFeed12.Text = "24 입고대 파렛트 투입 (CV#12)";
            this.btnFeed12.Click += new System.EventHandler(this.btnFeed12_Click);
            this.btnFeed15.Location = new System.Drawing.Point(592, 14);
            this.btnFeed15.Size = new System.Drawing.Size(130, 30);
            this.btnFeed15.Text = "CV#15 파렛트 투입";
            this.btnFeed15.Click += new System.EventHandler(this.btnFeed15_Click);
            //
            // btnReset
            //
            this.btnReset.Location = new System.Drawing.Point(750, 14);
            this.btnReset.Size = new System.Drawing.Size(120, 30);
            this.btnReset.Text = "상태 초기화";
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            //
            // txtClearTrack
            // 
            this.txtClearTrack.Location = new System.Drawing.Point(750, 54);
            this.txtClearTrack.Size = new System.Drawing.Size(70, 25);
            // 
            // btnClearPallet
            // 
            this.btnClearPallet.Location = new System.Drawing.Point(828, 52);
            this.btnClearPallet.Size = new System.Drawing.Size(192, 27);
            this.btnClearPallet.Text = "화물 제거 (P번호 31 / 트랙 131)";
            this.btnClearPallet.Click += new System.EventHandler(this.btnClearPallet_Click);
            // 
            // btnScenarioTest
            //
            this.btnScenarioTest.Location = new System.Drawing.Point(880, 14);
            this.btnScenarioTest.Size = new System.Drawing.Size(140, 30);
            this.btnScenarioTest.Text = "[시나리오 테스트]";
            this.btnScenarioTest.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
            this.btnScenarioTest.ForeColor = System.Drawing.Color.DarkRed;
            this.btnScenarioTest.Click += new System.EventHandler(this.btnScenarioTest_Click);
            //
            // splitMain
            //
            this.splitMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitMain.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.splitMain.Panel1.Controls.Add(this.lvDevices);
            this.splitMain.Panel2.Controls.Add(this.lvLog);
            this.splitMain.SplitterDistance = 420;
            //
            // lvDevices
            //
            this.lvDevices.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvDevices.View = System.Windows.Forms.View.Details;
            this.lvDevices.FullRowSelect = true;
            this.lvDevices.GridLines = true;
            this.lvDevices.HideSelection = false;
            this.lvDevices.Font = new System.Drawing.Font("맑은 고딕", 9F);
            this.lvDevices.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] { this.colDev, this.colState });
            this.colDev.Text = "설비"; this.colDev.Width = 110;
            this.colState.Text = "상태"; this.colState.Width = 290;
            //
            // lvLog
            //
            this.lvLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvLog.View = System.Windows.Forms.View.Details;
            this.lvLog.FullRowSelect = true;
            this.lvLog.GridLines = true;
            this.lvLog.HideSelection = false;
            this.lvLog.Font = new System.Drawing.Font("Consolas", 9F);
            this.lvLog.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] { this.colTime, this.colMsg });
            this.colTime.Text = "시각"; this.colTime.Width = 90;
            this.colMsg.Text = "내용"; this.colMsg.Width = 420;
            //
            // statusStrip
            //
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] { this.lblStatus });
            this.lblStatus.Text = "대기";
            //
            // timerUi
            //
            this.timerUi.Interval = 500;
            this.timerUi.Tick += new System.EventHandler(this.timerUi_Tick);
            //
            // MainForm
            //
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1000, 640);
            this.Controls.Add(this.splitMain);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.pnlTop);
            this.Text = "EQP_SIM - XGT PLC Equipment Simulator (LGLS ECS)";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.pnlTop.ResumeLayout(false);
            this.splitMain.Panel1.ResumeLayout(false);
            this.splitMain.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitMain)).EndInit();
            this.splitMain.ResumeLayout(false);
            this.ResumeLayout(false);
        }

        private System.Windows.Forms.Panel pnlTop;
        private System.Windows.Forms.Label lblServer;
        private System.Windows.Forms.CheckBox chkAutoFeed;
        private System.Windows.Forms.CheckBox chkInjectDouble;
        private System.Windows.Forms.CheckBox chkInjectEmpty;
        private System.Windows.Forms.Button btnClearErr;
        private System.Windows.Forms.Button btnFeed11;
        private System.Windows.Forms.Button btnFeed12;
        private System.Windows.Forms.Button btnFeed15;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.TextBox txtClearTrack;
        private System.Windows.Forms.Button btnClearPallet;
        private System.Windows.Forms.Button btnScenarioTest;
        private System.Windows.Forms.SplitContainer splitMain;
        private System.Windows.Forms.ListView lvDevices;
        private System.Windows.Forms.ColumnHeader colDev;
        private System.Windows.Forms.ColumnHeader colState;
        private System.Windows.Forms.ListView lvLog;
        private System.Windows.Forms.ColumnHeader colTime;
        private System.Windows.Forms.ColumnHeader colMsg;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel lblStatus;
        private System.Windows.Forms.Timer timerUi;
    }
}
