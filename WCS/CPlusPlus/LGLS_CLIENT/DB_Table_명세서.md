# KET_WCS_CLIENT DB Table 명세서

> 대상: `KET_CLIENT`(= KET_WCS_CLIENT) 소스 전반의 DB 접근 취합
> 작성일: 2026-07-01 · 문서 인코딩: UTF-8
> 근거: `Ecs`, `HostSim_DB` 프로젝트 정적 분석 (소스 CP949)

---

## 1. 개요

| 항목 | 내용 |
|---|---|
| **DBMS (활성)** | **PostgreSQL** (ODBC) — 컴파일 매크로 `POSTGRESQL=1`(`Ecs\stdafx.h:73`) |
| **DBMS (비활성)** | Oracle 11g — 코드/설정은 있으나 `#if ORACLE` 분기라 미사용 |
| **접근 기술** | Microsoft ADO (`#import msado20.tlb`), `_ConnectionPtr` / `_RecordsetPtr`, 클라이언트 커서(`adUseClient`) |
| **DB 접근 존재 프로젝트** | **`Ecs`**(WCS Client), **`HostSim_DB`**(HostSim의 DB 버전) |
| **DB 접근 없음** | `CvSim`, `ScSim`, `HostSim`, `Lib`(XmlLib/DciLib/EcsLib/BuilderLib) → 소켓/Melsec PLC 통신 전용 |
| **접근 스타일** | 대부분 **동적 문자열 SQL**(`CString::Format`) 직접 실행. 저장 프로시저 실행 기구는 존재하나 **호출되지 않는 dead code** |
| **필드 읽기** | `CRecordSetWrap::GetItem(_T("COLUMN"))` 방식이 지배적(수백 개 호출부) |

> ⚠️ **보안 유의**: DB 자격증명이 `Ecs.ini`에 **평문** 저장. SQL은 파라미터 바인딩 없이 문자열 결합 → **SQL 인젝션 위험**. (개선은 별건 과제)

---

## 2. 연결 정보

연결 문자열은 `CAdoDB::ConnectDB()`에서 조립되며, 설정값은 `Ecs.ini`의 `[DB_1]`(Oracle) / `[DB_2]`(PostgreSQL) 섹션에서 읽음(`Config.cpp`). `[CONNECT]` 섹션은 **연결 실패 시 진단용으로 기록**될 뿐 시작 시 읽지 않음.

### 2.1 Ecs (WCS Client) — `Ecs\Ecs.ini`

| 섹션 | 키 | 값 |
|---|---|---|
| `[DB_1]` Oracle(비활성) | DRIVER / SERVER / USERID / USERPASSWORD | `Oracle in OraClient11g_home1` / `ORCL` / `SKI_WCS` / `SKI_WCS` |
| `[DB_2]` **PostgreSQL(활성)** | DRIVER | `PostgreSQL Unicode` |
| | SERVER | `LOCALHOST` (주석 대안: `10.117.3.174`, `10.99.43.238`) |
| | USERID / USERPASSWORD / DATABASE | `LFC` / `LFC` / `LFC` |

연결 문자열 조립 (`Ecs\CAdoDB.cpp:80`, PostgreSQL 활성 분기):
```cpp
strConnet.Format(_T("Driver=%s;Server=%s;uid=%s;pwd=%s;Database=%s"),
    m_strDATABASE_DRIVER, m_strDATABASE_SERVER, m_strDATABASE_USERID,
    m_strDATABASE_USERPASSWORD, m_strDATABASE_DATABASE);
```
Oracle 분기(`CAdoDB.cpp:74`): `DRIVER=%s;Dbq=%s;UID=%s;PWD=%s`

### 2.2 HostSim_DB — `HostSim_DB\Ecs.ini`

