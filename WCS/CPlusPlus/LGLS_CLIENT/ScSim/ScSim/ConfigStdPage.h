
#pragma once
#include "afxcmn.h"

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
	CReportCtrl m_ctlEquip;

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMDblclkListEquipment(NMHDR *pNMHDR, LRESULT *pResult);
};


