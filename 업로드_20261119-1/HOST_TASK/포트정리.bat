@echo off
rem ============================================================
rem  포트 정리.bat  -  HOST_TASK [포트 정리] 버튼과 같은 순서로 묻는 명령 파일
rem   1) netstat 결과 보여 주기
rem   2) 다른 PID TASKKILL 확인
rem   3) 자기 자신 HOST_TASK 종료 확인 - 예 이면 마지막에 종료
rem   4) 죽은 PID 가 쥔 소켓 핸들 닫기 확인 - HandleHunter.exe 필요
rem   5) LISTENING 은 끊을 수 없음 확인 후 CLOSE_WAIT 등 정리 - 관리자 권한 필요
rem  같은 폴더에 HandleHunter.exe 가 있어야 4, 5 단계가 동작합니다.
rem  사용법: 포트정리.bat [포트]   포트를 안 주면 같은 폴더 EcsComA.ini 의 LocalPort, 그것도 없으면 9911
rem ============================================================
setlocal EnableExtensions EnableDelayedExpansion
chcp 949 >nul
title 포트 정리
cd /d "%~dp0"
rem 같은 폴더의 도구를 경로까지 붙여 부른다 - 현재 폴더 검색이 꺼진 PC 에서도 되게
set "HH=%~dp0HandleHunter.exe"

set "HOSTEXE=TASK_LFC10_G1_ECSCOM.exe"
set "PORT=%~1"
if defined PORT goto :PORTOK
if not exist EcsComA.ini goto :PORTOK
for /f "usebackq tokens=2 delims==" %%a in (`findstr /b /i /c:"LocalPort=" EcsComA.ini`) do set "PORT=%%a"
:PORTOK
if not defined PORT set "PORT=9911"
set "PORT=%PORT: =%"

set "TMPD=%TEMP%\portclean_%RANDOM%"
mkdir "%TMPD%" >nul 2>&1

set "ISADMIN="
fltmc >nul 2>&1 && set "ISADMIN=1"

set "KILLED="
set "EXITSELF="
set "HANDLEDONE="
set "SELFPID="
set "KILLIST="
set "DEADLIST="

rem ---------- 1) netstat ----------
echo.
echo ============================================================
echo  포트 정리   수신 포트 %PORT%     netstat -ano ^| findstr :%PORT%
echo ============================================================
netstat -ano | findstr :%PORT% > "%TMPD%\ns.txt"
if errorlevel 1 goto :NOROWS
type "%TMPD%\ns.txt"
goto :COLLECT

:NOROWS
echo  해당 접속이 없습니다.
echo.
echo  수신 포트 %PORT% 에 남은 접속이 없습니다.
goto :END

rem ---------- PID 분류: 살아 있는 PID / 죽은 PID / HOST_TASK 자신 ----------
:COLLECT
set "PIDLIST="
for /f "tokens=2,5" %%b in ('netstat -ano ^| findstr /r /c:":%PORT% "') do call :ADDPID "%%b" "%%c"
for %%p in (%PIDLIST%) do call :CLASSIFY %%p

rem ---------- 2) 다른 PID TASKKILL ----------
if not defined KILLIST goto :ASKSELF
echo.
echo ------------------------------------------------------------
echo  netstat 결과의 아래 PID 를 TASKKILL 하겠습니까?
echo ------------------------------------------------------------
for %%p in (!KILLIST!) do echo     PID %%p   !N_%%p!
echo.
echo   예     = 위 프로세스를 강제 종료합니다 - taskkill /F /PID
echo   아니요 = 종료하지 않고 다음 단계로 갑니다
call :YN "TASKKILL 하겠습니까?"
if not defined YES goto :ASKSELF
set "KILLED=1"
for %%p in (!KILLIST!) do call :KILL1 %%p
ping -n 2 127.0.0.1 >nul

rem ---------- 3) 자기 자신 ----------
:ASKSELF
if not defined SELFPID goto :AFTERSELF
echo.
echo ------------------------------------------------------------
echo  현재 구동중인 프로그램 입니다.
echo     PID %SELFPID%   %HOSTEXE%
echo  이 프로그램을 종료하시겠습니까?
echo ------------------------------------------------------------
echo   예     = 다른 작업을 모두 마친 뒤 마지막에 이 프로그램을 종료합니다
echo   아니요 = 종료하지 않습니다
call :YN "이 프로그램을 종료하시겠습니까?"
if defined YES set "EXITSELF=1"

:AFTERSELF
if defined KILLED goto :FINISH

rem ---------- 4) 죽은 PID 가 쥔 소켓 핸들 닫기 ----------
if not defined DEADLIST goto :ASKTCP
if exist "%HH%" goto :HUNT
echo.
echo  죽은 PID 가 쥔 소켓 핸들 닫기: HandleHunter.exe 가 이 폴더에 없어 건너뜁니다.
goto :ASKTCP
:HUNT
for %%p in (!DEADLIST!) do call :HANDLE1 %%p
if defined HANDLEDONE goto :FINISH

