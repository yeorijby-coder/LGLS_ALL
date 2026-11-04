param([string]$Where = "job_status='15' AND dest_pos IN ('122','126','129')", [string]$Tag = "st15out", [int]$WaitSec = 360, [int]$Settle = 25, [string]$Lugg = "", [int]$StableSec = 0, [string]$DlgCids = "", [string]$OutDir = "D:\project\LGLS\Renewal\docs\산출물_20261110\shots")
. "$PSScriptRoot\Win.ps1"
function Q([string]$q) { & sqlcmd -S 127.0.0.1,1435 -U LGLS_IO -P LGLS_IO -d LGLS_MCS_IO -h -1 -W -s '|' -Q ("SET NOCOUNT ON; " + $q) }
$m = Get-ProcMain "EcsMain" "Ecs V*"; $pidE = [W]::Pid($m)
$views = [W]::Children($m) | Where-Object { ([W]::Cls($_)) -eq '#32770' } | ForEach-Object { $r = New-Object W+R; [void][W]::GetWindowRect($_, [ref]$r); if (($r.Rt-$r.L) -ge 1500) { $_ } } | Select-Object -First 1
# 1. 대상 작업 기다리기
$row = $null
if ($Lugg -ne "") { $row = (Q "SELECT TOP 1 lugg_no, job_status, start_pos, dest_pos FROM job_mst WHERE lugg_no='$Lugg'") }
else {
  for ($t=0; $t -lt $WaitSec; $t += 5) {
    $row = (Q "SELECT TOP 1 lugg_no, job_status, start_pos, dest_pos FROM job_mst WHERE $Where") | Where-Object { $_ -match '\|' } | Select-Object -First 1
    if ($row) {
      if ($StableSec -le 0) { break }
      Start-Sleep $StableSec; $t += $StableSec
      $row2 = (Q "SELECT TOP 1 lugg_no, job_status, start_pos, dest_pos FROM job_mst WHERE $Where") | Where-Object { $_ -match '\|' } | Select-Object -First 1
      if ($row2 -eq $row) { Write-Output "stable ${StableSec}s: $row"; break }
      Write-Output "changed: $row -> $row2"; $row = $null
    }
    Start-Sleep 5
  }
}
if (-not $row) { Write-Output "not found: $Where"; exit }
$p = ($row -split '\|') | ForEach-Object { $_.Trim() }
$lugg = $p[0]; $sta = $p[1]; $dest = $p[3]
Write-Output "found lugg=$lugg sta=$sta dest=$dest"
Start-Sleep $Settle
Cap-Win $m (Join-Path $OutDir ("main_" + $Tag + "_1105.png"))
Q "SELECT lugg_no, job_status, start_pos, dest_pos, hs_track_no FROM job_mst ORDER BY lugg_no" | Write-Output
# 2. 도착 트랙 C/V 상태창
if ($dest -match '^\d{3}$') {
  $cid = [int]("1711" + $dest.PadLeft(4,'0'))
  $b = Snap-Tops; [void][W]::PostMessage($views, 0x40A, [IntPtr]$cid, [IntPtr]::Zero); Start-Sleep 4
  $d = New-Tops $b | Where-Object { [W]::Pid($_) -eq $pidE } | Select-Object -First 1
  if ($d) { Cap-Win $d (Join-Path $OutDir ("dlg_" + $cid + "_" + $Tag + "_1105.png")); [void][W]::PostMessage($d, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero); Start-Sleep 1 }
}
$hs = (Q "SELECT hs_track_no FROM job_mst WHERE lugg_no='$lugg'") | Where-Object { $_ -match '^\s*\d{3}\s*$' } | ForEach-Object { $_.Trim() } | Select-Object -First 1
if ($hs) { $DlgCids = ($DlgCids + ",1711" + $hs.PadLeft(4,'0')).Trim(',') ; Write-Output "hs=$hs" }
foreach ($cs in ($DlgCids -split ',' | Where-Object { $_ -ne "" })) {
  $cid2 = [int]$cs
  $b = Snap-Tops; [void][W]::PostMessage($views, 0x40A, [IntPtr]$cid2, [IntPtr]::Zero); Start-Sleep 4
  $d = New-Tops $b | Where-Object { [W]::Pid($_) -eq $pidE } | Select-Object -First 1
  if ($d) { Cap-Win $d (Join-Path $OutDir ("dlg_" + $cid2 + "_" + $Tag + "_1105.png")); [void][W]::PostMessage($d, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero); Start-Sleep 1 }
}
# 3. 작업 목록에서 행 클릭 → 선택 작업번호 확인
$lv = [W]::Children($m) | Where-Object { ([W]::Cls($_)) -eq 'SysListView32' } | ForEach-Object { $r = New-Object W+R; [void][W]::GetWindowRect($_, [ref]$r); [pscustomobject]@{H=$_; T=$r.T; W=($r.Rt-$r.L)} } | Where-Object { $_.W -gt 500 } | Sort-Object T
$jobLv = $lv[1].H; $seqLv = $lv[2].H
$panel = [W]::GetParent($jobLv)
$btn = [W]::Children($panel) | Where-Object { ([W]::Text($_)) -eq '완료처리' } | Select-Object -First 1
$sel = ""
for ($i=0; $i -lt 8; $i++) {
  $y = 34 + 17*$i; $lp = [IntPtr](($y -shl 16) -bor 50)
  [void][W]::PostMessage($jobLv, 0x201, [IntPtr]1, $lp); [void][W]::PostMessage($jobLv, 0x202, [IntPtr]0, $lp)
  Start-Sleep -Milliseconds 1200
  $stat = [W]::Children($panel) | Where-Object { ([W]::Cls($_)) -like '*Static*' } | ForEach-Object { [W]::Text($_) }
  $hit = $stat | Where-Object { $_ -like ("*" + $lugg + "*") }
  if ($hit) { $sel = $hit; break }
}
Write-Output "selected: $sel (row $i)"
if ($sel -eq "") { Write-Output "row not found"; exit }
Cap-Win $m (Join-Path $OutDir ("main_" + $Tag + "_sel_1105.png"))
# 4. [완료처리] → 뜨는 확인창을 차례로 캡처하고 '예'/'확인'
Click-Btn $btn
for ($k=0; $k -lt 5; $k++) {
  $cap = Join-Path $OutDir ("ask_complete_" + $Tag + "_" + $k + "_1105.png")
  $ok = Answer-Box $pidE "Ecs" 6 8 $cap
  if (-not $ok) { break }
}
Start-Sleep 30
Cap-Win $m (Join-Path $OutDir ("main_" + $Tag + "_done_1105.png"))
Q "SELECT lugg_no, job_status, start_pos, dest_pos FROM job_mst ORDER BY lugg_no" | Write-Output
