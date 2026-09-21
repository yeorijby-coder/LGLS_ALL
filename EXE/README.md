# EXE - 실행본 모음 (2026-09-21)

각 폴더를 통째로 복사하면 그 자리에서 실행된다. 빌드 산출물(pdb/bsc/map)과
런타임 로그는 빼고, 실행에 필요한 파일과 설정(INI/XML/리소스)만 담았다.

```
EXE/
 ├ SIM/
 │   ├ EQP_SIM/          설비 시뮬 (XGT PLC 시뮬, 2004/3001)
 │   └ HOST_SIM/         상위 시뮬 (WMS/IMS)
 ├ TASK/
 │   ├ WCS_TASK_CV/      설비 통신
 │   ├ IO_TASK/          스케줄러
 │   └ WCS_TASK_HOST/    상위 통신
 ├ WCS_CLIENT/           운전 화면 (Ecs.exe)
 └ SQL/                  현장 DB 반영 SQL (공통코드 추가, 알람코드)
```

**현장에 처음 올릴 때는 `SQL/` 을 먼저 실행한다** - 아래 [현장 DB 반영](#현장-db-반영-sql) 참고.

## 기동 순서

    EQP_SIM  ->  WCS_TASK_CV  ->  IO_TASK  ->  HOST_SIM  ->  WCS_TASK_HOST
    -> HOST_SIM 이 첫 상태보고(S)를 받은 뒤 [시작] 클릭
    운전 화면(Ecs.exe)은 아무 때나 띄워도 된다.

실제 설비에 붙일 때는 EQP_SIM / HOST_SIM 을 띄우지 않고 나머지만 올린다.

### 스크립트로 한 번에 (권장)

PowerShell 을 이 폴더에서 열고 :

```powershell
.\Start-All.ps1              # 시뮬 포함 전체 기동 (순서·대기시간 자동)
.\Start-All.ps1 -Site        # 현장용 - EQP_SIM / HOST_SIM 제외
.\Start-All.ps1 -NoClient    # 서버용 - 운전 화면 제외
.\Start-All.ps1 -Stop        # 기동 역순으로 전부 종료
```

이미 떠 있는 프로그램은 건너뛰고, 없는 실행 파일은 ★없음★ 으로 알려 준다.

### 접속 설정 전환

```powershell
.\Set-Mode.ps1               # 현재 설정 보기
.\Set-Mode.ps1 -Mode Sim     # 시뮬 (PLC 127.0.0.1 / HOST 9911-127.0.0.1-9910)
.\Set-Mode.ps1 -Mode Site    # 현장 (PLC 192.100.1.110 / HOST 8001-192.100.1.191-8002)
```

바꾸기 전 원본을 `*.bak_setmode` 로 남긴다. **DB 접속([DB]/[DB_2])은 건드리지 않는다** -
서버마다 다르므로 직접 적는다.

## 접속 설정 (지금 담긴 값은 **현장값**)

| 프로그램 | 파일 | 항목 | 시뮬값 | 현장값 |
|---|---|---|---|---|
| WCS_TASK_CV | WCS_DB.INI | [COMM0] IP | 127.0.0.1 | 192.100.1.110 |
| WCS_TASK_HOST | EcsComA.ini | LocalPort / RemoteIP / RemotePort | 9911 / 127.0.0.1 / 9910 | 8001 / 192.100.1.191 / 8002 |
| 공통 | 각 INI [DB] IP | DB 서버 | localhost\SQLEXPRESS | 서버 IP,1433 |

각 프로그램 화면의 [INI 열기] 버튼으로 해당 파일을 바로 열 수 있다.
운전 화면은 리본 ECS > 환경설정 > [INI 열기].

## 화면 표시 스위치 (WCS_CLIENT/Ecs.ini 의 [MENU])

| 키 | 내용 | 기본 |
|---|---|---|
| SEMITEST_MENU | MANUAL 탭 '반자동 TEST' 그룹 | 1 |
| INI_MENU | ECS 리본 '[INI 열기]' | 1 |
| PRODINFO_MENU | 뷰 '생산정보' | 1 |
| EMPTYPLT_MENU | 뷰 '공PLT작업' | 1 |
| SCWAIT_VIEW | 크레인 대기 위치 표시 | 1 |
| ZOOM_BTN | 설비 대화상자 '[확대]' 버튼 | 1 |
| SC_MANUAL_BTN | 크레인 상태창·리본 MANUAL 의 'S/C 수동지시' | 1 |
| RTV_MANUAL_BTN | RGV 상태창·리본 MANUAL 의 'RTV 수동지시' | 1 |
| LOADBIT_GATE | 차량 색을 적재비트로 게이팅 | 1 |
| UI_TRACE | 화면 동작 추적 로그 | 1 |
| DIR_ADDR_CV11 / DIR_ADDR_CV2 | 방향 칸 더블클릭 시 쓰는 PLC 주소 | 310 / 301 |
| DEFAULT_ID / DEFAULT_PW | 기동 시 자동 로그인 계정([USER] 섹션). 비우면 로그인 창을 그대로 둔다 | LGLS |
| COLLECT_INTERVAL_MS | 설비 데이터 수집 주기 ms([ETC] 섹션, 100~5000) | 300 |
| [SC_ERR] ERR_TYP / DUAL_CODES / EMPTY_CODES | 크레인 알람 코드표 구분과 이중입고·공출고 코드 | SC_LGLS / 73,74 / 75 |

2026-09-19 에 아래 키를 **없앴다**(코드에 고정). INI 에 적어도 읽지 않는다.

| 없앤 키 | 고정된 값 |
|---|---|
| USER_MENU / UIMODE_MENU / AUTOTIME_MENU / PANEVIEW_MENU / PROCESS_MENU | 해당 메뉴·그룹을 쓰지 않는다(화면에서 제거) |
| MAIN_UI | 2 (좌측 판넬 + 리본) |
| VEH_CLEAR_MODE | 3 (종전 표시 + 새 트랙 기록 시 지움) |
| STATUS_POS | RIBBON |
| COMM_MODE | 2 (탭 줄 [통신] 이름표 + 리본 오른쪽 끝 [통신] 그룹) |

TASK 쪽은 각 INI 의 [VIEW] LOG_FILTER_BTN 으로 [로그 필터] 버튼을 켜고 끈다.

## 서버(VS 미설치) 구동 요건

- **운전 화면(WCS_CLIENT/Ecs.exe)은 Release 빌드**다(디버그 런타임 의존 없음). 서버에는
  **Microsoft Visual C++ 2015-2022 Redistributable (x86)** 를 설치해야 한다
  (mfc140u.dll / vcruntime140.dll / msvcp140.dll / UCRT). FarPoint 스프레드(SPR32DU70.DLL)는 폴더에 들어 있다.
- **TASK 3종과 SIM 2종은 .NET Framework 4.8** 로 돈다(IO/HOST 는 4.0 대상이지만 4.8 위에서 동작).
  Windows Server 2019 이상은 기본 포함, 없으면 .NET Framework 4.8 런타임 설치.
- 현장 서버에서는 **EQP_SIM / HOST_SIM 은 띄우지 않는다**(시뮬레이터).
- DB 는 서버의 MS-SQL 을 쓰도록 각 INI 의 [DB] 접속 정보(IP,1433 / 계정)를 바꾼다. DB 이관은 `DB_BACKUP/서버_구축_절차.md`. 최신 정리본은 `DB_BACKUP/LGLS_MCS_IO_20260907_clean.bak`.
- 위 표의 시뮬값(127.0.0.1 등)을 현장값으로 바꾼다.

## 키오스크(운전 화면 전용 PC) 구동 요건

키오스크에는 **WCS_CLIENT 폴더만** 복사한다. MS-SQL / .NET / Visual Studio 모두 필요 없다.

1. `EXE/WCS_CLIENT` 폴더를 통째로 복사 (Release 실행 파일, DLL, 리소스, 폰트 포함)
2. `EXE/Prerequisites/vc_redist.x86.exe` 설치 (VC++ 2015-2022 x86) - 이것 하나만
3. `WCS_CLIENT/Ecs.ini` 의 `[DB_2]` 를 서버로 변경

```
[DB_2]
DRIVER=SQL Server          ; Windows 기본 내장 ODBC 드라이버 - 추가 설치 불필요
SERVER=서버IP,1433         ; 인스턴스 이름(서버IP\인스턴스) 보다 IP,포트 형식이 간단
USERID=LGLS_IO             ; 서버에 만든 SQL 로그인(혼합 인증)
USERPASSWORD=LGLS_IO
DATABASE=LGLS_MCS_IO
```

### SQL 로그인을 만들지 않고 쓰려면 (Windows 인증)

운전 화면을 **서버에서 직접** 돌리는 경우, 로그인을 새로 만들 것 없이 Windows 인증으로 붙일 수 있다.

```
[DB_2]
DRIVER=SQL Server
SERVER=localhost\인스턴스명     ; 기본 인스턴스면 localhost
TRUSTED=1                        ; ← 이것만 켜면 USERID/USERPASSWORD 는 무시된다
DATABASE=LGLS_MCS_IO
```

Ecs.exe 를 실행하는 **Windows 계정**이 SQL Server 에 권한이 있어야 한다(설치한 관리자 계정이면 보통 이미 sysadmin).
없다면 서버에서 한 번만 :

```sql
CREATE LOGIN [서버\계정] FROM WINDOWS;
USE LGLS_MCS_IO; CREATE USER [서버\계정] FOR LOGIN [서버\계정];
EXEC sp_addrolemember 'db_owner', '서버\계정';
```

### 접속이 안 될 때

`Test-EcsDb.ps1` 을 WCS_CLIENT 폴더에서 돌리면 Ecs.exe 와 **똑같은 경로**(32비트 + ADO + ODBC)로
붙어 보고, 인스턴스 이름 / 실제 TCP 포트 / 32비트 드라이버 목록까지 보여 준다.

```powershell
.\Test-EcsDb.ps1                 # Ecs.ini 그대로
.\Test-EcsDb.ps1 -Trusted        # Windows 인증으로 시험
```

주의할 점 두 가지 :

- 포트는 **쉼표**다. `Server=IP:1433` (콜론) 은 반드시 실패한다 → `Server=IP,1433`
- `,1433` 은 그 인스턴스가 정말 1433 을 듣고 있을 때만 된다. SQL Express 기본값은 **동적 포트**다.
  잘 모르겠으면 `SERVER=localhost\인스턴스명` 형식을 쓰는 편이 안전하다.

- 서버 쪽은 `DB_BACKUP/서버_구축_절차.md` 3·4번 (혼합 인증, TCP 1433 고정, 방화벽 1433 인바운드) 이 되어 있어야 한다.
- 확인 : Ecs.exe 하단 상태바의 EQUIP / HOST / SCH 가 초록이면 DB 를 정상으로 읽는 것(서버의 TASK 가 돌고 있어야 초록).
- 접속이 안 되면 키오스크의 **ODBC 데이터 원본 관리자(32비트)** 에서 "SQL Server" 드라이버로 서버 연결 테스트 -
  네트워크/인증 문제를 먼저 가린다.
- 키오스크에는 EQP_SIM / HOST_SIM / TASK 를 두지 않는다. 화면·알림창·판넬·설비 대화상자 모두 서버 DB 만 보고 동작한다.

## Prerequisites/ (설치 파일, Microsoft 공식 배포본)

| 파일 | 용도 | 비고 |
|---|---|---|
| `vc_redist.x86.exe` | VC++ 2015-2022 재배포 패키지 x86 (운전 화면용) | aka.ms/vs/17/release/vc_redist.x86.exe |
| `ndp48-x86-x64-allos-enu.exe` | .NET Framework 4.8 오프라인 설치(인터넷 없는 서버) | 121MB - GitHub 100MB 제한으로 저장소에는 올리지 않음(로컬 폴더에만) |
| `ndp48-web.exe` | .NET Framework 4.8 웹 설치(인터넷 되는 서버) | |

설치 순서 : .NET 4.8 → vc_redist.x86 → (재부팅 요구 시 재부팅) → 각 프로그램 폴더 복사 → INI 수정.

## 현장 DB 반영 SQL

`SQL/` 의 두 파일을 **프로그램 교체 전에** 이 순서로 실행한다. 여러 번 실행해도 안전하다(있으면 건너뛴다).

| 파일 | 내용 |
|---|---|
| `00_현장DB_공통코드_추가.sql` | 작업상태 `16`(입고 통로 H/S), 크레인 에러상태 `3` 공통코드 |
| `260917_알람리스트_EQP_ECD_MST.sql` | PLC 알람 리스트 86건 - 크레인 `SC_LGLS` 58 / 메인 C/V `0119~0131` 13 / RTV 15 |

`1호기_출고_지연분석.sql` 은 현장에서 1호기 출고가 늦을 때 원인을 보는 조회용이다(SQL Server 2008 호환).

교체는 **크레인·RTV 가 반송 중(25/35)인 작업이 없을 때** 한다.

## 2026-09-21 판 변경 요약

| 구분 | 내용 |
|---|---|
| 공통 | **PLC 알람 리스트 반영** - 크레인 코드표 `SC_LGLS`, 메인 C/V 과부하 비트(M5501~M550D) → 코드 `0119~0131`, RTV 알람 비트(M5601~M560F) 보조 판정. 이중입고 `73`·`74` / 공출고 `75` 로 변경(종전 54/58) |
| 스케줄러 | **C/V#2 통로 사용권(`CV2_DIR_MODE=2`)** - 통로 방향을 작업 상태로 판정해 역방향 지시를 보류. `CV2_FAIR=0`(출고 우선), `CV2_STALL_SEC=60` |
| 스케줄러 | **같은 트랙 이중 발행 방지** - 한 사이클에 같은 작업대로 지시가 두 번 나가 화물이 정체되던 결함 수정 |
| 스케줄러 | 입고 통로 상태 `16`(`IN_HS_STATUS`) 적용 |
| 운전 화면 | 설비 대화상자에서 **쓰지 않는 숨김 컨트롤 제거**, 컨트롤 위치를 리소스(rc) 기준으로 이관(런타임 재배치 제거) |
| 운전 화면 | 크레인 상태창 **수직주행 삭제**, 에러 칸에 "코드 + 알람 문구" 표시 |
| 운전 화면 | HOST 로그에 **[상태 보고(S) 제외] 체크박스** |
| 운전 화면 | 미사용 메뉴 제거 + 화면 방식 고정(위 표) |
| 공통 | 미사용 INI 키·소스 정리, 스케줄러 표시등을 `EQP_MST` 하트비트 기준으로 변경 |

검증 : 2026-09-20~21 연속 운전 18시간 50분 - 3,269건 완료, 무정지·설비에러 0건.

## 2026-09-07 판 변경 요약

| 구분 | 내용 |
|---|---|
| 운전 화면 | C/V 상태창 **[H/S 배출]** 신설 (예외 처리 그룹) - 작업 없는 화물을 출고대로 내보낸다 |
| 운전 화면 | S/C·RTV 상태창의 **[지시 삭제] 제외** - 설비 인터페이스에 반송지시 취소 신호가 없다 |
| 운전 화면 | 설비 **확대 패널에 작업정보 6항목 복원** (구 ECS 팝업 대비 누락분), 항목마다 영역 병기 |
| 운전 화면 | C/V 상태창 **방향 표시 수정** - 다른 현장 트랙번호를 읽던 것을 현재 트랙으로 |
| 스케줄러 | **[강제완료] 동작 구현** (S/C 25→29 / RTV 35→39) - 종전에는 버튼만 있고 소비하는 곳이 없었다 |
| 스케줄러 | 출고 지시 시 **빈 출고대에 트래킹이 미리 찍히던 문제 수정** (구 ECS 규약 복원) |
| 스케줄러 | RTV 체류에도 **1회 한정 시간기반 자동 처리** 확대 |
| 설비 시뮬 | **[화물 생성]** 신설 - 원하는 트랙에 (작업번호 없는) 화물을 만들어 예외 상황 재현 |

검증 : 2026-09-07 전체 시험운전 29분 - 3개 로직 순환 70건 완료,
설비에러 / 체류 / 통신오류 **각 0건**, 평균 사이클 44초.
이 배포본으로 재기동해 3개 로직 작업 발행·전문 수불까지 확인했다.

## 참고

- 다국어 폰트는 한국어(kor)만 담았다. 중국어 화면을 쓸 경우
  소스 트리의 `rc_resource/font/chin` 을 같은 경로에 복사한다.
- 각 폴더의 `Data` 는 런타임 상태/로그가 쌓이는 곳이라 비워 두었다.
