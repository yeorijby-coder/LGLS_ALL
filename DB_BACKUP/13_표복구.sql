/* ============================================================================
   LGLS ECS - 되돌리기 : 지운 표를 같은 자료형으로 다시 만든다
   작성 2026-09-10   (짝 : 11_표삭제.sql)

   값은 되살릴 수 없다. 어차피 이 현장에서 쓰지 않던 표라 빈 표면 충분하다.
   되돌릴 때는 프로그램도 2026-09-10 이전 빌드로 함께 되돌려야 짝이 맞는다.
   ============================================================================ */
USE LGLS_MCS_IO;
GO

IF OBJECT_ID('dbo.auto_sc_work','U') IS NULL
CREATE TABLE dbo.[auto_sc_work] (
    [wh_typ] varchar(4) NULL,
    [sc_no] varchar(10) NULL,
    [lugg_no] varchar(8) NULL,
    [job_typ] varchar(30) NULL,
    [wrk_typ] varchar(30) NULL,
    [ins_dt] date NULL,
    [job_seq] varchar(2) NULL,
    [log_seq] int NOT NULL,
    [bcr_bottom] varchar(10) NULL,
    [bcr_top] varchar(10) NULL,
    [pgr_nm] varchar(20) NULL,
    [log_kor] varchar(100) NULL,
    [track_from] varchar(10) NULL,
    [track_to] varchar(10) NULL,
    [job_sta] varchar(10) NULL,
    [rq_ins_id] varchar(20) NULL,
    [rq_ins_dt] date NULL,
    [eqp_typ] varchar(10) NULL,
    [spare1] varchar(50) NULL,
    [spare2] varchar(50) NULL
);
GO

IF OBJECT_ID('dbo.bcr_mst','U') IS NULL
CREATE TABLE dbo.[bcr_mst] (
    [wh_typ] varchar(2) NULL,
    [bcr_no] varchar(2) NULL,
    [bcr_mc_no] varchar(3) NULL,
    [upd_dt] datetime NULL,
    [upd_user_id] varchar(20) NULL,
    [use_yn] varchar(1) NULL,
    [plc_ip] varchar(15) NULL,
    [plc_port] varchar(5) NULL,
    [sock_timeout] varchar(4) NULL,
    [retry_yn] varchar(1) NULL,
    [connected_yn] varchar(1) NULL,
    [bcr_sta] varchar(2) NULL,
    [bcr_lev] varchar(2) NULL,
    [noread_cnt] varchar(4) NULL,
    [cmd_rq_id] varchar(10) NULL,
    [cmd_rq_yn] varchar(1) NULL,
    [suspend] varchar(1) NULL
);
GO

IF OBJECT_ID('dbo.client_pgr_nm','U') IS NULL
CREATE TABLE dbo.[client_pgr_nm] (
    [wh_typ] varchar(5) NULL,
    [pgr_nm] varchar(100) NULL,
    [pgr_nm_kor] varchar(100) NULL,
    [upd_dt] datetime2 NULL,
    [upd_id] varchar(50) NULL,
    [priority] varchar(3) NULL
);
GO

IF OBJECT_ID('dbo.dest_def','U') IS NULL
CREATE TABLE dbo.[dest_def] (
    [wh_typ] varchar(4) NULL,
    [dest_no] varchar(5) NULL,
    [dest_nm] varchar(30) NULL,
    [dest_typ] varchar(20) NULL,
    [cnt] int NULL,
    [cnt_user_def] int NULL,
    [upd_dt_user_id] date NULL,
    [ins_dt_user_id] varchar(20) NULL,
    [mes_rq] varchar(5) NULL
);
GO

IF OBJECT_ID('dbo.event_log','U') IS NULL
CREATE TABLE dbo.[event_log] (
    [wh_typ] varchar(4) NULL,
    [issue_date] date NOT NULL,
    [event_seq] varchar(30) NULL,
    [job_typ] varchar(5) NULL,
    [job_kind] varchar(5) NULL,
    [job_status] varchar(5) NULL,
    [mc_typ] varchar(10) NULL,
    [mc_no] varchar(10) NULL,
    [sour_site] varchar(10) NULL,
    [dest_site] varchar(10) NULL,
    [lugg_no] varchar(10) NULL,
    [cmd_sta] varchar(30) NULL,
    [bcr_bottom] varchar(20) NULL,
    [bcr_top] varchar(20) NULL,
    [mc_error_cd] varchar(10) NULL,
    [ins_dt] date NULL,
    [ins_user_id] varchar(20) NULL,
    [remarks] varchar(100) NULL
);
GO

