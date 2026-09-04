using System;
using System.Globalization;
using EQP_SIM.Core;

namespace EQP_SIM.Sim
{
    /// <summary>
    /// S/C(스태커크레인)/RGV 1대의 PLC 측 동작 (PPTX 슬라이드 16~17, VehicleScenario 참조).
    ///  - TRANSFER_REQUEST 스트로브 수신 → From/To/PALLET_ID 판독 → RUN
    ///  - 출발지 상차: Pallet Presence ON, LOAD_COMPLETE ↔ ACK 핸드셰이크
    ///  - 도착지 하차: UNLOAD_COMPLETE ↔ ACK, Transfer Complete Location, IDLE 복귀
    /// From/To 인코딩 (ECSDispatcher): 포트 = "00","00",포트2자리 / 랙 셀 = 통로(01/02),Bay,Level
    /// </summary>
    public class VehicleSim
    {
        // [LGLS 2026-08-23] 크레인(S/C)은 실제 설비처럼 구분 동작으로 나눠 수행한다.
        //   입고 : 입고HS 이동 -> 포크출 -> 호이스트UP -> 포크센터(화물감지 ON)
        //          -> 랙 이동 -> 포크출(화물감지 OFF) -> 호이스트DOWN -> 포크센터(완료)
        //   출고 : 랙 이동 -> 포크출 -> 호이스트UP -> 포크센터(화물감지 ON)
        //          -> 출고HS 이동 -> 포크출(화물감지 OFF) -> 호이스트DOWN -> 포크센터(완료)
        //   RGV 는 종전 동작(도착 즉시 상/하차)을 그대로 쓴다.
        private enum VState
        {
            Idle, ToSource, AtSource, ToDest, AtDest, Error,
            SrcForkOut, SrcHoistUp, SrcForkCenter,
            DstForkOut, DstHoistDown, DstForkCenter
        }

        public readonly VehicleDef Def;
        private readonly PlcIo io;
        private readonly ScenarioEngine engine;

        private VState state = VState.Idle;
        private DateTime stateUntil = DateTime.MinValue;
        private string from01, from02, from03, to01, to02, to03, palletId;
        private SimPallet carrying;

        // [LGLS] 이중입고/공출고 에러 주입: 이 작업에서 발생시킬 에러코드 (0=정상, 54=이중입고, 58=공출고)
        private int pendingErrCode = 0;

        // [LGLS 2026-07-22] 주행 보간: 출발지→도착지로 순간이동하지 않고 주행 축 좌표를 단계적으로
        //   LOCATION 에 반영해 화면(RTV 레일/SC Bay)에서 이동이 보이게 한다.
        //   축: RTV = 레일 슬롯(Client <Position> plc 순서와 동일), SC = Bay.
        private double curAxis = 0;
        private double travelFromAxis, travelToAxis;
        private DateTime travelStart = DateTime.MinValue;
        private int lastEmitSlot = int.MinValue;
        private static readonly System.Collections.Generic.Dictionary<int, int> RtvPortAxis =
            new System.Collections.Generic.Dictionary<int, int>()
            { { 1, 1 }, { 3, 3 }, { 5, 5 }, { 7, 6 }, { 9, 8 }, { 11, 9 }, { 13, 11 }, { 15, 12 },
              { 17, 13 }, { 19, 14 }, { 21, 15 }, { 23, 2 }, { 24, 2 }, { 25, 4 }, { 26, 4 },
              { 27, 7 }, { 29, 7 }, { 30, 10 }, { 31, 10 } };
        private static readonly System.Collections.Generic.Dictionary<int, string> RtvAxisPort =
            new System.Collections.Generic.Dictionary<int, string>()
            { { 1, "01" }, { 2, "23" }, { 3, "03" }, { 4, "25" }, { 5, "05" }, { 6, "07" }, { 7, "27" },
              { 8, "09" }, { 9, "11" }, { 10, "30" }, { 11, "13" }, { 12, "15" }, { 13, "17" },
              { 14, "19" }, { 15, "21" } };

        private double AxisOf(string w1, string w2, string w3)
        {
            if (IsPort(w1, w2))
            {
                if (Def.IsRgv)
                {
                    int ax;
                    return RtvPortAxis.TryGetValue(ParseInt(w3), out ax) ? ax : curAxis;
                }
                return 0;   // SC 의 포트(라인 핸드오프)는 Bay 0(홈) 측
            }
            return ParseInt(w2);   // 랙 셀: Bay
        }

