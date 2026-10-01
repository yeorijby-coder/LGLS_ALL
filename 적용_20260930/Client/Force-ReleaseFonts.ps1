# Forcibly release font files under <Dir>\rc_resource\font that an old Client left locked.
#   1) RemoveFontResource until the GDI registration count drops to zero
#   2) Restart Manager: find processes holding the files; close them (explorer is restarted)
#   3) restart the Windows font cache services
#   4) verify each file can be opened for write (= replaceable / deletable)
# Output is ASCII-only on purpose (PowerShell 5.1 reads a BOM-less script as ANSI).
param([Parameter(Mandatory=$true)][string]$Dir, [switch]$Yes)

Add-Type @"
using System; using System.Text; using System.Runtime.InteropServices;
public class FontForce {
  [DllImport("gdi32.dll", CharSet=CharSet.Unicode)] public static extern bool RemoveFontResourceW(string f);
  [DllImport("user32.dll")] public static extern IntPtr SendMessageTimeout(IntPtr h, uint m, IntPtr w, IntPtr l, uint f, uint t, out IntPtr r);

  [StructLayout(LayoutKind.Sequential)] public struct RM_UNIQUE_PROCESS { public int dwProcessId; public System.Runtime.InteropServices.ComTypes.FILETIME ProcessStartTime; }
  [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Unicode)]
  public struct RM_PROCESS_INFO {
    public RM_UNIQUE_PROCESS Process;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst=256)] public string strAppName;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst=64)]  public string strServiceShortName;
    public int ApplicationType; public uint AppStatus; public uint TSSessionId; [MarshalAs(UnmanagedType.Bool)] public bool bRestartable;
  }
  [DllImport("rstrtmgr.dll", CharSet=CharSet.Unicode)] public static extern int RmStartSession(out uint pSessionHandle, int dwSessionFlags, string strSessionKey);
  [DllImport("rstrtmgr.dll")] public static extern int RmEndSession(uint pSessionHandle);
  [DllImport("rstrtmgr.dll", CharSet=CharSet.Unicode)] public static extern int RmRegisterResources(uint pSessionHandle, uint nFiles, string[] rgsFilenames, uint nApplications, IntPtr rgApplications, uint nServices, string[] rgsServiceNames);
  [DllImport("rstrtmgr.dll")] public static extern int RmGetList(uint dwSessionHandle, out uint pnProcInfoNeeded, ref uint pnProcInfo, [In, Out] RM_PROCESS_INFO[] rgAffectedApps, ref uint lpdwRebootReasons);

  public static int[] Holders(string[] files) {
    uint h; string key = Guid.NewGuid().ToString();
    if (RmStartSession(out h, 0, key) != 0) return new int[0];
    try {
      if (RmRegisterResources(h, (uint)files.Length, files, 0, IntPtr.Zero, 0, null) != 0) return new int[0];
      uint needed = 0, n = 0, reasons = 0;
      int rc = RmGetList(h, out needed, ref n, null, ref reasons);
      if (rc != 234 /*ERROR_MORE_DATA*/) return new int[0];
      if (needed == 0) return new int[0];
      RM_PROCESS_INFO[] arr = new RM_PROCESS_INFO[needed]; n = needed;
      rc = RmGetList(h, out needed, ref n, arr, ref reasons);
      if (rc != 0) return new int[0];
      int[] pids = new int[n]; for (int i = 0; i < n; i++) pids[i] = arr[i].Process.dwProcessId;
      return pids;
    } finally { RmEndSession(h); }
  }
}
"@

$fontDir = Join-Path $Dir 'rc_resource\font'
if (-not (Test-Path -LiteralPath $fontDir)) { Write-Host ("no font folder : " + $fontDir); exit 0 }
$exts = '.ttf', '.ttc', '.otf', '.fon'

# a delete-pending file is listed by the directory but Get-ChildItem may throw; collect names defensively
$files = @()
foreach ($d in @(Get-ChildItem -LiteralPath $fontDir -Recurse -Directory -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName }) + @($fontDir)) {
  try { $files += [System.IO.Directory]::GetFiles($d) | Where-Object { $exts -contains [System.IO.Path]::GetExtension($_).ToLower() } } catch { }
}
$files = $files | Sort-Object -Unique

