#pragma once


// CConfigStdPage 대화 상자입니다.
//
class CConfigStdPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CConfigStdPage)

public:
	CConfigStdPage();
	CConfigStdPage(CEcsDoc* pDoc, CConfig* pConfig);
	virtual ~CConfigStdPage();

// Dialog Data
	enum { IDD = IDD_CONFIG_STD_PAGE };
	BOOL	m_bBeep;
	BOOL	m_bDebug;
	BOOL	m_bError;
	BOOL	m_bEvent;
	BOOL	m_bJob;
	BOOL	m_bAlarm;
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
	void InitButtons();
	void UpdateListEhternet();
	void SetChildWndFont();

	// Generated message map functions
protected:
	afx_msg void OnButtonSave();
	afx_msg void OnDblclkListCommPortEthernet(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	// 1F Main Loop 가동률(%)
	int m_n1FCapa;
	// 3F Main Loop 가동률(%)
	int m_n3FCapa;
	// // BCR 시뮬레이션 모드 설정
	BOOL m_bBcrSimMode;
	BOOL m_bSameCheck;
	BOOL m_bRolling1NotUse;
	BOOL m_bRolling2NotUse;
};
