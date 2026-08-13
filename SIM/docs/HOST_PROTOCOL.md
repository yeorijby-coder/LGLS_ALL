# ECS ↔ HOST(WMS/IMS) TCP/IP 전문 사양 (ECS 소스 분석 결과)

분석 원본: `ECP\Com\Tcp\WMSCommand.cs`, `ECP\Service\Interface\ExternalIFManager.cs`,
`ECP\BizRule\MakeWmsIfString.cs`, `ECP\Db\Entity\TransferData.cs`, `ECS\App.config`, `ECS\HECS.ini`

## 채널 구성 (2개의 TCP 연결)

| 채널 | 방향 | 역할 | 기본 주소 |
|---|---|---|---|
| A | WMS → ECS | ECS가 **서버** (ECS_PORT=**9911** 수신대기). WMS(=HOST_SIM)가 접속해 O/R/M 전문 전송, ECS가 같은 소켓으로 ACK/NAK 즉시 응답 | ECS IP:9911 |
| B | ECS → WMS | ECS가 **클라이언트** (WMS_HOST:WMS_PORT=**192.100.1.142:9910** 으로 접속). S/E/F 보고 전문 송신. WMS 응답은 무시 | HOST_SIM이 9910 수신대기 |

- `HECS.ini [HOST] IP/PORT`가 WMS_HOST/WMS_PORT를 오버라이드 (기본 127.0.0.1/9910), `CONNECT_ON_START`, `RECONNECT` 플래그 존재.
- ECS는 상태문자열이 바뀔 때마다 S 전문 송출 (1초 타이머, 최초 연결 후 첫 수신 전문 = S 상태보고).
- 송신(S/E/F)은 5초 타이머로 드레인, 최대 3회 재시도.

## 프레임 구조 (ASCII 고정 필드)

```
[ HEADER 15 bytes ][ STX(0x02) ][ BODY ... ][ ETX(0x03) ]
```

### Header (15 bytes)
| 필드 | Off | Len | 내용 |
|---|---|---|---|
| MessageBox | 0 | 10 | 좌측정렬 space 패딩. WMS발신=`WMS_MBX`, ECS발신=`ECS_MBX` (수신검증: WMS_MBX 아니면 NAK 13) |
| MessageSize | 10 | 4 | 우측정렬 '0' 패딩. **STX+ETX 포함 BODY 바이트 수** (예: O=`0066`, S=`0043`, E=`0023`, F=`0010`, ACK=`0011`) |
| 예비 | 14 | 1 | space |

체크섬 없음. 검증: 크기/STX/ETX/메일박스/타입만.

## 메시지 타입
| 코드 | 이름 | 방향 | BODY 크기(STX/ETX 포함) |
|---|---|---|---|
| M | 모드변경 | WMS→ECS | 11 |
| O | 작업지시 | WMS→ECS | 66 |
| R | 재작업지시 | WMS→ECS | ~30 |
| S | 상태보고 | ECS→WMS | 43 |
| E | 에러보고 | ECS→WMS | 23 |
| F | 작업완료보고 | ECS→WMS | 10 |
| (reply) | ACK/NAK | ECS→WMS(같은 소켓) | 11 |

## O 작업지시 (BODY 66 bytes, body offset 기준)
| 필드 | Off | Len | 내용 |
|---|---|---|---|
| STX | 0 | 1 | 0x02 |
| Type | 1 | 1 | `O` |
| O_JobDefine | 2 | 1 | **1=입고(INGO), 2=출고(OUTGO), 6=이동(MOVE)** |
| O_LuggageNo | 3 | 4 | 작업/파렛트 번호 (숫자, 좌측 '0' 패딩). Commandid=yyyyMMdd+LuggageNo |
| O_SeqNo | 7 | 4 | 시퀀스 번호 |
| O_PalletNo | 11 | 7 | 파렛트 ID (space 패딩) |
| O_SourceStn | 18 | 3 | 출발 스테이션 (3자리, CodeMaster 조회) |
| O_SourceBank | 21 | 2 | 출발 랙 Bank |
| O_SourceBay | 23 | 3 | 출발 랙 Bay |
| O_SourceLevel | 26 | 2 | 출발 랙 Level |
| O_RouteStn | 28 | 3 | 경유 스테이션 |
| O_DestStn | 31 | 3 | 도착 스테이션 (3자리) |
| O_DestBank | 34 | 2 | 도착 랙 Bank |
| O_DestBay | 36 | 3 | 도착 랙 Bay |
| O_DestLevel | 39 | 2 | 도착 랙 Level |
| O_Priority | 41 | 3 | 우선순위 |
| O_JobRouting | 44 | 1 | 라우팅 플래그 |
| O_ProductID | 45 | 20 | 품목 ID |
| ETX | 65 | 1 | 0x03 |

