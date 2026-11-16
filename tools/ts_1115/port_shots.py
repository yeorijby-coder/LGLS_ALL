# -*- coding: utf-8 -*-
"""[포트 정리] 모든 메시지 박스 캡처. 사용: python port_shots.py <시나리오> <출력폴더>
 시나리오별로 필요한 상태를 만들고 하네스(h.exe / h_noadmin.exe)를 띄워 뜨는 모든 창(netstat 창·메시지 박스)을 PNG 로 저장한다.
 선택은 시나리오 안에 정해 두었다."""
import sys, os, subprocess, time, socket, ctypes, ctypes.wintypes as wt
from ctypes import Structure, c_int32, c_uint16, c_uint32, sizeof, byref, create_string_buffer
from PIL import Image

scn, outdir = sys.argv[1], sys.argv[2]
os.makedirs(outdir, exist_ok=True)
HERE = os.path.dirname(os.path.abspath(__file__))
SP = os.path.dirname(HERE)
H_ADMIN = os.path.join(SP, 'harness_a', 'TASK_LFC10_G1_ECSCOM.exe')
H_NOADM = os.path.join(SP, 'harness_n', 'TASK_LFC10_G1_ECSCOM.exe')
H_NOHUNT = os.path.join(SP, 'harness_nohunter', 'TASK_LFC10_G1_ECSCOM.exe')
REPRO = os.path.join(SP, 'hunt', 'Repro.exe')

user32 = ctypes.windll.user32
gdi = ctypes.windll.gdi32
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)
DEVNULL = subprocess.DEVNULL


def sh(c):
    return subprocess.run(c, shell=True, capture_output=True).stdout.decode('cp949', 'replace')


def wins(pid):
    out = []
    def cb(h, l):
        pp = wt.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(pp))
        if pp.value == pid and user32.IsWindowVisible(h):
            k = ctypes.create_unicode_buffer(100); user32.GetClassNameW(h, k, 100)
            t = ctypes.create_unicode_buffer(300); user32.GetWindowTextW(h, t, 300)
            out.append((h, k.value, t.value))
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


def shot(h, path):
    r = wt.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
    w, hh = r.right - r.left, r.bottom - r.top
    if w <= 0 or hh <= 0: return
    if w <= 0 or hh <= 0: return
    hdc = user32.GetDC(0); mdc = gdi.CreateCompatibleDC(hdc); bmp = gdi.CreateCompatibleBitmap(hdc, w, hh)
    gdi.SelectObject(mdc, bmp); user32.PrintWindow(h, mdc, 2)

    class BIH(Structure):
        _fields_ = [('biSize', c_uint32), ('biWidth', c_int32), ('biHeight', c_int32), ('biPlanes', c_uint16), ('biBitCount', c_uint16),
                    ('biCompression', c_uint32), ('biSizeImage', c_uint32), ('biXPels', c_int32), ('biYPels', c_int32), ('biClrUsed', c_uint32), ('biClrImp', c_uint32)]
    bi = BIH(sizeof(BIH), w, -hh, 1, 32, 0, 0, 0, 0, 0, 0)
    buf = create_string_buffer(w * hh * 4)
    gdi.GetDIBits(mdc, bmp, 0, hh, buf, byref(bi), 0)
    Image.frombuffer('RGBA', (w, hh), buf, 'raw', 'BGRA', 0, 1).convert('RGB').save(path)
    gdi.DeleteObject(bmp); gdi.DeleteDC(mdc); user32.ReleaseDC(0, hdc)


def run_harness(exe, args, choices, prefix, kill_children=True):
    """exe 를 띄워 뜨는 창을 모두 캡처하고, 예/아니요 창은 choices 순서대로 누른다."""
    p = subprocess.Popen([exe] + args, stdout=subprocess.PIPE, stderr=DEVNULL)
    seen = set(); n = 0; shot_net = False
    t0 = time.time()
    log = []
    while time.time() - t0 < 40 and p.poll() is None:
        time.sleep(0.4)
        for h, k, t in wins(p.pid):
            if h in seen: continue
            if k.startswith('WindowsForms') and '포트 정리 - netstat' in t and not shot_net:
                time.sleep(0.8)
                shot(h, os.path.join(outdir, prefix + '_netstat창.png')); shot_net = True; seen.add(h)
                log.append(('netstat창', ''))
            elif k == '#32770':
                seen.add(h); n += 1
                time.sleep(0.6)
                tx = '\n'.join(texts(h))
                fn = os.path.join(outdir, '%s_%02d.png' % (prefix, n))
                shot(h, fn)
                b1 = user32.FindWindowExW(h, None, 'Button', None)
                b2 = user32.FindWindowExW(h, b1, 'Button', None)
                if b2:
                    ch = choices.pop(0) if choices else 'no'
                    log.append((fn, tx, '예' if ch == 'yes' else '아니요'))
                    user32.PostMessageW(b1 if ch == 'yes' else b2, 0xF5, 0, 0)
                else:
                    log.append((fn, tx, '확인'))
                    user32.PostMessageW(b1, 0xF5, 0, 0)
                time.sleep(1.0)
    for h, k, t in wins(p.pid):
        user32.PostMessageW(h, 0x10, 0, 0)
    time.sleep(1)
    if p.poll() is None: p.kill()
    out = p.stdout.read().decode('utf-8', 'replace')
    return log, out


