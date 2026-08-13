namespace HECS.Gui.Monitor.Controls
{
    partial class LabelDualLine
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
            this.labelBottom = new System.Windows.Forms.Label();
            this.labelTop = new System.Windows.Forms.Label();
            this.panelContent.SuspendLayout();
            this.panelBack.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelContent
            // 
            this.panelContent.Controls.Add(this.labelBottom);
            this.panelContent.Controls.Add(this.labelTop);
            this.panelContent.Padding = new System.Windows.Forms.Padding(2);
            this.panelContent.Size = new System.Drawing.Size(243, 84);
            // 
            // panelBack
            // 
            this.panelBack.Size = new System.Drawing.Size(245, 86);
            // 
            // labelBottom
            // 
            this.labelBottom.AutoEllipsis = true;
            this.labelBottom.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelBottom.Location = new System.Drawing.Point(2, 46);
            this.labelBottom.Name = "labelBottom";
            this.labelBottom.Size = new System.Drawing.Size(239, 36);
            this.labelBottom.TabIndex = 7;
            this.labelBottom.Text = "Bottom Text";
            this.labelBottom.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.labelBottom.Click += new System.EventHandler(this.labelBottom_Click);
            // 
            // labelTop
            // 
            this.labelTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelTop.Location = new System.Drawing.Point(2, 2);
            this.labelTop.Name = "labelTop";
            this.labelTop.Size = new System.Drawing.Size(239, 44);
            this.labelTop.TabIndex = 6;
            this.labelTop.Text = "Top Text";
            this.labelTop.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.labelTop.Click += new System.EventHandler(this.labelTop_Click);
            // 
            // LabelDualLine
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Name = "LabelDualLine";
            this.Size = new System.Drawing.Size(245, 86);
            this.Resize += new System.EventHandler(this.LabelDualLine_Resize);
            this.panelContent.ResumeLayout(false);
            this.panelBack.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label labelBottom;
        private System.Windows.Forms.Label labelTop;
    }
}
