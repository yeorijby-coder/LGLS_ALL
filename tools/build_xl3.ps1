param(
    [string]$Src  = "D:\project\LGLS\Renewal\시나리오 영역_시나리오그림포함_V1.1_20260820.xlsx",
    [string]$Dst  = "D:\project\LGLS\Renewal\시나리오 영역_시나리오그림포함_V1.2_20260820.xlsx",
    [string]$Json = "scen.json"
)
$ErrorActionPreference = "Stop"
$data = Get-Content -Raw -Encoding UTF8 $Json | ConvertFrom-Json

# 시트 순서 ↔ 시나리오 id
$SHEET_ID = @("1", "3-1", "4-1", "4-2", "3-2", "2")

# ── 그림 규격 (전 시트 동일) ──────────────────────────────────────────
$FONT      = "맑은 고딕"
$FS_TITLE  = 13     # 시나리오 제목
$FS_HEAD   = 10     # EQP / ECS 머리글
$FS_SEC    = 10     # 구간 머리글
$FS_SIG    = 9      # 신호명
$FS_ADDR   = 8      # 주소
$FS_NOTE   = 8      # 범례

$X0        = 6      # 그림 좌측
$W_ALL     = 660    # 그림 폭
$X_ADDR_L  = 6      # 좌측 주소열
$W_ADDR    = 118
$X_EQP     = 132    # EQP 라이프라인
$X_ECS     = 592    # ECS 라이프라인
$H_STEP    = 26     # 스텝 1행 높이
$H_SEC     = 24     # 구간 머리글 높이

$C_EQP     = 15773696   # 파랑 계열 (BGR)
$C_ECS     = 3050327    # 초록 계열
$C_WORD    = 1937407    # 주황 계열
$C_SEC     = 15921906   # 연회색 배경
$C_LINE    = 12632256   # 라이프라인 회색

Copy-Item $Src $Dst -Force

$xl = New-Object -ComObject Excel.Application
$xl.Visible = $false
$xl.DisplayAlerts = $false
$wb = $xl.Workbooks.Open($Dst)

function Get-Scen($id) { foreach ($s in $data.scenarios) { if ($s.id -eq $id) { return $s } } return $null }

function Split-Sections($steps) {
    $secs = @(); $cur = $null; $key = $null
    foreach ($s in $steps) {
        $k = $s.equip + "#" + $s.per.'1'.no
        if ($k -ne $key) {
            if ($cur) { $secs += ,$cur }
            $cur = [pscustomobject]@{ Equip = $s.equip; No = $s.per.'1'.no; Steps = New-Object System.Collections.ArrayList }
            $key = $k
        }
        [void]$cur.Steps.Add($s)
    }
    if ($cur) { $secs += ,$cur }
    return $secs
}

function Bit-Label([int]$a) { "M{0:000}.{1}" -f [int]($a/16), ($a % 16) }

# [LGLS 2026-08-21] 표기 규칙 : "M/D/R 영역 문서표기  실제 접근주소"
#   M : M037.1  %MX593      (비트)
#   D : D0310   %DW490      (워드, %DB=워드x2)
#   R : R0102   %RB516      (트래킹, 진법은 XML rAddrMode 적용 후)
function Addr-Text($step) {
    $p = $step.per.'1'
    if ($p.addr -lt 0) { return "-" }
    $lab = [string]$p.label
    if ([string]::IsNullOrEmpty($lab)) { $lab = (Bit-Label $p.addr) }
    switch ($p.dev) {
        "M" { return $lab + "  %MX" + $p.addr }
        "D" { return $lab + "  %DW" + $p.addr }
        "R" { return $lab + "  %RB" + ($p.addr * 2) }
    }
    return $lab + "  %MX" + $p.addr
}

function Eq-Name($equip, $no) {
    switch ($equip) { "CV" { return "C/V #" + $no } "SC" { return "S/C #" + $no } "RGV" { return "RGV #" + $no } }
    return $equip
}