### ECS 소비측 파싱 (TransferData.setTransferExternalIfData — wire offset = 15+body offset)
- CommandType = wire[17,1] (=JobDefine), LuggageNo = wire[18,4]
- FromStation = wire[33,3], FromLoc = wire[36,2]+wire[39,2]+wire[41,2] → **6자리 "BBbbLL" (Bank2+Bay뒤2+Level2)** 예: `010101`
  - `000000`이면 랙 셀 아님(스테이션 기본 위치), 아니면 Fromlocationid=`RACK:`+bank, Fromsublocationid=6자리
- ToStation = wire[46,3], ToLoc = wire[49,2]+wire[52,2]+wire[54,2]
- Priority = wire[56,3], ProductID = wire[60,20]

※ Bay는 3자리 필드지만 위치 조합 시 **뒤 2자리만** 사용됨 → 랙 표기 6자리 `010101` = Bank 01, Bay 01, Level 01.

※ 스테이션 코드 (TB_CODEMASTER codeclass '022', `db\seed_wms_station_codes.sql`):
101=C/V#11(TR#22), 102=C/V#12(TR#24, 입고대), 103=C/V#13(TR#26, 출고대),
104=C/V#14(TR#29), 105=C/V#15(TR#30), 201~205=하이랙 S/C#1~#5(Bank 2k-1,2k).
랙 셀을 지정할 때는 해당 Bank 담당 SC의 코드(201~205)를 스테이션으로 사용해야
경로 매칭(TB_TOTALPATH: STKSEM:k)이 성립함.

## ACK/NAK 응답 (BODY 11 bytes)
| 필드 | Off | Len | 내용 |
|---|---|---|---|
| STX | 0 | 1 | 0x02 |
| ReplyType | 1 | 1 | 수신타입 소문자 (`o`,`m`,`r`) 또는 오류 시 `x` |
| ACK | 2 | 1 | `A`=수락, `N`=거부 |
| Reason | 3 | 2 | 00=OK, 01=STX, 02=ETX, 07=크기, 13=메일박스, 16=타입 |
| LuggageNo | 5 | 4 | O/R의 LuggageNo 에코 |
| StepCount | 6 | 1 | (LuggageNo 영역과 겹침, 코드상 off 6) |
| ETX | 10 | 1 | 0x03 |

## F 작업완료보고 (BODY 10 bytes)
| 필드 | Off | Len | 내용 |
|---|---|---|---|
| STX | 0 | 1 | 0x02 |
| Type | 1 | 1 | `F` |
| F_JobDefine | 2 | 1 | 1=입고, 2=출고, 6=이동 |
| F_LuggageNo | 3 | 4 | 작업 번호 |
| F_CompleteClass | 7 | 1 | 완료 구분 1/2/6 |
| F_StepCount | 8 | 1 | 1=정상, 2=출고 전체완료 |
| ETX | 9 | 1 | 0x03 |

※ F에는 랙 위치가 없음 → HOST_SIM은 자기가 보낸 O의 LuggageNo↔랙 매핑을 기억해야 함.

## S 상태보고 (BODY 43 bytes)
STX, `S`, [SC01 id(2)+status(1)+luggage(4)] ×5 (SC01~SC05), 플래그 5개(1×5), ETX.
초기 상태 문자열: `0100000020000003000000400000050000000000` → SC 5대 status=0, luggage=0000.

## E 에러보고 (BODY 23 bytes)
STX, `E`, DeviceClass(2,1), DeviceNo(3,3), ErrorKind(6,1), ErrorCode(7,4), LuggageNo(11,4), Bank(15,2), Bay(17,3), Level(20,2), ETX. (RGV="10", SC="20" 계열)

## M 모드변경 (BODY 11 bytes)
STX, `M`, ConveyorNo(2,3) — 예: C/V#11 = "101", Mode(5,1) 0=RGV입고/1=RGV출고, ETX.
(ECS 파싱: conveyorNo=wire[17,3], mode=wire[20,1])
