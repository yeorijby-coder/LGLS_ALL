
#pragma once

class CConfigSub2Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigSub2Page)

public:
	CConfigSub2Page();
	CConfigSub2Page(CEcsDoc* pDoc, CConfig* pConfig);
	virtual ~CConfigSub2Page();

protected:
	CEcsDoc* m_pDoc;
	CConfig* m_pConfig;
	CReportCtrl m_ctlReport;

protected:
	void SetChildWndFont();

public:
// Dialog Data
	//{{AFX_DATA(CConfigSub2Page)
	enum { IDD = IDD_CONFIG_SUB2_PAGE };
	int		m_nMonPort;
	//}}AFX_DATA
	BUTTON_TYPE	m_btnSave;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigSub2Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigSub2Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


