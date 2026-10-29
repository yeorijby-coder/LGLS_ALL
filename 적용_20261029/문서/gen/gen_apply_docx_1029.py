# -*- coding: utf-8 -*-
u"""2026-10-27 적용 안내서 - ★추가분만★ (Word).

   현장 보고(2026-10-27) : 작업 2161(입고) 화물이 입고 통로 PORT:12 까지 가 크레인에 실렸는데
   상태는 15, 단계표는 첫 줄 진행중. [완료처리] 를 누르면 "PORT:23 에 화물이 감지되지 않아 ..." 로만
   끝나 다음으로 넘길 수 없었다.

   앞 안내서(10-15 꾸러미)는 그대로 두고, 이 건만 담는다.
   실행 : python gen_apply_docx_1029.py
"""
import os, sys

HERE = os.path.dirname(os.path.abspath(__file__))        # 적용_20261029/문서/gen
DOCS = os.path.dirname(HERE)                             # 적용_20261029/문서
ROOT = os.path.dirname(os.path.dirname(DOCS))            # Renewal
sys.path.insert(0, os.path.join(ROOT, 'docs', '산출물_20260927', 'gen'))

from docx.shared import Pt, Cm, RGBColor
import gen_common as G

RED = RGBColor(0xB0, 0x2A, 0x20)
SHOT = os.path.join(DOCS, 'shots_1029')


def code(d, lines, size=9):
    t = d.add_table(rows=1, cols=1)
    t.style = 'Table Grid'
    c = t.rows[0].cells[0]
    G.shade(c, 'F4F5F7')
    c.text = ''
    for i, ln in enumerate(lines):
        p = c.paragraphs[0] if i == 0 else c.add_paragraph()
        p.paragraph_format.space_after = Pt(0)
        r = p.add_run(ln)
        r.font.name = 'D2Coding'
        r.font.size = Pt(size)
    d.add_paragraph()


def note(d, text):
    p = d.add_paragraph()
    p.paragraph_format.left_indent = Cm(0.4)
    r = p.add_run('※ ' + text)
    r.font.size = Pt(9.5)
    r.font.color.rgb = RED
    return p


G.DATE = '2026-10-29'
G.VER = '1.0'
G.BASE_HISTORY = []
G.FINAL_NOTE = ('2026-10-15 꾸러미에 이어지는 추가분. 입고 단계표가 통로 H/S 를 알아보고, [완료처리] 가 화물이 있는 칸을 스스로 찾아 그 단계로 넘긴다.')

d = G.new_doc('2026-10-29 적용 안내서 (추가분)',
              '입고 단계표 표시 · [완료처리] 경로 탐색 (현장 작업 2161, 2026-10-27)')

d.add_heading('0. 이 문서는 무엇인가', 1)
G.para(d, '2026-10-15 꾸러미(적용_20261015)를 이미 받아 보셨다면, 그 뒤에 더해진 것만 여기에 담았다. '
          '바뀐 것은 Client(EcsMain.exe) 와 IO_TASK(이력 자동 삭제, 6절) 두 가지이고 DB 는 손대지 않았다. 파일은 적용_20261029 폴더에 있다.')
note(d, '앞 문서들(09-29 ~ 10-15)의 내용은 그대로 유효하다. 이 문서는 그 위에 얹는 것이다.')

d.add_heading('1. 현장에서 생긴 일 (2026-10-27, 작업 2161 입고)', 1)
G.para(d, '화물은 입고 통로 PORT:12 까지 가서 크레인에 실렸는데, 작업 상태는 [15] 그대로이고 단계표는 첫 줄(0001 CONVEYOR:12 PORT:24→PORT:23)이 '
          '진행중이었다. 그 자리에서 [완료처리] 를 누르면 "작업 상태가 이미 이 구간의 완료 코드입니다. [15] 그런데 PORT:23 에 화물이 감지되지 않아 '
          '작업번호를 기록할 수 없습니다. 화물이 그 칸에 오면 다시 누르세요." 로 끝나, 다음으로 넘길 길이 없었다.')
