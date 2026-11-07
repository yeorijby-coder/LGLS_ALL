# C/V 상태창 H/S 신호 단추 → 조건 칸 깜빡임 캡처 (2026-11-07)
#   -Cid 17110104 -BtnId 1342(입고 H/S) / 1343(출고 H/S) / 30014(RTV 출발 H/S) / 30015(RTV 도착 H/S)
param([int]$Cid = 17110104, [int]$BtnId = 1342, [string]$Tag = "hsblink", [string]$OutDir = "D:\project\LGLS\Renewal\docs\산출물_20261110\shots")
. "$PSScriptRoot\Win.ps1"
$m = Get-ProcMain "EcsMain" "Ecs V*"; $pidE = [W]::Pid($m)
$views = [W]::Children($m) | Where-Object { ([W]::Cls($_)) -eq '#32770' } | ForEach-Object { $r = New-Object W+R; [void][W]::GetWindowRect($_, [ref]$r); if (($r.Rt-$r.L) -ge 1500) { $_ } } | Select-Object -First 1
$b = Snap-Tops; [void][W]::PostMessage($views, 0x40A, [IntPtr]$Cid, [IntPtr]::Zero); Start-Sleep 4
$d = New-Tops $b | Where-Object { [W]::Pid($_) -eq $pidE } | Select-Object -First 1
if (-not $d) { Write-Output "dlg none"; exit }
Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_0.png"))
$hb = [W]::GetDlgItem($d, $BtnId); Write-Output ("btn {0} [{1}]" -f $hb, [W]::Text($hb))
$t0 = Get-Date
Click-Btn $hb
Start-Sleep -Milliseconds 350
Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_hide1.png")); Write-Output ("+{0:N1}s hide1" -f ((Get-Date)-$t0).TotalSeconds)
Start-Sleep -Milliseconds 1000
Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_show1.png")); Write-Output ("+{0:N1}s show1" -f ((Get-Date)-$t0).TotalSeconds)
Start-Sleep -Milliseconds 1000
Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_hide2.png")); Write-Output ("+{0:N1}s hide2" -f ((Get-Date)-$t0).TotalSeconds)
Start-Sleep -Milliseconds 5000
Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_end.png")); Write-Output ("+{0:N1}s end" -f ((Get-Date)-$t0).TotalSeconds)
[void][W]::PostMessage($d, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero)
