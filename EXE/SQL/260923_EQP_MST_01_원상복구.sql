/* ============================================================================
   [LGLS 2026-09-23] EQP_MST - 2026-09-23 작업 ★이전 상태★ 로 되돌린다.

   되돌리는 내용
     · 이번에 넣은 EQP_TYP='EQP' 행을 지운다.
     · 지웠던 CV(15) · SC(5) · RTV(1) 행 21건을 그대로 다시 넣는다.
       (2026-09-23 작업 직전 값 그대로. UPD_DT 만 실행 시각으로 들어간다 -
        어차피 통신 TASK 가 곧 덮어쓰는 상태 값이다.)

   HOST / HOST2 / SCH 세 행은 이번 작업에서 값이 바뀌지 않았으므로 건드리지 않는다.

   같이 되돌려야 하는 프로그램 (SQL 만으로는 끝나지 않는다)
     · WCS_TASK_CV  : EQP 행을 함께 갱신하는 코드
     · Ecs(운전화면) : EQP 정보 창의 UNION ALL, Cv/Sc/Rtv 조회의 EQP_MST 조인
     커밋 387c4a6 이전 실행 파일로 되돌리면 된다.
   ============================================================================ */

SET NOCOUNT ON;

DECLARE @WH_TYP varchar(10) = '10';   /* 창고 구분 - 현장 값에 맞춘다 */

BEGIN TRAN;

/* ── 1) 이번에 넣은 EQP 행 제거 ─────────────────────────────────── */
DELETE FROM EQP_MST WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'EQP';

/* ── 2) CV / SC / RTV 21건 복원 (이미 있으면 넣지 않는다) ────────── */
;WITH SRC (EQP_TYP, PLC_NO, SC_GRP_NO, PLC_IP, PLC_PORT, PLC_PORT_FROM, PLC_PORT_TO,
           SOCK_TIMEOUT, RETRY_YN, USE_YN, CONNECTED_YN, REMARKS) AS
(
    SELECT 'CV' ,'01','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'02','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'03','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'04','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'05','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'06','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'07','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'08','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'09','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'10','01','127.0.0.1','2004','2004','' ,'2','N','Y','N','라인 C/V (게이트웨이 없음 - 표시용)' UNION ALL
    SELECT 'CV' ,'11','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','C/V#11 입출고대(TR#21~22)'   UNION ALL
    SELECT 'CV' ,'12','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','C/V#12 입고대(TR#23~24)'     UNION ALL
    SELECT 'CV' ,'13','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','C/V#13 출고대(TR#25~26)'     UNION ALL
    SELECT 'CV' ,'14','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','C/V#14 출고대(TR#27~29)'     UNION ALL
    SELECT 'CV' ,'15','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','C/V#15 입고대(TR#30~32)'     UNION ALL
    SELECT 'SC' ,'01','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','S/C#1 (Bank01,02)'           UNION ALL
    SELECT 'SC' ,'02','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','S/C#2 (Bank03,04)'           UNION ALL
    SELECT 'SC' ,'03','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','S/C#3 (Bank05,06)'           UNION ALL
    SELECT 'SC' ,'04','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','S/C#4 (Bank07,08)'           UNION ALL
    SELECT 'SC' ,'05','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','S/C#5 (Bank09,10)'           UNION ALL
    SELECT 'RTV','01','01','127.0.0.1','2004','2004','3001','2','Y','Y','N','RGV#1'
)
INSERT INTO EQP_MST
      (WH_TYP, EQP_TYP, PLC_NO, SC_GRP_NO, PLC_IP, PLC_PORT, PLC_PORT_FROM, PLC_PORT_TO,
       SOCK_TIMEOUT, RETRY_YN, USE_YN, CONNECTED_YN, UPD_DT, REMARKS)
SELECT @WH_TYP, S.EQP_TYP, S.PLC_NO, S.SC_GRP_NO, S.PLC_IP, S.PLC_PORT, S.PLC_PORT_FROM, S.PLC_PORT_TO,
       S.SOCK_TIMEOUT, S.RETRY_YN, S.USE_YN, S.CONNECTED_YN, GETDATE(), S.REMARKS
  FROM SRC S
 WHERE NOT EXISTS (SELECT 1 FROM EQP_MST E
                    WHERE E.WH_TYP = @WH_TYP AND E.EQP_TYP = S.EQP_TYP AND E.PLC_NO = S.PLC_NO);

COMMIT;

/* ── 확인 ────────────────────────────────────────────────────────── */
SELECT EQP_TYP, COUNT(*) AS 건수
  FROM EQP_MST WHERE WH_TYP = @WH_TYP
 GROUP BY EQP_TYP ORDER BY EQP_TYP;
