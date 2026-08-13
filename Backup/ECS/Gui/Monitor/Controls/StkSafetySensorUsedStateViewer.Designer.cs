namespace HECS.Gui.Monitor.Controls
{
    partial class StkSafetySensorUsedStateViewer
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
            this.groupSensorMonitor = new HECS.Gui.Jaeit.Panels.GroupBox.GroupBoxChamfer();
            this.panelSensorMonitor = new System.Windows.Forms.Panel();
            this.groupSensorMonitor.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupSensorMonitor
            // 
            this.groupSensorMonitor.BackColor = System.Drawing.Color.Teal;
            this.groupSensorMonitor.BorderWidth = 6;
            this.groupSensorMonitor.ColorScheme = HECS.Gui.Jaeit.Panels.GroupBox.EnmColorScheme.Yellow;
            this.groupSensorMonitor.Controls.Add(this.panelSensorMonitor);
            this.groupSensorMonitor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupSensorMonitor.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupSensorMonitor.FontColor = System.Drawing.Color.FromArgb(((int)(((byte)(57)))), ((int)(((byte)(66)))), ((int)(((byte)(255)))));
            this.groupSensorMonitor.Location = new System.Drawing.Point(0, 0);
            this.groupSensorMonitor.Name = "groupSensorMonitor";
            this.groupSensorMonitor.Padding = new System.Windows.Forms.Padding(10, 6, 10, 10);
            this.groupSensorMonitor.ShadowColor = System.Drawing.Color.FromArgb(((int)(((byte)(142)))), ((int)(((byte)(143)))), ((int)(((byte)(255)))));
            this.groupSensorMonitor.Size = new System.Drawing.Size(425, 269);
            this.groupSensorMonitor.TabIndex = 0;
            this.groupSensorMonitor.TabStop = false;
            this.groupSensorMonitor.Text = "Safety Sensor Monitorring";
            // 
            // panelSensorMonitor
            // 
            this.panelSensorMonitor.BackColor = System.Drawing.SystemColors.Window;
            this.panelSensorMonitor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelSensorMonitor.Location = new System.Drawing.Point(10, 21);
            this.panelSensorMonitor.Margin = new System.Windows.Forms.Padding(10);
            this.panelSensorMonitor.Name = "panelSensorMonitor";
            this.panelSensorMonitor.Size = new System.Drawing.Size(405, 238);
            this.panelSensorMonitor.TabIndex = 0;
            // 
            // StkSafetySensorStateViewer
            // 
            this.Controls.Add(this.groupSensorMonitor);
            this.Name = "StkSafetySensorStateViewer";
            this.Size = new System.Drawing.Size(425, 269);
            this.groupSensorMonitor.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private HECS.Gui.Jaeit.Panels.GroupBox.GroupBoxChamfer groupSensorMonitor;
        private System.Windows.Forms.Panel panelSensorMonitor;



    }
}
