-- [LGLS 2026-09-05] 스케줄러 옵션 : 시간 기반 자동 처리 사용 여부
--   운전 화면 [환경설정] > [시간 기반 자동 처리] 버튼이 이 값을 켜고 끈다.
--   IO_TASK 는 이 값이 'Y' 일 때만 설비 신호 없이 경과시간으로 완료를 추정한다(기본 'N' = 사용 안 함).
USE LGLS_MCS_IO;

IF NOT EXISTS (SELECT 1 FROM COMMON_CODE WHERE WH_TYP = '10' AND CDX_CD = 'SCH_OPT' AND CCD_CD = 'AUTO_TIME')
BEGIN
    INSERT INTO COMMON_CODE (WH_TYP, CDX_CD, CCD_CD, CCD_CD_YN, CCD_EPR_ORD, CCD_NM_KOR, CCD_NM_ENG)
    VALUES ('10', 'SCH_OPT', 'AUTO_TIME', 'N', 1, N'시간 기반 자동 처리', 'Time-based auto handling');
END

SELECT WH_TYP, CDX_CD, CCD_CD, CCD_CD_YN, CCD_NM_KOR FROM COMMON_CODE WHERE CDX_CD = 'SCH_OPT';
