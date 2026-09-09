/* ============================================================================
   LGLS ECS - 이 현장에서 쓰지 않는 컬럼 정리
   작성 2026-09-10
   대상 DB : LGLS_MCS_IO

   무엇을 지우나
     cv_data      39개  리버서(REV) / 롤러(ROLL) / 종이(PAPER) 설비용 컬럼.
                        이 현장에는 그 설비가 없다. 값도 들어간 적이 없다.
     job_mst      6개  MES 기록용 2개 + SECS/GEM 연동용 4개.
                        이 현장은 MES/SECS 연동을 하지 않는다.
     job_mst_his  6개  위와 같음(이력 테이블).

   지우지 않는 것
     WC_STEP  - 정의서에 "이 현장 미사용" 으로 적혀 있었으나 실제로 쓰인다.
                HOST 완료 보고를 한 번만 내보내기 위한 재보고 방지 플래그다.
                (WCS_TASK_HOST/CCliWork.cs 의 WC_STEP='0' 조회 → '1' 갱신)
                정의서 설명을 고쳤다.

   실행 순서
     1) 00_점검_지우기전.sql   지울 컬럼에 값이 있는지 먼저 본다
     2) 01_컬럼정리.sql        실제 삭제 (이 파일)
     3) 03_점검_지운후.sql     남았는지 확인
     되돌리려면 02_컬럼복구.sql (자료형만 되살린다. 값은 되살릴 수 없다)

   주의
     - 프로그램(ECS Client / WCS_TASK_CV / IO_TASK / WCS_TASK_HOST)을 모두 내린 뒤 실행한다.
     - 이 스크립트와 짝이 되는 프로그램 버전(2026-09-10 이후)으로 올려야 한다.
       그 이전 버전은 지운 컬럼을 SELECT/INSERT 하므로 오류가 난다.
   ============================================================================ */


USE LGLS_MCS_IO;
GO

SET NOCOUNT ON;

-- 아래 조회 결과가 0행이면 정리가 끝난 것이다.
SELECT x.tbl, x.col
  FROM (VALUES ('cv_data','comming_rd'),
       ('cv_data','comming_od'),
       ('cv_data','comming_dest_tr'),
       ('cv_data','comming_check_tr1'),
       ('cv_data','comming_check_tr2'),
       ('cv_data','comming_check_tr3'),
       ('cv_data','comming_check_tr4'),
       ('cv_data','d5n_4_rd'),
       ('cv_data','d5n_1_rd'),
       ('cv_data','roll_mode'),
       ('cv_data','driv_paper_pos'),
       ('cv_data','elev_asc_err'),
       ('cv_data','elev_desc_err'),
       ('cv_data','clamp_forward_err'),
       ('cv_data','clamp_backward_err'),
       ('cv_data','driv_forward_err'),
       ('cv_data','driv_backward_err'),
       ('cv_data','paper_block_sensor1'),
       ('cv_data','paper_block_sensor2'),
       ('cv_data','paper_block_sensor3'),
       ('cv_data','paper_block_sensor4'),
       ('cv_data','paper_full_sensor'),
       ('cv_data','driv_forward_pos'),
       ('cv_data','driv_backward_pos'),
       ('cv_data','crush_paper_sensor'),
       ('cv_data','clamp_forward_sensor'),
       ('cv_data','clamp_backward_sensor'),
       ('cv_data','rev_elev_pos_data_asc'),
       ('cv_data','rev_elev_pos_data_1'),
       ('cv_data','rev_elev_pos_data_2'),
       ('cv_data','rev_driv_pos_data_pickup'),
       ('cv_data','rev_driv_pos_data_reverse'),
       ('cv_data','rev_sensor_data_height'),
       ('cv_data','roll_elev_pos_data_pickup'),
       ('cv_data','roll_elev_pos_data_rolling'),
       ('cv_data','roll_clamp_pos_data_backward'),
       ('cv_data','roll_clamp_pos_data_forward'),
       ('cv_data','roll_sensor_data_height'),
       ('cv_data','roll_elev_pos_data_asc'),
       ('job_mst','mes_write_yn'),
       ('job_mst','mes_error_cd'),
       ('job_mst','system_byte'),
       ('job_mst','s6f12_response'),
       ('job_mst','s6f11_yon'),
       ('job_mst','ceid_name'),
       ('job_mst_his','mes_write_yn'),
       ('job_mst_his','mes_error_cd'),
       ('job_mst_his','system_byte'),
       ('job_mst_his','s6f12_response'),
       ('job_mst_his','s6f11_yon'),
       ('job_mst_his','ceid_name')
       ) AS x(tbl, col)
 WHERE COL_LENGTH('dbo.'+x.tbl, x.col) IS NOT NULL;
GO

-- 테이블별 남은 컬럼 수
SELECT TABLE_NAME, COUNT(*) AS cols FROM INFORMATION_SCHEMA.COLUMNS
 WHERE TABLE_NAME IN ('cv_data','job_mst','job_mst_his')
 GROUP BY TABLE_NAME ORDER BY TABLE_NAME;
GO