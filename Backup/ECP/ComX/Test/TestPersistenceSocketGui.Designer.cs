namespace ECP.ComX.Test
{
    partial class TestPersistentSocketGui
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

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.groupBoxSendMessage = new System.Windows.Forms.GroupBox();
            this.buttonSendOneShot = new System.Windows.Forms.Button();
            this.checkBoxContinousMessage = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxMessage = new System.Windows.Forms.TextBox();
            this.textBoxInterval = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBoxConnectionInfo = new System.Windows.Forms.GroupBox();
            this.checkBoxConnection = new System.Windows.Forms.CheckBox();
            this.checkBoxISActive = new System.Windows.Forms.CheckBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxHostIP = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.LabelConnection = new System.Windows.Forms.Label();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.panel3 = new System.Windows.Forms.Panel();
            this.textBoxLog = new System.Windows.Forms.TextBox();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.groupBoxSendMessage.SuspendLayout();
            this.groupBoxConnectionInfo.SuspendLayout();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.groupBoxSendMessage);
            this.splitContainer1.Panel1.Controls.Add(this.groupBoxConnectionInfo);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.panel3);
            this.splitContainer1.Size = new System.Drawing.Size(738, 586);
            this.splitContainer1.SplitterDistance = 216;
            this.splitContainer1.TabIndex = 0;
            // 
            // groupBoxSendMessage
            // 
            this.groupBoxSendMessage.Controls.Add(this.buttonSendOneShot);
            this.groupBoxSendMessage.Controls.Add(this.checkBoxContinousMessage);
            this.groupBoxSendMessage.Controls.Add(this.label2);
            this.groupBoxSendMessage.Controls.Add(this.textBoxMessage);
            this.groupBoxSendMessage.Controls.Add(this.textBoxInterval);
            this.groupBoxSendMessage.Controls.Add(this.label6);
            this.groupBoxSendMessage.Location = new System.Drawing.Point(307, 16);
            this.groupBoxSendMessage.Name = "groupBoxSendMessage";
            this.groupBoxSendMessage.Size = new System.Drawing.Size(419, 177);
            this.groupBoxSendMessage.TabIndex = 13;
            this.groupBoxSendMessage.TabStop = false;
            this.groupBoxSendMessage.Text = "Send Message";
            // 
            // buttonSendOneShot
            // 
            this.buttonSendOneShot.Location = new System.Drawing.Point(12, 20);
            this.buttonSendOneShot.Name = "buttonSendOneShot";
            this.buttonSendOneShot.Size = new System.Drawing.Size(58, 23);
            this.buttonSendOneShot.TabIndex = 15;
            this.buttonSendOneShot.Text = "Send";
            this.buttonSendOneShot.UseVisualStyleBackColor = true;
            this.buttonSendOneShot.Click += new System.EventHandler(this.buttonSendOneShot_Click);
            // 
            // checkBoxContinousMessage
            // 
            this.checkBoxContinousMessage.AutoSize = true;
            this.checkBoxContinousMessage.Location = new System.Drawing.Point(159, 153);
            this.checkBoxContinousMessage.Name = "checkBoxContinousMessage";
            this.checkBoxContinousMessage.Size = new System.Drawing.Size(15, 14);
            this.checkBoxContinousMessage.TabIndex = 14;
            this.checkBoxContinousMessage.UseVisualStyleBackColor = true;
            this.checkBoxContinousMessage.CheckedChanged += new System.EventHandler(this.checkBoxContinousMessage_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 154);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(126, 12);
            this.label2.TabIndex = 12;
            this.label2.Text = "Continuous Message";
            // 
            // textBoxMessage
            // 
            this.textBoxMessage.Location = new System.Drawing.Point(12, 49);
            this.textBoxMessage.Multiline = true;
            this.textBoxMessage.Name = "textBoxMessage";
            this.textBoxMessage.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.textBoxMessage.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxMessage.Size = new System.Drawing.Size(401, 91);
            this.textBoxMessage.TabIndex = 16;
            this.textBoxMessage.TextChanged += new System.EventHandler(this.textBoxSleep_TextChanged);
            // 
            // textBoxInterval
            // 
            this.textBoxInterval.Location = new System.Drawing.Point(315, 150);
            this.textBoxInterval.Name = "textBoxInterval";
            this.textBoxInterval.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.textBoxInterval.Size = new System.Drawing.Size(71, 21);
            this.textBoxInterval.TabIndex = 16;
            this.textBoxInterval.Text = "10";
            this.textBoxInterval.TextChanged += new System.EventHandler(this.textBoxSleep_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(198, 154);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(102, 12);
            this.label6.TabIndex = 13;
            this.label6.Text = "Message Interval";
            // 
            // groupBoxConnectionInfo
            // 
            this.groupBoxConnectionInfo.Controls.Add(this.checkBoxConnection);
            this.groupBoxConnectionInfo.Controls.Add(this.checkBoxISActive);
            this.groupBoxConnectionInfo.Controls.Add(this.label4);
            this.groupBoxConnectionInfo.Controls.Add(this.textBoxHostIP);
            this.groupBoxConnectionInfo.Controls.Add(this.label5);
            this.groupBoxConnectionInfo.Controls.Add(this.label3);
            this.groupBoxConnectionInfo.Controls.Add(this.LabelConnection);
            this.groupBoxConnectionInfo.Controls.Add(this.textBoxPort);
            this.groupBoxConnectionInfo.Location = new System.Drawing.Point(12, 12);
            this.groupBoxConnectionInfo.Name = "groupBoxConnectionInfo";
            this.groupBoxConnectionInfo.Size = new System.Drawing.Size(273, 181);
            this.groupBoxConnectionInfo.TabIndex = 12;
            this.groupBoxConnectionInfo.TabStop = false;
            this.groupBoxConnectionInfo.Text = "Connection Info";
            // 
            // checkBoxConnection
            // 
            this.checkBoxConnection.AutoSize = true;
            this.checkBoxConnection.Location = new System.Drawing.Point(110, 132);
            this.checkBoxConnection.Name = "checkBoxConnection";
            this.checkBoxConnection.Size = new System.Drawing.Size(15, 14);
            this.checkBoxConnection.TabIndex = 18;
            this.checkBoxConnection.UseVisualStyleBackColor = true;
            this.checkBoxConnection.CheckedChanged += new System.EventHandler(this.checkBoxConnection_CheckedChanged);
            // 
            // checkBoxISActive
            // 
            this.checkBoxISActive.AutoSize = true;
            this.checkBoxISActive.Location = new System.Drawing.Point(110, 106);
            this.checkBoxISActive.Name = "checkBoxISActive";
            this.checkBoxISActive.Size = new System.Drawing.Size(15, 14);
            this.checkBoxISActive.TabIndex = 19;
            this.checkBoxISActive.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(59, 74);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(27, 12);
            this.label4.TabIndex = 14;
            this.label4.Text = "Port";
            // 
            // textBoxHostIP
            // 
            this.textBoxHostIP.Location = new System.Drawing.Point(110, 42);
            this.textBoxHostIP.Name = "textBoxHostIP";
            this.textBoxHostIP.Size = new System.Drawing.Size(146, 21);
            this.textBoxHostIP.TabIndex = 17;
            this.textBoxHostIP.Text = "127.0.0.1";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(47, 105);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(39, 12);
            this.label5.TabIndex = 11;
            this.label5.Text = "Active";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(41, 43);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(45, 12);
            this.label3.TabIndex = 12;
            this.label3.Text = "Host IP";
            // 
            // LabelConnection
            // 
            this.LabelConnection.AutoSize = true;
            this.LabelConnection.Location = new System.Drawing.Point(17, 132);
            this.LabelConnection.Name = "LabelConnection";
            this.LabelConnection.Size = new System.Drawing.Size(69, 12);
            this.LabelConnection.TabIndex = 10;
            this.LabelConnection.Text = "Connection";
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(110, 74);
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(146, 21);
            this.textBoxPort.TabIndex = 15;
            this.textBoxPort.Text = "6000";
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.textBoxLog);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(0, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(738, 366);
            this.panel3.TabIndex = 0;
            // 
            // textBoxLog
            // 
            this.textBoxLog.AcceptsReturn = true;
            this.textBoxLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxLog.Location = new System.Drawing.Point(0, 0);
            this.textBoxLog.MaxLength = 100;
            this.textBoxLog.Multiline = true;
            this.textBoxLog.Name = "textBoxLog";
            this.textBoxLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxLog.Size = new System.Drawing.Size(738, 366);
            this.textBoxLog.TabIndex = 0;
            // 
            // TestPersistentSocketGui
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(738, 586);
            this.Controls.Add(this.splitContainer1);
            this.Name = "TestPersistentSocketGui";
            this.Text = "TestPersistenceSocket";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.groupBoxSendMessage.ResumeLayout(false);
            this.groupBoxSendMessage.PerformLayout();
            this.groupBoxConnectionInfo.ResumeLayout(false);
            this.groupBoxConnectionInfo.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.TextBox textBoxLog;
        private System.Windows.Forms.GroupBox groupBoxConnectionInfo;
        private System.Windows.Forms.CheckBox checkBoxConnection;
        private System.Windows.Forms.CheckBox checkBoxISActive;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxHostIP;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxInterval;
        private System.Windows.Forms.Label LabelConnection;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.GroupBox groupBoxSendMessage;
        private System.Windows.Forms.Button buttonSendOneShot;
        private System.Windows.Forms.CheckBox checkBoxContinousMessage;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxMessage;

    }
}