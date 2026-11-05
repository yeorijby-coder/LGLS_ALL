# -*- coding: utf-8 -*-
"""01 ECS 운영자 매뉴얼 - 2026-11-10 종료 산출물 (신규).
   서버·TASK·DB 를 관리하는 사람을 위한 안내. 2026-09-27 판의 01 설치 안내서 · 02 DB 구축 정보 · 12 INI 정의서(요약) 와
   10월~11월 적용 안내서(런처/Upload, 폰트 잠금, 이력 자동 삭제, 사후 확인 SQL)를 한 권으로 묶었다.
   실행 : python gen_opmanual_docx.py"""
import os
from common1110 import *

d = new_doc('ECS 운영자 매뉴얼', '설치 · 기동/정지 · 서버 프로그램 · 설정 · DB 운영 · 배포 · 장애 대응 - ' + SITE,
            history=[['1.0', '2026-09-21', AUTHOR, '01 설치 및 구동 안내서 · 02 DB 구축 정보 · 12 INI 설정 정의서 (분권)'],
                     ['2.0', '2026-10-15', AUTHOR, '적용 안내서 09-29 ~ 10-15 : 런처/Upload 배포, 폰트 잠금, 4K 배율, 작업정보 판넬']])

# ─────────────────────────────────────────────────────────────
d.add_heading('0. 이 문서는', 1)
para(d, '자동창고 WCS(ECS)의 서버 프로그램과 DB 를 설치·기동·설정·관리하는 사람을 위한 안내서다. 운전 화면을 쓰는 법은 02 사용자 매뉴얼에, '
        '표·열의 뜻은 03 테이블 정의서에, 상위·설비와 주고받는 전문·신호는 04 인터페이스 정의서에 있다.')
doc_set_table(d, 1)
d.add_page_break()

# ─────────────────────────────────────────────────────────────
d.add_heading('1. 시스템 구성', 1)
d.add_heading('1.1 구성도', 2)
code(d, [
    '  WMS / IMS (192.100.1.191)                      마스터 PLC XGT (192.100.1.110 : 2004)',
    '      │ 8001 명령 수신 / 8002 보고 송신               │ 1 소켓 (C/V 15 · S/C 5 · RGV 1)',
    '      ▼                                              ▼',
    '  상위 통신 WCS_TASK_HOST  ◀──▶  SQL Server LGLS_MCS_IO  ◀──▶  설비 통신 WCS_TASK_CV',
    '                                     ▲        ▲',
    '                                     │        └──  스케줄러 IO_TASK (작업 상태 기계 · 이력 삭제)',
    '                                     └──  운전 화면 Ecs (서버 또는 키오스크 PC, 여러 대 가능)',
])
table(d, ['프로그램', '실행 파일', '위치', '역할'], [
    ['설비 통신 (EQP)', 'WCS_TASK_CV.exe', '서버', '마스터 PLC(XGT)와 1 소켓으로 통신. CV_DATA / SC_DATA_LGLS / RTV_DATA_LGLS 갱신, 지시를 PLC 에 쓴다. 한 바퀴 약 1초'],
    ['스케줄러 (SCH)', 'IO_TASK_SEMI_FINISH.exe', '서버', 'JOB_MST 를 읽어 CV/RGV/SC 지시를 내리고 완료를 판정한다(200ms 루프). 2026-10-29 부터 보존기간 지난 이력 삭제'],
    ['상위 통신 (HOST)', 'TASK_LFC10_G1_ECSCOM.exe', '서버', 'WMS/IMS 와 전문(O/R/M/D ↔ S/E/F) 교환, 스테이션 코드 변환'],
    ['통합 실행본', 'ALL_TASK.exe', '서버', '위 셋을 한 프로세스의 탭으로. [제어판]에서 태스크별 기동·정지. 둘 중 한 방식만 쓴다'],
    ['운전 화면', 'Ecs.exe(런처) → EcsMain.exe', '서버 · 키오스크 PC', 'Ecs.exe 가 DB 의 새 파일을 받아 EcsMain.exe 를 띄운다'],
    ['배포 도구', 'WmsUp.exe (Upload)', '관리 PC', 'Client 파일을 DB(UP_DOWN/DN_MST)에 올린다 (7장)'],
    ['DB', 'SQL Server (Express 가능)', '서버', 'LGLS_MCS_IO. 모든 프로그램이 이 DB 로만 대화한다'],
], widths=[3.0, 4.2, 2.3, 7.5], font=8.5)

d.add_heading('1.2 네트워크 · 포트', 2)
table(d, ['구간', '방향', '주소 · 포트', '설정 위치'], [
    ['설비 통신 ↔ 마스터 PLC', 'WCS 가 접속', '192.100.1.110 : 2004 (XGT 전용 TCP)', 'WCS_DB.INI [COMM0] IP / CUR_PORT'],
    ['WMS → 상위 통신 (명령)', 'WMS 가 접속', '서버 : 8001 (LocalPort)', 'EcsComA.ini [Network] LocalPort'],
    ['상위 통신 → WMS (보고)', 'WCS 가 접속', '192.100.1.191 : 8002 (RemoteIP / RemotePort)', 'EcsComA.ini [Network]'],
    ['모든 프로그램 ↔ DB', 'TCP', '서버IP : 1433 (고정)', '각 INI 의 DB 섹션, Ecs.ini [DB_2]'],
], widths=[4.5, 2.5, 5.5, 4.5], font=8.5)
note(d, '개발 PC 시험값(127.0.0.1 / 9911 / 9910 / localhost\\SQLEXPRESS,1435)이 현장 INI 에 남아 있으면 안 된다. 5장 점검표.')

