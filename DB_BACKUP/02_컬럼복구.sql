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


/* 되돌리기 : 컬럼을 같은 자료형으로 다시 만든다.
   지운 값은 되살릴 수 없다(어차피 이 현장에서 쓰지 않던 컬럼이라 값이 없다).
   프로그램도 2026-09-10 이전 버전으로 되돌려야 짝이 맞는다. */

USE LGLS_MCS_IO;
GO

-- ===== cv_data =====
IF COL_LENGTH('dbo.cv_data','comming_rd') IS NULL
    ALTER TABLE dbo.cv_data ADD COMMING_RD varchar(5) NULL;
GO
IF COL_LENGTH('dbo.cv_data','comming_od') IS NULL
    ALTER TABLE dbo.cv_data ADD COMMING_OD varchar(5) NULL;
GO
IF COL_LENGTH('dbo.cv_data','comming_dest_tr') IS NULL
    ALTER TABLE dbo.cv_data ADD COMMING_DEST_TR varchar(MAX) NULL;
GO
IF COL_LENGTH('dbo.cv_data','comming_check_tr1') IS NULL
    ALTER TABLE dbo.cv_data ADD COMMING_CHECK_TR1 varchar(MAX) NULL;
GO
IF COL_LENGTH('dbo.cv_data','comming_check_tr2') IS NULL
    ALTER TABLE dbo.cv_data ADD COMMING_CHECK_TR2 varchar(MAX) NULL;
GO
IF COL_LENGTH('dbo.cv_data','comming_check_tr3') IS NULL
    ALTER TABLE dbo.cv_data ADD COMMING_CHECK_TR3 varchar(MAX) NULL;
GO
IF COL_LENGTH('dbo.cv_data','comming_check_tr4') IS NULL
    ALTER TABLE dbo.cv_data ADD COMMING_CHECK_TR4 varchar(MAX) NULL;
GO
IF COL_LENGTH('dbo.cv_data','d5n_4_rd') IS NULL
    ALTER TABLE dbo.cv_data ADD D5N_4_RD varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','d5n_1_rd') IS NULL
    ALTER TABLE dbo.cv_data ADD D5N_1_RD varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','roll_mode') IS NULL
    ALTER TABLE dbo.cv_data ADD ROLL_MODE varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','driv_paper_pos') IS NULL
    ALTER TABLE dbo.cv_data ADD DRIV_PAPER_POS varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','elev_asc_err') IS NULL
    ALTER TABLE dbo.cv_data ADD ELEV_ASC_ERR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','elev_desc_err') IS NULL
    ALTER TABLE dbo.cv_data ADD ELEV_DESC_ERR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','clamp_forward_err') IS NULL
    ALTER TABLE dbo.cv_data ADD CLAMP_FORWARD_ERR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','clamp_backward_err') IS NULL
    ALTER TABLE dbo.cv_data ADD CLAMP_BACKWARD_ERR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','driv_forward_err') IS NULL
    ALTER TABLE dbo.cv_data ADD DRIV_FORWARD_ERR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','driv_backward_err') IS NULL
    ALTER TABLE dbo.cv_data ADD DRIV_BACKWARD_ERR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','paper_block_sensor1') IS NULL
    ALTER TABLE dbo.cv_data ADD PAPER_BLOCK_SENSOR1 varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','paper_block_sensor2') IS NULL
    ALTER TABLE dbo.cv_data ADD PAPER_BLOCK_SENSOR2 varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','paper_block_sensor3') IS NULL
    ALTER TABLE dbo.cv_data ADD PAPER_BLOCK_SENSOR3 varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','paper_block_sensor4') IS NULL
    ALTER TABLE dbo.cv_data ADD PAPER_BLOCK_SENSOR4 varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','paper_full_sensor') IS NULL
    ALTER TABLE dbo.cv_data ADD PAPER_FULL_SENSOR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','driv_forward_pos') IS NULL
    ALTER TABLE dbo.cv_data ADD DRIV_FORWARD_POS varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','driv_backward_pos') IS NULL
    ALTER TABLE dbo.cv_data ADD DRIV_BACKWARD_POS varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','crush_paper_sensor') IS NULL
    ALTER TABLE dbo.cv_data ADD CRUSH_PAPER_SENSOR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','clamp_forward_sensor') IS NULL
    ALTER TABLE dbo.cv_data ADD CLAMP_FORWARD_SENSOR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','clamp_backward_sensor') IS NULL
    ALTER TABLE dbo.cv_data ADD CLAMP_BACKWARD_SENSOR varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','rev_elev_pos_data_asc') IS NULL
    ALTER TABLE dbo.cv_data ADD REV_ELEV_POS_DATA_ASC varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','rev_elev_pos_data_1') IS NULL
    ALTER TABLE dbo.cv_data ADD REV_ELEV_POS_DATA_1 varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','rev_elev_pos_data_2') IS NULL
    ALTER TABLE dbo.cv_data ADD REV_ELEV_POS_DATA_2 varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','rev_driv_pos_data_pickup') IS NULL
    ALTER TABLE dbo.cv_data ADD REV_DRIV_POS_DATA_PICKUP varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','rev_driv_pos_data_reverse') IS NULL
    ALTER TABLE dbo.cv_data ADD REV_DRIV_POS_DATA_REVERSE varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','rev_sensor_data_height') IS NULL
    ALTER TABLE dbo.cv_data ADD REV_SENSOR_DATA_HEIGHT varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','roll_elev_pos_data_pickup') IS NULL
    ALTER TABLE dbo.cv_data ADD ROLL_ELEV_POS_DATA_PICKUP varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','roll_elev_pos_data_rolling') IS NULL
    ALTER TABLE dbo.cv_data ADD ROLL_ELEV_POS_DATA_ROLLING varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','roll_clamp_pos_data_backward') IS NULL
    ALTER TABLE dbo.cv_data ADD ROLL_CLAMP_POS_DATA_BACKWARD varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','roll_clamp_pos_data_forward') IS NULL
    ALTER TABLE dbo.cv_data ADD ROLL_CLAMP_POS_DATA_FORWARD varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','roll_sensor_data_height') IS NULL
    ALTER TABLE dbo.cv_data ADD ROLL_SENSOR_DATA_HEIGHT varchar(255) NULL;
