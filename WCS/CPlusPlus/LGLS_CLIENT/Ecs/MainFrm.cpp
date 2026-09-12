// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Ecs.h"
#include "MainFrm.h"
#include "Splash.h"
#include "MFCRibbonPanel_Wrap.h"
#include "MinButton.h"
#include "RecordSetWrap.h"
#include "Lib.h"		// [LGLS 2026-09-12] UiLog (리본 툴팁 적용 증거)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMainFrame
//
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

const int iCategoryIndex_MAIN = 0;
const int iCategoryIndex_SETTING = 1;
const int iCategoryIndex_AA = 2;
const int iCategoryIndex_BB = 3;
const int iCategoryIndex_CC = 4;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_TIMER()		// [LGLS 2026-09-12] 제목줄 마퀴
	ON_COMMAND(ID_CONFIG_INI_OPEN, &CMainFrame::OnConfigIniOpen)
	ON_COMMAND(ID_CONFIG_AUTO_TIME, &CMainFrame::OnConfigAutoTime)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_AUTO_TIME, &CMainFrame::OnUpdateConfigAutoTime)
	// [LGLS 2026-09-09] [판넬 보기] : 판넬 3개 개별 표시 토글
	ON_COMMAND_RANGE(ID_PANE_SHOW_JOB, ID_PANE_SHOW_VEH, &CMainFrame::OnPaneShow)
	ON_UPDATE_COMMAND_UI_RANGE(ID_PANE_SHOW_JOB, ID_PANE_SHOW_VEH, &CMainFrame::OnUpdatePaneShow)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()

	ON_MESSAGE(WM_USER_LANG_NOTIFY, &CMainFrame::OnLangUpdate)


	//	ON_UPDATE_COMMAND_UI(IDC_BTN_ACTIVE, &CMainFrame::OnUpdateBtnActive)
	ON_UPDATE_COMMAND_UI(ID_STATUS_CV, &CMainFrame::OnUpdateStatusCv)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SR_MODE, &CMainFrame::OnUpdateStatusSR_MODE)
	ON_UPDATE_COMMAND_UI(ID_STATUS_CV_1, &CMainFrame::OnUpdateStatusCv1)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_1, &CMainFrame::OnUpdateStatusSc1)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_2, &CMainFrame::OnUpdateStatusSc2)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_3, &CMainFrame::OnUpdateStatusSc3)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_4, &CMainFrame::OnUpdateStatusSc4)
	ON_UPDATE_COMMAND_UI(ID_STATUS_HOST, &CMainFrame::OnUpdateStatusHost)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SCH, &CMainFrame::OnUpdateStatusSch)

	//ON_BN_CLICKED(ID_STATUS_CV_1, &CMainFrame::OnButtonCv1)
	//ON_BN_CLICKED(ID_STATUS_SC_1, &CMainFrame::OnButtonSc1)
	//ON_BN_CLICKED(ID_STATUS_SC_2, &CMainFrame::OnButtonSc2)
	//ON_BN_CLICKED(ID_STATUS_SC_3, &CMainFrame::OnButtonSc3)
	//ON_BN_CLICKED(ID_STATUS_SC_4, &CMainFrame::OnButtonSc4)
	//ON_BN_CLICKED(ID_STATUS_WC_1, &CMainFrame::OnButtonWc1)
	//ON_BN_CLICKED(ID_STATUS_HOST, &CMainFrame::OnButtonHost)

	ON_CONTROL_RANGE(BN_CLICKED, ID_STATUS_CV_1,ID_STATUS_SCH, &CMainFrame::OnButtonComm)
	// [LGLS 2026-09-08] 리본 요소는 WM_COMMAND(lParam=0)로 오므로 ON_CONTROL_RANGE 로는 안 잡힌다.
	ON_COMMAND_RANGE(ID_STATUS_CV_1, ID_STATUS_SCH, &CMainFrame::OnButtonComm)

END_MESSAGE_MAP()

// [LGLS] SCH pane: IO_TASK writes an EQP_MST heartbeat (EQP_TYP='SCH') about every 2s.
//   Allow some slack for DB/network jitter before declaring it dead.
static const int SCH_ALIVE_SEC = 10;

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	m_bPanelBarsCreated = FALSE;   // [LGLS 2026-09-01] 도킹 판넬
	m_bUiModePanel = FALSE;        // 기본 = 대화상자 모드
	m_pDoc = NULL;                 // [LGLS 2026-09-01] 종전에 초기화 누락(쓰레기 포인터)
	m_bAutoTimeProc = FALSE;       // [LGLS 2026-09-05] 시간 기반 자동 처리(첫 갱신 전 기본 = 사용 안 함)
	m_dwAutoTimeRead = 0;
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_bNotDockingJob = false;
	m_bNotDockingInfo = false;
	m_bShowStatusBar = false;
	m_bToolNMenuBar = false;
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), 0);
	// [LGLS 2026-09-12] 제목줄·리본 툴팁 ini
	m_nTitleOfs = 0; m_nTitleBuild = -1; m_nTitleDb = -1; m_nTitlePath = -1; m_nRibbonTip = -1; m_bRibbonTipApplied = FALSE;
}
CMainFrame::~CMainFrame()
{
}
#define LIGHT_SKY	RGB(0x99, 0xFF, 0xFF)
#define NAVY		RGB(0x00, 0x00, 0x80)
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	DockPane(&m_wndToolBar);
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1){	return -1;	}
	ExcuteTheme();
	if (!m_wndRibbonBar.Create(this)){		return -1;		}

	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//if (!m_wndStatusBar.Create(this) ||
	//	!m_wndStatusBar.SetIndicators(indicators,
	//	  sizeof(indicators)/sizeof(UINT)))
	//{
	//	TRACE0("Failed to create status bar\n");
	//	return -1;      // fail to create
	//}

 	
	CRect rect;
	//this->GetClientRect(&rect);
	GetDesktopWindow()->GetWindowRect(&rect);
	int nWindowSizeX = rect.Width() - 4;
	int nWindowSizeY = rect.Height() - 80;

    // [LGLS 2026-09-08] 최초 배치도 STATUS_POS 를 따른다.
    LayoutStatusBar(nWindowSizeX, nWindowSizeY + 40);

	//m_wndStatusBar.AddPane(ID_INDICATOR_MODE, 1);
	
    //m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NORMAL|SBPS_STRETCH, rect.Width()/4);  
    //m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_CAPS, SBPS_NORMAL|SBPS_STRETCH, rect.Width()/4);  
	//m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_NUM, SBPS_NORMAL|SBPS_STRETCH, rect.Width()/4);  
	//m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_SCRL, SBPS_NORMAL|SBPS_STRETCH, 30);  

    //m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_CAPS, SBPS_NORMAL|SBPS_STRETCH, rect.Width()/4);  
	//m_wndStatusBar.SetPaneText(1, _T("TEST"));
	//m_wndStatusBar.SetPaneInfo(ID_INDICATOR_CAPS, _T("TEST"), LIGHT_SKY, NAVY);
	//m_wndStatusBar.SetBackColor(ID_INDICATOR_CAPS, RED);
	//
	//
	//m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_NUM, SBPS_NORMAL, rect.Width()/4);  
	//m_wndStatusBar.SetPaneText(2, _T("TE"));


	InitializeRibbonMenu();
	AddStatusBarPane();
	CSplashWnd::ShowSplashScreen(this);

	//m_hIcon = LoadIcon(::AfxGetInstanceHandle(), _T("WCS.exe"));
	SetIcon(Global.m_hIcon[IDX_ICON_MAX-1][2], TRUE);

	// [LGLS 2026-09-12] 제목줄 마퀴(0.2초) + 리본 툴팁 지연 적용. 첫 틱에서 [Title]/[RibbonMenu] 를 읽는다.
	SetTimer(TIMER_TITLE_MARQUEE, 200, NULL);

	return TRUE;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	cs.cx = 1024;
	cs.cy = 768;

	return TRUE;
}

// CMainFrame 진단
//
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기
//
void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 메뉴를 검색합니다. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}
BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 기본 클래스가 실제 작업을 수행합니다.

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::MakeSureFileExists(CString strFile)
{
	if ( strFile.IsEmpty() )		return FALSE;

	if ( strFile.Right(1) == _T("\\") )
		return FALSE;

	if ( GetFileAttributes(strFile) == 0xFFFFFFFF )
		return FALSE;

	return TRUE;
}
void CMainFrame::DisplayMessage(CString strMsg)
{
}
void CMainFrame::UpdateCommStatus(int nIndex, UINT unID, char Status)
{
	//CString strAppPath = _T("");
	//CString chrFileName = _T("");
	//
	//strAppPath.Format(_T("%s"),chrFileName);
	//strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_status\\");
	////strExtension = _T(".png");
	//strExtension = _T(".ico");
	//
	//m_BtnStatusCv1 = new CMFCRibbonButton(ID_STATUS_CV, _T("CV1"), HICONFromPATH(GetConcatPath(strAppPath, _T("Trffc10b"), strExtension)), TRUE);
	//m_BtnStatusCv1->SetAlwaysLargeImage();
	//
	//m_BtnStatusCv1->SetImageIndex(IDI_ICON_STATUS_OK, true);
}
int CMainFrame::GetIndex(char Status) 
{
	switch (Status)
	{
	case COMM_STA_SEND:			return 13;
	case COMM_STA_RECEIVE:		return 14;
	case COMM_STA_STOP:			return 15;
	default:					return 15;
	}
}



void CMainFrame :: OnStyleRibboninterface ()
{
	// 리본 생성은 한 번만 수행됩니다.
	// 리본 컨트롤을 사용하여 리본 리소스를로드 
	// 첨부합니다.
	if ( nullptr == m_wndRibbonBar.m_hWnd)
	{
		m_wndRibbonBar.Create (this);
		m_wndRibbonBar.LoadFromResource (IDR_MAINFRAME_RIBBON);
	}

	// 리본을 표시하고 메뉴 / 툴바를 숨 깁니다.
	m_wndRibbonBar.ShowPane ( TRUE , TRUE , TRUE );

	// 크기와 위치를 변경하지 않고 프레임 윈도우를 강제로 다시 그립니다.
	CRect rectThis;
	GetWindowRect (& rectThis);
	SetWindowPos (
		& wndTop, 
		rectThis.left, 
		rectThis.top, 
		rectThis.Width (), 
		rectThis.Height (), 
		SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOZORDER
		);
}


void CMainFrame :: OnRbnBtnToolbarInterface ()
{
	// 메뉴 / 도구 모음을 표시하고 리본을 숨 깁니다.
	m_wndRibbonBar.ShowPane ( FALSE , FALSE , FALSE );

	// 크기와 위치를 변경하지 않고 프레임 윈도우를 강제로 다시 그립니다.
	CRect rectThis;
	GetWindowRect (& rectThis);
	SetWindowPos (
		& wndTop, 
		rectThis.left, 
		rectThis.top, 
		rectThis.Width (), 
		rectThis.Height (), 
		SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOZORDER
		);
}

HICON CMainFrame::HICONFromPATH(CString pstrPath)
{

	CImage image;
	image.Load(pstrPath);
	if(image == NULL)
	{
		return Global.GetIcon(Global.ICO_CV_ON);
	}
	CBitmap bitmap;
	bitmap.Attach(image.Detach());
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);


	HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL), 
		bmp.bmWidth, bmp.bmHeight);

	ICONINFO ii = {0};
	ii.fIcon    = TRUE;
	ii.hbmColor = bitmap;
	ii.hbmMask  = hbmMask;
	

	HICON hIcon = ::CreateIconIndirect(&ii);
	::DeleteObject(hbmMask);

	return hIcon;
}
CString CMainFrame::GetConcatPath(CString pstrAppPath, CString pstrFileName, CString pstrExtension)
{
	CString strReturn = _T("");
	strReturn.Format(_T("%s%s%s"), pstrAppPath, pstrFileName, pstrExtension);
	return strReturn;
}

CString CMainFrame::GetConcatPath_LANG(CString pstrAppPath, CString pstrFileName, CString pstrExtension, EN_LANG penLang)
{
	CString strReturn = _T("");
	if(penLang == EN_LANG::EN_KOR)
	{
		strReturn.Format(_T("%s%s%s%s"), pstrAppPath, pstrFileName, _T("_K"), pstrExtension);
		return strReturn;
	}
	if(penLang == EN_LANG::EN_ENG)
	{
		strReturn.Format(_T("%s%s%s%s"), pstrAppPath, pstrFileName, _T("_E"), pstrExtension);
		return strReturn;
	}
	if(penLang == EN_LANG::EN_CHIN)
	{
		strReturn.Format(_T("%s%s%s%s"), pstrAppPath, pstrFileName, _T("_C"), pstrExtension);
		return strReturn;
	}
	if(penLang == EN_LANG::EN_HUN)
	{
		strReturn.Format(_T("%s%s%s%s"), pstrAppPath, pstrFileName, _T("_H"), pstrExtension);
		return strReturn;
	}
	strReturn.Format(_T("%s%s%s%s"), pstrAppPath, _T("BLIND"), _T("_E"), _T(".png"));
	return _T("");
}

void CMainFrame::InitializeRibbonMenu(EN_LANG penLang)
{

	m_MainButton.SetImage(IDB_LOGO_SKI);
	m_MainButton.SetToolTipText(_T("File"));
	m_MainButton.SetText(_T("\nf"));

	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\");

	CString strCMainframeConfigFileName[4];
	CString strExtension = _T(".png");
	strCMainframeConfigFileName[0] = _T("CMAINFRAME_CONFIG");
	strCMainframeConfigFileName[1] = _T("");
	strCMainframeConfigFileName[2] = _T("");
	strCMainframeConfigFileName[3] = _T("");

	m_MainButton.SetImage(IDB_LOGO_SKI);
	m_MainButton.SetToolTipText(_T("File"));
	m_MainButton.SetText(_T("\nf"));
	//m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize(45, 45));
	AddCategoryWCS();
	AddCategoryMANUAL();
	AddCategoryLOG();		// [LGLS 2026-09-01] 안에 [알람] 패널 포함(독립 카테고리 폐지)
	//AddCategoryUSER();
	//AddCategorySTATUS();
	AddCommToTabs();	// [LGLS 2026-09-08] 리본 오른쪽 끝 [통신] 표시(항상 보임)
	//RenameRibbonText();	//test
}

