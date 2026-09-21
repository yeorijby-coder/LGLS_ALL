/* ============================================================================
   1호기(S/C#1) 출고 소요 시간 · 지연 원인 분석 쿼리        2026-09-17
   대상 DB : LGLS_MCS_IO (SQL Server 2008 호환 - LAG/STRING_AGG 미사용)

   사용법
     1) 맨 위 [기간] 두 줄만 고친다. 기본 = 어제 0시 ~ 오늘 0시
     2) 필요한 블록만 드래그해서 실행한다 (블록마다 [기간] DECLARE 가 있고 GO 로 나뉘어 있어 전체 실행도 된다)
     3) Q4 는 @LUGG 에 조사할 작업번호를 넣는다

   1호기 출고 흐름과 상태(JOB_STATUS)
     20 SC 구동대기 → 25 SC 구동중(크레인이 랙에서 꺼내 H/S 로) → 29 SC 구동완료
     → 16 통로CV 구동중(104→103) → 35 RGV 구동중(103 에서 싣고) → 39 RGV 구동완료
     → 19/09 완료(작업대 도착·상위 보고)
   통로 C/V#2 : 트랙 104 = 크레인 쪽 출고 H/S, 트랙 103 = RGV 쪽. 방향 워드 D301
   ★ 29 → 35 (크레인이 내려놓은 뒤 RGV 가 싣기까지) 가 "출고 H/S 체류" 이다.
============================================================================ */


GO

/* ── Q1. 1호기 출고 작업별 단계 시각 · 구간 소요(초) ───────────────────────── */
DECLARE @D0 datetime; SET @D0 = DATEADD(day, -1, CONVERT(date, GETDATE()));   -- [기간] 시작
DECLARE @D1 datetime; SET @D1 = CONVERT(date, GETDATE());                     -- [기간] 끝

;WITH H AS (
    SELECT LUGG_NO, JOB_STATUS, UPD_DT, DEST_POS, START_LOCATION
      FROM JOB_MST_HIS
     WHERE JOB_TYP = '2' AND START_POS = '901'
       AND UPD_DT >= DATEADD(hour, -1, @D0) AND UPD_DT < DATEADD(hour, 2, @D1)
), J AS (
    SELECT LUGG_NO, MAX(DEST_POS) AS DEST_POS, MAX(START_LOCATION) AS FROM_LOC,
           MIN(CASE WHEN JOB_STATUS = '20' THEN UPD_DT END) AS T20,
           MIN(CASE WHEN JOB_STATUS = '25' THEN UPD_DT END) AS T25,
           MIN(CASE WHEN JOB_STATUS = '29' THEN UPD_DT END) AS T29,
           MIN(CASE WHEN JOB_STATUS = '16' THEN UPD_DT END) AS T16,
           MIN(CASE WHEN JOB_STATUS = '35' THEN UPD_DT END) AS T35,
           MIN(CASE WHEN JOB_STATUS = '39' THEN UPD_DT END) AS T39,
           MIN(CASE WHEN JOB_STATUS IN ('19','09') THEN UPD_DT END) AS T_END
      FROM H
     GROUP BY LUGG_NO
)
SELECT LUGG_NO, FROM_LOC, DEST_POS, T20, T25, T29, T16, T35, T39, T_END,
       DATEDIFF(second, T20, T25)    AS [대기_20→25],
       DATEDIFF(second, T25, T29)    AS [크레인_25→29],
       DATEDIFF(second, T29, T35)    AS [HS체류_29→35],
       DATEDIFF(second, T35, T39)    AS [RGV_35→39],
       DATEDIFF(second, T39, T_END)  AS [작업대_39→완료],
       DATEDIFF(second, ISNULL(T20, T25), T_END) AS [전체]
  FROM J
 WHERE T25 >= @D0 AND T25 < @D1
 ORDER BY T25;


GO

/* ── Q2. 구간별 요약 (평균 · 최대 · 10초/15초 초과 건수) ─────────────────────── */
DECLARE @D0 datetime; SET @D0 = DATEADD(day, -1, CONVERT(date, GETDATE()));
DECLARE @D1 datetime; SET @D1 = CONVERT(date, GETDATE());

