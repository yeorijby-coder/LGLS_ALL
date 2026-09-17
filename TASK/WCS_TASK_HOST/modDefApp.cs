using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.VisualBasic;
using System.Data.OleDb;
using System.Threading;
using System.Collections;
using System.Data.SqlClient;
using Npgsql;


namespace TSK_HostCom
{
	class modDefApp
	{
		public static Boolean g_blListenThread, g_blSrvThread;
		//*********************************************************************************************   
	    // 전역 변수 정의
		public static UserInfo g_User;
		public static string g_strUserID;
		public static string g_strUserName;
		// Socket Listen Port
		public static int g_iListenPort;
		// Remote IP
		public static string g_strRemoteIP;
		// Remote Port
		public static int g_iRemotePort;
        
        public static bool[] g_bEmtpyPltJob = new bool[10000];

		public static string g_strPcNm;
		public static bool g_blSTOP_REQ;
        public static string g_strSYS_LANG = "LANG1";
        public static string COMPANY_CD = "F100";
        public static string AREA_CD = "F101";
        public static string WH_TYP = "10";
		public static string g_strRE_DRCT_CNT;
		// [LGLS 2026-08-30] 재지정 작업상태 (common_code 'JOB_STATUS' 코드값)
		//   09 = 이중입고 에러 / 08 = 공출고 에러 / 07 = 이중입고 재지정 / 06 = 공출고 재지정
		//   ※ EN_JOB_STATUS enum 값(9, 10)을 그대로 쓰면 안 된다 - 현행 체계에서 10 은 "CV 구동대기"다.
		public const string JOB_ST_DUAL_RETRY  = "06";  // [LGLS 2026-08-30] 07 → 06
		public const string JOB_ST_EMPTY_RETRY = "05";  // [LGLS 2026-08-30] 06 → 05
		// [LGLS 2026-08-30] 크레인 에러코드 마스터(EQP_ECD_MST.EQP_TYP) 구분.
		//   현장 크레인은 SFA 제작이라 이중입고/공출고 의미코드(0054~0059)가 'SC_SFA' 에 있다.
		//   'SC' 는 무라타 기계코드표(같은 번호, 다른 의미)라 0058(공출고)이 아예 없다.
		//   EcsComA.ini [Host] ScErrCodeType 으로 전환.
		// [LGLS 2026-09-17] PLC 알람 리스트(260917_1동 자동창고_알람리스트.xlsx) 적용 → 기본 'SC_LGLS'.
		//   지상반 코드 11~93·101~118 은 SC_SFA 의 같은 번호와 뜻이 달라 별도 구분으로 넣었다.
		public static string g_strScErrCodeTyp = "SC_LGLS";
		// [LGLS 2026-09-17] 크레인 에러의 상위 보고 종류(ErrorKind) 판정 코드 - [Host] ScDualCodes 등.
		//   알람 리스트 기준 : 73 좌측 렉 이중입고 / 74 우측 렉 이중입고 / 75 공출고. 입고·출고장애 코드는 없다.
		//   (종전 SC_SFA 기준 54·55 이중입고 / 56 입고장애 / 57 출고장애 / 58·59 공출고)
		public static string g_strScDualCodes    = "73,74";
		public static string g_strScEmptyCodes   = "75";
		public static string g_strScInFailCodes  = "";
		public static string g_strScOutFailCodes = "";

		/// <summary>[LGLS 2026-09-17] 코드 목록("73,74")에 에러코드("0073")가 들어 있는지 - 숫자로 비교한다.</summary>
		public static bool IsCodeIn(string pList, string pCode)
		{
			int nCode;
			if (pList == null || !int.TryParse(("" + pCode).Trim(), out nCode) || nCode == 0) return false;
			foreach (string s in pList.Split(new char[] { ',', ' ' }, StringSplitOptions.RemoveEmptyEntries))
			{
				int n;
				if (int.TryParse(s.Trim(), out n) && n == nCode) return true;
			}
			return false;
		}
        public static string GM_RTN_MSG;

#if ORACLE
public const string II = "||";
public const string NVL = "NVL";
#endif
#if SQL
public const string II = "+";
public const string NVL = "ISNULL";
#endif
#if POSTGRESSQL
public const string II = "+";               // ????
public const string NVL = "COALESCE";
#endif
        //*********************************************************************************************

		//*********************************************************************************************
// 전역 객체 정의
#if ORACLE
public static OleDbConnection g_conDb;
#endif
#if SQL
	public static SqlConnection g_conDb;
#endif
#if POSTGRESSQL
    public static NpgsqlConnection g_conDb;
#endif

public static frmMain g_frmForm;
		// app 종료 여부
		public static bool g_blIsAppExit;

		// 서버소켓용 작업 Object
		public static CSrvWork g_SrvWork = new CSrvWork();
		// 클라이언트소켓용 작업 Object

