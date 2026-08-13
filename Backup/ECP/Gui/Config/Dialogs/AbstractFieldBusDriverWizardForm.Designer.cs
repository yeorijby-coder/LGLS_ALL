namespace ECP.Gui.Config.Dialogs
{
    partial class AbstractFieldBusDriverWizardForm
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonApply = new System.Windows.Forms.Button();
            this.mainPanel = new System.Windows.Forms.Panel();
            this.comboBoxOwnerType = new System.Windows.Forms.ComboBox();
            this.comboBoxDriverType = new System.Windows.Forms.ComboBox();
            this.checkBoxUseFlag = new System.Windows.Forms.CheckBox();
            this.textBoxClassName = new System.Windows.Forms.TextBox();
            this.textBoxAssemblyName = new System.Windows.Forms.TextBox();
            this.textBoxDescription = new System.Windows.Forms.TextBox();
            this.textBoxScanInterval = new System.Windows.Forms.TextBox();
            this.textBoxConnectString = new System.Windows.Forms.TextBox();
            this.textBoxTimeOutSec = new System.Windows.Forms.TextBox();
            this.textBoxOpenRetSec = new System.Windows.Forms.TextBox();
            this.textBoxOwnerId = new System.Windows.Forms.TextBox();
            this.textBoxDriverNo = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.tableLayoutPanel1.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.mainPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.flowLayoutPanel1, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.mainPanel, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 92.29209F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 7.707911F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(446, 438);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.flowLayoutPanel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.flowLayoutPanel1.Controls.Add(this.buttonCancel);
            this.flowLayoutPanel1.Controls.Add(this.buttonOK);
            this.flowLayoutPanel1.Controls.Add(this.buttonApply);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(3, 407);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(440, 28);
            this.flowLayoutPanel1.TabIndex = 0;
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(360, 3);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 0;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(279, 3);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 1;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // buttonApply
            // 
            this.buttonApply.Location = new System.Drawing.Point(198, 3);
            this.buttonApply.Name = "buttonApply";
            this.buttonApply.Size = new System.Drawing.Size(75, 23);
            this.buttonApply.TabIndex = 2;
            this.buttonApply.Text = "Apply";
            this.buttonApply.UseVisualStyleBackColor = true;
            this.buttonApply.Click += new System.EventHandler(this.buttonApply_Click);
            // 
            // mainPanel
            // 
            this.mainPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.mainPanel.Controls.Add(this.comboBoxOwnerType);
            this.mainPanel.Controls.Add(this.comboBoxDriverType);
            this.mainPanel.Controls.Add(this.checkBoxUseFlag);
            this.mainPanel.Controls.Add(this.textBoxClassName);
            this.mainPanel.Controls.Add(this.textBoxAssemblyName);
            this.mainPanel.Controls.Add(this.textBoxDescription);
            this.mainPanel.Controls.Add(this.textBoxScanInterval);
            this.mainPanel.Controls.Add(this.textBoxConnectString);
            this.mainPanel.Controls.Add(this.textBoxTimeOutSec);
            this.mainPanel.Controls.Add(this.textBoxOpenRetSec);
            this.mainPanel.Controls.Add(this.textBoxOwnerId);
            this.mainPanel.Controls.Add(this.textBoxDriverNo);
            this.mainPanel.Controls.Add(this.label12);
            this.mainPanel.Controls.Add(this.label11);
            this.mainPanel.Controls.Add(this.label10);
            this.mainPanel.Controls.Add(this.label9);
            this.mainPanel.Controls.Add(this.label8);
            this.mainPanel.Controls.Add(this.label7);
            this.mainPanel.Controls.Add(this.label6);
            this.mainPanel.Controls.Add(this.label5);
            this.mainPanel.Controls.Add(this.label4);
            this.mainPanel.Controls.Add(this.label3);
            this.mainPanel.Controls.Add(this.label2);
            this.mainPanel.Controls.Add(this.label1);
            this.mainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainPanel.Location = new System.Drawing.Point(3, 3);
            this.mainPanel.Name = "mainPanel";
            this.mainPanel.Size = new System.Drawing.Size(440, 398);
            this.mainPanel.TabIndex = 1;
            // 
            // comboBoxOwnerType
            // 
            this.comboBoxOwnerType.FormattingEnabled = true;
            this.comboBoxOwnerType.Items.AddRange(new object[] {
            "Equipment",
            "Module",
            "Subsystem",
            "IODevice"});
            this.comboBoxOwnerType.Location = new System.Drawing.Point(206, 115);
            this.comboBoxOwnerType.Name = "comboBoxOwnerType";
            this.comboBoxOwnerType.Size = new System.Drawing.Size(121, 20);
            this.comboBoxOwnerType.TabIndex = 4;
            // 
            // comboBoxDriverType
            // 
            this.comboBoxDriverType.FormattingEnabled = true;
            this.comboBoxDriverType.Items.AddRange(new object[] {
            "SIMULATION",
            "MELSECNET",
            "CCLINK",
            "DEVICENET",
            "FILE"});
            this.comboBoxDriverType.Location = new System.Drawing.Point(206, 61);
            this.comboBoxDriverType.Name = "comboBoxDriverType";
            this.comboBoxDriverType.Size = new System.Drawing.Size(121, 20);
            this.comboBoxDriverType.TabIndex = 3;
            // 
            // checkBoxUseFlag
            // 
            this.checkBoxUseFlag.AutoSize = true;
            this.checkBoxUseFlag.Location = new System.Drawing.Point(206, 254);
            this.checkBoxUseFlag.Name = "checkBoxUseFlag";
            this.checkBoxUseFlag.Size = new System.Drawing.Size(15, 14);
            this.checkBoxUseFlag.TabIndex = 2;
            this.checkBoxUseFlag.UseVisualStyleBackColor = true;
            // 
            // textBoxClassName
            // 
            this.textBoxClassName.Location = new System.Drawing.Point(206, 330);
            this.textBoxClassName.Name = "textBoxClassName";
            this.textBoxClassName.Size = new System.Drawing.Size(133, 21);
            this.textBoxClassName.TabIndex = 1;
            // 
            // textBoxAssemblyName
            // 
            this.textBoxAssemblyName.Location = new System.Drawing.Point(206, 303);
            this.textBoxAssemblyName.Name = "textBoxAssemblyName";
            this.textBoxAssemblyName.Size = new System.Drawing.Size(100, 21);
            this.textBoxAssemblyName.TabIndex = 1;
            // 
            // textBoxDescription
            // 
            this.textBoxDescription.Location = new System.Drawing.Point(206, 276);
            this.textBoxDescription.Name = "textBoxDescription";
            this.textBoxDescription.Size = new System.Drawing.Size(133, 21);
            this.textBoxDescription.TabIndex = 1;
            // 
            // textBoxScanInterval
            // 
            this.textBoxScanInterval.Location = new System.Drawing.Point(206, 222);
            this.textBoxScanInterval.Name = "textBoxScanInterval";
            this.textBoxScanInterval.Size = new System.Drawing.Size(39, 21);
            this.textBoxScanInterval.TabIndex = 1;
            // 
            // textBoxConnectString
            // 
            this.textBoxConnectString.Location = new System.Drawing.Point(206, 195);
            this.textBoxConnectString.Name = "textBoxConnectString";
            this.textBoxConnectString.Size = new System.Drawing.Size(133, 21);
            this.textBoxConnectString.TabIndex = 1;
            // 
            // textBoxTimeOutSec
            // 
            this.textBoxTimeOutSec.Location = new System.Drawing.Point(206, 168);
            this.textBoxTimeOutSec.Name = "textBoxTimeOutSec";
            this.textBoxTimeOutSec.Size = new System.Drawing.Size(39, 21);
            this.textBoxTimeOutSec.TabIndex = 1;
            // 
            // textBoxOpenRetSec
            // 
            this.textBoxOpenRetSec.Location = new System.Drawing.Point(206, 141);
            this.textBoxOpenRetSec.Name = "textBoxOpenRetSec";
            this.textBoxOpenRetSec.Size = new System.Drawing.Size(39, 21);
            this.textBoxOpenRetSec.TabIndex = 1;
            // 
            // textBoxOwnerId
            // 
            this.textBoxOwnerId.Location = new System.Drawing.Point(206, 87);
            this.textBoxOwnerId.Name = "textBoxOwnerId";
            this.textBoxOwnerId.Size = new System.Drawing.Size(100, 21);
            this.textBoxOwnerId.TabIndex = 1;
            // 
            // textBoxDriverNo
            // 
            this.textBoxDriverNo.Location = new System.Drawing.Point(206, 33);
            this.textBoxDriverNo.Name = "textBoxDriverNo";
            this.textBoxDriverNo.Size = new System.Drawing.Size(39, 21);
            this.textBoxDriverNo.TabIndex = 1;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(102, 333);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(76, 12);
            this.label12.TabIndex = 0;
            this.label12.Text = "Class Name";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(78, 306);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(100, 12);
            this.label11.TabIndex = 0;
            this.label11.Text = "Assembly Name";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(117, 279);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(61, 12);
            this.label10.TabIndex = 0;
            this.label10.Text = "Decription";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(123, 252);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(55, 12);
            this.label9.TabIndex = 0;
            this.label9.Text = "Use Flag";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(100, 225);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(78, 12);
            this.label8.TabIndex = 0;
            this.label8.Text = "Scan Interval";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(90, 198);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(88, 12);
            this.label7.TabIndex = 0;
            this.label7.Text = "Connect String";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(101, 171);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 12);
            this.label6.TabIndex = 0;
            this.label6.Text = "Timeout Sec";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(84, 144);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(94, 12);
            this.label5.TabIndex = 0;
            this.label5.Text = "Open Retry Sec";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(103, 117);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 12);
            this.label4.TabIndex = 0;
            this.label4.Text = "Owner Type";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(121, 90);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 12);
            this.label3.TabIndex = 0;
            this.label3.Text = "Owner ID";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(108, 63);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(70, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "Driver Type";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(121, 36);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "Driver No";
            // 
            // AbstractFieldBusDriverWizardForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(446, 438);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "AbstractFieldBusDriverWizardForm";
            this.Text = "AbstractFieldBusDriverWizardForm";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.ResumeLayout(false);
            this.mainPanel.ResumeLayout(false);
            this.mainPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonApply;
        private System.Windows.Forms.Panel mainPanel;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox checkBoxUseFlag;
        private System.Windows.Forms.TextBox textBoxClassName;
        private System.Windows.Forms.TextBox textBoxAssemblyName;
        private System.Windows.Forms.TextBox textBoxDescription;
        private System.Windows.Forms.TextBox textBoxScanInterval;
        private System.Windows.Forms.TextBox textBoxConnectString;
        private System.Windows.Forms.TextBox textBoxTimeOutSec;
        private System.Windows.Forms.TextBox textBoxOpenRetSec;
        private System.Windows.Forms.TextBox textBoxOwnerId;
        private System.Windows.Forms.TextBox textBoxDriverNo;
        private System.Windows.Forms.ComboBox comboBoxOwnerType;
        private System.Windows.Forms.ComboBox comboBoxDriverType;




    }
}