;WITH J AS (
    SELECT LUGG_NO,
           MIN(CASE WHEN JOB_STATUS = '20' THEN UPD_DT END) AS T20,
           MIN(CASE WHEN JOB_STATUS = '25' THEN UPD_DT END) AS T25,
           MIN(CASE WHEN JOB_STATUS = '29' THEN UPD_DT END) AS T29,
           MIN(CASE WHEN JOB_STATUS = '35' THEN UPD_DT END) AS T35,
           MIN(CASE WHEN JOB_STATUS = '39' THEN UPD_DT END) AS T39,
           MIN(CASE WHEN JOB_STATUS IN ('19','09') THEN UPD_DT END) AS T_END
      FROM JOB_MST_HIS
     WHERE JOB_TYP = '2' AND START_POS = '901'
       AND UPD_DT >= DATEADD(hour, -1, @D0) AND UPD_DT < DATEADD(hour, 2, @D1)
     GROUP BY LUGG_NO
)
SELECT COUNT(*)                                        AS [건수],
       AVG(DATEDIFF(second, T25, T29))                 AS [크레인_평균],
       MAX(DATEDIFF(second, T25, T29))                 AS [크레인_최대],
       AVG(DATEDIFF(second, T29, T35))                 AS [HS체류_평균],
       MAX(DATEDIFF(second, T29, T35))                 AS [HS체류_최대],
       SUM(CASE WHEN DATEDIFF(second, T29, T35) > 10 THEN 1 ELSE 0 END) AS [HS체류_10초초과],
       SUM(CASE WHEN DATEDIFF(second, T29, T35) > 15 THEN 1 ELSE 0 END) AS [HS체류_15초초과],
       AVG(DATEDIFF(second, T35, T39))                 AS [RGV_평균],
       MAX(DATEDIFF(second, T35, T39))                 AS [RGV_최대],
       AVG(DATEDIFF(second, ISNULL(T20, T25), T_END))  AS [전체_평균],
       MAX(DATEDIFF(second, ISNULL(T20, T25), T_END))  AS [전체_최대]
  FROM J
 WHERE T25 >= @D0 AND T25 < @D1;


GO

/* ── Q3. H/S 체류가 긴 작업의 원인 후보 한눈에 ─────────────────────────────────
   방향전환_횟수 : 크레인 완료 전후(29-10초 ~ 35) 통로 103 에 나간 방향 지시 수. 2 이상 = 입고↔출고 왕복
   방향_29시점   : 크레인 완료 직전 마지막 방향 지시 (입고면 출고 H/S 에서 벨트가 안 움직인다)
   RGV_다른작업  : 그 사이 RGV 가 다른 작업(35~39)을 하고 있던 건수
   1호기_입고대기: 그 사이 1호기로 가는 입고 작업이 진행 중이던 건수
   HS기록_지연   : 크레인 완료(29) 뒤 트랙 104 에 작업번호가 찍히기까지 초
   104→103_이동  : 104 기록 뒤 103 에 찍히기까지 초 (벨트가 옮긴 시간)            */
DECLARE @D0 datetime; SET @D0 = DATEADD(day, -1, CONVERT(date, GETDATE()));
DECLARE @D1 datetime; SET @D1 = CONVERT(date, GETDATE());
DECLARE @SLOW int;    SET @SLOW = 10;   -- 이 초 이상 H/S 체류만 본다

