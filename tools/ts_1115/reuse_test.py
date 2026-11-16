import sys, os, re, socket, subprocess, time, glob
d, port, reuse = sys.argv[1], int(sys.argv[2]), sys.argv[3]
ini = os.path.join(d, 'EcsComA.ini'); raw = open(ini, 'rb').read()
raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % port, raw)
raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=18002', raw)
if reuse == '1':
    raw = raw.replace(b'[Host]', b'[Host]\r\nLISTEN_REUSE_ADDR=1', 1)
open(ini, 'wb').write(raw)
blocker = socket.socket(); blocker.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1); blocker.bind(('0.0.0.0', port)); blocker.listen(1)   # 이미 쓰고 있는 소켓(재사용 옵션 없음)
p = subprocess.Popen([os.path.join(d, 'TASK_LFC10_G1_ECSCOM.exe')], cwd=d)
time.sleep(30)
logs = ''
for f in glob.glob(os.path.join(d, 'ECSCOMALog', 'Srv', '*.Log')):
    logs += open(f, 'rb').read().decode('cp949', 'replace')
n = logs.count('열지 못했습니다')
print('LISTEN_REUSE_ADDR=%s -> 포트 열기 실패 로그 %d회' % (reuse, n))
p.kill(); blocker.close()
