// ViewJobListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "ViewJobListDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"
#include "JOB_MST.h"


// CViewJobListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewJobListDlg, CSkinDialog)

CViewJobListDlg::CViewJobListDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewJobListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CViewJobListDlg::CViewJobListDlg(CEcsDoc* pDoc, CWnd* pParent)
	: CSkinDialog(CViewJobListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}


CViewJobListDlg::~CViewJobListDlg()
{
	m_pDoc->m_pViewJobListDlg = NULL;
	this->DestroyWindow();
}


// [LGLS 2026-08-30] 그리드 자동 갱신용 런타임 컨트롤/타이머 ID.
//   리소스(rc)를 건드리지 않고 코드에서 만든다 - rc 는 사용자가 직접 관리하므로.
#define IDC_LGLS_JL_AUTOREFRESH   0x7F31
#define TIMER_JL_AUTOREFRESH      0x7F32
#define TIMER_JL_AUTOREFRESH_MS   2000

BEGIN_MESSAGE_MAP(CViewJobListDlg, CSkinDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND_RANGE(ID_JOB_INSERT, ID_JOB_UPDATE, &CViewJobListDlg::OnCommandRangeButtonEvent)
	//ON_MESSAGE(WM_USER_REFRESH_COLLECT_JOBINFO, &CViewJobListDlg::OnUpdateSpread)
 	ON_BN_CLICKED(ID_JOB_DATA_CLEAR, &CViewJobListDlg::OnBnClickedJobDataClear)	
	ON_MESSAGE(SSM_CLICK, &CViewJobListDlg::OnSpreadLClick)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_LGLS_JL_AUTOREFRESH, &CViewJobListDlg::OnBnClickedAutoRefresh)
//	ON_BN_CLICKED(ID_JOB_UPDATE, &CViewJobListDlg::OnBnClickedJobUpdate)
ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CViewJobListDlg, CSkinDialog)
	//ON_EVENT(CViewRackDlg, IDC_VIEW_JOB_LIST_SPD_MAIN, 2, CViewJobListDlg::BlockSelectedCellMst, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
//	ON_EVENT(CViewRackDlg, IDC_VIEW_JOB_LIST_SPD_MAIN, 5, CViewJobListDlg::ClickSpread, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CViewJobListDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX); 

//	DDX_Control(pDX, IDC_VIEW_JOB_LIST_SPD_MAIN, m_pSpreadMain);
	DDX_Control(pDX, IDC_LBL_SPD_MAIN_CNT, m_lblSpdMainCnt);
	DDX_Control(pDX, IDC_LBL_JOB_MST_RESULT, m_lblSelectCnt);


	DDX_Control(pDX, ID_JOB_DATA_CLEAR, m_btnJobDataClear);
	DDX_Control(pDX, ID_JOB_DELETE, m_btnJobDelete);
	DDX_Control(pDX, ID_JOB_SEARCH, m_btnJobSearch);
	DDX_Control(pDX, ID_JOB_UPDATE, m_btnJobUpdate);

	DDX_Control(pDX, IDC_LBL_WHTYPE, m_lblWhType);
	DDX_Control(pDX, IDC_VIEW_JOBLIST_COMBO_WHTYPE,		 m_cmbWhTyp);
	DDX_Control(pDX, IDC_LBL_LUGGNUM, m_lblLuggNum);
	DDX_Control(pDX, IDC_VIEW_JOBLIST_EDIT_LUGGNUM,		 m_edtLuggNum);
	DDX_Control(pDX, IDC_LBL_STARTPOS, m_lblStartPos);
	DDX_Control(pDX, IDC_VIEW_JOBLIST_COMBO_STARTPOS,	 m_cmbStartPos);
	DDX_Control(pDX, IDC_LBL_DESTPOS, m_lblDestPos);
	DDX_Control(pDX, IDC_VIEW_JOBLIST_COMBO_DESTPOS,	 m_cmbDestPos);
	DDX_Control(pDX, IDC_LBL_JOBTYP, m_lblJobTyp);
	DDX_Control(pDX, IDC_VIEW_JOBLIST_EDIT_JOB_TYP,		 m_cmbJobTyp);
	DDX_Control(pDX, IDC_LBL_JOB_STATUS, m_lblJobStatus);
	DDX_Control(pDX, IDC_VIEW_JOBLIST_EDIT_JOBSTATUS,	 m_cmbJobStatus);
	DDX_Control(pDX, IDC_LBL_JOB_STATUS2, m_lblJobStatus2);
	DDX_Control(pDX, IDC_VIEW_JOBLIST_EDIT_JOBSTATUS2,	 m_cmbJobStatus2);

	DDX_Control(pDX, IDC_GRP_SEARCH,	 m_grpSearch);
	DDX_Control(pDX, IDC_GRP_SEARCH2,	 m_grpSearch2);
	DDX_Control(pDX, IDC_GRP_UPDATE,	 m_grpEdit);
	DDX_Control(pDX, IDC_LBL_JOB_PRIORITY, m_lblJobPriority);
	DDX_Control(pDX, IDC_LBL_PRODUCT_SIZE, m_lblProductSize);
	DDX_Control(pDX, IDC_VIEW_JOBLIST_EDIT_JOB_PRIORITY, m_cbxJobPriority);
	DDX_Control(pDX, IDC_VIEW_JOBLIST_EDIT_PRODUCT_SIZE, m_cbxProductSize);
	DDX_Control(pDX, ID_JOB_COPY, m_btnJobCopy);
	DDX_Control(pDX, ID_JOB_CV_COMPLETE, m_btnJobCvComplete);
	DDX_Control(pDX, ID_JOB_SC_COMPLETE, m_btnJobScComplete);
}

LRESULT CViewJobListDlg::OnSpreadLClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1){ m_SpreadSheet.SortByHeaderColumn(pCell->Col); return 0; }	// [LGLS] 헤더 클릭 정렬
	if ( pCell->Col == 0 || pCell->Row == 0 )
		return 0;

	if ( m_SpreadSheet.IsCellType(pCell->Col, pCell->Row, SS_TYPE_CHECKBOX) )
	{
		BOOL bCheck = !m_SpreadSheet.GetValueINT(pCell->Col, pCell->Row);
		m_SpreadSheet.SetValueINT(pCell->Col, pCell->Row, bCheck);

		if ( m_SpreadSheet.IsCellType(pCell->Col, 0, SS_TYPE_CHECKBOX) && 
			 m_SpreadSheet.IsCellType(pCell->Col, pCell->Row, SS_TYPE_CHECKBOX) )
		{
			if ( !bCheck )  // Uncheck 시
				m_SpreadSheet.SetHeaderCellCheck(pCell->Col, false);
		}
		return 0;
	}

	//variant_t val;
	if (pCell->Row > 0)
	{
		m_nActiveRow = pCell->Row;

		//작업정보
		CString strJobTyp = m_SpreadSheet.GetValueTXT(7, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
		strJobTyp.Trim();

		//m_pSpreadMain.GetText(10, m_nActiveRow, &val);
		CString strJobSta = m_SpreadSheet.GetValueTXT(8, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
		strJobSta.Trim();
		m_cmbJobStatus2.SetCurSelTextEx(strJobSta);

		// [LGLS 2026-08-01] PULP 단수 컬럼 제거 ? 9=적재 용기(LOT_NO), 10=제품정보(PRODUCT_ID), 11=우선순위
		CString strJobPriority = m_SpreadSheet.GetValueTXT(11, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
		strJobPriority.Trim();
		m_cbxJobPriority.SetCurSelTextEx(strJobPriority);

		//작업구분 가져오기 (CDX_CD, CCD_NM_KOR, CCD_CD 순서)
		CLib::BindSpreadCommonCode(_T("JOB_TYP"), strJobTyp, m_strJOB_TYP, m_pDoc);
	}

	return 0;
}

void CViewJobListDlg::ClearInputData()
{
	CString strWhType;
	CString strLuggNum;
	CString strStartPos;
	CString strDestPos;
	CString strJobStauts;
	CString strJobTyp;

	
	strWhType.Trim();
	strLuggNum.Trim();
	strStartPos.Trim();
	strDestPos.Trim();
	strJobStauts.Trim();
	strJobTyp.Trim();

	m_edtLuggNum.GetWindowText(strLuggNum);
	m_cmbWhTyp.SetCurSel(0);
	m_cmbJobStatus.SetCurSel(0);
	m_cmbJobTyp.SetCurSel(0);
	m_cmbStartPos.SetCurSel(0);
	m_cmbDestPos.SetCurSel(0);

	//m_cmbWhTyp.SetCurSelTextEx(0);
	//m_cmbJobStatus.SetCurSelTextEx(0);
	//m_cmbJobTyp.SetCurSelTextEx(0);
	//m_cmbStartPos.SetCurSelTextEx(0);
	//m_cmbDestPos.SetCurSelTextEx(0);
	
	//m_cmbStartPos.SetWindowText(strStartPos);
	//m_cmbDestPos.SetWindowText(strDestPos);
}



BOOL CViewJobListDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);	
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		m_bInitialized = TRUE;		
	}