;WITH J AS (
    SELECT LUGG_NO,
           MIN(CASE WHEN JOB_STATUS = '29' THEN UPD_DT END) AS T29,
           MIN(CASE WHEN JOB_STATUS = '35' THEN UPD_DT END) AS T35
      FROM JOB_MST_HIS
     WHERE JOB_TYP = '2' AND START_POS = '901'
       AND UPD_DT >= DATEADD(hour, -1, @D0) AND UPD_DT < DATEADD(hour, 2, @D1)
     GROUP BY LUGG_NO
)
SELECT J.LUGG_NO, J.T29, J.T35,
       DATEDIFF(second, J.T29, J.T35) AS [HS체류],
       DIR.CNT                       AS [방향전환_횟수],
       CASE WHEN LASTDIR.LOG_KOR LIKE N'%출고(%' THEN N'출고'
            WHEN LASTDIR.LOG_KOR LIKE N'%입고(%' THEN N'입고' ELSE N'?' END AS [방향_29시점],
       LASTDIR.INS_DT                AS [방향_29시점_지시시각],
       RGV.CNT                       AS [RGV_다른작업],
       INB.CNT                       AS [1호기_입고대기],
       DATEDIFF(ms, J.T29, T104.INS_DT) / 1000.0 AS [HS기록_지연],
       DATEDIFF(ms, T104.INS_DT, T103.INS_DT) / 1000.0 AS [104→103_이동]
  FROM J
 OUTER APPLY (SELECT COUNT(*) AS CNT FROM WCS_LOG_PGR L
               WHERE L.INS_DT BETWEEN DATEADD(second, -10, J.T29) AND J.T35
                 AND L.LOG_KOR LIKE '%CvChg_CMD_RQ_YN%[[]103]%') DIR
 OUTER APPLY (SELECT TOP 1 L.INS_DT, L.LOG_KOR FROM WCS_LOG_PGR L
               WHERE L.INS_DT <= J.T29 AND L.INS_DT >= DATEADD(hour, -2, J.T29)
                 AND L.LOG_KOR LIKE '%CvChg_CMD_RQ_YN%[[]103]%'
               ORDER BY L.INS_DT DESC) LASTDIR
 OUTER APPLY (SELECT COUNT(DISTINCT H.LUGG_NO) AS CNT FROM JOB_MST_HIS H
               WHERE H.LUGG_NO <> J.LUGG_NO AND H.JOB_STATUS IN ('35','39')
                 AND H.UPD_DT BETWEEN J.T29 AND J.T35) RGV
 OUTER APPLY (SELECT COUNT(DISTINCT H.LUGG_NO) AS CNT FROM JOB_MST_HIS H
               WHERE H.JOB_TYP = '1' AND H.DEST_POS = '901'
                 AND H.UPD_DT BETWEEN DATEADD(minute, -3, J.T29) AND J.T35) INB
 OUTER APPLY (SELECT TOP 1 L.INS_DT FROM WCS_LOG_PGR L
               WHERE L.INS_DT BETWEEN DATEADD(minute, -1, J.T29) AND DATEADD(minute, 2, J.T35)
                 AND L.LOG_KOR LIKE N'%트랙 104 R트래킹%-> [[]' + J.LUGG_NO + ']%'
               ORDER BY L.INS_DT) T104
 OUTER APPLY (SELECT TOP 1 L.INS_DT FROM WCS_LOG_PGR L
               WHERE L.INS_DT BETWEEN DATEADD(minute, -1, J.T29) AND DATEADD(minute, 2, J.T35)
                 AND L.LOG_KOR LIKE N'%트랙 103 R트래킹%-> [[]' + J.LUGG_NO + ']%'
               ORDER BY L.INS_DT) T103
 WHERE J.T29 >= @D0 AND J.T29 < @D1
   AND DATEDIFF(second, J.T29, J.T35) >= @SLOW
 ORDER BY DATEDIFF(second, J.T29, J.T35) DESC;


GO

/* ── Q4. 작업 1건의 전체 타임라인 (작업 상태 + 관련 로그를 시간순으로) ─────────
   @LUGG 에 Q1/Q3 에서 찾은 작업번호를 넣는다.
   통로 103/104 트래킹, 103 방향 지시, 1호기 크레인(VEHICLE@11), RGV, 스케줄러 보류 로그를 함께 본다. */
DECLARE @D0 datetime; SET @D0 = DATEADD(day, -1, CONVERT(date, GETDATE()));
DECLARE @D1 datetime; SET @D1 = CONVERT(date, GETDATE());
DECLARE @LUGG varchar(10); SET @LUGG = '1234';   -- ★ 작업번호

DECLARE @TS datetime, @TE datetime;
SELECT @TS = DATEADD(second, -20, MIN(UPD_DT)), @TE = DATEADD(second, 20, MAX(UPD_DT))
  FROM JOB_MST_HIS
 WHERE LUGG_NO = @LUGG AND UPD_DT >= DATEADD(hour, -1, @D0) AND UPD_DT < DATEADD(hour, 2, @D1);

