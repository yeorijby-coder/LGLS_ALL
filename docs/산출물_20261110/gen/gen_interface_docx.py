# -*- coding: utf-8 -*-
"""04 인터페이스 정의서 - 2026-11-10 종료 산출물 (신규, 가로).
   근거 : TASK/WCS_TASK_HOST (CSrvWork.cs ParseOorR/ParseModeChange/MakeResponse, CCliWork.cs GetStatusReport/GetErrorReport/GetJobCompleteReport,
          modDefApp.cs 상수, modStationMap.cs), 인터페이스명세서 20100311(1WH ECP 규격),
          TASK/WCS_TASK_CV_BIN/7_DeviceMap/PlcAddressMap.xml v1.4 (rAddrMode=DEC, dAddrMode=DOC).
   실행 : python gen_interface_docx.py"""
import os
from common1110 import *

d = new_doc('인터페이스 정의서', '상위(WMS/IMS) 전문 · 설비(PLC) 신호 · 프로그램 간 DB 인터페이스 - ' + SITE, landscape=True,
            history=[['1.0', '2026-07-30', AUTHOR, 'HOST WMS-ECS 명세 통일(2010 명세 = 1WH ECP 규격), 작업대 101~104 변환, 실기동 검증'],
                     ['2.0', '2026-09-21', AUTHOR, 'PlcAddressMap v1.4 (실PLC 대사 확정 DEC/DOC, RGV To3=D375), 04 기능 흐름 10장 상위 인터페이스 표']])

# ─────────────────────────────────────────────────────────────
d.add_heading('0. 이 문서는', 1)
para(d, 'WCS(ECS)가 바깥과 주고받는 모든 것을 적는다. 위로는 WMS/IMS 와 소켓 전문, 아래로는 마스터 PLC 와 XGT 디바이스 읽기·쓰기, 안으로는 프로그램 사이에 DB 표로 넘기는 값이다. '
        '전문·신호의 바이트 단위 정의는 소스 코드(WCS_TASK_HOST, PlcAddressMap.xml)와 1:1 로 맞춘 것이며, 코드가 바뀌면 이 문서도 같이 바꾼다.')
doc_set_table(d, 4)
d.add_heading('0.1 인터페이스 지도', 2)
table(d, ['번호', '구간', '수단', '담당 프로그램', '장'], [
    ['①', 'WMS/IMS ↔ WCS', 'TCP 소켓 2개(명령 수신 8001 / 보고 송신 8002), 고정 길이 ASCII 전문', 'WCS_TASK_HOST', '2'],
    ['②', 'WCS ↔ 마스터 PLC(XGT)', 'TCP 2004 XGT 전용 프로토콜, 디바이스 M(비트)·D(워드)·R(트래킹) 읽기/쓰기, 1 소켓', 'WCS_TASK_CV', '3'],
    ['③', 'HOST ↔ 스케줄러 ↔ 설비 통신', 'DB 표 JOB_MST(상태 기계) · CV_DATA/SC_DATA_LGLS/RTV_DATA_LGLS(_RD 읽기값 / _OD 지시값) · EQP_MST(하트비트)', 'TASK 3종', '4'],
    ['④', '운전 화면 ↔ DB', '조회·지시 쓰기(OD/CMD) · 조작 로그 · 배포 파일(UP_DOWN/DN_MST)', 'Ecs / WmsUp', '5'],
], widths=[1.2, 5.0, 11.0, 4.0, 1.2], font=9)
d.add_page_break()

# ═════════════════════════════════════════════════════════════
d.add_heading('1. 공통 규약', 1)
table(d, ['항목', '값'], [
    ['문자 · 길이', '전문은 ASCII 고정 길이. 숫자는 0 채움 우측 정렬(예 LuggNo 4자리 "0758"), 문자는 공백 채움 좌측 정렬'],
    ['제어 문자', 'STX = 0x02, ETX = 0x03'],
    ['창고 구분', 'WH_TYP = "10" (LG 화학 1동). 전문의 Job_Define 창고 문자 WH_DEFINE = "A"'],
    ['작업번호', 'LuggNo 4자리 0001~8999 상위 발행, 9000번대 = WCS 반자동(상위 미보고)'],
    ['셀 좌표', 'Bank(2) Bay(3) Level(2). DB 는 "BB-BBB-LL" 문자열(예 03-012-04)'],
    ['시각', 'DB 는 datetime, 전문에는 시각 필드 없음'],
], widths=[3.5, 19.0], font=9)
d.add_page_break()

# ═════════════════════════════════════════════════════════════
d.add_heading('2. 상위 (WMS / IMS) 인터페이스', 1)
d.add_heading('2.1 접속', 2)
table(d, ['소켓', '누가 접속', '포트 (현장)', '포트 (개발 PC)', '흐르는 전문', 'INI'], [
    ['명령 수신', 'WMS → WCS', '서버 : 8001', '9911', 'O 작업지시 / R 재지정 / M 모드 전환 / D 작업 삭제 → 응답(소문자)', 'EcsComA.ini [Network] LocalPort'],
    ['보고 송신', 'WCS → WMS', '192.100.1.191 : 8002', '127.0.0.1 : 9910', 'S 상태 / E 에러 / F 완료 → 응답(소문자) 수신', 'EcsComA.ini [Network] RemoteIP / RemotePort'],
], widths=[2.2, 2.4, 3.2, 2.4, 8.0, 4.3], font=9)
bullets(d, [
    '명령 수신 소켓은 접속별 작업 객체(SRV_MULTI_CONN=1, 2026-09-30)로 처리한다. 상위가 겹쳐 접속해도 먼저 붙은 소켓을 닫지 않는다(구 ECP 다중접속과 동일).',
    '보고 송신은 WCS 가 접속한다. 끊기면 재접속하고, 응답 없는 완료(09)는 RE_REPORT_09_SEC(60초)마다 재보고한다.',
    '모든 송수신 전문은 HOST_IF_LOG 에 원문으로 남는다(60일).',
])