        private void BeginTravel(DateTime now, double toAxis)
        {
            travelFromAxis = curAxis;
            travelToAxis = toAxis;
            travelStart = now;
            lastEmitSlot = int.MinValue;
        }

        /// <summary>주행 중 현재 축 위치를 LOCATION 관측값으로 단계 반영</summary>
        private void EmitTravel(DateTime now)
        {
            double dur = engine.TravelMs <= 0 ? 1 : engine.TravelMs;
            double p = (now - travelStart).TotalMilliseconds / dur;
            if (p < 0) p = 0; if (p > 1) p = 1;
            double ax = travelFromAxis + (travelToAxis - travelFromAxis) * p;
            curAxis = ax;
            int slot = (int)Math.Round(ax);
            if (slot == lastEmitSlot) return;
            lastEmitSlot = slot;
            if (Def.IsRgv)
            {
                string port;
                if (!RtvAxisPort.TryGetValue(slot, out port)) return;
                io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", "00");
                io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", "00");
                io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", port);
            }
            else if (slot <= 0)
            {
                io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", "00");
                io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", "00");
                io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", "00");
            }
            else
            {
                io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", "01");
                io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", slot.ToString("00"));
                io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", "01");
            }
        }

        public string StatusText { get; private set; }

        public VehicleSim(VehicleDef def, PlcIo io, ScenarioEngine engine)
        {
            Def = def;
            this.io = io;
            this.engine = engine;
            StatusText = "IDLE";
        }

        public void Initialize()
        {
            io.SetShort(Def.Id, "SUBSYSTEM_STATUS", 1);       // IDLE=1
            io.SetShort(Def.Id, "OPERATION_MODE", 1);
            io.SetString(Def.Id, "PALLET_ON_VEHICLE", "");
            io.SetBool(Def.Id, "PALLET_EXIST_FLAG", false);
            if (!Def.IsRgv) io.SetShort(Def.Id, "ERR_CODE_RD", 0);   // [LGLS] 크레인 에러코드 초기화(정상)
            // [LGLS] 상태머신·화물·에러 완전 초기화 — 이중입고/공출고 Error 상태의 화물감지·작업 잔류 방지
            state = VState.Idle;
            carrying = null;
            pendingErrCode = 0;
            stateUntil = DateTime.MinValue;
            srcClearAt = DateTime.MinValue;   // [LGLS 2026-07-31] 출발지 클리어 대기 상태도 초기화
        }

        private bool IsPort(string w1, string w2) { return (w1 == "" || w1 == "00") && (w2 == "" || w2 == "00"); }

        // [LGLS 2026-07-31] 상차 후 출발지 클리어 확인 시각(dwell 기준). 랙 셀 출고는 해당 없음(항상 통과).
        private DateTime srcClearAt = DateTime.MinValue;

        /// <summary>출발지(라인/스테이션 포트)의 화물·트래킹이 실제로 사라졌는지</summary>
        /// <summary>
        /// [LGLS 2026-08-22] 출발지 클리어 판정.
        ///   종전에는 "그 포트가 비었는지"(IsPortClear)를 봤다. 그런데 상차 직후 컨베이어가 다음 화물을
        ///   같은 포트로 밀어 넣으면 그 조건은 영영 성립하지 않아 RGV 가 AtSource 에서 무한 대기했다
        ///   (실측: 상차 109ms 뒤 후속 화물 도착 → 11시간 교착, 작업 9건 적체).
        ///   실제 PLC 는 자기가 실은 화물이 빠졌는지만 확인하면 되므로,
        ///   "내가 집은 파렛트가 그 포트에 더 이상 없는지"로 판정한다.
        /// </summary>
        private DateTime srcWaitFrom = DateTime.MinValue;   // 출발지 클리어 대기 시작 시각(타임아웃용)
        private DateTime srcCargoWaitFrom = DateTime.MinValue;   // [LGLS 2026-09-04] 출발지 화물 대기 시작 시각(타임아웃용)