LRESULT CMainFrame::OnLangUpdate(WPARAM wParam, LPARAM lParam)
{
	CEcsDoc* pDoc = (CEcsDoc*)lParam;
	if(pDoc == NULL)
		return 0;

// 	CFont* pFont = new CFont();
// 	pFont->CreateFont( 
// 		10,
// 		10,
// 		0, // nEscapement 
// 		0, // nOrientation 
// 		FW_BOLD, // nWeight 
// 		0, // bItalic 
// 		0, // bUnderline 
// 		0, // cStrikeOut 
// 		0, // nCharSet 
// 		OUT_DEFAULT_PRECIS, // nOutPrecision 
// 		0, // nClipPrecision 
// 		DEFAULT_QUALITY, // nQuality 
// 		DEFAULT_PITCH | FF_DONTCARE, // nPitchAndFamily 
// 		//strFontPath
// 		Global.GetFontName((int)pDoc->m_enLang)
// 		); 
// 
// 	m_wndRibbonBar.SetFont(pFont);

	RenameRibbonText(pDoc->m_enLang);			// TEST

	m_wndRibbonBar.ForceRecalcLayout();			// TEST
	ApplyRibbonToolTipIni(TRUE);		// [LGLS 2026-09-12] 탭·패널 이름이 바뀐 뒤 경로 툴팁([RibbonMenu] ToolTip)을 다시 붙인다

	return 0;
}

void CMainFrame::AddCategoryWCS()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_config\\");
	CString strExtension = _T(".png");

	CString TEST_WH_TYP = strAppPath.Left(strAppPath.ReverseFind('\\'));
	TEST_WH_TYP = TEST_WH_TYP.Left(TEST_WH_TYP.ReverseFind('\\'));
	//TEST_WH_TYP = TEST_WH_TYP.Left(TEST_WH_TYP.ReverseFind('\\'));
	//TEST_WH_TYP = TEST_WH_TYP.Left(TEST_WH_TYP.ReverseFind('\\'));
	//TEST_WH_TYP = TEST_WH_TYP.Left(TEST_WH_TYP.ReverseFind('\\'));
	CString strWH_TYP = _T("G동");
	
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("WH"), _T("WH_TYP"), _T("10"), szTemp, _MAX_PATH, ECS_INI_FILE);
	strWH_TYP.Format(_T("%s"), szTemp); 


	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("ECS"), IDB_LOGO_ECS, IDB_LOGO_ECS);

	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_config\\");
	strExtension = _T(".png");

	CMFCRibbonPanel* pPanelConfig = pCategory->AddPanel(_T("CONFIG"));

	/*CMFCRibbonButton* pBtnSign = new CMFCRibbonButton(ID_SYSTEM_LOGIN, _T("SIGN"), HICONFromPATH(GetConcatPath(strAppPath, _T("sign"), strExtension)), TRUE);
	pBtnSign->SetAlwaysLargeImage();
	pPanelConfig->Add(pBtnSign);*/

	CMFCRibbonButton* pBtnEqpSuspend = new CMFCRibbonButton(ID_SYSTEM_EQP_SUSPEND, _T("EQP"), HICONFromPATH(GetConcatPath(strAppPath, _T("eqpsuspend"), strExtension)), TRUE);
	pBtnEqpSuspend->SetAlwaysLargeImage();
	pPanelConfig->Add(pBtnEqpSuspend);

	CMFCRibbonButton* pBtnLogDelete = new CMFCRibbonButton(IDD_CONFIG_LOG_DELETE, _T("LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("configlogdelete"), strExtension)), TRUE);
	pBtnLogDelete->SetAlwaysLargeImage();
	pPanelConfig->Add(pBtnLogDelete);

	CMFCRibbonButton* pBtnConfigStatus = new CMFCRibbonButton(IDD_CONFIG_STATUS, _T("CONF"), HICONFromPATH(GetConcatPath(strAppPath, _T("configstatus"), strExtension)), TRUE);
	pBtnConfigStatus->SetAlwaysLargeImage();
	pPanelConfig->Add(pBtnConfigStatus);

	// [LGLS 2026-09-03] [INI 열기] : 접속/화면 설정 파일(Ecs.ini)을 메모장으로 연다.
	//   Ecs.ini [MENU] INI_MENU=1/0 으로 표시 여부 선택(기본 1=표시)
	if (::GetPrivateProfileInt(_T("MENU"), _T("INI_MENU"), 1, ECS_INI_FILE) != 0)
	{
		CMFCRibbonButton* pBtnIniOpen = new CMFCRibbonButton(ID_CONFIG_INI_OPEN, _T("INI 열기"),
			HICONFromPATH(GetConcatPath(strAppPath, _T("iniopen"), strExtension)), TRUE);
		pBtnIniOpen->SetAlwaysLargeImage();
		pPanelConfig->Add(pBtnIniOpen);
	}

	// [LGLS 2026-08-12] 사용자 메뉴는 Ecs.ini [MENU] USER_MENU=1/0 으로 표시 여부 선택(기본 1=표시)
	if (::GetPrivateProfileInt(_T("MENU"), _T("USER_MENU"), 1, ECS_INI_FILE) != 0)
	{
		// [LGLS 2026-08-05] 사용자 관리 대화상자는 메뉴가 없어 접근 자체가 불가능했다 -> CONFIG 패널에 추가.
		//   ID_USER_USER 는 이미 ON_COMMAND_RANGE(ID_USER_USER, ID_USER_GROUP) 로 라우팅되어 있다.
		//   RenameRibbonText 는 이 패널의 0~2번만 다시 이름 붙이므로 여기 캡션이 그대로 유지된다.
		CMFCRibbonButton* pBtnUserUser = new CMFCRibbonButton(ID_USER_USER, _T("사용자"), HICONFromPATH(GetConcatPath(strAppPath, _T("sign"), strExtension)), TRUE);
		pBtnUserUser->SetAlwaysLargeImage();
		pPanelConfig->Add(pBtnUserUser);
	}

	//
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_view\\");
	strExtension = _T(".png");

	CMFCRibbonPanel* pPanelView = pCategory->AddPanel(_T("VIEW"));
	//CMFCRibbonButton* pBtnRack = new CMFCRibbonButton(ID_VIEW_USAGE, _T("USAGE"), HICONFromPATH(GetConcatPath(strAppPath, _T("usage"), strExtension)), TRUE);
	//pBtnRack->SetAlwaysLargeImage();
	//pPanelView->Add(pBtnRack);

	CMFCRibbonButton* pBtnJob = new CMFCRibbonButton(ID_VIEW_JOBLIST, _T("JOB-INFO"), HICONFromPATH(GetConcatPath(strAppPath, _T("job"), strExtension)), TRUE);
	pBtnJob->SetAlwaysLargeImage();
	pPanelView->Add(pBtnJob);

	// [LGLS 2026-09-10] 공PLT작업 버튼 폐지 - 이 현장에서 쓰지 않는다(사용자 결정).

	CMFCRibbonButton* pBtnSearch = new CMFCRibbonButton(ID_VIEW_SEARCH, _T("SEARCH"), HICONFromPATH(GetConcatPath(strAppPath, _T("search"), strExtension)), TRUE);
	pBtnSearch->SetAlwaysLargeImage();
	pPanelView->Add(pBtnSearch);

	//

	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_monitor\\");
	strExtension = _T(".png");

	CMFCRibbonPanel* pPanelMonitor = pCategory->AddPanel(_T("WAREHOUSE MONITOR"));


	if(strWH_TYP == _T("10"))
	{
		// [LGLS 2026-07-20] 1F 버튼 제거(사용자 요청) ? 표시 모드 버튼(작업번호/트랙번호/제품정보)으로 대체
		//CMFCRibbonButton* pBtn1f = new CMFCRibbonButton(ID_MONITORING_1F, _T("1F"), HICONFromPATH(GetConcatPath(strAppPath, _T("1f"), strExtension)), TRUE);
		//pBtn1f->SetAlwaysLargeImage();
		//pPanelMonitor->Add(pBtn1f);

		// [LGLS 2026-07-19] 트랙 표시 모드 버튼 3종 (검은 배경 + 흰 글자 아이콘)
		CMFCRibbonButton* pBtnViewJobNo = new CMFCRibbonButton(ID_MONITORING_VIEW_JOBNO, _T("작업번호"), HICONFromPATH(GetConcatPath(strAppPath, _T("j"), strExtension)), TRUE);
		pBtnViewJobNo->SetAlwaysLargeImage();
		pPanelMonitor->Add(pBtnViewJobNo);

		CMFCRibbonButton* pBtnViewTrackNo = new CMFCRibbonButton(ID_MONITORING_VIEW_TRACKNO, _T("트랙번호"), HICONFromPATH(GetConcatPath(strAppPath, _T("t"), strExtension)), TRUE);
		pBtnViewTrackNo->SetAlwaysLargeImage();
		pPanelMonitor->Add(pBtnViewTrackNo);

		// [LGLS 2026-09-03] 제품정보 : Ecs.ini [MENU] PRODINFO_MENU=1/0 으로 표시 여부 선택(기본 1=표시)
		if (::GetPrivateProfileInt(_T("MENU"), _T("PRODINFO_MENU"), 1, ECS_INI_FILE) != 0)
		{
			CMFCRibbonButton* pBtnViewProd = new CMFCRibbonButton(ID_MONITORING_VIEW_PRODINFO, _T("제품정보"), HICONFromPATH(GetConcatPath(strAppPath, _T("p"), strExtension)), TRUE);
			pBtnViewProd->SetAlwaysLargeImage();
			pPanelMonitor->Add(pBtnViewProd);
		}

		//CMFCRibbonButton* pBtn2f = new CMFCRibbonButton(ID_MONITORING_2F, _T("2F"), HICONFromPATH(GetConcatPath(strAppPath, _T("2f"), strExtension)), TRUE);
		//pBtn2f->SetAlwaysLargeImage();
		//pPanelMonitor->Add(pBtn2f);
		//
		//CMFCRibbonButton* pBtnAssemble = new CMFCRibbonButton(ID_MONITORING_ASSEMBLE, _T("ASSEMBLE"), HICONFromPATH(GetConcatPath(strAppPath, _T("assemble"), strExtension)), TRUE);
		//pBtnAssemble->SetAlwaysLargeImage();
		//pPanelMonitor->Add(pBtnAssemble);
	}
	//else
	//{
	//	CMFCRibbonButton* pBtnAssemble = new CMFCRibbonButton(ID_MONITORING_COMP, _T("COMP"), HICONFromPATH(GetConcatPath(strAppPath, _T("cmp"), strExtension)), TRUE);
	//	pBtnAssemble->SetAlwaysLargeImage();
	//	pPanelMonitor->Add(pBtnAssemble);
	//}


	//strAppPath.Format(_T("%s"),chrFileName);
	//strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_language\\");
	//strExtension = _T(".ico");
	//
	//CMFCRibbonPanel* pPanelLanguage = pCategory->AddPanel(_T("NATION"));
	//
	//CMFCRibbonButton* pBtnKOR = new CMFCRibbonButton(ID_LANGUAGE_KOR, _T("KOREA"), HICONFromPATH(GetConcatPath(strAppPath, _T("kor"), strExtension)), TRUE);
	//pBtnKOR->SetAlwaysLargeImage();
	//pPanelLanguage->Add(pBtnKOR);

	//CMFCRibbonButton* pBtnENG = new CMFCRibbonButton(ID_LANGUAGE_ENGLISH, _T("US"), HICONFromPATH(GetConcatPath(strAppPath, _T("eng"), strExtension)), TRUE);
	//pBtnENG->SetAlwaysLargeImage();
	//pPanelLanguage->Add(pBtnENG);

	/*CMFCRibbonButton* pBtnCHIN = new CMFCRibbonButton(ID_LANGUAGE_CHIN, _T("CHINA"), HICONFromPATH(GetConcatPath(strAppPath, _T("chin"), strExtension)), TRUE);
	pBtnCHIN->SetAlwaysLargeImage();
	pPanelLanguage->Add(pBtnCHIN);*/

	//CMFCRibbonButton* pBtnHUN = new CMFCRibbonButton(ID_LANGUAGE_HUNGARIAN, _T("HUNGARY"), HICONFromPATH(GetConcatPath(strAppPath, _T("hun"), strExtension)), TRUE);
	//pBtnHUN->SetAlwaysLargeImage();
	//pPanelLanguage->Add(pBtnHUN);

	// [LGLS 2026-09-09] [판넬 보기] 그룹 : 판넬 3개를 각각 켜고 끈다(선택=표시).
	//   종전 [UI모드](대화상자/판넬 모드 2버튼)를 대체한다.
	//   Ecs.ini [MENU] UIMODE_MENU=1/0 으로 그룹 표시 여부 선택(기본 1=표시)
	if (::GetPrivateProfileInt(_T("MENU"), _T("UIMODE_MENU"), 1, ECS_INI_FILE) != 0)
	{
		// 이 시점의 strAppPath 는 직전 패널(창고 모니터링) 폴더라 다시 지정한다.
		strAppPath.Format(_T("%s"), chrFileName);
		strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_view\\");
		CMFCRibbonPanel* pPanelPaneView = pCategory->AddPanel(_T("판넬 보기"));

		CMFCRibbonButton* pBtnPaneJob = new CMFCRibbonButton(ID_PANE_SHOW_JOB, _T("작업정보"),
			HICONFromPATH(GetConcatPath(strAppPath, _T("pane_job"), strExtension)), TRUE);
		pBtnPaneJob->SetAlwaysLargeImage();
		pPanelPaneView->Add(pBtnPaneJob);

		CMFCRibbonButton* pBtnPaneInfo = new CMFCRibbonButton(ID_PANE_SHOW_INFO, _T("상세정보"),
			HICONFromPATH(GetConcatPath(strAppPath, _T("pane_info"), strExtension)), TRUE);
		pBtnPaneInfo->SetAlwaysLargeImage();
		pPanelPaneView->Add(pBtnPaneInfo);

		CMFCRibbonButton* pBtnPaneVeh = new CMFCRibbonButton(ID_PANE_SHOW_VEH, _T("설비반송"),
			HICONFromPATH(GetConcatPath(strAppPath, _T("pane_veh"), strExtension)), TRUE);
		pBtnPaneVeh->SetAlwaysLargeImage();
		pPanelPaneView->Add(pBtnPaneVeh);
	}

	// [LGLS 2026-09-06] [처리] 그룹 : 스케줄러의 처리 방식과 관련된 메뉴를 모은다.
	//   Ecs.ini [MENU] PROCESS_MENU=1/0 으로 그룹 표시 여부 선택(기본 1=표시)
	//   그룹 안의 버튼은 각자의 키로 따로 숨길 수 있다(예: AUTOTIME_MENU).
	//   표시할 버튼이 하나도 없으면 빈 그룹이 생기지 않도록 패널 자체를 만들지 않는다.
	{
		BOOL bProcessGrp = (::GetPrivateProfileInt(_T("MENU"), _T("PROCESS_MENU"),  1, ECS_INI_FILE) != 0);
		BOOL bAutoTime   = (::GetPrivateProfileInt(_T("MENU"), _T("AUTOTIME_MENU"), 1, ECS_INI_FILE) != 0);
		if (bProcessGrp && bAutoTime)
		{
			// 아이콘(autotime.png)은 mainframe_config 폴더에 있다. 직전 패널에서 넘어온
			//   strAppPath 는 mainframe_view 라 그대로 쓰면 아이콘을 못 찾는다(UI모드에서 겪은 문제).
			strAppPath.Format(_T("%s"), chrFileName);
			strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_config\\");
			CMFCRibbonPanel* pPanelProcess = pCategory->AddPanel(_T("처리"));

			// [시간 기반 자동 처리] : 스케줄러가 설비 신호 대신 경과시간으로 완료를 추정하는
			//   처리의 사용 여부를 켜고 끈다. 선택 = 사용, 해제 = 사용 안 함(설비 신호로만 처리).
			//   상태는 DB(COMMON_CODE SCH_OPT/AUTO_TIME)에 있어 IO_TASK 가 같은 값을 본다.
			CMFCRibbonButton* pBtnAutoTime = new CMFCRibbonButton(ID_CONFIG_AUTO_TIME, _T("시간기반 자동처리"),
				HICONFromPATH(GetConcatPath(strAppPath, _T("autotime"), strExtension)), TRUE);
			pBtnAutoTime->SetAlwaysLargeImage();
			pBtnAutoTime->SetToolTipText(_T("시간 기반 자동 처리"));
			pBtnAutoTime->SetDescription(_T("설비 완료 신호가 오지 않아도 경과시간으로 완료를 추정할지 선택합니다."));
			pPanelProcess->Add(pBtnAutoTime);
		}
	}

	AddCommPanel(pCategory);	// [LGLS 2026-09-08] 모드 2 : [통신] 그룹
}
	

void CMainFrame::AddCategoryMANUAL()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_manual\\");
	CString strExtension = _T(".png");

	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("MANUAL"), IDB_LOGO_ECS, IDB_LOGO_ECS);
	CMFCRibbonPanel* pPanelManual = pCategory->AddPanel(_T("MANUAL"));

	CMFCRibbonButton* pBtnManualJob = new CMFCRibbonButton(ID_MANUAL_JOB, _T("JOB"), HICONFromPATH(GetConcatPath(strAppPath, _T("job"), strExtension)), TRUE);
	pBtnManualJob->SetAlwaysLargeImage();
	pPanelManual->Add(pBtnManualJob);

	CMFCRibbonButton* pBtnManualSc = new CMFCRibbonButton(ID_MANUAL_SC, _T("SC"), HICONFromPATH(GetConcatPath(strAppPath, _T("sc"), strExtension)), TRUE);
	pBtnManualSc->SetAlwaysLargeImage();
	pPanelManual->Add(pBtnManualSc);

	//[LGLS 공PLT→RTV 대체] EMPTY 버튼 제거
	//CMFCRibbonButton* pBtnManualEmpty = new CMFCRibbonButton(IDD_MANUAL_EMPTY, _T("EMPTY"), HICONFromPATH(GetConcatPath(strAppPath, _T("empty"), strExtension)), TRUE);
	//pBtnManualEmpty->SetAlwaysLargeImage();
	//pPanelManual->Add(pBtnManualEmpty);

	CMFCRibbonButton* pBtnManualRtv = new CMFCRibbonButton(ID_MANUAL_RTV, _T("RTV"), HICONFromPATH(GetConcatPath(strAppPath, _T("rtv"), strExtension)), TRUE);
	pBtnManualRtv->SetAlwaysLargeImage();
	pPanelManual->Add(pBtnManualRtv);

	// [LGLS 2026-08-13] 반자동 TEST 그룹 - Ecs.ini [MENU] SEMITEST_MENU=1/0 으로 표시 선택(기본 1=표시)
	if (::GetPrivateProfileInt(_T("MENU"), _T("SEMITEST_MENU"), 1, ECS_INI_FILE) != 0)
	{
		CMFCRibbonPanel* pPanelSemiTest = pCategory->AddPanel(_T("반자동 TEST"));

		CMFCRibbonButton* pBtnSemiTest = new CMFCRibbonButton(ID_MANUAL_SEMITEST, _T("반자동 TEST"), HICONFromPATH(GetConcatPath(strAppPath, _T("job"), strExtension)), TRUE);
		pBtnSemiTest->SetAlwaysLargeImage();
		pPanelSemiTest->Add(pBtnSemiTest);

		CMFCRibbonButton* pBtnTestClear = new CMFCRibbonButton(ID_MANUAL_TESTCLEAR, _T("TEST 초기화"), HICONFromPATH(GetConcatPath(strAppPath, _T("empty"), strExtension)), TRUE);
		pBtnTestClear->SetAlwaysLargeImage();
		pPanelSemiTest->Add(pBtnTestClear);
	}

	AddCommPanel(pCategory);	// [LGLS 2026-09-08] 모드 2 : [통신] 그룹
}

