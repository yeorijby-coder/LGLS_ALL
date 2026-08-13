# 구 ECS ↔ 신규 WCS 기능 커버리지 대조 (2026-07-18)

> 작성 근거: `Backup`(구 ECS = HECS/ECP) 소스와 WCS 시스템(HOST_TASK/EQP_TASK/IO_TASK/WCS Client) 소스 전수 대조.
> 신규 WCS는 재고(인벤토리) 관련 기능을 의도적으로 제외한다.

## 0. 시스템 구조 요약

| 구분 | 구 ECS (Backup) | 신규 WCS |
|---|---|---|
| WMS/IMS 통신 | `ECP` (ExternalIFManager + WMSCommand) — HECS 프로세스 내장 | **HOST_TASK** = `TASK\WCS_TASK_HOST` (별도 프로세스) |
| 배차/반송 로직 | `ECS\Device\Dispatcher\ECSDispatcher.cs` — HECS 내장 | **스케줄러** = `TASK\IO_TASK\CLS\Thread\cThread_SCH.cs` (DB 폴링) |
| PLC 통신 | `ECP\Com\FieldBus\Fenet\FenetDriver.cs` — HECS 내장 | **EQP_TASK** = `TASK\WCS_TASK_CV_BIN` (CvThread + FenetProtocol) |
| GUI/모니터링 | `ECS\Gui\*` (WinForms) — HECS 내장 | **WCS Client** = `WCS\CPlusPlus\LGLS_CLIENT\Ecs` (MFC) |
| 재고/셀/파렛트 | `ECP\Db\Entity\PalletData/HirackData` + HiRack 화면 | **의도적 제외** |

핵심 아키텍처 차이: 구 ECS는 단일 프로세스가 모든 계층을 인메모리 옵저버블로 통합, 신규 WCS는 4개 프로세스가 공통 DB(LGLS_MCS_IO) 폴링으로 느슨하게 결합. 아래 "부분구현" 다수의 근본 원인.

## 1. WMS 전문 처리

| 기능(전문) | 구 ECS 구현 위치 | WCS 구현 위치 | 상태 | 비고 |
|---|---|---|---|---|
| O 작업지시 수신 | WMSCommand.cs (body 66B) | CSrvWork.cs `ParseOorR` | 구현됨 | LGLS 66B 재배치 |
| R 재작업지시 수신 | WMSCommand.cs | CSrvWork.cs `ParseOorR` | 구현됨 | |
| M 모드변경 수신 | ExternalIFManager `checkMode` | CSrvWork.cs `ParseModeChange` | **부분구현** | ACK만 회신, 방향워드(CV_DATA) 실기록 미구현 |
| D 작업삭제 수신 | (LFC 계열) | CSrvWork.cs `ParseCancel` | 구현됨 | |
| ACK/NAK 응답 | WMSCommand.cs `checkMessage()` | CSrvWork.cs `MakeResponse` | 구현됨 | |
| S 상태보고 송신 | MakeWmsIfString `insertStatus` (43B 집계, 1초) | CCliWork.cs `GetStatusReport` (설비별, 30초+변경시) | **부분구현** | 형식/주기 상이 — HOST_SIM은 무해, 실 WMS 연동 시 확인 필요 |
| E 에러보고 송신 | MakeWmsIfString `insertError` (23B) | CCliWork.cs `GetErrorReport` | 구현됨 | WCS는 24B(WhDefine 포함) |
| F 작업완료보고 송신 | MakeWmsIfString `insertTransferComplete` (10B) | CCliWork.cs `GetJobCompleteReport` | 구현됨 | 출고 최종 Step '2' |
| A 반입도착보고 | (LFC 계열) | CCliWork.cs `GetLoadArrivalReport` | 구현됨 | |
| U 무게보고 | (HECS 미구현) | GetWeightReport 존재하나 호출 주석 | **재고제외** | GetSendData L701 |
| P 빈파렛트 요청/콜 | (HECS 미구현) | GetEmptyPltRequest/ParsePallet 주석 | **재고제외** | L702 / L423 복원 지점 |
| B 바코드 | — | ParseBcrData 주석 | 미사용 | |

> 구 HECS WMSCommand는 O/M/R/S/E/F/ACK/NAK 6종만 정의. U/P/B/A/D는 WCS_TASK_HOST가 재활용한 LFC ECSCOM 프로토콜 슈퍼셋 유래.

## 2. 배차/반송 로직

