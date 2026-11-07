# 공용 Win32 도우미 : 창/컨트롤 찾기, 버튼 누르기, 텍스트 넣기, 콤보 고르기, 메시지박스 응답, 창 캡처
Add-Type -AssemblyName System.Drawing
if (-not ([System.Management.Automation.PSTypeName]'W').Type) {
Add-Type @"
using System; using System.Text; using System.Runtime.InteropServices; using System.Collections.Generic;
public class W {
  public delegate bool EnumProc(IntPtr h, IntPtr l);
  [DllImport("user32.dll")] public static extern bool EnumWindows(EnumProc f, IntPtr l);
  [DllImport("user32.dll")] public static extern bool EnumChildWindows(IntPtr p, EnumProc f, IntPtr l);
  [DllImport("user32.dll", CharSet=CharSet.Unicode)] public static extern int GetWindowTextW(IntPtr h, StringBuilder s, int n);
  [DllImport("user32.dll", CharSet=CharSet.Unicode)] public static extern int GetClassNameW(IntPtr h, StringBuilder s, int n);
  [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr h);
  [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr h, out uint pid);
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out R r);
  [DllImport("user32.dll")] public static extern bool PostMessage(IntPtr h, uint m, IntPtr w, IntPtr l);
  [DllImport("user32.dll", CharSet=CharSet.Unicode)] public static extern IntPtr SendMessageW(IntPtr h, uint m, IntPtr w, string l);
  [DllImport("user32.dll")] public static extern IntPtr SendMessage(IntPtr h, uint m, IntPtr w, IntPtr l);
  [DllImport("user32.dll")] public static extern IntPtr GetDlgItem(IntPtr h, int id);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr d, uint f);
  [DllImport("user32.dll")] public static extern bool SetWindowPos(IntPtr h, IntPtr a, int x, int y, int cx, int cy, uint f);
  [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int cmd);
  [DllImport("user32.dll")] public static extern IntPtr GetParent(IntPtr h);
  [StructLayout(LayoutKind.Sequential)] public struct R { public int L,T,Rt,B; }
  public static List<IntPtr> Tops(uint pid) { var l = new List<IntPtr>(); EnumWindows((h, p) => { uint q; GetWindowThreadProcessId(h, out q); if (q == pid && IsWindowVisible(h)) l.Add(h); return true; }, IntPtr.Zero); return l; }
  public static List<IntPtr> AllTops() { var l = new List<IntPtr>(); EnumWindows((h, p) => { if (IsWindowVisible(h)) l.Add(h); return true; }, IntPtr.Zero); return l; }
  public static List<IntPtr> Children(IntPtr parent) { var l = new List<IntPtr>(); EnumChildWindows(parent, (h, p) => { l.Add(h); return true; }, IntPtr.Zero); return l; }
  public static string Text(IntPtr h) { var sb = new StringBuilder(1024); GetWindowTextW(h, sb, 1024); return sb.ToString(); }
  public static string Cls(IntPtr h) { var sb = new StringBuilder(256); GetClassNameW(h, sb, 256); return sb.ToString(); }
  public static uint Pid(IntPtr h) { uint q; GetWindowThreadProcessId(h, out q); return q; }
}
"@
}
$script:BM_CLICK=0xF5; $script:WM_SETTEXT=0xC; $script:CB_SETCURSEL=0x14E; $script:CB_SELECTSTRING=0x14D; $script:WM_COMMAND=0x111; $script:WM_CLOSE=0x10; $script:BM_GETCHECK=0xF0; $script:CB_GETCURSEL=0x147
function Get-ProcMain([string]$name, [string]$titleLike="*") {
  $p = Get-Process -Name $name -ErrorAction SilentlyContinue | Select-Object -First 1
  if (-not $p) { return $null }
  $t = [W]::Tops([uint32]$p.Id) | Where-Object { ([W]::Text($_)) -like $titleLike } | Select-Object -First 1
  return $t
}
function Find-Child([IntPtr]$parent, [string]$textLike, [string]$cls="") {
  [W]::Children($parent) | Where-Object { ([W]::Text($_)) -like $textLike -and ($cls -eq "" -or ([W]::Cls($_)) -like ("*"+$cls+"*")) } | Select-Object -First 1
}
# 라벨 오른쪽/같은 줄의 컨트롤 : 부모의 자식 중 클래스로 골라 Y 가 가장 가깝고 X 가 라벨보다 큰 것
function Find-Near([IntPtr]$parent, [IntPtr]$label, [string]$cls, [int]$nth=0) {
  $lr = New-Object W+R; [void][W]::GetWindowRect($label, [ref]$lr)
  $c = [W]::Children($parent) | Where-Object { ([W]::Cls($_)) -like ("*"+$cls+"*") } | ForEach-Object {
    $r = New-Object W+R; [void][W]::GetWindowRect($_, [ref]$r)
    if ($r.L -ge $lr.L -and [Math]::Abs($r.T - $lr.T) -le 8) { [pscustomobject]@{H=$_; X=$r.L} } } | Sort-Object X
  if ($c) { return ($c | Select-Object -Index $nth).H }
  return [IntPtr]::Zero
}
function Click-Btn([IntPtr]$h) { [void][W]::PostMessage($h, $BM_CLICK, [IntPtr]::Zero, [IntPtr]::Zero) }
function Set-Text([IntPtr]$h, [string]$s) { [void][W]::SendMessageW($h, $WM_SETTEXT, [IntPtr]::Zero, $s) }
function Set-Combo([IntPtr]$h, [int]$i) { [void][W]::SendMessage($h, $CB_SETCURSEL, [IntPtr]$i, [IntPtr]::Zero); [void][W]::SendMessage([W]::GetParent($h), $WM_COMMAND, [IntPtr]((1 -shl 16)), $h) }
# 어떤 프로세스의 #32770 메시지박스(제목 like)를 기다려 버튼(기본 '확인'/IDOK=1, Yes=6, No=7) 누르기
function Answer-Box([uint32]$procId, [string]$titleLike, [int]$btnId=1, [int]$waitSec=5, [string]$capPath="") {
  for ($i=0; $i -lt ($waitSec*4); $i++) {
    $b = [W]::Tops($procId) | Where-Object { ([W]::Cls($_)) -eq '#32770' -and ([W]::Text($_)) -like $titleLike } | Select-Object -First 1
    if ($b) {
      $txt = ([W]::Children($b) | ForEach-Object { [W]::Text($_) }) -join ' / '
      Write-Output "box: $txt"
      if ($capPath -ne "") { Cap-Win $b $capPath }
      $hb = [W]::GetDlgItem($b, $btnId); if ($hb -eq [IntPtr]::Zero) { $hb = [W]::GetDlgItem($b, 2) }
      Click-Btn $hb; Start-Sleep -Milliseconds 500; return $true }
    Start-Sleep -Milliseconds 250 }
  return $false
}
function Cap-Win([IntPtr]$h, [string]$path) {
  $r = New-Object W+R; [void][W]::GetWindowRect($h, [ref]$r)
  $w = $r.Rt - $r.L; $ht = $r.B - $r.T
  if ($w -le 0 -or $ht -le 0) { Write-Output "cap skip ($w x $ht)"; return }
  $bmp = New-Object System.Drawing.Bitmap($w, $ht)
  $g = [System.Drawing.Graphics]::FromImage($bmp); $hdc = $g.GetHdc(); [void][W]::PrintWindow($h, $hdc, 0); $g.ReleaseHdc($hdc); $g.Dispose()
  $px = $bmp.GetPixel([int]($w/2), [int]($ht/2)); $px2 = $bmp.GetPixel(5, 5)
  if ($px.ToArgb() -eq -16777216 -and $px2.ToArgb() -eq -16777216) { $g2 = [System.Drawing.Graphics]::FromImage($bmp); $g2.CopyFromScreen($r.L, $r.T, 0, 0, $bmp.Size); $g2.Dispose(); Write-Output "cap: screen copy" }
  New-Item -ItemType Directory -Force (Split-Path $path) | Out-Null
  $bmp.Save($path, [System.Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
  Write-Output "saved $path ($w x $ht)"
}

# WinForms CheckBox 는 BM_CLICK 이 안 먹는다 - 클라이언트 좌표(8,8)에 WM_LBUTTONDOWN/UP
function Click-Chk([IntPtr]$h) { $lp=[IntPtr]((8 -shl 16) -bor 8); [void][W]::PostMessage($h, 0x201, [IntPtr]1, $lp); [void][W]::PostMessage($h, 0x202, [IntPtr]0, $lp) }
function Get-Chk([IntPtr]$h) { return [int][W]::SendMessage($h, 0xF0, [IntPtr]::Zero, [IntPtr]::Zero) }
# 새로 생긴 최상위 창(전체) 찾기
function New-Tops($before) { [W]::AllTops() | Where-Object { -not $before.ContainsKey([int64]$_) } }
function Snap-Tops() { $d=@{}; foreach ($h in [W]::AllTops()) { $d[[int64]$h]=1 }; return $d }