void CMainFrame::AddCategoryLOG()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_log\\");
	CString strExtension = _T(".png");

	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("LOG"), IDB_LOGO_ECS, IDB_LOGO_ECS);
	CMFCRibbonPanel* pPanelLog = pCategory->AddPanel(_T("LOG"));

	CMFCRibbonButton* pBtnIO_LOG = new CMFCRibbonButton(ID_LOG_IO, _T("IO_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("iolog"), strExtension)), TRUE);
	pBtnIO_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnIO_LOG);

	CMFCRibbonButton* pBtnMES_LOG = new CMFCRibbonButton(ID_LOG_MES, _T("MES_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("meslog"), strExtension)), TRUE);
	pBtnMES_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnMES_LOG);

	CMFCRibbonButton* pBtnEQP_HIS_LOG = new CMFCRibbonButton(ID_LOG_EQP_HIS, _T("EQP_HIS_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("eqphislog"), strExtension)), TRUE);
	pBtnEQP_HIS_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnEQP_HIS_LOG);

	// [LGLS 2026-07-22] WC(BCR)로그 버튼 제거 ? 대체 시스템 미사용

	CMFCRibbonButton* pBtnCLIENT_LOG = new CMFCRibbonButton(ID_LOG_CLIENT, _T("CLIENT_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("clientlog"), strExtension)), TRUE);
	pBtnCLIENT_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnCLIENT_LOG);

	CMFCRibbonButton* pBtnWCS_LOG = new CMFCRibbonButton(IDD_LOG_WCS_LOG_PGR, _T("WCS_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("wcslog"), strExtension)), TRUE);
	pBtnWCS_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnWCS_LOG);

	// [LGLS 2026-09-01] [알람] 그룹을 LOG 카테고리 안 별도 패널로 이동(독립 카테고리 폐지).
	//   작업 체류 경보창(CWarningDlg) 수동 표시 경로.
	CMFCRibbonPanel* pPanelAlarm = pCategory->AddPanel(_T("알람"));
	CMFCRibbonButton* pBtnAlarm = new CMFCRibbonButton(ID_ALARM_SHOW, _T("알람"),
		HICONFromPATH(GetConcatPath(strAppPath, _T("eqphislog"), strExtension)), TRUE);
	pBtnAlarm->SetAlwaysLargeImage();
	pPanelAlarm->Add(pBtnAlarm);

	//CMFCRibbonButton* pBtnEQP_LOG = new CMFCRibbonButton(ID_LOG_EQP, _T("EQP_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("eqplog"), strExtension)), TRUE);
	//pBtnEQP_LOG->SetAlwaysLargeImage();
	//pPanelLog->Add(pBtnEQP_LOG);

	//CMFCRibbonButton* pBtnCLIENT_LOG = new CMFCRibbonButton(ID_LOG_CLIENT, _T("CLIENT_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("clientlog"), strExtension)), TRUE);
	//pBtnCLIENT_LOG->SetAlwaysLargeImage();
	//pPanelLog->Add(pBtnCLIENT_LOG);

	AddCommPanel(pCategory);	// [LGLS 2026-09-08] 모드 2 : [통신] 그룹
}

void CMainFrame::RenameRibbonText(EN_LANG penLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");
	
	CMFCRibbonCategory* pCategory = m_wndRibbonBar.GetCategory(1);
	BOOL bTemp1 = FALSE;
	BOOL bTemp2 = FALSE;
	CString strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_config\\"), _T("config"), strExtension);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_Config = (CMFCRibbonPanel_Wrap*)pCategory->GetPanel(0);

	CMFCRibbonButton* pButton_sign = (CMFCRibbonButton*)pPanel_Wrap_Config->GetElement(0);
	pButton_sign->SetText(CLib::GetIniStringFromPath(strFullPath, _T("eqpsuspend"), (int)penLang));

	CMFCRibbonButton* pButton_log_delete = (CMFCRibbonButton*)pPanel_Wrap_Config->GetElement(1);
	pButton_log_delete->SetText(CLib::GetIniStringFromPath(strFullPath, _T("configlogdelete"), (int)penLang));

	CMFCRibbonButton* pButton_config_status = (CMFCRibbonButton*)pPanel_Wrap_Config->GetElement(2);
	pButton_config_status->SetText(CLib::GetIniStringFromPath(strFullPath, _T("configstatus"), (int)penLang));

	pPanel_Wrap_Config->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));

	pPanel_Wrap_Config->SetCenterColumnVert();
	pPanel_Wrap_Config->SetJustifyColumns();

	//bTemp2 = pPanel_Wrap_Config->IsJustifyColumns();
	//bTemp1 = pPanel_Wrap_Config->IsCenterColumnVert();

	
	 strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_view\\"), _T("view"), strExtension);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_View = (CMFCRibbonPanel_Wrap*)pCategory->GetPanel(1);
	pPanel_Wrap_View->SetCenterColumnVert();
	pPanel_Wrap_View->SetJustifyColumns();
	pPanel_Wrap_View->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));
	CMFCRibbonButton* pButton_Rack = (CMFCRibbonButton*)pPanel_Wrap_View->GetElement(0);
	pButton_Rack->SetText(CLib::GetIniStringFromPath(strFullPath, _T("job-info"), (int)penLang));
	// [LGLS 2026-09-10] 공PLT작업 버튼을 없앴으므로 [찾기] 가 바로 다음 자리다.
	CMFCRibbonButton* pButton_Search = (CMFCRibbonButton*)pPanel_Wrap_View->GetElement(1);

	if (pButton_Search != NULL)
		pButton_Search->SetText(CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)penLang));


	strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_monitor\\"), _T("monitor"), strExtension);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_Monitor = (CMFCRibbonPanel_Wrap*)pCategory->GetPanel(2);
	pPanel_Wrap_Monitor->SetCenterColumnVert();
	pPanel_Wrap_Monitor->SetJustifyColumns();
	pPanel_Wrap_Monitor->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));

	//MANUAL
	strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_manual\\"), _T("manual"), strExtension);
	CMFCRibbonCategory* pCategory2 = m_wndRibbonBar.GetCategory(2);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_Manual = (CMFCRibbonPanel_Wrap*)pCategory2->GetPanel(0);
	pPanel_Wrap_Manual->SetCenterColumnVert();
	pPanel_Wrap_Manual->SetJustifyColumns();
	pPanel_Wrap_Manual->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));
	CMFCRibbonButton* pBtnManualJob = (CMFCRibbonButton*)pPanel_Wrap_Manual->GetElement(0);
	pBtnManualJob->SetText(CLib::GetIniStringFromPath(strFullPath, _T("job"), (int)penLang));
	CMFCRibbonButton* pBtnManualSc = (CMFCRibbonButton*)pPanel_Wrap_Manual->GetElement(1);
	pBtnManualSc->SetText(CLib::GetIniStringFromPath(strFullPath, _T("sc"), (int)penLang));
	CMFCRibbonButton* pBtnManualRtv = (CMFCRibbonButton*)pPanel_Wrap_Manual->GetElement(2);
	pBtnManualRtv->SetText(CLib::GetIniStringFromPath(strFullPath, _T("rtv"), (int)penLang));
	//[공PLT→RTV] EMPTY 라벨 제거

	strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_log\\"), _T("log"), strExtension);
	CMFCRibbonCategory* pCategory3 = m_wndRibbonBar.GetCategory(3);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_Log = (CMFCRibbonPanel_Wrap*)pCategory3->GetPanel(0);
	pPanel_Wrap_Log->SetCenterColumnVert();
	pPanel_Wrap_Log->SetJustifyColumns();
	pPanel_Wrap_Log->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));
	CMFCRibbonButton* pBtnLogIoLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(0);
	pBtnLogIoLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("io_log"), (int)penLang));
	CMFCRibbonButton* pBtnLogMesLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(1);
	pBtnLogMesLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("mes_log"), (int)penLang));
	CMFCRibbonButton* pBtnLogEqpHisLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(2);
	pBtnLogEqpHisLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("eqphislog"), (int)penLang));
	// [LGLS 2026-07-22] WC(BCR)로그 캡션 제거(버튼 삭제에 따라 아래 인덱스 당김)
 	CMFCRibbonButton* pBtnLogClientLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(3);
 	pBtnLogClientLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("client_log"), (int)penLang));
	CMFCRibbonButton* pBtnLogWcsLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(4);
 	pBtnLogWcsLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("wcs_log"), (int)penLang));
	// [LGLS 2026-09-01] [알람] 다국어 : LOG 카테고리(인덱스 3)의 두 번째 패널.
	//   패널 이름 / 버튼 이름을 rc_resource\mainframe_alarm\alarm.ini 에서 읽는다.
	//   키 값이면 캡션을 그대로 둔다(빈 문자열 대입 금지).
	strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\\\rc_resource\\\\mainframe_alarm\\\\"), _T("alarm"), strExtension);
	CMFCRibbonPanel_Wrap* pPanelAlarm = (CMFCRibbonPanel_Wrap*)pCategory3->GetPanel(1);
	if (pPanelAlarm != NULL)
	{
		CString strAlarmCap = CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang);
		if (!strAlarmCap.IsEmpty()) pPanelAlarm->SetName(strAlarmCap);

		CMFCRibbonButton* pBtnAlarm = (CMFCRibbonButton*)pPanelAlarm->GetElement(0);
		CString strAlarmBtn = CLib::GetIniStringFromPath(strFullPath, _T("alarm"), (int)penLang);
		if (pBtnAlarm != NULL && !strAlarmBtn.IsEmpty()) pBtnAlarm->SetText(strAlarmBtn);
	}

