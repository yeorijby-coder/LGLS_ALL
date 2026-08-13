#if !defined(AFX_CONFIGSUB3PAGE_H__CCE02625_F209_419C_8985_D59E5B007575__INCLUDED_)
#define AFX_CONFIGSUB3PAGE_H__CCE02625_F209_419C_8985_D59E5B007575__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigSub3Page.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigSub3Page dialog

class CConfigSub3Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigSub3Page)

// Construction
public:
	CConfigSub3Page();
	CConfigSub3Page(CEcsDoc* pDoc, CConfig* pConfig);
	~CConfigSub3Page();

// Dialog Data
	//{{AFX_DATA(CConfigSub3Page)
	enum { IDD = IDD_CONFIG_SUB3_PAGE };
	BUTTON_TYPE	m_btnSave;
	//}}AFX_DATA

protected:
	CEcsDoc* m_pDoc;
	CConfig* m_pConfig;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigSub3Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetChildWndFont();

	// Generated message map functions
	//{{AFX_MSG(CConfigSub3Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnCheckMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGSUB3PAGE_H__CCE02625_F209_419C_8985_D59E5B007575__INCLUDED_)
