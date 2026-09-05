# [LGLS 2026-09-05] 이중입고(54) / 공출고(58) 반복 시험
#   EQP_SIM 에서 에러를 주입하고, HOST_SIM 의 재지정 버튼을 눌러 작업이 정상 완료되는지 확인한다.
param([int]$Rounds = 12)

$ErrorActionPreference = 'Continue'
$sig = @"
[DllImport("user32.dll")] public static extern bool EnumChildWindows(IntPtr h, EnumProc cb, IntPtr l);
[DllImport("user32.dll")] public static extern int GetWindowText(IntPtr h, System.Text.StringBuilder s, int n);
[DllImport("user32.dll")] public static extern int GetClassName(IntPtr h, System.Text.StringBuilder s, int n);
[DllImport("user32.dll")] public static extern IntPtr SendMessage(IntPtr h, uint m, IntPtr w, IntPtr l);
[DllImport("user32.dll")] public static extern bool PostMessage(IntPtr h, uint m, IntPtr w, IntPtr l);
[DllImport("user32.dll")] public static extern bool IsWindowEnabled(IntPtr h);
public delegate bool EnumProc(IntPtr h, IntPtr l);
"@
Add-Type -MemberDefinition $sig -Name ET -Namespace ErrT -UsingNamespace System.Text

$LOG = "C:\Users\USER\AppData\Local\Temp\claude\D--project-LGLS-Renewal\4609e0ee-85bd-49e7-8c4e-b9231f6fb7f0\scratchpad\err_test_result.txt"
function W($m) { $line = "{0} {1}" -f (Get-Date -Format 'HH:mm:ss'), $m; Write-Host $line; Add-Content -Path $LOG -Value $line -Encoding UTF8 }

function FindCtl($procName, $match) {
  $p = Get-Process $procName -ErrorAction SilentlyContinue | Where-Object { $_.MainWindowHandle -ne 0 } | Select-Object -First 1
  if (-not $p) { return [IntPtr]::Zero }
  $script:hit = [IntPtr]::Zero
  $cb = [ErrT.ET+EnumProc]{ param($h, $l)
    $sb = New-Object System.Text.StringBuilder 256
    [ErrT.ET]::GetWindowText($h, $sb, 256) | Out-Null
    if ($sb.ToString() -like $match) { $script:hit = $h; return $false }
    return $true }
  [ErrT.ET]::EnumChildWindows($p.MainWindowHandle, $cb, [IntPtr]::Zero) | Out-Null
  return $script:hit
}

function Q($sql) {
  $r = sqlcmd -S 'localhost\SQLEXPRESS' -d LGLS_MCS_IO -E -h -1 -W -Q "SET NOCOUNT ON; $sql" 2>$null
  if ($null -eq $r) { return '' }
  return ($r -join '').Trim()
}

# 조건이 참이 될 때까지 대기. 참이면 $true
function WaitFor($sql, $timeoutSec, $desc) {
  $t0 = Get-Date
  while (((Get-Date) - $t0).TotalSeconds -lt $timeoutSec) {
    if ((Q $sql) -ne '0') { return $true }
    Start-Sleep -Milliseconds 1500
  }
  W ("    [시간초과] " + $desc + " (" + $timeoutSec + "s)")
  return $false
}

W "════════ 이중입고(54) / 공출고(58) 반복 시험 시작 : 각 $Rounds 회 ════════"
$okD = 0; $ngD = 0; $okE = 0; $ngE = 0
$detail = @()

