#ifndef __CONFIGPPTSHEET_H__
#define __CONFIGPPTSHEET_H__

#include "ConfigStdPage.h"
#include "ConfigSub1Page.h"
#include "ConfigSub2Page.h"
#include "ConfigSub3Page.h"
//#include "LogoBandWnd.h"

class CConfigPptSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CConfigPptSheet)

// Construction
public:
	CConfigPptSheet(CEcsDoc *pDoc, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0) {};
	CConfigPptSheet(CEcsDoc *pDoc, CConfig *pConfig, LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CConfigStdPage m_Page1;
	CConfigSub1Page m_Page2;
	CConfigSub2Page m_Page3;
	CConfigSub3Page m_Page4;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigPptSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConfigPptSheet();

// Generated message map functions
protected:
	//{{AFX_MSG(CConfigPptSheet)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif	// __CONFIGPPTSHEET_H__
