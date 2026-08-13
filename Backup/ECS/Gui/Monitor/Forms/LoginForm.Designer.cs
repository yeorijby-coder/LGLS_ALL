namespace HECS.Gui.Monitor.Forms
{
    partial class LoginForm
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
            this.webBrowser1 = new System.Windows.Forms.WebBrowser();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.panelContainer.SuspendLayout();
            this.groupBoxChamfer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelGradient3
            // 
            this.labelGradient3.Text = "Admin";
            // 
            // labelGradient1
            // 
            this.labelGradient1.Text = "Login";
            // 
            // panelContainer
            // 
            this.panelContainer.Controls.Add(this.webBrowser1);
            // 
            // groupBoxChamfer1
            // 
            this.groupBoxChamfer1.Text = "Login";
            // 
            // panelNavigation
            // 
            this.panelNavigation.Visible = false;
            // 
            // webBrowser1
            // 
            this.webBrowser1.Dock = System.Windows.Forms.DockStyle.Top;
            this.webBrowser1.Location = new System.Drawing.Point(0, 0);
            this.webBrowser1.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser1.Name = "webBrowser1";
            this.webBrowser1.Size = new System.Drawing.Size(947, 420);
            this.webBrowser1.TabIndex = 0;
            this.webBrowser1.Url = new System.Uri("D:\\ezMCS\\main.swf", System.UriKind.Absolute);
            // 
            // LoginForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(970, 658);
            this.DoubleBuffered = false;
            this.Name = "LoginForm";
            this.Text = "사용자 로그인";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.panelContainer.ResumeLayout(false);
            this.groupBoxChamfer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.WebBrowser webBrowser1;



    }
}