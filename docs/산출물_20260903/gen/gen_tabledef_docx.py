# -*- coding: utf-8 -*-
"""테이블 정의서 : LGLS_MCS_IO 스키마(schema.txt) 기반"""
import os, re
from collections import OrderedDict
from gen_common import *

# 테이블 설명 (사용 여부/역할)
TDESC = OrderedDict([
 ('JOB_MST', ('작업 마스터', '진행 중 입/출고 작업. 완료(09/19/29)되면 JOB_MST_HIS 로 이관되고 삭제된다. 스케줄러(IO_TASK)가 상태를 진행시킨다.')),
 ('JOB_MST_HIS', ('작업 이력', '작업 상태 변화 이력(상태별 1행). 운전 화면 [작업 로그] 조회 대상.')),
 ('CV_DATA', ('컨베이어 트랙 데이터', '트랙(101~132)별 PLC 읽기값(_RD)과 지시값(_OD). 설비 통신(WCS_TASK_CV)이 갱신, 스케줄러가 지시.')),
 ('SC_DATA_LGLS', ('스태커 크레인 데이터', '크레인 901~905 의 상태/지시. 현장 PLC 맵(LGLS) 기준 컬럼.')),
 ('RTV_DATA_LGLS', ('RGV 데이터', 'RGV 801 의 상태/지시. DEPART_TRACK/ARRIVE_TRACK 은 실제 트랙번호.')),
 ('SC_DATA', ('(구) 크레인 데이터', '구 ECS 호환용. 현재 운전에는 SC_DATA_LGLS 를 사용.')),
 ('RTV_DATA', ('(구) RGV 데이터', '구 ECS 호환용. 현재 운전에는 RTV_DATA_LGLS 를 사용.')),
 ('EQP_MST', ('설비/프로그램 마스터', 'PLC 접속 정보, TASK 하트비트(UPD_DT/CONNECTED_YN). 운전 화면 상태바 EQUIP/HOST/SCH 판정 근거.')),
 ('EQP_ECD_MST', ('설비 에러코드 마스터', '설비별 에러코드와 명칭.')),
 ('EQP_ERR_HIS', ('설비 에러 이력', '에러 발생/해제 이력. 운전 화면 [설비에러이력] 조회 대상.')),
 ('COMMON_CODE', ('공통 코드', 'CDX_CD(코드군)/CCD_CD(코드). 작업상태·작업구분·창고구분·콤보 항목 등. 다국어 명칭 컬럼 보유.')),
 ('CELL_MST', ('셀(랙 위치) 마스터', '크레인별 뱅크/베이/레벨 셀 정보와 재고 유무.')),
 ('CELL_DTL', ('셀 상세', '셀별 부가 정보(미사용 가능).')),
 ('CV_DEF_INF', ('컨베이어 정의', '트랙 간 이동 정의(From/Buffer/To).')),
 ('CV_FLOW_DEF', ('컨베이어 흐름 정의', '트랙 흐름 순서 정의.')),
 ('DEST_DEF', ('목적지 정의', '출발/도착 조합 정의.')),
 ('DEST_POS_DEF', ('목적지 위치 정의', '작업대·크레인 위치 정의.')),
 ('DEAD_LOCK_ZONE_DEF', ('교착 구간 정의', '동시 진입 금지 구간.')),
 ('SC_DEF_INF', ('크레인 정의', '크레인별 담당 뱅크 등.')),
 ('SC_HS_DEF', ('크레인 H/S 정의', '크레인별 핸드셰이크 트랙(구 정의, 스케줄러 일부 쿼리에서 참조).')),
 ('DEL_HIS_SETTING', ('이력 삭제 주기 설정', '로그 테이블별 보존 주기. 운전 화면 [로그 삭제 설정].')),
 ('HOST_IF_LOG', ('상위(WMS/IMS) 통신 로그', '송수신 전문 이력. 운전 화면 [HOST 로그].')),
 ('MES_IF_LOG', ('MES 통신 로그', '(미사용) 상위 통신 로그 대체 조회 대상.')),
 ('WCS_LOG_PGR', ('프로그램 이력', 'TASK 들이 남기는 운전 이력. 운전 화면 [알람]/[ECS 프로그램 이력].')),
 ('WCS_CLIENT_LOG', ('운전 화면 조작 로그', '사용자 조작(지시/삭제/설정 변경) 기록. [유저사용 로그].')),
 ('EVENT_LOG', ('이벤트 로그', '(예비)')),
 ('CLIENT_PGR_NM', ('화면 프로그램 명', '운전 화면 대화상자 이름/권한 키.')),
 ('CHG_LANG', ('다국어 문구', '화면 문구 번역표(예비).')),
 ('USER_MST', ('사용자', '운전 화면 로그인 사용자.')),
 ('USER_GRP', ('사용자 그룹', '권한 그룹.')),
 ('USER_GRP_WIN', ('그룹별 화면 권한', '그룹×화면 조회/수정/실행 권한(UPD_YN/EXE_YN 등).')),
 ('BCR_MST', ('바코드 리더 마스터', '(이 현장 미사용)')),
 ('WC_DATA', ('작업대 컨트롤러 데이터', '(이 현장 미사용)')),
 ('WC_HIS', ('작업대 이력', '(이 현장 미사용)')),
 ('WAIT_TRACK', ('대기 트랙', '(예비)')),
 ('HOST_EMPTY_PLT', ('공파렛트 작업', '상위 공PLT 작업 요청(화면 숨김 상태).')),
 ('AUTO_SC_WORK', ('자동 크레인 작업', '(예비)')),
 ('DUAL', ('더미', 'Oracle DUAL 호환.')),
])
# 컬럼 설명(핵심)
CDESC = {
 'WH_TYP': '창고 구분(10 = LG 생명과학 1동)', 'LUGG_NO': '작업(화물) 번호 4자리', 'JOB_TYP': '작업 구분 1=입고 2=출고 3=피킹출고 4=랙투랙 5=호기간이동 6=이동 1x=반자동',
 'JOB_STATUS': '작업 상태 99신규 10/11/15 CV 20/21/25 SC 30/31/35 RGV 09/19/29 완료', 'START_POS': '출발지(설비번호 : 트랙 1xx / 크레인 90x)', 'DEST_POS': '도착지(설비번호)',
 'START_LOCATION': '출발 셀(뱅크-베이-레벨)', 'DEST_LOCATION': '도착 셀(뱅크-베이-레벨)', 'PRODUCT_ID': '제품 정보', 'LOT_NO': '적재 용기(파렛트)',
 'JOB_PRIORITY': '우선순위(기본 100)', 'HS_TRACK_NO': 'RGV 인계 트랙(H/S)', 'SC_NO': '크레인 번호', 'FK_NO': '포크 번호', 'DEL_YN': '삭제 여부',
 'INS_DT': '생성 일시', 'UPD_DT': '갱신 일시', 'UPD_USER_ID': '갱신자', 'INS_USER_ID': '생성자', 'REMARKS': '비고/설명',
 'MC_NO': '설비(트랙) 번호', 'MC_NO_NM': '설비 명칭', 'TRACK_NO': '트랙 번호', 'PLC_NO': 'PLC(컨베이어 그룹) 번호', 'LUGG_NO_RD': '트래킹 작업번호(읽기)',
 'LUGG_NO_OD': '지시 작업번호', 'DEST_POS_OD': '지시 목적지', 'JOB_TYP_OD': '지시 작업구분', 'OD_RQ_YN': '지시 요청 여부(Y=미소비)', 'CMD_RQ_ID': '명령 코드(RESET/PAUSE/DELFK1 등)',
 'CMD_RQ_YN': '명령 요청 여부', 'SENSOR0_DATA_RD': '재하 감지', 'STOCK_MODE': '입출고 모드(0 입고, 1 출고)', 'TR_PAUSE_RD': '트랙 일시정지 상태', 'TR_PAUSE_OD': '트랙 일시정지 지시',
 'ERR_CODE_RD': '에러 코드', 'AUTO_MODE_RD': '자동 모드', 'ONLINE_MODE_RD': '온라인', 'ACTIVE_MODE_RD': '가동', 'UCSTATUS_RD': '상태(1 IDLE 2 RUN)', 'SUBSYSTEM_STATUS_RD': '서브시스템 상태(1 IDLE 2 RUN)',
 'PALLET_ID_OD': '지시 파렛트', 'FROM_01_OD': '출발 좌표1', 'TO_01_OD': '도착 좌표1', 'DEPART_TRACK': 'RGV 출발 트랙', 'ARRIVE_TRACK': 'RGV 도착 트랙', 'RTV_NO': 'RGV 번호(801)',
 'PALLET_ON_VEHICLE_RD': '차상 파렛트', 'TRANSFER_REQUEST_OD': '반송 요청', 'HOST_ERR_SEND_YN': '상위 에러 보고 여부', 'SUSPEND': '사용 중지(1 입고 2 출고 3 입출)',
 'EQP_TYP': '설비 종류(EQP/HOST/HOST2/SCH/CV/SC/RTV)', 'PLC_IP': 'PLC IP', 'PLC_PORT': 'PLC 포트', 'CONNECTED_YN': '접속 여부(하트비트)', 'USE_YN': '사용 여부',
 'CDX_CD': '코드군', 'CCD_CD': '코드', 'CCD_NM_KOR': '명칭(한글)', 'CCD_NM_ENG': '명칭(영문)', 'CCD_NM_CHIN': '명칭(중문)', 'CCD_EPR_ORD': '표시 순서', 'CCD_CD_YN': '사용 여부',
 'TABLE_NAME': '대상 테이블', 'CYCLE': '보존 주기', 'UNIT': '단위(DAY)', 'PGR_NM': '프로그램(스레드) 명', 'MSG': '메시지', 'LOG_MSG': '메시지',
 'USER_ID': '사용자 ID', 'USER_NM': '사용자명', 'USER_PW': '비밀번호', 'GRP_ID': '그룹 ID', 'WIN_ID': '화면 ID', 'UPD_YN': '수정 권한', 'EXE_YN': '실행 권한', 'INS_YN': '입력 권한', 'SEL_YN': '조회 권한',
 'BANK': '뱅크', 'BAY': '베이', 'LEVEL': '레벨', 'CELL_STATUS': '셀 상태', 'CELL_SC_NO': '담당 크레인',
}
rows = [l.split('|') for l in open(os.path.join(SCRATCH, 'schema.txt'), encoding='cp949', errors='replace').read().splitlines() if '|' in l and not l.startswith('TABLE_NAME') and not set(l.strip()) <= set('-|')]
by = OrderedDict()
for r in rows:
    if len(r) < 8: continue
    t = r[0].strip()
    by.setdefault(t.upper(), []).append([c.strip() for c in r])
