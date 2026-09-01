// PanelInfoDlg.cpp : [LGLS 2026-09-01] 각종 정보(CV/SC/RTV/작업) 도킹 판넬
#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "PanelInfoDlg.h"
#include "RecordSetWrap.h"
#include "Lib.h"

#define TIMER_PANEL_INFO     7302
#define TIMER_PANEL_INFO_MS  3000

enum { TAB_CV = 0, TAB_SC, TAB_RTV, TAB_JOB };

CPanelInfoDlg::CPanelInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelInfoDlg::IDD, pParent)
{
	m_pDoc = NULL;
}

void CPanelInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PANEL_INFO_TAB, m_tab);
	DDX_Control(pDX, IDC_PANEL_INFO_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CPanelInfoDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_PANEL_INFO_TAB, OnTabChanged)
END_MESSAGE_MAP()

BOOL CPanelInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_tab.InsertItem(TAB_CV,  _T("C/V"));
	m_tab.InsertItem(TAB_SC,  _T("S/C"));
	m_tab.InsertItem(TAB_RTV, _T("RTV"));
	m_tab.InsertItem(TAB_JOB, _T("작업"));

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	RebuildColumns();
	SetTimer(TIMER_PANEL_INFO, TIMER_PANEL_INFO_MS, NULL);
	Refresh();
	return TRUE;
}

static CString StatusText(CString strVal)
{
	strVal.Trim();
	if (strVal == _T("0")) return _T("0 DOWN");
	if (strVal == _T("1")) return _T("1 IDLE");
	if (strVal == _T("2")) return _T("2 RUN");
	return strVal;
}

void CPanelInfoDlg::RebuildColumns()
{
	while (m_list.DeleteColumn(0));
	m_list.DeleteAllItems();

	int nTab = m_tab.GetCurSel();
	switch (nTab)
	{
	case TAB_CV:
		m_list.InsertColumn(0, _T("호기"),     LVCFMT_LEFT,  55);
		m_list.InsertColumn(1, _T("작업번호"), LVCFMT_LEFT,  70);
		m_list.InsertColumn(2, _T("구분"),     LVCFMT_LEFT,  50);
		m_list.InsertColumn(3, _T("목적지"),   LVCFMT_LEFT,  60);
		m_list.InsertColumn(4, _T("에러"),     LVCFMT_LEFT,  55);
		m_list.InsertColumn(5, _T("자동"),     LVCFMT_LEFT,  45);
		break;
	case TAB_SC:
	case TAB_RTV:
		m_list.InsertColumn(0, _T("호기"),     LVCFMT_LEFT,  55);
		m_list.InsertColumn(1, _T("차상화물"), LVCFMT_LEFT,  75);
		m_list.InsertColumn(2, _T("상태"),     LVCFMT_LEFT,  70);
		m_list.InsertColumn(3, _T("에러"),     LVCFMT_LEFT,  55);
		m_list.InsertColumn(4, _T("위치"),     LVCFMT_LEFT, 100);
		if (nTab == TAB_RTV)
			m_list.InsertColumn(5, _T("지시작업"), LVCFMT_LEFT, 75);
		break;
	case TAB_JOB:
		m_list.InsertColumn(0, _T("항목"), LVCFMT_LEFT, 110);
		m_list.InsertColumn(1, _T("값"),   LVCFMT_LEFT, 190);
		break;
	}
}

