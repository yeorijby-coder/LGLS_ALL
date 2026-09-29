using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using System.IO;
using System.Diagnostics;
using System.Data.OleDb;
using System.Data.SqlClient;
using Npgsql;
using NpgsqlTypes;
using System.Runtime.InteropServices; //DllImport

namespace WmsUp
{

	//[DllImport("kernel32.dll")]
	//private static extern uint GetPrivateProfileString(string section,
	//											   string key,
	//											   string defaultValue, //키값이 없을 때의 기본 값
	//											   StringBuilder returnedString,
	//											   uint size,
	//											   string filePath);

	//[DllImport("kernel32.dll")]
	//private static extern bool WritePrivateProfileString(string section,
	//													 string key,
	//													 string value,
	//													 string filePath);

	public partial class FileUp : Form
	{
		[DllImport("kernel32")]
		public static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

		[DllImport("kernel32")]
		public static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

		public const string DOWN_INI = ".\\\\WmsDown.ini";
		public const string CONFIG_INI = ".\\\\Config.ini";

		public FileUp()
		{
			InitializeComponent();
		}

		//---------------------------------------------
		// 선언부
		//---------------------------------------------
		//트리 뷰에서 선택한 노드를 프로그래밍 방식으로 변경하고 있는지 여부를 나타냅니다.
		private bool m_blChangingSelectedNode;
		private bool m_blChangingListview;
		private string m_strSql;
		private int m_iSelcnt;

		private void FileUp_Load(object sender, EventArgs e)
		{
			string strPico = null;
			string str_p_dwn = null;
			string str_pi_dwn = null;
			int i = 0;

			StatusLabel1.Text = "";
			StatusLabel2.Text = modCom.g_strPcNm;

			////--------------------------------
			//// ICon 적용
			////--------------------------------
			strPico = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strPico, "APPLICATION", "ICON");

			if (!string.IsNullOrEmpty(FileSystem.Dir(strPico)))
			{
				// Create icon.
				Icon newIcon = new Icon(strPico);

				this.Icon = newIcon;
			}

			////--------------------------------
			//// 콤보 박스 적용
			////--------------------------------
			str_pi_dwn = new string(' ', 100);
			modCom.ReadInitProfile_C(ref str_pi_dwn, "DOWNLOAD PROGRAM", "CNT");


			for (i = 1; i <= Convert.ToInt32(str_pi_dwn); i++)
			{
				str_p_dwn = new string(' ', 100);
				modCom.ReadInitProfile_C(ref str_p_dwn, "DOWNLOAD PROGRAM", i.ToString());

				ComboBox1.Items.Add(str_p_dwn);
			}

			if (ComboBox1.Items.Count > 0)
				ComboBox1.SelectedIndex = 0;

			m_blChangingSelectedNode = true;
			m_blChangingListview = true;

			//UI를 설정합니다.
			SetUpListViewColumns();

			ShowDrives();
		}

		//---------------------------------------------
		// 컨트롤 이벤트
		//---------------------------------------------
		private void tvw_AfterExpand(object sender, TreeViewEventArgs e)
		{

			m_blChangingSelectedNode = true;

			if (tvw.SelectedNode.Text == "내 컴퓨터")
			{
				ShowDrives();
			}

			if (e.Node.Text == "내 컴퓨터")
			{
				FillFoldersInTree(tvw.SelectedNode);
			}
			else
			{
				FillFoldersInTree(e.Node);
			}

			if (!m_blChangingSelectedNode)
				return;
		}

		private void tvw_AfterSelect(object sender, TreeViewEventArgs e)
		{
			// TODO: 트리 뷰에서 현재 선택한 노드에 따라 목록 뷰 내용을 변경하는 코드를 추가합니다.

			{
				m_blChangingSelectedNode = true;

				LoadListView_folder(tvw.SelectedNode);
				LoadListView_file(tvw.SelectedNode);
			}
		}

		private void lvw_DoubleClick(object sender, EventArgs e)
		{
			ListView.SelectedListViewItemCollection collectFileDir = lvw.SelectedItems;
			//ListViewItem collectitem = default(ListViewItem);

			foreach (ListViewItem collectitem1 in collectFileDir)
			{
				if (Strings.Left(collectitem1.SubItems[3].Text, 1) == "D")
				{
					string SELECTITEM = lvw.FocusedItem.Text;
					//NODE 펼침
					FillFoldersInTree(tvw.SelectedNode, SELECTITEM);
					//LVW 에 파일 선택
					FillFoldersInTree(tvw.SelectedNode, SELECTITEM);
				}
			}
		}