		public static CCliWork g_CliWork = new CCliWork();
		// 디폴트 Read Timeout
		public const int TIME_OUT = 5;

		public static AutoResetEvent[] g_areLogExitEvent = new AutoResetEvent[2];

		// Listen Socket
		public static System.Net.Sockets.TcpListener g_tcplsn;
		// Socket 동기화 객체
		public static object g_objSockSync = new object();
		// Log 메세지 리스트
		//Public gLogList As New ArrayList
		public static  ArrayList[] g_arrlstLogList = new ArrayList[2];

		//*********************************************************************************************

		//*********************************************************************************************
		// 전역 상수 정의
		public const string MAIN_INI = ".\\\\ECSCOMA.ini";

		public const string LOG_DIR = ".\\\\ECSCOMALog\\\\";
		// DB 에러
		public const int DB_ERR = -1;
		// DB 에러중 DB Lock
		public const int DB_LOCK = -2;
		// DB 에러중 중복 데이타

		public const int DB_DUP = -3;
		// Msg 종류
		// 보통
		public const string MSG_NOR = "NOR";
		// 중요
		public const string MSG_IMP = "IMP";
		// 에러
		public const string MSG_ERR = "ERR";


		// 제어문자
		public const string CRLF = ControlChars.CrLf;
		public const int STX = 0x2;
		public const int ETX = 0x3;


		public const string MSG_ACK = "A";	//- Response Ack
		public const string MSG_NAK = "N";	//- Response Nak

		public const   int MSG_HEAD_CNT = 15;

		// Interface목록서[ECS-자동창고] 기준 MailBox 명 (Header 10자리)
		public const string MSG_MAILBOX_SEND = "ECS_MBX";	// WCS(ECS역할) -> HOST(WMS/IMS)
		public const string MSG_MAILBOX_RECV = "WMS_MBX";	// HOST(WMS/IMS) -> WCS(ECS역할)

		// WareHouse Define (A:PalletRack자동창고, B:P-BoxRack자동창고) - INI [Host]WarehouseDefine
		public static string WH_DEFINE = "A";

		// Interface목록서 기준 Body 길이(STX~ETX 포함)
		// [LGLS] ECS 원본(WMSCommand.cs) 규격으로 변경: O=66 (WhDefine/PairLuggNo/UserData 없음,
		//        PalletNo 7자). 구형 104바이트 규격은 폐기.
		// [LGLS 2026-07-30] WMS-ECS 인터페이스명세서(20100311)·원본 ECP(WMSCommand.cs) 실측으로 정합:
		//        M=7 (STX+'M'+CvNo3+Mode1+ETX, 구 11=예비4 포함), R=31 (WhDefine 자리 없음, LuggNo가 body[3]),
		//        응답=11 (LuggNo 뒤가 DeviceNo(3)가 아니라 StepCount(1)). S=43 집계/E=23 은 CCliWork 참조.
		public const int MSG_ORDER_BODY_CNT = 66;		// 'O' 작업지시
		public const int MSG_MODE_BODY_CNT = 7;			// 'M' 모드변경 (C/V#11 RGV 방향)
		public const int MSG_REDIRECT_BODY_CNT = 31;	// 'R' 재작업지시
		public const int MSG_RSPS_BODY_CNT = 11;		// 응답 메세지

		
		//  Error Message Code
		public const int MSG_NO_ERROR = 0;
		public const int MSG_NO_STX = 1;
		public const int MSG_NO_ETX = 2;
		public const int MSG_LUGG_NO_DUPLICATED = 3;
		public const int MSG_NO_EXIST_JOB = 4;
		public const int MSG_ECS_BUFFER_FULL = 5;
		public const int MSG_INVALID_HEADER_LENG = 6;
		public const int MSG_INVALID_MSG_LENG = 7;
		public const int MSG_INVALID_LOC = 8;
		public const int MSG_INVALID_STN_NO = 9;
		public const int MSG_INVALID_LUGG_NO = 10;
		public const int MSG_UNKNOWN_JOB_TYPE = 11;
		public const int MSG_INHIBITED_LOC = 12;
		public const int MSG_INVALID_MAILBOX_NAME = 13;
		public const int MSG_ALREADY_INVOKED_JOB = 14;
		public const int MSG_INVALID_CONTENT = 15;
		public const int MSG_UNKNOWN_MSG_TYPE = 16;
		public const int MSG_IMPROPER_HANDSHAKE = 17;
		public const int MSG_NO_RESPONSE = 18;
		public const int MSG_SOCKET_ERROR = 19;
		public const int MSG_INTERNAL_ERROR = 99;

		// 조회조건 중 '전체'
		//Public Const ALL_QRY = "전체"

		// PDA Client 수

		// 통신상태
		public enum ComSts : int
		{
			ComNor = 0,
			ComErr = 1
		}
		
