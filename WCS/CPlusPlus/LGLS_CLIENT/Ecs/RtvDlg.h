// RtvDlg.h : header file
//

#pragma once
#include "afxwin.h"

class CRtv;


// CRtvDlg
//
class CRtvDlg : public CDialogEx
{
public:
	CRtvDlg(CWnd* pParent = NULL);								// standard constructor
	CRtvDlg(CRtv* pRtv, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRtvDlg();

// Dialog Data
	enum { IDD = IDD_RTV_PAIR_CTRL };
	BUTTON_TYPE	m_btnComplete;
	BUTTON_TYPE	m_btnEmergency;
	BUTTON_TYPE	m_btnActive;
	BUTTON_TYPE	m_btnStop;
	BUTTON_TYPE	m_btnReset;
	BUTTON_TYPE	m_btnDelete;
	BUTTON_TYPE	m_btnHome;
	BUTTON_TYPE	m_btnManual;
	BUTTON_TYPE	m_btnOK;
	BUTTON_TYPE m_btnReject;
	BOOL		m_bSuspend;

// Variables
protected:
public:
	CRtv* m_pRtv;

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
	afx_msg void OnButtonReset();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonCallToHome();
	afx_msg void OnButtonManual();
	afx_msg void OnCheckSuspend();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonReject();
};
