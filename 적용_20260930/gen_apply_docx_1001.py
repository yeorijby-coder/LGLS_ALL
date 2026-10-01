# -*- coding: utf-8 -*-
u"""2026-10-01 적용 안내서 - ★추가분만★ (Word).

   사용자 지시 : "이 질문 하기 전에 적용안내서를 출력했어..
                 저 질문 이후에 추가된 내용은 일자를 다르게 만들어서
                 추가된 부분만 문서를 만들어줘"

   앞선 안내서(2026-09-29)를 이미 출력하셨으므로, 그 뒤에 더해진 것만 담는다.
   앞 문서를 다시 볼 필요가 없게, 필요한 배경은 이 문서 안에 짧게 적는다.

   실행 : python gen_apply_docx_1001.py
"""
import os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, os.path.join(ROOT, 'docs', '산출물_20260927', 'gen'))

from docx.shared import Pt, Cm, RGBColor
import gen_common as G

NAVY, GRAY = G.NAVY, G.GRAY
RED = RGBColor(0xB0, 0x2A, 0x20)


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


G.DATE = '2026-10-01'
G.VER = '1.0'
G.BASE_HISTORY = []
G.FINAL_NOTE = ('2026-09-30 적용 안내서(추가분)에 이어지는 추가분. 리본 신호등을 다른 리본 단추와 같은 방식으로 그리는 선택지.')

d = G.new_doc('2026-10-01 적용 안내서 (추가분)',
              '리본 신호등을 리본 단추 방식으로 (LAMP_STYLE) · 로그오프 없이 설치')

d.add_heading('0. 이 문서는 무엇인가', 1)
G.para(d, '2026-09-30 적용 안내서(추가분, 1~9절)를 이미 받아 보셨다면, 그 뒤에 더해진 것만 여기에 담았다. '
          '파일은 모두 적용_20260930 폴더에 있다(폴더는 그대로 쓴다).')
note(d, '앞 문서들(09-29, 09-30)의 내용은 그대로 유효하다. 이 문서는 그 위에 얹는 것이다.')

d.add_heading('1. 리본 신호등을 다른 리본 단추처럼 그리는 선택지', 1)
G.para(d, '질문 : "신호등을 리본과 표처럼(Windows 가 키우는 방식으로) 바꾸는 것은 안 되나?" - 된다. '
          '리본의 다른 단추들은 32픽셀 아이콘 + 글자를 MFC 가 그리고, Windows 배율만큼 아이콘과 글자를 키운다. '
          '신호등을 그 길로 보내면 현장에서 이미 정상으로 보이던 다른 단추들과 똑같이 그려진다.')
G.para(d, '지금까지의 신호등(사진 같은 모양)은 우리가 직접 그리므로 배율을 우리가 맞춰야 했다. 09-30 안내서 8절에서 그 계산을 고쳤지만 '
          '실제 300% 화면에서 돌려 보지는 못했다. 그래서 두 방식을 모두 두고 설정으로 고른다.')

d.add_heading('1.1 설정 (Ecs.ini [DISPLAY])', 2)
code(d, [
    '[DISPLAY]',
    'LAMP_STYLE = 0   ; 0 프로그램이 직접 그린다(기본, 사진 같은 모양, UI_SCALE 을 따른다)',
    '                 ; 1 다른 리본 단추와 같은 아이콘 단추 - Windows 배율과 리본 글꼴을 그대로 따른다',
])
G.table(d, ['값', '모양', '배율', '어울리는 때'], [
    ['0', '사진 같은 삼색 신호등 + 이름', 'UI_SCALE(자동이면 Windows 배율)', '1920x1080, 4K 모니터링 TV(Windows 100%)'],
    ['1', '리본 단추(아이콘 32px + 이름)', 'Windows 배율 그대로 (리본의 다른 단추와 같다)', '4K@300% 처럼 0 이 어긋나 보일 때'],
], widths=[1.2, 5.0, 5.0, 4.8])
G.para(d, '1 로 바꾸면 다시 띄워야 반영된다. 아이콘은 rc_resource\mainframe_config\lamp_red/yellow/green.png 이며, '
          '없으면 0 으로 돈다. 상태 표시(빨강 = 끊김, 노랑<->초록 1초 교대 = 정상)와 눌렀을 때의 핑/포트 확인은 두 방식이 같다.')

