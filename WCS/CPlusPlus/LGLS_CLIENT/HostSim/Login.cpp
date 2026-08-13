// Login.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog


CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogin)
	m_strUserID = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogin)
	DDX_Text(pDX, IDC_LOGIN_USER_ID, m_strUserID);
	DDX_Text(pDX, IDC_LOGIN_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_CHECK_CHANGE_PASS, m_bCheckPass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	//{{AFX_MSG_MAP(CLogin)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_CHANGE_PASS, OnCheckChangePass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlers

//==============================================================================
//	Desc	: 
//	Date	: 2013.09
//	Update	:
//==============================================================================
BOOL CLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Title
	SetWindowText(AfxGetAppName());

	SET(IDC_LOGIN_USER_ID, _T("ADMIN"));
	SET(IDC_LOGIN_PASSWORD, _T("SLI35"));

	// Background Bitmap
//	m_bitmap.LoadBitmap(IDB_LOGIN);
//	m_hBitmap = (HBITMAP)::LoadImage(NULL, "Res\\OciLogo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//==============================================================================
//	Desc	: 
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CLogin::OnDestroy() 
{
	CDialog::OnDestroy();
}

//==============================================================================
//	Desc	: 
//	Date	: 2014.10
//	Update	:
//==============================================================================
void CLogin::OnCheckChangePass() 
{
	if (!UpdateData())
		return;

	if (m_bCheckPass == TRUE)
	{
		GetDlgItem(IDC_EDT_NEW_PASSWORD)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_NEW_PASSWORD)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDT_PASSWORD_CONFIRM)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_PASSWORD_CONFIRM)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_EDGE)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDT_NEW_PASSWORD)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_NEW_PASSWORD)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDT_PASSWORD_CONFIRM)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_PASSWORD_CONFIRM)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_EDGE)->ShowWindow(FALSE);
	
	}
}

//==============================================================================
//	Desc	: 
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CLogin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC  dcMem;
	CRect rc;

	GetClientRect(&rc);

	dcMem.CreateCompatibleDC(&dc);
	CBitmap * pOld = dcMem.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOld);
}

//==============================================================================
//	Desc	: 
//	Date	: 2013.09
//	Update	:
//==============================================================================
HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush brush;

	// Control을 투명하게
    if (nCtlColor == CTLCOLOR_STATIC)
	{
		brush.CreateStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);

		return (HBRUSH) brush;
    }

	return hbr;
}
/*
//==============================================================================
//	Desc	: 
//	Date	: 2013.09
//	Update	:
//==============================================================================
BOOL CLogin::OnEraseBkgnd(CDC* pDC) 
{

//	CDC dc;
//	CRect rc;
//	GetClientRect(&rc);
//
//	dc.CreateCompatibleDC(pDC);
//	CBitmap * pOld = dc.SelectObject(&m_bitmap);
//	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);
//	dc.SelectObject(pOld);

	CDialog::OnEraseBkgnd(pDC);

	CRect  rect;
	CWnd::GetClientRect(rect);
	CMemDC pMemDC(pDC, rect);

	if (m_hBitmap)
	{
		pMemDC->SelectObject(m_hBitmap);
	}

	return TRUE;
}
*/
//==============================================================================
//	Desc	: [확인] 버튼
//	Date	: 2013.09
//	Update	: 
//==============================================================================
void CLogin::OnOK() 
{
	CString strPassword;
	TCHAR szTemp[_MAX_PATH] = {0};

	CWaitCursor wait;

	CString str, strCap, strNewPassword, strConfirmPassWord, strTemp;
	BOOL bOK = FALSE;

	CWnd::GetWindowText(strCap);

	::GetPrivateProfileString(_T("LOGIN"), _T("LoginPW"), _T("SLI35"), szTemp, _MAX_PATH, ECS_INI_FILE);
	strPassword.Format(_T("%s"), szTemp);

	if (m_bCheckPass == TRUE)
	{
		if (m_strPassword != strPassword)
		{
			CWnd::MessageBox(_T("기존 Password가 틀렸습니다. 올바르게 입력하세요"), strCap, MB_OK | MB_ICONWARNING);
			return;
		}

		GET(IDC_EDT_NEW_PASSWORD, strNewPassword);
		GET(IDC_EDT_PASSWORD_CONFIRM, strConfirmPassWord);

		if (strNewPassword != strConfirmPassWord)
		{
			CWnd::MessageBox(_T("새 암호와 암호확인의 내용이 다릅니다. 올바르게 입력해주세요!"), strCap, MB_OK | MB_ICONWARNING);
			return;		
		}
		else if(strNewPassword == "")
		{
			CWnd::MessageBox(_T("새 암호를 올바르게 입력해주세요 "), strCap, MB_OK | MB_ICONWARNING);
			return;			
		}
		else if(strConfirmPassWord == "")
		{
			CWnd::MessageBox(_T("암호확인을 올바르게 입력해주세요 "), strCap, MB_OK | MB_ICONWARNING);
			return;			
		}
		else
		{
			CWnd::MessageBox(_T("암호를 변경합니다! 확인을 누르시면 로그인 됩니다."), strCap, MB_OK | MB_ICONWARNING);
		}

	}

	UpdateData(TRUE);

	if (m_strUserID == _T("ADMIN") && m_strPassword == strPassword)
	{
		CDialog::OnOK();
		return;
	}


	TRY 
	{
		CStringArray strArray;
		CString str;
		char szPath[_MAX_PATH] = {0};

		::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);
		str.Format(_T("%s\\Data\\EcsUser.dat"), szPath);

		CFile f(str, CFile::modeRead);
		CArchive ar(&f, CArchive::load);

		while (ar.ReadString(str))
		{
			CLib::Split(str, strArray, _T(","));
			if (m_strUserID == strArray.GetAt(0) && m_strPassword == strArray.GetAt(2))
			{
				if (strArray.GetAt(3) != _T("0"))
				{
					CWnd::MessageBox(_T("일반사용자 권한으로는 서버를 실행할 수 없습니다."), strCap, MB_OK | MB_ICONWARNING);
					ar.Close();
					f.Close();
					wait.Restore();
					return;
				}
				bOK = TRUE;
				break;
			}
		}

		ar.Close();
		f.Close();

		if (!bOK)
		{
			CWnd::MessageBox(_T("미등록 사용자입니다. 재입력해 주십시오."), strCap, MB_OK | MB_ICONWARNING);
			(GetDlgItem(IDC_LOGIN_USER_ID))->SetFocus();
			wait.Restore();
			return;
		}
	}
	CATCH (CException, e)
	{
		CWnd::MessageBox(CLib::GetExceptionString(e), strCap, MB_OK | MB_ICONERROR);
		wait.Restore();
		return;
	}
	END_CATCH

	wait.Restore();

	CDialog::OnOK();
}
