// ScDongJinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "ScDongJinDlg.h"
#include "ScDongJin.h"
#include "ScDongJinManualDlg.h"
#include "ScRelocationDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScDongJinDlg dialog

CScDongJinDlg::CScDongJinDlg(CWnd* pParent /* = NULL */)
	: CDialog(CScDongJinDlg::IDD, pParent)
{
	m_pSc = NULL;
	m_nEquipNum = 0;
	m_pScInfo = NULL;
	
	//{{AFX_DATA_INIT(CScDongJinDlg)
	m_bRetSuspend = FALSE;
	m_bStoSuspend = FALSE;
	m_bR2RSuspend = FALSE;
	//}}AFX_DATA_INIT
}


CScDongJinDlg::CScDongJinDlg(CScDongJin* pSc, int nEquipNum, CWnd* pParent /* = NULL */)
	: CDialog(CScDongJinDlg::IDD, pParent)
{
	m_pSc = pSc;
	m_nEquipNum = pSc->m_nNumber;
	DEBUGER_ASSERT_VALID(m_pSc != NULL);

	m_pScInfo = (CScInfo *)m_pSc->GetInfo(nEquipNum, FALSE);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);
	
	//{{AFX_DATA_INIT(CScDongJinDlg)
	m_bRetSuspend = m_pScInfo->m_bRetSuspend;
	m_bStoSuspend = m_pScInfo->m_bStoSuspend;
	m_bR2RSuspend = m_pScInfo->m_bR2RSuspend;
	//}}AFX_DATA_INIT
}


void CScDongJinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScDongJinDlg)
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
	DDX_Check(pDX, IDC_CHECK_SUSPEND_RACKTORACK, m_bR2RSuspend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScDongJinDlg, CDialog)
	//{{AFX_MSG_MAP(CScDongJinDlg)
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
	ON_BN_CLICKED(IDC_CHECK_SUSPEND_RACKTORACK, OnCheckSuspendRacktorack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScDongJinDlg message handlers

BOOL CScDongJinDlg::OnInitDialog() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	m_pSc->m_pEquipDlg = this;

	CDialog::OnInitDialog();

	CString strTemp;
	strTemp.Format(_T("%s [S/C #%d]"), m_pSc->m_port.m_strDevice, m_pScInfo->m_nEquipNum);
	SetDlgItemText(IDC_EDIT_DEVICE, strTemp);

	/////////////////////////////////////////////////////////////////////////

	
	int w = 16, h = 16;
	INIT_BUTTON(m_btnComplete, IDI_ICON_COMPLETE, w, h, _T("강제완료"));
	INIT_BUTTON(m_btnEmergency, IDI_ICON_ESTOP, w, h, _T("강제완료"));
	INIT_BUTTON(m_btnStop, IDI_ICON_LEDOFF, w, h, _T("정지지시"));
//	INIT_BUTTON(m_btnActive, IDI_ICON_LEDON, w, h, "ACTIVE 지시");

//	if (m_pScInfo->m_nCycleType == CScInfo::enFullCycle)
//		strTemp = "ACTIVE 지시";
//	else
//		strTemp = "StartUp Req.";

	INIT_BUTTON(m_btnActive, IDI_ICON_LEDON, w, h, strTemp);
	INIT_BUTTON(m_btnReset, IDI_ICON_RETURN, w, h, _T("에러 리셋"));

//	if (m_pScInfo->IsDualStoreError() == TRUE)
//	{
//		ENABLE_WND(IDC_BUTTON_ERROR_RESET, FALSE)
//	}

	INIT_BUTTON(m_btnDelete, IDI_CANCEL, w, h, _T("작업삭제"));
	INIT_BUTTON(m_btnHome, IDI_ICON_HOME, w, h, _T("홈복귀"));
	INIT_BUTTON(m_btnDualStore, IDI_RUN, w, h, _T("이중입고재지정"));
	INIT_BUTTON(m_btnManual, IDI_ICON_MANUAL, w, h, _T("수동지시"));
	INIT_BUTTON(m_btnOK, IDI_EXIT, w, h, _T("창닫기"));
	

	WindowProc(WM_USER_REFRESH_NOTIFY, 0, 0);
	
//	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CScDongJinDlg::DestroyWindow() 
{
	m_pSc->m_pEquipDlg = NULL;
	
	return CDialog::DestroyWindow();
}

LRESULT CScDongJinDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_USER_REFRESH_NOTIFY:
		{
			CScInfo* pInfo = m_pScInfo;
			DEBUGER_ASSERT_VALID(pInfo != NULL);

			SetDlgItemInt(IDC_EDIT_LUGG_NUM, pInfo->m_ScInfo[0].m_nInternalLuggNum);
			SetDlgItemText(IDC_EDIT_CMD_TYPE, CJobItem::GetJobTypeString(pInfo->m_ScInfo[0].m_nInternalJobType));
			SetDlgItemText(IDC_EDIT_START_POS, pInfo->GetDepartInfoString());
			SetDlgItemText(IDC_EDIT_DEST_POS, pInfo->GetArriveInfoString());

			CString strRcMode, strScStatus, strForkPos, strHoist, strRcStatus;
			strRcMode = pInfo->IsScJobStatusScManualMode() ? _T("수동") : (pInfo->IsScJobStatusScAutoMode() ? _T("자동") : (pInfo->IsScJobStatusScRemoteMode() ? _T("원격") : _T("")) );	// (pInfo->IsScJobStatusRemoteMode() ? "REMOTE" : "LOCAL")
			strRcStatus = pInfo->IsScJobStatusRcManualMode() ? _T("수동") : (pInfo->IsScJobStatusRcAutoMode() ? _T("자동") : (pInfo->IsScJobStatusRcRemoteMode() ? _T("원격") : _T("")) );	// (pInfo->IsScJobStatusRemoteMode() ? "REMOTE" : "LOCAL")
			strScStatus = pInfo->IsScJobStatusIdle() ? _T("대기") : (pInfo->IsScJobStatusError() ? _T("이상") : _T("운전") ); // @@
			strForkPos = pInfo->IsScJobStatusForkCenter() ? _T("CENTER") : (pInfo->IsScJobStatusForkLeft() ? _T("LEFT") : (pInfo->IsScJobStatusForkRight() ? _T("RIGHT") : _T("")));
			strHoist = pInfo->IsScJobStatusHoistUp() ? _T("UP") : (pInfo->IsScJobStatusHoistDown() ? _T("DOWN") : _T(""));

			SetDlgItemText(IDC_EDIT_RC_MODE,		strRcStatus);													// 지상반	- D100.E
//			SetDlgItemText(IDC_EDIT_RC_STATUS,		(pInfo->m_bBufferEmpty == TRUE) ? "지워짐" : "지령됨" );			// Active			- D100.D
			SetDlgItemText(IDC_EDIT_SC_MODE,		strRcMode);														// 기상반			- D100.B, D100.9
			SetDlgItemText(IDC_EDIT_SC_STATUS,		pInfo->IsScJobStatusIdle() ? _T("대기") : _T("운전"));													// 크래인 상태		- D100.0, D100.1, D100.E
			SetDlgItemText(IDC_EDIT_FORK_POS,		strForkPos);													// 포크위치			- D100.5, D100.6, D100.7
			SetDlgItemText(IDC_EDIT_FORK_STATUS,	pInfo->IsScJobStatusRegPos() ? _T("주행 정위치") : _T(""));				// 주행 정위치		- D100.2							
			SetDlgItemText(IDC_EDIT_COMPLETE_NUM,	strHoist);														// HOIST			- D100.3, D100.4
			SetDlgItemText(IDC_EDIT_PROD_LOAD,		pInfo->IsScJobStatusSensorProduct() ? _T("화물있음") : _T("화물없음"));	// 화물유무			- D100.8
			SetDlgItemText(IDC_EDIT_SC_JOB_STATUS,	pInfo->IsScJobStatusJobComplete() ? _T("작업완료") : _T(""));			// 완료상태			- D100.F
			SetDlgItemInt(IDC_EDIT_HORIZONTAL_POS,	pInfo->m_wHorizontalPos);										// 주행				- D102
			SetDlgItemInt(IDC_EDIT_VERTICAL_POS,	pInfo->m_wVerticalPos);											// 승강				- D103
			SetDlgItemText(IDC_EDIT_RC_STATUS,		pInfo->IsScDoorStatusOpen() ? _T("열  림") : _T("닫  힘"));				// 펜스상태			- D111.0

			CJobItem* pJobItem = m_pSc->m_pDoc->m_pJob->Find(pInfo->m_ScInfo[0].m_nInternalLuggNum);
			SetDlgItemInt(IDC_EDIT_INTERNAL_LUGG_NUM, pInfo->m_ScInfo[0].m_nInternalLuggNum);
			SetDlgItemText(IDC_EDIT_ECS_JOB_TYPE, CJobItem::GetJobTypeString(pInfo->m_ScInfo[0].m_nInternalJobType));
			SetDlgItemText(IDC_EDIT_JOB_START_POS, pJobItem ? pJobItem->GetStartStationString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_DEST_POS, pJobItem ? pJobItem->GetDestStationString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_STATUS, pJobItem ? pJobItem->GetJobStatusString() : _T(""));
			SetDlgItemText(IDC_EDIT_JOB_PROD_INFO, pJobItem ? pJobItem->m_strPalletNo : _T(""));
//			SetDlgItemInt(IDC_EDIT_JOB_PROD_INFO2, pJobItem ? pJobItem->m_nProdCount: 0);
			SetDlgItemText(IDC_EDIT_JOB_PROD_INFO3, pJobItem ? pJobItem->m_strProductID : _T(""));	

			SetDlgItemText(IDC_EDIT_DIAGNOSIS, m_pScInfo->GetErrorString());
		}
		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////

void CScDongJinDlg::OnButtonComplete() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (!m_pSc->IsConnect())
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

//	if (m_pScInfo->IsInvoked() == FALSE)
//	{
//		AfxMessageBox("S/C 상태 확인 바람.. 현재 S/C이 수행 중인 작업이 없습니다!");
//		return;
//	}

//	if (m_pScInfo->IsScJobStatusIdle() == FALSE)
//	{
//		AfxMessageBox("SC 상태 확인 바람..  현재 SC는 대기 상태가 아닙니다!");
//		return;
//	}

	if (m_pScInfo->IsScJobStatusSensorProduct() == TRUE)
	{
		AfxMessageBox(_T("SC 상태 확인 바람..  현재 SC에 화물이 적재된 상태 입니다!"));
		return;
	}

	if (AfxMessageBox(_T("현 작업을 강제완료 하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdComplete;
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	m_pSc->SetCmdMsg(pCmdMsg);
}


// 재지시 버튼으로 수정
void CScDongJinDlg::OnButtonEmergency() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (!m_pSc->IsConnect())
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pScInfo->IsScJobStatusScRemoteMode() == FALSE) || (m_pScInfo->IsScJobStatusRcRemoteMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 지상반과 기상반 모드가 원격 상태가 아닙니다!"));
		return;
	}

	if ((m_pScInfo->IsScJobStatusIdle() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 대기 상태가 아닙니다!"));
		return;
	}

	if ((m_pScInfo->m_ScInfo[0].m_nInternalLuggNum == 0))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 작업번호가 없습니다."));
		return;
	}

	CJobItem* pJobItem = m_pSc->m_pDoc->m_pJob->Find(m_pScInfo->m_ScInfo[0].m_nInternalLuggNum);
	if (pJobItem == NULL)
	{
		AfxMessageBox(_T("작업정보 확인바람... 작업정보가 없습니다."));
		return;
	}

	if (AfxMessageBox(_T("재지시하시겠습니까?"), MB_YESNO) == IDNO)
		return;

//	CScCmdMsg* pCmdMsg = new CScCmdMsg;
//	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
//	pCmdMsg->m_nCommand = CScCmdMsg::enCmdEmergency;
//	pCmdMsg->m_nCommand = CScCmdMsg::enCmdStartUp;
//	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
//	m_pSc->SetCmdMsg(pCmdMsg);

	m_pSc->StartUpRequest(m_pScInfo);
}

void CScDongJinDlg::OnButtonActive() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (!m_pSc->IsConnect())
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pScInfo->IsScJobStatusScRemoteMode() == FALSE) || (m_pScInfo->IsScJobStatusRcRemoteMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 지상반과 기상반 모드가 원격 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("ACTIVE 상태로 변경하시겠습니까?\n\n현장에 사람이 없는지 확인후 진행해주세요."), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdActive;
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDongJinDlg::OnButtonStop() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (!m_pSc->IsConnect())
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pScInfo->IsScJobStatusScRemoteMode() == FALSE) || (m_pScInfo->IsScJobStatusRcRemoteMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 지상반과 기상반 모드가 원격 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("STOP 상태로 변경하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdStop;
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDongJinDlg::OnButtonErrorReset() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (!m_pSc->IsConnect())
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if (m_pScInfo->IsDualStoreError() == TRUE)
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
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDongJinDlg::OnButtonCancel() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (!m_pSc->IsConnect())
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if ((m_pScInfo->IsScJobStatusScRemoteMode() == FALSE) || (m_pScInfo->IsScJobStatusRcRemoteMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 지상반과 기상반 모드가 원격 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("작업을 삭제 하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdCancel;
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDongJinDlg::OnButtonCallToHome() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (!m_pSc->IsConnect())
	{
		AfxMessageBox(_T("통신이 연결되지 않아 명령을 수행할 수 없습니다."));
		return;
	}

	if (m_pScInfo->IsInvoked())
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 현재 S/C이 작업을 수행 중입니다!"));
		return;
	}

	if (m_pScInfo->IsScJobStatusIdle() == FALSE)
	{
		AfxMessageBox(_T("SC 상태 확인 바람..  현재 SC는 대기 상태가 아닙니다!"));
		return;
	}

	if ((m_pScInfo->IsScJobStatusScRemoteMode() == FALSE) || (m_pScInfo->IsScJobStatusRcRemoteMode() == FALSE))
	{
		AfxMessageBox(_T("S/C 상태 확인 바람.. 지상반과 기상반 모드가 원격 상태가 아닙니다!"));
		return;
	}

	if (AfxMessageBox(_T("홈복귀를 지시하시겠습니까?"), MB_YESNO) == IDNO)
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdHomeReturn;
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDongJinDlg::OnButtonDualStore() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pSc->m_pDoc->m_pJob != NULL);

	if (m_pScInfo->IsDualStoreError() == FALSE)
	{
		AfxMessageBox(_T("이 기능은 이중입고 & 입고장애 에러 상황에서만 사용할 수 있습니다."));
		return;
	}

	CJobItem* pJobItem = m_pSc->m_pDoc->m_pJob->Find(m_pScInfo->m_ScInfo[0].m_nInternalLuggNum);
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
		AfxMessageBox(_T("출고작업이나 피킹출고작업이나 이동작업에는 사용 할 수 없습니다..! 확인 바람."));
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

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdNewLocation;
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	pCmdMsg->m_strValues.SetSize(CScCmdMsg::enSizeScCommand);
	pCmdMsg->m_strValues[0].Format(_T("%d"), pJobItem->m_nLuggNum);
	pCmdMsg->m_strValues[1] = dlg.m_strLocation;
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDongJinDlg::OnButtonManual() 
{
	CScDongJinManualDlg dlg(m_pSc, m_pScInfo->m_nEquipNum, this);
	dlg.DoModal();
}

void CScDongJinDlg::OnCheckSuspentStore() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (m_pSc->IsConnect() == FALSE)
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
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	pCmdMsg->m_strValues.SetSize(1);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_bStoSuspend);
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDongJinDlg::OnCheckSuspendRetrieve() 
{
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (m_pSc->IsConnect() == FALSE)
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
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	pCmdMsg->m_strValues.SetSize(1);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_bRetSuspend);
	m_pSc->SetCmdMsg(pCmdMsg);
}

void CScDongJinDlg::OnCheckSuspendRacktorack() 
{
	// TODO: Add your control notification handler code here
	DEBUGER_ASSERT_VALID(m_pSc != NULL);
	DEBUGER_ASSERT_VALID(m_pScInfo != NULL);

	if (m_pSc->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		UpdateData(FALSE);
		return;
	}

	if (!UpdateData())
		return;

	CScCmdMsg* pCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CScCmdMsg::enCmdR2RSuspend;
	pCmdMsg->m_nSubCmd = m_pScInfo->m_nEquipNum;
	pCmdMsg->m_strValues.SetSize(1);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_bR2RSuspend);
	m_pSc->SetCmdMsg(pCmdMsg);
	
}
