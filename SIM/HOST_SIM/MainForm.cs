using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;
using HOST_SIM.Comm;
using HOST_SIM.Logic;
using HOST_SIM.Protocol;

namespace HOST_SIM
{
    public partial class MainForm : Form
    {
        private SimConfig config;
        private EcsChannel ecsChannel;
        private ReportServer reportServer;
        private readonly List<CycleLogic> logics = new List<CycleLogic>();
        private readonly object logicLock = new object();

        private bool firstStatusReceived;   // 첫 상태보고(S) 수신 여부
        // [LGLS 2026-08-22] 101(C/V#11, 트랙 22) 출고 모드 자동 복귀.
        //   출고 화물이 반출되어 화물감지가 사라지면 일정 시간 뒤 입고 모드로 되돌린다.
        //   (출고로 남아 있으면 설비가 입고대에 화물을 올리지 못해 다음 입고가 시작되지 않는다)
        private DateTime cv11EmptySince = DateTime.MinValue;
        private const int CV11_AUTO_IN_MS = 5000;
        private bool runningCycle;          // 시작 버튼 눌린 상태

        // [LGLS 2026-07-30] SC(크레인) 상태표 — 인터페이스명세서(20100311) 집계 S 상태보고(BODY 43)에서 갱신.
        //   상태값(명세): 0=가능, 1=불가 (WCS 가 오프라인/에러/입고·전체정지를 불가로 집계해 보냄)
        private readonly Dictionary<int, int> scStatusMap = new Dictionary<int, int>();

        // [LGLS] 이중입고/공출고 재지정: 마지막 E 에러보고 정보 (버튼 핸들러에서 사용)
        private string redirLuggD, redirCellD; private int redirScD;   // 이중입고(ErrorKind=1)
        private string redirLuggE, redirCellE; private int redirScE;   // 공출고(ErrorKind=3)

        /// <summary>SC가 입고 작업을 받을 수 있는 상태인지 (명세 S 상태: 0=가능, 1=불가)</summary>
        private bool IsScIngoAvailable(int sc)
        {
            int st;
            lock (scStatusMap)
            {
                if (!scStatusMap.TryGetValue(sc, out st)) return true;   // 미수신 크레인은 기존 동작 유지
            }
            return st == 0;
        }
        private int luggageSeq;             // 작업번호 시퀀스 (파일 보존)
        private string seqFile;
        private StreamWriter logWriter;

        // [LGLS 2026-07-24] [CV#2 교착 TEST] — SC1(901) 집중 스트레스: 출고 9건(124/129/122×3) +
        //   입고 9건(126/130/122×3 → 901 랙). 전 건 완료 시 보고 메시지박스.
        private readonly HashSet<string> cv2TestPending = new HashSet<string>();
        private bool cv2TestActive;
        private const int CV2_TEST_TOTAL = 18;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            config = new SimConfig(Path.Combine(SimConfig.BaseDir, "HOST_SIM.ini"));

            string dataDir = Path.Combine(SimConfig.BaseDir, "Data");
            Directory.CreateDirectory(dataDir);
            seqFile = Path.Combine(dataDir, "luggage_seq.txt");
            LoadSequence();
            logWriter = new StreamWriter(Path.Combine(dataDir,
                "host_sim_" + DateTime.Now.ToString("yyyyMMdd_HHmmss") + ".log"), true, Encoding.UTF8) { AutoFlush = true };

            BuildLogics();

            // 보고 채널 서버 (ECS → HOST : S/E/F)
            reportServer = new ReportServer(config.GetInt("REPORT", "PORT", 9910));
            reportServer.ReportReceived += OnReportReceived;
            reportServer.ClientChanged += up => SafeUI(() =>
            {
                lblReportConn.Text = "보고 채널(ECS→): " + (up ? "접속됨" : "미접속");
                lblReportConn.ForeColor = up ? System.Drawing.Color.Green : System.Drawing.Color.Red;
            });
            reportServer.Error += msg => Log("ERR", msg);
            try
            {
                reportServer.Start();
                Log("SYS", "보고 채널 수신대기 시작 (port " + config.GetInt("REPORT", "PORT", 9910) + ")");
            }
            catch (Exception ex)
            {
                Log("ERR", "보고 채널 포트 열기 실패: " + ex.Message);
            }

