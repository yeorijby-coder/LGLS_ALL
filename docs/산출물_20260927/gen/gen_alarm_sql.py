# -*- coding: utf-8 -*-
# PLC 알람 리스트(260917) → EQP_ECD_MST INSERT SQL 생성
import os
ROOT = r'D:\project\LGLS\Renewal'
sc = [
("0011","승강 인버터 이상","HOIST INVERTER ERROR","M01111"),
("0012","주행 인버터 이상","TRAVEL INVERTER ERROR","M01112"),
("0013","승강 EOCR TRIP","HOIST EOCR TRIP","M01113"),
("0014","주행 EOCR TRIP","TRAVEL EOCR TRIP","M01114"),
("0015","포크 EOCR TRIP","FORK EOCR TRIP","M01115"),
("0016","승강 NFB TRIP","HOIST NFB TRIP","M01116"),
("0017","주행 NFB TRIP","TRAVEL NFB TRIP","M01117"),
("0018","승강 LOAD_LIMITTER 동작","HOIST LOAD LIMITER ACTIVE","M01118"),
("0021","전진 방향 와이어 늘어짐 및 끊어짐","WIRE SLACK/BREAK (FORWARD)","M01121"),
("0022","후진 방향 와이어 늘어짐 및 끊어짐","WIRE SLACK/BREAK (BACKWARD)","M01122"),
("0031","승강 한계점 에러","HOIST UPPER LIMIT ERROR","M01131"),
("0032","하강 한계점 에러","HOIST LOWER LIMIT ERROR","M01132"),
("0033","주행전진 한계점 에러","TRAVEL FORWARD LIMIT ERROR","M01133"),
("0034","주행후진 한계점 에러","TRAVEL BACKWARD LIMIT ERROR","M01134"),
("0036","자동모드에서 수동동작 에러","MANUAL OPERATION IN AUTO MODE","M01136"),
("0041","승강 카운터 이상","HOIST COUNTER ERROR","M01141"),
("0042","주행 카운터 이상","TRAVEL COUNTER ERROR","M01142"),
("0043","포크중심 밀림","FORK CENTER SLIP","M01143"),
("0044","승강 밀림","HOIST SLIP","M01144"),
("0045","주행 밀림","TRAVEL SLIP","M01145"),
("0051","승강 시간초과","HOIST UP TIMEOVER","M01151"),
("0052","하강 시간초과","HOIST DOWN TIMEOVER","M01152"),
("0053","주행 전진 시간초과","TRAVEL FORWARD TIMEOVER","M01153"),
("0054","주행 후진 시간초과","TRAVEL BACKWARD TIMEOVER","M01154"),
("0055","포크 시간초과","FORK TIMEOVER","M01155"),
("0056","동작 시간초과","OPERATION TIMEOVER","M01156"),
("0061","우측 화물이탈 이상","LOAD OVERHANG (RIGHT)","M01161"),
("0062","좌측 화물이탈 이상","LOAD OVERHANG (LEFT)","M01162"),
("0063","사이즈 이상","LOAD SIZE ERROR","M01163"),
("0071","입고대에서 로딩 후 화물 없음","NO LOAD AFTER PICK-UP AT STATION","M01171"),
("0072","언로딩후 화물 있음","LOAD REMAINS AFTER UNLOADING","M01172"),
("0073","좌측 렉 이중입고","DOUBLE STORAGE (LEFT RACK)","M01174"),
("0074","우측 렉 이중입고","DOUBLE STORAGE (RIGHT RACK)","M01173"),
("0075","공출고 에러","EMPTY RETRIEVAL","M01175"),
("0081","기상반 비상정지","EMERGENCY STOP (ON-BOARD PANEL)","M01181"),
("0082","지상반 비상정지","EMERGENCY STOP (GROUND PANEL)","M01182"),
("0091","주행 번지목적 데이터 이상","TRAVEL (BAY) DESTINATION DATA ERROR","M01191"),
("0092","승강 번지목적 데이터 이상","HOIST (LEVEL) DESTINATION DATA ERROR","M01192"),
("0093","열 목적 데이터 이상","ROW (BANK) DESTINATION DATA ERROR","M01193"),
]
# 101~118 : 지상반별 스테이션 C/V 과부하 (원본 102 의 문구 "CV101" 은 CV102 오기로 보고 고침)
cvp = {1: [101, 102], 2: [103, 104, 105, 106], 3: [107, 108, 109, 110], 4: [111, 112, 113, 114], 5: [115, 116, 117, 118]}
paddr = ["P00048", "P00049", "P0004A", "P0004B"]
for gp, lst in cvp.items():
    for i, c in enumerate(lst):
        sc.append(("%04d" % c, "CV%d 모터 과부하" % c, "CV%d MOTOR OVERLOAD" % c, "지상반%d %s" % (gp, paddr[i])))

