# -*- coding: utf-8 -*-
u"""2026-09-29 적용 안내서 (Word).

   사용자 지시 - 문서는 md 가 아니라 ★WORD★ 로 만든다(md 를 볼 장치가 없다).
   문서 스타일은 docs/산출물_20260927/gen/gen_common.py 를 그대로 쓴다.

   실행 : python gen_apply_docx.py
"""
import os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, os.path.join(ROOT, 'docs', '산출물_20260927', 'gen'))

from docx.shared import Pt, Cm, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
import gen_common as G

SHOT = os.path.join(HERE, 'shots')
NAVY, GRAY = G.NAVY, G.GRAY
RED = RGBColor(0xB0, 0x2A, 0x20)


def code(d, lines, size=9):
    u"""로그·SQL·코드 조각 - 고정폭에 옅은 바탕."""
    t = d.add_table(rows=1, cols=1)
    t.style = 'Table Grid'
    c = t.rows[0].cells[0]
    G.shade(c, 'F4F5F7')
    c.text = ''
    for i, ln in enumerate(lines):
        p = c.paragraphs[0] if i == 0 else c.add_paragraph()
        p.paragraph_format.space_after = Pt(0)
        r = p.add_run(ln)
        r.font.name = 'D2Coding'
        r.font.size = Pt(size)
    d.add_paragraph()


def note(d, text):
    p = d.add_paragraph()
    p.paragraph_format.left_indent = Cm(0.4)
    r = p.add_run('※ ' + text)
    r.font.size = Pt(9.5)
    r.font.color.rgb = RED
    return p


G.DATE = '2026-09-29'
G.VER = '1.0'
G.BASE_HISTORY = []
G.FINAL_NOTE = ('최초 작성 - 2026-09-29 적용분 (출고 완료 지연, 신호등 PLC, EQP_MST 옵션, 범례 정리, 4K 대응)'
                ' / 2026-09-30 Client 배포(Upload·Download)와 설치 꾸러미 추가')

d = G.new_doc('2026-09-29 적용 안내서',
              '출고 완료 지연 · 신호등 PLC · EQP_MST 옵션 · 범례 정리 · 4K 대응\n'
              'Client 배포(Upload · Download) · 설치 꾸러미')

# ════════════════════════════════════════════════════════════════
d.add_heading('1. 이 폴더는 무엇인가', 1)
G.para(d, '원격지(현장) 서버의 프로그램이 9월 23일 이전 판이라, 2026-09-29 에 작업한 것만 따로 모은 폴더다. '
          '현장 파일을 덮어쓰기 전에 반드시 백업한다.')
G.table(d, ['폴더', '내용'], [
    ['SQL', 'EQP_MST 정리·복구 SQL 3종'],
    ['Client', '운전 화면 Ecs.exe (Release 판) + 추가할 설정'],
    ['TASK/IO_TASK', '스케줄러 IO_TASK_SEMI_FINISH.exe + 추가할 설정'],
    ['TASK/HOST_TASK', '상위 통신 TASK_LFC10_G1_ECSCOM.exe + 추가할 설정'],
], widths=[4.5, 11.5])

d.add_heading('1.1 ini 는 통째로 넣지 않는다', 2)
G.para(d, '현장 ini 에는 DB 주소와 상위 IP 가 들어 있어 덮어쓰면 접속이 끊긴다. '
          '각 폴더의 「추가할_설정_*.txt」 에 있는 줄만 해당 섹션에 붙여넣는다.')
note(d, '붙여넣지 않아도 코드 기본값으로 동작한다. 즉 exe 만 바꿔도 돌아간다.')

d.add_heading('1.2 Client 는 Release 판이다', 2)
G.para(d, '현장 배포 폴더(EXE_NEWUI\\WCS_CLIENT)의 DLL 은 Release 로 빌드된 것이라, '
          'Debug 로 빌드한 Ecs.exe 를 넣으면 다음 오류가 난다.')
code(d, ['프로시저 시작 지점 ?Dump@?$CList@PAVCDciControl@@PAV1@@@UBEXAAVCDumpContext@@@Z 을(를)',
         'DLL ...\\Ecs.exe 에서 찾을 수 없습니다.'])
G.para(d, '이 심볼(CList<CDciControl*>::Dump)은 MFC 디버그 빌드에서만 만들어진다. '
          'Debug exe 가 그것을 요구하는데 Release DciLib.dll 에는 없어서 나는 오류다. '
          '이 폴더의 Ecs.exe 는 Release 로 빌드했으므로 DLL 과 짝이 맞는다.')
note(d, 'Ecs.exe 를 바꿀 때 DciLib.dll / EcsLib.dll / XmlLib.dll 도 같은 판으로 함께 바꾼다.')

d.add_page_break()

# ════════════════════════════════════════════════════════════════
d.add_heading('2. 출고 완료가 1~2분 걸리던 것', 1)
G.para(d, '대상 : TASK/IO_TASK', bold=True)

d.add_heading('2.1 증상', 2)
G.para(d, '자동 출고 작업이 끝났는데 작업이 상태 15(C/V 구동중)에 1~2분 머물다 완료됐다. '
          '출고대 준비 신호는 운전 화면에서 확실히 ON 이었다.')

d.add_heading('2.2 출고 완료는 원래 네 단계로 판정한다', 2)
G.para(d, 'IO_TASK 의 CompleteCV() 는 "출고 화물이 정말 출고대에 도착해서 나갔는가" 를 '
          '다음 순서로 확인한다. 위에서 성립하면 아래는 보지 않는다.')
G.table(d, ['순서', '무엇을 보나', '판정'], [
    ['①', '출고대 신호 (CV_DATA.RET_READY_RD = 1)',
     '설비가 "출고대에 화물이 놓였다" 를 직접 알린 것 → 곧바로 완료'],
    ['②', '실도착 관측 (센서 ON + 트래킹이 내 작업번호)',
     '신호를 놓쳤어도 내 화물이 출고대에 있는 것을 직접 봤다 → 기록해 두고 대기'],
    ['③', '배출 확인 (②를 본 뒤 트랙이 비었다)',
     '내 화물이 나간 것을 확인 → 완료'],
    ['④', '최후 안전망 (트래킹이 전 트랙에 없는 상태가 60초)',
     '아무것도 못 봤지만 이미 나간 것으로 인정 → 완료(로그에 경고)'],
], widths=[1.5, 6.5, 8.0])
G.para(d, '①이 가장 빠르고 정확하다. ④는 "어쩔 수 없을 때" 의 마지막 수단이라 60초를 기다린다.')

d.add_heading('2.3 왜 ①을 못 타고 ④까지 갔나 — 문턱이 둘 있었다', 2)
G.para(d, '신호가 ON 이어도 ①에 닿기까지 조건이 두 개 더 있었다.')

G.para(d, '문턱 ㉠ — 반출 시퀀스 가드', bold=True, size=10.5)
code(d, ['// 종전 : 신호를 보기도 전에 여기서 걸렀다',
         'if (OutSeqPending(luggNo)) continue;   // 반출(RTV) 시퀀스가 진행 중이면 판정 자체를 건너뜀'])
