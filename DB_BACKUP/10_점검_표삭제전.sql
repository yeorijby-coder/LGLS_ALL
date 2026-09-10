/* ============================================================================
   LGLS ECS - 이 현장에서 쓰지 않는 표 정리
   작성 2026-09-10
   대상 DB : LGLS_MCS_IO

   지우는 표 12개
     auto_sc_work    client_pgr_nm   dest_def        event_log
     wait_track      rtv_data        sc_data
     bcr_mst         wc_data         wc_his          host_empty_plt   mes_if_log

   판정 근거 (테이블_사용현황_조사표.docx)
     빌드되는 프로그램의 소스에 이 표들을 대상으로 한 SQL 이 하나도 없다.
     행 수는 근거로 쓰지 않았다(eqp_err_his 는 0행이지만 쓰는 표이고,
     dest_def 는 154행이 들어 있지만 읽는 프로그램이 없다).

   같이 정리한 프로그램 (2026-09-10 빌드부터)
     · ECS Client : WC / BCR / 공PLT / WC 로그 화면과 설비 클래스를 걷어냈다
                    (소스는 Ecs\_removed_20260910\ 에 보관)
     · WCS_TASK_HOST : 무게보고(GetWeightReport) / 빈파렛트 요청(GetEmptyPltRequest) 제거
                       - 두 기능은 이미 호출부가 주석 처리돼 있었다
     · 신규 구축 DDL 과 시뮬레이터 시드에서도 뺐다

   실행 순서
     10_점검_표삭제전.sql  ->  11_표삭제.sql  ->  12_점검_표삭제후.sql
     되돌리려면 13_표복구.sql (빈 표만 되살린다. 값은 되살릴 수 없다)

   주의
     프로그램을 모두 내린 뒤 실행하고, 2026-09-10 이후 빌드로 올린다.
   ============================================================================ */


USE LGLS_MCS_IO;
GO

SET NOCOUNT ON;

-- 지울 표의 행 수. 참고용이며, 행이 있어도 읽는 프로그램이 없으면 지운다.
SELECT 'auto_sc_work' AS tbl, COUNT(*) AS rows_ FROM dbo.[auto_sc_work]
UNION ALL SELECT 'bcr_mst' AS tbl, COUNT(*) AS rows_ FROM dbo.[bcr_mst]
UNION ALL SELECT 'client_pgr_nm' AS tbl, COUNT(*) AS rows_ FROM dbo.[client_pgr_nm]
UNION ALL SELECT 'dest_def' AS tbl, COUNT(*) AS rows_ FROM dbo.[dest_def]
UNION ALL SELECT 'event_log' AS tbl, COUNT(*) AS rows_ FROM dbo.[event_log]
UNION ALL SELECT 'host_empty_plt' AS tbl, COUNT(*) AS rows_ FROM dbo.[host_empty_plt]
UNION ALL SELECT 'mes_if_log' AS tbl, COUNT(*) AS rows_ FROM dbo.[mes_if_log]
UNION ALL SELECT 'rtv_data' AS tbl, COUNT(*) AS rows_ FROM dbo.[rtv_data]
UNION ALL SELECT 'sc_data' AS tbl, COUNT(*) AS rows_ FROM dbo.[sc_data]
UNION ALL SELECT 'wait_track' AS tbl, COUNT(*) AS rows_ FROM dbo.[wait_track]
UNION ALL SELECT 'wc_data' AS tbl, COUNT(*) AS rows_ FROM dbo.[wc_data]
UNION ALL SELECT 'wc_his' AS tbl, COUNT(*) AS rows_ FROM dbo.[wc_his];
GO

-- 참조 무결성 : 이 표들을 가리키는 외래키가 있으면 여기 나온다(있으면 먼저 처리).
SELECT fk.name AS fk, OBJECT_NAME(fk.parent_object_id) AS child, OBJECT_NAME(fk.referenced_object_id) AS parent
  FROM sys.foreign_keys fk
 WHERE OBJECT_NAME(fk.referenced_object_id) IN ('auto_sc_work', 'bcr_mst', 'client_pgr_nm', 'dest_def', 'event_log', 'host_empty_plt', 'mes_if_log', 'rtv_data', 'sc_data', 'wait_track', 'wc_data', 'wc_his')
    OR OBJECT_NAME(fk.parent_object_id) IN ('auto_sc_work', 'bcr_mst', 'client_pgr_nm', 'dest_def', 'event_log', 'host_empty_plt', 'mes_if_log', 'rtv_data', 'sc_data', 'wait_track', 'wc_data', 'wc_his');
GO