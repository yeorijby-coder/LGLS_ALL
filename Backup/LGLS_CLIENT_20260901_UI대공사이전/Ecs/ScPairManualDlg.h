#if !defined(AFX_SCPAIRMANUALDLG_H__3E2EC440_4F8C_43B0_83F2_A411282A3365__INCLUDED_)
#define AFX_SCPAIRMANUALDLG_H__3E2EC440_4F8C_43B0_83F2_A411282A3365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScPairManualDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScPairManualDlg dialog

class CScPair;
class CScInfo;
class CScPairManualDlg : public CDialog
{
// Construction
public:
	CScPairManualDlg(CScPair* pSc, CWnd* pParent = NULL);   // standard constructor
	enum { enTabStore, enTabRetrieve, enTabHs2Hs, enTabRack2Rack };

protected:
	CScPair* m_pSc;
	
	CScInfo * m_pInfo;

public:
// Dialog Data
	//{{AFX_DATA(CScPairManualDlg)
	enum { IDD = IDD_SC_PAIR_MANUAL };
	CTabCtrl	m_tabCommand;
	CComboBox	m_cbxDepartHS;
	CComboBox	m_cbxArriveHS;
	CString		m_strDepartLoc;
	CString		m_strArriveLoc;
	CString		m_strDepartLoc2;
	CString		m_strArriveLoc2;
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnOK;
	BOOL	m_b2Fork;
	BOOL	m_bPair;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScPairManualDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScPairManualDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeTabCommand(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckPair();
	afx_msg void OnCheck2fork();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCPAIRMANUALDLG_H__3E2EC440_4F8C_43B0_83F2_A411282A3365__INCLUDED_)
