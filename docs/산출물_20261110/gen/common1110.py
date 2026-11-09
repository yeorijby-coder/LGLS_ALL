# -*- coding: utf-8 -*-
"""2026-11-10 종료 산출물 공통 설정.
   docs/산출물_20260927/gen/gen_common.py 를 그대로 쓰되 출력·캡처 폴더와 날짜·판만 이 폴더 기준으로 바꾼다.
   사용 :  from common1110 import *      (G = gen_common 모듈, 나머지는 gen_common 의 함수)"""
import os, sys
HERE = os.path.dirname(os.path.abspath(__file__))              # docs/산출물_20261110/gen
BASE1110 = os.path.dirname(HERE)                                # docs/산출물_20261110
sys.path.insert(0, HERE)
sys.path.insert(0, r'D:\project\LGLS\Renewal\docs\산출물_20260927\gen')
import gen_common as G
from gen_common import (Document, Pt, Cm, RGBColor, WD_ALIGN_PARAGRAPH, qn, OxmlElement,
                        NAVY, GRAY, AUTHOR, new_doc, shade, table, para, bullets, numbered, image, read_cp949)

G.BASE = BASE1110
G.OUT = BASE1110
G.SHOTS = os.path.join(BASE1110, 'shots')
G.SCRATCH = HERE
G.DATE = '2026-11-10'
G.VER = '3.0'
G.BASE_HISTORY = []
G.FINAL_NOTE = '2026-11-10 현장 적용 기준 종료 산출물'
OUT = G.OUT
SHOTS = G.SHOTS
RED = RGBColor(0xC0, 0x00, 0x00)
BLUE = RGBColor(0x1F, 0x4E, 0x9A)
SITE = 'LG 화학 1동 자동창고'


def shot(name):
    return os.path.join(G.SHOTS, name)


def note(d, text, color=None):
    """회색 작은 글씨 주석 한 줄"""
    return para(d, '※ ' + text, size=9, color=color or GRAY)


def warn(d, text):
    return para(d, '★ ' + text, bold=True, color=RED)


def code(d, lines, font='Consolas', size=8.5):
    """명령·설정 발췌를 회색 상자 하나로"""
    t = d.add_table(rows=1, cols=1); t.style = 'Table Grid'
    c = t.rows[0].cells[0]; shade(c, 'F2F2F2'); c.text = ''
    for k, ln in enumerate(lines):
        p = c.paragraphs[0] if k == 0 else c.add_paragraph()
        r = p.add_run(ln); r.font.name = font; r.font.size = Pt(size)
    d.add_paragraph()
    return t


def doc_set_table(d, this):
    """네 문서의 관계표. this = 이 문서 번호(1~4)"""
    rows = [
        ['01 ECS 운영자 매뉴얼', '설치 · 기동/정지 · 서버 프로그램 · 설정 · DB 운영 · 배포 · 장애 대응', '서버/시스템을 관리하는 사람'],
        ['02 ECS 사용자 매뉴얼', '운전 화면(Ecs)을 쓰는 순서, 자주 하는 일, 이상 대처', '운전 화면을 쓰는 작업자'],
        ['03 테이블 정의서', 'LGLS_MCS_IO 의 모든 표와 열, 트리거 · 인덱스 · 보존 정책', 'DB 를 조회·관리하는 사람'],
        ['04 인터페이스 정의서', '상위(WMS/IMS) 전문, 설비(PLC) 신호, 프로그램 사이 DB 인터페이스', '연동 담당자 · 설비/상위 담당자'],
    ]
    for i, r in enumerate(rows, 1):
        if i == this:
            r[0] = r[0] + '  (이 문서)'
    table(d, ['문서', '무엇을 담았나', '누가 보나'], rows, widths=[4.5, 8.0, 4.5])


def save_doc(d, name):
    """OUT 에 저장한다. 사용자가 열어 둔 채(PermissionError)면 _MMDD재생성.docx 로 낸다."""
    import datetime
    out = os.path.join(G.OUT, name)
    try:
        d.save(out)
    except PermissionError:
        out = os.path.join(G.OUT, name[:-5] + '_%s재생성.docx' % datetime.date.today().strftime('%m%d'))
        d.save(out)
    print('saved', out)
    return out
    # "2026-09-27 판 산출물 ... 다시 묶은 것" 주석 줄은 사용자가 지웠다(11/9, 본문 설명에 날짜 금지)
