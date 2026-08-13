namespace HECS.Gui.Monitor.Controls
{
    partial class ControlOptionButton
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
            this.panelContent = new System.Windows.Forms.Panel();
            this.panelBackLamp = new System.Windows.Forms.Panel();
            this.buttonDisable = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonEnable = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.panelTop = new System.Windows.Forms.Panel();
            this.chkRecovery = new System.Windows.Forms.CheckBox();
            this.labelTop = new System.Windows.Forms.Label();
            this.labelSplit = new System.Windows.Forms.Label();
            this.panelContent.SuspendLayout();
            this.panelBackLamp.SuspendLayout();
            this.panelTop.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelContent
            // 
            this.panelContent.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelContent.Controls.Add(this.panelBackLamp);
            this.panelContent.Controls.Add(this.panelTop);
            this.panelContent.Controls.Add(this.labelSplit);
            this.panelContent.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelContent.Location = new System.Drawing.Point(0, 0);
            this.panelContent.Name = "panelContent";
            this.panelContent.Padding = new System.Windows.Forms.Padding(2);
            this.panelContent.Size = new System.Drawing.Size(250, 76);
            this.panelContent.TabIndex = 0;
            // 
            // panelBackLamp
            // 
            this.panelBackLamp.BackColor = System.Drawing.Color.Lime;
            this.panelBackLamp.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelBackLamp.Controls.Add(this.buttonDisable);
            this.panelBackLamp.Controls.Add(this.buttonEnable);
            this.panelBackLamp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelBackLamp.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.panelBackLamp.Location = new System.Drawing.Point(2, 22);
            this.panelBackLamp.Name = "panelBackLamp";
            this.panelBackLamp.Padding = new System.Windows.Forms.Padding(2);
            this.panelBackLamp.Size = new System.Drawing.Size(244, 50);
            this.panelBackLamp.TabIndex = 16;
            // 
            // buttonDisable
            // 
            this.buttonDisable.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonDisable.BackColor = System.Drawing.Color.Transparent;
            this.buttonDisable.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonDisable.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.OliveGreen;
            this.buttonDisable.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonDisable.ForeColor = System.Drawing.Color.Black;
            this.buttonDisable.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonDisable.Location = new System.Drawing.Point(124, 2);
            this.buttonDisable.Name = "buttonDisable";
            this.buttonDisable.Size = new System.Drawing.Size(116, 44);
            this.buttonDisable.TabIndex = 1;
            this.buttonDisable.Text = "buttonXP1";
            this.buttonDisable.UseVisualStyleBackColor = false;
            this.buttonDisable.Click += new System.EventHandler(this.buttonDisable_Click);
            // 
            // buttonEnable
            // 
            this.buttonEnable.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonEnable.BackColor = System.Drawing.Color.Transparent;
            this.buttonEnable.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonEnable.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.OliveGreen;
            this.buttonEnable.Dock = System.Windows.Forms.DockStyle.Left;
            this.buttonEnable.ForeColor = System.Drawing.Color.Black;
            this.buttonEnable.Image = global:: HECS.Properties.Resources.system52158600;
            this.buttonEnable.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonEnable.Location = new System.Drawing.Point(2, 2);
            this.buttonEnable.Name = "buttonEnable";
            this.buttonEnable.Size = new System.Drawing.Size(116, 44);
            this.buttonEnable.TabIndex = 0;
            this.buttonEnable.Text = "buttonXP1";
            this.buttonEnable.UseVisualStyleBackColor = false;
            this.buttonEnable.Click += new System.EventHandler(this.buttonEnable_Click);
            // 
            // panelTop
            // 
            this.panelTop.Controls.Add(this.chkRecovery);
            this.panelTop.Controls.Add(this.labelTop);
            this.panelTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelTop.Location = new System.Drawing.Point(2, 4);
            this.panelTop.Name = "panelTop";
            this.panelTop.Size = new System.Drawing.Size(244, 18);
            this.panelTop.TabIndex = 15;
            // 
            // chkRecovery
            // 
            this.chkRecovery.AllowDrop = true;
            this.chkRecovery.AutoSize = true;
            this.chkRecovery.BackColor = System.Drawing.Color.LightSteelBlue;
            this.chkRecovery.Location = new System.Drawing.Point(168, 0);
            this.chkRecovery.Name = "chkRecovery";
            this.chkRecovery.Size = new System.Drawing.Size(76, 16);
            this.chkRecovery.TabIndex = 16;
            this.chkRecovery.Text = "수동 복구";
            this.chkRecovery.UseVisualStyleBackColor = true;
            this.chkRecovery.MouseDown += new System.Windows.Forms.MouseEventHandler(this.chkRecovery_MouseDown);
            // 
            // labelTop
            // 
            this.labelTop.BackColor = System.Drawing.Color.LightSteelBlue;
            this.labelTop.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTop.ForeColor = System.Drawing.Color.Black;
            this.labelTop.Location = new System.Drawing.Point(0, 0);
            this.labelTop.Margin = new System.Windows.Forms.Padding(0);
            this.labelTop.Name = "labelTop";
            this.labelTop.Size = new System.Drawing.Size(152, 17);
            this.labelTop.TabIndex = 15;
            this.labelTop.Text = "Dispatching";
            this.labelTop.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelSplit
            // 
            this.labelSplit.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelSplit.Location = new System.Drawing.Point(2, 2);
            this.labelSplit.Name = "labelSplit";
            this.labelSplit.Size = new System.Drawing.Size(244, 2);
            this.labelSplit.TabIndex = 12;
            // 
            // ControlOptionButton
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.LightSteelBlue;
            this.Controls.Add(this.panelContent);
            this.Name = "ControlOptionButton";
            this.Size = new System.Drawing.Size(250, 76);
            this.panelContent.ResumeLayout(false);
            this.panelBackLamp.ResumeLayout(false);
            this.panelTop.ResumeLayout(false);
            this.panelTop.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelContent;
        private System.Windows.Forms.Label labelSplit;
        private System.Windows.Forms.Panel panelBackLamp;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonDisable;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonEnable;
        private System.Windows.Forms.Panel panelTop;
        private System.Windows.Forms.CheckBox chkRecovery;
        private System.Windows.Forms.Label labelTop;

    }
}
