#if !defined(AFX_SCMANUALDLG_H__3E2EC440_4F8C_43B0_83F2_A411282A3365__INCLUDED_)
#define AFX_SCMANUALDLG_H__3E2EC440_4F8C_43B0_83F2_A411282A3365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScManualDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScManualDlg dialog

class CSc;
class CScManualDlg : public CDialog
{
// Construction
public:
	CScManualDlg(CSc* pSc, CWnd* pParent = NULL);   // standard constructor
	enum { enTabStore, enTabRetrieve, enTabHs2Hs, enTabRack2Rack };

protected:
	CSc* m_pSc;

public:
// Dialog Data
	//{{AFX_DATA(CScManualDlg)
	enum { IDD = IDD_SC_MANUAL };
	CTabCtrl	m_tabCommand;
	CComboBox	m_cbxDepartHS;
	CComboBox	m_cbxArriveHS;
	CString		m_strDepartLoc;
	CString		m_strArriveLoc;
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScManualDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScManualDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeTabCommand(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCMANUALDLG_H__3E2EC440_4F8C_43B0_83F2_A411282A3365__INCLUDED_)
