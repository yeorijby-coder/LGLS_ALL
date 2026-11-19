# ============================================================
#  DB 접속 정보를 설정 파일에 적는다.
#  2026-11-19
#
#  Ecs.ini 를 기준으로 삼는다.
#    - 인자로 받은 주소가 있으면 Ecs.ini 에 먼저 적고
#    - 그 다음 Ecs.ini 의 [DB_2] 를 읽어 WmsDown.ini 에 그대로 옮긴다.
#  둘이 어긋날 일이 없다. Ecs.ini 만 고친 뒤 이것을 돌려도 된다.
#
#  ini 는 CP949 라 인코딩을 지켜 읽고 쓴다(Default = 시스템 ANSI).
# ============================================================
param(
    [Parameter(Mandatory=$true)][string]$InstDir,
    [string]$Server   = "",      # 비우면 Ecs.ini 의 값을 그대로 쓴다
    [string]$Database = "",
    [switch]$Quiet
)

$ErrorActionPreference = "Stop"

function Say([string]$msg) { if (-not $Quiet) { Write-Host $msg } }

# ── ini 한 줄 읽기 ───────────────────────────────────────────
function Get-IniValue {
    param([string]$Path, [string]$Section, [string]$Key)

    if (-not (Test-Path $Path)) { return $null }
    $inSec = $false
    foreach ($ln in @(Get-Content $Path -Encoding Default)) {
        if ($ln -match "^\s*\[(.+?)\]") {
            $inSec = ($matches[1].Trim() -eq $Section)
            continue
        }
        if (-not $inSec) { continue }
        if ($ln -match "^\s*[;-]") { continue }          # 주석은 건너뛴다
        if ($ln -match ("^\s*" + [regex]::Escape($Key) + "\s*=(.*)$")) {
            return $matches[1].Trim()
        }
    }
    return $null
}

# ── ini 한 줄 쓰기 ───────────────────────────────────────────
function Set-IniValue {
    param([string]$Path, [string]$Section, [string]$Key, [string]$Value)

    $name = Split-Path $Path -Leaf
    if (-not (Test-Path $Path)) {
        Say ("        [주의] " + $name + " 가 없어 건너뜁니다.")
        return $false
    }

    $lines = @(Get-Content $Path -Encoding Default)
    $inSec = $false
    $done  = $false
    $out   = New-Object System.Collections.ArrayList

    foreach ($ln in $lines) {
        if ($ln -match "^\s*\[(.+?)\]") {
            $inSec = ($matches[1].Trim() -eq $Section)
            [void]$out.Add($ln)
            continue
        }
        if ($inSec -and (-not $done) -and
            ($ln -notmatch "^\s*[;-]") -and
            ($ln -match ("^\s*" + [regex]::Escape($Key) + "\s*="))) {
            [void]$out.Add($Key + "=" + $Value)
            $done = $true
            continue
        }
        [void]$out.Add($ln)
    }

    if (-not $done) {
        Say ("        [주의] " + $name + " 의 [" + $Section + "] 에서 " + $Key + " 줄을 찾지 못했습니다.")
        return $false
    }

    Set-Content -Path $Path -Value $out -Encoding Default
    Say ("        " + $name + "  [" + $Section + "] " + $Key + "=" + $Value)
    return $true
}

$ecs  = Join-Path $InstDir "Ecs.ini"
$down = Join-Path $InstDir "WmsDown.ini"

if (-not (Test-Path $ecs)) {
    Write-Host "        [주의] Ecs.ini 가 없습니다 : $ecs"
    exit 1
}

# ── 1. 받은 주소를 Ecs.ini 에 적는다 ─────────────────────────
Say "        [ Ecs.ini ]"
if ($Server   -ne "") { [void](Set-IniValue -Path $ecs -Section "DB_2" -Key "SERVER"   -Value $Server) }
if ($Database -ne "") { [void](Set-IniValue -Path $ecs -Section "DB_2" -Key "DATABASE" -Value $Database) }

# ── 2. Ecs.ini 를 읽어 WmsDown.ini 로 옮긴다 ─────────────────
#      여기서부터가 기준이다. 사람이 Ecs.ini 를 고친 뒤 돌려도 된다.
$srv  = Get-IniValue -Path $ecs -Section "DB_2" -Key "SERVER"
$db   = Get-IniValue -Path $ecs -Section "DB_2" -Key "DATABASE"
$uid  = Get-IniValue -Path $ecs -Section "DB_2" -Key "USERID"
$pwd  = Get-IniValue -Path $ecs -Section "DB_2" -Key "USERPASSWORD"
$trst = Get-IniValue -Path $ecs -Section "DB_2" -Key "TRUSTED"

# Windows 인증이면 계정을 비운다 - 내려받기 쪽도 같은 규칙이다.
if ($trst -eq "1") {
    $uid = ""
    $pwd = ""
}
if ($uid -eq $null) { $uid = "" }
if ($pwd -eq $null) { $pwd = "" }

Say ""
Say "        [ WmsDown.ini ]  - Ecs.ini 의 값을 그대로 옮깁니다"
if ($srv -ne $null -and $srv -ne "") { [void](Set-IniValue -Path $down -Section "DB Server" -Key "SERVERNAME" -Value $srv) }
if ($db  -ne $null -and $db  -ne "") { [void](Set-IniValue -Path $down -Section "DB Server" -Key "DATABASE"   -Value $db) }
[void](Set-IniValue -Path $down -Section "DB Server" -Key "USERID"   -Value $uid)
[void](Set-IniValue -Path $down -Section "DB Server" -Key "PASSWORD" -Value $pwd)

if ($trst -eq "1") {
    Say ""
    Say "        Ecs.ini 가 TRUSTED=1 이라 Windows 인증입니다. 계정은 비웠습니다."
}

exit 0
