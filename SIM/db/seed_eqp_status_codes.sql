-- =====================================================================
-- Equipment status source seed (TB_CODEMASTER codeclass '024')
--
-- ExternalIFManager.regEqpStatus() -> CheckEquipmentStatus.createWmsStatusInfo()
-- builds the 40-char WMS status string from codeclass '024' rows:
--   ATTRIBUTE4='Y' rows emit  CODE(2) + available(1) + "0000"   (crane block, 7 chars)
--   ATTRIBUTE4<>'Y' rows emit available(1)                      (flag, 1 char)
--   availability query: SELECT available FROM {ATTR1} WHERE {ATTR2}='{ATTR3}'
--   ordered by ATTRIBUTE5.
-- Expected layout (= seed baseline "0100000020000003000000400000050000000000"):
--   [01 s 0000][02 s 0000][03 s 0000][04 s 0000][05 s 0000][f1..f5]  = 40 chars
-- The restored DB has only a dummy row, so no S(status) message is ever
-- produced. This seed restores the layout.
-- Run: sqlcmd -S localhost\sqlexpress -E -d LglsMCS -i seed_eqp_status_codes.sql
-- =====================================================================
SET NOCOUNT ON;

DELETE FROM dbo.TB_CODEMASTER WHERE SYSTEMCD='C' AND CODECLASS='024';

INSERT INTO dbo.TB_CODEMASTER (SYSTEMCD, CODECLASS, CODE, VALUE, DISABLED, REMARK,
    ATTRIBUTE1, ATTRIBUTE2, ATTRIBUTE3, ATTRIBUTE4, ATTRIBUTE5, ATTRIBUTE6, ATTRIBUTE7, ATTRIBUTE8)
VALUES
 ('C','024','01','S/C#1 status','N','','TB_EQUIPMENT','EQUIPMENTID','STKSEM:1','Y','01','','',''),
 ('C','024','02','S/C#2 status','N','','TB_EQUIPMENT','EQUIPMENTID','STKSEM:2','Y','02','','',''),
 ('C','024','03','S/C#3 status','N','','TB_EQUIPMENT','EQUIPMENTID','STKSEM:3','Y','03','','',''),
 ('C','024','04','S/C#4 status','N','','TB_EQUIPMENT','EQUIPMENTID','STKSEM:4','Y','04','','',''),
 ('C','024','05','S/C#5 status','N','','TB_EQUIPMENT','EQUIPMENTID','STKSEM:5','Y','05','','',''),
 ('C','024','F1','Flag ExtInputOnly'        ,'N','','TB_EQUIPMENT','EQUIPMENTID','CNVSEM:1','N','06','','',''),
 ('C','024','F2','Flag PickingWorkConveyor' ,'N','','TB_EQUIPMENT','EQUIPMENTID','CNVSEM:2','N','07','','',''),
 ('C','024','F3','Flag MaterialInputConveyor','N','','TB_EQUIPMENT','EQUIPMENTID','CNVSEM:3','N','08','','',''),
 ('C','024','F4','Flag InputConveyorMode'   ,'N','','TB_EQUIPMENT','EQUIPMENTID','RGVSEM:1','N','09','','',''),
 ('C','024','F5','Flag SC01ConveyorMode'    ,'N','','TB_EQUIPMENT','EQUIPMENTID','ECS:1','N','10','','','');
GO

-- getEQPStatus() casts AVAILABLE with (string)reader[...] BEFORE the null guard,
-- so a NULL value throws InvalidCastException and kills the whole status report.
UPDATE dbo.TB_EQUIPMENT SET AVAILABLE='0' WHERE AVAILABLE IS NULL;
GO

SELECT CODE, VALUE, ATTRIBUTE3, ATTRIBUTE4, ATTRIBUTE5
  FROM dbo.TB_CODEMASTER WHERE SYSTEMCD='C' AND CODECLASS='024' ORDER BY ATTRIBUTE5;
GO
