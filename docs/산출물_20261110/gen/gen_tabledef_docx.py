# -*- coding: utf-8 -*-
"""03 테이블 정의서 - 2026-11-10 종료 산출물.
   2026-09-27 판(Rev4, 25표) 뒤 추가된 Upload/Download 런처용 표 3개(DN_MST / DN_SEQ / UP_DOWN)와
   출고대 3초 펄스 래치 열 3개(CV_DATA.RET_READY_LATCH / _LUGG / _DT)를 반영.
   스키마 : schema_1102.txt / tables_1102.txt (2026-11-02 개발 DB LGLS_MCS_IO 에서 추출, *_BAK 표 제외)
   2026-11-09 사용자 검토 : CELL_MST / CELL_DTL 삭제(DB_BACKUP/20_CELL표_삭제.sql) → 26 표. 본문 설명에서 날짜 문구와
     "운전 화면 ▲▼" 문구를 뺐다(사용자가 03_테이블_정의서.docx 에 직접 고친 내용을 생성기에 옮긴 것).
   실행 : python gen_tabledef_docx.py   (출력이 열려 있으면 _MMDD재생성.docx 로 저장)"""
import os, re
from collections import OrderedDict
from common1110 import *
import coldesc

TDESC = OrderedDict([
 ('JOB_MST', ('작업 마스터', '진행 중 입/출고 작업. 완료(09)되면 JOB_MST_HIS 에 마지막 행을 남기고 삭제된다. 스케줄러(IO_TASK)가 상태를 진행시키고, 트리거 trg_JOB_MST_StatusHis 가 상태 변화를 JOB_MST_HIS 에 기록한다.')),
 ('JOB_MST_HIS', ('작업 이력', '작업 상태 변화 이력(상태별 1행). 운전 화면 [작업 로그]·사후 확인 SQL 의 조회 대상. DEL_HIS_SETTING(60일)으로 IO_TASK 가 자동 삭제.')),
 ('CV_DATA', ('컨베이어 트랙 데이터', '트랙(101~132)별 PLC 읽기값(_RD)과 지시값(_OD). 설비 통신(WCS_TASK_CV)이 갱신, 스케줄러가 지시. 출고대 3초 펄스 래치 열(RET_READY_LATCH*) 포함.')),
 ('SC_DATA_LGLS', ('스태커 크레인 데이터', '크레인 901~905 의 상태/지시. 현장 PLC 맵(LGLS) 기준 컬럼. PK_SC_DATA_LGLS(SC_NO).')),
 ('RTV_DATA_LGLS', ('RGV 데이터', 'RGV 801 의 상태/지시. DEPART_TRACK/ARRIVE_TRACK 은 실제 트랙번호. PLC_NO 는 EQP_MST 와 같은 2자리(01). PK_RTV_DATA_LGLS(RTV_NO).')),
 ('EQP_MST', ('설비/프로그램 마스터', 'PLC 접속 정보, TASK 하트비트(UPD_DT/CONNECTED_YN). 운전 화면 [통신] 신호등 EQP/SCH/HOST 판정 근거. EQP·HOST·HOST2·SCH 네 행 중심.')),
 ('EQP_ECD_MST', ('설비 에러코드 마스터', '설비별 에러코드와 명칭. EQP_TYP : CV / RTV / SC(공통) / SC_SFA / SC_MURATA_EN / SC_RESP / SC_LGLS(PLC 알람 리스트, 크레인 지상반 알람 전량).')),
 ('EQP_ERR_HIS', ('설비 에러 이력', '에러 발생/해제 이력. 운전 화면 [설비에러이력] 조회 대상. DEL_HIS_SETTING(30일, ERROR_DT)으로 자동 삭제.')),
 ('COMMON_CODE', ('공통 코드', 'CDX_CD(코드군)/CCD_CD(코드). 작업상태·작업구분·창고구분·콤보 항목 등. 다국어 명칭 컬럼 보유. JOB_STATUS 16(통로CV 구동중)·SC_ERR_STA_FK 3(사용정지 DOWN) 포함.')),
 ('CV_DEF_INF', ('컨베이어 정의', '트랙 간 이동 정의(From/Buffer/To).')),
 ('CV_FLOW_DEF', ('컨베이어 흐름 정의', '트랙 흐름 순서 정의.')),
 ('DEST_POS_DEF', ('목적지 위치 정의', '작업대·크레인 위치 정의. REMARKS 가 운전 화면 출발지/도착지 이름표(PosLabel)의 출처.')),
 ('DEAD_LOCK_ZONE_DEF', ('교착 구간 정의', '동시 진입 금지 구간.')),
 ('SC_DEF_INF', ('크레인 정의', '크레인별 담당 뱅크 등.')),
 ('SC_HS_DEF', ('크레인 H/S 정의', '크레인별 H/S 트랙(구 정의, 스케줄러 일부 쿼리에서 참조).')),
 ('DEL_HIS_SETTING', ('이력 삭제 주기 설정', '로그 표별 보존 주기. 운전 화면 [로그 삭제 설정]에서 고치고, IO_TASK PurgeHistory 가 읽어 실제로 지운다.')),
 ('HOST_IF_LOG', ('상위(WMS/IMS) 통신 로그', '송수신 전문 이력. 운전 화면 [HOST 로그]. 60일 자동 삭제.')),
 ('WCS_LOG_PGR', ('프로그램 이력', 'TASK 들이 남기는 운전 이력. 운전 화면 [ECS 프로그램 이력]. 인덱스 IX_WCS_LOG_PGR_INS_DT(ins_dt, pgr_nm). 60일 자동 삭제.')),
 ('WCS_CLIENT_LOG', ('운전 화면 조작 로그', '사용자 조작(지시/삭제/설정 변경/완료처리/방향 전환) 기록과 PC IP. [유저사용 로그]. 60일 자동 삭제.')),
 ('CHG_LANG', ('다국어 문구', '운전 화면 문구 번역표. 화면 ini 의 4개 언어 섹션과 짝을 이룬다. 현재 0행(ini 가 기준).')),
 ('USER_MST', ('사용자', '운전 화면 로그인 사용자.')),
 ('USER_GRP', ('사용자 그룹', '권한 그룹.')),
 ('USER_GRP_WIN', ('그룹별 화면 권한', '그룹×화면 조회/수정/실행 권한(UPD_YN/EXE_YN 등). 완료처리·우선순위 변경은 UPD_YN 으로 판정.')),
 ('DN_MST', ('배포 파일 이력 (Upload/Download)', 'Upload(WmsUp.exe)가 Client 파일을 올릴 때 한 묶음(DN_NO)마다 1행. 런처 Ecs.exe 가 기동 때 읽어 받을 것이 있는지 판단한다. DN_MST_PK(DN_NO).')),
 ('DN_SEQ', ('배포 번호 시퀀스', 'DN_MST.DN_NO 채번용 1행 표(SEQ_NO). Upload 가 올릴 때 1 올린다.')),
 ('UP_DOWN', ('배포 파일 본체 (Upload/Download)', '올린 파일의 내용(UP_DAT, varbinary(max))과 이름·크기·버전. 같은 이름을 다시 올리면 DN_VER 이 올라가고, 런처는 가장 높은 버전만 받은 뒤 오래된 버전을 정리한다. UP_DOWN_PK(DN_NO, DN_LN), UP_DOWN_IX1.')),
])

