/* ============================================================================
   끝난 작업의 경과를 사후에 확인한다 (JOB_MST 에서 지워진 뒤에도 남는 자료만 쓴다)
   2026-10-29   현장 작업 2161 (입고) 계기

   쓰는 법
     1) 아래 DECLARE 세 줄을 고친다 : 작업번호, 조회 시작/끝(그 작업이 있던 날).
     2) 현장 DB 를 고르고(LGLS_IO) 한 번에 실행한다. 결과 표가 차례로 나온다.
        sqlcmd -S <서버> -d LGLS_IO -E -W -s "|" -i "<이 파일>"

   읽기 전용이다. 아무것도 바꾸지 않는다.
   남는 기간 : JOB_MST_HIS / WCS_LOG_PGR / WCS_CLIENT_LOG / HOST_IF_LOG 60일, EQP_ERR_HIS 30일
               (DEL_HIS_SETTING 의 값. 조회 0 번에서 현재 값을 보여 준다)
   ============================================================================ */
SET NOCOUNT ON
GO
DECLARE @L    varchar(10) = '2161'                 -- ★작업번호
DECLARE @FROM datetime    = '2026-10-27 00:00'     -- ★조회 시작
DECLARE @TO   datetime    = '2026-10-28 00:00'     -- ★조회 끝
DECLARE @F8   varchar(8)  = CONVERT(varchar(8), @FROM, 112)
DECLARE @T8   varchar(8)  = CONVERT(varchar(8), @TO,   112)

PRINT '== 0. 이력 보존 기간 / 상태 이력 트리거 =='
SELECT table_name, cycle, unit FROM DEL_HIS_SETTING
 WHERE table_name IN ('job_mst_his','wcs_log_pgr','wcs_client_log','host_if_log','eqp_err_his')
SELECT name AS 트리거 FROM sys.triggers WHERE name = 'trg_JOB_MST_StatusHis'
-- 위 트리거가 한 줄도 안 나오면 1 번 표는 비어 있다 (TASK\IO_TASK\trg_job_status_his.sql 로 만들 수 있으나, 지난 일은 채워지지 않는다)

PRINT ''
PRINT '== 1. 상태가 바뀐 순서 (JOB_MST_HIS) =='
PRINT '   35 가 있으면 IO_TASK 가 RGV 를 지시한 것 / 25 가 있으면 IO_TASK 가 크레인을 지시한 것 / hs_track_no = 통로 트랙'
SELECT ins_date AS 날짜, ins_time AS 시각, job_status AS 상태, hs_track_no AS HS트랙,
       job_typ AS 구분, start_pos AS 출발, dest_pos AS 도착, upd_user_id AS 변경자
  FROM JOB_MST_HIS
 WHERE lugg_no = @L AND ins_date BETWEEN @F8 AND @T8
 ORDER BY ins_date, ins_time

PRINT ''
PRINT '== 2. 트랙을 따라간 작업번호 (PLC 트래킹 변화) =='
PRINT '   [0000] -> [작업번호] 가 그 트랙에 화물이 들어온 시각, [작업번호] -> [0000] 이 나간(또는 지워진) 시각'
SELECT ins_dt AS 시각, track_from AS 트랙, LEFT(log_kor, 160) AS 내용
  FROM WCS_LOG_PGR
 WHERE ins_dt BETWEEN @FROM AND @TO
   AND log_kor LIKE '%R트래킹 JOB NO 변경%'
   AND log_kor LIKE '%[[]' + @L + ']%'
 ORDER BY ins_dt

PRINT ''
PRINT '== 3. 설비가 받은 반송 지시 (RGV / 크레인 / 컨베이어) =='
PRINT '   이 작업번호의 지시가 없으면 그 설비는 IO_TASK 지시 없이 움직인 것이다'
SELECT ins_dt AS 시각, pgr_nm AS 프로그램, LEFT(log_kor, 160) AS 내용
  FROM WCS_LOG_PGR
 WHERE ins_dt BETWEEN @FROM AND @TO
   AND log_kor LIKE '%' + @L + '%'
   AND ( (pgr_nm IN ('VEH_SC','VEH_RTV') AND log_kor LIKE '%반송지시%')
      OR log_kor LIKE '%CV 구동 지시%' )
 ORDER BY ins_dt

PRINT ''
PRINT '== 4. 운전자가 Client 에서 한 조작 (완료처리 / 쓰기 / 삭제 등) =='
SELECT ins_dt AS 시각, ip AS PC, user_id AS 사용자, win_id AS 화면, LEFT(message, 200) AS 내용
  FROM WCS_CLIENT_LOG
 WHERE ins_dt BETWEEN @FROM AND @TO
   AND (lugg_no = @L OR message LIKE '%' + @L + '%')
 ORDER BY ins_dt

PRINT ''
PRINT '== 5. IO_TASK 가 남긴 대기/보류 사유 (SCH_DBG) =='
SELECT ins_dt AS 시각, LEFT(log_kor, 200) AS 내용
  FROM WCS_LOG_PGR
 WHERE ins_dt BETWEEN @FROM AND @TO
   AND pgr_nm LIKE 'SCH%' AND log_kor LIKE '%' + @L + '%'
 ORDER BY ins_dt

PRINT ''
PRINT '== 6. 상위(HOST) 전문 =='
SELECT ins_dt AS 시각, host_cmd AS 명령, direction AS 방향, LEFT(message, 120) AS 내용
  FROM HOST_IF_LOG
 WHERE ins_dt BETWEEN @FROM AND @TO AND lugg_no = @L
 ORDER BY ins_dt

PRINT ''
PRINT '== 7. 설비 에러 (그 시간대, 작업번호가 붙은 것) =='
SELECT error_dt AS 시각, eqp_typ AS 구분, eqp_no AS 설비, eqp_err_cd AS 코드, remark AS 비고
  FROM EQP_ERR_HIS
 WHERE error_dt BETWEEN @FROM AND @TO AND lugg_no = @L
 ORDER BY error_dt
GO
