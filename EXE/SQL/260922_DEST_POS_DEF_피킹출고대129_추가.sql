-- =====================================================================
-- [LGLS 2026-09-22] DEST_POS_DEF : 피킹 출고대 129 추가 (사용자 지시)
--   수동지시(반자동) 창의 도착지 목록이 DEST_POS_DEF 에서 나온다.
--   129(C/V#14, IMS102, TR#29 피킹 출고대)가 없어서 도착지에 입고대 130 이 떠 있었다.
--   130 은 C/V#15 피킹 ★입고★ 대라 도착지가 될 수 없다.
--   GROUP_NO : IN = 입고대 / OUT = 출고대 / SC = 크레인
--   여러 번 돌려도 안전하다(이미 있으면 건너뛴다).
-- =====================================================================
USE LGLS_MCS_IO;
GO

IF NOT EXISTS (SELECT 1 FROM DEST_POS_DEF WHERE MC_NO = '129')
BEGIN
    INSERT INTO DEST_POS_DEF (TRACK_NO, MC_NO, GROUP_NO, REMARKS)
    VALUES ('1029', '129', 'OUT', 'IMS102 C/V#14 피킹출고 TR#29');
    PRINT 'DEST_POS_DEF : 129 (피킹 출고대) 추가';
END
ELSE
    PRINT 'DEST_POS_DEF : 129 이미 있음 - 건너뜀';
GO

-- 확인
SELECT TRACK_NO, MC_NO, GROUP_NO, REMARKS FROM DEST_POS_DEF ORDER BY GROUP_NO, MC_NO;
GO
