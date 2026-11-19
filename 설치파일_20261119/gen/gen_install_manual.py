# -*- coding: utf-8 -*-
"""Client 설치 매뉴얼 - 운영체제만 설치된 새 PC 에 LGLS CLIENT 를 까는 절차.
   근거 : 설치파일_20261119/Client_설치 (gen_client_setup_1119.py 로 생성), 개발 PC 시험 결과
   실행 : python gen_install_manual.py   → 설치파일_20261119/설치_매뉴얼_20261119.docx"""
import os, sys
HERE = os.path.dirname(os.path.abspath(__file__))
BASE = os.path.dirname(HERE)
sys.path.insert(0, r'D:\project\LGLS\Renewal\docs\산출물_20260927\gen')
import gen_common as G
from gen_common import (Document, Pt, Cm, RGBColor, WD_ALIGN_PARAGRAPH, qn, OxmlElement,
                        NAVY, GRAY, AUTHOR, new_doc, shade, table, para, bullets, numbered, image)
G.BASE = BASE; G.OUT = BASE; G.SHOTS = os.path.join(HERE, 'shots'); G.SCRATCH = HERE
G.DATE = '2026-11-19'; G.VER = '1.0'; G.BASE_HISTORY = []
G.FINAL_NOTE = '최초 작성 - 운영체제만 설치된 새 PC 에 LGLS CLIENT 를 설치하는 절차'


def note(d, t):
    return para(d, '※ ' + t, size=9, color=GRAY)


def code(d, lines):
    t = d.add_table(rows=1, cols=1); t.style = 'Table Grid'
    c = t.rows[0].cells[0]; shade(c, 'F2F2F2'); c.text = ''
    for k, ln in enumerate(lines):
        p = c.paragraphs[0] if k == 0 else c.add_paragraph()
        r = p.add_run(ln); r.font.name = 'Consolas'; r.font.size = Pt(9)
    d.add_paragraph()


def steps(d, items):
    for i, t in enumerate(items, 1):
        para(d, '%d. %s' % (i, t), indent=0.6)


d = new_doc('LGLS CLIENT 설치 매뉴얼', '운영체제만 설치된 새 PC 에 운전 화면(Client)을 설치하는 방법')

# ───────────────────────── 1
d.add_heading('1. 개요', 1)
para(d, '이 문서는 포맷 직후처럼 운영체제(Windows)만 들어 있는 PC 에 LGLS 운전 화면(Client)을 설치하는 절차입니다. '
        '설치 꾸러미 폴더 하나(설치파일_20261119\\Client_설치)를 PC 에 복사하고 Setup.bat 을 관리자 권한으로 실행하면 됩니다.')
bullets(d, [
    '설치 대상은 서버(DB)와 네트워크가 연결된 PC 입니다. 서버 쪽 프로그램(HOST_TASK, IO_TASK, WCS_TASK_CV)과 DB 는 이 꾸러미로 설치하지 않습니다.',
    '설치가 끝나면 바탕화면에 [LGLS CLIENT] 가 생기고, 누르면 내려받기 프로그램(Ecs.exe)이 서버와 파일 버전을 견준 뒤 Client 본체(EcsMain.exe)를 띄웁니다.',
    '운영체제에 따로 설치할 것이 없도록 필요한 구성요소(Visual C++ 재배포 패키지, .NET Framework)는 꾸러미에 들어 있고, 없을 때만 설치합니다.',
    '이전 꾸러미(2026-10-15)는 쓰지 마십시오. 오래된 프로그램 파일이 들어 있고, 삭제 파일(Uninstall.bat)과 서버 주소 변경 도우미(Set-DbServer.ps1)가 빠져 있었습니다. 이번에 모두 새로 만들었습니다.',
])

# ───────────────────────── 2
d.add_heading('2. 설치 전에 준비할 것', 1)
d.add_heading('2.1 PC', 2)
table(d, ['항목', '조건'], [
    ['운영체제', 'Windows 10 / 11 (64비트 권장). Windows 10 이상에는 .NET Framework 4.8 이 이미 들어 있습니다.'],
    ['계정', '관리자 권한이 있는 계정. 설치 중 구성요소를 설치하고 바탕화면 바로가기를 만들기 때문입니다.'],
    ['언어 설정', '제어판 - 지역 - 관리자 옵션 - "유니코드를 지원하지 않는 프로그램용 언어" 를 한국어로 맞춥니다. 설정 파일(ini)의 한글을 읽는 데 필요합니다.'],
    ['디스크', '약 200 MB 이상 여유 (설치 위치 기본값은 C:\\LGLS\\CLIENT)'],
    ['화면', 'Full HD 이상 권장. 4K 에서 글자가 작거나 크면 설치 폴더의 화면배율_방식.bat 을 쓸 수 있습니다.'],
], widths=[3.0, 14.5], font=9)