//	m_pSpreadMain.SetFontName(Global.GetFontName(pEn));

	RedrawImage();
	RenameResource(pEn);
	
	CLib::BindCombo(m_cmbWhTyp, _T("WH_TYP"), m_pDoc, int(pEn), FALSE);
	CLib::BindCombo(m_cmbJobTyp, _T("JOB_TYP"), m_pDoc, int(pEn), TRUE);
	CLib::BindCombo(m_cmbJobStatus, _T("JOB_STATUS"), m_pDoc, int(pEn), TRUE);
	CLib::BindCombo(m_cmbJobStatus2, _T("JOB_STATUS"), m_pDoc, int(pEn), FALSE);

	// [LGLS 2026-08-30] 작업상태 콤보도 그리드와 같이 "[코드] 상태명" 으로 표기한다.
	//   그리드의 작업상태 열은 SQL 에서 이미 "[19] 출고완료" 로 만들고 있는데
	//   콤보는 "출고완료" 뿐이라, 행을 클릭했을 때 SetCurSelTextEx() 가 텍스트로
	//   찾지 못해 수정 콤보가 그 행의 상태로 맞춰지지 않았다. 표기를 맞추면 같이 풀린다.
	AddCodePrefixToCombo(m_cmbJobStatus);
	AddCodePrefixToCombo(m_cmbJobStatus2);
	AlignUpdateGroupRight();
	CLib::BindCombo(m_cbxJobPriority, _T("JOB_PRIORITY"), m_pDoc, int(pEn), FALSE);
	CLib::BindCombo(m_cbxProductSize, _T("PRODUCT_SIZE"), m_pDoc, int(pEn), FALSE);
	CLib::SetBindCombo_DEST_POS_DEF(m_cmbStartPos, m_pDoc);
	CLib::SetBindCombo_DEST_POS_DEF(m_cmbDestPos, m_pDoc);

	InitializeControlLanguage();
	
	//SetItemReSize();				// 이거때문임
	PreCreateWindow(cs);


#pragma region 스프레드 초기화 
	//IDC_CUSTOM1 라는 컨트롤에서 Rect 값을 가져와서 스프레드에 적용 
	// 상대 좌표 구하기
	CRect rect;							// 윈도우 기준 절대 좌표 
	GetDlgItem(IDC_STATIC_SPREAD)->GetWindowRect(&rect);

	CRect rectTemp = rect;				// 다이얼 로그 영역안의 컨트롤(rect)의 상대좌표 
	ScreenToClient(&rectTemp);
	 
	CRect rtTemp;
	::GetWindowRect(this->m_hWnd, &rtTemp);
	
	CRect rectMargin = CRect(rectTemp.left, rectTemp.top, rtTemp.right-rect.right, rtTemp.bottom - rect.bottom);

//	m_SpreadSheet.m_rectWnd = rectTemp;//CRect(0, 0, 0, 0);
	m_SpreadSheet.m_nSorting = SS_USERCOLACTION_DEFAULT;
	m_SpreadSheet.m_bAutoResizeCol = true;
	m_SpreadSheet.m_colorBaseBack = WHITE;
	m_SpreadSheet.m_wGridType = SS_GRID_HORIZONTAL | SS_GRID_VERTICAL | SS_GRID_SOLID;

	m_SpreadSheet.m_rectMargin = rectMargin;

	HFONT hFontH = CreateFont(15,0,0,0,700,0,0,0,0,0,0,0,0,_T("System"));
	HFONT hFontB = CreateFont(15,0,0,0,FW_BOLD,0,0,0,0,0,0,0,0,_T("System"));

	m_SpreadSheet.AddSheet(_T(""), this, hFontH, hFontB);
	//m_SpreadSheet.AddColHead(_T(" "), 10);
	m_SpreadSheet.AddColHead(_T("창고 타입"), 8);
	m_SpreadSheet.AddColHead(_T("작업 번호"), 8);
	m_SpreadSheet.AddColHead(_T("출발지"), 6);
	m_SpreadSheet.AddColHead(_T("출발 위치"), 9);
	m_SpreadSheet.AddColHead(_T("도착지"), 6);
	m_SpreadSheet.AddColHead(_T("도착 위치"), 9);
	m_SpreadSheet.AddColHead(_T("작업 구분"), 11);
	m_SpreadSheet.AddColHead(_T("작업 상태"), 15);
	m_SpreadSheet.AddColHead(_T("적재 용기"), 12);
	m_SpreadSheet.AddColHead(_T("제품정보"), 12);
	m_SpreadSheet.AddColHead(_T("우선 순위"), 8);
	m_SpreadSheet.AddColHead(_T("수정 시간"), 16);//16

	// [LGLS 2026-08-04] 조회 조건에 [적재 용기]/[제품 정보] 추가 (보이는 행 좌표로 한 행 아래에 배치)
	CLib::AddFilterField(this, IDC_LBL_WHTYPE, IDC_LBL_JOBTYP, IDC_VIEW_JOBLIST_EDIT_JOB_TYP,
	                     _T("적재 용기"), IDC_LGLS_JL_LOT_LBL, IDC_LGLS_JL_LOT_EDT, m_arLglsCtrl);
	CLib::AddFilterField(this, IDC_LBL_LUGGNUM, IDC_LBL_JOB_STATUS, IDC_VIEW_JOBLIST_EDIT_JOBSTATUS,
	                     _T("제품 정보"), IDC_LGLS_JL_PRD_LBL, IDC_LGLS_JL_PRD_EDT, m_arLglsCtrl);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));
	//GetDlgItem(IDC_STATIC_TOP)->BringWindowToTop();
//	m_SpreadSheet.m_Spread.BringWindowToTop();

