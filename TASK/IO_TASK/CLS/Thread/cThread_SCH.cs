// 작성자   : LG화학 WCS Scheduler
// 작성일   : 2026-06-23 (2026-06-27 기존 JOB_MST 스키마 통합)
// 개요     : PPT 시나리오(ECS-분석-PLC사양및시나리오_LG화학_V0.9) 기반 자동 반송 Scheduler
//
//  ※ 이번 현장 구성 (단순 반송):
//     - MES/CEID 통신 없음, CELL(로케이션) 관리 없음, 바코드/공파레트 없음
//     - JOB(작업)은 외부/상위 시스템이 JOB_MST 에 직접 INSERT 하며,
//       START_POS / DEST_POS 가 이미 채워져 들어온다. (스케줄러는 작업을 생성하지 않는다)
//     - 따라서 본 스케줄러는 "JOB_MST 의 구동대기 작업 + 유휴 설비 → 설비 명령 발행
//       + 완료 감지 + 상태 전이" 만 담당한다.
//
//  ※ DB : PostgreSQL (방언은 DbLang 사용). 파라미터는 :PARAM, 날짜는 DbLang.SYSDATE(NOW()).
//  ※ 사용 테이블 : JOB_MST(작업), CV_DATA(컨베이어), SC_DATA_LGLS(스태커크레인), RTV_DATA_LGLS(RGV)
//     - 설비 상태는 _RD(PLC→DB readback) 컬럼, 명령은 _OD(DB→PLC) 컬럼 + OD_RQ_YN 플래그로 핸드셰이크.
//     - OD_RQ_YN='N' = 유휴(명령 수신 가능), 명령 발행 시 _OD 채우고 OD_RQ_YN='Y'.
//       (PLC↔DB TASK프로그램가 명령 반영 후 OD_RQ_YN 을 'N'으로 되돌린다)
//
//  ※ 기존 설비 스레드 cThread_CV / cThread_SC / cThread_R 의 SQL 패턴을 인용하여 작성.
//     (해당 스레드는 현장 구성에서 빌드 제외 - DB 구조 참고용으로만 보존)

using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Data;
using NpgsqlTypes;
using Samoh_Lib;

namespace TSK_COMM_IOSCH
{
    /// <summary>
    /// LG화학 물류창고 자동 반송 Scheduler Thread (JOB_MST 스키마)
    /// - [LGLS] 신규 접수 : WCS_TASK_HOST INSERT(JOB_STATUS='99') → 입고 '10' / 출고 '20' 라우팅
    /// - 상태 체계(2026-08-31 확정) :
    ///     입고 : 99 → 10 → 15 → 35 → 39 → 15 → 25 → 29 → 09
    ///     출고 : 99 → 20 → 25 → 29 → 15 → 35 → 39 → 15 → 19 → 09
    ///   자동은 29/19 를 WCS_TASK_HOST 가 F(완료) 보고 후 09 → 응답 시 삭제,
    ///   반자동(11/12)은 29/19 에서 IO_TASK 가 바로 삭제(상위 보고 없음).
    /// - 물리(이동/하역/공급)는 전부 설비(EQP_SIM/실장비) 담당 -
    ///   스케줄러는 지시(_OD)와 상태 전이만 한다.
    /// </summary>
    public class cThread_SCH : IOSchDB
    {
        #region 변수 선언
        private int      m_nId        = 0;
        public  Thread   m_Thread;
        public  SYS_MAIN m_Main;
        private bool     m_Open;
        public  bool     IsOpen       { get { return m_Open; } set { m_Open = value; } }
        public  string   m_strLogName = "";

        public PsMsgView callPsMsgView = null;

        // 설비별 직전 발행 작업 추적 (동일 작업 중복 발행 방지)
        private Dictionary<string, string> m_dicPrevCV  = new Dictionary<string, string>();
        // [LGLS 2026-08-31] 중복 발행 방지 키의 ★유효기간★.
        //   이 가드의 목적은 "설비 리드백이 반영되기 전 같은 폴링 창에서 두 번 발행" 을 막는 것뿐이다.
        //   그런데 값이 작업번호라, ★반자동처럼 번호를 재사용하면 그 트랙이 영구히 막힌다.★
        //   (실측 : 9001 이 124 에서 10 으로 무한 정체 - 앞 사이클의 CV_124=9001 잔재)
        //   중복 발행은 상태 전이(10 → 15)가 이미 막는다. 짧은 만료만 있으면 충분하다.
        private readonly Dictionary<string, DateTime> m_dicPrevAt = new Dictionary<string, DateTime>();
        private const int PREV_KEY_TTL_MS = 5000;
        private bool PrevIssued(Dictionary<string, string> dic, string key, string lugg)
        {
            string prev;
            if (!dic.TryGetValue(key, out prev) || prev != lugg) return false;
            DateTime at;
            if (!m_dicPrevAt.TryGetValue(key, out at)) { dic.Remove(key); return false; }
            if ((DateTime.Now - at).TotalMilliseconds >= PREV_KEY_TTL_MS)
            {
                dic.Remove(key); m_dicPrevAt.Remove(key);   // 만료 - 잔재로 본다
                return false;
            }
            return true;
        }
        private Dictionary<string, string> m_dicPrevSC  = new Dictionary<string, string>();
        private Dictionary<string, string> m_dicPrevRGV = new Dictionary<string, string>();
        #endregion

        #region 상수 정의
        // 창고 구분 : cDefApp.eWHTYP.SKI_WH01 = 10
        private static readonly string SCH_WH_TYP = ((int)cDefApp.eWHTYP.SKI_WH01).ToString();

        // [LGLS 2026-08-31] 크레인이 실제로 들고 있는 작업번호(포크 우선, 없으면 차상).
        //   화면의 크레인 색을 이 값으로도 칠한다 - 상태만 보면 색이 먼저 꺼진다.
        private const string SC_HELD_LUGG =
            "(CASE WHEN ISNULL(SD.ITN_LUGG_FK1,'0')         NOT IN ('','0','0000') THEN SD.ITN_LUGG_FK1 " +
            "      WHEN ISNULL(SD.PALLET_ON_VEHICLE_RD,'0') NOT IN ('','0','0000') THEN SD.PALLET_ON_VEHICLE_RD " +
            "      ELSE NULL END)";

        // JOB_MST.JOB_STATUS 라이프사이클 (설비별 3단계: 대기 → 중 → 완료)
        //   CV : 대기 10 → 중 15 → 완료 19
        //   SC : 대기 20 → 중 25 → 완료 29
        //   RGV: 대기 30 → 중 35 → 완료 39
        //   (구동완료 후 다음 처리로 핸드오프하려면 Complete* 에서 다음 대기상태로 전이 - ★정책확인)
        private const string ST_CV_WAIT = "10"; // CV 구동대기
        // [LGLS 2026-08-30] 구동지시(11/21/31) 폐기 - "대기 → 중 → 완료" 3단계.
        //   명령을 발행한 순간이 곧 구동 중이다. 수락 확인은 Complete* 의 OD_RQ_YN='N' 이 한다.
        private const string ST_CV_RUN  = "15"; // CV 구동중
        private const string ST_CV_DONE = "19"; // CV 구동완료

        private const string ST_SC_WAIT = "20"; // SC 구동대기
        private const string ST_SC_RUN  = "25"; // SC 구동중
        private const string ST_SC_DONE = "29"; // SC 구동완료

        private const string ST_RGV_WAIT = "30"; // RGV 구동대기
        private const string ST_RGV_RUN  = "35"; // RGV 구동중
        private const string ST_RGV_DONE = "39"; // RGV 구동완료

        // [LGLS] 신규 작업 : WCS_TASK_HOST(WMS I/F)가 JOB_MST 에 INSERT 하는 초기 상태
        private const string ST_JOB_NEW  = "99";

        // [LGLS] 다단계(2단계 처리) 반송 체인 및 최종 완료 상태
        //   입고(JOB_TYP='1') : CV(워크스테이션 1xx=START_POS) → SC(크레인 9xx=DEST_POS) → 최종 '29'
        //   출고(JOB_TYP='2') : SC(크레인 9xx=START_POS) → CV(워크스테이션 1xx=DEST_POS) → 최종 '19'
        //   ※ WCS_TASK_HOST.GetJobCompleteReport 는 JOB_STATUS 19/29 모두에서 F(작업완료) 보고를
        //     송신하므로, 중간 처리는 19/29 를 거치지 않고 곧바로 다음 처리 대기상태로 전이한다.
        //   처리별 설비위치 결정식 (CV_DATA.MC_NO / SC_DATA_LGLS.SC_NO 매칭용)
        private const string CV_POS_EXPR = "(CASE WHEN JM.JOB_TYP IN ('2','12') THEN JM.DEST_POS ELSE JM.START_POS END)";
        private const string SC_POS_EXPR = "(CASE WHEN JM.JOB_TYP IN ('1','11') THEN JM.DEST_POS ELSE JM.START_POS END)";

        // 명령 발행 주체 표기
        private const string OD_USER = "IOTASK";

        // [LGLS] SC 자동완주(TASK프로그램 부재 시뮬레이션) 설정 - ENV_IOSCH.INI [CNF] SC_AUTO_COMPLETE
        //   현 환경에는 SC/RGV PLC TASK프로그램이 없어 SC_DATA_LGLS.OD_RQ_YN='Y' 를 소비할 주체가 없다.
        //   1(기본) : SC 처리(20→21→25→완료)를 타이머 기반으로 자동 전이하여 시나리오를 완주시킨다.
        //   0       : 실제 SC TASK프로그램 사용 (DriveSC/RunSC/CompleteSC 핸드셰이크 경로)
        private const int SC_AUTO_ACCEPT_MS   = 3500;   // 지시(21) → 중(25) 지연 [LGLS] 라인CV 체류 확보(RTV→SC 인계 분리 가시화)
        private const int SC_AUTO_COMPLETE_MS = 5000;   // 지시 후 완료까지 지연
        private readonly Dictionary<string, DateTime> m_dicScIssueDt = new Dictionary<string, DateTime>();

        // [LGLS] RGV 자동완주(TASK프로그램 부재 시뮬레이션) : 입고 CV→SC 사이 RGV 처리를 타이머 전이시켜
        //   RTV_DATA_LGLS 관측치(위치/적재/색상)를 애니메이션한다. 30(대기)→31(지시)→35(중)→20(SC 인계)
        private const int IN_DWELL_MS   = 2500;   // [LGLS] 입고대 22 등장 체류(이 시간 후 22→21 이송) — 22 등장이 보이게
        private const int RGV_ACCEPT_MS = 4200;   // 지시(31) → 중(35) 지연 (22→21 이송·RTV 입고대 도착 후 픽업)
        private const int RGV_DONE_MS   = 6500;   // 지시 후 드롭 완료까지 지연
        private Dictionary<string, DateTime> m_dicRgvIssueDt = new Dictionary<string, DateTime>();
        private readonly HashSet<string> m_setInShifted = new HashSet<string>();   // [LGLS] 입고대 22→21 이송 1회 처리 추적
        private readonly Dictionary<string, DateTime> m_dicInFeedDt = new Dictionary<string, DateTime>();   // [LGLS] 입고대 파렛트 공급 시각(22 등장 dwell 계산)
        private readonly Dictionary<string, DateTime> m_dicCvClear = new Dictionary<string, DateTime>();  // [LGLS] 라인CV 트랙 지연 정리
        private class CvMovePend { public DateTime Due; public string Odd = ""; public string Lugg = ""; public string JobTyp = "2"; public bool NoClear = false; public string OutStn = "122"; }
        private readonly Dictionary<string, CvMovePend> m_dicCvMove = new Dictionary<string, CvMovePend>();
        // [LGLS 2026-08-23] OddStallSince : Stage 0 에서 "픽업 라인트랙에 내 화물이 없다"로 지시를 못 낸 시각.
        //   이 상태는 로그도 없이 영원히 continue 하던 자리라, 화물이 유실된 항목 하나가 m_dicOutStn 를 물고
        //   대기열 전체를 막았다(작업 1783 이 슬롯을 점유 → 1785 가 OUTPEND 에서 무한 대기).
        private class OutStnState { public DateTime Due; public int Stage = 0; public string Lugg = ""; public string Odd = ""; public string OutStn = "122"; public DateTime OddStallSince = DateTime.MinValue; public DateTime AckStallSince = DateTime.MinValue; }
        private readonly Dictionary<string, OutStnState> m_dicOutStn = new Dictionary<string, OutStnState>();  // [LGLS] RTV 출고대 반출 시퀀스(113→RTV→121→22)  // [LGLS] 출고 라인CV 짝수→홀수 지연 이동
        // [LGLS] RTV 출고대 반출 대기열(FIFO): RTV 는 1대뿐이라 출고대 반출 경로는 동시에 1건만 돈다.
        //   홀수(RGV 픽업)트랙에 도착한 출고 화물을 여기 쌓아두고, 출고대 반출 경로가 비면 선입선출로 하나씩 태운다.
        //   (구코드는 SC 완료 시점에 `if (m_dicOutStn.Count == 0)` 로만 출고대 반출 경로를 만들어서, 선행 화물이
        //    출고대 반출 경로를 점유 중이면 후속 화물이 출고대 반출 경로를 영영 못 받고 유실됐음.)
        private class OutPend { public string Lugg = ""; public string Odd = ""; public string OutStn = "122"; }
        private readonly List<OutPend> m_lstOutPend = new List<OutPend>();
        // [LGLS 2026-07-30] 출고 반출(RTV) 반송 완료 시각. CompleteCV 가 출고대 실도착(SENSOR+트래킹 일치)을
        //   폴링/미러 주기 사이에 놓친 채 설비가 배출·데이터클리어까지 끝내면 작업이 15에서 영구 정체(0008/0011/0013 사례)
        //   → 반출 완료 후 GRACE 경과에도 출고대에서 해당 작업번호가 관측되지 않으면 완료로 인정하는 보강에 사용.
        private readonly Dictionary<string, DateTime> m_dicOutDoneDt = new Dictionary<string, DateTime>();
        // [LGLS 2026-08-23] 종전 20초. 지게차가 도착 즉시 화물을 걷어가는 현장이라 완료가 2~3초 안에 나야 한다.
        //   위 LuggOnAnyTrack 이 '실화물 있는 트랙' 만 세도록 바뀌어, 이송 중인 작업을 조기 완료할 위험은 없다.
        private const int OUT_MISS_GRACE_MS = 2000;
        // [LGLS 2026-08-24] 출고대 신호/실도착을 둘 다 놓쳤을 때의 최후 유예.
        //   짧게 두면 RGV 하역 직후에 완료되어 버린다(작업 2034). 넉넉히 둔다.
        private const int OUT_SIGNAL_MISS_MS = 60000;
        // [LGLS 2026-08-23] 반출 대기 항목이 "픽업 라인트랙에 내 화물 없음"으로 지시를 못 내는 상태를 견디는 한계(ms).
        //   승격 직후에는 화물이 아직 홀수 트랙에 안 올라왔을 수 있어 얼마간은 정상 대기다. 그러나 화물이 유실됐거나
        //   트래킹만 남은 경우엔 영영 오지 않으므로, 이 시간이 지나면 슬롯을 놓아 뒤에 줄 선 작업을 통과시킨다.
        private const int OUT_ODD_STALL_MS = 60000;
        // [LGLS 2026-08-23] 반출 지시 후 RTV 완료신호(COMPLETE_RD='1' + LUGG_OD=내작업)를 기다리는 한계(ms).
        //   완료 판정은 LUGG_OD 가 아직 내 작업일 때만 성립하는데, 폴링 주기(약 3초) 사이에 다음 반출 지시가
        //   LUGG_OD 를 덮어쓰면 신호를 영영 못 본다. 그러면 이 항목이 Stage 1 에 갇혀 m_dicOutStn 를 영구 점유하고
        //   ProcessOutPend 가 막혀 **이후 출고가 전부 정지**한다(작업 1663 사례 - 반송은 끝났는데 15 에서 굳음).
        //   실측 반송이 11초쯤이라 15초는 너무 촉박했다(정상 반송 중에 인정될 수 있다) → 30초.
        private const int OUT_ACK_STALL_MS = 30000;
        // [LGLS 2026-08-24] RTV 완료신호(COMPLETE_RD)를 아무도 소비하지 않은 채 남는 경우 대비.
        //   반출 완료 대기 한계로 슬롯을 놓은 뒤에 뒤늦게 신호가 올라오면 소비자가 사라져 있다.
        //   그러면 RtvIdle() 이 영원히 false 라 RTV 가 통째로 멈춘다(작업 2057 로 실제 교착).
        private const int RTV_CMP_STALE_MS = 20000;
        private DateTime m_dtRtvCmpSince = DateTime.MinValue;
        private const int OUT_WAIT_LOG_MS  = 5000;    // 이 시간 넘게 기다릴 때만 대기 로그를 남긴다(정상 흐름 소음 방지)
        private readonly Dictionary<string, int> m_dicCraneTgt = new Dictionary<string, int>();       // [LGLS] 크레인 목표 POS_H
        private readonly Dictionary<string, int> m_dicCraneCur = new Dictionary<string, int>();       // [LGLS] 크레인 현재 POS_H
        private readonly Dictionary<string, DateTime> m_dicCraneStepDt = new Dictionary<string, DateTime>();
        private const int CRANE_STEP_MS = 600;   // [LGLS] 1칸 이동 간격(주행 가시화)
        #endregion

        #region 생성자
        public cThread_SCH(int Id)
        {
            m_nId = Id;
        }
        #endregion

        #region 메시지 출력 헬퍼
        private void MakeMsg(string msg, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
            try { callPsMsgView(msg, m_nId.ToString(), "", "", m_nId, cDefApp.eLogMsgType.MSG_NOR, pFile, pFunc); }
            catch { }
        }
        private void MakeMsg_Error(string msg, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
            try
            {
                callPsMsgView(msg, m_nId.ToString(), "", "", m_nId, cDefApp.eLogMsgType.MSG_ERR, pFile, pFunc);
                if (cDefApp.m_LogQ[m_nId] != null)
                    cDefApp.m_LogQ[m_nId].Enqueue(new LogParam(DateTime.Now, msg));
            }
            catch { }
        }
        private void MakeMsg_Imp(string msg, [CallerFilePath] string pFile = "", [CallerMemberName] string pFunc = "")
        {
            try
            {
                callPsMsgView(msg, m_nId.ToString(), "", "", m_nId, cDefApp.eLogMsgType.MSG_IMP, pFile, pFunc);
                if (cDefApp.m_LogQ[m_nId] != null)
                    cDefApp.m_LogQ[m_nId].Enqueue(new LogParam(DateTime.Now, msg));
            }
            catch { }
        }

        // [LGLS 2026-07-24] 계측 로그: WCS_LOG_PGR 에 직접 남겨 sqlcmd 로 관찰(반출 교착 진단용).
        //   변경 감지 캐시로 같은 메시지 반복 폭증 방지.
        private readonly Dictionary<string, string> m_dicDbgLast = new Dictionary<string, string>();
        private void DbgLog(string key, string msg)
        {
            try
            {
                string prev; m_dicDbgLast.TryGetValue(key, out prev);
                if (prev == msg) return;
                m_dicDbgLast[key] = msg;
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.ExcuteNonQry(" INSERT INTO WCS_LOG_PGR (WH_TYP, INS_DT, LOG_SEQ, LUGG_NO, PGR_NM, LOG_KOR) " +
                                   " VALUES ('" + SCH_WH_TYP + "', GETDATE(), '0', '', 'SCH_DBG', '" + (msg ?? "").Replace("'", "''") + "') ");
            }
            catch { }
        }
        #endregion

        #region DB 실행 래퍼 - 조용한 실패를 드러낸다
        // [LGLS 2026-08-30] ExcuteQry/ExcuteNonQry 는 실패해도 예외를 던지지 않는다.
        //   DB_ERR(-1) / DB_LOCK(-2) / DB_DUP(-3) 을 돌려주는데 cCbBasePost 안에서
        //   ShowErrMsg 가 주석 처리돼 있어 화면에도 로그에도 아무것도 남지 않는다.
        //   그래서 SQL 이 깨져도 "해당 없음" 과 구분이 안 된 채 조용히 지나간다.
        //
        //   ★0 은 실패가 아니다★
        //     SELECT 0건  = 조건에 맞는 작업이 없음 (정상)
        //     UPDATE 0건  = 바꿀 대상이 없음 (정상. SyncScForkPos 처럼 대부분의 주기가 여기다)
        //   그래서 0 을 실패로 보는 검사(nCnt <= 0 을 오류로 취급)는 넣으면 안 된다.
        //   실패는 오직 음수다. 음수일 때만 로그를 남긴다.
        //
        //   호출 흐름은 그대로 둔다 - 기존 검사(<= 0 이면 return)는 손대지 않고,
        //   "실패했는데 아무도 몰랐던" 경우만 보이게 만든다.

        private readonly Dictionary<string, DateTime> m_dicDbErrLog = new Dictionary<string, DateTime>();
        private const int DB_ERR_LOG_SEC = 30;   // 같은 자리 반복 실패는 30초에 한 번만

        /// <summary>SELECT 실행. 실패(음수)면 로그를 남긴다. 반환값 의미는 원본과 같다.</summary>
        private int DbQry(string strSql, [CallerMemberName] string pFunc = "")
        {
            int nRet = _pBdb.ExcuteQry(strSql);
            if (nRet < 0) DbErrLog("SELECT", nRet, pFunc);
            return nRet;
        }

        /// <summary>INSERT/UPDATE/DELETE 실행. 실패(음수)면 로그를 남긴다.
        ///   0(영향 행 없음)은 정상이므로 로그하지 않는다.</summary>
        private int DbNonQry(string strSql, [CallerMemberName] string pFunc = "")
        {
            int nRet = _pBdb.ExcuteNonQry(strSql);
            if (nRet < 0) DbErrLog("NONQRY", nRet, pFunc);
            return nRet;
        }

        private void DbErrLog(string strKind, int nRet, string pFunc)
        {
            try
            {
                string strKey = pFunc + "|" + strKind;
                DateTime dtLast;
                if (m_dicDbErrLog.TryGetValue(strKey, out dtLast) &&
                    (DateTime.Now - dtLast).TotalSeconds < DB_ERR_LOG_SEC) return;
                m_dicDbErrLog[strKey] = DateTime.Now;

                string strWhy = (nRet == -2) ? "DB LOCK" : (nRet == -3) ? "중복" : "DB 오류";
                MakeMsg_Error(string.Format("[SCH][DB] {0} {1} 실패({2}) : {3}",
                              strKind, pFunc, strWhy, _pBdb.ErrMsg), "", pFunc);
            }
            catch { }
        }
        #endregion

        // ─────────────────────────────────────────────────────────────────
        // 메인 Thread 루프 (200ms 폴링)
        // ─────────────────────────────────────────────────────────────────
        #region Thread_Doing
        public void Thread_Doing(object value)
        {
            // DB 연결이 성공할 때까지 재시도 (연결 전에 처리 메서드가 호출되지 않도록)
            while (!IsDBOpen)
            {
                try
                {
                    if (DBOpen())
                    {
                        MakeMsg("[SCH] DB Open 완료 - LG화학 Scheduler 시작 (JOB_MST 스키마)");

                        // [LGLS 2026-09-01] SC_AUTO_COMPLETE(자동완주) 스위치 폐기 (사용자 지시).
                        //   구 경로(FeedInGate/AutoRun*/ProcessRvSeq/StepCranes 등 물리 재현)는
                        //   EQP_SIM 도입 후 쓸 일이 없고, 현장/시뮬 공히 =0 으로만 구동해 왔다.

                        // [LGLS 2026-08-31] 출고 RGV 구간을 상태(30/35)로 표현할지
                        cDefApp.GM_OUT_VIA_RGV = (cDefApi.GsReadInitProfileCnf("OUT_VIA_RGV_STATE", 0) != 0);
                        if (cDefApp.GM_OUT_VIA_RGV)
                            MakeMsg_Imp("[SCH] 출고 RGV 상태화 모드 - 15 → 30 → 35 → 15 → 19 ([CNF] OUT_VIA_RGV_STATE=1)");
                        MakeMsg("[SCH] SC 실TASK프로그램 모드 - SC_DATA_LGLS OD_RQ_YN 핸드셰이크 사용");
                        break;
                    }
                    else
                    {
                        MakeMsg_Error("[SCH] DB Open 실패 - 5초 후 재시도");
                    }
                }
                catch (Exception ex)
                {
                    MakeMsg_Error("[SCH] DB Open 오류 - 5초 후 재시도: " + ex.Message);
                }
                Thread.Sleep(5000);
            }

            while (true)
            {
                Thread.Sleep(200);

                // _pBdb null 안전 확인
                if (_pBdb == null)
                {
                    try { IsDBOpen = false; DBOpen(); } catch { }
                    continue;
                }

                try
                {
                    // [LGLS] ── 신규 작업 접수 : WCS_TASK_HOST INSERT('99') → 첫 처리 대기상태로 라우팅
                    AcceptNewJob(); // 99 → 10(입고:CV먼저) / 20(출고:SC먼저)

                    // ── ① 구동 완료 : 설비 완료 신호(_RD) → (중 → 완료 또는 다음 처리 인계)
                    CompleteCV();       // 15 → 19(출고 최종) / 입고는 15 유지(RGV 가 가져간다)
                    CompleteSC();       // 25 → 29 (입고 최종 / 출고 1차 - 랙 셀 해제)
                    CompleteRGVReal();  // 35 → 39 (RTV COMPLETE_RD 소비)
                    CompleteRGVManual(); // [LGLS 2026-09-04] 수동지시(9998) 완료 정리 - 종료 이벤트/이력
                    CompleteSCManual();  // [LGLS 2026-09-05] SC 수동지시(9999) 완료 정리 - RTV 와 대칭

                    // ── ② 착지 처리 : 도착 신호 대신 ★화물 위치★ 로 판정해 다음 구간에 인계
                    LandRgvDrop();      // 39 + HS_TRACK_NO 에 화물 → 15 (CV/SC 인계)
                    LandScDrop();       // 출고 29 + HS_TRACK_NO 에 화물 → 15 (CV 인계)

                    // ── ③ 구동 지시 : 대기 작업 + 유휴 설비 → 명령 발행 (대기 → 중)
                    //   [LGLS 2026-09-01] Drive* 를 사이클당 1회로 정리 (사용자 지적).
                    //   종전에는 "인계 즉시 디스패치" 를 위해 완료 앞뒤로 2회 불렀는데,
                    //   완료·착지 ★뒤★ 에 한 번만 불러도 같은 효과다 - 방금 인계된 작업도
                    //   이 호출이 잡는다. (Drive* 는 대기 상태 + 유휴 설비에만 작용하는 멱등 함수)
                    SyncDualCvDirection();       // 겸용대 방향 정합(양방향)
                    PromotePendingDirection();   // 보류된 모드 전환(DIRW) 승격
                    DriveCV();      // 10 → 15
                    DriveSC();      // 입고 15 / 출고 20 → 25
                    DriveRGV();     // 15 → 35 (RGV 도착지를 HS_TRACK_NO 에 기록)

                    // ── ④ 마무리 : 반자동 삭제 / 감시
                    DeleteSemiFinished();      // 반자동(11/12)은 19/29 에서 바로 삭제(상위 보고 없음)
                    CheckStalledJobs();        // 작업 체류(설비 무응답) 감시 → 경고

                    // ── 알람 감시 : 설비 에러코드 로깅 (Set/Reset Report Ack 는 통신 Task 담당)
                    ReportOutStationArrival();  // [LGLS 2026-08-30] 출고대 신호 ON → 상위 도착보고(22)
                    MonitorAlarm();
                    MarkErrorJobStatus();       // [LGLS 2026-08-30] 이중입고(54)/공출고(58) → 작업상태 반영
                    ResumeRedirectedJobs();     // [LGLS 2026-08-30] 재지정(07/06) → 새 셀로 재개 지시

                    // [LGLS 2026-07-22] 표시용 작업구분 보강(실경로): 클라이언트는 CV_DATA/SC_DATA_LGLS 의
                    //   JOB_TYP_RD 로 입고/출고 색을 칠하는데, 실경로의 설비 관측(CvThread/VehThread)은
                    //   작업구분까지 알 수 없으므로 스케줄러가 작업 정보로 채운다. (표시 전용 — 제어 미사용)
                    SyncDisplayTyp();

                    // [LGLS] 한 사이클을 예외 없이 완주 → 스케줄러 정상 가동 하트비트
                    Heartbeat(true);
                }
                catch (Exception ex)
                {
                    // [LGLS] 사이클이 깨짐 → 오류 상태를 즉시 알림(Client 상태표시줄 SCH 가 RED)
                    Heartbeat(false);
                    MakeMsg_Error("[SCH] 처리 오류: " + ex.Message);
                    // DB 연결 끊김 가능성 → 재연결 시도
                    try
                    {
                        if (_pConObj == null ||
                            _pConObj.State != System.Data.ConnectionState.Open)
                        {
                            IsDBOpen = false;
                            DBOpen();
                        }
                    }
                    catch { }
                }
            }
        }
        #endregion