G.para(d, 'RTV 반출 시퀀스의 잔여 상태가 남아 있으면 신호를 볼 기회조차 없었다.')

G.para(d, '문턱 ㉡ — 트래킹이 내 번호여야 함', bold=True, size=10.5)
code(d, ['bool bMine = (cvLugg == "" || cvLugg == "0" || cvLugg == "0000" || cvLugg == luggNo);',
         'if (cvRetRdy == "1" && bMine) { ...출고 완료... }'])
G.para(d, '출고대 트랙의 트래킹 값(LUGG_NO_RD)이 내 작업번호이거나 비어 있어야 신호를 인정했다. '
          '그런데 앞 화물이 나간 뒤에도 설비 트래킹에 그 번호가 남는 일이 있다. '
          '그러면 cvLugg 는 "남의 번호" 가 되어 bMine 이 거짓이 된다.')

G.para(d, '두 문턱 중 하나만 걸려도 이렇게 흘러간다.', bold=True, size=10.5)
G.bullets(d, [
    '① 불성립 — 신호는 ON 인데 인정받지 못한다',
    '② 불성립 — 트래킹이 남의 번호라 "내 화물 실도착" 도 성립하지 않는다',
    '③ 불성립 — ②를 못 봤으니 배출 확인 경로로 못 간다',
    '④ 로 떨어진다 — 트래킹이 사라진 뒤 60초를 더 기다린 끝에 완료',
])
G.para(d, '설비가 트래킹을 지우는 시점이 제각각이라 60초 계측 시작도 늦어진다. '
          '이것이 현장에서 본 "1~2분" 의 정체다.')

d.add_heading('2.4 고친 내용', 2)
G.para(d, '출고대 신호를 최우선 근거로 삼는다. 다만 앞 화물이 아직 안 나간 경우를 조기 완료하면 안 되므로, '
          '"남의 번호" 가 살아 있는 작업인지 아닌지를 갈라서 본다.')
G.table(d, ['출고대 트래킹 상태', '종전', '지금'], [
    ['비었거나 내 번호', '신호로 완료', '신호로 완료 (같음)'],
    ['남의 번호인데 그 작업이 JOB_MST 에 없다',
     '완료 못 함 → 60초 대기', '유령 트래킹으로 보고 신호로 완료'],
    ['남의 번호이고 그 작업이 살아 있다',
     '완료 못 함 → 60초 대기', '앞 화물이 아직 안 나간 것 → 종전대로 기다림'],
], widths=[6.0, 5.0, 5.0])
G.para(d, '또 신호로 완료가 서면 반출 시퀀스 가드(문턱 ㉠)를 건너뛴다. '
          '신호 자체가 "출고대에 놓였다" 는 설비의 직접 통보라 시퀀스 잔여 상태보다 믿을 만하다.')
note(d, '신호를 못 본 경우의 ②③④ 경로는 한 줄도 바꾸지 않았다. 여전히 못 보면 1분 뒤 완료된다.')

d.add_heading('2.5 로그로 어느 경로인지 알 수 있다', 2)
code(d, ['[SCH][CV] 작업 0123 출고대 126 출고대 신호 ON - 출고 완료',
         '[SCH][CV] 작업 0123 출고대 126 트래킹에 끝난 작업 0119 가 남아 있습니다(유령) - 신호로 완료',
         '[SCH][CV] 작업 0123 출고대 126 실도착 관측 (신호/배출 대기)',
         '[SCH][CV] 작업 0123 출고대 126 배출 확인 - 출고 완료',
         '[SCH][CV] 작업 0123 출고대 126 - 출고대 신호도 실도착도 관측하지 못했습니다. ...'])
G.para(d, '첫 줄이나 둘째 줄이 나오면 신호로 곧바로 완료된 것이다. '
          '마지막 줄이 자주 나오면 설비 신호 배선을 점검할 일이다.')

d.add_heading('2.6 설정', 2)
G.table(d, ['키', '기본', '설명'], [
    ['OUT_DONE_BY_SIGNAL', '1',
     '1 = 위 내용대로. 0 = 전부 종전 동작(트래킹 일치 + 반출 시퀀스 완료를 요구)'],
    ['OUT_SIGNAL_MISS_SEC', '60',
     '④ 최후 안전망의 유예(초). 요청대로 1분을 유지했다. 5 미만은 5 로 올린다'],
], widths=[5.0, 1.8, 9.2])
G.para(d, '둘 다 ENV_IOSCH.INI 의 [CNF] 섹션에 넣는다. 호출할 때마다 읽으므로 고치면 곧 반영된다.')

d.add_page_break()

# ════════════════════════════════════════════════════════════════
d.add_heading('2-2. 3초 펄스를 놓치지 않게 — 근본 대책', 1)
G.para(d, '대상 : TASK/EQP_TASK, TASK/IO_TASK, SQL/04', bold=True)
G.para(d, '앞 장의 조치는 "신호를 봤을 때" 를 다뤘다. 그런데 현장에서는 신호를 '
          '아예 못 보는 경우가 있다는 것이 확인됐다. 이 장이 그 대책이다.')

d.add_heading('2-2.1 현장 조건', 2)
G.para(d, '출고 화물이 출고대에 도착하면 약 3초간 신호가 서 있고, 그 뒤 PLC 가 데이터를 지운다. '
          '즉 완료를 잡을 수 있는 창이 3초뿐이다.')

d.add_heading('2-2.2 왜 놓치나 — 두 겹의 문제', 2)
G.para(d, '① 출고대 신호는 사이클 머리에서 전 설비를 한 번에 읽는 M 블록에만 들어 있다. '
          '즉 ★사이클당 1회★ 만 본다. 사이클이 3초를 넘으면 그 창이 두 샘플 사이에 통째로 들어간다.')
G.para(d, '② 값이 바뀔 때만 DB 에 쓴다. ON 을 한 번도 못 보면 값이 계속 \'0\' 이라 '
          'DB 에 아무 일도 일어나지 않는다 — 신호가 있었다는 사실 자체가 어디에도 안 남는다.')
G.para(d, '그러면 IO_TASK 는 완료할 근거가 없어 최후 안전망(60초)까지 기다린다. '
          '이것이 현장에서 보신 "1~2분" 의 또 다른 갈래다.')

d.add_heading('2-2.3 시뮬레이터 재현 (2026-09-29)', 2)
G.table(d, ['조건', '펄스', '결과'], [
    ['사이클 ~2초', '3.0초', '6건 모두 정상. 다만 최악 +2.74초 — 소멸까지 여유 0.3초'],
    ['사이클 ~2초', '1.1초', '★신호를 한 번도 못 봄 → 77초 뒤 안전망으로 완료★'],
], widths=[3.5, 2.0, 10.5])
G.para(d, '3초 펄스에서도 여유가 0.3초뿐이었다. 현장에서 설비 대수·PLC 응답·DB 부하로 '
          '사이클이 조금만 길어지면 그대로 떨어진다.')

