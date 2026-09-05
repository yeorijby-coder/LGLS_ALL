[구 ECS 실행 폴더] (2026-09-01 구성)

원본 : Backup\ECS\bin\Debug 을 Release(최적화)로 빌드한 산출물 (2026-09-01 교체. 종전 Debug 빌드가 느렸음) + INI\, Resources\
실행 : 구ECS_실행.bat  (또는 HECS.exe 직접)

★기동 전 반드시 : EQP_SIM 을 먼저 띄운다★
  구 ECS 는 부팅 중(ECS 장비 Load 단계)에 PLC 127.0.0.1:2004 로 ★접속을 시도★ 하고,
  실패하면 "프로그램 기동에 실패 했습니다" 대화상자를 띄우고 종료한다.
  (오류 화면에는 DB 관련 스택만 보이지만 실제 원인은 소켓 연결 거부다.
   진짜 원인은 D:\LOG\ezMCS\Log\Statistics\RUN_TIME_EXCEPTION 로그에 남는다.)
  또 WMS(127.0.0.1:9910)가 없으면 기동 직후 스스로 종료하므로 HOST_SIM 도 함께 띄운다.

  기동 순서 : EQP_SIM → HOST_SIM → HECS.exe
  ※ WCS_TASK_CV 와 2004 포트를 공유하므로 구 ECS 시험 중에는 WCS_TASK_CV 를 내린다.

접속 설정 (HECS.exe.config) :
  DB      : localhost\sqlexpress / LglsMCS   (신 WCS 의 LGLS_MCS_IO 와 별개 DB)
  PLC_IP  : 127.0.0.1 : 2004   ★EQP_SIM 과 같은 포트 - 신 WCS_TASK_CV 와 동시에
                                붙이면 경합한다. 구 ECS 시험 시 WCS_TASK_CV 를 내릴 것★
  WMS     : 9910 / ECS : 9911  (HOST_SIM 과 같은 포트 대역)

판단 기준 원본이므로 이 폴더의 파일은 수정하지 않는다 (복사본만 수정).
