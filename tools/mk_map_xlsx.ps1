param([string]$Json = "map_tbl.json",
      [string]$Out  = "D:\project\LGLS\Renewal\PLC원본_구ECS_영역매칭표_20260822.xlsx")
$ErrorActionPreference = "Stop"
$rows = Get-Content -Raw -Encoding UTF8 $Json | ConvertFrom-Json
if (Test-Path $Out) { Remove-Item $Out -Force }

$xl = New-Object -ComObject Excel.Application
$xl.Visible = $false; $xl.DisplayAlerts = $false
$wb = $xl.Workbooks.Add()
while ($wb.Worksheets.Count -gt 1) { $wb.Worksheets.Item($wb.Worksheets.Count).Delete() }

# ── 시트1 : 매칭표 ─────────────────────────────────────────────
$ws = $wb.Worksheets.Item(1)
$ws.Name = "영역 매칭표"
$hdr = @("슬라이드","슬라이드 내용","PLC 원본 표기","영역","실제 접근주소",
         "구 ECS 설비ID","구 ECS 관측명(TAG)","구 ECS 주소표기",
         "정의 파일","Observable 프로퍼티","구현 파일","핸들러","의미 함수","비고")
for ($c = 1; $c -le $hdr.Count; $c++) {
    $ws.Cells.Item(1, $c).Value2 = $hdr[$c-1]
}
$r = 2
foreach ($x in $rows) {
    $ws.Cells.Item($r,1).Value2  = [int]$x.slide
    $ws.Cells.Item($r,2).Value2  = [string]$x.title
    $ws.Cells.Item($r,3).Value2  = [string]$x.tok
    $ws.Cells.Item($r,4).Value2  = [string]$x.dev
    $ws.Cells.Item($r,5).Value2  = [string]$x.real
    $ws.Cells.Item($r,6).Value2  = [string]$x.owner
    $ws.Cells.Item($r,7).Value2  = [string]$x.tag
    $ws.Cells.Item($r,8).Value2  = [string]$x.ecsaddr
    $ws.Cells.Item($r,9).Value2  = [string]$x.cfgfile
    $ws.Cells.Item($r,10).Value2 = [string]$x.prop
    $ws.Cells.Item($r,11).Value2 = [string]$x.unitfile
    $ws.Cells.Item($r,12).Value2 = [string]$x.handler
    $ws.Cells.Item($r,13).Value2 = [string]$x.func
    $ws.Cells.Item($r,14).Value2 = [string]$x.note
    $r++
}
$last = $r - 1
$hd = $ws.Range($ws.Cells.Item(1,1), $ws.Cells.Item(1,$hdr.Count))
$hd.Font.Bold = $true
$hd.Interior.Color = 15773696
$hd.Font.Color = 16777215
$ws.Range($ws.Cells.Item(1,1), $ws.Cells.Item($last,$hdr.Count)).Borders.LineStyle = 1
$ws.Range($ws.Cells.Item(1,1), $ws.Cells.Item($last,$hdr.Count)).Font.Size = 9
$ws.Range("A1:N1").HorizontalAlignment = -4108
$ws.Columns.Item(2).ColumnWidth = 34
$ws.Columns.Item(12).ColumnWidth = 42
foreach ($c in @(1,3,4,5,6,7,8,9,10,11,13,14)) { $ws.Columns.Item($c).AutoFit() | Out-Null }
$ws.Rows.Item(1).AutoFilter() | Out-Null
$xl.ActiveWindow.SplitRow = 1
$xl.ActiveWindow.FreezePanes = $true

# ── 시트2 : 요약 ───────────────────────────────────────────────
$ws2 = $wb.Worksheets.Add([System.Reflection.Missing]::Value, $ws)
$ws2.Name = "요약"
$sum = @(
 @("PLC 원본 ↔ 구 ECS 영역 매칭표",""),
 @("",""),
 @("원본 문서","260818_ECS-분석-PLC사양및시나리오_LG생명과학_V1.1.ppt (PLC 업체 제공, 26슬라이드)"),
 @("대조 기준","구 ECS 운영 정의 SIM\EQP_SIM\observables.tsv (= TB_OBSERVABLE 덤프, 529행)"),
 @("구 ECS 소스","Backup\ECS\Device\Unit\  (Conveyor/Vehicle + 각 Config)"),
 @("작성일","2026-08-22"),
 @("",""),
 @("대조 결과",""),
 @("  고유 주소","120 개"),
 @("  구 ECS 와 일치","120 개"),
 @("  불일치","0 개"),
 @("  구현 함수 매칭","120 개"),
 @("",""),
 @("표기 규칙 (PLC 원본 → 실주소)",""),
 @("  M0211","앞 3자리=워드, 끝 1자리=비트  →  21x16+1 = %MX337"),
 @("  D0161","문서 라벨(설비당 +10, 실주소 +16)  →  %DW257"),
 @("  R0020","16진 워드주소  →  워드 32 (%RB64)"),
 @("",""),
 @("읽는 법",""),
 @("  정의 파일","해당 관측명이 Observable 로 선언된 구 ECS 파일"),
 @("  구현 파일","그 신호가 실제로 처리되는 파일"),
 @("  핸들러","PLC 값 변화를 받는 함수. '(ECS 쓰기)' 는 ECS 가 값을 내보내는 신호(Ack 등)"),
 @("  의미 함수","핸들러가 호출하는 업무 함수")
)
$rr = 1
foreach ($p in $sum) {
    $ws2.Cells.Item($rr,1).Value2 = $p[0]
    $ws2.Cells.Item($rr,2).Value2 = $p[1]
    $rr++
}
$ws2.Cells.Item(1,1).Font.Size = 14
$ws2.Cells.Item(1,1).Font.Bold = $true
$ws2.Columns.Item(1).ColumnWidth = 30
$ws2.Columns.Item(2).ColumnWidth = 78
foreach ($n in @(3,4,5,6,9,10,11,12,15,16,17,20,21,22,23)) { $ws2.Cells.Item($n,1).Font.Bold = $true }

$wb.Worksheets.Item(1).Activate()
$wb.SaveAs($Out, 51)
$wb.Close($true); $xl.Quit()
Write-Output ("saved: " + $Out + "  (" + $last + " rows)")
