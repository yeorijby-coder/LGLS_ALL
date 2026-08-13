// EcsView.h : interface of the CEcsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECSVIEW_H__67D0EF3C_EA19_4198_864B_D1E07B38FD2E__INCLUDED_)
#define AFX_ECSVIEW_H__67D0EF3C_EA19_4198_864B_D1E07B38FD2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CvInfo.h"			// test@@
#include "MyTabCtrl.h"

class CByteArrayEx;
class CEcsView : public CFormView
{
protected: // create from serialization only
	CEcsView();
	DECLARE_DYNCREATE(CEcsView)

public:
	//{{AFX_DATA(CEcsView)
	enum { IDD = IDD_ECS_FORM };
//	CTabCtrl	m_tabLayout;
	CMyTabCtrl	m_tabLayout;
	CComboBox	m_cbxPlcNo;
	CComboBox	m_cbxDevice;
	CComboBox	m_cbxCvKind;
	CListCtrl	m_lstRegList;
	BOOL   m_bBit01;
	BOOL   m_bBit02;
	BOOL   m_bBit03;
	BOOL   m_bBit04;
	BOOL   m_bBit05;
	BOOL   m_bBit06;
	BOOL   m_bBit07;
	BOOL   m_bBit08;
	BOOL   m_bBit09;
	BOOL   m_bBit10;
	BOOL   m_bBit11;
	BOOL   m_bBit12;
	BOOL   m_bBit13;
	BOOL   m_bBit14;
	BOOL   m_bBit15;
	BOOL   m_bBit16;

	BOOL   m_bBit17;
	BOOL   m_bBit18;
	BOOL   m_bBit19;
	BOOL   m_bBit20;
	BOOL   m_bBit21;
	BOOL   m_bBit22;
	BOOL   m_bBit23;
	BOOL   m_bBit24;
	BOOL   m_bBit25;
	BOOL   m_bBit26;
	BOOL   m_bBit27;
	BOOL   m_bBit28;
	BOOL   m_bBit29;
	BOOL   m_bBit30;
	BOOL   m_bBit31;
	BOOL   m_bBit32;

	BOOL   m_bBit33;
	BOOL   m_bBit34;
	BOOL   m_bBit35;
	BOOL   m_bBit36;
	BOOL   m_bBit37;
	BOOL   m_bBit38;
	BOOL   m_bBit39;
	BOOL   m_bBit40;
	BOOL   m_bBit41;
	BOOL   m_bBit42;
	BOOL   m_bBit43;
	BOOL   m_bBit44;
	BOOL   m_bBit45;
	BOOL   m_bBit46;
	BOOL   m_bBit47;
	BOOL   m_bBit48;

	BOOL   m_bNextNor;
	BOOL   m_bCenter;
	BOOL   m_bCenterNext;
	BOOL   m_bCenterDest;
	BOOL   m_bCross;
	BOOL   m_bCrossTrack;

	//}}AFX_DATA

public:
	CCvInfo* m_pCvInfo;		// test@@
	UINT m_nMainTimerID;

	enum EN_LAYOUT_TAB { en1Floor,/* en1_5Floor,*/ en2Floor =2, en3Floor=3};
	enum EN_LAYOUT_INDEX { EN_1F, EN_2F, EN_Assemble};

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
	void UpdateStationInfo(int nWarehouse, CComboBox& cbxStation);
	void UpdateList(int nPlcNo, int nDeviceNo);
	void UpdateBits(int nPlcNo, int nDeviceNo);
	void UpdateTrack(int nPlcNo, int nDeviceNo);
	void InvokeStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation);
	void InvokeHS(CCv* pCv, CTrackInfo* pTrack);
	void LgvStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation);
	void MoveNextTrackForKindNormal_1(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation);
	void MoveNextTrackForKindDiverter_1(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation);
	void MoveNextTrackForKindNormal_2(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation);
	void MoveNextTrackForKindDiverter_2(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation);
	void LoadMG(CCv* pCv, CTrackInfo* pTrack, int nNextPlcNum, int nNextTrNum);
	void UnLoadDP(CCv* pCv, CTrackInfo* pTrack, int nNextPlcNum, int nNextTrNum);

public:
	void InvokeArvStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation);
	void InvokeRetStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation);
	void InvokeStoStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation);

	void InvokeScStoHs(CCv* pCv, CTrackInfo* pTrack);
	void InvokeScRetHs(CCv* pCv, CTrackInfo* pTrack);

	BOOL InvokeLogic(CCv* pCv, CTrackInfo* pTrack, CLogicArray* pLogicArray);
	BOOL IsBitOnOffByKeyWord(int m_nNumber, int nDevNum, CString strKeyWord, BOOL bOn, BOOL bNext);

	BOOL UpdateSignalReg(CTrackInfo* pTrack, int nPlcNo, int nDeviceNo, CString strKeyWord, BOOL bOn);

// Attributes
public:
	CEcsDoc* GetDocument();

// Operations
public:
	LRESULT OnHostNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnCvNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnScNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnRtvNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnLgvNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnLifterNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnRfidNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnBcrNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnThsNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnDisplayNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnAlarmNotify(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEcsView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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

public:
	void OnMouseMoveSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point );
	void PreTranslateMessageSelectedLayout( CEcsDoc * pDoc, WPARAM wParam, int param3, int param4 );

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEcsView)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewLayoutInfo();
	afx_msg void OnUnSetAllSuspend(); 
	afx_msg void OnSetAllSuspend();
	afx_msg void OnSelchangeLayoutTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton18();
	afx_msg void OnButton13();
	afx_msg void OnClickListRegData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton5();
	afx_msg void OnButton12();
	afx_msg void OnButton17();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	afx_msg void OnSelchangeComboCvKind();
	afx_msg void OnButton22();
	afx_msg void OnCheckNextNor();
	afx_msg void OnCheckCenterNext();
	afx_msg void OnCheckCenterDest();
	afx_msg void OnCheckCrossTrack();
	afx_msg void OnCheckCenter();
	afx_msg void OnCheckCross();
	afx_msg void OnButton23();
	afx_msg void OnButtonSave();
	afx_msg void OnBtnAllauto();
	afx_msg void OnBtnAllSuspend();
	afx_msg void OnSysControl();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EcsView.cpp
inline CEcsDoc* CEcsView::GetDocument()
   { return (CEcsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECSVIEW_H__67D0EF3C_EA19_4198_864B_D1E07B38FD2E__INCLUDED_)
