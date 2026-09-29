# -*- coding: utf-8 -*-
u"""Client 설치 꾸러미를 만든다. 2026-09-30.

   실행 : python gen_client_setup.py
   결과 : 적용_20260929/Client_설치/  (리포에는 넣지 않는다 - 다른 폴더의 파일을 모아 만든다)

   새 PC 에 이것 하나만 가져가면 Client 가 돌게 한다.
     · Client 실행에 필요한 파일 전부
     · 내려받기 프로그램(Ecs.exe)과 그 설정
     · Visual C++ 재배포 패키지(mfc140u.dll 등 - 없으면 Client 가 아예 뜨지 않는다)
     · .NET Framework 4.8 (Windows 10 이상은 이미 들어 있다 - 없을 때만 깐다)

   MS-SQL 접속에 따로 깔 것은 없다.
     Client 는 Windows 에 늘 들어 있는 ODBC 드라이버 "SQL Server" 로 붙고
     (Ecs.ini 의 [DB_2] DRIVER=SQL Server),
     Ecs.exe(내려받기)는 .NET 에 들어 있는 System.Data.SqlClient 로 붙는다.
"""
import os, shutil, stat

ROOT = r'D:\project\LGLS\Renewal'
SRC_CLIENT = os.path.join(ROOT, 'EXE_NEWUI', 'WCS_CLIENT')
SRC_DOWN = os.path.join(ROOT, 'TASK', 'Download', 'bin', 'Release')   # 배포는 Release 판
SRC_BIN = os.path.join(ROOT, 'WCS', 'CPlusPlus', 'LGLS_CLIENT', 'Bin', 'Release')
SRC_PRE = os.path.join(ROOT, 'EXE', 'Prerequisites')
DST = os.path.join(ROOT, '적용_20260929', 'Client_설치')

# 새로 만든다
def _drop_readonly(func, path, _exc):
    # 원본에 읽기전용이 붙어 있으면 지워지지 않는다 - 풀고 다시 지운다
    os.chmod(path, stat.S_IWRITE)
    func(path)


if os.path.isdir(DST):
    shutil.rmtree(DST, onexc=_drop_readonly)
os.makedirs(os.path.join(DST, 'Client'))
os.makedirs(os.path.join(DST, 'Prerequisites'))

