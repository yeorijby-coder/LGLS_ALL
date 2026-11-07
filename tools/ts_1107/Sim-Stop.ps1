# EQP_SIM 완료처리 시험 체크박스(①~④) / 상황 A 를 Space 키로 토글, 시뮬 로그로 확인
param([int]$N = 2, [switch]$FaultA, [int]$Count = -1)
. "$PSScriptRoot\Win.ps1"
$s = Get-ProcMain "EQP_SIM" "EQP_SIM*"
if (-not $s) { Write-Output "EQP_SIM none"; exit }
$txt = @{ 1 = "① 크레인*"; 2 = "② 통로*"; 3 = "③ RGV*"; 4 = "④ 출고대*" }[$N]
if ($FaultA) { $txt = "상황 A*" }
$c = Find-Child $s $txt "BUTTON"
if ($c -eq $null) { Write-Output "checkbox not found: $txt"; exit }
if ($FaultA -and $Count -ge 0) {
  $lbl = Find-Child $s "상황 A*" "BUTTON"; $nud = Find-Near $s $lbl "EDIT"
  if ($nud -ne [IntPtr]::Zero) { Set-Text $nud ([string]$Count); Write-Output "count=$Count" }
}
# WinForms CheckBox : WM_KEYDOWN/UP VK_SPACE
[void][W]::PostMessage($c, 0x100, [IntPtr]0x20, [IntPtr]::Zero); Start-Sleep -Milliseconds 120
[void][W]::PostMessage($c, 0x101, [IntPtr]0x20, [IntPtr]::Zero)
Start-Sleep 1
$log = Get-ChildItem "D:\project\LGLS\Renewal\SIM\EQP_SIM\bin\Debug\Data\eqp_sim_*.log" | Sort-Object LastWriteTime | Select-Object -Last 1
Get-Content $log.FullName -Tail 3 | Where-Object { $_ -match '시험정지|고장 주입' }
