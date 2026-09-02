namespace TSK_COMM_IOSCH
{
    partial class frmLogFilter
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.tmrRefresh = new System.Windows.Forms.Timer(this.components);
            this.pnlTop = new System.Windows.Forms.Panel();
            this.lblHead = new System.Windows.Forms.Label();
            this.clbHead = new System.Windows.Forms.CheckedListBox();
            this.btnAll = new System.Windows.Forms.Button();
            this.btnNone = new System.Windows.Forms.Button();
            this.lblStat = new System.Windows.Forms.Label();
            this.cboStat = new System.Windows.Forms.ComboBox();
            this.lblFind = new System.Windows.Forms.Label();
            this.txtFind = new System.Windows.Forms.TextBox();
            this.lblJob = new System.Windows.Forms.Label();
            this.txtJob = new System.Windows.Forms.TextBox();
            this.btnClear = new System.Windows.Forms.Button();
            this.chkAuto = new System.Windows.Forms.CheckBox();
            this.lblCnt = new System.Windows.Forms.Label();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.lsvF = new System.Windows.Forms.ListView();
            this.txtMsg = new System.Windows.Forms.TextBox();
            this.pnlTop.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            //
            // tmrRefresh
            //
            this.tmrRefresh.Interval = 1000;
            this.tmrRefresh.Tick += new System.EventHandler(this.tmrRefresh_Tick);
            //
            // pnlTop
            //
            this.pnlTop.Controls.Add(this.lblHead);
            this.pnlTop.Controls.Add(this.clbHead);
            this.pnlTop.Controls.Add(this.btnAll);
            this.pnlTop.Controls.Add(this.btnNone);
            this.pnlTop.Controls.Add(this.lblStat);
            this.pnlTop.Controls.Add(this.cboStat);
            this.pnlTop.Controls.Add(this.lblFind);
            this.pnlTop.Controls.Add(this.txtFind);
            this.pnlTop.Controls.Add(this.lblJob);
            this.pnlTop.Controls.Add(this.txtJob);
            this.pnlTop.Controls.Add(this.btnClear);
            this.pnlTop.Controls.Add(this.chkAuto);
            this.pnlTop.Controls.Add(this.lblCnt);
            this.pnlTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlTop.Location = new System.Drawing.Point(0, 0);
            this.pnlTop.Name = "pnlTop";
            this.pnlTop.Size = new System.Drawing.Size(1012, 164);
            this.pnlTop.TabIndex = 0;
            //
            // lblHead
            //
            this.lblHead.AutoSize = true;
            this.lblHead.Location = new System.Drawing.Point(8, 8);
            this.lblHead.Name = "lblHead";
            this.lblHead.Text = "헤더 (스레드 / 구분)";
            //
            // clbHead
            //
            this.clbHead.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.clbHead.CheckOnClick = true;
            this.clbHead.MultiColumn = true;
            this.clbHead.ColumnWidth = 200;
            this.clbHead.Location = new System.Drawing.Point(8, 26);
            this.clbHead.Name = "clbHead";
            this.clbHead.Size = new System.Drawing.Size(700, 130);
            this.clbHead.TabIndex = 1;
            this.clbHead.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.clbHead_ItemCheck);
            //
            // btnAll
            //
            this.btnAll.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnAll.Location = new System.Drawing.Point(716, 26);
            this.btnAll.Name = "btnAll";
            this.btnAll.Size = new System.Drawing.Size(140, 23);
            this.btnAll.TabIndex = 2;
            this.btnAll.Text = "전체 선택";
            this.btnAll.UseVisualStyleBackColor = true;
            this.btnAll.Click += new System.EventHandler(this.btnAll_Click);
            //
            // btnNone
            //
            this.btnNone.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnNone.Location = new System.Drawing.Point(862, 26);
            this.btnNone.Name = "btnNone";
            this.btnNone.Size = new System.Drawing.Size(140, 23);
            this.btnNone.TabIndex = 3;
            this.btnNone.Text = "전체 해제";
            this.btnNone.UseVisualStyleBackColor = true;
            this.btnNone.Click += new System.EventHandler(this.btnNone_Click);
            //
            // lblStat
            //
            this.lblStat.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblStat.AutoSize = true;
            this.lblStat.Location = new System.Drawing.Point(716, 58);
            this.lblStat.Name = "lblStat";
            this.lblStat.Text = "작업상태";
            //
            // cboStat
            //
            this.cboStat.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cboStat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboStat.DropDownWidth = 260;
            this.cboStat.Location = new System.Drawing.Point(790, 54);
            this.cboStat.Name = "cboStat";
            this.cboStat.Size = new System.Drawing.Size(212, 20);
            this.cboStat.TabIndex = 4;
            this.cboStat.SelectedIndexChanged += new System.EventHandler(this.cboStat_SelectedIndexChanged);
            //
            // lblFind
            //
            this.lblFind.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblFind.AutoSize = true;
            this.lblFind.Location = new System.Drawing.Point(716, 86);
            this.lblFind.Name = "lblFind";
            this.lblFind.Text = "메시지 검색";
            //
            // txtFind
            //
            this.txtFind.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.txtFind.Location = new System.Drawing.Point(790, 82);
            this.txtFind.Name = "txtFind";
            this.txtFind.Size = new System.Drawing.Size(212, 21);
            this.txtFind.TabIndex = 5;
            this.txtFind.TextChanged += new System.EventHandler(this.txtFind_TextChanged);
            //
            // lblJob
            //
            this.lblJob.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblJob.AutoSize = true;
            this.lblJob.Location = new System.Drawing.Point(716, 114);
            this.lblJob.Name = "lblJob";
            this.lblJob.Text = "작업번호 검색";
            //
            // txtJob
            //
            this.txtJob.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.txtJob.Location = new System.Drawing.Point(790, 110);
            this.txtJob.Name = "txtJob";
            this.txtJob.Size = new System.Drawing.Size(212, 21);
            this.txtJob.TabIndex = 6;
            this.txtJob.TextChanged += new System.EventHandler(this.txtFind_TextChanged);
            //
            // btnClear
            //
            this.btnClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnClear.Location = new System.Drawing.Point(862, 136);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(140, 23);
            this.btnClear.TabIndex = 8;
            this.btnClear.Text = "조건 지우기";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            //
            // chkAuto
            //
            this.chkAuto.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.chkAuto.AutoSize = true;
            this.chkAuto.Checked = true;
            this.chkAuto.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkAuto.Location = new System.Drawing.Point(716, 140);
            this.chkAuto.Name = "chkAuto";
            this.chkAuto.TabIndex = 7;
            this.chkAuto.Text = "최신 자동 스크롤";
            this.chkAuto.UseVisualStyleBackColor = true;
            //
            // lblCnt
            //
            this.lblCnt.AutoSize = true;
            this.lblCnt.Location = new System.Drawing.Point(160, 8);
            this.lblCnt.Name = "lblCnt";
            this.lblCnt.Text = "0 / 0";
            //
            // splitContainer1
            //
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 164);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            this.splitContainer1.Panel1.Controls.Add(this.lsvF);
            this.splitContainer1.Panel2.Controls.Add(this.txtMsg);
            this.splitContainer1.Size = new System.Drawing.Size(1012, 469);
            this.splitContainer1.SplitterDistance = 366;
            this.splitContainer1.TabIndex = 1;
            //
            // lsvF
            //
            this.lsvF.AllowColumnReorder = true;
            this.lsvF.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lsvF.FullRowSelect = true;
            this.lsvF.GridLines = true;
            this.lsvF.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.lsvF.Location = new System.Drawing.Point(0, 0);
            this.lsvF.MultiSelect = false;
            this.lsvF.Name = "lsvF";
            this.lsvF.TabIndex = 0;
            this.lsvF.UseCompatibleStateImageBehavior = false;
            this.lsvF.View = System.Windows.Forms.View.Details;
            this.lsvF.Click += new System.EventHandler(this.lsvF_Click);
            //
            // txtMsg
            //
            this.txtMsg.BackColor = System.Drawing.SystemColors.InactiveBorder;
            this.txtMsg.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtMsg.Location = new System.Drawing.Point(0, 0);
            this.txtMsg.Multiline = true;
            this.txtMsg.Name = "txtMsg";
            this.txtMsg.ReadOnly = true;
            this.txtMsg.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtMsg.TabIndex = 0;
            //
            // frmLogFilter
            //
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(1012, 633);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.pnlTop);
            this.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Name = "frmLogFilter";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "로그 필터";
            this.Load += new System.EventHandler(this.frmLogFilter_Load);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.frmLogFilter_FormClosed);
            this.pnlTop.ResumeLayout(false);
            this.pnlTop.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);
        }

        #endregion

        private System.Windows.Forms.Timer tmrRefresh;
        private System.Windows.Forms.Panel pnlTop;
        private System.Windows.Forms.Label lblHead;
        private System.Windows.Forms.CheckedListBox clbHead;
        private System.Windows.Forms.Button btnAll;
        private System.Windows.Forms.Button btnNone;
        private System.Windows.Forms.Label lblStat;
        private System.Windows.Forms.ComboBox cboStat;
        private System.Windows.Forms.Label lblFind;
        private System.Windows.Forms.TextBox txtFind;
        private System.Windows.Forms.Label lblJob;
        private System.Windows.Forms.TextBox txtJob;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.CheckBox chkAuto;
        private System.Windows.Forms.Label lblCnt;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ListView lsvF;
        private System.Windows.Forms.TextBox txtMsg;
    }
}