GO
IF COL_LENGTH('dbo.cv_data','roll_elev_pos_data_asc') IS NULL
    ALTER TABLE dbo.cv_data ADD ROLL_ELEV_POS_DATA_ASC varchar(2) NULL;
GO

-- ===== job_mst =====
IF COL_LENGTH('dbo.job_mst','mes_write_yn') IS NULL
    ALTER TABLE dbo.job_mst ADD MES_WRITE_YN varchar(1) NULL;
GO
IF COL_LENGTH('dbo.job_mst','mes_error_cd') IS NULL
    ALTER TABLE dbo.job_mst ADD MES_ERROR_CD varchar(4) NULL;
GO
IF COL_LENGTH('dbo.job_mst','system_byte') IS NULL
    ALTER TABLE dbo.job_mst ADD SYSTEM_BYTE varchar(30) NULL;
GO
IF COL_LENGTH('dbo.job_mst','s6f12_response') IS NULL
    ALTER TABLE dbo.job_mst ADD S6F12_RESPONSE varchar(1) NULL;
GO
IF COL_LENGTH('dbo.job_mst','s6f11_yon') IS NULL
    ALTER TABLE dbo.job_mst ADD S6F11_YON varchar(1) NULL;
GO
IF COL_LENGTH('dbo.job_mst','ceid_name') IS NULL
    ALTER TABLE dbo.job_mst ADD CEID_NAME varchar(20) NULL;
GO

-- ===== job_mst_his =====
IF COL_LENGTH('dbo.job_mst_his','mes_write_yn') IS NULL
    ALTER TABLE dbo.job_mst_his ADD MES_WRITE_YN varchar(1) NULL;
GO
IF COL_LENGTH('dbo.job_mst_his','mes_error_cd') IS NULL
    ALTER TABLE dbo.job_mst_his ADD MES_ERROR_CD varchar(4) NULL;
GO
IF COL_LENGTH('dbo.job_mst_his','system_byte') IS NULL
    ALTER TABLE dbo.job_mst_his ADD SYSTEM_BYTE varchar(30) NULL;
GO
IF COL_LENGTH('dbo.job_mst_his','s6f12_response') IS NULL
    ALTER TABLE dbo.job_mst_his ADD S6F12_RESPONSE varchar(1) NULL;
GO
IF COL_LENGTH('dbo.job_mst_his','s6f11_yon') IS NULL
    ALTER TABLE dbo.job_mst_his ADD S6F11_YON varchar(1) NULL;
GO
IF COL_LENGTH('dbo.job_mst_his','ceid_name') IS NULL
    ALTER TABLE dbo.job_mst_his ADD CEID_NAME varchar(20) NULL;
GO
