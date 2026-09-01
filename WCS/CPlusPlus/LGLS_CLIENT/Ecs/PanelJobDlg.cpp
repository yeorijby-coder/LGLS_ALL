// PanelJobDlg.cpp : [LGLS 2026-09-01] 전체 작업(JOB_MST) 도킹 판넬
#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "MainFrm.h"
#include "PanelJobDlg.h"
#include "RecordSetWrap.h"
#include "Lib.h"

#define TIMER_PANEL_JOB      7301
#define TIMER_PANEL_JOB_MS   3000

CPanelJobDlg::CPanelJobDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelJobDlg::IDD, pParent)
{
	m_pDoc = NULL;
}

void CPanelJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PANEL_JOB_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CPanelJobDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PANEL_JOB_LIST, OnItemChanged)
END_MESSAGE_MAP()

BOOL CPanelJobDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	struct { LPCTSTR strHead; int nWidth; } COLS[] = {
		{ _T("작업번호"),  70 }, { _T("구분"),     90 }, { _T("상태"),    140 },
		{ _T("출발"),      55 }, { _T("출발위치"), 75 }, { _T("도착"),     55 },
		{ _T("도착위치"),  75 }, { _T("LOT"),      80 }, { _T("제품"),     80 },
		{ _T("우선"),      45 }, { _T("수정시각"), 125 },
	};
	for (int i = 0; i < (int)(sizeof(COLS)/sizeof(COLS[0])); i++)
		m_list.InsertColumn(i, COLS[i].strHead, LVCFMT_LEFT, COLS[i].nWidth);

	SetTimer(TIMER_PANEL_JOB, TIMER_PANEL_JOB_MS, NULL);
	Refresh();
	return TRUE;
}

void CPanelJobDlg::Refresh()
{
	if (m_pDoc == NULL || !::IsWindow(m_list.m_hWnd))
		return;

	// [LGLS] ViewJobListDlg 의 조회와 같은 골격(코드명 조인) - 필터 없이 전체
	CString strSql;
	strSql.Format(
		_T(" SELECT ") + m_pDoc->NVL + _T("(JM.LUGG_NO, ' ') AS LUGG_NO  ")
		_T("       ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR, JM.JOB_TYP) AS JOB_TYP  ")
		_T("       ,'[' + JM.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CC.CCD_NM_KOR, JM.JOB_STATUS) AS JOB_STATUS  ")
		_T("       ,JM.START_POS, ") + m_pDoc->NVL + _T("(JM.START_LOCATION, ' ') AS START_LOCATION  ")
		_T("       ,JM.DEST_POS,  ") + m_pDoc->NVL + _T("(JM.DEST_LOCATION, ' ') AS DEST_LOCATION  ")
		_T("       ,") + m_pDoc->NVL + _T("(JM.LOT_NO, ' ') AS LOT_NO  ")
		_T("       ,") + m_pDoc->NVL + _T("(JM.PRODUCT_ID, ' ') AS PRODUCT_ID  ")
		_T("       ,") + m_pDoc->NVL + _T("(JM.JOB_PRIORITY, ' ') AS JOB_PRIORITY  ")
		_T("       ,CONVERT(VARCHAR(19), JM.UPD_DT, 120) AS UPD_DT  ")
		_T("   FROM JOB_MST JM  ")
		_T("   LEFT OUTER JOIN COMMON_CODE CC ON CC.WH_TYP LIKE '%%%s%%' AND CC.CDX_CD = 'JOB_STATUS' AND JM.JOB_STATUS = CC.CCD_CD  ")
		_T("   LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP ON CCD_JOB_TYP.WH_TYP LIKE '%%%s%%' AND CCD_JOB_TYP.CDX_CD = 'JOB_TYP' AND JM.JOB_TYP = CCD_JOB_TYP.CCD_CD  ")
		_T("  WHERE JM.WH_TYP = '%s'  ")
		_T("  ORDER BY JM.UPD_DT DESC, JM.LUGG_NO DESC  "),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pDoc->m_WH_TYP);
	strSql = CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);

	int nRowCnt = -1;
	CString strMessage;
	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt < 0)
		return;
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);

	// 갱신 전 선택/스크롤 위치 기억
	CString strSelLugg;
	int nSel = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (nSel >= 0) strSelLugg = m_list.GetItemText(nSel, 0);
	int nTop = m_list.GetTopIndex();

	static LPCTSTR FIELDS[] = { _T("LUGG_NO"), _T("JOB_TYP"), _T("JOB_STATUS"),
		_T("START_POS"), _T("START_LOCATION"), _T("DEST_POS"), _T("DEST_LOCATION"),
		_T("LOT_NO"), _T("PRODUCT_ID"), _T("JOB_PRIORITY"), _T("UPD_DT") };

	m_list.SetRedraw(FALSE);
	m_list.DeleteAllItems();
	if (nRowCnt > 0)
	{
		pRsw->MoveFirst();
		for (int nRow = 0; nRow < nRowCnt; nRow++)
		{
			m_list.InsertItem(nRow, pRsw->GetItem(FIELDS[0]));
			for (int nCol = 1; nCol < (int)(sizeof(FIELDS)/sizeof(FIELDS[0])); nCol++)
				m_list.SetItemText(nRow, nCol, pRsw->GetItem(FIELDS[nCol]));
			pRsw->MoveNext();
		}
	}
	delete pRsw;

	// 선택/스크롤 복원
	if (!strSelLugg.IsEmpty())
	{
		LVFINDINFO fi; memset(&fi, 0, sizeof(fi));
		fi.flags = LVFI_STRING;
		fi.psz = (LPCTSTR)strSelLugg;
		int nFound = m_list.FindItem(&fi);
		if (nFound >= 0)
			m_list.SetItemState(nFound, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	if (nTop > 0 && m_list.GetItemCount() > 0)
		m_list.EnsureVisible(min(nTop + m_list.GetCountPerPage() - 1, m_list.GetItemCount() - 1), FALSE);
	m_list.SetRedraw(TRUE);
	m_list.Invalidate(FALSE);
}

void CPanelJobDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (::IsWindow(m_list.m_hWnd))
		m_list.MoveWindow(0, 0, cx, cy);
}

void CPanelJobDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_PANEL_JOB && IsWindowVisible())
		Refresh();
	CDialog::OnTimer(nIDEvent);
}

void CPanelJobDlg::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pNMLV = (NMLISTVIEW*)pNMHDR;
	*pResult = 0;
	if (!(pNMLV->uNewState & LVIS_SELECTED) || pNMLV->iItem < 0)
		return;

	CString strLugg = m_list.GetItemText(pNMLV->iItem, 0);
	strLugg.Trim();
	if (strLugg.IsEmpty())
		return;

	// [LGLS] 선택한 작업을 정보 판넬(작업 탭)에 표시
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (pFrame != NULL)
		pFrame->ShowJobDetail(strLugg);
}
