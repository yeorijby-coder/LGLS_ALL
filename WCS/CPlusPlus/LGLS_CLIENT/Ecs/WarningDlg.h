// WarningDlg.h : header file
//

#pragma once

#include "Resource.h"
#include "DialogResize.h"
#include "StartupTip.h"

class CEcsDoc;


// CWarningDlg
//
class CWarningDlg : public CDialogResize
{

public:
	CWarningDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	virtual ~CWarningDlg();

public:
	CEcsDoc* m_pDoc;
	CCriticalSection m_syncAlarmRefresh;

	BOOL m_bExpand;
	bool m_bShow;

public:
	void Refresh();
	void SetSxButton();
	void ExpandShow(UINT nMarkID, BOOL bExpand);

public:
// Dialog Data
	enum { IDD = IDD_WARNING_DLG };
	CStartupTip m_ctlTip;
	CReportCtrl m_ctlReport;
	CFooButton	m_btnExit;
	CFooButton	m_btnFirst;
	CFooButton	m_btnLast;
	CFooButton	m_btnNext;
	CFooButton	m_btnPrev;
	CFooButton	m_btnDelete;
	CFooButton	m_btnShow;

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void OnOK();

// Implementation
protected:
	LRESULT OnRefreshNotify(WPARAM wParam, LPARAM lParam);

protected:
	// Generated message map functions
	//{{AFX_MSG(CWarningDlg)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonExpand();
	afx_msg void OnButtonShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DLGRESIZE_MAP;
public:
	afx_msg void OnDblclkListWarning(NMHDR *pNMHDR, LRESULT *pResult);
};
