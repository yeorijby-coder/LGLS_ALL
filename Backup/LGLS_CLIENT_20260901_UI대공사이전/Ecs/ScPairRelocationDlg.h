#if !defined(AFX_SCPAIRRELOCATIONDLG_H__C5E870D8_3B9B_4C87_B6AE_1448738C6C24__INCLUDED_)
#define AFX_SCPAIRRELOCATIONDLG_H__C5E870D8_3B9B_4C87_B6AE_1448738C6C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScPairRelocationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScPairRelocationDlg dialog

class CScPairRelocationDlg : public CDialog
{
// Construction
public:
	CScPairRelocationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScRelocationDlg)
	enum { IDD = IDD_SC_PAIR_NEW_LOCATION };
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnOK;
	CString	m_strLocation;
	CString	m_strLocation2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScPairRelocationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScPairRelocationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
//	afx_msg void OnComSc3W1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCPAIRRELOCATIONDLG_H__C5E870D8_3B9B_4C87_B6AE_1448738C6C24__INCLUDED_)
