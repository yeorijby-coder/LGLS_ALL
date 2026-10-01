# -*- coding: utf-8 -*-
u"""2026-10-01 설비 대화상자 표시 내용 비교 - 구 ECS vs 신 ECS (Word).

   사용자 지시 : "구 ECS 에 있는 설비 대화상자의 내용이 전부 현재 ECS 의 설비 대화상자 [확대] 를
                 누르면 그대로 표현되어야 해 (최대한 위치도 맞춰서). 그렇게 하고 난 다음에
                 설비 대화상자에 표현하는 내용을 구 ECS 와 신규 ECS 를 비교하는 문서를 WORD 로"

   구 ECS 쪽은 Backup/ECS 의 StackerForm / RGVForm / ConveyorForm 디자이너(좌표·컨트롤)와
   실행 화면(HECS.exe) 캡처를, 신 ECS 쪽은 이 PC 에서 띄운 Client 캡처를 근거로 한다.

   실행 : python gen_compare_dlg_1001.py
"""
import os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, os.path.join(ROOT, 'docs', '산출물_20260927', 'gen'))

from docx.shared import Pt, Cm, RGBColor
import gen_common as G

SHOT = os.path.join(HERE, 'shots_1001')
RED = RGBColor(0xB0, 0x2A, 0x20)


def note(d, text):
    p = d.add_paragraph()
    p.paragraph_format.left_indent = Cm(0.4)
    r = p.add_run('※ ' + text)
    r.font.size = Pt(9.5)
    r.font.color.rgb = RED


G.DATE = '2026-10-01'
G.VER = '1.0'
G.BASE_HISTORY = []
G.FINAL_NOTE = '구 ECS(HECS.exe) 설비 대화상자의 항목을 신 ECS 설비 상태창 [확대] 패널로 옮긴 결과를 항목별로 대조한 것.'

d = G.new_doc('설비 대화상자 표시 내용 비교 (구 ECS vs 신 ECS)',
              '스태커 크레인 · RGV · 컨베이어 - 2026-10-01', landscape=True)

d.add_heading('0. 한눈에', 1)
G.para(d, '구 ECS 는 설비를 더블클릭하면 StackerForm / RGVForm / ConveyorForm 이 뜬다. '
          '신 ECS 는 같은 더블클릭에 SC / RTV / CV 상태창이 뜨고, 그 안의 [확대] 를 누르면 오른쪽에 패널이 펼쳐진다. '
          '이번(2026-10-01)에 그 패널을 구 ECS 폼과 같은 배치(같은 좌표)로 다시 그렸다. '
          '즉 구 ECS 의 모든 항목이 신 ECS 에서는 ★[확대] 패널★ 에 그대로 있고, 왼쪽의 원래 상태창은 그대로 둔 채 덧붙인 것이다.')
G.table(d, ['구분', '구 ECS (HECS.exe)', '신 ECS (EcsMain.exe)'], [
    ['여는 법', '설비 더블클릭 → 폼', '설비 더블클릭 → 상태창 → [확대]'],
    ['크기', 'Stacker 505x349, RGV 505x349, Conveyor 226x485', '패널 폭 512(SC/RTV) · 240(CV) 를 오른쪽에 덧붙임'],
    ['항목 수', 'Stacker 14 LED + 4 위치 + 알람 + 7 작업항목 + 3 버튼 + 사용금지/확인', '같음 (+ PLC 주소 병기, + 완료 Ack [쓰기])'],
    ['[확대] 표시', '-', 'Ecs.ini [MENU] ZOOM_BTN=1 이어야 보인다 (0 이면 숨김)'],
    ['실 데이터', '구 ECS DB(ECS_DB)', 'LGLS_MCS_IO : SC_DATA_LGLS / RTV_DATA_LGLS / CV_DATA / JOB_MST'],
], widths=[3.0, 10.0, 11.0])
note(d, '이 문서의 신 ECS 캡처는 시뮬레이터(EQP_SIM) 와 이 PC 의 DB 로 띄운 것이라 값은 예시다. 배치만 보면 된다.')

