/* ============================================================================
   끝난 작업의 경과를 사후에 확인한다 (JOB_MST 에서 지워진 뒤에도 남는 자료만 쓴다)
   2026-10-29   현장 작업 2161 (입고) 계기

   쓰는 법
     1) 아래 [★ 고칠 곳] INSERT 한 줄의 작업번호, 조회 시작/끝(그 작업이 있던 날)을 고친다.
     2) 현장 DB(LGLS_IO)를 고르고 ★스크립트 전체★ 를 실행한다.
          DBeaver : Alt+X (Execute script).  Ctrl+Enter 는 커서가 있는 문장 하나만 돌리므로 쓰지 않는다.
          SSMS    : F5 (전체 선택 안 한 상태)
          sqlcmd  : sqlcmd -S <서버> -d LGLS_IO -E -W -s "|" -i "<이 파일>"
     3) 결과 표가 차례로 나온다. 각 표의 맨 왼쪽 칸 [표] 가 번호이다 (0a, 0b, 1 ... 7).

   읽기 전용이다 (임시 표 #P 에 조회 조건만 담는다. 접속을 닫으면 사라진다).
   빈 줄에서 문장을 나눠 실행하는 도구에서도 되도록, 변수 대신 임시 표를 쓴다.

   남는 기간 : JOB_MST_HIS / WCS_LOG_PGR / WCS_CLIENT_LOG / HOST_IF_LOG 60일, EQP_ERR_HIS 30일
               (DEL_HIS_SETTING 의 값. 표 0a 가 현재 값을 보여 준다)
   ============================================================================ */

SET NOCOUNT ON;

IF OBJECT_ID('tempdb..#P') IS NOT NULL DROP TABLE #P;

CREATE TABLE #P (L varchar(10), F datetime, T datetime, F8 varchar(8), T8 varchar(8));

INSERT INTO #P (L, F, T) VALUES ('2161', '2026-10-27 00:00', '2026-10-28 00:00');   -- ★ 고칠 곳 : 작업번호, 조회 시작, 조회 끝

UPDATE #P SET F8 = CONVERT(varchar(8), F, 112), T8 = CONVERT(varchar(8), T, 112);

/* 0a. 이력 보존 기간 */
SELECT '0a 보존기간' AS 표, table_name, cycle, unit FROM DEL_HIS_SETTING
 WHERE table_name IN ('job_mst_his','wcs_log_pgr','wcs_client_log','host_if_log','eqp_err_his');

/* 0b. 상태 이력 트리거 - 한 줄도 안 나오면 표 1 은 비어 있다 (TASK\IO_TASK\trg_job_status_his.sql 로 만들 수 있으나 지난 일은 채워지지 않는다) */
SELECT '0b 트리거' AS 표, name AS 트리거 FROM sys.triggers WHERE name = 'trg_JOB_MST_StatusHis';

/* 1. 상태가 바뀐 순서 - 35 가 있으면 IO_TASK 가 RGV 를 지시, 25 가 있으면 크레인을 지시한 것. hs_track_no = 통로 트랙 */
SELECT '1 상태순서' AS 표, h.ins_date AS 날짜, h.ins_time AS 시각, h.job_status AS 상태, h.hs_track_no AS HS트랙,
       h.job_typ AS 구분, h.start_pos AS 출발, h.dest_pos AS 도착, h.upd_user_id AS 변경자
  FROM JOB_MST_HIS h CROSS JOIN #P p
 WHERE h.lugg_no = p.L AND h.ins_date BETWEEN p.F8 AND p.T8
 ORDER BY h.ins_date, h.ins_time;

/* 2. 트랙을 따라간 작업번호 - [0000] -> [번호] 는 그 트랙에 들어온 시각, [번호] -> [0000] 은 나가거나 지워진 시각 */
SELECT '2 트래킹' AS 표, g.ins_dt AS 시각, g.track_from AS 트랙, LEFT(g.log_kor, 160) AS 내용
  FROM WCS_LOG_PGR g CROSS JOIN #P p
 WHERE g.ins_dt BETWEEN p.F AND p.T
   AND g.log_kor LIKE '%R트래킹 JOB NO 변경%'
   AND g.log_kor LIKE '%[[]' + p.L + ']%'
 ORDER BY g.ins_dt;

/* 3. 설비가 받은 반송 지시 - 이 작업번호의 지시가 없으면 그 설비는 IO_TASK 지시 없이 움직인 것 */
SELECT '3 반송지시' AS 표, g.ins_dt AS 시각, g.pgr_nm AS 프로그램, LEFT(g.log_kor, 160) AS 내용
  FROM WCS_LOG_PGR g CROSS JOIN #P p
 WHERE g.ins_dt BETWEEN p.F AND p.T
   AND g.log_kor LIKE '%' + p.L + '%'
   AND ( (g.pgr_nm IN ('VEH_SC','VEH_RTV') AND g.log_kor LIKE '%반송지시%')
      OR g.log_kor LIKE '%CV 구동 지시%' )
 ORDER BY g.ins_dt;

/* 4. 운전자가 Client 에서 한 조작 (완료처리 / 쓰기 / 삭제 등) */
SELECT '4 Client조작' AS 표, c.ins_dt AS 시각, c.ip AS PC, c.user_id AS 사용자, c.win_id AS 화면, LEFT(c.message, 200) AS 내용
  FROM WCS_CLIENT_LOG c CROSS JOIN #P p
 WHERE c.ins_dt BETWEEN p.F AND p.T
   AND (c.lugg_no = p.L OR c.message LIKE '%' + p.L + '%')
 ORDER BY c.ins_dt;

/* 5. IO_TASK 가 남긴 대기/보류 사유 (SCH_DBG) */
SELECT '5 보류사유' AS 표, g.ins_dt AS 시각, LEFT(g.log_kor, 200) AS 내용
  FROM WCS_LOG_PGR g CROSS JOIN #P p
 WHERE g.ins_dt BETWEEN p.F AND p.T
   AND g.pgr_nm LIKE 'SCH%' AND g.log_kor LIKE '%' + p.L + '%'
 ORDER BY g.ins_dt;

/* 6. 상위(HOST) 전문 */
SELECT '6 HOST' AS 표, h.ins_dt AS 시각, h.host_cmd AS 명령, h.direction AS 방향, LEFT(h.message, 120) AS 내용
  FROM HOST_IF_LOG h CROSS JOIN #P p
 WHERE h.ins_dt BETWEEN p.F AND p.T AND h.lugg_no = p.L
 ORDER BY h.ins_dt;

/* 7. 설비 에러 (그 시간대, 작업번호가 붙은 것) */
SELECT '7 에러' AS 표, e.error_dt AS 시각, e.eqp_typ AS 구분, e.eqp_no AS 설비, e.eqp_err_cd AS 코드, e.remark AS 비고
  FROM EQP_ERR_HIS e CROSS JOIN #P p
 WHERE e.error_dt BETWEEN p.F AND p.T AND e.lugg_no = p.L
 ORDER BY e.error_dt;

DROP TABLE #P;
