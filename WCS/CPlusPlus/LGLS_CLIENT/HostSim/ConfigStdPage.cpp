// ConfigStdPage.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "ConfigStdPage.h"
#include "PortEthernetDlg.h"
#include "PortSerialDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CConfigStdPage, CPropertyPage)


////////////////////////////////
// CConfigStdPage property page

CConfigStdPage::CConfigStdPage() : CPropertyPage(CConfigStdPage::IDD)
{
	//{{AFX_DATA_INIT(CConfigStdPage)
	m_bBeep = FALSE;
	m_bDebug = FALSE;
	m_bError = FALSE;
	m_bEvent = FALSE;
	m_bJob = FALSE;
	m_bConnect = FALSE;
	m_bRetry = FALSE;
	m_nHostPort = 0;
	m_nEcsPort = 0;
	m_bAlarm = FALSE;
	m_bViewLampControler = FALSE;
	m_bViewProductInfoInputer = FALSE;
	m_nWeightValue = 0;
	m_bLogTest = FALSE;
	m_nFontSize = 0;
	m_nBufferLimit = 2;
	m_bNotCheckRetHS = FALSE;
	m_bNotCheckRetHsSc1 = FALSE;
	m_bNotCheckRetHsSc2 = FALSE;
	m_bNotCheckRetHsSc3 = FALSE;
	m_bNotCheckRetHsSc4 = FALSE;
	m_bNotCheckRetHsSc5 = FALSE;
	m_bNotCheckRetHsSc6 = FALSE;
	m_bNotBackupNetwork = FALSE;
	m_nCompleteCnt = 0;
	//}}AFX_DATA_INIT
}

CConfigStdPage::CConfigStdPage(CEcsDoc* pDoc, CConfig* pConfig) : CPropertyPage(CConfigStdPage::IDD)
{
	m_pDoc = pDoc;
	m_pConfig = pConfig;
	//m_bLogTest = FALSE;
}

CConfigStdPage::~CConfigStdPage()
{
}

void CConfigStdPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigStdPage)
	DDX_Check(pDX, IDC_CHECK_BEEP, m_bBeep);
	DDX_Check(pDX, IDC_CHECK_DEBUG, m_bDebug);
	DDX_Check(pDX, IDC_CHECK_ERROR, m_bError);
	DDX_Check(pDX, IDC_CHECK_EVENT, m_bEvent);
	DDX_Check(pDX, IDC_CHECK_JOB, m_bJob);
	DDX_Check(pDX, IDC_CHK_CONNECT, m_bConnect);
	DDX_Check(pDX, IDC_CHK_RETRY, m_bRetry);
	DDX_Text(pDX, IDC_EDT_PORT, m_nHostPort);
	DDX_Text(pDX, IDC_EDT_PORT_ECS, m_nEcsPort);
	DDX_Check(pDX, IDC_CHECK_ALARM, m_bAlarm);
	DDX_Check(pDX, IDC_VIEW_LAMP, m_bViewLampControler);
	DDX_Check(pDX, IDC_VIEW_PRODUCT_INFO, m_bViewProductInfoInputer);
	DDX_Text(pDX, IDC_EDT_WEIGHT_VALUE, m_nWeightValue);
	DDV_MinMaxInt(pDX, m_nWeightValue, 1, 1000);
	DDX_Check(pDX, IDC_LOG, m_bLogTest);
	DDX_Text(pDX, IDC_EDT_FONT_SIZE, m_nFontSize);
	DDX_Text(pDX, IDC_EDT_BUFFER_CNT, m_nBufferLimit);
	DDV_MinMaxInt(pDX, m_nBufferLimit, 2, 3);
	DDX_Check(pDX, IDC_NOT_CHECK_RET_HS, m_bNotCheckRetHS);
	DDX_Check(pDX, IDC_NOT_CHECK_RET_HS_SC1, m_bNotCheckRetHsSc1);
	DDX_Check(pDX, IDC_NOT_CHECK_RET_HS_SC2, m_bNotCheckRetHsSc2);
	DDX_Check(pDX, IDC_NOT_CHECK_RET_HS_SC3, m_bNotCheckRetHsSc3);
	DDX_Check(pDX, IDC_NOT_CHECK_RET_HS_SC4, m_bNotCheckRetHsSc4);
	DDX_Check(pDX, IDC_NOT_CHECK_RET_HS_SC5, m_bNotCheckRetHsSc5);
	DDX_Check(pDX, IDC_NOT_CHECK_RET_HS_SC6, m_bNotCheckRetHsSc6);
	DDX_Check(pDX, IDC_CHECK_NO_BACKUP_NETWORK, m_bNotBackupNetwork);
	DDX_Text(pDX, IDC_EDT_COMPLETE_CNT, m_nCompleteCnt);
	DDV_MinMaxInt(pDX, m_nCompleteCnt, 0, 10);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CConfigStdPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigStdPage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COMM1, OnDblclkListCommPortEthernet)
	ON_BN_CLICKED(IDC_LOG, OnLog)
	ON_BN_CLICKED(IDC_NOT_CHECK_RET_HS, OnCheckNotCheckHS)
	ON_BN_CLICKED(IDC_NOT_CHECK_RET_HS_SC1, OnNotCheckRetHsSc1)
	ON_BN_CLICKED(IDC_NOT_CHECK_RET_HS_SC2, OnNotCheckRetHsSc2)
	ON_BN_CLICKED(IDC_NOT_CHECK_RET_HS_SC3, OnNotCheckRetHsSc3)
	ON_BN_CLICKED(IDC_NOT_CHECK_RET_HS_SC4, OnNotCheckRetHsSc4)
	ON_BN_CLICKED(IDC_NOT_CHECK_RET_HS_SC5, OnNotCheckRetHsSc5)
	ON_BN_CLICKED(IDC_NOT_CHECK_RET_HS_SC6, OnNotCheckRetHsSc6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CConfigStdPage::OnInitDialog() 
{
	m_nEcsPort			= m_pConfig->m_nEcsPort;
	m_nHostPort			= m_pConfig->m_portHost.m_nPort;
	m_bConnect			= m_pConfig->m_portHost.m_bConnect;
	m_bRetry			= m_pConfig->m_portHost.m_bRetry;

	m_bJob				= m_pConfig->m_bJob;
	m_bEvent			= m_pConfig->m_bEvent;
	m_bError			= m_pConfig->m_bError;
	m_bAlarm			= m_pConfig->m_bAlarm;
	m_bDebug			= m_pConfig->m_bDebug;
	m_bBeep				= m_pConfig->m_bBeep;

	m_nWeightValue		= m_pConfig->m_nWeightValue;
	m_nFontSize			= m_pConfig->m_nFontSize;

	m_nBufferLimit		= m_pConfig->m_nRetBufferCount;
	m_bNotCheckRetHS	= m_pConfig->m_bNoCheckScRetHs;

	m_bNotBackupNetwork	= m_pConfig->m_bNotBackupNetwork;

	m_nCompleteCnt		= m_pConfig->m_nCompleteCnt;

	m_bNotCheckRetHsSc1 = m_pConfig->m_bNotCheckRetHsSc1;
	m_bNotCheckRetHsSc2 = m_pConfig->m_bNotCheckRetHsSc2;
	m_bNotCheckRetHsSc3 = m_pConfig->m_bNotCheckRetHsSc3;
	m_bNotCheckRetHsSc4 = m_pConfig->m_bNotCheckRetHsSc4;
	m_bNotCheckRetHsSc5 = m_pConfig->m_bNotCheckRetHsSc5;
	m_bNotCheckRetHsSc6 = m_pConfig->m_bNotCheckRetHsSc6;


	CPropertyPage::OnInitDialog();

	InitButtons();
//	SetChildWndFont();

	SetDlgItemText(IDC_IPADDRESS, m_pConfig->m_portHost.m_strIP);

	CImageList imageList;
	CListCtrl* pListCtrl1 = (CListCtrl*)GetDlgItem(IDC_LIST_COMM1);

	// Ethernet Port List Column
	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/,	1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BOARD));
	pListCtrl1->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl1->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask			= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt			= LVCFMT_LEFT;
		int cx[]			= {90, 60, 100, 42, 60, 45, 40, 60};
		LPCTSTR lpszCol[]	= { _T("Device"), _T("Type"), _T("Address"), _T("Port"), _T("Connect"), _T("Retry"), _T("Log"), _T("Timeout")};
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i=0; i<nCol; ++i)
			pListCtrl1->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	BOOL bCheck = IsDlgButtonChecked(IDC_NOT_CHECK_RET_HS);
	
	if (bCheck== TRUE)
	{
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC1)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC2)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC3)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC4)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC5)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC6)->EnableWindow(TRUE);
//		GetDlgItem(IDC_EDT_BUFFER_CNT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC2)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC3)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC4)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC5)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC6)->EnableWindow(FALSE);
//		GetDlgItem(IDC_EDT_BUFFER_CNT)->EnableWindow(FALSE);
	}


	UpdateListEhternet();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigStdPage::InitButtons()
{
	INIT_BUTTON(m_btnSave, IDI_SAVE, 18, 18, _T("현재 설정된 값으로 저장"));
}

void CConfigStdPage::UpdateListEhternet()
{
	CString str;
	int i, j, nIndex = 0;
	UINT nMask = LVIF_TEXT | LVIF_IMAGE;
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_COMM1);
	CEquipment* pEquipment = NULL;
	CPortEthernet* pPort = NULL;

	pList->DeleteAllItems();

	for (i=0, j=0; i<m_pDoc->m_pEquipments.GetSize(); ++i, j=0)
	{
		pEquipment = m_pDoc->m_pEquipments[i];
		pPort = &(pEquipment->m_port);
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		nIndex = pList->GetItemCount();
		pList->InsertItem(nMask, nIndex, pPort->m_strDevice, 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, _T("Ethernet"), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, pPort->m_strIP, 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, pPort->m_bConnect ? _T("Yes") : _T("No"), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, pPort->m_bRetry ? _T("Yes") : _T("No"), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, pPort->m_bWriteLog ? _T("Yes") : _T("No"), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString((int)pPort->m_dwRecvTimeout), 0, 0, 0, 0);
		pList->SetItemData(nIndex, (LPARAM)pEquipment);
	}
}

