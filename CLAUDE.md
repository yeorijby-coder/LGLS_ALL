# LGLS Renewal — 작업 지침

## 답변 형식 (최우선)

답변할 때 **맨 첫 줄에 답변 시리얼**을 쓰고, 그 다음에 본문을 이어간다.

```
YYYY-MM-DD HH:mm:SS.시리얼      예) 2026-08-22 11:58:54.6
```

- 시각은 **로컬 시각, 24시간제**.
- **시각을 추측하지 않는다.** 답변 직전에 반드시 조회한다:
  `date "+%Y-%m-%d %H:%M:%S"` (또는 `Get-Date -Format 'yyyy-MM-dd HH:mm:ss'`)
  조회 수단이 없는 환경이면 지어내지 말고 그 사실을 밝힌다.
- 시리얼은 답변마다 1씩 증가, **매일 0시 정각에 1로 리셋**.
- **한 턴에서 사용자에게 처음 보이는 텍스트**에 붙인다. 도구를 먼저 돌리며
  "~를 확인합니다" 같은 중간 안내 문장을 낼 거라면 **그 문장**이 첫 출력이므로
  거기에 시리얼을 붙인다. 마지막 정리 답변에만 붙이면 사용자 화면에서는
  시각 없이 시작한 것으로 보인다.

> claude.ai **설정 → 일반 → Claude 지침** 에 넣은 내용은 Claude Code 세션에
> 전달되지 않는다. 이 파일과 메모리에 적어야 반영된다.

---

## 시스템 구성

```
EQP_SIM ─(XGT 2004)─ WCS_TASK_CV ─┐
                                  ├─ SQL Server: LGLS_MCS_IO ─ IO_TASK
Client(Ecs) ──────────────────────┘                              │
                                        HOST_SIM ─(9911/9910)─ HOST_TASK
```

| 구분 | 경로 | 비고 |
|---|---|---|
| 설비 시뮬 | `SIM/EQP_SIM` | XGT PLC 시뮬, 포트 2004/3001 |
| 상위 시뮬 | `SIM/HOST_SIM` | WMS/IMS, 명령 9911 접속 / 보고 9910 수신 |
| 설비 통신 | `TASK/WCS_TASK_CV_BIN` | 구 EQP_TASK 계열 |
| 스케줄러 | `TASK/IO_TASK` | `.NET 4.0` → **`-p:Platform=x86`** 필요 |
| 상위 통신 | `TASK/WCS_TASK_HOST` | 〃 |
| 운전 화면 | `WCS/CPlusPlus/LGLS_CLIENT` | MFC, `Ecs.sln` |
| 구 ECS 원본 | `Backup/ECS`, `Backup/ECP` | **판단 기준** |

주소 정의는 `TASK/WCS_TASK_CV_BIN/7_DeviceMap/PlcAddressMap.xml` **단일 기준**.
수정 후 `bin/Debug/7_DeviceMap/` 에도 복사해야 한다.

---

## 절대 규칙

1. **모든 판단의 기준은 구 ECS(`Backup/ECS`, `Backup/ECP`)** 다. 명세서와 어긋나면 구 ECS를 따르되, 근거를 밝힌다.
2. **구 ECS / EQP_TASK 의 PLC 통신부를 임의로 수정하지 않는다.** V0.9 분기도 삭제 금지.
3. **C# ECS(`WCS/CSharp`)는 사용하지 않는다** — 다시 요구할 때까지.
4. 반자동은 **HOST 보고 금지**, 완료 시 삭제.
5. 작업이 끝나면 **커밋 + 푸시** (`yeorijby-coder/LGLS_ALL`).
6. 시뮬레이터 시험으로 바꾼 접속 설정은 **현장값으로 복원**한다.
   `TASK/WCS_TASK_HOST/bin/Debug/EcsComA.ini` → `RemoteIP=192.100.1.191`, `LocalPort=8001`, `RemotePort=8002`

---

## 인코딩 — 파일을 깨뜨리기 쉬운 지점

