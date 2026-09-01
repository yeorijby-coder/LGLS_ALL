// PanelVehDlg.cpp : [LGLS 2026-09-01] Crane & Vehicle 반송 현황 판넬
//   구 ECS 메인화면의 "Crane & Vehicle 반송 현황" 표를 재현 - S/C#1~5 + RGV 의
//   차상(없으면 지시) 작업을 JOB_MST 와 조인해 작업번호/자재/팔렛/출발/도착을 표시.
#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "PanelVehDlg.h"
#include "RecordSetWrap.h"
#include "Lib.h"

#define TIMER_PANEL_VEH      7303
#define TIMER_PANEL_VEH_MS   2000

CPanelVehDlg::CPanelVehDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelVehDlg::IDD, pParent)
{
	m_pDoc = NULL;
}

void CPanelVehDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PANEL_VEH_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CPanelVehDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static LPCTSTR VEH_NAMES[] = { _T("S/C#1"), _T("S/C#2"), _T("S/C#3"), _T("S/C#4"), _T("S/C#5"), _T("RGV") };
static LPCTSTR VEH_KEYS[]  = { _T("901"), _T("902"), _T("903"), _T("904"), _T("905"), _T("801") };

BOOL CPanelVehDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	struct { LPCTSTR strHead; int nWidth; } COLS[] = {
		{ _T(""),         55 }, { _T("작업번호"), 70 }, { _T("자재"),   90 },
		{ _T("팔렛"),     80 }, { _T("출발"),    95 }, { _T("도착"),   95 },
	};
	for (int i = 0; i < (int)(sizeof(COLS)/sizeof(COLS[0])); i++)
		m_list.InsertColumn(i, COLS[i].strHead, LVCFMT_LEFT, COLS[i].nWidth);

	for (int i = 0; i < 6; i++)
		m_list.InsertItem(i, VEH_NAMES[i]);

	SetTimer(TIMER_PANEL_VEH, TIMER_PANEL_VEH_MS, NULL);
	Refresh();
	return TRUE;
}

void CPanelVehDlg::Refresh()
{
	if (m_pDoc == NULL || !::IsWindow(m_list.m_hWnd))
		return;

	CString strSql;
	strSql.Format(
		_T(" SELECT V.UNIT_NO, LTRIM(RTRIM(V.LUGG)) AS LUGG, V.ON_BOARD ")
		_T("       ,") + m_pDoc->NVL + _T("(J.LUGG_NO, '') AS JLUGG ")
		_T("       ,") + m_pDoc->NVL + _T("(J.PRODUCT_ID, '') AS PRODUCT_ID ")
		_T("       ,") + m_pDoc->NVL + _T("(J.LOT_NO, '') AS LOT_NO ")
		_T("       ,") + m_pDoc->NVL + _T("(J.START_POS, '') + ' ' + ") + m_pDoc->NVL + _T("(J.START_LOCATION, '') AS S_POS ")
		_T("       ,") + m_pDoc->NVL + _T("(J.DEST_POS, '') + ' ' + ") + m_pDoc->NVL + _T("(J.DEST_LOCATION, '') AS D_POS ")
		_T("   FROM ( SELECT MC_NO AS UNIT_NO ")
		_T("               , CASE WHEN LTRIM(RTRIM(") + m_pDoc->NVL + _T("(PALLET_ON_VEHICLE_RD,''))) NOT IN ('','0','0000') THEN PALLET_ON_VEHICLE_RD ELSE PALLET_ID_OD END AS LUGG ")
		_T("               , CASE WHEN LTRIM(RTRIM(") + m_pDoc->NVL + _T("(PALLET_ON_VEHICLE_RD,''))) NOT IN ('','0','0000') THEN 1 ELSE 0 END AS ON_BOARD ")
		_T("            FROM SC_DATA_LGLS WHERE WH_TYP = '%s' ")
		_T("          UNION ALL ")
		_T("          SELECT RTV_NO ")
		_T("               , CASE WHEN LTRIM(RTRIM(") + m_pDoc->NVL + _T("(PALLET_ON_VEHICLE_RD,''))) NOT IN ('','0','0000') THEN PALLET_ON_VEHICLE_RD ELSE ") + m_pDoc->NVL + _T("(LUGG_OD,'') END ")
		_T("               , CASE WHEN LTRIM(RTRIM(") + m_pDoc->NVL + _T("(PALLET_ON_VEHICLE_RD,''))) NOT IN ('','0','0000') THEN 1 ELSE 0 END ")
		_T("            FROM RTV_DATA_LGLS WHERE WH_TYP = '%s' ) V ")
		_T("   LEFT OUTER JOIN JOB_MST J ON J.WH_TYP = '%s' AND J.LUGG_NO = LTRIM(RTRIM(V.LUGG)) "),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pDoc->m_WH_TYP);

	int nCnt = -1; CString strMsg;
	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
	if (nCnt < 0)
		return;

	m_list.SetRedraw(FALSE);
	for (int i = 0; i < 6; i++)
		for (int c = 1; c <= 5; c++)
			m_list.SetItemText(i, c, _T(""));

	if (nCnt > 0)
	{
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
		pRsw->MoveFirst();
		for (int r = 0; r < nCnt; r++)
		{
			CString strUnit = pRsw->GetItem(_T("UNIT_NO")); strUnit.Trim();
			int nRow = -1;
			for (int i = 0; i < 6; i++)
				if (strUnit == VEH_KEYS[i]) { nRow = i; break; }
			if (nRow >= 0)
			{
				CString strLugg = pRsw->GetItem(_T("LUGG")); strLugg.Trim();
				if (strLugg == _T("0") || strLugg == _T("0000")) strLugg = _T("");
				// [LGLS] 지시(OD) 유래 번호는 JOB_MST 에 실재할 때만 표시 - 죽은 지시 잔값 숨김
				CString strOn = pRsw->GetItem(_T("ON_BOARD")); strOn.Trim();
				CString strJl = pRsw->GetItem(_T("JLUGG"));   strJl.Trim();
				if (strOn != _T("1") && strJl.IsEmpty()) strLugg = _T("");
				m_list.SetItemText(nRow, 1, strLugg);
				m_list.SetItemText(nRow, 2, pRsw->GetItem(_T("PRODUCT_ID")));
				m_list.SetItemText(nRow, 3, pRsw->GetItem(_T("LOT_NO")));
				m_list.SetItemText(nRow, 4, pRsw->GetItem(_T("S_POS")));
				m_list.SetItemText(nRow, 5, pRsw->GetItem(_T("D_POS")));
			}
			pRsw->MoveNext();
		}
		delete pRsw;
	}
	m_list.SetRedraw(TRUE);
	m_list.Invalidate(FALSE);
}

void CPanelVehDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (::IsWindow(m_list.m_hWnd))
		m_list.MoveWindow(0, 0, cx, cy);
}

void CPanelVehDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_PANEL_VEH && IsWindowVisible())
		Refresh();
	CDialog::OnTimer(nIDEvent);
}
