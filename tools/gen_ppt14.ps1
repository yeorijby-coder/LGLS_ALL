param(
    [string]$Src  = "D:\project\LGLS\Renewal\ECS-분석-PLC사양및시나리오_LG생명과학_V1.3_20260819.pptx",
    [string]$Dst  = "D:\project\LGLS\Renewal\ECS-분석-PLC사양및시나리오_LG생명과학_V1.4_20260819.pptx",
    [string]$Json = "scen.json"
)
$ErrorActionPreference = "Stop"
$data = Get-Content -Raw -Encoding UTF8 $Json | ConvertFrom-Json

# ── 구간 분류 : 한 시나리오의 스텝을 ①입출고대C/V ②RGV ③통로C/V ④S/C 로 나눈다 ──
#    출고(2/3-2/4-2)는 순서가 ①S/C ②통로C/V ③RGV ④입출고대C/V 이므로
#    "등장 순서" 기준으로 구간을 끊는다(설비종류+번호가 바뀌는 지점).
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
    # 같은 설비가 연속으로 다시 나오면(예: C/V→RGV→C/V) 구간으로 유지한다.
    return $secs
}

function Is-Common($steps) {
    foreach ($s in $steps) {
        if ($s.per.'1'.addr -ne $s.per.'5'.addr) { return $false }
    }
    return $true
}

function Bit-Label([int]$a) { "M{0:000}.{1} %MX{2}" -f [int]($a/16), ($a % 16), $a }

