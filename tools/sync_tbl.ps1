param([string]$Doc = "D:\project\LGLS\Renewal\시나리오 영역_시나리오그림포함_V1.2_20260820.xlsx",
      [string]$Json = "sync_cells.json", [int]$ColOffset = 13)
$ErrorActionPreference="Stop"
$fix = Get-Content -Raw -Encoding UTF8 $Json | ConvertFrom-Json
$xl = New-Object -ComObject Excel.Application
$xl.Visible=$false; $xl.DisplayAlerts=$false
$wb = $xl.Workbooks.Open($Doc)
foreach ($f in $fix) {
    $ws = $wb.Worksheets.Item([int]$f.sheet)
    $c = $ws.Cells.Item([int]$f.row, $ColOffset + [int]$f.col)
    $old = $c.Text
    $c.Value2 = $f.text
    $c.Interior.Color = 65535     # 노랑 = 정정 표시
    Write-Output ("sheet{0} r{1} c{2} : [{3}] -> [{4}]" -f $f.sheet, $f.row, $f.col, $old, $f.text)
}
$wb.Save(); $wb.Close($true); $xl.Quit()
Write-Output ("synced: " + $fix.Count)