d.add_heading('1.2 확인한 것', 2)
G.table(d, ['구분', '내용'], [
    ['확인', '1920x1080@100% 에서 LAMP_STYLE=1 이 다른 리본 단추와 같은 크기·글꼴로 그려지고, UI_SCALE 을 200 으로 올려도 신호등은 리본 단추 크기를 지킨다(범례만 커진다).'],
    ['확인', '0 과 1 모두 상태 교대(노랑<->초록)와 끊김(빨강)이 같은 판정으로 바뀐다.'],
    ['못 함', '실제 Windows 배율 300% 화면에서는 돌려 보지 못했다. 다른 리본 단추가 그 화면에서 정상이므로 1 은 같은 길을 타지만, 현장에서 눈으로 확인해 달라.'],
], widths=[2.0, 14.0])

d.add_heading('1.3 현장에서 고르는 순서', 2)
G.numbered(d, [
    '새 Client 를 깔고 그대로(LAMP_STYLE=0, UI_SCALE=0) 띄운다. 신호등이 잘리지 않으면 끝.',
    '어긋나 보이면 Ecs.ini [DISPLAY] LAMP_STYLE = 1 로 바꾸고 다시 띄운다.',
    '그래도 지도·범례가 어긋나면 09-30 안내서 8.3 의 표(UI_SCALE / LAYOUT_FONT_SCALE), 그 다음 8.4(화면배율_방식.bat).',
])

d.add_heading('2. 바뀐 파일', 1)
G.table(d, ['폴더', '파일', '비고'], [
    ['Client', 'EcsMain.exe', 'LAMP_STYLE 포함 (09-30 의 4K 대응도 들어 있다)'],
    ['Client/rc_resource/mainframe_config', 'lamp_red.png, lamp_yellow.png, lamp_green.png', '아이콘 방식용 그림 3장 - ★같이 복사★'],
    ['Client', '추가할_설정_Ecs.ini.txt', '[DISPLAY] 에 LAMP_STYLE 추가'],
    ['Client / Client_설치', '폰트잠금_풀기.bat, Force-ReleaseFonts.ps1', '잠긴 폰트 강제 풀기 (3.1절)'],
    ['Client_설치', '(꾸러미 전체)', '위 내용이 들어간 새 꾸러미'],
], widths=[4.5, 6.5, 5.0])

d.add_heading('3. 로그오프 없이 설치하기 (폰트 잠금)', 1)
G.para(d, '09-30 안내서 대로면 옛 Client 를 돌린 적이 있는 PC 는 폰트 파일이 잠겨 로그오프가 필요했다. '
          '현장 PC 는 로그오프할 수 없다고 하여, 설치 프로그램이 스스로 피해 가게 했다.')
G.table(d, ['경우', '설치 프로그램이 하는 일'], [
    ['옛 Client 가 깔린 폴더에 그대로 덮어쓴다 (제거하지 않았다)', '폰트 파일은 잠겨 있어도 ★같은 파일★ 이라 건너뛰고 나머지를 복사한다. 그대로 쓴다.'],
    ['제거(Uninstall)한 뒤라 지우다 만 폰트가 남아 있다', '그 폴더는 로그오프 전까지 못 쓴다. ★옆 이름(C:\LGLS\CLIENT_1, _2 ...)★ 에 깔고 바로가기도 거기로 만든다. 옛 폴더는 나중에 로그오프한 뒤 지운다.'],
    ['처음 까는 PC', '종전과 같다.'],
], widths=[6.0, 10.0])
G.para(d, '즉 현장에서는 제거 프로그램을 돌리지 말고 바로 Setup.bat 으로 덮어쓰는 것이 가장 간단하다. '
          '이미 제거한 PC 라도 Setup.bat 이 옆 폴더를 골라 주므로 그냥 진행하면 된다. '
          '설치가 끝나면 새 Client 는 폰트를 자기 프로세스에만 등록하므로 다음부터는 이 문제가 없다.')