d.add_heading('1.3 작업대 · 설비 배치', 2)
table(d, ['트랙', '설비', '역할', 'IMS 스테이션', '비고'], [
    ['122', 'C/V#11', '외부 입고 전용 입출고대(방향전환)', '101', '입출고 겸용. RGV 픽업·드롭 모두 121'],
    ['124', 'C/V#12', '제품 입고대', '103', 'RGV 픽업 123'],
    ['126', 'C/V#13', '원부자재 불출대', '104', 'RGV 드롭 125'],
    ['129', 'C/V#14', 'Picking 작업대(출고)', '102', 'RGV 드롭 127'],
    ['130', 'C/V#15', 'Picking 작업대(입고)', '102', '슬롯 비선형 trackOrder 131,132,130. RGV 픽업 131'],
    ['103 · 104', 'C/V#2', 'S/C#1 통로(방향전환, 입출고 겸용)', '105/110', '통로 한 라인을 입출고가 함께 쓴다 → 통로 사용권(CV2_DIR_MODE=2)'],
    ['105~120', 'C/V#3~#10', 'S/C#2~#5 통로', '-', '홀수 = RGV 측, 짝수 = S/C 측. 입고 C/V 짝수호기(4·6·8·10), 출고 홀수호기(3·5·7·9)'],
    ['901~905', 'S/C#1~#5', '스태커 크레인', '201~205(ECS) / 1~5(WMS)', ''],
    ['801', 'RGV', '무인 대차', '-', ''],
], widths=[2.0, 2.2, 5.0, 2.8, 5.0], font=8.5)
note(d, '작업대 이름표는 Client 폴더의 EcsDefine.xml, 트랙↔C/V 배치는 7_DeviceMap\\PlcAddressMap.xml <Equipments>/<CraneMap> 이 단일 기준이다.')
d.add_page_break()

# ─────────────────────────────────────────────────────────────
d.add_heading('2. 설치', 1)
d.add_heading('2.1 실행본 구성', 2)
para(d, '각 폴더를 통째로 복사하면 그 자리에서 실행된다. 실행에 필요한 파일과 설정(INI/XML/리소스)만 담았다.')
table(d, ['폴더', '프로그램', '실행 파일', '비고'], [
    ['EXE\\TASK\\WCS_TASK_CV', '설비 통신', 'WCS_TASK_CV.exe', 'WCS_DB.INI, 7_DeviceMap\\PlcAddressMap.xml 폴더째'],
    ['EXE\\TASK\\IO_TASK', '스케줄러', 'IO_TASK_SEMI_FINISH.exe', 'ENV_IOSCH.INI'],
    ['EXE\\TASK\\WCS_TASK_HOST', '상위 통신', 'TASK_LFC10_G1_ECSCOM.exe', 'EcsComA.ini'],
    ['TASK\\ALL_TASK\\bin\\Debug', '통합 실행본(선택)', 'ALL_TASK.exe', '각 TASK 의 INI 를 그대로 읽는다. ALL_TASK.INI 는 로그 폴더·보관일·자동기동만'],
    ['Client_설치\\Client', '운전 화면', 'Ecs.exe + EcsMain.exe + DciLib/EcsLib/XmlLib.dll + rc_resource + 폰트', 'Release 빌드. Setup.bat 로 설치(2.3)'],
    ['Client_설치\\Prerequisites', '사전 설치', 'vc_redist.x86.exe / ndp48-web.exe', 'VC++ 2015-2022 x86, .NET Framework 4.8'],
    ['TASK\\Upload', '배포 도구', 'WmsUp.exe + WmsUp.ini', '관리 PC 에만'],
    ['DB_BACKUP', 'DB 백업', '*.bak, 서버_구축_절차.md', 'DB 이관용 (6장)'],
    ['SQL', '현장 DB 반영 SQL', '00_현장DB_공통코드_추가.sql, 260917_알람리스트_EQP_ECD_MST.sql, 261029_사후확인_작업번호.sql', '앞 둘은 멱등, 셋째는 읽기 전용'],
], widths=[4.2, 2.4, 5.6, 4.8], font=8)
note(d, 'TASK 3종을 따로 띄우는 방식과 ALL_TASK 한 개로 띄우는 방식 중 하나만 쓴다. 둘을 동시에 띄우면 같은 PLC 소켓과 DB 를 두 번 잡는다.')

d.add_heading('2.2 서버 요건', 2)
bullets(d, [
    'Windows Server 2019 이상(또는 Windows 10/11). .NET Framework 4.8 (TASK 3종·ALL_TASK·Upload). IO/HOST 는 4.0 대상이지만 4.8 위에서 돈다.',
    'Microsoft Visual C++ 2015-2022 Redistributable (x86) - 운전 화면(Release 빌드, mfc140u / vcruntime140 / msvcp140 / UCRT).',
    'SQL Server (Express 가능). 혼합 인증, TCP 1433 고정, 방화벽 1433 인바운드. 복원 절차는 6.1.',
    '설치 순서 : .NET 4.8 → vc_redist.x86 → (재부팅 요구 시 재부팅) → DB 복원(6.1) → 폴더 복사 → INI 수정(5장) → 현장 DB 반영 SQL(6.3) → 기동(3장).',
])

d.add_heading('2.3 운전 화면(Client) 설치 - Setup.bat 과 폰트 잠금', 2)
para(d, 'Client_설치\\Setup.bat 을 관리자 권한으로 실행하면 Client 폴더 복사 · vc_redist 설치 · 폰트 복사 · 바로가기를 한 번에 한다. '
        '키오스크 PC 에는 Client 폴더만 있으면 되고 MS-SQL / .NET / Visual Studio 는 필요 없다.')
table(d, ['증상', '원인', '처치'], [
    ['설치 중 "액세스가 거부되었습니다" (rc_resource\\font\\*.ttf)', '종전 판 Ecs 가 폰트를 공용으로 등록(AddFontResource)해 로그오프까지 잠겼다',
     'Client_설치\\Release-Fonts.ps1 (또는 폰트잠금_풀기.bat / Force-ReleaseFonts.ps1) 로 풀고 다시 설치. 2026-10-01 판부터는 폰트를 프로세스 전용(FR_PRIVATE)으로 올려 재발하지 않는다'],
    ['설치 뒤 아이콘·그림이 안 나온다', '작업 디렉터리가 Client 폴더가 아니다(재시작 관리자로 떴을 때 등)', '2026-10-15 판부터 프로그램이 스스로 실행 파일 폴더로 맞춘다. 바로가기의 "시작 위치" 도 Client 폴더로'],
    ['DciLib 심볼 오류', 'Debug 빌드 파일이 섞였다', 'Client 는 반드시 Release 빌드만 배포한다'],
], widths=[5.0, 5.0, 7.0], font=8.5)
d.add_heading('2.4 키오스크 Ecs.ini 발췌', 2)
code(d, ['[DB_2]', 'DRIVER=SQL Server          ; Windows 기본 내장 ODBC 드라이버',
         'SERVER=서버IP,1433         ; IP,포트 형식', 'USERID=LGLS_IO', 'USERPASSWORD=LGLS_IO', 'DATABASE=LGLS_MCS_IO',
         '', '[DISPLAY]', 'START_MONITOR=0            ; 0 주 모니터 / N 디스플레이 번호 / -1 마지막 위치',
         'UI_SCALE=0                 ; 0 = Windows 배율 자동 (4K@300% 도 그대로)', 'LAMP_STYLE=0               ; 신호등이 어긋나면 1'])