#pragma endregion

	InitializeSpread(FALSE);

	// [LGLS 2026-08-30] [CV 도착보고] 왼쪽에 [자동 갱신] 체크박스 생성(런타임).
	CreateAutoRefreshCheck();

	{ CString _tt; GetWindowText(_tt); if(_tt.Find(_T("[")) < 0) SetWindowText(_tt + _T(" [JOB_MST]")); }	// [LGLS] 제목에 조회 테이블명
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// [LGLS 2026-08-30] 그리드 자동 갱신 체크박스를 [CV 도착보고] 버튼 왼쪽에 만든다.
//   버튼의 실제 위치를 읽어 그 왼쪽에 붙이므로, 리소스에서 버튼이 옮겨져도 따라간다.
// [LGLS 2026-08-30] 콤보 항목을 "[코드] 이름" 으로 다시 쓴다.
//   코드가 없는 항목(ALL)과 이미 붙어 있는 항목은 건드리지 않는다.
//   ResetContent() 는 목록만 비우고 m_Key 는 그대로 두므로,
//   키를 먼저 걷어낸 뒤 원래 순서대로 다시 넣는다.
// [LGLS 2026-08-30] 수정 그룹의 오른쪽 끝 정렬. ★콤보에 항목이 채워진 뒤에 불러야 한다★
//   RedrawImage() 는 BindCombo 보다 먼저 돌아서, 거기서 부르면 항목 폭이 0 으로 나온다.
// [LGLS 2026-08-30] 작업정보 화면 정렬.
//   ★콤보에 항목이 채워진 뒤에 불러야 한다★ - RedrawImage() 는 BindCombo 보다 먼저 돌아서,
//   거기서 부르면 항목 폭이 0 으로 나온다.
//   기준선은 두 개다.
//     ① 오른쪽 끝 = 그리드(IDC_STATIC_SPREAD) 우변
//        → 작업상태/우선순위 콤보, [수정], [삭제](같은 줄 통째로) 를 여기에 맞춘다.
//     ② [초기화]/[조회] = 조회 그룹의 오른쪽 아래 구석
void CViewJobListDlg::AlignUpdateGroupRight()
{
	CWnd* pGrpU = GetDlgItem(IDC_GRP_UPDATE);
	CWnd* pGrpS = GetDlgItem(IDC_GRP_SEARCH);
	CWnd* pSpd  = GetDlgItem(IDC_STATIC_SPREAD);
	if (pGrpU == NULL || pSpd == NULL) return;

	CRect rcGrpU, rcGrpS, rcSpd;
	pGrpU->GetWindowRect(&rcGrpU); ScreenToClient(&rcGrpU);
	pSpd ->GetWindowRect(&rcSpd);  ScreenToClient(&rcSpd);

	const int nInset = 8;	// 그룹 테두리에서 띄울 여백
	const int nGap   = 7;	// 라벨-콤보 / 버튼-버튼 사이

	// ── ① 오른쪽 끝 = 그리드 우변
	int nRight = rcSpd.right;
	if (nRight > rcGrpU.right - nInset) nRight = rcGrpU.right - nInset;

	// 콤보 : 라벨은 글자 폭만 남기고, 나머지를 콤보가 다 쓴다(RTEXT 라 글자는 콤보에 붙는다).
	int nLeft = rcGrpU.left + nInset + nGap
	          + max(LabelTextWidth(IDC_LBL_JOB_STATUS2), LabelTextWidth(IDC_LBL_JOB_PRIORITY));
	if (nLeft < rcGrpU.left) nLeft = rcGrpU.left + nInset;

	MoveLabelRightTo(IDC_LBL_JOB_STATUS2,  rcGrpU.left + nInset, nLeft - nGap);
	MoveLabelRightTo(IDC_LBL_JOB_PRIORITY, rcGrpU.left + nInset, nLeft - nGap);
	SetComboSpan(m_cmbJobStatus2,  nLeft, nRight);
	SetComboSpan(m_cbxJobPriority, nLeft, nRight);

	// [수정] : 배경이 비트맵이라 늘리면 그림이 늘어진다 - 폭은 두고 위치만 옮긴다.
	MoveCtrlRightTo(&m_btnJobUpdate, nRight);

	// [삭제] 를 그리드 우변에 맞추고, 같은 줄 버튼들을 같은 만큼 함께 민다(간격 유지).
	{
		CRect rcDel;
		m_btnJobDelete.GetWindowRect(&rcDel); ScreenToClient(&rcDel);
		int nShift = nRight - rcDel.right;
		if (nShift != 0)
		{
			ShiftCtrlX(&m_btnJobDelete,     nShift);
			ShiftCtrlX(&m_btnJobCopy,       nShift);
			ShiftCtrlX(&m_btnJobScComplete, nShift);
			ShiftCtrlX(&m_btnJobCvComplete, nShift);
		}
	}

	// ── ② [초기화]/[조회] : 조회 그룹의 오른쪽 아래 구석
	if (pGrpS != NULL)
	{
		pGrpS->GetWindowRect(&rcGrpS); ScreenToClient(&rcGrpS);

		CRect rcSrch, rcClr;
		m_btnJobSearch   .GetWindowRect(&rcSrch); ScreenToClient(&rcSrch);
		m_btnJobDataClear.GetWindowRect(&rcClr);  ScreenToClient(&rcClr);

		int nBottom = rcGrpS.bottom - nInset;
		int nSrchL  = rcGrpS.right - nInset - rcSrch.Width();
		int nClrL   = nSrchL - nGap - rcClr.Width();

		m_btnJobSearch   .MoveWindow(nSrchL, nBottom - rcSrch.Height(), rcSrch.Width(), rcSrch.Height());
		m_btnJobDataClear.MoveWindow(nClrL,  nBottom - rcClr.Height(),  rcClr.Width(),  rcClr.Height());
	}
}

// [LGLS 2026-08-30] 폭은 두고 오른쪽 끝만 맞춘다
void CViewJobListDlg::MoveCtrlRightTo(CWnd* pWnd, int nRight)
{
	if (pWnd == NULL || pWnd->GetSafeHwnd() == NULL) return;

	CRect rc;
	pWnd->GetWindowRect(&rc);
	ScreenToClient(&rc);
	pWnd->MoveWindow(nRight - rc.Width(), rc.top, rc.Width(), rc.Height());
}

// [LGLS 2026-08-30] 가로로 그만큼 민다
void CViewJobListDlg::ShiftCtrlX(CWnd* pWnd, int nDx)
{
	if (pWnd == NULL || pWnd->GetSafeHwnd() == NULL) return;

	CRect rc;
	pWnd->GetWindowRect(&rc);
	ScreenToClient(&rc);
	pWnd->MoveWindow(rc.left + nDx, rc.top, rc.Width(), rc.Height());
}


// [LGLS 2026-08-30] 라벨 글자가 실제로 차지하는 폭
int CViewJobListDlg::LabelTextWidth(int nID)
{
	CWnd* p = GetDlgItem(nID);
	if (p == NULL) return 0;

	CString strText;
	p->GetWindowText(strText);
	CDC* pDC = p->GetDC();
	if (pDC == NULL) return 0;
	CFont* pOld = pDC->SelectObject(p->GetFont());
	int cx = pDC->GetTextExtent(strText).cx;
	pDC->SelectObject(pOld);
	p->ReleaseDC(pDC);
	return cx + 4;
}

// [LGLS 2026-08-30] 라벨의 좌우 끝을 지정한다(RTEXT 이므로 글자는 오른쪽에 붙는다)
void CViewJobListDlg::MoveLabelRightTo(int nID, int nLeft, int nRight)
{
	CWnd* p = GetDlgItem(nID);
	if (p == NULL || nRight <= nLeft) return;

	CRect rc;
	p->GetWindowRect(&rc);
	ScreenToClient(&rc);
	p->MoveWindow(nLeft, rc.top, nRight - nLeft, rc.Height());
}

// [LGLS 2026-08-30] 콤보의 좌우 끝을 지정한다.
//   ★GetDroppedControlRect() 를 써야 한다★ - GetWindowRect() 는 닫힌 상태 높이만 주므로
//   그 값으로 MoveWindow 하면 드롭다운 목록이 펼쳐지지 않는다.
void CViewJobListDlg::SetComboSpan(CComboBoxWrapper& cbx, int nLeft, int nRight)
{
	if (cbx.GetSafeHwnd() == NULL || nRight <= nLeft) return;

	CRect rc;
	cbx.GetDroppedControlRect(&rc);
	ScreenToClient(&rc);
	cbx.MoveWindow(nLeft, rc.top, nRight - nLeft, rc.Height());
}


// [LGLS 2026-08-30] 콤보 항목 중 가장 긴 것이 잘리지 않는 폭(드롭다운 화살표 + 여백 포함).
int CViewJobListDlg::CalcComboItemWidth(CComboBoxWrapper& cbx)
{
	if (cbx.GetSafeHwnd() == NULL) return 0;

	CDC* pDC = cbx.GetDC();
	if (pDC == NULL) return 0;
	CFont* pOld = pDC->SelectObject(cbx.GetFont());

	int nMax = 0;
	for (int i = 0; i < cbx.GetCount(); i++)
	{
		CString strText;
		cbx.GetLBText(i, strText);
		int cx = pDC->GetTextExtent(strText).cx;
		if (cx > nMax) nMax = cx;
	}

	pDC->SelectObject(pOld);
	cbx.ReleaseDC(pDC);

	// 드롭다운 화살표(시스템 스크롤바 폭) + 좌우 여백
	return nMax + ::GetSystemMetrics(SM_CXVSCROLL) + 10;
}

// [LGLS 2026-08-30] 콤보의 오른쪽 끝을 지정한 x 좌표에 맞춘다(왼쪽 위치는 그대로).
//   드롭다운 목록 높이까지 포함한 GetDroppedControlRect() 를 써야 한다.
//   GetWindowRect() 의 높이(닫힌 상태)로 MoveWindow 하면 목록이 안 펼쳐진다.
void CViewJobListDlg::AlignComboRightTo(CComboBoxWrapper& cbx, int nRight)
{
	if (cbx.GetSafeHwnd() == NULL) return;

	CRect rcCbx;
	cbx.GetDroppedControlRect(&rcCbx);
	ScreenToClient(&rcCbx);

	if (nRight <= rcCbx.left) return;	// 비정상 좌표면 건드리지 않는다
	rcCbx.right = nRight;
	cbx.MoveWindow(&rcCbx);
}

