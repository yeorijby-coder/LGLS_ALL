# [LGLS] 운전 화면(Ecs.exe)과 똑같은 방식으로 DB 접속을 시험한다
#   Ecs.exe 는 32비트 + ADO + ODBC 다. 64비트 도구로 되는지 봐야 소용없다.
#   그래서 이 스크립트는 스스로 32비트 PowerShell 로 다시 뜬다.
#
#   .\Test-EcsDb.ps1                       옆의 Ecs.ini 를 읽어 시험
#   .\Test-EcsDb.ps1 -Ini C:\WCS\Ecs.ini
#   .\Test-EcsDb.ps1 -Server "192.168.0.10,1433" -Driver "ODBC Driver 17 for SQL Server"
param(
    [string]$Ini,
    [string]$Driver,
    [string]$Server,
    [string]$User,
    [string]$Password,
    [string]$Database
)

# ── 64비트에서 실행됐으면 32비트로 다시 띄운다 ─────────────────────
if ([IntPtr]::Size -eq 8) {
    $ps32 = "$env:WINDIR\SysWOW64\WindowsPowerShell\v1.0\powershell.exe"
    if (Test-Path $ps32) {
        Write-Host "(32비트로 다시 실행)"
        $a = @('-NoProfile','-ExecutionPolicy','Bypass','-File',$MyInvocation.MyCommand.Path)
        foreach ($k in $PSBoundParameters.Keys) { $a += "-$k"; $a += $PSBoundParameters[$k] }
        & $ps32 $a
        return
    }
}

$here = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not $Ini) { $Ini = Join-Path $here 'Ecs.ini' }

function IniGet([string]$path, [string]$sec, [string]$key) {
    if (-not (Test-Path $path)) { return $null }
    $enc = [Text.Encoding]::GetEncoding(949)
    $inSec = $false
    foreach ($ln in [IO.File]::ReadAllLines($path, $enc)) {
        $t = $ln.Trim()
        if ($t.StartsWith('--') -or $t.StartsWith(';') -or $t.StartsWith('#')) { continue }
        if ($t.StartsWith('[')) { $inSec = $t.ToUpper().StartsWith("[$sec]".ToUpper()); continue }
        if ($inSec -and $t -match "^\s*$key\s*=\s*(.+?)\s*$") { return $Matches[1] }
    }
    return $null
}

if (Test-Path $Ini) { Write-Host "INI : $Ini" } else { Write-Host "INI 없음 : $Ini" }
if (-not $Driver)   { $Driver   = IniGet $Ini 'DB_2' 'DRIVER' }
if (-not $Server)   { $Server   = IniGet $Ini 'DB_2' 'SERVER' }
if (-not $User)     { $User     = IniGet $Ini 'DB_2' 'USERID' }
if (-not $Password) { $Password = IniGet $Ini 'DB_2' 'USERPASSWORD' }
if (-not $Database) { $Database = IniGet $Ini 'DB_2' 'DATABASE' }

$cs = "Driver={$Driver};Server=$Server;uid=$User;pwd=$Password;Database=$Database"
Write-Host "연결 문자열 : $($cs -replace 'pwd=[^;]*','pwd=***')"
Write-Host ""

# ── 설치된 32비트 ODBC 드라이버 목록 ────────────────────────────
Write-Host "[이 PC 의 32비트 ODBC 드라이버]"
$k = 'HKLM:\SOFTWARE\WOW6432Node\ODBC\ODBCINST.INI\ODBC Drivers'
if (Test-Path $k) {
    (Get-Item $k).GetValueNames() | Sort-Object | ForEach-Object {
        $mark = if ($_ -eq $Driver) { ' <== INI 지정' } else { '' }
        Write-Host "  $_$mark"
    }
} else { Write-Host "  (목록을 읽지 못함)" }
Write-Host ""

# ── 이 PC 에 실제로 있는 SQL Server 인스턴스 ────────────────────
Write-Host "[이 PC 의 SQL Server 인스턴스]"
$rk = [Microsoft.Win32.RegistryKey]::OpenBaseKey('LocalMachine','Registry64')
$sub = $rk.OpenSubKey('SOFTWARE\Microsoft\Microsoft SQL Server\Instance Names\SQL')
if ($sub) {
    foreach ($nm in $sub.GetValueNames()) {
        $target = if ($nm -eq 'MSSQLSERVER') { 'localhost        (기본 인스턴스 - 인스턴스명을 쓰지 않는다)' } else { "localhost\$nm" }
        Write-Host ("  {0,-16} -> SERVER={1}" -f $nm, $target)
        # 이 인스턴스가 실제로 듣고 있는 TCP 포트
        $pk = $rk.OpenSubKey("SOFTWARE\Microsoft\Microsoft SQL Server\$($sub.GetValue($nm))\MSSQLServer\SuperSocketNetLib\Tcp\IPAll")
        if ($pk) {
            $fixed = $pk.GetValue('TcpPort'); $dyn = $pk.GetValue('TcpDynamicPorts')
            if ($fixed) { Write-Host "      TCP 고정 포트 = $fixed  (SERVER=localhost,$fixed 로 쓸 수 있다)" }
            elseif ($dyn) { Write-Host "      ★TCP 동적 포트 = $dyn★  고정 1433 이 아니다 - localhost,1433 은 실패한다" }
            else { Write-Host "      ★TCP 포트 설정 없음★ (TCP/IP 미사용일 수 있다)" }
        }
    }
} else { Write-Host "  없음 - 이 PC 에는 SQL Server 가 설치돼 있지 않다" }
Get-Service -Name 'MSSQL$*','MSSQLSERVER','SQLBrowser' -ErrorAction SilentlyContinue |
    ForEach-Object { Write-Host ("  서비스 {0,-24} {1}" -f $_.Name, $_.Status) }
Write-Host ""

# ── 서버 도달 여부 ──────────────────────────────────────────────
$hostOnly = ($Server -split '[\,]')[0]
$port = 1433
if ($Server -match ',\s*(\d+)') { $port = [int]$Matches[1] }
Write-Host "[TCP 확인] $hostOnly : $port"
try {
    $c = New-Object Net.Sockets.TcpClient
    $ar = $c.BeginConnect($hostOnly, $port, $null, $null)
    if ($ar.AsyncWaitHandle.WaitOne(3000) -and $c.Connected) { Write-Host "  열림" } else { Write-Host "  ★막힘/무응답★ (방화벽 또는 TCP 미사용)" }
    $c.Close()
} catch { Write-Host "  ★실패★ $($_.Exception.Message)" }
if ($Server.Contains([char]92)) { Write-Host "  ※ 명명된 인스턴스다. SQL Browser(UDP 1434)가 열려 있어야 한다. IP,1433 형식을 권한다." }
Write-Host ""

# ── 실제 접속 (Ecs.exe 와 동일한 ADO 경로) ──────────────────────
Write-Host "[ADO 접속 시도]"
try {
    $conn = New-Object -ComObject ADODB.Connection
    $conn.ConnectionTimeout = 10
    $conn.Open($cs)
    $rs = $conn.Execute("SELECT DB_NAME(), SUSER_NAME(), (SELECT COUNT(*) FROM cv_data)")
    Write-Host ("  성공 - DB={0} 로그인={1} cv_data={2} 건" -f $rs.Fields.Item(0).Value, $rs.Fields.Item(1).Value, $rs.Fields.Item(2).Value)
    $conn.Close()
} catch {
    Write-Host "  ★실패★"
    Write-Host "  $($_.Exception.Message)"
    if ($_.Exception.InnerException) { Write-Host "  $($_.Exception.InnerException.Message)" }
}
