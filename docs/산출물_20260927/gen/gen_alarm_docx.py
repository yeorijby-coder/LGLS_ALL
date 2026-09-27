# -*- coding: utf-8 -*-
"""11_알람코드_정의서 (2026-09-21) : PLC 알람 리스트(260917_1동 자동창고_알람리스트.xlsx) 적용 결과"""
import os
from gen_common import *
from gen_alarm_sql import sc, cv, rgv, rgv_codes

d = new_doc('알람코드 정의서', 'PLC 알람 리스트 → ECS 에러코드 마스터(EQP_ECD_MST)',
            history=[['1.0', '2026-09-17', AUTHOR, '최초 작성 - PLC 담당 알람 리스트(260917) 적용']])

d.add_heading('1. 개요', 1)
para(d, 'PLC 담당이 정리한 1동 자동창고 알람 리스트(2026-09-17)를 ECS 에러코드 마스터(EQP_ECD_MST)에 넣고, '
        '설비 통신·스케줄러·상위 통신·운전 화면이 이 코드표를 쓰도록 맞췄다.')
table(d, ['구분', 'PLC', '주소', '코드', 'EQP_TYP', '건수'], [
    ['크레인 지상반 (5대 공통)', '지상반_1 ~ 5', 'M01111 ~ M01193', '11 ~ 93', 'SC_LGLS', str(sum(1 for c in sc if int(c[0]) < 100))],
    ['크레인 스테이션 C/V 과부하', '지상반_1 ~ 5', 'P00048 ~ P0004B', '101 ~ 118', 'SC_LGLS', str(sum(1 for c in sc if int(c[0]) >= 100))],
    ['메인 C/V 모터 과부하', '메인 PLC', 'M5501 ~ M550D', '119 ~ 131 (= C/V 번호)', 'CV', str(len(cv))],
    ['RGV', '메인 PLC', 'M5601 ~ M560F', '12 ~ 91', 'RTV', str(len(rgv))]],
    widths=[4.2, 2.4, 3.2, 3.4, 2.0, 1.4], font=9)
para(d, '코드는 DB 에 4자리 0 채움 문자열로 저장한다 (예: 73 → \'0073\'). SC_LGLS 에는 정상 코드 \'0000\' 도 넣었다(총 %d건).' % (len(sc) + 1))

d.add_heading('2. 코드가 흐르는 길', 1)
table(d, ['단계', '프로그램', '처리'], [
    ['① 발생', 'PLC', '크레인 : 알람코드 워드(ALARM_SET_CODE, D0161+10k)에 코드. 메인 C/V·RGV : 알람 비트'],
    ['② 판독', '설비 통신 (WCS_TASK_CV)',
     '크레인·RGV : VehThread 가 코드 워드를 읽어 SC_DATA_LGLS / RTV_DATA_LGLS.ERR_CODE_RD 에 기록.\n'
     'RGV 는 워드가 0 인데 알람 비트(M5601~)가 켜져 있으면 그 비트의 코드를 쓴다.\n'
     '메인 C/V : CvThread.CvMainAlarmBits 가 M550x 워드를 사이클마다 읽어 CV_DATA.ERROR_CODE(\'0119\'~) 기록, 꺼지면 \'0\''],
    ['③ 이력', '설비 통신', '새 에러마다 EQP_ERR_HIS 적재 (EQP_TYP = SC_LGLS / RTV / CV)'],
    ['④ 상위 보고', '상위 통신 (WCS_TASK_HOST)', 'HOST_ERR_SEND_YN=\'N\' 인 설비를 E 전문으로 보고. 코드는 설비 원본값 그대로.\n'
                                              '크레인 ErrorKind : 73·74 → 1(이중입고), 75 → 3(공출고), 그 밖 0(기계적 에러)'],
    ['⑤ 화면', '운전 화면 (Ecs.exe)', '크레인·RGV 상태창에 "코드 + 문구", 설비에러이력 창은 코드표 조인으로 문구 표시'],
    ['⑥ 작업', '스케줄러 (IO_TASK)', '이중입고·공출고 코드일 때만 재지정 지시를 허용 (그 밖 에러는 지시 보류)']],
    widths=[1.8, 4.2, 11.0], font=9)

