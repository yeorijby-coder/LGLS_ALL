# -*- coding: utf-8 -*-
"""Upload(WmsUp.exe) 사용 매뉴얼 (2026-10-15)  -  python gen_upload_manual_1015.py -> 문서/2026-10-15_Upload_사용매뉴얼.docx"""
import os, sys
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, r'D:\project\LGLS\Renewal\docs\산출물_20260927\gen')
import gen_common as G
G.DATE = '2026-10-15'; G.VER = '1.0'; G.FINAL_NOTE = '최초 작성 - Upload(WmsUp) 사용법, 설정, 문제 해결'
SH = os.path.join(HERE, '문서', 'shots_upload')
def shot(n): return os.path.join(SH, n)

d = G.new_doc('Client 배포 Upload 프로그램\n사용 매뉴얼', 'WmsUp.exe - 올리기·조회·삭제·설정', history=[])

d.add_heading('1. 개요', 1)
G.para(d, 'Upload(WmsUp.exe)는 수정한 Client 파일을 DB(UP_DOWN, DN_MST 테이블)에 올리는 프로그램이다. '
          '현장 PC 의 Client 폴더에 있는 런처 Ecs.exe 는 시작할 때 DB 의 최신 파일을 받아 Client 를 갱신한다. '
          '즉 파일을 PC 마다 복사하지 않고, 한 곳에서 올리면 각 PC 가 다음 실행 때 스스로 받아 간다.')
G.table(d, ['프로그램', '위치', '역할'], [
    ['Upload (WmsUp.exe)', '개발·관리 PC', '파일을 DB 에 올리고, 올린 목록을 조회·삭제한다'],
    ['Download 런처 (Ecs.exe)', '각 Client PC 의 Client 폴더', '실행 때 DB 의 최신 파일을 받고 EcsMain.exe 를 띄운다'],
    ['DB', 'LGLS_MCS_IO', '올린 파일 본체는 UP_DOWN, 올린 이력은 DN_MST'],
], widths=[4.0, 4.5, 8.0])

d.add_heading('2. 준비 - 설정 파일', 1)
G.para(d, '같은 폴더의 WmsUp.ini 에 DB 접속 정보를 평문으로 둔다. 화면의 [설정]에서도 바꿀 수 있다(5 절).')
G.table(d, ['항목', '값', '설명'], [
    ['[DB Server] SERVERNAME', '예) 127.0.0.1,1435', 'DB 서버 주소(IP,포트). 현장 DB 로 맞춘다'],
    ['DATABASE', 'LGLS_MCS_IO', ''],
    ['USERID / PASSWORD', 'LGLS_IO / (비밀번호)', '비워 두면 Windows 인증으로 접속'],
    ['[DOWNLOAD PROGRAM] CNT, 1=', 'CNT=1, 1=WCS', '올릴 때 고르는 프로그램 구분. 이 현장은 WCS 하나'],
    ['Config.ini [DOWN_FILE] PATH', '.', '받은 파일을 놓을 하위 폴더. 비우면 Client 폴더 바로 아래'],
], widths=[5.0, 4.0, 7.5])
G.bullets(d, ['DB 에 접속하지 못하면 프로그램이 바로 종료된다. 접속 정보(주소·사용자)를 먼저 확인한다.',
              'DN_MST / UP_DOWN 테이블이 없으면 [조회] 때 "Table을 생성 하시겠습니까?" 가 뜬다. 새 DB 에서만 [확인]을 누른다. 있는 테이블을 다시 만들면 올린 파일이 지워진다.'])

