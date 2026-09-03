# EXE - 실행본 모음 (2026-09-03)

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
 └ WCS_CLIENT/           운전 화면 (Ecs.exe)
```

## 기동 순서

    EQP_SIM  ->  WCS_TASK_CV  ->  IO_TASK  ->  HOST_SIM  ->  WCS_TASK_HOST
    -> HOST_SIM 이 첫 상태보고(S)를 받은 뒤 [시작] 클릭
    운전 화면(Ecs.exe)은 아무 때나 띄워도 된다.

실제 설비에 붙일 때는 EQP_SIM / HOST_SIM 을 띄우지 않고 나머지만 올린다.

## 접속 설정 (지금 담긴 값은 시뮬레이터용)

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
| USER_MENU | ECS 리본 '사용자' | 1 |
| SEMITEST_MENU | MANUAL 탭 '반자동 TEST' 그룹 | 1 |
| INI_MENU | ECS 리본 '[INI 열기]' | 1 |
| UIMODE_MENU | ECS 리본 'UI모드' 그룹 | 1 |
| EMPTYPLT_MENU | 뷰 '공PLT작업' | 1 |

TASK 쪽은 각 INI 의 [VIEW] LOG_FILTER_BTN 으로 [로그 필터] 버튼을 켜고 끈다.

## 서버(VS 미설치) 구동 요건

- **운전 화면(WCS_CLIENT/Ecs.exe)은 Release 빌드**다(디버그 런타임 의존 없음). 서버에는
  **Microsoft Visual C++ 2015-2022 Redistributable (x86)** 를 설치해야 한다
  (mfc140u.dll / vcruntime140.dll / msvcp140.dll / UCRT). FarPoint 스프레드(SPR32DU70.DLL)는 폴더에 들어 있다.
- **TASK 3종과 SIM 2종은 .NET Framework 4.8** 로 돈다(IO/HOST 는 4.0 대상이지만 4.8 위에서 동작).
  Windows Server 2019 이상은 기본 포함, 없으면 .NET Framework 4.8 런타임 설치.
- 현장 서버에서는 **EQP_SIM / HOST_SIM 은 띄우지 않는다**(시뮬레이터).
- DB 는 서버의 MS-SQL 을 쓰도록 각 INI 의 [DB] 접속 정보(IP,1433 / 계정)를 바꾼다. DB 이관은 `DB_BACKUP/서버_구축_절차.md`.
- 위 표의 시뮬값(127.0.0.1 등)을 현장값으로 바꾼다.

## 참고

- 다국어 폰트는 한국어(kor)만 담았다. 중국어 화면을 쓸 경우
  소스 트리의 `rc_resource/font/chin` 을 같은 경로에 복사한다.
- 각 폴더의 `Data` 는 런타임 상태/로그가 쌓이는 곳이라 비워 두었다.
