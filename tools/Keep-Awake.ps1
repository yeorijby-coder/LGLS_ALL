# [LGLS 2026-09-11] 야간 구동 중 PC 가 절전(Modern Standby)에 들어가지 않게 잡아 둔다.
#
#   배경 : 2026-09-11 09:41~09:50 에 노트북이 "Adaptive Connected Standby" 로 들어가면서
#          EQP_SIM / WCS_TASK_CV / IO_TASK 가 10분간 통째로 멈췄다. 통신 끊김 메시지 없이
#          전 로그가 끊겼다가 한꺼번에 재개됐고, 그 사이 작업 5204(35) / 5205(25)가
#          그 상태에 머물러 SC·RTV 색이 남아 보였다.
#
#   powercfg 의 절전 시간 제한(standby-timeout)은 이미 AC 0 이었는데도 들어갔다.
#   Adaptive Connected Standby 는 사용자 입력이 없으면 들어가므로 시간 제한으로는 못 막는다.
#   powercfg /requestsoverride 는 관리자 권한이 필요해서, 권한 없이 되는
#   SetThreadExecutionState 로 전원 요청을 건다.
#
#   쓰는 법 : 구동 시작 전에 한 번 띄워 두면 된다(창 없음).
#       powershell -NoProfile -ExecutionPolicy Bypass -File tools\Keep-Awake.ps1
#   끄는 법 : 이 프로세스를 종료한다. 종료하면 요청이 풀려 원래 전원 설정으로 돌아간다.
#       Get-Process powershell | Where-Object { $_.MainWindowTitle -eq '' } ... 또는 작업 관리자
#
#   확인 : powercfg /requests  ->  [SYSTEM] 에 이 프로세스가 보이면 걸린 것이다.

Add-Type @"
using System;
using System.Runtime.InteropServices;
public static class PW {
  [DllImport("kernel32.dll", SetLastError = true)]
  public static extern uint SetThreadExecutionState(uint esFlags);
}
"@

$ES_CONTINUOUS        = [uint32]0x80000000
$ES_SYSTEM_REQUIRED   = [uint32]0x00000001
$ES_AWAYMODE_REQUIRED = [uint32]0x00000040

# 화면은 끄게 두고(ES_DISPLAY_REQUIRED 는 안 건다) 시스템만 깨어 있게 한다.
$flags = $ES_CONTINUOUS -bor $ES_SYSTEM_REQUIRED -bor $ES_AWAYMODE_REQUIRED
$r = [PW]::SetThreadExecutionState($flags)
if ($r -eq 0) {
    Write-Output '전원 요청 실패'
    exit 1
}
Write-Output ("절전 억제 시작 (PID {0}). 이 프로세스를 끄면 해제된다." -f $PID)

# 요청은 이 스레드가 살아 있는 동안만 유지된다. 그냥 잠들어 있는다.
while ($true) { Start-Sleep -Seconds 60 }
