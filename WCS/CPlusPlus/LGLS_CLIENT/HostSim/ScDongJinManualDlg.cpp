// ScDongJinManualDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "ScDongJinManualDlg.h"
#include "ScDongJin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScDongJinManualDlg dialog


CScDongJinManualDlg::CScDongJinManualDlg(CScDongJin* pSc, int nEquipNum, CWnd* pParent /* = NULL */)
	: CDialog(CScDongJinManualDlg::IDD, pParent)
{
//	m_pSc = pSc;
//	m_nEquipNum = nEquipNum;
//	DEBUGER_ASSERT_VALID(m_pSc != NULL);

	m_pSc = pSc;
	m_nEquipNum = pSc->m_nNumber;
	DEBUGER_ASSERT_VALID(m_pSc != NULL);

	m_pScInfo = (CScInfo *)m_pSc->GetInfo(nEquipNum, FALSE);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	m_strDeviceName.Format(_T("%s [S/C #%d]"), m_pSc->m_port.m_strDevice, nEquipNum);
	
	//{{AFX_DATA_INIT(CScDongJinManualDlg)
	m_strDepartLoc = _T("");
	m_strArriveLoc = _T("");
	//}}AFX_DATA_INIT
}


void CScDongJinManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScDongJinManualDlg)
	DDX_Control(pDX, IDC_TAB_COMMAND, m_tabCommand);
	DDX_Control(pDX, IDC_COMBO_DEPART, m_cbxDepartHS);
	DDX_Control(pDX, IDC_COMBO_ARRIVE, m_cbxArriveHS);
	DDX_Text(pDX, IDC_EDIT_DEPART, m_strDepartLoc);
	DDX_Text(pDX, IDC_EDIT_ARRIVE, m_strArriveLoc);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScDongJinManualDlg, CDialog)
	//{{AFX_MSG_MAP(CScDongJinManualDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_COMMAND, OnSelchangeTabCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScDongJinManualDlg message handlers

BOOL CScDongJinManualDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);
	SetWindowText(m_strDeviceName + " 수동지시");

	LRESULT lResult = 0;
	m_tabCommand.InsertItem(TCIF_TEXT|TCIF_PARAM, 0, _T("입고"), 0, CScCmdMsg::enCmdManualSto);
	m_tabCommand.InsertItem(TCIF_TEXT|TCIF_PARAM, 1, _T("출고"), 0, CScCmdMsg::enCmdManualRet);
	m_tabCommand.InsertItem(TCIF_TEXT|TCIF_PARAM, 2, _T("직출고"), 0, CScCmdMsg::enCmdManualHs2Hs);
