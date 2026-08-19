using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using Samoh_Lib;
using System.Data;
using System.Data.OleDb;
using log4net;
using log4net.Config;
#if POSTGRESQL
using NpgsqlTypes; // [LGLS] POSTGRESQL 전용
#endif

namespace WCS_TASK_CV
{
	//2014 조형준 메모리에 Cv 상태를 저장한다.
	public class CVData
	{
		private string CvStatHexVal;
		public string CVSTATHEXVAL
		{
			get { return CvStatHexVal; }
			set { CvStatHexVal = value; }
		}
        private string CvPosStatHexVal;
        public string CVPOSSTATHEXVAL
        {
            get { return CvPosStatHexVal; }
            set { CvPosStatHexVal = value; }
        }
		private string CvBcrVal;
		public string CVBCRVAL
		{
			get { return CvBcrVal; }
			set { CvBcrVal = value; }
		}
        private string CvOpMode;
        public string CVOPMODE
        {
            get { return CvOpMode; }
            set { CvOpMode = value; }
        }
		private int CvErrCd;
		public int CVERRCD
		{
			get { return CvErrCd; }
			set { CvErrCd = value; }
		}

        // [V1.1 시나리오] CvStatusScenario 전용 상태 캐시 (변화 감지용)
        public string V11_JOBNO = "";       // R 트래킹 JOB NO (슬롯별, 트랙 키에 저장)
        public string V11_DIR = "";         // 방향 워드 (CV 공통, FrTrack 키에 저장)

        private string AutoModeRd;
        public string AUTO_MODE_RD
        {
            get { return AutoModeRd; }
            set { AutoModeRd = value; }
        }

        private string StoReadyRd;
        public string STO_READY_RD
        {
            get { return StoReadyRd; }
            set { StoReadyRd = value; }
        }
        private string RetReadyRd;
        public string RET_READY_RD
        {
            get { return RetReadyRd; }
            set { RetReadyRd = value; }
        }
        private string StohsReadyRd;
        public string STOHS_READY_RD
        {
            get { return StohsReadyRd; }
            set { StohsReadyRd = value; }
        }
        private string RethsReadyRd;
        public string RETHS_READY_RD
        {
            get { return RethsReadyRd; }
            set { RethsReadyRd = value; }
        }
        private string Sensor0DataRd;
        public string SENSOR0_DATA_RD
        {
            get { return Sensor0DataRd; }
            set { Sensor0DataRd = value; }
        }
        private string Sensor1DataRd;
        public string SENSOR1_DATA_RD
        {
            get { return Sensor1DataRd; }
            set { Sensor1DataRd = value; }
        }
        private string Sensor2DataRd;
        public string SENSOR2_DATA_RD
        {
            get { return Sensor2DataRd; }
            set { Sensor2DataRd = value; }
        }
        private string DeleteTrackRd;
        public string DELETE_TRACK_RD
        {
            get { return DeleteTrackRd; }
            set { DeleteTrackRd = value; }
        }
        private string aTurnYn;
        public string A_TURN_YN
        {
            get { return aTurnYn; }
            set { aTurnYn = value; }
        }
        private string bTurnYn;
        public string B_TURN_YN
        {
            get { return bTurnYn; }
            set { bTurnYn = value; }
        }
        private string RemoteControl;
        public string REMOTE_CONTROL
        {
            get { return RemoteControl; }
            set { RemoteControl = value; }
        }
        private string StockMode;
        public string STOCK_MODE
        {
            get { return StockMode; }
            set { StockMode = value; }
        }
        private string RollMode;
        public string ROLL_MODE
        {
            get { return RollMode; }
            set { RollMode = value; }
        }
        private string ScLockSensor;
        public string SC_LOCK_SENSOR
        {
            get { return ScLockSensor; }
            set { ScLockSensor = value; }
        }
        private string SimMode;
        public string SIM_MODE
        {
            get { return SimMode; }
            set { SimMode = value; }
        }
        private string StoStatus;
        public string STO_STATUS
        {
            get { return StoStatus; }
            set { StoStatus = value; }
        }
        private string RetStatus;
        public string RET_STATUS
        {
            get { return RetStatus; }
            set { RetStatus = value; }
        }

        // M 비트 이벤트 ACK 상태 (PPT 시나리오 Load/Unload Complete 핸드셰이크)
        public bool UnloadComp1Acked = false; // Unload Complete #1 ACK 전송 여부 (RGV→CV 언로드)
        public bool LoadComp1Acked   = false; // Load Complete #1 ACK 전송 여부  (RGV→CV 로드)
        public bool UnloadComp2Acked = false; // Unload Complete #2 ACK 전송 여부 (작업자 반출)
        public bool LoadComp2Acked   = false; // Load Complete #2 ACK 전송 여부  (입고쪽 로드)
        public bool UnloadReq2Sent   = false; // Unload Request #2 발행 여부 (C/V #14 피킹존, ACK base +5)
        public bool AlarmSetAcked    = false; // 알람 세트 ACK 전송 여부
        public bool AlarmRstAcked    = false; // 알람 리셋 ACK 전송 여부

		public CVData()
		{
			CVSTATHEXVAL = "";
			CVBCRVAL = "";
			CVERRCD = 0;
            CVOPMODE = "";
            AUTO_MODE_RD = "";
            STO_READY_RD = "";
            RET_READY_RD = "";
            STOHS_READY_RD = "";
            RETHS_READY_RD = "";
            SENSOR0_DATA_RD = "";
            SENSOR1_DATA_RD = "";
            SENSOR2_DATA_RD = "";
            DELETE_TRACK_RD = "";
            A_TURN_YN = "";
            B_TURN_YN = "";
            REMOTE_CONTROL = "";
            STOCK_MODE = "";
            SC_LOCK_SENSOR = "";
            SIM_MODE = "";
            STO_STATUS = "";
            RET_STATUS = "";
		}
	}

    public class CvThread : maindefine
    {
        #region 변수정의
        #region ㅇㅇ
        #endregion
        private string m_strWh_typ;
        private string m_strEqmt_typ;
        private string m_strPlc_No;
        private string m_strMc_No;
        private string m_strPlcNo;
        private string m_strIp;
        private int m_nCurPort;
        private int m_nFromPort;
        private int m_nToPort;
        private int m_nPortCnt;
        public int m_nCnt;
        public int m_nFrTrackNo;
        public int m_nToTrackNo;
        public int m_nthNo;
        public string m_strRtnMsg;
        public string m_strLogFileNm;
        public string m_strLogMsg;
        public bool m_blHostErrSendYN = false;
        public bool m_blHostSendYN = false;
        public bool m_blSimModeWrite = false;
        public bool m_blRetStatusWrite = false;
        public bool m_blConnectYn = false;

        public string m_strCvNo;
        private string m_strAddress;
        private int m_nAddress;
        private string m_strConnectString;
        private FenetProtocol m_msQPlc;

        // [LGLS 2026-07-28] 꼬인 출고 잔류(JOB_STATUS=15 CV구동중) + 유령 CV 파렛트(JOB_MST 미존재) 정리.
        //   SYS_MAIN [정리] 버튼에서 호출. 진행 중 쿼리와 겹치지 않게 스레드 일시정지 후 실행.
        public string CleanupStuckOutgo()
        {
            // [LGLS] 운영 DB(DB_2: SQL Server LGLS_MCS_IO)에 직접 연결해 정리 — _pBdb(PLC통신용)와 무관하게 확실히 실행
            System.Data.SqlClient.SqlConnection con = null;
            try
            {
                string cs = string.Format("Server={0};Database={1};User Id={2};Password={3};Connection Timeout=6;",
                    cDefApp.GM_DB2_IP, cDefApp.GM_DB2_DATABASE, cDefApp.GM_DB2_USER, cDefApp.GM_DB2_USER_PW);
                con = new System.Data.SqlClient.SqlConnection(cs);
                con.Open();
                int n1, n2, n3, n4;
                using (var c1 = new System.Data.SqlClient.SqlCommand("UPDATE CV_DATA SET LUGG_NO_RD='0' WHERE LUGG_NO_RD IS NOT NULL AND LUGG_NO_RD NOT IN ('0','') AND (NOT EXISTS (SELECT 1 FROM JOB_MST JM WHERE JM.LUGG_NO=CV_DATA.LUGG_NO_RD) OR EXISTS (SELECT 1 FROM JOB_MST JM WHERE JM.LUGG_NO=CV_DATA.LUGG_NO_RD AND JM.JOB_STATUS='15' AND JM.JOB_TYP='2'))", con)) n1 = c1.ExecuteNonQuery();
                using (var c2 = new System.Data.SqlClient.SqlCommand("UPDATE SC_DATA_LGLS SET lugg_no_fk1_od='0000' WHERE lugg_no_fk1_od IN (SELECT LUGG_NO FROM JOB_MST WHERE JOB_STATUS='15' AND JOB_TYP='2')", con)) n2 = c2.ExecuteNonQuery();
                using (var c3 = new System.Data.SqlClient.SqlCommand("DELETE FROM JOB_MST WHERE JOB_STATUS='15' AND JOB_TYP='2'", con)) n3 = c3.ExecuteNonQuery();
                // [LGLS] 화물번호 없는데 센서만 1로 남은 잔류 화물감지 정리 (CvThread 미담당 CV의 센서 갱신 누락분)
                using (var c4 = new System.Data.SqlClient.SqlCommand("UPDATE CV_DATA SET sensor0_data_rd='0', sensor1_data_rd='0', sensor2_data_rd='0' WHERE (lugg_no_rd IS NULL OR lugg_no_rd IN ('0','')) AND (lugg_no_od IS NULL OR lugg_no_od IN ('0','0000','')) AND (sensor0_data_rd='1' OR sensor1_data_rd='1' OR sensor2_data_rd='1')", con)) n4 = c4.ExecuteNonQuery();
                return string.Format("정리 완료 — CV화물:{0}, SC지시:{1}, JOB:{2}, 잔류센서:{3}건 (DB={4}@{5})", n1, n2, n3, n4, cDefApp.GM_DB2_DATABASE, cDefApp.GM_DB2_IP);
            }
            catch (Exception ex) { return "정리 실패: " + ex.Message; }
            finally { if (con != null) con.Close(); }
        }
        public volatile bool m_bPaused = false;   // [LGLS 2026-07-27] [시나리오 테스트] true면 자동 통신(슬롯 순회) 정지 — 시나리오 창의 강제 write와 충돌 방지
        public Thread m_thThread;
        public SYS_MAIN m_frmMain;
        private bool m_bOpen;
        public bool IsOpen { get { return m_bOpen; } set { m_bOpen = value; } } //프로그램 화면표시용.

        //Dictionary 객체를 생성함.
        Dictionary<int, CVData> CvDic = new Dictionary<int, CVData>();
        // [LGLS 2026-07-21] 변경감지 캐시는 DB가 외부에서 바뀌면(수동 리셋/글리치) 영구 불일치가 됨 → 주기적 재동기화
        // [LGLS 2026-07-31] 설비별 카운터로 변경(전역 카운터 폐기).
        //   1소켓 통합 후 CvStatusScenario 는 '사이클당 설비 수'만큼 호출되므로, 전역 30 카운트는
        //   설비 15개 기준 **2 사이클마다 전 트랙 캐시 전체 무효화**가 되어 매번 전 컬럼 재기록(=UPDATE 폭주)을
        //   유발했다(사이클 3.4초 → 11.5초 스파이크의 주원인). 설비별로 30회 방문마다, 그 설비의 트랙만
        //   무효화해 재동기화 비용을 사이클 전체에 분산한다(자가 치유 주기는 설비당 종전과 동일).
        private readonly Dictionary<string, int> m_dicCacheSyncCnt = new Dictionary<string, int>();

        string strSql = "";
        string CRLF = "\r\n";
        int nSelCnt = 0;
        private string _strErrorMsg = "";

        // [LGLS 2026-07-27] 마스터 PLC 1소켓 통합: 한 스레드(1소켓)가 여러 설비(CV)를 순회 처리한다.
        //   주소는 전부 PLC_NO(cvMachineNo) 기반 절대주소라 소켓 1개로 전 설비 접근 가능(조사 확인).
        private class EqpSlot { public string Plc; public string Typ; public int Fr; public int To; public int Cnt; }
        private readonly System.Collections.Generic.List<EqpSlot> m_slots = new System.Collections.Generic.List<EqpSlot>();

        // [LGLS 2026-07-31] 미러 지연 단축용 캐시/스캔.
        //   배경: 슬롯(설비) 15개를 직렬 순회하면서 슬롯마다 DB SELECT 를 4~5회 하면 한 바퀴가 ~10초까지
        //   늘어나, 어떤 트랙의 변화가 CV_DATA(→Client)에 반영되기까지 최대 그만큼 지연된다.
        //   → RGV 가 출발지에서 상차해 도착지에 하역할 때, 도착지(빠른 슬롯)가 먼저 갱신되고 출발지(늦은 슬롯)는
        //     아직 옛 값이라 "출발지에 화물이 남은 채 도착지에 화물이 생기는" 순서 역전으로 보였다.
        //   ① 첫 주소(GetFirstAddress)는 CV_DATA 의 정적 설정값이므로 설비별 1회만 조회하고 캐시한다.
        //   ② CMD/OD/트래킹 쓰기 대기는 사이클당 1회 스캔으로 '대기 있는 설비'만 추려, 없는 설비의 SELECT 3회를 생략한다.
        private readonly System.Collections.Generic.Dictionary<string, string> m_dicFirstAddr = new System.Collections.Generic.Dictionary<string, string>();
        private readonly System.Collections.Generic.HashSet<string> m_setPendCmd = new System.Collections.Generic.HashSet<string>();
        private readonly System.Collections.Generic.HashSet<string> m_setPendOd  = new System.Collections.Generic.HashSet<string>();
        private readonly System.Collections.Generic.HashSet<string> m_setPendTrk = new System.Collections.Generic.HashSet<string>();
        private bool m_bPendScanOk = false;   // 스캔 실패 시 false → 전 설비 종전대로 처리(안전측)
        private const int CYCLE_WARN_MS = 3000;               // 미러 한 바퀴 경고 임계(초과 시 단계별 소요 로깅)
        private DateTime m_dtLastCycleLog = DateTime.MinValue;
        // [LGLS 2026-08-01] 통신 실패가 연속된 사이클 수. 임계 도달 시 소켓을 닫고 스레드를 종료해 재접속시킨다.
        //   (설비 재기동으로 상대가 세션을 리셋하면 Send/Recv 는 실패하지만 m_bSocCon 은 여전히 true 라
        //    기존 EXIT 조건만으로는 영원히 재접속하지 못했다)
        private int m_nComFailCycle = 0;
        private const int COM_FAIL_LIMIT = 3;

        #endregion
        #region CvThread
        public CvThread(int nThNo,
                        string strWh_typ,
                        string Eqmt_typ,
                        string Plc_No,
                        string Ip,
                        int CurPort,
                        int FromPort,
                        int ToPort,
                        int PortCnt,
                        int Cnt,
                        int FrTrackNo,
                        int ToTrackNo,
                        string ConnectString,
                        string strLogFileNm)
        {
            #region 변수값 세팅
            m_nthNo = nThNo;
            m_strWh_typ = strWh_typ;
            m_strEqmt_typ = Eqmt_typ;
            m_strPlc_No = Plc_No;
            m_strIp = Ip;

            m_nCurPort = CurPort;
            m_nFromPort = FromPort;
            m_nToPort = ToPort;

            m_nPortCnt = PortCnt;
            m_nFrTrackNo = FrTrackNo;
            m_nToTrackNo = ToTrackNo;
            m_strConnectString = ConnectString;
            m_strLogFileNm = strLogFileNm;
            IsOpen = false;
            // PLC 제조사에 따라 프로토콜 선택 (ini [PLC] MAKER : 1=XGT/FEnet, 0=Melsec/Q3E)
            if (cDefApi.GsReadInitProfilePlcMaker() == 1)
                m_msQPlc = new FenetProtocol(m_strConnectString);
            else
                m_msQPlc = new MelsecQ3EProtocol(m_strConnectString);

            m_msQPlc.IsHex = true;
            m_nCnt = Cnt;

            // 첫 설비(COMM0)를 슬롯 목록에 등록. 나머지 설비는 SYS_MAIN 이 AddEquip 으로 추가.
            m_slots.Add(new EqpSlot { Plc = m_strPlc_No, Typ = m_strEqmt_typ, Fr = m_nFrTrackNo, To = m_nToTrackNo, Cnt = m_nCnt });

            #endregion
        }

        // [LGLS 2026-07-27] 통합 스레드에 처리할 추가 설비(CV)를 등록. 소켓은 공유(1개).
        public void AddEquip(string plc, string typ, int fr, int to, int cnt)
        {
            m_slots.Add(new EqpSlot { Plc = plc, Typ = typ, Fr = fr, To = to, Cnt = (cnt <= 0 ? 1 : cnt) });
        }
        #endregion
        #region Thread_Doing
        /*
         * 화면 표시용
         */
        #region
        private void MakeMsg(string msg, int nThGbn)
        {
            try
            {
                m_frmMain.PsMsgView(msg, m_strPlc_No.ToString(), nThGbn);
            }
            catch (Exception ex)
            {
                return;
            }
        }

        private void MakeMsg_Error(string msg, int nThGbn)
        {
            try
            {
                m_frmMain.PsMsgView_Error(msg, m_strPlc_No.ToString(), nThGbn);
                cDefApp.m_LogQ[m_nthNo].Enqueue(new LogParam(DateTime.Now, msg));
                // [LGLS 진단] 에러는 DB 로그에도 남긴다 (파일/화면 로그 확인 불가 환경 대비)
                try { InsertWcsLogPgr("", "[ERR] " + msg); } catch { }
            }
            catch (Exception ex)
            {
                return;
            }
        }

        private void MakeMsg_Imp(string msg, int nThGbn)
        {
            try
            {
                m_frmMain.PsMsgView_IMP(msg, m_strPlc_No.ToString(), nThGbn);
                cDefApp.m_LogQ[m_nthNo].Enqueue(new LogParam(DateTime.Now, msg));
            }
            catch (Exception ex)
            {
                return;
            }
        }

