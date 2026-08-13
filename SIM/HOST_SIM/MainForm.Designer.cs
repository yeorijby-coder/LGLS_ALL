namespace HOST_SIM
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
            this.btnStart = new System.Windows.Forms.Button();
            this.btnStop = new System.Windows.Forms.Button();
            this.lblEcsConn = new System.Windows.Forms.Label();
            this.lblReportConn = new System.Windows.Forms.Label();
            this.lblLogic1 = new System.Windows.Forms.Label();
            this.lblLogic2 = new System.Windows.Forms.Label();
            this.lblLogic3 = new System.Windows.Forms.Label();
            this.chkLogic1 = new System.Windows.Forms.CheckBox();
            this.chkLogic2 = new System.Windows.Forms.CheckBox();
            this.chkLogic3 = new System.Windows.Forms.CheckBox();
            this.chkCv2Test = new System.Windows.Forms.CheckBox();
            this.btnRedirectDouble = new System.Windows.Forms.Button();
            this.btnRedirectEmpty = new System.Windows.Forms.Button();
            this.splitMain = new System.Windows.Forms.SplitContainer();
            this.lvSend = new System.Windows.Forms.ListView();
            this.colSTime = new System.Windows.Forms.ColumnHeader();
            this.colSKind = new System.Windows.Forms.ColumnHeader();
            this.colSText = new System.Windows.Forms.ColumnHeader();
            this.lvRecv = new System.Windows.Forms.ListView();
            this.colRTime = new System.Windows.Forms.ColumnHeader();
            this.colRKind = new System.Windows.Forms.ColumnHeader();
            this.colRText = new System.Windows.Forms.ColumnHeader();
            this.lblSend = new System.Windows.Forms.Label();
            this.lblRecv = new System.Windows.Forms.Label();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.pnlTop.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitMain)).BeginInit();
            this.splitMain.Panel1.SuspendLayout();
            this.splitMain.Panel2.SuspendLayout();
            this.splitMain.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            //
            // pnlTop
            //
            this.pnlTop.Controls.Add(this.btnStart);
            this.pnlTop.Controls.Add(this.btnStop);
            this.pnlTop.Controls.Add(this.lblEcsConn);
            this.pnlTop.Controls.Add(this.lblReportConn);
            this.pnlTop.Controls.Add(this.lblLogic1);
            this.pnlTop.Controls.Add(this.lblLogic2);
            this.pnlTop.Controls.Add(this.lblLogic3);
            this.pnlTop.Controls.Add(this.chkLogic1);
            this.pnlTop.Controls.Add(this.chkLogic2);
            this.pnlTop.Controls.Add(this.chkLogic3);
            this.pnlTop.Controls.Add(this.chkCv2Test);
            this.pnlTop.Controls.Add(this.btnRedirectDouble);
            this.pnlTop.Controls.Add(this.btnRedirectEmpty);
            this.pnlTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlTop.Height = 124;
            this.pnlTop.Padding = new System.Windows.Forms.Padding(8);
            //
            // btnStart
            //
            this.btnStart.Location = new System.Drawing.Point(12, 10);
            this.btnStart.Size = new System.Drawing.Size(120, 40);
            this.btnStart.Text = "시작";
            this.btnStart.Font = new System.Drawing.Font("맑은 고딕", 11F, System.Drawing.FontStyle.Bold);
            this.btnStart.Enabled = false;
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            //
            // btnStop
            //
            this.btnStop.Location = new System.Drawing.Point(140, 10);
            this.btnStop.Size = new System.Drawing.Size(120, 40);
            this.btnStop.Text = "종료";
            this.btnStop.Font = new System.Drawing.Font("맑은 고딕", 11F, System.Drawing.FontStyle.Bold);
            this.btnStop.Enabled = false;
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            //
            // lblEcsConn
            //
            this.lblEcsConn.Location = new System.Drawing.Point(280, 12);
            this.lblEcsConn.Size = new System.Drawing.Size(230, 18);
            this.lblEcsConn.Text = "명령 채널(→ECS): 미접속";
            this.lblEcsConn.ForeColor = System.Drawing.Color.Red;
            //
            // lblReportConn
            //
            this.lblReportConn.Location = new System.Drawing.Point(280, 32);
            this.lblReportConn.Size = new System.Drawing.Size(230, 18);
            this.lblReportConn.Text = "보고 채널(ECS→): 미접속";
            this.lblReportConn.ForeColor = System.Drawing.Color.Red;
            //
            // chkLogic1 / 2 / 3  (로직 실행 선택 — 해제 시 진행 중 사이클의 출고 완료 후 일시정지)
            //
            this.chkLogic1.Location = new System.Drawing.Point(520, 10);
            this.chkLogic1.Size = new System.Drawing.Size(66, 18);
            this.chkLogic1.Text = "로직1";
            this.chkLogic1.Checked = true;
            this.chkLogic1.CheckedChanged += new System.EventHandler(this.chkLogic_CheckedChanged);
            //
            // chkCv2Test — SC1/C/V#2 교착 스트레스 테스트 (선택 시 로직1~3 비활성 후 출고 9건+입고 9건 발행)
            //
            this.chkCv2Test.Location = new System.Drawing.Point(280, 58);
            this.chkCv2Test.Size = new System.Drawing.Size(220, 22);
            this.chkCv2Test.Text = "[CV#2 교착 TEST]";
            this.chkCv2Test.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
            this.chkCv2Test.ForeColor = System.Drawing.Color.DarkRed;
            this.chkCv2Test.CheckedChanged += new System.EventHandler(this.chkCv2Test_CheckedChanged);
            //
            // btnRedirectDouble / btnRedirectEmpty (이중입고/공출고 재지정 처리 — 평소 비활성)
            //
            this.btnRedirectDouble.Location = new System.Drawing.Point(12, 90);
            this.btnRedirectDouble.Size = new System.Drawing.Size(200, 28);
            this.btnRedirectDouble.Text = "[이중입고 재지정 처리]";
            this.btnRedirectDouble.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
            this.btnRedirectDouble.ForeColor = System.Drawing.Color.DarkRed;
            this.btnRedirectDouble.Enabled = false;
            this.btnRedirectDouble.Click += new System.EventHandler(this.btnRedirectDouble_Click);
            this.btnRedirectEmpty.Location = new System.Drawing.Point(220, 90);
            this.btnRedirectEmpty.Size = new System.Drawing.Size(200, 28);
            this.btnRedirectEmpty.Text = "[공출고 재지정 처리]";
            this.btnRedirectEmpty.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
            this.btnRedirectEmpty.ForeColor = System.Drawing.Color.DarkBlue;
            this.btnRedirectEmpty.Enabled = false;
            this.btnRedirectEmpty.Click += new System.EventHandler(this.btnRedirectEmpty_Click);
            this.chkLogic2.Location = new System.Drawing.Point(520, 30);
            this.chkLogic2.Size = new System.Drawing.Size(66, 18);
            this.chkLogic2.Text = "로직2";
            this.chkLogic2.Checked = true;
            this.chkLogic2.CheckedChanged += new System.EventHandler(this.chkLogic_CheckedChanged);
            this.chkLogic3.Location = new System.Drawing.Point(520, 50);
            this.chkLogic3.Size = new System.Drawing.Size(66, 18);
            this.chkLogic3.Text = "로직3";
            this.chkLogic3.Checked = true;
            this.chkLogic3.CheckedChanged += new System.EventHandler(this.chkLogic_CheckedChanged);
            //
            // lblLogic1
            //
            this.lblLogic1.Location = new System.Drawing.Point(590, 12);
            this.lblLogic1.Size = new System.Drawing.Size(390, 18);
            this.lblLogic1.Text = "로직1: -";
            //
            // lblLogic2
            //
            this.lblLogic2.Location = new System.Drawing.Point(590, 32);
            this.lblLogic2.Size = new System.Drawing.Size(390, 18);
            this.lblLogic2.Text = "로직2: -";
            //
            // lblLogic3
            //
            this.lblLogic3.Location = new System.Drawing.Point(590, 52);
            this.lblLogic3.Size = new System.Drawing.Size(390, 18);
            this.lblLogic3.Text = "로직3: -";
            //
            // splitMain
            //
            this.splitMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitMain.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.splitMain.Panel1.Controls.Add(this.lvSend);
            this.splitMain.Panel1.Controls.Add(this.lblSend);
            this.splitMain.Panel2.Controls.Add(this.lvRecv);
            this.splitMain.Panel2.Controls.Add(this.lblRecv);
            this.splitMain.SplitterDistance = 490;
            //
            // lblSend
            //
            this.lblSend.Dock = System.Windows.Forms.DockStyle.Top;
            this.lblSend.Height = 22;
            this.lblSend.Text = " ▶ 송신 전문 (HOST → ECS)";
            this.lblSend.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
            this.lblSend.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            //
            // lvSend
            //
            this.lvSend.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvSend.View = System.Windows.Forms.View.Details;
            this.lvSend.FullRowSelect = true;
            this.lvSend.GridLines = true;
            this.lvSend.HideSelection = false;
            this.lvSend.Font = new System.Drawing.Font("Consolas", 9F);
            this.lvSend.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] { this.colSTime, this.colSKind, this.colSText });
            this.colSTime.Text = "시각"; this.colSTime.Width = 90;
            this.colSKind.Text = "구분"; this.colSKind.Width = 200;
            this.colSText.Text = "전문"; this.colSText.Width = 640;
            //
            // lblRecv
            //
            this.lblRecv.Dock = System.Windows.Forms.DockStyle.Top;
            this.lblRecv.Height = 22;
            this.lblRecv.Text = " ◀ 수신 전문 (ECS → HOST)";
            this.lblRecv.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
            this.lblRecv.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            //
            // lvRecv
            //
            this.lvRecv.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvRecv.View = System.Windows.Forms.View.Details;
            this.lvRecv.FullRowSelect = true;
            this.lvRecv.GridLines = true;
            this.lvRecv.HideSelection = false;
            this.lvRecv.Font = new System.Drawing.Font("Consolas", 9F);
            this.lvRecv.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] { this.colRTime, this.colRKind, this.colRText });
            this.colRTime.Text = "시각"; this.colRTime.Width = 90;
            this.colRKind.Text = "구분"; this.colRKind.Width = 200;
            this.colRText.Text = "전문"; this.colRText.Width = 640;
            //
            // statusStrip
            //
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] { this.lblStatus });
            this.lblStatus.Text = "대기";
            //
            // MainForm
            //
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1000, 620);
            this.Controls.Add(this.splitMain);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.pnlTop);
            this.Text = "HOST_SIM - WMS/IMS Simulator (LGLS ECS)";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.pnlTop.ResumeLayout(false);
            this.splitMain.Panel1.ResumeLayout(false);
            this.splitMain.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitMain)).EndInit();
            this.splitMain.ResumeLayout(false);
            this.statusStrip.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        private System.Windows.Forms.Panel pnlTop;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Label lblEcsConn;
        private System.Windows.Forms.Label lblReportConn;
        private System.Windows.Forms.Label lblLogic1;
        private System.Windows.Forms.Label lblLogic2;
        private System.Windows.Forms.Label lblLogic3;
        private System.Windows.Forms.CheckBox chkLogic1;
        private System.Windows.Forms.CheckBox chkLogic2;
        private System.Windows.Forms.CheckBox chkLogic3;
        private System.Windows.Forms.CheckBox chkCv2Test;
        private System.Windows.Forms.Button btnRedirectDouble;
        private System.Windows.Forms.Button btnRedirectEmpty;
        private System.Windows.Forms.SplitContainer splitMain;
        private System.Windows.Forms.ListView lvSend;
        private System.Windows.Forms.ColumnHeader colSTime;
        private System.Windows.Forms.ColumnHeader colSKind;
        private System.Windows.Forms.ColumnHeader colSText;
        private System.Windows.Forms.ListView lvRecv;
        private System.Windows.Forms.ColumnHeader colRTime;
        private System.Windows.Forms.ColumnHeader colRKind;
        private System.Windows.Forms.ColumnHeader colRText;
        private System.Windows.Forms.Label lblSend;
        private System.Windows.Forms.Label lblRecv;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel lblStatus;
    }
}