d.add_heading('2.2 전문 틀 - 헤더 15바이트 + BODY', 2)
code(d, ['┌──────────── 헤더 15 ────────────┐┌───────────── BODY (길이 = 본문 + 2) ─────────────┐',
         '│ 메일박스(10, 좌정렬 공백) 길이(4) 공백(1) ││ STX(1)  본문 …  ETX(1)                              │',
         '예) "ECS_MBX   0066 " + STX + "O" + … + ETX        (O 전문 : 본문 64 → BODY 66)'])
table(d, ['필드', '길이', '값', '비고'], [
    ['메일박스', '10', 'ECS_MBX (WCS → WMS) / WMS_MBX (WMS → WCS)', '좌측 정렬, 나머지 공백. 다른 이름이면 사유 13 INVALID_MAILBOX_NAME'],
    ['BODY 길이', '4', 'STX+본문+ETX 의 길이 (0 채움)', '예 0066, 0031, 0007, 0011, 0043, 0023, 0010'],
    ['구분', '1', '공백', ''],
    ['BODY', '길이', 'STX + 본문 + ETX', 'STX/ETX 없으면 사유 1/2, 길이 틀리면 6/7'],
], widths=[3.0, 1.5, 9.0, 9.0], font=9)

d.add_heading('2.3 전문 목록', 2)
table(d, ['Type', '방향', 'BODY', '이름', '언제'], [
    ['O', 'WMS → WCS', '66', '작업 지시 (Order)', '입고 / 출고 / 피킹 / 랙투랙 / 이동 지시. 응답 "o"'],
    ['R', 'WMS → WCS', '31', '재지정 (Re-assign)', '이중입고(54)·공출고(58) 뒤 목적지 재지정. 응답 "r"'],
    ['M', 'WMS → WCS', '7', '모드 전환 (Mode)', '작업대 C/V 의 입고/출고 방향 전환. 응답 "m"'],
    ['D', 'WMS → WCS', '-', '작업 삭제 (Delete)', '작업상태 99/10/20 인 작업만 삭제 가능. 응답 "d"'],
    ['S', 'WCS → WMS', '43', '상태 보고 (Status)', '크레인 5대 · 작업대 상태 · PLC 모드. 변경 시 + 30초 주기. 응답 "s"'],
    ['E', 'WCS → WMS', '23', '에러 보고 (Error)', '크레인 / 작업대 / RGV 알람 발생·해제. 응답 "e"'],
    ['F', 'WCS → WMS', '10', '완료 보고 (Finish)', '입고 S/C 완료(1차) / 출고 S/C 완료(1차) + 출고대 도착(2차). 응답 "f"'],
    ['o r m d s e f', '응답', '11', '응답 (ACK / NAK)', 'Type 소문자 + A/N + 사유(2) + LuggNo(4) + StepCount(1)'],
], widths=[2.3, 2.4, 1.4, 4.0, 12.4], font=9)
d.add_page_break()

d.add_heading('2.4 O - 작업 지시 (BODY 66)', 2)
table(d, ['Offset', '길이', '필드', '값 · 뜻', 'DB (JOB_MST)'], [
    ['0', '1', 'STX', '0x02', ''],
    ['1', '1', 'Type', 'O', ''],
    ['2', '1', 'Job_Define', '작업 구분 : 1 입고 / 2 출고 / 3 피킹출고 / 4 랙투랙 / 5 호기간 이동 / 6 이동', 'JOB_TYP'],
    ['3', '4', 'LuggNo', '작업번호 0001~8999 (중복이면 사유 3)', 'LUGG_NO'],
    ['7', '4', 'SeqNo', '상위 순번 (참고)', '-'],
    ['11', '7', 'PalletNo', '파렛트 번호', 'LOT_NO'],
    ['18', '3', 'StartPos', '출발 스테이션(IMS 코드, 2.9 변환) 또는 크레인', 'START_POS (내부 코드로 변환)'],
    ['21', '2', 'StartBank', '출발 셀 Bank', 'START_LOCATION'],
    ['23', '3', 'StartBay', '출발 셀 Bay', '〃'],
    ['26', '2', 'StartLevel', '출발 셀 Level', '〃'],
    ['28', '3', 'RouteStn', '경유 스테이션 (미사용, 0 채움)', '-'],
    ['31', '3', 'DestPos', '도착 스테이션(IMS 코드) 또는 크레인', 'DEST_POS (변환)'],
    ['34', '2', 'DestBank', '도착 셀 Bank', 'DEST_LOCATION'],
    ['36', '3', 'DestBay', '도착 셀 Bay', '〃'],
    ['39', '2', 'DestLevel', '도착 셀 Level', '〃'],
    ['41', '3', 'Priority', '우선순위 (기본 100, 작을수록 먼저)', 'JOB_PRIORITY'],
    ['44', '1', 'JobRouting', '경유 구분 (참고)', '-'],
    ['45', '20', 'ProductID', '제품 정보', 'PRODUCT_ID'],
    ['65', '1', 'ETX', '0x03', ''],
], widths=[1.6, 1.4, 3.0, 11.0, 5.5], font=8.5)
bullets(d, [
    '입고(1) : StartPos = 작업대(101/103/102 재입고), Dest = 크레인(1~5) + 셀. 출고(2)/피킹(3) : Start = 크레인 + 셀, DestPos = 작업대(101/104/102).',
    '검증 순서 → 사유 : STX/ETX(1,2) → 길이(6,7) → 메일박스(13) → 작업구분(11) → 작업번호 중복(3)/형식(10) → 스테이션(9) → 셀(8)/금지 셀(12) → 저장. 성공하면 JOB_STATUS 99 로 생성.',
])