d.add_heading('3.1 잠긴 폰트를 강제로 푸는 도구 - 폰트잠금_풀기.bat', 2)
G.para(d, '"로그오프 없이 강제로 풀 수 없나" - 잠금의 주인에 따라 다르다. 세 가지를 차례로 시도하는 도구를 넣었다. '
          'Setup.bat 도 잠긴 폴더를 만나면 먼저 이것을 돌려 보고, 그래도 안 풀릴 때만 옆 폴더로 간다.')
G.table(d, ['단계', '하는 일', '푸는 것'], [
    ['1', 'RemoveFontResource 를 등록이 0 이 될 때까지 반복 + 글꼴 변경 알림', '옛 Client 가 AddFontResource 로 쌓아 둔 시스템 등록'],
    ['2', 'Restart Manager 로 그 파일을 잡은 프로그램을 찾아 보여 주고, 승낙하면 닫는다 (탐색기는 닫았다가 다시 띄운다)', '브라우저·메신저 등 글꼴을 읽어 둔 프로그램'],
    ['3', 'Windows 글꼴 캐시 서비스(FontCache, FontCache3.0.0.0) 재시작', '글꼴 캐시가 잡은 것'],
    ['4', '파일마다 쓰기로 열어 보아 풀렸는지 확인', '-'],
], widths=[1.2, 9.0, 5.8])
G.para(d, '사용 : 설치 폴더(또는 꾸러미)의 폰트잠금_풀기.bat 을 관리자 권한으로 실행한다. 인자 없이 돌리면 그 폴더, '
          '인자로 폴더를 주면 그 폴더를 푼다. 2단계에서 닫히는 프로그램이 있으니 저장할 것은 먼저 저장한다.')
note(d, '세션(로그온) 자체가 잡고 있는 경우는 이 도구로도 풀리지 않는다. 그때는 "그래도 잠겨 있습니다" 를 보이고 끝나며, '
        'Setup.bat 은 옆 폴더에 깐다. 이 PC 에서는 (1) 다른 프로그램이 잡은 상태를 만들어 1~2단계로 풀리는 것, '
        '(2) Setup.bat 이 풀린 뒤 같은 폴더를 다시 쓰는 것을 확인했다. 실제 현장의 "지우다 만" 상태는 재현하지 못했다.')

note(d, '이 PC 에서 폰트를 일부러 잠가 두고 확인했다 : 잠긴 폴더를 알아내 _1 로 바꾸고, 잠금을 풀면 원래 폴더를 쓴다. '
        '실제 "지우다 만" 상태는 이 PC 에서 이미 풀려 재현하지 못했다.')

SHOT = os.path.join(HERE, 'shots_1001')

d.add_heading('4. 작업정보 판넬을 구 ECS 하단 판넬처럼 셋으로', 1)
G.para(d, '구 ECS 메인 아래쪽의 [우선순위 ▲▼ 반송조정 | 작업목록 | 선택작업 상세(ECS번호·작업번호·SEQ 표)·완료처리] 를 '
          '신 ECS 왼쪽 작업정보 판넬에 그대로 나누어 넣었다. 모두 동작한다.')
