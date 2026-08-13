# LGLS.SIM — LG생명과학 ECS 시뮬레이터 (EQP_SIM / HOST_SIM)

참조 시스템: `C:\Users\USER\Downloads\일반비_LG생명과학(2011_04_13)` (이하 **ECS**)
분석 근거: ECS/ECP 소스, `ECS-분석-PLC사양및시나리오_LG생명과학_V1.0_20260708.pptx`, LglsMCS DB

```
┌──────────┐  WMS 전문(TCP)    ┌───────┐  XGT FEnet(TCP)   ┌──────────┐
│ HOST_SIM │ ──9911──────────► │  ECS  │ ──2004/3001─────► │ EQP_SIM  │
│ (WMS/IMS)│ ◄─9910── S/E/F ── │(HECS) │    (ECS=클라)      │ (XGT PLC)│
└──────────┘                   └───────┘                   └──────────┘
```

## 빌드 / 실행

- VS2019 로 `LGLS.SIM.sln` 열기 (.NET Framework 4.7.2, WinForms) 또는
  `MSBuild LGLS.SIM.sln -p:Configuration=Debug`
- **기동 순서**: ① EQP_SIM → ② HOST_SIM → ③ ECS(HECS.exe)
- 사전 1회: `db\seed_wms_station_codes.sql`, `db\seed_eqp_status_codes.sql` 을
  LglsMCS(**localhost\SQL2022**) 에 적용 (적용 완료됨)
- ※ **EQP_SIM 을 재시작하면 ECS(HECS)도 재시작**할 것 — ECS FenetDriver 는 운영 중
  PLC 소켓 절단을 복구하지 못함(오류를 무시하는 legacy 코드). 미반송 작업은
  ECS 재기동 시 TB_TRANSFER 에서 재로드되어 이어서 진행됨.

## EQP_SIM (XGT PLC 설비 시뮬레이터)

- **XGT FEnet 서버** (기본 포트 2004, 3001 동시 수신 — `EQP_SIM.ini [XGT]`)
  ECS의 `FenetDriver` 프레임(LSIS-XGT, 연속읽기 0x54/연속쓰기 0x58/%MX 비트쓰기,
  "0###" DATACOUNT 인코딩, 읽기응답 오프셋 32) 그대로 구현.
