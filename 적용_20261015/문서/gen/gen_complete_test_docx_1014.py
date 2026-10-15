# -*- coding: utf-8 -*-
"""작업정보 판넬 [완료처리] 시험 보고서 + 시험 안내서 (2026-10-14)
   캡처 : shots_1014/full_s<n>_before.png / full_s<n>_after.png (시뮬 자동 시험, EXE_NEWUI Client 전체 창)
   생성 : python gen_complete_test_docx_1014.py   →  2026-10-14_완료처리_시험보고서.docx / 2026-10-14_완료처리_시험안내서.docx
"""
import os, sys
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(r'D:\project\LGLS\Renewal\docs\산출물_20260927', 'gen'))
import gen_common as G
from docx.shared import Pt, Cm, RGBColor

G.DATE = '2026-10-14'
G.VER = '1.0'
G.FINAL_NOTE = '최초 작성 - 시뮬레이터 자동 시험 결과와 시험 안내'
SHOTS = os.path.join(HERE, 'shots_1014')

def shot(n):
    return os.path.join(SHOTS, n)

def read_log():
    p = os.path.join(SHOTS, 'stop_all.log')
    if not os.path.exists(p): return ''
    return open(p, encoding='utf-8', errors='replace').read()

# 구간 정의 : (번호, 이름, 시험정지 스위치, 멈춤 상태, 완료처리 동작, 기대 결과, 캡처 태그)
STEPS = [
    (1, 'S/C (크레인) 구간', '① 크레인 하역 Ack 유실 (1호기 출고)',
     '1호기가 출고 화물을 통로 H/S(104)에 내려놓은 뒤 하역완료 Ack 를 받지 못해 RUN 인 채 정지. 작업은 25(SC 구동중), 크레인 칸에 출고색과 작업번호가 남고, 104 에는 화물 감지만 있고 작업번호는 없음.',
     '작업을 고르고 [완료처리]. 단계표의 진행중 줄(S/C)이 대상. 확인창 2개 - "구간을 완료 처리하시겠습니까? [25 -> 29]" 와 "설비가 RUN 상태입니다. 화물을 이미 내려놓은 것이 확실합니까?"',
     '작업 25 → 29, 크레인에 물린 지시 해제(크레인 색·번호 사라짐). IO_TASK 가 104 의 화물에 작업번호를 찍고 16(통로 C/V 구동중)으로 넘김. 벨트는 크레인 핸드셰이크가 풀릴 때까지(지상반 리셋, 시뮬은 [고장 해제·수동 인수]) 기다린 뒤 103 으로 옮김.',
     's1'),
    (2, '통로 C/V 구간', '② 통로 C/V 번호 소실',
     '크레인 하역 뒤 통로 벨트가 화물을 RGV 픽업 칸(홀수)으로 옮겼는데 작업번호가 함께 오지 않음. 작업은 16(통로 C/V 구동중), 픽업 칸에 화물 감지만 있음. IO_TASK 는 픽업 칸의 번호를 보고 RGV 를 내므로 여기서 멈춤.',
     '작업을 고르고 단계표에서 통로 C/V 줄(SEQ 0002)을 고른 뒤 [완료처리]. 확인창 "[16 -> 15]" 다음에 "픽업 칸에 화물은 있는데 작업번호가 없습니다. 기록할까요?" 가 뜨면 [예].',
     '작업 16 → 15, 픽업 칸에 작업번호가 기록됨(지도의 그 칸에 번호가 나타남). IO_TASK 가 RGV 지시(30/35)를 냄.',
     's2'),
    (3, 'RGV 구간', '③ RGV 하역 완료 보고 유실',
     'RGV 가 출고 화물을 작업대 하역 칸에 내려놓았으나 완료 보고를 올리지 않음. 작업은 35(RGV 구동중), RTV 는 IDLE 이고 차상 데이터 없음, 하역 칸에는 화물 감지만 있음(번호는 ECS 가 써야 함).',
     '작업을 고르고 [완료처리]. 진행중 줄(RGV)이 대상. 확인창 "[35 -> 39]".',
     '작업 35 → 39, RTV 에 물린 지시·완료신호 해제, 통로 트랙 = RGV 도착 칸. IO_TASK 가 도착 칸의 화물에 작업번호를 찍고 15 로 넘기면 벨트가 출고대로 옮김.',
     's3'),
    (4, '작업대 C/V (출고대) 구간', '④ 출고대 도착 번호 소실 (신호 없음)',
     '화물이 출고대에 들어섰는데 작업번호가 함께 오지 않음. 번호가 없어 출고대 도착 신호도 서지 않고 IO_TASK 의 실도착 판정(출고대에 그 작업번호의 화물)도 되지 않아 작업은 15(CV 구동중)로 남음. 지게차도 번호 없는 화물은 빼 가지 않음.',
     '작업을 고르고 [완료처리]. 진행중 줄(작업대 C/V)이 대상. 확인창 "[15 -> 19]" 다음에 "출고대에 화물은 있는데 작업번호가 없습니다. 기록할까요?" 가 뜨면 [예].',
     '작업 15 → 19 → HOST_TASK 가 완료(F) 보고 → 09 → 목록에서 사라짐. 출고대 화물에 번호가 기록되어 지게차 반출.',
     's4'),
]

