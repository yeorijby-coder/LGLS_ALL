//최초작성자	: 권혁찬	
//작성일		: 20171114
//화면개요		: WMS DOWNLOAD 프로그램 (VB소스 => C# 컨버팅)
//수정이력		: 

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Threading;
using System.Net;
using System.Collections;
using Microsoft.VisualBasic;
using System.Diagnostics;
using System.IO;
using System.Data.SqlClient;
using System.Data.OleDb;
using Npgsql;
using NpgsqlTypes;
using System.Runtime.InteropServices; //DllImport

namespace EcsClient
{
	public partial class WmsDown : Form
	{
		[DllImport("kernel32")]
		public static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

		[DllImport("kernel32")]
		public static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

		private string m_strSql;
		private int m_iSelcnt;
		private bool m_blStop_value;
		private bool m_blForma_Act;
		private string m_strDn_Prog;
		private string m_strDown_Comp;
		private string m_strDOWN_PATH_SET;
		public bool blBatchRun = false;
		//---------------------------------------------
		// 메인화면 로딩 및 초기화
		//---------------------------------------------
		public WmsDown()
		{
			InitializeComponent();
		}

		private void WmsDown_Load(object sender, EventArgs e)
		{
			//--------------------------------------------------
			// 프로그램이 기동중인지 확인
			//--------------------------------------------------
			{
                if (System.Diagnostics.Process.GetCurrentProcess().ProcessName.ToUpper() != "ECSCLIENT" & System.Diagnostics.Process.GetCurrentProcess().ProcessName.ToUpper() != "ECSCLIENT.VSHOST")
				{
					MessageBox.Show("프로세스명이 변경되어 프로그램을 실행 할 수 없습니다.", "Running...", MessageBoxButtons.OK, MessageBoxIcon.Warning);
					Application.Exit();
					this.Close();
					return;
				}
			}

			//--------------------------------------------------
			// 프로그램이 기동중인지 확인
			//--------------------------------------------------
			{
				if ((Information.UBound(System.Diagnostics.Process.GetProcessesByName(System.Diagnostics.Process.GetCurrentProcess().ProcessName)) > 0) == true)
				{
					MessageBox.Show("기존 프로그램이 이미 사용중 입니다.", "Running...", MessageBoxButtons.OK, MessageBoxIcon.Warning);
					Application.Exit();
					this.Close();
					return;
				}
			}

			//--------------------------------
			// 초기화
			//--------------------------------

			Label6.Text = "";
			Label7.Text = "";
			lbl_Cnt.Text = "";

			//bForma_Act = False
			subForm_init();

			if (m_blStop_value == false)
			{
				//DownLoad_Start()
				Timer1.Enabled = true;
			}
		}
		
		//---------------------------------------------
		// 컨트롤 이벤트
		//---------------------------------------------
		private void Button1_Click(object sender, EventArgs e)
		{
			WmsSet sForm = new WmsSet();

			sForm.ShowDialog();
		}

		private void Button2_Click(object sender, EventArgs e)
		{
			if (!m_blStop_value)
			{
				m_blStop_value = true;
				Label7.Text = "정지 요청 중...";
			}
			else
			{
				m_blStop_value = false;
			}

			Timer1_Tick(sender, e);
			//DownLoad_Start()
		}

		private void Timer1_Tick(object sender, EventArgs e)
		{

			this.Timer1.Enabled = false;

			DownLoad_Start();
		}

		//---------------------------------------------
		// 함수
		//---------------------------------------------
		//Db연결을 한다.
		private bool  gconDbinit()
		{
			Label7.Text = "서버에 연결중...";
			Label7.Refresh();
            //DB 서버 접속
            //----------------------------------------------------------------------------
            // WMSDOWN.INI 이용.
            // 1차 접속 후 실패가 나면 
            // 2차 접속함. 
            //----------------------------------------------------------------------------
			if (modCom.gconDb == null)
			{
				if (!modCom.DBLogIn(ref modCom.gconDb))
				{
					Application.Exit();
					this.Close();

					return false;
				}
			}
			return true;
		}

