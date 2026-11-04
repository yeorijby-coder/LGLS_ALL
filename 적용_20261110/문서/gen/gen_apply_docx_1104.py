# -*- coding: utf-8 -*-
u"""2026-11-04 적용 안내서 - ★추가분만★ (Word).

   크레인/RGV 상태창 [데이터 삭제] + 리본 MANUAL [CV 지우기] : PLC 담당자가 열어 주는 M 영역 비트 7개에 1 을 쓴다.
   앞 안내서(10-29 추가분)는 그대로 두고, 이 건만 담는다.
   실행 : python gen_apply_docx_1104.py
"""
import os, sys

HERE = os.path.dirname(os.path.abspath(__file__))        # 적용_20261110/문서/gen
DOCS = os.path.dirname(HERE)                             # 적용_20261110/문서
ROOT = os.path.dirname(os.path.dirname(DOCS))            # Renewal
sys.path.insert(0, os.path.join(ROOT, 'docs', '산출물_20260927', 'gen'))

from docx.shared import Pt, Cm, RGBColor
import gen_common as G

RED = RGBColor(0xB0, 0x2A, 0x20)
SHOT = os.path.join(DOCS, 'shots_1104')


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


G.DATE = '2026-11-04'
G.VER = '1.0'
G.BASE_HISTORY = []
G.FINAL_NOTE = ('2026-10-29 추가분에 이어지는 추가분. 지상반 [삭제] 와 같은 동작을 운전 화면에서 - 크레인/RGV 상태창 [데이터 삭제], 리본 [CV 지우기]. DB 변경 없음.')

d = G.new_doc('2026-11-04 적용 안내서 (추가분)',
              '크레인/RGV 상태창 [데이터 삭제] · 리본 MANUAL [CV 지우기] (PLC M 비트 7개)')

d.add_heading('0. 이 문서는 무엇인가', 1)
G.para(d, '적용_20261110 꾸러미(10-29 추가분)에 더해진 것만 담았다. 바뀐 것은 Client(EcsMain.exe + rc_resource 4개) 와 설비 통신(WCS_TASK_CV.exe) 두 가지이고, '
          '★DB 는 손대지 않았다★ (표·열 추가 없음 - 기존 명령 열 CMD_RQ_ID / CMD_RQ_YN 을 그대로 쓴다). 파일은 같은 적용_20261110 폴더에 있다.')
note(d, '앞 문서들(09-29 ~ 10-29)의 내용은 그대로 유효하다. 이 문서는 그 위에 얹는 것이다.')

d.add_heading('1. 배경 - 왜 필요한가', 1)
G.para(d, '크레인·RGV 가 비정상으로 멈춘 뒤 설비(PLC) 안에 지난 작업 데이터가 남으면, 화물을 옮겨 놓아도 설비가 새 지시를 받지 않는다. '
          '지금까지는 설비 지상반(터치)의 [삭제] 를 눌러야 했다(10-15 안내서 참고). PLC 담당자가 그 동작을 ECS 에서 하도록 M 영역 비트를 열어 준다.')
G.table(d, ['구분', '내용'], [
    ['PLC 가 주는 것', 'M 영역 비트 7개 = 크레인 1~5호기(SC1~SC5) + RGV(RTV1) 6개, 그리고 C/V 용 1개(CV)'],
    ['ECS 가 하는 것', '해당 비트에 ★1 만 쓴다★. 끄는 것은 PLC 다(1 을 보고 처리한 뒤 스스로 내린다). ECS 는 0 을 쓰지 않는다.'],
    ['SC/RTV 비트', 'PLC 가 그 설비의 작업 데이터를 지운다 = 지상반 [삭제] 와 같다'],
    ['CV 비트', 'PLC 가 ★모든 C/V 의 R 영역(트래킹 작업번호)★ 을 0 으로 지운다. R 트래킹은 ASCII 라 ECS 가 "0" 을 써도 0x30 이 남아 PLC 는 "값 있음" 으로 보기 때문에, 지우는 것은 PLC 몫이다.'],
    ['주소', '2026-11-04 현재 ★미확정★. 담당자가 확정하면 서버 WCS_DB.INI [HMI_DELETE] 에 적고 WCS_TASK_CV 를 재기동한다(5절). 그 전까지 버튼은 이력만 남기고 PLC 에 쓰지 않는다.'],
], widths=[3.5, 12.5])

