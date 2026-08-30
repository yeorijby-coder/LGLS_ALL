param(
    [string]$Doc  = "D:\project\LGLS\Renewal\ECS-반송시나리오6종_LG생명과학_V2.0_20260820.pptx",
    [string]$Json = "scen.json"
)
$ErrorActionPreference = "Stop"
$data = Get-Content -Raw -Encoding UTF8 $Json | ConvertFrom-Json

# 챕터 정의 : 개요 슬라이드 위치, 병기 4장 시작 위치, 시나리오 id
$CH = @(
    @{ Key="A"; Ov=3;  Tbl=12; Id="1";   Name="입출고대 #22 입고"; Dir="입고" },
    @{ Key="B"; Ov=16; Tbl=25; Id="2";   Name="입출고대 #22 출고"; Dir="출고" },
    @{ Key="C"; Ov=29; Tbl=38; Id="3-1"; Name="26 입고대 입고";     Dir="입고" },
    @{ Key="D"; Ov=42; Tbl=51; Id="3-2"; Name="24 출고대 출고";     Dir="출고" },
    @{ Key="E"; Ov=55; Tbl=64; Id="4-1"; Name="30 입고대 입고";     Dir="입고" },
    @{ Key="F"; Ov=68; Tbl=77; Id="4-2"; Name="29 피킹대 출고";     Dir="출고" }
)

$ppt  = New-Object -ComObject PowerPoint.Application
$pres = $ppt.Presentations.Open($Doc, $false, $false, $false)
$W = $pres.PageSetup.SlideWidth
$H = $pres.PageSetup.SlideHeight

function Get-Scen($id) { foreach ($s in $data.scenarios) { if ($s.id -eq $id) { return $s } } return $null }

function Split-Sections($steps) {
    $secs = @(); $cur = $null; $key = $null
    foreach ($s in $steps) {
        $k = $s.equip + "#" + $s.per.'1'.no
        if ($k -ne $key) {
            if ($cur) { $secs += ,$cur }
            $cur = [pscustomobject]@{ Equip = $s.equip; Steps = New-Object System.Collections.ArrayList }
            $key = $k
        }
        [void]$cur.Steps.Add($s)
    }
    if ($cur) { $secs += ,$cur }
    return $secs
}
function Bit-Label([int]$a) { "M{0:000}.{1}" -f [int]($a/16), ($a % 16) }
function Cell-Text($step, $cr) {
    $p = $step.per."$cr"
    if ($p.addr -lt 0) { return "-" }
    $eq = switch ($step.equip) { "CV" { "C/V #" + $p.no } "SC" { "S/C #" + $p.no } "RGV" { "RGV #" + $p.no } default { "" } }
    $ad = switch ($p.dev) {
        "M" { (Bit-Label $p.addr) + " %MX" + $p.addr }
        "D" { "%DW" + $p.addr }
        "R" { "%RB" + ($p.addr * 2) }
        default { "%MX" + $p.addr }
    }
    return "$eq  $ad"
}
function Kind-Text($k, $v) {
    switch ($k) {
        "observe" { if ($v -eq "off") { "EQP ▼" } else { "EQP ▲" } }
        "force"   { "ECS ▲" }
        default   { "ECS 값" }
    }
}

# 기존 슬라이드를 지우고 같은 자리에 제목형 슬라이드를 새로 만든다
function New-At([int]$idx, [string]$title, [double]$tSize) {
    $pres.Slides.Item($idx).Delete()
    $sl = $pres.Slides.Add($idx, 11)
    $sl.Shapes.Item(1).TextFrame.TextRange.Text = $title
    $sl.Shapes.Item(1).TextFrame.TextRange.Font.Size = $tSize
    $sl.Shapes.Item(1).Top = 10; $sl.Shapes.Item(1).Height = 42
    return $sl
}

function Add-Legend($sl) {
    $lg = $sl.Shapes.AddTextbox(1, 24, $H - 52, $W - 48, 32)
    $lg.TextFrame.WordWrap = -1
    $lg.TextFrame.TextRange.Text = "[ 주체 읽는 법 ]  EQP ▲ = 설비(PLC)가 그 비트를 ON  ·  EQP ▼ = 설비가 OFF(완료 후 리셋)  ·  ECS ▲ = WCS 가 응답 비트를 ON  ·  ECS 값 = WCS 가 D/R 워드에 값을 기록" + [char]13 + "※ R(트래킹)은 16진 해석 기준(%RB = 문서표기를 16진으로 읽은 워드 x 2). 앞의 다이어그램은 구 ECS 문서표기(10진)를 그대로 두었으므로 두 표기가 함께 나온다."
    $r = $lg.TextFrame.TextRange
    $r.Font.Size = 8
    $r.Font.Color.RGB = 0x993300
    $r.ParagraphFormat.Alignment = 1
    $lg.Line.Visible = -1
    $lg.Line.ForeColor.RGB = 0xC0C0C0
    $lg.Fill.Visible = 0
}

