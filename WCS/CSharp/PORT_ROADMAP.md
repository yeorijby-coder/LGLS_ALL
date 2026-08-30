# C# WPF ↔ C++ ECS 클라이언트 정렬 로드맵

기준: C++ 레퍼런스 `D:\project\LGLS\Renewal\WCS\CPlusPlus\LGLS_CLIENT\Ecs`
대상: C# WPF `D:\project\LGLS\Renewal\WCS\CSharp\Ecs`

작성: 2026-08-14 (자동 진행 세션)

---

## 2026-08-14 COMMON_CODE 코드→한글 변환 완료 ✅ (런타임 검증됨)

- **`Ecs\Db\CommonCode.cs`** — COMMON_CODE(cdx_cd/ccd_cd/wh_typ/ccd_nm_kor) 캐시 + `GetName(분류,코드,창고)`. C++ `CLib::GetCommonCodeLang` 대응.
- **`EcsDb.LoadCommonCodes`** — 621건 로드. **`EcsDoc`**: 시작 시 DB 접속되면 자동 로드, 헬퍼 `JobTypName/JobStatusName/WhTypName`.
- **`JobItem`**: `RawJobTyp/RawJobStatus`(DB 원본코드) + `JobTypeName/StatusName`(변환 한글). `LoadJobsFromDb`가 변환 채움.
- **`JobListView`**: 유형/상태 컬럼을 **한글명**으로 표시(DB 로드 시), 출발/도착은 로케이션 문자열.
- **정규화**: WH_TYP의 wh_typ 값에 콤마("10,")가 섞여 있어 `NormWh`로 보정.
- **검증**: JOB_TYP 1→`[01]입고`/2→`[02]출고`/6→`[06]작업대 이동`, JOB_STATUS 99→`신규`/29→`SC 구동완료`, WH_TYP 10→`LG 생명과학 1동` 실측 확인.

---

## 2026-08-14 DB 실연동 완료 ✅ (런타임 검증됨)

- **DB 계층** `Ecs\Db\EcsDb.cs` — MS SQL Server(`System.Data.SqlClient`). JOB_MST INSERT/SELECT/DELETE/완료(UPDATE), **파라미터 바인딩**(C++의 %s 결합보다 안전).
- **설정** `EcsConfig.xml` `<Db server database user password whtyp/>` (기본 = C++ Ecs.ini `[DB_2]`: `localhost\SQLEXPRESS` / `LGLS_MCS_IO` / `LGLS_IO`). `EcsConfig.cs`가 파싱.
- **연동** `EcsDoc`: 시작 시 DB 접속 시도(실패해도 인메모리로 계속). `AddJob` → JOB_MST INSERT(JOB_STATUS='99' 신규, INS_DT=GETDATE). `LoadJobsFromDb()` → JOB_MST SELECT.
- **UI**: 수동 작업 창의 [작업 추가]가 실제 JOB_MST에 INSERT / 리본 ECS▸작업정보가 JOB_MST에서 로드. ManualJob 창에 DB 연결상태 표시.
- **검증**: 이 PC의 `localhost\SQLEXPRESS/LGLS_MCS_IO`에 **실접속 성공**, 실제 INSERT(typ=1/status=99/로케이션)·SELECT·DELETE 동작 확인(테스트행 정리 완료). JOB_TYP 1=입고/2=출고, JOB_STATUS 99=신규.
- 남음: JOB_MST 외 테이블(CV/SC/RTV_DATA, COMMON_CODE 코드→한글), 조회 필터·페이징, 자격증명 보안(현재 평문).

---

## 2026-08-14 추가 구현 완료 (로드맵 2~7) ✅