| 섹션 | 키 | 값 |
|---|---|---|
| `[DB_1]` Oracle(비활성) | DRIVER / SERVER / UID·PW | `Oracle in OraClient11g_home1` / `ORCL` / `SKI_WCS`·`SKI_WCS` |
| `[DB_2]` **PostgreSQL(활성)** | DRIVER | `PostgreSQL Unicode` (주석 대안: `PostgreSQL35W`, `PostgreSQL ANSI`) |
| | SERVER | `Localhost` (주석 대안: `10.117.2.174`, `10.17.17.24`, `192.168.243.189`) |
| | USERID / USERPASSWORD / DATABASE | `HUONS` / `HUONS` / `HUONS` |

연결 문자열 조립 (`HostSim_DB\CAdoDB.cpp:88`, 활성 분기 — Ecs와 형식이 다름):
```cpp
strConnet.Format(_T("Provider=MSDASQL.1;Driver={%s};Server=%s;Port=5432;uid=%s;pwd=%s;"
                    "Database=%s;SSLMode=prefer;Client Encoding=UTF8;"), ...);
```

### 2.3 SQL 방언 토큰 (`Ecs\EcsDoc.cpp:193` `ConfigDbLang`)

DBMS별로 아래 토큰이 세팅되어 SQL 문자열에 결합됨:

| 토큰 | Oracle | **PostgreSQL(활성)** |
|---|---|---|
| `SYSDATE` | `SYSDATE` | `NOW()` |
| `NVL` | `NVL` | `COALESCE` |

---

## 3. DB 접근 아키텍처

| 클래스 | 파일 | 역할 |
|---|---|---|
| `CAdoDB` | `CAdoDB.cpp`, `AdoDB.h` | 연결/트랜잭션/쿼리 실행 핵심. `ExecuteQueryString`(adCmdText), `SelectSqlForThread(_RecordSet)`(SELECT), `ExecuteSqlForMainPGM`, `ExecuteStoredProc`(adCmdStoredProc, 미사용) |
| `CAdoDbIO` / `CIOParam` | `AdoDbIO.cpp/.h` | 저장 프로시저 입·출력 파라미터 컨테이너(`m_strProcName`, `m_listInput/Output`). 작업유형 상수 `STO="01"`, `RET="02"`, `RTR="03"` |
| `CRecordSetWrap` | `RecordSetWrap.cpp/.h` | `_RecordsetPtr` 래퍼. `GetItem(name)` → `Fields->GetItem(name)->Value` (주 읽기 경로) |
| `CCollectDB` / `CCollectDataList` | `CollectDB.cpp`, `CollectDataList.cpp` | 백그라운드 폴링 스레드로 설비/연결 상태를 주기적으로 SELECT. `GetSelQuery_CELL_MST()`, `GetSelQuery_JOB_MST()` 등 |
| `CURMDBAccess` | `URMDBAccess.cpp/.h` | 업무 DB 접근 파사드(~50개 메서드). **다수가 구 Oracle/STKCIM 스키마 기반 주석처리(dead)** |

### 트랜잭션
`CAdoDB::BeginTrans / CommitTrans / RollbackTrans` 제공.

### 저장 프로시저 (현재 **미사용/dormant**)
- 제네릭 실행부: `ExecuteStoredProc(CAdoDbIO*, int)` — proc명은 `m_strProcName`에서 오지만 Ecs 코드 어디서도 실제 proc명을 대입하지 않음(`""`로만 초기화).
- 고정 출력 프로시저 실행부: `ExecuteProcForInputOrder` / `ExecuteProcForOutputOrder` / `ExecuteProcForManualOrder`.
  - 공통 출력 파라미터 관례: `poRetCd`(int 4), `poMsg`(char 1000), `poDBErr`(char 1000), 출고주문은 추가로 `poJobNo`(char 5).
  - 입력 파라미터: `strTempList`에서 접두사 `I`면 정수(`adInteger`), 그 외 문자(`adChar`).
- 주석으로만 남은 Oracle 프로시저: `Sp_Logging(...)`, `Sp_RackUpdateProc(...)` (`URMDBAccess.cpp:1639,1656`).

