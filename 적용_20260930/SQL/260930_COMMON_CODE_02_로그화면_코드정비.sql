/* ============================================================================
   로그 화면의 고르는 목록(COMMON_CODE)을 지금 시스템에 맞춘다
   2026-09-30   사용자 지시

   고치는 것 셋

     1) 프로그램 로그 - 프로그램      PGR_NM
          종전 : CV_1그룹 / SC_1호기 / SC_2호기 / RTV_1호기 / BCR_1호기 / BCR_2호기 / IO_TASK
          지금 : HOST / EQP / IO_TASK
          설비별로 나뉘어 있었으나, 실제로 로그를 남기는 것은 TASK 세 가지다.
          BCR 은 이 현장에 없다.

          ※ 로그에 실제로 남는 이름은 스레드 단위라 여럿이다.
               EQP  : WCS_TASK_CV_COMM0, VEH_SC, VEH_RTV ...
               IO   : SCH_DBG ...
             그래서 Client 가 이름 생김새로 묶어 건다(WCS_TASK% / VEH_% / SCH% ...).
             기록하는 쪽은 건드리지 않았으므로 ★쌓여 있는 옛 로그도 그대로 조회된다★.

     2) 사용자 사용 로그 - 프로그램   CLIENT_PGR_NM
          지금 Client 에 있는 대화상자에 맞춘다.
          없어진 것은 감추고(N), 빠져 있던 것을 넣는다.

     3) 설비 에러 이력 - 설비 구분    EQP_TYP
          BCR 을 감춘다. 이 현장에는 CV / SC / RTV / HOST 뿐이다.

   지우지 않고 감춘다(CCD_CD_YN = 'N')
     옛 로그에 그 코드가 남아 있으면, 지워 버릴 경우 조회 결과에 이름이 뜨지 않고
     코드만 보인다. 감추면 고르는 목록에서만 빠지고 이름은 그대로 보인다.

   작업 구분(JOB_TYP)은 손대지 않는다
     프로그램 로그와 HOST 로그의 작업 구분을 이 표에 붙이도록 Client 를 고쳤다.
     작업 정보 창이 쓰던 것과 같은 목록이 그대로 나온다.

   몇 번을 돌려도 같은 결과가 된다.
   ============================================================================ */

USE [LGLS_MCS_IO]
GO

SET NOCOUNT ON
GO

PRINT '── 고치기 전 ──'
SELECT CDX_CD, CCD_CD, CCD_NM_KOR, CCD_CD_YN, CCD_EPR_ORD
  FROM COMMON_CODE
 WHERE CDX_CD IN ('PGR_NM', 'CLIENT_PGR_NM', 'EQP_TYP')
 ORDER BY CDX_CD, CCD_EPR_ORD
GO

/* ── 1. 프로그램 로그 - 프로그램 ─────────────────────────────────────── */

/*   종전 설비별 코드는 감춘다 */
UPDATE COMMON_CODE
   SET CCD_CD_YN = 'N'
 WHERE CDX_CD = 'PGR_NM'
   AND CCD_CD NOT IN ('HOST', 'EQP', 'IO_TASK')
GO

/*   쓸 것 셋을 넣거나 되살린다 */
MERGE COMMON_CODE AS T
USING (VALUES
        ('HOST',    'HOST',    1),
        ('EQP',     'EQP',     2),
        ('IO_TASK', 'IO_TASK', 3)
      ) AS S (CCD_CD, CCD_NM_KOR, CCD_EPR_ORD)
   ON  T.CDX_CD = 'PGR_NM'
   AND T.CCD_CD = S.CCD_CD
   AND T.WH_TYP = '10'
 WHEN MATCHED THEN
      UPDATE SET CCD_NM_KOR = S.CCD_NM_KOR, CCD_EPR_ORD = S.CCD_EPR_ORD, CCD_CD_YN = 'Y'
 WHEN NOT MATCHED THEN
      INSERT (CDX_CD, CCD_CD, CCD_NM_KOR, CCD_EPR_ORD, CCD_CD_YN, WH_TYP)
      VALUES ('PGR_NM', S.CCD_CD, S.CCD_NM_KOR, S.CCD_EPR_ORD, 'Y', '10');
GO

/* ── 2. 사용자 사용 로그 - 프로그램 ──────────────────────────────────── */

/*   지금 Client 에 없는 것은 감춘다 */
UPDATE COMMON_CODE
   SET CCD_CD_YN = 'N'
 WHERE CDX_CD = 'CLIENT_PGR_NM'
   AND CCD_CD NOT IN ('CViewJobListDlg', 'CManualJob', 'CManualSc', 'CManualRtv',
                      'CManualEmpty', 'CCvSkinDlg', 'CScSkinDlg', 'CRtvSkinDlg',
                      'CWcSkinDlg', 'CEqpSuspendDlg', 'CConfigLogDelete',
                      'CViewHostEmptyPltDlg', 'CViewSearchDlg',
                      'CSystemLoginDlg', 'CUserUserDlg', 'CEcsView')
