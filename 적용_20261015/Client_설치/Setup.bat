@echo off
chcp 949 > nul
setlocal enabledelayedexpansion

rem ============================================================
rem  LGLS CLIENT 설치
rem  2026-09-30
rem ============================================================

set "INSTDIR=C:\LGLS\CLIENT"
set "SRC=%~dp0"
set "DBIP=127.0.0.1"
set "DBPORT=1433"
set "DBNAME=LGLS_MCS_IO"

echo.
echo  ============================================================
echo   LGLS CLIENT 설치
echo  ============================================================
echo.

rem ── 관리자 권한이 있어야 재배포 패키지를 깔 수 있다 ──────────
net session >nul 2>&1
if errorlevel 1 (
    echo  [주의] 관리자 권한으로 실행해 주세요.
    echo      이 파일을 오른쪽 단추로 눌러 "관리자 권한으로 실행" 을 고르면 됩니다.
    echo.
    pause
    exit /b 1
)

rem ── 설치 위치 ────────────────────────────────────────────────
echo  [ 설치 위치 ]
echo    그대로 하려면 Enter, 바꾸려면 경로를 적으세요.
rem    set /p 는 엔터만 치면 변수를 지우지 않고 ★이전 값을 그대로 둔다★.
rem    그래서 앞 물음의 답이 다음 물음에 그대로 들어갔다.
rem    (포트에 1435 를 넣고 데이터베이스를 엔터로 넘기면 DATABASE=1435 가 되었다)
rem    물어보기 전에 매번 비운다.
set "ANS="
set /p ANS=   설치 위치 [%INSTDIR%] : 
if not "!ANS!"=="" set "INSTDIR=!ANS!"
echo.

rem ── DB 서버 ──────────────────────────────────────────────────
echo  [ DB 서버 ]
echo    Client 와 내려받기 프로그램이 붙을 SQL Server 입니다.
echo    PC 마다 다르므로 여기서 받아 설정에 적어 둡니다.
echo.
set "ANS="
set /p ANS=   서버 IP [%DBIP%] : 
if not "!ANS!"=="" set "DBIP=!ANS!"
set "ANS="
set /p ANS=   포트 [%DBPORT%] : 
if not "!ANS!"=="" set "DBPORT=!ANS!"
set "ANS="
set /p ANS=   데이터베이스 [%DBNAME%] : 
if not "!ANS!"=="" set "DBNAME=!ANS!"

rem    SQL Server 는 "주소,포트" 로 적는다. 1433 이면 포트를 생략해도 된다.
rem    괄호 블록 안에서는 %VAR% 가 블록에 들어가기 전에 치환된다.
rem    방금 받은 값을 쓰려면 !VAR! 로 읽어야 한다.
if "!DBPORT!"=="1433" (
    set "DBSERVER=!DBIP!"
) else (
    set "DBSERVER=!DBIP!,!DBPORT!"
)

rem    이름있는 인스턴스(예 PC이름\SQLEXPRESS)를 적었으면 포트를 붙이지 않는다
rem    역슬래시를 지운 값과 다르면 역슬래시가 들어 있다는 뜻이다.
rem    (find 같은 바깥 명령을 부르지 않는다 - 환경을 타지 않는다)
if not "!DBIP!"=="!DBIP:\=!" set "DBSERVER=!DBIP!"

echo.
echo  ------------------------------------------------------------
echo   설치 위치   : %INSTDIR%
echo   DB 서버     : %DBSERVER%
echo   데이터베이스 : %DBNAME%
echo  ------------------------------------------------------------
set "ANS="
set /p ANS=   이대로 설치할까요? (Y/n) : 
if /i "!ANS!"=="n" (
    echo   취소했습니다.
    pause
    exit /b 0
)
echo.

rem ── 1. Visual C++ 재배포 패키지 ───────────────────────────────
rem    Client 본체(EcsMain.exe)가 mfc140u.dll / MSVCP140.dll 을 쓴다. 없으면 아예 뜨지 않는다.
echo  [1/5] Visual C++ 재배포 패키지를 확인합니다...
if exist "%WINDIR%\SysWOW64\mfc140u.dll" (
    echo        이미 들어 있습니다. 건너뜁니다.
) else (
    if exist "%WINDIR%\System32\mfc140u.dll" (
        echo        이미 들어 있습니다. 건너뜁니다.
    ) else (
        echo        설치합니다. 잠시 기다려 주세요...
        "%SRC%Prerequisites\vc_redist.x86.exe" /install /quiet /norestart
        if errorlevel 1 echo        [주의] 설치가 끝나지 않았습니다. 나중에 직접 실행해 주세요.
    )
)
echo.

