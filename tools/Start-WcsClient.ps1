# =====================================================================
# WCS Client(Ecs) 기동 + 화면 반반 배치
#   [LGLS 2026-07-31] 사용자 요청: Client 를 켤 때 화면의 절반만 차지하게 하고,
#   나머지 절반은 Claude 창이 쓰도록 좌우로 나눈다.
#
#   기본 배치 : Claude 가 있는 모니터의  왼쪽 절반 = WCS Client(Ecs)
#                                        오른쪽 절반 = Claude
#   (Claude 창을 못 찾으면 주 모니터에 Client 만 왼쪽 절반으로 배치)
#
#   사용법:
#     .\Start-WcsClient.ps1                 # Ecs 기동(이미 떠 있으면 재사용) 후 반반 배치
#     .\Start-WcsClient.ps1 -ClientRight     # 좌우 반대로 (Client 오른쪽 / Claude 왼쪽)
#     .\Start-WcsClient.ps1 -ArrangeOnly     # 기동하지 않고 현재 창 배치만 다시 맞춤
# =====================================================================
[CmdletBinding()]
param(
    [string]$ExePath      = "D:\project\LGLS\Renewal\WCS\CPlusPlus\LGLS_CLIENT\Bin\Debug\Ecs.exe",
    [switch]$ClientRight,
    [switch]$ArrangeOnly,
    [int]$WaitSec         = 40
)

Add-Type -AssemblyName System.Windows.Forms

Add-Type @'
using System;
using System.Runtime.InteropServices;
public class WinPos
{
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
    [DllImport("user32.dll")] public static extern bool SetWindowPos(IntPtr h, IntPtr after, int x, int y, int cx, int cy, uint flags);
    [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int cmd);
    [DllImport("user32.dll")] public static extern bool IsZoomed(IntPtr h);
    [DllImport("dwmapi.dll")]  public static extern int DwmGetWindowAttribute(IntPtr h, int attr, out RECT val, int size);
    public struct RECT { public int Left, Top, Right, Bottom; }
    public const int SW_RESTORE = 9;
    public const uint SWP_NOZORDER = 0x0004, SWP_NOACTIVATE = 0x0010, SWP_SHOWWINDOW = 0x0040;
    public const int DWMWA_EXTENDED_FRAME_BOUNDS = 9;
}
'@

# 창의 '보이지 않는 그림자 테두리' 두께 (Windows 스냅과 동일한 여백을 재현하기 위함)
function Get-FrameSlack([IntPtr]$h) {
    $win = New-Object WinPos+RECT
    $ext = New-Object WinPos+RECT
    [void][WinPos]::GetWindowRect($h, [ref]$win)
    $size = [System.Runtime.InteropServices.Marshal]::SizeOf([type]([WinPos+RECT]))
    if ([WinPos]::DwmGetWindowAttribute($h, [WinPos]::DWMWA_EXTENDED_FRAME_BOUNDS, [ref]$ext, $size) -ne 0) {
        return @{ L = 0; T = 0; R = 0; B = 0 }
    }
    return @{ L = ($ext.Left - $win.Left); T = ($ext.Top - $win.Top);
              R = ($win.Right - $ext.Right); B = ($win.Bottom - $ext.Bottom) }
}

function Set-WindowHalf([IntPtr]$h, $area, [string]$side) {
    if (-not $h -or $h -eq [IntPtr]::Zero) { return $false }
    if ([WinPos]::IsZoomed($h)) { [void][WinPos]::ShowWindow($h, [WinPos]::SW_RESTORE); Start-Sleep -Milliseconds 250 }

    $halfW = [int]($area.Width / 2)
    $x = if ($side -eq 'left') { $area.X } else { $area.X + $halfW }
    $slack = Get-FrameSlack $h        # 스냅과 같은 시각적 크기가 되도록 테두리만큼 바깥으로 확장
    [void][WinPos]::SetWindowPos($h, [IntPtr]::Zero,
        ($x - $slack.L), ($area.Y - $slack.T),
        ($halfW + $slack.L + $slack.R), ($area.Height + $slack.T + $slack.B),
        ([WinPos]::SWP_NOZORDER -bor [WinPos]::SWP_NOACTIVATE -bor [WinPos]::SWP_SHOWWINDOW))
    return $true
}

function Get-MainWindow([string]$procName) {
    Get-Process -Name $procName -ErrorAction SilentlyContinue |
        Where-Object { $_.MainWindowHandle -ne 0 } |
        Select-Object -First 1
}

# ── 1) Client 기동 (이미 떠 있으면 재사용) ────────────────────────────
$client = Get-MainWindow 'Ecs'
if (-not $client -and -not $ArrangeOnly) {
    if (-not (Test-Path $ExePath)) { throw "Client 실행파일을 찾을 수 없습니다: $ExePath" }
    Start-Process -FilePath $ExePath -WorkingDirectory (Split-Path $ExePath)
    $deadline = (Get-Date).AddSeconds($WaitSec)
    while (-not $client -and (Get-Date) -lt $deadline) {
        Start-Sleep -Milliseconds 500
        $client = Get-MainWindow 'Ecs'
    }
}
if (-not $client) { throw "WCS Client 창을 찾지 못했습니다." }

# ── 2) 기준 모니터 결정: Claude 창이 있는 모니터 (없으면 주 모니터) ──
$claude = Get-MainWindow 'claude'
$screen = if ($claude) {
    [System.Windows.Forms.Screen]::FromHandle($claude.MainWindowHandle)
} else {
    [System.Windows.Forms.Screen]::PrimaryScreen
}
$area = $screen.WorkingArea

# ── 3) 좌우 반반 배치 ────────────────────────────────────────────────
$clientSide = if ($ClientRight) { 'right' } else { 'left' }
$claudeSide = if ($ClientRight) { 'left' }  else { 'right' }

[void](Set-WindowHalf $client.MainWindowHandle $area $clientSide)
if ($claude) { [void](Set-WindowHalf $claude.MainWindowHandle $area $claudeSide) }

"모니터 {0} 작업영역 {1}" -f $screen.DeviceName, $area
"  WCS Client : {0} 절반" -f $clientSide
if ($claude) { "  Claude     : {0} 절반" -f $claudeSide } else { "  Claude     : 창을 찾지 못해 배치 생략" }