function Cell-Text($step, $cr) {
    $p = $step.per."$cr"
    if ($p.addr -lt 0) { return "-" }
    $eq = switch ($step.equip) { "CV" { "C/V #" + $p.no } "SC" { "S/C #" + $p.no } "RGV" { "RGV #" + $p.no } default { "" } }
    $ad = switch ($p.dev) {
        "M" { "%MX" + $p.addr }
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

$ppt = New-Object -ComObject PowerPoint.Application
$pres = $ppt.Presentations.Open($Src, $false, $false, $false)
Write-Output ("opened: " + $pres.Slides.Count + " slides")

$W = $pres.PageSetup.SlideWidth
$H = $pres.PageSetup.SlideHeight

function Add-TableSlide($idx, $title, $subtitle, $steps, $startNo) {
    $sl = $pres.Slides.Add($idx, 11)   # 11 = ppLayoutTitleOnly
    $sl.Shapes.Item(1).TextFrame.TextRange.Text = $title
    $sl.Shapes.Item(1).TextFrame.TextRange.Font.Size = 20
    $sl.Shapes.Item(1).Top = 12; $sl.Shapes.Item(1).Height = 40

    $note = $(if (Is-Common $steps) {
        "   ▷ 이 구간은 입출고대/RGV 공용 설비라 1~5호기 주소가 모두 같다."
    } else {
        "   ▷ 이 구간은 호기마다 설비번호가 달라 주소가 모두 다르다."
    })
    $sub = $sl.Shapes.AddTextbox(1, 24, 48, $W - 48, 26)
    $sub.TextFrame.WordWrap = 0
    $sub.TextFrame.TextRange.Text = ($subtitle + "`r" + $note.Trim())
    $sub.TextFrame.TextRange.Font.Size = 9
    $sub.TextFrame.TextRange.Font.Color.RGB = 0x993300

    $rows = $steps.Count + 1
    $tbl = $sl.Shapes.AddTable($rows, 8, 24, 80, $W - 48, 18 * $rows).Table
    $hdr = @("#", "주체", "신호 / 설명", "1호기", "2호기", "3호기", "4호기", "5호기")
    $wid = @(28, 44, 239, 87, 87, 87, 87, 87)   # 합계 746 = 슬라이드폭 795 - 여백 48
    for ($c = 1; $c -le 8; $c++) {
        $tbl.Columns.Item($c).Width = $wid[$c - 1]
        $tr = $tbl.Cell(1, $c).Shape.TextFrame.TextRange
        $tr.Text = $hdr[$c - 1]; $tr.Font.Size = 10; $tr.Font.Bold = -1
    }
    for ($r = 0; $r -lt $steps.Count; $r++) {
        $s = $steps[$r]
        $vals = @(
            ($startNo + $r).ToString(),
            (Kind-Text $s.kind $s.value),
            $s.desc,
            (Cell-Text $s 1), (Cell-Text $s 2), (Cell-Text $s 3), (Cell-Text $s 4), (Cell-Text $s 5)
        )
        for ($c = 1; $c -le 8; $c++) {
            $tr = $tbl.Cell($r + 2, $c).Shape.TextFrame.TextRange
            $tr.Text = $vals[$c - 1]
            $tr.Font.Size = $(if ($c -ge 4 -and $c -le 8) { 8 } else { 9 })
        }
        if ($s.kind -eq "force") { $tbl.Rows.Item($r + 2).Cells.Item(2).Shape.Fill.ForeColor.RGB = 0xE0F0D0 }
    }
    for ($r = 1; $r -le $rows; $r++) {
        for ($c = 1; $c -le 8; $c++) {
            $cf = $tbl.Cell($r, $c).Shape.TextFrame
            $cf.MarginTop = 1; $cf.MarginBottom = 1; $cf.MarginLeft = 3; $cf.MarginRight = 2
        }
    }
    $tbl.Rows.Item(1).Height = 16
    for ($r = 2; $r -le $rows; $r++) { $tbl.Rows.Item($r).Height = 12 }
    return $sl
}

function Add-OverviewSlide($idx, $sc, $secs) {
    $sl = $pres.Slides.Add($idx, 11)
    $sl.Shapes.Item(1).TextFrame.TextRange.Text = "반송 시나리오 " + $sc.title
    $sl.Shapes.Item(1).TextFrame.TextRange.Font.Size = 17
    $sl.Shapes.Item(1).Top = 10; $sl.Shapes.Item(1).Height = 40

    $lines = New-Object System.Collections.ArrayList
    [void]$lines.Add("경로(호기별) — PlcAddressMap.xml <CraneMap> 기준")
    foreach ($c in $data.cranes.PSObject.Properties) {
        $v = $c.Value
        [void]$lines.Add(("  S/C #{0} : 입고통로 C/V #{1} (RGV측 #{2} → S/C측 #{3})   ·   출고통로 C/V #{4} (S/C측 #{5} → RGV측 #{6})" -f `
            $v.no, $v.inCv, $v.inRgvPort, $v.inScPort, $v.outCv, $v.outScPort, $v.outRgvPort))
    }
    [void]$lines.Add("")
    [void]$lines.Add("구간 구성 (이어지는 슬라이드)")
    $k = 1
    foreach ($sec in $secs) {
        $eqn = switch ($sec.Equip) { "CV" { "C/V" } "SC" { "S/C" } "RGV" { "RGV" } default { $sec.Equip } }
        [void]$lines.Add(("  구간 {0} : {1}  ({2} 스텝)" -f $k, $eqn, $sec.Steps.Count))
        $k++
    }
    [void]$lines.Add("")
    [void]$lines.Add("※ 이 장의 모든 주소는 PlcAddressMap.xml 에서 계산되었다. XML 을 고치면 EQP_TASK · EQP_SIM · 본 문서가 함께 바뀐다.")
    [void]$lines.Add("   주소식 :  base = origin + (설비번호 - numberFrom) × stride ,  주소 = base + offset")
    [void]$lines.Add("   %MX=M비트 절대주소 · %DW=D워드(%DB=워드×2) · %RB=R바이트(=워드×2, 문서표기 16진 해석)")

    $tb = $sl.Shapes.AddTextbox(1, 30, 66, $W - 60, 320)
    $tb.TextFrame.TextRange.Text = ($lines -join "`r")
    $tr = $tb.TextFrame.TextRange
    $tr.Font.Size = 11
    $tr.Font.Bold = 0
    $tr.Font.Color.RGB = 0x000000
    $tr.ParagraphFormat.Alignment = 1     # ppAlignLeft
    return $sl
}

# ── 1) 시나리오 1(입고, 슬라이드 20~28 대응) : 구간별 호기 병기표를 뒤에 붙인다 ──
$scen = @{}
foreach ($s in $data.scenarios) { $scen[$s.id] = $s }

$insertAt = 29     # 슬라이드 28 다음
$added = 0

$s1 = $scen["1"]
$secs1 = Split-Sections $s1.steps
$labels1 = @("① 입출고대 C/V", "② RGV 반송", "③ 통로 C/V", "④ S/C 반송")
$no = 1
for ($i = 0; $i -lt $secs1.Count; $i++) {
    $lab = $(if ($i -lt $labels1.Count) { $labels1[$i] } else { "구간 " + ($i + 1) })
    Add-TableSlide $insertAt ("[호기 병기] 입고작업 " + $lab + "  (슬라이드 20~28)") `
        ("슬라이드 20~28 은 S/C #1 기준. 같은 단계를 2~5호기로 옮기면 아래 주소가 된다.  (구간 " + ($i + 1) + "/" + $secs1.Count + ")") `
        $secs1[$i].Steps $no | Out-Null
    $no += $secs1[$i].Steps.Count
    $insertAt++; $added++
}

# ── 2) 나머지 시나리오(2, 3-1, 3-2, 4-1, 4-2) : 개요 + 구간별 표 ──
foreach ($id in @("2", "3-1", "3-2", "4-1", "4-2")) {
    $sc = $scen[$id]
    $secs = Split-Sections $sc.steps
    Add-OverviewSlide $insertAt $sc $secs | Out-Null
    $insertAt++; $added++
    $no = 1
    for ($i = 0; $i -lt $secs.Count; $i++) {
        $eqn = switch ($secs[$i].Equip) { "CV" { "C/V" } "SC" { "S/C" } "RGV" { "RGV" } default { $secs[$i].Equip } }
        Add-TableSlide $insertAt ("반송 시나리오 " + $sc.id + "  —  구간 " + ($i + 1) + "/" + $secs.Count + " (" + $eqn + ")") `
            ($sc.title + "     [ EQP▲=설비 ON · EQP▼=설비 OFF · ECS▲=WCS 가 ON · ECS 값=WCS 가 D/R 기록 ]") `
            $secs[$i].Steps $no | Out-Null
        $no += $secs[$i].Steps.Count
        $insertAt++; $added++
    }
}

Write-Output ("added: " + $added + "  total: " + $pres.Slides.Count)
$pres.SaveAs($Dst)
$pres.Close()
$ppt.Quit()
Write-Output ("saved: " + $Dst)
