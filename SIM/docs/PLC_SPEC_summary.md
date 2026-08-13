# LG생명과학 ECS PLC 통신 사양 (PPT V0.9, 2009) 요약

원문: docs/PLC_SPEC_PPT_full.txt (ECS-분석-PLC사양및시나리오_LG생명과학_V0.9.ppt, 26슬라이드).
구성: ECS PC ↔ **Master PLC(=C/V PLC)** ETHERNET(사설망), Master PLC ↔ S/C#1~5. WMS ↔ ECS ETHERNET.
설비: C/V #1~15, S/C #1~5, RGV #1. 실온창고 6*10*14, 냉동창고 4*10*14.

## 통신 규칙(Command Sequence)
- **Event(PLC→ECS)**: PLC가 Status Bit/Word Set(Event Trigger) → ECS가 Read → ECS Event Ack SET → PLC가 Event Reset.
- **Command(ECS→PLC)**: ECS가 Cmd Bit/Word Write + Cmd Strobe Set → PLC Enable Check → PLC가 Cmd Start 시 Strobe Reset.
- Exist Bit 변경 후 ECS가 0.5초 이내 읽으므로 PLC는 Data Move 무결성 보장.

## M(Bit) 영역 — C/V (설비당 16bit=1word 블록, +0x10)
C/V#1=M0160, #2=M0170, ... #11=**M0360**, #15=M0470.
- +0: Event / Pallet Exist #(2n-1)  (홀수 트랙)
- +1: Unload Complete#1 / Pallet Exist #(2n) (짝수 트랙)
- +2: Load Complete#1
- +3: Unload Complete#2
- +4: Load Complete#2
- +5: WI(Wait In)  +6: WO(Wait Out)  +7: InReady#2  +8: Op Mode
- C/V#14 특수: +2에 #29 PalletExist, C/V#15 +0 #31,+1 #32,+2 #30 등(피킹존).

## M(Bit) 영역 — S/C, RGV (설비당 16bit, +0x10)
S/C#1=M0480,#2=M0490,...#5=M0520, RGV#1=**M0580**.
- +0: Pallet Presence(status) / Load Complete? (Status/Event 두 그룹). 실제: +0 Pallet Presence, Load Complete
- +1: Unload Complete  +2: Alarm Set Report  +3: Alarm Reset Report  +4: Transfer Complete

## M(Bit) 영역 — Ack & Command (C/V)
Conveyor#1=M0800,...#11=**M0900**,#15=M0940.
- +1: Unload Complete Ack#1  +2: Load Complete Ack#1  +3: Unload Complete Ack#2  +4: Load Complete Ack#2  +5: Unload Request#2

## M(Bit) 영역 — Ack & Command (S/C, RGV)
S/C#1=M0960,...#5=M1000, RGV#1=**M1010**.
- +0: Transfer(From/To)  +1: Load Complete Ack  +2: Unload Complete Ack  +3: Alarm Set Report Ack  +4: Alarm Reset Report Ack  +5: Transfer Complete Ack

## D(Word) 영역 — PLC 상태 (S/C, RGV)
S/C#1=D0160,...#5=D0200, RGV#1=**D0210**, Direction Value D0310.
- +0: Run(=?)/Idle/Down 상태값 (RGV: IDLE=1, RUN=2)
- +1: Set Alarm Code  +2: Reset Alarm Code
- +3~5: Transfer Complete Location (1,2)(3,4)(5,6)   +6~8: Location (1,2)(3,4)(5,6)   +9: Op Mode

## D(Word) 영역 — Direction (C/V, Word를 Bit처럼)
D0300=C/V#1 ... D0300+A=**D0310=C/V#11** ... D0314=C/V#15. (0:입고, 1:출고)

## D(Word) 영역 — ECS 명령 (S/C, RGV Transfer)
S/C#1=D0320,...#5=D0360, RGV#1=**D0370**.
- +0~2: From(1,2)(3,4)(5,6)  +3~5: To(1,2)(3,4)(5,6)  +6~7: Job no(1,2)(3,4)
- ※슬라이드16엔 RGV Transfer Data From D0530/To D0533/JobNo D0536 표기(버전 상이 가능 — 확인 필요).

## R(Tracking) 영역
- **Conveyor Start Address = (Conveyor No - 1) * 10 word**. RGV Line에 가까운 쪽이 Start Address.
  - C/V#1=R0000~, C/V#5 #9위치=R0040/R0041, C/V#11 트래킹 R0100~R0103(입고 JOB R0102~R0103→shift→R0100~R0101).
- **JOB NO 4자리(2word), 바이트스왑**: 1234 → 메모리 2143.
- S/C: R0300~R0309(S/C#1~5, 2word씩), RGV#1: R0310~R0311.

## 주요 시나리오 (M/D/R 주소)
- **알람**: Set M0492 ON→D0101(Set Alarm Code)→Ack M0963; Reset M0493→D0162→Ack M0964.
- **입고 C/V#11**: Pallet#22 M0371 ON, Load Complete#2 M0364/M0904 → ECS JOB Write R0102~R0103, Direction D0310 확인(입고0) → PLC #22→#21 move, PalletExist#21 M0370 ON/#22 M0371 OFF, JOB shift R0100~R0101 → W.O로 RGV Call → RGV Load M0361/M0901 → W.O off M0366.
- **출고 C/V#11**: RGV Unload#21 M0370, Load Complete#1 M0362/M0902 → ECS JOB Write, D0310="1"(출고) → #21→#22 move, M0371 ON/M0370 OFF, JOB shift R0102~R0103 → 작업자 Unload#2 M0363/M0903.
- **RGV#1 반송**: Transfer Cmd Req' D0210(IDLE1/RUN2) M1010, Transfer Data(From/To/JobNo) → RGV Pallet Presence M0580, Load Complete M0590/M1011, Transfer Complete Location D0213 → Unload Complete M0591/M1012 → Transfer Complete M0594/M1015.

## 할 일 (사용자 요청)
1. 각 통신을 이 사양대로 동작하게 수정.
2. 실제 PLC CPU=**XGK** → 각 시스템 영역표기를 XGK에 맞게 변경(M/D/R → XGK %MW/%DW 등 확인).
3. PPT 시나리오 + 구ECS(LGLS_1WH_original/Backup, **소스 변경 금지**)와 통신 가능한지 확인.
4. XGK 기준 구ECS·PPT 영역 정합 확인.
5. 실제 설비 통신 되면 전체 시뮬레이션 가능한지 확인.