note(d, '접속이 안 되면 ODBC 데이터 원본 관리자(32비트)에서 "SQL Server" 드라이버로 서버 연결 테스트를 해 네트워크/인증 문제를 먼저 가린다.')
d.add_page_break()

# ─────────────────────────────────────────────────────────────
d.add_heading('3. 기동 · 정지', 1)
d.add_heading('3.1 기동 순서', 2)
para(d, 'WCS_TASK_CV (설비 통신)  →  IO_TASK (스케줄러)  →  WCS_TASK_HOST (상위 통신)  →  운전 화면', bold=True, indent=0.5)
bullets(d, [
    'ALL_TASK 는 내부에서 같은 순서(EQP → IO → HOST)로 기동한다. ALL_TASK.INI [AUTOSTART] EQP/IO/HOST=1 이면 띄우자마자 자동 기동.',
    '운전 화면은 아무 때나 띄워도 된다. 껐다 켜도 창고는 그대로 돈다.',
    '상위(WMS)는 우리 8001 에 붙어 명령을 보내고, 우리는 WMS 8002 에 붙어 보고한다. HOST 가 뜨면 상태보고(S)를 먼저 보낸다.',
])
image(d, shot('alltask_ctrl.png'), width_cm=14.0, caption='ALL_TASK [제어판] - 태스크별 기동 · 정지')

d.add_heading('3.2 정지 · 교체(재기동) 주의', 2)
warn(d, '프로그램 교체·재기동은 크레인·RGV 가 반송 중인 작업(작업상태 25 = SC 반송중, 35 = RGV 반송중)이 없을 때 한다.')
bullets(d, [
    '운전 화면 작업정보에서 25/35 작업이 없는지 확인한 뒤 종료한다. 반송 중에 끄면 설비 완료 신호를 놓쳐 작업이 멈춘 채 남을 수 있다 '
    '(완료 판정은 설비 신호로만 하며 시간 경과로 자동 완료하지 않는다). 그래도 남으면 운전 화면 [완료처리] 로 넘긴다(02 사용자 매뉴얼 5.7).',
    '종료 순서는 기동의 역순(HOST → IO → CV)을 권장한다. ALL_TASK 면 [제어판]에서 하나씩 정지하거나 창을 닫는다.',
    '실행 파일 교체 전 기존 파일을 백업(예 *.exe.bak_날짜)해 두면 즉시 되돌릴 수 있다.',
    '한 TASK 만 바꿀 때는 그 TASK 만 정지 → 덮어쓰기 → 기동. 예) IO_TASK 교체 = ALL_TASK [IO] 정지 → exe 덮어쓰기 → [IO] 기동.',
])
d.add_heading('3.3 중복 실행 검사', 2)
para(d, '각 TASK 는 같은 프로세스 이름이 이미 떠 있으면 "프로그램이 이미 종료 중 입니다" 창을 내고 스스로 끝난다. '
        '다른 프로젝트의 같은 이름 실행 파일(IO_TASK_SEMI_FINISH.exe 등)이 같은 PC 에 떠 있어도 걸리므로, 한 PC 에는 한 현장의 TASK 만 둔다.')
d.add_page_break()

# ─────────────────────────────────────────────────────────────
d.add_heading('4. 서버 프로그램 화면', 1)
d.add_heading('4.1 통합 태스크 ALL_TASK', 2)
para(d, '설비 통신·스케줄러·상위 통신 세 프로그램을 한 창에 합친 것. 탭으로 각 화면을 보고 [제어판]에서 태스크별로 기동·정지한다. '
        '로그는 스레드별 파일(ALL_TASK.INI [LOG] DIR\\<스레드>\\<스레드>_yyyyMMdd.log, KEEP_DAYS 기본 365)과 WCS_LOG_PGR 에 남는다.')
