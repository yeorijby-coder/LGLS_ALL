# 자식 컨트롤 열거 (핸들/텍스트/클래스/활성)
param([Parameter(Mandatory=$true)][int]$ProcId, [string]$Filter='')
Add-Type @"
using System;using System.Text;using System.Runtime.InteropServices;
public class EC {
 public delegate bool EnumProc(IntPtr h, IntPtr l);
 [DllImport("user32.dll")] public static extern bool EnumChildWindows(IntPtr h, EnumProc cb, IntPtr l);
 [DllImport("user32.dll",CharSet=CharSet.Unicode)] public static extern int GetWindowTextW(IntPtr h, StringBuilder s, int m);
 [DllImport("user32.dll",CharSet=CharSet.Unicode)] public static extern int GetClassNameW(IntPtr h, StringBuilder s, int m);
 [DllImport("user32.dll")] public static extern bool IsWindowEnabled(IntPtr h);
 [DllImport("user32.dll")] public static extern IntPtr SendMessage(IntPtr h, uint m, IntPtr w, IntPtr l);
}
"@
$p = Get-Process -Id $ProcId
$script:acc = @()
$cb = [EC+EnumProc]{ param($h,$l)
  $sb = New-Object System.Text.StringBuilder 512
  [void][EC]::GetWindowTextW($h,$sb,512); $txt=$sb.ToString()
  $c2 = New-Object System.Text.StringBuilder 256
  [void][EC]::GetClassNameW($h,$c2,256); $cls=$c2.ToString()
  # BM_GETCHECK = 0x00F0 (체크박스 상태)
  $chk = [EC]::SendMessage($h, 0x00F0, [IntPtr]::Zero, [IntPtr]::Zero)
  if ($txt -ne '') { $script:acc += [pscustomobject]@{H=[int64]$h;Text=$txt;Class=($cls -replace 'WindowsForms10\.','');En=[EC]::IsWindowEnabled($h);Chk=[int]$chk} }
  return $true }
[void][EC]::EnumChildWindows($p.MainWindowHandle,$cb,[IntPtr]::Zero)
$r = $script:acc
if ($Filter -ne '') { $r = $r | Where-Object { $_.Text -match $Filter } }
$r | Format-Table -AutoSize