rem ---------- 5) CLOSE_WAIT 등 정리 ----------
:ASKTCP
if exist "%HH%" goto :TCPOK
echo.
echo  HandleHunter.exe 가 이 폴더에 없어 접속 정리를 할 수 없습니다.
goto :FINISH
:TCPOK
"%HH%" %PORT% --tcp-targets > "%TMPD%\t.txt" 2>&1
set "CNT=%errorlevel%"
echo.
echo ------------------------------------------------------------
echo  끊을 수 있는 접속 - CLOSE_WAIT 또는 소유 프로세스가 없는 접속
echo ------------------------------------------------------------
type "%TMPD%\t.txt"
if "%CNT%"=="0" goto :NOTGT
if defined ISADMIN goto :ADMINOK
echo.
echo  끊을 수 있는 접속이 %CNT% 건 있지만 관리자 권한이 아니라 끊을 수 없습니다.
echo  이 파일을 마우스 오른쪽 단추 - 관리자 권한으로 실행 하세요.
goto :FINISH
:ADMINOK
echo.
call :YN "LISTENING은 끊을 수 없습니다. 계속 진행하겠습니까?"
if not defined YES goto :FINISH
echo.
"%HH%" %PORT% --tcp-delete
goto :FINISH
:NOTGT
echo.
echo  끊을 수 있는 접속이 없습니다.
if defined DEADLIST echo  소유 프로세스가 없는 LISTENING 이 있으면 이 소켓은 끊을 수 없습니다. 그 소켓을 물려받아 쥔 프로세스를 찾아 닫아야 합니다.

rem ---------- 마무리: 정리 뒤 netstat, 자기 종료 ----------
:FINISH
echo.
echo ------------------------------------------------------------
echo  정리 뒤 netstat -ano ^| findstr :%PORT%
echo ------------------------------------------------------------
netstat -ano | findstr :%PORT%
if errorlevel 1 echo  해당 접속이 없습니다.
if not defined EXITSELF goto :END

echo.
echo  요청에 따라 %HOSTEXE% 를 종료합니다.  종료 확인 창이 뜨면 확인을 누르세요.
taskkill /PID %SELFPID% >nul 2>&1
set /a WAITN=0
:WAITSELF
tasklist /FI "PID eq %SELFPID%" 2>nul | findstr /c:"%SELFPID%" >nul
if errorlevel 1 goto :SELFGONE
set /a WAITN+=1
if %WAITN% GEQ 10 goto :SELFSTUCK
ping -n 2 127.0.0.1 >nul
goto :WAITSELF
:SELFSTUCK
echo.
call :YN "아직 종료되지 않았습니다. 강제 종료하겠습니까?"
if not defined YES goto :END
taskkill /F /PID %SELFPID%
goto :END
:SELFGONE
echo  %HOSTEXE% 가 종료되었습니다.
goto :END

:END
echo.
rd /s /q "%TMPD%" >nul 2>&1
pause
exit /b 0

rem ====================== 부분 루틴 ======================

rem :ADDPID "<로컬주소:포트>" "<PID>"  - 이 포트가 로컬인 줄의 PID 를 모은다(IPv4)
:ADDPID
set "LA=%~1"
set "AP=%~2"
for /f "tokens=2 delims=:" %%q in ("%LA%") do if "%%q"=="%PORT%" (
  echo !PIDLIST! | findstr /c:" %AP% " >nul || set "PIDLIST=!PIDLIST! %AP% "
)
exit /b 0

rem :CLASSIFY <PID>
:CLASSIFY
set "CP=%~1"
if "%CP%"=="0" exit /b 0
set "CNAME="
for /f "tokens=1 delims=," %%n in ('tasklist /FI "PID eq %CP%" /NH /FO CSV 2^>nul') do set "CNAME=%%~n"
set "ISEXE="
rem echo 와 | 사이에 공백을 두면 줄 끝에 공백이 붙어 /e 가 맞지 않는다
echo !CNAME!| findstr /i /e /c:".exe" >nul && set "ISEXE=1"
if not defined ISEXE (
  set "DEADLIST=!DEADLIST! %CP%"
  exit /b 0
)
if /i "!CNAME!"=="%HOSTEXE%" (
  set "SELFPID=%CP%"
  exit /b 0
)
for %%s in (system svchost lsass services wininit winlogon csrss smss explorer dwm sqlservr) do (
  if /i "!CNAME!"=="%%s.exe" (
    echo  제외: PID %CP% !CNAME! - 시스템 보호 프로세스
    exit /b 0
  )
)
set "KILLIST=!KILLIST! %CP%"
set "N_%CP%=!CNAME!"
exit /b 0

rem :KILL1 <PID>
:KILL1
taskkill /F /PID %~1
exit /b 0

rem :HANDLE1 <죽은 PID>  - 자손이 쥔 핸들을 찾고 닫을지 묻는다
:HANDLE1
if defined HANDLEDONE exit /b 0
set "DP=%~1"
"%HH%" %PORT% --owner %DP% > "%TMPD%\hh.txt" 2>&1
findstr /r /c:"^  PID " "%TMPD%\hh.txt" > "%TMPD%\hh_h.txt"
if errorlevel 1 goto :HANDLE1_NONE
echo.
echo ------------------------------------------------------------
echo  PID %DP% 프로세스는 이미 없지만, 그 프로세스가 띄운 프로세스가
echo  포트 %PORT% 소켓의 핸들을 쥐고 있습니다.
echo ------------------------------------------------------------
type "%TMPD%\hh_h.txt"
echo.
echo   이 핸들만 닫습니다. 프로세스는 종료하지 않습니다.
call :YN "닫겠습니까?"
if not defined YES exit /b 0
set "HANDLEDONE=1"
echo.
"%HH%" %PORT% --owner %DP% --close
exit /b 0
:HANDLE1_NONE
echo.
echo  죽은 PID %DP% 의 자손 프로세스에서 포트 %PORT% 를 쥔 핸들을 찾지 못했습니다.
exit /b 0

rem :YN "<질문>"  - YES 변수를 정한다(기본 아니요)
:YN
set "ANS="
set /p "ANS=%~1 [Y/N, 기본 N] : "
set "YES="
if /i "!ANS!"=="Y" set "YES=1"
if /i "!ANS!"=="YES" set "YES=1"
if "!ANS!"=="예" set "YES=1"
exit /b 0