function Test-Writable([string]$f) {
  try { $fs = [System.IO.File]::Open($f, 'Open', 'ReadWrite', 'None'); $fs.Close(); return $true } catch { return $false }
}
function Test-Listed([string]$f) {
  try { return ([System.IO.Directory]::GetFiles((Split-Path $f -Parent), (Split-Path $f -Leaf)).Count -gt 0) } catch { return $false }
}
function Show-State([string]$tag) {
  $bad = 0
  foreach ($f in $files) {
    if (-not (Test-Listed $f)) { continue }          # gone (a delete-pending file disappears once released)
    if (-not (Test-Writable $f)) { $bad++; Write-Host ("  LOCKED  " + $f.Substring($Dir.Length + 1)) }
  }
  Write-Host ("[" + $tag + "] locked files : " + $bad + " / " + $files.Count)
  return $bad
}

if ((Show-State 'before') -eq 0) { Write-Host 'nothing to do'; exit 0 }

# 1) GDI registration
$n = 0
foreach ($f in $files) { for ($i = 0; $i -lt 200; $i++) { if (-not [FontForce]::RemoveFontResourceW($f)) { break }; $n++ } }
if ($n -gt 0) { $r = [IntPtr]::Zero; [void][FontForce]::SendMessageTimeout([IntPtr]0xFFFF, 0x001D, [IntPtr]::Zero, [IntPtr]::Zero, 2, 1000, [ref]$r) }
Write-Host ("[1] RemoveFontResource : " + $n + " registration(s) released")
if ((Show-State 'after-1') -eq 0) { exit 0 }

# 2) processes holding the files (Restart Manager)
$pids = [FontForce]::Holders([string[]]$files) | Sort-Object -Unique
$me = $PID
$holders = @()
foreach ($p in $pids) {
  if ($p -eq $me) { continue }
  $pr = Get-Process -Id $p -ErrorAction SilentlyContinue
  if ($pr -ne $null) { $holders += $pr }
}
if ($holders.Count -eq 0) {
  Write-Host '[2] no user process holds the files (kernel/session font mapping)'
} else {
  Write-Host '[2] processes holding the files :'
  foreach ($pr in $holders) { Write-Host ("      " + $pr.Id + "  " + $pr.ProcessName) }
  $go = $Yes
  if (-not $go) { $a = Read-Host '    close them? (explorer restarts) y/N'; $go = ($a -eq 'y' -or $a -eq 'Y') }
  if ($go) {
    foreach ($pr in $holders) {
      $nm = $pr.ProcessName.ToLower()
      if ($nm -in @('csrss','wininit','winlogon','services','lsass','smss','fontdrvhost','dwm','system')) { Write-Host ("      skip " + $nm + " (system)"); continue }
      try {
        if ($nm -eq 'explorer') { Stop-Process -Id $pr.Id -Force; Start-Sleep -Seconds 2; Start-Process explorer.exe; Write-Host '      explorer restarted' }
        else { Stop-Process -Id $pr.Id -Force; Write-Host ("      closed " + $nm) }
      } catch { Write-Host ("      could not close " + $nm + " : " + $_.Exception.Message) }
    }
    Start-Sleep -Seconds 2
  }
}
if ((Show-State 'after-2') -eq 0) { exit 0 }

# 3) font cache services
foreach ($svc in @('FontCache', 'FontCache3.0.0.0')) {
  $s = Get-Service -Name $svc -ErrorAction SilentlyContinue
  if ($s -eq $null) { continue }
  try {
    if ($s.Status -eq 'Running') { Restart-Service -Name $svc -Force -ErrorAction Stop; Write-Host ("[3] restarted service " + $svc) }
  } catch { Write-Host ("[3] could not restart " + $svc + " : " + $_.Exception.Message) }
}
Start-Sleep -Seconds 2
$left = Show-State 'after-3'
if ($left -eq 0) { exit 0 }
Write-Host 'still locked - the mapping is held by the session itself; only logoff/reboot clears it. Setup.bat will install into a side folder instead.'
exit 1