### 시퀀스
- **`MANUAL_SEQ`** — 수동 작업번호 채번. `NEXTVAL('MANUAL_SEQ')`(PostgreSQL, `ScSkinDlg.cpp:2036`, `ScManualRet.cpp:508`) 및 `MANUAL_SEQ.NEXTVAL`(Oracle식, `ViewRackDlg.cpp:1192,1240`).

---

## 4. 테이블 목록 (요약)

### 4.1 핵심 WCS/WMS 스키마 (활성)

| # | 테이블 | 구분 | 래퍼 클래스 | 비고 |
|---|---|---|---|---|
| 1 | `JOB_MST` | 마스터 | `CJOB_MST` | 작업(반송지시) |
| 2 | `JOB_MST_HIS` | 이력 | - | 작업 이력 |
| 3 | `CELL_MST` | 마스터 | - | 셀(랙 로케이션) 마스터 |
| 4 | `CELL_DTL` | 상세 | `CCELL_DTL` | 셀 상세 |
| 5 | `CV_DATA` | 데이터 | `CCV_DATA` | 컨베이어 상태 |
| 6 | `SC_DATA` | 데이터 | `CSC_DATA` | 스태커크레인 상태 |
| 7 | `RTV_DATA` | 데이터 | `CRTV_DATA` | RTV(레일 반송차) 상태 |
| 8 | `WC_DATA` | 데이터 | `CWC_DATA` | 중량검사기 상태 |
| 9 | `WC_HIS` | 이력 | - | 중량검사 이력 |
| 10 | `BCR_MST` | 마스터 | `CBCR_MST` | 바코드리더 마스터 |
| 11 | `EQP_MST` | 마스터 | - | 설비 마스터 |
| 12 | `EQP_ECD_MST` | 마스터 | `CEQP_ECD_MST` | 설비 에러코드 마스터 |
| 13 | `EQP_ERR_HIS` | 이력 | - | 설비 에러 이력 |
| 14 | `COMMON_CODE` | 코드 | - | 공통코드 |
| 15 | `CHG_LANG` | 코드 | - | 다국어 번역 |
| 16 | `USER_MST` | 보안 | - | 사용자 |
| 17 | `USER_GRP` | 보안 | - | 사용자 그룹 |
| 18 | `USER_GRP_WIN` | 보안 | - | 그룹별 화면 권한 |
| 19 | `HOST_EMPTY_PLT` | 업무 | - | 공파렛트(호스트) |
| 20 | `CELL_MES_REQ` | 업무 | - | 셀 MES 요청 |
| 21 | `DEL_HIS_SETTING` | 설정 | - | 로그/이력 삭제 설정 |
| 22 | `SC_HS_DEF` | 정의 | - | SC 핸드셰이크 위치 정의 |
| 23 | `DEST_POS_DEF` | 정의 | - | 목적지 위치 정의 |
| 24 | `CV_DEF_INF` | 정의 | - | 컨베이어 정의 정보 |
| 25 | `WCS_CLIENT_LOG` | 로그 | - | 클라이언트 로그 |
| 26 | `WCS_LOG_PGR` | 로그 | - | 프로그램 로그 |
| 27 | `MES_IF_LOG` | 로그 | - | MES 인터페이스 로그 |
| 28 | `HOST_IF_LOG` | 로그 | - | 호스트 인터페이스 로그 |
| 29 | `IF_LUGG_STA` | 인터페이스 | - | 화물 상태 IF (HostSim_DB) |
| 30 | `IF_LUGG_STA_HIS` | 이력 | - | 화물 상태 이력 (HostSim_DB) |
| 31 | `IF_MC_STA` | 인터페이스 | - | 설비 상태 IF (HostSim_DB) |
| — | `MANUAL_SEQ` | 시퀀스 | - | 수동 작업번호 채번 |

### 4.2 레거시 스키마 (URMDBAccess.cpp — 전량 주석처리, 미사용)
구 Oracle/STKCIM(타 WMS) 스키마: `LocMast`, `JobInfo`, `ZoneInfo`, `TaskInfo`, `PortInfo`, `ScInfo`, `Code`, `CodeGroup`, `WhInfo`, `Users`, `PgmMast`, `Dual`. → 8개 래퍼 클래스와 무관. 참고용으로만 §7에 정리.

