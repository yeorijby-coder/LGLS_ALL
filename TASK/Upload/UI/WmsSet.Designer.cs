namespace WmsUp
{
	partial class WmsSet
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(WmsSet));
			this.StatusStrip1 = new System.Windows.Forms.StatusStrip();
			this.StatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
			this.ToolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
			this.OpenFileDialog2 = new System.Windows.Forms.OpenFileDialog();
			this.OpenFileDialog1 = new System.Windows.Forms.OpenFileDialog();
			this.TabControl1 = new System.Windows.Forms.TabControl();
			this.TabPage1 = new System.Windows.Forms.TabPage();
			this.Panel1 = new System.Windows.Forms.Panel();
			this.btnDir = new System.Windows.Forms.Button();
			this.txtDownFix = new System.Windows.Forms.TextBox();
			this.Label5 = new System.Windows.Forms.Label();
			this.GroupBox7 = new System.Windows.Forms.GroupBox();
			this.txtDb_Pw = new System.Windows.Forms.TextBox();
			this.txtDb_User = new System.Windows.Forms.TextBox();
			this.Label12 = new System.Windows.Forms.Label();
			this.Label17 = new System.Windows.Forms.Label();
			this.txtDb_Svc = new System.Windows.Forms.TextBox();
			this.txtDb_Ip = new System.Windows.Forms.TextBox();
			this.Label15 = new System.Windows.Forms.Label();
			this.Label16 = new System.Windows.Forms.Label();
			this.GroupBox4 = new System.Windows.Forms.GroupBox();
			this.rdoUpLoad_2 = new System.Windows.Forms.RadioButton();
			this.rdoUpLoad_1 = new System.Windows.Forms.RadioButton();
			this.GroupBox1 = new System.Windows.Forms.GroupBox();
			this.rdoSetup_2 = new System.Windows.Forms.RadioButton();
			this.rdoSetup_1 = new System.Windows.Forms.RadioButton();
			this.btnIcon = new System.Windows.Forms.Button();
			this.GroupBox3 = new System.Windows.Forms.GroupBox();
			this.rdoDownPath_2 = new System.Windows.Forms.RadioButton();
			this.rdoDownPath_1 = new System.Windows.Forms.RadioButton();
			this.Label1 = new System.Windows.Forms.Label();
			this.txtIcon = new System.Windows.Forms.TextBox();
			this.Label4 = new System.Windows.Forms.Label();
			this.Label2 = new System.Windows.Forms.Label();
			this.txtTitle = new System.Windows.Forms.TextBox();
			this.but_Init = new System.Windows.Forms.Button();
			this.btn_Save = new System.Windows.Forms.Button();
			this.TabPage2 = new System.Windows.Forms.TabPage();
			this.Panel2 = new System.Windows.Forms.Panel();
			this.Label3 = new System.Windows.Forms.Label();
			this.RTxtTable = new System.Windows.Forms.RichTextBox();
			this.StatusStrip1.SuspendLayout();
			this.TabControl1.SuspendLayout();
			this.TabPage1.SuspendLayout();
			this.Panel1.SuspendLayout();
			this.GroupBox7.SuspendLayout();
			this.GroupBox4.SuspendLayout();
			this.GroupBox1.SuspendLayout();
			this.GroupBox3.SuspendLayout();
			this.TabPage2.SuspendLayout();
			this.Panel2.SuspendLayout();
			this.SuspendLayout();
			// 
			// StatusStrip1
			// 
			this.StatusStrip1.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
			this.StatusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusLabel1,
            this.ToolStripStatusLabel1});
			this.StatusStrip1.Location = new System.Drawing.Point(0, 313);
			this.StatusStrip1.Name = "StatusStrip1";
			this.StatusStrip1.Size = new System.Drawing.Size(494, 22);
			this.StatusStrip1.TabIndex = 6;
			this.StatusStrip1.Text = "StatusStrip1";
			// 
			// StatusLabel1
			// 
			this.StatusLabel1.Name = "StatusLabel1";
			this.StatusLabel1.Size = new System.Drawing.Size(23, 17);
			this.StatusLabel1.Text = "...";
			// 
			// ToolStripStatusLabel1
			// 
			this.ToolStripStatusLabel1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.None;
			this.ToolStripStatusLabel1.Name = "ToolStripStatusLabel1";
			this.ToolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
			this.ToolStripStatusLabel1.Text = "ToolStripStatusLabel1";
			// 
			// OpenFileDialog2
			// 
			this.OpenFileDialog2.Filter = "아이콘파일 (*.ico)|*.ico|모든 파일 (*.*)|*.*";
			// 
			// TabControl1
			// 
			this.TabControl1.Controls.Add(this.TabPage1);
			this.TabControl1.Controls.Add(this.TabPage2);
			this.TabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TabControl1.Location = new System.Drawing.Point(0, 0);
			this.TabControl1.Name = "TabControl1";
			this.TabControl1.SelectedIndex = 0;
			this.TabControl1.Size = new System.Drawing.Size(494, 313);
			this.TabControl1.TabIndex = 8;
			// 
			// TabPage1
			// 
			this.TabPage1.Controls.Add(this.Panel1);
			this.TabPage1.Location = new System.Drawing.Point(4, 22);
			this.TabPage1.Name = "TabPage1";
			this.TabPage1.Padding = new System.Windows.Forms.Padding(3);
			this.TabPage1.Size = new System.Drawing.Size(486, 287);
			this.TabPage1.TabIndex = 0;
			this.TabPage1.Text = "프로그램및 서버 설정";
			this.TabPage1.UseVisualStyleBackColor = true;
			// 
			// Panel1
			// 
			this.Panel1.BackColor = System.Drawing.SystemColors.Control;
			this.Panel1.Controls.Add(this.btnDir);
			this.Panel1.Controls.Add(this.txtDownFix);
			this.Panel1.Controls.Add(this.Label5);
			this.Panel1.Controls.Add(this.GroupBox7);
			this.Panel1.Controls.Add(this.GroupBox4);
			this.Panel1.Controls.Add(this.GroupBox1);
			this.Panel1.Controls.Add(this.btnIcon);
			this.Panel1.Controls.Add(this.GroupBox3);
			this.Panel1.Controls.Add(this.Label1);
			this.Panel1.Controls.Add(this.txtIcon);
			this.Panel1.Controls.Add(this.Label4);
			this.Panel1.Controls.Add(this.Label2);
			this.Panel1.Controls.Add(this.txtTitle);
			this.Panel1.Controls.Add(this.but_Init);
			this.Panel1.Controls.Add(this.btn_Save);
			this.Panel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Panel1.Location = new System.Drawing.Point(3, 3);
			this.Panel1.Name = "Panel1";
			this.Panel1.Size = new System.Drawing.Size(480, 281);
			this.Panel1.TabIndex = 0;
			// 
			// btnDir
			// 
			this.btnDir.Location = new System.Drawing.Point(383, 58);
			this.btnDir.Name = "btnDir";
			this.btnDir.Size = new System.Drawing.Size(42, 20);
			this.btnDir.TabIndex = 46;
			this.btnDir.TabStop = false;
			this.btnDir.Text = "열기";
			this.btnDir.UseVisualStyleBackColor = true;
			this.btnDir.Click += new System.EventHandler(this.btnDir_Click);
			// 
			// txtDownFix
			// 
			this.txtDownFix.BackColor = System.Drawing.SystemColors.Control;
			this.txtDownFix.Location = new System.Drawing.Point(169, 56);
			this.txtDownFix.MaxLength = 100;
			this.txtDownFix.Name = "txtDownFix";
			this.txtDownFix.ReadOnly = true;
			this.txtDownFix.Size = new System.Drawing.Size(212, 21);
			this.txtDownFix.TabIndex = 4;
			this.txtDownFix.TabStop = false;
			this.txtDownFix.Text = ".";
			// 
			// Label5
			// 
			this.Label5.AutoSize = true;
			this.Label5.Location = new System.Drawing.Point(18, 60);
			this.Label5.Name = "Label5";
			this.Label5.Size = new System.Drawing.Size(147, 24);
			this.Label5.TabIndex = 44;
			this.Label5.Text = "DOWN_LOAD_PATH_FIX=\r\n  (파일 다운로드 패스)";
			// 
			// GroupBox7
			// 
			this.GroupBox7.Controls.Add(this.txtDb_Pw);
			this.GroupBox7.Controls.Add(this.txtDb_User);
			this.GroupBox7.Controls.Add(this.Label12);
			this.GroupBox7.Controls.Add(this.Label17);
			this.GroupBox7.Controls.Add(this.txtDb_Svc);
			this.GroupBox7.Controls.Add(this.txtDb_Ip);
			this.GroupBox7.Controls.Add(this.Label15);
			this.GroupBox7.Controls.Add(this.Label16);
			this.GroupBox7.Location = new System.Drawing.Point(252, 88);
			this.GroupBox7.Name = "GroupBox7";
			this.GroupBox7.Size = new System.Drawing.Size(222, 150);
			this.GroupBox7.TabIndex = 39;
			this.GroupBox7.TabStop = false;
			this.GroupBox7.Text = "DB 서버 설정";
			// 
			// txtDb_Pw
			// 
			this.txtDb_Pw.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
			this.txtDb_Pw.Location = new System.Drawing.Point(109, 112);
			this.txtDb_Pw.MaxLength = 8;
			this.txtDb_Pw.Name = "txtDb_Pw";
			this.txtDb_Pw.PasswordChar = '*';
			this.txtDb_Pw.Size = new System.Drawing.Size(99, 21);
			this.txtDb_Pw.TabIndex = 22;
			// 
			// txtDb_User
			// 
			this.txtDb_User.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
			this.txtDb_User.Location = new System.Drawing.Point(109, 85);
			this.txtDb_User.MaxLength = 15;
			this.txtDb_User.Name = "txtDb_User";
			this.txtDb_User.Size = new System.Drawing.Size(99, 21);
			this.txtDb_User.TabIndex = 21;
			// 
			// Label12
			// 
			this.Label12.AutoSize = true;
			this.Label12.Location = new System.Drawing.Point(74, 116);
			this.Label12.Name = "Label12";
			this.Label12.Size = new System.Drawing.Size(29, 12);
			this.Label12.TabIndex = 24;
			this.Label12.Text = "PW=";
			// 
			// Label17
			// 
			this.Label17.AutoSize = true;
			this.Label17.Location = new System.Drawing.Point(49, 88);
			this.Label17.Name = "Label17";
			this.Label17.Size = new System.Drawing.Size(54, 12);
			this.Label17.TabIndex = 23;
			this.Label17.Text = "USERID=";
			// 
			// txtDb_Svc
			// 
			this.txtDb_Svc.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
			this.txtDb_Svc.Location = new System.Drawing.Point(109, 58);
			this.txtDb_Svc.MaxLength = 10;
			this.txtDb_Svc.Name = "txtDb_Svc";
			this.txtDb_Svc.Size = new System.Drawing.Size(99, 21);
			this.txtDb_Svc.TabIndex = 11;
			// 
			// txtDb_Ip
			// 
			this.txtDb_Ip.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
			this.txtDb_Ip.Location = new System.Drawing.Point(109, 31);
			this.txtDb_Ip.MaxLength = 15;
			this.txtDb_Ip.Name = "txtDb_Ip";
			this.txtDb_Ip.Size = new System.Drawing.Size(99, 21);
			this.txtDb_Ip.TabIndex = 10;
			// 
			// Label15
			// 
			this.Label15.AutoSize = true;
			this.Label15.Location = new System.Drawing.Point(28, 63);
			this.Label15.Name = "Label15";
			this.Label15.Size = new System.Drawing.Size(75, 12);
			this.Label15.TabIndex = 20;
			this.Label15.Text = "DATABASE=";
			// 
			// Label16
			// 
			this.Label16.AutoSize = true;
			this.Label16.Location = new System.Drawing.Point(8, 34);
			this.Label16.Name = "Label16";
			this.Label16.Size = new System.Drawing.Size(95, 24);
			this.Label16.TabIndex = 19;
			this.Label16.Text = "SERVERNAME=\r\n(IP or TNS)";
			// 
			// GroupBox4
			// 
			this.GroupBox4.Controls.Add(this.rdoUpLoad_2);
			this.GroupBox4.Controls.Add(this.rdoUpLoad_1);
			this.GroupBox4.Location = new System.Drawing.Point(20, 132);
			this.GroupBox4.Name = "GroupBox4";
			this.GroupBox4.Size = new System.Drawing.Size(226, 60);
			this.GroupBox4.TabIndex = 35;
			this.GroupBox4.TabStop = false;
			this.GroupBox4.Text = "UP_LOAD_KEY";
			// 
			// rdoUpLoad_2
			// 
			this.rdoUpLoad_2.AutoSize = true;
			this.rdoUpLoad_2.Enabled = false;
			this.rdoUpLoad_2.ForeColor = System.Drawing.SystemColors.GradientInactiveCaption;
			this.rdoUpLoad_2.Location = new System.Drawing.Point(19, 38);
			this.rdoUpLoad_2.Name = "rdoUpLoad_2";
			this.rdoUpLoad_2.Size = new System.Drawing.Size(199, 16);
			this.rdoUpLoad_2.TabIndex = 6;
			this.rdoUpLoad_2.Text = "UPLAOD파일명 [UPLOAD 위치]";
			this.rdoUpLoad_2.UseVisualStyleBackColor = true;
			// 
			// rdoUpLoad_1
			// 
			this.rdoUpLoad_1.AutoSize = true;
			this.rdoUpLoad_1.Checked = true;
			this.rdoUpLoad_1.Location = new System.Drawing.Point(19, 16);
			this.rdoUpLoad_1.Name = "rdoUpLoad_1";
			this.rdoUpLoad_1.Size = new System.Drawing.Size(59, 16);
			this.rdoUpLoad_1.TabIndex = 6;
			this.rdoUpLoad_1.TabStop = true;
			this.rdoUpLoad_1.Text = "File 명";
			this.rdoUpLoad_1.UseVisualStyleBackColor = true;
			// 
			// GroupBox1
			// 
			this.GroupBox1.Controls.Add(this.rdoSetup_2);
			this.GroupBox1.Controls.Add(this.rdoSetup_1);
			this.GroupBox1.Location = new System.Drawing.Point(20, 88);
			this.GroupBox1.Name = "GroupBox1";
			this.GroupBox1.Size = new System.Drawing.Size(226, 38);
			this.GroupBox1.TabIndex = 34;
			this.GroupBox1.TabStop = false;
			this.GroupBox1.Text = "SETUP_BTN";
			// 
			// rdoSetup_2
			// 
			this.rdoSetup_2.AutoSize = true;
			this.rdoSetup_2.Checked = true;
			this.rdoSetup_2.Location = new System.Drawing.Point(90, 16);
			this.rdoSetup_2.Name = "rdoSetup_2";
			this.rdoSetup_2.Size = new System.Drawing.Size(47, 16);
			this.rdoSetup_2.TabIndex = 5;
			this.rdoSetup_2.TabStop = true;
			this.rdoSetup_2.Text = "불가";
			this.rdoSetup_2.UseVisualStyleBackColor = true;
			// 
			// rdoSetup_1
			// 
			this.rdoSetup_1.AutoSize = true;
			this.rdoSetup_1.Location = new System.Drawing.Point(19, 16);
			this.rdoSetup_1.Name = "rdoSetup_1";
			this.rdoSetup_1.Size = new System.Drawing.Size(47, 16);
			this.rdoSetup_1.TabIndex = 5;
			this.rdoSetup_1.TabStop = true;
			this.rdoSetup_1.Text = "가능";
			this.rdoSetup_1.UseVisualStyleBackColor = true;
			// 
			// btnIcon
			// 
			this.btnIcon.Location = new System.Drawing.Point(383, 35);
			this.btnIcon.Name = "btnIcon";
			this.btnIcon.Size = new System.Drawing.Size(42, 20);
			this.btnIcon.TabIndex = 3;
			this.btnIcon.TabStop = false;
			this.btnIcon.Text = "열기";
			this.btnIcon.UseVisualStyleBackColor = true;
			this.btnIcon.Click += new System.EventHandler(this.btnIcon_Click);
			// 
			// GroupBox3
			// 
			this.GroupBox3.Controls.Add(this.rdoDownPath_2);
			this.GroupBox3.Controls.Add(this.rdoDownPath_1);
			this.GroupBox3.Location = new System.Drawing.Point(21, 198);
			this.GroupBox3.Name = "GroupBox3";
			this.GroupBox3.Size = new System.Drawing.Size(225, 57);
			this.GroupBox3.TabIndex = 36;
			this.GroupBox3.TabStop = false;
			this.GroupBox3.Text = "DOWN_LOAD_PATH_SET";
			// 
			// rdoDownPath_2
			// 
			this.rdoDownPath_2.AutoSize = true;
			this.rdoDownPath_2.Location = new System.Drawing.Point(19, 38);
			this.rdoDownPath_2.Name = "rdoDownPath_2";
			this.rdoDownPath_2.Size = new System.Drawing.Size(47, 16);
			this.rdoDownPath_2.TabIndex = 7;
			this.rdoDownPath_2.TabStop = true;
			this.rdoDownPath_2.Text = "불가";
			this.rdoDownPath_2.UseVisualStyleBackColor = true;
			// 
			// rdoDownPath_1
			// 
			this.rdoDownPath_1.AutoSize = true;
			this.rdoDownPath_1.Checked = true;
			this.rdoDownPath_1.Location = new System.Drawing.Point(19, 16);
			this.rdoDownPath_1.Name = "rdoDownPath_1";
			this.rdoDownPath_1.Size = new System.Drawing.Size(47, 16);
			this.rdoDownPath_1.TabIndex = 7;
			this.rdoDownPath_1.TabStop = true;
			this.rdoDownPath_1.Text = "가능";
			this.rdoDownPath_1.UseVisualStyleBackColor = true;
			// 
			// Label1
			// 
			this.Label1.AutoSize = true;
			this.Label1.Location = new System.Drawing.Point(13, 265);
			this.Label1.Name = "Label1";
			this.Label1.Size = new System.Drawing.Size(250, 12);
			this.Label1.TabIndex = 28;
			this.Label1.Text = "설명) 프로그램 설정 및 서버설정에 관한 정의";
			// 
			// txtIcon
			// 
			this.txtIcon.Location = new System.Drawing.Point(66, 32);
			this.txtIcon.Name = "txtIcon";
			this.txtIcon.ReadOnly = true;
			this.txtIcon.Size = new System.Drawing.Size(315, 21);
			this.txtIcon.TabIndex = 3;
			// 
			// Label4
			// 
			this.Label4.AutoSize = true;
			this.Label4.Location = new System.Drawing.Point(19, 35);
			this.Label4.Name = "Label4";
			this.Label4.Size = new System.Drawing.Size(41, 12);
			this.Label4.TabIndex = 33;
			this.Label4.Text = "ICON=";
			// 
			// Label2
			// 
			this.Label2.AutoSize = true;
			this.Label2.Location = new System.Drawing.Point(18, 11);
			this.Label2.Name = "Label2";
			this.Label2.Size = new System.Drawing.Size(45, 12);
			this.Label2.TabIndex = 30;
			this.Label2.Text = "TITLE=";
			// 
			// txtTitle
			// 
			this.txtTitle.BackColor = System.Drawing.Color.White;
			this.txtTitle.Location = new System.Drawing.Point(66, 8);
			this.txtTitle.MaxLength = 100;
			this.txtTitle.Name = "txtTitle";
			this.txtTitle.Size = new System.Drawing.Size(315, 21);
			this.txtTitle.TabIndex = 2;
			// 
			// but_Init
			// 
			this.but_Init.Location = new System.Drawing.Point(295, 244);
			this.but_Init.Name = "but_Init";
			this.but_Init.Size = new System.Drawing.Size(74, 20);
			this.but_Init.TabIndex = 12;
			this.but_Init.TabStop = false;
			this.but_Init.Text = "초기화";
			this.but_Init.UseVisualStyleBackColor = true;
			this.but_Init.Click += new System.EventHandler(this.but_Init_Click);
			// 
			// btn_Save
			// 
			this.btn_Save.Location = new System.Drawing.Point(373, 244);
			this.btn_Save.Name = "btn_Save";
			this.btn_Save.Size = new System.Drawing.Size(74, 20);
			this.btn_Save.TabIndex = 13;
			this.btn_Save.TabStop = false;
			this.btn_Save.Text = "저장";
			this.btn_Save.UseVisualStyleBackColor = true;
			this.btn_Save.Click += new System.EventHandler(this.btn_Save_Click);
			// 
			// TabPage2
			// 
			this.TabPage2.Controls.Add(this.Panel2);
			this.TabPage2.Location = new System.Drawing.Point(4, 22);
			this.TabPage2.Name = "TabPage2";
			this.TabPage2.Padding = new System.Windows.Forms.Padding(3);
			this.TabPage2.Size = new System.Drawing.Size(443, 292);
			this.TabPage2.TabIndex = 1;
			this.TabPage2.Text = "테이블 생성";
			this.TabPage2.UseVisualStyleBackColor = true;
			// 
			// Panel2
			// 
			this.Panel2.BackColor = System.Drawing.SystemColors.Control;
			this.Panel2.Controls.Add(this.Label3);
			this.Panel2.Controls.Add(this.RTxtTable);
			this.Panel2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Panel2.Location = new System.Drawing.Point(3, 3);
			this.Panel2.Name = "Panel2";
			this.Panel2.Size = new System.Drawing.Size(437, 286);
			this.Panel2.TabIndex = 0;
			// 
			// Label3
			// 
			this.Label3.AutoSize = true;
			this.Label3.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
			this.Label3.Location = new System.Drawing.Point(9, 246);
			this.Label3.Name = "Label3";
			this.Label3.Size = new System.Drawing.Size(223, 36);
			this.Label3.TabIndex = 14;
			this.Label3.Text = "설명) DOWNLOAD 테이블 생성\r\n\r\n주의) 테이블, 필드는 바꾸지 말 것";
			// 
			// RTxtTable
			// 
			this.RTxtTable.BackColor = System.Drawing.Color.White;
			this.RTxtTable.Font = new System.Drawing.Font("굴림체", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
			this.RTxtTable.Location = new System.Drawing.Point(3, 3);
			this.RTxtTable.Name = "RTxtTable";
			this.RTxtTable.ReadOnly = true;
			this.RTxtTable.Size = new System.Drawing.Size(429, 237);
			this.RTxtTable.TabIndex = 0;
			this.RTxtTable.Text = resources.GetString("RTxtTable.Text");
			// 
			// WmsSet
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(494, 335);
			this.Controls.Add(this.TabControl1);
			this.Controls.Add(this.StatusStrip1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "WmsSet";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "UPLOAD환경설정";
			this.Load += new System.EventHandler(this.WmsSet_Load);
			this.StatusStrip1.ResumeLayout(false);
			this.StatusStrip1.PerformLayout();
			this.TabControl1.ResumeLayout(false);
			this.TabPage1.ResumeLayout(false);
			this.Panel1.ResumeLayout(false);
			this.Panel1.PerformLayout();
			this.GroupBox7.ResumeLayout(false);
			this.GroupBox7.PerformLayout();
			this.GroupBox4.ResumeLayout(false);
			this.GroupBox4.PerformLayout();
			this.GroupBox1.ResumeLayout(false);
			this.GroupBox1.PerformLayout();
			this.GroupBox3.ResumeLayout(false);
			this.GroupBox3.PerformLayout();
			this.TabPage2.ResumeLayout(false);
			this.Panel2.ResumeLayout(false);
			this.Panel2.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		internal System.Windows.Forms.StatusStrip StatusStrip1;
		internal System.Windows.Forms.ToolStripStatusLabel StatusLabel1;
		internal System.Windows.Forms.ToolStripStatusLabel ToolStripStatusLabel1;
		internal System.Windows.Forms.OpenFileDialog OpenFileDialog2;
		internal System.Windows.Forms.OpenFileDialog OpenFileDialog1;
		internal System.Windows.Forms.TabControl TabControl1;
		internal System.Windows.Forms.TabPage TabPage1;
		internal System.Windows.Forms.Panel Panel1;
		internal System.Windows.Forms.Button btnDir;
		internal System.Windows.Forms.TextBox txtDownFix;
		internal System.Windows.Forms.Label Label5;
		internal System.Windows.Forms.GroupBox GroupBox7;
		internal System.Windows.Forms.TextBox txtDb_Pw;
		internal System.Windows.Forms.TextBox txtDb_User;
		internal System.Windows.Forms.Label Label12;
		internal System.Windows.Forms.Label Label17;
		internal System.Windows.Forms.TextBox txtDb_Svc;
		internal System.Windows.Forms.TextBox txtDb_Ip;
		internal System.Windows.Forms.Label Label15;
		internal System.Windows.Forms.Label Label16;
		internal System.Windows.Forms.GroupBox GroupBox4;
		internal System.Windows.Forms.RadioButton rdoUpLoad_2;
		internal System.Windows.Forms.RadioButton rdoUpLoad_1;
		internal System.Windows.Forms.GroupBox GroupBox1;
		internal System.Windows.Forms.RadioButton rdoSetup_2;
		internal System.Windows.Forms.RadioButton rdoSetup_1;
		internal System.Windows.Forms.Button btnIcon;
		internal System.Windows.Forms.GroupBox GroupBox3;
		internal System.Windows.Forms.RadioButton rdoDownPath_2;
		internal System.Windows.Forms.RadioButton rdoDownPath_1;
		internal System.Windows.Forms.Label Label1;
		internal System.Windows.Forms.TextBox txtIcon;
		internal System.Windows.Forms.Label Label4;
		internal System.Windows.Forms.Label Label2;
		internal System.Windows.Forms.TextBox txtTitle;
		internal System.Windows.Forms.Button but_Init;
		internal System.Windows.Forms.Button btn_Save;
		internal System.Windows.Forms.TabPage TabPage2;
		internal System.Windows.Forms.Panel Panel2;
		internal System.Windows.Forms.Label Label3;
		internal System.Windows.Forms.RichTextBox RTxtTable;

	}
}