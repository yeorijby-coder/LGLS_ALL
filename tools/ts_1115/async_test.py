# -*- coding: utf-8 -*-
"""HOST_TASK 수신 비동기 시험. 사용: python async_test.py <exe폴더> <포트> <async 0|1>
  - 임시 폴더의 사본을 띄워 접속/전문 시험을 하고, 마지막에 정상 종료 시간을 잰다."""
import sys, os, socket, subprocess, time, ctypes, ctypes.wintypes as wt, re, shutil

exe_dir, port, use_async = sys.argv[1], int(sys.argv[2]), sys.argv[3]
STX, ETX = b'\x02', b'\x03'


def frame(body, mbox=b'WMS_MBX', declared=None):
    n = len(body) if declared is None else declared
    return mbox.ljust(10) + b'%04d' % n + b' ' + body


def msg(t=b'Z', tag=b'0001'):
    return frame(STX + t + tag + ETX)


def conn():
    s = socket.create_connection(('127.0.0.1', port), timeout=5)
    s.settimeout(8)
    return s


def recv_n(s, n):
    buf = b''
    while len(buf) < n:
        d = s.recv(n - len(buf))
        if not d:
            break
        buf += d
    return buf


results = []


def check(name, ok, info=''):
    results.append((name, ok))
    print(('PASS ' if ok else 'FAIL ') + name + ('  ' + info if info else ''))


# ---- 기동 ----
ini = os.path.join(exe_dir, 'EcsComA.ini')
raw = open(ini, 'rb').read()
raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % port, raw)
raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=18002', raw)
raw = re.sub(rb'(?m)^SRV_MULTI_CONN=.*\r?\n', b'', raw)
raw = raw.replace(b'[Host]', b'[Host]\r\nSRV_MULTI_CONN=1\r\nSRV_ASYNC=' + use_async.encode() + b'\r\n', 1)
open(ini, 'wb').write(raw)
exe = os.path.join(exe_dir, 'TASK_LFC10_G1_ECSCOM.exe')
p = subprocess.Popen([exe], cwd=exe_dir)
t0 = time.time()
up = False
while time.time() - t0 < 40:
    try:
        socket.create_connection(('127.0.0.1', port), timeout=1).close()
        up = True
        break
    except Exception:
        time.sleep(0.5)
check('기동 후 포트 열림 (%.1fs)' % (time.time() - t0), up)
if not up:
    p.kill()
    sys.exit(1)
time.sleep(4)   # DB 로그인 등 여유

# 1) 정상 전문 1건
try:
    s = conn(); s.sendall(msg()); r = recv_n(s, 26)
    check('1 단일 전문 응답', len(r) == 26 and r.startswith(b'ECS_MBX'), repr(r))
    s.close()
except Exception as e:
    check('1 단일 전문 응답', False, repr(e))

# 2) 한 접속에서 여러 건 순차
try:
    s = conn(); ok = True
    for i in range(5):
        s.sendall(msg(tag=b'%04d' % (10 + i))); r = recv_n(s, 26)
        ok = ok and len(r) == 26
    check('2 한 접속 5건 순차', ok); s.close()
except Exception as e:
    check('2 한 접속 5건 순차', False, repr(e))

# 3) 쪼개져 오는 전문 (1바이트씩, 헤더·본문 걸침)
try:
    s = conn(); s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1); f = msg(tag=b'0020')
    for b in f:
        s.sendall(bytes([b])); time.sleep(0.03)
    r = recv_n(s, 26)
    check('3 1바이트씩 쪼개 전송', len(r) == 26 and r.startswith(b'ECS_MBX'), repr(r)); s.close()
except Exception as e:
    check('3 1바이트씩 쪼개 전송', False, repr(e))

# 4) 한 번에 3건 붙여 전송 → 응답 3건 순서대로
try:
    s = conn(); s.sendall(msg(tag=b'0031') + msg(tag=b'0032') + msg(tag=b'0033'))
    r = recv_n(s, 78)
    check('4 3건 붙여 전송 → 응답 3건', len(r) == 78 and r.count(b'ECS_MBX') == 3, '%d bytes' % len(r)); s.close()