d.add_heading('4.2 설비 통신 WCS_TASK_CV', 2)
image(d, shot('cvtask_main.png'), width_cm=15.0, caption='설비 통신 화면 - 접속 상태 · 설비별 순회 · 로그')
bullets(d, [
    'C/V 15대 한 바퀴 약 1초. 운전 화면 반영 속도를 좌우한다. WCS_DB.INI [CNF] CV_BULK_R / CV_EVT_BULK / CV_HB_MS / CV_CYCLE_SLEEP_MS / CV_CYCLE_LOG_MS / UI_LOG_ASYNC (저장 후 2초 내 반영).',
    '출고대 도착(WAIT_IN) 신호는 3초 펄스라 사이클당 1회 샘플링으로 놓칠 수 있다 → OUT_SCAN_MS 간격으로 출고대만 따로 보고 CV_DATA.RET_READY_LATCH 에 래치한다(2026-09-29). 별도 스레드는 두지 않는다(소켓 1개).',
    '크레인 에러 판정 SC_ERR_CODE_BLOCK (현장 권장 3 : 보고 비트 래치 + 워드 0 해제) - 3·5호기 유령 에러 대책(5.2).',
    '2026-10-29 부터 "[] -> [0000]" 트래킹 변화 로그를 남기지 않는다(빈 값/0/0000 은 모두 "없음").',
    '기동 로그 [HMI_DEL] 에 WCS_DB.INI [HMI_DELETE] 7개 영역(SC1~SC5 · RTV1 · CV)이 풀려 보인다. "-" 는 미정의. 운전 화면 [데이터 삭제]/[CV 지우기] 처리 결과는 WCS_LOG_PGR 에 남는다(2026-11-04).',
    '화면의 라디오(R 주소 10진/16진)와 PlcAddressMap.xml 의 rAddrMode 는 함께 바뀐다. 현장 확정값 DEC / DOC. 바꾸지 않는다.',
])
d.add_heading('4.3 스케줄러 IO_TASK', 2)
image(d, shot('task_IO_TASK_SEMI_FINISH.png'), width_cm=15.0, caption='스케줄러 화면 - 사이클 로그')
bullets(d, [
    '작업 상태 기계. JOB_MST 를 읽어 CV/RGV/SC 지시(OD_RQ_YN=Y)를 내리고, 설비 신호로 완료를 판정한다. 체류는 경고만 하고 자동 완료하지 않는다.',
    'ENV_IOSCH.INI [CNF] : CV2_DIR_MODE(2 = 통로 사용권) / IN_HS_STATUS(15·16) / MANUAL_DIR_HOLD_SEC(120) / OUT_DONE_ON_ARRIVAL(1 = 출고대 실도착 즉시 완료) / HIS_DEL_*(이력 삭제, 6.4).',
    '화면 로그 구분 [HIS] 로 이력 삭제를 확인한다 : "job_mst_his : 60 DAY 지난 12,345건 삭제" / "이력 정리 끝 (n초) - 다음 검사 HH:mm". 일반 로그는 화면만, 중요/에러 로그는 파일(ECS_LOG)에도 남는다.',
    '크레인당 사이클 1건 지시(2026-09-16) · S/C#1 통로 2-deep 입고 · 겸용대 122 입고 전환 가드 등 교착 방지 규칙이 들어 있다(세부는 09-27 산출물 04 기능 흐름).',
])
image(d, shot('iotask_his_purge.png'), width_cm=15.0, caption='이력 삭제 로그 예 (2026-10-29 개발 PC) - 없는 표 건너뜀 → eqp_err_his 50건 삭제 → 정리 끝')
d.add_heading('4.4 상위 통신 WCS_TASK_HOST', 2)
image(d, shot('chk_TASK_LFC10_G1_ECSCOM.png'), width_cm=15.0, caption='상위 통신 화면 - 수신/송신 소켓 상태 · 전문 로그')
bullets(d, [
    'WMS/IMS 와 소켓(명령 수신 8001 / 보고 송신 8002)으로 전문을 교환한다. 전문 규격은 04 인터페이스 정의서 2장.',
    '스테이션 코드 변환(StationMapMode=WMS : 101=122, 102=129/130, 103=124, 104=126)을 담당한다.',
    '완료(09)가 되면 즉시 F 를 보내고, 응답이 없으면 [Host] RE_REPORT_09_SEC(기본 60초)마다 다시 보낸다.',
    '접속별 수신 작업(SRV_MULTI_CONN=1, 2026-09-30) : 상위가 겹쳐 접속해도 먼저 붙은 소켓이 끊기지 않는다(구 ECP 다중접속과 같음).',
    '반자동(작업구분 10~15 / 9000번대)은 상위에 보고하지 않는다.',
])
d.add_page_break()

# ─────────────────────────────────────────────────────────────
d.add_heading('5. 설정 파일 (INI)', 1)
para(d, '각 프로그램은 실행 파일과 같은 폴더의 INI 를 읽는다. 전 항목의 정의는 2026-09-27 산출물 12_INI_설정_정의서.docx 에 있고, 여기에는 운영 중 손댈 키만 모았다.')
table(d, ['프로그램', '설정 파일', 'DB 접속 섹션', '운전 옵션 섹션', '반영'], [
    ['WCS_TASK_CV', 'WCS_DB.INI', '[DB_2]', '[CNF] [PLC] [COMM0]', '[CNF] 는 저장 후 2초 내, [PLC]·[COMM0] 는 재기동'],
    ['IO_TASK', 'ENV_IOSCH.INI', '[P_DB]', '[CNF]', '호출마다 읽음 - 저장 즉시'],
    ['WCS_TASK_HOST', 'EcsComA.ini', '[DB]', '[Network] [Host]', '[Network] 재기동, [Host] 일부 즉시'],
    ['운전 화면', 'Ecs.ini', '[DB_2]', '[DISPLAY] [MENU] [USER] [ETC] [SC_ERR]', '대부분 핫 리로드, 색·배치 일부 재기동'],
    ['ALL_TASK', 'ALL_TASK.INI', '-', '[LOG] [AUTOSTART]', '재기동'],
], widths=[3.0, 3.0, 2.4, 4.6, 4.0], font=8.5)

d.add_heading('5.1 현장 접속값 점검표', 2)
table(d, ['파일', '키', '현장값', '개발 PC 시험값(현장에 남으면 안 됨)'], [
    ['WCS_DB.INI [COMM0]', 'IP / CUR_PORT', '192.100.1.110 / 2004', '127.0.0.1 / 2004 (EQP_SIM)'],
    ['EcsComA.ini [Network]', 'RemoteIP / LocalPort / RemotePort', '192.100.1.191 / 8001 / 8002', '127.0.0.1 / 9911 / 9910 (HOST_SIM)'],
    ['WCS_DB.INI [DB_2] · ENV_IOSCH.INI [P_DB] · EcsComA.ini [DB]', 'IP(서버) / 계정', '서버IP,1433 / LGLS_IO', 'localhost\\SQLEXPRESS 또는 127.0.0.1,1435'],
    ['Ecs.ini [DB_2]', 'SERVER / USERID / USERPASSWORD', '서버IP,1433 / LGLS_IO / (지정)', '〃'],
    ['WCS_DB.INI [PLC]', 'DIR_CODE', 'IN1 (PLC 담당 확인값 : 입고 1 / 출고 0)', 'IN0 (시뮬레이터는 IN0 만 이해)'],
    ['WCS_DB.INI', 'CNT (설비 수)', '15', '15'],
], widths=[5.0, 4.0, 4.0, 4.0], font=8.5)

