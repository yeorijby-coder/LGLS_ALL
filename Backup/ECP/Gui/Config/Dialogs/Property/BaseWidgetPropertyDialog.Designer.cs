namespace ECP.Gui.Config.Dialogs.Property
{
    partial class BaseWidgetPropertyDialog
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
            this.colorDialogTextColor = new System.Windows.Forms.ColorDialog();
            this.colorDialogBGColor = new System.Windows.Forms.ColorDialog();
            this.fontDialog = new System.Windows.Forms.FontDialog();
            this.colorDialogFGColor = new System.Windows.Forms.ColorDialog();
            this.imageChooser = new System.Windows.Forms.OpenFileDialog();
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.panelMain = new System.Windows.Forms.Panel();
            this.comboBoxAutoSize = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.numericUpDownTextCornerMargin = new System.Windows.Forms.NumericUpDown();
            this.comboBoxTextAlign = new System.Windows.Forms.ComboBox();
            this.comboBoxImageLayout = new System.Windows.Forms.ComboBox();
            this.comboBoxBorderStyle = new System.Windows.Forms.ComboBox();
            this.ChooserFontColor = new System.Windows.Forms.Button();
            this.chooserFGColor = new System.Windows.Forms.Button();
            this.chooserImage = new System.Windows.Forms.Button();
            this.chooserBGColor = new System.Windows.Forms.Button();
            this.chooserFont = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxImage = new System.Windows.Forms.TextBox();
            this.textBoxFGColor = new System.Windows.Forms.TextBox();
            this.textBoxBGColor = new System.Windows.Forms.TextBox();
            this.textBoxTextColor = new System.Windows.Forms.TextBox();
            this.textBoxFont = new System.Windows.Forms.TextBox();
            this.textBoxTextContent = new System.Windows.Forms.TextBox();
            this.flowLayoutPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonApply = new System.Windows.Forms.Button();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            this.panelMain.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTextCornerMargin)).BeginInit();
            this.flowLayoutPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // imageChooser
            // 
            this.imageChooser.DefaultExt = "bmp";
            this.imageChooser.Filter = "Bitmap 파일 (*.bmp)|*.bmp|Jpeg 파일 (Jpeg.*)|*.jpg|모든 파일(*.*)|*.*";
            // 
            // splitContainer
            // 
            this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer.Location = new System.Drawing.Point(0, 0);
            this.splitContainer.Name = "splitContainer";
            this.splitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.Controls.Add(this.panelMain);
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.Controls.Add(this.flowLayoutPanel);
            this.splitContainer.Size = new System.Drawing.Size(460, 404);
            this.splitContainer.SplitterDistance = 367;
            this.splitContainer.TabIndex = 0;
            // 
            // panelMain
            // 
            this.panelMain.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelMain.Controls.Add(this.comboBoxAutoSize);
            this.panelMain.Controls.Add(this.label11);
            this.panelMain.Controls.Add(this.numericUpDownTextCornerMargin);
            this.panelMain.Controls.Add(this.comboBoxTextAlign);
            this.panelMain.Controls.Add(this.comboBoxImageLayout);
            this.panelMain.Controls.Add(this.comboBoxBorderStyle);
            this.panelMain.Controls.Add(this.ChooserFontColor);
            this.panelMain.Controls.Add(this.chooserFGColor);
            this.panelMain.Controls.Add(this.chooserImage);
            this.panelMain.Controls.Add(this.chooserBGColor);
            this.panelMain.Controls.Add(this.chooserFont);
            this.panelMain.Controls.Add(this.label9);
            this.panelMain.Controls.Add(this.label2);
            this.panelMain.Controls.Add(this.label6);
            this.panelMain.Controls.Add(this.label7);
            this.panelMain.Controls.Add(this.label10);
            this.panelMain.Controls.Add(this.label8);
            this.panelMain.Controls.Add(this.label5);
            this.panelMain.Controls.Add(this.label3);
            this.panelMain.Controls.Add(this.label4);
            this.panelMain.Controls.Add(this.label1);
            this.panelMain.Controls.Add(this.textBoxImage);
            this.panelMain.Controls.Add(this.textBoxFGColor);
            this.panelMain.Controls.Add(this.textBoxBGColor);
            this.panelMain.Controls.Add(this.textBoxTextColor);
            this.panelMain.Controls.Add(this.textBoxFont);
            this.panelMain.Controls.Add(this.textBoxTextContent);
            this.panelMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMain.Location = new System.Drawing.Point(0, 0);
            this.panelMain.Name = "panelMain";
            this.panelMain.Size = new System.Drawing.Size(460, 367);
            this.panelMain.TabIndex = 0;
            // 
            // comboBoxAutoSize
            // 
            this.comboBoxAutoSize.FormattingEnabled = true;
            this.comboBoxAutoSize.Items.AddRange(new object[] {
            "True",
            "False"});
            this.comboBoxAutoSize.Location = new System.Drawing.Point(164, 126);
            this.comboBoxAutoSize.Name = "comboBoxAutoSize";
            this.comboBoxAutoSize.Size = new System.Drawing.Size(170, 20);
            this.comboBoxAutoSize.TabIndex = 3;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(100, 129);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(59, 12);
            this.label11.TabIndex = 17;
            this.label11.Text = "Auto Size";
            // 
            // numericUpDownTextCornerMargin
            // 
            this.numericUpDownTextCornerMargin.Location = new System.Drawing.Point(166, 181);
            this.numericUpDownTextCornerMargin.Name = "numericUpDownTextCornerMargin";
            this.numericUpDownTextCornerMargin.Size = new System.Drawing.Size(51, 21);
            this.numericUpDownTextCornerMargin.TabIndex = 5;
            this.numericUpDownTextCornerMargin.ValueChanged += new System.EventHandler(this.numericUpDownTextCornerMargin_ValueChanged);
            // 
            // comboBoxTextAlign
            // 
            this.comboBoxTextAlign.FormattingEnabled = true;
            this.comboBoxTextAlign.Items.AddRange(new object[] {
            "BottomCenter",
            "BottomLeft",
            "BottomRight",
            "MiddleCenter",
            "MiddleLeft",
            "MiddleRight",
            "TopCenter",
            "TopLeft",
            "TopRight"});
            this.comboBoxTextAlign.Location = new System.Drawing.Point(164, 153);
            this.comboBoxTextAlign.Name = "comboBoxTextAlign";
            this.comboBoxTextAlign.Size = new System.Drawing.Size(170, 20);
            this.comboBoxTextAlign.TabIndex = 4;
            // 
            // comboBoxImageLayout
            // 
            this.comboBoxImageLayout.FormattingEnabled = true;
            this.comboBoxImageLayout.Items.AddRange(new object[] {
            "Center",
            "None",
            "Stretch",
            "Tile",
            "Zoom"});
            this.comboBoxImageLayout.Location = new System.Drawing.Point(164, 332);
            this.comboBoxImageLayout.Name = "comboBoxImageLayout";
            this.comboBoxImageLayout.Size = new System.Drawing.Size(170, 20);
            this.comboBoxImageLayout.TabIndex = 11;
            // 
            // comboBoxBorderStyle
            // 
            this.comboBoxBorderStyle.FormattingEnabled = true;
            this.comboBoxBorderStyle.Items.AddRange(new object[] {
            "Fixed3D",
            "FixedSingle",
            "None"});
            this.comboBoxBorderStyle.Location = new System.Drawing.Point(164, 272);
            this.comboBoxBorderStyle.Name = "comboBoxBorderStyle";
            this.comboBoxBorderStyle.Size = new System.Drawing.Size(170, 20);
            this.comboBoxBorderStyle.TabIndex = 8;
            // 
            // ChooserFontColor
            // 
            this.ChooserFontColor.Location = new System.Drawing.Point(340, 94);
            this.ChooserFontColor.Name = "ChooserFontColor";
            this.ChooserFontColor.Size = new System.Drawing.Size(25, 23);
            this.ChooserFontColor.TabIndex = 2;
            this.ChooserFontColor.Text = "...";
            this.ChooserFontColor.UseVisualStyleBackColor = true;
            this.ChooserFontColor.Click += new System.EventHandler(this.ChooserFontColor_Click);
            // 
            // chooserFGColor
            // 
            this.chooserFGColor.Location = new System.Drawing.Point(340, 210);
            this.chooserFGColor.Name = "chooserFGColor";
            this.chooserFGColor.Size = new System.Drawing.Size(25, 23);
            this.chooserFGColor.TabIndex = 6;
            this.chooserFGColor.Text = "...";
            this.chooserFGColor.UseVisualStyleBackColor = true;
            this.chooserFGColor.Click += new System.EventHandler(this.chooserFGColor_Click);
            // 
            // chooserImage
            // 
            this.chooserImage.Location = new System.Drawing.Point(340, 299);
            this.chooserImage.Name = "chooserImage";
            this.chooserImage.Size = new System.Drawing.Size(25, 23);
            this.chooserImage.TabIndex = 9;
            this.chooserImage.Text = "...";
            this.chooserImage.UseVisualStyleBackColor = true;
            // 
            // chooserBGColor
            // 
            this.chooserBGColor.Location = new System.Drawing.Point(340, 243);
            this.chooserBGColor.Name = "chooserBGColor";
            this.chooserBGColor.Size = new System.Drawing.Size(25, 23);
            this.chooserBGColor.TabIndex = 7;
            this.chooserBGColor.Text = "...";
            this.chooserBGColor.UseVisualStyleBackColor = true;
            this.chooserBGColor.Click += new System.EventHandler(this.chooserBGColor_Click);
            // 
            // chooserFont
            // 
            this.chooserFont.Location = new System.Drawing.Point(340, 65);
            this.chooserFont.Name = "chooserFont";
            this.chooserFont.Size = new System.Drawing.Size(25, 23);
            this.chooserFont.TabIndex = 1;
            this.chooserFont.Text = "...";
            this.chooserFont.UseVisualStyleBackColor = true;
            this.chooserFont.Click += new System.EventHandler(this.chooserFont_Click);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(97, 158);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(62, 12);
            this.label9.TabIndex = 10;
            this.label9.Text = "Text Align";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(95, 100);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 12);
            this.label2.TabIndex = 10;
            this.label2.Text = "Text Color";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(50, 216);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(109, 12);
            this.label6.TabIndex = 11;
            this.label6.Text = "Fore Ground Color";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(82, 276);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(74, 12);
            this.label7.TabIndex = 11;
            this.label7.Text = "Border Style";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(77, 335);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(82, 12);
            this.label10.TabIndex = 11;
            this.label10.Text = "Image Layout";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(119, 304);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(40, 12);
            this.label8.TabIndex = 11;
            this.label8.Text = "Image";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(47, 246);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(112, 12);
            this.label5.TabIndex = 11;
            this.label5.Text = "Back Ground Color";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(44, 185);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(115, 12);
            this.label3.TabIndex = 12;
            this.label3.Text = "Text Corner Margin";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(129, 40);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(30, 12);
            this.label4.TabIndex = 9;
            this.label4.Text = "Text";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(101, 70);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 12);
            this.label1.TabIndex = 8;
            this.label1.Text = "Text Font";
            // 
            // textBoxImage
            // 
            this.textBoxImage.Location = new System.Drawing.Point(164, 300);
            this.textBoxImage.Name = "textBoxImage";
            this.textBoxImage.Size = new System.Drawing.Size(170, 21);
            this.textBoxImage.TabIndex = 10;
            // 
            // textBoxFGColor
            // 
            this.textBoxFGColor.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.textBoxFGColor.Enabled = false;
            this.textBoxFGColor.ForeColor = System.Drawing.SystemColors.MenuText;
            this.textBoxFGColor.Location = new System.Drawing.Point(164, 210);
            this.textBoxFGColor.Multiline = true;
            this.textBoxFGColor.Name = "textBoxFGColor";
            this.textBoxFGColor.Size = new System.Drawing.Size(170, 23);
            this.textBoxFGColor.TabIndex = 5;
            // 
            // textBoxBGColor
            // 
            this.textBoxBGColor.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.textBoxBGColor.Enabled = false;
            this.textBoxBGColor.ForeColor = System.Drawing.SystemColors.MenuText;
            this.textBoxBGColor.Location = new System.Drawing.Point(164, 241);
            this.textBoxBGColor.Multiline = true;
            this.textBoxBGColor.Name = "textBoxBGColor";
            this.textBoxBGColor.Size = new System.Drawing.Size(170, 23);
            this.textBoxBGColor.TabIndex = 6;
            // 
            // textBoxTextColor
            // 
            this.textBoxTextColor.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.textBoxTextColor.Enabled = false;
            this.textBoxTextColor.Location = new System.Drawing.Point(164, 94);
            this.textBoxTextColor.Multiline = true;
            this.textBoxTextColor.Name = "textBoxTextColor";
            this.textBoxTextColor.Size = new System.Drawing.Size(170, 23);
            this.textBoxTextColor.TabIndex = 2;
            // 
            // textBoxFont
            // 
            this.textBoxFont.Location = new System.Drawing.Point(164, 65);
            this.textBoxFont.Name = "textBoxFont";
            this.textBoxFont.Size = new System.Drawing.Size(170, 21);
            this.textBoxFont.TabIndex = 1;
            // 
            // textBoxTextContent
            // 
            this.textBoxTextContent.Location = new System.Drawing.Point(164, 36);
            this.textBoxTextContent.Name = "textBoxTextContent";
            this.textBoxTextContent.Size = new System.Drawing.Size(170, 21);
            this.textBoxTextContent.TabIndex = 0;
            this.textBoxTextContent.KeyUp += new System.Windows.Forms.KeyEventHandler(this.textBoxTextContent_KeyUp);
            // 
            // flowLayoutPanel
            // 
            this.flowLayoutPanel.BackColor = System.Drawing.SystemColors.ControlDark;
            this.flowLayoutPanel.Controls.Add(this.buttonCancel);
            this.flowLayoutPanel.Controls.Add(this.buttonOK);
            this.flowLayoutPanel.Controls.Add(this.buttonApply);
            this.flowLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPanel.Name = "flowLayoutPanel";
            this.flowLayoutPanel.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.flowLayoutPanel.Size = new System.Drawing.Size(460, 33);
            this.flowLayoutPanel.TabIndex = 0;
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(382, 3);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 1;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(301, 3);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 0;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // buttonApply
            // 
            this.buttonApply.Location = new System.Drawing.Point(220, 3);
            this.buttonApply.Name = "buttonApply";
            this.buttonApply.Size = new System.Drawing.Size(75, 23);
            this.buttonApply.TabIndex = 2;
            this.buttonApply.Text = "Apply";
            this.buttonApply.UseVisualStyleBackColor = true;
            this.buttonApply.Click += new System.EventHandler(this.buttonApply_Click);
            // 
            // BaseWidgetPropertyDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(460, 404);
            this.Controls.Add(this.splitContainer);
            this.Name = "BaseWidgetPropertyDialog";
            this.Text = "BaseWidget Property";
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel2.ResumeLayout(false);
            this.splitContainer.ResumeLayout(false);
            this.panelMain.ResumeLayout(false);
            this.panelMain.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTextCornerMargin)).EndInit();
            this.flowLayoutPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ColorDialog colorDialogTextColor;
        private System.Windows.Forms.ColorDialog colorDialogBGColor;
        private System.Windows.Forms.FontDialog fontDialog;
        private System.Windows.Forms.ColorDialog colorDialogFGColor;
        private System.Windows.Forms.OpenFileDialog imageChooser;
        private System.Windows.Forms.SplitContainer splitContainer;
        public System.Windows.Forms.Panel panelMain;
        private System.Windows.Forms.NumericUpDown numericUpDownTextCornerMargin;
        private System.Windows.Forms.ComboBox comboBoxTextAlign;
        private System.Windows.Forms.ComboBox comboBoxImageLayout;
        private System.Windows.Forms.ComboBox comboBoxBorderStyle;
        private System.Windows.Forms.Button ChooserFontColor;
        private System.Windows.Forms.Button chooserFGColor;
        private System.Windows.Forms.Button chooserImage;
        private System.Windows.Forms.Button chooserBGColor;
        private System.Windows.Forms.Button chooserFont;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxImage;
        private System.Windows.Forms.TextBox textBoxFGColor;
        private System.Windows.Forms.TextBox textBoxBGColor;
        private System.Windows.Forms.TextBox textBoxTextColor;
        private System.Windows.Forms.TextBox textBoxFont;
        private System.Windows.Forms.TextBox textBoxTextContent;
        protected System.Windows.Forms.FlowLayoutPanel flowLayoutPanel;
        protected System.Windows.Forms.Button buttonCancel;
        protected System.Windows.Forms.Button buttonOK;
        protected System.Windows.Forms.Button buttonApply;
        private System.Windows.Forms.ComboBox comboBoxAutoSize;
        private System.Windows.Forms.Label label11;
    }
}