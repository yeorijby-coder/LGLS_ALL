# ECS 대체 시스템 연동 작업 기록 (2026-07-11)

대체 체인: **HOST_SIM(WMS) ⇄ WCS_TASK_HOST ⇄ [공통 DB] ⇄ IO_TASK ⇄ [공통 DB] ⇄ WCS_TASK_CV ⇄ EQP_SIM(PLC)**

공통 DB (§5에서 전환):
- (구) PostgreSQL 16 `localhost:5432 / LGLS_TEST / LGLS_TEST / LGLS_TEST` (pg_hba=password 평문,
  LGLS_TEST 비번을 MD5 재해시 — 구형 Npgsql 2.2.7 호환 목적)
- (현) **SQL Server 2008 `localhost\SQLEXPRESS / LGLS_MCS_IO / LGLS_IO / LGLS_IO`**

## 1. WCS_TASK_CV ↔ EQP_SIM  (완료 · 실검증)
수정 (C:\Users\USER\Downloads\WCS_TASK_CV):
- `7_Socket\FenetProtocol.cs` — [V1.5] 디바이스 문자를 논리 영역별로 전송 (%MB/%DB/%RB).
  (V1.4의 "항상 %MB"는 ECS 오독 — ECS는 연속 R/W에 영역 문자를 구분함)
- `2_Thread\EQP_THREAD\CvThread.cs`
  - `GetRTrackingAddr`: R 트래킹 실주소 = **hex((CV번호-1)*10 표기)** (CV#11 "0100"→0x100).
  - `EncodeJobNoR`: 십진 BCD → **ASCII 4문자 팩** (ECS WriteWordA와 동일; "2143"은 워드 표기 착시).
- `bin\Debug\WCS_DB.INI` — COMM0 포트 3001, **PLC_NO=11** (C/V#11 게이트웨이).
- DB — `cv_data.plc_no '01'→'11'` (88트랙).
검증: EQP_SIM(3001) 접속 유지, C/V#11 Load Complete#2(%MX580) 감지→Ack(%MX1444) 기록,
EQP_SIM 핸드셰이크 정상 클리어(타임아웃 소멸). 단독테스트로 D/M/R 읽기(800워드 포함) 전건 성공.
진단 이력: LGLS_TEST 롤 비번 SCRAM→MD5 재해시(Npgsql 2.2.7), 로그는 wcs_log_pgr 로 확인.

## 2. WCS_TASK_HOST ↔ HOST_SIM  (완료 · 실검증)
수정 (D:\project\LGLS\WCS_TASK_HOST):
- `bin\Debug\EcsComA.ini` — LocalPort=**9911**, RemotePort=**9910**, [DB]→LGLS_TEST.
- `modDefApp.cs` — `MSG_ORDER_BODY_CNT 104→66`, `MSG_MODE_BODY_CNT=11` 추가.
- `CSrvWork.cs`
  - ParseOorR: O 전문을 **LGLS 66B 규격**으로 재배치 (WhDefine 없음, PalletNo 7자,
    LuggNo@3, ProductID@45). R 재작업은 기존 유지.
  - "Convert S/C No"에 **하이랙 코드 201~205 → 크레인 901~905** 매핑 추가.
  - `case "M"` 추가(`ParseModeChange`) — C/V#11 모드변경 수신·ACK.
  - 입고/출고 검증을 `IsValidStationLgls`(1xx=cv_data, 9xx=sc_data 존재 확인)로 교체.
    기존 IsValidStation(STN_KIND enum)·IsValidLocation(CELL_MST)은 LFC 체계라 미사용
    (CELL_MST 자체가 없음 — 셀 관리 도입 시 복원 지점 주석).
- `CCliWork.cs` — F 작업완료를 **LGLS 10B 규격**(F+JobDefine+LuggNo4+Class+Step)으로
  (WhDefine/DeviceNo 제거, 출고 최종=Step '2').
- HOST_SIM 측: `ReportServer.SendAck` 추가 — 수신 S/E/F에 규격 ACK(11B) 회신
  (원본 ECS는 응답을 무시하므로 원본 호환 유지, WCS의 응답 대기 해소).
- DB 시드: `host_if_log` 테이블 생성, `eqp_mst`에 EQP_TYP='HOST2' 행,
  `sc_data`에 SC 903~905 추가, `cv_data`에 comp_vr(기본 'Y') 컬럼·`wc_data` 테이블
  (S 상태보고 쿼리 요건).
검증: 첫 S 수신→[시작] 활성화, 3개 로직 O 전건 ACK + job_mst(JOB_STATUS 99) 생성,
M ACK, E/S 보고 왕복(host_if_log 양방향 기록).
잔여: E 전문은 WhDefine 포함 24B(원본 23B — HOST_SIM은 로그만 하므로 무해),
S 는 설비별 개별 전문(원본 43B 집계와 상이 — HOST_SIM은 타입만 판정하므로 무해).

## 3. IO_TASK  (완료 · 실검증)
수정 (C:\Users\USER\Downloads\IO_TASK):
- `IO_TASK_SEMI_FINISH.csproj` — Debug|x86 을 SQL→**POSTGRESQL** 로 전환,
  `cCbBasePost.cs`/`cDbPostUse.cs` 의 Npgsql 별칭 오류 수정(잠재 컴파일 버그).
- `bin\Debug\ENV_IOSCH.INI` — [P_DB]→LGLS_TEST, `[CNF] SC_AUTO_COMPLETE=1` 추가.
- `CLS\Thread\cThread_SCH.cs` — [LGLS] 표기:
  - `AcceptNewJob()`: JOB_STATUS '99' → 입고 '10'(CV 레그) / 출고 '20'(SC 레그) 접수.
  - CV/SC 레그의 설비 위치식(입고=START_POS/출고=DEST_POS)·**설비 키를 MC_NO 로 통일**
    (TRACK_NO 는 '1'+MC_NO 표기용), 발행 시 `TRACKING_WRITE_YN='Y'` 세팅.
  - 멀티레그 핸드오프: 입고 CV완료→'20'→SC→**'29'**(F 발신 트리거),
    출고 SC완료→'10'→CV→**'19'**. (WCS_TASK_HOST GetJobCompleteReport 는 19/29 를 F 로 보고)
  - `AutoRunSC()`: SC PLC 게이트웨이 부재 시(SC_AUTO_COMPLETE=1) SC 레그를
    타이머 자동완주(20→21→25→최종, ~5초)로 시뮬레이션. RGV 레그는 체인 외(주석).
- 검증: O 접수→99→10→CV 발행(od_rq_yn/tracking_write_yn)→WCS 트래킹 기록(R)→
  EQP_SIM JOB 부여·이동→완료→SC 자동완주→'29'→F(입고)→HOST_SIM 출고지시→
  출고 SC→CV→'19'→**F(출고 step2)** — 로직3 기준 입고+출고 풀사이클 완주 실증.

## 3.5 설비 마스터 시드 (원본 ECS 분석 → LGLS_TEST, 재고 제외)
- `db\seed_replacement_equipment.sql` — 원본 TB_EQUIPMENT/TB_LOCATION/TB_ITINERARY 근거:
  cv_data **TR#1~32** (MC_NO='1'+TR 2자리, stn_kind 1=입고대/2=출고대/3=겸용/0=라인,
  기존 88트랙 모델은 cv_data_bak_lgls 로 백업), sc_data 901~905(뱅크 2k-1,2k),
  rtv_data RGV#1, eqp_mst(CV 게이트웨이 11~15 + SC 5 + RTV, 재고성 BCR 제거),
  cv_def_inf(원본 반송 흐름 16), sc_def_inf(SC별 라인 CV/뱅크), dest_pos_def.
- 연동 정렬: HOST_SIM.ini 스테이션 = **TR 기반**(124/126, 130/129, 122/122 — 원본 ECS
  연동값 101~105 는 주석 병기), WCS_DB.INI **COMM0~4 = C/V#11~15**(PLC_NO=CV번호,
  FR/TO_TRACK=담당 TR), EQP_SIM.ini [WCS_MIRROR] CV11=121…CV15=130.
- EQP_SIM 추가 훅: ① D영역 트랙 미러(MC_NO 뒤2자리×10: LuggNum/Status/Sensor —
  WCS CvStatus 규격), ② 출고 트래킹 기록 시 **출고 파렛트 재현**(S/C 하역 시뮬레이션).
- WCS_TASK_CV: MakeMsg_Error 를 wcs_log_pgr 에도 기록(진단), cv_data 에
  pulp_sensor_od/wait_sc_ret_job_od 컬럼 보강(오더 쓰기 루프 요건).

## 3.6 재고 관리 제거 (WCS_TASK_HOST)
- 발신 제거: GetWeightReport(무게보고 U), GetEmptyPltRequest(빈파렛트 요청 P).
- 수신/완료 경로의 공파레트(HOST_EMPTY_PLT) 갱신 로직 제거.
- CELL_MST 기반 랙 셀 검증은 앞서 제거됨(§2) — 재고 미관리 방침과 일치.

## 5. DB 전환: PostgreSQL LGLS_TEST → SQL Server LGLS_MCS_IO (2026-07-11)
접속: `localhost\SQLEXPRESS`(SQL Server **2008** SP1) / DB `LGLS_MCS_IO` / 로그인 `LGLS_IO`/`LGLS_IO`.
세 TASK 모두 컴파일 상수(`SQL` ↔ `POSTGRESQL`/`POSTGRESSQL`)로 DB를 전환하는 구조 —
Debug 구성 DefineConstants 를 `SQL` 로 교체하고 INI 접속정보 변경. PG 모드도 컴파일 유지.

- **DB 시드**: `db\seed_replacement_equipment_mssql.sql` (pg 시드의 T-SQL 이식판)
  — cv_data 88트랙 백업(cv_data_bak_lgls) 후 TR#1~32 재구성, sc_data 901~905 보강,
  eqp_mst(HOST2 신규 + CV 11~15/SC/RTV), cv_def_inf/sc_def_inf/dest_pos_def,
  스키마 보강(comp_vr/pulp_sensor_od/wait_sc_ret_job_od 컬럼, wc_data/host_if_log 테이블),
  잔여 진행중 작업 status '9'. (기존 데이터 status 5/8/3 구 체계 500여건은 폴링 대상 아님 — 방치)
- **WCS_TASK_HOST**: csproj Debug `POSTGRESSQL`→`SQL`; EcsComA.ini [DB] IP=localhost\SQLEXPRESS 등;
  modCmLib SQL ReadInitProfile 이 [DB] 섹션을 직접 읽도록 수정(기존 WmsInfo.dll 매핑은
  LFC 운영 DB(10.99.10.137/WMSLFCKR10)라 사용 불가 — DATABASE 미설정 시 폴백 유지);
  CCliWork GetStatusReport 2곳 `#if SQL` 방언(DATEDIFF / CAST&1), frmMain LIMIT→MAX.