- **수동 JOB** (`ManualJobDialog`) — 리본 MANUAL▸작업. 창고/유형/번호/수량/출발·목적 로케이션 → `AddJob` 투입.
- **반자동 TEST** (`SemiTestDialog`) — 리본 MANUAL▸반자동TEST. 다중 라인이 입고↔출고 사이클 자동 반복(타이머), 실시간 로그.
- **수동 SC** (`ManualScDialog`) — 리본 MANUAL▸크레인. 입고/출고/홈/취소 명령 발행(`Sc.Store/Retrieve/Home/Cancel`). **RTV**는 상태 표시(Rtv.cs 명령계층 미구현).
- **로그 5종 분리** — `LogView.SetCategoryFilter` + LOG 탭 5버튼(작업/HOST/설비에러/유저/ECS 카테고리 필터).
- **찾기** (`FindDialog`) — 리본 ECS▸찾기. 레이아웃 컨트롤 ID/라벨 검색. **공PLT작업**은 작업목록 이동+안내.
- **환경설정** — 통신연결정의(설비 연결상태 표시), 로그삭제설정(로그 삭제), 범례(상태색상 안내), 사용자(인증계층 필요 안내).
- **창고 모니터링 트랙 텍스트 모드** — `DciMaster.TrackTextMode`(0/1/2) + `DciTrackCtrl`이 작업번호/트랙번호/제품정보로 렌더링. 리본 ECS▸작업번호/트랙번호/제품정보 버튼으로 전환.

전 항목 빌드 정상. 남은 심화: RTV 명령계층(Rtv.cs), 사용자/권한, DB(JOB_MST 등) 실연동, 공PLT 전용 조회.

---

## 이번에 완료한 것 (✅)

1. **불필요 폴더 정리** — `CSharp\LGLS_CLIENT`(C++ 복사본 1.7GB)를 `Renewal\WCS\_TRASH_CSharp_LGLS_CLIENT_20260814`로 이동.
   (이 환경은 `D:\project` 하위 영구삭제가 차단되어 이동 처리. 잠긴 폰트 `Arial.ttf` 1개만 원위치 잔존 — 재부팅 후 삭제 가능.)
2. **상단 UI를 C++ 리본 구조로 교체** — 기존 메뉴바 → **ECS / MANUAL / LOG** 탭 리본.
   - ECS: 환경설정(통신연결정의·로그삭제설정·범례·사용자) / 뷰(작업정보·공PLT작업·찾기) / 창고 모니터링(작업번호·트랙번호·제품정보)
   - MANUAL: 수동조작(작업·크레인·RTV) / 반자동 TEST(반자동 TEST·TEST 초기화)
   - LOG: 로그(작업로그·HOST로그·설비에러이력·유저사용로그·ECS프로그램로그)
   - 파일: `Ecs\Views\MainWindow.xaml`(리본), `MainWindow.xaml.cs`(Rb_* 핸들러), `EcsDoc.cs`(`TrackTextMode` 추가)
3. C# 솔루션 빌드 정상(Debug).

리본 라벨/구조는 C++ 원본(`MainFrm.cpp` AddCategoryWCS/MANUAL/LOG + `rc_resource\mainframe_*\*.ini` `[kor]`)과 일치시켰습니다.

---

## 리본 버튼 → C++ 동작 → C# 현황/할 일

