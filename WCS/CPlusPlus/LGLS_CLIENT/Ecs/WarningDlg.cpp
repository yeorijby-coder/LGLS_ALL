// WarningDlg.cpp : implementation file
//
// [LGLS 2026-08-22] 작업 체류(설비 무응답) 경고창 - Client 표시용.
//   IO_TASK 도 같은 판정으로 로그를 남기지만 TASK 는 서버에서 돌아 사람이 보지 못한다.
//   운전자가 보는 Client 에서 창을 띄워 알린다.

#include "stdafx.h"
#include "Ecs.h"
#include "WarningDlg.h"
#include "EcsDoc.h"
#include "RecordSetWrap.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWarningDlg::CWarningDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
:CDialog(CWarningDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_bMute = FALSE;
	m_nCursor = -1;
}

CWarningDlg::~CWarningDlg()
{
}

// [LGLS 2026-09-22] 체류(지연) 알람 폐기 - ReloadIni()/JOB_STALL_WARN_SEC 제거. 경고창은 설비 에러만 알린다.

void CWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WARNING, m_ctlList);
}

BEGIN_MESSAGE_MAP(CWarningDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CWarningDlg::OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SHOW,   &CWarningDlg::OnButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_FIRST,  &CWarningDlg::OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREV,   &CWarningDlg::OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT,   &CWarningDlg::OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST,   &CWarningDlg::OnButtonLast)
END_MESSAGE_MAP()

BOOL CWarningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// [LGLS 2026-08-22] rc 의 CONTROL 정의에 LVS_REPORT 가 없어 아이콘 뷰로 생성된다.
	//   컬럼이 보이지 않고 항목이 한 줄로 뭉쳐 나오므로 여기서 리포트 뷰로 바꾼다.
	m_ctlList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	m_ctlList.SetExtendedStyle(m_ctlList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctlList.InsertColumn(0, _T("시각"),     LVCFMT_CENTER,  80);
	m_ctlList.InsertColumn(1, _T("작업번호"), LVCFMT_CENTER,  80);
	m_ctlList.InsertColumn(2, _T("구분"),     LVCFMT_CENTER, 120);
	m_ctlList.InsertColumn(3, _T("에러코드"), LVCFMT_CENTER,  70);	// [LGLS 2026-09-22] 체류(초) → 에러코드
	m_ctlList.InsertColumn(4, _T("내용"),     LVCFMT_LEFT,   280);

	SetDlgItemText(IDC_STATIC_TIP, _T(""));
	SetDlgItemText(IDC_BUTTON_SHOW, _T("STOP"));
	if (GetDlgItem(IDC_STATIC_PANE)) GetDlgItem(IDC_STATIC_PANE)->ShowWindow(SW_HIDE);

	// 화면 우하단에 고정 배치 (원본은 CDialogResize 로 처리했으나 그 클래스가 없다)
	CRect rcDlg, rcWork;
	GetWindowRect(rcDlg);
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
	SetWindowPos(&wndTop, rcWork.right - rcDlg.Width() - 12,
	              rcWork.bottom - rcDlg.Height() - 12, 0, 0, SWP_NOSIZE);

	SetTimer(TIMER_SCAN, 10000, NULL);      // 10초 주기 점검
	return TRUE;
}

// [ESC]/[Enter] 로 창이 파괴되지 않게 (숨김만)
void CWarningDlg::OnOK()     { ShowWindow(SW_HIDE); }
void CWarningDlg::OnCancel() { ShowWindow(SW_HIDE); }

void CWarningDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_SCAN)
	{
		ScanEquipErrors();	// [LGLS 2026-09-22] 설비 에러만 (체류 알람 폐기)
		ScanAlarmLogs();
	}
	CDialog::OnTimer(nIDEvent);
}

void CWarningDlg::OnButtonDelete()
{
	m_ctlList.DeleteAllItems();
	m_arrNotified.RemoveAll();
	m_nCursor = -1;
	SetDlgItemText(IDC_STATIC_TIP, _T(""));
}

