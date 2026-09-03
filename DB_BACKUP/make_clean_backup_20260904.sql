-- [LGLS 2026-09-04] 현장 이관용 정리 백업
--   운전 DB(LGLS_MCS_IO)를 복제(LGLS_MCS_IO_DEPLOY)한 뒤, 이 현장에서 쓰지 않는 테이블을 빼고
--   시험 데이터를 비운 상태로 백업한다. 운전 DB 자체는 건드리지 않는다.
USE master;
IF DB_ID('LGLS_MCS_IO_DEPLOY') IS NOT NULL
BEGIN
    ALTER DATABASE LGLS_MCS_IO_DEPLOY SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
    DROP DATABASE LGLS_MCS_IO_DEPLOY;
END
BACKUP DATABASE LGLS_MCS_IO TO DISK = N'D:\project\LGLS\Renewal\DB_BACKUP\_tmp_full.bak' WITH INIT, COPY_ONLY;
RESTORE DATABASE LGLS_MCS_IO_DEPLOY FROM DISK = N'D:\project\LGLS\Renewal\DB_BACKUP\_tmp_full.bak'
  WITH MOVE 'LGLS_MCS_IO'     TO N'C:\Program Files\Microsoft SQL Server\MSSQL10.SQLEXPRESS\MSSQL\DATA\LGLS_MCS_IO_DEPLOY.mdf',
       MOVE 'LGLS_MCS_IO_log' TO N'C:\Program Files\Microsoft SQL Server\MSSQL10.SQLEXPRESS\MSSQL\DATA\LGLS_MCS_IO_DEPLOY_log.ldf',
       REPLACE;
GO
USE LGLS_MCS_IO_DEPLOY;
-- 이 현장에서 쓰지 않는 테이블 (소스 참조 없음 / 0행 / 숨김 기능)
DROP TABLE auto_sc_work;
DROP TABLE bcr_mst;
DROP TABLE cell_dtl;
DROP TABLE chg_lang;
DROP TABLE dest_def;
DROP TABLE event_log;
DROP TABLE wait_track;
DROP TABLE wc_data;
DROP TABLE wc_his;
DROP TABLE host_empty_plt;
-- 시험 데이터 비움
DELETE FROM job_mst;
TRUNCATE TABLE job_mst_his;
TRUNCATE TABLE host_if_log;
TRUNCATE TABLE mes_if_log;
TRUNCATE TABLE wcs_log_pgr;
TRUNCATE TABLE wcs_client_log;
TRUNCATE TABLE eqp_err_his;
-- 시험 중 남은 트래킹/지시 초기화 (읽기값은 현장 PLC 가 덮어쓴다)
UPDATE cv_data SET lugg_no_rd = '', lugg_no_od = '', dest_pos_od = '', od_rq_yn = 'N';
GO
DBCC SHRINKDATABASE (LGLS_MCS_IO_DEPLOY, 5);
BACKUP DATABASE LGLS_MCS_IO_DEPLOY TO DISK = N'D:\project\LGLS\Renewal\DB_BACKUP\LGLS_MCS_IO_20260904_clean.bak' WITH INIT, COPY_ONLY;
GO
USE master;
ALTER DATABASE LGLS_MCS_IO_DEPLOY SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
DROP DATABASE LGLS_MCS_IO_DEPLOY;
GO
