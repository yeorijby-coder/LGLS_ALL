/* ============================================================================
   DEST_POS_DEF.REMARKS 를 IMS/프로토콜 문서에 맞춘다
   2026-09-30

   무엇이 틀렸나
     2026-09-22 에 넣은 값에서 입고/출고의 IMS 번호가 서로 뒤집혀 있었다.

       트랙 124 (제품 입고대)     : IMS104  →  IMS103
       트랙 126 (원부자재 불출대) : IMS103  →  IMS104

   무엇이 맞나 - IMS/프로토콜 문서의 Station Number 표

       101  외부 입고 전용 입출고대
       102  Picking 작업대
       103  제품 입고대
       104  원부자재 불출대

   프로그램 동작에는 영향이 없었다
     상위(WMS/IMS)와 주고받는 번호는 이 칸을 쓰지 않는다.
     WCS_TASK_HOST\modStationMap.cs 가 코드로 매핑하며, 그쪽은 처음부터
     문서와 같았다(103=제품 입고대 TR#24, 104=원부자재 불출대 TR#26).
     이 칸은 ★화면에 보이는 이름표★ 에만 쓰인다.
       · 작업 정보 창 / 작업 판넬의 출발지·도착지   (2026-09-29 부터)
       · 수동 지시 창의 출발/도착 목록              (2026-09-16 부터)
       · 설비 상태창의 도착지

   칸을 늘린다
     문서의 이름이 길어 varchar(30) 에 들어가지 않는다("외부 입고 전용 입출고대").
     60 으로 늘린다. 값을 줄이는 것이 아니라 늘리는 것이라 되돌릴 일이 없다.

   작업 정보 창과 작업 판넬의 ★이름★ 은 EcsDefine.xml 에서 온다.
   이 표의 이름은 수동 지시 창과 설비 상태창에서 쓰이므로, 같은 말로 맞춰 둔다.

   몇 번을 돌려도 같은 결과가 된다.
   ============================================================================ */

USE [LGLS_MCS_IO]
GO

SET NOCOUNT ON
GO

PRINT '── 고치기 전 ──'
SELECT MC_NO, REMARKS FROM DEST_POS_DEF WHERE REMARKS LIKE 'IMS%' ORDER BY MC_NO
GO

/* ── 1. 칸 늘리기 ─────────────────────────────────────────────────────── */
IF EXISTS (SELECT 1 FROM sys.columns
            WHERE object_id = OBJECT_ID('DEST_POS_DEF')
              AND name = 'REMARKS'
              AND max_length < 60)
BEGIN
    ALTER TABLE [dbo].[DEST_POS_DEF] ALTER COLUMN [REMARKS] [varchar](60) NULL
    PRINT 'REMARKS 를 varchar(60) 으로 늘렸습니다.'
END
ELSE
    PRINT 'REMARKS 는 이미 60 이상입니다 - 그대로 둡니다.'
GO

/* ── 2. 값 맞추기 ─────────────────────────────────────────────────────── */
UPDATE DEST_POS_DEF SET REMARKS = 'IMS101 C/V#11 외부 입고 전용 입출고대 TR#22' WHERE MC_NO = '122'
UPDATE DEST_POS_DEF SET REMARKS = 'IMS103 C/V#12 제품 입고대 TR#24'             WHERE MC_NO = '124'
UPDATE DEST_POS_DEF SET REMARKS = 'IMS104 C/V#13 원부자재 불출대 TR#26'         WHERE MC_NO = '126'
UPDATE DEST_POS_DEF SET REMARKS = 'IMS102 C/V#14 Picking 작업대 TR#29'          WHERE MC_NO = '129'
UPDATE DEST_POS_DEF SET REMARKS = 'IMS102 C/V#15 Picking 작업대 TR#30'          WHERE MC_NO = '130'
GO

PRINT ''
PRINT '── 고친 뒤 ──'
SELECT MC_NO, REMARKS FROM DEST_POS_DEF WHERE REMARKS LIKE 'IMS%' ORDER BY MC_NO
GO

PRINT ''
PRINT '확인 : 작업 정보 창과 작업 판넬의 출발지/도착지가 아래처럼 보이면 된다.'
PRINT '         외부 입고 전용 입출고대[101] TR#22'
PRINT '         제품 입고대[103] TR#24'
PRINT '         원부자재 불출대[104] TR#26'
PRINT '         Picking 작업대[102] TR#29 / TR#30'
GO

/* ── 되돌리려면(2026-09-22 판) ──────────────────────────────────────────
UPDATE DEST_POS_DEF SET REMARKS = 'IMS101 C/V#11 입출고대 TR#22'   WHERE MC_NO = '122';
UPDATE DEST_POS_DEF SET REMARKS = 'IMS104 C/V#12 입고대 TR#24'     WHERE MC_NO = '124';
UPDATE DEST_POS_DEF SET REMARKS = 'IMS103 C/V#13 출고대 TR#26'     WHERE MC_NO = '126';
UPDATE DEST_POS_DEF SET REMARKS = 'IMS102 C/V#14 피킹출고 TR#29'   WHERE MC_NO = '129';
UPDATE DEST_POS_DEF SET REMARKS = 'IMS102 C/V#15 피킹입고 TR#30'   WHERE MC_NO = '130';
   칸 길이는 되돌리지 않아도 된다(늘린 것이라 탈이 없다).
   ------------------------------------------------------------------ */
