namespace ALL_TASK
{
    partial class AllTaskMain
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

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.tabMain = new System.Windows.Forms.TabControl();
            this.tabCtrl = new System.Windows.Forms.TabPage();
            this.tabEqp = new System.Windows.Forms.TabPage();
            this.tabIo = new System.Windows.Forms.TabPage();
            this.tabHost = new System.Windows.Forms.TabPage();
            this.pnlCtrl = new System.Windows.Forms.Panel();
            this.grpTask = new System.Windows.Forms.GroupBox();
            this.grpLog = new System.Windows.Forms.GroupBox();
            this.lsvThread = new System.Windows.Forms.ListView();
            this.tmrState = new System.Windows.Forms.Timer(this.components);
            this.staMain = new System.Windows.Forms.StatusStrip();
            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.tabMain.SuspendLayout();
            this.tabCtrl.SuspendLayout();
            this.pnlCtrl.SuspendLayout();
            this.staMain.SuspendLayout();
            this.SuspendLayout();
            //
            // tabMain
            //
            this.tabMain.Controls.Add(this.tabCtrl);
            this.tabMain.Controls.Add(this.tabEqp);
            this.tabMain.Controls.Add(this.tabIo);
            this.tabMain.Controls.Add(this.tabHost);
            this.tabMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabMain.Location = new System.Drawing.Point(0, 0);
            this.tabMain.Name = "tabMain";
            this.tabMain.SelectedIndex = 0;
            this.tabMain.Size = new System.Drawing.Size(1200, 778);
            this.tabMain.TabIndex = 0;
            //
            // tabCtrl
            //
            this.tabCtrl.Controls.Add(this.pnlCtrl);
            this.tabCtrl.Location = new System.Drawing.Point(4, 22);
            this.tabCtrl.Name = "tabCtrl";
            this.tabCtrl.Padding = new System.Windows.Forms.Padding(3);
            this.tabCtrl.Size = new System.Drawing.Size(1192, 752);
            this.tabCtrl.TabIndex = 0;
            this.tabCtrl.Text = "제어판";
            this.tabCtrl.UseVisualStyleBackColor = true;
            //
            // tabEqp
            //
            this.tabEqp.Location = new System.Drawing.Point(4, 22);
            this.tabEqp.Name = "tabEqp";
            this.tabEqp.Size = new System.Drawing.Size(1192, 752);
            this.tabEqp.TabIndex = 1;
            this.tabEqp.Text = "EQP (설비통신)";
            this.tabEqp.UseVisualStyleBackColor = true;
            //
            // tabIo
            //
            this.tabIo.Location = new System.Drawing.Point(4, 22);
            this.tabIo.Name = "tabIo";
            this.tabIo.Size = new System.Drawing.Size(1192, 752);
            this.tabIo.TabIndex = 2;
            this.tabIo.Text = "IO (스케줄러)";
            this.tabIo.UseVisualStyleBackColor = true;
            //
            // tabHost
            //
            this.tabHost.Location = new System.Drawing.Point(4, 22);
            this.tabHost.Name = "tabHost";
            this.tabHost.Size = new System.Drawing.Size(1192, 752);
            this.tabHost.TabIndex = 3;
            this.tabHost.Text = "HOST (상위통신)";
            this.tabHost.UseVisualStyleBackColor = true;
            //
            // pnlCtrl
            //
            this.pnlCtrl.Controls.Add(this.lsvThread);
            this.pnlCtrl.Controls.Add(this.grpLog);
            this.pnlCtrl.Controls.Add(this.grpTask);
            this.pnlCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlCtrl.Location = new System.Drawing.Point(3, 3);
            this.pnlCtrl.Name = "pnlCtrl";
            this.pnlCtrl.Size = new System.Drawing.Size(1186, 746);
            this.pnlCtrl.TabIndex = 0;
            //
            // grpTask
            //
            this.grpTask.Location = new System.Drawing.Point(10, 8);
            this.grpTask.Name = "grpTask";
            this.grpTask.Size = new System.Drawing.Size(560, 150);
            this.grpTask.TabIndex = 0;
            this.grpTask.TabStop = false;
            this.grpTask.Text = "태스크 스레드";
            //
            // grpLog
            //
            this.grpLog.Location = new System.Drawing.Point(580, 8);
            this.grpLog.Name = "grpLog";
            this.grpLog.Size = new System.Drawing.Size(590, 150);
            this.grpLog.TabIndex = 1;
            this.grpLog.TabStop = false;
            this.grpLog.Text = "로그";
            //
            // lsvThread
            //
            this.lsvThread.FullRowSelect = true;
            this.lsvThread.GridLines = true;
            this.lsvThread.HideSelection = false;
            this.lsvThread.Location = new System.Drawing.Point(10, 168);
            this.lsvThread.Name = "lsvThread";
            this.lsvThread.Size = new System.Drawing.Size(1160, 560);
            this.lsvThread.TabIndex = 2;
            this.lsvThread.UseCompatibleStateImageBehavior = false;
            this.lsvThread.View = System.Windows.Forms.View.Details;
            //
            // tmrState
            //
            this.tmrState.Interval = 1000;
            //
            // staMain
            //
            this.staMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblStatus});
            this.staMain.Location = new System.Drawing.Point(0, 778);
            this.staMain.Name = "staMain";
            this.staMain.Size = new System.Drawing.Size(1200, 22);
            this.staMain.TabIndex = 1;
            //
            // lblStatus
            //
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(0, 17);
            //
            // AllTaskMain
            //
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1200, 800);
            this.Controls.Add(this.tabMain);
            this.Controls.Add(this.staMain);
            this.Name = "AllTaskMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ALL_TASK";
            this.tabMain.ResumeLayout(false);
            this.tabCtrl.ResumeLayout(false);
            this.pnlCtrl.ResumeLayout(false);
            this.staMain.ResumeLayout(false);
            this.staMain.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion

        private System.Windows.Forms.TabControl tabMain;
        private System.Windows.Forms.TabPage tabCtrl;
        private System.Windows.Forms.TabPage tabEqp;
        private System.Windows.Forms.TabPage tabIo;
        private System.Windows.Forms.TabPage tabHost;
        private System.Windows.Forms.Panel pnlCtrl;
        private System.Windows.Forms.GroupBox grpTask;
        private System.Windows.Forms.GroupBox grpLog;
        private System.Windows.Forms.ListView lsvThread;
        private System.Windows.Forms.Timer tmrState;
        private System.Windows.Forms.StatusStrip staMain;
        private System.Windows.Forms.ToolStripStatusLabel lblStatus;
    }
}
