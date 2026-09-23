using System;
using System.Collections.Generic;
using System.Text;
using System.Data.OleDb;
using System.Threading;
using Microsoft.VisualBasic;

namespace WCS_TASK_CV
{
    public class cDefApp
    {
        public static OleDbConnection GM_DB1_CN;            // @.DB연결 정의[OracleConnection]

        public const string GM_ENV_INI = "./WCS_DB.INI";   // @.INI 파일경로

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
        //   ini [PLC] R_ADDR_MODE = HEX | DEC
        //   [확정 2026-09-01] 실PLC(테스트 베드) 대사 결과 ★DEC(10진)가 맞다★ (사용자 확인).
        //   기본값을 DEC 로 바꾼다. HEX 해석·라디오 전환 기능은 유지(확정이지만 기능 삭제 금지 - 사용자 지시).
        //   ※ R 영역에만 적용된다. M(비트)·D(워드) 주소 체계와 FEnet 프레임 규약(워드×2=바이트)은
        //     양 모드 공통이므로 건드리지 않는다.
        public static bool GM_R_ADDR_HEX = false;

        // [LGLS 2026-09-01] D 블록 해석 모드 (R 과 대칭). true=DOC(문서표기 10진, 확정) / false=LEGACY(구 ezMCS 환산).
        //   실PLC 대사로 DOC 확정 - LEGACY 는 전환용 보존(기능 삭제 금지, 사용자 지시).
        public static bool GM_D_ADDR_DOC = true;
        // [LGLS 2026-09-15] C/V 방향 워드의 입고/출고 부호 (WCS_DB.INI [PLC] DIR_CODE = IN0 | IN1, SYS_MAIN 라디오)
        //   false(IN0, 현행) : 입고='0'(0x30) 출고='1'(0x31)   /   true(IN1) : 입고='1' 출고='0'
        //   현장 PLC 담당자가 래더에서 확인한 값이 입고=1/출고=0 이라 바꿔 쓸 수 있게 둔다. 쓰기·판독 양쪽에 적용.
        public static bool GM_DIR_IN1 = false;
        // [LGLS 2026-09-23] C/V#11 은 따로 잡는다 (사용자 지시).
        //   현장 조작반에 C/V#11 전용 입고/출고 모드 스위치가 있어 부호 규약이 다를 수 있다.
        //   WCS_DB.INI [PLC] DIR_CODE_CV11. 값이 없으면 DIR_CODE 를 따른다(하위 호환).
        public const int DIR_CV11 = 11;
        public static bool GM_DIR_IN1_CV11 = false;
        /// <summary>그 설비의 방향 부호가 IN1(입고=1/출고=0) 인가. nCvNo 는 C/V 기계번호(11 이면 C/V#11).</summary>
        public static bool GsDirIn1(int nCvNo) { return (nCvNo == DIR_CV11) ? GM_DIR_IN1_CV11 : GM_DIR_IN1; }
        public static byte GsDirChar(bool bOut, int nCvNo) { return (byte)((bOut ^ GsDirIn1(nCvNo)) ? 0x31 : 0x30); }
        public static byte GsDirChar(bool bOut) { return GsDirChar(bOut, 0); }   // 설비 미지정 - 공통 부호
        public static string GsDirModeText(bool bIn1) { return bIn1 ? "입고=1 / 출고=0 (IN1)" : "입고=0 / 출고=1 (IN0, 현행)"; }
        public static string GsDirModeText() { return GsDirModeText(GM_DIR_IN1); }
        // [LGLS 2026-09-02] 구 트랙테이블(트랙x10) D영역 지시 쓰기 - V1.1 확정 주소(크레인 D160~/RGV D210/방향 D300~)와
        //   정면 충돌(RGV 상태를 0으로 덮어씀)해 기본 차단. 구 방식 PLC 필요 시 INI [PLC] CV_DTRACK_WRITE=ON.
        public static bool GM_CV_DTRACK_WRITE = false;
        public static string GsDAddrModeText() { return GM_D_ADDR_DOC ? "문서 10진(DOC)" : "구 환산(LEGACY)"; }

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

        // @@.DB Err 상수
        public const int DB_ERR = -1;      // @.DB 에러
        public const int DB_LOCK = -2 ;    // @.DB 에러중 DB Lock
        public const int DB_DUP = -3;      // @.DB 에러중 중복 데이타

        // @@.enum 정의
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

    }
}
