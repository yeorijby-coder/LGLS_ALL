-- [LGLS 2026-11-12] JOB_MST 에 작업이 접수(INSERT)될 때 JOB_MST_HIS 에 첫 이력을 남긴다 (사용자 지시).
--   종전 트리거 trg_JOB_MST_StatusHis 는 상태 변경(UPDATE)만 기록해서, 접수 직후(99 등) 행이 이력에 없었다.
--   HOST_TASK · Client(수동/반자동) · 시험 화면 등 작업을 만드는 모든 경로를 한 곳에서 잡는다.
--   여러 번 실행해도 된다. 실행 : sqlcmd -S <서버> -d LGLS_MCS_IO -E -i trg_job_insert_his.sql
IF OBJECT_ID('trg_JOB_MST_InsertHis','TR') IS NOT NULL
    DROP TRIGGER trg_JOB_MST_InsertHis;
GO
CREATE TRIGGER trg_JOB_MST_InsertHis ON JOB_MST AFTER INSERT
AS
BEGIN
    SET NOCOUNT ON;

    -- 이력 기록 실패가 실제 작업 접수(JOB_MST INSERT)를 롤백시키지 않도록 보호
    BEGIN TRY
    INSERT INTO JOB_MST_HIS
        (ins_date, ins_time, wh_typ, lugg_no, start_pos, start_location, dest_pos, dest_location,
         turn, product_id, product_size, job_typ, bcr_top, bcr_bottom, cmd_sta,
         job_kind, job_status, job_priority, od_last_page, od_last_user, job_start_dt,
         upd_dt, upd_user_id, ins_dt, ins_user_id, remarks, tray_typ, tray_lev, hs_track_no, sc_no,
         duration_time, del_yn, fk_no, aging_time,
         pair_lugg_no, lot_no, cmp_step)
    SELECT
        CONVERT(varchar(8), GETDATE(), 112),                      -- ins_date  YYYYMMDD
        REPLACE(CONVERT(varchar(8), GETDATE(), 108), ':', ''),    -- ins_time  HHMMSS
        i.wh_typ, i.lugg_no, i.start_pos, i.start_location, i.dest_pos, i.dest_location,
        i.turn, i.product_id, i.product_size, i.job_typ, i.bcr_top, i.bcr_bottom, i.cmd_sta,
        i.job_kind, i.job_status, i.job_priority, i.od_last_page, i.od_last_user, i.job_start_dt,
        i.upd_dt, i.upd_user_id, i.ins_dt, i.ins_user_id, i.remarks, i.tray_typ, i.tray_lev, i.hs_track_no, i.sc_no,
        i.duration_time, i.del_yn, i.fk_no, i.aging_time,
        i.pair_lugg_no, i.lot_no, i.cmp_step
    FROM inserted i;
    END TRY
    BEGIN CATCH
        -- 이력 기록 실패는 무시 (작업 흐름 보호)
    END CATCH
END
GO
