using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;

namespace EcsClient
{
	public partial class WmsSet : Form
	{
		public WmsSet()
		{
			InitializeComponent();
		}

		private void WmsSet_Load(object sender, EventArgs e)
		{

			string strTitle = "";
			string strForm_Pic = "";
			string strIco = "";
			string strSet_Btn = "";
			string strDn_St = "";
			string strDn_Path = "";
			string strDn_Comp = "";
			string strExec_file = "";
			string strDb_ip = "";
			string strDb_Svc = "";
			string strDb_User = "";
			string strDb_Pw = "";
			string strDown_Progm = "";

			////--------------------------------
			//// 초기화
			////--------------------------------
			StatusBar1.Text = "";

			////--------------------------------
			//// Title
			////--------------------------------
			modCom.ReadInitProfile_C(ref strTitle, "APPLICATION", "TITLE");

			txtTitle.Text = strTitle;

			////--------------------------------
			//// Form Picture 적용
			////--------------------------------
			modCom.ReadInitProfile_C(ref strForm_Pic, "APPLICATION", "FORM_PICTURE");

			txtForm_Pic.Text = strForm_Pic;

			////--------------------------------
			//// ICon 적용
			////--------------------------------
			modCom.ReadInitProfile_C(ref strIco, "APPLICATION", "ICON");

			if (!string.IsNullOrEmpty(FileSystem.Dir(strIco)) & !string.IsNullOrEmpty(strIco))
			{
				// Create icon.
				Icon newIcon = new Icon(strIco);

				this.Icon = newIcon;

				txtIcon.Text = strIco;
			}

			////--------------------------------
			//// 셋업 버튼 적용
			////--------------------------------
			modCom.ReadInitProfile_C(ref strSet_Btn, "APPLICATION", "SETUP_BTN");

			if (strSet_Btn == "1")
			{
				rdoSetup_1.Checked = true;
			}
			else
			{
				rdoSetup_2.Checked = true;
			}

			////--------------------------------
			//// DownLoad 시작 적용
			////--------------------------------
			modCom.ReadInitProfile_C(ref strDn_St, "APPLICATION", "DOWN_LOAD_START");

			if (strDn_St == "1")
			{
				rdoDownSt_1.Checked = true;
			}
			else
			{
				rdoDownSt_2.Checked = true;
			}

			////--------------------------------
			//// 다운로드 path 적용
			////--------------------------------
			modCom.ReadInitProfile_C(ref strDn_Path, "APPLICATION", "DOWN_LOAD_PATH_SET");

			if (strDn_Path == "2")
			{
				rdoDownPath_2.Checked = true;
			}
			else if (strDn_Path == "3")
			{
				rdoDownPath_3.Checked = true;
			}
			else
			{
				rdoDownPath_1.Checked = true;
			}

			////--------------------------------
			//// DownLoad 파일 비교
			////--------------------------------
			modCom.ReadInitProfile_C(ref strDn_Comp, "APPLICATION", "DOWN_LOAD_COMP");

			if (strDn_Comp == "1")
			{
				rdoDownComp_1.Checked = true;
			}
			else
			{
				rdoDownComp_2.Checked = true;
			}

			////--------------------------------
			//// 실행파일 1
			////--------------------------------
			modCom.ReadInitProfile_C(ref strExec_file, "RUN_FILE", "FILE1");

			txtFile1.Text = strExec_file;

			////--------------------------------
			//// 실행파일 2
			////--------------------------------
			modCom.ReadInitProfile_C(ref strExec_file, "RUN_FILE", "FILE2");

			txtFile2.Text = strExec_file;

			////--------------------------------
			//// 실행파일 3
			////--------------------------------
			modCom.ReadInitProfile_C(ref strExec_file, "RUN_FILE", "FILE3");

			txtFile3.Text = strExec_file;

			////--------------------------------
			//// 실행파일 4
			////--------------------------------
			modCom.ReadInitProfile_C(ref strExec_file, "RUN_FILE", "FILE4");

			txtFile4.Text = strExec_file;

			////--------------------------------
			//// 실행파일 5
			////--------------------------------
			modCom.ReadInitProfile_C(ref strExec_file, "RUN_FILE", "FILE5");

			txtFile5.Text = strExec_file;

			////--------------------------------
			//// DB SERVER IP
			////--------------------------------
			modCom.ReadInitProfile_C(ref strDb_ip, "DB SERVER", "IP");

			txtDb_Ip.Text = strDb_ip;

			////--------------------------------
			//// DB SERVER 서비스 네임즈
			////--------------------------------
			modCom.ReadInitProfile_C(ref strDb_Svc, "DB SERVER", "SERVICENAME");

			txtDb_Svc.Text = strDb_Svc;

			////--------------------------------
			//// DB SERVER USER
			////--------------------------------
			modCom.ReadInitProfile_C(ref strDb_User, "DB SERVER", "USER");

			txtDb_User.Text = strDb_User;

			////--------------------------------
			//// DB SERVER PASSWORD
			////--------------------------------
			modCom.ReadInitProfile_C(ref strDb_Pw, "DB SERVER", "PASSWORD");

			txtDb_Pw.Text = strDb_Pw;

		}