d.add_heading('2.5 R - 재지정 (BODY 31)', 2)
table(d, ['Offset', '길이', '필드', '값 · 뜻'], [
    ['0 / 1', '1 / 1', 'STX / Type', '0x02 / R'],
    ['2', '1', 'Job_Define', '작업 구분'],
    ['3', '4', 'LuggNo', '재지정할 작업번호 (없으면 사유 4 NO_EXIST_JOB)'],
    ['7', '3+2+3+2', 'StartPos / Bank / Bay / Level', '새 출발지 (이중입고 54 : 실제 화물이 있던 셀)'],
    ['17', '3+2+3+2', 'DestPos / Bank / Bay / Level', '새 도착지 (공출고 58 : 다른 셀)'],
    ['27', '1', 'Kind', '재지정 종류'],
    ['28', '2', 'ScNum', '크레인 번호 (이 현장 재지정에서는 미사용)'],
    ['30', '1', 'ETX', '0x03'],
], widths=[2.2, 2.6, 6.0, 11.7], font=8.5)
note(d, '이중입고(크레인 에러 54 / SC_DUAL_CODES 73,74)·공출고(58 / SC_EMPTY_CODES 75)가 나면 WCS 가 E 전문으로 알리고 작업을 세운다. WMS 가 R 로 새 셀을 주면 작업을 이어 간다. 재지정 지시 허용 판정에 위 코드를 쓴다.')

d.add_heading('2.6 M - 모드 전환 (BODY 7) · D - 작업 삭제', 2)
table(d, ['전문', 'BODY', '구성', '동작'], [
    ['M', '7', 'STX + "M" + CvNo(3 : 101~105 작업대 코드) + Mode(1 : 0 입고 / 1 출고) + ETX',
     '해당 작업대 C/V 의 방향 워드(D300+n, 3.3)에 쓴다. 105/110 은 C/V#2(S/C#1 통로) 방향전환. 응답 "m"'],
    ['D', '-', 'STX + "D" + Job_Define(1) + LuggNo(4) + … + ETX',
     '작업상태 99 / 10 / 20 (설비에 아직 안 나간 것)만 삭제. 그 밖은 사유 14 ALREADY_INVOKED_JOB'],
], widths=[1.5, 1.5, 9.5, 10.0], font=8.5)

d.add_heading('2.7 응답 (BODY 11)', 2)
code(d, ['STX  Type(1, 소문자 o/r/m/d/s/e/f)  Result(1 : A 정상 / N 거부)  Reason(2)  LuggNo(4)  StepCount(1)  ETX'])
table(d, ['Reason', '이름', '뜻', 'Reason', '이름', '뜻'], [
    ['00', 'NO_ERROR', '정상', '10', 'INVALID_LUGG_NO', '작업번호 형식 오류'],
    ['01', 'NO_STX', 'STX 없음', '11', 'UNKNOWN_JOB_TYPE', '작업 구분 오류'],
    ['02', 'NO_ETX', 'ETX 없음', '12', 'INHIBITED_LOC', '금지 셀'],
    ['03', 'LUGG_NO_DUPLICATED', '작업번호 중복', '13', 'INVALID_MAILBOX_NAME', '메일박스 이름 오류'],
    ['04', 'NO_EXIST_JOB', '해당 작업 없음', '14', 'ALREADY_INVOKED_JOB', '이미 설비에 나간 작업(삭제 불가)'],
    ['05', 'ECS_BUFFER_FULL', '수용 불가', '15', 'INVALID_CONTENT', '내용 오류'],
    ['06', 'INVALID_HEADER_LENG', '헤더 길이 오류', '16', 'UNKNOWN_MSG_TYPE', '모르는 전문 종류'],
    ['07', 'INVALID_MSG_LENG', 'BODY 길이 오류', '17', 'IMPROPER_HANDSHAKE', '순서 위반'],
    ['08', 'INVALID_LOC', '셀 좌표 오류', '18', 'NO_RESPONSE', '응답 없음'],
    ['09', 'INVALID_STN_NO', '스테이션 번호 오류', '19', 'SOCKET_ERROR', '소켓 오류'],
    ['', '', '', '99', 'INTERNAL_ERROR', '내부 오류'],
], widths=[1.4, 4.2, 5.6, 1.4, 4.2, 5.7], font=8.5)
d.add_page_break()

d.add_heading('2.8 S - 상태 보고 (BODY 43)', 2)
code(d, ['STX "S" [SC호기(2) 상태(1) LuggNo(4)] x 5   STN101상태(1) STN102상태(1) STN103상태(1)   STN101 PLC모드(1) STN110 PLC모드(1)  ETX'])
table(d, ['필드', '길이', '값'], [
    ['SC호기 / 상태 / LuggNo (x5)', '7 x 5 = 35', '호기 01~05, 상태 0 = 작업 가능 / 1 = 불가(에러·수동·DOWN), 작업 중인 작업번호(없으면 0000)'],
    ['STN101 / 102 / 103 상태', '1 x 3', '작업대 상태 0 = 가능 / 1 = 불가'],
    ['STN101 / 110 PLC 모드', '1 x 2', '입출고 겸용 C/V 의 현재 방향 0 입고 / 1 출고 (101 = C/V#11, 110 = C/V#2 통로)'],
], widths=[6.0, 3.0, 13.5], font=8.5)
note(d, '값이 바뀌면 즉시, 그렇지 않아도 30초마다 보낸다. HOST 가 기동되면 첫 S 를 먼저 보내므로 WMS 는 이것으로 WCS 생존을 알 수 있다.')

d.add_heading('2.9 E - 에러 보고 (BODY 23)', 2)
code(d, ['STX "E" DeviceClass(1) DeviceNo(3) ErrorKind(1) ErrorCode(4) LuggNo(4) Bank(2) Bay(3) Level(2) ETX'])
table(d, ['필드', '값'], [
    ['DeviceClass', '1 = S/C (DeviceNo 901~905 → 001~005) / 2 = 작업대 (101~104) / 3 = RGV (801 → 001)'],
    ['ErrorKind', '0 해제 / 1 발생 / 이중입고·공출고는 별도 Kind (ScErrCodeType=SC_LGLS 코드표 ScDualCodes 73,74 · ScEmptyCodes 75 로 판정)'],
    ['ErrorCode', 'EQP_ECD_MST 의 코드 4자리 (크레인 SC_LGLS / C/V 과부하 0119~0131 / RGV)'],
    ['LuggNo · Bank · Bay · Level', '그때 설비가 들고 있던 작업과 위치 (없으면 0 채움)'],
], widths=[4.5, 18.0], font=8.5)

