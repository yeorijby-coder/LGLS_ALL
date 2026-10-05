// PanelJobDlg.cpp : [LGLS 2026-09-01] 전체 작업(JOB_MST) 도킹 판넬 (작업구분 탭 필터)
//   [LGLS 2026-10-01] 구 ECS 하단 반송 판넬(MonitorMainTransferListPanel)과 같게 셋으로 나눈다 (사용자 지시)
//     왼쪽  : 우선순위 값 / ▲ ▼ (JOB_PRIORITY ±1) / [반송조정](작업정보 창)
//     가운데: 작업 목록 (종전 그대로)
//     오른쪽: 선택 작업의 상세 - ECS번호(작업번호) / 작업번호(적재용기) + 단계표(SEQ/디바이스/시작/도착/상태) + [완료처리]
//   구 ECS 는 명령 단계표(TB_TRANSFERDETAIL)를 따로 두었지만 신 ECS 는 JOB_STATUS 하나로 흐르므로,
//   작업구분과 상태 코드에서 단계를 만들어 보인다 (BuildSeqRows).
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
	m_nSplitR = 0; m_nSplitB = 0; m_bSplitVert = FALSE; m_bDragSplit = FALSE; m_rcSplit.SetRectEmpty(); m_nRcLeftW = 0; m_nSplitMode = 0; m_nSeqPhase = -1; m_nSeqHs = 0;
}

void CPanelJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PANEL_JOB_TAB, m_tabTyp);
	DDX_Control(pDX, IDC_PANEL_JOB_LIST, m_list);
	// [LGLS 2026-10-01] 리소스(IDD_PANEL_JOB)의 컨트롤
	DDX_Control(pDX, IDC_PANEL_JOB_PRI_LBL,  m_lblPriTitle);
	DDX_Control(pDX, IDC_PANEL_JOB_PRI_VAL,  m_lblPriVal);
	DDX_Control(pDX, IDC_PANEL_JOB_PRI_UP,   m_btnPriUp);
	DDX_Control(pDX, IDC_PANEL_JOB_PRI_DN,   m_btnPriDn);
	DDX_Control(pDX, IDC_PANEL_JOB_TRANSFER, m_btnTransfer);
	DDX_Control(pDX, IDC_PANEL_JOB_SPLIT_BTN, m_btnSplit);
	DDX_Control(pDX, IDC_PANEL_JOB_ECS_LBL,  m_lblEcs);
	DDX_Control(pDX, IDC_PANEL_JOB_ECS_VAL,  m_lblEcsVal);
	DDX_Control(pDX, IDC_PANEL_JOB_JOB_LBL,  m_lblJob);
	DDX_Control(pDX, IDC_PANEL_JOB_JOB_VAL,  m_lblJobVal);
	DDX_Control(pDX, IDC_PANEL_JOB_SEQ,      m_listSeq);
	DDX_Control(pDX, IDC_PANEL_JOB_COMPLETE, m_btnComplete);
}

BEGIN_MESSAGE_MAP(CPanelJobDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_PANEL_JOB_TAB, OnTabChanged)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PANEL_JOB_LIST, OnListItemChanged)
	ON_BN_CLICKED(IDC_PANEL_JOB_PRI_UP,   OnPriUp)
	ON_BN_CLICKED(IDC_PANEL_JOB_PRI_DN,   OnPriDown)
	ON_BN_CLICKED(IDC_PANEL_JOB_TRANSFER, OnTransferCtl)
	ON_BN_CLICKED(IDC_PANEL_JOB_COMPLETE, OnComplete)
	ON_BN_CLICKED(IDC_PANEL_JOB_SPLIT_BTN, OnSplitToggle)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CPanelJobDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < (int)(sizeof(JOB_TABS)/sizeof(JOB_TABS[0])); i++)
		m_tabTyp.InsertItem(i, JOB_TABS[i]);

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// [LGLS 2026-10-01] 목록 열은 Ecs.ini [DISPLAY] JOB_PANEL_COLS 로 순서를 정한다 (사용자 지시)
	//   예) JOB_PANEL_COLS=LUGG,TYP,STA,LOT,PROD,START,DEST        (기본 - 구 ECS 반송 목록 순서를 참조)
	//       폭을 같이 주려면 LUGG:70,TYP:90,...   없는 토큰은 무시, 빈 값이면 기본 순서
	//   토큰 : LUGG 작업번호 / TYP 구분 / STA 상태 / LOT / PROD 제품 / START 출발 / DEST 도착 / PRI 우선 / UPD 수정시각
	BuildColumnOrder();
	for (int i = 0; i < (int)m_arCols.GetCount(); i++)
		m_list.InsertColumn(i, COL_DEF[m_arCols[i]].strHead, LVCFMT_LEFT, CLib::DpiPx(m_arColW[i]));
	BuildOldEcsControls();

	SetTimer(TIMER_PANEL_JOB, TIMER_PANEL_JOB_MS, NULL);
	Refresh();
	return TRUE;
}