d.add_heading('2-2.4 대책 A — 관측을 촘촘하게', 2)
G.para(d, '출고대가 있는 설비의 M 워드만, 사이클 도중에도 짧은 주기로 다시 읽는다. '
          '왕복 1회라 수 ms 다. 설비 사이사이와 사이클 슬립 직전(공백이 가장 긴 구간)에 넣었다.')
note(d, '별도 스레드를 쓰지 않았다. 이 시스템은 마스터 PLC 에 소켓 1개로 붙으므로, '
        '두 스레드가 같은 소켓에 요청을 보내면 응답이 섞여 통신 전체가 깨진다. '
        '같은 스레드 안에 끼워 넣으면 그 위험 없이 샘플 간격이 0.3초가 된다.')

d.add_heading('2-2.5 대책 B — 에지를 래치', 2)
G.para(d, '"신호를 봤다" 를 레벨이 아니라 래치로 남긴다.')
G.table(d, ['누가', '무엇을'], [
    ['WCS_TASK_CV (EQP)', 'ON 을 본 순간 RET_READY_LATCH=\'1\' 과 그때의 트래킹·시각을 적는다. '
                          '신호가 꺼져도 내리지 않는다'],
    ['IO_TASK (SCH)', '래치를 보고 완료하고, 완료한 뒤 래치를 지운다(소비)'],
], widths=[4.5, 11.5])
G.para(d, '래치 시점의 트래킹을 함께 붙잡는 것이 중요하다 — 3초 뒤에는 그 값도 사라져 '
          '나중에 "어느 작업이었나" 를 알 수 없기 때문이다.')

d.add_heading('2-2.6 검증 결과', 2)
G.para(d, '적용 전 77초가 걸렸던 바로 그 조건(펄스 약 1.1초)에서 다시 돌렸다.')
code(d, ['작업 0022',
         '  20:45:36.867  시뮬 출고대 신호 ON',
         '  20:45:37.270  래치 기록          (+0.40초)   <- A 가 잡음',
         '  20:45:37.703  작업 완료 (19)     (+0.84초)   <- B 로 완료',
         '  20:45:37.941  배출 - 데이터 소멸 (+1.07초)',
         '',
         '작업 0024        신호 ON -> 완료 +0.65초'])
G.para(d, '두 건 모두 데이터가 사라지기 전에 완료됐다. 더 중요한 것은 그 사이 '
          'CV_DATA.RET_READY_RD 가 ★한 번도 1 이 되지 않았다★ 는 점이다. '
          '종전 판정 경로로는 이번에도 신호를 못 봤고, 오직 래치가 잡아냈다. '
          '두 대책이 각각 제 역할을 했다는 직접 증거다.', bold=True)

d.add_heading('2-2.7 SQL 을 돌리지 않아도 보장이 켜진다 — 파일 래치', 2)
G.para(d, '래치 컬럼(SQL 04)이 없는 DB 에서는 래치를 ★파일★ 에 남긴다. '
          '그래서 SQL 을 돌리지 않아도 출고 완료 보장이 그대로 동작한다.')
G.table(d, ['컬럼', '래치 저장소', '파일'], [
    ['없음', 'OutLatch.ini 의 [LATCH] 섹션', '기록·소비·고아정리를 모두 파일로'],
    ['있음', 'CV_DATA 컬럼', '[LATCH] 를 기동 후 1회 통째로 지운다'],
], widths=[2.0, 5.5, 8.5])
G.para(d, '컬럼이 생기면 파일에 남은 내용이 자동으로 없어진다 - 래치가 두 곳에 나뉘어 있는 '
          '상태가 생기지 않는다.')
code(d, ['[LATCH]',
         '126=1|0062|20260929214030      (상태 | 래치 시점 작업번호 | 시각)'])

G.para(d, '경로 — 두 TASK 가 같은 파일을 봐야 한다', bold=True, size=10.5)
G.para(d, '기본값은 <실행폴더>\..\OutLatch.ini 다. 현장 구조(EXE\TASK\<이름>\)에서는 '
          '공통 상위가 EXE\TASK 이므로 ★설정 없이 자동으로 공유된다.★')
note(d, '폴더 구조가 다르면(개발 bin\Debug 등) 상위가 서로 달라 공유되지 않는다. '
        '그때는 [CNF] OUT_LATCH_FILE 로 양쪽에 같은 절대 경로를 적는다.')

G.para(d, '검증 (래치 컬럼을 일부러 지우고)', bold=True, size=10.5)
code(d, ['파일 기록  126=1|0062|20260929214030',
         '파일 완료  신호 ON -> +1.19초 완료, RET_READY_RD 는 끝까지 0 (레벨로는 못 봄)',
         '파일 소비  완료 후 항목 사라짐',
         '컬럼 복원  재기동하니 [LATCH] 의 항목이 자동으로 지워짐'])

d.add_heading('2-2.7 검증 중 찾은 결함 — 고아 래치', 2)
G.para(d, '1초 펄스로 확인을 마친 뒤, 현장과 같은 3초 펄스로 되돌려 한 번 더 돌렸다. '
          '거기서만 드러나는 문제가 있었다.')
code(d, ['작업 0029',
         '  21:03:30.809  출고대 신호 ON',
         '  21:03:32.006  IO_TASK 가 신호 레벨로 보고 완료  <- 래치를 지우려 했으나 아직 없었다',
         '  21:03:32.340  고속 샘플링이 아직 켜져 있는 신호를 보고 래치를 세움',
         '                -> 작업은 끝났는데 래치만 남았다'])
G.para(d, '3초 펄스에서는 레벨로도 신호가 보이므로 완료가 먼저 끝나고, 그 뒤에 A 가 '
          '아직 ON 인 신호를 보고 래치를 세운다. 소비할 작업이 없으니 고아로 남는다.')
G.para(d, '그대로 두면 그 출고대에 다음 화물이 오기 전에, 다음 출고 작업이 15 가 되는 순간 '
          '남아 있던 래치를 보고 ★도착하기도 전에 완료★ 할 수 있다. '
          '조기 완료는 출고 지연보다 나쁜 고장이다.', bold=True)
G.para(d, '막은 방법 — 완료 판정 앞에서 쓸모없는 래치를 먼저 지운다(SweepOrphanLatch). '
          '둘 중 하나면 고아로 본다.')
G.bullets(d, [
    '그 출고대를 목적지로 하는 진행 중(15) 출고 작업이 없다',
    '래치가 선 지 오래됐다 ([CNF] LATCH_TTL_SEC, 기본 30초)',
])
G.para(d, '200ms 주기로 돌므로 다음 출고가 오기 훨씬 전에 정리된다. '
          '적용 후 3초 펄스로 다시 돌려 고아가 남지 않는 것을 확인했다.')
note(d, '1초 펄스 검증만 했다면 이 결함을 놓쳤을 것이다. 그 조건에서는 레벨이 아예 안 보여 '
        '래치가 항상 정상 소비됐다. 정상 조건에서만 나타나는 문제였다.')

