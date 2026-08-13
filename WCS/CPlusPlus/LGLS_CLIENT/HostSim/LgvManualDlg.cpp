// LgvManualDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "LgvManualDlg.h"
#include "Lgv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLgvManualDlg dialog


CLgvManualDlg::CLgvManualDlg(CLgv* pLgv, CWnd* pParent /* = NULL */)
	: CDialog(CLgvManualDlg::IDD, pParent)
{
	m_pLgv = pLgv;
	DEBUGER_ASSERT_VALID(m_pLgv != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo != NULL);

	//{{AFX_DATA_INIT(CLgvManualDlg)
	//}}AFX_DATA_INIT
}


void CLgvManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLgvManualDlg)
	DDX_Control(pDX, IDC_COMBO_DEPART_HS, m_cbxDepartsHS);
	DDX_Control(pDX, IDC_COMBO_ARRIVE_HS, m_cbxArrivesHS);
	DDX_Control(pDX, IDC_COMBO_GEN_CODE, m_cbxGenCode);
	DDX_Control(pDX, IDC_COMBO_PRIORITY, m_cbxPriority);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLgvManualDlg, CDialog)
	//{{AFX_MSG_MAP(CLgvManualDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLgvManualDlg message handlers

BOOL CLgvManualDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cbxGenCode.AddString(_T("없음"));
	m_cbxGenCode.SetItemData(0, 0);
	m_cbxGenCode.SetCurSel(0);
	CLib::SetComboBoxGenCode(m_cbxGenCode);

	int i = 0;
	CString str;
	for (i=0; i<5; ++i)
	{
		str.Format(_T("PRIORITY%d"), i);
		m_cbxPriority.AddString(str);
		m_cbxPriority.SetItemData(i, i);
	}
	m_cbxPriority.SetCurSel(0);

	CTrackHS* pTrackHS = NULL;
	CLgvInfo* pInfo = m_pLgv->m_pInfo;
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	int nHsCount = pInfo->m_pDepartsHS.GetSize();
	for (i=0; i<nHsCount; i++)
	{
		pTrackHS = pInfo->m_pDepartsHS[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

		str.Format(_T("[ST%03d] TR%03d"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);
		m_cbxDepartsHS.AddString(str);
		m_cbxDepartsHS.SetItemData(i, pTrackHS->m_nPos);
	}
	m_cbxDepartsHS.SetCurSel(0);

	nHsCount = pInfo->m_pArrivesHS.GetSize();		//@.@
	for (i=0; i<nHsCount; i++)
	{
		pTrackHS = pInfo->m_pArrivesHS[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

//		if (pTrackHS->m_pTrack == NULL)
//			continue;

		str.Format(_T("[ST%03d] TR%03d"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);
		m_cbxArrivesHS.AddString(str);
		m_cbxArrivesHS.SetItemData(i, pTrackHS->m_nPos);
	}
	m_cbxArrivesHS.SetCurSel(0);
	
	INIT_BUTTON(m_btnCancel, IDI_CANCEL3, 16, 16, _T("LGV 수동지시 취소"));
	INIT_BUTTON(m_btnOK, IDI_OK3, 16, 16, _T("LGV 수동지시"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLgvManualDlg::OnOK() 
{
	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	if (m_cbxDepartsHS.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("출발위치를 선택해주세요!"));
		return;
	}

	if (m_cbxArrivesHS.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("도착위치를 선택해주세요!"));
		return;
	}

//	if (m_cbxGenCode.GetCurSel() == CB_ERR)
//	{
//		AfxMessageBox(_T("GEN 정보를 선택해주세요!"));
//		return;
//	}
//
//	if (m_cbxPriority.GetCurSel() == CB_ERR)
//	{
//		AfxMessageBox(_T("우선순위를 선택해주세요!"));
//		return;
//	}

	int nTranportID = GetDlgItemInt(IDC_EDIT_TRANSPORT_ID);
//	if (m_pLgv->m_pInfo->GetLgvJobItem(nTranportID))
//	{
//		AfxMessageBox(_T("동일한 LGV 작업번호가 존재합니다. 다시 입력해주세요!"));
//		return;
//	}

	CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CLgvCmdMsg::enTRR;
	pCmdMsg->m_strValues.SetSize(CLgvCmdMsg::enSizeTRR);
	pCmdMsg->m_strValues[0].Format(_T("%d"), nTranportID);
//	pCmdMsg->m_strValues[1].Format(_T("%d"), m_cbxGenCode.GetItemData(m_cbxGenCode.GetCurSel()));
//	pCmdMsg->m_strValues[1].Format(_T("%d"), m_cbxPriority.GetItemData(m_cbxPriority.GetCurSel()));
	pCmdMsg->m_strValues[1].Format(_T("%d"), m_cbxDepartsHS.GetItemData(m_cbxDepartsHS.GetCurSel()));
	pCmdMsg->m_strValues[2].Format(_T("%d"), m_cbxArrivesHS.GetItemData(m_cbxArrivesHS.GetCurSel()));
	pCmdMsg->m_strValues[3] = "";

	m_pLgv->SetCmdMsg(pCmdMsg);	
	
	CDialog::OnOK();
}
