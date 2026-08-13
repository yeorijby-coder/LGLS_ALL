#if !defined(AFX_JOBTESTDLG_H__542FF167_1811_465A_B24E_F98A4970AF4A__INCLUDED_)
#define AFX_JOBTESTDLG_H__542FF167_1811_465A_B24E_F98A4970AF4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobTestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJobTestDlg dialog

class CEcsDoc;
class CJobTestDlg : public CDialog
{
// Construction
public:
	CJobTestDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

protected:
	CEcsDoc* m_pDoc;

	enum EN_TAB { enSemiSto, enSemiRet, enSemiR2R, enSemiMove, enSemiW2W };
	void UpdateStationInfo(int nWarehouse, CComboBox& cbxStation);

public:
	CString m_strLocation;
	CString m_strStation;

public:
// Dialog Data
	//{{AFX_DATA(CJobTestDlg)
	enum { IDD = IDD_JOB_TEST };
	CComboBox	m_cbxStation;
	CEdit		m_edtLocation;
	CEdit		m_edtStation;
	BUTTON_TYPE	m_btnCreate;
	BUTTON_TYPE	m_btnOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJobTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJobTestDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOBTESTDLG_H__542FF167_1811_465A_B24E_F98A4970AF4A__INCLUDED_)
