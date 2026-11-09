/* ============================================================================
   LGLS ECS - 셀 관리 표 정리 : CELL_MST / CELL_DTL
   대상 DB : LGLS_MCS_IO

   왜 지우나
     이 현장은 셀(랙 위치) 관리를 쓰지 않는다. 상위(WMS)가 로케이션을 정해 내려 주고
     ECS 는 그대로 지시한다. 프로그램도 CELL_MST 없이 돌도록 이미 바뀌어 있다.
       · 스케줄러(IO_TASK) · 설비 통신(WCS_TASK_CV) : 처음부터 이 표를 읽지 않는다
       · 운전 화면(Ecs) : 수동 작업 지시는 Ecs.ini [CNF] USE_CELL_MST=0(기본)이면
         SC_DEF_INF.AREA 로 담당 크레인을 구한다. 화재 화면·랙 사용현황(USAGE)은
         호출하는 메뉴가 없다(리본에서 뺀 지 오래)
       · 상위 통신(WCS_TASK_HOST) : 입고(1)/출고(2) 수신은 이 표를 보지 않는다.
         랙투랙(4)/호기간 이동(5)만 보는데 이 현장에 그 작업은 없다
     CELL_DTL 은 0행이고, CELL_MST 500행은 구 ECS 가 남긴 셀 목록일 뿐이다.
     DB 안에서 이 두 표를 참조하는 뷰 · 프로시저 · 트리거 · 외래키는 없다.

   실행 순서
     프로그램을 모두 내린 뒤 이 파일을 실행한다.
     되돌리려면 21_CELL표_복구.sql (표를 다시 만들고 bcp 로 값까지 되살린다)

   값 백업
     cell_20261109\cell_mst.bcp (500행, 네이티브 형식)   cell_20261109\cell_dtl.bcp (0행)
   ============================================================================ */
USE LGLS_MCS_IO;
GO
IF OBJECT_ID('dbo.cell_dtl','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[cell_dtl];
    PRINT '  삭제 cell_dtl';
END
ELSE
    PRINT '  (이미 없음) cell_dtl';
GO
IF OBJECT_ID('dbo.cell_mst','U') IS NOT NULL
BEGIN
    DROP TABLE dbo.[cell_mst];
    PRINT '  삭제 cell_mst';
END
ELSE
    PRINT '  (이미 없음) cell_mst';
GO
PRINT 'CELL 표 정리 완료';
GO