d.add_heading('2. 운전 화면(Client) 에 더해진 것', 1)
d.add_heading('2.1 크레인 · RGV 상태창 [데이터 삭제]', 2)
G.para(d, '상태창 오른쪽 명령 그룹의 [지시 재전송] [강제완료] 아래에 [데이터 삭제] 가 생겼다(종전 [수동지시] 자리). 누르면 확인창이 뜨고 [예] 를 누르면 '
          '그 호기의 삭제 신호 요청이 설비 통신으로 넘어간다. 권한은 [강제완료] 와 같은 EXE_YN 이다.')
G.image(d, os.path.join(SHOT, 'dlg_17120901_sc1.png'), 8.5, 'S/C#1 상태창 - 명령 그룹 [지시 재전송] [강제완료] [데이터 삭제]')
G.image(d, os.path.join(SHOT, 'dlg_17130801_rtv1.png'), 8.5, 'RGV 상태창 - [데이터 삭제]')
G.image(d, os.path.join(SHOT, 'ask_8246_sc1.png'), 9.0, '[데이터 삭제] 확인창 (S/C#1)')
G.image(d, os.path.join(SHOT, 'result_8246_sc1.png'), 9.0, '[예] 뒤 안내 - 요청이 DB 에 기록되었다는 뜻이며, PLC 에 쓰는 것은 설비 통신이 한다')
d.add_heading('2.2 리본 MANUAL ▷ 수동조작 ▷ [CV 지우기]', 2)
G.para(d, '리본 MANUAL 탭 수동조작 그룹의 [작업] 옆에 [CV 지우기] 가 생겼다. 누르면 확인창 → [예] 로 CV 비트 요청이 넘어간다. 설정과 무관하게 늘 보인다.')
G.image(d, os.path.join(SHOT, 'main_manual.png'), 15.0, 'MANUAL 탭 - 수동조작 [작업] [CV 지우기]')
G.image(d, os.path.join(SHOT, 'ask_ribbon_35071_cvclr.png'), 9.0, '[CV 지우기] 확인창')
note(d, '반송 중인 화물이 있을 때 [CV 지우기] 를 누르면 그 화물의 트래킹 번호도 지워진다. 설비가 비어 있을 때(또는 멈춘 상태에서 정리할 때) 쓴다.')
d.add_heading('2.3 어떻게 전달되나 (DB 변경 없음)', 2)
G.table(d, ['단추', 'Client 가 쓰는 것', '설비 통신(WCS_TASK_CV)이 하는 것', '이력'], [
    ['[데이터 삭제] (S/C n)', 'SC_DATA_LGLS(해당 호기) CMD_RQ_ID=HMIDEL, CMD_RQ_YN=Y', 'VehThread 가 [HMI_DELETE] SCn 비트에 1 을 쓰고 CMD_RQ_YN=N', 'WCS_LOG_PGR(VEH_SC) "운전 명령 HMIDEL …", WCS_CLIENT_LOG'],
    ['[데이터 삭제] (RGV)', 'RTV_DATA_LGLS CMD_RQ_ID=HMIDEL, CMD_RQ_YN=Y', 'VehThread 가 RTV1 비트에 1, N', 'WCS_LOG_PGR(VEH_RTV), WCS_CLIENT_LOG'],
    ['[CV 지우기]', 'CV_DATA(가장 작은 트랙 1행) CMD_RQ_ID=CVCLR, CMD_RQ_YN=Y', 'CvThread 가 CV 비트에 1, N', 'WCS_LOG_PGR(COMM0) "[CVCLR] …", WCS_CLIENT_LOG'],
], widths=[3.0, 4.5, 5.0, 3.5], font=8.5)
note(d, '주소가 비어 있으면 설비 통신은 "미정의" 로 이력만 남기고 N 으로 소비한다 - 요청이 쌓여 있다가 나중에 한꺼번에 나가는 일은 없다.')

