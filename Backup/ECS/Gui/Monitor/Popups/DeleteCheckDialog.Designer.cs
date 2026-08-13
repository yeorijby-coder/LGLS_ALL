using HECS.Gui.Jaeit.Buttons;
namespace HECS.Gui.Monitor.Popups
{
    partial class DeleteCheckDialog
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.txtDesciption = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.buttonNo = new HECS.Gui.Jaeit.Buttons.ButtonGlass();
            this.buttonYes = new HECS.Gui.Jaeit.Buttons.ButtonGlass();
            this.panelBack.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelBack
            // 
            this.panelBack.Controls.Add(this.panel2);
            this.panelBack.Padding = new System.Windows.Forms.Padding(9, 7, 8, 9);
            this.panelBack.Size = new System.Drawing.Size(533, 160);
            this.panelBack.Text = "확인";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Thistle;
            this.panel1.Controls.Add(this.txtDesciption);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(2, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(508, 64);
            this.panel1.TabIndex = 0;
            // 
            // txtDesciption
            // 
            this.txtDesciption.BackColor = System.Drawing.Color.RoyalBlue;
            this.txtDesciption.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.txtDesciption.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtDesciption.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.txtDesciption.ForeColor = System.Drawing.Color.White;
            this.txtDesciption.Location = new System.Drawing.Point(0, 0);
            this.txtDesciption.Name = "txtDesciption";
            this.txtDesciption.Size = new System.Drawing.Size(508, 64);
            this.txtDesciption.TabIndex = 0;
            this.txtDesciption.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.LemonChiffon;
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel2.Controls.Add(this.buttonNo);
            this.panel2.Controls.Add(this.buttonYes);
            this.panel2.Controls.Add(this.panel1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(9, 21);
            this.panel2.Name = "panel2";
            this.panel2.Padding = new System.Windows.Forms.Padding(2);
            this.panel2.Size = new System.Drawing.Size(516, 130);
            this.panel2.TabIndex = 7;
            // 
            // buttonNo
            // 
            this.buttonNo.BackColor = System.Drawing.Color.RoyalBlue;
            this.buttonNo.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonNo.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonNo.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonNo.ForeColor = System.Drawing.Color.Black;
            this.buttonNo.Image = global::HECS.Properties.Resources.Cancel22;
            this.buttonNo.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonNo.Location = new System.Drawing.Point(253, 70);
            this.buttonNo.Name = "buttonNo";
            this.buttonNo.OuterBorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonNo.Size = new System.Drawing.Size(101, 54);
            this.buttonNo.TabIndex = 10;
            this.buttonNo.Text = "아니오";
            this.buttonNo.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonNo.Click += new System.EventHandler(this.buttonNo_Click);
            // 
            // buttonYes
            // 
            this.buttonYes.BackColor = System.Drawing.Color.RoyalBlue;
            this.buttonYes.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonYes.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonYes.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonYes.ForeColor = System.Drawing.Color.Black;
            this.buttonYes.Image = global::HECS.Properties.Resources.OK4;
            this.buttonYes.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonYes.Location = new System.Drawing.Point(151, 70);
            this.buttonYes.Name = "buttonYes";
            this.buttonYes.OuterBorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonYes.Size = new System.Drawing.Size(101, 54);
            this.buttonYes.TabIndex = 9;
            this.buttonYes.TabStop = false;
            this.buttonYes.Text = "예";
            this.buttonYes.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonYes.Click += new System.EventHandler(this.buttonYes_Click);
            // 
            // DeleteCheckDialog
            // 
            this.ClientSize = new System.Drawing.Size(533, 160);
            this.Name = "DeleteCheckDialog";
            this.Text = "확인";
            this.panelBack.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label txtDesciption;
        private System.Windows.Forms.Panel panel2;
        private ButtonGlass buttonNo;
        private ButtonGlass buttonYes;
    }
}
