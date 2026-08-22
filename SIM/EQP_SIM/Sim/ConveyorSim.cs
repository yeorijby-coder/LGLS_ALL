using System;
using System.Collections.Generic;
using EQP_SIM.Core;

namespace EQP_SIM.Sim
{
    /// <summary>
    /// 컨베이어 1대의 PLC 측 동작 (PPTX 반송 시나리오 슬라이드 8~15).
    ///  - 파렛트 이동(트래킹/Exist Shift), Wait-Out 보고, Load/Unload Complete 이벤트,
    ///    ECS Ack 수신 시 이벤트/Ack 클리어(PLC 담당), 방향 모드 준수
    ///  - 입고대 자동/수동 투입, 출고대 자동 반출(작업자 역할)
    /// </summary>
    public class ConveyorSim
    {
        public readonly ConveyorDef Def;
        private readonly PlcIo io;
        private readonly ScenarioEngine engine;

        // 포트 인덱스(1-base) → 파렛트
        public readonly Dictionary<int, SimPallet> Pallets = new Dictionary<int, SimPallet>();

        public bool AutoFeed;                          // 입고대 자동 투입
        public bool AutoFeedConfigured;                // INI [FEED] CVnn 설정값 (체크박스 토글 시에도 존중)
        public DateTime NextFeedAt = DateTime.MinValue;

        /// <summary>
        /// [WCS 미러] 대체 WCS(WCS_TASK_CV)의 D영역 트랙 상태 모델 시작 트랙 번호 (0=미사용).
        /// 트랙 t = base+(order-1), D워드 base = t*10 :
        ///   +0 LuggNum(바이너리), +6 ErrorCode, +7 Status(bit0=AUTO, bit1=입고대, bit2=출고대),
        ///   +8 Sensor(bit0=화물감지)  — WCS_TASK_CV CvThread.CvStatus 파싱 규격
        /// </summary>
        public int WcsTrackBase = 0;

        public ConveyorSim(ConveyorDef def, PlcIo io, ScenarioEngine engine)
        {
            Def = def;
            this.io = io;
            this.engine = engine;
        }

        public string Direction
        {
            get
            {
                if (!HasObs("DIRECTION_MODE")) return "0";
                string v = io.GetString(Def.Id, "DIRECTION_MODE");
                return v == "1" ? "1" : "0";
            }
        }

        private bool HasObs(string name)
        {
            ObservableDef d;
            return io.Map.TryGet(Def.Id, name, out d);
        }

        public void Initialize()
        {
            if (HasObs("OPERATION_MODE")) io.SetBool(Def.Id, "OPERATION_MODE", true);   // AUTO
            // [LGLS 2026-08-22] IN_READY_02 는 통로 C/V(#1~#10)에서는 "설비 준비완료" 게이트로 상시 ON 이어야 한다
            //   (IO_TASK 배차가 이 비트를 본다). 입고대/출고대를 가진 #11~#15 만 UpdateStationSignals 가
            //   지게차 규약대로 조건부로 관리하므로 여기서는 OFF 로 시작한다.
            if (HasObs("IN_READY_02")) io.SetBool(Def.Id, "IN_READY_02", (Def.No < 11) || Def.IngoPath == null);
            if (Def.No >= 11 && Def.OutgoPath != null && HasObs("WAIT_IN")) io.SetBool(Def.Id, "WAIT_IN", false);
            // [LGLS 2026-07-23] OP_Mode(슬라이드 18/19 의 8번째 비트, mBase+8) = 자동 운전 ON.
            //   CvThread 가 이 비트를 AUTO_MODE_RD 로 파싱해 DB에 기록 → IO_TASK 자동 지시 게이트의 실측값이 된다.
            if (HasObs("OPERATION_MODE")) io.SetBool(Def.Id, "OPERATION_MODE", true);
            if (HasObs("DIRECTION_MODE") && io.GetString(Def.Id, "DIRECTION_MODE").Length == 0)
                io.SetString(Def.Id, "DIRECTION_MODE", "0");
            SyncMemory();
        }

        /// <summary>내부 파렛트 상태를 PLC 메모리(Exist/트래킹/WaitOut)에 반영</summary>
        public void SyncMemory()
        {
            for (int i = 1; i <= Def.Ports.Length; i++)
            {
                SimPallet p;
                bool exist = Pallets.TryGetValue(i, out p);
                // [LGLS 2026-07-21] 재하감지 지연 중(SensorOnAt 미도래)이면 파렛트는 있어도 신호는 아직 OFF
                bool sensed = exist && (p.SensorOnAt == DateTime.MinValue || DateTime.Now >= p.SensorOnAt);
                SetExist(i, sensed);
                SetTracking(i, exist ? p.Id : "");
            }
            UpdateWaitOut();
            UpdateStationSignals();
        }

