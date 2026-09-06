using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using EQP_SIM.Core;

namespace EQP_SIM.Sim
{
    /// <summary>
    /// 시나리오 엔진: 100ms 주기로 모든 컨베이어/차량 상태 머신을 구동하고,
    /// 이벤트↔Ack 핸드셰이크(Event SET → ECS Ack SET → PLC가 둘 다 RESET, 슬라이드5)를 관리한다.
    /// 상태는 파일(Data\)에 저장한다 — DB 미사용.
    /// </summary>
    public class ScenarioEngine : IDisposable
    {
        public readonly WorldModel World;
        private readonly PlcIo io;

        /// <summary>
        /// [MODE] TARGET=WCS(기본) : 대체 WCS 체인 지원 — CV12/13 스왑 배치 + 재현/픽업 훅 + D영역 미러.
        /// [MODE] TARGET=ECS : 구(원본) ECS 연동 — 원본 배치(C/V#12=입고, C/V#13=출고),
        ///   차량 반송은 VehicleSim 의 실제 TRANSFER_REQUEST 핸드셰이크로만 진행(훅/미러 비활성).
        /// </summary>
        public bool WcsSupport { get; private set; }

        /// <summary>
        /// [MODE] LEGACY_HOOKS=1 : 구(§8~13) WCS 설계용 재현/픽업 훅 활성.
        /// 현행 WCS 설계(2026-07-17: EQP_TASK 는 마스터 PLC COMM0 만 브리지, 스테이션·차량 애니메이션은
        /// IO_TASK 가 DB 직접 구동 — FeedInGate/AutoRun*)에서는 훅이 IO_TASK 소유 상태를 오염시키므로 기본 OFF.
        /// (예: 122 입고 트래킹 기록을 '출고 재현'으로 오인 → 파렛트 즉시 배출 → 입고 레그 교착)
        /// </summary>
        public bool LegacyWcsHooks { get; private set; }
        private readonly Dictionary<string, ConveyorSim> conveyors = new Dictionary<string, ConveyorSim>();
        private readonly Dictionary<string, VehicleSim> vehicles = new Dictionary<string, VehicleSim>();

        // 진행 중 이벤트 핸드셰이크
        private class PendingEvent
        {
            public string Owner, EventName, AckName;
            public DateTime ExpireAt;   // Ack 미수신 시 자동 해제
        }
        private readonly List<PendingEvent> pendingEvents = new List<PendingEvent>();
        private readonly object sync = new object();

        // [LGLS 2026-07-20] 설비군별 처리 스레드 3개 (CV/SC/RTV — 사용자 요구 구조)
        private Thread cvTickThread, scTickThread, rtvTickThread;
        private volatile bool running;
        private volatile bool paused;   // [LGLS 2026-07-24] [시나리오 테스트] 자동 운전 일시정지
        private string dataDir;
        private DateTime nextSaveAt = DateTime.MinValue;
        private StreamWriter logWriter;

