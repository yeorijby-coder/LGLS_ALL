/* ============================================================================
   현장 DB 반영 (2026-09-21 최종판 기준)   대상 : LGLS_MCS_IO (SQL Server 2008 이상)
   09-11 판 이후 프로그램이 새로 쓰는 공통코드. 이미 있으면 건너뛴다(여러 번 실행 가능).
   ① JOB_STATUS 16  : 통로 C/V 구동중 (IO_TASK [CNF] IN_HS_STATUS=16 일 때 입고 통로 상태)
   ② SC_ERR_STA_FK 3 : 사용정지(DOWN) (WCS_TASK_CV [CNF] SC_ERR_CODE_BLOCK=1 또는 4 일 때 크레인 에러색)
   알람 코드는 260917_알람리스트_EQP_ECD_MST.sql 을 이어서 실행한다.
   ============================================================================ */
SET NOCOUNT ON;

IF NOT EXISTS (SELECT 1 FROM COMMON_CODE WHERE CDX_CD = 'JOB_STATUS' AND CCD_CD = '16' AND WH_TYP = '10')
    INSERT INTO COMMON_CODE (CDX_CD, CCD_CD, CCD_CD_YN, CCD_EPR_ORD, CCD_NM_KOR, CCD_NM_ENG, CCD_NM_HUN, CCD_NM_CHIN, WH_TYP)
    VALUES ('JOB_STATUS', '16', 'Y', 3, N'통로CV 구동중', N'Aisle CV Driving', N'', N'', '10');

IF NOT EXISTS (SELECT 1 FROM COMMON_CODE WHERE CDX_CD = 'SC_ERR_STA_FK' AND CCD_CD = '3' AND WH_TYP = '10')
    INSERT INTO COMMON_CODE (CDX_CD, CCD_CD, CCD_CD_YN, CCD_EPR_ORD, CCD_NM_KOR, CCD_NM_ENG, CCD_NM_HUN, CCD_NM_CHIN, WH_TYP)
    VALUES ('SC_ERR_STA_FK', '3', 'Y', 3, N'사용정지(DOWN)', N'DOWN', N'DOWN', N'停止(DOWN)', '10');

/* 확인 : 2행 */
SELECT CDX_CD, CCD_CD, CCD_NM_KOR FROM COMMON_CODE
 WHERE (CDX_CD = 'JOB_STATUS' AND CCD_CD = '16') OR (CDX_CD = 'SC_ERR_STA_FK' AND CCD_CD = '3');
GO