        public void SetErrorMsg(string strMsg)
        {
            _strErrorMsg = strMsg;
            Log.Error(_strErrorMsg);
        }
        #endregion
        /*
         * 실구동용
         */
        #region
        public void Thread_Doing(object value)
        {
            try
            {
                if (cDefApp.GM_STAT_MAIN == false)
                {
                    throw new Exception("서비스 중지로 인한 쓰레드 종료");
                }

                MakeMsg_Imp("DB/Socket Connectting", m_nthNo);

                if (m_msQPlc.m_bSocCon == false && m_msQPlc.m_bDBOpen == false)
                {
                    // open된 포트개수 만큼 재연결 (FROM==TO 단일 포트여도 1회는 시도)
                    for (int i = 0; i <= m_nToPort - m_nFromPort; i++)
                    {
                        if (m_nCurPort > m_nToPort)
                        {
                            m_nCurPort = m_nFromPort;
                        }
                        for (int j = 0; j < m_nPortCnt; j++)
                        {
                            MakeMsg_Imp(string.Format("IP [{0}] PORT [{1}] 접속시도", m_strIp, m_nCurPort.ToString()), m_nthNo);
                            m_msQPlc.SetConfig(m_strIp, m_nCurPort, 2);

                            if (!m_msQPlc.Open(ref m_strRtnMsg))
                            {
                                SetErrorMsg("Comm" + m_nthNo + " :" + m_strRtnMsg);
                                MakeMsg_Error(m_strRtnMsg, m_nthNo);

                                //DB는 접속 되었는데 설비와 연결이 안되어 있는 경우 LOG남기기
                                if (m_msQPlc.m_bSocCon == false && m_msQPlc.m_bDBOpen == true)
                                {
                                    InsertWcsLogPgr("", "[Thread_Doing] 소켓 연결중 에러");
                                }

                                m_msQPlc.Close(ref m_strRtnMsg);

                                if (j == m_nPortCnt - 1)
                                {
                                    m_nCurPort = m_nCurPort + 1;
                                }
                                m_blConnectYn = false;

                                Thread.Sleep(500); //2000
                                continue;
                            }
                            else
                            {
                                // ini에 현재 설정된 포트값 쓰기
                                string strCOMM = "COMM" + m_nthNo;
                                cDefApi.WritePrivateProfileString(strCOMM, "CUR_PORT", Convert.ToString("" + m_nCurPort), cDefApp.GM_ENV_INI);

                                //접속 성공 로그 남기기 
                                InsertWcsLogPgr("", "[Thread_Doing] CV 그룹 번호 : " + m_strPlc_No + ", 연결포트 : " + m_nCurPort + " 접속 성공");

                                //접속이 성공하거나 시도횟수를 OVER하면 빠져나간다.
                                m_blConnectYn = true;
                                break;
                            }
                        }
                        //연결이 성공하면 빠져나가기.
                        if (m_blConnectYn == true)
                        {
                            break;
                        }
                    }
                }

                if (m_msQPlc.m_bSocCon == true && m_msQPlc.m_bDBOpen == true)
                {
                    IsOpen = true;
                    MakeMsg_Imp("DB login Ok! (21.07.07)", m_nthNo);

                    while (true)
                    {
                        // [LGLS 2026-07-27] [시나리오 테스트] 자동 통신 일시정지 — 시나리오 창이 소켓을 강제 조작하는 동안 슬롯 순회 스킵
                        if (m_bPaused) { Thread.Sleep(100); continue; }

                        // [LGLS 2026-07-27] 마스터 PLC 1소켓 통합: 등록된 설비(슬롯)를 순회하며 각 설비의
                        //   PLC_NO/트랙 필드를 세팅한 뒤 기존 처리(READ/WRITE)를 그대로 재사용한다.
                        //   주소는 모두 cvMachineNo 기반 절대주소라 소켓 1개로 전 설비 접근 가능.
                        // [LGLS 2026-07-31] 사이클 시작 시 쓰기 대기 스캔 1회(슬롯별 SELECT 3회씩 생략 → 미러 주기 단축)
                        System.Diagnostics.Stopwatch swCycle = System.Diagnostics.Stopwatch.StartNew();
                        long msScan = 0, msAddr = 0, msStat = 0, msCmd = 0, msOd = 0, msEvt = 0, msTrk = 0;
                        int nCycleFail = 0;   // [LGLS 2026-08-01] 이번 사이클의 통신 실패 건수(자동 재접속 판정)
                        System.Diagnostics.Stopwatch swPh = new System.Diagnostics.Stopwatch();

                        swPh.Restart();
                        ScanPendingWork();
                        msScan = swPh.ElapsedMilliseconds;

                        for (int si = 0; si < m_slots.Count; si++)
                        {
                            var slot = m_slots[si];
                            m_strPlc_No   = slot.Plc;
                            m_strEqmt_typ = slot.Typ;
                            m_nFrTrackNo  = slot.Fr;
                            m_nToTrackNo  = slot.To;

                            bool slotOk = true;
                            for (int Idx = 1; Idx <= slot.Cnt; Idx++)
                            {
                                this.m_msQPlc.IsAscii = m_frmMain.IsAscii;
                                this.m_msQPlc.IsHex = m_frmMain.IsHex;

                                swPh.Restart();
                                bool okAddr = GetFirstAddress(Idx);  msAddr += swPh.ElapsedMilliseconds;
                                if (!okAddr) { slotOk = false; break; } //읽을 첫 주소(그 설비 CV_DATA)를 못 구하면 이 설비만 skip

                                swPh.Restart();
                                bool okStat = CvStatusScenario(Idx); msStat += swPh.ElapsedMilliseconds;
                                if (!okStat) { slotOk = false; break; } // CV 상태 READ → CV_DATA 반영

                                // 쓰기 대기가 없는 설비는 해당 SELECT 자체를 생략(스캔 실패 시엔 종전대로 전부 수행)
                                swPh.Restart();
                                bool okCmd = (!m_bPendScanOk || m_setPendCmd.Contains(slot.Plc)) ? CvChg_CMD_RQ_YN(Idx) : true;
                                msCmd += swPh.ElapsedMilliseconds;
                                if (!okCmd) { slotOk = false; break; } // CMD 쓰기지시 → WRITE

                                swPh.Restart();
                                bool okOd = (!m_bPendScanOk || m_setPendOd.Contains(slot.Plc)) ? CvChg_OD_RQ_YN(Idx) : true;
                                msOd += swPh.ElapsedMilliseconds;
                                if (!okOd) { slotOk = false; break; } // OD 쓰기지시 → WRITE

                                swPh.Restart();
                                bool okEvt = CvEventCheck(Idx);      msEvt += swPh.ElapsedMilliseconds;
                                if (!okEvt) { slotOk = false; break; } // Load/Unload Complete ACK

                                swPh.Restart();
                                bool okTrk = (!m_bPendScanOk || m_setPendTrk.Contains(slot.Plc)) ? CvTrackingWrite(Idx) : true;
                                msTrk += swPh.ElapsedMilliseconds;
                                if (!okTrk) { slotOk = false; break; } // R 트래킹 JOB WRITE
                            }
                            // [LGLS 2026-07-27] 통합 순회: 슬롯 실패가 "소켓 끊김"이면 전체 재접속(EXIT),
                            //   그 설비의 DB/데이터 문제면 해당 설비만 건너뛰고 다음 설비를 계속 처리(스레드 독립성 재현).
                            if (!slotOk && m_msQPlc.m_bSocCon == false) goto EXIT_LBL;
                            if (!slotOk) nCycleFail++;
                        }

                        // 알람 M비트(M0492/M0493)는 전 CV 공통 단일주소 → 사이클당 1회만 처리
                        if (!CvAlarmCheck(1))
                        {
                            if (m_msQPlc.m_bSocCon == false) goto EXIT_LBL;
                            nCycleFail++;
                        }

                        // [LGLS 2026-08-01] 죽은 소켓 자동 재접속.
                        //   설비(EQP_SIM/PLC)가 재기동되면 기존 TCP 세션은 상대가 리셋한다(WSAECONNRESET:
                        //   "현재 연결은 원격 호스트에 의해 강제로 끊겼습니다"). 그런데 Send/Recv 실패는
                        //   m_bSocCon 을 내리지 않아 위의 두 EXIT 조건이 성립하지 않았고, 결과적으로 죽은
                        //   소켓에 대고 전 슬롯 READ 실패를 무한 반복하며 하트비트가 멈춘 채 방치됐다
                        //   (2026-07-31 EQP_SIM 재기동 후 34시간 정지 사례 — Client EQUIP 빨강).
                        //   → 연속 실패가 임계를 넘으면 소켓을 명시적으로 닫고 스레드를 종료한다.
                        //     SYS_MAIN.Thread_Tick 이 m_thThread==null 을 보고 재생성 → 접속 루프 재실행.
                        if (nCycleFail > 0)
                        {
                            if (++m_nComFailCycle >= COM_FAIL_LIMIT)
                            {
                                MakeMsg_Error(string.Format(
                                    "[Thread_Doing] 통신 연속 실패 {0}회 - 소켓이 끊긴 것으로 보고 재접속합니다. ({1})",
                                    m_nComFailCycle, m_msQPlc.GetErrorMsg()), m_nthNo);
                                InsertWcsLogPgr("", "[Thread_Doing] 통신 연속 실패 → 소켓 재접속 시도");
                                try { m_msQPlc.Close(ref m_strRtnMsg); } catch { }
                                goto EXIT_LBL;
                            }
                        }
                        else m_nComFailCycle = 0;

                        // [LGLS 2026-07-31] 미러 주기 감시: 한 바퀴가 길어지면 그만큼 CV_DATA(→Client) 반영이 늦어져
                        //   설비 동작 순서가 뒤바뀐 것처럼 보인다. 느릴 때만(임계 초과) 30초에 1번 단계별 소요를 남긴다.
                        swCycle.Stop();
                        if (swCycle.ElapsedMilliseconds > CYCLE_WARN_MS &&
                            (DateTime.Now - m_dtLastCycleLog).TotalSeconds >= 30)
                        {
                            m_dtLastCycleLog = DateTime.Now;
                            string strCyc = string.Format(
                                "[CYCLE] 슬롯 {0}개 순회 {1}ms (scan {2} / addr {3} / status {4} / cmd {5} / od {6} / event {7} / trk {8})",
                                m_slots.Count, swCycle.ElapsedMilliseconds, msScan, msAddr, msStat, msCmd, msOd, msEvt, msTrk);
                            MakeMsg_Imp(strCyc, m_nthNo);
                            InsertWcsLogPgr("", strCyc);
                        }

                        Thread.Sleep(200);
                    }
                }

            EXIT_LBL:
                {
                    SetErrorMsg("CoMM" + m_nthNo + " DB & Socket logoff!");
                    MakeMsg_Imp("DB & Socket logoff!", m_nthNo);
                }

            }
            catch (Exception ex)
            {
                MakeMsg_Error(ex.Message, m_nthNo);
            }
            IsOpen = false;
            m_msQPlc.Close(ref m_strRtnMsg);
            MakeMsg_Imp(m_strRtnMsg, m_nthNo);
            m_thThread = null;
        }
        #endregion
        #endregion Thread_Doing

        #region [GetFirstAddresss] :: 시작 주소 구하기
        /*
         * 시작위치 구함.
         */
        private bool GetFirstAddress(int Idx)
        {
            string strTitle = "[GetFirstAddress]";

            string strSql = "";
            string CRLF = "\r\n";
            int nSelCnt;
            DataTable dttest = new DataTable();

            try
            {
                // [LGLS 2026-07-31] 정적 설정값이라 설비별 1회만 조회하고 캐시(슬롯 순회 SELECT 15회/사이클 제거).
                string strCacheKey = m_strPlc_No + "|" + m_nFrTrackNo.ToString("000");
                string strCached;
                if (m_dicFirstAddr.TryGetValue(strCacheKey, out strCached))
                {
                    m_strAddress = strCached;
                    m_nAddress = (Convert.ToInt32(0 + m_strAddress)) * 10;   // 시작트랙 * 10 -> 시작 어드레스(원 계산과 동일)
                    return true;
                }

                strSql = "";
                strSql += CRLF + "SELECT TRACK_NO                ";
                strSql += CRLF + "      ,MC_NO                   ";
                strSql += CRLF + "FROM   CV_DATA                 ";
                strSql += CRLF + "WHERE  WH_TYP = :WH_TYP        ";
                strSql += CRLF + "AND    PLC_NO = :PLC_NO        ";
                strSql += CRLF + "AND    MC_NO  = :MC_NO         ";

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;
                m_msQPlc._pBdb.mComMain.Parameters.Add("MC_NO", DbLang.VARCHAR, 255).Value = m_nFrTrackNo.ToString("000");
                nSelCnt = m_msQPlc._pBdb.ExcuteQry(strSql);

                if (nSelCnt < 0)
                {
                    MakeMsg_Error(strTitle + "첫 트랙의 DATA를 가져오는 중 ERROR. WH_TYP [" + m_strWh_typ + "]  PLC_NO [" + m_strPlc_No + "] TRACK_NO [" + m_nFrTrackNo + "] ErroMsg [" + m_msQPlc._pBdb.ErrMsg + "]",m_nthNo);
                    return false;
                }

                if (nSelCnt == 0)
                {
                    MakeMsg_Error(strTitle + "첫 트랙의 DATA를 찾지 못했습니다. WH_TYP [" + m_strWh_typ + "]  PLC_NO [" + m_strPlc_No + "] TRACK_NO [" + m_nFrTrackNo + "]", m_nthNo);
                    return false;
                }

                //롯데 G동은 mc_no가 1(그룹번호)+01(설비번호)형식임.
                m_strAddress = m_msQPlc._pBdb.mDtMain.Rows[0]["MC_NO"].ToString().Substring(1,2);
                //m_strAddress = m_nFrTrackNo.ToString().Substring(2, 3);

                if (nSelCnt > 0)
                    m_nAddress = (Convert.ToInt32(0 + m_strAddress)) * 10; //시작트랙 * 10 -> 시작 어드레스


                if (m_nAddress < 0)
                {
                    MakeMsg_Error(strTitle + "트랙 시작위치 주소가 이상합니다. WH_TYP [" + m_strWh_typ + "]  PLC_NO [" + m_strPlc_No + "] TRACK_NO [" + m_nFrTrackNo + "]", m_nthNo);
                    return false;
                }
                m_dicFirstAddr[strCacheKey] = m_strAddress;   // [LGLS 2026-07-31] 정상 산출된 값만 캐시
                return true;
            }
            catch (Exception ex)
            {
                MakeMsg_Error(strTitle + "Exception Error [" + ex.Message + "]", m_nthNo);
                return false;
            }
        }
        #endregion GetFirstAddress

        // [LGLS 2026-07-31] 사이클당 1회: CMD/OD/트래킹 쓰기 대기가 있는 설비(PLC_NO)만 추린다.
        //   실패하면 m_bPendScanOk=false 로 두어 전 설비를 종전대로 처리한다(누락 방지).
        private void ScanPendingWork()
        {
            m_setPendCmd.Clear(); m_setPendOd.Clear(); m_setPendTrk.Clear();
            m_bPendScanOk = false;
            try
            {
                string CRLF = "\r\n";
                string sql = "";
                sql += CRLF + "SELECT PLC_NO, CMD_RQ_YN, OD_RQ_YN, TRACKING_WRITE_YN   ";
                sql += CRLF + "  FROM CV_DATA                                          ";
                sql += CRLF + " WHERE WH_TYP = :WH_TYP                                 ";
                sql += CRLF + "   AND (CMD_RQ_YN = 'Y' OR OD_RQ_YN = 'Y' OR TRACKING_WRITE_YN = 'Y') ";

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                int n = m_msQPlc._pBdb.ExcuteQry(sql);
                if (n < 0) return;

                for (int i = 0; i < m_msQPlc._pBdb.mDtMain.Rows.Count; i++)
                {
                    var row = m_msQPlc._pBdb.mDtMain.Rows[i];
                    string plc = ("" + row["PLC_NO"]).Trim();
                    if (("" + row["CMD_RQ_YN"]).Trim() == "Y") m_setPendCmd.Add(plc);
                    if (("" + row["OD_RQ_YN"]).Trim() == "Y") m_setPendOd.Add(plc);
                    if (("" + row["TRACKING_WRITE_YN"]).Trim() == "Y") m_setPendTrk.Add(plc);
                }
                m_bPendScanOk = true;
            }
            catch (Exception ex)
            {
                MakeMsg_Error("[ScanPendingWork] Exception: " + ex.Message, m_nthNo);
            }
        }

        // [LGLS 2026-07-30] 등록된 전 설비(슬롯)의 EQP_MST 하트비트를 1회 UPDATE 로 일괄 갱신.
        //   배경: 마스터 PLC 1소켓 통합 후 한 스레드가 15개 설비를 순회하므로, 설비별로만 하트비트를 찍으면
        //   각 PLC_NO 의 UPD_DT 가 '전체 순회 1바퀴'(~10초)마다 갱신된다. Client 의 EQUIP 판정은
        //   PLC_NO 별 `DATEDIFF(SECOND, EQP_MST.UPD_DT, GETDATE()) > 5`(Ecs\Cv.cpp) 이라 5초를 넘는 순간
        //   접속끊김으로 표시됐다가 다음 순회에 복구되는 **깜빡임**이 발생한다.
        //   소켓·접속 상태는 전 설비 공통(1소켓)이므로 슬롯 처리마다 전 설비 행을 함께 갱신해 항상 신선하게 유지한다.
        public bool CommunicationAllSlots(string CONNECTED_YN)
        {
            try
            {
                if (m_slots.Count == 0) return true;

                string strIn = "";
                for (int i = 0; i < m_slots.Count; i++)
                    strIn += (strIn.Length == 0 ? "" : ",") + "'" + m_slots[i].Plc + "'";

                string CRLF = "\r\n";
                string strSql = "";
                strSql += CRLF + "UPDATE EQP_MST                                    ";
                strSql += CRLF + "   SET CONNECTED_YN      = :CONNECTED_YN          ";
                strSql += CRLF + "      ,UPD_DT            = " + DbLang.SYSDATE + " ";
                strSql += CRLF + "      ,PLC_PORT          = :PLC_PORT              ";
                strSql += CRLF + "WHERE  WH_TYP            = :WH_TYP                ";
                strSql += CRLF + "AND    EQP_TYP           = :EQP_TYP               ";
                strSql += CRLF + "AND    PLC_NO            IN (" + strIn + ")       ";

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("CONNECTED_YN", DbLang.VARCHAR).Value = CONNECTED_YN;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_PORT", DbLang.VARCHAR, 255).Value = Convert.ToString("" + m_nCurPort);
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("EQP_TYP", DbLang.VARCHAR, 255).Value = m_strEqmt_typ;
                return m_msQPlc._pBdb.ExcuteNonQry(strSql) >= 0;
            }
            catch (Exception ex)
            {
                MakeMsg_Error("[CommunicationAllSlots] Exception Error" + ex.Message, m_nthNo);
                return false;
            }
        }

        #region [Communication] :: EQP_MST의 CONNECT 여부 설정
        public bool Communication(string CONNECTED_YN, string WH_TYP, string EQP_TYP, string PLC_NO)
        {
            string strTitle = "[Communication]";

            try
            {
                m_msQPlc._pBdb.BeginTrans();

                string strSql = "";
                string CRLF = "\r\n";
                int nSelCnt;

                MakeMsg("PLC 통신 OK", m_nthNo);

                strSql = "";
                strSql += CRLF + "UPDATE EQP_MST                                    ";
                strSql += CRLF + "   SET CONNECTED_YN      = :CONNECTED_YN          ";
                strSql += CRLF + "      ,UPD_DT            = " + DbLang.SYSDATE + " ";
                strSql += CRLF + "      ,PLC_PORT          = :PLC_PORT              ";
                strSql += CRLF + "WHERE  WH_TYP            = :WH_TYP                ";
                strSql += CRLF + "AND    EQP_TYP           = :EQP_TYP               ";
                strSql += CRLF + "AND    PLC_NO            = :PLC_NO                ";

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("CONNECTED_YN", DbLang.VARCHAR).Value = CONNECTED_YN;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_PORT", DbLang.VARCHAR, 255).Value = Convert.ToString("" + m_nCurPort);
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = WH_TYP;
                m_msQPlc._pBdb.mComMain.Parameters.Add("EQP_TYP", DbLang.VARCHAR, 255).Value = EQP_TYP;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = PLC_NO;
                nSelCnt = m_msQPlc._pBdb.ExcuteNonQry(strSql);
                if (nSelCnt < 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    MakeMsg_Error(strTitle + "PLC정보 변경중 ERROR. ErrorMsg [" + m_msQPlc._pBdb.ErrMsg + "] WH_TYP [" + WH_TYP + "] EQP_TYP [" + EQP_TYP + "]  PLC_NO [" + PLC_NO + "]", m_nthNo);
                    return false;
                }

                if (nSelCnt == 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    MakeMsg_Error(strTitle + "PLC정보 변경중 Data가 없습니다.WH_TYP [" + WH_TYP + "] EQP_TYP [" + EQP_TYP + "] PLC_NO [" + PLC_NO + "] CONNECTED_YN [" + CONNECTED_YN + "]", m_nthNo);
                    return false;
                }

                m_msQPlc._pBdb.Commit();
                return true;
            }
            catch (Exception ex)
            {
                m_msQPlc._pBdb.Rollback();
                MakeMsg_Error(strTitle + "Exception Error" + ex.Message, m_nthNo);
                return false;
            }
        }
        #endregion