		private void but_Init_Click(object sender, EventArgs e)
		{
			////--------------------------------
			//// 초기화
			////--------------------------------

			{
				txtTitle.Text = "통합 창고 [WMS SYSTEM]";
				txtForm_Pic.Text = ".\\WmsDown.jpg";
				txtIcon.Text = ".\\WMS.ico";
				rdoSetup_2.Checked = true;
				rdoDownSt_1.Checked = true;
				rdoDownPath_1.Checked = true;
				rdoDownComp_1.Checked = true;

				StatusBar1.Text = "초기화 하였습니다.";
			}
		}

		private void btn_Save_Click(object sender, EventArgs e)
		{

			string strSet_Btn = "";
			string strDn_St = "";
			string strDn_Path = "";
			string strDn_Comp = "";

			////--------------------------------
			//// Title
			////--------------------------------
			modCom.WriteInitProfile("APPLICATION", "TITLE", txtTitle.Text.Trim());

			////--------------------------------
			//// Form Picture 적용
			////--------------------------------
			modCom.WriteInitProfile("APPLICATION", "FORM_PICTURE", txtForm_Pic.Text.Trim());

			////--------------------------------
			//// ICon 적용
			////--------------------------------
			modCom.WriteInitProfile("APPLICATION", "ICON", txtIcon.Text.Trim());

			////--------------------------------
			//// 셋업 버튼 적용
			////--------------------------------
			if (rdoSetup_1.Checked == true)
			{
				strSet_Btn = "1";
			}
			else
			{
				strSet_Btn = "2";
			}

			modCom.WriteInitProfile("APPLICATION", "SETUP_BTN", strSet_Btn);

			////--------------------------------
			//// DownLoad 시작 적용
			////--------------------------------
			if (rdoDownSt_1.Checked == true)
			{
				strDn_St = "1";
			}
			else
			{
				strDn_St = "2";
			}

			modCom.WriteInitProfile("APPLICATION", "DOWN_LOAD_START", strDn_St);

			////--------------------------------
			//// 다운로드 path 적용
			////--------------------------------
			if (rdoDownPath_2.Checked == true)
			{
				strDn_Path = "2";
			}
			else if (rdoDownPath_3.Checked == true)
			{
				strDn_Path = "3";
			}
			else
			{
				strDn_Path = "1";
			}

			modCom.WriteInitProfile("APPLICATION", "DOWN_LOAD_PATH_SET", strDn_Path);

			////--------------------------------
			//// DownLoad 파일 비교
			////--------------------------------
			if (rdoDownComp_1.Checked == true)
			{
				strDn_Comp = "1";
			}
			else
			{
				strDn_Comp = "2";
			}

			modCom.WriteInitProfile("APPLICATION", "DOWN_LOAD_COMP", strDn_Comp);

			////--------------------------------
			//// 상태바 표시
			////--------------------------------
			StatusBar1.Text = "저장하였습니다.";
		}

		private void btnForm_Pic_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string strName = null;

			OpenFileDialog1.FileName = txtForm_Pic.Text;


