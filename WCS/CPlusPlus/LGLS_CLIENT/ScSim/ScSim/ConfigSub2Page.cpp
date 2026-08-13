// ConfigStdPage.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "ConfigSub2Page.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CConfigSub2Page, CPropertyPage)

///////////////////////////////////////////
// CConfigSub2Page property page

CConfigSub2Page::CConfigSub2Page() : CPropertyPage(CConfigSub2Page::IDD)
{
	//{{AFX_DATA_INIT(CConfigSub2Page)
	m_nMonPort = 0;
	//}}AFX_DATA_INIT
}

CConfigSub2Page::CConfigSub2Page(CEcsDoc* pDoc, CConfig* pConfig) : CPropertyPage(CConfigSub2Page::IDD)
{
	m_pDoc = pDoc;
	m_pConfig = pConfig;
}

CConfigSub2Page::~CConfigSub2Page()
{
}

void CConfigSub2Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigSub2Page)
	DDX_Control(pDX, ID_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_LIST_CLIENT, m_ctlReport);
	DDX_Text(pDX, IDC_EDIT_PORT_MON, m_nMonPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfigSub2Page, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigSub2Page)
	ON_BN_CLICKED(ID_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CConfigSub2Page::OnInitDialog() 
{
	m_nMonPort = m_pConfig->m_nMonPort;

	CPropertyPage::OnInitDialog();

	m_ctlReport.LoadImage(IDI_ICON_MODIFY, -1);
	m_ctlReport.InsertColumn(_T("컴퓨터명"), _T("접속시간"), _T("접속 IP주소"), _T("접속포트"), _T("인증성공"), _T("ETX"));

	INIT_BUTTON(m_btnSave, IDI_SAVE, 18, 18, _T("현재 설정된 값으로 저장"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigSub2Page::OnButtonSave() 
{
	if (AfxMessageBox(_T("변경된 내용을 저장 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		return;
	}

	m_pConfig->m_nMonPort = m_nMonPort;
 	m_pConfig->SaveConfig3();
}

void CConfigSub2Page::SetChildWndFont()
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
