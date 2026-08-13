namespace ECP.Gui.Config.Dialogs
{
    partial class ObservableWizardForm
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
            this.groupBoxConnectString = new System.Windows.Forms.GroupBox();
            this.comboBoxDeviceType = new System.Windows.Forms.ComboBox();
            this.labelLength = new System.Windows.Forms.Label();
            this.labelBitIndex = new System.Windows.Forms.Label();
            this.labelAddressNo = new System.Windows.Forms.Label();
            this.labelDeviceType = new System.Windows.Forms.Label();
            this.textBoxLength = new System.Windows.Forms.TextBox();
            this.textBoxBitIndex = new System.Windows.Forms.TextBox();
            this.textBoxAddressNo = new System.Windows.Forms.TextBox();
            this.checkBoxLogEnable = new System.Windows.Forms.CheckBox();
            this.checkBoxMonFlag = new System.Windows.Forms.CheckBox();
            this.comboBoxDataType = new System.Windows.Forms.ComboBox();
            this.comboBoxOwnerType = new System.Windows.Forms.ComboBox();
            this.textBoxDescription = new System.Windows.Forms.TextBox();
            this.comboBoxAccessType = new System.Windows.Forms.ComboBox();
            this.textBoxDriverNo = new System.Windows.Forms.TextBox();
            this.textBoxMax = new System.Windows.Forms.TextBox();
            this.textBoxMin = new System.Windows.Forms.TextBox();
            this.textBoxObservablePath = new System.Windows.Forms.TextBox();
            this.textBoxObservableID = new System.Windows.Forms.TextBox();
            this.textBoxOwnerID = new System.Windows.Forms.TextBox();
            this.labelDescription = new System.Windows.Forms.Label();
            this.labelLogEnable = new System.Windows.Forms.Label();
            this.labelMonFlag = new System.Windows.Forms.Label();
            this.labelDriverNo = new System.Windows.Forms.Label();
            this.labelMax = new System.Windows.Forms.Label();
            this.labelMin = new System.Windows.Forms.Label();
            this.labelDataType = new System.Windows.Forms.Label();
            this.labelAccessType = new System.Windows.Forms.Label();
            this.labelObservablePath = new System.Windows.Forms.Label();
            this.labelOwnerType = new System.Windows.Forms.Label();
            this.labelObservableID = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonApply = new System.Windows.Forms.Button();
            this.buttonNew = new System.Windows.Forms.Button();
            this.groupBoxConnectString.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxConnectString
            // 
            this.groupBoxConnectString.Controls.Add(this.comboBoxDeviceType);
            this.groupBoxConnectString.Controls.Add(this.labelLength);
            this.groupBoxConnectString.Controls.Add(this.labelBitIndex);
            this.groupBoxConnectString.Controls.Add(this.labelAddressNo);
            this.groupBoxConnectString.Controls.Add(this.labelDeviceType);
            this.groupBoxConnectString.Controls.Add(this.textBoxLength);
            this.groupBoxConnectString.Controls.Add(this.textBoxBitIndex);
            this.groupBoxConnectString.Controls.Add(this.textBoxAddressNo);
            this.groupBoxConnectString.Location = new System.Drawing.Point(59, 331);
            this.groupBoxConnectString.Name = "groupBoxConnectString";
            this.groupBoxConnectString.Size = new System.Drawing.Size(331, 134);
            this.groupBoxConnectString.TabIndex = 12;
            this.groupBoxConnectString.TabStop = false;
            this.groupBoxConnectString.Text = "ConnectString";
            // 
            // comboBoxDeviceType
            // 
            this.comboBoxDeviceType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDeviceType.FormattingEnabled = true;
            this.comboBoxDeviceType.Items.AddRange(new object[] {
            "B",
            "W",
            "R"});
            this.comboBoxDeviceType.Location = new System.Drawing.Point(119, 20);
            this.comboBoxDeviceType.Name = "comboBoxDeviceType";
            this.comboBoxDeviceType.Size = new System.Drawing.Size(71, 20);
            this.comboBoxDeviceType.TabIndex = 0;
            // 
            // labelLength
            // 
            this.labelLength.AutoSize = true;
            this.labelLength.Location = new System.Drawing.Point(61, 109);
            this.labelLength.Name = "labelLength";
            this.labelLength.Size = new System.Drawing.Size(43, 12);
            this.labelLength.TabIndex = 0;
            this.labelLength.Text = "Length";
            // 
            // labelBitIndex
            // 
            this.labelBitIndex.AutoSize = true;
            this.labelBitIndex.Location = new System.Drawing.Point(54, 82);
            this.labelBitIndex.Name = "labelBitIndex";
            this.labelBitIndex.Size = new System.Drawing.Size(50, 12);
            this.labelBitIndex.TabIndex = 0;
            this.labelBitIndex.Text = "BitIndex";
            // 
            // labelAddressNo
            // 
            this.labelAddressNo.AutoSize = true;
            this.labelAddressNo.Location = new System.Drawing.Point(36, 55);
            this.labelAddressNo.Name = "labelAddressNo";
            this.labelAddressNo.Size = new System.Drawing.Size(68, 12);
            this.labelAddressNo.TabIndex = 0;
            this.labelAddressNo.Text = "AddressNo";
            // 
            // labelDeviceType
            // 
            this.labelDeviceType.AutoSize = true;
            this.labelDeviceType.Location = new System.Drawing.Point(32, 28);
            this.labelDeviceType.Name = "labelDeviceType";
            this.labelDeviceType.Size = new System.Drawing.Size(72, 12);
            this.labelDeviceType.TabIndex = 0;
            this.labelDeviceType.Text = "DeviceType";
            // 
            // textBoxLength
            // 
            this.textBoxLength.Location = new System.Drawing.Point(119, 100);
            this.textBoxLength.Name = "textBoxLength";
            this.textBoxLength.Size = new System.Drawing.Size(100, 21);
            this.textBoxLength.TabIndex = 3;
            // 
            // textBoxBitIndex
            // 
            this.textBoxBitIndex.Location = new System.Drawing.Point(119, 73);
            this.textBoxBitIndex.Name = "textBoxBitIndex";
            this.textBoxBitIndex.Size = new System.Drawing.Size(100, 21);
            this.textBoxBitIndex.TabIndex = 2;
            // 
            // textBoxAddressNo
            // 
            this.textBoxAddressNo.Location = new System.Drawing.Point(119, 46);
            this.textBoxAddressNo.Name = "textBoxAddressNo";
            this.textBoxAddressNo.Size = new System.Drawing.Size(100, 21);
            this.textBoxAddressNo.TabIndex = 1;
            // 
            // checkBoxLogEnable
            // 
            this.checkBoxLogEnable.AutoSize = true;
            this.checkBoxLogEnable.Location = new System.Drawing.Point(178, 273);
            this.checkBoxLogEnable.Name = "checkBoxLogEnable";
            this.checkBoxLogEnable.Size = new System.Drawing.Size(15, 14);
            this.checkBoxLogEnable.TabIndex = 10;
            this.checkBoxLogEnable.UseVisualStyleBackColor = true;
            // 
            // checkBoxMonFlag
            // 
            this.checkBoxMonFlag.AutoSize = true;
            this.checkBoxMonFlag.Location = new System.Drawing.Point(178, 253);
            this.checkBoxMonFlag.Name = "checkBoxMonFlag";
            this.checkBoxMonFlag.Size = new System.Drawing.Size(15, 14);
            this.checkBoxMonFlag.TabIndex = 9;
            this.checkBoxMonFlag.UseVisualStyleBackColor = true;
            // 
            // comboBoxDataType
            // 
            this.comboBoxDataType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDataType.FormattingEnabled = true;
            this.comboBoxDataType.Items.AddRange(new object[] {
            "Boolean",
            "String",
            "Short",
            "Bool Array",
            "Short Array"});
            this.comboBoxDataType.Location = new System.Drawing.Point(178, 143);
            this.comboBoxDataType.Name = "comboBoxDataType";
            this.comboBoxDataType.Size = new System.Drawing.Size(73, 20);
            this.comboBoxDataType.TabIndex = 5;
            // 
            // comboBoxOwnerType
            // 
            this.comboBoxOwnerType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxOwnerType.FormattingEnabled = true;
            this.comboBoxOwnerType.Items.AddRange(new object[] {
            "Equipment",
            "Module",
            "IODevice",
            "SubSystem"});
            this.comboBoxOwnerType.Location = new System.Drawing.Point(178, 64);
            this.comboBoxOwnerType.Name = "comboBoxOwnerType";
            this.comboBoxOwnerType.Size = new System.Drawing.Size(73, 20);
            this.comboBoxOwnerType.TabIndex = 2;
            // 
            // textBoxDescription
            // 
            this.textBoxDescription.Location = new System.Drawing.Point(178, 293);
            this.textBoxDescription.Name = "textBoxDescription";
            this.textBoxDescription.Size = new System.Drawing.Size(181, 21);
            this.textBoxDescription.TabIndex = 11;
            // 
            // comboBoxAccessType
            // 
            this.comboBoxAccessType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAccessType.FormattingEnabled = true;
            this.comboBoxAccessType.Items.AddRange(new object[] {
            "In",
            "Out",
            "In_Out"});
            this.comboBoxAccessType.Location = new System.Drawing.Point(178, 117);
            this.comboBoxAccessType.Name = "comboBoxAccessType";
            this.comboBoxAccessType.Size = new System.Drawing.Size(73, 20);
            this.comboBoxAccessType.TabIndex = 4;
            // 
            // textBoxDriverNo
            // 
            this.textBoxDriverNo.Location = new System.Drawing.Point(178, 223);
            this.textBoxDriverNo.Name = "textBoxDriverNo";
            this.textBoxDriverNo.Size = new System.Drawing.Size(34, 21);
            this.textBoxDriverNo.TabIndex = 8;
            this.textBoxDriverNo.Text = "1";
            // 
            // textBoxMax
            // 
            this.textBoxMax.Location = new System.Drawing.Point(178, 196);
            this.textBoxMax.Name = "textBoxMax";
            this.textBoxMax.Size = new System.Drawing.Size(34, 21);
            this.textBoxMax.TabIndex = 7;
            // 
            // textBoxMin
            // 
            this.textBoxMin.Location = new System.Drawing.Point(178, 169);
            this.textBoxMin.Name = "textBoxMin";
            this.textBoxMin.Size = new System.Drawing.Size(34, 21);
            this.textBoxMin.TabIndex = 6;
            this.textBoxMin.Text = "0";
            // 
            // textBoxObservablePath
            // 
            this.textBoxObservablePath.Location = new System.Drawing.Point(178, 90);
            this.textBoxObservablePath.Name = "textBoxObservablePath";
            this.textBoxObservablePath.Size = new System.Drawing.Size(181, 21);
            this.textBoxObservablePath.TabIndex = 3;
            // 
            // textBoxObservableID
            // 
            this.textBoxObservableID.Location = new System.Drawing.Point(178, 36);
            this.textBoxObservableID.Name = "textBoxObservableID";
            this.textBoxObservableID.Size = new System.Drawing.Size(181, 21);
            this.textBoxObservableID.TabIndex = 1;
            // 
            // textBoxOwnerID
            // 
            this.textBoxOwnerID.Location = new System.Drawing.Point(178, 10);
            this.textBoxOwnerID.Name = "textBoxOwnerID";
            this.textBoxOwnerID.Size = new System.Drawing.Size(100, 21);
            this.textBoxOwnerID.TabIndex = 0;
            // 
            // labelDescription
            // 
            this.labelDescription.AutoSize = true;
            this.labelDescription.Location = new System.Drawing.Point(98, 302);
            this.labelDescription.Name = "labelDescription";
            this.labelDescription.Size = new System.Drawing.Size(68, 12);
            this.labelDescription.TabIndex = 0;
            this.labelDescription.Text = "Description";
            // 
            // labelLogEnable
            // 
            this.labelLogEnable.AutoSize = true;
            this.labelLogEnable.Location = new System.Drawing.Point(98, 275);
            this.labelLogEnable.Name = "labelLogEnable";
            this.labelLogEnable.Size = new System.Drawing.Size(65, 12);
            this.labelLogEnable.TabIndex = 0;
            this.labelLogEnable.Text = "LogEnable";
            // 
            // labelMonFlag
            // 
            this.labelMonFlag.AutoSize = true;
            this.labelMonFlag.Location = new System.Drawing.Point(109, 255);
            this.labelMonFlag.Name = "labelMonFlag";
            this.labelMonFlag.Size = new System.Drawing.Size(54, 12);
            this.labelMonFlag.TabIndex = 0;
            this.labelMonFlag.Text = "MonFlag";
            // 
            // labelDriverNo
            // 
            this.labelDriverNo.AutoSize = true;
            this.labelDriverNo.Location = new System.Drawing.Point(110, 232);
            this.labelDriverNo.Name = "labelDriverNo";
            this.labelDriverNo.Size = new System.Drawing.Size(53, 12);
            this.labelDriverNo.TabIndex = 0;
            this.labelDriverNo.Text = "DriverNo";
            // 
            // labelMax
            // 
            this.labelMax.AutoSize = true;
            this.labelMax.Location = new System.Drawing.Point(133, 205);
            this.labelMax.Name = "labelMax";
            this.labelMax.Size = new System.Drawing.Size(30, 12);
            this.labelMax.TabIndex = 0;
            this.labelMax.Text = "Max";
            // 
            // labelMin
            // 
            this.labelMin.AutoSize = true;
            this.labelMin.Location = new System.Drawing.Point(137, 178);
            this.labelMin.Name = "labelMin";
            this.labelMin.Size = new System.Drawing.Size(26, 12);
            this.labelMin.TabIndex = 0;
            this.labelMin.Text = "Min";
            // 
            // labelDataType
            // 
            this.labelDataType.AutoSize = true;
            this.labelDataType.Location = new System.Drawing.Point(104, 151);
            this.labelDataType.Name = "labelDataType";
            this.labelDataType.Size = new System.Drawing.Size(59, 12);
            this.labelDataType.TabIndex = 0;
            this.labelDataType.Text = "DataType";
            // 
            // labelAccessType
            // 
            this.labelAccessType.AutoSize = true;
            this.labelAccessType.Location = new System.Drawing.Point(86, 125);
            this.labelAccessType.Name = "labelAccessType";
            this.labelAccessType.Size = new System.Drawing.Size(77, 12);
            this.labelAccessType.TabIndex = 0;
            this.labelAccessType.Text = "AccessType";
            // 
            // labelObservablePath
            // 
            this.labelObservablePath.AutoSize = true;
            this.labelObservablePath.Location = new System.Drawing.Point(69, 99);
            this.labelObservablePath.Name = "labelObservablePath";
            this.labelObservablePath.Size = new System.Drawing.Size(94, 12);
            this.labelObservablePath.TabIndex = 0;
            this.labelObservablePath.Text = "ObservablePath";
            // 
            // labelOwnerType
            // 
            this.labelOwnerType.AutoSize = true;
            this.labelOwnerType.Location = new System.Drawing.Point(92, 72);
            this.labelOwnerType.Name = "labelOwnerType";
            this.labelOwnerType.Size = new System.Drawing.Size(71, 12);
            this.labelOwnerType.TabIndex = 0;
            this.labelOwnerType.Text = "OwnerType";
            // 
            // labelObservableID
            // 
            this.labelObservableID.AutoSize = true;
            this.labelObservableID.Location = new System.Drawing.Point(83, 45);
            this.labelObservableID.Name = "labelObservableID";
            this.labelObservableID.Size = new System.Drawing.Size(80, 12);
            this.labelObservableID.TabIndex = 0;
            this.labelObservableID.Text = "ObservableID";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.flowLayoutPanel1, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 92.49531F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 7.504691F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(481, 530);
            this.tableLayoutPanel1.TabIndex = 13;
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.groupBoxConnectString);
            this.panel1.Controls.Add(this.labelDescription);
            this.panel1.Controls.Add(this.labelLogEnable);
            this.panel1.Controls.Add(this.checkBoxLogEnable);
            this.panel1.Controls.Add(this.textBoxOwnerID);
            this.panel1.Controls.Add(this.labelMonFlag);
            this.panel1.Controls.Add(this.checkBoxMonFlag);
            this.panel1.Controls.Add(this.textBoxObservableID);
            this.panel1.Controls.Add(this.comboBoxDataType);
            this.panel1.Controls.Add(this.labelDriverNo);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.labelObservableID);
            this.panel1.Controls.Add(this.textBoxObservablePath);
            this.panel1.Controls.Add(this.comboBoxOwnerType);
            this.panel1.Controls.Add(this.labelMax);
            this.panel1.Controls.Add(this.labelOwnerType);
            this.panel1.Controls.Add(this.textBoxMin);
            this.panel1.Controls.Add(this.comboBoxAccessType);
            this.panel1.Controls.Add(this.labelMin);
            this.panel1.Controls.Add(this.labelObservablePath);
            this.panel1.Controls.Add(this.textBoxMax);
            this.panel1.Controls.Add(this.textBoxDescription);
            this.panel1.Controls.Add(this.labelDataType);
            this.panel1.Controls.Add(this.labelAccessType);
            this.panel1.Controls.Add(this.textBoxDriverNo);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(475, 484);
            this.panel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(110, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "OwnerID";
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.flowLayoutPanel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.flowLayoutPanel1.Controls.Add(this.buttonCancel);
            this.flowLayoutPanel1.Controls.Add(this.buttonOK);
            this.flowLayoutPanel1.Controls.Add(this.buttonApply);
            this.flowLayoutPanel1.Controls.Add(this.buttonNew);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(3, 493);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(475, 34);
            this.flowLayoutPanel1.TabIndex = 1;
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(395, 3);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 2;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.HandleButtons);
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(314, 3);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 1;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.HandleButtons);
            // 
            // buttonApply
            // 
            this.buttonApply.Location = new System.Drawing.Point(233, 3);
            this.buttonApply.Name = "buttonApply";
            this.buttonApply.Size = new System.Drawing.Size(75, 23);
            this.buttonApply.TabIndex = 0;
            this.buttonApply.Text = "Apply";
            this.buttonApply.UseVisualStyleBackColor = true;
            this.buttonApply.Click += new System.EventHandler(this.HandleButtons);
            // 
            // buttonNew
            // 
            this.buttonNew.Location = new System.Drawing.Point(152, 3);
            this.buttonNew.Name = "buttonNew";
            this.buttonNew.Size = new System.Drawing.Size(75, 23);
            this.buttonNew.TabIndex = 3;
            this.buttonNew.Text = "New";
            this.buttonNew.UseVisualStyleBackColor = true;
            this.buttonNew.Click += new System.EventHandler(this.HandleButtons);
            // 
            // ObservableWizardForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(481, 530);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "ObservableWizardForm";
            this.Text = "ObservableWizardForm";
            this.groupBoxConnectString.ResumeLayout(false);
            this.groupBoxConnectString.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.flowLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxConnectString;
        private System.Windows.Forms.Label labelLength;
        private System.Windows.Forms.Label labelBitIndex;
        private System.Windows.Forms.Label labelAddressNo;
        private System.Windows.Forms.Label labelDeviceType;
        private System.Windows.Forms.Label labelDescription;
        private System.Windows.Forms.Label labelLogEnable;
        private System.Windows.Forms.Label labelMonFlag;
        private System.Windows.Forms.Label labelDriverNo;
        private System.Windows.Forms.Label labelMax;
        private System.Windows.Forms.Label labelMin;
        private System.Windows.Forms.Label labelDataType;
        private System.Windows.Forms.Label labelAccessType;
        private System.Windows.Forms.Label labelObservablePath;
        private System.Windows.Forms.Label labelOwnerType;
        private System.Windows.Forms.Label labelObservableID;
        protected System.Windows.Forms.ComboBox comboBoxDeviceType;
        protected System.Windows.Forms.TextBox textBoxLength;
        protected System.Windows.Forms.TextBox textBoxBitIndex;
        protected System.Windows.Forms.TextBox textBoxAddressNo;
        protected System.Windows.Forms.CheckBox checkBoxLogEnable;
        protected System.Windows.Forms.CheckBox checkBoxMonFlag;
        protected System.Windows.Forms.ComboBox comboBoxDataType;
        protected System.Windows.Forms.ComboBox comboBoxOwnerType;
        protected System.Windows.Forms.TextBox textBoxDescription;
        protected System.Windows.Forms.ComboBox comboBoxAccessType;
        protected System.Windows.Forms.TextBox textBoxDriverNo;
        protected System.Windows.Forms.TextBox textBoxMax;
        protected System.Windows.Forms.TextBox textBoxMin;
        protected System.Windows.Forms.TextBox textBoxObservablePath;
        protected System.Windows.Forms.TextBox textBoxObservableID;
        protected System.Windows.Forms.TextBox textBoxOwnerID;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        protected System.Windows.Forms.Button buttonNew;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonApply;
    }
}