d.add_heading('2.10 F - 완료 보고 (BODY 10)', 2)
code(d, ['STX "F" Job_Define(1) LuggNo(4) CompleteClass(1) StepCount(1) ETX'])
table(d, ['작업', '언제', 'StepCount', '그 뒤'], [
    ['입고 (1)', 'S/C 완료 (29) = 작업 완료', '1', '응답 "f" 받으면 09 → JOB_MST_HIS 이관 · 삭제'],
    ['출고 (2) / 피킹 (3)', 'S/C 완료 (29) = 랙이 비었다 (1차)', '1', '작업은 지우지 않고 15 로 계속. WMS 는 그 셀에 입고 예약을 더 빨리 걸 수 있다'],
    ['출고 (2) / 피킹 (3)', '출고대 도착 (19) = 작업 끝 (2차)', '2', '응답 받으면 09 → 삭제'],
    ['반자동 (10~15, 9000번대)', '-', '-', '보고하지 않는다. 완료 즉시 삭제'],
], widths=[4.0, 7.0, 2.0, 9.5], font=8.5)
note(d, '응답이 없으면 RE_REPORT_09_SEC(60초, 5~3600)마다 다시 보낸다. 별도의 A(도착) 전문은 없다 - 1WH ECP 규격.')

d.add_heading('2.11 스테이션 코드 변환 (StationMapMode = WMS)', 2)
table(d, ['IMS 코드', '내부 트랙 (ToInner)', '역변환 (ToIms)', '설명'], [
    ['101', '122', '122 → 101', '외부 입고 전용 입출고대 (C/V#11 TR#22, 입출고 겸용)'],
    ['102', '출고 도착 129 / 재입고 출발 130', '129 → 102, 130 → 102', 'Picking 작업대 (C/V#14 출고 / C/V#15 입고)'],
    ['103', '124', '124 → 103', '제품 입고대 (C/V#12 TR#24)'],
    ['104', '126', '126 → 104', '원부자재 불출대 (C/V#13 TR#26)'],
    ['105 / 110', '103', '103 → 105', 'C/V#2 (S/C#1 통로, 방향전환용. M 전문에서만 쓰임)'],
    ['1~5 (또는 201~205)', '901~905', '90n → n', '크레인. WMS 모드는 1~5, 구 ECS 모드는 201~205'],
], widths=[3.5, 5.5, 4.0, 9.5], font=8.5)
note(d, 'IMS 스테이션 번호는 IMS/프로토콜 문서의 Station Number 표 기준이다(2026-09-30 정정 : 종전 운영 표에서 24/26 의 STN 이 뒤집혀 있었으나 통신 코드는 처음부터 맞았다). '
        'EcsComA.ini [StationMap_WMS] 로 개별 재정의할 수 있다.')
d.add_page_break()

# ═════════════════════════════════════════════════════════════
d.add_heading('3. 설비 (마스터 PLC, XGT) 인터페이스', 1)
d.add_heading('3.1 접속 · 디바이스 규약', 2)
table(d, ['항목', '값'], [
    ['접속', 'TCP 192.100.1.110 : 2004, LS XGT 전용 프로토콜(연속 읽기/쓰기, 비트 단일 쓰기 %MX). 마스터 PLC 한 대가 C/V 15 · S/C 5 · RGV 1 을 모두 대표하므로 소켓 1개'],
    ['순회', 'C/V 15대 + S/C 5 + RGV 1 한 바퀴 약 1초(일괄 읽기). 출고대는 OUT_SCAN_MS 간격으로 보강 스캔(3초 펄스)'],
    ['M (내부 릴레이, 비트)', 'id 0x90. 이벤트 / ACK / 알람. 워드주소 = 비트주소 ÷ 16, 비트위치 = 비트주소 % 16. 쓰기는 단일 비트(BIT_WRITE=1)'],
    ['D (데이터 레지스터, 워드)', 'id 0xA8. 방향 / 상태 / 명령. 문서표기 10진 그대로 = 워드주소 (D0160 = %DW160). [확정 2026-09-01 실PLC 대사, dAddrMode=DOC]'],
    ['R (파일 레지스터, 워드)', 'id 0xAF. 트래킹(작업번호 ASCII 4자리 = 2워드). 문서표기 10진 (rAddrMode=DEC, 실PLC 대사 확정)'],
    ['블록 주소식', 'base = origin + (설비번호 - numberFrom) x stride,  주소 = base + offset'],
    ['단일 기준', 'TASK\\WCS_TASK_CV_BIN\\7_DeviceMap\\PlcAddressMap.xml v1.4. 수정 후 bin\\Debug\\7_DeviceMap 에도 복사. 설비 통신 · 시뮬레이터 · 화면 메모리맵 · 시나리오가 모두 이 파일을 읽는다'],
], widths=[5.0, 17.5], font=8.5)