| 기능 | 구 ECS | WCS | 상태 | 비고 |
|---|---|---|---|---|
| 신규작업 접수/배차 | ECSDispatcher `DoWork`/`IsExecutableTransfer` | cThread_SCH `AcceptNewJob` (99→10/20) | 구현됨 | |
| 입고 CV 레그 | `ExecuteCommandConveyor` | `DriveCV`/`RunCV`/`CompleteCV`/`FeedInGate` | 구현됨 | |
| 출고 SC 레그 | `ExecuteCommandStockerSEM` 등 | `DriveSC`/`AutoRunSC` (SC_AUTO_COMPLETE) | **부분구현** | SC PLC 게이트웨이 부재 → DB 타이머 자동완주 |
| 멀티레그 핸드오프 | `ExecuteVehicleCommand` + itinerary | 상태전이 10→11→20→29 / 20→…→19 | **부분구현** | 입고 RGV 구현, **출고 RGV 레그 미완** |
| RGV 구동 | `ExecuteVehicleCommand` | `AutoRunRGV`/`UpdateRtvVehicle` | **부분구현** | RGV 게이트웨이 부재 → DB 애니메이션 |
| 방향워드 전환 | ECSDispatcher L1303-1310 + M전문 복귀 | `WriteCvSensor`/direction_mode_od | **부분구현** | M전문→워드 기록 미연계 |
| 트래킹 Shift/R영역 | ECSDispatcher L1294-1300 | IO_TASK from/to_od + EQP_TASK `GetRTrackingAddr`/`EncodeJobNoR` | 구현됨(실검증) | |
| Wait-Out 처리 | Conveyor.cs `OnWaitOut` (1회성) | FeedInGate + EQP_SIM 훅 | **부분구현** | 실 PLC 연동 시 재시도 확인 필요 |
| Ack 핸드셰이크 | Conveyor.cs L984-1008 | EQP_TASK CvThread Ack (실검증) | 구현됨 | |
| 상태머신 | TranferWorkItem/TransferState | `UpdateJobStatus` ST_* | 구현됨 | 등가 |
| 스케줄/중복/이동규칙 | TransferScheduleTime/CalculationMoveRule | 폴링 + 중복체크만 | **부분구현** | MoveRule 미이식 |
| MOVE(6)/R2R(4)/피킹(3) | IsValidStation 패턴 분기 | AcceptNewJob JOB_TYP 1/2만 (L319-324 "미지원") | **미구현** | |

## 3. 설비 상태 관리

| 기능 | 구 ECS | WCS | 상태 | 비고 |
|---|---|---|---|---|
| Conveyor 옵저버블 25종 | Device\Unit\Conveyor.cs | cv_data + EQP_TASK 브리지 | 구현됨 | align_equipment_fields_ecs_mssql.sql |
| Vehicle(RGV/SC) 29종 | Device\Unit\Vehicle.cs | rtv_data/sc_data + IO_TASK | **부분구현** | 게이트웨이 부재로 DB 직접 갱신 |
| 서브시스템 상태 | AbstractSubSystem 등 | subsystem_status_rd | 구현됨 | |
| 알람 감지/E보고 | CheckEquipmentStatus + AlarmManager | MonitorAlarm/CheckAlarm + IsEquip_ERROR_Modified | 구현됨 | |
| 통신상태 판정 | CommunicationStateControl | connected_yn/upd_dt + 상태바 | 구현됨 | |
| AGV/Lift | AgvSEM/LiftSEM | — | 해당없음 | LGLS 구성 외 |

## 4. PLC 통신 (실검증 완료 — EQP_TASK 통신부 수정 금지)

XGT FEnet 프레임(0x54/0x58, %MB/%DB/%RB/%MX, 주소×2), R영역 트래킹 실주소, BCD ASCII 팩 모두 구현·실PLC 검증. Melsec 보조, SECS/GEM은 대상 외.

## 5. GUI / 수동 조작 (WCS Client)

메인 모니터링(레이아웃/화물 애니메이션), CV/SC/RGV 팝업, 수동 반송지시(ManualJob/Sc/Rtv), 강제완료, 명령재전송, 일시정지/suspend, 에러해제, CV 쓰기/삭제/복사/붙여넣기, 로그인/권한, 상태바 — 모두 구현·수정 이력 있음.
재고 계열(하이랙 셀 모니터링, 빈파렛트)은 뷰 골격만(재고제외). BCR 미사용.

## 6. 기타

| 기능 | 상태 | 비고 |
|---|---|---|
| 에러/반송 이력 조회 화면 | **부분구현** | host_if_log/wc_his에 데이터만 쌓임, 조회 UI 미이식 |
| 통계(가동률/에러 Collector) | **미구현** | 필요 시 신규 개발 |
| SAP I/F | 해당없음 | |
| 이동규칙 자동 목적지 | **미구현** | 현재 O 전문이 목적지 명시라 당장 불요 |

## 7. 미구현/확인 필요 항목 (우선순위)

### B. 실동작 영향 있는 공백
1. **출고 RGV 레그(라인CV→게이트웨이 배출) 미완** — 게이트웨이(C/V#11 121/122)는 EQP_SIM 소유, 라인/크레인은 IO_TASK 소유라는 소유권 분리가 근본 원인 (REPLACEMENT_SYSTEMS.md §12.4·16.2·20·21). 해결안: IO_TASK가 게이트웨이 입출고 단일 소유 + 입고 CV레그 직접 구동 (상당한 재설계).
2. **M 모드변경 → cv_data.direction_mode_od 기록 추가** (CSrvWork.cs L668 부근) + EQP_TASK가 %DB980에 기록 연계.
3. **JOB_TYP 4(R2R)/6(MOVE) 접수 분기** — cThread_SCH.AcceptNewJob에 추가 (입출고 위주 운영이면 낮은 우선순위).

### C. 운영 보조 (낮음)
4. 통계 모듈, 5. 이력 조회 UI, 6. 이동규칙, 7. S 상태보고 43B 집계 형식(실 WMS 연동 시), 8. Wait-Out 재시도(실 PLC 연동 시).

### A. 재고제외(조치 불요, 복원 지점만 기록)
- U/P 전문: GetSendData L701-702 주석 해제 + CSrvWork L423 복원
- 하이랙 셀/파렛트 재고: 재고 도입 시 신규 설계

**요약**: 핵심 온라인 반송 경로(O/R/M 수신 → 입출고 배차 → PLC 트래킹/Ack → F/S/E 보고)는 실사이클 커버. 최대 공백은 출고 RGV 게이트웨이 레그.