d.add_heading('2.2 서버 정보 (설치 담당자가 미리 확인)', 2)
table(d, ['항목', '내용', '기본값'], [
    ['서버 IP', 'DB(SQL Server)가 있는 PC 의 주소. 이름 있는 인스턴스는 PC이름\\SQLEXPRESS 처럼 적습니다.', '127.0.0.1 (시험용, 현장에서는 반드시 변경)'],
    ['포트', 'SQL Server 포트. 이름 있는 인스턴스를 적으면 쓰지 않습니다.', '1433'],
    ['데이터베이스', 'Client 가 쓰는 DB 이름', 'LGLS_MCS_IO'],
    ['계정', '설치 꾸러미 기본값은 USERID / USERPASSWORD = LGLS_IO. 현장 계정이 다르면 설치 뒤 Ecs.ini 를 고칩니다. Windows 인증을 쓰려면 TRUSTED=1.', 'Ecs.ini 의 [DB_2]'],
], widths=[3.0, 9.0, 5.5], font=9)

d.add_heading('2.3 서버 쪽 조건 (새 PC 가 서버에 붙으려면)', 2)
bullets(d, [
    'SQL Server 에서 TCP/IP 접속이 켜져 있고, 서버 방화벽이 해당 포트(기본 1433)를 허용해야 합니다.',
    '위 DB 계정으로 SQL 로그인이 가능해야 합니다.',
    '설치 전에 새 PC 에서 서버까지 닿는지 확인해 두면 좋습니다. 명령 프롬프트에서 아래를 실행해 "True" 가 나오면 포트가 열려 있는 것입니다.',
])
code(d, ['powershell -NoProfile -Command "(New-Object Net.Sockets.TcpClient).Connect(\'서버IP\', 1433); \'True\'"'])
note(d, '위 확인은 PowerShell 만 쓰며 따로 설치할 것이 없습니다. 오류가 나면 네트워크 또는 방화벽을 먼저 해결해야 합니다.')

# ───────────────────────── 3
d.add_heading('3. 설치 꾸러미 구성', 1)
para(d, '설치파일_20261119\\Client_설치 폴더를 통째로 USB 등으로 새 PC 에 복사합니다 (약 107 MB). 폴더 안의 파일은 아래와 같습니다.')
table(d, ['파일/폴더', '역할'], [
    ['Setup.bat', '설치 실행 파일. 관리자 권한으로 실행합니다.'],
    ['Client\\', '설치될 프로그램 전체 (Ecs.exe, EcsMain.exe, 라이브러리, 화면 정의, 다국어 문구 rc_resource, 설정 ini). 619개 파일.'],
    ['Prerequisites\\', 'vc_redist.x86.exe (Visual C++ 14.44), ndp48-web.exe (.NET Framework 4.8 웹 설치). 없을 때만 설치합니다.'],
    ['Set-DbServer.ps1', '서버 주소를 Ecs.ini 와 WmsDown.ini 에 적어 주는 도우미. 나중에 서버가 바뀔 때도 씁니다.'],
    ['Uninstall.bat', '제거 파일. 설치 폴더에도 복사됩니다.'],
    ['Release-Fonts.ps1, Force-ReleaseFonts.ps1, 폰트잠금_풀기.bat', '옛 Client 가 잠가 둔 글꼴 파일을 푸는 도구. 새 PC 에서는 쓸 일이 없습니다.'],
    ['읽어보세요.txt, 확인하는_법.txt', '짧은 안내문'],
], widths=[5.5, 12.0], font=9)
note(d, 'Ecs.exe 는 "내려받기 프로그램"이고 EcsMain.exe 가 Client 본체입니다. 바로가기와 시작은 Ecs.exe 로 합니다.')