d.add_heading('3.2 C/V (컨베이어 15대, 설비번호 1~15)', 2)
table(d, ['블록', '디바이스', 'origin', 'stride', '신호 (offset)', '방향 · 뜻'], [
    ['Event', 'M', '256', '32',
     'UnloadComplete1(1) LoadComplete1(2) UnloadComplete2(3) LoadComplete2(4) WorkInstruction(5, WAIT_IN) WorkOrder(6) InReady2(7) OperationMode(8) UnloadRequest2Ack(9, 14·15호기만) PalletExist[슬롯](16+슬롯)',
     '설비 → WCS. 1 = RGV 측(홀수 트랙), 2 = S/C 측 또는 작업대 측. WorkInstruction = 출고대 화물 도착(3초 펄스 → CV_DATA.RET_READY_RD/LATCH). WorkOrder = 다음 설비 호출(RGV Call / S/C Call)'],
    ['Ack', 'M', '1280', '16',
     'UnloadComplete1Ack(1) LoadComplete1Ack(2) UnloadComplete2Ack(3) LoadComplete2Ack(4) UnloadRequest2(5, 14·15호기만)',
     'WCS → 설비. 완료 신호를 받으면 같은 번호 Ack 를 세우고, 설비가 신호를 내리면 Ack 도 내린다'],
    ['Direction', 'D', '300', '1', 'IoDirection(0) ASCII 1문자', '양방향. 0 = 입고 / 1 = 출고 (DIR_CODE=IN1 현장 : 입고 1 / 출고 0 - PLC 담당 확인값, INI 로 극성 선택). 운전 화면 더블클릭·M 전문·스케줄러 자동 정합이 쓴다'],
    ['Tracking', 'R', '0', '10', 'JobNo[슬롯](슬롯x2, 2워드 ASCII 4자리), 최대 3슬롯', '양방향. 설비가 ID Shift 로 옮기고, WCS 는 최초 기록(입고대 적재 시)·운전 화면 [쓰기]·[완료처리] 기록 때 쓴다. C/V#15 슬롯 순서 131,132,130'],
], widths=[1.8, 1.6, 1.3, 1.3, 8.5, 8.0], font=8)
note(d, '예) C/V#2(설비 2) : Event base = 256 + 1x32 = 288 → LoadComplete1 = M289 ; Ack base = 1296 ; 방향 = D301 ; 트래킹 base = R10 (슬롯 0 = R10~11 트랙 103, 슬롯 1 = R12~13 트랙 104).')

d.add_heading('3.3 S/C (스태커 크레인 5대, 설비번호 1~5)', 2)
table(d, ['블록', '디바이스', 'origin', 'stride', '신호 (offset)', '방향 · 뜻'], [
    ['Event', 'M', '768', '32', 'PalletExistFlag(0) LoadComplete(16) UnloadComplete(17) AlarmSetReport(18) AlarmResetReport(19) TransferAck(20)', '설비 → WCS. 차상 화물 / 적재·하역 완료 / 알람 발생·해제 보고 / 반송 지시 접수'],
    ['Ack', 'M', '1536', '16', 'TransferRequest(0) LoadCompleteAck(1) UnloadCompleteAck(2) AlarmSetReportAck(3) AlarmResetReportAck(4) TransferComplete(5)', 'WCS → 설비. 반송 요청 / 완료·알람 보고 응답 / 반송 완료 통지'],
    ['Status', 'D', '160', '10', 'SubsystemStatus(0 : 1 IDLE / 2 RUN) AlarmSetCode(1) AlarmResetCode(2) TransferCompleteLocation(3~5) SubsystemLocation(6~8 열/연/단) OperationMode(9)', '설비 → WCS'],
    ['ErrCode', 'D', '1160', '10', 'ErrCode(0)', '설비 → WCS. 에러코드 전용 블록(2026-09-02 분리 : 종전 Status offset10 이 다음 호기 상태와 겹쳐 전 크레인 유령 에러). ★실PLC 주소 재확인 필요 항목★'],
    ['Command', 'D', '320', '10', 'From(0~2 : 포트/열/연/단) To(3~5) JobNo(6~7, 2워드 ASCII)', 'WCS → 설비. 반송 지시 데이터. TransferRequest 를 세우기 전에 쓴다'],
    ['Tracking', 'R', '300', '2', 'PalletOnVehicle(0, 2워드 ASCII 4자리)', '설비 → WCS. 차상 작업번호'],
], widths=[1.8, 1.6, 1.3, 1.3, 8.5, 8.0], font=8)

d.add_heading('3.4 RGV (무인 대차 1대)', 2)
table(d, ['블록', '디바이스', 'origin', 'stride', '신호 (offset)', '방향 · 뜻'], [
    ['Event', 'M', '928', '32', 'S/C 와 같음 (PalletExistFlag 0, LoadComplete 16, UnloadComplete 17, AlarmSetReport 18, AlarmResetReport 19, TransferAck 20)', '설비 → WCS'],
    ['Ack', 'M', '1616', '16', 'S/C 와 같음 (TransferRequest 0 … TransferComplete 5)', 'WCS → 설비'],
    ['Status', 'D', '210', '10', 'S/C 와 같음 (SubsystemStatus 0 … OperationMode 9)', '설비 → WCS'],
    ['Command', 'D', '370', '10', 'From(0~2) To1(3) To2(4) To3(5 = 트랙) JobNo(6~7)', 'WCS → 설비. To3 = D375 (2026-09-04 정정 : 구 ECS 표의 W0345 는 오기, S/C#3 To3 와 겹쳐 오지시 재현됨). ★실PLC 대사 시 D375 확인★'],
    ['Tracking', 'R', '310', '2', 'PalletOnVehicle(0)', '설비 → WCS. 차상 작업번호'],
], widths=[1.8, 1.6, 1.3, 1.3, 8.5, 8.0], font=8)

d.add_heading('3.5 공통 알람 비트', 2)
table(d, ['그룹', '신호', '주소 (M 비트)', '뜻'], [
    ['Alarm (S/C#1 대표)', 'AlarmSet / AlarmReset', 'M786 / M787', '설비 → WCS. 사이클당 1회 점검'],
    ['', 'AlarmSetAck / AlarmResetAck', 'M1539 / M1540', 'WCS → 설비'],
    ['MainAlarm (2026-09-17 알람 리스트)', 'CvOverloadWord', 'M8800 (= M5500 워드) bit1~D', 'C/V 119~131 모터 과부하 → CV_DATA.ERROR_CODE 0119~0131. 운전 화면 C/V 에러는 이것만'],
    ['', 'RgvAlarmWord', 'M8960 (= M5600 워드) bit1~F', 'RGV 알람 12,14,15,33,34,42,45,53,54,55,61,62,81,82,91 → RTV_DATA_LGLS.ERR_CODE_RD'],
], widths=[5.0, 5.0, 5.0, 7.5], font=8.5)
note(d, '사용 여부 WCS_DB.INI [CNF] MAIN_ALM_BITS (1 사용). 주소 표기 : 워드 10진 + 비트 16진 (M5501 = 550x16+1 = 8801).')
d.add_page_break()

