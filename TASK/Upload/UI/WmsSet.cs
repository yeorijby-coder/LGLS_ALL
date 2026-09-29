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
	public partial class WmsSet : Form
	{
		public WmsSet()
		{
			InitializeComponent();
		}

		private void WmsSet_Load(object sender, EventArgs e)
		{

			string strTitle = null;
			string strPico = null;
			string strSet_Btn = null;
			string strDn_St = null;
			string strDn_Comp = null;

			string strDb_Ip = null;
			string strDb_Svc = null;
			string strDb_User = null;
			string strDb_Pw = null;

			////--------------------------------
			//// 초기화
			////--------------------------------
			StatusLabel1.Text = "";

			////--------------------------------
			//// TITLE
			////--------------------------------
			strTitle = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strTitle, "APPLICATION", "TITLE");

			txtTitle.Text = strTitle;

			////--------------------------------
			//// DOWN_LOAD_PATH_FIX
			////--------------------------------
			strDn_St = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strDn_St, "APPLICATION", "DOWN_LOAD_PATH_FIX");

			txtDownFix.Text = strDn_St;

			////--------------------------------
			//// ICON 적용
			////--------------------------------
			strPico = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strPico, "APPLICATION", "ICON");

			if (!string.IsNullOrEmpty(FileSystem.Dir(strPico)))
			{
				// Create icon.
				if (strPico != "")
				{
					Icon newIcon = new Icon(strPico);

					this.Icon = newIcon;

					txtIcon.Text = strPico;
				}		
			}

			////--------------------------------
			//// 설정 버튼 적용
			////--------------------------------
			strSet_Btn = new string(' ', 100);
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
			//// UPLOAD키
			////--------------------------------
			strDn_St = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strDn_St, "APPLICATION", "UP_LOAD_KEY");

			if (strDn_St == "1")
			{
				rdoUpLoad_1.Checked = true;
			}
			else
			{
				rdoUpLoad_2.Checked = true;
			}

			////--------------------------------
			//// DOWNLOAD 파일 비교
			////--------------------------------
			strDn_Comp = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strDn_Comp, "APPLICATION", "DOWN_LOAD_PATH_SET");

			if (strDn_Comp == "1")
			{
				rdoDownPath_1.Checked = true;
			}
			else
			{
				rdoDownPath_2.Checked = true;
			}

			////--------------------------------
			//// DB SERVER IP
			////--------------------------------
			strDb_Ip = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strDb_Ip, "DB SERVER", "SERVERNAME");

			txtDb_Ip.Text = strDb_Ip;

			////--------------------------------
			//// DB Server 서비스 네임즈
			////--------------------------------
			strDb_Svc = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strDb_Svc, "DB SERVER", "DATABASE");

			txtDb_Svc.Text = strDb_Svc;

			////--------------------------------
			//// DB SERVER USER
			////--------------------------------
			strDb_User = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strDb_User, "DB SERVER", "USERID");

			txtDb_User.Text = strDb_User;

			////--------------------------------
			//// DB SERVER PASSWORD
			////--------------------------------
			strDb_Pw = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strDb_Pw, "DB SERVER", "PASSWORD");

			txtDb_Pw.Text = strDb_Pw;
		}

		private void but_Init_Click(object sender, EventArgs e)
		{

			txtTitle.Text = "통합 창고 시스템 [WMS] UpLoad현황";
			txtIcon.Text = ".\\WMS.ICO";

			////--------------------------------
			//// 설정 버튼 적용
			////--------------------------------
			rdoSetup_1.Checked = true;

			////--------------------------------
			//// UPLOAD키
			////--------------------------------
			rdoUpLoad_1.Checked = true;

			////--------------------------------
			//// DownLoad 파일 비교
			////--------------------------------
			rdoDownPath_1.Checked = true;

			////--------------------------------
			//// DOWN_LOAD_PATH_FIX 적용
			////--------------------------------
			txtDownFix.Text = "";

			////--------------------------------
			//// DB SERVER IP
			////--------------------------------
			txtDb_Ip.Text = "";

			////--------------------------------
			//// DB SERVER 서비스 네임즈
			////--------------------------------
			txtDb_Svc.Text = "";

			////--------------------------------
			//// DB SERVER USER
			////--------------------------------
			txtDb_User.Text = "";

			////--------------------------------
			//// DB SERVER PASSWORD
			////--------------------------------
			txtDb_Pw.Text = "";
		}

		private void btn_Save_Click(object sender, EventArgs e)
		{

			string strSet_Btn = "";
			string strDn_Path = "";
			string strUp_Key = "";

			////--------------------------------
			//// Title
			////--------------------------------
			modCom.WriteInitProfile_C("APPLICATION", "TITLE", txtTitle.Text.Trim());

			////--------------------------------
			//// ICon 적용
			////--------------------------------
			modCom.WriteInitProfile_C("APPLICATION", "ICON", txtIcon.Text.Trim());

			////--------------------------------
			//// DOWN_LOAD_PATH_FIX 적용
			////--------------------------------
			modCom.WriteInitProfile_C("APPLICATION", "DOWN_LOAD_PATH_FIX", txtDownFix.Text.Trim());

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

			modCom.WriteInitProfile_C("APPLICATION", "SETUP_BTN", strSet_Btn);

			////--------------------------------
			//// DownLoad 시작 적용
			////--------------------------------
			if (rdoUpLoad_1.Checked == true)
			{
				strUp_Key = "1";
			}
			else
			{
				strUp_Key = "2";
			}

			modCom.WriteInitProfile_C("APPLICATION", "UP_LOAD_KEY", strUp_Key);

			////--------------------------------
			//// DownLoad 경로 
			////--------------------------------
			if (rdoDownPath_1.Checked == true)
			{
				strDn_Path = "1";
			}
			else
			{
				strDn_Path = "2";
			}

			modCom.WriteInitProfile_C("APPLICATION", "DOWN_LOAD_PATH_SET", strDn_Path);

			////--------------------------------
			//// DB SERVER IP
			////--------------------------------
			modCom.WriteInitProfile_C("DB SERVER", "SERVERNAME", txtDb_Ip.Text.Trim());

			////--------------------------------
			//// DB SERVER 서비스 네임즈
			////--------------------------------
			modCom.WriteInitProfile_C("DB SERVER", "DATABASE", txtDb_Svc.Text.Trim());

			////--------------------------------
			//// DB SERVER IP
			////--------------------------------
			modCom.WriteInitProfile_C("DB SERVER", "USERID", txtDb_User.Text.Trim());

			////--------------------------------
			//// DB SERVER Port
			////--------------------------------
			modCom.WriteInitProfile_C("DB SERVER", "PASSWORD", txtDb_Pw.Text.Trim());


			////--------------------------------
			//// 상태바 표시
			////--------------------------------
			StatusLabel1.Text = "저장하였습니다.";
		}

		private void btnIcon_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string sName = null;

			OpenFileDialog2.FileName = txtIcon.Text;


			if (OpenFileDialog2.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				sName = OpenFileDialog2.FileName.ToString();

				iPos = Strings.InStrRev(sName, "\\");
				if (iPos <= 0)
				{
					modCmWork.ShowError("FILE 에러입니다.", "그림파일 선택");
					return;
				}

				txtIcon.Text = ".\\" + Strings.Right(sName, Strings.Len(sName) - iPos);
			}
		}

		private void btnDir_Click(object sender, EventArgs e)
		{
			int iPos = 0;
			string sName = null;

			OpenFileDialog1.InitialDirectory = txtDownFix.Text;


			if (OpenFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				sName = OpenFileDialog1.FileName.ToString();

				iPos = Strings.InStrRev(sName, "\\");
				if (iPos <= 0)
				{
					modCmWork.ShowError("FILE 에러입니다.", "그림파일 선택");
					return;
				}

				txtDownFix.Text = Strings.Left(sName, iPos - 1);
			}

		}


	}
}