# 글자 크기·글꼴을 한 곳에서만 정한다 → 전 도형 통일
function Add-Text($ws, $x, $y, $w, $h, $text, $size, $bold, $color, $align) {
    $tb = $ws.Shapes.AddTextbox(1, $x, $y, $w, $h)
    $tf = $tb.TextFrame
    $tf.MarginLeft = 1; $tf.MarginRight = 1; $tf.MarginTop = 0; $tf.MarginBottom = 0
    $tf.Characters().Text = $text
    $tf.Characters().Font.Name = $FONT
    $tf.Characters().Font.Size = $size
    $tf.Characters().Font.Bold = $bold
    $tf.Characters().Font.Color = $color
    $tf.HorizontalAlignment = $align       # -4131 왼쪽 / -4108 가운데 / -4152 오른쪽
    $tf.VerticalAlignment = -4108
    $tb.Line.Visible = $false
    $tb.Fill.Visible = $false
    return $tb
}

function Add-Box($ws, $x, $y, $w, $h, $text, $size, $fill, $fontColor) {
    $sh = $ws.Shapes.AddShape(1, $x, $y, $w, $h)     # msoShapeRectangle
    $sh.Fill.ForeColor.RGB = $fill
    $sh.Line.ForeColor.RGB = $C_LINE
    $sh.Line.Weight = 0.75
    $tf = $sh.TextFrame
    $tf.MarginLeft = 4; $tf.MarginRight = 4; $tf.MarginTop = 0; $tf.MarginBottom = 0
    $tf.Characters().Text = $text
    $tf.Characters().Font.Name = $FONT
    $tf.Characters().Font.Size = $size
    $tf.Characters().Font.Bold = $true
    $tf.Characters().Font.Color = $fontColor
    $tf.HorizontalAlignment = -4108
    $tf.VerticalAlignment = -4108
    return $sh
}

function Add-Arrow($ws, $x1, $y, $x2, $color, $dash) {
    $ln = $ws.Shapes.AddLine($x1, $y, $x2, $y)
    $ln.Line.ForeColor.RGB = $color
    $ln.Line.Weight = 1.25
    $ln.Line.EndArrowheadStyle = 2          # msoArrowheadTriangle
    $ln.Line.EndArrowheadLength = 2
    $ln.Line.EndArrowheadWidth = 2
    if ($dash) { $ln.Line.DashStyle = 4 }   # msoLineDash
    return $ln
}