        // ─────────────────────────────────────────────────────────────────
        // [LGLS] 신규 작업 접수 (99 → 첫 처리 대기상태)
        //   WCS_TASK_HOST(WMS I/F)가 JOB_MST 에 JOB_STATUS='99' 로 INSERT 한 작업을
        //   JOB_TYP 에 따라 첫 처리로 라우팅한다.
        //     입고(JOB_TYP='1', START_POS=1xx 워크스테이션) → '10' (CV 구동대기)
        //     출고(JOB_TYP='2', START_POS=9xx 크레인)       → '20' (SC 구동대기)
        // ─────────────────────────────────────────────────────────────────
        #region AcceptNewJob
        private void AcceptNewJob()
        {
            try
            {
                string strSql = "";
                strSql += CRLF + " SELECT JM.LUGG_NO, JM.JOB_TYP, JM.START_POS, JM.DEST_POS       ";
                strSql += CRLF + "   FROM JOB_MST JM                                              ";
                strSql += CRLF + "  WHERE JM.WH_TYP     = :WH_TYP                                 ";
                strSql += CRLF + "    AND JM.JOB_STATUS = :ST_NEW                                 ";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_NEW", DbLang.VARCHAR).Value = ST_JOB_NEW;
                int nCnt = DbQry(strSql);
                if (nCnt <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();

                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string luggNo   = GetVal(dt.Rows[i], "LUGG_NO");
                    string jobTyp   = GetVal(dt.Rows[i], "JOB_TYP");
                    if (jobTyp == "11") jobTyp = "1"; else if (jobTyp == "12") jobTyp = "2";   // [LGLS 2026-07-20] 반자동(11/12) → 기본형 정규화(JOB_MST 원본은 유지)
                    string startPos = GetVal(dt.Rows[i], "START_POS");
                    string destPos  = GetVal(dt.Rows[i], "DEST_POS");

                    string stFirst;
                    string strProc;
                    string strJobData;
                    switch (jobTyp)
                    {
                        case "1": { stFirst = ST_CV_WAIT; strJobData = "CV(입고"; } break;
                        case "2": { stFirst = ST_SC_WAIT; strJobData = "SC(출고"; } break;
                        case "3": { stFirst = ST_SC_WAIT; strJobData = "SC(피킹출고"; } break;
                        case "4": { stFirst = ST_SC_WAIT; strJobData = "SC(랙투랙"; } break;
                        case "5": { stFirst = ST_SC_WAIT; strJobData = "SC(호기간이동"; } break;
                        case "6": { stFirst = ST_CV_WAIT; strJobData = "CV(이동"; } break;
                        default:
                        {
                            // [LGLS 2026-07-19] 미지원 유형(반자동 10~15 등)은 작업당 1회만 로깅 (매 폴링 반복 스팸 방지)
                            if (!m_setUnsupportedLogged.Contains(luggNo))
                            {
                                m_setUnsupportedLogged.Add(luggNo);
                                MakeMsg_Error(string.Format("[SCH][NEW] 작업 {0} 접수 불가 - 미지원 JOB_TYP:{1} (START:{2} DEST:{3})",
                                    luggNo, jobTyp, startPos, destPos));
                            }
                            continue;
                        }
                    }
                    strProc = strJobData + "1차 처리";
                    //if (jobTyp == "1")      { stFirst = ST_CV_WAIT; strProc = "CV(입고 1차 처리)"; }  // 워크스테이션 → 크레인
                    //else if (jobTyp == "2") { stFirst = ST_SC_WAIT; strProc = "SC(출고 1차 처리)"; }  // 크레인 → 워크스테이션
                    //else
                    //{
                    //    // [LGLS 2026-07-19] 미지원 유형(반자동 10~15 등)은 작업당 1회만 로깅 (매 폴링 반복 스팸 방지)
                    //    if (!m_setUnsupportedLogged.Contains(luggNo))
                    //    {
                    //        m_setUnsupportedLogged.Add(luggNo);
                    //        MakeMsg_Error(string.Format("[SCH][NEW] 작업 {0} 접수 불가 - 미지원 JOB_TYP:{1} (START:{2} DEST:{3})",
                    //            luggNo, jobTyp, startPos, destPos));
                    //    }
                    //    continue;
                    //}

                    string rtn = "";
                    if (UpdateJobStatus(stFirst, luggNo, ref rtn))
                        MakeMsg_Imp(string.Format("[SCH][NEW] 신규 작업 {0} 접수 (TYP:{1} START:{2} DEST:{3}) → {4} 대기 상태 '{5}'",
                            luggNo, jobTyp, startPos, destPos, strProc, stFirst));
                    else
                        MakeMsg_Error(string.Format("[SCH][NEW] 신규 작업 {0} 접수 실패: {1}", luggNo, rtn));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][NEW] AcceptNewJob 오류: " + ex.Message); }
        }
        #endregion


        #region DriveCV
        /// <summary>
        /// [LGLS 2026-08-30] 겸용 입출고대 방향을 입고로 되돌린다 (실경로).
        ///   DriveCV 에는 출고 반송 발행 시 출고(1) 전환만 있고 **입고 복귀 경로가 없다**.
        ///   한 번 출고로 돌아서면 계속 출고로 남아, 그 작업대발 입고가 영영 시작되지 못한다
        ///   (실측: C/V#11 이 출고 모드로 굳어 122 발 입고 0101~0103 이 상태 10 에서 50분 정지).
        ///   현장에서는 WMS 가 M 전문으로 되돌려 주지만, 상위 지시가 없어도 ECS 스스로 복귀해야 한다.
        ///   구 경로(FeedInGate)에는 같은 복귀가 이미 있었는데 실경로에만 빠져 있었다.
        ///
        ///   전환 자체는 RequestCvDirection 이 "작업번호 붙은 화물 없음"을 확인한 뒤에만 수행한다
        ///   — 작업번호 없는 파렛트는 그대로 두고 전환한다(사용자 확정).
        /// </summary>
        /// <summary>
        /// [LGLS 2026-08-30] 출고대 도착 보고 트리거 (사용자 요구).
        ///   "출고대 트랙에 그 작업의 화물이 있고 출고대 신호(RET_READY_RD)가 ON 되면 상위로 도착 보고한다."
        ///   ECS 의 책임은 화물을 출고대에 내어 놓는 데까지다 — 지게차가 실제로 가져가는 것은 그 다음이다.
        ///   JOB_STATUS 를 22(출고 H/S 도착보고)로 올리면 HOST_TASK 가 F 전문(StepCount=1)으로 보고하고,
        ///   응답을 받으면 작업을 삭제한다.
        /// </summary>
        private void ReportOutStationArrival()
        {
            try
            {
                string q = "";
                q += CRLF + " UPDATE JM                                                     ";
                q += CRLF + "    SET JM.JOB_STATUS  = '19'                                  ";
                // [LGLS 2026-08-30] ★22 폐기★ - 코드표에 없는 상태였다(사용자 지시).
                //   출고대 도착 = 출고 최종 완료로 본다. 19(CV 구동완료)로 올리면
                //   HOST_TASK.GetJobCompleteReport 가 F(완료) 보고 → 09(완료) → 응답 → 삭제로 이어간다.
                //   HS_TRACK_NO : HOST_TASK IsJobExist 가 읽는다. 비워두면 보고 구성에서
                //   값이 없어 보고가 나가지 못한다 - 도착한 출고대 트랙을 채운다.
                q += CRLF + "      , JM.HS_TRACK_NO = JM.DEST_POS                            ";
                q += CRLF + "      , JM.UPD_DT      = " + DbLang.SYSDATE + "                ";
                q += CRLF + "      , JM.UPD_USER_ID = '" + OD_USER + "'                     ";
                q += CRLF + "   FROM JOB_MST JM                                             ";
                q += CRLF + "  INNER JOIN CV_DATA CD                                        ";
                q += CRLF + "     ON CD.WH_TYP           = JM.WH_TYP                        ";
                q += CRLF + "    AND CD.MC_NO            = JM.DEST_POS                      ";
                q += CRLF + "  WHERE JM.WH_TYP           = :WH_TYP                          ";
                q += CRLF + "    AND JM.JOB_TYP         IN ('2','12')                       ";
                q += CRLF + "    AND JM.JOB_STATUS      IN ('15')                      ";   // CV 구동지시/구동중
                q += CRLF + "    AND CD.LUGG_NO_RD       = JM.LUGG_NO                       ";   // 그 화물이 출고대에 도착
                q += CRLF + "    AND CD.SENSOR0_DATA_RD  = '1'                              ";
                q += CRLF + "    AND CD.RET_READY_RD     = '1'                              ";   // ★출고대 신호 ON★
                q += CRLF + "    AND (JM.DEL_YN IS NULL OR JM.DEL_YN <> 'Y')                ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                int n = DbNonQry(q);
                if (n > 0)
                    MakeMsg_Imp(string.Format("[SCH][CV] 출고대 도착 - {0}건 출고 완료(19)로 전환 → 상위 완료보고", n));
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][CV] ReportOutStationArrival 오류: " + ex.Message); }
        }

        private void SyncDualCvDirection()
        {
            try
            {
                // ── ① 출고 방향 정합 : 그 작업대로 오는 출고가 진행 중인데 설비가 입고 모드면 출고로 맞춘다.
                //   [LGLS 2026-08-30] 방향이 어긋난 채 출고 화물이 도착하면 배출되지 못하고 그대로 갇힌다.
                //   DriveCV 의 출고 전환은 지시 시점(10→11)에만 일어나므로, 그 뒤(11/15)에 방향이
                //   틀어지면 되돌릴 주체가 없었다(실측: 작업 0113 이 입고 모드인 122 에 도착해 정지,
                //   트래킹이 121·122 에 이중 표시). 여기서 상시 정합을 맞춘다.
                string qo = "";
                qo += CRLF + " SELECT DISTINCT JM.DEST_POS AS STN               ";
                qo += CRLF + "   FROM JOB_MST JM                                ";
                qo += CRLF + "  WHERE JM.WH_TYP      = :WH_TYP                  ";
                qo += CRLF + "    AND JM.JOB_TYP    IN ('2','12')               ";
                qo += CRLF + "    AND JM.DEST_POS   IN ('122','103')            ";
                qo += CRLF + "    AND JM.JOB_STATUS NOT IN ('09','19','29','" + ST_SC_WAIT + "') ";
                qo += CRLF + "    AND (JM.DEL_YN IS NULL OR JM.DEL_YN <> 'Y')   ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(qo) > 0)
                {
                    DataTable dto = _pBdb.mDtMain.Copy();
                    for (int k = 0; k < dto.Rows.Count; k++)
                    {
                        string dp = GetVal(dto.Rows[k], "STN");
                        if (string.IsNullOrEmpty(dp)) continue;
                        if (GetCvStockMode(dp) == "1") continue;          // 이미 출고 모드
                        if (RequestCvDirection(dp, "1"))
                            MakeMsg_Imp(string.Format("[SCH][CV] 겸용대 {0} 방향 정합 지시 - 출고(1) (진행 중인 출고 있음)", dp));
                    }
                }

                // ── ①-B S/C#1 통로(C/V#2 = 103) 방향 정합  [LGLS 2026-08-31 사용자 요구]
                //   S/C#1 은 입고 드롭과 출고 반출이 같은 라인(C/V#2)을 쓴다. 그래서 이 겸용대의
                //   방향은 "작업대" 가 아니라 ★크레인 작업의 방향★ 을 따라가야 한다.
                //   위 ①은 작업의 START/DEST 가 103 인 경우만 보므로, 반자동처럼 124→901 / 901→126
                //   으로 도는 작업에서는 103 이 전혀 갱신되지 않았다(실측: 103 이 출고 모드로 굳은 채
                //   입고가 진행돼 크레인 통로가 어긋남).
                //     SC#1 발 출고(START_POS=901) 진행 중 → 출고(1)
                //     SC#1 행 입고(DEST_POS =901) 진행 중 → 입고(0)
                //   전환 자체의 안전(현재 방향 화물이 남아 있으면 보류)은 RequestCvDirection 이 맡는다.
                {
                    string qs = "";
                    qs += CRLF + " SELECT SUM(CASE WHEN JM.JOB_TYP IN ('2','12') THEN 1 ELSE 0 END) AS OUT_CNT, ";
                    qs += CRLF + "        SUM(CASE WHEN JM.JOB_TYP IN ('1','11') THEN 1 ELSE 0 END) AS IN_CNT  ";
                    // [LGLS 2026-08-31] 입고 15 도, RGV 가 통로에 화물을 내려놓은 뒤(HS=103/104)라면
                    //   진행 중이다. 15 를 안 치면 대기 출고(20)가 통로를 출고로 잡아
                    //   화물이 103→104 로 못 넘어가고 SC 지시가 영영 나가지 못한다(9029 실측).
                    // [LGLS 2026-09-01] ★15 는 실물까지 확인해야 진행이다★ (교착 실측)
                    //   HS_TRACK_NO 는 RGV ★지시 시★ 기록되는 예약이라, 화물이 아직 출발지에
                    //   있어도 15+HS 로 잡혔다. 그 오판이 통로를 입고로 고정해 출고 실물(9004)이
                    //   104 에 갇히고, RGV 는 라인 점유로 입고 드롭을 보류하는 상호 대기가 됐다.
                    //   통로에 실물이 있는 쪽이 이긴다 - 15 는 (HS 트랙에 그 화물 실재)일 때만 진행.
                    //   (SQL Server 는 집계 안에 하위쿼리를 못 넣는다 - 실물 여부는 LEFT JOIN 으로)
                    qs += CRLF + "      , SUM(CASE WHEN JM.JOB_TYP IN ('1','11') AND (JM.JOB_STATUS IN ('25','35','39') ";
                    qs += CRLF + "                 OR (JM.JOB_STATUS = '15' AND JM.HS_TRACK_NO IN ('103','104') AND C1.MC_NO IS NOT NULL) ";
                    qs += CRLF + "                ) THEN 1 ELSE 0 END) AS IN_RUN  ";
                    qs += CRLF + "      , SUM(CASE WHEN JM.JOB_TYP IN ('2','12') AND JM.JOB_STATUS IN ('25','35','39','15') THEN 1 ELSE 0 END) AS OUT_RUN ";
                    qs += CRLF + "   FROM JOB_MST JM                                ";
                    qs += CRLF + "   LEFT JOIN CV_DATA C1 ON C1.WH_TYP = JM.WH_TYP  ";
                    qs += CRLF + "        AND C1.MC_NO = JM.HS_TRACK_NO             ";
                    qs += CRLF + "        AND C1.SENSOR0_DATA_RD = '1' AND C1.LUGG_NO_RD = JM.LUGG_NO ";
                    qs += CRLF + "  WHERE JM.WH_TYP      = :WH_TYP                  ";
                    qs += CRLF + "    AND ( (JM.JOB_TYP IN ('2','12') AND JM.START_POS = '901')  ";
                    qs += CRLF + "       OR (JM.JOB_TYP IN ('1','11') AND JM.DEST_POS  = '901') ) ";
                    qs += CRLF + "    AND JM.JOB_STATUS NOT IN ('09','19','29') ";
                    qs += CRLF + "    AND (JM.DEL_YN IS NULL OR JM.DEL_YN <> 'Y')   ";
                    _pBdb.mComMain.CommandType = CommandType.Text;
                    _pBdb.mComMain.Parameters.Clear();
                    _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                    if (DbQry(qs) > 0)
                    {
                        int nOut, nIn;
                        int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "OUT_CNT"), out nOut);
                        int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "IN_CNT"),  out nIn);
                        // [LGLS 2026-08-31] ★지시가 이미 나간 쪽(25)이 이긴다★ (실측 정체)
                        //   종전 "출고 우선"은 아직 20(대기)인 출고가, 이미 25(크레인 구동중)인
                        //   입고의 통로를 빼앗았다 - 크레인은 103 에서 화물을 뜨려는데 통로가
                        //   출고 모드가 되어 입고 흐름이 죽고, 세 작업이 전부 갇혔다.
                        int nInRun, nOutRun;
                        int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "IN_RUN"),  out nInRun);
                        int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "OUT_RUN"), out nOutRun);
                        string want = (nInRun  > 0) ? "0"
                                    : (nOutRun > 0) ? "1"
                                    : (nOut > 0) ? "1" : (nIn > 0) ? "0" : "";
                        if (want != "" && GetCvStockMode("103") != want)
                        {
                            if (RequestCvDirection("103", want))
                                MakeMsg_Imp(string.Format("[SCH][CV] S/C#1 통로(C/V#2 103) 방향 지시 - {0} (SC#1 {1} 진행 중)",
                                            want == "1" ? "출고(1)" : "입고(0)", want == "1" ? "출고" : "입고"));
                        }
                    }
                }

                // ── ② 입고 방향 복귀 : 대기 중인 입고가 있고, 오는 출고가 없으면 입고로 되돌린다.
                string q = "";
                q += CRLF + " SELECT DISTINCT JM.START_POS                     ";
                q += CRLF + "   FROM JOB_MST JM                                ";
                q += CRLF + "  WHERE JM.WH_TYP      = :WH_TYP                  ";
                q += CRLF + "    AND JM.JOB_TYP    IN ('1','11')               ";
                q += CRLF + "    AND JM.JOB_STATUS  = :ST_WAIT                 ";
                q += CRLF + "    AND JM.START_POS  IN ('122','103')            ";   // 방향전환형 겸용 입출고대
                q += CRLF + "    AND (JM.DEL_YN IS NULL OR JM.DEL_YN <> 'Y')   ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_WAIT", DbLang.VARCHAR).Value = ST_CV_WAIT;
                if (DbQry(q) <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string sp = GetVal(dt.Rows[i], "START_POS");
                    if (string.IsNullOrEmpty(sp)) continue;
                    if (GetCvStockMode(sp) != "1") continue;              // 이미 입고 모드
                    // [LGLS 2026-08-30] ★그 작업대로 오는 출고가 진행 중이면 절대 되돌리지 않는다★
                    //   화물이 아직 작업대에 닿지 않았어도(이송 중) 방향을 뒤집으면, 도착한 출고 화물이
                    //   반대로 밀려 트래킹이 두 트랙에 걸쳐 남는다(실측: 작업 0113 이 트랙 121·122 동시 표시).
                    //   위의 IsOppositeDirCargo 는 "이미 올라온 화물"만 보므로 이 비행 중 구간이 비어 있었다.
                    // [LGLS 2026-08-31] ★화물이 아직 겸용대 위에 있으면 되돌리지 않는다★
                    //   출고 완료(19)는 "출고대에 내어 놓는 데까지"가 ECS 책임이라 작업을 지운다.
                    //   그런데 지게차가 가져가기 전에 방향을 입고로 되돌리면, 그 화물이
                    //   ★입고 경로로 끌려 들어간다★ (실측 : 22:33:28 작업 9012 출고 완료·삭제 →
                    //   22:33:29 겸용대 122 입고 복귀 → 화물 9012 가 122 에서 121 로 옮겨져 갇힘).
                    //   IsDualCvBusyWithJob 은 작업번호로 방향을 판별하므로 ★작업이 지워진 화물★ 은
                    //   막지 못한다. 물리적으로 화물이 있으면 작업 유무와 무관하게 보류한다.
                    if (!IsTrackEmpty(sp))
                    {
                        DbgLog("DIRRST_" + sp, "[겸용대] 입고 방향 복귀 보류 - 출고 화물이 아직 작업대에 있음(트랙 " + sp + ")");
                        continue;
                    }
                    if (HasActiveOutboundTo(sp))
                    {
                        DbgLog("DIRRST_" + sp, "[겸용대] 입고 방향 복귀 보류 - 그 작업대로 오는 출고 진행 중");
                        continue;
                    }
                    if (RequestCvDirection(sp, "0"))
                        MakeMsg_Imp(string.Format("[SCH][CV] 겸용대 {0} 방향 복귀 지시 - 입고(0) (대기 중인 입고 작업 있음)", sp));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][CV] SyncDualCvDirection 오류: " + ex.Message); }
        }

        private void DriveCV()
        {
            try
            {
                string strSql = "";
                strSql += CRLF + " SELECT CD.WH_TYP, CD.PLC_NO, CD.MC_NO, CD.TRACK_NO,            ";
                strSql += CRLF + "        JM.LUGG_NO, JM.JOB_TYP, JM.START_POS, JM.DEST_POS, JM.TURN, ";   // [LGLS 2026-07-21] START_POS: 출고 반출 대기열 등록용(출고 라인 홀수 트랙 계산)
                strSql += CRLF + "        JM.PRODUCT_SIZE, JM.TRAY_LEV                            ";
                strSql += CRLF + "   FROM CV_DATA CD, JOB_MST JM                                  ";
                strSql += CRLF + "  WHERE CD.MC_NO           = " + CV_POS_EXPR + "                ";   // [LGLS] 입고=START_POS, 출고=DEST_POS (설비 키=MC_NO, TRACK_NO 는 '1'+MC_NO 형식)
                strSql += CRLF + "    AND CD.WH_TYP          = JM.WH_TYP                          ";
                strSql += CRLF + "    AND (JM.JOB_TYP IN ('2','12') OR CD.SENSOR0_DATA_RD = '1')          ";   // [LGLS] 화물 감지는 입고만 — 출고는 S/C 하역 전이라 파렛트가 없음(트래킹 기록 후 재현됨)
                strSql += CRLF + "    AND CD.AUTO_MODE_RD    = '1'                                ";   // AUTO 모드
                strSql += CRLF + "    AND CD.OD_RQ_YN        = 'N'                                ";   // 설비 유휴(명령 수신 가능)
                strSql += CRLF + "    AND CD.READ_UPD_DT     > CD.WRITE_UPD_DT                    ";   // 최신 readback
                strSql += CRLF + "    AND (CD.ERROR_CODE = '0' OR CD.ERROR_CODE = '0000')         ";
                // [LGLS 2026-07-19] 입고대 일시정지(TR_PAUSE, 내부값) 시 입고 발행(PLC 기록) 금지
                strSql += CRLF + "    AND NOT (JM.JOB_TYP IN ('1','11') AND (ISNULL(CD.TR_PAUSE_OD,'0') = '1' OR ISNULL(CD.TR_PAUSE_RD,'0') = '1')) ";
                strSql += CRLF + "    AND CD.WH_TYP          = :WH_TYP                            ";
                strSql += CRLF + "    AND JM.JOB_STATUS      = :ST_WAIT                           ";   // CV 구동대기
                strSql += CRLF + "    AND JM.DEST_POS  Is not null                                ";
                // [LGLS 2026-07-21] 트랙별 배타: 같은 트랙의 운반 중(11/15) 작업이 남아 있으면 신규 지시 금지
                //   (겸용 C/V#11 에서 입고/출고 지시가 같은 트랙에 연속 발행되는 것 방지)
                strSql += CRLF + "    AND NOT EXISTS (SELECT 1 FROM JOB_MST J2                    ";
                strSql += CRLF + "                     WHERE J2.WH_TYP = JM.WH_TYP                ";
                strSql += CRLF + "                       AND J2.JOB_STATUS IN ('15')         ";
                strSql += CRLF + "                       AND CD.MC_NO = (CASE WHEN J2.JOB_TYP IN ('2','12') THEN J2.DEST_POS ELSE J2.START_POS END)) ";
                // [LGLS 2026-07-23] 겸용 입출고대(122) 교착 방지: 122 로 오는 출고 반출이 RTV 에 지시된 상태
                //   (RTV_DATA_LGLS.JOB_TYP_OD='2' + LUGG_OD=해당 출고작업)면 122 입고 발행(트래킹 기록)을 보류한다.
                strSql += CRLF + "    AND NOT ( JM.JOB_TYP IN ('1','11') AND " + CV_POS_EXPR + " = '122'   ";
                strSql += CRLF + "              AND EXISTS (SELECT 1                              ";
                strSql += CRLF + "                            FROM JOB_MST J6                     ";
                strSql += CRLF + "                           INNER JOIN RTV_DATA_LGLS R6               ";
                strSql += CRLF + "                              ON R6.WH_TYP     = J6.WH_TYP      ";
                strSql += CRLF + "                             AND R6.RTV_NO     = '801'          ";
                strSql += CRLF + "                             AND R6.LUGG_OD    = J6.LUGG_NO     ";
                strSql += CRLF + "                             AND R6.JOB_TYP_OD IN ('2','12')    ";
                strSql += CRLF + "                           WHERE J6.WH_TYP     = JM.WH_TYP      ";
                strSql += CRLF + "                             AND J6.JOB_TYP   IN ('2','12')     ";
                strSql += CRLF + "                             AND J6.DEST_POS   = '122'          ";
                strSql += CRLF + "                             AND J6.JOB_STATUS = '15' ) )       ";
                // [LGLS 2026-07-24] 122발 901행 입고는 SC1 출고 미종결 존재 시 발행 보류 — 입고 파렛트가
                //   하역트랙(121)을 선점하면 "출고 CV 발행(121 비어야) ↔ 입고 RTV(SC1 출고 우선 특례)" 의
                //   상호 대기 교착이 된다. 발행을 미루면 파렛트가 무지시 상태로 남아 작업자 회수가 가능해
                //   출고 반출 길이 열린다 ([CV#2 교착 TEST] 검출 교착 고리 차단).
                strSql += CRLF + "    AND NOT ( JM.JOB_TYP IN ('1','11') AND " + CV_POS_EXPR + " = '122'   ";
                strSql += CRLF + "              AND JM.DEST_POS = '901'                           ";
                strSql += CRLF + "              AND EXISTS (SELECT 1                              ";
                strSql += CRLF + "                            FROM JOB_MST J8                     ";
                strSql += CRLF + "                           WHERE J8.WH_TYP     = JM.WH_TYP      ";
                strSql += CRLF + "                             AND J8.JOB_TYP   IN ('2','12')     ";
                strSql += CRLF + "                             AND J8.START_POS  = '901'          ";
                strSql += CRLF + "                             AND J8.JOB_STATUS NOT IN ('09','19','29') ) ) ";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_WAIT", DbLang.VARCHAR).Value = ST_CV_WAIT;
                int nCnt = DbQry(strSql);
                if (nCnt <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();

                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string plcNo    = GetVal(dt.Rows[i], "PLC_NO");
                    string trackNo  = GetVal(dt.Rows[i], "MC_NO");   // [LGLS] 설비 키 = MC_NO (TRACK_NO 는 '1'+MC_NO 표시용)
                    string luggNo   = GetVal(dt.Rows[i], "LUGG_NO");
                    string jobTyp   = GetVal(dt.Rows[i], "JOB_TYP");
                    if (jobTyp == "11") jobTyp = "1"; else if (jobTyp == "12") jobTyp = "2";   // [LGLS 2026-07-20] 반자동(11/12) → 기본형 정규화(JOB_MST 원본은 유지)
                    string destPos  = GetVal(dt.Rows[i], "DEST_POS");
                    string isTurn   = GetVal(dt.Rows[i], "TURN");
                    string trayTyp  = GetVal(dt.Rows[i], "PRODUCT_SIZE");
                    string trayLev  = GetVal(dt.Rows[i], "TRAY_LEV");

                    // 동일 작업 중복 발행 방지
                    string key = "CV_" + trackNo;
                    if (PrevIssued(m_dicPrevCV, key, luggNo)) continue;   // [LGLS 2026-08-31] 5초 만료

                    // [LGLS 2026-07-21] 출고 CV 지시는 목적지(출고대 배출트랙)와 하역트랙이 빈 상태에서만 발행.
                    //   (지시 즉시 CvThread 가 도착 예약 트래킹을 배출트랙에 선기록하므로, 겸용 입고대(C/V#11)에
                    //    대기 중인 무지시 입고 파렛트가 예약 작업번호를 오배정받아 픽업트랙을 점유하고 교착한다
                    //    — 구 ECS 디스패처의 "입고대 점유 시 C/V#11 출고 배차 금지" 가드와 같은 취지)
                    if (jobTyp == "2" && (!IsTrackEmpty(trackNo) || !IsTrackEmpty(RgvPickupTrack(trackNo)))) continue;

                    // [LGLS 2026-08-01] 입출고 겸용대(STN_KIND=3, 현장 22번=C/V#11) 방향 게이트.
                    //   원본 ECS(1WH Backup ECSDispatcher.cs)의 두 가드를 그대로 이식한다.
                    //   ② 겸용대에서 출발하는 입고 반송은 방향이 '입고'일 때만 발행
                    //      (원본 578~584: fromPort==PORT:22 이고 CONVEYOR11_MODE != RGV_INGO 이면 return).
                    //      → 출고 화물이 그 작업대로 향하는 동안 작업자가 올려둔 화물을 가져가지 않는다.
                    //   ③ 겸용대로 도착하는 출고 반송은 '입고 모드 + 입고대 점유' 이면 보류
                    //      (원본 1203~1216 / 1239~1249: DIRECTION_MODE=='0' 이고 PALLET_EXIST/트래킹 존재 시 return)
                    //      + 그 작업대발 입고 반송이 진행 중이면 보류(원본 getCheckInTransferFromCNV11).
                    if (IsDualStation(trackNo))
                    {
                        if (jobTyp == "1" && GetCvStockMode(trackNo) != "0")
                        {
                            DbgLog("DUAL_" + trackNo, "[겸용대] 입고 보류 - 방향이 출고 모드(작업 " + luggNo + ")");
                            continue;
                        }
                        if (jobTyp == "2")
                        {
                            if (GetCvStockMode(trackNo) == "0" && (!IsTrackEmpty(trackNo) || !IsTrackLuggEmpty(trackNo)))
                            {
                                DbgLog("DUAL_" + trackNo, "[겸용대] 출고 보류 - 입고 모드에서 입고대 점유(작업 " + luggNo + ")");
                                continue;
                            }
                            if (HasActiveInboundFrom(trackNo))
                            {
                                DbgLog("DUAL_" + trackNo, "[겸용대] 출고 보류 - 해당 작업대발 입고 반송 진행 중(작업 " + luggNo + ")");
                                continue;
                            }

                            // [LGLS 2026-08-01] 출고 반송 발행 시 자동 방향 전환(원본 ECS ECSDispatcher 1303~1309 대응).
                            //   원본은 C\V#2·#11 로 가는 반송 지시 직전에 DIRECTION_MODE 를 '1'(출고)로 바꾸고
                            //   그 다음에 TRANSFER_REQUEST 를 세웠다. 현행도 같은 순서로, 방향 지시를 먼저 남기고
                            //   CV 지시를 이어서 발행한다(설비 반영은 수 초 내, 화물 도착은 그보다 늦다).
                            if (GetCvStockMode(trackNo) != "1" && RequestCvDirection(trackNo, "1"))
                                MakeMsg_Imp(string.Format("[SCH][CV] 겸용대 {0} 방향 전환 지시 - 출고(1) (작업 {1})", trackNo, luggNo));
                        }

                    }

                    MakeMsg_Imp(string.Format("[SCH][CV] CV TRACK:{0} 작업:{1}(TYP:{2}) → CV 명령 발행 (DEST:{3})",
                        trackNo, luggNo, jobTyp, destPos));

                    _pBdb.BeginTrans();

                    string rtn = "";
                    bool ok = UpdateCvData(jobTyp, destPos, luggNo, plcNo, trackNo, ref rtn)
                              && UpdateJobStatus(ST_CV_RUN, luggNo, ref rtn);

                    if (ok)
                    {
                        _pBdb.Commit();
                        m_dicPrevCV[key] = luggNo;  m_dicPrevAt[key] = DateTime.Now;
                        MakeMsg_Imp(string.Format("[SCH][CV] CV TRACK:{0} 명령 발행 완료, 작업 {1} 상태 '{2}'", trackNo, luggNo, ST_CV_RUN));
                        // [LGLS 2026-07-21] 실경로: 출고 CV 발행과 동시에 출고대 반출 대기열(FIFO) 등록.
                        //   (구 경로에선 ProcessCvMove 가 홀수 트랙 도착 시 등록했으나, 실경로는 라인 이동을
                        //    설비가 하므로 등록 지점이 없어 RTV 반출 지시가 영영 나가지 않았다)
                        // [LGLS 2026-08-31] 새 경로(OUT_VIA_RGV_STATE=1)에서는 메모리 큐를 쓰지 않는다.
                        //   화물이 홀수 트랙에 도착하면 PromoteOutToRgvWait 가 30 으로 올린다.
                        if (jobTyp == "2" && !cDefApp.GM_OUT_VIA_RGV && !OutSeqPending(luggNo))
                        {
                            // [LGLS 2026-07-21] RTV 픽업 위치는 홀수 트랙(하역트랙-1) — SC 가 짝수 하역트랙에 내려놓은
                            //   화물을 설비(CV)가 홀수 트랙으로 옮겨 놓은 뒤 RTV 가 집는다. 짝수 트랙을 지정하면
                            //   이동 타이밍에 따라 RTV 가 빈 트랙에서 영구 재시도(레이스)한다.
                            string startPos = GetVal(dt.Rows[i], "START_POS");
                            string outOdd = RgvPickupTrack(RgvOutDropTrack(startPos));
                            m_lstOutPend.Add(new OutPend { Lugg = luggNo, Odd = outOdd, OutStn = trackNo });
                            DbgLog("OUTREG_" + luggNo, "[OUT등록] " + luggNo + " 라인" + outOdd + "→출고대" + trackNo);
                            MakeMsg_Imp(string.Format("[SCH][OUT] 출고대 반출 대기열 등록 - 작업 {0} (라인 {1} → 출고대 {2})",
                                luggNo, outOdd, trackNo));
                        }
                    }
                    else
                    {
                        _pBdb.Rollback();
                        MakeMsg_Error(string.Format("[SCH][CV] CV TRACK:{0} 발행 실패: {1}", trackNo, rtn));
                    }
                }
            }
            catch (System.Data.Common.DbException nex)
            {
                MakeMsg_Error("[SCH][CV] DriveCV DB 오류: " + nex.Message);
                try { _pBdb.Rollback(); } catch { }
            }
            catch (Exception ex)
            {
                MakeMsg_Error("[SCH][CV] DriveCV 오류: " + ex.Message);
                try { _pBdb.Rollback(); } catch { }
            }
        }
        #endregion

        // ─────────────────────────────────────────────────────────────────
        // S/C 반송
        //   기존 cThread_SC.CHECK_RACK_TO_RACK 의 "유휴 S/C 판정 블록"(IDLE block) 인용.
        //   조건 : 구동대기 작업(JOB_STATUS='20') + 해당 S/C(SC_NO=START_POS) 유휴
        //          → S/C 이송 명령(_OD) 발행(OD_RQ_YN='Y') + JOB_STATUS '20'→'21'
        //
        //   ※ CELL 관리가 없는 현장이므로 BANK/BAY/LEVEL 분해는 하지 않고
        //     START_POS/DEST_POS(설비번호) + LUGG_NO 기반의 단순 이송으로 처리한다.
        //     아래 _OD 명령 컬럼/값(USER_COMMAND_OD, WRITE_FLAG_OD, 위치 필드 등)은
        //     실제 SC_DATA_LGLS DDL 및 PLC 명령 사양에 맞게 보완해야 한다. ★TODO(현장값 확인)
        // ─────────────────────────────────────────────────────────────────
        #region DriveSC
        private void DriveSC()
        {
            try
            {
                string strSql = "";
                strSql += CRLF + " SELECT JM.LUGG_NO, JM.JOB_TYP, JM.START_POS, JM.DEST_POS,      ";
                strSql += CRLF + "        JM.START_LOCATION, JM.DEST_LOCATION,                     ";   // [LGLS 2026-07-21] Vehicle 셀 좌표용
                strSql += CRLF + "        SD.SC_NO, SD.SC_TYP                                      ";
                strSql += CRLF + "   FROM JOB_MST JM                                               ";
                strSql += CRLF + "  INNER JOIN SC_DATA_LGLS SD                                          ";
                strSql += CRLF + "     ON JM.WH_TYP    = SD.WH_TYP                                 ";
                strSql += CRLF + "    AND SD.SC_NO     = " + SC_POS_EXPR + "                       ";   // [LGLS] 입고=DEST_POS, 출고=START_POS
                strSql += CRLF + "  WHERE JM.WH_TYP          = :WH_TYP                             ";
                // [LGLS 2026-08-31] 입고는 CV 구동중(15)에서 SC 가 가져간다(SC 대기 상태를 두지 않는다).
                //   출고는 첫 진입이므로 SC 구동대기(20) 그대로.
                //     입고 : 99 → 10 → 15 → 35 → 39 → 15 → 25 → 29 → 09
                //     출고 : 99 → 20 → 25 → 29 → 15 → 35 → 39 → 15 → 19 → 09
                strSql += CRLF + "    AND ( (JM.JOB_TYP IN ('1','11') AND JM.JOB_STATUS = '15')       ";
                strSql += CRLF + "       OR (JM.JOB_TYP NOT IN ('1','11') AND JM.JOB_STATUS = :ST_WAIT) ) ";
                strSql += CRLF + "    AND SD.ONLINE_MODE_RD  = '1'                                 ";
                strSql += CRLF + "    AND SD.AUTO_MODE_RD    = '1'                                 ";
                strSql += CRLF + "    AND (SD.ERR_CODE_RD IS NULL OR SD.ERR_CODE_RD IN ('0','00','0000','')) ";   // [LGLS 2026-08-30] 정상 표기 흔들림(0/00/0000/빈 값) 흡수
                strSql += CRLF + "    AND SD.ACTIVE_MODE_RD  = '1'                                 ";
                strSql += CRLF + "    AND SD.UCSTATUS_RD     = '1'                                 ";
                // [LGLS 2026-08-30] 포크 비어있음 판정 — NULL/빈 값/'0000' 도 "비어있음"으로 본다.
                //   종전에는 = '0' 하나만 봐서, 이 컬럼이 NULL 이 되면 (수동 정리·초기 적재 등)
                //   5대 크레인 전 지시가 조용히 영구 정지했다. VehThread 는 값이 바뀔 때만 쓰므로
                //   한번 NULL 이 되면 스스로 복구되지도 않는다.
                strSql += CRLF + "    AND (SD.ITN_LUGG_FK1 IS NULL OR SD.ITN_LUGG_FK1 IN ('0','00','0000','')) ";   // 포크1 비어있음
                strSql += CRLF + "    AND (SD.ITN_LUGG_FK2 IS NULL OR SD.ITN_LUGG_FK2 IN ('0','00','0000','')) ";   // 포크2 비어있음
                strSql += CRLF + "    AND SD.OD_RQ_YN        = 'N'                                 ";   // 설비 유휴
                // [LGLS 2026-07-24] (제거) COMPLETE_RD<>'1' 게이트 삭제 — 마지막 작업의 완료신호가 '1'로 잔류하면
                //   이 게이트가 SC 에 신규 지시를 영구 차단하는 데드락(완료신호 리셋이 다음 지시 소비에 의존하는데
                //   그 지시를 막음)이 된다. 완료신호 오소비 방지는 아래 21/25 배타 게이트가 이미 커버.
                // [LGLS 2026-07-21] 크레인별 배타: 같은 크레인의 운반 중(21/25) 작업이 남아 있으면 신규 지시 금지
                //   (리드백 지연 창 동안의 연속 지시 → LUGG_OD 덮어쓰기 → 완료 귀속 유실 방지)
                strSql += CRLF + "    AND NOT EXISTS (SELECT 1 FROM JOB_MST J2                     ";
                strSql += CRLF + "                     WHERE J2.WH_TYP = JM.WH_TYP                 ";
                strSql += CRLF + "                       AND J2.JOB_STATUS IN ('25')          ";
                strSql += CRLF + "                       AND SD.SC_NO = (CASE WHEN J2.JOB_TYP IN ('1','11') THEN J2.DEST_POS ELSE J2.START_POS END)) ";
                // [LGLS 2026-07-23] SC1/C\V#2 교착 방지: SC1 출고 지시는 ① C\V#2(103/104)에 입고 작업 화물이 없고
                //   ② RTV 에 901행 입고 지시(RTV_DATA_LGLS.JOB_TYP_OD='1')가 없을 때만 발행한다.
                strSql += CRLF + "    AND NOT ( SD.SC_NO = '901' AND JM.JOB_TYP IN ('2','12') AND (        ";
                strSql += CRLF + "          EXISTS (SELECT 1 FROM CV_DATA C2                               ";
                strSql += CRLF + "                   INNER JOIN JOB_MST J3 ON J3.WH_TYP  = C2.WH_TYP       ";
                strSql += CRLF + "                                       AND J3.LUGG_NO = C2.LUGG_NO_RD    ";
                strSql += CRLF + "                   WHERE C2.WH_TYP = SD.WH_TYP                           ";
                strSql += CRLF + "                     AND C2.MC_NO IN ('103','104')                       ";
                strSql += CRLF + "                     AND J3.JOB_TYP IN ('1','11')                        ";
                strSql += CRLF + "                     AND J3.JOB_STATUS NOT IN ('09','19','29'))           ";
                strSql += CRLF + "       OR EXISTS (SELECT 1 FROM RTV_DATA_LGLS R3                              ";
                strSql += CRLF + "                   INNER JOIN JOB_MST J4 ON J4.WH_TYP  = R3.WH_TYP       ";
                strSql += CRLF + "                                       AND J4.LUGG_NO = R3.LUGG_OD       ";
                strSql += CRLF + "                   WHERE R3.WH_TYP = SD.WH_TYP                           ";
                strSql += CRLF + "                     AND R3.RTV_NO = '801'                               ";
                strSql += CRLF + "                     AND R3.JOB_TYP_OD IN ('1','11')                     ";
                strSql += CRLF + "                     AND J4.JOB_TYP IN ('1','11')                        ";
                strSql += CRLF + "                     AND J4.DEST_POS = '901'                             ";
                strSql += CRLF + "                     AND J4.JOB_STATUS NOT IN ('09','19','29')) ) )       ";
                // [LGLS 2026-07-24] 출고 직렬화: 같은 크레인의 선행 출고가 CV/반출 구간(10/11/15)에 남아 있으면
                //   다음 출고 SC 지시를 보류한다 — 출고 화물이 라인(하역 2트랙)에 겹겹이 쌓여 겸용 라인이
                //   막히는 것을 방지 ([CV#2 교착 TEST]에서 출고 3건이 C\V#2 를 가득 채워 교착하던 고리 차단).
                strSql += CRLF + "    AND NOT ( JM.JOB_TYP IN ('2','12') AND EXISTS (SELECT 1              ";
                strSql += CRLF + "                   FROM JOB_MST J7                                       ";
                strSql += CRLF + "                  WHERE J7.WH_TYP = JM.WH_TYP                            ";
                strSql += CRLF + "                    AND J7.JOB_TYP IN ('2','12')                         ";
                strSql += CRLF + "                    AND J7.START_POS = SD.SC_NO                          ";
                strSql += CRLF + "                    AND J7.JOB_STATUS IN ('10','15')) )             ";
                // [LGLS 2026-07-19] suspend 방향별 게이트: 1=입고정지, 2=출고정지, 3=입출고정지
                strSql += CRLF + "    AND NOT (JM.JOB_TYP IN ('1','11') AND SD.SUSPEND IN ('1','3'))      ";
                strSql += CRLF + "    AND NOT (JM.JOB_TYP IN ('2','12') AND SD.SUSPEND IN ('2','3'))      ";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_WAIT", DbLang.VARCHAR).Value = ST_SC_WAIT;
                int nCnt = DbQry(strSql);
                if (nCnt <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();

                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string scNo    = GetVal(dt.Rows[i], "SC_NO");
                    string luggNo  = GetVal(dt.Rows[i], "LUGG_NO");
                    string jobTyp  = GetVal(dt.Rows[i], "JOB_TYP");
                    if (jobTyp == "11") jobTyp = "1"; else if (jobTyp == "12") jobTyp = "2";   // [LGLS 2026-07-20] 반자동(11/12) → 기본형 정규화(JOB_MST 원본은 유지)
                    string startPos= GetVal(dt.Rows[i], "START_POS");
                    string destPos = GetVal(dt.Rows[i], "DEST_POS");
                    string startLoc= GetVal(dt.Rows[i], "START_LOCATION");
                    string destLoc = GetVal(dt.Rows[i], "DEST_LOCATION");

                    string key = "SC_" + scNo;
                    if (m_dicPrevSC.ContainsKey(key) && m_dicPrevSC[key] == luggNo) continue;

                    // [LGLS 2026-07-21] 실경로 게이트 (AutoRunSC 에서 이식):
                    //   ① 작업대(라인 트랙) 일시정지, ② 출고 H/S 점유 시 지시 금지, ③ SC1 특례,
                    //   ④ 입고는 화물이 라인 픽업트랙(짝수)에 도착한 뒤에만 지시
                    {
                        string _wT;
                        if (jobTyp == "1") { string _d = RgvDropTrack(destPos); int _n; _wT = int.TryParse(_d, out _n) ? (_n + 1).ToString() : _d; }
                        else _wT = RgvOutDropTrack(startPos);
                        if (IsCvPaused(_wT)) continue;
                        // [LGLS 2026-08-30] 그 작업이 쓸 라인 트랙(CV)이 에러면 S/C 지시 금지 —
                        //   에러난 CV 는 움직이지 못하므로 크레인이 화물을 들고 갇힌다.
                        if (IsCvError(_wT)) continue;
                        // [LGLS 2026-09-01] ★출고 하차 지시 전, 같은 라인의 입고 진행 화물 확인★
                        //   (실측 : 크레인이 P4 로 출고 하차 지시를 받은 직후 103 의 입고 화물
                        //    9035 가 P4 로 이동해 와, 크레인이 점유된 P4 에 9036 을 내려
                        //    9035 가 소멸 - 실장비면 충돌 사고다.)
                        //   하차 트랙의 홀수 짝(RGV측)에 화물이 있으면 그 화물이 곧 이 트랙으로
                        //   들어오므로 출고 지시를 보류한다.
                        if (jobTyp == "2")
                        {
                            int _wTn; string _oddT = int.TryParse(_wT, out _wTn) ? (_wTn - 1).ToString() : "";
                            if (!string.IsNullOrEmpty(_oddT) && !IsTrackEmpty(_oddT))
                            {
                                DbgLog("SCOUT_" + scNo, string.Format("[SC] 출고 보류 - 라인 {0} 에 입고 진행 화물(하차 트랙 {1} 로 진입 예정)", _oddT, _wT));
                                continue;
                            }
                        }
                        if (jobTyp == "2" && !IsTrackEmpty(_wT))
                        {
                            // [LGLS 2026-08-31] 그 트랙에 있는 것이 ★이 작업의 화물★ 이면
                            //   크레인은 이미 내려놓은 것이고 상태만 뒤처진 것이다.
                            //   (Client 를 사이클 중간에 재기동했을 때 실제로 나왔다 - 화물은 106 에
                            //    있는데 작업은 20 에 머물러, 트랙이 비지 않으니 영영 지시되지 않았다.)
                            //   실물 기준으로 CV 구간(15)에 넘겨 마저 내보낸다.
                            if ((TrackLugg(_wT) ?? "").Trim() == luggNo)
                            {
                                string _rtnSkip = "";
                                if (UpdateJobStatusHs(ST_CV_RUN, luggNo, _wT, ref _rtnSkip))
                                    MakeMsg_Imp(string.Format(
                                        "[SCH][SC] 작업 {0} 화물이 이미 하역 트랙 {1} 에 있음 - 크레인 단계 생략, 상태 '{2}'",
                                        luggNo, _wT, ST_CV_RUN));
                            }
                            continue;
                        }
                        if (jobTyp == "2" && startPos == "901" && HasSc1InboundOnRtv()) continue;
                        if (jobTyp == "1" && IsTrackEmpty(_wT)) continue;
                        // [LGLS 2026-08-31] ★픽업 트랙의 화물이 이 작업의 화물인지 확인한다★
                        //   (RGV 오집 수정과 같은 패턴 - 크레인 판)
                        //   실측 : 방향 전환이 늦어 앞 출고 화물(9004)이 작업대에 남았는데,
                        //   화물 유무만 보고 입고 지시를 내 크레인이 남의 화물을 집어
                        //   랙에 저장했다(시뮬 로그 "JOB 재부여: 9004 → 9003").
                        if (jobTyp == "1" && (TrackLugg(_wT) ?? "").Trim() != luggNo)
                        {
                            DbgLog("SCOWN_" + scNo, string.Format("[SC] 입고 보류 - 트랙 {0} 화물({1})이 작업 {2} 의 화물이 아님",
                                        _wT, (TrackLugg(_wT) ?? "").Trim(), luggNo));
                            continue;
                        }

                        // [LGLS 2026-08-22] S/C #1 은 입고·출고를 C/V#2(트랙 103/104) 하나로 겸용한다
                        //   (PlcAddressMap CraneMap : Crane no=1 inCv=outCv=2 - 방향전환형).
                        //   작업 방향과 설비 방향이 어긋나면 그 쪽 HS 가 서지 않아 지시가 영영 보류된다
                        //   (출고 작업이 상태 '20' 에서 멈춘 사례). 종전에는 입출고대(C/V#11)만 전환하고
                        //   이 통로 C/V 를 전환하는 경로가 없었다 - C/V#11 과 같은 방식으로 지시 직전에 맞춘다.
                        //   RGV 접점(103)에 화물이 남아 있는 동안에는 바꾸지 않는다(이송 방향이 뒤집혀 갇힌다).
                        if (scNo == SC1_NO)
                        {
                            string wantDir = (jobTyp == "2") ? "1" : "0";
                            if (GetCvStockMode(SC1_DUAL_CV) != wantDir)
                            {
                                if (!IsDualCvBusyWithJob(SC1_DUAL_CV, wantDir))   // [LGLS 2026-08-30] 작업번호 없는 화물은 전환을 막지 않는다
                                {
                                    if (RequestCvDirection(SC1_DUAL_CV, wantDir))
                                        MakeMsg_Imp(string.Format("[SCH][SC] 겸용 통로 C/V#2({0}) 방향 전환 지시 - {1} (작업 {2})",
                                            SC1_DUAL_CV, wantDir == "1" ? "출고(1)" : "입고(0)", luggNo));
                                }
                                else
                                    DbgLog("DUALCV2", string.Format("[겸용통로] 방향 전환 보류 - 트랙 {0} 점유(작업 {1})", SC1_DUAL_CV, luggNo));
                                continue;   // 방향이 설비에 반영된 뒤 다음 폴링에 지시
                            }
                        }

                        // [LGLS 2026-08-22] 핸드셰이크 최우선 규칙
                        //   입고는 그 트랙의 입고 HS 가, 출고는 출고 HS 가 서 있어야 지시한다.
                        if (jobTyp == "1" && !IsHsOn(_wT, "STOHS_READY_RD"))
                        {
                            DbgLog("SCHS_" + scNo, string.Format("[SC] 입고 보류 - 트랙 {0} 입고 HS 신호 없음(작업 {1})", _wT, luggNo));
                            continue;
                        }
                        if (jobTyp == "2" && !IsHsOn(_wT, "RETHS_READY_RD"))
                        {
                            DbgLog("SCHS_" + scNo, string.Format("[SC] 출고 보류 - 트랙 {0} 출고 HS 신호 없음(작업 {1})", _wT, luggNo));
                            continue;
                        }
                    }

                    // [LGLS 2026-07-21] Vehicle 반송 좌표 인코딩:
                    //   입고(1): From=라인 픽업트랙 포트, To=셀(DEST_LOCATION) / 출고(2): From=셀(START_LOCATION), To=출고 하역트랙 포트
                    string f1, f2, f3, t1, t2, t3;
                    if (jobTyp == "1")
                    {
                        string _d = RgvDropTrack(destPos); int _n; string pickT = int.TryParse(_d, out _n) ? (_n + 1).ToString() : _d;
                        VehPortLoc(pickT, out f1, out f2, out f3);
                        VehCellLoc(destLoc, out t1, out t2, out t3);
                    }
                    else
                    {
                        VehCellLoc(startLoc, out f1, out f2, out f3);
                        VehPortLoc(RgvOutDropTrack(startPos), out t1, out t2, out t3);
                    }

                    MakeMsg_Imp(string.Format("[SCH][SC] S/C #{0} 작업:{1}(TYP:{2}) → 반송지시 (From:{3}/{4}/{5} To:{6}/{7}/{8})",
                        scNo, luggNo, jobTyp, f1, f2, f3, t1, t2, t3));

                    _pBdb.BeginTrans();

                    string rtn = "";
                    bool ok = UpdateScData(scNo, jobTyp, luggNo, f1, f2, f3, t1, t2, t3, ref rtn)
                              && (jobTyp == "2"
                                  // [LGLS 2026-08-31] 출고는 25 로 올리면서 SC 하역 도착지를 HS_TRACK_NO 에 적는다.
                                  //   LandScDrop() 이 그 트랙의 도착 신호가 꺼진 것을 보고 15 로 내린다.
                                  ? UpdateJobStatusHs(ST_SC_RUN, luggNo, RgvOutDropTrack(startPos), ref rtn)
                                  : UpdateJobStatus  (ST_SC_RUN, luggNo, ref rtn));

                    if (ok)
                    {
                        _pBdb.Commit();
                        m_dicPrevSC[key] = luggNo;
                        MakeMsg_Imp(string.Format("[SCH][SC] S/C #{0} 명령 발행 완료, 작업 {1} 상태 '{2}'", scNo, luggNo, ST_SC_RUN));
                    }
                    else
                    {
                        _pBdb.Rollback();
                        MakeMsg_Error(string.Format("[SCH][SC] S/C #{0} 발행 실패: {1}", scNo, rtn));
                    }
                }
            }
            catch (System.Data.Common.DbException nex)
            {
                MakeMsg_Error("[SCH][SC] DriveSC DB 오류: " + nex.Message);
                try { _pBdb.Rollback(); } catch { }
            }
            catch (Exception ex)
            {
                MakeMsg_Error("[SCH][SC] DriveSC 오류: " + ex.Message);
                try { _pBdb.Rollback(); } catch { }
            }
        }
        #endregion

        // ─────────────────────────────────────────────────────────────────
        // RGV 반송
        //   조건 : 구동대기 작업(JOB_STATUS=ST_RGV_WAIT) + 유휴 RGV → 이송 명령 발행.
        //   ※ RTV_DATA_LGLS 의 명령(_OD)/상태(_RD) 컬럼은 기존 코드에 정의가 거의 없어
        //     SC_DATA_LGLS 핸드셰이크(OD_RQ_YN + _OD)를 모델로 작성. RGV가 1대인 현장.
        //     실제 RTV_DATA_LGLS DDL 및 PLC 사양에 맞게 컬럼/값 보완 필요. ★TODO(현장값 확인)
        //   RGV 미설치/미사용 현장이면 본 메서드는 0건으로 자연 통과한다.
        // ─────────────────────────────────────────────────────────────────
        #region DriveRGV
        private void DriveRGV()
        {
            try
            {
                string strSql = "";
                strSql += CRLF + " SELECT JM.LUGG_NO, JM.JOB_TYP, JM.START_POS, JM.DEST_POS,      ";
                strSql += CRLF + "        RD.RTV_NO                                                ";
                strSql += CRLF + "   FROM JOB_MST JM                                               ";
                strSql += CRLF + "  INNER JOIN RTV_DATA_LGLS RD                                         ";
                strSql += CRLF + "     ON JM.WH_TYP    = RD.WH_TYP                                 ";
                strSql += CRLF + "  WHERE JM.WH_TYP          = :WH_TYP                             ";
                strSql += CRLF + "    AND JM.JOB_STATUS      = :ST_WAIT                            ";   // [LGLS 2026-08-31] CV 구동중(15) - RGV 대기 상태(30) 폐기
                strSql += CRLF + "    AND RD.AUTO_MODE_RD    = '1'                                 ";   // AUTO
                strSql += CRLF + "    AND RD.OD_RQ_YN        = 'N'                                 ";   // 설비 유휴
                strSql += CRLF + "    AND RD.SUBSYSTEM_STATUS_RD = '1'                             ";   // [LGLS 2026-07-21] IDLE 일 때만(물리 1대 — 반송 중 중복 지시 방지)
                // [LGLS 2026-07-24] (제거) COMPLETE_RD<>'1' 게이트 삭제 — 완료신호 '1' 잔류가 RTV 신규 지시를
                //   영구 차단하는 데드락 방지. 오소비 방지는 아래 31/35 배타 게이트가 커버.
                // [LGLS 2026-07-21] RTV 는 물리 1대 — 운반 중(31/35) 작업이 남아 있으면 신규 지시 금지.
                //   (설비 리드백 지연 ~1초 동안 OD_RQ_YN/STATUS 게이트가 통과되는 창이 있어, 연속 지시가
                //    직전 지시의 LUGG_OD 를 덮어써 앞 작업의 완료 귀속이 유실된다 — DB 작업상태는 지연이 없다)
                // [LGLS 2026-08-31] RTV 1대 배타는 IsRtvBusyWithOwnJob() 이 한다 -
                //   "35 이면서 RTV 가 그 작업번호를 받아 동작 중" 일 때만 막는다(사용자 확정).
                //   상태만 보고 막으면 RTV 가 손 뗀 뒤 상태만 남았을 때 영구 정체가 된다.
                // [LGLS 2026-07-23] SC1/C\V#2 교착 방지: 901행 입고의 RTV 지시는 ① SC1 이 출고를 수령(21/25)하지 않았고
                //   ② C\V#2(103/104)에 출고 작업 화물이 없을 때만 발행한다.
                strSql += CRLF + "    AND NOT ( JM.JOB_TYP IN ('1','11') AND JM.DEST_POS = '901' AND (     ";
                strSql += CRLF + "          EXISTS (SELECT 1 FROM JOB_MST J5                               ";
                strSql += CRLF + "                   WHERE J5.WH_TYP = JM.WH_TYP                           ";
                strSql += CRLF + "                     AND J5.JOB_TYP IN ('2','12')                        ";
                strSql += CRLF + "                     AND J5.START_POS = '901'                            ";
                strSql += CRLF + "                     AND J5.JOB_STATUS IN ('25'))                   ";
                strSql += CRLF + "       OR EXISTS (SELECT 1 FROM CV_DATA C3                               ";
                strSql += CRLF + "                   INNER JOIN JOB_MST J6 ON J6.WH_TYP  = C3.WH_TYP       ";
                strSql += CRLF + "                                       AND J6.LUGG_NO = C3.LUGG_NO_RD    ";
                strSql += CRLF + "                   WHERE C3.WH_TYP = JM.WH_TYP                           ";
                strSql += CRLF + "                     AND C3.MC_NO IN ('103','104')                       ";
                strSql += CRLF + "                     AND J6.JOB_TYP IN ('2','12')                        ";
                strSql += CRLF + "                     AND J6.JOB_STATUS NOT IN ('09','19','29')) ) )       ";
                strSql += CRLF + "    AND (RD.ERR_CODE_RD = '0' OR RD.ERR_CODE_RD = '0000' OR RD.ERR_CODE_RD IS NULL)";
                strSql += CRLF + "  ORDER BY JM.INS_DT, JM.LUGG_NO                    ";   // [LGLS 2026-09-01] 오래 기다린 작업부터(라인별 FIFO 전제)

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_WAIT", DbLang.VARCHAR).Value = ST_CV_RUN;   // [LGLS 2026-08-31] 15 에서 RGV 가 가져간다
                int nCnt = DbQry(strSql);
                if (nCnt <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();

                // [LGLS 2026-09-01] ★라인별 FIFO★ (기아 실측 : 로직2 입고 9002 가 30분 대기)
                //   로직1/3 의 새 입고가 같은 S/C 라인을 반복 선점해, 라인 점유로 보류된
                //   오래된 작업이 영영 순번을 얻지 못했다. 어떤 후보가 라인 사유로 보류되면
                //   같은 라인을 쓰려는 ★이후(더 새로운) 후보★ 도 이번 사이클엔 보류한다 -
                //   라인이 비는 순간 가장 오래 기다린 작업이 잡는다. (후보는 오래된 순 정렬)
                var setBlockedLine = new HashSet<string>();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string rtvNo   = GetVal(dt.Rows[i], "RTV_NO");
                    string luggNo  = GetVal(dt.Rows[i], "LUGG_NO");
                    string jobTyp  = GetVal(dt.Rows[i], "JOB_TYP");
                    if (jobTyp == "11") jobTyp = "1"; else if (jobTyp == "12") jobTyp = "2";   // [LGLS 2026-07-20] 반자동(11/12) → 기본형 정규화(JOB_MST 원본은 유지)
                    string startPos= GetVal(dt.Rows[i], "START_POS");
                    string destPos = GetVal(dt.Rows[i], "DEST_POS");

                    string key = "RGV_" + rtvNo;
                    if (PrevIssued(m_dicPrevRGV, key, luggNo)) continue;  // [LGLS 2026-08-31] 5초 만료

                    // [LGLS 2026-07-21] 실경로 게이트 (AutoRunRGV 에서 이식):
                    // [LGLS 2026-08-31] 출고도 이 경로를 탄다(OUT_VIA_RGV_STATE=1).
                    //   입고 : 픽업 = 입고대 픽업트랙,     드롭 = S/C 라인 드롭트랙
                    //   출고 : 픽업 = S/C 라인 홀수 트랙,  드롭 = 출고대 픽업트랙
                    bool bOutRgv = (jobTyp == "2");   // [LGLS 2026-08-31] 출고도 RGV 구간을 상태로 표현한다
                    string pickupTrack = bOutRgv ? RgvPickupTrack(RgvOutDropTrack(startPos)) : RgvPickupTrack(startPos);
                    string dropTrack   = bOutRgv ? RgvPickupTrack(destPos)                   : RgvDropTrack(destPos);
                    string lineKey = bOutRgv ? "" : RgvDropTrack(destPos);
                    if (!bOutRgv && setBlockedLine.Contains(lineKey)) continue;         // 앞선(더 오래된) 작업이 이 라인 대기 중
                    if (!bOutRgv && !CanEnterLine(dropTrack, luggNo)) { setBlockedLine.Add(lineKey); continue; }   // 드롭 라인 점유 시 대기 (CV_DATA 기준)
                    // [LGLS 2026-08-30] 위 판정은 CV_DATA 미러(최대 ~16초 지연) 기반이라, RTV 가 막
                    //   내려놓은 화물을 못 보고 같은 드롭 트랙에 다음 입고를 또 보낼 수 있다(크레인 충돌).
                    //   JOB_MST 로 지연 없이 한 번 더 막는다.
                    if (!bOutRgv && HasInboundWaitingOnScLine(destPos, luggNo))
                    {
                        setBlockedLine.Add(lineKey);
                        DbgLog("RGVLINE_" + rtvNo, string.Format("[RGV] 보류 - S/C {0} 라인에 픽업 대기 화물 있음(작업 {1})", destPos, luggNo));
                        continue;
                    }
                    // [LGLS 2026-08-31] RTV 1대 배타 (사용자 확정 조건)
                    //   "JOB_STATUS=35 이면서 RTV 가 그 작업번호를 받아 동작 중(RTV 레일 파랑)" 일 때만 막는다.
                    //   단순히 35 인 작업이 있다고 막으면, RTV 가 이미 놓아버린 뒤 상태만 남은 경우
                    //   영구히 아무도 못 받는다(m_dicPrevCV 에서 겪은 함정과 같은 형태).
                    if (IsRtvBusyWithOwnJob(luggNo))
                    {
                        DbgLog("RGVBUSY_" + rtvNo, string.Format("[RGV] 보류 - RTV 가 다른 작업 반송 중(작업 {0})", luggNo));
                        continue;
                    }
                    // [LGLS 2026-08-31] RtvBusyByOutbound(구 메모리 큐 기반) 제거 - 위 IsRtvBusyWithOwnJob 이 대신한다.
                    if (IsRtvSuspended()) continue;                                    // RTV 작업정지
                    if (IsCvPaused(pickupTrack) || IsCvPaused(dropTrack)) continue;    // 작업대 일시정지
                    // [LGLS 2026-08-30] SC1 특례(출고 우선).
                    //   ※에이징 우회는 넣지 않는다 - 사용자 확정 : "보류가 길어져도 통과시키면 안 된다".
                    //     보류는 물리적 이유로 서는 것이라 시간으로 뚫으면 정면 경합이 난다
                    //     (실제로 에이징을 넣었을 때 S/C #1 충돌이 발생했다).
                    //   기아는 우회가 아니라 **보류 조건 자체를 정확히 좁혀서** 푼다 -
                    //   HasActiveSc1Outbound 는 실제로 라인을 점유한 출고만 센다(대기 20 제외).
                    if (!bOutRgv && destPos == "901" && HasActiveSc1Outbound()) continue;
                    // [LGLS 2026-08-31] ★픽업트랙의 화물이 그 작업의 것인지까지 확인한다★
                    //   종전에는 "비어 있지 않다" 만 봤다. 신규 체계에서 입고는 15 를 두 번 지나므로
                    //   (CV 이동 후 / RGV 착지 후) 뒤 작업의 화물을 앞 작업이 집어가는 일이 생겼다.
                    //   실측 : 9003 의 화물이 123 에 있는데 9001(15) 이 그것을 대상으로 반복 지시했고,
                    //          EQP_SIM 이 "JOB 재부여: 9002 → 9001" 을 남기며 작업번호가 덮어써졌다.
                    //          그 뒤 크레인이 화물을 든 채 비유휴(UCSTATUS=2)로 굳어 전면 정체.
                    if (IsTrackEmpty(pickupTrack)) continue;                           // 화물이 픽업트랙 도착 후에만
                    {
                        string _lgOnTrk = (TrackLugg(pickupTrack) ?? "").Trim();
                        if (_lgOnTrk != luggNo)
                        {
                            DbgLog("RGVOWN_" + rtvNo, string.Format("[RGV] 보류 - 픽업트랙 {0} 의 화물이 다른 작업({1})",
                                   pickupTrack, string.IsNullOrEmpty(_lgOnTrk) ? "미상" : _lgOnTrk));
                            continue;
                        }
                    }

                    // [LGLS 2026-08-22] S/C #1 겸용 통로 C/V#2(103/104) 방향 맞추기.
                    //   입고는 RGV 가 103 에 내려놓는 것이 SC 지시보다 먼저다 - SC 단계에서만 전환하면
                    //   그 앞 RGV 단계에서 도착 HS 가 서지 않아 작업이 '30' 에서 멈춘다.
                    if (dropTrack == SC1_DUAL_CV && GetCvStockMode(SC1_DUAL_CV) != "0")
                    {
                        if (!IsDualCvBusyWithJob(SC1_DUAL_CV, "0"))   // [LGLS 2026-08-30] 반대 방향(출고) 작업 화물이 있을 때만 보류
                        {
                            if (RequestCvDirection(SC1_DUAL_CV, "0"))
                                MakeMsg_Imp(string.Format("[SCH][RGV] 겸용 통로 C/V#2({0}) 방향 전환 지시 - 입고(0) (작업 {1} 드롭 대기)",
                                    SC1_DUAL_CV, luggNo));
                        }
                        else
                            DbgLog("DUALCV2", string.Format("[겸용통로] 방향 전환 보류 - 트랙 {0} 점유(작업 {1})", SC1_DUAL_CV, luggNo));
                        continue;   // 방향이 설비에 반영된 뒤 다음 폴링에 지시
                    }

                    // [LGLS 2026-08-22] 핸드셰이크 최우선 규칙
                    //   출발지 HS(RGV 가 집어갈 준비)와 도착지 HS(RGV 가 내려놓을 준비)가 모두 서야 지시한다.
                    // [LGLS 2026-08-30] 픽업/드롭 컨베이어가 에러면 RTV 지시 금지 —
                    //   에러난 CV 는 화물을 내보내지도 받지도 못하므로 RTV 가 그 앞에서 멈춘다.
                    if (IsCvError(pickupTrack))
                    {
                        DbgLog("RGVERR_" + rtvNo, string.Format("[RGV] 보류 - 출발지 CV {0} 에러(작업 {1})", pickupTrack, luggNo));
                        continue;
                    }
                    if (IsCvError(dropTrack))
                    {
                        DbgLog("RGVERR_" + rtvNo, string.Format("[RGV] 보류 - 도착지 CV {0} 에러(작업 {1})", dropTrack, luggNo));
                        continue;
                    }
                    // [LGLS 2026-09-04] (사용자 확정) 도착지에 "RGV 완료(39)됐지만 아직 착지 기록이 안 된 작업" 이 있으면
                    //   지시하지 않는다. 39 + HS_TRACK_NO = 도착트랙 이 그 상태다. 미러(CV_DATA) 지연과 무관하게
                    //   JOB_MST 만으로 판정하므로 같은 트랙 중복 하역이 원천 차단된다. 운전자가 알 수 있게 알람 이력에 남긴다.
                    {
                        string pendLugg = RgvDoneNotLanded(dropTrack, luggNo);
                        if (!string.IsNullOrEmpty(pendLugg))
                        {
                            string msgPend = string.Format("[RGV] 지시 보류 - 도착지 {0} 에 RGV 완료(39) 후 착지 기록 대기 작업 {1} 있음 (지시하려던 작업 {2})", dropTrack, pendLugg, luggNo);
                            DbgLog("RGVPEND_" + dropTrack, msgPend);
                            MakeMsg_Imp("[SCH]" + msgPend);
                            continue;
                        }
                    }
                    if (!IsHsOn(pickupTrack, "RTV_DEPARTHS_READY_RD"))
                    {
                        DbgLog("RGVHS_" + rtvNo, string.Format("[RGV] 보류 - 출발지 {0} 출발 HS 신호 없음(작업 {1})", pickupTrack, luggNo));
                        continue;
                    }
                    if (!IsHsOn(dropTrack, "RTV_ARRIVEHS_READY_RD"))
                    {
                        DbgLog("RGVHS_" + rtvNo, string.Format("[RGV] 보류 - 도착지 {0} 도착 HS 신호 없음(작업 {1})", dropTrack, luggNo));
                        continue;
                    }

                    MakeMsg_Imp(string.Format("[SCH][RGV] RGV #{0} 작업:{1} → 반송지시 (픽업 {2} → 드롭 {3})",
                        rtvNo, luggNo, pickupTrack, dropTrack));

                    _pBdb.BeginTrans();

                    string rtn = "";
                    // [LGLS 2026-08-31] ★35 로 올리면서 RGV 도착지를 HS_TRACK_NO 에 함께 적는다★ (사용자 지시)
                    //   이 한 줄이 "누가 이 화물을 책임지는가" 를 DB 에 남긴다. 프로세스가 죽어도
                    //   39 인 작업과 그 도착지가 그대로 남아 고아가 생길 자리가 없다.
                    bool ok = UpdateRtvData(rtvNo, jobTyp, luggNo, pickupTrack, dropTrack, ref rtn)
                              && UpdateJobStatusHs(ST_RGV_RUN, luggNo, dropTrack, ref rtn);

                    if (ok)
                    {
                        _pBdb.Commit();
                        m_dicPrevRGV[key] = luggNo; m_dicPrevAt[key] = DateTime.Now;
                        MakeMsg_Imp(string.Format("[SCH][RGV] RGV #{0} 명령 발행 완료, 작업 {1} 상태 '{2}'", rtvNo, luggNo, ST_RGV_RUN));
                        break;   // [LGLS 2026-07-21] RTV 1대 — 한 폴링에 1건만 발행(같은 결과셋의 후속 후보가 덮어쓰지 않게)
                    }
                    else
                    {
                        _pBdb.Rollback();
                        MakeMsg_Error(string.Format("[SCH][RGV] RGV #{0} 발행 실패: {1}", rtvNo, rtn));
                    }
                }
            }
            catch (System.Data.Common.DbException)
            {
                // RTV_DATA_LGLS 테이블 미존재(RGV 미사용 현장) 시 무시
                try { _pBdb.Rollback(); } catch { }
            }
            catch (Exception ex)
            {
                MakeMsg_Error("[SCH][RGV] DriveRGV 오류: " + ex.Message);
                try { _pBdb.Rollback(); } catch { }
            }
        }
        #endregion

        // ─────────────────────────────────────────────────────────────────
        // 구동 중 (지시 → 중)
        //   설비가 명령을 수신(OD_RQ_YN 이 'Y'→'N' 으로 리셋)하면 "구동 중" 으로 전이.
        // ─────────────────────────────────────────────────────────────────

        // ─────────────────────────────────────────────────────────────────
        // 완료 감지
        //   [LGLS] 2단계 처리 반송 체인 확정 (구 ★정책확인 반영):
        //     입고(JOB_TYP='1') : CV 완료(15) → '20'(SC 처리 인계) / SC 완료(25) → '29'(최종)
        //     출고(JOB_TYP='2') : SC 완료(25) → '10'(CV 처리 인계) / CV 완료(15) → '19'(최종)
        //   ※ 최종 상태(19/29)는 WCS_TASK_HOST.GetJobCompleteReport 가 F(작업완료) 보고로
        //     소비하는 값이다. 중간 처리가 19/29 를 거치면 조기 완료보고가 나가므로 금지.
        // ─────────────────────────────────────────────────────────────────
        #region CompleteCV
        private void CompleteCV()
        {
            try
            {
                // 구동중(15) 작업 중, 명령을 발행한 CV(MC_NO=CV처리 설비위치)에서
                // 명령이 소비되고(OD_RQ_YN='N') 팔레트가 소스에서 이탈(LUGG_NO_RD='0000')하면 완료.
                string strSql = "";
                strSql += CRLF + " SELECT JM.LUGG_NO, JM.JOB_TYP, CD.MC_NO,                    ";
                strSql += CRLF + "        CD.SENSOR0_DATA_RD, CD.LUGG_NO_RD, CD.RET_READY_RD       ";
                strSql += CRLF + "   FROM JOB_MST JM                                           ";
                strSql += CRLF + "  INNER JOIN CV_DATA CD                                      ";
                strSql += CRLF + "     ON CD.WH_TYP = JM.WH_TYP AND CD.MC_NO = " + CV_POS_EXPR + " ";   // [LGLS]
                strSql += CRLF + "  WHERE JM.WH_TYP     = :WH_TYP                              ";
                strSql += CRLF + "    AND JM.JOB_STATUS = :ST_DONE                             ";
                strSql += CRLF + "    AND CD.OD_RQ_YN   = 'N'                                  ";
                // [LGLS 2026-07-21] 물리 이관: 빈트랙/도착 판정은 코드 분기로 이동
                //   (입고=소스 이탈 확인, 출고=도착 관측 후 배출 확인 — CvThread 의 도착 예약 트래킹 기록을
                //    도착으로 오인하던 조기 완료 방지)

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_DONE", DbLang.VARCHAR).Value = ST_CV_RUN;   // 중(15) 대상
                int nCnt = DbQry(strSql);
                if (nCnt <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string luggNo = GetVal(dt.Rows[i], "LUGG_NO");
                    string jobTyp = GetVal(dt.Rows[i], "JOB_TYP");
                    if (jobTyp == "11") jobTyp = "1"; else if (jobTyp == "12") jobTyp = "2";   // [LGLS 2026-07-20] 반자동(11/12) → 기본형 정규화(JOB_MST 원본은 유지)
                    string mcNo   = GetVal(dt.Rows[i], "MC_NO");
                    string cvSen  = GetVal(dt.Rows[i], "SENSOR0_DATA_RD");
                    string cvLugg = GetVal(dt.Rows[i], "LUGG_NO_RD");
                    string cvRetRdy = GetVal(dt.Rows[i], "RET_READY_RD");   // [LGLS 2026-08-23] 출고대 신호(구 ECS WAIT_IN)
                    string rtn = "";

                    // [LGLS 2026-07-19] 출고대 일시정지(TR_PAUSE, 내부값) 시 도착완료 처리 보류 (해제되면 다음 폴링에 완료)
                    if (jobTyp == "2" && IsCvPaused(mcNo)) continue;

                    // [LGLS 2026-07-21] 완료 판정 (물리 이관):
                    //   입고(1) : 픽업트랙(RGV 상차 대기 위치)에 실화물 도착(SENSOR=1 + 작업번호 일치)이면 완료.
                    //             ("소스 빈 트랙" 판정은 입고대 자동 재투입이 이탈 직후 일어나면 빈 순간을
                    //              관측할 수 없어 영구 대기한다 — 도착 관측은 재투입과 무관)
                    //   출고(2) : 목적지(출고대)에서 실화물 도착(SENSOR=1 + 작업번호 일치)을 1회 관측한 뒤
                    //             배출(빈 트랙)까지 확인되면 완료. (CvThread 의 출고 지시가 도착 예약 트래킹을
                    //             미리 기록하므로 트래킹만으로 판정하면 조기 완료된다)
                    bool bEmpty = (cvLugg == "" || cvLugg == "0" || cvLugg == "0000") && cvSen != "1";
                    if (jobTyp == "1")
                    {
                        string pickup = RgvPickupTrack(mcNo);
                        if (IsTrackEmpty(pickup) || TrackLugg(pickup) != luggNo) continue;
                    }
                    else
                    {
                        // [LGLS 2026-07-21] 반출(RTV) 시퀀스가 끝나기 전의 "도착"은 인정하지 않는다.
                        if (OutSeqPending(luggNo)) continue;

                        // [LGLS 2026-08-23] 출고 완료의 키는 **출고대 신호**(CV_DATA.RET_READY_RD) 다.
                        //   구 ECS 의 WAIT_IN 과 같은 비트로(PlcAddressMap: WorkInstruction offset 5),
                        //   설비가 "출고대에 작업번호를 가진 화물이 실제로 놓였다"를 이 신호로 알린다.
                        //   종전에는 화물 트래킹이 어디에도 안 보이는 상태가 2초 지속되면 완료로 인정했는데,
                        //   RGV 가 하역한 직후 - 아직 출고대에 도착하기도 전에 - 그 조건이 성립해서
                        //   작업이 조기 삭제됐다(작업 2034 사례).
                        bool bMine = (cvLugg == "" || cvLugg == "0" || cvLugg == "0000" || cvLugg == luggNo);

                        if (cvRetRdy == "1" && bMine)
                        {
                            MakeMsg_Imp(string.Format("[SCH][CV] 작업 {0} 출고대 {1} 출고대 신호 ON - 출고 완료", luggNo, mcNo));
                        }
                        else
                        {
                            // [LGLS 2026-08-24] 보조 판정 : 출고대 신호는 설비가 잠깐만 올리는 펄스라
                            //   폴링 주기(약 3초) 사이에 통째로 지나갈 수 있다(작업 2064 로 실제 유실).
                            //   그 경우엔 '출고대에서 내 화물을 실제로 봤다(센서+작업번호) → 배출됐다' 로 인정한다.
                            //   이 경로는 출고대 도착을 직접 관측하므로 조기 완료가 되지 않는다.
                            if (cvSen == "1" && cvLugg == luggNo)
                            {
                                if (!m_setOutArrived.Contains(luggNo))
                                {
                                    m_setOutArrived.Add(luggNo);
                                    MakeMsg_Imp(string.Format("[SCH][CV] 작업 {0} 출고대 {1} 실도착 관측 (신호/배출 대기)", luggNo, mcNo));
                                }
                                continue;
                            }
                            if (m_setOutArrived.Contains(luggNo))
                            {
                                if (!bEmpty) continue;                       // 아직 배출 전
                                MakeMsg_Imp(string.Format("[SCH][CV] 작업 {0} 출고대 {1} 배출 확인 - 출고 완료", luggNo, mcNo));
                            }
                            else
                            {
                                // [LGLS 2026-08-24] 최후 안전망 : 신호도 도착도 못 봤는데 화물 트래킹이
                                //   전 트랙 어디에도 없는 상태가 오래 지속되면, 이미 나갔는데 관측만 놓친 것이다.
                                //   영구 정체를 막되 조기 완료가 되지 않도록 유예를 길게 둔다(2034 는 2초에 지워졌다).
                                if (LuggOnAnyTrack(luggNo))
                                {
                                    m_dicOutDoneDt.Remove(luggNo);
                                    continue;
                                }
                                DateTime dtOutDone;
                                if (!m_dicOutDoneDt.TryGetValue(luggNo, out dtOutDone))
                                {
                                    m_dicOutDoneDt[luggNo] = DateTime.Now;
                                    DbgLog("OUTRDY_" + luggNo, "[출고대신호대기] " + luggNo + " 트랙 " + mcNo + " 출고대 신호 OFF - 완료 보류");
                                    continue;
                                }
                                if ((DateTime.Now - dtOutDone).TotalMilliseconds < OUT_SIGNAL_MISS_MS) continue;
                                MakeMsg_Error(string.Format(
                                    "[SCH][CV] 작업 {0} 출고대 {1} - 출고대 신호도 실도착도 관측하지 못했습니다. 화물 트래킹 {2}초 무관측이라 완료로 인정합니다(설비 신호 확인 필요).",
                                    luggNo, mcNo, OUT_SIGNAL_MISS_MS / 1000));
                            }
                        }
                        m_setOutArrived.Remove(luggNo);
                        m_dicOutDoneDt.Remove(luggNo);
                    }

                    // [LGLS 2026-08-31] 입고는 15 를 유지한다 - RGV 가 15 에서 화물을 가져간다.
                    //   (RGV 대기 상태 30 폐기). 출고만 최종 완료(19 → HOST 2차 완료보고).
                    if (jobTyp == "1")
                    {
                        // [LGLS 2026-08-31] ★상태는 그대로 두더라도 지시값 정리는 반드시 한다★
                        //   여기서 그냥 continue 했더니 LUGG_NO_OD 에 죽은 작업번호가 남았고,
                        //   SweepStaleTracking 이 "지시가 걸린 트랙" 으로 보고 유령 트래킹을
                        //   영영 못 지웠다(실측 : 트랙 115 에 0355 가 40분 잔류 → 도착 HS 미성립 → 0371 정체).
                        ClearScOd(luggNo);
                        ClearCvOd(luggNo);
                        m_dicPrevCV.Remove("CV_" + mcNo);
                        continue;
                    }
                    string stNext = ST_CV_DONE;

                    if (UpdateJobStatus(stNext, luggNo, ref rtn))
                    {
                        ClearScOd(luggNo);	// [LGLS] SC 작업 완료 -> 해당 작업 od 클리어(잔류 방지)
                        ClearCvOd(luggNo);	// [LGLS 2026-08-23] CV 지시값(LUGG_NO_OD)도 비운다 - 남아 있으면 R트래킹이 재기록된다
                        m_dicPrevCV.Remove("CV_" + mcNo);   // [LGLS] 발행 키(CV_+트랙번호)로 해제
                        // [LGLS] 출고대 반출(21→22)는 SC 완료 시 m_dicOutStn(ProcessOutStn)에서 RTV 반송과 함께 시퀀스 처리
                        if (jobTyp == "1")
                            MakeMsg_Imp(string.Format("[SCH][CV] 작업 {0} CV 반송 완료 → RGV 처리 인계 (상태 '{1}')", luggNo, stNext));
                        else
                            MakeMsg_Imp(string.Format("[SCH][CV] 작업 {0} CV 반송 완료(출고 최종) → 상태 '{1}' (HOST 완료보고 대상)", luggNo, stNext));
                    }
                    else
                        MakeMsg_Error(string.Format("[SCH][CV] 완료 전이 실패({0}): {1}", luggNo, rtn));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][CV] CompleteCV 오류: " + ex.Message); }
        }
        #endregion



        // [LGLS 2026-08-22] 작업 체류 감시용 상태
        private Dictionary<string, string> m_dicStallWarned = new Dictionary<string, string>();
        private DateTime m_dtNextStallScan = DateTime.MinValue;

        /// <summary>
        /// [LGLS 2026-08-22] 작업 체류(무응답) 감시.
        ///   설비가 응답을 멈추면 작업은 그 상태 그대로 무한정 남고, 뒤따르는 작업까지 적체된다
        ///   (실측: RGV 가 상태 35 로 11시간 정지 → 작업 9건 적체, 경고 한 줄 없었음).
        ///   완료(29/19)가 아닌 작업이 임계시간 넘게 갱신되지 않으면 경고를 남긴다.
        ///   자동 회복은 하지 않는다 — 실물 설비 상태를 모른 채 상태를 건드리는 편이 더 위험하다.
        ///   임계시간 : ENV_IOSCH.INI [CNF] JOB_STALL_WARN_SEC (기본 300초, 0=감시 끔)
        /// </summary>
        private void CheckStalledJobs()
        {
            try
            {
                int nWarnSec = cDefApi.GsReadInitProfileCnf("JOB_STALL_WARN_SEC", 300);
                if (nWarnSec <= 0) return;
                if (DateTime.Now < m_dtNextStallScan) return;
                m_dtNextStallScan = DateTime.Now.AddSeconds(60);      // 1분 주기 점검

                string q = "";
                q += CRLF + " SELECT JM.LUGG_NO, JM.JOB_STATUS, JM.JOB_TYP, JM.START_POS, JM.DEST_POS,      ";
                q += CRLF + "        DATEDIFF(second, JM.UPD_DT, GETDATE()) AS IDLE_SEC                     ";
                q += CRLF + "   FROM JOB_MST JM                                                            ";
                q += CRLF + "  WHERE JM.WH_TYP      = :WH_TYP                                              ";
                q += CRLF + "    AND JM.JOB_STATUS NOT IN ('29','19')                                      ";
                q += CRLF + "    AND DATEDIFF(second, JM.UPD_DT, GETDATE()) >= :IDLE                       ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("IDLE",   DbLang.VARCHAR).Value = nWarnSec.ToString();
                int nCnt = DbQry(q);
                if (nCnt <= 0) { m_dicStallWarned.Clear(); return; }

                DataTable dt = _pBdb.mDtMain.Copy();
                List<string> lstAlive = new List<string>();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string luggNo = GetVal(dt.Rows[i], "LUGG_NO");
                    string status = GetVal(dt.Rows[i], "JOB_STATUS");
                    lstAlive.Add(luggNo);

                    // 같은 상태로 계속 머무는 동안에는 한 번만 알린다(상태가 바뀌면 다시 알림)
                    string strPrev;
                    if (m_dicStallWarned.TryGetValue(luggNo, out strPrev) && strPrev == status) continue;
                    m_dicStallWarned[luggNo] = status;

                    MakeMsg_Error(string.Format(
                        "[SCH][체류경고] 작업 {0} 이(가) 상태 '{1}' 로 {2}초째 진행되지 않습니다 ({3} → {4}). 설비 응답을 확인하세요.",
                        luggNo, status, GetVal(dt.Rows[i], "IDLE_SEC"),
                        GetVal(dt.Rows[i], "START_POS"), GetVal(dt.Rows[i], "DEST_POS")));

                    // [LGLS 2026-09-06] SC 구동중(25) 체류는 완료신호 유실이 원인일 수 있다
                    //   (VehThread 가 Ack 를 PLC 에 쓴 직후 COMPLETE_RD 커밋 전에 죽는 좁은 창).
                    //   ★작업당 딱 1회★ 시간 기반 자동 처리를 허용해 스스로 빠져나오게 하고,
                    //   그러고도 다시 체류하면 자동 처리하지 않고 그대로 세워 둔다(사용자 확정).
                    //   - 세워 두는 이유 : 두 번째부터는 신호 유실이 아니라 설비/작업 자체의
                    //     문제일 가능성이 높다. 자동으로 계속 밀어내면 원인이 가려진다.
                    if (status == ST_SC_RUN)
                    {
                        if (m_setAutoTimeUsed.Contains(luggNo))
                        {
                            MakeMsg_Error(string.Format(
                                "[SCH][체류경고] 작업 {0} - 시간 기반 자동 처리를 이미 1회 사용했습니다. "
                                + "자동 처리하지 않고 정지 상태로 둡니다. 설비와 작업을 확인하세요.", luggNo));
                        }
                        else if (m_setAutoTimeGrant.Add(luggNo))
                        {
                            MakeMsg_Imp(string.Format(
                                "[SCH][체류복구] 작업 {0} - 완료신호 유실 가능성. 시간 기반 자동 처리를 1회 허용합니다.", luggNo));
                        }
                    }
                }

                // 정상 진행으로 돌아선 작업은 경고 이력에서 제거
                List<string> lstDrop = new List<string>();
                foreach (string k in m_dicStallWarned.Keys)
                    if (!lstAlive.Contains(k)) lstDrop.Add(k);
                foreach (string k in lstDrop) m_dicStallWarned.Remove(k);

                // [LGLS 2026-09-06] 사라진(완료/삭제된) 작업의 1회 허용 이력도 함께 정리한다.
                //   JOB_MST 에 없는 작업번호를 계속 들고 있을 이유가 없다.
                CleanupAutoTimeSets();
            }
            catch (Exception ex)
            {
                MakeMsg_Error("[SCH] 작업 체류 감시 오류: " + ex.Message);
            }
        }

        #region CompleteSC
        // [LGLS 2026-08-23] 크레인 구분 동작(포크출/호이스트/포크센터 각 3초 x 2 + 주행)의 최소 소요.
        //   이보다 빨리 오는 '완료' 는 이전 작업의 잔류 신호로 본다.
        private const int SC_MIN_RUN_SEC = 8;

        // [LGLS 2026-09-06] 체류 시 1회 한정 시간 기반 자동 처리 (사용자 확정)
        //   Grant   : 지금 1회 허용된 작업 / Used : 이미 1회 써버린 작업(다시는 자동 처리하지 않는다)
        //   UsedSc  : 그 호기가 자동 처리를 쓴 뒤 아직 ★정상 완료신호★ 를 한 번도 못 낸 상태.
        //             신호가 계속 죽어 있으면 작업마다 1회씩 복구되어 결함이 영영 가려지므로,
        //             호기 단위로도 한 번만 허용하고 그 뒤에는 멈춰 세운다.
        //             그 호기가 완료신호로 정상 완료하면 신호가 살아난 것이므로 해제한다.
        //   RefuseLogged : 같은 작업에 대한 거부 메시지를 한 번만 남기기 위한 표시.
        private readonly HashSet<string> m_setAutoTimeGrant   = new HashSet<string>();
        private readonly HashSet<string> m_setAutoTimeUsed    = new HashSet<string>();
        private readonly HashSet<string> m_setAutoTimeUsedSc  = new HashSet<string>();
        private readonly HashSet<string> m_setAutoTimeRefused = new HashSet<string>();

        /// <summary>[LGLS 2026-09-06] JOB_MST 에 없는 작업번호를 허용/사용 이력에서 지운다.</summary>
        private void CleanupAutoTimeSets()
        {
            try
            {
                if (m_setAutoTimeGrant.Count == 0 && m_setAutoTimeUsed.Count == 0) return;
                string q = "";
                q += CRLF + " SELECT LUGG_NO FROM JOB_MST WHERE WH_TYP = :WH_TYP ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                var setAlive = new HashSet<string>();
                if (DbQry(q) > 0)
                {
                    DataTable dt = _pBdb.mDtMain.Copy();
                    for (int i = 0; i < dt.Rows.Count; i++)
                        setAlive.Add(GetVal(dt.Rows[i], "LUGG_NO"));
                }
                m_setAutoTimeGrant.RemoveWhere(k => !setAlive.Contains(k));
                m_setAutoTimeUsed.RemoveWhere(k => !setAlive.Contains(k));
                m_setAutoTimeRefused.RemoveWhere(k => !setAlive.Contains(k));
                // m_setAutoTimeUsedSc 는 호기 단위라 작업 소멸로 지우지 않는다.
                //   그 호기가 정상 완료신호를 낼 때(CompleteSC)만 해제된다.
            }
            catch { }
        }

        private void CompleteSC()
        {
            try
            {
                // 구동중(25) 작업 중, 해당 S/C(SC_NO=SC처리 설비위치)의 완료신호(COMPLETE_RD<>0)
                // 이고 명령이 소비(OD_RQ_YN='N')되었으면 완료.  [슬라이드17 Transfer Complete]
                string strSql = "";
                strSql += CRLF + " SELECT JM.LUGG_NO, JM.JOB_TYP, SD.SC_NO,                    ";
                // [LGLS 2026-09-06] 이 행이 ★설비 완료신호★ 로 걸린 것인지(1) 시간 기반으로 걸린 것인지(0).
                strSql += CRLF + "        CASE WHEN SD.COMPLETE_RD IS NOT NULL                       ";
                strSql += CRLF + "              AND SD.COMPLETE_RD NOT IN ('0','00','0000','')       ";
                strSql += CRLF + "             THEN '1' ELSE '0' END AS BY_SIGNAL                    ";
                strSql += CRLF + "   FROM JOB_MST JM                                           ";
                strSql += CRLF + "  INNER JOIN SC_DATA_LGLS SD                                       ";
                strSql += CRLF + "     ON SD.WH_TYP = JM.WH_TYP AND SD.SC_NO = " + SC_POS_EXPR + " ";   // [LGLS]
                strSql += CRLF + "  WHERE JM.WH_TYP      = :WH_TYP                             ";
                strSql += CRLF + "    AND JM.JOB_STATUS  = :ST_RUN                             ";
                strSql += CRLF + "    AND SD.OD_RQ_YN    = 'N'                                 ";
                // [LGLS 2026-08-23] 크레인이 실제로 멈춘 뒤에만 완료로 본다.
                //   구분 동작(포크출→호이스트→포크센터, 각 3초)을 도입하면서 '지시 후 화물을 들기까지'
                //   구간이 9초로 늘었다. 그 사이 이전 작업의 COMPLETE_RD 가 남아 있으면 화물을 뜨는
                //   도중에 완료로 처리돼 버린다(작업 1654 사례 - 입고 HS 에서 드는 중 완료).
                //   설비가 하역을 마치면 SUBSYSTEM_STATUS_RD 가 1(IDLE)이 되므로 그때만 인정한다.
                strSql += CRLF + "    AND ISNULL(SD.SUBSYSTEM_STATUS_RD,'1') = '1'             ";
                // [LGLS 2026-08-23] IDLE 은 '완료 후' 뿐 아니라 '아직 시작 전' 에도 참이다.
                //   지시를 소비(OD_RQ_YN='N')한 직후 설비가 RUN 으로 바뀌기 전 그 짧은 창에
                //   이전 작업의 완료신호가 남아 있으면 크레인이 움직이기도 전에 완료돼 버린다
                //   (작업 1726 - 구동중 전이 3초 만에 입고 최종). 구분 동작은 최소 18초가 걸리므로
                //   구동중(25) 전이 후 최소 경과시간을 둔다.
                strSql += CRLF + "    AND DATEDIFF(second, JM.UPD_DT, " + DbLang.SYSDATE + ") >= " + SC_MIN_RUN_SEC.ToString() + " ";
                // [LGLS 2026-09-04] 완료 판정은 ★설비 완료신호만★ 본다(시간 경과 추정 제거 - 사용자 확정).
                //   종전 주석의 "COMPLETE_RD 는 단발이라 놓치면 소실" 은 사실이 아니다 :
                //     · PLC 비트(Unload Complete)는 사양대로 핸드셰이크 펄스지만,
                //     · VehThread 가 그것을 관측해 DB COMPLETE_RD='1' 로 ★래치★ 하고(중복기록 방지 __cmpSeen),
                //       그 값은 ★그 설비에 새 지시가 소비될 때★ 비로소 '0' 이 된다(VehThread.ConsumeCommands).
                //   그리고 DriveSC 는 같은 크레인에 25 작업이 남아 있으면 신규 지시를 내지 않으므로,
                //   앞 작업의 완료신호가 새 지시에 지워질 창 자체가 없다. → 시간 백업이 필요 없다.
                //   [LGLS 2026-09-05 추가] 다만 완료신호가 실제로 유실되는 경로가 있었다(VehThread 가 Ack 를
                //   올리기만 하고 내리지 않아 설비가 다음 이벤트를 즉시 지움 - 같은 날 수정).
                //   그런 유실 대비 백업(유휴+포크빔+스트로브 내려감+경과)은 ★[환경설정] > [시간 기반 자동 처리]★
                //   가 선택돼 있을 때만 쓴다. 해제 상태면 설비 완료신호로만 처리한다(기본).
                // [LGLS 2026-09-06] 시간 기반 분기는 두 경우에 연다 :
                //   ① [환경설정] > [시간 기반 자동 처리] 가 켜져 있을 때 (종전 동작 - 상시)
                //   ② 체류경고로 그 작업에 1회 허용이 떨어졌을 때 (CheckStalledJobs)
                //   어느 행이 어느 분기로 걸렸는지는 BY_SIGNAL 로 구분해 아래 루프에서 판정한다.
                bool bAutoTime = AutoTimeProcEnabled();
                if (bAutoTime || m_setAutoTimeGrant.Count > 0)
                {
                    strSql += CRLF + "    AND ( ( SD.COMPLETE_RD IS NOT NULL AND SD.COMPLETE_RD NOT IN ('0','00','0000','') ) ";
                    strSql += CRLF + "       OR ( SD.UCSTATUS_RD = '1'                              ";
                    strSql += CRLF + "            AND SD.TRANSFER_REQUEST_OD = 'N'                   ";
                    strSql += CRLF + "            AND ISNULL(SD.ITN_LUGG_FK1,'0') IN ('0','00','0000','') ";
                    strSql += CRLF + "            AND DATEDIFF(second, JM.UPD_DT, GETDATE()) >= 3 ) ) ";
                }
                else
                {
                    strSql += CRLF + "    AND SD.COMPLETE_RD IS NOT NULL AND SD.COMPLETE_RD NOT IN ('0','00','0000','') ";
                }

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_RUN", DbLang.VARCHAR).Value = ST_SC_RUN;
                int nCnt = DbQry(strSql);
                if (nCnt <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string luggNo = GetVal(dt.Rows[i], "LUGG_NO");
                    string jobTyp = GetVal(dt.Rows[i], "JOB_TYP");
                    if (jobTyp == "11") jobTyp = "1"; else if (jobTyp == "12") jobTyp = "2";   // [LGLS 2026-07-20] 반자동(11/12) → 기본형 정규화(JOB_MST 원본은 유지)
                    string scNo   = GetVal(dt.Rows[i], "SC_NO");
                    string rtn = "";

                    bool bBySignal = (GetVal(dt.Rows[i], "BY_SIGNAL") == "1");

                    // [LGLS 2026-09-06] 완료신호로 정상 완료했다면 그 호기의 신호는 살아 있는 것이다.
                    //   자동 처리 사용 표시를 풀어, 다음에 한 번 더 구제받을 수 있게 한다.
                    if (bBySignal) m_setAutoTimeUsedSc.Remove(scNo);

                    // 설비 완료신호가 아니라 ★시간 기반★ 으로 걸린 행이면,
                    //   [시간 기반 자동 처리] 가 켜져 있거나 그 작업에 1회 허용이 있어야 한다.
                    if (!bBySignal && !bAutoTime)
                    {
                        if (!m_setAutoTimeGrant.Contains(luggNo)) continue;   // 허용 없음 - 그대로 세워 둔다

                        // 그 호기가 직전에도 자동 처리로 빠져나갔고 그 뒤 정상 완료신호가 한 번도
                        //   없었다면, 신호 자체가 죽어 있다는 뜻이다. 더 밀어내지 않고 세워 둔다.
                        if (m_setAutoTimeUsedSc.Contains(scNo))
                        {
                            if (m_setAutoTimeRefused.Add(luggNo))
                                MakeMsg_Error(string.Format(
                                    "[SCH][체류경고] 작업 {0} - S/C #{1} 은 직전에도 완료신호 없이 자동 처리했고 "
                                    + "그 뒤 정상 완료신호가 없습니다. 자동 처리하지 않고 정지 상태로 둡니다. "
                                    + "크레인 완료신호(UNLOAD_COMPLETE) 배선/설비를 확인하세요.", luggNo, scNo));
                            continue;
                        }

                        m_setAutoTimeGrant.Remove(luggNo);
                        m_setAutoTimeUsed.Add(luggNo);
                        m_setAutoTimeUsedSc.Add(scNo);
                        MakeMsg_Imp(string.Format(
                            "[SCH][체류복구] 작업 {0} - 시간 기반 자동 처리 1회 사용(완료신호 없이 완료 처리, S/C #{1}). "
                            + "이 호기가 정상 완료신호를 낼 때까지 추가 자동 처리는 하지 않습니다.", luggNo, scNo));
                    }

                    // [LGLS] 출고(2)는 CV 처리로 인계, 입고(1)는 최종 완료(29 → HOST F보고)
                    // [LGLS 2026-08-31] 29 = 크레인이 완료했다 (입고/출고 공통).
                    //   출고는 이 시점에 랙 셀이 비므로 HOST 가 1차 완료보고를 낸다.
                    //   그 뒤 하역 도착지에 데이터를 기록하며 15 가 되는 것은 LandScDrop() 이 한다.
                    string stNext = ST_SC_DONE;

                    if (UpdateJobStatus(stNext, luggNo, ref rtn))
                    {
                        ClearScOd(luggNo);	// [LGLS] SC 작업 완료 -> 해당 작업 od 클리어(잔류 방지)
                        m_dicPrevSC.Remove("SC_" + scNo);   // [LGLS] 발행 키(SC_+호기번호)로 해제
                        // [LGLS 2026-07-21] 완료신호 소비 즉시 리셋 — 잔존 COMPLETE_RD 를 다음 작업이
                        //   같은 폴링 창에서 보고 조기 완료되는 것 방지
                        try
                        {
                            _pBdb.mComMain.CommandType = CommandType.Text;
                            _pBdb.mComMain.Parameters.Clear();
                            _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                            _pBdb.mComMain.Parameters.Add("SC_NO",  DbLang.VARCHAR).Value = scNo;
                            string strSqlCmpRst = "";
                            strSqlCmpRst += CRLF + " UPDATE SC_DATA_LGLS                  ";
                            strSqlCmpRst += CRLF + "    SET COMPLETE_RD  = '0'       ";
                            strSqlCmpRst += CRLF + "  WHERE WH_TYP       = :WH_TYP   ";
                            strSqlCmpRst += CRLF + "    AND SC_NO        = :SC_NO    ";
                            DbNonQry(strSqlCmpRst);
                        }
                        catch { }
                        if (jobTyp == "2")
                            MakeMsg_Imp(string.Format("[SCH][SC] 작업 {0} S/C 이송 완료 → CV 처리 인계 (상태 '{1}')", luggNo, stNext));
                        else
                            MakeMsg_Imp(string.Format("[SCH][SC] 작업 {0} S/C 이송 완료(입고 최종) → 상태 '{1}' (HOST 완료보고 대상)", luggNo, stNext));
                    }
                    else
                        MakeMsg_Error(string.Format("[SCH][SC] 완료 전이 실패({0}): {1}", luggNo, rtn));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][SC] CompleteSC 오류: " + ex.Message); }
        }
        #endregion

        // ─────────────────────────────────────────────────────────────────
        // [LGLS] SC 자동완주 (TASK프로그램 부재 시뮬레이션)
        //   현 환경에는 SC PLC TASK프로그램이 없어(WCS_TASK_CV 는 CV 전용)
        //   SC_DATA_LGLS 핸드셰이크가 진행되지 않는다. [CNF] SC_AUTO_COMPLETE=1 이면
        //   SC 처리를 JOB_MST 상태만으로 타이머 전이시켜 시나리오를 완주시킨다.
        //     20(대기) → 21(지시, 즉시) → 25(중, 지시 후 1초) → 완료(지시 후 5초)
        //     완료 시 : 입고(1) → '29'(최종, HOST F보고 대상) / 출고(2) → '10'(CV 처리 인계)
        //   ※ SC_DATA_LGLS 의 _OD 컬럼은 기록하지 않는다 (소비 주체가 없어 OD_RQ_YN='Y' 로
        //     잔류하면 후속 작업이 막히기 때문). 실제 SC TASK프로그램 도입 시
        //     SC_AUTO_COMPLETE=0 으로 바꾸면 기존 DriveSC/RunSC/CompleteSC 경로가 동작한다.
        // ─────────────────────────────────────────────────────────────────

        // ─────────────────────────────────────────────────────────────────
        // [LGLS] RGV(RTV) 자동완주 (입고 전용, TASK프로그램 부재 시뮬레이션)
        //   입고 CV 완료('30') → RGV 가 입력 컨베이어에서 팔레트를 픽업하여 크레인 하역대(드롭 트랙)
        //   까지 물리적으로 이동하는 모습을 RTV_DATA_LGLS 관측치로 애니메이션한 뒤 SC 처리('20')로 인계.
        //     30(대기) → 31(지시: 빈 RGV 픽업지점 이동, 즉시)
        //              → 35(중: 픽업 후 드롭지점 이동, 지시 후 1.2초)
        //              → 20(드롭완료: RGV 홈복귀 IDLE + SC 처리 인계, 지시 후 2.6초)
        //   ※ RGV 는 1대(RTV_NO='801'). 레이아웃 토폴로지(픽업/드롭 트랙, 화면 셀)는 고정 매핑.
        //     _OD 명령 컬럼은 소비 주체(PLC TASK프로그램)가 없어 기록하지 않고 _RD 관측치만 갱신한다.
        // ─────────────────────────────────────────────────────────────────

        // [LGLS] RGV 라우팅 헬퍼 (고정 레이아웃 토폴로지 - 현장 배치도 기준 하드코딩)
        //   픽업 트랙 : START_POS(입력 컨베이어 워크스테이션) 가 짝수면 -1 한 홀수 트랙, 홀수면 그대로.
        //   예외 : 로직2 입고대 130(C/V#15=트랙 130/131/132, FirstTrack=130)만 RGV측이 **+1 한 131**(TR#31=plc10).
        //          짝수-1 규칙을 그대로 쓰면 129 가 되는데, 129 는 C/V#14 의 출고대라 별개 설비다.
        private static string RgvPickupTrack(string startPos)
        {
            if (startPos == "130") return "131";
            // [LGLS 2026-07-19] 출고대 129(C/V#14=트랙 127/128/129)는 홀수라 "홀수면 그대로" 규칙을 타면
            //   하역트랙=129(자기 자신)가 되어, RTV 하역 화물이 배출트랙을 스스로 점유 → Stage 3 영구 차단
            //   → m_dicOutStn 미해제 → RTV 운반 전면 정지 + FeedInGate 공급 보류 교착.
            //   실배치(RGV 하역=127 → 128 → 배출=129)대로 127 을 하역트랙으로 사용한다.
            if (startPos == "129") return "127";
            int n;
            if (int.TryParse(startPos, out n))
                return (n % 2 == 0) ? (n - 1).ToString() : startPos;
            return startPos;
        }
        //   드롭 트랙 : DEST_POS(크레인 9xx) → 크레인 하역대 트랙.
        private static readonly Dictionary<string, string> m_dicRgvDrop = new Dictionary<string, string>()
        { { "901", "103" }, { "902", "107" }, { "903", "111" }, { "904", "115" }, { "905", "119" } };
        private static string RgvDropTrack(string destPos)
        {
            return m_dicRgvDrop.ContainsKey(destPos) ? m_dicRgvDrop[destPos] : destPos;
        }
        //   [LGLS] 출고 라인 드롭 트랙(짝수) : 크레인 9xx → 출고 라인 짝수 트랙. SC 하역=짝수(14), RGV 픽업=홀수(13=짝수-1).
        //   일반식은 SC k → 출고 C/V#(2k-1) 이지만 k=1 은 C/V#1 이 존재하지 않음(라인 C/V 는 #2~#10).
        //   **S/C 1호기만 예외로 입고·출고 모두 C/V#2(트랙 103/104)** — 그래서 C/V#2 만 방향전환형(WorldModel HasDirection=(n==2)).
        //   즉 SC1 출고: H/S 하역=104(SC측 짝수포트) → RGV 픽업=103(RGV측 홀수포트, RT#03→plc3).
        //   (구값 "901","102" 는 트랙 101/102=존재하지 않는 C/V#1 을 가리켜 SC1 출고가 동작하지 않았음.)
        private static readonly Dictionary<string, string> m_dicRgvOutDrop = new Dictionary<string, string>()
        { { "901", "104" }, { "902", "106" }, { "903", "110" }, { "904", "114" }, { "905", "118" } };
        private static string RgvOutDropTrack(string cranePos)
        {
            return m_dicRgvOutDrop.ContainsKey(cranePos) ? m_dicRgvOutDrop[cranePos] : cranePos;
        }
        //   화면 셀 : 트랙번호 → RTV_DATA_LGLS.POS_H_RD(=EcsDefine <Position> plc값). Client가 m_MapRtvPosition[plc]→view 로 재매핑하므로
        //   트랙 N → RT#N → 해당 plc 를 써야 함(RT#03→3,05→5,07→6,09→8,11→9,13→11,15→12,17→13,19→14,21→15,23→2,31→10). 미매핑 트랙은 홈(0).
        private static readonly Dictionary<string, string> m_dicRgvCell = new Dictionary<string, string>()
        { { "121", "15" }, { "123", "2" }, { "130", "10" }, { "131", "10" },   // 131=130(C/V#15 입고대)의 RGV측 픽업트랙(TR#31=plc10)
          { "103", "3" }, { "107", "6" }, { "111", "9" }, { "115", "12" }, { "119", "14" },      // 입고 홀수라인(RT#03,07,11,15,19)
          { "101", "1" }, { "105", "5" }, { "109", "8" }, { "113", "11" }, { "117", "13" },      // 출고 홀수라인(RT#01,05,09,13,17) — RTV 픽업지점
          { "125", "4" }, { "126", "4" }, { "124", "2" }, { "129", "7" }, { "127", "7" } };       // 스테이션: 126출고대(C/V#13)=RT#25=plc4, 124입고대(C/V#12)=RT#23=plc2  [LGLS 2026-08-24 현장기준] C/V#12=입고(124) / C/V#13=출고(126), 129출고대(C/V#14)=RT#27=plc7, 127=129의 RGV 하역트랙(동일 plc7)
        private static string RgvCell(string track)
        {
            return m_dicRgvCell.ContainsKey(track) ? m_dicRgvCell[track] : "0";
        }

        // [LGLS] 라인 진입 가드.
        //   기본: 진입 트랙이 비었을 때만 진입(선행 화물 덮어쓰기=유실 방지).
        //   예외: S/C 1호기 라인(C/V#2 = 트랙 103/104)만 입고·출고 겸용(WorldModel HasDirection=(n==2)).
        //     실설비는 방향워드로 한 번에 한 방향만 쓰지만, 시뮬레이션에서 양방향 화물이 동시에 진입하면
        //     입고 화물(103 에서 104 대기)과 출고 화물(104 에서 103 대기)이 서로를 기다려 **교착**한다.
        //     → 겸용 라인은 두 트랙이 모두 비고 예약된 이동도 없을 때만 진입시켜 한 번에 한 방향만 쓰게 한다.
        private static readonly string[] SHARED_LINE_CV2 = { "103", "104" };
        // [LGLS 2026-08-30] C/V#11 도 방향전환형 겸용대다(입출고 겸용대, 트랙 121/122).
        private static readonly string[] DUAL_LINE_CV11 = { "121", "122" };

        /// <summary>
        /// [LGLS 2026-08-30] 그 크레인의 드롭 라인에서 S/C 픽업을 기다리는 입고 화물이 이미 있는가.
        ///   ★크레인 충돌 방지의 최종 기준★
        ///   기존 점유 판정(IsTrackFreeFor)은 CV_DATA 미러를 읽는데, 이 미러는 WCS_TASK_CV 의
        ///   15설비 순회 때문에 최대 ~16초 지연된다. RTV 가 화물을 내려놓아도 미러는 아직
        ///   '비어있음'이라, 그 창에서 다음 입고가 같은
        ///   드롭 트랙으로 또 지시돼 앞 화물 위로 겹친다(실측: 트랙 103 의 0119 위로 0117 진입).
        ///   JOB_MST 는 지연이 없다 — 라인에 내려진 입고는 SC 가 집어갈 때까지 20/21/25 에 머문다.
        ///   라인(103/104)은 CanEnterLine 규약상 이미 1파렛트 단위이므로 이 직렬화는 설계와 일치한다.
        /// </summary>
        private bool HasInboundWaitingOnScLine(string scNo, string exceptLugg)
        {
            try {
                if (string.IsNullOrEmpty(scNo)) return false;
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                        ";
                q += CRLF + "   FROM JOB_MST                                ";
                q += CRLF + "  WHERE WH_TYP      = :WH_TYP                  ";
                q += CRLF + "    AND JOB_TYP    IN ('1','11')               ";
                q += CRLF + "    AND DEST_POS    = :SC_NO                   ";
                q += CRLF + "    AND JOB_STATUS IN ('" + ST_SC_WAIT + "','" + ST_SC_RUN + "') ";
                q += CRLF + "    AND LUGG_NO    <> :LUGG                    ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')      ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("SC_NO",  DbLang.VARCHAR).Value = scNo;
                _pBdb.mComMain.Parameters.Add("LUGG",   DbLang.VARCHAR).Value = exceptLugg ?? "";
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }

        // ─────────────────────────────────────────────────────────────────
        // [LGLS 2026-09-05] 시간 기반 자동 처리 사용 여부
        //   운전 화면 리본 [환경설정] > [시간 기반 자동 처리] 버튼이 켜고 끈다.
        //   저장 위치 : COMMON_CODE (CDX_CD='SCH_OPT', CCD_CD='AUTO_TIME', CCD_CD_YN='Y'/'N')
        //   ★기본은 사용 안 함★ - 행이 없거나 조회에 실패하면 자동 처리하지 않는다.
        //   설비 신호 없이 경과시간으로 완료를 추정하는 처리는 모두 이 게이트를 통과해야 한다.
        // ─────────────────────────────────────────────────────────────────
        private bool     m_bAutoTimeProc = false;
        private DateTime m_dtAutoTimeRead = DateTime.MinValue;
        private const int AUTO_TIME_CACHE_SEC = 5;
        private bool AutoTimeProcEnabled()
        {
            try
            {
                if ((DateTime.Now - m_dtAutoTimeRead).TotalSeconds < AUTO_TIME_CACHE_SEC) return m_bAutoTimeProc;
                m_dtAutoTimeRead = DateTime.Now;
                string q = "";
                q += CRLF + " SELECT CCD_CD_YN FROM COMMON_CODE                                     ";
                q += CRLF + "  WHERE WH_TYP = :WH_TYP AND CDX_CD = 'SCH_OPT' AND CCD_CD = 'AUTO_TIME' ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) { m_bAutoTimeProc = false; return false; }
                bool bNew = ((GetVal(_pBdb.mDtMain.Rows[0], "CCD_CD_YN") ?? "").Trim().ToUpper() == "Y");
                if (bNew != m_bAutoTimeProc)
                    MakeMsg_Imp("[SCH] 시간 기반 자동 처리 " + (bNew ? "사용" : "사용 안 함") + " (운전 화면 설정 변경 감지)");
                m_bAutoTimeProc = bNew;
                return m_bAutoTimeProc;
            }
            catch { return m_bAutoTimeProc; }
        }

        /// <summary>[LGLS 2026-09-04] 도착트랙에 "RGV 완료(39) + HS_TRACK_NO = 트랙" 인 다른 작업이 있으면 그 작업번호, 없으면 "".</summary>
        private string RgvDoneNotLanded(string dropTrack, string exceptLugg)
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT LUGG_NO FROM JOB_MST                                   ";
                q += CRLF + "  WHERE WH_TYP = :WH_TYP AND JOB_STATUS = :ST_DONE               ";
                q += CRLF + "    AND HS_TRACK_NO = :TRK AND LUGG_NO <> :LUGG                  ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')                        ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_DONE", DbLang.VARCHAR).Value = ST_RGV_DONE;
                _pBdb.mComMain.Parameters.Add("TRK",     DbLang.VARCHAR).Value = dropTrack ?? "";
                _pBdb.mComMain.Parameters.Add("LUGG",    DbLang.VARCHAR).Value = exceptLugg ?? "";
                if (DbQry(q) <= 0) return "";
                return GetVal(_pBdb.mDtMain.Rows[0], "LUGG_NO");
            }
            catch { return ""; }
        }

        /// <summary>
        /// [LGLS 2026-09-04] RGV 도착트랙에 화물(재하 ON)은 있는데 트래킹이 비어 있으면 작업번호 기록을 요청한다.
        ///   LUGG_NO_OD + TRACKING_WRITE_YN='Y' → WCS_TASK_CV 가 PLC R영역에 쓴다(CV 구동지시 OD_RQ_YN 은 건드리지 않음).
        ///   요청을 냈으면 true.
        /// </summary>
        private bool RequestArrivalTrackingWrite(string track, string luggNo)
        {
            try
            {
                string q = "";
                q += CRLF + " UPDATE CV_DATA SET LUGG_NO_OD = :LUGG, TRACKING_WRITE_YN = 'Y' ";
                q += CRLF + "  WHERE WH_TYP = :WH_TYP AND MC_NO = :TRK                        ";
                q += CRLF + "    AND SENSOR0_DATA_RD = '1'                                   ";
                q += CRLF + "    AND (LUGG_NO_RD IS NULL OR LTRIM(RTRIM(LUGG_NO_RD)) IN ('','0','0000')) ";
                q += CRLF + "    AND (TRACKING_WRITE_YN IS NULL OR TRACKING_WRITE_YN <> 'Y')  ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("LUGG",   DbLang.VARCHAR).Value = luggNo;
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("TRK",    DbLang.VARCHAR).Value = track;
                return DbNonQry(q) > 0;
            }
            catch { return false; }
        }

        /// <summary>트랙의 현재 작업번호(LUGG_NO_RD). 없으면 "".</summary>
        private string TrackLugg(string track)
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT LUGG_NO_RD          ";
                q += CRLF + "   FROM CV_DATA             ";
                q += CRLF + "  WHERE WH_TYP     = :WH    ";
                q += CRLF + "    AND MC_NO      = :MC    ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC", DbLang.VARCHAR).Value = track;
                if (DbQry(q) <= 0) return "";
                return GetVal(_pBdb.mDtMain.Rows[0], "LUGG_NO_RD");
            }
            catch { return ""; }
        }

        /// <summary>
        /// 트랙이 비었거나 **자기 화물**이 놓여 있으면 진입 가능.
        /// 자기 화물을 충돌로 보면, 하역 시퀀스가 그 트랙에 올려놓은 화물 때문에 자기가 막혀
        /// 완료 처리(다음 트랙 트래킹 등록)에 도달하지 못한다. 재기동 후 복구도 이 판정에 의존.
        /// </summary>
        private bool IsTrackFreeFor(string track, string lugg)
        {
            if (IsTrackEmpty(track)) return true;
            if (string.IsNullOrEmpty(lugg)) return false;
            return TrackLugg(track) == Cap(lugg, 4);
        }



        /// <summary>
        /// [LGLS 2026-08-23] 보류된 모드 전환(DIRW) 승격.
        ///   겸용 입출고대는 출고 화물이 아직 그 자리에 있는 동안 입고 모드로 넘어가면
        ///   설비가 같은 자리를 입고대로 쓰기 시작한다. 그래서 HOST 의 '입고 복귀' 지시는
        ///   HOST_TASK 가 DIRW 로 남겨 두고, 여기서 작업대(예: 21/22)에 화물도 데이터도
        ///   없는 것을 확인한 뒤 DIR 로 승격해 실제 설비에 반영한다.
        /// </summary>
        private void PromotePendingDirection()
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT MC_NO, ISNULL(CMD_RQ_PARM,'0') AS CMD_RQ_PARM ";
                q += CRLF + "   FROM CV_DATA                                        ";
                q += CRLF + "  WHERE WH_TYP    = :WH_TYP                            ";
                q += CRLF + "    AND CMD_RQ_ID = 'DIRW'                             ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string mcNo = GetVal(dt.Rows[i], "MC_NO");
                    string dir  = GetVal(dt.Rows[i], "CMD_RQ_PARM");
                    if (string.IsNullOrEmpty(mcNo)) continue;

                    // 그 작업대와 짝이 되는 트랙(입출고대는 21/22 처럼 둘이 한 쌍)이 모두 비어야 한다.
                    string mate = RgvPickupTrack(mcNo);
                    bool bFree = IsTrackEmpty(mcNo) && IsTrackLuggEmpty(mcNo);
                    if (bFree && mate != mcNo)
                        bFree = IsTrackEmpty(mate) && IsTrackLuggEmpty(mate);

                    if (!bFree)
                    {
                        DbgLog("DIRW_" + mcNo, string.Format("[모드] 전환 대기 - 작업대 {0}/{1} 에 화물·데이터 남음", mcNo, mate));
                        continue;
                    }

                    string upd = "";
                    upd += CRLF + " UPDATE CV_DATA                              ";
                    upd += CRLF + "    SET CMD_RQ_ID = 'DIR'                    ";
                    upd += CRLF + "      , CMD_RQ_YN = 'Y'                      ";
                    upd += CRLF + "      , WRITE_UPD_DT = " + DbLang.SYSDATE + " ";
                    upd += CRLF + "  WHERE WH_TYP    = :WH_TYP                  ";
                    upd += CRLF + "    AND MC_NO     = :MC_NO                   ";
                    upd += CRLF + "    AND CMD_RQ_ID = 'DIRW'                   ";
                    _pBdb.mComMain.CommandType = CommandType.Text;
                    _pBdb.mComMain.Parameters.Clear();
                    _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                    _pBdb.mComMain.Parameters.Add("MC_NO",  DbLang.VARCHAR).Value = mcNo;
                    if (DbNonQry(upd) > 0)
                        MakeMsg_Imp(string.Format("[SCH][CV] 보류된 모드 전환 반영 - 작업대 {0} → {1} (작업대가 비었음)",
                            mcNo, (dir == "1") ? "출고(1)" : "입고(0)"));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][CV] PromotePendingDirection 오류: " + ex.Message); }
        }

        private bool CanEnterLine(string entryTrack) { return CanEnterLine(entryTrack, ""); }
        private bool CanEnterLine(string entryTrack, string lugg)
        {
            if (Array.IndexOf(SHARED_LINE_CV2, entryTrack) < 0)
                return IsTrackFreeFor(entryTrack, lugg);
            foreach (string t in SHARED_LINE_CV2)
                if (!IsTrackFreeFor(t, lugg) || m_dicCvMove.ContainsKey(t)) return false;
            return true;
        }

        // [LGLS] RGV 드롭 시 해당 라인 컨베이어 트랙에 트래킹을 기록하여 EQP_SIM 이 파렛트를 재현하게 한다.
        //   (LUGG_NO_OD + TRACKING_WRITE_YN='Y' → WCS_TASK_CV 가 PLC R영역에 기록 → EQP_SIM GetTracking → 입고 파렛트 재현)
        // [LGLS] 해당 CV 트랙이 비어있는지(sensor0=0) 확인 — 출고대 반출 경합 가드
        // ─────────────────────────────────────────────────────────────────
        // [LGLS 2026-07-19] 설비 정지 게이트 (대화상자 지시값 기반)
        //   SC  : SC_DATA_LGLS.SUSPEND   '1'=입고정지 '2'=출고정지 '3'=입출고정지 → 해당 방향 지시 금지
        //   RTV : RTV_DATA_LGLS.SUSPEND  '0' 외 = 작업정지 → RTV 작업지시 금지
        //   CV  : CV_DATA.TR_PAUSE_OD/RD '1' = 일시정지 — 원래는 PLC 비트 기록 용도지만
        //         본 WCS 에서는 **내부값(DB)만** 사용한다. 트랙 성격별로:
        //         RTV 작업대→RTV 지시 금지, SC 작업대→SC 지시 금지,
        //         입고대→입고 발행(PLC 기록) 금지, 출고대→도착완료 처리 금지.
        // ─────────────────────────────────────────────────────────────────
        /// <summary>SC 대화상자 suspend 값 ('0'/'1'/'2'/'3'. 미조회 시 '0')</summary>
        private string ScSuspend(string scNo)
        {
            try {
                string q = "";
                q += CRLF + " SELECT SUSPEND             ";
                q += CRLF + "   FROM SC_DATA_LGLS             ";
                q += CRLF + "  WHERE WH_TYP     = :WH    ";
                q += CRLF + "    AND SC_NO      = :NO    ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("NO", DbLang.VARCHAR).Value = scNo;
                if (DbQry(q) <= 0) return "0";
                string s = GetVal(_pBdb.mDtMain.Rows[0], "SUSPEND");
                return string.IsNullOrEmpty(s) ? "0" : s;
            } catch { return "0"; }
        }
        /// <summary>SC 가 해당 작업유형의 지시를 받을 수 있는지 (suspend 게이트)</summary>
        private bool ScSuspendAllows(string scNo, string jobTyp)
        {
            string s = ScSuspend(scNo);
            if (jobTyp == "1") return s != "1" && s != "3";
            if (jobTyp == "2") return s != "2" && s != "3";
            return true;
        }
        /// <summary>RTV 대화상자 작업정지 여부 (RTV_DATA_LGLS.SUSPEND ≠ '0')</summary>
        private bool IsRtvSuspended()
        {
            try {
                string q = "";
                q += CRLF + " SELECT SUSPEND             ";
                q += CRLF + "   FROM RTV_DATA_LGLS            ";
                q += CRLF + "  WHERE WH_TYP     = :WH    ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) return false;
                string s = GetVal(_pBdb.mDtMain.Rows[0], "SUSPEND");
                return !(string.IsNullOrEmpty(s) || s == "0");
            } catch { return false; }
        }
        /// <summary>CV 트랙 일시정지 여부 (TR_PAUSE_OD/RD='1' — PLC 비트 대신 내부값만 사용)</summary>
        /// <summary>
        /// [LGLS 2026-08-30] 해당 컨베이어가 에러 상태인가.
        ///   에러난 CV 는 움직일 수 없으므로, 그 CV 를 거치는 S/C · RTV 반송 지시도 내리면 안 된다
        ///   (지시해봐야 화물이 그 트랙에서 멈춰 라인을 막는다).
        ///   정상 표기는 여러 형태다 — 설비 실값은 CvThread 가 ToString("0000") 로 쓰는 '0000',
        ///   구 경로/초기값으로 '0','00',빈 값,NULL 도 있다. 전부 정상으로 본다.
        /// </summary>
        private bool IsCvError(string track)
        {
            try {
                if (string.IsNullOrEmpty(track)) return false;
                string q = "";
                q += CRLF + " SELECT ERROR_CODE          ";
                q += CRLF + "   FROM CV_DATA             ";
                q += CRLF + "  WHERE WH_TYP     = :WH    ";
                q += CRLF + "    AND MC_NO      = :MC    ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC", DbLang.VARCHAR).Value = track;
                if (DbQry(q) <= 0) return false;
                string ec = (GetVal(_pBdb.mDtMain.Rows[0], "ERROR_CODE") ?? "").Trim();
                if (ec.Length == 0) return false;
                int n;
                if (!int.TryParse(ec, out n)) return true;   // 숫자가 아니면 판단 불가 → 보수적으로 에러 취급
                return n != 0;
            } catch { return false; }
        }

        private bool IsCvPaused(string track)
        {
            try {
                string q = "";
                q += CRLF + " SELECT TR_PAUSE_OD, TR_PAUSE_RD ";
                q += CRLF + "   FROM CV_DATA             ";
                q += CRLF + "  WHERE WH_TYP     = :WH    ";
                q += CRLF + "    AND MC_NO      = :MC    ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC", DbLang.VARCHAR).Value = track;
                if (DbQry(q) <= 0) return false;
                return GetVal(_pBdb.mDtMain.Rows[0], "TR_PAUSE_OD") == "1" ||
                       GetVal(_pBdb.mDtMain.Rows[0], "TR_PAUSE_RD") == "1";
            } catch { return false; }
        }

        // ─────────────────────────────────────────────────────────────────
        // [LGLS 2026-07-19] RTV 상호배타 : RTV 는 물리 1대 — 입고 RGV 처리(AutoRunRGV)와
        //   출고대 반출 시퀀스(ProcessOutStn)가 조율 없이 동시 점유하면 색(입고↔출고) 깜빡임,
        //   동시 픽업/동시 하역이 화면에 나타난다. 한 번에 한 동작만 하도록 서로 시작을 보류한다.
        // ─────────────────────────────────────────────────────────────────
        /// <summary>출고대 반출 시퀀스가 RTV 를 점유 중인지 (Stage 0~2 = 빈차이동/적재/하역. Stage 3~4 는 컨베이어만)</summary>
        private bool RtvBusyByOutbound()
        {
            foreach (var kv in m_dicOutStn) if (kv.Value.Stage <= 2) return true;
            return false;
        }
        // [LGLS 2026-07-20] SC1 특례 상호배타 — SC1 은 입고 드롭 트랙과 출고 반출 트랙이 같은 라인을 공유하므로
        //   ① SC1 출고 미종결 존재 시 SC1행 입고의 RTV 반송 보류(출고 우선), ② SC1행 입고가 RTV 지시/운반 중이면 SC1 출고 지시 보류.
        /// <summary>SC1(901) 출고 작업(자동 2/반자동 12)이 미종결 상태로 존재하는지</summary>
        private bool HasActiveSc1Outbound()
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                    ";
                q += CRLF + "   FROM JOB_MST                            ";
                q += CRLF + "  WHERE WH_TYP      = :WH_TYP              ";
                q += CRLF + "    AND JOB_TYP    IN ('2','12')           ";
                q += CRLF + "    AND START_POS   = '901'                ";
                // [LGLS 2026-08-30] ★기아(starvation) 방지★ — "미완료 출고가 하나라도 있으면" 이 아니라
                //   "출고가 실제로 공유 라인(C/V#2 = 103/104)을 쓰고 있으면" 으로 좁힌다.
                //   종전에는 NOT IN ('09','19','29') 라 **아직 지시도 안 나간 대기(20)** 까지 세어서,
                //   출고가 줄만 서 있어도 901행 입고 RTV 가 막혔다. 출고가 끊이지 않는 현장/시험에서는
                //   입고가 영원히 굶는다(실측: [CV#2 교착 TEST] 중 입고 0083/0098 이 30 에서 18분 정지,
                //   그때 RTV 는 완전 유휴였고 막은 출고 3건 중 2건은 상태 20 이었다).
                //   20 = 대기(아무것도 점유하지 않음) → 양보 사유 아님. 21 부터가 실제 점유.
                q += CRLF + "    AND JOB_STATUS NOT IN ('09','19','29','" + ST_SC_WAIT + "') ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')  ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }

        /// <summary>SC1행(DEST 901) 입고 작업(자동 1/반자동 11)이 RTV 에 지시/운반 중(31/35)인지</summary>
        private bool HasSc1InboundOnRtv()
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                    ";
                q += CRLF + "   FROM JOB_MST                            ";
                q += CRLF + "  WHERE WH_TYP      = :WH_TYP              ";
                q += CRLF + "    AND JOB_TYP    IN ('1','11')           ";
                q += CRLF + "    AND DEST_POS    = '901'                ";
                // [LGLS 2026-08-30] 구동대기(30)는 제외한다. ★교착 방지 규칙의 핵심★
                //   SC1 특례는 "출고 우선" 한 방향 우선권이어야 한다. 그런데 종전에는
                //     · DriveRGV  : 901행 입고 RTV 지시는 SC1 출고가 미종결이면 보류(HasActiveSc1Outbound)
                //     · DriveSC   : SC1 출고 지시는 입고가 30/31/35 이면 보류(이 함수)
                //   양쪽이 서로에게 양보해 순환 대기가 된다. [CV#2 교착 TEST] 18건에서 실제로
                //   입고 2건(30) ↔ 출고 9건(20) 이 5분 이상 완전 정지하는 것을 확인했다.
                //   30 = "픽업트랙에서 RTV 를 기다리는 중"일 뿐 RTV 가 아직 잡지 않은 상태라
                //   양보 사유가 되지 않는다. 실제 라인 점유는 31/35 부터이고, 하역트랙 물리 점유는
                //   DriveSC 의 !IsTrackEmpty(_wT) 가 따로 막는다.
                //   ~~[LGLS 2026-08-04] '받았으면' = 구동대기(30)부터 포함~~ (교착 유발로 철회)
                q += CRLF + "    AND JOB_STATUS IN ('" + ST_RGV_RUN + "') ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')  ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }
        /// <summary>
        /// [LGLS 2026-08-04] 지정 트랙들에 '진행 중인 입고 작업'의 화물이 올라와 있는지.
        ///   SC1 은 입고 드롭과 출고 반출이 같은 라인(C/V#2 = 103/104)을 쓰므로,
        ///   그 라인에 입고 화물이 남아 있으면 출고를 내보내면 안 된다(정면 충돌).
        /// </summary>
        private bool HasInboundCargoOnTracks(string tracksCsvQuoted)
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                                   ";
                q += CRLF + "   FROM CV_DATA CD                                        ";
                q += CRLF + "  INNER JOIN JOB_MST JM ON JM.WH_TYP  = CD.WH_TYP         ";
                q += CRLF + "                       AND JM.LUGG_NO = CD.LUGG_NO_RD     ";
                q += CRLF + "  WHERE CD.WH_TYP    = :WH_TYP                            ";
                q += CRLF + "    AND CD.MC_NO    IN (" + tracksCsvQuoted + ")          ";
                q += CRLF + "    AND JM.JOB_TYP  IN ('1','11')                         ";
                q += CRLF + "    AND JM.JOB_STATUS NOT IN ('09','19','29')              ";
                q += CRLF + "    AND (JM.DEL_YN IS NULL OR JM.DEL_YN <> 'Y')           ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }

        /// <summary>
        /// [LGLS 2026-08-04] 출고 H/S 라인(짝수=SC 하역, 홀수=RTV 픽업) 중 어느 한쪽이라도 화물이 감지되는지.
        ///   기존에는 짝수 트랙만 봐서, 홀수쪽에 선행 화물이 남아 있어도 SC 출고 지시가 나갔다.
        /// </summary>
        private bool IsOutHsOccupied(string evenTrack)
        {
            if (!IsTrackEmpty(evenTrack)) return true;
            int n;
            if (int.TryParse(evenTrack, out n) && n > 1)
            {
                string odd = (n - 1).ToString();
                if (!IsTrackEmpty(odd)) return true;
            }
            return false;
        }

        // [LGLS 2026-07-19] 작업번호가 진행 중인 입고 작업인지 (좌초화물 복구의 오인 방지용)
        /// <summary>
        /// [LGLS 2026-08-23] 출고 화물의 도착지(출고대)를 JOB_MST 에서 되찾는다.
        ///   트랙의 DEST_POS_OD 가 비어 있을 때 쓰던 "122" 고정 fallback 은, 실제 도착지가 124/129 인 화물을
        ///   엉뚱한 출고대로 보내버린다(작업 1663 = 도착지 124 인데 122 로 복구될 뻔함).
        ///   지시의 원본인 JOB_MST.DEST_POS 를 우선 보고, 거기서도 못 얻으면 그때만 dflt 를 쓴다.
        /// </summary>
        private string JobOutDestPos(string lugg, string dflt)
        {
            try {
                string q = "";
                q += CRLF + " SELECT DEST_POS                            ";
                q += CRLF + "   FROM JOB_MST                             ";
                q += CRLF + "  WHERE WH_TYP      = :WH_TYP               ";
                q += CRLF + "    AND LUGG_NO     = :LG                   ";
                q += CRLF + "    AND JOB_TYP    IN ('2','12')            ";
                q += CRLF + "    AND JOB_STATUS NOT IN ('09','19','29')   ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')   ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LG",     DbLang.VARCHAR).Value = lugg;
                if (DbQry(q) > 0)
                {
                    string dp = GetVal(_pBdb.mDtMain.Rows[0], "DEST_POS");
                    // 출고대만 인정 - 로직3=122 / 로직1=126 / 로직2(피킹)=129
                    //   [LGLS 2026-08-28 현장기준] C/V#12(124)=입고대, C/V#13(126)=출고대 → 124 는 더 이상 출고대가 아니다
                    if (dp == "122" || dp == "126" || dp == "129") return dp;
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][OUT] JobOutDestPos 오류: " + ex.Message); }
            return dflt;
        }

        /// <summary>
        /// [LGLS 2026-08-28] 해당 작업번호가 **진행 중인 출고 작업**으로 JOB_MST 에 살아 있는지.
        ///   RecoverOutOrphans 는 CV_DATA 의 잔류 트래킹(LUGG_NO_RD)만 보고 RTV 반출을 지시했다.
        ///   그래서 이미 삭제/완료된 작업의 잔류값(WCS_TASK_CV 가 아직 동기화하지 못한 기동 직후의
        ///   낡은 readback 포함)으로 **유령 반송지시**가 나가, 설비 RGV 가 빈 트랙에서 영원히 대기하고
        ///   SUBSYSTEM_STATUS 가 IDLE 로 돌아오지 않아 그 뒤의 모든 입고가 '30' 에서 멈췄다.
        /// </summary>
        private bool IsActiveOutboundJob(string lugg)
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                    ";
                q += CRLF + "   FROM JOB_MST                            ";
                q += CRLF + "  WHERE WH_TYP      = :WH_TYP              ";
                q += CRLF + "    AND LUGG_NO     = :LG                  ";
                q += CRLF + "    AND JOB_TYP    IN ('2','12')           ";
                q += CRLF + "    AND JOB_STATUS NOT IN ('09','19','29')  ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')  ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LG",     DbLang.VARCHAR).Value = lugg;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }

        private bool IsActiveInboundJob(string lugg)
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                    ";
                q += CRLF + "   FROM JOB_MST                            ";
                q += CRLF + "  WHERE WH_TYP      = :WH_TYP              ";
                q += CRLF + "    AND LUGG_NO     = :LG                  ";
                q += CRLF + "    AND JOB_TYP    IN ('1','11')           ";
                q += CRLF + "    AND JOB_STATUS NOT IN ('09','19','29')  ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')  ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LG",     DbLang.VARCHAR).Value = lugg;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }

        /// <summary>[LGLS 2026-07-30] 해당 작업번호의 화물 트래킹이 전 트랙(CV_DATA) 어디엔가 존재하는지.
        ///   CompleteCV 출고 '도착 관측 누락' 보강의 안전장치 — 설비 이송 중이면 어느 트랙엔가 트래킹이 남으므로
        ///   운반 중 작업의 조기 완료를 막는다. (RV 적재 시퀀스로 CV 데이터가 잠시 비는 구간은 GRACE 가 흡수)</summary>
        private bool LuggOnAnyTrack(string lugg)
        {
            try {
                string q = "";
                // [LGLS 2026-08-23] 실화물이 얹혀 있는 트랙만 '이송 중' 으로 본다.
                //   출고대는 지게차가 화물을 걷어간 뒤에도 트래킹(LUGG_NO_RD)이 몇 초 더 남고,
                //   지시값이 남아 있으면 그 트래킹이 되살아나기도 한다. 그것까지 '존재' 로 세면
                //   아래 무관측 유예가 영원히 리셋되어 출고 작업이 '15' 에서 굳는다(작업 1401 사례).
                q += CRLF + " SELECT COUNT(*) AS CNT       ";
                q += CRLF + "   FROM CV_DATA               ";
                q += CRLF + "  WHERE WH_TYP     = :WH_TYP  ";
                q += CRLF + "    AND LUGG_NO_RD = :LUGG    ";
                q += CRLF + "    AND SENSOR0_DATA_RD = '1' ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LUGG",   DbLang.VARCHAR).Value = Cap(lugg, 4);
                if (DbQry(q) <= 0) return true;   // 조회 실패 시엔 보수적으로 '존재'(완료 보류)
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return true; }
        }

        // [LGLS 2026-08-01] 입출고 겸용 작업대인지 (CV_DATA.STN_KIND 비트: 1=입고, 2=출고 → 3=겸용).
        //   현장 22번(C/V#11)이 여기 해당. 특정 트랙번호를 하드코딩하지 않고 설비 정의로 판정한다.
        private bool IsDualStation(string mcNo)
        {
            try {
                string q = "";
                q += CRLF + " SELECT STN_KIND            ";
                q += CRLF + "   FROM CV_DATA             ";
                q += CRLF + "  WHERE WH_TYP = :WH_TYP    ";
                q += CRLF + "    AND MC_NO  = :MC_NO     ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC_NO",  DbLang.VARCHAR).Value = mcNo;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "STN_KIND"), out n);
                return (n & 3) == 3;
            } catch { return false; }
        }

        /// <summary>[LGLS 2026-08-01] 작업대 방향 모드: "0"=입고, "1"=출고.
        ///   ※CV_DATA.STOCK_MODE 에는 WCS_TASK_CV(CvStatusScenario)가 PLC 방향 워드의 **원시 워드값**을 넣는다.
        ///     그 워드에는 ASCII 문자가 실려 있어 실제 값이 48('0')/49('1')로 보인다 → 여기서 정규화한다.</summary>
        private string GetCvStockMode(string mcNo)
        {
            try {
                string q = "";
                q += CRLF + " SELECT STOCK_MODE          ";
                q += CRLF + "   FROM CV_DATA             ";
                q += CRLF + "  WHERE WH_TYP = :WH_TYP    ";
                q += CRLF + "    AND MC_NO  = :MC_NO     ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC_NO",  DbLang.VARCHAR).Value = mcNo;
                if (DbQry(q) <= 0) return "0";
                string v = (GetVal(_pBdb.mDtMain.Rows[0], "STOCK_MODE") ?? "").Trim();
                if (v == "1" || v == "49") return "1";
                return "0";
            } catch { return "0"; }
        }

        /// <summary>[LGLS 2026-08-01] 해당 작업대에서 출발하는 입고 작업이 진행 중인지
        ///   (원본 ECS TransferDetail.getCheckInTransferFromCNV11 대응).</summary>
        /// <summary>
        /// [LGLS 2026-08-30] 그 작업대로 향하는 출고 반송이 진행 중인가.
        ///   SC 구동대기(20)는 아직 크레인이 집지도 않은 상태라 '진행 중'이 아니다.
        ///   21 부터가 실제로 화물이 그 작업대를 향해 움직이는 구간이다.
        /// </summary>
        private bool HasActiveOutboundTo(string station)
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                        ";
                q += CRLF + "   FROM JOB_MST                                ";
                q += CRLF + "  WHERE WH_TYP      = :WH_TYP                  ";
                q += CRLF + "    AND JOB_TYP    IN ('2','12')               ";
                q += CRLF + "    AND DEST_POS    = :STN                     ";
                q += CRLF + "    AND JOB_STATUS NOT IN ('09','19','29','" + ST_SC_WAIT + "') ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')      ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("STN",    DbLang.VARCHAR).Value = station;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }

        private bool HasActiveInboundFrom(string station)
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                    ";
                q += CRLF + "   FROM JOB_MST                            ";
                q += CRLF + "  WHERE WH_TYP      = :WH_TYP              ";
                q += CRLF + "    AND JOB_TYP    IN ('1','11')           ";
                q += CRLF + "    AND START_POS   = :STN                 ";
                // [LGLS 2026-08-30] ★기아 방지★ — SC1 특례와 같은 실수가 여기에도 있었다.
                //   "그 작업대발 입고가 하나라도 미종결이면" 출고를 보류하면, 입고가 끊이지 않는 한
                //   출고가 영원히 'CV 구동대기(10)' 에 남는다(실측: 122 발 입고 6건이 10 에 쌓여
                //   122 행 출고가 계속 보류됨).
                //   CV 구동대기(10) = 아직 그 작업대에서 출발도 하지 않은 상태다. 실제로 작업대를
                //   점유했는지는 바로 위 물리 가드(입고 모드 + 입고대 점유)가 이미 본다.
                //   따라서 여기서는 **실제로 반송이 시작된 입고(11 이후)** 만 센다.
                q += CRLF + "    AND JOB_STATUS NOT IN ('09','19','29','" + ST_CV_WAIT + "') ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')  ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("STN",    DbLang.VARCHAR).Value = station;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }

        // [LGLS 2026-08-01] 겸용대 방향 전환 지시. CV_DATA 커맨드 채널에 남기면
        //   WCS_TASK_CV(CvChg_CMD_RQ_YN, CMD_RQ_ID='DIR')가 설비 방향 워드에 기록한다(HOST M 전문과 동일 경로).
        //   dir : "0"=입고, "1"=출고
        // [LGLS 2026-08-22] S/C #1 전용 겸용 통로 C/V#2 : 입고·출고가 트랙 103/104 를 공유한다.
        //   방향 지시는 대표 트랙 103 으로 낸다(설비 단위 방향 워드 - 103/104 가 함께 바뀐다).
        private const string SC1_NO       = "901";
        private const string SC1_DUAL_CV  = "103";

        private readonly Dictionary<string, DateTime> m_dicDirReqAt = new Dictionary<string, DateTime>();
        private const int DIR_REQ_HOLD_MS = 15000;   // 설비 반영(미러 1주기)까지 재지시 억제

        /// <summary>
        /// [LGLS 2026-08-30] 방향전환형 겸용대(C/V#2 = 103/104, C/V#11 = 121/122)가
        /// "작업 화물"을 아직 붙들고 있는가.
        ///   ★사용자 정정★ 작업번호 없는 화물은 입고 화물이 아니다 — 작업자가 올려두었을 뿐
        ///   지시가 없는 파렛트나 자동투입 잔재(유령 파렛트)는 방향 전환을 막지 않는다.
        ///   막아야 하는 것은 시스템이 반송 중인 화물, 즉 작업번호가 붙은 화물뿐이다.
        /// </summary>
        private bool IsDualCvBusyWithJob(string mcNo, string wantDir)
        {
            string[] tracks = (mcNo == SC1_DUAL_CV || mcNo == "104") ? SHARED_LINE_CV2
                            : (mcNo == "121" || mcNo == "122")       ? DUAL_LINE_CV11
                            : null;
            if (tracks == null) tracks = new string[] { mcNo };
            foreach (string t in tracks)
            {
                string lugg = (TrackLugg(t) ?? "").Trim();
                if (lugg.Length == 0 || lugg == "0" || lugg == "0000") continue;   // 작업번호 없는 화물은 대상 아님
                // [LGLS 2026-08-30] ★반대 방향 화물만 전환을 막는다★
                //   요청이 출고(1)면 "그 작업대발 입고 화물"이, 요청이 입고(0)면 "그 작업대로 오는 출고 화물"이
                //   아직 위에 있을 때만 보류한다. 같은 방향 화물(예: 출고 전환을 기다리는 출고 화물)까지
                //   막으면 그 화물이 영영 못 빠지는 교착이 된다.
                if (IsOppositeDirCargo(mcNo, lugg, wantDir)) return true;
            }
            return false;
        }

        /// <summary>
        /// [LGLS 2026-08-30] 그 작업대 위의 화물이 "요청 방향과 반대 방향" 화물인가.
        ///   입고 화물 = 그 작업대에서 출발하는 입고 작업(START_POS=작업대)
        ///   출고 화물 = 그 작업대로 도착하는 출고 작업(DEST_POS=작업대)
        /// </summary>
        private bool IsOppositeDirCargo(string mcNo, string lugg, string wantDir)
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                        ";
                q += CRLF + "   FROM JOB_MST                                ";
                q += CRLF + "  WHERE WH_TYP   = :WH_TYP                     ";
                q += CRLF + "    AND LUGG_NO  = :LUGG                       ";
                // [LGLS 2026-08-31] ★겸용 통로는 작업대 기준으로 비교한다★
                //   SC#1 통로(103/104)의 입고는 START_POS 가 입고대(124)라 통로 번호와
                //   비교하면 절대 걸리지 않았다 - 진행 중인 입고 화물 위에서 방향이 뒤집혔다.
                //   입고는 "그 작업대로 가는(DEST) 화물", 출고는 "그 작업대발(START) 화물" 로 본다.
                string stn = (mcNo == SC1_DUAL_CV || mcNo == "104") ? "901" : mcNo;
                // [LGLS 2026-08-31] ★상태 제외는 방향별이다★ (실측 : 9004 오집 사건)
                //   입고 29 = 최종(랙 저장 끝) 이지만 ★출고 29 = 크레인이 작업대에 방금
                //   내려놓은 진행 단계★ 다. 29 를 일괄 제외했더니 출고 완료 3초 뒤
                //   방향이 입고로 뒤집혔고, 크레인이 그 출고 화물(9004)을 다음 입고
                //   작업(9003)의 화물로 오집했다.
                if (wantDir == "1")   // 출고로 바꾸려 한다 → 입고 화물이 남아 있으면 보류
                {
                    q += CRLF + "    AND JOB_TYP IN ('1','11') AND (START_POS = :STN OR DEST_POS = :STN) ";
                    q += CRLF + "    AND JOB_STATUS NOT IN ('09','29')       ";   // 입고 29 = 최종
                }
                else                  // 입고로 바꾸려 한다 → 출고 화물이 남아 있으면 보류
                {
                    q += CRLF + "    AND JOB_TYP IN ('2','12') AND (DEST_POS  = :STN OR START_POS = :STN) ";
                    q += CRLF + "    AND JOB_STATUS NOT IN ('09','19')       ";   // 출고 19 = 최종, 29 는 보호
                }
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LUGG",   DbLang.VARCHAR).Value = lugg;
                _pBdb.mComMain.Parameters.Add("STN",    DbLang.VARCHAR).Value = stn;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }

        private bool RequestCvDirection(string mcNo, string dir)
        {
            try
            {
                DateTime last;
                if (m_dicDirReqAt.TryGetValue(mcNo, out last) &&
                    (DateTime.Now - last).TotalMilliseconds < DIR_REQ_HOLD_MS)
                    return false;      // 직전 지시가 아직 반영 중 — 중복 지시 억제

                // [LGLS 2026-08-30] 겸용대 방향 전환 규약(사용자 확정):
                //   현재 방향의 작업 화물이 아직 설비에 있으면 전환하지 않는다. 뒤집으면 이송 방향이
                //   반대가 되어 그 화물이 갇히고(HS 미성립) 크레인 앞에서 충돌한다.
                //   화물이 빠지면 다음 폴링에 자연히 전환된다(호출부가 매 주기 재시도).
                if (IsDualCvBusyWithJob(mcNo, dir))
                {
                    DbgLog("DIRHOLD_" + mcNo, string.Format("[CV] 방향전환 보류 - 겸용대 {0} 에 작업 화물이 남아 있음", mcNo));
                    return false;
                }

                string q = "";
                q += CRLF + " UPDATE CV_DATA                              ";
                q += CRLF + "    SET CMD_RQ_ID    = 'DIR'                 ";
                q += CRLF + "      , CMD_RQ_PARM  = :DIR                  ";
                q += CRLF + "      , CMD_RQ_YN    = 'Y'                   ";
                q += CRLF + "      , WRITE_UPD_DT = " + DbLang.SYSDATE + " ";
                q += CRLF + "  WHERE WH_TYP       = :WH_TYP               ";
                q += CRLF + "    AND MC_NO        = :MC_NO                ";
                // 다른 커맨드가 대기 중이면 덮어쓰지 않는다(방향 지시는 다음 폴링에 재시도)
                q += CRLF + "    AND (CMD_RQ_YN <> 'Y' OR CMD_RQ_ID = 'DIR') ";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("DIR",    DbLang.VARCHAR).Value = dir;
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC_NO",  DbLang.VARCHAR).Value = mcNo;
                if (DbNonQry(q) <= 0) return false;

                m_dicDirReqAt[mcNo] = DateTime.Now;
                return true;
            }
            catch (Exception ex)
            {
                MakeMsg_Error("[SCH][CV] RequestCvDirection 오류: " + ex.Message);
                return false;
            }
        }

        /// <summary>[LGLS 2026-08-01] 트랙 트래킹(작업번호)이 비었는지 — 화물 센서와 별개로 예약/잔류 트래킹까지 본다.</summary>
        private bool IsTrackLuggEmpty(string track)
        {
            string v = (TrackLugg(track) ?? "").Trim();
            return v.Length == 0 || v == "0" || v == "0000";
        }

        // [LGLS 2026-08-22] 핸드셰이크 게이트 (사용자 최우선 규칙)
        //   SC 입고 : 입고 HS(STOHS_READY_RD) 가 없으면 입고 지시 금지
        //   SC 출고 : 출고 HS(RETHS_READY_RD) 가 없으면 출고 지시 금지
        //   RGV     : 출발지(RTV_DEPARTHS_READY_RD)·도착지(RTV_ARRIVEHS_READY_RD) 가 모두 있어야 지시
        //   신호는 WCS_TASK_CV 가 PLC 를 읽어 CV_DATA 에 기록한다.
        //   조회 실패·행 없음은 "신호 없음"으로 본다 - 모르면 지시하지 않는 쪽이 안전하다.
        private bool IsHsOn(string track, string column)
        {
            if (string.IsNullOrEmpty(track) || string.IsNullOrEmpty(column)) return false;
            try {
                string q = "";
                q += CRLF + " SELECT " + column + " AS HS_VAL  ";
                q += CRLF + "   FROM CV_DATA                   ";
                q += CRLF + "  WHERE WH_TYP     = :WH_TYP      ";
                q += CRLF + "    AND MC_NO      = :MC_NO       ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC_NO",  DbLang.VARCHAR).Value = track;
                if (DbQry(q) <= 0) return false;
                return GetVal(_pBdb.mDtMain.Rows[0], "HS_VAL") == "1";
            } catch { return false; }
        }

        private bool IsTrackEmpty(string track)
        {
            try {
                string q = "";
                q += CRLF + " SELECT SENSOR0_DATA_RD     ";
                q += CRLF + "   FROM CV_DATA             ";
                q += CRLF + "  WHERE WH_TYP     = :WH_TYP ";
                q += CRLF + "    AND MC_NO      = :MC_NO  ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC_NO",  DbLang.VARCHAR).Value = track;
                if (DbQry(q) <= 0) return true;
                return GetVal(_pBdb.mDtMain.Rows[0], "SENSOR0_DATA_RD") != "1";
            } catch { return false; }
        }
        #region [LGLS] RV(SC/RTV 공통) 적재·하역 4단계 시퀀스
        // 사용자 요구 동작(각 단계 2초). RV = SC·RTV 통칭.
        //  [적재] RV 도착(데이터만, 화물감지 X) →2s→ RV 에 화물 표시 →2s→ CV 의 화물·데이터 제거 →2s→ RV 출발
        //  [하역] RV 도착(데이터+화물감지) →2s→ RV 화물 사라짐 →2s→ CV 에 화물만 생김(데이터 X)
        //         →2s→ RV 데이터 사라지고 CV 에 데이터 생김 → 다음 트랙 이송
        // 호출자는 RvSeqStep() 이 true 를 돌려줄 때까지 그 단계에 머문다(false 면 continue).
        private const int RV_STEP_MS = 2000;
        private class RvSeq
        {
            public DateTime Due; public int Phase; public bool IsLoad;
            public string Kind = "R";   // "R"=RTV, "S"=SC
            public string No = "";      // RTV_NO / SC_NO
            public string Track = "";   // 상대 CV 트랙
            public string Lugg = "";
            public string JobTyp = "1";
            public string Dest = "";    // [LGLS] 출고 화물의 도착지(출고대) — 트랙에 함께 기록해 재기동 복구 가능하게
        }
        private readonly Dictionary<string, RvSeq> m_dicRvSeq = new Dictionary<string, RvSeq>();
        private readonly HashSet<string> m_setRvSeqDone = new HashSet<string>();

        /// <summary>적재/하역 시퀀스 진행. 완료=true(호출자 진행), 진행중=false(호출자 대기).</summary>
        private bool RvSeqStep(string key, bool isLoad, string kind, string no, string track, string lugg, string jobTyp, string dest = "")
        {
            if (m_setRvSeqDone.Contains(key)) return true;
            if (!m_dicRvSeq.ContainsKey(key))
                m_dicRvSeq[key] = new RvSeq { Due = DateTime.Now.AddMilliseconds(RV_STEP_MS), Phase = 0,
                                              IsLoad = isLoad, Kind = kind, No = no, Track = track, Lugg = lugg, JobTyp = jobTyp, Dest = dest };
            return false;
        }
        /// <summary>시퀀스가 아직 시작 전인지(= 도착 상태를 1회만 세팅하기 위한 판정).</summary>
        private bool RvSeqFresh(string key) { return !m_dicRvSeq.ContainsKey(key) && !m_setRvSeqDone.Contains(key); }
        private void RvSeqReset(string key) { m_dicRvSeq.Remove(key); m_setRvSeqDone.Remove(key); }


        /// <summary>RV 화물감지만 토글. 데이터/위치/이동목표는 건드리지 않는다.</summary>
        private void SetRvCargo(string kind, string no, string on)
        {
            try
            {
                string s = "";
                if (kind == "R")
                {
                    s += CRLF + " UPDATE RTV_DATA_LGLS               ";
                    s += CRLF + "    SET SENSOR_RTV_RD  = :S    ";
                    s += CRLF + "  WHERE WH_TYP         = :WH   ";
                    s += CRLF + "    AND RTV_NO         = :NO   ";
                }
                else
                {
                    s += CRLF + " UPDATE SC_DATA_LGLS                ";
                    s += CRLF + "    SET SENSOR_FK_RD   = :S    ";
                    s += CRLF + "  WHERE WH_TYP         = :WH   ";
                    s += CRLF + "    AND SC_NO          = :NO   ";
                }
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("S",  DbLang.VARCHAR).Value = on;
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("NO", DbLang.VARCHAR).Value = no;
                DbNonQry(s);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][RV] SetRvCargo 오류: " + ex.Message); }
        }

        /// <summary>RV 데이터(작업번호/작업색)만 제거. 화물감지는 이미 0 인 상태.</summary>
        private void ClearRvData(string kind, string no)
        {
            try
            {
                string s = "";
                if (kind == "R")
                {
                    s += CRLF + " UPDATE RTV_DATA_LGLS                       ";
                    s += CRLF + "    SET LUGG_OD              = '0',    ";
                    s += CRLF + "        JOB_TYP_OD           = '0',    ";
                    s += CRLF + "        PALLET_ON_VEHICLE_RD = '0'     ";
                    s += CRLF + "  WHERE WH_TYP               = :WH     ";
                    s += CRLF + "    AND RTV_NO               = :NO     ";
                }
                else
                {
                    s += CRLF + " UPDATE SC_DATA_LGLS                        ";
                    s += CRLF + "    SET ITN_LUGG_FK1         = '0',    ";
                    s += CRLF + "        JOB_TYP_RD           = '0',    ";
                    s += CRLF + "        PALLET_ON_VEHICLE_RD = '0'     ";
                    s += CRLF + "  WHERE WH_TYP               = :WH     ";
                    s += CRLF + "    AND SC_NO                = :NO     ";
                }
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("NO", DbLang.VARCHAR).Value = no;
                DbNonQry(s);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][RV] ClearRvData 오류: " + ex.Message); }
        }

        /// <summary>CV 트랙에 '화물만' 표시(데이터 없음). 하역 3단계에서 잠깐 쓰는 과도상태.</summary>
        private void WriteCvCargoOnly(string track)
        {
            try
            {
                string s = "";
                s += CRLF + " UPDATE CV_DATA SET SENSOR0_DATA_RD = '1', LUGG_NO_RD = '0000', JOB_TYP_RD = '0' ";
                s += CRLF + "      , AUTO_MODE_RD = '1', READ_UPD_DT = " + DbLang.SYSDATE;
                s += CRLF + "  WHERE WH_TYP = :WH AND MC_NO = :MC ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC", DbLang.VARCHAR).Value = track;
                DbNonQry(s);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][RV] WriteCvCargoOnly 오류: " + ex.Message); }
        }
        #endregion

        // [LGLS] SCH(IO_TASK) 헬스 하트비트 → EQP_MST(EQP_TYP='SCH', PLC_NO='01').
        //   IO_TASK 는 원격지 서버에서 돌 수 있어 WCS Client 가 OS 프로세스를 직접 확인할 수 없다.
        //   → 스케줄러가 한 사이클을 정상 완주할 때마다 UPD_DT/CONNECTED_YN='Y' 를 갱신하고,
        //     예외가 나면 CONNECTED_YN='N' 을 즉시 기록한다.
        //     Client(상태표시줄 SCH)는 이 행의 UPD_DT 신선도 + CONNECTED_YN 으로 판정하므로
        //     **프로세스가 죽거나 멈추면 UPD_DT 가 낡아 자동으로 RED** 가 된다(별도 통보 불필요).
        private DateTime m_dtLastHb = DateTime.MinValue;
        private const int HB_INTERVAL_MS = 2000;
        private void Heartbeat(bool healthy)
        {
            try
            {
                if (_pBdb == null) return;
                DateTime now = DateTime.Now;
                // 정상일 때만 주기 제한(200ms 루프마다 쓰면 낭비). 오류는 즉시 반영해 RED 가 빨리 뜨게 한다.
                if (healthy && (now - m_dtLastHb).TotalMilliseconds < HB_INTERVAL_MS) return;
                m_dtLastHb = now;

                string s = "";
                s += CRLF + " UPDATE EQP_MST SET CONNECTED_YN = :CY, UPD_DT = " + DbLang.SYSDATE;
                s += CRLF + "  WHERE WH_TYP = :WH AND EQP_TYP = 'SCH' AND PLC_NO = '01' ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("CY", DbLang.VARCHAR).Value = healthy ? "Y" : "N";
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                int n = DbNonQry(s);
                if (n == 0)
                {
                    // 행이 없으면 최초 1회 생성(시드 누락 대비)
                    string ins = "";
                    ins += CRLF + " INSERT INTO EQP_MST (WH_TYP, EQP_TYP, PLC_NO, CONNECTED_YN, UPD_DT, USE_YN, REMARKS) ";
                    ins += CRLF + " VALUES (:WH, 'SCH', '01', :CY, " + DbLang.SYSDATE + ", 'Y', 'IO_TASK Scheduler') ";
                    _pBdb.mComMain.Parameters.Clear();
                    _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                    _pBdb.mComMain.Parameters.Add("CY", DbLang.VARCHAR).Value = healthy ? "Y" : "N";
                    DbNonQry(ins);
                }
            }
            catch { /* 하트비트 실패가 스케줄러를 죽이면 안 됨 */ }
        }

        // [LGLS] dest : 출고 화물의 **도착지(출고대)** 를 트랙에 기록한다(DEST_POS_OD).
        //   출고 목적지가 IO_TASK 메모리(m_dicOutStn/m_lstOutPend)에만 있으면 **재기동 시 사라져서**
        //   라인에 놓인 출고 화물을 아무도 가져가지 못하는 고아가 된다(도착지 0 으로 영구 정지).
        //   DB 에 남겨두면 RecoverOutOrphans() 가 대기열을 재구성해 스스로 복구할 수 있다.
        private void WriteCvSensor(string track, string sensor, string lugg, string jobTyp = "0", string dest = "")
        {
            try
            {
                // [LGLS 2026-07-19] 화물색 = 작업유형 배경색(입고=1 녹색 / 출고=2 파랑 / 클리어=0). GetCvColor가 색 결정.
                //   (구코드는 11/12(반자동 코드)를 차용했으나, 반자동이 다크 색으로 분리되면서 자동 코드 1/2로 정정 —
                //    클라이언트 GetCvColor의 AutoSto TRAY_LEV 분기 제거와 세트)
                string jt = (sensor == "1") ? ((jobTyp == "2") ? "2" : "1") : "0";
                // [LGLS] AUTO_MODE_RD 는 설비 운전모드이지 화물 유무 속성이 아님 → 클리어해도 '1' 유지.
                //   (구코드 sensor=='0' 일 때 '0' 기록 → 출고대 122 가 auto=0 으로 뭉개지고, 브리지(WCS_TASK_CV)는
                //    변화감지 게이트라 되돌리지 않아 DriveCV 의 AUTO_MODE_RD='1' 게이트가 영구 차단됨.)
                string am = "1";
                string s = "";
                s += CRLF + " UPDATE CV_DATA SET SENSOR0_DATA_RD = :SEN, LUGG_NO_RD = :LUGG, JOB_TYP_RD = :JT, AUTO_MODE_RD = :AM ";
                // [LGLS] 라인 트랙의 도착지 기록/정리. 클리어(sensor='0')면 '0' 으로 되돌린다.
                //   dest 는 라인 트랙에만 넘긴다(스테이션 122/124/126/129/130 의 DEST_POS_OD 는 DriveCV 발행이 소유).
                if (dest != "" || sensor == "0") s += CRLF + "      , DEST_POS_OD = :DEST         ";
                s += CRLF + "      , READ_UPD_DT = " + DbLang.SYSDATE + "                          ";   // [LGLS] readback 갱신 → DriveCV 의 READ_UPD_DT>WRITE_UPD_DT 게이트 충족(입고대 IO_TASK 공급용)
                s += CRLF + "  WHERE WH_TYP = :WH_TYP AND MC_NO = :MC_NO                          ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("SEN",    DbLang.VARCHAR).Value = sensor;
                _pBdb.mComMain.Parameters.Add("LUGG",   DbLang.VARCHAR).Value = (sensor == "1") ? Cap(lugg, 4) : "0000";   // [LGLS] 클리어는 '0000'(ECS 빈트랙 규약 + DriveCV/CompleteCV LUGG_NO_RD='0000' 게이트)
                _pBdb.mComMain.Parameters.Add("JT",     DbLang.VARCHAR).Value = jt;
                _pBdb.mComMain.Parameters.Add("AM",     DbLang.VARCHAR).Value = am;
                if (dest != "" || sensor == "0") _pBdb.mComMain.Parameters.Add("DEST", DbLang.VARCHAR).Value = (sensor == "1") ? dest : "0";
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("MC_NO",  DbLang.VARCHAR).Value = track;
                DbNonQry(s);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][RGV] WriteCvSensor 오류: " + ex.Message); }
        }



        // [LGLS] 출고 고아 화물 자동 복구.
        //   출고 화물이 RGV 픽업 트랙(홀수)에 놓였는데 대기열/반출 시퀀스 어디에도 없으면 아무도 가져가지 않는다.
        //   IO_TASK 재기동(=in-memory 대기열 소실)·비정상 종료 후 반드시 발생하며, 증상은
        //   "03/13/21 트랙 출고 화물이 도착지 없이 영구 정지". 트랙에 기록해둔 DEST_POS_OD 로 대기열을 재구성한다.
        private void RecoverOutOrphans()
        {
            // [LGLS 2026-08-31] 새 경로(OUT_VIA_RGV_STATE=1)에서는 고아가 생기지 않는다.
            //   30/35 가 DB 에 남으므로 재기동해도 DriveRGV 가 그대로 이어받는다.
            //   그대로 두면 30 인 출고를 "고아" 로 오인해 구 메모리 큐로 끌어가 경합한다.
            if (cDefApp.GM_OUT_VIA_RGV) return;
            try
            {
                string q = "";
                q += CRLF + " SELECT MC_NO, LUGG_NO_RD, DEST_POS_OD FROM CV_DATA ";
                // [LGLS 2026-08-23] JOB_TYP_RD 는 2026-07-19 화물색 코드 정정(반자동 11/12 → 자동 1/2)으로 '2' 가 된다.
                //   여기만 옛 코드 '12' 로 남아 있어서 그 뒤로 고아 복구가 **한 건도 매칭되지 않았다**
                //   (증상: IO_TASK 재기동 후 라인에 남은 출고 화물이 도착지 없이 영구 정지 - 작업 1663 사례).
                //   구데이터 호환을 위해 둘 다 받는다.
                q += CRLF + "  WHERE WH_TYP = :WH AND SENSOR0_DATA_RD = '1' AND JOB_TYP_RD IN ('2','12') ";
                q += CRLF + "    AND MC_NO IN ('101','103','105','109','113','117') ";   // 출고 RGV 픽업(홀수) 라인
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                // [LGLS 2026-07-19] 고아 0건이어도 return 금지 — 아래 짝수트랙/출고대 하역트랙 복구가 통째로 건너뛰어짐
                DataTable dt = (DbQry(q) > 0) ? _pBdb.mDtMain.Copy() : new DataTable();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string trk  = GetVal(dt.Rows[i], "MC_NO");
                    string lg   = GetVal(dt.Rows[i], "LUGG_NO_RD");
                    string dest = GetVal(dt.Rows[i], "DEST_POS_OD");
                    if (string.IsNullOrEmpty(lg) || lg == "0" || lg == "0000") continue;
                    if (m_dicOutStn.ContainsKey(lg)) continue;                       // 이미 반송 중
                    if (m_lstOutPend.Exists(x => x.Lugg == lg)) continue;           // 이미 대기열
                    if (!IsActiveOutboundJob(lg)) continue;                         // [LGLS 2026-08-28] 삭제/완료된 작업의 잔류 트래킹 → 유령 반송지시 금지
                    bool movePending = false;                                       // 곧 이 트랙으로 이동 예정이면 그쪽이 등록함
                    foreach (var kv in m_dicCvMove) if (kv.Value.Odd == trk) { movePending = true; break; }
                    if (movePending) continue;
                    // [LGLS 2026-08-23] 미기록 구화물 구제 - 122 고정이 아니라 JOB_MST 의 실제 도착지를 먼저 본다.
                    if (string.IsNullOrEmpty(dest) || dest == "0" || dest == "0000") dest = JobOutDestPos(lg, "122");
                    m_lstOutPend.Add(new OutPend { Lugg = lg, Odd = trk, OutStn = dest });
                    DbgLog("OUTREC_" + lg, "[OUT고아복구] " + lg + " 라인" + trk + "→출고대" + dest);
                    MakeMsg_Imp(string.Format("[SCH][OUT] 고아 출고화물 복구 - 작업 {0} 트랙 {1} → 출고대 {2}", lg, trk, dest));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][OUT] RecoverOutOrphans 오류: " + ex.Message); }

            // [LGLS 2026-07-19] SC 하역트랙(짝수 104/106/110/114/118)에 좌초한 출고 화물 복구.
            //   출고 SC 완료 시 예약되는 짝수→홀수 이동(m_dicCvMove)이 재기동으로 소실되면 화물이 짝수 트랙에
            //   영구 잔류하고, CanEnterLine(짝수) 가드 때문에 같은 크레인의 **후속 출고 완료(25→10)가 전부 차단**된다.
            //   → 이동 예약을 재구성해 홀수(RGV 픽업측)로 흘려보낸다.
            // [LGLS 2026-08-23] 이 블록은 m_dicCvMove(구경로 전용 - ProcessCvMove 가 구경로에서만 소비)를 쓴다.
            //   실경로에서는 짝수→홀수 이동을 설비가 직접 하므로 재구성하면 안 되고, 넣어봐야 소비자가 없어 쌓이기만 한다.
            try
            {
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][OUT] RecoverOutOrphans(SC하역트랙) 오류: " + ex.Message); }

            // [LGLS] 출고대 **하역트랙**(121→122, 125→126)에 좌초한 화물도 복구한다.
            //   여기 막힌 화물은 ProcessOutPend 의 "출고대가 비어야 시작" 조건에 걸려 **대기열 전체를 막는 머리**가 된다.
            //   반송은 이미 끝난 상태이므로 배출 단계(Stage 3)부터 재개시켜 내보내고 정리한다.
            // [LGLS 2026-08-23] 이 블록도 구경로 전용이다.
            //   Stage 3 은 "배출 단계부터 재개" 를 뜻하는데, 배출을 실제로 구동하는 것은 구경로의 ProcessOutStn 이다.
            //   실경로에서는 배출을 설비가 하고 도착 판정은 CompleteCV 가 하므로 여기서 등록할 일이 없다.
            //   오히려 등록하면 OutSeqPending 이 true 가 되어 **CompleteCV 가 그 작업을 완료시키지 못한다**
            //   (정상 출고에서도 화물이 121 을 지나가는 순간 걸려 완료가 지연됐다).
            try
            {
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][OUT] RecoverOutOrphans(출고대) 오류: " + ex.Message); }
        }

        // [LGLS 2026-07-21] 해당 작업의 출고대 반출(RTV) 시퀀스가 대기/진행 중인지 (CompleteCV 도착 인정 게이트용)
        private bool OutSeqPending(string lugg)
        {
            if (m_dicOutStn.ContainsKey(lugg)) return true;
            for (int i = 0; i < m_lstOutPend.Count; i++)
                if (m_lstOutPend[i].Lugg == lugg) return true;
            return false;
        }


        private void WriteCranePos(string id, int pos)
        {
            try
            {
                string tbl    = id[0] == 'R' ? "RTV_DATA_LGLS" : "SC_DATA_LGLS";
                string keyCol = id[0] == 'R' ? "RTV_NO"  : "SC_NO";
                string no     = id.Substring(1);
                string s = "";
                s += CRLF + " UPDATE " + tbl + "             ";
                s += CRLF + "    SET POS_H_RD   = :POS       ";
                s += CRLF + "  WHERE WH_TYP     = :WH_TYP    ";
                s += CRLF + "    AND " + keyCol + " = :NO    ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("POS",    DbLang.VARCHAR).Value = pos.ToString();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("NO",     DbLang.VARCHAR).Value = no;
                DbNonQry(s);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH] WriteCranePos 오류: " + ex.Message); }
        }

        #region CompleteRGV
        #endregion

        // ─────────────────────────────────────────────────────────────────
        // 알람 감시 : 설비 에러코드 로깅
        //   [슬라이드7] Alarm Set/Reset Report + Ack 의 Bit 핸드셰이크는 통신 Task 담당.
        //   스케줄러는 _RD 에러코드를 감시하여 신규 알람만 1회 로깅한다.
        // ─────────────────────────────────────────────────────────────────
        #region MonitorAlarm
        private bool RtvCompleteFor(string lugg)
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT             ";
                q += CRLF + "   FROM RTV_DATA_LGLS                    ";
                q += CRLF + "  WHERE WH_TYP      = :WH_TYP       ";
                q += CRLF + "    AND RTV_NO      = '801'         ";
                q += CRLF + "    AND COMPLETE_RD = '1'           ";
                q += CRLF + "    AND LUGG_OD     = :LG           ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LG",     DbLang.VARCHAR).Value = lugg;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return false; }
        }


        /// <summary>[LGLS 2026-08-24] 그 작업이 RGV 구동지시/구동중(31/35) 상태인지</summary>
        private bool IsRgvRunningJob(string lugg)
        {
            try {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT FROM JOB_MST                  ";
                q += CRLF + "  WHERE WH_TYP = :WH AND LUGG_NO = :LG                ";
                q += CRLF + "    AND JOB_STATUS IN ('" + ST_RGV_RUN + "') ";
                q += CRLF + "    AND (DEL_YN IS NULL OR DEL_YN <> 'Y')             ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LG", DbLang.VARCHAR).Value = lugg;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            } catch { return true; }
        }

        private void RtvResetComplete()
        {
            try {
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                // [LGLS 2026-07-22] 완료 시 지시 흔적(JOB_TYP_OD/LUGG_OD)도 클리어 — Client 레일색의
                //   m_bInvoke 의미(지시 성공=파랑, 완료/삭제=검정)를 구현 (Client 는 JOB_TYP_OD→JOB_TYP_RD 별칭 표시)
                string strSqlRtvRst = "";
                strSqlRtvRst += CRLF + " UPDATE RTV_DATA_LGLS                 ";
                strSqlRtvRst += CRLF + "    SET COMPLETE_RD  = '0',      ";
                strSqlRtvRst += CRLF + "        JOB_TYP_OD   = '0',      ";
                strSqlRtvRst += CRLF + "        LUGG_OD      = '0'       ";
                strSqlRtvRst += CRLF + "  WHERE WH_TYP       = :WH_TYP   ";
                strSqlRtvRst += CRLF + "    AND RTV_NO       = '801'     ";
                DbNonQry(strSqlRtvRst);
            } catch { }
        }

        /// <summary>[실경로] RGV 완료: 구동중(35) + RTV COMPLETE_RD='1'(해당 작업) → SC 처리 인계(20)</summary>
        /// <summary>
        /// [LGLS 2026-08-31] 출고 화물이 홀수(RGV 픽업) 트랙에 도착하면 15 → 30(RGV 구동대기) 로 올린다.
        ///   ★이 한 줄이 고아 문제의 핵심이다★ - 종전에는 이 사실이 m_lstOutPend(메모리)에만 있었다.
        ///   상태로 남기면 IO_TASK 가 죽어도 "순번 대기" 라는 사실이 DB 에 남아, 재기동 후 DriveRGV 가
        ///   그대로 이어받는다. 고아가 생길 자리가 없어진다.
        /// </summary>
        /// <summary>
        /// [LGLS 2026-08-31] 상태와 함께 HS_TRACK_NO(도착지 트랙)를 기록한다.
        ///   ★이 컬럼이 "누가 이 화물을 책임지는가" 를 DB 에 남긴다★
        ///   RGV 지시(→35) 시 RGV 도착지, 출고 SC 지시(→25) 시 SC 하역 도착지를 적어 두면
        ///   IO_TASK 가 죽어도 그 사실이 남아 재기동 후 그대로 이어받는다(고아가 생기지 않는다).
        ///   ※HOST 보고는 이 컬럼을 12/22 상태에서만 읽는데 신규 궤적엔 둘 다 없으므로 충돌하지 않는다.
        /// </summary>
        private bool UpdateJobStatusHs(string strStatus, string strLuggNo, string strHsTrack, ref string strRtn)
        {
            try
            {
                string strSql = "";
                strSql += CRLF + " UPDATE JOB_MST                       ";
                strSql += CRLF + "    SET JOB_STATUS  = :JOB_STATUS     ";
                strSql += CRLF + "      , HS_TRACK_NO = :HS_TRACK_NO    ";
                strSql += CRLF + "      , UPD_DT      = " + DbLang.SYSDATE + " ";
                strSql += CRLF + "      , UPD_USER_ID = '" + OD_USER + "' ";
                strSql += CRLF + "  WHERE WH_TYP      = :WH_TYP         ";
                strSql += CRLF + "    AND LUGG_NO     = :LUGG_NO        ";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("JOB_STATUS",  DbLang.VARCHAR).Value = strStatus;
                _pBdb.mComMain.Parameters.Add("HS_TRACK_NO", DbLang.VARCHAR).Value = strHsTrack ?? "";
                _pBdb.mComMain.Parameters.Add("WH_TYP",      DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LUGG_NO",     DbLang.VARCHAR).Value = strLuggNo;
                int n = DbNonQry(strSql);
                if (n < 0)  { strRtn += "JOB_MST 상태/도착지 기록 오류:" + _pBdb.ErrMsg; return false; }
                if (n == 0) { strRtn += "변경할 JOB_MST 작업이 없음(LUGG_NO:" + strLuggNo + ")"; return false; }
                return true;
            }
            catch (Exception ex) { strRtn += ex.Message; return false; }
        }

        /// <summary>
        /// [LGLS 2026-08-31] RGV 착지 처리.  (사용자 지시)
        ///   "RGV 도착지에서 도착지 신호가 꺼진 것과, 39 인 작업 중 HS_TRACK_NO 가 자기 자신인 작업을
        ///    찾아서 RGV 도착지에 데이터를 기록한다 (이러면서 15 가 된다)"
        ///   도착 신호(RTV_ARRIVEHS_READY_RD)가 꺼졌다 = RGV 가 그 트랙에 내려놓아 더 받을 수 없다.
        /// </summary>
        /// <summary>
        /// [LGLS 2026-08-31] 그 작업의 화물이 도착지에 실제로 내려졌는가.
        ///   설비가 하역트랙(짝수) → RGV 픽업트랙(홀수) 으로 곧바로 옮기므로 둘 다 본다.
        ///   ★신호(H/S)는 펄스라 폴링이 놓칠 수 있지만 화물은 남는다★ - 위치로 보는 편이 견고하다.
        /// </summary>
        /// <summary>
        /// [LGLS 2026-08-31] ★화물이 실제로 있는 트랙을 돌려준다★ (사용자 지적으로 원인 확인)
        ///   종전 LuggLandedAt 은 "하역트랙 또는 그 홀수 짝 어디에든 있으면 착지" 로 판정만 하고,
        ///   기록은 늘 하역트랙(짝수)에 했다. 그런데 설비는 크레인이 내려놓자마자 화물을
        ///   짝수(하역) → 홀수(RGV 픽업) 로 옮긴다. 그래서 ★화물은 105 에 있는데 기록은 106 에★
        ///   써졌다 - 태어날 때부터 잔재인 기록이다.
        ///   ("RTV 가 출고 화물을 가져가면 출고 HS 에 데이터가 써진다" 의 정체)
        ///   찾은 자리를 그대로 돌려주어 그 자리에 기록하게 한다. 없으면 빈 문자열.
        /// </summary>
        private string LuggLandedTrack(string strTrack, string strLuggNo)
        {
            if (string.IsNullOrEmpty(strTrack) || string.IsNullOrEmpty(strLuggNo)) return "";
            if ((TrackLugg(strTrack) ?? "").Trim() == strLuggNo) return strTrack;

            int n;
            if (int.TryParse(strTrack, out n))
            {
                string odd = (n % 2 == 0) ? (n - 1).ToString() : (n + 1).ToString();
                if ((TrackLugg(odd) ?? "").Trim() == strLuggNo) return odd;
            }
            return "";
        }

        private bool LuggLandedAt(string strTrack, string strLuggNo)
        {
            return !string.IsNullOrEmpty(LuggLandedTrack(strTrack, strLuggNo));
        }

        /// <summary>
        /// [LGLS 2026-08-31] 반자동/수동 작업 완결 처리.  (사용자 지시 : HOST_TASK 에서 IO_TASK 로 이관)
        ///   ★반자동은 상위에 보고하지 않는다★ - 09(완료)를 거치지 않고 19/29 에서 바로 지운다.
        ///     입고 계열 : 29(크레인 완료) = 최종
        ///     출고 계열 : 19(출고대 도착) = 최종
        ///   종전에는 HOST_TASK.GetJobCompleteReport 가 지웠다. 그러면 반자동 시험을 하려고
        ///   상위 통신을 내렸을 때 작업이 지워지지 않고 쌓인다 - 반자동은 상위와 무관해야 한다.
        ///   ※이력(JOB_MST_HIS)은 DB 트리거 trg_JOB_MST_StatusHis 가 남기므로 여기서는 삭제만 한다.
        /// </summary>
        private void DeleteSemiFinished()
        {
            try
            {
                // 반자동/수동 판정은 HOST_TASK 와 같은 기준을 쓴다 : LUGG 9000번대 또는 JOB_TYP 10 이상
                string q = "";
                q += CRLF + " DELETE FROM JOB_MST                                             ";
                q += CRLF + "  WHERE WH_TYP = :WH_TYP                                         ";
                // [LGLS 2026-08-31] ★TRY_CAST 를 쓰면 안 된다★ - 이 DB 는 SQL Server 2008(호환성 100) 이라
                //   TRY_CAST(2012+)를 인식하지 못하고 DELETE 가 매 주기 통째로 실패했다.
                //   (증상 : 반자동 9001 이 29 에서 지워지지 않고 남아 크레인 색까지 유지됐다)
                //   캐스팅 없이 판정한다 - 작업번호는 4자리 고정이라 문자열 비교로 충분하다.
                q += CRLF + "    AND ( (LEN(LUGG_NO) = 4 AND LUGG_NO >= '9000')                    ";
                q += CRLF + "       OR JOB_TYP IN ('10','11','12','13','14','15') )              ";
                q += CRLF + "    AND ( (JOB_TYP IN ('1','11','4','14') AND JOB_STATUS = '29')  ";
                q += CRLF + "       OR (JOB_TYP NOT IN ('1','11','4','14') AND JOB_STATUS = '19') ) ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                int n = DbNonQry(q);
                if (n > 0)
                    MakeMsg_Imp(string.Format("[SCH][JOB] 반자동 작업 완결 - {0}건 삭제(상위 보고 없음)", n));
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][JOB] DeleteSemiFinished 오류: " + ex.Message); }
        }

        private void LandRgvDrop()
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT JM.LUGG_NO, JM.JOB_TYP, " + DbLang.NVL + "(JM.HS_TRACK_NO,'') AS HS, ";
                q += CRLF + "        JM.DEST_POS, DATEDIFF(second, JM.UPD_DT, GETDATE()) AS ELAPSED ";
                q += CRLF + "   FROM JOB_MST JM                                  ";
                q += CRLF + "  WHERE JM.WH_TYP     = :WH_TYP                     ";
                q += CRLF + "    AND JM.JOB_STATUS = :ST_DONE                    ";
                q += CRLF + "    AND (JM.DEL_YN IS NULL OR JM.DEL_YN <> 'Y')     ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_DONE", DbLang.VARCHAR).Value = ST_RGV_DONE;
                if (DbQry(q) <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string luggNo = GetVal(dt.Rows[i], "LUGG_NO");
                    string hs     = (GetVal(dt.Rows[i], "HS") ?? "").Trim();
                    if (string.IsNullOrEmpty(hs)) continue;

                    // [LGLS 2026-08-31] 신호가 아니라 화물 위치로 판정한다(위 LandScDrop 주석 참조).
                    string landTrk = LuggLandedTrack(hs, luggNo);
                    if (string.IsNullOrEmpty(landTrk))
                    {
                        // [LGLS 2026-09-04] (사용자 확정) 39 + HS_TRACK_NO 와 CV_DATA 를 묶어, 도착트랙에 화물은 있는데
                        //   작업번호(트래킹)가 비어 있으면 WCS 가 그 트랙에 작업번호를 기록한다(PLC 담당자 : 하역 후 번호는 ECS 가 써 준다).
                        //   설비가 번호를 함께 옮겨 준 경우(트래킹 = 작업번호)는 위 LuggLandedTrack 에서 이미 착지로 잡힌다.
                        if (RequestArrivalTrackingWrite(hs, luggNo))
                        {
                            MakeMsg_Imp(string.Format("[SCH][RGV] 작업 {0} RGV 도착지 {1} 화물 감지 - 트랙에 작업번호 기록 요청", luggNo, hs));
                            continue;
                        }
                        DbgLog("LANDRGV_" + luggNo, "[착지대기] " + luggNo + " RGV 도착지 " + hs + " 에 아직 화물 없음");
                        // [LGLS 2026-09-01] ★겸용 출고대(122) 직행 드롭의 최종 구간 특례★ (9007 실측)
                        //   RGV 가 121 에 내려놓으면 벨트가 즉시 121→122 로 옮기고, 출고대 신호
                        //   ON 3초 뒤 지게차가 가져간다. RGV 완료 감지(폴링)가 그보다 늦으면
                        //   39 가 된 시점엔 화물이 이미 배출된 뒤라 여기서 영영 기다렸다.
                        //   조건을 좁혀 판정한다 : 출고 작업 + 도착지가 최종 출고대(DEST=122)
                        //   + 그 짝(121/122) 모두 빔 + RGV 가 이 화물을 더 이상 물고 있지 않음
                        //   + 39 로 20초 경과 → 배출 완료로 보고 19(출고 최종)로 올린다.
                        {
                            string jTyp2   = GetVal(dt.Rows[i], "JOB_TYP");
                            string dest2   = (GetVal(dt.Rows[i], "DEST_POS") ?? "").Trim();
                            int elapsed; int.TryParse(GetVal(dt.Rows[i], "ELAPSED"), out elapsed);
                            if (AutoTimeProcEnabled() &&
                                (jTyp2 == "2" || jTyp2 == "12") && dest2 == "122" && elapsed >= 20 &&
                                IsTrackEmpty("121") && IsTrackEmpty("122"))
                            {
                                string rtnF = "";
                                if (UpdateJobStatus(ST_CV_DONE, luggNo, ref rtnF))
                                    MakeMsg_Imp(string.Format(
                                        "[SCH][RGV] 작업 {0} 겸용 출고대 배출 확인(라인 빔, {1}초 경과) → 상태 '{2}' (출고 최종)",
                                        luggNo, elapsed, ST_CV_DONE));
                                continue;
                            }
                        }
                        continue;
                    }

                    string rtn = "";
                    // [LGLS 2026-08-31] ★착지 기록(트랙 R영역 쓰기)을 폐기했다★ - 구 ECS 기준 확인 결과.
                    //   구 ECS 는 ECSDispatcher.cs:592 에서 SetPallet(fromPort, palletId) 단 한 곳,
                    //   즉 ★화물이 실제로 있는 출발 포트★ 에 그 구간을 시작할 때만 쓴다
                    //   (fromPort.IsPalletExist == true 조건 안). ★도착지에 쓰는 코드는 없다.★
                    //   도착 뒤의 트래킹은 설비가 화물과 함께 옮긴다 -
                    //   EQP_SIM 도 VehicleSim.UnloadAtDest 가 PlacePallet(port, carrying.Id) 로
                    //   하역 화물에 번호를 함께 얹고, ConveyorSim 이 이동 때마다 따라 옮긴다.
                    //   우리가 도착지에 덧쓰던 값만 그 자리에 눌러앉아 잔재가 됐다.
                    //   (우리 쪽 SetPallet 대응물은 UpdateCvData - CV 반송지시 시 출발 트랙에 쓴다)
                    if (UpdateJobStatus(ST_CV_RUN, luggNo, ref rtn))
                        MakeMsg_Imp(string.Format("[SCH][RGV] 작업 {0} RGV 도착지 {1} 기록 완료 → 상태 '{2}'",
                                    luggNo, landTrk, ST_CV_RUN));
                    else
                        MakeMsg_Error(string.Format("[SCH][RGV] 착지 전이 실패({0}): {1}", luggNo, rtn));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][RGV] LandRgvDrop 오류: " + ex.Message); }
        }

        /// <summary>
        /// [LGLS 2026-08-31] SC 착지 처리(출고 전용).  (사용자 지시)
        ///   "SC 도착지에서 도착지 신호가 꺼진 것과, 29 인 작업 중 HS_TRACK_NO 가 자기 자신인 작업을
        ///    찾아서 SC 도착지에 데이터를 기록한다 (이러면서 15 가 된다)"
        ///   ※입고의 29 는 최종(크레인이 랙에 넣음)이므로 대상이 아니다.
        /// </summary>
        private void LandScDrop()
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT JM.LUGG_NO, " + DbLang.NVL + "(JM.HS_TRACK_NO,'') AS HS ";
                q += CRLF + "   FROM JOB_MST JM                                  ";
                q += CRLF + "  WHERE JM.WH_TYP     = :WH_TYP                     ";
                q += CRLF + "    AND JM.JOB_TYP   IN ('2','12')                  ";
                q += CRLF + "    AND JM.JOB_STATUS = :ST_DONE                    ";
                // [LGLS 2026-08-31] 1차 완료보고를 기다리지 않는다 (사용자 확정 : 누락되어도 무방).
                //   보고를 기다리면 크레인이 29 에 붙잡혀 화물을 든 채 서 있게 된다.
                //   착지(도착 신호 OFF)만 보고 바로 15 로 내린다.
                q += CRLF + "    AND (JM.DEL_YN IS NULL OR JM.DEL_YN <> 'Y')     ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_DONE", DbLang.VARCHAR).Value = ST_SC_DONE;
                if (DbQry(q) <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string luggNo = GetVal(dt.Rows[i], "LUGG_NO");
                    string hs     = (GetVal(dt.Rows[i], "HS") ?? "").Trim();
                    if (string.IsNullOrEmpty(hs)) continue;

                    // [LGLS 2026-08-31] ★신호가 아니라 화물 위치로 판정한다★
                    //   출고 H/S(RETHS_READY_RD)는 크레인이 내려놓는 동안만 꺼졌다가, 설비가 화물을
                    //   짝수(하역) → 홀수(RGV 픽업) 로 옮기면 ★다시 켜진다★. 폴링이 그 짧은 창을
                    //   놓치면 작업이 29 에 영원히 갇힌다(실측 : 0426/0428 이 35분 잔류, 크레인에는
                    //   색만 남음). 화물은 남지만 신호는 사라지므로 위치로 본다.
                    //   하역트랙(짝수) 또는 그 홀수 짝 어디에든 그 작업 화물이 있으면 착지 완료다.
                    string landTrk = LuggLandedTrack(hs, luggNo);
                    if (string.IsNullOrEmpty(landTrk))
                    {
                        DbgLog("LANDSC_" + luggNo, "[착지대기] " + luggNo + " SC 도착지 " + hs + " 에 아직 화물 없음");
                        continue;
                    }

                    string rtn = "";
                    // [LGLS 2026-08-31] ★착지 기록(트랙 R영역 쓰기)을 폐기했다★ - 구 ECS 기준 확인 결과.
                    //   구 ECS 는 ECSDispatcher.cs:592 에서 SetPallet(fromPort, palletId) 단 한 곳,
                    //   즉 ★화물이 실제로 있는 출발 포트★ 에 그 구간을 시작할 때만 쓴다
                    //   (fromPort.IsPalletExist == true 조건 안). ★도착지에 쓰는 코드는 없다.★
                    //   도착 뒤의 트래킹은 설비가 화물과 함께 옮긴다 -
                    //   EQP_SIM 도 VehicleSim.UnloadAtDest 가 PlacePallet(port, carrying.Id) 로
                    //   하역 화물에 번호를 함께 얹고, ConveyorSim 이 이동 때마다 따라 옮긴다.
                    //   우리가 도착지에 덧쓰던 값만 그 자리에 눌러앉아 잔재가 됐다.
                    //   (우리 쪽 SetPallet 대응물은 UpdateCvData - CV 반송지시 시 출발 트랙에 쓴다)
                    if (UpdateJobStatus(ST_CV_RUN, luggNo, ref rtn))
                        MakeMsg_Imp(string.Format("[SCH][SC] 작업 {0} SC 도착지 {1} 기록 완료 → 상태 '{2}'",
                                    luggNo, landTrk, ST_CV_RUN));
                    else
                        MakeMsg_Error(string.Format("[SCH][SC] 착지 전이 실패({0}): {1}", luggNo, rtn));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][SC] LandScDrop 오류: " + ex.Message); }
        }


        /// <summary>
        /// [LGLS 2026-08-31] RTV 가 "다른 작업" 을 실제로 반송 중인가.  (사용자 확정 조건)
        ///   JOB_STATUS='35' 인 작업이 있고, ★RTV 가 그 작업번호를 받아★ 동작 중
        ///   (RTV 레일 파랑 = JOB_TYP_OD 가 '0'/공백이 아님) 일 때만 배타로 본다.
        ///   상태만 35 로 남고 RTV 는 이미 손을 뗀 경우까지 막으면 영구 정체가 된다.
        /// </summary>
        private bool IsRtvBusyWithOwnJob(string luggNoSelf)
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT COUNT(*) AS CNT                                  ";
                q += CRLF + "   FROM JOB_MST JM                                       ";
                q += CRLF + "  INNER JOIN RTV_DATA_LGLS RD                            ";
                q += CRLF + "     ON RD.WH_TYP  = JM.WH_TYP                           ";
                q += CRLF + "    AND RD.LUGG_OD = JM.LUGG_NO                          ";
                q += CRLF + "  WHERE JM.WH_TYP     = :WH_TYP                          ";
                q += CRLF + "    AND JM.JOB_STATUS = :ST_RUN                          ";
                q += CRLF + "    AND JM.LUGG_NO   <> :SELF                            ";
                q += CRLF + "    AND ISNULL(RD.JOB_TYP_OD,'0') NOT IN ('0','')        ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST_RUN", DbLang.VARCHAR).Value = ST_RGV_RUN;
                _pBdb.mComMain.Parameters.Add("SELF",   DbLang.VARCHAR).Value = luggNoSelf;
                if (DbQry(q) <= 0) return false;
                int n; int.TryParse(GetVal(_pBdb.mDtMain.Rows[0], "CNT"), out n);
                return n > 0;
            }
            catch { return false; }
        }

        /// <summary>
        /// [LGLS 2026-09-04] 운전 화면 RTV 수동지시(작업번호 9998)의 종료 처리.
        ///   수동지시는 JOB_MST 에 없어 CompleteRGVReal 이 잡지 못했고, RTV 의 COMPLETE_RD=1 / LUGG_OD=9998 이
        ///   그대로 남아 화면에 "9998 진행 중" 으로 영영 보였다. 완료를 감지하면 지시 흔적을 지우고 이력(알람창)에 남긴다.
        /// </summary>
        private void CompleteRGVManual()
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT DEPART_TRACK, ARRIVE_TRACK FROM RTV_DATA_LGLS ";
                q += CRLF + "  WHERE WH_TYP = :WH_TYP AND RTV_NO = '801'         ";
                q += CRLF + "    AND COMPLETE_RD = '1' AND LUGG_OD = '9998'    ";
                q += CRLF + "    AND OD_RQ_YN = 'N' AND TRANSFER_REQUEST_OD = 'N' ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) return;
                string dep = GetVal(_pBdb.mDtMain.Rows[0], "DEPART_TRACK");
                string arr = GetVal(_pBdb.mDtMain.Rows[0], "ARRIVE_TRACK");
                RtvResetComplete();
                string strUpd = "";
                strUpd += CRLF + " UPDATE RTV_DATA_LGLS SET DEPART_TRACK = '', ARRIVE_TRACK = '', PALLET_ID_OD = '' ";
                strUpd += CRLF + "  WHERE WH_TYP = :WH_TYP AND RTV_NO = '801' AND LUGG_OD = '0' ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                DbNonQry(strUpd);
                string msg = string.Format("[RGV] 수동지시(9998) 반송 완료 - {0} → {1}, RTV 지시 정보 정리", dep, arr);
                DbgLog("RGVMAN_" + DateTime.Now.Ticks, msg);
                MakeMsg_Imp("[SCH]" + msg);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][RGV] CompleteRGVManual 오류: " + ex.Message); }
        }

        /// <summary>
        /// [LGLS 2026-09-05] 운전 화면 SC 수동지시(작업번호 9999)의 종료 처리.
        ///   수동지시는 JOB_MST 에 없어 CompleteSC 가 잡지 못한다. 완료(COMPLETE_RD)를 감지하면
        ///   지시 흔적을 지우고 이력에 남긴다. CompleteRGVManual(RTV 9998) 과 대칭이다.
        ///   ※ ManualSc 가 VehThread 소비 컬럼(TRANSFER_REQUEST_OD/FROM_/TO_)을 채우도록 고친
        ///     2026-09-05 이후에야 이 완료가 실제로 발생한다(그 전에는 지시 자체가 나가지 않았다).
        /// </summary>
        private void CompleteSCManual()
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT SC_NO FROM SC_DATA_LGLS                          ";
                q += CRLF + "  WHERE WH_TYP            = :WH_TYP                      ";
                q += CRLF + "    AND LUGG_NO_FK1_OD    = '9999'                       ";
                q += CRLF + "    AND COMPLETE_RD IS NOT NULL                          ";
                q += CRLF + "    AND COMPLETE_RD NOT IN ('0','00','0000','')          ";
                q += CRLF + "    AND OD_RQ_YN          = 'N'                          ";
                q += CRLF + "    AND TRANSFER_REQUEST_OD = 'N'                        ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string scNo = GetVal(dt.Rows[i], "SC_NO");

                    // 지시 흔적 정리 : 남겨 두면 다음 작업을 받은 크레인에 9999 가 붙어 보이고
                    //   잔존 COMPLETE_RD 가 다음 완료 판정에 섞인다(ClearScOd 와 같은 취지).
                    string u = "";
                    u += CRLF + " UPDATE SC_DATA_LGLS                                  ";
                    u += CRLF + "    SET LUGG_NO_FK1_OD     = '0000'                   ";
                    u += CRLF + "      , PALLET_ID_OD       = '0000'                   ";
                    u += CRLF + "      , JOB_TYP_OD         = '0'                      ";
                    u += CRLF + "      , JOB_TYP_RD         = '0'                      ";
                    u += CRLF + "      , COMPLETE_RD        = '0'                      ";
                    u += CRLF + "      , ITN_LUGG_FK1       = '0'                      ";
                    u += CRLF + "      , PALLET_ON_VEHICLE_RD = ''                     ";
                    u += CRLF + "      , FROM_01_OD = '00', FROM_02_OD = '00', FROM_03_OD = '00' ";
                    u += CRLF + "      , TO_01_OD   = '00', TO_02_OD   = '00', TO_03_OD   = '00' ";
                    u += CRLF + "      , START_BANK_FK1_OD  = '0', START_BAY_FK1_OD  = '0'  ";
                    u += CRLF + "      , START_LEVEL_FK1_OD = '0', START_HSPOS_FK1_OD = '0'  ";
                    u += CRLF + "      , DEST_BANK_FK1_OD   = '0', DEST_BAY_FK1_OD   = '0'  ";
                    u += CRLF + "      , DEST_LEVEL_FK1_OD  = '0', DEST_HSPOS_FK1_OD = '0'  ";
                    u += CRLF + "  WHERE WH_TYP = :WH_TYP AND SC_NO = :SC_NO           ";
                    _pBdb.mComMain.CommandType = CommandType.Text;
                    _pBdb.mComMain.Parameters.Clear();
                    _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                    _pBdb.mComMain.Parameters.Add("SC_NO",  DbLang.VARCHAR).Value = scNo;
                    DbNonQry(u);

                    m_dicPrevSC.Remove("SC_" + scNo);
                    string msg = string.Format("[SC] 수동지시(9999) 반송 완료 - S/C #{0}, 지시 정보 정리", scNo);
                    DbgLog("SCMAN_" + scNo, msg);
                    MakeMsg_Imp("[SCH]" + msg);
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][SC] CompleteSCManual 오류: " + ex.Message); }
        }

        private void CompleteRGVReal()
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT JM.LUGG_NO, JM.JOB_TYP  ";
                q += CRLF + "   FROM JOB_MST JM              ";
                q += CRLF + "  WHERE JM.WH_TYP      = :WH_TYP ";
                q += CRLF + "    AND JM.JOB_STATUS  = :ST     ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("ST",     DbLang.VARCHAR).Value = ST_RGV_RUN;
                if (DbQry(q) <= 0) return;
                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string luggNo = GetVal(dt.Rows[i], "LUGG_NO");
                    string jTyp   = GetVal(dt.Rows[i], "JOB_TYP");
                    if (jTyp == "11") jTyp = "1"; else if (jTyp == "12") jTyp = "2";
                    if (!RtvCompleteFor(luggNo)) continue;
                    string rtn = "";
                    // [LGLS 2026-08-31] RGV 반송 완료 = 39. 도착지에 데이터를 기록하는 것은
                    //   LandRgvDrop() 이 도착 신호가 꺼진 것을 보고 한다(그때 15 가 된다).
                    string stNextRgv = ST_RGV_DONE;
                    if (UpdateJobStatus(stNextRgv, luggNo, ref rtn))
                    {
                        RtvResetComplete();
                        m_dicPrevRGV.Remove("RGV_801");
                        MakeMsg_Imp(string.Format("[SCH][RGV] 작업 {0} RTV 반송 완료 → 상태 '{1}' (도착지 기록 대기)",
                                    luggNo, stNextRgv));
                    }
                    else
                        MakeMsg_Error(string.Format("[SCH][RGV] 완료 전이 실패({0}): {1}", luggNo, rtn));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][RGV] CompleteRGVReal 오류: " + ex.Message); }
        }

        private readonly HashSet<string> m_setUnsupportedLogged = new HashSet<string>();   // [LGLS] 미지원 JOB_TYP 1회 로깅용
        private readonly HashSet<string> m_setOutArrived = new HashSet<string>();          // [LGLS 2026-07-21] 출고 실도착 관측 플래그(배출 확인용)
        // [LGLS 2026-07-22] 표시용 작업구분 동기화: 트랙 화물(LUGG_NO_RD)·크레인 진행 작업의 구분을
        //   CV_DATA/SC_DATA_LGLS.JOB_TYP_RD 에 반영(없음=0). 변경 시에만 UPDATE - 표시 전용.
        //   [LGLS 2026-08-31] ★작업구분을 그대로 쓴다★ (사용자 지적 : 반자동 색이 범례와 다름)
        //     종전에는 CASE WHEN JOB_TYP IN (2,12) THEN 2 ELSE 1 로 뭉개서 반자동(11/12)이
        //     자동(1/2)과 같은 값이 됐다. Client 는 enJobTypeSemiSto(11)/SemiRet(12) 에
        //     별도 색(m_clrUSER_COLOR_SEMI_*)을 갖고 있는데 그 값이 도달하지 못해
        //     반자동 화물이 자동 색으로 그려졌다.
        private void SyncDisplayTyp()
        {
            try
            {
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                string strSqlDsp1 = "";
                strSqlDsp1 += CRLF + " UPDATE CD                                                              ";
                strSqlDsp1 += CRLF + "    SET JOB_TYP_RD = JM.JOB_TYP ";
                strSqlDsp1 += CRLF + "   FROM CV_DATA CD                                                      ";
                strSqlDsp1 += CRLF + "  INNER JOIN JOB_MST JM                                                 ";
                strSqlDsp1 += CRLF + "     ON JM.WH_TYP   = CD.WH_TYP                                         ";
                strSqlDsp1 += CRLF + "    AND JM.LUGG_NO  = CD.LUGG_NO_RD                                     ";
                strSqlDsp1 += CRLF + "  WHERE CD.WH_TYP   = :WH_TYP                                           ";
                strSqlDsp1 += CRLF + "    AND ISNULL(CD.JOB_TYP_RD,'0') <> JM.JOB_TYP ";
                DbNonQry(strSqlDsp1);

                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                string strSqlDsp2 = "";
                strSqlDsp2 += CRLF + " UPDATE CV_DATA                                             ";
                strSqlDsp2 += CRLF + "    SET JOB_TYP_RD = '0'                                    ";
                strSqlDsp2 += CRLF + "  WHERE WH_TYP     = :WH_TYP                                ";
                strSqlDsp2 += CRLF + "    AND ISNULL(JOB_TYP_RD,'0') <> '0'                       ";
                strSqlDsp2 += CRLF + "    AND (LUGG_NO_RD IS NULL OR LUGG_NO_RD IN ('','0','0000')) ";
                DbNonQry(strSqlDsp2);

                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                // [LGLS] 지시(21)~완료 구간만 — 레일색 m_bInvoke 의미
                string strSqlDsp3 = "";
                strSqlDsp3 += CRLF + " UPDATE SD                                                              ";
                strSqlDsp3 += CRLF + "    SET JOB_TYP_RD = JM.JOB_TYP ";
                strSqlDsp3 += CRLF + "   FROM SC_DATA_LGLS SD                                                      ";
                strSqlDsp3 += CRLF + "  INNER JOIN JOB_MST JM                                                 ";
                strSqlDsp3 += CRLF + "     ON JM.WH_TYP   = SD.WH_TYP                                         ";
                strSqlDsp3 += CRLF + "    AND SD.SC_NO    = (CASE WHEN JM.JOB_TYP IN ('1','11') THEN JM.DEST_POS ELSE JM.START_POS END) ";
                strSqlDsp3 += CRLF + "  WHERE SD.WH_TYP   = :WH_TYP                                           ";
                strSqlDsp3 += CRLF + "    AND JM.JOB_STATUS IN ('25','29')                               ";
                strSqlDsp3 += CRLF + "    AND ISNULL(SD.JOB_TYP_RD,'0') <> JM.JOB_TYP ";
                DbNonQry(strSqlDsp3);

                // [LGLS 2026-08-31] 크레인이 화물을 들고 있는 동안은 색을 유지한다 (사용자 지적)
                //   증상 : 출고를 마친 크레인에 색 없이 작업번호만 남았다.
                //   원인 : 색은 작업 상태(25/29)로만 칠하는데 작업이 15 로 내려가는 순간 색이 지워진다.
                //          반면 크레인의 작업번호는 설비가 치울 때까지 남는다.
                //          색 지우는 시점과 번호 지워지는 시점이 어긋난 것이다.
                //   조치 : 상태가 아니라 크레인이 실제로 든 화물을 기준으로도 칠한다.
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                string strSqlDsp3b = "";
                strSqlDsp3b += CRLF + " UPDATE SD                                                    ";
                strSqlDsp3b += CRLF + "    SET JOB_TYP_RD = JM.JOB_TYP                               ";
                strSqlDsp3b += CRLF + "   FROM SC_DATA_LGLS SD                                       ";
                strSqlDsp3b += CRLF + "  INNER JOIN JOB_MST JM                                       ";
                strSqlDsp3b += CRLF + "     ON JM.WH_TYP  = SD.WH_TYP                                ";
                strSqlDsp3b += CRLF + "    AND JM.LUGG_NO = " + SC_HELD_LUGG;
                strSqlDsp3b += CRLF + "  WHERE SD.WH_TYP  = :WH_TYP                                  ";
                strSqlDsp3b += CRLF + "    AND ISNULL(SD.JOB_TYP_RD,'0') <> JM.JOB_TYP               ";
                DbNonQry(strSqlDsp3b);

                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                string strSqlDsp4 = "";
                strSqlDsp4 += CRLF + " UPDATE SD                                                              ";
                strSqlDsp4 += CRLF + "    SET JOB_TYP_RD = '0'                                                ";
                strSqlDsp4 += CRLF + "   FROM SC_DATA_LGLS SD                                                      ";
                strSqlDsp4 += CRLF + "  WHERE SD.WH_TYP   = :WH_TYP                                           ";
                strSqlDsp4 += CRLF + "    AND ISNULL(SD.JOB_TYP_RD,'0') <> '0'                                ";
                // [LGLS 2026-08-31] 크레인이 화물을 들고 있으면 색을 지우지 않는다(위 3b 참조).
                strSqlDsp4 += CRLF + "    AND " + SC_HELD_LUGG + " IS NULL                           ";
                strSqlDsp4 += CRLF + "    AND NOT EXISTS (SELECT 1                                            ";
                strSqlDsp4 += CRLF + "                      FROM JOB_MST JM                                   ";
                strSqlDsp4 += CRLF + "                     WHERE JM.WH_TYP = SD.WH_TYP                        ";
                strSqlDsp4 += CRLF + "                       AND JM.JOB_STATUS IN ('25','29')            ";
                strSqlDsp4 += CRLF + "                       AND SD.SC_NO = (CASE WHEN JM.JOB_TYP IN ('1','11') THEN JM.DEST_POS ELSE JM.START_POS END)) ";
                DbNonQry(strSqlDsp4);

                // [LGLS 2026-08-31] RTV 화물색 : 지시 시점에 정규화(11→1/12→2)된 값이 들어가 있어
                //   반자동이 자동 색으로 그려졌다(사용자 지적). 살아 있는 작업의 실제 구분으로 맞춘다.
                //   ※JOB_TYP_OD 를 조건으로 쓰는 게이트(DriveCV 122 보류 / DriveSC 901 보류)는
                //     IN ('2','12') / IN ('1','11') 로 함께 넓혔다.
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                string strSqlDsp5 = "";
                strSqlDsp5 += CRLF + " UPDATE RD                                                   ";
                strSqlDsp5 += CRLF + "    SET JOB_TYP_OD = JM.JOB_TYP                              ";
                strSqlDsp5 += CRLF + "   FROM RTV_DATA_LGLS RD                                     ";
                strSqlDsp5 += CRLF + "  INNER JOIN JOB_MST JM                                      ";
                strSqlDsp5 += CRLF + "     ON JM.WH_TYP  = RD.WH_TYP                               ";
                strSqlDsp5 += CRLF + "    AND JM.LUGG_NO = RD.LUGG_OD                              ";
                strSqlDsp5 += CRLF + "  WHERE RD.WH_TYP  = :WH_TYP                                 ";
                strSqlDsp5 += CRLF + "    AND ISNULL(RD.JOB_TYP_OD,'0') NOT IN ('0','')          ";
                strSqlDsp5 += CRLF + "    AND ISNULL(RD.JOB_TYP_OD,'0') <> JM.JOB_TYP             ";
                DbNonQry(strSqlDsp5);

                SyncScForkPos();
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][DSP] SyncDisplayTyp 오류: " + ex.Message); }
        }

        private readonly Dictionary<string, string> m_dicAlarm = new Dictionary<string, string>();
        // [LGLS 2026-08-30] 이중입고/공출고 발생 시 그 작업의 상태를 에러 상태로 바꾼다.
        //   JOB_STATUS (common_code 'JOB_STATUS') : 09 = 이중입고 에러, 08 = 공출고 에러
        //   종전에는 크레인만 멈추고 JOB_MST 는 직전 상태(20/25 등) 그대로여서, 운전화면에서
        //   "왜 안 움직이는지" 를 작업 목록만 보고는 알 수 없었다.
        //   에러 상태로 바꿔두면 ① 화면에 사유가 드러나고 ② 정상 지시 쿼리(JOB_STATUS = 20/30 …)에
        //   더는 걸리지 않아 조치 전까지 재지시되지 않는다.
        private const string ST_ERR_DUAL_STORE   = "08";   // 이중입고 에러  [LGLS 2026-08-30] 09 → 08 (09 는 완료가 가져감)
        private const string ST_ERR_EMPTY_RETR   = "07";   // 공출고 에러    [LGLS 2026-08-30] 08 → 07
        private const string ST_RETRY_DUAL_STORE = "06";   // 이중입고 재지정 (상위가 새 셀을 내려준 상태)  [LGLS 2026-08-30] 07 → 06
        private const string ST_RETRY_EMPTY_RETR = "05";   // 공출고 재지정  [LGLS 2026-08-30] 06 → 05

        /// <summary>
        /// [LGLS 2026-08-30] 재지정 후 작업 재개.
        ///   상위(WMS)가 R 전문으로 새 로케이션을 내려주면 HOST_TASK 가 JOB_MST 의 로케이션을 바꾸고
        ///   상태를 07(이중입고 재지정) / 06(공출고 재지정) 으로 둔다. 종전에는 여기서 끝이라
        ///   아무도 그 작업을 다시 집지 않았고, 크레인도 새 반송지시를 못 받아 에러가 안 풀렸다.
        ///
        ///   재개 규약 — "재지정 = 크레인이 든 화물을 새 셀로 보낸다"
        ///     · 이중입고(입고) : 크레인이 화물을 든 채 원래 목적셀에 서 있다.
        ///                       From = 크레인 현재 위치(LOCATION_*_RD), To = 새 DEST_LOCATION.
        ///     · 공출고(출고)   : 크레인은 빈 채로 서 있다(그 셀에 재고가 없었다).
        ///                       From = 새 START_LOCATION, To = 출고 하역트랙.
        ///   설비는 새 TRANSFER_REQUEST 를 받으면 에러를 해제한다(EQP_SIM VState.Error / 실 PLC 동일 규약).
        ///   ★현장 확인 필요: 실 SFA 크레인이 "이미 든 화물"을 From 재픽업 없이 To 로 옮기는지 —
        ///     EQP_SIM 은 그렇게 동작하도록 맞춰 두었다(적재 상태 유지 후 목적지로 직행).
        /// </summary>
        private void ResumeRedirectedJobs()
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT JM.LUGG_NO, JM.JOB_TYP, JM.JOB_STATUS                        ";
                q += CRLF + "      , JM.START_POS, JM.DEST_POS, JM.START_LOCATION, JM.DEST_LOCATION ";
                q += CRLF + "      , SD.SC_NO                                                     ";
                q += CRLF + "      , SD.LOCATION_01_RD, SD.LOCATION_02_RD, SD.LOCATION_03_RD      ";
                q += CRLF + "   FROM JOB_MST JM                                                   ";
                q += CRLF + "  INNER JOIN SC_DATA_LGLS SD                                         ";
                q += CRLF + "     ON JM.WH_TYP = SD.WH_TYP                                        ";
                q += CRLF + "    AND SD.SC_NO  = " + SC_POS_EXPR + "                              ";
                q += CRLF + "  WHERE JM.WH_TYP     = :WH_TYP                                      ";
                q += CRLF + "    AND JM.JOB_STATUS IN ('" + ST_RETRY_DUAL_STORE + "','" + ST_RETRY_EMPTY_RETR + "') ";
                q += CRLF + "    AND SD.TRANSFER_REQUEST_OD = 'N'                                 ";   // 직전 지시가 소비된 뒤에만
                q += CRLF + "    AND (JM.DEL_YN IS NULL OR JM.DEL_YN <> 'Y')                      ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string luggNo   = GetVal(dt.Rows[i], "LUGG_NO");
                    string jobTyp   = GetVal(dt.Rows[i], "JOB_TYP");
                    string scNo     = GetVal(dt.Rows[i], "SC_NO");
                    string startPos = GetVal(dt.Rows[i], "START_POS");
                    string startLoc = GetVal(dt.Rows[i], "START_LOCATION");
                    string destLoc  = GetVal(dt.Rows[i], "DEST_LOCATION");
                    if (jobTyp == "11") jobTyp = "1"; else if (jobTyp == "12") jobTyp = "2";

                    string f1, f2, f3, t1, t2, t3;
                    if (jobTyp == "1")
                    {
                        // 크레인이 서 있는 자리(= 이중입고가 난 셀)에서 새 셀로
                        f1 = Cap(GetVal(dt.Rows[i], "LOCATION_01_RD"), 2);
                        f2 = Cap(GetVal(dt.Rows[i], "LOCATION_02_RD"), 2);
                        f3 = Cap(GetVal(dt.Rows[i], "LOCATION_03_RD"), 2);
                        VehCellLoc(destLoc, out t1, out t2, out t3);
                    }
                    else
                    {
                        VehCellLoc(startLoc, out f1, out f2, out f3);
                        VehPortLoc(RgvOutDropTrack(startPos), out t1, out t2, out t3);
                    }

                    MakeMsg_Imp(string.Format("[SCH][재지정] S/C #{0} 작업:{1}(TYP:{2}) 재개 지시 (From:{3}/{4}/{5} To:{6}/{7}/{8})",
                        scNo, luggNo, jobTyp, f1, f2, f3, t1, t2, t3));

                    _pBdb.BeginTrans();
                    string rtn = "";
                    bool ok = UpdateScData(scNo, jobTyp, luggNo, f1, f2, f3, t1, t2, t3, ref rtn, true)
                              && UpdateJobStatus(ST_SC_RUN, luggNo, ref rtn);
                    if (ok)
                    {
                        _pBdb.Commit();
                        m_dicPrevSC["SC_" + scNo] = luggNo;
                        MakeMsg_Imp(string.Format("[SCH][재지정] 작업 {0} 재개 완료 → 상태 '{1}'", luggNo, ST_SC_RUN));
                    }
                    else
                    {
                        _pBdb.Rollback();
                        MakeMsg_Error(string.Format("[SCH][재지정] 작업 {0} 재개 실패: {1}", luggNo, rtn));
                    }
                }
            }
            catch (Exception ex)
            {
                MakeMsg_Error("[SCH][재지정] ResumeRedirectedJobs 오류: " + ex.Message);
                try { _pBdb.Rollback(); } catch { }
            }
        }

        private void MarkErrorJobStatus()
        {
            try
            {
                string q = "";
                q += CRLF + " SELECT SD.SC_NO                                                        ";
                q += CRLF + "      , SD.ERR_CODE_RD                                                  ";
                q += CRLF + "      , COALESCE(NULLIF(SD.PALLET_ON_VEHICLE_RD,''), NULLIF(SD.ITN_LUGG_FK1,''), NULLIF(SD.LUGG_NO_FK1_RD,'')) AS LUGG ";
                q += CRLF + "   FROM SC_DATA_LGLS SD                                                 ";
                q += CRLF + "  WHERE SD.WH_TYP       = :WH_TYP                                       ";
                q += CRLF + "    AND SD.ERR_CODE_RD IN ('54','0054','58','0058')                     ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                if (DbQry(q) <= 0) return;

                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string scNo = GetVal(dt.Rows[i], "SC_NO");
                    string ec   = (GetVal(dt.Rows[i], "ERR_CODE_RD") ?? "").Trim();
                    string lugg = (GetVal(dt.Rows[i], "LUGG") ?? "").Trim();
                    if (lugg.Length == 0 || lugg == "0" || lugg == "0000") continue;   // 실을 화물이 없으면 귀속할 작업도 없다

                    int nEc; int.TryParse(ec, out nEc);
                    string want = (nEc == 54) ? ST_ERR_DUAL_STORE : ST_ERR_EMPTY_RETR;

                    // 이미 그 상태면 조용히 넘어간다(폴링마다 UPDATE 하지 않도록)
                    string u = "";
                    u += CRLF + " UPDATE JOB_MST                                  ";
                    u += CRLF + "    SET JOB_STATUS  = :NEW_ST                    ";
                    u += CRLF + "      , UPD_DT      = " + DbLang.SYSDATE + "      ";
                    u += CRLF + "      , UPD_USER_ID = '" + OD_USER + "'          ";
                    u += CRLF + "  WHERE WH_TYP      = :WH_TYP                    ";
                    u += CRLF + "    AND LUGG_NO     = :LUGG_NO                   ";
                    u += CRLF + "    AND JOB_STATUS <> :NEW_ST2                   ";
                    // [LGLS 2026-09-05] ★재지정 지시를 덮어쓰지 않는다★
                    //   상위가 재지정(R 전문)을 내리면 HOST_TASK 가 상태를 06/05 로 바꾼다. 그런데 크레인 에러는
                    //   ★새 지시를 받아야 비로소 해제★ 되므로, 그 사이 이 함수가 08/07 로 되돌리면
                    //   ResumeRedirectedJobs 가 재개할 대상을 영영 찾지 못한다(실측 : 작업 4756 이 08 에 고착).
                    u += CRLF + "    AND JOB_STATUS NOT IN ('" + ST_RETRY_DUAL_STORE + "','" + ST_RETRY_EMPTY_RETR + "') ";
                    _pBdb.mComMain.CommandType = CommandType.Text;
                    _pBdb.mComMain.Parameters.Clear();
                    _pBdb.mComMain.Parameters.Add("NEW_ST",  DbLang.VARCHAR).Value = want;
                    _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                    _pBdb.mComMain.Parameters.Add("LUGG_NO", DbLang.VARCHAR).Value = lugg;
                    _pBdb.mComMain.Parameters.Add("NEW_ST2", DbLang.VARCHAR).Value = want;
                    int n = DbNonQry(u);
                    if (n > 0)
                        MakeMsg_Imp(string.Format("[SCH][ERR] S/C #{0} {1} (코드 {2}) - 작업 {3} 상태 → '{4}'",
                            scNo, (nEc == 54) ? "이중입고" : "공출고", ec, lugg, want));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][ERR] MarkErrorJobStatus 오류: " + ex.Message); }
        }

        private void MonitorAlarm()
        {
            try
            {
                string strSqlAlmCv = "";
                strSqlAlmCv += CRLF + " SELECT MC_NO AS EQ, ERROR_CODE AS EC          ";
                strSqlAlmCv += CRLF + "   FROM CV_DATA                                ";
                strSqlAlmCv += CRLF + "  WHERE WH_TYP     = :WH_TYP                   ";
                strSqlAlmCv += CRLF + "    AND ERROR_CODE IS NOT NULL                 ";
                strSqlAlmCv += CRLF + "    AND ERROR_CODE NOT IN ('0','00','0000','') ";
                CheckAlarm("CV", strSqlAlmCv);

                string strSqlAlmSc = "";
                strSqlAlmSc += CRLF + " SELECT SC_NO AS EQ, ERR_CODE_RD AS EC          ";
                strSqlAlmSc += CRLF + "   FROM SC_DATA_LGLS                                 ";
                strSqlAlmSc += CRLF + "  WHERE WH_TYP      = :WH_TYP                   ";
                strSqlAlmSc += CRLF + "    AND ERR_CODE_RD IS NOT NULL                 ";
                strSqlAlmSc += CRLF + "    AND ERR_CODE_RD NOT IN ('0','00','0000','') ";
                CheckAlarm("SC", strSqlAlmSc);

                string strSqlAlmRgv = "";
                strSqlAlmRgv += CRLF + " SELECT RTV_NO AS EQ, ERR_CODE_RD AS EC         ";
                strSqlAlmRgv += CRLF + "   FROM RTV_DATA_LGLS                                ";
                strSqlAlmRgv += CRLF + "  WHERE WH_TYP      = :WH_TYP                   ";
                strSqlAlmRgv += CRLF + "    AND ERR_CODE_RD IS NOT NULL                 ";
                strSqlAlmRgv += CRLF + "    AND ERR_CODE_RD NOT IN ('0','00','0000','') ";
                CheckAlarm("RGV", strSqlAlmRgv);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][ALM] MonitorAlarm 오류: " + ex.Message); }
        }
        private void CheckAlarm(string kind, string sql)
        {
            try
            {
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                int n = DbQry(sql);
                if (n <= 0) return;
                DataTable dt = _pBdb.mDtMain.Copy();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string eq = GetVal(dt.Rows[i], "EQ");
                    string ec = GetVal(dt.Rows[i], "EC");
                    string key = kind + "_" + eq;
                    if (m_dicAlarm.ContainsKey(key) && m_dicAlarm[key] == ec) continue; // 신규만
                    m_dicAlarm[key] = ec;
                    MakeMsg_Error(string.Format("[SCH][ALM] {0} #{1} 알람 발생 (코드:{2})", kind, eq, ec));
                }
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][ALM] CheckAlarm(" + kind + ") 오류: " + ex.Message); }
        }
        #endregion

        // ─────────────────────────────────────────────────────────────────
        // DB Helper
        // ─────────────────────────────────────────────────────────────────
        #region DB Helper

        /// <summary>
        /// JOB_MST.JOB_STATUS 상태 변경 (기존 cThread_*.UPDATE_JOB_DATA 의 핵심부)
        /// </summary>
        private bool UpdateJobStatus(string strStatus, string strLuggNo, ref string strRtn)
        {
            try
            {
                string strSql = "";
                strSql += CRLF + " UPDATE JOB_MST                       ";
                strSql += CRLF + "    SET JOB_STATUS  = :JOB_STATUS     ";
                strSql += CRLF + "      , UPD_DT      = " + DbLang.SYSDATE + " ";
                strSql += CRLF + "      , UPD_USER_ID = '" + OD_USER + "' ";
                strSql += CRLF + "  WHERE WH_TYP      = :WH_TYP         ";
                strSql += CRLF + "    AND LUGG_NO     = :LUGG_NO        ";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("JOB_STATUS", DbLang.VARCHAR).Value = strStatus;
                _pBdb.mComMain.Parameters.Add("WH_TYP",     DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LUGG_NO",    DbLang.VARCHAR).Value = strLuggNo;
                int n = DbNonQry(strSql);
                if (n < 0) { strRtn += "JOB_MST 상태변경 오류:" + _pBdb.ErrMsg; return false; }
                if (n == 0) { strRtn += "변경할 JOB_MST 작업이 없음(LUGG_NO:" + strLuggNo + ")"; return false; }
                return true;
            }
            catch (Exception ex) { strRtn += ex.Message; return false; }
        }

        /// <summary>[LGLS] SC 작업 완료 시 해당 작업번호의 SC od(LUGG_NO_FK1_OD) 잔류 클리어</summary>
        private void ClearScOd(string strLuggNo)
        {
            try
            {
                // [LGLS 2026-08-23] 지시값만 지우면 관측 잔류값(ITN_LUGG_FK1 / PALLET_ON_VEHICLE_RD)이
                //   그대로 남아, 다음 작업을 받은 크레인에 **이전 작업번호** 가 붙어 보이고 작업색도
                //   엉킨다(4호기가 1492 를 받았는데 1489 가 남아 있던 사례).
                //   완료 시 작업색(JOB_TYP_RD)까지 함께 내린다.
                string strSql = "";
                strSql += CRLF + " UPDATE SC_DATA_LGLS                                   ";
                strSql += CRLF + "    SET LUGG_NO_FK1_OD       = '0000'                  ";
                strSql += CRLF + "      , ITN_LUGG_FK1         = '0'                     ";
                strSql += CRLF + "      , PALLET_ON_VEHICLE_RD = ''                      ";
                strSql += CRLF + "      , JOB_TYP_RD           = '0'                     ";
                strSql += CRLF + "  WHERE WH_TYP = :WH_TYP                               ";
                strSql += CRLF + "    AND (   LUGG_NO_FK1_OD       = :LUGG_NO            ";
                strSql += CRLF + "         OR ITN_LUGG_FK1         = :LUGG_NO            ";
                strSql += CRLF + "         OR PALLET_ON_VEHICLE_RD = :LUGG_NO )          ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LUGG_NO", DbLang.VARCHAR).Value = strLuggNo;
                DbNonQry(strSql);
            }
            catch { }
        }

        /// <summary>
        /// [LGLS 2026-08-23] 완료된 작업의 CV 지시값 정리.
        ///   LUGG_NO_OD 가 남아 있으면 WCS_TASK_CV 가 그 값을 R 트래킹에 다시 써서,
        ///   설비가 이미 비운 출고대에 작업번호가 되살아난다(작업 1401 사례 - 8시간 정체).
        /// </summary>

        /// <summary>
        /// [LGLS 2026-08-31] ★잔재의 근원이던 비대칭을 바로잡았다★
        ///   종전에는 TRACKING_WRITE_YN='N' 이었다. 그러면 DB 의 LUGG_NO_OD 만 지워지고
        ///   ★PLC R 트래킹 영역은 그 번호를 그대로 들고 있다★. 화면의 번호는 그 R 영역을
        ///   되읽은 LUGG_NO_RD 이므로, "지시값은 비었는데 번호는 남아 있는" 상태가 된다.
        ///   (실측 : 106 이 od=0000 인데 rd=9001 로 1분 넘게 남았다. 작업번호로 찾는
        ///    해제문이 0건 UPDATE 로 헛돈 이유도 이것이다.)
        ///   지우려면 PLC 까지 0 을 밀어야 한다 - 'Y' 로 두어 CvTrackingWrite 가 쓰게 한다.
        /// </summary>
        private void ClearCvOd(string strLuggNo)
        {
            try
            {
                string strSql = "";
                strSql += CRLF + " UPDATE CV_DATA                              ";
                strSql += CRLF + "    SET LUGG_NO_OD = '0000'                  ";
                strSql += CRLF + "      , TRACKING_WRITE_YN = 'N'              ";
                strSql += CRLF + "  WHERE WH_TYP     = :WH_TYP                 ";
                strSql += CRLF + "    AND LUGG_NO_OD = :LUGG_NO                ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP",  DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("LUGG_NO", DbLang.VARCHAR).Value = Cap(strLuggNo, 4);
                DbNonQry(strSql);
            }
            catch { }
        }

        /// <summary>
        /// CV_DATA 명령 발행 (기존 cThread_CV.UPDATE_CV_DATA)
        ///   _OD 컬럼 기록 + OD_RQ_YN='Y'. 유휴(OD_RQ_YN='N') + 무에러 행만 대상.
        /// </summary>
        private bool UpdateCvData(string strJobTyp, string strDestPos, string strLuggNo,
                                  string strPlcNo, string strTrackNo,
                                  ref string strRtn)
        {
            try
            {
                // 실제 CV_DATA 명령(_OD) 컬럼만 사용: JOB_TYP_OD, DEST_POS_OD, LUGG_NO_OD, OD_RQ_YN
                //   방향(입고0/출고1, D0310)은 통신 Task가 JOB_TYP 기반으로 PLC에 기록.
                string strSql = "";
                strSql += CRLF + " UPDATE CV_DATA                                 ";
                strSql += CRLF + "    SET JOB_TYP_OD  = :JOB_TYP_OD               ";
                strSql += CRLF + "      , DEST_POS_OD = :DEST_POS_OD              ";
                strSql += CRLF + "      , LUGG_NO_OD  = :LUGG_NO_OD               ";
                strSql += CRLF + "      , TRACKING_WRITE_YN = 'Y'                 ";   // [LGLS] WCS_TASK_CV가 트래킹(JOB번호)을 PLC R영역에 기록하도록 지시
                strSql += CRLF + "      , OD_RQ_YN    = 'Y'                       ";
                strSql += CRLF + "      , OD_USER_ID  = '" + OD_USER + "'         ";
                strSql += CRLF + "      , OD_UPD_DT   = " + DbLang.SYSDATE + "     ";
                strSql += CRLF + "  WHERE WH_TYP      = :WH_TYP                   ";
                strSql += CRLF + "    AND PLC_NO      = :PLC_NO                   ";
                strSql += CRLF + "    AND MC_NO       = :TRACK_NO                 ";
                strSql += CRLF + "    AND OD_RQ_YN    = 'N'                       ";
                strSql += CRLF + "    AND (ERROR_CODE = '0' OR ERROR_CODE = '0000' OR ERROR_CODE IS NULL)";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("JOB_TYP_OD",  DbLang.VARCHAR).Value = strJobTyp;
                _pBdb.mComMain.Parameters.Add("DEST_POS_OD", DbLang.VARCHAR).Value = strDestPos;
                _pBdb.mComMain.Parameters.Add("LUGG_NO_OD",  DbLang.VARCHAR).Value = strLuggNo;
                _pBdb.mComMain.Parameters.Add("WH_TYP",      DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("PLC_NO",      DbLang.VARCHAR).Value = strPlcNo;
                _pBdb.mComMain.Parameters.Add("TRACK_NO",    DbLang.VARCHAR).Value = strTrackNo;
                int n = DbNonQry(strSql);
                if (n < 0) { strRtn += "CV_DATA 명령 오류:" + _pBdb.ErrMsg; return false; }
                if (n == 0) { strRtn += "발행할 CV_DATA 가 없음(TRACK_NO:" + strTrackNo + ")"; return false; }
                return true;
            }
            catch (Exception ex) { strRtn += ex.Message; return false; }
        }

        // [LGLS] 컬럼 폭 초과 방지(문자열 잘림 오류 회피)
        private static string Cap(string v, int n) { if (v == null) return ""; return v.Length > n ? v.Substring(0, n) : v; }

        // [LGLS 2026-08-01] SC 포크 출입방향(FORKPOS_FK1_RD) 산출/반영.
        //   원본 ECS 소스·백업 DB·PLC 사양서 어디에도 포크 위치 산출 로직이 없어(컬럼과 COMMON_CODE 'SC_FORKPOS'
        //   정의만 존재) 여기서 직접 계산한다. 현행 체인에서 SC 관측치는 WCS_TASK_CV VehThread 가 미러하지만
        //   FORKPOS_FK1_RD 는 아무도 쓰지 않으므로, 크레인 작업색(JOB_TYP_RD)과 동일한 집합 UPDATE 방식으로 유지한다.
        //   기준(좌/우 판정은 한 가지 규칙): **크레인이 이때 포크를 뻗는 상대편의 라인번호**가 홀수면 좌출, 짝수면 우출.
        //     - 랙에 할 때(입고 하역 / 출고 픽업) : 랙 셀의 뱅크번호. SC_DEF_INF.AREA 가 담당 뱅크쌍을
        //       "홀수,짝수"(901=01,02 … 905=09,10)로 정의하고 레이아웃상 홀수 뱅크가 진행방향 좌측이다.
        //     - 작업대(H/S)에 할 때(입고 픽업 / 출고 하역) : 그 H/S 가 붙어 있는 **라인 C/V 번호**.
        //       레이아웃상 랙 n행과 C/V#n 이 같은 쪽이므로 뱅크와 동일한 홀짝 규칙이 그대로 성립한다.
        //       입고 H/S = RgvDropTrack(SC)+1 → C/V#2,4,6,8,10(전부 짝수=우출)
        //       출고 H/S = RgvOutDropTrack(SC) → C/V#2,3,5,7,9 (SC1 만 C/V#2 특례로 우출, 나머지 좌출)
        //   단계 구분: 지시(21)=출발지에서 집는 단계, 구동중(25)=도착지에 내려놓는 단계.
        //     입고 21=입고 H/S 픽업 / 25=랙 하역, 출고 21=랙 픽업 / 25=출고 H/S 하역.
        //   값: 0=센터(대기/완료/판정불가), 1=좌출, 2=우출.
        private void SyncScForkPos()
        {
            try
            {
                // 랙 셀 위치 = 입고면 도착지, 출고면 출발지. "BB-LLL-DD" 의 앞 2자리가 뱅크.
                const string BANK = "LEFT(CASE WHEN JM.JOB_TYP IN ('1','11') THEN JM.DEST_LOCATION ELSE JM.START_LOCATION END, 2)";
                const string RACKSIDE = "(CASE WHEN " + BANK + " BETWEEN '01' AND '15'"
                                      + " THEN (CASE WHEN CAST(" + BANK + " AS int) % 2 = 1 THEN '1' ELSE '2' END) ELSE '0' END)";
                const string SCNO = "(CASE WHEN JM.JOB_TYP IN ('1','11') THEN JM.DEST_POS ELSE JM.START_POS END)";

                // H/S 라인 C/V 번호 = (트랙번호 - 100 + 1) / 2 (정수나눗셈). 크레인별 H/S 트랙은 C# 매핑에서 생성.
                string caseIn = "(CASE";     // 입고 H/S (SC 가 화물을 집는 작업대)
                string caseOut = "(CASE";    // 출고 H/S (SC 가 화물을 내려놓는 작업대)
                foreach (KeyValuePair<string, string> kv in m_dicRgvDrop)
                {
                    int nDrop; if (!int.TryParse(kv.Value, out nDrop)) continue;
                    caseIn += " WHEN " + SCNO + " = '" + kv.Key + "' THEN '" + LineSide(nDrop + 1) + "'";
                }
                foreach (KeyValuePair<string, string> kv in m_dicRgvOutDrop)
                {
                    int nOut; if (!int.TryParse(kv.Value, out nOut)) continue;
                    caseOut += " WHEN " + SCNO + " = '" + kv.Key + "' THEN '" + LineSide(nOut) + "'";
                }
                caseIn += " ELSE '0' END)";
                caseOut += " ELSE '0' END)";

                //   입고(1/11) : 21=입고 H/S 픽업, 그 외(25)=랙 하역
                //   출고(2/12) : 21=랙 픽업,       그 외(25)=출고 H/S 하역
                string side = "(CASE WHEN JM.JOB_TYP IN ('1','11')"
                            + " THEN (CASE WHEN JM.JOB_STATUS = '21' THEN " + caseIn + " ELSE " + RACKSIDE + " END)"
                            + " ELSE (CASE WHEN JM.JOB_STATUS = '21' THEN " + RACKSIDE + " ELSE " + caseOut + " END) END)";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                string q1 = "";
                q1 += CRLF + " UPDATE SD                                                  ";
                q1 += CRLF + "    SET FORKPOS_FK1_RD = " + side;
                q1 += CRLF + "   FROM SC_DATA_LGLS SD                                     ";
                q1 += CRLF + "  INNER JOIN JOB_MST JM                                     ";
                q1 += CRLF + "     ON JM.WH_TYP = SD.WH_TYP                               ";
                q1 += CRLF + "    AND SD.SC_NO  = " + SCNO;
                q1 += CRLF + "  WHERE SD.WH_TYP = :WH_TYP                                 ";
                q1 += CRLF + "    AND JM.JOB_STATUS IN ('25')                        ";
                q1 += CRLF + "    AND ISNULL(SD.FORKPOS_FK1_RD,'0') <> " + side;
                DbNonQry(q1);

                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                string q2 = "";
                q2 += CRLF + " UPDATE SD                                                  ";
                q2 += CRLF + "    SET FORKPOS_FK1_RD = '0'                                ";
                q2 += CRLF + "   FROM SC_DATA_LGLS SD                                     ";
                q2 += CRLF + "  WHERE SD.WH_TYP = :WH_TYP                                 ";
                q2 += CRLF + "    AND ISNULL(SD.FORKPOS_FK1_RD,'0') <> '0'                ";
                q2 += CRLF + "    AND NOT EXISTS (SELECT 1                                ";
                q2 += CRLF + "                      FROM JOB_MST JM                       ";
                q2 += CRLF + "                     WHERE JM.WH_TYP = SD.WH_TYP            ";
                q2 += CRLF + "                       AND JM.JOB_STATUS IN ('25')     ";
                q2 += CRLF + "                       AND SD.SC_NO = " + SCNO + ") ";
                DbNonQry(q2);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][SC] SyncScForkPos 오류: " + ex.Message); }
        }

        // [LGLS] 라인 트랙번호 → 그 라인의 C/V 번호(=같은 쪽 랙 행번호) → 좌출(홀수)/우출(짝수).
        //   트랙 101,102=C/V#1 … 119,120=C/V#10 이므로 C/V번호 = (트랙-100+1)/2.
        private static string LineSide(int nTrack)
        {
            int nCv = (nTrack - 100 + 1) / 2;
            if (nCv <= 0) return "0";
            return (nCv % 2 == 1) ? "1" : "2";
        }


        // [LGLS] SC_DATA_LGLS Vehicle 컬럼 갱신(대화상자 표시용). 대체 체인에 SC PLC TASK프로그램이 없어
        //        AutoRunSC 시뮬레이션이 원본 ECS Vehicle 관측치를 SC_DATA_LGLS 에 직접 반영한다.
        //        status(1=IDLE,2=RUN), req/ack=transfer_request_od/transfer_ack_rd, completeLoc=완료위치.
        private void UpdateScVehicle(string scNo, string status, string lugg, string fromPos, string toPos,
                                     string loadComplete, string unloadComplete, string req, string ack, string completeLoc = "", string jobTyp = "0")
        {
            try
            {
                string s = "";
                s += CRLF + " UPDATE SC_DATA_LGLS SET SUBSYSTEM_STATUS_RD = :ST                 ";
                s += CRLF + "      , PALLET_ON_VEHICLE_RD = :LUGG                          ";
                s += CRLF + "      , JOB_TYP_RD = :JOBTYP                                ";
                s += CRLF + "      , SENSOR_FK_RD = :SEN                                 ";
                s += CRLF + "      , ITN_LUGG_FK1 = :ITNL                                ";
                s += CRLF + "      , FROM_01_OD = :FROM01, TO_01_OD = :TO01                ";
                s += CRLF + "      , LOAD_COMPLETE_RD = :LC, LOAD_COMPLETE_ACK_OD = :LC    ";
                s += CRLF + "      , UNLOAD_COMPLETE_RD = :UC, UNLOAD_COMPLETE_ACK_OD = :UC";
                s += CRLF + "      , TRANSFER_REQUEST_OD = :REQ, TRANSFER_ACK_RD = :ACK    ";
                s += CRLF + "      , TRANSFER_COMPLETE_LOCATION_01_RD = :CLOC              ";
                s += CRLF + "      , LOCATION_01_RD = :LOC1, LOCATION_02_RD = :LOC2, LOCATION_03_RD = :LOC3 ";
                s += CRLF + "      , READ_UPD_DT = " + DbLang.SYSDATE + "                  ";
                s += CRLF + "  WHERE WH_TYP = :WH_TYP AND SC_NO = :SC_NO                   ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("ST",     DbLang.VARCHAR).Value = status;
                _pBdb.mComMain.Parameters.Add("JOBTYP", DbLang.VARCHAR).Value = jobTyp;
                _pBdb.mComMain.Parameters.Add("LUGG",   DbLang.VARCHAR).Value = Cap(lugg, 4);
                string senFk = (loadComplete == "1" && unloadComplete != "1") ? "1" : "0";  // [LGLS] 화물 코너마커
                string itnLugg = (!string.IsNullOrEmpty(lugg) && lugg != "0") ? Cap(lugg, 4) : "0";  // [LGLS] BLUE 운반레일
                _pBdb.mComMain.Parameters.Add("SEN",  DbLang.VARCHAR).Value = senFk;
                _pBdb.mComMain.Parameters.Add("ITNL", DbLang.VARCHAR).Value = itnLugg;
                _pBdb.mComMain.Parameters.Add("FROM01", DbLang.VARCHAR).Value = Cap(fromPos, 2);
                _pBdb.mComMain.Parameters.Add("TO01",   DbLang.VARCHAR).Value = Cap(toPos, 2);
                _pBdb.mComMain.Parameters.Add("LC",     DbLang.VARCHAR).Value = loadComplete;
                _pBdb.mComMain.Parameters.Add("UC",     DbLang.VARCHAR).Value = unloadComplete;
                _pBdb.mComMain.Parameters.Add("REQ",    DbLang.VARCHAR).Value = req;
                _pBdb.mComMain.Parameters.Add("ACK",    DbLang.VARCHAR).Value = ack;
                _pBdb.mComMain.Parameters.Add("CLOC",   DbLang.VARCHAR).Value = Cap(completeLoc, 2);
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                // [LGLS] 현재위치: toPos 를 bay/location 으로 사상 (크레인 위치 표현용)
                string posBay = Cap(toPos, 2);
                int nBay; int.TryParse(posBay, out nBay); if (nBay < 0) nBay = 0; else if (nBay > 15) nBay = 15;
                _pBdb.mComMain.Parameters.Add("SC_NO",  DbLang.VARCHAR).Value = scNo;
                if (senFk == "1") { m_dicCraneTgt["S" + scNo] = (status == "1" ? 0 : nBay); }  // [LGLS] 화물 실음: 목적지로 스텝 이동
                else { int _cur = m_dicCraneCur.ContainsKey("S" + scNo) ? m_dicCraneCur["S" + scNo] : 0; m_dicCraneTgt["S" + scNo] = _cur; }  // [LGLS] 빈차: 현재 위치 고정(데이터 없이 이동 금지) — 픽업은 현재 위치에서
                _pBdb.mComMain.Parameters.Add("LOC1",   DbLang.VARCHAR).Value = Cap(toPos, 2);
                _pBdb.mComMain.Parameters.Add("LOC2",   DbLang.VARCHAR).Value = nBay.ToString();
                _pBdb.mComMain.Parameters.Add("LOC3",   DbLang.VARCHAR).Value = "0";
                DbNonQry(s);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][SC] UpdateScVehicle 오류: " + ex.Message); }
        }

        // [LGLS] RTV_DATA_LGLS(RGV) Vehicle 컬럼 갱신(모니터 애니메이션용). SC 와 마찬가지로 RGV PLC
        //        TASK프로그램이 없어 AutoRunRGV 시뮬레이션이 원본 ECS 관측치를 RTV_DATA_LGLS 에 직접 반영한다.
        //        모니터 표시 규칙 :
        //          - 입고(STO) 색상 = JOB_TYP_OD='1' + AUTO_MODE_RD='1' + SUBSYSTEM_STATUS_RD='2'
        //          - 화물 코너마커  = SENSOR_RTV_RD='1'
        //          - BLUE 운반레일  = LUGG_OD != '0'
        //          - 화면 위치      = POS_H_RD 셀(0-15)
        //        status(1=IDLE, 2=RUN). RTV_DATA_LGLS 에는 READ_UPD_DT/OD_UPD_DT 갱신 컬럼을 두지 않는다.
        private void UpdateRtvVehicle(string rtvNo, string status, string luggPallet, string posCell,
                                     string jobTyp, string sensor, string luggOd,
                                     string departTrack = "", string arriveTrack = "", bool allowEmptyMove = false)
        {
            try
            {
                string s = "";
                s += CRLF + " UPDATE RTV_DATA_LGLS SET SUBSYSTEM_STATUS_RD = :ST              ";
                s += CRLF + "      , PALLET_ON_VEHICLE_RD = :PALLET                      ";
                s += CRLF + "      , SENSOR_RTV_RD        = :SENSOR                      ";
                s += CRLF + "      , JOB_TYP_OD           = :JOBTYP                      ";
                s += CRLF + "      , LUGG_OD              = :LUGGOD                       ";
                s += CRLF + "      , AUTO_MODE_RD         = '1'                          ";
                if (departTrack != "") s += CRLF + "      , DEPART_TRACK = :DEPART       ";
                if (arriveTrack != "") s += CRLF + "      , ARRIVE_TRACK = :ARRIVE       ";
                s += CRLF + "  WHERE WH_TYP = :WH_TYP AND RTV_NO = :RTV_NO               ";
                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("ST",     DbLang.VARCHAR).Value = status;
                if (sensor == "1" || allowEmptyMove) { int _rtvtgt = 0; int.TryParse(posCell, out _rtvtgt); m_dicCraneTgt["R" + rtvNo] = _rtvtgt; }  // [LGLS] 화물 실음(또는 입고대 접근 이동): 목적지로 스텝 이동
                else { int _cur = m_dicCraneCur.ContainsKey("R" + rtvNo) ? m_dicCraneCur["R" + rtvNo] : 0; m_dicCraneTgt["R" + rtvNo] = _cur; }  // [LGLS] 빈차: 현재 위치 고정
                _pBdb.mComMain.Parameters.Add("PALLET", DbLang.VARCHAR).Value = Cap(luggPallet, 4);
                _pBdb.mComMain.Parameters.Add("SENSOR", DbLang.VARCHAR).Value = sensor;
                _pBdb.mComMain.Parameters.Add("JOBTYP", DbLang.VARCHAR).Value = jobTyp;
                _pBdb.mComMain.Parameters.Add("LUGGOD", DbLang.VARCHAR).Value = Cap(luggOd, 4);
                if (departTrack != "") _pBdb.mComMain.Parameters.Add("DEPART", DbLang.VARCHAR).Value = Cap(departTrack, 4);
                if (arriveTrack != "") _pBdb.mComMain.Parameters.Add("ARRIVE", DbLang.VARCHAR).Value = Cap(arriveTrack, 4);
                _pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("RTV_NO", DbLang.VARCHAR).Value = rtvNo;
                DbNonQry(s);
            }
            catch (Exception ex) { MakeMsg_Error("[SCH][RGV] UpdateRtvVehicle 오류: " + ex.Message); }
        }

        /// <summary>
        /// SC_DATA_LGLS 이송 명령 발행 (단순 이송 - CELL 미사용 현장).
        ///   ★TODO(현장값 확인): USER_COMMAND_OD / WRITE_FLAG_OD / 위치(BANK/BAY/LEVEL/HSPOS) 등
        ///     실제 SC_DATA_LGLS DDL 및 PLC 명령 사양에 맞춰 _OD 컬럼을 보완할 것.
        ///     현재는 확신 가능한 핸드셰이크 컬럼(JOB_TYP_OD, LUGG_NO_FK1_OD, OD_RQ_YN)만 기록.
        /// </summary>
        // [LGLS 2026-07-21] 물리 이관: Vehicle 반송지시 기록 — EQP_TASK VehThread 가 PLC 로 반영한다.
        //   (From/To 는 2자리 3필드: 포트=00/00/pp, 셀=통로/Bay/Level)
        private bool UpdateScData(string strScNo, string strJobTyp, string strLuggNo,
                                  string f1, string f2, string f3, string t1, string t2, string t3, ref string strRtn,
                                  bool bAllowRedirectErr = false)
        {
            try
            {
                string strSql = "";
                strSql += CRLF + " UPDATE SC_DATA_LGLS                                 ";
                strSql += CRLF + "    SET JOB_TYP_OD     = :JOB_TYP_OD            ";
                // [LGLS 2026-08-23] 작업색 근거. 실경로에서는 이 값을 채우는 곳이 없어 화면의 크레인이
                //   색 없이 움직였다(구 경로 UpdateScVehicle 만 쓰던 컬럼). 지시와 함께 세우고
                //   완료 시 ClearScOd 가 0 으로 내린다.
                strSql += CRLF + "      , JOB_TYP_RD   = :JOB_TYP_OD                ";
                // 이전 작업의 관측 잔류가 남아 새 작업에 붙어 보이지 않게 지시 시점에 함께 비운다.
                strSql += CRLF + "      , ITN_LUGG_FK1 = '0'                        ";
                strSql += CRLF + "      , PALLET_ON_VEHICLE_RD = ''                 ";
                strSql += CRLF + "      , LUGG_NO_FK1_OD = :LUGG_NO_FK1_OD        ";
                strSql += CRLF + "      , USE_FK_OD      = '1'                    ";   // FK1 사용 (TWIN 미사용 가정)
                strSql += CRLF + "      , PALLET_ID_OD   = :PALLET_ID_OD          ";   // Vehicle 반송 JOB
                strSql += CRLF + "      , FROM_01_OD = :VEH_FROM_A, FROM_02_OD = :VEH_FROM_B, FROM_03_OD = :VEH_FROM_C ";
                strSql += CRLF + "      , TO_01_OD   = :VEH_TO_A, TO_02_OD   = :VEH_TO_B, TO_03_OD   = :VEH_TO_C ";
                strSql += CRLF + "      , TRANSFER_REQUEST_OD = 'Y'               ";
                strSql += CRLF + "      , COMPLETE_RD    = '0'                    ";
                strSql += CRLF + "      , OD_RQ_YN       = 'Y'                    ";
                strSql += CRLF + "      , OD_USER_ID     = '" + OD_USER + "'      ";
                strSql += CRLF + "      , OD_UPD_DT      = " + DbLang.SYSDATE + " ";
                strSql += CRLF + "  WHERE WH_TYP         = :WH_TYP                ";
                strSql += CRLF + "    AND SC_NO          = :SC_NO                 ";
                strSql += CRLF + "    AND OD_RQ_YN       = 'N'                    ";
                // [LGLS 2026-09-05] 평소에는 무에러일 때만 지시한다. 다만 ★재지정★ 은 예외다 :
                //   이중입고(54/55)·공출고(58/59)는 크레인이 새 지시를 받아야 비로소 에러가 풀리므로,
                //   에러가 남아 있다는 이유로 재지정 지시를 막으면 영영 회복하지 못한다(닭과 달걀).
                if (bAllowRedirectErr)
                    strSql += CRLF + "    AND (ERR_CODE_RD IN ('0','00','0000','') OR ERR_CODE_RD IS NULL OR ERR_CODE_RD IN ('54','0054','55','0055','58','0058','59','0059')) ";
                else
                    strSql += CRLF + "    AND ERR_CODE_RD    = '0000'                 ";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("JOB_TYP_OD",     DbLang.VARCHAR).Value = strJobTyp;
                _pBdb.mComMain.Parameters.Add("LUGG_NO_FK1_OD", DbLang.VARCHAR).Value = strLuggNo;
                _pBdb.mComMain.Parameters.Add("PALLET_ID_OD",   DbLang.VARCHAR).Value = strLuggNo;
                _pBdb.mComMain.Parameters.Add("VEH_FROM_A", DbLang.VARCHAR).Value = f1;
                _pBdb.mComMain.Parameters.Add("VEH_FROM_B", DbLang.VARCHAR).Value = f2;
                _pBdb.mComMain.Parameters.Add("VEH_FROM_C", DbLang.VARCHAR).Value = f3;
                _pBdb.mComMain.Parameters.Add("VEH_TO_A", DbLang.VARCHAR).Value = t1;
                _pBdb.mComMain.Parameters.Add("VEH_TO_B", DbLang.VARCHAR).Value = t2;
                _pBdb.mComMain.Parameters.Add("VEH_TO_C", DbLang.VARCHAR).Value = t3;
                _pBdb.mComMain.Parameters.Add("WH_TYP",         DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("SC_NO",          DbLang.VARCHAR).Value = strScNo;
                int n = DbNonQry(strSql);
                if (n < 0) { strRtn += "SC_DATA_LGLS 명령 오류:" + _pBdb.ErrMsg; return false; }
                if (n == 0) { strRtn += "발행할 SC_DATA_LGLS 가 없음(SC_NO:" + strScNo + ")"; return false; }
                return true;
            }
            catch (Exception ex) { strRtn += ex.Message; return false; }
        }

        // [LGLS 2026-07-21] Vehicle 좌표 인코딩 헬퍼 (VehicleSim/구 ECS ECSDispatcher 규약)
        /// <summary>포트 좌표: ("00","00",포트 2자리). 트랙 1xx → 포트 = 트랙-100.</summary>
        private static void VehPortLoc(string track, out string a, out string b, out string c)
        {
            int t; int.TryParse(track, out t);
            a = "00"; b = "00"; c = (t % 100).ToString("00");
        }
        /// <summary>셀 좌표: LOCATION "BB-bbb-LL" → (통로: 홀수뱅크=01/짝수뱅크=02, Bay 2자리, Level 2자리)</summary>
        private static void VehCellLoc(string loc, out string a, out string b, out string c)
        {
            int bank = 0, bay = 0, lev = 0;
            string[] p = (loc ?? "").Split('-');
            if (p.Length >= 3) { int.TryParse(p[0], out bank); int.TryParse(p[1], out bay); int.TryParse(p[2], out lev); }
            a = (bank % 2 == 1) ? "01" : "02";
            b = (bay % 100).ToString("00");
            c = (lev % 100).ToString("00");
        }

        /// <summary>
        /// RTV_DATA_LGLS(RGV) 이송 명령 발행.
        ///   ★TODO(현장값 확인): RTV_DATA_LGLS 의 _OD 명령 컬럼이 기존 코드에 정의가 없어
        ///     SC_DATA_LGLS 패턴으로 작성. 실제 RTV_DATA_LGLS DDL/PLC 사양에 맞춰 보완할 것.
        /// </summary>
        private bool UpdateRtvData(string strRtvNo, string strJobTyp, string strLuggNo,
                                   string strStartPos, string strDestPos, ref string strRtn)
        {
            try
            {
                // 실제 RTV_DATA_LGLS 명령(_OD) 컬럼: JOB_TYP_OD, LUGG_OD, RTV_PASSCV_OD(경유/출발 CV),
                //   RTV_DEST_OD(목적지), DEPART_TRACK, ARRIVE_TRACK, OD_RQ_YN.  (OD_UPD_DT 컬럼 없음)
                //   [슬라이드16-17] Transfer Command Data(From/To/JobNo) 매핑.
                // [LGLS 2026-07-21] 물리 이관: Vehicle 반송지시 기록 — From/To 는 트랙→포트 좌표.
                //   strStartPos/strDestPos 에는 호출부가 이미 픽업/드롭 "트랙"을 넣는다.
                string f1, f2, f3, t1, t2, t3;
                VehPortLoc(strStartPos, out f1, out f2, out f3);
                VehPortLoc(strDestPos, out t1, out t2, out t3);

                string strSql = "";
                strSql += CRLF + " UPDATE RTV_DATA_LGLS                                ";
                strSql += CRLF + "    SET JOB_TYP_OD    = :JOB_TYP_OD             ";
                strSql += CRLF + "      , LUGG_OD       = :LUGG_OD                ";
                strSql += CRLF + "      , RTV_PASSCV_OD = :FROM_POS               ";
                strSql += CRLF + "      , RTV_DEST_OD   = :TO_POS                 ";
                strSql += CRLF + "      , DEPART_TRACK  = :FROM_POS2              ";
                strSql += CRLF + "      , ARRIVE_TRACK  = :TO_POS2                ";
                strSql += CRLF + "      , PALLET_ID_OD  = :PALLET_ID_OD           ";
                strSql += CRLF + "      , FROM_01_OD = :VEH_FROM_A, FROM_02_OD = :VEH_FROM_B, FROM_03_OD = :VEH_FROM_C ";
                strSql += CRLF + "      , TO_01_OD   = :VEH_TO_A, TO_02_OD   = :VEH_TO_B, TO_03_OD   = :VEH_TO_C ";
                strSql += CRLF + "      , TRANSFER_REQUEST_OD = 'Y'               ";
                strSql += CRLF + "      , COMPLETE_RD   = '0'                     ";
                strSql += CRLF + "      , OD_RQ_YN      = 'Y'                     ";
                strSql += CRLF + "      , OD_USER_ID    = '" + OD_USER + "'       ";
                strSql += CRLF + "  WHERE WH_TYP        = :WH_TYP                 ";
                strSql += CRLF + "    AND RTV_NO        = :RTV_NO                 ";
                strSql += CRLF + "    AND OD_RQ_YN      = 'N'                     ";

                _pBdb.mComMain.CommandType = CommandType.Text;
                _pBdb.mComMain.Parameters.Clear();
                _pBdb.mComMain.Parameters.Add("JOB_TYP_OD", DbLang.VARCHAR).Value = strJobTyp;
                _pBdb.mComMain.Parameters.Add("LUGG_OD",    DbLang.VARCHAR).Value = strLuggNo;
                _pBdb.mComMain.Parameters.Add("FROM_POS",   DbLang.VARCHAR).Value = strStartPos;
                _pBdb.mComMain.Parameters.Add("TO_POS",     DbLang.VARCHAR).Value = strDestPos;
                _pBdb.mComMain.Parameters.Add("FROM_POS2",  DbLang.VARCHAR).Value = strStartPos;
                _pBdb.mComMain.Parameters.Add("TO_POS2",    DbLang.VARCHAR).Value = strDestPos;
                _pBdb.mComMain.Parameters.Add("PALLET_ID_OD", DbLang.VARCHAR).Value = strLuggNo;
                _pBdb.mComMain.Parameters.Add("VEH_FROM_A", DbLang.VARCHAR).Value = f1;
                _pBdb.mComMain.Parameters.Add("VEH_FROM_B", DbLang.VARCHAR).Value = f2;
                _pBdb.mComMain.Parameters.Add("VEH_FROM_C", DbLang.VARCHAR).Value = f3;
                _pBdb.mComMain.Parameters.Add("VEH_TO_A", DbLang.VARCHAR).Value = t1;
                _pBdb.mComMain.Parameters.Add("VEH_TO_B", DbLang.VARCHAR).Value = t2;
                _pBdb.mComMain.Parameters.Add("VEH_TO_C", DbLang.VARCHAR).Value = t3;
                _pBdb.mComMain.Parameters.Add("WH_TYP",     DbLang.VARCHAR).Value = SCH_WH_TYP;
                _pBdb.mComMain.Parameters.Add("RTV_NO",     DbLang.VARCHAR).Value = strRtvNo;
                int n = DbNonQry(strSql);
                if (n < 0) { strRtn += "RTV_DATA_LGLS 명령 오류:" + _pBdb.ErrMsg; return false; }
                if (n == 0) { strRtn += "발행할 RTV_DATA_LGLS 가 없음(RTV_NO:" + strRtvNo + ")"; return false; }
                return true;
            }
            catch (Exception ex) { strRtn += ex.Message; return false; }
        }

        /// <summary>DataRow 값 추출 (null/공백 안전, Trim)</summary>
        private string GetVal(DataRow row, string col)
        {
            if (row[col] == null || row[col] == DBNull.Value) return "";
            return row[col].ToString().Trim();
        }
        #endregion
    }
}