cv = [("%04d" % (118 + b), "CV%d 모터 과부하" % (118 + b), "CV%d MOTOR OVERLOAD" % (118 + b), "M550%X" % b) for b in range(1, 14)]

rgv_codes = [12, 14, 15, 33, 34, 42, 45, 53, 54, 55, 61, 62, 81, 82, 91]
rgv_k = ["RGV 주행용 인버터 이상", "RGV 주행용 EOCR_TRIP", "RGV 콘베어 EOCR_TRIP", "RGV 전진 한계점", "RGV 후진 한계점",
         "RGV 주행 COUNT MISS", "RGV 주행 밀림", "RGV 주행 전진 시간초과", "RGV 주행 후진 시간초과", "RGV 콘베어 시간초과",
         "RGV 콘베어 우측 화물이탈", "RGV 콘베어 좌측 화물이탈", "제어반 비상정지", "OP-8 비상정지", "RGV 번지 목적 DATA이상"]
rgv_e = ["RGV TRAVEL INVERTER ERROR", "RGV TRAVEL EOCR TRIP", "RGV CONVEYOR EOCR TRIP", "RGV FORWARD LIMIT", "RGV BACKWARD LIMIT",
         "RGV TRAVEL COUNT MISS", "RGV TRAVEL SLIP", "RGV TRAVEL FORWARD TIMEOVER", "RGV TRAVEL BACKWARD TIMEOVER", "RGV CONVEYOR TIMEOVER",
         "RGV CONVEYOR LOAD OVERHANG (RIGHT)", "RGV CONVEYOR LOAD OVERHANG (LEFT)", "EMERGENCY STOP (CONTROL PANEL)",
         "EMERGENCY STOP (OP-8)", "RGV DESTINATION DATA ERROR"]
rgv = [("%04d" % c, k, e, "M560%X" % (i + 1)) for i, (c, k, e) in enumerate(zip(rgv_codes, rgv_k, rgv_e))]

ALL = {"SC_LGLS": sc, "CV": cv, "RTV": rgv}


def q(x):
    return "N'" + x.replace("'", "''") + "'"


def rows(typ, lst):
    out = []
    for code, k, e, addr in lst:
        out.append("      (%s, '%s', %s, NULL, %s, NULL, %s, NULL, %s, NULL, NULL)"
                   % (q(typ), code, q(k), q(e), q("PLC " + addr), q("PLC " + addr)))
    return ",\r\n".join(out)


