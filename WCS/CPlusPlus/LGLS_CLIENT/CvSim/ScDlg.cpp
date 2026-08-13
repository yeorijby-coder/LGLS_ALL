// ScDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "ScDlg.h"
#include "Sc.h"
#include "ScManualDlg.h"
#include "ScRelocationDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScDlg dialog


CScDlg::CScDlg(CSc* pSc, CWnd* pParent /* = NULL */)
	: CDialog(CScDlg::IDD, pParent)
{
	m_pSc = pSc;
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);
	
	//{{AFX_DATA_INIT(CScDlg)
	m_bRetSuspend = m_pSc->m_pInfo->m_bRetSuspend;
	m_bStoSuspend = m_pSc->m_pInfo->m_bStoSuspend;
	//}}AFX_DATA_INIT
}


void CScDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScDlg)
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


BEGIN_MESSAGE_MAP(CScDlg, CDialog)
	//{{AFX_MSG_MAP(CScDlg)
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
	ON_BN_CLICKED(IDC_BTN_FIRE_ALARM, OnClickedFireAlarm)
	ON_BN_CLICKED(IDC_BTN_FIREALARM_CANCEL, OnClickedFirealarmCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScDlg message handlers

BOOL CScDlg::OnInitDialog() 
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

BOOL CScDlg::DestroyWindow() 
{
	m_pSc->m_pEquipDlg = NULL;
	
	return CDialog::DestroyWindow();
}

LRESULT CScDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_USER_REFRESH_NOTIFY:
		{
			CScInfo* pInfo = m_pSc->m_pInfo;
			DEBUGER_ASSERT_VALID(pInfo != NULL);

			SetDlgItemInt(IDC_EDIT_LUGG_NUM, pInfo->m_ScInfo[0].m_nLuggNum);
			SetDlgItemText(IDC_EDIT_CMD_TYPE, pInfo->GetCmdType());
			SetDlgItemText(IDC_EDIT_START_POS, pInfo->GetDepartInfoString());
			SetDlgItemText(IDC_EDIT_DEST_POS, pInfo->GetArriveInfoString());

			SetDlgItemText(IDC_EDIT_RC_MODE, pInfo->GetRcMode());
			SetDlgItemText(IDC_EDIT_RC_STATUS, pInfo->GetRcStatus());
//			SetDlgItemInt(IDC_EDIT_JOB_COUNT, pInfo->m_ucJobCount);
			SetDlgItemText(IDC_EDIT_SC_MODE, pInfo->GetScMode());
			SetDlgItemText(IDC_EDIT_SC_STATUS, pInfo->GetScStatus());
			SetDlgItemText(IDC_EDIT_FORK_POS, pInfo->GetForkPos());
			SetDlgItemText(IDC_EDIT_FORK_STATUS, pInfo->GetForkStatus());
			SetDlgItemText(IDC_EDIT_PROD_LOAD, pInfo->GetProdLoad());
			SetDlgItemText(IDC_EDIT_SC_JOB_STATUS, pInfo->GetJobStatus());
			SetDlgItemInt(IDC_EDIT_HORIZONTAL_POS, pInfo->m_wHorizontalPos);
			SetDlgItemInt(IDC_EDIT_VERTICAL_POS, pInfo->m_wVerticalPos);
//			SetDlgItemInt(IDC_EDIT_COMPLETE_NUM, pInfo->m_nCompleteNum);

			CJobItem* pJobItem = m_pSc->m_pDoc->m_pJob->Find(pInfo->m_ScInfo[0].m_nInternalLuggNum);
			SetDlgItemInt(IDC_EDIT_INTERNAL_LUGG_NUM, pInfo->m_ScInfo[0].m_nInternalLuggNum);
			SetDlgItemText(IDC_EDIT_ECS_JOB_TYPE, CJobItem::GetJobTypeString(pInfo->m_nInternalJobType));
			SetDlgItemText(IDC_EDIT_JOB_START_POS, pJobItem ? pJobItem->GetStartStationString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_DEST_POS, pJobItem ? pJobItem->GetDestStationString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_STATUS, pJobItem ? pJobItem->GetJobStatusString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_PROD_INFO, pJobItem ? pJobItem->m_strProductID : _T(""));
//			SetDlgItemInt(IDC_EDIT_JOB_PROD_INFO2, pJobItem ? pJobItem->m_nProdCount: 0);
//			SetDlgItemText(IDC_EDIT_JOB_PROD_INFO3, pJobItem ? pJobItem->m_strProductID : _T(""));	

			SetDlgItemText(IDC_EDIT_DIAGNOSIS, m_pSc->m_pInfo->GetErrorString());
		}
		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////

void CScDlg::OnButtonComplete() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect(0))
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if (m_pSc->m_pInfo->IsInvoked() == FALSE)
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 현재 S/C이 수행 중인 작업이 없습니다!"));
		return;
	}

	if (m_pSc->m_pInfo->IsWaitStatus() == FALSE)
	{
		AfxMessageBox(_T("SC 상태 확인 바람..  현재 SC는 대기 상태가 아닙니다!"));
		return;
	}

	if (m_pSc->m_pInfo->IsProductEmpty() == FALSE)
	{
		AfxMessageBox(_T("SC 상태 확인 바람..  현재 SC에 화물이 적재된 상태 입니다!"));
		return;
	}

	if (AfxMessageBox(_T("현 작업을 강제완료 하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdComplete;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnButtonEmergency() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect(0))
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("비상정지를 지시하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdEmergency;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnButtonActive() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect(0))
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("ACTIVE 상태로 변경하시겠습니까?\n\n현장에 사람이 없는지 확인후 진행해주세요."), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdActive;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnButtonStop() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect(0))
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("STOP 상태로 변경하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdStop;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnButtonErrorReset() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect(0))
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if (m_pSc->m_pInfo->IsDualStoreError() == TRUE)
	{
		if (AfxMessageBox(_T("이중입고 에러일때는 에러를 해제하면 S/C을 수동 및 반자동 조작해야 할수도 있습니다.\n정말로 에러를 해제 하시겠습니까?"), MB_YESNO) == IDNO)
		{
			return;
		}
	}

	if (AfxMessageBox(_T("에러를 RESET 하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdReset;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnButtonCancel() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect(0))
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("작업을 삭제 하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdCancel;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnButtonCallToHome() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect(0))
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if (m_pSc->m_pInfo->IsInvoked())
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 현재 S/C이 작업을 수행 중입니다!"));
		return;
	}

	if (m_pSc->m_pInfo->IsWaitStatus() == FALSE)
	{
		AfxMessageBox(_T("SC 상태 확인 바람..  현재 SC는 대기 상태가 아닙니다!"));
		return;
	}

	if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("홈복귀를 지시하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdHomeReturn;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnButtonDualStore() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pDoc->m_pJob != NULL);

	if (m_pSc->m_pInfo->IsDualStoreError() == FALSE)
	{
		AfxMessageBox(_T("이 기능은 이중입고 & 입고장애 에러 상황에서만 사용할 수 있습니다."));
		return;
	}

	CJobItem* pJobItem = m_pSc->m_pDoc->m_pJob->Find(m_pSc->m_pInfo->m_ScInfo[0].m_nInternalLuggNum);
	if (pJobItem == NULL)
	{
		AfxMessageBox(_T("내부 작업 번호를 찾을 수 없습니다. 작업을 삭제 처리 하십시오."));
		return;
	}

	int	nPattern = pJobItem->GetJobPattern();
	if ((nPattern == enJobPatternRet) || 
		(nPattern == enJobPatternMove) || 
		(nPattern == enJobPatternPR) )
	{
		AfxMessageBox(_T("출고작업이나 이동작업에는 사용할 수 없습니다. 확인 바람."));
		return;
	}

	CScRelocationDlg dlg(this);
	dlg.m_strLocation = pJobItem->m_strDestLoc;
	if (dlg.DoModal() != IDOK)
		return;

	if (!m_pSc->IsValidLocation(dlg.m_strLocation))
	{
		AfxMessageBox(m_pSc->m_strLog);
		return;
	}

//	pJobItem->m_strDestLoc = dlg.m_strLocation;
//	pJobItem->SetJobStatus(enJobStatusDualStoreRetry);
//	m_pSc->m_pDoc->m_pJob->Backup();

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdNewLocation;
	pCmdMsg->m_strValues.SetSize(CScCmdMsg::enSizeScCommand);
	pCmdMsg->m_strValues[0].Format(_T("%d"), pJobItem->m_nLuggNum);
	pCmdMsg->m_strValues[1] = dlg.m_strLocation;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnButtonManual() 
{
	CScManualDlg dlg(m_pSc, this);
	dlg.DoModal();
}

void CScDlg::OnCheckSuspentStore() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (m_pSc->IsConnect(0) == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		UpdateData(FALSE);
		return;
	}

	if (!UpdateData())
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdStoSuspend;
	pCmdMsg->m_strValues.SetSize(1);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_bStoSuspend);
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnCheckSuspendRetrieve() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (m_pSc->IsConnect(0) == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		UpdateData(FALSE);
		return;
	}

	if (!UpdateData())
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdRetSuspend;
	pCmdMsg->m_strValues.SetSize(1);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_bRetSuspend);
	m_pSc->SetCmdMsg(pCmdMsg);
}

//=============================================================================
//	Desc	: 화재발생 On
//	Date	:
//	Update	: 
//=============================================================================
void CScDlg::OnClickedFireAlarm() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pInfo != NULL);

	if (!m_pSc->IsConnect(0))
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pSc->m_pInfo->IsOnlineMode() == FALSE) || (m_pSc->m_pInfo->IsAutoMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. S/C 모드가 ONLINE 자동 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("화재발생 수동 지시를 하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdFireAlarmOn;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDlg::OnClickedFirealarmCancel() 
{
	m_pSc->FireAlarmOff();
}
