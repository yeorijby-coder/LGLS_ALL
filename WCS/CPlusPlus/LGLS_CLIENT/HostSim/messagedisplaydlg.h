#if !defined(AFX_MESSAGEDISPLAYDLG_H__78A9A8B5_4F1D_47B1_8E90_E0D7F8ADE0EF__INCLUDED_)
#define AFX_MESSAGEDISPLAYDLG_H__78A9A8B5_4F1D_47B1_8E90_E0D7F8ADE0EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageDisplayDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageDisplayDlg dialog

class CMessageDisplayDlg : public CDialog
{
// Construction
public:
	CMessageDisplayDlg(CString strData, CWnd* pParent = NULL);   // standard constructor

public:
	CFont m_font;

// Dialog Data
	//{{AFX_DATA(CMessageDisplayDlg)
	enum { IDD = IDD_DISPLAY };
	CString	m_strMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageDisplayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageDisplayDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEDISPLAYDLG_H__78A9A8B5_4F1D_47B1_8E90_E0D7F8ADE0EF__INCLUDED_)
