
#pragma once

#include "SizingControlBar.h"

//#include <afxbasepane.h>
//#include <afxPane.h>
//#include <afxDockablePane.h>
//#include <afxpanedialog.h>

class CDockingBar : public CSizingControlBar//,  public CDockablePane
{
public:
    CDockingBar();
    virtual ~CDockingBar();

public:
	CDialog*	m_pDialog;
	CWnd*		m_pParentWnd;

public:
	BOOL Create(CWnd* pParentWnd, CDialog *pDialog, UINT nID, CString pTitle, CSize Size, DWORD dwStyle = WS_CHILD|WS_VISIBLE|CBRS_TOP);

public:
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDockingBar)
	//}}AFX_VIRTUAL


// Generated message map functions
protected:
    //{{AFX_MSG(CDockingBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

