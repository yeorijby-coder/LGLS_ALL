#if !defined(AFX_CHANGEPASSWORDDLG_H__466B5E80_022A_11D3_B136_080009B90446__INCLUDED_)
#define AFX_CHANGEPASSWORDDLG_H__466B5E80_022A_11D3_B136_080009B90446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangePasswordDlg.h : header file
//

class CEcsDoc;

/////////////////////////////////////////////////////////////////////////////
// CChangePasswordDlg dialog

class CChangePasswordDlg : public CDialog
{
// Construction
public:
	CChangePasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangePasswordDlg)
	enum { IDD = IDD_PASSWORD_CHANGE };
	CString	m_strPasswordChanged;
	CString	m_strPasswordConfirm;
	CString	m_strPasswordOrigin;
	//}}AFX_DATA

// Attributes
	CEcsDoc* m_pDoc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangePasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangePasswordDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEPASSWORDDLG_H__466B5E80_022A_11D3_B136_080009B90446__INCLUDED_)
