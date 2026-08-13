-- =====================================================================
-- 원본 ECS 설비 변수 ↔ 대체 시스템 테이블 필드 정렬 (LGLS_MCS_IO)
--
-- 근거: 원본 ECS(D:\project\LGLS\LGLS_1WH_original\Backup\ECS) Device 계층 분석
--   - CV  : Conveyor.cs 옵저버블 25종 (컨베이어 단위 + 포트 단위)
--   - RGV/SC : Vehicle.cs 옵저버블 29종 (VEHICLE:1 = RGV, VEHICLE:11~15 = S/C — 동일 인터페이스)
--   주소 원본: EQP_SIM\observables.tsv (TB_OBSERVABLE 524점)
--
-- 원칙:
--   * 기존 컬럼과 의미가 일치하는 변수는 기존 컬럼 재사용 (매핑은 docs\ECS_EQUIPMENT_FIELD_MAP.md)
--   * 대응이 없는 변수만 ECS 변수명 기반 snake_case 로 추가 (_rd=PLC→ECS 판독, _od=ECS→PLC 기록)
--   * 비트=VARCHAR(1) '0'/'1', 2문자 위치코드=VARCHAR(2), 파렛트ID=VARCHAR(4), 알람코드=VARCHAR(4)
--   * RGV/SC 는 ECS 가 동일 Vehicle 로 다루므로 rtv_data/sc_data 에 같은 컬럼 세트 추가
--
-- 실행: sqlcmd -S localhost\sqlexpress -U LGLS_IO -P LGLS_IO -d LGLS_MCS_IO -i 본파일 -f 65001
-- =====================================================================
SET NOCOUNT ON;

-- ---------------------------------------------------------------------
-- 1) cv_data — Conveyor 옵저버블 (행 = 포트/TR)
--    재사용: OPERATION_MODE→auto_mode_rd, PALLET_EXIST_FLAG_0n→sensor0_data_rd,
--            PALLET_EXIST0n(ID)→lugg_no_rd, 에러→error_code
-- ---------------------------------------------------------------------
IF COL_LENGTH('cv_data','direction_mode_rd')     IS NULL ALTER TABLE cv_data ADD direction_mode_rd     VARCHAR(1) DEFAULT '0';  -- DIRECTION_MODE (W, 판독) 0=RGVINGO 1=RGVOUTGO
IF COL_LENGTH('cv_data','direction_mode_od')     IS NULL ALTER TABLE cv_data ADD direction_mode_od     VARCHAR(1) DEFAULT '';   -- DIRECTION_MODE (W, 기록 — 디스패치 시 방향 전환)
IF COL_LENGTH('cv_data','wait_in_rd')            IS NULL ALTER TABLE cv_data ADD wait_in_rd            VARCHAR(1) DEFAULT '0';  -- WAIT_IN (B) 입고 대기
IF COL_LENGTH('cv_data','wait_out_rd')           IS NULL ALTER TABLE cv_data ADD wait_out_rd           VARCHAR(1) DEFAULT '0';  -- WAIT_OUT (B) 출고 대기(반송완료 트리거)
IF COL_LENGTH('cv_data','in_ready_rd')           IS NULL ALTER TABLE cv_data ADD in_ready_rd           VARCHAR(1) DEFAULT '0';  -- IN_READY_02 (B) 입고 준비완료
IF COL_LENGTH('cv_data','load_complete_rd')      IS NULL ALTER TABLE cv_data ADD load_complete_rd      VARCHAR(1) DEFAULT '0';  -- LOAD_COMPLETE_0n (B)
IF COL_LENGTH('cv_data','load_complete_ack_od')  IS NULL ALTER TABLE cv_data ADD load_complete_ack_od  VARCHAR(1) DEFAULT '0';  -- LOAD_COMPLETE_ACK_0n (B)
IF COL_LENGTH('cv_data','unload_complete_rd')    IS NULL ALTER TABLE cv_data ADD unload_complete_rd    VARCHAR(1) DEFAULT '0';  -- UNLOAD_COMPLETE_0n (B)
IF COL_LENGTH('cv_data','unload_complete_ack_od')IS NULL ALTER TABLE cv_data ADD unload_complete_ack_od VARCHAR(1) DEFAULT '0'; -- UNLOAD_COMPLETE_ACK_0n (B)
IF COL_LENGTH('cv_data','unload_request_od')     IS NULL ALTER TABLE cv_data ADD unload_request_od     VARCHAR(1) DEFAULT '0';  -- UNLOAD_REQUEST_02 (B, ECS→PLC)
IF COL_LENGTH('cv_data','unload_request_ack_rd') IS NULL ALTER TABLE cv_data ADD unload_request_ack_rd VARCHAR(1) DEFAULT '0';  -- UNLOAD_REQUEST_ACK_02 (B)
GO