// 	CMFCRibbonButton* pBtnLogEqpLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(1);
// 	pBtnLogEqpLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("eqp_log"), (int)penLang));
// 	CMFCRibbonButton* pBtnLogClientLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(2);
// 	pBtnLogClientLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("client_log"), (int)penLang));
// 	CMFCRibbonButton* pBtnLogJobHisLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(5);
// 	pBtnLogJobHisLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("jobhislog"), (int)penLang));

	//로그인 기능 없애서 유저 항목 제외
	//strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_user\\"), _T("user"), strExtension);
	//CMFCRibbonCategory* pCategory4 = m_wndRibbonBar.GetCategory(4);
	//CMFCRibbonPanel_Wrap* pPanel_Wrap_User = (CMFCRibbonPanel_Wrap*)pCategory4->GetPanel(0);
	//pPanel_Wrap_User->SetCenterColumnVert();
	//pPanel_Wrap_User->SetJustifyColumns();
	//pPanel_Wrap_User->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));
	//
	//CMFCRibbonButton* pBtnUserUser = (CMFCRibbonButton*)pPanel_Wrap_User->GetElement(0);
	//pBtnUserUser->SetText(CLib::GetIniStringFromPath(strFullPath, _T("user"), (int)penLang));

	m_wndRibbonBar.ForceRecalcLayout();	

	bTemp1 = pPanel_Wrap_Config->IsJustifyColumns();
	bTemp2 = pPanel_Wrap_Config->IsCenterColumnVert();

}

void CMainFrame::ExcuteTheme()
{	
	CDockingManager::SetDockingMode (DT_SMART);
	CTabbedPane::m_StyleTabWnd = CMFCTabCtrl::STYLE_3D;
	CMFCVisualManagerOffice2007::SetStyle (CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));	
	CDockingManager::SetDockingMode (DT_SMART);
	CDockingManager* pDockManager = GetDockingManager ();
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->AdjustPaneFrames ();
	}
	CTabbedPane::ResetTabs ();
	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	theApp.WriteInt (_T("ApplicationLook"), m_nAppLook);
}

// [LGLS 2026-09-01] 리본 [작업정보] 진입점 : 우측 도킹 판넬 생성/토글
//   구 SPL EcsSv CreateDockingBar 를 MFC Feature Pack(CDockablePane)으로 재구현.
void CMainFrame::TogglePanelBars(CEcsDoc* pDoc)
{
	BOOL bShow = TRUE;
	if (m_bPanelBarsCreated)
		bShow = !m_JobPane.IsVisible();
	ShowPanelBars(pDoc, bShow);
}

// [LGLS 2026-09-01] 우측 도킹 판넬 생성/표시 (구 SPL EcsSv CreateDockingBar 를 CDockablePane 으로 재구현)
void CMainFrame::ShowPanelBars(CEcsDoc* pDoc, BOOL bShow)
{
	if (!m_bPanelBarsCreated)
	{
		if (!bShow || pDoc == NULL)
			return;

		EnableDocking(CBRS_ALIGN_ANY);

		m_pDoc = pDoc;
		m_PanelJobDlg.m_pDoc  = pDoc;
		m_PanelInfoDlg.m_pDoc = pDoc;
		m_JobPane.m_pDlg  = &m_PanelJobDlg;   m_JobPane.m_nIDD  = IDD_PANEL_JOB;
		m_InfoPane.m_pDlg = &m_PanelInfoDlg;  m_InfoPane.m_nIDD = IDD_PANEL_INFO;
		m_PanelVehDlg.m_pDoc = pDoc;
		m_VehPane.m_pDlg = &m_PanelVehDlg;    m_VehPane.m_nIDD = IDD_PANEL_VEH;

		// [LGLS 2026-09-09] 판넬 배치 변경(사용자 요청)
		//   작업 정보 : 상단에 가로로 넓게 도킹 (작업 목록이 열 수가 많아 가로가 필요하다)
		//   상세정보 / Crane & Vehicle : 오른쪽에, 작업 정보 판넬 아래로 위아래 배치
		DWORD dwStyleTop   = WS_CHILD | WS_VISIBLE | CBRS_TOP   | CBRS_FLOAT_MULTI;
		DWORD dwStyleRight = WS_CHILD | WS_VISIBLE | CBRS_RIGHT | CBRS_FLOAT_MULTI;

		if (!m_JobPane.Create(_T("작업 정보"), this, CRect(0, 0, 1200, 240), TRUE,
				ID_PANE_JOB, dwStyleTop, AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE | AFX_CBRS_CLOSE))
			return;
		if (!m_InfoPane.Create(_T("상세정보"), this, CRect(0, 0, 480, 460), TRUE,
				ID_PANE_INFO, dwStyleRight, AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE | AFX_CBRS_CLOSE))
			return;
		if (!m_VehPane.Create(_T("Crane && Vehicle 반송 현황"), this, CRect(0, 0, 480, 260), TRUE,
				ID_PANE_VEH, dwStyleRight, AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE | AFX_CBRS_CLOSE))
			return;

		m_JobPane.EnableDocking(CBRS_ALIGN_ANY);
		m_InfoPane.EnableDocking(CBRS_ALIGN_ANY);
		m_VehPane.EnableDocking(CBRS_ALIGN_ANY);

		DockPane(&m_JobPane);            // 상단 전폭
		RecalcLayout();
		DockPane(&m_InfoPane);           // 오른쪽 (상단 판넬 아래로 자리잡는다)
		RecalcLayout();
		m_VehPane.DockToWindow(&m_InfoPane, CBRS_ALIGN_BOTTOM);   // 상세정보 아래

		m_bPanelBarsCreated = TRUE;
		RecalcLayout();
		return;
	}

	m_JobPane.ShowPane(bShow, FALSE, TRUE);
	m_InfoPane.ShowPane(bShow, FALSE, TRUE);
	m_VehPane.ShowPane(bShow, FALSE, TRUE);
	RecalcLayout();
}

// [LGLS 2026-09-01] 상세정보 판넬 캡션 변경 (탭 선택에 따라 "CV 상세정보" 등)
void CMainFrame::SetInfoPaneTitle(CString strTitle)
{
	if (m_bPanelBarsCreated && ::IsWindow(m_InfoPane.m_hWnd))
		m_InfoPane.SetWindowText(strTitle);
}

// ---------------------------------------------------------------------------
// [LGLS 2026-09-09] [판넬 보기] : 판넬 3개를 각각 켜고 끈다.
//   버튼이 눌린 상태(체크) = 그 판넬이 보이는 상태다.
//   판넬은 처음 켤 때 3개를 한꺼번에 만들고(도킹 위치를 함께 잡아야 한다),
//   그 자리에서 누르지 않은 둘은 접어 둔다.
// ---------------------------------------------------------------------------
CDockablePane* CMainFrame::PaneOf(UINT nID)
{
	switch (nID)
	{
	case ID_PANE_SHOW_JOB:  return &m_JobPane;
	case ID_PANE_SHOW_INFO: return &m_InfoPane;
	case ID_PANE_SHOW_VEH:  return &m_VehPane;
	default: break;
	}
	return NULL;
}

void CMainFrame::OnPaneShow(UINT nID)
{
	if (m_pDoc == NULL) m_pDoc = (CEcsDoc*)GetActiveDocument();
	if (m_pDoc == NULL) return;

	if (!m_bPanelBarsCreated)
	{
		ShowPanelBars(m_pDoc, TRUE);          // 3개 생성 + 도킹
		if (!m_bPanelBarsCreated) return;     // 생성 실패

		// 설비를 눌렀을 때 상세정보 판넬로 보내려면 판넬 모드여야 한다(EcsView 참조).
		m_bUiModePanel = TRUE;

		// 누른 것만 남기고 나머지는 접는다.
		m_JobPane.ShowPane(nID == ID_PANE_SHOW_JOB,   FALSE, TRUE);
		m_InfoPane.ShowPane(nID == ID_PANE_SHOW_INFO, FALSE, TRUE);
		m_VehPane.ShowPane(nID == ID_PANE_SHOW_VEH,   FALSE, TRUE);
		RecalcLayout();

		// [LGLS 2026-09-11] 작업정보 판넬을 켜면 작업정보 창도 함께 띄운다(사용자 요청).
		//   판넬은 그대로 동작하고 창이 하나 더 뜬다. 끌 때는 창을 닫지 않는다
		//   - 판넬을 접고 창만 보는 쓰임이 있다.
		if (nID == ID_PANE_SHOW_JOB)
			m_pDoc->OpenJobListDialog();
		return;
	}

	CDockablePane* pPane = PaneOf(nID);
	if (pPane == NULL || !::IsWindow(pPane->GetSafeHwnd())) return;

	BOOL bShow = !pPane->IsVisible();
	pPane->ShowPane(bShow, FALSE, TRUE);
	if (bShow) m_bUiModePanel = TRUE;
	RecalcLayout();

	// [LGLS 2026-09-11] 작업정보 판넬을 켜면 작업정보 창도 함께 띄운다(사용자 요청).
	//   판넬은 그대로 동작하고 창이 하나 더 뜬다. 끌 때는 창을 닫지 않는다
	//   - 판넬을 접고 창만 보는 쓰임이 있다.
	if (bShow && nID == ID_PANE_SHOW_JOB)
		m_pDoc->OpenJobListDialog();
}

void CMainFrame::OnUpdatePaneShow(CCmdUI* pCmdUI)
{
	if (pCmdUI == NULL) return;
	CDockablePane* pPane = PaneOf(pCmdUI->m_nID);
	BOOL bOn = (m_bPanelBarsCreated && pPane != NULL
				&& ::IsWindow(pPane->GetSafeHwnd()) && pPane->IsVisible());
	pCmdUI->SetCheck(bOn ? 1 : 0);
	pCmdUI->Enable(TRUE);
}

// [LGLS 2026-09-05] 시간 기반 자동 처리 사용 여부를 DB 에서 읽는다.
//   행이 없거나 조회 실패면 FALSE(사용 안 함) - "모르면 자동 처리하지 않는다" 가 안전한 쪽이다.
BOOL CMainFrame::ReadAutoTimeFlag()
{
	if (m_pDoc == NULL) m_pDoc = (CEcsDoc*)GetActiveDocument();
	if (m_pDoc == NULL) return FALSE;
	try
	{
		CString strSql;
		strSql.Format(_T(" SELECT CCD_CD_YN FROM COMMON_CODE \n")
			_T("  WHERE WH_TYP = '%s' AND CDX_CD = 'SCH_OPT' AND CCD_CD = 'AUTO_TIME' "), m_pDoc->m_WH_TYP);
		int nCnt = 0; CString strErr;
		_RecordsetPtr pRs = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strErr);
		if (nCnt <= 0) return FALSE;
		CRecordSetWrap rsw(pRs); rsw.MoveFirst();
		CString strYn = rsw.GetItem(_T("CCD_CD_YN")); strYn.Trim();
		return (strYn == _T("Y")) ? TRUE : FALSE;
	}
	catch (...) { return FALSE; }
}

// [LGLS 2026-09-05] [시간 기반 자동 처리] 토글. 확인 후 DB 값을 뒤집는다.
void CMainFrame::OnConfigAutoTime()
{
	if (m_pDoc == NULL) m_pDoc = (CEcsDoc*)GetActiveDocument();
	if (m_pDoc == NULL) return;
	BOOL bNew = !ReadAutoTimeFlag();
	CString strMsg = bNew
		? _T("시간 기반 자동 처리를 [사용] 합니다.\n\n설비 완료 신호가 오지 않아도 경과시간으로 완료를 추정합니다.\n계속하시겠습니까?")
		: _T("시간 기반 자동 처리를 [사용 안 함] 으로 바꿉니다.\n\n설비 완료 신호로만 처리합니다. 신호가 오지 않으면 작업이 그 상태에 남습니다.\n계속하시겠습니까?");
	if (AfxMessageBox(strMsg, MB_YESNO | MB_ICONQUESTION) != IDYES) return;
	CString strSql;
	strSql.Format(_T(" UPDATE COMMON_CODE SET CCD_CD_YN = '%s' \n")
		_T("  WHERE WH_TYP = '%s' AND CDX_CD = 'SCH_OPT' AND CCD_CD = 'AUTO_TIME' "),
		bNew ? _T("Y") : _T("N"), m_pDoc->m_WH_TYP);
	m_pDoc->ExcuteQueryString_DLG(strSql);
	m_pDoc->GetQueryInsertClientLog(_T("CMainFrame"), _T(""), _T(""), _T(""),
		CString(_T("CONFIG AUTO TIME -> ")) + (bNew ? _T("Y") : _T("N")));
	m_dwAutoTimeRead = 0;
	m_bAutoTimeProc = ReadAutoTimeFlag();
}

// [LGLS 2026-09-05] 버튼 선택 표시. 다른 화면에서 바꿨을 수도 있어 10초마다 DB 를 다시 읽는다.
void CMainFrame::OnUpdateConfigAutoTime(CCmdUI* pCmdUI)
{
	DWORD dwNow = ::GetTickCount();
	if (m_dwAutoTimeRead == 0 || (dwNow - m_dwAutoTimeRead) > 10000)
	{
		m_dwAutoTimeRead = dwNow;
		m_bAutoTimeProc = ReadAutoTimeFlag();
	}
	pCmdUI->SetCheck(m_bAutoTimeProc ? 1 : 0);
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnConfigIniOpen()
{
	// [LGLS 2026-09-03] 접속/화면 설정 파일(Ecs.ini)을 메모장으로 연다.
	//   32비트 프로세스가 "notepad.exe" 를 부르면 WOW64 리다이렉션으로 SysWOW64 의
	//   32비트 메모장이 뜨는데, Win11 새 메모장은 64비트 전용이라 실행되지 않는다.
	//   Sysnative 로 실제 64비트 메모장을 직접 지정한다.
	CString strIni = ECS_INI_FILE;
	if (::GetFileAttributes(strIni) == INVALID_FILE_ATTRIBUTES)
	{
		AfxMessageBox(_T("INI 파일이 없습니다 : ") + strIni);
		return;
	}

	TCHAR szWin[MAX_PATH] = {0};
	::GetWindowsDirectory(szWin, MAX_PATH);

	BOOL bWow64 = FALSE;
	::IsWow64Process(::GetCurrentProcess(), &bWow64);

	CString strNotepad;
	strNotepad.Format(_T("%s\%s\notepad.exe"), szWin, bWow64 ? _T("Sysnative") : _T("System32"));

	CString strParam;
	strParam.Format(_T("\"%s\""), (LPCTSTR)strIni);

	HINSTANCE hRet = ::ShellExecute(GetSafeHwnd(), _T("open"), strNotepad, strParam, NULL, SW_SHOWNORMAL);
	if ((INT_PTR)hRet <= 32)
		::ShellExecute(GetSafeHwnd(), _T("open"), strIni, NULL, NULL, SW_SHOWNORMAL);
}

void CMainFrame::ShowJobDetail(CString strLuggNo)
{
	if (m_bPanelBarsCreated && ::IsWindow(m_PanelInfoDlg.m_hWnd))
		m_PanelInfoDlg.SetJob(strLuggNo);
}

void CMainFrame::AddCategoryUSER()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_user\\");
	CString strExtension = _T(".png");

	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("USER"), IDB_LOGO_ECS, IDB_LOGO_ECS);
	CMFCRibbonPanel* pPanelUser = pCategory->AddPanel(_T("USER"));

	CMFCRibbonButton* pBtnUserUser = new CMFCRibbonButton(ID_USER_USER, _T("USER"), HICONFromPATH(GetConcatPath(strAppPath, _T("USER"), strExtension)), TRUE);
	pBtnUserUser->SetAlwaysLargeImage();
	pPanelUser->Add(pBtnUserUser);

	//CMFCRibbonButton* pBtnUserGroup = new CMFCRibbonButton(ID_USER_GROUP, _T("GROUP"), HICONFromPATH(GetConcatPath(strAppPath, _T("GROUP"), strExtension)), TRUE);
	//pBtnUserGroup->SetAlwaysLargeImage();
	//pPanelUser->Add(pBtnUserGroup);
}