tinfo = {}
for l in open(os.path.join(SCRATCH, 'tables.txt'), encoding='cp949', errors='replace').read().splitlines():
    p = [c.strip() for c in l.split('|')]
    if len(p) == 3 and p[0] not in ('tbl',) and not set(p[0]) <= set('-'): tinfo[p[0].upper()] = p[2]

d = new_doc('테이블 정의서', 'LGLS_MCS_IO (Microsoft SQL Server) - %d 테이블' % len(by))
d.add_heading('1. 테이블 목록', 1)
lst = []
for i, t in enumerate(by.keys(), 1):
    nm, desc = TDESC.get(t, ('', ''))
    lst.append([i, t, nm, len(by[t]), tinfo.get(t, ''), desc])
table(d, ['No', '테이블', '한글명', '컬럼수', '행수(현재)', '설명'], lst, widths=[0.9, 4.2, 3.2, 1.3, 1.6, 6.5], font=8)
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
        body.append([pos, col.upper(), typ2, 'N' if nul == 'NO' else 'Y', pk, dflt.strip('()'), CDESC.get(col.upper(), '')])
    table(d, ['No', '컬럼', '타입', 'Null', 'PK', '기본값', '설명'], body, widths=[0.9, 4.6, 3.0, 0.9, 0.9, 1.8, 5.6], font=8)
out = os.path.join(OUT, '03_테이블_정의서.docx'); d.save(out); print('saved', out, 'tables', len(by))