d.add_heading('3.6 핸드셰이크 규약', 2)
para(d, '모든 비트 신호는 "요청 → 응답(ACK) → 해제" 한 바퀴다. 설비가 신호를 세우면 WCS 가 같은 이름의 Ack 를 세우고, 설비가 신호를 내리면 WCS 도 Ack 를 내린다(설비 리셋). '
        '반송 지시는 데이터(D)를 먼저 쓰고 요청 비트(M)를 세운다.')
table(d, ['단계', 'WCS', '설비', 'DB 상태'], [
    ['① 지시 준비', 'SubsystemStatus = 1(IDLE) 확인 → Command From/To/JobNo 쓰기', '', '20/30 (지시)'],
    ['② 요청', 'TransferRequest ON', 'TransferAck ON (접수)', '21/31 (전송)'],
    ['③ 접수 확인', 'TransferRequest OFF', 'TransferAck OFF, 이동 시작 (RUN=2)', '25/35 (반송중)'],
    ['④ 적재', '', 'PalletExistFlag ON, PalletOnVehicle = 작업번호, LoadComplete ON', ''],
    ['⑤ 적재 응답', 'LoadCompleteAck ON → (설비 OFF 뒤) OFF', 'LoadComplete OFF', '출발 C/V 의 UnloadComplete 도 같은 식으로 ACK'],
    ['⑥ 하역', '', 'PalletExistFlag OFF, UnloadComplete ON, TransferCompleteLocation', ''],
    ['⑦ 하역 응답', 'UnloadCompleteAck ON/OFF, TransferComplete ON/OFF', 'UnloadComplete OFF, IDLE 복귀', '29/39 (완료)'],
    ['⑧ 트래킹 이어받기', '도착 C/V 의 LoadComplete → Ack. PLC 가 ID Shift 로 차상 R → C/V R 슬롯에 작업번호를 옮긴다', '', '15/16 (다음 C/V 구동중)'],
], widths=[3.0, 8.0, 7.5, 4.0], font=8.5)
bullets(d, [
    '알람 : AlarmSetReport ON + AlarmSetCode → WCS 가 AlarmSetReportAck, 에러 래치(SC_ERR_CODE_BLOCK=3 : 보고 비트 래치, RESET 보고 또는 워드 0 이면 해제). E 전문 발신.',
    '완료 판정은 설비 신호로만 한다. 시간 경과로 자동 완료하지 않는다(체류복구 폐기 2026-09-17). 신호가 유실되면 운전 화면 [완료처리]가 DB 를 넘기고 트래킹 기록을 요청한다.',
    '출고대 도착(WorkInstruction, 3초 펄스)은 OUT_SCAN_MS 보강 스캔으로 래치한다. OUT_DONE_ON_ARRIVAL=1 이면 출고대 트래킹에 그 작업번호가 실제 도착하면 즉시 19.',
])

d.add_heading('3.7 표준 시나리오 (PlcAddressMap.xml <Scenarios>)', 2)
table(d, ['ID', '제목', '경로'], [
    ['1', '입출고대 #22 → S/C 입고', 'C/V #11 → RGV → 통로(짝수호기) → S/C → 랙'],
    ['2', 'S/C → 입출고대 #22 출고', '랙 → S/C → 통로(홀수호기 / 1호기는 C/V#2) → RGV → C/V #11'],
    ['3-1', '24 입고대 → S/C 입고', 'C/V #12 → RGV → 통로 → S/C → 랙'],
    ['3-2', 'S/C → 26 출고대 출고', '랙 → S/C → 통로 → RGV → C/V #13'],
    ['4-1', '30 입고대 → S/C 입고', 'C/V #15 → RGV → 통로 → S/C → 랙'],
    ['4-2', 'S/C → 29 피킹대 출고', '랙 → S/C → 통로 → RGV → C/V #14'],
], widths=[1.5, 7.0, 14.0], font=9)
note(d, '각 시나리오는 Step(kind = observe 관찰 / force WCS 가 세움 / word 워드 확인)으로 신호 순서를 정의하며, 시뮬레이터(EQP_SIM)와 설비 통신의 시나리오 시험이 같은 정의를 쓴다. '
        '호기를 고르면 $inCv / $outCv 가 <CraneMap> 으로 치환된다.')

