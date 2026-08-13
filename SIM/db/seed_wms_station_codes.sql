-- =====================================================================
-- WMS station codes seed (TB_CODEMASTER, systemcd='C', codeclass='022')
--
-- ECS resolves the 3-digit source/dest station of a WMS 'O' (work order)
-- message against TB_CODEMASTER codeclass '022' (getPositionInfoList).
-- The restored LglsMCS DB has no '022' rows, so O messages are ignored
-- until this seed is applied.
--
--   CODE       : 3-digit station code used in WMS messages
--   ATTRIBUTE1 : zone code (codeclass 001: KR00 in-zone / KR01 hi-rack /
--                KR02 out-zone / KR03 picking zone)
--   ATTRIBUTE2 : equipment id (CNVSEM:n, see TB_EQUIPMENT/TB_TOTALPATH)
--   ATTRIBUTE3 : location id  (TB_LOCATION OWNERID,  CONVEYOR:nn)
--   ATTRIBUTE4 : sub location (TB_LOCATION LOCATIONID, PORT:tt)
--   ATTRIBUTE5 : picking-zone ingo redirect code (unused = blank)
--
-- Code scheme: 101..105 = C/V#11..#15
--              (matches GlobalConstant.CONVEYOR11_WMS_CODE='101')
--              200      = Hi-Rack (bank/bay/level taken from message)
-- Run: sqlcmd -S localhost\sqlexpress -E -d LglsMCS -i seed_wms_station_codes.sql
-- =====================================================================
SET NOCOUNT ON;

DELETE FROM dbo.TB_CODEMASTER WHERE SYSTEMCD='C' AND CODECLASS='022';

INSERT INTO dbo.TB_CODEMASTER (SYSTEMCD, CODECLASS, CODE, VALUE, DISABLED, REMARK,
    ATTRIBUTE1, ATTRIBUTE2, ATTRIBUTE3, ATTRIBUTE4, ATTRIBUTE5, ATTRIBUTE6, ATTRIBUTE7, ATTRIBUTE8)
VALUES
 ('C','022','101','C/V#11 IN/OUT (TR#22)','N','', 'KR00','CNVSEM:1','CONVEYOR:11','PORT:22','','','',''),
 ('C','022','102','C/V#12 IN (TR#24)'    ,'N','', 'KR00','CNVSEM:2','CONVEYOR:12','PORT:24','','','',''),
 ('C','022','103','C/V#13 OUT (TR#26)'   ,'N','', 'KR02','CNVSEM:2','CONVEYOR:13','PORT:26','','','',''),
 ('C','022','104','C/V#14 OUT (TR#29)'   ,'N','', 'KR02','CNVSEM:3','CONVEYOR:14','PORT:29','','','',''),
 ('C','022','105','C/V#15 IN (TR#30)'    ,'N','', 'KR00','CNVSEM:3','CONVEYOR:15','PORT:30','','','',''),
 ('C','022','201','Hi-Rack S/C#1 (Bank01,02)','N','', 'KR01','STKSEM:1','','','','','',''),
 ('C','022','202','Hi-Rack S/C#2 (Bank03,04)','N','', 'KR01','STKSEM:2','','','','','',''),
 ('C','022','203','Hi-Rack S/C#3 (Bank05,06)','N','', 'KR01','STKSEM:3','','','','','',''),
 ('C','022','204','Hi-Rack S/C#4 (Bank07,08)','N','', 'KR01','STKSEM:4','','','','','',''),
 ('C','022','205','Hi-Rack S/C#5 (Bank09,10)','N','', 'KR01','STKSEM:5','','','','','',''),
 ('C','022','ZZ' ,'WMS Station Codes'    ,'N','', 'zone','equipment','location','subloc','redirect','','','');
GO

SELECT CODE, VALUE, ATTRIBUTE1, ATTRIBUTE2, ATTRIBUTE3, ATTRIBUTE4
  FROM dbo.TB_CODEMASTER WHERE SYSTEMCD='C' AND CODECLASS='022' ORDER BY CODE;
GO
