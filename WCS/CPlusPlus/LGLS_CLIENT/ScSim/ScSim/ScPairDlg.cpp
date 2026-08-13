// ScPairDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "ScPairDlg.h"
#include "ScPair.h"
//#include "ScPairManualDlg.h"
//#include "ScPairRelocationDlg.h"
//#include "ScCheckDelDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScPairDlg dialog


CScPairDlg::CScPairDlg(CScPair* pSc, CWnd* pParent /* = NULL */)
	: CDialog(CScPairDlg::IDD, pParent)
{
	m_pSc = pSc;
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);
	
	//{{AFX_DATA_INIT(CScPairDlg)
	m_bRetSuspend = m_pSc->m_pInfo->m_bRetSuspend;
	m_bStoSuspend = m_pSc->m_pInfo->m_bStoSuspend;
	//}}AFX_DATA_INIT
}


void CScPairDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScPairDlg)
	DDX_Control(pDX, IDC_BUTTON_COMPLETE, m_btnComplete);
	DDX_Control(pDX, IDC_BUTTON_EMERGENCY, m_btnEmergency);
	DDX_Control(pDX, IDC_BUTTON_ACTIVE, m_btnActive);
	DDX_Control(pDX, IDC_BUTTON_ESTOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTTON_ERROR_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_CALL_TO_HOME, m_btnHome);
	DDX_Control(pDX, IDC_BUTTON_DUAL_STORE, m_btnDualStore);
	DDX_Control(pDX, IDC_BUTTON_MANUAL, m_btnManual);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Check(pDX, IDC_CHECK_SUSPEND_RETRIEVE, m_bRetSuspend);
	DDX_Check(pDX, IDC_CHECK_SUSPENT_STORE, m_bStoSuspend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScPairDlg, CDialog)
	//{{AFX_MSG_MAP(CScPairDlg)
	ON_BN_CLICKED(IDC_BUTTON_COMPLETE, OnButtonComplete)
	ON_BN_CLICKED(IDC_BUTTON_EMERGENCY, OnButtonEmergency)
	ON_BN_CLICKED(IDC_BUTTON_ACTIVE, OnButtonActive)
	ON_BN_CLICKED(IDC_BUTTON_ESTOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_ERROR_RESET, OnButtonErrorReset)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_CALL_TO_HOME, OnButtonCallToHome)
	ON_BN_CLICKED(IDC_BUTTON_DUAL_STORE, OnButtonDualStore)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL, OnButtonManual)
	ON_BN_CLICKED(IDC_CHECK_SUSPENT_STORE, OnCheckSuspentStore)
	ON_BN_CLICKED(IDC_CHECK_SUSPEND_RETRIEVE, OnCheckSuspendRetrieve)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScPairDlg message handlers

