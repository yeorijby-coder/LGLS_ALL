// ConfigStdPage.cpp : implementation file

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
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
}

BEGIN_MESSAGE_MAP(CConfigStdPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigStdPage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COMM1, OnDblclkListCommPortEthernet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CConfigStdPage::OnInitDialog() 
{
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
		int cx[]			= {90, 60, 100, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 60, 45, 40, 60};
		LPCTSTR lpszCol[]	= {_T("디바이스"), _T("통신"), _T("IP 주소"), _T("포트1"), _T("포트2"), _T("포트3"), _T("포트4"), _T("포트5"), _T("포트6"), _T("포트7"), _T("포트8"), _T("포트9"), _T("포트10"), _T("포트11"), _T("포트12"), _T("포트13"), _T("포트14"), _T("포트15"), _T("포트16"), _T("포트17"), _T("포트18"), _T("연결"), _T("재시도"), _T("로그"), _T("Timeout")};
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i=0; i<nCol; ++i)
			pListCtrl1->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}
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
		if (pEquipment == NULL)
			continue;

		// CV 시뮬레이터 이므로 CV만 처리함!
		if (pEquipment->m_enKind != CEquipment::enCV)
			continue;

		pPort = &(pEquipment->m_port);
//		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		nIndex = pList->GetItemCount();
		pList->InsertItem(nMask, nIndex, pPort->m_strDevice, 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, _T("Ethernet"), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, pPort->m_strIP, 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[0]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[1]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[2]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[3]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[4]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[5]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[6]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[7]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[8]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[9]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[10]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[11]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[12]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[13]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[14]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[15]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[16]), 0, 0, 0, 0);
		pList->SetItem(nIndex, ++j, nMask, CConvert::ToString(pPort->m_nPort[17]), 0, 0, 0, 0);
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