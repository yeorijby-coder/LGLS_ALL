# -*- coding: utf-8 -*-
# [LGLS 2026-09-10] 전체 테이블 사용 현황 조사
#   각 테이블이 "실제로 빌드되는 프로그램" 에서 읽히는지/쓰이는지를 소스 전수 검색으로 판정한다.
#   0행 여부는 판정 근거로 쓰지 않는다(eqp_err_his 처럼 아직 안 쌓였을 뿐인 표가 있다).
import os, re, io, json

ROOT = r'D:\project\LGLS\Renewal'
SEP = os.sep

# 빌드에서 빠진 파일(참조가 있어도 실행에 영향 없음)
NOT_BUILT = [
    r'TASK\IO_TASK\CLS\cThread_CV.cs',
    r'TASK\IO_TASK\CLS\cThread_SC.cs',
    r'TASK\IO_TASK\CLS\cThread_R.cs',
    r'TASK\IO_TASK\CLS\Thread\cThread_CV.cs',
    r'TASK\IO_TASK\CLS\Thread\cThread_SC.cs',
    r'TASK\IO_TASK\CLS\Thread\cThread_R.cs',
]
NOT_BUILT = [x.lower() for x in NOT_BUILT]

# 소스(빌드 대상) 폴더
SRC_DIRS = [
    r'WCS\CPlusPlus\LGLS_CLIENT\Ecs',
    r'TASK\WCS_TASK_CV_BIN',
    r'TASK\IO_TASK',
    r'TASK\WCS_TASK_HOST',
    r'TASK\ALL_TASK',
]
# 스크립트(DDL/시드) - 실행 프로그램이 아니라 구축/시험용
SQL_DIRS = [r'SIM\db', r'TASK\IO_TASK\DB', r'DB_BACKUP']

SKIP_PARTS = [SEP + s + SEP for s in ('obj', 'bin', 'Bin', 'Debug', 'Release', '_removed_20260910')]

TABLES = [l.split('|')[0].strip() for l in
          io.open(os.path.join(ROOT, r'docs\산출물_20260909\gen\tables.txt'), encoding='cp949',
                  errors='replace').read().splitlines()
          if '|' in l and not l.startswith('tbl') and not set(l.strip()) <= set('-|')]
TABLES = [t for t in TABLES if t]

WRITE_RE = {}
READ_RE = {}
ANY_RE = {}
for t in TABLES:
    e = re.escape(t)
    ANY_RE[t] = re.compile(r'(?<![A-Za-z0-9_])' + e + r'(?![A-Za-z0-9_])', re.I)
    WRITE_RE[t] = re.compile(r'(INSERT\s+INTO|UPDATE|DELETE\s+FROM)\s+(dbo\.)?' + e + r'(?![A-Za-z0-9_])', re.I)
    READ_RE[t] = re.compile(r'(FROM|JOIN)\s+(dbo\.)?' + e + r'(?![A-Za-z0-9_])', re.I)


def read_text(p):
    raw = open(p, 'rb').read()
    for enc in ('utf-8-sig', 'cp949', 'latin-1'):
        try:
            return raw.decode(enc)
        except Exception:
            pass
    return ''


def walk(dirs, exts):
    for d in dirs:
        base = os.path.join(ROOT, d)
        for dp, dn, fn in os.walk(base):
            if any(s.lower() in (dp + SEP).lower() for s in SKIP_PARTS):
                dn[:] = []
                continue
            for f in fn:
                if f.lower().endswith(exts):
                    yield os.path.join(dp, f)


res = {t: {'w': set(), 'r': set(), 'a': set(), 'x': set(), 's': set()} for t in TABLES}

for p in walk(SRC_DIRS, ('.cs', '.cpp', '.h')):
    rel = p[len(ROOT) + 1:]
    built = rel.lower() not in NOT_BUILT
    txt = read_text(p)
    if not txt:
        continue
    for t in TABLES:
        if not ANY_RE[t].search(txt):
            continue
        if not built:
            res[t]['x'].add(rel)
            continue
        res[t]['a'].add(rel)
        if WRITE_RE[t].search(txt):
            res[t]['w'].add(rel)
        if READ_RE[t].search(txt):
            res[t]['r'].add(rel)

for p in walk(SQL_DIRS, ('.sql',)):
    rel = p[len(ROOT) + 1:]
    txt = read_text(p)
    for t in TABLES:
        if ANY_RE[t].search(txt):
            res[t]['s'].add(rel)

out = {t: {k: sorted(v) for k, v in d.items()} for t, d in res.items()}
io.open(r'C:\Users\USER\AppData\Local\Temp\tablescan.json', 'w', encoding='utf-8').write(
    json.dumps(out, ensure_ascii=False, indent=1))
print('테이블 %d개 조사' % len(TABLES))
for t in TABLES:
    d = res[t]
    print('%-18s 빌드참조 %2d (쓰기 %d/읽기 %d)  빌드제외 %d  스크립트 %d'
          % (t, len(d['a']), len(d['w']), len(d['r']), len(d['x']), len(d['s'])))
