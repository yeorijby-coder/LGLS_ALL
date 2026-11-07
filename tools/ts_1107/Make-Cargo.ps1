# EQP_SIM [화물 생성] : 트랙에 작업번호 붙은 화물을 만든다
param([string]$Track = "124", [string]$Lugg = "")
. "$PSScriptRoot\Win.ps1"
$s = Get-ProcMain "EQP_SIM" "EQP_SIM*"
$l1 = Find-Child $s "트랙/포트" "STATIC"; $e1 = Find-Near $s $l1 "EDIT"
$l2 = Find-Child $s "작업번호" "STATIC"; $e2 = Find-Near $s $l2 "EDIT"
$bg = Find-Child $s "화물 생성" "BUTTON"
Set-Text $e1 $Track; Set-Text $e2 $Lugg; Click-Btn $bg
Start-Sleep 2
$log = Get-ChildItem "D:\project\LGLS\Renewal\SIM\EQP_SIM\bin\Debug\Data\eqp_sim_*.log" | Sort-Object LastWriteTime | Select-Object -Last 1
Get-Content $log.FullName -Tail 4 | Where-Object { $_ -match '화물' }
