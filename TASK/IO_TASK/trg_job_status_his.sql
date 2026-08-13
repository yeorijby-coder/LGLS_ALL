-- [LGLS] JOB_MST.job_status 변경 시마다 JOB_MST_HIS 자동 기록 트리거 (SQL Server / LGLS_MCS_IO)
-- 작업로그(JOB_MST_HIS)에 하나의 작업(lugg_no)이 상태가 바뀔 때마다 이력을 남긴다.
-- 코드 로직을 건드리지 않고 모든 상태변경 경로를 커버한다.
IF OBJECT_ID('trg_JOB_MST_StatusHis','TR') IS NOT NULL
    DROP TRIGGER trg_JOB_MST_StatusHis;
GO
CREATE TRIGGER trg_JOB_MST_StatusHis ON JOB_MST AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON;
    IF NOT UPDATE(job_status) RETURN;

    -- 이력 기록 실패가 실제 작업(JOB_MST UPDATE)을 롤백시키지 않도록 보호
    BEGIN TRY
    INSERT INTO JOB_MST_HIS
        (ins_date, ins_time, wh_typ, lugg_no, start_pos, start_location, dest_pos, dest_location,
         turn, product_id, product_size, job_typ, bcr_top, bcr_bottom, mes_write_yn, cmd_sta,
         job_kind, job_status, job_priority, mes_error_cd, od_last_page, od_last_user, job_start_dt,
         upd_dt, upd_user_id, ins_dt, ins_user_id, remarks, tray_typ, tray_lev, hs_track_no, sc_no,
         duration_time, system_byte, s6f12_response, s6f11_yon, ceid_name, del_yn, fk_no, aging_time,
         pair_lugg_no, lot_no, cmp_step)
    SELECT
        CONVERT(varchar(8), GETDATE(), 112),                      -- ins_date  YYYYMMDD
        REPLACE(CONVERT(varchar(8), GETDATE(), 108), ':', ''),    -- ins_time  HHMMSS
        i.wh_typ, i.lugg_no, i.start_pos, i.start_location, i.dest_pos, i.dest_location,
        i.turn, i.product_id, i.product_size, i.job_typ, i.bcr_top, i.bcr_bottom, i.mes_write_yn, i.cmd_sta,
        i.job_kind, i.job_status, i.job_priority, i.mes_error_cd, i.od_last_page, i.od_last_user, i.job_start_dt,
        i.upd_dt, i.upd_user_id, i.ins_dt, i.ins_user_id, i.remarks, i.tray_typ, i.tray_lev, i.hs_track_no, i.sc_no,
        i.duration_time, i.system_byte, i.s6f12_response, i.s6f11_yon, i.ceid_name, i.del_yn, i.fk_no, i.aging_time,
        i.pair_lugg_no, i.lot_no, i.cmp_step
    FROM inserted i
    JOIN deleted  d ON i.wh_typ = d.wh_typ AND i.lugg_no = d.lugg_no
    WHERE ISNULL(CAST(i.job_status AS varchar(20)), '') <> ISNULL(CAST(d.job_status AS varchar(20)), '');
    END TRY
    BEGIN CATCH
        -- 이력 기록 실패는 무시 (작업 흐름 보호)
    END CATCH
END
GO
