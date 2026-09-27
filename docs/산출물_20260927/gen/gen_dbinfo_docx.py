# -*- coding: utf-8 -*-
"""DB구축 정보.doc (Oracle, 2010) -> 현재 시스템(MS-SQL) 기준 DB 구축 정보
   1.2(09-09) 현행 스키마 재추출, 1.3(09-11) 미사용 표 12개 정리 반영.
   2026-09-21 최종판 : DB_BACKUP 실제 목록 갱신, 현장 DB 반영 SQL(00/260917) 적용 절차 추가."""
import os
from gen_common import *

d = new_doc('DB 구축 정보', 'LGLS_MCS_IO (Microsoft SQL Server)',
            history=[['1.0', '2026-09-03', AUTHOR, '최초 작성'],
                     ['1.1', '2026-09-04', AUTHOR, '검토 의견 반영'],
                     ['1.2', '2026-09-09', AUTHOR, '현행 스키마 재추출 반영'],
                     ['1.3', '2026-09-11', AUTHOR, '미사용 표 12개 정리 반영']])
d.add_heading('1. DB 구축 정보', 1)
para(d, '구 WMS(2010, Oracle) 의 "DB구축 정보" 양식을 현재 ECS 시스템의 MS-SQL 기준으로 다시 정리한 것이다. '
        '개발 PC 값은 현재 시험 환경, 서버 값은 이관 시 정할 값(공란은 구축 시 기입).', color=GRAY)
rows = [
 ['1', 'DB 종류', 'Microsoft SQL Server', '개발 PC : SQL Server Express (localhost\\SQLEXPRESS). 서버 : Standard 이상 권장(Express 도 동작)'],
 ['2', '데이터베이스', 'LGLS_MCS_IO', 'WCS 운전 DB (작업/설비/로그). 정리 후 백업 약 4.8MB'],
 ['3', '데이터 파일', 'LGLS_MCS_IO.mdf', '서버 기본 DATA 폴더 (예: C:\\Program Files\\Microsoft SQL Server\\MSSQLxx\\MSSQL\\DATA)'],
 ['3', '로그 파일', 'LGLS_MCS_IO_log.ldf', '복구 모델 : SIMPLE 권장(로그 테이블 트래픽 많음)'],
 ['4', '백업 파일(최신)', 'LGLS_MCS_IO_20260907_clean.bak', 'DB_BACKUP 폴더(리포 루트). 시험 데이터를 비운 배포용 정리본(약 4.6MB, 09-07). 백업 안의 DB 이름은 LGLS_MCS_IO_DEPLOY 이므로 복원 시 대상 이름을 LGLS_MCS_IO 로 지정한다'],
 ['4', '백업 파일(이전판)', 'LGLS_MCS_IO_20260904_clean.bak / _20260902_clean.bak', 'DB_BACKUP 폴더. 정리본 이전 판(약 4.6MB). 보관용, 복원에는 최신판 사용'],
 ['4', '백업 파일(전체)', 'LGLS_MCS_IO_20260902.bak', 'DB_BACKUP 폴더. 운전 이력까지 포함한 전체 백업(약 611MB, 09-02). 이력이 필요할 때만 쓴다'],
 ['4', '백업 파일(구 WMS)', 'LglsMCS_20260902.bak', 'DB_BACKUP 폴더. 구 WMS(Oracle 이관 전 MS-SQL) 백업(약 2.8MB). 참고용, 현 ECS 는 사용 안 함'],
 ['4', '표 정리 스크립트', '00~04 (컬럼정리), 10~13 (표삭제)', 'DB_BACKUP 폴더. 09-10 미사용 표 12개 삭제 작업 시 사용한 점검/삭제/복구 스크립트. 정상 배포 시에는 실행 불필요(이미 반영됨)'],
 ['5', '인증 방식', 'SQL Server 및 Windows 인증(혼합)', '서버 속성 > 보안 에서 변경 후 SQL 서비스 재시작'],
 ['6', 'WCS 접속 계정', 'LGLS_IO / LGLS_IO', '운전 화면(Ecs.ini [DB_2])이 쓰는 계정. TASK 는 각 INI [DB] 의 계정'],
 ['6', '권장 전용 계정', 'mcsuser / (지정)', 'CREATE LOGIN ... CHECK_POLICY=OFF; db_owner 부여. sa 는 사용하지 않고 잠금 권장'],
 ['6', 'User Role', 'db_owner', 'LGLS_MCS_IO'],
 ['7', '서비스명/인스턴스', 'MSSQLSERVER 또는 SQLEXPRESS', 'IP : (서버 IP 기입)'],
 ['8', 'PORT', '1433 (TCP 고정)', 'SQL Server 구성 관리자 > TCP/IP > IPAll : 동적 포트 비움, TCP 포트 1433. 방화벽 인바운드 허용'],
 ['9', '접속 문자열(운전 화면)', 'DRIVER=SQL Server; SERVER=서버IP,1433', 'Windows 내장 ODBC 드라이버. Ecs.ini [DB_2]'],
 ['9', '접속 문자열(TASK)', 'IP=서버IP,1433 (각 INI [DB])', 'WCS_DB.INI / ENV_IOSCH.INI / EcsComA.ini'],
 ['10', 'sa 계정', 'sa / (관리자 보관)', '비밀번호 분실 시 : 서버 로컬 관리자로 단일 사용자 모드(-m) 기동 후 ALTER LOGIN sa WITH PASSWORD'],
 ['11', '정리 정책', 'DEL_HIS_SETTING', '운전 화면 [로그 삭제 설정] 에서 로그 테이블별 보존 주기(일) 설정'],
 ['12', '로그 테이블', 'WCS_LOG_PGR, HOST_IF_LOG, JOB_MST_HIS, WCS_CLIENT_LOG, ALL_TASK_LOG', '누적량이 큰 테이블. 배포용 정리본은 이 테이블들을 비운 상태다'],
]
table(d, ['순번', '항목', '정보', '설 명'], rows, widths=[1.2, 3.6, 5.2, 8.0], font=8.5)
d.add_heading('2. 복원 절차 요약', 1)
numbered(d, ['백업 파일을 서버 로컬 경로로 복사 (SQL 서비스 계정이 읽을 수 있는 경로, 네트워크 경로 지양)',
             'SSMS > 데이터베이스 > 복원 : LGLS_MCS_IO_20260907_clean.bak - 대상 데이터베이스 이름을 LGLS_MCS_IO 로 입력하고, 파일 경로는 서버 DATA 폴더로 재배치',
             '혼합 인증으로 변경 후 서비스 재시작, 전용 로그인/사용자 생성(db_owner)',
             'TCP 1433 고정, 방화벽 허용, 클라이언트 PC 에서 sqlcmd -S 서버IP,1433 -U 계정 으로 접속 확인',
             '각 프로그램 INI 의 [DB] 접속 정보 변경 (설치 및 구동 안내서 참조)'])
