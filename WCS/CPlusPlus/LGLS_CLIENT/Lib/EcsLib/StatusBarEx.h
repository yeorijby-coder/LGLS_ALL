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
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


#pragma once

#include "ExtLabel.h"
#include "MinButton.h"
#include "afxtempl.h"

class AFX_EXT_CLASS CExtStatusBarButton : public CMinButton
{
	virtual void PostNcDestroy()
	{
		delete this;
	}
};

class AFX_EXT_CLASS CExtStatusBarLabel : public CExtLabel
{
	virtual void PostNcDestroy()
	{
		delete this;
	}
};

class AFX_EXT_CLASS CStatusBarEx : public CStatusBar
{
public:
	CStatusBarEx();
	virtual ~CStatusBarEx();

public:
	struct _STATUSBAR_PANE_
	{
		_STATUSBAR_PANE_(){	nID = cxText = nStyle = nFlags = 0;	}
		
		UINT    nID;        // IDC of indicator: 0 => normal text area
		int     cxText;     // width of string area in pixels
							//   on both sides there is a 3 pixel gap and
							//   a one pixel border, making a pane 6 pixels wider
		UINT    nStyle;     // style flags (SBPS_*)
		UINT    nFlags;     // state flags (SBPF_*)
		CString strText;    // text in the pane
	};

	struct _STATUSBAR_PANE_CTRL_
	{
		CWnd* pWnd;
		HWND hWnd;
		UINT nID;
		BOOL bAutoDestroy;		
	};
	
	CArray < _STATUSBAR_PANE_CTRL_*, _STATUSBAR_PANE_CTRL_* > m_arrPaneControls;

	_STATUSBAR_PANE_* GetPanePtr(int nIndex) const
	{
		ASSERT((nIndex >= 0 && nIndex < m_nCount) || m_nCount == 0);
		return ((_STATUSBAR_PANE_*)m_pData) + nIndex;
	}

public:
	int		GetPanesCount() const;

	void	SetPaneWidth(int nIndex, int nWidth);

	void	SetPaneInfo(UINT nID, CString strText, COLORREF colorBack, COLORREF colorText);
	void	SetPaneInfo(UINT nID, int nFontSize, CString strFontName, COLORREF colorText,
						COLORREF color1, COLORREF color2, int nFillMode, int nGradentPer, int nGradentType);

	void	SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth);
	void	SetPaneStyle(int nIndex, UINT nStyle);

	void	SetPaneText(UINT nID, CString strText);
	void	SetBackColor(UINT nID, COLORREF color);
	void	SetTextColor(UINT nID, COLORREF color);

public:
	BOOL	AddPane(UINT nID, int nIndex);		// ID of the  pane, index of the pane
	BOOL	RemovePane(UINT nID);				// ID of the pane
	BOOL	AddPaneControl(CWnd* pWnd, UINT nID, BOOL bAutoDestroy);
	BOOL	AddPaneControl(HWND hWnd, UINT nID, BOOL bAutoDestroy, CWnd* pWnd=NULL);
	void	DisableControl( int nIndex, BOOL bDisable=TRUE);

	void	RepositionControls();
	BOOL	PaneInfoGet(int nIndex, _STATUSBAR_PANE_* pPane);
	BOOL	PaneInfoSet(int nIndex, _STATUSBAR_PANE_* pPane);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusBarEx)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CStatusBarEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