SELECT T, SRC, MSG FROM (
    SELECT UPD_DT AS T, 'JOB' AS SRC,
           N'상태 ' + JOB_STATUS + N'  (' + ISNULL(START_POS, '') + N' → ' + ISNULL(DEST_POS, '') + N', H/S ' + ISNULL(HS_TRACK_NO, '') + N')' AS MSG
      FROM JOB_MST_HIS
     WHERE LUGG_NO = @LUGG AND UPD_DT BETWEEN @TS AND @TE
    UNION ALL
    SELECT INS_DT, PGR_NM, LOG_KOR
      FROM WCS_LOG_PGR
     WHERE INS_DT BETWEEN @TS AND @TE
       AND (   LUGG_NO = @LUGG
            OR LOG_KOR LIKE '%' + @LUGG + '%'
            OR LOG_KOR LIKE N'%트랙 10[34] %'
            OR LOG_KOR LIKE '%CvChg_CMD_RQ_YN%[[]103]%'
            OR LOG_KOR LIKE '%VEHICLE@11 %'
            OR PGR_NM = 'VEH_RTV'
            OR (PGR_NM LIKE 'SCH%' AND (LOG_KOR LIKE '%103%' OR LOG_KOR LIKE '%104%' OR LOG_KOR LIKE N'%1호기%')))
) X
ORDER BY T;


GO

/* ── Q5. 통로 C/V#2(트랙 103) 방향 지시 이력과 직전 방향 유지 시간 ──────────────
   짧게(수 초) 입고↔출고가 왔다 갔다 한 구간이 체류 원인 후보다. */
DECLARE @D0 datetime; SET @D0 = DATEADD(day, -1, CONVERT(date, GETDATE()));
DECLARE @D1 datetime; SET @D1 = CONVERT(date, GETDATE());

;WITH D AS (
    SELECT ROW_NUMBER() OVER (ORDER BY INS_DT) AS RN, INS_DT,
           CASE WHEN LOG_KOR LIKE N'%출고(%' THEN N'출고' WHEN LOG_KOR LIKE N'%입고(%' THEN N'입고' ELSE N'?' END AS DIR,
           LOG_KOR
      FROM WCS_LOG_PGR
     WHERE INS_DT >= @D0 AND INS_DT < @D1
       AND LOG_KOR LIKE '%CvChg_CMD_RQ_YN%[[]103]%'
)
SELECT C.INS_DT, ISNULL(P.DIR, N'-') AS [이전], C.DIR AS [전환],
       DATEDIFF(second, P.INS_DT, C.INS_DT) AS [이전방향_유지초],
       C.LOG_KOR
  FROM D C
  LEFT JOIN D P ON P.RN = C.RN - 1
 ORDER BY C.INS_DT;

GO
/* Q5-1. 시간대별 방향 지시 횟수 (몰린 시간대 찾기) */
DECLARE @D0 datetime; SET @D0 = DATEADD(day, -1, CONVERT(date, GETDATE()));
DECLARE @D1 datetime; SET @D1 = CONVERT(date, GETDATE());
SELECT DATEPART(hour, INS_DT) AS [시],
       SUM(CASE WHEN LOG_KOR LIKE N'%출고(%' THEN 1 ELSE 0 END) AS [출고로],
       SUM(CASE WHEN LOG_KOR LIKE N'%입고(%' THEN 1 ELSE 0 END) AS [입고로],
       COUNT(*) AS [합계]
  FROM WCS_LOG_PGR
 WHERE INS_DT >= @D0 AND INS_DT < @D1
   AND LOG_KOR LIKE '%CvChg_CMD_RQ_YN%[[]103]%'
 GROUP BY DATEPART(hour, INS_DT)
 ORDER BY 1;


GO

/* ── Q6. 체류 중 RGV 가 하고 있던 작업 (RGV 가 바빠서 못 온 경우) ─────────────── */
DECLARE @D0 datetime; SET @D0 = DATEADD(day, -1, CONVERT(date, GETDATE()));
DECLARE @D1 datetime; SET @D1 = CONVERT(date, GETDATE());
DECLARE @SLOW int;    SET @SLOW = 10;

