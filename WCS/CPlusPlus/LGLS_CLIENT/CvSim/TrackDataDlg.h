
#pragma once

#include "DialogResize.h"
#include "StartupTip.h"

class CEcsDoc;
class CTrackDataDlg : public CDialogResize
{

public:
	CTrackDataDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

public:
	CEcsDoc* m_pDoc;


public:
	void SetSxButton();
	void ValueChange(CString Type, CString Addr, CString Name, CString Value);

public:
// Dialog Data
	//{{AFX_DATA(CTrackDataDlg)
	enum { IDD = IDD_TRACK_DATA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CFooButton	m_btnExit;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackDataDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrackDataDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DLGRESIZE_MAP;
public:
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedBtnApply();
};