        #region 작업정보 처리 상수들
        public enum EN_JOB_TYPE : int
        {
            enJobTypeNone = 0,
            enJobTypeAutoSto = 1, enJobTypeAutoRet = 2, enJobTypeAutoPR = 3, enJobTypeAutoR2R = 4, enJobTypeAutoW2W = 5, enJobTypeAutoMove = 6,
            enJobTypeSemiSto = 11, enJobTypeSemiRet = 12, enJobTypeSemiPR = 13, enJobTypeSemiR2R = 14, enJobTypeSemiW2W = 15, enJobTypeSemiMove = 16,
            enJobTypeManual = 21
        };
        public enum EN_JOB_PATTERN : int
        {
            enJobPatternNone,
            enJobPatternSto, enJobPatternRet, enJobPatternPR, enJobPatternR2R, enJobPatternW2W, enJobPatternMove
        };

        public	enum EN_LENGTH : int 
        {
            enLengthStation = 3, 
            enLengthLocation = 7,
            enLengthControlID = 10
        };

        public enum EN_STN_KIND : int 
        { 
            enScStation, enStoStation, enRetStation, enArvStation 
        };

        #endregion

        #region 작업정보 처리 함수들
        public static int ConvertJobPattern(int nJobType)
        {
            switch (nJobType)
            {
                case (int)EN_JOB_TYPE.enJobTypeAutoSto:
                case (int)EN_JOB_TYPE.enJobTypeSemiSto:
                    return (int)EN_JOB_PATTERN.enJobPatternSto;

                case (int)EN_JOB_TYPE.enJobTypeAutoRet:
                case (int)EN_JOB_TYPE.enJobTypeSemiRet:
                    return (int)EN_JOB_PATTERN.enJobPatternRet;

                case (int)EN_JOB_TYPE.enJobTypeAutoR2R:
                case (int)EN_JOB_TYPE.enJobTypeSemiR2R:
                    return (int)EN_JOB_PATTERN.enJobPatternR2R;

                case (int)EN_JOB_TYPE.enJobTypeAutoMove:
                case (int)EN_JOB_TYPE.enJobTypeSemiMove:
                    return (int)EN_JOB_PATTERN.enJobPatternMove;

                case (int)EN_JOB_TYPE.enJobTypeAutoW2W:
                case (int)EN_JOB_TYPE.enJobTypeSemiW2W:
                    return (int)EN_JOB_PATTERN.enJobPatternW2W;
            }
            return (int)EN_JOB_PATTERN.enJobPatternNone;
        }
        public static bool IsOnlineJobType(int nJobType)
        {
            switch (nJobType)
            {
                case (int)EN_JOB_TYPE.enJobTypeAutoSto:
                case (int)EN_JOB_TYPE.enJobTypeAutoRet:
                case (int)EN_JOB_TYPE.enJobTypeAutoR2R:
                case (int)EN_JOB_TYPE.enJobTypeAutoPR:
                case (int)EN_JOB_TYPE.enJobTypeAutoMove:
                case (int)EN_JOB_TYPE.enJobTypeAutoW2W:	
                    return true;
            }

            return false;
        }
        public static bool IsOnlineLuggNum(int nLuggNum)
        {
            return ((nLuggNum > 0) && (nLuggNum < 9000)) ? true : false;
        }


        public static int GetBank(string strLocation)
        {
	        if (strLocation.Length != (int)EN_LENGTH.enLengthLocation)
		        return 0;

            return Convert.ToInt32(strLocation.Substring(0, 2));
        }

        public static int GetStackerNum(int nWarehouse, string strLocation)
        {
            if (strLocation.Length != (int)EN_LENGTH.enLengthLocation)
                return 0;

            int nScNum = (GetBank(strLocation) + 1) / 2;

            return nScNum;
        }



        #endregion 
      	
        public const int	SC_STATUS_NORMAL_WAIT			= 0;
        public const int	SC_STATUS_STORING				= 1;
        public const int	SC_STATUS_RETRIEVING			= 2;
        public const int	SC_STATUS_RACK_TO_RACK			= 3;
        public const int	SC_STATUS_NO_ONLINE				= 4;
        public const int	SC_STATUS_ERROR					= 5;
        public const int	SC_STATUS_SUSPEND_STO			= 6;
        public const int	SC_STATUS_SUSPEND_RET			= 7;
        public const int	SC_STATUS_SUSPEND_ALL			= 8;

		//*********************************************************************************************

		//**************************************************************************************
	    //Structure 정의

		// DB User Info
		public struct UserInfo
		{
			public string g_strUserID;
			public string g_strUserName;
			public string g_strUserPassword;
			public string g_strDbAlias;
			//mssql
			public string g_strDatabase;

            // PostgresSql
            public string g_strDbIP;
            public string g_strDbPort;
		}

		//**************************************************************************************
	}
}
