namespace HECS.Gui.Monitor.Forms
{
    partial class StockMainForm
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
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBoxChamfer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(857, 0);
            // 
            // labelGradient1
            // 
            this.labelGradient1.Size = new System.Drawing.Size(569, 30);
            // 
            // panelContainer
            // 
            this.panelContainer.AutoScroll = false;
            this.panelContainer.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelContainer.Size = new System.Drawing.Size(885, 538);
            // 
            // groupBoxChamfer1
            // 
            this.groupBoxChamfer1.Size = new System.Drawing.Size(904, 569);
            // 
            // panelNavigation
            // 
            this.panelNavigation.Location = new System.Drawing.Point(10, 522);
            this.panelNavigation.Size = new System.Drawing.Size(885, 37);
            // 
            // StockMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(908, 607);
            this.Name = "StockMain";
            this.Text = "StockMain";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBoxChamfer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

    }
}