d.add_heading('2-2.7 설정과 적용', 2)
G.table(d, ['키', '기본', '어디에'], [
    ['OUT_SCAN_MS', '300', 'WCS_DB.INI [CNF] — 출고대 고속 확인 간격(ms). 0 이면 끈다'],
    ['LATCH_TTL_SEC', '30', 'ENV_IOSCH.INI [CNF] — 이만큼 지난 래치는 고아로 보고 지운다'],
], widths=[4.0, 1.8, 10.2])
G.para(d, '래치는 CV_DATA 에 컬럼 3개가 필요하다. '
          'SQL/260929_CV_DATA_04_출고대신호_래치컬럼.sql 을 돌린다.')
note(d, 'SQL 을 돌리지 않아도 프로그램은 종전대로 동작한다. 두 TASK 가 시작할 때 컬럼 유무를 '
        '한 번 확인하고, 없으면 래치를 쓰지 않는다. 돌리는 순간부터 보장이 켜진다.')

d.add_page_break()

# ════════════════════════════════════════════════════════════════
d.add_heading('3. 신호등 아래 글자 EQP → PLC', 1)
G.para(d, '대상 : Client', bold=True)
G.para(d, '리본 [통신] 그룹의 셋째 칸 이름을 PLC 로 바꿨다.')
G.image(d, os.path.join(SHOT, 'ribbon_plc.png'), 16.0,
        '리본 [통신] 그룹 - WMS1 / WMS2 / PLC / SCH')
G.para(d, 'EQP_MST 의 EQP_TYP 값은 아직 \'EQP\' 다. 나중에 \'PLC\' 로 바꿀 것을 감안해 '
          '운전 화면의 조회는 \'EQP\' · \'PLC\' · \'CV\' 를 모두 같은 칸으로 본다. '
          '값을 바꿔도 화면은 그대로 돌고, 바꾸지 않아도 그대로 돈다.')

d.add_page_break()

# ════════════════════════════════════════════════════════════════
d.add_heading('4. EQP_MST 행을 줄여도 / 안 줄여도 동작', 1)
G.para(d, '대상 : TASK/HOST_TASK, SQL', bold=True)

d.add_heading('4.1 먼저 - CLR20r3 의 원인에 대해', 2)
G.para(d, '행 삭제가 원인이라고 보셨는데, EQP_MST 를 쓰는 곳을 모두 확인한 결과 그렇게 보기 어렵다.')
G.table(d, ['프로그램', '하는 일', '행이 0개일 때'], [
    ['HOST_TASK CCliWork', 'SELECT 후 건수 분기', 'iCnt == 0 분기가 있어 정상 동작'],
    ['HOST_TASK frmMain', '통신상태 UPDATE', '고친 행 0 - 예외 없음'],
    ['WCS_TASK_CV', '하트비트 UPDATE', '고친 행 0 - 예외 없음'],
    ['IO_TASK', 'UPDATE 후 없으면 INSERT', '스스로 행을 만든다'],
], widths=[4.5, 4.5, 7.0])
G.para(d, '행이 없어서 나는 증상은 운전 화면 신호등이 미접속으로 보이는 것이지 프로세스 종료가 아니다.')
G.para(d, '★시험으로 확인했다★ - EQP_MST 를 현장과 같은 구조(CV 15 · SC 5 · RTV 1 + HOST/HOST2/SCH)로 '
          '되돌리고 HOST_TASK 를 돌렸다. 크래시 없음, 통신 4채널 정상, 작업 흐름 정상이었다. '
          'CLR20r3 의 실제 원인은 따로 있다 - 6-5 장을 보라.', bold=True)

d.add_heading('4.2 그래도 요청대로 옵션을 넣었다', 2)
G.table(d, ['키', '기본', '설명'], [
    ['EQP_MST_AUTO_ROW', '1',
     '1 = 5초 주기 통신상태 기록에서 고친 행이 없으면 그 행을 스스로 만든다. '
     '0 = UPDATE 만 한다(행이 없으면 신호등은 미접속으로 보인다)'],
], widths=[5.0, 1.8, 9.2])
G.para(d, 'EcsComA.ini 의 [Host] 섹션에 넣는다. '
          '행을 줄여도 신호등이 정상으로 보이고, 줄이지 않아도 종전대로 돈다.')

d.add_heading('4.3 SQL 3종', 2)
G.table(d, ['파일', '하는 일'], [
    ['260929_EQP_MST_01_원상복구.sql',
     '설비통신 행을 지우고 CV(15)·SC(5)·RTV(1) 21건을 되돌린다'],
    ['260929_EQP_MST_02_설비통신행_INSERT.sql',
     '설비통신 행 1건을 넣는다(이미 있으면 주소만 맞춘다)'],
    ['260929_EQP_MST_03_설비행_정리.sql',
     'CV/SC/RTV 를 지워 4행만 남긴다'],
], widths=[7.5, 8.5])
G.bullets(d, [
    '02 안의 @EQP_TYPE 한 줄만 \'PLC\' 로 고치면 종류 값까지 바꿀 수 있다',
    '@WH_TYP 은 현장 창고 구분에 맞춘다 (기본 \'10\')',
    '세 파일 모두 여러 번 돌려도 안전하다',
])

d.add_page_break()

# ════════════════════════════════════════════════════════════════
d.add_heading('5. 범례', 1)
G.para(d, '대상 : Client', bold=True)

d.add_heading('5.1 뺀 항목', 2)
G.para(d, '범례 판넬과 범례 설정 창 양쪽에서 없앴다. 설정 창은 리소스에서 실제로 지웠다.')
G.bullets(d, [
    '랙투랙 · 호기간 이동',
    '반자동 랙투랙 · 반자동 호기간 이동',
    '입고 HS · 출고 HS',
    '통신두절 · 작업번호 있음',
    '레일 관련 전부 (입고 금지 · 출고 금지 · 입출고 정지 · 레일 에러 · 작업중)',
    '창고간 이동 (이미 숨겨져 있어 같이 정리)',
])
note(d, '색 자체는 그대로 쓰인다. 범례에서만 뺀 것이고, 랙투랙 작업이 들어오면 여전히 그 색으로 칠한다.')

d.add_heading('5.2 화물만 있으면 살색이 되던 이유', 2)
G.para(d, 'TrackInfo.cpp 의 GetCvColor() 맨 끝에 이런 줄이 있었다.')
code(d, ['//화물D',
         'if (V_JOB_TYP_RD == "0" && V_SENSOR0_DATA_RD == "1")',
         '    return WHEAT;          // RGB(255,225,192) = 살색'])
G.para(d, '작업구분이 0(작업 없음)이고 화물감지 센서만 켜져 있으면 살색으로 칠했다. '
          '구 ECS 부터 있던 "작업은 없는데 화물은 놓여 있다" 표시인데, '
          '범례에 없는 색이라 화면에서만 튀었다.')
G.para(d, '요청대로 이 줄을 없앴다. 이제 화물만 있는 트랙은 빈 트랙과 같은 회색이고, '
          '화물이 있다는 것은 트랙 네 모서리의 검은 점 4개로만 보인다. '
          '이 표시는 CDciTrackCtrl 이 센서값(m_bExist)을 그대로 받아 그리므로 따로 손댈 것이 없었다.')

