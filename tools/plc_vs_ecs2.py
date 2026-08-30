# -*- coding: utf-8 -*-
"""PLC 제공 원본(260818) ↔ 구 ECS 소스(observables.tsv) 전수 대조 (D 라벨 규칙 반영)."""
import io, re, os, collections
BASE='plcorig'; TSV=r"D:\project\LGLS\Renewal\SIM\EQP_SIM\observables.tsv"

ecs = collections.defaultdict(list)
rng = []            # (dev, start, len, owner, tag) : LENGTH>1 범위
for raw in io.open(TSV, encoding='utf-8', errors='replace'):
    c = raw.strip('\ufeff \r\n').split('\t')
    if len(c) < 6: continue
    owner, tag, cs = c[0].strip(), c[1].strip(), c[5].strip()
    dev=adr=None; ln=1
    for p in cs.split(','):
        if '=' not in p: continue
        k,v = p.split('=',1); k=k.strip().upper(); v=v.strip()
        if k=='DEVICE_TYPE': dev={'B':'M','W':'D','R':'R'}.get(v)
        elif k=='ADDRESS_NO': adr=v
        elif k=='LENGTH':
            try: ln=int(v)
            except: pass
    if not dev or adr is None: continue
    try: a=int(adr,16)
    except: continue
    ecs[(dev,a)].append((owner,tag))
    if ln>1: rng.append((dev,a,ln,owner,tag))

def find(dev,a):
    if (dev,a) in ecs: return ecs[(dev,a)][0], "일치"
    for d,s,l,o,t in rng:
        if d==dev and s < a < s+l:
            return (o,t), "일치(%s 범위 +%d)" % (t, a-s)
    return None, None

# D 문서라벨 → 실주소 (PPT/구 ECS 공통 규칙)
def dlabel(n):
    if 160 <= n <= 209:  return 256 + ((n-160)//10)*16 + (n-160)%10, "S/C #%d Status+%d" % ((n-160)//10+1, (n-160)%10)
    if 210 <= n <= 219:  return 336 + (n-210), "RGV Status+%d" % (n-210)
    if 300 <= n <= 315:  return 480 + (n-300), "C/V #%d Direction" % (n-300+1)
    if 320 <= n <= 369:  return 768 + ((n-320)//10)*16 + (n-320)%10, "S/C #%d Command+%d" % ((n-320)//10+1, (n-320)%10)
    if 370 <= n <= 379:  return 848 + (n-370), "RGV Command+%d" % (n-370)
    return None, None

order = io.open('slide_order.txt', encoding='utf-8').read().split()
rows=[]
for i,f in enumerate(order,1):
    j = " ".join(re.findall(r'<a:t>([^<]*)</a:t>', io.open(os.path.join(BASE,'ppt/slides',f),encoding='utf-8').read()))
    for m in re.finditer(r'\bM(\d{3})([0-9A-F])\b', j):
        rows.append((i,'M'+m.group(1)+m.group(2),'M',int(m.group(1))*16+int(m.group(2),16),''))
    for m in re.finditer(r'\bD(\d{4})\b', j):
        a,d = dlabel(int(m.group(1)))
        if a is not None: rows.append((i,'D'+m.group(1),'D',a,d))
    for m in re.finditer(r'\bR(\d{4})\b', j):
        rows.append((i,'R'+m.group(1),'R',int(m.group(1),16),''))

seen=set(); uniq=[]
for r in rows:
    if (r[1],r[2]) in seen: continue
    seen.add((r[1],r[2])); uniq.append(r)

hit=0; miss=[]
lines=[]
for sl,tok,dv,a,note in uniq:
    o,how = find(dv,a)
    if o:
        hit+=1
        lines.append("%-3d %-7s %-5d %-30s %s" % (sl,tok,a,o[0]+"."+o[1],how))
    else:
        miss.append((sl,tok,dv,a,note))
print("PLC 원본 주소 토큰 : 총 %d개 (고유 %d개)" % (len(rows),len(uniq)))
print("구 ECS 소스와 일치 : %d / 불일치 : %d\n" % (hit,len(miss)))
for l in lines: print("  ",l)

if miss:
    print("\n[구 ECS 소스에서 못 찾은 주소]")
    for sl,tok,dv,a,note in miss:
        print("   슬라이드 %-3d %-8s %s %-5d %s" % (sl,tok,dv,a,note))
