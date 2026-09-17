# -*- coding: utf-8 -*-
"""설치 및 구동 안내서 (2026-09-21 최종판) : 현장 기본 시험 완료 기준. 시뮬레이터·숨김 메뉴 표기 없음.
   09-04 판 대비 : 통합 실행본 ALL_TASK, INI 파일/섹션 정리(상세는 12_INI_설정_정의서), 교체(재기동) 주의,
   현장 DB 반영 SQL 2종, Ecs.exe + DciLib.dll 동반 배포."""
import os
from gen_common import *

d = new_doc('설치 및 구동 안내서', 'EXE 실행본 / 서버 · 키오스크 구동 요건',
            history=[['1.0', '2026-09-03', AUTHOR, '최초 작성'],
                     ['1.1', '2026-09-04', AUTHOR, '검토 의견 반영 (숨김 메뉴·시뮬레이터 표기 제외, 서버·키오스크 구동 요건 추가)']])

d.add_heading('1. 실행본 구성', 1)
para(d, '각 폴더를 통째로 복사하면 그 자리에서 실행된다. 실행에 필요한 파일과 설정(INI/XML/리소스)만 담았다.')
table(d, ['폴더', '프로그램', '실행 파일', '역할'], [
    ['EXE\\TASK\\WCS_TASK_CV', '설비 통신', 'WCS_TASK_CV.exe', '마스터 PLC(XGT)와 통신, 설비 데이터 갱신·지시 전송'],
    ['EXE\\TASK\\IO_TASK', '스케줄러', 'IO_TASK_SEMI_FINISH.exe', '작업 상태 진행, 설비 지시 발행, 완료 판정'],
    ['EXE\\TASK\\WCS_TASK_HOST', '상위 통신', 'TASK_LFC10_G1_ECSCOM.exe', 'WMS/IMS 전문 송수신'],
    ['EXE\\WCS_CLIENT', '운전 화면', 'Ecs.exe (+ DciLib.dll)', '창고 모니터링·수동 지시·로그 조회. Ecs.exe 와 DciLib.dll 은 반드시 함께 교체·배포한다'],
    ['TASK\\ALL_TASK\\bin\\Debug', '통합 실행본(선택)', 'ALL_TASK.exe', '위 TASK 3종을 한 프로세스로 실행. 설정은 각 TASK 의 INI 를 그대로 읽고, ALL_TASK.INI 는 로그 폴더·보관일·자동기동만 가진다'],
    ['EXE\\Prerequisites', '사전 설치 파일', 'vc_redist.x86.exe / ndp48-*.exe', 'VC++ 재배포 패키지, .NET Framework 4.8'],
    ['EXE\\DB_BACKUP', 'DB 백업', '*.bak, 서버_구축_절차.md', 'DB 이관용 (02_DB_구축_정보 참조)'],
    ['산출물 폴더\\SQL', '현장 DB 반영 SQL', '00_현장DB_공통코드_추가.sql 외 1', '최종판 프로그램이 쓰는 공통코드·알람코드 (6장)']],
    widths=[4.2, 2.4, 4.6, 5.8], font=8.5)
para(d, '※ TASK 3종을 따로 띄우는 방식과 ALL_TASK 한 개로 띄우는 방식 중 하나만 쓴다. 둘을 동시에 띄우지 않는다.', color=GRAY, size=9)

d.add_heading('2. 기동 순서와 교체(재기동) 주의', 1)
para(d, 'WCS_TASK_CV  ->  IO_TASK  ->  WCS_TASK_HOST', bold=True, indent=0.5)
para(d, '운전 화면(Ecs.exe)은 아무 때나 띄워도 된다. ALL_TASK 는 내부에서 같은 순서(EQP -> IO -> HOST)로 기동한다.', indent=0.5)
bullets(d, ['프로그램 교체·재기동은 크레인·RGV 가 반송 중인 작업(작업 상태 25 = SC 반송중, 35 = RGV 반송중)이 없을 때 한다. '
            '운전 화면 작업 목록에서 25/35 작업이 없는지 확인한 뒤 종료한다.',
            '반송 중에 끄면 설비 완료 신호를 놓쳐 작업이 멈춘 채 남을 수 있다. 완료 판정은 설비 신호로만 하며 시간 경과로 자동 완료하지 않는다.',
            '종료 순서는 기동의 역순(WCS_TASK_HOST -> IO_TASK -> WCS_TASK_CV)을 권장한다.',
            '실행 파일 교체 전 기존 파일을 백업(예: *.exe.bak_날짜)해 두면 즉시 되돌릴 수 있다.'])

d.add_heading('3. 설정 파일(INI)', 1)
para(d, '각 프로그램은 실행 파일과 같은 폴더의 INI 를 읽는다. 항목별 상세(키·기본값·현장값)는 12_INI_설정_정의서.docx 를 참조한다.')
table(d, ['프로그램', '설정 파일', 'DB 접속 섹션', '비고'], [
    ['WCS_TASK_CV', 'WCS_DB.INI', '[DB_2]', 'PLC 주소는 7_DeviceMap\\PlcAddressMap.xml 이 단일 기준(폴더째 배포)'],
    ['IO_TASK', 'ENV_IOSCH.INI', '[P_DB]', '스케줄 옵션 [CNF]'],
    ['WCS_TASK_HOST', 'EcsComA.ini', '[DB]', '상위 접속 IP/포트'],
    ['운전 화면', 'Ecs.ini', '[DB_2]', 'ODBC "SQL Server" 드라이버'],
    ['ALL_TASK', 'ALL_TASK.INI', '-', '[LOG] DIR/KEEP_DAYS, [AUTOSTART] EQP/IO/HOST']], widths=[3.2, 3.2, 3.0, 7.6])

