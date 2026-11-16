# -*- coding: utf-8 -*-
"""[포트 정리] TASKKILL 단계 시험(하네스 h.exe). 사용: python port_kill_test.py <h.exe> <포트> <선택들: yes|no ...>
 선택은 예/아니요 확인창이 뜰 때마다 차례로 쓰고, 확인 단추만 있는 창은 확인을 누른다.
 소켓은 별도의 자식 python 이 쥔다(그 프로세스가 TASKKILL 대상)."""
import sys, socket, subprocess, time, ctypes, ctypes.wintypes as wt

exe, port, choices = sys.argv[1], int(sys.argv[2]), list(sys.argv[3:])
user32 = ctypes.windll.user32
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)

CHILD = ("import socket,time\n"
         "s=socket.socket();s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1);s.bind(('0.0.0.0',%d));s.listen(5)\n"
         "c=socket.create_connection(('127.0.0.1',%d));a,_=s.accept();c.close()\n"
         "time.sleep(300)\n") % (port, port)


def wins(pid):
    out = []
    def cb(h, l):
        pp = wt.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(pp))
        if pp.value == pid and user32.IsWindowVisible(h):
            k = ctypes.create_unicode_buffer(100); user32.GetClassNameW(h, k, 100)
            out.append((h, k.value))
        return True
    user32.EnumWindows(WNDENUM(cb), 0); return out


def texts(h):
    out = []
    def cb(c, l):
        k = ctypes.create_unicode_buffer(100); user32.GetClassNameW(c, k, 100)
        t = ctypes.create_unicode_buffer(4000); user32.SendMessageW(c, 0x000D, 4000, t)
        if k.value == 'Static' and t.value: out.append(t.value)
        return True
    user32.EnumChildWindows(h, WNDENUM(cb), 0); return out


child = subprocess.Popen([sys.executable, '-c', CHILD])
time.sleep(2)
print('자식(소켓을 쥔 프로세스) PID =', child.pid)

p = subprocess.Popen([exe, str(port)], stdout=subprocess.PIPE)
handled = set()
t0 = time.time()
n = 0
while time.time() - t0 < 25 and p.poll() is None:
    time.sleep(0.5)
    for h, k in wins(p.pid):
        if k == '#32770' and h not in handled:
            handled.add(h)
            n += 1
            print('--- 메시지 박스 %d' % n)
            print('\n'.join(texts(h)))
            b1 = user32.FindWindowExW(h, None, 'Button', None)
            b2 = user32.FindWindowExW(h, b1, 'Button', None)
            if b2:
                ch = choices.pop(0) if choices else 'no'
                print('>>> 선택:', '예' if ch == 'yes' else '아니요')
                user32.PostMessageW(b1 if ch == 'yes' else b2, 0xF5, 0, 0)
            else:
                user32.PostMessageW(b1, 0xF5, 0, 0)
            time.sleep(1)
for h, k in wins(p.pid):
    user32.PostMessageW(h, 0x10, 0, 0)
time.sleep(1)
if p.poll() is None: p.kill()
out = p.stdout.read().decode('utf-8', 'replace')
print('--- 하네스 로그'); print(out.strip() or '(없음)')
print('자식 프로세스 살아 있음:', child.poll() is None)
if child.poll() is None: child.kill()