        private bool SourceCleared()
        {
            if (!IsPort(from01, from02)) return true;      // 랙 셀에서 꺼낸 출고는 컨베이어 클리어 대상 없음
            int port = ParseInt(from03);
            var cv = engine.World.FindByPort(port);
            if (cv == null) return true;
            var sim = engine.Conveyor(cv.Id);
            if (sim == null) return true;
            if (carrying == null) return sim.IsPortClear(port);
            return !sim.HasPalletObject(port, carrying);
        }

        public void Tick(DateTime now)
        {
            // [LGLS 2026-09-02] 상태워드 하트비트 - 잔재 메모리/외부 덮어쓰기(구 트랙테이블 D 충돌 등)가 있어도
            //   매 틱 현재 상태를 다시 기록해 항상 참값을 유지한다 (IDLE=1, 동작중=2).
            io.SetShort(Def.Id, "SUBSYSTEM_STATUS", (ushort)(state == VState.Idle ? 1 : 2));

            switch (state)
            {
                case VState.Idle:
                    if (io.GetBool(Def.Id, "TRANSFER_REQUEST"))
                    {
                        from01 = io.GetString(Def.Id, "FROM_01");
                        from02 = io.GetString(Def.Id, "FROM_02");
                        from03 = io.GetString(Def.Id, "FROM_03");
                        to01 = io.GetString(Def.Id, "TO_01");
                        to02 = io.GetString(Def.Id, "TO_02");
                        to03 = io.GetString(Def.Id, "TO_03");
                        palletId = io.GetString(Def.Id, "PALLET_ID");

                        // [LGLS] 에러 주입 판정 (S/C 랙작업만): 입고(TO=랙셀)→이중입고, 출고(FROM=랙셀)→공출고. 다음 최초 1회 소비.
                        pendingErrCode = 0;
                        if (!Def.IsRgv)
                        {
                            bool isIngo  = !IsPort(to01, to02);     // 목적지가 랙 셀 = 입고
                            bool isOutgo = !IsPort(from01, from02); // 출발지가 랙 셀 = 출고
                            if (engine.InjectDoubleStorage && isIngo)
                            {
                                pendingErrCode = 54; engine.InjectDoubleStorage = false;
                                engine.Log(Def.Id + " ★이중입고 에러 예약 (다음 입고 하차 시 발생)");
                            }
                            else if (engine.InjectEmptyRetrieval && isOutgo)
                            {
                                pendingErrCode = 58; engine.InjectEmptyRetrieval = false;
                                engine.Log(Def.Id + " ★공출고 에러 예약 (출발셀 픽업 시 발생)");
                            }
                        }

                        // [LGLS 2026-08-22] 반송지시를 받으면 그 작업번호를 즉시 싣는다 — 화물감지는 아직 OFF.
                        //   구 ECS 규약 : "실으러 가는 차는 데이터만 있고 화물은 없다".
                        //   종전에는 상차하는 순간에야 작업번호를 썼다. WCS 는 작업번호(R영역 문자열)와
                        //   화물감지(M영역 비트)를 서로 다른 READ 로 읽으므로, 그 좁은 창에서 READ 가
                        //   어긋나거나 실패하면 "화물은 실렸는데 작업번호가 없는" 상태로 화면에 남았다.
                        io.SetString(Def.Id, "PALLET_ON_VEHICLE", palletId ?? "");
                        io.SetBool(Def.Id, "TRANSFER_REQUEST", false);     // Cmd Start 시 PLC가 리셋 (슬라이드6)
                        io.SetShort(Def.Id, "SUBSYSTEM_STATUS", 2);        // RUN=2
                        state = VState.ToSource;
                        stateUntil = now.AddMilliseconds(engine.TravelMs);
                        BeginTravel(now, AxisOf(from01, from02, from03));  // [LGLS 2026-07-22] 주행 보간 시작
                        StatusText = "RUN → 출발지 " + FromText();
                        engine.Log(Def.Id + " 반송지시 수신: " + FromText() + " → " + ToText() + " (JOB " + palletId + ")");
                    }
                    break;

                case VState.ToSource:
                    EmitTravel(now);                                       // [LGLS 2026-07-22] 주행 위치 단계 반영
                    if (now < stateUntil) break;
                    if (pendingErrCode == 58)   // 공출고: 출발 랙셀이 비어 있음 → 픽업 실패, 화물없음(SENSOR_FK=0)
                    {
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", from01);
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", from02);
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", from03);
                        io.SetBool(Def.Id, "PALLET_EXIST_FLAG", false);
                        io.SetShort(Def.Id, "ERR_CODE_RD", 58);
                        engine.Log(Def.Id + " ★공출고 발생: " + FromText() + " 재고없음 (ERR_CODE_RD=58)");
                        StatusText = "★공출고 에러 (ERR 58) — 재지정 대기";
                        state = VState.Error;
                        break;
                    }
                    // [LGLS 2026-08-23] 크레인은 도착 즉시 싣지 않고 포크/호이스트 3단계를 밟는다.
                    if (!Def.IsRgv)
                    {
                        if (!SourceHasCargo())
                        {
                            stateUntil = now.AddMilliseconds(500);
                            break;
                        }
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", from01);
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", from02);
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", from03);
                        state = VState.SrcForkOut;
                        stateUntil = now.AddMilliseconds(engine.ForkStepMs);
                        StatusText = "출발지 " + FromText() + " 포크 출";
                        engine.Log(Def.Id + " [2/8] " + FromText() + " 포크 출 (" + engine.ForkStepMs + "ms)");
                        break;
                    }

                    if (TryLoadAtSource(now))
                    {
                        srcCargoWaitFrom = DateTime.MinValue;
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", from01);   // [LGLS] 출발지 도착 위치 확정
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", from02);
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", from03);
                        // [LGLS 2026-08-22] 데이터(ID) 를 먼저 싣고 감지 플래그를 나중에 세운다.
                        //   WCS 는 PALLET_ON_VEHICLE(문자열)과 PALLET_EXIST_FLAG(비트)를 각각 따로 읽으므로,
                        //   감지를 먼저 세우면 그 사이에 읽힌 주기에 "화물은 있는데 작업번호가 없는" 상태로 보인다.
                        io.SetString(Def.Id, "PALLET_ON_VEHICLE", carrying.Id);   // 재라벨 반영(지시 수신 때 이미 실림)
                        io.SetBool(Def.Id, "PALLET_EXIST_FLAG", true);
                        engine.RaiseEvent(Def.Id, "LOAD_COMPLETE", "LOAD_COMPLETE_ACK");
                        state = VState.AtSource;
                        StatusText = "상차 완료 (" + carrying.Id + ")";
                    }
                    else
                    {
                        // [LGLS 2026-09-04] 출발지에 화물이 없으면 무한 대기하지 않는다(실측 : 빈 트랙에서 RTV 수동지시 → 영영 정지).
                        //   일정 시간 기다린 뒤 지시를 포기하고 IDLE 로 돌아가며, 화물 없음 상태를 남긴다.
                        if (srcCargoWaitFrom == DateTime.MinValue) srcCargoWaitFrom = now;
                        if ((now - srcCargoWaitFrom).TotalMilliseconds >= engine.SrcCargoTimeoutMs)
                        {
                            engine.Log(Def.Id + " ※ 출발지 " + FromText() + " 에 화물 없음 " + engine.SrcCargoTimeoutMs + "ms - 지시 포기, IDLE 복귀 (JOB " + palletId + ")");
                            srcCargoWaitFrom = DateTime.MinValue;
                            io.SetString(Def.Id, "PALLET_ON_VEHICLE", "");
                            io.SetBool(Def.Id, "PALLET_EXIST_FLAG", false);
                            io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", from01);
                            io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", from02);
                            io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", from03);
                            io.SetShort(Def.Id, "SUBSYSTEM_STATUS", 1);
                            engine.RaiseEvent(Def.Id, "TRANSFER_ACK", null, 1500);   // 반송 종료 보고(화물 없이 종료)
                            carrying = null;
                            state = VState.Idle;
                            StatusText = "IDLE (출발지 화물 없음으로 지시 포기)";
                            break;
                        }
                        stateUntil = now.AddMilliseconds(500);            // 파렛트 대기 (재시도)
                    }
                    break;

                case VState.SrcForkOut:
                    if (now < stateUntil) break;
                    state = VState.SrcHoistUp;
                    stateUntil = now.AddMilliseconds(engine.ForkStepMs);
                    StatusText = "출발지 " + FromText() + " 호이스트 UP";
                    engine.Log(Def.Id + " [3/8] " + FromText() + " 호이스트 UP (" + engine.ForkStepMs + "ms)");
                    break;

                case VState.SrcHoistUp:
                    if (now < stateUntil) break;
                    state = VState.SrcForkCenter;
                    stateUntil = now.AddMilliseconds(engine.ForkStepMs);
                    StatusText = "출발지 " + FromText() + " 포크 센터";
                    engine.Log(Def.Id + " [4/8] " + FromText() + " 포크 센터 (" + engine.ForkStepMs + "ms, 끝나면 화물감지 ON)");
                    break;

                case VState.SrcForkCenter:
                    if (now < stateUntil) break;
                    // 포크가 센터로 돌아오면 화물이 크레인 위에 올라온 것으로 본다 -> 이때 화물감지 ON
                    if (!TryLoadAtSource(now))
                    {
                        stateUntil = now.AddMilliseconds(500);
                        break;
                    }
                    io.SetString(Def.Id, "PALLET_ON_VEHICLE", carrying.Id);
                    io.SetBool(Def.Id, "PALLET_EXIST_FLAG", true);
                    engine.RaiseEvent(Def.Id, "LOAD_COMPLETE", "LOAD_COMPLETE_ACK");
                    state = VState.AtSource;
                    StatusText = "상차 완료 (" + carrying.Id + ")";
                    engine.Log(Def.Id + " [4/8 완료] 화물감지 ON - 상차 완료 (JOB " + carrying.Id + ")");
                    break;

                case VState.DstForkOut:
                    if (now < stateUntil) break;
                    // [LGLS 2026-09-01] ★점유된 포트에는 내리지 않는다★ (실측 : 크레인이 점유된
                    //   P4 에 출고 화물을 내려 그 자리의 입고 화물 9035 가 소멸했다 - 파렛트 겹침).
                    //   실물도 자리가 빌 때까지 하역하지 못한다. 자리가 날 때까지 이 단계에서 대기.
                    if (IsDestPortOccupied())
                    {
                        stateUntil = now.AddMilliseconds(500);
                        StatusText = "도착지 점유 - 하역 대기";
                        break;
                    }
                    // 포크가 나가 화물을 놓는다 -> 이때 화물감지 OFF (하역도 여기서)
                    UnloadAtDest();
                    io.SetBool(Def.Id, "PALLET_EXIST_FLAG", false);
                    state = VState.DstHoistDown;
                    stateUntil = now.AddMilliseconds(engine.ForkStepMs);
                    StatusText = "도착지 " + ToText() + " 호이스트 DOWN";
                    engine.Log(Def.Id + " [6/8 완료] 화물감지 OFF -> [7/8] 호이스트 DOWN (" + engine.ForkStepMs + "ms)");
                    break;

                case VState.DstHoistDown:
                    if (now < stateUntil) break;
                    state = VState.DstForkCenter;
                    stateUntil = now.AddMilliseconds(engine.ForkStepMs);
                    StatusText = "도착지 " + ToText() + " 포크 센터";
                    engine.Log(Def.Id + " [8/8] " + ToText() + " 포크 센터 (" + engine.ForkStepMs + "ms, 끝나면 작업 완료)");
                    break;

                case VState.DstForkCenter:
                    if (now < stateUntil) break;
                    io.SetString(Def.Id, "TRANSFER_COMPLETE_LOCATION_01", to01);
                    io.SetString(Def.Id, "TRANSFER_COMPLETE_LOCATION_02", to02);
                    io.SetString(Def.Id, "TRANSFER_COMPLETE_LOCATION_03", to03);
                    io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", to01);
                    io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", to02);
                    io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", to03);
                    engine.RaiseEvent(Def.Id, "UNLOAD_COMPLETE", "UNLOAD_COMPLETE_ACK");
                    state = VState.AtDest;
                    StatusText = "하차 완료";
                    engine.Log(Def.Id + " [8/8 완료] 하차 완료 - 작업 완료");
                    break;

                case VState.AtSource:
                    if (engine.IsEventCleared(Def.Id, "LOAD_COMPLETE"))
                    {
                        // [LGLS 2026-07-31] 출발지 클리어 선행 보장(사용자 요구):
                        //   상차했는데 출발지(예: 트랙 131)에 화물·트래킹이 남은 채로 도착지(111)에 먼저
                        //   화물·데이터가 생기는 순서 역전을 막는다. 상차 → 출발지 화물·데이터 소멸 확인
                        //   → SrcClearDwellMs 유지(상위 판독 주기 확보) → 그 다음에 출발/하역.
                        if (!SourceCleared())
                        {
                            // [LGLS 2026-08-22] 안전장치: 어떤 이유로든 클리어가 오지 않아도 영구 교착되지 않게
                            //   타임아웃 후에는 경고를 남기고 진행한다.
                            if (srcWaitFrom == DateTime.MinValue) srcWaitFrom = now;
                            if ((now - srcWaitFrom).TotalMilliseconds < engine.SrcClearTimeoutMs)
                            {
                                StatusText = "상차 완료 — 출발지 클리어 대기";
                                break;
                            }
                            engine.Log(Def.Id + " ※ 출발지 클리어 대기 " + engine.SrcClearTimeoutMs +
                                       "ms 초과 — 교착 방지를 위해 진행 (JOB " + palletId + ")");
                        }
                        srcWaitFrom = DateTime.MinValue;
                        if (srcClearAt == DateTime.MinValue) srcClearAt = now;
                        if ((now - srcClearAt).TotalMilliseconds < engine.SrcClearDwellMs)
                        {
                            StatusText = "출발지 클리어 확인 — 판독 대기";
                            break;
                        }
                        srcClearAt = DateTime.MinValue;
                        state = VState.ToDest;
                        stateUntil = now.AddMilliseconds(engine.TravelMs);
                        BeginTravel(now, AxisOf(to01, to02, to03));        // [LGLS 2026-07-22] 주행 보간 시작
                        StatusText = "RUN → 도착지 " + ToText();
                    }
                    break;

                case VState.ToDest:
                    EmitTravel(now);                                       // [LGLS 2026-07-22] 주행 위치 단계 반영
                    if (now < stateUntil) break;
                    if (pendingErrCode == 54)   // 이중입고: 목적 랙셀에 이미 화물 → 저장 실패, 화물 실은채 유지(SENSOR_FK=1)
                    {
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", to01);
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", to02);
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", to03);
                        io.SetShort(Def.Id, "ERR_CODE_RD", 54);
                        engine.Log(Def.Id + " ★이중입고 발생: " + ToText() + " 이미 점유 (ERR_CODE_RD=54)");
                        StatusText = "★이중입고 에러 (ERR 54) — 재지정 대기";
                        state = VState.Error;
                        break;
                    }
                    // [LGLS 2026-08-23] 크레인은 도착 즉시 내리지 않고 포크/호이스트 3단계를 밟는다.
                    if (!Def.IsRgv)
                    {
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", to01);
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", to02);
                        io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", to03);
                        state = VState.DstForkOut;
                        stateUntil = now.AddMilliseconds(engine.ForkStepMs);
                        StatusText = "도착지 " + ToText() + " 포크 출";
                        engine.Log(Def.Id + " [6/8] " + ToText() + " 포크 출 (" + engine.ForkStepMs + "ms, 끝나면 화물감지 OFF)");
                        break;
                    }
                    UnloadAtDest();
                    // [LGLS 2026-08-22] 하차는 화물감지만 내린다. 데이터(작업번호)는 반송 완료까지 유지 —
                    //   하역 규약(RvSeq 4단계)의 "차 화물 사라짐 → 라인에 화물 → 차 데이터 사라짐" 순서와 같다.
                    io.SetBool(Def.Id, "PALLET_EXIST_FLAG", false);
                    io.SetString(Def.Id, "TRANSFER_COMPLETE_LOCATION_01", to01);
                    io.SetString(Def.Id, "TRANSFER_COMPLETE_LOCATION_02", to02);
                    io.SetString(Def.Id, "TRANSFER_COMPLETE_LOCATION_03", to03);
                    io.SetString(Def.Id, "SUBSYSTEM_LOCATION_01", to01);
                    io.SetString(Def.Id, "SUBSYSTEM_LOCATION_02", to02);
                    io.SetString(Def.Id, "SUBSYSTEM_LOCATION_03", to03);
                    engine.RaiseEvent(Def.Id, "UNLOAD_COMPLETE", "UNLOAD_COMPLETE_ACK");
                    state = VState.AtDest;
                    StatusText = "하차 완료";
                    break;

                case VState.AtDest:
                    if (engine.IsEventCleared(Def.Id, "UNLOAD_COMPLETE"))
                    {
                        io.SetShort(Def.Id, "SUBSYSTEM_STATUS", 1);        // IDLE=1
                        engine.RaiseEvent(Def.Id, "TRANSFER_ACK", null, 1500);  // Transfer Complete 보고 (Ack 관측값 없음)
                        engine.Log(Def.Id + " 반송 완료 (JOB " + palletId + ")");
                        io.SetString(Def.Id, "PALLET_ON_VEHICLE", "");   // [LGLS 2026-08-22] 데이터는 여기서 내린다
                        carrying = null;
                        state = VState.Idle;
                        StatusText = "IDLE";
                    }
                    break;

                case VState.Error:
                    // [LGLS] 이중입고/공출고 에러 정지. 재지정 명령(새 TRANSFER_REQUEST) 수신 시 에러 해제 후 재작업.
                    if (io.GetBool(Def.Id, "TRANSFER_REQUEST"))
                    {
                        if (!Def.IsRgv) io.SetShort(Def.Id, "ERR_CODE_RD", 0);
                        pendingErrCode = 0;
                        io.SetBool(Def.Id, "TRANSFER_REQUEST", false);      // PLC 가 스트로브 리셋
                        io.SetShort(Def.Id, "SUBSYSTEM_STATUS", 2);         // RUN

                        // [LGLS 2026-08-30] 재지정 규약 = "이미 든 화물을 새 목적지로".
                        //   종전에는 carrying 을 버리고 IDLE 로 돌아가, 화물이 사라진 채 처음부터 다시
                        //   하는 모양이 됐다(재지정 작업이 끝까지 가지 못한 원인).
                        //   이제 적재 상태를 유지한 채 새 To 로 직행한다. 빈 차(공출고)면 종전처럼
                        //   출발지부터 정상 수행한다.
                        to01 = io.GetString(Def.Id, "TO_01");
                        to02 = io.GetString(Def.Id, "TO_02");
                        to03 = io.GetString(Def.Id, "TO_03");
                        from01 = io.GetString(Def.Id, "FROM_01");
                        from02 = io.GetString(Def.Id, "FROM_02");
                        from03 = io.GetString(Def.Id, "FROM_03");
                        palletId = io.GetString(Def.Id, "PALLET_ID");

                        if (carrying != null)
                        {
                            io.SetString(Def.Id, "PALLET_ON_VEHICLE", palletId ?? "");
                            state = VState.ToDest;
                            stateUntil = now.AddMilliseconds(engine.TravelMs);
                            BeginTravel(now, AxisOf(to01, to02, to03));
                            StatusText = "재지정 수신 → 적재 유지, 새 목적지 " + ToText();
                            engine.Log(Def.Id + " 재지정 수신 → 에러 해제, 든 화물을 새 목적지 " + ToText() + " 로 이송 (JOB " + palletId + ")");
                        }
                        else
                        {
                            io.SetString(Def.Id, "PALLET_ON_VEHICLE", palletId ?? "");
                            io.SetBool(Def.Id, "PALLET_EXIST_FLAG", false);
                            state = VState.ToSource;
                            stateUntil = now.AddMilliseconds(engine.TravelMs);
                            BeginTravel(now, AxisOf(from01, from02, from03));
                            StatusText = "재지정 수신 → 새 출발지 " + FromText();
                            engine.Log(Def.Id + " 재지정 수신 → 에러 해제, 새 출발지 " + FromText() + " 에서 재수행 (JOB " + palletId + ")");
                        }
                    }
                    break;
            }
        }

