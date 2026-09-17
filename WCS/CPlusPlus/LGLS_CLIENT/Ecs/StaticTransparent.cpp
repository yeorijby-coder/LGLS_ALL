// StaticTransparent.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StaticTransparent.h"


// CStaticTransparent

IMPLEMENT_DYNAMIC(CStaticTransparent, CStatic)

CStaticTransparent::CStaticTransparent()
{
	m_bDraging = FALSE;
}

CStaticTransparent::~CStaticTransparent()
{
}


BEGIN_MESSAGE_MAP(CStaticTransparent, CStatic)
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_ENTERSIZEMOVE()
	ON_WM_EXITSIZEMOVE()
END_MESSAGE_MAP()



LRESULT CStaticTransparent::OnSetText(WPARAM wParam,LPARAM lParam)
{
	LRESULT Result = Default();
	Invalidate();
	UpdateWindow();
	return Result;
}

HBRUSH CStaticTransparent::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	//pDC->SetBkMode(TRANSPARENT);
	//return (HBRUSH)GetStockObject(NULL_BRUSH);
	return NULL;
}

BOOL CStaticTransparent::OnEraseBkgnd(CDC* pDC)
{

	return TRUE;
}



// CStaticTransparent 메시지 처리기입니다.




void CStaticTransparent::OnEnterSizeMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//if( !m_bDraging )
	//{
	//	m_bDraging = TRUE;
	//	::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
	//	::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	//}
}


void CStaticTransparent::OnExitSizeMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//if( m_bDraging )
//{
//	m_bDraging = FALSE;
//	::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
//	::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
//}
//CStatic::OnExitSizeMove();
}
