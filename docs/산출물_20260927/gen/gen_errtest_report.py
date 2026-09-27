# -*- coding: utf-8 -*-
"""이중입고(54) / 공출고(58) 반복 시험 보고서 (docx)

2.0 (2026-09-21) : 시험 본문(09-04/05, 당시 SFA 코드표 54/58)은 역사 기록으로 그대로 두고,
앞쪽에 코드표 개정 안내 절과 문서 이력을 붙였다. 결과 파일은 이 폴더의 err_test_result.txt.
"""
import os, re, sys
from docx import Document
from docx.shared import Pt, Cm, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_TABLE_ALIGNMENT
from docx.oxml.ns import qn
from docx.oxml import OxmlElement

OUT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RESULT = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'err_test_result.txt')
NAVY = RGBColor(0x1E, 0x27, 0x61)
GRAY = RGBColor(0x66, 0x66, 0x66)
RED = RGBColor(0xC0, 0x00, 0x00)
TEST_DATE = '2026-09-05'   # 시험 수행일 (본문 역사 기록)
DATE = '2026-09-21'        # 문서 작성일 (2.0 판)
VER = '2.0'


def shade(cell, hex_):
    tcPr = cell._tc.get_or_add_tcPr()
    sh = OxmlElement('w:shd'); sh.set(qn('w:val'), 'clear')
    sh.set(qn('w:color'), 'auto'); sh.set(qn('w:fill'), hex_)
    tcPr.append(sh)


def table(d, headers, rows, widths=None, font=9):
    t = d.add_table(rows=1, cols=len(headers)); t.style = 'Table Grid'
    t.alignment = WD_TABLE_ALIGNMENT.CENTER
    for i, h in enumerate(headers):
        c = t.rows[0].cells[i]; c.text = ''
        r = c.paragraphs[0].add_run(str(h)); r.bold = True
        r.font.size = Pt(font); r.font.color.rgb = RGBColor(0xFF, 0xFF, 0xFF)
        c.paragraphs[0].alignment = WD_ALIGN_PARAGRAPH.CENTER; shade(c, '1E2761')
    for row in rows:
        cells = t.add_row().cells
        for i, v in enumerate(row):
            cells[i].text = ''
            r = cells[i].paragraphs[0].add_run('' if v is None else str(v))
            r.font.size = Pt(font)
    if widths:
        for row in t.rows:
            for i, w in enumerate(widths):
                row.cells[i].width = Cm(w)
    d.add_paragraph()
    return t


def para(d, text, bold=False, size=10, color=None, indent=0):
    p = d.add_paragraph()
    if indent: p.paragraph_format.left_indent = Cm(indent)
    r = p.add_run(text); r.bold = bold; r.font.size = Pt(size)
    if color: r.font.color.rgb = color
    return p


def bullets(d, items, indent=0.4):
    for it in items:
        p = d.add_paragraph(style='List Bullet')
        p.paragraph_format.left_indent = Cm(indent + 0.6)
        r = p.add_run(it); r.font.size = Pt(10)


# ── 결과 파일 파싱 ───────────────────────────────────────────
txt = open(RESULT, encoding='utf-8-sig').read()
rounds = {'DOUBLE': [], 'EMPTY': []}
cur = None
for ln in txt.splitlines():
    if '이중입고(54) 시험' in ln: cur = 'DOUBLE'; continue
    if '공출고(58) 시험' in ln: cur = 'EMPTY'; continue
    m = re.search(r'#(\d+)\s+(OK|NG)\s*:\s*(.+)$', ln)
    if m and cur:
        rounds[cur].append((int(m.group(1)), m.group(2), m.group(3).strip(),
                            ln.split()[0]))
okD = sum(1 for r in rounds['DOUBLE'] if r[1] == 'OK')
okE = sum(1 for r in rounds['EMPTY'] if r[1] == 'OK')
ngD = len(rounds['DOUBLE']) - okD
ngE = len(rounds['EMPTY']) - okE

