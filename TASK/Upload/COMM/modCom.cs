using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualBasic;
using System.Data.OleDb;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Data;
using Npgsql;

namespace WmsUp
{
	 class modCom
	{		
		//*********************************************************************************************
		// 전역 객체 정의
		public const string UP_INI = ".\\WmsUp.ini";
		//#Ins 2014.10.31 HDS
		
		public const string CONFIG_INI = ".\\\\config.ini";

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
        public static OleDbConnection gconDb;
#elif mssql
        public static SqlConnection gconDb;
#elif postgresql
        public static NpgsqlConnection gconDb;
#endif

        public static string g_strPcIp;
		public static string g_strPcNm;
		public static string g_strPcIni_Path;
		
		public static string g_strDatabase;
		// DB 에러
		public const int DB_ERR = -1;
		// DB 에러중 DB Lock
		public const int DB_LOCK = -2;
		// DB 에러중 중복 데이타
		
		public const int DB_DUP = -3;
		// sql 작성 상수
		public const string CRLF = ControlChars.CrLf;
		//*********************************************************************************************

		//*** DB Log In ***********************************************
		// DB 관련 프로젝트는 모두 사용하므로 여기에 두기로 한다.
#if oracle
		public static bool DBLogIn(ref OleDbConnection pConObj)
		{
			string strDbAlias = "";
			string strDbUserID = "";
			string strDbUserPass = "";

			try
			{
				ReadInitProfile(ref strDbAlias, ref strDbUserID, ref strDbUserPass);

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

			g_strPcNm = g_strPcIp + " (" + strDbAlias + ")";
			return true;

		}
#elif mssql
		public static bool DBLogIn(ref SqlConnection pConObj)
		{
            //string strServerName = "";
            //string strDatabase = "";
            //string strDbUserID = "";
            //string strDbUserPass = "";

            string strRtnMsg = null;

            //ReadInitProfile(ref strServerName, ref strDatabase, ref strDbUserID, ref strDbUserPass);
            ReadInitProfile(ref strRtnMsg);

            try
			{
				//ReadInitProfile(ref strServerName, ref strDatabase, ref strDbUserID, ref strDbUserPass);

				pConObj = new SqlConnection();

				// [LGLS 2026-09-30] USERID 를 비우면 Windows 인증으로 붙는다.
				//   종전에는 Trusted_Connection = False 로 박혀 있어,
				//   USERID 를 비우면 "사용자 ''이(가) 로그인하지 못했습니다" 가 났다.
				if (string.IsNullOrEmpty(g_strWmsDbUserID))
					pConObj.ConnectionString = "Server = " + g_strWmsServerName
											 + " ; Database = " + g_strWmsDatabase
											 + " ; Integrated Security = SSPI ; connection Timeout = 50";
				else
					pConObj.ConnectionString = "Server = " + g_strWmsServerName
											 + " ; Trusted_Connection = False ; Database = " + g_strWmsDatabase
											 + " ; User Id = " + g_strWmsDbUserID
											 + "; Password = " + g_strWmsDbUserPass + ";connection Timeout = 50";
				pConObj.Open();
			}
			catch (Exception AppErr)
			{
				MessageBox.Show("서버 DB접속을 실패하였습니다. 관리자에게 문의하여 주십시요." + CRLF + AppErr.Message, "DB 접속 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}

			g_strPcNm = g_strPcIp;
			return true;
		}
#elif postgresql
        public static bool DBLogIn(ref NpgsqlConnection pConObj)
         {
            string strDbIp = "";
			string strDbDatabase = "";
			string strDbPort = "";
            string strDbUser = "";
            string strDbUserPw = "";

			try
			{

                string strRtnMsg = null;
                GsGetInitPorFileDB_2(ref strRtnMsg);

				pConObj = new NpgsqlConnection();

				//-------------------------------------
				// Provider=OraOLEDB.Oracle.1  Login
				//-------------------------------------
				//g_strWmsServerName = "10.17.17.24";

				pConObj.ConnectionString = "host=" + g_strWmsServerName + ";username=" + g_strWmsDbUserID + ";password=" + g_strWmsDbUserPass + ";database=" + g_strWmsDatabase + ";MAXPOOLSIZE=50;";
				pConObj.Open();
			}
			catch (Exception AppErr)
			{
				MessageBox.Show("서버 DB접속을 실패하였습니다. 관리자에게 문의하여 주십시요." + CRLF + AppErr.Message, "DB 접속 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}

            g_strPcNm = g_strPcIp + " (" + g_strWmsServerName + ")";
			return true;
         }
#endif

        //최초작성자	: 노형선
        //작성일		: 20180123
        //개요		    : WMS DB 접속정보 DLL로 Get
        //수정이력		:	
        // [LGLS 2026-09-30] 접속정보를 WmsInfo.dll 대신 ini 에서 직접 읽는다.
        //   WmsInfo.dll 은 앞 현장(HUONS) 전용이다 - 키(INFO=P1)로 그 현장의 서버 주소를
        //   풀어 주는 물건이라, 여기에는 LGLS 서버가 들어 있지 않다.
        //   Download 와 같은 자리를 읽는다.
        //     [DB Server]
        //     SERVERNAME=localhost\\\\SQLEXPRESS
        //     DATABASE=LGLS_MCS_IO
        //     USERID=sa
        //     PASSWORD=****
        public static void ReadDbInfoFromIni(ref string p_strRtnMsg)
        {
            StringBuilder sb = new StringBuilder(256);

            modDefAPI.GetPrivateProfileString("DB Server", "SERVERNAME", "", sb, sb.Capacity, UP_INI);
            g_strWmsServerName = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("DB Server", "DATABASE", "", sb, sb.Capacity, UP_INI);
            g_strWmsDatabase = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("DB Server", "USERID", "", sb, sb.Capacity, UP_INI);
            g_strWmsDbUserID = sb.ToString().Trim();
            modDefAPI.GetPrivateProfileString("DB Server", "PASSWORD", "", sb, sb.Capacity, UP_INI);
            g_strWmsDbUserPass = sb.ToString().Trim();

            // 무엇이 빠졌는지 정확히 알려 준다 - 값을 지어내면 엉뚱한 서버로 붙어 더 나쁘다.
            if (g_strWmsServerName.Length == 0 || g_strWmsDatabase.Length == 0)
            {
                p_strRtnMsg = "설정 파일에서 DB 접속 정보를 읽지 못했습니다." + CRLF + CRLF
                            + "  파일 : " + UP_INI + CRLF
                            + "  항목 : [DB Server] SERVERNAME / DATABASE";
            }
        }

        public static void ReadInitProfile(ref string p_strRtnMsg)
        {
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

            // [LGLS 2026-09-30] WmsInfo.dll(앞 현장 전용) 대신 ini 의 평문 항목을 읽는다.
            //   SAP 연계는 이 현장에 없으므로 함께 지웠다.
            ReadDbInfoFromIni(ref p_strRtnMsg);
        }
        public static void ReadInitProfile(ref string pAlias, ref string pUser, ref string pPwd)
		{
			string strApplicationName = "DB Server";
			StringBuilder sbAlias = new StringBuilder(30);
			StringBuilder sbUser = new StringBuilder(30);
			StringBuilder sbPwd = new StringBuilder(30);

			modDefAPI.GetPrivateProfileString(strApplicationName, "SERVERNAME", "", sbAlias, sbAlias.Capacity, UP_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "UserId", "", sbUser, sbUser.Capacity, UP_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "Password", "", sbPwd, sbPwd.Capacity, UP_INI);

			pAlias = sbAlias.ToString();
			pUser = sbUser.ToString();
			pPwd = sbPwd.ToString();
		}


		public static void ReadInitProfile(ref string pServerName, ref string pDatabase, ref string pUser, ref string pPwd)
		{

			string strApplicationName = "DB Server";
			StringBuilder sbServerName = new StringBuilder(30);
			StringBuilder sbDatabase = new StringBuilder(30);
			StringBuilder sbUser = new StringBuilder(30);
			StringBuilder sbPwd = new StringBuilder(30);
			StringBuilder sbTimeout = new StringBuilder(30);

			modDefAPI.GetPrivateProfileString(strApplicationName, "ServerName", "", sbServerName, sbServerName.Capacity, UP_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "Database", "", sbDatabase, sbDatabase.Capacity, UP_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "UserId", "", sbUser, sbUser.Capacity, UP_INI);
			modDefAPI.GetPrivateProfileString(strApplicationName, "Password", "", sbPwd, sbPwd.Capacity, UP_INI);

			pServerName = sbServerName.ToString();
			pDatabase = sbDatabase.ToString();
			pUser = sbUser.ToString();
			pPwd = sbPwd.ToString();
		}

        #region [DB_2]::PostgreSql 접속정보
        public static void GsGetInitPorFileDB_2(ref string p_strRtnMsg)
        {

            // [LGLS 2026-09-30] WmsInfo.dll(앞 현장 전용) 대신 ini 의 평문 항목을 읽는다.
            //   SAP 연계는 이 현장에 없으므로 함께 지웠다.
            ReadDbInfoFromIni(ref p_strRtnMsg);
        }
        #endregion

		public static void ReadInitProfile_C(ref string pCommon, string pSec, string pItem)
		{
			StringBuilder sbCommon = new StringBuilder(100);

			modDefAPI.GetPrivateProfileString(pSec, pItem, "", sbCommon, sbCommon.Capacity, UP_INI);

			pCommon = sbCommon.ToString();
		}

		public void ReadInitProfile_i(ref double pCommon, string pSec, string pItem)
		{
			pCommon = 0;
			pCommon = modDefAPI.GetPrivateProfileInt(pSec, pItem, 0, UP_INI);
		}


		public static void WriteInitProfile_C(string pSec, string pItem,  string pCommon)
		{
			modDefAPI.WritePrivateProfileString(pSec, pItem, pCommon, string.Copy(g_strPcIni_Path));
		}


		public static string ValidateInput(string tmpStr)
		{
			//SQL 문장에 위험한 문장이 있으면 에러처리한다.
			if ((string.IsNullOrEmpty(tmpStr) | tmpStr.Length < 3))
			{
				return "";
			}

			string[] pattern = {
					"'--",
					"'or",
					"'union",
					"')--",
					";select",
					";insert",
					";update",
					";delete",
					";alter",
					";create",
					";drop",
					";exec",
					";grant",
					";revoke",
					";rename",
					";trunc",
					";shutdown"
			};

			string msgStr = "";

			tmpStr = tmpStr.ToLower();
			tmpStr = tmpStr.Replace(" ", "");

			try
			{
				foreach (string one in pattern)
				{
					if (tmpStr.IndexOf(one) != -1)
					{
						msgStr = "입력한 요청에 잠재적으로 위험한 클라이언트 입력 값을 발견하여, 요청이 취소되었습니다." + "(" + one + ")";

						throw new Exception(msgStr);
					}
				}

			}
			catch (Exception ex)
			{
				return (ex.Message);
			}

			return "";
		}
	}
}
