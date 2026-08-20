param(
    [string]$Src = "D:\project\LGLS\Renewal\ECS-분석-PLC사양및시나리오_LG생명과학_V1.4_20260819.pptx",
    [string]$Dst = "D:\project\LGLS\Renewal\ECS-반송시나리오6종_LG생명과학_V2.0_20260820.pptx"
)
$ErrorActionPreference = "Stop"

# ── 새 덱의 슬라이드 구성 ───────────────────────────────────────────────
#   각 원소 = V1.4 원본 슬라이드 번호 (0 = 나중에 생성할 자리표시자)
#   챕터 = 개요1 + 다이어그램8 + 병기4 = 13장
#
#   A 입출고대 #22 입고 : 20 | 21 22 23 24 25 26 27 28 | 29 30 31 32
#   B 입출고대 #22 출고 : ●  | 27 28 25 26 23 24 10 11 | ● ● ● ●
#   C 26 입고           : A 복제 (C/V#11 → #13)
#   D 24 출고           : B 복제 (C/V#11 → #12)
#   E 30 입고           : A 복제, 단 ①구간은 14 15 (C/V#15 피킹존 입고 손그림)
#   F 29 출고           : B 복제, 단 ④구간은 12 13 (C/V#14 피킹존 출고 손그림)

$PLAN = @(
    @{ Ch = "표지"; Src = 0 },
    @{ Ch = "목차"; Src = 0 },

    @{ Ch = "A"; Src = 20 }, @{ Ch = "A"; Src = 21 }, @{ Ch = "A"; Src = 22 },
    @{ Ch = "A"; Src = 23 }, @{ Ch = "A"; Src = 24 }, @{ Ch = "A"; Src = 25 },
    @{ Ch = "A"; Src = 26 }, @{ Ch = "A"; Src = 27 }, @{ Ch = "A"; Src = 28 },
    @{ Ch = "A"; Src = 29 }, @{ Ch = "A"; Src = 30 }, @{ Ch = "A"; Src = 31 }, @{ Ch = "A"; Src = 32 },

    @{ Ch = "B"; Src = 0 },
    @{ Ch = "B"; Src = 27 }, @{ Ch = "B"; Src = 28 }, @{ Ch = "B"; Src = 25 }, @{ Ch = "B"; Src = 26 },
    @{ Ch = "B"; Src = 23 }, @{ Ch = "B"; Src = 24 }, @{ Ch = "B"; Src = 10 }, @{ Ch = "B"; Src = 11 },
    @{ Ch = "B"; Src = 29 }, @{ Ch = "B"; Src = 30 }, @{ Ch = "B"; Src = 31 }, @{ Ch = "B"; Src = 32 },

    @{ Ch = "C"; Src = 20 }, @{ Ch = "C"; Src = 21 }, @{ Ch = "C"; Src = 22 },
    @{ Ch = "C"; Src = 23 }, @{ Ch = "C"; Src = 24 }, @{ Ch = "C"; Src = 25 },
    @{ Ch = "C"; Src = 26 }, @{ Ch = "C"; Src = 27 }, @{ Ch = "C"; Src = 28 },
    @{ Ch = "C"; Src = 29 }, @{ Ch = "C"; Src = 30 }, @{ Ch = "C"; Src = 31 }, @{ Ch = "C"; Src = 32 },

    @{ Ch = "D"; Src = 0 },
    @{ Ch = "D"; Src = 27 }, @{ Ch = "D"; Src = 28 }, @{ Ch = "D"; Src = 25 }, @{ Ch = "D"; Src = 26 },
    @{ Ch = "D"; Src = 23 }, @{ Ch = "D"; Src = 24 }, @{ Ch = "D"; Src = 10 }, @{ Ch = "D"; Src = 11 },
    @{ Ch = "D"; Src = 29 }, @{ Ch = "D"; Src = 30 }, @{ Ch = "D"; Src = 31 }, @{ Ch = "D"; Src = 32 },

    @{ Ch = "E"; Src = 20 }, @{ Ch = "E"; Src = 14 }, @{ Ch = "E"; Src = 15 },
    @{ Ch = "E"; Src = 23 }, @{ Ch = "E"; Src = 24 }, @{ Ch = "E"; Src = 25 },
    @{ Ch = "E"; Src = 26 }, @{ Ch = "E"; Src = 27 }, @{ Ch = "E"; Src = 28 },
    @{ Ch = "E"; Src = 29 }, @{ Ch = "E"; Src = 30 }, @{ Ch = "E"; Src = 31 }, @{ Ch = "E"; Src = 32 },

    @{ Ch = "F"; Src = 0 },
    @{ Ch = "F"; Src = 27 }, @{ Ch = "F"; Src = 28 }, @{ Ch = "F"; Src = 25 }, @{ Ch = "F"; Src = 26 },
    @{ Ch = "F"; Src = 23 }, @{ Ch = "F"; Src = 24 }, @{ Ch = "F"; Src = 12 }, @{ Ch = "F"; Src = 13 },
    @{ Ch = "F"; Src = 29 }, @{ Ch = "F"; Src = 30 }, @{ Ch = "F"; Src = 31 }, @{ Ch = "F"; Src = 32 }
)

Write-Output ("plan slides: " + $PLAN.Count)

$ppt = New-Object -ComObject PowerPoint.Application
$pres = $ppt.Presentations.Add()

# V1.4 의 디자인을 그대로 쓰기 위해 첫 슬라이드를 원본에서 가져온 뒤 빈 슬라이드를 지운다
$pres.Slides.InsertFromFile($Src, 0, 1, 1) | Out-Null      # 표지 자리 (원본 1번 = 표지 서식)
if ($pres.Slides.Count -gt 1) { $pres.Slides.Item(1).Delete() }   # Add() 가 만든 빈 슬라이드 제거

$idx = 1   # 현재까지 삽입된 슬라이드 수 (표지 자리 1장)
for ($i = 1; $i -lt $PLAN.Count; $i++) {
    $s = [int]$PLAN[$i].Src
    if ($s -eq 0) {
        # 자리표시자 : 제목만 있는 빈 슬라이드 (나중에 개요/목차로 채운다)
        $sl = $pres.Slides.Add($idx + 1, 11)
        $sl.Shapes.Item(1).TextFrame.TextRange.Text = "(자리표시자) " + $PLAN[$i].Ch
    }
    else {
        $pres.Slides.InsertFromFile($Src, $idx, $s, $s) | Out-Null
    }
    $idx++
    if ($idx % 20 -eq 0) { Write-Output ("  ... " + $idx) }
}

Write-Output ("assembled: " + $pres.Slides.Count)
$pres.SaveAs($Dst)
$pres.Close()
$ppt.Quit()
Write-Output ("saved: " + $Dst)
