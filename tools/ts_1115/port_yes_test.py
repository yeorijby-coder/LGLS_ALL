# -*- coding: utf-8 -*-
"""[포트 정리] 확인창 흐름 시험(하네스 h.exe, 관리자 확인 우회). 사용: python port_yes_test.py <h.exe> <포트>"""
import sys, socket, subprocess, time, ctypes, ctypes.wintypes as wt

exe, port = sys.argv[1], int(sys.argv[2])
user32 = ctypes.windll.user32
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)


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


def run(choice):
    srv = socket.socket(); srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('0.0.0.0', port)); srv.listen(5)
    c = socket.create_connection(('127.0.0.1', port)); a, _ = srv.accept(); c.close()
    p = subprocess.Popen([exe, str(port)], stdout=subprocess.PIPE)
    seen = []
    t0 = time.time()
    while time.time() - t0 < 20 and p.poll() is None:
        time.sleep(0.5)
        for h, k in wins(p.pid):
            if k == '#32770':
                tx = '\n'.join(texts(h))
                seen.append(tx)
                b1 = user32.FindWindowExW(h, None, 'Button', None)
                b2 = user32.FindWindowExW(h, b1, 'Button', None)
                if b2:           # 예/아니요 확인창
                    user32.PostMessageW(b1 if choice == 'yes' else b2, 0xF5, 0, 0)
                else:
                    user32.PostMessageW(b1, 0xF5, 0, 0)
                time.sleep(1)
    for h, k in wins(p.pid):
        user32.PostMessageW(h, 0x10, 0, 0)
    time.sleep(1)
    if p.poll() is None: p.kill()
    out = p.stdout.read().decode('utf-8', 'replace')
    a.close(); srv.close()
    return seen, out


for choice in sys.argv[3:]:
    seen, out = run(choice)
    print('===== 선택:', '아니요' if choice == 'no' else '예')
    for i, tx in enumerate(seen): print('--- 메시지 박스 %d' % (i + 1)); print(tx)
    print('--- 로그'); print(out.strip() or '(없음)')
