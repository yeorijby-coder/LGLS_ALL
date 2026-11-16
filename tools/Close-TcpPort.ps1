#requires -Version 5.1
<#
.SYNOPSIS
  지정한 포트에 남은 TCP 접속(기본 CLOSE_WAIT)을 보여 주고, -Close 를 주면 강제로 끊는다.

.DESCRIPTION
  HOST_TASK 를 껐는데도 9911 에 CLOSE_WAIT 가 남고 netstat 의 PID 는 있는데 프로세스가 없을 때 쓴다.
  소켓을 쥔 프로세스를 못 찾거나 끝낼 수 없을 때, 윈도우의 SetTcpEntry(MIB_TCP_STATE_DELETE_TCB) 로
  그 접속의 상태 정보(TCB)를 지운다. (Sysinternals TCPView 의 [Close Connection] 과 같은 동작)

  - 기본은 목록만 보여 준다(아무것도 끊지 않는다). -Close 를 줘야 끊는다.
  - 끊으려면 ★관리자 권한★ PowerShell 이어야 한다.
  - IPv4 접속만 다룬다. LISTENING 소켓은 끊을 수 없다(쥔 프로세스를 끝내야 한다).
  - 상대에게 FIN/RST 를 보내지 않고 이쪽 정보만 지운다. 소켓을 쥔 프로세스가 살아 있으면 그 소켓은 오류를 받는다.

.EXAMPLE
  .\Close-TcpPort.ps1 -Port 9911                  # 목록만
  .\Close-TcpPort.ps1 -Port 9911 -Close           # CLOSE_WAIT 를 끊는다 (관리자)
  .\Close-TcpPort.ps1 -Port 9911 -State CloseWait,Established -Close
#>
param(
    [Parameter(Mandatory = $true)][int]$Port,
    [string[]]$State = @('CloseWait'),
    [switch]$Close
)

Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class TcpKill
{
    [StructLayout(LayoutKind.Sequential)]
    public struct MIB_TCPROW
    {
        public uint dwState;
        public uint dwLocalAddr;
        public uint dwLocalPort;
        public uint dwRemoteAddr;
        public uint dwRemotePort;
    }
    [DllImport("iphlpapi.dll")]
    public static extern int SetTcpEntry(ref MIB_TCPROW pTcpRow);

    public static int Delete(string localIp, int localPort, string remoteIp, int remotePort)
    {
        MIB_TCPROW r = new MIB_TCPROW();
        r.dwState = 12;   // MIB_TCP_STATE_DELETE_TCB
        r.dwLocalAddr = BitConverter.ToUInt32(System.Net.IPAddress.Parse(localIp).GetAddressBytes(), 0);
        r.dwLocalPort = (uint)System.Net.IPAddress.HostToNetworkOrder((short)localPort) & 0xFFFF;
        r.dwRemoteAddr = BitConverter.ToUInt32(System.Net.IPAddress.Parse(remoteIp).GetAddressBytes(), 0);
        r.dwRemotePort = (uint)System.Net.IPAddress.HostToNetworkOrder((short)remotePort) & 0xFFFF;
        return SetTcpEntry(ref r);
    }
}
'@

$rows = @(Get-NetTCPConnection -ErrorAction SilentlyContinue |
    Where-Object { ($_.LocalPort -eq $Port -or $_.RemotePort -eq $Port) -and $_.LocalAddress -notmatch ':' })

if ($rows.Count -eq 0) { Write-Host "포트 $Port 에 관련된 접속이 없습니다."; return }

Write-Host ("{0,-8}{1,-22}{2,-22}{3,-12}{4,-7}{5}" -f '구분', '로컬', '원격', '상태', 'PID', '프로세스')
foreach ($r in $rows) {
    $p = Get-Process -Id $r.OwningProcess -ErrorAction SilentlyContinue
    $name = if ($p) { $p.ProcessName } elseif ($r.OwningProcess -eq 0) { '(없음/시스템)' } else { '(없는 프로세스 - 다른 프로세스가 소켓을 쥐고 있음)' }
    $target = $State -contains [string]$r.State
    Write-Host ("{0,-8}{1,-22}{2,-22}{3,-12}{4,-7}{5}" -f $(if ($target) { '대상' } else { '' }),
        ("{0}:{1}" -f $r.LocalAddress, $r.LocalPort), ("{0}:{1}" -f $r.RemoteAddress, $r.RemotePort), $r.State, $r.OwningProcess, $name)
}

$targets = @($rows | Where-Object { $State -contains [string]$_.State })
if (-not $Close) {
    Write-Host ""
    Write-Host "목록만 보여 줬습니다. 대상($($State -join ',')) $($targets.Count)건을 끊으려면 관리자 PowerShell 에서 -Close 를 붙여 다시 실행하세요."
    return
}

$isAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) { Write-Warning "관리자 권한이 아닙니다. SetTcpEntry 가 거부될 수 있습니다(오류 5). 관리자 PowerShell 로 다시 실행하세요." }

foreach ($r in $targets) {
    $rc = [TcpKill]::Delete($r.LocalAddress, $r.LocalPort, $r.RemoteAddress, $r.RemotePort)
    $res = if ($rc -eq 0) { '끊음' } elseif ($rc -eq 5) { '거부됨(관리자 권한 필요)' } else { "실패(코드 $rc)" }
    Write-Host ("{0}:{1} <- {2}:{3} [{4}] : {5}" -f $r.LocalAddress, $r.LocalPort, $r.RemoteAddress, $r.RemotePort, $r.State, $res)
}
