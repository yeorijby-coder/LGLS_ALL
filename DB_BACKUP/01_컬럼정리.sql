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

SET XACT_ABORT ON;

-- ===== cv_data (39개) =====
IF COL_LENGTH('dbo.cv_data','comming_rd') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_comming_rd sysname;
    SELECT @d_cv_data_comming_rd = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'comming_rd','ColumnId');
    IF @d_cv_data_comming_rd IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_comming_rd + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [comming_rd];
    PRINT '  삭제 cv_data.comming_rd';
END
ELSE
    PRINT '  (이미 없음) cv_data.comming_rd';
GO
IF COL_LENGTH('dbo.cv_data','comming_od') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_comming_od sysname;
    SELECT @d_cv_data_comming_od = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'comming_od','ColumnId');
    IF @d_cv_data_comming_od IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_comming_od + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [comming_od];
    PRINT '  삭제 cv_data.comming_od';
END
ELSE
    PRINT '  (이미 없음) cv_data.comming_od';
GO
IF COL_LENGTH('dbo.cv_data','comming_dest_tr') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_comming_dest_tr sysname;
    SELECT @d_cv_data_comming_dest_tr = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'comming_dest_tr','ColumnId');
    IF @d_cv_data_comming_dest_tr IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_comming_dest_tr + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [comming_dest_tr];
    PRINT '  삭제 cv_data.comming_dest_tr';
END
ELSE
    PRINT '  (이미 없음) cv_data.comming_dest_tr';
GO
IF COL_LENGTH('dbo.cv_data','comming_check_tr1') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_comming_check_tr1 sysname;
    SELECT @d_cv_data_comming_check_tr1 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'comming_check_tr1','ColumnId');
    IF @d_cv_data_comming_check_tr1 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_comming_check_tr1 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [comming_check_tr1];
    PRINT '  삭제 cv_data.comming_check_tr1';
END
ELSE
    PRINT '  (이미 없음) cv_data.comming_check_tr1';
GO
IF COL_LENGTH('dbo.cv_data','comming_check_tr2') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_comming_check_tr2 sysname;
    SELECT @d_cv_data_comming_check_tr2 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'comming_check_tr2','ColumnId');
    IF @d_cv_data_comming_check_tr2 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_comming_check_tr2 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [comming_check_tr2];
    PRINT '  삭제 cv_data.comming_check_tr2';
END
ELSE
    PRINT '  (이미 없음) cv_data.comming_check_tr2';
GO
IF COL_LENGTH('dbo.cv_data','comming_check_tr3') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_comming_check_tr3 sysname;
    SELECT @d_cv_data_comming_check_tr3 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'comming_check_tr3','ColumnId');
    IF @d_cv_data_comming_check_tr3 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_comming_check_tr3 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [comming_check_tr3];
    PRINT '  삭제 cv_data.comming_check_tr3';
END
ELSE
    PRINT '  (이미 없음) cv_data.comming_check_tr3';
GO
IF COL_LENGTH('dbo.cv_data','comming_check_tr4') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_comming_check_tr4 sysname;
    SELECT @d_cv_data_comming_check_tr4 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'comming_check_tr4','ColumnId');
    IF @d_cv_data_comming_check_tr4 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_comming_check_tr4 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [comming_check_tr4];
    PRINT '  삭제 cv_data.comming_check_tr4';
END
ELSE
    PRINT '  (이미 없음) cv_data.comming_check_tr4';
GO
IF COL_LENGTH('dbo.cv_data','d5n_4_rd') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_d5n_4_rd sysname;
    SELECT @d_cv_data_d5n_4_rd = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'d5n_4_rd','ColumnId');
    IF @d_cv_data_d5n_4_rd IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_d5n_4_rd + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [d5n_4_rd];
    PRINT '  삭제 cv_data.d5n_4_rd';
END
ELSE
    PRINT '  (이미 없음) cv_data.d5n_4_rd';
GO
IF COL_LENGTH('dbo.cv_data','d5n_1_rd') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_d5n_1_rd sysname;
    SELECT @d_cv_data_d5n_1_rd = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'d5n_1_rd','ColumnId');
    IF @d_cv_data_d5n_1_rd IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_d5n_1_rd + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [d5n_1_rd];
    PRINT '  삭제 cv_data.d5n_1_rd';
