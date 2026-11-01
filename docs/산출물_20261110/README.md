# 산출물 (2026-11-10) - 종료 문서 네 건

2026-11-10 현장 적용(`적용_20261110`)에 맞춘 **프로젝트 종료용 문서**다.
09-27 판 이후 10월~11월 변경분(런처/Upload 배포, 작업정보 판넬 3칸 + 스텝 단위 완료처리,
경로 탐색, 이력 자동 삭제, 출고대 펄스 래치, 4K 배율 등)을 모두 합쳐 다시 묶었다.
09-21·09-27 판의 다른 문서(01~13)는 그대로 유효하다.

| 파일 | 내용 | 생성기 | 바탕 |
|---|---|---|---|
| **01_ECS_운영자_매뉴얼.docx** | **신규.** 시스템 구성 · 설치 · 기동/정지 · 서버 프로그램 4화면 · INI 주요 키 · DB 운영(복원/백업/현장 SQL/이력 자동 삭제/사후 확인) · 배포(Upload/런처, 꾸러미 규약) · 로그 · 장애 대응 · 점검표 · 적용 이력 | `gen/gen_opmanual_docx.py` | 09-21 의 01 설치 안내서 · 02 DB 구축 정보 · 12 INI 정의서 요약 + 10월 적용 안내서 |
| **02_ECS_사용자_매뉴얼.docx** | 운전 화면을 쓰는 순서대로. 런처 기동 · 메인 화면/판넬 3칸/단계표 · 신호등 · 상태창 [확대] · 자주 하는 일(우선순위 ▲▼, **[완료처리]** 스텝 단위 + 경로 탐색 + 4 상황) · 이상 대처 · 작업대 표 · 4K 배율 | `gen/gen_usermanual_docx.py` | 09-28 의 13 사용자 매뉴얼 |
| **03_테이블_정의서.docx** | LGLS_MCS_IO **28 표** 전 컬럼(2026-11-02 추출). 추가 표 DN_MST/DN_SEQ/UP_DOWN, 추가 열 CV_DATA.RET_READY_LATCH*, 트리거 · 뷰 · 인덱스, DEL_HIS_SETTING 보존 정책, 상태 코드 | `gen/gen_tabledef_docx.py` (`gen/schema_1102.txt`, `tables_1102.txt`, `coldesc.py`) | 09-21 의 03 (Rev4, 25표) |
| **04_인터페이스_정의서.docx** | **신규(가로).** 상위 WMS/IMS 전문(헤더 15 + O66/R31/M7/D/응답11/S43/E23/F10, 사유 코드 20, 스테이션 변환) · 설비 PLC(디바이스 규약, C/V·S/C·RGV 블록 주소·신호, 공통 알람 비트, 핸드셰이크, 시나리오 6, 설비 배치) · 프로그램 간 DB 인터페이스(상태 기계, _RD/_OD) · 운전 화면↔DB · 변경 이력 | `gen/gen_interface_docx.py` | WCS_TASK_HOST 소스, PlcAddressMap.xml v1.4 |

## 생성

```
cd docs\산출물_20261110\gen
python gen_opmanual_docx.py
python gen_usermanual_docx.py
python gen_tabledef_docx.py
python gen_interface_docx.py
```

- `gen/common1110.py` 가 `docs/산출물_20260927/gen/gen_common.py` 를 그대로 가져와 출력 폴더 · 캡처 폴더 · 날짜(2026-11-10) · 판(3.0)만 바꾼다.
- 캡처 `shots/` = 09-27 `shots/` 전부 + `적용_20261110/문서/shots_1029` + `적용_20261015/문서/shots_1014` · `shots_upload` + 메인 화면 `main_1029.png`.
- 스키마를 다시 뽑으려면 (sqlcmd, *_BAK 제외) :
  `schema_1102.txt` = INFORMATION_SCHEMA.COLUMNS + PK 표시 `TABLE_NAME|ORDINAL_POSITION|COLUMN_NAME|DATA_TYPE|LEN|IS_NULLABLE|DEF|PK`,
  `tables_1102.txt` = `tbl|cols|rows_`.

## 2026-11-10 적용분과의 관계

현장에 들고 가는 파일(exe · SQL · 적용 안내서 · 사후 확인 방법)은 `적용_20261110/` 에 있다.
이 폴더의 네 문서는 **인수인계 · 종료용**이며 적용 절차 자체는 `적용_20261110/문서/2026-10-29_적용안내서_추가분.docx` 와 `읽어보세요.txt` 를 따른다.