d.add_heading('5.3 범례에 「화물 감지」 를 넣었다', 2)
G.para(d, '색으로 칠하지 않게 됐으니 범례에도 그 표시를 넣어야 뜻이 통한다. '
          '색칩이 아니라 트랙과 같은 모양(회색 바탕 + 네 모서리 검은 점)으로 그린다.')
G.image(d, os.path.join(SHOT, 'legend_panel.png'), 15.0,
        '메인 화면 왼쪽 범례 판넬 - C/V 상태 그룹에 「화물 감지」')

d.add_heading('5.4 숨김 체크박스', 2)
G.para(d, '범례 설정 창의 색상 칸 옆에 [숨김] 체크박스를 뒀다. '
          '체크하고 [저장] 하면 메인 화면 왼쪽 범례 판넬에서 그 줄이 빠지고 나머지가 당겨진다. '
          'Ecs.ini 의 [USER] LEGEND_HIDE_* 에 저장되므로 다시 켜도 유지된다.')
G.image(d, os.path.join(SHOT, 'legend_dlg.png'), 16.0,
        '범례 설정 창 - 4그룹 12칸, 항목마다 [숨김]')

d.add_heading('5.5 도면 안의 「범 례」 표는 없앴다', 2)
G.para(d, '메인 화면 도면 위에 레이아웃 도형으로 박힌 범례표가 또 하나 있었다. '
          '왼쪽 판넬과 내용이 어긋나 있어(랙투랙·HS·레일이 그대로 남아 있었다), '
          '현장 배포본(EXE_NEWUI\\WCS_CLIENT)의 레이아웃으로 교체했다.')
G.para(d, '교체한 파일 : EcsLayout1.xml · EcsLayout2.xml · EcsLayout3.xml '
          '(종전 파일은 같은 폴더에 .bak_0929 로 남겨 두었다)')
G.para(d, '이 레이아웃에는 작업대 명칭(제품 입고대 · 원부자재불출대 · 피킹 출고/입고 작업대 · '
          '외부 입고 전용 입출고대)도 함께 들어 있다.')

d.add_page_break()

# ════════════════════════════════════════════════════════════════
d.add_heading('6. 4K(3840×2160) 대응', 1)
G.para(d, '대상 : Client', bold=True)
G.para(d, '하나의 Client 로 1920×1080 과 3840×2160 을 모두 받는다. 화면 높이를 보고 배율을 정한다.')
G.table(d, ['화면 높이', '배율'], [
    ['2160 이상', '200%'], ['1800 이상', '175%'], ['1600 이상', '150%'],
    ['1300 이상', '125%'], ['그 미만', '100%'],
], widths=[5.0, 4.0])
G.para(d, '리본 [통신] 신호등의 치수·글꼴과 왼쪽 범례 판넬의 치수·글꼴이 이 배율을 따른다.')

d.add_heading('6.1 신호등이 2줄(위 3 / 아래 1)로 접히던 이유', 2)
G.para(d, 'MFC 리본은 패널 안을 「행」 단위로 채운다. 신호등 칸이 62×56 픽셀에 박혀 있어서 '
          '4K 에서는 상대적으로 작아지고, 그러면 한 패널에 두 행이 들어갈 여유가 생겨 '
          'MFC 가 4칸을 3+1 로 나눠 담았다.')
G.para(d, '칸을 화면에 맞게 키우면 한 행만 들어가 다시 한 줄이 된다. '
          '즉 배율을 키우는 것이 곧 이 문제의 해결이다.', bold=True)

d.add_heading('6.2 설정', 2)
G.table(d, ['키', '기본', '설명'], [
    ['UI_SCALE', '0', '0 = 자동(위 표대로). 100~300 = 그 값을 그대로 쓴다'],
], widths=[5.0, 1.8, 9.2])
G.para(d, 'Ecs.ini 에 [DISPLAY] 섹션을 만들어 넣는다. 고치면 3초 안에 반영된다(다시 그리는 시점에). '
          '4K 에서 여전히 접히면 값을 더 키운다.')
note(d, '4K 화면이 없어 이 항목만은 코드 경로로만 확인했다. 현장에서 값 조정이 필요할 수 있다.')

d.add_page_break()

# ════════════════════════════════════════════════════════════════
d.add_heading('6-2. 크레인 작업번호를 지시 시점부터 표시', 1)
G.para(d, '대상 : Client', bold=True)
G.para(d, '종전에는 크레인에 작업이 지시돼도 번호가 보이지 않다가, 크레인이 화물을 실어야 '
          '비로소 나타났다. 그래서 작업을 받아 화물을 뜨러 가는 동안에는 그 크레인이 '
          '노는 것처럼 보였다.')
G.para(d, '원인은 CalcScText 의 이 줄이었다.')
code(d, ['// [LGLS 2026-09-11] 번호도 차상 비트로 거른다',
         'if (!CLib::IsVehicleLoaded(pData->V_SENSOR_FK_RD)) return;'])
G.para(d, '차상(적재) 비트가 서야 번호를 그렸다. 그런데 작업정보 캐시는 이미 '
          '25(크레인 지시) 부터 그 호기에 작업을 매달고 있으므로, 이 게이트만 풀면 된다.')
G.para(d, '「번호와 색은 늘 함께 간다」 는 원칙을 지켜 색을 내는 두 곳의 같은 게이트도 '
          '함께 풀었다. 번호만 풀면 색 없이 번호만 나온다.')
G.table(d, ['키', '기본', '설명'], [
    ['VEH_SHOW_ON_ORDER', '1',
     '1 = 작업이 지시된 시점(25)부터 번호·색을 보인다. 0 = 종전(화물을 실어야)'],
], widths=[4.5, 1.8, 9.7])
G.para(d, 'Ecs.ini 의 [MENU] 섹션에 넣는다. 하역 뒤 잔상 처리는 VEH_CLEAR_MODE 가 그대로 맡는다.')

d.add_page_break()

d.add_heading('6-3. RTV 도 작업 지시 시점부터 작업번호 표시', 1)
G.para(d, '대상 : Client', bold=True)
G.para(d, 'S/C 와 같은 문제가 RTV 에도 있었다 - 차상(적재) 비트를 기다려, 화물을 실어야 '
          '번호가 보였다. 같은 규칙으로 풀었다. 작업정보 캐시는 35(RGV 반송중) 부터 값을 준다.')
G.para(d, '색을 내는 자리도 함께 풀었다 - 「번호와 색은 늘 함께」 원칙. '
          '설정은 S/C 와 같은 VEH_SHOW_ON_ORDER 하나로 묶었다.')

d.add_page_break()

d.add_heading('6-4. 출발지 / 도착지 표기', 1)
G.para(d, '대상 : Client', bold=True)
G.para(d, '이름표의 원본은 DEST_POS_DEF.REMARKS 다. '
          '현장에서 REMARKS 를 고치면 표기도 따라간다 - 프로그램을 다시 만들 필요가 없다.')
