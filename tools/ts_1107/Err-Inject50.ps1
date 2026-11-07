param([int]$Combo = 27, [string]$Code = "11", [string]$Tag = "cverr", [int]$Cid = 17110124, [string]$OutDir = "D:\project\LGLS\Renewal\docs\산출물_20261110\shots")
. "$PSScriptRoot\Win.ps1"
$s = Get-ProcMain "EQP_SIM" "EQP_SIM*"
$m = Get-ProcMain "EcsMain" "Ecs V*"
$pidE = [W]::Pid($m)
$lbl = Find-Child $s "설비 에러 주입" "STATIC"
$cb = Find-Near $s $lbl "COMBOBOX"
$ed = Find-Near $s $lbl "EDIT"
$btn = Find-Child $s "에러 발생" "BUTTON"
$rel = Find-Child $s "선택 설비 해제" "BUTTON"
Set-Combo $cb $Combo; Set-Text $ed $Code
$before = Snap-Tops
Click-Btn $btn
Write-Output "injected combo=$Combo code=$Code"
Start-Sleep 50
# 새 창(알람 경고창 등)
$new = New-Tops $before | Where-Object { [W]::Pid($_) -eq $pidE }
$i=0; foreach ($h in $new) { Write-Output ("new: [{0}] {1}" -f [W]::Text($h), [W]::Cls($h)); Cap-Win $h (Join-Path $OutDir ("alarm_" + $Tag + "_1107" + $(if($i){"_$i"}) + ".png")); $i++ }
Cap-Win $m (Join-Path $OutDir ("main_" + $Tag + "_1107.png"))
# 상태창
$views = [W]::Children($m) | Where-Object { ([W]::Cls($_)) -eq '#32770' } | ForEach-Object { $r = New-Object W+R; [void][W]::GetWindowRect($_, [ref]$r); if (($r.Rt-$r.L) -ge 1500) { $_ } } | Select-Object -First 1
$b2 = Snap-Tops
[void][W]::PostMessage($views, 0x40A, [IntPtr]$Cid, [IntPtr]::Zero)
Start-Sleep 4
$d = New-Tops $b2 | Where-Object { [W]::Pid($_) -eq $pidE } | Select-Object -First 1
if ($d) { Write-Output ("dlg: [{0}]" -f [W]::Text($d)); Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_1107.png")); [void][W]::PostMessage($d, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero) } else { Write-Output "dlg: none" }
Start-Sleep 1
# 해제
Click-Btn $rel
Write-Output "released"
Start-Sleep 20
foreach ($h in $new) { if ([W]::IsWindowVisible($h)) { [void][W]::PostMessage($h, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero); Write-Output ("closed [{0}]" -f [W]::Text($h)) } }
Cap-Win $m (Join-Path $OutDir ("main_" + $Tag + "_after_1107.png"))