END
ELSE
    PRINT '  (이미 없음) cv_data.d5n_1_rd';
GO
IF COL_LENGTH('dbo.cv_data','roll_mode') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_roll_mode sysname;
    SELECT @d_cv_data_roll_mode = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'roll_mode','ColumnId');
    IF @d_cv_data_roll_mode IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_roll_mode + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [roll_mode];
    PRINT '  삭제 cv_data.roll_mode';
END
ELSE
    PRINT '  (이미 없음) cv_data.roll_mode';
GO
IF COL_LENGTH('dbo.cv_data','driv_paper_pos') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_driv_paper_pos sysname;
    SELECT @d_cv_data_driv_paper_pos = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'driv_paper_pos','ColumnId');
    IF @d_cv_data_driv_paper_pos IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_driv_paper_pos + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [driv_paper_pos];
    PRINT '  삭제 cv_data.driv_paper_pos';
END
ELSE
    PRINT '  (이미 없음) cv_data.driv_paper_pos';
GO
IF COL_LENGTH('dbo.cv_data','elev_asc_err') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_elev_asc_err sysname;
    SELECT @d_cv_data_elev_asc_err = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'elev_asc_err','ColumnId');
    IF @d_cv_data_elev_asc_err IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_elev_asc_err + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [elev_asc_err];
    PRINT '  삭제 cv_data.elev_asc_err';
END
ELSE
    PRINT '  (이미 없음) cv_data.elev_asc_err';
GO
IF COL_LENGTH('dbo.cv_data','elev_desc_err') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_elev_desc_err sysname;
    SELECT @d_cv_data_elev_desc_err = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'elev_desc_err','ColumnId');
    IF @d_cv_data_elev_desc_err IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_elev_desc_err + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [elev_desc_err];
    PRINT '  삭제 cv_data.elev_desc_err';
END
ELSE
    PRINT '  (이미 없음) cv_data.elev_desc_err';
GO
IF COL_LENGTH('dbo.cv_data','clamp_forward_err') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_clamp_forward_err sysname;
    SELECT @d_cv_data_clamp_forward_err = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'clamp_forward_err','ColumnId');
    IF @d_cv_data_clamp_forward_err IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_clamp_forward_err + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [clamp_forward_err];
    PRINT '  삭제 cv_data.clamp_forward_err';
END
ELSE
    PRINT '  (이미 없음) cv_data.clamp_forward_err';
GO
IF COL_LENGTH('dbo.cv_data','clamp_backward_err') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_clamp_backward_err sysname;
    SELECT @d_cv_data_clamp_backward_err = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'clamp_backward_err','ColumnId');
    IF @d_cv_data_clamp_backward_err IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_clamp_backward_err + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [clamp_backward_err];
    PRINT '  삭제 cv_data.clamp_backward_err';
END
ELSE
    PRINT '  (이미 없음) cv_data.clamp_backward_err';
GO
IF COL_LENGTH('dbo.cv_data','driv_forward_err') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_driv_forward_err sysname;
    SELECT @d_cv_data_driv_forward_err = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'driv_forward_err','ColumnId');
    IF @d_cv_data_driv_forward_err IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_driv_forward_err + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [driv_forward_err];
    PRINT '  삭제 cv_data.driv_forward_err';
END
ELSE
    PRINT '  (이미 없음) cv_data.driv_forward_err';
GO
IF COL_LENGTH('dbo.cv_data','driv_backward_err') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_driv_backward_err sysname;
    SELECT @d_cv_data_driv_backward_err = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'driv_backward_err','ColumnId');
    IF @d_cv_data_driv_backward_err IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_driv_backward_err + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [driv_backward_err];
    PRINT '  삭제 cv_data.driv_backward_err';
END
ELSE
    PRINT '  (이미 없음) cv_data.driv_backward_err';
