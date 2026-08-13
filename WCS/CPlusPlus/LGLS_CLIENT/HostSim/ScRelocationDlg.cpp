// ScRelocationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "ScRelocationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScRelocationDlg dialog


CScRelocationDlg::CScRelocationDlg(CWnd* pParent /* = NULL */)
	: CDialog(CScRelocationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScRelocationDlg)
	m_strLocation = _T("");
	//}}AFX_DATA_INIT
}


void CScRelocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScRelocationDlg)
	DDX_Text(pDX, IDC_EDIT_LOCATION, m_strLocation);
//	DDX_Text(pDX, IDC_EDIT_LOCATION2, m_strLocation2);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScRelocationDlg, CDialog)
	//{{AFX_MSG_MAP(CScRelocationDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScRelocationDlg message handlers

BOOL CScRelocationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INIT_BUTTON(m_btnOK, IDI_OK, 16, 16, _T("이중입고재지시"));
	INIT_BUTTON(m_btnCancel, IDI_CANCEL, 16, 16, _T("취소"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScRelocationDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

