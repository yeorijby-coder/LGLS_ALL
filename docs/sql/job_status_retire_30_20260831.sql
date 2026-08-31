-- [LGLS 2026-08-31] RGV 구동대기(30) 폐기
--   신규 상태 체계에서 RGV 는 35(구동중)/39(완료)만 쓴다.
--     입고 : 99 -> 10 -> 15 -> 35 -> 39 -> 15 -> 25 -> 29 -> 09
--     출고 : 99 -> 20 -> 25 -> 29 -> 15 -> 35 -> 39 -> 15 -> 19 -> 09
--   구동지시 11/21/31 은 이미 폐기(2026-08-30).
--   ※삭제가 아니라 CCD_CD_YN='N' 이다. 콤보 목록에서는 빠지고,
--     혹시 남은 값이 그리드에 뜨더라도 이름 해석은 살아 있다.
SET NOCOUNT ON;
UPDATE COMMON_CODE SET CCD_CD_YN='N'
 WHERE CDX_CD='JOB_STATUS' AND CCD_CD='30';

SELECT CCD_CD, CCD_NM_KOR, CCD_EPR_ORD, CCD_CD_YN
  FROM COMMON_CODE WHERE CDX_CD='JOB_STATUS' ORDER BY CCD_EPR_ORD;
