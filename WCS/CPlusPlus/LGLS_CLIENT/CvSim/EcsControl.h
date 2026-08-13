#if !defined(AFX_ECSCONTROL_H__FC68C2D1_1833_40ED_B660_61465499BB35__INCLUDED_)
#define AFX_ECSCONTROL_H__FC68C2D1_1833_40ED_B660_61465499BB35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EcsControl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CEcsControl dialog
class CEcsDoc;
class CEcsControl : public CDialog
{
// Construction
public:
	CEcsControl(CWnd* pParent = NULL);   // standard constructor

public:
	CEcsDoc* m_pDoc;
	
public:
	int m_nCurrPlcNum;
	int m_nTrackNum;
	CString m_strID;
public:
	void Refresh();
	void InitialRefresh();
// Dialog Data
	//{{AFX_DATA(CEcsControl)
	enum { IDD = IDD_ECS_CONTROL };
	CComboBox	m_cbxPlcNo;
	CComboBox	m_cbxDevice;
	CComboBox	m_cbxCvKind;
	CListCtrl	m_lstRegList;
	CListCtrl	m_lstTrackData;
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
	BOOL	m_bTwinCheck;
	BOOL	m_bHSTrack;
	//}}AFX_DATA

public:
	void UpdateList(int nPlcNo, int nDeviceNo);
	void UpdateBits(int nPlcNo, int nDeviceNo);
	void UpdateTrack(int nPlcNo, int nDeviceNo);

	void UpdateTrackData(int nPlcNo, int nDeviceNo);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEcsControl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


public:
	afx_msg void OnButton19();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEcsControl)
	afx_msg void OnViewLayoutInfo();
	afx_msg void OnUnSetAllSuspend(); 
	afx_msg void OnSetAllSuspend();
	afx_msg void OnSelchangeLayoutTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton18();
	afx_msg void OnButton13();
	afx_msg void OnClickListRegData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton5();
	afx_msg void OnButton12();
	afx_msg void OnButton17();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnButton20();
	afx_msg void OnSelchangeComboCvKind();
	afx_msg void OnBtnOpAuto();
	afx_msg void OnCheckNextNor();
	afx_msg void OnCheckCenterNext();
	afx_msg void OnCheckCenterDest();
	afx_msg void OnCheckCrossTrack();
	afx_msg void OnCheckCenter();
	afx_msg void OnCheckCross();
	afx_msg void OnBtnOpSuspend();
	afx_msg void OnButtonSave();
	afx_msg void OnBtnAllauto();
	afx_msg void OnBtnAllSuspend();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckTwin();
	afx_msg void OnBtnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkListTrackData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedTimeSet();
	afx_msg void OnBnClickedChkStartFlag();
	// // CTrackInfo::m_bStartFlag 를 강제 설정하기 위한 변수
	BOOL m_bStartFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECSCONTROL_H__FC68C2D1_1833_40ED_B660_61465499BB35__INCLUDED_)