        // 타이밍 (ms) — ECS 판독주기(수 초)를 감안해 여유있게
        public int MoveMs = 2000;
        public int TravelMs = 2500;
        // [LGLS 2026-08-23] 크레인 구분 동작(포크 출/호이스트/포크 센터) 1단계 시간
        public int ForkStepMs = 3000;
        public int WaitOutHoldMs = 9000;
        public int SrcCargoTimeoutMs = 60000;   // [LGLS 2026-09-04] 차량이 출발지에서 화물을 기다리는 최대 시간(넘으면 지시 포기)
        public int OutRemoveMs = 3000;       // [LGLS 2026-08-22] 출고대 신호 ON 후 지게차가 화물을 가져가기까지
        public int OutTrackClearMs = 3000;   // [LGLS 2026-08-22] 화물 반출 후 남은 데이터(트래킹) 제거까지
        // [LGLS 2026-07-31] 차량(RGV/S/C) 상차 후 "출발지 화물·트래킹 소멸"을 확인하고 그 상태를 유지하는 시간.
        //   상위(WCS_TASK_CV) 미러 주기 동안 출발지 클리어가 관측되도록 보장해, 도착지에 먼저 화물이
        //   생기는 순서 역전을 막는다. 값이 상위 미러 1주기보다 짧으면 화면상 겹쳐 보일 수 있다.
        //   기본 4000ms = 상위 미러 1주기(계측 ~2.5~3.4초)보다 크게 잡은 값. 미러가 느려지면 이 값도 올릴 것.
        public int SrcClearDwellMs = 4000;
        /// <summary>[LGLS 2026-08-22] 출발지 클리어 대기 상한. 넘기면 경고 후 진행(교착 방지)</summary>
        public int SrcClearTimeoutMs = 20000;
        public int InReadyDelayMs = 3000;    // [LGLS 2026-08-22] 입고대 화물감지 유지 → 입고대(반출요청) ON 까지
        public int InSensorDelayMs = 2000;   // [LGLS 2026-07-21] 입고대 적재 후 재하감지 ON까지 지연 (PLC 2초 규약)
        public int FeedMs = 5000;
        public int AckTimeoutMs = 60000;
        // [LGLS 2026-07-31] 30000 → 60000. 상위가 작업을 배정(지시)한 뒤 PLC 트래킹에 작업번호가 실제로
        //   기록되기까지 '작업 접수 지연 + 미러 1주기'가 걸리는데, 30초는 그 최악값과 겹쳐
        //   지시받은 파렛트를 회수해 버리는 사고(작업 0221)를 냈다. 안전 마진을 2배로 둔다.
        //   (회수 직전 트래킹 재확인 로직과 함께 이중 방어 — ConveyorSim '작업자 회피' 블록)
        public int UnstampedTimeoutMs = 60000;   // 입출고 겸용 C/V 미지정 파렛트 회수 대기

        // [LGLS] 이중입고/공출고 에러 주입 (시나리오 테스트): 체크 시 다음 최초 입고/출고 S/C 작업에서 1회 발생
        public volatile bool InjectDoubleStorage = false;   // 이중입고 (입고 목적셀 이미 점유 → ERR 54)
        public volatile bool InjectEmptyRetrieval = false;  // 공출고 (출고 출발셀 재고없음 → ERR 58)

        public event Action<string> LogAdded;
        public event Action StateChanged;

        // 랙 재고 (셀 BBbbLL → JOB)
        public readonly Dictionary<string, string> Rack = new Dictionary<string, string>();
        public int CompletedOutgoCount { get; private set; }

        public ScenarioEngine(PlcIo io, SimConfig config)
        {
            this.io = io;
            WcsSupport = !"ECS".Equals(config.Get("MODE", "TARGET", "WCS"), StringComparison.OrdinalIgnoreCase);
            LegacyWcsHooks = WcsSupport && config.GetBool("MODE", "LEGACY_HOOKS", false);
            World = new WorldModel(WcsSupport);
            foreach (var def in World.Conveyors.Values)
                conveyors[def.Id] = new ConveyorSim(def, io, this);
            foreach (var def in World.Vehicles.Values)
                vehicles[def.Id] = new VehicleSim(def, io, this);
        }

        public ConveyorSim Conveyor(string id) { return conveyors[id]; }
        public IEnumerable<ConveyorSim> AllConveyors { get { return conveyors.Values; } }
        public IEnumerable<VehicleSim> AllVehicles { get { return vehicles.Values; } }

        /// <summary>[LGLS 2026-09-05] 설비(현장 조작반) 에러 해제 - 에러 상태인 차량을 모두 푼다. 해제한 대수를 돌려준다.</summary>
        public int ClearVehicleErrors()
        {
            lock (sync)
            {
                int n = 0;
                foreach (var v in vehicles.Values) if (v.ClearError()) n++;
                if (n == 0) Log("[설비 에러 해제] 에러 상태인 설비가 없습니다");
                return n;
            }
        }

