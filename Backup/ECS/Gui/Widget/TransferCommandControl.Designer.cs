namespace HECS.Gui.Widget
{
    partial class TransferCommandControl
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
            this.textBoxBatch = new System.Windows.Forms.TextBox();
            this.textBoxCommandType = new System.Windows.Forms.TextBox();
            this.textBoxMaterialNo = new System.Windows.Forms.TextBox();
            this.textBoxPalletId = new System.Windows.Forms.TextBox();
            this.textBoxCommandId = new System.Windows.Forms.TextBox();
            this.textBoxPriority = new System.Windows.Forms.TextBox();
            this.textBoxNewPriority = new System.Windows.Forms.TextBox();
            this.textBoxSource = new System.Windows.Forms.TextBox();
            this.textBoxDestination = new System.Windows.Forms.TextBox();
            this.buttonChangePriority = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.SuspendLayout();
            // 
            // textBoxBatch
            // 
            this.textBoxBatch.BackColor = System.Drawing.Color.LightGoldenrodYellow;
            this.textBoxBatch.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxBatch.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxBatch.Location = new System.Drawing.Point(76, 0);
            this.textBoxBatch.Name = "textBoxBatch";
            this.textBoxBatch.ReadOnly = true;
            this.textBoxBatch.Size = new System.Drawing.Size(121, 21);
            this.textBoxBatch.TabIndex = 19;
            // 
            // textBoxCommandType
            // 
            this.textBoxCommandType.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxCommandType.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxCommandType.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxCommandType.Location = new System.Drawing.Point(0, 0);
            this.textBoxCommandType.Name = "textBoxCommandType";
            this.textBoxCommandType.ReadOnly = true;
            this.textBoxCommandType.Size = new System.Drawing.Size(76, 21);
            this.textBoxCommandType.TabIndex = 20;
            // 
            // textBoxMaterialNo
            // 
            this.textBoxMaterialNo.BackColor = System.Drawing.Color.LightGoldenrodYellow;
            this.textBoxMaterialNo.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxMaterialNo.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxMaterialNo.Location = new System.Drawing.Point(197, 0);
            this.textBoxMaterialNo.Name = "textBoxMaterialNo";
            this.textBoxMaterialNo.ReadOnly = true;
            this.textBoxMaterialNo.Size = new System.Drawing.Size(116, 21);
            this.textBoxMaterialNo.TabIndex = 21;
            // 
            // textBoxPalletId
            // 
            this.textBoxPalletId.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxPalletId.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxPalletId.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxPalletId.Location = new System.Drawing.Point(554, 0);
            this.textBoxPalletId.Name = "textBoxPalletId";
            this.textBoxPalletId.ReadOnly = true;
            this.textBoxPalletId.Size = new System.Drawing.Size(58, 21);
            this.textBoxPalletId.TabIndex = 22;
            this.textBoxPalletId.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // textBoxCommandId
            // 
            this.textBoxCommandId.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxCommandId.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxCommandId.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxCommandId.Location = new System.Drawing.Point(313, 0);
            this.textBoxCommandId.Name = "textBoxCommandId";
            this.textBoxCommandId.ReadOnly = true;
            this.textBoxCommandId.Size = new System.Drawing.Size(121, 21);
            this.textBoxCommandId.TabIndex = 23;
            // 
            // textBoxPriority
            // 
            this.textBoxPriority.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxPriority.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxPriority.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxPriority.Location = new System.Drawing.Point(612, 0);
            this.textBoxPriority.Name = "textBoxPriority";
            this.textBoxPriority.ReadOnly = true;
            this.textBoxPriority.Size = new System.Drawing.Size(40, 21);
            this.textBoxPriority.TabIndex = 24;
            this.textBoxPriority.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // textBoxNewPriority
            // 
            this.textBoxNewPriority.BackColor = System.Drawing.Color.DodgerBlue;
            this.textBoxNewPriority.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxNewPriority.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxNewPriority.Location = new System.Drawing.Point(652, 0);
            this.textBoxNewPriority.Name = "textBoxNewPriority";
            this.textBoxNewPriority.Size = new System.Drawing.Size(40, 21);
            this.textBoxNewPriority.TabIndex = 28;
            this.textBoxNewPriority.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // textBoxSource
            // 
            this.textBoxSource.BackColor = System.Drawing.Color.CadetBlue;
            this.textBoxSource.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxSource.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxSource.Location = new System.Drawing.Point(434, 0);
            this.textBoxSource.Name = "textBoxSource";
            this.textBoxSource.ReadOnly = true;
            this.textBoxSource.Size = new System.Drawing.Size(60, 21);
            this.textBoxSource.TabIndex = 29;
            // 
            // textBoxDestination
            // 
            this.textBoxDestination.BackColor = System.Drawing.Color.CadetBlue;
            this.textBoxDestination.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxDestination.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxDestination.Location = new System.Drawing.Point(494, 0);
            this.textBoxDestination.Name = "textBoxDestination";
            this.textBoxDestination.ReadOnly = true;
            this.textBoxDestination.Size = new System.Drawing.Size(60, 21);
            this.textBoxDestination.TabIndex = 30;
            // 
            // buttonChangePriority
            // 
            this.buttonChangePriority.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonChangePriority.BackColor = System.Drawing.Color.LemonChiffon;
            this.buttonChangePriority.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonChangePriority.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.OliveGreen;
            this.buttonChangePriority.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonChangePriority.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonChangePriority.ForeColor = System.Drawing.SystemColors.HotTrack;
            this.buttonChangePriority.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonChangePriority.Location = new System.Drawing.Point(692, 0);
            this.buttonChangePriority.Name = "buttonChangePriority";
            this.buttonChangePriority.Size = new System.Drawing.Size(88, 20);
            this.buttonChangePriority.TabIndex = 18;
            this.buttonChangePriority.Text = "변경";
            this.buttonChangePriority.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            this.buttonChangePriority.UseVisualStyleBackColor = false;
            this.buttonChangePriority.Click += new System.EventHandler(this.buttonChangePriority_Click);
            // 
            // TransferCommandControl
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.LemonChiffon;
            this.Controls.Add(this.buttonChangePriority);
            this.Controls.Add(this.textBoxNewPriority);
            this.Controls.Add(this.textBoxPriority);
            this.Controls.Add(this.textBoxPalletId);
            this.Controls.Add(this.textBoxDestination);
            this.Controls.Add(this.textBoxSource);
            this.Controls.Add(this.textBoxCommandId);
            this.Controls.Add(this.textBoxMaterialNo);
            this.Controls.Add(this.textBoxBatch);
            this.Controls.Add(this.textBoxCommandType);
            this.Name = "TransferCommandControl";
            this.Size = new System.Drawing.Size(780, 20);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonChangePriority;
        private System.Windows.Forms.TextBox textBoxBatch;
        private System.Windows.Forms.TextBox textBoxCommandType;
        private System.Windows.Forms.TextBox textBoxMaterialNo;
        private System.Windows.Forms.TextBox textBoxPalletId;
        private System.Windows.Forms.TextBox textBoxCommandId;
        private System.Windows.Forms.TextBox textBoxPriority;
        private System.Windows.Forms.TextBox textBoxNewPriority;
        private System.Windows.Forms.TextBox textBoxSource;
        private System.Windows.Forms.TextBox textBoxDestination;
    }
}
