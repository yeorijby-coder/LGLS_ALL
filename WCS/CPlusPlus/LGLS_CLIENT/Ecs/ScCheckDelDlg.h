#if !defined(AFX_SCCHECKDELDLG_H__64F9CD21_8743_4B61_823E_45A1882AB191__INCLUDED_)
#define AFX_SCCHECKDELDLG_H__64F9CD21_8743_4B61_823E_45A1882AB191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScCheckDelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScCheckDelDlg dialog

class CEcsDoc;

class CScCheckDelDlg : public CDialog
{
// Construction
public:
	CScCheckDelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScCheckDelDlg)
	enum { IDD = IDD_CHECK_DELETION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	int m_nID;
	CEcsDoc* m_pDoc;
	int m_nCheck;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScCheckDelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScCheckDelDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnFork1();
	afx_msg void OnFork2();
	afx_msg void OnFork12();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCCHECKDELDLG_H__64F9CD21_8743_4B61_823E_45A1882AB191__INCLUDED_)
