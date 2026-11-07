param([string]$OutDir = "D:\project\LGLS\Renewal\docs\산출물_20261110\shots")
. "$PSScriptRoot\Win.ps1"
$s = Get-ProcMain "EQP_SIM" "EQP_SIM*"
$m = Get-ProcMain "EcsMain" "Ecs V*"; $pidE = [W]::Pid($m)
$views = [W]::Children($m) | Where-Object { ([W]::Cls($_)) -eq '#32770' } | ForEach-Object { $r = New-Object W+R; [void][W]::GetWindowRect($_, [ref]$r); if (($r.Rt-$r.L) -ge 1500) { $_ } } | Select-Object -First 1
function Open-Cid([int]$cid) { $b = Snap-Tops; [void][W]::PostMessage($views, 0x40A, [IntPtr]$cid, [IntPtr]::Zero); Start-Sleep 4; return (New-Tops $b | Where-Object { [W]::Pid($_) -eq $pidE } | Select-Object -First 1) }
function Open-Cmd([int]$id, [int]$wait=4) { $b = Snap-Tops; [void][W]::PostMessage($m, $WM_COMMAND, [IntPtr]$id, [IntPtr]::Zero); Start-Sleep $wait; return (New-Tops $b | Where-Object { [W]::Pid($_) -eq $pidE } | Select-Object -First 1) }
# 1. 신호등(PLC) 클릭 → ping 창
$d = Open-Cmd 59502 9
if ($d) { Write-Output ("lamp: [{0}] {1}" -f [W]::Text($d), [W]::Cls($d)); Cap-Win $d (Join-Path $OutDir "lamp_plc_1107.png"); [void][W]::PostMessage($d, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero); Start-Sleep 1 } else { Write-Output "lamp: none" }
# 2. 설비에러이력 창 [조회]
$d = Open-Cmd 35051 4
if ($d) { $q = Find-Child $d "조회" "Button"; Write-Output ("eqp_his btn={0}" -f $q); if ($q -ne $null) { Click-Btn $q; Start-Sleep 4 }; Cap-Win $d (Join-Path $OutDir "log_eqp_his_1107.png"); [void][W]::PostMessage($d, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero); Start-Sleep 1 }
# 3. C/V#2 방향 글자 더블클릭 → 확인창 (아니오)
$vr = New-Object W+R; [void][W]::GetWindowRect($views, [ref]$vr)
$sx = 1037 - 8; $sy = 355 - 8; $cx = $sx - $vr.L; $cy = $sy - $vr.T
Write-Output "view=$($vr.L),$($vr.T) client=$cx,$cy"
$lp = [IntPtr](($cy -shl 16) -bor $cx)
[void][W]::PostMessage($views, 0x201, [IntPtr]1, $lp); [void][W]::PostMessage($views, 0x202, [IntPtr]0, $lp)
[void][W]::PostMessage($views, 0x203, [IntPtr]1, $lp); [void][W]::PostMessage($views, 0x202, [IntPtr]0, $lp)
$ok = Answer-Box $pidE "Ecs" 7 6 (Join-Path $OutDir "ask_dir_cv2_1107.png"); Write-Output "dir ask=$ok"