---

## 5. 테이블 상세 명세 (래퍼 클래스 기준 — 컬럼 확정)

> 표기: 장비-데이터 클래스는 멤버 접두사 `V_`가 실제 컬럼값, `K_`는 조회 키 헬퍼. 아래는 실제 컬럼(`V_` 제거)만 정리. 타입은 C++ 멤버 타입(대부분 `CString`).

### 5.1 `JOB_MST` — 작업(반송지시) 마스터 · `CJOB_MST`(`JOB_MST.h`)

| 컬럼 | 타입 |
|---|---|
| WH_TYP | CString |
| LUGG_NO | CString |
| START_POS / START_LOCATION | CString |
| DEST_POS / DEST_LOCATION | CString |
| TURN | CString |
| JOB_TYP | CString |
| BCR_TOP / BCR_BOTTOM | CString |
| JOB_KIND | CString |
| JOB_STATUS | CString |
| JOB_PRIORITY | CString |
| MES_ERROR_CD | CString |
| TRAY_TYP / TRAY_LEV | CString |
| DURATION_TIME | CString |
| PRODUCT_SIZE | CString |
| COPY_YN | bool |

> SQL에서 추가 참조되나 클래스엔 없는 컬럼: `JOB_START_DT`, `INS_DT`, `INS_USER_ID`. HostSim_DB SQL에선 `JOB_STA`도 관측.

### 5.2 `CELL_DTL` — 셀(랙 로케이션) 상세 · `CCELL_DTL`(`CELL_DTL.h`)

| 컬럼 | 타입 |
|---|---|
| WH_TYP | CString |
| CELL_NO | CString |
| CELL_SC_NO | CString |
| BANK / BAY / LEV | CString |
| AGING_START_DT / AGING_END_DT | CString |
| INS_DT / INS_USER_ID | CString |
| UPD_DT / UPD_USER_ID | CString |
| BOTTOM_TRAY / TOP_TRAY | CString |
| USER_REMARKS / REMARKS | CString |

### 5.3 `CELL_MST` — 셀 마스터 (래퍼 없음, SQL 컬럼)
`GetSelQuery_CELL_MST()`(`CollectDataList.cpp:187`)의 `CELL_MST CM LEFT OUTER JOIN CELL_DTL` 기준:

`WH_TYP, CELL_SC_NO, CELL_NO, BANK, BAY, LEV, GDBOX_NO, GDBOX_RACD, FIRE_DETECTOR_NO, CELL_FIRE_YN, CELL_STA, CELL_USE_DEF, CELL_USE_YN, AGING_TYP, CELL_TYP, BOTTOM_TRAY, TOP_TRAY, UPD_DT, USER_REMARKS, REMARKS, CELL_STATUS`

### 5.4 `CV_DATA` — 컨베이어 상태 · `CCV_DATA`(`CV_DATA.h`)
키: `WH_TYP, PLC_NO, TRACK_NO`. 대부분 `CString`, 단 `READ_UPD_DT / WRITE_UPD_DT / OD_UPD_DT`는 **`CTime`**.

