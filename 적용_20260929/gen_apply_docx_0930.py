# -*- coding: utf-8 -*-
u"""2026-09-30 적용 안내서 - ★추가분만★ (Word).

   사용자 지시 : "이 질문 하기 전에 적용안내서를 출력했어..
                 저 질문 이후에 추가된 내용은 일자를 다르게 만들어서
                 추가된 부분만 문서를 만들어줘"

   앞선 안내서(2026-09-29)를 이미 출력하셨으므로, 그 뒤에 더해진 것만 담는다.
   앞 문서를 다시 볼 필요가 없게, 필요한 배경은 이 문서 안에 짧게 적는다.

   실행 : python gen_apply_docx_0930.py
"""
import os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, os.path.join(ROOT, 'docs', '산출물_20260927', 'gen'))

from docx.shared import Pt, Cm, RGBColor
import gen_common as G

NAVY, GRAY = G.NAVY, G.GRAY
RED = RGBColor(0xB0, 0x2A, 0x20)


def code(d, lines, size=9):
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


G.DATE = '2026-09-30'
G.VER = '1.0'
G.BASE_HISTORY = []
G.FINAL_NOTE = ('2026-09-29 적용 안내서에 이어지는 추가분. '
                '작업대 이름을 EcsDefine.xml 에서 가져오게 한 것과, '
                'IMS 스테이션 번호를 문서에 맞춰 바로잡은 것.')

d = G.new_doc('2026-09-30 적용 안내서 (추가분)',
              '작업대 명칭을 EcsDefine.xml 에서\n'
              'IMS 스테이션 번호 정정')

# ════════════════════════════════════════════════════════════════
d.add_heading('0. 이 문서는 무엇인가', 1)
G.para(d, '2026-09-29 적용 안내서를 이미 받아 보셨다면, 그 뒤에 더해진 것만 여기에 담았다. '
          '앞 문서를 다시 볼 필요가 없도록 필요한 배경은 짧게 함께 적는다.')
G.table(d, ['장', '무엇'], [
    ['1', '작업대 이름을 EcsDefine.xml 에서 가져온다 - 이름을 바꾸려고 프로그램을 다시 만들 일이 없어졌다'],
    ['2', 'IMS 스테이션 번호가 뒤집혀 있던 것을 문서에 맞춰 바로잡았다'],
    ['3', '적용 순서'],
], widths=[1.6, 14.4])
note(d, '앞 문서(2026-09-29)의 내용은 그대로 유효하다. 이 문서는 그 위에 얹는 것이다.')

# ════════════════════════════════════════════════════════════════
d.add_page_break()
d.add_heading('1. 작업대 이름을 EcsDefine.xml 에서 가져온다', 1)
G.para(d, '작업대 이름을 바꾸고 싶을 때 프로그램을 다시 만들지 않아도 되게 했다. '
          'EcsDefine.xml 의 이름을 고치면 작업 정보 창과 작업 판넬에 그대로 나온다.')

d.add_heading('1.1 어디를 고치나', 2)
code(d, [
    '<Track number="122">',
    '    <Status>',
    '        <StoStation name="외부 입고 전용 입출고대" id="101"/>',
    '        <RetStation name="외부 입고 전용 입출고대" id="101"/>',
    '    </Status>',
    '</Track>',
])
G.para(d, 'name 만 고치면 된다. 프로그램을 다시 만들 필요도, 다시 깔 필요도 없다. '
          'Client 를 다시 띄우면 바뀐 이름이 나온다.')

d.add_heading('1.2 어느 값이 어디서 오나', 2)
G.table(d, ['보이는 것', '어디서 오나'], [
    ['외부 입고 전용 입출고대', 'EcsDefine.xml 의 name'],
    ['[101]', 'DEST_POS_DEF.REMARKS 의 IMS 번호'],
    ['TR#22', 'DEST_POS_DEF.REMARKS 의 트랙번호'],
], widths=[5.5, 10.5])
note(d, '이어 주는 열쇠는 ★트랙번호★ 다. EcsDefine.xml 의 <Track number> 와 '
        'DEST_POS_DEF.MC_NO 가 같은 값이므로 그것으로 잇는다. '
        'EcsDefine.xml 에 없는 트랙이면 종전대로 REMARKS 의 이름을 쓴다 - 빈 이름이 되지 않는다.')

d.add_heading('1.3 무엇을 고쳤나', 2)
G.table(d, ['파일', '무엇'], [
    ['EcsDefine.cpp', 'Status 아래 요소의 이름만 보고 속성은 버리던 것을, name 도 함께 읽어 '
                      '트랙번호별로 모아 두게 했다.'],
    ['EcsDoc.h / EcsDoc.cpp', '출발지/도착지 이름표를 만들 때, 이름만 그 표에서 가져온다. '
                              'IMS 번호와 트랙번호는 종전대로 REMARKS 에서 온다.'],
    ['PanelJobDlg.cpp', '이름이 길어져 작업 판넬에서 잘렸다. 출발/도착 칸을 145 → 210 으로 넓히고 '
                        '작업번호 바로 뒤로 당겼다. 판넬이 좁아 뒤 칸은 화면 밖으로 밀리기 때문이다. '
                        '나머지 칸은 가로로 밀어 보면 된다.'],
], widths=[4.5, 11.5])
G.para(d, '작업 정보 창은 칸 너비가 내용에 맞게 스스로 늘어나므로 손대지 않았다.')