void CMainFrame::AddCategorySTATUS()
{
	//TCHAR chrFileName[500];
	//GetModuleFileName(NULL, chrFileName, MAX_PATH);
	//CString strAppPath = _T("");
	//strAppPath.Format(_T("%s"),chrFileName);
	//strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_status\\");
	//CString strExtension = _T(".png");
	//
	//CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("STATUS"), IDB_LOGO_ECS, IDB_LOGO_ECS);
	//CMFCRibbonPanel* pPanelStatus = pCategory->AddPanel(_T("STATUS"));
	//
	//CMFCRibbonButton* pBtnStatusCv = new CMFCRibbonButton(ID_STATUS_CV, _T("CV1"), HICONFromPATH(GetConcatPath(strAppPath, _T("cv1"), strExtension)), TRUE);
	//pBtnStatusCv->SetAlwaysLargeImage();
	//pPanelStatus->Add(pBtnStatusCv);
	//
	//CMFCRibbonButton* pBtnStatusSc1 = new CMFCRibbonButton(ID_STATUS_SC1, _T("SC1"), HICONFromPATH(GetConcatPath(strAppPath, _T("sc1"), strExtension)), TRUE);
	//pBtnStatusSc1->SetAlwaysLargeImage();
	//pPanelStatus->Add(pBtnStatusSc1);
	//
	//CMFCRibbonButton* pBtnStatusSc2 = new CMFCRibbonButton(ID_STATUS_SC2, _T("SC2"), HICONFromPATH(GetConcatPath(strAppPath, _T("sc2"), strExtension)), TRUE);
	//pBtnStatusSc2->SetAlwaysLargeImage();
	//pPanelStatus->Add(pBtnStatusSc2);
	//
	//CMFCRibbonButton* pBtnStatusSc3 = new CMFCRibbonButton(ID_STATUS_SC3, _T("SC3"), HICONFromPATH(GetConcatPath(strAppPath, _T("sc3"), strExtension)), TRUE);
	//pBtnStatusSc3->SetAlwaysLargeImage();
	//pPanelStatus->Add(pBtnStatusSc3);
	//
	//CMFCRibbonButton* pBtnStatusSc4 = new CMFCRibbonButton(ID_STATUS_SC4, _T("SC4"), HICONFromPATH(GetConcatPath(strAppPath, _T("sc4"), strExtension)), TRUE);
	//pBtnStatusSc4->SetAlwaysLargeImage();
	//pPanelStatus->Add(pBtnStatusSc4);
	//
	//CMFCRibbonButton* pBtnStatusWc = new CMFCRibbonButton(ID_STATUS_WC1, _T("WC1"), HICONFromPATH(GetConcatPath(strAppPath, _T("wc1"), strExtension)), TRUE);
	//pBtnStatusWc->SetAlwaysLargeImage();
	//pPanelStatus->Add(pBtnStatusWc);
}


//void CMainFrame::OnUpdateBtnActive(CCmdUI *pCmdUI)
//{
//	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
//}


void CMainFrame::OnUpdateStatusCv(CCmdUI *pCmdUI)
{
	 //pCmdUI->Enable(!m_bOperationOn);
}

// [LGLS 2026-09-08] 통신상태 표시를 리본 오른쪽 빈자리로 올린다. (사용자 요청)
//   현장 화면이 1024x768 이라 하단 40px 를 통째로 쓰는 것이 아깝고, 리본 오른쪽은
//   비어 있다. Ecs.ini [MENU] STATUS_POS 로 고른다(TOP 기본 / BOTTOM 이면 종전 그대로).
#define LGLS_STATUS_TOP_W   300   // 위로 올렸을 때 상태바 폭(버튼 80 x 3 + 여백)
#define LGLS_STATUS_TOP_H   34    // 〃 높이
#define LGLS_STATUS_TOP_Y   34    // 〃 리본 위쪽에서의 거리
#define LGLS_STATUS_TOP_MGN 26    // 〃 오른쪽 여백(마지막 버튼이 잘리지 않게)

// [LGLS 2026-09-08] 통신상태 색 (사용자 지정) : 정상=푸른색 / 단절=붉은색
#define LGLS_COMM_OK   RGB(0, 112, 224)
#define LGLS_COMM_NG   RGB(214, 40, 40)

IMPLEMENT_DYNCREATE(CLglsRibbonTabLabel, CMFCRibbonLabel)

void CLglsRibbonTabLabel::OnDraw(CDC* pDC)
{
	if (pDC == NULL) return;
	CRect rc = m_rect;
	if (rc.IsRectEmpty()) return;

	// 아래(리본 그룹 영역)와 이어져 보이도록 바닥을 살짝 넘겨 칠한다 - 활성 탭과 같은 모양.
	CRect rcFill = rc;
	rcFill.bottom += 2;

	CDrawingManager dm(*pDC);
	dm.FillGradient(rcFill, RGB(213, 217, 222), RGB(247, 250, 252), TRUE);

	CPen  pen(PS_SOLID, 1, RGB(150, 156, 165));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rc.left, rc.bottom);
	pDC->LineTo(rc.left, rc.top);
	pDC->LineTo(rc.right - 1, rc.top);
	pDC->LineTo(rc.right - 1, rc.bottom);
	pDC->SelectObject(pOldPen);

	int      nBk   = pDC->SetBkMode(TRANSPARENT);
	COLORREF clrTx = pDC->SetTextColor(RGB(30, 30, 30));
	CFont*   pOldF = pDC->SelectObject(&afxGlobalData.fontRegular);
	pDC->DrawText(m_strText, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldF);
	pDC->SetTextColor(clrTx);
	pDC->SetBkMode(nBk);
}

IMPLEMENT_DYNCREATE(CLglsRibbonPanel, CMFCRibbonPanel)
IMPLEMENT_DYNCREATE(CLglsRibbonComm, CMFCRibbonButton)

CLglsRibbonComm::CLglsRibbonComm()
	: m_clrState(RGB(0, 0, 0)), m_hOn(NULL), m_hOff(NULL) {}

CLglsRibbonComm::CLglsRibbonComm(UINT nID, LPCTSTR lpszText, HICON hOn, HICON hOff, BOOL bLarge)
	: CMFCRibbonButton(nID, lpszText, hOff, FALSE, hOff), m_clrState(LGLS_COMM_NG), m_hOn(hOn), m_hOff(hOff)
{
	// 탭 줄(모드 1)은 작은 아이콘, 리본 그룹(모드 2)은 다른 그룹과 같은 큰 아이콘.
	if (bLarge) SetAlwaysLargeImage();
}

// [LGLS 2026-09-08] 색 대신 ★아이콘★ 을 갈아 끼운다.
//   크기/글자 배치를 직접 그리면 리본이 폭을 줄여 글자가 잘린다 - 기본 그리기에 맡기고
//   아이콘만 바꾸는 편이 확실하다. 호출부는 그대로 색을 넘긴다(판정 로직 불변).
void CLglsRibbonComm::SetStateColor(COLORREF clr)
{
	if (m_clrState == clr) return;
	m_clrState = clr;
	HICON h = (clr == LGLS_COMM_OK) ? m_hOn : m_hOff;
	if (h != NULL) { m_hIcon = h; m_hIconSmall = h; }
	Redraw();
}

// [LGLS 2026-09-08] STATUS_POS=RIBBON 이면 리본 안(각 탭의 마지막 그룹)에 붙인다.
BOOL CMainFrame::IsStatusOnRibbon()
{
	TCHAR szTemp[64] = {0};
	::GetPrivateProfileString(_T("MENU"), _T("STATUS_POS"), _T("RIBBON"), szTemp, 64, ECS_INI_FILE);
	CString str(szTemp); str.Trim(); str.MakeUpper();
	return (str == _T("RIBBON")) ? TRUE : FALSE;
}

// [LGLS 2026-09-08] 기본 배치 뒤 [통신] 탭만 오른쪽 끝으로 민다.
void CLglsRibbonBar::RecalcLayout()
{
	CMFCRibbonBar::RecalcLayout();

	if (!::IsWindow(GetSafeHwnd()))
		return;

	CRect rcCli;
	GetClientRect(rcCli);

	// ── 탭을 오른쪽 끝으로 (지금은 쓰지 않는다 - 별도 [통신] 탭을 두던 시절의 장치) ──
	if (m_pRightCat != NULL)
	{
		CMFCRibbonTab* pTab = m_pRightCat->GetTab();
		if (pTab != NULL)
		{
			CRect rcTab = pTab->GetRect();
			int nDx = (rcCli.right - 10) - rcTab.right;
			if (!rcTab.IsRectEmpty() && nDx > 0)
			{
				rcTab.OffsetRect(nDx, 0);
				pTab->SetRect(rcTab);
			}
		}
	}

	// ── [통신] 그룹을 리본 오른쪽 끝으로 (COMM_MODE=2) ──
	//   활성 탭의 그룹만 자리(rect)가 잡힌다. 비활성 탭의 것은 비어 있으므로 건너뛴다.
	BOOL bMoved = FALSE;
	for (int i = 0; i < m_arRightPanels.GetCount(); i++)
	{
		CLglsRibbonPanel* pPn = (CLglsRibbonPanel*)m_arRightPanels.GetAt(i);
		if (pPn == NULL) continue;
		CRect rcP = pPn->GetRect();
		if (rcP.IsRectEmpty()) continue;
		int nDxP = (rcCli.right - 12) - rcP.right;
		if (nDxP <= 0) continue;
		CClientDC dc(this);
		CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);
		CRect rcNew = rcP;
		rcNew.OffsetRect(nDxP, 0);
		pPn->LglsMoveTo(&dc, rcNew);
		if (pOldFont != NULL) dc.SelectObject(pOldFont);
		bMoved = TRUE;
	}

	// [LGLS 2026-09-08] ★옮기고 나면 반드시 리본 전체를 다시 그린다★
	//   기본 배치가 끝난 자리(왼쪽)에 이미 그려진 그림이 남아, 기동 직후 [통신] 그룹이
	//   두 군데 보였다(사용자 지적). 탭을 한 번 누르면 전체 다시 그리기가 일어나 사라졌다.
	//   RecalcLayout 은 그리기(WM_PAINT)에서 불리지 않으므로 되돌이 걱정은 없다.
	if (bMoved)
		Invalidate(FALSE);
}

// [LGLS 2026-09-08] 통신상태 표시 방식.  Ecs.ini [MENU] COMM_MODE
//   1 = 리본 탭 줄 오른쪽 끝 (기본)   2 = 탭마다 [통신] 그룹, 리본 오른쪽 끝
int CMainFrame::GetCommMode()
{
	return ::GetPrivateProfileInt(_T("MENU"), _T("COMM_MODE"), 1, ECS_INI_FILE);
}

