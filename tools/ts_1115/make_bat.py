# -*- coding: utf-8 -*-
import os, sys
snip = os.path.dirname(os.path.abspath(__file__))
src = open(os.path.join(snip, 'portclean_bat.txt'), encoding='utf-8').read()
src = src.replace('net session >nul 2>&1 && set "ISADMIN=1"', 'fltmc >nul 2>&1 && set "ISADMIN=1"')
data = src.replace('\r\n', '\n').replace('\n', '\r\n').encode('cp949')      # 먼저 인코딩 검증 후 쓴다(없는 글자면 여기서 예외)
for dst in sys.argv[1:]:
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    with open(dst, 'wb') as f:
        f.write(data)
    print('wrote', dst, len(data))
