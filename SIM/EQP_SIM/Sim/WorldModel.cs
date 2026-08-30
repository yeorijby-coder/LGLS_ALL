using System;
using System.Collections.Generic;
using System.Linq;

namespace EQP_SIM.Sim
{
    public enum FlowDir { None, Ingo, Outgo }

    /// <summary>컨베이어 위 파렛트</summary>
    public class SimPallet
    {
        public string Id = "";            // JOB NO (4자리) — ECS가 트래킹에 기록하면 채워짐
        public FlowDir Dir = FlowDir.Ingo;
        public DateTime MoveReadyAt = DateTime.MinValue;
        public DateTime HoldUntil = DateTime.MinValue;   // Wait-Out 포트 체류 시각
        public DateTime ArrivedAt = DateTime.MinValue;
        public bool Discharged = false;              // [LGLS] 출고대 배출(지게차) 1단계 완료
        public DateTime DischargedAt = DateTime.MinValue;  // [LGLS] 배출 시각(+지연 후 트래킹 제거)
        public DateTime SensedAt = DateTime.MinValue;      // [LGLS 2026-08-22] 화물감지가 실제로 ON 된 시각(입고대 준비 판정 기준)
        /// <summary>[LGLS 2026-08-22] 출고대 신호(WAIT_IN)가 ON 된 시각. 지게차 반출 3초 카운트의 기준점.</summary>
        public DateTime OutSignalAt = DateTime.MinValue;
        public DateTime SensorOnAt = DateTime.MinValue;    // [LGLS 2026-07-21] 입고대 적재 후 재하감지 ON 예정 시각
                                                           //   (MinValue=이미 감지됨. PLC가 적재 2초 뒤 신호를 올리는 동작 재현)
    }

    public class ConveyorDef
    {
        public string Id;                 // CONVEYOR:11
        public int No;                    // 11
        public int[] Ports;               // 포트 번호 목록
        public int[] Orders;              // 각 포트의 PortOrder (ECS Port.cs GetDefaultPortOrder 하드코딩과 일치)
                                          // 관측값 _01.._0n 은 PortOrder 기준! (C/V#15: 31→1, 32→2, 30→3)
        public int[] IngoPath;            // 입고 이동 경로 (포트 번호)
        public int[] OutgoPath;           // 출고 이동 경로
        public bool IsInput;              // ECSDeviceManager.IsInputConveyor
        public bool HasDirection;         // C/V#2, #11 (방향 전환형)
        /// <summary>[LGLS 2026-08-22] RTV(RGV) 도착지 신호를 내보내는 포트. 0=없음.
        ///   겸용 트랙은 출고 모드일 때 RGV 가 이 포트로 화물을 가지러/놓으러 오므로 ON 이 된다.</summary>
        public int RtvArrivePort;
        /// <summary>[LGLS 2026-08-22] 출고HS(핸드셰이크) 신호를 내보내는 포트(S/C 접점). 0=없음.</summary>
        public int RetHsPort;

        /// <summary>포트 번호 → PortOrder (1-base). 없으면 0.</summary>
        public int OrderOf(int port)
        {
            int i = Array.IndexOf(Ports, port);
            return i < 0 ? 0 : Orders[i];
        }

        /// <summary>PortOrder → 포트 번호. 없으면 0.</summary>
        public int PortOfOrder(int order)
        {
            int i = Array.IndexOf(Orders, order);
            return i < 0 ? 0 : Ports[i];
        }
    }

    public class VehicleDef
    {
        public string Id;                 // VEHICLE:1(RGV), VEHICLE:11..15(S/C)
        public bool IsRgv;
        public int ScNo;                  // S/C 번호 (1~5), RGV=0
        public int[] Banks;               // S/C 담당 Bank (2k-1, 2k)
    }

