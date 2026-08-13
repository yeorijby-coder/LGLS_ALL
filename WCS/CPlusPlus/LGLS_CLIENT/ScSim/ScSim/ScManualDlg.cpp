// ScManualDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "ScManualDlg.h"
#include "Sc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScManualDlg dialog


CScManualDlg::CScManualDlg(CSc* pSc, CWnd* pParent /* = NULL */)
	: CDialog(CScManualDlg::IDD, pParent)
{
	m_pSc = pSc;
	DEBUGER_ASSERT_VALID(m_pSc != NULL);

	//{{AFX_DATA_INIT(CScManualDlg)
	m_strDepartLoc = _T("");
	m_strArriveLoc = _T("");
	//}}AFX_DATA_INIT
}


void CScManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScManualDlg)
	DDX_Control(pDX, IDC_TAB_COMMAND, m_tabCommand);
	DDX_Control(pDX, IDC_COMBO_DEPART, m_cbxDepartHS);
	DDX_Control(pDX, IDC_COMBO_ARRIVE, m_cbxArriveHS);
	DDX_Text(pDX, IDC_EDIT_DEPART, m_strDepartLoc);
	DDX_Text(pDX, IDC_EDIT_ARRIVE, m_strArriveLoc);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScManualDlg, CDialog)
	//{{AFX_MSG_MAP(CScManualDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_COMMAND, OnSelchangeTabCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScManualDlg message handlers

BOOL CScManualDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);
	SetWindowText(m_pSc->m_port.m_strDevice + " 수동지시");

	LRESULT lResult = 0;
	m_tabCommand.InsertItem(TCIF_TEXT|TCIF_PARAM, 0, _T("입고"), 0, CScCmdMsg::enCmdManualSto);
	m_tabCommand.InsertItem(TCIF_TEXT|TCIF_PARAM, 1, _T("출고"), 0, CScCmdMsg::enCmdManualRet);
	m_tabCommand.InsertItem(TCIF_TEXT|TCIF_PARAM, 2, _T("직출고"), 0, CScCmdMsg::enCmdManualHs2Hs);
	m_tabCommand.InsertItem(TCIF_TEXT|TCIF_PARAM, 3, _T("랙투랙"), 0, CScCmdMsg::enCmdManualR2R);
	m_tabCommand.SetCurSel(0);
	OnSelchangeTabCommand(NULL, &lResult);

	CTrackHS* pTrackHS = NULL;
	CTrackHSArray& pArrStoHS = m_pSc->m_pInfo->m_pStosHS;
	CTrackHSArray& pArrRetHS = m_pSc->m_pInfo->m_pRetsHS;

	int i=0;
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

void CScManualDlg::OnOK() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (m_pSc->m_pInfo->IsInvoked())
	{
		AfxMessageBox(_T("현재 작업중입니다."));
		return;
	}

	if (m_pSc->m_pInfo->IsWaitStatus() == FALSE)
	{
		AfxMessageBox(_T("현재 대기상태가 아닙니다."));
		return;
	}

	if (m_pSc->m_pInfo->IsAutoMode() == FALSE)
	{
		AfxMessageBox(_T("현재 자동상태가 아닙니다."));
		return;
	}

	if (m_pSc->m_pInfo->IsOnlineMode() == FALSE)
	{
		AfxMessageBox(_T("현재 ONLINE 상태가 아닙니다."));
		return;
	}

	if (AfxMessageBox(m_pSc->m_port.m_strDevice + " 수동 작업을 하시겠습니까?", MB_OKCANCEL) == IDCANCEL)
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
			pCmdMsg->m_strValues.SetSize(CScCmdMsg::enSizeScCommand);
			pCmdMsg->m_strValues[0].Format(_T("%d"), LOG_SYSTEM);
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

			CScCmdMsg* pCmdMsg = new CScCmdMsg;
			DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
			pCmdMsg->m_nCommand = CScCmdMsg::enCmdManualRet;
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

void CScManualDlg::OnSelchangeTabCommand(NMHDR* pNMHDR, LRESULT* pResult) 
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
