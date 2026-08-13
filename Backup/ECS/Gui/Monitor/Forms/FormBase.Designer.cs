namespace HECS.Gui.Monitor.Forms
{
    partial class FormBase
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
            this.panelTitle = new System.Windows.Forms.Panel();
            this.labelGradient1 = new HECS.Gui.Jaeit.Labels.LabelGradient();
            this.labelGradient3 = new HECS.Gui.Jaeit.Labels.LabelGradient();
            this.labelGradient2 = new HECS.Gui.Jaeit.Labels.LabelGradient();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.panelMain = new System.Windows.Forms.Panel();
            this.groupBoxChamfer1 = new HECS.Gui.Jaeit.Panels.GroupBox.GroupBoxChamfer();
            this.panelContainer = new System.Windows.Forms.Panel();
            this.panelNavigation = new System.Windows.Forms.Panel();
            this.panelTitle.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.panelMain.SuspendLayout();
            this.groupBoxChamfer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelTitle
            // 
            this.panelTitle.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panelTitle.Controls.Add(this.labelGradient1);
            this.panelTitle.Controls.Add(this.labelGradient3);
            this.panelTitle.Controls.Add(this.labelGradient2);
            this.panelTitle.Controls.Add(this.pictureBox1);
            this.panelTitle.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelTitle.Location = new System.Drawing.Point(0, 0);
            this.panelTitle.Name = "panelTitle";
            this.panelTitle.Size = new System.Drawing.Size(970, 34);
            this.panelTitle.TabIndex = 0;
            this.panelTitle.Visible = false;
            // 
            // labelGradient1
            // 
            this.labelGradient1.BorderStyle = System.Windows.Forms.Border3DStyle.RaisedOuter;
            this.labelGradient1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.labelGradient1.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelGradient1.ForeColor = System.Drawing.Color.MediumBlue;
            this.labelGradient1.GradientColorOne = System.Drawing.Color.LightSteelBlue;
            this.labelGradient1.GradientColorTwo = System.Drawing.Color.DodgerBlue;
            this.labelGradient1.Location = new System.Drawing.Point(288, 0);
            this.labelGradient1.Name = "labelGradient1";
            this.labelGradient1.Size = new System.Drawing.Size(631, 30);
            this.labelGradient1.TabIndex = 1;
            this.labelGradient1.Text = "Title";
            this.labelGradient1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelGradient3
            // 
            this.labelGradient3.Dock = System.Windows.Forms.DockStyle.Left;
            this.labelGradient3.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelGradient3.ForeColor = System.Drawing.Color.White;
            this.labelGradient3.GradientColorOne = System.Drawing.Color.RoyalBlue;
            this.labelGradient3.GradientColorTwo = System.Drawing.Color.RoyalBlue;
            this.labelGradient3.Location = new System.Drawing.Point(135, 0);
            this.labelGradient3.Name = "labelGradient3";
            this.labelGradient3.Size = new System.Drawing.Size(153, 30);
            this.labelGradient3.TabIndex = 3;
            this.labelGradient3.Text = "Worker";
            this.labelGradient3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelGradient2
            // 
            this.labelGradient2.BorderStyle = System.Windows.Forms.Border3DStyle.Raised;
            this.labelGradient2.Dock = System.Windows.Forms.DockStyle.Left;
            this.labelGradient2.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelGradient2.GradientColorOne = System.Drawing.Color.Lime;
            this.labelGradient2.GradientColorTwo = System.Drawing.Color.Lime;
            this.labelGradient2.Location = new System.Drawing.Point(0, 0);
            this.labelGradient2.Name = "labelGradient2";
            this.labelGradient2.Size = new System.Drawing.Size(135, 30);
            this.labelGradient2.TabIndex = 2;
            this.labelGradient2.Text = "Normal";
            this.labelGradient2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.White;
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Right;
            this.pictureBox1.Image = global::HECS.Properties.Resources.Handok_logo;
            this.pictureBox1.Location = new System.Drawing.Point(919, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(47, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // panelMain
            // 
            this.panelMain.BackColor = System.Drawing.Color.LemonChiffon;
            this.panelMain.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panelMain.Controls.Add(this.groupBoxChamfer1);
            this.panelMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMain.Location = new System.Drawing.Point(0, 34);
            this.panelMain.Name = "panelMain";
            this.panelMain.Size = new System.Drawing.Size(970, 624);
            this.panelMain.TabIndex = 1;
            // 
            // groupBoxChamfer1
            // 
            this.groupBoxChamfer1.BorderWidth = 6;
            this.groupBoxChamfer1.ColorScheme = HECS.Gui.Jaeit.Panels.GroupBox.EnmColorScheme.Green;
            this.groupBoxChamfer1.Controls.Add(this.panelContainer);
            this.groupBoxChamfer1.Controls.Add(this.panelNavigation);
            this.groupBoxChamfer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBoxChamfer1.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBoxChamfer1.FontColor = System.Drawing.Color.FromArgb(((int)(((byte)(57)))), ((int)(((byte)(66)))), ((int)(((byte)(1)))));
            this.groupBoxChamfer1.Location = new System.Drawing.Point(0, 0);
            this.groupBoxChamfer1.Name = "groupBoxChamfer1";
            this.groupBoxChamfer1.Padding = new System.Windows.Forms.Padding(10, 6, 9, 10);
            this.groupBoxChamfer1.ShadowColor = System.Drawing.Color.FromArgb(((int)(((byte)(142)))), ((int)(((byte)(143)))), ((int)(((byte)(116)))));
            this.groupBoxChamfer1.Size = new System.Drawing.Size(966, 620);
            this.groupBoxChamfer1.TabIndex = 0;
            this.groupBoxChamfer1.TabStop = false;
            this.groupBoxChamfer1.Text = "[NO_TITLE]";
            // 
            // panelContainer
            // 
            this.panelContainer.AutoScroll = true;
            this.panelContainer.BackColor = System.Drawing.Color.LemonChiffon;
            this.panelContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelContainer.Location = new System.Drawing.Point(10, 21);
            this.panelContainer.Name = "panelContainer";
            this.panelContainer.Size = new System.Drawing.Size(947, 552);
            this.panelContainer.TabIndex = 2;
            // 
            // panelNavigation
            // 
            this.panelNavigation.BackColor = System.Drawing.Color.LemonChiffon;
            this.panelNavigation.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelNavigation.Location = new System.Drawing.Point(10, 573);
            this.panelNavigation.Name = "panelNavigation";
            this.panelNavigation.Size = new System.Drawing.Size(947, 37);
            this.panelNavigation.TabIndex = 3;
            // 
            // FormBase
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(970, 658);
            this.ControlBox = false;
            this.Controls.Add(this.panelMain);
            this.Controls.Add(this.panelTitle);
            this.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormBase";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FormBase";
            this.TopMost = true;
            this.panelTitle.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.panelMain.ResumeLayout(false);
            this.groupBoxChamfer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        protected HECS.Gui.Jaeit.Labels.LabelGradient labelGradient2;
        protected HECS.Gui.Jaeit.Labels.LabelGradient labelGradient3;
        protected System.Windows.Forms.PictureBox pictureBox1;
        protected HECS.Gui.Jaeit.Labels.LabelGradient labelGradient1;
        protected System.Windows.Forms.Panel panelContainer;
        protected HECS.Gui.Jaeit.Panels.GroupBox.GroupBoxChamfer groupBoxChamfer1;
        private System.Windows.Forms.Panel panelMain;
        private System.Windows.Forms.Panel panelTitle;
        protected System.Windows.Forms.Panel panelNavigation;

    }
}