        /// <summary>
        /// [LGLS 2026-08-22] 입고대 / 출고대 신호 (지게차 규약)
        ///   · 입고대(IN_READY_02) : 화물감지가 InReadyDelayMs(3초) 유지되면 ON.
        ///       양방향 겸용대는 입고 모드일 때만.  구 ECS Conveyor.OnInputReady → InReady
        ///   · 출고대(WAIT_IN)     : 출고 화물이 출고대에 도착해 있으면 ON.
        ///       양방향 겸용대는 출고 모드일 때만.  구 ECS Conveyor.OnWaitIn → IsWaitIn
        ///   두 신호 모두 CvThread 가 STO_READY_RD / RET_READY_RD 로 DB 에 올린다.
        /// </summary>
        /// <summary>화물감지가 ON 된 시각을 파렛트에 각인 (Tick/SyncMemory 양쪽에서 호출)</summary>
        private void StampSensed()
        {
            DateTime now = DateTime.Now;
            foreach (var kv in Pallets)
            {
                SimPallet p = kv.Value;
                if (p.SensedAt != DateTime.MinValue) continue;
                if (p.SensorOnAt == DateTime.MinValue || now >= p.SensorOnAt) p.SensedAt = now;
            }
        }

        private bool m_bInReadyLogged;      // 입고대 신호 ON 로그 1회 억제
        private string m_strLastDir = "";   // 방향 전환 로그용

        /// <summary>
        /// [LGLS 2026-08-22] 양방향(겸용) 트랙의 방향 파생 신호.
        ///   출고 모드  : RTV 도착지 ON  · 출고HS ON   (RGV 가 이 트랙으로 온다)
        ///   입고 모드  : 둘 다 OFF
        /// 구 ECS 관측 정의(TB_OBSERVABLE)에는 이 두 신호의 M비트가 없어 PLC 메모리에 실을 자리가
        /// 없다 — 시뮬레이터 표시 상태로만 유지하고 화면(설비 상태열)에 내보낸다.
        /// </summary>
        public bool RtvArriveHs { get { return Def.RtvArrivePort > 0 && Direction == "1"; } }
        public bool RetHs       { get { return Def.RetHsPort     > 0 && Direction == "1"; } }

        private void UpdateStationSignals()
        {
            StampSensed();
            DateTime now = DateTime.Now;
            if (Def.HasDirection)
            {
                string d = Direction;
                if (d != m_strLastDir)
                {
                    m_strLastDir = d;
                    string txt = (d == "1") ? "출고" : "입고";
                    string sig = "";
                    if (Def.RtvArrivePort > 0) sig += "  P" + Def.RtvArrivePort + " RTV도착지=" + (RtvArriveHs ? "ON" : "OFF");
                    if (Def.RetHsPort     > 0) sig += "  P" + Def.RetHsPort     + " 출고HS=" + (RetHs ? "ON" : "OFF");
                    engine.Log(Def.Id + " 방향 " + txt + " 모드" + sig);
                }
            }
            bool bBi = (Def.IngoPath != null && Def.OutgoPath != null);   // 입출고 겸용(방향전환형)
            string dir = Direction;

            // ── 입고대 ──
            //   통로 C/V(#1~#10)와 입고대가 없는 설비는 건드리지 않는다(Initialize 의 상시 ON 유지).
            if (Def.No >= 11 && Def.IngoPath != null && HasObs("IN_READY_02"))
            {
                bool on = false;
                if (!bBi || dir != "1")
                {
                    int inIdx = Def.OrderOf(Def.IngoPath[0]);
                    SimPallet p;
                    if (Pallets.TryGetValue(inIdx, out p) && p.Dir == FlowDir.Ingo &&
                        p.SensedAt != DateTime.MinValue &&
                        now >= p.SensedAt.AddMilliseconds(engine.InReadyDelayMs))
                    {
                        on = true;
                        if (!m_bInReadyLogged) { m_bInReadyLogged = true;
                            engine.Log(Def.Id + " P" + Def.PortOfOrder(inIdx) + " 입고대 신호 ON (IN_READY_02, 화물감지 " + engine.InReadyDelayMs + "ms 유지)"); }
                    }
                }
                if (!on) m_bInReadyLogged = false;
                io.SetBool(Def.Id, "IN_READY_02", on);
            }

            // ── 출고대 ──
            if (Def.No >= 11 && Def.OutgoPath != null && HasObs("WAIT_IN"))
            {
                bool on = false;
                if (!bBi || dir == "1")
                {
                    int outIdx = Def.OrderOf(Def.OutgoPath[Def.OutgoPath.Length - 1]);
                    SimPallet p;
                    // [LGLS 2026-08-22] 규약: 화물과 데이터(JOB)가 모두 있어야 출고대 ON
                    if (Pallets.TryGetValue(outIdx, out p) && p.Dir == FlowDir.Outgo && !p.Discharged &&
                        p.SensedAt != DateTime.MinValue && !string.IsNullOrEmpty(p.Id))
                    {
                        on = true;
                        if (p.OutSignalAt == DateTime.MinValue)
                        {
                            p.OutSignalAt = now;                                        // 반출 3초 카운트 시작
                            engine.Log(Def.Id + " P" + Def.PortOfOrder(outIdx) + " 출고대 신호 ON (WAIT_IN, JOB " + p.Id + ")");
                        }
                    }
                }
                io.SetBool(Def.Id, "WAIT_IN", on);
            }
        }

        private string Obs(string baseName, int idx) { return baseName + "_0" + idx; }