GO
IF COL_LENGTH('dbo.cv_data','paper_block_sensor1') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_paper_block_sensor1 sysname;
    SELECT @d_cv_data_paper_block_sensor1 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'paper_block_sensor1','ColumnId');
    IF @d_cv_data_paper_block_sensor1 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_paper_block_sensor1 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [paper_block_sensor1];
    PRINT '  삭제 cv_data.paper_block_sensor1';
END
ELSE
    PRINT '  (이미 없음) cv_data.paper_block_sensor1';
GO
IF COL_LENGTH('dbo.cv_data','paper_block_sensor2') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_paper_block_sensor2 sysname;
    SELECT @d_cv_data_paper_block_sensor2 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'paper_block_sensor2','ColumnId');
    IF @d_cv_data_paper_block_sensor2 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_paper_block_sensor2 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [paper_block_sensor2];
    PRINT '  삭제 cv_data.paper_block_sensor2';
END
ELSE
    PRINT '  (이미 없음) cv_data.paper_block_sensor2';
GO
IF COL_LENGTH('dbo.cv_data','paper_block_sensor3') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_paper_block_sensor3 sysname;
    SELECT @d_cv_data_paper_block_sensor3 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'paper_block_sensor3','ColumnId');
    IF @d_cv_data_paper_block_sensor3 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_paper_block_sensor3 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [paper_block_sensor3];
    PRINT '  삭제 cv_data.paper_block_sensor3';
END
ELSE
    PRINT '  (이미 없음) cv_data.paper_block_sensor3';
GO
IF COL_LENGTH('dbo.cv_data','paper_block_sensor4') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_paper_block_sensor4 sysname;
    SELECT @d_cv_data_paper_block_sensor4 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'paper_block_sensor4','ColumnId');
    IF @d_cv_data_paper_block_sensor4 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_paper_block_sensor4 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [paper_block_sensor4];
    PRINT '  삭제 cv_data.paper_block_sensor4';
END
ELSE
    PRINT '  (이미 없음) cv_data.paper_block_sensor4';
GO
IF COL_LENGTH('dbo.cv_data','paper_full_sensor') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_paper_full_sensor sysname;
    SELECT @d_cv_data_paper_full_sensor = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'paper_full_sensor','ColumnId');
    IF @d_cv_data_paper_full_sensor IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_paper_full_sensor + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [paper_full_sensor];
    PRINT '  삭제 cv_data.paper_full_sensor';
END
ELSE
    PRINT '  (이미 없음) cv_data.paper_full_sensor';
GO
IF COL_LENGTH('dbo.cv_data','driv_forward_pos') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_driv_forward_pos sysname;
    SELECT @d_cv_data_driv_forward_pos = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'driv_forward_pos','ColumnId');
    IF @d_cv_data_driv_forward_pos IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_driv_forward_pos + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [driv_forward_pos];
    PRINT '  삭제 cv_data.driv_forward_pos';
END
ELSE
    PRINT '  (이미 없음) cv_data.driv_forward_pos';
GO
IF COL_LENGTH('dbo.cv_data','driv_backward_pos') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_driv_backward_pos sysname;
    SELECT @d_cv_data_driv_backward_pos = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'driv_backward_pos','ColumnId');
    IF @d_cv_data_driv_backward_pos IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_driv_backward_pos + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [driv_backward_pos];
    PRINT '  삭제 cv_data.driv_backward_pos';
END
ELSE
    PRINT '  (이미 없음) cv_data.driv_backward_pos';
GO
IF COL_LENGTH('dbo.cv_data','crush_paper_sensor') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_crush_paper_sensor sysname;
    SELECT @d_cv_data_crush_paper_sensor = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'crush_paper_sensor','ColumnId');
    IF @d_cv_data_crush_paper_sensor IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_crush_paper_sensor + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [crush_paper_sensor];
    PRINT '  삭제 cv_data.crush_paper_sensor';
END
ELSE
    PRINT '  (이미 없음) cv_data.crush_paper_sensor';
GO
IF COL_LENGTH('dbo.cv_data','clamp_forward_sensor') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_clamp_forward_sensor sysname;
    SELECT @d_cv_data_clamp_forward_sensor = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'clamp_forward_sensor','ColumnId');
    IF @d_cv_data_clamp_forward_sensor IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_clamp_forward_sensor + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [clamp_forward_sensor];
    PRINT '  삭제 cv_data.clamp_forward_sensor';
