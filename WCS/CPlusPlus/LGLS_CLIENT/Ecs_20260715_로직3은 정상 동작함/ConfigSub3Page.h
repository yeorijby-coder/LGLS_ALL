#pragma once

//#include "Config.h"


// CConfigSub3Page 대화 상자입니다.

class CConfigSub3Page : public CPropertyPage
{
	DECLARE_DYNAMIC(CConfigSub3Page)

public:
	CConfigSub3Page();
	CConfigSub3Page(CEcsDoc* pDoc, CConfig* pConfig);
	virtual ~CConfigSub3Page();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIG_SUB3_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	CEcsDoc* m_pDoc;
	CConfig* m_pConfig;

protected:
//	BOOL	m_bConnect;
//	BOOL	m_bRetry;
//	int		m_nHostPort;
//	int		m_nEcsPort;

public:
	// 1F Main Loop 가동률(%)
//	int m_n1FCapa;
	// 3F Main Loop 가동률(%)
//	int m_n3FCapa;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonSave();
	virtual BOOL OnInitDialog();
public:
//	int m_nHostPort;
	int m_n1FCapa;
	int m_n3FCapa;
	int m_nEcsPort;
	BOOL m_bConnect;
	BOOL m_bRetry;
//	CEdit m_nHostPort;
	int m_nHostPort;
	BUTTON_TYPE m_btnSave;

	CString m_strServer;
	CString m_strDriver;
	CString m_strDBID;
	CString m_strDBPW;

};