    /// <summary>
    /// 설비 구성 (TB_LOCATION / TB_ITINERARY / ECSDeviceManager 와 일치).
    ///  - 입고대: C/V#11(22→21), C/V#12(24→23), C/V#15(30→31)
    ///  - 출고대: C/V#11(21→22), C/V#13(25→26), C/V#14(27→28→29)
    ///  - S/C 라인: C/V#2(SC1), #3(SC2), #5(SC3), #7(SC4), #9(SC5) — 홀수포트=RGV측, 짝수포트=SC측
    /// </summary>
    public class WorldModel
    {
        public readonly Dictionary<string, ConveyorDef> Conveyors = new Dictionary<string, ConveyorDef>();
        public readonly Dictionary<string, VehicleDef> Vehicles = new Dictionary<string, VehicleDef>();
        private readonly Dictionary<int, ConveyorDef> portOwner = new Dictionary<int, ConveyorDef>();

        // ECSDeviceManager.IsInputConveyor 와 동일
        public static readonly int[] InputConveyors = { 3, 5, 7, 9, 11, 12, 15 };
        // S/C 라인 컨베이어: CV번호 → SC번호
        public static readonly Dictionary<int, int> LineCvToSc = new Dictionary<int, int>
        { { 2, 1 }, { 3, 2 }, { 5, 3 }, { 7, 4 }, { 9, 5 } };