def build_report():
    d = G.new_doc('작업정보 판넬 [완료처리]\n시뮬레이터 시험 보고서', '4 구간 설비 정지 → 완료처리 → 결과 (캡처 8장)', landscape=False,
                  history=[])
    d.add_heading('1. 목적', 1)
    G.para(d, '구 ECS 의 스텝(SEQ) 단위 완료처리를 신 Client 작업정보 판넬에 옮긴 뒤, 설비가 구간마다 멈췄을 때 '
              '[완료처리] 한 번으로 다음 구간이 이어지는지 시뮬레이터(EQP_SIM)에서 확인했다. '
              '네 구간은 1호기 출고(랙 → 통로 H/S → RGV → 출고대)를 기준으로 했다.')
    d.add_heading('2. 시험 환경', 1)
    G.table(d, ['구분', '내용'], [
        ['체인', 'EQP_SIM → WCS_TASK_CV → IO_TASK → HOST_SIM → HOST_TASK, Client(EXE_NEWUI, 빌드 2026-10-14)'],
        ['DB', 'LGLS_MCS_IO @ 127.0.0.1,1435 (시뮬)'],
        ['설비 정지', 'EQP_SIM 의 "완료처리 시험:" 체크박스 ①~④ (또는 Data/sim_cmd.txt 의 STOP n). ①은 상황 A(횟수 0)와 같고, ②③④는 다음 1건에만 걸리고 스스로 꺼진다'],
        ['시뮬 설정', '[TIMING] RGV_DROP_STAMP=0 (RGV 하역 칸에 번호를 얹지 않음 - 현장과 같게), HS_RECORD_LAG_MS=1500'],
        ['조작', '판넬의 작업 선택·[완료처리]·확인창 [예] 를 스크립트로 눌렀고(사람 손과 같은 메시지), 캡처는 창 단위 PrintWindow'],
    ], widths=[3.0, 13.0])
    d.add_heading('3. 구간별 결과', 1)
    log = read_log()
    for n, name, sw, stuck, act, expect, tag in STEPS:
        d.add_heading('3.%d %s' % (n, name), 2)
        G.table(d, ['항목', '내용'], [
            ['정지 스위치', sw], ['멈춤 상태', stuck], ['완료처리 조작', act], ['기대 결과', expect]], widths=[3.0, 13.0])
        G.para(d, '멈춤 상태 (완료처리 전)', bold=True)
        G.image(d, shot('cap_%s_before.png' % tag), width_cm=17.0, caption='캡처 %d-1. %s - 설비 정지 상태 (왼쪽 판넬 / 오른쪽 지도)' % (n, name))
        G.para(d, '완료처리 후', bold=True)
        G.image(d, shot('cap_%s_after.png' % tag), width_cm=17.0, caption='캡처 %d-2. %s - [완료처리] 뒤' % (n, name))
        # 로그 발췌
        seg = ''
        if log:
            import re
            m = re.search(r'(\d\d:\d\d:\d\d [^\n]*STOP %d[^\n]*\n(?:.*\n){0,12}?.*done)' % n, log)
            if m: seg = m.group(1)
        if seg:
            G.para(d, '자동 시험 로그 발췌', bold=True)
            for line in seg.splitlines():
                G.para(d, line.strip(), size=8, color=G.GRAY)
        d.add_page_break()
    d.add_heading('4. 시험하면서 고친 것', 1)
    G.bullets(d, [
        'EQP_SIM : 핸드셰이크가 막힌 화물도 H/S 에 화물 감지가 서게(현장과 같게). 종전엔 감지도 없어 완료처리 뒤 번호를 찍을 수 없었다.',
        'EQP_SIM : RGV 하역 때 시뮬이 번호를 얹지 않는 옵션 RGV_DROP_STAMP=0 (기본). 종전엔 IO_TASK 가 없어도 화물이 굴러갔다.',
        'Client 판넬 : C/V 구간을 넘긴 직후 도착 칸에 번호 없는 화물이 있으면 바로 기록을 묻는다(두 번 누르지 않게).',
        'Client 판넬 : 출고 15/16 의 진행중 줄을 통로/작업대로 구분, RGV 지시 뒤 통로 트랙이 도착 칸으로 바뀌어도 단계표가 흔들리지 않게.',
        'Client 판넬 : 확인창이 떠 있는 동안 목록이 갱신되면 Client 가 내려가던 것(행 참조 → 복사본).',
        'Client 속도 : 수집 스레드의 설비마다 50ms 고정 대기를 없애고 수집 주기 기본 300→200ms (한 바퀴 620ms → 320ms 실측), 판넬 갱신 2초 → 1초(JOB_PANEL_REFRESH_MS).',
    ])
    d.add_heading('5. 남는 한계', 1)
    G.bullets(d, [
        '①에서 완료처리 뒤 벨트가 움직이려면 크레인 핸드셰이크가 풀려야 한다(현장 = 지상반 리셋). 완료처리는 WCS 쪽 상태와 번호 기록까지만 한다.',
        '현장 PLC 가 작업자 수동 트랙 쓰기(번호 기록)를 받아 주는지는 현장에서 확인이 필요하다.',
        '시험은 시뮬레이터 기준이다. 현장 적용 뒤 같은 네 상황을 1건씩 확인하는 것을 권한다(시험 안내서 참조).',
    ])
    out = os.path.join(HERE, '2026-10-14_완료처리_시험보고서.docx')
    d.save(out); print(out)