// [LGLS 2026-08-22] |<< << >> >>| 목록 순회.
//   원본은 CStartupTip 이 알람 목록을 넘겼는데 그 클래스가 없어 버튼이 죽어 있었다.
//   지금은 리스트 행을 직접 넘기며 상단 팁에 그 행 내용을 보여준다.
void CWarningDlg::ShowRow(int nIndex)
{
	int nCnt = m_ctlList.GetItemCount();
	if (nCnt <= 0)
	{
		m_nCursor = -1;
		SetDlgItemText(IDC_STATIC_TIP, _T(""));
		return;
	}
	if (nIndex < 0)      nIndex = 0;
	if (nIndex >= nCnt)  nIndex = nCnt - 1;
	m_nCursor = nIndex;

	CString strTime   = m_ctlList.GetItemText(nIndex, 0);
	CString strLugg   = m_ctlList.GetItemText(nIndex, 1);
	CString strStatus = m_ctlList.GetItemText(nIndex, 2);
	CString strIdle   = m_ctlList.GetItemText(nIndex, 3);
	CString strRoute  = m_ctlList.GetItemText(nIndex, 4);

	CString strTip;
	strTip.Format(_T("[%d/%d]  %s   작업 %s 이(가) 상태 '%s' 로 %s초째 진행되지 않습니다.\r\n%s\r\n설비 응답을 확인하세요."),
	              nIndex + 1, nCnt, strTime, strLugg, strStatus, strIdle, strRoute);
	SetDlgItemText(IDC_STATIC_TIP, strTip);

	m_ctlList.SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ctlList.EnsureVisible(nIndex, FALSE);
}

void CWarningDlg::OnButtonFirst() { ShowRow(0); }
void CWarningDlg::OnButtonLast()  { ShowRow(m_ctlList.GetItemCount() - 1); }
void CWarningDlg::OnButtonPrev()  { ShowRow((m_nCursor < 0) ? 0 : m_nCursor - 1); }
void CWarningDlg::OnButtonNext()  { ShowRow((m_nCursor < 0) ? 0 : m_nCursor + 1); }

void CWarningDlg::OnButtonShow()
{
	// [LGLS 2026-08-23] STOP : 창을 닫고 이후로는 스스로 뜨지 않는다.
	//   다시 보려면 리본 [알람] 으로 강제로 띄운다(그때 자동 표시도 함께 다시 켜진다).
	//   종전에는 m_bMute 만 토글하고 창은 그대로 남아 있었다.
	m_bMute = TRUE;
	ShowWindow(SW_HIDE);
}

// 캡션이 없는 창이라 본문을 잡아 끌어 옮길 수 있게 한다(원본 동작 복원)
LRESULT CWarningDlg::OnNcHitTest(CPoint point)
{
	UINT hit = CDialog::OnNcHitTest(point);
	if (hit == HTCLIENT) return HTCAPTION;
	return hit;
}

BOOL CWarningDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	// 빨강 -> 노랑 그라데이션 (원본 배경 유지)
	for (int i = 0; i < rect.right; i++)
	{
		int g = (rect.right > 0) ? (i * 255 / rect.right) : 0;
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, g, 0));
		pDC->FillRect(CRect(i, 0, i + 1, rect.bottom), &brush);
	}
	return TRUE;
}

void CWarningDlg::AddRow(LPCTSTR lpszTime, LPCTSTR lpszLugg, LPCTSTR lpszStatus,
                         LPCTSTR lpszIdle, LPCTSTR lpszRoute)
{
	int n = m_ctlList.InsertItem(0, lpszTime);       // 최신이 위로
	m_ctlList.SetItemText(n, 1, lpszLugg);
	m_ctlList.SetItemText(n, 2, lpszStatus);
	m_ctlList.SetItemText(n, 3, lpszIdle);
	m_ctlList.SetItemText(n, 4, lpszRoute);

	// 너무 쌓이지 않게 상한
	while (m_ctlList.GetItemCount() > 200)
		m_ctlList.DeleteItem(m_ctlList.GetItemCount() - 1);
}

// [LGLS 2026-08-22] 구 EcsSv CLog::PumpupAlarm 대응 - 로그를 남기며 즉시 알람으로 띄운다.
void CWarningDlg::PumpupAlarm(LPCTSTR lpszWinId, LPCTSTR lpszLugg, LPCTSTR lpszMessage)
{
	CTime tmNow = CTime::GetCurrentTime();
	AddRow(tmNow.Format(_T("%H:%M:%S")), lpszLugg, lpszWinId, _T(""), lpszMessage);
	m_nCursor = 0;

	CString strTip;
	strTip.Format(_T("[알람] %s\r\n%s"), lpszWinId, lpszMessage);
	SetDlgItemText(IDC_STATIC_TIP, strTip);

	if (!m_bMute && !IsWindowVisible()) ShowWindow(SW_SHOWNA);
}

