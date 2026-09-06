# -*- coding: utf-8 -*-
"""WCS 운전 지침서 - 2026-09-04 판"""
import os
from gen_common import *
from gen_screens import SCREENS

d = new_doc('WCS 운전 지침서', '운전 화면 · 서버 프로그램 사용 안내')
d.add_heading('목 차', 1)
groups = []
for g, *_ in SCREENS:
    if g not in groups: groups.append(g)
for gi, g in enumerate(groups, 1):
    para(d, '%d. %s' % (gi, g), bold=True)
    for si, s in enumerate([x for x in SCREENS if x[0] == g], 1):
        para(d, '   %d.%d %s' % (gi, si, s[1]), size=9)
d.add_page_break()

d.add_heading('0. 시스템 개요', 1)
para(d, '본 시스템은 구 ECS 를 대체하는 WCS 로, 다음 프로그램으로 구성된다.')
table(d, ['프로그램', '역할', '실행 파일'], [
    ['운전 화면(Client)', '창고 모니터링, 수동 지시, 로그 조회, 설정', 'WCS_CLIENT\\Ecs.exe'],
    ['설비 통신(EQP)', '마스터 PLC(XGT) 통신, 설비 데이터 갱신/지시 전송', 'TASK\\WCS_TASK_CV\\WCS_TASK_CV.exe'],
    ['스케줄러(SCH)', '작업 상태 진행, 설비 지시 발행, 완료 판정', 'TASK\\IO_TASK\\IO_TASK_SEMI_FINISH.exe'],
    ['상위 통신(HOST)', 'WMS/IMS 전문 송수신(O/R/M/S/E/F)', 'TASK\\WCS_TASK_HOST\\TASK_LFC10_G1_ECSCOM.exe'],
    ['DB', 'LGLS_MCS_IO (MS-SQL)', '-']], widths=[3.5, 8, 5.5])
para(d, '기동 순서 : 설비 통신 → 스케줄러 → 상위 통신. 운전 화면은 아무 때나.')
para(d, '작업 상태 코드 : 99 신규 → 10 CV 구동대기 → 15 CV 구동중 → 30/35 RGV → 39 RGV 완료 → 15 → 20/25 SC → 29 SC 완료 → 09 완료. 출고는 SC(20) 부터 시작해 CV 완료(19)로 끝난다.')
d.add_page_break()

gi = 0; cur = None
for g, name, sh, path, overview, notes, ctrls in SCREENS:
    if g != cur:
        gi += 1; cur = g; si = 0; d.add_heading('%d. %s' % (gi, g), 1)
    si += 1
    d.add_heading('%d.%d %s' % (gi, si, name), 2)
    para(d, '메뉴 PATH', bold=True); para(d, path, indent=0.5)
    para(d, '프로그램 개요', bold=True); para(d, overview, indent=0.5)
    para(d, '화면', bold=True)
    if sh: image(d, shot(sh), width_cm=15.5 if 'main' not in sh else 17.0)
    else: d.add_paragraph()
    if notes:
        para(d, '※ 특기사항', bold=True); bullets(d, notes)
    if ctrls:
        para(d, '※ 화면설명', bold=True)
        table(d, ['항목', '설명'], [[a, b] for a, b in ctrls], widths=[4.5, 12.5])
    d.add_paragraph()

d.add_heading('%d. 운전 중 예외 처리' % (gi + 1), 1)
table(d, ['상황', '처리'], [
    ['작업이 같은 상태에 오래 머묾(정체 알림창)', '해당 설비 대화상자에서 상태 확인 → [지시 재전송] 또는 [지시 삭제] 후 작업정보에서 상태 조정/삭제'],
    ['RGV 가 지시를 받지 않음', 'RTV 상태창 [지시 삭제] (요청 플래그까지 비움) → 스케줄러가 다음 지시를 낸다'],
    ['수동 지시가 거부됨("진행 중인 지시가 있습니다")', '스케줄러 지시가 살아 있는 상태. 필요하면 설비 대화상자 [지시 삭제] 후 재시도'],
    ['이중입고(54)/공출고(58) 에러', '크레인 에러 해제 후 상위(WMS) 재지정을 기다린다(자동 재지정 흐름 구현됨)'],
    ['설비 신호 유실로 완료가 안 됨', '작업정보 [CV 도착보고]/[SC 완료보고] 또는 설비 대화상자 [강제완료]'],
    ['상태바 EQUIP/HOST/SCH 빨강', '해당 서버 프로그램이 죽었거나 DB 하트비트가 끊김 - 프로그램 재기동'],
    ['RGV/크레인이 화물을 든 채 목적지에서 내리지 못함\n(가이드 틀어짐 등 물리적 원인)',
     '★설비에서 사람이 푼다.★ WCS 에는 목적지를 바꾸거나 반송을 취소하는 수단이 없다(아래 참조). '
     '① 설비 알람을 확인한다 ② 조작반에서 물리적 원인을 제거한다(가이드 정렬 등) '
     '③ 알람을 해제하면 설비가 하역을 마치고 완료신호를 올린다 ④ 작업이 다음 단계로 진행되는지 작업정보에서 확인한다'],
], widths=[6, 11])

para(d, '※ 설비가 목적지에서 하역하지 못하는 경우', bold=True)
bullets(d, [
    'WCS 는 반송 중인 설비에 "다른 곳에 내려라"(대체 목적지)나 "이 반송을 포기해라"(취소)를 지시할 수 없다. '
    '설비 인터페이스에 해당 신호가 없기 때문이다 - 명령은 출발지/도착지/작업번호와 요청 스트로브뿐이다.',
    '구 ECS 도 같았다. 대체 목적지 처리는 프레임워크에 정의만 있고 실제 로직은 없었으며, '
    '구 EQP_TASK 는 일정 시간이 지나면 핸드셰이크를 리셋해 같은 자리에서 다시 시도하게 했을 뿐이다.',
    '따라서 물리적 원인은 반드시 현장에서 제거해야 한다. 원인을 없애면 설비가 하던 반송을 그대로 마치고, '
    '완료신호가 올라오면 WCS 가 정상적으로 다음 단계로 넘긴다.',
    '해당 설비가 에러 상태인 동안 스케줄러는 그 설비에 새 지시를 내지 않는다. '
    '다른 설비의 작업은 계속 진행되므로, 한 대가 멈춰도 전체가 서지는 않는다. '
    '다만 S/C #1 은 입고·출고가 C/V#2(트랙 103/104) 한 라인을 함께 쓰므로 그 라인이 막히면 1호기 작업이 함께 대기한다.',
])
out = os.path.join(OUT, '05_WCS_운전_지침서.docx'); d.save(out); print('saved', out)
