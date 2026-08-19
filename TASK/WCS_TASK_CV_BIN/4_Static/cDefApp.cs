using System;
using System.Collections.Generic;
using System.Text;
using System.Data.OleDb;
using System.Threading;
using Microsoft.VisualBasic;
#if POSTGRESQL
using Npgsql; // [LGLS] POSTGRESQL 전용
#endif

namespace WCS_TASK_CV
{
    public class cDefApp
    {
        public static OleDbConnection GM_DB1_CN;            // @.DB연결 정의[OracleConnection]
#if POSTGRESQL
        public static NpgsqlConnection GM_DB2_CN;            // @.DB연결 정의[NpgsqlConnection]
#elif SQL
        public static System.Data.SqlClient.SqlConnection GM_DB2_CN; // [LGLS] @.DB연결 정의[SqlConnection]
#endif

        public const string GM_ENV_INI = "./WCS_DB.INI";   // @.INI 파일경로

        // @@.DB 종류
        public const string DB_TYPE_NONE = "0";
        public const string DB_TYPE_ORACLE = "1";
        public const string DB_TYPE_POSTGRESQL = "2";
        public const string DB_TYPE_MS_SQL = "3";
        public const string DB_TYPE_MY_SQL = "4";
        public const string DB_TYPE_SIZE = "5";

        // @@.[DB_TYPE] 접속정보
        public static string DB_TYPE_INI = "0"; // 0:None, 1:Oracle, 2:PostgreSql, 3:MS_SQL, 4:MY_SQL

        // @@.[DB_1] Oracle 접속정보
        public static string GM_DB1_PROVIDER = "";
        public static string GM_DB1_ALIAS = "";
        public static string GM_DB1_USERID = "";
        public static string GM_DB1_PASSWORD = "";

        // @@.[DB_2] PostgreSql 접속정보
        public static string GM_DB2_IP = "";
        public static string GM_DB2_DATABASE = "";
        public static string GM_DB2_PORT = "";
        public static string GM_DB2_USER = "";
        public static string GM_DB2_USER_PW = "";

        // @@.설비통신 정보
        // [CNF]
        public static string GM_WH_TYP = "";
        public static string GM_USERID = "";


        // @@.소켓통신 타임아웃 정의
        public static int GM_COMM_SND_TIME_OUT = 500;
        public static int GM_COMM_RCV_TIME_OUT = 500;
        

        public static Queue<LogParam>[] m_LogQ = new Queue<LogParam>[200];

        // @@.Application 종료 여부 설정 변수 선언
        public static bool  GM_STAT_MAIN = false;  // @.전체 시스템 종료 여부[메인 시스템이 종료 되면 전체 종료!]
        public static bool GM_RE_START = false;

        // @@.XGT 주소 표기: V1.1 매핑 고정 (V0.9는 2026-08-10 실 현장 판정으로 폐기 - 통신 불가·D영역 계산 불일치)
        //   이 플래그는 항상 false. 수정 금지 영역(FenetProtocol/CvThread)의 V0.9 분기를 사문화하기 위해 존치.
        public static bool GM_ADDR_V09 = false;

        // ─────────────────────────────────────────────────────────────────────
        // @@.R(트래킹) 영역 주소 해석 모드  [LGLS 2026-08-19 추가]
        //   PPT/문서의 R 표기(= (C/V번호-1)*10, S/C·RGV는 R0300~)를 실제 워드주소로 바꾸는 방식이
        //   구 ECS 와 현행이 서로 다르다. 현장/상대 시스템에 맞춰 전환할 수 있도록 스위치로 뺀다.
        //
        //   true (HEX) = 구 ECS 호환 : 문서 표기를 "16진 문자열"로 파싱한다.
        //                구 ECS(ECP.dll FenetDriver)가 TB_OBSERVABLE 의 ADDRESS_NO 를 R 포함 전부
        //                Convert.ToInt32(str,16) 으로 읽기 때문. 예) C/V#11 "0100" → 0x100 = 워드 256
        //   false(DEC) = 현행 10진   : 문서 표기를 그대로 10진 워드주소로 쓴다.
        //                예) C/V#11 "0100" → 워드 100 (%RB200)
        //
        //   ini [PLC] R_ADDR_MODE = HEX | DEC   (기본 HEX = 종전 통신 동작 유지)
        //   ※ R 영역에만 적용된다. M(비트)·D(워드) 주소 체계와 FEnet 프레임 규약(워드×2=바이트)은
        //     양 모드 공통이므로 건드리지 않는다.
        public static bool GM_R_ADDR_HEX = true;