        /// <summary>
        /// [LGLS 2026-08-21] 관측값 번호 = 구 ECS PortOrder 그대로 (WCS 모드 트랙정렬 보정 폐기).
        ///   종전에는 EQP_TASK 가 슬롯 s 를 트랙 FR+s(선형)로 해석해 C/V#15(PortOrder 3/1/2)가
        ///   어긋났고, 그것을 이 함수가 시뮬레이터 쪽을 비틀어 가렸다.
        ///   이제 EQP_TASK 가 XML trackOrder(131,132,130)로 비선형을 직접 해석하므로
        ///   시뮬레이터는 구 ECS 정의(관측 _NN = PortOrder N)를 그대로 따른다.
        ///   → TB_OBSERVABLE·PPT 슬라이드 59(+0=#31,+1=#32,+2=#30)와 일치.
        /// </summary>
        private int ObsNo(int order)
        {
            return order;
        }

        private void SetExist(int idx, bool on)
        {
            string name = Obs("PALLET_EXIST_FLAG", ObsNo(idx));
            if (HasObs(name)) io.SetBool(Def.Id, name, on);
        }

        private void SetTracking(int idx, string id)
        {
            string name = "PALLET_EXIST0" + ObsNo(idx);
            if (HasObs(name)) io.SetString(Def.Id, name, id ?? "");
        }

        private string GetTracking(int idx)
        {
            string name = "PALLET_EXIST0" + ObsNo(idx);
            return HasObs(name) ? io.GetString(Def.Id, name) : "";
        }

        private string staleFeedTracking = "";                  // [LGLS 2026-07-21] 빈 입고 포트에 선기록된 예약 트래킹
        private DateTime staleFeedSince = DateTime.MinValue;   // [LGLS 2026-07-24] 선기록 관측 시각(90초 자가 회복)
        private DateTime waitOutSince = DateTime.MinValue;      // WAIT_OUT ON 유지 시작 시각
        private DateTime waitOutSuppressUntil = DateTime.MinValue;

        /// <summary>
        /// WAIT_OUT 보고. ECS OnWaitOut 은 rising edge 1회만 발화하고 실패해도 재시도하지 않으므로
        /// (그 시점 R영역 셰도우가 미갱신이면 빈 트래킹으로 조기 return), 파렛트가 계속 대기 중이면
        /// 12초 ON / 3초 OFF 주기로 재발화시켜 재트리거를 보장한다.
        /// </summary>
        private void UpdateWaitOut()
        {
            if (!HasObs("WAIT_OUT")) return;
            DateTime now = DateTime.Now;
            int woPort = engine.World.GetWaitOutPort(Def, Direction);
            int woIdx = Def.OrderOf(woPort);
            SimPallet p;
            bool shouldOn = woIdx > 0 && Pallets.TryGetValue(woIdx, out p) && !string.IsNullOrEmpty(p.Id);

            if (!shouldOn)
            {
                io.SetBool(Def.Id, "WAIT_OUT", false);
                waitOutSince = DateTime.MinValue;
                return;
            }
            if (now < waitOutSuppressUntil)
            {
                io.SetBool(Def.Id, "WAIT_OUT", false);      // OFF 구간 (ECS 스캔이 하강을 인지하도록 유지)
                return;
            }
            if (waitOutSince == DateTime.MinValue) waitOutSince = now;
            if ((now - waitOutSince).TotalMilliseconds > 12000)
            {
                waitOutSuppressUntil = now.AddMilliseconds(3000);
                waitOutSince = DateTime.MinValue;
                io.SetBool(Def.Id, "WAIT_OUT", false);
                return;
            }
            io.SetBool(Def.Id, "WAIT_OUT", true);
        }

        /// <summary>이벤트 접미사: 첫 포트=_01, 그 외=_02 (PPTX 이벤트 배치)</summary>
        private string EvSuffix(int idx) { return idx == 1 ? "_01" : "_02"; }

        private void PulseEvent(string baseName, int idx)
        {
            string ev = baseName + EvSuffix(idx);
            string ack = baseName + "_ACK" + EvSuffix(idx);
            if (HasObs(ev)) engine.RaiseEvent(Def.Id, ev, HasObs(ack) ? ack : null);
        }

        // ------------------------------------------------------------------
        // 파렛트 조작
        // ------------------------------------------------------------------

