param([string]$Path, [string]$Out)
$ErrorActionPreference="Stop"
$xl = New-Object -ComObject Excel.Application
$xl.Visible=$false; $xl.DisplayAlerts=$false
$wb = $xl.Workbooks.Open($Path, $false, $true)
$sb = New-Object System.Text.StringBuilder
foreach ($ws in $wb.Worksheets) {
    [void]$sb.AppendLine("=== SHEET: " + $ws.Name + " ===")
    $ur = $ws.UsedRange
    $rows = $ur.Rows.Count; $cols = $ur.Columns.Count
    [void]$sb.AppendLine("dims: $rows x $cols")
    for ($r=1; $r -le $rows; $r++) {
        $line = New-Object System.Collections.ArrayList
        for ($c=1; $c -le $cols; $c++) {
            $v = $ur.Cells.Item($r,$c).Text
            [void]$line.Add(($v -replace "`r`n"," / " -replace "`n"," / "))
        }
        $s = ($line -join " | ").TrimEnd(' ','|')
        if ($s.Trim() -ne "") { [void]$sb.AppendLine(("{0,3}: {1}" -f $r, $s)) }
    }
    [void]$sb.AppendLine("")
}
$wb.Close($false); $xl.Quit()
[System.IO.File]::WriteAllText($Out, $sb.ToString(), [System.Text.Encoding]::UTF8)
Write-Output ("written: " + $Out)
