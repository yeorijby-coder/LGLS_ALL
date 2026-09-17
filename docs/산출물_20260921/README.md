# 산출물 (2026-09-21) - 최종판 Ver 2.0

현장 기본 시험을 마친 상태(2026-09-17)를 기준으로 지금까지 만든 산출물을 모두 다시 정리한 판이다.
같은 날 미사용 설정·소스를 정리했고(커밋 `2331c87`), PLC 알람 리스트를 적용했다(커밋 `3269fc7`).

| 파일 | 내용 | 이전 판 |
|---|---|---|
| 01_설치_및_구동_안내서.docx | 실행본 구성(개별 TASK·ALL_TASK·Client), 접속 설정, 기동·교체 절차, DB 반영 SQL | 09-04 |
| 02_DB_구축_정보.docx | MS-SQL 구축·백업·복원, **현장 DB 반영 SQL 적용 절차** | 09-11 |
| 03_테이블_정의서.docx | LGLS_MCS_IO 25 테이블. EQP_TYP 구분(SC_LGLS 추가), 작업상태 16 반영 | 09-11 |
| 04_ECS_업무_흐름도.docx | 작업상태 전이(현행), C/V#2 통로 사용권, 겸용대 보호, 알람 흐름 | 09-04 (04_WCS_…) |
| 05_ECS_운전_지침서.docx | 화면별 사용법 + 예외 처리(알람 확인, 통로 방향 수동 전환) | 09-04 (05_WCS_…) |
| 06_ECS_화면설계서.pptx | **09-17 사용자 수정본 그대로** (아래 참고) | 09-17 |
| 07_ECS_구성도_및_흐름도.pptx | 구성도·입출고 흐름 + 통로 사용권·겸용대·알람 장 | 09-05 (07_WCS_…) |
| 08_이중입고_공출고_시험_보고서.docx | 09-05 시험 기록 + 2.0 개정 안내(이 현장 미사용, 코드 73/74/75) | 09-05 |
| 09_함수단_흐름도.pptx | 현행 함수 이름 기준 호출 흐름(사이클, 사용권, 방향 지시, 알람) | 09-05 |
| 10_테이블_사용현황_조사표.docx | 표 37개 조사 → 12개 삭제 기록 (내용 동일, 재발행) | 09-11 |
| **11_알람코드_정의서.docx** | 신규. PLC 알람 리스트 → EQP_ECD_MST 코드표, 흐름, 설정, 확인 요청 | - |
| **12_INI_설정_정의서.docx** | 신규. 4개 INI 전 키(현재값·기본값·반영 시점·읽는 코드), 불일치 목록 | - |
| SQL\00_현장DB_공통코드_추가.sql | JOB_STATUS 16, SC_ERR_STA_FK 3 (있으면 건너뜀) | - |
| SQL\260917_알람리스트_EQP_ECD_MST.sql | 알람 코드 86건(SC_LGLS 58 / CV 13 / RTV 15), 여러 번 실행 가능 | - |
| SQL\260917_1동 자동창고_알람리스트.xlsx | PLC 담당 원본 | - |
| SQL\1호기_출고_지연분석.sql | 현장 분석 쿼리 (SQL Server 2008 호환) | 09-16 |
| 참고\ | 현장점검 보고서(09-15, 09-17), 전체시스템·반자동 시험 수행계획서 V1.0 | - |

## 화면설계서(06)

사용자 요청(2026-09-17)으로 이번 판에서는 갱신하지 않았다. Client 를 더 손본 뒤 요청하면 만든다.
그때는 이 폴더의 `06_ECS_화면설계서.pptx`(09-17 사용자 수정본 사본)를 제자리에서 고친다.
`gen\edit_screen_pptx_v20.py` 가 버전 표기·이력 행·알람 문구 설명·캡처 교체(`shots\*_v20.png`)를 해 두는 초안이다.

## 현장 적용 순서

1. `SQL\00_현장DB_공통코드_추가.sql` → `SQL\260917_알람리스트_EQP_ECD_MST.sql` 실행
2. 크레인·RTV 반송 중(25/35) 작업이 없을 때 서버 프로그램 교체 (`EXE\TASK\*`), Client 교체(`Ecs.exe` + `DciLib.dll` 등)
3. 현장 INI 는 12번 문서 7장 점검표로 확인. 특히 `ENV_IOSCH.INI` 의 `CV2_DIR_MODE / CV2_STALL_SEC / CV2_FAIR` 가 **[CNF] 섹션 안**에 있는지

## 다시 만들기

```
cd gen
python gen_readme_docx.py        # 01
python gen_dbinfo_docx.py        # 02
python gen_tabledef_docx.py      # 03 (schema.txt / tables.txt)
python gen_flow_docx.py          # 04
python gen_guide_docx.py         # 05 (gen_screens.py, shots\)
python gen_flowdeck.py           # 07
python gen_errtest_report.py     # 08 (err_test_result.txt)
python gen_funcflow.py           # 09
python gen_table_survey.py       # 10
python gen_alarm_sql.py          # SQL (docs\sql 과 이 폴더 SQL\ 에 같이 씀)
python gen_alarm_docx.py         # 11
python gen_ini_docx.py           # 12 (실제 INI 파일을 읽는다)
```

`gen_common.py` 가 날짜(2026-09-21)·버전(2.0)·출력 폴더(이 폴더)를 정한다.
