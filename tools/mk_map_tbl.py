# -*- coding: utf-8 -*-
"""PLC 원본(260818) 슬라이드 주소 ↔ 구 ECS 소스(파일·함수) 매칭 테이블 생성."""
import io, re, os, json, collections

BASE = 'plcorig'
ECSU = r"D:\project\LGLS\Renewal\Backup\ECS\Device\Unit"
TSV  = r"D:\project\LGLS\Renewal\SIM\EQP_SIM\observables.tsv"

# ── 구 ECS 소스 : tag → (Config 프로퍼티 파일, 핸들러 파일, 핸들러, 의미 함수) ──
def load_unit(cfg, unit):
    cfgs = io.open(os.path.join(ECSU, cfg), encoding='utf-8', errors='replace').read()
    unis = io.open(os.path.join(ECSU, unit), encoding='utf-8', errors='replace').read()
    prop = {}
    for m in re.finditer(r'public Observable @__IO_(\w+)(.*?)Observables\["(\w+)"\]', cfgs, re.S):
        prop[m.group(3)] = '@__IO_' + m.group(1)
    hand = {}
    for m in re.finditer(r'void __IO_(\w+)_ObservableValueChanged\([^)]*\)\s*\{\s*([^;}]*)', unis):
        body = ' '.join(m.group(2).split())
        fn = re.match(r'(\w+)\s*\(', body)
        hand[m.group(1)] = ('__IO_%s_ObservableValueChanged' % m.group(1),
                            (fn.group(1) + '()') if fn else '')
    return cfgs, unis, prop, hand

cvC, cvU, cvProp, cvHand = load_unit('ConveyorConfig.cs', 'Conveyor.cs')
vhC, vhU, vhProp, vhHand = load_unit('VehicleConfig.cs',  'Vehicle.cs')

# ── 구 ECS 관측 정의(주소) ──
ecs = collections.defaultdict(list); rng = []
for raw in io.open(TSV, encoding='utf-8', errors='replace'):
    c = raw.strip('\ufeff \r\n').split('\t')
    if len(c) < 6: continue
    owner, tag, cs = c[0].strip(), c[1].strip(), c[5].strip()
    dev = adr = None; ln = 1
    for pp in cs.split(','):
        if '=' not in pp: continue
        k, v = pp.split('=', 1); k = k.strip().upper(); v = v.strip()
        if k == 'DEVICE_TYPE': dev = {'B':'M','W':'D','R':'R'}.get(v)
        elif k == 'ADDRESS_NO': adr = v
        elif k == 'LENGTH':
            try: ln = int(v)
            except: pass
    if not dev or adr is None: continue
    try: a = int(adr, 16)
    except: continue
    ecs[(dev, a)].append((owner, tag, adr, ln))
    if ln > 1: rng.append((dev, a, ln, owner, tag, adr))

def find(dev, a):
    if (dev, a) in ecs: return ecs[(dev, a)][0], ''
    for d, st, l, o, t, ad in rng:
        if d == dev and st < a < st + l: return (o, t, ad, l), '%s 범위 +%d' % (t, a - st)
    return None, ''

# ── PLC 원본 슬라이드에서 주소 토큰 추출 ──
def dlabel(n):
    if 160 <= n <= 209: return 256 + ((n-160)//10)*16 + (n-160)%10
    if 210 <= n <= 219: return 336 + (n-210)
    if 300 <= n <= 315: return 480 + (n-300)
    if 320 <= n <= 369: return 768 + ((n-320)//10)*16 + (n-320)%10
    if 370 <= n <= 379: return 848 + (n-370)
    return None

SLIDE_TITLE = {}
order = io.open('slide_order.txt', encoding='utf-8').read().split()
rows = []
for i, f in enumerate(order, 1):
    txt = re.findall(r'<a:t>([^<]*)</a:t>', io.open(os.path.join(BASE,'ppt/slides',f), encoding='utf-8').read())
    j = " ".join(txt)
    # 슬라이드 제목만 뽑는다 (첫 의미 있는 문장 + 시나리오 구간 표기)
    cand = [t.strip() for t in txt if t.strip()]
    ttl = ''
    for t in cand:
        if '반송 시나리오' in t or 'Alarm' in t:
            ttl = t; break
    if not ttl and cand: ttl = cand[0]
    # 뒤따르는 괄호 구간(예 "( SC_2 출고 : CV103  CV104 )") 을 이어 붙인다
    for k, t in enumerate(cand):
        if t == ttl:
            tail = ''.join(cand[k+1:k+8])
            m2 = re.search(r'\(([^()]{4,60})\)', tail)
            if m2: ttl = ttl + ' (' + m2.group(1).strip() + ')'
            break
    # 알람 슬라이드는 대상 호기(S/C #n Alarm 보고)를 덧붙여 구분한다
    if 'Alarm' in ttl:
        m3 = re.search(r'(S/C\s*#\s*\d)', ''.join(cand))
        if m3: ttl = ttl + ' - ' + re.sub(r'\s+', '', m3.group(1))
    SLIDE_TITLE[i] = re.sub(r'\s+', ' ', ttl)[:60]
    for m in re.finditer(r'\bM(\d{3})([0-9A-F])\b', j):
        rows.append((i, 'M'+m.group(1)+m.group(2), 'M', int(m.group(1))*16+int(m.group(2),16)))
    for m in re.finditer(r'\bD(\d{4})\b', j):
        a = dlabel(int(m.group(1)))
        if a is not None: rows.append((i, 'D'+m.group(1), 'D', a))
    for m in re.finditer(r'\bR(\d{4})\b', j):
        rows.append((i, 'R'+m.group(1), 'R', int(m.group(1), 16)))

seen = set(); out = []
for sl, tok, dv, a in rows:
    if (tok, dv) in seen: continue
    seen.add((tok, dv))
    o, note = find(dv, a)
    if not o: continue
    owner, tag, adr, ln = o
    isveh = owner.startswith('VEHICLE')
    prop, hand = (vhProp, vhHand) if isveh else (cvProp, cvHand)
    cfgfile = 'VehicleConfig.cs' if isveh else 'ConveyorConfig.cs'
    unitfile = 'Vehicle.cs' if isveh else 'Conveyor.cs'
    h = hand.get(tag, ('', ''))
    # ECS 가 '쓰는' 값(Ack 등)은 관측 핸들러가 없다 → 쓰기 위치를 찾아 채운다
    if not h[0]:
        src = vhU if isveh else cvU
        wm = re.search(r'private void (\w+)\([^)]*\)\s*\{(?:(?!private void).)*?observables\["'
                       + re.escape(tag) + r'"\]\.Value', src, re.S)
        if wm: h = ('(ECS 쓰기)', wm.group(1) + '()')
    devtxt = {'M':'B(비트)','D':'W(워드)','R':'R(트래킹)'}[dv]
    real = ('%%MX%d' % a) if dv=='M' else (('%%DW%d' % a) if dv=='D' else ('%%RB%d' % (a*2)))
    out.append({
        'slide': sl, 'title': SLIDE_TITLE.get(sl,''), 'tok': tok, 'dev': devtxt,
        'real': real, 'addr': a, 'owner': owner, 'tag': tag, 'ecsaddr': adr,
        'cfgfile': cfgfile, 'prop': prop.get(tag,''),
        'unitfile': unitfile, 'handler': h[0], 'func': h[1], 'note': note })

io.open('map_tbl.json','w',encoding='utf-8').write(json.dumps(out, ensure_ascii=False, indent=1))
print('행:', len(out), ' / 핸들러 매칭:', sum(1 for r in out if r['handler']))
