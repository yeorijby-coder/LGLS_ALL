// ChangePasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"
#include "ChangePasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangePasswordDlg dialog


CChangePasswordDlg::CChangePasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangePasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangePasswordDlg)
	m_strPasswordChanged = _T("");
	m_strPasswordConfirm = _T("");
	m_strPasswordOrigin = _T("");
	//}}AFX_DATA_INIT
}


void CChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangePasswordDlg)
	DDX_Text(pDX, IDC_EDIT_PASSWORD_CHANGED, m_strPasswordChanged);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_CONFIRM, m_strPasswordConfirm);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_ORGIN, m_strPasswordOrigin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangePasswordDlg, CDialog)
	//{{AFX_MSG_MAP(CChangePasswordDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangePasswordDlg message handlers

void CChangePasswordDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