# ── 문서 ─────────────────────────────────────────────────────
d = Document()
st = d.styles['Normal']; st.font.name = '맑은 고딕'; st.font.size = Pt(10)
st.element.rPr.rFonts.set(qn('w:eastAsia'), '맑은 고딕')
for lv, sz in ((1, 15), (2, 12)):
    h = d.styles['Heading %d' % lv]; h.font.name = '맑은 고딕'
    h.font.size = Pt(sz); h.font.bold = True; h.font.color.rgb = NAVY
    h.element.rPr.rFonts.set(qn('w:eastAsia'), '맑은 고딕')
sec = d.sections[0]
sec.left_margin = sec.right_margin = Cm(2.0)
sec.top_margin = sec.bottom_margin = Cm(1.8)

p = d.add_paragraph(); p.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = p.add_run('LG 화학 1동 자동창고'); r.font.size = Pt(12); r.font.color.rgb = GRAY
p = d.add_paragraph(); p.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = p.add_run('이중입고 · 공출고 반복 시험 보고서'); r.font.size = Pt(20)
r.bold = True; r.font.color.rgb = NAVY
p = d.add_paragraph(); p.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = p.add_run('시험일 %s   ·   작성일 %s   ·   Ver %s   ·   WCS Renewal' % (TEST_DATE, DATE, VER))
r.font.size = Pt(10); r.font.color.rgb = GRAY
d.add_paragraph()

d.add_heading('문서 이력', 1)
table(d, ['버전', '일자', '작성', '내용'], [
    ['1.0', '2026-09-05', 'LGLS ECS Renewal', '최초 시험 보고 (이중입고 54 / 공출고 58, SFA 코드표 기준)'],
    [VER, DATE, 'LGLS ECS Renewal', '코드표 개정 안내 추가 (PLC 알람 리스트 260917, 이 현장 이중입고/공출고 처리 미사용)'],
], widths=[1.5, 2.5, 3.5, 9.3])

# ── 2.0 개정 안내 (본문 앞) ─────────────────────────────────────
d.add_heading('0. 2.0 개정 안내', 1)
para(d, '이 보고서의 1~5장은 2026-09-04/05 시험 당시의 기록이다. 그때의 크레인 코드표(SFA)로 이중입고 = 54, '
        '공출고 = 58 을 썼고, 기록 보존을 위해 본문은 고치지 않았다. 그 뒤 바뀐 사항은 다음과 같다.', bold=True, color=RED)