        private bool TryLoadAtSource(DateTime now)
        {
            if (IsPort(from01, from02))
            {
                int port = ParseInt(from03);
                var cv = engine.World.FindByPort(port);
                if (cv == null) { engine.Log(Def.Id + " 알 수 없는 출발 포트 " + from03); return false; }
                var sim = engine.Conveyor(cv.Id);
                var p = sim.TakePallet(port);
                if (p == null) return false;                              // 아직 도착 전 — 대기
                carrying = p;
                // 반송지시의 PALLET_ID(현재 반송 JOB)가 트래킹 기준 — 실PLC와 동일하게 재라벨
                if (!string.IsNullOrEmpty(palletId) && carrying.Id != palletId)
                {
                    engine.Log(Def.Id + " JOB 재부여: " + carrying.Id + " → " + palletId);
                    carrying.Id = palletId;
                }
                return true;
            }
            else
            {
                // 랙 셀 출고: 재고에서 꺼내되, 트래킹에는 반송지시의 PALLET_ID(새 JOB)를 사용
                string cell = RackCell(from01, from02, from03);
                string stored = engine.TakeFromRack(cell);
                string id = !string.IsNullOrEmpty(palletId) ? palletId : stored;
                carrying = new SimPallet { Id = id ?? "", Dir = FlowDir.Outgo };
                engine.Log(Def.Id + " 랙 " + cell + " 상차 (JOB " + carrying.Id +
                           (string.IsNullOrEmpty(stored) || stored == id ? "" : ", 보관 JOB " + stored) + ")");
                return true;
            }
        }