CDESC = {
 'WH_TYP': '창고 구분(10 = LG 화학 1동)', 'LUGG_NO': '작업(화물) 번호 4자리', 'JOB_TYP': '작업 구분 1=입고 2=출고 3=피킹출고 4=랙투랙 5=호기간이동 6=이동 1x=반자동',
 'JOB_STATUS': '작업 상태(COMMON_CODE) 99 신규 10/11/15 CV 16 통로CV 구동중 20/21/25 SC 30/31/35 RGV 39 RGV완료 19/29 완료 09 이력이관', 'START_POS': '출발지(설비번호 : 트랙 1xx / 크레인 90x)', 'DEST_POS': '도착지(설비번호)',
 'START_LOCATION': '출발 셀(뱅크-베이-레벨)', 'DEST_LOCATION': '도착 셀(뱅크-베이-레벨)', 'PRODUCT_ID': '제품 정보', 'LOT_NO': '적재 용기(파렛트)',
 'JOB_PRIORITY': '우선순위(기본 100)', 'HS_TRACK_NO': 'RGV 인계 트랙(H/S). 입고 15/16 에서 통로 트랙이면 단계표 Step 3', 'SC_NO': '크레인 번호', 'FK_NO': '포크 번호', 'DEL_YN': '삭제 여부',
 'INS_DT': '생성 일시(이력 표는 보존기간 판정 열)', 'UPD_DT': '갱신 일시', 'UPD_USER_ID': '갱신자', 'INS_USER_ID': '생성자', 'REMARKS': '비고/설명',
 'MC_NO': '설비(트랙) 번호', 'MC_NO_NM': '설비 명칭', 'TRACK_NO': '트랙 번호', 'PLC_NO': 'PLC(컨베이어 그룹) 번호 2자리', 'LUGG_NO_RD': '트래킹 작업번호(읽기)',
 'LUGG_NO_OD': '지시 작업번호', 'DEST_POS_OD': '지시 목적지', 'JOB_TYP_OD': '지시 작업구분', 'OD_RQ_YN': '지시 요청 여부(Y=미소비)', 'CMD_RQ_ID': '명령 코드(RESET/PAUSE/DELFK1/DIR 등)',
 'CMD_RQ_YN': '명령 요청 여부', 'SENSOR0_DATA_RD': '재하 감지', 'STOCK_MODE': '입출고 모드(0 입고, 1 출고)', 'TR_PAUSE_RD': '트랙 일시정지 상태', 'TR_PAUSE_OD': '트랙 일시정지 지시',
 'ERR_CODE_RD': '에러 코드', 'AUTO_MODE_RD': '자동 모드', 'ONLINE_MODE_RD': '온라인', 'ACTIVE_MODE_RD': '가동', 'UCSTATUS_RD': '상태(1 IDLE 2 RUN)', 'SUBSYSTEM_STATUS_RD': '서브시스템 상태(1 IDLE 2 RUN)',
 'PALLET_ID_OD': '지시 파렛트', 'FROM_01_OD': '출발 좌표1', 'TO_01_OD': '도착 좌표1', 'DEPART_TRACK': 'RGV 출발 트랙', 'ARRIVE_TRACK': 'RGV 도착 트랙', 'RTV_NO': 'RGV 번호(801)',
 'PALLET_ON_VEHICLE_RD': '차상 파렛트', 'TRANSFER_REQUEST_OD': '반송 요청', 'HOST_ERR_SEND_YN': '상위 에러 보고 여부', 'SUSPEND': '사용 중지(1 입고 2 출고 3 입출)',
 'EQP_TYP': 'EQP_MST: 설비 종류(EQP/HOST/HOST2/SCH/CV/SC/RTV). EQP_ECD_MST: CV/RTV/SC/SC_SFA/SC_MURATA_EN/SC_RESP/SC_LGLS', 'PLC_IP': 'PLC IP', 'PLC_PORT': 'PLC 포트', 'CONNECTED_YN': '접속 여부(하트비트)', 'USE_YN': '사용 여부',
 'CDX_CD': '코드군', 'CCD_CD': '코드', 'CCD_NM_KOR': '명칭(한글)', 'CCD_NM_ENG': '명칭(영문)', 'CCD_NM_CHIN': '명칭(중문)', 'CCD_EPR_ORD': '표시 순서', 'CCD_CD_YN': '사용 여부',
 'TABLE_NAME': '대상 테이블(이 DB 에 없는 표는 IO_TASK 가 건너뜀)', 'CYCLE': '보존 주기(숫자, 1 이상)', 'UNIT': '단위(DAY / HOUR / MONTH)', 'KEY_COLUMN': '보존기간 판정 열(INS_DT / ERROR_DT)',
 'PGR_NM': '프로그램(스레드) 명', 'MSG': '메시지', 'LOG_MSG': '메시지',
 'USER_ID': '사용자 ID', 'USER_NM': '사용자명', 'USER_PW': '비밀번호', 'GRP_ID': '그룹 ID', 'WIN_ID': '화면 ID', 'UPD_YN': '수정 권한', 'EXE_YN': '실행 권한', 'INS_YN': '입력 권한', 'SEL_YN': '조회 권한',
 'BANK': '뱅크', 'BAY': '베이', 'LEVEL': '레벨', 'CELL_STATUS': '셀 상태', 'CELL_SC_NO': '담당 크레인',
 # 2026-09-29 출고대 3초 펄스 래치
 'RET_READY_RD': '출고대 도착(WAIT_IN) 신호 읽기값', 'RET_READY_LATCH': '출고대 도착 펄스 래치(Y = 3초 펄스를 잡아 둔 상태, 설비 통신이 세우고 스케줄러가 소비)',
 'RET_READY_LATCH_LUGG': '래치 시점에 출고대에 있던 작업번호', 'RET_READY_LATCH_DT': '래치 시각(60초 지나면 재확인)',
 # 2026-10-15 Upload/Download
 'DN_NO': '배포 묶음 번호(DN_SEQ 로 채번)', 'DN_LN': '묶음 안의 줄 번호(파일 순번)', 'DN_PGM': '대상 프로그램 구분(ECS Client)', 'DN_DIR': '받는 쪽 저장 폴더(상대 경로)',
 'DN_INF': '올린 때의 설명', 'UP_CPT_NM': '올린 PC 이름', 'UP_DNT': '올린 일시', 'SEQ_NO': '다음 DN_NO', 'UP_FNM': '올린 원본 파일 전체 경로', 'DN_NM': '받는 쪽 파일 이름',
 'DN_SIZE': '파일 크기(바이트)', 'DN_VER': '버전(같은 이름 재업로드마다 +1, 런처는 최고 버전만 받음)', 'UP_DAT': '파일 내용(varbinary(max))', 'UP_DT': '올린 일시',
}

