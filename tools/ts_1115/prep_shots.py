# -*- coding: utf-8 -*-
import os, shutil, re
snip = os.path.dirname(os.path.abspath(__file__))
sp = os.path.dirname(snip)
name = 'TASK_LFC10_G1_ECSCOM.exe'

# 하네스 폴더: 실제 프로그램과 같은 프로세스 이름으로 보이게 한다
for folder, src, hunter in (('harness_a', 'harness/h.exe', True), ('harness_n', 'harness/h_noadmin.exe', True), ('harness_nohunter', 'harness/h.exe', False)):
    d = os.path.join(sp, folder)
    os.makedirs(d, exist_ok=True)
    shutil.copy(os.path.join(sp, src), os.path.join(d, name))
    hh = os.path.join(d, 'HandleHunter.exe')
    if hunter: shutil.copy(os.path.join(sp, 'hunt', 'HandleHunter.exe'), hh)
    elif os.path.exists(hh): os.remove(hh)

p = os.path.join(snip, 'port_shots.py')
s = open(p, encoding='utf-8').read()
s = s.replace("H_ADMIN = os.path.join(SP, 'harness', 'h.exe')", "H_ADMIN = os.path.join(SP, 'harness_a', 'TASK_LFC10_G1_ECSCOM.exe')")
s = s.replace("H_NOADM = os.path.join(SP, 'harness', 'h_noadmin.exe')", "H_NOADM = os.path.join(SP, 'harness_n', 'TASK_LFC10_G1_ECSCOM.exe')")
s = s.replace("H_NOHUNT = os.path.join(SP, 'harness_nohunter', 'h.exe')", "H_NOHUNT = os.path.join(SP, 'harness_nohunter', 'TASK_LFC10_G1_ECSCOM.exe')")
# 모든 시나리오가 같은 포트(현장 9911)를 쓴다 - 시나리오 사이에 정리한다
s = re.sub(r"PORT = \{[^}]*\}", "PORT = {k: 9911 for k in ('idle','none','kill','dead','self','noadmin','nohunter','closewait')}", s)
s = s.replace("    port = 18209\n", "    port = 9911\n")
# 캡처 실패는 건너뛴다
s = s.replace("def shot(h, path):\n    r = wt.RECT(); user32.GetWindowRect(h, ctypes.byref(r))\n    w, hh = r.right - r.left, r.bottom - r.top\n",
              "def shot(h, path):\n    r = wt.RECT(); user32.GetWindowRect(h, ctypes.byref(r))\n    w, hh = r.right - r.left, r.bottom - r.top\n    if w <= 0 or hh <= 0: return\n")
open(p, 'w', encoding='utf-8').write(s)
print('ok')
