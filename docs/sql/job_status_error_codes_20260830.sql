-- =====================================================================
-- [LGLS 2026-08-30] 설비 에러(이중입고/공출고) 작업상태 코드 정비
--   JOB_MST.JOB_STATUS 표시명을 "이중입고 에러" / "공출고 에러" 로 명확히 한다.
--   코드값은 구 ECS 체계를 그대로 쓴다 (09=이중입고, 08=공출고, 07=이중입고재지정).
--   없으면 만들고, 있으면 이름만 갱신한다.
-- =====================================================================
SET NOCOUNT ON;

DECLARE @wh varchar(10) = '10';

MERGE common_code AS T
USING (VALUES
        ('09', '이중입고 에러',   'Dual Store Error',     13),
        ('08', '공출고 에러',     'Empty Retrieve Error', 14),
        ('07', '이중입고 재지정', 'Dual Store Retry',     15),
        ('06', '공출고 재지정',   'Empty Retrieve Retry', 16)
      ) AS S(ccd_cd, nm_kor, nm_eng, ord)
   ON  T.cdx_cd = 'JOB_STATUS'
   AND T.ccd_cd = S.ccd_cd
   AND T.wh_typ = @wh
WHEN MATCHED THEN
    UPDATE SET T.ccd_nm_kor = S.nm_kor,
               T.ccd_nm_eng = S.nm_eng,
               T.ccd_epr_ord = S.ord,
               T.ccd_cd_yn  = 'Y'
WHEN NOT MATCHED THEN
    INSERT (cdx_cd, ccd_cd, ccd_cd_yn, ccd_epr_ord, ccd_nm_kor, ccd_nm_eng, wh_typ)
    VALUES ('JOB_STATUS', S.ccd_cd, 'Y', S.ord, S.nm_kor, S.nm_eng, @wh);

SELECT ccd_cd, ccd_nm_kor, ccd_nm_eng, ccd_epr_ord
  FROM common_code
 WHERE cdx_cd = 'JOB_STATUS' AND ccd_cd IN ('06','07','08','09')
 ORDER BY ccd_cd;