EXCLUDE = ('CELL_MST', 'CELL_DTL')      # 2026-11-09 삭제한 표(DB_BACKUP/20_CELL표_삭제.sql). 스키마 파일에는 남아 있어 여기서 뺀다
# 본문 설명에는 날짜를 쓰지 않는다(사용자 지시 11/9). 표지 작성일 · 개정이력 표는 그대로 둔다(사용자 수정본과 같게).
SPARE = re.compile(r'^(SPARE|RESERVE|DUMMY)\d*$', re.I)

def read_txt(p):
    b = open(p, 'rb').read()
    try: return b.decode('utf-8-sig')
    except UnicodeDecodeError: return b.decode('cp949', errors='replace')

rows = [l.split('|') for l in read_txt(os.path.join(G.SCRATCH, 'schema_1102.txt')).splitlines()
        if '|' in l and not l.startswith('TABLE_NAME') and not set(l.strip()) <= set('-|')]
by = OrderedDict()
for r in rows:
    if len(r) < 8: continue
    t = r[0].strip()
    if t.upper() in EXCLUDE: continue
    if SPARE.match(r[2].strip()): continue
    by.setdefault(t.upper(), []).append([c.strip() for c in r])
# 문서 순서 = TDESC 순서, TDESC 에 없는 표는 뒤에
ordered = OrderedDict()
for t in TDESC:
    if t in by: ordered[t] = by[t]