d.add_heading('3.8 설비 배치 (<Equipments> / <CraneMap>)', 2)
table(d, ['C/V', 'PLC', '트랙', '역할', 'C/V', 'PLC', '트랙', '역할'], [
    ['#1', '01', '101~102', '(예비)', '#9', '09', '117~118', 'S/C#5 출고 통로 (S/C측 118 → RGV측 117)'],
    ['#2', '02', '103~104', 'S/C#1 통로 입출고 겸용 (RGV측 103 / S/C측 104)', '#10', '10', '119~120', 'S/C#5 입고 통로 (RGV측 119 → S/C측 120)'],
    ['#3', '03', '105~106', 'S/C#2 출고 통로', '#11', '11', '121~122', '입출고대 122 (STN 101), RGV 픽업·드롭 121'],
    ['#4', '04', '107~108', 'S/C#2 입고 통로', '#12', '12', '123~124', '제품 입고대 124 (STN 103), RGV 픽업 123'],
    ['#5', '05', '109~110', 'S/C#3 출고 통로', '#13', '13', '125~126', '원부자재 불출대 126 (STN 104), RGV 드롭 125'],
    ['#6', '06', '111~112', 'S/C#3 입고 통로', '#14', '14', '127~129', 'Picking 출고대 129 (STN 102), RGV 드롭 127'],
    ['#7', '07', '113~114', 'S/C#4 출고 통로', '#15', '15', '130~132', 'Picking 입고대 130 (STN 102), 슬롯 131,132,130, RGV 픽업 131'],
    ['#8', '08', '115~116', 'S/C#4 입고 통로', '', '', '', ''],
], widths=[1.2, 1.0, 2.2, 6.8, 1.2, 1.0, 2.2, 6.9], font=8)
table(d, ['S/C', '입고 C/V', 'RGV 포트 → S/C 포트', '출고 C/V', 'S/C 포트 → RGV 포트', '비고'], [
    ['#1 (901)', '#2', '103 → 104', '#2', '104 → 103', '입출고 겸용 통로. 입고는 2-deep(103 비면 104 대기 중에도 드롭 허용), 방향은 사용권으로'],
    ['#2 (902)', '#4', '107 → 108', '#3', '106 → 105', ''],
    ['#3 (903)', '#6', '111 → 112', '#5', '110 → 109', ''],
    ['#4 (904)', '#8', '115 → 116', '#7', '114 → 113', ''],
    ['#5 (905)', '#10', '119 → 120', '#9', '118 → 117', ''],
], widths=[2.0, 2.0, 4.0, 2.0, 4.0, 8.5], font=8.5)
d.add_page_break()

# ═════════════════════════════════════════════════════════════
d.add_heading('4. 프로그램 간 DB 인터페이스', 1)
para(d, 'TASK 3종과 운전 화면은 서로 소켓으로 말하지 않는다. 모든 인계는 LGLS_MCS_IO 의 표로 한다. 열의 뜻은 03 테이블 정의서.')
d.add_heading('4.1 작업 상태 기계 (JOB_MST.JOB_STATUS)', 2)
table(d, ['상태', '뜻', '세우는 쪽', '다음으로 넘기는 조건'], [
    ['99', '신규', 'HOST (O 전문)', 'IO 가 출발 설비·경로를 정하면 10 또는 20'],
    ['10 / 11', 'C/V 지시 / 전송', 'IO / EQP', '트래킹에 번호 기록 확인 → 15'],
    ['15', 'C/V 구동중', 'IO', 'WorkOrder(RGV Call) + 픽업 트랙 번호 일치 → 30. 출고대 실도착(번호 일치) → 19'],
    ['16', '통로 C/V 구동중 (입고, IN_HS_STATUS=16)', 'IO / 운전 화면 [쓰기]', 'S/C 측 트랙에 번호 + WorkOrder(S/C Call) → 20. 읽는 쪽 5곳은 15/16 모두 수용'],
    ['30 / 31 / 35', 'RGV 지시 / 전송 / 반송중', 'IO / EQP / EQP', 'RTV_DATA_LGLS TRANSFER_REQUEST_OD → PLC. UnloadComplete + 도착 트랙 → 39'],
    ['39', 'RGV 완료', 'EQP', 'IO 가 도착 트랙 번호 확인 → 15/16 (입고) 또는 15 (출고)'],
    ['20 / 21 / 25', 'S/C 지시 / 전송 / 반송중', 'IO / EQP / EQP', 'SC_DATA_LGLS FROM/TO/PALLET_ID_OD + TRANSFER_REQUEST_OD → PLC. 크레인당 사이클 1건'],
    ['29', 'S/C 완료', 'EQP', '입고 = 완료 → HOST F(1). 출고 = 1차 완료 F(1), 통로 트랙 번호 확인 → 15'],
    ['19', 'C/V 완료 (출고대 도착)', 'IO', 'HOST F(2)'],
    ['09', '완료 보고 응답 받음', 'HOST', 'JOB_MST_HIS 이관 후 삭제 (반자동은 보고 없이 즉시 삭제)'],
], widths=[2.2, 5.0, 3.5, 11.8], font=8.5)

d.add_heading('4.2 설비 데이터 표 - 읽기값(_RD)과 지시값(_OD)', 2)
table(d, ['표', '쓰는 쪽 → 읽는 쪽', '주요 열', '규약'], [
    ['CV_DATA (트랙 101~132)', 'EQP → IO · 화면 : *_RD / IO · 화면 → EQP : *_OD, CMD_RQ_ID',
     'LUGG_NO_RD(트래킹) SENSOR0_DATA_RD(감지) RET_READY_RD/LATCH* ERROR_CODE / LUGG_NO_OD TRACKING_WRITE_YN(=Y 기록 요청) STOCK_MODE CMD_RQ_ID(DIR·PAUSE·RESET…) CMD_RQ_YN',
     'EQP 는 OD/CMD 를 소비하면 YN 을 N 으로. ":" 는 토큰 구분자로 쓰지 않는다(DB 계층이 @ 로 바꿈)'],
    ['SC_DATA_LGLS (901~905)', 'EQP → IO · 화면 / IO · 화면 → EQP',
     'SUBSYSTEM_STATUS_RD ERR_CODE_RD ERR_STA_FK1_RD PALLET_ON_VEHICLE_RD LOCATION_RD / FROM_01~03_OD TO_01~03_OD PALLET_ID_OD TRANSFER_REQUEST_OD OD_RQ_YN JOB_TYP_RD',
     'OD_RQ_YN=Y 가 지시 요청. EQP 가 PLC 에 쓰고 N. [지시 재전송] = 다시 Y. [강제완료]/[완료처리] = OD 비움'],
    ['RTV_DATA_LGLS (801)', '〃', 'DEPART_TRACK ARRIVE_TRACK(실제 트랙번호) TRANSFER_REQUEST_OD PALLET_ON_VEHICLE_RD ERR_CODE_RD(메인 알람 비트 파생)', 'PLC_NO 는 EQP_MST 와 같은 "01"'],
    ['EQP_MST', 'TASK 3종 → 화면', 'UPD_DT CONNECTED_YN (EQP / SCH / HOST / HOST2)', '하트비트. 운전 화면 신호등이 이 표로 판정(EQP 1행이 PLC 소켓 대표)'],
    ['DEL_HIS_SETTING', '화면 → IO', 'TABLE_NAME CYCLE UNIT KEY_COLUMN', 'IO PurgeHistory 가 1시간마다 읽어 삭제(2026-10-29)'],
    ['COMMON_CODE / EQP_ECD_MST', 'DB → 전부', 'JOB_STATUS 등 코드 / 에러코드 문구', '운전 화면은 기동 때 읽는다 → SQL 반영 뒤 재기동'],
], widths=[4.0, 5.0, 8.5, 5.0], font=8)
d.add_page_break()