        /// <summary>입고대 파렛트 투입 (작업자). 성공 시 true.</summary>
        public bool InjectPallet()
        {
            if (Def.IngoPath == null) return false;
            int inPort = Def.IngoPath[0];
            int idx = Def.OrderOf(inPort);
            if (Pallets.ContainsKey(idx)) return false;
            // [LGLS 2026-08-22] 지게차 규약 : 화물과 데이터(트래킹)가 모두 없어야 새 화물을 올린다.
            //   데이터만 남아 있는 동안(출고 반출 직후 등)에 올리면 남의 JOB 을 물고 들어간다.
            if (!string.IsNullOrEmpty(GetTracking(idx))) return false;
            // 겸용 트랙(C/V#2/#11)은 입고 모드일 때만. 출고 모드면 그 자리는 출고 화물이 나올 자리다.
            if (Def.HasDirection && Direction == "1") return false;
            // 입출고 겸용 C/V#11: 컨베이어가 완전히 빌 때만 투입 (출고 흐름과 충돌 방지)
            if (Def.No == 11 && Pallets.Count > 0) return false;

            var p = new SimPallet { Id = "", Dir = FlowDir.Ingo, ArrivedAt = DateTime.Now,
                                    MoveReadyAt = DateTime.Now.AddMilliseconds(engine.MoveMs) };
            Pallets[idx] = p;
            // [LGLS 2026-07-21] PLC 는 적재 후 2초 뒤에 재하감지를 올린다 — 신호 ON 을 지연(Tick 에서 처리).
            if (engine.InSensorDelayMs > 0)
            {
                p.SensorOnAt = DateTime.Now.AddMilliseconds(engine.InSensorDelayMs);
                engine.Log(Def.Id + " P" + inPort + " 파렛트 적재 (재하감지 " + engine.InSensorDelayMs + "ms 대기)");
            }
            else
            {
                SetExist(idx, true);
                PulseEvent("LOAD_COMPLETE", idx);             // 슬라이드8: 입고대 적재 → Load Complete
                engine.Log(Def.Id + " P" + inPort + " 파렛트 투입 (JOB 대기)");
            }
            UpdateWaitOut();
            return true;
        }

        /// <summary>차량(RGV/SC)이 포트에 파렛트를 내려놓음</summary>
        public void PlacePallet(int port, string palletId, FlowDir dir)
        {
            int idx = Def.OrderOf(port);
            if (idx <= 0) return;
            var p = new SimPallet { Id = palletId ?? "", Dir = dir, ArrivedAt = DateTime.Now,
                                    MoveReadyAt = DateTime.Now.AddMilliseconds(engine.MoveMs),
                                    HoldUntil = DateTime.Now.AddMilliseconds(engine.WaitOutHoldMs) };
            Pallets[idx] = p;
            SetExist(idx, true);
            SetTracking(idx, p.Id);
            PulseEvent("LOAD_COMPLETE", idx);                 // 차량 하역 → Load Complete
            UpdateWaitOut();
        }

        /// <summary>차량(RGV/SC)이 포트에서 파렛트를 집어감. 반환: 파렛트 (없으면 null)</summary>
        public SimPallet TakePallet(int port)
        {
            int idx = Def.OrderOf(port);
            SimPallet p;
            if (idx <= 0 || !Pallets.TryGetValue(idx, out p)) return null;
            Pallets.Remove(idx);
            SetExist(idx, false);
            SetTracking(idx, "");
            PulseEvent("UNLOAD_COMPLETE", idx);               // 차량 상차 → Unload Complete
            UpdateWaitOut();
            return p;
        }

        public SimPallet PalletAt(int port)
        {
            int idx = Def.OrderOf(port);
            SimPallet p;
            return idx > 0 && Pallets.TryGetValue(idx, out p) ? p : null;
        }

        /// <summary>[LGLS 2026-07-31] 해당 포트가 완전히 비었는지: 파렛트 없음 + 재하감지 OFF + 트래킹 비어 있음.
        ///   차량(RGV/S/C)이 상차 후 "출발지 화물·데이터 소멸"을 확인하고 출발하기 위한 판정.</summary>
        public bool IsPortClear(int port)
        {
            int idx = Def.OrderOf(port);
            if (idx <= 0) return true;
            if (Pallets.ContainsKey(idx)) return false;
            string name = Obs("PALLET_EXIST_FLAG", ObsNo(idx));
            if (HasObs(name) && io.GetBool(Def.Id, name)) return false;
            string trk = (GetTracking(idx) ?? "").Trim();
            return trk.Length == 0 || trk == "0" || trk == "0000";
        }

