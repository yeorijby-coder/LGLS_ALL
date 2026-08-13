using System;
using HOST_SIM.Protocol;

namespace HOST_SIM.Logic
{
    public enum CyclePhase
    {
        Stopped,        // 미기동
        WaitIngoDone,   // 입고 작업지시 전송 후 입고완료(F/1) 대기
        WaitOutgoDone   // 출고 작업지시 전송 후 출고완료(F/2) 대기
    }

    /// <summary>
    /// 개별 순환 로직 상태 머신.
    /// [시작] → 입고 O 전송(입고대→랙) → F(입고완료) 수신 → 출고 O 전송(랙→출고대)
    ///        → F(출고완료) 수신 → 랙+1 후 다시 입고 O ... 반복. [정지] 시 중단.
    /// </summary>
    public class CycleLogic
    {
        public string Name { get; private set; }
        public string InStation { get; private set; }    // 입고대 스테이션 코드(3자리)
        public string OutStation { get; private set; }   // 출고대 스테이션 코드(3자리)
        public int RackStationBase { get; private set; } // 하이랙 스테이션 코드 베이스 (200 + SC번호 = 201~205)
        public RackAllocator Racks { get; private set; }

        public CyclePhase Phase { get; private set; }
        public string CurrentLuggageNo { get; private set; }  // 진행 중 작업번호
        public string CurrentPallet { get; private set; }     // 진행 중 파렛트명 (입고/출고 동일해야 함)
        public string CurrentCell { get; private set; }       // 진행 중 랙 셀
        public int CycleCount { get; private set; }

        /// <summary>
        /// 로직 활성화(체크박스). false 로 바꾸면 진행 중인 사이클은 출고 완료까지 계속 진행하되
        /// 이후 새 입고 작업을 만들지 않고 일시정지한다. 다시 true 가 되면(Start 호출)
        /// 직전 출고 랙의 +1 셀(RackAllocator 가 이미 전진해 둔 다음 셀)에서 입고를 재개한다.
        /// </summary>
        public bool Enabled = true;

        /// <summary>출고 완료 후 Enabled=false 로 인해 일시정지된 상태인지</summary>
        public bool PausedAfterOutgo { get; private set; }

        /// <summary>
        /// [LGLS 2026-07-19] 담당 크레인이 전부 정지/에러라 입고를 만들지 못하고 대기 중인지.
        /// S 상태보고로 크레인이 회복되면 MainForm 이 Start() 를 재호출해 재개한다.
        /// </summary>
        public bool PausedNoCrane { get; private set; }

        private readonly Func<string> nextLuggageNo;          // 전역 작업번호 발번기
        private readonly Action<CycleLogic, byte[], string> sendOrder; // (logic, body, 설명)
        private readonly Func<int, bool> scIngoAvailable;     // [LGLS] SC번호 → 입고 가능 여부 (null=항상 가능)
        private readonly Action<string> logInfo;              // [LGLS] 정보 로그 콜백

        public string ProductId { get; private set; }     // O_ProductID (ASCII 전용 — 전문은 ASCII 인코딩)

        public CycleLogic(string name, string productId, string inStation, string outStation, int rackStationBase,
                          RackAllocator racks, Func<string> nextLuggageNo,
                          Action<CycleLogic, byte[], string> sendOrder,
                          Func<int, bool> scIngoAvailable = null, Action<string> logInfo = null)
        {
            this.scIngoAvailable = scIngoAvailable;
            this.logInfo = logInfo;
            Name = name;
            ProductId = productId;
            InStation = inStation;
            OutStation = outStation;
            RackStationBase = rackStationBase;
            Racks = racks;
            this.nextLuggageNo = nextLuggageNo;
            this.sendOrder = sendOrder;
            Phase = CyclePhase.Stopped;
        }

        /// <summary>[시작]/재개 — 입고 작업지시 전송 (비활성 로직은 기동하지 않음)</summary>
        public void Start()
        {
            if (Phase != CyclePhase.Stopped) return;
            if (!Enabled) return;
            PausedAfterOutgo = false;
            PausedNoCrane = false;
            SendIngo();
        }

        /// <summary>[정지] — 즉시 중단 (진행 중 전문에는 더 이상 반응하지 않음)</summary>
        public void Stop()
        {
            Phase = CyclePhase.Stopped;
            PausedAfterOutgo = false;
        }

        /// <summary>
        /// F 작업완료보고 수신 처리. 이 로직의 작업이면 true.
        /// 출고는 StepCount='2'(전체 출고 완료)만 인정 — StepCount='1'은 하이랙 단위 중간 보고.
        /// </summary>
        public bool OnComplete(char jobDefine, char stepCount, string luggageNo)
        {
            if (Phase == CyclePhase.Stopped) return false;
            if (luggageNo != CurrentLuggageNo) return false;

            if (Phase == CyclePhase.WaitIngoDone && jobDefine == WmsMessage.JOB_INGO)
            {
                // 입고 완료 → 같은 랙에서 출고대로 출고 작업 생성
                SendOutgo();
                return true;
            }
            if (Phase == CyclePhase.WaitOutgoDone && jobDefine == WmsMessage.JOB_OUTGO && stepCount == '2')
            {
                // 출고 완료 → 랙+1 하여 다음 입고 작업 생성 (비활성화 시 여기서 일시정지)
                CycleCount++;
                if (Enabled)
                {
                    SendIngo();
                }
                else
                {
                    Phase = CyclePhase.Stopped;
                    PausedAfterOutgo = true;
                }
                return true;
            }
            return false;
        }

