
#pragma once

class CConfigSub3Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigSub3Page)

public:
	CConfigSub3Page();
	CConfigSub3Page(CEcsDoc* pDoc, CConfig* pConfig);
	virtual ~CConfigSub3Page();

protected:
	CEcsDoc* m_pDoc;
	CConfig* m_pConfig;
	CReportCtrl m_ctlReport;

protected:
	void SetChildWndFont();

public:
// Dialog Data
	//{{AFX_DATA(CConfigSub3Page)
	enum { IDD = IDD_CONFIG_SUB3_PAGE };
	CComboBox	m_cbxLevel;
	CComboBox	m_cbxBay;
	CComboBox	m_cbxBank;
	int		m_nBank;
	int		m_nBay;
	int		m_nLevel;
//	int		m_nMonPort;
	//}}AFX_DATA
	BUTTON_TYPE	m_btnRackWrite;
	BUTTON_TYPE	m_btnDisableRack;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigSub3Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigSub3Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnRackWrite();
	afx_msg void OnBtnDisableRack();
//	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void Refrash();

};


