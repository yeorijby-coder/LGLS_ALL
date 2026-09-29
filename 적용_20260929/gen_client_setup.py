# -*- coding: utf-8 -*-
u"""Client 설치 꾸러미를 만든다. 2026-09-30.

   실행 : python gen_client_setup.py
   결과 : 적용_20260929/Client_설치/  (리포에는 넣지 않는다 - 다른 폴더의 파일을 모아 만든다)

   새 PC 에 이것 하나만 가져가면 Client 가 돌게 한다.
     · Client 실행에 필요한 파일 전부
     · 내려받기 프로그램(EcsClient.exe)과 그 설정
     · Visual C++ 재배포 패키지(mfc140u.dll 등 - 없으면 Client 가 아예 뜨지 않는다)
     · .NET Framework 4.8 (Windows 10 이상은 이미 들어 있다 - 없을 때만 깐다)

   MS-SQL 접속에 따로 깔 것은 없다.
     Client 는 Windows 에 늘 들어 있는 ODBC 드라이버 "SQL Server" 로 붙고
     (Ecs.ini 의 [DB_2] DRIVER=SQL Server),
     EcsClient 는 .NET 에 들어 있는 System.Data.SqlClient 로 붙는다.
"""
import os, shutil

ROOT = r'D:\project\LGLS\Renewal'
SRC_CLIENT = os.path.join(ROOT, 'EXE_NEWUI', 'WCS_CLIENT')
SRC_DOWN = os.path.join(ROOT, 'TASK', 'Download', 'bin', 'Debug')
SRC_PRE = os.path.join(ROOT, 'EXE', 'Prerequisites')
DST = os.path.join(ROOT, '적용_20260929', 'Client_설치')

# 새로 만든다
if os.path.isdir(DST):
    shutil.rmtree(DST)
os.makedirs(os.path.join(DST, 'Client'))
os.makedirs(os.path.join(DST, 'Prerequisites'))

# ── Client 실행에 필요한 것 ───────────────────────────────────────
FILES = [
    # 프로그램
    'Ecs.exe',
    # 함께 쓰는 라이브러리 (Ecs.exe 가 곧바로 부른다)
    'DciLib.dll', 'EcsLib.dll', 'XmlLib.dll', 'BuilderLib.dll',
    'SPR32DU70.DLL', 'spr32d70.dll', 'QUvc_dll.dll',
    # 설정과 화면 정의
    'Ecs.ini', 'EcsDefine.xml', 'KeyWord.xml', 'SC.XML', 'observables.tsv',
    'EcsLayout1.xml', 'EcsLayout2.xml', 'EcsLayout3.xml',
    # 설비 주소 정의
    'DeviceMap01.xml', 'DeviceMap02.xml', 'DeviceMap03.xml', 'DeviceMap04.xml',
    'DeviceMap05.xml', 'DeviceMap06.xml', 'DeviceMap07.xml', 'DeviceMap08.xml',
    # 옛 라이브러리 (구 ECS 에서 이어진 것 - 빠지면 일부 화면이 뜨지 않는다)
    'MFC42D.DLL', 'MFCN42D.DLL', 'MFCO42D.DLL', 'MSVCP60D.DLL', 'MSVCRTD.DLL',
    'mfc42.dll', 'mfc100.dll', 'mfc100u.dll', 'msvcirt.dll', 'msvcp60.dll',
    'msvcr100.dll', 'msvcrt.dll', 'msxml4.dll', 'nmsql.dll',
]

miss = []
for f in FILES:
    src = os.path.join(SRC_CLIENT, f)
    if os.path.isfile(src):
        shutil.copy2(src, os.path.join(DST, 'Client', f))
    else:
        miss.append(f)

# 다국어 문구
shutil.copytree(os.path.join(SRC_CLIENT, 'rc_resource'),
                os.path.join(DST, 'Client', 'rc_resource'))
# 로그 폴더는 비워서 만들어 둔다
os.makedirs(os.path.join(DST, 'Client', 'LOG'))

# ── 내려받기 프로그램 ─────────────────────────────────────────────
for f in ('EcsClient.exe', 'WmsDown.ini'):
    shutil.copy2(os.path.join(SRC_DOWN, f), os.path.join(DST, 'Client', f))