UPDATE cv_data SET
    direction_mode_rd      = '0',   -- 초기 입고 방향 (C/V#11 은 WMS M 전문 mode 0 으로 복귀되는 값)
    direction_mode_od      = '',
    wait_in_rd             = '0',
    wait_out_rd            = '0',
    in_ready_rd            = CASE WHEN stn_kind IN ('1','3') THEN '1' ELSE '0' END,  -- 입고대는 준비완료
    load_complete_rd       = '0',
    load_complete_ack_od   = '0',
    unload_complete_rd     = '0',
    unload_complete_ack_od = '0',
    unload_request_od      = '0',
    unload_request_ack_rd  = '0'
 WHERE wh_typ = '10';
GO

-- ---------------------------------------------------------------------
-- 2) rtv_data — Vehicle 옵저버블 (VEHICLE:1 = RGV)
--    재사용: OPERATION_MODE→auto_mode_rd, PALLET_EXIST_FLAG→sensor_rtv_rd,
--            ALARM 상태 요약→is_error_rd/err_code_rd
-- ---------------------------------------------------------------------
IF COL_LENGTH('rtv_data','subsystem_status_rd')     IS NULL ALTER TABLE rtv_data ADD subsystem_status_rd     VARCHAR(1) DEFAULT '1';  -- SUBSYSTEM_STATUS (W) 0=DOWN 1=IDLE 2=RUN
IF COL_LENGTH('rtv_data','location_01_rd')          IS NULL ALTER TABLE rtv_data ADD location_01_rd          VARCHAR(2) DEFAULT '';   -- SUBSYSTEM_LOCATION_01 (W)
IF COL_LENGTH('rtv_data','location_02_rd')          IS NULL ALTER TABLE rtv_data ADD location_02_rd          VARCHAR(2) DEFAULT '';
IF COL_LENGTH('rtv_data','location_03_rd')          IS NULL ALTER TABLE rtv_data ADD location_03_rd          VARCHAR(2) DEFAULT '';
IF COL_LENGTH('rtv_data','pallet_on_vehicle_rd')    IS NULL ALTER TABLE rtv_data ADD pallet_on_vehicle_rd    VARCHAR(4) DEFAULT '';   -- PALLET_ON_VEHICLE (R, 4자 ASCII)
IF COL_LENGTH('rtv_data','transfer_request_od')     IS NULL ALTER TABLE rtv_data ADD transfer_request_od     VARCHAR(1) DEFAULT '0';  -- TRANSFER_REQUEST (B, ECS→PLC)
IF COL_LENGTH('rtv_data','transfer_ack_rd')         IS NULL ALTER TABLE rtv_data ADD transfer_ack_rd         VARCHAR(1) DEFAULT '0';  -- TRANSFER_ACK (B)
IF COL_LENGTH('rtv_data','pallet_id_od')            IS NULL ALTER TABLE rtv_data ADD pallet_id_od            VARCHAR(4) DEFAULT '';   -- PALLET_ID (W 2w, 지시 파렛트)
IF COL_LENGTH('rtv_data','from_01_od')              IS NULL ALTER TABLE rtv_data ADD from_01_od              VARCHAR(2) DEFAULT '';   -- FROM_01~03 (W, 6자 위치코드 2자씩)
IF COL_LENGTH('rtv_data','from_02_od')              IS NULL ALTER TABLE rtv_data ADD from_02_od              VARCHAR(2) DEFAULT '';
IF COL_LENGTH('rtv_data','from_03_od')              IS NULL ALTER TABLE rtv_data ADD from_03_od              VARCHAR(2) DEFAULT '';
IF COL_LENGTH('rtv_data','to_01_od')                IS NULL ALTER TABLE rtv_data ADD to_01_od                VARCHAR(2) DEFAULT '';   -- TO_01~03 (W, CV행선은 TO_03=포트번호)
IF COL_LENGTH('rtv_data','to_02_od')                IS NULL ALTER TABLE rtv_data ADD to_02_od                VARCHAR(2) DEFAULT '';
IF COL_LENGTH('rtv_data','to_03_od')                IS NULL ALTER TABLE rtv_data ADD to_03_od                VARCHAR(2) DEFAULT '';
IF COL_LENGTH('rtv_data','load_complete_rd')        IS NULL ALTER TABLE rtv_data ADD load_complete_rd        VARCHAR(1) DEFAULT '0';  -- LOAD_COMPLETE (B)
IF COL_LENGTH('rtv_data','load_complete_ack_od')    IS NULL ALTER TABLE rtv_data ADD load_complete_ack_od    VARCHAR(1) DEFAULT '0';  -- LOAD_COMPLETE_ACK (B)
IF COL_LENGTH('rtv_data','unload_complete_rd')      IS NULL ALTER TABLE rtv_data ADD unload_complete_rd      VARCHAR(1) DEFAULT '0';  -- UNLOAD_COMPLETE (B, 반송 완료 처리 트리거)
IF COL_LENGTH('rtv_data','unload_complete_ack_od')  IS NULL ALTER TABLE rtv_data ADD unload_complete_ack_od  VARCHAR(1) DEFAULT '0';  -- UNLOAD_COMPLETE_ACK (B)
IF COL_LENGTH('rtv_data','alarm_set_report_rd')     IS NULL ALTER TABLE rtv_data ADD alarm_set_report_rd     VARCHAR(1) DEFAULT '0';  -- ALARM_SET_REPORT (B)
IF COL_LENGTH('rtv_data','alarm_set_report_ack_od') IS NULL ALTER TABLE rtv_data ADD alarm_set_report_ack_od VARCHAR(1) DEFAULT '0';  -- ALARM_SET_REPORT_ACK (B)
IF COL_LENGTH('rtv_data','alarm_set_code_rd')       IS NULL ALTER TABLE rtv_data ADD alarm_set_code_rd       VARCHAR(4) DEFAULT '0';  -- ALARM_SET_CODE (W)
IF COL_LENGTH('rtv_data','alarm_reset_report_rd')   IS NULL ALTER TABLE rtv_data ADD alarm_reset_report_rd   VARCHAR(1) DEFAULT '0';  -- ALARM_RESET_REPORT (B)
IF COL_LENGTH('rtv_data','alarm_reset_report_ack_od') IS NULL ALTER TABLE rtv_data ADD alarm_reset_report_ack_od VARCHAR(1) DEFAULT '0'; -- ALARM_RESET_REPORT_ACK (B)
IF COL_LENGTH('rtv_data','alarm_reset_code_rd')     IS NULL ALTER TABLE rtv_data ADD alarm_reset_code_rd     VARCHAR(4) DEFAULT '0';  -- ALARM_RESET_CODE (W)
GO

