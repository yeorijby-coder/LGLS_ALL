// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "LogoBandWnd.h"
#include "PanelDockPane.h"
#include "PanelJobDlg.h"
#include "PanelInfoDlg.h"
#include "PanelVehDlg.h"
#include "EcsDoc.h"

// [LGLS 2026-09-08] 리본에 붙는 통신상태 표시(EQUIP/HOST/SCH).
//   구 상태바 버튼과 같은 초록/빨강 배경을 쓰기 위해 그리기만 갈아끼운 리본 버튼이다.
//   (아이콘으로 하면 색이 두 가지뿐이라 오히려 손이 더 간다 - 배경색이 곧 상태다)
class CLglsRibbonComm : public CMFCRibbonButton
{
	DECLARE_DYNCREATE(CLglsRibbonComm)
public:
	CLglsRibbonComm();
	CLglsRibbonComm(UINT nID, LPCTSTR lpszText);
	void     SetStateColor(COLORREF clr);
	COLORREF GetStateColor() const { return m_clrState; }
protected:
	COLORREF m_clrState;
	virtual void  OnDraw(CDC* pDC);
	virtual void  OnDrawBorder(CDC* /*pDC*/) {}
	virtual CSize GetRegularSize(CDC* pDC);
	virtual CSize GetCompactSize(CDC* pDC);
};

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

	// [LGLS 2026-09-01] 우측 도킹 판넬 2종 (구 SPL EcsSv CPanelJobDlg/CPanelInfoDlg 참고)
	CPanelDockPane m_JobPane;
	CPanelDockPane m_InfoPane;
	CPanelJobDlg   m_PanelJobDlg;
	CPanelInfoDlg  m_PanelInfoDlg;
	CPanelDockPane m_VehPane;
	CPanelVehDlg   m_PanelVehDlg;    // Crane & Vehicle 반송 현황
	BOOL           m_bPanelBarsCreated;
	BOOL           m_bUiModePanel;        // [LGLS 2026-09-01] UI모드 : TRUE=판넬, FALSE=대화상자
	void ShowPanelBars(CEcsDoc* pDoc, BOOL bShow);
	void SetInfoPaneTitle(CString strTitle);   // 상세정보 판넬 캡션 변경
	void TogglePanelBars(CEcsDoc* pDoc);   // 리본 [작업정보] 진입점
	void ShowJobDetail(CString strLuggNo); // 작업 판넬 선택 -> 정보 판넬 연동
	virtual void RecalcLayout(BOOL bNotify = TRUE);   // [LGLS] 판넬이 상태바를 침범하지 않게 클램프
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
	afx_msg void OnConfigIniOpen();
	// [LGLS 2026-09-05] [환경설정] > [시간 기반 자동 처리] 토글
	//   선택 = 스케줄러가 설비 신호 없이 경과시간으로 완료를 추정하는 처리를 사용한다.
	//   상태는 DB(COMMON_CODE : CDX_CD='SCH_OPT', CCD_CD='AUTO_TIME')에 두어 IO_TASK 가 함께 본다.
	afx_msg void OnConfigAutoTime();
	afx_msg void OnUpdateConfigAutoTime(CCmdUI* pCmdUI);
	BOOL  ReadAutoTimeFlag();
	BOOL  m_bAutoTimeProc;
	DWORD m_dwAutoTimeRead;   // [LGLS 2026-09-03] 리본 [INI 열기]
	afx_msg void OnUiModeDlg();            // [LGLS 2026-09-01] UI모드
	afx_msg void OnUiModePanel();
	afx_msg void OnUpdateUiModeDlg(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUiModePanel(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
	void AddCategoryUSER();
	UINT	m_nAppLook;

public:
	//STATUS
	CMFCRibbonButton* m_BtnStatusCv1;
	CMFCRibbonButton* m_BtnStatusCv1Off;
//	afx_msg void OnUpdateBtnActive(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusCv(CCmdUI *pCmdUI);

	// [LGLS 2026-09-08] 통신상태(EQUIP/HOST/SCH) 표시 위치.
	//   Ecs.ini [MENU] STATUS_POS = TOP(리본 오른쪽 빈자리) / BOTTOM(종전 하단 상태바).
	// [LGLS 2026-09-08] 리본에 붙인 통신상태 요소들(카테고리마다 한 벌 - 어느 탭에서도 보인다)
	CObArray m_arRbnComm;
	BOOL IsStatusOnRibbon();
	void AddCommPanel(CMFCRibbonCategory* pCategory);
	void SetCommColor(UINT nID, COLORREF clr);
	BOOL IsStatusBarOnTop();
	void LayoutStatusBar(int cx, int cy);
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


