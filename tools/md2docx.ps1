param(
    [Parameter(Mandatory = $true)][string]$In,
    [Parameter(Mandatory = $true)][string]$Out,
    [string]$Title = "",
    [string]$Sub = ""
)
$ErrorActionPreference = "Stop"

Add-Type @"
using System; using System.Runtime.InteropServices;
[ComImport, Guid("00000016-0000-0000-C000-000000000046"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
public interface IOleMessageFilter {
  [PreserveSig] int HandleInComingCall(int dwCallType, IntPtr hTaskCaller, int dwTickCount, IntPtr lpInterfaceInfo);
  [PreserveSig] int RetryRejectedCall(IntPtr hTaskCallee, int dwTickCount, int dwRejectType);
  [PreserveSig] int MessagePending(IntPtr hTaskCallee, int dwTickCount, int dwPendingType);
}
public class MFW2 : IOleMessageFilter {
  [DllImport("Ole32.dll")] static extern int CoRegisterMessageFilter(IOleMessageFilter n, out IOleMessageFilter o);
  public static void Register() { IOleMessageFilter o; CoRegisterMessageFilter(new MFW2(), out o); }
  public static void Revoke() { IOleMessageFilter o; CoRegisterMessageFilter(null, out o); }
  public int HandleInComingCall(int a, IntPtr b, int c, IntPtr d) { return 0; }
  public int RetryRejectedCall(IntPtr a, int b, int c) { return 200; }
  public int MessagePending(IntPtr a, int b, int c) { return 2; }
}
"@
[MFW2]::Register()

if (Test-Path $Out) { Remove-Item $Out -Force }

$word = New-Object -ComObject Word.Application
$word.Visible = $false
$word.DisplayAlerts = 0
$doc = $word.Documents.Add()
$sel = $word.Selection

$doc.PageSetup.PageWidth  = 595.3
$doc.PageSetup.PageHeight = 841.9
$doc.PageSetup.TopMargin    = 56.7
$doc.PageSetup.BottomMargin = 56.7
$doc.PageSetup.LeftMargin   = 45.0
$doc.PageSetup.RightMargin  = 45.0
$BODYW = 505.3

$doc.Styles.Item(-1).Font.Name        = "맑은 고딕"
$doc.Styles.Item(-1).Font.NameFarEast = "맑은 고딕"
$doc.Styles.Item(-1).Font.Size        = 10

function GoEnd { $sel.EndKey(6) | Out-Null }

# 인라인 마크업 제거 : **굵게** `코드` [텍스트](링크)
function Clean([string]$t) {
    if ($null -eq $t) { return "" }
    $t = [regex]::Replace($t, '\[([^\]]*)\]\([^\)]*\)', '$1')
    $t = $t -replace '\*\*', ''
    $t = $t -replace '`', ''
    $t = $t -replace '<br\s*/?>', ' '
    $t = $t -replace '&lt;', '<' -replace '&gt;', '>' -replace '&amp;', '&'
    return $t.Trim()
}

function Hd([string]$t, [double]$size, [int]$color, [int]$outline, [double]$spBefore, [double]$spAfter) {
    GoEnd
    $sel.Style = $doc.Styles.Item(-1)
    $sel.ParagraphFormat.OutlineLevel = $outline
    $sel.ParagraphFormat.SpaceBefore  = $spBefore
    $sel.ParagraphFormat.SpaceAfter   = $spAfter
    $sel.ParagraphFormat.LeftIndent   = 0
    $sel.Font.Size = $size
    $sel.Font.Bold = -1
    if ($color -ge 0) { $sel.Font.Color = $color }
    $sel.TypeText($t)
    $sel.TypeParagraph()
    $sel.ParagraphFormat.OutlineLevel = 10   # wdOutlineLevelBodyText
    $sel.ParagraphFormat.SpaceBefore  = 0
    $sel.Font.Bold  = 0
    $sel.Font.Color = 0
    $sel.Font.Size  = 10
}

function P([string]$t, [int]$style, [double]$size, [bool]$bold, [int]$color, [double]$spAfter) {
    GoEnd
    $sel.Style = $doc.Styles.Item($style)
    $sel.ParagraphFormat.SpaceAfter = $spAfter
    if ($size -gt 0) { $sel.Font.Size = $size }
    $sel.Font.Bold = $(if ($bold) { -1 } else { 0 })
    if ($color -ge 0) { $sel.Font.Color = $color }
    $sel.TypeText($t)
    if ($style -ne -1) { $sel.Range.ListFormat.RemoveNumbers() }
    $sel.TypeParagraph()
    $sel.Font.Bold = 0
    $sel.Font.Color = 0
    $sel.Style = $doc.Styles.Item(-1)
}

function Tbl($rows, [double[]]$w) {
    GoEnd
    $r = $rows.Count; $c = $rows[0].Count
    $tbl = $doc.Tables.Add($sel.Range, $r, $c)
    $tbl.Borders.InsideLineStyle  = 1
    $tbl.Borders.OutsideLineStyle = 1
    $tbl.Range.Font.Size = 8.5
    $tbl.Range.Font.Name = "맑은 고딕"
    $tbl.Range.ParagraphFormat.SpaceAfter = 0
    $tbl.Rows.Item(1).HeadingFormat = -1
    for ($i = 0; $i -lt $c; $i++) { $tbl.Columns.Item($i + 1).Width = $BODYW * $w[$i] }
    for ($i = 0; $i -lt $r; $i++) {
        for ($j = 0; $j -lt $c; $j++) {
            $txt = [string]$rows[$i][$j]
            if ($txt -ne "") { $tbl.Cell($i + 1, $j + 1).Range.Text = $txt }
        }
    }
    $tbl.Rows.Item(1).Range.Font.Bold = -1
    $tbl.Rows.Item(1).Shading.BackgroundPatternColor = 15132390
    GoEnd
    $sel.TypeParagraph()
}

# ── 표지 ──────────────────────────────────────────────────────────
$lines = Get-Content -Encoding UTF8 $In
if ($Title -eq "") {
    foreach ($l in $lines) { if ($l -match '^#\s+(.+)$') { $Title = Clean $Matches[1]; break } }
}
GoEnd
$sel.ParagraphFormat.Alignment = 1
$sel.Font.Size = 22; $sel.Font.Bold = -1; $sel.Font.Color = 6299648
$sel.TypeText($Title)
$sel.TypeParagraph()
$sel.Font.Size = 11; $sel.Font.Bold = 0; $sel.Font.Color = 0
if ($Sub -ne "") { $sel.TypeText($Sub); $sel.TypeParagraph() }
$sel.ParagraphFormat.Alignment = 0
$sel.TypeParagraph()

# ── 본문 파싱 ─────────────────────────────────────────────────────
$i = 0
$firstH1Seen = $false
$inCode = $false
while ($i -lt $lines.Count) {
    $ln = $lines[$i]

    if ($ln -match '^```') { $inCode = -not $inCode; $i++; continue }
    if ($inCode) {
        GoEnd
        $sel.Font.Name = "Consolas"; $sel.Font.Size = 8.5
        $sel.ParagraphFormat.LeftIndent = 14
        $sel.ParagraphFormat.SpaceAfter = 0
        $sel.TypeText($ln)
        $sel.TypeParagraph()
        $sel.Font.Name = "맑은 고딕"; $sel.Font.Size = 10
        $sel.ParagraphFormat.LeftIndent = 0
        $i++; continue
    }

    if ($ln -match '^---+\s*$') { $i++; continue }
    if ($ln.Trim() -eq '') { $i++; continue }

    # 표 (헤더행 + 구분행 + 데이터행)
    if ($ln -match '^\s*\|' -and ($i + 1) -lt $lines.Count -and $lines[$i + 1] -match '^\s*\|[\s\-:\|]+\|\s*$') {
        $rows = New-Object System.Collections.ArrayList
        while ($i -lt $lines.Count -and $lines[$i] -match '^\s*\|') {
            if ($lines[$i] -match '^\s*\|[\s\-:\|]+\|\s*$') { $i++; continue }
            $cells = $lines[$i].Trim().Trim('|') -split '\|'
            $vals = @()
            foreach ($cv in $cells) { $vals += (Clean $cv) }
            [void]$rows.Add($vals)
            $i++
        }
        # 열 너비 : 각 열 최대 글자수 비례 (최소 6%)
        $c = $rows[0].Count
        $mx = New-Object double[] $c
        foreach ($rw in $rows) {
            for ($j = 0; $j -lt $c; $j++) {
                $len = [double]([string]$rw[$j]).Length
                if ($len -gt $mx[$j]) { $mx[$j] = $len }
            }
        }
        # 제곱근 비례 : 글자수 그대로 쓰면 짧은 열이 과도하게 좁아져 세로로 깨진다
        $sum = 0.0
        for ($j = 0; $j -lt $c; $j++) {
            if ($mx[$j] -lt 5) { $mx[$j] = 5 }
            if ($mx[$j] -gt 60) { $mx[$j] = 60 }
            $mx[$j] = [Math]::Sqrt($mx[$j])
            $sum += $mx[$j]
        }
        $w = New-Object double[] $c
        for ($j = 0; $j -lt $c; $j++) { $w[$j] = $mx[$j] / $sum }
        # 하한 8% / 상한 42% 로 보정한 뒤 다시 정규화 (좁은 열이 세로로 깨지는 것 방지)
        $lo = $(if ($c -ge 8) { 0.055 } else { 0.09 })
        for ($j = 0; $j -lt $c; $j++) {
            if ($w[$j] -lt $lo)  { $w[$j] = $lo }
            if ($w[$j] -gt 0.42) { $w[$j] = 0.42 }
        }
        $tot = 0.0; for ($j = 0; $j -lt $c; $j++) { $tot += $w[$j] }
        for ($j = 0; $j -lt $c; $j++) { $w[$j] = $w[$j] / $tot }
        Tbl $rows $w
        continue
    }

    if ($ln -match '^####\s+(.+)$') { Hd (Clean $Matches[1]) 10 0 4 6 3; $i++; continue }
    if ($ln -match '^###\s+(.+)$')  { Hd (Clean $Matches[1]) 10.5 0 3 8 4; $i++; continue }
    if ($ln -match '^##\s+(.+)$')   { Hd (Clean $Matches[1]) 13 6299648 1 12 6; $i++; continue }
    if ($ln -match '^#\s+(.+)$') {
        if ($firstH1Seen) {
            GoEnd; $sel.InsertBreak(7) | Out-Null
        }
        $firstH1Seen = $true
        $i++; continue      # 최상위 제목은 표지에서 이미 출력
    }
    if ($ln -match '^\s*>\s?(.*)$') { P (Clean $Matches[1]) -1 9.5 $true 6299648 6; $i++; continue }
    if ($ln -match '^\s*[-*]\s+(.+)$') {
        GoEnd
        $sel.Style = $doc.Styles.Item(-1)
        $sel.ParagraphFormat.LeftIndent = 14
        $sel.ParagraphFormat.SpaceAfter = 2
        $sel.Font.Size = 10
        $sel.TypeText([char]0x00B7 + " " + (Clean $Matches[1]))
        $sel.TypeParagraph()
        $sel.ParagraphFormat.LeftIndent = 0
        $i++; continue
    }
    P (Clean $ln) -1 10 $false 0 3
    $i++
}

# 머리글/바닥글 : 쪽번호
$sec = $doc.Sections.Item(1)
$ftr = $sec.Footers.Item(1).Range
$ftr.Text = ""
$ftr.Fields.Add($ftr, 33) | Out-Null       # wdFieldPage
$sec.Footers.Item(1).Range.ParagraphFormat.Alignment = 1
$sec.Footers.Item(1).Range.Font.Size = 9

$doc.SaveAs([ref]$Out, [ref]16)
$pages = $doc.ComputeStatistics(2)
$tables = $doc.Tables.Count
$doc.Close()
$word.Quit()
[MFW2]::Revoke()
Write-Output ("saved: {0}  ({1} pages, {2} tables)" -f $Out, $pages, $tables)
