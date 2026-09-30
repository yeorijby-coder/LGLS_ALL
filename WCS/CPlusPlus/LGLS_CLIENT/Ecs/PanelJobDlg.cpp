// PanelJobDlg.cpp : [LGLS 2026-09-01] 전체 작업(JOB_MST) 도킹 판넬 (작업구분 탭 필터)
#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "MainFrm.h"
#include "PanelJobDlg.h"
#include "RecordSetWrap.h"
#include "Lib.h"

#define TIMER_PANEL_JOB      7301
#define TIMER_PANEL_JOB_MS   2000	// [LGLS 2026-09-10] 자동 갱신 주기(사용자 지시)
#define IDC_PANEL_JOB_AUTO   2357	// 이 판넬 안에서만 쓰는 자식 ID

// 탭 구성 : 전체 / 입고 / 출고 / 피킹출고 / 랙투랙 / 호기간이동 / 이동
//   JOB_TYP : 1,11=입고  2,12=출고  3,13=피킹출고  4,14=랙투랙  5,15=호기간이동  0,6,10=이동
static LPCTSTR JOB_TABS[]    = { _T("전체"), _T("입고"), _T("출고"), _T("피킹출고"), _T("랙투랙"), _T("호기간이동"), _T("이동") };
static LPCTSTR JOB_FILTERS[] = { NULL, _T("('1','11')"), _T("('2','12')"), _T("('3','13')"),
                                 _T("('4','14')"), _T("('5','15')"), _T("('0','6','10')") };

CPanelJobDlg::CPanelJobDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelJobDlg::IDD, pParent)
{
	m_pDoc = NULL;
}

void CPanelJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PANEL_JOB_TAB, m_tabTyp);
	DDX_Control(pDX, IDC_PANEL_JOB_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CPanelJobDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_PANEL_JOB_TAB, OnTabChanged)
END_MESSAGE_MAP()

BOOL CPanelJobDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < (int)(sizeof(JOB_TABS)/sizeof(JOB_TABS[0])); i++)
		m_tabTyp.InsertItem(i, JOB_TABS[i]);

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	struct { LPCTSTR strHead; int nWidth; } COLS[] = {
		// [LGLS 2026-09-29] 출발/도착 한 칸에 위치까지 넣는다 (사용자 지시).
		//   랙 : S/C #1[00-000-00]   /   작업대 : 입출고대[101] TR#22
		//   그래서 「출발위치」「도착위치」 칸을 없앴다.
		// [LGLS 2026-09-30] 작업대 명칭이 EcsDefine.xml 것으로 바뀌며 길어졌다.
		//   "외부 전용 입출고대[101] TR#22" 가 들어가게 넓히고,
		//   판넬이 좁아 뒤 칸이 화면 밖으로 밀리므로 ★출발/도착을 앞으로★ 당겼다.
		//   나머지는 가로로 밀어 보면 된다.
		{ _T("작업번호"),  70 },
		{ _T("출발"),     210 }, { _T("도착"),    210 },
		{ _T("구분"),      90 }, { _T("상태"),    140 },
		{ _T("LOT"),       80 }, { _T("제품"),     80 },
		{ _T("우선"),      45 }, { _T("수정시각"), 125 },
	};
	for (int i = 0; i < (int)(sizeof(COLS)/sizeof(COLS[0])); i++)
		m_list.InsertColumn(i, COLS[i].strHead, LVCFMT_LEFT, COLS[i].nWidth);

	// [LGLS 2026-09-10] 자동 갱신 체크박스. 작업이 쌓이고 설비가 많이 움직이면
	//   목록이 계속 새로 그려져 눈이 아프므로, 필요할 때만 켜서 본다.
	m_chkAuto.Create(_T("자동 갱신"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		CRect(0, 0, 10, 10), this, IDC_PANEL_JOB_AUTO);
	m_chkAuto.SetFont(GetFont());
	m_chkAuto.SetCheck(BST_CHECKED);

	SetTimer(TIMER_PANEL_JOB, TIMER_PANEL_JOB_MS, NULL);
	Refresh();
	return TRUE;
}

CString CPanelJobDlg::TypFilter()
{
	int nTab = m_tabTyp.GetCurSel();
	if (nTab <= 0 || nTab >= (int)(sizeof(JOB_FILTERS)/sizeof(JOB_FILTERS[0])))
		return _T("");
	CString strCond;
	strCond.Format(_T("   AND JM.JOB_TYP IN %s "), JOB_FILTERS[nTab]);
	return strCond;
}