G.image(d, os.path.join(SHOT, 'panel_up.png'), 16.0, '왼쪽 작업정보 판넬 - 0873 을 고르고 ▲ 를 눌러 우선순위가 002 로 바뀐 모습')
G.table(d, ['칸', '내용', '동작'], [
    ['왼쪽', '우선순위 값(큰 글씨), [▲] [▼], [반송조정]', '▲▼ = JOB_MST.JOB_PRIORITY ±1 (확인창 → 권한 UPD_YN → CLIENT_LOG). 반송조정 = 작업목록 [JOB_MST] 창'],
    ['가운데', '작업 목록 (탭 필터, 자동갱신) - 종전 그대로', '행을 고르면 상세가 채워진다'],
    ['상세', '머리줄 한 줄 : 작업번호(LUGG_NO, 붉은 글씨) / 팔렛(LOT_NO) / [완료처리]  + SEQ 표', '완료처리 = 출고류 19, 그 밖 29 (확인창 → 권한 → 로그). 구 ECS 의 ECS번호/작업번호는 신 ECS 용어(작업번호/팔렛)로'],
    ['SEQ 표', '구 ECS 처럼 전부 트랙 번호 기준 : CONVEYOR:11 PORT:122→PORT:121 / RGV PORT:121→PORT:103 / CONVEYOR:2 PORT:103→PORT:104 / S/C 1 PORT:104→LOC:04-001-01', '트랙→C/V 번호는 CV_DATA(PLC_NO,TRACK_NO). RGV 픽업 트랙 = 같은 C/V 의 옆 트랙. 통로(HS_TRACK_NO)가 아직 없으면 "통로" 로 표시'],
], widths=[2.0, 8.0, 6.0])
note(d, '판넬이 좁으면(620px 미만, 왼쪽 도킹 기본 폭이 약 310) 상세 칸이 목록 ★아래★ 로 내려간다. 판넬을 넓히면 구 ECS 처럼 가로 세 칸이 된다.')

d.add_heading('5. 설비 상태창 [확대] 패널을 구 ECS 폼 배치로', 1)
G.para(d, '구 ECS 의 StackerForm / RGVForm / ConveyorForm 항목이 전부 신 ECS 상태창의 [확대] 패널에, 구 폼과 같은 좌표로 들어갔다. '
          '항목별 대조는 같은 폴더의 "2026-10-01_설비대화상자_비교_구ECS_vs_신ECS.docx" 에 있다.')
G.image(d, os.path.join(SHOT, 'dlg_sc_1.png'), 16.0, 'SC 상태창 [확대] - 오른쪽이 구 ECS [Stacker Crane 정보] 사진 배치')
G.image(d, os.path.join(SHOT, 'dlg_rtv_1.png'), 16.0, 'RTV 상태창 [확대] - 구 ECS RGV 사진 배치')
G.image(d, os.path.join(SHOT, 'dlg_cv_1.png'), 14.0, 'CV 상태창 [확대] - 구 ECS [Conveyor 정보] 사진 배치')
G.table(d, ['설비', '구 ECS 에 있던 것 → [확대] 패널', '신 ECS 에서 더한 것'], [
    ['S/C', '[DOWN/IDLE/RUN] 상태칸·설비명·설명 / LED 4x3 (Load Complete … Alarm Reset ACK, Pallet ID) / 현재위치·출발지·도착지·완료위치·알람코드 한 줄 / 요청번호 ― 순번·배치번호·자재코드·팔렛(에러 tag)·출발위치·도착위치 / [명령 재전송][이상종료] / 사용금지 / [확인]  - 사진(Stacker Crane 정보) 과 같은 자리·순서', 'PLC 주소 한 줄(맨 아래), [Load ACK 쓰기][Unload ACK 쓰기]'],
    ['RGV', 'S/C 와 같은 틀 (완료처리 = 39)', '〃'],
    ['C/V', '[IDLE] 상태칸·이름(굵은 파랑)·설명 / "포트"·"Pallet" 머리글 + 포트마다 [번호칸][팔렛 값·입력][PalletID설정] (4색 규칙) / 사용금지 / [확인] - 사진([Conveyor 정보]) 과 같은 자리', '사진의 가운데 빈 자리에 LED 11 + 주소, [적재ACK 쓰기][하역ACK 쓰기], 트래킹화물·방향모드'],
], widths=[1.5, 10.0, 4.5], font=8.5)
G.table(d, ['버튼', '하는 일', '권한/기록'], [
    ['이상종료', '설비 데이터의 작업(OD) 을 지운다 (PanelInfoDlg 의 이상종료와 같음)', '〃'],
    ['사용금지', '기존 [작업금지]/[RTV 금지]/[일시정지] 와 같은 동작을 체크박스로', '〃'],
    ['PalletID설정', 'CV_DATA.LUGG_NO_OD 에 입력값 + TRACKING_WRITE_YN=Y', 'CCvSkinDlg'],
], widths=[2.5, 9.0, 4.5], font=8.5)
note(d, '[확대] 는 Ecs.ini [MENU] ZOOM_BTN=1 일 때만 보인다. 0 으로 둔 PC 는 1 로 바꿔야 패널을 볼 수 있다.')
G.para(d, 'SC/RTV/CV 패널은 코드가 아니라 Ecs.rc 의 대화상자 템플릿 IDD_SCV_PANEL / IDD_RTVV_PANEL / IDD_CVV_PANEL 로 그린다(2026-10-01 저녁, 사용자 지시). '
          '[확대] 때 그 템플릿으로 자식 창을 만들고 안의 컨트롤을 본체 창으로 옮겨 붙이므로 값 갱신·버튼·색 코드는 ID 그대로 동작한다. '
          '배치를 바꾸려면 VS 리소스 편집기에서 그 템플릿을 열어 끌어 놓으면 된다. 설비명·PLC 주소·[쓰기] 문구만 런타임에 채운다 (CV 의 LED 옆 주소 칸은 IDC_CVV_ADDR_BASE+0~12).')

