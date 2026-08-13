
#pragma once

#include "DialogResize.h"
#include "StartupTip.h"

class CEcsDoc;
class CWarningDlg : public CDialogResize
{

public:
	CWarningDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

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
	//{{AFX_DATA(CWarningDlg)
	enum { IDD = IDD_WARNING_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
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
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarningDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	LRESULT OnRefreshNotify(WPARAM wParam, LPARAM lParam);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWarningDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual void OnOK();
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

};

