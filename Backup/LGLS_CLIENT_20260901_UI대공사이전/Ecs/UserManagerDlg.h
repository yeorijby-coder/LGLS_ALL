#if !defined(AFX_USERMANAGERDLG_H__6E6861D7_9E24_4C1D_B71A_2D5A7C7FA205__INCLUDED_)
#define AFX_USERMANAGERDLG_H__6E6861D7_9E24_4C1D_B71A_2D5A7C7FA205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserManagerDlg.h : header file
//

#include "SpreadSheet.h"
#include "ResizableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CUserManagerDlg dialog

class CUserManagerDlg : public CResizableDialog
{
// Construction
public:
	CUserManagerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserManagerDlg)
	enum { IDD = IDD_SYS_USER_MANAGER };
	CComboBox	m_cbxLevel;
	CString	m_strUserID;
	CString	m_strUserName;
	CString	m_strPwd;
	CString	m_strPwdConf;
	CSpreadSheet	m_fpSpread;
	//}}AFX_DATA
	BOOL m_bModified;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL WriteFiles();
	BOOL CheckList();

	// Generated message map functions
	//{{AFX_MSG(CUserManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnQueryClicked();
	afx_msg void OnInsertClicked();
	afx_msg void OnUpdateClicked();
	afx_msg void OnDeleteClicked();
	afx_msg void OnExcelClicked();
	afx_msg void OnClickFpspread1(long Col, long Row);
	virtual void OnCancel();
	afx_msg void OnClickedSave();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERMANAGERDLG_H__6E6861D7_9E24_4C1D_B71A_2D5A7C7FA205__INCLUDED_)
