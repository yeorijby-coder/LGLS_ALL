namespace HECS.ConveyorSimulator.Controls
{
    partial class AGVSEMControl
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
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.richTextBoxAGVAll = new System.Windows.Forms.RichTextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.agvVehicleControl3 = new HECS.ConveyorSimulator.Controls.AgvVehicleControl();
            this.agvVehicleControl2 = new HECS.ConveyorSimulator.Controls.AgvVehicleControl();
            this.agvVehicleControl1 = new HECS.ConveyorSimulator.Controls.AgvVehicleControl();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("굴림", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label1.Location = new System.Drawing.Point(6, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "Port";
            // 
            // textBoxPort
            // 
            this.textBoxPort.Font = new System.Drawing.Font("굴림", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxPort.Location = new System.Drawing.Point(83, 3);
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(81, 20);
            this.textBoxPort.TabIndex = 1;
            this.textBoxPort.Text = "9900";
            // 
            // richTextBoxAGVAll
            // 
            this.richTextBoxAGVAll.BackColor = System.Drawing.Color.Beige;
            this.richTextBoxAGVAll.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.richTextBoxAGVAll.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.richTextBoxAGVAll.Location = new System.Drawing.Point(3, 510);
            this.richTextBoxAGVAll.MaxLength = 32768;
            this.richTextBoxAGVAll.Name = "richTextBoxAGVAll";
            this.richTextBoxAGVAll.ReadOnly = true;
            this.richTextBoxAGVAll.Size = new System.Drawing.Size(794, 131);
            this.richTextBoxAGVAll.TabIndex = 2;
            this.richTextBoxAGVAll.Text = "";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.DarkKhaki;
            this.panel1.Controls.Add(this.textBoxPort);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Font = new System.Drawing.Font("굴림", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(794, 32);
            this.panel1.TabIndex = 3;
            // 
            // agvVehicleControl3
            // 
            this.agvVehicleControl3.BackColor = System.Drawing.Color.Silver;
            this.agvVehicleControl3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.agvVehicleControl3.Dock = System.Windows.Forms.DockStyle.Top;
            this.agvVehicleControl3.Location = new System.Drawing.Point(3, 349);
            this.agvVehicleControl3.Name = "agvVehicleControl3";
            this.agvVehicleControl3.Padding = new System.Windows.Forms.Padding(2);
            this.agvVehicleControl3.Size = new System.Drawing.Size(794, 157);
            this.agvVehicleControl3.TabIndex = 6;
            this.agvVehicleControl3.VehicleObject = null;
            // 
            // agvVehicleControl2
            // 
            this.agvVehicleControl2.BackColor = System.Drawing.Color.Silver;
            this.agvVehicleControl2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.agvVehicleControl2.Dock = System.Windows.Forms.DockStyle.Top;
            this.agvVehicleControl2.Location = new System.Drawing.Point(3, 192);
            this.agvVehicleControl2.Name = "agvVehicleControl2";
            this.agvVehicleControl2.Padding = new System.Windows.Forms.Padding(2);
            this.agvVehicleControl2.Size = new System.Drawing.Size(794, 157);
            this.agvVehicleControl2.TabIndex = 5;
            this.agvVehicleControl2.VehicleObject = null;
            // 
            // agvVehicleControl1
            // 
            this.agvVehicleControl1.BackColor = System.Drawing.Color.Silver;
            this.agvVehicleControl1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.agvVehicleControl1.Dock = System.Windows.Forms.DockStyle.Top;
            this.agvVehicleControl1.Location = new System.Drawing.Point(3, 35);
            this.agvVehicleControl1.Name = "agvVehicleControl1";
            this.agvVehicleControl1.Padding = new System.Windows.Forms.Padding(2);
            this.agvVehicleControl1.Size = new System.Drawing.Size(794, 157);
            this.agvVehicleControl1.TabIndex = 4;
            this.agvVehicleControl1.VehicleObject = null;
            // 
            // AGVSEMControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.AutoScroll = true;
            this.BackColor = System.Drawing.Color.Silver;
            this.Controls.Add(this.agvVehicleControl3);
            this.Controls.Add(this.agvVehicleControl2);
            this.Controls.Add(this.agvVehicleControl1);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.richTextBoxAGVAll);
            this.Name = "AGVSEMControl";
            this.Padding = new System.Windows.Forms.Padding(3);
            this.Size = new System.Drawing.Size(800, 644);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.RichTextBox richTextBoxAGVAll;
        private System.Windows.Forms.Panel panel1;
        private AgvVehicleControl agvVehicleControl1;
        private AgvVehicleControl agvVehicleControl2;
        private AgvVehicleControl agvVehicleControl3;
    }
}
