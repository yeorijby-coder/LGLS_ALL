# 구 ECS(Backup\ECS, HECS) PLC 기록 구문 전수 정리

작성: 2026-08-13. 대상: `Backup\ECS` C# 소스 전체.

## 기록 구조 (공통)

구 ECS는 PLC에 직접 Write 함수를 호출하지 않고 **ezMCS 프레임워크의 태그(Observable) 대입**으로 기록한다.

```
업무 함수
  → 설비객체.IO_태그명 = 값                        (VehicleConfig/ConveyorConfig 의 IO_ 프로퍼티)
  → Observables["태그명"].AsBoolean/AsString = 값   (프레임워크 Observable)
  → TB_OBSERVABLE 주소 매핑 (DEVICE_TYPE=W/B/R + ADDRESS_NO)
  → FieldbusDriver(FENET) 블록 Write                (프레임워크 DLL, TB_FIELDBUSDRIVER 설정)
```

- 주소 표기: `W`(워드)=D영역, `B`(비트)=M영역, `R`=트래킹 R영역.
  W/B의 ADDRESS_NO는 16진, R은 10진. 실전송: W→%DW(×2=%DB), B→%MX, R→%RB(×2).
- 전체 태그↔주소 표는 `observables.tsv`(=TB_OBSERVABLE 덤프) 참조.

### 5단계 ↔ TB_OBSERVABLE 필드 매칭 (LglsMCS DB 실측)