// [LGLS 2026-10-01] 구 ECS 판넬의 왼쪽(우선순위)과 오른쪽(상세) 칸을 만든다
void CPanelJobDlg::BuildOldEcsControls()
{
	// [LGLS 2026-10-01] ★리소스 방식★ 컨트롤은 Ecs.rc 의 IDD_PANEL_JOB 에 있다 (사용자 지시 - 리소스에서 편집).
	//   여기서는 만들지 않고 받아만 온다(DoDataExchange 의 DDX_Control). 글꼴·열만 맞춘다.
	//   왼쪽 칸(우선순위/▲▼/반송조정)은 리소스 좌표를 그대로 두고(OnSize 가 옮기지 않는다),
	//   오른쪽 머리줄(ECS번호/작업번호/완료처리)은 리소스의 상대 위치를 유지한 채 칸의 왼쪽 끝만 옮긴다.
	CFont* pFont = GetFont();
	m_listSeq.SetExtendedStyle(m_listSeq.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	struct { LPCTSTR strHead; int nWidth; } SEQCOLS[] = {
		{ _T(""), 50 }, { _T("SEQ"), 45 }, { _T("디바이스"), 80 }, { _T("시작"), 150 }, { _T("도착"), 150 },
	};
	if (m_listSeq.GetHeaderCtrl() == NULL || m_listSeq.GetHeaderCtrl()->GetItemCount() == 0)
		for (int i = 0; i < (int)(sizeof(SEQCOLS)/sizeof(SEQCOLS[0])); i++)
			m_listSeq.InsertColumn(i, SEQCOLS[i].strHead, LVCFMT_LEFT, CLib::DpiPx(SEQCOLS[i].nWidth));

	CWnd* pAll[] = { &m_lblPriTitle, &m_lblPriVal, &m_btnPriUp, &m_btnPriDn, &m_btnTransfer, &m_btnSplit,
	                 &m_lblEcs, &m_lblEcsVal, &m_lblJob, &m_lblJobVal, &m_listSeq, &m_btnComplete };
	for (int i = 0; i < (int)(sizeof(pAll)/sizeof(pAll[0])); i++)
		if (pFont != NULL && ::IsWindow(pAll[i]->m_hWnd)) pAll[i]->SetFont(pFont);

	// 리소스 좌표 기억 : 왼쪽 칸 폭 = 왼쪽 컨트롤들의 오른쪽 끝, 오른쪽 머리줄은 ECS번호 라벨 기준 상대 위치
	CRect rc;
	m_nRcLeftW = 0;
	CWnd* pL[] = { &m_lblPriTitle, &m_lblPriVal, &m_btnPriUp, &m_btnPriDn, &m_btnTransfer, &m_btnSplit };
	for (int i = 0; i < 6; i++)
		if (::IsWindow(pL[i]->m_hWnd)) { pL[i]->GetWindowRect(&rc); ScreenToClient(&rc); if (rc.right > m_nRcLeftW) m_nRcLeftW = rc.right; }
	CWnd* pR[] = { &m_lblEcs, &m_lblEcsVal, &m_lblJob, &m_lblJobVal, &m_btnComplete, &m_listSeq };
	int x0 = 0;
	if (::IsWindow(m_lblEcs.m_hWnd)) { m_lblEcs.GetWindowRect(&rc); ScreenToClient(&rc); x0 = rc.left - 4; }
	for (int i = 0; i < 6; i++)
	{
		m_rcRcRight[i].SetRectEmpty();
		if (::IsWindow(pR[i]->m_hWnd)) { pR[i]->GetWindowRect(&rc); ScreenToClient(&rc); rc.OffsetRect(-x0, 0); m_rcRcRight[i] = rc; }
	}

	// 우선순위 숫자는 구 ECS 처럼 크게
	static CFont s_fntBig;
	if (s_fntBig.GetSafeHandle() == NULL)
	{
		LOGFONT lf = {0};
		if (pFont != NULL) pFont->GetLogFont(&lf);
		lf.lfHeight = -CLib::DpiPx(20); lf.lfWeight = FW_BOLD;
		s_fntBig.CreateFontIndirect(&lf);
	}
	if (s_fntBig.GetSafeHandle() != NULL && ::IsWindow(m_lblPriVal.m_hWnd)) m_lblPriVal.SetFont(&s_fntBig);
}

// [LGLS 2026-10-01] 목록 열 카탈로그 (토큰, 머리글, 기본 폭, 조회 필드)
const CPanelJobDlg::COLDEF CPanelJobDlg::COL_DEF[] = {
	{ _T("LUGG"),  _T("작업번호"),  70, _T("LUGG_NO") },
	{ _T("TYP"),   _T("구분"),      90, _T("JOB_TYP") },
	{ _T("STA"),   _T("상태"),     140, _T("JOB_STATUS") },
	{ _T("LOT"),   _T("LOT"),       80, _T("LOT_NO") },
	{ _T("PROD"),  _T("제품"),      80, _T("PRODUCT_ID") },
	{ _T("START"), _T("출발"),     210, _T("START_POS") },
	{ _T("DEST"),  _T("도착"),     210, _T("DEST_POS") },
	{ _T("PRI"),   _T("우선"),      45, _T("JOB_PRIORITY") },
	{ _T("UPD"),   _T("수정시각"), 125, _T("UPD_DT") },
};
const int CPanelJobDlg::COL_DEF_N = sizeof(CPanelJobDlg::COL_DEF) / sizeof(CPanelJobDlg::COL_DEF[0]);

void CPanelJobDlg::BuildColumnOrder()
{
	m_arCols.RemoveAll(); m_arColW.RemoveAll();
	TCHAR buf[512] = { 0 };
	::GetPrivateProfileString(_T("DISPLAY"), _T("JOB_PANEL_COLS"), _T(""), buf, 511, ECS_INI_FILE);
	CString strCols = buf; strCols.Trim();
	if (strCols.IsEmpty()) strCols = _T("LUGG,TYP,STA,LOT,PROD,START,DEST");
	int nPos = 0;
	CString tok = strCols.Tokenize(_T(",; "), nPos);
	while (!tok.IsEmpty())
	{
		CString name = tok, w; int c = tok.Find(_T(':'));
		if (c >= 0) { name = tok.Left(c); w = tok.Mid(c + 1); }
		name.Trim(); name.MakeUpper();
		for (int i = 0; i < COL_DEF_N; i++)
			if (name == COL_DEF[i].strKey)
			{
				m_arCols.Add(i);
				int nW = _ttoi(w); m_arColW.Add(nW > 0 ? nW : COL_DEF[i].nWidth);
				break;
			}
		tok = strCols.Tokenize(_T(",; "), nPos);
	}
	if (m_arCols.GetCount() == 0) { m_arCols.Add(0); m_arColW.Add(COL_DEF[0].nWidth); }
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

int CPanelJobDlg::FindRow(const CString& strLugg)
{
	for (int i = 0; i < m_arRow.GetCount(); i++)
		if (m_arRow[i].lugg == strLugg) return i;
	return -1;
}

void CPanelJobDlg::Refresh()
{
	if (m_pDoc == NULL || !::IsWindow(m_list.m_hWnd))
		return;

	// [LGLS] ViewJobListDlg 의 조회와 같은 골격(코드명 조인) + 탭의 작업구분 필터
	//   [LGLS 2026-10-01] 원시 코드(JOB_TYP_CD/JOB_STATUS_CD)와 통로/크레인(HS_TRACK_NO/SC_NO)도 함께 읽는다 - 오른쪽 단계표용
	CString strSql;
	strSql.Format(
		_T(" SELECT ") + m_pDoc->NVL + _T("(JM.LUGG_NO, ' ') AS LUGG_NO ")
		_T("       ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR, JM.JOB_TYP) AS JOB_TYP ")
		_T("       ,'[' + JM.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CC.CCD_NM_KOR, JM.JOB_STATUS) AS JOB_STATUS ")
		_T("       ,JM.JOB_TYP AS JOB_TYP_CD, JM.JOB_STATUS AS JOB_STATUS_CD ")
		_T("       ,") + m_pDoc->NVL + _T("(JM.HS_TRACK_NO, ' ') AS HS_TRACK_NO, ") + m_pDoc->NVL + _T("(JM.SC_NO, ' ') AS SC_NO ")
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
	if (nSel >= 0 && nSel < m_arRow.GetCount()) strSelLugg = m_arRow[nSel].lugg;	// [LGLS 2026-10-01] 작업번호 열이 0 번이 아닐 수 있다
	int nTop = m_list.GetTopIndex();

	// [LGLS 2026-09-29] 출발/도착은 코드와 위치를 합쳐 한 칸에 넣는다 (사용자 지시).
	//   [LGLS 2026-09-30] 칸 차례는 위 COLS 와 ★똑같아야 한다★.
	// [LGLS 2026-10-01] 열은 m_arCols(ini 순서) 대로 채운다

	m_list.SetRedraw(FALSE);
	m_list.DeleteAllItems();
	m_arRow.RemoveAll();
	if (nRowCnt > 0)
	{
		pRsw->MoveFirst();
		for (int nRow = 0; nRow < nRowCnt; nRow++)
		{
			m_list.InsertItem(nRow, _T(""));
			for (int nCol = 0; nCol < (int)m_arCols.GetCount(); nCol++)
			{
				LPCTSTR pszField = COL_DEF[m_arCols[nCol]].strField;
				CString strVal = pRsw->GetItem(pszField);

				// [LGLS 2026-09-29] 출발/도착은 이름표 + 위치로 합쳐 보인다 (사용자 지시)
				if (CString(pszField) == _T("START_POS"))
					strVal = m_pDoc->PosLabel(strVal, pRsw->GetItem(_T("START_LOCATION")), TRUE);
				else if (CString(pszField) == _T("DEST_POS"))
					strVal = m_pDoc->PosLabel(strVal, pRsw->GetItem(_T("DEST_LOCATION")), TRUE);

				m_list.SetItemText(nRow, nCol, strVal);
			}
			ROW r;
			r.lugg     = pRsw->GetItem(_T("LUGG_NO"));       r.lugg.Trim();
			r.typCd    = pRsw->GetItem(_T("JOB_TYP_CD"));    r.typCd.Trim();
			r.staCd    = pRsw->GetItem(_T("JOB_STATUS_CD")); r.staCd.Trim();
			r.startPos = pRsw->GetItem(_T("START_POS"));     r.startPos.Trim();
			r.startLoc = pRsw->GetItem(_T("START_LOCATION")); r.startLoc.Trim();
			r.destPos  = pRsw->GetItem(_T("DEST_POS"));      r.destPos.Trim();
			r.destLoc  = pRsw->GetItem(_T("DEST_LOCATION")); r.destLoc.Trim();
			r.hs       = pRsw->GetItem(_T("HS_TRACK_NO"));   r.hs.Trim();
			r.sc       = pRsw->GetItem(_T("SC_NO"));         r.sc.Trim();
			r.pri      = pRsw->GetItem(_T("JOB_PRIORITY"));  r.pri.Trim();
			r.lot      = pRsw->GetItem(_T("LOT_NO"));        r.lot.Trim();
			m_arRow.Add(r);
			pRsw->MoveNext();
		}
	}
	delete pRsw;

	int nFound = -1;
	if (!strSelLugg.IsEmpty())
	{
		LVFINDINFO fi; memset(&fi, 0, sizeof(fi));
		fi.flags = LVFI_STRING;
		fi.psz = (LPCTSTR)strSelLugg;
		nFound = m_list.FindItem(&fi);
		if (nFound >= 0)
			m_list.SetItemState(nFound, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	if (nTop > 0 && m_list.GetItemCount() > 0)
		m_list.EnsureVisible(min(nTop + m_list.GetCountPerPage() - 1, m_list.GetItemCount() - 1), FALSE);
	m_list.SetRedraw(TRUE);
	m_list.Invalidate(FALSE);

	// [LGLS 2026-10-01] 오른쪽 상세는 선택이 살아 있으면 새 값으로, 아니면 비운다
	FillDetail(nFound);
}

// [LGLS 2026-10-01] 선택한 작업의 상세(오른쪽) + 우선순위(왼쪽)
void CPanelJobDlg::FillDetail(int nRow)
{
	if (!::IsWindow(m_listSeq.m_hWnd)) return;
	if (nRow < 0 || nRow >= m_arRow.GetCount())
	{
		m_strSelLugg.Empty();
		m_lblEcsVal.SetWindowText(_T(""));
		m_lblJobVal.SetWindowText(_T(""));
		m_lblPriVal.SetWindowText(_T(""));
		m_listSeq.DeleteAllItems();
		return;
	}
	const ROW& r = m_arRow[nRow];
	m_strSelLugg = r.lugg;
	m_lblEcsVal.SetWindowText(r.lugg);
	m_lblJobVal.SetWindowText(r.lot);
	m_lblPriVal.SetWindowText(r.pri.IsEmpty() ? _T("-") : r.pri);
	BuildSeqRows(r);
}

// [LGLS 2026-10-01] 작업구분 + 상태 코드로 구 ECS 의 명령 단계표(SEQ/디바이스/시작/도착)를 만든다.
//   입고(1)      : C/V(출발 작업대 → RGV 픽업) → RGV(→ 통로) → S/C(통로 → 랙)
//                  10,11,15,16 = 1단계 / 30,31,35,39 = 2단계 / 20,21,25 = 3단계 / 29,09,05~08 = 완료
//   출고(2,3)    : S/C(랙 → 통로) → RGV(통로 → 출고 픽업) → C/V(→ 도착 작업대)
//                  20,21,25 = 1단계 / 29,30,31,35 = 2단계 / 39,10,11,15,16 = 3단계 / 19,09 = 완료
//   랙투랙(4)    : S/C 한 단계.   호기간(5) : S/C → RGV → S/C.   이동(0,6) : C/V 한 단계.
//   반자동(1x)은 같은 흐름이다.
// [LGLS 2026-10-01] 구 ECS 와 같은 단계표 (사용자 지시) : 디바이스/시작/도착을 전부 ★트랙 번호★ 기준으로
//   CONVEYOR:11  PORT:122 → PORT:121   (입고대 → RGV 픽업 트랙)
//   RGV          PORT:121 → PORT:103   (→ 통로)
//   CONVEYOR:2   PORT:103 → PORT:104   (통로 두 칸)
//   S/C 1        PORT:104 → LOC:04-001-01
//   트랙→C/V 번호는 CV_DATA(PLC_NO, TRACK_NO 1xxx)에서 읽어 둔다. RGV 픽업 트랙 = 같은 C/V 의 옆 트랙(아래쪽 우선, 없으면 위쪽).
int CPanelJobDlg::CvOfTrack(int nTrk)
{
	int cv = 0;
	if (m_mapTrackCv.Lookup(nTrk, cv)) return cv;
	return 0;
}
CString CPanelJobDlg::PortText(int nTrk)
{
	// [LGLS 2026-10-01] 구 ECS 처럼 2자리 트랙 번호로 (122 → 22) (사용자 지시)
	CString t; if (nTrk > 0) t.Format(_T("PORT:%d"), (nTrk >= 100 && nTrk < 1000) ? nTrk - 100 : nTrk); return t;
}
CString CPanelJobDlg::CvDevText(int nTrk)
{
	CString t; int cv = CvOfTrack(nTrk);
	if (cv > 0) t.Format(_T("CONVEYOR:%d"), cv); else t = _T("CONVEYOR");
	return t;
}
int CPanelJobDlg::NeighborTrack(int nTrk)
{
	int cv = CvOfTrack(nTrk); if (cv <= 0) return 0;
	if (CvOfTrack(nTrk - 1) == cv) return nTrk - 1;
	if (CvOfTrack(nTrk + 1) == cv) return nTrk + 1;
	return 0;
}
// C/V 번호의 트랙 범위 (가장 작은/큰 트랙). 통로 C/V 는 트랙이 둘(103/104)이다.
BOOL CPanelJobDlg::CvTracks(int nCv, int& nLo, int& nHi)
{
	nLo = 0; nHi = 0;
	POSITION pos = m_mapTrackCv.GetStartPosition();
	while (pos != NULL)
	{
		int trk, cv; m_mapTrackCv.GetNextAssoc(pos, trk, cv);
		if (cv != nCv) continue;
		if (nLo == 0 || trk < nLo) nLo = trk;
		if (trk > nHi) nHi = trk;
	}
	return nLo > 0;
}
// 크레인 n 의 통로 C/V : S/C#1 은 C/V#2 (양방향, 방향전환), 그 밖은 입고 C/V#(2n) / 출고 C/V#(2n+1)
int CPanelJobDlg::HsCvOfCrane(int nSc, BOOL bInbound)
{
	if (nSc <= 0) return 0;
	if (nSc == 1) return 2;
	return bInbound ? 2 * nSc : 2 * nSc + 1;
}

void CPanelJobDlg::LoadTrackCvMap()
{
	if (m_pDoc == NULL || m_mapTrackCv.GetCount() > 0) return;
	CString strSql; strSql.Format(_T(" SELECT PLC_NO, TRACK_NO FROM CV_DATA WHERE WH_TYP = '%s' "), (LPCTSTR)m_pDoc->m_WH_TYP);
	int nRowCnt = -1; CString strMessage;
	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt <= 0) return;
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
	pRsw->MoveFirst();
	for (int i = 0; i < nRowCnt; i++)
	{
		int cv = _ttoi(pRsw->GetItem(_T("PLC_NO")));
		int trk = _ttoi(pRsw->GetItem(_T("TRACK_NO")));			// 1022 → 122 (JOB_MST 의 3자리 트랙)
		if (trk >= 1000) trk = trk % 1000 + 100;
		if (trk > 0 && cv > 0) m_mapTrackCv.SetAt(trk, cv);
		pRsw->MoveNext();
	}
	delete pRsw;
}

void CPanelJobDlg::BuildSeqRows(const ROW& r)
{
	m_listSeq.DeleteAllItems();
	LoadTrackCvMap();
	int typ = _ttoi(r.typCd); if (typ >= 10) typ -= 10;
	int s = _ttoi(r.staCd);

	int nStart = _ttoi(r.startPos), nDest = _ttoi(r.destPos), nHs = _ttoi(r.hs);
	// 랙 쪽은 LOC:bb-bbb-ll
	CString strStartLoc = (nStart >= 900 || nStart < 100) ? (_T("LOC:") + r.startLoc) : PortText(nStart);
	CString strDestLoc  = (nDest  >= 900 || nDest  < 100) ? (_T("LOC:") + r.destLoc)  : PortText(nDest);
	CString strSc;
	int nSc = 0;
	{
		if (!r.sc.IsEmpty()) nSc = _ttoi(r.sc) % 100;			// 901 → 1
		if (nSc <= 0)
		{
			CString loc = (typ == 1) ? r.destLoc : r.startLoc;	// 랙 쪽 위치 bb-bbb-ll
			int nBank = _ttoi(loc.Left(2));
			if (nBank > 0) nSc = (nBank + 1) / 2;
		}
		if (nSc <= 0 && nDest >= 900) nSc = nDest % 100;		// 도착 902 → S/C 2
		if (nSc <= 0 && nStart >= 900) nSc = nStart % 100;
		if (nSc > 0) strSc.Format(_T("S/C %d"), nSc); else strSc = _T("S/C");
	}
	// [LGLS 2026-10-01] 통로(HS_TRACK_NO)가 아직 배정되지 않은 작업도 4줄을 다 보인다 (사용자 지시 - 앞으로 생기는 모든 작업).
	//   크레인 번호로 통로 C/V 를 정하고 그 두 트랙을 쓴다. 입고 = 바깥 칸(작은 번호) → 안쪽 칸, 출고 = 안쪽 칸 → 바깥 칸.
	//   실제 배정(HS_TRACK_NO)이 생기면 그 값이 우선한다.
	int nHs2 = 0;
	if (nHs > 0) nHs2 = NeighborTrack(nHs);
	else
	{
		int lo = 0, hi = 0;
		BOOL bIn = (typ == 1 || typ == 5);
		if (CvTracks(HsCvOfCrane(nSc, bIn), lo, hi))
		{
			if (bIn) { nHs = lo; nHs2 = hi; } else { nHs = hi; nHs2 = lo; }
		}
	}
	m_nSeqHs = nHs;			// 완료처리 때 HS_TRACK_NO 가 비어 있으면 이 값으로 채운다 (IO_TASK 는 HS 없는 29/39 를 못 넘긴다)
	CString strHs  = (nHs > 0) ? PortText(nHs) : _T("통로");
	CString strHs2 = (nHs2 > 0) ? PortText(nHs2) : strHs;

	struct STEP { CString dev, from, to; int kind; int toTrk; };		// kind : 0 작업대 C/V / 1 RGV / 2 통로 C/V / 3 S/C
	CArray<STEP, STEP&> steps;
	m_arSeqKind.RemoveAll();
	m_arSeqToTrk.RemoveAll();
	int nPhase = 0;			// 진행 중인 단계 (steps.GetCount() 이면 모두 완료)
	STEP st;
	st.toTrk = 0;
	switch (typ)
	{
	case 1:		// 입고 : C/V(입고대→픽업) → RGV(픽업→통로) → C/V(통로 두 칸) → S/C(통로→랙)
	{
		int nPick = NeighborTrack(nStart);
		st.kind = 0; st.dev = CvDevText(nStart); st.from = PortText(nStart); st.to = PortText(nPick > 0 ? nPick : nStart); steps.Add(st);
		st.kind = 1; st.dev = _T("RGV");         st.from = st.to;             st.to = strHs;      st.toTrk = nHs; steps.Add(st); st.toTrk = 0;
		if (nHs2 > 0) { st.kind = 2; st.dev = CvDevText(nHs); st.from = strHs; st.to = strHs2; steps.Add(st); }
		st.kind = 3; st.dev = strSc;             st.from = strHs2;           st.to = strDestLoc; steps.Add(st);
		int nCvHs = (nHs2 > 0) ? 1 : 0;
		if      (s == 99 || s == 10 || s == 11 || s == 15 || s == 16) nPhase = 0;
		else if (s == 30 || s == 31 || s == 35 || s == 39)            nPhase = 1;
		else if (s == 20 || s == 21)                                  nPhase = 1 + nCvHs;
		else if (s == 25)                                             nPhase = 2 + nCvHs;
		else                                                          nPhase = (int)steps.GetCount();
		break;
	}
	case 2: case 3:		// 출고 / 피킹출고 : S/C(랙→통로) → C/V(통로 두 칸) → RGV(통로→출고 픽업) → C/V(픽업→출고대)
	{
		int nPick = NeighborTrack(nDest);
		st.kind = 3; st.dev = strSc;             st.from = strStartLoc;      st.to = strHs;      steps.Add(st);
		if (nHs2 > 0) { st.kind = 2; st.dev = CvDevText(nHs); st.from = strHs; st.to = strHs2; steps.Add(st); }
		st.kind = 1; st.dev = _T("RGV");         st.from = strHs2;           st.to = PortText(nPick > 0 ? nPick : nDest); st.toTrk = (nPick > 0 ? nPick : nDest); steps.Add(st); st.toTrk = 0;
		st.kind = 0; st.dev = CvDevText(nDest);  st.from = st.to;             st.to = PortText(nDest); steps.Add(st);
		int nCvHs = (nHs2 > 0) ? 1 : 0;
		if      (s == 99 || s == 20 || s == 21 || s == 25)            nPhase = 0;
		else if (s == 29)                                             nPhase = 1;
		else if (s == 30 || s == 31 || s == 35)                       nPhase = 1 + nCvHs;
		else if (s == 39 || s == 10 || s == 11 || s == 15 || s == 16) nPhase = 2 + nCvHs;
		else                                                          nPhase = (int)steps.GetCount();
		break;
	}
	case 4:		// 랙투랙
		st.kind = 3; st.dev = strSc;      st.from = strStartLoc; st.to = strDestLoc;  steps.Add(st);
		nPhase = (s == 99 || s == 20 || s == 21 || s == 25) ? 0 : 1;
		break;
	case 5:		// 호기간 이동
		st.kind = 3; st.dev = strSc;      st.from = strStartLoc; st.to = strHs;       steps.Add(st);
		st.kind = 1; st.dev = _T("RGV");  st.from = strHs;       st.to = _T("통로");  steps.Add(st);
		st.kind = 3; st.dev = _T("S/C");  st.from = _T("통로");  st.to = strDestLoc;  steps.Add(st);
		if      (s == 99 || s == 20 || s == 21 || s == 25)            nPhase = 0;
		else if (s == 29 || s == 30 || s == 31 || s == 35)            nPhase = 1;
		else if (s == 39 || s == 10 || s == 11 || s == 15 || s == 16) nPhase = 2;
		else                                                          nPhase = 3;
		break;
	default:	// 이동 등
		st.kind = 0; st.dev = CvDevText(nStart); st.from = PortText(nStart); st.to = PortText(nDest); steps.Add(st);
		nPhase = (s == 99 || s == 10 || s == 11 || s == 15 || s == 16) ? 0 : 1;
		break;
	}

	for (int i = 0; i < steps.GetCount(); i++)
	{
		LPCTSTR pszSta = (i < nPhase) ? _T("완료") : (i == nPhase) ? _T("진행중") : _T("대기");
		m_listSeq.InsertItem(i, pszSta);
		CString strSeq; strSeq.Format(_T("%04d"), i + 1);
		m_listSeq.SetItemText(i, 1, strSeq);
		m_listSeq.SetItemText(i, 2, steps[i].dev);
		m_listSeq.SetItemText(i, 3, steps[i].from);
		m_listSeq.SetItemText(i, 4, steps[i].to);
		m_arSeqKind.Add(steps[i].kind);
		m_arSeqToTrk.Add(steps[i].toTrk);
	}
	m_nSeqPhase = nPhase;
	if (nPhase < steps.GetCount())
		m_listSeq.SetItemState(nPhase, LVIS_SELECTED, LVIS_SELECTED);
}

// [LGLS 2026-10-01] 작업번호 값은 구 ECS 처럼 붉게
HBRUSH CPanelJobDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd != NULL && pWnd->GetDlgCtrlID() == IDC_PANEL_JOB_ECS_VAL)
		pDC->SetTextColor(RGB(192, 0, 0));
	return hbr;
}

void CPanelJobDlg::OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* p = (NMLISTVIEW*)pNMHDR;
	*pResult = 0;
	if (p == NULL) return;
	if ((p->uChanged & LVIF_STATE) && (p->uNewState & LVIS_SELECTED))
		FillDetail(p->iItem);
}

BOOL CPanelJobDlg::ExecUpdate(CString strSql, CString strLogMsg, CString strLuggNo)
{
	if (m_pDoc == NULL) return FALSE;
	if (!m_pDoc->Permission(_T("CViewJobListDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return FALSE;
	}
	if (m_pDoc->BeginTrans_DLG() < 1) return FALSE;
	if (strLuggNo.IsEmpty()) strLuggNo = _T("0");
	if (!m_pDoc->GetQueryInsertClientLog(_T("CViewJobListDlg"), strLuggNo, _T(""), _T(""), strLogMsg))
	{
		m_pDoc->RollbackTrans_DLG();
		return FALSE;
	}
	if (!m_pDoc->ExcuteQueryString_DLG(strSql))
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return FALSE;
	}
	m_pDoc->CommitTrans_DLG();
	return TRUE;
}

// [LGLS 2026-10-01] ▲ / ▼ : 선택 작업의 JOB_PRIORITY 를 1 씩 (001~999, 판넬의 우선순위 변경과 같은 갱신)
static void LglsPriStep(CPanelJobDlg* pDlg, CEcsDoc* pDoc, const CString& strLugg, const CString& strPri, int nDelta, BOOL (CPanelJobDlg::*pfnExec)(CString, CString, CString))
{
	if (pDoc == NULL || strLugg.IsEmpty()) { AfxMessageBox(_T("작업을 먼저 고르세요.")); return; }
	int n = _ttoi(strPri) + nDelta;
	if (n < 1) n = 1; if (n > 999) n = 999;
	CString strNew; strNew.Format(_T("%03d"), n);
	if (strNew == strPri) return;
	if (AfxMessageBox(pDoc->GetMsgLangDef(_T("우선순위를 변경 하시겠습니까?")) + _T(" [") + strLugg + _T(" : ") + strPri + _T(" -> ") + strNew + _T("]"), MB_YESNO) != IDYES)
		return;
	CString strSql;
	strSql.Format(_T("UPDATE JOB_MST SET JOB_PRIORITY = '%s', UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
		(LPCTSTR)strNew, (LPCTSTR)pDoc->m_WH_TYP, (LPCTSTR)strLugg);
	if ((pDlg->*pfnExec)(strSql, _T("JOB_MST UPDATE : JOB_PRIORITY -> ") + strNew, strLugg))
		pDlg->Refresh();
}

void CPanelJobDlg::OnPriUp()
{
	int i = FindRow(m_strSelLugg);
	LglsPriStep(this, m_pDoc, m_strSelLugg, (i >= 0) ? m_arRow[i].pri : _T("1"), +1, &CPanelJobDlg::ExecUpdate);
}

void CPanelJobDlg::OnPriDown()
{
	int i = FindRow(m_strSelLugg);
	LglsPriStep(this, m_pDoc, m_strSelLugg, (i >= 0) ? m_arRow[i].pri : _T("1"), -1, &CPanelJobDlg::ExecUpdate);
}

// [LGLS 2026-10-01] [반송조정] : 구 ECS 의 반송명령조정 창 = 신 ECS 의 작업정보 창(상태/우선순위 변경, 삭제)
void CPanelJobDlg::OnTransferCtl()
{
	CWnd* pMain = AfxGetMainWnd();
	if (pMain != NULL && ::IsWindow(pMain->GetSafeHwnd()))
		pMain->PostMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_JOBLIST, 0), 0);
}

// [LGLS 2026-10-01] [완료처리] : 구 ECS 는 선택한 명령 단계(TB_TRANSFERDETAIL)를 완료로 돌렸다.
//   신 ECS 는 단계가 JOB_STATUS 하나이므로 작업을 설비 완료 상태로 올린다 - 출고류는 19(C/V 반송완료), 그 밖은 29(S/C 반송완료).
//   판넬의 강제완료와 같은 갱신이며 이후는 IO_TASK/HOST_TASK 가 평소대로 처리한다.
// [LGLS 2026-10-01] 완료처리 = ★스텝(SEQ) 단위★ (구 ECS MonitorMainTransferListPanel.buttonComplete_Click / chageTransferDetailComplete 와 같게,
//   담당자 확인). 고른 SEQ 줄의 설비 구간을 "끝난 것"으로 보고 JOB_STATUS 를 그 구간 완료 코드로 옮긴다 → IO_TASK 가 다음 구간을 낸다.
//   신 ECS 는 스텝 테이블이 없고 JOB_MST.JOB_STATUS 하나로 단계를 나타내므로 IO_TASK(cThread_SCH) 의 선택 조건에 맞춘 코드를 쓴다 :
//     입고 : 작업대 C/V 15 (DriveRGV 는 15/16 을 가져감) / RGV 39 (CompleteRGV) / 통로 C/V 16 (DriveSC 는 15/16) / S/C 29 (최종)
//     출고 : S/C 29 (CompleteSC) / 통로 C/V 15 (DriveRGV) / RGV 39 / 작업대 C/V 19 (최종)
//   줄을 안 골랐으면 진행중 줄.
CString CPanelJobDlg::StepDoneStatus(int typ, int kind)
{
	BOOL bOut = (typ == 2 || typ == 3);
	switch (kind)
	{
	case 0: return bOut ? _T("19") : _T("15");
	case 1: return _T("39");
	case 2: return bOut ? _T("15") : _T("16");
	case 3: return _T("29");
	}
	return _T("");
}

void CPanelJobDlg::OnComplete()
{
	int i = FindRow(m_strSelLugg);
	if (m_pDoc == NULL || i < 0) { AfxMessageBox(_T("작업을 먼저 고르세요.")); return; }
	const ROW& r = m_arRow[i];
	int typ = _ttoi(r.typCd); if (typ >= 10) typ -= 10;

	int nSeq = m_listSeq.GetNextItem(-1, LVNI_SELECTED);
	if (nSeq < 0) nSeq = m_nSeqPhase;
	if (nSeq < 0 || nSeq >= m_arSeqKind.GetCount()) { AfxMessageBox(_T("완료 처리할 단계(SEQ)를 고르세요.")); return; }
	CString strSta = StepDoneStatus(typ, m_arSeqKind[nSeq]);
	if (strSta.IsEmpty()) { AfxMessageBox(_T("이 단계는 완료 처리할 수 없습니다.")); return; }
	CString strSeq = m_listSeq.GetItemText(nSeq, 1), strDev = m_listSeq.GetItemText(nSeq, 2);
	if (strSta == r.staCd) { AfxMessageBox(_T("이미 그 단계가 끝난 상태입니다. [") + r.staCd + _T("]")); return; }

	CString strMsg;
	strMsg.Format(_T("작업번호(%s), 순번(%s) %s 구간을 완료 처리하시겠습니까?  [%s -> %s]"),
		(LPCTSTR)r.lugg, (LPCTSTR)strSeq, (LPCTSTR)strDev, (LPCTSTR)r.staCd, (LPCTSTR)strSta);
	if (AfxMessageBox(strMsg, MB_YESNO) != IDYES) return;
	int nKind = m_arSeqKind[nSeq];
	BOOL bOut = (typ == 2 || typ == 3);

	// [LGLS 2026-10-05] S/C·RGV 구간은 구 ECS 설비 창 [완료처리](buttonForceComplete_Click) 가 하던 일까지 함께 한다 (사용자 지시 -
	//   설비 창 [강제완료] 를 쓰지 않고 판넬 [완료처리] 하나로 처리).
	//   ① 그 작업을 물고 있는 설비의 지시(_OD)·잔류 관측값·완료신호를 비운다 → 설비가 다음 명령을 받는다.
	//      비우지 않으면 IO_TASK 의 "아직 차상에 있다" 판정에 걸려 H/S 번호 찍기도 되지 않는다.
	//   ② 번호 찍기는 IO_TASK 가 한다 : 출고 S/C 29 → LandScDrop 이 H/S 의 화물에 작업번호를 찍고 16,
	//      RGV 39 → LandRgvDrop 이 HS_TRACK_NO(도착 트랙)의 화물에 작업번호를 찍고 15/16.
	//      그래서 RGV 구간은 HS_TRACK_NO 를 RGV 도착 트랙으로 맞춰 준다(ForceCompleteRtv 와 같다).
	CString strVehWhere, strVehTbl;
	if (nKind == 3)
	{
		strVehTbl = _T("SC_DATA_LGLS");
		strVehWhere.Format(_T(" WHERE WH_TYP = '%s' AND (LUGG_NO_FK1_OD = '%s' OR ITN_LUGG_FK1 = '%s' OR PALLET_ON_VEHICLE_RD = '%s' OR PALLET_ID_OD = '%s')"),
			(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)r.lugg, (LPCTSTR)r.lugg, (LPCTSTR)r.lugg, (LPCTSTR)r.lugg);
	}
	else if (nKind == 1)
	{
		strVehTbl = _T("RTV_DATA_LGLS");
		strVehWhere.Format(_T(" WHERE WH_TYP = '%s' AND (LUGG_OD = '%s' OR PALLET_ON_VEHICLE_RD = '%s' OR PALLET_ID_OD = '%s')"),
			(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)r.lugg, (LPCTSTR)r.lugg, (LPCTSTR)r.lugg);
	}
	if (!strVehTbl.IsEmpty())
	{
		// 그 설비가 아직 RUN(작업중) 이면 한 번 더 묻는다 (구 ECS 설비 창은 RUN 에서 거부했다. 판넬은 Ack 대기로 RUN 에 머문 설비도
		// 넘겨야 하므로 막지는 않는다)
		CString strChk = _T("SELECT COUNT(*) AS CNT FROM ") + strVehTbl + strVehWhere + _T(" AND SUBSYSTEM_STATUS_RD = '2'");
		CString strChkMsg; int nChk = -1;
		_RecordsetPtr ptrChk = m_pDoc->GetSelectQryRecordsetPtr_DLG(strChk, nChk, strChkMsg);
		if (nChk > 0)
		{
			CRecordSetWrap* pChk = new CRecordSetWrap(ptrChk);
			pChk->MoveFirst();
			int nRun = _ttoi(pChk->GetItem(_T("CNT")));
			delete pChk;
			if (nRun > 0 && AfxMessageBox(_T("이 작업을 맡은 설비가 RUN(작업중) 상태입니다.\n화물을 이미 내려놓은 것이 확실합니까?\n(설비에 남은 지시 정보를 비웁니다)"),
				MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) != IDYES) return;
		}
	}

	CString strSql;
	// HS_TRACK_NO : RGV 구간은 RGV 도착 트랙으로 맞춘다. 그 밖에는 비어 있을 때만 단계표가 예측한 통로 트랙을 넣는다
	//   - IO_TASK 가 다음 구간(RGV/크레인)을 내거나 착지 화물에 번호를 찍을 때 이 값으로 트랙을 찾는다
	CString strHsSet;
	if (nKind == 1 && nSeq < m_arSeqToTrk.GetCount() && m_arSeqToTrk[nSeq] > 0)
		strHsSet.Format(_T(", HS_TRACK_NO = '%d'"), m_arSeqToTrk[nSeq]);
	else if (r.hs.IsEmpty() && m_nSeqHs > 0)
		strHsSet.Format(_T(", HS_TRACK_NO = '%d'"), m_nSeqHs);
	strSql.Format(_T("UPDATE JOB_MST SET JOB_STATUS = '%s'%s, UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
		(LPCTSTR)strSta, (LPCTSTR)strHsSet, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)r.lugg);
	CString strVehLog;
	if (nKind == 3)
	{
		strSql += _T("; UPDATE SC_DATA_LGLS SET LUGG_NO_FK1_OD = '0000', PALLET_ID_OD = '0000', JOB_TYP_OD = '0', JOB_TYP_RD = '0'")
			_T(", ITN_LUGG_FK1 = '0', PALLET_ON_VEHICLE_RD = '', COMPLETE_RD = '0'")
			_T(", FROM_01_OD = '00', FROM_02_OD = '00', FROM_03_OD = '00', TO_01_OD = '00', TO_02_OD = '00', TO_03_OD = '00'")
			_T(", OD_RQ_YN = 'N', TRANSFER_REQUEST_OD = 'N'") + strVehWhere;
		strVehLog = _T(" + S/C 지시 정리");
	}
	else if (nKind == 1)
	{
		strSql += _T("; UPDATE RTV_DATA_LGLS SET LUGG_OD = '0000', PALLET_ID_OD = '0000', JOB_TYP_OD = '0', COMPLETE_RD = '0'")
			_T(", FROM_01_OD = '00', FROM_02_OD = '00', FROM_03_OD = '00', TO_01_OD = '00', TO_02_OD = '00', TO_03_OD = '00'")
			_T(", RTV_DEST_OD = '', RTV_PASSCV_OD = '', OD_RQ_YN = 'N', TRANSFER_REQUEST_OD = 'N', DEPART_TRACK = '', ARRIVE_TRACK = ''") + strVehWhere;
		strVehLog = _T(" + RTV 지시 정리");
	}
	UNREFERENCED_PARAMETER(bOut);
	CString strLog; strLog.Format(_T("JOB_MST UPDATE : 완료처리(스텝 %s %s) JOB_STATUS %s -> %s%s%s"), (LPCTSTR)strSeq, (LPCTSTR)strDev, (LPCTSTR)r.staCd, (LPCTSTR)strSta, (LPCTSTR)strHsSet, (LPCTSTR)strVehLog);
	if (ExecUpdate(strSql, strLog, r.lugg))
		Refresh();
}

void CPanelJobDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	UNREFERENCED_PARAMETER(nType);
	if (!::IsWindow(m_list.m_hWnd)) return;

	// [LGLS 2026-10-01] 구 ECS 판넬과 같은 세 칸 : [우선순위 95] [목록] [상세]
	//   판넬이 좁으면(왼쪽 도킹 기본 폭 ~310) 세 칸이 안 들어가므로 상세 칸을 아래로 내린다
	//   탭 줄과 [자동 갱신] 은 없앴고(숨김), 목록|상세 사이는 끌어서 폭을 바꿀 수 있다(m_nSplitR).
	//   [완료처리] 는 상세 칸 맨 위 줄 오른쪽(사용자 지시).
	const int nListY = 2;
	const int nChkY = CLib::DpiPx(4),  nChkH  = CLib::DpiPx(18);
	const int nGap  = CLib::DpiPx(6);				// 분할선 폭
	const int L  = (m_nRcLeftW > 0) ? m_nRcLeftW + 2 : CLib::DpiPx(96);	// 왼쪽 칸 폭 = 리소스의 왼쪽 컨트롤 오른쪽 끝
	// [LGLS 2026-10-01] 분할 방향을 Ecs.ini 로 정한다 (사용자 지시)
	//   [DISPLAY] JOB_PANEL_SPLIT    = 0 자동(폭 620px 미만이면 세로) / 1 가로(상세를 오른쪽) / 2 세로(상세를 아래)
	//   [DISPLAY] JOB_PANEL_DETAIL_W = 가로일 때 상세 칸 폭(px, 0=38%%)   JOB_PANEL_DETAIL_H = 세로일 때 상세 칸 높이(px, 0=178)
	//   분할선을 끌면 그 값이 우선하고(m_nSplitR / m_nSplitB), 다시 띄우면 ini 값으로 돌아온다.
	//   [LGLS 2026-10-01] 왼쪽 칸 [가로보기]/[세로보기] 단추로도 바꾼다(사용자 지시). 바꾸면 ini 에도 써서 다음에도 유지.
	if (m_nSplitMode == 0) m_nSplitMode = ::GetPrivateProfileInt(_T("DISPLAY"), _T("JOB_PANEL_SPLIT"), 0, ECS_INI_FILE);
	const int nSplitMode = m_nSplitMode;
	const int nIniW = ::GetPrivateProfileInt(_T("DISPLAY"), _T("JOB_PANEL_DETAIL_W"), 0, ECS_INI_FILE);
	const int nIniH = ::GetPrivateProfileInt(_T("DISPLAY"), _T("JOB_PANEL_DETAIL_H"), 0, ECS_INI_FILE);
	const BOOL bNarrow = (nSplitMode == 2) ? TRUE : (nSplitMode == 1) ? FALSE : (cx < CLib::DpiPx(620));
	int R = (m_nSplitR > 0) ? m_nSplitR : (nIniW > 0) ? CLib::DpiPx(nIniW) : cx * 38 / 100;		// 오른쪽 칸 폭
	if (R < CLib::DpiPx(200)) R = CLib::DpiPx(200);
	if (R > cx - L - CLib::DpiPx(160)) R = max(0, cx - L - CLib::DpiPx(160));
	if (bNarrow) R = 0;
	int xM = L + nGap, wM = cx - L - R - 2 * nGap; if (wM < 100) wM = 100;
	int xR = cx - R;
	const int hD = (m_nSplitB > 0) ? m_nSplitB : CLib::DpiPx((nIniH > 0) ? nIniH : 178);		// 아래 칸 높이
	int cyTop = bNarrow ? max(CLib::DpiPx(80), cy - hD - nGap) : cy;

	// 왼쪽 : 리소스(IDD_PANEL_JOB) 좌표 그대로 (옮기지 않는다)

	// 가운데 (탭 없이 목록만)
	m_list.MoveWindow(xM, nListY, wM, cyTop - nListY);

	// 분할선 (넓을 때만)
	m_bSplitVert = bNarrow;
	if (::IsWindow(m_btnSplit.m_hWnd)) m_btnSplit.SetWindowText(bNarrow ? _T("가로보기") : _T("세로보기"));
	m_rcSplit = bNarrow ? CRect(xM, cyTop, cx, cyTop + nGap) : CRect(xM + wM, 0, xR, cy);

	// 오른쪽(넓을 때) / 아래(좁을 때)
	int yR = bNarrow ? cyTop + nGap : 0;
	if (bNarrow) { xR = 0; R = cx; }
	// 머리 줄 : 리소스의 상대 위치(ECS번호 라벨 왼쪽-4 를 원점) 를 유지하고 칸의 왼쪽 끝(xR)만 옮긴다.
	//   좁을 때(세로 쌓기)는 ECS번호/작업번호를 두 줄로.
	const int wBtn = m_rcRcRight[4].IsRectEmpty() ? CLib::DpiPx(88) : m_rcRcRight[4].Width();
	const int hBtn = m_rcRcRight[4].IsRectEmpty() ? CLib::DpiPx(22) : m_rcRcRight[4].Height();
	int yHead = yR + (m_rcRcRight[0].IsRectEmpty() ? nChkY : m_rcRcRight[0].top);
	int yList = yR + (m_rcRcRight[5].IsRectEmpty() ? CLib::DpiPx(28) : m_rcRcRight[5].top);
	CWnd* pR[] = { &m_lblEcs, &m_lblEcsVal, &m_lblJob, &m_lblJobVal };
	// [LGLS 2026-10-01] 머리줄은 늘 한 줄 : 작업번호 [값]  팔렛 [값]            [완료처리]   (사용자 지시)
	//   넓으면 리소스의 상대 위치 그대로, 좁으면 값 칸을 줄여 한 줄에 맞춘다.
	if (bNarrow || R < CLib::DpiPx(420))
	{
		int wLbl = CLib::DpiPx(48);
		int wVal = (R - (bNarrow ? wBtn + 8 : 0) - 2 * (wLbl + 4) - 8) / 2;	// 가로+좁음이면 버튼이 아래로 가니 그 자리까지 값 칸으로
		if (wVal < CLib::DpiPx(40)) wVal = CLib::DpiPx(40);
		int x = xR + 4;
		if (::IsWindow(m_lblEcs.m_hWnd))    m_lblEcs.MoveWindow(x, yHead, wLbl, nChkH);            x += wLbl + 4;
		if (::IsWindow(m_lblEcsVal.m_hWnd)) m_lblEcsVal.MoveWindow(x, yHead, wVal, nChkH);         x += wVal + 8;
		if (::IsWindow(m_lblJob.m_hWnd))    m_lblJob.MoveWindow(x, yHead, wLbl, nChkH);            x += wLbl + 4;
		if (::IsWindow(m_lblJobVal.m_hWnd)) m_lblJobVal.MoveWindow(x, yHead, wVal, nChkH);
		yList = yHead + nChkH + 4;
	}
	else
	{
		for (int i = 0; i < 4; i++)
			if (::IsWindow(pR[i]->m_hWnd) && !m_rcRcRight[i].IsRectEmpty())
				pR[i]->MoveWindow(xR + m_rcRcRight[i].left, yR + m_rcRcRight[i].top, m_rcRcRight[i].Width(), m_rcRcRight[i].Height());
	}
	// [LGLS 2026-10-01] 가로 분할인데 상세 칸이 좁으면(머리줄에 자리가 없으면) [완료처리] 를 상세 칸 아래로 내린다 (사용자 지시)
	BOOL bBtnBottom = (!bNarrow && R < CLib::DpiPx(420));
	int cyList = cy - yList - 2;
	if (bBtnBottom)
	{
		if (::IsWindow(m_btnComplete.m_hWnd)) m_btnComplete.MoveWindow(xR + R - wBtn - 2, cy - hBtn - 2, wBtn, hBtn);
		cyList -= (hBtn + 4);
	}
	else if (::IsWindow(m_btnComplete.m_hWnd)) m_btnComplete.MoveWindow(xR + R - wBtn - 2, yR + 2, wBtn, hBtn);
	if (::IsWindow(m_listSeq.m_hWnd))
	{
		m_listSeq.MoveWindow(xR, yList, R, max(CLib::DpiPx(40), cyList));
		static const int W_WIDE[]   = { 52, 45, 110, 130, 130 };
		static const int W_NARROW[] = { 48, 40, 100, 110, 110 };
		for (int i = 0; i < 5; i++)
			m_listSeq.SetColumnWidth(i, CLib::DpiPx(bNarrow ? W_NARROW[i] : W_WIDE[i]));
	}
}

