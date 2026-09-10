# -*- coding: utf-8 -*-
"""[LGLS 2026-09-09] 컬럼 설명 보강 (테이블 정의서 Rev2 용)

  이 현장 테이블은 컬럼명이 규칙을 따른다. 그래서
    (1) 뜻이 분명한 컬럼은 EXPLICIT 에 직접 적고
    (2) 나머지는 이름 조각(접두/접미)을 조합해 설명을 만든다.
  규칙으로도 못 만들면 빈 문자열을 돌려준다(억지 설명을 쓰지 않는다).

  근거 : PlcAddressMap.xml(주소 정의), CvThread/cThread_SCH(사용처),
         CvSkinDlg/ScSkinDlg/RtvSkinDlg(화면 표시 항목).
"""
import re

# ─────────────────────────────────────────────────────────────────────────
# 이름 조각
# ─────────────────────────────────────────────────────────────────────────
SUFFIX = {
    '_RD': '설비(PLC) 읽기값',
    '_OD': '설비(PLC) 지시값',
    '_YN': '여부(Y/N)',
    '_YON': '여부(Y/N)',
    '_DT': '일시',
    '_NM': '명칭',
    '_CD': '코드',
    '_ID': 'ID',
    '_NO': '번호',
}

PART = {
    'WH_TYP': '창고 구분', 'LUGG': '작업(화물) 번호', 'JOB': '작업',
    'START': '출발', 'DEST': '도착', 'FROM': '출발', 'TO': '도착',
    'BANK': '뱅크(랙 열)', 'BAY': '베이(랙 칸)', 'LEVEL': '레벨(랙 단)', 'LEV': '레벨(랙 단)',
    'HSPOS': '핸드셰이크 트랙', 'POS': '위치', 'LOCATION': '위치(셀)',
    'FK1': '1번 포크', 'FK2': '2번 포크', 'FK': '포크',
    'SC': '크레인', 'RTV': 'RGV', 'CV': '컨베이어', 'MC': '설비',
    'SENSOR': '센서', 'ERR': '에러', 'ERROR': '에러', 'ALARM': '알람',
    'CMD': '명령', 'RQ': '요청', 'ACK': '응답(ACK)', 'COMPLETE': '완료',
    'LOAD': '상차', 'UNLOAD': '하역', 'TRANSFER': '반송',
    'UPD': '갱신', 'INS': '생성', 'DEL': '삭제', 'USE': '사용',
    'TRACK': '트랙', 'PLC': 'PLC', 'HOST': '상위(WMS/IMS)', 'MES': 'MES',
    'BCR': '바코드', 'TRAY': '트레이', 'PLT': '파렛트', 'PRIORITY': '우선순위',
    'MSG': '메시지', 'MESSAGE': '내용', 'LOG': '로그', 'GRP': '그룹',
    'STA': '상태', 'STATUS': '상태', 'MODE': '모드', 'TIME': '시간', 'DATE': '일자',
    'SEQ': '일련번호', 'CNT': '건수', 'COUNT': '건수', 'STEP': '단계',
    'DIRECTION': '방향', 'SPEED': '속도', 'POOL': '수조', 'FIRE': '화재',
    'WAIT': '대기', 'TURN': '방향전환', 'ITN': '설비 보유', 'VIEW': '표시',
    'PRODUCT': '제품', 'ITEM': '품목', 'REMARK': '비고', 'REMARKS': '비고',
    'PGR': '프로그램', 'WIN': '화면', 'USER': '사용자', 'IP': '접속 IP',
}