		private void lvw_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
		{
			
			ListView.SelectedListViewItemCollection collectFileDir = lvw.SelectedItems;
			//ListViewItem collectitem = default(ListViewItem);
			int i = 0;
			
			foreach ( ListViewItem collectitem in collectFileDir) {
			
				if (Strings.Left(collectitem.SubItems[3].Text, 1) == "D") {
					//FillFoldersInTree(tvw.SelectedNode, e.Item.Text)
			
				}else {
					for (i = 0; i <= ListBox1.Items.Count - 1; i++) {
						if (e.Item.Text == ListBox1.Items[i].ToString()) {
							return;
						}
					}
					ListBox1.Items.AddRange(new string[] {
						e.Item.Text,
						Strings.Mid(tvw.SelectedNode.FullPath, 7)
					});
				}
			}
		}

		private void ListBox1_DoubleClick(object sender, EventArgs e)
		{
			int i_add = 0;

			try
			{

				if (ListBox1.SelectedIndex % 2 == 0)
				{
					i_add = 1;
				}
				else
				{
					i_add = -1;
				}

				ListBox1.Items.RemoveAt(ListBox1.SelectedIndex + i_add);
				ListBox1.Items.RemoveAt(ListBox1.SelectedIndex);

				ListBox1.Refresh();

			}
			catch (Exception ex)
			{
			}
		}