d.add_heading('5.2 주요 운영 키', 2)
table(d, ['프로그램 / 섹션.키', '권장', '요점'], [
    ['설비 통신 [CNF] SC_ERR_CODE_BLOCK', '3', '크레인 에러 판정. 0 워드 상시(유령 에러 위험) / 2 보고 비트 래치 / 3 = 2 + 워드 0 해제(권장) / 4 = 2 + DOWN 에러색 / 1 전부'],
    ['설비 통신 [PLC] DIR_CODE', 'IN1', 'C/V 방향 워드 부호. 현장 PLC 확인값 IN1(입고 1/출고 0). 재기동 필요'],
    ['설비 통신 [CNF] MAIN_ALM_BITS', '1', '메인 PLC 알람 비트(M5501~ C/V 과부하, M5601~ RGV 알람) 읽기'],
    ['설비 통신 [CNF] OUT_SCAN_MS · RET_READY_LATCH', '300 · 1', '출고대 3초 펄스 보강 스캔 간격 / 래치 사용'],
    ['스케줄러 [CNF] CV2_DIR_MODE · CV2_FAIR', '2 · 0', 'S/C#1 통로 방향 : 2 = 사용권 방식(출고 우선). 1 = 교착 때만 전환'],
    ['스케줄러 [CNF] IN_HS_STATUS', '16', '입고가 통로 H/S 에 내려진 뒤 작업상태 15/16. 읽는 쪽은 둘 다 수용'],
    ['스케줄러 [CNF] MANUAL_DIR_HOLD_SEC', '120', '운전 화면 수동 방향 전환 뒤 자동 정합 억제(초)'],
    ['스케줄러 [CNF] OUT_DONE_ON_ARRIVAL', '1', '출고대에 그 작업번호 화물이 실제 도착하면 즉시 19(완료). 현장에서 설비가 3초 펄스를 못 잡던 대책(2026-09-30)'],
    ['스케줄러 [CNF] HIS_DEL_ENABLE · _INTERVAL_MIN · _BATCH · _START_SEC', '1 · 60 · 2000 · 120', '이력 자동 삭제(6.4). 끄려면 ENABLE=0, 재기동 불필요'],
    ['상위 통신 [Host] StationMapMode', 'WMS', 'IMS 스테이션 해석 기준. 바꾸지 않는다'],
    ['상위 통신 [Host] RE_REPORT_09_SEC', '60', '완료 보고 재전송 간격'],
    ['상위 통신 [Host] SRV_MULTI_CONN', '1', '접속별 수신 작업(겹친 접속 경합 방지)'],
    ['상위 통신 [Host] ScErrCodeType · ScDualCodes · ScEmptyCodes', 'SC_LGLS · 73,74 · 75', 'E 전문 ErrorKind 판정(이중입고/공출고)'],
    ['운전 화면 [MENU] VEH_CLEAR_MODE', '3', '크레인·RGV 색·번호 지우는 시점(새 트랙 기록 시)'],
    ['운전 화면 [MENU] VEH_SHOW_ON_ORDER', '1', '크레인 번호·색은 지시(25)된 뒤에만'],
    ['운전 화면 [MENU] ZOOM_BTN · FORCE_BTN', '1 · 1', '상태창 [확대] / [강제완료] 표시'],
    ['운전 화면 [MENU] HMIDEL_BTN', '1', '크레인·RGV 상태창 [데이터 삭제] 표시 (2026-11-04)'],
    ['설비 통신 [HMI_DELETE] SC1~SC5 · RTV1 · CV 의 _AREA / _ADDR', 'M / 6800 ~ 6806', 'PLC 담당자가 준 삭제 신호 M 비트 7개(2026-11-05 확정 : S/C#1~#5 = M6800~M6804, RGV = M6805, CONVEYOR = M6806). _ADDR 은 PLC 표기 그대로 적는다(6800 = 워드 680 비트 0). 운전 화면 [데이터 삭제]/[CV 지우기] 가 1 을 쓰는 자리(끄는 것은 PLC). 비우면 이력만 남기고 쓰지 않음. 기입 뒤 EQP 재기동'],
    ['운전 화면 [ETC] COLLECT_INTERVAL_MS · COLLECT_STEP_MS', '200 · 0', '설비 데이터 수집 주기 / 설비 간 대기(DB 부하 시 20~50)'],
    ['운전 화면 [DISPLAY] START_MONITOR · UI_SCALE · LAYOUT_FONT_SCALE · LAMP_STYLE', '0 · 0 · 0 · 0', '기동 모니터 / 배율(0 자동) / 그림 글자 배율 / 신호등 그리기 방식'],
    ['운전 화면 [DISPLAY] JOB_PANEL_SPLIT · _DETAIL_W · _DETAIL_H · _COLS · JOB_PANEL_REFRESH_MS', '0 · 0 · 0 · (기본) · 1000', '작업정보 판넬 배치·열·갱신 주기'],
], widths=[6.5, 2.8, 7.7], font=8)
d.add_heading('5.3 INI 를 고칠 때', 2)
bullets(d, [
    '적용 꾸러미의 "추가할_설정_*.ini.txt" 는 ★발췌★ 다. 현장 ini 를 통째로 바꾸지 말고 해당 섹션 아래에 키를 덧붙인다.',
    'INI 는 "마지막 [섹션] 머리 뒤" 의 줄을 그 섹션 키로 본다. 파일 끝에 덧붙일 때는 마지막 섹션이 무엇인지 먼저 본다(ENV_IOSCH.INI · WCS_DB.INI 모두 마지막이 [VIEW]). '
    '2026-09-17 전까지 CV2_DIR_MODE 3키가 [VIEW] 아래에 있어 적용되지 않았던 사고가 있었다.',
    'C++ 운전 화면 ini 는 CP949, TASK ini 도 CP949 다. 메모장으로 저장할 때 인코딩을 ANSI 로 둔다.',
])
d.add_page_break()

# ─────────────────────────────────────────────────────────────
d.add_heading('6. DB 운영', 1)
d.add_heading('6.1 구축 · 복원', 2)
table(d, ['항목', '값', '비고'], [
    ['DB', 'LGLS_MCS_IO (SQL Server, Express 가능)', '복구 모델 SIMPLE 권장(로그 표 트래픽)'],
    ['최신 배포용 백업', 'DB_BACKUP\\LGLS_MCS_IO_20260907_clean.bak', '시험 데이터를 비운 정리본. 백업 안 DB 이름이 LGLS_MCS_IO_DEPLOY 라 복원 시 대상 이름을 LGLS_MCS_IO 로 지정'],
    ['인증', '혼합(SQL + Windows)', '서버 속성 > 보안 변경 후 SQL 서비스 재시작'],
    ['접속 계정', 'LGLS_IO / LGLS_IO (db_owner)', 'sa 는 쓰지 않고 잠금 권장'],
    ['포트', 'TCP 1433 고정', 'SQL Server 구성 관리자 > TCP/IP > IPAll 동적 포트 비움. 방화벽 인바운드 허용'],
], widths=[3.5, 6.0, 7.5], font=8.5)
numbered(d, ['백업 파일을 서버 로컬 경로로 복사(SQL 서비스 계정이 읽을 수 있는 곳).',
             'SSMS > 데이터베이스 > 복원 : 대상 이름 LGLS_MCS_IO, 파일 경로를 서버 DATA 폴더로 재배치.',
             '혼합 인증 전환 → 서비스 재시작 → 로그인 LGLS_IO 생성(db_owner).',
             'TCP 1433 고정 · 방화벽 → 다른 PC 에서 sqlcmd -S 서버IP,1433 -U LGLS_IO 로 접속 확인.',
             '현장 DB 반영 SQL(6.3) 실행 → 각 INI DB 섹션 변경(5.1).'])