주요 컬럼:
`LUGG_NO_RD/OD, DEST_POS_RD/OD, JOB_TYP_RD/OD, PULP_SENSOR_RD/OD, TRAY_LEV_RD/OD, TRAY_TYP_RD/OD, ERR_RQ_RD/OD, AUTO_MODE_RD, STO_READY_RD, RET_READY_RD, STOHS_READY_RD, RETHS_READY_RD, RTV_DEPARTHS_READY_RD, RTV_ARRIVEHS_READY_RD, LGV_DEPARTHS_READY_RD, LGV_ARRIVEHS_READY_RD, SENSOR0~4_DATA_RD, RTV_IN_SIGN, RTV_OUT_SIGN, RTV_LOCK_SIGN, MES_ERROR_CD, WAIT_SC_RET_JOB_RD(_NM), WAIT_SC_RET_JOB_OD, ERROR_CODE, OD_RQ_YN, OD_USER_ID, FLOOR_NO, BCR_BOTTOM, BCR_TOP, USE_YN, RET_HS_YN, SUSPEND, OD_RQ_FLAG, LIFTER_HS_YN, DP_HS_YN, MG_HS_YN, LOOP_NM, DESTINATION_YN, CMD_RQ_ID, CMD_RQ_YN, COMP_MG, CV_BCR_BOTTOM(하단 바코드), CV_BCR_TOP(상단 바코드), COMP_DP, MES_SEND_YN, MES_ERR_SEND_YN, WAIT_TIME_RD, MC_NO(_NM), TR_PAUSE_RD(_NM)/OD, STOCK_MODE, REMOTE_CONTROL, ROLL_MODE, EQP_TIME, EQP_CONNECTED_YN, EQP_COLOR, CHK_BYPASS_YN, ITEM_NO, DRIV_PAPER_POS, ELEV_ASC_ERR, ELEV_DESC_ERR, CLAMP_FORWARD/BACKWARD_ERR, DRIV_FORWARD/BACKWARD_ERR, PAPER_BLOCK_SENSOR1~4, PAPER_FULL_SENSOR, DRIV_FORWARD/BACKWARD_POS, CRUSH_PAPER_SENSOR, CLAMP_FORWARD/BACKWARD_SENSOR, REV_ELEV_POS_DATA_*, REV_DRIV_POS_DATA_*, REV_SENSOR_DATA_HEIGHT, ROLL_ELEV_POS_DATA_*, ROLL_CLAMP_POS_DATA_*, ROLL_SENSOR_DATA_HEIGHT, SC_PLT_JOB_TYP`
(설비 사양에 따른 센서/위치 컬럼 다수)

### 5.5 `SC_DATA` — 스태커크레인 상태 · `CSC_DATA`(`SC_DATA.h`)
키: `WH_TYP, PLC_NO, SC_NO`. `READ_UPD_DT / WRITE_UPD_DT / OD_UPD_DT`는 **`CTime`**, 나머지 `CString`.

주요 컬럼:
`SC_VIEW_NM, SC_GRP_NO, ONLINE_MODE_RD, AUTO_MODE_RD, SENSOR_FK_RD, UCSTATUS_RD, POS_H_RD, POS_V_RD, ERR_CODE_RD, ERR_STA_FK1/2_RD, ACTIVE_MODE_RD, COMPLETE_RD, JOB_TYP_RD/OD, LUGG_NO_FK1/2_RD/OD, START_BANK/HSPOS/BAY/LEVEL_FK1/2_RD/OD, DEST_BANK/HSPOS/BAY/LEVEL_FK1/2_RD/OD, USE_FK_RD/OD, WRITE_CONTINUE_OD, WRITE_FLAG_OD, USER_COMMAND_OD, OD_RQ_YN, OD_USER_ID, FORKPOS_FK1/2_RD, CV_WORKBENCH_RD(_SUB), SC_TYP, SUSPEND, ITN_LUGG_FK1/2, PLT_INFO_RD, OD_RQ_FLAG, CMD_RQ_ID, CMD_RQ_YN, MES_SEND_YN, MES_ERR_SEND_YN, MC_NO(_NM), EQP_TIME, EQP_CONNECTED_YN, EQP_COLOR`

### 5.6 `RTV_DATA` — RTV(레일 반송차) 상태 · `CRTV_DATA`(`RTV_DATA.h`)
키: `WH_TYP, PLC_NO, RTV_NO`. `READ_UPD_DT / WRITE_UPD_DT / OD_UPD_DT`는 **`CTime`**, 나머지 `CString`.