d.add_heading('3. 현장 DB 반영 SQL 적용 절차', 1)
para(d, '최종판 프로그램(에러코드 마스터 확장, 통로CV 구동중 상태 추가)이 새로 참조하는 코드를 현장 DB 에 넣는 절차다. '
        '두 파일 모두 산출물 폴더\\SQL 에 있으며, 이미 있는 코드는 건너뛰도록(존재 확인 후 INSERT) 작성되어 있어 여러 번 실행해도 안전하다.')
table(d, ['순서', '파일', '대상 테이블', '내용'], [
    ['1', '00_현장DB_공통코드_추가.sql', 'COMMON_CODE', 'JOB_STATUS 16 (통로CV 구동중), SC_ERR_STA_FK 3 (사용정지 DOWN) 2행 추가'],
    ['2', '260917_알람리스트_EQP_ECD_MST.sql', 'EQP_ECD_MST', '2026-09-17 PLC 알람 리스트 : SC_LGLS(크레인 지상반)·CV·RTV 에러코드/명칭']], widths=[1.3, 6.2, 3.5, 8.0], font=8.5)
numbered(d, ['서버 DB 접속 정보로 sqlcmd 또는 SSMS 준비 (예 : sqlcmd -S 서버IP,1433 -d LGLS_MCS_IO -U LGLS_IO -P LGLS_IO -f 65001 -i 파일명, -f 65001 은 UTF-8 BOM 파일을 올바르게 읽기 위한 코드페이지 지정)',
             '00_현장DB_공통코드_추가.sql 실행 - COMMON_CODE 에 두 코드가 있는지 확인 후 실행(멱등)',
             '260917_알람리스트_EQP_ECD_MST.sql 실행 - EQP_ECD_MST 에 알람코드 반영',
             '운전 화면(Ecs.exe)은 에러코드 마스터를 기동 시점에 읽으므로, SQL 실행 후 운전 화면을 재기동해야 반영된다. TASK 3종은 재기동 불필요'])
para(d, '※ 새 DB 를 처음 구축하는 경우(신규 구축 DDL 로 스키마부터 만드는 경우)에도 위 두 SQL 은 마지막에 실행한다(공통코드/설비마스터 데이터 보강).', color=GRAY, size=9)

d.add_heading('4. 참고', 1)
bullets(d, ['서버 SQL 버전은 개발 PC(SQL Server Express) 이상이어야 복원 가능'])
out = os.path.join(OUT, '02_DB_구축_정보.docx'); d.save(out); print('saved', out)
