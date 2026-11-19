@echo off
chcp 949 >nul
setlocal
rem ===========================================================
rem  옛 Client 가 잠가 놓은 폰트 파일을 로그오프 없이 푼다   2026-10-01
rem    1 GDI 등록 해제  2 파일을 잡은 프로그램 닫기(탐색기는 다시 띄움)
rem    3 글꼴 캐시 서비스 재시작  4 확인
rem  사용 : 폰트잠금_풀기.bat [설치 폴더]   (생략하면 이 파일이 있는 폴더)
rem ===========================================================
set "DIR=%~1"
if "%DIR%"=="" set "DIR=%~dp0"
if "%DIR:~-1%"=="\" set "DIR=%DIR:~0,-1%"
net session >nul 2>&1
if errorlevel 1 (
    echo  [주의] 관리자 권한으로 실행해 주세요. 오른쪽 단추 - 관리자 권한으로 실행
    pause
    exit /b 1
)
echo.
echo  폰트 잠금 풀기 : %DIR%
echo  파일을 잡고 있는 프로그램이 있으면 이름을 보여 주고 닫을지 묻습니다.
echo  Windows 탐색기가 잡고 있으면 닫았다가 다시 띄웁니다. 브라우저 등은 닫히므로 저장할 것은 먼저 저장하세요.
echo.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0Force-ReleaseFonts.ps1" -Dir "%DIR%"
if errorlevel 1 (
    echo.
    echo  [안내] 그래도 잠겨 있습니다 - 이 세션 자체가 잡고 있는 경우라 로그오프 전엔 풀리지 않습니다.
    echo         Setup.bat 은 이런 폴더를 만나면 옆 이름 폴더에 깔아 주므로 설치는 그대로 할 수 있습니다.
) else (
    echo.
    echo  풀렸습니다. 이제 이 폴더에 설치하거나 지울 수 있습니다.
)
echo.
pause
endlocal
