# -*- coding: utf-8 -*-
"""PPT 시나리오 슬라이드의 %MX/%DW 주소가 주소맵(XML) 계산 집합에 있는지 전수 확인."""
import io, re, os, sys, xml.etree.ElementTree as ET
XML = r"D:\project\LGLS\Renewal\TASK\WCS_TASK_CV_BIN\7_DeviceMap\PlcAddressMap.xml"
root = ET.parse(XML).getroot()
MX, DW = set(), set()
for g in root.findall('EquipGroup'):
    typ = g.get('type'); fr = int(g.get('numberFrom','1')); cnt = int(g.get('count'))
    for b in g.findall('Block'):
        dev = b.get('device'); org = int(b.get('origin')); std = int(b.get('stride'))
        for n in range(fr, fr+cnt):
            base = org + (n-fr)*std
            (MX if dev=="M" else DW if dev=="D" else set()).add(base)
            for s in list(b):
                ofs = int(s.get('offset')); slots = int(s.get('maxSlots','1')) if s.tag=='SignalArray' else 1
                ps = int(s.get('perSlot','1')); words = int(s.get('words','1'))
                for k in range(max(slots, words)):
                    a = base + ofs + k*(ps if s.tag=='SignalArray' else 1)
                    (MX if dev=='M' else DW if dev=='D' else set()).add(a)
gl = root.find('Global')
if gl is not None:
    for s in list(gl):
        if s.get('addr'): MX.add(int(s.get('addr')))

def audit(base):
    pres = io.open(os.path.join(base,'ppt/presentation.xml'),encoding='utf-8').read()
    rels = io.open(os.path.join(base,'ppt/_rels/presentation.xml.rels'),encoding='utf-8').read()
    rmap = dict(re.findall(r'Id="([^"]+)"[^>]*Target="slides/(slide\d+\.xml)"',rels))
    ids  = re.findall(r'<p:sldId[^>]*r:id="([^"]+)"',pres)
    okm=badm=okd=badd=0; miss=[]
    for i,rid in enumerate(ids,1):
        s = io.open(os.path.join(base,'ppt/slides',rmap[rid]),encoding='utf-8').read()
        j = " ".join(re.findall(r'<a:t>([^<]*)</a:t>', s))
        for m in re.finditer(r'%MX(\d+)', j):
            a=int(m.group(1))
            if a in MX: okm+=1
            else: badm+=1; miss.append((i,'%MX'+m.group(1)))
        for m in re.finditer(r'%DW(\d+)', j):
            a=int(m.group(1))
            if a in DW: okd+=1
            else: badd+=1; miss.append((i,'%DW'+m.group(1)))
    print("[%s] %%MX %d개(주소맵에 있음 %d / 없음 %d) · %%DW %d개(있음 %d / 없음 %d)"
          % (os.path.basename(base), okm+badm, okm, badm, okd+badd, okd, badd))
    seen=set()
    for sl,tok in miss:
        if tok in seen: continue
        seen.add(tok); print("   미정의: 슬라이드 %d  %s" % (sl,tok))
    return badm+badd

t=0
for b in ('pptaud/v11','pptaud/v14'): t+=audit(b)
print("\n총 주소맵 밖 토큰:", t)
