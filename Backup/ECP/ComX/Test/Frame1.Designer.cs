namespace ECP.ComX.Test
{
    partial class Frame1
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
            this.groupBoxConfiguration = new System.Windows.Forms.GroupBox();
            this.checkBoxConnect = new System.Windows.Forms.CheckBox();
            this.checkBoxActive = new System.Windows.Forms.CheckBox();
            this.checkBoxAutoDeleteLog = new System.Windows.Forms.CheckBox();
            this.checkBoxLogToStdout = new System.Windows.Forms.CheckBox();
            this.checkBoxHexLogs = new System.Windows.Forms.CheckBox();
            this.checkBoxIndentedLog = new System.Windows.Forms.CheckBox();
            this.checkBoxHSMS = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonStressStop = new System.Windows.Forms.Button();
            this.buttonStressStart = new System.Windows.Forms.Button();
            this.buttonClearTextBoard = new System.Windows.Forms.Button();
            this.buttonThreadDump = new System.Windows.Forms.Button();
            this.buttonSaveSetting = new System.Windows.Forms.Button();
            this.buttonSend = new System.Windows.Forms.Button();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.textBoxHost = new System.Windows.Forms.TextBox();
            this.textBoxDelayTime = new System.Windows.Forms.TextBox();
            this.textBoxSessionId = new System.Windows.Forms.TextBox();
            this.comboBoxSendMessage = new System.Windows.Forms.ComboBox();
            this.comboBoxTool = new System.Windows.Forms.ComboBox();
            this.textBoxLogArea = new System.Windows.Forms.TextBox();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.groupBoxConfiguration.SuspendLayout();
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
            this.splitContainer1.Panel1.Controls.Add(this.groupBoxConfiguration);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.textBoxLogArea);
            this.splitContainer1.Size = new System.Drawing.Size(910, 467);
            this.splitContainer1.SplitterDistance = 147;
            this.splitContainer1.TabIndex = 0;
            // 
            // groupBoxConfiguration
            // 
            this.groupBoxConfiguration.Controls.Add(this.checkBoxConnect);
            this.groupBoxConfiguration.Controls.Add(this.checkBoxActive);
            this.groupBoxConfiguration.Controls.Add(this.checkBoxAutoDeleteLog);
            this.groupBoxConfiguration.Controls.Add(this.checkBoxLogToStdout);
            this.groupBoxConfiguration.Controls.Add(this.checkBoxHexLogs);
            this.groupBoxConfiguration.Controls.Add(this.checkBoxIndentedLog);
            this.groupBoxConfiguration.Controls.Add(this.checkBoxHSMS);
            this.groupBoxConfiguration.Controls.Add(this.label5);
            this.groupBoxConfiguration.Controls.Add(this.label4);
            this.groupBoxConfiguration.Controls.Add(this.label3);
            this.groupBoxConfiguration.Controls.Add(this.label2);
            this.groupBoxConfiguration.Controls.Add(this.label1);
            this.groupBoxConfiguration.Controls.Add(this.buttonStressStop);
            this.groupBoxConfiguration.Controls.Add(this.buttonStressStart);
            this.groupBoxConfiguration.Controls.Add(this.buttonClearTextBoard);
            this.groupBoxConfiguration.Controls.Add(this.buttonThreadDump);
            this.groupBoxConfiguration.Controls.Add(this.buttonSaveSetting);
            this.groupBoxConfiguration.Controls.Add(this.buttonSend);
            this.groupBoxConfiguration.Controls.Add(this.textBoxPort);
            this.groupBoxConfiguration.Controls.Add(this.textBoxHost);
            this.groupBoxConfiguration.Controls.Add(this.textBoxDelayTime);
            this.groupBoxConfiguration.Controls.Add(this.textBoxSessionId);
            this.groupBoxConfiguration.Controls.Add(this.comboBoxSendMessage);
            this.groupBoxConfiguration.Controls.Add(this.comboBoxTool);
            this.groupBoxConfiguration.Location = new System.Drawing.Point(12, 8);
            this.groupBoxConfiguration.Name = "groupBoxConfiguration";
            this.groupBoxConfiguration.Size = new System.Drawing.Size(883, 128);
            this.groupBoxConfiguration.TabIndex = 1;
            this.groupBoxConfiguration.TabStop = false;
            this.groupBoxConfiguration.Text = "Configuration";
            // 
            // checkBoxConnect
            // 
            this.checkBoxConnect.AutoSize = true;
            this.checkBoxConnect.Location = new System.Drawing.Point(385, 76);
            this.checkBoxConnect.Name = "checkBoxConnect";
            this.checkBoxConnect.Size = new System.Drawing.Size(71, 16);
            this.checkBoxConnect.TabIndex = 4;
            this.checkBoxConnect.Text = "Connect";
            this.checkBoxConnect.UseVisualStyleBackColor = true;
            this.checkBoxConnect.CheckStateChanged += new System.EventHandler(this.checkBoxConnect_CheckStateChanged);
            // 
            // checkBoxActive
            // 
            this.checkBoxActive.AutoSize = true;
            this.checkBoxActive.Checked = true;
            this.checkBoxActive.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxActive.Location = new System.Drawing.Point(385, 53);
            this.checkBoxActive.Name = "checkBoxActive";
            this.checkBoxActive.Size = new System.Drawing.Size(58, 16);
            this.checkBoxActive.TabIndex = 4;
            this.checkBoxActive.Text = "Active";
            this.checkBoxActive.UseVisualStyleBackColor = true;
            // 
            // checkBoxAutoDeleteLog
            // 
            this.checkBoxAutoDeleteLog.AutoSize = true;
            this.checkBoxAutoDeleteLog.Location = new System.Drawing.Point(575, 104);
            this.checkBoxAutoDeleteLog.Name = "checkBoxAutoDeleteLog";
            this.checkBoxAutoDeleteLog.Size = new System.Drawing.Size(142, 16);
            this.checkBoxAutoDeleteLog.TabIndex = 4;
            this.checkBoxAutoDeleteLog.Text = "Auto Delete Log Msg";
            this.checkBoxAutoDeleteLog.UseVisualStyleBackColor = true;
            // 
            // checkBoxLogToStdout
            // 
            this.checkBoxLogToStdout.AutoSize = true;
            this.checkBoxLogToStdout.Location = new System.Drawing.Point(575, 86);
            this.checkBoxLogToStdout.Name = "checkBoxLogToStdout";
            this.checkBoxLogToStdout.Size = new System.Drawing.Size(104, 16);
            this.checkBoxLogToStdout.TabIndex = 4;
            this.checkBoxLogToStdout.Text = "Logs to stdout";
            this.checkBoxLogToStdout.UseVisualStyleBackColor = true;
            // 
            // checkBoxHexLogs
            // 
            this.checkBoxHexLogs.AutoSize = true;
            this.checkBoxHexLogs.Location = new System.Drawing.Point(575, 67);
            this.checkBoxHexLogs.Name = "checkBoxHexLogs";
            this.checkBoxHexLogs.Size = new System.Drawing.Size(78, 16);
            this.checkBoxHexLogs.TabIndex = 4;
            this.checkBoxHexLogs.Text = "Hex Logs";
            this.checkBoxHexLogs.UseVisualStyleBackColor = true;
            // 
            // checkBoxIndentedLog
            // 
            this.checkBoxIndentedLog.AutoSize = true;
            this.checkBoxIndentedLog.Checked = true;
            this.checkBoxIndentedLog.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxIndentedLog.Location = new System.Drawing.Point(575, 48);
            this.checkBoxIndentedLog.Name = "checkBoxIndentedLog";
            this.checkBoxIndentedLog.Size = new System.Drawing.Size(104, 16);
            this.checkBoxIndentedLog.TabIndex = 4;
            this.checkBoxIndentedLog.Text = "Indented Logs";
            this.checkBoxIndentedLog.UseVisualStyleBackColor = true;
            // 
            // checkBoxHSMS
            // 
            this.checkBoxHSMS.AutoSize = true;
            this.checkBoxHSMS.Checked = true;
            this.checkBoxHSMS.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxHSMS.Location = new System.Drawing.Point(385, 30);
            this.checkBoxHSMS.Name = "checkBoxHSMS";
            this.checkBoxHSMS.Size = new System.Drawing.Size(59, 16);
            this.checkBoxHSMS.TabIndex = 4;
            this.checkBoxHSMS.Text = "HSMS";
            this.checkBoxHSMS.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(751, 23);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(110, 12);
            this.label5.TabIndex = 3;
            this.label5.Text = "Stress Delay Time";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(181, 81);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(61, 12);
            this.label4.TabIndex = 3;
            this.label4.Text = "SessionId";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(215, 55);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(27, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "Port";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(212, 29);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(30, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Host";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(30, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "Tool";
            // 
            // buttonStressStop
            // 
            this.buttonStressStop.Location = new System.Drawing.Point(753, 97);
            this.buttonStressStop.Name = "buttonStressStop";
            this.buttonStressStop.Size = new System.Drawing.Size(97, 23);
            this.buttonStressStop.TabIndex = 2;
            this.buttonStressStop.Text = "Stress Stop";
            this.buttonStressStop.UseVisualStyleBackColor = true;
            this.buttonStressStop.Click += new System.EventHandler(this.buttonStressStop_Click);
            // 
            // buttonStressStart
            // 
            this.buttonStressStart.Location = new System.Drawing.Point(753, 68);
            this.buttonStressStart.Name = "buttonStressStart";
            this.buttonStressStart.Size = new System.Drawing.Size(97, 23);
            this.buttonStressStart.TabIndex = 2;
            this.buttonStressStart.Text = "Stress Start";
            this.buttonStressStart.UseVisualStyleBackColor = true;
            this.buttonStressStart.Click += new System.EventHandler(this.buttonStressStart_Click);
            // 
            // buttonClearTextBoard
            // 
            this.buttonClearTextBoard.Location = new System.Drawing.Point(466, 99);
            this.buttonClearTextBoard.Name = "buttonClearTextBoard";
            this.buttonClearTextBoard.Size = new System.Drawing.Size(97, 23);
            this.buttonClearTextBoard.TabIndex = 2;
            this.buttonClearTextBoard.Text = "ClearTextBoard";
            this.buttonClearTextBoard.UseVisualStyleBackColor = true;
            this.buttonClearTextBoard.Click += new System.EventHandler(this.buttonClearTextBoard_Click);
            // 
            // buttonThreadDump
            // 
            this.buttonThreadDump.Location = new System.Drawing.Point(466, 73);
            this.buttonThreadDump.Name = "buttonThreadDump";
            this.buttonThreadDump.Size = new System.Drawing.Size(97, 23);
            this.buttonThreadDump.TabIndex = 2;
            this.buttonThreadDump.Text = "Thread Dump";
            this.buttonThreadDump.UseVisualStyleBackColor = true;
            // 
            // buttonSaveSetting
            // 
            this.buttonSaveSetting.Location = new System.Drawing.Point(466, 47);
            this.buttonSaveSetting.Name = "buttonSaveSetting";
            this.buttonSaveSetting.Size = new System.Drawing.Size(97, 23);
            this.buttonSaveSetting.TabIndex = 2;
            this.buttonSaveSetting.Text = "SaveSetting";
            this.buttonSaveSetting.UseVisualStyleBackColor = true;
            // 
            // buttonSend
            // 
            this.buttonSend.Location = new System.Drawing.Point(466, 21);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(97, 23);
            this.buttonSend.TabIndex = 2;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(249, 50);
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(122, 21);
            this.textBoxPort.TabIndex = 1;
            this.textBoxPort.Text = "6000";
            // 
            // textBoxHost
            // 
            this.textBoxHost.Location = new System.Drawing.Point(249, 26);
            this.textBoxHost.Name = "textBoxHost";
            this.textBoxHost.Size = new System.Drawing.Size(122, 21);
            this.textBoxHost.TabIndex = 1;
            this.textBoxHost.Text = "127.0.0.1";
            // 
            // textBoxDelayTime
            // 
            this.textBoxDelayTime.Location = new System.Drawing.Point(753, 41);
            this.textBoxDelayTime.Name = "textBoxDelayTime";
            this.textBoxDelayTime.Size = new System.Drawing.Size(97, 21);
            this.textBoxDelayTime.TabIndex = 1;
            this.textBoxDelayTime.Text = "500";
            this.textBoxDelayTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.textBoxDelayTime.TextChanged += new System.EventHandler(this.textBoxDelayTime_TextChanged);
            // 
            // textBoxSessionId
            // 
            this.textBoxSessionId.Location = new System.Drawing.Point(249, 74);
            this.textBoxSessionId.Name = "textBoxSessionId";
            this.textBoxSessionId.Size = new System.Drawing.Size(122, 21);
            this.textBoxSessionId.TabIndex = 1;
            this.textBoxSessionId.Text = "1";
            // 
            // comboBoxSendMessage
            // 
            this.comboBoxSendMessage.FormattingEnabled = true;
            this.comboBoxSendMessage.Location = new System.Drawing.Point(575, 20);
            this.comboBoxSendMessage.Name = "comboBoxSendMessage";
            this.comboBoxSendMessage.Size = new System.Drawing.Size(146, 20);
            this.comboBoxSendMessage.TabIndex = 0;
            // 
            // comboBoxTool
            // 
            this.comboBoxTool.FormattingEnabled = true;
            this.comboBoxTool.Items.AddRange(new object[] {
            "SecsTest"});
            this.comboBoxTool.Location = new System.Drawing.Point(56, 26);
            this.comboBoxTool.Name = "comboBoxTool";
            this.comboBoxTool.Size = new System.Drawing.Size(142, 20);
            this.comboBoxTool.TabIndex = 0;
            // 
            // textBoxLogArea
            // 
            this.textBoxLogArea.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxLogArea.Location = new System.Drawing.Point(0, 0);
            this.textBoxLogArea.Multiline = true;
            this.textBoxLogArea.Name = "textBoxLogArea";
            this.textBoxLogArea.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxLogArea.Size = new System.Drawing.Size(910, 316);
            this.textBoxLogArea.TabIndex = 0;
            // 
            // Frame1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(910, 467);
            this.Controls.Add(this.splitContainer1);
            this.Name = "Frame1";
            this.Text = "Frame1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Frame1_FormClosing);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            this.splitContainer1.ResumeLayout(false);
            this.groupBoxConfiguration.ResumeLayout(false);
            this.groupBoxConfiguration.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.GroupBox groupBoxConfiguration;
        private System.Windows.Forms.Button buttonThreadDump;
        private System.Windows.Forms.Button buttonSaveSetting;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.TextBox textBoxHost;
        private System.Windows.Forms.TextBox textBoxSessionId;
        private System.Windows.Forms.ComboBox comboBoxSendMessage;
        private System.Windows.Forms.ComboBox comboBoxTool;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox checkBoxConnect;
        private System.Windows.Forms.CheckBox checkBoxActive;
        private System.Windows.Forms.CheckBox checkBoxLogToStdout;
        private System.Windows.Forms.CheckBox checkBoxHexLogs;
        private System.Windows.Forms.CheckBox checkBoxIndentedLog;
        private System.Windows.Forms.CheckBox checkBoxHSMS;
        private System.Windows.Forms.Button buttonClearTextBoard;
        private System.Windows.Forms.Button buttonStressStart;
        private System.Windows.Forms.Button buttonStressStop;
        private System.Windows.Forms.CheckBox checkBoxAutoDeleteLog;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxDelayTime;
        private System.Windows.Forms.TextBox textBoxLogArea;

    }
}