# -*- coding: utf-8 -*-
"""DB구축 정보.doc (Oracle, 2010) -> 현재 시스템(MS-SQL) 기준 DB 구축 정보"""
import os
from gen_common import *

d = new_doc('DB 구축 정보', 'LGLS_MCS_IO (Microsoft SQL Server)')
d.add_heading('1. DB 구축 정보', 1)
para(d, '구 WMS(2010, Oracle) 의 "DB구축 정보" 양식을 현재 WCS 시스템의 MS-SQL 기준으로 다시 정리한 것이다. '
        '개발 PC 값은 현재 시험 환경, 서버 값은 이관 시 정할 값(공란은 구축 시 기입).', color=GRAY)
rows = [
 ['1', 'DB 종류', 'Microsoft SQL Server', '개발 PC : SQL Server Express (localhost\\SQLEXPRESS). 서버 : Standard 이상 권장(Express 도 동작)'],
 ['2', '데이터베이스', 'LGLS_MCS_IO', 'WCS 운전 DB (작업/설비/로그). 정리 후 백업 약 4.8MB'],
 ['2', '데이터베이스', 'LglsMCS', '구 ECS(HECS) 참조 DB - 구 ECS 를 병행 운용할 때만 복원'],
 ['3', '데이터 파일', 'LGLS_MCS_IO.mdf', '서버 기본 DATA 폴더 (예: C:\\Program Files\\Microsoft SQL Server\\MSSQLxx\\MSSQL\\DATA)'],
 ['3', '로그 파일', 'LGLS_MCS_IO_log.ldf', '복구 모델 : SIMPLE 권장(로그 테이블 트래픽 많음)'],
 ['4', '백업 파일', 'LGLS_MCS_IO_20260902_clean.bak', 'DB_BACKUP 폴더. 로그 테이블 비운 정리본(4.8MB). 원본 641MB 는 LGLS_MCS_IO_20260902.bak'],
 ['4', '백업 파일', 'LglsMCS_20260902.bak', '구 ECS DB'],
 ['5', '인증 방식', 'SQL Server 및 Windows 인증(혼합)', '서버 속성 > 보안 에서 변경 후 SQL 서비스 재시작'],
 ['6', 'WCS 접속 계정', 'LGLS_IO / LGLS_IO', '운전 화면(Ecs.ini [DB_2])이 쓰는 계정. TASK 는 각 INI [DB] 의 계정'],
 ['6', '권장 전용 계정', 'mcsuser / (지정)', 'CREATE LOGIN ... CHECK_POLICY=OFF; db_owner 부여. sa 는 사용하지 않고 잠금 권장'],
 ['6', 'User Role', 'db_owner', 'LGLS_MCS_IO (구 ECS 병행 시 LglsMCS 에도 부여)'],
 ['7', '서비스명/인스턴스', 'MSSQLSERVER 또는 SQLEXPRESS', 'IP : (서버 IP 기입)'],
 ['8', 'PORT', '1433 (TCP 고정)', 'SQL Server 구성 관리자 > TCP/IP > IPAll : 동적 포트 비움, TCP 포트 1433. 방화벽 인바운드 허용'],
 ['9', '접속 문자열(운전 화면)', 'DRIVER=SQL Server; SERVER=서버IP,1433', 'Windows 내장 ODBC 드라이버. Ecs.ini [DB_2]'],
 ['9', '접속 문자열(TASK)', 'IP=서버IP,1433 (각 INI [DB])', 'WCS_DB.INI / ENV_IOSCH.INI / EcsComA.ini'],
 ['10', 'sa 계정', 'sa / (관리자 보관)', '비밀번호 분실 시 : 서버 로컬 관리자로 단일 사용자 모드(-m) 기동 후 ALTER LOGIN sa WITH PASSWORD'],
 ['11', '정리 정책', 'DEL_HIS_SETTING', '운전 화면 [로그 삭제 설정] 에서 로그 테이블별 보존 주기(일) 설정'],
 ['12', '로그 테이블', 'WCS_LOG_PGR, HOST_IF_LOG, JOB_MST_HIS, WCS_CLIENT_LOG, EQP_ERR_HIS', '이관 전 TRUNCATE 하여 용량 축소(cleanup_20260902.sql)'],
 ['13', '폐기 테이블', '22개 (cell_mes_req, dn_mst, if_* , rtv_sch_* 등)', '전 소스 전수조사로 미참조 확인 후 DROP (cleanup_20260902.sql)'],
]
table(d, ['순번', '항목', '정보', '설 명'], rows, widths=[1.2, 3.6, 5.2, 8.0], font=8.5)
d.add_heading('2. 복원 절차 요약', 1)
numbered(d, ['백업 파일을 서버 로컬 경로로 복사 (SQL 서비스 계정이 읽을 수 있는 경로, 네트워크 경로 지양)',
             'SSMS > 데이터베이스 > 복원 : LGLS_MCS_IO_20260902_clean.bak (파일 경로는 서버 DATA 폴더로 재배치)',
             '혼합 인증으로 변경 후 서비스 재시작, 전용 로그인/사용자 생성(db_owner)',
             'TCP 1433 고정, 방화벽 허용, 클라이언트 PC 에서 sqlcmd -S 서버IP,1433 -U 계정 으로 접속 확인',
             '각 프로그램 INI 의 [DB] 접속 정보 변경 (설치 및 구동 안내서 참조)'])
d.add_heading('3. 참고', 1)
bullets(d, ['상세 절차 : DB_BACKUP\\서버_구축_절차.md', '정리 스크립트 : DB_BACKUP\\cleanup_20260902.sql',
            '서버 SQL 버전은 개발 PC(SQL Server Express) 이상이어야 복원 가능'])
out = os.path.join(OUT, '02_DB_구축_정보.docx'); d.save(out); print('saved', out)
