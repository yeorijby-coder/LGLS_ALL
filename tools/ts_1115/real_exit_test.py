# -*- coding: utf-8 -*-
"""실제 HOST_TASK 로 [포트 정리] 시험: 메인 화면(버튼 위치) 캡처, 자기 자신 종료 확인에 예 → 마지막에 정상 종료되는지.
사용: python real_exit_test.py <exe폴더> <포트> <출력폴더> <yes|no>"""
import sys, os, re, subprocess, time, ctypes, ctypes.wintypes as wt
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import importlib.util
spec = importlib.util.spec_from_file_location('ps', os.path.join(os.path.dirname(os.path.abspath(__file__)), 'port_shots_lib.py'))
ps = importlib.util.module_from_spec(spec); spec.loader.exec_module(ps)

d, port, outdir, choice = sys.argv[1], int(sys.argv[2]), sys.argv[3], sys.argv[4]
os.makedirs(outdir, exist_ok=True)
ini = os.path.join(d, 'EcsComA.ini'); raw = open(ini, 'rb').read()
raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % port, raw)
raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=18002', raw)
open(ini, 'wb').write(raw)

p = subprocess.Popen([os.path.join(d, 'TASK_LFC10_G1_ECSCOM.exe')], cwd=d)
time.sleep(16)
user32 = ps.user32
main = [h for h, k, t in ps.wins(p.pid) if not k.startswith('#32770')][0]
ps.shot(main, os.path.join(outdir, 'R01_메인화면_포트정리버튼.png'))
btn = None
def find(root, text):
    out = []
    def cb(c, l):
        b = ctypes.create_unicode_buffer(256); user32.GetWindowTextW(c, b, 256)
        if b.value == text: out.append(c)
        return True
    user32.EnumChildWindows(root, ps.WNDENUM(cb), 0)
    return out[0] if out else None
btn = find(main, '포트 정리')
print('버튼 찾음:', bool(btn))
user32.PostMessageW(btn, 0xF5, 0, 0)

handled = set(); n = 0; t0 = time.time(); shot_net = False
while time.time() - t0 < 40 and p.poll() is None:
    time.sleep(0.5)
    for h, k, t in ps.wins(p.pid):
        if h in handled: continue
        if '포트 정리 - netstat' in t and not shot_net:
            time.sleep(0.8); ps.shot(h, os.path.join(outdir, 'R02_netstat창.png')); shot_net = True; handled.add(h)
        elif k == '#32770':
            handled.add(h); n += 1; time.sleep(0.6)
            print('--- 메시지 박스 %d' % n); print('\n'.join(ps.texts(h)))
            ps.shot(h, os.path.join(outdir, 'R03_%02d.png' % n))
            b1 = user32.FindWindowExW(h, None, 'Button', None)
            b2 = user32.FindWindowExW(h, b1, 'Button', None)
            if b2:
                print('>>> 선택:', '예' if choice == 'yes' else '아니요')
                user32.PostMessageW(b1 if choice == 'yes' else b2, 0xF5, 0, 0)
            else:
                user32.PostMessageW(b1, 0xF5, 0, 0)
            time.sleep(1)
tt = time.time()
while time.time() - tt < 20 and p.poll() is None:
    time.sleep(0.5)
print('프로그램 종료됨:', p.poll() is not None, '(종료 코드 %s)' % p.poll())
if p.poll() is None: p.kill()