# ─────────────────────────────────────────────────────────────────────────
# 뜻이 분명한 컬럼 - 직접 적는다
# ─────────────────────────────────────────────────────────────────────────
EXPLICIT = {
    # 공통
    'WH_TYP': '창고 구분(10 = LG 화학 1동)',
    'LUGG_NO': '작업(화물) 번호 4자리. 이 시스템의 작업 식별자',
    'JOB_TYP': '작업 구분 1=입고 2=출고 3=피킹출고 4=랙투랙 5=호기간이동 6=이동, 1x=반자동',
    'JOB_STATUS': '작업 상태 99 신규 → 10/15 C/V → 20/25 S/C → 30/35 RGV → 19/29 완료 → 09 이력이관',
    'START_POS': '출발지 설비번호(트랙 1xx / 크레인 90x / RGV 801)',
    'DEST_POS': '도착지 설비번호',
    'START_LOCATION': '출발 셀(뱅크-베이-레벨, 예 01-003-02)',
    'DEST_LOCATION': '도착 셀(뱅크-베이-레벨)',
    'PRODUCT_ID': '제품 정보(상위가 내려준 값)',
    'LOT_NO': '적재 용기(파렛트) 번호',
    'JOB_PRIORITY': '우선순위. 값이 작을수록 먼저 처리(기본 100)',
    'HS_TRACK_NO': 'RGV 인계 트랙(핸드셰이크 위치)',
    'INS_DT': '생성 일시', 'UPD_DT': '갱신 일시',
    'INS_USER_ID': '생성자', 'UPD_USER_ID': '갱신자',
    'REMARKS': '비고', 'REMARK': '비고',
    'DEL_YN': '삭제 여부',

    # 설비 공통
    'MC_NO': '설비(트랙) 번호. C/V 는 101~132 = TR#1~32',
    'MC_NO_NM': '설비 명칭',
    'PLC_NO': 'PLC(컨베이어 그룹) 번호 01~15',
    'TRACK_NO': '트랙 번호',
    'SC_NO': '크레인 번호 901~905',
    'RTV_NO': 'RGV 번호(801)',
    'FK_NO': '포크 번호(1/2)',
    'ERR_CODE_RD': '설비 에러 코드(0000 = 정상)',
    'ERROR_CODE': '설비 에러 코드(0000 = 정상)',
    'AUTO_MODE_RD': '자동 모드 여부',
    'ONLINE_MODE_RD': '온라인 여부',
    'ACTIVE_MODE_RD': '가동 여부',
    'UCSTATUS_RD': '설비 상태(1 IDLE / 2 RUN)',
    'SUBSYSTEM_STATUS_RD': '서브시스템 상태(1 IDLE / 2 RUN)',
    'SUSPEND': '사용 중지(1 입고 금지 / 2 출고 금지 / 3 입출고 정지)',
    'CONNECTED_YN': 'TASK 하트비트. 상태바 EQUIP/HOST/SCH 판정 근거',
    'EQP_TIME': '설비 데이터 갱신 시각',
    'HOST_ERR_SEND_YN': '상위 에러 보고 여부',
    'HOST_SEND_YN': '상위 보고 여부',
    'OD_RQ_YN': '지시 요청 여부(Y = 설비가 아직 소비하지 않음)',
    'OD_RQ_FLAG': '지시 요청 플래그(내부)',
    'CMD_RQ_YN': '명령 요청 여부',
    'CMD_RQ_ID': '명령 코드(RESET / PAUSE / DELFK1 등)',
    'CMD_RQ_PARM': '명령 파라미터(방향 전환 등에서 1=출고 0=입고)',
    'READ_UPD_DT': '읽기값 갱신 시각', 'WRITE_UPD_DT': '지시 기록 시각',
    'OD_USER_ID': '지시한 사용자', 'OD_UPD_DT': '지시 시각',

    # C/V
    'LUGG_NO_RD': '트랙에 실려 있는 작업번호(PLC 트래킹 R영역 읽기값)',
    'LUGG_NO_OD': '트랙에 써 넣을 작업번호(지시)',
    'DEST_POS_RD': '설비가 들고 있는 도착지',
    'DEST_POS_OD': '설비에 써 넣을 도착지(지시)',
    'JOB_TYP_RD': '설비가 들고 있는 작업 구분',
    'JOB_TYP_OD': '설비에 써 넣을 작업 구분(지시)',
    'SENSOR0_DATA_RD': '재하(在荷) 감지 - 트랙에 화물이 있는지',
    'STO_READY_RD': '입고 준비 완료(작업대에서 화물을 받을 수 있음)',
    'RET_READY_RD': '출고 준비 완료',
    'STOHS_READY_RD': '입고 핸드셰이크 준비',
    'RETHS_READY_RD': '출고 핸드셰이크 준비',
    'RTV_DEPARTHS_READY_RD': 'RGV 출발 핸드셰이크 준비',
    'RTV_ARRIVEHS_READY_RD': 'RGV 도착 핸드셰이크 준비',
    'STOCK_MODE': '입출고 모드(0 = 입고, 1 = 출고). 방향전환형 C/V 에 쓴다',
    'DIRECTION_MODE_RD': '현재 방향(방향전환형 C/V)',
    'DIRECTION_MODE_OD': '방향 전환 지시',
    'TR_PAUSE_RD': '트랙 일시정지 상태', 'TR_PAUSE_OD': '트랙 일시정지 지시',
    'TRACKING_WRITE_YN': 'PLC 트래킹(R영역) 기록 요청 여부',
    'WAIT_TIME_RD': '트랙 대기 시간(초)',
    'REMOTE_CONTROL': '원격/수동 상태',
    'EMERGENCY_RD': '비상정지',
    'PULP_SENSOR_RD': '화물 감지 보조 센서',
    'SC_PLT_JOB_TYP': '파렛트 종류 구분(작업대/셀 짝 맞춤용)',
    'STN_KIND': '스테이션 종류(비트마스크)',
    'BARCODE': '바코드 판독값',

    # S/C
    'ITN_LUGG_FK1': '1번 포크가 물고 있는 작업번호(설비 보유값)',
    'ITN_LUGG_FK2': '2번 포크가 물고 있는 작업번호',
    'LUGG_NO_FK1_RD': '1번 포크 차상(車上) 작업번호',
    'LUGG_NO_FK2_RD': '2번 포크 차상 작업번호',
    'POS_H_RD': '현재 수평 위치(베이)',
    'POS_V_RD': '현재 수직 위치(레벨)',
    'COMPLETE_RD': '작업 완료 신호',
    'ERR_STA_FK1_RD': '1번 포크 에러 상태',
    'ERR_STA_FK2_RD': '2번 포크 에러 상태',
    'FORKPOS_FK1_RD': '1번 포크 위치',
    'FORKPOS_FK2_RD': '2번 포크 위치',
    'USE_FK_RD': '사용 중인 포크', 'USE_FK_OD': '사용할 포크(지시)',
    'CV_WORKBENCH_RD': '크레인이 붙어 있는 작업대',
    'CV_WORKBENCH_SUB_RD': '작업대 보조 위치',
    'SC_VIEW_NM': '화면 표시용 크레인 명칭',
    'SC_TYP': '크레인 종류',
    'PLT_INFO_RD': '파렛트 정보',
    'SC_GRP_NO': '크레인 그룹 번호',
    'USER_COMMAND_OD': '사용자 수동 명령',
    'WRITE_FLAG_OD': '지시 기록 플래그', 'WRITE_CONTINUE_OD': '연속 지시 플래그',

    # RGV
    'PALLET_ON_VEHICLE_RD': '차상 파렛트(RGV 가 싣고 있는 화물)',
    'TRANSFER_REQUEST_OD': '반송 요청(지시)',
    'DEPART_TRACK': 'RGV 출발 트랙(실제 트랙번호)',
    'ARRIVE_TRACK': 'RGV 도착 트랙(실제 트랙번호)',
    'PALLET_ID_OD': '지시 파렛트',
    'RTV_PASSCV_OD': 'RGV 경유 C/V 지시',
    'RTV_DEST_OD': 'RGV 도착지 지시',
    'WAITING_ORDER_RD': '대기 순번',
    'SENSOR_RTV_RD': 'RGV 재하 센서',
    'SPEED_RD': '주행 속도',

    # 마스터/정의
    'EQP_TYP': '설비 종류(EQP 설비통신 / HOST 상위통신 / SCH 스케줄러 / CV / SC / RTV)',
    'PLC_IP': 'PLC 접속 IP', 'PLC_PORT': 'PLC 접속 포트',
    'PLC_PORT_FROM': 'PLC 포트 범위 시작', 'PLC_PORT_TO': 'PLC 포트 범위 끝',
    'SOCK_TIMEOUT': '소켓 타임아웃(ms)', 'RETRY_YN': '재시도 여부',
    'USE_YN': '사용 여부', 'USE_YON': '사용 여부',
    'CDX_CD': '코드군', 'CCD_CD': '코드',
    'CCD_NM_KOR': '명칭(한글)', 'CCD_NM_ENG': '명칭(영문)',
    'CCD_NM_CHIN': '명칭(중문)', 'CCD_NM_HUN': '명칭(헝가리어)',
    'CCD_EPR_ORD': '표시 순서', 'CCD_CD_YN': '사용 여부',
    'FR_CV': '출발 컨베이어', 'TO_CV': '도착 컨베이어',
    'WAIT_CV': '대기 컨베이어', 'BUFFER_CV': '버퍼 컨베이어',
    'FRONT_TRACK_NO': '앞 트랙 번호(흐름 순서)',
    'FLOW_YN': '흐름 사용 여부',
    'AREA': '담당 뱅크 목록(예 01,02). 이 값으로 로케이션의 담당 크레인을 정한다',
    'CV_NO': '크레인이 붙는 컨베이어 번호',
    'HS_NO': '핸드셰이크 번호', 'HS_MC_NO': '핸드셰이크 설비 번호',
    'HS_USE_YN': '핸드셰이크 사용 여부', 'PAIR_HS_NO': '짝이 되는 핸드셰이크',
    'WAIT_TRACK': '대기 트랙',
    'GROUP_NO': '목적지 그룹(OUT 출고대 / SC 크레인)',
    'RET_CNT': '출고 배정 누적 건수(부하 분산용)',
    'PRIORITY': '우선순위',
    'EQP_ERR_CD': '설비 에러 코드',
    'MSG_KOR': '에러 메시지(한글)', 'MSG_ENG': '에러 메시지(영문)',
    'MSG_CHIN': '에러 메시지(중문)', 'MSG_HUN': '에러 메시지(헝가리어)',
    'ACTION_KOR': '조치 방법(한글)', 'ACTION_ENG': '조치 방법(영문)',
    'ACTION_CHIN': '조치 방법(중문)', 'ACTION_HUN': '조치 방법(헝가리어)',
    'ERR_LEV': '에러 등급',
    'EQP_NO': '설비 번호', 'ERROR_DT': '에러 발생 일시',
    'TABLE_NAME': '대상 테이블', 'CYCLE': '보존 주기', 'UNIT': '단위(DAY)',
    'KEY_COLUMN': '보존 주기 판정에 쓰는 일시 컬럼',
    'CUR_POS': '현재 위치', 'CUR_DEST_POS': '현재 도착지',
    'CHK_DEST_POS': '검사할 도착지', 'BUFFERS': '버퍼 트랙 목록',
    'EQP_STA_CHK_TR': '상태를 확인할 트랙',

    # 로그
    'PGR_NM': '프로그램(스레드) 명',
    'LOG_SEQ': '로그 일련번호',
    'LOG_KOR': '로그 내용(한글)',
    'LOG_DATE': '일자(YYYYMMDD)', 'LOG_TIME': '시각(HH:MM:SS.mmm)',
    'HOST_CMD': '상위 전문 종류(O 지시 / R 응답 / M 모드 / S 상태 / E 에러 / F 완료)',
    'DIRECTION': '송수신 방향(SND/RCV)',
    'MESSAGE': '전문 원문 또는 내용',
    'BCR_BOTTOM': '하단 바코드', 'BCR_TOP': '상단 바코드',
    'TRACK_FROM': '출발 트랙', 'TRACK_TO': '도착 트랙',
    'JOB_STA': '작업 상태', 'RQ_INS_ID': '요청자', 'RQ_INS_DT': '요청 일시',
    'MES_CMD': 'MES 전문 종류', 'SR_TYPE': '송수신 구분',
    'IP': '조작한 PC 의 IP', 'ALARM_YN': '알람 표시 대상 여부',
    'ALARM_CHK_YN': '알람 확인 여부',
    'BOTTOM_TRAY': '하단 트레이', 'TOP_TRAY': '상단 트레이',

    # 사용자/권한
    'USER_ID': '사용자 ID', 'USER_NM': '사용자명', 'USER_PW': '비밀번호',
    'GRP_ID': '그룹 ID', 'GRP_CD': '그룹 코드', 'GRP_DESC': '그룹 설명',
    'GRP_LEVEL': '권한 등급',
    'WIN_ID': '화면 ID', 'WIN_VIEW_YN': '화면 표시 권한',
    'SEL_YN': '조회 권한', 'INS_YN': '입력 권한',
    'UPD_YN': '수정 권한', 'EXE_YN': '실행 권한',
    'PRT_YN': '출력 권한', 'DOWN_YN': '내려받기 권한',
    'PGR_NM_KOR': '화면 명칭(한글)', 'UPD_ID': '갱신자',
}