def cleanup():
    sh('taskkill /F /IM PING.EXE')


def show(log, out):
    for e in log:
        print(e[0], '|', (e[1] or '').replace('\n', ' / ')[:140], '|', e[2] if len(e) > 2 else '')
    print('하네스 출력:', out.strip().replace('\n', ' / ')[:200])


PORT = {k: 9911 for k in ('idle','none','kill','dead','self','noadmin','nohunter','closewait')}

if scn == 'none':          # 남은 접속 없음
    log, out = run_harness(H_ADMIN, [str(PORT['none'])], [], 'S01_접속없음')
    show(log, out)

elif scn == 'kill':        # 다른 프로세스가 CLOSE_WAIT 를 쥠: TASKKILL 질문 아니요 → 포트 정리 질문 예(결과)
    port = PORT['kill']
    code = ("import socket,time\ns=socket.socket();s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1);s.bind(('0.0.0.0',%d));s.listen(5)\n"
            "c=socket.create_connection(('127.0.0.1',%d));a,_=s.accept();c.close()\ntime.sleep(300)\n") % (port, port)
    child = subprocess.Popen([sys.executable, '-c', code], stdout=DEVNULL, stderr=DEVNULL)
    time.sleep(2)
    log, out = run_harness(H_ADMIN, [str(port)], ['no', 'yes'], 'S02_TASKKILL아니요_정리예')
    show(log, out)
    child.kill()
    time.sleep(1)
    child = subprocess.Popen([sys.executable, '-c', code.replace('18203', '18203')], stdout=DEVNULL, stderr=DEVNULL)
    time.sleep(2)
    log, out = run_harness(H_ADMIN, [str(port)], ['yes'], 'S03_TASKKILL예')
    show(log, out)
    child.kill()

elif scn == 'dead':        # 죽은 PID 가 남긴 LISTENING: 핸들 닫기 예(결과) / 아니요(대상 없음 안내)
    port = PORT['dead']
    subprocess.run([REPRO, str(port)], stdin=DEVNULL, stdout=DEVNULL, stderr=DEVNULL)
    time.sleep(2)
    log, out = run_harness(H_ADMIN, [str(port)], ['no'], 'S04_핸들닫기아니요')
    show(log, out)
    log, out = run_harness(H_ADMIN, [str(port)], ['yes'], 'S05_핸들닫기예')
    show(log, out)
    cleanup()

elif scn == 'nohunter':    # HandleHunter.exe 가 없을 때
    port = PORT['nohunter']
    subprocess.run([REPRO, str(port)], stdin=DEVNULL, stdout=DEVNULL, stderr=DEVNULL)
    time.sleep(2)
    log, out = run_harness(H_NOHUNT, [str(port)], [], 'S06_핸들도구없음')
    show(log, out)
    cleanup()

elif scn == 'noadmin':     # 관리자 아님 경고
    port = PORT['noadmin']
    code = ("import socket,time\ns=socket.socket();s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1);s.bind(('0.0.0.0',%d));s.listen(5)\n"
            "c=socket.create_connection(('127.0.0.1',%d));a,_=s.accept();c.close()\ntime.sleep(300)\n") % (port, port)
    child = subprocess.Popen([sys.executable, '-c', code], stdout=DEVNULL, stderr=DEVNULL)
    time.sleep(2)
    log, out = run_harness(H_NOADM, [str(port)], ['no'], 'S07_관리자아님')
    show(log, out)
    child.kill()

elif scn == 'self':        # 구동 중인 프로그램 자신: 종료 확인 예 → 마지막에 종료 / 아니요
    port = PORT['self']
    log, out = run_harness(H_ADMIN, [str(port), 'self'], ['yes'], 'S08_자기종료예')
    show(log, out)
    log, out = run_harness(H_ADMIN, [str(port), 'self'], ['no'], 'S09_자기종료아니요')
    show(log, out)

elif scn == 'combo':       # 자손 핸들 + 자기 자신 + 다른 프로세스: 모든 질문이 한 번에
    port = 9911
    code = ("import socket,time\ns=socket.socket();s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1);s.bind(('127.0.0.1',%d));s.listen(5)\n"
            "time.sleep(300)\n") % port
    # 자기 자신이 0.0.0.0 에 리슨하면 python 의 127.0.0.1 바인드와 충돌하지 않게 python 은 ReuseAddr
    child = subprocess.Popen([sys.executable, '-c', code], stdout=DEVNULL, stderr=DEVNULL)
    time.sleep(2)
    log, out = run_harness(H_ADMIN, [str(port), 'self'], ['yes', 'yes'], 'S10_자기종료_다른프로세스')
    show(log, out)
    child.kill()