// [모드 2] 카테고리(탭)마다 [통신] 그룹을 하나씩 붙인다 - 어느 탭에서도 보인다.
//   그룹 위치는 CLglsRibbonBar::RecalcLayout 이 리본 오른쪽 끝으로 옮긴다.
void CMainFrame::AddCommPanel(CMFCRibbonCategory* pCategory)
{
	if (pCategory == NULL || !IsStatusOnRibbon() || GetCommMode() != 2) return;

	CMFCRibbonPanel* pPanel = pCategory->AddPanel(_T("통신"), 0, RUNTIME_CLASS(CLglsRibbonPanel));
	if (pPanel == NULL) return;

	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	strAppPath.Format(_T("%s"), chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_config\\");
	HICON hOn  = HICONFromPATH(GetConcatPath(strAppPath, _T("comm_on"),  _T(".png")));
	HICON hOff = HICONFromPATH(GetConcatPath(strAppPath, _T("comm_off"), _T(".png")));

	struct D3 { UINT id; LPCTSTR s; LPCTSTR d; };
	D3 defs[] = { { ID_STATUS_CV_1, _T("EQUIP"), _T("설비 통신 (WCS_TASK_CV)") },
	              { ID_STATUS_HOST, _T("HOST"),  _T("상위 통신 (WCS_TASK_HOST)") },
	              { ID_STATUS_SCH,  _T("SCH"),   _T("스케줄러 (IO_TASK)") } };
	for (int i = 0; i < 3; i++)
	{
		CLglsRibbonComm* p = new CLglsRibbonComm(defs[i].id, defs[i].s, hOn, hOff, TRUE);
		p->SetToolTipText(defs[i].d);
		p->SetDescription(defs[i].d);
		pPanel->Add(p);
		m_arRbnComm.Add(p);
	}
	m_wndRibbonBar.AddRightPanel((CLglsRibbonPanel*)pPanel);
}

// [LGLS 2026-09-08] ★리본 탭 줄 오른쪽 끝★ 에 [통신] 표시를 붙인다. (사용자 확정)
//   별도 탭(카테고리)으로 만들었더니 그 탭을 눌러야만 보였고, 탭이 두 번 그려졌다.
//   AddToTabs 는 어느 탭을 보고 있든 항상 같은 자리에 남는다 - 이쪽이 맞다.
void CMainFrame::AddCommToTabs()
{
	if (!IsStatusOnRibbon()) return;

	if (GetCommMode() == 2)
	{
		// [LGLS 2026-09-08] 모드 2 : 상태는 아래 [통신] 그룹에 있고,
		//   탭 줄 오른쪽 끝에는 그 그룹이 무엇인지 알리는 이름만 둔다. (사용자 요청)
		m_wndRibbonBar.AddToTabs(new CLglsRibbonTabLabel(_T("  통신  ")));
		m_wndRibbonBar.AddToTabs(new CMFCRibbonLabel(_T("     ")));	// 오른쪽 여백
		return;
	}

	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	strAppPath.Format(_T("%s"), chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_config\\");
	HICON hOn  = HICONFromPATH(GetConcatPath(strAppPath, _T("comm_on16"),  _T(".png")));
	HICON hOff = HICONFromPATH(GetConcatPath(strAppPath, _T("comm_off16"), _T(".png")));

	m_wndRibbonBar.AddToTabs(new CMFCRibbonLabel(_T("통신 ")));

	struct D2 { UINT id; LPCTSTR s; LPCTSTR d; };
	D2 defs[] = { { ID_STATUS_CV_1, _T("EQUIP  "), _T("설비 통신 (WCS_TASK_CV)") },
	              { ID_STATUS_HOST, _T("HOST  "),  _T("상위 통신 (WCS_TASK_HOST)") },
	              { ID_STATUS_SCH,  _T("SCH"),    _T("스케줄러 (IO_TASK)") } };
	// 탭 줄에서는 아이콘만 그려지고 글자는 빠진다. 그래서 이름은 라벨로 따로 붙인다.
	for (int i = 0; i < 3; i++)
	{
		CLglsRibbonComm* p = new CLglsRibbonComm(defs[i].id, _T(""), hOn, hOff);
		p->SetToolTipText(defs[i].d);
		p->SetDescription(defs[i].d);
		m_wndRibbonBar.AddToTabs(p);
		m_arRbnComm.Add(p);

		CMFCRibbonLabel* pNm = new CMFCRibbonLabel(defs[i].s);
		pNm->SetToolTipText(defs[i].d);
		m_wndRibbonBar.AddToTabs(pNm);
	}

	// 마지막 항목이 창 오른쪽 끝에 붙어 잘리지 않도록 여백을 둔다.
	m_wndRibbonBar.AddToTabs(new CMFCRibbonLabel(_T("    ")));
}

void CMainFrame::SetCommColor(UINT nID, COLORREF clr)
{
	for (int i = 0; i < m_arRbnComm.GetCount(); i++)
	{
		CLglsRibbonComm* p = (CLglsRibbonComm*)m_arRbnComm.GetAt(i);
		if (p != NULL && p->GetID() == nID) p->SetStateColor(clr);
	}
}

BOOL CMainFrame::IsStatusBarOnTop()
{
	TCHAR szTemp[64] = {0};
	::GetPrivateProfileString(_T("MENU"), _T("STATUS_POS"), _T("TOP"), szTemp, 64, ECS_INI_FILE);
	CString str(szTemp); str.Trim(); str.MakeUpper();
	return (str == _T("BOTTOM")) ? FALSE : TRUE;
}

void CMainFrame::LayoutStatusBar(int cx, int cy)
{
	if (!::IsWindow(m_wndStatusBar.GetSafeHwnd()) || cx <= 0 || cy <= 0)
		return;
	if (IsStatusOnRibbon())
	{
		// 리본 안에 붙였으므로 떠 있는 상태바는 감춘다(하단 40px 도 돌려준다).
		m_wndStatusBar.ShowWindow(SW_HIDE);
		return;
	}
	m_wndStatusBar.ShowWindow(SW_SHOW);
	if (IsStatusBarOnTop())
	{
		int w = LGLS_STATUS_TOP_W;
		if (w > cx - 40) w = cx - 40;		// 창이 좁으면 줄인다
		m_wndStatusBar.MoveWindow(cx - w - LGLS_STATUS_TOP_MGN, LGLS_STATUS_TOP_Y, w, LGLS_STATUS_TOP_H);
	}
	else
	{
		m_wndStatusBar.MoveWindow(0, cy - 40, cx, 40);
	}
	m_wndStatusBar.BringWindowToTop();
}

// [LGLS 2026-09-01] 도킹 판넬이 하단 커스텀 상태바(40px, 통신상태 버튼)를 침범해
//   통신상태 표시가 가려지던 문제 - 레이아웃 후 판넬 높이를 상태바 위까지로 클램프한다.
void CMainFrame::RecalcLayout(BOOL bNotify)
{
	CFrameWndEx::RecalcLayout(bNotify);

	if (!m_bPanelBarsCreated || !::IsWindow(m_wndStatusBar.GetSafeHwnd()))
		return;
	CRect rcCli; GetClientRect(&rcCli);
	LayoutStatusBar(rcCli.Width(), rcCli.Height());
	// [LGLS 2026-09-08] 위로 올렸으면 하단을 비워 두지 않아도 된다 - 클램프는 하단일 때만.
	if (IsStatusOnRibbon() || IsStatusBarOnTop())
	{
		if (!IsStatusOnRibbon()) m_wndStatusBar.BringWindowToTop();
		return;
	}
	int nBarTop = rcCli.Height() - 40;

	CDockablePane* pPanes[] = { &m_JobPane, &m_InfoPane, &m_VehPane };
	for (int i = 0; i < 3; i++)
	{
		CDockablePane* p = pPanes[i];
		if (!::IsWindow(p->m_hWnd) || !p->IsVisible() || p->IsFloating())
			continue;
		CRect rc; p->GetWindowRect(rc); ScreenToClient(rc);
		if (rc.bottom > nBarTop && rc.top < nBarTop)
			p->SetWindowPos(NULL, 0, 0, rc.Width(), nBarTop - rc.top,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	m_wndStatusBar.BringWindowToTop();   // 통신상태 버튼이 항상 보이게
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	// [LGLS] resize: relocate custom status bar so comm buttons(EQUIP/HOST/SCH) stay on-screen
	LayoutStatusBar(cx, cy);
}

void CMainFrame::AddStatusBarPane()
{
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(30);
	m_wndStatusBar.SetPaneWidth(0, 30);

	int i=0;
	++i;
	//InsertLabelPainToStatusBar(_T("SR_MODE"), ID_STATUS_SR_MODE, i, 200);

	//InsertLabelPainToStatusBar(_T("CV_1G"), ID_STATUS_CV_1, i+1, 90);
	//InsertLabelPainToStatusBar(_T("SC_1호기"), ID_STATUS_SC_1, i+2, 90);
	//InsertLabelPainToStatusBar(_T("SC_2호기"), ID_STATUS_SC_2, i+3, 90);
	//InsertLabelPainToStatusBar(_T("SC_3호기"), ID_STATUS_SC_3, i+4, 90);
	//InsertLabelPainToStatusBar(_T("SC_4호기"), ID_STATUS_SC_4, i+5, 90);
	//InsertLabelPainToStatusBar(_T("WC_104"), ID_STATUS_WC_1, i+6, 90);
	//InsertLabelPainToStatusBar(_T("HOST"), ID_STATUS_HOST, i+7, 90);

	// [LGLS] only EQUIP (equipment TASK program) + HOST are actually communicated with
	// [LGLS 2026-09-08] 리본 오른쪽으로 올리면 자리가 좁으므로 버튼도 좁게 만든다.
	int nBtnW = IsStatusBarOnTop() ? 80 : 100;
	InsertButtonPainToStatusBar(_T("EQUIP"), ID_STATUS_CV_1, i+1, nBtnW);
	InsertButtonPainToStatusBar(_T("HOST"), ID_STATUS_HOST, i+2, nBtnW);
	// [LGLS] SCH = IO_TASK (scheduler) health, read from EQP_MST heartbeat (EQP_TYP='SCH').
	InsertButtonPainToStatusBar(_T("SCH"), ID_STATUS_SCH, i+3, nBtnW);
		
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SR_MODE, _T("MODE"), YELLOW, BLACK); 

	//m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, _T("CV_1G"), YELLOW, BLACK); //배경/글씨
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_1, _T("SC_1호기"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_2, _T("SC_2호기"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_3, _T("SC_3호기"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_4, _T("SC_4호기"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_WC_1, _T("WC_104"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_HOST, _T("HOST"), YELLOW, BLACK);

	//DARK_GRAY,//RGB(255, 128, 192),
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, nBtnW, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_HOST, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, nBtnW, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SCH, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, nBtnW, CMinButton::UPTODOWN);


}

void CMainFrame::InsertLabelPainToStatusBar(CString strCaption, int nID, int nPos, int nWidth)
{
	CExtStatusBarLabel *pLabel = new CExtStatusBarLabel;
	if (!pLabel->Create(strCaption, WS_CHILD|WS_VISIBLE|WS_TABSTOP|SS_CENTER|SS_CENTERIMAGE, CRect(0,0,0,0), &m_wndStatusBar, 0))
		return;

	//	pLabel->SetFont( CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)) );
	if ( !m_wndStatusBar.AddPane(nID, nPos) ) // ID, nIndex
		return;
	int nIndex = m_wndStatusBar.CommandToIndex(nID);
	if (nIndex == -1)
		return;

	m_wndStatusBar.SetPaneWidth(nIndex, nWidth);
	m_wndStatusBar.AddPaneControl(pLabel, nID, true);
}

void CMainFrame::InsertButtonPainToStatusBar(CString strCaption, int nID, int nPos, int nWidth)
{
	CExtStatusBarButton* pButton = new CExtStatusBarButton;
	if (!pButton->Create(strCaption, WS_CHILD|WS_VISIBLE|WS_TABSTOP|WM_DRAWITEM, CRect(0,0,0,0), &m_wndStatusBar, nID))
		return;

	pButton->SetFont( CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT))	);
	if ( !m_wndStatusBar.AddPane(nID, nPos) )
		return;
	int nIndex = m_wndStatusBar.CommandToIndex(nID);
	if (nIndex == -1)
		return;
	m_wndStatusBar.SetPaneWidth(nIndex, nWidth);
	m_wndStatusBar.AddPaneControl(pButton, nID, true);
}

void CMainFrame::OnButtonComm(UINT nID)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	
		return;

	CString strEQP_TIME = _T("");
	CString strEQP_CONNECTED_YN = _T("");
	CString strTYPE = _T("");
	int nEQP_TIME = 0;
	CString strTemp1, strTemp2;
	int nEQP_NUM = 0;
	int nCheckTime = 0;				// Check하는 시간이 다름!

	CTrackInfo* pTrackInfo = NULL;
	CSC_DATA* pSC_DATA = NULL;

	// TEST 성공 - ANSI에서만 됨
	//LPCSTR strPingTest = "cmd.exe /k ping -t 127.0.0.1";
	//WinExec(strPingTest, SW_SHOW);

	// TEST UNICODE 에서 사용가능한거 찾기
	//CString strPingTest = _T("/k ping -t 127.0.0.1");
	//::ShellExecute(NULL, _T("open"), _T("CMD.exe"), strPingTest, NULL, SW_SHOW);

    #pragma region Control 번호를 통해서 필요한 정보들을 설정하는 부분

	switch(nID)
	{
	case ID_STATUS_CV_1:
		#pragma region ID_STATUS_CV_1
		// [LGLS] Must match OnUpdateStatusCv1, which colours this pane from track "121" (PLC_NO=11,
		//   the live EQP_TASK heartbeat). The old "101" is PLC_NO=01: a dummy C/V nobody updates, so
		//   its age was always > 5 -> clicking a GREEN EQUIP still popped the ping-test dialog.
		pTrackInfo = pDoc->GetTrackInfoNew(_T("121"));
	
		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 11;   // [LGLS] EQP_MST PLC_NO=11 (was 1 -> the dead dummy C/V 01)
		nCheckTime = 5;

		#pragma endregion
		break;
	case ID_STATUS_SC_1:
		#pragma region ID_STATUS_SC_1
		pSC_DATA = pDoc->GetSC_DATA(_T("901"));
	
		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.242"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 1;
		nCheckTime = 5;

		#pragma endregion
		break;
	case ID_STATUS_SC_2:
		#pragma region ID_STATUS_SC_2
		pSC_DATA = pDoc->GetSC_DATA(_T("902"));
	
		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 2;
		nCheckTime = 5;

		#pragma endregion
		break;
	case ID_STATUS_SC_3:
		#pragma region ID_STATUS_SC_3
		pSC_DATA = pDoc->GetSC_DATA(_T("903"));
	
		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 3;
		nCheckTime = 5;

		#pragma endregion
		break;
	case ID_STATUS_SC_4:
		#pragma region ID_STATUS_SC_4
		pSC_DATA = pDoc->GetSC_DATA(_T("904"));
	
		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 4;
		nCheckTime = 5;

		#pragma endregion
		break;
	case ID_STATUS_HOST:
		#pragma region ID_STATUS_HOST
		if (pDoc->m_pConnectStatus == NULL)
		{
 			//AfxMessageBox(m_pDoc->GetMsgLangDef(_T("HOST 접속 정보가 없습니다!")));		// 번역할때
 			AfxMessageBox(_T("HOST 접속 정보가 없습니다!"));								// 번역안할때
			return;
		}
		strEQP_TIME = pDoc->m_pConnectStatus->GetTime2Safe();
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("HOST2");
		nEQP_NUM = 1;
		nCheckTime = 120;
		#pragma endregion
		break;
	case ID_STATUS_SCH:
		#pragma region ID_STATUS_SCH
		// [LGLS] IO_TASK heartbeat (EQP_MST EQP_TYP='SCH' PLC_NO='01').
		if (pDoc->m_pConnectStatus == NULL)
			return;
		strEQP_TIME = pDoc->m_pConnectStatus->GetTime3Safe();
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SCH");
		nEQP_NUM = 1;
		nCheckTime = SCH_ALIVE_SEC;
		#pragma endregion
		break;
	default:
		return;
	}
	#pragma endregion

	if (pDoc->m_blConnectStatus == TRUE)
	{
		//HOST_IF_LOG에 들어온 메시지가 30초 이상 경과 되었을 떄 에러로 표시
		if (nEQP_TIME > nCheckTime)
		{
			// 쿼리문을 날려라
			// 가져온값으로 확인하라.
			CString strSql, strTemp, strMessage;
			strSql.Format(_T("SELECT * FROM EQP_MST WHERE WH_TYP = '%s' AND EQP_TYP = '%s' AND PLC_NO = '%02d'"), pDoc->m_WH_TYP, strTYPE, nEQP_NUM);

			int nRowCount = 0;//m_pDoc->GetSelectQryCnt_DLG(strSql);
			_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr(strSql, nRowCount, strMessage);

 			if (nRowCount < 1)
 			{
				strTemp.Format(_T("설비가 존재하지 않습니다. [설비종류 : %s][설비번호 : %02d]"), strTYPE, nEQP_NUM);
 				//AfxMessageBox(m_pDoc->GetMsgLangDef(strTemp));		// 번역할때 
 				AfxMessageBox(strTemp);									// 번역안할때
 				return;
 			}

 			if (nRowCount > 1)
 			{
				strTemp.Format(_T("설비가 한개가 아니라 여러개 존재합니다. - 검색 조건 확인요망 => [설비종류 : %s][설비번호 : %02d]"), strTYPE, nEQP_NUM);
 				//AfxMessageBox(m_pDoc->GetMsgLangDef(strTemp));		// 번역할때 
 				AfxMessageBox(strTemp);									// 번역안할때
				
 				return;
 			}

			CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	
			pRsw->MoveFirst(); 

			//CString strIP, strPORT, strBAT, strEQP_TYP;
	
			//for(int i = 1; i <= nRowCount; i++)
			//{
			CString	strREMARKS = pRsw->GetItem(_T("REMARKS"));		
			CString	strIP = pRsw->GetItem(_T("PLC_IP"));		
			CString	strPORT = pRsw->GetItem(_T("PLC_PORT"));		
			CString	strBAT = pRsw->GetItem(_T("BATCH"));
			//	pRsw->MoveNext();
			//}

			delete pRsw;

			strIP.Trim();
			strPORT.Trim();
			strTemp1.Format(_T("%s (IP:%s) (PORT:%s)"), strREMARKS, strIP, strPORT);

			// [LGLS 2026-09-08] 통신이 끊겼을 때(빨간색) 눌러 확인하는 절차. (사용자 지시)
			//   ① 핑 테스트 하겠습니까?  예 → 그 IP 로 ping
			//   ② 아니오 → 포트 확인 하겠습니까?  예 → 그 IP:PORT 가 열려 있는지 확인
			//   ③ 아니오 → 접속할 수 없다는 안내
			//   종전에는 EQP_MST.BATCH 의 BAT 파일을 실행했는데 그 값이 비어 있어
			//   아무 일도 일어나지 않았다. 명령을 직접 띄운다(창은 열어 두고 결과를 보여 준다).
			if (strIP.IsEmpty())
			{
				strTemp.Format(m_pDoc == NULL ? _T("접속 IP 가 등록되어 있지 않습니다.\r\n%s")
				                              : _T("접속 IP 가 등록되어 있지 않습니다.\r\n%s"), strTemp1);
				AfxMessageBox(strTemp);
				return;
			}

			strTemp2.Format(_T("통신이 끊어졌습니다.\r\n\r\n[%s]\r\n\r\n핑 테스트를 하시겠습니까?"), strTemp1);
			if (AfxMessageBox(strTemp2, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				CString strArg;
				strArg.Format(_T("/k ping -n 4 %s"), strIP);
				::ShellExecute(NULL, _T("open"), _T("cmd.exe"), strArg, NULL, SW_SHOWNORMAL);
				return;
			}

			strTemp2.Format(_T("포트가 열려 있는지 확인하시겠습니까?\r\n\r\n[%s : %s]"), strIP, strPORT);
			if (AfxMessageBox(strTemp2, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				if (strPORT.IsEmpty() || strPORT == _T("0"))
				{
					AfxMessageBox(_T("확인할 포트가 등록되어 있지 않습니다."));
					return;
				}
				CString strArg;
				strArg.Format(_T("/k powershell -NoProfile -Command \"Test-NetConnection -ComputerName %s -Port %s -InformationLevel Detailed\""),
				              strIP, strPORT);
				::ShellExecute(NULL, _T("open"), _T("cmd.exe"), strArg, NULL, SW_SHOWNORMAL);
				return;
			}

			strTemp.Format(_T("설비에 접속할 수 없습니다.\r\n\r\n[%s]\r\n\r\n네트워크/설비 담당자에게 확인하십시오."), strTemp1);
			AfxMessageBox(strTemp, MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// [LGLS] GREEN (link healthy) -> the status button must not react at all.
			//   The old code called AfxMessageBox(strTemp1) here, but strTemp1 is only built
			//   inside the error branch above, so a healthy link popped an EMPTY message box.
			return;
		}
	}
}

void CMainFrame::OnButtonCv1()
{
	//AfxMessageBox(_T("야호"));
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	
		return;

	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("101"));
	
	if (pTrackInfo == NULL)
		return;

	CString strTemp1, strTemp2;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	
	strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);

	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5)
	{
		strTemp2.Format(_T("PingTest를 하겠습니까? [접속정보 -> %s]"),strTemp1);
		
		if (AfxMessageBox(strTemp2, MB_YESNO) != IDYES)
		{
			AfxMessageBox(_T("통신 연결 되지 않았습니다!"));
			return;
		}

		AfxMessageBox(_T("PingTest 하겠습니다.\n\nPing 비정상시 네트워크 담당자에게 확인바랍니다!\nPing 정상시 PORT가 정상적으로 OPEN 되어있는지 확인하세요"));
		::ShellExecute(NULL, _T("open"), _T(".\\PING_CV01.BAT"), NULL, NULL, SW_SHOW);
	}
	else
	{
		AfxMessageBox(strTemp1);
	}
}

void CMainFrame::OnButtonSc1()
{
	//AfxMessageBox(_T("야호"));
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	
		return;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("901"));
	
	if (pSC_DATA == NULL)
		return;

	CString strTemp1, strTemp2;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.242"),8192);
	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5)
	{
		strTemp2.Format(_T("PingTest를 하겠습니까? [접속정보 -> %s]"),strTemp1);
		
		if (AfxMessageBox(strTemp2, MB_YESNO) != IDYES)
		{
			AfxMessageBox(_T("통신 연결 되지 않았습니다!"));
			return;
		}

		AfxMessageBox(_T("PingTest 하겠습니다.\n\nPing 비정상시 네트워크 담당자에게 확인바랍니다!\nPing 정상시 PORT가 정상적으로 OPEN 되어있는지 확인하세요"));
		::ShellExecute(NULL, _T("open"), _T(".\\PING_SC01.BAT"), NULL, NULL, SW_SHOW);
	}
	else
	{
		AfxMessageBox(strTemp1);
	}
}

void CMainFrame::OnButtonSc2()
{
	//AfxMessageBox(_T("야호"));
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	
		return;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("902"));
	
	if (pSC_DATA == NULL)
		return;

	CString strTemp1, strTemp2;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	strTemp1.Format(_T("SC_02 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);

	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5)
	{
		strTemp2.Format(_T("PingTest를 하겠습니까? [접속정보 -> %s]"),strTemp1);
		
		if (AfxMessageBox(strTemp2, MB_YESNO) != IDYES)
		{
			AfxMessageBox(_T("통신 연결 되지 않았습니다!"));
			return;
		}

		AfxMessageBox(_T("PingTest 하겠습니다.\n\nPing 비정상시 네트워크 담당자에게 확인바랍니다!\nPing 정상시 PORT가 정상적으로 OPEN 되어있는지 확인하세요"));
		::ShellExecute(NULL, _T("open"), _T(".\\PING_SC02.BAT"), NULL, NULL, SW_SHOW);
	}
	else
	{
		AfxMessageBox(strTemp1);
	}
}

void CMainFrame::OnButtonSc3()
{
	//AfxMessageBox(_T("야호"));
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	
		return;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("903"));
	
	if (pSC_DATA == NULL)
		return;

	CString strTemp1, strTemp2;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	strTemp1.Format(_T("SC_03 (IP:%s) (PORT:%04d)"),_T("10.99.43.244"),8192);

	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5)
	{
		strTemp2.Format(_T("PingTest를 하겠습니까? [접속정보 -> %s]"),strTemp1);
		
		if (AfxMessageBox(strTemp2, MB_YESNO) != IDYES)
		{
			AfxMessageBox(_T("통신 연결 되지 않았습니다!"));
			return;
		}

		AfxMessageBox(_T("PingTest 하겠습니다.\n\nPing 비정상시 네트워크 담당자에게 확인바랍니다!\nPing 정상시 PORT가 정상적으로 OPEN 되어있는지 확인하세요"));
		::ShellExecute(NULL, _T("open"), _T(".\\PING_SC03.BAT"), NULL, NULL, SW_SHOW);
	}
	else
	{
		AfxMessageBox(strTemp1);
	}
}