for ($si = 0; $si -lt 6; $si++) {
    $ws = $wb.Worksheets.Item($si + 1)
    $sc = Get-Scen $SHEET_ID[$si]
    Write-Output ("sheet " + ($si + 1) + " : " + $ws.Name + "  <- " + $sc.title)

    # 기존 그림(슬라이드 캡처) 전부 제거
    for ($k = $ws.Shapes.Count; $k -ge 1; $k--) { $ws.Shapes.Item($k).Delete() }
    # V1.1 이 넣어둔 셀 안내문 제거 (새 제목 도형과 겹친다)
    $ws.Range("A1:M2").ClearContents() | Out-Null

    $secs = Split-Sections $sc.steps
    $marks = @("①", "②", "③", "④", "⑤")

    # ── 제목 ──
    $y = 6.0
    Add-Text $ws $X0 $y $W_ALL 20 ("반송 시나리오 " + $sc.title) $FS_TITLE $true 0 -4108 | Out-Null
    $y += 22

    # ── 범례 ──
    Add-Text $ws $X0 $y $W_ALL 14 `
        "실선 → 비트 ON/OFF   ·   점선 → D/R 워드 값 기록·확인   ·   왼쪽 끝 = M/D/R 영역 문서표기 + 실제 접근주소 (S/C #1 기준)" `
        $FS_NOTE $false 8421504 -4108 | Out-Null
    $y += 18

    # ── EQP / ECS 머리글 ──
    $headTop = $y
    Add-Box $ws ($X_EQP - 60) $y 120 20 "EQP  (설비 / PLC)" $FS_HEAD 15790320 $C_EQP | Out-Null
    Add-Box $ws ($X_ECS - 60) $y 120 20 "ECS  (WCS)"       $FS_HEAD 15790320 $C_ECS | Out-Null
    $y += 24

    $lifeTop = $y

    # ── 구간별 스텝 ──
    $stepNo = 1
    for ($k = 0; $k -lt $secs.Count; $k++) {
        $sec = $secs[$k]
        $secName = Eq-Name $sec.Equip $sec.No

        # 구간 머리글 띠
        $band = $ws.Shapes.AddShape(1, $X0, $y, $W_ALL, $H_SEC)
        $band.Fill.ForeColor.RGB = $C_SEC
        $band.Line.Visible = $false
        $tf = $band.TextFrame
        $tf.MarginLeft = 8; $tf.MarginTop = 0; $tf.MarginBottom = 0
        $tf.Characters().Text = ($marks[$k] + "  " + $secName + "   —   " + $sec.Steps.Count + " 스텝")
        $tf.Characters().Font.Name = $FONT
        $tf.Characters().Font.Size = $FS_SEC
        $tf.Characters().Font.Bold = $true
        $tf.Characters().Font.Color = 0
        $tf.HorizontalAlignment = -4131
        $tf.VerticalAlignment = -4108
        $y += $H_SEC + 2

        foreach ($st in $sec.Steps) {
            $isWord = ($st.kind -eq "word")
            $isEqp  = ($st.kind -eq "observe")
            $col = if ($isWord) { $C_WORD } elseif ($isEqp) { $C_EQP } else { $C_ECS }

            # 좌측 주소
            Add-Text $ws $X_ADDR_L $y $W_ADDR $H_STEP (Addr-Text $st) $FS_ADDR $false $col -4152 | Out-Null

            # 신호명 (라이프라인 사이 상단)
            $sigTxt = ("{0}. {1}" -f $stepNo, $st.desc)
            Add-Text $ws ($X_EQP + 6) $y ($X_ECS - $X_EQP - 12) ($H_STEP - 9) $sigTxt $FS_SIG $false 0 -4108 | Out-Null

            # 화살표 : EQP→ECS(설비 보고) / ECS→EQP(WCS 응답·기록)
            $ay = $y + $H_STEP - 5
            if ($isEqp) { Add-Arrow $ws $X_EQP $ay $X_ECS $col $false | Out-Null }
            else        { Add-Arrow $ws $X_ECS $ay $X_EQP $col $isWord | Out-Null }

            $y += $H_STEP
            $stepNo++
        }
        $y += 4
    }

    # ── 배경 흰 판 (셀 격자선을 가려 그림처럼 보이게) ──
    $bg = $ws.Shapes.AddShape(1, ($X0 - 4), 2, ($W_ALL + 8), ($y + 6))
    $bg.Fill.ForeColor.RGB = 16777215
    $bg.Line.ForeColor.RGB = $C_LINE
    $bg.Line.Weight = 0.75
    $bg.ZOrder(1) | Out-Null      # msoSendToBack

    # ── 라이프라인 (마지막에 그려 길이를 확정) ──
    foreach ($lx in @($X_EQP, $X_ECS)) {
        $ln = $ws.Shapes.AddLine($lx, $lifeTop, $lx, $y)
        $ln.Line.ForeColor.RGB = $C_LINE
        $ln.Line.Weight = 1.0
        $ln.Line.DashStyle = 4
        $ln.ZOrder(1) | Out-Null      # msoSendToBack
        $ln.ZOrder(4) | Out-Null      # msoBringForward (배경 위로)
    }

    Write-Output ("   도형 " + $ws.Shapes.Count + " 개, 높이 " + [int]$y + "pt")
}

$wb.Save()
$wb.Close($true)
$xl.Quit()
Write-Output ("saved: " + $Dst)
