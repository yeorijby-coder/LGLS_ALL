#if !defined(AFX_PASSWORDDLG_H__C0E7EF00_AA77_11D0_B136_080009B90446__INCLUDED_)
#define AFX_PASSWORDDLG_H__C0E7EF00_AA77_11D0_B136_080009B90446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswordDlg.h : header file
//

class CEcsDoc;

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog

class CPasswordDlg : public CDialog
{
// Construction
public:
	CPasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPasswordDlg)
	enum { IDD = IDD_PASSWORD };
	CString	m_strPassword;
	//}}AFX_DATA

// Attributes
	CEcsDoc* m_pDoc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswordDlg)
	afx_msg void OnButtonChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORDDLG_H__C0E7EF00_AA77_11D0_B136_080009B90446__INCLUDED_)
