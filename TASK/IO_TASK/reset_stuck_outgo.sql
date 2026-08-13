-- =====================================================================
-- [LGLS] 이중입고/공출고 테스트 후 "꼬인 출고 잔류" 진단·정리 스크립트
--   증상: 출고작업이 JOB_STATUS='15'(CV 구동중)로 잔류 + CV_DATA에 화물감지 잔류
--   원인: 테스트 작업이 완결 안 되고 중단 → JOB_MST / SC_DATA_LGLS / CV_DATA 상태 불일치
--         → RGV 배정 SQL(cThread_R.cs:1285: CV_DATA에 LUGG 있으면 배정 제외)이 막힘
--   대상 DB: localhost\SQLEXPRESS / LGLS_MCS_IO
--   사용: sqlcmd -S "localhost\SQLEXPRESS" -d LGLS_MCS_IO -U LGLS_IO -P LGLS_IO -i reset_stuck_outgo.sql
-- =====================================================================

SET NOCOUNT ON;

-- ── 1) 진단: 잔류 출고작업 (JOB_STATUS=15 CV구동중, JOB_TYP=2 출고) ──────────
PRINT '--- 잔류 출고작업 (JOB_STATUS=15) ---';
SELECT LUGG_NO, JOB_STATUS, JOB_TYP, START_LOCATION, DEST_LOCATION
  FROM JOB_MST WHERE JOB_STATUS='15' AND JOB_TYP='2';

-- ── 2) 진단: JOB_MST에 대응 JOB이 없는 CV_DATA 유령 파렛트 ───────────────────
PRINT '--- 유령 CV 파렛트 (JOB_MST 미존재) ---';
SELECT CD.MC_NO, CD.LUGG_NO_RD
  FROM CV_DATA CD
 WHERE CD.LUGG_NO_RD IS NOT NULL AND CD.LUGG_NO_RD NOT IN ('0','')
   AND NOT EXISTS (SELECT 1 FROM JOB_MST JM WHERE JM.LUGG_NO = CD.LUGG_NO_RD);

-- ── 3) 정리 (트랜잭션) ────────────────────────────────────────────────────
--   ※ 운영 데이터이므로, 위 진단 결과를 확인한 뒤에만 아래 BEGIN~COMMIT 을 실행하세요.
/*
BEGIN TRAN;

  -- 3-1) CV_DATA: 유령 파렛트 + 잔류 출고 파렛트 클리어 (화물감지 해제)
  UPDATE CV_DATA SET LUGG_NO_RD='0'
   WHERE LUGG_NO_RD IS NOT NULL AND LUGG_NO_RD NOT IN ('0','')
     AND ( NOT EXISTS (SELECT 1 FROM JOB_MST JM WHERE JM.LUGG_NO = CV_DATA.LUGG_NO_RD)   -- 유령
        OR EXISTS (SELECT 1 FROM JOB_MST JM WHERE JM.LUGG_NO = CV_DATA.LUGG_NO_RD AND JM.JOB_STATUS='15' AND JM.JOB_TYP='2') );  -- 잔류출고
  PRINT 'CV_DATA 클리어: ' + CAST(@@ROWCOUNT AS VARCHAR);

  -- 3-2) SC_DATA_LGLS: 잔류 출고작업의 크레인 출고지시(od) 클리어
  UPDATE SC_DATA_LGLS SET lugg_no_fk1_od='0000'
   WHERE lugg_no_fk1_od IN (SELECT LUGG_NO FROM JOB_MST WHERE JOB_STATUS='15' AND JOB_TYP='2');
  PRINT 'SC_DATA od 클리어: ' + CAST(@@ROWCOUNT AS VARCHAR);

  -- 3-3) JOB_MST: 미완결 출고작업(CV구동중) 삭제
  DELETE FROM JOB_MST WHERE JOB_STATUS='15' AND JOB_TYP='2';
  PRINT 'JOB_MST 삭제: ' + CAST(@@ROWCOUNT AS VARCHAR);

COMMIT;
PRINT '=== 정리 완료 ===';
*/
