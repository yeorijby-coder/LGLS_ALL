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
