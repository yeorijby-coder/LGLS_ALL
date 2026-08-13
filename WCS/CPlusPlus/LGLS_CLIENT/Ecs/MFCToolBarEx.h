// MFCToolBarEx.h : header file
//

#pragma once

#include "LogoBandWnd.h"


// CMFCToolBarEx
//
class CMFCToolBarEx : public CMFCToolBar
{
public:
	CMFCToolBarEx();
	virtual ~CMFCToolBarEx();

// Variables
public:
	CLogoBandWnd m_wndLogoBand;
	int m_nTimerID;

// Implementation
public:
	void InitLogo(int nLogoIndex, CString strLogoFile);
	BOOL MakeSureFileExists(CString strFile);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};