ico = os.path.join(ROOT, '참조', 'WCS_HUONS', 'WCS.ico')
if os.path.isfile(ico):
    shutil.copy2(ico, os.path.join(DST, 'Client', 'WCS.ico'))
else:
    miss.append('WCS.ico')

# ── 먼저 깔아야 하는 것 ───────────────────────────────────────────
for f in ('vc_redist.x86.exe', 'ndp48-web.exe'):
    shutil.copy2(os.path.join(SRC_PRE, f), os.path.join(DST, 'Prerequisites', f))

print('빠진 파일 :', miss if miss else '없다')
n = sum(len(fs) for _, _, fs in os.walk(os.path.join(DST, 'Client')))
sz = sum(os.path.getsize(os.path.join(r, f))
         for r, _, fs in os.walk(DST) for f in fs)
print('Client 파일 %d 개, 꾸러미 전체 %.1f MB' % (n, sz / 1024.0 / 1024.0))


# ══════════ 설치 실행 파일과 안내문 ══════════
ROOT = r'D:\project\LGLS\Renewal'
DST = os.path.join(ROOT, '적용_20260929', 'Client_설치')
N = '\r\n'


def wr(path, lines):
    data = (N.join(lines) + N).encode('cp949')
    with open(path, 'wb') as f:
        f.write(data)
    print(path)


PCT = '%'