END
ELSE
    PRINT '  (이미 없음) cv_data.clamp_forward_sensor';
GO
IF COL_LENGTH('dbo.cv_data','clamp_backward_sensor') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_clamp_backward_sensor sysname;
    SELECT @d_cv_data_clamp_backward_sensor = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'clamp_backward_sensor','ColumnId');
    IF @d_cv_data_clamp_backward_sensor IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_clamp_backward_sensor + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [clamp_backward_sensor];
    PRINT '  삭제 cv_data.clamp_backward_sensor';
END
ELSE
    PRINT '  (이미 없음) cv_data.clamp_backward_sensor';
GO
IF COL_LENGTH('dbo.cv_data','rev_elev_pos_data_asc') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_rev_elev_pos_data_asc sysname;
    SELECT @d_cv_data_rev_elev_pos_data_asc = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'rev_elev_pos_data_asc','ColumnId');
    IF @d_cv_data_rev_elev_pos_data_asc IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_rev_elev_pos_data_asc + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [rev_elev_pos_data_asc];
    PRINT '  삭제 cv_data.rev_elev_pos_data_asc';
END
ELSE
    PRINT '  (이미 없음) cv_data.rev_elev_pos_data_asc';
GO
IF COL_LENGTH('dbo.cv_data','rev_elev_pos_data_1') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_rev_elev_pos_data_1 sysname;
    SELECT @d_cv_data_rev_elev_pos_data_1 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'rev_elev_pos_data_1','ColumnId');
    IF @d_cv_data_rev_elev_pos_data_1 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_rev_elev_pos_data_1 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [rev_elev_pos_data_1];
    PRINT '  삭제 cv_data.rev_elev_pos_data_1';
END
ELSE
    PRINT '  (이미 없음) cv_data.rev_elev_pos_data_1';
GO
IF COL_LENGTH('dbo.cv_data','rev_elev_pos_data_2') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_rev_elev_pos_data_2 sysname;
    SELECT @d_cv_data_rev_elev_pos_data_2 = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'rev_elev_pos_data_2','ColumnId');
    IF @d_cv_data_rev_elev_pos_data_2 IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_rev_elev_pos_data_2 + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [rev_elev_pos_data_2];
    PRINT '  삭제 cv_data.rev_elev_pos_data_2';
END
ELSE
    PRINT '  (이미 없음) cv_data.rev_elev_pos_data_2';
GO
IF COL_LENGTH('dbo.cv_data','rev_driv_pos_data_pickup') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_rev_driv_pos_data_pickup sysname;
    SELECT @d_cv_data_rev_driv_pos_data_pickup = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'rev_driv_pos_data_pickup','ColumnId');
    IF @d_cv_data_rev_driv_pos_data_pickup IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_rev_driv_pos_data_pickup + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [rev_driv_pos_data_pickup];
    PRINT '  삭제 cv_data.rev_driv_pos_data_pickup';
END
ELSE
    PRINT '  (이미 없음) cv_data.rev_driv_pos_data_pickup';
GO
IF COL_LENGTH('dbo.cv_data','rev_driv_pos_data_reverse') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_rev_driv_pos_data_reverse sysname;
    SELECT @d_cv_data_rev_driv_pos_data_reverse = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'rev_driv_pos_data_reverse','ColumnId');
    IF @d_cv_data_rev_driv_pos_data_reverse IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_rev_driv_pos_data_reverse + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [rev_driv_pos_data_reverse];
    PRINT '  삭제 cv_data.rev_driv_pos_data_reverse';
END
ELSE
    PRINT '  (이미 없음) cv_data.rev_driv_pos_data_reverse';
GO
IF COL_LENGTH('dbo.cv_data','rev_sensor_data_height') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_rev_sensor_data_height sysname;
    SELECT @d_cv_data_rev_sensor_data_height = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'rev_sensor_data_height','ColumnId');
    IF @d_cv_data_rev_sensor_data_height IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_rev_sensor_data_height + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [rev_sensor_data_height];
    PRINT '  삭제 cv_data.rev_sensor_data_height';
