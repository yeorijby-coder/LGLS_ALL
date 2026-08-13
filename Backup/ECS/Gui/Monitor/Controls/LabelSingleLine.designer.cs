namespace HECS.Gui.Monitor.Controls
{
    partial class LabelSingleLine
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

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.labelText = new System.Windows.Forms.Label();
            this.panelContent.SuspendLayout();
            this.panelBack.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelContent
            // 
            this.panelContent.Controls.Add(this.labelText);
            this.panelContent.Padding = new System.Windows.Forms.Padding(3);
            this.panelContent.Size = new System.Drawing.Size(219, 98);
            // 
            // panelBack
            // 
            this.panelBack.Size = new System.Drawing.Size(221, 100);
            // 
            // labelText
            // 
            this.labelText.Dock = System.Windows.Forms.DockStyle.Fill;
            this.labelText.Location = new System.Drawing.Point(3, 3);
            this.labelText.Name = "labelText";
            this.labelText.Size = new System.Drawing.Size(213, 92);
            this.labelText.TabIndex = 4;
            this.labelText.Text = "Text";
            this.labelText.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.labelText.Click += new System.EventHandler(this.labelText_Click);
            // 
            // LabelSingleLine
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Name = "LabelSingleLine";
            this.Size = new System.Drawing.Size(221, 100);
            this.panelContent.ResumeLayout(false);
            this.panelBack.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label labelText;


    }
}