d.add_heading('0.1 이 현장은 이중입고/공출고 처리를 쓰지 않음 (2026-09-16 사용자 확인)', 2)
bullets(d, [
    '스케줄러(IO_TASK) : 주기 처리의 MarkErrorJobStatus()(에러 작업상태 표시) · ResumeRedirectedJobs()(재지정 재개 지시) '
    '호출을 주석 처리했다. 함수 정의는 보존했다.',
    '시뮬레이터(EQP_SIM) : 이중입고 · 공출고 주입 체크박스를 숨기고 핸들러를 무력화했으며, VehicleSim 의 주입 분기를 '
    '끈 상태(false &&)로 두었다.',
    '따라서 현행 프로그램으로는 이 보고서의 시험을 그대로 재현할 수 없다.',
])
d.add_heading('0.2 크레인 코드표 변경 (2026-09-17 PLC 알람 리스트 적용)', 2)
table(d, ['구분', '종전 (이 보고서 본문)', '현행'], [
    ['코드표 구분 (EQP_ECD_MST.EQP_TYP)', 'SC_SFA', 'SC_LGLS (PLC 알람 리스트 260917, 지상반 코드 11~93, 101~118)'],
    ['이중입고', '54 (SFA 표는 54, 55)', '73 = 좌측 렉 / 74 = 우측 렉'],
    ['공출고', '58 (SFA 표는 58, 59)', '75'],
    ['종전 코드의 새 뜻', '-', '54 = 주행 후진 시간초과 등 다른 알람. 54/58 로 판정하면 오판한다'],
], widths=[5.0, 4.5, 7.3])
para(d, '그래서 판정 코드를 프로그램에 고정하지 않고 INI 로 뺐다 (상세는 12_INI_설정_정의서).')
table(d, ['프로그램', 'INI 위치', '현행 값', '용도'], [
    ['스케줄러', 'ENV_IOSCH.INI [CNF] SC_DUAL_CODES / SC_EMPTY_CODES', '73,74 / 75',
     '현재는 재지정 지시 허용 판정(UpdateScData)에만 쓰임'],
    ['상위 통신', 'EcsComA.ini [Host] ScErrCodeType / ScDualCodes / ScEmptyCodes', 'SC_LGLS / 73,74 / 75',
     'E 전문 ErrorKind 1(이중입고) · 3(공출고) 판정. SC_SFA 로 되돌리면 54,55 / 58,59'],
    ['운전 화면', 'Ecs.ini [SC_ERR] ERR_TYP / DUAL_CODES / EMPTY_CODES', 'SC_LGLS / 73,74 / 75',
     '상태창 알람 문구, 에러해제 · 삭제 확인창'],
    ['설비 통신', 'WCS_DB.INI [CNF] SC_ERR_TYP', 'SC_LGLS', '설비 에러이력(EQP_ERR_HIS) 코드표 구분'],
], widths=[2.3, 6.5, 3.2, 4.8], font=8.5)
d.add_heading('0.3 처리를 되살릴 때 할 일', 2)
bullets(d, [
    '현장에서 이중입고 · 공출고 처리가 필요한지와 실 크레인 코드(73/74/75)를 다시 확인한다.',
    'IO_TASK cThread_SCH.cs 의 MarkErrorJobStatus() · ResumeRedirectedJobs() 호출 주석을 푼다. '
    '함수 안의 판정이 SC_DUAL_CODES / SC_EMPTY_CODES 를 쓰는지(54/58 고정값이 남지 않았는지) 확인한다.',
    '세 프로그램의 판정 코드(위 표)를 같은 값으로 맞춘다. 코드표 구분(SC_LGLS)도 네 곳이 같아야 한다.',
    '설비 통신 [CNF] SC_ERR_CODE_BLOCK : 종전 "ErrCode 블록(D1160+10k) 읽기" 용도는 폐기됐다. 래치 계열(2 또는 3)에서 '
    '이중입고 · 공출고 코드가 ERR_CODE_RD 에 실리고 재지정 지시 뒤 해제되는지 확인한다.',
    'EQP_SIM 주입 기능을 되살린다(체크박스 표시, 핸들러 복원, VehicleSim 의 false && 제거). 주입 코드는 이미 73 / 75 로 바뀌어 있다.',
    '상위(HOST_SIM / WMS)의 재지정(R 전문) · 작업 삭제 절차가 새 판정과 맞는지 확인하고, 이 보고서와 같은 방법으로 각 12회 이상 재시험한다.',
])
d.add_page_break()

d.add_heading('1. 시험 목적과 방법', 1)
para(d, '크레인이 이중입고(에러 54) 또는 공출고(에러 58)를 낸 뒤, 정해진 절차로 작업이 정상으로 '
        '돌아오는지 반복 확인한다. 두 에러의 회복 절차는 서로 다르다.')