- **WCS_TASK_CV**: (에이전트 변환) 5_DB 클래스 DbConn 별칭화+ConvQry(`:p`→`@p`)+
  BeginTrans 시 `mComMain.Transaction=mTrnMain`(SqlClient 필수), DbLang SQL 분기 완성,
  SYS_MAIN SQL 접속문자열([DB_2]), FenetProtocol Open() SQL 겸용, LIMIT→TOP,
  wcs_log_pgr 시퀀스는 **SQL2008이라 NEXT VALUE FOR 불가 → ISNULL(MAX+1)** 로 수정.
- **IO_TASK**: csproj Debug|x86 `POSTGRESQL`→`SQL`; ENV_IOSCH.INI [P_DB] 변경;
  cCbBasePost **BeginTrans/Commit/Rollback 에 mComMain.Transaction 연계 추가**
  (미설정 시 "명령의 Transaction 속성이 초기화되지 않았습니다" 로 CV 발행 실패 — 실측 후 수정).
- **DriveCV 출고 결함 수정(IO_TASK, DB 무관 잠재 버그)**: `SENSOR0_DATA_RD='1'`(화물 감지)
  조건이 출고에도 걸려 있었음 — 출고는 S/C 하역 전이라 파렛트가 없고(트래킹 기록 후
  EQP_SIM 이 재현) 로직3(TR#122)만 자동투입 파렛트로 우연히 통과, 출고 전용대(126/129)는
  PG 시절에도 미발행(JOB 0019 미완료 흔적). → `(JOB_TYP='2' OR SENSOR0_DATA_RD='1')`.
- 검증: S 상태보고 왕복(host_if_log), CV 게이트웨이 5개 접속(connected_yn='Y',
  wcs_log_pgr MAX+1 정상), 로직1 기준 O 0021 접수→99→10→CV 발행→트래킹→F(입고 step1)
  →HOST_SIM 출고지시 0022→SC 자동완주→CV 레그→**F2002222(출고 최종 step2)**→
  다음 입고 0023 자동 발행 — 풀사이클 SQL Server 에서 완주.

## 6. 원본 ECS 설비 변수 ↔ 테이블 필드 정렬 (2026-07-11)
- 원본 ECS Device 계층 분석: CV=Conveyor 옵저버블 25종, RGV/S.C=**동일 Vehicle 인터페이스 29종**
  (VEHICLE:1=RGV, VEHICLE:11~15=S/C — 주소 블록만 상이). 주소 원본: EQP_SIM\observables.tsv.
- LGLS_MCS_IO 반영: `db\align_equipment_fields_ecs_mssql.sql`
  — cv_data +11 컬럼(direction_mode, wait_in/out, in_ready, load/unload_complete(+ack), unload_request(+ack)),
  rtv_data/sc_data 각 +25 컬럼(subsystem_status, location×3, pallet_on_vehicle, transfer_request/ack,
  pallet_id, from×3/to×3, load/unload_complete(+ack), alarm set/reset(report/ack/code)).
  기존 의미 일치 컬럼(auto_mode_rd, sensor0_data_rd, lugg_no_rd 등)은 재사용.
- 초기 데이터 입력 완료(TR#1~32, RGV#1, SC 901~905 — IDLE/자동/무알람, 입고대 in_ready='1').
- 상세 매핑: `docs\ECS_EQUIPMENT_FIELD_MAP.md`

## 7. WCS Client (D:\project\LGLS\CPlusPlus\LGLS_CLIENT\Ecs — MFC, VS2019 v142, Debug|Win32)
사용자 화면 시스템. LGLS_MCS_IO 로 전환·정합·기동 완료 (2026-07-12).
- **접속**: stdafx.h `POSTGRESQL 0 / MSSQL 1`, Config/CAdoDB `#elif POSTGRESQL || MSSQL`(+MSSQL 접속문자열
  `Driver={SQL Server}`), EcsDoc 방언 토큰(SYSDATE=GETDATE, NVL=COALESCE, TO_NUMBER=CAST),
  Ecs.ini [DB_2] = SQL Server / localhost\SQLEXPRESS / LGLS_MCS_IO / LGLS_IO. **CWD=Ecs 폴더로 실행**(INI 위치).
- **설비 정합**: CV 조회 부족 컬럼 6종은 DB 보강(`db\wcs_client_support_mssql.sql` — is_turn_rd,
  wait_time_rd, suspend, item_no, roll_elev_pos_data_asc, sc_plt_job_typ + wc_data 7컬럼 + suspend '0' 정규화
  + cell_mst 500셀/cell_dtl/bcr_mst/dual 뷰). RTV 조회는 실컬럼 별칭 매핑(pallet_on_vehicle_rd→LUGG_NO_FK1_RD,
  unload_complete_rd→COMPLETE_RD, subsystem_status_rd→UCSTATUS_RD, depart/arrive_track→DEPARTHS/ARRIVEHS),
  RTV 수동지시 UPDATE 는 depart/arrive_track·lugg_od·od_rq_yn 으로 재작성. SC 는 완전 정합. EQP_TIME 식 4곳 DATEDIFF.
- **방언 일괄 변환**(에이전트): TO_DATE→CONVERT(+STUFF 14자리), TO_CHAR→CONVERT 스타일, LIMIT→TOP
  (CLib::ApplyTopN 헬퍼), NEXTVAL('MANUAL_SEQ')→JOB_MST 9000번대 MAX+1, ||→+, SUBSTR→SUBSTRING, now()→GETDATE.
- **메인 화면 정의**: EcsDefine.xml 을 LGLS 구성으로 재생성(CV 15 PLC / TR#101~132 + 입출고대 스테이션,
  S/C 901~905 bank2×bay10×lev5). ※ EcsLayout1.xml(도면 그래픽)은 구 사이트 배치 그대로 — 별건.
  eqp_mst 에 라인 C/V 01~10 표시용 행 추가.
- **부수 대화상자**: host_empty_plt/cell_mes_req/mes_if_log/wc_his 테이블,
  cell_mst.fire_od_rq_yn/wc_data.od_rq_id 컬럼 생성. 사용자: LFC/LFC(자동 로그인·보기전용),
  LGLS/LGLS·HUONS/HUONS(관리) — user_mst.grp_cd 는 user_grp.grp_level 값('0')과 조인됨에 유의.
- **수정한 클라이언트 버그(기존 잠복)**: ① CString 자기참조 Format(EcsDoc InitializeErrorMst)
  → VS2019 UCRT 에서 AV — 임시변수 분리. ② XmlDom Get* null ASSERT 가 레이아웃 파싱을 모달로 블로킹
  → null 안전 반환(XmlLib 재빌드). ③ ManualRtv `'\\'` 문자 리터럴/Rtv 문자열 개행 손상(작업 중 유입) 복구.
- 검증: 자동 로그인(LFC) 통과, 메인 창 정상 타이틀 기동, DB 세션 3개 폴링, 60초+ 안정.
- **설비 클릭 런타임 에러 수정(2026-07-12)**: 메인 화면에서 CV/SC/RTV 클릭 시 즉시 AV 크래시.
  ① 근본 원인 — `CEcsDoc::Permission()` 이 `new CPermission()` 후 `Lookup` 성공 시 맵의
  dangling 포인터(InitilizeUserInfo 가 맵 저장 객체를 루프 안에서 delete 하던 버그)를 역참조.
  → 저장 객체의 조기 delete 제거 + Permission 을 Lookup 실패 시 허용/누수 제거로 재작성.
  ② RTV 는 EcsDefine.xml 에 설비 정의 자체가 없어 클릭해도 무반응 → `<Equipment class="CRtv">`
  (RTV #801, 레이아웃 위젯 id 17130801 과 일치) 추가, rtv_data.rtv_no '01'→'801'.
  ③ RTV 맵 키 `%05s` 공백패딩("  801")이 클릭/DB/주기갱신 키("801")와 불일치 → RtvInfo
  GetStringEQP_NO 패딩 제거. 검증: CV(124)/SC(901)/RTV(801) 대화상자 필드까지 정상 표시,
  연속 클릭 후에도 프로세스 안정.
- **설비 대화상자/표시 정합·버그 수정(2026-07-12)** — 사용자 피드백 반영:
  ① **RTV_DATA::GetCid** `11130%05s`→`17130%03s`: 레이아웃 위젯(17130801)과 결합(m_pControl)
  실패로 RTV 도형이 색/클릭 반영 안 되던 문제 수정.
  ② **SC/RTV 레일색 반전**(ScInfo/RtvInfo GetRailColor): 원 코드 idle 반환식
  `(!= "0" || != "0000")`이 **항상 참**이라 정지 안 해도 색이 칠해짐 → 정지(입고/출고/입출고
  suspend 1/2/3)=**빨강**, 화물적재=파랑, idle=검정으로 재작성. err_code '0'/'0000' 양형식 허용.
  ③ **상태바 EQUIP+HOST 2개**만 표시(MainFrm AddStatusBarPane): 기존 CV_1G/SC_1~4/WC_104/HOST
  7개 → 실제 통신 대상만(설비 게이트웨이=EQUIP, WMS=HOST).
  ④ **CV 일시정지 즉시 반영**: 일시정지 클릭은 tr_pause_od(지시)만 갱신하고 표시는
  tr_pause_rd(판독)라 CV일시정지 처리 게이트웨이 부재 시 다음 폴링에 원복 → 지시 UPDATE 가
  tr_pause_rd 도 함께 세팅(시뮬레이터 echo)해 토글 표시 지속.
  ⑤ **대화상자↔원본ECS 변수 정합**(서브에이전트 매핑 근거, `docs`): DB 컬럼은 모두 존재하고
  클라이언트 쿼리만 뒤처짐. SC(Sc.cpp): 게이트웨이 미기록 레거시 크레인 컬럼 대신 LGLS 컬럼으로
  재지정 — UCSTATUS_RD←subsystem_status_rd, COMPLETE_RD←unload_complete_rd,
  LUGG_NO_FK1_RD←pallet_on_vehicle_rd(출력 별칭 유지→대화상자 코드 무변경). RTV(Rtv.cpp):
  위치 별칭 arrive/depart_track→to_01_od/from_01_od. (잔여: CV 관측치 추가 표시·fork2/POS_V 등
  LGLS 무대응 필드 숨김은 대화상자 레이아웃 변경 필요 — 후속 과제로 매핑표 `docs` 보관.)
- **RTV 싱글 포크 대화상자 정리(2026-07-12)** — 사용자 피드백:
  ① **Fork2 계열 숨김**(RtvSkinDlg::HideFork2SingleMode, OnInitDialog 호출): 작업번호2/출발·도착위치2/
  작업구분2 컨트롤·FORK2 그룹박스·포크 선택 체크박스(1/2/1·2) ShowWindow(SW_HIDE). **측정 검증**:
  1 FORK/2 FORK/1,2 FORK/FORK2 모두 [HIDDEN].
  ② **Fork1 라벨→"Fork"**: dlg_rtv.ini `fork1=FORK1→Fork` + 코드 재지정. **측정 검증**: 그룹박스 "Fork".
  ③ **제목 "801"→"RTV N호기"**: InvalidateRtvData 에서 m_edtRtvNo 를 K_RTV_NO 대신
  `"RTV %d호기"(K_PLC_NO)` 로. (스킨 Edit 라 GetWindowText 측정 불가 — 기존 "801" 표시와 동일
  SetWindowText 경로라 표시는 동작, **육안 확인 권장**.)
  ④ **삭제 시 포크 선택 불필요**: OnBnClickedBtnRtvDelete 에서 m_strRtvFork 가드 제거, 항상
  단일 포크 삭제(DELFK1) 진행, 확인문구 "Fork1 삭제"→"삭제".
- **RTV/SC 대화상자 원본 ECS Vehicle 레이아웃 전면 재설계(2026-07-12)** — 첨부 그림 근거:
  IDD_SKIN_RTV_CTRL/IDD_SKIN_SC_CTRL(Ecs.rc) + RtvSkinDlg/ScSkinDlg + resource.h 를 원본 ECS
  Vehicle 인터페이스로 재설계(서브에이전트, 새 ID RTV 8000~/SC 8100~):
  ① 제목(RGV1·(RGV N호차) / 스태커 크레인 NN), IDLE 상태 박스,
  ② 핸드셰이크 LED 11종(Load/Unload Complete(+ACK), Transfer Request(+ACK), Pallet Exist,
     Alarm Set/Reset(+ACK)) — 소형 EDITTEXT+OnCtlColor(켜짐=초록/꺼짐=회색),
  ③ Pallet ID 값, 현재/출발/도착/완료위치 3워드×4 + 알람코드, 요청번호·자재코드·팔렛·입출고 태그,
  ④ 버튼: 명령 재전송(CMD_RQ_YN='Y'), 이상종료(강제완료 재사용), 확인, 사용금지(suspend).
  대화상자 표시는 전용 SELECT + SetDlgItemText 로 SC_DATA/RTV_DATA Vehicle 컬럼 직접 조회
  (기존 LFC 크레인 컨트롤 머시너리는 조기반환으로 무력화 — 미서브클래스 컨트롤 역참조 방지).
  DB/데이터클래스/AutoRunProc 무변경. Ecs.exe 재빌드 클린.
- **EQP_TASK(IO_TASK) → SC_DATA Vehicle 컬럼 실시간 갱신(2026-07-12)**: 대체 체인에 SC PLC
  게이트웨이가 없어, IO_TASK cThread_SCH.AutoRunSC 가 SC 작업 단계별로 SC_DATA Vehicle 컬럼을
  직접 반영(UpdateScVehicle): 지시=RUN+반송지시/응답 ON, 구동중=적재완료 ON, 완료=하역완료 ON·
  IDLE 복귀·파렛트 하차. **실검증**: SC 작업 투입 시 subsystem_status_rd 2(RUN)→1(IDLE),
  pallet_on_vehicle_rd/ load_complete_rd/ transfer_request·ack 정상 토글. (2자 컬럼 초과 방지
  Cap 헬퍼. RGV 레그는 체인 외라 RTV_DATA 는 기준 IDLE 표시.)
  ※ IO_TASK 소스는 **UTF-8(BOM)** — cp949 로 편집 금지(이번에 인코딩/라인엔딩 혼선 1회 복구).
- **상태바 EQUIP ← EQP_TASK**: OnUpdateStatusCv1 이 EQP_MST(EQP_TYP='CV', WCS_TASK_CV 유지)
  connected_yn/upd_dt 로 통신 상태 판정(>5초 무갱신 또는 'N' → 빨강). 패널 텍스트를 매 갱신마다
  "CV_1G"로 되돌리던 것을 "EQUIP"로 고정.
- **크레인(SC/RTV) 도형 빨강 표시 수정(2026-07-12)**: 레일색(§앞) 외에 **포크색**(ScInfo
  GetForkColor1/2)도 `V_ERR_CODE_RD != "0000"` 이면 에러색(빨강)인데 DB err_code_rd='0'(한 자리)
  이라 무에러인데도 빨강. → '0'/'0000'/'' 모두 무에러로 처리(ScInfo 4곳, RtvInfo 2곳 동일 보강).
  현 DB 상태(err='0', status=1 IDLE, suspend='0')에서 크레인이 정상색(포크=DARK_GRAY, 레일=검정)
  으로 표시.
- **RTV/SC 대화상자 틀·CV 트랙·크레인 위치(2026-07-12)** — 사용자 피드백:
  ① **대화상자 틀(그룹박스) 추가**: 재설계본이 평면 배치라, 그림처럼 외곽 프레임("RGV 정보"/
     "Stacker Crane 정보") + 설비상태/위치정보/작업정보 3섹션 GROUPBOX 를 두 DIALOGEX 에 삽입.
  ② **CV 트랙번호 마지막 2자리만 표시**: CvSkinDlg InvalidateTrackData 에서 V_MC_NO.Right(2).
     추가로 **레이아웃(1F 모니터링) 트랙 도형**도 3자리로 나와, DciLib DciTrackCtrl 드로우에서
     m_strText(트랙 키, 값 자체는 로직용으로 유지) 대신 표시만 Right(2) 로 그리도록 수정(DciLib 재빌드).
  ③ **RTV/SC 현재위치 표현**: 원본 ECS 는 크레인 현재위치를 Vehicle 관측치 **SUBSYSTEM_LOCATION_01~03**
     (=location_01~03_rd, PPT 슬라이드 18~26 메모리맵)로 읽을 수 있음 → **확인 가능**. DciRvCtrl
     크레인 위치(m_nForkPos)를 기존 고정('1', "SC 행위치 표현하지 않음")에서 **POS_H_RD 기반**으로 변경
     (ScInfo, 0~15 클램프). IO_TASK UpdateScVehicle 가 SC 작업 중 pos_h_rd·location_01~03_rd 를
     현재위치로 기록(실검증: 작업 시 pos_h_rd/location 갱신, IDLE 시 0 복귀). RTV 는 기존
     m_MapRtvPosition[V_POS_H_RD]→m_nForkPos 메커니즘 유지.

## 4. 원본 ECS ↔ 시뮬레이터 (구성 완료 · 기동은 관리자 권한 대기)
- 검증 보고: `docs\ORIGINAL_ECS_VERIFICATION.md`
- 남은 1단계: **관리자 PowerShell에서 `db\setup_original_ecs_ip.ps1` 실행**
  (192.100.1.110/142 루프백 별칭 — 원본 하드코딩 IP를 로컬로).
- 이후: EQP_SIM(2004 수신 중) → HOST_SIM → `D:\project\LGLS\LGLS_1WH_original\Backup\ECS\bin\Debug\HECS.exe`
- 원본용 DB(localhost\sqlexpress LglsMCS)는 시드/R영역 연결문자열 반영 완료.


### RTV/SC 대화상자: 원본 폼 복원 + 현재 값 매핑 (첨부 D:\LFC_CLIENT\Ecs\Ecs.rc)
사용자가 원본 LFC의 RTV/SC 대화상자 **폼 자체**로 되돌리고, 현재 Vehicle 값을 그 폼에 담기를 요청.

- **rc 템플릿 교체**: 첨부 원본 rc의 `IDD_SKIN_RTV_CTRL`(353x438), `IDD_SKIN_SC_CTRL`(327x446) 다이얼로그 블록을 현재 `Ecs.rc`의 서브에이전트 재설계 블록과 교체(UTF-16 유지). 원본 폼의 67(RTV)/82(SC) 컨트롤 ID가 모두 현재 `resource.h`에 존재함을 사전 검증.
- **`RtvSkinDlg::InvalidateRtvData` 재작성**(원본 컨트롤 채우기): 제목=`RGV N호기`; `GetQrySelectRTV_STATUS_CCD`(Vehicle 컬럼 별칭)로 모드/완료/화물/RTV상태/ACTIVE/작업구분/SUSPEND 디코드→`IDC_EDT_RTV_*`; 현재위치=`V_POS_H_RD`→`IDC_EDT_RTV_HORIZONTAL_POS`; FORK1 작업정보→`IDC_EDIT_RTV_JOB_NO`/`IDC_CBX_RTV_START_POS`/`DEST_POS`; 에러=`V_ERR_CODE_RD`; SUSPEND 버튼 토글; `GetQrySelectJOB_MST`→하단 JOB_STATUS. `HideFork2SingleMode`는 `GetDlgItem(id)` 기반으로 재작성(Fork2 컨트롤·체크박스 숨김, FORK1 그룹박스 라벨→"Fork").
- **`ScSkinDlg::InvalidateScData` 재작성**(원본 SC 컨트롤 채우기): 라이브 Vehicle SELECT(SC_DATA에 원본+Vehicle 컬럼 공존, `pos_h_rd`/`pos_v_rd` 확인)로 SC상태/RC상태=`SUBSYSTEM_STATUS`; 수평=`POS_H`(없으면 `LOCATION_01`)·수직=`POS_V`→`IDC_EDT_SC_HORIZONTAL/VERTICAL_POS`; 화물=`SENSOR_FK`; 완료=`UNLOAD_COMPLETE`; 에러=`ALARM_SET_CODE`; 작업번호/출발/도착/작업구분→`IDC_EDT_SC_JOB_NO`·`IDC_CBX_SC_START/DEST_POS`·`JOB_TYP`; SUSPEND; `GetQrySelectJOB_MST_FK1`→하단. 제목=설비명(`V_MC_NO_NM`) 또는 `스태커 크레인 N`.
- **버튼**: 원본 버튼 핸들러(강제완료/비상정지/ACTIVE/정지/에러해제/삭제/홈복귀/SUSPEND; SC의 CONFIRM/EMERGENCY/…/ALL_SUSPEND/MANUAL_RET)는 그대로 유지·동작. 두 대화상자 모두 `DDX_Control`이 없어 템플릿 교체로 인한 서브클래싱 크래시 없음. 서브에이전트 추가 컨트롤(`IDC_RTVV_*`/`IDC_SCV_*`)은 템플릿에서 빠졌지만 `SetDlgItemText`/`OnCtlColor`는 없는 ID에 무해(no-op).
- **빌드/검증**: `Ecs.vcxproj` Debug|Win32 성공, Ecs.exe 재빌드·실행. 제어 클릭 메시지(17130801 RTV / 17120901 SC)로 대화상자 열림 확인(#32770 뷰 3개, ComboBox 21/Edit 100 등 원본 폼 컨트롤 로드), 대화상자 오픈 후에도 프로세스 안정.
- **백업**: `Ecs.rc.presrc_bak`, `RtvSkinDlg.cpp.presrc_bak`, `ScSkinDlg.cpp.presrc_bak`.

### 창고 모니터링(EcsLayout2/3) 트랙번호 3자리 → 2자리 (진짜 렌더 경로)
이전 `DciTrackCtrl::UpdateControl` line 168 `DrawText(strTrDisp)` 수정만으로는 부족했음. 실제로 보이는 번호는 함수 말미의 `DrawFontText(pDC, _T(""), ...)`(DciTrackCtrl.cpp:349)가 그림 — `CDciControl::DrawFontText`는 넘긴 문자열이 빈 값이면 `strRealText = m_strText`로 되돌려 **전체 3자리("719","603")를 그대로** 출력(DciControl.cpp:222-223,265)했기 때문. `EcsLayout1`(1F 모니터링)은 트랙 텍스트가 1~2자리라 표가 안 났고, `EcsLayout2/3`(창고 모니터링 1F/2F)는 3자리라 노출됨.
- 수정: `DrawFontText(pDC, strTrDisp, NULL, ...)`로 잘라낸 문자열을 명시 전달, 중복이던 line 168 `DrawText`는 주석 처리. `m_strText`(트랙 키)는 불변. DciLib.dll 재빌드→Ecs.exe가 런타임에 로드.

### RTV/SC 대화상자 콤보박스 → ReadOnly EditBox
사용자 요청으로 `IDD_SKIN_RTV_CTRL`·`IDD_SKIN_SC_CTRL`의 모든 `COMBOBOX`(19개: RTV 11 + SC 8)를 `EDITTEXT ... ES_AUTOHSCROLL | ES_READONLY`로 변환(ID·x·y·w 유지, 높이는 ≤16이면 그대로 아니면 14, 원본 `NOT WS_VISIBLE`는 보존). 채우기 코드는 이미 `SetDlgItemText(ID, ...)`라 에디트 컨트롤에 그대로 동작. DDX_Control이 없어 컨트롤 클래스 변경으로 인한 부작용 없음. CV 대화상자 콤보는 요청 범위 밖이라 유지.
- 빌드/검증: DciLib·Ecs 재빌드 성공. 실행 후 RTV/SC 대화상자 열림(에디트 컨트롤 로드, 대화상자 내 콤보 0), 프로세스 안정.

## 8. 시뮬레이션 재가동 — WCS Client 화물 이송 표현 (2026-07-12)
목표: 시뮬레이션을 다시 돌려 WCS Client(Ecs) 레이아웃의 트랙에 화물 이송이 보이게 함. (트랙 화물 = `DciTrackCtrl.m_bExist ← cv_data.SENSOR0_DATA_RD`, `TrackInfo.cpp:192`)

### 기동 체인 (설비)
- WCS_TASK_HOST=`TASK_LFC10_G1_ECSCOM.exe`(D:\project\LGLS\WCS_TASK_HOST), EQP_TASK=`WCS_TASK_CV.exe`(Downloads), IO_TASK=`IO_TASK_SEMI_FINISH.exe`(Downloads), PLC=`EQP_SIM.exe`(SIM\EQP_SIM), WMS=`HOST_SIM.exe`(SIM\HOST_SIM).
- HOST_SIM: 첫 S 수신 후 [시작] 버튼 활성 → 3개 로직 O(입고/출고) 발행. 명령채널 127.0.0.1:9911(WCS_TASK_HOST 리슨), 보고채널 9910(HOST_SIM 리슨).

### 데드락 원인·해소
- **원인**: EQP_SIM이 기동 시 `bin\Debug\Data\eqp_state.txt`에서 컨베이어 파렛트를 복원 → 지난 실행의 파렛트 4개(121/123/124/132)가 입고대·픽업 지점을 점유해 신규 화물이 못 흐름(cv_data만 지워도 EQP_SIM이 복원).
- **해소**: EQP_SIM 정지→`eqp_state.txt` 비움→재기동으로 파렛트 0개에서 신선하게 시작. AutoFeed(`[FEED] AUTO=1`, FEED_MS=5000)로 재투입.

### 구조적 공백 수정 — 입고 크레인 픽업 훅 (`EQP_SIM\Sim\ConveyorSim.cs`)
- **문제**: 대체 체인은 S/C·RGV PLC 게이트웨이가 없어 크레인 레그를 IO_TASK가 DB로만 자동완주 → 컨베이어가 파렛트를 크레인 핸드오프 포트(WAIT_OUT)까지 옮긴 뒤 **픽업 신호를 못 받아 파렛트가 영구 정체 → 연쇄 막힘(재클로그)**.
- **수정**: `Tick()`의 MovePallets 직후 `2.5)` 훅 추가 — 입고(Dir=Ingo) 파렛트가 `GetWaitOutPort()`(입고=port1, 라인=종점) 지점에서 지시(JOB Id)를 받고 체류(HoldUntil=지시시각+WaitOutHold 9s) 경과 시 S/C·RGV 상차로 보고 제거(`Pallets.Remove`+`SetExist(false)`+`SetTracking("")`+`PulseEvent("UNLOAD_COMPLETE")`). 출고 '재현/반출' 훅의 입고 대응. Dir=Ingo·HoldUntil≠MinValue 가드로 출고·복원 파렛트 미영향.
- 백업: `ConveyorSim.cs.bak`, `eqp_state.txt.bak_*`.

### 검증 (실측)
- 재기동 후 지속 이송: JOB 0017/0021/0023이 각각 `CONVEYOR:11 22→21 이동` → `P21 입고 파렛트 크레인 픽업(S/C·RGV 상차 시뮬레이션)` — 이동·픽업 반복, 파렛트 누적 없음.
- host_if_log F(입고완료) 보고 연속 왕복, cv_data SENSOR0 트랙 이동(예 124→123, 122→121) 관찰. Ecs 클라이언트 안정 — 레이아웃 트랙에 화물 이송 표현됨.
- 참고: SC Vehicle 컬럼 업데이트에 `[VEH-OUT] n=-1 err=문자열이나 이진 데이터는 잘립니다` 잔존(크레인 대화상자 Vehicle 표시용, 컨베이어 이송엔 무관) — 별도 확인 대상.

## 9. 추가 요청 4건 (2026-07-12)

### 9.1 EQP_SIM 출고대 배출 — 2단계(지게차) (`EQP_SIM\Sim\ConveyorSim.cs`)
기존엔 출고 종점 파렛트를 OutRemoveMs+HoldUntil(~9초)에 화물·트래킹을 **동시** 제거. 사용자 요청대로 2단계로 분리:
- ① 도착 +`OutRemoveMs`(1000ms) → `Discharged=true`, `SetExist(false)` = 화물(센서) 사라짐. `MirrorWcsTracks` 센서를 `exist && !Discharged`로 하여 배출 1단계면 OFF.
- ② `+OutTrackClearMs`(1000ms) → `Pallets.Remove` + `SetTracking("")` = 트래킹 데이터 제거.
- `SimPallet`에 `Discharged`/`DischargedAt` 필드 추가, `ScenarioEngine`에 `OutTrackClearMs` 상수+INI(`[TIMING] OUT_REMOVE_MS=1000, OUT_TRACK_CLEAR_MS=1000`). 입출고 겸용 C/V#11~15(Def.No>=11) 출고 종점만 대상. 빌드 성공.
- 참고: 출고 물리 파렛트는 "출고 파렛트 재현" 훅이 출고 CV 레그 디스패치(트래킹 기록) 시 생성 → 그때 이 2단계 배출이 발동. 현재 사이클은 F2(출고완료) 보고까지 완주하나 출고가 DB로 완주되는 구간이 많아 물리 출고대 통과가 드묾.

### 9.2 CV 대화상자 쓰기/삭제/복사/잘라내기/붙여넣기 (`CvSkinDlg.cpp`)
- 버튼·핸들러·DB 경로는 정상(삭제를 유휴 트랙에서 실측: `lugg_no_od 8888→0, od_rq_yn→Y` 확인). "동작 안 함"의 실제 원인 = UPDATE의 `AND OD_RQ_YN='N'` 가드로 **진행중(바쁜) 트랙에서 0행 갱신 → 조용히 무동작**.
- 수정: Write/Delete/Cut/Paste 4개 UPDATE의 `AND OD_RQ_YN='N'`를 `AND '1'='1'`로 바꿔 **어느 상태에서도 수동 조작 반영**.
- Copy 트랜잭션 누수 수정: Copy는 DB 미변경인데 `BeginTrans_DLG` 후 커밋 없이 break → `m_pDoc->RollbackTrans_DLG()` 추가.

### 9.3 RTV 컨트롤 평상시 밝은 회색 (`RtvInfo.cpp` GetForkColor1)
- 원인: SC_DATA는 `auto/active_mode='1'` 시드라 GetForkColor1이 LIGHT_GRAY. RTV는 active_mode_rd 컬럼이 없어 `CASE subsystem='2'`로 유도→유휴 시 '0' → 게이트에서 `DARK_GRAY` 반환.
- 수정: GetForkColor1(pRTV_DATA)의 비자동/비액티브 분기 `return DARK_GRAY;` → `return LIGHT_GRAY;` (크레인 몸통 wing은 레이아웃 wingcolor=f0f0f0 유지). 평상시 SC처럼 밝은 회색.

### 9.4 상태표시줄 EQUIP 통신상태 (`MainFrm.cpp` OnUpdateStatusCv1)
- 원인: EQUIP 판정이 트랙 **"101"**(PLC 01, 라인 컨베이어)을 조회 → EQP_MST(EQP_TYP='CV') PLC01은 `CONNECTED_YN='N'`(EQP_TASK 미통신) → 항상 RED.
- 실제 EQP_TASK(WCS_TASK_CV)는 게이트웨이 PLC 11~15와 통신(CONNECTED_YN='Y', UPD_DT 신선).
- 수정: 조회 트랙 "101" → **"121"**(PLC 11 게이트웨이). 실측: 121→PLC11→connected='Y'/FRESH → GREEN. EQP_TASK 끊기면 UPD_DT 정체로 RED.

빌드: Ecs.vcxproj / EQP_SIM.csproj 성공. 6개 프로세스(HOST_SIM·WCS_TASK_HOST·IO_TASK·WCS_TASK_CV·EQP_SIM·Ecs) 안정 가동.

## 10. 모니터링 물리 흐름 재현 — RGV/SC 크레인 애니메이션 (2026-07-12)

문제: 모니터링에서 RGV가 전혀 안 움직이고 SC는 목적지로 점프만 함.
- 근본 원인: IO_TASK의 **RGV 레그(상태 30)는 죽은 코드** — 어떤 코드도 '30'으로 라우팅하지 않아 인바운드가 CV→바로 SC로 인계(RGV 구간 건너뜀). RTV_DATA 위치/상태/화물 미갱신. EQP_SIM VehicleSim은 RGV/SC 물리 시뮬레이션 코드가 있으나 디스패치(TRANSFER_REQUEST) 주체 부재로 잠들어 있고, EQP_SIM엔 DB 접근이 없어 rtv/sc_data를 직접 못 씀 → **RGV/SC 애니메이션은 IO_TASK가 구동**.

### 모니터링 구동 필드 맵 (WCS Client 조사)
- RGV 위치: `RTV_DATA.POS_H_RD` → EcsDefine `<Rtv><Position><Pos plc view>` 맵 → 화면 셀 0~15. **기존 맵에 `0→0` 하나뿐 → 항등 0~15로 확장**(Bin/Debug+Ecs 소스 EcsDefine.xml). 이제 POS_H_RD=셀 직접. **Ecs 재기동 필요**(기동 시 로드).
- RGV 화물마커=`SENSOR_RTV_RD='1'`, BLUE 운반레일=`LUGG_OD!='0'`, 입고(STO)색=`JOB_TYP_OD='1'+AUTO_MODE_RD='1'+SUBSYSTEM_STATUS_RD='2'`.
- SC 위치: `SC_DATA.POS_H_RD` 정수 0~15 직접. 화물마커=`SENSOR_FK_RD='1'`, BLUE=`ITN_LUGG_FK1!='0'`. 재도색은 ~1초 CCollectDB 폴링(변경분만).

### 토폴로지 (DB·레이아웃)
- 인바운드: 입고대 122→121(RGV 픽업, 셀14)→**RGV**→115(드롭, 셀10)→116→**SC904**→랙. (`22→21`, `15→16`은 컨베이어)
- SC→라인트랙: 901→103/104, 902→107/108, 903→111/112, **904→115/116**, 905→119/120 (RGV 드롭=홀수, SC 픽업=짝수). RGV 픽업=START_POS−1.
- RGV 셀맵: 121→14,123→2,130→7 / 103→3,107→5,111→8,115→10,119→13.

### IO_TASK 구현 (`cThread_SCH.cs`, UTF-8 BOM)
- **인바운드 RGV 라우팅**: CompleteCV 입고 → '30'(ST_RGV_WAIT). 옛 DriveRGV/RunRGV/CompleteRGV 호출은 **주석 처리**(신규 AutoRunRGV와 경쟁해 '39' 데드엔드로 빠지던 버그 제거).
- **AutoRunRGV + UpdateRtvVehicle 신설**(타임스탬프 게이팅): 30→31(RUN·픽업셀 이동·입고색) → 31→35(+1.2s, 화물 싣고 드롭셀 이동·마커·BLUE) → 35→'20'(+2.6s, IDLE·홈복귀·화물 지움 → SC 레그 인계). RTV_DATA에 SUBSYSTEM_STATUS_RD/POS_H_RD/PALLET_ON_VEHICLE_RD/SENSOR_RTV_RD/JOB_TYP_OD/LUGG_OD/AUTO_MODE_RD/DEPART·ARRIVE_TRACK 기록.
- **AutoRunSC 강화**(입고): 지시=빈차 H/S(POS_H='0', 화물0) → 중=픽업+목적지 bay 이동(화물, LC=1) → 완료=하역+**초기복귀(POS_H='0', IDLE, 화물0)**. UpdateScVehicle에 SENSOR_FK_RD·ITN_LUGG_FK1(캐리 시 마커·BLUE) 추가.

### 검증 (실측)
- **RGV 전체 사이클**: st2/pos14(픽업,입고색) → st2/pos10/lugg=0484/sen1(화물 싣고 드롭) → st1/pos0(홈복귀). ✓
- **SC 전체 사이클**: st2/pos0(빈차 H/S) → st2/pos15→12/plt·sn1·itn(픽업+목적지) → st1/pos0(완료·초기복귀). ✓
- **비회귀**: F1(입고)·F2(출고) 완료 보고 정상, 작업 진행 정상. 6개 프로세스 안정.
- 일반화: 라우팅이 START_POS/DEST_POS 기반이라 로직1~3(각 크레인) 모두 동일 동작.

### 남은 정제 (후속)
- 컨베이어 트랙 **15/16의 순간 화물 표시**("15번에 화물 생기고 16번으로"): 컨베이어 cv_data는 EQP_SIM이 소유(WCS_TASK_CV 미러)하고 EQP_SIM은 RGV 드롭 목적지를 모름 → IO_TASK→EQP_SIM 목적지 신호(트래킹 기록 경로) + EQP_SIM 재현 훅을 인고로 확장 필요. 크레인 이동 표현이 주 시각요소이며 이 컨베이어 순간표시는 후속 과제.
- 백업: cThread_SCH.cs.prephys_bak, EcsDefine.xml.prephys_bak.

### 10.1 라인 컨베이어 화물 전환 (15→16) 추가 — 남은 과제 완료 (2026-07-12)
사용자 스펙의 "15번에 화물 생기고 16번으로 가야"(RGV 드롭→라인 컨베이어 통과→SC 픽업) 구현.
- **경로 조사**: cv_data 트래킹은 IO_TASK→WCS_TASK_CV→PLC R영역→EQP_SIM `GetTracking` 경로가 있으나, **WCS_TASK_CV는 게이트웨이 컨베이어(11~15)만 담당**하고 SC 라인(PLC 02~10)은 담당 안 함(tracking_write_yn='Y'가 소비 안 됨). 따라서 라인 트랙 R영역이 안 써져 EQP_SIM 재현 미발동.
- **핵심 발견**: **아무도 라인 트랙(103~120) cv_data 를 안 씀**(WCS_TASK_CV 미담당, EQP_SIM DB 접근 없음) → 실측으로 IO_TASK 가 직접 쓴 `sensor0_data_rd`/`lugg_no_rd` 가 16초간 유지됨 확인. **IO_TASK 가 라인 트랙 cv_data 직접 소유**.
- **구현(IO_TASK)**: `WriteCvSensor(track,sensor,lugg)` 신설(cv_data SENSOR0_DATA_RD/LUGG_NO_RD 직접 UPDATE, MC_NO 키). 단계별:
  - AutoRunRGV 드롭(35→20): `WriteCvSensor(dropTrack,"1",lugg)` — 드롭 트랙(홀수 115/119…)에 화물 생성.
  - AutoRunSC 中(21→25): `WriteCvSensor(drop,"0","0"); WriteCvSensor(pick,"1",lugg)` — 드롭→픽업 트랙(짝수 116/120…) 이동. (드롭 트랙은 그 사이 유지)
  - AutoRunSC 완료(25→29): `WriteCvSensor(pick,"0","0")` — SC 픽업완료로 픽업 트랙 클리어.
  - 드롭 트랙 = `RgvDropTrack(destPos)`, 픽업 트랙 = 드롭+1.
- **EQP_SIM**: 입고 재현 훅(ConveyorSim 1.3절, No≤10 라인 컨베이어의 IngoPath[0]에 트래킹 시 입고 파렛트 재현) 추가 + 출고 재현(1.2절)이 입고 드롭 포트 가로채지 않게 가드. (WCS_TASK_CV 미담당으로 현재 미발동이나, 향후 라인 컨베이어 브리지 시 대비해 보존.)
- **검증(실측)**: 라인 트랙에 화물 순차 표시 — `119(드롭)→120(픽업)` 이동, 픽업 트랙 ~4초 유지 후 SC 픽업 시 클리어. F1/F2 완료 보고 정상(비회귀). 6개 프로세스 안정.
- 전체 인바운드 물리 흐름 완성: 22→21(컨베이어) → RGV(픽업·운반·드롭·복귀) → 15→16(라인 컨베이어) → SC(빈차 H/S·픽업·목적지·초기복귀). 로직1~3 공통.

### 10.2 RTV 색상·SC 작업색·출고 컨베이어 보정 (2026-07-12)
사용자 피드백 3건:
- **RTV가 명령 중 짙은 회색으로 바뀜**: `RtvInfo::InvokeControl`에서 단일포크 **보이는 m_clrFork = GetForkColor2**(337행)로 덮어쓰는데, #30에서 GetForkColor1만 고쳤음. GetForkColor2(pRTV_DATA)의 비자동/비액티브 분기 `return DARK_GRAY` → `LIGHT_GRAY`로 수정(평상시 밝은 회색).
- **SC가 색 없이 화물만 이동**: SC 색상은 `SC_DATA.JOB_TYP_RD`로 결정되는데 UpdateScVehicle이 안 씀. UpdateScVehicle에 `jobTyp` 파라미터+`JOB_TYP_RD=:JOBTYP` 추가, 호출부에서 입고='1'/출고='2', 완료 시 '0'(해제) 전달 → 이동 중 작업색(입고 STO/출고 RET) 표시.
- **출고 흐름 컨베이어 트래킹 미표현**: 인바운드만 라인 CV 애니메이션했음. 출고는 SC→라인CV→(RGV)→출고대. 출고대(게이트웨이 PLC)는 기존 재현/배출로 표시됨. **라인 CV 구간**을 AutoRunSC 출고(jobTyp='2')에 추가: 中(21→25)에 `WriteCvSensor(라인CV 짝수트랙, "1", lugg)`로 SC 하역 화물 표시, 완료(25→10)에 클리어. 라인CV 짝수트랙 = `RgvDropTrack(startPos 크레인)+1`.
- 검증: 라인 CV 트랙에 인바운드(115→116)·출고(116/120 SC 하역) 화물 표시 확인. F1/F2 정상, 6개 프로세스 안정.

### 10.3 CV 내 트랙간 이동 가시화 (2026-07-12)
피드백: "입고 출고시 같은 CV 내에서 트랙간 이동이 보이지 않음". 정밀 폴링 결과 라인 CV 첫 트랙(입고 RGV 드롭 115)이 **~0.7초만 체류** → 클라이언트 CCollectDB 폴링(~1초)이 놓쳐 이동이 안 보임. 출고는 짝수 트랙만 표시(이동 없음).
- **입고**: `SC_AUTO_ACCEPT_MS` 1000→2500 → 라인 CV 첫 트랙(115)이 RGV 드롭~SC 中까지 ~2.5초 체류(폴링 포착). 116은 中~완료 ~2.5초.
- **출고**: AutoRunSC 출고 완료(25→10)에서 라인CV 짝수(116)→홀수(115, RGV 픽업측) 이동 표시 + `m_dicCvClear[odd]=now+2500`로 예약 정리. `ProcessCvClear()`(메인 루프 매 틱 실행, else 블록 밖)가 예약시각 도달 시 sensor0/lugg 클리어 → 잔류 방지.
- 검증: 라인 CV 각 트랙 ~2.4초 체류(0.8초 폴링 3회). 입고 115→116/119→120, 출고 116→115/120→119 트랙간 이동 확인. 잔류 없음. F1/F2 정상.

### 10.4 크레인/RTV 주행 스텝 이동 + CV 삭제 표시데이터 제거 (2026-07-12)
- **크레인/RTV 주행 1씩 이동**(피드백: 목표로 점프해 주행이 안 보임): IO_TASK에 `StepCranes()`(메인 루프 매 틱) 추가 — `UpdateRtvVehicle`/`UpdateScVehicle`은 POS_H_RD를 직접 안 쓰고 `m_dicCraneTgt[craneId]`(목표)만 저장, StepCranes가 `CRANE_STEP_MS=600`ms 간격으로 현재→목표를 **1씩** 이동하며 POS_H_RD 기록. RGV는 단일 유닛이라 AutoRunRGV가 가장 진행된 작업(35>31>30) 1건만 처리하도록 가드 추가(다중 작업 목표 충돌 방지). 검증: SC 1→3→4→6→8→10, RTV 0→2→3→2→1→0 부드러운 스텝.
- **CV 삭제가 표시 데이터 제거**(피드백: 트랙26 삭제해도 안 없어짐): 삭제 SQL이 _OD(주문)만 지우고 _RD(표시)는 안 지웠음. `CvSkinDlg` Delete에 `LUGG_NO_RD/JOB_TYP_RD/DEST_POS_RD/SENSOR0_DATA_RD/PULP_SENSOR_RD='0'` 추가. 검증: 트랙 126 삭제 후 sensor0/lugg_rd/job_typ_rd 모두 '0'.
- F1/F2 정상, 6개 프로세스 안정.

## 11. 이송 표현·작업대 색상 정제 (2026-07-12)

사용자 피드백 5건:

### 11.1 작업대 데이터 없으면 색상 미표시 (24/26/32) — `TrackInfo.cpp`
- 원인: **모든 트랙에 STO_READY_RD='1', RET_READY_RD='1'이 상시 시드**되어, 화물 유무와 무관하게 STO/RET ready 색이 항상 표시. 132는 sensor0='1'/lugg='0000' 팬텀이라 ready색(노란색)까지 표시.
- 수정: `bHasData = (V_SENSOR0_DATA_RD=="1" && V_LUGG_NO_RD!="0" && V_LUGG_NO_RD!="0000")` 를 계산해 4개 색상 라인(STN_STO/STN_RET/HS_STO/HS_RET)을 `bHasData &&` 로 게이트. TR_PAUSE(일시정지=빨강) 분기는 유지. 실화물 있을 때만 작업색 표시.
- 132 팬텀: cv_data에 정적 시드된 sensor0='1'/lugg='0000' (EQP_SIM ingo 경로 30→31에 32 미포함 → 재공급 안 됨). 타깃 UPDATE로 클리어 → 화물/색 완전 제거. (스테이션 123~131엔 다른 팬텀 없음 확인)

### 11.2 EcsDefine 24↔26 입출고대 스왑 — `EcsDefine.xml` (Bin/Debug + Ecs)
- 124: `<StoStation 입고대>` → `<RetStation 출고대>`, 126: `<RetStation 출고대>` → `<StoStation 입고대>`. euc-kr 바이트 편집, 양쪽 사본. Ecs 재기동 시 로드.
- 주의: HOST_SIM.ini는 IN_STATION=124/OUT_STATION=126 유지 → 라벨만 스왑(현 흐름에서 124/126은 실사용 안 되어 시각 충돌 없음). 기능 일치까지 원하면 HOST_SIM.ini도 스왑 필요.

### 11.3 출고 라인CV 이송 트랙 매핑 버그 (14→13) — `cThread_SCH.cs`
- 원인: 출고 시 `RgvDropTrack(startPos)`가 **입고 라인**(115/116, PLC08)을 반환 → 116→115를 애니메이션(사용자가 본 것과 다름). 사용자 요청 = 114→113(PLC07, 홀수 라인).
- 수정: `RgvOutDropTrack` 헬퍼 신설 {901:102,902:106,903:110,904:114,905:118}(출고 짝수=SC 하역). AutoRunSC 출고 中/완료 2곳을 짝수(14)→홀수(13=짝수-1) 이동으로 변경.
- 검증(실측): 출고가 이제 **118→117**(크레인905, PLC09 홀수라인)로 표현 = 크레인904 기준 114→113. 입고는 119→120(=115→116) 정상.

### 11.4 입력CV 21↔22 이송 (121/122) — 데이터 정상 확인
- EQP_SIM C/V#11 IngoPath{22,21}, MoveMs=2000/WaitOutHold=9000. 실측: 122(2~11s 체류)→121(9s 체류)로 동일 파렛트(lugg 1461) 이동 = 정상 이송. cv_data 반영 확인.
- 렌더링: 스테이션도 TrackInfo::InvokeControl 대상(24/26/32 색이 뜨던 것이 증거) → m_bExist 앰버(RGB 255,200,0) 채움 경로 활성. DciLib.dll(앰버판) 로드된 Ecs 재기동 완료.

빌드: IO_TASK_SEMI_FINISH.csproj(x86)·Ecs.vcxproj(Win32) 성공. IO_TASK·Ecs 재기동, 6개 프로세스 안정. (DciLib.dll 앰버판은 기존 빌드 유지, Ecs 런타임 로드)
백업: TrackInfo.cpp.colorgate_bak, EcsDefine.xml.stnswap_bak(x2), cThread_SCH.cs.outline_bak.

## 12. 스테이션 내부신호/트랙번호/SC 출고반송 (2026-07-12, 2차 피드백)

### 12.1 스테이션 내부신호 색 조건 (`TrackInfo.cpp`)
사용자 스펙: **입고대(22,26,30)** = 데이터 불필요, 화물 감지 시 **핑크**(RGB 255,120,200). **출고대(22,24,29)** = 화물+데이터+목적지=자기자신 시 **하늘색**(RGB 120,200,255).
- StoStation(입고대) case → `V_SENSOR0_DATA_RD=="1"` 이면 핑크. (기존 STO_READY 시드 무시)
- RetStation(출고대) case → `sensor0=1 && lugg∉{0,0000} && (CConvert::ToInt(DEST_POS_RD)==트랙번호 || JOB_TYP_RD=="2")` 이면 하늘색.
- 데이터 없으면 clrStatusNone → 그림 빨강/파랑원(녹색/노란색 오표시) 제거. (2초 지연 ON은 미구현—화물 감지 즉시 표시로 근사)
- EcsDefine 스테이션 정의가 스펙과 일치 확인: 입고대=StoStation(122,126,130), 출고대=RetStation(122,124,129), 132는 정의 없음(무색).

### 12.2 트랙번호 항상 표시 (`DciTrackCtrl.cpp`)
- 버그: UpdateControl이 `m_items.GetSize()==0` 시 조기 return → DrawFontText(번호) 미실행 → 내부아이템 없는 트랙은 번호 미표시.
- 수정: 조기 return 전에 `DrawFontText(pDC, strTrDisp, ...)` 호출 추가. 이제 아이템 유무와 무관하게 번호 표시.

### 12.3 SC 출고 반송: 화물 싣고 H/S 하역 (`cThread_SCH.cs`)
- 버그: 출고 SC가 화물 픽업 후 화물이 즉시 사라지고 빈차가 홈으로. (toPos=목적지작업대→bay15 clamp, 완료 시 pallet=0)
- 수정(AutoRunSC 출고 재구성): 지시=빈차 픽업bay(4) 이동 → **중=화물 싣고(SENSOR_FK=1,BLUE) H/S(bay0)로 반송, CV 미하역** → 완료=H/S 하역→라인CV(14) 화물, 크레인 빈차.
- 라인CV 14→13은 즉시 이동이 아니라 **지연 스테이징**(`m_dicCvMove`/`ProcessCvMove`): 14 표시 → 2.5s후 13 이동 → 2.5s후 정리. 14가 먼저 보이도록.
- 검증(실측): `S904 pos0 SENSOR_FK=1 PALLET=1573`(크레인이 화물 싣고 H/S) → `CV 114:1573`(하역) → `CV 113:1573`(13 이동) → 정리. 입고는 반대(라인 픽업→랙 반송) 정상.

### 12.4 미해결(후속) — 게이트웨이/RTV 핸드오프
- 출고 21↔22(121/122): 출고 시 RTV가 라인(113)→게이트웨이(121)→121→122 배출 구간이 IO_TASK AutoRunRGV(입고전용)에 미구현. EQP_SIM C/V#11 OutgoPath{21,22}는 파렛트가 121에 놓여야 발동하나 배치 주체 부재.
- RTV→SC 인계 속도: 입고 115(RTV드롭)→116(SC픽업) 간격이 짧게 느껴짐(타이밍 RGV_DONE_MS/SC_AUTO_ACCEPT_MS 조정 또는 EQP_SIM 파렛트 제거 협조 필요).
- 근본: 게이트웨이(11-15)는 EQP_SIM 소유, 라인/크레인은 IO_TASK 소유 — 출고 RTV 레그는 양측 협조 설계 필요. (별도 세션 권장)

빌드: DciLib.dll·Ecs.exe(Win32)·IO_TASK(x86) 성공, 재기동, 6개 프로세스 안정.
백업: DciTrackCtrl.cpp.trknum_bak, TrackInfo.cpp.colorgate_bak, cThread_SCH.cs.sccarry_bak.

### 12.5 자동 크레인 데이터 없이 이동 금지 (SC/RTV) (`cThread_SCH.cs`)
사용자 규칙: **자동 상태의 SC/RTV는 데이터(화물) 없이 스텝 이동하지 않는다.** 기존엔 빈차 픽업/복귀 구간이 StepCranes로 미끄러지듯 이동 → 위반.
- 수정: UpdateScVehicle/UpdateRtvVehicle에서 **화물 실음(senFk=='1' / SENSOR_RTV=='1')일 때만 m_dicCraneTgt만 세팅→StepCranes가 1칸씩 스텝**. **빈차면 m_dicCraneCur=목표로 스냅 + (메인 UPDATE 후) WriteCranePos로 POS_H 즉시 반영** → 글라이딩 없이 즉시 위치.
- ⚠ 주의: WriteCranePos가 `Parameters.Clear()` 하므로 메인 UPDATE **파라미터 구성 중 호출 금지** → ExcuteNonQry 이후로 분리(POS_H 쓰기는 별도 UPDATE). RTV `_pcv`는 블록스코프라 `_rtvtgt` 메서드스코프로 승격.
- 검증(실측): 빈차 `p4 f0` 고정(스냅, 정지) → 화물 실음 `p4→3→1→0 f1` 스텝(운반). RTV `p14 s0` 스냅 후 운반 시 스텝. 데이터 없는 글라이딩 구간 소멸.
백업: cThread_SCH.cs.nomovenodata_bak.

## 13. 크레인 완전정지 모델 + 게이트웨이/인계 (2026-07-12, 3차)

### 13.1 자동 크레인 "완전 정지"(스냅 제거) (`cThread_SCH.cs`)
- 12.5의 빈차 스냅(순간이동)도 제거. **화물 실었을 때만 m_dicCraneTgt 세팅→스텝. 빈차면 m_dicCraneTgt=현재위치(m_dicCraneCur)로 고정** → 위치 변화 전혀 없음. 픽업은 크레인 현재 위치에서.
- 검증(실측): 빈차 `p0→p0`, `p4→p4` 완전 정지. 화물 실으면 현재위치서 목적지로 스텝(예 4→2→0). RTV 동일.

### 13.2 RTV→SC 인계 속도 (`cThread_SCH.cs`)
- SC_AUTO_ACCEPT_MS 2500→3500ms: 라인CV 첫 트랙(15) 체류를 늘려 RTV 드롭→SC 픽업 분리 가시화.

### 13.3 출고 21→22 게이트웨이 배출 (`cThread_SCH.cs`)
- CompleteCV에서 출고(2) & 목적지=122(22행)일 때 **121→122 배출 표현**(WriteCvSensor+m_dicCvMove 스테이징). `IsTrackEmpty("121")` 가드로 인바운드(122→121, EQP_SIM 소유) 경합 회피.
- 실측: 출고 1801이 118→117(라인) 후 121→122(게이트웨이) 이동 관측.
- ⚠ 한계: 게이트웨이(C/V#11 21/22)는 EQP_SIM 인바운드 상시 사용 → 가드로 비었을 때만 표현. 22행 출고에만 적용.

### 13.4 미해결/설계결정 필요
- **24/26 기능 스왑**: 클라이언트(EcsDefine 라벨+핑크/하늘색 색)는 24=출고/26=입고로 반영됨. 그러나 HOST_SIM.ini LOGIC1은 IN=124/OUT=126(원본 ECS 구성: C/V#13=출고 통과전용). 126은 물리 체류 없음(항상 0/0)이라 오표시 미노출. 기능까지 24=출고로 하려면 HOST_SIM.ini LOGIC1(IN=126/OUT=124)+EQP_SIM WorldModel(C/V#12 outgo/C/V#13 ingo) 협조 변경 필요 — EQP_SIM이 126 입고를 처리하도록 재구성해야 함(흐름 파손 위험). 사용자 확인 필요.
- **m_dicCvMove 트랙키 덮어쓰기**: 동일 크레인(예 904) 출고가 2.5s 내 연속 완료 시 라인CV(14) 표시가 일시 적체(~수초 고착) 후 자기복구. 스케줄 시 기존 항목 flush로 개선 가능(경미, 미적용).

빌드: IO_TASK(x86) 성공, 재기동, 6개 프로세스 안정. 백업: cThread_SCH.cs.freeze_bak, .gw_bak.

## 14. ★근본원인★ 라인CV 화물 미표시 = InvokeControl 조기 return (2026-07-12)

증상: 라인 컨베이어(113-120 등) 화물 전이(19→20,18→17,15→16,14→13)가 화면에 전혀 안 보이고 RTV↔SC가 즉시 넘어가듯 표현. DB엔 정상(sensor0 전이 확인)인데 클라이언트 미표시. 여러 라운드 "화물 안 보임" 반복의 진짜 원인.

원인(에이전트 조사로 확정): `CTrackInfo::InvokeControl`(TrackInfo.cpp ~164)이
```
if (GetItemSize() != m_arrayStatus.GetSize() || GetItemSize() != m_mapStatusInfos.GetCount()) return;
```
로 **조기 return**. 라인트랙은 EcsLayout에 아이템 1개지만 EcsDefine `<Status>`가 없어 상태수=0 → 불일치 → return → **m_bExist(앰버)·배경색·InvalidateControl 미실행**. 스테이션(Status 有, 개수 일치)만 통과해 렌더링. 트랙 컨트롤/CV_DATA/레이아웃 위치는 모두 정상 존재(EcsDefine.cpp 227-248, EcsLayout1.xml 173-194) — 오직 이 가드가 문제.

수정(TrackInfo.cpp): 조기 return 제거 → `BOOL bStatusMatch=(...)`. m_clrBgColor/m_bExist(181-182)·InvalidateControl(278)은 **무조건 실행**. 상태 아이템 색상 for루프(189)만 `if(bStatusMatch)`로 가드(m_arrayStatus[i] OOB 방지). Ecs 전체 재빌드·재기동.

효과: 라인 컨베이어 전 구간 화물 앰버가 이동에 따라 표시 → 15→16,14→13,19→20,18→17 및 RTV→라인→SC 중간 단계 가시화. (스테이션은 count 일치라 종전과 동일 동작.)
부수: 사용자가 본 24/26 녹색은 구(舊) 바이너리 잔상(현 소스로는 DARK_GRAY/gray라 녹색 불가) — 재빌드로 해소.
백업: TrackInfo.cpp.linetrack_bak.

## 15. 라인CV 화물색 = 작업유형(입고 녹색/출고 파랑) (2026-07-12)

§14 수정으로 라인CV 화물이 표시되기 시작하니, 화물이 **작업 무관 노란색(앰버)**으로 표시되고 track간 이동이 아니라 누적처럼 보이는 문제. 사용자 요구: 입고=초록, 출고=파랑.
- 원인: WriteCvSensor가 SENSOR0/LUGG만 써서 JOB_TYP_RD=0 → GetCvColor가 회색 → 앰버(m_bExist)가 노란색으로 덮음.
- 수정 1(IO_TASK cThread_SCH.cs): `WriteCvSensor(track,sensor,lugg,jobTyp="0")` 확장. 화물(sensor=1) 시 **JOB_TYP_RD = 입고 '11'(SemiSto→GREEN) / 출고 '12'(SemiRet→BLUE)** + AUTO_MODE_RD='1', 클리어 시 '0'. 호출부: RGV드롭·라인픽업=입고'1', 출고 라인/게이트웨이=출고'2' 전달.
- 수정 2(DciLib DciTrackCtrl.cpp): m_bExist 화물채움 `RGB(255,200,0)`(앰버)→**`m_clrBgColor`**(작업유형색). 4모서리 검정 팔레트 마커는 유지 → 작업유형 없는 화물(게이트웨이 j0)도 코너로 가시.
- 검증(실측): 입고 3226 `115:j11→116:j11`(녹색, 15→16 이동), 출고 3227 `114:j12→113:j12`(파랑, 14→13) + 게이트웨이 121→122. 단일 파렛트 이동, 누적 없음(트랙 [0]으로 복귀).
- 잔여: 게이트웨이 인바운드(121/122)는 EQP_SIM 소유(JOB_TYP=0/1)라 회색/민트 — 코너로 보이나 초록 통일하려면 EQP_SIM 또는 IO_TASK가 121/122에 JOB_TYP 기록 필요(후속).

빌드: DciLib.dll·IO_TASK(x86) 성공, 재기동, 6개 프로세스 안정.
백업: cThread_SCH.cs.jobcolor_bak, DciTrackCtrl.cpp.jobcolor_bak.

## 16. 출고 크레인 H/S 완료 + 잔여 피드백 (2026-07-12, 4차)

### 16.1 [수정] 출고 크레인은 H/S(bay0) 도달 시에만 완료 (`cThread_SCH.cs` AutoRunSC)
- 문제: 출고 RUN→완료가 `SC_AUTO_COMPLETE_MS` 경과만으로 발동 → 크레인이 bay0(출고 H/S)에 도달하기 전에 완료되어 랙 중간에 멈춤.
- 수정: 완료 조건에 `(jobTyp != "2" || m_dicCraneCur["S"+startPos] <= 0)` 추가 → 출고는 크레인이 bay0 도달해야 완료. 검증: 출고 잡이 SC레그(25) 고착 없이 CV레그(10/15)로 흐름(hang 없음).

### 16.2 [잔여] 게이트웨이(21/22)·RTV 협조 필요 — EQP_SIM/IO_TASK 소유 분리 문제
사용자 피드백(미해결), 근본은 게이트웨이 C/V#11(121/122)이 EQP_SIM 소유이고 RGV/라인/크레인은 IO_TASK 소유라 클리어·시퀀싱 협조가 안 됨:
- 입고: (1)화물이 22 출발해도 122 데이터 미클리어, (2)RGV 픽업했는데 21(121) 데이터 잔존, (3)RTV 주행 위치 불일치, (4)라인 15→16 이동 완료 전에 SC 지시(15에서 지시→SC 이동 중 화물 16 도착).
- 출고: (6)출고 화물 노란색(→JOB_TYP=12 청색 수정 후 재확인 필요), (7)출고 RTV 미동작(출고 RGV 레그 미구현)→21 이송 인지 어려움, (8)22 도착 후 화물 2s·색 2s 단계 정리 필요(현재 22 색 잔존).
- 해결안(제안): **IO_TASK가 게이트웨이 121/122 애니메이션을 입·출고 모두 소유**(EQP_SIM 게이트 피드 비활성/조정) + **출고 RGV 레그 신설**(113 픽업→121 반송→121→122) + 2단계 정리. 범위가 커 사용자 확인 후 착수.

빌드: IO_TASK(x86) 성공, 재기동, 6개 프로세스 안정. 백업: cThread_SCH.cs.craneHS_bak.

## 17. 유휴 트랙 지속 색상 제거 = GetCvColor 게이트 (2026-07-12, 5차)

증상: 화물이 지나간 뒤에도 트랙에 색(청록/녹색)이 남아 안 지워짐. 11/12처럼 중립 회색이어야.
원인(실측): `GetCvColor`(TrackInfo.cpp) 말단이 `if (DEST_POS_RD != "000") return MINT;` — 잔여 DEST_POS가 **"0"**(≠"000" 문자열)면 MINT(청록) 반환. 124/126이 DEST="0"이라 청록. 또 122는 JOB_TYP=1 잔존→LIGHT_MINT(녹). 화물(sensor0) 없는데도 잔여 필드로 색이 남음.
수정: GetCvColor 상단(에러/바이패스 체크 후, 작업유형 switch 전)에 **`if (V_SENSOR0_DATA_RD != "1") return LIGHT_GRAY;`** 게이트 추가. → **화물 없으면 무조건 중립 회색(11/12처럼), 화물 있을 때만 작업색(녹/청)**. 잔여 JOB_TYP/DEST_POS 무시.
효과: 122/124/126/106-112 등 유휴 트랙 모두 중립화. 화물 있는 트랙만 초록(입고)/파랑(출고). Ecs 재빌드·재기동.
잔여: 121에 파렛트(sensor0=1) 고착(RGV 픽업이 121 미클리어=이슈2)은 실제 화물이라 게이트로 안 지워짐 — 게이트웨이 플로우 재설계 대상.
백업: TrackInfo.cpp.idlegray_bak.

## 18. 입고 시퀀싱: 15→16 완료 후 SC 지시 (2026-07-12, 6차)

문제: RGV가 15 드롭 후, SC가 15 시점에 지시되어 SC 이동 중에 화물이 16으로 감(순서 뒤엉킴). 사용자 요구: 화물 16 도착 후 SC 지시.
수정(cThread_SCH.cs):
- CvMovePend에 `JobTyp`/`NoClear` 필드 추가, ProcessCvMove가 사용(색·자동정리 제어).
- **RGV 드롭(35→'20')**: 15 화물 쓴 뒤 `m_dicCvMove[15]={+2.5s, 16, JobTyp=1, NoClear=true}` 예약 → 15→16 자동 진행(16은 SC 픽업 시 클리어하므로 NoClear).
- **SC 지시(ST_SC_WAIT 20→21)**: 입고는 `IsTrackEmpty(16)` 이면 `continue`(대기) → **16에 화물 도착해야 지시**.
- **SC 중(21→25)**: 입고 라인이동 제거(RGV 레그가 완료), 대신 16 클리어(SC 픽업).
검증(실측): 904 `115`(드롭,SC無)→`116`+`S904 st2`(도착 후 지시)→픽업. 905 동일(`119`→`120`+지시). 순서 정상.
백업: cThread_SCH.cs.seq_bak.

## 19. 아이템 표시 + 출고 RTV 게이트웨이 시퀀스 (2026-07-12, 7차)

### 19.1 [수정] 화물 있어도 내부 아이템 표시 (`DciTrackCtrl.cpp`)
- 문제: m_bExist 화물채움 블록이 두 곳(아이템루프 前 170, **後 315**)에 있어 두번째가 스테이션 아이템(핑크/하늘색)을 덮음 → 작업 중 아이템 안 보임.
- 수정: 두번째 블록(315) `if (m_bExist)` → `if (m_bExist && 0)` 비활성. 170 블록이 화물 배경색 유지, 아이템은 그 위에 표시. (라인트랙은 210에서 조기 return이라 315 미도달 → 영향 없음). DciLib 재빌드.

### 19.2 [수정] 출고 RTV 게이트웨이 시퀀스 (`cThread_SCH.cs`)
- 문제: 출고 시 RTV에 데이터 없음(미동작), 21/22가 SC 드롭 시점에 조기 점등(CompleteCV 즉시 write).
- 수정: CompleteCV 즉시 write 제거. SC 완료 시 `m_dicOutGw` 시퀀스 예약(한 번에 하나). `ProcessOutGw`:
  - stage0: 라인 홀수(예 117) 픽업 → RTV 반송(`UpdateRtvVehicle` status2/sensor1/JOB_TYP2, posCell=14=121, 파랑) → RTV가 셀14로 스텝 이동
  - stage1: 121 하역(파랑), RTV 빈차 복귀 / stage2: 21→22 배출 / stage3: 22 정리(게이트로 회색)
  - 114→113 이동은 NoClear=true(RTV 픽업 시 클리어). 시작 지연 +5s(라인 홀수 체류 확보).
- 검증(실측): 출고 3391 — SC드롭 118 → RTV `p10→12→14 s1 L3391`(파랑 반송, 데이터표시!) → 121/122 파랑. **RTV 이동+데이터 표시 확인**.
- ⚠ 한계: 게이트웨이 121/122가 EQP_SIM 인바운드 피드와 공유 → 121 잔존/122 flip 등 경합 아티팩트. 근본 해결은 IO_TASK가 게이트웨이 완전 소유(EQP_SIM 피드 조정) 필요.

빌드: DciLib·IO_TASK 성공, 재기동, 6개 프로세스 안정.
백업: DciTrackCtrl.cpp.itemshow_bak, cThread_SCH.cs.outrtv_bak.

## 20. 출고 게이트웨이 충돌 가드 + 근본 병목 (2026-07-12, 8차)

문제(사용자): 출고 시 RTV에 화물 싣기 전 21/22에 입고 화물이 있으면 출고하지 말아야 하는데 바로 진행 → 21/22에서 충돌(입고 작업 덮어씀).
수정(cThread_SCH.cs ProcessOutGw): 각 게이트웨이 단계에 **비었을 때만 진행** 가드 추가 — stage0(출고 시작): `!IsTrackEmpty("121")||!IsTrackEmpty("122")` 면 대기(출고 시작 안 함, RTV도 안 잡음), stage1(121 하역): 121 빌 때만, stage2(22 배출): 122 빌 때만. → 파괴적 충돌 완화(출고가 입고에 양보).
검증: 작업 흐름 정상(가드는 시각 시퀀스만 제어, JOB_MST 흐름 무관). 출고는 21/22 점유 시 대기.

★ 근본 병목 확정: **EQP_SIM이 게이트웨이(C/V#11 22)에 입고 파렛트를 FEED_MS=5000(5초)마다 공급, 21에 WAITOUT_HOLD_MS=9000(9초) 유지** → 게이트웨이가 거의 상시 입고 점유 → 출고가 깨끗한 창을 못 얻어 게이트 애니메이션이 드물게 발동/여전히 j2↔j1 순간 경합.
→ 정의적 해결: **IO_TASK가 게이트웨이 121/122 완전 소유**(EQP_SIM C/V#11 입고 피드 비활성, IO_TASK가 입고 잡 시작 시 122 생성·구동) → 입출고가 한 주체에서 조율. 상당한 재설계(EQP_SIM+IO_TASK)라 사용자 승인 후 착수 권장. 임시로 EQP_SIM FEED_MS↑/WAITOUT_HOLD↓로 창 확보 가능(부분책).
백업: cThread_SCH.cs.gwguard_bak.

## 21. 게이트웨이 IO_TASK 소유 재설계 시도 → 되돌림 + 체인 정지 (2026-07-13)

### 시도한 것
- WCS_TASK_CV(CvThread.UpdateCvData)에 `if(nCvNo=="121"||"122") return true;` 가드로 121/122 _RD 브리지 제외 → IO_TASK 소유. (WCS_DB.INI CNT=1이라 121/122만 브리지되던 상태에서 코드 가드 선택; EQP_MST 상태는 유지)
- IO_TASK에 입고 게이트웨이 시퀀스(m_dicInGw: 22→21 후 RGV 픽업) + 입고 CV레그 SENSOR0 요구 제거.

### 왜 되돌렸나 (근본)
- **입고 CV 레그(DriveCV/CompleteCV)가 WCS 브리지에 의존**: 입력스테이션(122) 파렛트 감지(SENSOR0)와 22→21 이동이 EQP_SIM↔WCS 경로로 이뤄짐. WCS 가드가 이를 끊자 입고·출고 CV레그가 모두 정지(잡이 '10'/'15'에 고착). 즉 게이트웨이 소유는 브리지 스킵만으로 안 되고 **입고 CV레그까지 IO_TASK로 재작성**해야 하는 더 큰 작업.
- 조치: WCS_TASK_CV.cs / cThread_SCH.cs 를 **작동 백업(gwown_bak / ingw_bak)으로 복원**하고 재빌드. (WCS 가드·입고게이트웨이·조건변경 모두 제거)

### 현재 체인 상태 (미해결, 핸즈온 필요)
- 세션 중 체인 4개(EQP_SIM/HOST_SIM/IO_TASK/TASK_LFC10)가 한 번 죽었고(원인 불명), 이후 여러 번 정상순서 재기동·eqp_state.txt 클리어·HOST_SIM [시작]·WCS_TASK_CV 재연결(EQP_MST PLC11 CONNECTED_YN='Y' 신선 확인)에도 **체인이 안 흐름**: EQP_SIM이 파렛트를 안 흘리고(화물 0), 잡이 22:20:59 이후 미갱신.
- EQP_SIM은 실행·리스닝(포트 2004/3001) 정상, WCS PLC11 연결됨. 그런데 피드/진행이 멈춤 = **크래시 잔여 상태 문제**(원격 진단 한계). 프로세스 UI(EQP_SIM 피드상태, HOST_SIM 발행, IO_TASK 스케줄러 에러) 확인 또는 **머신 재부팅 후 순서 재기동** 권장.
- 코드는 작동 백업 상태로 복원됨 → 체인이 다시 흐르면 재설계 이전과 동일 동작.

### 클라이언트 개선은 그대로 유효 (체인 무관)
- §14~19의 Ecs/DciLib 수정(라인트랙 렌더 InvokeControl 가드, 유휴 중립회색 게이트, 작업유형 화물색, 아이템 표시, 트랙번호 등)은 클라이언트 빌드에 반영되어 있음.

### 재설계 재개 시 (별도 세션)
- 입고 CV레그를 IO_TASK가 직접 구동(브리지 비의존)하도록 재작성 + 입출 게이트웨이 단일 소유. 또는 간이책: EQP_SIM FEED_MS↑/WAITOUT↓ + 기존 ProcessOutGw 가드로 충돌만 완화.