d.add_heading('3. 화면 구성', 1)
G.image(d, shot('upl_main.png'), 14.5, '그림 1. 메인 화면')
G.table(d, ['부분', '설명'], [
    ['업로드일시', '조회할 기간(시작 ~ 끝). 기본은 최근 3개월. 서버 시각을 기준으로 채워진다'],
    ['다운로드 공통정보', '올린 건의 목록 - 번호, 프로그램명(WCS), 위치, 상세정보(올릴 때 적은 수정 내용)'],
    ['다운로드 상세정보', '고른 건에 들어 있는 파일 목록과 버전'],
    ['오른쪽 칸', '고른 건의 수정내역(올릴 때 적은 설명)'],
    ['[조회]', '기간 안에 올린 건을 다시 불러온다'],
    ['[업로드]', '파일 올리기 창(Upload처리)을 연다'],
    ['[다운로드]', '고른 건의 파일을 이 PC 에 받는다(수동 확인용). 평소에는 Client 런처가 자동으로 한다'],
    ['[삭제]', '고른 건과 그 건의 파일을 DB 에서 지운다'],
    ['[설정]', 'DB 접속, 제목, 아이콘 등을 바꾼다'],
    ['[종료]', '프로그램 종료'],
], widths=[3.5, 13.0])

d.add_heading('4. 파일 올리기', 1)
G.image(d, shot('upl_fileup.png'), 14.0, '그림 2. Upload처리 창')
G.numbered(d, [
    '메인 화면에서 [업로드]를 누르면 Upload처리 창이 열린다.',
    '오른쪽 위 "Download프로그램"에서 WCS 를 고른다.',
    '왼쪽 폴더 트리와 가운데 목록에서 올릴 파일을 찾아 고른다(여러 개 선택 가능). 보통 EcsMain.exe, Ecs.exe, rc_resource 안의 바뀐 파일 등이다.',
    '[추가]를 누르면 오른쪽 목록에 올라간다. 잘못 넣었으면 목록에서 골라 [삭제]한다.',
    '아래 "업데이트정보"에 이번에 무엇을 바꿨는지 적는다. 이 글이 메인 화면의 상세정보·수정내역으로 보인다.',
    '[UPLOAD]를 누른다. "다운로드 위치변경" 작은 창이 뜨면 받을 위치를 확인하고 [확인]한다(보통 "." 그대로).',
    '"데이타를 업로드 했습니다..." 가 아래 상태줄에 보이면 끝. 메인 화면에서 [조회]로 목록에 들어갔는지 확인한다.',
])
G.image(d, shot('upl_path.png'), 6.5, '그림 3. 받을 위치 지정')
G.para(d, '같은 이름의 파일을 다시 올리면 버전 번호가 하나 올라간다. 받는 쪽은 가장 높은 버전만 받고, 받은 뒤 DB 의 오래된 버전은 정리된다.', size=9.5)

d.add_heading('5. 조회와 삭제', 1)
G.bullets(d, [
    '[조회]는 "업로드일시" 기간 안의 건만 보여 준다. 끝 시각은 그날 23:59:59 까지로 본다(2026-10-15 판부터).',
    '목록의 건을 한 번 누르면 아래 상세정보와 오른쪽 수정내역이 바뀐다.',
    '[삭제]는 목록에서 고른 한 건을 지운다. 지우면 되돌릴 수 없으므로, 아직 받지 않은 PC 가 있는지 확인한 뒤 지운다.',
    '삭제할 건을 고르지 않고 누르면 "삭제할 데이터를 선택하여 주십시오." 가 뜬다.',
])
G.image(d, shot('upl_main_after.png'), 14.5, '그림 4. 조회 결과 (예시 행 1건)')

d.add_heading('6. 환경설정', 1)
G.image(d, shot('upl_set.png'), 9.5, '그림 5. UPLOAD환경설정 - 프로그램 및 서버 설정')
G.table(d, ['항목', '설명'], [
    ['TITLE / ICON', '창 제목과 아이콘 파일'],
    ['DOWN_LOAD_PATH_FIX', '받는 파일의 고정 위치. 비우면 Client 폴더'],
    ['SETUP_BTN / DOWN_LOAD_PATH_SET', '가능/불가 - 설정 단추 사용, 올릴 때 위치 지정 허용 여부. 이 현장 기본: 가능 / 불가'],
    ['UP_LOAD_KEY', '파일 구분 방식. File 명(기본)'],
    ['DB 서버 설정', 'SERVERNAME(IP,포트), DATABASE, USERID, PW. 바꾸고 [저장]하면 WmsUp.ini 에 기록된다'],
    ['테이블 생성 탭', 'DN_MST / UP_DOWN 을 새로 만든다. 새 DB 에서만 사용(기존 자료가 지워짐)'],
], widths=[5.0, 11.5])

