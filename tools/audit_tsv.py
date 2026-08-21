# -*- coding: utf-8 -*-
"""구 ECS(TB_OBSERVABLE=observables.tsv) ↔ PLC 시나리오 영역(PlcAddressMap.xml=PPT 메모리맵) 전수 대조."""
import io, re, sys, xml.etree.ElementTree as ET

XML = r"D:\project\LGLS\Renewal\TASK\WCS_TASK_CV_BIN\7_DeviceMap\PlcAddressMap.xml"
TSV = r"D:\project\LGLS\Renewal\SIM\EQP_SIM\observables.tsv"

root = ET.parse(XML).getroot()
RHEX = (root.get('rAddrMode', 'HEX').upper() not in ('DEC', '10'))

groups = {}
for g in root.findall('EquipGroup'):
    gd = {'from': int(g.get('numberFrom', '1')), 'blocks': {}}
    for b in g.findall('Block'):
        bd = {'dev': b.get('device'), 'origin': int(b.get('origin')), 'stride': int(b.get('stride')),
              'perSlot': int(b.get('perSlotWords', '1')), 'sig': {}}
        for s in list(b):
            bd['sig'][s.get('name')] = {'ofs': int(s.get('offset')), 'tag': s.get('tag') or '',
                                        'arr': s.tag == 'SignalArray', 'ps': int(s.get('perSlot', '1')),
                                        'words': int(s.get('words', '1'))}
        gd['blocks'][b.get('name')] = bd
    groups[g.get('type')] = gd

def rword(d):
    if not RHEX: return d
    try: return int(str(d), 16)
    except: return d

def addr(typ, no, blk, sig, slot=-1):
    g = groups.get(typ);  b = g['blocks'].get(blk) if g else None
    s = b['sig'].get(sig) if b else None
    if s is None: return -1
    if b['dev'] == 'R':
        d = b['origin'] + (no - g['from']) * b['stride'] + s['ofs']
        if slot > 0: d += slot * b['perSlot']
        return rword(d)
    a = b['origin'] + (no - g['from']) * b['stride'] + s['ofs']
    if slot > 0: a += slot * (s['ps'] if s['arr'] else 1)
    return a

# CV 태그명(구 ECS) ↔ XML Signal 매핑
CV_EVT = {"UNLOAD_COMPLETE_01":"UnloadComplete1","LOAD_COMPLETE_01":"LoadComplete1",
          "UNLOAD_COMPLETE_02":"UnloadComplete2","LOAD_COMPLETE_02":"LoadComplete2",
          "WAIT_IN":"WorkInstruction","WAIT_OUT":"WorkOrder","IN_READY_02":"InReady2",
          "OPERATION_MODE":"OperationMode","UNLOAD_REQUEST_ACK_02":"UnloadRequest2Ack"}
CV_ACK = {"UNLOAD_COMPLETE_ACK_01":"UnloadComplete1Ack","LOAD_COMPLETE_ACK_01":"LoadComplete1Ack",
          "UNLOAD_COMPLETE_ACK_02":"UnloadComplete2Ack","LOAD_COMPLETE_ACK_02":"LoadComplete2Ack",
          "UNLOAD_REQUEST_02":"UnloadRequest2"}

def expect_cv(no, tag):
    if tag in CV_EVT: return addr("CV", no, "Event", CV_EVT[tag])
    if tag in CV_ACK: return addr("CV", no, "Ack", CV_ACK[tag])
    if tag == "DIRECTION_MODE": return addr("CV", no, "Direction", "IoDirection")
    m = re.match(r"PALLET_EXIST_FLAG_0(\d)$", tag)
    if m: return addr("CV", no, "Event", "PalletExist", int(m.group(1)) - 1)
    m = re.match(r"PALLET_EXIST0(\d)$", tag)
    if m: return addr("CV", no, "Tracking", "JobNo", int(m.group(1)) - 1)
    return None

def expect_veh(typ, no, tag):
    g = groups.get(typ)
    for bn, b in g['blocks'].items():
        for sn, s in b['sig'].items():
            if s['tag'] and s['tag'] == tag:
                return addr(typ, no, bn, sn)
            # _01 시리즈 전개 (words>1)
            if s['tag'] and s['tag'].endswith('_01') and s['words'] > 1:
                base = s['tag'][:-3]
                m = re.match(re.escape(base) + r"_(\d\d)$", tag)
                if m:
                    k = int(m.group(1)) - 1
                    if 0 <= k < s['words']:
                        return addr(typ, no, bn, sn) + k
    return None

rows = []
for raw in io.open(TSV, encoding='utf-8', errors='replace'):
    line = raw.strip('\ufeff \r\n')
    if not line: continue
    c = line.split('\t')
    if len(c) < 6: continue
    owner, tag, cs = c[0].strip(), c[1].strip(), c[5].strip()
    dev = None; adr = None; ln = 1
    for p in cs.split(','):
        if '=' not in p: continue
        k, v = p.split('=', 1); k = k.strip().upper(); v = v.strip()
        if k == 'DEVICE_TYPE': dev = {'B':'M','W':'D','R':'R'}.get(v, '?')
        elif k == 'ADDRESS_NO': adr = v
        elif k == 'LENGTH':
            try: ln = int(v)
            except: pass
    if dev in (None, '?') or adr is None: continue
    if dev == 'R' and not RHEX:
        try: a = int(adr)
        except: continue
    else:
        try: a = int(adr, 16)
        except: continue
    rows.append((owner, tag, dev, a, ln))

print("구 ECS observables.tsv 행 :", len(rows))
stat = {'match':0, 'diff':0, 'nodef':0}
diffs = []; nodef = []
for owner, tag, dev, a, ln in rows:
    m = re.match(r"CONVEYOR:(\d+)$", owner)
    if m:
        exp = expect_cv(int(m.group(1)), tag)
    else:
        m2 = re.match(r"VEHICLE:(\d+)$", owner)
        if not m2: continue
        n = int(m2.group(1))
        exp = expect_veh("RGV", 1, tag) if n == 1 else expect_veh("SC", n - 10, tag)
    if exp is None or exp < 0:
        stat['nodef'] += 1; nodef.append((owner, tag, dev, a)); continue
    if exp == a: stat['match'] += 1
    else:
        stat['diff'] += 1; diffs.append((owner, tag, dev, a, exp))

print("일치 :", stat['match'], " / 불일치 :", stat['diff'], " / XML 미정의 :", stat['nodef'])
if diffs:
    print("\n[불일치 상세]")
    for d in diffs[:40]:
        print("   %-14s %-26s %s  구ECS=%-6s  PLC맵=%s" % d)
if nodef:
    print("\n[XML 미정의 태그] %d건 (구 ECS 에만 있음)" % len(nodef))
    seen = {}
    for o, t, dv, a in nodef: seen.setdefault(t, []).append(o)
    for t in sorted(seen): print("   %-26s %s" % (t, ("%d개 설비" % len(seen[t]))))