d.add_heading('4. 현장 접속값', 1)
table(d, ['프로그램', '파일', '항목', '현장값'], [
    ['WCS_TASK_CV', 'WCS_DB.INI', '[COMM0] IP / CUR_PORT', '192.100.1.110 (마스터 PLC) / 2004'],
    ['WCS_TASK_HOST', 'EcsComA.ini', 'RemoteIP / LocalPort / RemotePort', '192.100.1.191 / 8001 / 8002'],
    ['TASK 3종', '각 INI DB 섹션', 'IP', '서버IP,1433'],
    ['운전 화면', 'Ecs.ini [DB_2]', 'SERVER / USERID / USERPASSWORD', '서버IP,1433 / LGLS_IO / (지정)']], widths=[3.5, 3.5, 5.5, 4.5])

d.add_heading('5. 서버(VS 미설치) 구동 요건', 1)
bullets(d, ['운전 화면(WCS_CLIENT\\Ecs.exe)은 Release 빌드다. 서버에는 Microsoft Visual C++ 2015-2022 Redistributable (x86) 를 설치한다 (mfc140u / vcruntime140 / msvcp140 / UCRT). FarPoint 스프레드(SPR32DU70.DLL)는 폴더에 들어 있다.',
            'TASK 3종(및 ALL_TASK)은 .NET Framework 4.8 로 돈다(IO/HOST 는 4.0 대상이지만 4.8 위에서 동작). Windows Server 2019 이상은 기본 포함, 없으면 .NET Framework 4.8 런타임 설치.',
            'DB 는 서버의 MS-SQL 을 쓰도록 각 INI 의 DB 접속 정보(IP,1433 / 계정)를 바꾼다. DB 이관은 02_DB_구축_정보 문서와 DB_BACKUP\\서버_구축_절차.md 를 따른다.'])

d.add_heading('6. 현장 DB 반영 SQL', 1)
para(d, '최종판 프로그램이 새로 쓰는 코드를 현장 DB 에 넣는다. 두 파일 모두 여러 번 실행해도 된다.')
table(d, ['순서', '파일 (산출물 폴더\\SQL)', '내용'], [
    ['1', '00_현장DB_공통코드_추가.sql', 'COMMON_CODE 2행 : JOB_STATUS 16 (통로CV 구동중), SC_ERR_STA_FK 3 (사용정지 DOWN). 이미 있으면 건너뜀'],
    ['2', '260917_알람리스트_EQP_ECD_MST.sql', 'PLC 알람 리스트(2026-09-17) : EQP_ECD_MST 의 SC_LGLS(크레인 지상반)·CV·RTV 알람 코드']], widths=[1.3, 5.7, 10])
para(d, '실행 : SSMS 에서 파일을 열고 F5, 또는  sqlcmd -S 서버IP,1433 -d LGLS_MCS_IO -U 계정 -P 암호 -f 65001 -i 파일명', size=9)
para(d, '운전 화면은 에러코드 마스터를 기동할 때 읽으므로 SQL 실행 후 Ecs.exe 를 다시 띄운다.', bold=True)

d.add_heading('7. 키오스크(운전 화면 전용 PC) 구동 요건', 1)
para(d, '키오스크에는 WCS_CLIENT 폴더만 복사한다. MS-SQL / .NET / Visual Studio 모두 필요 없다.')
numbered(d, ['EXE\\WCS_CLIENT 폴더를 통째로 복사 (Ecs.exe, DciLib.dll 등 DLL, 리소스, 폰트 포함)',
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
            '키오스크에는 TASK 를 두지 않는다. 화면·알림창·설비 대화상자 모두 서버 DB 만 보고 동작한다.',
            'Ecs.exe 를 교체할 때는 DciLib.dll 도 같은 판으로 함께 교체한다.'])

d.add_heading('8. 사전 설치 파일 (Prerequisites, Microsoft 공식 배포본)', 1)
table(d, ['파일', '용도', '비고'], [
    ['vc_redist.x86.exe', 'VC++ 2015-2022 재배포 패키지 x86 (운전 화면용)', '13MB'],
    ['ndp48-x86-x64-allos-enu.exe', '.NET Framework 4.8 오프라인 설치(인터넷 없는 서버)', '116MB'],
    ['ndp48-web.exe', '.NET Framework 4.8 웹 설치(인터넷 되는 서버)', '1.4MB']], widths=[5.5, 8, 3.5])
para(d, '설치 순서 : .NET 4.8 → vc_redist.x86 → (재부팅 요구 시 재부팅) → 각 프로그램 폴더 복사 → INI 수정 → 현장 DB 반영 SQL(6장) → 기동(2장).')
out = os.path.join(OUT, '01_설치_및_구동_안내서.docx'); d.save(out); print('saved', out)
