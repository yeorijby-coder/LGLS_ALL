#if !defined(AFX_JOBDLG_H__C6E1F470_07A7_4A3B_889B_759A42ADE31F__INCLUDED_)
#define AFX_JOBDLG_H__C6E1F470_07A7_4A3B_889B_759A42ADE31F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJobDlg dialog

class CEcsDoc;
class CJobItem;
class CJobDlg : public CDialog
{
// Construction
public:
	CJobDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
//	enum EN_KIND { enKindAll, enKindNew, enKindProgress, enKindComplete, enKindError, enKindNotUsed };
	enum EN_KIND { enKindAll, enKindNew, enKindProgress, enKindComplete, enKindError };

	BOOL m_bPairSelected;

protected:
	CEcsDoc* m_pDoc;

protected:
	void UpdateList();
	CJobItem* GetSelectJobItem();

public:
// Dialog Data
	//{{AFX_DATA(CJobDlg)
	enum { IDD = IDD_JOB };
	CComboBox	m_cbxFilterIdleJob;
	CComboBox	m_cbxFilterDestStn;
	CComboBox	m_cbxFilterStartStn;
	CComboBox	m_cbxFilterJobType;
	BUTTON_TYPE	m_btnOK;
	BUTTON_TYPE	m_btnCopy;
	BUTTON_TYPE	m_btnPair;
	BUTTON_TYPE	m_btnArrive;
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnComplete;
	BUTTON_TYPE	m_btnAdd;
	BUTTON_TYPE	m_btnRefresh;
	BUTTON_TYPE	m_btnFind;
	CTabCtrl	m_tabJobKind;
	CListCtrl	m_lstJobList;
	BOOL	m_bAutoRefresh;
	int		m_nFindLuggNum;
	UINT	m_uElapse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJobDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJobDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBtnFindFromLuggNum();
	afx_msg void OnBtnRefresh();
	afx_msg void OnBtnAddOfflineJob();
	afx_msg void OnBtnComplete();
	afx_msg void OnBtnArrive();
	afx_msg void OnBtnCancel();
	afx_msg void OnChkAutoRefresh();
	afx_msg void OnDblclkListJob(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabJob(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCbxFilterJobType();
	afx_msg void OnSelchangeCbxFilterStartStation();
	afx_msg void OnSelchangeCbxFilterDestStation();
	afx_msg void OnTimer(UINT_PTR  nIDEvent);
	afx_msg void OnBtnCopy();
	afx_msg void OnBtnPair();
	afx_msg void OnSelchangeCbxFilterIdleJob();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOBDLG_H__C6E1F470_07A7_4A3B_889B_759A42ADE31F__INCLUDED_)