;WITH J AS (
    SELECT LUGG_NO,
           MIN(CASE WHEN JOB_STATUS = '29' THEN UPD_DT END) AS T29,
           MIN(CASE WHEN JOB_STATUS = '35' THEN UPD_DT END) AS T35
      FROM JOB_MST_HIS
     WHERE JOB_TYP = '2' AND START_POS = '901'
       AND UPD_DT >= DATEADD(hour, -1, @D0) AND UPD_DT < DATEADD(hour, 2, @D1)
     GROUP BY LUGG_NO
), R AS (
    SELECT LUGG_NO, MAX(JOB_TYP) AS JOB_TYP, MAX(START_POS) AS START_POS, MAX(DEST_POS) AS DEST_POS,
           MIN(CASE WHEN JOB_STATUS = '35' THEN UPD_DT END) AS R35,
           MIN(CASE WHEN JOB_STATUS = '39' THEN UPD_DT END) AS R39
      FROM JOB_MST_HIS
     WHERE UPD_DT >= DATEADD(hour, -1, @D0) AND UPD_DT < DATEADD(hour, 2, @D1)
     GROUP BY LUGG_NO
)
SELECT J.LUGG_NO AS [1호기출고], J.T29, J.T35, DATEDIFF(second, J.T29, J.T35) AS [HS체류],
       R.LUGG_NO AS [RGV작업], R.JOB_TYP, R.START_POS, R.DEST_POS, R.R35, R.R39
  FROM J
  JOIN R ON R.LUGG_NO <> J.LUGG_NO
        AND R.R35 < J.T35 AND ISNULL(R.R39, J.T35) > J.T29
 WHERE J.T29 >= @D0 AND J.T29 < @D1
   AND DATEDIFF(second, J.T29, J.T35) >= @SLOW
 ORDER BY J.T29, R.R35;


GO

/* ── Q7. 체류 중 진행되던 1호기행 입고 (입고가 통로 방향을 잡은 경우) ───────────
   입고 흐름 : 15 CV구동 → 35 RGV구동(→103 에 내림) → 39 → 16/15 통로(103→104) → 25 크레인 → 29 */
DECLARE @D0 datetime; SET @D0 = DATEADD(day, -1, CONVERT(date, GETDATE()));
DECLARE @D1 datetime; SET @D1 = CONVERT(date, GETDATE());
DECLARE @SLOW int;    SET @SLOW = 10;

;WITH J AS (
    SELECT LUGG_NO,
           MIN(CASE WHEN JOB_STATUS = '29' THEN UPD_DT END) AS T29,
           MIN(CASE WHEN JOB_STATUS = '35' THEN UPD_DT END) AS T35
      FROM JOB_MST_HIS
     WHERE JOB_TYP = '2' AND START_POS = '901'
       AND UPD_DT >= DATEADD(hour, -1, @D0) AND UPD_DT < DATEADD(hour, 2, @D1)
     GROUP BY LUGG_NO
)
SELECT J.LUGG_NO AS [1호기출고], J.T29, J.T35, DATEDIFF(second, J.T29, J.T35) AS [HS체류],
       H.LUGG_NO AS [1호기입고], H.START_POS, H.JOB_STATUS, H.UPD_DT
  FROM J
  JOIN JOB_MST_HIS H
    ON H.JOB_TYP = '1' AND H.DEST_POS = '901'
   AND H.UPD_DT BETWEEN DATEADD(minute, -3, J.T29) AND DATEADD(second, 30, J.T35)
 WHERE J.T29 >= @D0 AND J.T29 < @D1
   AND DATEDIFF(second, J.T29, J.T35) >= @SLOW
 ORDER BY J.T29, H.LUGG_NO, H.UPD_DT;


GO

/* ── Q8. 스케줄러가 남긴 1호기 출고 보류 사유 집계 ─────────────────────────────
   (프로그램 버전에 따라 문구가 다를 수 있다. 0건이면 Q4 로 1건씩 본다) */
DECLARE @D0 datetime; SET @D0 = DATEADD(day, -1, CONVERT(date, GETDATE()));
DECLARE @D1 datetime; SET @D1 = CONVERT(date, GETDATE());

;WITH K AS (
    SELECT DISTINCT LUGG_NO
      FROM JOB_MST_HIS
     WHERE JOB_TYP = '2' AND START_POS = '901'
       AND UPD_DT >= @D0 AND UPD_DT < @D1
)
SELECT REPLACE(LEFT(L.LOG_KOR, 60), K.LUGG_NO, '####') AS [사유(작업번호=####)], COUNT(*) AS [횟수], COUNT(DISTINCT K.LUGG_NO) AS [작업수]
  FROM WCS_LOG_PGR L
  JOIN K ON L.LOG_KOR LIKE '%' + K.LUGG_NO + '%'
 WHERE L.INS_DT >= @D0 AND L.INS_DT < @D1
   AND L.PGR_NM LIKE 'SCH%'
 GROUP BY REPLACE(LEFT(L.LOG_KOR, 60), K.LUGG_NO, '####')
 ORDER BY 2 DESC;
GO
