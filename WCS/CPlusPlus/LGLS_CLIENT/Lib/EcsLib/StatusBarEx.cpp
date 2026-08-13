// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2004 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com

// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.o
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

// ExtStatusControlBar.cpp : implementation file
//

#include "stdafx.h"
#include "StatusBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////
// CStatusBarEx

CStatusBarEx::CStatusBarEx()
{
}

CStatusBarEx::~CStatusBarEx()
{
	for ( int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		if( m_arrPaneControls[i]->hWnd && ::IsWindow(m_arrPaneControls[i]->hWnd) )
		{
			::ShowWindow(m_arrPaneControls[i]->hWnd, SW_HIDE); 
			if( m_arrPaneControls[i]->bAutoDestroy )
			{
				::DestroyWindow(m_arrPaneControls[i]->hWnd);
			}
		}

		_STATUSBAR_PANE_CTRL_ *pPaneCtrl = m_arrPaneControls[i];
		if( pPaneCtrl )
			delete pPaneCtrl;
	}
}

BEGIN_MESSAGE_MAP(CStatusBarEx, CStatusBar)
	//{{AFX_MSG_MAP(CStatusBarEx)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////
// CStatusBarEx message handlers
int CStatusBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CStatusBar::OnCreate(lpCreateStruct) == -1 )
		return -1;
	
	return 0;
}

LRESULT CStatusBarEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult =CStatusBar::WindowProc(message, wParam, lParam);
	if( message == WM_SIZE ){
		RepositionControls();
	}
	return lResult;
}

//////////////////////////////////////////
//
int CStatusBarEx::GetPanesCount() const
{
	return m_nCount;
}

void CStatusBarEx::SetPaneWidth(int nIndex, int nWidth)
{
	_STATUSBAR_PANE_ pane;
	PaneInfoGet(nIndex, &pane);
	pane.cxText = nWidth;
	PaneInfoSet(nIndex, &pane);
}

void CStatusBarEx::SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth)
{
	CStatusBar::SetPaneInfo(nIndex, nID, nStyle, cxWidth);
	BOOL bDisabled = ((nStyle&SBPS_DISABLED) == 0);
	DisableControl(nIndex, bDisabled);
}

void CStatusBarEx::SetPaneStyle(int nIndex, UINT nStyle)
{
	CStatusBar::SetPaneStyle(nIndex, nStyle);
	BOOL bDisabled = ((nStyle&SBPS_DISABLED) == 0);
	DisableControl(nIndex, bDisabled);
}

void CStatusBarEx::SetPaneInfo(UINT nID, CString strText, COLORREF colorBack, COLORREF colorText)
{
	for (int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		if ( nID == m_arrPaneControls[i]->nID)
		{
			CExtStatusBarLabel* pWnd = dynamic_cast<CExtStatusBarLabel*>(m_arrPaneControls[i]->pWnd);
			if ( pWnd )
			{
				HWND hWnd = m_arrPaneControls[i]->hWnd;
				::SetWindowText(hWnd, strText);

				pWnd->SetBackColor(colorBack);
				pWnd->SetTextColor(colorText);

				::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
				return;
			}

			CExtStatusBarButton* pWndB = dynamic_cast<CExtStatusBarButton*>((CExtStatusBarButton*)m_arrPaneControls[i]->pWnd);
			{
				pWndB->SetBkColor(colorBack);
			}
		}
	}
}