wr(os.path.join(DST, 'Setup.bat'), [
 '@echo off',
 'chcp 949 > nul',
 'setlocal',
 '',
 'rem ============================================================',
 'rem  LGLS CLIENT 설치',
 'rem  2026-09-30',
 'rem ============================================================',
 '',
 'set "INSTDIR=C:\\LGLS\\CLIENT"',
 'set "SRC=%~dp0"',
 '',
 'echo.',
 'echo  ============================================================',
 'echo   LGLS CLIENT 설치',
 'echo  ============================================================',
 'echo.',
 '',
 'rem ── 관리자 권한이 있어야 재배포 패키지를 깔 수 있다 ──────────',
 'net session >nul 2>&1',
 'if errorlevel 1 (',
 '    echo  [!] 관리자 권한으로 실행해 주세요.',
 '    echo      이 파일을 오른쪽 단추로 눌러 "관리자 권한으로 실행" 을 고르면 됩니다.',
 '    echo.',
 '    pause',
 '    exit /b 1',
 ')',
 '',
 'rem ── 설치 위치 ────────────────────────────────────────────────',
 'echo  설치 위치 [%INSTDIR%]',
 'set /p ANS=  그대로 하려면 Enter, 바꾸려면 경로를 적으세요 : ',
 'if not "%ANS%"=="" set "INSTDIR=%ANS%"',
 'echo.',
 'echo  설치 위치 : %INSTDIR%',
 'echo.',
 '',
 'rem ── 1. Visual C++ 재배포 패키지 ───────────────────────────────',
 'rem    Ecs.exe 가 mfc140u.dll / MSVCP140.dll 을 쓴다. 없으면 아예 뜨지 않는다.',
 'echo  [1/4] Visual C++ 재배포 패키지를 확인합니다...',
 'if exist "%WINDIR%\\SysWOW64\\mfc140u.dll" (',
 '    echo        이미 들어 있습니다. 건너뜁니다.',
 ') else (',
 '    if exist "%WINDIR%\\System32\\mfc140u.dll" (',
 '        echo        이미 들어 있습니다. 건너뜁니다.',
 '    ) else (',
 '        echo        설치합니다. 잠시 기다려 주세요...',
 '        "%SRC%Prerequisites\\vc_redist.x86.exe" /install /quiet /norestart',
 '        if errorlevel 1 echo        [!] 설치가 끝나지 않았습니다. 나중에 직접 실행해 주세요.',
 '    )',
 ')',
 'echo.',
 '',
 'rem ── 2. .NET Framework 4.8 ─────────────────────────────────────',
 'rem    내려받기 프로그램(EcsClient.exe)이 .NET 으로 만들어져 있다.',
 'rem    Windows 10 이상은 이미 들어 있으므로 대개 건너뛴다.',
 'echo  [2/4] .NET Framework 를 확인합니다...',
 'reg query "HKLM\\SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full" /v Release >nul 2>&1',
 'if errorlevel 1 (',
 '    echo        설치합니다. 인터넷 연결이 필요합니다...',
 '    "%SRC%Prerequisites\\ndp48-web.exe" /q /norestart',
 ') else (',
 '    echo        이미 들어 있습니다. 건너뜁니다.',
 ')',
 'echo.',
 '',
 'rem ── 3. 프로그램 복사 ─────────────────────────────────────────',
 'echo  [3/4] 프로그램을 복사합니다...',
 'if not exist "%INSTDIR%" mkdir "%INSTDIR%"',
 '',
 'rem    설정 파일은 이미 있으면 덮어쓰지 않는다 - 현장 값이 들어 있다.',
 'if exist "%INSTDIR%\\Ecs.ini"     copy /y "%INSTDIR%\\Ecs.ini"     "%INSTDIR%\\Ecs.ini.before_setup"     >nul',
 'if exist "%INSTDIR%\\WmsDown.ini" copy /y "%INSTDIR%\\WmsDown.ini" "%INSTDIR%\\WmsDown.ini.before_setup" >nul',
 '',
 'xcopy "%SRC%Client\\*" "%INSTDIR%\\" /E /I /Y /Q',
 'if errorlevel 1 (',
 '    echo        [!] 복사에 실패했습니다.',
 '    pause',
 '    exit /b 1',
 ')',
 '',
 'rem    설정을 되돌려 준다 - 새 설정은 .new 로 남긴다',
 'if exist "%INSTDIR%\\Ecs.ini.before_setup" (',
 '    move /y "%INSTDIR%\\Ecs.ini" "%INSTDIR%\\Ecs.ini.new" >nul',
 '    move /y "%INSTDIR%\\Ecs.ini.before_setup" "%INSTDIR%\\Ecs.ini" >nul',
 '    echo        Ecs.ini 는 쓰던 것을 그대로 두었습니다 ^(새 것은 Ecs.ini.new^).',
 ')',
 'if exist "%INSTDIR%\\WmsDown.ini.before_setup" (',
 '    move /y "%INSTDIR%\\WmsDown.ini" "%INSTDIR%\\WmsDown.ini.new" >nul',
 '    move /y "%INSTDIR%\\WmsDown.ini.before_setup" "%INSTDIR%\\WmsDown.ini" >nul',
 '    echo        WmsDown.ini 는 쓰던 것을 그대로 두었습니다 ^(새 것은 WmsDown.ini.new^).',
 ')',
 'echo.',
 '',
 'rem ── 4. 바탕화면 바로가기 ─────────────────────────────────────',
 'echo  [4/4] 바로가기를 만듭니다...',
 'set "VBS=%TEMP%\\lgls_shortcut.vbs"',
 '> "%VBS%" echo Set oWS = WScript.CreateObject^("WScript.Shell"^)',
 '>>"%VBS%" echo sLink = oWS.SpecialFolders^("AllUsersDesktop"^) ^& "\\LGLS CLIENT.lnk"',
 '>>"%VBS%" echo Set oLink = oWS.CreateShortcut^(sLink^)',
 '>>"%VBS%" echo oLink.TargetPath = "%INSTDIR%\\EcsClient.exe"',
 '>>"%VBS%" echo oLink.WorkingDirectory = "%INSTDIR%"',
 '>>"%VBS%" echo oLink.IconLocation = "%INSTDIR%\\WCS.ico"',
 '>>"%VBS%" echo oLink.Description = "LGLS 자동창고 운전 화면"',
 '>>"%VBS%" echo oLink.Save',
 'cscript //nologo "%VBS%"',
 'del "%VBS%" >nul 2>&1',
 'echo.',
 '',
 'echo  ============================================================',
 'echo   설치가 끝났습니다.',
 'echo  ============================================================',
 'echo.',
 'echo   설치 위치 : %INSTDIR%',
 'echo   실행      : 바탕화면의 [LGLS CLIENT]',
 'echo.',
 'echo   ★ 띄우기 전에 설정 두 곳의 DB 주소를 확인해 주세요 ★',
 'echo.',
 'echo     %INSTDIR%\\Ecs.ini       [DB_2] SERVER=',
 'echo     %INSTDIR%\\WmsDown.ini   [DB Server] SERVERNAME=',
 'echo.',
 'pause',
 'endlocal',
])