		private void btnDel_Click(object sender, EventArgs e)
		{
			ListBox1_DoubleClick(sender, e);
		}
		private void ExitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//응용 프로그램을 끝냅니다.
			System.Windows.Forms.Application.Exit();
		}
		private void ToolBarToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//ToolStrip을 표시하거나 숨기고, 연결된 메뉴 항목의 선택 상태를 전환합니다.
			ToolBarToolStripMenuItem.Checked = !ToolBarToolStripMenuItem.Checked;
			ToolStrip.Visible = ToolBarToolStripMenuItem.Checked;
		}
		private void StatusBarToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//StatusStrip을 표시하거나 숨기고, 연결된 메뉴 항목의 선택 상태를 전환합니다.
			StatusBarToolStripMenuItem.Checked = !StatusBarToolStripMenuItem.Checked;
			StatusStrip.Visible = StatusBarToolStripMenuItem.Checked;
		}
		private void FoldersToolStripButton_Click(object sender, EventArgs e)
		{
			ToggleFoldersVisible();
		}
		private void FoldersToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ToggleFoldersVisible();
		}
		private void ListToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SetView(View.List);
		}
		private void DetailsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SetView(View.Details);
		}
		private void LargeIconsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SetView(View.LargeIcon);
		}
		private void SmallIconsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SetView(View.SmallIcon);
		}
		private void TileToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SetView(View.Tile);
		}

		private void btnUpload_Click(object sender, EventArgs e)
		{

			Path sForm = new Path();
			string s_Ret = ".";

			sForm.ShowDialog();

			if (sForm.psYN == "N")
				return;

			s_Ret = sForm.psRet;

			sForm.Dispose();

			StatusLabel1.Text = "서버에 연결중...";

			Application.DoEvents();

			gconDbinit();

			StatusLabel1.Text = "서버 연결완료...";
			StatusLabel2.Text = modCom.g_strPcNm;
			StatusStrip.Refresh();

			updatedata(s_Ret);

			ListBox1.Items.Clear();

			Application.DoEvents();

			StatusLabel1.Text = "데이타를 업로드 했습니다...";
		}
		//---------------------------------------------
		// 함수
		//---------------------------------------------

		//Db연결을 한다.
		public void gconDbinit()
		{
			//DB 서버 접속
			if (modCom.gconDb == null)
			{
				if (!modCom.DBLogIn(ref modCom.gconDb))
				{
					//DB서버 접속 실패시 프로그램을 종료한다.
					System.Environment.Exit(0);
				}
			}
			else
			{
				if (modCom.gconDb.State == ConnectionState.Closed)
				{
					if (!modCom.DBLogIn(ref modCom.gconDb))
					{
						//DB서버 접속 실패시 프로그램을 종료한다.
						System.Environment.Exit(0);
					}
				}
			}
		}	

		private void ShowDrives()
		{
			TreeNode tvRoot = default(TreeNode);
			TreeNode tvNode = default(TreeNode);

			string []dirs = Environment.GetLogicalDrives();
		
			this.tvw.Nodes.Clear();
		
			tvRoot = this.tvw.Nodes.Add("내 컴퓨터");
		
			foreach (string dir1 in dirs) {
				try {
					tvNode = tvRoot.Nodes.Add(Strings.Left(dir1, 2));
		
					if (dir1 == "C:\\") {
						m_blChangingSelectedNode = false;
		
						tvw.SelectedNode = tvNode;
						txtFolder.Text = "내 컴퓨터\\" + dir1;
					}
		
					Application.DoEvents();
				} catch (Exception ex) {
					continue;
				}
			}
		}

		private void FillFoldersInTree(TreeNode Node, string sFolder = "")
		{
			string strAbsoluteAddress = null;

			try
			{
				Node.Nodes.Clear();

				if (Node.Text == "내 컴퓨터")
				{
					strAbsoluteAddress = "C:\\";
				}
				else
				{
					if (Node.Parent.Text == "내 컴퓨터")
					{
						strAbsoluteAddress = Node.Text + "\\";
					}
					else
					{
						strAbsoluteAddress = Strings.Mid(Node.FullPath, 7);
					}
				}



				DirectoryInfo dir1 = new DirectoryInfo(strAbsoluteAddress);

				DirectoryInfo[] subDirs = dir1.GetDirectories();


				if (subDirs.Length == 0)
				{
				}
				else
				{

					foreach (DirectoryInfo subDir in subDirs)
					{
						try
						{
							if (subDir.Name == "System Volume Information" | subDir.Name == "RRbackups")
							{
							}
							else
							{
								TreeNode current = Node.Nodes.Add(subDir.Name);

								if ((subDir.GetDirectories().Length > 0))
								{
									current.Nodes.Add("");
								}

								if (subDir.Name == sFolder)
								{
									tvw.SelectedNode = current;

									txtFolder.Text = tvw.SelectedNode.FullPath;
								}
							}
						}
						catch (Exception ex)
						{
							continue;
						}
					}
				}


			}
			catch (IOException e)
			{

			}
			catch (Exception e)
			{
				MessageBox.Show(e.Message);
			}
		}

		private void LoadListView_folder(TreeNode Node, string sFolder = "")
		{
			string strAbsoluteAddress = null;
			string strAttrib = null;

			try
			{
				strAbsoluteAddress = Strings.Mid(Node.FullPath, 7) + "\\" + sFolder;

				DirectoryInfo dir1 = new DirectoryInfo(strAbsoluteAddress);

				lvw.Items.Clear();

				DirectoryInfo[] subDirs = dir1.GetDirectories();

				ListViewItem lvItem = default(ListViewItem);

				foreach (DirectoryInfo subDir in subDirs)
				{
					try
					{
						lvItem = lvw.Items.Add(subDir.Name, 0);

						strAttrib = (((FileAttribute)subDir.Attributes & Constants.vbDirectory) == Constants.vbDirectory ? "D" : "");
						strAttrib += (((FileAttribute)subDir.Attributes & Constants.vbArchive) == Constants.vbArchive ? "A" : "");
						strAttrib += (((FileAttribute)subDir.Attributes & Constants.vbReadOnly) == Constants.vbReadOnly ? "R" : "");
						strAttrib += (((FileAttribute)subDir.Attributes & Constants.vbHidden) == Constants.vbHidden ? "H" : "");
						strAttrib += (((FileAttribute)subDir.Attributes & Constants.vbSystem) == Constants.vbSystem ? "S" : "");

						lvItem.SubItems.AddRange(new string[] {
					"",
					subDir.CreationTime.ToString(),
					strAttrib
				});
					}
					catch (Exception ex)
					{
						continue;
					}
				}


			}
			catch (IOException e)
			{

			}
			catch (Exception e)
			{
				MessageBox.Show(e.Message);
			}
		}



		private void LoadListView_file(TreeNode Node, string sFolder = "")
		{
			string strAbsoluteAddress = null;
			string strAttrib = null;

			try
			{
				strAbsoluteAddress = Strings.Mid(Node.FullPath, 7) + "\\" + sFolder;

				DirectoryInfo dir1 = new DirectoryInfo(strAbsoluteAddress);

				ListViewItem lvItem = default(ListViewItem);

				FileInfo[] files = dir1.GetFiles();

				foreach (FileInfo file in files)
				{
					try
					{
						lvItem = lvw.Items.Add(file.Name, 2);

						strAttrib = (((FileAttribute)file.Attributes & Constants.vbDirectory) == Constants.vbDirectory ? "D" : "");
						strAttrib += (((FileAttribute)file.Attributes & Constants.vbArchive) == Constants.vbArchive ? "A" : "");
						strAttrib += (((FileAttribute)file.Attributes & Constants.vbReadOnly) == Constants.vbReadOnly ? "R" : "");
						strAttrib += (((FileAttribute)file.Attributes & Constants.vbHidden) == Constants.vbHidden ? "H" : "");
						strAttrib += (((FileAttribute)file.Attributes & Constants.vbSystem) == Constants.vbSystem ? "S" : "");

						lvItem.SubItems.AddRange(new string[] {
					FormatSize(file.Length),
					file.CreationTime.ToString(),
					strAttrib
				});
					}
					catch (Exception ex)
					{
						continue;
					}
				}

			}
			catch (IOException e)
			{

			}
			catch (Exception e)
			{
				MessageBox.Show(e.Message);
			}
		}

		private string FormatSize(long size)
		{

			double dblFsize = Convert.ToDouble(size);
			if ((dblFsize < 1024))
			{
				return size.ToString() + " bytes";
			}
			else
			{

				if ((dblFsize < 1048576))
				{
					dblFsize = dblFsize / 1024;
					dblFsize = System.Math.Round(dblFsize, 2);
					return dblFsize.ToString() + " KiloBytes";

				}
				else
				{

					if ((dblFsize < 1073741824))
					{
						dblFsize = dblFsize / 1048576;
						dblFsize = System.Math.Round(dblFsize, 2);
						return dblFsize.ToString() + " MegaBytes";

					}
					else
					{

						if ((dblFsize < 1099511627776L))
						{
							dblFsize = dblFsize / 1073741824;
							dblFsize = System.Math.Round(dblFsize, 2);
							return dblFsize.ToString() + " GigaBytes";

						}
						else
						{
							return size.ToString() + "bytes";
						}
					}
				}
			}
		}

		private void SetUpListViewColumns()
		{
			// TODO: listview 열을 설정하는 코드를 추가합니다.
			lvw.Columns.Add("이름", 100);
			lvw.Columns.Add("파일 크기", 100);
			lvw.Columns.Add("생성 일자", 100);
			lvw.Columns.Add("파일 속성", 40);
			SetView(View.Details);
		}


		//폴더 창의 표시 여부를 변경합니다.
		private void ToggleFoldersVisible()
		{
			//먼저, 연결된 메뉴 항목의 선택 상태를 전환합니다.
			FoldersToolStripMenuItem.Checked = !FoldersToolStripMenuItem.Checked;

			//폴더 도구 모음 단추가 동기화되도록 변경합니다.
			FoldersToolStripButton.Checked = FoldersToolStripMenuItem.Checked;

			// TreeView가 들어 있는 창을 축소합니다.
			this.SplitContainer.Panel1Collapsed = !FoldersToolStripMenuItem.Checked;
		}

		private void SetView(System.Windows.Forms.View View)
		{
			//선택되어야 하는 메뉴 항목을 확인합니다.
			ToolStripMenuItem MenuItemToCheck = null;
			switch (View)
			{
				case View.Details:
					MenuItemToCheck = DetailsToolStripMenuItem;
					break;
				case View.LargeIcon:
					MenuItemToCheck = LargeIconsToolStripMenuItem;
					break;
				case View.List:
					MenuItemToCheck = ListToolStripMenuItem;
					break;
				case View.SmallIcon:
					MenuItemToCheck = SmallIconsToolStripMenuItem;
					break;
				case View.Tile:
					MenuItemToCheck = TileToolStripMenuItem;
					break;
				default:
					Debug.Fail("예기치 않은 보기");
					View = View.Details;
					MenuItemToCheck = DetailsToolStripMenuItem;
					break;
			}

			//적절한 메뉴 항목을 선택하고 [보기] 메뉴에서 다른 모든 항목의 선택을 취소합니다.
			foreach (ToolStripMenuItem MenuItem in ListViewToolStripButton.DropDownItems)
			{
				if (object.ReferenceEquals(MenuItem, MenuItemToCheck))
				{
					MenuItem.Checked = true;
				}
				else
				{
					MenuItem.Checked = false;
				}
			}

			//마지막으로, 요청한 보기를 설정합니다.
			lvw.View = View;
		}

		private void updatedata(string p_strRet)
		{
			////use filestream object to read the image.
			////read to the full length of image to a byte array.
			////add this byte as an oracle parameter and insert it into database.
			int i_lst = 0;
			string strlst = null;
			string strdir = null;
		
			int iDN_NO = 0;
			int strDN_VER = 0;
			int strDN_LN = 1;
		
			////open the database using odp.net and insert the data
			CUserDb Bdb = new CUserDb();
#if oracle
			OleDbCommand cmnd = default(OleDbCommand);
#elif mssql
			SqlCommand cmnd = default(SqlCommand);
#elif postgresql
			NpgsqlCommand cmnd = default(NpgsqlCommand);
#endif
            ProgressBar1.Visible = true;
		
			try {
				Bdb.BeginTrans();
		
#if oracle
				m_strSql = "";
				m_strSql += modCom.CRLF + "        SELECT DN_SEQ.NEXTVAL AS DN_SEQ       ";
				m_strSql += modCom.CRLF + "          FROM DUAL                           ";
#elif mssql
				m_strSql = "";
				// [LGLS 2026-09-29] SEQUENCE 는 SQL Server 2012 부터다. 현장(2008)에서는 1행 표로 채번한다.
				//   UPDATE 한 문장 안에서 올리고 받아 오므로 둘이 동시에 눌러도 겹치지 않는다.
				m_strSql += modCom.CRLF + "  UPDATE [dbo].[DN_SEQ] SET SEQ_NO = CASE WHEN SEQ_NO >= 9999 THEN 1 ELSE SEQ_NO + 1 END ";
				m_strSql += modCom.CRLF + "  SELECT SEQ_NO AS DN_SEQ FROM [dbo].[DN_SEQ] ";
#elif postgresql
				m_strSql = "";
				m_strSql += modCom.CRLF + "SELECT NEXTVAL('DN_SEQ') AS DN_SEQ "; //
#endif
                m_iSelcnt = Bdb.ExcuteQry(m_strSql);
		
				if (m_iSelcnt < 0) {
					throw new Exception("");
				} else if (m_iSelcnt > 0) {
					iDN_NO = Convert.ToInt32(Bdb.dtMain.Rows[0]["DN_SEQ"].ToString());
				}
#if oracle
				m_strSql = "";
				m_strSql += modCom.CRLF + "   INSERT INTO DN_MST                                                                   ";
				m_strSql += modCom.CRLF + "             (                                                                          ";
				m_strSql += modCom.CRLF + "               DN_NO                                                                    ";
				m_strSql += modCom.CRLF + "             , DN_PGM                                                                   ";
				m_strSql += modCom.CRLF + "             , DN_DIR                                                                   ";
				m_strSql += modCom.CRLF + "             , DN_INF                                                                   ";
				m_strSql += modCom.CRLF + "             , UP_CPT_NM                                                                ";
				m_strSql += modCom.CRLF + "             , UP_DNT                                                                   ";
				m_strSql += modCom.CRLF + "             )                                                                          ";
				m_strSql += modCom.CRLF + "        VALUES                                                                          ";
				m_strSql += modCom.CRLF + "             (                                                                          ";
				m_strSql += modCom.CRLF + "               " + iDN_NO + "                                                           ";
				m_strSql += modCom.CRLF + "             , '" + ComboBox1.Text + "'                                                 ";
				m_strSql += modCom.CRLF + "             , '" + p_strRet + "'                                                       ";
				m_strSql += modCom.CRLF + "             , '" + txtDN_INF.Text + "'                                                 ";
				m_strSql += modCom.CRLF + "             , '" + Environment.MachineName + "'                                        ";
				m_strSql += modCom.CRLF + "             , TO_DATE('" + Strings.Format(DateTime.Now , "yyyyMMddHHmmss") + "', 'YYYYMMDDhh24miss')     ";
				m_strSql += modCom.CRLF + "             )                                                                          ";
#elif mssql
				m_strSql = "";
				m_strSql += modCom.CRLF + "   INSERT INTO DN_MST                                                                   ";
				m_strSql += modCom.CRLF + "             (                                                                          ";
				m_strSql += modCom.CRLF + "               DN_NO                                                                    ";
				m_strSql += modCom.CRLF + "             , DN_PGM                                                                   ";
				m_strSql += modCom.CRLF + "             , DN_DIR                                                                   ";
				m_strSql += modCom.CRLF + "             , DN_INF                                                                   ";
				m_strSql += modCom.CRLF + "             , UP_CPT_NM                                                                ";
				m_strSql += modCom.CRLF + "             , UP_DNT                                                                   ";
				m_strSql += modCom.CRLF + "             )                                                                          ";
				m_strSql += modCom.CRLF + "        VALUES                                                                          ";
				m_strSql += modCom.CRLF + "             (                                                                          ";
				m_strSql += modCom.CRLF + "               " + iDN_NO + "                                                           ";
				m_strSql += modCom.CRLF + "             , '" + ComboBox1.Text + "'                                                 ";
				m_strSql += modCom.CRLF + "             , '" + p_strRet + "'                                                       ";
				m_strSql += modCom.CRLF + "             , '" + txtDN_INF.Text + "'                                                 ";
				m_strSql += modCom.CRLF + "             , '" + Environment.MachineName + "'                                        ";
				m_strSql += modCom.CRLF + "             , GETDATE()                                                                ";
				m_strSql += modCom.CRLF + "             )                                                                          ";
#elif postgresql
				/*
				m_strSql = "";
				m_strSql += modCom.CRLF + "   INSERT INTO DN_MST                                                                   ";
				m_strSql += modCom.CRLF + "             (                                                                          ";
				m_strSql += modCom.CRLF + "               DN_NO                                                                    ";
				m_strSql += modCom.CRLF + "             , DN_PGM                                                                   ";
				m_strSql += modCom.CRLF + "             , DN_DIR                                                                   ";
				m_strSql += modCom.CRLF + "             , DN_INF                                                                   ";
				m_strSql += modCom.CRLF + "             , UP_CPT_NM                                                                ";
				m_strSql += modCom.CRLF + "             , UP_DNT                                                                   ";
				m_strSql += modCom.CRLF + "             )                                                                          ";
				m_strSql += modCom.CRLF + "        VALUES                                                                          ";
				m_strSql += modCom.CRLF + "             (                                                                          ";
				m_strSql += modCom.CRLF + "               " + iDN_NO + "                                                           ";
				m_strSql += modCom.CRLF + "             , '" + ComboBox1.Text + "'                                                 ";
				m_strSql += modCom.CRLF + "             , '" + p_strRet + "'                                                       ";
				m_strSql += modCom.CRLF + "             , '" + txtDN_INF.Text + "'                                                 ";
				m_strSql += modCom.CRLF + "             , '" + Environment.MachineName + "'                                        ";
				m_strSql += modCom.CRLF + "             , NOW() )                                                                   ";
// [LGLS 2026-09-30] 주석 안의 #endif 라도 ★쓰이지 않는 분기에서는 지시문으로 읽힌다★.
//   그래서 #if 짝이 어긋나 MS-SQL 빌드가 깨졌다. 지시문이 되지 않게 //# 로 둔다.
//#endif
				m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);
		
				if (m_iSelcnt <= 0) 
                {
					throw new Exception("업로드에 실패했습니다.");
				}
				//*/
#endif

				for (i_lst = 0; i_lst <= ListBox1.Items.Count - 1; i_lst += 2) {
					strlst = ListBox1.Items[i_lst].ToString();
					strdir = ListBox1.Items[i_lst + 1].ToString();
		
					if ((!string.IsNullOrEmpty(strlst))) {
						m_strSql = "";
						m_strSql += modCom.CRLF + "                 SELECT " + modDef.NVL + "(DN_VER, '0') AS DN_VER       ";
						m_strSql += modCom.CRLF + "                   FROM UP_DOWN                                 ";
						m_strSql += modCom.CRLF + "                  WHERE DN_NM = '" + strlst + "'               ";
						m_strSql += modCom.CRLF + "               ORDER BY DN_VER DESC                              ";
		
						m_iSelcnt = Bdb.ExcuteQry(m_strSql);
		
						if (m_iSelcnt < 0) {
							throw new Exception ("");
						}
		
						if (m_iSelcnt == 0) {
							strDN_VER = 1;
						} else if (m_iSelcnt > 0) {
							strDN_VER = Convert.ToInt32(int.Parse(Bdb.dtMain.Rows[0]["DN_VER"].ToString()) + 1);
						}
		
						Bdb.dtMain.Dispose();

#if oracle
						m_strSql = "";
						m_strSql += modCom.CRLF + "   INSERT INTO UP_DOWN                                             ";
						m_strSql += modCom.CRLF + "             (                                                    ";
						m_strSql += modCom.CRLF + "               DN_NO                                              ";
						m_strSql += modCom.CRLF + "             , DN_LN                                              ";
						m_strSql += modCom.CRLF + "             , UP_FNM                                             ";
						m_strSql += modCom.CRLF + "             , DN_NM                                              ";
						m_strSql += modCom.CRLF + "             , DN_SIZE                                            ";
						m_strSql += modCom.CRLF + "             , DN_VER                                             ";
						m_strSql += modCom.CRLF + "             , UP_DAT                                             ";
						m_strSql += modCom.CRLF + "             )                                                    ";
						m_strSql += modCom.CRLF + "        VALUES                                                    ";
						m_strSql += modCom.CRLF + "             (                                                    ";
						m_strSql += modCom.CRLF + "               " + iDN_NO + "                                     ";
						m_strSql += modCom.CRLF + "             , " + strDN_LN + "                                   ";
						m_strSql += modCom.CRLF + "             , '" + strdir + "\\" + strlst + "'                    ";
						m_strSql += modCom.CRLF + "             , '" + strlst + "'                                   ";
						m_strSql += modCom.CRLF + "             , :FileLen                                           ";
						m_strSql += modCom.CRLF + "             , " + strDN_VER + "                                  ";
						m_strSql += modCom.CRLF + "             , :BlobParameter                                     ";
						m_strSql += modCom.CRLF + "             )                                                    ";
		
						//// Command객체를 생성했으니, 이제 Parameter를 추가하자.
		
						////a byte array to read the image 
						byte[] File_Data = GetFile_Data(strdir + "\\" + strlst);
		
						var _with1 = Bdb.comMain.Parameters;
						_with1.Clear();
						_with1.Add("FileLen", OleDbType.Decimal, 10, "DN_SIZE").Value = File_Data.Length;
						_with1.Add("BlobParameter", OleDbType.LongVarBinary, File_Data.Length, "UP_DAT").Value = File_Data;
		
						//cmnd = New OleDbCommand(m_strSql, gconDb)
		
		
						//cmnd.Parameters.Add("FileLen", OleDbType.Decimal, 10, "DN_SIZE").Value = File_Data.Length
						//cmnd.Parameters.Add("BlobParameter", OleDbType.LongVarBinary, File_Data.Length, "UP_DAT").Value = File_Data
#elif mssql
						//--------------------------------------------------------------------------------------------------------------------
						// 20170831 KMS
						// 롯데첨단소재 수정.
						// INSERT [UP_DOWN]
						//--------------------------------------------------------------------------------------------------------------------

						// [LGLS 2026-09-29] Download 가 읽는 컬럼 세 개(DN_PGM/UP_DT/DN_DIR)를 함께 넣는다.
						//   종전 MS-SQL 판은 이 셋을 넣지 않아, 내려받을 때 폴더를 알 수 없었다.
						StringBuilder sbDnPath = new StringBuilder(256);
						GetPrivateProfileString("DOWN_FILE", "PATH", "", sbDnPath, sbDnPath.Capacity, CONFIG_INI);
						string strDownPathM = sbDnPath.ToString();

						m_strSql = "";
						m_strSql += modCom.CRLF + "    INSERT INTO UP_DOWN                                                ";
						m_strSql += modCom.CRLF + "              (                                                       ";
						m_strSql += modCom.CRLF + "                DN_NO                                                 ";
						m_strSql += modCom.CRLF + "              , DN_LN                                                 ";
						m_strSql += modCom.CRLF + "              , UP_FNM                                                ";
						m_strSql += modCom.CRLF + "              , DN_NM                                                 ";
						m_strSql += modCom.CRLF + "              , DN_SIZE                                               ";
						m_strSql += modCom.CRLF + "              , DN_VER                                                ";
						m_strSql += modCom.CRLF + "              , UP_DAT                                                ";
						m_strSql += modCom.CRLF + "              , DN_PGM                                                ";
						m_strSql += modCom.CRLF + "              , UP_DT                                                 ";
						m_strSql += modCom.CRLF + "              , DN_DIR                                                ";
						m_strSql += modCom.CRLF + "              )                                                       ";
						m_strSql += modCom.CRLF + "         VALUES                                                       ";
						m_strSql += modCom.CRLF + "              (                                                       ";
						m_strSql += modCom.CRLF + "                " + iDN_NO + "                                        ";
						m_strSql += modCom.CRLF + "              , " + strDN_LN + "                                      ";
						m_strSql += modCom.CRLF + "              , '" + strdir + "\\" + strlst + "'                       ";
						m_strSql += modCom.CRLF + "              , '" + strlst + "'                                      ";
						m_strSql += modCom.CRLF + "              , @FileLen                                              ";
						m_strSql += modCom.CRLF + "              , " + strDN_VER + "                                     ";
						m_strSql += modCom.CRLF + "              , @BlobParameter                                        ";
						m_strSql += modCom.CRLF + "              , 'COMMON'                                              ";
						m_strSql += modCom.CRLF + "              , GETDATE()                                             ";
						m_strSql += modCom.CRLF + "              , '" + strDownPathM + "'                                ";
						m_strSql += modCom.CRLF + "              )                                                       ";

						//// Command객체를 생성했으니, 이제 Parameter를 추가하자.

						////a byte array to read the image 
						byte[] File_Data = GetFile_Data(strdir + "\\" + strlst);

						cmnd = new SqlCommand(m_strSql, modCom.gconDb);

						var _with2 = Bdb.comMain.Parameters;
						_with2.Clear();
						_with2.Add("FileLen", SqlDbType.Int, 10, "DN_SIZE").Value = File_Data.Length;
						_with2.Add("BlobParameter", SqlDbType.VarBinary, File_Data.Length, "UP_DAT").Value = File_Data;
						//.Add("FileLen", OleDbType.Decimal, 10, "DN_SIZE").Value = File_Data.Length
						//.Add("BlobParameter", OleDbType.LongVarBinary, File_Data.Length, "UP_DAT").Value = File_Data

						//cmnd.Parameters.Add("@FileLen", SqlDbType.Int, 10, "DN_SIZE").Value = File_Data.Length
						//cmnd.Parameters.Add("@BlobParameter", SqlDbType.VarBinary, File_Data.Length, "UP_DAT").Value = File_Data
#elif postgresql
						//public static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

						//public const string DOWN_INI = ".\\\\WmsDown.ini";
						StringBuilder sb = new StringBuilder(30);
						GetPrivateProfileString("DOWN_FILE", "PATH", "", sb, sb.Capacity, CONFIG_INI);
						string strDownPath = sb.ToString();

						m_strSql = "";
						m_strSql += modCom.CRLF + "   INSERT INTO  UP_DOWN                                           ";   // UP_DOWN
						m_strSql += modCom.CRLF + "             (                                                    ";
						m_strSql += modCom.CRLF + "               DN_NO                                              ";
						m_strSql += modCom.CRLF + "             , DN_LN                                              ";
						m_strSql += modCom.CRLF + "             , UP_FNM                                             ";
						m_strSql += modCom.CRLF + "             , DN_NM                                              ";
						m_strSql += modCom.CRLF + "             , DN_SIZE                                            ";
						m_strSql += modCom.CRLF + "             , DN_VER                                             ";
						m_strSql += modCom.CRLF + "             , UP_DAT                                             ";
						m_strSql += modCom.CRLF + "             , DN_PGM                                             ";
						m_strSql += modCom.CRLF + "             , UP_DT                                              ";
						m_strSql += modCom.CRLF + "             , DN_DIR                                             ";
						m_strSql += modCom.CRLF + "             )                                                    ";
						m_strSql += modCom.CRLF + "        VALUES                                                    ";
						m_strSql += modCom.CRLF + "             (                                                    ";
						m_strSql += modCom.CRLF + "               " + iDN_NO + "                                     ";
						m_strSql += modCom.CRLF + "             , " + strDN_LN + "                                   ";
						m_strSql += modCom.CRLF + "             , '" + strdir + "\\" + strlst + "'                   ";
						m_strSql += modCom.CRLF + "             , '" + strlst + "'                                   ";
						m_strSql += modCom.CRLF + "             , :FileLen                                           ";
						m_strSql += modCom.CRLF + "             , " + strDN_VER + "                                  ";
						m_strSql += modCom.CRLF + "             , :BlobParameter                                     ";
						m_strSql += modCom.CRLF + "             , 'COMMON'											 ";
						m_strSql += modCom.CRLF + "             , " + modDateTime.SYSDATE + "						 ";
						m_strSql += modCom.CRLF + "             , '" + strDownPath + "'								 ";
						m_strSql += modCom.CRLF + "             )                                                    ";
		
						//// Command객체를 생성했으니, 이제 Parameter를 추가하자.
		
						////a byte array to read the image 
						byte[] File_Data = GetFile_Data(strdir + "\\" + strlst);
		
						var _with1 = Bdb.comMain.Parameters;
						_with1.Clear();
						_with1.Add("FileLen", NpgsqlDbType.Integer, 10, "DN_SIZE").Value = File_Data.Length;
						_with1.Add("BlobParameter", NpgsqlDbType.Bytea, File_Data.Length, "UP_DAT").Value = File_Data;		
#endif

                        m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);
						Bdb.dtMain.Dispose();
						//cmnd.ExecuteNonQuery()
						//cmnd.Dispose()








						//string filePath = Path.Combine(Environment.CurrentDirectory, "test.ini");

						//파일이 없다면 만들어줍니다. 해당 프로젝트 Debug 폴더에 위치합니다.
						if (File.Exists(DOWN_INI) == false)
						{
							File.Create(DOWN_INI);
						}

						WritePrivateProfileString("DOWN_FILE", strlst, strDN_VER.ToString(), DOWN_INI); //Section1 전체가 사라집니다.
					}
					else {
						throw new Exception("");
					}

					strDN_LN = strDN_LN + 1;
				}

                //// 오래된 자료를 삭제한다. 
                //{
                //    m_strSql = "";
                //    //m_strSql += modCom.CRLF + "  SELECT UD.*";
                //    m_strSql += modCom.CRLF + "  DELETE										";
                //    m_strSql += modCom.CRLF + "  FROM UP_DOWN UD							";
                //    m_strSql += modCom.CRLF + "  LEFT JOIN (								";
                //    m_strSql += modCom.CRLF + "      SELECT MAX(DN_VER) AS DN_VER, DN_NM	";
                //    m_strSql += modCom.CRLF + "      FROM UP_DOWN							";
                //    m_strSql += modCom.CRLF + "      WHERE DN_PGM = 'COMMON'				";
                //    m_strSql += modCom.CRLF + "      GROUP BY DN_NM							";
                //    m_strSql += modCom.CRLF + "  ) A										";
                //    m_strSql += modCom.CRLF + "  ON UD.DN_VER <> A.DN_VER					";
                //    m_strSql += modCom.CRLF + "  AND UD.DN_NM = A.DN_NM						";
                //    m_strSql += modCom.CRLF + "  AND UD.DN_PGM = 'COMMON'					";
                //    m_strSql += modCom.CRLF + "  WHERE A.DN_NM IS not NULL;					";
                //    m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);

                //    if (m_iSelcnt <= 0)
                //    {
                //        throw new Exception("");
                //    }
                //}

                ProgressBar1.Visible = false;
		
				Bdb.CommitTrans();
		
				modCom.gconDb.Close();
				modCom.gconDb.Dispose();
			} catch (Exception ex) {
				Bdb.RollbackTrans();
				ProgressBar1.Visible = false;
		
				modCom.gconDb.Close();
				modCom.gconDb.Dispose();
				return;
			}
		
		}

		//Shared 클래스 내에서만 사용하겠다. 인스턴스 생성에서는 접근 불가..
		public static byte[] GetFile_Data(string filePath)
		{
			FileStream stream = new FileStream(filePath, FileMode.Open, FileAccess.Read);

			BinaryReader reader = new BinaryReader(stream);

			byte[] File_Data = reader.ReadBytes((int)stream.Length);

			reader.Close();
			stream.Close();

			return File_Data;
		}
	}
}