END
ELSE
    PRINT '  (이미 없음) cv_data.rev_sensor_data_height';
GO
IF COL_LENGTH('dbo.cv_data','roll_elev_pos_data_pickup') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_roll_elev_pos_data_pickup sysname;
    SELECT @d_cv_data_roll_elev_pos_data_pickup = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'roll_elev_pos_data_pickup','ColumnId');
    IF @d_cv_data_roll_elev_pos_data_pickup IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_roll_elev_pos_data_pickup + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [roll_elev_pos_data_pickup];
    PRINT '  삭제 cv_data.roll_elev_pos_data_pickup';
END
ELSE
    PRINT '  (이미 없음) cv_data.roll_elev_pos_data_pickup';
GO
IF COL_LENGTH('dbo.cv_data','roll_elev_pos_data_rolling') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_roll_elev_pos_data_rolling sysname;
    SELECT @d_cv_data_roll_elev_pos_data_rolling = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'roll_elev_pos_data_rolling','ColumnId');
    IF @d_cv_data_roll_elev_pos_data_rolling IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_roll_elev_pos_data_rolling + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [roll_elev_pos_data_rolling];
    PRINT '  삭제 cv_data.roll_elev_pos_data_rolling';
END
ELSE
    PRINT '  (이미 없음) cv_data.roll_elev_pos_data_rolling';
GO
IF COL_LENGTH('dbo.cv_data','roll_clamp_pos_data_backward') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_roll_clamp_pos_data_backward sysname;
    SELECT @d_cv_data_roll_clamp_pos_data_backward = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'roll_clamp_pos_data_backward','ColumnId');
    IF @d_cv_data_roll_clamp_pos_data_backward IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_roll_clamp_pos_data_backward + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [roll_clamp_pos_data_backward];
    PRINT '  삭제 cv_data.roll_clamp_pos_data_backward';
END
ELSE
    PRINT '  (이미 없음) cv_data.roll_clamp_pos_data_backward';
GO
IF COL_LENGTH('dbo.cv_data','roll_clamp_pos_data_forward') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_roll_clamp_pos_data_forward sysname;
    SELECT @d_cv_data_roll_clamp_pos_data_forward = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'roll_clamp_pos_data_forward','ColumnId');
    IF @d_cv_data_roll_clamp_pos_data_forward IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_roll_clamp_pos_data_forward + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [roll_clamp_pos_data_forward];
    PRINT '  삭제 cv_data.roll_clamp_pos_data_forward';
END
ELSE
    PRINT '  (이미 없음) cv_data.roll_clamp_pos_data_forward';
GO
IF COL_LENGTH('dbo.cv_data','roll_sensor_data_height') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_roll_sensor_data_height sysname;
    SELECT @d_cv_data_roll_sensor_data_height = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'roll_sensor_data_height','ColumnId');
    IF @d_cv_data_roll_sensor_data_height IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_roll_sensor_data_height + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [roll_sensor_data_height];
    PRINT '  삭제 cv_data.roll_sensor_data_height';
END
ELSE
    PRINT '  (이미 없음) cv_data.roll_sensor_data_height';
GO
IF COL_LENGTH('dbo.cv_data','roll_elev_pos_data_asc') IS NOT NULL
BEGIN
    DECLARE @d_cv_data_roll_elev_pos_data_asc sysname;
    SELECT @d_cv_data_roll_elev_pos_data_asc = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.cv_data')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.cv_data'),'roll_elev_pos_data_asc','ColumnId');
    IF @d_cv_data_roll_elev_pos_data_asc IS NOT NULL EXEC('ALTER TABLE dbo.cv_data DROP CONSTRAINT [' + @d_cv_data_roll_elev_pos_data_asc + ']');
    ALTER TABLE dbo.cv_data DROP COLUMN [roll_elev_pos_data_asc];
    PRINT '  삭제 cv_data.roll_elev_pos_data_asc';
END
ELSE
    PRINT '  (이미 없음) cv_data.roll_elev_pos_data_asc';
GO

