# 산출물 (2026-09-09) - DB 문서 갱신분

2026-09-04 판에서 **DB 관련 2건만** 현행 스키마로 다시 만든 것이다.
나머지 문서(01, 04~09)는 `docs/산출물_20260904/` 를 그대로 쓴다.

| 파일 | 내용 |
|---|---|
| 02_DB_구축_정보.docx | MS-SQL 기준 DB 구축 정보(계정·포트·백업·복원 절차) |
| 03_테이블_정의서.docx | LGLS_MCS_IO **28 테이블** 컬럼 정의 (현 스키마 자동 추출 + 설명) |

## 09-04 판에서 바뀐 점

- **`ALL_TASK_LOG` 테이블 추가** (27 → 28 테이블).
  ALL_TASK(EQP/IO/HOST 통합 프로그램)가 남기는 스레드별 운전 로그다.
  같은 내용이 `LOG\<스레드>\<스레드>_yyyyMMdd.log` 파일로도 쌓이고,
  보관기간(기본 365일)이 지나면 파일과 DB 행이 함께 자동 삭제된다.
- 행수는 2026-09-09 실측값으로 갱신.
- 그 밖의 테이블·컬럼 구조 변경은 없다.

## 다시 만들기

```
cd gen
python gen_dbinfo_docx.py
python gen_tabledef_docx.py
```

스키마는 `gen/schema.txt` · `gen/tables.txt` (sqlcmd 추출본)를 쓴다. 다시 뽑으려면:

```
sqlcmd -S localhost\SQLEXPRESS -d LGLS_MCS_IO -E -W -s "|" -Q "SET NOCOUNT ON; SELECT c.TABLE_NAME, c.ORDINAL_POSITION, c.COLUMN_NAME, c.DATA_TYPE, ISNULL(CONVERT(varchar(10), c.CHARACTER_MAXIMUM_LENGTH),'') AS LEN, c.IS_NULLABLE, ISNULL(c.COLUMN_DEFAULT,'') AS DEF, ISNULL((SELECT TOP 1 'Y' FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE k JOIN INFORMATION_SCHEMA.TABLE_CONSTRAINTS t ON k.CONSTRAINT_NAME=t.CONSTRAINT_NAME AND t.CONSTRAINT_TYPE='PRIMARY KEY' WHERE k.TABLE_NAME=c.TABLE_NAME AND k.COLUMN_NAME=c.COLUMN_NAME),'') AS PK FROM INFORMATION_SCHEMA.COLUMNS c ORDER BY c.TABLE_NAME, c.ORDINAL_POSITION" > gen\schema.txt
```

DB 이관 절차는 `DB_BACKUP/서버_구축_절차.md` 를 본다(2026-09-09 갱신).

---

## 06_ECS_화면설계서.pptx (2026-09-09 추가)

화면 캡처를 오늘 상태로 다시 찍고, 오늘 바뀐 내용을 반영했다. 총 26 슬라이드 / 23 화면.

09-04 판에서 바뀐 점

- **명칭 ECS 확정** — 표지·푸터·문서 이력 표기를 ECS 로. 실행파일 아이콘도 ECS 로 새로 만들었다.
- **메인 화면** : 범례가 레이아웃 안에 표로 들어갔다(왼쪽). [범례] 창에서 색을 바꾸면 이 표에 즉시 반영된다.
  크레인별 "출고대기 N" 은 `Ecs.ini [MENU] SCWAIT_VIEW=0` 으로 끌 수 있다.
- **범례** : S/C·RTV 레일 색이 정지 사유별로 나뉜다(입고금지·출고금지·입출고정지·에러·작업중).
- **[판넬 보기] 신설** : 종전 [UI모드](대화상자/판넬 2버튼)를 대체. 작업정보·상세정보·설비반송 판넬을 각각 켜고 끈다.
  작업정보는 상단 전폭, 나머지 둘은 오른쪽 위아래.
- **RTV 상태** 화면 캡처 추가(종전에는 캡처 없이 설명만).
- **통합 태스크(ALL_TASK)** 항목 추가 — 설비통신/스케줄러/상위통신을 한 창에서 기동·정지한다.
  개별 태스크 화면은 ALL_TASK 의 탭으로 안내한다.

캡처는 `shots/` 에 있다. 다시 만들려면 `cd gen && python gen_screen_pptx.py`.

---

## 2026-09-10 개정 - 미사용 컬럼·표 정리

이 현장에서 쓰지 않는 컬럼 51개와 표 12개를 DB 에서 지우고, 그에 맞춰 프로그램과 문서를 함께 정리했다.

| 파일 | 내용 |
|---|---|
| 03_테이블_정의서_Rev3.docx | 미사용 컬럼 51개 삭제 반영 (cv_data 39 / job_mst 6 / job_mst_his 6) |
| **03_테이블_정의서_Rev4.docx** | **미사용 표 12개까지 삭제 반영. 남은 25개 표 전부 수록(문서와 DB 가 1:1)** |
| 테이블_사용현황_조사표.docx | Rev1 - 조사 결과(삭제 권고 7 / 판단 필요 5 / 유지 25) |
| **테이블_사용현황_조사표_Rev2.docx** | **판단 필요 5건도 폐지 결정. 삭제 실행·회귀시험 기록 포함** |

지운 표 12개

```
auto_sc_work  bcr_mst   client_pgr_nm  dest_def   event_log   host_empty_plt
mes_if_log    rtv_data  sc_data        wait_track wc_data     wc_his
```

함께 정리한 프로그램

- **Client** : WC / BCR / 공PLT / WC로그 화면과 설비 클래스를 걷어냈다.
  지운 소스는 `Ecs/_removed_20260910/` 에 보관. 공PLT 는 메뉴 항목과 `Ecs.ini [MENU]` 키까지 제거(숨김이 아니라 폐지).
  HOST 로그 창 제목의 테이블명을 `[MES_IF_LOG]` → `[HOST_IF_LOG]` 로 정정(이 화면은 원래 HOST_IF_LOG 를 조회한다).
- **WCS_TASK_HOST** : 무게보고(`GetWeightReport`) / 빈파렛트 요청(`GetEmptyPltRequest`) / `UpdateHostEmptyPlt` 제거.
  세 기능 모두 호출부가 이미 주석 처리돼 있었다.
- 신규 구축 DDL(`TASK/IO_TASK/DB/mssql_schema.sql`)과 시뮬레이터 시드에서도 뺐다.

SQL 스크립트는 `DB_BACKUP/` 에 있다. 현장은 백업·복원을 할 수 없으므로 점검·삭제·재점검·복구를 짝으로 두었다.

```
01_컬럼정리.sql   02_컬럼복구.sql    (컬럼)
10_점검_표삭제전.sql  11_표삭제.sql  12_점검_표삭제후.sql  13_표복구.sql   (표)
```

회귀 시험 2026-09-10 11:23~11:28 - 5개 프로그램 재빌드 후 전체 기동, 입출고 풀사이클 5분 연속 운전.
작업 이력 124행 증가, 진행 중 작업 3건 정상, CV/HOST/SCH 하트비트 정상, 새 크래시 리포트·조회 오류 없음.

다시 만들기

```
cd gen
python gen_tabledef_docx_rev4.py
python gen_table_survey_rev2.py
```
