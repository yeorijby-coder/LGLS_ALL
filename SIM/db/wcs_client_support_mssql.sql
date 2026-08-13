-- =====================================================================
-- WCS Client(D:\project\LGLS\CPlusPlus\LGLS_CLIENT\Ecs) 지원 스키마 보강
--   (LGLS_MCS_IO, SQL Server 2008)
--
-- 1) cv_data — 클라이언트 조회/기능 컬럼 6종 (LFC 계열 화면 기능 유지)
-- 2) wc_data — 클라이언트 CWC_DATA 기대 컬럼
-- 3) suspend 값 정규화 — 클라이언트는 '0'/'1' 숫자 토글(1-SUSPEND) 규약
-- 4) cell_mst / cell_dtl — 랙 조회 대화상자(ViewRackDlg)용 최소 스키마+시드
--    (재고 관리는 범위 외 — 화면 동작용 좌표 마스터만. Bank 01~10 × Bay 01~10 × Lev 01~05)
-- =====================================================================
SET NOCOUNT ON;

-- 1) cv_data 클라이언트 컬럼
IF COL_LENGTH('cv_data','is_turn_rd')             IS NULL ALTER TABLE cv_data ADD is_turn_rd             VARCHAR(1)  DEFAULT '0';
IF COL_LENGTH('cv_data','wait_time_rd')           IS NULL ALTER TABLE cv_data ADD wait_time_rd           VARCHAR(4)  DEFAULT '0';
IF COL_LENGTH('cv_data','suspend')                IS NULL ALTER TABLE cv_data ADD suspend                VARCHAR(1)  DEFAULT '0';
IF COL_LENGTH('cv_data','item_no')                IS NULL ALTER TABLE cv_data ADD item_no                VARCHAR(20) DEFAULT ' ';
IF COL_LENGTH('cv_data','roll_elev_pos_data_asc') IS NULL ALTER TABLE cv_data ADD roll_elev_pos_data_asc VARCHAR(2)  DEFAULT '0';
IF COL_LENGTH('cv_data','sc_plt_job_typ')         IS NULL ALTER TABLE cv_data ADD sc_plt_job_typ         VARCHAR(1)  DEFAULT '0';
GO
UPDATE cv_data SET is_turn_rd='0', wait_time_rd='0', suspend='0', item_no=' ',
                   roll_elev_pos_data_asc='0', sc_plt_job_typ='0'
 WHERE wh_typ='10' AND (is_turn_rd IS NULL OR suspend IS NULL OR wait_time_rd IS NULL
                        OR item_no IS NULL OR roll_elev_pos_data_asc IS NULL OR sc_plt_job_typ IS NULL);
GO

-- 2) wc_data 클라이언트 컬럼
IF COL_LENGTH('wc_data','wc_no')           IS NULL ALTER TABLE wc_data ADD wc_no           VARCHAR(2)  DEFAULT '01';
IF COL_LENGTH('wc_data','plc_no')          IS NULL ALTER TABLE wc_data ADD plc_no          VARCHAR(2)  DEFAULT '01';
IF COL_LENGTH('wc_data','weight_read_sta') IS NULL ALTER TABLE wc_data ADD weight_read_sta VARCHAR(1)  DEFAULT '0';
IF COL_LENGTH('wc_data','cmd_rq_id')       IS NULL ALTER TABLE wc_data ADD cmd_rq_id       VARCHAR(10) DEFAULT '';
IF COL_LENGTH('wc_data','cmd_rq_yn')       IS NULL ALTER TABLE wc_data ADD cmd_rq_yn       VARCHAR(1)  DEFAULT 'N';
IF COL_LENGTH('wc_data','suspend')         IS NULL ALTER TABLE wc_data ADD suspend         VARCHAR(1)  DEFAULT '0';
IF COL_LENGTH('wc_data','chk_bypass_yn')   IS NULL ALTER TABLE wc_data ADD chk_bypass_yn   VARCHAR(1)  DEFAULT 'N';
GO

-- 3) suspend 정규화 (클라이언트 토글: SET SUSPEND = 1 - SUSPEND)
UPDATE rtv_data SET suspend='0' WHERE suspend IS NULL OR suspend NOT IN ('0','1');
UPDATE sc_data  SET suspend='0' WHERE suspend IS NULL OR suspend NOT IN ('0','1');
GO

