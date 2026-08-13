#if !defined(AFX_SCRELOCATIONDLG_H__C5E870D8_3B9B_4C87_B6AE_1448738C6C24__INCLUDED_)
#define AFX_SCRELOCATIONDLG_H__C5E870D8_3B9B_4C87_B6AE_1448738C6C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScRelocationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScRelocationDlg dialog

class CScRelocationDlg : public CDialog
{
// Construction
public:
	CScRelocationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScRelocationDlg)
	enum { IDD = IDD_SC_NEW_LOCATION };
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnOK;
	CString	m_strLocation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScRelocationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScRelocationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnComSc3W1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRELOCATIONDLG_H__C5E870D8_3B9B_4C87_B6AE_1448738C6C24__INCLUDED_)