function Fill-Table($sl, $steps, [int]$startNo, [string]$subtitle) {
    $sub = $sl.Shapes.AddTextbox(1, 24, 48, $W - 48, 26)
    $sub.TextFrame.WordWrap = 0
    $sub.TextFrame.TextRange.Text = $subtitle
    $sub.TextFrame.TextRange.Font.Size = 9
    $sub.TextFrame.TextRange.Font.Color.RGB = 0x993300

    $rows = $steps.Count + 1
    $tbl = $sl.Shapes.AddTable($rows, 8, 24, 80, $W - 48, 18 * $rows).Table
    $hdr = @("#", "주체", "신호 / 설명", "1호기", "2호기", "3호기", "4호기", "5호기")
    $wid = @(28, 44, 209, 96, 96, 96, 96, 96)
    for ($c = 1; $c -le 8; $c++) {
        $tbl.Columns.Item($c).Width = $wid[$c - 1]
        $tr = $tbl.Cell(1, $c).Shape.TextFrame.TextRange
        $tr.Text = $hdr[$c - 1]; $tr.Font.Size = 9.5; $tr.Font.Bold = -1
    }
    for ($r = 0; $r -lt $steps.Count; $r++) {
        $s = $steps[$r]
        $vals = @(($startNo + $r).ToString(), (Kind-Text $s.kind $s.value), $s.desc,
                  (Cell-Text $s 1), (Cell-Text $s 2), (Cell-Text $s 3), (Cell-Text $s 4), (Cell-Text $s 5))
        for ($c = 1; $c -le 8; $c++) {
            $tr = $tbl.Cell($r + 2, $c).Shape.TextFrame.TextRange
            $tr.Text = $vals[$c - 1]
            $tr.Font.Size = $(if ($c -ge 4) { 7.5 } else { 8.5 })
        }
    }
    for ($r = 1; $r -le $rows; $r++) {
        for ($c = 1; $c -le 8; $c++) {
            $cf = $tbl.Cell($r, $c).Shape.TextFrame
            $cf.MarginTop = 1; $cf.MarginBottom = 1; $cf.MarginLeft = 3; $cf.MarginRight = 2
        }
    }
    $tbl.Rows.Item(1).Height = 16
    for ($r = 2; $r -le $rows; $r++) { $tbl.Rows.Item($r).Height = 12 }
    Add-Legend $sl
}

# ══ 뒤에서부터 처리해야 인덱스가 흔들리지 않는다 ══════════════════════
for ($i = $CH.Count - 1; $i -ge 0; $i--) {
    $c   = $CH[$i]
    $sc  = Get-Scen $c.Id
    $secs = Split-Sections $sc.steps
    $labels = @("① ", "② ", "③ ", "④ ")

    # ── 병기 4장 (Tbl ~ Tbl+3) : 뒤에서부터 교체
    for ($k = 3; $k -ge 0; $k--) {
        $idx = [int]$c.Tbl + $k
        $eqn = switch ($secs[$k].Equip) { "CV" { "C/V" } "SC" { "S/C" } "RGV" { "RGV" } default { $secs[$k].Equip } }
        $no  = 1
        for ($j = 0; $j -lt $k; $j++) { $no += $secs[$j].Steps.Count }
        $sl = New-At $idx ("[호기 병기] " + $c.Name + "  " + $labels[$k] + $eqn + " 구간") 18
        Fill-Table $sl $secs[$k].Steps $no `
            ("앞 다이어그램은 S/C #1 기준이다. 같은 단계를 2~5호기로 옮기면 아래 주소가 된다.   (구간 " + ($k + 1) + "/" + $secs.Count + ")")
        Write-Output ("  병기 " + $idx + " : " + $c.Name + " " + $labels[$k] + $eqn)
    }

    # ── 개요 1장
    $sl = New-At ([int]$c.Ov) ("반송 시나리오 " + $sc.title) 17
    $lines = New-Object System.Collections.ArrayList
    [void]$lines.Add("경로(호기별) — PlcAddressMap.xml <CraneMap> 기준")
    foreach ($cr in $data.cranes.PSObject.Properties) {
        $v = $cr.Value
        if ($c.Dir -eq "입고") {
            [void]$lines.Add(("  S/C #{0} : 입고통로 C/V #{1}   RGV측 #{2} → S/C측 #{3}" -f $v.no, $v.inCv, $v.inRgvPort, $v.inScPort))
        } else {
            [void]$lines.Add(("  S/C #{0} : 출고통로 C/V #{1}   S/C측 #{2} → RGV측 #{3}" -f $v.no, $v.outCv, $v.outScPort, $v.outRgvPort))
        }
    }
    [void]$lines.Add("")
    [void]$lines.Add("구간 구성 (이어지는 슬라이드)")
    for ($k = 0; $k -lt $secs.Count; $k++) {
        $eqn = switch ($secs[$k].Equip) { "CV" { "C/V" } "SC" { "S/C" } "RGV" { "RGV" } default { $secs[$k].Equip } }
        [void]$lines.Add(("  {0}{1}  ({2} 스텝)   … 다이어그램 {3}·{4}   호기 병기 {5}" -f `
            $labels[$k], $eqn, $secs[$k].Steps.Count, ([int]$c.Ov + 1 + $k*2), ([int]$c.Ov + 2 + $k*2), ([int]$c.Tbl + $k)))
    }
    [void]$lines.Add("")
    [void]$lines.Add("※ 모든 주소는 PlcAddressMap.xml 에서 계산되었다. XML 을 고치면 EQP_TASK · EQP_SIM · 본 문서가 함께 바뀐다.")
    [void]$lines.Add("   주소식 :  base = origin + (설비번호 - numberFrom) × stride ,  주소 = base + offset")
    $tb = $sl.Shapes.AddTextbox(1, 30, 62, $W - 60, 330)
    $tb.TextFrame.TextRange.Text = ($lines -join "`r")
    $tr = $tb.TextFrame.TextRange
    $tr.Font.Size = 11; $tr.Font.Bold = 0; $tr.Font.Color.RGB = 0x000000
    $tr.ParagraphFormat.Alignment = 1
    Write-Output ("개요 " + $c.Ov + " : " + $sc.title)
}

