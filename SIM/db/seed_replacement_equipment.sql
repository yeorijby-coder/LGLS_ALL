-- =====================================================================
-- LGLS 설비 마스터 시드 (대체 시스템 DB: PostgreSQL LGLS_TEST)
--
-- 원본 ECS(TB_EQUIPMENT/TB_SUBSYSTEM/TB_LOCATION/TB_ITINERARY) 분석 결과를
-- 대체 시스템(WCS_TASK_HOST / IO_TASK / WCS_TASK_CV) 테이블 체계로 입력.
-- ※ 재고 관리(하이랙 셀/파렛트 재고)는 이번 범위에서 제외 — 관련 테이블 미시드.
--
-- 설비 구성 (원본 ECS 근거):
--   C/V #1~#15, 반송 포트 TR#1~#32 (TB_LOCATION: CONVEYOR:n / PORT:t)
--     입고대: TR#22(C/V#11 겸용), TR#24(C/V#12), TR#30(C/V#15)
--     출고대: TR#22(C/V#11 겸용), TR#26(C/V#13), TR#29(C/V#14)
--     라인:   C/V#2~#10 (RGV측 홀수 TR / S/C측 짝수 TR)
--   S/C #1~#5 (냉동 1,2호기 / 실온 3,4,5호기 구분 없이 뱅크 2k-1,2k 담당)
--   RGV #1
--
-- 대체 체계 표준 키:
--   cv_data.MC_NO   = '1' + TR번호 2자리   (TR#21 → '121')
--   cv_data.TRACK_NO= '1' + MC_NO 뒤 3자리 ('121' → '1121' 아님 → '1' + '021' = '1021')
--   sc_data.SC_NO   = '90' + 호기          (S/C#1 → '901')
--   WCS_TASK_CV D주소 = MC_NO 뒤 2자리 × 10 (TR#21 → D워드 210)
--   WCS_TASK_CV R트래킹 = hex((PLC_NO-1)*10) + (MC_NO-FR_TRACK)*2
-- =====================================================================
SET client_encoding TO 'UTF8';

-- ---------------------------------------------------------------------
-- 0) 기존 데이터 백업 후 재구성 (이전 사이트 트랙 모델 101~188 대체)
-- ---------------------------------------------------------------------
DROP TABLE IF EXISTS cv_data_bak_lgls;
CREATE TABLE cv_data_bak_lgls AS SELECT * FROM cv_data;
DELETE FROM cv_data;

-- ---------------------------------------------------------------------
-- 1) cv_data — 반송 포트(TR) 32개
--    stn_kind : bit0(1)=입고대, bit1(2)=출고대, 0=라인 포트
-- ---------------------------------------------------------------------
INSERT INTO cv_data (wh_typ, plc_no, track_no, mc_no, mc_no_nm, stn_kind,
                     lugg_no_rd, lugg_no_od, dest_pos_rd, dest_pos_od,
                     job_typ_rd, job_typ_od, error_code,
                     auto_mode_rd, sto_ready_rd, ret_ready_rd,
                     sensor0_data_rd, od_rq_yn, od_rq_flag, cmd_rq_yn,
                     host_send_yn, host_err_send_yn, use_yn, comp_vr,
                     tracking_write_yn, remote_control, roll_mode, stock_mode,
                     read_upd_dt, write_upd_dt)
SELECT '10',
       lpad(v.cv::text, 2, '0'),
       '1' || lpad(v.tr::text, 3, '0'),
       '1' || lpad(v.tr::text, 2, '0'),
       'C/V#' || v.cv || ' P' || v.tr || ' (TR#' || v.tr || ') ' || v.nm,
       v.kind::text,
       '0000', '0', '000', '0',
       '0', '0', '0',
       '1', '1', '1',
       '0', 'N', 'N', 'N',
       'Y', 'Y', 'Y', 'Y',
       'N', '0', '0', '0',
       now(), now() - interval '1 second'
FROM (VALUES
    -- (TR, CV, 종별, 이름)  종별: 1=입고대, 2=출고대, 3=입출고대, 0=라인
    ( 1,  1, 0, '라인'), ( 2,  1, 0, '라인'),
    ( 3,  2, 0, '라인(RGV측)'), ( 4,  2, 0, '라인(S/C#1측)'),
    ( 5,  3, 0, '라인(RGV측)'), ( 6,  3, 0, '라인(S/C#2측)'),
    ( 7,  4, 0, '라인(RGV측)'), ( 8,  4, 0, '라인(S/C#2측)'),
    ( 9,  5, 0, '라인(RGV측)'), (10,  5, 0, '라인(S/C#3측)'),
    (11,  6, 0, '라인(RGV측)'), (12,  6, 0, '라인(S/C#3측)'),
    (13,  7, 0, '라인(RGV측)'), (14,  7, 0, '라인(S/C#4측)'),
    (15,  8, 0, '라인(RGV측)'), (16,  8, 0, '라인(S/C#4측)'),
    (17,  9, 0, '라인(RGV측)'), (18,  9, 0, '라인(S/C#5측)'),
    (19, 10, 0, '라인(RGV측)'), (20, 10, 0, '라인(S/C#5측)'),
    (21, 11, 0, '라인(RGV측)'), (22, 11, 3, '입출고대'),
    (23, 12, 0, '라인(RGV측)'), (24, 12, 1, '입고대'),
    (25, 13, 0, '라인(RGV측)'), (26, 13, 2, '출고대'),
    (27, 14, 0, '라인(RGV측)'), (28, 14, 0, '버퍼'), (29, 14, 2, '출고대'),
    (30, 15, 1, '입고대'),      (31, 15, 0, '라인(RGV측)'), (32, 15, 0, '예비')
) AS v(tr, cv, kind, nm);

-- ---------------------------------------------------------------------
-- 2) sc_data — S/C #1~#5 (901~905) 정보 보강 (기존 행 갱신 + 누락 생성)
-- ---------------------------------------------------------------------
INSERT INTO sc_data (wh_typ, sc_no, host_send_yn)
SELECT '10', s.no, 'N'
FROM (VALUES ('901'),('902'),('903'),('904'),('905')) AS s(no)
WHERE NOT EXISTS (SELECT 1 FROM sc_data WHERE sc_no = s.no);

