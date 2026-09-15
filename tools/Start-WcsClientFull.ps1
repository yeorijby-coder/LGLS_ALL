# =====================================================================
# WCS Client(Ecs) 전체 화면 기동 + Claude 창 topmost 해제
#   [LGLS 2026-09-15] 사용자 요청: Client 를 켤 때 반 화면이 아니라 전체 화면(최대화)으로.
#   또, 이 Claude Code 창이 항상 위(topmost)로 떠서 다른 창을 가리지 않도록 topmost 해제.
#
#   사용법:
#     .\Start-WcsClientFull.ps1                 # Ecs 기동(이미 떠 있으면 재사용) 후 전체화면
#     .\Start-WcsClientFull.ps1 -ExePath <경로>  # 다른 exe 지정
# =====================================================================
[CmdletBinding()]
param(
    [string]$ExePath = "D:\project\LGLS\Renewal\EXE_NEWUI\WCS_CLIENT\Ecs.exe",
    [int]$WaitSec    = 40
)

Add-Type @'
using System;
using System.Runtime.InteropServices;
public class WinFull
{
    [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int cmd);
    [DllImport("user32.dll")] public static extern bool SetWindowPos(IntPtr h, IntPtr after, int x, int y, int cx, int cy, uint flags);
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
    public const int SW_MAXIMIZE = 3;
    public static IntPtr HWND_NOTOPMOST = new IntPtr(-2);
    public const uint SWP_NOMOVE = 0x0002, SWP_NOSIZE = 0x0001, SWP_NOACTIVATE = 0x0010;
}
'@

function Get-MainWindow([string]$procName) {
    Get-Process -Name $procName -ErrorAction SilentlyContinue |
        Where-Object { $_.MainWindowHandle -ne 0 } |
        Select-Object -First 1
}

# ── 1) Client 기동(이미 떠 있으면 재사용) ────────────────────────────
$client = Get-MainWindow 'Ecs'
if (-not $client) {
    if (-not (Test-Path $ExePath)) { throw "Client 실행파일을 찾을 수 없습니다: $ExePath" }
    Start-Process -FilePath $ExePath -WorkingDirectory (Split-Path $ExePath)
    $deadline = (Get-Date).AddSeconds($WaitSec)
    while (-not $client -and (Get-Date) -lt $deadline) {
        Start-Sleep -Milliseconds 500
        $client = Get-MainWindow 'Ecs'
    }
}
if (-not $client) { throw "WCS Client 창을 찾지 못했습니다." }

# ── 2) 전체 화면(최대화) ─────────────────────────────────────────────
[void][WinFull]::ShowWindow($client.MainWindowHandle, [WinFull]::SW_MAXIMIZE)
[void][WinFull]::SetForegroundWindow($client.MainWindowHandle)
"WCS Client 전체 화면(최대화) 완료 (pid {0})" -f $client.Id

# ── 3) Claude 창 topmost 해제 (다른 창이 앞으로 올 수 있게) ───────────
$claude = Get-MainWindow 'claude'
if ($claude) {
    [void][WinFull]::SetWindowPos($claude.MainWindowHandle, [WinFull]::HWND_NOTOPMOST,
        0, 0, 0, 0,
        ([WinFull]::SWP_NOMOVE -bor [WinFull]::SWP_NOSIZE -bor [WinFull]::SWP_NOACTIVATE))
    "Claude 창 topmost 해제 완료 (pid {0})" -f $claude.Id
} else {
    "Claude 창을 찾지 못해 topmost 해제 생략"
}
