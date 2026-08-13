// ConfigStdPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "EcsDoc.h"
#include "EcsView.h"

#include "ConfigSub1Page.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CConfigSub1Page, CPropertyPage)

////////////////////////////////
// CConfigSub1Page property page

CConfigSub1Page::CConfigSub1Page() : CPropertyPage(CConfigSub1Page::IDD)
{
	//{{AFX_DATA_INIT(CConfigSub1Page)
	m_strJobPath = _T("");
	m_strLogPath = _T("");
	m_strStcPath = _T("");
	m_strInfoPath = _T("");
	m_strJobFile = _T("");
	m_strLogFile = _T("");
	m_strStcFile = _T("");
	m_strInfoFile = _T("");
	//}}AFX_DATA_INIT
}

CConfigSub1Page::CConfigSub1Page(CEcsDoc* pDoc, CConfig* pConfig) : CPropertyPage(CConfigSub1Page::IDD)
{
	m_pDoc = pDoc;
	m_pConfig = pConfig;
}

CConfigSub1Page::~CConfigSub1Page()
{
}

void CConfigSub1Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigSub1Page)
	DDX_Text(pDX, IDC_EDIT_JOB_PATH, m_strJobPath);
	DDX_Text(pDX, IDC_EDIT_LOG_PATH, m_strLogPath);
	DDX_Text(pDX, IDC_EDIT_STC_PATH, m_strStcPath);
	DDX_Text(pDX, IDC_EDIT_INFO_PATH, m_strInfoPath);
	DDX_Text(pDX, IDC_EDIT_JOB_FILE, m_strJobFile);
	DDX_Text(pDX, IDC_EDIT_LOG_FILE, m_strLogFile);
	DDX_Text(pDX, IDC_EDIT_STC_FILE, m_strStcFile);
	DDX_Text(pDX, IDC_EDIT_INFO_FILE, m_strInfoFile);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_JOB_PATH, m_btnJobPath);
	DDX_Control(pDX, IDC_BUTTON_LOG_PATH, m_btnLogPath);
	DDX_Control(pDX, IDC_BUTTON_STC_PATH, m_btnStcPath);
	DDX_Control(pDX, IDC_BUTTON_INFO_PATH, m_btnInfoPath);
	DDX_Control(pDX, IDC_BTN_AUTO_STO, m_btnAutoSto);
	DDX_Control(pDX, IDC_BTN_AUTO_RET, m_btnAutoRet);
	DDX_Control(pDX, IDC_BTN_AUTO_MOVE, m_btnAutoMove);
	DDX_Control(pDX, IDC_BTN_AUTO_RTR, m_btnAutoRtR);
	DDX_Control(pDX, IDC_BTN_AUTO_WTW, m_btnAutoWtW);
	DDX_Control(pDX, IDC_BTN_SEMI_STO, m_btnSemiSto);
	DDX_Control(pDX, IDC_BTN_SEMI_RET, m_btnSemiRet);
	DDX_Control(pDX, IDC_BTN_SEMI_MOVE, m_btnSemiMove);
	DDX_Control(pDX, IDC_BTN_SEMI_RTR, m_btnSemiRtR);
	DDX_Control(pDX, IDC_BTN_SEMI_WTW, m_btnSemiWtW);
	DDX_Control(pDX, IDC_BTN_TRACK_STO_ST, m_btnStoST);
	DDX_Control(pDX, IDC_BTN_TRACK_RET_ST, m_btnRetST);
	DDX_Control(pDX, IDC_BTN_TRACK_ARV_ST, m_btnArvST);
	DDX_Control(pDX, IDC_BTN_TRACK_DEPART_HS, m_btnDepart);
	DDX_Control(pDX, IDC_BTN_TRACK_ARRIVE_HS, m_btnArrive);
	DDX_Control(pDX, IDC_BTN_DISCONNECT, m_btnDisconnect);
	DDX_Control(pDX, IDC_BTN_SUSPEND, m_btnSuspend);
	DDX_Control(pDX, IDC_BTN_MANUAL, m_btnManual);
	DDX_Control(pDX, IDC_BTN_ALARM, m_btnAlarm);
	DDX_Control(pDX, IDC_BTN_ERROR, m_btnError);
	DDX_Control(pDX, ID_BUTTON_SAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CConfigSub1Page, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigSub1Page)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_JOB_PATH, OnButtonJobPath)
	ON_BN_CLICKED(IDC_BUTTON_LOG_PATH, OnButtonLogPath)
	ON_BN_CLICKED(IDC_BUTTON_STC_PATH, OnButtonStcPath)
	ON_BN_CLICKED(IDC_BUTTON_INFO_PATH, OnButtonInfoPath)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_RESET, OnButtonColorReset)
	ON_BN_CLICKED(IDC_BUTTON_PATH_RESET, OnButtonPathReset)
	ON_BN_CLICKED(ID_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CConfigSub1Page::OnInitDialog() 
{
	m_strJobPath = m_pConfig->m_strJobPath;
	m_strLogPath = m_pConfig->m_strLogPath;
	m_strStcPath = m_pConfig->m_strStcPath;
	m_strInfoPath = m_pConfig->m_strInfoPath;

	m_strLogFile = "Log**.elg";

	CPropertyPage::OnInitDialog();

	m_btnAutoSto.m_clrCurrent	= m_pConfig->m_clrAutoSto;
	m_btnAutoRet.m_clrCurrent	= m_pConfig->m_clrAutoRet;
	m_btnAutoMove.m_clrCurrent	= m_pConfig->m_clrAutoMove;
	m_btnAutoRtR.m_clrCurrent	= m_pConfig->m_clrAutoR2R;
	m_btnAutoWtW.m_clrCurrent	= m_pConfig->m_clrAutoW2W;

	m_btnSemiSto.m_clrCurrent	= m_pConfig->m_clrSemiSto;
	m_btnSemiRet.m_clrCurrent	= m_pConfig->m_clrSemiRet;
	m_btnSemiMove.m_clrCurrent	= m_pConfig->m_clrSemiMove;
	m_btnSemiRtR.m_clrCurrent	= m_pConfig->m_clrSemiR2R;
	m_btnSemiWtW.m_clrCurrent	= m_pConfig->m_clrSemiW2W;

	m_btnStoST.m_clrCurrent		= m_pConfig->m_clrStoST;
	m_btnRetST.m_clrCurrent		= m_pConfig->m_clrRetST;
	m_btnArvST.m_clrCurrent		= m_pConfig->m_clrArvST;
	m_btnDepart.m_clrCurrent	= m_pConfig->m_clrDepart;
	m_btnArrive.m_clrCurrent	= m_pConfig->m_clrArrive;

	m_btnDisconnect.m_clrCurrent= m_pConfig->m_clrDisconnect;
	m_btnSuspend.m_clrCurrent	= m_pConfig->m_clrSuspend;
	m_btnManual.m_clrCurrent	= m_pConfig->m_clrManual;
	m_btnAlarm.m_clrCurrent		= m_pConfig->m_clrAlarm;
	m_btnError.m_clrCurrent		= m_pConfig->m_clrError;

	SetSxButton();
//	SetChildWndFont();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigSub1Page::SetChildWndFont()
{
	CFont font;
	font.CreateFont (10, 0, 0, 0, 100, FALSE, 0, 0, 1, 0, 0, 0, 0, _T("System") );

	CWnd* pChildWnd = GetWindow(GW_CHILD);
	while(pChildWnd)
	{
		pChildWnd->SetFont(&font);
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CConfigSub1Page::SetSxButton()
{
	INIT_BUTTON(m_btnSave, IDI_SAVE, 18, 18, _T("현재 설정된 값으로 저장"));

	m_btnJobPath.SetIcon(IDI_OPEN, 20, 20);
	m_btnLogPath.SetIcon(IDI_OPEN, 20, 20);
	m_btnStcPath.SetIcon(IDI_OPEN, 20, 20);
	m_btnInfoPath.SetIcon(IDI_OPEN, 20, 20);
}

void CConfigSub1Page::OnButtonDisplay() 
{
/*
	CDisplayDlg Dlg(m_pDoc);
	Dlg.DoModal();
*/
}

void CConfigSub1Page::OnButtonJobPath() 
{
	char szFolderName[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szFolderName;
	bi.lpszTitle = _T("작업정보 폴더");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_EDITBOX;
	bi.lpfn = NULL;

	char szFullPathName[MAX_PATH];
	LPITEMIDLIST lpID = SHBrowseForFolder(&bi);
	if (lpID != NULL)
	{
		if ( SHGetPathFromIDList(lpID, (LPWSTR)szFullPathName) )
			SetDlgItemText(IDC_EDIT_JOB_PATH, (LPWSTR)szFullPathName);
	}
}

void CConfigSub1Page::OnButtonLogPath() 
{
	char szFolderName[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szFolderName;
	bi.lpszTitle = _T("로그정보 폴더");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_EDITBOX;
	bi.lpfn = NULL;

	char szFullPathName[MAX_PATH];
	LPITEMIDLIST lpID = SHBrowseForFolder(&bi);
	if (lpID != NULL)
	{
		if ( SHGetPathFromIDList(lpID, (LPWSTR)szFullPathName) )
			SetDlgItemText(IDC_EDIT_LOG_PATH, (LPWSTR)szFullPathName);
	}	
}

void CConfigSub1Page::OnButtonStcPath() 
{
	char szFolderName[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szFolderName;
	bi.lpszTitle = _T("통계정보 폴더");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_EDITBOX;
	bi.lpfn = NULL;

	char szFullPathName[MAX_PATH];
	LPITEMIDLIST lpID = SHBrowseForFolder(&bi);
	if (lpID != NULL)
	{
		if ( SHGetPathFromIDList(lpID, (LPWSTR)szFullPathName) )
			SetDlgItemText(IDC_EDIT_STC_PATH, (LPWSTR)szFullPathName);
	}	
}

void CConfigSub1Page::OnButtonInfoPath() 
{
	char szFolderName[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szFolderName;
	bi.lpszTitle = _T("설비정보 폴더");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_EDITBOX;
	bi.lpfn = NULL;

	char szFullPathName[MAX_PATH];
	LPITEMIDLIST lpID = SHBrowseForFolder(&bi);
	if (lpID != NULL)
	{
		if ( SHGetPathFromIDList(lpID, (LPWSTR)szFullPathName) )
			SetDlgItemText(IDC_EDIT_INFO_PATH, (LPWSTR)szFullPathName);
	}	
}

void CConfigSub1Page::OnButtonSave() 
{
	if (AfxMessageBox(_T("변경된 내용을 저장 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return ;

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

	m_pConfig->m_clrAutoSto		= m_btnAutoSto.m_clrCurrent;
	m_pConfig->m_clrAutoRet		= m_btnAutoRet.m_clrCurrent;
	m_pConfig->m_clrAutoMove	= m_btnAutoMove.m_clrCurrent;
	m_pConfig->m_clrAutoR2R		= m_btnAutoRtR.m_clrCurrent;
	m_pConfig->m_clrAutoW2W		= m_btnAutoWtW.m_clrCurrent;

	m_pConfig->m_clrSemiSto		= m_btnSemiSto.m_clrCurrent;
	m_pConfig->m_clrSemiRet		= m_btnSemiRet.m_clrCurrent;
	m_pConfig->m_clrSemiMove	= m_btnSemiMove.m_clrCurrent;
	m_pConfig->m_clrSemiR2R		= m_btnSemiRtR.m_clrCurrent;
	m_pConfig->m_clrSemiW2W		= m_btnSemiWtW.m_clrCurrent;

	m_pConfig->m_clrStoST		= m_btnStoST.m_clrCurrent;
	m_pConfig->m_clrRetST		= m_btnRetST.m_clrCurrent;
	m_pConfig->m_clrArvST		= m_btnArvST.m_clrCurrent;
	m_pConfig->m_clrDepart		= m_btnDepart.m_clrCurrent;
	m_pConfig->m_clrArrive		= m_btnArrive.m_clrCurrent;

	m_pConfig->m_clrDisconnect	= m_btnDisconnect.m_clrCurrent;
	m_pConfig->m_clrSuspend		= m_btnSuspend.m_clrCurrent;
	m_pConfig->m_clrManual		= m_btnManual.m_clrCurrent;
	m_pConfig->m_clrAlarm		= m_btnAlarm.m_clrCurrent;
	m_pConfig->m_clrError		= m_btnError.m_clrCurrent;

	m_pConfig->m_strJobPath		= m_strJobPath;
	m_pConfig->m_strLogPath		= m_strLogPath;
	m_pConfig->m_strStcPath		= m_strStcPath;
	m_pConfig->m_strInfoPath	= m_strInfoPath;
//	JOB->ModifyJobPath(m_strJobPath);

	m_pConfig->SaveConfig2();
	m_pDoc->InvokeControl();
}

void CConfigSub1Page::OnButtonColorReset() 
{
	if (AfxMessageBox(_T("색상 정보를 초기화 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return ;

	m_pConfig->InitializeColor();

	m_btnAutoSto.m_clrCurrent	= m_pConfig->m_clrAutoSto;
	m_btnAutoRet.m_clrCurrent	= m_pConfig->m_clrAutoRet;
	m_btnAutoMove.m_clrCurrent	= m_pConfig->m_clrAutoMove;
	m_btnAutoRtR.m_clrCurrent	= m_pConfig->m_clrAutoR2R;
	m_btnAutoWtW.m_clrCurrent	= m_pConfig->m_clrAutoW2W;

	m_btnSemiSto.m_clrCurrent	= m_pConfig->m_clrSemiSto;
	m_btnSemiRet.m_clrCurrent	= m_pConfig->m_clrSemiRet;
	m_btnSemiMove.m_clrCurrent	= m_pConfig->m_clrSemiMove;
	m_btnSemiRtR.m_clrCurrent	= m_pConfig->m_clrSemiR2R;
	m_btnSemiWtW.m_clrCurrent	= m_pConfig->m_clrSemiW2W;

	m_btnStoST.m_clrCurrent		= m_pConfig->m_clrStoST;
	m_btnRetST.m_clrCurrent		= m_pConfig->m_clrRetST;
	m_btnArvST.m_clrCurrent		= m_pConfig->m_clrArvST;
	m_btnDepart.m_clrCurrent	= m_pConfig->m_clrDepart;
	m_btnArrive.m_clrCurrent	= m_pConfig->m_clrArrive;

	m_btnDisconnect.m_clrCurrent= m_pConfig->m_clrDisconnect;
	m_btnSuspend.m_clrCurrent	= m_pConfig->m_clrSuspend;
	m_btnManual.m_clrCurrent	= m_pConfig->m_clrManual;
	m_btnAlarm.m_clrCurrent		= m_pConfig->m_clrAlarm;
	m_btnError.m_clrCurrent		= m_pConfig->m_clrError;

	Invalidate();
}

void CConfigSub1Page::OnButtonPathReset() 
{
	if (AfxMessageBox(_T("경로 설정을 초기화 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return ;

	m_pConfig->InitializePath();

	m_strJobPath = m_pConfig->m_strJobPath;
	m_strLogPath = m_pConfig->m_strLogPath;
	m_strStcPath = m_pConfig->m_strStcPath;
	m_strInfoPath = m_pConfig->m_strInfoPath;

	UpdateData(FALSE);
}

/*
void CConfigSub1Page::OnButtonSideLogo() 
{
	char	szOldDir[128];
	char	szNewDir[512];

	GetCurrentDirectory(sizeof(szOldDir), szOldDir);
	SetCurrentDirectory(szOldDir);
	GetCurrentDirectory(sizeof(szNewDir), szNewDir);

	CFileDialog FileDlg(TRUE, NULL, "*.bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		" ECS Logo File (*.bmp) | *.bmp ||");

	FileDlg.m_ofn.lpstrInitialDir = szNewDir;
	FileDlg.m_ofn.lpstrTitle = "ECS LOGO 파일";

	if (FileDlg.DoModal() == IDOK) 
	{
		CString strLogoFile = FileDlg.GetPathName();
		SET(IDC_EDIT_SIDE_LOGO, strLogoFile);
	}
	SetCurrentDirectory(szOldDir);
}

void CConfigSub1Page::OnButtonMenuLogo() 
{
	char	szOldDir[128];
	char	szNewDir[512];

	GetCurrentDirectory(sizeof(szOldDir), szOldDir);
	SetCurrentDirectory(szOldDir);
	GetCurrentDirectory(sizeof(szNewDir), szNewDir);

	CFileDialog FileDlg(TRUE, NULL, "*.bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		" ECS Logo File (*.bmp) | *.bmp ||");

	FileDlg.m_ofn.lpstrInitialDir = szNewDir;
	FileDlg.m_ofn.lpstrTitle = "ECS LOGO 파일";

	if (FileDlg.DoModal() == IDOK) 
	{
		CString strLogoFile = FileDlg.GetPathName();
		SET(IDC_EDIT_MENU_LOGO, strLogoFile);
	}
	SetCurrentDirectory(szOldDir);	
}
*/
