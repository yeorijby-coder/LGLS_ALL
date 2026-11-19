@echo off
chcp 949 > nul
setlocal enabledelayedexpansion

rem ===========================================================
rem  LGLS CLIENT 제거   2026-11-19
rem
rem  지우는 일은 되돌릴 수 없다. 묻기 전에 스스로 확인한다.
rem    1) 정말 우리가 깐 폴더인가
rem    2) 프로그램이 돌고 있지 않은가
rem    3) 사람이 무엇을 지울지 보고 승낙했는가
rem ===========================================================

set "INSTDIR=C:\LGLS\CLIENT"
set "RC=0"

echo.
echo  ============================================================
echo   LGLS CLIENT 제거
echo  ============================================================
echo.

rem --- 관리자 권한 ---
net session >nul 2>&1
if errorlevel 1 goto NOADMIN

rem --- [1/4] 지울 폴더 ---
echo  [1/4] 지울 폴더
echo.
echo        지금 정한 곳 : %INSTDIR%
echo.
echo        이대로면 그냥 Enter 를 누르세요.
echo        다른 곳을 지우려면 그 ^"폴더 경로^" 를 적으세요.
echo        (여기는 예/아니오를 적는 자리가 아닙니다)
echo.
set "ANS="
set /p ANS=        입력 : 
rem    경로를 따옴표로 감싸 붙여넣는 일이 많다. 따옴표는 걷어낸다.
if defined ANS set "ANS=!ANS:"=!"
if not "!ANS!"=="" set "INSTDIR=!ANS!"
echo.
echo        지울 폴더 : !INSTDIR!
echo.

rem --- 안전 확인 : 경로가 말이 되는가 ---
if "!INSTDIR!"=="" goto BADPATH
set "CHK=!INSTDIR!"
if "!CHK:~3!"=="" goto ROOTPATH
if not exist "!INSTDIR!" goto NOFOLDER

rem --- 안전 확인 : 정말 우리가 깐 폴더인가 ---
set "ISOURS="
if exist "!INSTDIR!\EcsMain.exe" set "ISOURS=1"
if exist "!INSTDIR!\Ecs.exe"     set "ISOURS=1"
if not defined ISOURS goto NOTOURS

rem --- 안전 확인 : 돌고 있지 않은가 ---
echo  [2/4] 프로그램이 돌고 있는지 봅니다
set "RUNNING="
for /f "tokens=1 delims=," %%a in ('tasklist /FI "IMAGENAME eq Ecs.exe" /NH /FO CSV 2^>nul') do (
    if /i "%%~a"=="Ecs.exe" set "RUNNING=1"
)
for /f "tokens=1 delims=," %%a in ('tasklist /FI "IMAGENAME eq EcsMain.exe" /NH /FO CSV 2^>nul') do (
    if /i "%%~a"=="EcsMain.exe" set "RUNNING=1"
)
if not defined RUNNING goto NOTRUNNING

echo        돌고 있습니다. 먼저 닫아야 지울 수 있습니다.
echo.
set "ANS="
set /p ANS=        지금 닫을까요? y 를 적으면 닫습니다 : 
if /i not "!ANS!"=="y" goto KEEPRUN
taskkill /F /IM EcsMain.exe >nul 2>&1
taskkill /F /IM Ecs.exe     >nul 2>&1
echo        닫았습니다.
goto RUNDONE

:NOTRUNNING
echo        돌고 있지 않습니다.

:RUNDONE
echo.

rem --- [3/4] 설정을 남길까 ---
echo  [3/4] 설정 파일
echo.
echo        Ecs.ini 와 WmsDown.ini 에는 이 PC 의 DB 주소가 들어 있습니다.
echo        남겨 두면 다시 깔 때 주소를 또 넣지 않아도 됩니다.
echo.
set "BACKUP=1"
set "ANS="
set /p ANS=        남겨 둘까요? 남기지 않으려면 n 을 적으세요 : 
if /i "!ANS!"=="n" set "BACKUP="
echo.

rem --- [4/4] 마지막 확인 ---
echo  [4/4] 지울 것을 확인해 주세요
echo.
echo        폴더     : !INSTDIR!
echo                   그 안의 파일을 모두 지웁니다.
echo        바로가기 : 바탕화면의 LGLS CLIENT
echo.
echo        남기는 것
echo          Visual C++ 재배포 패키지와 .NET Framework
echo          다른 프로그램도 쓰므로 건드리지 않습니다.
if defined BACKUP echo          설정 파일 두 개 - 아래 자리에 남깁니다.
echo.
echo        지우면 되돌릴 수 없습니다.
echo.
set "ANS="
set /p ANS=        지우려면 YES 를 적으세요 : 
if /i not "!ANS!"=="yes" goto CANCEL
echo.

