# -*- coding: utf-8 -*-
"""사용자 엑셀(시나리오 영역.xlsx) ↔ PlcAddressMap.xml 전수 대조.

  엑셀 시트 순서 ↔ 시나리오 id
    1 입고 TR #22 => SC #1  : 1
    2 입고 TR #26 => SC #1  : 3-1
    3 입고 TR #30 => SC #1  : 4-1
    4 출고 SC #1 => TR #29  : 4-2
    5 출고 SC #1 => TR #24  : 3-2
    6 출고 SC #1 => TR #22  : 2
"""
import io, json, re, sys

SHEET2ID = ["1", "3-1", "4-1", "4-2", "3-2", "2"]

d = json.load(io.open("scen.json", encoding="utf-8"))
scen = {s["id"]: s for s in d["scenarios"]}

def cell_expect(step, cr):
    p = step["per"][str(cr)]
    if p["addr"] < 0:
        return "-"
    eq = {"CV": "C/V #", "SC": "S/C #", "RGV": "RGV #"}.get(step["equip"], "")
    eq = (eq + str(p["no"])) if eq else ""
    dev = p["dev"]
    if dev == "M":
        ad = "%%MX%d" % p["addr"]
    elif dev == "D":
        ad = "%%DW%d" % p["addr"]
    else:
        ad = "%%RB%d" % (p["addr"] * 2)
    return "%s  %s" % (eq, ad)

# ── 엑셀 덤프 파싱 ────────────────────────────────────────────────────
txt = io.open("xl_dump.txt", encoding="utf-8-sig").read()
blocks = re.split(r"=== SHEET: (.*?) ===", txt)[1:]
sheets = []
for i in range(0, len(blocks), 2):
    name, body = blocks[i], blocks[i + 1]
    rows = []
    for line in body.splitlines():
        m = re.match(r"\s*(\d+):\s(.*)$", line)
        if not m:
            continue
        rows.append((int(m.group(1)), m.group(2).split(" | ")))
    sheets.append((name, rows))

print("시트 수:", len(sheets))
total_cells = 0
diffs = []
missing = []

for si, (name, rows) in enumerate(sheets):
    sid = SHEET2ID[si]
    sc = scen[sid]
    # 데이터 행 = 첫 열이 순번 숫자인 행
    data = []
    for rn, cols in rows:
        if cols and re.fullmatch(r"\d+", cols[0].strip()):
            data.append((rn, cols))
    print("\n" + "=" * 78)
    print("SHEET %d  %s   ↔  시나리오 %s (%s)" % (si + 1, name, sid, sc["title"]))
    print("  엑셀 데이터행 %d 개 / XML 스텝 %d 개" % (len(data), len(sc["steps"])))
    if len(data) != len(sc["steps"]):
        missing.append((name, len(data), len(sc["steps"])))

    n = min(len(data), len(sc["steps"]))
    for k in range(n):
        rn, cols = data[k]
        st = sc["steps"][k]
        # 열 구성 : # | 구간 | 주체 | 신호/설명 | 1호기 .. 5호기
        if len(cols) < 9:
            diffs.append((name, rn, "열부족", " | ".join(cols), ""))
            continue
        desc_xl = cols[3].strip()
        desc_xml = (st["desc"] or "").strip()
        if desc_xl.replace(" ", "") != desc_xml.replace(" ", ""):
            diffs.append((name, rn, "설명", desc_xl, desc_xml))
        for cr in range(1, 6):
            got = cols[3 + cr].strip()
            exp = cell_expect(st, cr).strip()
            total_cells += 1
            g = re.sub(r"\s+", " ", got)
            e = re.sub(r"\s+", " ", exp)
            if g != e:
                diffs.append((name, rn, "%d호기" % cr, got, exp))

print("\n" + "=" * 78)
print("대조 셀 수 :", total_cells)
print("불일치     :", len(diffs))
if missing:
    print("\n[행수 불일치]")
    for m in missing:
        print("   %s : 엑셀 %d행 vs XML %d스텝" % m)
if diffs:
    print("\n[불일치 상세]")
    for t in diffs[:120]:
        print("   %s r%s [%s]\n      엑셀: %s\n      XML : %s" % t)
    if len(diffs) > 120:
        print("   ... 외 %d건" % (len(diffs) - 120))
else:
    print("\n★ 주소·설명 전 항목 일치")