void CConfigStdPage::OnCheckNotCheckHS() 
{
	BOOL bCheck = IsDlgButtonChecked(IDC_NOT_CHECK_RET_HS);

	if (bCheck== TRUE)
	{
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC1)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC2)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC3)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC4)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC5)->EnableWindow(TRUE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC6)->EnableWindow(TRUE);
//		GetDlgItem(IDC_EDT_BUFFER_CNT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC2)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC3)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC4)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC5)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOT_CHECK_RET_HS_SC6)->EnableWindow(FALSE);
//		GetDlgItem(IDC_EDT_BUFFER_CNT)->EnableWindow(FALSE);

	}

}

void CConfigStdPage::OnButtonSave() 
{
	if (AfxMessageBox(_T("변경된 내용을 저장 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return ;

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	
	GetDlgItemText(IDC_IPADDRESS, m_pConfig->m_portHost.m_strIP);
	m_pConfig->m_portHost.m_nPort			= m_nHostPort;
	m_pConfig->m_portHost.m_bConnect		= m_bConnect;
	m_pConfig->m_portHost.m_bRetry			= m_bRetry;
	
	m_pConfig->m_nEcsPort					= m_nEcsPort;
	m_pConfig->m_nWeightValue				= m_nWeightValue;
	m_pConfig->m_nFontSize					= m_nFontSize;

	m_pConfig->m_bViewLampControler			= m_bViewLampControler;
	m_pConfig->m_bViewProductInfoInputer	= m_bViewProductInfoInputer;
	

	m_pConfig->m_bJob						= m_bJob;
	m_pConfig->m_bEvent						= m_bEvent;
	m_pConfig->m_bError						= m_bError;
	m_pConfig->m_bAlarm						= m_bAlarm;
	m_pConfig->m_bDebug						= m_bDebug;
	m_pConfig->m_bBeep						= m_bBeep;

	m_pConfig->m_nRetBufferCount			= m_nBufferLimit;
	m_pConfig->m_bNoCheckScRetHs			= m_bNotCheckRetHS;

	m_pConfig->m_bNotBackupNetwork			= m_bNotBackupNetwork;

	m_pConfig->m_nCompleteCnt				= m_nCompleteCnt;
	
	m_pConfig->m_bNotCheckRetHsSc1 = m_bNotCheckRetHsSc1;
	m_pConfig->m_bNotCheckRetHsSc2 = m_bNotCheckRetHsSc2;
	m_pConfig->m_bNotCheckRetHsSc3 = m_bNotCheckRetHsSc3;
	m_pConfig->m_bNotCheckRetHsSc4 = m_bNotCheckRetHsSc4;
	m_pConfig->m_bNotCheckRetHsSc5 = m_bNotCheckRetHsSc5;
	m_pConfig->m_bNotCheckRetHsSc6 = m_bNotCheckRetHsSc6;


//	m_pConfig->SaveXML();
	m_pConfig->SaveConfig1();
}


void CConfigStdPage::OnDblclkListCommPortEthernet(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_COMM1);
	POSITION pos = pList->GetFirstSelectedItemPosition();
	int nSel = pList->GetNextSelectedItem(pos);
	if (nSel == CB_ERR) return;

	CEquipment* pEquipment = (CEquipment*)pList->GetItemData(nSel);
	DEBUGER_ASSERT_VALID(pEquipment != NULL);

	CPortEthernetDlg dlg(&(pEquipment->m_port));
	if (dlg.DoModal() == IDOK)
		UpdateListEhternet();
	
	*pResult = 0;
}

void CConfigStdPage::SetChildWndFont()
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

void CConfigStdPage::OnLog() 
{
	m_bLogTest = IsDlgButtonChecked(IDC_LOG);

	if(m_bLogTest== TRUE)
		m_pDoc->m_bLog = TRUE;
	else
		m_pDoc->m_bLog = FALSE;

	((CButton*)GetDlgItem(IDC_LOG))->SetCheck(m_pDoc->m_bLog);  //체크 변경
}

void CConfigStdPage::OnNotCheckRetHsSc1() 
{
	UpdateData(TRUE);	
}

void CConfigStdPage::OnNotCheckRetHsSc2() 
{
	UpdateData(TRUE);	
}

void CConfigStdPage::OnNotCheckRetHsSc3() 
{
	UpdateData(TRUE);
}

void CConfigStdPage::OnNotCheckRetHsSc4() 
{
	UpdateData(TRUE);	
}

void CConfigStdPage::OnNotCheckRetHsSc5() 
{
	UpdateData(TRUE);	
}

void CConfigStdPage::OnNotCheckRetHsSc6() 
{
	UpdateData(TRUE);	
}
