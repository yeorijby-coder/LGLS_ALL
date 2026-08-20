param(
    [string]$Src = "D:\project\LGLS\Renewal\시나리오 영역.xlsx",
    [string]$Dst = "D:\project\LGLS\Renewal\시나리오 영역_시나리오그림포함_V1.1_20260820.xlsx",
    [string]$Img = "C:\Users\USER\AppData\Local\Temp\claude\D--project-LGLS-Renewal\acbf154d-f43c-4419-af82-1b94e3fa8c7d\scratchpad\chimg"
)
$ErrorActionPreference = "Stop"

# 시트 순서 ↔ V2.0 챕터의 슬라이드 번호 (개요 1 + 구간 다이어그램 8)
$MAP = @(
    @{ Sheet = 1; Ov = 3;  Dg = @(4,5,6,7,8,9,10,11);      Name = "시나리오 1  입출고대 #22 → S/C 입고" },
    @{ Sheet = 2; Ov = 29; Dg = @(30,31,32,33,34,35,36,37); Name = "시나리오 3-1  26 입고대 → S/C 입고" },
    @{ Sheet = 3; Ov = 55; Dg = @(56,57,58,59,60,61,62,63); Name = "시나리오 4-1  30 입고대 → S/C 입고" },
    @{ Sheet = 4; Ov = 68; Dg = @(69,70,71,72,73,74,75,76); Name = "시나리오 4-2  S/C → 29 피킹대 출고" },
    @{ Sheet = 5; Ov = 42; Dg = @(43,44,45,46,47,48,49,50); Name = "시나리오 3-2  S/C → 24 출고대 출고" },
    @{ Sheet = 6; Ov = 16; Dg = @(17,18,19,20,21,22,23,24); Name = "시나리오 2  S/C → 입출고대 #22 출고" }
)

$COLS = 13          # 왼쪽에 끼워 넣을 열 수
$CW   = 9           # 각 열 너비(문자) ≈ 50pt

Copy-Item $Src $Dst -Force

$xl = New-Object -ComObject Excel.Application
$xl.Visible = $false
$xl.DisplayAlerts = $false
$wb = $xl.Workbooks.Open($Dst)

foreach ($m in $MAP) {
    $ws = $wb.Worksheets.Item([int]$m.Sheet)
    Write-Output ("sheet " + $m.Sheet + " : " + $ws.Name)

    # ── 왼쪽에 그림 자리 확보 (데이터를 오른쪽으로 밀어낸다) ──
    for ($i = 0; $i -lt $COLS; $i++) { $ws.Columns.Item(1).Insert() | Out-Null }
    for ($i = 1; $i -le $COLS; $i++) { $ws.Columns.Item($i).ColumnWidth = $CW }

    # ── 안내 제목 ──
    $ws.Cells.Item(1, 1).Value2 = "▼ 반송 시나리오 (시작 → 끝)   —   " + $m.Name
    $ws.Cells.Item(1, 1).Font.Bold = $true
    $ws.Cells.Item(1, 1).Font.Size = 12
    $ws.Cells.Item(2, 1).Value2 = "개요 + 구간 다이어그램 ①→④ (좌→우, 위→아래).  오른쪽 표의 '구간' 열과 대응한다."
    $ws.Cells.Item(2, 1).Font.Size = 9
    $ws.Cells.Item(2, 1).Font.ColorIndex = 5

    # ── 개요 그림 (상단, 폭 넓게) ──
    $ovFile = Join-Path $Img ("s" + $m.Ov + ".png")
    $ov = $ws.Shapes.AddPicture($ovFile, $false, $true, 6, 34, 646, 364)
    $ov.Line.Visible = $true
    $ov.Line.ForeColor.RGB = 12632256

    # ── 구간 다이어그램 8장 : 1열 세로 배치 (읽을 수 있는 크기) ──
    $y = 406.0
    $w = 646.0
    $h = 364.0
    $cap = @("① 구간 (1/2)", "① 구간 (2/2)", "② 구간 (1/2)", "② 구간 (2/2)",
             "③ 구간 (1/2)", "③ 구간 (2/2)", "④ 구간 (1/2)", "④ 구간 (2/2)")
    for ($k = 0; $k -lt 8; $k++) {
        $lbl = $ws.Shapes.AddTextbox(1, 6, $y, $w, 16)
        $lbl.TextFrame.Characters().Text = $cap[$k]
        $lbl.TextFrame.Characters().Font.Size = 10
        $lbl.TextFrame.Characters().Font.Bold = $true
        $lbl.Line.Visible = $false
        $lbl.Fill.Visible = $false
        $y = $y + 18
        $f = Join-Path $Img ("s" + $m.Dg[$k] + ".png")
        $sh = $ws.Shapes.AddPicture($f, $false, $true, 6, $y, $w, $h)
        $sh.Line.Visible = $true
        $sh.Line.ForeColor.RGB = 12632256
        $y = $y + $h + 10
    }
}

# ── 시트 5·6 의 RGV To 설명 오기 정정 (시트4에서 복사된 흔적) ──
$fix = @(
    # 신호/설명 열(원래 D) : 시트4에서 복사된 흔적
    @{ Sheet = 5; Row = 39; Col = 4; Text = "Transfer Command Data - To (24 출고대 #23)" },
    @{ Sheet = 6; Row = 39; Col = 4; Text = "Transfer Command Data - To (입출고대 #21)" },
    # 구간 열(원래 B) : 라벨과 실제 주소의 설비번호 불일치
    @{ Sheet = 3; Row = 4;  Col = 2; Text = "CV #15" },
    @{ Sheet = 6; Row = 52; Col = 2; Text = "CV #11" }
)
foreach ($f in $fix) {
    $ws = $wb.Worksheets.Item([int]$f.Sheet)
    $c = $ws.Cells.Item([int]$f.Row, $COLS + [int]$f.Col)
    $old = $c.Text
    $c.Value2 = $f.Text
    $c.Interior.Color = 65535
    Write-Output ("  fix sheet " + $f.Sheet + " r" + $f.Row + " c" + $f.Col + " : [" + $old + "] -> [" + $f.Text + "]")
}


$wb.Save()
$wb.Close($true)
$xl.Quit()
Write-Output ("saved: " + $Dst)
