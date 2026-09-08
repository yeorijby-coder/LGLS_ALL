# [LGLS 2026-09-08] 크레인/RGV 위치 관측값이 실시간으로 바뀌는지 본다.
#
#   화면의 크레인이 "뚝뚝 끊겨" 움직이는 원인이 우리 쪽인지 PLC 쪽인지 가른다.
#   설비 인터페이스에는 연속 위치 태그가 없고 SUBSYSTEM_LOCATION_01/02/03(뱅크/베이/단)
#   하나뿐이다(구 ECS 도 같다). PLC 가 이 값을 이동 중에 계속 올려 주면 실시간이 되고,
#   목적지에서 한 번만 올려 주면 화면은 뛸 수밖에 없다.
#
#   크레인이 움직이는 동안 돌려서 LOCATION_02(베이) 가 계속 변하는지 본다.
#     .\Watch-ScPos.ps1                     기본 60초, 0.5초 간격
#     .\Watch-ScPos.ps1 -Seconds 120 -Sc 1  1호기만 120초
param(
    [string]$Server   = 'localhost',
    [string]$Database = 'LGLS_MCS_IO',
    [int]$Seconds     = 60,
    [int]$IntervalMs  = 500,
    [string]$Sc       = ''
)

$where = "WH_TYP = '10'"
if ($Sc -ne '') { $where += " AND SC_NO = '$Sc'" }

$q = @"
SET NOCOUNT ON;
SELECT SC_NO,
       ISNULL(LOCATION_01_RD,'') AS L1,
       ISNULL(LOCATION_02_RD,'') AS L2,
       ISNULL(LOCATION_03_RD,'') AS L3,
       ISNULL(POS_H_RD,'')       AS POSH,
       ISNULL(SUBSYSTEM_STATUS_RD,'') AS ST
  FROM SC_DATA_LGLS WHERE $where ORDER BY SC_NO;
"@

Write-Host "[$Server / $Database] $Seconds 초 동안 $IntervalMs ms 간격으로 관측한다. 값이 바뀔 때만 찍는다."
Write-Host "  L1=뱅크 L2=베이 L3=단  POSH=화면 레일위치  ST=설비상태"
Write-Host ""

$prev = @{}
$chg  = 0
$tick = 0
$end  = (Get-Date).AddSeconds($Seconds)
while ((Get-Date) -lt $end) {
    $tick++
    $tmp = [IO.Path]::GetTempFileName()
    & sqlcmd -S $Server -d $Database -E -W -s '|' -h -1 -Q $q -o $tmp 2>&1 | Out-Null
    foreach ($ln in (Get-Content $tmp -Encoding Default)) {
        $a = $ln -split '\|'
        if ($a.Count -lt 6) { continue }
        $sc = $a[0].Trim()
        $cur = ($a[1..5] -join '|')
        if ($prev.ContainsKey($sc) -and $prev[$sc] -eq $cur) { continue }
        $prev[$sc] = $cur
        $chg++
        Write-Host ("{0}  S/C {1}  L1={2,-4} L2={3,-6} L3={4,-4} POSH={5,-6} ST={6}" -f `
            (Get-Date -Format 'HH:mm:ss.fff'), $sc, $a[1].Trim(), $a[2].Trim(), $a[3].Trim(), $a[4].Trim(), $a[5].Trim())
    }
    Remove-Item $tmp -Force -ErrorAction SilentlyContinue
    Start-Sleep -Milliseconds $IntervalMs
}

Write-Host ""
Write-Host "조회 $tick 회 / 값 변화 $chg 회"
Write-Host "L2(베이)가 이동 중에 계속 변했다면 -> PLC 는 실시간으로 준다. 우리 쪽 표시 로직을 고치면 된다."
Write-Host "L2 가 출발/도착에서만 변했다면   -> PLC 가 그때만 올려 준다. 화면이 뛰는 것은 인터페이스 한계다."
