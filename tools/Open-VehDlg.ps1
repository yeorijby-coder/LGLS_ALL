# =====================================================================
# [LGLS 2026-11-04] 설비 상태창 열기 / 버튼 누르기 / 창 단위 캡처 (시험 자동화)
#   메인 창은 제목 "Ecs V" 접두로 찾고, 뷰(#32770, 폭>=1500)에 WM_USER+10(0x40A) 로 CID 를 보내 상태창을 연다.
#   포그라운드 전환 없이 PostMessage/SendMessage 만 쓴다(다른 창을 누르는 사고 방지).
#   사용:
#     .\Open-VehDlg.ps1 -Cid 17120901 -OutDir D:\shots            # SC#1 상태창 열고 캡처
#     .\Open-VehDlg.ps1 -Cid 17120901 -ClickId 8246 -Answer Yes   # [데이터 삭제](8246) 누르고 확인창 Yes, 결과창 캡처·닫기
#     .\Open-VehDlg.ps1 -Cid 17120901 -ClickId 8246 -Answer No
#     .\Open-VehDlg.ps1 -Ribbon 35071 -Answer Yes                  # 리본 명령(ID_MANUAL_CVCLEAR) 보내기
#     .\Open-VehDlg.ps1 -CloseAll                                   # 메인 외 열린 Ecs 창 전부 닫기
# =====================================================================
param(
    [int]$Cid = 0,
    [int]$ClickId = 0,
    [int]$Ribbon = 0,
    [ValidateSet('None','Yes','No','Ok')][string]$Answer = 'None',
    [string]$OutDir = "D:\project\LGLS\Renewal\적용_20261110\문서\shots_1104",
    [string]$Tag = "",
    [switch]$CloseAll,
    [switch]$NoCapture,
    [switch]$CaptureMain,
    [string]$TabClick = ""
)
Add-Type -AssemblyName System.Drawing
Add-Type @"
using System; using System.Text; using System.Runtime.InteropServices; using System.Collections.Generic;
public class U {
  public delegate bool EnumProc(IntPtr h, IntPtr l);
  [DllImport("user32.dll")] public static extern bool EnumWindows(EnumProc f, IntPtr l);
  [DllImport("user32.dll")] public static extern bool EnumChildWindows(IntPtr p, EnumProc f, IntPtr l);
  [DllImport("user32.dll")] public static extern int GetWindowText(IntPtr h, StringBuilder s, int n);
  [DllImport("user32.dll")] public static extern int GetClassName(IntPtr h, StringBuilder s, int n);
  [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr h);
  [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr h, out uint pid);
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out R r);
  [DllImport("user32.dll")] public static extern bool PostMessage(IntPtr h, uint m, IntPtr w, IntPtr l);
  [DllImport("user32.dll")] public static extern IntPtr SendMessage(IntPtr h, uint m, IntPtr w, IntPtr l);
  [DllImport("user32.dll")] public static extern IntPtr GetDlgItem(IntPtr h, int id);
  [DllImport("user32.dll")] public static extern IntPtr GetWindow(IntPtr h, uint cmd);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr d, uint f);
  [DllImport("user32.dll")] public static extern IntPtr WindowFromPoint(P p);
  [DllImport("user32.dll")] public static extern bool ScreenToClient(IntPtr h, ref P p);
  [StructLayout(LayoutKind.Sequential)] public struct P { public int X, Y; }
  [StructLayout(LayoutKind.Sequential)] public struct R { public int L,T,Rt,B; }
  public static List<IntPtr> Tops(uint pid) {
    var l = new List<IntPtr>();
    EnumWindows((h, p) => { uint q; GetWindowThreadProcessId(h, out q); if (q == pid && IsWindowVisible(h)) l.Add(h); return true; }, IntPtr.Zero);
    return l;
  }
  public static List<IntPtr> Children(IntPtr parent) {
    var l = new List<IntPtr>();
    EnumChildWindows(parent, (h, p) => { l.Add(h); return true; }, IntPtr.Zero);
    return l;
  }
  public static string Text(IntPtr h) { var sb = new StringBuilder(512); GetWindowText(h, sb, 512); return sb.ToString(); }
  public static string Cls(IntPtr h) { var sb = new StringBuilder(128); GetClassName(h, sb, 128); return sb.ToString(); }
}
"@
$WM_COMMAND = 0x111; $WM_CLOSE = 0x10; $BM_CLICK = 0xF5; $WM_USER10 = 0x40A