foreach ($kind in @('DOUBLE', 'EMPTY')) {
  $errCode = if ($kind -eq 'DOUBLE') { '0054' } else { '0058' }
  $chkMatch = if ($kind -eq 'DOUBLE') { '*이중입고 에러*' } else { '*공출고 에러*' }
  $btnMatch = if ($kind -eq 'DOUBLE') { '*이중입고 재지정*' } else { '*공출고 작업 삭제*' }
  $name = if ($kind -eq 'DOUBLE') { '이중입고(54)' } else { '공출고(58)' }
  W ""
  W "──────── $name 시험 ────────"

  for ($i = 1; $i -le $Rounds; $i++) {
    $t0 = Get-Date
    $chk = FindCtl 'EQP_SIM' $chkMatch
    if ($chk -eq [IntPtr]::Zero) { W "  #$i 실패 : EQP_SIM 체크박스를 찾지 못함"; $detail += "$kind #$i NG(체크박스없음)"; continue }

    # 에러 주입 : ★항상 껐다 켠다★
    #   EQP_SIM 은 주입 플래그(InjectDoubleStorage)를 내부적으로만 소비하고 체크박스는 켠 채로 둔다.
    #   따라서 "이미 켜져 있으면 그대로" 두면 다음 회차에 주입이 일어나지 않는다.
    if ([int][ErrT.ET]::SendMessage($chk, 0x00F0, [IntPtr]::Zero, [IntPtr]::Zero) -ne 0) {
      [ErrT.ET]::PostMessage($chk, 0x00F5, [IntPtr]::Zero, [IntPtr]::Zero) | Out-Null
      Start-Sleep -Milliseconds 600
    }
    [ErrT.ET]::PostMessage($chk, 0x00F5, [IntPtr]::Zero, [IntPtr]::Zero) | Out-Null
    Start-Sleep -Milliseconds 800
    if ([int][ErrT.ET]::SendMessage($chk, 0x00F0, [IntPtr]::Zero, [IntPtr]::Zero) -eq 0) {
      W "    (주의) 체크박스가 켜지지 않았다"
    }

    # 에러 발생 대기 (안 오면 한 번 더 주입 시도 - 시뮬이 플래그를 먼저 소비했을 수 있다)
    $sqlErr = "SELECT COUNT(*) FROM SC_DATA_LGLS WHERE ERR_CODE_RD = '$errCode'"
    $got = WaitFor $sqlErr 150 "$name 에러 발생 대기(1차)"
    if (-not $got) {
      W "    재주입 시도"
      if ([int][ErrT.ET]::SendMessage($chk, 0x00F0, [IntPtr]::Zero, [IntPtr]::Zero) -ne 0) {
        [ErrT.ET]::PostMessage($chk, 0x00F5, [IntPtr]::Zero, [IntPtr]::Zero) | Out-Null
        Start-Sleep -Milliseconds 600
      }
      [ErrT.ET]::PostMessage($chk, 0x00F5, [IntPtr]::Zero, [IntPtr]::Zero) | Out-Null
      $got = WaitFor $sqlErr 180 "$name 에러 발생 대기(2차)"
    }
    if (-not $got) {
      W "  #$i NG : 에러가 발생하지 않음"; $detail += "$kind #$i NG(에러미발생)"
      if ($kind -eq 'DOUBLE') { $ngD++ } else { $ngE++ }; continue
    }
    $sc = Q "SELECT TOP 1 SC_NO FROM SC_DATA_LGLS WHERE ERR_CODE_RD='$errCode'"
    $lugg = Q "SELECT TOP 1 LTRIM(RTRIM(COALESCE(NULLIF(PALLET_ON_VEHICLE_RD,''), NULLIF(ITN_LUGG_FK1,''), NULLIF(LUGG_NO_FK1_OD,''), ''))) FROM SC_DATA_LGLS WHERE ERR_CODE_RD='$errCode'"
    W "  #$i 에러 발생 : S/C$sc  작업 $lugg"

    # HOST_SIM 재지정 버튼 활성화 대기 후 클릭
    $clicked = $false
    $t1 = Get-Date
    while (((Get-Date) - $t1).TotalSeconds -lt 60) {
      $btn = FindCtl 'HOST_SIM' $btnMatch
      if ($btn -ne [IntPtr]::Zero -and [ErrT.ET]::IsWindowEnabled($btn)) {
        [ErrT.ET]::PostMessage($btn, 0x00F5, [IntPtr]::Zero, [IntPtr]::Zero) | Out-Null
        $clicked = $true; break
      }
      Start-Sleep -Milliseconds 1500
    }
    if (-not $clicked) { W "  #$i NG : 상위 처리 버튼이 활성화되지 않음"; $detail += "$kind #$i NG(상위버튼)"
      if ($kind -eq 'DOUBLE') { $ngD++ } else { $ngE++ }; continue }

    # 공출고는 재지정이 없다. 상위(IMS)가 삭제하고, WCS 쪽은 운전자가 [작업정보] 에서 삭제한다.
    #   운전 화면 조작을 자동화할 수 없으므로, Client 가 실행하는 것과 같은 삭제를 대행한다.
    if ($kind -eq 'EMPTY' -and $lugg -ne '') {
      Start-Sleep -Seconds 3
      Q "DELETE FROM JOB_MST WHERE WH_TYP='10' AND LUGG_NO='$lugg'" | Out-Null
      $left = Q "SELECT COUNT(*) FROM JOB_MST WHERE WH_TYP='10' AND LUGG_NO='$lugg'"
      if ($left -eq '0') { W "    WCS 작업 $lugg 삭제 (운전자 조작 대행)" }
      else { W "    (주의) WCS 작업 $lugg 이 삭제되지 않았다" }

      # 크레인 에러는 설비에서 사람이 푼다. 스케줄러는 에러난 크레인에 새 지시를 주지 않으므로
      #   재지정이 없는 공출고에서는 이 조작이 없으면 에러가 영영 남는다.
      Start-Sleep -Seconds 2
      $btnClr = FindCtl 'EQP_SIM' '*설비 에러 해제*'
      if ($btnClr -ne [IntPtr]::Zero) {
        [ErrT.ET]::PostMessage($btnClr, 0x00F5, [IntPtr]::Zero, [IntPtr]::Zero) | Out-Null
        W "    EQP_SIM [설비 에러 해제] (현장 조작반 대행)"
      } else { W "    (주의) EQP_SIM [설비 에러 해제] 버튼을 찾지 못함" }
    }

    # 에러 해제 대기 (재지시로 크레인이 다시 움직이면 해제된다)
    $sqlClear = "SELECT CASE WHEN EXISTS(SELECT 1 FROM SC_DATA_LGLS WHERE ERR_CODE_RD='$errCode') THEN 0 ELSE 1 END"
    if (-not (WaitFor $sqlClear 240 "$name 에러 해제 대기")) {
      W "  #$i NG : 에러가 해제되지 않음"; $detail += "$kind #$i NG(에러미해제)"
      if ($kind -eq 'DOUBLE') { $ngD++ } else { $ngE++ }; continue
    }

    $sec = [int]((Get-Date) - $t0).TotalSeconds
    W ("  #$i OK : " + $(if($kind -eq 'DOUBLE'){'재지정 후 정상 복귀'}else{'삭제 후 정상 복귀'}) + " ($sec 초)")
    $detail += "$kind #$i OK ${sec}s"
    if ($kind -eq 'DOUBLE') { $okD++ } else { $okE++ }
    Start-Sleep -Seconds 12
  }
}

W ""
W "════════ 결과 ════════"
W ("이중입고(54) : 성공 {0} / 실패 {1}" -f $okD, $ngD)
W ("공출고(58)   : 성공 {0} / 실패 {1}" -f $okE, $ngE)
W ("상세 : " + ($detail -join '  |  '))