# ══ 목차 (2번) ═══════════════════════════════════════════════════════
$sl = New-At 2 "목  차" 24
$toc = New-Object System.Collections.ArrayList
[void]$toc.Add("각 시나리오는 개요 1장 + 구간 다이어그램 8장 + 호기 병기 4장 = 13장으로 구성된다.")
[void]$toc.Add("")
foreach ($c in $CH) {
    $sc = Get-Scen $c.Id
    [void]$toc.Add(("{0,-3}  {1,-46}  슬라이드 {2} ~ {3}" -f $c.Id, $sc.title, $c.Ov, ([int]$c.Ov + 12)))
}
[void]$toc.Add("")
[void]$toc.Add("공통 규약")
[void]$toc.Add("   좌측 EQP = 설비(PLC) 측 신호   ·   우측 ECS = WCS 측 응답/지시")
[void]$toc.Add("   %MX = M 비트 절대주소   ·   %DW = D 워드(%DB = 워드×2)   ·   %RB = R 바이트")
[void]$toc.Add("   호기 병기 슬라이드는 같은 단계를 S/C #1~#5 로 옮겼을 때의 주소를 전부 나열한다.")
$tb = $sl.Shapes.AddTextbox(1, 40, 76, $W - 80, 380)
$tb.TextFrame.TextRange.Text = ($toc -join "`r")
$tr = $tb.TextFrame.TextRange
$tr.Font.Size = 12; $tr.Font.Name = "맑은 고딕"; $tr.Font.Color.RGB = 0x000000
$tr.ParagraphFormat.Alignment = 1

# ══ 표지 (1번) ═══════════════════════════════════════════════════════
$sl = New-At 1 "" 20
$sl.Shapes.Item(1).Delete()
$t1 = $sl.Shapes.AddTextbox(1, 40, 150, $W - 80, 60)
$t1.TextFrame.TextRange.Text = "LG 생명과학 물류창고 시스템"
$t1.TextFrame.TextRange.Font.Size = 20
$t1.TextFrame.TextRange.ParagraphFormat.Alignment = 2
$t2 = $sl.Shapes.AddTextbox(1, 40, 200, $W - 80, 80)
$t2.TextFrame.TextRange.Text = "반송 시나리오 6종"
$t2.TextFrame.TextRange.Font.Size = 40
$t2.TextFrame.TextRange.Font.Bold = -1
$t2.TextFrame.TextRange.Font.Color.RGB = 0x8B0000
$t2.TextFrame.TextRange.ParagraphFormat.Alignment = 2
$t3 = $sl.Shapes.AddTextbox(1, 40, 290, $W - 80, 120)
$t3.TextFrame.TextRange.Text = "입출고대 #22 입출고  ·  26 입고대 / 24 출고대  ·  30 입고대 / 29 피킹대" + [char]13 + `
    "S/C #1 기준 다이어그램 + S/C #1~#5 호기 병기" + [char]13 + [char]13 + `
    "V2.0    2026-08-20"
$t3.TextFrame.TextRange.Font.Size = 13
$t3.TextFrame.TextRange.ParagraphFormat.Alignment = 2

$pres.Save()
Write-Output ("total: " + $pres.Slides.Count)
$pres.Close()
$ppt.Quit()
Write-Output "done"
