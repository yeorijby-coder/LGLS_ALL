# 원본 ECS 설비 변수 ↔ 대체 시스템 테이블 필드 매핑

- 대상 DB: `localhost\SQLEXPRESS` / LGLS_MCS_IO (적용 스크립트: `db\align_equipment_fields_ecs_mssql.sql`)
- 근거: 원본 ECS `Device\Unit\Conveyor.cs` / `Vehicle.cs` / `ECSDispatcher.cs` + `EQP_SIM\observables.tsv`(TB_OBSERVABLE 524점)
- 규칙: `_rd` = PLC→ECS 판독(상태), `_od` = ECS→PLC 기록(지시/응답). 비트=VARCHAR(1), 2자 위치코드=VARCHAR(2), 파렛트ID=VARCHAR(4)
- ECS 는 RGV(VEHICLE:1)와 S/C(VEHICLE:11~15)를 **동일한 Vehicle 인터페이스(29종)** 로 다루므로
  rtv_data / sc_data 에 같은 컬럼 세트를 적용했다.

## 1. CV — Conveyor 옵저버블 25종 → cv_data (행 = 포트/TR)

ECS 는 컨베이어 단위(모드/방향/대기) + 포트 단위(재실/완료, `_01~_05` 접미사)로 나누지만,
cv_data 는 행이 곧 포트(TR)이므로 포트 접미사는 행으로 흡수된다.
(예: C/V#11 의 LOAD_COMPLETE_02 → MC_NO '122' 행의 load_complete_rd)

| ECS Observable | 영역 (CONVEYOR:10 예) | 방향 | cv_data 컬럼 | 비고 |
|---|---|---|---|---|
| OPERATION_MODE | B 0228 | 판독 | **auto_mode_rd** (기존) | 0=수동 1=자동 |
| DIRECTION_MODE | W 01E9 | 판독+기록 | **direction_mode_rd / direction_mode_od** (추가) | 0=RGVINGO 1=RGVOUTGO — 문서와 의미 반전 주의(검증보고 §차이1) |
| WAIT_IN | B 0225 | 판독 | **wait_in_rd** (추가) | 입고 대기 |
| WAIT_OUT | B 0226 | 판독 | **wait_out_rd** (추가) | 출고 대기 — 반송완료 트래킹 트리거 |
| IN_READY_02 | B 0227 | 판독 | **in_ready_rd** (추가) | 입고 준비완료 (입고대 행) |
| LOAD_COMPLETE_01/02 | B 0222/0224 | 판독 | **load_complete_rd** (추가) | 적재완료 |
| LOAD_COMPLETE_ACK_01/02 | B 0592/0594 | 기록 | **load_complete_ack_od** (추가) | 적재완료 응답 |
| UNLOAD_COMPLETE_01/02 | B 0221/0223 | 판독 | **unload_complete_rd** (추가) | 불출완료 |
| UNLOAD_COMPLETE_ACK_01/02 | B 0591/0593 | 기록 | **unload_complete_ack_od** (추가) | 불출완료 응답 |
| UNLOAD_REQUEST_02 | B 0595 | 기록 | **unload_request_od** (추가) | 불출 요청(ECS→PLC) |
| UNLOAD_REQUEST_ACK_02 | B 0229 | 판독 | **unload_request_ack_rd** (추가) | 불출요청 응답 |
| PALLET_EXIST_FLAG_01~05 | B 0230~0234 | 판독 | **sensor0_data_rd** (기존) | 포트 파렛트 재실 |
| PALLET_EXIST01~05 (ID) | R 0090~0098 (2w) | 판독 | **lugg_no_rd** (기존) | 포트 파렛트 ID(4자 ASCII) |
| (에러) | — | 판독 | **error_code** (기존) | |

주소 스트라이드(참고): 상태 B 블록 = 0x121(CV2) +0x20/대, ACK B = 0x515(CV2) +0x10/대,
파렛트ID R = 0x010(CV2) +0x10/대, DIRECTION_MODE W = 0x1E1(CV2) +1/대.

초기 데이터: 전 행 '0'/'', 단 in_ready_rd 는 입고대(stn_kind 1/3)만 '1', direction_mode_rd='0'(입고 방향).

## 2. RGV / S/C — Vehicle 옵저버블 29종 → rtv_data / sc_data (행 = 호기)

| ECS Observable | 영역 (V:1 / V:11 예) | 방향 | rtv_data 컬럼 | sc_data 컬럼 | 비고 |
|---|---|---|---|---|---|
| SUBSYSTEM_STATUS | W 0150 / 0100 | 판독 | **subsystem_status_rd** (추가) | **subsystem_status_rd** (추가) | 0=DOWN 1=IDLE 2=RUN (sc 기존 ucstatus_rd 는 병행 유지) |
| OPERATION_MODE | W 0159 / 0109 | 판독 | **auto_mode_rd** (기존) | **auto_mode_rd** (기존) | |
| SUBSYSTEM_LOCATION_01~03 | W 0156~ / 0106~ | 판독 | **location_01~03_rd** (추가) | **location_01~03_rd** (추가) | 2자 코드 ×3 |
| PALLET_EXIST_FLAG | B 03A0 / 0300 | 판독 | **sensor_rtv_rd** (기존) | **sensor_fk_rd** (기존) | 재실 비트 |
| PALLET_ON_VEHICLE | R 0310 / 0300 (2w) | 판독 | **pallet_on_vehicle_rd** (추가) | **pallet_on_vehicle_rd** (추가) | 실적 파렛트 ID |
| TRANSFER_REQUEST | B 0650 / 0600 | 기록 | **transfer_request_od** (추가) | **transfer_request_od** (추가) | 지시 발화(기존 od_rq_yn 은 스케줄러 핸드셰이크로 병행) |
| TRANSFER_ACK | B 03B4 / 0314 | 판독 | **transfer_ack_rd** (추가) | **transfer_ack_rd** (추가) | |
| PALLET_ID | W 0356 / 0306 (2w) | 기록 | **pallet_id_od** (추가) | **pallet_id_od** (추가) | 기존 lugg_od / lugg_no_fk1_od 병행 |
| FROM_01~03 | W 0350~ / 0300~ | 기록 | **from_01~03_od** (추가) | **from_01~03_od** (추가) | 6자 위치코드 2자 분할 |
| TO_01~03 | W 0353~ / 0303~ | 기록 | **to_01~03_od** (추가) | **to_01~03_od** (추가) | CV 행선은 TO_03=포트번호 |
| LOAD_COMPLETE | B 03B0 / 0310 | 판독 | **load_complete_rd** (추가) | **load_complete_rd** (추가) | |
| LOAD_COMPLETE_ACK | B 0651 / 0601 | 기록 | **load_complete_ack_od** (추가) | **load_complete_ack_od** (추가) | |
| UNLOAD_COMPLETE | B 03B1 / 0311 | 판독 | **unload_complete_rd** (추가) | **unload_complete_rd** (추가) | 반송 detail 완료 트리거 |
| UNLOAD_COMPLETE_ACK | B 0652 / 0602 | 기록 | **unload_complete_ack_od** (추가) | **unload_complete_ack_od** (추가) | |
| TRANSFER_COMPLETE_LOCATION_01~03 | W 0153~ / 0103~ | 판독 | (rtv 기존 **depart_track/arrive_track** 요약) | (sc 기존 dest_bank/bay/level_fk1_rd 요약) | 필요 시 추가 예정 — 현 체인 미사용 |
| ALARM_SET_REPORT | B 03B2 / 0312 | 판독 | **alarm_set_report_rd** (추가) | **alarm_set_report_rd** (추가) | |
| ALARM_SET_REPORT_ACK | B 0653 / 0603 | 기록 | **alarm_set_report_ack_od** (추가) | **alarm_set_report_ack_od** (추가) | |
| ALARM_SET_CODE | W 0151 / 0101 | 판독 | **alarm_set_code_rd** (추가, 기존 err_code_rd 병행) | **alarm_set_code_rd** (추가, 기존 err_code_rd 병행) | |
| ALARM_RESET_REPORT | B 03B3 / 0313 | 판독 | **alarm_reset_report_rd** (추가) | **alarm_reset_report_rd** (추가) | |
| ALARM_RESET_REPORT_ACK | B 0654 / 0604 | 기록 | **alarm_reset_report_ack_od** (추가) | **alarm_reset_report_ack_od** (추가) | |
| ALARM_RESET_CODE | W 0152 / 0102 | 판독 | **alarm_reset_code_rd** (추가) | **alarm_reset_code_rd** (추가) | |

주소 스트라이드(참고): S/C V:11→15 는 W 0x100/+0x10, FROM·TO W 0x300/+0x10, 상태 B 0x300/+0x20,
지시 B 0x600/+0x10, R 0x300/+2. RGV(V:1)는 별도 블록(W 0x150/0x350, B 0x3A0/0x650, R 0x310).

초기 데이터: subsystem_status_rd='1'(IDLE), 나머지 비트 '0', 문자열 ''.
대상: rtv_data RGV#1, sc_data 901~905 (구 사이트 잔여 906~909 는 제외).

## 3. 비고
- ECS 지시 세트(디스패처가 한 번에 쓰는 필드): PALLET_ID + FROM_01~03 + TO_01~03 → TRANSFER_REQUEST=1.
  응답/완료 핸드셰이크: TRANSFER_ACK, LOAD/UNLOAD_COMPLETE(+ACK), ALARM SET/RESET(+ACK).
- 랙 선반(CSTID)은 LGLS 에서 PLC 폴링이 아니라 DB(TB_PALLET) 관리 — 재고 관리 제외 방침에 따라 미이식.
- 현 대체 체인(WCS_TASK_CV/IO_TASK)은 기존 컬럼으로 동작 중이며, 추가 컬럼은 향후 SC/RGV
  PLC 게이트웨이 구현 시 원본 ECS 와 동일한 신호 체계로 바로 쓸 수 있도록 정렬해 둔 것.