rem ── 2. .NET Framework 4.8 ─────────────────────────────────────
rem    내려받기 프로그램(Ecs.exe)이 .NET 으로 만들어져 있다.
rem    Windows 10 이상은 이미 들어 있으므로 대개 건너뛴다.
echo  [2/5] .NET Framework 를 확인합니다...
reg query "HKLM\SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Full" /v Release >nul 2>&1
if errorlevel 1 (
    echo        설치합니다. 인터넷 연결이 필요합니다...
    "%SRC%Prerequisites\ndp48-web.exe" /q /norestart
) else (
    echo        이미 들어 있습니다. 건너뜁니다.
)
echo.

rem ── 3. 프로그램 복사 ─────────────────────────────────────────
echo  [3/5] 프로그램을 복사합니다...

rem    [2026-10-01] 옛 Client 가 등록한 폰트는 로그오프 전까지 잠긴다. 지우다 만 폰트가 남은 폴더에는
rem    복사가 막히므로, 그런 폴더면 ★옆 이름(_2, _3 ...)★ 에 깐다 - 로그오프 없이 설치할 수 있다.
rem    (옛 폴더는 나중에 로그오프한 뒤 지우면 된다)
set "BASEDIR=%INSTDIR%"
set "SUFFIX=0"
set "TRIEDFORCE="
:PROBE
powershell -NoProfile -ExecutionPolicy Bypass -File "%SRC%Release-Fonts.ps1" -Dir "%INSTDIR%" -SrcDir "%SRC%Client" -Probe >nul 2>&1
if errorlevel 2 (
    if not defined TRIEDFORCE (
        set "TRIEDFORCE=1"
        echo        이 폴더에 옛 Client 가 잠가 놓은 폰트 파일이 있습니다. 먼저 풀어 봅니다...
        powershell -NoProfile -ExecutionPolicy Bypass -File "%SRC%Force-ReleaseFonts.ps1" -Dir "%INSTDIR%"
        echo.
        goto PROBE
    )
    set /a SUFFIX+=1
    if !SUFFIX! GEQ 6 goto PROBEDONE
    set "INSTDIR=!BASEDIR!_!SUFFIX!"
    goto PROBE
)
:PROBEDONE
if not "%INSTDIR%"=="%BASEDIR%" (
    echo        [안내] %BASEDIR% 에는 지우다 만 폰트 파일이 잠겨 있습니다 ^(로그오프 전까지 풀리지 않음^).
    echo               로그오프 없이 깔 수 있게 옆 폴더에 깝니다 : %INSTDIR%
    echo               옛 폴더는 나중에 로그오프한 뒤 지우면 됩니다.
    echo.
)
if not exist "%INSTDIR%" mkdir "%INSTDIR%"

rem    설정 파일은 이미 있으면 덮어쓰지 않는다 - 현장 값이 들어 있다.
if exist "%INSTDIR%\Ecs.ini"     copy /y "%INSTDIR%\Ecs.ini"     "%INSTDIR%\Ecs.ini.before_setup"     >nul
if exist "%INSTDIR%\WmsDown.ini" copy /y "%INSTDIR%\WmsDown.ini" "%INSTDIR%\WmsDown.ini.before_setup" >nul

rem    돌고 있으면 exe 가 잠겨 복사가 중간에 끊긴다.
rem    (그러면 파일이 몇 개만 들어가 프로그램이 이상하게 뜬다)
set "RUNNING="
for /f "tokens=1 delims=," %%a in ('tasklist /FI "IMAGENAME eq Ecs.exe" /NH /FO CSV 2^>nul') do (
    if /i "%%~a"=="Ecs.exe" set "RUNNING=1"
)
for /f "tokens=1 delims=," %%a in ('tasklist /FI "IMAGENAME eq EcsMain.exe" /NH /FO CSV 2^>nul') do (
    if /i "%%~a"=="EcsMain.exe" set "RUNNING=1"
)
if defined RUNNING (
    echo        [주의] Client 가 돌고 있습니다. 먼저 닫아 주세요.
    echo               Ecs.exe  /  EcsMain.exe
    echo.
    pause
    exit /b 1
)

rem    Client 가 등록한 폰트는 Client 를 닫아도 잠겨 있다(AddFontResource). 먼저 푼다.
rem    풀지 않으면 폰트 파일에서 "액세스가 거부되었습니다" 가 나고 복사가 거기서 끊긴다.
powershell -NoProfile -ExecutionPolicy Bypass -File "%SRC%Release-Fonts.ps1" -Dir "%INSTDIR%" -SrcDir "%SRC%Client" >nul 2>&1

rem    /C 하나가 실패해도 나머지는 계속  /R 읽기전용도 덮어쓴다
xcopy "%SRC%Client\*" "%INSTDIR%\" /E /I /Y /Q /C /R
copy /y "%SRC%Release-Fonts.ps1" "%INSTDIR%\" >nul
copy /y "%SRC%Force-ReleaseFonts.ps1" "%INSTDIR%\" >nul
copy /y "%SRC%폰트잠금_풀기.bat" "%INSTDIR%\" >nul
rem    주소를 나중에 바꿀 수 있게 도우미와 안내문도 함께 둔다
copy /y "%SRC%Set-DbServer.ps1" "%INSTDIR%\" >nul
copy /y "%SRC%확인하는_법.txt" "%INSTDIR%\" >nul
copy /y "%SRC%Uninstall.bat" "%INSTDIR%\" >nul
if errorlevel 1 (
    echo        [주의] 복사에 실패했습니다.
    pause
    exit /b 1
)

