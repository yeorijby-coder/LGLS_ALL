// Ecs.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Ecs.h"

#include "MainFrm.h"
#include "EcsDoc.h"
#include "EcsView.h"
//#include "Splash.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEcsApp

BEGIN_MESSAGE_MAP(CEcsApp, CWinApp)
	//{{AFX_MSG_MAP(CEcsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEcsApp construction

CEcsApp::CEcsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEcsApp object

CEcsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEcsApp initialization

BOOL CEcsApp::InitInstance()
{
//	HANDLE hMutex = ::CreateMutex(NULL, TRUE, _T("SC_SIMULATOR"));
//	if (::GetLastError() == ERROR_ALREADY_EXISTS)
//	{
//		AfxMessageBox(_T("이미 ECS SERVER 프로그램이 실행중입니다."));
//		::CloseHandle(hMutex);
//		return FALSE;
//	}

	::CoInitialize(NULL);

	// CG: The following block was added by the Splash Screen component.
//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);
//	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	/*
	CLogin dlg;
	
	if (dlg.DoModal() != IDOK)	return FALSE;
	m_strCurID = dlg.m_strUserID;
	m_strCurPwd = dlg.m_strPassword;
	*/
	
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("SFA"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEcsDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEcsView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
	{
		AfxMessageBox(_T("프로그램 초기화에 실패했습니다.\n명령줄 인자 또는 리소스를 확인하세요."));
		// 필요하다면 로그 파일에 기록
		// FILE* fp = _tfopen(_T("error.log"), _T("a"));
		// if (fp) { _ftprintf(fp, _T("ProcessShellCommand 실패\n")); fclose(fp); }
		return FALSE;
	}

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->SetWindowText(_T("Stacker Crain Simulation - Ver 1.0 (2020.03.11)"));
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("COMMON"), _T("Title"), _T("Stacker Crain Simulation"), szTemp, _MAX_PATH, ECS_INI_FILE);
//	m_strEcsPath.Format(_T("%s"),		szTemp);

	m_pMainWnd->SetWindowText(szTemp);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CEcsApp::ExitInstance() 
{
	::CoUninitialize();

	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CEcsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
BOOL CEcsApp::PreTranslateMessage(MSG* pMsg)
{

	
	return CWinApp::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////
// CEcsApp message handlers


