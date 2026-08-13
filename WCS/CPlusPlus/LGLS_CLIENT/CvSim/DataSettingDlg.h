#if !defined(AFX_DATASETTINGDLG_H__426916B8_38C6_4178_8A09_3B4861EF552F__INCLUDED_)
#define AFX_DATASETTINGDLG_H__426916B8_38C6_4178_8A09_3B4861EF552F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogResize.h"
#include "StartupTip.h"

// DataSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataSettingDlg dialog

class CEcsDoc;
class CDataSettingDlg : public CDialogResize
{
public:
	CDataSettingDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

public:
	CEcsDoc* m_pDoc;

public:
	CString m_strData;
	CString m_strCaption;

	void ValueChange(CString Value);
	
public:
// Dialog Data
	//{{AFX_DATA(CDataSettingDlg)
	enum { IDD = IDD_DATA_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	BUTTON_TYPE	m_btnOK;
	BUTTON_TYPE m_btnCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASETTINGDLG_H__426916B8_38C6_4178_8A09_3B4861EF552F__INCLUDED_)
