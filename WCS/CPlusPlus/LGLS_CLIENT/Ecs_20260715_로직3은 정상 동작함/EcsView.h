// EcsView.h : CEcsView 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "CvInfo.h"			// test@@
#include "afxcmn.h"

class CEcsView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CEcsView();
	DECLARE_DYNCREATE(CEcsView)

public:
	enum{ IDD = IDD_ECS_FORM };

	//}}AFX_DATA
public:
	int m_nSearchType;
	BOOL m_bSearchFlag;
	int m_nSearchCount;
	CString m_strSearch;	

	BOOL m_bTest;

public:
	CCvInfo* m_pCvInfo;		// test@@
	UINT m_nMainTimerID;


protected:
	enum EN_TIMER { 
		enTimerStatusDisplay	= 1000,		// 상태바 메시지 갱신 타이머
		enTimerReconnect		= 20000,	// 설비 재접속 시도 타이머
		enTimerStatusReport		= 30000,	// 상태보고 타이머
		enTimerClientReConnect	= 120000,	// Client 재접속 시도 타이머
		enTimerCheckIdleJob		= 100000,	// 작업정보 체크 타이머
		enTimerThsInfo			= 600000,	// 온습도계 정보 송신 타이머
		enTimercheckTestMode	= 500		// Test Mode 체크 타이머
	};

public:
	char ConvertNotifyToStatus(int nNotify);
// 특성입니다.
public:
	CEcsDoc* GetDocument() const;




public:
	//WM_USER_MESSAGE
/*	LRESULT OnUpdateLayout(WPARAM wParam, LPARAM lParam);*/

	void DrawSelectedLayout( CDC* pDC, CEcsDoc* pDoc );
	void WheelSelectedLayout( CEcsDoc* pDoc, UINT nFlags, short zDelta, CPoint pt );
	void RButtonUpSelectedLayout( CEcsDoc* pDoc);
	void OnSizeSelectedLayout(CEcsDoc* pDoc, UINT nType, int cx, int cy );
	void OnLButtonDownSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point );
	void OnKeyDownSelectedLayout( CEcsDoc* pDoc, UINT nChar, UINT nRepCnt, UINT nFlags );
	void OnSizeSelectedLayout( UINT nType, int cx, int cy );
	void OnLButtonDownSelectedLayout( CEcsDoc* pDoc, CEcsView* param2, UINT nFlags, CPoint point );
	void OnButtonUpSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point );
	void PreTranslateMessageSelectedLayout( CEcsDoc * pDoc, WPARAM wParam, int param3, int param4 );
	void OnMouseMoveSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point );


// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnDraw(CDC* pDC);

	LRESULT OnControlClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnRefreshDialog(WPARAM wParam, LPARAM lParam);
	LRESULT OnViewLayoutInfo(WPARAM wParam, LPARAM lParam);

// 구현입니다.
public:
	virtual ~CEcsView();
	void CalcSerachControlPos();						// YJY
	void MoveSearchWnd(int cx, int cy, BOOL bStatus);	// YJY

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()



public:
	afx_msg void OnSearch();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void GetQrySelectStatusAll( CCV_DATA* pCV_DATA, CString& pSTOCK_MODE, CString& pREMOTE_CONTROL, CString& pROLL_MODE);
	CTabCtrl m_tabLayout;
};

#ifndef _DEBUG  // EcsView.cpp의 디버그 버전
inline CEcsDoc* CEcsView::GetDocument() const
   { return reinterpret_cast<CEcsDoc*>(m_pDocument); }
#endif