for t in by:
    if t not in ordered: ordered[t] = by[t]
by = ordered
tinfo = {}
for l in read_txt(os.path.join(G.SCRATCH, 'tables_1102.txt')).splitlines():
    p = [c.strip() for c in l.split('|')]
    if len(p) == 3 and p[0] not in ('tbl',) and not set(p[0]) <= set('-'): tinfo[p[0].upper()] = p[2]

d = new_doc('테이블 정의서', 'LGLS_MCS_IO (Microsoft SQL Server) - %d 테이블 · 전 컬럼 설명 · 트리거 · 인덱스 · 보존 정책' % len(by),
            history=[['1.0', '2026-09-03', AUTHOR, '최초 작성'],
                     ['1.1', '2026-09-04', AUTHOR, '검토 의견 반영'],
                     ['1.2', '2026-09-09', AUTHOR, '현행 스키마 재추출 반영'],
                     ['1.3', '2026-09-11', AUTHOR, '미사용 표 12개 정리 반영 (Rev4, 25표)'],
                     ['2.0', '2026-09-21', AUTHOR, '현장 기본 시험 완료 기준 (PLC 알람 리스트 SC_LGLS 반영)']])

d.add_heading('0. 이 문서는', 1)
para(d, '운전 DB LGLS_MCS_IO 에 지금 있는 표 %d 개를 하나도 빼지 않고 전부 싣는다. 문서에 없는 표는 DB 에도 없고, DB 에 있는 표는 모두 문서에 있다 '
        '(시험용 백업 표 *_BAK 는 제외). 앞 판 뒤에 더해지거나 바뀐 것은 아래와 같다.' % len(by))