code(d, ['작업대 : "IMS101 C/V#11 입출고대 TR#22"',
         '크레인 : "S/C#1"'])

d.add_heading('6-4.1 작업 정보 창', 2)
G.table(d, ['구분', '표기', '예'], [
    ['작업대', '작업대명[IMS 번호] TR#트랙번호(2자리)', '피킹입고[102] TR#30'],
    ['크레인', 'S/C #n호기[코드]', 'S/C #2호기[902]'],
], widths=[2.0, 7.0, 7.0])
G.para(d, '출발 위치 / 도착 위치 칸은 종전대로 따로 둔다.')

d.add_heading('6-4.2 작업 판넬 (메인 화면 왼쪽)', 2)
G.para(d, '출발지와 도착지는 랙·작업대 중 하나만 나오므로, 위치까지 ★한 칸★ 에 넣고 '
          '「출발위치」「도착위치」 칸은 없앴다.')
G.table(d, ['구분', '표기', '예'], [
    ['랙(크레인)', 'S/C #n[셀 위치]', 'S/C #1[02-001-01]'],
    ['작업대', '작업대명[IMS 번호] TR#트랙번호(2자리)', '입출고대[101] TR#22'],
], widths=[2.5, 6.5, 7.0])
note(d, '두 창이 같은 함수(CEcsDoc::PosLabel)를 쓰므로 규칙이 어긋나지 않는다. '
        '이름표를 못 찾으면 코드를 그대로 보인다 - 없는 정보를 지어내지 않는다.')

d.add_page_break()

d.add_heading('6-5. HOST_TASK 가 기동 직후 죽던 원인 (CLR20r3)', 1)
G.para(d, '대상 : TASK/HOST_TASK', bold=True)
G.para(d, '"ALL_TASK 적용했는데 CLR20r3" 로 보고하신 건이다. '
          'EQP_MST 행을 줄인 것이 원인이라고 보셨지만 ★아니었다.★ '
          '현장과 같은 구조(CV 15 · SC 5 · RTV 1)로 되돌려 돌려 봐도 HOST_TASK 는 멀쩡했다.')

d.add_heading('6-5.1 원인 ① ini 키 누락 — 메시지 없이 죽는다', 2)
G.para(d, '이것이 증상과 가장 잘 맞는다.', bold=True)
code(d, ['GetPrivateProfileString("Network", "LocalPort", "", sb, ...);   // 기본값이 ""',
         'modDefApp.g_iListenPort = int.Parse(sb.ToString());             // int.Parse("") -> 예외'])
G.para(d, '게다가 DBLogIn 에서 ReadInitProfile() 이 ★try 블록 밖★ 이라 아무도 잡지 않는다. '
          '그래서 기동 직후 메시지 한 줄 없이 프로세스가 사라진다.')
G.table(d, ['증상', '설명'], [
    ['HOST 만 죽는다', 'IO·EQP 는 각자 다른 ini 와 코드를 쓴다'],
    ['기동하자마자 죽는다', 'DB 로그인은 기동 직후다'],
    ['다른 프로세스가 없어도 죽는다', '포트와 무관 - 값을 읽는 단계에서 터진다'],
    ['ini 를 고쳐도 죽는다', '그 키가 빠져 있으면 계속 같다'],
    ['ODBC 오류는 메시지가 떴다', '그건 try 안이라 잡혔다'],
    ['이건 메시지가 없었다', '이건 try 밖이라 못 잡았다'],
    ['9/17 판은 정상', '그때 쓰던 ini 에는 두 키가 다 있었다'],
], widths=[5.0, 11.0])

G.para(d, '없으면 죽는 키는 두 개다.', bold=True, size=10.5)
code(d, ['[Network]', 'LocalPort=8001', 'RemotePort=8002'])
note(d, '[DB] 쪽 키(IP·DATABASE·USER·USER_PW)는 없어도 죽지 않는다 - 문자열이라 '
        '접속 실패 메시지만 뜬다. 그래서 ODBC 오류는 보이고 포트 키 누락은 조용히 죽었다.')

G.para(d, '고친 방법 — 값을 지어내지 않는다. 엉뚱한 포트로 열리면 더 나쁘다. '
          '대신 무엇이 빠졌는지 말하고 멈춘다.')
code(d, ['설정 파일을 읽지 못했습니다.',
         '  파일 : ...\\EcsComA.ini',
         '  항목 : [Network] LocalPort',
         '  읽은 값 : (없음)',
         '',
         '이 항목이 없거나 숫자가 아니면 프로그램을 시작할 수 없습니다.',
         '설정 파일에 다음처럼 넣어 주세요.',
         '    [Network]',
         '    LocalPort=8001'])
G.para(d, 'LocalPort 줄을 일부러 지우고 기동해 확인했다 - 죽지 않고 위 메시지가 떴다.')

d.add_heading('6-5.2 원인 ② 소켓 예외 — 이벤트 로그에 남아 있었다', 2)
G.para(d, 'Windows 이벤트 로그(.NET Runtime)에 실제 스택이 두 건 남아 있었다.')
code(d, ['2026-09-28 16:03  TASK_LFC10_G1_ECSCOM.exe',
         '  SocketException  Socket.Shutdown() <- CloseSocket <- CliWorkThread',
         '',
         '2026-09-22 19:16  ALL_TASK.exe',
         '  SocketException  Socket.Bind() -> TcpListener.Start() <- ListenThread'])
G.bullets(d, [
    'Shutdown : Connected 는 "마지막 I/O 시점의 상태" 라, 확인과 Shutdown 사이에 '
    '상대가 끊으면 예외가 난다. 상대가 먼저 끊는 것은 통신에서 늘 있는 일이지 고장이 아니다.',
    '그 자리의 Monitor.Enter/Exit 가 try/finally 로 감싸여 있지 않아, 예외가 나면 '
    '락이 영영 풀리지 않았다 - 죽지 않더라도 이후 소켓 종료가 모두 멈춘다.',
    'Bind : 수신 포트를 이미 다른 인스턴스가 쓰면 난다. 중복 실행 방지가 ★프로세스 이름★ '
    '기준이라 ALL_TASK 와 단독 HOST_TASK 는 서로를 막지 못한다.',
])
G.para(d, '셋 다 고쳤다. Shutdown/Close 를 각각 감싸고, 락을 try/finally 로 바꾸고, '
          'Bind 실패는 사유를 남긴 뒤 10초마다 다시 시도한다.')

d.add_heading('6-5.3 그리고 안전망', 2)
G.para(d, '작업 스레드의 미처리 예외는 .NET 에서 프로세스를 통째로 끝낸다. '
          'SafeRun 으로 네 스레드(SRV·LSN·CLI·LOG)를 감싸, 예외가 나도 그 스레드만 끝나고 '
          '프로그램은 살아남는다. 실제로 포트 충돌을 일부러 만들어 확인했다 - '
          '종전에는 프로세스가 죽었고, 지금은 HOST_LSN 만 멈추고 나머지는 계속 돌았다.')
