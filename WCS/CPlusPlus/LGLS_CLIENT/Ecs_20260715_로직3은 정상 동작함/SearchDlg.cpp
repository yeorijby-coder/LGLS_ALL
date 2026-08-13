// SearchDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "SearchDlg.h"
#include "afxdialogex.h"
#include "EcsView.h"

// CSearchDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSearchDlg, CDialogResize)

CSearchDlg::CSearchDlg(CEcsView* pView, CWnd* pParent /*=NULL*/)
	: CDialogResize(CSearchDlg::IDD, pParent)
{
	m_pView = pView;
}

CSearchDlg::~CSearchDlg()
{
	if(m_pDlgFont!= NULL) delete m_pDlgFont;
	//this->DestroyWindow();
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_LUGGNUM_SEARCH, m_chxLuggSearch);
	DDX_Control(pDX, IDC_CHECK_STATION_SEARCH, m_chxStationSearch);
	DDX_Control(pDX, IDC_CHECK_TRACKNUM_SEARCH, m_chxTrackSearch);
	DDX_Control(pDX, IDC_CHECK_TRAYID_SEARCH, m_chxTraySearch);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialogResize)
	ON_BN_CLICKED(IDC_CHECK_LUGGNUM_SEARCH, &CSearchDlg::OnCheckLuggnumSearch)
	ON_BN_CLICKED(IDC_CHECK_TRAYID_SEARCH, &CSearchDlg::OnCheckTrayidSearch)
	ON_BN_CLICKED(IDC_CHECK_STATION_SEARCH, &CSearchDlg::OnCheckStationSearch)
	ON_BN_CLICKED(IDC_BUTTON_SIZE, &CSearchDlg::OnBnClickedButtonSize)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_CHECK_TRACKNUM_SEARCH, &CSearchDlg::OnCheckTrackSearch)
END_MESSAGE_MAP()


// CSearchDlg 메시지 처리기입니다.


BOOL CSearchDlg::OnInitDialog()
{
	CDialogResize::OnInitDialog();

	SetWindowText(_T("Search..."));

	
	m_pDlgFont = new CFont(); 
	m_pDlgFont->CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
	, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("MS Shell Dlg")); 

	//폰트변경 메시지 보내기
	SendMessageToDescendants(WM_SETFONT, (WPARAM)m_pDlgFont->GetSafeHandle(), 1, TRUE, FALSE); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}





void CSearchDlg::OnCheckLuggnumSearch()
{
	if(m_pView->m_bSearchFlag == FALSE)
	{
		m_chxTraySearch.SetCheck(0);
		m_chxTrackSearch.SetCheck(0);
		m_chxStationSearch.SetCheck(0);
	
		m_pView->m_nSearchType = enLuggSearch;
	}
	else
	{
		AfxMessageBox(_T("검색이 진행중입니다."));	
		m_chxLuggSearch.SetCheck(0);
	}
}


void CSearchDlg::OnCheckTrayidSearch()
{
	if(m_pView->m_bSearchFlag == FALSE)
	{
		m_chxLuggSearch.SetCheck(0);
		m_chxTrackSearch.SetCheck(0);
		m_chxStationSearch.SetCheck(0);

		m_pView->m_nSearchType = enTraySearch;
	}
	else
	{
		AfxMessageBox(_T("검색이 진행중입니다."));	
		m_chxTraySearch.SetCheck(0);
	}
}


void CSearchDlg::OnCheckStationSearch()
{
	if(m_pView->m_bSearchFlag == FALSE)
	{
		m_chxLuggSearch.SetCheck(0);
		m_chxTraySearch.SetCheck(0);
		m_chxTrackSearch.SetCheck(0);

		m_pView->m_nSearchType = enStationSearch;
	}
	else
	{
		AfxMessageBox(_T("검색이 진행중입니다."));
		m_chxStationSearch.SetCheck(0);
	}
}

void CSearchDlg::OnCheckTrackSearch()
{

	if(m_pView->m_bSearchFlag == FALSE)
	{
		m_chxLuggSearch.SetCheck(0);
		m_chxTraySearch.SetCheck(0);
		m_chxStationSearch.SetCheck(0);

		m_pView->m_nSearchType = enTrackNumSearch;
	}
	else
	{
		AfxMessageBox(_T("검색이 진행중입니다."));
		m_chxStationSearch.SetCheck(0);
	}
}


void CSearchDlg::OnBnClickedButtonSize()
{
	InitResizing(FALSE);

	this->SendMessage( WM_SYSCOMMAND, SC_MINIMIZE );

	SetWindowPos(NULL, 0, 90, 0, 0, SWP_NOSIZE);
	SetWindowText(_T("Search..."));

	this->UpdateData();
}


void CSearchDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );

	CDialogResize::OnLButtonDown(nFlags, point);
}


LRESULT CSearchDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	   UINT hit = CDialogResize::OnNcHitTest(point);
	if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
//	return CDialogResize::OnNcHitTest(point);
}