# 규칙으로 만들 때 쓰는 정규식 (앞에서부터 맞는 것을 쓴다)
RULES = [
    # 크레인 좌표 : START_BANK_FK1_RD 등
    (re.compile(r'^(START|DEST)_(BANK|BAY|LEVEL|HSPOS)_(FK\d)_(RD|OD)$'),
     lambda m: '%s %s %s (%s)' % (PART[m.group(1)], PART[m.group(3)], PART[m.group(2)],
                                  '읽기값' if m.group(4) == 'RD' else '지시값')),
    # 좌표 3분할 : LOCATION_01_RD, FROM_02_OD, TO_03_OD
    (re.compile(r'^(LOCATION|FROM|TO)_0(\d)_(RD|OD)$'),
     lambda m: '%s 좌표 %s번째 자리(뱅크/베이/레벨) %s'
               % (PART[m.group(1)], m.group(2), '읽기값' if m.group(3) == 'RD' else '지시값')),
    (re.compile(r'^TRANSFER_COMPLETE_LOCATION_0(\d)_RD$'),
     lambda m: '반송 완료 위치 %s번째 자리(뱅크/베이/레벨) 읽기값' % m.group(1)),
    # 알람 핸드셰이크
    (re.compile(r'^ALARM_(SET|RESET)_REPORT_ACK_OD$'),
     lambda m: '알람 %s 보고에 대한 응답(ACK) 지시' % ('발생' if m.group(1) == 'SET' else '해제')),
    (re.compile(r'^ALARM_(SET|RESET)_REPORT_RD$'),
     lambda m: '알람 %s 보고 신호(설비 → 시스템)' % ('발생' if m.group(1) == 'SET' else '해제')),
    (re.compile(r'^ALARM_(SET|RESET)_CODE_RD$'),
     lambda m: '알람 %s 코드' % ('발생' if m.group(1) == 'SET' else '해제')),
    # 상·하차 핸드셰이크
    (re.compile(r'^(LOAD|UNLOAD)_COMPLETE_RD$'),
     lambda m: '%s 완료 신호(설비 → 시스템)' % PART[m.group(1)]),
    (re.compile(r'^(LOAD|UNLOAD)_COMPLETE_ACK_OD$'),
     lambda m: '%s 완료에 대한 응답(ACK) 지시' % PART[m.group(1)]),
    (re.compile(r'^UNLOAD_REQUEST_OD$'), lambda m: '하역 요청 지시'),
    (re.compile(r'^UNLOAD_REQUEST_ACK_RD$'), lambda m: '하역 요청에 대한 설비 응답'),
    (re.compile(r'^TRANSFER_ACK_RD$'), lambda m: '반송 지시에 대한 설비 응답(ACK)'),
    # 센서/에러 계열
    (re.compile(r'^SENSOR(\d|_FK\d|_RTV)_(DATA_)?RD$'), lambda m: '센서 감지값 읽기'),
    (re.compile(r'^(\w+)_ERR$'), lambda m: '%s 에러' % _kor(m.group(1))),
    (re.compile(r'^(\w+)_SENSOR\d?$'), lambda m: '%s 센서' % _kor(m.group(1))),
    (re.compile(r'^(\w+)_POS$'), lambda m: '%s 위치' % _kor(m.group(1))),
]