-- ===== job_mst (6개) =====
IF COL_LENGTH('dbo.job_mst','mes_write_yn') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_mes_write_yn sysname;
    SELECT @d_job_mst_mes_write_yn = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst'),'mes_write_yn','ColumnId');
    IF @d_job_mst_mes_write_yn IS NOT NULL EXEC('ALTER TABLE dbo.job_mst DROP CONSTRAINT [' + @d_job_mst_mes_write_yn + ']');
    ALTER TABLE dbo.job_mst DROP COLUMN [mes_write_yn];
    PRINT '  삭제 job_mst.mes_write_yn';
END
ELSE
    PRINT '  (이미 없음) job_mst.mes_write_yn';
GO
IF COL_LENGTH('dbo.job_mst','mes_error_cd') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_mes_error_cd sysname;
    SELECT @d_job_mst_mes_error_cd = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst'),'mes_error_cd','ColumnId');
    IF @d_job_mst_mes_error_cd IS NOT NULL EXEC('ALTER TABLE dbo.job_mst DROP CONSTRAINT [' + @d_job_mst_mes_error_cd + ']');
    ALTER TABLE dbo.job_mst DROP COLUMN [mes_error_cd];
    PRINT '  삭제 job_mst.mes_error_cd';
END
ELSE
    PRINT '  (이미 없음) job_mst.mes_error_cd';
GO
IF COL_LENGTH('dbo.job_mst','system_byte') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_system_byte sysname;
    SELECT @d_job_mst_system_byte = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst'),'system_byte','ColumnId');
    IF @d_job_mst_system_byte IS NOT NULL EXEC('ALTER TABLE dbo.job_mst DROP CONSTRAINT [' + @d_job_mst_system_byte + ']');
    ALTER TABLE dbo.job_mst DROP COLUMN [system_byte];
    PRINT '  삭제 job_mst.system_byte';
END
ELSE
    PRINT '  (이미 없음) job_mst.system_byte';
GO
IF COL_LENGTH('dbo.job_mst','s6f12_response') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_s6f12_response sysname;
    SELECT @d_job_mst_s6f12_response = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst'),'s6f12_response','ColumnId');
    IF @d_job_mst_s6f12_response IS NOT NULL EXEC('ALTER TABLE dbo.job_mst DROP CONSTRAINT [' + @d_job_mst_s6f12_response + ']');
    ALTER TABLE dbo.job_mst DROP COLUMN [s6f12_response];
    PRINT '  삭제 job_mst.s6f12_response';
END
ELSE
    PRINT '  (이미 없음) job_mst.s6f12_response';
GO
IF COL_LENGTH('dbo.job_mst','s6f11_yon') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_s6f11_yon sysname;
    SELECT @d_job_mst_s6f11_yon = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst'),'s6f11_yon','ColumnId');
    IF @d_job_mst_s6f11_yon IS NOT NULL EXEC('ALTER TABLE dbo.job_mst DROP CONSTRAINT [' + @d_job_mst_s6f11_yon + ']');
    ALTER TABLE dbo.job_mst DROP COLUMN [s6f11_yon];
    PRINT '  삭제 job_mst.s6f11_yon';
END
ELSE
    PRINT '  (이미 없음) job_mst.s6f11_yon';
GO
IF COL_LENGTH('dbo.job_mst','ceid_name') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_ceid_name sysname;
    SELECT @d_job_mst_ceid_name = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst'),'ceid_name','ColumnId');
    IF @d_job_mst_ceid_name IS NOT NULL EXEC('ALTER TABLE dbo.job_mst DROP CONSTRAINT [' + @d_job_mst_ceid_name + ']');
    ALTER TABLE dbo.job_mst DROP COLUMN [ceid_name];
    PRINT '  삭제 job_mst.ceid_name';
END
ELSE
    PRINT '  (이미 없음) job_mst.ceid_name';
GO

-- ===== job_mst_his (6개) =====
IF COL_LENGTH('dbo.job_mst_his','mes_write_yn') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_his_mes_write_yn sysname;
    SELECT @d_job_mst_his_mes_write_yn = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst_his')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst_his'),'mes_write_yn','ColumnId');
    IF @d_job_mst_his_mes_write_yn IS NOT NULL EXEC('ALTER TABLE dbo.job_mst_his DROP CONSTRAINT [' + @d_job_mst_his_mes_write_yn + ']');
    ALTER TABLE dbo.job_mst_his DROP COLUMN [mes_write_yn];
    PRINT '  삭제 job_mst_his.mes_write_yn';
