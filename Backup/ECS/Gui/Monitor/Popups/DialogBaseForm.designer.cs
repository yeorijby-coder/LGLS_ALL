namespace HECS.Gui.Monitor.Popups
{
    partial class DialogBaseForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DialogBaseForm));
            this.panelBorder = new System.Windows.Forms.Panel();
            this.panelBack = new HECS.Gui.Jaeit.Panels.GroupBox.GroupBoxRoundedRect();
            this.panelBorder.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelBorder
            // 
            this.panelBorder.BackColor = System.Drawing.Color.White;
            this.panelBorder.Controls.Add(this.panelBack);
            this.panelBorder.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelBorder.Location = new System.Drawing.Point(0, 0);
            this.panelBorder.Margin = new System.Windows.Forms.Padding(0);
            this.panelBorder.Name = "panelBorder";
            this.panelBorder.Size = new System.Drawing.Size(853, 241);
            this.panelBorder.TabIndex = 0;
            // 
            // panelBack
            // 
            this.panelBack.BackColor = System.Drawing.Color.DarkKhaki;
            this.panelBack.BorderWidth = 6;
            this.panelBack.ColorScheme = HECS.Gui.Jaeit.Panels.GroupBox.EnmColorScheme.Green;
            this.panelBack.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelBack.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.panelBack.FontColor = System.Drawing.Color.FromArgb(((int)(((byte)(57)))), ((int)(((byte)(66)))), ((int)(((byte)(1)))));
            this.panelBack.Location = new System.Drawing.Point(0, 0);
            this.panelBack.Name = "panelBack";
            this.panelBack.Padding = new System.Windows.Forms.Padding(10, 7, 9, 7);
            this.panelBack.ShadowColor = System.Drawing.Color.DimGray;
            this.panelBack.Size = new System.Drawing.Size(853, 241);
            this.panelBack.TabIndex = 1;
            this.panelBack.TabStop = false;
            this.panelBack.Text = "groupBoxRoundedRect1";
            // 
            // DialogBaseForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(853, 241);
            this.Controls.Add(this.panelBorder);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "DialogBaseForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FormDialogBase";
            this.TopMost = true;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.DialogBaseForm_FormClosing);
            this.panelBorder.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelBorder;
        protected HECS.Gui.Jaeit.Panels.GroupBox.GroupBoxRoundedRect panelBack;


    }
}