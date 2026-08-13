# =====================================================================
# 원본 ECS(LGLS_1WH_original) 연동용 로컬 IP 별칭 추가
#
# 원본 ECS 는 다음 주소가 하드코딩되어 있음 (GlobalConstant.cs — ECS 무수정 원칙):
#   - PLC(=EQP_SIM)  : 192.100.1.110 : 2004  (FenetDriver 가 기동 시 즉시 접속)
#   - WMS(=HOST_SIM) : 192.100.1.142 : 9910  (S/E/F 보고 송신처)
# 이 IP 들을 이 PC 의 별칭으로 추가하면 EQP_SIM/HOST_SIM(모두 0.0.0.0 바인드)이
# 해당 접속을 받을 수 있다. ECS/시뮬레이터 수정 불필요.
#
# ※ 관리자 PowerShell 에서 실행:
#    powershell -ExecutionPolicy Bypass -File .\add_local_ips.ps1
# =====================================================================
$ErrorActionPreference = 'Stop'

$id = [Security.Principal.WindowsIdentity]::GetCurrent()
if (-not (New-Object Security.Principal.WindowsPrincipal($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Write-Host "[STOP] 관리자 PowerShell 에서 실행하세요." -ForegroundColor Red
    exit 1
}

# 호스트 전용 가상 어댑터(VMnet1)가 있으면 우선 사용, 없으면 첫 번째 활성 어댑터
$nic = Get-NetAdapter | Where-Object { $_.Status -eq 'Up' -and $_.Name -like '*VMnet1*' } | Select-Object -First 1
if ($null -eq $nic) { $nic = Get-NetAdapter | Where-Object Status -eq 'Up' | Select-Object -First 1 }
Write-Host "대상 어댑터: $($nic.Name)"

foreach ($ip in '192.100.1.110', '192.100.1.142') {
    if (Get-NetIPAddress -IPAddress $ip -ErrorAction SilentlyContinue) {
        Write-Host "$ip — 이미 존재"
        continue
    }
    # skipassource: 이 IP 가 아웃바운드 기본 소스 IP 로 잡히지 않도록
    netsh interface ipv4 add address name="$($nic.Name)" address=$ip mask=255.255.255.0 skipassource=true | Out-Null
    Write-Host "$ip — 추가됨"
}

Start-Sleep -Seconds 2
Get-NetIPAddress -AddressFamily IPv4 | Where-Object IPAddress -like '192.100.1.*' |
    Select-Object IPAddress, InterfaceAlias, AddressState | Format-Table

Write-Host ""
Write-Host "제거하려면:" -ForegroundColor Yellow
Write-Host '  netsh interface ipv4 delete address name="<어댑터명>" address=192.100.1.110'
Write-Host '  netsh interface ipv4 delete address name="<어댑터명>" address=192.100.1.142'