void CViewJobListDlg::AddCodePrefixToCombo(CComboBoxWrapper& cbx)
{
	int nCnt = cbx.GetCount();
	if (nCnt <= 0) return;

	int nSel = cbx.GetCurSel();
	CStringArray arrKey, arrText;

	for (int i = 0; i < nCnt; i++)
	{
		CString strText, strKey = cbx.GetItemKey(i);
		cbx.GetLBText(i, strText);
		strKey.Trim();
		strText.Trim();

		if (!strKey.IsEmpty() && strKey != _T("ALL") && strText.Left(1) != _T("["))
		{
			CString strNew;
			strNew.Format(_T("[%s] %s"), strKey, strText);
			strText = strNew;
		}
		arrKey.Add(strKey);
		arrText.Add(strText);
	}

	cbx.ResetContent();
	for (int i = 0; i < nCnt; i++)
	{
		cbx.SetItemDataEx(i, arrKey.GetAt(i));
		cbx.AddString(arrText.GetAt(i));
	}
	cbx.SetCurSel(nSel < 0 ? 0 : nSel);
}

void CViewJobListDlg::CreateAutoRefreshCheck()
{
	if (m_chkAutoRefresh.GetSafeHwnd() != NULL)
		return;

	CWnd* pBtn = GetDlgItem(ID_JOB_CV_COMPLETE);
	if (pBtn == NULL)
		return;

	CRect rcBtn;
	pBtn->GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);

	const int nW = 104;					// 체크박스 폭
	const int nGap = 8;					// 버튼과의 간격
	CRect rc(rcBtn.left - nGap - nW, rcBtn.top, rcBtn.left - nGap, rcBtn.bottom);

	m_chkAutoRefresh.Create(m_pDoc->GetMsgLangDef(_T("자동 갱신")),
							WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
							rc, this, IDC_LGLS_JL_AUTOREFRESH);
	m_chkAutoRefresh.SetFont(GetFont());
	m_chkAutoRefresh.SetCheck(BST_UNCHECKED);
	// [LGLS 2026-08-30] 그룹박스보다 뒤(Z순서 아래)에 만들어지면 클릭이 그룹박스에 먹혀 눌리지 않는다.
	//   맨 앞으로 올려 마우스 입력을 받게 한다.
	m_chkAutoRefresh.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_chkAutoRefresh.BringWindowToTop();
}

void CViewJobListDlg::OnBnClickedAutoRefresh()
{
	if (m_chkAutoRefresh.GetSafeHwnd() == NULL)
		return;

	if (m_chkAutoRefresh.GetCheck() == BST_CHECKED)
	{
		SetTimer(TIMER_JL_AUTOREFRESH, TIMER_JL_AUTOREFRESH_MS, NULL);
		InitializeSpread(TRUE);			// 켜는 즉시 한 번 갱신
	}
	else
	{
		KillTimer(TIMER_JL_AUTOREFRESH);
	}
}

void CViewJobListDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_JL_AUTOREFRESH)
	{
		// 체크가 풀렸는데 타이머가 남아 있으면 정리한다(방어).
		if (m_chkAutoRefresh.GetSafeHwnd() == NULL || m_chkAutoRefresh.GetCheck() != BST_CHECKED)
			KillTimer(TIMER_JL_AUTOREFRESH);
		else
			InitializeSpread(TRUE);		// [조회] 와 같은 경로로 목록을 다시 읽는다
	}

	CSkinDialog::OnTimer(nIDEvent);
}

void CViewJobListDlg::OnCommandRangeButtonEvent(UINT nID)
{
	switch(nID)
	{
	case ID_JOB_INSERT:
		{
			break;
		}
	case ID_JOB_DELETE:
		{
			DeleteJob();
			break;
		}
	case ID_JOB_DATA_CLEAR:
		{
			ClearInputData();
			break;
		}
	case ID_JOB_SEARCH:
		{
			InitializeSpread(TRUE);
			break;
		}    
	case ID_JOB_UPDATE:
		{
			UpdateJob();
			break;
		}
	case ID_JOB_COPY:
		{
			CopyJob();
			break;
		}
	case ID_JOB_CV_COMPLETE:
		{
			JobComplete(_T("19"));
			break;
		}
	case ID_JOB_SC_COMPLETE:
		{
			JobComplete(_T("29"));
			break;
		}
	default:
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("정의되지 않은 버튼 이벤트")) + CConvert::ToString((int)nID));
		}
	}
}