d.add_heading('3. 확인한 것 (개발 PC, 설비 시뮬레이터, 2026-11-04)', 1)
G.para(d, 'WCS_DB.INI [HMI_DELETE] 에 시험 주소 M60000~60006 을 적고 시뮬(EQP_SIM)의 M 영역을 직접 읽어 확인했다.')
G.table(d, ['경로', '결과'], [
    ['S/C#1 상태창 [데이터 삭제] → [예]', 'SC_DATA_LGLS 901 CMD_RQ_ID=HMIDEL/Y → 설비 통신이 M60000=1 기록, N 소비, WCS_LOG_PGR·WCS_CLIENT_LOG 기록'],
    ['RGV 상태창 [데이터 삭제] → [예]', 'RTV_DATA_LGLS 801 → M60005=1, N 소비, 이력 기록'],
    ['리본 [CV 지우기] → [예]', 'CV_DATA(트랙 101) CMD_RQ_ID=CVCLR/Y → M60006=1, N 소비, 이력 기록'],
    ['주소를 비운 상태', '세 경로 모두 PLC 쓰기 없이 "미정의" 이력만 남고 N 으로 소비됨'],
], widths=[5.0, 11.0])
G.image(d, os.path.join(SHOT, 'result_ribbon_35071_cvclr.png'), 9.0, '[CV 지우기] 요청 뒤 안내')
note(d, '시험 뒤 WCS_DB.INI 의 주소는 비웠다(현장 기본). 적용 꾸러미의 추가할_설정_WCS_DB.ini.txt 도 빈 값이다.')

d.add_heading('4. 바뀐 파일', 1)
G.table(d, ['폴더', '파일', '비고'], [
    ['Client', 'EcsMain.exe', '빌드 2026-11-04 12:31 (Release). DLL 은 바꾸지 않는다'],
    ['Client\\rc_resource', 'dlg_sc\\dlg_sc.ini · dlg_rtv\\dlg_rtv.ini · mainframe_manual\\manual.ini · mainframe_manual\\cvclear.png', '버튼 글자(한/영/중/헝) 와 리본 아이콘. 같은 자리에 덮어쓴다'],
    ['Client', '추가할_설정_Ecs.ini.txt', '[MENU] HMIDEL_BTN=1 (안 적어도 기본 1)'],
    ['TASK\\EQP_TASK', 'WCS_TASK_CV.exe', '빌드 2026-11-04 12:05. HMIDEL / CVCLR 처리'],
    ['TASK\\EQP_TASK', '추가할_설정_WCS_DB.ini.txt', '[HMI_DELETE] 7개 영역(기존 6키 + CV 2키). 주소는 담당자 확정 뒤'],
    ['소스_변경분', 'TASK\\cHmiDeleteMap.cs · VehThread.cs · CvThread.cs / WCS\\ScSkinDlg · RtvSkinDlg · EcsDoc · MainFrm · resource.h · Ecs.rc + 변경내용_1104_*.diff', ''],
    ['문서', '이 문서, shots_1104', ''],
], widths=[3.0, 7.5, 5.5])