# ─────────────────────────────────────────────────────────────────────────
# 나머지 - 이 현장에서 쓰지 않는 잔재는 억지로 뜻을 붙이지 않고 그대로 밝힌다.
#   (구 ECS/타 현장 설비용 컬럼이 스키마에 함께 남아 있다)
# ─────────────────────────────────────────────────────────────────────────
OTHER_SITE = '(이 현장 미사용 - 타 현장 설비용 컬럼)'
SECS       = '(SECS/GEM 연동용 - 이 현장 미사용)'
RESIDUAL = {
    'TURN': '방향전환 여부', 'A_TURN_YN': 'A 방향전환 여부', 'B_TURN_YN': 'B 방향전환 여부',
    'IS_TURN_RD': '방향전환 중 여부',
    'JOB_KIND': '작업 종류(상위 구분값)',
    'PRODUCT_SIZE': '제품 규격',
    'TRAY_TYP': '트레이 종류', 'TRAY_LEV': '트레이 단수',
    'DURATION_TIME': '작업 소요 시간',
    'AGING_TIME': '숙성(에이징) 시간',
    'PAIR_LUGG_NO': '짝이 되는 작업번호(2매 적재 등)',
    # [LGLS 2026-09-10] 실제로 쓰인다 - HOST 완료보고 재보고 방지 플래그.
    #   WCS_TASK_HOST/CCliWork.cs 가 WC_STEP='0' 인 작업만 골라 보고하고 '1' 로 올린다.
    'WC_STEP': 'HOST 완료보고 진행 플래그(0=미보고 / 1=보고 완료). 같은 작업이 두 번 보고되지 않게 막는다',
    'CMP_STEP': '완료 처리 단계(예비)',
    'OD_LAST_PAGE': '지시 마지막 페이지(예비)', 'OD_LAST_USER': '마지막 지시자',
    'MES_WRITE_YN': 'MES 기록 여부(이 현장 미사용)',
    'MES_ERROR_CD': 'MES 에러 코드(이 현장 미사용)',
    'CMD_STA': '명령 상태',
    'JOB_START_DT': '작업 시작 일시',
    'INS_DATE': '생성 일자(YYYYMMDD)', 'INS_TIME': '생성 시각',
    'STEP': '진행 단계',
    'BATCH': '배치 구동 여부', 'PROCESS': '프로세스명',
    'SET_TIME': '설정 시각', 'COUNT': '건수',
    'DEST_DAT_POS': '도착 데이터 위치', 'DEST_DAT_TOKEN': '도착 데이터 구분자',
    'DEST_DEF_DAT': '도착 정의 데이터',
    'FD_OVERTIME_RD': '주행 시간 초과',
    'DELETE_TRACK_RD': '트랙 삭제 요청',
    'RTV_LOCK_SENSOR': 'RGV 인터록 센서', 'SC_LOCK_SENSOR': '크레인 인터록 센서',
    'DOOR_STATUS_RD': '도어 상태',
    'CV_CHECK_POS_RD': '트랙 확인 위치',
    'WAIT_SC_RET_JOB_RD': '크레인 출고 대기 작업', 'WAIT_SC_RET_JOB_OD': '크레인 출고 대기 지시',
    'WAIT_IN_RD': '입고 대기', 'WAIT_OUT_RD': '출고 대기', 'IN_READY_RD': '입고 준비',
    'RET_STATUS': '출고 처리 상태',
    'COMP_VR': '완료 확인값',
    'ITEM_NO': '품목 번호',
    'SC_FIRE_OD': '화재 시 크레인 대피 지시',
    'POOL_POSITION_RD': '수조 위치', 'POOL_LIMIT_RD': '수조 한계', 'POOL_WATER_LEVEL': '수조 수위',
    'SC_PLT_JOB_TYP_RD': '파렛트 종류(읽기)', 'SC_PLT_JOB_TYP_OD': '파렛트 종류(지시)',
    'IS_ERROR_RD': '에러 여부',
    'ERR_RQ_RD': '에러 보고 요청(설비)', 'ERR_RQ_OD': '에러 보고 요청(지시)',
    'ROLL_MODE': OTHER_SITE,
    'SYSTEM_BYTE': SECS, 'S6F12_RESPONSE': SECS, 'S6F11_YON': SECS, 'CEID_NAME': SECS,
}
_OTHER_PREFIX = ('REV_', 'ROLL_', 'D5N_', 'COMMING', 'PAPER_', 'CRUSH_', 'CLAMP_', 'DRIV_', 'ELEV_')


def _kor(name):
    """이름 조각들을 한글로 바꿔 이어 붙인다."""
    out = []
    for w in name.split('_'):
        out.append(PART.get(w, w))
    return ' '.join(out)


def describe(col):
    c = col.upper()
    if c in EXPLICIT:
        return EXPLICIT[c]
    if c in RESIDUAL:
        return RESIDUAL[c]
    for pre in _OTHER_PREFIX:
        if c.startswith(pre):
            return OTHER_SITE
    for pat, fn in RULES:
        m = pat.match(c)
        if m:
            try:
                return fn(m)
            except Exception:
                pass
    # 접미사 + 조각 조합
    for suf, sdesc in SUFFIX.items():
        if c.endswith(suf):
            base = c[:-len(suf)]
            words = [PART[w] for w in base.split('_') if w in PART]
            if words:
                return '%s %s' % (' '.join(words), sdesc)
    words = [PART[w] for w in c.split('_') if w in PART]
    if len(words) >= 2:
        return ' '.join(words)
    return ''