// WCS_CLIENT_LOG 에 ALARM_YN='Y' 로 남은 로그 중 아직 띄우지 않은 것을 가져와 표시한다.
//   표시한 로그는 ALARM_CHK_YN='Y' 로 마킹해 다시 뜨지 않게 한다.
//   (구 구조는 파일 로그라 그 PC 에서만 떴지만, DB 라 다른 Client 가 남긴 알람도 받는다)
void CWarningDlg::ScanAlarmLogs()
{
	if (m_pDoc == NULL) return;

	CString strSql;
	strSql.Format(
		_T("SELECT TOP 50 CONVERT(varchar(8), INS_DT, 108) AS LOG_TM, ")
		_T("       %s(LUGG_NO,'') AS LUGG_NO, %s(WIN_ID,'') AS WIN_ID, %s(MESSAGE,'') AS MESSAGE ")
		_T("  FROM WCS_CLIENT_LOG ")
		_T(" WHERE ALARM_YN = 'Y' ")
		_T("   AND %s(ALARM_CHK_YN,'N') <> 'Y' ")
		_T(" ORDER BY INS_DT "),
		m_pDoc->NVL, m_pDoc->NVL, m_pDoc->NVL, m_pDoc->NVL);

	int nRowCnt = -1;
	CString strMessage;
	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt <= 0) return;

	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
	CString strLast;
	int nNew = 0;

	pRsw->MoveFirst();
	for (int i = 0; i < nRowCnt; i++)
	{
		CString strTm   = pRsw->GetItem(_T("LOG_TM"));
		CString strLugg = pRsw->GetItem(_T("LUGG_NO"));
		CString strWin  = pRsw->GetItem(_T("WIN_ID"));
		CString strMsg  = pRsw->GetItem(_T("MESSAGE"));

		AddRow(strTm, strLugg, strWin, _T(""), strMsg);
		strLast.Format(_T("[알람] %s\r\n%s"), strWin, strMsg);
		nNew++;
		pRsw->MoveNext();
	}
	delete pRsw;

	if (nNew > 0)
	{
		// 띄운 알람은 확인 표시 - 같은 로그가 반복해서 뜨지 않게
		m_pDoc->ExcuteQueryString_DLG(
			_T("UPDATE WCS_CLIENT_LOG SET ALARM_CHK_YN = 'Y' ")
			_T(" WHERE ALARM_YN = 'Y' AND ") + m_pDoc->NVL + _T("(ALARM_CHK_YN,'N') <> 'Y' "));

		m_nCursor = 0;
		SetDlgItemText(IDC_STATIC_TIP, strLast);
		if (!m_bMute && !IsWindowVisible()) ShowWindow(SW_SHOWNA);
	}
}

