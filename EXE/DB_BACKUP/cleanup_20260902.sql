-- [LGLS 2026-09-02] 서버 이관 전 정리 

USE LGLS_MCS_IO;

DROP TABLE cell_mes_req;
DROP TABLE cv_data_bak_lgls;
DROP TABLE cv_data_pb;
DROP TABLE dn_dtl;
DROP TABLE dn_mst;
DROP TABLE if_lugg_sta;
DROP TABLE if_lugg_sta_his;
DROP TABLE if_mc_sta;
DROP TABLE if_mc_sta_his;
DROP TABLE job_mst_bak_20260822;
DROP TABLE job_mst_stale_20260822;
DROP TABLE rtv_data_pb;
DROP TABLE rtv_hori_def;
DROP TABLE rtv_hs_def;
DROP TABLE rtv_sch_def;
DROP TABLE rtv_sch_prt;
DROP TABLE sc_data_murata;
DROP TABLE sc_data_murata_pb;
DROP TABLE seq_no_gnr;
DROP TABLE terminal_display;
DROP TABLE up_down;
DROP TABLE wcs_wrk_log;

TRUNCATE TABLE wcs_log_pgr;
TRUNCATE TABLE host_if_log;
TRUNCATE TABLE job_mst_his;
TRUNCATE TABLE wcs_client_log;
TRUNCATE TABLE eqp_err_his;

DBCC SHRINKDATABASE (LGLS_MCS_IO, 10);