        /// <summary>
        /// wcsOrientation=false(구 ECS 모드): 원본 ECS 구성 그대로 — C/V#12=입고대(24→23), C/V#13=출고대(25→26).
        ///   (ECSDeviceManager.IsInputConveyor={..12..}, TB_ITINERARY I002/O032 기준. 구 ECS는 수정 불가이므로 이 구성이 기준)
        /// wcsOrientation=true(WCS 모드): [LGLS 2026-07-15] EcsDefine(WCS Client) 정렬 — 124(C/V#12)=출고대, 126(C/V#13)=입고대.
        /// </summary>
        public WorldModel(bool wcsOrientation)
        {
            // 라인 컨베이어 C/V#2~#10 : 포트 (2n-1, 2n), 홀수=RGV측 (PortOrder: 홀수=1, 짝수=2)
            for (int n = 2; n <= 10; n++)
            {
                int lo = (n - 1) * 2 + 1;      // CV2→3, CV3→5 ... CV10→19
                int hi = lo + 1;
                AddConveyor(new ConveyorDef
                {
                    Id = "CONVEYOR:" + n,
                    No = n,
                    Ports = new[] { lo, hi },
                    Orders = new[] { 1, 2 },
                    IngoPath = new[] { lo, hi },   // RGV가 홀수에 내려놓고 SC측(짝수)으로
                    OutgoPath = new[] { hi, lo },  // SC가 짝수에 내려놓고 RGV측(홀수)으로
                    IsInput = Array.IndexOf(InputConveyors, n) >= 0,
                    HasDirection = (n == 2),
                    // [LGLS 2026-08-22] C/V#2(S/C#1 통로 겸용)만 방향 신호를 갖는다:
                    //   RGV측 3번 트랙 = RTV 도착지, S/C측 4번 트랙 = 출고HS
                    RtvArrivePort = (n == 2) ? lo : 0,
                    RetHsPort     = (n == 2) ? hi : 0
                });
            }

            // PortOrder 는 ECS Port.cs GetDefaultPortOrder 하드코딩과 동일해야 함:
            //  21=1,22=2 | 23=1,24=2 | 25=1,26=2 | 27=1,28=2,29=3 | 31=1,32=2,30=3 (※ C/V#15 특수)
            AddConveyor(new ConveyorDef { Id = "CONVEYOR:11", No = 11, Ports = new[] { 21, 22 }, Orders = new[] { 1, 2 },
                IngoPath = new[] { 22, 21 }, OutgoPath = new[] { 21, 22 }, IsInput = true, HasDirection = true,
                RtvArrivePort = 21 });   // [LGLS 2026-08-22] 21번 트랙 = RGV 접점(RTV 도착지). 22번은 지게차 입출고대
            if (wcsOrientation)
            {
                // [LGLS 2026-08-24] ★현장 확인 기준으로 정정 : C/V#12 = 입고대(TR#24), C/V#13 = 출고대(TR#26).
                //   종전에는 "EcsDefine 정렬"이라며 반대로(124=출고/126=입고) 두었으나, 현장·WCS DB 경로정의
                //   (CV_DEF_INF : 124→123 "C/V#12 입고" / 125→126 "C/V#13 출고") 및 DEST_POS_DEF(출고 목적지
                //   1022·1026·1029)와 어긋났다. DB 가 맞고 이 모델이 틀렸던 것이라 여기를 뒤집는다.
                //   → 구 ECS 모드(else)와 결과적으로 같아지지만, 모드 분기 구조는 그대로 둔다.
                AddConveyor(new ConveyorDef { Id = "CONVEYOR:12", No = 12, Ports = new[] { 23, 24 }, Orders = new[] { 1, 2 },
                    IngoPath = new[] { 24, 23 }, OutgoPath = null, IsInput = true });    // 124=입고대(Sto)
                AddConveyor(new ConveyorDef { Id = "CONVEYOR:13", No = 13, Ports = new[] { 25, 26 }, Orders = new[] { 1, 2 },
                    IngoPath = null, OutgoPath = new[] { 25, 26 }, IsInput = false });   // 126=출고대(Ret)
            }
            else
            {
                // 원본 ECS 구성: C/V#12=입고대(24→23), C/V#13=출고대(25→26)
                AddConveyor(new ConveyorDef { Id = "CONVEYOR:12", No = 12, Ports = new[] { 23, 24 }, Orders = new[] { 1, 2 },
                    IngoPath = new[] { 24, 23 }, OutgoPath = null, IsInput = true });
                AddConveyor(new ConveyorDef { Id = "CONVEYOR:13", No = 13, Ports = new[] { 25, 26 }, Orders = new[] { 1, 2 },
                    IngoPath = null, OutgoPath = new[] { 25, 26 }, IsInput = false });
            }
            AddConveyor(new ConveyorDef { Id = "CONVEYOR:14", No = 14, Ports = new[] { 27, 28, 29 }, Orders = new[] { 1, 2, 3 },
                IngoPath = null, OutgoPath = new[] { 27, 28, 29 }, IsInput = false });
            AddConveyor(new ConveyorDef { Id = "CONVEYOR:15", No = 15, Ports = new[] { 30, 31, 32 }, Orders = new[] { 3, 1, 2 },
                IngoPath = new[] { 30, 31 }, OutgoPath = null, IsInput = true });

            Vehicles["VEHICLE:1"] = new VehicleDef { Id = "VEHICLE:1", IsRgv = true };
            for (int k = 1; k <= 5; k++)
            {
                Vehicles["VEHICLE:1" + k] = new VehicleDef
                {
                    Id = "VEHICLE:1" + k,
                    IsRgv = false,
                    ScNo = k,
                    Banks = new[] { 2 * k - 1, 2 * k }
                };
            }
        }

        private void AddConveyor(ConveyorDef def)
        {
            Conveyors[def.Id] = def;
            foreach (int p in def.Ports) portOwner[p] = def;
        }

        public ConveyorDef FindByPort(int port)
        {
            ConveyorDef def;
            return portOwner.TryGetValue(port, out def) ? def : null;
        }

        /// <summary>S/C 번호 → 담당 라인 컨베이어</summary>
        public ConveyorDef LineConveyorOfSc(int scNo)
        {
            foreach (var kv in LineCvToSc)
                if (kv.Value == scNo) return Conveyors["CONVEYOR:" + kv.Key];
            return null;
        }

        /// <summary>
        /// ECSDeviceManager.GetWaitOutPort 과 동일한 규칙 (PortOrder 기준).
        /// direction: 해당 컨베이어의 DIRECTION_MODE 문자열("0"/"1")
        /// </summary>
        public int GetWaitOutPort(ConveyorDef cv, string direction)
        {
            if (cv.IsInput) return cv.PortOfOrder(1);
            if (cv.No == 2 && direction == "0") return cv.PortOfOrder(1);
            return cv.PortOfOrder(cv.Ports.Length);
        }
    }
}
