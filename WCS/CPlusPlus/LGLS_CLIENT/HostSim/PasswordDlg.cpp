// PasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "PasswordDlg.h"
#include "ChangePasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog


CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordDlg)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordDlg)
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	//{{AFX_MSG_MAP(CPasswordDlg)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, OnButtonChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg message handlers

void CPasswordDlg::OnButtonChange() 
{
	// TODO: Add your control notification handler code here

	CChangePasswordDlg ChangePasswordDlg;

	ChangePasswordDlg.m_pDoc = m_pDoc;
	if (ChangePasswordDlg.DoModal() != IDOK) return;
	if ( ChangePasswordDlg.m_strPasswordOrigin != m_pDoc->m_pConfig->m_strPassword )
	{
		AfxMessageBox(_T("변경전 PASSWORD 확인 바람.."));
		return;
	}
	if ( ChangePasswordDlg.m_strPasswordChanged != ChangePasswordDlg.m_strPasswordConfirm )
	{
		AfxMessageBox(_T("패스워드를 정확히 입력하십시오 .."));
		return;
	}

	char	szBuff[128];
	sprintf(szBuff, "%s", (LPCTSTR) ChangePasswordDlg.m_strPasswordChanged);
	WritePrivateProfileString(_T("LOGIN"), _T("LoginPW"), (LPCWSTR)szBuff, ECS_INI_FILE);
	m_pDoc->m_pConfig->m_strPassword = ChangePasswordDlg.m_strPasswordChanged;
	AfxMessageBox(_T("패스워드가 변경되었습니다.."));

}