void CPanelInfoDlg::Refresh()
{
	if (m_pDoc == NULL || !::IsWindow(m_list.m_hWnd))
		return;

	CString strSql;
	int nTab = m_tab.GetCurSel();
	switch (nTab)
	{
	case TAB_CV:
		strSql.Format(
			_T(" SELECT MC_NO, LUGG_NO_RD, JOB_TYP_RD, DEST_POS_RD, ERROR_CODE, AUTO_MODE_RD ")
			_T("   FROM CV_DATA WHERE WH_TYP = '%s' ORDER BY MC_NO "), (LPCTSTR)m_pDoc->m_WH_TYP);
		break;
	case TAB_SC:
		strSql.Format(
			_T(" SELECT MC_NO, PALLET_ON_VEHICLE_RD, SUBSYSTEM_STATUS_RD, ERR_CODE_RD ")
			_T("       ,LOCATION_01_RD, LOCATION_02_RD, LOCATION_03_RD ")
			_T("   FROM SC_DATA_LGLS WHERE WH_TYP = '%s' ORDER BY MC_NO "), (LPCTSTR)m_pDoc->m_WH_TYP);
		break;
	case TAB_RTV:
		strSql.Format(
			_T(" SELECT RTV_NO AS MC_NO, PALLET_ON_VEHICLE_RD, SUBSYSTEM_STATUS_RD, ERR_CODE_RD ")
			_T("       ,LOCATION_01_RD, LOCATION_02_RD, LOCATION_03_RD, LUGG_OD ")
			_T("   FROM RTV_DATA_LGLS WHERE WH_TYP = '%s' ORDER BY RTV_NO "), (LPCTSTR)m_pDoc->m_WH_TYP);
		break;
	case TAB_JOB:
		{
			m_list.DeleteAllItems();
			if (m_strJobNo.IsEmpty())
			{
				m_list.InsertItem(0, _T("안내"));
				m_list.SetItemText(0, 1, _T("작업 판넬에서 작업을 선택하세요"));
				return;
			}
			strSql.Format(
				_T(" SELECT LUGG_NO, JOB_TYP, JOB_STATUS, START_POS, START_LOCATION ")
				_T("       ,DEST_POS, DEST_LOCATION, LOT_NO, PRODUCT_ID, JOB_PRIORITY ")
				_T("       ,CONVERT(VARCHAR(19), INS_DT, 120) AS INS_DT ")
				_T("       ,CONVERT(VARCHAR(19), UPD_DT, 120) AS UPD_DT ")
				_T("   FROM JOB_MST WHERE WH_TYP = '%s' AND LUGG_NO = '%s' "),
				(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_strJobNo);
		}
		break;
	default:
		return;
	}

	int nRowCnt = -1;
	CString strMessage;
	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt < 0)
		return;
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);

	m_list.SetRedraw(FALSE);
	m_list.DeleteAllItems();

	if (nRowCnt > 0)
	{
		pRsw->MoveFirst();
		if (nTab == TAB_JOB)
		{
			static struct { LPCTSTR strName; LPCTSTR strCap; } JOBF[] = {
				{ _T("LUGG_NO"),        _T("작업번호") },   { _T("JOB_TYP"),      _T("작업구분") },
				{ _T("JOB_STATUS"),     _T("작업상태") },   { _T("START_POS"),    _T("출발") },
				{ _T("START_LOCATION"), _T("출발위치") },   { _T("DEST_POS"),     _T("도착") },
				{ _T("DEST_LOCATION"),  _T("도착위치") },   { _T("LOT_NO"),       _T("LOT") },
				{ _T("PRODUCT_ID"),     _T("제품") },       { _T("JOB_PRIORITY"), _T("우선순위") },
				{ _T("INS_DT"),         _T("등록시각") },   { _T("UPD_DT"),       _T("수정시각") },
			};
			for (int i = 0; i < (int)(sizeof(JOBF)/sizeof(JOBF[0])); i++)
			{
				m_list.InsertItem(i, JOBF[i].strCap);
				m_list.SetItemText(i, 1, pRsw->GetItem(JOBF[i].strName));
			}
		}
		else
		{
			for (int nRow = 0; nRow < nRowCnt; nRow++)
			{
				if (nTab == TAB_CV)
				{
					m_list.InsertItem(nRow, pRsw->GetItem(_T("MC_NO")));
					m_list.SetItemText(nRow, 1, pRsw->GetItem(_T("LUGG_NO_RD")));
					m_list.SetItemText(nRow, 2, pRsw->GetItem(_T("JOB_TYP_RD")));
					m_list.SetItemText(nRow, 3, pRsw->GetItem(_T("DEST_POS_RD")));
					m_list.SetItemText(nRow, 4, pRsw->GetItem(_T("ERROR_CODE")));
					m_list.SetItemText(nRow, 5, pRsw->GetItem(_T("AUTO_MODE_RD")));
				}
				else
				{
					CString strLoc;
					strLoc.Format(_T("%s/%s/%s"),
						(LPCTSTR)pRsw->GetItem(_T("LOCATION_01_RD")),
						(LPCTSTR)pRsw->GetItem(_T("LOCATION_02_RD")),
						(LPCTSTR)pRsw->GetItem(_T("LOCATION_03_RD")));
					m_list.InsertItem(nRow, pRsw->GetItem(_T("MC_NO")));
					m_list.SetItemText(nRow, 1, pRsw->GetItem(_T("PALLET_ON_VEHICLE_RD")));
					m_list.SetItemText(nRow, 2, StatusText(pRsw->GetItem(_T("SUBSYSTEM_STATUS_RD"))));
					m_list.SetItemText(nRow, 3, pRsw->GetItem(_T("ERR_CODE_RD")));
					m_list.SetItemText(nRow, 4, strLoc);
					if (nTab == TAB_RTV)
						m_list.SetItemText(nRow, 5, pRsw->GetItem(_T("LUGG_OD")));
				}
				pRsw->MoveNext();
			}
		}
	}
	delete pRsw;

	m_list.SetRedraw(TRUE);
	m_list.Invalidate(FALSE);
}

void CPanelInfoDlg::SetJob(CString strLuggNo)
{
	m_strJobNo = strLuggNo;
	if (!::IsWindow(m_tab.m_hWnd))
		return;
	if (m_tab.GetCurSel() != TAB_JOB)
	{
		m_tab.SetCurSel(TAB_JOB);
		RebuildColumns();
	}
	Refresh();
}

void CPanelInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (::IsWindow(m_tab.m_hWnd))
		m_tab.MoveWindow(0, 0, cx, 24);
	if (::IsWindow(m_list.m_hWnd))
		m_list.MoveWindow(0, 26, cx, cy - 26);
}

void CPanelInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_PANEL_INFO && IsWindowVisible())
		Refresh();
	CDialog::OnTimer(nIDEvent);
}

void CPanelInfoDlg::OnTabChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	RebuildColumns();
	Refresh();
}