-- 4) cell_mst / cell_dtl (랙 대화상자용)
IF OBJECT_ID('cell_mst','U') IS NULL
BEGIN
    CREATE TABLE cell_mst (
        wh_typ           VARCHAR(2),
        cell_sc_no       VARCHAR(3),
        cell_no          VARCHAR(8),
        bank             VARCHAR(2),
        bay              VARCHAR(2),
        lev              VARCHAR(2),
        gdbox_no         VARCHAR(10) DEFAULT '',
        gdbox_racd       VARCHAR(10) DEFAULT '',
        fire_detector_no VARCHAR(10) DEFAULT '',
        cell_fire_yn     VARCHAR(1)  DEFAULT 'N',
        cell_sta         VARCHAR(2)  DEFAULT '0',
        cell_use_def     VARCHAR(2)  DEFAULT '1',
        cell_use_yn      VARCHAR(1)  DEFAULT 'Y',
        aging_typ        VARCHAR(2)  DEFAULT '0',
        cell_typ         VARCHAR(2)  DEFAULT '0',
        bottom_tray      VARCHAR(10) DEFAULT '',
        top_tray         VARCHAR(10) DEFAULT '',
        upd_dt           DATETIME    DEFAULT GETDATE(),
        user_remarks     VARCHAR(100) DEFAULT '',
        remarks          VARCHAR(100) DEFAULT '',
        cell_status      VARCHAR(2)  DEFAULT '0'
    );

    -- 시드: Bank 01~10(S/C 901~905 가 2개 뱅크씩 담당) × Bay 01~10 × Lev 01~05
    DECLARE @bank INT, @bay INT, @lev INT;
    SET @bank = 1;
    WHILE @bank <= 10
    BEGIN
        SET @bay = 1;
        WHILE @bay <= 10
        BEGIN
            SET @lev = 1;
            WHILE @lev <= 5
            BEGIN
                INSERT INTO cell_mst (wh_typ, cell_sc_no, cell_no, bank, bay, lev)
                VALUES ('10',
                        '90' + CAST((@bank + 1) / 2 AS VARCHAR(1)),
                        RIGHT('00' + CAST(@bank AS VARCHAR(2)), 2)
                          + RIGHT('00' + CAST(@bay AS VARCHAR(2)), 2)
                          + RIGHT('00' + CAST(@lev AS VARCHAR(2)), 2),
                        RIGHT('00' + CAST(@bank AS VARCHAR(2)), 2),
                        RIGHT('00' + CAST(@bay  AS VARCHAR(2)), 2),
                        RIGHT('00' + CAST(@lev  AS VARCHAR(2)), 2));
                SET @lev = @lev + 1;
            END
            SET @bay = @bay + 1;
        END
        SET @bank = @bank + 1;
    END
END
GO

IF OBJECT_ID('cell_dtl','U') IS NULL
    CREATE TABLE cell_dtl (
        wh_typ         VARCHAR(2),
        cell_no        VARCHAR(8),
        cell_sc_no     VARCHAR(3),
        bank           VARCHAR(2),
        bay            VARCHAR(2),
        lev            VARCHAR(2),
        aging_start_dt VARCHAR(14),
        aging_end_dt   VARCHAR(14),
        ins_dt         DATETIME DEFAULT GETDATE(),
        ins_user_id    VARCHAR(20),
        upd_dt         DATETIME,
        upd_user_id    VARCHAR(20),
        bottom_tray    VARCHAR(10),
        top_tray       VARCHAR(10),
        user_remarks   VARCHAR(100),
        remarks        VARCHAR(100)
    );
GO

SELECT 'cv_data cols' t, COUNT(*) n FROM sys.columns WHERE object_id=OBJECT_ID('cv_data')
UNION ALL SELECT 'wc_data cols', COUNT(*) FROM sys.columns WHERE object_id=OBJECT_ID('wc_data')
UNION ALL SELECT 'cell_mst rows', COUNT(*) FROM cell_mst
UNION ALL SELECT 'cell_dtl rows', COUNT(*) FROM cell_dtl;
