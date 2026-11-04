param([int]$Cid = 17120901, [int]$BtnId = 1087, [string]$Tag = "instop", [string]$OutDir = "D:\project\LGLS\Renewal\docs\산출물_20261110\shots")
. "$PSScriptRoot\Win.ps1"
$m = Get-ProcMain "EcsMain" "Ecs V*"; $pidE = [W]::Pid($m)
$views = [W]::Children($m) | Where-Object { ([W]::Cls($_)) -eq '#32770' } | ForEach-Object { $r = New-Object W+R; [void][W]::GetWindowRect($_, [ref]$r); if (($r.Rt-$r.L) -ge 1500) { $_ } } | Select-Object -First 1
function Open-Dlg { $b = Snap-Tops; [void][W]::PostMessage($views, 0x40A, [IntPtr]$Cid, [IntPtr]::Zero); Start-Sleep 4; return (New-Tops $b | Where-Object { [W]::Pid($_) -eq $pidE } | Select-Object -First 1) }
$d = Open-Dlg
if (-not $d) { Write-Output "dlg none"; exit }
$hb = [W]::GetDlgItem($d, $BtnId); Write-Output ("dlg [{0}] btn={1} [{2}]" -f [W]::Text($d), $hb, [W]::Text($hb))
Click-Btn $hb
$ok = Answer-Box $pidE "Ecs" 6 6 (Join-Path $OutDir ("ask_" + $BtnId + "_" + $Tag + "_1105.png"))
$ok2 = Answer-Box $pidE "Ecs" 1 6 (Join-Path $OutDir ("result_" + $BtnId + "_" + $Tag + "_1105.png"))
Start-Sleep 20
Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_1105.png"))
Cap-Win $m (Join-Path $OutDir ("main_" + $Tag + "_1105.png"))
# 해제(토글)
Click-Btn $hb
[void](Answer-Box $pidE "Ecs" 6 6 (Join-Path $OutDir ("ask_" + $BtnId + "_" + $Tag + "_off_1105.png")))
[void](Answer-Box $pidE "Ecs" 1 6 "")
Start-Sleep 20
Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_off_1105.png"))
Cap-Win $m (Join-Path $OutDir ("main_" + $Tag + "_off_1105.png"))
[void][W]::PostMessage($d, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero)
