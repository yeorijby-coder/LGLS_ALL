namespace ECP.Gui.User
{
    partial class AddOrModifyPrivilegeForm
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
            this.tableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.flowLayoutPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.cancelButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.applyButton = new System.Windows.Forms.Button();
            this.mainPanel = new System.Windows.Forms.Panel();
            this.privilegeDescTextBox = new System.Windows.Forms.TextBox();
            this.privilegeTextBox = new System.Windows.Forms.TextBox();
            this.privilegeDescLabel = new System.Windows.Forms.Label();
            this.labelPrivilegeIdLabel = new System.Windows.Forms.Label();
            this.tableLayoutPanel.SuspendLayout();
            this.flowLayoutPanel.SuspendLayout();
            this.mainPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel
            // 
            this.tableLayoutPanel.ColumnCount = 1;
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel.Controls.Add(this.flowLayoutPanel, 0, 1);
            this.tableLayoutPanel.Controls.Add(this.mainPanel, 0, 0);
            this.tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel.Name = "tableLayoutPanel";
            this.tableLayoutPanel.RowCount = 2;
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 73.94958F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 26.05042F));
            this.tableLayoutPanel.Size = new System.Drawing.Size(584, 163);
            this.tableLayoutPanel.TabIndex = 0;
            // 
            // flowLayoutPanel
            // 
            this.flowLayoutPanel.Controls.Add(this.cancelButton);
            this.flowLayoutPanel.Controls.Add(this.okButton);
            this.flowLayoutPanel.Controls.Add(this.applyButton);
            this.flowLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel.Location = new System.Drawing.Point(3, 123);
            this.flowLayoutPanel.Name = "flowLayoutPanel";
            this.flowLayoutPanel.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.flowLayoutPanel.Size = new System.Drawing.Size(578, 37);
            this.flowLayoutPanel.TabIndex = 0;
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(500, 3);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 5;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(419, 3);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 4;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // applyButton
            // 
            this.applyButton.Location = new System.Drawing.Point(338, 3);
            this.applyButton.Name = "applyButton";
            this.applyButton.Size = new System.Drawing.Size(75, 23);
            this.applyButton.TabIndex = 3;
            this.applyButton.Text = "Apply";
            this.applyButton.UseVisualStyleBackColor = true;
            this.applyButton.Click += new System.EventHandler(this.applyButton_Click);
            // 
            // mainPanel
            // 
            this.mainPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.mainPanel.Controls.Add(this.privilegeDescTextBox);
            this.mainPanel.Controls.Add(this.privilegeTextBox);
            this.mainPanel.Controls.Add(this.privilegeDescLabel);
            this.mainPanel.Controls.Add(this.labelPrivilegeIdLabel);
            this.mainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainPanel.Location = new System.Drawing.Point(3, 3);
            this.mainPanel.Name = "mainPanel";
            this.mainPanel.Size = new System.Drawing.Size(578, 114);
            this.mainPanel.TabIndex = 1;
            // 
            // privilegeDescTextBox
            // 
            this.privilegeDescTextBox.Location = new System.Drawing.Point(115, 46);
            this.privilegeDescTextBox.Multiline = true;
            this.privilegeDescTextBox.Name = "privilegeDescTextBox";
            this.privilegeDescTextBox.Size = new System.Drawing.Size(442, 51);
            this.privilegeDescTextBox.TabIndex = 2;
            // 
            // privilegeTextBox
            // 
            this.privilegeTextBox.Location = new System.Drawing.Point(115, 15);
            this.privilegeTextBox.Name = "privilegeTextBox";
            this.privilegeTextBox.Size = new System.Drawing.Size(442, 21);
            this.privilegeTextBox.TabIndex = 1;
            // 
            // privilegeDescLabel
            // 
            this.privilegeDescLabel.AutoSize = true;
            this.privilegeDescLabel.Location = new System.Drawing.Point(38, 49);
            this.privilegeDescLabel.Name = "privilegeDescLabel";
            this.privilegeDescLabel.Size = new System.Drawing.Size(68, 12);
            this.privilegeDescLabel.TabIndex = 1;
            this.privilegeDescLabel.Text = "Description";
            // 
            // labelPrivilegeIdLabel
            // 
            this.labelPrivilegeIdLabel.AutoSize = true;
            this.labelPrivilegeIdLabel.Location = new System.Drawing.Point(40, 25);
            this.labelPrivilegeIdLabel.Name = "labelPrivilegeIdLabel";
            this.labelPrivilegeIdLabel.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.labelPrivilegeIdLabel.Size = new System.Drawing.Size(68, 12);
            this.labelPrivilegeIdLabel.TabIndex = 0;
            this.labelPrivilegeIdLabel.Text = "Privilege ID";
            // 
            // AddOrModifyPrivilegeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(584, 163);
            this.Controls.Add(this.tableLayoutPanel);
            this.Name = "AddOrModifyPrivilegeForm";
            this.Text = "Add Privilege";
            this.tableLayoutPanel.ResumeLayout(false);
            this.flowLayoutPanel.ResumeLayout(false);
            this.mainPanel.ResumeLayout(false);
            this.mainPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel;
        private System.Windows.Forms.Panel mainPanel;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Label privilegeDescLabel;
        private System.Windows.Forms.Label labelPrivilegeIdLabel;
        private System.Windows.Forms.TextBox privilegeDescTextBox;
        private System.Windows.Forms.TextBox privilegeTextBox;
        private System.Windows.Forms.Button applyButton;
    }
}