using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualBasic;
#if oracle
	using System.Data.OleDb;
using System.Windows.Forms; 
#elif mssql
using System.Data.SqlClient;
using System.Windows.Forms;
#endif
#if postgresql
using Npgsql;
using NpgsqlTypes;
using System.Windows.Forms; 
#endif

namespace EcsClient
{
	class modCom
	{



        // 전역 객체 정의
        public const string DOWN_INI = ".\\\\WmsDown.ini";
		public const string CONFIG_INI = ".\\\\config.ini";
#if oracle
		public static OleDbConnection gconDb;
#elif mssql
		public static SqlConnection gconDb;
#elif postgresql
		public static NpgsqlConnection gconDb;
#endif

        public static string gPcIp;		
		public static string gPcNm;

		// sql 작성 상수
		public const string CRLF = ControlChars.CrLf;

        #region Wms DB Info
        public static string g_strWmsDBKey;
        public static string g_strWmsServerName;
        public static string g_strWmsDatabase;
        public static string g_strWmsDbUserID;
        public static string g_strWmsDbUserPass;
        #endregion
        #region Sap RFC Info
        public static string g_strSapKey;
        public static string g_strSapAppServerHost;
        public static string g_strSapClient;
        public static string g_strSapUser;
        public static string g_strSapPassword;
        public static string g_strSapSystemNumber;
        public static string g_strSapLanguage;
        public static string g_strSapPoolSize;
        public static string g_strSapIdleTimeout;
        #endregion

#if oracle
		public static bool DBLogIn(ref OleDbConnection pConObj)
		{
			string strDbAlias = "";
			string strDbUserID = "";
			string strDbUserPass = "";

			try
			{
				modCom.ReadInitProfile(ref strDbAlias,ref strDbUserID, ref strDbUserPass);

				pConObj = new OleDbConnection();

				//-------------------------------------
				// Provider=OraOLEDB.Oracle.1  Login
				//-------------------------------------
				pConObj.ConnectionString = "Provider=OraOLEDB.Oracle.1; Data Source = " + strDbAlias + "; User ID = " + strDbUserID + "; Password = " + strDbUserPass;


				pConObj.Open();
			}
			catch (Exception AppErr)
			{
				MessageBox.Show("서버 DB접속을 실패하였습니다. 관리자에게 문의하여 주십시요." + CRLF + AppErr.Message, "DB 접속 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}

			gPcNm = gPcIp + " (" + strDbAlias + ")";

			return true;
		}
#elif mssql
        public static bool DBLogIn(ref SqlConnection pConObj)
		{
            //string strServerName = "";
            //string strDatabase = "";
            //string strDbUserID = "";
            //string strDbUserPass = "";
            //string strDBTimeout = "";
            //Dim connStr As String

            string strRtnMsg = null;

            ReadInitProfile(ref strRtnMsg);

            pConObj = new SqlConnection();
            try
			{
                //ReadInitProfile(ref strServerName, ref strDatabase, ref strDbUserID, ref strDbUserPass, ref strDBTimeout);

                //pConObj = new SqlConnection();

                //pConObj.ConnectionString = "Server = " + strServerName + " ; Trusted_Connection = False ; Database = " + strDatabase + " ; User Id = " + strDbUserID + "; Password = " + strDbUserPass;
                //if (!string.IsNullOrEmpty(strDBTimeout))
                //{
                //	pConObj.ConnectionString = pConObj.ConnectionString + "; connection Timeout = " + strDBTimeout;
                //}

                //pConObj.Open();
                //g_Form.chkStopLogClient.Text = "로그중지   [" + g_strWmsDBKey + "]";  // 20180112 RYU 실행위치 구분을 위해 추가 + 20180123 노형선 INI DB Key 값으로 실행위치 구분
                // [LGLS 2026-09-30] USERID 를 비우면 Windows 인증으로 붙는다.
                //   종전에는 Trusted_Connection = False 로 박혀 있어,
                //   USERID 를 비우면 "사용자 ''이(가) 로그인하지 못했습니다" 가 났다.
                //   현장마다 계정 방식이 다르므로 둘 다 되게 한다.
                if (string.IsNullOrEmpty(g_strWmsDbUserID))
                    pConObj.ConnectionString = "Server = " + g_strWmsServerName
                                             + "; Database = " + g_strWmsDatabase
                                             + "; Integrated Security = SSPI";
                else
                    pConObj.ConnectionString = "Server = " + g_strWmsServerName
                                             + "; Trusted_Connection = False; Database = " + g_strWmsDatabase
                                             + "; User Id = " + g_strWmsDbUserID
                                             + "; Password = " + g_strWmsDbUserPass;
                pConObj.Open();

                if (!string.IsNullOrEmpty(strRtnMsg))
                {
                    return false;
                }
            }
			catch (Exception AppErr)
			{
				MessageBox.Show("서버 DB접속을 실패하였습니다. 관리자에게 문의하여 주십시요." + CRLF + AppErr.Message, "DB 접속 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}

			gPcNm = gPcIp;
			return true;

		}
#elif postgresql
        public static bool DBLogIn(ref NpgsqlConnection pConObj)
        {
            string strDbAlias = "";
            string strDbUserID = "";
            string strDbUserPass = "";

            try
            {
                string strRtnMsg = null;
                GsGetInitPorFileDB_2(ref strRtnMsg);

                pConObj = new NpgsqlConnection();

                //-------------------------------------
                // Provider=OraOLEDB.Oracle.1  Login
                //-------------------------------------

                //g_strWmsServerName = "10.17.17.24";     // TEST 시에

                pConObj.ConnectionString = "host=" + g_strWmsServerName + ";username=" + g_strWmsDbUserID + ";password=" + g_strWmsDbUserPass + ";database=" + g_strWmsDatabase + ";MAXPOOLSIZE=50;";
                pConObj.Open();
            }
            catch (Exception AppErr)
            {
                MessageBox.Show("서버 DB접속을 실패하였습니다. 관리자에게 문의하여 주십시요." + CRLF + AppErr.Message, "DB 접속 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            gPcNm = gPcIp + " (" + g_strWmsServerName + ")";

            return true;
        }
#endif
        public static void ReadInitProfile(ref string pAlias, ref string pUser, ref string pPwd)
		{
			string strApplicationName = "DB SERVER";
			StringBuilder sbAlias = new StringBuilder(30);
			StringBuilder sbUser = new StringBuilder(30);
			StringBuilder sbPwd = new StringBuilder(30);

			modDefAPI.GetPrivateProfileString(strApplicationName, "Alias", "", sbAlias, sbAlias.Capacity, DOWN_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "UserId", "", sbUser, sbUser.Capacity, DOWN_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "Password", "", sbPwd, sbPwd.Capacity, DOWN_INI);

			pAlias = sbAlias.ToString();
			pUser = sbUser.ToString();
			pPwd = sbPwd.ToString();
		}


		public static void ReadInitProfile(ref string pServerName, ref string pDatabase, ref string pUser, ref string pPwd, ref string pTimeout)
		{
			string strApplicationName = "DB SERVER";
			StringBuilder sbServerName = new StringBuilder(30);
			StringBuilder sbDatabase = new StringBuilder(30);
			StringBuilder sbUser = new StringBuilder(30);
			StringBuilder sbPwd = new StringBuilder(30);
			StringBuilder sbTimeout = new StringBuilder(30);

			modDefAPI.GetPrivateProfileString(strApplicationName, "ServerName", "", sbServerName, sbServerName.Capacity, DOWN_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "Database", "", sbDatabase, sbDatabase.Capacity, DOWN_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "UserId", "", sbUser, sbUser.Capacity, DOWN_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "Password", "", sbPwd, sbPwd.Capacity, DOWN_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "Timeout", "", sbTimeout, sbTimeout.Capacity, DOWN_INI);

			pServerName = sbServerName.ToString();
			pDatabase = sbDatabase.ToString();
			pUser = sbUser.ToString();
			pPwd = sbPwd.ToString();
			//pUser = DecodeConnectInfo(strApplicationName, "UserId")
			//pPwd = DecodeConnectInfo(strApplicationName, "Password")
			//pTimeout = sbTimeout.ToString
		}

        // [LGLS 2026-09-29] 접속정보를 WmsInfo.dll 대신 ini 에서 직접 읽는다.
        //   WmsInfo.dll 은 HUONS 현장 전용이다 - 키(INFO=C1)로 ★그 현장의★ 서버 주소를
        //   풀어 주는 물건이라, 여기에는 LGLS 서버가 들어 있지 않다.
        //   WmsDown.ini 의 [DB Server] 는 HUONS 판에도 이미 있던 섹션이므로 형식은 그대로다.
        //     [DB Server]
        //     SERVERNAME=localhost\SQLEXPRESS
        //     DATABASE=LGLS_MCS_IO
        //     USERID=sa            (비우면 Windows 인증)
        //     PASSWORD=****
        public static void ReadDbInfoFromIni(ref string p_strRtnMsg)
        {
            StringBuilder sb = new StringBuilder(256);

            modDefAPI.GetPrivateProfileString("DB Server", "SERVERNAME", "", sb, sb.Capacity, DOWN_INI);
            g_strWmsServerName = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("DB Server", "DATABASE", "", sb, sb.Capacity, DOWN_INI);
            g_strWmsDatabase = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("DB Server", "USERID", "", sb, sb.Capacity, DOWN_INI);
            g_strWmsDbUserID = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("DB Server", "PASSWORD", "", sb, sb.Capacity, DOWN_INI);
            g_strWmsDbUserPass = sb.ToString().Trim();

            // 무엇이 빠졌는지 정확히 알려 준다 - 값을 지어내면 엉뚱한 서버로 붙어 더 나쁘다.
            if (g_strWmsServerName.Length == 0 || g_strWmsDatabase.Length == 0)
            {
                p_strRtnMsg = "설정 파일에서 DB 접속 정보를 읽지 못했습니다." + CRLF + CRLF
                            + "  파일 : " + DOWN_INI + CRLF
                            + "  항목 : [DB Server] SERVERNAME / DATABASE" + CRLF + CRLF
                            + "설정 파일에 다음처럼 넣어 주세요." + CRLF
                            + "    [DB Server]" + CRLF
                            + "    SERVERNAME=localhost\\SQLEXPRESS" + CRLF
                            + "    DATABASE=LGLS_MCS_IO";
            }
        }

        //최초작성자	: 노형선
        //작성일		: 20180123
        //개요		    : Wms DB 접속정보를 DLL로 불러온다. 
        //수정이력		:	 
        public static void ReadInitProfile(ref string p_strRtnMsg)
        {
            //string strKeyData1 = null;
            //string strKeyData2 = null;

            //StringBuilder sb = new StringBuilder(30);
            //modDefAPI.GetPrivateProfileString("WMS", "INFO", "", sb, sb.Capacity, DOWN_INI);
            //strKeyData1 = sb.ToString();
            //g_strWmsDBKey = strKeyData1;
            //modDefAPI.GetPrivateProfileString("SAP", "INFO", "", sb, sb.Capacity, DOWN_INI);
            //strKeyData2 = sb.ToString();
            //g_strSapKey = strKeyData2;

            //if (string.IsNullOrEmpty(strKeyData1) || string.IsNullOrEmpty(strKeyData2))
            //{
            //    p_strRtnMsg = "설정값이 되어있지 않습니다." + CRLF + "Wms 담당자에게 연락주세요.";
            //    return;
            //}

            //WmsInfo.CWmsInfo.WmsDBInfo(ref g_strWmsServerName, ref g_strWmsDatabase, ref g_strWmsDbUserID, ref g_strWmsDbUserPass, strKeyData1);
            //WmsInfo.CWmsInfo.SapRFCInfo(ref g_strSapAppServerHost, ref g_strSapClient, ref g_strSapUser, ref g_strSapPassword, ref g_strSapSystemNumber
            //                          , ref g_strSapLanguage, ref g_strSapPoolSize, ref g_strSapIdleTimeout, strKeyData2);
            //string strKeyData1 = null;
            //string strKeyData2 = null;

            //StringBuilder sb = new StringBuilder(30);
            //modDefAPI.GetPrivateProfileString("WMS", "INFO", "", sb, sb.Capacity, modDef.MAIN_INI);
            //strKeyData1 = sb.ToString();
            //modDefApp.g_strWmsDBKey = strKeyData1;
            //modDefAPI.GetPrivateProfileString("SAP", "INFO", "", sb, sb.Capacity, modDef.MAIN_INI);
            //strKeyData2 = sb.ToString();
            //modDefApp.g_strSapKey = strKeyData2;

            //if (string.IsNullOrEmpty(strKeyData1) || string.IsNullOrEmpty(strKeyData2))
            //{
            //	p_strRtnMsg = "설정값이 되어있지 않습니다." + modDefApp.CRLF + "Wms 담당자에게 연락주세요.";
            //	return;
            //}

            //WmsInfo.CWmsInfo.WmsDBInfo(ref modDefApp.g_strWmsServerName, ref modDefApp.g_strWmsDatabase, ref modDefApp.g_strWmsDbUserID, ref modDefApp.g_strWmsDbUserPass, strKeyData1);
            //WmsInfo.CWmsInfo.SapRFCInfo(ref modDefApp.g_strSapAppServerHost, ref modDefApp.g_strSapClient, ref modDefApp.g_strSapUser, ref modDefApp.g_strSapPassword, ref modDefApp.g_strSapSystemNumber
            //						  , ref modDefApp.g_strSapLanguage, ref modDefApp.g_strSapPoolSize, ref modDefApp.g_strSapIdleTimeout, strKeyData2);

            // [LGLS 2026-09-29] WmsInfo.dll(HUONS 전용) 대신 ini 의 평문 항목을 읽는다.
            //   SAP 연계는 이 현장에 없으므로 함께 지웠다.
            ReadDbInfoFromIni(ref p_strRtnMsg);
        }

        #region [DB_2]::PostgreSql 접속정보
        public static void GsGetInitPorFileDB_2(ref string p_strRtnMsg)
        {

            // [LGLS 2026-09-29] WmsInfo.dll(HUONS 전용) 대신 ini 의 평문 항목을 읽는다.
            //   SAP 연계는 이 현장에 없으므로 함께 지웠다.
            ReadDbInfoFromIni(ref p_strRtnMsg);
        }
        #endregion
        //------------------------------------------

        public static void ReadInitProfile_C(ref string pCommon, string pSec, string pItem)
		{
			StringBuilder sbCommon = new StringBuilder(100);

			modDefAPI.GetPrivateProfileString(pSec, pItem, "", sbCommon, sbCommon.Capacity, DOWN_INI);

			pCommon = sbCommon.ToString();
		}

		public static void ReadInitProfile_i(ref double pCommon, string pSec, string pItem)
		{
			pCommon = modDefAPI.GetPrivateProfileInt(pSec, pItem, 0, DOWN_INI);
		}

		public static string ReadInitProfileStr(string pSec, string pItem)
		{
			StringBuilder sbValue = new StringBuilder(100);

			modDefAPI.GetPrivateProfileString(pSec, pItem, "", sbValue, sbValue.Capacity, DOWN_INI);

			return sbValue.ToString();
		}

		public double ReadInitProfileNum(string pSec, string pItem)
		{
			return modDefAPI.GetPrivateProfileInt(pSec, pItem, 0, DOWN_INI);
		}

		public static void WriteInitProfile(string pSec, string pItem, string pCommon)
		{
			modDefAPI.WritePrivateProfileString(pSec, pItem, pCommon, DOWN_INI);
		}

		public static void WriteInitProfile(string pSec, string pItem, ref double pCommon)
		{
			modDefAPI.WritePrivateProfileString(pSec, pItem, pCommon.ToString(), DOWN_INI);
		}


	}	
}
