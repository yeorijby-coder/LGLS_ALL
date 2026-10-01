# -*- coding: utf-8 -*-
u"""2026-10-01 적용 안내서 - ★추가분만★ (Word).

   사용자 지시 : "이 질문 하기 전에 적용안내서를 출력했어..
                 저 질문 이후에 추가된 내용은 일자를 다르게 만들어서
                 추가된 부분만 문서를 만들어줘"

   앞선 안내서(2026-09-29)를 이미 출력하셨으므로, 그 뒤에 더해진 것만 담는다.
   앞 문서를 다시 볼 필요가 없게, 필요한 배경은 이 문서 안에 짧게 적는다.

   실행 : python gen_apply_docx_1001.py
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


G.DATE = '2026-10-01'
G.VER = '1.0'
G.BASE_HISTORY = []
G.FINAL_NOTE = ('2026-09-30 적용 안내서(추가분)에 이어지는 추가분. 리본 신호등을 다른 리본 단추와 같은 방식으로 그리는 선택지.')

d = G.new_doc('2026-10-01 적용 안내서 (추가분)',
              '리본 신호등을 리본 단추 방식으로 (LAMP_STYLE) · 로그오프 없이 설치')

d.add_heading('0. 이 문서는 무엇인가', 1)
G.para(d, '2026-09-30 적용 안내서(추가분, 1~9절)를 이미 받아 보셨다면, 그 뒤에 더해진 것만 여기에 담았다. '
          '파일은 모두 적용_20260930 폴더에 있다(폴더는 그대로 쓴다).')
note(d, '앞 문서들(09-29, 09-30)의 내용은 그대로 유효하다. 이 문서는 그 위에 얹는 것이다.')

d.add_heading('1. 리본 신호등을 다른 리본 단추처럼 그리는 선택지', 1)
G.para(d, '질문 : "신호등을 리본과 표처럼(Windows 가 키우는 방식으로) 바꾸는 것은 안 되나?" - 된다. '
          '리본의 다른 단추들은 32픽셀 아이콘 + 글자를 MFC 가 그리고, Windows 배율만큼 아이콘과 글자를 키운다. '
          '신호등을 그 길로 보내면 현장에서 이미 정상으로 보이던 다른 단추들과 똑같이 그려진다.')
G.para(d, '지금까지의 신호등(사진 같은 모양)은 우리가 직접 그리므로 배율을 우리가 맞춰야 했다. 09-30 안내서 8절에서 그 계산을 고쳤지만 '
          '실제 300% 화면에서 돌려 보지는 못했다. 그래서 두 방식을 모두 두고 설정으로 고른다.')

d.add_heading('1.1 설정 (Ecs.ini [DISPLAY])', 2)
code(d, [
    '[DISPLAY]',
    'LAMP_STYLE = 0   ; 0 프로그램이 직접 그린다(기본, 사진 같은 모양, UI_SCALE 을 따른다)',
    '                 ; 1 다른 리본 단추와 같은 아이콘 단추 - Windows 배율과 리본 글꼴을 그대로 따른다',
])
G.table(d, ['값', '모양', '배율', '어울리는 때'], [
    ['0', '사진 같은 삼색 신호등 + 이름', 'UI_SCALE(자동이면 Windows 배율)', '1920x1080, 4K 모니터링 TV(Windows 100%)'],
    ['1', '리본 단추(아이콘 32px + 이름)', 'Windows 배율 그대로 (리본의 다른 단추와 같다)', '4K@300% 처럼 0 이 어긋나 보일 때'],
], widths=[1.2, 5.0, 5.0, 4.8])
G.para(d, '1 로 바꾸면 다시 띄워야 반영된다. 아이콘은 rc_resource\mainframe_config\lamp_red/yellow/green.png 이며, '
          '없으면 0 으로 돈다. 상태 표시(빨강 = 끊김, 노랑<->초록 1초 교대 = 정상)와 눌렀을 때의 핑/포트 확인은 두 방식이 같다.')

d.add_heading('1.2 확인한 것', 2)
G.table(d, ['구분', '내용'], [
    ['확인', '1920x1080@100% 에서 LAMP_STYLE=1 이 다른 리본 단추와 같은 크기·글꼴로 그려지고, UI_SCALE 을 200 으로 올려도 신호등은 리본 단추 크기를 지킨다(범례만 커진다).'],
    ['확인', '0 과 1 모두 상태 교대(노랑<->초록)와 끊김(빨강)이 같은 판정으로 바뀐다.'],
    ['못 함', '실제 Windows 배율 300% 화면에서는 돌려 보지 못했다. 다른 리본 단추가 그 화면에서 정상이므로 1 은 같은 길을 타지만, 현장에서 눈으로 확인해 달라.'],
], widths=[2.0, 14.0])

d.add_heading('1.3 현장에서 고르는 순서', 2)
G.numbered(d, [
    '새 Client 를 깔고 그대로(LAMP_STYLE=0, UI_SCALE=0) 띄운다. 신호등이 잘리지 않으면 끝.',
    '어긋나 보이면 Ecs.ini [DISPLAY] LAMP_STYLE = 1 로 바꾸고 다시 띄운다.',
    '그래도 지도·범례가 어긋나면 09-30 안내서 8.3 의 표(UI_SCALE / LAYOUT_FONT_SCALE), 그 다음 8.4(화면배율_방식.bat).',
])

d.add_heading('2. 바뀐 파일', 1)
G.table(d, ['폴더', '파일', '비고'], [
    ['Client', 'EcsMain.exe', 'LAMP_STYLE 포함 (09-30 의 4K 대응도 들어 있다)'],
    ['Client/rc_resource/mainframe_config', 'lamp_red.png, lamp_yellow.png, lamp_green.png', '아이콘 방식용 그림 3장 - ★같이 복사★'],
    ['Client', '추가할_설정_Ecs.ini.txt', '[DISPLAY] 에 LAMP_STYLE 추가'],
    ['Client_설치', '(꾸러미 전체)', '위 내용이 들어간 새 꾸러미'],
], widths=[4.5, 6.5, 5.0])

d.add_heading('3. 로그오프 없이 설치하기 (폰트 잠금)', 1)
G.para(d, '09-30 안내서 대로면 옛 Client 를 돌린 적이 있는 PC 는 폰트 파일이 잠겨 로그오프가 필요했다. '
          '현장 PC 는 로그오프할 수 없다고 하여, 설치 프로그램이 스스로 피해 가게 했다.')
G.table(d, ['경우', '설치 프로그램이 하는 일'], [
    ['옛 Client 가 깔린 폴더에 그대로 덮어쓴다 (제거하지 않았다)', '폰트 파일은 잠겨 있어도 ★같은 파일★ 이라 건너뛰고 나머지를 복사한다. 그대로 쓴다.'],
    ['제거(Uninstall)한 뒤라 지우다 만 폰트가 남아 있다', '그 폴더는 로그오프 전까지 못 쓴다. ★옆 이름(C:\LGLS\CLIENT_1, _2 ...)★ 에 깔고 바로가기도 거기로 만든다. 옛 폴더는 나중에 로그오프한 뒤 지운다.'],
    ['처음 까는 PC', '종전과 같다.'],
], widths=[6.0, 10.0])
G.para(d, '즉 현장에서는 제거 프로그램을 돌리지 말고 바로 Setup.bat 으로 덮어쓰는 것이 가장 간단하다. '
          '이미 제거한 PC 라도 Setup.bat 이 옆 폴더를 골라 주므로 그냥 진행하면 된다. '
          '설치가 끝나면 새 Client 는 폰트를 자기 프로세스에만 등록하므로 다음부터는 이 문제가 없다.')
note(d, '이 PC 에서 폰트를 일부러 잠가 두고 확인했다 : 잠긴 폴더를 알아내 _1 로 바꾸고, 잠금을 풀면 원래 폴더를 쓴다. '
        '실제 "지우다 만" 상태는 이 PC 에서 이미 풀려 재현하지 못했다.')

out = os.path.join(HERE, '2026-10-01_적용안내서_추가분.docx')
d.save(out)
print('생성 :', out)