| 탭·그룹 | 버튼 | C++ 여는 창/동작 | C# 현재 연결 | 구현 필요 |
|---|---|---|---|---|
| ECS·환경설정 | 통신 연결 정의 | `CEqpSuspendDlg` (설비 통신 on/off) | 안내(스텁) | 설비 통신정의 다이얼로그 |
| | 로그 삭제 설정 | `CConfigLogDelete` | 안내(스텁) | 로그 보존기간 설정 다이얼로그 |
| | 범례 | `CConfigStatus` (설비 상태 범례) | 안내(스텁) | 범례 다이얼로그 (EquipmentStatusPanel 확장 가능) |
| | 사용자 | `CUserUserDlg` (권한/사용자 관리) | 안내(스텁) | 사용자·권한 관리 |
| ECS·뷰 | 작업정보 | `CViewJobListDlg` | ✅ **Job 목록 탭 연결** | (JobListView 기능 확장) |
| | 공PLT작업 | `CViewHostEmptyPltDlg` | 안내(스텁) | 공PLT 작업 조회/생성 |
| | 찾기 | `CViewSearchDlg` | 안내(스텁) | 트랙/화물 검색 |
| ECS·창고모니터링 | 작업번호/트랙번호/제품정보 | `OnCommandTrackTextMode` (트랙 표시 모드 0/1/2) | ✅ **`TrackTextMode` 설정** | DciTrackCtrl이 모드에 따라 다른 텍스트 렌더링하도록 반영 필요 |
| MANUAL·수동조작 | 작업(JOB) | `CManualJob` (반자동 작업 생성 입고/출고/R2R/이동) | 안내(스텁) | **핵심**: 수동 작업 생성 창 |
| | 크레인(SC) | `CManualSc` | 안내(스텁) | SC 수동 조작 창 |
| | RTV | `CManualRtv` | 안내(스텁) | RTV 수동 조작 창 |
| MANUAL·반자동TEST | 반자동 TEST | `CSemiTestDlg` (HOST 대신 입출고 사이클 자동반복) | 안내(스텁) | 반자동 테스트 창 (5행 루프 + 로그) |
| | TEST 초기화 | JOB_MST DELETE + CV/SC/RTV_DATA UPDATE | ✅ **작업 초기화 연결** | DB 연동 시 실제 초기화 쿼리 반영 |
| LOG | 작업로그(IO) | `CLogIoSkinDlg` | ✅ 로그 탭 | 로그 종류별 분리 뷰 |
| | HOST로그(MES) | `CLogMesSkinDlg` | 안내(스텁) | |
| | 설비에러이력 | `CLogEqpErrHisSkinDlg` | 안내(스텁) | |
| | 유저사용로그 | `CLogClientSkinDlg` | 안내(스텁) | |
| | ECS프로그램로그 | `CLogWcsLogPgr` | ✅ 로그 탭 | |

---

## 권장 구현 순서 (우선순위)

1. **수동조작 JOB (`CManualJob`)** — 가장 많이 쓰는 기능. 입고/출고/R2R/이동 탭 + 작업 생성. (반자동 TEST의 기반)
2. **반자동 TEST (`CSemiTestDlg`)** — HOST 없이 입출고 사이클 자동 검증. 개발/테스트 필수.
3. **수동 SC / RTV (`CManualSc/CManualRtv`)** — 설비 수동 조작.
4. **로그 5종 분리 뷰** — IO/MES/EQP_HIS/CLIENT/WCS. 현재 단일 LogView를 종류별로.
5. **뷰: 공PLT작업, 찾기** — 조회 다이얼로그.
6. **환경설정: 통신연결정의, 로그삭제설정, 범례, 사용자** — 설정/관리 다이얼로그.
7. **창고 모니터링 트랙 텍스트 모드** — `DciTrackCtrl`이 `TrackTextMode`(0/1/2)에 따라 작업번호/트랙번호/제품정보를 렌더링하도록.

각 다이얼로그의 정확한 필드/DB 쿼리는 C++ 원본 대응 클래스(`ManualJob.cpp`, `SemiTestDlg.cpp`, `ViewJobListDlg.cpp` 등)를 참조.

---

## 주의사항

- **C# 소스(.cs/.xaml)는 UTF-8** → 일반 편집 안전.
- **C++ 소스와 `EcsLayout.xml` 등은 EUC-KR(cp949)/MBCS** → 편집 시 반드시 cp949 인코딩 명시(일반 UTF-8 편집은 한글을 U+FFFD로 손상시킴).
- 실제 DB(JOB_MST/CV_DATA/SC_DATA_LGLS/RTV_DATA_LGLS) 연동은 C++가 ADO(`CURMDBAccess`)를 쓰며, C# 포트의 DB 계층 구현 상태에 맞춰 진행 필요.
