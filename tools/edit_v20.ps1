param(
    [string]$Doc  = "D:\project\LGLS\Renewal\ECS-반송시나리오6종_LG생명과학_V2.0_20260820.pptx",
    [string]$Rules = "rules.json"
)
$ErrorActionPreference = "Stop"

# 규칙 파일 : [{ slide: n, subs: [[from,to],...] }, ...]
#   치환은 2단계(placeholder)로 수행해 연쇄 치환(#3→#4→#3)을 막는다.
$plan = Get-Content -Raw -Encoding UTF8 $Rules | ConvertFrom-Json

$ppt = New-Object -ComObject PowerPoint.Application
$pres = $ppt.Presentations.Open($Doc, $false, $false, $false)

$script:hit = 0
$script:miss = New-Object System.Collections.ArrayList

# 런(run) 단위 치환 : 서식을 보존한다. 주소·짧은 토큰은 대부분 한 런 안에 있다.
#   phrase=$true 인 규칙은 도형 전체 텍스트를 대상으로 한다(서식 단순화 감수).
function Sub-Runs($tr, $pairs) {
    if ($null -eq $tr) { return }
    $n = 0
    try { $n = $tr.Runs().Count } catch { return }
    for ($r = 1; $r -le $n; $r++) {
        $run = $tr.Runs($r)
        $t = $run.Text
        if ([string]::IsNullOrEmpty($t)) { continue }
        $orig = $t
        # 1단계 : 자리표시자
        for ($k = 0; $k -lt $pairs.Count; $k++) {
            $from = [string]$pairs[$k][0]
            if ($from.StartsWith("@@")) { continue }
            if ($from.StartsWith("==")) {
                if ($t.Trim() -eq $from.Substring(2)) {
                    $t = ([char]0x0001).ToString() + $k + ([char]0x0002).ToString()
                    $script:hit++
                }
                continue
            }
            if ($t.Contains($from)) {
                $t = $t.Replace($from, ([char]0x0001).ToString() + $k + ([char]0x0002).ToString())
                $script:hit++
            }
        }
        # 2단계 : 복원
        for ($k = 0; $k -lt $pairs.Count; $k++) {
            $ph = ([char]0x0001).ToString() + $k + ([char]0x0002).ToString()
            if ($t.Contains($ph)) { $t = $t.Replace($ph, [string]$pairs[$k][1]) }
        }
        if ($t -ne $orig) { $run.Text = $t }
    }
}

# 도형 전체 텍스트 대상 치환 (@@접두) : 런을 넘나드는 문장용. 서식은 단순화된다.
function Sub-Shape($tr, $pairs) {
    if ($null -eq $tr) { return }
    $t = $tr.Text
    if ([string]::IsNullOrEmpty($t)) { return }
    $orig = $t
    foreach ($pr in $pairs) {
        $from = [string]$pr[0]
        if (-not $from.StartsWith("@@")) { continue }
        $f = $from.Substring(2)
        if ($t.Contains($f)) { $t = $t.Replace($f, [string]$pr[1]); $script:hit++ }
    }
    if ($t -ne $orig) { $tr.Text = $t }
}

function Walk($shapes, $pairs) {
    foreach ($sh in $shapes) {
        if ($sh.Type -eq 6) { Walk $sh.GroupItems $pairs; continue }
        if ($sh.HasTable -eq -1) {
            $tb = $sh.Table
            for ($r = 1; $r -le $tb.Rows.Count; $r++) {
                for ($c = 1; $c -le $tb.Columns.Count; $c++) {
                    Sub-Runs $tb.Cell($r, $c).Shape.TextFrame.TextRange $pairs
                }
            }
            continue
        }
        if ($sh.HasTextFrame -eq -1) {
            if ($sh.TextFrame.HasText -eq -1) {
                Sub-Runs  $sh.TextFrame.TextRange $pairs
                Sub-Shape $sh.TextFrame.TextRange $pairs
            }
        }
    }
}

foreach ($item in $plan) {
    $n = [int]$item.slide
    $pairs = $item.subs
    $sl = $pres.Slides.Item($n)
    $before = $script:hit
    Walk $sl.Shapes $pairs
    $applied = $script:hit - $before
    Write-Output ("slide {0,3} : {1,3} 치환" -f $n, $applied)
    # 적용 0건인 규칙 경고
    if ($applied -eq 0) { [void]$script:miss.Add($n) }
}

if ($script:miss.Count -gt 0) { Write-Output ("[경고] 치환 0건 슬라이드: " + ($script:miss -join ", ")) }
Write-Output ("총 치환: " + $script:hit)

$pres.Save()
$pres.Close()
$ppt.Quit()
Write-Output "done"