IF OBJECT_ID('dbo.host_empty_plt','U') IS NULL
CREATE TABLE dbo.[host_empty_plt] (
    [wh_typ] varchar(2) NULL,
    [kind] varchar(2) NULL,
    [stn] varchar(3) NULL,
    [status] varchar(2) NULL,
    [lugg_no] varchar(4) NULL,
    [ins_id] varchar(20) NULL,
    [ins_dt] datetime NULL,
    [upd_id] varchar(20) NULL,
    [upd_dt] datetime NULL
);
GO

IF OBJECT_ID('dbo.mes_if_log','U') IS NULL
CREATE TABLE dbo.[mes_if_log] (
    [wh_typ] varchar(2) NULL,
    [log_date] varchar(8) NULL,
    [log_time] varchar(9) NULL,
    [mes_cmd] varchar(10) NULL,
    [sr_type] varchar(3) NULL,
    [message] varchar(4000) NULL,
    [lugg_no] varchar(4) NULL,
    [bcr_bottom] varchar(31) NULL,
    [bcr_top] varchar(31) NULL,
    [remarks] varchar(100) NULL,
    [ins_dt] datetime NULL,
    [ins_user_id] varchar(20) NULL,
    [upd_dt] datetime NULL,
    [upd_user_id] varchar(20) NULL
);
GO

IF OBJECT_ID('dbo.rtv_data','U') IS NULL
CREATE TABLE dbo.[rtv_data] (
    [wh_typ] varchar(4) NULL,
    [plc_no] varchar(5) NULL,
    [rtv_no] varchar(5) NULL,
    [auto_mode_rd] varchar(4) NULL,
    [waiting_order_rd] varchar(MAX) NULL,
    [sensor_rtv_rd] varchar(MAX) NULL,
    [err_code_rd] varchar(MAX) NULL,
    [pos_h_rd] varchar(MAX) NULL,
    [speed_rd] varchar(MAX) NULL,
    [fd_overtime_rd] varchar(MAX) NULL,
    [sensor_fk1_rd] varchar(MAX) NULL,
    [sensor_fk2_rd] varchar(MAX) NULL,
    [read_upd_dt] datetime2 NULL,
    [cmd_rq_yn] varchar(MAX) NULL,
    [cmd_rq_id] varchar(MAX) NULL,
    [od_rq_yn] varchar(MAX) NULL,
    [rtv_passcv_od] varchar(MAX) NULL,
    [rtv_dest_od] varchar(MAX) NULL,
    [write_upd_dt] datetime2 NULL,
    [lugg_od] varchar(MAX) NULL,
    [step] varchar(MAX) NULL,
    [job_typ_od] varchar(MAX) NULL,
    [depart_track] varchar(MAX) NULL,
    [arrive_track] varchar(MAX) NULL,
    [suspend] varchar(MAX) NULL,
    [od_user_id] varchar(MAX) NULL,
    [is_error_rd] varchar(MAX) NULL,
    [subsystem_status_rd] varchar(1) NULL,
    [location_01_rd] varchar(2) NULL,
    [location_02_rd] varchar(2) NULL,
    [location_03_rd] varchar(2) NULL,
    [pallet_on_vehicle_rd] varchar(4) NULL,
    [transfer_request_od] varchar(1) NULL,
    [transfer_ack_rd] varchar(1) NULL,
    [pallet_id_od] varchar(4) NULL,
    [from_01_od] varchar(2) NULL,
    [from_02_od] varchar(2) NULL,
    [from_03_od] varchar(2) NULL,
    [to_01_od] varchar(2) NULL,
    [to_02_od] varchar(2) NULL,
    [to_03_od] varchar(2) NULL,
    [load_complete_rd] varchar(1) NULL,
    [load_complete_ack_od] varchar(1) NULL,
    [unload_complete_rd] varchar(1) NULL,
    [unload_complete_ack_od] varchar(1) NULL,
    [alarm_set_report_rd] varchar(1) NULL,
    [alarm_set_report_ack_od] varchar(1) NULL,
    [alarm_set_code_rd] varchar(4) NULL,
    [alarm_reset_report_rd] varchar(1) NULL,
    [alarm_reset_report_ack_od] varchar(1) NULL,
    [alarm_reset_code_rd] varchar(4) NULL,
    [transfer_complete_location_01_rd] varchar(2) NULL,
    [transfer_complete_location_02_rd] varchar(2) NULL,
    [transfer_complete_location_03_rd] varchar(2) NULL,
    [COMPLETE_RD] varchar(1) NULL
);
GO

