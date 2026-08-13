namespace HECS.Gui.Monitor.Controls
{
    partial class ControlBase
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
            this.panelBack = new System.Windows.Forms.Panel();
            this.panelContent = new System.Windows.Forms.Panel();
            this.panelBack.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelBack
            // 
            this.panelBack.Controls.Add(this.panelContent);
            this.panelBack.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelBack.Location = new System.Drawing.Point(0, 0);
            this.panelBack.Margin = new System.Windows.Forms.Padding(0);
            this.panelBack.Name = "panelBack";
            this.panelBack.Padding = new System.Windows.Forms.Padding(5);
            this.panelBack.Size = new System.Drawing.Size(245, 113);
            this.panelBack.TabIndex = 0;
            this.panelBack.Click += new System.EventHandler(this.panelBack_Click);
            // 
            // panelContent
            // 
            this.panelContent.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelContent.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.panelContent.Location = new System.Drawing.Point(5, 5);
            this.panelContent.Name = "panelContent";
            this.panelContent.Size = new System.Drawing.Size(235, 103);
            this.panelContent.TabIndex = 0;
            this.panelContent.Click += new System.EventHandler(this.panelContent_Click);
            // 
            // ControlBase
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.panelBack);
            this.Name = "ControlBase";
            this.Size = new System.Drawing.Size(245, 113);
            this.ControlAdded += new System.Windows.Forms.ControlEventHandler(this.ControlBase_ControlAdded);
            this.panelBack.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        protected System.Windows.Forms.Panel panelContent;
        protected System.Windows.Forms.Panel panelBack;
    }
}