void CPanelJobDlg::Refresh()
{
	if (m_pDoc == NULL || !::IsWindow(m_list.m_hWnd))
		return;

	// [LGLS] ViewJobListDlg 의 조회와 같은 골격(코드명 조인) + 탭의 작업구분 필터
	CString strSql;
	strSql.Format(
		_T(" SELECT ") + m_pDoc->NVL + _T("(JM.LUGG_NO, ' ') AS LUGG_NO ")
		_T("       ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR, JM.JOB_TYP) AS JOB_TYP ")
		_T("       ,'[' + JM.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CC.CCD_NM_KOR, JM.JOB_STATUS) AS JOB_STATUS ")
		_T("       ,JM.START_POS, ") + m_pDoc->NVL + _T("(JM.START_LOCATION, ' ') AS START_LOCATION ")
		_T("       ,JM.DEST_POS,  ") + m_pDoc->NVL + _T("(JM.DEST_LOCATION, ' ') AS DEST_LOCATION ")
		_T("       ,") + m_pDoc->NVL + _T("(JM.LOT_NO, ' ') AS LOT_NO ")
		_T("       ,") + m_pDoc->NVL + _T("(JM.PRODUCT_ID, ' ') AS PRODUCT_ID ")
		_T("       ,") + m_pDoc->NVL + _T("(JM.JOB_PRIORITY, ' ') AS JOB_PRIORITY ")
		_T("       ,CONVERT(VARCHAR(19), JM.UPD_DT, 120) AS UPD_DT ")
		_T("   FROM JOB_MST JM ")
		_T("   LEFT OUTER JOIN COMMON_CODE CC ON CC.WH_TYP LIKE '%%%s%%' AND CC.CDX_CD = 'JOB_STATUS' AND JM.JOB_STATUS = CC.CCD_CD ")
		_T("   LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP ON CCD_JOB_TYP.WH_TYP LIKE '%%%s%%' AND CCD_JOB_TYP.CDX_CD = 'JOB_TYP' AND JM.JOB_TYP = CCD_JOB_TYP.CCD_CD ")
		_T("  WHERE JM.WH_TYP = '%s' ")
		_T("%s")
		_T("  ORDER BY JM.UPD_DT DESC, JM.LUGG_NO DESC "),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pDoc->m_WH_TYP,
		(LPCTSTR)TypFilter());
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

	// [LGLS 2026-09-29] 출발/도착은 코드와 위치를 합쳐 한 칸에 넣는다 (사용자 지시).
	//   FIELDS 에서 위치 두 칸을 뺐다 - 아래에서 PosLabel 이 합쳐 준다.
	//   [LGLS 2026-09-30] 칸 차례는 위 COLS 와 ★똑같아야 한다★.
	static LPCTSTR FIELDS[] = { _T("LUGG_NO"),
		_T("START_POS"), _T("DEST_POS"),
		_T("JOB_TYP"), _T("JOB_STATUS"),
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
			{
				CString strVal = pRsw->GetItem(FIELDS[nCol]);

				// [LGLS 2026-09-29] 출발/도착은 이름표 + 위치로 합쳐 보인다 (사용자 지시)
				if (CString(FIELDS[nCol]) == _T("START_POS"))
					strVal = m_pDoc->PosLabel(strVal, pRsw->GetItem(_T("START_LOCATION")), TRUE);
				else if (CString(FIELDS[nCol]) == _T("DEST_POS"))
					strVal = m_pDoc->PosLabel(strVal, pRsw->GetItem(_T("DEST_LOCATION")), TRUE);

				m_list.SetItemText(nRow, nCol, strVal);
			}
			pRsw->MoveNext();
		}
	}
	delete pRsw;

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
	const int nChkW = 92;	// [LGLS 2026-09-10] 오른쪽 위 [자동 갱신] 자리
	if (::IsWindow(m_tabTyp.m_hWnd))
		m_tabTyp.MoveWindow(0, 0, (cx > nChkW + 20) ? (cx - nChkW - 4) : cx, 24);
	if (::IsWindow(m_chkAuto.m_hWnd) && cx > nChkW + 20)
		m_chkAuto.MoveWindow(cx - nChkW, 4, nChkW - 4, 18);
	if (::IsWindow(m_list.m_hWnd))
		m_list.MoveWindow(0, 26, cx, cy - 26);
}

void CPanelJobDlg::OnTimer(UINT_PTR nIDEvent)
{
	// [LGLS 2026-09-10] 체크를 끄면 목록을 그대로 세워 둔다(사용자 지시).
	if (nIDEvent == TIMER_PANEL_JOB && IsWindowVisible()
		&& ::IsWindow(m_chkAuto.m_hWnd) && m_chkAuto.GetCheck() == BST_CHECKED)
		Refresh();
	CDialog::OnTimer(nIDEvent);
}

void CPanelJobDlg::OnTabChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	Refresh();
}