			if (OpenFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				strName = OpenFileDialog1.FileName.ToString();

				iPos = Strings.InStrRev(strName, "\\");
				if (iPos <= 0)
				{
					MessageBox.Show("FILE 에러입니다.", "그림파일 선택", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				txtForm_Pic.Text = ".\\" + Strings.Right(strName, Strings.Len(strName) - iPos);
			}

		}

		private void btnIcon_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string strName = null;

			OpenFileDialog2.FileName = txtIcon.Text;


			if (OpenFileDialog2.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				strName = OpenFileDialog2.FileName.ToString();

				iPos = Strings.InStrRev(strName, "\\");
				if (iPos <= 0)
				{
					MessageBox.Show("FILE 에러입니다.", "그림파일 선택", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				txtIcon.Text = ".\\" + Strings.Right(strName, Strings.Len(strName) - iPos);
			}

		}

		private void but_Init2_Click(object sender, EventArgs e)
		{
			////--------------------------------
			//// 초기화
			////--------------------------------

			{
				txtFile1.Text = ".\\WMS.EXE";
				txtFile2.Text = "";
				txtFile3.Text = "";
				txtFile4.Text = "";
				txtFile5.Text = "";

				txtDb_Ip.Text = "";
				txtDb_Svc.Text = "";
				txtDb_User.Text = "";
				txtDb_Pw.Text = "";
			}
		}

		private void TabControl1_SelectedIndexChanged(object sender, EventArgs e)
		{
			StatusBar1.Text = "";
		}

		private void btnFile1_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string sName = null;

			OpenFileDialog3.FileName = txtFile1.Text;


			if (OpenFileDialog3.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				sName = OpenFileDialog3.FileName.ToString();

				iPos = Strings.InStrRev(sName, "\\");
				if (iPos <= 0)
				{
					MessageBox.Show("FILE 에러입니다.", "실행파일 선택", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				txtFile1.Text = ".\\" + Strings.Right(sName, Strings.Len(sName) - iPos);
			}
		}

		private void btnFile2_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string sName = null;

			OpenFileDialog3.FileName = txtFile2.Text;


			if (OpenFileDialog3.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				sName = OpenFileDialog3.FileName.ToString();

				iPos = Strings.InStrRev(sName, "\\");
				if (iPos <= 0)
				{
					MessageBox.Show("FILE 에러입니다.", "실행파일 선택", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				txtFile1.Text = ".\\" + Strings.Right(sName, Strings.Len(sName) - iPos);
			}
		}

		private void btnFile3_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string sName = null;

			OpenFileDialog3.FileName = txtFile3.Text;


			if (OpenFileDialog3.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				sName = OpenFileDialog3.FileName.ToString();

				iPos = Strings.InStrRev(sName, "\\");
				if (iPos <= 0)
				{
					MessageBox.Show("FILE 에러입니다.", "실행파일 선택", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				txtFile1.Text = ".\\" + Strings.Right(sName, Strings.Len(sName) - iPos);
			}
		}

		private void btnFile4_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string sName = null;

			OpenFileDialog3.FileName = txtFile4.Text;


			if (OpenFileDialog3.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				sName = OpenFileDialog3.FileName.ToString();

				iPos = Strings.InStrRev(sName, "\\");
				if (iPos <= 0)
				{
					MessageBox.Show("FILE 에러입니다.", "실행파일 선택", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				txtFile1.Text = ".\\" + Strings.Right(sName, Strings.Len(sName) - iPos);
			}
		}

		private void btnFile5_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string sName = null;

			OpenFileDialog3.FileName = txtFile5.Text;


			if (OpenFileDialog3.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				sName = OpenFileDialog3.FileName.ToString();

				iPos = Strings.InStrRev(sName, "\\");
				if (iPos <= 0)
				{
					MessageBox.Show("FILE 에러입니다.", "실행파일 선택", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}

				txtFile1.Text = ".\\" + Strings.Right(sName, Strings.Len(sName) - iPos);
			}
		}

		private void btnSave_2_Click(object sender, EventArgs e)
		{

			////--------------------------------
			//// 실행파일 1
			////--------------------------------

			{
				modCom.WriteInitProfile("RUN_FILE", "FILE1", txtFile1.Text.Trim());

				////--------------------------------
				//// 실행파일 2
				////--------------------------------
				modCom.WriteInitProfile("RUN_FILE", "FILE2", txtFile2.Text.Trim());

				////--------------------------------
				//// 실행파일 3
				////--------------------------------
				modCom.WriteInitProfile("RUN_FILE", "FILE3", txtFile3.Text.Trim());

				////--------------------------------
				//// 실행파일 4
				////--------------------------------
				modCom.WriteInitProfile("RUN_FILE", "FILE4", txtFile4.Text.Trim());

				////--------------------------------
				//// 실행파일 5
				////--------------------------------
				modCom.WriteInitProfile("RUN_FILE", "FILE5", txtFile5.Text.Trim());

				////--------------------------------
				//// DB Server IP
				////--------------------------------
				modCom.WriteInitProfile("DB SERVER", "IP", txtDb_Ip.Text.Trim());
				modCom.WriteInitProfile("DB SERVER", "IP", txtDb_Ip.Text.Trim());

				////--------------------------------
				//// DB Server 서비스 네임즈
				////--------------------------------
				modCom.WriteInitProfile("DB SERVER", "SERVICENAME", txtDb_Svc.Text.Trim());
				modCom.WriteInitProfile("DB SERVER", "SERVICENAME", txtDb_Svc.Text.Trim());

				////--------------------------------
				//// DB Server USER
				////--------------------------------
				modCom.WriteInitProfile("DB SERVER", "USER", txtDb_User.Text.Trim());
				modCom.WriteInitProfile("DB SERVER", "USER", txtDb_User.Text.Trim());

				////--------------------------------
				//// DB Server PASSWORD
				////--------------------------------
				modCom.WriteInitProfile("DB SERVER", "PASSWORD", txtDb_Pw.Text.Trim());
				modCom.WriteInitProfile("DB SERVER", "PASSWORD", txtDb_Pw.Text.Trim());

				////--------------------------------
				//// 상태바 표시
				////--------------------------------
				StatusBar1.Text = "저장하였습니다.";
			}

		}
		
	}
}