# ───────────────────────── 4
d.add_heading('4. 설치 절차', 1)
steps(d, [
    '꾸러미 폴더를 새 PC 의 아무 곳(예: 바탕화면)에 복사합니다. 압축 파일이면 먼저 풉니다.',
    'Setup.bat 을 마우스 오른쪽 단추로 눌러 [관리자 권한으로 실행] 을 고릅니다. 관리자 권한이 없으면 안내만 나오고 끝납니다.',
    '질문에 답합니다. 기본값으로 두려면 Enter 만 누릅니다 (아래 표).',
    '"이대로 설치할까요? (Y/n)" 에서 Enter(Y)를 누르면 설치가 진행됩니다. 5단계가 차례로 표시됩니다.',
    '"설치가 끝났습니다" 가 나오면 아무 키나 눌러 창을 닫습니다.',
])
table(d, ['질문', '입력', '비고'], [
    ['설치 위치', 'Enter = C:\\LGLS\\CLIENT', '다른 곳에 두려면 경로를 적습니다.'],
    ['서버 IP', '서버 주소', '이름 있는 인스턴스는 PC이름\\SQLEXPRESS (포트는 묻더라도 쓰지 않음)'],
    ['포트', 'Enter = 1433', '포트를 바꾼 서버만 적습니다 (예: 1435).'],
    ['데이터베이스', 'Enter = LGLS_MCS_IO', ''],
], widths=[3.5, 5.0, 9.0], font=9)

d.add_heading('4.1 설치가 하는 일 (5단계)', 2)
table(d, ['단계', '내용', '이미 있을 때'], [
    ['[1/5]', 'Visual C++ 재배포 패키지 설치. Client 본체가 mfc140u.dll 을 쓰며, 없으면 "mfc140u.dll 을 찾을 수 없습니다" 가 나고 아예 뜨지 않습니다.', '건너뜁니다.'],
    ['[2/5]', '.NET Framework 확인. 내려받기 프로그램(Ecs.exe)이 .NET 으로 만들어져 있습니다.', 'Windows 10 이상은 건너뜁니다. 없을 때만 설치하며 이때는 인터넷이 필요합니다.'],
    ['[3/5]', '프로그램 복사. 복사 뒤 원본과 이름으로 대조해 빠진 파일이 없는지 확인합니다.', 'Ecs.ini 와 WmsDown.ini 는 덮어쓰지 않습니다(새 것은 .new 로 남김).'],
    ['[4/5]', '서버 주소를 Ecs.ini [DB_2] 와 WmsDown.ini [DB Server] 에 적습니다. 주석은 건드리지 않습니다.', '-'],
    ['[5/5]', '바탕화면(모든 사용자)에 [LGLS CLIENT] 바로가기를 만듭니다.', '-'],
], widths=[1.8, 10.2, 5.5], font=9)

d.add_heading('4.2 화면 예 (서버 IP 에 10.20.30.40 을 적은 시험 설치)', 2)
code(d, [
    ' ============================================================',
    '  LGLS CLIENT 설치',
    ' ============================================================',
    '  [ 설치 위치 ]  설치 위치 [C:\\LGLS\\CLIENT] :',
    '  [ DB 서버 ]    서버 IP [127.0.0.1] : 10.20.30.40',
    '                 포트 [1433] :    데이터베이스 [LGLS_MCS_IO] :',
    ' ------------------------------------------------------------',
    '  설치 위치   : C:\\LGLS\\CLIENT',
    '  DB 서버     : 10.20.30.40',
    '  데이터베이스 : LGLS_MCS_IO',
    ' ------------------------------------------------------------',
    '  이대로 설치할까요? (Y/n) :',
    '  [1/5] Visual C++ 재배포 패키지를 확인합니다...',
    '  [2/5] .NET Framework 를 확인합니다...',
    '  [3/5] 프로그램을 복사합니다...   619개 파일이 복사되었습니다.   모두 들어갔습니다.',
    '  [4/5] DB 서버 주소를 설정에 적습니다...  Ecs.ini [DB_2] SERVER=10.20.30.40',
    '  [5/5] 바로가기를 만듭니다...',
    '  설치가 끝났습니다.',
])
note(d, '위 화면은 이 PC 에 이미 구성요소가 있어 [1/5], [2/5] 가 "이미 들어 있습니다. 건너뜁니다." 로 나온 시험 결과를 줄인 것입니다. 새 PC 에서는 [1/5] 에서 재배포 패키지를 설치하느라 1~2분 걸립니다.')

# ───────────────────────── 5
d.add_heading('5. 설치 뒤 확인', 1)
steps(d, [
    '바탕화면의 [LGLS CLIENT] 를 누릅니다. 내려받기 창(Ecs.exe)이 잠시 보인 뒤 Client 화면이 뜹니다.',
    '제목줄에 [DB:데이터베이스@서버] 가 서버 주소로 표시되고, 화면에 설비(C/V, S/C)가 나타나면 정상입니다.',
    '서버의 TASK 가 돌고 있으면 설비 상태가 실시간으로 바뀝니다. TASK 가 안 돌면 화면은 뜨지만 상태가 정지해 있을 수 있습니다.',
])
image(d, os.path.join(G.SHOTS, 'client_first.png'), 16.0, '설치 직후 첫 실행 화면 (시험 PC, DB 연결 표시가 제목줄에 보임)')
note(d, '제목줄의 [Build:날짜] 가 이 꾸러미의 Client 본체 빌드 시각입니다 (이번 꾸러미: 2026.11.12 23:11).')