// [LGLS 2026-10-01] 목록|상세 분할선 끌기
void CPanelJobDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if (!m_rcSplit.IsRectEmpty() && m_rcSplit.PtInRect(pt))
	{
		m_bDragSplit = TRUE; SetCapture(); return;
	}
	CDialog::OnLButtonDown(nFlags, pt);
}

void CPanelJobDlg::OnMouseMove(UINT nFlags, CPoint pt)
{
	if (m_bDragSplit)
	{
		CRect rc; GetClientRect(&rc);
		if (m_bSplitVert)
		{
			// 세로 쌓기 : 아래 칸 높이
			int B = rc.Height() - pt.y - CLib::DpiPx(3);
			if (B < CLib::DpiPx(80)) B = CLib::DpiPx(80);
			if (B > rc.Height() - CLib::DpiPx(80)) B = max(CLib::DpiPx(80), rc.Height() - CLib::DpiPx(80));
			if (B != m_nSplitB)
			{
				m_nSplitB = B;
				OnSize(SIZE_RESTORED, rc.Width(), rc.Height());
				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ALLCHILDREN);
			}
			return;
		}
		int R = rc.Width() - pt.x - CLib::DpiPx(3);
		if (R < CLib::DpiPx(200)) R = CLib::DpiPx(200);
		int nMax = rc.Width() - CLib::DpiPx(96) - CLib::DpiPx(160);
		if (R > nMax) R = max(CLib::DpiPx(200), nMax);
		if (R != m_nSplitR)
		{
			m_nSplitR = R;
			OnSize(SIZE_RESTORED, rc.Width(), rc.Height());
			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ALLCHILDREN);
		}
		return;
	}
	CDialog::OnMouseMove(nFlags, pt);
}

