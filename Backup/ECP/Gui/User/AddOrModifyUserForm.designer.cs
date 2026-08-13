namespace ECP.Gui.User
{
    partial class AddOrModifyUserForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AddOrModifyUserForm));
            this.tableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.flowLayoutPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.cancleButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.headerPanel = new System.Windows.Forms.Panel();
            this.numericUpDownPassworldDuration = new System.Windows.Forms.NumericUpDown();
            this.checkBoxUserDiable = new System.Windows.Forms.CheckBox();
            this.checkBoxPassworldExpire = new System.Windows.Forms.CheckBox();
            this.maskedTextBoxPasswordConfirm = new System.Windows.Forms.MaskedTextBox();
            this.maskedTextBoxPassworld = new System.Windows.Forms.MaskedTextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.labelUserID = new System.Windows.Forms.Label();
            this.textBoxDesc = new System.Windows.Forms.TextBox();
            this.textBoxUserName = new System.Windows.Forms.TextBox();
            this.textBoxUserID = new System.Windows.Forms.TextBox();
            this.middlePanel = new System.Windows.Forms.Panel();
            this.rightButton = new System.Windows.Forms.Button();
            this.downButton = new System.Windows.Forms.Button();
            this.upButton = new System.Windows.Forms.Button();
            this.leftButton = new System.Windows.Forms.Button();
            this.availableListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.assignedListView = new System.Windows.Forms.ListView();
            this.columnAssigned = new System.Windows.Forms.ColumnHeader();
            this.tableLayoutPanel.SuspendLayout();
            this.flowLayoutPanel.SuspendLayout();
            this.headerPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPassworldDuration)).BeginInit();
            this.middlePanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel
            // 
            this.tableLayoutPanel.ColumnCount = 1;
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel.Controls.Add(this.flowLayoutPanel, 0, 2);
            this.tableLayoutPanel.Controls.Add(this.headerPanel, 0, 0);
            this.tableLayoutPanel.Controls.Add(this.middlePanel, 0, 1);
            this.tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel.Name = "tableLayoutPanel";
            this.tableLayoutPanel.RowCount = 3;
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 49.83108F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50.16892F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 36F));
            this.tableLayoutPanel.Size = new System.Drawing.Size(570, 629);
            this.tableLayoutPanel.TabIndex = 0;
            // 
            // flowLayoutPanel
            // 
            this.flowLayoutPanel.Controls.Add(this.cancleButton);
            this.flowLayoutPanel.Controls.Add(this.okButton);
            this.flowLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel.Location = new System.Drawing.Point(3, 595);
            this.flowLayoutPanel.Name = "flowLayoutPanel";
            this.flowLayoutPanel.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.flowLayoutPanel.Size = new System.Drawing.Size(564, 31);
            this.flowLayoutPanel.TabIndex = 0;
            // 
            // cancleButton
            // 
            this.cancleButton.Location = new System.Drawing.Point(486, 3);
            this.cancleButton.Name = "cancleButton";
            this.cancleButton.Size = new System.Drawing.Size(75, 23);
            this.cancleButton.TabIndex = 11;
            this.cancleButton.Text = "Cancel";
            this.cancleButton.UseVisualStyleBackColor = true;
            this.cancleButton.Click += new System.EventHandler(this.cancleButton_Click);
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(405, 3);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 10;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // headerPanel
            // 
            this.headerPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.headerPanel.Controls.Add(this.numericUpDownPassworldDuration);
            this.headerPanel.Controls.Add(this.checkBoxUserDiable);
            this.headerPanel.Controls.Add(this.checkBoxPassworldExpire);
            this.headerPanel.Controls.Add(this.maskedTextBoxPasswordConfirm);
            this.headerPanel.Controls.Add(this.maskedTextBoxPassworld);
            this.headerPanel.Controls.Add(this.label7);
            this.headerPanel.Controls.Add(this.label6);
            this.headerPanel.Controls.Add(this.label5);
            this.headerPanel.Controls.Add(this.label4);
            this.headerPanel.Controls.Add(this.label3);
            this.headerPanel.Controls.Add(this.label2);
            this.headerPanel.Controls.Add(this.labelUserID);
            this.headerPanel.Controls.Add(this.textBoxDesc);
            this.headerPanel.Controls.Add(this.textBoxUserName);
            this.headerPanel.Controls.Add(this.textBoxUserID);
            this.headerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.headerPanel.Location = new System.Drawing.Point(3, 3);
            this.headerPanel.Name = "headerPanel";
            this.headerPanel.Size = new System.Drawing.Size(564, 289);
            this.headerPanel.TabIndex = 1;
            // 
            // numericUpDownPassworldDuration
            // 
            this.numericUpDownPassworldDuration.Location = new System.Drawing.Point(215, 213);
            this.numericUpDownPassworldDuration.Name = "numericUpDownPassworldDuration";
            this.numericUpDownPassworldDuration.Size = new System.Drawing.Size(50, 21);
            this.numericUpDownPassworldDuration.TabIndex = 6;
            // 
            // checkBoxUserDiable
            // 
            this.checkBoxUserDiable.AutoSize = true;
            this.checkBoxUserDiable.Location = new System.Drawing.Point(215, 252);
            this.checkBoxUserDiable.Name = "checkBoxUserDiable";
            this.checkBoxUserDiable.Size = new System.Drawing.Size(15, 14);
            this.checkBoxUserDiable.TabIndex = 7;
            this.checkBoxUserDiable.UseVisualStyleBackColor = true;
            // 
            // checkBoxPassworldExpire
            // 
            this.checkBoxPassworldExpire.AutoSize = true;
            this.checkBoxPassworldExpire.Location = new System.Drawing.Point(215, 181);
            this.checkBoxPassworldExpire.Name = "checkBoxPassworldExpire";
            this.checkBoxPassworldExpire.Size = new System.Drawing.Size(57, 16);
            this.checkBoxPassworldExpire.TabIndex = 5;
            this.checkBoxPassworldExpire.Text = "Never";
            this.checkBoxPassworldExpire.UseVisualStyleBackColor = true;
            this.checkBoxPassworldExpire.CheckedChanged += new System.EventHandler(this.checkBoxPassworldExpire_CheckedChanged);
            // 
            // maskedTextBoxPasswordConfirm
            // 
            this.maskedTextBoxPasswordConfirm.Location = new System.Drawing.Point(215, 138);
            this.maskedTextBoxPasswordConfirm.Name = "maskedTextBoxPasswordConfirm";
            this.maskedTextBoxPasswordConfirm.PasswordChar = '*';
            this.maskedTextBoxPasswordConfirm.Size = new System.Drawing.Size(197, 21);
            this.maskedTextBoxPasswordConfirm.TabIndex = 4;
            // 
            // maskedTextBoxPassworld
            // 
            this.maskedTextBoxPassworld.Location = new System.Drawing.Point(215, 111);
            this.maskedTextBoxPassworld.Name = "maskedTextBoxPassworld";
            this.maskedTextBoxPassworld.PasswordChar = '*';
            this.maskedTextBoxPassworld.Size = new System.Drawing.Size(197, 21);
            this.maskedTextBoxPassworld.TabIndex = 3;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(154, 252);
            this.label7.Name = "label7";
            this.label7.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.label7.Size = new System.Drawing.Size(54, 12);
            this.label7.TabIndex = 1;
            this.label7.Text = "Disabled";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(99, 181);
            this.label6.Name = "label6";
            this.label6.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.label6.Size = new System.Drawing.Size(109, 12);
            this.label6.TabIndex = 1;
            this.label6.Text = "Password Expires";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(96, 142);
            this.label5.Name = "label5";
            this.label5.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.label5.Size = new System.Drawing.Size(110, 12);
            this.label5.TabIndex = 1;
            this.label5.Text = "Confirm Password";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(144, 111);
            this.label4.Name = "label4";
            this.label4.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.label4.Size = new System.Drawing.Size(62, 12);
            this.label4.TabIndex = 1;
            this.label4.Text = "Password";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(144, 84);
            this.label3.Name = "label3";
            this.label3.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.label3.Size = new System.Drawing.Size(68, 12);
            this.label3.TabIndex = 1;
            this.label3.Text = "Description";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(144, 56);
            this.label2.Name = "label2";
            this.label2.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.label2.Size = new System.Drawing.Size(65, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "UserName";
            // 
            // labelUserID
            // 
            this.labelUserID.AutoSize = true;
            this.labelUserID.Location = new System.Drawing.Point(167, 27);
            this.labelUserID.Name = "labelUserID";
            this.labelUserID.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.labelUserID.Size = new System.Drawing.Size(42, 12);
            this.labelUserID.TabIndex = 1;
            this.labelUserID.Text = "UserID";
            // 
            // textBoxDesc
            // 
            this.textBoxDesc.Location = new System.Drawing.Point(215, 81);
            this.textBoxDesc.Name = "textBoxDesc";
            this.textBoxDesc.Size = new System.Drawing.Size(197, 21);
            this.textBoxDesc.TabIndex = 2;
            // 
            // textBoxUserName
            // 
            this.textBoxUserName.Location = new System.Drawing.Point(215, 53);
            this.textBoxUserName.Name = "textBoxUserName";
            this.textBoxUserName.Size = new System.Drawing.Size(197, 21);
            this.textBoxUserName.TabIndex = 1;
            // 
            // textBoxUserID
            // 
            this.textBoxUserID.Location = new System.Drawing.Point(215, 24);
            this.textBoxUserID.Name = "textBoxUserID";
            this.textBoxUserID.Size = new System.Drawing.Size(197, 21);
            this.textBoxUserID.TabIndex = 0;
            // 
            // middlePanel
            // 
            this.middlePanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.middlePanel.Controls.Add(this.rightButton);
            this.middlePanel.Controls.Add(this.downButton);
            this.middlePanel.Controls.Add(this.upButton);
            this.middlePanel.Controls.Add(this.leftButton);
            this.middlePanel.Controls.Add(this.availableListView);
            this.middlePanel.Controls.Add(this.assignedListView);
            this.middlePanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.middlePanel.Location = new System.Drawing.Point(3, 298);
            this.middlePanel.Name = "middlePanel";
            this.middlePanel.Size = new System.Drawing.Size(564, 291);
            this.middlePanel.TabIndex = 2;
            // 
            // rightButton
            // 
            this.rightButton.Image = ((System.Drawing.Image)(resources.GetObject("rightButton.Image")));
            this.rightButton.Location = new System.Drawing.Point(286, 126);
            this.rightButton.Name = "rightButton";
            this.rightButton.Size = new System.Drawing.Size(40, 39);
            this.rightButton.TabIndex = 8;
            this.rightButton.UseVisualStyleBackColor = true;
            this.rightButton.Click += new System.EventHandler(this.rightButton_Click);
            // 
            // downButton
            // 
            this.downButton.Image = ((System.Drawing.Image)(resources.GetObject("downButton.Image")));
            this.downButton.Location = new System.Drawing.Point(267, 171);
            this.downButton.Name = "downButton";
            this.downButton.Size = new System.Drawing.Size(38, 37);
            this.downButton.TabIndex = 7;
            this.downButton.UseVisualStyleBackColor = true;
            this.downButton.Click += new System.EventHandler(this.downButton_Click);
            // 
            // upButton
            // 
            this.upButton.Image = ((System.Drawing.Image)(resources.GetObject("upButton.Image")));
            this.upButton.Location = new System.Drawing.Point(267, 81);
            this.upButton.Name = "upButton";
            this.upButton.Size = new System.Drawing.Size(38, 38);
            this.upButton.TabIndex = 6;
            this.upButton.UseVisualStyleBackColor = true;
            this.upButton.Click += new System.EventHandler(this.upButton_Click);
            // 
            // leftButton
            // 
            this.leftButton.Image = ((System.Drawing.Image)(resources.GetObject("leftButton.Image")));
            this.leftButton.Location = new System.Drawing.Point(243, 126);
            this.leftButton.Name = "leftButton";
            this.leftButton.Size = new System.Drawing.Size(38, 39);
            this.leftButton.TabIndex = 5;
            this.leftButton.UseVisualStyleBackColor = true;
            this.leftButton.Click += new System.EventHandler(this.leftButton_Click);
            // 
            // availableListView
            // 
            this.availableListView.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.availableListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.availableListView.FullRowSelect = true;
            this.availableListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.availableListView.HideSelection = false;
            this.availableListView.Location = new System.Drawing.Point(340, 21);
            this.availableListView.Name = "availableListView";
            this.availableListView.Size = new System.Drawing.Size(190, 244);
            this.availableListView.TabIndex = 9;
            this.availableListView.UseCompatibleStateImageBehavior = false;
            this.availableListView.View = System.Windows.Forms.View.Details;
            this.availableListView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.assignedListView_MouseUp);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "";
            this.columnHeader1.Width = 185;
            // 
            // assignedListView
            // 
            this.assignedListView.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.assignedListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnAssigned});
            this.assignedListView.FullRowSelect = true;
            this.assignedListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.assignedListView.HideSelection = false;
            this.assignedListView.Location = new System.Drawing.Point(37, 21);
            this.assignedListView.Name = "assignedListView";
            this.assignedListView.Size = new System.Drawing.Size(190, 244);
            this.assignedListView.TabIndex = 8;
            this.assignedListView.UseCompatibleStateImageBehavior = false;
            this.assignedListView.View = System.Windows.Forms.View.Details;
            this.assignedListView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.assignedListView_MouseUp);
            // 
            // columnAssigned
            // 
            this.columnAssigned.Text = "";
            this.columnAssigned.Width = 185;
            // 
            // AddOrModifyUserForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(570, 629);
            this.Controls.Add(this.tableLayoutPanel);
            this.Name = "AddOrModifyUserForm";
            this.Text = "AddOrModifyUserForm";
            this.tableLayoutPanel.ResumeLayout(false);
            this.flowLayoutPanel.ResumeLayout(false);
            this.headerPanel.ResumeLayout(false);
            this.headerPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPassworldDuration)).EndInit();
            this.middlePanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel;
        private System.Windows.Forms.Button cancleButton;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Panel headerPanel;
        private System.Windows.Forms.Panel middlePanel;
        private System.Windows.Forms.ListView assignedListView;
        private System.Windows.Forms.ColumnHeader columnAssigned;
        private System.Windows.Forms.Button rightButton;
        private System.Windows.Forms.Button downButton;
        private System.Windows.Forms.Button upButton;
        private System.Windows.Forms.Button leftButton;
        private System.Windows.Forms.ListView availableListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.TextBox textBoxUserID;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelUserID;
        private System.Windows.Forms.TextBox textBoxDesc;
        private System.Windows.Forms.TextBox textBoxUserName;
        private System.Windows.Forms.NumericUpDown numericUpDownPassworldDuration;
        private System.Windows.Forms.CheckBox checkBoxPassworldExpire;
        private System.Windows.Forms.MaskedTextBox maskedTextBoxPasswordConfirm;
        private System.Windows.Forms.MaskedTextBox maskedTextBoxPassworld;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckBox checkBoxUserDiable;
        private System.Windows.Forms.Label label7;
    }
}