주요 컬럼:
`AUTO_MODE_RD, SENSOR_FK_RD, UCSTATUS_RD, POS_H_RD, POS_V_RD, FORKPOS_FK1/2_RD, ERR_CODE_RD, ACTIVE_MODE_RD, COMPLETE_RD, JOB_TYP_RD/OD, LUGG_NO_FK1/2_RD/OD, DEPARTHS_NO_FK1/2_RD/OD, ARRIVEHS_NO_FK1/2_RD/OD, USE_FORK_RD, USE_FK_OD, USER_COMMAND_OD, OD_RQ_YN, OD_USER_ID, ITN_LUGG_FK1/2, ITN_ARR_HS_MC_NO_FK1/2, CMD_RQ_ID, CMD_RQ_YN, MES_SEND_YN, MES_ERR_SEND_YN, SUSPEND`

### 5.7 `WC_DATA` — 중량검사기 상태 · `CWC_DATA`(`WC_DATA.h`) — 전부 `CString`
키: `WH_TYP, PLC_NO, WC_MC_NO, WC_NO`.
`WEIGHT_READ_STA(_NM), WEIGHT_RCV_VAL, CMD_RQ_ID, CMD_RQ_YN, SUSPEND(_NM), EQP_TIME, EQP_CONNECTED_YN, EQP_COLOR, CHK_BYPASS_YN(_NM)`

### 5.8 `BCR_MST` — 바코드리더 마스터 · `CBCR_MST`(`BCR_MST.h`) — 전부 `CString`
`WH_TYP, BCR_NO, BCR_MC_NO, UPD_DT, UPD_USER_ID, USE_YN, PLC_IP, PLC_PORT, SOCK_TIMEOUT, RETRY_YN, CONNECTED_YN, BCR_STA, BCR_LEV, NOREAD_CNT, CMD_RQ_ID, CMD_RQ_YN, SUSPEND`

### 5.9 `EQP_ECD_MST` — 설비 에러코드 마스터 · `CEQP_ECD_MST`(`EQP_ECD_MST.h`)
키: `EQP_TYP, ERROR_CODE`. 다국어 메시지/조치는 `CStringArray`로 보관.
- DB 컬럼: `EQP_TYP, ERROR_CODE, ERR_LEV(int)`, 메시지 `MSG_KOR/MSG_ENG/MSG_CHIN/MSG_HUN`, 조치 `ACTION_KOR/ACTION_ENG/ACTION_CHIN/ACTION_HUN`.
- 참조: `CvSkinDlg.cpp:2298`, `RtvSkinDlg.cpp:1003`, `ErrorMst.cpp:34`.

---

## 6. 래퍼 없는 테이블 (SQL에서 관측된 대표 컬럼)

> 이 테이블들은 전용 C++ 클래스 없이 인라인 SQL/`GetItem`으로 접근. 컬럼은 관측된 **대표값**으로 완전한 DDL은 아님.

| 테이블 | 대표 컬럼 (관측) |
|---|---|
| `EQP_MST` | PLC_NO, WH_TYP, EQP_TYP, EQP_NO, CONNECTED_YN, UPD_DT, PLC_IP, PLC_PORT |
| `COMMON_CODE` | CCD_CD, CDX_CD, CCD_CD_YN, CCD_KOR, CCD_NM_KOR/ENG/HUN/CHIN, CCD_NM_VALUE |
| `CHG_LANG` | TO_LANG_*, MSG_KOR/ENG/HUN/CHIN, ACTION_KOR/ENG/HUN/CHIN |
| `USER_MST` | USER_ID, (비밀번호/이름/권한 등) |
| `USER_GRP` | GRP_LEVEL 등 |
| `USER_GRP_WIN` | WIN_ID, WIN_VIEW_YN 등 |
| `HOST_EMPTY_PLT` | (공파렛트 정보) |
| `CELL_MES_REQ` | (셀 MES 요청) |
| `DEL_HIS_SETTING` | (이력 삭제 보존기간 설정) — `ConfigLogDelete.cpp:407,464` SELECT/UPDATE |
| `SC_HS_DEF` | (SC 핸드셰이크 위치) |
| `DEST_POS_DEF` | (목적지 위치 정의) |
| `CV_DEF_INF` | (컨베이어 정의) |
| `JOB_MST_HIS` | JOB_MST 이력(동일 컬럼군 + 이력키) |
| `WC_HIS` / `EQP_ERR_HIS` | 각 데이터/에러 이력 |
| `WCS_CLIENT_LOG` | 클라이언트 로그 — `EcsDoc.cpp` INSERT |
| `WCS_LOG_PGR` | 프로그램 로그 |
| `MES_IF_LOG` / `HOST_IF_LOG` | 인터페이스 송수신 로그 |
| `IF_LUGG_STA` / `IF_LUGG_STA_HIS` / `IF_MC_STA` | HostSim_DB 화물/설비 상태 인터페이스 |