d.add_heading('5.1 내려받기 프로그램이 하는 일', 2)
bullets(d, [
    'Ecs.exe 는 서버 DB 의 배포 목록(DN_MST)과 이 PC 의 파일을 견주어 바뀐 파일만 내려받고, 이어서 EcsMain.exe 를 실행합니다.',
    '서버 DB 에 배포 목록이 없거나 새 파일이 없으면 받을 것이 없으므로, 설치된 파일 그대로 Client 가 뜹니다 (시험 서버가 이 경우였고 정상으로 떴습니다).',
    '앞으로 프로그램을 고치면 PC 마다 다시 설치하지 않고, 서버에 한 번 올리면(WmsUp.exe) 각 PC 가 다음 실행 때 받아 갑니다.',
])

# ───────────────────────── 6
d.add_heading('6. 서버 주소 변경, 재설치, 제거', 1)
table(d, ['하려는 일', '방법'], [
    ['서버 주소가 바뀜', 'Setup.bat 을 다시 실행해 새 주소를 적습니다. 쓰던 설정은 그대로 두고 주소만 바꿉니다. 또는 Ecs.ini 의 [DB_2] 를 고친 뒤 설치 폴더에서 Set-DbServer.ps1 을 한 번 실행하면 WmsDown.ini 가 따라옵니다.'],
    ['계정/Windows 인증 변경', 'Ecs.ini [DB_2] 의 USERID, USERPASSWORD 또는 TRUSTED 를 고친 뒤 Set-DbServer.ps1 실행. 설정은 Ecs.ini 하나를 기준으로 삼습니다.'],
    ['재설치/업데이트', 'Client 가 켜져 있으면 닫고 Setup.bat 을 다시 실행합니다. Ecs.ini, WmsDown.ini 는 보존되고 새 것은 .new 로 남습니다. 새 항목이 필요하면 .new 를 보고 옮깁니다.'],
    ['제거', '설치 폴더의 Uninstall.bat 을 관리자 권한으로 실행. 지울 대상을 보여 주고 YES 를 적어야 지웁니다. 설정 파일은 원하면 따로 남깁니다. 재배포 패키지와 .NET 은 다른 프로그램도 쓰므로 남깁니다.'],
], widths=[4.0, 13.5], font=9)
code(d, ['powershell -NoProfile -ExecutionPolicy Bypass -File "C:\\LGLS\\CLIENT\\Set-DbServer.ps1" -InstDir "C:\\LGLS\\CLIENT"'])

# ───────────────────────── 7
d.add_heading('7. 문제가 생겼을 때', 1)
table(d, ['증상', '원인', '조치'], [
    ['"관리자 권한으로 실행해 주세요" 가 나오고 끝남', 'Setup.bat 을 그냥 더블클릭함', '오른쪽 단추 - 관리자 권한으로 실행'],
    ['실행하자마자 "mfc140u.dll 을 찾을 수 없습니다"', 'Visual C++ 재배포 패키지가 없음', 'Prerequisites\\vc_redist.x86.exe 를 실행해 설치한 뒤 다시 실행'],
    ['[2/5] 에서 .NET 설치가 안 됨', '인터넷이 없는 Windows 10 미만 PC', 'Windows 10 이상 PC 를 쓰거나, .NET Framework 4.8 오프라인 설치 파일을 따로 설치'],
    ['Client 가 뜨지만 DB 연결 실패 / 화면이 비어 있음', '서버 주소·포트 오류, 방화벽, 계정 오류', '2.3 의 포트 확인을 먼저 합니다. 이어서 Ecs.ini [DB_2] 의 SERVER, 계정을 확인하고 Set-DbServer.ps1 실행'],
    ['내려받기 창에서 멈추거나 오류', 'WmsDown.ini 의 서버 주소가 Ecs.ini 와 다름', 'Set-DbServer.ps1 을 실행해 맞춥니다'],
    ['글자가 깨져 보임', '"유니코드 미지원 프로그램용 언어" 가 한국어가 아님', '2.1 의 언어 설정을 한국어로 바꾸고 재부팅'],
    ['복사 중 "액세스가 거부되었습니다"', '옛 Client 가 잠가 둔 글꼴 파일 (새 PC 에서는 거의 없음)', 'Client 를 모두 닫고, 필요하면 폰트잠금_풀기.bat 실행. 그래도 안 되면 설치가 옆 폴더(_1, _2…)에 자동으로 설치함'],
    ['4K 에서 화면이 너무 작거나 큼', '화면 배율', '설치 폴더의 화면배율_방식.bat 안내 참조 (Ecs.ini UI_SCALE)'],
], widths=[4.8, 4.7, 8.0], font=8.5)