# ── Client 실행에 필요한 것 ───────────────────────────────────────
FILES = [
    # [LGLS 2026-09-30] 본체 exe 는 SRC_BIN(방금 빌드한 Release)에서 따로 가져온다.
    #   사람이 누르는 Ecs.exe 는 내려받기 프로그램이고, 본체는 EcsMain.exe 다.
    # 함께 쓰는 라이브러리 (본체가 곧바로 부른다)
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

# ── Client 본체 - 이름을 EcsMain.exe 로 (사람이 누르는 Ecs.exe 와 겹치지 않게) ──
src_main = os.path.join(SRC_BIN, 'EcsMain.exe')
if os.path.isfile(src_main):
    shutil.copy2(src_main, os.path.join(DST, 'Client', 'EcsMain.exe'))
else:
    miss.append('EcsMain.exe')

# 다국어 문구
shutil.copytree(os.path.join(SRC_CLIENT, 'rc_resource'),
                os.path.join(DST, 'Client', 'rc_resource'))
# 로그 폴더는 비워서 만들어 둔다
os.makedirs(os.path.join(DST, 'Client', 'LOG'))

# ── 내려받기 프로그램 ─────────────────────────────────────────────
# ── 내려받기 프로그램 - 이것이 사람이 누르는 Ecs.exe 다 ──────────
for f in ('Ecs.exe', 'WmsDown.ini'):
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
    data = (N.join(lines) + N).encode('cp949')          # 먼저 검증
    with open(path, 'wb') as f:                          # 그 다음 연다
        f.write(data)
    print(path)


wr(os.path.join(DST, 'Setup.bat'), [
 '@echo off',
 'chcp 949 > nul',
 'setlocal enabledelayedexpansion',
 '',
 'rem ============================================================',
 'rem  LGLS CLIENT 설치',
 'rem  2026-09-30',
 'rem ============================================================',
 '',
 'set "INSTDIR=C:\\LGLS\\CLIENT"',
 'set "SRC=%~dp0"',
 'set "DBIP=127.0.0.1"',
 'set "DBPORT=1433"',
 'set "DBNAME=LGLS_MCS_IO"',
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
 'echo  [ 설치 위치 ]',
 'echo    그대로 하려면 Enter, 바꾸려면 경로를 적으세요.',
 'set /p ANS=   설치 위치 [%INSTDIR%] : ',
 'if not "!ANS!"=="" set "INSTDIR=!ANS!"',
 'echo.',
 '',
 'rem ── DB 서버 ──────────────────────────────────────────────────',
 'echo  [ DB 서버 ]',
 'echo    Client 와 내려받기 프로그램이 붙을 SQL Server 입니다.',
 'echo    PC 마다 다르므로 여기서 받아 설정에 적어 둡니다.',
 'echo.',
 'set /p ANS=   서버 IP [%DBIP%] : ',
 'if not "!ANS!"=="" set "DBIP=!ANS!"',
 'set /p ANS=   포트 [%DBPORT%] : ',
 'if not "!ANS!"=="" set "DBPORT=!ANS!"',
 'set /p ANS=   데이터베이스 [%DBNAME%] : ',
 'if not "!ANS!"=="" set "DBNAME=!ANS!"',
 '',
 'rem    SQL Server 는 "주소,포트" 로 적는다. 1433 이면 포트를 생략해도 된다.',
 'if "%DBPORT%"=="1433" (',
 '    set "DBSERVER=%DBIP%"',
 ') else (',
 '    set "DBSERVER=%DBIP%,%DBPORT%"',
 ')',
 '',
 'rem    이름있는 인스턴스(예 PC이름\\SQLEXPRESS)를 적었으면 포트를 붙이지 않는다',
 'echo %DBIP% | find "\\" > nul',
 'if not errorlevel 1 set "DBSERVER=%DBIP%"',
 '',
 'echo.',
 'echo  ------------------------------------------------------------',
 'echo   설치 위치   : %INSTDIR%',
 'echo   DB 서버     : %DBSERVER%',
 'echo   데이터베이스 : %DBNAME%',
 'echo  ------------------------------------------------------------',
 'set /p ANS=   이대로 설치할까요? (Y/n) : ',
 'if /i "!ANS!"=="n" (',
 '    echo   취소했습니다.',
 '    pause',
 '    exit /b 0',
 ')',
 'echo.',
 '',
 'rem ── 1. Visual C++ 재배포 패키지 ───────────────────────────────',
 'rem    Client 본체(EcsMain.exe)가 mfc140u.dll / MSVCP140.dll 을 쓴다. 없으면 아예 뜨지 않는다.',
 'echo  [1/5] Visual C++ 재배포 패키지를 확인합니다...',
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
 'rem    내려받기 프로그램(Ecs.exe)이 .NET 으로 만들어져 있다.',
 'rem    Windows 10 이상은 이미 들어 있으므로 대개 건너뛴다.',
 'echo  [2/5] .NET Framework 를 확인합니다...',
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
 'echo  [3/5] 프로그램을 복사합니다...',
 'if not exist "%INSTDIR%" mkdir "%INSTDIR%"',
 '',
 'rem    설정 파일은 이미 있으면 덮어쓰지 않는다 - 현장 값이 들어 있다.',
 'if exist "%INSTDIR%\\Ecs.ini"     copy /y "%INSTDIR%\\Ecs.ini"     "%INSTDIR%\\Ecs.ini.before_setup"     >nul',
 'if exist "%INSTDIR%\\WmsDown.ini" copy /y "%INSTDIR%\\WmsDown.ini" "%INSTDIR%\\WmsDown.ini.before_setup" >nul',
 '',
 'xcopy "%SRC%Client\\*" "%INSTDIR%\\" /E /I /Y /Q',
 'rem    주소를 나중에 바꿀 수 있게 도우미와 안내문도 함께 둔다',
 'copy /y "%SRC%Set-DbServer.ps1" "%INSTDIR%\\" >nul',
 'copy /y "%SRC%확인하는_법.txt" "%INSTDIR%\\" >nul',
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
 'rem ── 4. DB 서버 주소 적용 ─────────────────────────────────────',
 'rem    ini 한 줄만 바꾸는 일은 배치로 하면 한글이 깨지므로 PowerShell 로 한다.',
 'echo  [4/5] DB 서버 주소를 설정에 적습니다...',
 'powershell -NoProfile -ExecutionPolicy Bypass -File "%SRC%Set-DbServer.ps1" -InstDir "%INSTDIR%" -Server "%DBSERVER%" -Database "%DBNAME%"',
 'if errorlevel 1 (',
 '    echo        [!] 설정을 적지 못했습니다. 아래 두 곳을 직접 고쳐 주세요.',
 '    echo            %INSTDIR%\\Ecs.ini       [DB_2] SERVER=',
 '    echo            %INSTDIR%\\WmsDown.ini   [DB Server] SERVERNAME=',
 ')',
 'echo.',
 '',
 'rem ── 5. 바탕화면 바로가기 ─────────────────────────────────────',
 'echo  [5/5] 바로가기를 만듭니다...',
 'set "VBS=%TEMP%\\lgls_shortcut.vbs"',
 '> "%VBS%" echo Set oWS = WScript.CreateObject^("WScript.Shell"^)',
 '>>"%VBS%" echo sLink = oWS.SpecialFolders^("AllUsersDesktop"^) ^& "\\LGLS CLIENT.lnk"',
 '>>"%VBS%" echo Set oLink = oWS.CreateShortcut^(sLink^)',
 '>>"%VBS%" echo oLink.TargetPath = "%INSTDIR%\\Ecs.exe"',
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
 'echo   설치 위치   : %INSTDIR%',
 'echo   DB 서버     : %DBSERVER%',
 'echo   데이터베이스 : %DBNAME%',
 'echo   실행        : 바탕화면의 [LGLS CLIENT]',
 'echo.',
 'echo   계정을 쓰려면 아래에 적어 주세요. 비워 두면 Windows 인증으로 붙습니다.',
 'echo     %INSTDIR%\\WmsDown.ini   [DB Server] USERID= / PASSWORD=',
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
 '  3. 물어보는 것에 답합니다.',
 '',
 '       설치 위치     그대로 두려면 Enter (기본 C:\\LGLS\\CLIENT)',
 '       서버 IP       DB 가 있는 PC 의 주소',
 '       포트          기본 1433. 다르면 적어 주세요.',
 '       데이터베이스  기본 LGLS_MCS_IO',
 '',
 '  4. 끝나면 바탕화면에 [LGLS CLIENT] 가 생깁니다.',
 '',
 '',
 '[ 서버 주소를 어떻게 적나 ]',
 '',
 '  · 보통    : IP 와 포트를 따로 적으면 됩니다.',
 '              예) 192.100.1.191  /  1433',
 '',
 '  · 포트를 바꾼 현장 : 그 포트를 적습니다.',
 '              예) 127.0.0.1  /  1435',
 '',
 '  · 이름있는 인스턴스 : 서버 IP 칸에 인스턴스까지 적습니다.',
 '              예) PC이름\\SQLEXPRESS',
 '              이때는 포트를 묻더라도 쓰지 않습니다.',
 '',
 '  적어 넣는 자리는 두 곳이고, 설치가 알아서 넣습니다.',
 '      Ecs.ini      [DB_2]      SERVER=      DATABASE=',
 '      WmsDown.ini  [DB Server] SERVERNAME=  DATABASE=',
 '',
 '  나중에 서버가 바뀌면 Setup.bat 을 다시 돌리면 됩니다.',
 '  (쓰던 설정은 그대로 두고 주소만 바꿔 줍니다.)',
 '',
 '',
 '[ 설치가 하는 일 ]',
 '',
 '  1) Visual C++ 재배포 패키지',
 '       Client 본체(EcsMain.exe)가 mfc140u.dll 을 씁니다. 없으면 아예 뜨지 않고',
 '       "mfc140u.dll 을 찾을 수 없습니다" 가 납니다.',
 '       이미 들어 있으면 건너뜁니다.',
 '',
 '  2) .NET Framework 4.8',
 '       내려받기 프로그램(Ecs.exe)이 .NET 으로 되어 있습니다.',
 '       Windows 10 이상은 이미 들어 있으므로 거의 건너뜁니다.',
 '',
 '  3) 프로그램 복사',
 '       프로그램과 함께 쓰는 파일, 화면 정의(xml), 다국어 문구(rc_resource)를',
 '       설치 위치에 복사합니다.',
 '       ★ 이미 쓰던 Ecs.ini / WmsDown.ini 는 덮어쓰지 않습니다.',
 '          새 것은 .new 로 남겨 두니 견주어 보고 필요한 줄만 옮기세요.',
 '',
 '  4) DB 서버 주소 적용',
 '       위에서 받은 주소를 두 설정 파일에 적습니다.',
 '       주석(; 로 시작하는 줄)은 건드리지 않습니다.',
 '',
 '  5) 바탕화면 바로가기',
 '',
 '',
 '[ MS-SQL 접속에 따로 깔 것 ]',
 '',
 '  없습니다.',
 '    · Client 본체(EcsMain.exe)는 Windows 에 늘 들어 있는 ODBC 드라이버',
 '      "SQL Server" 로 붙습니다 (Ecs.ini 의 [DB_2] DRIVER=SQL Server).',
 '    · Ecs.exe(내려받기)는 .NET 에 들어 있는 SqlClient 로 붙습니다.',
 '  둘 다 Windows 와 .NET 에 이미 들어 있는 것이라 따로 깔 것이 없습니다.',
 '',
 '',
 '[ 계정 ]',
 '',
 '  계정을 쓰지 않고 Windows 인증으로 붙이려면',
 '  WmsDown.ini 의 USERID 와 PASSWORD 를 비워 두면 됩니다. (기본값)',
 '  계정을 쓰려면 그 두 줄에 적어 주세요.',
 '',
 '',
 '[ 바탕화면의 [LGLS CLIENT] 를 누르면 ]',
 '',
 '  Ecs.exe 가 먼저 뜹니다. 이것이',
 '    · DB 에 올라와 있는 프로그램 판과 지금 PC 의 것을 견주어',
 '    · 바뀐 파일만 내려받고',
 '    · 이어서 Client 본체(EcsMain.exe)를 띄웁니다.',
 '',
 '  그래서 앞으로 프로그램을 고치면, 각 PC 를 돌아다닐 필요 없이',
 '  WmsUp.exe 로 한 번 올리기만 하면 됩니다.',
 '',
])


# ══════════ DB 서버 주소를 적어 주는 도우미 ══════════
DST = r'D:\project\LGLS\Renewal\적용_20260929\Client_설치'
N = '\r\n'


def wr(path, lines, enc='cp949'):
    data = (N.join(lines) + N).encode(enc)
    with open(path, 'wb') as f:
        f.write(data)
    print(path)


wr(os.path.join(DST, 'Set-DbServer.ps1'), [
 '# ============================================================',
 '#  설치할 때 받은 DB 서버 주소를 설정 파일에 적는다.',
 '#  2026-09-30',
 '#',
 '#  Setup.bat 이 부른다. 따로 실행할 일은 없다.',
 '#  ini 는 CP949 라 인코딩을 지켜 읽고 쓴다(Default = 시스템 ANSI).',
 '# ============================================================',
 'param(',
 '    [Parameter(Mandatory=$true)][string]$InstDir,',
 '    [Parameter(Mandatory=$true)][string]$Server,',
 '    [string]$Database = ""',
 ')',
 '',
 '$ErrorActionPreference = "Stop"',
 '',
 'function Set-IniValue {',
 '    param(',
 '        [string]$Path,',
 '        [string]$Section,   # 대괄호 없이. 예 "DB_2", "DB Server"',
 '        [string]$Key,',
 '        [string]$Value',
 '    )',
 '',
 '    $name = Split-Path $Path -Leaf',
 '',
 '    if (-not (Test-Path $Path)) {',
 '        Write-Host ("        [!] " + $name + " 가 없어 건너뜁니다.")',
 '        return',
 '    }',
 '',
 '    $lines  = @(Get-Content $Path -Encoding Default)',
 '    $inSec  = $false',
 '    $done   = $false',
 '    $out    = New-Object System.Collections.ArrayList',
 '',
 '    foreach ($ln in $lines) {',
 '',
 '        # 섹션 머리글을 만나면 여기가 찾는 섹션인지 다시 판단한다',
 '        if ($ln -match "^\\s*\\[(.+?)\\]") {',
 '            $inSec = ($matches[1].Trim() -eq $Section)',
 '            [void]$out.Add($ln)',
 '            continue',
 '        }',
 '',
 '        # 찾는 섹션 안이고, 주석이 아니고, 그 키인 첫 줄만 바꾼다',
 '        if ($inSec -and (-not $done) -and',
 '            ($ln -notmatch "^\\s*[;-]") -and',
 '            ($ln -match ("^\\s*" + [regex]::Escape($Key) + "\\s*="))) {',
 '            [void]$out.Add($Key + "=" + $Value)',
 '            $done = $true',
 '            continue',
 '        }',
 '',
 '        [void]$out.Add($ln)',
 '    }',
 '',
 '    if (-not $done) {',
 '        Write-Host ("        [!] " + $name + " 의 [" + $Section + "] 에서 " + $Key + " 줄을 찾지 못했습니다.")',
 '        Write-Host ("            직접 확인해 주세요.")',
 '        return',
 '    }',
 '',
 '    Set-Content -Path $Path -Value $out -Encoding Default',
 '    Write-Host ("        " + $name + "  [" + $Section + "] " + $Key + "=" + $Value)',
 '}',
 '',
 '$ecs  = Join-Path $InstDir "Ecs.ini"',
 '$down = Join-Path $InstDir "WmsDown.ini"',
 '',
 '# ── Client 본체 ──────────────────────────────────────────────',
 'Set-IniValue -Path $ecs -Section "DB_2" -Key "SERVER" -Value $Server',
 'if ($Database -ne "") {',
 '    Set-IniValue -Path $ecs -Section "DB_2" -Key "DATABASE" -Value $Database',
 '}',
 '',
 '# ── 내려받기 프로그램 ────────────────────────────────────────',
 'Set-IniValue -Path $down -Section "DB Server" -Key "SERVERNAME" -Value $Server',
 'if ($Database -ne "") {',
 '    Set-IniValue -Path $down -Section "DB Server" -Key "DATABASE" -Value $Database',
 '}',
], enc='utf-8')


# ══════════ 손으로 확인하는 법 ══════════
DST = r'D:\project\LGLS\Renewal\적용_20260929\Client_설치'
N = '\r\n'


def wr(path, lines):
    data = (N.join(lines) + N).encode('cp949')
    with open(path, 'wb') as f:
        f.write(data)
    print(path)


wr(os.path.join(DST, '확인하는_법.txt'), [
 '============================================================',
 ' 설치가 제대로 됐는지 손으로 확인하는 법',
 ' 2026-09-30',
 '============================================================',
 '',
 ' 아래 명령을 그대로 복사해 명령 프롬프트에 붙여넣으면 됩니다.',
 ' 경로는 설치한 곳에 맞게 바꿔 주세요.',
 '',
 '',
 '------------------------------------------------------------',
 ' 1. DB 서버 주소를 다시 적기',
 '------------------------------------------------------------',
 '',
 '   설치할 때 물어본 주소를 나중에 바꾸고 싶을 때 씁니다.',
 '   Setup.bat 을 다시 돌려도 되고, 이 명령만 써도 됩니다.',
 '',
 'powershell -NoProfile -ExecutionPolicy Bypass -File "C:\\LGLS\\CLIENT\\Set-DbServer.ps1" -InstDir "C:\\LGLS\\CLIENT" -Server "192.100.1.191" -Database "LGLS_MCS_IO"',
 '',
 '   이렇게 나오면 제대로 된 것입니다.',
 '',
 '        Ecs.ini  [DB_2] SERVER=192.100.1.191',
 '        Ecs.ini  [DB_2] DATABASE=LGLS_MCS_IO',
 '        WmsDown.ini  [DB Server] SERVERNAME=192.100.1.191',
 '        WmsDown.ini  [DB Server] DATABASE=LGLS_MCS_IO',
 '',
 '   네 줄이 다 나와야 합니다. "찾지 못했습니다" 가 나오면',
 '   그 파일의 섹션 이름이나 항목 이름이 다른 것이니 알려 주세요.',
 '',
 '   포트가 1433 이 아니면 주소 뒤에 쉼표로 붙입니다.',
 '       -Server "192.100.1.191,1435"',
 '   이름있는 인스턴스면 포트를 붙이지 않습니다.',
 '       -Server "PC이름\\SQLEXPRESS"',
 '',
 '',
 '------------------------------------------------------------',
 ' 2. 지금 설정이 어떻게 되어 있는지 보기',
 '------------------------------------------------------------',
 '',
 'powershell -NoProfile -Command "Get-Content C:\\LGLS\\CLIENT\\Ecs.ini -Encoding Default | Select-String \'^SERVER=|^DATABASE=\'"',
 '',
 'powershell -NoProfile -Command "Get-Content C:\\LGLS\\CLIENT\\WmsDown.ini -Encoding Default | Select-String \'^SERVERNAME=|^DATABASE=\'"',
 '',
 '',
 '------------------------------------------------------------',
 ' 3. 어느 판을 받아 두었는지 보기',
 '------------------------------------------------------------',
 '',
 '   내려받기가 WmsDown.ini 아래쪽에 스스로 적습니다.',
 '',
 'powershell -NoProfile -Command "Get-Content C:\\LGLS\\CLIENT\\WmsDown.ini -Encoding Default | Select-String \'COMMON->\'"',
 '',
 '   예)',
 '        COMMON->DciLib.dll=1',
 '        COMMON->EcsDefine.xml=1',
 '        COMMON->EcsLayout1.xml=1',
 '        COMMON->KeyWord.xml=1',
 '',
 '   여기 적힌 판과 DB 에 올라온 판이 같으면 내려받지 않고 지나갑니다.',
 '   일부러 다시 받게 하려면 이 줄들을 지우고 다시 실행하면 됩니다.',
 '',
 '',
 '------------------------------------------------------------',
 ' 4. 설치된 파일이 무엇인지',
 '------------------------------------------------------------',
 '',
 '   Ecs.exe       사람이 누르는 것. 바뀐 파일을 받아 오고 본체를 띄웁니다.',
 '   EcsMain.exe   운전 화면 본체. 내려받기가 이어서 띄웁니다.',
 '',
 '   두 파일은 아이콘이 같습니다. 누르는 것은 Ecs.exe 하나뿐입니다.',
 '',
 'powershell -NoProfile -Command "Get-ChildItem C:\\LGLS\\CLIENT\\*.exe | Select-Object Name,Length,LastWriteTime"',
 '',
 '',
 '------------------------------------------------------------',
 ' 5. ★올리면 안 되는 파일★',
 '------------------------------------------------------------',
 '',
 '   설정 파일은 PC 마다 다릅니다. 올려 두면 각 PC 가 그것을 받아',
 '   자기 설정을 잃고, DB 에 붙지 못해 화면이 뜨지 않습니다.',
 '',
 '       Ecs.ini',
 '       WmsDown.ini',
 '',
 '   실수로 올라오더라도 내려받기가 이 둘은 받지 않게 해 두었습니다.',
 '   목록을 바꾸려면 WmsDown.ini 에 이렇게 적습니다.',
 '',
 '       [APPLICATION]',
 '       SKIP_FILES=Ecs.ini,WmsDown.ini',
 '',
 '',
 '------------------------------------------------------------',
 ' 6. 잘 안 될 때',
 '------------------------------------------------------------',
 '',
 '   · "mfc140u.dll 을 찾을 수 없습니다"',
 '       Visual C++ 재배포 패키지가 없습니다.',
 '       Prerequisites\\vc_redist.x86.exe 를 직접 실행해 주세요.',
 '',
 '   · 내려받기 창이 뜬 뒤 아무 일도 없다',
 '       DB 주소가 틀렸을 수 있습니다. 위 2번으로 확인해 주세요.',
 '',
 '   · 에러 창이 뜬다',
 '       창에 어디서 났는지까지 나옵니다. 그 내용을 그대로 알려 주세요.',
 '',
])