UPDATE rtv_data SET
    subsystem_status_rd = '1',      -- IDLE
    location_01_rd = '', location_02_rd = '', location_03_rd = '',
    pallet_on_vehicle_rd = '',
    transfer_request_od = '0', transfer_ack_rd = '0',
    pallet_id_od = '',
    from_01_od = '', from_02_od = '', from_03_od = '',
    to_01_od = '', to_02_od = '', to_03_od = '',
    load_complete_rd = '0', load_complete_ack_od = '0',
    unload_complete_rd = '0', unload_complete_ack_od = '0',
    alarm_set_report_rd = '0', alarm_set_report_ack_od = '0', alarm_set_code_rd = '0',
    alarm_reset_report_rd = '0', alarm_reset_report_ack_od = '0', alarm_reset_code_rd = '0'
 WHERE wh_typ = '10';
GO

-- ---------------------------------------------------------------------
-- 3) sc_data — Vehicle 옵저버블 (VEHICLE:11~15 = S/C#1~5, RGV 와 동일 세트)
--    재사용: OPERATION_MODE→auto_mode_rd, SUBSYSTEM_STATUS 요약→ucstatus_rd,
--            PALLET_EXIST_FLAG→sensor_fk_rd, PALLET_ID(od)→lugg_no_fk1_od,
--            FROM/TO(뱅크·베이·단)→start/dest_bank/bay/level_fk1_od, 알람→err_code_rd
-- ---------------------------------------------------------------------
IF COL_LENGTH('sc_data','subsystem_status_rd')     IS NULL ALTER TABLE sc_data ADD subsystem_status_rd     VARCHAR(1) DEFAULT '1';
IF COL_LENGTH('sc_data','location_01_rd')          IS NULL ALTER TABLE sc_data ADD location_01_rd          VARCHAR(2) DEFAULT '';
IF COL_LENGTH('sc_data','location_02_rd')          IS NULL ALTER TABLE sc_data ADD location_02_rd          VARCHAR(2) DEFAULT '';
IF COL_LENGTH('sc_data','location_03_rd')          IS NULL ALTER TABLE sc_data ADD location_03_rd          VARCHAR(2) DEFAULT '';
IF COL_LENGTH('sc_data','pallet_on_vehicle_rd')    IS NULL ALTER TABLE sc_data ADD pallet_on_vehicle_rd    VARCHAR(4) DEFAULT '';
IF COL_LENGTH('sc_data','transfer_request_od')     IS NULL ALTER TABLE sc_data ADD transfer_request_od     VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','transfer_ack_rd')         IS NULL ALTER TABLE sc_data ADD transfer_ack_rd         VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','pallet_id_od')            IS NULL ALTER TABLE sc_data ADD pallet_id_od            VARCHAR(4) DEFAULT '';
IF COL_LENGTH('sc_data','from_01_od')              IS NULL ALTER TABLE sc_data ADD from_01_od              VARCHAR(2) DEFAULT '';
IF COL_LENGTH('sc_data','from_02_od')              IS NULL ALTER TABLE sc_data ADD from_02_od              VARCHAR(2) DEFAULT '';
IF COL_LENGTH('sc_data','from_03_od')              IS NULL ALTER TABLE sc_data ADD from_03_od              VARCHAR(2) DEFAULT '';
IF COL_LENGTH('sc_data','to_01_od')                IS NULL ALTER TABLE sc_data ADD to_01_od                VARCHAR(2) DEFAULT '';
IF COL_LENGTH('sc_data','to_02_od')                IS NULL ALTER TABLE sc_data ADD to_02_od                VARCHAR(2) DEFAULT '';
IF COL_LENGTH('sc_data','to_03_od')                IS NULL ALTER TABLE sc_data ADD to_03_od                VARCHAR(2) DEFAULT '';
IF COL_LENGTH('sc_data','load_complete_rd')        IS NULL ALTER TABLE sc_data ADD load_complete_rd        VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','load_complete_ack_od')    IS NULL ALTER TABLE sc_data ADD load_complete_ack_od    VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','unload_complete_rd')      IS NULL ALTER TABLE sc_data ADD unload_complete_rd      VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','unload_complete_ack_od')  IS NULL ALTER TABLE sc_data ADD unload_complete_ack_od  VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','alarm_set_report_rd')     IS NULL ALTER TABLE sc_data ADD alarm_set_report_rd     VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','alarm_set_report_ack_od') IS NULL ALTER TABLE sc_data ADD alarm_set_report_ack_od VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','alarm_set_code_rd')       IS NULL ALTER TABLE sc_data ADD alarm_set_code_rd       VARCHAR(4) DEFAULT '0';
IF COL_LENGTH('sc_data','alarm_reset_report_rd')   IS NULL ALTER TABLE sc_data ADD alarm_reset_report_rd   VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','alarm_reset_report_ack_od') IS NULL ALTER TABLE sc_data ADD alarm_reset_report_ack_od VARCHAR(1) DEFAULT '0';
IF COL_LENGTH('sc_data','alarm_reset_code_rd')     IS NULL ALTER TABLE sc_data ADD alarm_reset_code_rd     VARCHAR(4) DEFAULT '0';
GO

