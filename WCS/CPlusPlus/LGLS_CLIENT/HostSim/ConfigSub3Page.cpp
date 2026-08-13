// ConfigStdPage.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "ConfigSub3Page.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CConfigSub3Page, CPropertyPage)

///////////////////////////////////////////
// CConfigSub3Page property page

CConfigSub3Page::CConfigSub3Page() : CPropertyPage(CConfigSub3Page::IDD)
{
	//{{AFX_DATA_INIT(CConfigSub3Page)
//	m_nMonPort = 0;

//	m_nBank = 0;
//	m_nBay = 0;
//	m_nLevel = 0;
	//}}AFX_DATA_INIT
}

CConfigSub3Page::CConfigSub3Page(CEcsDoc* pDoc, CConfig* pConfig) : CPropertyPage(CConfigSub3Page::IDD)
{
	m_pDoc = pDoc;
	m_pConfig = pConfig;
}

CConfigSub3Page::~CConfigSub3Page()
{
}

void CConfigSub3Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigSub3Page)
	DDX_Control(pDX, IDC_COMBO6, m_cbxLevel);
	DDX_Control(pDX, IDC_COMBO5, m_cbxBay);
	DDX_Control(pDX, IDC_COMBO1, m_cbxBank);
	DDX_Text(pDX, IDC_EDIT_BANK, m_nBank);
	DDX_Text(pDX, IDC_EDIT_BAY, m_nBay);
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_nLevel);
	DDX_Control(pDX, IDC_BTN_RACK_WRITE, m_btnRackWrite);
	DDX_Control(pDX, IDC_BTN_DISABLE_RACK, m_btnDisableRack);
	DDX_Control(pDX, IDC_LIST_RACK, m_ctlReport);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfigSub3Page, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigSub3Page)
	ON_BN_CLICKED(IDC_BTN_RACK_WRITE, OnBtnRackWrite)
	ON_BN_CLICKED(IDC_BTN_DISABLE_RACK, OnBtnDisableRack)
//	ON_BN_CLICKED(ID_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CConfigSub3Page::OnInitDialog() 
{
//	m_nMonPort = m_pConfig->m_nMonPort;

	CPropertyPage::OnInitDialog();

	m_ctlReport.LoadImage(IDI_ICON_LIGHTBULB, -1);
	m_ctlReport.InsertColumn(_T("Location"), _T("Disable"));

	INIT_BUTTON(m_btnRackWrite, IDI_SAVE, 18, 18, _T("현재 설정된 값으로 저장"));
	INIT_BUTTON(m_btnDisableRack, IDI_SAVE, 18, 18, _T("현재 설정된 값으로 저장"));

	Refrash();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigSub3Page::Refrash() 
{
	CString strTemp;
	strTemp.Format(_T("RACK 현황\t\t\tBANK : %d개\tBAY : %d개\tLEVEL : %d개\t"), m_pConfig->m_nBank, m_pConfig->m_nBay, m_pConfig->m_nLevel);
	SET(IDC_STATIC_RACK_CNT, strTemp);

	(m_pConfig->m_nBank <= 0)	?	m_nBank = 1		:	m_nBank = m_pConfig->m_nBank	;	
	(m_pConfig->m_nBay <= 0)	?	m_nBay = 1		:	m_nBay = m_pConfig->m_nBay		;	
	(m_pConfig->m_nLevel <= 0)	?	m_nLevel = 1	:	m_nLevel = m_pConfig->m_nLevel	;	

	UpdateData(FALSE);

	int i = 0;
	m_cbxBank.Clear();
	for(i = 0 ; i < m_nBank ; i++)
	{
		strTemp.Format(_T("%d"), i + 1);
		m_cbxBank.AddString(strTemp);
	}

	m_cbxBay.Clear();
	for(i = 0 ; i < m_nBay ; i++)
	{
		strTemp.Format(_T("%d"), i + 1);
		m_cbxBay.AddString(strTemp);
	}

	m_cbxLevel.Clear();
	for(i = 0 ; i < m_nLevel ; i++)
	{
		strTemp.Format(_T("%d"), i + 1);
		m_cbxLevel.AddString(strTemp);
	}

	m_pDoc->m_pConfig->Load(&m_ctlReport);			// 봐야함

}
/*
void CConfigSub3Page::OnButtonSave() 
{
	if (AfxMessageBox("변경된 내용을 저장 하시겠습니까?", MB_OKCANCEL) == IDCANCEL)
		return ;

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		return;
	}

	m_pConfig->m_nMonPort = m_nMonPort;
 	m_pConfig->SaveConfig3();
}
*/
void CConfigSub3Page::SetChildWndFont()
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

void CConfigSub3Page::OnBtnRackWrite() 
{
	// TODO: Add your control notification handler code here
	// Rack 정보를 Write 하는 부분 
	CString strTemp;

	GET(IDC_EDIT_BANK, strTemp);		int nBank = _ttoi(strTemp);
	GET(IDC_EDIT_BAY, strTemp);			int nBay = _ttoi(strTemp);
	GET(IDC_EDIT_LEVEL, strTemp);		int nLevel = _ttoi(strTemp);

	if (nBank <= 0 || nBay <= 0 || nLevel <= 0 )
	{
		AfxMessageBox(_T("RACK 정보를 확인하시고 다시 입력해주세요"));
		return;
	}

	if (AfxMessageBox(_T("변경된 내용을 저장 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return ;

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	
	m_pConfig->m_nBank		= 	m_nBank;
	m_pConfig->m_nBay		= 	m_nBay;
	m_pConfig->m_nLevel		= 	m_nLevel;
	
	m_pConfig->SaveConfig4();

	Refrash();

}

void CConfigSub3Page::OnBtnDisableRack() 
{
	// TODO: Add your control notification handler code here
	if (AfxMessageBox(_T("변경된 내용을 저장 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return ;

	if (m_cbxBank.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("금지할 Location의 BANK를 선택해주세요!"));
		return;
	}

	if (m_cbxBay.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("금지할 Location의 BAY를 선택해주세요!"));
		return;
	}

	if (m_cbxLevel.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("금지할 Location의 LEVEL를 선택해주세요!"));
		return;
	}

	if (!UpdateData(TRUE))
		return ;
	
	CString strTemp;
	int nSel = m_cbxBay.GetCurSel();
	m_cbxBank.GetLBText(m_cbxBank.GetCurSel(), strTemp);		int nBank	= _ttoi(strTemp);
	m_cbxBay.GetLBText(m_cbxBay.GetCurSel(), strTemp);			int nBay	= _ttoi(strTemp);
	m_cbxLevel.GetLBText(m_cbxLevel.GetCurSel(), strTemp);		int nLevel	= _ttoi(strTemp);

	if (nBank <= 0 || nBay <= 0 || nLevel <= 0 )
	{
		AfxMessageBox(_T("RACK 정보를 확인하시고 다시 입력해주세요"));
		return;
	}
		
	strTemp.Format(_T("%02d-%03d-%02d"), nBank, nBay, nLevel);
	m_pConfig->m_strDisableRacks.Add(strTemp);
	
	m_pConfig->SaveConfig5();

	Refrash();
}
