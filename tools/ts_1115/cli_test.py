# -*- coding: utf-8 -*-
"""송신 비동기 시험: 가짜 WMS(8002 역할) 서버를 열고 HOST_TASK 를 띄워 본다.
사용: python cli_test.py <exe폴더> <HOST수신포트> <가짜WMS포트> <CLI_ASYNC 0|1>"""
import sys, os, re, socket, subprocess, time, threading, ctypes, ctypes.wintypes as wt

d, hport, wport, mode = sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), sys.argv[4]
SQL = ['sqlcmd', '-S', '127.0.0.1,1435', '-U', 'LGLS_IO', '-P', 'LGLS_IO', '-d', 'LGLS_MCS_IO', '-W', '-h', '-1']

ini = os.path.join(d, 'EcsComA.ini'); raw = open(ini, 'rb').read()
raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % hport, raw)
raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=%d' % wport, raw)
raw = raw.replace(b'[Host]', b'[Host]\r\nCLI_ASYNC=' + mode.encode(), 1)
open(ini, 'wb').write(raw)

events = []          # (시각, 종류, 내용)
t0 = time.time()
lock = threading.Lock()
conns = []
ack_f = threading.Event()
srv = socket.socket(); srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
srv.bind(('127.0.0.1', wport)); srv.listen(5)
stop = False


def log(kind, what):
    with lock:
        events.append((round(time.time() - t0, 1), kind, what))


def frames(buf):
    out = []
    while len(buf) >= 15:
        n = int(buf[10:14]); tot = 15 + n
        if len(buf) < tot: break
        out.append(buf[:tot]); buf = buf[tot:]
    return out, buf


def handle(c, idx):
    buf = b''
    c.settimeout(1)
    while not stop:
        try:
            dta = c.recv(4096)
        except socket.timeout:
            if c.fileno() == -1: break
            continue
        except Exception:
            break
        if not dta:
            log('server', '접속 %d: 클라이언트(HOST)가 닫음' % idx); break
        buf += dta
        fs, buf = frames(buf)
        for f in fs:
            t = chr(f[16]) if len(f) > 16 else '?'
            log('recv', '접속 %d: %s %s' % (idx, t, f[15:].decode('latin1').replace('\x02', '<STX>').replace('\x03', '<ETX>')[:40]))
            if t == 'F':
                body = b'\x02' + b'fA00' + f[18:22] + b'1' + b'\x03'      # 응답 11바이트
                hdr = b'WMS_MBX'.ljust(10) + b'%04d' % len(body) + b' '
                c.sendall(hdr + body); log('send', '접속 %d: F 에 대한 ACK 전송' % idx); ack_f.set()


def acceptor():
    idx = 0
    srv.settimeout(1)
    while not stop:
        try:
            c, a = srv.accept()
        except socket.timeout:
            continue
        except Exception:
            break
        idx += 1; conns.append(c); log('accept', '접속 %d 수락' % idx)
        threading.Thread(target=handle, args=(c, idx), daemon=True).start()


threading.Thread(target=acceptor, daemon=True).start()

p = subprocess.Popen([os.path.join(d, 'TASK_LFC10_G1_ECSCOM.exe')], cwd=d)
t_start = time.time()
while time.time() - t_start < 60 and not conns:
    time.sleep(0.5)
print('HOST 가 가짜 WMS 에 접속: %s (%.1fs)' % (bool(conns), time.time() - t_start))
time.sleep(10)

# 1) 유휴 중 상대가 연결을 끊으면?
n_before = len(conns)
tclose = time.time() - t0
log('test', '서버가 첫 접속을 닫음')
conns[0].close()
reconnected = None
t1 = time.time()
while time.time() - t1 < 40:
    if len(conns) > n_before:
        reconnected = time.time() - t1; break
    time.sleep(0.2)
print('유휴 중 끊김 → 재접속까지: %s' % ('%.1f초' % reconnected if reconnected is not None else '40초 안에 안 됨'))

# 2) 완료(F) 보고 + 응답: 시험용 입고 작업(29)을 하나 넣는다
def sql(q):
    return subprocess.run(SQL + ['-Q', q], capture_output=True).stdout.decode('cp949', 'replace').strip()

print(sql("SET NOCOUNT ON; IF OBJECT_ID('tempdb..#t') IS NOT NULL DROP TABLE #t; "
          "SELECT * INTO #t FROM JOB_MST WHERE LUGG_NO='0254'; "
          "UPDATE #t SET LUGG_NO='7701', JOB_STATUS='29', WC_STEP='0'; "
          "INSERT INTO JOB_MST SELECT * FROM #t; SELECT COUNT(*) FROM JOB_MST WHERE LUGG_NO='7701'"))
got = ack_f.wait(30)
print('F 보고를 받고 ACK 를 돌려줌: %s' % got)
time.sleep(5)
left = sql("SET NOCOUNT ON; SELECT COUNT(*) FROM JOB_MST WHERE LUGG_NO='7701'")
print('HOST 가 응답을 받은 뒤 시험 작업을 지움(남은 행): %s' % left)

# 3) 정리
sql("DELETE FROM JOB_MST WHERE LUGG_NO='7701'")

user32 = ctypes.windll.user32
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)
def wins(pid):
    out = []
    def cb(h, l):
        pp = wt.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(pp))
        if pp.value == pid and user32.IsWindowVisible(h):
            b = ctypes.create_unicode_buffer(256); user32.GetClassNameW(h, b, 256); out.append((h, b.value))
        return True
    user32.EnumWindows(WNDENUM(cb), 0); return out
for h, c in wins(p.pid):
    if not c.startswith('#32770'): user32.PostMessageW(h, 0x10, 0, 0)
tt = time.time(); clicked = False
while time.time() - tt < 10 and not clicked:
    time.sleep(0.2)
    for h, c in wins(p.pid):
        if c == '#32770':
            b = user32.FindWindowExW(h, None, 'Button', None)
            if b: user32.PostMessageW(b, 0xF5, 0, 0); clicked = True
tc = time.time()
try:
    p.wait(timeout=60); print('종료 %.1f초' % (time.time() - tc))
except subprocess.TimeoutExpired:
    print('종료 안 됨'); p.kill()
stop = True
print('--- 서버가 본 이벤트(요약)')
kinds = {}
for t, k, w in events:
    if k == 'recv':
        kinds[w.split(': ')[1][:1]] = kinds.get(w.split(': ')[1][:1], 0) + 1
    else:
        print('  %5.1fs %s %s' % (t, k, w))
print('  수신한 전문 종류별 건수:', kinds)
