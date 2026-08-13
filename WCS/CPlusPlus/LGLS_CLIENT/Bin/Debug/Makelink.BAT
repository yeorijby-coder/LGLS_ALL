@echo off

set lnk=%userprofile%\desktop\Ecs.lnk
set tgpath=%ProgramFiles(x86)%\ECS\EcsClient.exe
set icon=

set command=^
$objshell = New-object -ComObject WScript.Shell;^
$lnk = $objshell.CreateShortcut('%lnk%');^
$lnk.TargetPath = '%tgpath%';
if not "%icon%"=="" set command=%command%$lnk.IconLocation ^= '%icon%';
set command=%command%$lnk.Save();

powershell -command "& {%command%}"
pause