            // 명령 채널 클라이언트 (HOST → ECS : O + ACK/NAK)
            string ecsIp = config.Get("ECS", "IP", "127.0.0.1");
            int ecsPort = config.GetInt("ECS", "PORT", 9911);
            ecsChannel = new EcsChannel(ecsIp, ecsPort);
            ecsChannel.ConnectionChanged += up => SafeUI(() =>
            {
                lblEcsConn.Text = "명령 채널(→ECS): " + (up ? "접속됨" : "미접속");
                lblEcsConn.ForeColor = up ? System.Drawing.Color.Green : System.Drawing.Color.Red;
            });
            ecsChannel.ReplyReceived += OnReplyReceived;
            ecsChannel.Sent += (kind, text) => SafeUI(() => AddSend(kind, text));
            ecsChannel.Error += msg => Log("ERR", msg);
            ecsChannel.Start();
            Log("SYS", "명령 채널 접속 시도 (" + ecsIp + ":" + ecsPort + ")");

            SetupLogicCheckboxes();
            UpdateLogicLabels();
            lblStatus.Text = "첫 상태보고(S) 수신 대기 중 — 수신 시 [시작] 버튼이 활성화됩니다.";
        }

        /// <summary>로직별 실행 선택 체크박스 초기화 (INI 에서 제외된 로직은 비활성)</summary>
        private void SetupLogicCheckboxes()
        {
            CheckBox[] checks = { chkLogic1, chkLogic2, chkLogic3 };
            for (int i = 0; i < checks.Length; i++)
            {
                CycleLogic logic = FindLogic(i + 1);
                if (logic == null)
                {
                    checks[i].Checked = false;
                    checks[i].Enabled = false;
                }
                else
                {
                    checks[i].Checked = logic.Enabled;
                }
            }
        }

        private CycleLogic FindLogic(int no)
        {
            string name = "로직" + no;
            foreach (var logic in logics)
                if (logic.Name == name) return logic;
            return null;
        }

