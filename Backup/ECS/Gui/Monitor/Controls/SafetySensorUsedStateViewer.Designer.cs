namespace HECS.Gui.Monitor.Controls
{
    partial class SafetySensorUsedStateViewer
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
            this.labelTitle = new System.Windows.Forms.Label();
            this.panelViewerBack = new System.Windows.Forms.Panel();
            this.panelPoint4 = new System.Windows.Forms.Panel();
            this.panelOPSensor = new System.Windows.Forms.Panel();
            this.panelPoint10 = new System.Windows.Forms.Panel();
            this.panelPoint9 = new System.Windows.Forms.Panel();
            this.panelPortSensor = new System.Windows.Forms.Panel();
            this.panelRearRM = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.panelMidSensor = new System.Windows.Forms.Panel();
            this.panelPoint8 = new System.Windows.Forms.Panel();
            this.panelHPSensor = new System.Windows.Forms.Panel();
            this.panelPoint7 = new System.Windows.Forms.Panel();
            this.panelPoint6 = new System.Windows.Forms.Panel();
            this.panelPoint5 = new System.Windows.Forms.Panel();
            this.panelEQPSensor = new System.Windows.Forms.Panel();
            this.panelPoint2 = new System.Windows.Forms.Panel();
            this.panelPoint3 = new System.Windows.Forms.Panel();
            this.panelPoint = new System.Windows.Forms.Panel();
            this.panelHomeRM = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.panelViewerBack.SuspendLayout();
            this.panelOPSensor.SuspendLayout();
            this.panelPortSensor.SuspendLayout();
            this.panelRearRM.SuspendLayout();
            this.panelMidSensor.SuspendLayout();
            this.panelHPSensor.SuspendLayout();
            this.panelEQPSensor.SuspendLayout();
            this.panelHomeRM.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelTitle
            // 
            this.labelTitle.BackColor = System.Drawing.Color.LightSteelBlue;
            this.labelTitle.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.labelTitle.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelTitle.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelTitle.Location = new System.Drawing.Point(0, 0);
            this.labelTitle.Margin = new System.Windows.Forms.Padding(0);
            this.labelTitle.Name = "labelTitle";
            this.labelTitle.Size = new System.Drawing.Size(524, 24);
            this.labelTitle.TabIndex = 0;
            this.labelTitle.Text = "Safety Sensor Used State Viewer";
            this.labelTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panelViewerBack
            // 
            this.panelViewerBack.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelViewerBack.Controls.Add(this.panelEQPSensor);
            this.panelViewerBack.Controls.Add(this.panelPoint4);
            this.panelViewerBack.Controls.Add(this.panelOPSensor);
            this.panelViewerBack.Controls.Add(this.panelPoint10);
            this.panelViewerBack.Controls.Add(this.panelPoint9);
            this.panelViewerBack.Controls.Add(this.panelPortSensor);
            this.panelViewerBack.Controls.Add(this.panelRearRM);
            this.panelViewerBack.Controls.Add(this.panelMidSensor);
            this.panelViewerBack.Controls.Add(this.panelPoint8);
            this.panelViewerBack.Controls.Add(this.panelHPSensor);
            this.panelViewerBack.Controls.Add(this.panelPoint7);
            this.panelViewerBack.Controls.Add(this.panelPoint6);
            this.panelViewerBack.Controls.Add(this.panelPoint5);
            this.panelViewerBack.Controls.Add(this.panelPoint2);
            this.panelViewerBack.Controls.Add(this.panelPoint3);
            this.panelViewerBack.Controls.Add(this.panelPoint);
            this.panelViewerBack.Controls.Add(this.panelHomeRM);
            this.panelViewerBack.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelViewerBack.Location = new System.Drawing.Point(0, 24);
            this.panelViewerBack.Margin = new System.Windows.Forms.Padding(0);
            this.panelViewerBack.Name = "panelViewerBack";
            this.panelViewerBack.Size = new System.Drawing.Size(524, 248);
            this.panelViewerBack.TabIndex = 21;
            // 
            // panelPoint4
            // 
            this.panelPoint4.BackColor = System.Drawing.Color.Silver;
            this.panelPoint4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint4.Location = new System.Drawing.Point(471, 187);
            this.panelPoint4.Name = "panelPoint4";
            this.panelPoint4.Size = new System.Drawing.Size(31, 41);
            this.panelPoint4.TabIndex = 44;
            // 
            // panelOPSensor
            // 
            this.panelOPSensor.BackColor = System.Drawing.Color.Red;
            this.panelOPSensor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelOPSensor.Controls.Add(this.label6);
            this.panelOPSensor.Location = new System.Drawing.Point(417, 113);
            this.panelOPSensor.Name = "panelOPSensor";
            this.panelOPSensor.Size = new System.Drawing.Size(70, 21);
            this.panelOPSensor.TabIndex = 52;
            // 
            // panelPoint10
            // 
            this.panelPoint10.BackColor = System.Drawing.Color.Silver;
            this.panelPoint10.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint10.Location = new System.Drawing.Point(471, 103);
            this.panelPoint10.Name = "panelPoint10";
            this.panelPoint10.Size = new System.Drawing.Size(31, 41);
            this.panelPoint10.TabIndex = 54;
            // 
            // panelPoint9
            // 
            this.panelPoint9.BackColor = System.Drawing.Color.Silver;
            this.panelPoint9.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint9.Location = new System.Drawing.Point(403, 103);
            this.panelPoint9.Name = "panelPoint9";
            this.panelPoint9.Size = new System.Drawing.Size(31, 41);
            this.panelPoint9.TabIndex = 53;
            // 
            // panelPortSensor
            // 
            this.panelPortSensor.BackColor = System.Drawing.Color.Yellow;
            this.panelPortSensor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPortSensor.Controls.Add(this.label1);
            this.panelPortSensor.ForeColor = System.Drawing.SystemColors.ControlText;
            this.panelPortSensor.Location = new System.Drawing.Point(34, 29);
            this.panelPortSensor.Name = "panelPortSensor";
            this.panelPortSensor.Size = new System.Drawing.Size(454, 21);
            this.panelPortSensor.TabIndex = 38;
            // 
            // panelRearRM
            // 
            this.panelRearRM.BackColor = System.Drawing.Color.GreenYellow;
            this.panelRearRM.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelRearRM.Controls.Add(this.label3);
            this.panelRearRM.Location = new System.Drawing.Point(308, 80);
            this.panelRearRM.Name = "panelRearRM";
            this.panelRearRM.Size = new System.Drawing.Size(96, 86);
            this.panelRearRM.TabIndex = 51;
            // 
            // label3
            // 
            this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label3.Font = new System.Drawing.Font("굴림", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label3.Location = new System.Drawing.Point(0, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(94, 84);
            this.label3.TabIndex = 0;
            this.label3.Text = "RM#2";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panelMidSensor
            // 
            this.panelMidSensor.BackColor = System.Drawing.Color.Red;
            this.panelMidSensor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelMidSensor.Controls.Add(this.label5);
            this.panelMidSensor.Location = new System.Drawing.Point(226, 113);
            this.panelMidSensor.Name = "panelMidSensor";
            this.panelMidSensor.Size = new System.Drawing.Size(70, 21);
            this.panelMidSensor.TabIndex = 46;
            // 
            // panelPoint8
            // 
            this.panelPoint8.BackColor = System.Drawing.Color.Silver;
            this.panelPoint8.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint8.Location = new System.Drawing.Point(280, 103);
            this.panelPoint8.Name = "panelPoint8";
            this.panelPoint8.Size = new System.Drawing.Size(31, 41);
            this.panelPoint8.TabIndex = 49;
            // 
            // panelHPSensor
            // 
            this.panelHPSensor.BackColor = System.Drawing.Color.Red;
            this.panelHPSensor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelHPSensor.Controls.Add(this.label4);
            this.panelHPSensor.Location = new System.Drawing.Point(34, 113);
            this.panelHPSensor.Name = "panelHPSensor";
            this.panelHPSensor.Size = new System.Drawing.Size(70, 21);
            this.panelHPSensor.TabIndex = 45;
            // 
            // panelPoint7
            // 
            this.panelPoint7.BackColor = System.Drawing.Color.Silver;
            this.panelPoint7.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint7.Location = new System.Drawing.Point(212, 103);
            this.panelPoint7.Name = "panelPoint7";
            this.panelPoint7.Size = new System.Drawing.Size(31, 41);
            this.panelPoint7.TabIndex = 48;
            // 
            // panelPoint6
            // 
            this.panelPoint6.BackColor = System.Drawing.Color.Silver;
            this.panelPoint6.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint6.Location = new System.Drawing.Point(87, 103);
            this.panelPoint6.Name = "panelPoint6";
            this.panelPoint6.Size = new System.Drawing.Size(31, 41);
            this.panelPoint6.TabIndex = 50;
            // 
            // panelPoint5
            // 
            this.panelPoint5.BackColor = System.Drawing.Color.Silver;
            this.panelPoint5.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint5.Location = new System.Drawing.Point(20, 103);
            this.panelPoint5.Name = "panelPoint5";
            this.panelPoint5.Size = new System.Drawing.Size(31, 41);
            this.panelPoint5.TabIndex = 47;
            // 
            // panelEQPSensor
            // 
            this.panelEQPSensor.BackColor = System.Drawing.Color.Lime;
            this.panelEQPSensor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelEQPSensor.Controls.Add(this.label7);
            this.panelEQPSensor.Location = new System.Drawing.Point(34, 197);
            this.panelEQPSensor.Name = "panelEQPSensor";
            this.panelEQPSensor.Size = new System.Drawing.Size(454, 21);
            this.panelEQPSensor.TabIndex = 40;
            // 
            // panelPoint2
            // 
            this.panelPoint2.BackColor = System.Drawing.Color.Silver;
            this.panelPoint2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint2.Location = new System.Drawing.Point(471, 19);
            this.panelPoint2.Name = "panelPoint2";
            this.panelPoint2.Size = new System.Drawing.Size(31, 41);
            this.panelPoint2.TabIndex = 43;
            // 
            // panelPoint3
            // 
            this.panelPoint3.BackColor = System.Drawing.Color.Silver;
            this.panelPoint3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint3.Location = new System.Drawing.Point(20, 187);
            this.panelPoint3.Name = "panelPoint3";
            this.panelPoint3.Size = new System.Drawing.Size(31, 41);
            this.panelPoint3.TabIndex = 42;
            // 
            // panelPoint
            // 
            this.panelPoint.BackColor = System.Drawing.Color.Silver;
            this.panelPoint.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelPoint.Location = new System.Drawing.Point(20, 19);
            this.panelPoint.Name = "panelPoint";
            this.panelPoint.Size = new System.Drawing.Size(31, 41);
            this.panelPoint.TabIndex = 41;
            // 
            // panelHomeRM
            // 
            this.panelHomeRM.BackColor = System.Drawing.Color.GreenYellow;
            this.panelHomeRM.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelHomeRM.Controls.Add(this.label2);
            this.panelHomeRM.Location = new System.Drawing.Point(117, 80);
            this.panelHomeRM.Name = "panelHomeRM";
            this.panelHomeRM.Size = new System.Drawing.Size(96, 86);
            this.panelHomeRM.TabIndex = 39;
            // 
            // label2
            // 
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Font = new System.Drawing.Font("굴림", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label2.Location = new System.Drawing.Point(0, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(94, 84);
            this.label2.TabIndex = 0;
            this.label2.Text = "RM#1";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Font = new System.Drawing.Font("굴림", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label1.Location = new System.Drawing.Point(0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(452, 19);
            this.label1.TabIndex = 1;
            this.label1.Text = "STK PORT LIGHT CURTAIN";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.Font = new System.Drawing.Font("굴림", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label4.Location = new System.Drawing.Point(0, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(68, 19);
            this.label4.TabIndex = 2;
            this.label4.Text = "HP";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label5
            // 
            this.label5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label5.Font = new System.Drawing.Font("굴림", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label5.Location = new System.Drawing.Point(0, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(68, 19);
            this.label5.TabIndex = 2;
            this.label5.Text = "MIDDLE";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label6
            // 
            this.label6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label6.Font = new System.Drawing.Font("굴림", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label6.Location = new System.Drawing.Point(0, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(68, 19);
            this.label6.TabIndex = 2;
            this.label6.Text = "OP";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label7
            // 
            this.label7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label7.Font = new System.Drawing.Font("굴림", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label7.Location = new System.Drawing.Point(0, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(452, 19);
            this.label7.TabIndex = 2;
            this.label7.Text = "INLINE PORT LIGHT CURTAIN";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // SafetySensorUsedStateViewer
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.OldLace;
            this.Controls.Add(this.panelViewerBack);
            this.Controls.Add(this.labelTitle);
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Name = "SafetySensorUsedStateViewer";
            this.Size = new System.Drawing.Size(524, 272);
            this.panelViewerBack.ResumeLayout(false);
            this.panelOPSensor.ResumeLayout(false);
            this.panelPortSensor.ResumeLayout(false);
            this.panelRearRM.ResumeLayout(false);
            this.panelMidSensor.ResumeLayout(false);
            this.panelHPSensor.ResumeLayout(false);
            this.panelEQPSensor.ResumeLayout(false);
            this.panelHomeRM.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label labelTitle;
        private System.Windows.Forms.Panel panelViewerBack;
        private System.Windows.Forms.Panel panelPoint4;
        private System.Windows.Forms.Panel panelOPSensor;
        private System.Windows.Forms.Panel panelPoint10;
        private System.Windows.Forms.Panel panelPoint9;
        private System.Windows.Forms.Panel panelPortSensor;
        private System.Windows.Forms.Panel panelRearRM;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panelMidSensor;
        private System.Windows.Forms.Panel panelPoint8;
        private System.Windows.Forms.Panel panelHPSensor;
        private System.Windows.Forms.Panel panelPoint7;
        private System.Windows.Forms.Panel panelPoint6;
        private System.Windows.Forms.Panel panelPoint5;
        private System.Windows.Forms.Panel panelEQPSensor;
        private System.Windows.Forms.Panel panelPoint2;
        private System.Windows.Forms.Panel panelPoint3;
        private System.Windows.Forms.Panel panelPoint;
        private System.Windows.Forms.Panel panelHomeRM;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
    }
}