table(d, ['구분', '대상', '왜'], [
    ['표 추가', 'DN_MST / DN_SEQ / UP_DOWN', 'Client 배포 런처(Upload/Download). 파일을 DB 에 올리면 각 PC 의 Ecs.exe 가 기동 때 받아 간다'],
    ['열 추가', 'CV_DATA.RET_READY_LATCH / RET_READY_LATCH_LUGG / RET_READY_LATCH_DT', '출고대 도착 신호가 3초 펄스라 사이클당 1회 샘플링으로 놓치던 것을 설비 통신이 래치해 두고 스케줄러가 소비'],
    ['동작 변경', 'DEL_HIS_SETTING', 'IO_TASK 가 이 표를 읽어 보존기간 지난 이력을 실제로 지운다(종전엔 아무도 지우지 않았다)'],
    ['동작 변경', 'WCS_LOG_PGR', '"[] -> [0000]" 트래킹 로그 억제(개발 DB 32.9만 건이 쌓이던 원인)'],
    ['표 삭제', 'CELL_MST / CELL_DTL', '이 현장은 셀(랙 위치) 관리를 쓰지 않는다(상위가 로케이션을 정해 내려 줌). 프로그램은 이 표 없이 돌도록 되어 있다. DB_BACKUP/20_CELL표_삭제.sql, 되돌리기 21_CELL표_복구.sql'],
], widths=[2.2, 6.3, 8.5])
doc_set_table(d, 3)
d.add_page_break()

d.add_heading('1. 테이블 목록', 1)
lst = []
for i, t in enumerate(by.keys(), 1):
    nm, desc = TDESC.get(t, ('', ''))
    lst.append([i, t, nm, len(by[t]), tinfo.get(t, ''), desc])
table(d, ['No', '테이블', '한글명', '컬럼수', '행수', '설명'], lst, widths=[0.9, 4.2, 3.2, 1.3, 1.8, 6.3], font=8)
note(d, '행수는 개발 PC 기준이며 현장과 다르다(이력 표는 보존기간이 지나면 IO_TASK 가 지운다).')
d.add_page_break()

