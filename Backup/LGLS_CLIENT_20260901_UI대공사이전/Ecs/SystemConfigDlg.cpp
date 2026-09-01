// SystemConfig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "SystemConfigDlg.h"
#include "afxdialogex.h"


// CSystemConfigDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSystemConfigDlg, CSkinDialog)

CSystemConfigDlg::CSystemConfigDlg(CWnd* pParent /*=NULL*/)
: CSkinDialog(CSystemConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}


CSystemConfigDlg::CSystemConfigDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CSystemConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;	
}

CSystemConfigDlg::~CSystemConfigDlg()
{
	this->DestroyWindow();
}

void CSystemConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_SYSTEM_CONFIG_OK, m_btnOk);
	DDX_Control(pDX, ID_SYSTEM_CONFIG_CANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CSystemConfigDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_SYSTEM_CONFIG_OK, &CSystemConfigDlg::OnClickedSystemConfigOk)
	ON_BN_CLICKED(ID_SYSTEM_CONFIG_CANCEL, &CSystemConfigDlg::OnBnClickedSystemConfigCancel)
END_MESSAGE_MAP()

BOOL CSystemConfigDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized ){		m_bInitialized = TRUE;		}

	RedrawImage();
	RenameResource(pEn);

	Invalidate(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSystemConfigDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_config\\"), _T("dlg_config"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("sign"), (int)m_enLang);
	//SetDlgItemText(IDOK, strValue);
}

void CSystemConfigDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_config\\20x20\\");
	CString strExtension = _T(".png");

	//m_btnLock.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
//	m_btnLock.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T//("lock"), strExtension)), NULL, 5, 5);
}




void CSystemConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}


HCURSOR CSystemConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSystemConfigDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CSystemConfigDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);

	x = rc.Width() - 20 - size.cx;
	y = rc.Height() - 10 - size.cy;
	m_btnCancel.MoveWindow(x, y, size.cx, size.cy, TRUE);

	x -= size.cx + 10;
	m_btnOk.MoveWindow(x, y, size.cx, size.cy, TRUE);
	m_btnOk.Invalidate();
}


BOOL CSystemConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)  
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			{
				OnClickedSystemConfigOk();
				break;
			}
		case VK_ESCAPE:
			{
				OnBnClickedSystemConfigCancel();
				break;
			}
		}
	}
	return CSkinDialog::PreTranslateMessage(pMsg);
}


void CSystemConfigDlg::OnClickedSystemConfigOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSkinDialog::OnOK();
}


void CSystemConfigDlg::OnBnClickedSystemConfigCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
