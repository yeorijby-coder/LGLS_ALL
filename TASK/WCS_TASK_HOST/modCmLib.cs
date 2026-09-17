using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.OleDb;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using System.Data.SqlClient;
using Npgsql;

namespace TSK_HostCom
{
	//*** DB Log In ***********************************************
    // DB 관련 프로젝트는 모두 사용하므로 여기에 두기로 한다.
	class modCmLib
	{
#if ORACLE
		public static bool DBLogIn(ref OleDbConnection p_ConObj)
		{
			ReadInitProfile();

			bool blDbCon = false;
			p_ConObj = new OleDbConnection();

			//-------------------------------------
			// Provider=MSDAORA.1  Login
			//-------------------------------------
			//Try
			//    pConObj.ConnectionString = "Provider=MSDAORA.1; Data Source = " & _
			//                            gUser.DbAlias & "; User ID = " & _
			//                            gUser.UserID & "; Password = " & _
			//                            gUser.UserPassword
			//    pConObj.Open()

			//    bDbCon = True
			//Catch AppErr As Exception
			blDbCon = false;
			//End Try

			//-------------------------------------
			// Provider=OraOLEDB.Oracle.1  Login
			//-------------------------------------
			if (blDbCon == false)
			{
				try
				{
					p_ConObj.ConnectionString = "Provider=OraOLEDB.Oracle.1; Data Source = " 
						+ modDefApp.g_User.g_strDbAlias + "; User ID = "
						+ modDefApp.g_User.g_strUserID + "; Password = " 
						+ modDefApp.g_User.g_strUserPassword;
					p_ConObj.Open();

				}
				catch (Exception AppErr)
				{
					MessageBox.Show(AppErr.Message, "Log In Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return false;
				}
			}
			return true;
		}

		// 로그인 실패시 백그라운드에서는 ErrMsg 리턴
		public static bool DBLogIn(ref OleDbConnection p_ConObj, ref string p_strErrMsg)
		{
			ReadInitProfile();

			bool blDbCon = false;

			p_ConObj = new OleDbConnection();

			//-------------------------------------
			// Provider=MSDAORA.1  Login
			//-------------------------------------
			//Try
			//    pConObj.ConnectionString = "Provider=MSDAORA.1; Data Source = " & _
			//                            gUser.DbAlias & "; User ID = " & _
			//                            gUser.UserID & "; Password = " & _
			//                            gUser.UserPassword
			//    pConObj.Open()

			//    bDbCon = True
			//Catch AppErr As Exception
			blDbCon = false;
			//End Try

			//-------------------------------------
			// Provider=OraOLEDB.Oracle.1  Login
			//-------------------------------------
			if (blDbCon == false)
			{
				try
				{
					p_ConObj.ConnectionString = "Provider=OraOLEDB.Oracle.1; Data Source = "
						+ modDefApp.g_User.g_strDbAlias + "; User ID = "
						+ modDefApp.g_User.g_strUserID + "; Password = "
						+ modDefApp.g_User.g_strUserPassword;
					p_ConObj.Open();

				}
				catch (Exception AppErr)
				{
					p_strErrMsg = AppErr.Message;
					return false;
				}
			}
			return true;
		}

		public static void ReadInitProfile()
		{
			StringBuilder sb = new StringBuilder(30);
			modDefAPI.GetPrivateProfileString("DB", "SERVICENAME", "", sb, sb.Capacity,modDefApp.MAIN_INI);
			modDefApp.g_User.g_strDbAlias = sb.ToString();

			modDefAPI.GetPrivateProfileString("DB", "USER", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_User.g_strUserID = sb.ToString();

			modDefAPI.GetPrivateProfileString("DB", "PASSWORD", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_User.g_strUserPassword = sb.ToString();

			modDefAPI.GetPrivateProfileString("Network", "LocalPort", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_iListenPort = int.Parse(sb.ToString());

			modDefAPI.GetPrivateProfileString("Network", "RemoteIP", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_strRemoteIP = sb.ToString();

			modDefAPI.GetPrivateProfileString("Network", "RemotePort", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_iRemotePort = int.Parse(sb.ToString());


			//이길문[20161122]이중입고재지정횟수추가
			modDefAPI.GetPrivateProfileString("Property", "RE_DRCT_CNT", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			if (Information.IsNumeric(modDefApp.g_strRE_DRCT_CNT) == false)
			{
				modDefApp.g_strRE_DRCT_CNT = "0";
			}
			else
			{
				modDefApp.g_strRE_DRCT_CNT = sb.ToString();
			}

			if (string.IsNullOrEmpty(modDefApp.g_User.g_strUserID))
				modDefApp.g_User.g_strUserID = "STD_USER";
			if (string.IsNullOrEmpty(modDefApp.g_User.g_strUserPassword))
				modDefApp.g_User.g_strUserPassword = "STD_USER";

		}
#endif
#if SQL
    public static bool DBLogIn(ref SqlConnection p_ConObj)
        {
			ReadInitProfile();

			p_ConObj = new SqlConnection();

                try
                {
					p_ConObj.ConnectionString = "Server = " +
                                               modDefApp.g_User.g_strDbAlias  + "; Trusted_Connection = False; Database = " +
                                               modDefApp.g_User.g_strDatabase   + "; User Id = " +
                                               modDefApp.g_User.g_strUserID  + "; Password = " +
                                               modDefApp.g_User.g_strUserPassword ;
					p_ConObj.Open();

                }
				catch (Exception AppErr)
				{
					MessageBox.Show(AppErr.Message, "Log In Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return false;
				}

            return true;

        }

		// 로그인 실패시 백그라운드에서는 ErrMsg 리턴
	public static bool DBLogIn(ref SqlConnection p_ConObj, ref string p_strErrMsg)
		{
			ReadInitProfile();

			bool blDbCon = false;

			p_ConObj = new SqlConnection();


			blDbCon = false;

			if (blDbCon == false)
			{
				try
				{
					p_ConObj.ConnectionString = "Server = " +
												modDefApp.g_User.g_strDbAlias + "; Trusted_Connection = False; Database = " +
												modDefApp.g_User.g_strDatabase + "; User Id = " +
												modDefApp.g_User.g_strUserID + "; Password = " +
												modDefApp.g_User.g_strUserPassword;

				}
				catch (Exception AppErr)
				{
					p_strErrMsg = AppErr.Message;
					return false;
				}
			}
			return true;
		}

        public static void ReadInitProfile()
        {

            string strKeyData1 = "Q1";
            StringBuilder sb = new StringBuilder(30);

            modDefAPI.GetPrivateProfileString("WMS", "INFO", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            strKeyData1 = sb.ToString();

            modDefAPI.GetPrivateProfileString("Network", "LocalPort", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_iListenPort = int.Parse(sb.ToString());

			modDefAPI.GetPrivateProfileString("Network", "RemoteIP", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_strRemoteIP = sb.ToString();

			modDefAPI.GetPrivateProfileString("Network", "RemotePort", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_iRemotePort = int.Parse(sb.ToString());

            // [LGLS] MS-SQL 접속정보를 EcsComA.ini [DB] 에서 직접 읽는다
            //        (IP=서버 인스턴스, DATABASE/USER/USER_PW). 미설정 시 기존 WmsInfo.dll 매핑 사용.
            modDefAPI.GetPrivateProfileString("DB", "IP", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_User.g_strDbAlias = sb.ToString();

            modDefAPI.GetPrivateProfileString("DB", "DATABASE", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_User.g_strDatabase = sb.ToString();

            modDefAPI.GetPrivateProfileString("DB", "USER", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_User.g_strUserID = sb.ToString();

            modDefAPI.GetPrivateProfileString("DB", "USER_PW", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_User.g_strUserPassword = sb.ToString();

            if (string.IsNullOrEmpty(modDefApp.g_User.g_strDatabase))
            {
                WmsInfo.CWmsInfo.WmsDBInfo(ref modDefApp.g_User.g_strDbAlias, ref modDefApp.g_User.g_strDatabase, ref modDefApp.g_User.g_strUserID, ref modDefApp.g_User.g_strUserPassword, strKeyData1);
            }

            //창고구분(A:PalletRack자동창고, B:P-BoxRack자동창고), 미설정시 기본값 유지
            modDefAPI.GetPrivateProfileString("Host", "WarehouseDefine", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            if (sb.ToString() == "A" || sb.ToString() == "B")
            {
                modDefApp.WH_DEFINE = sb.ToString();
            }

            // [LGLS] IMS(WMS) 스테이션 코드 해석 기준 적재 ([Host]StationMapMode = ECS | WMS)
            modStationMap.LoadFromIni();

            // [LGLS 2026-08-30] 크레인 에러코드 마스터 구분 ([Host]ScErrCodeType, 기본 SC_LGLS)
            modDefAPI.GetPrivateProfileString("Host", "ScErrCodeType", "SC_LGLS", sb, sb.Capacity, modDefApp.MAIN_INI);
            if (sb.ToString().Trim().Length > 0) modDefApp.g_strScErrCodeTyp = sb.ToString().Trim();
            // [LGLS 2026-09-17] 크레인 에러 종류 판정 코드 ([Host] ScDualCodes / ScEmptyCodes / ScInFailCodes / ScOutFailCodes)
            modDefAPI.GetPrivateProfileString("Host", "ScDualCodes", "73,74", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_strScDualCodes = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("Host", "ScEmptyCodes", "75", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_strScEmptyCodes = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("Host", "ScInFailCodes", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_strScInFailCodes = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("Host", "ScOutFailCodes", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_strScOutFailCodes = sb.ToString().Trim();

            //이중입고재지정횟수추가
            modDefAPI.GetPrivateProfileString("Property", "RE_DRCT_CNT", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			if (Information.IsNumeric(modDefApp.g_strRE_DRCT_CNT) == false)
			{
				modDefApp.g_strRE_DRCT_CNT = "0";
			}
			else
			{
				modDefApp.g_strRE_DRCT_CNT = sb.ToString();
			}

			if (string.IsNullOrEmpty(modDefApp.g_User.g_strUserID))
				modDefApp.g_User.g_strUserID = "STD_USER";
			if (string.IsNullOrEmpty(modDefApp.g_User.g_strUserPassword))
				modDefApp.g_User.g_strUserPassword = "STD_USER";
        }
#endif
#if POSTGRESSQL
        public static bool DBLogIn(ref NpgsqlConnection p_ConObj)
        {
			ReadInitProfile();

            p_ConObj = new NpgsqlConnection();

                try
                {
                    p_ConObj.ConnectionString = "host=" + modDefApp.g_User.g_strDbIP + 
                        ";username=" + modDefApp.g_User.g_strUserID +
                        ";password=" + modDefApp.g_User.g_strUserPassword +
                        ";database=" + modDefApp.g_User.g_strDatabase + 
                        ";MAXPOOLSIZE=50;";

                    p_ConObj.Open();

                }
				catch (Exception AppErr)
				{
					MessageBox.Show(AppErr.Message, "Log In Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return false;
				}

            return true;

        }

		// 로그인 실패시 백그라운드에서는 ErrMsg 리턴
        public static bool DBLogIn(ref NpgsqlConnection p_ConObj, ref string p_strErrMsg)
		{
			ReadInitProfile();

			bool blDbCon = false;

            p_ConObj = new NpgsqlConnection();


			blDbCon = false;

			if (blDbCon == false)
			{
				try
				{
                    //p_ConObj.ConnectionString = "Server = " +
                    //                            modDefApp.g_User.g_strDbAlias + "; Trusted_Connection = False; Database = " +
                    //                            modDefApp.g_User.g_strDatabase + "; User Id = " +
                    //                            modDefApp.g_User.g_strUserID + "; Password = " +
                    //                            modDefApp.g_User.g_strUserPassword;

                    p_ConObj.ConnectionString = "host=" + modDefApp.g_User.g_strDbIP +
                        ";username=" + modDefApp.g_User.g_strUserID +
                        ";password=" + modDefApp.g_User.g_strUserPassword +
                        ";database=" + modDefApp.g_User.g_strDatabase +
                        ";MAXPOOLSIZE=50;";
				}
				catch (Exception AppErr)
				{
					p_strErrMsg = AppErr.Message;
					return false;
				}
			}
			return true;
		}

        public static void ReadInitProfile()
        {

            string strKeyData1 = "Q1";
            StringBuilder sb = new StringBuilder(30);

            modDefAPI.GetPrivateProfileString("DB", "DATABASE", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_User.g_strDatabase = sb.ToString();

            modDefAPI.GetPrivateProfileString("DB", "USER", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_User.g_strUserID = sb.ToString();

            modDefAPI.GetPrivateProfileString("DB", "USER_PW", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_User.g_strUserPassword = sb.ToString();

            modDefAPI.GetPrivateProfileString("DB", "IP", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_User.g_strDbIP = sb.ToString();

            modDefAPI.GetPrivateProfileString("DB", "PORT", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_User.g_strDbPort = sb.ToString();

            modDefAPI.GetPrivateProfileString("Network", "LocalPort", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_iListenPort = int.Parse(sb.ToString());

            modDefAPI.GetPrivateProfileString("Network", "RemoteIP", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_strRemoteIP = sb.ToString();

            modDefAPI.GetPrivateProfileString("Network", "RemotePort", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_iRemotePort = int.Parse(sb.ToString());

            modDefAPI.GetPrivateProfileString("WMS", "INFO", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            strKeyData1 = sb.ToString();

            modDefAPI.GetPrivateProfileString("Network", "LocalPort", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_iListenPort = int.Parse(sb.ToString());

			modDefAPI.GetPrivateProfileString("Network", "RemoteIP", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_strRemoteIP = sb.ToString();

			modDefAPI.GetPrivateProfileString("Network", "RemotePort", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			modDefApp.g_iRemotePort = int.Parse(sb.ToString());

      //      WmsInfo.CWmsInfo.WmsDBInfo(ref modDefApp.g_User.g_strDbAlias, ref modDefApp.g_User.g_strDatabase, ref modDefApp.g_User.g_strUserID, ref modDefApp.g_User.g_strUserPassword, strKeyData1);

            //창고구분(A:PalletRack자동창고, B:P-BoxRack자동창고), 미설정시 기본값 유지
            modDefAPI.GetPrivateProfileString("Host", "WarehouseDefine", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            if (sb.ToString() == "A" || sb.ToString() == "B")
            {
                modDefApp.WH_DEFINE = sb.ToString();
            }

            // [LGLS] IMS(WMS) 스테이션 코드 해석 기준 적재 ([Host]StationMapMode = ECS | WMS)
            modStationMap.LoadFromIni();

            // [LGLS 2026-08-30] 크레인 에러코드 마스터 구분 ([Host]ScErrCodeType, 기본 SC_LGLS)
            modDefAPI.GetPrivateProfileString("Host", "ScErrCodeType", "SC_LGLS", sb, sb.Capacity, modDefApp.MAIN_INI);
            if (sb.ToString().Trim().Length > 0) modDefApp.g_strScErrCodeTyp = sb.ToString().Trim();
            // [LGLS 2026-09-17] 크레인 에러 종류 판정 코드 ([Host] ScDualCodes / ScEmptyCodes / ScInFailCodes / ScOutFailCodes)
            modDefAPI.GetPrivateProfileString("Host", "ScDualCodes", "73,74", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_strScDualCodes = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("Host", "ScEmptyCodes", "75", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_strScEmptyCodes = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("Host", "ScInFailCodes", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_strScInFailCodes = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("Host", "ScOutFailCodes", "", sb, sb.Capacity, modDefApp.MAIN_INI);
            modDefApp.g_strScOutFailCodes = sb.ToString().Trim();

            //이중입고재지정횟수추가
            modDefAPI.GetPrivateProfileString("Property", "RE_DRCT_CNT", "", sb, sb.Capacity, modDefApp.MAIN_INI);
			if (Information.IsNumeric(modDefApp.g_strRE_DRCT_CNT) == false)
			{
				modDefApp.g_strRE_DRCT_CNT = "0";
			}
			else
			{
				modDefApp.g_strRE_DRCT_CNT = sb.ToString();
			}

			if (string.IsNullOrEmpty(modDefApp.g_User.g_strUserID))
				modDefApp.g_User.g_strUserID = "STD_USER";
			if (string.IsNullOrEmpty(modDefApp.g_User.g_strUserPassword))
				modDefApp.g_User.g_strUserPassword = "STD_USER";
        }
#endif


        public static bool WriteLog(ref LogMsgInfo p_MsgLog)
		{
			System.IO.FileStream fs = default(System.IO.FileStream);
			System.IO.StreamWriter sw = default(System.IO.StreamWriter);
			string strFileName = null;

			strFileName = modDefApp.LOG_DIR + Strings.Format(DateTime.Now, "yyyyMMdd") + ".Log";
			if (p_MsgLog.g_strType == modDefApp.MSG_NOR)
			{
				//Return True
			}

			try
			{
				fs = new System.IO.FileStream(strFileName, System.IO.FileMode.Append);
				sw = new System.IO.StreamWriter(fs);

				sw.WriteLine(p_MsgLog.g_strTime + ";" + p_MsgLog.g_strType + ";" + p_MsgLog.g_strMsg);
			}
			catch (Exception ex)
			{
				string strMsg = null;

				strMsg = "Log 파일 오류(" + ex.Message + ")";
				modCmWork.ShowMsgClient(strMsg, modDefApp.MSG_ERR, false);
			}

			if ((sw != null))
			{
				sw.Close();
			}
			if ((fs != null))
			{
				fs.Close();
			}

			return true;
		}

		public static bool DelLog()
		{
			// 제작년 파일 삭제
			string strFileName = null;

			strFileName = modDefApp.LOG_DIR + Strings.Format(DateTime.Now.AddYears(-2), "yyyy") + "*.Log";

			try
			{
				FileSystem.Kill(strFileName);
			}
			catch (Exception ex)
			{
				//Dim strMsg As String

				//strMsg = "Log 파일 삭제 오류(" & ex.Message & ")"
				//ShowMsgClient(strMsg, MSG_ERR, False)
			}
			return true;
		}

		//*** 응용 프로그램의 이전 인스턴스가 실행 중인지 여부를 확인 ***
		public static bool PrevInstance()
		{
			if (Information.UBound(System.Diagnostics.Process.GetProcessesByName(System.Diagnostics.Process.GetCurrentProcess().ProcessName)) > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}


	}
}
