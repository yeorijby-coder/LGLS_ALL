-- [LGLS 2026-08-30] JOB_STATUS 코드 재배치
--   · 완료      9 → 09
--   · 이중입고 에러   09 → 08
--   · 공출고 에러     08 → 07
--   · 이중입고 재지정 07 → 06
--   · 공출고 재지정   06 → 05
--   · 구동지시 11/21/31 폐기 (CCD_CD_YN='N' - 목록에서 빼되 구 경로 값 해석은 남긴다)
SET NOCOUNT ON;
BEGIN TRAN;

-- 1) 실데이터 먼저 (뒤에서 앞으로 - 충돌 방지)
UPDATE JOB_MST     SET JOB_STATUS='05' WHERE JOB_STATUS='06';
UPDATE JOB_MST     SET JOB_STATUS='06' WHERE JOB_STATUS='07';
UPDATE JOB_MST     SET JOB_STATUS='07' WHERE JOB_STATUS='08';
UPDATE JOB_MST     SET JOB_STATUS='08' WHERE JOB_STATUS='09';
UPDATE JOB_MST     SET JOB_STATUS='09' WHERE JOB_STATUS='9';
UPDATE JOB_MST     SET JOB_STATUS='15' WHERE JOB_STATUS='11';
UPDATE JOB_MST     SET JOB_STATUS='25' WHERE JOB_STATUS='21';
UPDATE JOB_MST     SET JOB_STATUS='35' WHERE JOB_STATUS='31';

-- 2) 코드표
DELETE FROM COMMON_CODE WHERE CDX_CD='JOB_STATUS' AND CCD_CD IN ('9','09','08','07','06','05');

INSERT INTO COMMON_CODE (WH_TYP, CDX_CD, CCD_CD, CCD_NM_KOR, CCD_NM_ENG, CCD_EPR_ORD, CCD_CD_YN)
VALUES ('10','JOB_STATUS','09','완료',            'Done',                 13,'Y'),
       ('10','JOB_STATUS','08','이중입고 에러',   'Dual Store Error',     14,'Y'),
       ('10','JOB_STATUS','07','공출고 에러',     'Empty Retrieve Error', 15,'Y'),
       ('10','JOB_STATUS','06','이중입고 재지정', 'Dual Store Retry',     16,'Y'),
       ('10','JOB_STATUS','05','공출고 재지정',   'Empty Retrieve Retry', 17,'Y');

-- 3) 구동지시 폐기 : 이름 해석은 남기고 목록에서만 제외
UPDATE COMMON_CODE SET CCD_CD_YN='N'
 WHERE CDX_CD='JOB_STATUS' AND CCD_CD IN ('11','21','31');

COMMIT;

SELECT CCD_CD, CCD_NM_KOR, CCD_EPR_ORD, CCD_CD_YN
  FROM COMMON_CODE WHERE CDX_CD='JOB_STATUS' ORDER BY CCD_EPR_ORD;
