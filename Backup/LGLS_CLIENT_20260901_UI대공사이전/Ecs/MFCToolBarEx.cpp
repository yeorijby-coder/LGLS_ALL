// MFCToolBarEx.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCToolBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CMFCToolBarEx
//
CMFCToolBarEx::CMFCToolBarEx()
{
	m_nTimerID = 0;
}

CMFCToolBarEx::~CMFCToolBarEx()
{
}


BEGIN_MESSAGE_MAP(CMFCToolBarEx, CMFCToolBar)
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMFCToolBarEx 메시지 처리기입니다.
//
int CMFCToolBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void CMFCToolBarEx::OnNcDestroy()
{
	CMFCToolBar::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMFCToolBarEx::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CMFCToolBar::OnTimer(nIDEvent);
}

void CMFCToolBarEx::InitLogo(int nLogoIndex, CString strLogoFile)
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

	m_nTimerID = SetTimer(ID_TIMER_LOGO, 1000, NULL);
}

BOOL CMFCToolBarEx::MakeSureFileExists(CString strFile)
{
	if ( strFile.IsEmpty() )		return FALSE;

	if ( strFile.Right(1) == _T("\\") )
		return FALSE;

	if ( GetFileAttributes(strFile) == 0xFFFFFFFF )
		return FALSE;

	return TRUE;
}