d.add_heading('1.1 왜 그랬나', 2)
G.table(d, ['구분', '내용'], [
    ['[완료처리] 의 기준', '고른 단계표 줄(안 고르면 진행중 줄 = 첫 줄)의 ★도착 칸★ 하나만 본다. 그 칸에 화물이 없으면 "없다" 로 끝낸다.'],
    ['실제 화물 위치', '두 단계 뒤(0003 줄의 도착 칸 PORT:12). 첫 줄 기준 PORT:23 에는 당연히 없다.'],
    ['단계표 표시', '입고 작업은 상태 15 를 두 번 지난다(입고대 CV 이동 중 / RGV 가 통로 H/S 에 내린 뒤). Client 는 둘 다 "Step 1 진행중" 으로 그려, 화물이 통로 H/S 에 내려와 있어도 첫 줄에 머문 것으로 보였다. 출고는 10-14 에 구분했지만 입고는 빠져 있었다.'],
    ['빠져 있던 것', '"그럼 어느 줄을 골라야 하나" 를 화면이 알려 주지 않았다. 10-15 시험안내서의 네 상황에는 "화물이 이미 뒤 칸에 가 있는" 경우가 없었다.'],
], widths=[3.5, 12.5])
note(d, '현장 확인 결과 작업자가 급한 출고 때문에 크레인을 수동으로 조작해 화물을 H/S 로 옮기고 H/S 데이터를 지웠다. 크레인에 지시(데이터)가 없던 것은 그래서 정상이며, 이후 H/S 에 작업번호만 기록하자 IO_TASK 가 이어서 입고했다(DriveSC 는 Step 이 아니라 "상태 15/16 + 픽업 트랙의 화물 + 같은 작업번호 + HS 신호" 로 지시한다). 이 안내서는 그 뒤에 담당자가 작업을 이어 가기 쉽게 하는 수정이다.')

d.add_heading('2. 고친 것 (1) 입고 단계표가 통로 H/S 를 알아본다', 1)
G.para(d, '입고 작업이 상태 15/16 이고 HS_TRACK_NO 가 통로 트랙(103~120)이면 RGV 는 끝난 것이므로 ★통로 C/V 줄(Step 3)★ 을 진행중으로 그린다. '
          'HS_TRACK_NO 가 비어 있으면(아직 RGV 지시 전) 종전처럼 Step 1 이다. 출고에 있던 구분(10-14)을 입고에도 같은 방식으로 넣었다.')
G.table(d, ['JOB_STATUS', 'HS_TRACK_NO', '진행중 줄 (종전 → 지금)'], [
    ['15', '없음 (RGV 지시 전)', 'Step 1 → Step 1 (같음)'],
    ['15', '통로 트랙 (RGV 가 내린 뒤)', 'Step 1 → ★Step 3 통로 C/V★'],
    ['16', '통로 트랙', 'Step 1 → ★Step 3 통로 C/V★'],
    ['30 / 31 / 35 / 39', '-', 'Step 2 RGV (같음)'],
], widths=[3.5, 5.5, 7.0])
G.image(d, os.path.join(SHOT, 'panel_phase_inbound_aisle.png'), 13.0,
        '시험(개발 PC) : 위 = 15 + 통로 107 → Step 3 진행중 / 가운데 = 15 + 통로 없음 → Step 1 / 아래 = 16 + 통로 107 → Step 3')
note(d, '통로 C/V 가 없는 라인(통로 칸이 하나뿐)이면 같은 조건에서 S/C 줄이 진행중이 된다. 현장 PC 의 화면 설정(Windows 배율 등)과 무관하다.')

d.add_heading('3. 고친 것 (2) [완료처리] 가 화물이 있는 칸을 찾는다', 1)
G.para(d, '고른 줄의 도착 칸에 화물이 없으면, 그 작업 경로의 ★다른 칸들(단계표의 모든 도착 칸)★ 을 CV_DATA 에서 찾아본다. '
          '찾으면 그 단계로 넘기자고 묻고, [예] 한 번으로 상태 이동과 작업번호 기록까지 끝낸다. 어디에도 없으면 무엇을 해야 하는지 알려 준다.')
