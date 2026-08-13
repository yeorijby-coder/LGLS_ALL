-- =====================================================================
-- Original ECS (D:\project\LGLS\LGLS_1WH_original\Backup) — R-area monitor patch
--
-- The ORIGINAL ECS binary reads TB_FIELDBUSDRIVER.CONNECTSTRING as-is
-- (it has no HECS.ini/ApplyDriverType). Without R_START_ADDR/R_END_ADDR the
-- FEnet driver never registers the %R monitor block, and every tracking
-- read (PALLET_EXISTnn, PALLET_ON_VEHICLE = String observables on the R
-- area) throws KeyNotFoundException in m_MonitorMemory["R"] — so the
-- PPT scenario steps "ECS reads the tracking area within 0.5s"
-- (slides 8~15) can never complete and no transfer ever finishes.
--
-- This is a DB CONFIG VALUE (operational data), not an ECS source change.
-- Target: the instance the original ECS uses = localhost\sqlexpress, LglsMCS
-- Run  : sqlcmd -S localhost\sqlexpress -E -d LglsMCS -i patch_fieldbusdriver_r_area_sqlexpress.sql
-- =====================================================================
SET NOCOUNT ON;

UPDATE dbo.TB_FIELDBUSDRIVER
   SET CONNECTSTRING = 'CHANNEL_NO=51, MODE=-1, STATION_NO=255, MONITOR_INTERVAL=500, B_START_ADDR=0, B_END_ADDR=8192, W_START_ADDR=0, W_END_ADDR=8192, R_START_ADDR=0, R_END_ADDR=8192'
 WHERE USEFLAG = 'T'
   AND DRIVERTYPE = 'FENET'
   AND CONNECTSTRING NOT LIKE '%R_START_ADDR%';
GO

-- 원복(rollback):
-- UPDATE dbo.TB_FIELDBUSDRIVER
--    SET CONNECTSTRING = 'CHANNEL_NO=51, MODE=-1, STATION_NO=255, MONITOR_INTERVAL=500, B_START_ADDR=0, B_END_ADDR=8192, W_START_ADDR=0, W_END_ADDR=8192'
--  WHERE USEFLAG = 'T' AND DRIVERTYPE = 'FENET';

SELECT DRIVERNO, DRIVERTYPE, CONNECTSTRING, USEFLAG FROM dbo.TB_FIELDBUSDRIVER;
GO
