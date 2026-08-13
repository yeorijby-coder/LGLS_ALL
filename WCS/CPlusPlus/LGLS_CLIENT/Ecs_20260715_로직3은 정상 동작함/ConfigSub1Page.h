#pragma once

#include "SxButton.h"
#include "ColorPicker.h"


// CConfigSub1Page 대화 상자입니다.
//
class CConfigSub1Page : public CPropertyPage
{
	DECLARE_DYNAMIC(CConfigSub1Page)

public:
	CConfigSub1Page();
	CConfigSub1Page(CEcsDoc* pDoc, CConfig* pConfig);
	virtual ~CConfigSub1Page();

// Dialog Data
	enum { IDD = IDD_CONFIG_SUB1_PAGE };
	CString	m_strJobPath;
	CString	m_strLogPath;
	CString	m_strStcPath;
	CString	m_strInfoPath;
	CString	m_strJobFile;
	CString	m_strLogFile;
	CString	m_strStcFile;
	CString	m_strInfoFile;
	CSxButton	m_btnJobPath;
	CSxButton	m_btnLogPath;
	CSxButton	m_btnStcPath;
	CSxButton	m_btnInfoPath;
	CXColorPickerXP m_btnAutoSto;
	CXColorPickerXP m_btnAutoRet;
	CXColorPickerXP m_btnAutoMove;
	CXColorPickerXP m_btnAutoRtR;
	CXColorPickerXP m_btnAutoWtW;
	CXColorPickerXP m_btnSemiSto;
	CXColorPickerXP m_btnSemiRet;
	CXColorPickerXP m_btnSemiMove;
//	CXColorPickerXP m_btnAutoPriorityRet;
	CXColorPickerXP m_btnSemiRtR;
	CXColorPickerXP m_btnSemiWtW;
	CXColorPickerXP m_btnStoST;
	CXColorPickerXP m_btnRetST;
	CXColorPickerXP m_btnArvST;
	CXColorPickerXP m_btnDepart;
	CXColorPickerXP m_btnArrive;
	CXColorPickerXP m_btnDisconnect;
	CXColorPickerXP m_btnSuspend;
	CXColorPickerXP m_btnManual;
	CXColorPickerXP m_btnAlarm;
	CXColorPickerXP m_btnError;
	BUTTON_TYPE	m_btnSave;

protected:
	CEcsDoc* m_pDoc;
	CConfig* m_pConfig;

// Overrides
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// Implementation
protected:
	void SetSxButton();
	void SetChildWndFont();

protected:
	// Generated message map functions
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonJobPath();
	afx_msg void OnButtonLogPath();
	afx_msg void OnButtonStcPath();
	afx_msg void OnButtonInfoPath();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonColorReset();
	afx_msg void OnButtonPathReset();
	DECLARE_MESSAGE_MAP()
};
