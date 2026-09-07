# [LGLS 2026-09-07] WCS 전 프로그램 기동 (기동 순서 고정)
#
#   EQP_SIM -> WCS_TASK_CV -> IO_TASK -> HOST_SIM -> WCS_TASK_HOST -> 운전 화면
#   현장(-Site)에서는 시뮬레이터 2종을 띄우지 않는다.
#
#   사용법
#     .\Start-All.ps1            시뮬 포함 전체 기동
#     .\Start-All.ps1 -Site      현장용 (EQP_SIM / HOST_SIM 제외)
#     .\Start-All.ps1 -NoClient  운전 화면은 띄우지 않음 (서버용)
#     .\Start-All.ps1 -Stop      기동 역순으로 전부 종료
param([switch]$Site, [switch]$NoClient, [switch]$Stop)

$ErrorActionPreference = 'Stop'
$EXE = Split-Path -Parent $MyInvocation.MyCommand.Path

# 이름 / 실행파일 / 대기(초)
$PROGS = @(
    @{ n='EQP_SIM';      exe="$EXE\SIM\EQP_SIM\EQP_SIM.exe";                         wait=6; sim=$true  },
    @{ n='WCS_TASK_CV';  exe="$EXE\TASK\WCS_TASK_CV\WCS_TASK_CV.exe";                wait=7; sim=$false },
    @{ n='IO_TASK';      exe="$EXE\TASK\IO_TASK\IO_TASK_SEMI_FINISH.exe";            wait=6; sim=$false },
    @{ n='HOST_SIM';     exe="$EXE\SIM\HOST_SIM\HOST_SIM.exe";                       wait=5; sim=$true  },
    @{ n='WCS_TASK_HOST';exe="$EXE\TASK\WCS_TASK_HOST\TASK_LFC10_G1_ECSCOM.exe";     wait=8; sim=$false },
    @{ n='WCS_CLIENT';   exe="$EXE\WCS_CLIENT\Ecs.exe";                              wait=3; sim=$false; client=$true }
)

if ($Stop) {
    Write-Host "[종료] 기동 역순으로 내린다"
    foreach ($p in ($PROGS | Sort-Object { $PROGS.IndexOf($_) } -Descending)) {
        $procName = [IO.Path]::GetFileNameWithoutExtension($p.exe)
        $ps = Get-Process $procName -ErrorAction SilentlyContinue
        if (-not $ps) { continue }
        foreach ($q in $ps) { [void]$q.CloseMainWindow() }
        Start-Sleep -Seconds 2
        Get-Process $procName -ErrorAction SilentlyContinue | Stop-Process -Force
        Write-Host ("  {0,-16} 종료" -f $p.n)
    }
    return
}

Write-Host ("[기동] {0} 모드" -f $(if ($Site) { '현장 (시뮬 제외)' } else { '시뮬 포함' }))
foreach ($p in $PROGS) {
    if ($Site -and $p.sim)      { Write-Host ("  {0,-16} 건너뜀 (현장 모드)" -f $p.n); continue }
    if ($NoClient -and $p.client) { Write-Host ("  {0,-16} 건너뜀 (-NoClient)" -f $p.n); continue }
    if (-not (Test-Path $p.exe)) { Write-Host ("  {0,-16} ★없음★ {1}" -f $p.n, $p.exe); continue }

    $procName = [IO.Path]::GetFileNameWithoutExtension($p.exe)
    if (Get-Process $procName -ErrorAction SilentlyContinue) {
        Write-Host ("  {0,-16} 이미 실행 중" -f $p.n); continue
    }
    Start-Process -FilePath $p.exe -WorkingDirectory (Split-Path $p.exe -Parent)
    Write-Host ("  {0,-16} 기동" -f $p.n)
    Start-Sleep -Seconds $p.wait
}

Write-Host ""
Write-Host "실행 중 :"
Get-Process | Where-Object { $_.ProcessName -match 'EQP_SIM|HOST_SIM|WCS_TASK_CV|IO_TASK|TASK_LFC10|Ecs' } |
    Select-Object ProcessName, Id | Format-Table -AutoSize

if (-not $Site) {
    Write-Host "※ HOST_SIM 은 첫 상태보고(S)를 받은 뒤 [시작] 을 눌러야 작업이 나간다."
}
