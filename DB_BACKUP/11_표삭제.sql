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

SET XACT_ABORT ON;

IF OBJECT_ID('dbo.auto_sc_work','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[auto_sc_work];
    PRINT '  삭제 auto_sc_work';
END
ELSE
    PRINT '  (이미 없음) auto_sc_work';
GO
IF OBJECT_ID('dbo.bcr_mst','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[bcr_mst];
    PRINT '  삭제 bcr_mst';
END
ELSE
    PRINT '  (이미 없음) bcr_mst';
GO
IF OBJECT_ID('dbo.client_pgr_nm','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[client_pgr_nm];
    PRINT '  삭제 client_pgr_nm';
END
ELSE
    PRINT '  (이미 없음) client_pgr_nm';
GO
IF OBJECT_ID('dbo.dest_def','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[dest_def];
    PRINT '  삭제 dest_def';
END
ELSE
    PRINT '  (이미 없음) dest_def';
GO
IF OBJECT_ID('dbo.event_log','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[event_log];
    PRINT '  삭제 event_log';
END
ELSE
    PRINT '  (이미 없음) event_log';
GO
IF OBJECT_ID('dbo.host_empty_plt','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[host_empty_plt];
    PRINT '  삭제 host_empty_plt';
END
ELSE
    PRINT '  (이미 없음) host_empty_plt';
GO
IF OBJECT_ID('dbo.mes_if_log','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[mes_if_log];
    PRINT '  삭제 mes_if_log';
END
ELSE
    PRINT '  (이미 없음) mes_if_log';
GO
IF OBJECT_ID('dbo.rtv_data','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[rtv_data];
    PRINT '  삭제 rtv_data';
END
ELSE
    PRINT '  (이미 없음) rtv_data';
GO
IF OBJECT_ID('dbo.sc_data','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[sc_data];
    PRINT '  삭제 sc_data';
END
ELSE
    PRINT '  (이미 없음) sc_data';
GO
IF OBJECT_ID('dbo.wait_track','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[wait_track];
    PRINT '  삭제 wait_track';
END
ELSE
    PRINT '  (이미 없음) wait_track';
GO
IF OBJECT_ID('dbo.wc_data','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[wc_data];
    PRINT '  삭제 wc_data';
END
ELSE
    PRINT '  (이미 없음) wc_data';
GO
IF OBJECT_ID('dbo.wc_his','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[wc_his];
    PRINT '  삭제 wc_his';
END
ELSE
    PRINT '  (이미 없음) wc_his';
GO
PRINT '표 정리 완료';
GO