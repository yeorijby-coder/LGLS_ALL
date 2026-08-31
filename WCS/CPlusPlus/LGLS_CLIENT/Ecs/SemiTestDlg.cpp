// SemiTestDlg.cpp : 반자동 TEST 창 (2026-08-13)
//   반자동 작업 생성 창(CManualJob)에서 [반자동 TEST] 버튼으로 연다.
//   시나리오 테스트와 같은 취지로 ECS↔PLC 교신 값을 로그로 보여준다.
#include "stdafx.h"
#include "Ecs.h"
#include "SemiTestDlg.h"
#include "Lib.h"
#include "RecordSetWrap.h"

IMPLEMENT_DYNAMIC(CSemiTestDlg, CSkinDialog)

CSemiTestDlg::CSemiTestDlg(CEcsDoc* pDoc, CWnd* pParent)
	: CSkinDialog(CSemiTestDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_bRun = FALSE;
	// [LGLS 2026-08-13] 리사이징 허용(CSkinDialog 가 THICKFRAME + 비례 배치 처리)
	for (int r = 0; r < ROWS; r++)
	{
		m_row[r].nState = 0;
		m_row[r].nCrane = -1;
		m_row[r].nSeq   = 0;
	}
}

CSemiTestDlg::~CSemiTestDlg()
{
}

void CSemiTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LGLS_ST_START, m_btnStart);
	DDX_Control(pDX, IDC_LGLS_ST_STOP,  m_btnStop);
	DDX_Control(pDX, IDCANCEL,          m_btnClose);
	DDX_Control(pDX, IDC_LGLS_ST_LOG,   m_lstLog);
}

BEGIN_MESSAGE_MAP(CSemiTestDlg, CSkinDialog)
	ON_BN_CLICKED(IDC_LGLS_ST_START, &CSemiTestDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_LGLS_ST_STOP,  &CSemiTestDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDCANCEL,          &CSemiTestDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CSemiTestDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// 버튼 스킨 + 아이콘 (다른 대화상자 버튼과 동일 스타일)
	TCHAR chrFile[500]; GetModuleFileName(NULL, chrFile, MAX_PATH);
	CString strApp; strApp.Format(_T("%s"), chrFile);
	CString strIcoPath = strApp.Left(strApp.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\20x20\\");
	m_btnStart.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnStart.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strIcoPath, _T("write"), _T(".png"))), NULL, 5, 5);
	m_btnStop.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnStop.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strIcoPath, _T("cut"), _T(".png"))), NULL, 5, 5);
	m_btnClose.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnClose.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strIcoPath, _T("arrow-left"), _T(".png"))), NULL, 5, 5);

	// [LGLS 2026-08-31] ★기본값이 거꾸로였다★ - 출발지 126 / 도착지 124 로 깔려 있었다.
	//   현장 기준은 C/V#12(124)=입고대, C/V#13(126)=출고대 다.
	//   실제로 HOST_SIM 로직1 도 124→901(입고) / 901→126(출고) 으로 돈다.
	//   그 탓에 반자동 시험이 스스로 막혔다 : 126(출고대)에는 파렛트를 올릴 수단이 없어
	//   DriveCV 의 화물감지 게이트에서 멈추고, 되돌아오는 출고는 124(입고대)로 향했다.
	// 기본값: 입고대 124 → 출고대 126
	for (int r = 0; r < ROWS; r++)
	{
		SetDlgItemText(IDC_LGLS_ST_FR1 + r, _T("124"));
		SetDlgItemText(IDC_LGLS_ST_TO1 + r, _T("126"));
		SetRowStat(r, _T("-"));
	}
	// [LGLS 2026-08-19] 로케이션 모드 기본값 = 크레인 자동(기존 동작)
	CheckRadioButton(IDC_LGLS_ST_MODE_AUTO, IDC_LGLS_ST_MODE_LOC, IDC_LGLS_ST_MODE_AUTO);
	RenameByIni();
	InitLogList();
	return TRUE;
}

