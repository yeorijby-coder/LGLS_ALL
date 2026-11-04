param([string]$Track = "104", [int]$Cid = 17110104, [string]$Tag = "nolugg", [string]$OutDir = "D:\project\LGLS\Renewal\docs\산출물_20261110\shots")
. "$PSScriptRoot\Win.ps1"
$s = Get-ProcMain "EQP_SIM" "EQP_SIM*"
$m = Get-ProcMain "EcsMain" "Ecs V*"; $pidE = [W]::Pid($m)
$l1 = Find-Child $s "트랙/포트" "STATIC"; $e1 = Find-Near $s $l1 "EDIT"
$l2 = Find-Child $s "작업번호" "STATIC"; $e2 = Find-Near $s $l2 "EDIT"
$bg = Find-Child $s "화물 생성" "BUTTON"
Set-Text $e1 $Track; Set-Text $e2 ""; Click-Btn $bg
Write-Output "cargo created at $Track (no job no)"
Start-Sleep 25
Cap-Win $m (Join-Path $OutDir ("main_" + $Tag + "_1105.png"))
$views = [W]::Children($m) | Where-Object { ([W]::Cls($_)) -eq '#32770' } | ForEach-Object { $r = New-Object W+R; [void][W]::GetWindowRect($_, [ref]$r); if (($r.Rt-$r.L) -ge 1500) { $_ } } | Select-Object -First 1
$b = Snap-Tops; [void][W]::PostMessage($views, 0x40A, [IntPtr]$Cid, [IntPtr]::Zero); Start-Sleep 4
$d = New-Tops $b | Where-Object { [W]::Pid($_) -eq $pidE } | Select-Object -First 1
if (-not $d) { Write-Output "dlg none"; exit }
Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_1105.png"))
$hb = [W]::GetDlgItem($d, 8245); Write-Output ("btn [{0}]" -f [W]::Text($hb))
Click-Btn $hb
[void](Answer-Box $pidE "Ecs" 6 6 (Join-Path $OutDir ("ask_8245_" + $Tag + "_1105.png")))
[void](Answer-Box $pidE "Ecs" 1 6 (Join-Path $OutDir ("result_8245_" + $Tag + "_1105.png")))
Start-Sleep 25
Cap-Win $d (Join-Path $OutDir ("dlg_" + $Cid + "_" + $Tag + "_after_1105.png"))
Cap-Win $m (Join-Path $OutDir ("main_" + $Tag + "_after_1105.png"))
[void][W]::PostMessage($d, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero)
