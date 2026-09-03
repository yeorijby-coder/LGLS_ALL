# 산출물 (2026-09-03)

| 파일 | 내용 | 원본 양식 |
|---|---|---|
| 01_설치_및_구동_안내서.docx | EXE/README.md 를 워드로. 서버·키오스크 구동 요건, 접속 설정, 화면 표시 스위치, 현재 INI 상태 부록 | EXE/README.md |
| 02_DB_구축_정보.docx | MS-SQL 기준 DB 구축 정보(계정·포트·백업·복원 절차) | DB구축 정보.doc (2010, Oracle) |
| 03_테이블_정의서.docx | LGLS_MCS_IO 38 테이블 컬럼 정의(현 스키마 자동 추출 + 설명) | - |
| 04_WCS_업무_흐름도.docx | 시스템 구성, 기준정보, 입/출고/피킹/반자동 흐름, 상위 I/F, 예외 처리 | WMS 업무 흐름도_20100414.doc |
| 05_WCS_운전_지침서.docx | 화면별 메뉴 PATH/개요/화면/특기사항/화면설명 + 예외 처리 | WMS 지침서_20100331.doc |
| 06_WCS_화면설계서.pptx | 표지/Document history/Contents/메뉴 표시 상태/화면별 캡처+설명 (30장) | 일반비_LG화학WMS화면설계서_V1.1.pptx |

- 캡처(`shots/`)는 2026-09-03 시뮬레이터 순환 중, Ecs.ini 의 표시/숨김 설정 그대로 찍은 것.
- `gen/` 의 파이썬(python-docx / python-pptx)으로 다시 만들 수 있다 : `python gen_*.py` (스키마는 sqlcmd 로 추출한 schema.txt / tables.txt 사용).
- 숨김 화면(사용자·INI 열기·공PLT·제품정보·UI모드·반자동 TEST)과 자동 캡처가 되지 않은 RGV 상태창은 설명으로 실었다.
