// SystemLoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "SystemLoginDlg.h"
#include "afxdialogex.h"


// CSystemLoginDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSystemLoginDlg, CSkinDialog)

	CSystemLoginDlg::CSystemLoginDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CSystemLoginDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CSystemLoginDlg::CSystemLoginDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CSystemLoginDlg::IDD, pParent)
{
		m_hIcon = AfxGetApp()->LoadIcon(IDCANCEL);
		m_bInitialized = FALSE;
		m_pDoc = pDoc;
}

CSystemLoginDlg::~CSystemLoginDlg()
{
	int a= 0;
	this->DestroyWindow();
}

void CSystemLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_LOCK, m_btnLock);
	DDX_Control(pDX, IDC_ID1, m_edtId);
	DDX_Control(pDX, IDC_PW1, m_edtPw);
}


BEGIN_MESSAGE_MAP(CSystemLoginDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CSystemLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSystemLoginDlg::OnBnClickedCancel)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(ID_LOCK, &CSystemLoginDlg::OnBnClickedLock)
END_MESSAGE_MAP()



BOOL CSystemLoginDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	if(m_pDoc == NULL)
		return FALSE;
	
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		RedrawImage();
		RenameResource(pEn);
		m_bInitialized = TRUE;
	}
	Invalidate(TRUE);
	
	if(m_pDoc->m_strId == _T(""))
	{
		m_edtId.SetWindowText(_T("LFC"));//ONLY_VIEW
		m_edtPw.SetWindowText(_T("LFC"));//ONLY_VIEW
		OnBnClickedOk();
	}

	return TRUE;  
}


void CSystemLoginDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	//if( m_bInitialized )
		//RelocationControls();
}

HCURSOR CSystemLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSystemLoginDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CSystemLoginDlg::RelocationControls()
{

	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnOk.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnOk.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}


void CSystemLoginDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStringList strList;
	CString strSql, strId, strPw;
	m_edtId.GetWindowText(strId);
	m_edtPw.GetWindowText(strPw);

	strId.Trim();
	strPw.Trim();

	if(strId.GetLength() <= 0 || strPw.GetLength() <= 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("ID, PASSWORD를 확인하세요")));
		return;
	}

	//클라이언트 최초 실행 때 로그인 여부를 false로 설정
	if(strId == "LFC")//ONLY_VIEW
	{
		m_blLogYn = FALSE;
	}

	if (m_blLogYn == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 로그인되어 있습니다. 새로운 아이디로 접속하시려면 로그아웃을 해주세요.")));
		return;
	}

	strSql.Format(_T(" SELECT USER_ID ")
				  _T("	 FROM USER_MST ")
				  _T("   WHERE USER_ID = '%s' AND USER_PW = '%s' "), strId, strPw);

	int nSuccess = m_pDoc->GetSelectQryCnt_DLG(strSql);
	if(nSuccess > 0)
	{
		int nSuccess = -1;
		m_pDoc->InitilizeUserInfo(strId, nSuccess);
		if(nSuccess < 1){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용자 권한이 정의되지 않은 아이디 입니다"))); CSkinDialog::OnCancel(); return; }
		m_pDoc->m_strId = strId;
		m_pDoc->UpdateRibbonLang();

		CString strLOG_LUGG_NO = _T("");
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("LOGIN");

		//m_pDoc->GetQueryInsertClientLog(_T("CSystemLoginDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG);

		CSkinDialog::OnOK();
		if (strId != "LFC")//ONLY_VIEW
		{
			m_blLogYn = TRUE;
		}
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("로그인에 실패했습니다")));
	CSkinDialog::OnCancel();
}


void CSystemLoginDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSkinDialog::OnCancel();
}


BOOL CSystemLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)  
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			{
				OnBnClickedOk();
				break;
			}
		case VK_ESCAPE:
			{
				OnBnClickedCancel();
				break;
			}
		}
	}
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CSystemLoginDlg::RenameResource( EN_LANG m_enLang)
{
 	TCHAR chrFileName[500];
 	GetModuleFileName(NULL, chrFileName, MAX_PATH);
 	CString strAppPath = _T("");
 	strAppPath.Format(_T("%s"),chrFileName);
 	CString strExtension = _T(".ini");
 
 	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_login\\"), _T("dlg_login"), strExtension);
 	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sign"), (int)m_enLang);
	SetDlgItemText(IDOK, strValue);
}

void CSystemLoginDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_login\\20x20\\");
	CString strExtension = _T(".png");

	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnOk.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("sign"), strExtension)), NULL, 5, 5);


	m_btnLock.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
	m_btnLock.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("lock"), strExtension)), NULL, 5, 5);
}


void CSystemLoginDlg::OnBnClickedLock()
{
	CString strId, strPw, strSql;

	if(m_pDoc == NULL)
	{
		return;
	}

	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("로그아웃 하시겠습니까?")), MB_YESNO) == IDYES)
	{

		strId = _T("LFC");//ONLY_VIEW
		strPw = _T("LFC");//ONLY_VIEW

		strSql.Format(_T(" SELECT USER_ID ")
			_T("	 FROM USER_MST ")
			_T("   WHERE USER_ID = '%s' AND USER_PW = '%s' "), strId, strPw);

		int nSuccess = m_pDoc->GetSelectQryCnt(strSql);
		if(nSuccess > 0)
		{
			int nSuccess = -1;
			m_pDoc->InitilizeUserInfo(strId, nSuccess);
			if(nSuccess < 1){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("로그아웃을 실패했습니다"))); CSkinDialog::OnCancel(); return; }
			m_pDoc->m_strId = strId;
			m_pDoc->UpdateRibbonLang();
			CSkinDialog::OnOK();
			m_blLogYn = FALSE;
			return;
		}
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("로그아웃을 실패했습니다")));
	}

	m_pDoc->m_strId = _T("");
	CSkinDialog::OnOK();
	return;


}

bool CSystemLoginDlg::InitializeUser()
{
	while(TRUE)
	{
		
		if(DoModal() == IDOK)
		{
			break;
		}
		else
			return false;
	}

	OnClose();
	return true;
}

// void CSystemLoginDlg::InitilizeNationFont( int pnLang )
// {
// 	CFont* pFont = new CFont();
// 	pFont->CreateFont( // m_font는 이 클래스의 멤버로 선언돼 있다고 가정
// 		15, // nHeight 
// 		8, // nWidth 
// 		0, // nEscapement 
// 		0, // nOrientation 
// 		0, // nWeight 
// 		0, // bItalic 
// 		0, // bUnderline 
// 		0, // cStrikeOut 
// 		0, // nCharSet 
// 		OUT_DEFAULT_PRECIS, // nOutPrecision 
// 		0, // nClipPrecision 
// 		DEFAULT_QUALITY, // nQuality 
// 		DEFAULT_PITCH | FF_DONTCARE, // nPitchAndFamily 
// 		Global.GetFontName(pnLang)
// 		); 
// 
// 	SetFont(pFont, TRUE);
// 	SendMessageToDescendants(WM_SETFONT, (WPARAM)pFont->GetSafeHandle(), 1, TRUE, FALSE);
// }