rem --- 1. 설정 빼 두기 ---
set "SAVEDIR="
if not defined BACKUP goto SKIPSAVE
echo  설정을 빼 둡니다...
for /f "tokens=1-3 delims=-/. " %%a in ("%DATE%") do set "D8=%%a%%b%%c"
set "T6=%TIME: =0%"
set "T6=!T6:~0,2!!T6:~3,2!"
set "SAVEDIR=!INSTDIR!_설정백업_!D8!_!T6!"
if not exist "!SAVEDIR!" mkdir "!SAVEDIR!" >nul 2>&1
if exist "!INSTDIR!\Ecs.ini"     copy /y "!INSTDIR!\Ecs.ini"     "!SAVEDIR!\" >nul
if exist "!INSTDIR!\WmsDown.ini" copy /y "!INSTDIR!\WmsDown.ini" "!SAVEDIR!\" >nul
echo        !SAVEDIR!
echo.

:SKIPSAVE

rem --- 2. 바로가기 지우기 ---
echo  바로가기를 지웁니다...
set "LNK1=%PUBLIC%\Desktop\LGLS CLIENT.lnk"
set "LNK2=%USERPROFILE%\Desktop\LGLS CLIENT.lnk"
set "LNKGONE=1"
if exist "!LNK1!" (
    del /f /q "!LNK1!" >nul 2>&1
    echo        !LNK1!
    set "LNKGONE="
)
if exist "!LNK2!" (
    del /f /q "!LNK2!" >nul 2>&1
    echo        !LNK2!
    set "LNKGONE="
)
if defined LNKGONE echo        지울 바로가기가 없습니다.
echo.

rem --- 3. 폴더 지우기 ---
echo  프로그램을 지웁니다...
attrib -r -h -s "!INSTDIR!\*" /s /d >nul 2>&1
rem    Client 가 등록한 폰트를 먼저 푼다 - 안 풀면 폰트 파일이 잠긴 채 남아 다음 설치를 막는다
if exist "%~dp0Release-Fonts.ps1" powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0Release-Fonts.ps1" -Dir "!INSTDIR!" >nul 2>&1
cd /d "%SystemRoot%" >nul 2>&1
rmdir /s /q "!INSTDIR!" 2>nul
if exist "!INSTDIR!" goto LEFTOVER
echo        지웠습니다.
goto FINISH

:LEFTOVER
echo        [주의] 다 지우지 못했습니다. 쓰고 있는 파일이 있을 수 있습니다.
echo               !INSTDIR!
echo               열려 있는 창을 닫고 다시 실행해 주세요.
set "RC=1"

:FINISH
echo.
echo  ============================================================
echo   제거가 끝났습니다.
echo  ============================================================
echo.
if defined SAVEDIR echo   남긴 설정 : !SAVEDIR!
if defined SAVEDIR echo     다시 깔고 나서 이 안의 ini 를 설치 폴더에 덮어쓰면
if defined SAVEDIR echo     DB 주소를 다시 넣지 않아도 됩니다.
if defined SAVEDIR echo.
echo   Visual C++ 재배포 패키지와 .NET 은 남겨 두었습니다.
echo   다른 프로그램도 쓰므로 함부로 지우지 않습니다.
echo.
goto END

:NOADMIN
echo  [주의] 관리자 권한으로 실행해 주세요.
echo         이 파일을 오른쪽 단추로 눌러 "관리자 권한으로 실행" 을 고르면 됩니다.
echo.
set "RC=1"
goto END

:BADPATH
echo  [주의] 폴더 경로가 비었습니다. 아무것도 하지 않습니다.
echo.
set "RC=1"
goto END

:ROOTPATH
echo  [주의] 드라이브 맨 위는 지울 수 없습니다.
echo         적으신 것 : !INSTDIR!
echo         [1/4] 에서는 ^"폴더 경로^" 를 적어야 합니다.
echo         그대로 지우려면 아무것도 적지 말고 Enter 만 누르세요.
echo.
set "RC=1"
goto END

:NOFOLDER
echo  그 폴더가 없습니다.
echo         !INSTDIR!
echo  이미 지워졌거나 다른 곳에 깔았을 수 있습니다.
echo.
goto END

:NOTOURS
echo  [주의] 여기는 LGLS CLIENT 를 깐 폴더가 아닌 것 같습니다.
echo         !INSTDIR!
echo         Ecs.exe 도 EcsMain.exe 도 없습니다. 아무것도 하지 않습니다.
echo.
set "RC=1"
goto END

:KEEPRUN
echo        그대로 두었습니다. 프로그램을 닫은 뒤 다시 실행해 주세요.
echo.
set "RC=1"
goto END

:CANCEL
echo.
echo   YES 가 아니므로 아무것도 지우지 않았습니다.
echo.
goto END

:END
pause
endlocal
exit /b %RC%
