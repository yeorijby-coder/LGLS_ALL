// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Ecs.h"
#include "MainFrm.h"
#include "Splash.h"
#include "MFCRibbonPanel_Wrap.h"
#include "MinButton.h"
#include "RecordSetWrap.h"

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
	ON_UPDATE_COMMAND_UI(ID_STATUS_WC_1, &CMainFrame::OnUpdateStatusWc1)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_1, &CMainFrame::OnUpdateStatusSc1)
//	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_2, &CMainFrame::OnUpdateStatusSc2)
//	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_3, &CMainFrame::OnUpdateStatusSc3)
//	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_4, &CMainFrame::OnUpdateStatusSc4)
	ON_UPDATE_COMMAND_UI(ID_STATUS_HOST, &CMainFrame::OnUpdateStatusHost)

	//ON_BN_CLICKED(ID_STATUS_CV_1, &CMainFrame::OnButtonCv1)
	//ON_BN_CLICKED(ID_STATUS_SC_1, &CMainFrame::OnButtonSc1)
	//ON_BN_CLICKED(ID_STATUS_SC_2, &CMainFrame::OnButtonSc2)
	//ON_BN_CLICKED(ID_STATUS_SC_3, &CMainFrame::OnButtonSc3)
	//ON_BN_CLICKED(ID_STATUS_SC_4, &CMainFrame::OnButtonSc4)
	//ON_BN_CLICKED(ID_STATUS_WC_1, &CMainFrame::OnButtonWc1)
	//ON_BN_CLICKED(ID_STATUS_HOST, &CMainFrame::OnButtonHost)

	ON_CONTROL_RANGE(BN_CLICKED, ID_STATUS_CV_1,ID_STATUS_HOST, &CMainFrame::OnButtonComm)