function Cap([IntPtr]$h, [string]$name) {
    if ($NoCapture) { return }
    New-Item -ItemType Directory -Force $OutDir | Out-Null
    $r = New-Object U+R; [void][U]::GetWindowRect($h, [ref]$r)
    $w = $r.Rt - $r.L; $ht = $r.B - $r.T
    if ($w -le 0 -or $ht -le 0) { Write-Output "cap skip $name ($w x $ht)"; return }
    $bmp = New-Object System.Drawing.Bitmap($w, $ht)
    $g = [System.Drawing.Graphics]::FromImage($bmp); $hdc = $g.GetHdc()
    $ok = [U]::PrintWindow($h, $hdc, 0); $g.ReleaseHdc($hdc); $g.Dispose()
    # PrintWindow 가 검게 나오면(렌더 안 된 창) 화면 복사로 대체 - 창이 가려지지 않은 경우에만 유효
    $px = $bmp.GetPixel([int]($w/2), [int]($ht/2)); $px2 = $bmp.GetPixel(5, 5)
    if ($px.ToArgb() -eq -16777216 -and $px2.ToArgb() -eq -16777216) {
        $g2 = [System.Drawing.Graphics]::FromImage($bmp); $g2.CopyFromScreen($r.L, $r.T, 0, 0, $bmp.Size); $g2.Dispose(); Write-Output "cap: screen copy"
    }
    $out = Join-Path $OutDir ($name + ".png"); $bmp.Save($out, [System.Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
    Write-Output "saved $out ($w x $ht)"
}

$proc = Get-Process -Name EcsMain -ErrorAction SilentlyContinue | Select-Object -First 1
if (-not $proc) { $proc = Get-Process -Name Ecs -ErrorAction SilentlyContinue | Where-Object { $_.MainWindowTitle -like "Ecs V*" } | Select-Object -First 1 }
if (-not $proc) { Write-Error "EcsMain 없음"; exit 1 }
$pid_ = [uint32]$proc.Id
$tops = [U]::Tops($pid_)
$hMain = $tops | Where-Object { ([U]::Text($_)).StartsWith("Ecs V") } | Select-Object -First 1
if (-not $hMain) { Write-Error "메인 창(Ecs V*) 없음"; exit 1 }
Write-Output ("main=0x{0:X} '{1}'" -f [int64]$hMain, [U]::Text($hMain))

if ($CloseAll) {
    foreach ($h in $tops) { if ($h -ne $hMain) { Write-Output ("close 0x{0:X} '{1}'" -f [int64]$h, [U]::Text($h)); [void][U]::PostMessage($h, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero) } }
    exit 0
}

if ($TabClick -ne "") {
    $xy = $TabClick.Split(","); $r = New-Object U+R; [void][U]::GetWindowRect($hMain, [ref]$r)
    $pt = New-Object U+P; $pt.X = $r.L + [int]$xy[0]; $pt.Y = $r.T + [int]$xy[1]
    $hc = [U]::WindowFromPoint($pt); $cp = $pt; [void][U]::ScreenToClient($hc, [ref]$cp)
    $lp = [IntPtr](($cp.Y -shl 16) -bor ($cp.X -band 0xFFFF))
    Write-Output ("tabclick child=0x{0:X} cls={1} client=({2},{3})" -f [int64]$hc, [U]::Cls($hc), $cp.X, $cp.Y)
    [void][U]::PostMessage($hc, 0x201, [IntPtr]1, $lp); [void][U]::PostMessage($hc, 0x202, [IntPtr]0, $lp); Start-Sleep -Seconds 2
}
if ($CaptureMain) { Cap $hMain ("main" + $Tag) }
$before = @{}; foreach ($h in $tops) { $before[[int64]$h] = 1 }

if ($Cid -ne 0) {
    $hView = [U]::Children($hMain) | Where-Object { [U]::Cls($_) -eq '#32770' -and ($r = New-Object U+R) -and [U]::GetWindowRect($_, [ref]$r) -and ($r.Rt - $r.L) -ge 1500 } | Select-Object -First 1
    if (-not $hView) { Write-Error "뷰 창(#32770 폭>=1500) 없음"; exit 1 }
    [void][U]::PostMessage($hView, $WM_USER10, [IntPtr]$Cid, [IntPtr]::Zero)
    Write-Output "posted WM_USER+10 cid=$Cid"
    Start-Sleep -Seconds 3
}
if ($Ribbon -ne 0) {
    [void][U]::PostMessage($hMain, $WM_COMMAND, [IntPtr]$Ribbon, [IntPtr]::Zero)
    Write-Output "posted WM_COMMAND id=$Ribbon"
    Start-Sleep -Seconds 2
}

function NewTops() { [U]::Tops($pid_) | Where-Object { -not $before.ContainsKey([int64]$_) } }
$new = @(NewTops)
foreach ($h in $new) { Write-Output ("new 0x{0:X} '{1}' {2}" -f [int64]$h, [U]::Text($h), [U]::Cls($h)); if (([U]::Text($h)) -like "*Runtime*") { Write-Output ("  assert: " + (([U]::Children($h) | ForEach-Object { [U]::Text($_) }) -join " / ")); Cap $h ("assert" + $Tag) } }
$hDlg = $new | Where-Object { [U]::Cls($_) -eq '#32770' -and -not (([U]::Text($_)) -like '*Runtime*') } | Select-Object -First 1
$hAs = $new | Where-Object { ([U]::Text($_)) -like '*Runtime*' } | Select-Object -First 1
if ($hAs) { $hI = [U]::GetDlgItem($hAs, 5); [void][U]::PostMessage($hI, $BM_CLICK, [IntPtr]::Zero, [IntPtr]::Zero); Write-Output 'assert: Ignore 누름'; Start-Sleep -Seconds 2 }
if ($hDlg -and $Cid -ne 0) { Cap $hDlg ("dlg_" + $Cid + $Tag) }

if ($ClickId -ne 0 -and $hDlg) {
    $hBtn = [U]::GetDlgItem($hDlg, $ClickId)
    Write-Output ("btn 0x{0:X} '{1}'" -f [int64]$hBtn, [U]::Text($hBtn))
    if ($hBtn -eq [IntPtr]::Zero) { Write-Error "버튼 $ClickId 없음"; exit 1 }
    $before2 = @{}; foreach ($h in [U]::Tops($pid_)) { $before2[[int64]$h] = 1 }
    [void][U]::PostMessage($hDlg, $WM_COMMAND, [IntPtr](($ClickId) -bor (0 -shl 16)), $hBtn)
    Start-Sleep -Seconds 2
    $ask = [U]::Tops($pid_) | Where-Object { -not $before2.ContainsKey([int64]$_) -and [U]::Cls($_) -eq '#32770' } | Select-Object -First 1
    if ($ask) {
        $txt = ([U]::Children($ask) | ForEach-Object { [U]::Text($_) }) -join ' / '
        Write-Output "ask: $txt"
        Cap $ask ("ask_" + $ClickId + $Tag)
        if ($Answer -ne 'None') {
            $id = @{ Yes = 6; No = 7; Ok = 1 }[$Answer]
            $hB = [U]::GetDlgItem($ask, $id)
            if ($hB -eq [IntPtr]::Zero) { $hB = [U]::GetDlgItem($ask, 2) }
            [void][U]::PostMessage($hB, $BM_CLICK, [IntPtr]::Zero, [IntPtr]::Zero)
            Start-Sleep -Seconds 2
            $res = [U]::Tops($pid_) | Where-Object { -not $before2.ContainsKey([int64]$_) -and $_ -ne $ask -and [U]::Cls($_) -eq '#32770' } | Select-Object -First 1
            if ($res) {
                $txt2 = ([U]::Children($res) | ForEach-Object { [U]::Text($_) }) -join ' / '
                Write-Output "result: $txt2"
                Cap $res ("result_" + $ClickId + $Tag)
                $hO = [U]::GetDlgItem($res, 1); if ($hO -eq [IntPtr]::Zero) { $hO = [U]::GetDlgItem($res, 2) }
                [void][U]::PostMessage($hO, $BM_CLICK, [IntPtr]::Zero, [IntPtr]::Zero)
            }
        }
    } else { Write-Output "ask: (확인창 없음)" }
}
if ($Ribbon -ne 0) {
    $ask = @(NewTops) | Where-Object { [U]::Cls($_) -eq '#32770' } | Select-Object -First 1
    if ($ask) {
        $txt = ([U]::Children($ask) | ForEach-Object { [U]::Text($_) }) -join ' / '
        Write-Output "ask: $txt"
        Cap $ask ("ask_ribbon_" + $Ribbon + $Tag)
        if ($Answer -ne 'None') {
            $id = @{ Yes = 6; No = 7; Ok = 1 }[$Answer]
            $hB = [U]::GetDlgItem($ask, $id); if ($hB -eq [IntPtr]::Zero) { $hB = [U]::GetDlgItem($ask, 2) }
            [void][U]::PostMessage($hB, $BM_CLICK, [IntPtr]::Zero, [IntPtr]::Zero)
            Start-Sleep -Seconds 2
            $res = @(NewTops) | Where-Object { $_ -ne $ask -and [U]::Cls($_) -eq '#32770' } | Select-Object -First 1
            if ($res) {
                $txt2 = ([U]::Children($res) | ForEach-Object { [U]::Text($_) }) -join ' / '
                Write-Output "result: $txt2"
                Cap $res ("result_ribbon_" + $Ribbon + $Tag)
                $hO = [U]::GetDlgItem($res, 1); if ($hO -eq [IntPtr]::Zero) { $hO = [U]::GetDlgItem($res, 2) }
                [void][U]::PostMessage($hO, $BM_CLICK, [IntPtr]::Zero, [IntPtr]::Zero)
            }
        }
    } else { Write-Output "ask: (확인창 없음)" }
}