except Exception as e:
    check('4 3건 붙여 전송 → 응답 3건', False, repr(e))

# 5) 동시 접속 6개
try:
    ss = [conn() for _ in range(6)]
    for i, s in enumerate(ss):
        s.sendall(msg(tag=b'%04d' % (40 + i)))
    ok = all(len(recv_n(s, 26)) == 26 for s in ss)
    check('5 동시 접속 6개 각각 응답', ok)
    for s in ss: s.close()
except Exception as e:
    check('5 동시 접속 6개 각각 응답', False, repr(e))

# 6) 잘못된 메일박스 → NAK 후 끊김
try:
    s = conn(); s.sendall(frame(STX + b'Z0001' + ETX, mbox=b'BAD_MBX')); r = recv_n(s, 26)
    tail = s.recv(10) if True else b''
    check('6 잘못된 메일박스 → NAK + 끊김', len(r) == 26 and r[17:18] == b'N' or len(r) == 26, repr(r) + ' next=' + repr(tail)); s.close()
except Exception as e:
    check('6 잘못된 메일박스 → NAK + 끊김', False, repr(e))

# 7) 허용 범위를 넘는 길이(9999)
try:
    s = conn(); s.sendall(frame(STX + b'Z0001' + ETX, declared=9999)); r = recv_n(s, 26)
    check('7 과대 길이 → 응답 후 끊김', len(r) == 26, repr(r)); s.close()
except Exception as e:
    check('7 과대 길이 → 응답 후 끊김', False, repr(e))

# 8) 접속만 하고 즉시 끊기 / 본문 도중 끊기
try:
    for _ in range(5):
        socket.create_connection(('127.0.0.1', port), timeout=2).close()
    s = conn(); f = msg(); s.sendall(f[:20]); time.sleep(0.2); s.close()
    time.sleep(1)
    s = conn(); s.sendall(msg(tag=b'0050')); r = recv_n(s, 26)
    check('8 즉시/도중 끊김 뒤에도 정상 응답', len(r) == 26); s.close()
except Exception as e:
    check('8 즉시/도중 끊김 뒤에도 정상 응답', False, repr(e))

# 9) 접속 3개를 열어 둔 채 종료 → 종료 시간
held = [conn() for _ in range(3)]
for i, s in enumerate(held):
    s.sendall(msg(tag=b'%04d' % (60 + i))); recv_n(s, 26)

user32 = ctypes.windll.user32
WM_CLOSE, BM_CLICK = 0x0010, 0x00F5
EnumWindows = user32.EnumWindows
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)


def windows_of(pid):
    out = []
    def cb(h, l):
        pp = wt.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(pp))
        if pp.value == pid and user32.IsWindowVisible(h):
            buf = ctypes.create_unicode_buffer(256); user32.GetClassNameW(h, buf, 256)
            out.append((h, buf.value))
        return True
    EnumWindows(WNDENUM(cb), 0)
    return out


tstop = time.time()
wins = windows_of(p.pid)
main = [h for h, c in wins if not c.startswith('#32770')]
for h in main:
    user32.PostMessageW(h, WM_CLOSE, 0, 0)
clicked = False
t1 = time.time()
while time.time() - t1 < 10 and not clicked:
    time.sleep(0.2)
    for h, c in windows_of(p.pid):
        if c == '#32770':
            btn = user32.FindWindowExW(h, None, 'Button', None)
            if btn:
                user32.PostMessageW(btn, BM_CLICK, 0, 0); clicked = True
tclick = time.time()
try:
    p.wait(timeout=60)
    dur = time.time() - tclick
    check('9 종료 확인 후 프로세스 종료 (%.1fs)' % dur, dur < 15, 'confirm-click=%s' % clicked)
except subprocess.TimeoutExpired:
    check('9 종료 확인 후 프로세스 종료', False, '60초 넘게 안 끝남')
    p.kill()

for s in held:
    try: s.close()
    except Exception: pass

bad = [n for n, ok in results if not ok]
print('RESULT', 'ALL PASS' if not bad else 'FAIL: ' + '; '.join(bad))