        // ------------------------------------------------------------------
        // 주기 처리
        // ------------------------------------------------------------------
        public void Tick(DateTime now)
        {
            // 0) [LGLS 2026-07-21] 입고대 재하감지 지연 ON: 적재(InjectPallet) 후 PLC 2초 규약이 지나면 신호를 올린다.
            foreach (var kv in Pallets)
            {
                if (kv.Value.SensorOnAt != DateTime.MinValue && now >= kv.Value.SensorOnAt)
                {
                    kv.Value.SensorOnAt = DateTime.MinValue;
                    // [LGLS 2026-07-21] 재하감지 ON 순간 이미 트래킹이 있으면 이 파렛트의 지시가 아니라
                    //   출고 도착 예약값이다(정상 스탬프는 재하감지를 본 뒤 기록됨) → 배정 금지 값으로 기록.
                    //   (WCS 발행 게이트의 센서 리드백 지연 창(±2초)과 상보적으로 하이재킹을 차단)
                    if (Def.IngoPath != null && kv.Key == Def.OrderOf(Def.IngoPath[0]))
                    {
                        string preTrk = GetTracking(kv.Key);
                        if (!string.IsNullOrEmpty(preTrk) && preTrk != staleFeedTracking)
                        {
                            staleFeedTracking = preTrk;
                            staleFeedSince = now;   // [LGLS 2026-07-24] 90초 자가 회복 기준 시각
                        }
                    }
                    SetExist(kv.Key, true);
                    PulseEvent("LOAD_COMPLETE", kv.Key);      // 슬라이드8: 입고대 적재 → Load Complete
                    engine.Log(Def.Id + " P" + Def.PortOfOrder(kv.Key) + " 재하감지 ON (JOB 대기)");
                    UpdateWaitOut();
                }
            }

            // 0.5) [LGLS 2026-07-21] 입고 포트가 비어 있는 동안 남아 있는 트래킹 값을 기억한다.
            //   (입출고 겸용 C/V#11 에서 출고 지시의 "도착 예약 트래킹"이 배출트랙에 선기록된 채
            //    새 입고 파렛트가 투입되면, 그 파렛트가 출고 작업번호를 오배정받아 픽업트랙을
            //    점유하고 출고 반출과 교착한다 — 빈 포트에서 이미 보이던 트래킹은 예약값이다)
            if (Def.IngoPath != null)
            {
                int feedIdx = Def.OrderOf(Def.IngoPath[0]);
                if (!Pallets.ContainsKey(feedIdx))
                {
                    string curTrk = GetTracking(feedIdx);
                    // [LGLS 2026-07-24] 빈 입고 포트의 현재 트래킹을 예약값으로 계속 갱신한다.
                    //   출고 반출번호가 배출되면 트래킹이 0으로 돌아가 예약값도 자연 해제되므로, 별도
                    //   90초 강제배정은 두지 않는다. (겸용 122 를 출고+입고 동시 최대부하하면 RTV 순차 대기로
                    //    출고 반출이 90초를 넘길 수 있는데, 강제배정이 그 출고번호를 입고로 오인 재현해
                    //    RTV 하차 트랙과 충돌·교착시켰다 — [CV#2 교착 TEST] 검출)
                    staleFeedTracking = curTrk;
                    staleFeedSince = now;
                }
            }

            // 1) ECS가 트래킹에 JOB NO를 기록(SetPallet)했으면 내부 파렛트에 반영
            foreach (var kv in Pallets)
            {
                if (string.IsNullOrEmpty(kv.Value.Id))
                {
                    string job = GetTracking(kv.Key);
                    if (!string.IsNullOrEmpty(job))
                    {
                        // [LGLS 2026-07-21] 입고 포트의 파렛트에는 "투입 전부터 있던" 예약 트래킹을 배정하지 않는다.
                        if (Def.IngoPath != null && kv.Key == Def.OrderOf(Def.IngoPath[0]) &&
                            job == staleFeedTracking) continue;
                        kv.Value.Id = job;
                        kv.Value.HoldUntil = now.AddMilliseconds(engine.WaitOutHoldMs);
                        engine.Log(Def.Id + " P" + Def.PortOfOrder(kv.Key) + " JOB 부여됨: " + job);
                    }
                }
            }

            // 1.2) [대체 WCS 지원] 출고 경로 포트의 트래킹에 JOB 이 기록됐는데 파렛트가 없으면
            //      S/C 가 하역한 것으로 보고 파렛트를 재현한다.
            //      (대체 체인에는 S/C·RGV PLC 게이트웨이가 없어 크레인 반송이 DB 시뮬레이션으로만
            //       진행되므로, 출고대까지의 물리 반송을 이 훅이 이어준다)
            if (engine.LegacyWcsHooks && Def.OutgoPath != null)
            {
                foreach (int port in Def.OutgoPath)
                {
                    int idx = Def.OrderOf(port);
                    // [LGLS] SC 라인 컨베이어(No<=10)의 입고 드롭 포트(IngoPath[0])는 1.3(입고 재현)에서 처리하므로 여기서 제외
                    if (Def.No <= 10 && Def.IngoPath != null && Def.IngoPath.Length > 0 && port == Def.IngoPath[0]) continue;
                    if (Pallets.ContainsKey(idx)) continue;
                    string job = GetTracking(idx);
                    if (!string.IsNullOrEmpty(job))
                    {
                        var np = new SimPallet { Id = job, Dir = FlowDir.Outgo, ArrivedAt = now,
                                                 MoveReadyAt = now.AddMilliseconds(engine.MoveMs),
                                                 HoldUntil = now.AddMilliseconds(engine.WaitOutHoldMs) };
                        Pallets[idx] = np;
                        SetExist(idx, true);
                        engine.Log(Def.Id + " P" + port + " 출고 파렛트 재현 (S/C 하역 시뮬레이션, JOB " + job + ")");
                        break;
                    }
                }
            }

            // 1.3) [대체 WCS 지원] 입고 RGV 드롭 재현: SC 라인 컨베이어(No<=10)의 입고 시작 포트
            //      (IngoPath[0], RGV 드롭 지점)의 트래킹에 JOB 이 기록됐는데 파렛트가 없으면
            //      RGV 가 내려놓은 것으로 보고 입고 파렛트를 재현한다. (IO_TASK 가 AutoRunRGV 드롭
            //      단계에서 cv_data LUGG_NO_OD+TRACKING_WRITE_YN='Y' 로 트래킹을 기록 → WCS_TASK_CV
            //      가 R영역에 기록 → 여기서 판독). 이후 MovePallets 이동 시 SetTracking 이 소스
            //      트래킹을 지우므로 무한 재생성되지 않는다. (출고 '재현' 훅의 입고 대응)
            if (engine.LegacyWcsHooks && Def.No <= 10 && Def.IngoPath != null && Def.IngoPath.Length > 0)
            {
                int inPort = Def.IngoPath[0];
                int idx = Def.OrderOf(inPort);
                if (!Pallets.ContainsKey(idx))
                {
                    string job = GetTracking(idx);
                    if (!string.IsNullOrEmpty(job))
                    {
                        Pallets[idx] = new SimPallet { Id = job, Dir = FlowDir.Ingo, ArrivedAt = now,
                                                       MoveReadyAt = now.AddMilliseconds(engine.MoveMs) };
                        SetExist(idx, true);
                        engine.Log(Def.Id + " P" + inPort + " 입고 파렛트 재현 (RGV 드롭 시뮬레이션, JOB " + job + ")");
                    }
                }
            }

            // 1.5) 입고 대기 파렛트에 뒤늦게 도착한 작업번호 부여 (입출고 겸용 C/V#11 계열)
            //      [LGLS 2026-08-22] 종전에는 여기서 "작업자 회수"를 함께 수행했다 —
            //        a) 출고 파렛트가 컨베이어에 있거나  b) 일정 시간 지시를 못 받으면
            //        파렛트를 지웠고, 자동투입이 다시 올려 화물이 생겼다 없어졌다를 반복했다.
            //      실제 설비는 그렇게 동작하지 않는다. 작업자가 올린 화물은 입고 작업을 받아
            //      크레인이 실어갈 때까지 그 자리에 그대로 있다(사람이 직접 치우지 않는 한).
            //      → 자동 회수를 폐기하고, 작업번호 부여만 남긴다.
            //        · 화물 생성 : 입고 모드일 때만 (아래 4) 자동 투입의 방향 게이트)
            //        · 화물 소멸 : 크레인 픽업(2.5 훅) 또는 이동으로 자리를 뜰 때만
            if (Def.No >= 11 && Def.IngoPath != null && Def.OutgoPath != null)
            {
                int inIdx = Def.OrderOf(Def.IngoPath[0]);
                SimPallet wp;
                if (Pallets.TryGetValue(inIdx, out wp) && wp.Dir == FlowDir.Ingo && string.IsNullOrEmpty(wp.Id))
                {
                    // [LGLS 2026-07-31] 회수 직전 트래킹 재확인(작업 0221 유실 사례).
                    //   상위가 지시를 낸 뒤 PLC 트래킹에 작업번호가 기록되기까지 미러 1주기의 지연이 있어,
                    //   그 사이에 타임아웃이 걸리면 '지시받은 파렛트'를 회수해 버리고 뒤늦게 도착한 트래킹은
                    //   빈 포트에 찍힌다 → 작업은 화물 없이 영구 정체. 제거 직전에 한 번 더 읽어
                    //   그 사이 기록된 작업번호가 있으면 회수 대신 JOB 부여로 전환한다(경쟁 창을 폴링 1주기로 축소).
                    string lateJob = (GetTracking(inIdx) ?? "").Trim();
                    bool bLateStamp = lateJob.Length > 0 && lateJob != "0" && lateJob != "0000" && lateJob != staleFeedTracking;
                    if (bLateStamp)
                    {
                        wp.Id = lateJob;
                        wp.HoldUntil = now.AddMilliseconds(engine.WaitOutHoldMs);
                        engine.Log(Def.Id + " P" + Def.IngoPath[0] + " JOB 부여됨: " + lateJob);
                        UpdateWaitOut();
                    }
                    // [LGLS 2026-08-22] 자동 회수 없음 — 입고 지시를 받아 크레인이 실어갈 때까지 대기한다.
                }
            }

            // 2) 파렛트 이동 (경로 순, 앞 포트부터 처리해 연쇄 이동 허용)
            MovePallets(now, FlowDir.Ingo, Def.IngoPath);
            MovePallets(now, FlowDir.Outgo, Def.OutgoPath);

            // 2.5) [대체 WCS 지원] 입고 크레인 픽업 시뮬레이션:
            //   입고 파렛트가 크레인 핸드오프 포트(WAIT_OUT)에서 지시(JOB)를 받고 체류시간
            //   (WaitOutHold)이 지나면 S/C·RGV 가 상차한 것으로 보고 파렛트를 제거한다.
            //   (대체 체인은 크레인 PLC 게이트웨이가 없어 크레인 레그가 IO_TASK DB 시뮬레이션으로만
            //    진행되므로, 컨베이어에서의 픽업(반출)을 이 훅이 이어준다 — 출고 재현/반출 훅의 입고 대응)
            if (engine.LegacyWcsHooks && Def.IngoPath != null)
            {
                int wo = engine.World.GetWaitOutPort(Def, Direction);
                int woIdx = Def.OrderOf(wo);
                SimPallet ip;
                if (woIdx > 0 && Pallets.TryGetValue(woIdx, out ip) && ip.Dir == FlowDir.Ingo &&
                    !string.IsNullOrEmpty(ip.Id) && ip.HoldUntil != DateTime.MinValue && now >= ip.HoldUntil)
                {
                    Pallets.Remove(woIdx);
                    SetExist(woIdx, false);
                    SetTracking(woIdx, "");
                    PulseEvent("UNLOAD_COMPLETE", woIdx);
                    engine.Log(Def.Id + " P" + wo + " 입고 파렛트 크레인 픽업 (S/C·RGV 상차 시뮬레이션, JOB " + ip.Id + ")");
                }
            }

            // 3) 출고대 자동 반출 (지게차 Unload — 슬라이드 11/13). 2단계 배출:
            //    ① 도착 +OutRemoveMs 에 화물(센서) 사라짐  ② 다시 +OutTrackClearMs 에 트래킹 데이터 제거
            //    ※ 입출고 컨베이어(C/V#11~15)의 출고 종점만 해당.
            //      라인 컨베이어(C/V#2~10)의 출고 파렛트는 RGV가 픽업하므로 반출 금지.
            // [LGLS 2026-08-22] 겸용 입출고대는 출고 모드일 때만 지게차 반출이 일어난다
            if (Def.OutgoPath != null && Def.No >= 11 &&
                !(Def.IngoPath != null && Direction != "1"))
            {
                int outPort = Def.OutgoPath[Def.OutgoPath.Length - 1];
                int outIdx = Def.OrderOf(outPort);
                SimPallet p;
                if (Pallets.TryGetValue(outIdx, out p) && p.Dir == FlowDir.Outgo &&
                    !string.IsNullOrEmpty(p.Id))
                {
                    if (!p.Discharged)
                    {
                        // ① 화물 배출 (지게차가 파렛트를 들어냄 → 센서 OFF)
                        // [LGLS 2026-08-22] 지게차 규약: 출고대 신호(WAIT_IN)가 ON 된 뒤 3초가 지나야 반출한다.
                        //   OutSignalAt 은 UpdateStationSignals 가 신호를 올릴 때 각인하므로,
                        //   아직 신호가 서지 않았으면(MinValue) 반출하지 않는다.
                        if (p.OutSignalAt != DateTime.MinValue &&
                            now >= p.OutSignalAt.AddMilliseconds(engine.OutRemoveMs))
                        {
                            p.Discharged = true;
                            p.DischargedAt = now;
                            SetExist(outIdx, false);
                            PulseEvent("UNLOAD_COMPLETE", outIdx);
                            engine.Log(Def.Id + " P" + outPort + " 출고 화물 배출 (지게차, JOB " + p.Id +
                                       ", 신호ON후 " + ((int)(now - p.OutSignalAt).TotalMilliseconds) + "ms)");
                            UpdateWaitOut();
                        }
                    }
                    else if (now >= p.DischargedAt.AddMilliseconds(engine.OutTrackClearMs))
                    {
                        // ② 트래킹 데이터 제거
                        Pallets.Remove(outIdx);
                        SetExist(outIdx, false);
                        SetTracking(outIdx, "");
                        engine.Log(Def.Id + " P" + outPort + " 출고 트래킹 제거 (JOB " + p.Id + ")");
                        engine.NotifyOutgoRemoved(p.Id);
                        UpdateWaitOut();
                    }
                }
            }

            // 4) 입고대 자동 투입
            //   [LGLS 2026-07-23] 겸용 입출고대(C/V#11): 빈 입고 포트에 출고 도착 예약 트래킹(staleFeedTracking)이
            //   보이는 동안은 재투입을 억제한다 — 출고가 이쪽으로 나오는 중에 파렛트를 다시 올리면
            //   "회수 ↔ 재투입" 이 반복되어 화물이 생겼다 없어졌다 하는 것으로 보인다(출고 완료로 예약이
            //   지워지면 재투입 재개).
            //   [LGLS 2026-08-21] 출고 모드(DIRECTION_MODE="1")인 입출고대에는 파렛트를 올리지 않는다.
            //     겸용 입출고대(C/V#11 등)는 방향 워드가 출고로 서 있는 동안 입고대 자리를 비워 둬야
            //     출고 화물이 그 자리로 나올 수 있다. 종전에는 방향과 무관하게 투입해
            //     출고 중에도 입고대에 화물이 올라왔다(=상위 상태보고도 함께 흔들렸다).
            if (AutoFeed && Def.IngoPath != null && now >= NextFeedAt
                && string.IsNullOrEmpty(staleFeedTracking) && Direction != "1")
            {
                if (InjectPallet()) NextFeedAt = now.AddMilliseconds(engine.FeedMs);
            }

            UpdateWaitOut();
            UpdateStationSignals();     // [LGLS 2026-08-22] 입고대/출고대 신호는 경과시간 조건이라 매 주기 재평가
            MirrorWcsTracks();
        }

