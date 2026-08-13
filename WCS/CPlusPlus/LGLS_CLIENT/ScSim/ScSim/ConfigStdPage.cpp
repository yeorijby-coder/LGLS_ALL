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
	m_bAlarm = FALSE;
	m_bError = FALSE;
	m_bEvent = FALSE;
	m_bJob = FALSE;
	m_bConnect = FALSE;
	m_bRetry = FALSE;
	m_nHostPort = 0;
	m_nEcsPort = 0;
	//}}AFX_DATA_INIT
}

CConfigStdPage::CConfigStdPage(CEcsDoc* pDoc, CConfig* pConfig) : CPropertyPage(CConfigStdPage::IDD)
{
	m_pDoc = pDoc;
	m_pConfig = pConfig;
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
	DDX_Check(pDX, IDC_CHECK_ALARM, m_bAlarm);
	DDX_Check(pDX, IDC_CHECK_ERROR, m_bError);
	DDX_Check(pDX, IDC_CHECK_EVENT, m_bEvent);
	DDX_Check(pDX, IDC_CHECK_JOB, m_bJob);
	DDX_Check(pDX, IDC_CHK_CONNECT, m_bConnect);
	DDX_Check(pDX, IDC_CHK_RETRY, m_bRetry);
	DDX_Text(pDX, IDC_EDT_PORT, m_nHostPort);
	DDX_Text(pDX, IDC_EDT_PORT_ECS, m_nEcsPort);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_LIST_EQUIPMENT, m_ctlEquip);
}


BEGIN_MESSAGE_MAP(CConfigStdPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigStdPage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COMM1, OnDblclkListCommPortEthernet)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EQUIPMENT, &CConfigStdPage::OnNMDblclkListEquipment)
END_MESSAGE_MAP()

BOOL CConfigStdPage::OnInitDialog() 
{
	m_nEcsPort	= m_pConfig->m_nEcsPort;
//	m_nHostPort	= m_pConfig->m_portHost.m_nPort;
	m_bConnect	= m_pConfig->m_portHost.m_bConnect;
	m_bRetry	= m_pConfig->m_portHost.m_bRetry;

	m_bJob		= m_pConfig->m_bJob;
	m_bEvent	= m_pConfig->m_bEvent;
	m_bError	= m_pConfig->m_bError;
	m_bAlarm	= m_pConfig->m_bAlarm;
	m_bDebug	= m_pConfig->m_bDebug;
	m_bBeep		= m_pConfig->m_bBeep;

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
		int cx[]			= {90, 60, 100, 42, 42, 60, 45, 40, 60};
		LPCTSTR lpszCol[]	= {_T("디바이스"), _T("통신"), _T("IP 주소"), _T("포트1"), _T("포트2"),  _T("연결"), _T("재시도"), _T("로그"), _T("Timeout")};
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i=0; i<nCol; ++i)
			pListCtrl1->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	m_ctlEquip.LoadImage(IDI_ICON_MODIFY, -1);
	m_ctlEquip.InsertColumn(_T("DEVICE"), _T("접속시간"), _T("접속 IP주소"), _T("접속순번"), _T("접속포트"), _T("통신연결"), _T("ETX"));
	m_pDoc->m_pEquipments.Load(&m_ctlEquip);

	UpdateListEhternet();

	return TRUE;
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
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[0]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[1]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, pPort->m_bConnect ? _T("Yes") : _T("No"), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, pPort->m_bRetry ? _T("Yes") : _T("No"), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, pPort->m_bWriteLog ? _T("Yes") : _T("No"), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString((int)pPort->m_dwRecvTimeout), 0, 0, 0, 0);
		pList->SetItemData(nIndex, (LPARAM)pEquipment);
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
	
	m_pConfig->m_bJob		= m_bJob;
	m_pConfig->m_bEvent		= m_bEvent;
	m_pConfig->m_bError		= m_bError;
	m_pConfig->m_bAlarm		= m_bAlarm;
	m_pConfig->m_bDebug		= m_bDebug;
	m_pConfig->m_bBeep		= m_bBeep;

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

	// CV는 Port를 1개만 사용하도록(Socket[0]만 사용!) 하기 위해서 
//	CPortEthernetDlg dlg(&(pEquipment->m_port));
	CPortEthernetDlg dlg(pEquipment);
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


void CConfigStdPage::OnNMDblclkListEquipment(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION pos = m_ctlEquip.GetFirstSelectedItemPosition();
	CString strTemp;
	if (pos == NULL)
	{
		AfxMessageBox(_T("선택 정보를 가져오지 못함"));
		return;
	}
	
	int nIndex = m_ctlEquip.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex >= m_ctlEquip.GetItemCount()))
	{
		AfxMessageBox(_T("선택 정보가 이상함![nIndex=%d][ItemCount=%d]"), nIndex, m_ctlEquip.GetItemCount());
		return;
	}
	CEquipment* pEquipment = NULL;
	pEquipment = (CEquipment*)m_ctlEquip.GetItemData(nIndex);

	if (pEquipment == NULL)
	{
		AfxMessageBox(_T("설비 정보(pEquipment)를 가져오지 못함"));
		return;
	}

	int nIdx = m_ctlEquip.GetSelectionMark();
	if (nIdx < 0 || nIdx >= PLC_CONN_PORT_CNT)
		return;

	strTemp.Format(_T("의 %d번 소켓[Port:%d]"), nIdx + 1, pEquipment->m_port.m_nPort[nIdx]);

	if (pEquipment->IsConnect(nIdx))
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + strTemp + _T(" 통신을 중단하시겠습니까?"), MB_YESNO) != IDYES)
			return;
			
		pEquipment->Disconnect(nIdx);
	}
	else
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + strTemp + _T(" 통신을 연결하시겠습니까?"), MB_YESNO) != IDYES)
			return;
			
		pEquipment->ConnectServer(nIdx);
	}	

	*pResult = 0;
}