# ════════════════════════════════════════════════════════════════
d.add_page_break()
d.add_heading('2. IMS 스테이션 번호를 문서에 맞춰 바로잡았다', 1)

d.add_heading('2.1 무엇이 틀렸나', 2)
G.para(d, '입고와 출고의 IMS 번호가 서로 뒤집혀 있었다. 2026-09-22 에 넣은 값이다.')
G.table(d, ['트랙', '이름', '종전 (틀림)', '지금 (문서 기준)'], [
    ['124', '제품 입고대', 'IMS104', 'IMS103'],
    ['126', '원부자재 불출대', 'IMS103', 'IMS104'],
], widths=[2.0, 5.0, 4.5, 4.5])

d.add_heading('2.2 무엇이 맞나', 2)
G.para(d, 'IMS/프로토콜 문서의 Station Number 표가 기준이다.')
G.table(d, ['Station No', 'Station Name'], [
    ['001 ~ 003', '실온창고 S/C #1 ~ #3'],
    ['004 ~ 005', '냉장창고 S/C #1 ~ #2'],
    ['101', '외부 입고 전용 입출고대'],
    ['102', 'Picking 작업대'],
    ['103', '제품 입고대'],
    ['104', '원부자재 불출대'],
], widths=[4.0, 12.0])

d.add_heading('2.3 ★프로그램 동작에는 영향이 없었다★', 2)
G.para(d, '상위(WMS/IMS)와 주고받는 번호는 이 칸을 쓰지 않는다. '
          'WCS_TASK_HOST 의 modStationMap.cs 가 코드로 매핑하며, '
          '그쪽은 처음부터 문서와 같았다(103=제품 입고대 TR#24, 104=원부자재 불출대 TR#26).')
G.para(d, '틀렸던 곳은 화면에 보이는 이름표뿐이다.')
G.table(d, ['어디에 보이나', '언제부터'], [
    ['작업 정보 창 / 작업 판넬의 출발지·도착지', '2026-09-29'],
    ['수동 지시 창의 출발/도착 목록', '2026-09-16'],
    ['설비 상태창의 도착지', '〃'],
], widths=[9.0, 7.0])
note(d, '그래서 지금까지 상위와의 주고받기는 정상이었다. 눈에 보이는 번호만 달랐다.')

d.add_heading('2.4 무엇을 고쳤나', 2)
G.table(d, ['무엇', '어떻게'], [
    ['DEST_POS_DEF.REMARKS', 'SQL/260930_DEST_POS_DEF_01_IMS번호_정정.sql 로 값을 맞췄다. '
                             '문서의 이름이 길어 varchar(30) 에 들어가지 않아 60 으로 늘렸다 '
                             '(늘리는 것이라 되돌릴 일이 없다).'],
    ['EcsDefine.xml', '이름을 문서 표기와 같게 했다. '
                      '외부 전용 입출고대 → 외부 입고 전용 입출고대, 피킹 작업대 → Picking 작업대'],
    ['CLAUDE.md 의 트랙 표', '같은 곳이 뒤집혀 있어 함께 바로잡았다(작업 지침 파일).'],
], widths=[5.0, 11.0])

d.add_heading('2.5 고친 뒤 화면', 2)
code(d, [
    '작업번호   출발                              도착',
    '0854       Picking 작업대[102] TR#30         S/C #2[04-009-05]',
    '0853       제품 입고대[103] TR#24            S/C #1[02-007-10]',
    '0082       외부 입고 전용 입출고대[101] TR#22  S/C #4[07-001-01]',
])

# ════════════════════════════════════════════════════════════════
d.add_page_break()
d.add_heading('3. 적용 순서', 1)
G.numbered(d, [
    '현장 프로그램과 ini 를 백업한다.',
    'SQL/260930_DEST_POS_DEF_01_IMS번호_정정.sql 을 한 번 돌린다. '
    '(칸을 늘리고 값을 맞춘다. 몇 번을 돌려도 같은 결과가 된다.)',
    'Client 를 교체한다. 설치 꾸러미(Client_설치)를 쓰면 Setup.bat 하나로 끝난다. '
    '이미 깔려 있으면 EcsMain.exe 와 EcsDefine.xml 만 바꿔도 된다.',
    'Client 를 띄워 작업 정보 창과 작업 판넬의 출발지/도착지를 확인한다.',
])

d.add_heading('3.1 확인하면 좋은 것', 2)
G.table(d, ['항목', '무엇을 보나'], [
    ['작업대 이름', 'EcsDefine.xml 에 적은 이름이 그대로 보이는가'],
    ['IMS 번호', '제품 입고대가 [103], 원부자재 불출대가 [104] 로 보이는가'],
    ['트랙번호', 'TR#24 / TR#26 이 그대로 붙는가'],
    ['잘림', '작업 판넬에서 이름이 잘리지 않는가 (출발/도착이 작업번호 바로 뒤에 온다)'],
    ['수동 지시 창', '출발/도착 목록의 이름도 같은 말로 보이는가'],
], widths=[3.5, 12.5])

note(d, '이름을 더 바꾸고 싶으면 EcsDefine.xml 의 name 만 고치면 된다. '
        'IMS 번호나 트랙번호를 바꿀 일이 생기면 DEST_POS_DEF.REMARKS 를 고친다.')

out = os.path.join(HERE, '2026-09-30_적용안내서_추가분.docx')
d.save(out)
print('생성 :', out)