IF OBJECT_ID('dbo.sc_data','U') IS NULL
CREATE TABLE dbo.[sc_data] (
    [wh_typ] varchar(4) NULL,
    [plc_no] varchar(5) NULL,
    [sc_no] varchar(5) NULL,
    [sc_grp_no] varchar(5) NULL,
    [mc_no] varchar(5) NULL,
    [mc_no_nm] varchar(100) NULL,
    [auto_mode_rd] varchar(4) NULL,
    [ucstatus_rd] varchar(4) NULL,
    [online_mode_rd] varchar(1) NULL,
    [active_mode_rd] varchar(4) NULL,
    [sensor_fk_rd] varchar(1) NULL,
    [pos_h_rd] varchar(5) NULL,
    [pos_v_rd] varchar(5) NULL,
    [err_code_rd] varchar(4) NULL,
    [complete_rd] varchar(4) NULL,
    [job_typ_rd] varchar(4) NULL,
    [job_typ_od] varchar(4) NULL,
    [lugg_no_fk1_rd] varchar(4) NULL,
    [lugg_no_fk1_od] varchar(4) NULL,
    [itn_lugg_fk1] varchar(5) NULL,
    [start_bank_fk1_rd] varchar(3) NULL,
    [start_bank_fk1_od] varchar(3) NULL,
    [start_bay_fk1_rd] varchar(3) NULL,
    [start_bay_fk1_od] varchar(3) NULL,
    [start_level_fk1_rd] varchar(3) NULL,
    [start_level_fk1_od] varchar(3) NULL,
    [start_hspos_fk1_rd] varchar(4) NULL,
    [start_hspos_fk1_od] varchar(4) NULL,
    [dest_bank_fk1_rd] varchar(3) NULL,
    [dest_bank_fk1_od] varchar(3) NULL,
    [dest_bay_fk1_rd] varchar(3) NULL,
    [dest_bay_fk1_od] varchar(3) NULL,
    [dest_level_fk1_rd] varchar(3) NULL,
    [dest_level_fk1_od] varchar(3) NULL,
    [dest_hspos_fk1_rd] varchar(4) NULL,
    [dest_hspos_fk1_od] varchar(4) NULL,
    [lugg_no_fk2_rd] varchar(4) NULL,
    [lugg_no_fk2_od] varchar(4) NULL,
    [itn_lugg_fk2] varchar(5) NULL,
    [start_bank_fk2_rd] varchar(3) NULL,
    [start_bank_fk2_od] varchar(3) NULL,
    [start_bay_fk2_rd] varchar(3) NULL,
    [start_bay_fk2_od] varchar(3) NULL,
    [start_level_fk2_rd] varchar(3) NULL,
    [start_level_fk2_od] varchar(3) NULL,
    [start_hspos_fk2_rd] varchar(4) NULL,
    [start_hspos_fk2_od] varchar(4) NULL,
    [dest_bank_fk2_rd] varchar(3) NULL,
    [dest_bank_fk2_od] varchar(3) NULL,
    [dest_bay_fk2_rd] varchar(3) NULL,
    [dest_bay_fk2_od] varchar(3) NULL,
    [dest_level_fk2_rd] varchar(3) NULL,
    [dest_level_fk2_od] varchar(3) NULL,
    [dest_hspos_fk2_rd] varchar(4) NULL,
    [dest_hspos_fk2_od] varchar(4) NULL,
    [err_sta_fk1_rd] varchar(4) NULL,
    [err_sta_fk2_rd] varchar(4) NULL,
    [forkpos_fk1_rd] varchar(4) NULL,
    [forkpos_fk2_rd] varchar(4) NULL,
    [use_fk_rd] varchar(4) NULL,
    [use_fk_od] varchar(4) NULL,
    [stock_mode] varchar(4) NULL,
    [od_rq_yn] varchar(1) NULL,
    [od_rq_flag] varchar(1) NULL,
    [cmd_rq_id] varchar(10) NULL,
    [cmd_rq_yn] varchar(1) NULL,
    [host_send_yn] varchar(8) NULL,
    [host_err_send_yn] varchar(8) NULL,
    [suspend] varchar(2) NULL,
    [read_upd_dt] datetime2 NULL,
    [write_upd_dt] datetime2 NULL,
    [od_user_id] varchar(20) NULL,
    [od_upd_dt] datetime2 NULL,
    [write_continue_od] varchar(4) NULL,
    [write_flag_od] varchar(4) NULL,
    [user_command_od] varchar(4) NULL,
    [cv_workbench_rd] varchar(4) NULL,
    [cv_workbench_sub_rd] varchar(4) NULL,
    [sc_typ] varchar(20) NULL,
    [plt_info_rd] varchar(1) NULL,
    [sc_view_nm] varchar(5) NULL,
    [cmd_rq_parm] varchar(10) NULL,
    [sc_fire_od] varchar(10) NULL,
    [pool_position_rd] varchar(5) NULL,
    [pool_limit_rd] varchar(5) NULL,
    [pool_water_level] varchar(5) NULL,
    [sc_plt_job_typ_rd] varchar(1) NULL,
    [sc_plt_job_typ_od] varchar(1) NULL,
    [subsystem_status_rd] varchar(1) NULL,
    [location_01_rd] varchar(2) NULL,
    [location_02_rd] varchar(2) NULL,
    [location_03_rd] varchar(2) NULL,
    [pallet_on_vehicle_rd] varchar(4) NULL,
    [transfer_request_od] varchar(1) NULL,
    [transfer_ack_rd] varchar(1) NULL,
    [pallet_id_od] varchar(4) NULL,
    [from_01_od] varchar(2) NULL,
    [from_02_od] varchar(2) NULL,
    [from_03_od] varchar(2) NULL,
    [to_01_od] varchar(2) NULL,
    [to_02_od] varchar(2) NULL,
    [to_03_od] varchar(2) NULL,
    [load_complete_rd] varchar(1) NULL,
    [load_complete_ack_od] varchar(1) NULL,
    [unload_complete_rd] varchar(1) NULL,
    [unload_complete_ack_od] varchar(1) NULL,
    [alarm_set_report_rd] varchar(1) NULL,
    [alarm_set_report_ack_od] varchar(1) NULL,
    [alarm_set_code_rd] varchar(4) NULL,
    [alarm_reset_report_rd] varchar(1) NULL,
    [alarm_reset_report_ack_od] varchar(1) NULL,
    [alarm_reset_code_rd] varchar(4) NULL,
    [transfer_complete_location_01_rd] varchar(2) NULL,
    [transfer_complete_location_02_rd] varchar(2) NULL,
    [transfer_complete_location_03_rd] varchar(2) NULL
);
GO

