-- [LGLS 2026-09-03] SC_HS_DEF 원상 복구
--   fix_sc_hs_def_20260903.sql 로 바꿨던 것을 되돌린다.
--   이유 : 스케줄러(cThread_SC.cs 등)가 이 표를 INNER JOIN 으로 쓰고 있어
--          값 변경이 운전에 영향을 줄 수 있다. 크레인 수동 화면의 H/S 표시는
--          이 표 대신 CV_DATA(트랙 이름의 S/C측·RGV측)에서 읽도록 클라이언트를 고친다.
USE LGLS_MCS_IO;
SET NOCOUNT ON;
DELETE FROM SC_HS_DEF WHERE WH_TYP = '10';
INSERT INTO SC_HS_DEF (WH_TYP, SC_NO, HS_NO, HS_MC_NO, DEST_DAT_TOKEN, DEST_DEF_DAT, HS_USE_YN)
VALUES ('10','901','01','115', ',', '102, 105', 'Y'),
       ('10','901','02','114', '',  '',         'Y'),
       ('10','902','01','125', ',', '102, 105', 'Y'),
       ('10','902','02','124', '',  '',         'Y');
SELECT SC_NO, HS_NO, HS_MC_NO, DEST_DEF_DAT FROM SC_HS_DEF WHERE WH_TYP='10' ORDER BY SC_NO, HS_NO;
