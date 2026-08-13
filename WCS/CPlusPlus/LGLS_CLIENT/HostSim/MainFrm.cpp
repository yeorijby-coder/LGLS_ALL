// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Ecs.h"

#include "MainFrm.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar[0].CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOAT_MULTI) ||
		!m_wndToolBar[0].LoadToolBar(IDR_TOOLBAR0))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
//	m_wndToolBar[0].InitLogo(ID_MAKER_LOGO, "CargillLogo02.bmp");

	if (!m_wndToolBar[1].CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOAT_MULTI) ||
		!m_wndToolBar[1].LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// 두번째 툴바는 로고를 표현하지 않도록 요청함! - LC CNS
//	m_wndToolBar[1].InitLogo(ID_USER_LOGO, "footer_logo.bmp");

	if (!m_wndToolBar[2].CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		 | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOAT_MULTI) ||
		!m_wndToolBar[2].LoadToolBar(IDR_TOOLBAR2))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar[2].InitLogo(ID_MAKER_LOGO, "CargillLogo02.bmp");			//CargillLogo02

	if (!m_wndToolBar[3].CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOAT_MULTI) ||
		!m_wndToolBar[3].LoadToolBar(IDR_TOOLBAR3))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar[0].EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar[1].EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar[2].EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar[3].EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar[0]);			// 기본툴바
	DockControlBar(&m_wndToolBar[1]);			// 통신툴바
	DockControlBar(&m_wndToolBar[2]);			// 로고&닫기
	DockControlBar(&m_wndToolBar[3]);			// 로고&닫기
	DockControlBarLeftOf(&m_wndToolBar[1], &m_wndToolBar[0]); // 기본툴바 + 통신툴바
	DockControlBarLeftOf(&m_wndToolBar[3], &m_wndToolBar[1]); // 기본툴바 + 통신툴바
	DockControlBarRight(&m_wndToolBar[2], &m_wndToolBar[1]);	// 통신툴바 + 알파 + 로고&닫기

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

void CMainFrame::DockControlBarRight(CControlBar* pBar, CControlBar* pOnLeft)
{
    CRect rect;
	CRect rectWindows;

    RecalcLayout(TRUE);
    pOnLeft->GetWindowRect(&rect);		// 통신툴바

	GetWindowRect(rectWindows);			// 윈도우 크기 

	int nRight =  rectWindows.right;	// 전체 윈도우에서 Right 값을 가져온다.

	rect.left = nRight;
//	rect.right = nRight;
	
    UINT n = 0;
    DockControlBar(pBar, n, &rect);	
}

void CMainFrame::DockControlBarLeftOf(CControlBar* pBar, CControlBar* pLeftOf)
{
    CRect rect;
    // get MFC to adjust the dimensions of all docked ToolBars
    // so that GetWindowRect will be accurate
    RecalcLayout(TRUE);
    pLeftOf->GetWindowRect(&rect);
    rect.OffsetRect(1,0);
	
    DWORD dw = pLeftOf->GetBarStyle();
    UINT n = 0;
    n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
    n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
    n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
    n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;


    // When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. By calculating a rectangle, we
    // are simulating a Toolbar being dragged to that location and docked.
	
    DockControlBar(pBar, n, &rect);	
}

void CMainFrame::DockControlBarRightOf(CControlBar* pBar, CControlBar* pLeftOf)
{
    CRect rect;
    // get MFC to adjust the dimensions of all docked ToolBars
    // so that GetWindowRect will be accurate
    RecalcLayout(TRUE);
    pLeftOf->GetWindowRect(&rect);
    rect.OffsetRect(1,0);
	
    DWORD dw = pLeftOf->GetBarStyle();
    UINT n = 0;
    
	if (dw&CBRS_ALIGN_TOP) 
		n = AFX_IDW_DOCKBAR_RIGHT;
//	else
//		n = n;
    n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
    n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
    n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;


    // When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. By calculating a rectangle, we
    // are simulating a Toolbar being dragged to that location and docked.
	
    DockControlBar(pBar, n, &rect);	
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMinTrackSize.x = 1280;		//1024;
	lpMMI->ptMinTrackSize.y = 1024;		//768;
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//	cs.cx = 1024;
//	cs.cy = 768;
	cs.cx = 1280;
	cs.cy = 1024;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::DisplayMessage(CString strMsg)
{
	m_wndStatusBar.SetWindowText(strMsg);
}

void CMainFrame::UpdateCommStatus(int nIndex, UINT unID, char Status)
{
	if ((nIndex < 0) || (nIndex > 2))
	{
		ASSERT(FALSE);
		return;
	}

	m_wndToolBar[nIndex].SetButtonInfo(m_wndToolBar[nIndex].CommandToIndex(unID), unID, TBBS_BUTTON, GetIndex(Status));
}

int CMainFrame::GetIndex(char Status) 
{
	switch (Status)
	{
	case COMM_STA_SEND:			return 2;
	case COMM_STA_RECEIVE:		return 3;
	case COMM_STA_STOP:			return 4;
	default:					return 4;
	}
}
