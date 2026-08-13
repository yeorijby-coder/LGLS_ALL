namespace ECP.Gui.Config.Dialogs.Property
{
    partial class TextPropertyDialog
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
            this.buttonPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.cancelButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.mainPanel = new System.Windows.Forms.Panel();
            this.fontColorChooser = new System.Windows.Forms.Button();
            this.BGColorChooser = new System.Windows.Forms.Button();
            this.fontChooser = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.cornerMarginTextBox = new System.Windows.Forms.TextBox();
            this.bgColorTextBox = new System.Windows.Forms.TextBox();
            this.fontColorTextBox = new System.Windows.Forms.TextBox();
            this.textFontTextBox = new System.Windows.Forms.TextBox();
            this.textContentTextBox = new System.Windows.Forms.TextBox();
            this.fontDialog = new System.Windows.Forms.FontDialog();
            this.textColorDialog = new System.Windows.Forms.ColorDialog();
            this.bgColorDialog = new System.Windows.Forms.ColorDialog();
            this.fontColorDialog = new System.Windows.Forms.ColorDialog();
            this.tableLayoutPanel.SuspendLayout();
            this.buttonPanel.SuspendLayout();
            this.mainPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel
            // 
            this.tableLayoutPanel.ColumnCount = 1;
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel.Controls.Add(this.buttonPanel, 0, 1);
            this.tableLayoutPanel.Controls.Add(this.mainPanel, 0, 0);
            this.tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel.Name = "tableLayoutPanel";
            this.tableLayoutPanel.RowCount = 2;
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 83.47108F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.52892F));
            this.tableLayoutPanel.Size = new System.Drawing.Size(403, 242);
            this.tableLayoutPanel.TabIndex = 0;
            // 
            // buttonPanel
            // 
            this.buttonPanel.BackColor = System.Drawing.SystemColors.ControlDark;
            this.buttonPanel.Controls.Add(this.cancelButton);
            this.buttonPanel.Controls.Add(this.okButton);
            this.buttonPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonPanel.Location = new System.Drawing.Point(3, 205);
            this.buttonPanel.Name = "buttonPanel";
            this.buttonPanel.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.buttonPanel.Size = new System.Drawing.Size(397, 34);
            this.buttonPanel.TabIndex = 0;
            // 
            // cancelButton
            // 
            this.cancelButton.BackColor = System.Drawing.SystemColors.ControlDark;
            this.cancelButton.Location = new System.Drawing.Point(319, 3);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 0;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = false;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // okButton
            // 
            this.okButton.BackColor = System.Drawing.SystemColors.ControlDark;
            this.okButton.Location = new System.Drawing.Point(238, 3);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 1;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = false;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // mainPanel
            // 
            this.mainPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.mainPanel.Controls.Add(this.fontColorChooser);
            this.mainPanel.Controls.Add(this.BGColorChooser);
            this.mainPanel.Controls.Add(this.fontChooser);
            this.mainPanel.Controls.Add(this.label2);
            this.mainPanel.Controls.Add(this.label5);
            this.mainPanel.Controls.Add(this.label3);
            this.mainPanel.Controls.Add(this.label4);
            this.mainPanel.Controls.Add(this.label1);
            this.mainPanel.Controls.Add(this.cornerMarginTextBox);
            this.mainPanel.Controls.Add(this.bgColorTextBox);
            this.mainPanel.Controls.Add(this.fontColorTextBox);
            this.mainPanel.Controls.Add(this.textFontTextBox);
            this.mainPanel.Controls.Add(this.textContentTextBox);
            this.mainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainPanel.Location = new System.Drawing.Point(3, 3);
            this.mainPanel.Name = "mainPanel";
            this.mainPanel.Size = new System.Drawing.Size(397, 196);
            this.mainPanel.TabIndex = 1;
            // 
            // fontColorChooser
            // 
            this.fontColorChooser.Location = new System.Drawing.Point(324, 85);
            this.fontColorChooser.Name = "fontColorChooser";
            this.fontColorChooser.Size = new System.Drawing.Size(25, 23);
            this.fontColorChooser.TabIndex = 2;
            this.fontColorChooser.Text = "...";
            this.fontColorChooser.UseVisualStyleBackColor = true;
            this.fontColorChooser.Click += new System.EventHandler(this.fontColorChooser_Click);
            // 
            // BGColorChooser
            // 
            this.BGColorChooser.Location = new System.Drawing.Point(324, 120);
            this.BGColorChooser.Name = "BGColorChooser";
            this.BGColorChooser.Size = new System.Drawing.Size(25, 23);
            this.BGColorChooser.TabIndex = 2;
            this.BGColorChooser.Text = "...";
            this.BGColorChooser.UseVisualStyleBackColor = true;
            this.BGColorChooser.Click += new System.EventHandler(this.BGColorChooser_Click);
            // 
            // fontChooser
            // 
            this.fontChooser.Location = new System.Drawing.Point(324, 50);
            this.fontChooser.Name = "fontChooser";
            this.fontChooser.Size = new System.Drawing.Size(25, 23);
            this.fontChooser.TabIndex = 2;
            this.fontChooser.Text = "...";
            this.fontChooser.UseVisualStyleBackColor = true;
            this.fontChooser.Click += new System.EventHandler(this.fontChooser_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(81, 90);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(63, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "Font Color";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(32, 124);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(112, 12);
            this.label5.TabIndex = 1;
            this.label5.Text = "Back Ground Color";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(58, 155);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(86, 12);
            this.label3.TabIndex = 1;
            this.label3.Text = "Corner Margin";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(114, 26);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(30, 12);
            this.label4.TabIndex = 1;
            this.label4.Text = "Text";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(86, 55);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "Text Font";
            // 
            // cornerMarginTextBox
            // 
            this.cornerMarginTextBox.Location = new System.Drawing.Point(148, 154);
            this.cornerMarginTextBox.Name = "cornerMarginTextBox";
            this.cornerMarginTextBox.Size = new System.Drawing.Size(170, 21);
            this.cornerMarginTextBox.TabIndex = 0;
            // 
            // bgColorTextBox
            // 
            this.bgColorTextBox.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.bgColorTextBox.Enabled = false;
            this.bgColorTextBox.ForeColor = System.Drawing.SystemColors.MenuText;
            this.bgColorTextBox.Location = new System.Drawing.Point(148, 119);
            this.bgColorTextBox.Multiline = true;
            this.bgColorTextBox.Name = "bgColorTextBox";
            this.bgColorTextBox.Size = new System.Drawing.Size(170, 23);
            this.bgColorTextBox.TabIndex = 0;
            // 
            // fontColorTextBox
            // 
            this.fontColorTextBox.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.fontColorTextBox.Enabled = false;
            this.fontColorTextBox.Location = new System.Drawing.Point(148, 85);
            this.fontColorTextBox.Multiline = true;
            this.fontColorTextBox.Name = "fontColorTextBox";
            this.fontColorTextBox.Size = new System.Drawing.Size(170, 23);
            this.fontColorTextBox.TabIndex = 0;
            // 
            // textFontTextBox
            // 
            this.textFontTextBox.Location = new System.Drawing.Point(148, 52);
            this.textFontTextBox.Multiline = true;
            this.textFontTextBox.Name = "textFontTextBox";
            this.textFontTextBox.Size = new System.Drawing.Size(170, 23);
            this.textFontTextBox.TabIndex = 0;
            // 
            // textContentTextBox
            // 
            this.textContentTextBox.Location = new System.Drawing.Point(148, 21);
            this.textContentTextBox.Name = "textContentTextBox";
            this.textContentTextBox.Size = new System.Drawing.Size(170, 21);
            this.textContentTextBox.TabIndex = 0;
            // 
            // TextPropertyDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(403, 242);
            this.Controls.Add(this.tableLayoutPanel);
            this.Name = "TextPropertyDialog";
            this.Text = "TextPropertyDialog";
            this.tableLayoutPanel.ResumeLayout(false);
            this.buttonPanel.ResumeLayout(false);
            this.mainPanel.ResumeLayout(false);
            this.mainPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
        private System.Windows.Forms.FlowLayoutPanel buttonPanel;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Panel mainPanel;
        private System.Windows.Forms.FontDialog fontDialog;
        private System.Windows.Forms.TextBox textFontTextBox;
        private System.Windows.Forms.TextBox textContentTextBox;
        private System.Windows.Forms.ColorDialog textColorDialog;
        private System.Windows.Forms.ColorDialog bgColorDialog;
        private System.Windows.Forms.Button BGColorChooser;
        private System.Windows.Forms.Button fontChooser;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox cornerMarginTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox bgColorTextBox;
        private System.Windows.Forms.TextBox fontColorTextBox;
        private System.Windows.Forms.Button fontColorChooser;
        private System.Windows.Forms.ColorDialog fontColorDialog;
    }
}