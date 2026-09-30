/* ============================================================================
   [LGLS 2026-09-29] CV_DATA 에 출고대 신호 ★래치★ 컬럼을 넣는다. (사용자 지시)

   왜
     현장 출고대는 화물이 도착하면 약 3초간만 신호(RET_READY_RD)를 올리고
     PLC 가 데이터를 지운다. 그런데 WCS_TASK_CV 는 그 신호를 ★사이클당 한 번★ 만
     읽고, 값이 바뀔 때만 DB 에 쓴다. 사이클이 길어지면 3초 창이 두 샘플 사이에
     통째로 들어가 신호를 한 번도 보지 못하고, 본 적이 없으니 DB 에도 남지 않는다.
     그러면 IO_TASK 는 완료할 근거가 없어 최후 안전망(60초)까지 기다린다.

     시뮬레이터 실측 (2026-09-29)
       · 펄스 3.0초 : 6건 모두 정상 (그러나 최악 +2.74초 - 여유 0.3초)
       · 펄스 1.1초 : 신호를 한 번도 못 봄 -> 77초 뒤 안전망으로 완료

   무엇
     "신호를 봤다" 는 사실을 레벨이 아니라 ★래치★ 로 남긴다.
       WCS_TASK_CV : ON 을 본 순간 래치를 세우고, 그때의 트래킹·시각을 함께 적는다.
                     신호가 OFF 로 돌아가도 래치는 내리지 않는다.
       IO_TASK     : 래치를 보고 완료하고, 완료한 뒤 래치를 지운다(소비).
     이러면 PLC 가 데이터를 지워도 관측만 됐으면 완료가 보장된다.

   ※ 이 스크립트를 돌리지 않아도 프로그램은 종전대로 동작한다.
      컬럼이 없으면 래치를 쓰지 않고 지금까지의 판정(신호 레벨 -> 실도착 -> 배출 -> 60초)
      으로 돈다. 돌리는 순간부터 보장이 켜진다.

   여러 번 돌려도 안전하다.
   ============================================================================ */

SET NOCOUNT ON;

/* ── 1) 래치 : 출고대 신호를 봤다 ('1' = 봤고 아직 소비되지 않음) ── */
IF NOT EXISTS (SELECT 1 FROM sys.columns
                WHERE object_id = OBJECT_ID('CV_DATA') AND name = 'RET_READY_LATCH')
BEGIN
    ALTER TABLE CV_DATA ADD RET_READY_LATCH varchar(1) NULL;
    PRINT 'RET_READY_LATCH 추가';
END
ELSE PRINT 'RET_READY_LATCH 이미 있음';

/* ── 2) 래치를 세운 순간의 트래킹(작업번호) ─────────────────────── */
/*      3초 뒤에는 LUGG_NO_RD 도 사라지므로, 그때의 값을 붙잡아 둬야       */
/*      나중에 "어느 작업이었나" 를 알 수 있다.                            */
IF NOT EXISTS (SELECT 1 FROM sys.columns
                WHERE object_id = OBJECT_ID('CV_DATA') AND name = 'RET_READY_LATCH_LUGG')
BEGIN
    ALTER TABLE CV_DATA ADD RET_READY_LATCH_LUGG varchar(10) NULL;
    PRINT 'RET_READY_LATCH_LUGG 추가';
END
ELSE PRINT 'RET_READY_LATCH_LUGG 이미 있음';

/* ── 3) 래치를 세운 시각 (진단용 - 신호부터 완료까지의 시간을 잰다) ── */
IF NOT EXISTS (SELECT 1 FROM sys.columns
                WHERE object_id = OBJECT_ID('CV_DATA') AND name = 'RET_READY_LATCH_DT')
BEGIN
    ALTER TABLE CV_DATA ADD RET_READY_LATCH_DT datetime NULL;
    PRINT 'RET_READY_LATCH_DT 추가';
END
ELSE PRINT 'RET_READY_LATCH_DT 이미 있음';

/* ── 확인 ────────────────────────────────────────────────────────── */
SELECT name AS 컬럼, TYPE_NAME(system_type_id) AS 자료형, max_length AS 길이
  FROM sys.columns
 WHERE object_id = OBJECT_ID('CV_DATA')
   AND name LIKE 'RET_READY_LATCH%'
 ORDER BY name;