note(d, 'SafeRun 은 보험이고, 위 두 수정이 근본 처리다. 둘 다 둔다.')

d.add_page_break()

d.add_heading('7. 적용 순서', 1)
G.numbered(d, [
    '현장 프로그램과 ini 를 백업한다.',
    '★EcsComA.ini 의 [Network] 에 LocalPort 와 RemotePort 가 있는지 먼저 확인한다.★ '
    '(없으면 HOST_TASK 가 기동 직후 죽는다 - 6-5 장)',
    'TASK 를 정지한다.',
    'IO_TASK / HOST_TASK 의 exe 를 교체한다.',
    '각 폴더의 「추가할_설정_*.txt」 에 있는 줄을 현장 ini 의 해당 섹션에 붙여넣는다. (건너뛰어도 된다)',
    'TASK 를 기동한다. 기동 순서는 EQP → IO → HOST.',
    'Client 는 Ecs.exe 와 DciLib/EcsLib/XmlLib.dll 을 함께 교체하고, 레이아웃 3종도 함께 넣는다.',
    'SQL 은 필요한 것만 돌린다. 행을 줄일 때 02 → 03, 되돌릴 때 01.',
    '출고 완료를 3초 안에 보장하려면 SQL 04(래치 컬럼)를 돌리고 EQP_TASK 도 함께 교체한다.',
    'Client 를 DB 로 배포하려면 SQL UPDOWN_01 을 돌리고 TASK/Upload 를 담당자 PC 에 둔다. (8 장)',
    '새 PC 에 Client 를 깔 때는 Client_설치 폴더를 통째로 가져가 Setup.bat 을 관리자로 실행한다. (9 장)',
])

d.add_heading('7.1 확인하면 좋은 것', 2)
G.table(d, ['항목', '무엇을 보나'], [
    ['출고 완료', 'IO_TASK 로그에 「출고대 신호 ON - 출고 완료」 또는 「신호 래치 - 출고 완료」 가 나오는가'],
    ['래치 동작', 'EQP/IO 두 TASK 로그에 「CV_DATA.RET_READY_LATCH 있음」 이 찍히는가 (없으면 SQL 04 미적용)'],
    ['크레인 번호', 'S/C·RTV 가 화물을 뜨러 가는 동안에도 작업번호와 색이 보이는가'],
    ['출발/도착', '작업 정보 창과 작업 판넬에 이름표 형식으로 보이는가'],
    ['신호등', '리본 [통신] 셋째 칸이 PLC 로 보이는가'],
    ['4K TV', '신호등 4칸이 한 줄로 나오는가, 글자 크기가 맞는가'],
    ['범례', '뺀 항목이 안 보이는가, 「화물 감지」 가 보이는가, [숨김] 체크가 반영되는가'],
    ['도면', '도면 위의 옛 「범 례」 표가 사라졌는가, 작업대 명칭이 보이는가'],
    ['배포', 'WmsUp.exe 조회에 올린 내역이 보이는가, 운전 PC 에서 바뀐 파일만 받아 오는가'],
], widths=[3.5, 12.5])

d.add_page_break()

d.add_heading('8. Client 를 DB 로 배포한다 - Upload / Download', 1)
G.para(d, '고친 Client 를 PC 마다 돌아다니며 복사하지 않아도 되게 한다. '
          '고친 사람이 WmsUp.exe 로 한 번 올려 두면, 각 PC 는 Client 를 띄울 때 '
          'EcsClient.exe 가 DB 와 견주어 바뀐 파일만 내려받고 이어서 Ecs.exe 를 띄운다.')
note(d, '앞 현장(HUONS)에서 쓰던 프로그램을 이 현장에 맞춘 것이다. 참조/Updownload 와 참조/WCS_HUONS 가 원본이다.')

d.add_heading('8.1 어디에 무엇이 있나', 2)
G.table(d, ['것', '위치', '무엇'], [
    ['올리는 쪽', 'TASK/Upload/  (WmsUp.exe)', '고친 파일을 골라 DB 에 올린다. 개발/보수 담당자 PC 에만 둔다.'],
    ['내려받는 쪽', 'TASK/Download/  (EcsClient.exe)', 'Client 를 띄우기 전에 바뀐 것만 받아 온다. 모든 운전 PC 에 둔다.'],
    ['테이블', 'SQL/260929_UPDOWN_01_테이블생성.sql', 'DN_MST / UP_DOWN / DN_SEQ 를 만든다.'],
    ['설치 꾸러미', 'Client_설치/', '새 PC 에 Client 를 까는 꾸러미. 9 장.'],
], widths=[2.6, 5.4, 8.0])

d.add_heading('8.2 테이블 세 개', 2)
G.table(d, ['테이블', '무엇'], [
    ['DN_MST', '한 번의 올리기가 한 행. 언제 누가 무엇을 고쳐 올렸는지.'],
    ['UP_DOWN', '파일 하나가 한 행. 파일 내용이 UP_DAT(varbinary)에 통째로 들어간다.'],
    ['DN_SEQ', '올리기 번호를 매기는 1행짜리 표.'],
], widths=[3.0, 13.0])
note(d, '앞 현장은 SEQUENCE 를 썼는데 SEQUENCE 는 SQL Server 2012 부터다. '
        '현장 DB 가 2008 이라 쓸 수 없어 1행짜리 표로 바꿨다. 2012 이상에서도 그대로 돈다.')
G.para(d, 'SQL 은 몇 번을 돌려도 같은 결과가 되게 썼다(이미 있으면 건너뛴다). 자료가 든 테이블은 지우지 않는다.')

d.add_heading('8.3 이 현장에 맞추며 고친 것', 2)
G.table(d, ['무엇', '왜'], [
    ['접속 정보를 ini 에서 직접 읽는다',
     '앞 현장은 WmsInfo.dll 이 키(INFO=P1)로 주소를 풀어 주었다. '
     '그 dll 안에는 그 현장 서버만 들어 있어 여기서는 쓸 수 없다. '
     'WmsDown.ini / WmsUp.ini 의 [DB Server] 를 읽게 바꿨다.'],
    ['USERID 를 비우면 Windows 인증',
     '종전 접속 문자열은 Trusted_Connection=False 로 박혀 있어, 계정을 비우면 '
     '"사용자 \'\'이(가) 로그인하지 못했습니다" 가 났다. 이제 둘 다 된다.'],
    ['상세 테이블 이름을 UP_DOWN 으로',
     '올리는 쪽의 MS-SQL 분기만 옛 이름(DN_DTL)이었다. 내려받는 쪽이 읽는 것은 UP_DOWN 이라 '
     '서로 맞지 않았다. 함께 빠져 있던 세 칸(DN_PGM / UP_DT / DN_DIR)도 채웠다.'],
    ['파일 내용을 varbinary 로',
     '올리는 쪽 생성문이 UP_DAT 을 varchar(max) 로 만들고 있었다. '
     '파일 내용을 문자로 담으면 깨진다.'],
    ['날짜 조건을 표준 변환으로',
     '조회에 wms_sf_Get_DateTime_KMS 라는 앞 현장의 함수를 쓰고 있었다. 여기엔 없다.'],
    ['DataReader 를 닫는다',
     '내려받기가 ★아예 되지 않던 원인★. 첫 조회 결과를 쓰는 코드가 통째로 주석이 되면서 '
     '닫는 줄까지 같이 묻혔다. "이 Command와 연결된 DataReader가 이미 열려 있습니다" 가 뜨고 끝났다.'],
    ['목록을 표준 표로',
     '화면의 목록이 FarPoint Spread(유료)였다. 라이선스가 없어 빌드할 때 '
     '"Spread.NET License Notification" 창이 떠서 빌드가 멈추고, 넘겨도 띄울 때 또 뜬다. '
     '목록을 보여 주는 데 유료 부품이 필요하지 않아 .NET 기본 표로 바꿨다.'],
], widths=[4.2, 11.8])