G.table(d, ['상황', '뜨는 창', '[예] 를 누르면'], [
    ['다른 칸에 내 작업번호가 붙어 있다, 또는 번호 없는 화물(센서만 ON)이 있다',
     '"대신 PORT:8 (단계 0003 CONVEYOR:4) 에 화물이 감지됩니다(작업번호 없음). 그 단계로 넘기고 PORT:8 에 작업번호 0758 를 기록할까요?"',
     '그 단계의 완료 코드로 JOB_STATUS 이동(예: 15→16) + 그 칸에 작업번호 기록(CV_DATA LUGG_NO_OD, TRACKING_WRITE_YN=Y). ★한 번만 묻는다★ (종전의 "기록할까요?" 를 다시 묻지 않는다).'],
    ['경로의 어느 칸에도 없다',
     '"경로의 어느 칸(PORT:31, PORT:7, PORT:8)에도 화물이 감지되지 않습니다. · 설비(크레인/RGV)에 실린 채라면 : PORT:8 에 내린 뒤 단계 0003 줄을 골라 [완료처리] · 이미 마지막까지(랙/작업대) 옮겨졌다면 : 단계 0004 (S/C 2) 줄을 골라 [완료처리]"',
     '(안내만, 아무것도 바꾸지 않는다)'],
    ['고른 줄의 도착 칸에 있다', '종전과 같다 ("작업번호를 기록할까요?")', '종전과 같다'],
], widths=[4.0, 7.5, 4.5], font=8.5)
G.para(d, '찾는 순서 : 내 작업번호가 붙은 칸 > 센서만 ON 인 빈 번호 칸. 여러 칸이면 ★뒤 단계★ 를 고른다(화물은 앞으로만 가므로). '
          '다른 작업의 번호가 붙은 칸은 세지 않는다. [아니오] 를 누르면 아무것도 바꾸지 않는다.')
G.image(d, os.path.join(SHOT, 'panel_seq_0758.png'), 13.0,
        '시험 : 0758(입고 130→902) 상태 15, 첫 줄 진행중. 화물은 PORT:31 이 아니라 PORT:8 에 있다 (CV_DATA 108 센서 ON, 번호 없음)')
G.image(d, os.path.join(SHOT, 'popup_route_found.png'), 11.0, '[완료처리] → 찾은 칸으로 넘기자고 묻는다')
G.image(d, os.path.join(SHOT, 'popup_route_none.png'), 11.0, '어느 칸에도 없을 때 - 무엇을 할지 알려 준다')
d.add_heading('3.1 확인한 것 (개발 PC, 2026-10-27)', 2)
G.table(d, ['확인', '결과'], [
    ['찾은 칸으로 넘기기', '[예] 한 번에 JOB_MST 0758 : 15 → 16, HS_TRACK_NO=107 / CV_DATA 108 : LUGG_NO_OD=0758, TRACKING_WRITE_YN=Y. 추가 창 없음.'],
    ['어느 칸에도 없음', '안내 창만 뜨고 DB 는 그대로(15).'],
    ['[아니오]', '아무것도 바뀌지 않는다.'],
], widths=[4.0, 12.0])

d.add_heading('4. 같은 상황이 또 생기면 (현장 처리법)', 1)
G.para(d, '현장 2161 은 이미 끝났다. 같은 일이 다시 생겼을 때의 순서이다.')
G.table(d, ['Client', '하는 법'], [
    ['이 꾸러미(10-27)를 깐 뒤', '그 작업을 고르고 [완료처리] → "대신 PORT:12 (단계 0003 CONVEYOR:6) ..." 창에서 [예]. 화물이 아직 크레인 포크 위라면 2단 참조.'],
    ['아직 10-15 판이면', '단계표에서 ★0003 CONVEYOR:6 줄을 골라★ [완료처리] → 15→16 → "작업번호 기록할까요?" [예]. (첫 줄을 둔 채 누르면 종전처럼 "PORT:23 에 없다" 로 끝난다)'],
    ['화물이 크레인 포크 위에 있다', '먼저 화물을 PORT:12 에 내린다(크레인 수동). 그 뒤 위 순서. 포크 위에 둔 채로는 어느 판에서도 넘길 수 없다 - 포크 위 화물은 C/V 센서에 잡히지 않는다.'],
    ['이미 랙에 넣었다', '단계표 ★0004 S/C 3 줄을 골라★ [완료처리] → 29. 그 뒤 IO_TASK 가 HOST 보고까지 이어 간다. 랙 위치는 작업정보의 도착지(LOC)와 실제가 같은지 확인.'],
], widths=[4.0, 12.0])

d.add_heading('5. 끝난 작업을 사후에 확인하는 방법', 1)
G.para(d, '2161 처럼 끝나서 JOB_MST 에서 지워진 작업은 JOB_MST 로 볼 수 없다. 그래도 아래 자료가 60일(에러 이력은 30일) 남으므로, '
          '"RGV·크레인을 IO_TASK 가 지시했나, 사람이 움직였나" 를 사후에 가려낼 수 있다. 상세 순서와 읽는 법은 별도 문서 '
          '"2026-10-29_끝난작업_사후확인방법.docx" 에, 한 번에 돌리는 조회는 SQL\261029_사후확인_작업번호.sql 에 있다.')