---

## 7. 테이블별 CRUD 접근 위치 (Ecs 기준, file:line)

### `BCR_MST`
- SELECT: `Bcr.cpp:104,113`, `BCRSkinDlg.cpp:635,637`
- UPDATE: `BCRSkinDlg.cpp:375,461,529,559,611`, `EqpSuspendDlg.cpp:650`

### `CELL_DTL` (+ `CELL_MST` 조인)
- SELECT: `CollectDataList.cpp:231`(조인), `ViewRackDlg.cpp:1383,1409,1746`
- INSERT: `ViewRackDlg.cpp:471`
- UPDATE: `ViewRackDlg.cpp:730`
- DELETE: `ViewRackDlg.cpp:905,1392,1418`

### `CV_DATA`
- SELECT: `Cv.cpp:128`, `CvSkinDlg.cpp:1547,1971,2130,2342,2552,2743,2930,2952`, `EcsView.cpp:1328`, `Lib.cpp:896`, `ManualJob.cpp:1188`, `ScManualRet.cpp:389`, `WcSkinDlg.cpp:540,717`, `ViewSearchDlg.cpp:305,321,339`
- UPDATE: `CvSkinDlg.cpp:1161,1216,1273,1500,1635,1735,1776,1852,2016,2040,2151,2165,2232,2273,2661,2760,2846,3185`, `BCRSkinDlg.cpp:434,440,445`, `ScSkinDlg.cpp:2095`, `ScManualRet.cpp:299,414`, `EqpSuspendDlg.cpp:634`

### `EQP_ECD_MST`
- SELECT: `CvSkinDlg.cpp:2299`, `RtvSkinDlg.cpp:1004`, `ScSkinDlg.cpp:1526`, `ErrorMst.cpp:34`, `LogMesSkinDlg.cpp:959`, `LogEqpErrHisSkinDlg.cpp:651`(조인)

### `JOB_MST`
- SELECT: `CollectDataList.cpp:373`, `CvSkinDlg.cpp:1326,1392,1548,1904,2504`, `ManualJob.cpp:797,995`, `ScSkinDlg.cpp:1024,1682,1713,1990`, `ScManualRet.cpp:462,570`, `RtvSkinDlg.cpp:1050,1065`, `ViewSearchDlg.cpp:339`, `ViewSemiJobDlg.cpp:180`, `SystemScTimeDlg.cpp:265`, `ViewJobListDlg.cpp:1003`
- INSERT: `CvSkinDlg.cpp:2063`, `ManualJob.cpp:708`, `ScSkinDlg.cpp:2052`, `ScManualRet.cpp:524`, `ViewRackDlg.cpp:1190,1228`
- UPDATE: `CvSkinDlg.cpp:1338,1404,1564,1920`, `ManualJob.cpp:860`, `ScSkinDlg.cpp:1468`, `ScManualRet.cpp:585`, `ViewJobListDlg.cpp:397,492`
- DELETE: `ManualJob.cpp:881`, `ScSkinDlg.cpp:1034,2004`, `ScManualRet.cpp:476`, `ViewHostEmptyPltDlg.cpp:643`, `ViewJobListDlg.cpp:688`

### `RTV_DATA`
- SELECT: `RtvSkinDlg.cpp:870,1050,1065,1090`, `Lib.cpp:1537`
- UPDATE: `ManualRtv.cpp:341,361,382`, `RtvSkinDlg.cpp:755,895`, `EqpSuspendDlg.cpp:642`

