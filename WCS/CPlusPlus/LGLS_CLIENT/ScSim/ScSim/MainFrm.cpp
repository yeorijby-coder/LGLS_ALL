// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Ecs.h"

#include "MainFrm.h"
//#include "Splash.h"

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
	ON_WM_GETMINMAXINFO()
	ON_WM_KEYDOWN()
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
/*	
	if (!m_wndToolBar[0].CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar[0].LoadToolBar(IDR_TOOLBAR3))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
//	m_wndToolBar[0].InitLogo(ID_MAKER_LOGO, "LogoMaker.bmp");
//	m_wndToolBar[0].InitLogo(ID_USER_LOGO, "footer_logo.bmp");
/*
	if (!m_wndToolBar[1].CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar[1].LoadToolBar(IDR_TOOLBAR4))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
//	m_wndToolBar[1].InitLogo(ID_USER_LOGO, "LogoUser.bmp");
//	m_wndToolBar[1].InitLogo(ID_USER_LOGO, "LogoSLI.bmp");
/*
	if (!m_wndToolBar[2].CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar[2].LoadToolBar(IDR_TOOLBAR5))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
//	m_wndToolBar[2].InitLogo(ID_USER_LOGO, "LogoMaker.bmp");

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
//	m_wndToolBar[0].EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar[1].EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar[2].EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar[0]);
//	DockControlBar(&m_wndToolBar[1]);
//	DockControlBar(&m_wndToolBar[2]);

//	m_wndToolBar[1].ShowWindow(SW_HIDE);
//	m_wndToolBar[2].ShowWindow(SW_HIDE);

	// CG: The following line was added by the Splash Screen component.
//	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 768;
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx = 1024;
	cs.cy = 768;

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
	case COMM_STA_SEND:			return 3;
	case COMM_STA_RECEIVE:		return 4;
	case COMM_STA_STOP:			return 5;
	default:					return 5;
	}
}
