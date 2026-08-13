namespace HECS.Gui.Monitor.Controls
{
    partial class LampInform
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.panelContent.SuspendLayout();
            this.panelBack.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelContent
            // 
            this.panelContent.Controls.Add(this.label9);
            this.panelContent.Controls.Add(this.label8);
            this.panelContent.Controls.Add(this.label7);
            this.panelContent.Controls.Add(this.label6);
            this.panelContent.Controls.Add(this.label5);
            this.panelContent.Controls.Add(this.label4);
            this.panelContent.Controls.Add(this.label3);
            this.panelContent.Controls.Add(this.label2);
            this.panelContent.Controls.Add(this.label1);
            this.panelContent.Padding = new System.Windows.Forms.Padding(1);
            this.panelContent.Size = new System.Drawing.Size(26, 127);
            this.panelContent.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // panelBack
            // 
            this.panelBack.Size = new System.Drawing.Size(28, 129);
            this.panelBack.Click += new System.EventHandler(this.AnyControlClicked);
            this.panelBack.Resize += new System.EventHandler(this.panelBack_Resize);
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.DarkRed;
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(1, 1);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(24, 14);
            this.label1.TabIndex = 0;
            this.label1.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.DarkRed;
            this.label2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label2.Dock = System.Windows.Forms.DockStyle.Top;
            this.label2.Location = new System.Drawing.Point(1, 15);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(24, 14);
            this.label2.TabIndex = 1;
            this.label2.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // label3
            // 
            this.label3.BackColor = System.Drawing.Color.DarkRed;
            this.label3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label3.Dock = System.Windows.Forms.DockStyle.Top;
            this.label3.Location = new System.Drawing.Point(1, 29);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(24, 14);
            this.label3.TabIndex = 2;
            this.label3.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // label4
            // 
            this.label4.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.label4.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label4.Dock = System.Windows.Forms.DockStyle.Top;
            this.label4.Location = new System.Drawing.Point(1, 43);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(24, 14);
            this.label4.TabIndex = 3;
            this.label4.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // label5
            // 
            this.label5.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label5.Dock = System.Windows.Forms.DockStyle.Top;
            this.label5.Location = new System.Drawing.Point(1, 57);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(24, 14);
            this.label5.TabIndex = 4;
            this.label5.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // label6
            // 
            this.label6.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.label6.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label6.Dock = System.Windows.Forms.DockStyle.Top;
            this.label6.Location = new System.Drawing.Point(1, 71);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(24, 14);
            this.label6.TabIndex = 5;
            this.label6.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // label7
            // 
            this.label7.BackColor = System.Drawing.Color.DarkGreen;
            this.label7.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label7.Dock = System.Windows.Forms.DockStyle.Top;
            this.label7.Location = new System.Drawing.Point(1, 85);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(24, 14);
            this.label7.TabIndex = 6;
            this.label7.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // label8
            // 
            this.label8.BackColor = System.Drawing.Color.DarkGreen;
            this.label8.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label8.Dock = System.Windows.Forms.DockStyle.Top;
            this.label8.Location = new System.Drawing.Point(1, 99);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(24, 14);
            this.label8.TabIndex = 7;
            this.label8.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // label9
            // 
            this.label9.BackColor = System.Drawing.Color.DarkGreen;
            this.label9.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label9.Dock = System.Windows.Forms.DockStyle.Top;
            this.label9.Location = new System.Drawing.Point(1, 113);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(24, 14);
            this.label9.TabIndex = 8;
            this.label9.Click += new System.EventHandler(this.AnyControlClicked);
            // 
            // LampInform
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Name = "LampInform";
            this.Size = new System.Drawing.Size(28, 129);
            this.panelContent.ResumeLayout(false);
            this.panelBack.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
    }
}