BOOL CScPairDlg::OnInitDialog() 
{
	m_pSc->m_pEquipDlg = this;

	CDialog::OnInitDialog();

	SetDlgItemText(IDC_EDIT_DEVICE, m_pSc->m_port.m_strDevice);

	/////////////////////////////////////////////////////////////////////////

	int w = 16, h = 16;
	INIT_BUTTON(m_btnComplete, IDI_ICON_COMPLETE, w, h, _T("강제완료"));
	INIT_BUTTON(m_btnEmergency, IDI_ICON_ESTOP, w, h, _T("강제완료"));
	INIT_BUTTON(m_btnStop, IDI_ICON_LEDOFF, w, h, _T("정지지시"));
	INIT_BUTTON(m_btnActive, IDI_ICON_LEDON, w, h, _T("ACTIVE 지시"));
	INIT_BUTTON(m_btnReset, IDI_ICON_RETURN, w, h, _T("에러 리셋"));

//	if (m_pSc->m_pInfo->IsDualStoreError() == TRUE)
//	{
//		ENABLE_WND(IDC_BUTTON_ERROR_RESET, FALSE)
//	}

	INIT_BUTTON(m_btnDelete, IDI_CANCEL, w, h, _T("작업삭제"));
	INIT_BUTTON(m_btnHome, IDI_ICON_HOME, w, h, _T("홈복귀"));
	INIT_BUTTON(m_btnDualStore, IDI_RUN, w, h, _T("이중입고재지정"));
	INIT_BUTTON(m_btnManual, IDI_ICON_MANUAL, w, h, _T("수동지시"));
	INIT_BUTTON(m_btnOK, IDI_EXIT, w, h, _T("창닫기"));

	WindowProc(WM_USER_REFRESH_NOTIFY, 0, 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CScPairDlg::DestroyWindow() 
{
	m_pSc->m_pEquipDlg = NULL;
	
	return CDialog::DestroyWindow();
}

LRESULT CScPairDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_USER_REFRESH_NOTIFY:
		{
//			CScPairInfo* pInfo = m_pSc->m_pInfo;
			CScInfo* pInfo = m_pSc->m_pInfo;
			DEBUGER_ASSERT_VALID(pInfo != NULL);

			SetDlgItemInt(IDC_EDIT_LUGG_NUM, pInfo->m_ScInfo[0].m_nLuggNum);
			SetDlgItemText(IDC_EDIT_CMD_TYPE, pInfo->GetCmdType());
			SetDlgItemText(IDC_EDIT_START_POS, pInfo->GetDepartInfoString());
			SetDlgItemText(IDC_EDIT_DEST_POS, pInfo->GetArriveInfoString());

			SetDlgItemInt(IDC_EDIT_LUGG_NUM2, pInfo->m_ScInfo[1].m_nLuggNum);
			SetDlgItemText(IDC_EDIT_CMD_TYPE2, pInfo->GetCmdType());
			SetDlgItemText(IDC_EDIT_START_POS2, pInfo->GetDepartInfoString2());
			SetDlgItemText(IDC_EDIT_DEST_POS2, pInfo->GetArriveInfoString2());

			SetDlgItemText(IDC_EDIT_RC_MODE, pInfo->GetRcMode());
			SetDlgItemText(IDC_EDIT_RC_STATUS, pInfo->GetRcStatus());
//			SetDlgItemInt(IDC_EDIT_JOB_COUNT, pInfo->m_ucJobCount);
			SetDlgItemText(IDC_EDIT_SC_MODE, pInfo->GetScMode());
			SetDlgItemText(IDC_EDIT_SC_STATUS, pInfo->GetScStatus());
			SetDlgItemText(IDC_EDIT_FORK_POS, pInfo->GetForkPos1());
			SetDlgItemText(IDC_EDIT_FORK_STATUS, pInfo->GetForkStatus());
			SetDlgItemText(IDC_EDIT_PROD_LOAD, pInfo->GetProdLoad());
			SetDlgItemText(IDC_EDIT_SC_JOB_STATUS, pInfo->GetJobStatus());
			SetDlgItemInt(IDC_EDIT_HORIZONTAL_POS, pInfo->m_wHorizontalPos);
			SetDlgItemInt(IDC_EDIT_VERTICAL_POS, pInfo->m_wVerticalPos);
//			SetDlgItemInt(IDC_EDIT_COMPLETE_NUM, pInfo->m_nCompleteNum);
			SetDlgItemText(IDC_EDIT_PREV_STATUS, /*pInfo->m_ucPrevStatus */pInfo->GetPrevStatus());

			CJobItem* pJobItem = m_pSc->m_pDoc->m_pJob->Find(pInfo->m_ScInfo[0].m_nInternalLuggNum);
			SetDlgItemInt(IDC_EDIT_INTERNAL_LUGG_NUM, pInfo->m_ScInfo[0].m_nInternalLuggNum);
			SetDlgItemText(IDC_EDIT_ECS_JOB_TYPE, CJobItem::GetJobTypeString(pInfo->m_ScInfo[0].m_nInternalJobType));
			SetDlgItemText(IDC_EDIT_JOB_START_POS, pJobItem ? pJobItem->GetStartStationString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_DEST_POS, pJobItem ? pJobItem->GetDestStationString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_STATUS, pJobItem ? pJobItem->GetJobStatusString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_PROD_INFO, pJobItem ? pJobItem->m_strPalletNo : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_PROD_INFO3, pJobItem ? pJobItem->m_strProductID : _T(""));	

			/*CJobItem*/ pJobItem = m_pSc->m_pDoc->m_pJob->Find(pInfo->m_ScInfo[1].m_nInternalLuggNum);
			SetDlgItemInt(IDC_EDIT_INTERNAL_LUGG_NUM2, pInfo->m_ScInfo[1].m_nInternalLuggNum);
			SetDlgItemText(IDC_EDIT_ECS_JOB_TYPE2, CJobItem::GetJobTypeString(pInfo->m_ScInfo[1].m_nInternalJobType));
			SetDlgItemText(IDC_EDIT_JOB_START_POS2, pJobItem ? pJobItem->GetStartStationString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_DEST_POS2, pJobItem ? pJobItem->GetDestStationString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_STATUS2, pJobItem ? pJobItem->GetJobStatusString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_PROD_INFO4, pJobItem ? pJobItem->m_strPalletNo : _T(""));			//
			SetDlgItemText(IDC_EDIT_JOB_PROD_INFO5, pJobItem ? pJobItem->m_strProductID : _T(""));			//

			SetDlgItemText(IDC_EDIT_DIAGNOSIS, m_pSc->m_pInfo->GetErrorString());
		}
		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////

void CScPairDlg::OnButtonComplete() 
{
	//DEBUGER_ASSERT_VALID(m_pSc != NULL);
	//DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	//if (!m_pSc->IsConnect())
	//{
	//	AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
	//	return;
	//}

	//if (m_pSc->m_pInfo->IsInvoked() == FALSE)
	//{
	//	AfxMessageBox("S/C 상태 확인 바람.. 현재 S/C이 수행 중인 작업이 없습니다!");
	//	return;
	//}

	//if (m_pSc->m_pInfo->IsWaitStatus() == FALSE)
	//{
	//	AfxMessageBox("SC 상태 확인 바람..  현재 SC는 대기 상태가 아닙니다!");
	//	return;
	//}

	//if (m_pSc->m_pInfo->IsProductEmpty() == FALSE )
	//{
	//	AfxMessageBox("SC 상태 확인 바람..  현재 SC에 화물이 적재된 상태 입니다!");
	//	return;
	//}

	//if (AfxMessageBox(_T("현 작업을 강제완료 하시겠습니까?"), MB_YESNO) == IDNO)
	//	return;

	//CScCmdMsg* pCmdMsg = new CScCmdMsg;
	//DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	//pCmdMsg->m_nCommand = CScCmdMsg::enCmdComplete;
	//m_pSc->SetCmdMsg(pCmdMsg);
}

void CScPairDlg::OnButtonEmergency() 
{
	//DEBUGER_ASSERT_VALID(m_pSc != NULL);
	//DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	//if (!m_pSc->IsConnect())
	//{
	//	AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
	//	return;
	//}

	//if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	//{
	//	AfxMessageBox("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!");
	//	return;
	//}

	//if (AfxMessageBox(_T("비상정지를 지시하시겠습니까?"), MB_YESNO) == IDNO)
	//	return;

	//CScCmdMsg* pCmdMsg = new CScCmdMsg;
	//DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	//pCmdMsg->m_nCommand = CScCmdMsg::enCmdEmergency;
	//m_pSc->SetCmdMsg(pCmdMsg);
}

void CScPairDlg::OnButtonActive() 
{
	//DEBUGER_ASSERT_VALID(m_pSc != NULL);
	//DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	//if (!m_pSc->IsConnect())
	//{
	//	AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
	//	return;
	//}

	//if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	//{
	//	AfxMessageBox("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!");
	//	return;
	//}

	//if (AfxMessageBox(_T("ACTIVE 상태로 변경하시겠습니까?\n\n현장에 사람이 없는지 확인후 진행해주세요."), MB_YESNO) == IDNO)
	//	return;

	//CScCmdMsg* pCmdMsg = new CScCmdMsg;
	//DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	//pCmdMsg->m_nCommand = CScCmdMsg::enCmdActive;
	//m_pSc->SetCmdMsg(pCmdMsg);
}

void CScPairDlg::OnButtonStop() 
{
	//DEBUGER_ASSERT_VALID(m_pSc != NULL);
	//DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	//if (!m_pSc->IsConnect())
	//{
	//	AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
	//	return;
	//}

	//if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	//{
	//	AfxMessageBox("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!");
	//	return;
	//}

	//if (AfxMessageBox(_T("STOP 상태로 변경하시겠습니까?"), MB_YESNO) == IDNO)
	//	return;

	//CScCmdMsg* pCmdMsg = new CScCmdMsg;
	//DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	//pCmdMsg->m_nCommand = CScCmdMsg::enCmdStop;
	//m_pSc->SetCmdMsg(pCmdMsg);
}

void CScPairDlg::OnButtonErrorReset() 
{
	//DEBUGER_ASSERT_VALID(m_pSc != NULL);
	//DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	//if (!m_pSc->IsConnect())
	//{
	//	AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
	//	return;
	//}

	//if (m_pSc->m_pInfo->IsDualStoreError() == TRUE)
	//{
	//	if (AfxMessageBox(_T("이중입고 에러일때는 에러를 해제하면 S/C을 수동 및 반자동 조작해야 할수도 있습니다.\n정말로 에러를 해제 하시겠습니까?"), MB_YESNO) == IDNO)
	//	{
	//		return;
	//	}
	//}

	//if (AfxMessageBox(_T("에러를 RESET 하시겠습니까?"), MB_YESNO) == IDNO)
	//	return;

	//CScCmdMsg* pCmdMsg = new CScCmdMsg;
	//DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	//pCmdMsg->m_nCommand = CScCmdMsg::enCmdReset;
	//m_pSc->SetCmdMsg(pCmdMsg);
}

void CScPairDlg::OnButtonCancel() 
{
	/*
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect())
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	{
		AfxMessageBox("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!");
		return;
	}

	CScCheckDelDlg ScCheckDelDlg;				
	ScCheckDelDlg.m_pDoc = m_pSc->m_pDoc;	//m_pDoc;

	if (ScCheckDelDlg.DoModal() != IDOK) 		
		return;

	CString strTemp;							
	int nCommand;
	if (ScCheckDelDlg.m_nCheck == 1)		{	strTemp = "Fork #1 작업을 삭제하시겠습니까?\n";		nCommand = CScCmdMsg::enCmdCancel1;		}
	else if (ScCheckDelDlg.m_nCheck == 2)	{	strTemp = "Fork #2 작업을 삭제하시겠습니까?\n";		nCommand = CScCmdMsg::enCmdCancel2;		}
	else if (ScCheckDelDlg.m_nCheck == 3)	{	strTemp = "Fork #1,2 작업을 삭제하시겠습니까?\n";	nCommand = CScCmdMsg::enCmdCancel3;		}

	if (AfxMessageBox(_T(strTemp), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = nCommand;
	m_pSc->SetCmdMsg(pCmdMsg);
	//*/
}

void CScPairDlg::OnButtonCallToHome() 
{
	/*
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect())
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if (m_pSc->m_pInfo->IsInvoked())
	{
		AfxMessageBox("S/C 상태 확인 바람.. 현재 S/C이 작업을 수행 중입니다!");
		return;
	}

	if (m_pSc->m_pInfo->IsWaitStatus() == FALSE)
	{
		AfxMessageBox("SC 상태 확인 바람..  현재 SC는 대기 상태가 아닙니다!");
		return;
	}

	if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	{
		AfxMessageBox("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!");
		return;
	}

	if (AfxMessageBox(_T("홈복귀를 지시하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdHomeReturn;
	m_pSc->SetCmdMsg(pCmdMsg);
	//*/
}

void CScPairDlg::OnButtonDualStore() 
{
	/*
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pDoc->m_pJob != NULL);

	if (m_pSc->m_pInfo->IsDualStoreError() == FALSE && 
//		m_pSc->m_pInfo->IsEmptyRetrieveError() == FALSE &&		//
//		m_pSc->m_pInfo->IsRetrieveObstacle() == FALSE &&		//
		m_pSc->m_pInfo->IsStoreObstacle() == FALSE )
	{
		AfxMessageBox("이 기능은 이중입고 & 입고장애 에러 상황에서만 사용할 수 있습니다.");
//		AfxMessageBox("이 기능은 이중입고 & 입고장애 & 공출고 & 출고장애 에러 상황에서만 사용할 수 있습니다.");
		return;
	}

	CJobItem* pJobItem1 = NULL;
	CJobItem* pJobItem2 = NULL;
	int	nPattern = 0;
	int nUseFork = 0;
	if (m_pSc->m_pInfo->m_ScInfo[0].m_nInternalLuggNum != 0)
	{
		pJobItem1 = m_pSc->m_pDoc->m_pJob->Find(m_pSc->m_pInfo->m_ScInfo[0].m_nInternalLuggNum);

		if (pJobItem1 == NULL)
		{
			AfxMessageBox("내부 작업 번호를 찾을 수 없습니다. FORK #1 작업을 삭제 처리 하십시오.");
			return;
		}

		nUseFork |= 1;
		nPattern = pJobItem1->GetJobPattern();
		if ((nPattern == enJobPatternRet) || 
			(nPattern == enJobPatternMove) || 
			(nPattern == enJobPatternPR) )
		{
			AfxMessageBox("출고작업이나 피킹출고작업이나 이동작업에는 사용 할 수 없습니다..! FORK #1 확인 바람.");
			return;
		}
	}

	if (m_pSc->m_pInfo->m_ScInfo[1].m_nInternalLuggNum != 0)
	{
		pJobItem2 = m_pSc->m_pDoc->m_pJob->Find(m_pSc->m_pInfo->m_ScInfo[1].m_nInternalLuggNum);
		if (pJobItem2 == NULL)
		{
			AfxMessageBox("내부 작업 번호를 찾을 수 없습니다. FORK #2 작업을 삭제 처리 하십시오.");
			return;
		}

		nUseFork |= 2;
		nPattern = pJobItem2->GetJobPattern();
		if ((nPattern == enJobPatternRet) || 
			(nPattern == enJobPatternMove) || 
			(nPattern == enJobPatternPR) )
		{
			AfxMessageBox("출고작업이나 피킹출고작업이나 이동작업에는 사용 할 수 없습니다..! FORK #2 확인 바람.");
			return;
		}

	}


	CScPairRelocationDlg dlg(this);
	if (nUseFork == 3)
	{
		dlg.m_strLocation = pJobItem1->m_strDestLoc;
		dlg.m_strLocation2 = pJobItem2->m_strDestLoc;
//		pJobItem1->m_strDestLoc = dlg.m_strLocation;
//		pJobItem2->m_strDestLoc = dlg.m_strLocation2;
	}
	else if(nUseFork == 2)
	{
		dlg.m_strLocation2 = pJobItem2->m_strDestLoc;
//		pJobItem2->m_strDestLoc = dlg.m_strLocation2;
	}
	else if(nUseFork == 1)
	{
		dlg.m_strLocation = pJobItem1->m_strDestLoc;
//		pJobItem1->m_strDestLoc = dlg.m_strLocation;
	}
	else 
	{
		return;
	}

	if (dlg.DoModal() != IDOK)
		return;

	if (m_pSc->m_pInfo->m_ScInfo[0].m_nInternalLuggNum != 0)
	{
		if (!m_pSc->IsValidLocation(dlg.m_strLocation))
		{
			AfxMessageBox(m_pSc->m_strLog);
			return;
		}
	}

	if (m_pSc->m_pInfo->m_ScInfo[1].m_nInternalLuggNum != 0)
	{
		if (!m_pSc->IsValidLocation(dlg.m_strLocation2))
		{
			AfxMessageBox(m_pSc->m_strLog);
			return;
		}
	}
	
//	pJobItem->m_strDestLoc = dlg.m_strLocation;
//	pJobItem->SetJobStatus(enJobStatusDualStoreRetry);
//	m_pSc->m_pDoc->m_pJob->Backup();

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdNewLocation;
	pCmdMsg->m_strValues.SetSize(CScCmdMsg::enSizeScPairCommand);
	pCmdMsg->m_strValues[0].Format(_T("%d"), (pJobItem1 == NULL) ? 0 : pJobItem1->m_nLuggNum);
	pCmdMsg->m_strValues[1] = dlg.m_strLocation;
	pCmdMsg->m_strValues[2].Format(_T("%d"), (pJobItem2 == NULL) ? 0 : pJobItem2->m_nLuggNum);
	pCmdMsg->m_strValues[3] = dlg.m_strLocation2;
	m_pSc->SetCmdMsg(pCmdMsg);
	//*/
}

void CScPairDlg::OnButtonManual() 
{
	//CScPairManualDlg dlg(m_pSc, this);
	//dlg.DoModal();
}

void CScPairDlg::OnCheckSuspentStore() 
{
	//DEBUGER_ASSERT_VALID(m_pSc != NULL);
	//DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	//if (m_pSc->IsConnect() == FALSE)
	//{
	//	AfxMessageBox(_T("통신이 연결되지 않았습니다."));
	//	UpdateData(FALSE);
	//	return;
	//}

	//if (!UpdateData())
	//	return;

	//CScCmdMsg* pCmdMsg = new CScCmdMsg;
	//DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	//pCmdMsg->m_nCommand = CScCmdMsg::enCmdStoSuspend;
	//pCmdMsg->m_strValues.SetSize(1);
	//pCmdMsg->m_strValues[0].Format(_T("%d"), m_bStoSuspend);
	//m_pSc->SetCmdMsg(pCmdMsg);
}

void CScPairDlg::OnCheckSuspendRetrieve() 
{
	//DEBUGER_ASSERT_VALID(m_pSc != NULL);
	//DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	//if (m_pSc->IsConnect() == FALSE)
	//{
	//	AfxMessageBox(_T("통신이 연결되지 않았습니다."));
	//	UpdateData(FALSE);
	//	return;
	//}

	//if (!UpdateData())
	//	return;

	//CScCmdMsg* pCmdMsg = new CScCmdMsg;
	//DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	//pCmdMsg->m_nCommand = CScCmdMsg::enCmdRetSuspend;
	//pCmdMsg->m_strValues.SetSize(1);
	//pCmdMsg->m_strValues[0].Format(_T("%d"), m_bRetSuspend);
	//m_pSc->SetCmdMsg(pCmdMsg);
}