// [LGLS 2026-09-22] ★설비 에러 감시★ (사용자 지시 - 작업 지연 알람은 폐기)
//   크레인(SC_DATA_LGLS) · RGV(RTV_DATA_LGLS) · 컨베이어(CV_DATA) 의 에러코드가 서면 그 설비를 알린다.
//   문구는 EQP_ECD_MST 에서 설비 구분과 코드로 찾는다(크레인은 Ecs.ini [SC_ERR] ERR_TYP 코드표).
//   같은 설비·같은 코드는 한 번만 알리고, 해제되면 기억에서 지워 다시 나면 또 알린다.
void CWarningDlg::ScanEquipErrors()
{
	if (m_pDoc == NULL) return;

	CString strScTyp = CLib::ScErrTyp();	// 크레인 코드표 구분 (기본 SC_LGLS)
	CString strSql;
	strSql.Format(
		_T("SELECT 'SC' AS KIND, SD.MC_NO AS EQP_NO, SD.ERR_CODE_RD AS ERR_CD, ")
		_T("       %s(NULLIF(LTRIM(RTRIM(EM.MSG_KOR)),''), '') AS ERR_MSG ")
		_T("  FROM SC_DATA_LGLS SD LEFT OUTER JOIN EQP_ECD_MST EM ")
		_T("         ON EM.EQP_TYP = '%s' AND EM.EQP_ERR_CD = SD.ERR_CODE_RD ")
		_T(" WHERE %s(SD.ERR_CODE_RD,'0000') NOT IN ('0','00','0000','') ")
		_T(" UNION ALL ")
		_T("SELECT 'RTV' AS KIND, RD.RTV_NO AS EQP_NO, RD.ERR_CODE_RD AS ERR_CD, ")
		_T("       %s(NULLIF(LTRIM(RTRIM(EM.MSG_KOR)),''), '') AS ERR_MSG ")
		_T("  FROM RTV_DATA_LGLS RD LEFT OUTER JOIN EQP_ECD_MST EM ")
		_T("         ON EM.EQP_TYP = 'RTV' AND EM.EQP_ERR_CD = RD.ERR_CODE_RD ")
		_T(" WHERE %s(RD.ERR_CODE_RD,'0000') NOT IN ('0','00','0000','') ")
		_T(" UNION ALL ")
		_T("SELECT 'CV' AS KIND, CD.MC_NO AS EQP_NO, CD.ERROR_CODE AS ERR_CD, ")
		_T("       %s(NULLIF(LTRIM(RTRIM(EM.MSG_KOR)),''), '') AS ERR_MSG ")
		_T("  FROM CV_DATA CD LEFT OUTER JOIN EQP_ECD_MST EM ")
		_T("         ON EM.EQP_TYP = 'CV' AND EM.EQP_ERR_CD = CD.ERROR_CODE ")
		_T(" WHERE %s(CD.ERROR_CODE,'0') NOT IN ('0','00','0000','') "),
		m_pDoc->NVL, (LPCTSTR)strScTyp, m_pDoc->NVL,
		m_pDoc->NVL, m_pDoc->NVL,
		m_pDoc->NVL, m_pDoc->NVL);

	int nRowCnt = -1;
	CString strMessage;
	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);

	CStringArray arrNow;	// 지금 에러가 선 설비·코드
	CString strLast;
	int nNew = 0;

	if (nRowCnt > 0)
	{
		CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
		pRsw->MoveFirst();
		for (int i = 0; i < nRowCnt; i++)
		{
			CString strKind = pRsw->GetItem(_T("KIND"));
			CString strEqp  = pRsw->GetItem(_T("EQP_NO"));
			CString strCd   = pRsw->GetItem(_T("ERR_CD"));
			CString strMsg  = pRsw->GetItem(_T("ERR_MSG"));
			strCd.Trim(); strMsg.Trim();

			CString strName;	// 보여줄 설비 이름
			if      (strKind == _T("SC"))  strName.Format(_T("크레인 %s"), (LPCTSTR)strEqp);
			else if (strKind == _T("RTV")) strName.Format(_T("RGV %s"), (LPCTSTR)strEqp);
			else                            strName.Format(_T("C/V 트랙 %s"), (LPCTSTR)strEqp);

			CString strKey;
			strKey.Format(_T("%s|%s|%s"), (LPCTSTR)strKind, (LPCTSTR)strEqp, (LPCTSTR)strCd);
			arrNow.Add(strKey);

			BOOL bKnown = FALSE;
			for (int k = 0; k < m_arrNotified.GetSize(); k++)
				if (m_arrNotified.GetAt(k) == strKey) { bKnown = TRUE; break; }
			if (!bKnown)
			{
				m_arrNotified.Add(strKey);
				CTime tmNow = CTime::GetCurrentTime();
				AddRow(tmNow.Format(_T("%H:%M:%S")), _T(""), strName, strCd, strMsg);
				if (strMsg.IsEmpty())
					strLast.Format(_T("%s 에 에러 [%s] 가 발생했습니다.\r\n설비 상태를 확인하세요."), (LPCTSTR)strName, (LPCTSTR)strCd);
				else
					strLast.Format(_T("%s 에러 [%s] %s\r\n설비 상태를 확인하세요."), (LPCTSTR)strName, (LPCTSTR)strCd, (LPCTSTR)strMsg);
				nNew++;
			}
			pRsw->MoveNext();
		}
		delete pRsw;
	}

	// 해제된 설비는 기억에서 지운다 - 같은 에러가 다시 나면 또 알리기 위해
	for (int k = (int)m_arrNotified.GetSize() - 1; k >= 0; k--)
	{
		BOOL bAlive = FALSE;
		for (int m = 0; m < arrNow.GetSize(); m++)
			if (arrNow.GetAt(m) == m_arrNotified.GetAt(k)) { bAlive = TRUE; break; }
		if (!bAlive) m_arrNotified.RemoveAt(k);
	}

	if (nNew > 0)
	{
		m_nCursor = 0;
		SetDlgItemText(IDC_STATIC_TIP, strLast);
		if (!m_bMute && !IsWindowVisible())
			ShowWindow(SW_SHOWNA);	// 포커스를 뺏지 않고 띄운다
	}
}