IF OBJECT_ID('dbo.wait_track','U') IS NULL
CREATE TABLE dbo.[wait_track] (
    [wh_typ] varchar(3) NULL,
    [dest_pos] varchar(5) NULL,
    [wait_track] varchar(5) NULL,
    [sc_no] varchar(10) NULL
);
GO

IF OBJECT_ID('dbo.wc_data','U') IS NULL
CREATE TABLE dbo.[wc_data] (
    [wh_typ] varchar(2) NULL,
    [wc_mc_no] varchar(3) NULL,
    [weight_rcv_val] varchar(7) NULL,
    [wc_no] varchar(2) NULL,
    [plc_no] varchar(2) NULL,
    [weight_read_sta] varchar(1) NULL,
    [cmd_rq_id] varchar(10) NULL,
    [cmd_rq_yn] varchar(1) NULL,
    [suspend] varchar(1) NULL,
    [chk_bypass_yn] varchar(1) NULL,
    [od_rq_id] varchar(10) NULL
);
GO

IF OBJECT_ID('dbo.wc_his','U') IS NULL
CREATE TABLE dbo.[wc_his] (
    [wh_typ] varchar(2) NULL,
    [wc_no] varchar(2) NULL,
    [wc_mc_no] varchar(3) NULL,
    [weight_read_sta] varchar(1) NULL,
    [weight_rcv_val] varchar(7) NULL,
    [remarks] varchar(100) NULL,
    [ins_dt] datetime NULL
);
GO
