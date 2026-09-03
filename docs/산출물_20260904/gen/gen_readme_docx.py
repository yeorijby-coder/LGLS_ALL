# -*- coding: utf-8 -*-
"""설치 및 구동 안내서 (2026-09-04 판) : 시뮬레이터·숨김 메뉴 표기 없이 정리"""
import os
from gen_common import *

d = new_doc('설치 및 구동 안내서', 'EXE 실행본 / 서버 · 키오스크 구동 요건')

d.add_heading('1. 실행본 구성', 1)
para(d, '각 폴더를 통째로 복사하면 그 자리에서 실행된다. 실행에 필요한 파일과 설정(INI/XML/리소스)만 담았다.')
table(d, ['폴더', '프로그램', '실행 파일', '역할'], [
    ['EXE\\TASK\\WCS_TASK_CV', '설비 통신', 'WCS_TASK_CV.exe', '마스터 PLC(XGT)와 통신, 설비 데이터 갱신·지시 전송'],
    ['EXE\\TASK\\IO_TASK', '스케줄러', 'IO_TASK_SEMI_FINISH.exe', '작업 상태 진행, 설비 지시 발행, 완료 판정'],
    ['EXE\\TASK\\WCS_TASK_HOST', '상위 통신', 'TASK_LFC10_G1_ECSCOM.exe', 'WMS/IMS 전문 송수신'],
    ['EXE\\WCS_CLIENT', '운전 화면', 'Ecs.exe', '창고 모니터링·수동 지시·로그 조회'],
    ['EXE\\Prerequisites', '사전 설치 파일', 'vc_redist.x86.exe / ndp48-*.exe', 'VC++ 재배포 패키지, .NET Framework 4.8'],
    ['EXE\\DB_BACKUP', 'DB 백업', '*.bak, 서버_구축_절차.md', 'DB 이관용']], widths=[4.2, 2.4, 4.6, 5.8], font=8.5)

d.add_heading('2. 기동 순서', 1)
para(d, 'WCS_TASK_CV  ->  IO_TASK  ->  WCS_TASK_HOST', bold=True, indent=0.5)
para(d, '운전 화면(Ecs.exe)은 아무 때나 띄워도 된다.', indent=0.5)

d.add_heading('3. 접속 설정', 1)
table(d, ['프로그램', '파일', '항목', '현장값'], [
    ['WCS_TASK_CV', 'WCS_DB.INI', '[COMM0] IP', '192.100.1.110 (마스터 PLC)'],
    ['WCS_TASK_HOST', 'EcsComA.ini', 'LocalPort / RemoteIP / RemotePort', '8001 / 192.100.1.191 / 8002'],
    ['공통', '각 INI [DB] IP', 'DB 서버', '서버IP,1433'],
    ['운전 화면', 'Ecs.ini [DB_2]', 'SERVER / USERID / USERPASSWORD', '서버IP,1433 / LGLS_IO / (지정)']], widths=[3.5, 3.5, 5.5, 4.5])

d.add_heading('4. 서버(VS 미설치) 구동 요건', 1)
bullets(d, ['운전 화면(WCS_CLIENT\\Ecs.exe)은 Release 빌드다. 서버에는 Microsoft Visual C++ 2015-2022 Redistributable (x86) 를 설치한다 (mfc140u / vcruntime140 / msvcp140 / UCRT). FarPoint 스프레드(SPR32DU70.DLL)는 폴더에 들어 있다.',
            'TASK 3종은 .NET Framework 4.8 로 돈다(IO/HOST 는 4.0 대상이지만 4.8 위에서 동작). Windows Server 2019 이상은 기본 포함, 없으면 .NET Framework 4.8 런타임 설치.',
            'DB 는 서버의 MS-SQL 을 쓰도록 각 INI 의 [DB] 접속 정보(IP,1433 / 계정)를 바꾼다. DB 이관은 DB 구축 정보 문서와 DB_BACKUP\\서버_구축_절차.md 를 따른다.'])

d.add_heading('5. 키오스크(운전 화면 전용 PC) 구동 요건', 1)
para(d, '키오스크에는 WCS_CLIENT 폴더만 복사한다. MS-SQL / .NET / Visual Studio 모두 필요 없다.')
numbered(d, ['EXE\\WCS_CLIENT 폴더를 통째로 복사 (Release 실행 파일, DLL, 리소스, 폰트 포함)',
             'EXE\\Prerequisites\\vc_redist.x86.exe 설치 (VC++ 2015-2022 x86) - 이것 하나만',
             'WCS_CLIENT\\Ecs.ini 의 [DB_2] 를 서버로 변경'])
t = d.add_table(rows=1, cols=1); t.style = 'Table Grid'; c = t.rows[0].cells[0]; shade(c, 'F2F2F2'); c.text = ''
for k, cl in enumerate(['[DB_2]', 'DRIVER=SQL Server          ; Windows 기본 내장 ODBC 드라이버 - 추가 설치 불필요', 'SERVER=서버IP,1433         ; 인스턴스 이름(서버IP\\인스턴스) 보다 IP,포트 형식이 간단',
                        'USERID=LGLS_IO             ; 서버에 만든 SQL 로그인(혼합 인증)', 'USERPASSWORD=LGLS_IO', 'DATABASE=LGLS_MCS_IO']):
    p = c.paragraphs[0] if k == 0 else c.add_paragraph(); r = p.add_run(cl); r.font.name = 'Consolas'; r.font.size = Pt(8.5)
d.add_paragraph()
bullets(d, ['서버 쪽은 DB 구축 정보 문서의 혼합 인증, TCP 1433 고정, 방화벽 1433 인바운드가 되어 있어야 한다.',
            '확인 : Ecs.exe 하단 상태바의 EQUIP / HOST / SCH 가 초록이면 DB 를 정상으로 읽는 것(서버의 TASK 가 돌고 있어야 초록).',
            '접속이 안 되면 키오스크의 ODBC 데이터 원본 관리자(32비트)에서 "SQL Server" 드라이버로 서버 연결 테스트를 해 네트워크/인증 문제를 먼저 가린다.',
            '키오스크에는 TASK 를 두지 않는다. 화면·알림창·설비 대화상자 모두 서버 DB 만 보고 동작한다.'])

d.add_heading('6. 사전 설치 파일 (Prerequisites, Microsoft 공식 배포본)', 1)
table(d, ['파일', '용도', '비고'], [
    ['vc_redist.x86.exe', 'VC++ 2015-2022 재배포 패키지 x86 (운전 화면용)', '13MB'],
    ['ndp48-x86-x64-allos-enu.exe', '.NET Framework 4.8 오프라인 설치(인터넷 없는 서버)', '116MB'],
    ['ndp48-web.exe', '.NET Framework 4.8 웹 설치(인터넷 되는 서버)', '1.4MB']], widths=[5.5, 8, 3.5])
para(d, '설치 순서 : .NET 4.8 → vc_redist.x86 → (재부팅 요구 시 재부팅) → 각 프로그램 폴더 복사 → INI 수정.')
out = os.path.join(OUT, '01_설치_및_구동_안내서.docx'); d.save(out); print('saved', out)
