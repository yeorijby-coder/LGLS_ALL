namespace ECP.Gui.User
{
    partial class AddOrModifyRoleForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AddOrModifyRoleForm));
            this.tableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.flowLayoutPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.cancelButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.headerPanel = new System.Windows.Forms.Panel();
            this.roleDescTextBox = new System.Windows.Forms.TextBox();
            this.roleIDTextBox = new System.Windows.Forms.TextBox();
            this.roleIDLabel = new System.Windows.Forms.Label();
            this.descLabel = new System.Windows.Forms.Label();
            this.middlePanel = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.rightButton = new System.Windows.Forms.Button();
            this.downButton = new System.Windows.Forms.Button();
            this.upButton = new System.Windows.Forms.Button();
            this.leftButton = new System.Windows.Forms.Button();
            this.assignedPermListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.availablePermissionListView = new System.Windows.Forms.ListView();
            this.columnAvailablePermId = new System.Windows.Forms.ColumnHeader();
            this.tableLayoutPanel.SuspendLayout();
            this.flowLayoutPanel.SuspendLayout();
            this.headerPanel.SuspendLayout();
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
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20.91503F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 79.08497F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 38F));
            this.tableLayoutPanel.Size = new System.Drawing.Size(740, 597);
            this.tableLayoutPanel.TabIndex = 0;
            // 
            // flowLayoutPanel
            // 
            this.flowLayoutPanel.Controls.Add(this.cancelButton);
            this.flowLayoutPanel.Controls.Add(this.okButton);
            this.flowLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.flowLayoutPanel.Location = new System.Drawing.Point(3, 561);
            this.flowLayoutPanel.Name = "flowLayoutPanel";
            this.flowLayoutPanel.Size = new System.Drawing.Size(734, 33);
            this.flowLayoutPanel.TabIndex = 0;
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(656, 3);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 0;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(575, 3);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 1;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // headerPanel
            // 
            this.headerPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.headerPanel.Controls.Add(this.roleDescTextBox);
            this.headerPanel.Controls.Add(this.roleIDTextBox);
            this.headerPanel.Controls.Add(this.roleIDLabel);
            this.headerPanel.Controls.Add(this.descLabel);
            this.headerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.headerPanel.Location = new System.Drawing.Point(3, 3);
            this.headerPanel.Name = "headerPanel";
            this.headerPanel.Size = new System.Drawing.Size(734, 110);
            this.headerPanel.TabIndex = 1;
            // 
            // roleDescTextBox
            // 
            this.roleDescTextBox.Location = new System.Drawing.Point(258, 46);
            this.roleDescTextBox.Multiline = true;
            this.roleDescTextBox.Name = "roleDescTextBox";
            this.roleDescTextBox.Size = new System.Drawing.Size(244, 47);
            this.roleDescTextBox.TabIndex = 2;
            // 
            // roleIDTextBox
            // 
            this.roleIDTextBox.Location = new System.Drawing.Point(258, 19);
            this.roleIDTextBox.Name = "roleIDTextBox";
            this.roleIDTextBox.Size = new System.Drawing.Size(244, 21);
            this.roleIDTextBox.TabIndex = 1;
            // 
            // roleIDLabel
            // 
            this.roleIDLabel.AutoSize = true;
            this.roleIDLabel.Location = new System.Drawing.Point(210, 22);
            this.roleIDLabel.Name = "roleIDLabel";
            this.roleIDLabel.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.roleIDLabel.Size = new System.Drawing.Size(41, 12);
            this.roleIDLabel.TabIndex = 0;
            this.roleIDLabel.Text = "RoleID";
            // 
            // descLabel
            // 
            this.descLabel.AutoSize = true;
            this.descLabel.Location = new System.Drawing.Point(184, 49);
            this.descLabel.Name = "descLabel";
            this.descLabel.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.descLabel.Size = new System.Drawing.Size(68, 12);
            this.descLabel.TabIndex = 0;
            this.descLabel.Text = "Description";
            // 
            // middlePanel
            // 
            this.middlePanel.Controls.Add(this.label2);
            this.middlePanel.Controls.Add(this.label1);
            this.middlePanel.Controls.Add(this.rightButton);
            this.middlePanel.Controls.Add(this.downButton);
            this.middlePanel.Controls.Add(this.upButton);
            this.middlePanel.Controls.Add(this.leftButton);
            this.middlePanel.Controls.Add(this.assignedPermListView);
            this.middlePanel.Controls.Add(this.availablePermissionListView);
            this.middlePanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.middlePanel.Location = new System.Drawing.Point(3, 119);
            this.middlePanel.Name = "middlePanel";
            this.middlePanel.Size = new System.Drawing.Size(734, 436);
            this.middlePanel.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(431, 11);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(115, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "Available Privileges";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(117, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "Assigned Privileges";
            // 
            // rightButton
            // 
            this.rightButton.Image = ((System.Drawing.Image)(resources.GetObject("rightButton.Image")));
            this.rightButton.Location = new System.Drawing.Point(374, 186);
            this.rightButton.Name = "rightButton";
            this.rightButton.Size = new System.Drawing.Size(46, 47);
            this.rightButton.TabIndex = 4;
            this.rightButton.UseVisualStyleBackColor = true;
            this.rightButton.Click += new System.EventHandler(this.rightButton_Click);
            // 
            // downButton
            // 
            this.downButton.Image = ((System.Drawing.Image)(resources.GetObject("downButton.Image")));
            this.downButton.Location = new System.Drawing.Point(346, 240);
            this.downButton.Name = "downButton";
            this.downButton.Size = new System.Drawing.Size(45, 43);
            this.downButton.TabIndex = 3;
            this.downButton.UseVisualStyleBackColor = true;
            this.downButton.Click += new System.EventHandler(this.downButton_Click);
            // 
            // upButton
            // 
            this.upButton.Image = ((System.Drawing.Image)(resources.GetObject("upButton.Image")));
            this.upButton.Location = new System.Drawing.Point(346, 139);
            this.upButton.Name = "upButton";
            this.upButton.Size = new System.Drawing.Size(45, 41);
            this.upButton.TabIndex = 2;
            this.upButton.UseVisualStyleBackColor = true;
            this.upButton.Click += new System.EventHandler(this.upButton_Click);
            // 
            // leftButton
            // 
            this.leftButton.Image = ((System.Drawing.Image)(resources.GetObject("leftButton.Image")));
            this.leftButton.Location = new System.Drawing.Point(312, 186);
            this.leftButton.Name = "leftButton";
            this.leftButton.Size = new System.Drawing.Size(47, 47);
            this.leftButton.TabIndex = 1;
            this.leftButton.UseVisualStyleBackColor = true;
            this.leftButton.Click += new System.EventHandler(this.leftButton_Click);
            // 
            // assignedPermListView
            // 
            this.assignedPermListView.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.assignedPermListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.assignedPermListView.FullRowSelect = true;
            this.assignedPermListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.assignedPermListView.HideSelection = false;
            this.assignedPermListView.Location = new System.Drawing.Point(14, 29);
            this.assignedPermListView.Name = "assignedPermListView";
            this.assignedPermListView.Size = new System.Drawing.Size(292, 401);
            this.assignedPermListView.TabIndex = 3;
            this.assignedPermListView.UseCompatibleStateImageBehavior = false;
            this.assignedPermListView.View = System.Windows.Forms.View.Details;
            this.assignedPermListView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.assignedPermListView_MouseUp);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "";
            this.columnHeader1.Width = 288;
            // 
            // availablePermissionListView
            // 
            this.availablePermissionListView.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.availablePermissionListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnAvailablePermId});
            this.availablePermissionListView.FullRowSelect = true;
            this.availablePermissionListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.availablePermissionListView.HideSelection = false;
            this.availablePermissionListView.Location = new System.Drawing.Point(433, 29);
            this.availablePermissionListView.Name = "availablePermissionListView";
            this.availablePermissionListView.Size = new System.Drawing.Size(292, 401);
            this.availablePermissionListView.TabIndex = 4;
            this.availablePermissionListView.UseCompatibleStateImageBehavior = false;
            this.availablePermissionListView.View = System.Windows.Forms.View.Details;
            this.availablePermissionListView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.assignedPermListView_MouseUp);
            // 
            // columnAvailablePermId
            // 
            this.columnAvailablePermId.Text = "";
            this.columnAvailablePermId.Width = 288;
            // 
            // AddOrModifyRoleForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(740, 597);
            this.Controls.Add(this.tableLayoutPanel);
            this.Name = "AddOrModifyRoleForm";
            this.Text = "AddOrModifyRoleForm";
            this.tableLayoutPanel.ResumeLayout(false);
            this.flowLayoutPanel.ResumeLayout(false);
            this.headerPanel.ResumeLayout(false);
            this.headerPanel.PerformLayout();
            this.middlePanel.ResumeLayout(false);
            this.middlePanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel;
        private System.Windows.Forms.Panel headerPanel;
        private System.Windows.Forms.TextBox roleDescTextBox;
        private System.Windows.Forms.TextBox roleIDTextBox;
        private System.Windows.Forms.Label roleIDLabel;
        private System.Windows.Forms.Label descLabel;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Panel middlePanel;
        private System.Windows.Forms.ListView availablePermissionListView;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button leftButton;
        private System.Windows.Forms.Button rightButton;
        private System.Windows.Forms.Button downButton;
        private System.Windows.Forms.Button upButton;
        private System.Windows.Forms.ColumnHeader columnAvailablePermId;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListView assignedPermListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
    }
}