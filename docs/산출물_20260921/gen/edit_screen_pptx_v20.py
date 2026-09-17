# -*- coding: utf-8 -*-
"""06_ECS_화면설계서.pptx (사용자 수정본) 제자리 수정 - V2.0 (2026-09-21 최종판).
   사용자가 손본 09-17 본을 복사해 와서 글자 조각·그림만 바꾼다. 새로 만들지 않는다.
   사용: python edit_screen_pptx_v20.py [빌드문자열]"""
import os, sys, copy
from pptx import Presentation
from pptx.util import Emu

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PPT = os.path.join(BASE, '06_ECS_화면설계서.pptx')
SHOTS = os.path.join(BASE, 'shots')
BUILD = sys.argv[1] if len(sys.argv) > 1 else ''

prs = Presentation(PPT)


def replace_in_runs(shape, old, new):
    n = 0
    if not shape.has_text_frame:
        return 0
    for p in shape.text_frame.paragraphs:
        for r in p.runs:
            if old in r.text:
                r.text = r.text.replace(old, new); n += 1
    return n


def add_para(shape, text):
    """마지막 문단의 서식을 복사해 문단을 하나 덧붙인다."""
    tf = shape.text_frame
    last = tf.paragraphs[-1]._p
    newp = copy.deepcopy(last)
    last.addnext(newp)
    p = tf.paragraphs[-1]
    for r in p.runs[1:]:
        r._r.getparent().remove(r._r)
    p.runs[0].text = text


def replace_picture(slide, png):
    """슬라이드에서 가장 큰 그림을 같은 자리·같은 폭으로 바꾼다(높이는 비율 유지, 원래 높이를 넘으면 높이에 맞춤)."""
    if not os.path.exists(png):
        print('  그림 없음', png); return
    pics = [s for s in slide.shapes if s.shape_type == 13]
    if not pics:
        return
    old = max(pics, key=lambda s: s.width * s.height)
    from PIL import Image
    w, h = Image.open(png).size
    L, T, W, H = old.left, old.top, old.width, old.height
    nw, nh = W, int(W * h / w)
    if nh > H:
        nh, nw = H, int(H * w / h)
    sp = old._element
    slide.shapes._spTree.remove(sp)
    pic = slide.shapes.add_picture(png, L + (W - nw) // 2, T, nw, nh)
    print('  그림 교체', os.path.basename(png))


# 1) 모든 장 : 버전 표기
cnt = 0
for s in prs.slides:
    for sh in s.shapes:
        cnt += replace_in_runs(sh, 'Ver 1.2', 'Ver 2.0')
print('Ver 표기', cnt)

# 2) 표지
for sh in prs.slides[0].shapes:
    replace_in_runs(sh, '2026-09-17', '2026-09-21')
    if BUILD and sh.has_text_frame and 'Build 2026.09.17 08:18:34' in sh.text_frame.text:
        replace_in_runs(sh, 'Build 2026.09.17 08:18:34', 'Build ' + BUILD)

# 3) 이력 표에 V2.0 행
for sh in prs.slides[1].shapes:
    if sh.has_table:
        tbl = sh.table
        if tbl.rows[len(tbl.rows) - 1].cells[0].text != 'V2.0':
            tr = copy.deepcopy(tbl._tbl.tr_lst[-1])
            tbl._tbl.append(tr)
            vals = ['V2.0', '2026-09-21', 'LGLS ECS Renewal', '',
                    '최종판 : 기본 시험 완료 기준. 크레인·RGV 상태창 알람 문구(PLC 알람 리스트 2026-09-17), '
                    '설비에러이력 SC 조회에 코드표 구분(SC_LGLS 등) 포함, 미사용 설정 정리, 캡처 갱신']
            row = tbl.rows[len(tbl.rows) - 1]
            for c, v in zip(row.cells, vals):
                tf = c.text_frame
                for p in tf.paragraphs[1:]:
                    p._p.getparent().remove(p._p)
                rs = tf.paragraphs[0].runs
                if rs:
                    rs[0].text = v
                    for r in rs[1:]:
                        r._r.getparent().remove(r._r)
                else:
                    tf.paragraphs[0].text = v
            # 표 높이가 슬라이드를 넘지 않게 행 높이를 이전 행과 같게
            tbl.rows[len(tbl.rows) - 1].height = tbl.rows[len(tbl.rows) - 2].height

# 4) 설비에러이력 / SC / RTV 특기사항
def body(slide):
    for sh in slide.shapes:
        if sh.has_text_frame and '메뉴 Path' in sh.text_frame.text:
            return sh

b = body(prs.slides[20])
if b is not None and '코드표' not in b.text_frame.text:
    add_para(b, '')
    add_para(b, '■ 특기사항')
    add_para(b, '· 에러 문구는 에러코드 마스터(EQP_ECD_MST)에서 설비·코드표 구분으로 찾는다. 크레인은 PLC 알람 리스트 코드표(SC_LGLS).')
    add_para(b, '· 설비구분 SC 를 고르면 SC_LGLS 등 크레인 코드표 구분으로 쌓인 이력도 함께 조회된다.')

b = body(prs.slides[25])
if b is not None and '알람 문구' not in b.text_frame.text:
    add_para(b, '· 에러 칸은 "코드 + 알람 문구"로 표시한다 (예 0073 좌측 렉 이중입고). 코드표 = Ecs.ini [SC_ERR] ERR_TYP(SC_LGLS).')
    add_para(b, '· 에러 해제 시 이중입고(73·74)·공출고(75) 확인창은 Ecs.ini [SC_ERR] DUAL_CODES / EMPTY_CODES 기준.')

b = body(prs.slides[26])
if b is not None and '알람 문구' not in b.text_frame.text:
    add_para(b, '· 진단 칸은 "코드 + 알람 문구"로 표시한다 (예 0033 RGV 전진 한계점, PLC 알람 비트 M5601~M560F).')

# 5) 캡처 교체 (shots 에 새로 찍은 것이 있을 때만)
#    09-21 에 새로 찍은 것은 *_v20.png 로 둔다. 없으면 그 장은 09-17 그림 그대로.
for idx, name in ((3, 'main_full_v20.png'), (25, 'sc_0_v20.png'), (26, 'rtv_0_v20.png')):
    p = os.path.join(SHOTS, name)
    if os.path.exists(p):
        replace_picture(prs.slides[idx], p)

prs.save(PPT)
print('saved', PPT, len(prs.slides))