d.add_heading('6.2 정기 백업', 2)
bullets(d, [
    'Express 는 Agent 가 없다. Windows 작업 스케줄러에 아래 한 줄을 매일 등록한다(보관 폴더와 기간은 현장 정책대로).',
])
code(d, ['sqlcmd -S 서버IP,1433 -U LGLS_IO -P (암호) -Q "BACKUP DATABASE LGLS_MCS_IO TO DISK=N\'D:\\DB_BACKUP\\LGLS_MCS_IO_$(date).bak\' WITH INIT, COMPRESSION"'])
note(d, '2026-10-29 부터 이력 표를 IO_TASK 가 지우므로 DB 가 무한히 커지지 않는다. 지운 뒤 파일 크기는 줄지 않으니 디스크가 급하면 DBCC SHRINKDATABASE.')
d.add_heading('6.3 현장 DB 반영 SQL', 2)
table(d, ['파일 (SQL 폴더)', '대상', '내용', '성격'], [
    ['00_현장DB_공통코드_추가.sql', 'COMMON_CODE', 'JOB_STATUS 16 (통로CV 구동중), SC_ERR_STA_FK 3 (사용정지 DOWN)', '멱등(있으면 건너뜀)'],
    ['260917_알람리스트_EQP_ECD_MST.sql', 'EQP_ECD_MST', 'PLC 알람 리스트 SC_LGLS(크레인 지상반)·CV·RTV 코드', '멱등'],
    ['261029_사후확인_작업번호.sql', '(조회만)', '끝나서 JOB_MST 에서 지워진 작업을 이력·로그로 추적(6.5)', '읽기 전용'],
], widths=[5.0, 2.5, 7.0, 2.5], font=8.5)
code(d, ['sqlcmd -S 서버IP,1433 -d LGLS_MCS_IO -U LGLS_IO -P (암호) -f 65001 -i 파일명      ; -f 65001 = UTF-8 BOM 파일'])
para(d, '운전 화면은 에러코드 마스터를 기동할 때 읽으므로 SQL 실행 후 운전 화면을 다시 띄운다. TASK 는 재기동 불필요.', bold=True)

d.add_heading('6.4 이력 자동 삭제 (DEL_HIS_SETTING → IO_TASK PurgeHistory)', 2)
para(d, '운전 화면 ECS ▷ 환경설정 ▷ 로그 삭제 설정 의 보존기간(JOB_MST_HIS · WCS_LOG_PGR · WCS_CLIENT_LOG · HOST_IF_LOG 60일, EQP_ERR_HIS 30일)은 '
        '2026-10-29 전까지 어느 프로그램도 읽어 지우지 않았다(구 ECS/ECP 에도 없고, Express 라 Agent 잡도 없었다). 지금은 스케줄러가 지운다.')
table(d, ['항목', '내용'], [
    ['언제', '기동 HIS_DEL_START_SEC(120초) 뒤 첫 검사, 그 뒤 HIS_DEL_INTERVAL_MIN(60분)마다 설정표를 다시 읽는다'],
    ['무엇을', 'unit 이 DAY/HOUR/MONTH 이고 cycle ≥ 1 인 행. key_column(INS_DT / ERROR_DT) < 지금 - cycle. WH_TYP 열이 있으면 창고구분으로 좁힌다. 이 DB 에 없는 표는 건너뛰고 1회 알림'],
    ['어떻게', '사이클(200ms)마다 표 하나에서 HIS_DEL_BATCH(2,000)행씩 DELETE TOP. 긴 DELETE 하나로 스케줄링을 세우지 않는다. 개발 PC 17만 건 25초'],
    ['확인', 'IO_TASK 화면 로그 [HIS] 줄. 지운 게 없는 표는 줄을 내지 않는다'],
    ['끄기', 'ENV_IOSCH.INI [CNF] HIS_DEL_ENABLE=0. 재기동 불필요'],
], widths=[2.5, 14.5], font=8.5)
note(d, '처음 적용하는 현장은 쌓인 양이 많을 수 있다(분당 약 60만 행 처리). 보존기간을 늘리려면 Client 설정 화면에서 cycle 을 바꾼다(예 작업 이력 365). 지운 행은 되돌릴 수 없다.')

d.add_heading('6.5 끝난 작업 사후 확인', 2)
para(d, '완료(09)된 작업은 JOB_MST 에서 지워지지만 보존기간 안에는 아래 자료가 남아 "RGV·크레인을 IO_TASK 가 지시했나, 사람이 움직였나" 를 가려낼 수 있다. '
        'SQL\\261029_사후확인_작업번호.sql 의 맨 위 INSERT 한 줄에 작업번호·날짜를 넣고 ★전체★ 실행한다(DBeaver 는 Alt+X). 상세는 적용_20261110\\문서\\2026-10-29_끝난작업_사후확인방법.docx.')
table(d, ['자료', '무엇을 알 수 있나', '보존'], [
    ['JOB_MST_HIS (트리거 trg_JOB_MST_StatusHis)', '상태가 바뀐 순서·시각, 그때의 HS_TRACK_NO. 35 가 있으면 RGV 를, 25 가 있으면 크레인을 IO_TASK 가 지시한 것', '60일'],
    ['WCS_LOG_PGR', '트랙별 작업번호 이동, 설비가 받은 반송 지시, IO_TASK 보류 사유', '60일'],
    ['WCS_CLIENT_LOG', '운전자가 Client 에서 한 조작(완료처리·쓰기 등)과 PC IP', '60일'],
    ['HOST_IF_LOG / EQP_ERR_HIS', '상위 전문 / 그 시간대 설비 에러', '60일 / 30일'],
], widths=[5.0, 9.5, 2.5], font=8.5)
d.add_page_break()

