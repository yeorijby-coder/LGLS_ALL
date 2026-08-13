// ScDlg.h : header file
//

#pragma once

class CSc;


// CScDlg
//
class CScDlg : public CDialogEx
{
public:
	CScDlg(CWnd* pParent = NULL);								// standard constructor
	CScDlg(CSc* pSc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScDlg();

// Dialog Data
	enum { IDD = IDD_SC_CTRL1 };
	BUTTON_TYPE	m_btnComplete;
	BUTTON_TYPE	m_btnEmergency;
	BUTTON_TYPE	m_btnActive;
	BUTTON_TYPE	m_btnStop;
	BUTTON_TYPE	m_btnReset;
	BUTTON_TYPE	m_btnDelete;
	BUTTON_TYPE	m_btnHome;
	BUTTON_TYPE	m_btnDualStore;
	BUTTON_TYPE	m_btnManual;
	BUTTON_TYPE	m_btnOK;
	BOOL	m_bRetSuspend;
	BOOL	m_bStoSuspend;

// Variables
protected:
public:
	CSc* m_pSc;

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL DestroyWindow();

// Implementation
protected:
	// Generated message map functions
	afx_msg void OnButtonComplete();
	afx_msg void OnButtonEmergency();
	afx_msg void OnButtonActive();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonErrorReset();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonCallToHome();
	afx_msg void OnButtonDualStore();
	afx_msg void OnButtonManual();
	afx_msg void OnCheckSuspentStore();
	afx_msg void OnCheckSuspendRetrieve();
	DECLARE_MESSAGE_MAP()
};