# ───────────────────────── 8
d.add_heading('8. 검증 결과와 한계', 1)
para(d, '이 꾸러미는 개발 PC 에서 다음을 확인했습니다.', bold=True)
table(d, ['확인 항목', '결과'], [
    ['실행 파일이 필요로 하는 DLL 점검 (EcsMain, EcsLib, DciLib, XmlLib, BuilderLib, SPR32DU70, QUvc_dll, Ecs)',
     'mfc140u, MSVCP140, VCRUNTIME140 (재배포 패키지에 포함), Windows 기본 DLL, 꾸러미에 들어 있는 DLL 로 모두 충족. 디버그 DLL 에 의존하는 것 없음'],
    ['재배포 패키지 버전', '14.44 로, Client 를 빌드한 도구(14.44)와 같거나 새 버전이라 호환'],
    ['데이터베이스 접속', 'Client 는 Windows 기본 ODBC 드라이버("SQL Server"), 내려받기 프로그램은 .NET 기본 SqlClient 를 써 따로 설치할 것이 없음'],
    ['설치 실행 (Setup.bat 전 과정)', '별도 폴더에 설치 시험: 619개 파일 복사·이름 대조 통과, Ecs.ini/WmsDown.ini 에 서버 주소 기록'],
    ['설치본 첫 실행', 'Ecs.exe → EcsMain.exe 가 뜨고 DB 에 접속, 최신 화면 표시 (5절 그림)'],
], widths=[7.5, 10.0], font=8.5)
para(d, '확인하지 못한 것', bold=True)
bullets(d, [
    '운영체제만 설치된 실제 PC(또는 가상머신)에서의 설치는 시험하지 못했습니다. 이 개발 PC 에는 재배포 패키지와 .NET 이 이미 있어 [1/5], [2/5] 의 실제 설치 동작(무인 설치)은 확인하지 못했습니다. 새 PC 에서 처음 설치할 때 이 두 단계의 결과를 확인해 주십시오.',
    '[2/5] 의 .NET 웹 설치 파일은 인터넷이 필요합니다. 이 방법으로 설치해야 하는 PC(Windows 10 이전)는 지원 대상으로 보지 않습니다.',
    '바탕화면 바로가기는 모든 사용자 바탕화면에 만듭니다. 관리자 권한이 아닌 상태에서는 만들어지지 않습니다 (시험 PC 에서 확인).',
])

# ───────────────────────── 9
d.add_heading('9. 이전 꾸러미와 달라진 점', 1)
table(d, ['항목', '이전 (2026-10-15)', '이번 (2026-11-19)'], [
    ['Client 본체와 라이브러리', '10월 판 (EcsMain, DciLib, EcsLib, XmlLib, BuilderLib)', '11/12 빌드(Release) 기준으로 교체'],
    ['다국어 문구(rc_resource)', '10월 판', '11/12 빌드 기준으로 교체 (cvclear.png 등 추가분 포함)'],
    ['Ecs.ini', '10월 판 (지상반 삭제·C/V 깜빡임 등 최신 키 없음)', '최신 Release 의 Ecs.ini (HMIDEL_*, HS_BLINK 포함)'],
    ['Uninstall.bat, Set-DbServer.ps1, 확인하는_법.txt', '만들어지지 않아 꾸러미에 없었음 (Setup.bat 이 복사하려 하는 파일)', '모두 포함'],
    ['내려받기 라이브러리', 'Npgsql.dll, Mono.Security.dll 없음', '포함'],
    ['안내 문구 글자', 'Set-DbServer.ps1 의 한글 안내가 깨져 보일 수 있었음', 'UTF-8 BOM 으로 저장해 정상 표시'],
], widths=[4.2, 6.7, 6.6], font=9)

out = os.path.join(BASE, '설치_매뉴얼_20261119.docx')
try:
    d.save(out)
except PermissionError:
    out = out[:-5] + '_b.docx'
    d.save(out)
print('saved', out)
