param([string]$OutDir = "D:\project\LGLS\Renewal\docs\산출물_20261110\shots")
. "$PSScriptRoot\Win.ps1"
$m = Get-ProcMain "EcsMain" "Ecs V*"
$pidE = [W]::Pid($m)
$list = @(
  @{ id = 35011; name = "ecs_commdef" },
  @{ id = 35015; name = "ecs_logdel" },
  @{ id = 35021; name = "view_joblist" },
  @{ id = 35024; name = "view_search" },
  @{ id = 35046; name = "manual_job" },
  @{ id = 35050; name = "log_io" },
  @{ id = 35051; name = "log_eqp_his" },
  @{ id = 35053; name = "log_mes" },
  @{ id = 35055; name = "log_client" },
  @{ id = 35056; name = "log_job_his" },
  @{ id = 35090; name = "alarm_show" }
)
foreach ($it in $list) {
  $before = @{}; foreach ($h in [W]::Tops($pidE)) { $before[[int64]$h] = 1 }
  [void][W]::PostMessage($m, $WM_COMMAND, [IntPtr]$it.id, [IntPtr]::Zero)
  Start-Sleep 4
  $new = [W]::Tops($pidE) | Where-Object { -not $before.ContainsKey([int64]$_) }
  if (-not $new) { Write-Output ("{0} ({1}) : no window" -f $it.name, $it.id); continue }
  $i = 0
  foreach ($h in $new) {
    $t = [W]::Text($h)
    $suffix = if ($i -eq 0) { "" } else { "_$i" }
    Write-Output ("{0} ({1}) : [{2}] {3}" -f $it.name, $it.id, $t, [W]::Cls($h))
    Cap-Win $h (Join-Path $OutDir ($it.name + "_1105" + $suffix + ".png"))
    $i++
  }
  foreach ($h in $new) { [void][W]::PostMessage($h, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero) }
  Start-Sleep 2
  # 닫기 확인창(예/아니오)이 뜨면 '아니오' 또는 '확인'
  $left = [W]::Tops($pidE) | Where-Object { -not $before.ContainsKey([int64]$_) }
  foreach ($h in $left) { $hb = [W]::GetDlgItem($h, 7); if ($hb -eq [IntPtr]::Zero) { $hb = [W]::GetDlgItem($h, 1) }; if ($hb -ne [IntPtr]::Zero) { Click-Btn $hb; Write-Output ("  closed leftover [{0}]" -f [W]::Text($h)) } else { [void][W]::PostMessage($h, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero) } }
  Start-Sleep 1
}