// 다국어(ini) - 모든 대화상자는 번역 가능해야 한다는 원칙 준수
void CSemiTestDlg::RenameByIni()
{
	TCHAR chrFile[500]; GetModuleFileName(NULL, chrFile, MAX_PATH);
	CString strApp; strApp.Format(_T("%s"), chrFile);
	CString strPath = Global.GetConcatPath(strApp.Left(strApp.ReverseFind('\\')) + _T("\\rc_resource\\dlg_semitest\\"), _T("dlg_semitest"), _T(".ini"));
	EN_LANG enLang = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;
	CString s;
	s = CLib::GetIniStringFromPath(strPath, _T("dlgname"),  (int)enLang); if (!s.IsEmpty()) SetWindowText(s);
	s = CLib::GetIniStringFromPath(strPath, _T("startpos"), (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_ST_HDR_FR, s);
	s = CLib::GetIniStringFromPath(strPath, _T("destpos"),  (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_ST_HDR_TO, s);
	s = CLib::GetIniStringFromPath(strPath, _T("status"),   (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_ST_HDR_STAT, s);
	s = CLib::GetIniStringFromPath(strPath, _T("hdrloc"),   (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_ST_HDR_LOC, s);
	s = CLib::GetIniStringFromPath(strPath, _T("modeauto"), (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_ST_MODE_AUTO, s);
	s = CLib::GetIniStringFromPath(strPath, _T("modeloc"),  (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_ST_MODE_LOC, s);
	s = CLib::GetIniStringFromPath(strPath, _T("start"),    (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_ST_START, s);
	s = CLib::GetIniStringFromPath(strPath, _T("stop"),     (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_ST_STOP, s);
	s = CLib::GetIniStringFromPath(strPath, _T("close"),    (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDCANCEL, s);
}

// [LGLS 2026-08-13] JOB_STATUS 전이를 만든 IO_SCH(cThread_SCH) 함수명 추정 매핑
static LPCTSTR SchFuncForStatus(const CString& strOld, const CString& strNew)
{
	if (strOld == _T("99"))                        return _T("cThread_SCH.AcceptNewJob");
	if (strNew == _T("11"))                        return _T("cThread_SCH.DriveCV");
	if (strNew == _T("21"))                        return _T("cThread_SCH.DriveSC");
	if (strNew == _T("31"))                        return _T("cThread_SCH.DriveRGV");
	if (strNew == _T("15"))                        return _T("cThread_SCH.RunCV");
	if (strNew == _T("25"))                        return _T("cThread_SCH.RunSC");
	if (strNew == _T("35"))                        return _T("cThread_SCH.RunRGV");
	if (strNew == _T("19"))                        return _T("cThread_SCH.CompleteCV");
	if (strNew == _T("29"))                        return _T("cThread_SCH.CompleteSC");
	if (strNew == _T("39"))                        return _T("cThread_SCH.CompleteRGV");
	return _T("cThread_SCH.Thread_Doing");
}

void CSemiTestDlg::InitLogList()
{
	if (!::IsWindow(m_lstLog.m_hWnd)) return;
	m_lstLog.SetExtendedStyle(m_lstLog.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	TCHAR chrFile[500]; GetModuleFileName(NULL, chrFile, MAX_PATH);
	CString strApp; strApp.Format(_T("%s"), chrFile);
	CString strPath = Global.GetConcatPath(strApp.Left(strApp.ReverseFind('\\')) + _T("\\rc_resource\\dlg_semitest\\"), _T("dlg_semitest"), _T(".ini"));
	EN_LANG enLang = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;

	struct { LPCTSTR szKey; LPCTSTR szDef; int nW; } cols[] = {
		{ _T("coldate"),  _T("날짜"),     72 },
		{ _T("coltime"),  _T("시간"),     62 },
		{ _T("collogic"), _T("로직"),     44 },
		{ _T("colact"),   _T("동작"),     96 },
		{ _T("coljob"),   _T("작업번호"), 60 },
		{ _T("colsys"),   _T("시스템"),   70 },
		{ _T("colfunc"),  _T("함수명"),  170 },
		{ _T("colmsg"),   _T("내용"),    620 },
	};
	for (int i = 0; i < 8; i++)
	{
		CString s = CLib::GetIniStringFromPath(strPath, cols[i].szKey, (int)enLang);
		if (s.IsEmpty()) s = cols[i].szDef;
		m_lstLog.InsertColumn(i, s, LVCFMT_LEFT, cols[i].nW);
	}
}

void CSemiTestDlg::AddLog(int nLogic, LPCTSTR szAct, LPCTSTR szLugg, LPCTSTR szSys, LPCTSTR szFunc, const CString& strMsg)
{
	if (!::IsWindow(m_lstLog.m_hWnd)) return;
	if (m_lstLog.GetItemCount() > 2000) m_lstLog.DeleteItem(0);
	CTime t = CTime::GetCurrentTime();
	int n = m_lstLog.GetItemCount();
	m_lstLog.InsertItem(n, t.Format(_T("%Y.%m.%d")));
	m_lstLog.SetItemText(n, 1, t.Format(_T("%H:%M:%S")));
	CString s;
	if (nLogic >= 1) s.Format(_T("로직%d"), nLogic); else s = _T("-");
	m_lstLog.SetItemText(n, 2, s);
	m_lstLog.SetItemText(n, 3, szAct);
	m_lstLog.SetItemText(n, 4, szLugg);
	m_lstLog.SetItemText(n, 5, szSys);
	m_lstLog.SetItemText(n, 6, szFunc);
	m_lstLog.SetItemText(n, 7, strMsg);
	m_lstLog.EnsureVisible(n, FALSE);
}

void CSemiTestDlg::SetRowStat(int r, LPCTSTR s)
{
	SetDlgItemText(IDC_LGLS_ST_STAT1 + r, s);
}

BOOL CSemiTestDlg::RowActive(int r)
{
	CString strFr, strTo;
	GetDlgItemText(IDC_LGLS_ST_FR1 + r, strFr); strFr.Trim();
	GetDlgItemText(IDC_LGLS_ST_TO1 + r, strTo); strTo.Trim();
	if (strFr.IsEmpty() || strTo.IsEmpty()) return FALSE;
	// [LGLS 2026-08-19] 로케이션 직접 입력 모드: 로케이션이 입력된 행만 가동
	if (IsLocMode())
	{
		CString strLoc; GetDlgItemText(IDC_LGLS_ST_LOC1 + r, strLoc); strLoc.Trim();
		return !strLoc.IsEmpty();
	}
	for (int k = 0; k < CRANES; k++)
		if (IsDlgButtonChecked(IDC_LGLS_ST_SC_BASE + r * CRANES + k)) return TRUE;
	return FALSE;
}

// [LGLS 2026-08-19] 라디오: 로케이션 직접 입력 모드인가
BOOL CSemiTestDlg::IsLocMode()
{
	return IsDlgButtonChecked(IDC_LGLS_ST_MODE_LOC) == BST_CHECKED;
}

// [LGLS 2026-08-19] 행의 로케이션 입력(BB-BBB-LL, 구분자 - . 공백, 붙여쓰기 허용)을
//   CELL_MST 에서 찾아 로케이션/담당 크레인(CELL_SC_NO)을 확정한다.
//   ※ 점유 여부는 검사하지 않는다 - 이중입고 테스트(에러 54) 주입 용도로도 쓰기 위함.
BOOL CSemiTestDlg::ResolveLocRow(int r)
{
	CString s; GetDlgItemText(IDC_LGLS_ST_LOC1 + r, s); s.Trim();
	if (s.IsEmpty()) return FALSE;
	s.Replace(_T("."), _T("-")); s.Replace(_T(" "), _T("-"));
	while (s.Replace(_T("--"), _T("-")) > 0);
	int nBank = 0, nBay = 0, nLev = 0;
	if (s.Find(_T('-')) >= 0)
	{
		int nPos = 0;
		CString strBank = s.Tokenize(_T("-"), nPos);
		CString strBay  = s.Tokenize(_T("-"), nPos);
		CString strLev  = s.Tokenize(_T("-"), nPos);
		if (strLev.IsEmpty())
		{
			AddLog(r + 1, _T("-"), _T("-"), _T("Client"), _T("CSemiTestDlg::ResolveLocRow"), s + _T(" : 로케이션 형식 오류 (BB-BBB-LL)"));
			return FALSE;
		}
		nBank = CConvert::ToInt(strBank); nBay = CConvert::ToInt(strBay); nLev = CConvert::ToInt(strLev);
	}
	else if (s.GetLength() >= 5)	// 붙여쓰기 BBBBBLL (2-x-2)
	{
		nBank = CConvert::ToInt(s.Left(2));
		nBay  = CConvert::ToInt(s.Mid(2, s.GetLength() - 4));
		nLev  = CConvert::ToInt(s.Right(2));
	}
	else
	{
		AddLog(r + 1, _T("-"), _T("-"), _T("Client"), _T("CSemiTestDlg::ResolveLocRow"), s + _T(" : 로케이션 형식 오류 (BB-BBB-LL)"));
		return FALSE;
	}

	CString strSql, strMsg; int nCnt = 0;
	strSql.Format(_T("SELECT TOP 1 BANK, BAY, LEV, CELL_NO, CELL_SC_NO FROM CELL_MST")
	              _T(" WHERE WH_TYP = '%s' AND CELL_USE_YN = 'Y'")
	              _T("   AND CAST(BANK AS INT) = %d AND CAST(BAY AS INT) = %d AND CAST(LEV AS INT) = %d"),
	              (LPCTSTR)m_pDoc->m_WH_TYP, nBank, nBay, nLev);
	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	if (nCnt <= 0)
	{
		delete pRsw;
		CString strErr; strErr.Format(_T("%02d-%03d-%02d : CELL_MST 에 없는 로케이션입니다"), nBank, nBay, nLev);
		AddLog(r + 1, _T("-"), _T("-"), _T("Client"), _T("CSemiTestDlg::ResolveLocRow"), strErr);
		return FALSE;
	}
	pRsw->MoveFirst();
	m_row[r].strBank = pRsw->GetItem(_T("BANK"));
	m_row[r].strBay  = pRsw->GetItem(_T("BAY"));
	m_row[r].strLev  = pRsw->GetItem(_T("LEV"));
	m_row[r].strScNo = pRsw->GetItem(_T("CELL_SC_NO"));
	delete pRsw;
	int nCrane = CConvert::ToInt(m_row[r].strScNo) - 901;
	if (nCrane < 0 || nCrane >= CRANES)
	{
		AddLog(r + 1, _T("-"), _T("-"), _T("Client"), _T("CSemiTestDlg::ResolveLocRow"), _T("CELL_SC_NO(") + m_row[r].strScNo + _T(") 가 901~905 범위가 아닙니다"));
		return FALSE;
	}
	m_row[r].nCrane = nCrane;
	return TRUE;
}

// 체크된 크레인 중 다음(라운드로빈)
BOOL CSemiTestDlg::PickNextCrane(int r, int& nCrane)
{
	for (int i = 1; i <= CRANES; i++)
	{
		int k = (m_row[r].nCrane + i) % CRANES;
		if (IsDlgButtonChecked(IDC_LGLS_ST_SC_BASE + r * CRANES + k)) { nCrane = k; return TRUE; }
	}
	return FALSE;
}

// 해당 크레인의 다음 로케이션(CELL_NO 순서 +1, 끝이면 처음으로)
BOOL CSemiTestDlg::NextCell(int r, int nCrane)
{
	CString strScNo; strScNo.Format(_T("%d"), 901 + nCrane);
	CString strSql, strMsg;
	int nCnt = 0;
	for (int nTry = 0; nTry < 2; nTry++)
	{
		strSql  = _T("SELECT TOP 1 BANK, BAY, LEV, CELL_NO FROM CELL_MST");
		strSql += _T(" WHERE WH_TYP = ") + CLib::Quot(m_pDoc->m_WH_TYP);
		strSql += _T("   AND CELL_SC_NO = ") + CLib::Quot(strScNo);
		strSql += _T("   AND CELL_USE_YN = 'Y'");
		if (nTry == 0 && !m_row[r].strLastCell[nCrane].IsEmpty())
			strSql += _T("   AND CELL_NO > ") + CLib::Quot(m_row[r].strLastCell[nCrane]);
		strSql += _T(" ORDER BY CELL_NO");

		_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
		if (nCnt > 0)
		{
			pRsw->MoveFirst();
			m_row[r].strBank = pRsw->GetItem(_T("BANK"));
			m_row[r].strBay  = pRsw->GetItem(_T("BAY"));
			m_row[r].strLev  = pRsw->GetItem(_T("LEV"));
			m_row[r].strLastCell[nCrane] = pRsw->GetItem(_T("CELL_NO"));
			m_row[r].strScNo = strScNo;
			delete pRsw;
			return TRUE;
		}
		delete pRsw;
		if (nTry == 0) m_row[r].strLastCell[nCrane] = _T("");	// 끝까지 갔으면 처음부터
	}
	AddLog(r + 1, _T("-"), _T("-"), _T("Client"), _T("CSemiTestDlg::NextCell"), CString(_T("SC")) + strScNo + _T(" 사용 가능한 로케이션이 없습니다"));
	return FALSE;
}

// JOB_MST 에 반자동 작업 생성 (CManualJob 의 INSERT 와 동일 형식)
//   bSto=TRUE: 반자동입고(11) 출발지→로케이션 / FALSE: 반자동출고(12) 로케이션→도착지
BOOL CSemiTestDlg::InsertJob(int r, BOOL bSto)
{
	CString strFr, strTo;
	GetDlgItemText(IDC_LGLS_ST_FR1 + r, strFr); strFr.Trim();
	GetDlgItemText(IDC_LGLS_ST_TO1 + r, strTo); strTo.Trim();

	m_row[r].nSeq++;
	CString strTag;
	strTag.Format(_T("SEMITEST-R%d-%d"), r + 1, m_row[r].nSeq);

	CString strStartPos, strDestPos, strStartLoc, strDestLoc, strJobTyp;
	if (bSto)
	{
		strJobTyp   = _T("11");	// 반자동입고
		strStartPos = strFr;
		strDestPos  = m_row[r].strScNo;
		strStartLoc = _T("00-000-00");
		strDestLoc.Format(_T("%02d-%03d-%02d"), CConvert::ToInt(m_row[r].strBank), CConvert::ToInt(m_row[r].strBay), CConvert::ToInt(m_row[r].strLev));
	}
	else
	{
		strJobTyp   = _T("12");	// 반자동출고
		strStartPos = m_row[r].strScNo;
		strDestPos  = strTo;
		strStartLoc.Format(_T("%02d-%03d-%02d"), CConvert::ToInt(m_row[r].strBank), CConvert::ToInt(m_row[r].strBay), CConvert::ToInt(m_row[r].strLev));
		strDestLoc  = _T("00-000-00");
	}

	CString strSql;
	strSql.Format(_T(" INSERT INTO JOB_MST ( WH_TYP, LUGG_NO, START_POS, START_LOCATION, DEST_POS, DEST_LOCATION")
	              _T(", PRODUCT_SIZE, JOB_TYP, JOB_STATUS, JOB_PRIORITY, INS_DT, INS_USER_ID, REMARKS, WC_STEP )")
	              _T(" VALUES ('%s'")
	              _T(", (SELECT RIGHT('0000' + CAST(CASE WHEN ISNULL(MAX(CAST(LUGG_NO AS INT)),9000) >= 9900 THEN 9001 ELSE ISNULL(MAX(CAST(LUGG_NO AS INT)),9000)+1 END AS VARCHAR),4) FROM JOB_MST WHERE LUGG_NO LIKE '9[0-9][0-9][0-9]' AND LUGG_NO <= '9900')")
	              _T(", '%s', '%s', '%s', '%s', '0', '%s', '99', '100', ") + m_pDoc->SYSDATE + _T(", '%s', '%s', '0' )"),
	              (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strStartPos, (LPCTSTR)strStartLoc, (LPCTSTR)strDestPos, (LPCTSTR)strDestLoc,
	              (LPCTSTR)strJobTyp, (LPCTSTR)m_pDoc->m_strId, (LPCTSTR)strTag);

	m_pDoc->BeginTrans_DLG();
	if (!m_pDoc->ExcuteQueryString_DLG(strSql))
	{
		m_pDoc->RollbackTrans_DLG();
		AddLog(r + 1, bSto ? _T("작업생성(입고)") : _T("작업생성(출고)"), _T("-"), _T("Client"), _T("CSemiTestDlg::InsertJob"), CString(_T("작업 생성 실패 ")) + strTag);
		return FALSE;
	}
	// [LGLS 2026-08-31] 커밋 실패를 확인한다. 종전에는 반환값을 보지 않았고,
	//   ADO 가 던지는 _com_error 를 잡는 사람도 없어 프로세스가 abort() 로 죽었다.
	if (m_pDoc->CommitTrans_DLG() == 0)
	{
		m_pDoc->RollbackTrans_DLG();
		AddLog(r + 1, bSto ? _T("작업생성(입고)") : _T("작업생성(출고)"), _T("-"), _T("Client"),
		       _T("CSemiTestDlg::InsertJob"), CString(_T("커밋 실패 ")) + strTag);
		return FALSE;
	}

	// 방금 만든 작업번호 조회
	CString strMsg; int nCnt = 0;
	CString strSel = _T("SELECT TOP 1 LUGG_NO FROM JOB_MST WHERE WH_TYP = ") + CLib::Quot(m_pDoc->m_WH_TYP)
	               + _T(" AND REMARKS = ") + CLib::Quot(strTag) + _T(" ORDER BY INS_DT DESC");
	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSel, nCnt, strMsg);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	if (nCnt <= 0) { delete pRsw; AddLog(r + 1, _T("-"), _T("-"), _T("Client"), _T("CSemiTestDlg::InsertJob"), _T("생성 작업번호 조회 실패")); return FALSE; }
	pRsw->MoveFirst();
	m_row[r].strLugg = pRsw->GetItem(_T("LUGG_NO"));
	delete pRsw;

	m_strLastSta[r] = _T("99");
	CString strLog;
	if (bSto)
		strLog.Format(_T("%s → SC%s %s (입고)"), (LPCTSTR)strFr, (LPCTSTR)m_row[r].strScNo, (LPCTSTR)strDestLoc);
	else
		strLog.Format(_T("SC%s %s → %s (출고)"), (LPCTSTR)m_row[r].strScNo, (LPCTSTR)strStartLoc, (LPCTSTR)strTo);
	AddLog(r + 1, bSto ? _T("작업생성(입고)") : _T("작업생성(출고)"), m_row[r].strLugg, _T("Client"), _T("CSemiTestDlg::InsertJob"), strLog);
	return TRUE;
}

// 1=완료(입고 '29'=SC구동완료 / 출고 '19'=CV구동완료), 0=진행, -1=행 소멸
int CSemiTestDlg::CheckJob(int r)
{
	CString strMsg; int nCnt = 0;
	CString strSql = _T("SELECT JOB_TYP, JOB_STATUS FROM JOB_MST WHERE WH_TYP = ") + CLib::Quot(m_pDoc->m_WH_TYP)
	               + _T(" AND LUGG_NO = ") + CLib::Quot(m_row[r].strLugg);
	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	if (nCnt <= 0) { delete pRsw; return -1; }
	pRsw->MoveFirst();
	CString strTyp = pRsw->GetItem(_T("JOB_TYP"));
	CString strSta = pRsw->GetItem(_T("JOB_STATUS"));
	delete pRsw;

	CString strStat;
	strStat.Format(_T("%s LUGG=%s 상태=%s"), (m_row[r].nState == 1) ? _T("입고") : _T("출고"), (LPCTSTR)m_row[r].strLugg, (LPCTSTR)strSta);
	SetRowStat(r, strStat);

	// [LGLS 2026-08-13] JOB_STATUS 전이 로그 - 전이를 일으킨 IO_SCH 함수명을 병기
	if (strSta != m_strLastSta[r])
	{
		CString strTr;
		strTr.Format(_T("JOB_STATUS %s → %s"), (LPCTSTR)m_strLastSta[r], (LPCTSTR)strSta);
		AddLog(r + 1, _T("상태전이"), m_row[r].strLugg, _T("IO_SCH"), SchFuncForStatus(m_strLastSta[r], strSta), strTr);
		m_strLastSta[r] = strSta;
	}

	if (strTyp == _T("11") && strSta == _T("29")) return 1;
	if (strTyp == _T("12") && strSta == _T("19")) return 1;
	return 0;
}

// ★반자동 원칙: 완료돼도 HOST 로 보고하지 않는다. JOB_MST 에서 삭제만 한다.
void CSemiTestDlg::DeleteJob(int r)
{
	CString strSql = _T("DELETE FROM JOB_MST WHERE WH_TYP = ") + CLib::Quot(m_pDoc->m_WH_TYP)
	               + _T(" AND LUGG_NO = ") + CLib::Quot(m_row[r].strLugg);
	m_pDoc->BeginTrans_DLG();
	if (m_pDoc->ExcuteQueryString_DLG(strSql)) m_pDoc->CommitTrans_DLG();
	else m_pDoc->RollbackTrans_DLG();
	AddLog(r + 1, _T("작업삭제"), m_row[r].strLugg, _T("Client"), _T("CSemiTestDlg::DeleteJob"), _T("완료 → 삭제 (반자동: HOST 보고 없음)"));
}

void CSemiTestDlg::StepRow(int r)
{
	if (m_row[r].nState == 0)
	{
		if (!m_bRun) return;	// [LGLS 2026-08-13] 정지 상태에서는 새 작업을 만들지 않는다
		if (!RowActive(r)) return;
		// [LGLS 2026-08-19] 모드 분기: 직접 입력 = 입력 로케이션 반복 / 자동 = 크레인 순환 +1
		if (IsLocMode())
		{
			if (!ResolveLocRow(r)) return;
		}
		else
		{
			int nCrane = -1;
			if (!PickNextCrane(r, nCrane)) return;
			m_row[r].nCrane = nCrane;
			if (!NextCell(r, nCrane)) return;
		}
		if (InsertJob(r, TRUE)) m_row[r].nState = 1;
	}
	else if (m_row[r].nState == 1)
	{
		int nRet = CheckJob(r);
		if (nRet == -1) { AddLog(r + 1, _T("-"), m_row[r].strLugg, _T("Client"), _T("CSemiTestDlg::StepRow"), _T("입고 작업이 사라졌습니다 - 다음으로 진행")); m_row[r].nState = 0; return; }
		if (nRet == 1)
		{
			DeleteJob(r);
			// [LGLS 2026-08-13] 정지 요청 후에는 진행 중이던 작업만 마무리하고 다음(출고)을 만들지 않는다
			if (m_bRun && InsertJob(r, FALSE)) m_row[r].nState = 2;
			else m_row[r].nState = 0;
		}
	}
	else if (m_row[r].nState == 2)
	{
		int nRet = CheckJob(r);
		if (nRet == -1) { AddLog(r + 1, _T("-"), m_row[r].strLugg, _T("Client"), _T("CSemiTestDlg::StepRow"), _T("출고 작업이 사라졌습니다 - 다음으로 진행")); m_row[r].nState = 0; return; }
		if (nRet == 1)
		{
			DeleteJob(r);
			m_row[r].nState = 0;	// 다음 사이클 = 다음 크레인
		}
	}
}

// SC_DATA_LGLS 의 지시(OD)/보고(RD) 변화 로그 - "ECS가 뭘 적었고 PLC가 뭘 줬는지"
void CSemiTestDlg::TraceSc()
{
	BOOL bUse[CRANES] = { FALSE, };
	for (int r = 0; r < ROWS; r++)
		for (int k = 0; k < CRANES; k++)
			if (IsDlgButtonChecked(IDC_LGLS_ST_SC_BASE + r * CRANES + k)) bUse[k] = TRUE;
	// [LGLS 2026-08-19] 로케이션 직접 입력 모드: 진행 중 행이 확정한 크레인도 추적
	for (int r = 0; r < ROWS; r++)
		if (m_row[r].nState != 0 && m_row[r].nCrane >= 0 && m_row[r].nCrane < CRANES) bUse[m_row[r].nCrane] = TRUE;

	for (int k = 0; k < CRANES; k++)
	{
		if (!bUse[k]) continue;
		CString strScNo; strScNo.Format(_T("%d"), 901 + k);
		CString strMsg; int nCnt = 0;
		CString strSql =
			_T("SELECT ISNULL(JOB_TYP_OD,'') AS T_OD, ISNULL(LUGG_NO_FK1_OD,'') AS L_OD")
			_T(", ISNULL(START_BANK_FK1_OD,'')+'-'+ISNULL(START_BAY_FK1_OD,'')+'-'+ISNULL(START_LEVEL_FK1_OD,'') AS S_OD")
			_T(", ISNULL(DEST_BANK_FK1_OD,'')+'-'+ISNULL(DEST_BAY_FK1_OD,'')+'-'+ISNULL(DEST_LEVEL_FK1_OD,'') AS D_OD")
			_T(", ISNULL(TRANSFER_REQUEST_OD,'') AS REQ_OD")
			_T(", ISNULL(JOB_TYP_RD,'') AS T_RD, ISNULL(COMPLETE_RD,'') AS C_RD, ISNULL(LUGG_NO_FK1_RD,'') AS L_RDN")
			_T(", ISNULL(LOCATION_01_RD,'')+'-'+ISNULL(LOCATION_02_RD,'')+'-'+ISNULL(LOCATION_03_RD,'') AS LOC_RD")
			_T(", ISNULL(LOAD_COMPLETE_RD,'') AS LD_RD, ISNULL(UNLOAD_COMPLETE_RD,'') AS UL_RD")
			_T(", ISNULL(TRANSFER_COMPLETE_LOCATION_01_RD,'')+'-'+ISNULL(TRANSFER_COMPLETE_LOCATION_02_RD,'')+'-'+ISNULL(TRANSFER_COMPLETE_LOCATION_03_RD,'') AS TCL_RD")
			_T(", ISNULL(ALARM_SET_CODE_RD,'') AS ALM_RD")
			_T(" FROM SC_DATA_LGLS WHERE WH_TYP = ") + CLib::Quot(m_pDoc->m_WH_TYP)
			+ _T(" AND SC_NO = ") + CLib::Quot(strScNo);
		_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
		if (nCnt <= 0) { delete pRsw; continue; }
		pRsw->MoveFirst();
		CString strOd, strRd, strLuggOd, strLuggRd;
		strLuggOd = pRsw->GetItem(_T("L_OD"));
		strLuggRd = pRsw->GetItem(_T("L_RDN"));
		strOd.Format(_T("작업구분=%s FROM=%s TO=%s 전송요청=%s"),
			(LPCTSTR)pRsw->GetItem(_T("T_OD")),
			(LPCTSTR)pRsw->GetItem(_T("S_OD")), (LPCTSTR)pRsw->GetItem(_T("D_OD")),
			(LPCTSTR)pRsw->GetItem(_T("REQ_OD")));
		strRd.Format(_T("작업구분=%s 완료=%s 현재위치=%s 적재완료=%s 하역완료=%s 완료위치=%s 알람=%s"),
			(LPCTSTR)pRsw->GetItem(_T("T_RD")), (LPCTSTR)pRsw->GetItem(_T("C_RD")),
			(LPCTSTR)pRsw->GetItem(_T("LOC_RD")), (LPCTSTR)pRsw->GetItem(_T("LD_RD")),
			(LPCTSTR)pRsw->GetItem(_T("UL_RD")), (LPCTSTR)pRsw->GetItem(_T("TCL_RD")),
			(LPCTSTR)pRsw->GetItem(_T("ALM_RD")));
		delete pRsw;

		// 이 크레인을 쓰고 있는 로직(행) 찾기
		int nLogic = 0;
		for (int r = 0; r < ROWS; r++)
			if (m_row[r].nState != 0 && m_row[r].strScNo == strScNo) { nLogic = r + 1; break; }

		if (strOd != m_strScSnapOd[k])
		{
			// 지시(_OD)는 IO_SCH(DriveSC)가 DB에 발행하고 EQP_TASK 가 PLC 로 전송한다
			if (!m_strScSnapOd[k].IsEmpty())
				AddLog(nLogic, _T("ECS→SC") + strScNo, strLuggOd, _T("IO_SCH"), _T("cThread_SCH.DriveSC"), strOd);
			m_strScSnapOd[k] = strOd;
		}
		if (strRd != m_strScSnapRd[k])
		{
			// 보고(_RD)는 EQP_TASK(VehThread)가 PLC 에서 읽어 DB 에 기록한다
			if (!m_strScSnapRd[k].IsEmpty())
				AddLog(nLogic, _T("SC") + strScNo + _T("→ECS"), strLuggRd, _T("EQP_TASK"), _T("VehThread.PollObservations"), strRd);
			m_strScSnapRd[k] = strRd;
		}
	}
}

// 작업대(CV_DATA) 보고값 변화 로그
void CSemiTestDlg::TraceCv()
{
	CMapStringToPtr mapDone;
	for (int r = 0; r < ROWS; r++)
	{
		if (!RowActive(r)) continue;
		for (int i = 0; i < 2; i++)
		{
			CString strMc;
			GetDlgItemText((i == 0 ? IDC_LGLS_ST_FR1 : IDC_LGLS_ST_TO1) + r, strMc); strMc.Trim();
			if (strMc.IsEmpty()) continue;
			void* pDummy = NULL;
			if (mapDone.Lookup(strMc, pDummy)) continue;
			mapDone.SetAt(strMc, NULL);

			CString strMsg; int nCnt = 0;
			CString strSql =
				_T("SELECT ISNULL(LUGG_NO_RD,'') AS L_RD, ISNULL(JOB_TYP_RD,'') AS T_RD")
				_T(", ISNULL(DEST_POS_RD,'') AS D_RD, ISNULL(STO_READY_RD,'') AS STO, ISNULL(RET_READY_RD,'') AS RET")
				_T(" FROM CV_DATA WHERE WH_TYP = ") + CLib::Quot(m_pDoc->m_WH_TYP)
				+ _T(" AND MC_NO = ") + CLib::Quot(strMc);
			_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
			CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
			if (nCnt <= 0) { delete pRsw; continue; }
			pRsw->MoveFirst();
			CString strRd, strLuggRd;
			strLuggRd = pRsw->GetItem(_T("L_RD"));
			strRd.Format(_T("화물=%s 작업구분=%s 도착지=%s 입고대신호=%s 출고대신호=%s"),
				(LPCTSTR)strLuggRd, (LPCTSTR)pRsw->GetItem(_T("T_RD")),
				(LPCTSTR)pRsw->GetItem(_T("D_RD")), (LPCTSTR)pRsw->GetItem(_T("STO")),
				(LPCTSTR)pRsw->GetItem(_T("RET")));
			delete pRsw;

			CString strOld;
			if (!m_mapCvSnap.Lookup(strMc, strOld)) strOld = _T("");
			if (strRd != strOld)
			{
				// CV 보고(_RD)는 EQP_TASK(CvThread)가 PLC 에서 읽어 CV_DATA 에 기록한다
				if (!strOld.IsEmpty())
					AddLog(r + 1, _T("CV") + strMc + _T("→ECS"), strLuggRd, _T("EQP_TASK"), _T("CvThread.CvStatusScenario"), strRd);
				m_mapCvSnap.SetAt(strMc, strRd);
			}
		}
	}
}

// [LGLS 2026-08-13] 리사이즈 시 스킨 버튼은 비트맵(110x27) 크기를 유지한다.
//   비례 확대되면 캡션이 비트맵 바깥(오른쪽)에 그려져 글자가 떨어져 보인다.
void CSemiTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if (nType == SIZE_MINIMIZED) return;
	SIZE szL = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	CSkinButton* pBtns[] = { &m_btnStart, &m_btnStop, &m_btnClose };
	for (int i = 0; i < 3; i++)
	{
		if (!::IsWindow(pBtns[i]->m_hWnd)) continue;
		CRect r; pBtns[i]->GetWindowRect(&r); ScreenToClient(&r);
		pBtns[i]->MoveWindow(r.left, r.top, szL.cx, szL.cy);
	}
}

void CSemiTestDlg::OnBnClickedStart()
{
	if (m_bRun) return;
	BOOL bAny = FALSE;
	for (int r = 0; r < ROWS; r++)
		if (RowActive(r)) { bAny = TRUE; break; }
	if (!bAny)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지/도착지와 크레인을 설정한 행이 없습니다")));
		return;
	}
	m_bRun = TRUE;
	AddLog(0, _T("제어"), _T("-"), _T("Client"), _T("CSemiTestDlg::OnBnClickedStart"), _T("반자동 TEST 시작 (완료 작업은 HOST 보고 없이 삭제)"));
	SetTimer(1, 700, NULL);
}

void CSemiTestDlg::OnBnClickedStop()
{
	if (!m_bRun) return;
	m_bRun = FALSE;
	// [LGLS 2026-08-13] 정지 = 즉시 중단이 아니라 드레인:
	//   진행 중인 입고/출고 작업은 완료(삭제)까지 처리하고, 새 작업만 받지 않는다.
	BOOL bBusy = FALSE;
	for (int r = 0; r < ROWS; r++)
		if (m_row[r].nState != 0) { bBusy = TRUE; break; }
	if (bBusy)
		AddLog(0, _T("제어"), _T("-"), _T("Client"), _T("CSemiTestDlg::OnBnClickedStop"), _T("정지 요청 - 진행 중인 작업 완료 후 종료합니다 (새 작업 없음)"));
	else
	{
		KillTimer(1);
		AddLog(0, _T("제어"), _T("-"), _T("Client"), _T("CSemiTestDlg::OnBnClickedStop"), _T("정지"));
	}
}

void CSemiTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		for (int r = 0; r < ROWS; r++) StepRow(r);
		TraceSc();
		TraceCv();
		// [LGLS 2026-08-13] 정지 드레인 완료 판정: 모든 행이 대기면 타이머 종료
		if (!m_bRun)
		{
			BOOL bBusy = FALSE;
			for (int r = 0; r < ROWS; r++)
				if (m_row[r].nState != 0) { bBusy = TRUE; break; }
			if (!bBusy)
			{
				KillTimer(1);
				AddLog(0, _T("제어"), _T("-"), _T("Client"), _T("CSemiTestDlg::OnTimer"), _T("정지 완료 (진행 중이던 작업까지 정리됨)"));
			}
		}
	}
	CSkinDialog::OnTimer(nIDEvent);
}

void CSemiTestDlg::OnBnClickedCancel()
{
	OnClose();
}

void CSemiTestDlg::OnClose()
{
	// 모덜리스 - 숨기기만 한다(소유자 CManualJob 이 닫힐 때 파괴)
	// [LGLS 2026-08-13] 닫을 때도 정지와 같은 드레인: 진행 중 작업은 백그라운드에서 마무리
	if (m_bRun) OnBnClickedStop();
	ShowWindow(SW_HIDE);
}