wr(os.path.join(DST, '읽어보세요.txt'), [
 '============================================================',
 ' LGLS CLIENT 설치 꾸러미',
 ' 2026-09-30',
 '============================================================',
 '',
 '',
 '[ 무엇인가 ]',
 '',
 '  새 PC 에 운전 화면(Client)을 까는 꾸러미입니다.',
 '  이것 하나만 가져가면 됩니다.',
 '',
 '',
 '[ 어떻게 하나 ]',
 '',
 '  1. 이 폴더를 통째로 새 PC 에 복사합니다 (USB 등).',
 '  2. Setup.bat 을 오른쪽 단추로 눌러 [관리자 권한으로 실행] 합니다.',
 '  3. 설치 위치를 물으면 그대로 Enter (기본 C:\\LGLS\\CLIENT).',
 '  4. 끝나면 바탕화면에 [LGLS CLIENT] 가 생깁니다.',
 '',
 '',
 '[ 설치가 하는 일 ]',
 '',
 '  1) Visual C++ 재배포 패키지',
 '       Ecs.exe 가 mfc140u.dll 을 씁니다. 없으면 프로그램이 아예 뜨지 않고',
 '       "mfc140u.dll 을 찾을 수 없습니다" 가 납니다.',
 '       이미 들어 있으면 건너뜁니다.',
 '',
 '  2) .NET Framework 4.8',
 '       내려받기 프로그램(EcsClient.exe)이 .NET 으로 되어 있습니다.',
 '       Windows 10 이상은 이미 들어 있으므로 거의 건너뜁니다.',
 '',
 '  3) 프로그램 복사',
 '       Ecs.exe 와 함께 쓰는 파일, 화면 정의(xml), 다국어 문구(rc_resource)를',
 '       설치 위치에 복사합니다.',
 '       ★ 이미 쓰던 Ecs.ini / WmsDown.ini 는 덮어쓰지 않습니다.',
 '          새 것은 .new 로 남겨 두니 견주어 보고 필요한 줄만 옮기세요.',
 '',
 '  4) 바탕화면 바로가기',
 '',
 '',
 '[ MS-SQL 접속에 따로 깔 것 ]',
 '',
 '  없습니다.',
 '    · Client(Ecs.exe) 는 Windows 에 늘 들어 있는 ODBC 드라이버',
 '      "SQL Server" 로 붙습니다 (Ecs.ini 의 [DB_2] DRIVER=SQL Server).',
 '    · EcsClient.exe 는 .NET 에 들어 있는 SqlClient 로 붙습니다.',
 '  둘 다 Windows 와 .NET 에 이미 들어 있는 것이라 따로 깔 것이 없습니다.',
 '',
 '',
 '[ 설치 뒤에 반드시 볼 것 ]',
 '',
 '  DB 주소가 PC 마다 다릅니다. 두 군데를 봐 주세요.',
 '',
 '    Ecs.ini      [DB_2]      SERVER=      DATABASE=',
 '    WmsDown.ini  [DB Server] SERVERNAME=  DATABASE=',
 '',
 '  계정을 쓰지 않고 Windows 인증으로 붙이려면',
 '  WmsDown.ini 의 USERID 와 PASSWORD 를 비워 두면 됩니다.',
 '',
 '',
 '[ 바탕화면의 [LGLS CLIENT] 를 누르면 ]',
 '',
 '  EcsClient.exe 가 먼저 뜹니다. 이것이',
 '    · DB 에 올라와 있는 프로그램 판과 지금 PC 의 것을 견주어',
 '    · 바뀐 파일만 내려받고',
 '    · 이어서 Ecs.exe 를 띄웁니다.',
 '',
 '  그래서 앞으로 프로그램을 고치면, 각 PC 를 돌아다닐 필요 없이',
 '  WmsUp.exe 로 한 번 올리기만 하면 됩니다.',
 '',
])