		private bool OlDbBlob2File(string pComp, ref string pMsg)
		{
#if oracle
			OleDbCommand cmd = new OleDbCommand();
#elif mssql
			SqlCommand cmd = new SqlCommand();
			//Dim sbCommandTimeout As StringBuilder = New StringBuilder(30)
			//GetPrivateProfileString("DB SERVER", "CommandTimeout", "", sbCommandTimeout, sbCommandTimeout.Capacity, DOWN_INI)
			//If IsNumeric(sbCommandTimeout.ToString) Then
			//    cmd.CommandTimeout = CInt(sbCommandTimeout.ToString)
			//End If
#elif postgresql
			NpgsqlCommand cmd = new NpgsqlCommand();
#endif

            int iPictureCol = 6;
			// the column # of the BLOB field
			
			ushort Attributes = 0;
			ushort NewAttributes = 0;
			ushort uaReadOnly = 0;
			//// b_ReadOnly
			bool blReadOnly = false;
			bool blArchive = false;
			bool blSystem = false;
			bool blHidden = false;
			
			string strEX_PGM = null;
			string strEX_INI = null;
			string strLAST_UP_DT = null;
			string strLAST_DN_DT = null;
			string strWK_DN_PGM = null;
			string strWK_UP_FNM = null;
			string strDOWN_PATH = null;
			string strDown_Name = null;
			string strWK_DN_FNM = "";
			double dblCK_DN_VER = 0;
			double dblWK_DN_SIZE = 0;
			double dblWK_DN_VER = 0;
			int iCnt = 0;
			int iCnt_T = 0;
			bool bldown_load_ok = false;
			string strUP_FNM = "";
			string strDN_NO = "";

			try {		
				strEX_PGM = Application.ProductName + ".EXE";
				strEX_INI = Application.ProductName + ".INI";
				cmd.Connection = modCom.gconDb;
				//_______________________________________________________________________________________
				// 최종 업데이트 일시 구함. 비교함.
				//_______________________________________________________________________________________
				cmd.CommandText = "";
#if oracle
				cmd.CommandText += "       SELECT * ";
				if (pComp == "1")
				{
					cmd.CommandText += "    FROM(   SELECT DN_NO                                                                            ";
					cmd.CommandText += "            , MAX(TO_CHAR(UP_DNT, 'YYYY-MM-DD HH24:MI:SS')) AS UP_DNT FROM DN_MST              ";
				}
				else
				{
					cmd.CommandText += "    FROM(   SELECT DN_NO                                                                            ";
					cmd.CommandText += "             , TO_CHAR(SYSDATE , 'YYYY-MM-DD HH24:MI:SS') AS UP_DNT FROM DN_MST                              ";
				}
				cmd.CommandText += "            WHERE DN_PGM  = '" + Strings.UCase(strEX_PGM) + "'                                     ";
				cmd.CommandText += "               OR DN_PGM  = 'COMMON'                                                               ";
				cmd.CommandText += "               OR DN_PGM = '" + m_strDn_Prog + "'                                                  ";
				cmd.CommandText += "               OR DN_PGM  = '공통'                                                                 ";
				cmd.CommandText += "         GROUP BY DN_NO )                                                                           ";
				cmd.CommandText += "         ORDER BY UP_DNT DESC                                                                      ";

				OleDbDataReader dr_u = cmd.ExecuteReader();
#elif mssql

				if (pComp == "1")
				{
					cmd.CommandText += "       SELECT DN_NO                                                                            ";
					cmd.CommandText += "            , MAX(CONVERT(CHAR(30), UP_DNT, 20)) AS UP_DNT FROM DN_MST    ";
				}
				else
				{
					cmd.CommandText += "         SELECT DN_NO                                                                          ";
					cmd.CommandText += "            , RTRIM(CONVERT(CHAR(30), GETDATE(), 20)) AS UP_DNT FROM DN_MST                    ";
				}
				cmd.CommandText += "            WHERE DN_PGM  = '" + Strings.UCase(strEX_PGM) + "'                                     ";
				cmd.CommandText += "                Or DN_PGM  = 'COMMON'                                                              ";
				cmd.CommandText += "               OR DN_PGM = '" + m_strDn_Prog + "'                                                  ";
				cmd.CommandText += "               Or DN_PGM  = '공통'                                                                 ";
				cmd.CommandText += "           GROUP BY DN_NO                                                                          ";
				cmd.CommandText += "          ORDER BY UP_DNT DESC                                                                     ";

				SqlDataReader dr_u = cmd.ExecuteReader();
#elif postgresql
				string CRLF = ControlChars.CrLf; // @.제어문자[vbCrLf]
				/*

				cmd.CommandText += CRLF + "       SELECT A.* ";
				if (pComp == "1")
				{
					cmd.CommandText += CRLF + "    FROM(   SELECT DN_NO                                                                            ";
					cmd.CommandText += CRLF + "            , MAX(TO_CHAR(UP_DNT, 'YYYY-MM-DD HH24:MI:SS')) AS UP_DNT FROM DN_MST              ";
				}
				else
				{
					cmd.CommandText += CRLF + "    FROM(   SELECT DN_NO                                                                            ";
					cmd.CommandText += CRLF + "             , TO_CHAR(now() , 'YYYY-MM-DD HH24:MI:SS') AS UP_DNT FROM DN_MST                              ";
				}
				cmd.CommandText += CRLF + "            WHERE DN_PGM  = '" + Strings.UCase(strEX_PGM) + "'                                     ";
				cmd.CommandText += CRLF + "               OR DN_PGM  = 'COMMON'                                                               ";
				cmd.CommandText += CRLF + "               OR DN_PGM = '" + m_strDn_Prog + "'                                                  ";
				cmd.CommandText += CRLF + "               OR DN_PGM  = '공통'                                                                 ";
				cmd.CommandText += CRLF + "         GROUP BY DN_NO ) A                                                                          ";
				cmd.CommandText += CRLF + "         ORDER BY UP_DNT DESC, DN_NO DESC                                                                      ";

				NpgsqlDataReader dr_u = cmd.ExecuteReader();
				//*/
#endif

				//if (dr_u.Read())
				//{
				//	strLAST_UP_DT = dr_u[1].ToString();
				//	strDN_NO = dr_u[0].ToString();

				//	////--------------------------------
				//	//// 마지막 Download 일자
				//	////--------------------------------
				//	strLAST_DN_DT = "";
				//	modCom.ReadInitProfile_C(ref strLAST_DN_DT, "DOWN_FILE", strEX_PGM + "->최종DOWNLOAD일시");

				//	// TEST				- 2024-11-20 17:07:XX.XXX
				//	if (string.IsNullOrEmpty(strLAST_DN_DT))
				//	{
				//		strLAST_DN_DT = "2025-01-01 00:00:00";
				//	}

				//	if (pComp == "1")
				//	{
				//		if ((String.Compare(strLAST_UP_DT, strLAST_DN_DT) < 0) || 
				//			(String.Compare(strLAST_UP_DT, strLAST_DN_DT) == 0) || 
				//			(string.IsNullOrEmpty(strLAST_UP_DT)))
				//		{
				//			ProgressBar1.Maximum = 1;
				//			ProgressBar1.Value = 1;
				//			return true;
				//		}
				//	}
				//}

				// [LGLS 2026-09-30] 이 줄이 주석이라 내려받기가 아예 되지 않았다.
				//   "이 Command와 연결된 DataReader가 이미 열려 있습니다" 가 나고,
				//   그 메시지가 뜬 뒤 프로그램이 끝난다.
				//   위 조회 결과를 쓰는 코드가 통째로 주석이 되면서 닫는 줄까지 같이 묻혔다.
				dr_u.Close();

				Label7.Text = "Download 자료 검색중...";
				Label7.Refresh();

				Application.DoEvents();

				cmd.CommandText = "";

				cmd.CommandText = "";
                cmd.CommandText += modCom.CRLF + "     SELECT CAST(A.DN_VER as VARCHAR) as DN_VER, A.DN_NM		";
                cmd.CommandText += modCom.CRLF + "          , UD.UP_FNM										";
                cmd.CommandText += modCom.CRLF + "          , UD.DN_PGM										";
                cmd.CommandText += modCom.CRLF + "          , UD.DN_DIR										";
                cmd.CommandText += modCom.CRLF + "          , UD.UP_DAT										";
                cmd.CommandText += modCom.CRLF + "          , UD.DN_NO											";
                cmd.CommandText += modCom.CRLF + "          , UD.UP_DT											";
                cmd.CommandText += modCom.CRLF + "       FROM (												";
                cmd.CommandText += modCom.CRLF + "			     SELECT MAX(CAST(DN_VER as INTEGER)) as DN_VER	";
				cmd.CommandText += modCom.CRLF + "			          , DN_NM									";
				cmd.CommandText += modCom.CRLF + "			       FROM UP_DOWN									";
				cmd.CommandText += modCom.CRLF + "			      WHERE DN_PGM = 'COMMON'						";
				cmd.CommandText += modCom.CRLF + "			   GROUP BY DN_NM									";
				cmd.CommandText += modCom.CRLF + "			   ) A												";
                cmd.CommandText += modCom.CRLF + "      INNER JOIN UP_DOWN UD									";
                cmd.CommandText += modCom.CRLF + "              ON A.DN_VER = CAST(UD.DN_VER as INTEGER)       ";
                cmd.CommandText += modCom.CRLF + "             AND A.DN_NM = UD.DN_NM							";


#if oracle
				OleDbDataReader dr_s = cmd.ExecuteReader();
#elif mssql
                SqlDataReader dr_s = cmd.ExecuteReader();
#elif postgresql
                NpgsqlDataReader dr_s = cmd.ExecuteReader();
				//NpgsqlDataReader dr_d = cmd.ExecuteReader();
#endif

				iCnt = 0;

				if (dr_s.HasRows == true)
				{
					while ((dr_s.Read()))
					{
						if (m_blStop_value)
							return true;

						dblWK_DN_VER = Convert.ToDouble(dr_s.GetString(0));//DN_VER
						strDown_Name = dr_s.GetString(1);// DN_NM
						strWK_UP_FNM = dr_s.GetString(2);// UP_FNM
						//dblWK_DN_SIZE = Convert.ToDouble(dr_s.GetString(3));// DN_SIZE
						strWK_DN_PGM = dr_s.GetString(3);// DN_PGM
						strDOWN_PATH = dr_s.GetString(4);// DN_DIR

						bldown_load_ok = false;

						//if (m_strDOWN_PATH_SET == "1")
							strWK_DN_FNM = strDOWN_PATH + "\\" + strDown_Name;
                        //if (m_strDOWN_PATH_SET == "2")
                        //    strWK_DN_FNM = Strings.Left(Application.ExecutablePath, 1) + Strings.Mid(strWK_UP_FNM, 2);
                        //if (m_strDOWN_PATH_SET == "3")
                        //    strWK_DN_FNM = strWK_UP_FNM;
                        ////--------------------------------
                        //// 다운로드 경로 설정
                        ////--------------------------------
                        {
							dblCK_DN_VER = 0;

							StringBuilder sb = new StringBuilder(1000);
							
							if (!System.IO.File.Exists(modCom.DOWN_INI))
							{
								return false;
							}

							string strKey = strWK_DN_PGM + "->" + strDown_Name;
							GetPrivateProfileString("DOWN_FILE", strKey, "0", sb, sb.Capacity, modCom.DOWN_INI);
							dblCK_DN_VER = Convert.ToDouble(sb.ToString());

							if (dblWK_DN_VER != dblCK_DN_VER)
							{
								bldown_load_ok = true;
							}
							else
							{
								if (strDown_Name.ToUpper() != FileSystem.Dir(strWK_DN_FNM).ToUpper())
								{
									bldown_load_ok = true;
								}
							}

							if (bldown_load_ok == true)
							{
								iCnt_T += 1;
								strUP_FNM += "'" + strWK_UP_FNM + "',";
							}
						}
					}
				}

				dr_s.Close();


				if (iCnt_T > 0)
				{
					strUP_FNM = Strings.Mid(strUP_FNM, 1, strUP_FNM.Length - 1);

					Label7.Text = "Download 자료 다운중...";
					Label7.Refresh();

					cmd.CommandText = "";
					cmd.CommandText += modCom.CRLF + "     SELECT CAST(A.DN_VER as VARCHAR) as DN_VER, A.DN_NM				";
					cmd.CommandText += modCom.CRLF + "          , UD.UP_FNM												";
					cmd.CommandText += modCom.CRLF + "          , UD.DN_PGM												";
					cmd.CommandText += modCom.CRLF + "          , UD.DN_DIR												";
					cmd.CommandText += modCom.CRLF + "          , CAST(UD.DN_SIZE as VARCHAR) as DN_SIZE				";	// [LGLS 2026-09-30] 아래에서 GetString 으로 읽는다 - 숫자 그대로면 형식 오류
					cmd.CommandText += modCom.CRLF + "          , UD.UP_DAT												";
					cmd.CommandText += modCom.CRLF + "          , UD.DN_NO													";
					cmd.CommandText += modCom.CRLF + "          , UD.UP_DT													";
					cmd.CommandText += modCom.CRLF + "       FROM (														";
					cmd.CommandText += modCom.CRLF + "			     SELECT MAX(CAST(DN_VER as INTEGER)) as DN_VER			";
					cmd.CommandText += modCom.CRLF + "			          , DN_NM											";
					cmd.CommandText += modCom.CRLF + "			       FROM UP_DOWN											";
					cmd.CommandText += modCom.CRLF + "			      WHERE UP_FNM IN (" + strUP_FNM + ")					";
					cmd.CommandText += modCom.CRLF + "			   GROUP BY DN_NM											";
					cmd.CommandText += modCom.CRLF + "			   ) A														";
					cmd.CommandText += modCom.CRLF + "      INNER JOIN UP_DOWN UD											";
					cmd.CommandText += modCom.CRLF + "              ON A.DN_VER = CAST(UD.DN_VER as INTEGER)				";
					cmd.CommandText += modCom.CRLF + "             AND A.DN_NM = UD.DN_NM									";

					dr_s = cmd.ExecuteReader();

					iCnt = 0;


					if (dr_s.HasRows == true)
					{

						while ((dr_s.Read()))
						{
							if (m_blStop_value)
								return true;

							//DN_VER
							dblWK_DN_VER = Convert.ToDouble(dr_s.GetString(0));

							// DN_NM
							strDown_Name = dr_s.GetString(1);

							// UP_FNM
							strWK_UP_FNM = dr_s.GetString(2);

							// DN_PGM
							strWK_DN_PGM = dr_s.GetString(3);

							// DN_DIR
							strDOWN_PATH = dr_s.GetString(4);

							// DN_SIZE
							dblWK_DN_SIZE = Convert.ToDouble(dr_s.GetString(5));
																				


							//if (m_strDOWN_PATH_SET == "1")
								strWK_DN_FNM = strDOWN_PATH + "\\" + strDown_Name;
							//if (m_strDOWN_PATH_SET == "2")
							//	strWK_DN_FNM = Strings.Left(Application.ExecutablePath, 1) + Strings.Mid(strWK_UP_FNM, 2);
							//if (m_strDOWN_PATH_SET == "3")
							//	strWK_DN_FNM = strWK_UP_FNM;



							////--------------------------------
							//// 특정 압축파일이 업데이트 되어서 다운로드 받아야 하는 경우는 배치 프로그램을 실행해야함!
							////--------------------------------
							if (strWK_DN_FNM.Substring(strWK_DN_FNM.Length - 4, 4) == ".zip")
							{
								blBatchRun = true;
							}

							////--------------------------------
							//// 다운로드 경로 설정
							////--------------------------------
								dblCK_DN_VER = 0;
							//modCom.ReadInitProfile_i(ref dblCK_DN_VER, "DOWN_FILE", strWK_DN_PGM + "->" + strDown_Name);
							StringBuilder sb = new StringBuilder(1000);
							string strKey = strWK_DN_PGM + "->" + strDown_Name;
							GetPrivateProfileString("DOWN_FILE", strKey, "0", sb, sb.Capacity, modCom.DOWN_INI);
							dblCK_DN_VER = Convert.ToDouble(sb.ToString());

							iCnt += 1;
							lbl_Cnt.Text = iCnt + "/" + iCnt_T;
							lbl_Cnt.Refresh();

							Label6.Text = strDown_Name + " " + Strings.Format(dblWK_DN_SIZE, "#,##0") + "Bytes";
							Label6.Refresh();

							ProgressBar1.Maximum = 100;
							if (ProgressBar1.Value + (100 / iCnt_T) > 100 || iCnt == iCnt_T)
							{
								ProgressBar1.Value = 100;
							}
							else
							{
								if (ProgressBar1.Value + (100 / iCnt_T) > 100)
								{
									ProgressBar1.Value = 100;
								}
								else if (ProgressBar1.Value + (100 / iCnt_T) < 0)
								{
									ProgressBar1.Value = 0;
								}
								else
								{
									ProgressBar1.Value += 100 / iCnt_T;
								}
							}

							////-------------------------------------------------
							//// DownLoad프로그램과 Ini파일의 Download를 방지한다.
							////-------------------------------------------------
							//If down_load_ok Then
							if (strWK_DN_FNM != strEX_PGM)
							{
								bldown_load_ok = true;
							}
							else
							{
								bldown_load_ok = false;
							}

							if (strWK_DN_FNM != strEX_INI)
							{
								bldown_load_ok = true;
							}
							else
							{
								bldown_load_ok = false;
							}

							bldown_load_ok = Client_Dir_Create(strWK_DN_FNM);

							Label7.Text = "Download 자료 설치중...";
							Label7.Refresh();

							blReadOnly = false;
							blArchive = false;
							blSystem = false;
							blHidden = false;

							if (strDown_Name == FileSystem.Dir(strWK_DN_FNM))
							{
								////-------------------------------------------------
								//// 파일의 속성이 ReadOnly이면 오류가 발생하므로
								//// 속성을 ReadOnly = False로 변경한다.
								////-------------------------------------------------
								//Attributes = (ushort)FileSystem.GetAttr(strWK_DN_FNM);
								blReadOnly = (FileSystem.GetAttr(strWK_DN_FNM) & Constants.vbReadOnly) == Constants.vbReadOnly;
								blArchive = (FileSystem.GetAttr(strWK_DN_FNM) & Constants.vbArchive) == Constants.vbArchive;
								blSystem = (FileSystem.GetAttr(strWK_DN_FNM) & Constants.vbSystem) == Constants.vbSystem;
								blHidden = (FileSystem.GetAttr(strWK_DN_FNM) & Constants.vbHidden) == Constants.vbHidden;

								if (blReadOnly == true)
								{
									System.IO.File.SetAttributes(strWK_DN_FNM, System.IO.FileAttributes.Normal);  
								}
							}

							byte[] b = new byte[dr_s.GetBytes(iPictureCol, 0, null, 0, int.MaxValue)];


							dr_s.GetBytes(iPictureCol, 0, b, 0, b.Length);

							System.IO.FileStream fs = new System.IO.FileStream(strWK_DN_FNM, System.IO.FileMode.Create, System.IO.FileAccess.Write);

							fs.Write(b, 0, b.Length);

							fs.Close();

							//strKey = strWK_DN_PGM + "->" + strDown_Name;
							WritePrivateProfileString("DOWN_FILE", strKey, dblWK_DN_VER.ToString(), modCom.DOWN_INI);
							//GetPrivateProfileString("DOWN_FILE", strKey, "0", sb, sb.Capacity, modCom.DOWN_INI);
							//dblCK_DN_VER = Convert.ToDouble(sb.ToString());


							//modCom.WriteInitProfile("DOWN_FILE", strWK_DN_PGM + "->" + strDown_Name, ref dblWK_DN_VER);

							Application.DoEvents();
						}
					}
                }

				//오래된 자료를 삭제한다.
				{
					cmd.CommandText = "";
					//m_strSql += modCom.CRLF + "  SELECT *";
					cmd.CommandText += modCom.CRLF + "DELETE												";
					cmd.CommandText += modCom.CRLF + "  FROM UP_DOWN										";
					cmd.CommandText += modCom.CRLF + " WHERE (DN_NM, CAST(DN_VER as INTEGER)) NOT IN (		";
					cmd.CommandText += modCom.CRLF + "		  SELECT DN_NM, MAX(CAST(DN_VER as INTEGER))	";
					cmd.CommandText += modCom.CRLF + "		  FROM UP_DOWN									";
					cmd.CommandText += modCom.CRLF + "		  WHERE DN_PGM = 'COMMON'						";
					cmd.CommandText += modCom.CRLF + "		  GROUP BY DN_NM								";
					cmd.CommandText += modCom.CRLF + ")													";
					iCnt = cmd.ExecuteNonQuery();

					if (iCnt < 0)
					{
						
						throw new Exception("");
					}
				}


				strLAST_UP_DT = Strings.Format(DateTime.Now, "yyyy-MM-dd HH:mm:ss");
				modCom.WriteInitProfile("DOWN_FILE", strEX_PGM + "->최종DOWNLOAD일시", strLAST_UP_DT);

				dr_s.Close();

				modCom.gconDb.Close();

				return true; 
			} catch (Exception ex) {
				pMsg = ex.Message;
				return false;
			}
		}