        /// <summary>
        /// [LGLS 2026-08-23] 출발지에 실을 화물이 있는지. 포트면 파렛트 유무, 랙 셀이면 항상 가능.
        ///   크레인이 포크를 내밀기 전에 확인한다(없으면 그 자리에서 대기).
        /// </summary>
        private bool SourceHasCargo()
        {
            if (!IsPort(from01, from02)) return true;
            int port = ParseInt(from03);
            var cv = engine.World.FindByPort(port);
            if (cv == null) return false;
            return engine.Conveyor(cv.Id).PalletAt(port) != null;
        }

        // [LGLS 2026-09-01] 도착지가 포트일 때 그 포트가 점유 중인가 (랙 셀은 항상 false)
        private bool IsDestPortOccupied()
        {
            if (!IsPort(to01, to02)) return false;
            int port = ParseInt(to03);
            var cv = engine.World.FindByPort(port);
            if (cv == null) return false;
            return engine.Conveyor(cv.Id).PalletAt(port) != null;
        }

        private void UnloadAtDest()
        {
            if (carrying == null) carrying = new SimPallet { Id = palletId };

            if (IsPort(to01, to02))
            {
                int port = ParseInt(to03);
                var cv = engine.World.FindByPort(port);
                if (cv == null) { engine.Log(Def.Id + " 알 수 없는 도착 포트 " + to03); return; }

                // 라인 C/V(2~10)에 내려놓으면: RGV→입고 진행, S/C→출고 진행.
                // 입출고 C/V(11~15)에 내려놓으면 출고 흐름.
                FlowDir dir = cv.No >= 11 ? FlowDir.Outgo
                            : Def.IsRgv ? FlowDir.Ingo : FlowDir.Outgo;
                carrying.Dir = dir;
                engine.Conveyor(cv.Id).PlacePallet(port, carrying.Id, dir);
                engine.Log(Def.Id + " P" + port + " 하차 (JOB " + carrying.Id + ")");
            }
            else
            {
                string cell = RackCell(to01, to02, to03);
                engine.StoreToRack(cell, carrying.Id);
                engine.Log(Def.Id + " 랙 " + cell + " 입고 저장 (JOB " + carrying.Id + ")");
            }
        }

        /// <summary>통로(01=홀수뱅크/02=짝수뱅크), Bay, Level → 6자리 BBbbLL. 담당 Bank 로 환원.</summary>
        private string RackCell(string aisle, string bay, string level)
        {
            int bank;
            if (Def.Banks != null && Def.Banks.Length == 2)
                bank = aisle == "02" ? Def.Banks[1] : Def.Banks[0];
            else
                bank = ParseInt(aisle);
            return bank.ToString("00") + ParseInt(bay).ToString("00") + ParseInt(level).ToString("00");
        }

        private static int ParseInt(string s)
        {
            int v;
            return int.TryParse((s ?? "").Trim(), NumberStyles.Integer, CultureInfo.InvariantCulture, out v) ? v : 0;
        }

        private string FromText() { return IsPort(from01, from02) ? "P" + ParseInt(from03) : "랙 " + RackCell(from01, from02, from03); }
        private string ToText() { return IsPort(to01, to02) ? "P" + ParseInt(to03) : "랙 " + RackCell(to01, to02, to03); }

        public string DisplayText
        {
            get { return Def.Id + " : " + StatusText; }
        }
    }
}
