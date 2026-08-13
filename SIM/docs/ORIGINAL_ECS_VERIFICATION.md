# 원본 ECS ↔ PPT V1.1 시나리오 검증 보고

- 원본: `D:\project\LGLS\LGLS_1WH_original\Backup` (ECS/ECP/HECS.ConveyorSimulator...)
- 기준 문서: `ECS-분석-PLC사양및시나리오_LG생명과학_V1.1_20260709.pptx` (27슬라이드, V1.0+부록)
- 대조 방법: 참조본(일반비_LG생명과학) 소스와의 전수 diff + DB(TB_OBSERVABLE, localhost\sqlexpress LglsMCS) 주소 대조

## 결론: 핵심 로직·주소는 시나리오(V1.1)와 일치 — 실행 구성 요건만 충족하면 됨

### 일치 확인 (시나리오 = 원본 구현)
| 항목 | 근거 |
|---|---|
| 반송/배차 로직 | `ECS\Device\Dispatcher\ECSDispatcher.cs` 가 참조본과 **동일 파일**(md5 일치) — V1.0/1.1의 "구현확인" 주석 포함 검증 결과 그대로 적용 |
| WMS 전문 (O/M/R/S/E/F, ACK/NAK) | `ECP\Com\Tcp\WMSCommand.cs` 동일 |
| XGT FEnet 프로토콜 | `FenetDriver.cs` 의 프레임 생성/파싱(연속읽기 0x54, 쓰기 0x58, %MX 비트, "0###" DATACOUNT, 주소×2) 동일 — 차이는 참조본에 추가된 접속편의(TryConnectPlc/재접속/HECS.ini)뿐 |
| 메모리 맵 (B/W/R) | TB_OBSERVABLE 524점 = PPT 슬라이드 18~26 주소와 일치 (예: C/V#11 Load Complete#2 B0244=%MX580, Ack B05A4=%MX1444, 방향 W01EA=%DB980, 트래킹 R0100=%RB512) |
| 부록(V1.1) 매핑 규칙 | 워드주소×2=%DB 바이트, B 비트주소→%MX — 코드와 일치 |

### 문서(V1.1)와 실제 구현의 차이 (원본에도 그대로 존재)
1. **방향워드(DIRECTION_MODE) 의미가 문서와 반대**: 문서 "입고 0, 출고 1"이지만 실제 코드는
   RGV 하역(잉고) 디스패치에서 '1' 기록(ECSDispatcher 1303~1310), SC→C/V#2 출고 디스패치에서 '0'(846~852).
   C/V#11 은 '0' 복귀 코드가 없어 출고 후 '1' 고착 — **WMS M 전문(101, mode 0)** 만이 CONVEYOR11_MODE 를 복귀시킴.
2. **부록 슬라이드의 R영역 기술 오류 2건**: ① "R은 10진 주소" → 실제 드라이버는 CONNECTSTRING의
   ADDRESS_NO 를 **16진**으로 파싱 (R0102→%RB516, 슬라이드 8 병기값 자체가 16진 해석과 일치),
   ② "R은 M 영역 워드 접근" → 실제 전송 디바이스는 **%RB** (FenetDriver mdSend 22→'R').
3. V1.0 "구현확인" 노트의 미구현 항목들(방향전환 가드, Complete OFF 시 Ack OFF, Unload Request#2 등)은
   문서에 이미 명기된 그대로 원본에도 동일.
4. OnWaitOut 은 WAIT_OUT rising 1회만 처리(실패 시 재시도 없음) — PLC(시뮬레이터)측 재발화로 보완 필요.

### 원본 실행 구성 요건 (ECS 무수정 — 환경/데이터로만 해결)
| 요건 | 상태 |
|---|---|
| DB: `localhost\sqlexpress` LglsMCS (원본 App.config 고정) | attach 되어 있음 ✓ |
| TB_FIELDBUSDRIVER: FENET/USEFLAG=T + **R_START_ADDR=0, R_END_ADDR=8192** (없으면 트래킹 String 판독 KeyNotFound) | 반영되어 있음 ✓ |
| TB_CODEMASTER codeclass '022'(WMS 스테이션 코드 101~105/201~205), '024'(상태보고 소스) | 시드 반영됨 ✓ (`db\seed_*.sql`) |
| PLC 주소 **192.100.1.110:2004 하드코딩** (GlobalConstant, ini 없음) — FenetDriver 정적 생성자가 부팅 시 즉시 접속(실패 시 기동 불가) | **로컬 IP 별칭 필요** + EQP_SIM(2004) 선기동 |
| WMS 주소 **192.100.1.142:9910 하드코딩** — S/E/F 보고 채널 | **로컬 IP 별칭 필요** + HOST_SIM(9910) 선기동 |
| WMS→ECS 명령 채널 9911 (ECS 서버) | HOST_SIM이 127.0.0.1:9911 접속 ✓ |

IP 별칭(관리자 PowerShell 1회):
```
netsh interface ipv4 add address "Loopback Pseudo-Interface 1" 192.100.1.110 255.255.255.255
netsh interface ipv4 add address "Loopback Pseudo-Interface 1" 192.100.1.142 255.255.255.255
```
(제거: `netsh interface ipv4 delete address ...`)

### 기동 순서 (원본 ECS 기준)
① EQP_SIM (2004/3001 수신) → ② HOST_SIM (9910 수신) → ③ 원본 `HECS.exe`
(원본은 PLC 접속 실패 시 부팅 자체가 실패하므로 순서 엄수. EQP_SIM 재시작 시 원본 ECS도 재시작.)
