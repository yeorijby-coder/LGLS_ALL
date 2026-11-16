# -*- coding: utf-8 -*-
"""[포트 정리] 핸들 닫기 단계 시험(하네스 h.exe). 사용: python port_hunt_test.py <h.exe> <Repro.exe> <포트> <선택들: yes|no ...>
 Repro.exe 가 죽은 부모 + 리스너를 쥔 자손(cmd/ping)을 만든다. h.exe 옆에 HandleHunter.exe 가 있어야 한다."""
import sys, subprocess, time, ctypes, ctypes.wintypes as wt, socket, os

exe, repro, port, choices = sys.argv[1], sys.argv[2], int(sys.argv[3]), list(sys.argv[4:])
user32 = ctypes.windll.user32
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)


def sh(c):
    return subprocess.run(c, shell=True, capture_output=True).stdout.decode('cp949', 'replace')


def wins(pid):
    out = []
    def cb(h, l):
        pp = wt.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(pp))
        if pp.value == pid and user32.IsWindowVisible(h):
            k = ctypes.create_unicode_buffer(100); user32.GetClassNameW(h, k, 100); out.append((h, k.value))
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


subprocess.run([repro, str(port)])
time.sleep(2)
print('--- 시작 전 netstat'); print(sh('netstat -ano | findstr :%d' % port).strip())

p = subprocess.Popen([exe, str(port)], stdout=subprocess.PIPE)
handled = set(); n = 0; t0 = time.time()
while time.time() - t0 < 40 and p.poll() is None:
    time.sleep(0.5)
    for h, k in wins(p.pid):
        if k == '#32770' and h not in handled:
            handled.add(h); n += 1
            print('--- 메시지 박스 %d' % n); print('\n'.join(texts(h)))
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
print('--- 끝난 뒤 netstat'); print(sh('netstat -ano | findstr :%d' % port).strip() or '(없음)')
t = socket.socket()
try: t.bind(('0.0.0.0', port)); print('새 리스너 바인드: 성공')
except OSError as e: print('새 리스너 바인드: 실패', e.errno)
t.close()
# 시험용 자손 정리
sh('powershell -NoProfile -Command "Get-CimInstance Win32_Process | Where-Object { $_.CommandLine -like \'*ping -n 1500*\' } | ForEach-Object { Stop-Process -Id $_.ProcessId -Force }"')