        /// <summary>
        /// [WCS 미러] 포트 상태를 대체 WCS 의 D영역 트랙 모델로 복제.
        /// D 시작워드 = (MC_NO 뒤 2자리) × 10  — WCS CvThread.GetFirstAddress 의
        /// "MC_NO=그룹1자리+설비2자리" 규칙 (트랙 101 → D워드 10, 102 → 20)
        /// </summary>
        private void MirrorWcsTracks()
        {
            // [LGLS 2026-07-21] 물리 이관에서 비활성: EQP_TASK(V1.1 시나리오 맵)는 이 D 트랙테이블을 읽지 않고,
            //   D워드 10~328 기록이 Vehicle 관측값 영역(예: VEHICLE:11 SUBSYSTEM_STATUS=D0x100=256)과 충돌해
            //   크레인 상태 관측을 0으로 덮어쓴다 → 전면 미사용 처리.
            return;
#pragma warning disable 0162
            if (!engine.WcsSupport || WcsTrackBase <= 0) return;
            // [LGLS 2026-07-20] 트랙 = base + (포트 - 최소포트) — 포트 기준 매핑.
            //   (기존 order 기준은 C/V#15(포트 30/31/32, order 3/1/2)에서 130/131/132 와 어긋남)
            int minPort = int.MaxValue;
            for (int k = 0; k < Def.Ports.Length; k++) if (Def.Ports[k] < minPort) minPort = Def.Ports[k];
            for (int pi = 0; pi < Def.Ports.Length; pi++)
            {
                int order = Def.Orders[pi];
                int baseWord = ((WcsTrackBase % 100) + (Def.Ports[pi] - minPort)) * 10;
                SimPallet p;
                bool exist = Pallets.TryGetValue(order, out p);
                int lugg = 0;
                if (exist && !string.IsNullOrEmpty(p.Id)) int.TryParse(p.Id, out lugg);

                var mem = io.Memory;
                mem.SetWord('D', baseWord + 0, (ushort)lugg);                    // LuggNum
                mem.SetWord('D', baseWord + 6, 0);                               // ErrorCode
                mem.SetWord('D', baseWord + 7, (ushort)(0x01 | 0x02 | 0x04));    // AUTO|입고대|출고대
                mem.SetWord('D', baseWord + 8, (ushort)((exist && !p.Discharged) ? 1 : 0));   // Sensor(화물감지) — 배출 1단계면 OFF
            }
#pragma warning restore 0162
        }