//	m_tabCommand.InsertItem(TCIF_TEXT|TCIF_PARAM, 2, _T("랙투랙"), 0, CScCmdMsg::enCmdManualR2R);
	m_tabCommand.InsertItem(TCIF_TEXT|TCIF_PARAM, 3, _T("랙투랙"), 0, CScCmdMsg::enCmdManualR2R);
	m_tabCommand.SetCurSel(0);
	OnSelchangeTabCommand(NULL, &lResult);

	CTrackHS* pTrackHS = NULL;
	CTrackHSArray& pArrStoHS = m_pScInfo->m_pStosHS;
	CTrackHSArray& pArrRetHS = m_pScInfo->m_pRetsHS;
	int i = 0;
	for (i=0; i<pArrStoHS.GetSize(); ++i)
	{
		pTrackHS = pArrStoHS[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		
		m_cbxDepartHS.AddString(pTrackHS->ToString());
		m_cbxDepartHS.SetItemData(i, pTrackHS->m_nPos);
	}

	for (i=0; i<pArrRetHS.GetSize(); ++i)
	{
		pTrackHS = pArrRetHS[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		
		m_cbxArriveHS.AddString(pTrackHS->ToString());
		m_cbxArriveHS.SetItemData(i, pTrackHS->m_nPos);
	}

	INIT_BUTTON(m_btnOK, IDI_OK, 32, 32, _T("수동작업 지시"));
	INIT_BUTTON(m_btnCancel, IDI_CANCEL, 32, 32, _T("수동작업 취소"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScDongJinManualDlg::OnOK() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

//	if (m_pScInfo->IsInvoked())
//	{
//		AfxMessageBox(_T("현재 작업중입니다."));
//		return;
//	}

	if (m_pScInfo->IsScJobStatusIdle() == FALSE)
	{
		AfxMessageBox(_T("현재 대기상태가 아닙니다."));
		return;
	}

	if (m_pScInfo->IsScJobStatusScRemoteMode() == FALSE)
	{
		AfxMessageBox(_T("현재 기상반이 원격 상태가 아닙니다."));
		return;
	}

	if (m_pScInfo->IsScJobStatusRcRemoteMode() == FALSE)
	{
		AfxMessageBox(_T("현재 지상반이 원격 상태가 아닙니다."));
		return;
	}

	if (AfxMessageBox(m_strDeviceName + " 수동 작업을 하시겠습니까?", MB_OKCANCEL) == IDCANCEL)
		return;

	if (!UpdateData(TRUE))
		return;

	switch (m_tabCommand.GetCurSel())
	{
	case enTabStore:
		{
			if (m_cbxDepartHS.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("출발지를 선택해주세요."));
				return;
			}

			if (!m_pSc->IsValidLocation(m_strArriveLoc))
			{
				AfxMessageBox(m_pSc->m_strLog);
				return;
			}
			
			CScCmdMsg* pCmdMsg = new CScCmdMsg;
			DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
			pCmdMsg->m_nCommand = CScCmdMsg::enCmdManualSto;
			pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
			pCmdMsg->m_strValues.SetSize(CScCmdMsg::enSizeScCommand);
			pCmdMsg->m_strValues[0].Format(_T("%d"), (m_pScInfo->m_ScInfo[0].m_nInternalLuggNum == 0) ? LOG_SYSTEM : m_pScInfo->m_ScInfo[0].m_nInternalLuggNum);
			pCmdMsg->m_strValues[1].Format(_T("%d"), m_cbxDepartHS.GetItemData(m_cbxDepartHS.GetCurSel()));
			pCmdMsg->m_strValues[2].Format(_T("%s"), m_strArriveLoc);
			m_pSc->SetCmdMsg(pCmdMsg);
		}
		break;

	case enTabRetrieve:
		{
			if (!m_pSc->IsValidLocation(m_strDepartLoc))
			{
				AfxMessageBox(m_pSc->m_strLog);
				return;
			}
			
			if (m_cbxArriveHS.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("도착지를 선택해주세요."));
				return;
			}

//		enCmdManualSto		= 19, 
//		enCmdManualRet		= 20,  
//		enCmdManualHs2Hs	= 21,   
//		enCmdManualR2R		= 22

			CScCmdMsg* pCmdMsg = new CScCmdMsg;
			DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
			pCmdMsg->m_nCommand = CScCmdMsg::enCmdManualRet;
			pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
			pCmdMsg->m_strValues.SetSize(CScCmdMsg::enSizeScCommand);
			pCmdMsg->m_strValues[0].Format(_T("%d"), LOG_SYSTEM);
			pCmdMsg->m_strValues[1].Format(_T("%s"), m_strDepartLoc);
			pCmdMsg->m_strValues[2].Format(_T("%d"), m_cbxArriveHS.GetItemData(m_cbxArriveHS.GetCurSel()));
			m_pSc->SetCmdMsg(pCmdMsg);
		}
		break;

	case enTabHs2Hs:
		{
			if (m_cbxDepartHS.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("출발지를 선택해주세요."));
				return;
			}
			
			if (m_cbxArriveHS.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("도착지를 선택해주세요."));
				return;
			}

			CScCmdMsg* pCmdMsg = new CScCmdMsg;
			DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
			pCmdMsg->m_nCommand = CScCmdMsg::enCmdManualHs2Hs;
			pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
			pCmdMsg->m_strValues.SetSize(CScCmdMsg::enSizeScCommand);
			pCmdMsg->m_strValues[0].Format(_T("%d"), LOG_SYSTEM);
			pCmdMsg->m_strValues[1].Format(_T("%d"), m_cbxDepartHS.GetItemData(m_cbxDepartHS.GetCurSel()));
			pCmdMsg->m_strValues[2].Format(_T("%d"), m_cbxArriveHS.GetItemData(m_cbxArriveHS.GetCurSel()));
			m_pSc->SetCmdMsg(pCmdMsg);
		}
		break;

	case enTabRack2Rack:
		{
			if (!m_pSc->IsValidLocation(m_strDepartLoc))
			{
				AfxMessageBox(m_pSc->m_strLog);
				return;
			}
			
			if (!m_pSc->IsValidLocation(m_strArriveLoc))
			{
				AfxMessageBox(m_pSc->m_strLog);
				return;
			}

			CScCmdMsg* pCmdMsg = new CScCmdMsg;
			DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
			pCmdMsg->m_nCommand = CScCmdMsg::enCmdManualR2R;
			pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
			pCmdMsg->m_strValues.SetSize(CScCmdMsg::enSizeScCommand);
			pCmdMsg->m_strValues[0].Format(_T("%d"), LOG_SYSTEM);
			pCmdMsg->m_strValues[1].Format(_T("%s"), m_strDepartLoc);
			pCmdMsg->m_strValues[2].Format(_T("%s"), m_strArriveLoc);
			m_pSc->SetCmdMsg(pCmdMsg);
		}
		break;
	}

	CDialog::OnOK();
}

void CScDongJinManualDlg::OnSelchangeTabCommand(NMHDR* pNMHDR, LRESULT* pResult) 
{
	switch (m_tabCommand.GetCurSel())
	{
	case enTabStore:
		GetDlgItem(IDC_COMBO_DEPART)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_ARRIVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEPART)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ARRIVE)->EnableWindow(TRUE);
		break;

	case enTabRetrieve:
		GetDlgItem(IDC_COMBO_DEPART)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ARRIVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DEPART)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ARRIVE)->EnableWindow(FALSE);
		break;

	case enTabHs2Hs:
		GetDlgItem(IDC_COMBO_DEPART)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_ARRIVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DEPART)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ARRIVE)->EnableWindow(FALSE);
		break;

	case enTabRack2Rack:
		GetDlgItem(IDC_COMBO_DEPART)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ARRIVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEPART)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ARRIVE)->EnableWindow(TRUE);
		break;
	}
	
	*pResult = 0;
}
