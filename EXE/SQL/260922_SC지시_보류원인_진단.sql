-- =====================================================================
-- [LGLS 2026-09-22] S/C 가 작업을 안 받을 때 - 어느 관문에 걸렸는지 본다
--
--   IO_TASK DriveSC() 의 지시 조건을 그대로 옮겨 관문마다 O / X 로 찍는다.
--   X 가 하나라도 있으면 그 작업은 지시되지 않는다.
--   크레인이 화면상 유휴여도 X 가 나오는 자리가 있으면 그것이 원인이다.
--
--   대기 상태 : 출고/그 밖 = 20,  입고(1/11) = 15 또는 16
--   출고 하역 트랙 : S/C#1=104  #2=106  #3=110  #4=114  #5=118
--   입고 픽업 트랙 : S/C#1=104  #2=108  #3=112  #4=116  #5=120
-- =====================================================================
USE LGLS_MCS_IO;
GO
SET NOCOUNT ON;

;WITH J AS (
    SELECT JM.*,
           CASE WHEN JM.JOB_TYP IN ('1','11') THEN JM.DEST_POS ELSE JM.START_POS END AS SCPOS,
           CASE WHEN JM.JOB_TYP IN ('1','11')
                THEN CASE JM.DEST_POS WHEN '901' THEN '104' WHEN '902' THEN '108'
                                      WHEN '903' THEN '112' WHEN '904' THEN '116'
                                      WHEN '905' THEN '120' ELSE JM.DEST_POS END
                ELSE CASE JM.START_POS WHEN '901' THEN '104' WHEN '902' THEN '106'
                                       WHEN '903' THEN '110' WHEN '904' THEN '114'
                                       WHEN '905' THEN '118' ELSE JM.START_POS END
           END AS WT
      FROM JOB_MST JM
     WHERE (JM.DEL_YN IS NULL OR JM.DEL_YN <> 'Y')
       AND ( (JM.JOB_TYP IN ('1','11') AND JM.JOB_STATUS IN ('15','16'))
          OR (JM.JOB_TYP NOT IN ('1','11') AND JM.JOB_STATUS = '20') )
)
SELECT J.LUGG_NO                                   AS 작업번호
     , J.JOB_TYP                                   AS 구분
     , J.JOB_STATUS                                AS 상태
     , J.START_POS + '>' + J.DEST_POS              AS 출발_도착
     , J.SCPOS                                     AS 담당크레인
     , J.WT                                        AS 라인트랙
     -- ── 크레인 쪽 관문 ──────────────────────────────────────
     , CASE WHEN SD.SC_NO IS NULL THEN 'X 크레인없음' ELSE 'O' END            AS G01_크레인매칭
     , CASE WHEN SD.ONLINE_MODE_RD = '1' THEN 'O' ELSE 'X 지상반수동' END     AS G02_지상반
     , CASE WHEN SD.AUTO_MODE_RD   = '1' THEN 'O' ELSE 'X 기상반수동' END     AS G03_기상반
     , CASE WHEN SD.ACTIVE_MODE_RD = '1' THEN 'O' ELSE 'X 비가동' END         AS G04_SRC
     , CASE WHEN SD.UCSTATUS_RD    = '1' THEN 'O' ELSE 'X 상태'+ISNULL(SD.UCSTATUS_RD,'?') END AS G05_동작상태
     , CASE WHEN ISNULL(SD.ERR_CODE_RD,'0') IN ('0','00','0000','') THEN 'O'
            ELSE 'X 에러'+SD.ERR_CODE_RD END                                   AS G06_에러
     , CASE WHEN ISNULL(SD.ITN_LUGG_FK1,'0') IN ('0','00','0000','')
             AND ISNULL(SD.ITN_LUGG_FK2,'0') IN ('0','00','0000','') THEN 'O'
            ELSE 'X 차상화물' END                                              AS G07_포크빔
     , CASE WHEN SD.OD_RQ_YN = 'N' THEN 'O' ELSE 'X 지시소비대기' END          AS G08_유휴
     , CASE WHEN (J.JOB_TYP IN ('1','11') AND ISNULL(SD.SUSPEND,'0') IN ('1','3'))
              OR (J.JOB_TYP IN ('2','12') AND ISNULL(SD.SUSPEND,'0') IN ('2','3'))
            THEN 'X 정지'+ISNULL(SD.SUSPEND,'0') ELSE 'O' END                AS G09_입출고정지
     , CASE WHEN EXISTS (SELECT 1 FROM JOB_MST J2
                          WHERE J2.WH_TYP = J.WH_TYP AND J2.JOB_STATUS = '25'
                            AND J.SCPOS = CASE WHEN J2.JOB_TYP IN ('1','11')
                                               THEN J2.DEST_POS ELSE J2.START_POS END)
            THEN 'X 25작업점유' ELSE 'O' END                                   AS G10_크레인배타
     -- ── 라인(C/V) 쪽 관문 ───────────────────────────────────
     , CASE WHEN CD.MC_NO IS NULL THEN 'X 트랙없음'
            WHEN ISNULL(CD.TR_PAUSE_RD,'0') NOT IN ('0','') THEN 'X 일시정지'
            ELSE 'O' END                                                       AS G11_트랙정지
     , CASE WHEN ISNULL(CD.ERROR_CODE,'0') IN ('0','00','0000','') THEN 'O'
            ELSE 'X 트랙에러'+CD.ERROR_CODE END                                AS G12_트랙에러
     , CASE WHEN J.JOB_TYP IN ('2','12') AND ISNULL(CD.LUGG_NO_RD,'0') NOT IN ('0','00','0000','')
            THEN 'X 트랙점유'+CD.LUGG_NO_RD ELSE 'O' END                       AS G13_하역트랙빔
     , CASE WHEN J.JOB_TYP IN ('2','12') AND ISNULL(CD.RETHS_READY_RD,'0') <> '1'
            THEN 'X 출고HS없음'
            WHEN J.JOB_TYP IN ('1','11') AND ISNULL(CD.STOHS_READY_RD,'0') <> '1'
            THEN 'X 입고HS없음' ELSE 'O' END                                   AS G14_핸드셰이크
     , CASE WHEN J.JOB_TYP IN ('1','11') AND ISNULL(LTRIM(RTRIM(CD.LUGG_NO_RD)),'') <> LTRIM(RTRIM(J.LUGG_NO))
            THEN 'X 픽업트랙에 이 화물 없음('+ISNULL(CD.LUGG_NO_RD,'없음')+')'
            ELSE 'O' END                                                       AS G15_입고화물확인
     , CASE WHEN J.JOB_TYP IN ('2','12') AND EXISTS (
                 SELECT 1 FROM CV_DATA C9
                  INNER JOIN JOB_MST J9 ON J9.WH_TYP = C9.WH_TYP AND J9.LUGG_NO = C9.LUGG_NO_RD
                  WHERE C9.WH_TYP = J.WH_TYP
                    AND C9.MC_NO  = CONVERT(varchar, CONVERT(int, J.WT) - 1)
                    AND J9.JOB_TYP IN ('1','11'))
            THEN 'X 홀수트랙에 입고화물' ELSE 'O' END                          AS G16_반대방향
     , CASE WHEN J.JOB_TYP IN ('2','12') AND (SELECT COUNT(*) FROM JOB_MST J7
                 WHERE J7.WH_TYP = J.WH_TYP AND J7.JOB_TYP IN ('2','12')
                   AND J7.START_POS = J.SCPOS AND J7.LUGG_NO <> J.LUGG_NO
                   AND J7.JOB_STATUS IN ('10','15')) >= 2
            THEN 'X 선행출고 2건' ELSE 'O' END                                 AS G17_출고2deep
  FROM J
  LEFT OUTER JOIN SC_DATA_LGLS SD ON SD.WH_TYP = J.WH_TYP AND SD.SC_NO = J.SCPOS
  LEFT OUTER JOIN CV_DATA      CD ON CD.WH_TYP = J.WH_TYP AND CD.MC_NO = J.WT
 ORDER BY J.LUGG_NO;
GO

-- 보는 법
--   G10 X : 그 크레인에 상태 25 작업이 남아 있다 → 상태창 [강제완료] 로 푼다
--   G14 X : 라인 트랙의 핸드셰이크 신호가 안 섰다 → 설비(PLC) 쪽을 본다
--   G13 X : 하역 트랙에 화물이 남아 있다 → 그 화물을 먼저 내보낸다
--   G12 X : 그 트랙이 에러다 (RGV 비상정지 82 전파 포함) → 에러부터 푼다
--   G02~G05 X : 크레인이 자동/온라인/가동 상태가 아니다

-- 참고 : 담당 크레인이 안 잡히면(G01 X) 출고 작업의 출발지가 9xx(크레인)가 아닌 것이다.
--        출고는 START_POS, 입고는 DEST_POS 가 크레인 번호여야 한다.