void CMainFrame::OnButtonSc4()
{
	//AfxMessageBox(_T("야호"));
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	
		return;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("904"));
	
	if (pSC_DATA == NULL)
		return;

	CString strTemp1, strTemp2;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	strTemp1.Format(_T("SC_04 (IP:%s) (PORT:%04d)"),_T("10.99.43.245"),8192);

	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5)
	{
		strTemp2.Format(_T("PingTest를 하겠습니까? [접속정보 -> %s]"),strTemp1);
		
		if (AfxMessageBox(strTemp2, MB_YESNO) != IDYES)
		{
			AfxMessageBox(_T("통신 연결 되지 않았습니다!"));
			return;
		}

		AfxMessageBox(_T("PingTest 하겠습니다.\n\nPing 비정상시 네트워크 담당자에게 확인바랍니다!\nPing 정상시 PORT가 정상적으로 OPEN 되어있는지 확인하세요"));
		::ShellExecute(NULL, _T("open"), _T(".\\PING_SC04.BAT"), NULL, NULL, SW_SHOW);
	}
	else
	{
		AfxMessageBox(strTemp1);
	}
}

void CMainFrame::OnButtonHost()
{
	//헷갈리겠지만 이 함수가 EQP_MST에서 EQP_TYP = HOST2 (E2W)
	//AfxMessageBox(_T("야호"));
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	
		return;

	// [LGLS 2026-09-10] 종전에는 여기서 WC 설비 데이터를 찾아 없으면 그냥 돌아갔다.
	//   이 현장에는 WC 설비가 없어 늘 NULL 이었고, 그래서 HOST 상태등을 눌러도
	//   아무 일도 일어나지 않았다. WC 를 걷어내면서 그 빗장을 없앤다.

	CString strTemp1, strTemp2;
	strTemp1.Format(_T("HOST (IP:%s) (PORT:%04d)"),_T("10.99.10.141"),8400);

	//CConnectStatus가 생성된 후에 로직에 타게끔
	if (pDoc->m_blConnectStatus == TRUE)
	{
		CConnectStatus* pConnectStatus = pDoc->m_pConnectStatus;

		CString strEQP_TIME = pConnectStatus->GetTime2Safe();
		//CString strEQP_COLOR = pConnectStatus->V_EQP_COLOR;
		int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		
		//HOST_IF_LOG에 들어온 메시지가 30초 이상 경과 되었을 떄 에러로 표시
		if (nEQP_TIME > 120)
		{
			strTemp2.Format(_T("PingTest를 하겠습니까? [접속정보 -> %s]"),strTemp1);
			
			if (AfxMessageBox(strTemp2, MB_YESNO) != IDYES)
			{
				AfxMessageBox(_T("통신 연결 되지 않았습니다!"));
				return;
			}

			AfxMessageBox(_T("PingTest 하겠습니다.\n\nPing 비정상시 네트워크 담당자에게 확인바랍니다!\nPing 정상시 PORT가 정상적으로 OPEN 되어있는지 확인하세요"));
			::ShellExecute(NULL, _T("open"), _T(".\\PING_WC01.BAT"), NULL, NULL, SW_SHOW);
		}
		else
		{
			AfxMessageBox(strTemp1);
		}
	}
}



void CMainFrame::OnUpdateStatusSR_MODE(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	// TEST - 미사용으로 변경
		return;
	
	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("149"));
	
	//if (pTrackInfo == NULL)			// TEST - 미사용으로 변경
		return;


	CString strSTOCK_MODE = pTrackInfo->m_pCV_DATA->V_STOCK_MODE;
	//CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	//CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	//int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	//
	if (strSTOCK_MODE != _T("1"))
	{
	//	if(strSTOCK_MODE != _T("1"))
	//	{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SR_MODE, _T("입고 모드"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
	//	}
	}
	else
	{
	//	if(strEQP_COLOR != _T("GREEN"))
	//	{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SR_MODE, _T("출고 모드"), BLUE, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
	//	}
		
	}
	return;
}

void CMainFrame::OnUpdateStatusCv1(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;
	
	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("121"));
	
	if (pTrackInfo == NULL)
		return;


	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	// [LGLS 2026-08-04] 5초는 미러 사이클(~3초)에 여유가 없어 한 사이클만 느어도 벰가 정상이다. 15초로 여유를 주고
	//   실제 단절은 CONNECTED_YN='N' 또는 15초 무응답으로 판정한다.
	if (nEQP_TIME > 15 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, _T("EQUIP"), RED, BLACK);
			SetCommColor(ID_STATUS_CV_1, LGLS_COMM_NG);	// [LGLS] EQP_TASK ??????
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, _T("EQUIP"), GREEN, BLACK);
			SetCommColor(ID_STATUS_CV_1, LGLS_COMM_OK);	// [LGLS] EQP_TASK ????
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
		
	}
	return;
}


void CMainFrame::OnUpdateStatusSc1(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;
	
	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("901"));
	
	if (pSC_DATA == NULL)
		return;

	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	// [LGLS 2026-08-04] 5초는 미러 사이클(~3초)에 여유가 없어 한 사이클만 느어도 벰가 정상이다. 15초로 여유를 주고
	//   실제 단절은 CONNECTED_YN='N' 또는 15초 무응답으로 판정한다.
	if (nEQP_TIME > 15 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_1, _T("SC_1호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_1, _T("SC_1호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
		
	}
	return;
}


void CMainFrame::OnUpdateStatusSc2(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;
	
	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("902"));

	if (pSC_DATA == NULL)   // [LGLS 2026-09-09] 902 만 이 검사가 빠져 있었다
		return;

	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	// [LGLS 2026-08-04] 5초는 미러 사이클(~3초)에 여유가 없어 한 사이클만 느어도 벰가 정상이다. 15초로 여유를 주고
	//   실제 단절은 CONNECTED_YN='N' 또는 15초 무응답으로 판정한다.
	if (nEQP_TIME > 15 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_2, _T("SC_2호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_2, _T("SC_2호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
		
	}
	return;
}


void CMainFrame::OnUpdateStatusSc3(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;
	
	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("903"));
	
	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	// [LGLS 2026-08-04] 5초는 미러 사이클(~3초)에 여유가 없어 한 사이클만 느어도 벰가 정상이다. 15초로 여유를 주고
	//   실제 단절은 CONNECTED_YN='N' 또는 15초 무응답으로 판정한다.
	if (nEQP_TIME > 15 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_3, _T("SC_3호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_3, _T("SC_3호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
		
	}
	return;
}


