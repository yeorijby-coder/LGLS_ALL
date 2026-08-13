// Host.h : header file
//

#pragma once

#include "ConfigStdPage.h"
#include "ConfigSub1Page.h"
#include "ConfigSub2Page.h"
#include "ConfigSub3Page.h"
//#include "LogoBandWnd.h"


// CConfigPptSheet
//
class CConfigPptSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CConfigPptSheet)

public:
	CConfigPptSheet(CEcsDoc *pDoc, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0) {};
	CConfigPptSheet(CEcsDoc *pDoc, CConfig *pConfig, LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CConfigPptSheet();

// Attributes
public:
	CConfigStdPage m_Page1;
	CConfigSub1Page m_Page2;
	CConfigSub2Page m_Page3;
	CConfigSub3Page m_Page4;
//	CLogoBandWnd m_wndLogoBand;
//	CString m_strLogoFile;

// Operations

// Overrides
public:
	virtual BOOL OnInitDialog();

// Implementation
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};