UPDATE sc_data SET
    plc_no      = lpad((sc_no::int - 900)::text, 2, '0'),
    sc_grp_no   = '01',
    mc_no       = sc_no,
    mc_no_nm    = 'S/C#' || (sc_no::int - 900) || ' (Bank ' ||
                  lpad(((sc_no::int - 900) * 2 - 1)::text, 2, '0') || ',' ||
                  lpad(((sc_no::int - 900) * 2)::text, 2, '0') || ')',
    auto_mode_rd = '1', ucstatus_rd = '1', online_mode_rd = '1', active_mode_rd = '1',
    err_code_rd = '0', complete_rd = '0',
    od_rq_yn = 'N', od_rq_flag = 'N', cmd_rq_yn = 'N',
    use_fk_rd = '1', suspend = 'N', sc_typ = 'SF',
    read_upd_dt = now(), write_upd_dt = now() - interval '1 second'
WHERE wh_typ = '10' AND sc_no IN ('901','902','903','904','905');

-- ---------------------------------------------------------------------
-- 3) rtv_data — RGV #1
-- ---------------------------------------------------------------------
DELETE FROM rtv_data WHERE wh_typ = '10';
INSERT INTO rtv_data (wh_typ, plc_no, rtv_no, auto_mode_rd, waiting_order_rd,
                      sensor_rtv_rd, err_code_rd, sensor_fk1_rd, sensor_fk2_rd,
                      cmd_rq_yn, od_rq_yn, suspend, is_error_rd,
                      read_upd_dt, write_upd_dt)
VALUES ('10', '01', '01', '1', '1', '0', '0', '0', '0', 'N', 'N', 'N', '0',
        now(), now() - interval '1 second');

-- ---------------------------------------------------------------------
-- 4) eqp_mst — 통신 대상 설비 (HOST/HOST2 유지, 재고성 BCR 제거)
-- ---------------------------------------------------------------------
DELETE FROM eqp_mst WHERE eqp_typ NOT IN ('HOST', 'HOST2');
INSERT INTO eqp_mst (wh_typ, eqp_typ, plc_no, sc_grp_no, plc_ip,
                     plc_port_from, plc_port_to, plc_port, sock_timeout,
                     retry_yn, use_yn, connected_yn, upd_dt, remarks)
