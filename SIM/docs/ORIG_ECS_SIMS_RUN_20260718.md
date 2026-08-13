# 구 ECS + SIMs 실구동 검증 (2026-07-18, Renewal 통합본)

## 결과: 3개 로직 입고+출고 풀사이클 완주 — 구 ECS ↔ SIMs 정상 동작 확인

체인: EQP_SIM(XGT 2004) ← HECS.exe(Renewal\Backup 빌드본) → HOST_SIM(9910/9911), DB=localhost\SQLEXPRESS LglsMCS

실측 시퀀스 (로직3 예):
O 입고지시(101→랙 070101) ACK → CV11 파렛트 JOB 부여 → 22→21 이동 → WAIT_OUT →
RGV(VEHICLE:1) 반송지시 P21→P15 → 상차/하차 핸드셰이크 → 라인CV 15→16 →
SC4(VEHICLE:14) P16→랙 070101 입고 저장 → **F1(입고완료) 수신** →
HOST_SIM 출고지시(랙→101) → SC4 랙 상차 → P14 하차 → 라인CV 14→13 →
RGV P13→P21 → CV11 21→22 → P22 지게차 배출 → **F2(출고 step2) 수신** → 다음 입고 자동 발행(사이클 반복).
로직1(CV12 입고→CV13 출고), 로직2(CV15 입고→CV14 출고 27→28→29)도 동일 완주.
M 전문(C/V#11 모드변경) ACK + 방향모드 동작, S 상태보고 주기 왕복, Ack 핸드셰이크/WAIT_OUT 재발화 정상.

## 이번에 해결한 문제들

1. **Renewal\Backup(구 ECS)은 소스만 통합되어 있었음** → LGLS_1WH_original에 적용돼 있던
   IP/포트 패치(App.config 127.0.0.1 + Program.cs PLC_IP/PORT 읽기 — 허용된 IP/포트 변경)만 동일 적용 후
   VS2019 MSBuild로 HECS.csproj 빌드. 백업: `Program.cs.pre_ipport_bak`, `App.config.pre_ipport_bak`.

2. **EQP_SIM 7/15 수정이 구 ECS 호환을 깨고 있었음** → `[MODE] TARGET=ECS|WCS` 스위치 신설:
   - CV12/13 입출고 배치: ECS 모드=원본(C/V#12=입고 24→23, C/V#13=출고 25→26) / WCS 모드=7/15 스왑(124=출고,126=입고)
   - WCS 재현/픽업 훅 3종(출고 재현·입고 RGV드롭 재현·크레인 픽업)과 D영역 미러: ECS 모드에서 비활성
     (구 ECS는 VehicleSim의 실제 TRANSFER_REQUEST 핸드셰이크로 차량 반송 — 훅이 파렛트를 선제 제거해 배차를 막았음)
   - 수정 파일: WorldModel.cs, ScenarioEngine.cs, ConveyorSim.cs, MainForm.cs, EQP_SIM.ini(+.origecs/.wcs_chain 변형)

3. **★핵심★ XgtServer 7/14 수정이 구 ECS의 D/R 읽기를 깨뜨림**: 연속읽기 주소를 무조건 `address*2`
   (WCS V0.9 워드주소 대응) → 구 ECS FenetDriver는 바이트주소(%DB/%RB)를 보내므로 R 트래킹 읽기가
   2배 주소로 밀려 빈 값 → OnWaitOut 조기 return → RGV 배차 불가.
   → 접미 문자로 구분: `'W'`(V0.9 워드주소, 바이너리 카운트)일 때만 ×2, `'B'`(구 ECS/V1.1 바이트주소)는 그대로.
   DATACOUNT도 'B'+b1≤99 → "0###" 10진분할(구 ECS), 그 외 바이너리LE. 쓰기는 페이로드 길이 일치 해석 채택.
   (EQP_TASK FenetProtocol은 실PLC 검증본이라 무수정 — 서버(SIM)를 맞춤)

4. **LglsMCS 이력 테이블 4종 부재** (`tb_errorhist/transferhist/pallethist/hirackhist`) →
   INSERT 실패가 예외를 삼키고 트랜잭션을 오염시켜 진짜 오류가 안 보였음 → 엔티티 INSERT문 기준으로 생성(DB 보강).
   이후 실제 오류가 tb_errorhist에 기록됨: "(Cell 010101) 입고 할 수 없는 Location" —
   지난 런 잔여로 TB_HIRACK 셀이 Reserved 고착. → Reserved 셀 3건 Empty 복구.

## 테스트 잔여물 정리 요령 (재현 시)
- TB_TRANSFER/TB_TRANSFERDETAIL 미완료 행 + 해당 TB_PALLET 행 삭제, TB_HIRACK Reserved → Empty
- EQP_SIM `Data\eqp_state.txt` 비우기 + `plc_memory.bin` 삭제 (셋 다 지워야 완전 클린)
- 기동: EQP_SIM → HOST_SIM → HECS.exe, HOST_SIM 첫 S 수신 후 [시작]

## 미변경 확인
- 구 ECS 소스: IP/포트 관련 2개 파일만(허용 범위), 그 외 무수정. DB는 이력 테이블 추가/시드만.
- EQP_TASK(WCS_TASK_CV) 통신 코드: 무수정.
