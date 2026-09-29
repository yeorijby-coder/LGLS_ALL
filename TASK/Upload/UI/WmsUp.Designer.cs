namespace WmsUp
{
	partial class WmsUp
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(WmsUp));
            this.GroupBox1 = new System.Windows.Forms.GroupBox();
            this.Button5 = new System.Windows.Forms.Button();
            this.iml32 = new System.Windows.Forms.ImageList(this.components);
            this.dtpToDate = new System.Windows.Forms.DateTimePicker();
            this.dtpFrDate = new System.Windows.Forms.DateTimePicker();
            this.dtpToTime = new System.Windows.Forms.DateTimePicker();
            this.dtpFrTime = new System.Windows.Forms.DateTimePicker();
            this.Label1 = new System.Windows.Forms.Label();
            this.StatusStrip1 = new System.Windows.Forms.StatusStrip();
            this.StatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.StatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.ToolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.btnDel = new System.Windows.Forms.Button();
            this.btnSearch = new System.Windows.Forms.Button();
            this.btnUpload = new System.Windows.Forms.Button();
            this.btnDownload = new System.Windows.Forms.Button();
            this.btnEnd = new System.Windows.Forms.Button();
            this.btnSet = new System.Windows.Forms.Button();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            // [LGLS 2026-09-30] FarPoint Spread -> DataGridView (라이선스가 없다)
            this.fp = new SliGrid();
            this.fp2 = new SliGrid();
            this.Label2 = new System.Windows.Forms.Label();
            this.GroupBox1.SuspendLayout();
            this.StatusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
            // [LGLS 2026-09-30] BeginInit 는 Spread 전용이라 없앴다
            this.SuspendLayout();
            // 
            // GroupBox1
            // 
            this.GroupBox1.BackColor = System.Drawing.Color.Transparent;
            this.GroupBox1.Controls.Add(this.Button5);
            this.GroupBox1.Controls.Add(this.dtpToDate);
            this.GroupBox1.Controls.Add(this.dtpFrDate);
            this.GroupBox1.Controls.Add(this.dtpToTime);
            this.GroupBox1.Controls.Add(this.dtpFrTime);
            this.GroupBox1.Controls.Add(this.Label1);
            this.GroupBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.GroupBox1.ForeColor = System.Drawing.Color.White;
            this.GroupBox1.Location = new System.Drawing.Point(0, 0);
            this.GroupBox1.Name = "GroupBox1";
            this.GroupBox1.Size = new System.Drawing.Size(959, 49);
            this.GroupBox1.TabIndex = 22;
            this.GroupBox1.TabStop = false;
            this.GroupBox1.Text = " - 조회조건 -";
            // 
            // Button5
            // 
            this.Button5.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.Button5.ImageIndex = 1;
            this.Button5.ImageList = this.iml32;
            this.Button5.Location = new System.Drawing.Point(273, 17);
            this.Button5.Name = "Button5";
            this.Button5.Size = new System.Drawing.Size(20, 21);
            this.Button5.TabIndex = 443;
            this.Button5.Click += new System.EventHandler(this.Button5_Click);
            // 
            // iml32
            // 
            this.iml32.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("iml32.ImageStream")));
            this.iml32.TransparentColor = System.Drawing.Color.Transparent;
            this.iml32.Images.SetKeyName(0, "");
            this.iml32.Images.SetKeyName(1, "Next1.bmp");
            // 
            // dtpToDate
            // 
            this.dtpToDate.CustomFormat = "yyyy-MM-dd";
            this.dtpToDate.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.dtpToDate.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtpToDate.Location = new System.Drawing.Point(293, 17);
            this.dtpToDate.Name = "dtpToDate";
            this.dtpToDate.Size = new System.Drawing.Size(90, 21);
            this.dtpToDate.TabIndex = 441;
            // 
            // dtpFrDate
            // 
            this.dtpFrDate.CustomFormat = "yyyy-MM-dd";
            this.dtpFrDate.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.dtpFrDate.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtpFrDate.Location = new System.Drawing.Point(103, 17);
            this.dtpFrDate.Name = "dtpFrDate";
            this.dtpFrDate.Size = new System.Drawing.Size(90, 21);
            this.dtpFrDate.TabIndex = 439;
            // 
            // dtpToTime
            // 
            this.dtpToTime.CustomFormat = "HH:mm:ss";
            this.dtpToTime.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.dtpToTime.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtpToTime.Location = new System.Drawing.Point(383, 17);
            this.dtpToTime.Name = "dtpToTime";
            this.dtpToTime.ShowUpDown = true;
            this.dtpToTime.Size = new System.Drawing.Size(80, 21);
            this.dtpToTime.TabIndex = 442;
            this.dtpToTime.Value = new System.DateTime(2020, 11, 11, 0, 0, 0, 0);
            // 
            // dtpFrTime
            // 
            this.dtpFrTime.CustomFormat = "HH:mm:ss";
            this.dtpFrTime.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.dtpFrTime.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtpFrTime.Location = new System.Drawing.Point(193, 17);
            this.dtpFrTime.Name = "dtpFrTime";
            this.dtpFrTime.ShowUpDown = true;
            this.dtpFrTime.Size = new System.Drawing.Size(80, 21);
            this.dtpFrTime.TabIndex = 440;
            this.dtpFrTime.Value = new System.DateTime(2010, 1, 17, 0, 0, 0, 0);
            // 
            // Label1
            // 
            this.Label1.AutoSize = true;
            this.Label1.Location = new System.Drawing.Point(23, 21);
            this.Label1.Name = "Label1";
            this.Label1.Size = new System.Drawing.Size(77, 12);
            this.Label1.TabIndex = 438;
            this.Label1.Text = "업로드일시 : ";
            // 
            // StatusStrip1
            // 
            this.StatusStrip1.BackColor = System.Drawing.Color.White;
            this.StatusStrip1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("StatusStrip1.BackgroundImage")));
            this.StatusStrip1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.StatusStrip1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.StatusStrip1.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.StatusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusLabel2,
            this.StatusLabel1,
            this.ToolStripStatusLabel1});
            this.StatusStrip1.Location = new System.Drawing.Point(0, 0);
            this.StatusStrip1.Name = "StatusStrip1";
            this.StatusStrip1.Padding = new System.Windows.Forms.Padding(1, 0, 16, 0);
            this.StatusStrip1.Size = new System.Drawing.Size(959, 31);
            this.StatusStrip1.TabIndex = 23;
            this.StatusStrip1.Text = "StatusStrip1";
            // 
            // StatusLabel2
            // 
            this.StatusLabel2.BackColor = System.Drawing.SystemColors.Control;
            this.StatusLabel2.BorderStyle = System.Windows.Forms.Border3DStyle.RaisedOuter;
            this.StatusLabel2.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.StatusLabel2.ForeColor = System.Drawing.Color.MediumTurquoise;
            this.StatusLabel2.Name = "StatusLabel2";
            this.StatusLabel2.Size = new System.Drawing.Size(47, 26);
            this.StatusLabel2.Text = "SERVER";
            // 
            // StatusLabel1
            // 
            this.StatusLabel1.ActiveLinkColor = System.Drawing.Color.Red;
            this.StatusLabel1.BackColor = System.Drawing.Color.Transparent;
            this.StatusLabel1.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.StatusLabel1.ForeColor = System.Drawing.Color.OldLace;
            this.StatusLabel1.Name = "StatusLabel1";
            this.StatusLabel1.Size = new System.Drawing.Size(26, 26);
            this.StatusLabel1.Text = "...";
            // 
            // ToolStripStatusLabel1
            // 
            this.ToolStripStatusLabel1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.None;
            this.ToolStripStatusLabel1.Name = "ToolStripStatusLabel1";
            this.ToolStripStatusLabel1.Size = new System.Drawing.Size(0, 26);
            this.ToolStripStatusLabel1.Text = "ToolStripStatusLabel1";
            // 
            // btnDel
            // 
            this.btnDel.BackColor = System.Drawing.Color.Snow;
            this.btnDel.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnDel.BackgroundImage")));
            this.btnDel.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnDel.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnDel.ForeColor = System.Drawing.Color.OldLace;
            this.btnDel.Image = ((System.Drawing.Image)(resources.GetObject("btnDel.Image")));
            this.btnDel.Location = new System.Drawing.Point(721, 5);
            this.btnDel.Name = "btnDel";
            this.btnDel.Size = new System.Drawing.Size(74, 24);
            this.btnDel.TabIndex = 27;
            this.btnDel.Text = " 삭제";
            this.btnDel.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnDel.UseVisualStyleBackColor = false;
            this.btnDel.Click += new System.EventHandler(this.btnDel_Click);
            // 
            // btnSearch
            // 
            this.btnSearch.BackColor = System.Drawing.Color.Snow;
            this.btnSearch.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnSearch.BackgroundImage")));
            this.btnSearch.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnSearch.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnSearch.ForeColor = System.Drawing.Color.OldLace;
            this.btnSearch.Image = ((System.Drawing.Image)(resources.GetObject("btnSearch.Image")));
            this.btnSearch.Location = new System.Drawing.Point(441, 5);
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.btnSearch.Size = new System.Drawing.Size(74, 24);
            this.btnSearch.TabIndex = 24;
            this.btnSearch.Text = " 조회";
            this.btnSearch.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnSearch.UseVisualStyleBackColor = false;
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // btnUpload
            // 
            this.btnUpload.BackColor = System.Drawing.Color.Snow;
            this.btnUpload.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnUpload.BackgroundImage")));
            this.btnUpload.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnUpload.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnUpload.ForeColor = System.Drawing.Color.OldLace;
            this.btnUpload.Image = ((System.Drawing.Image)(resources.GetObject("btnUpload.Image")));
            this.btnUpload.Location = new System.Drawing.Point(521, 5);
            this.btnUpload.Name = "btnUpload";
            this.btnUpload.Size = new System.Drawing.Size(74, 24);
            this.btnUpload.TabIndex = 25;
            this.btnUpload.Text = " 업로드";
            this.btnUpload.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnUpload.UseVisualStyleBackColor = false;
            this.btnUpload.Click += new System.EventHandler(this.btnUpload_Click);
            // 
            // btnDownload
            // 
            this.btnDownload.BackColor = System.Drawing.Color.Snow;
            this.btnDownload.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnDownload.BackgroundImage")));
            this.btnDownload.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnDownload.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnDownload.ForeColor = System.Drawing.Color.OldLace;
            this.btnDownload.Image = ((System.Drawing.Image)(resources.GetObject("btnDownload.Image")));
            this.btnDownload.Location = new System.Drawing.Point(601, 5);
            this.btnDownload.Name = "btnDownload";
            this.btnDownload.Size = new System.Drawing.Size(110, 24);
            this.btnDownload.TabIndex = 29;
            this.btnDownload.Text = "  다운로드";
            this.btnDownload.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnDownload.UseVisualStyleBackColor = false;
            this.btnDownload.Click += new System.EventHandler(this.btnDownload_Click);
            // 
            // btnEnd
            // 
            this.btnEnd.BackColor = System.Drawing.Color.Snow;
            this.btnEnd.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnEnd.BackgroundImage")));
            this.btnEnd.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnEnd.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnEnd.ForeColor = System.Drawing.Color.OldLace;
            this.btnEnd.Image = ((System.Drawing.Image)(resources.GetObject("btnEnd.Image")));
            this.btnEnd.Location = new System.Drawing.Point(883, 5);
            this.btnEnd.Name = "btnEnd";
            this.btnEnd.Size = new System.Drawing.Size(74, 24);
            this.btnEnd.TabIndex = 28;
            this.btnEnd.Text = " 종료";
            this.btnEnd.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnEnd.UseVisualStyleBackColor = false;
            this.btnEnd.Click += new System.EventHandler(this.btnEnd_Click);
            // 
            // btnSet
            // 
            this.btnSet.BackColor = System.Drawing.Color.Snow;
            this.btnSet.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnSet.BackgroundImage")));
            this.btnSet.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnSet.Font = new System.Drawing.Font("굴림체", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnSet.ForeColor = System.Drawing.Color.OldLace;
            this.btnSet.Image = ((System.Drawing.Image)(resources.GetObject("btnSet.Image")));
            this.btnSet.Location = new System.Drawing.Point(801, 5);
            this.btnSet.Name = "btnSet";
            this.btnSet.Size = new System.Drawing.Size(74, 24);
            this.btnSet.TabIndex = 26;
            this.btnSet.Text = " 설정";
            this.btnSet.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnSet.UseVisualStyleBackColor = false;
            this.btnSet.Click += new System.EventHandler(this.btnSet_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(0, 49);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.btnDel);
            this.splitContainer1.Panel2.Controls.Add(this.btnSearch);
            this.splitContainer1.Panel2.Controls.Add(this.btnUpload);
            this.splitContainer1.Panel2.Controls.Add(this.btnSet);
            this.splitContainer1.Panel2.Controls.Add(this.btnDownload);
            this.splitContainer1.Panel2.Controls.Add(this.btnEnd);
            this.splitContainer1.Panel2.Controls.Add(this.StatusStrip1);
            this.splitContainer1.Size = new System.Drawing.Size(959, 452);
            this.splitContainer1.SplitterDistance = 417;
            this.splitContainer1.TabIndex = 30;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.splitContainer3);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("splitContainer2.Panel2.BackgroundImage")));
            this.splitContainer2.Panel2.Controls.Add(this.Label2);
            this.splitContainer2.Size = new System.Drawing.Size(959, 417);
            this.splitContainer2.SplitterDistance = 643;
            this.splitContainer2.TabIndex = 0;
            // 
            // splitContainer3
            // 
            this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer3.Location = new System.Drawing.Point(0, 0);
            this.splitContainer3.Name = "splitContainer3";
            this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.fp);
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.fp2);
            this.splitContainer3.Size = new System.Drawing.Size(643, 417);
            this.splitContainer3.SplitterDistance = 205;
            this.splitContainer3.TabIndex = 0;
            // 
            // fp
            // 
            // fp - 다운로드 공통정보
            // [LGLS 2026-09-30] Spread 의 칸 설정을 SetColumns 한 번으로 옮겼다.
            this.fp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fp.Location = new System.Drawing.Point(0, 0);
            this.fp.Name = "fp";
            this.fp.Size = new System.Drawing.Size(643, 205);
            this.fp.TabIndex = 9;
            this.fp.SetColumns(
                new string[]  { "DN_NO", "DN_PGM", "DN_DIR", "DN_INF", "UP_CPT_NM", "UP_DNT" },
                new string[]  { "다운로드번호", "프로그램명", "다운로드위치", "다운로드 상세정보", "업로드컴퓨터", "업로드일시" },
                new int[]     { 100, 90, 100, 350, 100, 200 },
                new string[]  { "C", "L", "L", "L", "L", "L" });
            this.fp.CellClickEx += new System.EventHandler(this.fp_CellClick);
            // fp2
            // 
            // fp2 - 다운로드 상세정보
            this.fp2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fp2.Location = new System.Drawing.Point(0, 0);
            this.fp2.Name = "fp2";
            this.fp2.Size = new System.Drawing.Size(643, 208);
            this.fp2.TabIndex = 10;
            this.fp2.SetColumns(
                new string[]  { "DN_NO", "DN_LN", "UP_FNM", "DN_NM", "DN_SIZE", "DN_VER" },
                new string[]  { "다운로드번호", "순번", "파일명 [경로포함]", "파일명", "파일사이즈", "버전" },
                new int[]     { 100, 40, 180, 180, 130, 60 },
                new string[]  { "L", "C", "L", "L", "R", "C" });
            this.fp2.EnterCellEx += new System.EventHandler(this.fp2_EnterCell);
            // Label2
            // 
            this.Label2.BackColor = System.Drawing.Color.Transparent;
            this.Label2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.Label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Label2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Label2.ForeColor = System.Drawing.Color.White;
            this.Label2.Location = new System.Drawing.Point(0, 0);
            this.Label2.Name = "Label2";
            this.Label2.Size = new System.Drawing.Size(312, 417);
            this.Label2.TabIndex = 21;
            this.Label2.Text = "                    ";
            // 
            // WmsUp
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(959, 501);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.GroupBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "WmsUp";
            this.Text = "WMS 업로드";
            this.Activated += new System.EventHandler(this.WmsUp_Activated);
            this.Load += new System.EventHandler(this.WmsUp_Load);
            this.GroupBox1.ResumeLayout(false);
            this.GroupBox1.PerformLayout();
            this.StatusStrip1.ResumeLayout(false);
            this.StatusStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
            this.splitContainer3.ResumeLayout(false);
            // [LGLS 2026-09-30] EndInit 는 Spread 전용이라 없앴다
            this.ResumeLayout(false);

		}

		#endregion

		internal System.Windows.Forms.GroupBox GroupBox1;
		internal System.Windows.Forms.DateTimePicker dtpToDate;
		internal System.Windows.Forms.DateTimePicker dtpFrDate;
		internal System.Windows.Forms.DateTimePicker dtpToTime;
		internal System.Windows.Forms.DateTimePicker dtpFrTime;
		internal System.Windows.Forms.Label Label1;
		internal System.Windows.Forms.StatusStrip StatusStrip1;
		internal System.Windows.Forms.ToolStripStatusLabel StatusLabel2;
		internal System.Windows.Forms.ToolStripStatusLabel StatusLabel1;
		internal System.Windows.Forms.ToolStripStatusLabel ToolStripStatusLabel1;
		internal System.Windows.Forms.Button btnDel;
		internal System.Windows.Forms.Button btnSearch;
		internal System.Windows.Forms.Button btnUpload;
		internal System.Windows.Forms.Button btnDownload;
		internal System.Windows.Forms.Button btnEnd;
		internal System.Windows.Forms.Button btnSet;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.SplitContainer splitContainer2;
		private System.Windows.Forms.SplitContainer splitContainer3;
		internal SliGrid fp;
		internal SliGrid fpSh;
		internal SliGrid fp2;
		internal SliGrid fp2Sh;
		internal System.Windows.Forms.Label Label2;
		internal System.Windows.Forms.Button Button5;
		internal System.Windows.Forms.ImageList iml32;

	}
}

