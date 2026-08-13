// ScRelocationDlg.h : header file
//

#pragma once


// CScRelocationDlg
//
class CScRelocationDlg : public CDialogEx
{
public:
	CScRelocationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScRelocationDlg();

// Dialog Data
	enum { IDD = IDD_SC_NEW_LOCATION };
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnOK;
	CString	m_strLocation;

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual void OnOK();

// Implementation
protected:
	// Generated message map functions
//	afx_msg void OnComSc3W1();
	DECLARE_MESSAGE_MAP()
};