- **관측값 주소**: `observables.tsv` (TB_OBSERVABLE 덤프, 524점) 파일 로드 — **DB 미사용**.
- **시나리오** (PPTX 슬라이드 5~17 그대로):
  - 컨베이어: 파렛트 투입(Load Complete#2) → ECS가 트래킹에 JOB 기록 → 구간 이동/트래킹 Shift
    → Wait-Out 보고 → RGV/SC 상·하차(Unload/Load Complete#1,2 + Ack 핸드셰이크)
  - S/C·RGV: Transfer Request 스트로브 → RUN(2) → 상차(Pallet Presence, Load Complete/Ack)
    → 하차(Unload Complete/Ack, Transfer Complete Location) → IDLE(1)
  - 이벤트↔Ack: Event SET → ECS Ack SET → **PLC가 둘 다 RESET** (슬라이드 5 공통 규칙)
- **데이터 파일 저장**: `bin\Debug\Data\` — `plc_memory.bin`(메모리 스냅샷),
  `eqp_state.txt`(파렛트/랙 재고), `eqp_sim_*.log`
- UI: 설비 상태(포트별 파렛트/JOB, 차량 상태, 랙 재고) + 로그.
  입고대 자동/수동 파렛트 투입, 출고대 자동 반출(작업자 자동화), 상태 초기화.

## HOST_SIM (WMS/IMS 시뮬레이터)

- **채널 A**: ECS_PORT **9911** 로 접속(클라이언트) — O 작업지시 송신, ACK/NAK 수신
- **채널 B**: **9910** 수신대기(서버) — ECS의 S(상태)/E(에러)/F(작업완료) 수신
- 전문 형식: `WMSCommand.cs` 그대로 (헤더15 + STX + BODY + ETX, O=66/ACK=11/S=43/E=23/F=10)
- **UI**: [시작]/[종료] 버튼 + 좌(송신 전문)/우(수신 전문) 리스트 2개
  - 프로그램 시작 시 두 버튼 모두 비활성 → **첫 S(상태보고) 수신 시 [시작]만 활성화**
  - [시작] 클릭 → 시작 비활성/종료 활성, 체크된 로직 기동. [종료] → 즉시 정지.
  - **로직별 체크박스(로직1~3)**: 체크 해제 시 해당 로직은 진행 중인 사이클을
    출고 완료까지 마친 뒤 일시정지(새 입고 미생성, 상태표시 "일시정지 — 출고 완료 후 대기").
    다시 체크하면 직전 출고 랙의 **+1 셀**부터 입고를 재개한다.
- **공통 로직** (로직별 독립 상태머신):
  1. 입고 O 전송 (출발=입고대, 도착=랙 셀, 구분 1)
  2. F(입고완료, JobDefine=1) 수신 → 출고 O 전송 (출발=그 랙 셀, 도착=출고대, 구분 2)
  3. F(출고 전체완료, JobDefine=2 & StepCount=2) 수신 → **랙+1** (담당 SC Bank 순환:
     010101→020101→…→Bay+1→Level+1) 후 다시 입고 O … 반복
- 작업번호(LuggageNo)는 일자별 시퀀스로 발번, `Data\luggage_seq.txt` 에 보존(재시작 중복 방지).
  파렛트명(O_PalletNo)은 입고/출고 동일 유지 (ECS TB_PALLET Assigned 매칭 요건).
- 로직 설정은 `HOST_SIM.ini` 에서 변경 가능:
  | 로직 | 입고대 | 출고대 | SC (Bank) |
  |---|---|---|---|
  | 1 | **C/V#12 → TR#24 (102)** | **C/V#13 → TR#26 (103)** | 1호기 (01,02) |
  | 2 | C/V#15 → TR#30 (105) | C/V#14 → TR#29 (104) | 2,3호기 (03~06) |
  | 3 | C/V#11 → TR#22 (101) | C/V#11 → TR#22 (101) | 4,5호기 (07~10) |

  > ※ **로직1 주의**: 요청서에는 입고대 C/V#13, 출고대 C/V#12로 되어 있으나 ECS 소스
  > (`IsInputConveyor={3,5,7,9,11,12,15}`, `OnWaitOut`의 CONVEYOR:13 통과전용 처리)와
  > DB 반송경로(TB_ITINERARY: 입고시작 I002=C/V#12 P24, 출고종점 O032=C/V#13 P26)상
  > **C/V#12=입고대(TR#24), C/V#13=출고대(TR#26)** 가 시스템의 실제 구성입니다.
  > 로직 2/3은 요청서와 DB가 정확히 일치하므로, 로직1은 시스템 구성에 맞춰 설정했습니다.

## WMS 스테이션 코드 (TB_CODEMASTER codeclass '022' — `db\seed_wms_station_codes.sql`)

O 전문의 3자리 출발/도착 스테이션 코드. 원본 DB에 누락되어 있어 시드 필수
(ECS `getPositionInfoList()` 가 이 코드로 위치를 해석; `CONVEYOR11_WMS_CODE="101"` 과 정합).

| 코드 | 의미 | 존 | 장비/위치 |
|---|---|---|---|
| 101 | C/V#11 입출고대 TR#22 | KR00 | CNVSEM:1 / CONVEYOR:11 / PORT:22 |
| 102 | C/V#12 입고대 TR#24 | KR00 | CNVSEM:2 / CONVEYOR:12 / PORT:24 |
| 103 | C/V#13 출고대 TR#26 | KR02 | CNVSEM:2 / CONVEYOR:13 / PORT:26 |
| 104 | C/V#14 출고대 TR#29 | KR02 | CNVSEM:3 / CONVEYOR:14 / PORT:29 |
| 105 | C/V#15 입고대 TR#30 | KR00 | CNVSEM:3 / CONVEYOR:15 / PORT:30 |
| 201~205 | 하이랙 S/C#1~#5 (Bank 2k-1,2k) | KR01 | STKSEM:1~5 |

## ECS 측 수정 사항 (참조 폴더에 적용됨)

1. `ECS\Program.cs` `ApplyDriverType()` — FEnet 연결문자열에 **`R_START_ADDR=0, R_END_ADDR=8192` 추가 후 리빌드**.
   (미적용 시 R영역 모니터 미등록 → 트래킹/PALLET_ON_VEHICLE String 판독이
   `m_MonitorMemory["R"]` KeyNotFoundException 으로 실패 → 반송 완료 불가)
2. `ECS\bin\Debug\HECS.ini` `[HOST]` — `CONNECT_ON_START=1`, `RECONNECT=1`
   (HOST_SIM 이 9910을 항상 열고 있으므로 안전. 미적용 시 S/E/F 보고 미전송)
3. DB — codeclass '022' 스테이션 코드 시드 (위 표).

## 문서

- `docs\HOST_PROTOCOL.md` — WMS 전문 상세 (필드 오프셋/길이, ACK/NAK, S/E/F)
- `EQP_SIM\observables.tsv` — PLC 주소맵 (TB_OBSERVABLE 덤프; 갱신 시 재추출)
