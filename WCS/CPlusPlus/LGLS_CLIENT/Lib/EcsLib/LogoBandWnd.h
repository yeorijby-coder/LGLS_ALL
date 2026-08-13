
#ifndef __LOGOBANDWND_H__
#define __LOGOBANDWND_H__

#pragma once

#include "Dib.h"

class AFX_EXT_CLASS CLogoBandWnd : public CWnd
{
public:
	CLogoBandWnd();

protected:
	CDib	m_dibLogo;

// Overrides

// Implementation
public:
	virtual ~CLogoBandWnd();

public:
	BOOL Create(CString strFile, CRect rectWnd, CWnd *pParentWnd);

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

#endif