        public void Start(string dataDir, SimConfig config)
        {
            this.dataDir = dataDir;
            Directory.CreateDirectory(dataDir);
            logWriter = new StreamWriter(Path.Combine(dataDir,
                "eqp_sim_" + DateTime.Now.ToString("yyyyMMdd_HHmmss") + ".log"), true, Encoding.UTF8) { AutoFlush = true };

            MoveMs = config.GetInt("TIMING", "MOVE_MS", MoveMs);
            TravelMs = config.GetInt("TIMING", "TRAVEL_MS", TravelMs);
            ForkStepMs = config.GetInt("TIMING", "FORK_STEP_MS", ForkStepMs);
            WaitOutHoldMs = config.GetInt("TIMING", "WAITOUT_HOLD_MS", WaitOutHoldMs);
            OutRemoveMs = config.GetInt("TIMING", "OUT_REMOVE_MS", OutRemoveMs);
            SrcCargoTimeoutMs = config.GetInt("TIMING", "SRC_CARGO_TIMEOUT_MS", SrcCargoTimeoutMs);
            OutTrackClearMs = config.GetInt("TIMING", "OUT_TRACK_CLEAR_MS", OutTrackClearMs);
            InSensorDelayMs = config.GetInt("TIMING", "IN_SENSOR_DELAY_MS", InSensorDelayMs);
            InReadyDelayMs  = config.GetInt("TIMING", "IN_READY_DELAY_MS", InReadyDelayMs);
            SrcClearTimeoutMs = config.GetInt("TIMING", "SRC_CLEAR_TIMEOUT_MS", SrcClearTimeoutMs);
            SrcClearDwellMs = config.GetInt("TIMING", "SRC_CLEAR_DWELL_MS", SrcClearDwellMs);
            FeedMs = config.GetInt("TIMING", "FEED_MS", FeedMs);
            UnstampedTimeoutMs = config.GetInt("TIMING", "UNSTAMPED_TIMEOUT_MS", UnstampedTimeoutMs);

            bool autoFeed = config.GetBool("FEED", "AUTO", true);
            foreach (var cv in conveyors.Values)
            {
                if (cv.Def.IngoPath != null && cv.Def.No >= 11)
                {
                    cv.AutoFeedConfigured = config.GetBool("FEED", "CV" + cv.Def.No, true);
                    cv.AutoFeed = autoFeed && cv.AutoFeedConfigured;
                }
                // [WCS 미러] 대체 WCS(WCS_TASK_CV)의 D영역 트랙 모델 매핑 (INI [WCS_MIRROR] CVnn=시작트랙)
                cv.WcsTrackBase = config.GetInt("WCS_MIRROR", "CV" + cv.Def.No, 0);
            }

            LoadState();

            foreach (var cv in conveyors.Values) cv.Initialize();
            foreach (var v in vehicles.Values) v.Initialize();

            running = true;
            // [LGLS 2026-07-20] CV/SC/RTV 설비군별 처리 스레드 분리 기동
            cvTickThread  = new Thread(() => TickLoop("CV"))  { IsBackground = true, Name = "SimTick:CV" };
            scTickThread  = new Thread(() => TickLoop("SC"))  { IsBackground = true, Name = "SimTick:SC" };
            rtvTickThread = new Thread(() => TickLoop("RTV")) { IsBackground = true, Name = "SimTick:RTV" };
            cvTickThread.Start(); scTickThread.Start(); rtvTickThread.Start();
            Log("시나리오 엔진 시작 [" + (WcsSupport ? "WCS 모드" : "구 ECS 모드") +
                "] (이동 " + MoveMs + "ms, 주행 " + TravelMs + "ms, WaitOut 체류 " + WaitOutHoldMs + "ms)");
        }