        /// <summary>셀(BBbbLL)의 담당 SC 번호 (Bank 2k-1/2k → SC#k)</summary>
        private static int ScOfCell(string cell)
        {
            int bank = int.Parse(cell.Substring(0, 2));
            return (bank + 1) / 2;
        }

        /// <summary>셀(BBbbLL)의 Bank 로 담당 SC의 하이랙 스테이션 코드 산출 (SC#k = base+k, Bank 2k-1/2k)</summary>
        private string RackStationOf(string cell)
        {
            return (RackStationBase + ScOfCell(cell)).ToString();
        }

        /// <summary>C/V#11 WMS 코드 (GlobalConstant.CONVEYOR11_WMS_CODE)</summary>
        public const string CV11_CODE = "101";

        private void SendIngo()
        {
            // [LGLS 2026-07-19] 정지/에러 크레인 건너뛰기: 대상 셀의 담당 SC 가 정지(입고금지/전체정지)·
            //   에러·오프라인이면 그 크레인의 셀을 건너뛰고 다음 뱅크(다음 크레인) 셀로 넘어간다.
            //   담당 크레인 전부가 불가면 입고를 만들지 않고 대기(PausedNoCrane) — S 상태보고로
            //   회복이 확인되면 MainForm 이 Start() 를 재호출해 재개한다.
            string cell = null;
            for (int t = 0; t < Racks.BankCount; t++)
            {
                string cand = Racks.Next();
                if (scIngoAvailable == null || scIngoAvailable(ScOfCell(cand))) { cell = cand; break; }
                if (logInfo != null) logInfo(string.Format("{0} 크레인 SC{1} 정지/에러 — 셀 {2} 건너뜀", Name, ScOfCell(cand), cand));
            }
            if (cell == null)
            {
                Phase = CyclePhase.Stopped;
                PausedNoCrane = true;
                if (logInfo != null) logInfo(Name + " 담당 크레인 전체 정지/에러 — 입고 생성 대기 (회복 시 자동 재개)");
                return;
            }

            // C/V#11(입출고 겸용)은 출고 후 CONVEYOR11_MODE 가 '1'로 남으므로
            // 입고 지시에 앞서 M 모드변경(입고=0) 전문으로 복귀시킨다.
            if (InStation == CV11_CODE)
            {
                byte[] mode = WmsMessage.BuildModeChange(CV11_CODE, '0');
                sendOrder(this, mode, Name + " 모드변경 C/V#11 → 입고(0)");
            }

            CurrentLuggageNo = nextLuggageNo();
            CurrentPallet = "P" + CurrentLuggageNo.PadLeft(6, '0');   // 7자, 입고/출고 동일 유지
            CurrentCell = cell;

            byte[] body = WmsMessage.BuildOrder(
                WmsMessage.JOB_INGO, CurrentLuggageNo, CurrentLuggageNo, CurrentPallet,
                InStation, null,                     // 출발지: 입고대
                RackStationOf(CurrentCell), CurrentCell,   // 도착지: 랙 셀
                1, ProductId);

            Phase = CyclePhase.WaitIngoDone;
            sendOrder(this, body, string.Format("{0} 입고지시 JOB={1} {2}→랙 {3}", Name, CurrentLuggageNo, InStation, CurrentCell));
        }

        private void SendOutgo()
        {
            CurrentLuggageNo = nextLuggageNo();
            // 파렛트명은 입고 때와 동일 (ECS가 TB_PALLET에서 Assigned 상태 파렛트를 찾음)

            byte[] body = WmsMessage.BuildOrder(
                WmsMessage.JOB_OUTGO, CurrentLuggageNo, CurrentLuggageNo, CurrentPallet,
                RackStationOf(CurrentCell), CurrentCell,   // 출발지: 입고 완료된 랙 셀
                OutStation, null,                    // 도착지: 출고대
                1, ProductId);

            Phase = CyclePhase.WaitOutgoDone;
            sendOrder(this, body, string.Format("{0} 출고지시 JOB={1} 랙 {2}→{3}", Name, CurrentLuggageNo, CurrentCell, OutStation));
        }

        public string StatusText
        {
            get
            {
                string tail = Enabled ? "" : " [비활성]";
                switch (Phase)
                {
                    case CyclePhase.WaitIngoDone:
                        return string.Format("{0}: 입고중 JOB={1} 랙={2} (완료 {3}회){4}", Name, CurrentLuggageNo, CurrentCell, CycleCount, tail);
                    case CyclePhase.WaitOutgoDone:
                        return string.Format("{0}: 출고중 JOB={1} 랙={2} (완료 {3}회){4}", Name, CurrentLuggageNo, CurrentCell, CycleCount, tail);
                    default:
                        if (PausedNoCrane)
                            return string.Format("{0}: 대기 — 담당 크레인 정지/에러 (완료 {1}회){2}", Name, CycleCount, tail);
                        if (PausedAfterOutgo)
                            return string.Format("{0}: 일시정지 — 출고 완료 후 대기 (완료 {1}회){2}", Name, CycleCount, tail);
                        return string.Format("{0}: 정지 (완료 {1}회){2}", Name, CycleCount, tail);
                }
            }
        }
    }
}