        private void MovePallets(DateTime now, FlowDir dir, int[] path)
        {
            if (path == null) return;
            string direction = Direction;

            // ※ DIRECTION_MODE 워드로 이동을 막지 않는다.
            //   운영 ECS 는 잉고 RGV 하역 시 "1"을 쓰고 "0" 복귀를 하지 않으므로(PPT와 상이),
            //   워드 게이트를 걸면 교착된다. 파렛트 진행 방향은 적재 주체(Dir)로 이미 확정됨.

            int woPort = engine.World.GetWaitOutPort(Def, direction);

            // 경로 끝쪽 파렛트부터 이동 (막힘 해소 순서)
            for (int i = path.Length - 2; i >= 0; i--)
            {
                int curIdx = Def.OrderOf(path[i]);
                int nextIdx = Def.OrderOf(path[i + 1]);
                SimPallet p;
                if (!Pallets.TryGetValue(curIdx, out p) || p.Dir != dir) continue;
                if (Pallets.ContainsKey(nextIdx)) continue;
                if (now < p.MoveReadyAt) continue;
                if (string.IsNullOrEmpty(p.Id)) continue;                    // JOB 지정 전 이동 금지 (슬라이드8)
                // 차량 하역/Wait-Out 포트 체류: ECS 판독·배차 주기(수 초)가 현재 위치의
                // Exist/트래킹을 확인할 시간을 보장 (예: C/V#14 최종 레그는 P27 재석 조건으로 배차됨)
                if (now < p.HoldUntil) continue;

                // Shift: PLC가 Pallet ID/Exist 를 다음 영역으로 이동 (무결성 보장)
                Pallets.Remove(curIdx);
                Pallets[nextIdx] = p;
                SetExist(curIdx, false);
                SetTracking(curIdx, "");
                SetExist(nextIdx, true);
                SetTracking(nextIdx, p.Id);
                p.ArrivedAt = now;
                // [LGLS 2026-08-22] 위치가 바뀌면 화물감지·출고대 신호는 새 위치에서 새로 서는 것이다.
                //   초기화하지 않으면 이전 포트에서 각인된 시각이 남아 출고대 도착 즉시 반출돼 버린다.
                p.SensedAt = DateTime.MinValue;
                p.OutSignalAt = DateTime.MinValue;
                p.MoveReadyAt = now.AddMilliseconds(engine.MoveMs);
                if (path[i + 1] == woPort) p.HoldUntil = now.AddMilliseconds(engine.WaitOutHoldMs);
                engine.Log(Def.Id + " " + path[i] + "→" + path[i + 1] + " 이동 (JOB " + p.Id + ")");
            }
        }

        /// <summary>상태 저장용 텍스트 (portIdx,palletId,dir)</summary>
        public IEnumerable<string> SaveLines()
        {
            foreach (var kv in Pallets)
                yield return "CV," + Def.Id + "," + kv.Key + "," + kv.Value.Id + "," + (int)kv.Value.Dir;
        }

        public void RestorePallet(int portIdx, string id, FlowDir dir)
        {
            Pallets[portIdx] = new SimPallet { Id = id, Dir = dir, ArrivedAt = DateTime.Now,
                                               MoveReadyAt = DateTime.Now.AddMilliseconds(engine.MoveMs) };
        }
    }
}