GO

/*   지금 Client 가 실제로 기록하는 대화상자를 넣거나 되살린다 */
MERGE COMMON_CODE AS T
USING (VALUES
        ('CViewJobListDlg',      '작업 정보',        1),
        ('CManualJob',           '수동 작업',        2),
        ('CManualSc',            '수동 크레인',      3),
        ('CManualRtv',           '수동 RTV',         4),
        ('CManualEmpty',         '수동 공PLT',       5),
        ('CCvSkinDlg',           'C/V 상태',         6),
        ('CScSkinDlg',           'S/C 상태',         7),
        ('CRtvSkinDlg',          'RTV 상태',         8),
        ('CWcSkinDlg',           '작업대 상태',      9),
        ('CEqpSuspendDlg',       '설비 정지',       10),
        ('CViewHostEmptyPltDlg', '공PLT 작업',      11),
        ('CViewSearchDlg',       '찾기',            12),
        ('CConfigLogDelete',     '로그 삭제 설정',  13),
        ('CSystemLoginDlg',      '로그인',          14),
        ('CUserUserDlg',         '사용자 관리',     15),
        ('CEcsView',             '메인 화면',       16)
      ) AS S (CCD_CD, CCD_NM_KOR, CCD_EPR_ORD)
   ON  T.CDX_CD = 'CLIENT_PGR_NM'
   AND T.CCD_CD = S.CCD_CD
   AND T.WH_TYP = '10'
 WHEN MATCHED THEN
      UPDATE SET CCD_NM_KOR = S.CCD_NM_KOR, CCD_EPR_ORD = S.CCD_EPR_ORD, CCD_CD_YN = 'Y'
 WHEN NOT MATCHED THEN
      INSERT (CDX_CD, CCD_CD, CCD_NM_KOR, CCD_EPR_ORD, CCD_CD_YN, WH_TYP)
      VALUES ('CLIENT_PGR_NM', S.CCD_CD, S.CCD_NM_KOR, S.CCD_EPR_ORD, 'Y', '10');
GO

/* ── 3. 설비 에러 이력 - 설비 구분 ───────────────────────────────────── */

/*   이 현장에는 BCR 이 없다 */
UPDATE COMMON_CODE
   SET CCD_CD_YN = 'N'
 WHERE CDX_CD = 'EQP_TYP'
   AND CCD_CD = 'BCR'
GO

PRINT ''
PRINT '── 고친 뒤 (고르는 목록에 나오는 것만) ──'
SELECT CDX_CD, CCD_CD, CCD_NM_KOR, CCD_EPR_ORD
  FROM COMMON_CODE
 WHERE CDX_CD IN ('PGR_NM', 'CLIENT_PGR_NM', 'EQP_TYP')
   AND CCD_CD_YN = 'Y'
 ORDER BY CDX_CD, CCD_EPR_ORD
GO

PRINT ''
PRINT '확인할 것'
PRINT '  프로그램 로그   프로그램  : ALL / HOST / EQP / IO_TASK'
PRINT '  프로그램 로그   작업 구분 : 작업 정보 창과 같은 목록 (Client 교체 필요)'
PRINT '  HOST 로그       작업 구분 : 〃'
PRINT '  사용자 사용 로그 프로그램 : 지금 Client 의 대화상자'
PRINT '  설비 에러 이력  설비 구분 : CV / SC / RTV / HOST  (BCR 없음)'
GO

/* ── 되돌리려면 ─────────────────────────────────────────────────────────
   감춘 것을 다시 보이게 한다. 새로 넣은 행은 남지만 CCD_CD_YN 으로 가려진다.

UPDATE COMMON_CODE SET CCD_CD_YN = 'Y'
 WHERE CDX_CD IN ('PGR_NM', 'CLIENT_PGR_NM')
   AND CCD_CD IN ('WCS_TASK_CV_COMM0','WCS_TASK_SC_COMM0','WCS_TASK_SC_COMM1',
                  'WCS_TASK_RTV_COMM0','WCS_TASK_BCR_COMM0','WCS_TASK_BCR_COMM1',
                  'CBcrSkinDlg');
UPDATE COMMON_CODE SET CCD_CD_YN = 'Y' WHERE CDX_CD = 'EQP_TYP' AND CCD_CD = 'BCR';
   --------------------------------------------------------------------- */