void CPanelJobDlg::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if (m_bDragSplit) { m_bDragSplit = FALSE; ReleaseCapture(); return; }
	CDialog::OnLButtonUp(nFlags, pt);
}

BOOL CPanelJobDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd == this && !m_rcSplit.IsRectEmpty())
	{
		CPoint pt; GetCursorPos(&pt); ScreenToClient(&pt);
		if (m_bDragSplit || m_rcSplit.PtInRect(pt))
		{
			::SetCursor(::LoadCursor(NULL, m_bSplitVert ? IDC_SIZENS : IDC_SIZEWE));
			return TRUE;
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

// [LGLS 2026-10-01] 가로보기/세로보기 전환 (왼쪽 칸 단추). ini [DISPLAY] JOB_PANEL_SPLIT 에도 써 둔다.
void CPanelJobDlg::OnSplitToggle()
{
	m_nSplitMode = m_bSplitVert ? 1 : 2;
	m_nSplitR = 0; m_nSplitB = 0;
	CString v; v.Format(_T("%d"), m_nSplitMode);
	::WritePrivateProfileString(_T("DISPLAY"), _T("JOB_PANEL_SPLIT"), v, ECS_INI_FILE);
	CRect rc; GetClientRect(&rc);
	OnSize(SIZE_RESTORED, rc.Width(), rc.Height());
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

void CPanelJobDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_PANEL_JOB)
	{
		// [LGLS 2026-09-10] 자동 갱신이 꺼져 있으면 건너뛴다
		if (::IsWindow(m_chkAuto.m_hWnd) && m_chkAuto.GetCheck() != BST_CHECKED)
			return;
		Refresh();
		return;
	}
	CDialog::OnTimer(nIDEvent);
}

void CPanelJobDlg::OnTabChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 0;
	Refresh();
}
