#if !defined(AFX_LGVMANUALDLG_H__F9FA3203_F774_4ACC_B9AA_F3A7CFAEB14E__INCLUDED_)
#define AFX_LGVMANUALDLG_H__F9FA3203_F774_4ACC_B9AA_F3A7CFAEB14E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LgvManualDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLgvManualDlg dialog

class CLgv;
class CLgvManualDlg : public CDialog
{
// Construction
public:
	CLgvManualDlg(CLgv* pLgv, CWnd* pParent = NULL);   // standard constructor

protected:
	CLgv* m_pLgv;

public:
// Dialog Data
	//{{AFX_DATA(CLgvManualDlg)
	enum { IDD = IDD_LGV_MANUAL };
	CComboBox	m_cbxDepartsHS;
	CComboBox	m_cbxArrivesHS;
	CComboBox	m_cbxPriority;
	CComboBox	m_cbxGenCode;
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLgvManualDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLgvManualDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LGVMANUALDLG_H__F9FA3203_F774_4ACC_B9AA_F3A7CFAEB14E__INCLUDED_)