END
ELSE
    PRINT '  (이미 없음) job_mst_his.mes_write_yn';
GO
IF COL_LENGTH('dbo.job_mst_his','mes_error_cd') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_his_mes_error_cd sysname;
    SELECT @d_job_mst_his_mes_error_cd = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst_his')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst_his'),'mes_error_cd','ColumnId');
    IF @d_job_mst_his_mes_error_cd IS NOT NULL EXEC('ALTER TABLE dbo.job_mst_his DROP CONSTRAINT [' + @d_job_mst_his_mes_error_cd + ']');
    ALTER TABLE dbo.job_mst_his DROP COLUMN [mes_error_cd];
    PRINT '  삭제 job_mst_his.mes_error_cd';
END
ELSE
    PRINT '  (이미 없음) job_mst_his.mes_error_cd';
GO
IF COL_LENGTH('dbo.job_mst_his','system_byte') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_his_system_byte sysname;
    SELECT @d_job_mst_his_system_byte = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst_his')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst_his'),'system_byte','ColumnId');
    IF @d_job_mst_his_system_byte IS NOT NULL EXEC('ALTER TABLE dbo.job_mst_his DROP CONSTRAINT [' + @d_job_mst_his_system_byte + ']');
    ALTER TABLE dbo.job_mst_his DROP COLUMN [system_byte];
    PRINT '  삭제 job_mst_his.system_byte';
END
ELSE
    PRINT '  (이미 없음) job_mst_his.system_byte';
GO
IF COL_LENGTH('dbo.job_mst_his','s6f12_response') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_his_s6f12_response sysname;
    SELECT @d_job_mst_his_s6f12_response = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst_his')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst_his'),'s6f12_response','ColumnId');
    IF @d_job_mst_his_s6f12_response IS NOT NULL EXEC('ALTER TABLE dbo.job_mst_his DROP CONSTRAINT [' + @d_job_mst_his_s6f12_response + ']');
    ALTER TABLE dbo.job_mst_his DROP COLUMN [s6f12_response];
    PRINT '  삭제 job_mst_his.s6f12_response';
END
ELSE
    PRINT '  (이미 없음) job_mst_his.s6f12_response';
GO
IF COL_LENGTH('dbo.job_mst_his','s6f11_yon') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_his_s6f11_yon sysname;
    SELECT @d_job_mst_his_s6f11_yon = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst_his')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst_his'),'s6f11_yon','ColumnId');
    IF @d_job_mst_his_s6f11_yon IS NOT NULL EXEC('ALTER TABLE dbo.job_mst_his DROP CONSTRAINT [' + @d_job_mst_his_s6f11_yon + ']');
    ALTER TABLE dbo.job_mst_his DROP COLUMN [s6f11_yon];
    PRINT '  삭제 job_mst_his.s6f11_yon';
END
ELSE
    PRINT '  (이미 없음) job_mst_his.s6f11_yon';
GO
IF COL_LENGTH('dbo.job_mst_his','ceid_name') IS NOT NULL
BEGIN
    DECLARE @d_job_mst_his_ceid_name sysname;
    SELECT @d_job_mst_his_ceid_name = dc.name FROM sys.default_constraints dc
     WHERE dc.parent_object_id = OBJECT_ID('dbo.job_mst_his')
       AND dc.parent_column_id = COLUMNPROPERTY(OBJECT_ID('dbo.job_mst_his'),'ceid_name','ColumnId');
    IF @d_job_mst_his_ceid_name IS NOT NULL EXEC('ALTER TABLE dbo.job_mst_his DROP CONSTRAINT [' + @d_job_mst_his_ceid_name + ']');
    ALTER TABLE dbo.job_mst_his DROP COLUMN [ceid_name];
    PRINT '  삭제 job_mst_his.ceid_name';
END
ELSE
    PRINT '  (이미 없음) job_mst_his.ceid_name';
GO

PRINT '컬럼 정리 완료';
GO