VALUES
 -- C/V 게이트웨이 (WCS_TASK_CV COMM0~4 = C/V#11~#15, XGT 시뮬레이터 3001)
 ('10','CV','11','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'C/V#11 입출고대(TR#21~22)'),
 ('10','CV','12','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'C/V#12 입고대(TR#23~24)'),
 ('10','CV','13','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'C/V#13 출고대(TR#25~26)'),
 ('10','CV','14','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'C/V#14 출고대(TR#27~29)'),
 ('10','CV','15','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'C/V#15 입고대(TR#30~32)'),
 -- S/C #1~#5
 ('10','SC','01','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'S/C#1 (Bank01,02)'),
 ('10','SC','02','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'S/C#2 (Bank03,04)'),
 ('10','SC','03','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'S/C#3 (Bank05,06)'),
 ('10','SC','04','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'S/C#4 (Bank07,08)'),
 ('10','SC','05','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'S/C#5 (Bank09,10)'),
 -- RGV #1
 ('10','RTV','01','01','127.0.0.1','3001','3001','3001','2','Y','Y','N',now(),'RGV#1');

-- ---------------------------------------------------------------------
-- 5) cv_def_inf — 컨베이어 반송 흐름 정의 (원본 TB_ITINERARY 근거, TR 기준)
-- ---------------------------------------------------------------------
DELETE FROM cv_def_inf WHERE wh_typ = '10';
INSERT INTO cv_def_inf (wh_typ, fr_cv, to_cv, use_yon, wait_cv, buffer_cv, remarks)
VALUES
 -- 입고 (입고대 → RGV측)
 ('10','122','121','Y','121','','C/V#11 입고 (I001)'),
 ('10','124','123','Y','123','','C/V#12 입고 (I002)'),
 ('10','130','131','Y','131','','C/V#15 입고 (I003)'),
 -- 출고 (RGV측 → 출고대)
 ('10','121','122','Y','122','','C/V#11 출고 (O031)'),
 ('10','125','126','Y','126','','C/V#13 출고 (O032)'),
 ('10','127','129','Y','129','128','C/V#14 출고 (O033, 버퍼 TR#28)'),
 -- S/C 라인 (입고: RGV측→S/C측 / 출고: S/C측→RGV측) — 원본 I/O 레그
 ('10','103','104','Y','104','','C/V#2 입고(S/C#1)'), ('10','104','103','Y','103','','C/V#2 출고(S/C#1)'),
 ('10','107','108','Y','108','','C/V#4 입고(S/C#2)'), ('10','106','105','Y','105','','C/V#3 출고(S/C#2)'),
 ('10','111','112','Y','112','','C/V#6 입고(S/C#3)'), ('10','110','109','Y','109','','C/V#5 출고(S/C#3)'),
 ('10','115','116','Y','116','','C/V#8 입고(S/C#4)'), ('10','114','113','Y','113','','C/V#7 출고(S/C#4)'),
 ('10','119','120','Y','120','','C/V#10 입고(S/C#5)'),('10','118','117','Y','117','','C/V#9 출고(S/C#5)');

-- ---------------------------------------------------------------------
-- 6) sc_def_inf — S/C 별 담당 라인 C/V (원본 TB_ITINERARY/TOTALPATH 근거)
--    area = 담당 Bank (2k-1, 2k)
-- ---------------------------------------------------------------------
DELETE FROM sc_def_inf WHERE wh_typ = '10';
INSERT INTO sc_def_inf (wh_typ, sc_no, cv_no, area, upd_dt)
VALUES
 ('10','901','02','01,02', now()),   -- S/C#1: 입출고 라인 C/V#2, Bank 1~2
 ('10','902','04','03,04', now()),   -- S/C#2: 입고 C/V#4 / 출고 C/V#3, Bank 3~4
 ('10','903','06','05,06', now()),
 ('10','904','08','07,08', now()),
 ('10','905','10','09,10', now());

-- ---------------------------------------------------------------------
-- 7) dest_pos_def — 도착지 정의 (출고대 + 크레인)
-- ---------------------------------------------------------------------
DELETE FROM dest_pos_def WHERE wh_typ = '10';
INSERT INTO dest_pos_def (wh_typ, track_no, mc_no, group_no, priority, ret_cnt, remarks)
VALUES
 ('10','1022','122','OUT', 1, '0', 'C/V#11 출고대 TR#22'),
 ('10','1026','126','OUT', 1, '0', 'C/V#13 출고대 TR#26'),
 ('10','1029','129','OUT', 1, '0', 'C/V#14 출고대 TR#29'),
 ('10','0901','901','SC',  1, '0', 'S/C#1'),
 ('10','0902','902','SC',  1, '0', 'S/C#2'),
 ('10','0903','903','SC',  1, '0', 'S/C#3'),
 ('10','0904','904','SC',  1, '0', 'S/C#4'),
 ('10','0905','905','SC',  1, '0', 'S/C#5');

-- ---------------------------------------------------------------------
-- 8) 테스트 잔여 작업 정리 (구 스테이션 체계 101~105 기반 작업)
-- ---------------------------------------------------------------------
UPDATE job_mst SET job_status = '9'
 WHERE wh_typ = '10' AND job_status IN ('99','10','11','15','20','21','25');

SELECT 'cv_data' t, COUNT(*) FROM cv_data
UNION ALL SELECT 'sc_data', COUNT(*) FROM sc_data WHERE wh_typ='10'
UNION ALL SELECT 'rtv_data', COUNT(*) FROM rtv_data
UNION ALL SELECT 'eqp_mst', COUNT(*) FROM eqp_mst
UNION ALL SELECT 'cv_def_inf', COUNT(*) FROM cv_def_inf
UNION ALL SELECT 'sc_def_inf', COUNT(*) FROM sc_def_inf
UNION ALL SELECT 'dest_pos_def', COUNT(*) FROM dest_pos_def;
