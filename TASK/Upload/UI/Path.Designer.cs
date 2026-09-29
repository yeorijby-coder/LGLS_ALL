namespace WmsUp
{
	partial class Path
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Path));
			this.OpenFileDialog1 = new System.Windows.Forms.OpenFileDialog();
			this.btnDir = new System.Windows.Forms.Button();
			this.txtDownFix = new System.Windows.Forms.TextBox();
			this.Label2 = new System.Windows.Forms.Label();
			this.Label1 = new System.Windows.Forms.Label();
			this.TableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.OK_Button = new System.Windows.Forms.Button();
			this.Cancel_Button = new System.Windows.Forms.Button();
			this.TableLayoutPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnDir
			// 
			this.btnDir.Location = new System.Drawing.Point(240, 43);
			this.btnDir.Name = "btnDir";
			this.btnDir.Size = new System.Drawing.Size(42, 20);
			this.btnDir.TabIndex = 53;
			this.btnDir.TabStop = false;
			this.btnDir.Text = "열기";
			this.btnDir.UseVisualStyleBackColor = true;
			this.btnDir.Click += new System.EventHandler(this.btnDir_Click);
			// 
			// txtDownFix
			// 
			this.txtDownFix.BackColor = System.Drawing.Color.White;
			this.txtDownFix.Location = new System.Drawing.Point(65, 42);
			this.txtDownFix.MaxLength = 100;
			this.txtDownFix.Name = "txtDownFix";
			this.txtDownFix.Size = new System.Drawing.Size(169, 21);
			this.txtDownFix.TabIndex = 52;
			this.txtDownFix.TabStop = false;
			this.txtDownFix.Text = ".";
			// 
			// Label2
			// 
			this.Label2.AutoSize = true;
			this.Label2.Font = new System.Drawing.Font("굴림체", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
			this.Label2.Location = new System.Drawing.Point(24, 44);
			this.Label2.Name = "Label2";
			this.Label2.Size = new System.Drawing.Size(42, 16);
			this.Label2.TabIndex = 51;
			this.Label2.Text = "위치";
			// 
			// Label1
			// 
			this.Label1.AutoSize = true;
			this.Label1.Font = new System.Drawing.Font("굴림체", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
			this.Label1.Location = new System.Drawing.Point(8, 9);
			this.Label1.Name = "Label1";
			this.Label1.Size = new System.Drawing.Size(277, 16);
			this.Label1.TabIndex = 50;
			this.Label1.Text = "Download할 위치를 지정하십시오.";
			// 
			// TableLayoutPanel1
			// 
			this.TableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.TableLayoutPanel1.ColumnCount = 2;
			this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.TableLayoutPanel1.Controls.Add(this.OK_Button, 0, 0);
			this.TableLayoutPanel1.Controls.Add(this.Cancel_Button, 1, 0);
			this.TableLayoutPanel1.Location = new System.Drawing.Point(98, 71);
			this.TableLayoutPanel1.Name = "TableLayoutPanel1";
			this.TableLayoutPanel1.RowCount = 1;
			this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.TableLayoutPanel1.Size = new System.Drawing.Size(170, 27);
			this.TableLayoutPanel1.TabIndex = 49;
			// 
			// OK_Button
			// 
			this.OK_Button.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.OK_Button.Location = new System.Drawing.Point(3, 3);
			this.OK_Button.Name = "OK_Button";
			this.OK_Button.Size = new System.Drawing.Size(78, 21);
			this.OK_Button.TabIndex = 0;
			this.OK_Button.Text = "확인";
			this.OK_Button.Click += new System.EventHandler(this.OK_Button_Click);
			// 
			// Cancel_Button
			// 
			this.Cancel_Button.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.Cancel_Button.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.Cancel_Button.Location = new System.Drawing.Point(88, 3);
			this.Cancel_Button.Name = "Cancel_Button";
			this.Cancel_Button.Size = new System.Drawing.Size(78, 21);
			this.Cancel_Button.TabIndex = 1;
			this.Cancel_Button.Text = "취소";
			this.Cancel_Button.Click += new System.EventHandler(this.Cancel_Button_Click);
			// 
			// Path
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(290, 106);
			this.Controls.Add(this.btnDir);
			this.Controls.Add(this.txtDownFix);
			this.Controls.Add(this.Label2);
			this.Controls.Add(this.Label1);
			this.Controls.Add(this.TableLayoutPanel1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "Path";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "다운로드 위치변경";
			this.Load += new System.EventHandler(this.Path_Load);
			this.TableLayoutPanel1.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		internal System.Windows.Forms.OpenFileDialog OpenFileDialog1;
		internal System.Windows.Forms.Button btnDir;
		internal System.Windows.Forms.TextBox txtDownFix;
		internal System.Windows.Forms.Label Label2;
		internal System.Windows.Forms.Label Label1;
		internal System.Windows.Forms.TableLayoutPanel TableLayoutPanel1;
		internal System.Windows.Forms.Button OK_Button;
		internal System.Windows.Forms.Button Cancel_Button;
	}
}