        #region [CvStatus] :: CV READ 후 값이 변한게 있으면 DB UPDATE
        private bool CvStatus(int Idx)
        {
            string strTitle = "[CvStatus]";

            try
            {
                byte[] byRxBuff = new byte[2000];
                int nReadTrack;

                int nStatusData = 0;
                int nSenSorData = 0;
                int nMortorData = 0;
                int nErrorCode = 0;
                int nRetStatus = 0;

                string LUGG_NO_RD = "";
                string DEST_POS_RD = "";
                string IS_TURN_RD = "";
                string JOB_TYP_RD = "";
                string TRAY_LEV_RD = "";
                string TRAY_TYP_RD = "";
                string FMS_RPT_RD = "";
                string TR_PAUSE_RD = "";
                string WAIT_TIME_RD = "";
                string ERR_RQ_RD = "";
                string AUTO_MODE_RD = "";
                string STO_READY_RD = "";
                string RET_READY_RD = "";
                string STOHS_READY_RD = "";
                string RETHS_READY_RD = "";
                string SENSOR0_DATA_RD = "";
                string SENSOR1_DATA_RD = "";
                string SENSOR2_DATA_RD = "";
                string ERROR_CODE = "";
                string REMOTE_CONTROL = "";
                string STOCK_MODE = "";
                string ROLL_MODE = "";
                string A_TURN_YN = "";
                string B_TURN_YN = "";
                string PULP_SENSOR_RD = "";
                string WAIT_SC_RET_JOB_RD = "";
                string DELETE_TRACK_RD = "";
                string SC_LOCK_SENSOR = "";

                string DRIV_PAPER_POS = "";

                //10n+9
                string ELEV_ASC_ERR = "";
                string ELEV_DESC_ERR = "";
                string CLAMP_FORWARD_ERR = "";
                string CLAMP_BACKWARD_ERR = "";
                string DRIV_FORWARD_ERR = "";
                string DRIV_BACKWARD_ERR = "";
                string PAPER_BLOCK_SENSOR1 = "";
                string PAPER_BLOCK_SENSOR2 = "";
                string PAPER_BLOCK_SENSOR3 = "";
                string PAPER_BLOCK_SENSOR4 = "";
                string PAPER_FULL_SENSOR = "";
                string DRIV_FORWARD_POS = "";
                string DRIV_BACKWARD_POS = "";
                string CRUSH_PAPER_SENSOR = "";
                string CLAMP_FORWARD_SENSOR = "";
                string CLAMP_BACKWARD_SENSOR = "";
                

                /*
                 * 80개의 트랙식 읽음.
                 */

                MakeMsg(strTitle + "80 TRACK PLC 통신", m_nthNo);
                nReadTrack = 80;

                for (int CvNo = m_nFrTrackNo; CvNo < m_nToTrackNo; )
                {
                    if ((CvNo + nReadTrack - 1) > m_nToTrackNo)
                    {
                        nReadTrack = m_nToTrackNo - CvNo + 1;
                    }

                    Array.Clear(byRxBuff, 0x00, byRxBuff.Length);
                    int nAddress = (CvNo - m_nFrTrackNo) * 10 + m_nAddress;

                    if (CvNo == 197 || CvNo == 198)
                    {
                        int a = 11;
                    }

                    if (m_msQPlc.READ((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                            (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                            nAddress,
                            nReadTrack * 10,
                            ref byRxBuff) == false)
                    {
                        // 진단용: READ 실패 사유 + 송수신 헥사를 예외 메시지에 포함 (파일 로그에 남김)
                        throw new Exception("READ 실패 ADDR[" + nAddress + "] LEN[" + (nReadTrack * 10)
                                            + "] ERR[" + m_msQPlc.GetErrorMsg()
                                            + "] TX[" + m_msQPlc.SndHexString
                                            + "] RX[" + m_msQPlc.RcvHexString + "]");
                    }

                    int nReadLen = 20;

                    MakeMsg("상태값 DB저장", m_nthNo);
                    for (int nIdx = 0; nIdx < nReadTrack; nIdx++)
                    {

                        int nCvNo = nIdx + CvNo;

                        int nArrayIdx = ((((nIdx))) * 10) * 2;

                        if (!CvDic.ContainsKey(nCvNo))
                        {
                            CvDic.Add(nCvNo, new CVData()); //Key를 추가한다.
                        }

                        //최초 실행 시 현재 DB값을 DIC에 넣기(상위에 상태보고 하는 값들)
                        if (CvDic[nCvNo].AUTO_MODE_RD == "" &&
                            CvDic[nCvNo].STO_READY_RD == "" &&
                            CvDic[nCvNo].RET_READY_RD == "" &&
                            CvDic[nCvNo].STOHS_READY_RD == "" &&
                            CvDic[nCvNo].RETHS_READY_RD == "" &&
                            CvDic[nCvNo].SENSOR0_DATA_RD == "" &&
                            CvDic[nCvNo].SENSOR1_DATA_RD == "" &&
                            CvDic[nCvNo].SENSOR2_DATA_RD == "" &&
                            CvDic[nCvNo].DELETE_TRACK_RD == "" &&
                            CvDic[nCvNo].A_TURN_YN == "" &&
                            CvDic[nCvNo].B_TURN_YN == "" &&
                            CvDic[nCvNo].SC_LOCK_SENSOR == "" &&
                            CvDic[nCvNo].REMOTE_CONTROL == "" &&
                            CvDic[nCvNo].STOCK_MODE == "" &&
                            CvDic[nCvNo].ROLL_MODE == "" &&
                            CvDic[nCvNo].CVERRCD == 0)
                        {
                            int nSelCount = 0;

                            strSql = "";
                            strSql += cDefApp.CRLF + "SELECT CD.*                                          ";
                            strSql += cDefApp.CRLF + "  FROM CV_DATA CD                                    ";
                            strSql += cDefApp.CRLF + " WHERE CD.WH_TYP = :WH_TYP                           ";
                            strSql += cDefApp.CRLF + "   AND CD.PLC_NO = :PLC_NO                           ";
                            strSql += cDefApp.CRLF + "   AND CD.MC_NO  = :MC_NO                            ";

                            m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                            m_msQPlc._pBdb.mComMain.Parameters.Clear();
                            m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                            m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;
                            m_msQPlc._pBdb.mComMain.Parameters.Add("MC_NO", DbLang.VARCHAR, 255).Value = nCvNo;

                            nSelCount = m_msQPlc._pBdb.ExcuteQry(strSql);

                            if (nSelCount < 0)
                            {
                                MakeMsg_Error(strTitle + "최초 트랙정보 읽는 중 에러(CV_DATA)", m_nthNo);
                                return false;
                            }

                            CvDic[nCvNo].AUTO_MODE_RD = m_msQPlc._pBdb.mDtMain.Rows[0]["AUTO_MODE_RD"].ToString();
                            CvDic[nCvNo].STO_READY_RD = m_msQPlc._pBdb.mDtMain.Rows[0]["STO_READY_RD"].ToString();
                            CvDic[nCvNo].RET_READY_RD = m_msQPlc._pBdb.mDtMain.Rows[0]["RET_READY_RD"].ToString();
                            CvDic[nCvNo].STOHS_READY_RD = m_msQPlc._pBdb.mDtMain.Rows[0]["STOHS_READY_RD"].ToString();
                            CvDic[nCvNo].RETHS_READY_RD = m_msQPlc._pBdb.mDtMain.Rows[0]["RETHS_READY_RD"].ToString();
                            CvDic[nCvNo].SENSOR0_DATA_RD = m_msQPlc._pBdb.mDtMain.Rows[0]["SENSOR0_DATA_RD"].ToString();
                            CvDic[nCvNo].SENSOR1_DATA_RD = m_msQPlc._pBdb.mDtMain.Rows[0]["SENSOR1_DATA_RD"].ToString();
                            CvDic[nCvNo].SENSOR2_DATA_RD = m_msQPlc._pBdb.mDtMain.Rows[0]["SENSOR2_DATA_RD"].ToString();
                            CvDic[nCvNo].DELETE_TRACK_RD = m_msQPlc._pBdb.mDtMain.Rows[0]["DELETE_TRACK_RD"].ToString();
                            CvDic[nCvNo].A_TURN_YN = m_msQPlc._pBdb.mDtMain.Rows[0]["A_TURN_YN"].ToString();
                            CvDic[nCvNo].B_TURN_YN = m_msQPlc._pBdb.mDtMain.Rows[0]["B_TURN_YN"].ToString();
                            CvDic[nCvNo].SC_LOCK_SENSOR = m_msQPlc._pBdb.mDtMain.Rows[0]["SC_LOCK_SENSOR"].ToString();
                            CvDic[nCvNo].REMOTE_CONTROL = m_msQPlc._pBdb.mDtMain.Rows[0]["REMOTE_CONTROL"].ToString();
                            CvDic[nCvNo].STOCK_MODE = m_msQPlc._pBdb.mDtMain.Rows[0]["STOCK_MODE"].ToString();
                            CvDic[nCvNo].ROLL_MODE = m_msQPlc._pBdb.mDtMain.Rows[0]["ROLL_MODE"].ToString();
                            CvDic[nCvNo].CVERRCD = Convert.ToInt32(0 + m_msQPlc._pBdb.mDtMain.Rows[0]["ERROR_CODE"].ToString());
                        }
                        //Hexa string 값으로 가져온다.
                        string strCvHexVal = BytesToHexs(byRxBuff, nArrayIdx, nReadLen);

                        //Conveyor상태값 또는 BCR 값이 다를 때만 Update.
                        if (CvDic[nCvNo].CVSTATHEXVAL != strCvHexVal)
                        {
                            CvDic[nCvNo].CVSTATHEXVAL = strCvHexVal; //Dictionary 값을 변경한다. true 0X

                            //D10n
                            LUGG_NO_RD = ((byRxBuff[1 + nArrayIdx] << 8) + byRxBuff[0 + nArrayIdx]).ToString("0000"); //작업번호

                            //D10n+1
                            DEST_POS_RD = ((byRxBuff[3 + nArrayIdx] << 8) + byRxBuff[2 + nArrayIdx]).ToString("000"); //목적지

                            //D10n+2
                            JOB_TYP_RD = Convert.ToString("" + (byRxBuff[4 + nArrayIdx] & 0X0F).ToString());//작업구분.
                            // [LGLS 2026-07-19] PLC 규격 작업구분(11=입고, 12=출고) → 표준 코드(1/2) 정규화.
                            //   (수신값 파싱 보정 — 통신 프레임/쓰기 로직 무변경. 클라이언트 범례에서 11/12는
                            //    '반자동' 다크 색상으로 분리되었으므로 자동 작업 readback 이 11/12 로 남으면 오표시됨)
                            if (JOB_TYP_RD == "11") JOB_TYP_RD = "1";
                            else if (JOB_TYP_RD == "12") JOB_TYP_RD = "2";
                            IS_TURN_RD = Convert.ToString("" + (byRxBuff[4 + nArrayIdx] >> 4).ToString());//Turn 신호.
                            PULP_SENSOR_RD = Convert.ToString("" + (byRxBuff[5 + nArrayIdx] & 0X0F).ToString());//PULP 단수(0,1,2,3)
                            //IS_TURN_RD = Convert.ToString("" + (byRxBuff[5 + nArrayIdx] >> 4).ToString());//Tilting

                            //D10n+3 (spare)
                            //TRAY_TYP_RD = (byRxBuff[6 + nArrayIdx] & 0X0F).ToString();
                            //TRAY_LEV_RD = (byRxBuff[6 + nArrayIdx] >> 4).ToString();
                            //TRAY_TYP_RD = (byRxBuff[7 + nArrayIdx] & 0X0F).ToString();
                            //TRAY_LEV_RD = (byRxBuff[7 + nArrayIdx] >> 4).ToString();

                            //D10n+4 (spare)
                            //WAIT_TIME_RD = (byRxBuff[8 + nArrayIdx]).ToString();
                            //FMS_RPT_RD = (byRxBuff[9 + nArrayIdx] & 0X0F).ToString();
                            //TR_PAUSE_RD = (byRxBuff[9 + nArrayIdx] >> 4).ToString();

                            //D10n+5
                            //TR_PAUSE_RD = (byRxBuff[10 + nArrayIdx] & 0X0F).ToString(); //TRACK PAUSE
                            //   [LGLS 2026-08-04] 위 PLC 관측은 사용하지 않는다(내부 변수 전용). 아래 대기필요만 읽는다.
                            WAIT_SC_RET_JOB_RD = (byRxBuff[10 + nArrayIdx] >> 4).ToString(); // 대기필요
                            //WAIT_TIME_RD = (byRxBuff[11 + nArrayIdx]).ToString(); //대기시간(사용X)
                            //ERR_RQ_RD = ((byRxBuff[11 + nArrayIdx] << 8) + byRxBuff[10 + nArrayIdx]).ToString(); //SKI에서 사용중
                            //IS_TURN_RD = Convert.ToString("" + (byRxBuff[10 + nArrayIdx] >> 4).ToString());//대기필요
                            //ERR_RQ_RD = (byRxBuff[10 + nArrayIdx] & 0X0F).ToString();
                            //ERR_RQ_RD = (byRxBuff[10 + nArrayIdx] >> 4).ToString();
                            //ERR_RQ_RD = (byRxBuff[11 + nArrayIdx]).ToString(); //대기시간(사용X)

                            //D10n+6
                            nErrorCode = (byRxBuff[13 + nArrayIdx] << 8) + byRxBuff[12 + nArrayIdx]; //에러코드 int형
                            ERROR_CODE = ((byRxBuff[13 + nArrayIdx] << 8) + byRxBuff[12 + nArrayIdx]).ToString("0000");	//에러코드

                            //7번, 8번 영역은 값이 변경되면 상위에 보고함.
                            //10n+7
                            nStatusData = (byRxBuff[15 + nArrayIdx] << 8) + byRxBuff[14 + nArrayIdx];

                            AUTO_MODE_RD = ((nStatusData >> 0) & 0x01).ToString();	//자동,수동.
                            if (CvDic[nCvNo].AUTO_MODE_RD != AUTO_MODE_RD)
                            {
                                CvDic[nCvNo].AUTO_MODE_RD = AUTO_MODE_RD;
                                m_blHostSendYN = true;
                            }

                            STO_READY_RD = ((nStatusData >> 1) & 0x01).ToString();	//입고대.
                            if (CvDic[nCvNo].STO_READY_RD != STO_READY_RD)
                            {
                                CvDic[nCvNo].STO_READY_RD = STO_READY_RD;
                                m_blHostSendYN = true;
                            }

                            RET_READY_RD = ((nStatusData >> 2) & 0x01).ToString();	//출고대.
                            if (CvDic[nCvNo].RET_READY_RD != RET_READY_RD)
                            {
                                CvDic[nCvNo].RET_READY_RD = RET_READY_RD;
                                m_blHostSendYN = true;
                            }

                            STOHS_READY_RD = ((nStatusData >> 3) & 0x01).ToString();	//입고HS.
                            if (CvDic[nCvNo].STOHS_READY_RD != STOHS_READY_RD)
                            {
                                CvDic[nCvNo].STOHS_READY_RD = STOHS_READY_RD;
                                m_blHostSendYN = true;
                            }

                            RETHS_READY_RD = ((nStatusData >> 4) & 0x01).ToString();	//출고HS.
                            if (CvDic[nCvNo].RETHS_READY_RD != RETHS_READY_RD)
                            {
                                CvDic[nCvNo].RETHS_READY_RD = RETHS_READY_RD;
                                m_blHostSendYN = true;
                            }

                            DRIV_PAPER_POS = ((nStatusData >> 15) & 0x01).ToString(); //주행 지관 위치

                            //10n+8
                            nSenSorData = (byRxBuff[17 + nArrayIdx] << 8) + byRxBuff[16 + nArrayIdx];

                            SENSOR0_DATA_RD = Convert.ToString(Convert.ToInt32((nSenSorData >> 0) & 0x01)) == "0" ? "0" : "1"; //화물감지1단
                            if (CvDic[nCvNo].SENSOR0_DATA_RD != SENSOR0_DATA_RD)
                            {
                                CvDic[nCvNo].SENSOR0_DATA_RD = SENSOR0_DATA_RD;
                                m_blHostSendYN = true;
                            }

                            SENSOR1_DATA_RD = Convert.ToString(Convert.ToInt32((nSenSorData >> 1) & 0x01)) == "0" ? "0" : "1"; //[PULP 1단] 0=1단아님, 1=1단
                            if (CvDic[nCvNo].SENSOR1_DATA_RD != SENSOR1_DATA_RD)
                            {
                                CvDic[nCvNo].SENSOR1_DATA_RD = SENSOR1_DATA_RD;
                                m_blHostSendYN = true;
                            }

                            SENSOR2_DATA_RD = Convert.ToString(Convert.ToInt32((nSenSorData >> 2) & 0x01)) == "0" ? "0" : "1"; //[PULP 2단] 0=2단아님, 1=2단
                            if (CvDic[nCvNo].SENSOR2_DATA_RD != SENSOR2_DATA_RD)
                            {
                                CvDic[nCvNo].SENSOR2_DATA_RD = SENSOR2_DATA_RD;
                                m_blHostSendYN = true;
                            }

                            DELETE_TRACK_RD = Convert.ToString(Convert.ToInt32((nSenSorData >> 3) & 0x01)) == "0" ? "0" : "1"; //PLC 삭제 요청
                            if (CvDic[nCvNo].DELETE_TRACK_RD != DELETE_TRACK_RD)
                            {
                                CvDic[nCvNo].DELETE_TRACK_RD = DELETE_TRACK_RD;
                                m_blHostSendYN = true;
                            }

                            A_TURN_YN = Convert.ToString(Convert.ToInt32((nSenSorData >> 4) & 0x01)) == "0" ? "0" : "1"; //A TURN 여부
                            if (CvDic[nCvNo].A_TURN_YN != A_TURN_YN)
                            {
                                CvDic[nCvNo].A_TURN_YN = A_TURN_YN;
                                m_blHostSendYN = true;
                            }

                            B_TURN_YN = Convert.ToString(Convert.ToInt32((nSenSorData >> 5) & 0x01)) == "0" ? "0" : "1"; //B TURN 여부
                            if (CvDic[nCvNo].B_TURN_YN != B_TURN_YN)
                            {
                                CvDic[nCvNo].B_TURN_YN = B_TURN_YN;
                                m_blHostSendYN = true;
                            }

                            REMOTE_CONTROL = Convert.ToString(Convert.ToInt32((nSenSorData >> 6) & 0x01)) == "0" ? "0" : "1"; //리모콘 정보
                            if (CvDic[nCvNo].REMOTE_CONTROL != REMOTE_CONTROL)
                            {
                                CvDic[nCvNo].REMOTE_CONTROL = REMOTE_CONTROL;
                                m_blHostSendYN = true;
                            }

                            SC_LOCK_SENSOR = Convert.ToString(Convert.ToInt32((nSenSorData >> 10) & 0x01)) == "0" ? "0" : "1"; //SC 인터락 센서 감지 여부
                            if (CvDic[nCvNo].SC_LOCK_SENSOR != SC_LOCK_SENSOR)
                            {
                                CvDic[nCvNo].SC_LOCK_SENSOR = SC_LOCK_SENSOR;
                                m_blHostSendYN = true;
                            }

                            ROLL_MODE = Convert.ToString(Convert.ToInt32((nSenSorData >> 14) & 0x01)) == "0" ? "0" : "1"; //ROLL 모드
                            if (CvDic[nCvNo].ROLL_MODE != ROLL_MODE)
                            {
                                CvDic[nCvNo].ROLL_MODE = ROLL_MODE;
                                m_blHostSendYN = true;
                            }

                            STOCK_MODE = Convert.ToString(Convert.ToInt32((nSenSorData >> 15) & 0x01)) == "0" ? "0" : "1"; //입출고 모드
                            if (CvDic[nCvNo].STOCK_MODE != STOCK_MODE)
                            {
                                CvDic[nCvNo].STOCK_MODE = STOCK_MODE;
                                m_blHostSendYN = true;
                            }

                            //10n + 9
                            nMortorData = (byRxBuff[19 + nArrayIdx] << 8) + byRxBuff[18 + nArrayIdx];

                            ELEV_ASC_ERR = Convert.ToString(Convert.ToInt32((nMortorData >> 0) & 0x01)) == "0" ? "0" : "1"; //승강 상승 비상(반전기,롤링기)
                            ELEV_DESC_ERR = Convert.ToString(Convert.ToInt32((nMortorData >> 1) & 0x01)) == "0" ? "0" : "1"; //승강 하강 비상(반전기,롤링기)
                            CLAMP_FORWARD_ERR = Convert.ToString(Convert.ToInt32((nMortorData >> 2) & 0x01)) == "0" ? "0" : "1"; //클램프 전진 비상(반전기,롤링기)
                            CLAMP_BACKWARD_ERR = Convert.ToString(Convert.ToInt32((nMortorData >> 3) & 0x01)) == "0" ? "0" : "1"; //클램프 후진 비상(반전기,롤링기)
                            DRIV_FORWARD_ERR = Convert.ToString(Convert.ToInt32((nMortorData >> 4) & 0x01)) == "0" ? "0" : "1"; //주행 전진 비상(반전기,롤링기)
                            DRIV_BACKWARD_ERR = Convert.ToString(Convert.ToInt32((nMortorData >> 5) & 0x01)) == "0" ? "0" : "1"; //주행 후진 비상(반전기,롤링기)
                            PAPER_BLOCK_SENSOR1 = Convert.ToString(Convert.ToInt32((nMortorData >> 6) & 0x01)) == "0" ? "0" : "1"; //지관 막힘 센서 #1(롤링기)
                            PAPER_BLOCK_SENSOR2 = Convert.ToString(Convert.ToInt32((nMortorData >> 7) & 0x01)) == "0" ? "0" : "1"; //지관 막힘 센서 #2(롤링기)
                            PAPER_BLOCK_SENSOR3 = Convert.ToString(Convert.ToInt32((nMortorData >> 8) & 0x01)) == "0" ? "0" : "1"; //지관 막힘 센서 #3(롤링기)
                            PAPER_BLOCK_SENSOR4 = Convert.ToString(Convert.ToInt32((nMortorData >> 9) & 0x01)) == "0" ? "0" : "1"; //지관 막힘 센서 #4(롤링기)
                            PAPER_FULL_SENSOR = Convert.ToString(Convert.ToInt32((nMortorData >> 10) & 0x01)) == "0" ? "0" : "1"; //지관 만재 센서(트랜스퍼)
                            DRIV_FORWARD_POS = Convert.ToString(Convert.ToInt32((nMortorData >> 11) & 0x01)) == "0" ? "0" : "1"; //주행 전진 위치(롤링기, 반전기)
                            DRIV_BACKWARD_POS = Convert.ToString(Convert.ToInt32((nMortorData >> 12) & 0x01)) == "0" ? "0" : "1"; //주행 후진 위치(롤링기, 반전기)
                            CRUSH_PAPER_SENSOR = Convert.ToString(Convert.ToInt32((nMortorData >> 13) & 0x01)) == "0" ? "0" : "1"; //파쇄 종이 감지 센서(롤링기)
                            CLAMP_FORWARD_SENSOR = Convert.ToString(Convert.ToInt32((nMortorData >> 14) & 0x01)) == "0" ? "0" : "1"; //클램프 전진 감지(롤링기, 반전기)
                            CLAMP_BACKWARD_SENSOR = Convert.ToString(Convert.ToInt32((nMortorData >> 15) & 0x01)) == "0" ? "0" : "1"; //클램프 후진 감지(롤링기, 반전기)

                            //에러코드가 있고 전에 에러코드와 다를때만.
                            //처음에 에러코드가 0인거는 안탐.
                            if (CvDic[nCvNo].CVERRCD != nErrorCode)
                            {
                                m_blHostErrSendYN = true;

                                if (!UpdateEQMT_ERR_LOG(m_strWh_typ, m_strEqmt_typ, nCvNo.ToString("000"), ERROR_CODE, LUGG_NO_RD))
                                {
                                    m_blHostErrSendYN = false;
                                    return false;
                                }

                                //에러코드를 SET한다.
                                CvDic[nCvNo].CVERRCD = nErrorCode;
                            }


                            //TRACK정보 UPDATE.
                            if (!UpdateCvData(LUGG_NO_RD
                                            , DEST_POS_RD
                                            , JOB_TYP_RD
                                            , IS_TURN_RD
                                            , TRAY_TYP_RD
                                            , TRAY_LEV_RD
                                            , FMS_RPT_RD
                                            , TR_PAUSE_RD
                                            , ERR_RQ_RD
                                            , nErrorCode
                                            , ERROR_CODE
                                            , AUTO_MODE_RD
                                            , STO_READY_RD
                                            , RET_READY_RD
                                            , STOHS_READY_RD
                                            , RETHS_READY_RD
                                            , SENSOR0_DATA_RD
                                            , SENSOR1_DATA_RD
                                            , SENSOR2_DATA_RD
                                            , nCvNo.ToString("000")
                                            , REMOTE_CONTROL
                                            , ROLL_MODE
                                            , STOCK_MODE
                                            , A_TURN_YN
                                            , B_TURN_YN
                                            , PULP_SENSOR_RD
                                            , WAIT_SC_RET_JOB_RD
                                            , DELETE_TRACK_RD
                                            , SC_LOCK_SENSOR
                                            , DRIV_PAPER_POS
                                            , ELEV_ASC_ERR
                                            , ELEV_DESC_ERR
                                            , CLAMP_FORWARD_ERR
                                            , CLAMP_BACKWARD_ERR
                                            , DRIV_FORWARD_ERR
                                            , DRIV_BACKWARD_ERR
                                            , PAPER_BLOCK_SENSOR1
                                            , PAPER_BLOCK_SENSOR2
                                            , PAPER_BLOCK_SENSOR3
                                            , PAPER_BLOCK_SENSOR4
                                            , PAPER_FULL_SENSOR
                                            , DRIV_FORWARD_POS
                                            , DRIV_BACKWARD_POS
                                            , CRUSH_PAPER_SENSOR
                                            , CLAMP_FORWARD_SENSOR
                                            , CLAMP_BACKWARD_SENSOR))
                            {
                                m_blHostSendYN = false;
                                m_blHostErrSendYN = false;
                                return false;
                            }
                        }
                        m_blHostSendYN = false;
                        m_blHostErrSendYN = false;
                        m_strCvNo = Convert.ToString("" + nCvNo);
                        nReadLen += 20;
                    }

                    CvNo += nReadTrack;

                    if (m_nToTrackNo < CvNo) break;
                }
                //설비 통신상태 업데이트
                Communication("Y", m_strWh_typ, m_strEqmt_typ, m_strPlc_No);
            }
            catch (Exception ex)
            {
                m_blHostSendYN = false;
                m_blHostErrSendYN = false;
                SetErrorMsg("Comm" + m_nthNo + strTitle + "Exception Error" + ex.Message);
                Communication("N", m_strWh_typ, m_strEqmt_typ, m_strPlc_No);
                InsertWcsLogPgr(m_strCvNo, strTitle + " 트랙번호 : [" + m_strCvNo + "] 데이터 읽기 중 에러");
                MakeMsg_Error(strTitle + m_strCvNo + "Exception Error" + ex.Message, m_nthNo);
                return false;
            }
            return true;
        }
        #endregion


        #region [CvChg_CMD_RQ_YN] :: CV_DATA에서 CMD_RQ_YN 여부에 따른 CV 지시
        private bool CvChg_CMD_RQ_YN(int Idx)
        {
            string strTitle = "[CvChg_CMD_RQ_YN]";

            try
            {
                string strSql = "";

                byte[] byTxBuff = new byte[1000];

                int nUpdCount = 0;
                /*
                 * 변경할 트랙 정보 읽음
                 */
                #region
                strSql = "";
#if SQL
                strSql += cDefApp.CRLF + "SELECT TOP 5 CD.*                                    "; // [LGLS] SQL Server: LIMIT → TOP
#else
                strSql += cDefApp.CRLF + "SELECT CD.*                                          ";
#endif
                strSql += cDefApp.CRLF + "  FROM CV_DATA CD                                    ";
                strSql += cDefApp.CRLF + " WHERE CD.WH_TYP = :WH_TYP                           ";
                strSql += cDefApp.CRLF + "   AND CD.PLC_NO = :PLC_NO                           ";
                strSql += cDefApp.CRLF + "   AND CD.MC_NO BETWEEN :FROM_TRACK AND :TO_TRACK    ";
                strSql += cDefApp.CRLF + "   AND CD.CMD_RQ_YN = 'Y'                            ";
                strSql += cDefApp.CRLF + "ORDER BY CD.WRITE_UPD_DT, CD.TRACK_NO                ";
#if !SQL
                strSql += cDefApp.CRLF + "LIMIT 5;                                             ";
#endif


                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;
                m_msQPlc._pBdb.mComMain.Parameters.Add("FROM_TRACK", DbLang.VARCHAR, 255).Value = m_nFrTrackNo.ToString("000");
                m_msQPlc._pBdb.mComMain.Parameters.Add("TO_TRACK", DbLang.VARCHAR, 255).Value = m_nToTrackNo.ToString("000");

                nUpdCount = m_msQPlc._pBdb.ExcuteQry(strSql);

                if (nUpdCount < 0)
                {
                    MakeMsg_Error(strTitle + "트랙정보 읽는 중 에러(CV_DATA)", m_nthNo);
                    return false;
                }

                #endregion

                for (int nRows = 0; nRows < nUpdCount; nRows++)
                {
                    #region table정보 읽음

                    //LFC 사용
                    string TRACK_NO = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["MC_NO"].ToString();
                    string ADDR_NO = TRACK_NO.Substring(TRACK_NO.Length - 2, 2);
                    int nADDR_NO = (Convert.ToInt32(0 + ADDR_NO)) * 10; //시작트랙 * 10 -> 해당 어드레스

                    string CMD_RQ_ID = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["CMD_RQ_ID"].ToString();

                    string CMD_RQ_PARM = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["CMD_RQ_PARM"].ToString();
                    int nCMD_RQ_PARM = (Convert.ToInt32(0 + CMD_RQ_PARM));

                    string PULP_SENSOR_OD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["PULP_SENSOR_OD"].ToString(); // 대기필요
                    int nPULP_SENSOR_OD = (Convert.ToInt32(0 + PULP_SENSOR_OD));

                    string WAIT_SC_RET_JOB_OD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["WAIT_SC_RET_JOB_OD"].ToString(); // 대기필요
                    int nWAIT_SC_RET_JOB_OD = (Convert.ToInt32(0 + WAIT_SC_RET_JOB_OD));

                    string WAIT_SC_RET_JOB_RD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["WAIT_SC_RET_JOB_RD"].ToString(); // 대기필요
                    int nWAIT_SC_RET_JOB_RD = (Convert.ToInt32(0 + WAIT_SC_RET_JOB_RD));

                    string TR_PAUSE_OD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["TR_PAUSE_OD"].ToString();
                    int nTR_PAUSE_OD = (Convert.ToInt32(0 + TR_PAUSE_OD));

                    string TR_PAUSE_RD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["TR_PAUSE_RD"].ToString();
                    int nTR_PAUSE_RD = (Convert.ToInt32(0 + TR_PAUSE_RD));

                    string JOB_TYP_RD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["JOB_TYP_RD"].ToString();
                    int nJOB_TYP_RD = (Convert.ToInt32(0 + JOB_TYP_RD));
                    #endregion

                    /*
                     * 한 트랙에 전체 쓰기
                     */
                    #region
                    if (CMD_RQ_ID == "ROTATE")
                    {
                        Array.Clear(byTxBuff, 0, byTxBuff.Length);
                        byTxBuff[0] = (byte)(0 >> 0); //0
                        byTxBuff[1] = (byte)(0 >> 8); //128

                        int nWriteLen = 1;

                        if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                           (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                           nADDR_NO + 9,
                                                           nWriteLen,
                                                           byTxBuff) == false)
                        {
                            if (this.m_msQPlc.IsHex)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                            }
                            if (this.m_msQPlc.IsAscii)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                            }

                            m_strLogMsg = strTitle + "트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 실패";
                            if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                            {
                                return false;
                            }
                            return false;
                        }

                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }

                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 성공";
                        if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                        {
                            return false;
                        }

                        if (!UpdateCvDataCmd(TRACK_NO))
                        {
                            return false;
                        }
                    }
                    else if (CMD_RQ_ID == "DIR")
                    {
                        // [LGLS 2026-08-01] 입출고 방향(입고=0 / 출고=1) 지시.
                        //   HOST 의 M 전문(WMS C/V IO 모드 송신)을 WCS_TASK_HOST 가 이 커맨드로 남기면
                        //   여기서 설비 방향 워드에 직접 써서 실제 설비 방향을 바꾼다.
                        //   주소·표현은 CvStatusScenario 의 방향워드 판독과 **동일**해야 한다:
                        //     V1.1 : %MB960 + (N-1)*2 → 워드주소 (960 + (N-1)*2)/2,  V0.9 : D0300 + (N-1)
                        //   값은 설비가 ASCII 문자로 싣는 규약이라 '0'(0x30) / '1'(0x31) 을 기록한다
                        //   (그래서 판독값이 48/49 로 보인다).
                        int nDirCv = 0;
                        int.TryParse(System.Text.RegularExpressions.Regex.Match(m_strPlc_No, @"\d+").Value, out nDirCv);
                        if (nDirCv < 1)
                        {
                            MakeMsg_Error(strTitle + " 방향지시 PLC_NO 파싱 실패 [" + m_strPlc_No + "]", m_nthNo);
                            return false;
                        }
                        // [LGLS 2026-08-19] 판독(CvStatusScenario)과 같은 주소를 쓰도록 주소맵 XML 우선.
                        //   XML 이 없으면 종전 내장 계산식으로 폴백한다.
                        int nDirAddr = cDefApp.GM_ADDR_V09 ? (300 + (nDirCv - 1))
                                                           : ((960 + (nDirCv - 1) * 2) / 2);
                        if (!cDefApp.GM_ADDR_V09)
                        {
                            int xDirW = cPlcAddrMap.Addr("CV", nDirCv, "Direction", "IoDirection");
                            if (xDirW >= 0) nDirAddr = xDirW;
                        }

                        Array.Clear(byTxBuff, 0, byTxBuff.Length);
                        byTxBuff[0] = (byte)((nCMD_RQ_PARM == 1) ? 0x31 : 0x30);   // '1'=출고 / '0'=입고
                        byTxBuff[1] = 0;

                        if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                           (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                           nDirAddr,
                                                           1,
                                                           byTxBuff) == false)
                        {
                            m_strLogMsg = strTitle + " TRACK_NO : [" + TRACK_NO + "] 방향지시(" +
                                          ((nCMD_RQ_PARM == 1) ? "출고" : "입고") + ") 실패";
                            MakeMsg_Error(m_strLogMsg + " " + m_msQPlc.GetErrorMsg(), m_nthNo);
                            InsertWcsLogPgr(TRACK_NO, m_strLogMsg);
                            return false;
                        }

                        m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] 방향지시 " +
                                      ((nCMD_RQ_PARM == 1) ? "출고(1)" : "입고(0)") + " → D워드 " + nDirAddr;
                        MakeMsg_Imp(m_strLogMsg, m_nthNo);
                        if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                        {
                            return false;
                        }