| 파일 | 인코딩 | 주의 |
|---|---|---|
| C++ Client `.cpp/.h` | **CP949** | Edit 도구 금지. Python/PowerShell로만 |
| `Ecs.rc` | **UTF-16** | |
| `EcsView.cpp` | UTF-8 BOM | 같은 폴더인데 다르다 — 열기 전 확인 |
| `rc_resource/**/*.ini` | UTF-16 | 다국어 4섹션 `kor/eng/chin/hun` |
| TASK C# / SIM C# | **UTF-8 BOM** | Edit 도구 사용 가능 |

줄끝은 **전부 CRLF** 로 통일했다(2026-08-30). 도구가 "줄 끝을 정규화할까요?" 를 묻지
않게 하기 위함이다. `.gitattributes` 에 `* -text` 를 두어 git 도 변환하지 않는다.
새 파일도 CRLF + (C#이면) BOM 으로 쓴다.

**파일 쓰기는 반드시 인코딩을 먼저 검증한 뒤 연다.**
`open(p,'w')` 는 즉시 truncate 하므로, encode 실패 시 **0바이트로 날아간다**.

```python
data = s.encode('cp949')          # 먼저 검증
with open(p, 'wb') as f:          # 그 다음 열기
    f.write(data)
```

CP949에 없는 문자(`—` em dash, `…` 등)를 쓰지 않는다. `→` 는 가능.

C++ 문자열 리터럴에 개행 이스케이프를 넣을 때 Python heredoc에서 `\r\n` 이
실제 개행이 되어 컴파일 오류가 나기 쉽다. `chr(92)+'r'+chr(92)+'n'` 로 조립한다.

---

## 빌드

```bash
MSB="/c/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe"

"$MSB" SIM/EQP_SIM/EQP_SIM.csproj -v:m -nologo
"$MSB" TASK/WCS_TASK_CV_BIN/WCS_TASK_CV.csproj -v:m -nologo
"$MSB" TASK/IO_TASK -p:Configuration=Debug -p:Platform=x86 -v:m -nologo
"$MSB" TASK/WCS_TASK_HOST -p:Configuration=Debug -p:Platform=x86 -v:m -nologo
"$MSB" WCS/CPlusPlus/LGLS_CLIENT/Ecs.sln -p:Configuration=Debug -p:Platform=Win32 -v:m -nologo
```

빌드 전에 해당 프로세스를 종료한다(파일 잠김).

---

## 기동 순서

`EQP_SIM` → `WCS_TASK_CV` → `IO_TASK` → `HOST_SIM` → `HOST_TASK`
→ HOST_SIM 이 **첫 상태보고(S)를 받은 뒤** [시작] 클릭

Client 는 `tools/Start-WcsClient.ps1` 로 띄운다 (왼쪽 절반 = Client, 오른쪽 = Claude).

---

## 검증

- **Client 작업은 반드시 화면 캡처로 확인**한다.
  창 단위 `PrintWindow` 캡처를 쓴다. 전체화면 캡처는 사용자의 다른 창이 찍히므로 피한다.
- UI 자동 클릭은 포그라운드 전환이 막혀 **다른 창을 누를 수 있다.** 창이 늘거나
  엉뚱한 동작이 보이면 즉시 멈추고 다른 방법을 찾는다.
- 설비 신호 검증은 `CV_DATA` 를 직접 조회하는 편이 확실하다.
  단 `WCS_TASK_CV` 의 15설비 순회 주기가 **약 16초**라, 3초짜리 펄스는 DB에서 놓칠 수 있다.

```bash
sqlcmd -S localhost\\SQLEXPRESS -d LGLS_MCS_IO -E -W -s '|' -Q "..."
```

---

## 주요 트랙 배치

| 트랙 | 설비 | 역할 | Host STN |
|---|---|---|---|
| 22 | C/V#11 | 입출고 겸용대(방향전환) | 101 |
| 24 | C/V#12 | 출고대 | 104 |
| 26 | C/V#13 | 입고대 | 103 |
| 29 | C/V#14 | 피킹 출고 | 102 |
| 30 | C/V#15 | 피킹 입고 | 102 |
| 103·104 | C/V#2 | S/C#1 통로(방향전환) | — |

- 로직1 = 126 입고 / 124 출고, 로직2 = 130 입고 / 129 출고, 로직3 = 122 겸용
- `JOB_STATUS` : 입고 `99→10→11→15→30→31→35→20→21→25→29`, 출고는 `→19`
- C/V#15 슬롯은 비선형 — `trackOrder="131,132,130"`