G.table(d, ['자료', '무엇을 알 수 있나', '보존'], [
    ['JOB_MST_HIS', '작업 상태가 바뀐 순서·시각, 그때의 HS_TRACK_NO. 35 가 있으면 RGV 를, 25 가 있으면 크레인을 IO_TASK 가 지시한 것', '60일'],
    ['WCS_LOG_PGR', '트랙별 작업번호 이동([0000]->[번호]), 설비가 받은 반송 지시, IO_TASK 보류 사유', '60일'],
    ['WCS_CLIENT_LOG', '운전자가 Client 에서 한 조작(완료처리·쓰기 등)과 PC IP', '60일'],
    ['HOST_IF_LOG / EQP_ERR_HIS', '상위 전문 / 그 시간대 설비 에러', '60일 / 30일'],
], widths=[3.5, 10.0, 2.5])

d.add_heading('6. 고친 것 (3) IO_TASK 가 보존기간 지난 이력을 지운다 (2026-10-29)', 1)
G.para(d, '설정 화면 [로그 삭제 설정] 이 보여 주는 DEL_HIS_SETTING(JOB_MST_HIS·WCS_LOG_PGR·WCS_CLIENT_LOG·HOST_IF_LOG 60일, EQP_ERR_HIS 30일)은 '
          '지금까지 값을 보여 주고 고치기만 했을 뿐, ★어느 프로그램도 읽어 지우지 않았다★ (구 ECS/ECP 에도 없고, SQL Express 라 Agent 잡도 둘 수 없다). '
          '5절의 "60일 남는다" 는 말은 그래서 "60일 뒤에도 지워지지 않고 계속 쌓인다" 가 실상이었다. 개발 PC 에는 9월 2일부터 WCS_LOG_PGR 133만 건, '
          'JOB_MST_HIS 28만 건이 그대로 있었다.')
G.table(d, ['항목', '내용'], [
    ['누가', 'IO_TASK 스케줄러 스레드. 기동 2분 뒤 첫 검사, 그 뒤 1시간마다.'],
    ['무엇을', 'DEL_HIS_SETTING 의 unit 이 DAY/HOUR/MONTH 이고 cycle 이 1 이상인 표. key_column(INS_DT / ERROR_DT) 이 "지금 - cycle" 보다 앞선 행을 지운다. '
              'WH_TYP 열이 있는 표는 창고구분(10)으로 좁힌다. empty_ret_auto_del(Y/N) 같은 행과, 이 DB 에 없는 표(cv_data_pb·bcr_his·rtv_data_pb·sc_data_murata_pb)는 건너뛴다(한 번만 알림).'],
    ['어떻게', '★사이클(200ms)마다 표 하나에서 2,000행씩★ 지우고 다음 사이클로 넘긴다. 긴 DELETE 하나로 스케줄링을 세우지 않기 위해서다. 적체가 남으면 이어서 지운다.'],
    ['설정 반영', '검사 때마다 DEL_HIS_SETTING 을 다시 읽는다. Client 설정 화면에서 보존기간을 바꾸면 다음 검사(최대 1시간)부터 반영. 재기동 불필요.'],
    ['확인', 'IO_TASK 화면 로그 구분 [HIS] : "eqp_err_his : 30 DAY 지난 50건 삭제", "이력 정리 끝 (2초) - 다음 검사 12:34". 지운 게 없는 표는 줄을 내지 않는다.'],
    ['끄기 / 조정', 'ENV_IOSCH.INI [CNF] HIS_DEL_ENABLE=0 (끔) / HIS_DEL_INTERVAL_MIN(기본 60) / HIS_DEL_BATCH(기본 2000) / HIS_DEL_START_SEC(기본 120). 안 적으면 기본값. 재기동 불필요.'],
], widths=[2.5, 13.5], font=9)
G.image(d, os.path.join(SHOT, 'iotask_his_purge.png'), 15.0,
        '시험(개발 PC, 2026-10-29) : 없는 표 4개 건너뜀(1회 알림) → eqp_err_his 30일 지난 50건 삭제 → 이력 정리 끝. '
        '이어서 wcs_log_pgr 보존을 55일로 잠시 낮춰 172,451건이 25초 만에 2,000행씩 나뉘어 지워지는 것을 확인했다(그 사이 스케줄러 사이클 정상).')
