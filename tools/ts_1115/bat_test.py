# -*- coding: utf-8 -*-
"""포트정리.bat 시험. 사용: python bat_test.py <시나리오: none|dead_yes|dead_no|kill_no|kill_yes|self_yes|self_no>
 bat 은 stdin 으로 답을 줄 수 있다(set /p). 시나리오마다 필요한 상태를 만든다."""
import sys, os, subprocess, time, re, ctypes, threading
HERE = os.path.dirname(os.path.abspath(__file__))
SP = os.path.dirname(HERE)
import importlib.util
spec = importlib.util.spec_from_file_location('ps', os.path.join(HERE, 'port_shots_lib.py'))
ps = importlib.util.module_from_spec(spec); spec.loader.exec_module(ps)

scn = sys.argv[1]
BAT_DIR = os.path.join(SP, 'battest')
BAT = os.path.join(BAT_DIR, '포트정리.bat')
REPRO = os.path.join(SP, 'hunt', 'Repro.exe')
DEV = subprocess.DEVNULL


def run_bat(port, answers, cwd=BAT_DIR, timeout=60):
    inp = ('\r\n'.join(answers) + '\r\n\r\n\r\n').encode('cp949') if answers else b'\r\n\r\n'
    r = subprocess.run(['cmd', '/c', os.path.join(cwd, '포트정리.bat'), str(port)], input=inp, capture_output=True, cwd=cwd, timeout=timeout)
    return r.stdout.decode('cp949', 'replace')


def kill_pings():
    subprocess.run('taskkill /F /IM PING.EXE', shell=True, capture_output=True)


def child_bound(port):
    code = ("import socket,time\ns=socket.socket();s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1);s.bind(('0.0.0.0',%d));s.listen(5)\n"
            "c=socket.create_connection(('127.0.0.1',%d));a,_=s.accept();c.close()\ntime.sleep(300)\n") % (port, port)
    p = subprocess.Popen([sys.executable, '-c', code], stdout=DEV, stderr=DEV, stdin=DEV)
    time.sleep(2)
    return p


def netstat(port):
    return subprocess.run('netstat -ano | findstr :%d' % port, shell=True, capture_output=True).stdout.decode('cp949', 'replace').strip() or '(없음)'


def show(title, out):
    print('=' * 20, title)
    print(out)


if scn == 'none':
    show('접속 없음', run_bat(18301, []))

elif scn in ('dead_yes', 'dead_no'):
    port = 18302
    subprocess.run([REPRO, str(port)], stdin=DEV, stdout=DEV, stderr=DEV)
    time.sleep(2)
    print('시작 전 netstat:', netstat(port))
    show('죽은 PID, 핸들 닫기 ' + ('예' if scn == 'dead_yes' else '아니요'), run_bat(port, ['Y'] if scn == 'dead_yes' else ['N']))
    print('끝난 뒤 netstat:', netstat(port))
    kill_pings()

elif scn in ('kill_no', 'kill_yes'):
    port = 18303
    c = child_bound(port)
    print('시작 전 netstat:', netstat(port))
    show('다른 PID TASKKILL ' + ('예' if scn == 'kill_yes' else '아니요'), run_bat(port, ['Y'] if scn == 'kill_yes' else ['N']))
    print('자식 살아 있음:', c.poll() is None)
    c.kill()

elif scn in ('self_yes', 'self_no'):
    port = 18304
    d = os.path.join(SP, 'T9')
    # T9 에 실제 HOST_TASK(최신 빌드)와 bat 을 둔다
    import shutil
    if not os.path.exists(os.path.join(d, 'TASK_LFC10_G1_ECSCOM.exe')):
        shutil.copytree(os.path.join(SP, 'hb3', 'TASK', 'WCS_TASK_HOST', 'bin', 'Debug'), d)
    shutil.copy(os.path.join(SP, 'hunt', 'HandleHunter.exe'), d)
    shutil.copy(BAT, os.path.join(d, '포트정리.bat'))
    ini = os.path.join(d, 'EcsComA.ini'); raw = open(ini, 'rb').read()
    raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % port, raw)
    raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
    raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=18002', raw)
    open(ini, 'wb').write(raw)
    host = subprocess.Popen([os.path.join(d, 'TASK_LFC10_G1_ECSCOM.exe')], cwd=d)
    time.sleep(16)

    def confirm_closer():           # HOST 의 종료 확인창이 뜨면 확인을 눌러 준다(사람이 하는 일)
        t0 = time.time()
        while time.time() - t0 < 30 and host.poll() is None:
            time.sleep(0.5)
            for h, k, t in ps.wins(host.pid):
                if k == '#32770':
                    b1 = ps.user32.FindWindowExW(h, None, 'Button', None)
                    ps.user32.PostMessageW(b1, 0xF5, 0, 0)
    if scn == 'self_yes':
        threading.Thread(target=confirm_closer, daemon=True).start()
    show('자기 자신 종료 ' + ('예' if scn == 'self_yes' else '아니요'), run_bat(port, ['Y'] if scn == 'self_yes' else ['N'], cwd=d, timeout=90))
    time.sleep(2)
    print('HOST_TASK 아직 실행 중:', host.poll() is None)
    if host.poll() is None: host.kill()