		private bool Client_Dir_Create(string pFileName)
		{
			string Client_Down_DIR = null;
			string Make_Dir = null;
			string Imsi_Dir = null;
			int iPos = 0;
			int jPos = 0;

			iPos = Strings.InStrRev(pFileName, "\\");
			if (iPos <= 0)
			{
				MessageBox.Show("Download file 에러입니다.", "Download 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}

			Client_Down_DIR = Strings.Left(pFileName, iPos);

			if (Strings.Right(Client_Down_DIR, 1) == "\\")
			{
				Client_Down_DIR = Strings.Left(Client_Down_DIR, Strings.Len(Client_Down_DIR) - 1);
			}

			Make_Dir = Client_Down_DIR;

			while (true)
			{
				if (!Directory.Exists(Make_Dir))
				{
					jPos = Strings.InStrRev(pFileName, "\\");

					if (jPos <= 0)
					{
						MessageBox.Show("Download 디렉토리를 만들 수 없습니다.", "Download 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
						return false;
					}

					Client_Down_DIR = Strings.Left(pFileName, iPos);
				}
				else
				{
					if (Make_Dir == Client_Down_DIR)
					{
						return true;
					}
				}

				Label7.Text = "다운로드 디렉토리 생성중...";
				Label7.Refresh();

				jPos = Strings.Len(Make_Dir) + 1;
				Imsi_Dir = Strings.Mid(Client_Down_DIR, jPos + 1, Strings.Len(Client_Down_DIR) - jPos);
				iPos = Strings.InStr(Imsi_Dir, "\\");

				if (iPos > 0)
				{
					Make_Dir += "\\" + Strings.Mid(Imsi_Dir, 1, iPos - 1);
				}
				else
				{
					Make_Dir += "\\" + Imsi_Dir;
				}

				DirectoryInfo DirInfo = Directory.CreateDirectory(Make_Dir);


				if (!DirInfo.Exists)
				{
					MessageBox.Show("신규 Download 디렉토리를 만들 수 없습니다.", "Download 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return false;
				}
			}

			return true;
		}

		private void subForm_init()
		{
			string pTitle = null;
			string pForm_Pic = null;
			string pIco = null;
			string pSet_Btn = null;
			string pDn_St = null;
			string pDown_Progm = "";

			////--------------------------------
			//// Caption 적용
			////--------------------------------
			pTitle = modCom.ReadInitProfileStr("APPLICATION", "TITLE");

			this.Text = pTitle;

			////--------------------------------
			//// Form Picture 적용
			////--------------------------------
			pForm_Pic = modCom.ReadInitProfileStr("APPLICATION", "FORM_PICTURE");

			if (!string.IsNullOrEmpty(Strings.Trim(pForm_Pic)))
			{
				pForm_Pic = Application.ExecutablePath + "\\" + pForm_Pic.Substring(Strings.InStr(pForm_Pic, "\\"));

				if (!string.IsNullOrEmpty(FileSystem.Dir(pForm_Pic)))
				{
					this.BackgroundImage = Image.FromFile(pForm_Pic);
				}
			}

			////--------------------------------
			//// ICon 적용
			////--------------------------------
			pIco = modCom.ReadInitProfileStr("APPLICATION", "ICON");

			if (!string.IsNullOrEmpty(Strings.Trim(pIco)))
			{
				pIco = Application.ExecutablePath + "\\" + pIco.Substring(Strings.InStr(pIco, "\\"));

				if (!string.IsNullOrEmpty(FileSystem.Dir(pIco)) & !string.IsNullOrEmpty(Strings.Trim(pIco)))
				{
					this.Icon = new Icon(pIco);
				}
			}

			////--------------------------------
			//// 설정 Button 적용
			////--------------------------------
			pSet_Btn = modCom.ReadInitProfileStr("APPLICATION", "SETUP_BTN");

			if (pSet_Btn == "2")
			{
				this.Button1.Enabled = false;
			}
			else
			{
				this.Button1.Enabled = true;
			}

			////--------------------------------
			//// 다운로드 할 프로그램
			////--------------------------------
			m_strDn_Prog = modCom.ReadInitProfileStr("APPLICATION", "DOWN_LOAD_PROGRAM");

			////--------------------------------
			//// DownLoad 시작 적용
			////--------------------------------
			pDn_St = modCom.ReadInitProfileStr("APPLICATION", "DOWN_LOAD_START");

			if (pDn_St == "1")
			{
				//Me.Timer1.Enabled = True

				m_blStop_value = false;

				this.Button2.Text = "정지";
			}
			else
			{
				m_blStop_value = true;
				//Me.Timer1.Enabled = False
			}

			////--------------------------------
			//// Down Load 비교 적용
			////--------------------------------
			m_strDown_Comp = modCom.ReadInitProfileStr("APPLICATION", "DOWN_LOAD_COMP");

			////--------------------------------
			//// 다운로드 경로 설정
			////--------------------------------
			m_strDOWN_PATH_SET = modCom.ReadInitProfileStr("APPLICATION", "DOWN_LOAD_PATH_SET");
		}


		private void DownLoad_Start()
		{
			string strMsg = "";
			string pExec_File = null;
			int i = 0;

			////---------------------------------------------
			//// DB 연결
			////---------------------------------------------
			if (gconDbinit() == false) {
				return;
			}

			Label7.Text = "프로그램 실행중 .... " + modCom.g_strWmsServerName + "접속중";
			Label7.Refresh();


			//m_strDown_Comp = "0";
			if (!OlDbBlob2File(m_strDown_Comp, ref strMsg))
			{
				MessageBox.Show("WMS DownLoad중 에러가 발생했습니다." + modCom.CRLF + strMsg, "WMS DownLoad에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
				Application.Exit();
				return;
			}

			Application.DoEvents();

			if (m_blStop_value)
			{
				Button2.Text = "시작";
				Label7.Text = "정지 ...";
				return;
			}

			Label7.Text = "WMS 실행중입니다 잠시만 기다려 주십시오... ";
			Label7.Refresh();

			////--------------------------------
			//// 실행 화일 적용하기
			////--------------------------------

			for (i = 1; i <= 5; i++)
			{
				pExec_File = modCom.ReadInitProfileStr("RUN_FILE", "FILE" + i);

				StringBuilder sbValue = new StringBuilder(100);
				GetPrivateProfileString("RUN_FILE", "FILE" + i, "", sbValue, sbValue.Capacity, modCom.DOWN_INI);
				pExec_File = sbValue.ToString();

				if (!string.IsNullOrEmpty(pExec_File.Trim()))
				{
                    //Shell(pExec_File, AppWinStyle.NormalFocus)

                    try
					{
						if (i == 1)
                        {
							if (blBatchRun == true)
                            {
								MessageBox.Show("리소스 파일을 업데이트 합니다. 시간이 10초이상 소요됩니다");
								Process.Start("C:\\CLIENT\\WCS\\" + pExec_File.Substring(Strings.InStr(pExec_File, "\\")));
								Thread.Sleep(10000);
							}
                        }
						else
                        {
							Process.Start("C:\\CLIENT\\WCS\\" + pExec_File.Substring(Strings.InStr(pExec_File, "\\")));
							//Process.Start(Application.StartupPath + "\\" + pExec_File.Substring(Strings.InStr(pExec_File, "\\")));
						}
					}
                    catch (Exception ex)
                    {
                        Label7.Text = ex.Message;
                        Label7.Refresh();
                    }
                    
                    Application.DoEvents();
				}
			}

			this.Dispose();

		}

	}
}
