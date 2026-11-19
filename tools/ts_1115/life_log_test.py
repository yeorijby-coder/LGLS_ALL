# -*- coding: utf-8 -*-
"""기동/종료 로그 시험 (HOST_TASK, IO_TASK, WCS_TASK_CV 공용).
사용: python life_log_test.py <exe폴더> <exe이름> <창제목에 포함된 글자> <종료확인 단추 순번 0=첫째>
  1) 실행 폴더의 LOG 를 지우고 exe 를 띄운다  2) 15초 뒤 LOG\\MMdd.log 확인
  3) 창에 WM_CLOSE → 종료 확인 대화상자에서 [예/확인] 클릭  4) 프로세스 종료·LOG 마지막 줄 확인
"""
import sys, os, time, subprocess, shutil, ctypes, ctypes.wintypes as wt, datetime
user32 = ctypes.windll.user32
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)


def wins(pid):
    out = []
    def cb(h, l):
        p = wt.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(p))
        if p.value == pid and user32.IsWindowVisible(h):
            t = ctypes.create_unicode_buffer(256); user32.GetWindowTextW(h, t, 256)
            k = ctypes.create_unicode_buffer(64); user32.GetClassNameW(h, k, 64)
            out.append((h, k.value, t.value))
        return True
    user32.EnumWindows(WNDENUM(cb), 0)
    return out


d, exe = sys.argv[1], sys.argv[2]
log = os.path.join(d, 'LOG', datetime.date.today().strftime('%m%d') + '.log')
shutil.rmtree(os.path.join(d, 'LOG'), ignore_errors=True)
p = subprocess.Popen([os.path.join(d, exe)], cwd=d, stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(15)
print('LOG 폴더:', os.path.isdir(os.path.join(d, 'LOG')), ' 파일:', os.path.isfile(log))
if os.path.isfile(log):
    ls = open(log, 'rb').read().decode('cp949', 'replace').splitlines()
    print('줄 수:', len(ls)); print('\n'.join(ls[:5]))
main = [h for h, k, t in wins(p.pid) if k != '#32770']
print('창:', [(k, t) for h, k, t in wins(p.pid)])
if main:
    user32.PostMessageW(main[0], 0x10, 0, 0)   # WM_CLOSE
    t0 = time.time()
    while time.time() - t0 < 20 and p.poll() is None:
        time.sleep(0.5)
        for h, k, t in wins(p.pid):
            if k == '#32770':
                b1 = user32.FindWindowExW(h, None, 'Button', None)
                b2 = user32.FindWindowExW(h, b1, 'Button', None)
                tgt = b1   # 첫째 단추 = 예/확인
                user32.PostMessageW(tgt, 0xF5, 0, 0)
                time.sleep(1)
tt = time.time()
while time.time() - tt < 20 and p.poll() is None:
    time.sleep(0.5)
print('종료됨:', p.poll() is not None, p.poll())
if p.poll() is None: p.kill()
if os.path.isfile(log):
    ls = open(log, 'rb').read().decode('cp949', 'replace').splitlines()
    print('종료 후 줄 수:', len(ls)); print('\n'.join(ls[-4:]))
print('(기동·종료 줄)')
for l in ls:
    if '[기동]' in l or '[종료]' in l: print(l)
