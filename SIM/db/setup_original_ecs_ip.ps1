# =====================================================================
# 원본 ECS(LGLS_1WH_original) 실행용 로컬 IP 별칭 등록  (관리자 권한 필요)
#  - 원본 ECS 는 PLC 192.100.1.110:2004, WMS 192.100.1.142:9910 이 하드코딩되어
#    있으므로(GlobalConstant.cs), 두 IP 를 루프백 인터페이스에 별칭으로 추가해
#    EQP_SIM / HOST_SIM 이 해당 주소에서 수신하도록 한다. (ECS 무수정)
#  - 제거: 아래 add 를 delete 로 바꿔 실행
# =====================================================================
$ErrorActionPreference = 'Continue'
Start-Transcript -Path "$PSScriptRoot\setup_ip_log.txt" -Force | Out-Null
$if = 'Loopback Pseudo-Interface 1'

foreach ($ip in '192.100.1.110', '192.100.1.142') {
    $exist = netsh interface ipv4 show addresses $if | Select-String $ip
    if ($exist) {
        Write-Host "[SKIP] $ip 이미 등록됨"
    } else {
        netsh interface ipv4 add address $if $ip 255.255.255.255
        Write-Host "[OK] $ip 추가"
    }
}
netsh interface ipv4 show addresses $if
Stop-Transcript | Out-Null