UPDATE sc_data SET
    subsystem_status_rd = '1',      -- IDLE
    location_01_rd = '', location_02_rd = '', location_03_rd = '',
    pallet_on_vehicle_rd = '',
    transfer_request_od = '0', transfer_ack_rd = '0',
    pallet_id_od = '',
    from_01_od = '', from_02_od = '', from_03_od = '',
    to_01_od = '', to_02_od = '', to_03_od = '',
    load_complete_rd = '0', load_complete_ack_od = '0',
    unload_complete_rd = '0', unload_complete_ack_od = '0',
    alarm_set_report_rd = '0', alarm_set_report_ack_od = '0', alarm_set_code_rd = '0',
    alarm_reset_report_rd = '0', alarm_reset_report_ack_od = '0', alarm_reset_code_rd = '0'
 WHERE wh_typ = '10' AND sc_no IN ('901','902','903','904','905');
GO

-- ---------------------------------------------------------------------
-- 결과 확인
-- ---------------------------------------------------------------------
SELECT 'cv_data'  t, COUNT(*) rows_upd, COUNT(CASE WHEN wait_in_rd IS NOT NULL THEN 1 END) filled FROM cv_data  WHERE wh_typ='10'
UNION ALL
SELECT 'rtv_data', COUNT(*), COUNT(CASE WHEN subsystem_status_rd IS NOT NULL THEN 1 END) FROM rtv_data WHERE wh_typ='10'
UNION ALL
SELECT 'sc_data 901~905', COUNT(*), COUNT(CASE WHEN subsystem_status_rd IS NOT NULL THEN 1 END) FROM sc_data WHERE wh_typ='10' AND sc_no IN ('901','902','903','904','905');
