# -*- coding: utf-8 -*-
"""PLC 제공 PPT 시나리오 슬라이드의 M비트 표기(Mxxx.y ↔ %MXnnn) 자체 정합성 + 주소맵 대조."""
import io, re, glob, sys, os
base = sys.argv[1]
pres = io.open(os.path.join(base,'ppt/presentation.xml'), encoding='utf-8').read()
rels = io.open(os.path.join(base,'ppt/_rels/presentation.xml.rels'), encoding='utf-8').read()
rmap = dict(re.findall(r'Id="([^"]+)"[^>]*Target="slides/(slide\d+\.xml)"', rels))
ids  = re.findall(r'<p:sldId[^>]*r:id="([^"]+)"', pres)

bad = []; ok = 0; tot_mx = 0
pairs = []
for i, rid in enumerate(ids, 1):
    f = os.path.join(base, 'ppt/slides', rmap[rid])
    s = io.open(f, encoding='utf-8').read()
    ts = re.findall(r'<a:t>([^<]*)</a:t>', s)
    j = " ".join(ts)
    # Mxxx.y 와 %MXnnn 이 인접해 나오는 쌍을 전부 추출
    for m in re.finditer(r'M(\d{3})\.(\d{1,2})\s*%MX(\d+)', j):
        w, b, mx = int(m.group(1)), int(m.group(2)), int(m.group(3))
        tot_mx += 1
        calc = w * 16 + b
        if calc == mx: ok += 1
        else: bad.append((i, m.group(0), calc, mx))
    # D0xxx → %DWnnn 쌍
    for m in re.finditer(r'D(\d{4})\s*(?:→|->)?\s*%DW(\d+)', j):
        pairs.append((i, 'D', m.group(1), int(m.group(2))))
    # R0xxx → %RBnnn 쌍
    for m in re.finditer(r'R(\d{4})\s*(?:~R\d{4})?\s*%RB(\d+)', j):
        pairs.append((i, 'R', m.group(1), int(m.group(2))))
print("[%s] M비트 표기쌍 %d개 : 일치 %d / 불일치 %d" % (os.path.basename(base), tot_mx, ok, len(bad)))
for b in bad[:15]: print("   슬라이드 %d : %s → 계산 %d ≠ 표기 %d" % b)
# D/R 진법 판정
dhex = dsum = rhex = rdec = 0
for sl, kind, doc, val in pairs:
    n = int(doc)
    if kind == 'D':
        dsum += 1
    else:
        if val == n * 2: rdec += 1
        try:
            if val == int(doc, 16) * 2: rhex += 1
        except: pass
print("   D 표기쌍 %d개 / R 표기쌍 : 10진해석 %d, 16진해석 %d" % (dsum, rdec, rhex))