void CViewJobListDlg::UpdateJob()
{
	if (!m_pDoc->Permission(_T("CViewJobListDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	variant_t val;
	CString strSql;
	CString strWH_TYP, strLUGG_NO, strJobStatus, strProductSize, strJobPriority;

	if (m_nActiveRow < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("스프레드를 클릭하시오"))); 
		return;
	}
	
	//m_pSpreadMain.GetText(1, m_nActiveRow, &val);
	strWH_TYP = m_SpreadSheet.GetValueTXT(1, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	//m_pSpreadMain.GetText(2, m_nActiveRow, &val);
	strLUGG_NO = m_SpreadSheet.GetValueTXT(2, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	strJobStatus = m_cmbJobStatus2.GetItemKey(m_cmbJobStatus2.GetCurSel());
	strProductSize = m_cbxProductSize.GetItemKey(m_cbxProductSize.GetCurSel());
	strJobPriority = m_cbxJobPriority.GetItemKey(m_cbxJobPriority.GetCurSel());

	strWH_TYP.Trim();
	strLUGG_NO.Trim();
	strJobStatus.Trim();
	strProductSize.Trim();
	strJobPriority.Trim();


	CString strSpace = _T(" ");
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업 상태를 수정 하시겠습니까?")) + strSpace + _T("[LUGG_NO : ") + strLUGG_NO + _T(" ]"), MB_YESNO) != IDYES) 
		return;

	UpdateData(TRUE);

	long bTrans = m_pDoc->BeginTrans_DLG();

	if (bTrans < 1)
		return;

	CString strLOG_LUGG_NO = strLUGG_NO;
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
	CString strLOG_MSG = _T("JOB_MST UPDATE : JOB_STA -> ") + strJobStatus;
	if (!m_pDoc->GetQueryInsertClientLog(_T("CViewJobListDlg"), strLOG_LUGG_NO, _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	strWH_TYP = m_cmbWhTyp.GetItemCCD(strWH_TYP);
	strSql=_T("");
	strSql.Format(_T("UPDATE JOB_MST     ")
		_T("	  SET JOB_STATUS = '%s'  ")
		_T("	    , PRODUCT_SIZE = '%s'  ")
		_T("	    , JOB_PRIORITY    = '%s'  ")
		_T("    WHERE WH_TYP     = '%s'  ")
		_T("      AND LUGG_NO    = '%s'  "), strJobStatus, strProductSize, strJobPriority, strWH_TYP, strLUGG_NO);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	m_pDoc->CommitTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
	InitializeSpread(TRUE);
}

void CViewJobListDlg::JobComplete(CString strJOB_STATUS)
{
	if (!m_pDoc->Permission(_T("CViewJobListDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	variant_t val;
	CString strSql;
	CString strWH_TYP, strLUGG_NO, strJOB_TYP;


	if (m_nActiveRow < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("스프레드를 클릭하시오"))); 
		return;
	}
	
	//m_pSpreadMain.GetText(1, m_nActiveRow, &val);
	strWH_TYP = m_SpreadSheet.GetValueTXT(1, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	//m_pSpreadMain.GetText(2, m_nActiveRow, &val);
	strLUGG_NO = m_SpreadSheet.GetValueTXT(2, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	strJOB_TYP = m_strJOB_TYP;

	strWH_TYP.Trim();
	strLUGG_NO.Trim();
	strJOB_TYP.Trim();


	CString strSpace = _T(" ");
	CString strMessage = _T(" ");
	
	if(strJOB_STATUS == _T("19"))
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 도착보고를 하시겠습니까?")) + strSpace + _T("[LUGG_NO : ") + strLUGG_NO + _T(" ]"), MB_YESNO) != IDYES) 
			return;

		if(strJOB_TYP == _T("1") || strJOB_TYP == _T("4") || strJOB_TYP == _T("5")) //236
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업구분은 CV 도착보고를 할 수 없습니다.")));
			return;
		}
	}
	else
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 구동완료 보고를 하시겠습니까?")) + strSpace + _T("[LUGG_NO : ") + strLUGG_NO + _T(" ]"), MB_YESNO) != IDYES) 
			return;

		if(strJOB_TYP == _T("2") || strJOB_TYP == _T("3") || strJOB_TYP == _T("6"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업구분은 SC 구동완료 보고를 할 수 없습니다.")));
			return;
		}
	}
	

	UpdateData(TRUE);

	long bTrans = m_pDoc->BeginTrans_DLG();

	if (bTrans < 1)
		return;

	CString strLOG_LUGG_NO = strLUGG_NO;
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
	CString strLOG_MSG = _T("JOB_MST UPDATE : JOB_STATUS -> ") + strJOB_STATUS;
	if (!m_pDoc->GetQueryInsertClientLog(_T("CViewJobListDlg"), strLOG_LUGG_NO, _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	strWH_TYP = m_cmbWhTyp.GetItemCCD(strWH_TYP);
	strSql=_T("");
	strSql.Format(_T("UPDATE JOB_MST     ")
		_T("	  SET JOB_STATUS = '%s'  ")
		_T("    WHERE WH_TYP     = '%s'  ")
		_T("      AND LUGG_NO    = '%s'  "), strJOB_STATUS, strWH_TYP, strLUGG_NO);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	m_pDoc->CommitTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
	InitializeSpread(TRUE);
}

void CViewJobListDlg::InitializeControlLanguage()
{
	
	// [LGLS 2026-08-30] 조회 필터 라벨이 한 칸씩 밀려 있었다 - JOB_STATUS 콤보에 "작업구분" 이 붙어
	//   화면상 "작업상태" 필터가 사라져 보였다. 그리드 머리글(작업 구분 / 작업 상태) 및
	//   다국어 ini(dlg_jobinfo.ini : jobtyp=작업구분, jobsta=작업상태) 와 일치시킨다.
	m_lblJobTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분"), m_pDoc->m_enLang));
	m_lblJobStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업상태"), m_pDoc->m_enLang));
	
	m_lblLuggNum.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호"), m_pDoc->m_enLang));
	m_lblStartPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발지"), m_pDoc->m_enLang));
	m_lblDestPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착지"), m_pDoc->m_enLang));
	m_lblWhType.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("창고구분"), m_pDoc->m_enLang));
	m_lblSpdMainCnt.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("0"), m_pDoc->m_enLang));
	
}


void CViewJobListDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	if( m_bInitialized )
	{
		m_bInitialized = TRUE;
	}
	Invalidate(TRUE);
}

HCURSOR CViewJobListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CViewJobListDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CViewJobListDlg::RelocationControls()
{
// 	CRect rc;
// 	GetClientRect(&rc);
// 
// 	int x=0, y=0;
// 
// 	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
// 
// 	x = rc.Width() - 20 - size.cx;
// 	y = rc.Height() - 10 - size.cy;
// 	m_btnCancel.MoveWindow(x, y, size.cx, size.cy, TRUE);
// 
// 	x -= size.cx + 10; 
// 	m_btnOk.MoveWindow(x, y, size.cx, size.cy, TRUE);
// 	m_btnOk.Invalidate();
}


void CViewJobListDlg::FillCopyJob(int nColIdx, int nRowIdx, CString &strValue)
{
	//variant_t val;
	//m_pSpreadMain.GetText(nColIdx+1, nRowIdx, &val);
	//strValue = val;
	//strValue = strValue.Trim();
}

void CViewJobListDlg::OnBnClickedJobDelete()
{
	//DeleteJob();
}

void CViewJobListDlg::DeleteJob()
{
	if (!m_pDoc->Permission(_T("CViewJobListDlg"), DEL_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}
	

	variant_t val;
//	int nActiveRow;
	CString strLUGG_NO, strSTART_POS, strSTART_LOCATION, strDEST_POS, strDEST_LOCATION, strJOB_TYP, strJOB_STATUS, strLOT_NO, strPRODUCT_ID;
	CString strSql;
	CString strWH_TYP = m_pDoc->m_WH_TYP;
	//nActiveRow = m_pSpreadMain.GetActiveRow();
	//if (nActiveRow < 1)
	//	return;

	
	//m_pSpreadMain.GetText(2, nActiveRow, &val);
	strLUGG_NO = m_SpreadSheet.GetValueTXT(2, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;

	//m_pSpreadMain.GetText(3, nActiveRow, &val);
	strSTART_POS = m_SpreadSheet.GetValueTXT(3, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;

	//m_pSpreadMain.GetText(4, nActiveRow, &val);
	strSTART_LOCATION = m_SpreadSheet.GetValueTXT(4, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;

	//m_pSpreadMain.GetText(5, nActiveRow, &val);
	strDEST_POS = m_SpreadSheet.GetValueTXT(5, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;

	//m_pSpreadMain.GetText(6, nActiveRow, &val);
	strDEST_LOCATION = m_SpreadSheet.GetValueTXT(6, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;

	//m_pSpreadMain.GetText(8, nActiveRow, &val);
	strJOB_TYP = m_SpreadSheet.GetValueTXT(7, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;

	//m_pSpreadMain.GetText(10, nActiveRow, &val);
	strJOB_STATUS = m_SpreadSheet.GetValueTXT(8, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;

	// [LGLS 2026-08-01] 9=적재 용기(LOT_NO), 10=제품정보(PRODUCT_ID)
	strLOT_NO     = m_SpreadSheet.GetValueTXT(9,  m_nActiveRow);
	strPRODUCT_ID = m_SpreadSheet.GetValueTXT(10, m_nActiveRow);



	strLUGG_NO.Trim();
	strSTART_POS.Trim();
	strSTART_LOCATION.Trim();
	strDEST_POS.Trim();
	strDEST_LOCATION.Trim();
	strJOB_TYP.Trim();
	strJOB_STATUS.Trim();
	strLOT_NO.Trim();
	strPRODUCT_ID.Trim();

	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("삭제하시겠습니까? 작업번호 : ")) + strLUGG_NO, MB_YESNO) == IDNO) {	return;	}
	
	m_pDoc->BeginTrans_DLG();

	CString strLOG_LUGG_NO = strLUGG_NO;
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
	CString strLOG_BOTTOM_TRAY = _T("");
	CString strLOG_TOP_TRAY = _T("");
	CString strLOG_MSG = _T("");
	strLOG_MSG.Format(_T("작업삭제 : 출발지[%s], 출발위치[%s], 도착지[%s], 도착위치[%s], 작업구분[%s], 작업상태[%s], 적재 용기[%s], 제품정보[%s]"),strSTART_POS, strSTART_LOCATION, strDEST_POS, strDEST_LOCATION, strJOB_TYP, strJOB_STATUS, strLOT_NO, strPRODUCT_ID) ;

	if (!m_pDoc->GetQueryInsertClientLog(_T("CViewJobListDlg"), strLOG_LUGG_NO, _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	//해당 작업번호가 공PLT 작업인지 체크
	int nRowCnt = 0;
	strSql = _T("");
	CString strMessage = _T("");
	
	strSql.Format(_T("  SELECT *						")
			      _T("	  FROM HOST_EMPTY_PLT			")
			      _T("	 WHERE WH_TYP = '%s'			")
			      _T("	   AND LUGG_NO = '%s'			")
			      _T("	   AND STATUS = 'Q'				"), strWH_TYP, strLUGG_NO);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	
	pRsw->MoveFirst(); 
	
	delete pRsw;
	
	//조회건수가 있다 -> host_empty_plt에서도 삭제해주기
	if(nRowCnt > 0)
	{
		strSql.Format(_T("DELETE FROM HOST_EMPTY_PLT	")
					  _T("		WHERE WH_TYP = '%s'		")
					  _T("		  AND LUGG_NO= '%s'		"), strWH_TYP, strLUGG_NO);

		int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
		
		if(isSuccess == FALSE)
		{
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("공PLT 작업 삭제 중 실패")));
			InitializeSpread(TRUE);
			return;
		}
	}

	strSql = _T("");
	strSql.Format(_T("DELETE FROM JOB_MST				")
			_T("				WHERE WH_TYP = '%s'			")
			_T("				  AND LUGG_NO= '%s'		"), strWH_TYP, strLUGG_NO);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
	
	if(isSuccess == TRUE)
	{
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		InitializeSpread(TRUE);
		return;
	}

	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));

	return;	
}

void CViewJobListDlg::OnBnClickedJobDataClear()
{
	//ClearInputData();
}

void CViewJobListDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pDoc->m_pViewJobListDlg = NULL;
	CSkinDialog::OnClose();
}


//void CViewJobListDlg::OnBnClickedCheckAll()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	if (m_chkCheckAll.GetCheck() == TRUE)
//	{
//		for(int nIdxRow = 1; nIdxRow < m_pSpreadMain.GetDataRowCnt() + 1; nIdxRow++)
//		{
//			m_pSpreadMain.SetText(1, nIdxRow, variant_t(_T("1")));
//		}
//	}
//	else
//	{
//		for(int nIdxRow = 1; nIdxRow < m_pSpreadMain.GetDataRowCnt() + 1; nIdxRow++)
//		{
//			m_pSpreadMain.SetText(1, nIdxRow, variant_t(_T("0")));
//		}
//	}
//}

void CViewJobListDlg::OnBnClickedViewJoblistCancel()
{
	OnClose();
}

void CViewJobListDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_WHTYPE, strValue);

	
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_JOBTYP, strValue);

	
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("luggno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LUGGNUM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobsta"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_JOB_STATUS, strValue);

	//ㅂ
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_STARTPOS, strValue);

	

	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_DESTPOS, strValue);

	

	

	

	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobsta"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_JOB_STATUS2, strValue);


	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("clear"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(ID_JOB_DATA_CLEAR, strValue);

		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(ID_JOB_UPDATE, strValue);

	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(ID_JOB_DELETE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("copy"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(ID_JOB_COPY, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(ID_JOB_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_UPDATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_JOB_MST_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("productsize"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_PRODUCT_SIZE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobpriority"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_JOB_PRIORITY, strValue);
	
}

void CViewJobListDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;

	m_btnJobDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnJobDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);
	m_btnJobDelete.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnJobDelete.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnJobDataClear.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnJobDataClear.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("dateclear"), strExtension)), NULL, 5, 5);
	m_btnJobDataClear.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnJobDataClear.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnJobSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnJobSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
	m_btnJobSearch.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnJobSearch.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnJobUpdate.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnJobUpdate.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
	m_btnJobUpdate.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnJobUpdate.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnJobCopy.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnJobCopy.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("copy"), strExtension)), NULL, 5, 5);
	m_btnJobCopy.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnJobCopy.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnJobCvComplete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnJobCvComplete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("copy"), strExtension)), NULL, 5, 5);
	m_btnJobCvComplete.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnJobCvComplete.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnJobScComplete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnJobScComplete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("copy"), strExtension)), NULL, 5, 5);
	m_btnJobScComplete.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnJobScComplete.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);
}


