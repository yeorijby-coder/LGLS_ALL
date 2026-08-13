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


CDataSettingDlg::CDataSettingDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialogResize(CDataSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = pDoc;
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
	ON_BN_CLICKED(IDOK, &CDataSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDataSettingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSettingDlg message handlers

BOOL CDataSettingDlg::OnInitDialog() 
{
	CDialogResize::OnInitDialog();
	
	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(&wndTopMost, 300, 300, rectWnd.Width(), rectWnd.Height(), SWP_NOMOVE | SWP_NOACTIVATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataSettingDlg::ValueChange(CString Value)
{
	SET(IDC_EDIT_DATA, Value);
}

void CDataSettingDlg::OnOK() 
{
	CString Value;

	GET(IDC_EDIT_DATA, Value);



	CDialog::OnOK();
}


void CDataSettingDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}

void CDataSettingDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}
