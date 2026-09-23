/* ============================================================================
   [LGLS 2026-09-23] EQP_MST 에서 CV / SC / RTV 행을 지운다. (사용자 지시)
                     남는 것은 EQP · HOST · HOST2 · SCH ★네 행★ 이다.

   왜 지워도 되나
     설비 접속은 마스터 PLC ★한 소켓★ 이다. C/V 15 · S/C 5 · RTV 1 행은
     모두 같은 IP/PORT 를 들고 있는 ★논리 설비 목록★ 이었을 뿐, 접속 단위가 아니다.
     접속 단위는 이제 EQP 행 하나가 대표한다.

   ★먼저 확인할 것★ - 프로그램을 같이 올려야 한다
     운전 화면(Ecs)의 C/V · S/C · RTV 조회가 종전에는 EQP_MST 를 PLC_NO 로
     INNER JOIN 했다. 그 상태에서 이 SQL 을 돌리면 조인이 0건이 되어
     ★화면에서 설비가 통째로 사라진다★.
     EQP 행 하나를 보도록 고친 Ecs.exe (커밋 387c4a6 이후) 로 올린 뒤 실행한다.

   지우기 전 값은 260923_EQP_MST_01_원상복구.sql 에 그대로 적어 두었다.
   ============================================================================ */

SET NOCOUNT ON;

DECLARE @WH_TYP varchar(10) = '10';   /* 창고 구분 - 현장 값에 맞춘다 */

/* ── 0) EQP 행이 없으면 지우지 않는다 (대표 행이 먼저 있어야 한다) ── */
IF NOT EXISTS (SELECT 1 FROM EQP_MST WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'EQP')
BEGIN
    RAISERROR('EQP 행이 없습니다. 260923_EQP_MST_02_EQP행_INSERT.sql 을 먼저 실행하십시오.', 16, 1);
    RETURN;
END

BEGIN TRAN;

/* ── 1) 지우기 전 사진을 남긴다 (되돌릴 때 쓴다) ──────────────────── */
IF OBJECT_ID('EQP_MST_BAK_20260923') IS NULL
    SELECT * INTO EQP_MST_BAK_20260923
      FROM EQP_MST
     WHERE WH_TYP = @WH_TYP AND EQP_TYP IN ('CV','SC','RTV');

/* ── 2) 설비 행 삭제 ─────────────────────────────────────────────── */
DELETE FROM EQP_MST
 WHERE WH_TYP = @WH_TYP AND EQP_TYP IN ('CV','SC','RTV');

COMMIT;

/* ── 확인 : 네 행만 남아야 한다 ──────────────────────────────────── */
SELECT EQP_TYP, PLC_NO, PLC_IP, PLC_PORT, USE_YN, CONNECTED_YN, REMARKS
  FROM EQP_MST
 WHERE WH_TYP = @WH_TYP
 ORDER BY CASE EQP_TYP WHEN 'EQP' THEN 1 WHEN 'HOST' THEN 2 WHEN 'HOST2' THEN 3
                       WHEN 'SCH' THEN 4 ELSE 9 END, PLC_NO;

SELECT COUNT(*) AS 남은_행수 FROM EQP_MST WHERE WH_TYP = @WH_TYP;

/* 되돌리려면
     INSERT INTO EQP_MST SELECT * FROM EQP_MST_BAK_20260923;
   또는 260923_EQP_MST_01_원상복구.sql 을 실행한다. */
