// ToolBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////
// CToolBarEx

CToolBarEx::CToolBarEx()
{
	m_nTimerID = 0;
}

CToolBarEx::~CToolBarEx()
{
}


BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
	//{{AFX_MSG_MAP(CToolBarEx)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////
// CToolBarEx message handlers

int CToolBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CToolBarEx::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CToolBar::OnTimer(nIDEvent);
}

void CToolBarEx::InitLogo(int nLogoIndex, CString strLogoFile)
{
	#define ID_TIMER_LOGO 100
    #define SNAP_WIDTH    100

	CRect rect;
    SetButtonInfo(CommandToIndex(nLogoIndex), nLogoIndex, TBBS_SEPARATOR, SNAP_WIDTH);
    GetItemRect(CommandToIndex(nLogoIndex), &rect);

	CString strFile = AfxGetApp()->GetProfileString(_T("Config"), _T("MenuLogoFile"), _T(""));
	if ( MakeSureFileExists(strFile) )
	{
		m_wndLogoBand.Create(strFile, rect, this);
		m_wndLogoBand.ShowWindow(SW_SHOW);
	}
	else
	{
		char szPath[MAX_PATH];
		GetCurrentDirectory(sizeof(szPath), CA2CT(szPath));
		strFile = CString(szPath) + _T("\\") + strLogoFile;

		if ( MakeSureFileExists(strFile) )
		{
			m_wndLogoBand.Create(strFile, rect, this);
			m_wndLogoBand.ShowWindow(SW_SHOW);
		}
	}


//	m_nTimerID = SetTimer(ID_TIMER_LOGO, 1000, NULL);
}

void CToolBarEx::OnNcDestroy() 
{
	KillTimer(m_nTimerID);
	CToolBar::OnNcDestroy();
}

BOOL CToolBarEx::MakeSureFileExists(CString strFile)
{
	if ( strFile.IsEmpty() )		return FALSE;

	if( strFile.Right(1) == _T("\\") )
		return FALSE;

	if( GetFileAttributes(strFile) == 0xFFFFFFFF )
		return FALSE;

	return TRUE;
}