### `SC_DATA`
- SELECT: `Sc.cpp:105`, `ScSkinDlg.cpp:1458,1610,1745`, `ManualSc.cpp:507`, `Lib.cpp:1488`, `SystemScTimeDlg.cpp:264`
- UPDATE: `ScSkinDlg.cpp:1053,1587,1939`, `ManualSc.cpp:461,535`, `ScManualRet.cpp:436`, `EqpSuspendDlg.cpp:659`

### `WC_DATA`
- SELECT: `Wc.cpp:99`, `CvSkinDlg.cpp:2980`
- UPDATE: `CvSkinDlg.cpp:3071`, `WcSkinDlg.cpp:393,478,587,650,762`

### `EQP_MST` / `COMMON_CODE` / 로그 테이블
- `EQP_MST` SELECT: `ConnectStatus.cpp:29`, `Cv.cpp:129`(서브쿼리), `Sc.cpp`, `Wc.cpp`, `EqpSuspendDlg.cpp`, `MainFrm.cpp`, `LogEqpSkinDlg.cpp`
- `COMMON_CODE` SELECT: `URMDBAccess::MakeComboWithCode`(`URMDBAccess.cpp:1163`) 외 다수
- 로그: `WCS_CLIENT_LOG` INSERT(`EcsDoc.cpp`), `WCS_LOG_PGR`(`LogWcsLogPgr.cpp`), `MES_IF_LOG`/`HOST_IF_LOG`(`LogMesSkinDlg.cpp`)

---

## 8. 레거시 스키마 (참고 — `URMDBAccess.cpp` 내 전량 주석처리)

구 Oracle/STKCIM 계열. **현재 빌드에서 실행되지 않음.** 향후 재활성/이관 판단용 참고.

| 테이블 | 대표 컬럼 |
|---|---|
| `LocMast` | Loc, WhZone, SHIPID, FOSBID, RackStat, OldRackStat, WmsJob, InDateTime, JobjisiDate, JobNo, RackGbn, StockGbn, Qty, CSTID, LotID, MSCODE, Descript1/2, CSTType, CSTStat, CarStat, EmptyFlag, Cleaned, Remark, UDate, UUser |
| `JobInfo` | JobNo, CSTID, IOGbn, ToLoc, SendGbn, ToPortNo, WmsJob, WaitCheck, ECSErrCode, JobOrderTime |
| `PortInfo` | PortNo, PortType, PortGbn, PortState, JisiGbn, PortDesc, WaitTray, UDate, UUser |
| `ScInfo` | ScNo, ScStatus, ScCut |
| `ZoneInfo` | ZoneNo, ZoneName, V_ZoneName, ZoneType, ZoneUseGbn |
| `TaskInfo` | TaskID, TaskStatus |
| `Code` / `CodeGroup` | Code_Group, Code_No, Code_Name |
| `WhInfo` | WhNo, WhDesc |
| `Users` | LoginID |
| `PgmMast` | PgmID |
| `Dual` | (Oracle 의사 테이블) SysDate, SysTimeStamp |

주석 프로시저: `Sp_Logging`, `Sp_RackUpdateProc`.

---

## 9. 요약 및 유의사항

1. **활성 DB는 PostgreSQL 단일** (Ecs=`LFC/LFC/LFC`, HostSim_DB=`HUONS/HUONS/HUONS`). Oracle·STKCIM 코드는 모두 dead.
2. **저장 프로시저는 실사용 없음** — 문자열 SQL 직접 실행이 전부. `MANUAL_SEQ` 시퀀스만 함수 호출 형태로 사용.
3. **컬럼 확정 범위**: 8개 래퍼 클래스(§5)는 소스 멤버로 컬럼이 확정적. 그 외 테이블(§6)은 SQL/`GetItem`에서 관측된 대표 컬럼이라 실제 DDL과 차이 가능 → 정확한 스펙은 **실 DB 스키마(PostgreSQL `LFC`/`HUONS`) 덤프와 대조** 권장.
4. **개선 후보(별건)**: 평문 자격증명, 파라미터 미바인딩 SQL(인젝션), Oracle/레거시 dead code 정리.
