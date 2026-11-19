@echo off
chcp 949 >nul
setlocal
rem ===========================================================
rem  LGLS CLIENT 화면 배율 방식   2026-10-01
rem  4K 화면에서 글자나 그림이 어긋나 보일 때 쓰는 대안이다.
rem  기본은 1 이고, Ecs.ini 의 DISPLAY 설정으로 맞춘다.
rem  2 와 3 은 Windows 가 창을 통째로 늘려 주는 방식이다.
rem ===========================================================
set "EXE=%~dp0EcsMain.exe"
set "KEY=HKCU\Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers"
echo.
echo  LGLS CLIENT 화면 배율 방식
echo  대상 = %EXE%
echo.
echo   1  프로그램이 직접 맞춘다 - 기본. 글자가 또렷하다. Ecs.ini 의 DISPLAY 로 조절한다.
echo   2  Windows 가 통째로 늘린다 - 모든 것이 같은 비율로 커진다. 조금 흐릿하다.
echo   3  Windows 가 늘리되 글자는 또렷하게 - 2 와 같고 글자만 다시 그린다.
echo.
echo  지금 값
reg query "%KEY%" /v "%EXE%" 2>nul | findstr /i "DPI" || echo     1 - 따로 정한 것이 없다
echo.
set "ANS="
set /p ANS= 번호를 적으세요. 그냥 Enter 는 그대로 둔다 : 
if "%ANS%"=="1" reg delete "%KEY%" /v "%EXE%" /f >nul 2>&1
if "%ANS%"=="1" echo  1 로 했습니다.
if "%ANS%"=="2" reg add "%KEY%" /v "%EXE%" /t REG_SZ /d "~ DPIUNAWARE" /f >nul
if "%ANS%"=="2" echo  2 로 했습니다.
if "%ANS%"=="3" reg add "%KEY%" /v "%EXE%" /t REG_SZ /d "~ GDIDPISCALING DPIUNAWARE" /f >nul
if "%ANS%"=="3" echo  3 으로 했습니다.
echo.
echo  Client 를 다시 띄우면 반영됩니다.
echo.
pause
endlocal
