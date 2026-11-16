# -*- coding: utf-8 -*-
"""[INI 열기] 로 뜬 메모장이 소켓을 물려받아, HOST_TASK 종료 뒤에도 포트가 남는지 재현.
사용: python inherit_test.py <exe폴더(새 사본)> <포트>"""
import sys, os, re, socket, subprocess, time, ctypes, ctypes.wintypes as wt

d, port = sys.argv[1], int(sys.argv[2])
ini = os.path.join(d, 'EcsComA.ini')
raw = open(ini, 'rb').read()
raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % port, raw)
raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=18002', raw)
open(ini, 'wb').write(raw)

user32 = ctypes.windll.user32
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)


def top_windows(pid):
    out = []
    def cb(h, l):
        pp = wt.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(pp))
        if pp.value == pid and user32.IsWindowVisible(h):
            buf = ctypes.create_unicode_buffer(256); user32.GetClassNameW(h, buf, 256)
            out.append((h, buf.value))
        return True
    user32.EnumWindows(WNDENUM(cb), 0)
    return out


def find_child_by_text(root, text):
    found = []
    def cb(h, l):
        buf = ctypes.create_unicode_buffer(256); user32.GetWindowTextW(h, buf, 256)
        if buf.value == text: found.append(h)
        return True
    user32.EnumChildWindows(root, WNDENUM(cb), 0)
    return found[0] if found else None


def sh(cmd):
    return subprocess.run(cmd, shell=True, capture_output=True).stdout.decode('cp949', 'replace')


def netstat():
    out = sh('netstat -ano | findstr :%d' % port)
    return [l.strip() for l in out.splitlines() if l.strip()]


def msg():
    body = b'\x02Z0001\x03'
    return b'WMS_MBX'.ljust(10) + b'%04d' % len(body) + b' ' + body


def start():
    return subprocess.Popen([os.path.join(d, 'TASK_LFC10_G1_ECSCOM.exe')], cwd=d)


def wait_up(p, secs=40):
    t = time.time()
    while time.time() - t < secs:
        try:
            socket.create_connection(('127.0.0.1', port), timeout=1).close(); return True
        except Exception:
            time.sleep(0.5)
    return False


def close_app(p):
    for h, c in top_windows(p.pid):
        if not c.startswith('#32770'): user32.PostMessageW(h, 0x10, 0, 0)
    t = time.time()
    while time.time() - t < 10:
        time.sleep(0.2)
        for h, c in top_windows(p.pid):
            if c == '#32770':
                b = user32.FindWindowExW(h, None, 'Button', None)
                if b: user32.PostMessageW(b, 0xF5, 0, 0); t = 0
        if p.poll() is not None: break
    try: p.wait(timeout=30)
    except Exception: p.kill()


p = start()
assert wait_up(p), 'HOST 기동 실패'
time.sleep(8)
host_pid = p.pid
print('HOST PID =', host_pid)

a = socket.create_connection(('127.0.0.1', port), timeout=5); a.settimeout(8)
a.sendall(msg()); print('응답', len(a.recv(26)))
print('--- INI 열기 누르기 전 netstat'); [print('  ', l) for l in netstat()]

btn = None
for h, c in top_windows(p.pid):
    if not c.startswith('#32770'):
        btn = find_child_by_text(h, 'INI 열기')
print('INI 열기 버튼 찾음:', bool(btn))
if btn: user32.PostMessageW(btn, 0xF5, 0, 0)
time.sleep(3)
notepads = sh('powershell -NoProfile -Command "Get-CimInstance Win32_Process -Filter \\"Name=\'notepad.exe\'\\" | Where-Object { $_.CommandLine -like \'*EcsComA.ini*\' } | ForEach-Object { $_.ProcessId }"').split()
print('뜬 메모장 PID =', notepads)

close_app(p)
time.sleep(2)
print('HOST 프로세스 살아 있음?', p.poll() is None, '| tasklist:', 'TASK_LFC10' in sh('tasklist /FI "PID eq %d"' % host_pid))
print('--- HOST 종료 직후 netstat (메모장이 열린 채)'); [print('  ', l) for l in netstat()]

a.close()
time.sleep(2)
print('--- 클라이언트(WMS 역할)도 닫은 뒤 netstat'); [print('  ', l) for l in netstat()]

# 같은 포트로 다시 띄워 본다
p2 = start()
time.sleep(25)
logs = ''
try:
    import glob
    for f in glob.glob(os.path.join(d, 'ECSCOMALog', 'Srv', '*.Log')):
        logs += open(f, 'rb').read().decode('cp949', 'replace')
except Exception: pass
print('새 HOST 의 "수신 포트 ... 열지 못했습니다" 횟수 =', logs.count('열지 못했습니다'))
close_app(p2)

for n in notepads: sh('taskkill /PID %s /F' % n)
time.sleep(2)
print('--- 메모장 종료 뒤 netstat'); [print('  ', l) for l in netstat()]
print('(끝)')
