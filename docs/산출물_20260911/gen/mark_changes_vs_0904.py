# -*- coding: utf-8 -*-
# [LGLS 2026-09-10] 화면설계서 : 0904 판에서 바뀐 부분을 붉은색으로 표시
#   · 슬라이드는 제목으로 짝을 맞춘다
#   · 0904 에 없던 문단(= 새로 쓰였거나 문구가 바뀐 문단)만 붉게 칠한다
#   · 0904 에 아예 없던 슬라이드는 제목을 붉게 칠하고 [신규] 를 붙인다
#   먼저 전체를 검정으로 되돌린 뒤 칠하므로, 여러 번 돌려도 결과가 같다.
import os, shutil
from pptx import Presentation
from pptx.dml.color import RGBColor

OLD = r"D:\project\LGLS\Renewal\docs\산출물_20260904\06_WCS_화면설계서.pptx"
NEW = r"D:\project\LGLS\Renewal\docs\산출물_20260911\06_ECS_화면설계서.pptx"

RED = RGBColor(0xC0, 0x00, 0x00)
BLACK = RGBColor(0x00, 0x00, 0x00)

SKIP_TITLES = ('Document history', 'Contents')     # 목차/이력은 손대지 않는다

# 모든 장에 똑같이 들어가는 것(머리말/쪽번호)은 표시하지 않는다.
#   WCS -> ECS 이름 변경 때문에 전 장이 붉어져 정작 바뀐 내용이 묻힌다.
def is_boilerplate(t):
    if t.isdigit():
        return True
    if '화면설계서' in t and '|' in t:
        return True
    return False


def para_text(p):
    return ''.join(r.text for r in p.runs).strip()


def slide_title(s):
    for sh in s.shapes:
        if sh.has_text_frame and sh.text_frame.text.strip():
            return sh.text_frame.text.strip().split('\n')[0].strip()
    return ''


def slide_paras(s):
    out = set()
    for sh in s.shapes:
        if not sh.has_text_frame:
            continue
        for p in sh.text_frame.paragraphs:
            t = para_text(p)
            if t:
                out.add(t)
    return out


old = Presentation(OLD)
oldmap = {}
for s in old.slides:
    oldmap.setdefault(slide_title(s), set()).update(slide_paras(s))

# 0904 전체 문단(제목 매칭이 어긋난 슬라이드 대비용 안전망)
oldall = set()
for v in oldmap.values():
    oldall |= v

if not os.path.exists(NEW + '.bak_mark'):
    shutil.copy(NEW, NEW + '.bak_mark')

new = Presentation(NEW)
nred = 0
nnew_slide = 0
for i, s in enumerate(new.slides):
    title = slide_title(s)
    if title in SKIP_TITLES:
        continue
    base = oldmap.get(title)
    is_new_slide = base is None

    for sh in s.shapes:
        if not sh.has_text_frame:
            continue
        for p in sh.text_frame.paragraphs:
            t = para_text(p)
            if not t:
                continue
            # 매번 같은 결과가 나오게 먼저 검정으로 되돌린다(머리말/쪽번호 포함)
            for r in p.runs:
                r.font.color.rgb = BLACK
            if is_boilerplate(t):
                continue
            if is_new_slide:
                changed = True
            else:
                changed = (t not in base) and (t not in oldall)
            if changed:
                for r in p.runs:
                    r.font.color.rgb = RED
                nred += 1
    if is_new_slide:
        nnew_slide += 1
        print('  [신규 슬라이드] %2d %s' % (i, title))

new.save(NEW)
print('붉게 칠한 문단 %d / 신규 슬라이드 %d' % (nred, nnew_slide))
