namespace HECS.Gui.Monitor.Forms
{
    partial class MonitorMain
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MonitorMain));
            this.imageListButton = new System.Windows.Forms.ImageList(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBoxChamfer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelGradient3
            // 
            this.labelGradient3.Text = "작업자 - Admin";
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(965, 0);
            // 
            // labelGradient1
            // 
            this.labelGradient1.Size = new System.Drawing.Size(677, 30);
            this.labelGradient1.Text = "장비 모니터링";
            // 
            // panelContainer
            // 
            this.panelContainer.Size = new System.Drawing.Size(993, 632);
            // 
            // groupBoxChamfer1
            // 
            this.groupBoxChamfer1.Size = new System.Drawing.Size(1012, 673);
            this.groupBoxChamfer1.Text = "반송현황 모니터링";
            // 
            // panelNavigation
            // 
            this.panelNavigation.Location = new System.Drawing.Point(10, 653);
            this.panelNavigation.Size = new System.Drawing.Size(993, 10);
            // 
            // imageListButton
            // 
            this.imageListButton.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListButton.ImageStream")));
            this.imageListButton.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListButton.Images.SetKeyName(0, "Main");
            this.imageListButton.Images.SetKeyName(1, "HiRack");
            this.imageListButton.Images.SetKeyName(2, "Conveyor");
            this.imageListButton.Images.SetKeyName(3, "AGV");
            this.imageListButton.Images.SetKeyName(4, "RGV");
            this.imageListButton.Images.SetKeyName(5, "Transfer");
            this.imageListButton.Images.SetKeyName(6, "Material");
            // 
            // MonitorMain
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(1016, 711);
            this.DoubleBuffered = false;
            this.Name = "MonitorMain";
            this.Text = "장비모니터링";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBoxChamfer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ImageList imageListButton;


    }
}