d.add_heading('3. 설정', 1)
para(d, '프로그램 기본값이 아래 값이라 키가 없어도 같게 동작한다. 종전 SFA 코드표로 되돌릴 때만 바꾼다.')
table(d, ['프로그램', '파일 [섹션]', '키', '값', '뜻'], [
    ['설비 통신', 'WCS_DB.INI [CNF]', 'SC_ERR_TYP', 'SC_LGLS', '크레인 이력의 코드표 구분'],
    ['설비 통신', 'WCS_DB.INI [CNF]', 'RTV_ERR_TYP', 'RTV', 'RGV 이력의 코드표 구분'],
    ['설비 통신', 'WCS_DB.INI [CNF]', 'MAIN_ALM_BITS', '1', '메인 C/V·RGV 알람 비트 읽기 (0 = 안 읽음)'],
    ['설비 통신', 'PlcAddressMap.xml', '<Global name="MainAlarm">', 'CvOverloadWord=8800\nRgvAlarmWord=8960', '비트 0 의 절대주소 (M5500 / M5600)'],
    ['상위 통신', 'EcsComA.ini [Host]', 'ScErrCodeType', 'SC_LGLS', 'E 보고 시 코드표'],
    ['상위 통신', 'EcsComA.ini [Host]', 'ScDualCodes / ScEmptyCodes', '73,74 / 75', 'ErrorKind 1 / 3 판정'],
    ['상위 통신', 'EcsComA.ini [Host]', 'ScInFailCodes / ScOutFailCodes', '(비움)', 'ErrorKind 2 / 4 판정 - 알람 리스트에 해당 코드 없음'],
    ['스케줄러', 'ENV_IOSCH.INI [CNF]', 'SC_DUAL_CODES / SC_EMPTY_CODES', '73,74 / 75', '재지정 지시 허용 코드'],
    ['운전 화면', 'Ecs.ini [SC_ERR]', 'ERR_TYP / DUAL_CODES / EMPTY_CODES', 'SC_LGLS / 73,74 / 75', '상태창 문구, 에러해제·삭제 확인창']],
    widths=[2.2, 3.6, 4.4, 3.2, 3.6], font=8.5)

d.add_heading('4. 크레인 지상반 코드 (EQP_TYP = SC_LGLS)', 1)
para(d, '11~93 은 5대 공통(각 지상반 PLC 의 같은 주소), 101~118 은 호기별 스테이션 C/V 과부하다.')
table(d, ['코드', '내용', 'English', 'PLC 주소'],
      [[c[0], c[1], c[2], c[3]] for c in sc], widths=[1.6, 6.4, 6.0, 3.0], font=8.5)

d.add_heading('5. 메인 C/V 모터 과부하 (EQP_TYP = CV)', 1)
para(d, '비트 번호 1~D 가 C/V 119~131 에 차례로 대응한다. 코드 = C/V 번호, CV_DATA 의 해당 MC_NO 행에 기록한다.')
table(d, ['코드', '내용', 'English', 'PLC 주소', 'CV_DATA.MC_NO'],
      [[c[0], c[1], c[2], c[3], str(int(c[0]))] for c in cv], widths=[1.6, 4.6, 5.0, 2.6, 3.2], font=8.5)

d.add_heading('6. RGV (EQP_TYP = RTV)', 1)
para(d, '기존 RTV 코드(0301~1101, 대차 제조사 코드)와 번호가 겹치지 않는다.')
table(d, ['코드', '내용', 'English', 'PLC 주소 (비트)'],
      [[c[0], c[1], c[2], c[3]] for c in rgv], widths=[1.6, 6.0, 6.4, 3.0], font=8.5)

d.add_heading('7. DB 적용', 1)
para(d, '같은 폴더 SQL\\260917_알람리스트_EQP_ECD_MST.sql 을 SSMS 에서 열어 실행한다(F5). 여러 번 실행해도 된다. '
        '실행 후 운전 화면을 다시 띄운다(에러코드 마스터는 기동할 때 읽는다).')
para(d, 'sqlcmd -S <서버> -d LGLS_MCS_IO -U <ID> -P <PW> -f 65001 -i 260917_알람리스트_EQP_ECD_MST.sql', indent=0.5, size=9)
para(d, '확인 : SC_LGLS %d건 / CV %d건(0119~0131) / RTV %d건' % (len(sc) + 1, len(cv), len(rgv)), indent=0.5)

d.add_heading('8. 원본 리스트와 다르게 넣은 부분 · 확인 요청', 1)
table(d, ['항목', '원본', '적용', '비고'], [
    ['지상반_1 코드 102', '"102 : CV101 모터 과부하"', 'CV102 모터 과부하', '번호로 보아 CV102 오기로 판단 - PLC 담당 확인 요청'],
    ['RGV 33 / 34', '"33 : 33 RGV 전진 한계점"', 'RGV 전진 한계점', '문구 앞 "33" 중복 표기 제거'],
    ['크레인 73 / 74 주소', '73 = M01174, 74 = M01173', '원본 주소 그대로 기재', '다른 코드와 달리 주소 순서가 뒤바뀌어 있음 - PLC 담당 확인 요청'],
    ['지상반 비트의 전달', '지상반_1~5 가 같은 주소(M01111 등)', '크레인 코드 워드(D0161+10k)로 받는다고 전제',
     'ECS 는 메인 PLC 한 곳에만 접속한다. 지상반 비트를 직접 읽으려면 호기별 접속이 필요 - PLC 담당 확인 요청'],
    ['P 디바이스 (P00048~)', '지상반 입력 비트', '코드 101~118 로만 사용', 'ECS 통신부는 M/D/R 만 읽는다'],
    ['입고장애·출고장애 코드', '리스트에 없음', 'ErrorKind 2 / 4 판정 코드 비움', '종전 SFA 코드표는 56 / 57'],
    ['메인 C/V 과부하의 상위 보고', '-', 'E 전문 DeviceClass 2, 장치번호 119~131', 'WMS 가 이 번호를 아는지 협의 필요']],
    widths=[3.4, 4.2, 4.2, 5.2], font=8.5)

out = os.path.join(OUT, '11_알람코드_정의서.docx'); d.save(out); print('saved', out)
