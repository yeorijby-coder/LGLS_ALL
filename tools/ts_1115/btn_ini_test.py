# -*- coding: utf-8 -*-
"""[포트 정리] 버튼 표시 설정 시험: ini 에 키가 없으면 숨김, PORT_CLEAN_BTN=1 이면 실행 중에도 바로 표시, 0 이면 다시 숨김.
사용: python btn_ini_test.py <exe폴더> <포트> <캡처폴더>"""
import sys, os, re, subprocess, time, ctypes, ctypes.wintypes as wt
HERE = os.path.dirname(os.path.abspath(__file__))
import importlib.util
spec = importlib.util.spec_from_file_location('ps', os.path.join(HERE, 'port_shots_lib.py'))
ps = importlib.util.module_from_spec(spec); spec.loader.exec_module(ps)

d, port, outdir = sys.argv[1], int(sys.argv[2]), sys.argv[3]
os.makedirs(outdir, exist_ok=True)
ini = os.path.join(d, 'EcsComA.ini')
raw = open(ini, 'rb').read()
raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % port, raw)
raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=18002', raw)
raw = re.sub(rb'(?m)^PORT_CLEAN_BTN=.*\r?\n', b'', raw)          # 키 없음 = 기본
open(ini, 'wb').write(raw)

user32 = ps.user32


def btn_state(main):
    found = []
    def cb(c, l):
        b = ctypes.create_unicode_buffer(256); user32.GetWindowTextW(c, b, 256)
        if b.value == '포트 정리': found.append(c)
        return True
    user32.EnumChildWindows(main, ps.WNDENUM(cb), 0)
    if not found: return '없음'
    return '보임' if user32.IsWindowVisible(found[0]) else '숨김'


def set_key(v):
    r = open(ini, 'rb').read()
    r = re.sub(rb'(?m)^PORT_CLEAN_BTN=.*\r?\n', b'', r)
    if v is not None:
        r = r.replace(b'[Host]', b'[Host]\r\nPORT_CLEAN_BTN=' + str(v).encode(), 1)
    open(ini, 'wb').write(r)


p = subprocess.Popen([os.path.join(d, 'TASK_LFC10_G1_ECSCOM.exe')], cwd=d)
time.sleep(16)
main = [h for h, k, t in ps.wins(p.pid) if not k.startswith('#32770')][0]
res = []
res.append(('키 없음(기본)', btn_state(main)))
ps.shot(main, os.path.join(outdir, 'B1_기본_숨김.png'))
set_key(1); time.sleep(3)
res.append(('PORT_CLEAN_BTN=1 로 바꾼 뒤 3초', btn_state(main)))
ps.shot(main, os.path.join(outdir, 'B2_표시.png'))
set_key(0); time.sleep(3)
res.append(('PORT_CLEAN_BTN=0 으로 되돌린 뒤 3초', btn_state(main)))
set_key(None); time.sleep(3)
res.append(('키를 지운 뒤 3초', btn_state(main)))
for k, v in res: print('%-34s -> %s' % (k, v))
p.kill()
