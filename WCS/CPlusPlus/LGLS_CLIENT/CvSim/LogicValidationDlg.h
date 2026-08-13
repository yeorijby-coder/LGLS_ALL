
#pragma once

#include "DialogResize.h"
#include "StartupTip.h"

class CEcsDoc;
class CLogicValidationDlg : public CDialogResize
{

public:
	CLogicValidationDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

public:
	CEcsDoc* m_pDoc;

public:
	void SetSxButton();

public:
// Dialog Data
	//{{AFX_DATA(CLogicValidationDlg)
	enum { IDD = IDD_LOGIC_VALIDATION };
	CListCtrl m_lstLogicGroup;
	CListCtrl m_lstLogicDesc;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CFooButton	m_btnExit;
	CListCtrl	m_lstConditionData;

	CString sSelectedGroup;
	CString sConditionResult[20];
	CString sActionResult[20];

	CString	 ContentConvert(int nNumber);
	int GetTrackLogicData(CString sKeyword, int nGetData, int nTrackNum, int enType, int nCount);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogicValidationDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogicValidationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DLGRESIZE_MAP;
public:
	afx_msg void OnNMDblclkListLogicGroup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListLogicDesc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeTrackNumber();
	afx_msg void OnNMDblclkListLogicCondition(NMHDR* pNMHDR, LRESULT* pResult);
};

