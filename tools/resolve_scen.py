# -*- coding: utf-8 -*-
"""PlcAddressMap.xml -> 시나리오 6종 x 호기 1~5 주소 해석 (PPT V1.4 생성용 데이터)
   C# cPlcAddrMap.cs 와 동일한 계산식을 쓴다.
     base = origin + (no - numberFrom) * stride
     addr = base + offset (+ slot*perSlot)
     R 은 문서표기를 16진 해석(구 ECS 호환, GM_R_ADDR_HEX 기본값 true)
"""
import io, json, sys
import xml.etree.ElementTree as ET

XML = r"D:\project\LGLS\Renewal\TASK\WCS_TASK_CV_BIN\7_DeviceMap\PlcAddressMap.xml"

def rword(doc):
    try:    return int(str(doc), 16)
    except: return doc

root = ET.parse(XML).getroot()

groups = {}
for g in root.findall('EquipGroup'):
    gd = {'numberFrom': int(g.get('numberFrom', '1')), 'blocks': {}}
    for b in g.findall('Block'):
        bd = {'device': b.get('device'), 'origin': int(b.get('origin')),
              'stride': int(b.get('stride')), 'perSlotWords': int(b.get('perSlotWords', '1')),
              'signals': {}}
        for s in list(b):
            nm = s.get('name')
            bd['signals'][nm] = {'offset': int(s.get('offset')),
                                 'isArray': (s.tag == 'SignalArray'),
                                 'perSlot': int(s.get('perSlot', '1'))}
        gd['blocks'][b.get('name')] = bd
    groups[g.get('type')] = gd

gbits = {}
gl = root.find('Global')
if gl is not None:
    for s in gl.iter():
        if s.get('name') and s.get('bit'):
            gbits[s.get('name')] = int(s.get('bit'))

cranes = {}
for c in root.findall('.//Crane'):
    cranes[int(c.get('no'))] = {k: (int(c.get(k)) if c.get(k) and c.get(k).isdigit() else c.get(k))
                                for k in c.keys()}

def addr(equip, no, block, signal, slot):
    g = groups.get(equip)
    if not g: return -1
    b = g['blocks'].get(block)
    if not b: return -1
    s = b['signals'].get(signal)
    if s is None: return -1
    if b['device'] == 'R':
        doc = b['origin'] + (no - g['numberFrom']) * b['stride'] + s['offset']
        if slot > 0: doc += slot * b['perSlotWords']
        return rword(doc)
    a = b['origin'] + (no - g['numberFrom']) * b['stride'] + s['offset']
    if slot > 0: a += slot * (s['perSlot'] if s['isArray'] else 1)
    return a

def resolve_no(expr, crane, default):
    if not expr: return default
    e = expr.strip()
    if not e.startswith('$'):
        try: return int(e)
        except: return default
    if e == '$': return crane
    c = cranes.get(crane)
    if not c: return default
    if e.lower() == '$incv':  return c['inCv']
    if e.lower() == '$outcv': return c['outCv']
    return default

def dev_of(equip, block):
    g = groups.get(equip)
    if not g: return 'M'
    b = g['blocks'].get(block)
    return b['device'] if b else 'M'

out = {'cranes': cranes, 'scenarios': []}
for sc in root.findall('.//Scenario'):
    sd = {'id': sc.get('id'), 'title': sc.get('title'),
          'craneSelect': (sc.get('crane') == 'select'), 'steps': []}
    for st in sc.findall('Step'):
        noExpr = st.get('no')
        rec = {'kind': st.get('kind', 'observe'), 'equip': st.get('equip'),
               'block': st.get('block'), 'signal': st.get('signal'),
               'desc': st.get('desc') or '', 'value': st.get('value') or '',
               'per': {}}
        slot = int(st.get('slot', '-1'))
        for cr in range(1, 6):
            no = resolve_no(noExpr, cr, 0)
            if st.get('equip', '').lower() == 'global':
                a = gbits.get(st.get('signal'), -1); dev = 'M'
            else:
                a = addr(st.get('equip'), no, st.get('block'), st.get('signal'), slot)
                dev = dev_of(st.get('equip'), st.get('block'))
            rec['per'][str(cr)] = {'no': no, 'addr': a, 'dev': dev}
        sd['steps'].append(rec)
    out['scenarios'].append(sd)

io.open(sys.argv[1] if len(sys.argv) > 1 else 'scen.json', 'w', encoding='utf-8').write(
    json.dumps(out, ensure_ascii=False, indent=1))
print("scenarios:", len(out['scenarios']),
      "steps:", sum(len(s['steps']) for s in out['scenarios']))
