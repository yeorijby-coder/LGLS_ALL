using HECS.Gui.Monitor.Panels.Monitoring;
namespace HECS.Gui.Monitor.Panels
{
    partial class MonitorMainPanel
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
            this.splitContainerMonitor = new System.Windows.Forms.SplitContainer();
            this.monitorAllEquipmentPanel01 = new HECS.Gui.Monitor.Panels.Monitoring.MonitorAllEquipmentPanel01();
            this.monitorMainTransferListPanel1 = new HECS.Gui.Monitor.Panels.Monitoring.MonitorMainTransferListPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.splitContainerMonitor.Panel1.SuspendLayout();
            this.splitContainerMonitor.Panel2.SuspendLayout();
            this.splitContainerMonitor.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainerMonitor
            // 
            this.splitContainerMonitor.BackColor = System.Drawing.Color.RoyalBlue;
            this.splitContainerMonitor.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainerMonitor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerMonitor.Location = new System.Drawing.Point(0, 0);
            this.splitContainerMonitor.Name = "splitContainerMonitor";
            this.splitContainerMonitor.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerMonitor.Panel1
            // 
            this.splitContainerMonitor.Panel1.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainerMonitor.Panel1.Controls.Add(this.monitorAllEquipmentPanel01);
            this.splitContainerMonitor.Panel1MinSize = 400;
            // 
            // splitContainerMonitor.Panel2
            // 
            this.splitContainerMonitor.Panel2.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainerMonitor.Panel2.Controls.Add(this.monitorMainTransferListPanel1);
            this.splitContainerMonitor.Panel2.Controls.Add(this.label1);
            this.splitContainerMonitor.Panel2MinSize = 100;
            this.splitContainerMonitor.Size = new System.Drawing.Size(1240, 771);
            this.splitContainerMonitor.SplitterDistance = 555;
            this.splitContainerMonitor.SplitterWidth = 3;
            this.splitContainerMonitor.TabIndex = 1;
            // 
            // monitorAllEquipmentPanel01
            // 
            this.monitorAllEquipmentPanel01.AutoScroll = true;
            this.monitorAllEquipmentPanel01.BackColor = System.Drawing.Color.LemonChiffon;
            this.monitorAllEquipmentPanel01.Dock = System.Windows.Forms.DockStyle.Fill;
            this.monitorAllEquipmentPanel01.Location = new System.Drawing.Point(0, 0);
            this.monitorAllEquipmentPanel01.Name = "monitorAllEquipmentPanel01";
            this.monitorAllEquipmentPanel01.Size = new System.Drawing.Size(1236, 551);
            this.monitorAllEquipmentPanel01.TabIndex = 0;
            // 
            // monitorMainTransferListPanel1
            // 
            this.monitorMainTransferListPanel1.BackColor = System.Drawing.Color.RoyalBlue;
            this.monitorMainTransferListPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.monitorMainTransferListPanel1.Location = new System.Drawing.Point(0, 0);
            this.monitorMainTransferListPanel1.Name = "monitorMainTransferListPanel1";
            this.monitorMainTransferListPanel1.Size = new System.Drawing.Size(1236, 205);
            this.monitorMainTransferListPanel1.TabIndex = 2;
            this.monitorMainTransferListPanel1.Load += new System.EventHandler(this.monitorMainTransferListPanel1_Load);
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.RoyalBlue;
            this.label1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label1.Location = new System.Drawing.Point(0, 205);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(1236, 4);
            this.label1.TabIndex = 1;
            // 
            // MonitorMainPanel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.splitContainerMonitor);
            this.Name = "MonitorMainPanel";
            this.Size = new System.Drawing.Size(1240, 771);
            this.splitContainerMonitor.Panel1.ResumeLayout(false);
            this.splitContainerMonitor.Panel2.ResumeLayout(false);
            this.splitContainerMonitor.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainerMonitor;
        private System.Windows.Forms.Label label1;
        private MonitorAllEquipmentPanel01 monitorAllEquipmentPanel01;
        private MonitorMainTransferListPanel monitorMainTransferListPanel1;
    }
}