# ── 1. 스태커 크레인 ──────────────────────────────────────────────
d.add_heading('1. 스태커 크레인 (StackerForm → SC 상태창 [확대])', 1)
G.image(d, os.path.join(SHOT, 'dlg_sc_1.png'), 22.0, '신 ECS - SC 상태창 [확대] (오른쪽 패널이 구 StackerForm 배치)')
G.para(d, '구 StackerForm 의 다섯 패널(panel2~5) 을 같은 y 좌표에 둔다. 왼쪽 상태창의 같은 항목(작업번호·출발/도착/현재/완료 위치 등)은 그대로 두었다.')
G.table(d, ['구 ECS 항목 (StackerForm)', '구 위치(x,y)', '신 ECS [확대] 패널', '값의 출처 (신)', '비고'], [
    ['상태 칸 (DOWN 빨강 / IDLE 노랑 / RUN 초록)', '왼쪽 위 (14,26) 52x36', '같은 자리, 같은 색 (DOWN 은 흰 글자)', 'SC_DATA_LGLS.UCSTATUS', ''],
    ['설비명(굵은 파랑) / 설명(파랑)', '가운데 2줄', '스태커 크레인 901 / S/C#1 (Bank 01,02) - 가운데 정렬, 파랑', 'EQP_MST.EQP_NAME / DEST_POS_DEF', ''],
    ['LED 11개 + Pallet ID : Load Complete / Load Complete ACK / Unload Complete / Unload Complete ACK, Transfer Request / Transfer Request ACK / Pallet Exist / Pallet ID, Alarm Set / Alarm Set ACK / Alarm Reset / Alarm Reset ACK', '4열 x 3행 (x 12/148/268/392, y 80/96/112)', '같은 4열x3행, 같은 영문 이름·순서', 'PlcAddressMap 신호 (M0310 …)', 'PLC 주소는 자리가 없어 맨 아래 한 줄'],
    ['완료 Ack 쓰기', '-', '아래 오른쪽 [Load ACK 쓰기] [Unload ACK 쓰기]', 'SC_DATA_LGLS.CMD_RQ_ID=ACKW…', '신 ECS 추가 (수동 Ack)'],
    ['현재위 / 출발지 / 도착지 / 완료위 (3칸씩)', 'panel3 라벨 x 7/112/210/311, 값 48/143/242/351', '같은 좌표, 3칸(열-단-번)', 'SC_DATA_LGLS 위치 워드', '주소 요약 한 줄 추가 (D0166 …)'],
    ['알람코드', 'panel3 (422,462)', '같은 자리', 'SC_DATA_LGLS.ERR_CODE', ''],
    ['요청번호 - 순번', 'panel4 (10,y)', '요청번호 = LUGG_NO, 순번 = JOB_STATUS', 'JOB_MST', '구 ECS 의 순번(SEQ) 은 신 ECS 에 없어 작업상태 코드로 대신'],
    ['배치번호', 'panel4', '배치번호 = LOT_NO', 'JOB_MST.LOT_NO', ''],
    ['자재코드', 'panel4', '자재코드', 'JOB_MST.ITEM_CD', ''],
    ['팔렛 + 입고/출고 tag', 'panel4 (283,70)', '팔렛 칸 + [입고]/[출고] 버튼 모양 tag', 'SC_DATA_LGLS.LUGG / JOB_TYP', ''],
    ['출발위치 / 도착위치', 'panel4', '같은 자리', 'JOB_MST.START_POS / DEST_POS', ''],
    ['조언(안내문)', 'panel4 (10,141)', '빨간 안내문 (예: "입/출고 요청번호가 DB에 없습니다. [이상종료]처리하세요!!")', '패널 로직', '구 ECS 문구를 그대로 썼다'],
    ['[명령 재전송] [이상종료]', '오른쪽 (385,150) 120x72 / (385,236) 120x50', '같은 자리, 같은 크기', 'CMD 재전송 / OD 클리어', '권한 CScSkinDlg UPD_YN. 사진에 [완료처리] 는 없어 넣지 않았다'],
    ['사용금지 체크', 'panel5 (25,24)', '같은 자리 - 체크하면 기존 [작업금지] 와 같은 동작', 'SC_DATA_LGLS.SUSPEND', ''],
    ['[확인]', 'panel5 (209,4) 86x40', '같은 자리 - 창 닫기', '-', ''],
], widths=[6.0, 4.2, 5.8, 4.5, 4.5], font=8)

