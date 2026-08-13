# SIMs + WCS 시스템 실구동 검증 (2026-07-18/19, Renewal 통합본)

## 결과: 로직1·3 입고(F1)+출고(F2) 풀사이클 반복 완주 — 6개 프로세스 안정
- 체인: HOST_SIM(9910/9911) ⇄ HOST_TASK(TASK_LFC10_G1_ECSCOM) ⇄ [LGLS_MCS_IO] ⇄ IO_TASK ⇄ [DB] ⇄ EQP_TASK(WCS_TASK_CV, COMM0=127.0.0.1:2004) ⇄ EQP_SIM + WCS Client(Ecs.exe)
- 최종 런 집계: F1(입고완료) 3건, F2(출고최종 step2) 3건, NAK 0. 로직1 완료 2회+진행, 로직3 완료 1회+진행.
- 기동 순서: EQP_SIM → WCS_TASK_CV → HOST_TASK → IO_TASK → HOST_SIM([시작]) → Ecs.exe

## 현행 설계 확인 (7/14~17 재설계 — 이전 문서 미기재)
- **EQP_TASK는 마스터 PLC 1소켓(COMM0, C/V#11 게이트웨이 121/122)만 브리지** → `WCS_DB.INI [PROCESS] CNT=1`이 정상값.
  COMM0 IP: 실PLC=192.100.1.110 / SIM=127.0.0.1 (INI로만 전환).
- 스테이션 124/126(비브리지)·라인CV·SC/RGV는 IO_TASK가 DB 직접 구동
  (FeedInGate: 입고대 122/126 공급·readback 재현·입고대→픽업트랙 이송 / AutoRunRGV·AutoRunSC / ProcessRvSeq·OutPend·OutGw / StepCranes).
- EQP_TASK FenetProtocol은 [BIN판](XGT 공식 바이너리 DATACOUNT·LENGTH 체크섬) — 실PLC 검증본, 수정 금지.

## 이번 수정 (모두 WCS 시스템/SIM 측 — 구 ECS·EQP_TASK 통신부 무수정)
1. **EQP_SIM `[MODE] LEGACY_HOOKS=0` 신설(기본 OFF)**: 구(§8~13) 설계용 재현/픽업 훅 3종이
   현행 설계와 충돌 (122 입고 트래킹 기록을 '출고 재현'으로 오인 → 파렛트 즉시 배출 → 입고 레그 교착).
   WCS 모드에서 D영역 미러만 유지(브리지 AUTO비트 공급), 훅은 LEGACY_HOOKS=1일 때만.
2. **EQP_SIM WCS 모드 물리 투입 전부 OFF** (`[FEED] CV11~15=0`) — 공급은 FeedInGate 담당.
   AutoFeed 체크박스가 INI 개별설정(CVnn=0)을 무시하던 버그 수정(AutoFeedConfigured).
3. **XgtServer 주소/카운트 해석 정정**: 접미 'W'(V0.9 워드주소·바이너리 카운트)만 ×2,
   'B'(구 ECS·V1.1 바이트주소)는 그대로 + "0###"/바이너리 카운트 자동 판별. (구 ECS 검증과 공용)
4. WCS_DB.INI: COMM0 IP=127.0.0.1(SIM), CNT=1 유지.

## 후속 반영 (2026-07-19)
- **로직2(C/V#15, 입고대 130/출고대 129) 지원 완료** — 별도 세션에서 FeedInGate 130 확장
  (공급·refresh·이송 + RgvPickupTrack 130→131 특례 + RgvCell 131→plc10). 3로직 동시 풀사이클 검증됨.
- **★출고 시퀀스 교착 근본 수정★** (`cThread_SCH.cs` ProcessOutGw Stage 2):
  하역 4단계 시퀀스(RvSeq Phase2)가 하역트랙에 화물을 놓는 순간, Stage 2 재진입 가드
  `!IsTrackEmpty(pickup)`가 **자기 자신이 놓은 화물에 막혀 영구 대기** → RTV RUN 고착,
  m_dicOutGw 미해제 → FeedInGate 신규 공급 전면 보류 → 체인 정지("IO_TASK 재시작하면 풀리는" 증상의 진짜 원인).
  → 빈 트랙 검사를 시퀀스 시작 전(RvSeqFresh)에만 하도록 수정. 이후 3로직 동시 지속 가동
  (재시작 없이 F1 10건/F2 9건, 로직1·2·3 완료 3/2/4회, RTV IDLE 복귀, NAK 0) 실증.
- **CompleteCV 빈트랙 게이트 보강**: `LUGG_NO_RD='0000'` 단일 비교 → NULL/'0'/'' 도 빈 트랙으로 인정
  (수동 초기화 등 표기 차이로 완료 전이가 영구 대기하는 것 방지).
- **★출고대 129 하역트랙 오매핑 수정★** (2026-07-19, "화물이 RTV를 안 탄다" 증상):
  `RgvPickupTrack`의 "홀수면 그대로" 규칙이 **홀수 출고대 129(C/V#14=트랙 127/128/129)**에서
  하역트랙=129(자기 자신)를 반환 → RTV가 하역한 화물이 배출트랙을 스스로 점유 → Stage 3 영구 차단
  → m_dicOutGw 미해제 → RTV 운반 전면 정지 + FeedInGate 공급 보류(두 번째 교착 지점).
  → 129→**127**(실배치 RGV 하역 지점) 예외 추가, RgvCell 127=plc7, 좌초화물 복구 대상에 127/129 추가.
  또한 좌초화물 복구가 **입고 이송 화물(121 대기)을 출고 좌초로 오인**해 배출하던 문제 →
  진행 중 입고 작업(JOB_TYP=1)의 화물은 건너뛰는 가드(IsActiveInboundJob) 추가.
  실증: 수정 후 4분간 로직1/2/3 완료 8/7/8회(수정 전 3/2/4에서 정체), RTV 적재·주행·하역 사이클 정상 표시.
- **RTV 상호배타(1대 1작업) 추가** (2026-07-19, "RTV 색 깜빡임/동시 픽업·하역" 증상):
  입고 RGV 레그(AutoRunRGV)와 출고 게이트웨이 시퀀스(ProcessOutGw)가 조율 없이 RTV 1대를 동시 점유
  → 입고색↔출고색 교대, 두 트랙 화물 동시 소멸/동시 등장. → `RtvBusyByOutbound()`(OutGw Stage 0~2 존재)
  이면 입고 지시(30→31) 보류, `RtvBusyByInbound()`(입고 31/35 존재)이면 출고 시퀀스 시작 보류.
  실측(60초 샘플링): 출고 1건(빈차이동→적재→운반→하역, jt=2 유지) 완결 후에야 입고 1건(jt=1) 시작 —
  한 운반 중 작업유형/화물이 일정, 교대 동작 완전 직렬화.
- **RTV "도착 스냅"(순간이동) 제거 → 실제 도착 후 적재/하역** (2026-07-19, "드롭 위치 표시 어긋남" 증상):
  적재 시퀀스(~6s) 때문에 운반(35) 전이 시점엔 드롭 타이머(RGV_DONE_MS 6.5s)가 이미 만료 →
  드롭 분기가 즉시 발동하며 도착 스냅이 내부 위치만 목표로 맞춰 StepCranes 가 POS_H 를 더 안 씀 →
  RTV 가 픽업/중간 위치(pos 9~10)에 표시된 채 드롭 트랙(예: 로직2 7번=107)에 화물 등장.
  → 스냅 4곳(입고 픽업/입고 드롭/출고 적재 Stage1/출고 하역 Stage2)을 "주행이 목표 셀에 실제 도착할 때까지
  대기(m_dicCraneTgt 재확인)"로 교체. 실측: 적재는 라인 셀 도착 후(pos 8=109), 하역은 출고대 셀 도착 후(pos 7=127),
  입고 드롭도 드롭 셀 도착 후 — 전 구간 위치 정합.

## 설비 정지(suspend/일시정지) 게이트 구현 (2026-07-19)
- **HOST_SIM**: WCS 설비별 S 상태보고(BODY 15)에서 SC 상태(4=오프라인/5=에러/6=입고정지/7=출고정지/8=전체정지)를
  파싱해 상태표 유지. 입고 생성 시 대상 셀의 담당 SC 가 4/5/6/8 이면 **그 크레인의 셀을 건너뛰고**, 담당 전체가
  불가면 "대기 — 담당 크레인 정지/에러"로 보류 후 회복 시 자동 재개. (구 ECS 43B 집계 전문은 코드 의미가 달라 미사용 — 구 ECS 체인 불변)
- **IO_TASK** (cThread_SCH.cs):
  - SC suspend(1=입고정지/2=출고정지/3=입출고정지): AutoRunSC·DriveSC 지시 단계에서 해당 방향 지시 금지
  - RTV suspend(≠0): AutoRunRGV 지시·출고 게이트웨이 시퀀스 시작 금지
  - CV 일시정지(TR_PAUSE_OD/RD='1' — PLC 비트 대신 **내부값만** 사용): RTV 작업대→RTV 지시 금지,
    SC 작업대→SC 지시 금지, 입고대→입고 발행(DriveCV)·공급(FeedInGate)·이송 금지, 출고대→도착완료(CompleteCV 출고) 보류
  - **출고 H/S(SC 하역트랙)에 선행 화물이 있으면 SC 출고 지시 금지** (크레인이 하역 못 하고 대기하는 것 방지)
- **재기동 고아 복구 보강**:
  - SC 하역트랙(짝수 104/106/110/114/118) 좌초 출고화물 → 짝수→홀수 이동 예약 재구성(RecoverOutOrphans)
    ("14번 화물이 13번으로 안 가는" 증상 — 완료된 출고의 이동 예약이 재기동으로 소실된 것)
  - RecoverOutOrphans 1부(홀수 복구)가 고아 0건이면 **메서드 전체를 조기 return** 해 짝수/출고대 복구까지
    건너뛰던 버그 수정.
  - 검증: 좌초 1854 → 114→113 이송 → RTV 배출, 후속 1856~1860 F1/F2 정상 완주, 사이클 재순환.

## MANUAL 작업 대화상자(CManualJob) 개선 (2026-07-19)
- **생성 불가 원인 3중 수정**: ① INSERT의 `nextval('manual_seq')`(PostgreSQL 잔재)가 SQL Server에서 실패 →
  9001~9900 순환 채번 서브쿼리로 교체(9900 도달 시 9001 재순환, 트랜잭션 내 다건도 순차),
  ② `GetQrySelectCELLMST`가 존재하지 않는 `SC_NO` 컬럼 조회 → `CELL_SC_NO AS SC_NO` 별칭,
  ③ BANK/BAY/LEV 자릿수 불일치(대화상자 3자리 vs 테이블 2자리) → CAST INT 비교.
  DB 보강: CELL_MST에 SC_PLT_JOB_TYP 컬럼 추가(cv_data와 동일 '0' 정렬).
- **반자동(10~15) 유형**: 활성/비활성·검증을 자동 유형과 동일 규칙으로 매핑(10→6 작업대이동, 11~15→1~5).
  INSERT 시 JOB_TYP은 원래 코드(10~15) 유지.
- **PULP단수·비고 삭제 + 좌우 정렬**: 리소스(IDD_MANAUL_JOB)에서 두 컨트롤 제거, 작업개수를 우측 열로 이동해
  4×2 격자 정렬, 대화상자/그룹박스 축소. 코드 참조(DDX/BindCombo/검증/refresh) 정리.
- **UI 자동화 실검증**: 메뉴(35046)→비밀번호(0000)→[11]반자동입고(122→셀01-002-01) 생성 → **9001**/TYP 11,
  [12]반자동출고(셀01-001-01→122) → **9002**/TYP 12. 활성화 상태 실측: 반자동 6종 모두 대응 자동 유형과 일치.
- IO_TASK 미지원 JOB_TYP 로깅을 작업당 1회로 (반복 스팸 방지). ※ 반자동 작업의 스케줄러(IO_TASK) 반송 처리는
  별도 과제(현재 접수 안 함 — 커버리지 문서 JOB_TYP 4/6과 동일 부류).

## 반자동 다크 색상/범례·정지 표시 (2026-07-19)
- **반자동(10~15) 색상 분리**: Config에 USER_COLOR_SEMI_STO/RET/MOVE/RTR/ATA/PR 신설(기본=자동의 다크 변형:
  다크그린/다크블루/다크시안/다크마젠타/…), Ecs.ini [USER] 저장/로드, GetCvColor의 Semi 케이스를 새 색으로 전환
  (+SemiW2W 케이스 추가). **범례(IDD_CONFIG_STATUS)에 "반자동 작업" 그룹 신설**(반자동입고/출고/이동/랙투랙/호기간
  5항목, 색 변경·저장·초기화 연동. BTN ID는 레거시 1676~1680 재사용 + ATA 28807).
- **연쇄 정정 2건**: ① IO_TASK WriteCvSensor가 자동 화물색으로 차용하던 11/12(반자동 코드) → 1/2 정정
  (+클라이언트 AutoSto의 TRAY_LEV 분기 제거 — LGLS 미사용), ② **EQP_TASK 수신 파싱 보정**: PLC 규격 작업구분
  11(입고)/12(출고) readback → 표준 1/2 정규화 (파싱만 수정 — 통신 프레임/쓰기 로직 무변경, 허용 범위).
  실측: 게이트웨이 121/122 포함 전 트랙 자동 화물 jt=1/2.
- **SC 대화상자 SC_SUSPEND 표시**: 기존 '1'만 "정지" 표시 → 1=입고정지/2=출고정지/3=입출고정지 모두 표시 + 체크박스.
- **CV 일시정지 트랙 표시**: GetCvColor에 TR_PAUSE_RD/OD='1' 분기 추가 — 트랙 전체를 범례 일시정지(입출고금지)
  색으로 표시 (기존엔 스테이션 내부 아이템만 반영, 라인 트랙은 무표시였음).
  → **7/19 사용자 피드백으로 롤백**: 트랙 전체색은 에러 표현과 혼동 — 아래 "CV 일시정지 아이템 색" 참조.

## SC 출고 잔여 수 표시 + 입/출고대 ON 규칙 + CV 일시정지 아이템 색 (2026-07-19)
- **[ETC] ViewRetCnt=1 (Ecs.ini)**: SC1~5의 출고 잔여 작업 수를 출고 라인 RTV측 트랙(101/105/109/113/117)에
  실시간 표시. 0=검정 / 1 이상=파랑. ViewRetCnt=0이면 미표시. 집계: JOB_MST JOB_TYP='2', START_POS 901~905,
  JOB_STATUS NOT IN (9,19,29), DEL_YN<>'Y' — 2초 주기 캐시(ApplyRetCntDisplay, TrackInfo.cpp).
  - DciLib: CDciTrackCtrl에 m_strExtraText/m_clrExtraText 신설 — 값이 있으면 트랙번호 대신 표시.
    ※ DrawFontText의 마지막 인자는 "복원용" 색 — 그리기 색은 호출 전 SetTextColor로 지정해야 함(초기 구현 함정).
  - **갱신 트리거 주의**: CTrackInfo::InvokeControl은 m_bModified=FALSE면 조기 return → 트랙 데이터가 안 변하면
    표시가 정체됨. 잔여 수 갱신을 조기 return 앞으로 이동 + 변경 시 자체 InvalidateControl.
  - 실측: 테스트 작업 삽입/삭제 시 재시작 없이 1(파랑)↔0(검정) 실시간 전환. ViewRetCnt=0에서 완전 미표시.
- **입고대 ON 규칙 변경**: (기존) 화물 감지 즉시 핑크 → (변경) "데이터 없는" 화물이 감지된 지 **2초 경과** 시 핑크
  (IsStoReadyOn, m_dwStoOnTick). 시간 경과 전이는 m_bModified와 무관하므로 상태 전이 감지(m_bLastStoOn) 시
  강제 재갱신 로직 추가. 실측: 감지 1.3초=회색 유지 → 4.3초=핑크 전환(재시작 없이).
- **출고대 ON 규칙 변경**: (기존) 데이터+화물+목적지=자기 트랙 조건 → (변경) **데이터+화물만**으로 하늘색 ON
  (이 현장은 목적지 개념 없음 — dest/jobtyp 조건 제거). 실측: 129 하늘색 확인.
- **CV 일시정지 아이템 색(피드백 반영)**: 트랙 전체색 분기 제거(롤백), 스테이션 내부 아이템 색을
  TR_PAUSE_RD/OD='1'이면 범례 일시정지(USER_COLOR_SUSPEND) 색으로 표시 (GetCvColor Semi/Auto 케이스 11곳).
- 구 ECS의 SC 주행위치 수신 여부(질의 응답): 가능 — Vehicle 관측치 SUBSYSTEM_LOCATION_01~03
  (Backup\ECS\Device\Unit\Vehicle.cs 구독, 마스터 PLC 메모리맵 PPT 슬라이드 18~26).

## 풀사이클 재검증 + 일시정지 표시 결함 2건 정정 (2026-07-19 저녁)
- **클린 재기동 후 로직1·2·3 풀사이클**: 완료 로직1=6회/로직2=4회/로직3=3회(총 13사이클), 정지 시
  진행 중이던 입고 3건(2051/2053/2055)도 21→30→31→35→완료로 자연 소진. 교착·재시작 없음, 6프로세스 안정.
- **실사이클 중 신규 표시 실측**:
  - SC 출고 잔여 수: 출고 레그 진행 중 트랙 101에 파랑 "1" 실시간 표시(타 SC는 검정 "0") — run1 캡처.
  - 출고대 124: 파랑 화물(자동출고) + 하늘색 아이템 ON(데이터+화물만, 목적지 무관) — st_1024 캡처.
  - 입고대 130: 녹색 화물(자동입고) + 아이템 OFF(데이터 있는 화물은 핑크 아님 — 규칙대로) — st_1030 캡처.
- **결함 발견·정정 2건 (일시정지 아이템 색 검증 중)**:
  1. GetCvColor의 "트랙 전체 일시정지 색" 분기(ALL_SUSPEND)가 **롤백 기록과 달리 잔존** → 제거
     (TrackInfo.cpp — RD 일시정지 시 트랙 전체가 빨강으로 뒤덮이던 원인).
  2. 클라이언트 CV 조회 쿼리가 **TR_PAUSE_RD만 SELECT하고 TR_PAUSE_OD 미조회** → V_TR_PAUSE_OD가 항상
     공백이라 출고 일시정지가 화면에 전혀 반영 안 됨. Cv.cpp SELECT + SetTR_PAUSE_OD 호출 추가
     (CV_DATA::SetTR_PAUSE_OD는 기존 구현 존재 — m_bModified 처리 포함).
  - 정정 후 실측: 129(OD 일시정지)·130(RD 일시정지) 모두 화물 배경 유지 + 내부 아이템만 범례 일시정지(빨강),
    RD 일시정지가 입고대 핑크(2초 규칙)보다 우선 — pause_items3 캡처.
- 주의: 클라이언트 재기동 전후로 tr_pause_rd가 0으로 초기화되는 현상 1회 관측(재현 안 됨, 값 유지 확인) —
  실운영에서 일시정지는 CV 대화상자로만 조작하므로 영향 없음.

## ViewRetCnt 표시 방식 변경 — 전용 CDciStaticCtrl (2026-07-19 밤, 사용자 피드백)
- 피드백: "트랙 번호를 바꿔치기하는 식으로 하지 말고, CV#X 라벨과 같은 CDciStaticCtrl을 추가해서 표시".
- **EcsLayout1.xml**: RACK_INFO 그룹에 static 5개 신설 — id **17199901~05**(유형코드 99=enOther,
  CV/SC/LGV 클릭·바인딩 로직에 안 걸림), 위치 = 각 SC 레일 기둥 하단 빈 셀(l=4/7/10/13/16, b=7~8,
  출고 라인 트랙 101/105/109/113/117 바로 옆). 백업 .bak_retcnt.
- **TrackInfo.cpp ApplyRetCntDisplay 재작성**: DciTrackCtrl m_strExtraText 대신
  m_pDoc->GetDciControl("171999NN")로 static을 찾아(1회 캐시) m_strText/m_clrFgColor 갱신 + 자체 InvalidateControl.
  집계 쿼리·2초 캐시·0=검정/1+=파랑·ViewRetCnt=0이면 빈 문자열 로직은 동일. 호출 트리거는 기존과 같이
  출고 라인 트랙의 InvokeControl(조기 return 앞). DciLib의 m_strExtraText 확장은 미사용 상태로 잔존(무해).
- 실측: 테스트 출고 작업(SC1 2건/SC3 1건) 삽입 → SC1 static 파랑 "2", SC3 파랑 "1", 나머지 검정 "0";
  삭제 → 전부 검정 "0" 실시간 복귀. 트랙 박스 표시는 원래 동작으로 복원.
- 참고: 트랙 박스 안 트랙번호 표시는 원래부터 모니터링 메뉴(1F) 클릭마다 토글되는 기존 동작
  (EcsLayout::OnViewLayoutInfo — WM_USER_REFRESH_LAYOUT). 신규 기능과 무관.
- 일시정지 아이템 색 재확인(사용자 "조치 안 됨" 문의): 출고대 129 일시정지 시 화물 배경 유지 + 내부 아이템만
  범례 일시정지 색 — 직전 섹션의 결함 2건 수정으로 이미 조치 완료된 상태임을 재실측(final_pause).

## 출고 잔여 수 "N개" 형식 + CV 일시정지 대화상자 경로 end-to-end 검증 (2026-07-19 밤 2차)
- **표시 형식**: ApplyRetCntDisplay 출력 "0"/"1" → **"0개"/"1개"** (색 규칙 동일: 0개=검정, 1개 이상=파랑). 실측 완료.
- **CV 일시정지 "표시 안 됨" 재현 시도** — 실제 대화상자 경로를 UI 자동화로 끝까지 구동:
  트랙 124 컨트롤 클릭(WM_USER_CONTROL_CLICK, wParam=17110124) → CvSkinDlg 열림 → 일시정지 버튼(IDC 1091)
  → "설정 하시겠습니까?" 예 → "성공" → **DB TR_PAUSE_OD/RD='1' + 화면 아이템 빨강 정상**. 해제 토글도 정상.
  브리지 트랙 122(입출고대)도 DB 경로로 표시·값 유지 확인(EQP_TASK가 OD를 PLC로 쓰고 리드백이 에코되어 RD 유지됨).
  → 결론: 현 빌드에서 대화상자→표시 경로 정상. 사용자가 본 증상은 TR_PAUSE_OD 미조회+전체색 분기 결함이 있던
  **수정 전 빌드**에서의 관측으로 판단. 참고: CvSkinDlg 일시정지 UPDATE는 0행 갱신이어도 "성공"이 뜨는 구조이니
  향후 유사 증상 시 CV_DATA의 TR_PAUSE_OD/RD 값부터 확인.
- 검증 후 정리: 테스트 작업/일시정지 원복, 체인 6종 재기동(EQP_SIM 외 4종이 외부 종료돼 있던 것 재기동 포함).

## 설비 작업대 일시정지 표시 + 리본 J/T/P 표시 모드 (2026-07-19 밤 3차)
- **설비 작업대(화살표 트랙) 일시정지 표시**: EcsDefine에 <Status> 정의가 없는 트랙(H/S·픽업 등 작업대)은
  아이템 색칠 루프가 스킵되어 일시정지가 무표시였음(사용자 신고 재현: 103/121/123). InvokeControl에
  상태 정의 없는 트랙 전용 처리 추가 — 일시정지 시 화살표 아이템을 범례 일시정지 색으로, 해제 시 원색 복원
  (CDWordArray m_arrOrigItemColors에 최초 1회 원색 보존). 실측: 123(사용자가 걸어둔 것)/103/121 빨강 표시 확인.
- **리본 '창고 모니터링' 버튼 3종 신설** (MainFrm.cpp, WH_TYP=10 한정):
  - 아이콘: `rc_resource\mainframe_monitor\j/t/p.png` 신규 생성(128x128, 검은 배경+흰 글자, 1f.png와 동일 규격).
  - [J] 작업번호(ID_MONITORING_VIEW_JOBNO=35066) / [T] 트랙번호(35067) / [P] 제품정보(35068) — resource.h.
  - 라우팅: EcsDoc ON_COMMAND_RANGE → OnCommandTrackTextMode → m_nTrackTextMode(0/1/2) 설정 후
    m_pEquipments.InvokeControl(TRUE)로 전 트랙 강제 갱신.
- **표시 로직**(TrackInfo::ApplyTrackTextMode, InvokeControl 말미에서 호출 — DciTrackCtrl m_strExtraText 재활용):
  - J: 트랙에 화물 데이터(LUGG_NO) 있으면 작업번호, 없으면 공백.
  - T: 트랙번호(끝 2자리) 명시 표시 — 기존 1F 메뉴의 트랙번호 토글 상태와 무관하게 항상 표시.
  - P: JOB_MST에서 LUGG_NO별 최신(INS_DT DESC) BCR_BOTTOM 조회(작업번호 변경 시 1회, 캐시).
    ※ DEL_YN 필터는 두지 않음(과거 이력 lugg도 최신 행 기준 표시). 긴 제품코드는 박스 폭을 넘어 표시됨(폰트 고정).
  - 실측: J="1478", P="201506307025", T=전 트랙 번호 표시 확인(트랙 127 데모).
- **아이콘 원형화 + 1F 제거 (2026-07-20, 사용자 요청)**: j/t/p.png를 투명 배경+검은 원+흰 글자로 재생성,
  MainFrm.cpp의 1F 버튼 생성 주석 처리(ID_MONITORING_1F 명령/레이아웃 로딩 자체는 유지 — 버튼만 제거).
- **반자동 입고 작업 PLC 미기록 원인(질의 응답)**: IO_TASK AcceptNewJob(cThread_SCH.cs ~317)이
  JOB_TYP '1'/'2'만 접수하고 반자동(10~15)은 "[SCH][NEW] 접수 불가 - 미지원 JOB_TYP" 1회 로깅 후 스킵 —
  기존에 미구현 영역으로 문서화된 부분. 사용자 작업 9001(TYP=11, 122→901)은 상태 99에서 접수되지 않아
  입고 발행(PLC 기록) 단계에 도달하지 않음. → 아래 구현으로 해소.

## 반자동 입고(11)/출고(12) 스케줄러 지원 (2026-07-20)
- **구현 방식** (cThread_SCH.cs, 백업 .bak_semi):
  1. SQL 필터 확장: `JM.JOB_TYP = '1'` → `IN ('1','11')` 5곳(CASE 포지션식/FeedInGate/입고 pause 게이트/SC SUSPEND 게이트 등),
     `JM.JOB_TYP = '2'` → `IN ('2','12')` 3곳, JM 접두사 없는 2곳(RtvBusyByInbound/IsActiveInboundJob) 동일 확장.
  2. C# 정규화: `jobTyp = GetVal(..., "JOB_TYP")` 8곳 직후 11→1, 12→2 정규화 — 이후 레그 라우팅/게이트/CV·SC·RTV
     지시 로직은 무변경으로 동작(JOB_MST 원본 JOB_TYP은 유지되어 클라이언트 색/조회는 반자동 그대로).
  3. CV/PLC 레벨 작업구분은 기존과 동일하게 1(입고)/2(출고) 정규값 사용 — EQP_TASK/PLC 프레임 무영향.
  4. 클라이언트 ViewRetCnt 집계 확장: `JOB_TYP = '2'` → `IN ('2','12')` (반자동 출고도 잔여 수에 포함).
- **실측 (풀사이클 4건 완주)**:
  - 9001(사용자 생성, 11, 122→901 셀 01-002-01): 99→…→29 완료, 이력 이관.
  - 9003(11): 99→15(CV)→31(RGV지시)→35(RGV운반)→20→21(SC)→29, 46초 — 자동 입고와 동일 레그.
  - 9002/9004(12, 901→124): 99→21→25(SC)→(CV 레그)→19 완료, 이력 이관. 출고 대기 중 SC1 잔여 "1개" 파랑 표시,
    SC 하역 화물 라인 재현 확인.
  - CELL_MST 재고(gdbox) 미기록은 자동 작업과 동일(재고는 상위 관리 — WCS 재고 제외 원칙대로).
- 미지원으로 남는 유형: 반자동이동(10)/PR(13)/R2R(14)/W2W(15), 자동이동(6) 등 — 필요 시 별도 라우팅 설계.

## 용어 정리 리팩터링 (2026-07-20, 사용자 요청 — 주석+식별자)
- **"게이트웨이"(PLC↔DB 통신자 의미) → "TASK프로그램"**: cThread_SCH.cs 주석 전체("TASK프로그램 부재 시뮬레이션",
  "실TASK프로그램 모드" 등), CSrvWork.cs(HOST_TASK), WCS_DB.INI 주석, 클라이언트 CvSkinDlg/MainFrm 주석.
- **"게이트웨이"(출고 RTV 반출 경로 의미) → "출고대 반출(경로/시퀀스/대기열)"**: 주석 전체 +
  식별자 개명 — `OutGwState`→`OutStnState`, `m_dicOutGw`→`m_dicOutStn`, `ProcessOutGw`→`ProcessOutStn`.
- **"레그" → "처리"**: "2-레그 체인"→"2단계 처리 체인", "CV/SC/RGV 레그"→"CV/SC/RGV 처리", `strLeg`→`strProc`.
- 이 문서의 이전 섹션들에 남은 옛 명칭(ProcessOutGw/m_dicOutGw/레그/게이트웨이)은 당시 기록 그대로 둠 —
  현행 소스 기준 명칭은 위와 같음. IO_TASK/HOST_TASK/Ecs 재빌드 후 반자동 입고(9005)·출고(9006) 스모크 완주로
  동작 무변화 확인.

## [진행 중] 물리 이관 프로젝트 — EQP_SIM 설비 구동 + EQP_TASK 3스레드 구조 (2026-07-21 시작)
- 사용자 확정 구조: EQP_SIM(DB 없음, PLC 시뮬레이터)에 SC/CV/RTV 설비별 처리 스레드,
  EQP_TASK(WCS_TASK_CV)에 SC/CV/RTV 통신 스레드(각각 SC_DATA/CV_DATA/RTV_DATA 갱신),
  IO_TASK는 지시(_OD)/상태만. 신규 TASK 프로그램 없이 기존 프로그램 내 스레드 추가.
- **M1 완료 (CV 전 트랙 브리지)**:
  1. EQP_SIM ScenarioEngine → CV/SC/RTV 3개 틱 스레드 분리(공유 월드 sync 락, 핸드셰이크 owner별 분담).
  2. WCS_DB.INI [PROCESS] CNT=15 — COMM0~4=스테이션(PLC11~15), COMM5~14=라인(PLC01~10, 트랙 101~120). 15접속 실증.
  3. CV_DATA에 CMD_RQ_PARM 컬럼 신설(스키마 누락 — 스테이션 COMM 스레드가 요구, 매 폴링 에러났었음).
  4. **관측값 번호 정렬**: EQP_SIM이 WCS 모드에서 PALLET_EXIST 플래그/R트래킹 번호를 PortOrder 대신
     트랙 순서(포트-최소포트+1)로 기록(ConveyorSim.ObsNo) — C/V#15(PortOrder 3/1/2)의 130/131/132 정렬 해결.
  5. CvThread 슬롯 상한 2→3 완화(프레임 무변경, READ 개수/매핑만) — 3트랙 C/V#14(129)/#15(132) 커버.
  6. 실증: 클린 기동 후 EQP_SIM 자동투입 파렛트가 122/126/130에서 CV_DATA로 정확 미러, 129/131/132 신선 갱신, 에러 0.
  - 주의: CvThread V1.1 시나리오 맵은 M256+(PLC-1)*32 블록/R트래킹/D480 방향워드 사용 — MirrorWcsTracks(D영역)는 실제 미사용.
  - EQP_SIM.ini: [WCS_MIRROR] CV2~CV10 추가, [FEED] CV11/13/15=1 (물리 투입 EQP_SIM 복귀).
- **M2 완료 (SC/RTV 브리지 스레드)**:
  1. WCS_TASK_CV에 `VehThread.cs` 신규(SC용/RTV용 2인스턴스, SYS_MAIN에서 기동) — 검증된 FenetProtocol
     재사용(무수정), observables.tsv(bin 복사)로 Vehicle 메모리 주소 해석.
  2. DB 계약: SC_DATA/RTV_DATA의 기존 Vehicle 컬럼 사용 — 지시=PALLET_ID_OD/FROM·TO_0n_OD(**2자리**)/
     TRANSFER_REQUEST_OD='Y'/OD_RQ_YN='Y' → 브리지가 PLC 기록 후 'N' 소비. 관측=SUBSYSTEM_STATUS_RD,
     LOCATION/PALLET_ON_VEHICLE/LOAD·UNLOAD_COMPLETE_RD 등 + 파생(SENSOR_FK_RD/ITN_LUGG_FK1/POS_H_RD).
     LOAD/UNLOAD_COMPLETE는 브리지가 자동 Ack. 완료=UNLOAD_COMPLETE 관측 시 SC_DATA COMPLETE_RD='1'.
  3. **치명 결함 수정**: ConveyorSim MirrorWcsTracks(D 트랙테이블) 전면 비활성 — CvThread(V1.1)가 읽지 않는
     미러인데 D워드 10~328 기록이 Vehicle 관측영역(예: SC1 SUBSYSTEM_STATUS=D0x100=256=트랙125블록+6)과
     충돌해 크레인 상태를 0으로 덮고 있었음.
  4. 실증: SC1 셀→포트4 반송(지시 소비→ST 1→2→적재 관측→하역→COMPLETE_RD=1→CV미러 104=JOB) ✓,
     RTV 포트3→포트23 반송(적재→하역→CV12 자율이동 23→24→출고대 자동배출) ✓ — 전 물리 경로 실월드 완주.
- **M3 구현 완료·검증 진행 중 (2026-07-21 00:30)**:
  - 구현: 메인 루프 분기(m_bScAutoComplete=0 → 실경로: FeedInGate/AutoRun*/RvSeq/StepCranes/OutStn·물리재현
    전부 auto 분기로 격리), DriveSC 확장(AutoRunSC 게이트 이식 + Vehicle 좌표 인코딩 VehPortLoc/VehCellLoc +
    UpdateScData가 PALLET_ID/FROM/TO/TRANSFER_REQUEST_OD 기록), DriveRGV 확장(게이트 이식+IDLE 게이트+라인 선점,
    UpdateRtvData Vehicle 필드), CompleteRGVReal(RTV COMPLETE_RD 소비→20), ProcessOutStnReal(반출=RTV 지시 1건+완료 대기),
    RtvIdle/RtvCompleteFor/RtvResetComplete 헬퍼. RTV_DATA에 COMPLETE_RD 컬럼 신설. ENV_IOSCH.INI SC_AUTO_COMPLETE=0.
    VehThread: RTV도 COMPLETE_RD 기록, SC 게이트 필드(ONLINE/ACTIVE/ERR_CODE) 유지 기록.
  - **실증 성공**: 반자동 입고 9013(122→901 셀 01-005-01) 실경로 풀사이클 99→11→31→35→20→21→29 완주 —
    투입(EQP_SIM AutoFeed)→CV 기록/스탬프→월드 이송→RTV Vehicle 반송→크레인 Vehicle 저장, IO_TASK는 지시/상태만.
  - **미해결 결함 (디버깅 중단 지점)**:
    1. 출고 9014(901→124)가 25→19 조기 완료: DriveCV의 출고 지시를 CvThread가 도착 예약 트래킹(R274)으로
       PLC에 기록 → 그 리드백(cv_data 124 lugg=9014, sensor=0)을 CompleteCV가 도착으로 오인.
       **수정안: CompleteCV 출고 판정에 SENSOR0_DATA_RD='1'(실화물 감지) 추가 필요.**
    2. 9014의 크레인 지시에 대한 VEH_SC "반송지시 기록" 로그 부재(9013 SC 지시 로그도 부재) — VehThread
       ConsumeCommands 소비/LogDb 침묵 의심. 그런데 21→25(OD 'N')·25 완료(COMPLETE_RD)는 성립 — **직전 작업(9013)의
       COMPLETE_RD='1' 잔존을 소비했을 가능성**(CompleteSC 소비 후 리셋 없음 — UpdateScData 지시 시 리셋만으론
       DriveSC→RunSC→CompleteSC가 같은 폴링 창에서 잔존값을 볼 수 있음). **수정안: CompleteSC 소비 직후 SC_DATA
       COMPLETE_RD='0' 리셋 추가 + RunSC에 TRANSFER_REQUEST_OD='N'(브리지 소비 확인) 조건 추가.**
    3. 월드 P21에 9013 파렛트 잔존(원인 미확정 — AutoFeed 재투입 파렛트에 R256 트래킹 잔존 스탬프 의심).
       배출/정리 후 재현 관찰 필요.
  - 검증 절차(재개용): 클린 기동 순서 = EQP_SIM(상태파일 비움) → WCS_TASK_CV → IO_TASK, CV/SC/RTV_DATA 리셋 쿼리는
    본 섹션 위 M1/M2 참조. 테스트: 반자동 입고 1건 → 출고 1건 → 로직1/2/3.
  - Vehicle 좌표: 포트=(00,00,트랙-100 2자리), 셀=(통로 01홀수뱅크/02짝수뱅크, Bay 2자리, Level 2자리) — OD 컬럼 2자리 규격.
- **M3 완료 + 결함 3건 중 2건 수정·재실증 (2026-07-21 00:42)**:
  1. CompleteCV 완료 판정 재작성 — 입고=소스 이탈(빈 트랙, 코드 분기), 출고=**실도착 관측(SENSOR=1+작업번호 일치,
     m_setOutArrived) 후 배출(빈 트랙) 확인** — CvThread 도착 예약 트래킹 조기완료 해소.
  2. CompleteSC 소비 직후 SC_DATA COMPLETE_RD='0' 리셋 + RunSC/RunRGV에 TRANSFER_REQUEST_OD='N'(브리지 실소비) 조건.
  3. 재실증: 입고 9015(46초)·출고 9016(도착 관측→배출 확인, 25→15→19 실물리 34초) 완주, 라인 잔존물 0.
- **M4 부분 성공 + 미해결 1건 (2026-07-21 00:52 중단 지점)**:
  - 로직2(입고대 130)·로직3(입고대 122): 신구조 풀사이클 **각 6회 완주** — EQP_SIM 물리 + 3스레드 브리지 + 순수 스케줄러로 지속 가동 실증.
  - **로직1(입고대 126, C/V#13) 정체**: JOB 2081이 상태 10(CV 대기)에서 멈춤. 진단 스냅샷:
    cv_data 126=빈, **125=SEN1/LUGG2075(이전 사이클 작업번호의 파렛트 잔존!)**, RTV IDLE.
    → 126 공급 파렛트가 픽업트랙(125) 점유로 못 들어오고 DriveCV 발행 대기.
    125의 2075 파렛트: 첫 실험의 "P21:9013 잔존"과 동일 패턴 — **RTV 픽업 후에도 트래킹/파렛트가 남거나
    재투입 파렛트가 직전 작업번호로 재스탬프되는 결함**(C/V#11·13 공통, 2-트랙 스테이션). 유력 후보:
    CvThread(CvTrackingWrite)가 기록한 R 트래킹이 파렛트 픽업 후 EQP_SIM 밖 요인으로 재기록되거나,
    OD 기록 지시(cv_data lugg_no_od/tracking_write_yn)가 잔존해 CvChg_OD_RQ_YN 이 재기록 → 새 파렛트 재스탬프.
    **다음 세션 확인 순서**: ① 정체 재현 시 cv_data(1025/1026)의 lugg_no_od/tracking_write_yn/od_rq_yn 잔존 여부,
    ② EQP_SIM 로그에서 "JOB 부여됨" 라인의 시각/트랙, ③ CvThread CvChg_OD_RQ_YN 의 소비(플래그 'N' 복귀) 로직,
    ④ RTV TakePallet 시점의 R 트래킹 클리어와 CvThread 리드백 순서(레이스).
  - 현재 잔여 작업: 2081(10)/2098(21)/2099(31) — 로직 정지 상태로 보존. 정리 시 상태 '9' + 클린 기동 절차 참조.

## SC1 특례 상호배타 — 출고 우선 / 입고 RTV 선점 시 출고 보류 (2026-07-20)
- 요구: ① SC1 출고 진행 중이면 SC1행 입고는 RTV 로딩 위치(픽업트랙, 상태 30)까지만 진행하고 반송 보류 —
  SC1 출고 먼저 완료. ② SC1행 입고가 이미 RTV에 지시(31/35)됐으면 SC1 출고 지시(20→21) 보류.
  (902~905행 입고는 무관. 배경: SC1은 입고 드롭과 출고 반출이 라인 트랙을 공유)
- 구현(cThread_SCH.cs, 백업 .bak_sc1):
  - `HasActiveSc1Outbound()` — JOB_TYP IN ('2','12') AND START_POS='901' 미종결 존재 여부.
  - `HasSc1InboundOnRtv()` — JOB_TYP IN ('1','11') AND DEST_POS='901' AND 상태 31/35 존재 여부.
  - 게이트 A: AutoRunRGV 30→31 지시부 — `destPos=='901' && HasActiveSc1Outbound()` 시 보류.
  - 게이트 B: AutoRunSC 20→21 지시부 — `jobTyp=='2' && startPos=='901' && HasSc1InboundOnRtv()` 시 보류.
  - 교착 없음: A는 입고를 30에 묶지만 출고는 진행(→19 후 해제), B는 출고를 20에 묶지만 입고 RGV는 진행(→20 후 해제).
- 실측 3시나리오:
  ① 9007(출고)+9008(입고 901행) 동시 생성 → 9008이 30에서 대기, 9007 완료(19) 후 31→35→29 ✓
  ② 9009(입고) 31 도달 후 9010(출고) 생성 → 9010이 20에서 대기, 9009 RGV 완료(20 진입) 후 지시→19 ✓
  ③ 9011(SC1 출고)+9012(902행 입고) → 9012는 보류 없이 31/35 진입(특례 미적용 확인), 둘 다 완료 ✓

## 잔여 (사용자 결정 필요)
- WCS 모드 CV12/13 배치(124=출고/126=입고)는 구 ECS 물리 구성(CV12=입고)과 반대 — §11.2 사용자 요청(클라이언트
  라벨 기준) 이력. 구 ECS와 물리 통일 여부는 사용자 결정 필요.
  (구 ECS 근거: ECSDeviceManager.IsInputConveyor {..12..}, Conveyor.OnWaitOut CONVEYOR:13 skip,
   ECSDispatcher CONVEYOR:13 최종완료 하드코딩, TB_ITINERARY I002/O017~O032)

## 테스트 잔여물 정리 요령
- JOB_MST 미완료(status 10/11/15/20/21/25/30/31/35, del_yn<>'Y') → '9'
- CV_DATA sensor0/lugg/od/tracking_write_yn 리셋, EQP_SIM eqp_state.txt 비움 + plc_memory.bin 삭제

## 2026-07-21 (2) — 물리 이관 M4: 결함 총정리와 로직1/2/3 각 10회 연속 완주

### 모든 미스터리의 진범: ENV_IOSCH.INI `SC_AUTO_COMPLETE = 1` (구 재현 경로)
클린 재현 중 IO_TASK 가 구(DB 재현) 경로로 돌고 있었음이 확인됐다. 이 하나가 이전의 세 미스터리를 전부 설명한다.
- 픽업트랙 파렛트 잔존(125 의 2100, 131 의 2144): CV 까지는 실물 이동, SC/RTV 는 DB 로만 가짜 완주 → 세계 파렛트 영구 잔존.
- EQP_SIM 로그 침묵: 차량이 실제로 안 움직여 세계가 조용했던 것(로깅 결함 아님 — Log 는 파일/이벤트 격리로 보강함).
- 129 데이터 잔존: 물리 배출 없이 DB 로만 출고 완료.
→ `SC_AUTO_COMPLETE = 0` 복구. 또한 bin 의 HOST_SIM.ini 가 7/11 구본(로직1 IN=124/OUT=126, WCS 배치와 반대)이어서 wcs_chain 변형(126/124)으로 교체.

### 실경로 결함 수정(순차 발견·수정, 각 수정 후 클린 재기동 검증)
1. **CompleteCV 입고 판정**: "소스 빈 트랙" → **픽업트랙 실도착 관측**(SENSOR=1+작업번호 일치).
   자동 재투입이 이탈 직후 같은 틱에 일어나 빈 순간이 관측 불가능했음 (로직1/2 S15 영구 정체 원인).
2. **겸용 C/V#11 예약 트래킹 하이재킹 차단(2중)**:
   - EQP_SIM: 빈 입고 포트에 선기록된 트래킹(출고 도착 예약값)을 기억(staleFeedTracking)하고 그 값은 신규 파렛트에 배정 금지.
   - IO_TASK DriveCV: 출고 CV 지시는 목적지 배출트랙+하역트랙이 빈 상태에서만 발행(구 ECS "입고대 점유 시 C/V#11 출고 배차 금지"와 동일 취지).
3. **설비 지시 직렬화(리드백 지연 창 레이스 차단)**: 설비 리드백 ~1초 지연 동안 게이트가 통과되어 연속 지시가
   LUGG_OD 를 덮어써 앞 작업의 완료 귀속이 유실됐음(2165 S35 고착). DB 작업상태 기반 배타(지연 없음)로 차단:
   - DriveRGV: JOB_STATUS 31/35 존재 시 신규 지시 금지 + 1폴링 1건(break) + COMPLETE_RD='1' 미소비 시 보류.
   - DriveSC: 같은 크레인 21/25 존재 시 금지 + COMPLETE_RD 보류. DriveCV: 같은 트랙 11/15 존재 시 금지.
4. **출고대 반출 대기열 등록 누락(실경로 최대 결함)**: m_lstOutPend 는 구경로 ProcessCvMove 만 채웠음 —
   실경로에선 RTV 반출 지시가 영영 나가지 않았고, 겸용 122 는 예약+입고 파렛트 조합을 CompleteCV 가
   실도착으로 오인해 **가짜 완료**(실물은 라인에 잔존 → 후속 SC 지시까지 연쇄 정체).
   → DriveCV 출고 발행 시 대기열 등록 + CompleteCV 출고 도착 관측은 반출 시퀀스 완료 후에만 인정(OutSeqPending 게이트)
   + ProcessOutStnReal Stage0 에 홀수 트랙 실도착 대기 게이트.
5. **반출 픽업 위치 교정**: 등록 Odd 가 짝수 하역트랙(RgvOutDropTrack)이라 세계가 홀수로 옮긴 뒤 RTV 가
   빈 트랙에서 영구 재시도(2232, From P10). → `RgvPickupTrack(RgvOutDropTrack(startPos))` (짝수-1 홀수).
6. **CvThread 자가 치유**: 변경감지 캐시(CvDic)와 DB 가 어긋나면 영구 불일치 → 30폴링(~1분)마다 캐시 무효화(전 컬럼 재기록).
7. **PLC 물리 타이밍 정밀화(사용자 지시)**: EQP_SIM 입고 적재→**2초 뒤 재하감지 ON**(IN_SENSOR_DELAY_MS, SyncMemory 게이트 포함),
   출고 배출→**2초 뒤 트래킹 삭제**(OUT_TRACK_CLEAR_MS 1000→2000).

### 검증
클린 기동 후 14분 연속 가동: **로직1/2/3 각 10회 완주, 무정체**(3로직 동시, 입출고 교대). VehThread 지시/완료 관측,
세계 물리 이동, JOB 상태머신, job_mst_his 이관 모두 일치. WCS_LOG_PGR 의 VEH_SC/VEH_RTV 로그 정상 기록(LOG_SEQ 무해).

### 역할 경계(사용자 질문 정리)
- 물리(지게차/PLC): 입고대 적재, 재하감지 2초 지연, 출고대 배출, 배출 2초 후 데이터 삭제 — **전부 EQP_SIM**.
- 스케줄링(IO_TASK): ProcessOutPend 는 물리 재현이 아니라 "RTV 1대 반출 순서 결정(FIFO)" — 지시 1건 발행+완료 대기만 하므로 실경로 방향성에 부합.

## 2026-07-21 (3) — 13시간 장기 가동에서 드러난 초저빈도 레이스 2건 봉합

13시간 가동(로직2/3 각 527회, 로직1 50회) 중 정체 2건의 원인과 수정:
1. **하이재킹 잔여 창(21:29, 527사이클 1회)**: DriveCV 출고 발행 게이트가 DB 센서 리드백 지연(~2초) 동안
   겸용 122 를 빈 것으로 오판 → 예약 트래킹 기록 → 감지 대기 중이던 무지시 파렛트가 오배정.
   → EQP_SIM: **재하감지 ON 순간 이미 존재하던 트래킹은 예약값으로 간주(배정 금지 값에 병합)**.
   정상 스탬프는 감지 ON 관측 후에만 기록되므로 발행 게이트와 상보적으로 창이 닫힘.
2. **지시 갱신 도중 소비(08:43, 로직1 50사이클 1회)**: VehThread 가 직전 스트로브 잔존 상태에서 FROM/TO 를
   덮어써 설비가 갱신 도중 값을 소비(DB 기록 To=23 vs 설비 수신 P2) → 존재하지 않는 트랙 102 에 하역·유실.
   → VehThread ConsumeCommands: **TRANSFER_REQUEST 비트가 0(설비가 직전 스트로브 해제)일 때만 새 지시 기록**.
검증: 완전 초기화 후 14분 — 로직1/2/3 각 10회 완주, 무정체, 잔존물 0건(점유 트랙=진행 중+입고대 대기뿐).

## 2026-07-23 — OP_Mode 실측 매칭 + 교착 방지 쿼리 게이트 3건
- **OP_Mode(슬라이드 18/19, mBase+8) ↔ CV_DATA.AUTO_MODE_RD 실측 매칭**: CvThread 는 원래 이 비트를 AUTO_MODE_RD 로
  파싱하고 있었음 — 빠져 있던 것은 EQP_SIM 이 OPERATION_MODE 관측(예: CV13=0x288=648=mBase+8)을 켜지 않던 것.
  ConveyorSim.Reset 에서 자동=ON 어서트. 검증: AUTO_MODE_RD 전체 '0' 시드 후 재기동 → 실트랙 30/30 이 관측으로 '1' 복원
  (101/102 는 존재하지 않는 C/V#1 자리행이라 제외). DriveCV 의 AUTO_MODE_RD='1' 게이트가 실측 기반이 됨.
- **122 입고 발행 게이트(DriveCV SQL)**: 122 로 오는 출고 반출이 RTV 에 지시된 상태(RTV_DATA.JOB_TYP_OD='2'+LUGG_OD 매칭,
  해당 출고 DEST=122·상태 15)면 122 입고 발행(트래킹 기록) 보류 — 겸용 입출고대 교착 방지.
- **SC1/C\V#2 교착 방지(쿼리 조건)**: DriveSC — SC1 출고 지시는 C\V#2(103/104)에 입고 작업 화물이 없고 RTV 에 901행
  입고 지시(JOB_TYP_OD='1')가 없을 때만. DriveRGV — 901행 입고 RTV 지시는 SC1 출고 수령(21/25)이 없고 C\V#2 에 출고
  작업 화물이 없을 때만. (기존 SC1 특례 코드 게이트와 상보)
- 검증: 클린 8분 가동 — 로직1 6회, 로직2·3 각 5회 완주(his 34건), 게이트 과차단·SQL 오류 없음.

## 2026-07-24 (2) - Client CString 멀티스레드 어설션(nRefs>0) 수정
- 증상: MANUAL/모니터링 중 mfc140ud.dll atlsimpstr.h:68 ASSERT(nRefs>0) 모달로 진행 불가.
- 원인: 클라이언트의 설비 데이터 CString을 **수집 스레드가 쓰고 UI 스레드가 읽는 경합**. 특히
  ① 트랙 컨트롤 CDciTrackCtrl::m_strExtraText(J/T/P 표시 텍스트)를 TrackInfo::ApplyTrackTextMode(수집)가
     쓰고 Draw(UI 페인트)가 읽음 — 부하 시 고빈도. ② SC/RTV 대화상자 1.5초 실시간 타이머가
     InvalidateScData/RtvData에서 공유 CString을 읽음 vs CScPair/CRtv::AutoRunProc 쓰기.
- 수정: (a) DciTrackCtrl에 전역 락 g_csExtraText + SetExtraTextSafe/GetExtraTextSafe(깊은 복사) — 쓰기/페인트 직렬화.
  (b) CEcsDoc::m_csEqpData 크리티컬 섹션으로 CScPair/CRtv::AutoRunProc 쓰기 ↔ 대화상자 Invalidate 읽기 직렬화.
- 검증: 체인 가동(4분 10사이클) 부하 중 J/T/P 25라운드 어설션 0회, 클라이언트 생존.
- 주의(중요): **CP949(euc-kr) 소스는 Edit 도구로 편집 금지** — UTF-8 재기록으로 한글 전멸. 반드시 PowerShell
  GetEncoding(949) 스크립트로 편집. 사고 시 .presrc_bak 복원 후 재적용(ScSkinDlg/RtvSkinDlg 복구 이력).
