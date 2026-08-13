#pragma once
#include "afxcmn.h"


// CConfigSub2Page 대화 상자입니다.
//
class CConfigSub2Page : public CPropertyPage
{
	DECLARE_DYNAMIC(CConfigSub2Page)

public:
	CConfigSub2Page();
	CConfigSub2Page(CEcsDoc* pDoc, CConfig* pConfig);
	virtual ~CConfigSub2Page();

// Dialog Data
	enum { IDD = IDD_CONFIG_SUB2_PAGE1 };
	int		m_nMonPort;
	BUTTON_TYPE	m_btnSave;

protected:
	CEcsDoc* m_pDoc;
	CConfig* m_pConfig;
	CReportCtrl m_ctlReport;
	CReportCtrl m_ctlEquip;

// Overrides
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// Implementation
protected:
	void SetChildWndFont();

protected:
	// Generated message map functions
	afx_msg void OnButtonSave();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListEquipment(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListEquipment(NMHDR *pNMHDR, LRESULT *pResult);
};