# ── 2. RGV ────────────────────────────────────────────────────────
d.add_heading('2. RGV (RGVForm → RTV 상태창 [확대])', 1)
G.image(d, os.path.join(SHOT, 'dlg_rtv_1.png'), 22.0, '신 ECS - RTV 상태창 [확대]')
G.para(d, 'RGVForm 은 StackerForm 과 같은 틀이라 패널도 같은 배치다. 다른 점만 적는다.')
G.table(d, ['구 ECS 항목 (RGVForm)', '신 ECS [확대] 패널', '값의 출처 (신)', '비고'], [
    ['상태 칸 / 설비명(RGV 1호기) / RTV 801', '같음', 'RTV_DATA_LGLS.UCSTATUS / EQP_MST', ''],
    ['LED 12개 (적재/하역 완료·ACK, 반송요청·ACK, 화물감지, 차상화물, 알람 SET/RST·ACK)', '같은 4열x3행', 'PlcAddressMap (M03B0 …)', '완료 Ack [쓰기] 추가'],
    ['현재위 / 출발지 / 도착지 / 완료위', '같음', 'RTV_DATA_LGLS', '주소 요약 D0216/D0370/D0373/D0213/D0211'],
    ['요청번호-순번 / 배치번호 / 자재코드 / 팔렛(+tag) / 출발·도착위치 / 조언', '같음', 'JOB_MST / RTV_DATA_LGLS', '순번 = JOB_STATUS'],
    ['[명령 재전송] [이상종료]', '같음', '', '사진에 [완료처리] 없음'],
    ['사용금지 / [확인]', '같음 (사용금지 = 기존 [RTV 금지])', 'RTV_DATA_LGLS.SUSPEND', ''],
], widths=[8.0, 6.0, 6.0, 5.0], font=8)

# ── 3. 컨베이어 ───────────────────────────────────────────────────
d.add_heading('3. 컨베이어 (ConveyorForm → CV 상태창 [확대])', 1)
G.image(d, os.path.join(SHOT, 'hecs_dlg_0.png'), 5.5, '구 ECS - ConveyorForm (HECS.exe 실행 화면, CONVEYOR:2)')
G.image(d, os.path.join(SHOT, 'dlg_cv_1.png'), 18.0, '신 ECS - CV 상태창 [확대] (오른쪽 패널 폭 240)')
G.table(d, ['구 ECS 항목 (ConveyorForm 226x485)', '신 ECS [확대] 패널', '값의 출처 (신)', '비고'], [
    ['상태 칸 (IDLE 등) / 이름(CONVEYOR:11, 굵은 파랑) / 설명([KR00] 외부 전용 입/출고, 파랑)', '같은 자리 - 자동/수동 색 칸 + C/V#2(굵은 파랑) + 설명(파랑) 가운데 정렬', 'CV_DATA.AUTO_MODE / EQP_MST / EcsDefine', '사진([Conveyor 정보]) 배치'],
    ['"포트" / "Pallet" 머리글, 포트마다 [번호칸(큰 칸)] [팔렛 값·입력] [PalletID설정]', '같은 자리·크기 (사진 15~71 / 77~127 / 136~212), 최대 3줄 (C/V#2 는 103/104 두 줄)', 'CV_DATA(PLC_NO 같은 트랙) LUGG_NO / PALLET_EXIST', '[PalletID설정] = LUGG_NO_OD + TRACKING_WRITE_YN=Y'],
    ['색 규칙 : 감지+ID 하늘 / 감지만 진초록 / ID만 진홍 / 없음 흰', '같은 4색', 'PALLET_EXIST, LUGG_NO', ''],
    ['(없음 - 사진의 가운데 빈 자리)', 'LED 11개 (적재/하역 완료·ACK, 배출요청·ACK, 입고준비, 대기 IN/OUT, 자동모드, 화물감지) + 주소', 'PlcAddressMap', '신 ECS 항목을 사진의 빈 자리에 둠 (기능 유지)'],
    ['(없음)', '[적재ACK 쓰기] [하역ACK 쓰기] - LED 표 아래 한 줄', 'CV_DATA 명령', '폭 240 이라 주소 옆에 못 두고 아래로'],
    ['(없음)', '트래킹화물 R0010 / 방향모드 D0301', 'CV_DATA', ''],
    ['사용금지 체크 / [확인]', '같은 자리 (사진 (15,419) / (109,398) 95x45), 사용금지 = 기존 [일시정지]', 'CV_DATA.TR_PAUSE', ''],
], widths=[8.0, 7.0, 5.5, 4.5], font=8)
note(d, '구 ECS 의 크레인·RGV 폼은 HECS.exe 에서 자동으로 열리지 않아(더블클릭이 컨베이어 폼만 열림) 실행 캡처 대신 디자이너 좌표로 대조했다. '
        '사용자가 보내 준 사진과 Backup/ECS 의 StackerForm.Designer.cs 가 근거다.')