d.add_heading('2. 테이블 정의', 1)
for t, cols in by.items():
    nm, desc = TDESC.get(t, ('', ''))
    d.add_heading('%s  %s' % (t, ('(' + nm + ')') if nm else ''), 2)
    if desc: para(d, desc, color=GRAY, size=9)
    body = []
    for c in cols:
        _, pos, col, typ, ln, nul, dflt, pk = c[:8]
        typ2 = typ + ('(%s)' % ln if ln and ln != '-1' else ('(max)' if ln == '-1' else ''))
        cu = col.upper()
        body.append([pos, cu, typ2, 'N' if nul == 'NO' else 'Y', pk, dflt.strip('()'),
                     CDESC.get(cu) or coldesc.describe(col)])
    table(d, ['No', '컬럼', '타입', 'Null', 'PK', '기본값', '설명'], body, widths=[0.9, 4.8, 3.0, 0.9, 0.9, 1.6, 5.4], font=8)
d.add_page_break()

d.add_heading('3. 트리거 · 뷰 · 인덱스', 1)
d.add_heading('3.1 트리거', 2)
table(d, ['이름', '표', '시점', '하는 일'], [
    ['trg_JOB_MST_StatusHis', 'JOB_MST', 'INSERT / UPDATE / DELETE',
     '작업이 생기거나 JOB_STATUS 가 바뀌거나 지워질 때 그 시점의 행을 JOB_MST_HIS 에 복사한다(상태별 1행). '
     '운전 화면 [작업 로그]와 사후 확인 SQL(261029_사후확인_작업번호.sql)이 이 이력으로 "RGV·크레인을 IO_TASK 가 지시했나(35/25 가 있나)" 를 가려낸다.'],
], widths=[4.0, 2.2, 3.2, 7.6], font=8.5)
d.add_heading('3.2 뷰', 2)
table(d, ['이름', '하는 일'], [
    ['dual', 'Oracle 호환용 1행 뷰. TASK 의 Oracle 계열 쿼리(SELECT ... FROM dual)가 그대로 돌게 한다.'],
], widths=[4.0, 13.0], font=8.5)
d.add_heading('3.3 인덱스 · 기본키', 2)
table(d, ['이름', '표', '열', '비고'], [
    ['PK_SC_DATA_LGLS', 'SC_DATA_LGLS', 'SC_NO', '크레인 5행'],
    ['PK_RTV_DATA_LGLS', 'RTV_DATA_LGLS', 'RTV_NO', 'RGV 1행'],
    ['IX_WCS_LOG_PGR_INS_DT', 'WCS_LOG_PGR', 'INS_DT, PGR_NM', '운전 화면 [ECS 프로그램 이력] 기간·프로그램 조회와 IO_TASK 보존기간 삭제(INS_DT < 기준) 모두 이 인덱스를 탄다'],
    ['DN_MST_PK', 'DN_MST', 'DN_NO', '배포 묶음'],
    ['UP_DOWN_PK', 'UP_DOWN', 'DN_NO, DN_LN', '배포 파일'],
    ['UP_DOWN_IX1', 'UP_DOWN', 'DN_NM, DN_VER', '런처가 이름별 최고 버전을 찾을 때'],
], widths=[4.5, 3.2, 3.3, 6.0], font=8.5)
note(d, '그 밖의 표(JOB_MST, CV_DATA, COMMON_CODE 등)는 구 ECS 때부터 명시적 PK 없이 쓰인다. 조회 조건은 WH_TYP + 번호 열(LUGG_NO / MC_NO / CDX_CD+CCD_CD)이다.')
d.add_page_break()

d.add_heading('4. 이력 보존 정책 (DEL_HIS_SETTING)', 1)
para(d, '운전 화면 ECS ▷ 환경설정 ▷ 로그 삭제 설정 에서 표별 보존 주기를 고친다. 스케줄러(IO_TASK)가 기동 2분 뒤, 그 뒤 1시간마다 이 표를 다시 읽어 '
        'key_column 이 "지금 - cycle" 보다 앞선 행을 지운다. 사이클(200ms)마다 표 하나에서 2,000행씩만 지우므로 스케줄링은 멈추지 않는다.')
