namespace EcsClient
{
	partial class WmsDown
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(WmsDown));
            this.List1 = new System.Windows.Forms.ListBox();
            this.lbl_Cnt = new System.Windows.Forms.Label();
            this.Label6 = new System.Windows.Forms.Label();
            this.Label5 = new System.Windows.Forms.Label();
            this.Label4 = new System.Windows.Forms.Label();
            this.Panel1 = new System.Windows.Forms.Panel();
            this.Button2 = new System.Windows.Forms.Button();
            this.Button1 = new System.Windows.Forms.Button();
            this.Label7 = new System.Windows.Forms.Label();
            this.Label3 = new System.Windows.Forms.Label();
            this.Label2 = new System.Windows.Forms.Label();
            this.Label1 = new System.Windows.Forms.Label();
            this.ProgressBar1 = new System.Windows.Forms.ProgressBar();
            this.Timer1 = new System.Windows.Forms.Timer(this.components);
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.Panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // List1
            // 
            this.List1.FormattingEnabled = true;
            this.List1.ItemHeight = 12;
            this.List1.Location = new System.Drawing.Point(439, 71);
            this.List1.Name = "List1";
            this.List1.Size = new System.Drawing.Size(170, 124);
            this.List1.TabIndex = 22;
            this.List1.TabStop = false;
            this.List1.Visible = false;
            // 
            // lbl_Cnt
            // 
            this.lbl_Cnt.AutoSize = true;
            this.lbl_Cnt.BackColor = System.Drawing.Color.Transparent;
            this.lbl_Cnt.Location = new System.Drawing.Point(264, 36);
            this.lbl_Cnt.Name = "lbl_Cnt";
            this.lbl_Cnt.Size = new System.Drawing.Size(17, 12);
            this.lbl_Cnt.TabIndex = 21;
            this.lbl_Cnt.Text = "...";
            this.lbl_Cnt.Visible = false;
            // 
            // Label6
            // 
            this.Label6.AutoSize = true;
            this.Label6.BackColor = System.Drawing.Color.Transparent;
            this.Label6.Location = new System.Drawing.Point(264, 9);
            this.Label6.Name = "Label6";
            this.Label6.Size = new System.Drawing.Size(17, 12);
            this.Label6.TabIndex = 20;
            this.Label6.Text = "...";
            this.Label6.Visible = false;
            // 
            // Label5
            // 
            this.Label5.AutoSize = true;
            this.Label5.BackColor = System.Drawing.Color.Transparent;
            this.Label5.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label5.Location = new System.Drawing.Point(167, 36);
            this.Label5.Name = "Label5";
            this.Label5.Size = new System.Drawing.Size(68, 13);
            this.Label5.TabIndex = 19;
            this.Label5.Text = "전체 상태";
            this.Label5.Visible = false;
            // 
            // Label4
            // 
            this.Label4.AutoSize = true;
            this.Label4.BackColor = System.Drawing.Color.Transparent;
            this.Label4.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label4.Location = new System.Drawing.Point(167, 9);
            this.Label4.Name = "Label4";
            this.Label4.Size = new System.Drawing.Size(68, 13);
            this.Label4.TabIndex = 18;
            this.Label4.Text = "전송 파일";
            this.Label4.Visible = false;
            // 
            // Panel1
            // 
            this.Panel1.BackColor = System.Drawing.Color.Transparent;
            this.Panel1.Controls.Add(this.Button2);
            this.Panel1.Controls.Add(this.Button1);
            this.Panel1.Controls.Add(this.Label7);
            this.Panel1.Location = new System.Drawing.Point(9, 113);
            this.Panel1.Name = "Panel1";
            this.Panel1.Size = new System.Drawing.Size(408, 30);
            this.Panel1.TabIndex = 17;
            // 
            // Button2
            // 
            this.Button2.BackColor = System.Drawing.Color.Navy;
            this.Button2.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Button2.ForeColor = System.Drawing.Color.White;
            this.Button2.Location = new System.Drawing.Point(270, 2);
            this.Button2.Name = "Button2";
            this.Button2.Size = new System.Drawing.Size(65, 26);
            this.Button2.TabIndex = 4;
            this.Button2.Text = "시작";
            this.Button2.UseVisualStyleBackColor = false;
            this.Button2.Click += new System.EventHandler(this.Button2_Click);
            // 
            // Button1
            // 
            this.Button1.BackColor = System.Drawing.Color.Navy;
            this.Button1.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Button1.ForeColor = System.Drawing.Color.White;
            this.Button1.Location = new System.Drawing.Point(336, 2);
            this.Button1.Name = "Button1";
            this.Button1.Size = new System.Drawing.Size(65, 26);
            this.Button1.TabIndex = 3;
            this.Button1.Text = "설정";
            this.Button1.UseVisualStyleBackColor = false;
            this.Button1.Click += new System.EventHandler(this.Button1_Click);
            // 
            // Label7
            // 
            this.Label7.AutoSize = true;
            this.Label7.Location = new System.Drawing.Point(8, 8);
            this.Label7.Name = "Label7";
            this.Label7.Size = new System.Drawing.Size(25, 12);
            this.Label7.TabIndex = 5;
            this.Label7.Text = ".....";
            // 
            // Label3
            // 
            this.Label3.AutoSize = true;
            this.Label3.BackColor = System.Drawing.Color.Transparent;
            this.Label3.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label3.Location = new System.Drawing.Point(385, 94);
            this.Label3.Name = "Label3";
            this.Label3.Size = new System.Drawing.Size(37, 12);
            this.Label3.TabIndex = 16;
            this.Label3.Text = "100%";
            this.Label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // Label2
            // 
            this.Label2.AutoSize = true;
            this.Label2.BackColor = System.Drawing.Color.Transparent;
            this.Label2.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label2.Location = new System.Drawing.Point(199, 95);
            this.Label2.Name = "Label2";
            this.Label2.Size = new System.Drawing.Size(30, 12);
            this.Label2.TabIndex = 15;
            this.Label2.Text = "50%";
            this.Label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // Label1
            // 
            this.Label1.AutoSize = true;
            this.Label1.BackColor = System.Drawing.Color.Transparent;
            this.Label1.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label1.Location = new System.Drawing.Point(7, 95);
            this.Label1.Name = "Label1";
            this.Label1.Size = new System.Drawing.Size(23, 12);
            this.Label1.TabIndex = 14;
            this.Label1.Text = "0%";
            this.Label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ProgressBar1
            // 
            this.ProgressBar1.Location = new System.Drawing.Point(9, 63);
            this.ProgressBar1.Name = "ProgressBar1";
            this.ProgressBar1.Size = new System.Drawing.Size(397, 21);
            this.ProgressBar1.TabIndex = 13;
            // 
            // Timer1
            // 
            this.Timer1.Interval = 500;
            this.Timer1.Tick += new System.EventHandler(this.Timer1_Tick);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::EcsClient.Properties.Resources.Doosan_Logo_3C_RGB;
            this.pictureBox1.Location = new System.Drawing.Point(13, 7);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(148, 50);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 23;
            this.pictureBox1.TabStop = false;
            // 
            // WmsDown
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(427, 150);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.List1);
            this.Controls.Add(this.lbl_Cnt);
            this.Controls.Add(this.Label6);
            this.Controls.Add(this.Label5);
            this.Controls.Add(this.Label4);
            this.Controls.Add(this.Panel1);
            this.Controls.Add(this.Label3);
            this.Controls.Add(this.Label2);
            this.Controls.Add(this.Label1);
            this.Controls.Add(this.ProgressBar1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "WCSDown";
            this.Text = "HUONS CLIENT";
            this.Load += new System.EventHandler(this.WmsDown_Load);
            this.Panel1.ResumeLayout(false);
            this.Panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		internal System.Windows.Forms.ListBox List1;
		internal System.Windows.Forms.Label lbl_Cnt;
		internal System.Windows.Forms.Label Label6;
		internal System.Windows.Forms.Label Label5;
		internal System.Windows.Forms.Label Label4;
		internal System.Windows.Forms.Panel Panel1;
		internal System.Windows.Forms.Button Button2;
		internal System.Windows.Forms.Button Button1;
		internal System.Windows.Forms.Label Label7;
		internal System.Windows.Forms.Label Label3;
		internal System.Windows.Forms.Label Label2;
		internal System.Windows.Forms.Label Label1;
		internal System.Windows.Forms.ProgressBar ProgressBar1;
        internal System.Windows.Forms.Timer Timer1;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}

