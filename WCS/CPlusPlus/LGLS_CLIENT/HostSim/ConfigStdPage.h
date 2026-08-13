
#pragma once

class CConfigStdPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigStdPage)

public:
	CConfigStdPage();
	CConfigStdPage(CEcsDoc* pDoc, CConfig* pConfig);
	virtual ~CConfigStdPage();

protected:
	CEcsDoc* m_pDoc;
	CConfig* m_pConfig;

protected:
	void InitButtons();
	void UpdateListEhternet();
	void SetChildWndFont();

public:

// Dialog Data
	//{{AFX_DATA(CConfigStdPage)
	enum { IDD = IDD_CONFIG_STD_PAGE };
	BOOL	m_bBeep;
	BOOL	m_bDebug;
	BOOL	m_bError;
	BOOL	m_bEvent;
	BOOL	m_bJob;
	BOOL	m_bConnect;
	BOOL	m_bRetry;
	int		m_nHostPort;
	int		m_nEcsPort;
	BOOL	m_bAlarm;
	BOOL	m_bViewLampControler;
	BOOL	m_bViewProductInfoInputer;
	int		m_nWeightValue;
	BOOL	m_bLogTest;
	int		m_nFontSize;
	int		m_nBufferLimit;
	BOOL	m_bNotCheckRetHS;
	BOOL	m_bNotCheckRetHsSc1;
	BOOL	m_bNotCheckRetHsSc2;
	BOOL	m_bNotCheckRetHsSc3;
	BOOL	m_bNotCheckRetHsSc4;
	BOOL	m_bNotCheckRetHsSc5;
	BOOL	m_bNotCheckRetHsSc6;
	BOOL	m_bNotBackupNetwork;
	int		m_nCompleteCnt;					// 완료 알람 Check 초 
	//}}AFX_DATA
	BUTTON_TYPE	m_btnSave;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigStdPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigStdPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnDblclkListCommPortEthernet(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLog();
	afx_msg void OnCheckNotCheckHS();
	afx_msg void OnNotCheckRetHsSc1();
	afx_msg void OnNotCheckRetHsSc2();
	afx_msg void OnNotCheckRetHsSc3();
	afx_msg void OnNotCheckRetHsSc4();
	afx_msg void OnNotCheckRetHsSc5();
	afx_msg void OnNotCheckRetHsSc6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


