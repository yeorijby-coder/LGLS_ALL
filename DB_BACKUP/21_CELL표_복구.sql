/* ============================================================================
   LGLS ECS - 되돌리기 : CELL_MST / CELL_DTL 을 같은 자료형으로 다시 만든다
   (짝 : 20_CELL표_삭제.sql)

   1) 이 파일을 실행해 빈 표를 만든다.
   2) 값까지 되살리려면 명령창에서 (서버 이름은 현장에 맞게)
        bcp LGLS_MCS_IO.dbo.cell_mst in cell_20261109\cell_mst.bcp -S localhost\SQLEXPRESS -T -n
      cell_dtl 은 0행이라 빈 표면 된다.
   3) 셀 관리를 다시 쓰려면 Ecs.ini [CNF] USE_CELL_MST=1 로 바꾼다.
   ============================================================================ */
USE LGLS_MCS_IO;
GO
IF OBJECT_ID('dbo.cell_mst','U') IS NULL
CREATE TABLE dbo.[cell_mst] (
    [wh_typ] varchar(2) NULL,
    [cell_sc_no] varchar(3) NULL,
    [cell_no] varchar(8) NULL,
    [bank] varchar(2) NULL,
    [bay] varchar(2) NULL,
    [lev] varchar(2) NULL,
    [gdbox_no] varchar(10) NULL DEFAULT (''),
    [gdbox_racd] varchar(10) NULL DEFAULT (''),
    [fire_detector_no] varchar(10) NULL DEFAULT (''),
    [cell_fire_yn] varchar(1) NULL DEFAULT ('N'),
    [cell_sta] varchar(2) NULL DEFAULT ('0'),
    [cell_use_def] varchar(2) NULL DEFAULT ('1'),
    [cell_use_yn] varchar(1) NULL DEFAULT ('Y'),
    [aging_typ] varchar(2) NULL DEFAULT ('0'),
    [cell_typ] varchar(2) NULL DEFAULT ('0'),
    [bottom_tray] varchar(10) NULL DEFAULT (''),
    [top_tray] varchar(10) NULL DEFAULT (''),
    [upd_dt] datetime NULL DEFAULT (getdate()),
    [user_remarks] varchar(100) NULL DEFAULT (''),
    [remarks] varchar(100) NULL DEFAULT (''),
    [cell_status] varchar(2) NULL DEFAULT ('0'),
    [fire_od_rq_yn] varchar(1) NULL DEFAULT ('N'),
    [SC_PLT_JOB_TYP] varchar(10) NULL
);
GO

IF OBJECT_ID('dbo.cell_dtl','U') IS NULL
CREATE TABLE dbo.[cell_dtl] (
    [wh_typ] varchar(2) NULL,
    [cell_no] varchar(8) NULL,
    [cell_sc_no] varchar(3) NULL,
    [bank] varchar(2) NULL,
    [bay] varchar(2) NULL,
    [lev] varchar(2) NULL,
    [aging_start_dt] varchar(14) NULL,
    [aging_end_dt] varchar(14) NULL,
    [ins_dt] datetime NULL DEFAULT (getdate()),
    [ins_user_id] varchar(20) NULL,
    [upd_dt] datetime NULL,
    [upd_user_id] varchar(20) NULL,
    [bottom_tray] varchar(10) NULL,
    [top_tray] varchar(10) NULL,
    [user_remarks] varchar(100) NULL,
    [remarks] varchar(100) NULL
);
GO
PRINT 'CELL 표 복구 완료';
GO