# ═════════════════════════════════════════════════════════════
d.add_heading('5. 운전 화면 ↔ DB · 배포', 1)
table(d, ['기능', '표 · 열', '규약'], [
    ['설비 상태 수집', 'CV_DATA / SC_DATA_LGLS / RTV_DATA_LGLS / JOB_MST', 'COLLECT_INTERVAL_MS(200) 마다 읽어 그림·판넬 갱신. 작업정보 판넬은 JOB_PANEL_REFRESH_MS(1000)'],
    ['트랙 [쓰기]/[삭제]/[붙여넣기]', 'CV_DATA.LUGG_NO_OD + TRACKING_WRITE_YN=Y, JOB_MST.JOB_STATUS(통로 16 / 작업대 15)', 'EQP 가 R 트래킹에 쓴다'],
    ['방향 전환 더블클릭', 'CV_DATA.CMD_RQ_ID = DIR 명령 (+ Ecs.ini DIR_ADDR_CV11/CV2 로 확인창에 PLC 주소 표시)', 'EQP 가 D 방향 워드에 즉시 쓴다. IO 는 MANUAL_DIR_HOLD_SEC 동안 자동 정합 억제'],
    ['[완료처리] (스텝 단위)', 'JOB_MST.JOB_STATUS 이동(15/39/16/29/19) + 설비 OD 해제 + CV_DATA 번호 기록 요청', '경로의 다른 칸에 화물이 있으면 그 단계로(2026-10-28). WCS_CLIENT_LOG 기록'],
    ['[지시 재전송] / [강제완료] / [이상종료]', 'SC/RTV OD_RQ_YN=Y 재세움 / OD 비움 + 상태 29·39 / OD 비움', '권한 UPD_YN, 확인창, WCS_CLIENT_LOG'],
    ['우선순위 ▲▼ / [수정]', 'JOB_MST.JOB_PRIORITY ±1 / 직접 입력', 'IO 는 다음 사이클부터 반영'],
    ['수동지시 / [H/S 배출]', 'JOB_MST 생성 (JOB_TYP 1x 반자동, LUGG_NO 9001~9900)', 'HOST 보고 안 함, 완료 시 삭제'],
    ['로그 삭제 설정', 'DEL_HIS_SETTING.CYCLE', 'IO 가 다음 검사(최대 1시간)부터 반영'],
    ['조작 기록', 'WCS_CLIENT_LOG (사용자 · PC IP · 내용)', '60일'],
    ['배포 (Upload → 런처)', 'UP_DOWN(UP_DAT varbinary(max), DN_NM, DN_VER) · DN_MST · DN_SEQ', 'WmsUp 이 올리면 DN_VER +1. Ecs.exe(런처)가 기동 때 이름별 최고 버전만 받고 EcsMain.exe 를 띄운다'],
    ['작업대 이름표 · 설비 배치', 'EcsDefine.xml (Client 폴더) · DEST_POS_DEF.REMARKS(PosLabel)', 'DB 아닌 파일. 이름을 바꾸면 각 PC 에 배포'],
], widths=[4.5, 9.0, 9.0], font=8.5)

d.add_heading('6. 변경 이력 (인터페이스에 영향 있는 것)', 1)
table(d, ['일자', '내용'], [
    ['2026-07-30', 'HOST 전문을 2010 인터페이스명세서(1WH ECP 규격)로 통일. O66/R31/M7/S43/E23/F10/응답11. 작업대 101~104 변환. HOST_SIM 실기동 검증'],
    ['2026-09-01', '실PLC(테스트 베드) 대사 : D 블록 DOC(문서표기 10진), R 블록 DEC 확정'],
    ['2026-09-02', 'S/C 에러코드 블록 D1160 분리(Status offset10 겹침으로 전 크레인 유령 에러)'],
    ['2026-09-04', 'RGV Command To3 = D375 정정(구 표 W0345 오기, S/C#3 To3 와 충돌)'],
    ['2026-09-15 ~ 16', 'C/V 방향 극성 DIR_CODE(IN0 시뮬 / IN1 현장). SC_ERR_CODE_BLOCK 모드 체계. RTV PLC_NO "01"'],
    ['2026-09-17', '메인 PLC 알람 비트(M8800 C/V 과부하, M8960 RGV) 반영. 알람 리스트 SC_LGLS. 체류복구 폐기(완료는 설비 신호로만)'],
    ['2026-09-29 ~ 30', '출고대 3초 펄스 래치(RET_READY_LATCH*), OUT_DONE_ON_ARRIVAL, HOST 접속별 수신(SRV_MULTI_CONN), IMS STN 24/26 표 정정'],
    ['2026-10-15', 'Upload/Download 표(UP_DOWN/DN_MST/DN_SEQ). 스텝 단위 [완료처리]'],
    ['2026-10-29', '[완료처리] 경로 탐색. DEL_HIS_SETTING 을 IO 가 실제로 소비. "[] -> [0000]" 트래킹 로그 억제'],
], widths=[3.0, 19.5], font=8.5)

para(d, '')
para(d, '문서 판 : %s' % G.DATE, size=9, color=GRAY)
out = os.path.join(OUT, '04_인터페이스_정의서.docx'); d.save(out); print('saved', out)