        // [LGLS 2026-07-20] 설비군별 처리 스레드(사용자 요구 구조): CV / SC / RTV 각각 별도 스레드에서
        //   해당 설비군의 명령 소비·동작·이벤트 핸드셰이크를 처리한다.
        //   월드(파렛트/랙/PLC 메모리)는 공유 자원이므로 sync 락으로 직렬화한다
        //   (크레인이 컨베이어 파렛트를 상·하차하는 교차 접근이 있어 무락 병렬은 유실을 만든다).
        // [LGLS 2026-09-02] 시뮬 월드에 없는 트랙의 R 트래킹 잔재 청소 (사용자 보고: 색 없는 번호 잔재).
        //   시뮬에 정의되지 않은 트랙에는 실물이 존재할 수 없으므로 그 R 은 '0000' 이 참값이다.
        //   CV 트래킹 영역(0 ~ 최대CV번호*10)만 대상으로 하고, 시뮬이 소유한 슬롯은 건드리지 않는다.
        private DateTime lastRSweep = DateTime.MinValue;
        private void SweepUnownedRTracking(DateTime now)
        {
            if ((now - lastRSweep).TotalMilliseconds < 2000) return;
            lastRSweep = now;
            var owned = new HashSet<int>();
            int maxNo = 0;
            foreach (var cv in conveyors.Values)
            {
                if (cv.Def.No < 1) continue;
                if (cv.Def.No > maxNo) maxNo = cv.Def.No;
                for (int i = 1; i <= cv.Def.Ports.Length; i++)
                    owned.Add((cv.Def.No - 1) * 10 + (i - 1) * 2);
            }
            if (maxNo < 1) return;
            int limit = maxNo * 10;
            for (int w = 0; w < limit; w += 2)
            {
                if (owned.Contains(w)) continue;
                string cur = (io.Memory.GetString('R', w, 2) ?? "").Trim().Trim(' ');
                if (cur.Length > 0 && cur != "0000")
                    io.Memory.SetString('R', w, 2, "0000");
            }
        }

        private void TickLoop(string kind)
        {
            while (running)
            {
                DateTime now = DateTime.Now;
                try
                {
                    lock (sync)
                    {
                        // [LGLS 2026-07-24] [시나리오 테스트] 일시정지 중에는 자동 운전(핸드셰이크/설비 Tick)을 멈춘다.
                        //   단 XGT 서버·메모리 저장(아래)은 계속 → 사용자가 수동으로 세팅한 EQP 메모리에 WCS 가 반응할 수 있음.
                        if (!paused)
                        {
                            ProcessHandshakes(now, kind);
                            if (kind == "CV")
                            {
                                foreach (var cv in conveyors.Values) cv.Tick(now);
                                SweepUnownedRTracking(now);   // [LGLS 2026-09-02] 미정의 트랙 R 잔재 청소
                            }
                            else if (kind == "SC")
                            {
                                foreach (var v in vehicles.Values) if (!v.Def.IsRgv) v.Tick(now);
                            }
                            else // RTV
                            {
                                foreach (var v in vehicles.Values) if (v.Def.IsRgv) v.Tick(now);
                            }
                        }
                    }
                    if (kind == "CV")
                    {
                        if (now >= nextSaveAt)
                        {
                            SaveState();
                            nextSaveAt = now.AddSeconds(2);
                        }
                        var h = StateChanged;
                        if (h != null) h();
                    }
                }
                catch (Exception ex)
                {
                    Log("엔진 오류(" + kind + "): " + ex.Message);
                }
                Thread.Sleep(100);
            }
        }

        // ------------------------------------------------------------------
        // 이벤트 ↔ Ack 핸드셰이크 (슬라이드 5: Reset By PLC When Event Reset)
        // ------------------------------------------------------------------
        public void RaiseEvent(string owner, string eventName, string ackName, int expireMs = 0)
        {
            io.SetBool(owner, eventName, true);
            lock (sync)
            {
                pendingEvents.Add(new PendingEvent
                {
                    Owner = owner,
                    EventName = eventName,
                    AckName = ackName,
                    ExpireAt = DateTime.Now.AddMilliseconds(expireMs > 0 ? expireMs : AckTimeoutMs)
                });
            }
        }