table(d, ['구분', '이중입고 (에러 54)', '공출고 (에러 58)'], [
    ['에러 주입', 'EQP_SIM [이중입고 에러] 체크박스', 'EQP_SIM [공출고 에러] 체크박스'],
    ['에러 보고', 'WCS → E 전문 (ErrorKind=1)', 'WCS → E 전문 (ErrorKind=3)'],
    ['상위 처리', 'HOST_SIM [이중입고 재지정] → R 전문(R_Kind=1)로 다른 셀 지정',
     'HOST_SIM [공출고 작업 삭제] — 재지정이 없다'],
    ['WCS 처리', '스케줄러가 새 셀로 크레인 재지시', '운전자가 [작업정보] 에서 작업 삭제'],
    ['에러 해제', '새 지시를 받으면서 설비가 해제', 'EQP_SIM [설비 에러 해제] (현장 조작반)'],
    ['성공 판정', '크레인 에러코드가 해제되고 순환이 이어지면 성공', '좌동'],
    ['반복 횟수', '12 회', '12 회'],
], widths=[2.6, 7.2, 7.0])
para(d, '※ 운전 화면의 작업 삭제와 설비 조작반의 에러 해제는 사람이 하는 조작이다. 반복 시험에서는 '
        '운전 화면이 실행하는 것과 같은 삭제를 스크립트가 대행하고, 설비 에러 해제는 EQP_SIM 의 '
        '[설비 에러 해제] 버튼을 눌러 대행했다.', size=9, color=GRAY)

d.add_heading('2. 시험 결과', 1)
table(d, ['시험', '성공', '실패', '판정'], [
    ['이중입고 (에러 54)', okD, ngD, '합격' if ngD == 0 and okD >= 10 else '조건부'],
    ['공출고 (에러 58)', okE, ngE, '합격' if ngE == 0 and okE >= 10 else '조건부'],
], widths=[6.0, 2.5, 2.5, 5.7], font=10)

for key, title in (('DOUBLE', '2.1 이중입고 (에러 54)'), ('EMPTY', '2.2 공출고 (에러 58)')):
    d.add_heading(title, 2)
    rows = [[n, '성공' if st_ == 'OK' else '실패', tm, msg] for n, st_, msg, tm in rounds[key]]
    table(d, ['회차', '결과', '시각', '내용'], rows, widths=[1.6, 2.0, 2.6, 10.6])

d.add_heading('3. 시험 중 발견해 수정한 결함', 1)
para(d, '첫 시행에서는 두 시험 모두 전부 실패했다. 원인을 추적해 다음 여섯 가지를 고쳤다.')
table(d, ['No', '증상', '원인', '조치'], [
    ['1', '재지정을 보내도 작업이 에러 상태에 머문다',
     '스케줄러가 매 주기 에러 상태(08/07)로 되돌려, 재지정 상태(06/05)가 유지되지 않았다',
     'MarkErrorJobStatus 가 재지정 상태를 덮어쓰지 않도록 제외 조건 추가'],
    ['2', '재지정 상태가 되어도 크레인이 움직이지 않는다',
     '지시 기록이 "에러 없을 때만" 조건에 막혔다. 크레인 에러는 새 지시를 받아야 풀리므로 서로 물린다',
     '재지정 지시는 이중입고/공출고 에러가 남아 있어도 기록하도록 예외 허용'],
    ['3', '출고 재지정이 항상 실패',
     '존재하지 않는 테이블(LUGG_MST)과 컬럼(LUGGNO)을 갱신하고 있었다',
     'JOB_MST / LUGG_NO 로 정정'],
    ['4', '상위가 재지정을 보내면 "그런 작업 없음"으로 반려',
     '에러보고(E)의 작업번호를 LUGG_NO_FK1_RD 에서 읽는데, 실경로에서는 채워지지 않아 낡은 값이 올라갔다',
     '실린 화물(PALLET_ON_VEHICLE_RD → ITN_LUGG_FK1 → LUGG_NO_FK1_OD) 기준으로 정정'],
    ['5', '재지정이 "동일 호기가 아님"으로 반려',
     '에러보고의 셀이 크레인 현재 위치라 작업의 목적 셀과 달랐다',
     '이중입고는 작업의 도착 셀, 공출고는 출발 셀을 보고하도록 정정'],
    ['6', '공출고 재지정이 "기존과 같은 위치"로 반려',
     '새 셀을 전문의 도착 필드에 실었으나, 출고 재지정은 출발 필드가 바뀌어야 한다',
     'HOST_SIM 이 새 출발 셀을 START 필드에 싣도록 정정. 아울러 종전에는 R 전문 자체를 보내지 않았다'],
    ['7', '공출고를 삭제 방식으로 바꾸자 크레인 에러가 영영 남는다',
     '재지정이 없어 새 지시가 들어가지 않는데, 스케줄러는 에러난 크레인에 지시하지 않는다. '
     '에러를 풀 수단이 어디에도 없었다',
     '현장에서 사람이 조작반에서 푸는 동작을 EQP_SIM [설비 에러 해제] 버튼으로 신설. '
     'TASK · Client 에는 자동 해제를 두지 않는다'],
], widths=[1.0, 4.4, 6.0, 5.4], font=8.5)
para(d, '※ 1~6 은 재지정 경로를 만들며 고친 것이고, 7 은 공출고를 삭제 방식으로 바꾸면서 드러난 것이다.',
     size=9, color=GRAY)