rem    정말 다 들어갔는지 ★이름으로★ 대조한다. 빠진 것이 있으면 어느 파일인지 보여 준다.
rem    (개수만 세면 대상 폴더의 다른 파일 때문에 맞는 것처럼 보일 수 있다)
powershell -NoProfile -ExecutionPolicy Bypass -File "%SRC%Release-Fonts.ps1" -Dir "%INSTDIR%" -SrcDir "%SRC%Client" -Check
if errorlevel 1 (
    echo.
    echo        [주의] 위 파일이 들어가지 않았습니다. 이대로 두면 프로그램이 이상하게 뜹니다.
    echo               - Client^(Ecs.exe / EcsMain.exe^) 와 설치 폴더를 연 창을 모두 닫고 다시 실행해 주세요.
    echo               - 폰트^(.ttf^) 가 남아 있으면 Windows 를 로그오프했다가 다시 실행하면 풀립니다.
    echo.
    pause
    exit /b 1
)
echo        모두 들어갔습니다.

rem    설정을 되돌려 준다 - 새 설정은 .new 로 남긴다
if exist "%INSTDIR%\Ecs.ini.before_setup" (
    move /y "%INSTDIR%\Ecs.ini" "%INSTDIR%\Ecs.ini.new" >nul
    move /y "%INSTDIR%\Ecs.ini.before_setup" "%INSTDIR%\Ecs.ini" >nul
    echo        Ecs.ini 는 쓰던 것을 그대로 두었습니다 ^(새 것은 Ecs.ini.new^).
)
if exist "%INSTDIR%\WmsDown.ini.before_setup" (
    move /y "%INSTDIR%\WmsDown.ini" "%INSTDIR%\WmsDown.ini.new" >nul
    move /y "%INSTDIR%\WmsDown.ini.before_setup" "%INSTDIR%\WmsDown.ini" >nul
    echo        WmsDown.ini 는 쓰던 것을 그대로 두었습니다 ^(새 것은 WmsDown.ini.new^).
)
echo.

rem ── 4. DB 서버 주소 적용 ─────────────────────────────────────
rem    ini 한 줄만 바꾸는 일은 배치로 하면 한글이 깨지므로 PowerShell 로 한다.
echo  [4/5] DB 서버 주소를 설정에 적습니다...
powershell -NoProfile -ExecutionPolicy Bypass -File "%SRC%Set-DbServer.ps1" -InstDir "%INSTDIR%" -Server "%DBSERVER%" -Database "%DBNAME%"
if errorlevel 1 (
    echo        [주의] 설정을 적지 못했습니다. 아래 두 곳을 직접 고쳐 주세요.
    echo            %INSTDIR%\Ecs.ini       [DB_2] SERVER=
    echo            %INSTDIR%\WmsDown.ini   [DB Server] SERVERNAME=
)
echo.

rem ── 5. 바탕화면 바로가기 ─────────────────────────────────────
echo  [5/5] 바로가기를 만듭니다...
set "VBS=%TEMP%\lgls_shortcut.vbs"
> "%VBS%" echo Set oWS = WScript.CreateObject^("WScript.Shell"^)
>>"%VBS%" echo sLink = oWS.SpecialFolders^("AllUsersDesktop"^) ^& "\LGLS CLIENT.lnk"
>>"%VBS%" echo Set oLink = oWS.CreateShortcut^(sLink^)
>>"%VBS%" echo oLink.TargetPath = "%INSTDIR%\Ecs.exe"
>>"%VBS%" echo oLink.WorkingDirectory = "%INSTDIR%"
>>"%VBS%" echo oLink.IconLocation = "%INSTDIR%\Ecs.exe,0"
>>"%VBS%" echo oLink.Description = "LGLS 자동창고 운전 화면"
>>"%VBS%" echo oLink.Save
cscript //nologo "%VBS%"
del "%VBS%" >nul 2>&1
echo.

echo  ============================================================
echo   설치가 끝났습니다.
echo  ============================================================
echo.
echo   설치 위치   : %INSTDIR%
echo   DB 서버     : %DBSERVER%
echo   데이터베이스 : %DBNAME%
echo   실행        : 바탕화면의 [LGLS CLIENT]
echo.
echo   지우려면   : %INSTDIR%\Uninstall.bat ^(관리자 권한으로^)
echo.
echo   계정을 쓰려면 아래에 적어 주세요. 비워 두면 Windows 인증으로 붙습니다.
echo     %INSTDIR%\WmsDown.ini   [DB Server] USERID= / PASSWORD=
echo.
pause
endlocal