d.add_heading('5.1 기동 위치 - 모니터가 여럿일 때 주 모니터에', 2)
G.para(d, '창이 마지막에 있던 모니터(다른 모니터)에 뜨는 것을 막는다. Ecs.ini [DISPLAY] START_MONITOR = 0(기본) 주 모니터 / N(1~) Windows 디스플레이 설정의 번호(식별 단추로 보이는 번호) / -1 종전처럼 마지막 위치. '
          '그 모니터의 작업 영역에 맞춘 뒤 최대화한다. 2026-10-01 저녁 추가 (사용자 지시 : 2번 디스플레이 전체). 번호는 바탕화면에 붙은 디스플레이 장치 순서로 센다.')
G.para(d, '작업정보 판넬(왼쪽 아래)도 바꿨다 : 작업구분 탭 줄과 [자동 갱신] 을 없앴고(자동 갱신은 켜진 채 숨김), [완료처리] 는 상세 칸 맨 위 오른쪽으로, '
          '목록|상세 사이 분할선은 끌어서 폭을 바꿀 수 있다. 이 판넬의 컨트롤도 Ecs.rc 의 IDD_PANEL_JOB 에 있어 리소스 편집기에서 고칠 수 있다 '
          '(왼쪽 칸은 리소스 좌표 그대로, 오른쪽 머리줄은 리소스의 상대 위치를 유지).')
G.table(d, ['Ecs.ini [DISPLAY]', '값', '뜻'], [
    ['JOB_PANEL_SPLIT', '0 / 1 / 2', '0 자동(판넬 폭 620px 미만이면 세로) / 1 가로(상세 칸 오른쪽) / 2 세로(상세 칸 아래)'],
    ['JOB_PANEL_DETAIL_W', 'px', '가로일 때 상세 칸 폭 (0 = 38%)'],
    ['JOB_PANEL_DETAIL_H', 'px', '세로일 때 상세 칸 높이 (0 = 178)'],
], widths=[4.5, 2.5, 9.0])
G.para(d, '분할선(가로면 세로줄, 세로면 가로줄)을 마우스로 끌면 그 크기가 우선하고, 다시 띄우면 ini 값으로 돌아온다. ini 를 저장하면 다음 크기 변경 때 반영된다.')
G.para(d, '왼쪽 칸 [반송조정] 아래의 [가로보기]/[세로보기] 단추로도 바꿀 수 있다(단추 글자는 바꿀 방향). 누르면 JOB_PANEL_SPLIT 에 그 값을 써 두므로 다음에 띄워도 유지된다.')

d.add_heading('6. 바뀐 파일 (4·5절)', 1)
G.table(d, ['폴더', '파일', '비고'], [
    ['Client', 'EcsMain.exe, DciLib.dll', '★둘을 같이★ 복사 (DciLib 없이 새 exe 만 넣으면 안 뜬다)'],
    ['Client_설치', '(꾸러미 전체)', '위 내용이 들어간 새 꾸러미'],
], widths=[4.5, 6.5, 5.0])

out = os.path.join(HERE, '2026-10-01_적용안내서_추가분.docx')
d.save(out)
print('생성 :', out)
