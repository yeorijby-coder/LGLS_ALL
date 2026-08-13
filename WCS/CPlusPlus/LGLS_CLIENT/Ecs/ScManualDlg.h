// ScManualDlg.h : header file
//

#pragma once

class CSc;


// CScManualDlg
//
class CScManualDlg : public CDialogEx
{
public:
	CScManualDlg(CSc* pSc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScManualDlg();

// Dialog Data
	enum { IDD = IDD_SC_MANUAL };
	CTabCtrl	m_tabCommand;
	CComboBox	m_cbxDepartHS;
	CComboBox	m_cbxArriveHS;
	CString		m_strDepartLoc;
	CString		m_strArriveLoc;
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnOK;

// Variables
protected:
	CSc* m_pSc;

// Operations
public:
	enum { enTabStore, enTabRetrieve, enTabHs2Hs, enTabRack2Rack };

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual void OnOK();

// Implementation
protected:
	// Generated message map functions
	afx_msg void OnSelchangeTabCommand(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
