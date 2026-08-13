// EcsView.h : interface of the CEcsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECSVIEW_H__67D0EF3C_EA19_4198_864B_D1E07B38FD2E__INCLUDED_)
#define AFX_ECSVIEW_H__67D0EF3C_EA19_4198_864B_D1E07B38FD2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CvInfo.h"			// test@@

class CEcsView : public CFormView
{
protected: // create from serialization only
	CEcsView();
	DECLARE_DYNCREATE(CEcsView)

public:
	//{{AFX_DATA(CEcsView)
	enum { IDD = IDD_ECS_FORM };
	CListBox	m_lstHostSv;
	CListBox	m_lstHostCl;
	CListCtrl	m_lstJobList;
	CComboBox	m_cbxLamp;
	CTabCtrl	m_tabLayout;
	CComboBox	m_cbxStation;
	BOOL		m_bOn;
	BOOL		m_bTesting1;
	BOOL		m_bTesting2;
	CListCtrl	m_lstThisLogicJob;
	//}}AFX_DATA
public:
//	CCvInfo* m_pCvInfo;		// test@@
	int m_nMainTimerID;

protected:
	enum EN_TIMER { 
		enTimerStatusDisplay	= 1000,		// 상태바 메시지 갱신 타이머
		enTimerReconnect		= 20000,	// 설비 재접속 시도 타이머
		enTimerStatusReport		= 30000,	// 상태보고 타이머
		enTimerCheckIdleJob		= 60000,	// 작업정보 체크 타이머
		enTimercheckTestMode	= 500		// Test Mode 체크 타이머
	};

	enum EN_LAYOUT_TAB { en1Floor, en2Floor };

public:
	char ConvertNotifyToStatus(int nNotify);
	void UpdateStationInfo(int nWarehouse, CComboBox& cbxStation);
	void UpdateRtvInfo(int nCarNum); 
	void UpdateScRetCountInfo(int nScNum); 

public:
//	void UpdateList(CListCtrl& oListCtrl,
//		int nLuggNum, int nJobType, CString strFromStn, CString strToStn,
//		CString strTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")),
//		BOOL bRemoveAll = FALSE, BOOL bOnlyRemove= FALSE);
	void UpdateList(CListCtrl& oListCtrl, CString strFunction, SJobInvokeInfo* pJobInvokeInfo, BOOL bRemoveAll = FALSE, BOOL bOnlyRemove = FALSE);


// Attributes
public:
	CEcsDoc* GetDocument();

// Operations
public:
	LRESULT OnHostNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnCvNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnScNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnRtvNotify(WPARAM wParam, LPARAM lParam);
//	LRESULT OnLgvNotify(WPARAM wParam, LPARAM lParam);
//	LRESULT OnLifterNotify(WPARAM wParam, LPARAM lParam);
//	LRESULT OnRfidNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnBcrNotify(WPARAM wParam, LPARAM lParam);
//	LRESULT OnDisplayNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnAlarmNotify(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEcsView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEcsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEcsView)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewLayoutInfo();
	afx_msg void OnUnSetAllSuspend(); 
	afx_msg void OnSetAllSuspend();
	afx_msg void OnUpdateViewLayoutInfo(CCmdUI* pCmdUI);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnSelchangeLayoutTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton9();
	afx_msg void OnCheckLamp();
	afx_msg void OnInputMessage();
	afx_msg void OnButton10();
	afx_msg void OnBtnReset1();
	afx_msg void OnBtnReset2();
	afx_msg void OnChkTestStart1();
	afx_msg void OnChkTestStart2();
	afx_msg void OnBtnAlterLoc();
	afx_msg void OnBtnRtrOrder();
	afx_msg void OnBtnAtaOrder();
	afx_msg void OnBtnSiteOrder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	// // LogicGroup
	CComboBox m_cbxLogicGroup;
	afx_msg void OnCbnSelchangeLogicGroup();
	afx_msg void OnBnClickedBtnEnd();
};

#ifndef _DEBUG  // debug version in EcsView.cpp
inline CEcsDoc* CEcsView::GetDocument()
   { return (CEcsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECSVIEW_H__67D0EF3C_EA19_4198_864B_D1E07B38FD2E__INCLUDED_)