        /// <summary>
        /// 로직 실행 선택 변경.
        ///  - 해제: 진행 중인 작업은 계속 진행되어 출고 완료 후 더 이상 입고를 만들지 않고 일시정지
        ///  - 재선택: 직전 출고 랙의 +1 셀에서 입고 작업 재개
        /// </summary>
        private void chkLogic_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox chk = (CheckBox)sender;
            int no = chk == chkLogic1 ? 1 : chk == chkLogic2 ? 2 : 3;
            lock (logicLock)
            {
                CycleLogic logic = FindLogic(no);
                if (logic == null) return;
                logic.Enabled = chk.Checked;
                if (chk.Checked)
                {
                    Log("SYS", logic.Name + " 활성화" + (runningCycle ? " — 재개" : ""));
                    if (runningCycle && logic.Phase == CyclePhase.Stopped)
                        logic.Start();                       // 랙+1 셀에서 입고 재개
                }
                else
                {
                    Log("SYS", logic.Name + " 비활성화 — 진행 중 사이클은 출고 완료 후 일시정지");
                }
            }
            UpdateLogicLabels();
        }

        private void BuildLogics()
        {
            int rackBase = config.GetInt("COMMON", "RACK_STATION_BASE", 200);
            int maxBay = config.GetInt("COMMON", "MAX_BAY", 10);
            int maxLevel = config.GetInt("COMMON", "MAX_LEVEL", 10);

            for (int i = 1; i <= 3; i++)
            {
                string sec = "LOGIC" + i;
                if (!config.GetBool(sec, "ENABLED", true)) continue;
                int[] scList = config.GetIntList(sec, "SC", new[] { i });
                var alloc = new RackAllocator(RackAllocator.BanksForScList(scList), maxBay, maxLevel);
                var logic = new CycleLogic(
                    "로직" + i, "SIM-LOGIC" + i,
                    config.Get(sec, "IN_STATION", "101"),
                    config.Get(sec, "OUT_STATION", "101"),
                    rackBase, alloc, NextLuggageNo, SendOrder,
                    IsScIngoAvailable, msg => Log("SYS", msg));   // [LGLS] 정지/에러 크레인 건너뛰기
                logics.Add(logic);
            }
        }

        // ------------------------------------------------------------------
        // 작업번호 발번 (재시작 시 중복 방지를 위해 파일에 보존)
        // ------------------------------------------------------------------
        private void LoadSequence()
        {
            luggageSeq = 0;
            try
            {
                if (File.Exists(seqFile))
                {
                    string[] parts = File.ReadAllText(seqFile).Split('|');
                    if (parts.Length == 2 && parts[0] == DateTime.Now.ToString("yyyyMMdd"))
                        int.TryParse(parts[1], out luggageSeq);
                }
            }
            catch { }
        }

        private string NextLuggageNo()
        {
            lock (seqFile)
            {
                luggageSeq++;
                // [LGLS 2026-07-24] 온라인 작업번호는 9000 미만이어야 함(HOST_TASK IsOnlineLuggNum: 0<n<9000).
                //   9000 이상은 오프라인/수동 예약 → 넘어가면 ECS 가 사유10(잘못된 작업번호)로 NAK. 8999 에서 순환.
                if (luggageSeq >= 9000) luggageSeq = 1;
                try { File.WriteAllText(seqFile, DateTime.Now.ToString("yyyyMMdd") + "|" + luggageSeq); } catch { }
                return luggageSeq.ToString().PadLeft(4, '0');
            }
        }

        // ------------------------------------------------------------------
        // 송수신
        // ------------------------------------------------------------------
        private void SendOrder(CycleLogic logic, byte[] body, string desc)
        {
            Log("ORD", desc);
            if (!ecsChannel.Send(body, desc))
            {
                Log("ERR", desc + " — 전송 실패(미접속). 로직 정지");
                logic.Stop();
            }
            SafeUI(UpdateLogicLabels);
        }

        private void OnReplyReceived(WmsFrame frame)
        {
            // ACK/NAK (BODY 11): [1]=type소문자, [2]=A/N, [3,2]=reason, [5,4]=luggage
            string kind = "응답";
            try
            {
                if (frame.Body.Length >= 11)
                {
                    char type = (char)frame.Body[1];
                    char ack = (char)frame.Body[2];
                    string reason = frame.BodyText(3, 2);
                    string luggage = frame.BodyText(5, 4);
                    kind = string.Format("{0} {1} (사유 {2}, JOB {3})",
                        char.ToUpper(type), ack == 'A' ? "ACK" : "NAK", reason, luggage);
                    if (ack != 'A') Log("NAK", "ECS 가 전문을 거부: " + kind);
                }
            }
            catch { }
            SafeUI(() => AddRecv(kind, frame.Display));
        }

        private void OnReportReceived(WmsFrame frame)
        {
            string kind;
            switch (frame.Type)
            {
                case 'S':
                    kind = "S 상태보고";
                    if (!firstStatusReceived)
                    {
                        firstStatusReceived = true;
                        SafeUI(() =>
                        {
                            if (!runningCycle) btnStart.Enabled = true;
                            lblStatus.Text = "첫 상태보고 수신 — 시작 가능";
                        });
                        Log("SYS", "첫 상태보고(S) 수신 → [시작] 버튼 활성화");
                    }
                    UpdateScStatus(frame);   // [LGLS] 크레인 상태표 갱신 + 크레인 대기 로직 재개 시도
                    break;

                case 'F':
                    {
                        // F 작업완료 (BODY 10): [2]=JobDefine, [3,4]=LuggageNo, [7]=CompleteClass, [8]=StepCount
                        char jobDefine = (char)frame.Body[2];
                        string luggage = frame.BodyText(3, 4);
                        char stepCount = frame.Body.Length > 8 ? (char)frame.Body[8] : '1';
                        kind = string.Format("F 작업완료 ({0}, JOB {1}, step {2})",
                            jobDefine == '1' ? "입고" : jobDefine == '2' ? "출고" : "이동", luggage, stepCount);
                        HandleComplete(jobDefine, stepCount, luggage);
                    }
                    break;

                case 'E':
                    kind = "E 에러보고";
                    Log("ERR", "ECS 에러보고 수신: " + frame.Display);
                    HandleErrorReport(frame);
                    break;

                default:
                    kind = frame.Type + " 전문";
                    break;
            }
            SafeUI(() => AddRecv(kind, frame.Display));
        }

        // [LGLS] E 에러보고 파싱 → 이중입고/공출고 재지정 버튼 활성화 + 정보 저장
        //   [2026-07-30] 인터페이스명세서(20100311) E BODY 23:
        //   [1]'E' [2]DeviceClass [3-5]DeviceNo(S/C 001~005) [6]ErrorKind [7-10]ErrorCode
        //   [11-14]LuggNo [15-16]Bank [17-19]Bay [20-21]Level   (구 24B WhDefine 규격 폐기)
        private void HandleErrorReport(WmsFrame frame)
        {
            try
            {
                if (frame.Body.Length != 23) return;
                char errKind = (char)frame.Body[6];
                char devClass = (char)frame.Body[2];
                // [LGLS 2026-09-01] DeviceClass=3(RTV) 확장 — 재지정 대상이 아니므로 로그만 남긴다
                if (devClass == '3')
                {
                    Log("SYS", string.Format("RTV 에러보고: RTV{0} 코드={1} JOB={2}",
                        frame.BodyText(3, 3), frame.BodyText(7, 4), frame.BodyText(11, 4)));
                    return;
                }
                int devNo; int.TryParse(frame.BodyText(3, 3), out devNo);
                int sc = devNo > 900 ? devNo - 900 : devNo;        // 명세 001~005 (구 901~905 호환)
                string lugg = frame.BodyText(11, 4);
                string bank = frame.BodyText(15, 2);
                string bay  = frame.BodyText(17, 3);
                string lev  = frame.BodyText(20, 2);
                string cell = bank + bay.Substring(1, 2) + lev;    // BBbbLL (Bay 3자리 중 뒤 2자리)
                if (errKind == '1')          // 이중입고
                {
                    redirLuggD = lugg; redirCellD = cell; redirScD = sc;
                    SafeUI(() => { btnRedirectDouble.Enabled = true;
                        lblStatus.Text = string.Format("이중입고 발생: JOB {0} SC{1} 랙 {2} — [이중입고 재지정] 가능", lugg, sc, cell); });
                    Log("SYS", string.Format("★이중입고(ErrorKind=1): JOB={0} SC{1} 셀={2} → 재지정 버튼 활성화", lugg, sc, cell));
                }
                else if (errKind == '3')     // 공출고
                {
                    redirLuggE = lugg; redirCellE = cell; redirScE = sc;
                    SafeUI(() => { btnRedirectEmpty.Enabled = true;
                        lblStatus.Text = string.Format("공출고 발생: JOB {0} SC{1} — [공출고 재지정] 가능", lugg, sc); });
                    Log("SYS", string.Format("★공출고(ErrorKind=3): JOB={0} SC{1} → 재지정 버튼 활성화", lugg, sc));
                }
            }
            catch { }
        }

        // [이중입고 재지정 처리]: 현재 입고중 랙셀 +1 로케이션으로 R 전문(R_Kind=1) 발행
        private void btnRedirectDouble_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(redirLuggD)) return;
            string newCell = NextLevelCell(redirCellD);
            // [LGLS 2026-07-30] 명세서 Station No 체계: S/C = 001~005 (WCS 가 901~905 로 변환)
            string stn = redirScD.ToString();
            byte[] body = WmsMessage.BuildRedirect('1', redirLuggD, stn, redirCellD, stn, newCell, '1', redirScD.ToString());
            ecsChannel.Send(body, "이중입고 재지정 JOB=" + redirLuggD);
            Log("ORD", string.Format("[이중입고 재지정] JOB={0} SC{1}: {2} → {3} (R_Kind=1)", redirLuggD, redirScD, redirCellD, newCell));
            btnRedirectDouble.Enabled = false;
            redirLuggD = null;
        }

        // [공출고 재지정 처리]: 공출고난 출고작업을 강제 완료 → 입고작업 재생성(SendIngo)
        private void btnRedirectEmpty_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(redirLuggE)) return;
            // [LGLS 2026-09-05] 종전에는 내부 사이클만 강제 완료하고 ECS 에 아무것도 보내지 않아,
            //   WCS 쪽 작업이 공출고 에러 상태(07)에 그대로 남았다. 이중입고와 같이 R 전문(R_Kind=2)으로
            //   ★다른 출발 셀★ 을 내려 준다. 같은 S/C 호기 안에서 레벨만 바꾼다(명세 : 동일 호기여야 함).
            string newCell = NextLevelCell(redirCellE);
            string stn = redirScE.ToString();
            byte[] body = WmsMessage.BuildRedirect('2', redirLuggE, stn, redirCellE, stn, newCell, '2', redirScE.ToString());
            ecsChannel.Send(body, "공출고 재지정 JOB=" + redirLuggE);
            Log("ORD", string.Format("[공출고 재지정] JOB={0} SC{1}: {2} → {3} (R_Kind=2)", redirLuggE, redirScE, redirCellE, newCell));
            btnRedirectEmpty.Enabled = false;
            redirLuggE = null;
            SafeUI(UpdateLogicLabels);
        }

        // 셀 BBbbLL 의 Level +1
        private static string NextLevelCell(string cell)
        {
            if (string.IsNullOrEmpty(cell) || cell.Length != 6) return cell;
            int lev; int.TryParse(cell.Substring(4, 2), out lev);
            lev += 1; if (lev > 99) lev = 1;
            return cell.Substring(0, 4) + lev.ToString("00");
        }

        /// <summary>
        /// [LGLS 2026-07-30] 인터페이스명세서(20100311) 집계 S 상태보고(BODY 43) 파싱:
        ///   [1]='S' + SC 블록 5개 [호기(2)+상태(1)+LuggNo(4)] = [2..8][9..15][16..22][23..29][30..36]
        ///   + 101/102/103 작업대 상태 [37][38][39] + 101/110 PLC 모드 [40][41]
        ///   상태값: 0=가능, 1=불가. 크레인 상태표를 갱신하고, 불가가 풀렸으면 크레인 대기 로직을 재개한다.
        ///   (구 WCS 설비별 개별 15B 전문 규격은 폐기 — 양측을 명세 43B 로 통일)
        /// </summary>
        private void UpdateScStatus(WmsFrame frame)
        {
            try
            {
                if (frame.Body.Length != 43) return;                 // 명세 집계 전문만
                bool anyChanged = false;
                for (int i = 0; i < 5; i++)
                {
                    int blk = 2 + i * 7;
                    int scNo, st;
                    if (!int.TryParse(frame.BodyText(blk, 2), out scNo)) continue;
                    if (!int.TryParse(frame.BodyText(blk + 2, 1), out st)) continue;
                    if (scNo < 1 || scNo > 5) continue;

                    bool changed;
                    lock (scStatusMap)
                    {
                        int prev;
                        changed = !scStatusMap.TryGetValue(scNo, out prev) || prev != st;
                        scStatusMap[scNo] = st;
                    }
                    if (changed)
                    {
                        anyChanged = true;
                        Log("SYS", string.Format("SC{0} 상태 = {1} ({2}) JOB={3}", scNo, st,
                            st == 0 ? "가능" : "불가", frame.BodyText(blk + 3, 4)));
                    }
                }

                // 크레인 대기 중이던 로직 재개 시도 (회복됐다면 Start 가 입고를 다시 만든다)
                lock (logicLock)
                {
                    if (runningCycle)
                        foreach (var logic in logics)
                            if (logic.Enabled && logic.PausedNoCrane && logic.Phase == CyclePhase.Stopped)
                                logic.Start();
                }
                if (anyChanged) SafeUI(UpdateLogicLabels);

                CheckCv11AutoInMode(frame);
            }
            catch { }
        }

        /// <summary>
        /// [LGLS 2026-08-22] 101 작업대(C/V#11, 트랙 22) 출고 모드 자동 복귀.
        ///   상태보고 [39]=101 PLC 모드(0=입고 1=출고), [37]=101 작업대 상태(0=가능 1=불가).
        ///   출고 모드인데 화물이 없어진(=가능) 상태가 CV11_AUTO_IN_MS 이상 이어지면
        ///   M 전문으로 입고 모드로 되돌린다.
        /// </summary>
        private void CheckCv11AutoInMode(WmsFrame frame)
        {
            try
            {
                if (frame.Body.Length != 43) return;

                string strAvail = frame.BodyText(37, 1);   // 101 작업대 상태 : 0=가능(화물 없음)
                string strMode  = frame.BodyText(39, 1);   // 101 PLC 모드   : 1=출고

                if (strMode != "1")
                {
                    cv11EmptySince = DateTime.MinValue;    // 이미 입고 모드
                    // [LGLS 2026-08-23] 입고 모드가 실제로 반영됐다. 모드 변경을 기다리던 로직이
                    //   그때 비로소 입고 작업을 만든다(출고 화물이 빠진 뒤에야 입고가 시작된다).
                    lock (logicLock)
                    {
                        if (runningCycle)
                            foreach (var logic in logics)
                                if (logic.Phase == CyclePhase.WaitInMode && logic.OnInModeConfirmed())
                                    Log("SYS", logic.Name + " 입고 모드 반영 확인 → 입고 작업 생성");
                    }
                    SafeUI(UpdateLogicLabels);
                    return;
                }
                if (strAvail != "0")
                {
                    cv11EmptySince = DateTime.MinValue;    // 아직 화물이 있다
                    return;
                }

                if (cv11EmptySince == DateTime.MinValue)
                {
                    cv11EmptySince = DateTime.Now;
                    return;
                }
                if ((DateTime.Now - cv11EmptySince).TotalMilliseconds < CV11_AUTO_IN_MS) return;

                cv11EmptySince = DateTime.MinValue;
                if (!ecsChannel.Connected) return;

                byte[] mode = WmsMessage.BuildModeChange(CycleLogic.CV11_CODE, '0');
                ecsChannel.Send(mode, "모드변경 C/V#11 → 입고(0) [출고 완료·화물 없음 " + (CV11_AUTO_IN_MS / 1000) + "초 경과]");
                Log("SYS", "[자동] C/V#11 출고 완료 후 화물 없음 " + (CV11_AUTO_IN_MS / 1000) + "초 경과 → 입고 모드로 복귀");
            }
            catch { }
        }

        private void HandleComplete(char jobDefine, char stepCount, string luggage)
        {
            lock (logicLock)
            {
                // [LGLS 2026-07-24] [CV#2 교착 TEST] 완료 귀속 — 로직 기동 여부와 무관하게 처리
                if (cv2TestActive && cv2TestPending.Contains(luggage))
                {
                    bool done = (jobDefine == WmsMessage.JOB_INGO)
                             || (jobDefine == WmsMessage.JOB_OUTGO && stepCount == '2');
                    if (done)
                    {
                        cv2TestPending.Remove(luggage);
                        int doneCnt = CV2_TEST_TOTAL - cv2TestPending.Count;
                        Log("SYS", string.Format("[CV#2 TEST] JOB {0} 완료 ({1}/{2})", luggage, doneCnt, CV2_TEST_TOTAL));
                        SafeUI(() => lblStatus.Text = string.Format("CV#2 교착 TEST 진행 중 (완료 {0}/{1})", doneCnt, CV2_TEST_TOTAL));
                        if (cv2TestPending.Count == 0)
                        {
                            cv2TestActive = false;
                            Log("SYS", "[CV#2 교착 TEST] 전체 " + CV2_TEST_TOTAL + "건 완료");
                            SafeUI(() =>
                            {
                                lblStatus.Text = "CV#2 교착 TEST 완료";
                                MessageBox.Show(this, "교착 테스트 완료", "[CV#2 교착 TEST]",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                            });
                        }
                    }
                    return;
                }

                if (!runningCycle) return;
                foreach (var logic in logics)
                {
                    if (logic.OnComplete(jobDefine, stepCount, luggage))
                    {
                        SafeUI(UpdateLogicLabels);
                        return;
                    }
                }
            }
        }

        // ------------------------------------------------------------------
        // [LGLS 2026-07-24] [CV#2 교착 TEST]
        //   선택 시: 로직1~3 전부 비활성화 후, SC1(901) 대상으로
        //   출고 9건(랙→124/129/122 각 3건) + 입고 9건(126/130/122→랙 각 3건)을 즉시 발행.
        //   SC1 의 모든 교통이 C/V#2(103/104) 한 라인을 공유하므로 교착 방지 게이트를 집중 검증한다.
        // ------------------------------------------------------------------
        /// <summary>
        /// [LGLS 2026-08-22] 겸용(양방향) 트랙 모드 강제 전환.
        ///   체크 = 입고 모드('0') / 해제 = 출고 모드('1') 로 M 전문(모드변경)을 즉시 발행한다.
        ///   C/V#11 = 명세(20100311) 작업대 101, C/V#2 = 확장 코드 105.
        ///   ECS(HOST_TASK)가 CV_DATA 커맨드 채널(CMD_RQ_ID='DIR')로 남기면
        ///   WCS_TASK_CV 가 설비 방향 워드에 기록하고, EQP_SIM 이 그 방향에 따라
        ///   RTV 도착지·출고HS·입고대·출고대 신호를 바꾼다.
        /// </summary>
        private void chkModeCv_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox cb = sender as CheckBox;
            if (cb == null) return;

            bool bCv11 = ReferenceEquals(cb, chkModeCv11);
            string strCode = bCv11 ? "101" : "105";
            string strName = bCv11 ? "C/V#11" : "C/V#2";
            char chMode = cb.Checked ? '0' : '1';               // 체크=입고, 해제=출고
            string strMode = cb.Checked ? "입고" : "출고";

            if (!ecsChannel.Connected)
            {
                MessageBox.Show(this, "ECS 명령 채널이 접속되지 않았습니다.", "[모드 변경]",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                // 되돌릴 때 핸들러가 재진입하지 않도록 이벤트를 잠시 뗀다
                cb.CheckedChanged -= chkModeCv_CheckedChanged;
                cb.Checked = !cb.Checked;
                cb.CheckedChanged += chkModeCv_CheckedChanged;
                return;
            }

            cb.Text = "[" + strName + " 모드 변경(" + strMode + "모드)]";
            byte[] body = WmsMessage.BuildModeChange(strCode, chMode);
            ecsChannel.Send(body, "모드변경 " + strName + " → " + strMode + "(" + chMode + ")");
            Log("SYS", "[모드 변경] " + strName + " → " + strMode + " 모드 (M 전문 코드 " + strCode + ")");
        }

        private void chkCv2Test_CheckedChanged(object sender, EventArgs e)
        {
            if (chkCv2Test.Checked)
            {
                if (!ecsChannel.Connected)
                {
                    MessageBox.Show(this, "ECS 명령 채널이 접속되지 않았습니다.", "[CV#2 교착 TEST]",
                        MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    chkCv2Test.Checked = false;
                    return;
                }
                // 기존 로직 1/2/3 전부 끄기 (체크 해제 → 기존 핸들러가 비활성 처리)
                chkLogic1.Checked = false;
                chkLogic2.Checked = false;
                chkLogic3.Checked = false;
                StartCv2DeadlockTest();
            }
            else
            {
                lock (logicLock)
                {
                    if (cv2TestActive || cv2TestPending.Count > 0)
                        Log("SYS", "[CV#2 교착 TEST] 중단 (잔여 " + cv2TestPending.Count + "건은 완료 추적만 해제)");
                    cv2TestActive = false;
                    cv2TestPending.Clear();
                }
                lblStatus.Text = "CV#2 교착 TEST 해제";
            }
        }

        private void StartCv2DeadlockTest()
        {
            int rackBase = config.GetInt("COMMON", "RACK_STATION_BASE", 200);
            string rackStation = (rackBase + 1).ToString();          // SC1 하이랙 스테이션 (기본 201)
            // [LGLS 2026-08-30] 현장기준(2026-08-24 정정) 반영 — C/V#12 = 24 입고대 / C/V#13 = 26 출고대.
            //   종전 목록은 정정 이전 값(124=출고대/126=입고대)이라 시험이 스스로 막혔다:
            //     · 출고 목적지를 입고대(124)로 잡아 EQP_SIM 자동투입 파렛트와 충돌 → 작업번호 없는 유령 화물이
            //       124 를 점유 → 출고 반출(10)이 못 끝남 → 출고 직렬화 게이트로 SC1 출고 전체 정지
            //       → 출고 우선 특례로 901행 입고 RTV 지시까지 보류(연쇄 정지)
            //     · 입고 출발지를 출고대(126)로 잡아 자동투입 대상이 아니라 파렛트가 영영 오지 않음
            string[] outStations = { "126", "129", "122" };          // 출고대 26(C/V#13) / 29(C/V#14) / 22(C/V#11)
            string[] inStations  = { "124", "130", "122" };          // 입고대 24(C/V#12) / 30(C/V#15) / 22(C/V#11)

            lock (logicLock)
            {
                cv2TestPending.Clear();
                cv2TestActive = true;

                int cellSeq = 0;
                Func<string> nextCell = delegate
                {
                    // SC1 담당 Bank 01/02 를 번갈아 Bay 전진 (Level 01 고정) — 18개 셀
                    string bank = (cellSeq % 2 == 0) ? "01" : "02";
                    string bay = ((cellSeq / 2) + 1).ToString().PadLeft(2, '0');
                    cellSeq++;
                    return bank + bay + "01";
                };

                // ① 출고 9건: SC1 랙 → 출고대(124/129/122) 각 3건
                foreach (string outStn in outStations)
                    for (int k = 0; k < 3; k++)
                    {
                        string lugg = NextLuggageNo();
                        string cell = nextCell();
                        string pallet = "P" + lugg.PadLeft(6, '0');
                        byte[] body = WmsMessage.BuildOrder(WmsMessage.JOB_OUTGO, lugg, lugg, pallet,
                            rackStation, cell, outStn, null, 1, "CV2-TEST");
                        cv2TestPending.Add(lugg);
                        Log("ORD", string.Format("[CV#2 TEST] 출고지시 JOB={0} 랙 {1}→{2}", lugg, cell, outStn));
                        ecsChannel.Send(body, "[CV#2 TEST] 출고 " + lugg + " 랙 " + cell + "→" + outStn);
                    }

                // ② 입고 9건: 입고대(126/130/122) → SC1 랙 각 3건
                foreach (string inStn in inStations)
                    for (int k = 0; k < 3; k++)
                    {
                        string lugg = NextLuggageNo();
                        string cell = nextCell();
                        string pallet = "P" + lugg.PadLeft(6, '0');
                        byte[] body = WmsMessage.BuildOrder(WmsMessage.JOB_INGO, lugg, lugg, pallet,
                            inStn, null, rackStation, cell, 1, "CV2-TEST");
                        cv2TestPending.Add(lugg);
                        Log("ORD", string.Format("[CV#2 TEST] 입고지시 JOB={0} {1}→랙 {2}", lugg, inStn, cell));
                        ecsChannel.Send(body, "[CV#2 TEST] 입고 " + lugg + " " + inStn + "→랙 " + cell);
                    }
            }

            lblStatus.Text = "CV#2 교착 TEST 진행 중 (완료 0/" + CV2_TEST_TOTAL + ")";
            Log("SYS", "[CV#2 교착 TEST] 시작 — 출고 9건(124/129/122) + 입고 9건(126/130/122→901)");
        }

        // ------------------------------------------------------------------
        // 버튼
        // ------------------------------------------------------------------
        private void btnStart_Click(object sender, EventArgs e)
        {
            if (!ecsChannel.Connected)
            {
                MessageBox.Show(this, "ECS 명령 채널이 접속되지 않았습니다.", "HOST_SIM",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            btnStart.Enabled = false;
            btnStop.Enabled = true;
            lblStatus.Text = "순환 로직 구동 중";
            Log("SYS", "[시작] — 전체 로직 기동");

            lock (logicLock)
            {
                runningCycle = true;
                foreach (var logic in logics) logic.Start();
            }
            UpdateLogicLabels();
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            lock (logicLock)
            {
                runningCycle = false;
                foreach (var logic in logics) logic.Stop();
            }
            btnStop.Enabled = false;
            btnStart.Enabled = firstStatusReceived;
            lblStatus.Text = "정지됨";
            Log("SYS", "[종료] — 전체 로직 정지");
            UpdateLogicLabels();
        }

        // ------------------------------------------------------------------
        // UI/로그
        // ------------------------------------------------------------------
        private const int MAX_LIST_ITEMS = 2000;

        private void AddSend(string kind, string text)
        {
            AddItem(lvSend, kind, text);
            Log("SEND", kind + " | " + text);
        }

        private void AddRecv(string kind, string text)
        {
            AddItem(lvRecv, kind, text);
            Log("RECV", kind + " | " + text);
        }

        private static void AddItem(ListView lv, string kind, string text)
        {
            var item = new ListViewItem(DateTime.Now.ToString("HH:mm:ss.fff"));
            item.SubItems.Add(kind);
            item.SubItems.Add(text);
            lv.BeginUpdate();
            lv.Items.Add(item);
            while (lv.Items.Count > MAX_LIST_ITEMS) lv.Items.RemoveAt(0);
            item.EnsureVisible();
            lv.EndUpdate();
        }

        private void UpdateLogicLabels()
        {
            Label[] labels = { lblLogic1, lblLogic2, lblLogic3 };
            for (int i = 0; i < labels.Length; i++)
                labels[i].Text = i < logics.Count ? logics[i].StatusText : "-";
        }

        private void SafeUI(Action action)
        {
            if (IsDisposed) return;
            if (InvokeRequired) { try { BeginInvoke(action); } catch { } }
            else action();
        }

        private void Log(string tag, string msg)
        {
            try
            {
                lock (logWriter)
                    logWriter.WriteLine("{0} [{1}] {2}", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"), tag, msg);
            }
            catch { }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            try { if (ecsChannel != null) ecsChannel.Dispose(); } catch { }
            try { if (reportServer != null) reportServer.Dispose(); } catch { }
            try { if (logWriter != null) logWriter.Close(); } catch { }
        }
    }
}