        /// <summary>
        /// R 트래킹 문서표기(10진 자릿수 그대로의 정수) → 실제 전송 워드주소.
        /// R 주소를 다루는 모든 지점(CvThread/VehThread/메모리맵/시나리오테스트)은 이 함수만 거친다.
        /// </summary>
        public static int GsRTrackWord(int pDocNo)
        {
            if (!GM_R_ADDR_HEX) return pDocNo;              // 현행 : 10진 그대로
            try { return Convert.ToInt32(pDocNo.ToString(), 16); }   // 구 ECS : 표기를 16진으로 파싱
            catch { return pDocNo; }
        }

        /// <summary>현재 R 주소 모드 표시문자열 (로그/화면용)</summary>
        public static string GsRAddrModeText()
        {
            return GM_R_ADDR_HEX ? "구 ECS 호환(16진)" : "현행(10진)";
        }
        // ─────────────────────────────────────────────────────────────────────

        // @@.쓰레드 선언
        //public Thread  GM_TH_WC1;     // @.W/C1와 통신하는 쓰레드[소켓속성:Client]
    
        // @@.구분자
        public const string cSPA = ";";

        // @@.통신처리작업구조체
        public struct stutComProc
        {
            public bool    bMakeTgmSnd;     // @.전송Tgm작성
            public bool   bSndTgm;          // @.Tgm전송
            public bool   bRcvTgm;          // @.Tgm수신
            public int   nChkTgm;           // @.수신Tgm체크
            public bool   bDBProc;          // @.DB처리
            public string   sProcMsg;       // @.처리메세지

            public void  init()
                {                               // @.구조체 선언변수 초기화
                this.bMakeTgmSnd = false;         // @.전송Tgm작성
                this.bSndTgm = false;             // @.Tgm전송
                this.bRcvTgm = false;             // @.Tgm수신
                this.nChkTgm = 99;                // @.수신Tgm체크
                this.bDBProc = false;             // @.DB처리
                this.sProcMsg = "";               // @.처리메세지
                }
        }

        // @@.DB Err 상수
        public const int DB_ERR = -1;      // @.DB 에러
        public const int DB_LOCK = -2 ;    // @.DB 에러중 DB Lock
        public const int DB_DUP = -3;      // @.DB 에러중 중복 데이타

        // @@.enum 정의
        public enum eComSts{ComNor = 0, ComErr = 1};                     // @.통신상태
        public enum eLogMsgType{MSG_NOR = 0,MSG_IMP = 1,MSG_ERR = 2  };  // @.eLogMsgType[0:보통, 1:중요, 2:에러]
		public enum eLogWriteGbn { COMM1 = 0, COMM2 = 1, COMM3 = 2, COMM4 = 3, COMM5 = 4, COMM6 = 5, COMM7 = 6, COMM8 = 7, COMM9 = 8, COMM10 = 9 };
        
        // @@.Structure 정의
        public struct stutLogMsgInfo 
        {
            public string  Time;  
            public string  ID;  
            public string  MsgTyp; 
            public string  Com;  
            public string  Msg;  
            public string  Tgm;  
            public void  init()
            {
                this.Time = "";
                this.ID = "";
                this.MsgTyp = "";
                this.Com = "";
                this.Msg = "";
                this.Tgm = "";
            }
        }

        public const string  CRLF  = ControlChars.CrLf; // @.제어문자[vbCrLf]
        public const byte  STX  =   0x2 ;
        public const byte ETX   =  0x3 ;
        public   char  GM_STR_STX = Convert.ToChar(2);
        public   char GM_STR_ETX = Convert.ToChar(3);

    }
}
