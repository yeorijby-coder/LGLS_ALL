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


-- 지울 컬럼에 값이 들어 있는지 본다. 모두 0 이어야 안심하고 지울 수 있다.
USE LGLS_MCS_IO;
GO

SET NOCOUNT ON;

PRINT '--- cv_data ---';
SELECT SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([comming_rd] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [comming_rd],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([comming_od] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [comming_od],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([comming_dest_tr] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [comming_dest_tr],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([comming_check_tr1] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [comming_check_tr1],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([comming_check_tr2] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [comming_check_tr2],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([comming_check_tr3] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [comming_check_tr3],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([comming_check_tr4] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [comming_check_tr4],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([d5n_4_rd] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [d5n_4_rd],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([d5n_1_rd] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [d5n_1_rd],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([roll_mode] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [roll_mode],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([driv_paper_pos] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [driv_paper_pos],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([elev_asc_err] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [elev_asc_err],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([elev_desc_err] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [elev_desc_err],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([clamp_forward_err] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [clamp_forward_err],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([clamp_backward_err] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [clamp_backward_err],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([driv_forward_err] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [driv_forward_err],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([driv_backward_err] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [driv_backward_err],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([paper_block_sensor1] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [paper_block_sensor1],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([paper_block_sensor2] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [paper_block_sensor2],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([paper_block_sensor3] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [paper_block_sensor3],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([paper_block_sensor4] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [paper_block_sensor4],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([paper_full_sensor] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [paper_full_sensor],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([driv_forward_pos] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [driv_forward_pos],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([driv_backward_pos] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [driv_backward_pos],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([crush_paper_sensor] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [crush_paper_sensor],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([clamp_forward_sensor] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [clamp_forward_sensor],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([clamp_backward_sensor] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [clamp_backward_sensor],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([rev_elev_pos_data_asc] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [rev_elev_pos_data_asc],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([rev_elev_pos_data_1] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [rev_elev_pos_data_1],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([rev_elev_pos_data_2] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [rev_elev_pos_data_2],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([rev_driv_pos_data_pickup] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [rev_driv_pos_data_pickup],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([rev_driv_pos_data_reverse] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [rev_driv_pos_data_reverse],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([rev_sensor_data_height] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [rev_sensor_data_height],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([roll_elev_pos_data_pickup] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [roll_elev_pos_data_pickup],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([roll_elev_pos_data_rolling] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [roll_elev_pos_data_rolling],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([roll_clamp_pos_data_backward] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [roll_clamp_pos_data_backward],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([roll_clamp_pos_data_forward] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [roll_clamp_pos_data_forward],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([roll_sensor_data_height] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [roll_sensor_data_height],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([roll_elev_pos_data_asc] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [roll_elev_pos_data_asc]
  FROM dbo.[cv_data];
GO

PRINT '--- job_mst ---';
SELECT SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([mes_write_yn] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [mes_write_yn],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([mes_error_cd] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [mes_error_cd],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([system_byte] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [system_byte],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([s6f12_response] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [s6f12_response],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([s6f11_yon] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [s6f11_yon],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([ceid_name] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [ceid_name]
  FROM dbo.[job_mst];
GO

PRINT '--- job_mst_his ---';
SELECT SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([mes_write_yn] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [mes_write_yn],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([mes_error_cd] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [mes_error_cd],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([system_byte] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [system_byte],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([s6f12_response] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [s6f12_response],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([s6f11_yon] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [s6f11_yon],
       SUM(CASE WHEN ISNULL(LTRIM(RTRIM(CAST([ceid_name] AS varchar(200)))),'') NOT IN ('','0') THEN 1 ELSE 0 END) AS [ceid_name]
  FROM dbo.[job_mst_his];
GO
