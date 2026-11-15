# -*- coding: utf-8 -*-
"""수신 비동기 부하·누수 시험. 사용: python async_stress.py <exe폴더> <포트> <초>"""
import struct, sys, os, socket, subprocess, time, threading, random, ctypes, ctypes.wintypes as wt, re

exe_dir, port, secs = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
STX, ETX = b'\x02', b'\x03'


def msg(tag):
    body = STX + b'Z' + tag + ETX
    return b'WMS_MBX'.ljust(10) + b'%04d' % len(body) + b' ' + body


def recv_n(s, n):
    buf = b''
    while len(buf) < n:
        d = s.recv(n - len(buf))
        if not d:
            break
        buf += d
    return buf


ini = os.path.join(exe_dir, 'EcsComA.ini')
raw = open(ini, 'rb').read()
raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % port, raw)
raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=18002', raw)
open(ini, 'wb').write(raw)
p = subprocess.Popen([os.path.join(exe_dir, 'TASK_LFC10_G1_ECSCOM.exe')], cwd=exe_dir)
t0 = time.time()
while time.time() - t0 < 40:
    try:
        socket.create_connection(('127.0.0.1', port), timeout=1).close(); break
    except Exception:
        time.sleep(0.5)
time.sleep(8)

k32 = ctypes.windll.kernel32
k32.OpenProcess.restype = wt.HANDLE
PROCESS_QUERY = 0x1000


def handles():
    h = k32.OpenProcess(PROCESS_QUERY, False, p.pid)
    c = wt.DWORD()
    k32.GetProcessHandleCount(h, ctypes.byref(c))
    k32.CloseHandle(h)
    return c.value


def threads():
    out = subprocess.check_output(['powershell', '-NoProfile', '-Command',
                                   '(Get-Process -Id %d).Threads.Count' % p.pid]).decode().strip()
    return int(out)


h0, t0n = handles(), threads()
print('시작 핸들=%d 스레드=%d' % (h0, t0n))

stat = {'ok': 0, 'bad': 0, 'err': 0}
lock = threading.Lock()
stop = 0


def worker(wid):
    rnd = random.Random(wid)
    while time.time() < stop:
        try:
            s = socket.create_connection(('127.0.0.1', port), timeout=5)
            s.settimeout(10)
            if rnd.random() < 0.5:
                s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
            n = rnd.randint(1, 8)
            for i in range(n):
                f = msg(b'%04d' % rnd.randint(0, 9999))
                if rnd.random() < 0.3:
                    cut = rnd.randint(1, len(f) - 1)
                    s.sendall(f[:cut]); time.sleep(0.01); s.sendall(f[cut:])
                else:
                    s.sendall(f)
                r = recv_n(s, 26)
                with lock:
                    if len(r) == 26 and r.startswith(b'ECS_MBX'): stat['ok'] += 1
                    else: stat['bad'] += 1
            if rnd.random() < 0.2:
                s.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER, struct.pack('ii', 1, 0))
        except Exception as e:
            with lock: stat['err'] += 1


for rd in range(3):
    stop = time.time() + secs
    ths = [threading.Thread(target=worker, args=(i,)) for i in range(12)]
    for t in ths: t.start()
    for t in ths: t.join()
    time.sleep(10)
    h1, t1n = handles(), threads()
    print('라운드%d 결과 %s  핸들=%d 스레드=%d' % (rd + 1, stat, h1, t1n))

user32 = ctypes.windll.user32
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)


def windows_of(pid):
    out = []
    def cb(h, l):
        pp = wt.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(pp))
        if pp.value == pid and user32.IsWindowVisible(h):
            buf = ctypes.create_unicode_buffer(256); user32.GetClassNameW(h, buf, 256)
            out.append((h, buf.value))
        return True
    user32.EnumWindows(WNDENUM(cb), 0)
    return out


held = [socket.create_connection(('127.0.0.1', port), timeout=5) for _ in range(4)]
time.sleep(1)
for h, c in windows_of(p.pid):
    if not c.startswith('#32770'):
        user32.PostMessageW(h, 0x10, 0, 0)
clicked = False
tt = time.time()
while time.time() - tt < 10 and not clicked:
    time.sleep(0.2)
    for h, c in windows_of(p.pid):
        if c == '#32770':
            b = user32.FindWindowExW(h, None, 'Button', None)
            if b: user32.PostMessageW(b, 0xF5, 0, 0); clicked = True
tc = time.time()
try:
    p.wait(timeout=60); print('종료 %.1f초' % (time.time() - tc))
except subprocess.TimeoutExpired:
    print('종료 안 됨'); p.kill()
for s in held:
    try: s.close()
    except Exception: pass
ok = stat['bad'] == 0 and stat['err'] == 0 and stat['ok'] > 0
print('RESULT', 'PASS' if ok else 'FAIL')