# ─────────────────────────────────────────────────────────────
d.add_heading('7. 배포 (Client 자동 갱신 · 적용 꾸러미)', 1)
d.add_heading('7.1 Upload / Download 런처', 2)
para(d, 'Upload(WmsUp.exe)는 수정한 Client 파일을 DB(UP_DOWN, DN_MST)에 올리는 프로그램이다. 현장 PC 의 Client 폴더에 있는 런처 Ecs.exe 는 시작할 때 '
        'DB 의 최신 파일을 받아 Client 를 갱신한 뒤 EcsMain.exe 를 띄운다. 파일을 PC 마다 복사하지 않고 한 곳에서 올리면 각 PC 가 다음 실행 때 스스로 받아 간다.')
image(d, shot('upl_main.png'), width_cm=14.0, caption='Upload 화면')
numbered(d, [
    'WmsUp.ini 의 DB 접속을 서버로 맞춘다(화면 [설정]에서도 가능).',
    '[파일 추가] 로 EcsMain.exe 등 올릴 파일을 고르고 받는 쪽 폴더(상대 경로)를 확인한 뒤 [올리기].',
    '같은 이름을 다시 올리면 DN_VER 이 하나 올라간다. 받는 쪽은 가장 높은 버전만 받고 오래된 버전은 정리한다.',
    '각 PC 에서 Ecs.exe 를 다시 띄우면 받아 간다. 제목줄 [Build:…] 로 확인.',
])
note(d, '2026-10-15 판 이전 런처가 깔린 PC 는 Ecs.exe 자체를 받을 때 "사용 중" 오류가 난다. 그 PC 는 Ecs.exe 를 한 번 직접 복사한다. 그 뒤로는 런처가 자기를 .old 로 비켜 놓고 갈아 끼운다.')
d.add_heading('7.2 적용 꾸러미 규약', 2)
table(d, ['규칙', '이유'], [
    ['실제로 바뀐 파일만 넣는다(해시 대조). 소스 변경이 없는 DLL 은 넣지 않는다', '2026-10-27 꾸러미에 DLL 3개가 불필요하게 들어간 사례. 바뀐 게 없는 파일은 "왜 바꿨나" 를 설명할 수 없다'],
    ['ini 는 통째 넣지 않고 "추가할_설정_*.ini.txt" 발췌로', '현장 접속값을 덮어쓰는 사고 방지'],
    ['Client 는 Release 빌드만', 'Debug 를 넣으면 DciLib 심볼 오류'],
    ['안내서는 날짜별 새 문서. 이전 출력본은 손대지 않는다', '어느 날 무엇을 적용했는지 추적'],
    ['읽어보세요.txt 에 바뀐 것 · 적용 순서 · 빌드 시각', '현장에서 문서를 열지 않고도 순서를 따를 수 있게'],
], widths=[8.0, 9.0], font=8.5)
d.add_heading('7.3 2026-11-10 적용분 (적용_20261110)', 2)
table(d, ['폴더', '파일', '적용'], [
    ['Client', 'EcsMain.exe (빌드 2026-11-04) + rc_resource 발췌(dlg_sc · dlg_rtv · mainframe_manual) + 추가할_설정_Ecs.ini.txt', 'Client 닫기 → exe 덮어쓰기(또는 Upload) + rc_resource 파일 덮어쓰기 + Ecs.ini [MENU] HMIDEL_BTN=1 → 제목줄 확인. DLL 그대로'],
    ['TASK\\IO_TASK', 'IO_TASK_SEMI_FINISH.exe (10-29 11:28) + 추가할_설정_ENV_IOSCH.ini.txt(선택)', 'IO 정지 → 덮어쓰기 → 기동. 2분 뒤 [HIS] 로그'],
    ['TASK\\EQP_TASK', 'WCS_TASK_CV.exe (11-05) + 추가할_설정_WCS_DB.ini.txt', 'EQP 정지 → 덮어쓰기 → WCS_DB.INI [HMI_DELETE] 에 7개 주소 기입(M / 6800 ~ 6806, 안내 파일 그대로) → 기동. 기동 로그 [HMI_DEL] 에 SC1=M6800(%MX10880) … CV=M6806(%MX10886) 확인'],
    ['SQL', '261029_사후확인_작업번호.sql', '필요할 때 조회'],
    ['문서', '2026-10-29_적용안내서_추가분 · 끝난작업_사후확인방법 · 2026-11-04_적용안내서_추가분', ''],
], widths=[3.0, 8.0, 6.0], font=8.5)
d.add_page_break()

# ─────────────────────────────────────────────────────────────
d.add_heading('8. 로그', 1)
table(d, ['프로그램', '어디', '무엇', '보존'], [
    ['TASK 3종 (ALL_TASK)', 'ALL_TASK.INI [LOG] DIR\\<스레드>\\<스레드>_yyyyMMdd.log', '스레드별 운전 로그', 'KEEP_DAYS (365)'],
    ['IO_TASK 단독', '실행 폴더 기준 .\\bin\\Debug\\ECS_LOG\\', '중요(Imp)·에러 로그만 파일. 일반 로그는 화면', '수동'],
    ['TASK 공통', 'DB WCS_LOG_PGR', '운전 이력(운전 화면 [ECS 프로그램 이력])', '60일 자동'],
    ['상위 통신', 'DB HOST_IF_LOG', '송수신 전문 원문', '60일 자동'],
    ['설비 통신', 'OutLatch.log (실행 폴더)', '출고대 펄스 래치 진단(2026-09-30)', '수동'],
    ['운전 화면', 'Client\\LOG\\', 'UI 로그(UiLog, 유니코드), 오류', '수동'],
    ['운전 화면', 'DB WCS_CLIENT_LOG', '사용자 조작 + PC IP', '60일 자동'],
    ['설비 에러', 'DB EQP_ERR_HIS', '발생·해제', '30일 자동'],
], widths=[3.0, 6.5, 5.0, 2.5], font=8.5)
note(d, 'TASK 의 DB 계층은 SQL 문자열 안의 ":" 를 파라미터로 보고 "@" 로 바꾼다. 로그 문구나 명령 토큰에 ":" 를 쓰지 않는다(구분자 "-").')
d.add_page_break()