                        // 캐시 무효화: 다음 판독에서 STOCK_MODE 변화를 즉시 DB에 반영
                        CvDic.Remove(Convert.ToInt32(TRACK_NO));

                        if (!UpdateCvDataCmd(TRACK_NO))
                        {
                            return false;
                        }
                    }
                    else if (CMD_RQ_ID == "WAIT")
                    {
                        Array.Clear(byTxBuff, 0, byTxBuff.Length);
                        byTxBuff[0] = (byte)(nCMD_RQ_PARM);
                        byTxBuff[1] = (byte)(0);

                        int nWriteLen = 1;

                        if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                           (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                           nADDR_NO + 4,
                                                           nWriteLen,
                                                           byTxBuff) == false)
                        {
                            if (this.m_msQPlc.IsHex)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                            }
                            if (this.m_msQPlc.IsAscii)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                            }

                            m_strLogMsg = strTitle + " TRACK_NO : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 실패";
                            if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                            {
                                return false;
                            }
                            return false;
                        }

                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }

                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 서공";
                        if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                        {
                            return false;
                        }

                        if (!UpdateCvDataCmd(TRACK_NO))
                        {
                            return false;
                        }
                    }
                    else if (CMD_RQ_ID == "NOREAD")
                    {
                        Array.Clear(byTxBuff, 0, byTxBuff.Length);
                        int nNoread = 102;
                        byTxBuff[0] = (byte)(nNoread >> 0);
                        byTxBuff[1] = (byte)(nNoread >> 8);

                        int nWriteLen = 1;

                        if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                           (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                           nADDR_NO + 6,
                                                           nWriteLen,
                                                           byTxBuff) == false)
                        {
                            if (this.m_msQPlc.IsHex)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                            }
                            if (this.m_msQPlc.IsAscii)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                            }

                            m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 실패";
                            if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                            {
                                return false;
                            }
                            return false;
                        }

                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }

                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 성공";
                        if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                        {
                            return false;
                        }

                        if (!UpdateCvDataCmd(TRACK_NO))
                        {
                            return false;
                        }
                    }
                    else if (CMD_RQ_ID == "RESET")  //에러 리셋
                    {

                        Array.Clear(byTxBuff, 0, byTxBuff.Length);
                        byTxBuff[0] = (byte)(0);
                        byTxBuff[1] = (byte)(0);
                        // byTxBuff[2] = (byte)(0);
                        //byTxBuff[3] = (byte)(0);

                        int nWriteLen = 1;

                        if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                           (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                           nADDR_NO + 6,
                                                           nWriteLen,
                                                           byTxBuff) == false)
                        {
                            if (this.m_msQPlc.IsHex)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                            }
                            if (this.m_msQPlc.IsAscii)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                            }

                            m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 실패";
                            if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                            {
                                return false;
                            }
                            return false;
                        }

                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }

                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 성공";
                        if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                        {
                            return false;
                        }

                        if (!UpdateCvDataCmd(TRACK_NO))
                        {
                            return false;
                        }
                    }
                    else if (CMD_RQ_ID == "1") // PULP_SENSOR
                    {
                        Array.Clear(byTxBuff, 0, byTxBuff.Length);
                        byTxBuff[0] = (byte)((nJOB_TYP_RD >> 0) | (0 << 4));
                        byTxBuff[1] = (byte)((nPULP_SENSOR_OD >> 0) | (0 << 4));
                        // byTxBuff[2] = (byte)(0);
                        //byTxBuff[3] = (byte)(0);

                        int nWriteLen = 1;

                        if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                           (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                           nADDR_NO + 2,
                                                           nWriteLen,
                                                           byTxBuff) == false)
                        {
                            if (this.m_msQPlc.IsHex)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                            }
                            if (this.m_msQPlc.IsAscii)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                            }

                            m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 실패";
                            if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                            {
                                return false;
                            }
                            return false;
                        }

                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }

                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 성공";
                        if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                        {
                            return false;
                        }

                        if (!UpdateCvDataCmd(TRACK_NO))
                        {
                            return false;
                        }
                    }
                    else if (CMD_RQ_ID == "2")//대기필요
                    {
                        Array.Clear(byTxBuff, 0, byTxBuff.Length);
                        byTxBuff[0] = (byte)((nTR_PAUSE_RD >> 0) | (nWAIT_SC_RET_JOB_OD << 4)); //TR_PAUSE는 기존에 가지고있던 rd값.
                        byTxBuff[1] = (byte)(0);
                        // byTxBuff[2] = (byte)(0);
                        //byTxBuff[3] = (byte)(0);

                        int nWriteLen = 1;

                        if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                           (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                           nADDR_NO + 5,
                                                           nWriteLen,
                                                           byTxBuff) == false)
                        {
                            if (this.m_msQPlc.IsHex)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                            }
                            if (this.m_msQPlc.IsAscii)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                            }

                            m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 실패";
                            if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                            {
                                return false;
                            }
                            return false;
                        }

                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }

                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 성공";
                        if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                        {
                            return false;
                        }

                        if (!UpdateCvDataCmd(TRACK_NO))
                        {
                            return false;
                        }
                    }
                    else if (CMD_RQ_ID == "3") //트랙 대기
                    {
                        Array.Clear(byTxBuff, 0, byTxBuff.Length);
                        // [LGLS 2026-08-04] TR_PAUSE 는 이 현장에서 PLC 비트가 아니라 프로그램 내부 변수(CV_DATA 필드) 전용이다.
                        //   설비로 내보내지 않는다 - 하위 4bit(TR_PAUSE)는 0 으로 두고 대기필요만 기존 rd 값으로 기록.
                        byTxBuff[0] = (byte)(nWAIT_SC_RET_JOB_RD << 4);//대기필요는 기존에 가지고 있던 rd값. (TR_PAUSE_OD 미전송)
                        byTxBuff[1] = (byte)(0);
                        // byTxBuff[2] = (byte)(0);
                        //byTxBuff[3] = (byte)(0);

                        int nWriteLen = 1;

                        if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                           (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                           nADDR_NO + 5,
                                                           nWriteLen,
                                                           byTxBuff) == false)
                        {
                            if (this.m_msQPlc.IsHex)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                            }
                            if (this.m_msQPlc.IsAscii)
                            {
                                MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                                MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                            }

                            m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 실패";
                            if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                            {
                                return false;
                            }
                            return false;
                        }

                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }

                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] CMD_RQ_ID : [" + CMD_RQ_ID + "] 커맨드 지시 성공";
                        if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                        {
                            return false;
                        }

                        if (!UpdateCvDataCmd(TRACK_NO))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (!UpdateCvDataCmd(TRACK_NO))
                        {
                            return false;
                        }
                    }
                    #endregion
                }

                return true;
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
                SetErrorMsg("Comm" + m_nthNo + ex.Message);
                MakeMsg_Error(ex.Message, m_nthNo);
                return false;
            }
        }
        #endregion

        #region [CvChg_OD_RQ_YN] :: CV_DATA에서 OD_RQ_YN 여부에 따른 CV 지시
        private bool CvChg_OD_RQ_YN(int Idx)
        {
            string strTitle = "[CvChg_OD_RQ_YN]";

            try
            {
                string strSql = "";

                byte[] byTxBuff = new byte[1000];

                int nUpdCount = 0;
                /*
                 * 변경할 트랙 정보 읽음
                 */
                #region
                strSql = "";
#if SQL
                strSql += cDefApp.CRLF + "SELECT TOP 5 CD.*                                    "; // [LGLS] SQL Server: LIMIT → TOP
#else
                strSql += cDefApp.CRLF + "SELECT CD.*                                          ";
#endif
                strSql += cDefApp.CRLF + "  FROM CV_DATA CD                                    ";
                strSql += cDefApp.CRLF + " WHERE CD.WH_TYP = :WH_TYP                           ";
                strSql += cDefApp.CRLF + "   AND CD.PLC_NO = :PLC_NO                           ";
                strSql += cDefApp.CRLF + "   AND CD.MC_NO BETWEEN :FROM_TRACK AND :TO_TRACK    ";
                strSql += cDefApp.CRLF + "   AND CD.OD_RQ_YN = 'Y'                             ";
                strSql += cDefApp.CRLF + "ORDER BY CD.WRITE_UPD_DT, CD.TRACK_NO                ";
#if !SQL
                strSql += cDefApp.CRLF + "LIMIT 5;                                             ";
#endif


                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;
                m_msQPlc._pBdb.mComMain.Parameters.Add("FROM_TRACK", DbLang.VARCHAR, 255).Value = m_nFrTrackNo.ToString("000");
                m_msQPlc._pBdb.mComMain.Parameters.Add("TO_TRACK", DbLang.VARCHAR, 255).Value = m_nToTrackNo.ToString("000");

                nUpdCount = m_msQPlc._pBdb.ExcuteQry(strSql);

                if (nUpdCount < 0)
                {
                    MakeMsg_Error(strTitle + "트랙정보 읽는 중 에러(CV_DATA)", m_nthNo);
                    return false;
                }

                #endregion

                for (int nRows = 0; nRows < nUpdCount; nRows++)
                {
                    #region table정보 읽음

                    //LFC 사용
                    string TRACK_NO = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["MC_NO"].ToString();
                    string ADDR_NO = TRACK_NO.Substring(TRACK_NO.Length - 2, 2);
                    int nADDR_NO = (Convert.ToInt32(0 + ADDR_NO)) * 10; //시작트랙 * 10 -> 해당 어드레스

                    string LUGG_NO_OD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["LUGG_NO_OD"].ToString();
                    int nLUGG_NO_OD = (Convert.ToInt32(0 + LUGG_NO_OD));

                    string DEST_POS_OD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["DEST_POS_OD"].ToString();
                    int nDEST_POS_OD = (Convert.ToInt32(0 + DEST_POS_OD));

                    string JOB_TYP_OD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["JOB_TYP_OD"].ToString();
                    int nJOB_TYP_OD = (Convert.ToInt32(0 + JOB_TYP_OD));

                    string PULP_SENSOR_RD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["PULP_SENSOR_RD"].ToString();
                    int nPULP_SENSOR_RD = (Convert.ToInt32(0 + PULP_SENSOR_RD));

                    string PULP_SENSOR_OD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["PULP_SENSOR_OD"].ToString();
                    int nPULP_SENSOR_OD = (Convert.ToInt32(0 + PULP_SENSOR_OD));

                    string WAIT_SC_RET_JOB_RD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["WAIT_SC_RET_JOB_RD"].ToString();
                    int nWAIT_SC_RET_JOB_RD = (Convert.ToInt32(0 + WAIT_SC_RET_JOB_RD));

                    string WAIT_SC_RET_JOB_OD = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["WAIT_SC_RET_JOB_OD"].ToString();
                    int nWAIT_SC_RET_JOB_OD = (Convert.ToInt32(0 + WAIT_SC_RET_JOB_OD));

                    #endregion

                    /*
                     * 한 트랙에 전체 쓰기
                     */
                    #region

                    Array.Clear(byTxBuff, 0, byTxBuff.Length);

                    MakeMsg_Imp("Track #" + TRACK_NO + " Writting"
                                                + ", 작업구분:" + JOB_TYP_OD
                                                + ", 작업번호:" + LUGG_NO_OD
                                                + ", 도착위치:" + DEST_POS_OD
                                                , m_nthNo);

                    byTxBuff[0] = (byte)(nLUGG_NO_OD >> 0);
                    byTxBuff[1] = (byte)(nLUGG_NO_OD >> 8);
                    byTxBuff[2] = (byte)(nDEST_POS_OD >> 0);
                    byTxBuff[3] = (byte)(nDEST_POS_OD >> 8);
                    byTxBuff[4] = (byte)((nJOB_TYP_OD >> 0) | (0 << 4));
                    byTxBuff[5] = (byte)((nPULP_SENSOR_OD >> 0) | (0 << 4)); //nPULP_SENSOR_OD
                    //byTxBuff[6] = (byte)((0 >> 0));
                    //byTxBuff[7] = (byte)(0 >> 0);
                    //byTxBuff[8] = (byte)(0 >> 0);
                    //byTxBuff[9] = (byte)(0 >> 0);
                    //byTxBuff[10] = (byte)((0 >> 0) | (nWAIT_SC_RET_JOB_RD << 4));
                    //byTxBuff[11] = (byte)(0 >> 0);

                    //int nWriteLen = 5;
                    int nWriteLen = 3;

                    if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                       (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                       nADDR_NO,
                                                       nWriteLen,
                                                       byTxBuff) == false)
                    {
                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }
                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] 작업번호 : [" + LUGG_NO_OD + "] 도착지 : [" + DEST_POS_OD + "] 작업구분 : [" + JOB_TYP_OD + "] CV 지시 실패";
                        if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                        {
                            return false;
                        }

                        return false;
                    }
                    if (this.m_msQPlc.IsHex)
                    {
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                    }
                    if (this.m_msQPlc.IsAscii)
                    {
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                    }

                    m_strLogMsg = strTitle + " 트랙번호 : [" + TRACK_NO + "] 작업번호 : [" + LUGG_NO_OD + "] 도착지 : [" + DEST_POS_OD + "] 작업구분 : [" + JOB_TYP_OD + "] CV 지시 성공";
                    if (!InsertWcsLogPgr(TRACK_NO, m_strLogMsg))
                    {
                        return false;
                    }

                    if (!UpdateCvDataOD(TRACK_NO))
                    {
                        return false;
                    }

                    #endregion
                }

                return true;
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
                SetErrorMsg("Comm" + m_nthNo + ex.Message);
                MakeMsg_Error(ex.Message, m_nthNo);
                return false;
            }
        }
        #endregion

        #region [UpdateCvData] :: CV_DATA 상태값 변경
        public bool UpdateCvData(string pLUGG_NO_RD
                                , string pDEST_POS_RD
                                , string pJOB_TYP_RD
                                , string pIS_TURN_RD
                                , string pTRAY_TYP_RD
                                , string pTRAY_LEV_RD
                                , string pFMS_RPT_RD
                                , string pTR_PAUSE_RD
                                , string pERR_RQ_RD
                                , int pnErrorCode
                                , string pERROR_CODE
                                , string pAUTO_MODE_RD
                                , string pSTO_READY_RD
                                , string pRET_READY_RD
                                , string pSTOHS_READY_RD
                                , string pRETHS_READY_RD
                                , string pSENSOR0_DATA_RD
                                , string pSENSOR1_DATA_RD
                                , string pSENSOR2_DATA_RD
                                , string nCvNo
                                , string pREMOTE_CONTROL
                                , string pROLL_MODE
                                , string pSTOCK_MODE
                                , string pA_TURN_YN
                                , string pB_TURN_YN
                                , string pPULP_SENSOR_RD
                                , string pWAIT_SC_RET_JOB_RD
                                , string pDELETE_TRACK_RD
                                , string pSC_LOCK_SENSOR
                                , string pDRIV_PAPER_POS
                                , string pELEV_ASC_ERR
                                , string pELEV_DESC_ERR
                                , string pCLAMP_FORWARD_ERR
                                , string pCLAMP_BACKWARD_ERR
                                , string pDRIV_FORWARD_ERR
                                , string pDRIV_BACKWARD_ERR
                                , string pPAPER_BLOCK_SENSOR1
                                , string pPAPER_BLOCK_SENSOR2
                                , string pPAPER_BLOCK_SENSOR3
                                , string pPAPER_BLOCK_SENSOR4
                                , string pPAPER_FULL_SENSOR
                                , string pDRIV_FORWARD_POS
                                , string pDRIV_BACKWARD_POS
                                , string pCRUSH_PAPER_SENSOR
                                , string pCLAMP_FORWARD_SENSOR
                                , string pCLAMP_BACKWARD_SENSOR)
        {
            string strTitle = "[UpdateCvData]";

            try
            {
                m_msQPlc._pBdb.BeginTrans();

                strSql = "";
                strSql += cDefApp.CRLF + " UPDATE CV_DATA                                                     ";
                strSql += cDefApp.CRLF + "    SET LUGG_NO_RD = :LUGG_NO_RD                                    ";
                strSql += cDefApp.CRLF + "       ,DEST_POS_RD = :DEST_POS_RD                                  ";
                //strSql += cDefApp.CRLF + "       ,IS_TURN_RD = :IS_TURN_RD                                    ";
                strSql += cDefApp.CRLF + "       ,JOB_TYP_RD = :JOB_TYP_RD                                    ";
                //strSql += cDefApp.CRLF + "       ,TRAY_LEV_RD = :TRAY_LEV_RD                                  ";
                //strSql += cDefApp.CRLF + "       ,TRAY_TYP_RD = :TRAY_TYP_RD                                  ";
                //strSql += cDefApp.CRLF + "       ,FMS_RPT_RD = :FMS_RPT_RD                                    ";
                // [LGLS 2026-08-04] TR_PAUSE_RD 는 이 시스템에서 PLC 비트가 아니라 **프로그램 내부 변수**로만 쓴다.
                //   (Client 트랙 일시정지 / IO_TASK 게이트가 세팅) 설비 관측값으로 덮어쓰면 매 주기 0 으로 지워지므로 UPDATE 대상에서 제외한다.
                //strSql += cDefApp.CRLF + "       ,TR_PAUSE_RD = :TR_PAUSE_RD                                  ";
               // strSql += cDefApp.CRLF + "       ,WAIT_TIME_RD = :WAIT_TIME_RD                                ";
                //strSql += cDefApp.CRLF + "       ,ERR_RQ_RD = :ERR_RQ_RD                                      ";
                strSql += cDefApp.CRLF + "       ,STO_READY_RD = :STO_READY_RD                                ";
                strSql += cDefApp.CRLF + "       ,RET_READY_RD = :RET_READY_RD                                ";
                strSql += cDefApp.CRLF + "       ,STOHS_READY_RD = :STOHS_READY_RD                            ";
                strSql += cDefApp.CRLF + "       ,RETHS_READY_RD = :RETHS_READY_RD                            ";
                strSql += cDefApp.CRLF + "       ,AUTO_MODE_RD = :AUTO_MODE_RD                                ";
                strSql += cDefApp.CRLF + "       ,SENSOR0_DATA_RD = :SENSOR0_DATA_RD                          ";
                strSql += cDefApp.CRLF + "       ,SENSOR1_DATA_RD = :SENSOR1_DATA_RD                          ";
                strSql += cDefApp.CRLF + "       ,SENSOR2_DATA_RD = :SENSOR2_DATA_RD                          ";
                strSql += cDefApp.CRLF + "       ,ERROR_CODE = :ERROR_CODE                                    ";
                strSql += cDefApp.CRLF + "       ,REMOTE_CONTROL = :REMOTE_CONTROL                            ";
                strSql += cDefApp.CRLF + "       ,ROLL_MODE = :ROLL_MODE                                      ";
                strSql += cDefApp.CRLF + "       ,STOCK_MODE = :STOCK_MODE                                    ";
                strSql += cDefApp.CRLF + "       ,A_TURN_YN = :A_TURN_YN                                      ";
                strSql += cDefApp.CRLF + "       ,B_TURN_YN = :B_TURN_YN                                      ";
                strSql += cDefApp.CRLF + "       ,PULP_SENSOR_RD = :PULP_SENSOR_RD                            ";
                strSql += cDefApp.CRLF + "       ,WAIT_SC_RET_JOB_RD = :WAIT_SC_RET_JOB_RD                    ";
                strSql += cDefApp.CRLF + "       ,DELETE_TRACK_RD = :DELETE_TRACK_RD                          ";
                strSql += cDefApp.CRLF + "       ,SC_LOCK_SENSOR = :SC_LOCK_SENSOR                            ";
                strSql += cDefApp.CRLF + "       ,READ_UPD_DT = " + DbLang.SYSDATE + "                        ";
                strSql += cDefApp.CRLF + "       ,OD_RQ_FLAG = 'N'                                            ";
                if (m_blHostSendYN == true)
                {
                    strSql += cDefApp.CRLF + "       ,HOST_SEND_YN = 'N'                                      ";
                }
                if (m_blHostErrSendYN == true)
                {
                    strSql += cDefApp.CRLF + "       ,HOST_ERR_SEND_YN = 'N'                                  ";
                }
                strSql += cDefApp.CRLF + "       ,DRIV_PAPER_POS = :DRIV_PAPER_POS					          ";
                strSql += cDefApp.CRLF + "       ,ELEV_ASC_ERR = :ELEV_ASC_ERR					              ";
                strSql += cDefApp.CRLF + "       ,ELEV_DESC_ERR = :ELEV_DESC_ERR                              ";
                strSql += cDefApp.CRLF + "       ,CLAMP_FORWARD_ERR = :CLAMP_FORWARD_ERR                      ";
                strSql += cDefApp.CRLF + "       ,CLAMP_BACKWARD_ERR = :CLAMP_BACKWARD_ERR                    ";
                strSql += cDefApp.CRLF + "       ,DRIV_FORWARD_ERR = :DRIV_FORWARD_ERR                        ";
                strSql += cDefApp.CRLF + "       ,DRIV_BACKWARD_ERR = :DRIV_BACKWARD_ERR                      ";
                strSql += cDefApp.CRLF + "       ,PAPER_BLOCK_SENSOR1 = :PAPER_BLOCK_SENSOR1                  ";
                strSql += cDefApp.CRLF + "       ,PAPER_BLOCK_SENSOR2 = :PAPER_BLOCK_SENSOR2                  ";
                strSql += cDefApp.CRLF + "       ,PAPER_BLOCK_SENSOR3 = :PAPER_BLOCK_SENSOR3                  ";
                strSql += cDefApp.CRLF + "       ,PAPER_BLOCK_SENSOR4 = :PAPER_BLOCK_SENSOR4                  ";
                strSql += cDefApp.CRLF + "       ,PAPER_FULL_SENSOR = :PAPER_FULL_SENSOR                      ";
                strSql += cDefApp.CRLF + "       ,DRIV_FORWARD_POS = :DRIV_FORWARD_POS                        ";
                strSql += cDefApp.CRLF + "       ,DRIV_BACKWARD_POS = :DRIV_BACKWARD_POS                      ";
                strSql += cDefApp.CRLF + "       ,CRUSH_PAPER_SENSOR = :CRUSH_PAPER_SENSOR                    ";
                strSql += cDefApp.CRLF + "       ,CLAMP_FORWARD_SENSOR = :CLAMP_FORWARD_SENSOR                ";
                strSql += cDefApp.CRLF + "       ,CLAMP_BACKWARD_SENSOR = :CLAMP_BACKWARD_SENSOR              ";
                strSql += cDefApp.CRLF + "WHERE  WH_TYP   = :WH_TYP                                           ";
                strSql += cDefApp.CRLF + "AND    PLC_NO   = :PLC_NO                                           ";
                strSql += cDefApp.CRLF + "AND    MC_NO    = :MC_NO                                         ";

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("LUGG_NO_RD", DbLang.VARCHAR).Value = pLUGG_NO_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("DEST_POS_RD", DbLang.VARCHAR).Value = pDEST_POS_RD.PadLeft(3, '0');
                //m_msQPlc._pBdb.mComMain.Parameters.Add("IS_TURN_RD", DbLang.VARCHAR).Value = pIS_TURN_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("JOB_TYP_RD", DbLang.VARCHAR).Value = pJOB_TYP_RD;
               // m_msQPlc._pBdb.mComMain.Parameters.Add("TRAY_LEV_RD", DbLang.VARCHAR).Value = pTRAY_LEV_RD;
                //m_msQPlc._pBdb.mComMain.Parameters.Add("TRAY_TYP_RD", DbLang.VARCHAR).Value = pTRAY_TYP_RD;
                //m_msQPlc._pBdb.mComMain.Parameters.Add("FMS_RPT_RD", DbLang.VARCHAR).Value = pFMS_RPT_RD;
                //m_msQPlc._pBdb.mComMain.Parameters.Add("TR_PAUSE_RD", DbLang.VARCHAR).Value = pTR_PAUSE_RD;   // [LGLS 2026-08-04] 내부 변수 - PLC 관측값으로 덮어쓰지 않음
                //m_msQPlc._pBdb.mComMain.Parameters.Add("WAIT_TIME_RD", DbLang.VARCHAR).Value = pWAIT_TIME_RD;
               // m_msQPlc._pBdb.mComMain.Parameters.Add("ERR_RQ_RD", DbLang.VARCHAR).Value = pERR_RQ_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("STO_READY_RD", DbLang.VARCHAR).Value = pSTO_READY_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("RET_READY_RD", DbLang.VARCHAR).Value = pRET_READY_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("STOHS_READY_RD", DbLang.VARCHAR).Value = pSTOHS_READY_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("RETHS_READY_RD", DbLang.VARCHAR).Value = pRETHS_READY_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("AUTO_MODE_RD", DbLang.VARCHAR).Value = pAUTO_MODE_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("SENSOR0_DATA_RD", DbLang.VARCHAR).Value = pSENSOR0_DATA_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("SENSOR1_DATA_RD", DbLang.VARCHAR).Value = pSENSOR1_DATA_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("SENSOR2_DATA_RD", DbLang.VARCHAR).Value = pSENSOR2_DATA_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("ERROR_CODE", DbLang.VARCHAR).Value = pnErrorCode;
                m_msQPlc._pBdb.mComMain.Parameters.Add("REMOTE_CONTROL", DbLang.VARCHAR).Value = pREMOTE_CONTROL;
                m_msQPlc._pBdb.mComMain.Parameters.Add("ROLL_MODE", DbLang.VARCHAR).Value = pROLL_MODE;
                m_msQPlc._pBdb.mComMain.Parameters.Add("STOCK_MODE", DbLang.VARCHAR).Value = pSTOCK_MODE;
                m_msQPlc._pBdb.mComMain.Parameters.Add("A_TURN_YN", DbLang.VARCHAR).Value = pA_TURN_YN;
                m_msQPlc._pBdb.mComMain.Parameters.Add("B_TURN_YN", DbLang.VARCHAR).Value = pB_TURN_YN;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PULP_SENSOR_RD", DbLang.VARCHAR).Value = pPULP_SENSOR_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("WAIT_SC_RET_JOB_RD", DbLang.VARCHAR).Value = pWAIT_SC_RET_JOB_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("DELETE_TRACK_RD", DbLang.VARCHAR).Value = pDELETE_TRACK_RD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("SC_LOCK_SENSOR", DbLang.VARCHAR).Value = pSC_LOCK_SENSOR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("DRIV_PAPER_POS", DbLang.VARCHAR).Value = pDRIV_PAPER_POS;
                m_msQPlc._pBdb.mComMain.Parameters.Add("ELEV_ASC_ERR", DbLang.VARCHAR).Value = pELEV_ASC_ERR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("ELEV_DESC_ERR", DbLang.VARCHAR).Value = pELEV_DESC_ERR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("CLAMP_FORWARD_ERR", DbLang.VARCHAR).Value = pCLAMP_FORWARD_ERR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("CLAMP_BACKWARD_ERR", DbLang.VARCHAR).Value = pCLAMP_BACKWARD_ERR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("DRIV_FORWARD_ERR", DbLang.VARCHAR).Value = pDRIV_FORWARD_ERR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("DRIV_BACKWARD_ERR", DbLang.VARCHAR).Value = pDRIV_BACKWARD_ERR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PAPER_BLOCK_SENSOR1", DbLang.VARCHAR).Value = pPAPER_BLOCK_SENSOR1;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PAPER_BLOCK_SENSOR2", DbLang.VARCHAR).Value = pPAPER_BLOCK_SENSOR2;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PAPER_BLOCK_SENSOR3", DbLang.VARCHAR).Value = pPAPER_BLOCK_SENSOR3;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PAPER_BLOCK_SENSOR4", DbLang.VARCHAR).Value = pPAPER_BLOCK_SENSOR4;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PAPER_FULL_SENSOR", DbLang.VARCHAR).Value = pPAPER_FULL_SENSOR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("DRIV_FORWARD_POS", DbLang.VARCHAR).Value = pDRIV_FORWARD_POS;
                m_msQPlc._pBdb.mComMain.Parameters.Add("DRIV_BACKWARD_POS", DbLang.VARCHAR).Value = pDRIV_BACKWARD_POS;
                m_msQPlc._pBdb.mComMain.Parameters.Add("CRUSH_PAPER_SENSOR", DbLang.VARCHAR).Value = pCRUSH_PAPER_SENSOR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("CLAMP_FORWARD_SENSOR", DbLang.VARCHAR).Value = pCLAMP_FORWARD_SENSOR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("CLAMP_BACKWARD_SENSOR", DbLang.VARCHAR).Value = pCLAMP_BACKWARD_SENSOR;
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR).Value = m_strPlc_No;
                m_msQPlc._pBdb.mComMain.Parameters.Add("MC_NO", DbLang.VARCHAR).Value = nCvNo;

                nSelCnt = m_msQPlc._pBdb.ExcuteNonQry(strSql);

                if (nSelCnt < 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    SetErrorMsg("Comm" + m_nthNo + " :" + strTitle + "트랙정보 변경 중 에러(CV_DATA)., MSG [" + m_msQPlc._pBdb.ErrMsg + "]");
                    MakeMsg_Error(strTitle + "트랙정보 변경 중 에러(CV_DATA)., MSG [" + m_msQPlc._pBdb.ErrMsg + "]", m_nthNo);
                    return false;
                }

                if (nSelCnt == 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    SetErrorMsg("Comm" + m_nthNo + " :" + strTitle + "트랙정보 변경 중 DATA가 없습니다., PLC_NO [" + m_strPlc_No + "] " + "TRACK_NO [" + nCvNo + "]");
                    MakeMsg_Error(strTitle + "트랙정보 변경 중 DATA가 없습니다., TRACK_NO [" + nCvNo.ToString() + "]", m_nthNo);
                    return false;
                }


                m_msQPlc._pBdb.Commit();
                return true;

            }
            catch (Exception ex)
            {
                m_msQPlc._pBdb.Rollback();
                SetErrorMsg("Comm" + m_nthNo + " :" + strTitle + "트랙정보 변경 중 에러(CV_DATA)., EXCEPTION MSG [" + ex.ToString() + "]");
                MakeMsg_Error(strTitle + "트랙정보 변경 중 에러(CV_DATA)., EXCEPTION MSG [" + ex.ToString() + "]", m_nthNo);
                return false;
            }
        }
        #endregion


        #region [InsertWcsLogPgr] :: WCS_LOG_PGR에 LOG 남기기
        public bool InsertWcsLogPgr(string strTRACK_NO, string strLOG_MSG)
        {
            try
            {
                m_msQPlc._pBdb.BeginTrans();

                strSql = "";
                strSql += cDefApp.CRLF + "INSERT INTO WCS_LOG_PGR (WH_TYP                ";
                strSql += cDefApp.CRLF + "						  ,INS_DT                ";
                strSql += cDefApp.CRLF + "						  ,LOG_SEQ               ";
                strSql += cDefApp.CRLF + "						  ,LUGG_NO               ";
                strSql += cDefApp.CRLF + "						  ,BCR_BOTTOM            ";
                strSql += cDefApp.CRLF + "						  ,BCR_TOP               ";
                strSql += cDefApp.CRLF + "						  ,PGR_NM                ";
                strSql += cDefApp.CRLF + "						  ,LOG_KOR               ";
                strSql += cDefApp.CRLF + "						  ,TRACK_FROM            ";
                strSql += cDefApp.CRLF + "						  ,TRACK_TO              ";
                strSql += cDefApp.CRLF + "						  ,JOB_STA               ";
                strSql += cDefApp.CRLF + "						  ,RQ_INS_ID             ";
                strSql += cDefApp.CRLF + "						  ,RQ_INS_DT             ";
                strSql += cDefApp.CRLF + "						  ,EQP_TYP )             ";
                strSql += cDefApp.CRLF + "				VALUES    (:WH_TYP               ";
                strSql += cDefApp.CRLF + "						  ," + DbLang.SYSDATE + "";
#if SQL
                // [LGLS] SQL Server 2008 은 시퀀스 미지원 → MAX+1 (단일 기록 스레드라 충분)
                strSql += cDefApp.CRLF + "						  ,ISNULL((SELECT MAX(LOG_SEQ) FROM WCS_LOG_PGR), 0) + 1";
#else
                strSql += cDefApp.CRLF + "						  ,NEXTVAL('LOG_SEQ')    ";
#endif
                strSql += cDefApp.CRLF + "						  ,NULL                  ";
                strSql += cDefApp.CRLF + "						  ,NULL                  ";
                strSql += cDefApp.CRLF + "						  ,NULL                  ";
                strSql += cDefApp.CRLF + "						  ,:PGR_NM               ";
                strSql += cDefApp.CRLF + "						  ,:LOG_KOR              ";
                strSql += cDefApp.CRLF + "						  ,NULL                  ";
                strSql += cDefApp.CRLF + "						  ,NULL                  ";
                strSql += cDefApp.CRLF + "						  ,:JOB_STA              ";
                strSql += cDefApp.CRLF + "						  ,:RQ_INS_ID            ";
                strSql += cDefApp.CRLF + "						  ," + DbLang.SYSDATE + "";
                strSql += cDefApp.CRLF + "						  ,:EQP_TYP )            ";


                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();

                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PGR_NM", DbLang.VARCHAR, 255).Value = m_strLogFileNm;
                m_msQPlc._pBdb.mComMain.Parameters.Add("LOG_KOR", DbLang.VARCHAR, 255).Value = strLOG_MSG;
                m_msQPlc._pBdb.mComMain.Parameters.Add("JOB_STA", DbLang.VARCHAR, 255).Value = "999";
                m_msQPlc._pBdb.mComMain.Parameters.Add("RQ_INS_ID", DbLang.VARCHAR, 255).Value = strTRACK_NO;
                m_msQPlc._pBdb.mComMain.Parameters.Add("EQP_TYP", DbLang.VARCHAR, 255).Value = m_strEqmt_typ;
                nSelCnt = m_msQPlc._pBdb.ExcuteNonQry(strSql);

                if (nSelCnt < 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    SetErrorMsg("Comm" + m_nthNo + " :[InsertWcsLogPgr] 쓰기지시 후 상태값 변경중 ERROR., PLC_NO [" + m_strPlc_No + "] TRACK_NO [" + strTRACK_NO + "] MSG [" + m_msQPlc._pBdb.ErrMsg + "]");
                    MakeMsg_Error("[InsertWcsLogPgr] 쓰기지시 후 상태값 변경중 ERROR., PLC_NO [" + m_strPlc_No + "] TRACK_NO [" + strTRACK_NO + "] MSG [" + m_msQPlc._pBdb.ErrMsg + "]", m_nthNo);
                    return false;
                }

                if (nSelCnt == 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    SetErrorMsg("Comm" + m_nthNo + " :[InsertWcsLogPgr]쓰기지시 후 상태값 변경중 DATA가 없습니다., PLC_NO [" + m_strPlc_No + "] TRACK_NO [" + strTRACK_NO + "]");
                    MakeMsg_Error("[InsertWcsLogPgr] 쓰기지시 후 상태값 변경중 DATA가 없습니다.,PLC_NO [" + m_strPlc_No + "]  TRACK_NO [" + strTRACK_NO + "]", m_nthNo);
                    return false;

                }

                m_msQPlc._pBdb.Commit();
                return true;

            }
            catch (Exception ex)
            {
                m_msQPlc._pBdb.Rollback();
                SetErrorMsg("Comm" + m_nthNo + " :[InsertWcsLogPgr] 쓰기지시 후 상태값 변경중 ERROR., PLC_NO [" + m_strPlc_No + "] TRACK_NO  [" + strTRACK_NO + "] MSG [" + ex.ToString() + "]");
                MakeMsg_Error("[InsertWcsLogPgr] 쓰기지시 후 상태값 변경중 ERROR., PLC_NO [" + strTRACK_NO + "] MSG [" + ex.ToString() + "]", m_nthNo);
                return false;
            }
        }
        #endregion

        #region [UpdateCvDataCmd] :: CV_DATA의 CMD_RQ_YN = 'N' 업데이트
        public bool UpdateCvDataCmd(string strTRACK_NO)
        {
            try
            {
                m_msQPlc._pBdb.BeginTrans();

                strSql = "";
                strSql += CRLF + "UPDATE CV_DATA						";
                strSql += CRLF + "   SET CMD_RQ_YN       = 'N'			";
                strSql += CRLF + "WHERE  WH_TYP          = :WH_TYP		";
                strSql += CRLF + "AND    PLC_NO          = :PLC_NO		";
                strSql += CRLF + "AND    MC_NO           = :MC_NO	    ";
                strSql += CRLF + "AND    CMD_RQ_YN       = 'Y'       	";

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;
                m_msQPlc._pBdb.mComMain.Parameters.Add("MC_NO", DbLang.VARCHAR, 255).Value = strTRACK_NO;

                nSelCnt = m_msQPlc._pBdb.ExcuteNonQry(strSql);

                if (nSelCnt < 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    MakeMsg_Error("[UpdateCvDataCmd] 쓰기지시 후 상태값 변경중 ERROR., TRACK_NO [" + strTRACK_NO + "] MSG [" + m_msQPlc._pBdb.ErrMsg + "]", m_nthNo);
                    return false;
                }

                if (nSelCnt == 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    MakeMsg_Error("[UpdateCvDataCmd]쓰기지시 후 상태값 변경중 DATA가 없습니다., TRACK_NO [" + strTRACK_NO + "]", m_nthNo);
                    return false;
                }

                m_msQPlc._pBdb.Commit();
                return true;
            }
            catch (Exception ex)
            {
                m_msQPlc._pBdb.Rollback();
                MakeMsg_Error("[UpdateCvDataCmd] 쓰기지시 후 상태값 변경중 ERROR., TRACK_NO [" + strTRACK_NO + "] MSG [" + ex.ToString() + "]", m_nthNo);
                return false;
            }
        }
        #endregion

        #region [UpdateCvDataOD] :: CV_DATA의 OD_RQ_YN = 'N' 업데이트
        public bool UpdateCvDataOD(string strTRACK_NO)
        {
            try
            {
                m_msQPlc._pBdb.BeginTrans();

                strSql = "";
                strSql += CRLF + "UPDATE CV_DATA						               ";
                strSql += CRLF + "   SET OD_RQ_YN        = 'N'			               ";
                strSql += CRLF + "      ,OD_RQ_FLAG      = 'Y'			               ";
                strSql += CRLF + "      ,WRITE_UPD_DT    = " + DbLang.SYSDATE + "      ";
                strSql += CRLF + "WHERE  WH_TYP          = :WH_TYP		               ";
                strSql += CRLF + "AND    PLC_NO          = :PLC_NO		               ";
                strSql += CRLF + "AND    MC_NO           = :MC_NO    	               ";
                strSql += CRLF + "AND    OD_RQ_YN        = 'Y'       	               ";

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;
                m_msQPlc._pBdb.mComMain.Parameters.Add("MC_NO", DbLang.VARCHAR, 255).Value = strTRACK_NO;

                nSelCnt = m_msQPlc._pBdb.ExcuteNonQry(strSql);

                if (nSelCnt < 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    MakeMsg_Error("[UpdateCvDataOD] 쓰기지시 후 상태값 변경중 ERROR., TRACK_NO [" + strTRACK_NO + "] MSG [" + m_msQPlc._pBdb.ErrMsg + "]", m_nthNo);
                    return false;
                }

                if (nSelCnt == 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    MakeMsg_Error("[UpdateCvDataOD]쓰기지시 후 상태값 변경중 DATA가 없습니다., TRACK_NO [" + strTRACK_NO + "]", m_nthNo);
                    return false;

                }

                m_msQPlc._pBdb.Commit();
                return true;
            }
            catch (Exception ex)
            {
                m_msQPlc._pBdb.Rollback();
                MakeMsg_Error("[UpdateCvDataOD] 쓰기지시 후 상태값 변경중 ERROR., TRACK_NO [" + strTRACK_NO + "] MSG [" + ex.ToString() + "]", m_nthNo);
                return false;
            }
        }
        #endregion

        #region [UpdateEQMT_ERR_LOG] :: CV 에러상태면 이력에 남기기
        public bool UpdateEQMT_ERR_LOG(string pWH_TYP,
                                       string pEQP_TYP,
                                       string pEQP_NO,
                                       string pEQP_ERR_CD,
                                       string pLUGG_NO)
        {
            try
            {
                m_msQPlc._pBdb.BeginTrans();

                strSql = "";
                strSql += cDefApp.CRLF + "INSERT INTO EQP_ERR_HIS (WH_TYP                ";
                strSql += cDefApp.CRLF + "                       , EQP_TYP               ";
                strSql += cDefApp.CRLF + "                       , EQP_NO                ";
                strSql += cDefApp.CRLF + "                       , ERROR_DT              ";
                strSql += cDefApp.CRLF + "                       , EQP_ERR_CD            ";
                strSql += cDefApp.CRLF + "                       , BCR_BOTTOM            ";
                strSql += cDefApp.CRLF + "                       , BCR_TOP               ";
                strSql += cDefApp.CRLF + "                       , LUGG_NO )             ";
                strSql += cDefApp.CRLF + "                VALUES  (:WH_TYP               ";
                strSql += cDefApp.CRLF + "                       , :EQP_TYP              ";
                strSql += cDefApp.CRLF + "                       , :EQP_NO               ";
                strSql += cDefApp.CRLF + "                       , " + DbLang.SYSDATE + "";
                strSql += cDefApp.CRLF + "                       , :EQP_ERR_CD           ";
                strSql += cDefApp.CRLF + "                       , null                  ";
                strSql += cDefApp.CRLF + "                       , null                  ";
                strSql += cDefApp.CRLF + "                       , :LUGG_NO );           ";



                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = pWH_TYP;
                m_msQPlc._pBdb.mComMain.Parameters.Add("EQP_TYP", DbLang.VARCHAR, 255).Value = pEQP_TYP;
                m_msQPlc._pBdb.mComMain.Parameters.Add("EQP_NO", DbLang.VARCHAR, 255).Value = pEQP_NO;
                m_msQPlc._pBdb.mComMain.Parameters.Add("EQP_ERR_CD", DbLang.VARCHAR, 255).Value = pEQP_ERR_CD;
                m_msQPlc._pBdb.mComMain.Parameters.Add("LUGG_NO", DbLang.VARCHAR, 255).Value = pLUGG_NO;
                nSelCnt = m_msQPlc._pBdb.ExcuteNonQry(strSql);

                if (nSelCnt < 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    MakeMsg_Error("[UpdEQMT_ERR_LOG]:: Error:PLC설비 에러 로깅 실패 ", m_nthNo);
                    return false;
                }

                m_msQPlc._pBdb.Commit();
                return true;
            }
            catch (Exception ex)
            {
                m_msQPlc._pBdb.Rollback();
                MakeMsg_Error("[UpdEQMT_ERR_LOG]:: Error:PLC설비 에러 로깅 Exception 에러 실패 ", m_nthNo);
                return false;
            }
        }
        #endregion

        #region [UpdateCvFlowDef] :: CV_FLOW_DEF 업데이트. FLOW BIT SET (나중에)
        public bool UpdateCvFlowDef(string strTRACK_NO)
        {
            try
            {

                strSql = "";
                strSql += CRLF + " SELECT CFD.*, COALESCE(CD2.TRACK_NO, 'PASS') AS FRONT_GET_TRACK_NO, COALESCE(CD2.SENSOR0_DATA_RD, 'PASS') AS FRONT_SENSOR_VALUE  ";
                strSql += CRLF + "   FROM CV_FLOW_DEF CFD INNER JOIN CV_DATA CD                                                                                     ";
                strSql += CRLF + "                                ON CFD.WH_TYP = CD.WH_TYP                                                                         ";
                strSql += CRLF + "                               AND CFD.PLC_NO = CD.PLC_NO                                                                         ";
                strSql += CRLF + "                               AND CFD.WH_TYP = :WH_TYP                                                                           ";
                strSql += CRLF + "                               AND CFD.PLC_NO = :PLC_NO                                                                           ";
                strSql += CRLF + "                               AND CFD.TRACK_NO = CD.TRACK_NO                                                                     ";
                strSql += CRLF + "                               AND CFD.TRACK_NO <> CD.DEST_POS_RD                                                                 ";
                strSql += CRLF + "                               AND CFD.FLOW_YN = 'Y'                                                                              ";
                strSql += CRLF + "                               AND CD.AUTO_MODE_RD = '1'                                                                          ";
                strSql += CRLF + "                               AND CD.LUGG_NO_RD IS NOT NULL                                                                      ";
                strSql += CRLF + "                               AND CD.LUGG_NO_RD <> '0'                                                                           ";
                strSql += CRLF + "                               AND CD.SENSOR0_DATA_RD = '1'                                                                       ";
                strSql += CRLF + "                               AND CD.ERROR_CODE = '0'                                                                            ";
                strSql += CRLF + "                               AND CD.OD_RQ_YN = 'N'                                                                              ";
                strSql += CRLF + "                               AND CD.CMD_RQ_YN = 'N'                                                                             ";
                strSql += CRLF + "                               AND CD.READ_UPD_DT >= CD.WRITE_UPD_DT                                                              ";
                //strSql += CRLF + "                               AND ((now() - CD.READ_UPD_DT) * 24 * 60 * 60) > SET_TIME                                           ";
                //strSql += CRLF + "                               AND ((now() - CFD.OD_UPD_DT) * 24 * 60 * 60) > SET_TIME                                            ";
                strSql += CRLF + "                   LEFT OUTER JOIN CV_DATA CD2                                                                                    ";
                strSql += CRLF + "                                ON CD2.WH_TYP = CFD.WH_TYP                                                                        ";
                strSql += CRLF + "                               AND CD2.PLC_NO = CFD.PLC_NO                                                                        ";
                strSql += CRLF + "                               AND ( (CD2.TRACK_NO = CFD.FRONT_TRACK_NO                                                           ";
                strSql += CRLF + "                               AND CD2.AUTO_MODE_RD = '1'                                                                         ";
                strSql += CRLF + "                               AND CD2.ERROR_CODE = '0'                                                                           ";
                strSql += CRLF + "                               AND CD2.SENSOR0_DATA_RD = '0'                                                                      ";
                strSql += CRLF + "                                  ) OR (CFD.FRONT_TRACK_NO = '0' OR CFD.FRONT_TRACK_NO IS NULL) )                                 ";

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;

                nSelCnt = m_msQPlc._pBdb.ExcuteQry(strSql);

                if (nSelCnt < 0)
                {
                    MakeMsg_Error("[UpdateCvFlowDef] FLOW BIT 값 조회중 ERROR., PLC_NO [" + m_strPlc_No + "] MSG [" + m_msQPlc._pBdb.ErrMsg + "]", m_nthNo);
                    return false;
                }

                if (nSelCnt == 0)
                {
                    MakeMsg_Error("[UpdateCvFlowDef] FLOW BIT 값 조회중 DATA가 없습니다., PLC_NO [" + m_strPlc_No + "]", m_nthNo);
                    return false;

                }

                for (int nRows = 0; nRows < nSelCnt; nRows++)
                {
                    string strFRONT_GET_TRACK_NO = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["FRONT_GET_TRACK_NO"].ToString();
                    string strFRONT_SENSOR_VALUE = "" + m_msQPlc._pBdb.mDtMain.Rows[nRows]["FRONT_SENSOR_VALUE"].ToString();

                    if (strFRONT_GET_TRACK_NO == "PASS")
                    {
                        continue;
                    }
                }

                return true;
            }
            catch (Exception ex)
            {
                MakeMsg_Error("[UpdateCvFlowDef] FLOW BIT 값 조회중 ERROR., PLC_NO [" + m_strPlc_No + "] MSG [" + ex.ToString() + "]", m_nthNo);
                return false;
            }
        }
        #endregion

        #region[chkSIM_MODE] :: 시뮬레이션 모드 확인
        private bool chkSIM_MODE(int Idx)
        {
            try
            {
                string strSql = "";
                string strTitle = "[sim_mode]";

                byte[] byTxBuff = new byte[1000];

                int nUpdCount = 0;

                strSql = "";
#if SQL
                strSql += cDefApp.CRLF + "SELECT TOP 1 SIM_MODE               "; // [LGLS] SQL Server: LIMIT → TOP
#else
                strSql += cDefApp.CRLF + "SELECT SIM_MODE                     ";
#endif
                strSql += cDefApp.CRLF + "  FROM HOST_IF_LOG                  ";
                strSql += cDefApp.CRLF + " WHERE WH_TYP = :WH_TYP             ";
                strSql += cDefApp.CRLF + "ORDER BY INS_DT DESC                ";
#if !SQL
                strSql += cDefApp.CRLF + "LIMIT 1                             ";
#endif


                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;

                nUpdCount = m_msQPlc._pBdb.ExcuteQry(strSql);

                if (nUpdCount == 0)
                {
                    MakeMsg_Error("HOST_IF_LOG에 데이터가 없어서 SIM_MODE는 건너뜁니다.", m_nthNo);
                    return true;
                }

                if (nUpdCount < 0)
                {
                    MakeMsg_Error(" SIM_MODE 읽는 중 에러", m_nthNo);
                    return false;
                }

                int nCvNo = 0;
                int nSIM_MODE = 0;
                string SIM_MODE = "" + m_msQPlc._pBdb.mDtMain.Rows[0]["SIM_MODE"].ToString();

                if (!CvDic.ContainsKey(nCvNo))
                {
                    CvDic.Add(nCvNo, new CVData()); //Key를 추가한다.
                }

                if (SIM_MODE == "0") //sim모드 아닐떄
                {
                    if (CvDic[nCvNo].SIM_MODE != SIM_MODE)
                    {
                        CvDic[nCvNo].SIM_MODE = SIM_MODE;
                        //D1에 0넣기
                        nSIM_MODE = 0;
                        m_blSimModeWrite = true;
                    }
                    else
                    {
                        m_blSimModeWrite = false;
                        return true;
                    }
                }
                else
                {
                    if (CvDic[nCvNo].SIM_MODE != SIM_MODE)
                    {
                        CvDic[nCvNo].SIM_MODE = SIM_MODE;
                        //D1에 1넣기
                        nSIM_MODE = 1;
                        m_blSimModeWrite = true;
                    }
                    else
                    {
                        m_blSimModeWrite = false;
                        return true;
                    }
                }

                if (m_blSimModeWrite == true)
                {
                    int nADDR_NO = 0;

                    Array.Clear(byTxBuff, 0, byTxBuff.Length);
                    byTxBuff[0] = (byte)(nSIM_MODE);
                    byTxBuff[1] = (byte)(0);

                    int nWriteLen = 1;

                    if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                       (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                       nADDR_NO + 1,
                                                       nWriteLen,
                                                       byTxBuff) == false)
                    {
                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }
                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " D1 = [" + nSIM_MODE + "] 쓰기 실패";
                        if (!InsertWcsLogPgr("000", m_strLogMsg))
                        {
                            return false;
                        }
                        return false;
                    }

                    if (this.m_msQPlc.IsHex)
                    {
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                    }

                    if (this.m_msQPlc.IsAscii)
                    {
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                    }

                    m_strLogMsg = strTitle + " D1 = [" + nSIM_MODE + "] 쓰기 성공";
                    if (!InsertWcsLogPgr("000", m_strLogMsg))
                    {
                        return false;
                    }
                }
            
                return true;
            }
            catch(Exception ex)
            {
                return false;
            }
        }
        #endregion

        #region[chkSTO_RET_ING] :: 출고 중 데이터 쓰기
        private bool chkRET_ING(int Idx)
        {
            try
            {
                string strSql = "";
                string strTitle = "[chkRET_ING]";

                byte[] byTxBuff = new byte[1000];

                int nSelCount = 0;

                strSql = "";
                strSql += cDefApp.CRLF + "SELECT COUNT(1) AS CNT                               ";
                strSql += cDefApp.CRLF + "  FROM CV_DATA CD                                    ";
                strSql += cDefApp.CRLF + " WHERE CD.WH_TYP = :WH_TYP                           ";
                strSql += cDefApp.CRLF + "   AND CD.PLC_NO = :PLC_NO                           ";
                strSql += cDefApp.CRLF + "   AND CD.MC_NO BETWEEN :FROM_TRACK AND :TO_TRACK    ";
                strSql += cDefApp.CRLF + "   AND CD.DEST_POS_RD = '101'                        "; // 101번으로 출고되는 경우만 찾기

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;
                m_msQPlc._pBdb.mComMain.Parameters.Add("FROM_TRACK", DbLang.VARCHAR, 255).Value = m_nFrTrackNo.ToString("000");
                m_msQPlc._pBdb.mComMain.Parameters.Add("TO_TRACK", DbLang.VARCHAR, 255).Value = m_nToTrackNo.ToString("000");

                nSelCount = m_msQPlc._pBdb.ExcuteQry(strSql);

                if (nSelCount < 0)
                {
                    MakeMsg_Error("101번 출고 중 조회 중 에러", m_nthNo);
                    return false;
                }

                //101번으로 출고중인 작업이 없음.
                if (nSelCount == 0)
                {
                    return true;
                }

                int nCvNo = 0;
                string strRET_CNT = "" + m_msQPlc._pBdb.mDtMain.Rows[0]["CNT"].ToString();
                int nRET_CNT = Convert.ToInt32(0 + strRET_CNT);

                string strRET_STATUS = "";
                int nRET_STATUS = 0;

                if (!CvDic.ContainsKey(nCvNo))
                {
                    CvDic.Add(nCvNo, new CVData()); //Key를 추가한다.
                }

                //101번으로 출고 중인것이 있을 때
                if (nRET_CNT > 0)
                {
                    strRET_STATUS = "1";
                    if (CvDic[nCvNo].RET_STATUS != strRET_STATUS)
                    {
                        CvDic[nCvNo].RET_STATUS = strRET_STATUS;
                        //D8에 2넣기(1번 bit 1)
                        nRET_STATUS = 2;
                        m_blRetStatusWrite = true;
                    }
                    else
                    {
                        m_blRetStatusWrite = false;
                        return true;
                    }
                }
                else // 없을때
                {
                    strRET_STATUS = "0";
                    if (CvDic[nCvNo].RET_STATUS != strRET_STATUS)
                    {
                        CvDic[nCvNo].RET_STATUS = strRET_STATUS;
                        //D8에 0넣기 (1번 bit 0)
                        nRET_STATUS = 0;
                        m_blRetStatusWrite = true;
                    }
                    else
                    {
                        m_blRetStatusWrite = false;
                        return true;
                    }
                }

                if (m_blRetStatusWrite == true)
                {
                    int nADDR_NO = 0;

                    Array.Clear(byTxBuff, 0, byTxBuff.Length);
                    byTxBuff[0] = (byte)(nRET_STATUS);
                    byTxBuff[1] = (byte)(0);

                    int nWriteLen = 1;

                    if (m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                                       (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                                       nADDR_NO + 8,
                                                       nWriteLen,
                                                       byTxBuff) == false)
                    {
                        if (this.m_msQPlc.IsHex)
                        {
                            MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }
                        if (this.m_msQPlc.IsAscii)
                        {
                            MakeMsg_Error(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                            MakeMsg_Error(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                        }

                        m_strLogMsg = strTitle + " D8 = [" + nRET_STATUS + "] 쓰기 실패";
                        if (!InsertWcsLogPgr("000", m_strLogMsg))
                        {
                            return false;
                        }
                        return false;
                    }

                    if (this.m_msQPlc.IsHex)
                    {
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndHexString + "]", m_nthNo);
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                    }

                    if (this.m_msQPlc.IsAscii)
                    {
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 SEND [" + m_msQPlc.SndAsciiString + "]", m_nthNo);
                        MakeMsg_Imp(strTitle + "트랙정보변경요청 RECEIVE [" + m_msQPlc.RcvAsciiString + "]", m_nthNo);
                    }

                    //쓰기 한 값 db에 넣기.
                    if (!UpdateRetStauts(strRET_STATUS))
                    {
                        return false;
                    }

                    m_strLogMsg = strTitle + " D8 = [" + nRET_STATUS + "] 쓰기 성공";
                    if (!InsertWcsLogPgr("000", m_strLogMsg))
                    {
                        return false;
                    }
                }

                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }
        #endregion

        #region [UpdateRetStauts] :: CV_DATA의 RET_STATUS_OD 업데이트
        public bool UpdateRetStauts(string strRET_STATUS)
        {
            //해당 데이터는 101번에만 기입
            string strTRACK_NO = "101";

            try
            {
                m_msQPlc._pBdb.BeginTrans();

                strSql = "";
                strSql += CRLF + "UPDATE CV_DATA						               ";
                strSql += CRLF + "   SET RET_STATUS      = :RET_STATUS                 ";
                strSql += CRLF + "WHERE  WH_TYP          = :WH_TYP		               ";
                // [LGLS] 트랙 101은 복귀라인(PLC_NO=01)이라 이 PLC(11) 소속이 아님 → PLC_NO 조건 제거(하드코딩 101 대상)
                strSql += CRLF + "AND    MC_NO           = :MC_NO    	               ";

                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("RET_STATUS", DbLang.VARCHAR, 255).Value = strRET_STATUS;
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("MC_NO", DbLang.VARCHAR, 255).Value = strTRACK_NO;

                nSelCnt = m_msQPlc._pBdb.ExcuteNonQry(strSql);

                if (nSelCnt < 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    MakeMsg_Error("[UpdateRetStauts] 쓰기지시 후 상태값 변경중 ERROR., TRACK_NO [" + strTRACK_NO + "] MSG [" + m_msQPlc._pBdb.ErrMsg + "]", m_nthNo);
                    return false;
                }

                if (nSelCnt == 0)
                {
                    m_msQPlc._pBdb.Rollback();
                    MakeMsg_Error("[UpdateRetStauts]쓰기지시 후 상태값 변경중 DATA가 없습니다., TRACK_NO [" + strTRACK_NO + "]", m_nthNo);
                    return false;

                }

                m_msQPlc._pBdb.Commit();
                return true;
            }
            catch (Exception ex)
            {
                m_msQPlc._pBdb.Rollback();
                MakeMsg_Error("[UpdateRetStauts] 쓰기지시 후 상태값 변경중 ERROR., TRACK_NO [" + strTRACK_NO + "] MSG [" + ex.ToString() + "]", m_nthNo);
                return false;
            }
        }
        #endregion

        // ─────────────────────────────────────────────────────────────────────
        //  헬퍼: M 비트 읽기/쓰기 (워드 단위 PLC 통신 기반)
        // ─────────────────────────────────────────────────────────────────────

        #region [M 비트 헬퍼]
        /// <summary>
        /// 2 워드(32비트) 버퍼에서 특정 비트 추출.
        /// bitIndex : 버퍼 시작(buf[0] 비트0)으로부터의 절대 비트 인덱스
        /// </summary>
        private bool GetMBitFromBuf(byte[] buf, int bitIndex)
        {
            int byteIdx = bitIndex / 8;
            int bitPos  = bitIndex % 8;
            if (byteIdx >= buf.Length) return false;
            return (buf[byteIdx] & (1 << bitPos)) != 0;
        }

        /// <summary>
        /// M 영역 특정 비트 한 개를 ON/OFF 쓰기 (Read-Modify-Write).
        /// mBitAddr : M 비트 절대 주소 (예: M0901 → 901)
        /// </summary>
        private bool WriteMBit(int mBitAddr, bool value)
        {
            int wordAddr = mBitAddr / 16;
            int bitPos   = mBitAddr % 16;

            // 현재 워드 읽기
            byte[] rxBuf = new byte[100];
            Array.Clear(rxBuf, 0, rxBuf.Length);
            if (!m_msQPlc.READ((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                               (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_M,
                               wordAddr, 1, ref rxBuf))
                return false;

            // 비트 변경
            int word = rxBuf[0] | (rxBuf[1] << 8);
            if (value)
                word |=  (1 << bitPos);
            else
                word &= ~(1 << bitPos);

            // 변경된 워드 쓰기
            byte[] txBuf = new byte[2];
            txBuf[0] = (byte)(word & 0xFF);
            txBuf[1] = (byte)((word >> 8) & 0xFF);
            return m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                  (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_M,
                                  wordAddr, 1, txBuf);
        }

        /// <summary>
        /// R 영역 트래킹 주소 계산.
        /// PPT: Tracking Start Address = (CV 기계번호 - 1) * 10 word
        ///      슬롯 s (0-based) → base + s*2 (JOB NO = 2 word)
        /// CV 기계번호는 m_strPlc_No 숫자 파싱 (예: "01" → 1, "11" → 11)
        /// </summary>
        private int GetRTrackingAddr(int trackNo)
        {
            int cvMachineNo = 0;
            string numStr = System.Text.RegularExpressions.Regex.Match(m_strPlc_No, @"\d+").Value;
            int.TryParse(numStr, out cvMachineNo);
            if (cvMachineNo < 1) cvMachineNo = 1;

            // [V1.5] 운영 DB(TB_OBSERVABLE)의 실주소 규칙:
            //   문서 표기 "R0000/0010/…/0100/0110"은 10진 (CV번호-1)*10 을 그대로 적은 것이지만
            //   ECS 드라이버가 이를 16진으로 파싱하므로 실효 워드주소 = hex((CV번호-1)*10 표기).
            //   예) CV#7 → "0060" → 0x60(96), CV#11 → "0100" → 0x100(256)
            // [LGLS 2026-08-19] 위 16진 해석은 '구 ECS 호환' 모드일 때만. ini [PLC] R_ADDR_MODE 로 전환한다.
            //   HEX(기본) = 종전과 동일 / DEC = 문서 표기를 10진 워드주소로 사용(CV#11 → 100)
            //   시작주소(문서표기 (N-1)*10)는 주소맵 XML <Block name="Tracking"> 을 우선 사용한다.
            int rBase = cPlcAddrMap.BlockBase("CV", cvMachineNo, "Tracking");
            if (rBase < 0) rBase = cDefApp.GsRTrackWord((cvMachineNo - 1) * 10);
            int slot  = trackNo - m_nFrTrackNo;           // 이 CV 내 슬롯 인덱스 (0-based)
            return rBase + slot * 2;                      // JOB NO = 2 word
        }

        /// <summary>
        /// JOB NO 트래킹 인코딩 (ECS FenetDriver.WriteWordA 와 동일한 ASCII 팩).
        /// [V1.5] 4자리 JOB 을 ASCII 4문자로, 워드당 2문자(하위바이트=앞문자)로 기록한다.
        ///   "1234" → word0 = '1'|'2'&lt;&lt;8 (0x3231), word1 = '3'|'4'&lt;&lt;8 (0x3433)
        ///   PPT의 "1234 → 2143" 표기는 각 워드를 16진 상위→하위로 읽을 때 보이는 모습일 뿐,
        ///   십진 BCD 재배열이 아님 (ECS 는 이 값을 문자열로 읽어 Palletid 와 대조).
        /// 2 word(4 byte) 버퍼 offset 위치에 little-endian 기록.
        /// </summary>
        private void EncodeJobNoR(int jobNo, byte[] buf, int offset)
        {
            string s = (jobNo % 10000).ToString("0000");   // 4자리 ASCII
            buf[offset + 0] = (byte)s[0];
            buf[offset + 1] = (byte)s[1];
            buf[offset + 2] = (byte)s[2];
            buf[offset + 3] = (byte)s[3];
        }
        #endregion

        // ─────────────────────────────────────────────────────────────────────
        //  PPT 시나리오 구현 메서드
        // ─────────────────────────────────────────────────────────────────────

        #region [CvEventCheck] :: M 비트 Load/Unload Complete 이벤트 감지 및 ACK 처리
        /// <summary>
        /// PPT Slide 5 / 8~15 시나리오:
        ///   PLC가 Load/Unload Complete M 비트 ON
        ///   → WCS ACK M 비트 ON
        ///   → PLC가 이벤트 비트 OFF
        ///   → WCS ACK 비트 OFF
        ///
        /// [V1.4] 비트 맵 (CV 기계번호 N 기준, 블록 = 1워드 16비트 정렬):
        ///   이벤트 base = %MX256 + (N-1)*32
        ///   +1 : Unload Complete #1  (RGV측 화물 수령)
        ///   +2 : Load Complete #1    (RGV측 화물 적재)
        ///   +3 : Unload Complete #2  (작업자 반출)
        ///   +4 : Load Complete #2    (입고쪽 화물 적재)
        ///   +5 : WI  /  +6 : WO (작업지시 보고)
        ///   +7 : InReady #2  /  +8 : Op Mode
        ///   +16: Pallet Exist 블록 (#1, #2)
        ///
        ///   ACK base = %MX1280 + (N-1)*16
        ///   +1 : Unload Complete Ack #1
        ///   +2 : Load Complete Ack #1
        ///   +3 : Unload Complete Ack #2
        ///   +4 : Load Complete Ack #2
        ///   +5 : Unload Request #2 (C/V #14)
        /// </summary>
        private bool CvEventCheck(int Idx)
        {
            string strTitle = "[CvEventCheck]";
            try
            {
                string numStr = System.Text.RegularExpressions.Regex.Match(m_strPlc_No, @"\d+").Value;
                int cvMachineNo = 0;
                int.TryParse(numStr, out cvMachineNo);
                if (cvMachineNo < 1) return true; // PLC_NO 파싱 불가 시 스킵

                int mBase, ackBase;
                if (cDefApp.GM_ADDR_V09)
                {
                    // [V0.9] 10단위 블록 (XGK 래더 직접 표기)
                    //   이벤트 블록 : M0160 + (N-1)*20  (PalletExist 블록은 +10)
                    //   ACK   블록 : M0800 + (N-1)*10
                    mBase   = 160 + (cvMachineNo - 1) * 20;
                    ackBase = 800 + (cvMachineNo - 1) * 10;
                }
                else
                {
                    // [V1.1] 블록이 워드(16비트) 정렬로 재배치됨 (ezMCS 매핑)
                    //   이벤트 블록 : %MX256 + (N-1)*32  (PalletExist 블록은 +16)
                    //   ACK   블록 : %MX1280 + (N-1)*16
                    // [LGLS 2026-08-19] 주소맵 XML(7_DeviceMap\PlcAddressMap.xml)이 있으면 그 값을 쓰고,
                    //   없으면 아래 내장 계산식으로 폴백한다(파일 문제로 통신이 멎지 않게).
                    mBase   = cPlcAddrMap.BlockBase("CV", cvMachineNo, "Event");
                    ackBase = cPlcAddrMap.BlockBase("CV", cvMachineNo, "Ack");
                    if (mBase   < 0) mBase   = 256  + (cvMachineNo - 1) * 32;
                    if (ackBase < 0) ackBase = 1280 + (cvMachineNo - 1) * 16;
                }

                // 이벤트 M 영역 읽기: 2 워드 (base 워드 정렬)
                int mWordAddr  = mBase / 16;
                int mBitOffset = mBase % 16; // 버퍼 내 시작 비트 위치

                byte[] byRxBuff = new byte[100];
                Array.Clear(byRxBuff, 0, byRxBuff.Length);
                if (!m_msQPlc.READ((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                   (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_M,
                                   mWordAddr, 2, ref byRxBuff))
                {
                    MakeMsg_Error(strTitle + " M비트 읽기 실패", m_nthNo);
                    return false;
                }

                // 이벤트 비트 추출
                bool unloadComp1 = GetMBitFromBuf(byRxBuff, mBitOffset + 1);
                bool loadComp1   = GetMBitFromBuf(byRxBuff, mBitOffset + 2);
                bool unloadComp2 = GetMBitFromBuf(byRxBuff, mBitOffset + 3);
                bool loadComp2   = GetMBitFromBuf(byRxBuff, mBitOffset + 4);
                bool workOrder   = GetMBitFromBuf(byRxBuff, mBitOffset + 6);

                // 기준 CVData 키 (이벤트는 CV 단위 → m_nFrTrackNo 키 사용)
                int dicKey = m_nFrTrackNo;
                if (!CvDic.ContainsKey(dicKey))
                    CvDic.Add(dicKey, new CVData());

                CVData cv = CvDic[dicKey];

                // ─── Unload Complete #1 (RGV측) : ACK = 블록 +1 ─────────────
                if (unloadComp1 && !cv.UnloadComp1Acked)
                {
                    if (!WriteMBit(ackBase + 1, true)) return false;
                    cv.UnloadComp1Acked = true;
                    InsertWcsLogPgr(m_nFrTrackNo.ToString("000"),
                        strTitle + " Unload Complete #1 ACK ON → %MX" + (ackBase + 1));
                    MakeMsg_Imp(strTitle + " Unload Complete #1 감지, ACK %MX" + (ackBase + 1) + " ON", m_nthNo);
                }
                else if (!unloadComp1 && cv.UnloadComp1Acked)
                {
                    if (!WriteMBit(ackBase + 1, false)) return false;
                    cv.UnloadComp1Acked = false;
                    MakeMsg(strTitle + " Unload Complete #1 해제, ACK OFF", m_nthNo);
                }

                // ─── Load Complete #1 (RGV측) : ACK = 블록 +2 ───────────────
                if (loadComp1 && !cv.LoadComp1Acked)
                {
                    if (!WriteMBit(ackBase + 2, true)) return false;
                    cv.LoadComp1Acked = true;
                    InsertWcsLogPgr(m_nFrTrackNo.ToString("000"),
                        strTitle + " Load Complete #1 ACK ON → %MX" + (ackBase + 2));
                    MakeMsg_Imp(strTitle + " Load Complete #1 감지, ACK %MX" + (ackBase + 2) + " ON", m_nthNo);
                }
                else if (!loadComp1 && cv.LoadComp1Acked)
                {
                    if (!WriteMBit(ackBase + 2, false)) return false;
                    cv.LoadComp1Acked = false;
                    MakeMsg(strTitle + " Load Complete #1 해제, ACK OFF", m_nthNo);
                }

                // ─── Unload Complete #2 (작업자 반출) : ACK = 블록 +3 ───────
                if (unloadComp2 && !cv.UnloadComp2Acked)
                {
                    if (!WriteMBit(ackBase + 3, true)) return false;
                    cv.UnloadComp2Acked = true;
                    InsertWcsLogPgr(m_nFrTrackNo.ToString("000"),
                        strTitle + " Unload Complete #2 ACK ON → %MX" + (ackBase + 3));
                    MakeMsg_Imp(strTitle + " Unload Complete #2 감지, ACK %MX" + (ackBase + 3) + " ON", m_nthNo);
                }
                else if (!unloadComp2 && cv.UnloadComp2Acked)
                {
                    if (!WriteMBit(ackBase + 3, false)) return false;
                    cv.UnloadComp2Acked = false;
                    MakeMsg(strTitle + " Unload Complete #2 해제, ACK OFF", m_nthNo);
                }

                // ─── Load Complete #2 (입고쪽) ──────────────────────────────
                if (loadComp2 && !cv.LoadComp2Acked)
                {
                    if (!WriteMBit(ackBase + 4, true)) return false;
                    cv.LoadComp2Acked = true;
                    InsertWcsLogPgr(m_nFrTrackNo.ToString("000"),
                        strTitle + " Load Complete #2 ACK ON → %MX" + (ackBase + 4));
                    MakeMsg_Imp(strTitle + " Load Complete #2 감지, ACK %MX" + (ackBase + 4) + " ON", m_nthNo);
                }
                else if (!loadComp2 && cv.LoadComp2Acked)
                {
                    if (!WriteMBit(ackBase + 4, false)) return false;
                    cv.LoadComp2Acked = false;
                    MakeMsg(strTitle + " Load Complete #2 해제, ACK OFF", m_nthNo);
                }

                // ─── Unload Request #2 (C/V #14 피킹존) : REQ = ACK 블록 +5 ──
                //   PPT V1.1 슬라이드 13: 피킹 완료 후 설비 W.O(mBase+6) 보고 →
                //   WCS 가 Unload Request #2(ackBase+5) 발행 → 설비 Ack(mBase+9) →
                //   설비 Unload Complete #2(mBase+3) → WCS Ack(ackBase+3) 순.
                //   피킹존 반출(#2): 출고 C/V #14 + 입고 C/V #15(2026-07-27 추가) 공통 지원.
                if (cvMachineNo == 14 || cvMachineNo == 15)
                {
                    bool unloadReq2Ack = GetMBitFromBuf(byRxBuff, mBitOffset + 9); // 설비 Unload Request #2 Ack
                    if (workOrder && !cv.UnloadReq2Sent)
                    {
                        if (!WriteMBit(ackBase + 5, true)) return false;
                        cv.UnloadReq2Sent = true;
                        InsertWcsLogPgr(m_nFrTrackNo.ToString("000"),
                            strTitle + " Unload Request #2 발행 → %MX" + (ackBase + 5));
                        MakeMsg_Imp(strTitle + " W.O 감지, Unload Request #2 %MX" + (ackBase + 5) + " ON (Ack대기 %MX" + (mBase + 9) + ")", m_nthNo);
                    }
                    else if (!workOrder && cv.UnloadReq2Sent)
                    {
                        // 설비 W.O 해제(피킹 반출 완료) → REQ 해제
                        if (!WriteMBit(ackBase + 5, false)) return false;
                        cv.UnloadReq2Sent = false;
                        MakeMsg(strTitle + " W.O 해제, Unload Request #2 %MX" + (ackBase + 5) + " OFF (Ack=" + (unloadReq2Ack ? "ON" : "off") + ")", m_nthNo);
                    }
                }

                // ─── W.O 비트 DB 반영 (HOST_SEND_YN 트리거) ─────────────────
                if (workOrder)
                {
                    MakeMsg_Imp(strTitle + " W.O 비트 ON %MX" + (mBase + 6) + " - 상위 보고 대기", m_nthNo);
                }

                return true;
            }
            catch (Exception ex)
            {
                MakeMsg_Error(strTitle + " Exception: " + ex.Message, m_nthNo);
                return false;
            }
        }
        #endregion

        #region [CvTrackingWrite] :: R 영역 트래킹 JOB 쓰기
        /// <summary>
        /// PPT Slide 8~15 시나리오 / Slide 26 Tracking Area:
        ///   DB CV_DATA에서 TRACKING_WRITE_YN = 'Y' 인 트랙 조회
        ///   → R 영역에 JOB NO BCD 인코딩 쓰기
        ///   → TRACKING_WRITE_YN = 'N' 업데이트
        ///
        ///   R 주소: (CV 기계번호-1)*10 + 슬롯*2
        ///   JOB NO: 2 word, BCD 인코딩 (1234 → 2143)
        /// </summary>
        private bool CvTrackingWrite(int Idx)
        {
            string strTitle = "[CvTrackingWrite]";
            try
            {
                // TRACKING_WRITE_YN = 'Y' 인 트랙 조회
                string sql = "";
#if SQL
                sql += cDefApp.CRLF + "SELECT TOP 5 CD.MC_NO, CD.LUGG_NO_OD, CD.DEST_POS_OD      "; // [LGLS] SQL Server: LIMIT → TOP
#else
                sql += cDefApp.CRLF + "SELECT CD.MC_NO, CD.LUGG_NO_OD, CD.DEST_POS_OD            ";
#endif
                sql += cDefApp.CRLF + "  FROM CV_DATA CD                                          ";
                sql += cDefApp.CRLF + " WHERE CD.WH_TYP = :WH_TYP                                ";
                sql += cDefApp.CRLF + "   AND CD.PLC_NO = :PLC_NO                                 ";
                sql += cDefApp.CRLF + "   AND CD.MC_NO BETWEEN :FROM_TRACK AND :TO_TRACK          ";
                sql += cDefApp.CRLF + "   AND CD.TRACKING_WRITE_YN = 'Y'                          ";
                sql += cDefApp.CRLF + " ORDER BY CD.MC_NO                                         ";
#if !SQL
                sql += cDefApp.CRLF + " LIMIT 5;                                                  ";
#endif

                m_msQPlc._pBdb.mComMain.CommandType = System.Data.CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP",      DbLang.VARCHAR, 255).Value = m_strWh_typ;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO",      DbLang.VARCHAR, 255).Value = m_strPlc_No;
                m_msQPlc._pBdb.mComMain.Parameters.Add("FROM_TRACK",  DbLang.VARCHAR, 255).Value = m_nFrTrackNo.ToString("000");
                m_msQPlc._pBdb.mComMain.Parameters.Add("TO_TRACK",    DbLang.VARCHAR, 255).Value = m_nToTrackNo.ToString("000");

                int cnt = m_msQPlc._pBdb.ExcuteQry(sql);
                if (cnt < 0)
                {
                    MakeMsg_Error(strTitle + " TRACKING_WRITE_YN 조회 오류", m_nthNo);
                    return false;
                }
                if (cnt == 0) return true;

                for (int i = 0; i < cnt; i++)
                {
                    string mcNo   = m_msQPlc._pBdb.mDtMain.Rows[i]["MC_NO"].ToString();
                    int trackNo   = Convert.ToInt32("0" + mcNo);
                    int luggNo    = Convert.ToInt32("0" + m_msQPlc._pBdb.mDtMain.Rows[i]["LUGG_NO_OD"].ToString());
                    int destPos   = Convert.ToInt32("0" + m_msQPlc._pBdb.mDtMain.Rows[i]["DEST_POS_OD"].ToString());

                    int rAddr = GetRTrackingAddr(trackNo);

                    // JOB NO 2 word BCD 인코딩 후 R 영역 쓰기
                    byte[] txBuf = new byte[4];
                    EncodeJobNoR(luggNo, txBuf, 0);

                    if (!m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                        (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_R,
                                        rAddr, 2, txBuf))
                    {
                        if (m_msQPlc.IsHex)
                        {
                            MakeMsg_Error(strTitle + " R 쓰기 TX [" + m_msQPlc.SndHexString + "]", m_nthNo);
                            MakeMsg_Error(strTitle + " R 쓰기 RX [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                        }
                        InsertWcsLogPgr(mcNo, strTitle + " R" + rAddr + " JOB[" + luggNo + "] 쓰기 실패");
                        return false;
                    }

                    if (m_msQPlc.IsHex)
                    {
                        MakeMsg_Imp(strTitle + " R 쓰기 TX [" + m_msQPlc.SndHexString + "]", m_nthNo);
                        MakeMsg_Imp(strTitle + " R 쓰기 RX [" + m_msQPlc.RcvHexString + "]", m_nthNo);
                    }

                    MakeMsg_Imp(strTitle + " TRACK[" + mcNo + "] R" + rAddr
                                + " JOB[" + luggNo + "] DEST[" + destPos + "] 쓰기 성공", m_nthNo);
                    InsertWcsLogPgr(mcNo, strTitle + " R" + rAddr + " JOB[" + luggNo + "] 쓰기 성공");

                    // DB TRACKING_WRITE_YN 초기화
                    m_msQPlc._pBdb.BeginTrans();
                    string updSql = "";
                    updSql += CRLF + "UPDATE CV_DATA                              ";
                    updSql += CRLF + "   SET TRACKING_WRITE_YN = 'N'             ";
                    updSql += CRLF + "      ,WRITE_UPD_DT = " + DbLang.SYSDATE + "      ";
                    updSql += CRLF + " WHERE WH_TYP = :WH_TYP                    ";
                    updSql += CRLF + "   AND PLC_NO  = :PLC_NO                   ";
                    updSql += CRLF + "   AND MC_NO   = :MC_NO                    ";

                    m_msQPlc._pBdb.mComMain.CommandType = System.Data.CommandType.Text;
                    m_msQPlc._pBdb.mComMain.Parameters.Clear();
                    m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                    m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;
                    m_msQPlc._pBdb.mComMain.Parameters.Add("MC_NO",  DbLang.VARCHAR, 255).Value = mcNo;

                    int upd = m_msQPlc._pBdb.ExcuteNonQry(updSql);
                    if (upd < 0)
                    {
                        m_msQPlc._pBdb.Rollback();
                        MakeMsg_Error(strTitle + " TRACKING_WRITE_YN 업데이트 오류", m_nthNo);
                        return false;
                    }
                    m_msQPlc._pBdb.Commit();
                }

                return true;
            }
            catch (Exception ex)
            {
                m_msQPlc._pBdb.Rollback();
                MakeMsg_Error(strTitle + " Exception: " + ex.Message, m_nthNo);
                return false;
            }
        }
        #endregion

        #region [CvAlarmCheck] :: 알람 보고 M 비트(M0492/M0493) 처리
        /// <summary>
        /// PPT Slide 7 시나리오 (Alarm Report):
        ///   M0492 ON → 알람 세트 보고 → ECS ACK M0963 ON
        ///   M0492 OFF → M0963 OFF
        ///   M0493 ON → 알람 리셋 보고 → ECS ACK M0964 ON
        ///   M0493 OFF → M0964 OFF
        /// </summary>
        private bool CvAlarmCheck(int Idx)
        {
            string strTitle = "[CvAlarmCheck]";
            // 알람 비트 주소 (버전별)
            //   V0.9 : M0492/M0493, ACK M0963/M0964 (XGK 래더 직접 표기)
            //   V1.1 : %MX786/%MX787, ACK %MX1539/%MX1540 (ezMCS 매핑)
            int M_ALARM_SET     = cDefApp.GM_ADDR_V09 ? 492 : 786;   // PLC → ECS: 알람 발생
            int M_ALARM_RST     = cDefApp.GM_ADDR_V09 ? 493 : 787;   // PLC → ECS: 알람 해제
            int M_ALARM_SET_ACK = cDefApp.GM_ADDR_V09 ? 963 : 1539;  // ECS → PLC: 알람 발생 ACK
            int M_ALARM_RST_ACK = cDefApp.GM_ADDR_V09 ? 964 : 1540;  // ECS → PLC: 알람 해제 ACK
            // [LGLS 2026-08-19] 주소맵 XML <Global name="Alarm"> 이 있으면 그 값 우선 (없으면 위 내장값)
            if (!cDefApp.GM_ADDR_V09)
            {
                int a;
                a = cPlcAddrMap.GlobalBit("AlarmSet");       if (a >= 0) M_ALARM_SET     = a;
                a = cPlcAddrMap.GlobalBit("AlarmReset");     if (a >= 0) M_ALARM_RST     = a;
                a = cPlcAddrMap.GlobalBit("AlarmSetAck");    if (a >= 0) M_ALARM_SET_ACK = a;
                a = cPlcAddrMap.GlobalBit("AlarmResetAck");  if (a >= 0) M_ALARM_RST_ACK = a;
            }

            try
            {
                // %MX786, %MX787 읽기 (같은 워드: 786/16=49, 787/16=49)
                byte[] byRxBuff = new byte[100];
                Array.Clear(byRxBuff, 0, byRxBuff.Length);
                if (!m_msQPlc.READ((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                   (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_M,
                                   M_ALARM_SET / 16, 1, ref byRxBuff))
                {
                    MakeMsg_Error(strTitle + " M알람 읽기 실패", m_nthNo);
                    return false;
                }

                int wordVal   = byRxBuff[0] | (byRxBuff[1] << 8);
                bool alarmSet = (wordVal & (1 << (M_ALARM_SET % 16))) != 0; // M0492
                bool alarmRst = (wordVal & (1 << (M_ALARM_RST % 16))) != 0; // M0493

                int dicKey = 0; // 알람은 CV 전체 공통 키 0 사용
                if (!CvDic.ContainsKey(dicKey))
                    CvDic.Add(dicKey, new CVData());
                CVData cvGlobal = CvDic[dicKey];

                // ─── 알람 세트 보고 ──────────────────────────────────────────
                if (alarmSet && !cvGlobal.AlarmSetAcked)
                {
                    if (!WriteMBit(M_ALARM_SET_ACK, true)) return false;
                    cvGlobal.AlarmSetAcked = true;
                    InsertWcsLogPgr("000", strTitle + " 알람 SET 보고 감지 → ACK M" + M_ALARM_SET_ACK + " ON");
                    MakeMsg_Imp(strTitle + " 알람 SET 감지 M" + M_ALARM_SET + ", ACK M" + M_ALARM_SET_ACK + " ON", m_nthNo);
                }
                else if (!alarmSet && cvGlobal.AlarmSetAcked)
                {
                    if (!WriteMBit(M_ALARM_SET_ACK, false)) return false;
                    cvGlobal.AlarmSetAcked = false;
                    MakeMsg(strTitle + " 알람 SET 해제, ACK M" + M_ALARM_SET_ACK + " OFF", m_nthNo);
                }

                // ─── 알람 리셋 보고 ──────────────────────────────────────────
                if (alarmRst && !cvGlobal.AlarmRstAcked)
                {
                    if (!WriteMBit(M_ALARM_RST_ACK, true)) return false;
                    cvGlobal.AlarmRstAcked = true;
                    InsertWcsLogPgr("000", strTitle + " 알람 RST 보고 감지 → ACK M" + M_ALARM_RST_ACK + " ON");
                    MakeMsg_Imp(strTitle + " 알람 RST 감지 M" + M_ALARM_RST + ", ACK M" + M_ALARM_RST_ACK + " ON", m_nthNo);
                }
                else if (!alarmRst && cvGlobal.AlarmRstAcked)
                {
                    if (!WriteMBit(M_ALARM_RST_ACK, false)) return false;
                    cvGlobal.AlarmRstAcked = false;
                    MakeMsg(strTitle + " 알람 RST 해제, ACK M" + M_ALARM_RST_ACK + " OFF", m_nthNo);
                }

                return true;
            }
            catch (Exception ex)
            {
                MakeMsg_Error(strTitle + " Exception: " + ex.Message, m_nthNo);
                return false;
            }
        }
        #endregion

        #region [CvStatusScenario] :: [V1.1/V0.9] 시나리오 맵 기반 CV 상태 READ → CV_DATA 반영
        /// <summary>
        /// 구 CvStatus()는 Melsec식 D 트랙테이블(트랙당 10워드)을 읽는 구버전 방식으로,
        /// V1.1/V0.9 시나리오 맵에는 해당 영역이 존재하지 않아 대체함.
        ///
        /// 시나리오 맵에서 읽는 CV 상태 (CV 기계번호 N):
        ///   1) M 이벤트 블록   : Op Mode(+8), InReady #2(+7)  - CvEventCheck 와 동일 블록
        ///   2) M PalletExist   : 블록 +16(V1.1) / +10(V0.9), 포지션당 1비트
        ///   3) D 방향 워드     : %MB960+(N-1)*2 (V1.1) / D0300+(N-1) (V0.9), 0=입고 1=출고
        ///   4) R 트래킹 영역   : 포지션당 2워드 ASCII JOB NO (EncodeJobNoR 의 역변환)
        ///
        /// CV_DATA 컬럼 매핑 (변화가 있을 때만 UPDATE, 자동작업 판단은 IO_SCH 담당):
        ///   Op Mode     -> AUTO_MODE_RD    (CV 전 트랙 공통)
        ///   방향 워드   -> STOCK_MODE      (CV 전 트랙 공통)
        ///   PalletExist -> SENSOR0_DATA_RD (포지션별)
        ///   R JOB NO    -> LUGG_NO_RD      (포지션별, PLC 트래킹 대사값)
        ///   InReady #2  -> STO_READY_RD    (포지션 #2 트랙만, 입고측 투입가능)
        /// </summary>
        private bool CvStatusScenario(int Idx)
        {
            string strTitle = "[CvStatusScenario]";
            // [LGLS 2026-07-21/31] 설비별 30폴링마다 그 설비의 트랙 캐시만 무효화 → 전 컬럼 재기록(자가 치유).
            //   (전 설비 동시 무효화는 한 사이클에 UPDATE 가 몰려 미러 주기를 3배 이상 늘린다)
            {
                int nSyncCnt;
                if (!m_dicCacheSyncCnt.TryGetValue(m_strPlc_No, out nSyncCnt))
                {
                    // [LGLS 2026-07-31] 최초 1회는 설비 순번에 따라 카운터를 흩뿌린다.
                    //   전 설비가 같은 사이클에 30회째를 맞으면 그 사이클에 전 트랙 재기록이 몰려
                    //   한 바퀴가 2.5초 → 11초로 튄다(80초마다 관측됨). 분산하면 사이클당 1설비씩만 재동기화된다.
                    int nIdx = 0;
                    for (int i = 0; i < m_slots.Count; i++)
                        if (m_slots[i].Plc == m_strPlc_No) { nIdx = i; break; }
                    nSyncCnt = (m_slots.Count > 0) ? (nIdx * 30 / m_slots.Count) : 0;
                }
                if (++nSyncCnt >= 30)
                {
                    nSyncCnt = 0;
                    for (int nTrk = m_nFrTrackNo; nTrk <= m_nToTrackNo; nTrk++) CvDic.Remove(nTrk);
                }
                m_dicCacheSyncCnt[m_strPlc_No] = nSyncCnt;
            }
            try
            {
                string numStr = System.Text.RegularExpressions.Regex.Match(m_strPlc_No, @"\d+").Value;
                int cvMachineNo = 0;
                int.TryParse(numStr, out cvMachineNo);
                if (cvMachineNo < 1) return true;   // PLC_NO 파싱 불가 시 스킵

                int mBase, palletOfs, dirWordAddr;
                if (cDefApp.GM_ADDR_V09)
                {
                    // [V0.9] XGK 래더 직접 표기
                    mBase       = 160 + (cvMachineNo - 1) * 20;      // 이벤트 블록 (M0160~)
                    palletOfs   = 10;                                // PalletExist 블록 = +10
                    dirWordAddr = 300 + (cvMachineNo - 1);           // 방향 (구 D0300~)
                }
                else
                {
                    // [V1.1] ezMCS 매핑 (워드 정렬 블록)
                    // [LGLS 2026-08-19] 주소맵 XML 우선, 없으면 내장 계산식 폴백
                    mBase       = 256 + (cvMachineNo - 1) * 32;      // 이벤트 블록 (%MX256~)
                    palletOfs   = 16;                                // PalletExist 블록 = +16
                    dirWordAddr = (960 + (cvMachineNo - 1) * 2) / 2; // 방향 (%MB960~ → 워드 480~)
                    int xB = cPlcAddrMap.BlockBase("CV", cvMachineNo, "Event");
                    if (xB >= 0) mBase = xB;
                    int xP = cPlcAddrMap.SignalOffset("CV", "Event", "PalletExist");
                    if (xP >= 0) palletOfs = xP;
                    int xD = cPlcAddrMap.Addr("CV", cvMachineNo, "Direction", "IoDirection");
                    if (xD >= 0) dirWordAddr = xD;
                }

                // 시나리오 맵은 CV당 최대 2포지션 (PalletExist #1/#2, Tracking Position #1/#2)
                int nSlots = m_nToTrackNo - m_nFrTrackNo + 1;
                if (nSlots < 1) nSlots = 1;
                int nMaxSlot = cPlcAddrMap.BlockMaxSlots("CV", "Tracking", 3);   // [LGLS 2026-08-19] XML 우선
                if (nSlots > nMaxSlot) nSlots = nMaxSlot;   // [LGLS 2026-07-21] 물리 이관: 3트랙 C/V(#14/#15) 대응 — 동일 READ의 개수·슬롯 매핑만 확장(프레임 무변경). EQP_SIM이 WCS 모드에서 관측값 번호를 트랙 순서로 기록해 슬롯 s=트랙 FR+s 정렬이 전 CV 성립

                // ── 1) M 이벤트/PalletExist 블록 읽기 (2워드에 모두 포함) ──────────
                int mWordAddr  = mBase / 16;
                int mBitOffset = mBase % 16;

                byte[] byMBuff = new byte[100];
                Array.Clear(byMBuff, 0, byMBuff.Length);
                if (!m_msQPlc.READ((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                   (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_M,
                                   mWordAddr, 2, ref byMBuff))
                {
                    MakeMsg_Error(strTitle + " M상태 읽기 실패: " + m_msQPlc.GetErrorMsg(), m_nthNo);
                    return false;
                }

                bool opMode   = GetMBitFromBuf(byMBuff, mBitOffset + 8);  // Op Mode
                bool inReady2 = GetMBitFromBuf(byMBuff, mBitOffset + 7);  // InReady #2

                // ── 2) D 방향 워드 읽기 (0=입고, 1=출고) ───────────────────────────
                byte[] byDBuff = new byte[100];
                Array.Clear(byDBuff, 0, byDBuff.Length);
                if (!m_msQPlc.READ((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                   (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D,
                                   dirWordAddr, 1, ref byDBuff))
                {
                    MakeMsg_Error(strTitle + " 방향워드 읽기 실패: " + m_msQPlc.GetErrorMsg(), m_nthNo);
                    return false;
                }
                int nDir = byDBuff[0] | (byDBuff[1] << 8);

                // ── 3) R 트래킹 JOB NO 읽기 (포지션당 2워드) ───────────────────────
                byte[] byRBuff = new byte[100];
                Array.Clear(byRBuff, 0, byRBuff.Length);
                if (!m_msQPlc.READ((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                                   (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_R,
                                   GetRTrackingAddr(m_nFrTrackNo), nSlots * 2, ref byRBuff))
                {
                    MakeMsg_Error(strTitle + " R트래킹 읽기 실패: " + m_msQPlc.GetErrorMsg(), m_nthNo);
                    return false;
                }

                // ── 4) 포지션(트랙)별 변화 감지 → CV_DATA UPDATE ──────────────────
                string AUTO_MODE = opMode ? "1" : "0";
                string STOCK     = Convert.ToString(nDir);
                string STO_READY = inReady2 ? "1" : "0";

                MakeMsg("상태값 DB저장", m_nthNo);

                for (int s = 0; s < nSlots; s++)
                {
                    int nCvNo = m_nFrTrackNo + s;
                    string SENSOR0 = GetMBitFromBuf(byMBuff, mBitOffset + palletOfs + s) ? "1" : "0";
                    string strJobNo = DecodeJobNoR(byRBuff, s * 4);

                    if (!CvDic.ContainsKey(nCvNo))
                        CvDic.Add(nCvNo, new CVData());
                    CVData cv = CvDic[nCvNo];

                    // 변경된 컬럼만 SET 절에 추가 ('0'/'1'/숫자만 들어가므로 값 직접 결합)
                    string strSet = "";
                    if ((cv.AUTO_MODE_RD ?? "") != AUTO_MODE)
                        strSet += cDefApp.CRLF + "      ,AUTO_MODE_RD = '" + AUTO_MODE + "'       ";
                    if ((cv.V11_DIR ?? "") != STOCK)
                        strSet += cDefApp.CRLF + "      ,STOCK_MODE = '" + STOCK + "'             ";
                    if ((cv.SENSOR0_DATA_RD ?? "") != SENSOR0)
                        strSet += cDefApp.CRLF + "      ,SENSOR0_DATA_RD = '" + SENSOR0 + "'      ";
                    if ((cv.V11_JOBNO ?? "") != strJobNo)
                        strSet += cDefApp.CRLF + "      ,LUGG_NO_RD = '" + strJobNo + "'          ";
                    if (s == 1 && (cv.STO_READY_RD ?? "") != STO_READY)
                        strSet += cDefApp.CRLF + "      ,STO_READY_RD = '" + STO_READY + "'       ";

                    if (strSet.Length == 0)
                        continue;   // 변화 없음

                    m_msQPlc._pBdb.BeginTrans();

                    string strSql = "";
                    strSql += cDefApp.CRLF + "UPDATE CV_DATA                                     ";
                    strSql += cDefApp.CRLF + "   SET READ_UPD_DT = " + DbLang.SYSDATE + "        ";
                    strSql += strSet;
                    strSql += cDefApp.CRLF + " WHERE WH_TYP = :WH_TYP                            ";
                    strSql += cDefApp.CRLF + "   AND PLC_NO = :PLC_NO                            ";
                    strSql += cDefApp.CRLF + "   AND MC_NO  = :MC_NO                             ";

                    m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                    m_msQPlc._pBdb.mComMain.Parameters.Clear();
                    m_msQPlc._pBdb.mComMain.Parameters.Add("WH_TYP", DbLang.VARCHAR, 255).Value = m_strWh_typ;
                    m_msQPlc._pBdb.mComMain.Parameters.Add("PLC_NO", DbLang.VARCHAR, 255).Value = m_strPlc_No;
                    m_msQPlc._pBdb.mComMain.Parameters.Add("MC_NO", DbLang.VARCHAR, 255).Value = nCvNo;

                    if (m_msQPlc._pBdb.ExcuteNonQry(strSql) < 0)
                    {
                        m_msQPlc._pBdb.Rollback();
                        MakeMsg_Error(strTitle + " CV_DATA 상태 반영 실패 MC_NO[" + nCvNo + "] MSG[" + m_msQPlc._pBdb.ErrMsg + "]", m_nthNo);
                        return false;
                    }
                    m_msQPlc._pBdb.Commit();

                    // 캐시 갱신 및 주요 변화 로그
                    if ((cv.SENSOR0_DATA_RD ?? "") != SENSOR0)
                        MakeMsg_Imp(strTitle + " MC_NO[" + nCvNo + "] PalletExist " + (SENSOR0 == "1" ? "ON" : "OFF")
                                    + " (%MX" + (mBase + palletOfs + s) + ")", m_nthNo);
                    if ((cv.V11_JOBNO ?? "") != strJobNo)
                        InsertWcsLogPgr(nCvNo.ToString("000"), strTitle + " R트래킹 JOB NO 변경 [" + (cv.V11_JOBNO ?? "") + "] -> [" + strJobNo + "]");
                    if ((cv.AUTO_MODE_RD ?? "") != AUTO_MODE)
                        MakeMsg_Imp(strTitle + " MC_NO[" + nCvNo + "] Op Mode = " + AUTO_MODE, m_nthNo);

                    cv.AUTO_MODE_RD = AUTO_MODE;
                    cv.V11_DIR = STOCK;
                    cv.SENSOR0_DATA_RD = SENSOR0;
                    cv.V11_JOBNO = strJobNo;
                    if (s == 1) cv.STO_READY_RD = STO_READY;
                }

                // [LGLS] 설비 통신상태(EQP_MST) 하트비트 — 구 CvStatus() 끝(Communication("Y",...))에 있던 것.
                //   CvStatusScenario 로 대체하면서 누락되어 EQP_MST.UPD_DT 가 갱신되지 않았고,
                //   WCS Client 상태표시줄 EQUIP 판정(CONNECTED_YN='Y' AND DATEDIFF(SECOND,UPD_DT,GETDATE())<=5)이
                //   항상 실패해 **EQUIP 이 빨강**으로 표시됨. 여기서 복원한다.
                // [LGLS 2026-07-30] 1소켓 통합 후에는 슬롯 순회 주기(~10초)가 Client 임계 5초를 넘겨 EQUIP 이
                //   깜빡였다 → 이 설비만이 아니라 등록된 전 설비 행을 함께 갱신(슬롯 처리마다 호출되므로 ~1초 신선도).
                CommunicationAllSlots("Y");
                return true;
            }
            catch (Exception ex)
            {
                MakeMsg_Error(strTitle + " Exception: " + ex.Message, m_nthNo);
                Communication("N", m_strWh_typ, m_strEqmt_typ, m_strPlc_No);
                return false;
            }
        }

        /// <summary>
        /// R 트래킹 2워드(4바이트 ASCII) → JOB NO 문자열 (EncodeJobNoR 의 역변환).
        /// 미기록(0x00)이거나 숫자가 아니면 "0" 반환.
        /// </summary>
        private string DecodeJobNoR(byte[] buf, int offset)
        {
            char[] c = new char[4];
            for (int i = 0; i < 4; i++)
            {
                byte b = buf[offset + i];
                if (b < (byte)'0' || b > (byte)'9')
                    return "0";
                c[i] = (char)b;
            }
            return new string(c);
        }
        #endregion
    }
}
