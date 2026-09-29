using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;

namespace WmsUp
{
	public partial class Path : Form
	{
		public Path()
		{
			InitializeComponent();
		}

		public string psRet;
		public string psYN;

		private void OK_Button_Click(object sender, EventArgs e)
		{
			this.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.psRet = txtDownFix.Text;
			this.psYN = "Y";
			this.Close();
		}

		private void Cancel_Button_Click(object sender, EventArgs e)
		{
			this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.psRet = "";
			this.psYN = "N";
			this.Close();
		}

		private void Path_Load(object sender, EventArgs e)
		{
			string pico = null;

			////--------------------------------
			//// ICon 적용
			////--------------------------------
			pico = new string(' ', 100);
			modCom.ReadInitProfile_C(ref pico, "APPLICATION", "ICON");

			if (!string.IsNullOrEmpty(FileSystem.Dir(pico)))
			{
				// Create icon.
				Icon newIcon = new Icon(pico);

				this.Icon = newIcon;
			}

			this.txtDownFix.Text = ".";
		}

		private void btnDir_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string sName = null;

			this.OpenFileDialog1.InitialDirectory = txtDownFix.Text;

			if (this.OpenFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				sName = OpenFileDialog1.FileName.ToString();
				iPos = Strings.InStrRev(sName, "\\");

				if (iPos <= 0)
				{
					MessageBox.Show("FILE 에러입니다.", "그림파일 선택", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				this.txtDownFix.Text = Strings.Left(sName, iPos - 1);
			}
		}


	}
}