        /// <summary>이벤트 비트가 (Ack 완료로) 내려갔는지</summary>
        public bool IsEventCleared(string owner, string eventName)
        {
            return !io.GetBool(owner, eventName);
        }

        // [LGLS 2026-07-20] kind: 담당 설비군의 이벤트만 처리 (CV=CONVEYOR:*, RTV=VEHICLE:1, SC=그 외 VEHICLE:*)
        private void ProcessHandshakes(DateTime now, string kind)
        {
            for (int i = pendingEvents.Count - 1; i >= 0; i--)
            {
                var pe = pendingEvents[i];
                bool mine = (kind == "CV") ? pe.Owner.StartsWith("CONVEYOR:")
                          : (kind == "RTV") ? pe.Owner == "VEHICLE:1"
                          : pe.Owner.StartsWith("VEHICLE:") && pe.Owner != "VEHICLE:1";
                if (!mine) continue;
                bool acked = pe.AckName != null && io.GetBool(pe.Owner, pe.AckName);
                if (acked || now >= pe.ExpireAt)
                {
                    io.SetBool(pe.Owner, pe.EventName, false);
                    if (pe.AckName != null) io.SetBool(pe.Owner, pe.AckName, false);
                    pendingEvents.RemoveAt(i);
                    if (!acked && pe.AckName != null)
                        Log(pe.Owner + " " + pe.EventName + " Ack 타임아웃 — 자동 해제");
                }
            }
        }

        // ------------------------------------------------------------------
        // 랙 재고
        // ------------------------------------------------------------------
        public void StoreToRack(string cell, string palletId)
        {
            lock (sync) Rack[cell] = palletId ?? "";
        }

        public string TakeFromRack(string cell)
        {
            lock (sync)
            {
                string id;
                if (Rack.TryGetValue(cell, out id)) { Rack.Remove(cell); return id; }
                return null;
            }
        }

        public void NotifyOutgoRemoved(string jobId)
        {
            CompletedOutgoCount++;
        }

        // ------------------------------------------------------------------
        // 수동 조작 (UI)
        // ------------------------------------------------------------------
        public bool ManualInject(string conveyorId)
        {
            lock (sync)
            {
                ConveyorSim cv;
                return conveyors.TryGetValue(conveyorId, out cv) && cv.InjectPallet();
            }
        }

        // [LGLS 2026-07-24] [시나리오 테스트] — 자동 운전 정지/재개 + PLC 메모리 직접 접근
        public bool IsPaused { get { return paused; } }
        public void Pause()  { paused = true;  Log("[시나리오 테스트] 자동 운전 일시정지"); }
        public void Resume() { paused = false; Log("[시나리오 테스트] 자동 운전 재개"); }
        public PlcMemory Memory { get { return io.Memory; } }

        /// <summary>
        /// [LGLS 2026-08-22] 잔재 화물 강제 제거.
        ///   spec = "31"       → 포트 번호 (화면 표기 P31 과 동일. 통로 C/V 포함 전 설비)
        ///        = "131"      → 트랙 번호 (WCS 표기. ini [WCS_MIRROR] 설정 설비만)
        ///        = "13:25"    → 설비번호:포트번호
        /// </summary>
        public bool RemovePallet(string spec, out string msg)
        {
            ConveyorSim target; int order;
            if (!ResolveSlot(spec, out target, out order, out msg)) return false;
            return RemovePalletAt(target, order, out msg);
        }