table(d, ['table_name', 'cycle', 'unit', 'key_column', '이 DB', '비고'], [
    ['eqp_err_his', '30', 'DAY', 'ERROR_DT', '있음', '설비 에러 이력'],
    ['job_mst_his', '60', 'DAY', 'INS_DT', '있음', '작업 이력. 사후 확인이 필요하면 그 안에 조회해 보관'],
    ['wcs_log_pgr', '60', 'DAY', 'INS_DT', '있음', '프로그램 이력'],
    ['wcs_client_log', '60', 'DAY', 'INS_DT', '있음', '운전 화면 조작 로그'],
    ['host_if_log', '60', 'DAY', 'INS_DT', '있음', '상위 전문 로그'],
    ['cv_data_pb / bcr_his / rtv_data_pb / sc_data_murata_pb', '60', 'DAY', 'INS_DT', '없음', '타 현장 표. IO_TASK 가 건너뛰고 1회 알림만 낸다'],
    ['empty_ret_auto_del', 'Y/N', '-', '-', '-', '보존 주기가 아닌 옵션 행. 삭제 대상 아님'],
], widths=[5.0, 1.3, 1.3, 2.2, 1.3, 5.9], font=8.5)
bullets(d, [
    'unit 은 DAY / HOUR / MONTH, cycle 은 1 이상이어야 한다. 그 밖의 행은 무시된다.',
    'WH_TYP 열이 있는 표는 창고구분(10)으로 좁혀 지운다.',
    '끄기·조정 : ENV_IOSCH.INI [CNF] HIS_DEL_ENABLE(1) / HIS_DEL_INTERVAL_MIN(60) / HIS_DEL_BATCH(2000) / HIS_DEL_START_SEC(120). 재기동 불필요.',
    '지운 뒤 DB 파일 크기는 줄지 않는다(SQL Server 특성). 디스크를 되찾으려면 따로 DBCC SHRINKDATABASE.',
    '보존기간을 늘리려면(예 작업 이력 1년) job_mst_his 의 cycle 을 365 로. 지운 행은 되돌릴 수 없으니 줄일 때는 신중히.',
])

d.add_heading('5. 참고 - 상태 코드 (COMMON_CODE.JOB_STATUS)', 1)
table(d, ['코드', '뜻', '누가 세우나'], [
    ['99', '신규(상위 지시 수신)', 'HOST_TASK'], ['10 / 11', 'CV 지시 / 지시 전송', 'IO_TASK / EQP_TASK'],
    ['15', 'CV 구동중(입고대·출고대·통로 H/S)', 'IO_TASK'], ['16', '통로 CV 구동중(입고 통로, 옵션 IN_HS_STATUS)', 'IO_TASK / Client 쓰기'],
    ['30 / 31 / 35', 'RGV 지시 / 전송 / 반송중', 'IO_TASK / EQP_TASK'], ['39', 'RGV 완료', 'EQP_TASK'],
    ['20 / 21 / 25', 'S/C 지시 / 전송 / 반송중', 'IO_TASK / EQP_TASK'], ['29', 'S/C 완료(입고 = 작업 완료, 출고 = 1차 완료)', 'EQP_TASK'],
    ['19', 'CV 완료(출고대 도착 = 출고 작업 완료)', 'IO_TASK'], ['09', '완료 보고 끝 → 이력 이관·삭제', 'HOST_TASK'],
], widths=[2.2, 10.0, 4.8], font=8.5)
para(d, '입고 : 99 → 10 → 11 → 15 → 30 → 31 → 35 → 39 → 15(16) → 20 → 21 → 25 → 29 → 09', size=9)
para(d, '출고 : 20 → 21 → 25 → 29 → 15 → 30 → 31 → 35 → 39 → 15 → 19 → 09', size=9)

save_doc(d, '03_테이블_정의서.docx'); print('tables', len(by))
