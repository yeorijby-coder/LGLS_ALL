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
	m_nStallSec = ::GetPrivateProfileInt(_T("USER"), _T("JOB_STALL_WARN_SEC"), 300, ECS_INI_FILE);
	if (m_nStallSec < 10) m_nStallSec = 10;
}

CWarningDlg::~CWarningDlg()
{
}

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
	m_ctlList.InsertColumn(3, _T("체류(초)"), LVCFMT_RIGHT,   70);
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
		ScanStalledJobs();
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

// 완료(29/19)가 아닌 작업이 기준시간 넘게 갱신되지 않으면 경고.
//   같은 상태로 머무는 동안 1회만 알리고, 상태가 바뀌면 다시 알린다.
//   자동 회복은 하지 않는다 - 실물 설비 상태를 모른 채 DB 를 건드리는 편이 더 위험하다.
void CWarningDlg::ScanStalledJobs()
{
	if (m_pDoc == NULL) return;

	// [LGLS 2026-08-22] 상태는 코드값 대신 COMMON_CODE(JOB_STATUS)의 이름으로 보여준다.
	//   언어 설정에 따라 KOR/ENG/HUN/CHIN 컬럼을 고르고, 코드가 없으면 원래 숫자를 그대로 쓴다.
	CString strNmCol = _T("CCD.CCD_NM_KOR");
	switch (m_pDoc->m_enLang)
	{
	case EN_ENG:  strNmCol = _T("CCD.CCD_NM_ENG");  break;
	case EN_HUN:  strNmCol = _T("CCD.CCD_NM_HUN");  break;
	case EN_CHIN: strNmCol = _T("CCD.CCD_NM_CHIN"); break;
	default: break;
	}

	CString strSql;
	strSql.Format(
		_T("SELECT JM.LUGG_NO, JM.JOB_STATUS, JM.START_POS, JM.DEST_POS, ")
		_T("       DATEDIFF(second, JM.UPD_DT, GETDATE()) AS IDLE_SEC, ")
		_T("       %s(NULLIF(LTRIM(RTRIM(%s)),''), JM.JOB_STATUS) AS STATUS_NM ")
		_T("  FROM JOB_MST JM ")
		_T("  LEFT OUTER JOIN COMMON_CODE CCD ")
		_T("         ON CCD.CDX_CD = 'JOB_STATUS' ")
		_T("        AND CCD.CCD_CD = JM.JOB_STATUS ")
		_T("        AND CCD.WH_TYP LIKE '%%' + JM.WH_TYP + '%%' ")
		_T(" WHERE JM.JOB_STATUS NOT IN ('29','19') ")
		_T("   AND DATEDIFF(second, JM.UPD_DT, GETDATE()) >= %d "),
		m_pDoc->NVL, strNmCol, m_nStallSec);

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
		CString strLugg   = pRsw->GetItem(_T("LUGG_NO"));
		CString strStatus = pRsw->GetItem(_T("JOB_STATUS"));
		CString strStatNm = pRsw->GetItem(_T("STATUS_NM"));
		if (strStatNm.Trim().IsEmpty()) strStatNm = strStatus;
		CString strIdle   = pRsw->GetItem(_T("IDLE_SEC"));
		CString strStart  = pRsw->GetItem(_T("START_POS"));
		CString strDest   = pRsw->GetItem(_T("DEST_POS"));

		CString strKey;
		strKey.Format(_T("%s|%s"), strLugg, strStatus);

		BOOL bKnown = FALSE;
		for (int k = 0; k < m_arrNotified.GetSize(); k++)
		{
			if (m_arrNotified.GetAt(k) == strKey) { bKnown = TRUE; break; }
		}
		if (!bKnown)
		{
			m_arrNotified.Add(strKey);

			CTime tmNow = CTime::GetCurrentTime();
			CString strRoute;
			strRoute.Format(_T("%s -> %s"), strStart, strDest);
			AddRow(tmNow.Format(_T("%H:%M:%S")), strLugg, strStatNm, strIdle, strRoute);

			strLast.Format(_T("작업 %s 이(가) 상태 '%s' 로 %s초째 진행되지 않습니다.\r\n%s\r\n설비 응답을 확인하세요."),
			                strLugg, strStatNm, strIdle, strRoute);
			nNew++;
		}
		pRsw->MoveNext();
	}
	delete pRsw;

	if (nNew > 0)
	{
		m_nCursor = 0;                     // 새 경고가 들어오면 최신 행을 가리킨다
		SetDlgItemText(IDC_STATIC_TIP, strLast);
		if (!m_bMute && !IsWindowVisible())
			ShowWindow(SW_SHOWNA);        // 포커스를 뺏지 않고 띄운다
	}
}
