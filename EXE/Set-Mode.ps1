# [LGLS 2026-09-07] 접속 설정 전환 (시뮬레이터 <-> 현장)
#
#   바뀌는 것은 두 곳뿐이다.
#     · WCS_TASK_CV  WCS_DB.INI   [COMM0] IP   = PLC 주소
#     · WCS_TASK_HOST EcsComA.ini [Network]    = LocalPort / RemoteIP / RemotePort
#   DB 접속([DB] IP)은 이 스크립트가 건드리지 않는다 - 서버마다 달라 직접 적는다.
#
#   사용법
#     .\Set-Mode.ps1              현재 설정만 보여준다
#     .\Set-Mode.ps1 -Mode Sim    시뮬레이터 (127.0.0.1 / 9911 / 9910)
#     .\Set-Mode.ps1 -Mode Site   현장       (192.100.1.110 / 8001 / 192.100.1.191 / 8002)
param([ValidateSet('Sim','Site')][string]$Mode)

$ErrorActionPreference = 'Stop'
$EXE = Split-Path -Parent $MyInvocation.MyCommand.Path
$CV  = "$EXE\TASK\WCS_TASK_CV\WCS_DB.INI"
$HO  = "$EXE\TASK\WCS_TASK_HOST\EcsComA.ini"
$ENC = [Text.Encoding]::GetEncoding(949)      # ★ INI 는 CP949 - 인코딩을 반드시 지정한다

function Show([string]$tag) {
    Write-Host "[$tag]"
    if (Test-Path $CV) {
        $t = [IO.File]::ReadAllText($CV, $ENC)
        foreach ($m in [regex]::Matches($t, '(?m)^\s*IP\s*=\s*(\S+)')) {
            Write-Host ("  WCS_DB.INI     IP        = {0}" -f $m.Groups[1].Value) }
    }
    if (Test-Path $HO) {
        $t = [IO.File]::ReadAllText($HO, $ENC)
        foreach ($k in 'LocalPort','RemoteIP','RemotePort') {
            $m = [regex]::Match($t, "(?m)^\s*$k\s*=\s*(\S+)")
            if ($m.Success) { Write-Host ("  EcsComA.ini    {0,-10}= {1}" -f $k, $m.Groups[1].Value) }
        }
    }
}

if (-not $Mode) { Show '현재 설정'; return }

$plc  = if ($Mode -eq 'Sim') { '127.0.0.1' } else { '192.100.1.110' }
$hip  = if ($Mode -eq 'Sim') { '127.0.0.1' } else { '192.100.1.191' }
$lp   = if ($Mode -eq 'Sim') { '9911' }      else { '8001' }
$rp   = if ($Mode -eq 'Sim') { '9910' }      else { '8002' }

# ── WCS_DB.INI : [COMM0] 섹션의 IP 만 바꾼다([DB_2] IP 는 그대로 둔다) ──
if (Test-Path $CV) {
    Copy-Item $CV "$CV.bak_setmode" -Force
    $t = [IO.File]::ReadAllText($CV, $ENC)
    $i = $t.IndexOf('[COMM0]')
    if ($i -lt 0) { throw 'WCS_DB.INI 에 [COMM0] 섹션이 없다' }
    $head = $t.Substring(0, $i)
    $tail = [regex]::Replace($t.Substring($i), '(?m)^(\s*IP\s*=\s*)\S+', "`${1}$plc")
    [IO.File]::WriteAllText($CV, $head + $tail, $ENC)
    Write-Host "  WCS_DB.INI  [COMM0] IP -> $plc"
}

# ── EcsComA.ini : 통신 3항목 ─────────────────────────────────────
if (Test-Path $HO) {
    Copy-Item $HO "$HO.bak_setmode" -Force
    $t = [IO.File]::ReadAllText($HO, $ENC)
    $t = [regex]::Replace($t, '(?m)^(\s*LocalPort\s*=\s*)\S+',  "`${1}$lp")
    $t = [regex]::Replace($t, '(?m)^(\s*RemoteIP\s*=\s*)\S+',   "`${1}$hip")
    $t = [regex]::Replace($t, '(?m)^(\s*RemotePort\s*=\s*)\S+', "`${1}$rp")
    [IO.File]::WriteAllText($HO, $t, $ENC)
    Write-Host "  EcsComA.ini LocalPort/RemoteIP/RemotePort -> $lp / $hip / $rp"
}

Write-Host ""
Show "적용 결과 ($Mode)"
Write-Host "※ DB 접속은 각 INI 의 [DB]/[DB_2] 를 직접 확인하세요."
