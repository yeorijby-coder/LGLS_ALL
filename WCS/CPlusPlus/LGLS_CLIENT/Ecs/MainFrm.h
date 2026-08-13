// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "LogoBandWnd.h"
#include "EcsDoc.h"

class CMainFrame : public CFrameWndEx
{

protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	int GetIndex(char Status);
	void UpdateCommStatus(int nIndex, UINT unID, char Status);
	void DisplayMessage(CString strMsg);

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CMFCRibbonBar		    m_wndRibbonBar;
	CStatusBarEx			m_wndStatusBar;
	
	CEcsDoc * m_pDoc;
	CCV_DATA* m_pCV_DATA;

	HICON m_hIcon;

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	//CMFCStatusBar			m_wndStatusBar;
	CMFCToolBar				m_wndToolBar;
	CMFCRibbonApplicationButton m_MainButton;     
	CMFCToolBarImages              m_PanelImages; 

	CRect					m_rLogoRect;
	bool					m_bNotDockingJob;
	bool					m_bNotDockingInfo;
	bool					m_bShowStatusBar;
	bool					m_bToolNMenuBar;


public:
	BOOL MakeSureFileExists(CString strFile);

public:
	void InitializeRibbonMenu(EN_LANG penLang = EN_LANG::EN_ENG);
	void RenameRibbonText(EN_LANG penLang = EN_ENG);
	HICON HICONFromPATH(CString pstrPath);
	CString GetConcatPath_LANG(CString pstrAppPath, CString pstrFileName, CString pstrExtension, EN_LANG penLang);

//	void InitSystemDisplay();
//	void InsertButtonPainToStatusBar(CString strCaption, int nID, int nPos, int nWidth);
//	void InsertLabelPainToStatusBar(CString strCaption, int nID, int nPos, int nWidth);

public:
LRESULT OnLangUpdate(WPARAM wParam, LPARAM lParam);

public:
	void ExcuteTheme();
	void AddCategoryWCS();
	void AddCategoryMANUAL();
	void AddCategoryLOG();
	void AddCategorySTATUS();
	CString GetConcatPath(CString pstrAppPath, CString pstrFileName, CString pstrExtension);

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();

	afx_msg void OnStyleRibboninterface (); //ribbon2
	afx_msg void OnRbnBtnToolbarInterface (); 
	afx_msg void OnAppLook(UINT id);
	DECLARE_MESSAGE_MAP()
	void AddCategoryUSER();
	UINT	m_nAppLook;

public:
	//STATUS
	CMFCRibbonButton* m_BtnStatusCv1;
	CMFCRibbonButton* m_BtnStatusCv1Off;
//	afx_msg void OnUpdateBtnActive(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusCv(CCmdUI *pCmdUI);

	void AddStatusBarPane();
	void InsertLabelPainToStatusBar(CString strCaption, int nID, int nPos, int nWidth);
	void InsertButtonPainToStatusBar(CString strCaption, int nID, int nPos, int nWidth);
	afx_msg void OnUpdateStatusSR_MODE(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusCv1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusSch(CCmdUI *pCmdUI);	// [LGLS 재적용] SCH(IO_TASK) 하트비트 표시등
	afx_msg void OnSize(UINT nType, int cx, int cy);	// [LGLS 재적용] 상태바 통신버튼 위치 재계산
	afx_msg void OnUpdateStatusWc1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusSc1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusSc2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusSc3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusSc4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusHost(CCmdUI *pCmdUI);

	afx_msg void OnButtonCv1();
	afx_msg void OnButtonSc1();
	afx_msg void OnButtonSc2();
	afx_msg void OnButtonSc3();
	afx_msg void OnButtonSc4();
	afx_msg void OnButtonWc1();
	afx_msg void OnButtonHost();

	afx_msg void OnButtonComm(UINT nID);
};


