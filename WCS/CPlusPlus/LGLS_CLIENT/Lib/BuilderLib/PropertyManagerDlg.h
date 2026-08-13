#if !defined(AFX_PROPERTYMANAGERDLG_H__1F2D2BEB_1F07_4DD5_ADE4_12509BDB87F5__INCLUDED_)
#define AFX_PROPERTYMANAGERDLG_H__1F2D2BEB_1F07_4DD5_ADE4_12509BDB87F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyManagerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyManagerDlg dialog

#include "BuildManager.h"

class AFX_EXT_CLASS CPropertyManagerDlg : public CDialog
{
// Construction
public:
	CPropertyManagerDlg(CBuildManager* pBuilder, CWnd* pParent = NULL);   // standard constructor

protected:
	CBuildManager* m_pBuilder;

public:
// Dialog Data
	//{{AFX_DATA(CPropertyManagerDlg)
	enum { IDD = IDD_PROPERTY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyManagerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYMANAGERDLG_H__1F2D2BEB_1F07_4DD5_ADE4_12509BDB87F5__INCLUDED_)