void CViewJobListDlg::SetBindCombo_DEST_POS_DEF(CComboBoxWrapper& cbx)
{
	CStringList strList;
	CString strSql;
	CString strTRACK_NO, strREMARKS, strMC_NO;
	int nRowCnt = 0, j=0;
	CString strDEST_POS;
	CString strMessage;
	cbx.ResetContent();
	strSql.Format(_T("  SELECT TRACK_NO						")
				  _T("       , PRIORITY						") 
				  _T("       , REMARKS						") 
				  _T("       , GROUP_NO						")
				  _T("       , MC_NO						")
				  _T("	  FROM DEST_POS_DEF					")
				  _T("ORDER BY GROUP_NO, MC_NO, PRIORITY	"));

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	
	cbx.SetItemDataEx(0, _T("ALL"));
	cbx.AddString(_T("ALL"));

	pRsw->MoveFirst(); 
	
	for(int i = 1; i <= nRowCnt; i++)
	{
		strTRACK_NO = pRsw->GetItem(_T("MC_NO"));
		strMC_NO = pRsw->GetItem(_T("MC_NO"));
		cbx.SetItemDataEx(i, strTRACK_NO);
		cbx.AddString(strMC_NO);

		pRsw->MoveNext();
	}
	cbx.SetCurSel(0);
	delete pRsw;
}



CString CViewJobListDlg::GetQrySelect_Main(BOOL bSearch)
{
 	CString strSql = _T("");
	CString strTemp = _T("");															
	CString strLUGG_NO = _T("");
	CString strSTART_POS = _T("");
	CString strDEST_POS = _T("");
	CString strJOB_TYP = _T("");
	CString strJOB_STATUS = _T("");

	m_edtLuggNum.GetWindowText(strLUGG_NO);

	int asd =m_cmbStartPos.GetCurSel();
	strSTART_POS = m_cmbStartPos.GetItemKey(m_cmbStartPos.GetCurSel());
	strDEST_POS = m_cmbDestPos.GetItemKey(m_cmbDestPos.GetCurSel());
	strJOB_TYP = m_cmbJobTyp.GetItemKey(m_cmbJobTyp.GetCurSel());
	strJOB_STATUS = m_cmbJobStatus.GetItemKey(m_cmbJobStatus.GetCurSel());
	
	strLUGG_NO.Trim();
	strSTART_POS.Trim();
	strDEST_POS.Trim();
	strJOB_TYP.Trim();
	strJOB_STATUS.Trim();

	
	strSql.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, JM.WH_TYP) AS WH_TYP 								\n")	
			  _T("		     ,") + m_pDoc->NVL + _T("(JM.LUGG_NO, ' ') AS LUGG_NO											\n")	
			  _T("		     ,JM.START_POS AS START_POS																	\n")	
			  _T("		     ,") + m_pDoc->NVL + _T("(JM.START_LOCATION, ' ') AS START_LOCATION										\n")	
			  _T("		     ,JM.DEST_POS AS DEST_POS																	\n")	
			  _T("		     ,") + m_pDoc->NVL + _T("(JM.DEST_LOCATION, ' ') AS DEST_LOCATION											\n")	
			  _T("		     ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR, JM.JOB_TYP) AS JOB_TYP											\n")	
			  _T("		     ,") + _T("'[' + JM.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CC.CCD_NM_KOR, JM.JOB_STATUS) AS JOB_STATUS										\n")	
			  _T("		     ,") + m_pDoc->NVL + _T("(JM.LOT_NO, ' ') AS LOT_NO											\n")	
			  _T("		     ,") + m_pDoc->NVL + _T("(JM.PRODUCT_ID, ' ') AS PRODUCT_ID										\n")	
			  _T("		     ,") + m_pDoc->NVL + _T("(JM.JOB_PRIORITY, ' ') AS JOB_PRIORITY											\n")		
			  _T("		     ,JM.UPD_DT AS UPD_DT																		\n")	
			  _T("       FROM JOB_MST JM LEFT OUTER JOIN COMMON_CODE CC													\n")	
			  _T("				            ON CC.WH_TYP LIKE '%%%s%%'													\n")	
			  _T("				           AND CC.CDX_CD = 'JOB_STATUS'													\n")	
			  _T("				           AND JM.JOB_STATUS = CC.CCD_CD												\n")	
			  _T("                   LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP												\n")
			  _T("								  ON CCD_WH_TYP.WH_TYP LIKE '%%%s%%'									\n")	
			  _T("								 AND CCD_WH_TYP.CDX_CD = 'WH_TYP'										\n")	
			  _T("								 AND JM.WH_TYP = CCD_WH_TYP.CCD_CD										\n")	
			  _T("                   LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP											\n")
			  _T("								  ON CCD_JOB_TYP.WH_TYP LIKE '%%%s%%'									\n")	
			  _T("								 AND CCD_JOB_TYP.CDX_CD = 'JOB_TYP'										\n")	
			  _T("								 AND JM.JOB_TYP = CCD_JOB_TYP.CCD_CD									\n")	
			  _T("  WHERE JM.WH_TYP = '%s'											\n"), m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP);				    
	
	
	if (strLUGG_NO != "")
	{		
		strSql += _T("AND JM.LUGG_NO LIKE '%") + strLUGG_NO + _T("%' \n");	
	}
	
	
	//{		
	//}
	//
	// [LGLS 2026-08-01] 적재 용기(LOT_NO) / 제품 정보(PRODUCT_ID) 조회 조건
	{
		CString strLOT_NO, strPRODUCT_ID;
		GetDlgItemText(IDC_LGLS_JL_LOT_EDT, strLOT_NO);     strLOT_NO.Trim();
		GetDlgItemText(IDC_LGLS_JL_PRD_EDT, strPRODUCT_ID); strPRODUCT_ID.Trim();
		if (strLOT_NO != _T(""))
			strSql += _T("AND JM.LOT_NO LIKE '%") + strLOT_NO + _T("%' ") + _T("\n");
		if (strPRODUCT_ID != _T(""))
			strSql += _T("AND JM.PRODUCT_ID LIKE '%") + strPRODUCT_ID + _T("%' ") + _T("\n");
	}
	if ((strSTART_POS != "") && (strSTART_POS != _T("ALL")))
	{	
		strSql += _T("AND JM.START_POS ='") + strSTART_POS + _T("' \n");	
	}
	
	if ((strDEST_POS != "") && (strDEST_POS != _T("ALL")))
	{		
		strSql += _T("AND JM.DEST_POS ='") + strDEST_POS + _T("' \n");	
	}
	
	if ((strJOB_TYP != "") && (strJOB_TYP != _T("ALL")))
	{	
		strSql += _T("AND JM.JOB_TYP ='") + strJOB_TYP + _T("' \n");	
	}
	
	if ((strJOB_STATUS != "") && (strJOB_STATUS != _T("ALL")))
	{	
		strSql += _T("AND JM.JOB_STATUS ='") + strJOB_STATUS + _T("' \n");	
	}

   	strSql += _T("   ORDER BY JM.UPD_DT DESC, JM.LUGG_NO DESC \n");


   return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CViewJobListDlg::ClickSpread(long Col, long Row) //셀클릭
{
	/*
	variant_t val;
	if (Row>0)
	{
		m_nActiveRow = Row;

		m_pSpreadMain.GetText(10, m_nActiveRow, &val);
		CString strJobSta = (LPCTSTR)(_bstr_t)val;
		strJobSta.Trim();
		m_cmbJobStatus2.SetCurSelTextEx(strJobSta);

		m_pSpreadMain.GetText(11, m_nActiveRow, &val);
		CString strProductSize = (LPCTSTR)(_bstr_t)val;
		strProductSize.Trim();
		m_cbxProductSize.SetCurSelTextEx(strProductSize);

		m_pSpreadMain.GetText(12, m_nActiveRow, &val);
		CString strJobPriority = (LPCTSTR)(_bstr_t)val;
		strJobPriority.Trim();
		m_cbxJobPriority.SetCurSelTextEx(strJobPriority);
	}
	*/
}


