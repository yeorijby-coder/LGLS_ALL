#if !defined(AFX_JOBOFFLINEDLG_H__542FF167_1811_465A_B24E_F98A4970AF4A__INCLUDED_)
#define AFX_JOBOFFLINEDLG_H__542FF167_1811_465A_B24E_F98A4970AF4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobOfflineDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJobOfflineDlg dialog

class CEcsDoc;
class CJobOfflineDlg : public CDialog
{
// Construction
public:
	CJobOfflineDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

protected:
	CEcsDoc* m_pDoc;
	enum EN_TAB { enSemiSto, enSemiRet, enSemiR2R, enSemiMove, enSemiW2W };
	void UpdateStationInfo(int nWarehouse, CComboBox& cbxStation);

public:
// Dialog Data
	//{{AFX_DATA(CJobOfflineDlg)
//	enum { IDD = IDD_JOB_OFFLINE };
	CTabCtrl	m_tabJobType;
	CComboBox	m_cbxStartWH;
	CComboBox	m_cbxDestWH;
	CComboBox	m_cbxStartStn;
	CComboBox	m_cbxDestStn;
	CEdit		m_edtStartLoc;
	CEdit		m_edtDestLoc;
	BUTTON_TYPE	m_btnCreate;
	BUTTON_TYPE	m_btnOK;
	BOOL		m_bAutoJob;
	BOOL		m_bBig;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJobOfflineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJobOfflineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabJobType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCreate();
	afx_msg void OnSelchangeComboStartWarehouse();
	afx_msg void OnSelchangeComboDestWarehouse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOBOFFLINEDLG_H__542FF167_1811_465A_B24E_F98A4970AF4A__INCLUDED_)