# ── 4. 하단 반송 판넬 ────────────────────────────────────────────
d.add_heading('4. (같이 한 것) 구 ECS 하단 반송 판넬 → 신 ECS 작업정보 판넬', 1)
G.image(d, os.path.join(SHOT, 'hecs_main2.png'), 20.0, '구 ECS 메인 - 아래쪽 [우선순위 | 작업목록 | 선택작업 상세(SEQ 표) + 완료처리]')
G.image(d, os.path.join(SHOT, 'panel_up.png'), 20.0, '신 ECS 메인 - 왼쪽 작업정보 판넬 (▲ 를 눌러 0873 의 우선순위가 002 로 바뀐 모습)')
G.table(d, ['구 ECS', '신 ECS 작업정보 판넬', '동작'], [
    ['우선순위 값 + ▲ ▼', '왼쪽 칸 : 값(큰 글씨) + [▲][▼]', 'JOB_MST.JOB_PRIORITY ±1 (확인창 → UPD_YN 권한 → CLIENT_LOG)'],
    ['[반송조정]', '왼쪽 칸 [반송조정]', '작업목록 [JOB_MST] 창을 연다 (리본 "작업정보" 와 같음)'],
    ['작업 목록 (ECS번호·구분·순위·자재·수량·작업·출발·도착)', '가운데 목록 (탭 필터 + 자동갱신)', '행을 고르면 오른쪽/아래 상세가 채워진다'],
    ['ECS번호 / 작업번호', '상세 칸 머리줄(한 줄) : 작업번호 = LUGG_NO, 팔렛 = LOT_NO (신 ECS 용어), 오른쪽 [완료처리]', '구 ECS 의 ECS번호(2026…)는 신 ECS 에 없는 개념'],
    ['SEQ 표 (SEQ·디바이스·시작·도착, 아이콘 = 완료/진행/대기) - CONVEYOR:11 PORT:22→PORT:21 / RGV / CONVEYOR:2 / S/C 1 → STKSEM', 'SEQ 표 (첫 열 = 완료/진행/대기) - 같은 꼴 : CONVEYOR:11 PORT:122→PORT:121 / RGV →PORT:103 / CONVEYOR:2 →PORT:104 / S/C 1 →LOC:04-001-01', 'JOB_STATUS 로 단계 위상 계산, 트랙→C/V 는 CV_DATA 에서'],
    ['[완료처리]', '[완료처리]', '출고류 19, 그 밖 29 (확인창 → 권한 → 로그)'],
    ['-', '판넬이 좁으면(<620px) 상세 칸이 ★아래★ 로 내려간다', '왼쪽 도킹 기본 폭(약 310) 에서 세 칸이 다 보이게'],
], widths=[7.0, 8.0, 10.0], font=8)

out = os.path.join(HERE, '2026-10-01_설비대화상자_비교_구ECS_vs_신ECS.docx')
d.save(out)
print('생성 :', out)
