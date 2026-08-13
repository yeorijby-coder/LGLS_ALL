// LogoBandWnd.cpp : implementation file
//

#include "stdafx.h"
#include "LogoBandWnd.h"


///////////////////////////////////////////////
// CLogoBandWnd

CLogoBandWnd::CLogoBandWnd()
{
}

CLogoBandWnd::~CLogoBandWnd()
{
}


BEGIN_MESSAGE_MAP(CLogoBandWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


///////////////////////////////////////////////
// CLogoBandWnd message handlers

BOOL CLogoBandWnd::Create(CString strFile, CRect rectWnd, CWnd *pParentWnd)
{
	if ( strFile.IsEmpty() || pParentWnd == NULL )
		return FALSE;

	CFile fileLogo;
	if ( !fileLogo.Open(strFile, CFile::modeRead) )
		return FALSE;

	if ( !m_dibLogo.Read(&fileLogo) )
		return FALSE;
	
	return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_DLGFRAME, rectWnd, pParentWnd, 0x1000);
}

void CLogoBandWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);

	m_dibLogo.Draw(&dc, CPoint(0,0), CSize(rect.Width(), rect.Height()));
}

BOOL CLogoBandWnd::OnEraseBkgnd(CDC* pDC)
{
	CWnd* pParentWnd = GetParent();
	HBRUSH hBrush = (HBRUSH)pParentWnd->SendMessage(WM_CTLCOLORDLG, (WPARAM)pDC->m_hDC, (LPARAM)pParentWnd->m_hWnd);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, CBrush::FromHandle(hBrush));

	return TRUE;
}
