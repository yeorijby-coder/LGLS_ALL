/* ============================================================================
   [LGLS 2026-09-23] EQP_MST 에 설비종류 'EQP' 행을 실제로 만든다. (사용자 지시)

   왜 :
     EQP 정보 창은 'EQP' 한 줄을 보여 주지만, 그 줄은 조회할 때 UNION ALL 로
     C/V#11 의 IP/PORT 를 빌려 ★만들어 내던 가상 행★ 이었다. 테이블에는 없었다.
     화면과 데이터를 같게 맞춘다 - 이제 진짜 행을 넣고 화면은 그 행을 그대로 읽는다.

   무엇을 :
     EQP_TYP='EQP', PLC_NO='01' 한 행. 주소는 마스터 PLC(= C/V#11 행과 같은 값).
     상태(CONNECTED_YN/UPD_DT)는 WCS_TASK_CV 가 C/V 행과 함께 갱신한다.

   건드리지 않는 것 :
     CV(15) · SC(5) · RTV(1) 행은 그대로 둔다. EQP 정보 창에는 안 보이지만
     WCS_TASK_CV / IO_TASK / 운전 화면의 설비 표시가 PLC_NO 단위로 쓰는 행이다.
     지우면 설비 통신과 화면이 함께 멈춘다.
   ============================================================================ */

SET NOCOUNT ON;

DECLARE @WH_TYP varchar(10) = '10';   /* 창고 구분 - 현장 값에 맞춘다 */

IF NOT EXISTS (SELECT 1 FROM EQP_MST WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'EQP')
BEGIN
    INSERT INTO EQP_MST
           (WH_TYP, EQP_TYP, PLC_NO, SC_GRP_NO, PLC_IP, PLC_PORT, PLC_PORT_FROM, PLC_PORT_TO,
            SOCK_TIMEOUT, RETRY_YN, USE_YN, CONNECTED_YN, UPD_DT, REMARKS, PROCESS)
    SELECT  @WH_TYP, 'EQP', '01', NULL, PLC_IP, PLC_PORT, PLC_PORT_FROM, PLC_PORT_TO,
            SOCK_TIMEOUT, RETRY_YN, 'Y', CONNECTED_YN, GETDATE(),
            'EQP_TASK 설비통신 (마스터 PLC)', 'EQP_TASK'
      FROM  EQP_MST
     WHERE  WH_TYP = @WH_TYP AND EQP_TYP = 'CV' AND PLC_NO = '11';

    PRINT 'EQP 행을 넣었습니다.';
END
ELSE
BEGIN
    UPDATE E
       SET E.PLC_IP        = C.PLC_IP
         , E.PLC_PORT      = C.PLC_PORT
         , E.PLC_PORT_FROM = C.PLC_PORT_FROM
         , E.PLC_PORT_TO   = C.PLC_PORT_TO
         , E.USE_YN        = 'Y'
         , E.REMARKS       = 'EQP_TASK 설비통신 (마스터 PLC)'
         , E.PROCESS       = 'EQP_TASK'
      FROM EQP_MST E
      JOIN EQP_MST C ON C.WH_TYP = E.WH_TYP AND C.EQP_TYP = 'CV' AND C.PLC_NO = '11'
     WHERE E.WH_TYP = @WH_TYP AND E.EQP_TYP = 'EQP';

    PRINT 'EQP 행을 마스터 PLC 주소에 맞췄습니다.';
END

/* 설비명(REMARKS)을 EQP 정보 창의 문구와 같게 맞춘다 */
UPDATE EQP_MST SET REMARKS = 'HOST_TASK 수신 소켓 (포트 열고 대기)'
 WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'HOST';
UPDATE EQP_MST SET REMARKS = 'HOST_TASK 송신 소켓 (상위로 보고)'
 WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'HOST2';
UPDATE EQP_MST SET REMARKS = 'IO_TASK Scheduler (SCH)'
 WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'SCH';

SELECT EQP_TYP, PLC_NO, PLC_IP, PLC_PORT, USE_YN, CONNECTED_YN, REMARKS
  FROM EQP_MST
 WHERE WH_TYP = @WH_TYP AND EQP_TYP IN ('EQP','HOST','HOST2','SCH')
 ORDER BY CASE EQP_TYP WHEN 'EQP' THEN 1 WHEN 'HOST' THEN 2 WHEN 'HOST2' THEN 3 ELSE 4 END;
