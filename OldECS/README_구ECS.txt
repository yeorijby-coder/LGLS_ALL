[구 ECS 실행 폴더] (2026-09-01 구성)

원본 : Backup\ECS\bin\Debug 을 Release(최적화)로 빌드한 산출물 (2026-09-01 교체. 종전 Debug 빌드가 느렸음) + INI\, Resources\
실행 : 구ECS_실행.bat  (또는 HECS.exe 직접)

접속 설정 (HECS.exe.config) :
  DB      : localhost\sqlexpress / LglsMCS   (신 WCS 의 LGLS_MCS_IO 와 별개 DB)
  PLC_IP  : 127.0.0.1 : 2004   ★EQP_SIM 과 같은 포트 - 신 WCS_TASK_CV 와 동시에
                                붙이면 경합한다. 구 ECS 시험 시 WCS_TASK_CV 를 내릴 것★
  WMS     : 9910 / ECS : 9911  (HOST_SIM 과 같은 포트 대역)

판단 기준 원본이므로 이 폴더의 파일은 수정하지 않는다 (복사본만 수정).