TB_OBSERVABLE 실제 행 예 (`OWNERID='VEHICLE:11'`, SC#1의 반송요청 태그):

| 필드 | 값 | 5단계 중 어디에 쓰이나 |
|---|---|---|
| **OWNERID** | `VEHICLE:11` | ①단계 — `vehicle` 객체가 기동 시 자기 소유 태그를 이 키로 로드 (1=RGV, 11~15=SC#1~5, CONVEYOR:n=C/V#n) |
| OWNERTYPE | `SubSystem` | 객체 클래스 종류 (Vehicle 계열) |
| **OBSERVABLEID** | `TRANSFER_REQUEST` | ②단계 — `Observables["TRANSFER_REQUEST"]` 딕셔너리 키(태그 이름) |
| **ACCESSTYPE** | `IN_OUT` | ③단계 — 쓰기 허용 여부 (IN=읽기전용 / IN_OUT=양방향) |
| **DATATYPE** | `Boolean` | ③단계 — `.AsBoolean`/`.AsString` 캐스팅과 짝 |
| **DRIVERNO** | `1` | ④단계 — TB_FIELDBUSDRIVER의 1번(FENET, `EzControl.Driver.Fenet.FenetDriver`)이 이 태그를 담당 |
| MONFLAG | `T` | 읽기 방향 — SCANINTERVAL(500ms) 주기 폴링 대상 |
| **CONNECTSTRING** | `DEVICE_TYPE=B,ADDRESS_NO=0600` | ⑤단계 — **주소 결정의 핵심**. B=M비트/W=D워드/R=트래킹, 16진 주소, LENGTH=워드수. B0600=%MX1536 |

TB_FIELDBUSDRIVER(DRIVERNO=1): DRIVERTYPE=FENET, SCANINTERVAL=500ms,
CONNECTSTRING=`CHANNEL_NO=51, B_START_ADDR=0~8192, W_START_ADDR=0~8192, R_START_ADDR=0~8192`
— B/W/R 영역을 블록으로 폴링(읽기)하고, 쓰기 태그는 변경 시 해당 주소에 기록.

다른 예: `PALLET_ID`(String, W,0306,LENGTH=2 → %DW774 2워드), `LOAD_COMPLETE`(보고, B,0310 → %MX784),
`FROM_01`(String, W,0300 → %DW768).

### B/W/R 표기 → 실주소 계산 방식 (3단계)

1. **디바이스 문자 치환 (★실 영역이 바뀌는 지점)** — XGT엔 B/W 디바이스가 없다(멜섹식 논리표기).
   드라이버 계층에서 `B→M(비트)`, `W→D(워드)`, `R→R(트래킹)` 로 치환된다.
2. **숫자 해석** — B/W 의 ADDRESS_NO 는 **16진**, R 은 **10진**(⚠ 과거에 R을 16진으로 잘못 읽는 실수가 있었고 실 PLC로 10진 확정).
3. **단위 변환** — M: `%MX=값, 통신워드=÷16, 비트=나머지` / D: `%DW=값, %DB=×2` / R: `워드=값, %RB=×2`.

계산 예:

| 구표기 | ①치환 | ②숫자 | ③실주소 | 비고 |
|---|---|---|---|---|
| `B02A1` | B→M | 0x2A1 = 673 | **%MX673** = %MW42 비트1 | 실존: C/V#14 `UNLOAD_COMPLETE_01`. PPT 구표기로는 **M0421**(워드42·비트1) — 같은 비트의 두 표기 |
| `W1EEE` | W→D | 0x1EEE = 7918 | **%DW7918** (%DB15836) | 실사용 대역(%DW256~854) 밖 = 스페어 |
| `R1336` | R→R | **10진 그대로** 1336 (0x1336=4918 은 오답) | 워드 1336 (**%RB2672**) | 트래킹 규칙 `R = (호기-1)×10 + (포지션-1)×2` 기준 "134호기 포지션4" 상당 — 이 현장(C/V#1~15) 범위 밖 |

---

## D영역 (워드) 기록

### 1. S/C·RGV 반송 지시 (FROM/TO/PALLET_ID)
```
ECSDispatcher.DoWork / DoWorkBoost / DoWorkKR01 / DoWorkOtherTransfers   (디스패처 스레드 루프)
  → ExecuteCommandStockerSEM
     → ExecuteCommandRackToConveyor   (출고: 랙→C/V)   Vehicle.IO_FROM_01~03(랙위치), IO_TO_01~03(00,00,포트), IO_PALLET_ID
     → ExecuteCommandConveyorToRack   (입고: C/V→랙)   Vehicle.IO_FROM_01~03(00,00,포트), IO_TO_01~03(랙위치), IO_PALLET_ID
  → ExecuteVehicleCommand             (RGV: 포트→포트)  Vehicle.IO_FROM/TO(00,00,포트), IO_PALLET_ID
  ⇒ Write
     SC#1(VEHICLE:11): FROM W0300~0302 ⇒ %DW768~770, TO W0303~0305 ⇒ %DW771~773, PALLET_ID W0306 ⇒ %DW774 (LEN 2워드)
       (SC#2~#5 는 W0310/0320/0330 대씩 증가)
     RGV(VEHICLE:1): FROM W0350~0352 ⇒ %DW848~850, TO W0353/0354/0345 ⇒ %DW851/852/837, PALLET_ID W0356 ⇒ %DW854
```
수동(팝업) 경로도 동일 태그를 쓴다:
```
StackerForm/RGVForm/LiftForm.buttonRetry_Click (명령 재전송)
  → vehicleObject.IO_PALLET_ID / IO_FROM_01~03 / IO_TO_01~03  ⇒ Write (위와 동일 주소)
```

### 2. C/V 방향(입·출고 모드)
```
ECSDispatcher.DoWork                  → cnv.Observables["DIRECTION_MODE"].AsString = CONVEYOR11_MODE  (C/V#11 고정 모드)
ECSDispatcher.ExecuteCommandRackToConveyor → toConveyor...["DIRECTION_MODE"] = "0"   (출고 방향)
ECSDispatcher.ExecuteVehicleCommand        → toConveyor...["DIRECTION_MODE"] = "1"   (입고 방향)
  ⇒ Write  C/V#n DIRECTION_MODE = W01E0+ (예: C/V#11 W01EA ⇒ %DW490, %DB980)   ← D0300+(n-1) 계열
```

---

## M영역 (비트) 기록

### 1. 반송 요청 트리거
```
ECSDispatcher.ExecuteCommand* / 폼 buttonRetry_Click
  → Vehicle.IO_TRANSFER_REQUEST = true
  ⇒ Write  SC#1 B0600 ⇒ %MX1536 (%MW96 비트0) / RGV B0650 ⇒ %MX1616 (%MW101 비트0)
```
(FROM/TO/PALLET_ID 를 D영역에 먼저 쓰고, 마지막에 이 비트를 올려 PLC 가 반송을 개시)

### 2. S/C·RGV 핸드셰이크 ACK (PLC 보고 비트 → ECS 응답 비트)
```
PLC가 보고 비트 세트 → Observable 이벤트(__IO_XXX_ObservableValueChanged) →
Vehicle.OnLoadComplete        → IO_LOAD_COMPLETE_ACK = true        ⇒ SC#1 B0601 ⇒ %MX1537 / RGV B0651 ⇒ %MX1617
Vehicle.OnUnloadComplete      → IO_UNLOAD_COMPLETE_ACK = true      ⇒ SC#1 B0602 ⇒ %MX1538 / RGV B0652 ⇒ %MX1618
Vehicle.OnAlarmSetReport      → IO_ALARM_SET_REPORT_ACK = true     ⇒ SC#1 B0603 ⇒ %MX1539 / RGV B0653 ⇒ %MX1619
Vehicle.OnAlarmResetReport    → IO_ALARM_RESET_REPORT_ACK = true   ⇒ SC#1 B0604 ⇒ %MX1540 / RGV B0654 ⇒ %MX1620
```

### 3. C/V 핸드셰이크 ACK
```
PLC 보고(LOAD/UNLOAD_COMPLETE_01/02) → Observable 이벤트 →
Conveyor.OnLoadComplete01   → observables["LOAD_COMPLETE_ACK_01"] = true   ⇒ C/V#11 B05A2 ⇒ %MX1442 (%MW90 비트2)
Conveyor.OnLoadComplete02   → observables["LOAD_COMPLETE_ACK_02"] = true   ⇒ C/V#11 B05A4 ⇒ %MX1444 (비트4)
Conveyor.OnUnloadComplete01 → observables["UNLOAD_COMPLETE_ACK_01"] = true ⇒ C/V#11 B05A1 ⇒ %MX1441 (비트1)
Conveyor.OnUnloadComplete02 → observables["UNLOAD_COMPLETE_ACK_02"] = true ⇒ C/V#11 B05A3 ⇒ %MX1443 (비트3)
Conveyor.OnUnloadRequestAck02 → Observables["UNLOAD_REQUEST_ACK_02"] = false (응답 해제)  ⇒ C/V#11 B0249 ⇒ %MX585
                              → Observables["PALLET_EXIST_FLAG_03"] = true/false          ⇒ C/V#11 B0252 ⇒ %MX594
```
(C/V ACK 는 M0800 대(=%MX1280~) 블록으로, 호기마다 16비트씩 증가. #11 은 %MW90 워드)

---

## R영역 (트래킹) 기록

```
ECSDispatcher.ExecuteCommandConveyor  → conveyor.SetPallet(fromPort, transfer.Palletid)
Gui ConveyorForm(수동 팔레트 기록)     → conveyorObject.SetPallet(port, writePalletId)
  → Conveyor.SetPallet
     → this.IO_PALLET_EXIST01~05 = palletId   (06~10 은 주석 처리됨)
  ⇒ Write  C/V#11 PALLET_EXIST01 = R0100(10진) ⇒ %RB200, PALLET_EXIST02 = R0102 ⇒ %RB204 ... (포지션당 2워드=화물번호 4자리)
```
(트랙별 시작 = 트랙번호×10: C/V#10=R0100, C/V#13=R0130 ...)

---

## 기타

- **FormTest.cs** (34건): 개발용 테스트 폼 — 위 태그들을 수동으로 조작하는 실험 코드(운영 경로 아님).
- ACK 비트의 해제(false)는 대부분 PLC 측이 보고 비트를 내리면 프레임워크/핸들러가 따라 내리는 구조이며,
  명시적 false 기록은 `UNLOAD_REQUEST_ACK_02` 한 곳에서 확인됨.
- 신 WCS 대응: 위 지시(D/M) 기록은 IO_TASK(cThread_SCH)의 `DriveSC/DriveCV/DriveRGV`(*_OD 발행) +
  EQP_TASK(VehThread.ConsumeCommands / CvThread)의 PLC 전송으로 이관되었고, 트래킹(R)은 CvThread.CvTrackingWrite 가 담당한다.
