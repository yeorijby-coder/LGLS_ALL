# -*- coding: utf-8 -*-
"""[포트 정리] 버튼 화면 시험. HOST 가 쓰는 포트에 python 이 CLOSE_WAIT 접속(+리스너)을 만들어 두고 버튼을 눌러 본다.
사용: python port_ui_test.py <exe폴더> <포트> <캡처저장경로.png>"""
import sys, os, re, socket, subprocess, time, ctypes, ctypes.wintypes as wt

d, port, shot = sys.argv[1], int(sys.argv[2]), sys.argv[3]
ini = os.path.join(d, 'EcsComA.ini'); raw = open(ini, 'rb').read()
raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % port, raw)
raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=18002', raw)
open(ini, 'wb').write(raw)

# 포트를 python 이 쥐고 있다: 리스너 + 상대가 닫은 접속(CLOSE_WAIT)
srv = socket.socket(); srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
srv.bind(('0.0.0.0', port)); srv.listen(5)
c = socket.create_connection(('127.0.0.1', port)); a, _ = srv.accept(); c.close()

p = subprocess.Popen([os.path.join(d, 'TASK_LFC10_G1_ECSCOM.exe')], cwd=d)
time.sleep(15)

user32 = ctypes.windll.user32
WNDENUM = ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)


def wins(pid):
    out = []
    def cb(h, l):
        pp = wt.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(pp))
        if pp.value == pid and user32.IsWindowVisible(h):
            b = ctypes.create_unicode_buffer(300); user32.GetWindowTextW(h, b, 300)
            k = ctypes.create_unicode_buffer(100); user32.GetClassNameW(h, k, 100)
            out.append((h, k.value, b.value))
        return True
    user32.EnumWindows(WNDENUM(cb), 0); return out


def children(h):
    out = []
    def cb(c, l):
        t = ctypes.create_unicode_buffer(8000); user32.SendMessageW(c, 0x000D, 8000, t)   # WM_GETTEXT
        k = ctypes.create_unicode_buffer(100); user32.GetClassNameW(c, k, 100)
        out.append((c, k.value, t.value)); return True
    user32.EnumChildWindows(h, WNDENUM(cb), 0); return out


def find_child(root, text):
    for c, k, t in children(root):
        if t == text: return c


main = [h for h, k, t in wins(p.pid) if not k.startswith('#32770') and 'netstat' not in t][0]
btn = find_child(main, '포트 정리')
print('[포트 정리] 버튼 찾음:', bool(btn))
user32.PostMessageW(btn, 0xF5, 0, 0)     # BM_CLICK
time.sleep(4)

for h, k, t in wins(p.pid):
    if 'netstat' in t:
        print('=== 창:', t)
        for c, kk, tt in children(h):
            if kk.startswith('WindowsForms10.EDIT') or kk == 'Edit':
                print(tt)
        # 캡처
        sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
        r = wt.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
        w, hh = r.right - r.left, r.bottom - r.top
        gdi = ctypes.windll.gdi32
        hdc = user32.GetDC(0); mdc = gdi.CreateCompatibleDC(hdc); bmp = gdi.CreateCompatibleBitmap(hdc, w, hh)
        gdi.SelectObject(mdc, bmp); user32.PrintWindow(h, mdc, 2)
        from ctypes import Structure, c_int32, c_uint16, c_uint32, sizeof, byref, create_string_buffer
        class BIH(Structure):
            _fields_ = [('biSize', c_uint32), ('biWidth', c_int32), ('biHeight', c_int32), ('biPlanes', c_uint16), ('biBitCount', c_uint16),
                        ('biCompression', c_uint32), ('biSizeImage', c_uint32), ('biXPels', c_int32), ('biYPels', c_int32), ('biClrUsed', c_uint32), ('biClrImp', c_uint32)]
        bi = BIH(sizeof(BIH), w, -hh, 1, 32, 0, 0, 0, 0, 0, 0)
        buf = create_string_buffer(w * hh * 4)
        gdi.GetDIBits(mdc, bmp, 0, hh, buf, byref(bi), 0)
        try:
            from PIL import Image
            Image.frombuffer('RGBA', (w, hh), buf, 'raw', 'BGRA', 0, 1).convert('RGB').save(shot)
            print('캡처 저장:', shot)
        except Exception as e:
            print('PIL 없음/저장 실패', e)
        break

for h, k, t in wins(p.pid):
    if k.startswith('#32770'):
        print('=== 메시지 박스:', t)
        for c, kk, tt in children(h):
            if tt and kk == 'Static': print(tt)
        btnY = user32.FindWindowExW(h, None, 'Button', None)
        # 첫 버튼(예 또는 확인) 대신 두 번째 버튼(아니요)이 있으면 그걸 누른다
        second = user32.FindWindowExW(h, btnY, 'Button', None)
        user32.PostMessageW(second if second else btnY, 0xF5, 0, 0)
time.sleep(2)

# 정리
for h, k, t in wins(p.pid):
    if 'netstat' in t: user32.PostMessageW(h, 0x10, 0, 0)
time.sleep(1)
p.kill(); a.close(); srv.close()
print('(끝)')