d.add_heading('4. 정상 복구 흐름 (확인된 순서)', 1)
para(d, '이중입고', bold=True)
bullets(d, [
    '크레인이 목적 셀에 넣지 못하고 에러 54 발생 (화물은 실은 채)',
    'WCS 가 E 전문 보고 (ErrorKind=1, 작업번호와 도착 셀 포함)',
    '상위가 같은 호기의 다른 셀로 R 전문(R_Kind=1) 재지정',
    'WCS 가 작업 상태를 06(이중입고 재지정)으로 바꾸고, 스케줄러가 새 셀로 크레인 재지시',
    '크레인이 새 지시를 받으면서 에러 해제 → 정상 입고 완료',
])
para(d, '공출고 — 현장 운용은 재지정이 아니라 삭제다', bold=True)
bullets(d, [
    '크레인이 출발 셀에서 집지 못하고 에러 58 발생 (빈 포크)',
    'WCS 가 E 전문 보고 (ErrorKind=3, 작업번호와 출발 셀 포함)',
    '상위(IMS)가 그 작업을 삭제한다 (담당자 확인 후)',
    '운전자가 WCS 운전 화면 [작업정보] 에서 같은 작업을 삭제한다 (확인 대화상자 후)',
    '운전자가 설비 조작반에서 크레인 에러를 해제한다 (재지정이 없으므로 이 조작이 반드시 필요하다)',
    '재고가 실제로 있는 셀로 상위가 새 출고를 지시한다',
])
para(d, '※ WCS 의 R_Kind=2(출고 재지정) 처리는 규격 지원으로 남겨 두었다. 상위가 보내지 않으면 동작하지 않는다.',
     size=9, color=GRAY)

d.add_heading('5. 현장 확인이 필요한 사항', 1)
bullets(d, [
    '설비 에러 해제 : 구 ECS 에도 S/C · RGV 에러를 해제하는 화면 기능은 없다. 설비에서 알람이 해제되면 '
    '설비가 AlarmResetReport 로 알리고 ECS 는 Ack 만 한다. 현장에서 크레인 에러를 누가 어떻게 푸는지 확인이 필요하다. '
    '(EQP_SIM 은 새 지시를 받으면 해제하도록 맞춰 두었다.)',
    '이중입고 재지정에서 실 크레인이 이미 든 화물을 다시 집지 않고 새 셀로 바로 옮기는지.',
    '재지정 전문의 셀 규약(동일 호기 안에서만 허용)이 현장 WMS 와 일치하는지.',
])
out = os.path.join(OUT, '08_이중입고_공출고_시험_보고서.docx')
d.save(out)
print('saved', out, '| DOUBLE %d/%d  EMPTY %d/%d' % (okD, okD + ngD, okE, okE + ngE))