d.add_heading('8.4 설정', 2)
G.para(d, '올리는 쪽 - TASK/Upload/WmsUp.ini')
code(d, [
    '[DB Server]',
    'SERVERNAME=localhost\\SQLEXPRESS',
    'DATABASE=LGLS_MCS_IO',
    'USERID=              ; 비우면 Windows 인증',
    'PASSWORD=',
    '',
    '[DOWNLOAD PROGRAM]',
    'CNT=1',
    '1=WCS                ; 올릴 때 고르는 프로그램 구분',
])
G.para(d, '내려받는 쪽 - Client 폴더의 WmsDown.ini')
code(d, [
    '[DB Server]',
    'SERVERNAME=localhost\\SQLEXPRESS',
    'DATABASE=LGLS_MCS_IO',
    'USERID=',
    'PASSWORD=',
    '',
    '[RUN_FILE]',
    'FILE1=.\\Ecs.exe      ; 내려받기가 끝나면 이것을 띄운다',
    '',
    '[APPLICATION]',
    'DOWN_LOAD_PROGRAM=WCS',
    'DOWN_LOAD_COMP=1     ; 올린 때와 견주어 바뀐 것만 받는다',
    'DOWN_LOAD_START=1    ; 띄우면 곧바로 시작',
    '',
    '[DOWN_FILE]          ; 받은 판 번호를 프로그램이 스스로 적는다 - 손대지 않는다',
])
note(d, '올리는 쪽 Config.ini 의 [DOWN_FILE] PATH 는 받은 파일을 놓을 하위 폴더다. '
        '점(.) 하나면 Client 폴더 바로 아래에 놓는다. ★비워 두면 안 된다★ - 드라이브 맨 위로 간다.')

d.add_heading('8.5 쓰는 순서', 2)
G.numbered(d, [
    'SQL/260929_UPDOWN_01_테이블생성.sql 을 한 번 돌린다.',
    'WmsUp.exe 를 띄우고 [업로드] 를 누른다.',
    '왼쪽 트리에서 Client 폴더로 가서 바뀐 파일을 고르고 [추가] 를 누른다.',
    '아래 「업데이트정보」 에 무엇을 고쳤는지 적는다. (나중에 이것만 보고도 알 수 있게)',
    '[UPLOAD] 를 누른다.',
    '각 운전 PC 는 바탕화면의 [LGLS CLIENT] 를 누르기만 하면 된다. '
    '바뀐 파일만 받고 이어서 Ecs.exe 가 뜬다.',
])

d.add_heading('8.6 검증한 것 (2026-09-30)', 2)
G.table(d, ['본 것', '결과'], [
    ['테이블 만들기', 'DN_MST / UP_DOWN / DN_SEQ 생성 확인'],
    ['올린 파일 내려받기', '5 개 파일이 그대로 내려왔다. 내용 대조(MD5) 전부 같음'],
    ['같은 판이면 건너뛰기', '일부러 망가뜨린 파일을 다시 받지 않았다 - 맞는 동작'],
    ['새 판이면 다시 받기', '판을 올리자 곧바로 다시 받아 제 내용으로 돌아왔다'],
    ['판 번호 기록', 'WmsDown.ini 의 [DOWN_FILE] 에 COMMON->파일명=판번호 가 적힌다'],
    ['올리는 쪽 화면', '조회 / 상세 / 수정내역 / 업로드 창까지 정상'],
], widths=[4.5, 11.5])
note(d, '올리기 화면에서 파일을 골라 [UPLOAD] 를 누르는 것까지는 사람이 눌러야 하는 자리라 '
        '화면이 뜨는 것까지만 확인했다. 넣는 SQL 은 내려받기로 왕복 검증했다.')

d.add_page_break()

d.add_heading('9. Client 설치 꾸러미 - 새 PC 에 깔 때', 1)
G.para(d, '적용 폴더의 Client_설치 를 통째로 새 PC 에 복사하고, '
          'Setup.bat 을 오른쪽 단추로 눌러 [관리자 권한으로 실행] 하면 된다.')

d.add_heading('9.1 들어 있는 것', 2)
G.table(d, ['것', '무엇'], [
    ['Client/', 'Ecs.exe 와 함께 쓰는 라이브러리, 화면 정의(xml), 다국어 문구(rc_resource), '
                '그리고 내려받기 프로그램(EcsClient.exe)과 설정'],
    ['Prerequisites/vc_redist.x86.exe', 'Visual C++ 재배포 패키지. Ecs.exe 가 mfc140u.dll 을 쓴다.'],
    ['Prerequisites/ndp48-web.exe', '.NET Framework 4.8. Windows 10 이상은 이미 있어 거의 건너뛴다.'],
    ['Setup.bat', '위 순서대로 깔고 바탕화면 바로가기를 만든다.'],
    ['읽어보세요.txt', '같은 내용의 짧은 안내.'],
], widths=[4.8, 11.2])

d.add_heading('9.2 MS-SQL 접속에 따로 깔 것은 없다', 2)
G.para(d, 'Client(Ecs.exe) 는 Windows 에 늘 들어 있는 ODBC 드라이버 "SQL Server" 로 붙고'
          '(Ecs.ini 의 [DB_2] DRIVER=SQL Server), '
          'EcsClient.exe 는 .NET 에 들어 있는 SqlClient 로 붙는다. '
          '둘 다 Windows 와 .NET 에 이미 들어 있는 것이라 따로 깔 것이 없다.')

d.add_heading('9.3 설정은 덮어쓰지 않는다', 2)
G.para(d, '이미 쓰던 Ecs.ini / WmsDown.ini 가 있으면 그대로 둔다. '
          '새 것은 Ecs.ini.new / WmsDown.ini.new 로 남기니 견주어 보고 필요한 줄만 옮기면 된다.')
note(d, '설치 뒤 두 곳의 DB 주소를 꼭 확인한다. '
        'Ecs.ini 의 [DB_2] SERVER, WmsDown.ini 의 [DB Server] SERVERNAME.')

out = os.path.join(HERE, '2026-09-29_적용안내서.docx')
d.save(out)
print('생성 :', out)
