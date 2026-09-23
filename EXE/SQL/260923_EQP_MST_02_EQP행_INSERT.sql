/* ============================================================================
   [LGLS 2026-09-23] EQP_MST 에 설비종류 'EQP' 행을 넣는다. (사용자 지시)

   왜
     EQP 정보 창의 EQP 한 줄은 조회할 때 UNION ALL 로 C/V#11 의 IP/PORT 를 빌려
     ★만들어 내던 가상 행★ 이었다. 테이블에는 없는 줄이었다.
     화면과 데이터를 같게 맞춘다 - 진짜 행을 넣고 화면은 그 행을 그대로 읽는다.

   무엇
     EQP_TYP='EQP', PLC_NO='01' 한 행.
     주소는 마스터 PLC - C/V#11 행의 IP/PORT 를 그대로 가져온다.
     상태(CONNECTED_YN/UPD_DT)는 WCS_TASK_CV 가 갱신한다.
       (CvThread.CommunicationAllSlots 의 UPDATE 에 OR EQP_TYP='EQP' 를 더했다)

   여러 번 돌려도 안전하다 - 이미 있으면 주소/설비명만 다시 맞춘다.

   ※ C/V 행이 이미 지워진 뒤라면(03_설비행_정리.sql 실행 후) 가져올 주소가 없다.
      그때는 아래 @IP / @PORT 기본값이 쓰인다 - 현장 값으로 고쳐서 돌린다.
   ============================================================================ */

SET NOCOUNT ON;

DECLARE @WH_TYP varchar(10) = '10';            /* 창고 구분  - 현장 값에 맞춘다 */
DECLARE @IP     varchar(50) = '127.0.0.1';     /* 마스터 PLC - C/V 행이 없을 때만 쓰인다 */
DECLARE @PORT   varchar(20) = '2004';
DECLARE @PFROM  varchar(20) = '2004';
DECLARE @PTO    varchar(20) = '3001';

/* C/V#11(마스터 PLC 대표) 이 아직 있으면 그 주소를 그대로 쓴다 */
SELECT @IP    = ISNULL(PLC_IP,        @IP)
     , @PORT  = ISNULL(PLC_PORT,      @PORT)
     , @PFROM = ISNULL(PLC_PORT_FROM, @PFROM)
     , @PTO   = ISNULL(PLC_PORT_TO,   @PTO)
  FROM EQP_MST
 WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'CV' AND PLC_NO = '11';

IF NOT EXISTS (SELECT 1 FROM EQP_MST WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'EQP')
BEGIN
    INSERT INTO EQP_MST
          (WH_TYP, EQP_TYP, PLC_NO, SC_GRP_NO, PLC_IP, PLC_PORT, PLC_PORT_FROM, PLC_PORT_TO,
           SOCK_TIMEOUT, RETRY_YN, USE_YN, CONNECTED_YN, UPD_DT, REMARKS, PROCESS)
    VALUES (@WH_TYP, 'EQP', '01', NULL, @IP, @PORT, @PFROM, @PTO,
            '2', 'Y', 'Y', 'N', GETDATE(), 'EQP_TASK 설비통신 (마스터 PLC)', 'EQP_TASK');

    PRINT 'EQP 행을 넣었습니다.';
END
ELSE
BEGIN
    UPDATE EQP_MST
       SET PLC_IP        = @IP
         , PLC_PORT      = @PORT
         , PLC_PORT_FROM = @PFROM
         , PLC_PORT_TO   = @PTO
         , USE_YN        = 'Y'
         , REMARKS       = 'EQP_TASK 설비통신 (마스터 PLC)'
         , PROCESS       = 'EQP_TASK'
     WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'EQP';

    PRINT 'EQP 행을 마스터 PLC 주소에 맞췄습니다.';
END

/* 설비명(REMARKS)을 EQP 정보 창의 문구와 같게 맞춘다 */
UPDATE EQP_MST SET REMARKS = 'HOST_TASK 수신 소켓 (포트 열고 대기)'
 WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'HOST';
UPDATE EQP_MST SET REMARKS = 'HOST_TASK 송신 소켓 (상위로 보고)'
 WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'HOST2';
UPDATE EQP_MST SET REMARKS = 'IO_TASK Scheduler (SCH)'
 WHERE WH_TYP = @WH_TYP AND EQP_TYP = 'SCH';

/* ── 확인 ────────────────────────────────────────────────────────── */
SELECT EQP_TYP, PLC_NO, PLC_IP, PLC_PORT, USE_YN, CONNECTED_YN, REMARKS
  FROM EQP_MST
 WHERE WH_TYP = @WH_TYP AND EQP_TYP IN ('EQP','HOST','HOST2','SCH')
 ORDER BY CASE EQP_TYP WHEN 'EQP' THEN 1 WHEN 'HOST' THEN 2 WHEN 'HOST2' THEN 3 ELSE 4 END;
