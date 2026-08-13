namespace HECS.Gui.Monitor.Panels
{
    partial class MonitorAGVMainPanel
    {
        /// <summary> 
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.label2 = new System.Windows.Forms.Label();
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.agvStatePanel3 = new HECS.Gui.Monitor.Panels.Monitoring.AGVStatePanel();
            this.label3 = new System.Windows.Forms.Label();
            this.agvStatePanel2 = new HECS.Gui.Monitor.Panels.Monitoring.AGVStatePanel();
            this.label1 = new System.Windows.Forms.Label();
            this.agvStatePanel1 = new HECS.Gui.Monitor.Panels.Monitoring.AGVStatePanel();
            this.agvDetailPanel1 = new HECS.Gui.Monitor.Panels.Monitoring.AGVDetailPanel();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.RoyalBlue;
            this.label2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label2.Location = new System.Drawing.Point(0, 615);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(889, 4);
            this.label2.TabIndex = 5;
            // 
            // splitContainer
            // 
            this.splitContainer.BackColor = System.Drawing.Color.RoyalBlue;
            this.splitContainer.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer.Location = new System.Drawing.Point(0, 0);
            this.splitContainer.Name = "splitContainer";
            this.splitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.AutoScroll = true;
            this.splitContainer.Panel1.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer.Panel1.Controls.Add(this.label5);
            this.splitContainer.Panel1.Controls.Add(this.label4);
            this.splitContainer.Panel1.Controls.Add(this.agvStatePanel3);
            this.splitContainer.Panel1.Controls.Add(this.label3);
            this.splitContainer.Panel1.Controls.Add(this.agvStatePanel2);
            this.splitContainer.Panel1.Controls.Add(this.label1);
            this.splitContainer.Panel1.Controls.Add(this.agvStatePanel1);
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer.Panel2.Controls.Add(this.agvDetailPanel1);
            this.splitContainer.Size = new System.Drawing.Size(889, 619);
            this.splitContainer.SplitterDistance = 368;
            this.splitContainer.SplitterWidth = 3;
            this.splitContainer.TabIndex = 4;
            // 
            // label5
            // 
            this.label5.BackColor = System.Drawing.Color.Black;
            this.label5.Dock = System.Windows.Forms.DockStyle.Top;
            this.label5.Location = new System.Drawing.Point(0, 318);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(885, 1);
            this.label5.TabIndex = 12;
            // 
            // label4
            // 
            this.label4.BackColor = System.Drawing.Color.Black;
            this.label4.Dock = System.Windows.Forms.DockStyle.Top;
            this.label4.Location = new System.Drawing.Point(0, 317);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(885, 1);
            this.label4.TabIndex = 10;
            // 
            // agvStatePanel3
            // 
            this.agvStatePanel3.Dock = System.Windows.Forms.DockStyle.Top;
            this.agvStatePanel3.Location = new System.Drawing.Point(0, 212);
            this.agvStatePanel3.Name = "agvStatePanel3";
            this.agvStatePanel3.Size = new System.Drawing.Size(885, 105);
            this.agvStatePanel3.TabIndex = 9;
            // 
            // label3
            // 
            this.label3.BackColor = System.Drawing.Color.Black;
            this.label3.Dock = System.Windows.Forms.DockStyle.Top;
            this.label3.Location = new System.Drawing.Point(0, 211);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(885, 1);
            this.label3.TabIndex = 8;
            // 
            // agvStatePanel2
            // 
            this.agvStatePanel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.agvStatePanel2.Location = new System.Drawing.Point(0, 106);
            this.agvStatePanel2.Name = "agvStatePanel2";
            this.agvStatePanel2.Size = new System.Drawing.Size(885, 105);
            this.agvStatePanel2.TabIndex = 7;
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.Black;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(0, 105);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(885, 1);
            this.label1.TabIndex = 6;
            // 
            // agvStatePanel1
            // 
            this.agvStatePanel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.agvStatePanel1.Location = new System.Drawing.Point(0, 0);
            this.agvStatePanel1.Name = "agvStatePanel1";
            this.agvStatePanel1.Size = new System.Drawing.Size(885, 105);
            this.agvStatePanel1.TabIndex = 0;
            // 
            // agvDetailPanel1
            // 
            this.agvDetailPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.agvDetailPanel1.Location = new System.Drawing.Point(0, 0);
            this.agvDetailPanel1.Name = "agvDetailPanel1";
            this.agvDetailPanel1.Size = new System.Drawing.Size(885, 244);
            this.agvDetailPanel1.TabIndex = 0;
            // 
            // MonitorAGVMainPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.Controls.Add(this.label2);
            this.Controls.Add(this.splitContainer);
            this.Name = "MonitorAGVMainPanel";
            this.Size = new System.Drawing.Size(889, 619);
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel2.ResumeLayout(false);
            this.splitContainer.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.SplitContainer splitContainer;
        private HECS.Gui.Monitor.Panels.Monitoring.AGVStatePanel agvStatePanel1;
        private HECS.Gui.Monitor.Panels.Monitoring.AGVDetailPanel agvDetailPanel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label4;
        private HECS.Gui.Monitor.Panels.Monitoring.AGVStatePanel agvStatePanel3;
        private System.Windows.Forms.Label label3;
        private HECS.Gui.Monitor.Panels.Monitoring.AGVStatePanel agvStatePanel2;
        private System.Windows.Forms.Label label5;
    }
}
