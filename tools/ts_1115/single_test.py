import sys, os, re, socket, subprocess, time
d, port = sys.argv[1], int(sys.argv[2])
ini = os.path.join(d, 'EcsComA.ini'); raw = open(ini,'rb').read()
raw = re.sub(rb'(?m)^LocalPort=\d+', b'LocalPort=%d' % port, raw)
raw = re.sub(rb'(?m)^RemoteIP=.*$', b'RemoteIP=127.0.0.1', raw)
raw = re.sub(rb'(?m)^RemotePort=\d+', b'RemotePort=18002', raw)
raw = re.sub(rb'(?m)^SRV_MULTI_CONN=.*', b'SRV_MULTI_CONN=0', raw)
open(ini,'wb').write(raw)
p = subprocess.Popen([os.path.join(d,'TASK_LFC10_G1_ECSCOM.exe')], cwd=d)
t=time.time()
while time.time()-t<40:
    try: socket.create_connection(('127.0.0.1',port),timeout=1).close(); break
    except Exception: time.sleep(0.5)
time.sleep(8)
def msg(tag):
    body=b'\x02Z'+tag+b'\x03'; return b'WMS_MBX'.ljust(10)+b'%04d'%len(body)+b' '+body
a=socket.create_connection(('127.0.0.1',port),timeout=5); a.settimeout(8)
a.sendall(msg(b'0001')); ra=a.recv(26)
b=socket.create_connection(('127.0.0.1',port),timeout=5); b.settimeout(8)
b.sendall(msg(b'0002')); rb=b.recv(26)
try: ea=a.recv(10)
except Exception as e: ea=repr(e)
print('A first resp', len(ra)==26, '| B resp', len(rb)==26, '| A after B connects ->', ea)
print('RESULT', 'PASS' if len(ra)==26 and len(rb)==26 and ea in (b'', ) or 'Reset' in str(ea) else 'FAIL')
p.kill()