void CStatusBarEx::SetPaneInfo(UINT nID, int nFontSize, CString strFontName, COLORREF colorText,
							   COLORREF color1, COLORREF color2, int nFillMode, int nGradentPer, int nGradentType)
{
	for (int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		if ( nID == m_arrPaneControls[i]->nID)
		{
			HWND hWnd = m_arrPaneControls[i]->hWnd;
			CExtStatusBarButton* pWnd = dynamic_cast<CExtStatusBarButton*>(m_arrPaneControls[i]->pWnd);
			if ( pWnd == NULL )
				return;

			pWnd->SetTextSize( nFontSize, FALSE);
			pWnd->SetFontName(strFontName);
			pWnd->SetBkColor(color1, color2, nFillMode, nGradentPer);	// 배경 색상
			pWnd->m_Gradation_Type = nGradentType;
			pWnd->m_textColor = colorText;

			::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}
}

void CStatusBarEx::SetPaneText(UINT nID, CString str)
{
	for (int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		if ( nID == m_arrPaneControls[i]->nID)
		{
			HWND hWnd = m_arrPaneControls[i]->hWnd;
			::SetWindowText(hWnd, str);
			::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}
}

void CStatusBarEx::SetBackColor(UINT nID, COLORREF color)
{
	for (int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		if ( nID == m_arrPaneControls[i]->nID)
		{
			CExtStatusBarLabel* pWndA = dynamic_cast<CExtStatusBarLabel*>((CExtStatusBarLabel*)m_arrPaneControls[i]->pWnd);
			if ( pWndA )
			{
				HWND hWnd = m_arrPaneControls[i]->hWnd;
				pWndA->SetBackColor(color);
				::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
				return;
			}
		}
	}
}

void CStatusBarEx::SetTextColor(UINT nID, COLORREF color)
{
	for (int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		if ( nID == m_arrPaneControls[i]->nID)
		{
			CExtStatusBarLabel* pWnd = dynamic_cast<CExtStatusBarLabel*>(m_arrPaneControls[i]->pWnd);
			if ( pWnd == NULL )
				return;

			HWND hWnd = m_arrPaneControls[i]->hWnd;
			pWnd->SetTextColor(color);
			::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}
}

//////////////////////////////////////////
//
BOOL CStatusBarEx::AddPane(UINT nID, int nIndex)	// ID, index of the pane
{
	if (nIndex < 0 || nIndex > m_nCount)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if (CommandToIndex(nID) != -1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	CArray<_STATUSBAR_PANE_*,_STATUSBAR_PANE_*> arrPanesTmp;
	int iIndex = 0;
	for (iIndex = 0; iIndex < m_nCount+1; iIndex++)
	{
		_STATUSBAR_PANE_* pNewPane = new _STATUSBAR_PANE_;
		
		if (iIndex == nIndex)
		{
			pNewPane->nID    = nID;
			pNewPane->nStyle = SBPS_NORMAL;
		}
		else
		{
			int idx = iIndex;
			if (iIndex > nIndex) idx--;
			
			_STATUSBAR_PANE_* pOldPane  = GetPanePtr(idx);
			pNewPane->cxText  = pOldPane->cxText;
			pNewPane->nFlags  = pOldPane->nFlags;
			pNewPane->nID     = pOldPane->nID;
			pNewPane->nStyle  = pOldPane->nStyle;
			pNewPane->strText = pOldPane->strText;
		}
		arrPanesTmp.Add(pNewPane);
	}
	
	int nPanesCount = arrPanesTmp.GetSize();
	UINT* lpIDArray = new UINT[ nPanesCount ];
	for (iIndex = 0; iIndex < nPanesCount; iIndex++) 
	{
		lpIDArray[iIndex] = arrPanesTmp[iIndex]->nID;
	}
	
	// set the indicators 
	SetIndicators(lpIDArray, nPanesCount);
	// free memory
	for (iIndex = 0; iIndex < nPanesCount; iIndex++)
	{
		_STATUSBAR_PANE_* pPane = arrPanesTmp[iIndex];
//		if (iIndex != nIndex)
			PaneInfoSet(iIndex, pPane);
		if(pPane) 
			delete pPane;
	}
	
	arrPanesTmp.RemoveAll();
	if(lpIDArray) 
		delete []lpIDArray;
	
	RepositionControls();
	
	return TRUE;
}

BOOL CStatusBarEx::RemovePane(UINT nID)  	// ID of the pane
{
	if ( CommandToIndex(nID) == -1 || m_nCount == 1 )
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	CArray<_STATUSBAR_PANE_*,_STATUSBAR_PANE_*> arrPanesTmp;
	int nIndex;
	for (nIndex = 0; nIndex < m_nCount; nIndex++)
	{
		_STATUSBAR_PANE_* pOldPane = GetPanePtr(nIndex);
		
		if (pOldPane->nID == nID)
			continue;
		
		_STATUSBAR_PANE_* pNewPane = new _STATUSBAR_PANE_;
		
		pNewPane->cxText  = pOldPane->cxText;
		pNewPane->nFlags  = pOldPane->nFlags;
		pNewPane->nID     = pOldPane->nID;
		pNewPane->nStyle  = pOldPane->nStyle;
		pNewPane->strText = pOldPane->strText;
		arrPanesTmp.Add(pNewPane);
	}
	
	UINT* lpIDArray = new UINT[arrPanesTmp.GetSize()];
	for (nIndex = 0; nIndex < arrPanesTmp.GetSize(); nIndex++)
	{
		lpIDArray[nIndex] = arrPanesTmp[nIndex]->nID;
	}
	
	// set the indicators
	SetIndicators(lpIDArray, arrPanesTmp.GetSize());
	// free memory
	for (nIndex = 0; nIndex < arrPanesTmp.GetSize(); nIndex++)
	{
		_STATUSBAR_PANE_* pPane = arrPanesTmp[nIndex];
		PaneInfoSet(nIndex, pPane);
		if(pPane) 
			delete pPane;
	}
	
	for ( int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		if (m_arrPaneControls[i]->nID == nID){
			if( m_arrPaneControls[i]->hWnd && ::IsWindow(m_arrPaneControls[i]->hWnd) )
			{
				::ShowWindow(m_arrPaneControls[i]->hWnd, SW_HIDE); 
				if( m_arrPaneControls[i]->bAutoDestroy ) 
				{
					::DestroyWindow(m_arrPaneControls[i]->hWnd);
				}
			}
			_STATUSBAR_PANE_CTRL_ *pPaneCtrl = m_arrPaneControls[i];
			if( pPaneCtrl )
				delete pPaneCtrl;
			m_arrPaneControls.RemoveAt(i);
			break;
		}
	}

	arrPanesTmp.RemoveAll();
	if(lpIDArray) 
		delete []lpIDArray;
	
	RepositionControls();
	
	return TRUE;
}

BOOL CStatusBarEx::AddPaneControl(CWnd* pWnd, UINT nID, BOOL bAutoDestroy)
{
	return AddPaneControl( pWnd->GetSafeHwnd(), nID, bAutoDestroy, pWnd);
}

BOOL CStatusBarEx::AddPaneControl(HWND hWnd, UINT nID, BOOL bAutoDestroy, CWnd* pWnd)
{
	if (CommandToIndex (nID) == -1) {
		return FALSE;
	}
	
	_STATUSBAR_PANE_CTRL_* pPaneCtrl = new _STATUSBAR_PANE_CTRL_;
	pPaneCtrl->pWnd      = pWnd;
	pPaneCtrl->nID         = nID;
	pPaneCtrl->hWnd        = hWnd;
	pPaneCtrl->bAutoDestroy = bAutoDestroy;
	
	m_arrPaneControls.Add(pPaneCtrl);

	RepositionControls();
	return TRUE;
}

void CStatusBarEx::DisableControl( int nIndex, BOOL bDisable)
{
	UINT uItemID = GetItemID(nIndex);
	for ( int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		if( uItemID == m_arrPaneControls[i]->nID )
		{
			if( m_arrPaneControls[i]->hWnd && ::IsWindow(m_arrPaneControls[i]->hWnd) )
			{
				::EnableWindow(m_arrPaneControls[i]->hWnd, bDisable); 
			}
		}
	}
}

//////////////////////////////////////////
//
void CStatusBarEx::RepositionControls()
{
	HDWP _hDWP = ::BeginDeferWindowPos( m_arrPaneControls.GetSize() );
	
	CRect rcClient;
	GetClientRect(&rcClient);
	for (int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		int   iIndex  = CommandToIndex(m_arrPaneControls[i]->nID);
		HWND hWnd    = m_arrPaneControls[i]->hWnd;
		
		CRect rcPane;
		GetItemRect(iIndex, &rcPane);
		
		// CStatusBar::GetItemRect() sometimes returns invalid size 
		// of the last pane - we will re-compute it
		int cx = 1;//::GetSystemMetrics( SM_CXEDGE );
		DWORD dwPaneStyle = GetPaneStyle( iIndex );
		if( iIndex == (m_nCount-1) )
		{
			if( (dwPaneStyle & SBPS_STRETCH ) == 0 )
			{
				UINT nID, nStyle;
				int  cxWidth;
				GetPaneInfo( iIndex, nID, nStyle, cxWidth );
				rcPane.right = rcPane.left + cxWidth + cx*3;
			} // if( (dwPaneStyle & SBPS_STRETCH ) == 0 )
			else
			{
				CRect rcClient;
				GetClientRect( &rcClient );
				rcPane.right = rcClient.right;
				if( (GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP )
				{
					int cxSmIcon = ::GetSystemMetrics( SM_CXSMICON );
					rcPane.right -= cxSmIcon + cx;
				} // if( (GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP )
			} // else from if( (dwPaneStyle & SBPS_STRETCH ) == 0 )
		} // if( iIndex == (m_nCount-1) )
		
		if ((GetPaneStyle (iIndex) & SBPS_NOBORDERS) == 0)
			rcPane.DeflateRect(cx,cx);
		else
			rcPane.DeflateRect(cx,1,cx,1);
		
		if (hWnd && ::IsWindow(hWnd))
		{
			_hDWP = ::DeferWindowPos(
				_hDWP, 
				hWnd, 
				NULL, 
				rcPane.left,
				rcPane.top, 
				rcPane.Width(), 
				rcPane.Height(),
				SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_SHOWWINDOW
				);

			::RedrawWindow(
				hWnd,
				NULL,
				NULL,
				RDW_INVALIDATE|RDW_UPDATENOW
				|RDW_ERASE|RDW_ERASENOW
				);
			
		} // if (hWnd && ::IsWindow(hWnd)){ 

	}
	
	VERIFY( ::EndDeferWindowPos( _hDWP ) );
}

//////////////////////////////////////////
//
BOOL CStatusBarEx::PaneInfoGet(int nIndex, _STATUSBAR_PANE_* pPane)
{
	if( nIndex < m_nCount  && nIndex >= 0 )
	{
		GetPaneInfo( nIndex,  pPane->nID, pPane->nStyle, pPane->cxText );
		CString strPaneText;
		GetPaneText( nIndex , strPaneText );
		pPane->strText = LPCTSTR(strPaneText);
		return TRUE;
	}
	return FALSE;
}

BOOL CStatusBarEx::PaneInfoSet(int nIndex, _STATUSBAR_PANE_* pPane)
{
	if( nIndex < m_nCount  && nIndex >= 0 ){
		SetPaneInfo( nIndex, pPane->nID, pPane->nStyle, pPane->cxText );
		SetPaneText( nIndex, LPCTSTR( pPane->strText) );
		return TRUE;
	}
	return FALSE;
}