END_MESSAGE_MAP()

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
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_bNotDockingJob = false;
	m_bNotDockingInfo = false;
	m_bShowStatusBar = false;
	m_bToolNMenuBar = false;
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), 0);

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

    m_wndStatusBar.MoveWindow(0, nWindowSizeY, nWindowSizeX, 40);

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
	AddCategoryLOG();
	//AddCategoryUSER();
	//AddCategorySTATUS();
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

	CMFCRibbonButton* pBtnEmptyPlt = new CMFCRibbonButton(ID_VIEW_HOST_EMPTY_PLT, _T("공PLT작업"), HICONFromPATH(GetConcatPath(strAppPath, _T("job"), strExtension)), TRUE);
	pBtnEmptyPlt->SetAlwaysLargeImage();
	pPanelView->Add(pBtnEmptyPlt);

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
		CMFCRibbonButton* pBtn1f = new CMFCRibbonButton(ID_MONITORING_1F, _T("1F"), HICONFromPATH(GetConcatPath(strAppPath, _T("1f"), strExtension)), TRUE);
		pBtn1f->SetAlwaysLargeImage();
		pPanelMonitor->Add(pBtn1f);

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

	CMFCRibbonButton* pBtnManualEmpty = new CMFCRibbonButton(IDD_MANUAL_EMPTY, _T("EMPTY"), HICONFromPATH(GetConcatPath(strAppPath, _T("empty"), strExtension)), TRUE);
	pBtnManualEmpty->SetAlwaysLargeImage();
	pPanelManual->Add(pBtnManualEmpty);

	//CMFCRibbonButton* pBtnManualRtv = new CMFCRibbonButton(ID_MANUAL_RTV, _T("RTV"), HICONFromPATH(GetConcatPath(strAppPath, _T("rtv"), strExtension)), TRUE);
	//pBtnManualRtv->SetAlwaysLargeImage();
	//pPanelManual->Add(pBtnManualRtv);
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

	CMFCRibbonButton* pBtnBCR_LOG = new CMFCRibbonButton(ID_LOG_BCR, _T("BCR_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("bcrlog"), strExtension)), TRUE);
	pBtnBCR_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnBCR_LOG);	   

	CMFCRibbonButton* pBtnCLIENT_LOG = new CMFCRibbonButton(ID_LOG_CLIENT, _T("CLIENT_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("clientlog"), strExtension)), TRUE);
	pBtnCLIENT_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnCLIENT_LOG);

	CMFCRibbonButton* pBtnWCS_LOG = new CMFCRibbonButton(IDD_LOG_WCS_LOG_PGR, _T("WCS_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("wcslog"), strExtension)), TRUE);
	pBtnWCS_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnWCS_LOG);

	//CMFCRibbonButton* pBtnEQP_LOG = new CMFCRibbonButton(ID_LOG_EQP, _T("EQP_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("eqplog"), strExtension)), TRUE);
	//pBtnEQP_LOG->SetAlwaysLargeImage();
	//pPanelLog->Add(pBtnEQP_LOG);

	//CMFCRibbonButton* pBtnCLIENT_LOG = new CMFCRibbonButton(ID_LOG_CLIENT, _T("CLIENT_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("clientlog"), strExtension)), TRUE);
	//pBtnCLIENT_LOG->SetAlwaysLargeImage();
	//pPanelLog->Add(pBtnCLIENT_LOG);

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
	CMFCRibbonButton* pButton_emptyplt = (CMFCRibbonButton*)pPanel_Wrap_View->GetElement(1);
	pButton_emptyplt->SetText(_T("공PLT작업"));
	CMFCRibbonButton* pButton_Search = (CMFCRibbonButton*)pPanel_Wrap_View->GetElement(2);
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
	//CMFCRibbonButton* pBtnManualRtv = (CMFCRibbonButton*)pPanel_Wrap_Manual->GetElement(2);
	//pBtnManualRtv->SetText(CLib::GetIniStringFromPath(strFullPath, _T("rtv"), (int)penLang));
	CMFCRibbonButton* pBtnManualEmpty = (CMFCRibbonButton*)pPanel_Wrap_Manual->GetElement(2);
	pBtnManualEmpty->SetText(CLib::GetIniStringFromPath(strFullPath, _T("empty"), (int)penLang));

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
	CMFCRibbonButton* pBtnLogBcrLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(3);
	pBtnLogBcrLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("bcrlog"), (int)penLang));
 	CMFCRibbonButton* pBtnLogClientLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(4);
 	pBtnLogClientLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("client_log"), (int)penLang));
	CMFCRibbonButton* pBtnLogWcsLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(5);
 	pBtnLogWcsLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("wcs_log"), (int)penLang));
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

	InsertButtonPainToStatusBar(_T("CV_1G"), ID_STATUS_CV_1, i+1, 80);
	InsertButtonPainToStatusBar(_T("SC_1호기"), ID_STATUS_SC_1, i+2, 80);
	InsertButtonPainToStatusBar(_T("SC_2호기"), ID_STATUS_SC_2, i+3, 80);
	InsertButtonPainToStatusBar(_T("SC_3호기"), ID_STATUS_SC_3, i+4, 80);
	InsertButtonPainToStatusBar(_T("SC_4호기"), ID_STATUS_SC_4, i+5, 80);
	InsertButtonPainToStatusBar(_T("WC_104"), ID_STATUS_WC_1, i+6, 80);
	InsertButtonPainToStatusBar(_T("HOST"), ID_STATUS_HOST, i+7, 80);
		
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SR_MODE, _T("MODE"), YELLOW, BLACK); 

	//m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, _T("CV_1G"), YELLOW, BLACK); //배경/글씨
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_1, _T("SC_1호기"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_2, _T("SC_2호기"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_3, _T("SC_3호기"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_4, _T("SC_4호기"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_WC_1, _T("WC_104"), YELLOW, BLACK);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_HOST, _T("HOST"), YELLOW, BLACK);

	//DARK_GRAY,//RGB(255, 128, 192),
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_1, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_2, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_3, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_4, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_WC_1, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_HOST, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);


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
	CWC_DATA* pWC_DATA = NULL;

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
		pTrackInfo = pDoc->GetTrackInfoNew(_T("101"));
	
		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 1;
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
	case ID_STATUS_WC_1:
		#pragma region ID_STATUS_WC_1
		pWC_DATA = pDoc->GetWC_DATA(_T("104"));
	
		if (pWC_DATA == NULL)
			return;
		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pWC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pWC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("WC");
		nEQP_NUM = 1;
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
		strEQP_TIME = pDoc->m_pConnectStatus->V_EQP_TIME2;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("HOST2");
		nEQP_NUM = 1;
		nCheckTime = 120;
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

			strTemp1.Format(_T("%s (IP:%s) (PORT:%s)"), strREMARKS, strIP, strPORT);		

			strTemp2.Format(_T("PingTest를 하겠습니까? [접속정보 -> %s]"), strTemp1);
			
			if (AfxMessageBox(strTemp2, MB_YESNO) != IDYES)
			{
				AfxMessageBox(_T("통신 연결 되지 않았습니다!"));
				return;
			}

			AfxMessageBox(_T("PingTest 하겠습니다.\n\nPing 비정상시 네트워크 담당자에게 확인바랍니다!\nPing 정상시 PORT가 정상적으로 OPEN 되어있는지 확인하세요"));
			//::ShellExecute(NULL, _T("open"), _T(".\\PING_WC01.BAT"), NULL, NULL, SW_SHOW);
			::ShellExecute(NULL, _T("open"), strBAT, NULL, NULL, SW_SHOW);		// 정상
		}
		else
		{
			AfxMessageBox(strTemp1);
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

void CMainFrame::OnButtonWc1()
{
	//AfxMessageBox(_T("야호"));
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	
		return;

	CWC_DATA* pWC_DATA = pDoc->GetWC_DATA(_T("104"));
	
	if (pWC_DATA == NULL)
		return;

	CString strTemp1, strTemp2;
	CString strEQP_CONNECTED_YN = pWC_DATA->V_EQP_CONNECTED_YN;
	strTemp1.Format(_T("WC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.249"),8198);

	CString strEQP_TIME = pWC_DATA->V_EQP_TIME;
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
		::ShellExecute(NULL, _T("open"), _T(".\\PING_WC01.BAT"), NULL, NULL, SW_SHOW);
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

	CWC_DATA* pWC_DATA = pDoc->GetWC_DATA(_T("104"));
	
	if (pWC_DATA == NULL)
		return;

	CString strTemp1, strTemp2;
	strTemp1.Format(_T("HOST (IP:%s) (PORT:%04d)"),_T("10.99.10.141"),8400);

	//CConnectStatus가 생성된 후에 로직에 타게끔
	if (pDoc->m_blConnectStatus == TRUE)
	{
		CConnectStatus* pConnectStatus = pDoc->m_pConnectStatus;

		CString strEQP_TIME = pConnectStatus->V_EQP_TIME2;
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
	
	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("101"));
	
	if (pTrackInfo == NULL)
		return;


	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, _T("CV_1G"), RED, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, _T("CV_1G"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
		
	}
	return;
}


void CMainFrame::OnUpdateStatusWc1(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;
	
	CWC_DATA* pWC_DATA = pDoc->GetWC_DATA(_T("104"));
	
	if (pWC_DATA == NULL)
		return;

	CString strEQP_TIME = pWC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pWC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pWC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_WC_1, _T("WC_104"), RED, BLACK);
			pWC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_WC_1, _T("WC_104"), GREEN, BLACK);
			pWC_DATA->SetEQP_COLOR(_T("GREEN"));
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

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
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
	
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
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
	
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
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
	
	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
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

		CString strEQP_TIME = pConnectStatus->V_EQP_TIME2;
		CString strEQP_COLOR = pConnectStatus->V_EQP_COLOR2;
		int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		
		//HOST_IF_LOG에 들어온 메시지가 59초 이상 경과 되었을 떄 에러로 표시
		if (nEQP_TIME > 58)
		{
			//그 전의 색상과 비교해서 다를떄만 변경. 그렇지 않으면 계속 깜빡거림
			if(strEQP_COLOR != _T("RED"))
			{
				m_wndStatusBar.SetPaneInfo(ID_STATUS_HOST, _T("HOST"), RED, BLACK);
				pConnectStatus->SetEQP_COLOR2(_T("RED"));
			}
		}
		else
		{
			if(strEQP_COLOR != _T("GREEN"))
			{
				m_wndStatusBar.SetPaneInfo(ID_STATUS_HOST, _T("HOST"), GREEN, BLACK);
				pConnectStatus->SetEQP_COLOR2(_T("GREEN"));
			}
		}
	}
	
	return;
}