////////SPREAD 값채우기
void CViewJobListDlg::InitializeSpread(BOOL bSearch = FALSE)
{
	
	//정렬 가능
	//m_pSpreadMain.SetUserColAction(1);

	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);
	m_SpreadSheet.PrepareLoadSpread();

	int nRowCnt = SetSpeadData(bSearch);
	m_lblSpdMainCnt.SetWindowText(CConvert::ToString(nRowCnt));

	m_SpreadSheet.FinishLoadSpread();

}

void CViewJobListDlg::SetMaxRows(int pRowCnt)
{
//	m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CViewJobListDlg::SetColWidth(int pColCnt, int pColSize)
{
//	m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CViewJobListDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
{
	CLib::GetColumnName(pStrArrColName, pStrSql);
	nColIdx = pStrArrColName.GetCount();
	int nStartCol = 0;
	if (pStrArrColName[0].Right(5) == _T("_HIDE"))
	{
		nStartCol = - 1;
	}

	//SetMaxCols(nColIdx + nStartCol);

	CString strTemp = _T("");
	CStringArray arrColRename;
	CLib::RenameColumn(pStrArrColName, m_pDoc->m_enLang, arrColRename);
	//int nNewColIdx = 0;
	for(int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		// [LGLS 2026-08-01] 한글 헤더는 글자수만큼의 폭으로는 잘린다(데이터 폭 계산과 동일하게 1.5배).
		int nSize = (int)(arrColRename[nIdxCol].GetLength() * 2);

		strTemp = arrColRename[nIdxCol];

		if (strTemp.GetLength() > 5 && strTemp.Right(5) == _T("_HIDE"))
		{
			nSize = 0;
			if (nIdxCol == 0)
				nSize = 3;

			continue;
		}
		//++nNewColIdx;
		pStrArrColSize.Add(CConvert::ToString(nSize));
	}

	//nStartCol = 1;
	for(int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		CString asd = arrColRename[nIdxCol];
		
		strTemp = arrColRename[nIdxCol];

		if (strTemp.GetLength() > 5 && strTemp.Right(5) == _T("_HIDE"))
			continue;

		SetColumnText(nIdxCol + nStartCol, 0, arrColRename[nIdxCol]);
		
	}

	return 0;
}

void CViewJobListDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
	//*/
}

int CViewJobListDlg::SetSpeadData(BOOL bSearch = FALSE)
{
	CStringList strList;
	CStringArray arrColName;
	CStringArray arrColSize;
	int nLANG = m_pDoc->m_enLang;
	int nColSize = -1;
	int nRowCnt = -1;
	CString strMessage = _T("");

	CString strSql = GetQrySelect_Main(bSearch);

	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp); 

	if(nRowCnt < 0)
	{
		if(pRsw != NULL)
		{
			delete pRsw;	
		}
	}
	else
	{	
		// arrColName과 arrColSize의 갯수가 다름!! Hide되어있는 Column을 가져오느라 어쩔수 없음~
		int nIdxStart = SetHeadColumn(arrColName, arrColSize, nColSize, strSql);

		//SetMaxRows(nRowCnt);

		pRsw->MoveFirst();

		for( int nIdxRow = 1; nIdxRow <= nRowCnt; nIdxRow++ )
		{
			m_SpreadSheet.SetCurrentRow();
			for (int nIdxCol = nIdxStart; nIdxCol < nColSize; nIdxCol++)		
			{
				CString strColValue = pRsw->GetItem(arrColName[nIdxCol]);

				strColValue += _T("    ");
				//if (strColValue.GetLength() < 6)
				//	continue;

				int nPreSize = CConvert::ToInt(arrColSize[nIdxCol]);

				if ((strColValue.GetLength()*1.5 > nPreSize) && (nPreSize != 0))
				{
					int nSize = strColValue.GetLength()*1.5;
					arrColSize[nIdxCol]= CConvert::ToString(nSize);
				}
				SetColumnText(nIdxCol, nIdxRow, strColValue);				// 번역됨
				//m_SpreadSheet.SetData(nIdxCol, nIdxRow, strColValue);		// 번역안됨

				// 짝수마다 색깔을 다르게 칠할것!
				if (nIdxRow % 2 == 0)
				{
					m_SpreadSheet.SetColor(nIdxCol+1, nIdxRow, m_pDoc->m_pConfig->m_clrSPREAD_COLOR, BLACK);
				}
				else
				{
					m_SpreadSheet.SetColor(nIdxCol+1, nIdxRow, SPREAD_COLOR_NONE, BLACK);					
				}

			}
			pRsw->MoveNext();
		}

		if (pRsw != NULL)
		{
			delete pRsw;
		}

		for (int nIdxCol = 0; nIdxCol < nColSize; nIdxCol++)
		{
			int nSize =CConvert::ToInt(arrColSize[nIdxCol]);

			//SetColWidth(nIdxCol, nSize);
			m_SpreadSheet.SetColWidth(nIdxCol + 1, nSize);
		}
	}
	return nRowCnt;
}

void CViewJobListDlg::SetMaxCols(int pMaxCol)
{
//	m_pSpreadMain.SetMaxCols(pMaxCol);
}