d.add_heading('7. 받는 쪽(Client PC) 동작', 1)
G.numbered(d, [
    'Client 폴더의 Ecs.exe(런처)를 실행한다.',
    '런처가 DB 에 접속해 올라온 최신 파일을 확인하고, 이 PC 의 버전보다 높은 것만 받는다.',
    '받기가 끝나면 EcsMain.exe 를 띄운다.',
    '런처 자신(Ecs.exe)이 새 버전이면 실행 중인 자기 파일을 Ecs.exe.old 로 비켜 놓고 새 파일을 쓴다. .old 는 다음 실행 때 지운다(2026-10-15 판부터).',
])
G.para(d, '주의: 2026-10-15 판 이전 런처가 깔린 PC 는 Ecs.exe 를 받을 때 "사용 중" 오류가 난다. 그 PC 는 Ecs.exe 를 한 번 직접 복사해 새 판으로 바꿔 준다.', size=9.5)

d.add_heading('8. 문제 해결', 1)
G.table(d, ['증상', '원인', '조치'], [
    ['"업로드 완료"는 떴는데 [조회] 목록이 비어 있다',
     '끝 시각이 화면을 연 시각으로 고정돼 있어서, 연 뒤에 올린 건이 기간 밖이었다(2026-10-15 판 이전)',
     '2026-10-15 판 WmsUp.exe 로 교체. 이전 판이면 끝 시각을 날짜 선택에서 내일로 바꾸고 [조회]'],
    ['시작하자마자 프로그램이 닫힌다', 'DB 접속 실패(주소·사용자·비밀번호)', 'WmsUp.ini 의 [DB Server] 값 확인, 서버 방화벽·포트 확인'],
    ['"Table이 없습니다" 가 뜬다', 'DN_MST / UP_DOWN 이 없다', '새 DB 라면 [확인]으로 생성. 기존 DB 면 접속한 DB 이름이 맞는지 먼저 확인'],
    ['다운로드할 때 Ecs.exe 가 사용 중이라고 한다', '런처가 자기 자신을 덮어쓰려 해서(작업 관리자에는 안 보임)', '2026-10-15 판 Ecs.exe 로 교체(7 절 주의 참고)'],
    ['"기존 프로그램이 이미 사용중 입니다"', '같은 경로의 Ecs.exe 가 두 개 이상 실행 중', '작업 관리자에서 Ecs.exe / EcsMain.exe 를 모두 끝낸 뒤 다시 실행'],
    ['"프로세스명이 변경되어 프로그램을 실행 할 수 없습니다"', 'Ecs.exe 이름을 바꿔 실행', '이름을 Ecs.exe 로 되돌린다'],
], widths=[4.8, 5.7, 6.0], font=8.5)

d.add_heading('9. 올리기 전 점검표', 1)
G.bullets(d, ['빌드한 EcsMain.exe 가 최신 Release 인지(Debug 를 올리면 DciLib 오류가 난다)',
              '올릴 때 "업데이트정보"에 바꾼 내용을 한 줄 이상 적었는지',
              '올린 뒤 [조회]에서 건이 보이고 상세정보에 파일이 들어 있는지',
              '한 PC 에서 Ecs.exe 를 실행해 받기와 Client 기동이 되는지 확인한 뒤 다른 PC 에 알린다'])

out = os.path.join(HERE, '문서', '2026-10-15_Upload_사용매뉴얼.docx')
d.save(out); print(out)