def build_guide():
    d = G.new_doc('작업정보 판넬 [완료처리]\n시험 안내서', '시뮬레이터에서 네 구간을 손으로 재현하는 법', history=[])
    d.add_heading('1. 준비', 1)
    G.numbered(d, [
        '체인을 순서대로 올린다 : EQP_SIM → WCS_TASK_CV → IO_TASK → HOST_SIM → HOST_TASK. HOST_SIM 이 첫 상태보고를 받은 뒤 [시작].',
        'Client 는 EXE_NEWUI\\WCS_CLIENT\\Ecs.exe (또는 EcsMain.exe) 로 띄운다.',
        'EQP_SIM 입고대에 화물이 없으면 "CV#11 / 24 입고대 / CV#15 파렛트 투입" 단추로 넣는다. HOST_SIM 이 입고 뒤 출고를 자동으로 낸다.',
        'EQP_SIM 위쪽 "완료처리 시험:" 줄의 체크박스 ①~④ 가 설비를 멈추는 스위치다. ②③④ 는 다음 1건에만 걸리고 적용되면 스스로 꺼진다. ① 은 상황 A(횟수 0)와 같아 [고장 해제·수동 인수] 로 푼다.',
        '파일로도 할 수 있다 : SIM\\EQP_SIM\\bin\\Debug\\Data\\sim_cmd.txt 에 한 줄 "STOP 2" 처럼 쓰고 저장(1초 안에 읽고 지움, 결과는 sim_cmd.log). SPAWN 104 (번호 없는 화물 생성) / REMOVE 103 (화물 제거) / RELEASE (고장 해제) 도 된다.',
    ])
    d.add_heading('2. 구간별 절차', 1)
    for n, name, sw, stuck, act, expect, tag in STEPS:
        d.add_heading('2.%d %s' % (n, name), 2)
        G.table(d, ['순서', '할 일', '보이는 것'], [
            ['1', 'EQP_SIM 에서 "%s" 체크' % sw, 'EQP_SIM 상태줄에 "시험정지 %d ... 켬 (다음 1건)"' % n],
            ['2', '다음 1호기 출고가 그 구간에 올 때까지 기다린다 (HOST_SIM 자동 발행, 1~3분)', stuck],
            ['3', act, '확인창'],
            ['4', '결과 확인', expect],
            ['5', n == 1 and 'EQP_SIM [고장 해제·수동 인수] 를 눌러 크레인 핸드셰이크를 푼다 (현장의 지상반 리셋)' or '별도 조치 없음 (스위치는 스스로 꺼짐)', n == 1 and '벨트가 103 으로 옮기고 RGV 가 가져감' or '다음 구간 자동 진행'],
        ], widths=[1.2, 7.4, 7.4])
        G.image(d, shot('cap_%s_before.png' % tag), width_cm=16.0, caption='%s - 멈춤 상태 예' % name)
    d.add_heading('3. 확인하는 자리', 1)
    G.table(d, ['무엇', '어디'], [
        ['작업 상태', '판넬 목록의 상태 열 (코드 : 25 SC 구동중 / 29 SC 완료 / 16 통로 C/V / 15 CV 구동중 / 35 RGV 구동중 / 39 RGV 완료 / 19 CV 완료 / 09 완료)'],
        ['설비에 물린 지시', '크레인·RGV 칸의 색과 번호, 또는 설비 상태창'],
        ['트랙의 화물·번호', '지도의 칸 (감지 = 회색 테두리, 번호 = 칸 안의 숫자). DB 는 CV_DATA.SENSOR0_DATA_RD / LUGG_NO_RD'],
        ['누가 넘겼는지', '로그 화면(또는 wcs_client_log)의 "완료처리(스텝 …)" 줄'],
        ['IO_TASK 가 받아 준 흔적', 'IO_TASK 로그 "트래킹은 없었지만 … 착지로 봅니다", "작업번호 기록 요청"'],
    ], widths=[3.5, 12.5])
    d.add_heading('4. 주의', 1)
    G.bullets(d, [
        '한 번에 하나의 스위치만 켠다. ① 을 켠 채 두면 1호기 출고가 매번 멈춘다 - 시험이 끝나면 [고장 해제·수동 인수] 또는 체크 해제.',
        '시뮬을 다시 띄우면 입고대 화물이 사라지고 저장된 옛 화물이 통로에 남을 수 있다. 통로에 번호 없는 화물이 남으면 sim_cmd.txt 에 REMOVE <트랙> 으로 치운다.',
        'HOST_SIM 로직은 자기가 낸 작업의 완료를 기다리므로, 작업을 DB 에서 지웠으면 HOST_SIM [종료] → [시작].',
        '[완료처리]는 DB(작업 상태·설비 지시·트랙 번호 기록 요청)만 바꾼다. 설비 자체의 에러·방향·핸드셰이크는 현장 조작반(시뮬은 해제 단추)으로 푼다.',
    ])
    out = os.path.join(HERE, '2026-10-14_완료처리_시험안내서.docx')
    d.save(out); print(out)

if __name__ == '__main__':
    build_report(); build_guide()