        /// <summary>
        /// [LGLS 2026-09-06] 시험용 화물 생성 - 원하는 트랙/포트에 파렛트를 하나 올린다.
        ///   spec   : RemovePallet 과 같은 표기 ("31" 포트 / "131" 트랙 / "13:25" 설비:포트)
        ///   jobNo  : 비우면 ★작업번호 없는 화물★. 크레인이 수동조작으로 출고 H/S 에
        ///            내려놓은 상황([H/S 배출] 시험)을 그대로 만들 수 있다.
        ///   bOutgo : true = 출고 화물, false = 입고 화물
        /// </summary>
        public bool SpawnPallet(string spec, string jobNo, bool bOutgo, out string msg)
        {
            ConveyorSim target; int order;
            if (!ResolveSlot(spec, out target, out order, out msg)) return false;

            bool ok;
            lock (sync)
            {
                ok = target.SpawnPalletAt(order, jobNo, bOutgo ? FlowDir.Outgo : FlowDir.Ingo, out msg);
            }
            if (!ok) { Log("[시험] 화물 생성 실패 - " + msg); return false; }

            Log("[시험] " + msg);
            SaveState();
            return true;
        }

        /// <summary>[LGLS 2026-09-06] spec 문자열 → 설비 + 슬롯 (제거·생성이 함께 쓴다)</summary>
        private bool ResolveSlot(string spec, out ConveyorSim target, out int order, out string msg)
        {
            msg = "";
            target = null;
            order = 0;
            if (string.IsNullOrEmpty(spec)) { msg = "트랙 번호를 입력하세요"; return false; }
            spec = spec.Trim().ToUpper().Replace("CV", "").Replace("C/V", "").Replace("#", "");

            int colon = spec.IndexOf(':');
            if (colon > 0)
            {
                int no, port;
                if (!int.TryParse(spec.Substring(0, colon), out no) ||
                    !int.TryParse(spec.Substring(colon + 1), out port))
                { msg = "형식 오류 - 설비번호:포트번호 (예 13:25)"; return false; }
                foreach (var cv in AllConveyors)
                    if (cv.Def.No == no) { target = cv; order = cv.Def.OrderOf(port); break; }
                if (target == null) { msg = "C/V #" + no + " 없음"; return false; }
                if (order <= 0) { msg = "C/V #" + no + " 에 포트 " + port + " 없음"; return false; }
            }
            else
            {
                int n;
                if (!int.TryParse(spec, out n)) { msg = "숫자가 아님: " + spec; return false; }
                if (n < 100)
                {
                    // 두 자리 = 포트 번호(화면 표기 P25/P31 과 같다). 포트 번호는 전 설비에서 유일하다.
                    foreach (var cv in AllConveyors)
                    {
                        int o = cv.Def.OrderOf(n);
                        if (o > 0) { target = cv; order = o; break; }
                    }
                    if (target == null) { msg = "포트 " + n + " 을 가진 설비 없음"; return false; }
                }
                else
                {
                    // 세 자리 = 트랙 번호(WCS 표기 125/131). 미러 미설정 설비는 포트 번호로 지정한다.
                    foreach (var cv in AllConveyors)
                    {
                        int o = cv.OrderOfTrack(n);
                        if (o > 0) { target = cv; order = o; break; }
                    }
                    // [LGLS 2026-09-06] 미러(ini [WCS_MIRROR]) 미설정 설비 폴백 : 트랙 = 100 + 포트.
                    //   이 현장은 C/V#1~#15 전 설비가 이 규칙이다(C/V#2 포트3/4 = 트랙 103/104,
                    //   C/V#13 포트25/26 = 125/126, C/V#15 포트30/31/32 = 130/131/132).
                    //   종전에는 통로 C/V 를 포트 번호로만 지정할 수 있어 트랙 104 가 안 먹혔다.
                    if (target == null && n > 100 && n < 200)
                    {
                        int port = n - 100;
                        foreach (var cv in AllConveyors)
                        {
                            int o = cv.Def.OrderOf(port);
                            if (o > 0) { target = cv; order = o; break; }
                        }
                    }
                    if (target == null)
                    { msg = "트랙 " + n + " 을 가진 설비 없음 (통로 C/V 는 포트 번호로 지정)"; return false; }
                }
            }
            return true;
        }