# ─────────────────────────────────────────────────────────────
d.add_heading('9. 장애 대응', 1)
table(d, ['증상', '먼저 볼 곳', '원인 후보', '처치'], [
    ['운전 화면 [통신] EQP 빨강', '신호등 클릭(ping/포트) · WCS_TASK_CV 화면', 'PLC 전원/네트워크, TASK 죽음, DB 하트비트 끊김',
     'ping 불가 → PLC/케이블. 포트 닫힘 → PLC 쪽. TASK 화면이 멈춤 → EQP 재기동(3.2)'],
    ['WMS1 / WMS2 빨강', '〃 · WCS_TASK_HOST 화면', 'WMS 미기동, 8001/8002 방화벽, 우리 HOST 안 떠 있음',
     '우리 8001 닫힘 → HOST 기동. WMS 8002 닫힘 → WMS 담당. 붙자마자 끊기면 SRV_MULTI_CONN=1 확인'],
    ['SCH 빨강', 'IO_TASK 화면', 'TASK 죽음 · DB 끊김 · "이미 종료 중" 창(중복 실행)', '같은 이름 프로세스 확인(3.3) → IO 재기동'],
    ['모든 신호등 빨강, 화면 값 안 바뀜', '제목줄 DB · SQL 서비스', 'DB 서비스 중단 · 1433 막힘 · 디스크 가득', 'SQL 서비스 기동, 디스크 확인(6.2 백업 정리)'],
    ['크레인 3·5호기가 실제 에러 없이 에러색', 'WCS_DB.INI SC_ERR_CODE_BLOCK', '워드 상시 판정(0)이 하트비트 값을 에러로 오독', '3 으로(5.2). 저장 후 2초 내 반영'],
    ['S/C#1 통로에서 입고·출고가 서로 기다림', '운전 화면 방향 칸 · IO_TASK 로그 "사용권"', '방향 불일치 · 사용권 교착', '운전 화면 방향 더블클릭(120초 자동 억제). CV2_DIR_MODE=2 확인'],
    ['작업이 25/35 에서 오래 머문다', '설비 상태창 · 작업정보 단계표', '설비 에러 · 완료 신호 유실 · 포크 위 화물', '설비 에러 해제(조작반) → 안 풀리면 [완료처리](02 사용자 매뉴얼 5.7)'],
    ['화물을 옮겨 놓아도 크레인·RGV 가 새 지시를 안 받음', '설비 지상반 · 상태창', '설비 안에 지난 작업 데이터 잔존', '상태창 [데이터 삭제] 또는 지상반 [삭제] → [완료처리]. [HMI_DELETE] 주소가 비어 있으면 운전 화면 단추는 이력만 남긴다(5.2)'],
    ['출고대에 화물이 왔는데 작업이 15 로 남음', 'CV_DATA 129/126 의 LUGG_NO_RD · RET_READY_LATCH', '3초 펄스 유실 · 번호 소실', 'OUT_DONE_ON_ARRIVAL=1 이면 번호 있는 실도착은 자동 완료. 번호 없으면 [완료처리] ④'],
    ['RGV 가 홈에 고착', 'RTV_DATA_LGLS.PLC_NO', 'EQP_MST 와 자릿수 불일치("1" vs "01") → 화면 조회 0행', 'PLC_NO 를 "01" 로'],
    ['Client 가 아이콘 없이 뜸 / 폰트 설치 거부', '2.3', '작업 디렉터리 · 폰트 잠금', '2.3 처치'],
    ['WCS_LOG_PGR 이 급격히 커짐', 'SELECT TOP 100 … ORDER BY INS_DT DESC', '같은 문구 반복(예 "[] -> [0000]")', '10-29 WCS_TASK_CV 적용 확인, 이력 삭제 동작 확인(6.4)'],
], widths=[3.6, 3.6, 4.4, 5.4], font=8)
warn(d, '어떤 경우에도 시간 경과로 작업을 자동 완료하지 않는다. 설비 신호가 없으면 사람이 실물을 확인하고 넘긴다.')

d.add_heading('10. 점검표', 1)
table(d, ['주기', '항목'], [
    ['매일', '운전 화면 신호등 4개 정상 · 작업정보에 하루 넘게 선 작업 없음 · 알람 경고창 잔류 없음 · ALL_TASK 로그 폴더에 오늘 파일 생성'],
    ['매주', 'IO_TASK [HIS] 로그로 이력 삭제가 돌고 있나 · DB 백업 파일 생성 확인 · 디스크 여유'],
    ['매월', 'DEL_HIS_SETTING 보존기간 적정 · EQP_ERR_HIS 로 잦은 알람 설비 파악 · INI 현장값 점검표(5.1)'],
    ['프로그램 교체 때', '25/35 작업 없음 → 백업 → 덮어쓰기 → 기동 → 제목줄/화면 로그 확인 → 읽어보세요.txt 순서 준수'],
], widths=[2.5, 14.5], font=9)

d.add_heading('부록 A. 적용 이력 (2026)', 1)
table(d, ['꾸러미', '주요 내용'], [
    ['산출물_20260921 / 0927', '현장 기본 시험 완료 기준 전면 갱신. PLC 알람 리스트 SC_LGLS, 통로 사용권, 미사용 표 12개 삭제, 문서 01~13'],
    ['적용_20260930', '출고대 3초 펄스 래치(OUT_SCAN_MS / RET_READY_LATCH), OUT_DONE_ON_ARRIVAL, HOST 겹친 접속 경합(SRV_MULTI_CONN), 폰트 잠금(FR_PRIVATE)'],
    ['적용_20261015', '4K@300% 배율(UI_SCALE/LAYOUT_FONT_SCALE/LAMP_STYLE), START_MONITOR, 작업정보 판넬 3칸 + 스텝 단위 [완료처리], [확대] 패널(구 ECS 폼 배치), 작업 디렉터리 보정, Upload/Download 런처, Setup.bat'],
    ['적용_20261110 (10-29 준비)', '입고 단계표 통로 Step 3, [완료처리] 경로 탐색, 사후 확인 SQL, IO_TASK 이력 자동 삭제(PurgeHistory), WCS_TASK_CV "[] -> [0000]" 로그 억제'],
], widths=[4.0, 13.0], font=8.5)

para(d, '')
para(d, '문서 판 : %s' % G.DATE, size=9, color=GRAY)
out = os.path.join(OUT, '01_ECS_운영자_매뉴얼.docx'); d.save(out); print('saved', out)