void CMainFrame::OnUpdateStatusSc4(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;
	
	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("904"));
	
	// [LGLS 2026-09-09] 수집 스레드가 같은 CString 을 갱신하는 중이면 복사가 죽는다
	//   (크래시 덤프 3건 - CMainFrame::OnUpdateStatusCv1). 설비 데이터 락으로 읽는다.
	CSingleLock _lockEqp(&pDoc->m_csEqpData, TRUE);
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	// [LGLS 2026-08-04] 5초는 미러 사이클(~3초)에 여유가 없어 한 사이클만 느어도 벰가 정상이다. 15초로 여유를 주고
	//   실제 단절은 CONNECTED_YN='N' 또는 15초 무응답으로 판정한다.
	if (nEQP_TIME > 15 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_4, _T("SC_4호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_4, _T("SC_4호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
		
	}
	return;
}
// [LGLS] SCH pane = IO_TASK (scheduler) health.
//   IO_TASK may run on a REMOTE server, so this client cannot inspect the OS process directly.
//   Instead IO_TASK writes a heartbeat row into EQP_MST (EQP_TYP='SCH'):
//     UPD_DT       -> refreshed every cycle while the scheduler loop is healthy
//     CONNECTED_YN -> 'Y' healthy, 'N' the scheduler itself reported an error
//   RED   : heartbeat stale (process down / hung / host unreachable) OR CONNECTED_YN='N' (error state)
//   GREEN : heartbeat fresh AND healthy
//   This mirrors how EQUIP is judged (OnUpdateStatusCv1), just sourced from the SCH row.
void CMainFrame::OnUpdateStatusSch(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;

	if (pDoc->m_blConnectStatus != TRUE)
		return;

	CConnectStatus* pConnectStatus = pDoc->m_pConnectStatus;
	if (pConnectStatus == NULL)
		return;

	CString strEQP_TIME  = pConnectStatus->GetTime3Safe();
	CString strEQP_CONN  = pConnectStatus->GetConn3Safe();
	CString strEQP_COLOR = pConnectStatus->GetColor3Safe();
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > SCH_ALIVE_SEC || strEQP_CONN == _T("N"))
	{
		// only repaint on change, otherwise the pane flickers every UI update
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SCH, _T("SCH"), RED, BLACK);
			SetCommColor(ID_STATUS_SCH, LGLS_COMM_NG);
			pConnectStatus->SetEQP_COLOR3(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SCH, _T("SCH"), GREEN, BLACK);
			SetCommColor(ID_STATUS_SCH, LGLS_COMM_OK);
			pConnectStatus->SetEQP_COLOR3(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusHost(CCmdUI *pCmdUI)
{
	//헷갈리겠지만 이 함수가 EQP_MST에서 EQP_TYP = HOST2 (E2W)

	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;

	//CConnectStatus가 생성된 후에 로직에 타게끔
	if (pDoc->m_blConnectStatus == TRUE)
	{
		CConnectStatus* pConnectStatus = pDoc->m_pConnectStatus;

		CString strEQP_TIME = pConnectStatus->GetTime2Safe();
		CString strEQP_COLOR = pConnectStatus->GetColor2Safe();
		int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		
		//HOST_IF_LOG에 들어온 메시지가 59초 이상 경과 되었을 떄 에러로 표시
		if (nEQP_TIME > 58)
		{
			//그 전의 색상과 비교해서 다를떄만 변경. 그렇지 않으면 계속 깜빡거림
			if(strEQP_COLOR != _T("RED"))
			{
				m_wndStatusBar.SetPaneInfo(ID_STATUS_HOST, _T("HOST"), RED, BLACK);
				SetCommColor(ID_STATUS_HOST, LGLS_COMM_NG);
				pConnectStatus->SetEQP_COLOR2(_T("RED"));
			}
		}
		else
		{
			if(strEQP_COLOR != _T("GREEN"))
			{
				m_wndStatusBar.SetPaneInfo(ID_STATUS_HOST, _T("HOST"), GREEN, BLACK);
				SetCommColor(ID_STATUS_HOST, LGLS_COMM_OK);
				pConnectStatus->SetEQP_COLOR2(_T("GREEN"));
			}
		}
	}
	
	return;
}

// ===========================================================================
// [LGLS 2026-09-12] 제목줄 (Ecs.ini [Title]) / 리본 경로 툴팁 (Ecs.ini [RibbonMenu]) - 사용자 지시
//   제목 = "Ecs V1.0   [Build:yyyy.MM.dd HH:mm:ss] [DB:DB명@서버][실행 파일 경로]"
//   · 빌드 시각은 PE 헤더의 링크 시각(TimeDateStamp) - 파일마다 다시 컴파일되지 않는 __DATE__ 보다 정확하다.
//   · 창 폭(캡션 글꼴로 측정)보다 길면 0.2초마다 한 글자씩 왼쪽으로 흘린다.
//   · MFC 는 문서 제목("Ecs")으로 제목을 되돌리려 하므로 OnUpdateFrameTitle 을 가로챈다.
// ===========================================================================
static CString PfExeBuildTime()
{
	HMODULE hMod = ::GetModuleHandle(NULL);
	if (hMod == NULL) return _T("");
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)hMod;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE) return _T("");
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((BYTE*)hMod + pDos->e_lfanew);
	if (pNt->Signature != IMAGE_NT_SIGNATURE) return _T("");
	CTime t((__time64_t)pNt->FileHeader.TimeDateStamp);
	return t.Format(_T("%Y.%m.%d %H:%M:%S"));
}

CString CMainFrame::BuildTitle()
{
	CString s;
	s.Format(_T("%s V%s"), AfxGetAppName(), WCS_VERSION_STR);
	if (m_nTitleBuild != 0)
	{
		static CString s_strBuild = PfExeBuildTime();
		s += _T("   [Build:") + s_strBuild + _T("]");
	}
	if (m_nTitleDb != 0)
	{
		TCHAR szSvr[_MAX_PATH] = {0}, szDb[_MAX_PATH] = {0};
#if ORACLE
		::GetPrivateProfileString(_T("DB_1"), _T("SERVER"), _T(""), szSvr, _MAX_PATH, ECS_INI_FILE);
		::GetPrivateProfileString(_T("DB_1"), _T("USERID"), _T(""), szDb, _MAX_PATH, ECS_INI_FILE);
#else
		::GetPrivateProfileString(_T("DB_2"), _T("SERVER"), _T(""), szSvr, _MAX_PATH, ECS_INI_FILE);
		::GetPrivateProfileString(_T("DB_2"), _T("DATABASE"), _T(""), szDb, _MAX_PATH, ECS_INI_FILE);
#endif
		CString strDb;
		strDb.Format(_T(" [DB:%s@%s]"), szDb, szSvr);
		s += strDb;
	}
	if (m_nTitlePath != 0)
	{
		TCHAR szPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, szPath, MAX_PATH);
		s += _T("[") + CString(szPath) + _T("]");
	}
	return s;
}

// 제목이 창 폭에 들어가면 그대로, 넘치면 m_nTitleOfs 부터 폭에 맞게 잘라 보여준다(OnTimer 가 0.2초마다 한 글자씩 민다).
void CMainFrame::UpdateTitleText(BOOL bForce)
{
	if (GetSafeHwnd() == NULL || m_strTitleFull.IsEmpty()) return;
	CString strShow = m_strTitleFull;
	CRect rcWin; GetWindowRect(&rcWin);
	int nAvail = rcWin.Width() - 260;			// 아이콘·빠른실행 도구모음·창 버튼 자리를 뺀 폭
	if (nAvail > 50)
	{
		NONCLIENTMETRICS ncm; ZeroMemory(&ncm, sizeof(ncm)); ncm.cbSize = sizeof(ncm);
		CFont font; BOOL bFont = FALSE;
		if (::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0)) bFont = font.CreateFontIndirect(&ncm.lfCaptionFont);
		CWindowDC dc(this);
		CFont* pOld = bFont ? dc.SelectObject(&font) : NULL;
		if (dc.GetTextExtent(m_strTitleFull).cx > nAvail)
		{
			CString strLoop = m_strTitleFull + _T("      ");		// 한 바퀴 = 전체 + 여백
			int L = strLoop.GetLength();
			if (m_nTitleOfs >= L) m_nTitleOfs = 0;
			CString strRot = strLoop.Mid(m_nTitleOfs) + strLoop.Left(m_nTitleOfs);
			int nFit = strRot.GetLength();
			while (nFit > 1 && dc.GetTextExtent(strRot.Left(nFit)).cx > nAvail) nFit -= (nFit / 16 > 0 ? nFit / 16 : 1);
			while (nFit < strRot.GetLength() && dc.GetTextExtent(strRot.Left(nFit + 1)).cx <= nAvail) nFit++;
			strShow = strRot.Left(nFit);
		}
		else
			m_nTitleOfs = 0;
		if (pOld != NULL) dc.SelectObject(pOld);
	}
	if (bForce || strShow != m_strTitleShown)
	{
		m_strTitleShown = strShow;
		SetWindowText(strShow);
	}
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// MFC 기본 동작은 문서 제목("Ecs")으로 덮어쓴다. 조립된 제목이 있으면 그것을 유지한다.
	if (m_strTitleFull.IsEmpty()) { CFrameWndEx::OnUpdateFrameTitle(bAddToTitle); return; }
	UpdateTitleText(TRUE);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_TITLE_MARQUEE)
	{
		if (m_nTitleBuild < 0)
			ReloadTitleAndTipIni();			// 첫 틱 : ini 를 읽고 제목을 조립한다
		else
		{
			m_nTitleOfs++;					// 넘칠 때만 의미 있다(안 넘치면 UpdateTitleText 가 0 으로 되돌린다)
			UpdateTitleText(FALSE);
		}
		if (!m_bRibbonTipApplied && m_wndRibbonBar.GetSafeHwnd() != NULL && m_wndRibbonBar.GetCategoryCount() > 0)
			ApplyRibbonToolTipIni(TRUE);
		return;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

// Ecs.ini [Title]/[RibbonMenu] 를 (다시) 읽는다. 기동 첫 틱과 CEcsView::ReloadIniHot(ini 저장 감지)에서 부른다.
CString CMainFrame::ReloadTitleAndTipIni()
{
	CString strChg = _T("");
	BOOL bFirst = (m_nTitleBuild < 0);
	int nB = (::GetPrivateProfileInt(_T("Title"), _T("BuildDate"), 1, ECS_INI_FILE) != 0) ? 1 : 0;
	int nD = (::GetPrivateProfileInt(_T("Title"), _T("DbInfo"),    1, ECS_INI_FILE) != 0) ? 1 : 0;
	int nP = (::GetPrivateProfileInt(_T("Title"), _T("Path"),      1, ECS_INI_FILE) != 0) ? 1 : 0;
	int nT = (::GetPrivateProfileInt(_T("RibbonMenu"), _T("ToolTip"), 1, ECS_INI_FILE) != 0) ? 1 : 0;
	if (nB != m_nTitleBuild) { if (!bFirst) strChg.AppendFormat(_T(" Title.BuildDate=%d"), nB); m_nTitleBuild = nB; }
	if (nD != m_nTitleDb)    { if (!bFirst) strChg.AppendFormat(_T(" Title.DbInfo=%d"),    nD); m_nTitleDb    = nD; }
	if (nP != m_nTitlePath)  { if (!bFirst) strChg.AppendFormat(_T(" Title.Path=%d"),      nP); m_nTitlePath  = nP; }
	if (nT != m_nRibbonTip)
	{
		if (m_nRibbonTip >= 0) strChg.AppendFormat(_T(" RibbonMenu.ToolTip=%d"), nT);
		m_nRibbonTip = nT;
		ApplyRibbonToolTipIni(TRUE);
	}
	CString strNew = BuildTitle();
	if (strNew != m_strTitleFull)
	{
		m_strTitleFull = strNew;
		m_nTitleOfs = 0;
		UpdateTitleText(TRUE);
	}
	return strChg;
}

// [RibbonMenu] ToolTip=1 : 리본 버튼 툴팁을 "탭 > 패널 > 버튼" 경로로, 0 : 원래 툴팁으로 되돌린다.
//   탭·패널 이름은 언어 전환(CEcsDoc::UpdateRibbonLang)으로 바뀌므로 그때도 다시 부른다.
// 적용 증거(UI 로그) : 적용한 버튼 수와 첫 버튼의 툴팁 - 툴팁은 화면 캡처로 잡기 어려워 로그로 확인한다
static int     s_nTipCnt = 0;
static CString s_strTipSample;

void CMainFrame::ApplyRibbonToolTipIni(BOOL bForce)
{
	UNREFERENCED_PARAMETER(bForce);
	if (m_wndRibbonBar.GetSafeHwnd() == NULL) return;
	int nCat = m_wndRibbonBar.GetCategoryCount();
	if (nCat <= 0) return;
	if (m_nRibbonTip < 0) m_nRibbonTip = (::GetPrivateProfileInt(_T("RibbonMenu"), _T("ToolTip"), 1, ECS_INI_FILE) != 0) ? 1 : 0;
	BOOL bOn = (m_nRibbonTip != 0);
	s_nTipCnt = 0; s_strTipSample.Empty();
	for (int c = 0; c < nCat; c++)
	{
		CMFCRibbonCategory* pCat = m_wndRibbonBar.GetCategory(c);
		if (pCat == NULL) continue;
		if (pCat == m_wndRibbonBar.GetMainCategory() || !pCat->IsVisible()) continue;	// 응용 프로그램 단추 패널·인쇄 미리보기 등 화면에 없는 탭 제외
		CString strCat = pCat->GetName(); strCat.Trim();
		for (int i = 0; i < pCat->GetPanelCount(); i++)
		{
			CMFCRibbonPanel* pPanel = pCat->GetPanel(i);
			if (pPanel == NULL) continue;
			CString strPanel = pPanel->GetName(); strPanel.Trim();
			for (int e = 0; e < pPanel->GetCount(); e++)
				SetElemPathTip(pPanel->GetElement(e), strCat, strPanel, bOn);
		}
	}
	m_bRibbonTipApplied = TRUE;
	CLib::UiLog(_T("[INI] ribbon tooltip %s n=%d sample=%s"), bOn ? _T("on") : _T("off"), s_nTipCnt, (LPCTSTR)s_strTipSample);
}

void CMainFrame::SetElemPathTip(CMFCRibbonBaseElement* pElem, const CString& strCat, const CString& strPanel, BOOL bOn)
{
	if (pElem == NULL) return;
	if (pElem->IsKindOf(RUNTIME_CLASS(CMFCRibbonButtonsGroup)))		// 버튼 묶음이면 안의 버튼마다
	{
		CMFCRibbonButtonsGroup* pGrp = (CMFCRibbonButtonsGroup*)pElem;
		for (int i = 0; i < pGrp->GetCount(); i++) SetElemPathTip(pGrp->GetButton(i), strCat, strPanel, bOn);
		return;
	}
	CString strText = pElem->GetText();
	strText.Replace(_T("\n"), _T(" "));
	strText.Trim();
	if (strText.IsEmpty()) return;							// 구분선·여백 라벨
	void* pKey = (void*)pElem;
	CString strOrig;
	if (!m_mapTipOrig.Lookup(pKey, strOrig))
	{
		strOrig = pElem->GetToolTipText();
		m_mapTipOrig.SetAt(pKey, strOrig);
	}
	if (bOn)
	{
		CString strPath;
		strPath.Format(_T("%s > %s > %s"), (LPCTSTR)strCat, (LPCTSTR)strPanel, (LPCTSTR)strText);
		pElem->SetToolTipText(strPath);
		s_nTipCnt++;
		if (s_strTipSample.IsEmpty()) s_strTipSample = strPath;
	}
	else
	{
		pElem->SetToolTipText(strOrig);
		s_nTipCnt++;
		if (s_strTipSample.IsEmpty()) s_strTipSample = strText + _T(" -> ") + (strOrig.IsEmpty() ? CString(_T("(none)")) : strOrig);
	}
}
