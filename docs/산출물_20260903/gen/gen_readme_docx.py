# -*- coding: utf-8 -*-
"""EXE/README.md -> 워드 (설치·구동 안내서)"""
import re, os
from gen_common import *

md = open(r'D:\project\LGLS\Renewal\EXE\README.md', encoding='utf-8').read()
d = new_doc('설치 및 구동 안내서', 'EXE 실행본 / 서버 · 키오스크 구동 요건')

def flush_table(d, rows):
    if not rows: return
    hdr = rows[0]; body = [r for r in rows[1:] if not all(re.fullmatch(r'-+', c.strip() or '-') for c in r)]
    table(d, hdr, body, font=9)

lines = md.splitlines(); i = 0; tbl = []; code = []; in_code = False
while i < len(lines):
    ln = lines[i]
    if ln.strip().startswith('```'):
        if in_code:
            t = d.add_table(rows=1, cols=1); t.style = 'Table Grid'; c = t.rows[0].cells[0]; shade(c, 'F2F2F2'); c.text = ''
            for k, cl in enumerate(code):
                p = c.paragraphs[0] if k == 0 else c.add_paragraph()
                r = p.add_run(cl); r.font.name = 'Consolas'; r.font.size = Pt(8.5)
            d.add_paragraph(); code = []; in_code = False
        else: in_code = True
        i += 1; continue
    if in_code: code.append(ln); i += 1; continue
    if ln.startswith('|'):
        cells = [c.strip() for c in ln.strip().strip('|').split('|')]
        tbl.append(cells); i += 1; continue
    else:
        flush_table(d, tbl); tbl = []
    if ln.startswith('# '):
        pass  # 문서 제목은 표지에서 대체
    elif ln.startswith('## '): d.add_heading(ln[3:].strip(), 1)
    elif ln.startswith('### '): d.add_heading(ln[4:].strip(), 2)
    elif re.match(r'^\s*[-*] ', ln):
        txt = re.sub(r'^\s*[-*] ', '', ln); txt = re.sub(r'\*\*(.+?)\*\*', r'\1', txt).replace('`', '')
        bullets(d, [txt])
    elif re.match(r'^\s*\d+\. ', ln):
        txt = re.sub(r'^\s*\d+\. ', '', ln); txt = re.sub(r'\*\*(.+?)\*\*', r'\1', txt).replace('`', '')
        numbered(d, [txt])
    elif ln.strip():
        txt = re.sub(r'\*\*(.+?)\*\*', r'\1', ln).replace('`', '')
        para(d, txt)
    i += 1
flush_table(d, tbl)

# 부록 : 현재 INI 표시 상태
d.add_page_break(); d.add_heading('부록. 현재 설정 파일의 화면 표시 상태 (%s 기준)' % DATE, 1)
ms = menu_states()
names = {'USER_MENU': "ECS > 환경설정 > 사용자", 'SEMITEST_MENU': "MANUAL > 반자동 TEST 그룹", 'INI_MENU': "ECS > 환경설정 > INI 열기",
         'UIMODE_MENU': "ECS > UI모드 그룹(대화상자/판넬)", 'EMPTYPLT_MENU': "ECS > 뷰 > 공PLT작업", 'PRODINFO_MENU': "ECS > 창고 모니터링 > 제품정보"}
rows = [[k, names.get(k, ''), '표시' if v else '숨김'] for k, v in ms.items()]
table(d, ['Ecs.ini [MENU] 키', '대상', '현재'], rows, widths=[4.5, 8, 3])
rows2 = [[k, '표시' if v else '숨김'] for k, v in task_view_states().items()]
table(d, ['TASK [VIEW] LOG_FILTER_BTN', '현재'], rows2, widths=[8, 3])
out = os.path.join(OUT, '01_설치_및_구동_안내서.docx'); d.save(out); print('saved', out)
