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
        public int WaitOutHoldMs = 9000;
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

        public void Start(string dataDir, SimConfig config)
        {
            this.dataDir = dataDir;
            Directory.CreateDirectory(dataDir);
            logWriter = new StreamWriter(Path.Combine(dataDir,
                "eqp_sim_" + DateTime.Now.ToString("yyyyMMdd_HHmmss") + ".log"), true, Encoding.UTF8) { AutoFlush = true };

            MoveMs = config.GetInt("TIMING", "MOVE_MS", MoveMs);
            TravelMs = config.GetInt("TIMING", "TRAVEL_MS", TravelMs);
            WaitOutHoldMs = config.GetInt("TIMING", "WAITOUT_HOLD_MS", WaitOutHoldMs);
            OutRemoveMs = config.GetInt("TIMING", "OUT_REMOVE_MS", OutRemoveMs);
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
        ///   spec = "125"      → 트랙 번호 (ini [WCS_MIRROR] 기준, 스테이션 C/V#11~#15)
        ///        = "13:25"    → 설비번호:포트번호 (화면 표기 P25 와 동일. 통로 C/V 포함 전 설비)
        /// </summary>
        public bool RemovePallet(string spec, out string msg)
        {
            msg = "";
            if (string.IsNullOrEmpty(spec)) { msg = "트랙 번호를 입력하세요"; return false; }
            spec = spec.Trim().ToUpper().Replace("CV", "").Replace("C/V", "").Replace("#", "");

            ConveyorSim target = null;
            int order = 0;

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
                int track;
                if (!int.TryParse(spec, out track)) { msg = "숫자가 아님: " + spec; return false; }
                foreach (var cv in AllConveyors)
                {
                    int o = cv.OrderOfTrack(track);
                    if (o > 0) { target = cv; order = o; break; }
                }
                if (target == null)
                { msg = "트랙 " + track + " 을 가진 설비 없음 (통로 C/V 는 설비번호:포트번호 으로 지정)"; return false; }
            }

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
