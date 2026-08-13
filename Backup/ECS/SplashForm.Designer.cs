namespace HECS
{
    partial class SplashForm
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panelDraw = new System.Windows.Forms.Panel();
            this.panel4 = new System.Windows.Forms.Panel();
            this.richTextProgress = new System.Windows.Forms.RichTextBox();
            this.labelBorder2 = new HECS.Gui.Jaeit.Labels.LabelBorder();
            this.labelBorder1 = new HECS.Gui.Jaeit.Labels.LabelBorder();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panelDraw.SuspendLayout();
            this.panel4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.Window;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(1, 1);
            this.panel1.Name = "panel1";
            this.panel1.Padding = new System.Windows.Forms.Padding(3);
            this.panel1.Size = new System.Drawing.Size(726, 467);
            this.panel1.TabIndex = 0;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.White;
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.panelDraw);
            this.panel2.Controls.Add(this.labelBorder1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(3, 3);
            this.panel2.Margin = new System.Windows.Forms.Padding(0);
            this.panel2.Name = "panel2";
            this.panel2.Padding = new System.Windows.Forms.Padding(3);
            this.panel2.Size = new System.Drawing.Size(718, 459);
            this.panel2.TabIndex = 1;
            // 
            // panelDraw
            // 
            this.panelDraw.Controls.Add(this.panel4);
            this.panelDraw.Controls.Add(this.pictureBox1);
            this.panelDraw.Controls.Add(this.labelBorder2);
            this.panelDraw.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelDraw.Location = new System.Drawing.Point(3, 57);
            this.panelDraw.Name = "panelDraw";
            this.panelDraw.Size = new System.Drawing.Size(710, 397);
            this.panelDraw.TabIndex = 0;
            // 
            // panel4
            // 
            this.panel4.AutoScroll = true;
            this.panel4.Controls.Add(this.richTextProgress);
            this.panel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel4.Location = new System.Drawing.Point(123, 30);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(587, 367);
            this.panel4.TabIndex = 7;
            // 
            // richTextProgress
            // 
            this.richTextProgress.BackColor = System.Drawing.Color.White;
            this.richTextProgress.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.richTextProgress.Cursor = System.Windows.Forms.Cursors.Hand;
            this.richTextProgress.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextProgress.Font = new System.Drawing.Font("DotumChe", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.richTextProgress.ForeColor = System.Drawing.Color.Black;
            this.richTextProgress.Location = new System.Drawing.Point(0, 0);
            this.richTextProgress.Name = "richTextProgress";
            this.richTextProgress.ReadOnly = true;
            this.richTextProgress.Size = new System.Drawing.Size(587, 367);
            this.richTextProgress.TabIndex = 0;
            this.richTextProgress.TabStop = false;
            this.richTextProgress.Text = "";
            // 
            // labelBorder2
            // 
            this.labelBorder2.BackColor = System.Drawing.Color.White;
            this.labelBorder2.BorderColor = System.Drawing.Color.DarkGreen;
            this.labelBorder2.BorderSize = 0.01F;
            this.labelBorder2.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelBorder2.Font = new System.Drawing.Font("GulimChe", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelBorder2.ForeColor = System.Drawing.Color.DodgerBlue;
            this.labelBorder2.Image = global::HECS.Properties.Resources.topLogo;
            this.labelBorder2.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.labelBorder2.Location = new System.Drawing.Point(0, 0);
            this.labelBorder2.Name = "labelBorder2";
            this.labelBorder2.Size = new System.Drawing.Size(710, 30);
            this.labelBorder2.TabIndex = 9;
            this.labelBorder2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // labelBorder1
            // 
            this.labelBorder1.BackColor = System.Drawing.Color.RoyalBlue;
            this.labelBorder1.BorderColor = System.Drawing.Color.Navy;
            this.labelBorder1.BorderSize = 3F;
            this.labelBorder1.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelBorder1.Font = new System.Drawing.Font("Century Gothic", 27.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelBorder1.ForeColor = System.Drawing.Color.White;
            this.labelBorder1.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.labelBorder1.Location = new System.Drawing.Point(3, 3);
            this.labelBorder1.Name = "labelBorder1";
            this.labelBorder1.Size = new System.Drawing.Size(710, 54);
            this.labelBorder1.TabIndex = 1;
            this.labelBorder1.Text = "ezMaterial Control System";
            this.labelBorder1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Left;
            this.pictureBox1.Image = global::HECS.Properties.Resources.LGLS;
            this.pictureBox1.Location = new System.Drawing.Point(0, 30);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(123, 367);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 8;
            this.pictureBox1.TabStop = false;
            // 
            // SplashForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.Gray;
            this.ClientSize = new System.Drawing.Size(728, 469);
            this.Controls.Add(this.panel1);
            this.DoubleBuffered = true;
            this.ForeColor = System.Drawing.Color.Gray;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "SplashForm";
            this.Padding = new System.Windows.Forms.Padding(1);
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Stocker Controller Loader";
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panelDraw.ResumeLayout(false);
            this.panel4.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panelDraw;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.RichTextBox richTextProgress;
        private System.Windows.Forms.PictureBox pictureBox1;
        private HECS.Gui.Jaeit.Labels.LabelBorder labelBorder1;
        private HECS.Gui.Jaeit.Labels.LabelBorder labelBorder2;
    }
}