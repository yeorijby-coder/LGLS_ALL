// DataSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "DataSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSettingDlg dialog


CDataSettingDlg::CDataSettingDlg(CString strData, CString strCaption, CWnd* pParent /*=NULL*/)
	: CDialog(CDataSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strData = strData;
	m_strCaption = strCaption;
}


void CDataSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDataSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CDataSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSettingDlg message handlers

BOOL CDataSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int w = 24, h = 24;
	INIT_BUTTON(m_btnOK, IDI_OK, w, h, _T("저장후 닫음"));
	INIT_BUTTON(m_btnCancel, IDI_CANCEL, w, h, _T("저장하지 않고 닫음"));
	SetWindowText(m_strCaption);
	SET(IDC_EDIT_DATA, m_strData);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataSettingDlg::OnOK() 
{
	GET(IDC_EDIT_DATA, m_strData);
	
	CDialog::OnOK();
}