note(d, '처음 적용하는 현장은 쌓인 양이 많을 수 있다. 2,000행/0.2초 = 분당 약 60만 행이므로 수백만 건이라도 몇 분 안에 끝나며, 그동안 스케줄링은 그대로 돈다. '
        '지운 뒤의 DB 파일 크기는 줄지 않는다(SQL Server 특성). 디스크를 되찾으려면 따로 DBCC SHRINKDATABASE 를 하면 되지만 필수는 아니다.')
note(d, '보존기간을 늘리고 싶으면(예 : 작업 이력 1년) Client [로그 삭제 설정] 에서 job_mst_his 의 주기를 365 로 바꾸면 된다. 지운 행은 되돌릴 수 없으니 줄일 때는 신중히.')

d.add_heading('6.1 덤 - WCS_TASK_CV 의 "[] -> [0000]" 트래킹 로그를 없앴다', 2)
G.para(d, 'WCS_LOG_PGR 에 "트랙 nnn R트래킹 JOB NO 변경 [] -> [0000]" 이 쉬지 않고 쌓였다(개발 DB 32.9만 건 = 전체의 1/4). 캐시가 비어 있던 자리가 '
          '0000 으로 채워지는 것일 뿐 작업번호의 변화가 아니다. 빈 값·0·0000 을 전부 "없음" 으로 같게 보고, ★번호가 실제로 생기거나 사라질 때만★ 남기도록 '
          'WCS_TASK_CV(EQP_TASK)를 고쳤다. 시험 : 시뮬 15설비 연결 70초 동안 새로 한 줄도 생기지 않음. 6절의 삭제와 합쳐 로그 표가 더 작게 유지된다.')

d.add_heading('7. 바뀐 파일 / 적용 순서', 1)
G.table(d, ['폴더', '파일', '비고'], [
    ['Client', 'EcsMain.exe, DciLib.dll, EcsLib.dll, XmlLib.dll', '★넷을 같이★ 복사 (빌드 2026-10-28 10:20). 10-15 의 Ecs.exe(런처)·설정·EcsDefine.xml 은 그대로'],
    ['TASK\\IO_TASK', 'IO_TASK_SEMI_FINISH.exe', '빌드 2026-10-29 11:28. 이력 자동 삭제(6절). 추가할_설정_ENV_IOSCH.ini.txt 는 값을 바꿀 때만'],
    ['TASK\\EQP_TASK', 'WCS_TASK_CV.exe', '빌드 2026-10-29 11:43. "[] -> [0000]" 트래킹 로그 억제(6.1절)'],
    ['소스_변경분', 'PanelJobDlg.cpp / .h + .diff, TASK\\cThread_SCH.cs · CvThread.cs + .diff', '입고 단계표 통로 구분, RouteSearchOffer, DoCompleteStep 분리 / PurgeHistory / IsNoJobNo'],
    ['SQL', '261029_사후확인_작업번호.sql', '읽기 전용 조회 (DB 변경 없음)'],
    ['문서', '이 문서, 2026-10-29_끝난작업_사후확인방법.docx, shots_1029', ''],
], widths=[3.0, 7.0, 6.0])
G.numbered(d, [
    'Client 를 닫는다.',
    'Client\\ 의 네 파일을 깔린 폴더(예 C:\\LGLS\\CLIENT)에 덮어쓴다. ini 는 손대지 않는다 (새 설정 없음).',
    '다시 띄워 제목줄 [Build:2026.10.28 10:20:16] 을 확인한다.',
    'IO_TASK 를 끝내고(ALL_TASK 면 [IO] 정지) TASK\\IO_TASK\\IO_TASK_SEMI_FINISH.exe 를 덮어쓴 뒤 다시 기동한다. ini 는 손대지 않아도 된다. '
    '2분 뒤 화면 로그에 [HIS] 줄이 나오면 된다.',
    'WCS_TASK_CV 도 같은 식으로 끝내고(ALL_TASK 면 [EQP] 정지) TASK\\EQP_TASK\\WCS_TASK_CV.exe 를 덮어쓴 뒤 다시 기동한다.',
    '같은 상황이 오면 4절대로 처리한다.',
])
note(d, 'Upload/Download 를 쓰고 있다면 Upload 로 EcsMain.exe 와 DLL 3개를 올려 각 PC 의 런처가 받게 해도 된다 (2026-10-15_Upload_사용매뉴얼).')

out = os.path.join(DOCS, '2026-10-29_적용안내서_추가분.docx')
d.save(out)
print(out)
