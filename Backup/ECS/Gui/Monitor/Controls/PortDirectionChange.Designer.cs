namespace HECS.Gui.Monitor.Controls
{
    partial class PortDirectionChange
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.lblPortID = new System.Windows.Forms.Label();
            this.lblNak = new System.Windows.Forms.Label();
            this.lblEnable = new System.Windows.Forms.Label();
            this.lblBackDummy = new System.Windows.Forms.Label();
            this.lbl_IN = new System.Windows.Forms.Label();
            this.lbl_OUT = new System.Windows.Forms.Label();
            this.lbl_BI = new System.Windows.Forms.Label();
            this.buttonBiDirection = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonOutput = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonInput = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.White;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.lblPortID);
            this.panel1.Controls.Add(this.lblNak);
            this.panel1.Controls.Add(this.lblEnable);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Padding = new System.Windows.Forms.Padding(5);
            this.panel1.Size = new System.Drawing.Size(185, 30);
            this.panel1.TabIndex = 0;
            // 
            // lblPortID
            // 
            this.lblPortID.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblPortID.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPortID.Location = new System.Drawing.Point(41, 5);
            this.lblPortID.Name = "lblPortID";
            this.lblPortID.Size = new System.Drawing.Size(100, 18);
            this.lblPortID.TabIndex = 3;
            this.lblPortID.Text = "CS01IO-C01";
            this.lblPortID.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblNak
            // 
            this.lblNak.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblNak.Dock = System.Windows.Forms.DockStyle.Right;
            this.lblNak.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblNak.Location = new System.Drawing.Point(141, 5);
            this.lblNak.Name = "lblNak";
            this.lblNak.Size = new System.Drawing.Size(37, 18);
            this.lblNak.TabIndex = 2;
            this.lblNak.Text = "NAK";
            // 
            // lblEnable
            // 
            this.lblEnable.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblEnable.Dock = System.Windows.Forms.DockStyle.Left;
            this.lblEnable.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblEnable.Location = new System.Drawing.Point(5, 5);
            this.lblEnable.Name = "lblEnable";
            this.lblEnable.Size = new System.Drawing.Size(36, 18);
            this.lblEnable.TabIndex = 0;
            this.lblEnable.Text = "ENB";
            // 
            // lblBackDummy
            // 
            this.lblBackDummy.BackColor = System.Drawing.Color.Gainsboro;
            this.lblBackDummy.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblBackDummy.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblBackDummy.Location = new System.Drawing.Point(0, 0);
            this.lblBackDummy.Name = "lblBackDummy";
            this.lblBackDummy.Size = new System.Drawing.Size(185, 90);
            this.lblBackDummy.TabIndex = 4;
            // 
            // lbl_IN
            // 
            this.lbl_IN.BackColor = System.Drawing.Color.Gainsboro;
            this.lbl_IN.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lbl_IN.Location = new System.Drawing.Point(10, 76);
            this.lbl_IN.Name = "lbl_IN";
            this.lbl_IN.Size = new System.Drawing.Size(45, 8);
            this.lbl_IN.TabIndex = 5;
            // 
            // lbl_OUT
            // 
            this.lbl_OUT.BackColor = System.Drawing.Color.Gainsboro;
            this.lbl_OUT.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lbl_OUT.Location = new System.Drawing.Point(70, 76);
            this.lbl_OUT.Name = "lbl_OUT";
            this.lbl_OUT.Size = new System.Drawing.Size(45, 8);
            this.lbl_OUT.TabIndex = 6;
            // 
            // lbl_BI
            // 
            this.lbl_BI.BackColor = System.Drawing.SystemColors.Control;
            this.lbl_BI.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lbl_BI.Location = new System.Drawing.Point(129, 76);
            this.lbl_BI.Name = "lbl_BI";
            this.lbl_BI.Size = new System.Drawing.Size(45, 8);
            this.lbl_BI.TabIndex = 7;
            // 
            // buttonBiDirection
            // 
            this.buttonBiDirection.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonBiDirection.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonBiDirection.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonBiDirection.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonBiDirection.Location = new System.Drawing.Point(125, 36);
            this.buttonBiDirection.Name = "buttonBiDirection";
            this.buttonBiDirection.Size = new System.Drawing.Size(54, 42);
            this.buttonBiDirection.TabIndex = 3;
            this.buttonBiDirection.Text = "BI";
            this.buttonBiDirection.UseVisualStyleBackColor = true;

            // 
            // buttonOutput
            // 
            this.buttonOutput.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonOutput.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonOutput.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonOutput.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonOutput.Location = new System.Drawing.Point(65, 36);
            this.buttonOutput.Name = "buttonOutput";
            this.buttonOutput.Size = new System.Drawing.Size(54, 42);
            this.buttonOutput.TabIndex = 2;
            this.buttonOutput.Text = "OUT";
            this.buttonOutput.UseVisualStyleBackColor = true;

            // 
            // buttonInput
            // 
            this.buttonInput.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonInput.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonInput.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonInput.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonInput.Location = new System.Drawing.Point(5, 36);
            this.buttonInput.Name = "buttonInput";
            this.buttonInput.Size = new System.Drawing.Size(54, 42);
            this.buttonInput.TabIndex = 1;
            this.buttonInput.Text = "IN";
            this.buttonInput.UseVisualStyleBackColor = true;

            // 
            // PortDirectionChange
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.lbl_BI);
            this.Controls.Add(this.lbl_OUT);
            this.Controls.Add(this.lbl_IN);
            this.Controls.Add(this.buttonBiDirection);
            this.Controls.Add(this.buttonOutput);
            this.Controls.Add(this.buttonInput);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.lblBackDummy);
            this.Name = "PortDirectionChange";
            this.Size = new System.Drawing.Size(185, 90);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lblNak;
        private System.Windows.Forms.Label lblEnable;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonInput;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonOutput;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonBiDirection;
        private System.Windows.Forms.Label lblBackDummy;
        private System.Windows.Forms.Label lbl_IN;
        private System.Windows.Forms.Label lbl_OUT;
        private System.Windows.Forms.Label lbl_BI;
        public System.Windows.Forms.Label lblPortID;


    }
}
