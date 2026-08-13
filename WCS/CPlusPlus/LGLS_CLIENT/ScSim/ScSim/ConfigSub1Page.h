
#pragma once

#include "SxButton.h"
#include "ColorPicker.h"

class CConfigSub1Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigSub1Page)

public:
	CConfigSub1Page();
	CConfigSub1Page(CEcsDoc* pDoc, CConfig* pConfig);
	virtual	~CConfigSub1Page();

protected:
	CEcsDoc* m_pDoc;
	CConfig* m_pConfig;

protected:
	void SetSxButton();
	void SetChildWndFont();

public:

// Dialog Data
	//{{AFX_DATA(CConfigSub1Page)
	enum { IDD = IDD_CONFIG_SUB1_PAGE };
	CString	m_strJobPath;
	CString	m_strLogPath;
	CString	m_strStcPath;
	CString	m_strInfoPath;
	CString	m_strJobFile;
	CString	m_strLogFile;
	CString	m_strStcFile;
	CString	m_strInfoFile;
	//}}AFX_DATA
	CSxButton	m_btnJobPath;
	CSxButton	m_btnLogPath;
	CSxButton	m_btnStcPath;
	CSxButton	m_btnInfoPath;
	CColorPicker m_btnAutoSto;
	CColorPicker m_btnAutoRet;
	CColorPicker m_btnAutoMove;
	CColorPicker m_btnAutoRtR;
	CColorPicker m_btnAutoWtW;
	CColorPicker m_btnSemiSto;
	CColorPicker m_btnSemiRet;
	CColorPicker m_btnSemiMove;
	CColorPicker m_btnSemiRtR;
	CColorPicker m_btnSemiWtW;
	CColorPicker m_btnStoST;
	CColorPicker m_btnRetST;
	CColorPicker m_btnArvST;
	CColorPicker m_btnDepart;
	CColorPicker m_btnArrive;
	CColorPicker m_btnDisconnect;
	CColorPicker m_btnSuspend;
	CColorPicker m_btnManual;
	CColorPicker m_btnAlarm;		// DeadLock으로 사용
	CColorPicker m_btnError;
	BUTTON_TYPE	m_btnSave;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigSub1Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigSub1Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonJobPath();
	afx_msg void OnButtonLogPath();
	afx_msg void OnButtonStcPath();
	afx_msg void OnButtonInfoPath();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonColorReset();
	afx_msg void OnButtonPathReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

