# Release fonts that the Client registered with AddFontResource (they stay locked
# until logoff even after the Client exits), and optionally verify the copy.
#   -Dir     install folder
#   -SrcDir  package folder (Client) - font names are taken from here too
#   -Check   list files that exist in SrcDir but not in Dir; exit 1 if any
param([Parameter(Mandatory=$true)][string]$Dir, [string]$SrcDir = "", [switch]$Check, [switch]$Probe)

Add-Type @"
using System; using System.Runtime.InteropServices;
public class FontRel {
  [DllImport("gdi32.dll", CharSet=CharSet.Unicode)] public static extern bool RemoveFontResourceW(string f);
  [DllImport("user32.dll")] public static extern IntPtr SendMessageTimeout(IntPtr h, uint m, IntPtr w, IntPtr l, uint f, uint t, out IntPtr r);
}
"@

function Get-Rel([string]$root) {
  if (-not $root -or -not (Test-Path -LiteralPath $root)) { return @() }
  $r = (Resolve-Path -LiteralPath $root).Path.TrimEnd('\')
  @(Get-ChildItem -LiteralPath $r -Recurse -File -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName.Substring($r.Length + 1) })
}

if ($Probe) {
  # delete-pending font in Dir?  exit 2 = yes (folder unusable until logoff), 0 = fine
  if (-not $Dir -or -not (Test-Path -LiteralPath $Dir)) { exit 0 }
  $exts = '.ttf', '.ttc', '.otf', '.fon'
  $dstRoot = $Dir.TrimEnd('\')
  foreach ($rel in (Get-Rel $SrcDir)) {
    if ($exts -notcontains [System.IO.Path]::GetExtension($rel).ToLower()) { continue }
    $full = Join-Path $dstRoot $rel
    $dir  = Split-Path $full -Parent
    $name = Split-Path $full -Leaf
    if (-not (Test-Path -LiteralPath $dir)) { continue }
    $listed = $false
    try { $listed = ([System.IO.Directory]::GetFiles($dir, $name).Count -gt 0) } catch { $listed = $false }
    if (-not $listed) { continue }
    $ok = $false
    try { $fs = [System.IO.File]::Open($full, 'Open', 'Read', 'ReadWrite'); $fs.Close(); $ok = $true } catch { $ok = $false }
    if (-not $ok) { Write-Host ("        locked " + $rel); exit 2 }
  }
  exit 0
}

if ($Check) {
  $dstRoot = $Dir.TrimEnd('\')
  $miss = @()
  foreach ($rel in (Get-Rel $SrcDir)) {
    $ok = $false
    try { $ok = [System.IO.File]::Exists((Join-Path $dstRoot $rel)) } catch { $ok = $false }
    if ($ok) { try { $fs = [System.IO.File]::Open((Join-Path $dstRoot $rel), 'Open', 'Read', 'ReadWrite'); $fs.Close() } catch { $ok = $false } }
    if (-not $ok) { $miss += $rel }
  }
  if ($miss.Count -gt 0) {
    Write-Host ("        missing " + $miss.Count + " :")
    $miss | Select-Object -First 10 | ForEach-Object { Write-Host ("          " + $_) }
    exit 1
  }
  exit 0
}

# font files : those present in Dir + the names the package has (a locked file may not be listable)
$exts = '.ttf', '.ttc', '.otf', '.fon'
$rels = @()
$rels += Get-Rel $Dir    | Where-Object { $exts -contains [System.IO.Path]::GetExtension($_).ToLower() }
$rels += Get-Rel $SrcDir | Where-Object { $exts -contains [System.IO.Path]::GetExtension($_).ToLower() }
$rels = $rels | Sort-Object -Unique
$n = 0
foreach ($rel in $rels) {
  $full = Join-Path $Dir.TrimEnd('\') $rel
  for ($i = 0; $i -lt 100; $i++) { if (-not [FontRel]::RemoveFontResourceW($full)) { break }; $n++ }
}
if ($n -gt 0) {
  $r = [IntPtr]::Zero
  [void][FontRel]::SendMessageTimeout([IntPtr]0xFFFF, 0x001D, [IntPtr]::Zero, [IntPtr]::Zero, 2, 1000, [ref]$r)
}
Write-Host ("released " + $n)
exit 0
