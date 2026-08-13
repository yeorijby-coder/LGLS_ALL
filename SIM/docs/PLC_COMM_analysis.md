# PLC 통신 비교 분석: 신ECS(EcsSv) vs 구ECS(ECP) vs PPT 사양 (2026-07-13)

## 정정: "쓰기 수정본" = EcsSv (ClientTask 아님)
- `D:\project\SPL\ECS\Source\ClientTask\CvTableLifter.cpp` = 모니터/XML 멀티캐스트 클라이언트, **PLC I/O 없음**.
- 실제 PLC 통신·쓰기수정(2026-07-13) = **`D:\project\SPL\ECS\Source\EcsSv\`**: 드라이버 `LgXgtFEnetIfSk.cpp`(+백업 .bak_20260713), 호출자 `CvTableLifter.cpp`.

## 신ECS(EcsSv) 통신
- **프로토콜**: LS XGT FEnet/TCP. `CLgXgtFEnetIfSk:CInterfaceSk:CAsyncSocketEx`. 프레임: "LSIS-XGT", CPU 0xA0, SoF 0x33, Read 0x54/recv0x55, Write 0x58/recv0x59, **DataType 0x14(연속바이트블록)만**.
- **읽기**(성공): `BatchRead(BYTE*,addr,cnt)` — 헤더+주소문자열 `TxBuff+30`(sprintf), 응답 offset32 파싱.
- **쓰기수정**: 백업의 `BatchWrite(CByteArray&)`는 **주소를 offset0에 memcpy → 헤더 파괴**(NAK). 수정본은 읽기방식 복제 → `BatchWrite(BYTE*,addr,cnt)`(주소 `TxBuff+30`, cmd0x58, DataType0x14), 별도 RxBuff, dwLen==0 가드.
- **영역표기**: **오직 `%DB<10진>`(D영역 바이트, word→byte *2)**. **M·R 영역 안 씀**, 비트는 word RMW(SetNibble/SetBit). 사이트 고유 트랙별 D블록(base 990word=%DB1980, 트랙*10*2).
- **토폴로지**: **설비당 소켓 1개**, INI IP/포트(기본 8192).

## 구ECS(ECP, 참조·변경금지) 통신
- **PLC 코드는 ECS가 아니라 형제 ECP 라이브러리**: `FenetDriver:AbstractFieldBusDriver`(EzControl.Driver.Fenet). 동일 LSIS-XGT 프레임(0x54/0x58, DataType 0x14 + 비트용 0x00/0x01).
- **읽기** `DirectReadFromPLC`, **쓰기** `DirectWriteToPLC`(addr*2, `%xB`), **비트** `mdDevSet/mdDevRst`(**`%MX`** + DataType 0x00/0x01).
- **영역표기**: XGT **`%MB/%DB/%RB`(바이트)+`%MX`(비트)**. 매핑 B→M, W→D, R→R.
- **메모리맵**: 소스 하드코딩 아님 — **LglsMCS SQL DB**(OBSERVABLE/ZCONFIG + FIELDBUSDRIVER.CONNECTSTRING) 구동. PPT 규약(M=이벤트/ack, D=word, R=tracking)과 일치하나 **정확한 오프셋은 DB에서 확인 필요**.
- **엔드포인트**: **단일 Master PLC `192.100.1.110:2004`**.

## 비교표 (핵심)
| 항목 | PPT사양 | 구ECS(ECP) | 신ECS(EcsSv) |
|---|---|---|---|
| 프로토콜/프레임 | LS FEnet(XGK) | LSIS-XGT FEnet/TCP | **동일** |
| Read/Write | — | 0x54/0x58 | 0x54/0x58 |
| DataType | — | 0x14 + 0x00/0x01(비트) | **0x14만(비트경로 없음)** |
| 표기 | M0160/D0210/R0102(약식) | %MB/%DB/%RB + %MX | **%DB만** |
| 사용영역 | M,D,R | M,D,R(+비트) | **D만** |
| C/V#11 | M0360/D0310/R0100 | DB구성 | **주소 안 씀**(자체 %DB블록) |
| 토폴로지 | 단일 Master PLC | 단일 192.100.1.110:2004 | **설비당 N개, 8192** |
| 맵 출처 | PPT | SQL DB | 코드 하드코딩 |

## Q3 통신 가능한가: 전송계층 YES, 응용계층 NO
- 프레임 동일 → 소켓 핸드셰이크는 됨. 차단요인: ①IP/포트/토폴로지(설비당8192 vs 단일 2004), ②메모리맵(신ECS는 M·R 안 건드림, PPT는 M비트/D워드/R트래킹), ③비트접근(%MX 없음), ④데이터 의미(JOB 4자리 2word 바이트스왑 1234→2143, Direction D0300~, 상태 D0160/D0210) 미구현. **재사용 가능=드라이버, 불가=CvTableLifter 맵/시나리오**.

## Q4 XGK 표기 정합: 문법은 3자 모두 정합
- XGK 직접변수 = `%`+device+size(X비트/B바이트/W워드/D더블/L). `%MB320/%MX0492/%DB620/%RB200` 모두 유효. PPT 약식 `M0160`은 `%MB320`로 변환 필요.
- 구ECS: `%MB/%DB/%RB+%MX` = XGK정확·검증된 형태. 신ECS: `%DB` = **형식상 XGK정확**(→ `%MW`로 바꿀 필요 없음). **부족한 건 표기가 아니라 M·R 영역과 %MX 비트경로.**

## 권장 수정(신ECS EcsSv만; 구ECS 변경 금지)
1. **Config**: 컨베이어군 설비 INI를 단일 Master PLC(192.100.1.110:2004)로. (Config.cpp:137-140)
2. **CvTableLifter에 M/R영역+비트 추가**: `%MB/%MX`(이벤트·ack: C/V#11 M0360, S/C#1 M0480, RGV#1 M0580, ack M0800/M0960/M1010), `%DB`(S/C D0160/RGV D0210/Direction D0300~/명령 D0320~), `%RB`(트래킹 (No-1)*10word, C/V#11 R0100, JOB 바이트스왑).
3. **드라이버에 %MX 비트경로 복원**: LgXgtFEnetIfSk에 cmd0x58/DataType0x00·0x01 비트 set/reset(구ECS mdDevSet/mdDevRst 미러).
4. **잠재버그 정리**: 미사용 `BatchRead(CByteArray&)`(offset0 헤더파괴 잔존) 제거/수정; 읽기(byte cnt)·쓰기(word cnt) 단위 통일.

## Q5 전체 시뮬레이션
- 신ECS가 PPT맵대로 통신하게 되면, 시뮬레이터(EQP_SIM)도 **PPT M/D/R 맵**을 말해야 전체 시뮬 성립. 현재 EQP_SIM은 LglsMCS 맵 기반. 정합 필요.