d.add_heading('5. 적용 순서', 1)
G.numbered(d, [
    'Client 를 닫는다.',
    'Client\\EcsMain.exe 를 깔린 폴더(예 C:\\LGLS\\CLIENT)에 덮어쓴다. Client\\rc_resource 아래 4개 파일도 같은 상대 경로에 덮어쓴다(dlg_sc, dlg_rtv, mainframe_manual).',
    'Ecs.ini [MENU] 의 ZOOM_BTN 아래에 추가할_설정_Ecs.ini.txt 의 두 줄(HMIDEL_BTN=1)을 더한다. ini 를 통째로 바꾸지 않는다.',
    '다시 띄워 제목줄 [Build:2026.11.04 …] 을 확인하고, 크레인 상태창에 [데이터 삭제], MANUAL 탭에 [CV 지우기] 가 보이는지 본다.',
    'WCS_TASK_CV 를 끝내고(ALL_TASK 면 [EQP] 정지) TASK\\EQP_TASK\\WCS_TASK_CV.exe 를 덮어쓴다.',
    'WCS_DB.INI [HMI_DELETE] 에 추가할_설정_WCS_DB.ini.txt 대로 CV_AREA= / CV_ADDR= 두 키를 더한다(기존 SC1~RTV1 6키는 10-15 판에 이미 있다). '
    '★PLC 담당자가 준 비트 주소 7개★ 를 각 _AREA=M, _ADDR=비트 10진 에 적는다. 아직 못 받았으면 비워 둔다.',
    'WCS_TASK_CV 를 기동하고 화면 로그 [HMI_DEL] 줄에 7개가 "SC1=M9000 …" 처럼 풀려 보이는지 확인한다("-" 는 미정의).',
    '주소를 적었다면 설비가 빈 상태에서 한 호기의 [데이터 삭제] 를 눌러 PLC 담당자와 비트가 켜지는지 맞춰 본다. 주소를 나중에 받으면 6~8 만 다시 한다.',
])
code(d, [
    '[HMI_DELETE]            ; WCS_DB.INI - 예시. 주소는 PLC 담당자 값으로',
    'SC1_AREA=M',
    'SC1_ADDR=9000           ; -> %MX9000',
    '...',
    'RTV1_AREA=M',
    'RTV1_ADDR=9005',
    'CV_AREA=M',
    'CV_ADDR=9006',
])
note(d, 'Upload/Download 를 쓰고 있다면 Upload 로 EcsMain.exe 와 rc_resource 파일을 올려 각 PC 의 런처가 받게 해도 된다 (2026-10-15_Upload_사용매뉴얼).')

d.add_heading('6. 현장 처리 순서 (운전자)', 1)
G.numbered(d, [
    '설비 위 화물을 먼저 내려놓는다(크레인은 출고 H/S 에). 반송 중인 화물이 있으면 [CV 지우기] 는 누르지 않는다.',
    '그 호기 상태창 [데이터 삭제] → [예]. (C/V 트래킹까지 지우려면 MANUAL ▷ [CV 지우기] → [예])',
    '설비 지상반에서 데이터가 지워졌는지 본다. 안 지워지면 종전대로 지상반 [삭제] 를 쓰고, 운영자는 WCS_LOG_PGR 의 "운전 명령 HMIDEL" 줄과 [HMI_DELETE] 주소를 확인한다.',
    '화면의 작업 정보는 이 단추로 지워지지 않는다. 작업까지 정리하려면 [완료처리](10-15 안내서) 를 이어서 쓴다.',
])

d.add_heading('7. 알아 둘 것', 1)
G.bullets(d, [
    'DB 변경 없음. 새 표·열·트리거가 없으므로 SQL 을 돌릴 것이 없다.',
    '[데이터 삭제] 는 Ecs.ini [MENU] HMIDEL_BTN=0 으로 숨길 수 있다. [CV 지우기] 는 늘 보인다.',
    'C/V 상태창의 [Ack 쓰기](ACKW) 는 현재 WCS_TASK_CV 소스에서 그 분기가 주석 처리된 블록 안에 있어 동작하지 않는다(종전부터). 이번 CVCLR 분기는 활성 체인에 두었다.',
    '개발 PC 의 Debug 빌드로 크레인 상태창을 열 때 뜨는 어설션(wincore.cpp 1079)은 이번 변경 전에도 있던 것으로 확인했고, 현장 Release 빌드에는 영향이 없다.',
])

out = os.path.join(DOCS, '2026-11-04_적용안내서_추가분.docx')
d.save(out)
print(out)
