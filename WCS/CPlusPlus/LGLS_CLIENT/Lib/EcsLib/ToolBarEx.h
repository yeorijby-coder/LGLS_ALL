
#ifndef __TOOLBAREX_H__
#define __TOOLBAREX_H__

#include "LogoBandWnd.h"

class AFX_EXT_CLASS CToolBarEx : public CToolBar
{
public:
	CToolBarEx();

public:
	CLogoBandWnd m_wndLogoBand;
	int m_nTimerID;

public:
	void InitLogo(int nLogoIndex, CString strLogoFile);
	BOOL MakeSureFileExists(CString strFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolBarEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBarEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
