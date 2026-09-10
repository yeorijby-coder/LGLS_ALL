# -*- coding: utf-8 -*-
# [LGLS 2026-09-10] 화면설계서에 "판넬 3개를 모두 올린 화면" 슬라이드를 하나 더 넣는다.
#   9쪽(판넬 보기)을 통째로 복제해 글과 그림만 바꾸고, 9쪽 바로 뒤로 옮긴다.
import os, copy, shutil
from pptx import Presentation
from pptx.util import Emu
from pptx.text.text import _Paragraph
from PIL import Image

PPTX = r"D:\project\LGLS\Renewal\docs\산출물_20260909\06_ECS_화면설계서.pptx"
IMG = r"D:\project\LGLS\Renewal\docs\산출물_20260909\shots\panels_all.png"
SRC = 9                       # 복제 원본 (판넬 보기)

TITLE = 'ECS>판넬 보기 - 판넬 3개를 모두 올린 화면'
PATH = 'ECS ▷ 판넬 보기  (작업정보 + 상세정보 + 설비반송)'
DESC = ('세 판넬을 모두 올렸을 때의 화면 구성. 위쪽 전폭이 작업정보, 오른쪽 위가 상세정보, '
        '오른쪽 아래가 설비반송, 남은 왼쪽 아래가 창고 모니터링이다.')
NOTES = [
    '· 작업정보 판넬은 열이 많아 화면 상단에 가로로 넓게 붙는다. 오른쪽 위 [자동 갱신] 으로 2초 주기 갱신을 켜고 끈다.',
    '· 상세정보 판넬은 메인 화면에서 클릭한 설비를 따라간다. CV / SC / RTV / 작업정보 탭으로 나뉜다.',
    '· 설비반송 판넬은 크레인 5대와 RGV 의 반송 현황을 한 줄씩 보여 준다.',
    '· 판넬은 보기 전용이다. 조작은 설비 대화상자에서 한다.',
    '· 판넬을 모두 끄면 창고 모니터링이 화면 전체를 쓴다.',
]

if not os.path.exists(PPTX + '.bak_addslide'):
    shutil.copy(PPTX, PPTX + '.bak_addslide')

prs = Presentation(PPTX)
src = prs.slides[SRC]

# 1) 같은 레이아웃으로 새 슬라이드 + 원본 도형 전부 복제
new = prs.slides.add_slide(src.slide_layout)
for sh in list(new.shapes):
    sh._element.getparent().remove(sh._element)
for sh in src.shapes:
    new.shapes._spTree.append(copy.deepcopy(sh._element))

# 2) 9쪽 바로 뒤로 옮긴다
sldIdLst = prs.slides._sldIdLst
ids = list(sldIdLst)
moved = ids[-1]
sldIdLst.remove(moved)
sldIdLst.insert(SRC + 1, moved)


def para_text(p):
    return ''.join(r.text for r in p.runs).strip()


def set_para(p, s):
    if not p.runs:
        return
    p.runs[0].text = s
    for r in p.runs[1:]:
        r.text = ''


# 3) 글 바꾸기
tgt = prs.slides[SRC + 1]
body = None
for sh in tgt.shapes:
    if not sh.has_text_frame:
        continue
    paras = sh.text_frame.paragraphs
    for i, p in enumerate(paras):
        t = para_text(p)
        if not t:
            continue
        if t.startswith('ECS>판넬 보기'):
            set_para(p, TITLE)
        elif t.startswith('ECS ▷ 판넬 보기'):
            set_para(p, PATH)
        elif t.startswith('작업 목록·설비 상세'):
            set_para(p, DESC)
        elif t.startswith('·'):
            body = (sh, p)

# 특기사항 : 기존 · 줄을 지우고 새로 쓴다
if body:
    sh, last = body
    olds = [p for p in sh.text_frame.paragraphs if para_text(p).startswith('·')]
    first = olds[0]
    set_para(first, NOTES[0])
    prev = first
    for line in NOTES[1:]:
        npp = copy.deepcopy(first._p)
        prev._p.addnext(npp)
        p2 = _Paragraph(npp, first._parent)
        set_para(p2, line)
        prev = p2
    for p in olds[1:]:
        p._p.getparent().remove(p._p)

# 4) 그림 교체
pics = [sh for sh in tgt.shapes if sh.__class__.__name__ == 'Picture']
if pics:
    cur = max(pics, key=lambda s: s.width * s.height)
    L, T, W, H = cur.left, cur.top, cur.width, cur.height
    cur._element.getparent().remove(cur._element)
    iw, ih = Image.open(IMG).size
    sc = min(W / float(iw), H / float(ih))
    nw, nh = int(iw * sc), int(ih * sc)
    tgt.shapes.add_picture(IMG, Emu(L + (W - nw) // 2), Emu(T + (H - nh) // 2), Emu(nw), Emu(nh))

prs.save(PPTX)
print('슬라이드 %d 뒤에 추가 (전체 %d쪽)' % (SRC, len(prs.slides._sldIdLst)))