BOOL CViewJobListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)  
	{
		int nID = GetFocus()->GetDlgCtrlID();

		switch(pMsg->wParam)
		{
		case VK_RETURN:
			{
				break;
			}
		case VK_ESCAPE:
			{
				break;
			}
		}
		if (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
		{
			switch(pMsg->wParam)
			{
			case 1768:
				{
					return true;
					break;
				}
			case 1868:
				{
					return true;
					break;
				}
			case 37://LEFT
				{
					return true;
					break;
				}
			case 38://UP
				{
					return true;
					break;
				}
			case 39://RIGHT
				{
					return true;
					break;
				}
			case 40://DOWN
				{
					return true;
					break;
				}
			}
		}
	}
	return CSkinDialog::PreTranslateMessage(pMsg);
}


void CViewJobListDlg::SetItemReSize()
{
	CStringArray arrItemID;

	GetItemID(arrItemID);

	for(int i=0; i<arrItemID.GetCount(); i++)
		ReSizing(CConvert::ToInt(arrItemID[i]));
}

void CViewJobListDlg::GetItemID(CStringArray& strArr)
{
	strArr.Add(CConvert::ToString(IDC_GRP_SEARCH));
	strArr.Add(CConvert::ToString(IDC_GRP_SEARCH2));
	strArr.Add(CConvert::ToString(IDC_GRP_UPDATE));
 	strArr.Add(CConvert::ToString(IDC_VIEW_JOB_LIST_SPD_MAIN));
 	strArr.Add(CConvert::ToString(IDC_VIEW_JOB_LIST_SPD_MAIN));
 	strArr.Add(CConvert::ToString(IDC_LBL_SPD_MAIN_CNT));
 	strArr.Add(CConvert::ToString(IDC_LBL_JOB_MST_RESULT));
 	strArr.Add(CConvert::ToString(ID_JOB_DATA_CLEAR));
 	strArr.Add(CConvert::ToString(ID_JOB_DELETE));
 	strArr.Add(CConvert::ToString(ID_JOB_SEARCH));
 	strArr.Add(CConvert::ToString(ID_JOB_UPDATE));
 	strArr.Add(CConvert::ToString(IDC_LBL_WHTYPE));
 	strArr.Add(CConvert::ToString(IDC_LBL_STARTPOS));
 	strArr.Add(CConvert::ToString(IDC_VIEW_JOBLIST_COMBO_WHTYPE));
 	strArr.Add(CConvert::ToString(IDC_LBL_LUGGNUM));
 	strArr.Add(CConvert::ToString(IDC_VIEW_JOBLIST_EDIT_LUGGNUM));
 	strArr.Add(CConvert::ToString(IDC_VIEW_JOBLIST_COMBO_STARTPOS));
 	strArr.Add(CConvert::ToString(IDC_LBL_DESTPOS));
 	strArr.Add(CConvert::ToString(IDC_VIEW_JOBLIST_COMBO_DESTPOS));
 	strArr.Add(CConvert::ToString(IDC_LBL_JOBTYP));
 	strArr.Add(CConvert::ToString(IDC_VIEW_JOBLIST_EDIT_JOB_TYP));
 	strArr.Add(CConvert::ToString(IDC_LBL_JOB_STATUS));
 	strArr.Add(CConvert::ToString(IDC_VIEW_JOBLIST_EDIT_JOBSTATUS));
 	strArr.Add(CConvert::ToString(IDC_LBL_JOB_STATUS2));
 	strArr.Add(CConvert::ToString(IDC_VIEW_JOBLIST_EDIT_JOBSTATUS2));

}

void CViewJobListDlg::ReSizing(int nID)
{
	CWnd* pCtl = GetDlgItem(nID);
	if (!pCtl) { return; }
	CRect rectCtl;
	pCtl->GetWindowRect(&rectCtl);
	ScreenToClient(&rectCtl);

	double dLeft, dTop, dWidth, dHeight, dMaxX, dMaxY;
	dMaxX = m_pDoc->m_pMaxSizeX;
	dMaxY = m_pDoc->m_pMaxSizeY;

	dLeft = rectCtl.left * dMaxX;
	dTop = rectCtl.top * dMaxY;
	dWidth = rectCtl.Width() * dMaxX;
	dHeight = rectCtl.Height() * dMaxY;
	pCtl->MoveWindow(dLeft, dTop, dWidth, dHeight, TRUE);

}

void CViewJobListDlg::CopyJob()
{
	if (!m_pDoc->Permission(_T("CViewJobListDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	variant_t val;
	CString strSql;
	CString strWH_TYP, strLUGG_NO, strJOB_TYP, strSTART_POS, strDEST_POS, strPRODUCT_SIZE;

	if (m_nActiveRow < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("스프레드를 클릭하시오"))); 
		return;
	}
	
	//m_pSpreadMain.GetText(1, m_nActiveRow, &val);
	strWH_TYP = m_SpreadSheet.GetValueTXT(1, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	//m_pSpreadMain.GetText(2, m_nActiveRow, &val);
	strLUGG_NO = m_SpreadSheet.GetValueTXT(2, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	//m_pSpreadMain.GetText(3, m_nActiveRow, &val);
	strSTART_POS = m_SpreadSheet.GetValueTXT(3, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	//m_pSpreadMain.GetText(5, m_nActiveRow, &val);
	strDEST_POS = m_SpreadSheet.GetValueTXT(5, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	//m_pSpreadMain.GetText(8, m_nActiveRow, &val);
	strJOB_TYP = m_SpreadSheet.GetValueTXT(7, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	// [LGLS 2026-08-01] PULP 단수 컬럼이 그리드에서 빠져 값을 읽을 수 없으므로 원본 작업에서 직접 읽는다.
	{
		CString strSqlPs; int nPsCnt = -1; CString strPsMsg;
		strSqlPs.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(PRODUCT_SIZE,' ') AS PRODUCT_SIZE FROM JOB_MST WHERE WH_TYP = '%s' AND LUGG_NO = '%s' "),
		                m_pDoc->m_WH_TYP, m_SpreadSheet.GetValueTXT(2, m_nActiveRow));
		_RecordsetPtr pPs = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSqlPs, nPsCnt, strPsMsg);
		if (nPsCnt > 0)
		{
			CRecordSetWrap* pPsw = new CRecordSetWrap(pPs);
			pPsw->MoveFirst();
			strPRODUCT_SIZE = pPsw->GetItem(_T("PRODUCT_SIZE"));
			delete pPsw;
		}
	}

	strWH_TYP.Trim();
	strLUGG_NO.Trim();
	strDEST_POS.Trim();
	strJOB_TYP.Trim();
	strPRODUCT_SIZE.Trim();
	strSTART_POS.Trim();


	CString strSpace = _T(" ");
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업을 복사 하시겠습니까?")) + strSpace + _T("[LUGG_NO : ") + strLUGG_NO + _T(" ]"), MB_YESNO) != IDYES) 
		return;

	UpdateData(TRUE);

	CString strLOG_LUGG_NO = strLUGG_NO;
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
	CString strLOG_MSG = _T("JOB_MST COPY");
	if (!m_pDoc->GetQueryInsertClientLog(_T("CViewJobListDlg"), strLOG_LUGG_NO, _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}


	strWH_TYP = m_cmbWhTyp.GetItemCCD(strWH_TYP);
	strJOB_TYP = m_cmbJobTyp.GetItemCCD(strJOB_TYP);
	// [LGLS] PRODUCT_SIZE 는 DB 원본 코드값 그대로 사용(그리드 코드명 변환 불필요)



	CJOB_MST* pCopyJob = m_pDoc->m_pJob->GetCopyJob();
	pCopyJob->WH_TYP = strWH_TYP;
	pCopyJob->LUGG_NO = strLUGG_NO;
	pCopyJob->DEST_POS = strDEST_POS;
	pCopyJob->JOB_TYP = strJOB_TYP;
	pCopyJob->PRODUCT_SIZE = strPRODUCT_SIZE;

	pCopyJob->COPY_YN = true;
	
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
	InitializeSpread(TRUE);
}

// [LGLS 2026-08-01] 런타임 생성 조회 라벨은 대화상자 배경이 비치도록 투명 처리한다.
//   (rc 라벨은 GetDlgItem 이 임시 CWnd 를 주어 기본 처리로 잘 그려지지만,
//    직접 만든 라벨은 기본 브러시로 칠해져 네모 배경이 보인다.)
HBRUSH CViewJobListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	int nId = pWnd->GetDlgCtrlID();
	if (nId == IDC_LGLS_JL_LOT_LBL || nId == IDC_LGLS_JL_PRD_LBL)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return CSkinDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