        private bool RemovePalletAt(ConveyorSim target, int order, out string msg)
        {
            string what;
            bool had = target.ForceClearOrder(order, out what);
            if (!had)
            {
                msg = target.Def.Id + " 슬롯 " + order + " - 화물 없음 (신호만 정리)";
                Log(msg);
                return false;
            }
            msg = target.Def.Id + " 슬롯 " + order + " 화물 제거 - " + what;
            Log("[정리] " + msg);
            return true;
        }

        public void ResetAll()
        {
            lock (sync)
            {
                pendingEvents.Clear();
                Rack.Clear();
                foreach (var cv in conveyors.Values) cv.Pallets.Clear();
                io.Memory.Clear();
                foreach (var cv in conveyors.Values) cv.Initialize();
                foreach (var v in vehicles.Values) v.Initialize();
                SaveState();
                // [LGLS] 상태 저장 파일 삭제 — 재기동 시 잔류 시뮬 상태(화물감지·트래킹 등) 로드 방지
                try { if (File.Exists(StatePath)) File.Delete(StatePath); } catch { }
                try { if (File.Exists(MemoryPath)) File.Delete(MemoryPath); } catch { }
                // 루트 폴더에 남을 수 있는 구(舊) 상태파일도 정리
                try { if (File.Exists("eqp_state.txt")) File.Delete("eqp_state.txt"); } catch { }
            }
            Log("전체 상태/메모리 초기화 (상태 파일 삭제)");
        }

        // ------------------------------------------------------------------
        // 파일 저장/복원
        // ------------------------------------------------------------------
        private string StatePath { get { return Path.Combine(dataDir, "eqp_state.txt"); } }
        private string MemoryPath { get { return Path.Combine(dataDir, "plc_memory.bin"); } }

        private void SaveState()
        {
            try
            {
                var sb = new StringBuilder();
                lock (sync)
                {
                    foreach (var kv in Rack)
                        sb.AppendLine("RACK," + kv.Key + "," + kv.Value);
                    foreach (var cv in conveyors.Values)
                        foreach (string line in cv.SaveLines())
                            sb.AppendLine(line);
                }
                File.WriteAllText(StatePath, sb.ToString(), Encoding.UTF8);
                io.Memory.Save(MemoryPath);
            }
            catch (Exception ex)
            {
                Log("상태 저장 실패: " + ex.Message);
            }
        }

        private void LoadState()
        {
            try
            {
                if (!File.Exists(StatePath)) return;
                foreach (string raw in File.ReadAllLines(StatePath))
                {
                    string[] p = raw.Split(',');
                    if (p.Length >= 3 && p[0] == "RACK")
                    {
                        Rack[p[1]] = p.Length > 2 ? p[2] : "";
                    }
                    else if (p.Length >= 5 && p[0] == "CV")
                    {
                        ConveyorSim cv;
                        int idx, dir;
                        if (conveyors.TryGetValue(p[1], out cv) &&
                            int.TryParse(p[2], out idx) && int.TryParse(p[4], out dir))
                        {
                            cv.RestorePallet(idx, p[3], (FlowDir)dir);
                        }
                    }
                }
                Log("저장 상태 복원: 랙 " + Rack.Count + "셀, 컨베이어 파렛트 " +
                    conveyors.Values.Sum(c => c.Pallets.Count) + "개");
            }
            catch (Exception ex)
            {
                Log("상태 복원 실패: " + ex.Message);
            }
        }

        public void Log(string msg)
        {
            // [LGLS 2026-07-21] 파일 기록과 UI 이벤트를 완전 격리 — 어느 한쪽 예외가 엔진 틱/다른 로그를 못 죽이게.
            try
            {
                if (logWriter != null)
                    lock (logWriter)
                        logWriter.WriteLine(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + " " + msg);
            }
            catch { }
            try
            {
                var h = LogAdded;
                if (h != null) h(msg);
            }
            catch { }
        }

        public void Dispose()
        {
            running = false;
            try { SaveState(); } catch { }
            try { if (logWriter != null) logWriter.Close(); } catch { }
        }
    }
}
