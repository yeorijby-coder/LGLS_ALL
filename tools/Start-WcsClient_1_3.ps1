# WCS Client (판넬 1/3 변형) 기동 - EXE_1_3/WCS_CLIENT
#   [LGLS 2026-09-16] 사용자 요청: 좌패널(통신/범례/지시정보/작업정보) = 화면 약 1/3,
#   메인 맵이 나머지(약 2/3)를 꽉 채우는 별도 레이아웃 Client.
#   기본 Client(EXE_NEWUI)와 별개 인스턴스로 뜬다. Ecs.ini : MAIN_UI=2(좌패널), MAIN_UI_LEFT_W=640(1/3).
#   사용법:  .\Start-WcsClient_1_3.ps1
[CmdletBinding()]
param([int]$WaitSec = 40)

$ExePath = "D:\project\LGLS\Renewal\EXE_1_3\WCS_CLIENT\Ecs.exe"
if (-not (Test-Path $ExePath)) { throw "실행파일 없음: $ExePath" }

Add-Type @'
using System;
using System.Runtime.InteropServices;
public class WinFull13 {
    [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int cmd);
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
    public const int SW_MAXIMIZE = 3;
}
'@

$before = @(Get-Process -Name 'Ecs' -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Id)
Start-Process -FilePath $ExePath -WorkingDirectory (Split-Path $ExePath)

$deadline = (Get-Date).AddSeconds($WaitSec)
$new = $null
while (-not $new -and (Get-Date) -lt $deadline) {
    Start-Sleep -Milliseconds 500
    $new = Get-Process -Name 'Ecs' -ErrorAction SilentlyContinue |
        Where-Object { ($before -notcontains $_.Id) -and ($_.MainWindowHandle -ne 0) } |
        Select-Object -First 1
}
if (-not $new) { throw "별도 Client 창을 찾지 못했습니다." }

[void][WinFull13]::ShowWindow($new.MainWindowHandle, [WinFull13]::SW_MAXIMIZE)
[void][WinFull13]::SetForegroundWindow($new.MainWindowHandle)
"별도 Client(판넬 1/3) 전체화면 기동 완료 (pid {0})" -f $new.Id