rgv_in = ",".join("'%04d'" % c for c in rgv_codes)
INS = "INSERT INTO EQP_ECD_MST (EQP_TYP, EQP_ERR_CD, MSG_KOR, MSG_CHIN, MSG_ENG, MSG_HUN, ACTION_KOR, ACTION_CHIN, ACTION_ENG, ACTION_HUN, ERR_LEV)"
L = []
A = L.append
A("/* ============================================================================")
A("   PLC 알람 리스트 적용 - EQP_ECD_MST (설비 에러코드 마스터)")
A("   근거 : 260917_1동 자동창고_알람리스트.xlsx (PLC 담당 정리)")
A("   작성 : 2026-09-17   대상 DB : LGLS_MCS_IO (SQL Server 2008 이상)")
A("")
A("   넣는 것")
A("     1) SC_LGLS : 크레인 지상반 알람 11~93 (5대 공통) + 101~118 (호기별 스테이션 C/V 과부하)  %d건" % (len(sc) + 1))
A("        기존 SC_SFA / SC 는 같은 번호가 다른 뜻이라 새 구분으로 넣는다 (기존 행은 건드리지 않음)")
A("     2) CV      : 0119~0131 메인 C/V 모터 과부하 (M5501~M550D)  %d건 - 기존 CV 코드(0000~0094)와 겹치지 않음" % len(cv))
A("     3) RTV     : RGV 알람 (M5601~M560F)  %d건 - 기존 RTV 코드(0301~1101)와 겹치지 않음" % len(rgv))
A("   ACTION_KOR/ENG 에는 PLC 주소를 적었다 (운전 화면 [설비에러이력] 조치 칸)")
A("")
A("   함께 쓰는 설정 (프로그램 기본값이 이미 아래 값이다)")
A("     WCS_TASK_CV  WCS_DB.INI    [CNF]    SC_ERR_TYP=SC_LGLS, MAIN_ALM_BITS=1")
A("     HOST_TASK    EcsComA.ini   [Host]   ScErrCodeType=SC_LGLS, ScDualCodes=73,74, ScEmptyCodes=75")
A("     IO_TASK      ENV_IOSCH.INI [CNF]    SC_DUAL_CODES=73,74, SC_EMPTY_CODES=75")
A("     Client       Ecs.ini       [SC_ERR] ERR_TYP=SC_LGLS, DUAL_CODES=73,74, EMPTY_CODES=75")
A("")
A("   여러 번 실행해도 된다 (같은 키를 지우고 다시 넣는다).")
A("   실행 : SSMS 에서 파일을 열고 F5, 또는")
A("     sqlcmd -S <서버> -d LGLS_MCS_IO -U <ID> -P <PW> -f 65001 -i 260917_알람리스트_EQP_ECD_MST.sql")
A("   Client 는 에러코드 마스터를 기동할 때 읽으므로 실행 후 Client 를 다시 띄운다.")
A("   ============================================================================ */")
A("SET NOCOUNT ON;")
A("SET XACT_ABORT ON;")
A("")
A("BEGIN TRAN;")
A("")
A("/* 1) 크레인 지상반 (SC_LGLS) */")
A("DELETE FROM EQP_ECD_MST WHERE EQP_TYP = 'SC_LGLS';")
A(INS)
A("VALUES")
A("      (N'SC_LGLS', '0000', N'정상', NULL, N'NORMAL', NULL, N'정상', NULL, N'NORMAL', NULL, NULL),")
A(rows("SC_LGLS", sc) + ";")
A("")
A("/* 2) 메인 C/V 모터 과부하 (CV) */")
A("DELETE FROM EQP_ECD_MST WHERE EQP_TYP = 'CV' AND EQP_ERR_CD BETWEEN '0119' AND '0131';")
A(INS)
A("VALUES")
A(rows("CV", cv) + ";")
A("")
A("/* 3) RGV (RTV) */")
A("DELETE FROM EQP_ECD_MST WHERE EQP_TYP = 'RTV' AND EQP_ERR_CD IN (" + rgv_in + ");")
A(INS)
A("VALUES")
A(rows("RTV", rgv) + ";")
A("")
A("COMMIT TRAN;")
A("")
A("/* 확인 : SC_LGLS %d건 / CV %d건 / RTV %d건 */" % (len(sc) + 1, len(cv), len(rgv)))
A("SELECT EQP_TYP, COUNT(*) AS CNT, MIN(EQP_ERR_CD) AS MIN_CD, MAX(EQP_ERR_CD) AS MAX_CD")
A("  FROM EQP_ECD_MST")
A(" WHERE EQP_TYP = 'SC_LGLS'")
A("    OR (EQP_TYP = 'CV'  AND EQP_ERR_CD BETWEEN '0119' AND '0131')")
A("    OR (EQP_TYP = 'RTV' AND EQP_ERR_CD IN (" + rgv_in + "))")
A(" GROUP BY EQP_TYP;")
A("GO")
A("")
A("/* ----------------------------------------------------------------------------")
A("   되돌리기 (필요할 때만 주석을 풀어 실행)")
A("   DELETE FROM EQP_ECD_MST WHERE EQP_TYP = 'SC_LGLS';")
A("   DELETE FROM EQP_ECD_MST WHERE EQP_TYP = 'CV'  AND EQP_ERR_CD BETWEEN '0119' AND '0131';")
A("   DELETE FROM EQP_ECD_MST WHERE EQP_TYP = 'RTV' AND EQP_ERR_CD IN (" + rgv_in + ");")
A("   그리고 INI 의 SC_ERR_TYP / ScErrCodeType / ERR_TYP 를 SC_SFA 로, 판정 코드를 54,55 / 58,59 로 되돌린다.")
A("   ---------------------------------------------------------------------------- */")
SQL_TEXT = "\r\n".join(L) + "\r\n"

if __name__ == '__main__':
    # import 하면 코드표(sc / cv / rgv)와 SQL_TEXT 만 쓴다. 직접 실행하면 두 곳에 SQL 을 쓴다.
    base = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    for p in (os.path.join(ROOT, 'docs', 'sql', '260917_알람리스트_EQP_ECD_MST.sql'),
              os.path.join(base, 'SQL', '260917_알람리스트_EQP_ECD_MST.sql')):
        os.makedirs(os.path.dirname(p), exist_ok=True)
        with open(p, 'wb') as f:
            f.write(b'\xef\xbb\xbf' + SQL_TEXT.encode('utf-8'))
        print('saved', p, len(sc) + 1, len(cv), len(rgv))
