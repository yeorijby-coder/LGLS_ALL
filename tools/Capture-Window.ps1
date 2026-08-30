# 창 단위 PrintWindow 캡처 (전체화면 캡처 금지 규약)
param([Parameter(Mandatory=$true)][int]$ProcId, [Parameter(Mandatory=$true)][string]$Out)
Add-Type -AssemblyName System.Drawing
Add-Type @"
using System;using System.Runtime.InteropServices;
public class W {
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h,IntPtr d,uint f);
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h,out R r);
  [StructLayout(LayoutKind.Sequential)] public struct R { public int L,T,Rt,B; }
}
"@
$p = Get-Process -Id $ProcId
$h = $p.MainWindowHandle
if ($h -eq [IntPtr]::Zero) { Write-Error "no main window"; exit 1 }
$r = New-Object W+R
[void][W]::GetWindowRect($h,[ref]$r)
$w = $r.Rt - $r.L; $ht = $r.B - $r.T
$bmp = New-Object System.Drawing.Bitmap($w,$ht)
$g = [System.Drawing.Graphics]::FromImage($bmp)
$hdc = $g.GetHdc()
[void][W]::PrintWindow($h,$hdc,2)
$g.ReleaseHdc($hdc); $g.Dispose()
$bmp.Save($Out,[System.Drawing.Imaging.ImageFormat]::Png)
$bmp.Dispose()
Write-Output "saved $Out ($w x $ht)"
