// EcsDoc.cpp : implementation of the CEcsDoc class
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "EcsView.h"
#include "MainFrm.h"
//#include "UserManagerDlg.h"

#include "ConfigPptSheet.h"
#include "JobOfflineDlg.h"
#include "JobTestDlg.h"
#include "PasswordDlg.h"

#define DEF_LOGIN_SKIP

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc

IMPLEMENT_DYNCREATE(CEcsDoc, CDocument)

BEGIN_MESSAGE_MAP(CEcsDoc, CDocument)
	//{{AFX_MSG_MAP(CEcsDoc)
	ON_COMMAND_EX(ID_COM_HOST_SERVER, OnComStatusHost)
	ON_COMMAND_EX(ID_COM_CV01, OnComStatusClient)
	ON_COMMAND(ID_SYSTEM_CONFIG1, OnSystemConfiguration1)
	ON_COMMAND(ID_SYSTEM_CONFIG2, OnSystemConfiguration2)
	ON_COMMAND(ID_SYSTEM_CONFIG3, OnSystemConfiguration3)
	ON_COMMAND(ID_SYSTEM_CONFIG4, OnSystemConfiguration4)
	ON_COMMAND(ID_JOB_OFFLINE, OnJobOffline)
	ON_COMMAND(ID_JOB_OFFLINE1, OnJobOffline1)
	ON_COMMAND(ID_JOB_OFFLINE2, OnJobOffline2)
	ON_COMMAND(ID_JOB_MANAGER, OnJobManager)
	ON_COMMAND(ID_LOG_ALL, OnLogAll)
	ON_COMMAND(ID_RACK_CONFIG, OnRackConfig)
	ON_COMMAND_EX(ID_COM_HOST_CLIENT, OnComStatusHost)
	ON_COMMAND_EX(ID_COM_CV02, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV03, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV04, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV05, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV06, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV07, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV08, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV09, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV10, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV11, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_CV12, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_SC1_W1, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_SC2_W1, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_SC3_W1, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_SC4_W1, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_SC5_W1, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_SC6_W1, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_RTV1, OnComStatusClient)
	ON_COMMAND_EX(ID_COM_RTV2, OnComStatusClient)
	ON_COMMAND(ID_WARNING, OnWarning)
	ON_COMMAND(ID_SYS_USER_MANAGER, OnSysUserManager)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc construction/destruction

CEcsDoc::CEcsDoc()
{
	m_pConfig	= NULL;
	m_pLog		= NULL;
	m_pJob		= NULL;

	m_pHostListener = NULL;
	m_pHostSv		= NULL;
	m_pHostCl		= NULL;

//	m_pMonitorListener	= NULL;

	m_hWndView		= NULL;
	m_pJobDlg		= NULL;
	m_pLogDlg		= NULL;
	m_pWarningDlg	= NULL;

	m_bTesting		= FALSE;

	m_wSc1Status910 = 0x0000;
	m_wSc1Status911 = 0x0000;
	m_wSc1Status912 = 0x0000;
	m_wSc2Status910 = 0x0000;
	m_wSc2Status911 = 0x0000;
	m_wSc1Status920 = 0x0000;
	m_wSc2Status921 = 0x0000;
	m_wSc3Status922 = 0x0000;

	m_bLogPlayback  = FALSE;

	m_nBank = 0;
	m_nBay = 0;
	m_nLevel = 0;
	
	m_nPairLuggNum = 0;

	m_bCommStatus  = FALSE;

	m_bProdInfo = FALSE;
	m_nProdAddr = 0;
	m_nProdDate = 0;
	m_nProdCount = 0;
	m_nProdPlc = 0;

	m_bLog = FALSE;

	int i = 0 ;
	for (i = 0 ;  i < SC1_PLC_CNT ; i++)
	{
		m_nScRetFrontRemainJobCnt[i]= 0; 
//		m_bScRetFrontCountChanged[i] = FALSE;
	}

	for (i = 0 ; i < SC_PLC_TOT_CNT ; i++)
	{
		m_nScRetRearRemainJobCnt[i]	= 0;
//		m_bScRetRearCountChanged[i] = FALSE;
	}

	for (i = 0 ; i < 10000 ; i++)
	{
		m_strDepartPos[i] = _T("");
		m_strArrivePos[i] = _T("");
		m_strWeightVal[i] = _T("");
		m_strProductID[i] = _T("");

		m_strStoLocation[i] = _T("");
		m_strRetLocation[i] = _T("");
	}	

	for (i = 0 ; i < SC_CNT ; i++)
	{
		m_nScStatus[i] = 0;
	}

	for (i = 0 ; i < STO_STN_CNT ; i++)
	{
		m_bStoStation[i] = 0;
	}

	/*
	for (i = 0 ; i < STO_STN_CNT ; i++)
	{
		m_JobInvokeInfo[i].m_strStoStation = "";
		m_JobInvokeInfo[i].m_strRetStation = "";

		m_JobInvokeInfo[i].m_strLocation = "";

		m_JobInvokeInfo[i].m_nWorkingLuggNum = 0;		// 현재 작업중인 작업번호 
		m_JobInvokeInfo[i].m_bCompleteStore = FALSE;	// 입고작업완료 
		m_JobInvokeInfo[i].m_nPrevLuggNum = 0;			// PRE 작업번호 
		m_JobInvokeInfo[i].m_strAlterLocation = "";		// (이중입고 에러시)재지정 할 Location;
		m_JobInvokeInfo[i].m_nDualStoScNo = 0;
		m_JobInvokeInfo[i].m_strFromPos = "";			// 기타 작업할 포지션 
		m_JobInvokeInfo[i].m_strToPos = "";				// 기타 작업할 포지션 
	}
	//*/
	m_nWorkingLuggNum1 = 0;
	m_nWorkingLuggNum2 = 0;
	m_bCompleteStore1 = FALSE;
	m_bCompleteStore2 = FALSE;
	m_nPrevLuggNum = 0;
//	m_nPrevLuggNum2 = 0;
	m_strAlterLocation = "";
	m_nDualStoScNo = 0;
	m_bReceiveStatus = FALSE;
}

CEcsDoc::~CEcsDoc()
{
	if (m_pWarningDlg != NULL) delete m_pWarningDlg;
	if (m_pLogDlg != NULL) delete m_pLogDlg;
	if (m_pJobDlg != NULL) delete m_pJobDlg;

	if (m_pHostCl != NULL) delete m_pHostCl;
	if (m_pHostSv != NULL) delete m_pHostSv;
	if (m_pHostListener != NULL) delete m_pHostListener;
//	if (m_pMonitorListener != NULL) delete m_pMonitorListener;

	if (m_pJob) delete m_pJob;
	if (m_pLog) delete m_pLog;
	if (m_pConfig) delete m_pConfig;
//	if (m_pLogPlayback) delete m_pLogPlayback;

	for (POSITION pos=m_MsgList.GetHeadPosition(); pos!=NULL; )
		delete (CStatusBarMsg*)m_MsgList.GetNext(pos);
	m_MsgList.RemoveAll();

	int i = 0;
	for (i=0; i<m_pStationInfos.GetSize(); ++i)
		delete m_pStationInfos[i];
	m_pStationInfos.RemoveAll();

	for (i=0; i<m_pTrackInfos.GetSize(); ++i)
		delete m_pTrackInfos[i];
	m_pTrackInfos.RemoveAll();

	//@@
	for (i=0; i<m_pEquipments.GetSize(); ++i)
		delete m_pEquipments[i];
	m_pEquipments.RemoveAll();
	  
	for (i = 0; i < m_pLogicGorupInfos.GetSize(); ++i)
	{
		int nSize = m_pLogicGorupInfos[i]->m_pJobInvokeInfos.GetSize();
		for(int j=0;j< nSize; ++j)
			delete m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j];

		m_pLogicGorupInfos[i]->m_pJobInvokeInfos.RemoveAll();
		delete m_pLogicGorupInfos[i];
	}
	m_pLogicGorupInfos.RemoveAll();
	
} 

BOOL CEcsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
 
	return Initialize();
}

BOOL CEcsDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	if (AfxMessageBox(_T("종료 하시 겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return FALSE;

	int j = 0;
	int nScCount = 0;
	int nRtvCount = 0;
	for (int i=0; i<m_pEquipments.GetSize(); ++i)
	{
		m_pEquipments[i]->Disconnect();
//		m_pEquipments[i]->GetInfo()->Backup();

		// 수정요망
		if (m_pEquipments[i]->m_enKind == CEquipment::enSC )
		{
			CScDongJin* pSc = (CScDongJin*)m_pEquipments[i];

			if (pSc == NULL)
				continue;

			if (m_pEquipments[i]->m_nNumber == 1)
			{
				j = 1 ;								// 1
				nScCount = SC1_PLC_CNT;				// 3	
			}
			else
			{
				j = SC1_PLC_CNT + 1 ;				// 4
				nScCount = SC_PLC_TOT_CNT;			// 6	
			}

			for ( ;j <= nScCount ; j++)
			{
				CScInfo* pInfo =  (CScInfo*)pSc->GetInfo(j, TRUE);

				if (pInfo == NULL)
					continue;

				pInfo->Backup(j);
			}
		}
		/*
		else if (m_pEquipments[i]->m_enKind == CEquipment::enRGV )
		{
			if (m_pEquipments[i]->m_nNumber == 1)
			{
				j = 1 ;								// 1
				nRtvCount = RTV1_PLC_CNT;			// 5	
			}
			else
			{
				j = RTV1_PLC_CNT + 1 ;				// 6
				nRtvCount = RTV_PLC_TOT_CNT;		// 13	
			}

			for (; j <= nRtvCount ; j++)
			{
				CLgvInfo* pInfo =  (CLgvInfo*)m_pEquipments[i]->GetInfo();

				if (pInfo == NULL)
					continue;

				pInfo->Restore();
			}
		}
		*/
		else
		{
			m_pEquipments[i]->GetInfo()->Backup();
		}

	}

	//m_pJob->Backup();	// 하지마

	WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, _T("ECS 종료... (Equipment Control System)"), _T("CEcsDoc::CanCloseFrame"));
	
	return CDocument::CanCloseFrame(pFrame);
}

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc serialization

void CEcsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc diagnostics

#ifdef _DEBUG
void CEcsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEcsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CEcsDoc::Initialize()
{
//	AfxSetAllocStop(72026);
	GetViewHandle();

	m_strLocation = "0600101"; 
	m_strStoStation = "104";
	m_strRetStation = "106";

	m_nMaxBank = 10;
	m_nMaxBay = 2;			// 36;
	m_nMaxLevel = 2;		// 16;

/*
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!m_layout.LoadXml(ECS_LAYOUT_FILE))
	{
		AfxMessageBox(m_layout.m_strLog);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CEcsDefine define(this);
	if (!define.ParseXml())
	{
		AfxMessageBox(define.GetErrorMessage());
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
//*/
	m_pConfig = new CConfig(this);
	DEBUGER_ASSERT_VALID(m_pConfig != NULL);

	m_pLog = new CLog(this);
	DEBUGER_ASSERT_VALID(m_pLog != NULL);

	m_pJob = new CJob(this);
	DEBUGER_ASSERT_VALID(m_pJob != NULL);

//	if (m_pJob->Restore(0) == FALSE)
//	{
//		if (AfxMessageBox(_T("JOB.ECS 파일에 오류가 있어 작업정보 복원을 실패하였습니다. \nJOB.BKP 파일로 작업정보를 생성시키겠습니까?"), MB_YESNO) == IDYES)
//		{
//			if (m_pJob->Restore(1) == FALSE)
//			{
//				if (AfxMessageBox(_T("JOB.BKP 파일에도 오류가 있어 작업정보 복원을 실패하였습니다. \n작업정보 전체를 삭제 하시겠습니까?"), MB_YESNO) == IDYES)
//				{
//					// 작업정보 파일 삭제!!
//					AfxMessageBox(_T("작업정보 복원에 실패(사용자 전체 삭제)하였으므로 \nWMS에서 제어관리 -> 작업개시 및 종료 메뉴에서\n[진행중인 작업 ECS에 재지시 버튼]을 누르세요!"));
//					WriteLog(LOG_TYPE_DEBUG, LOG_POS_SYSTEM, _T("JOB.BKP 파일에도 오류가 있어 작업정보 전체를 삭제하고 실행합니다."), _T("CEcsDoc::Initialize"));
//				}
//				else
//				{
//					AfxMessageBox(_T("작업정보 복원에 실패(사용자 취소)하였으므로 \nWMS에서 제어관리 -> 작업개시 및 종료 메뉴에서\n[진행중인 작업 ECS에 재지시 버튼]을 누르세요!"));
//				}
//			}
//			else
//			{
//				WriteLog(LOG_TYPE_DEBUG, LOG_POS_SYSTEM, _T("JOB.ECS 파일에 오류가 있어 JOB.BKP 파일로 작업정보를 정상적으로 복원하였습니다."), _T("CEcsDoc::Initialize"));
//			}
//		}
//		else
//		{
//			AfxMessageBox(_T("작업정보 복원에 실패(사용자 취소)하였으므로 \nWMS에서 제어관리 -> 작업개시 및 종료 메뉴에서\n[진행중인 작업 ECS에 재지시 버튼]을 누르세요!"));
//		}
//	}
//	else
//	{
//		WriteLog(LOG_TYPE_DEBUG, LOG_POS_SYSTEM, _T("JOB.ECS 파일로 작업정보를 정상적으로 복원하였습니다."), _T("CEcsDoc::Initialize"));
//	}
//
//	WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, _T("ECS 시작... (Equipment Control System)"), _T("CEcsDoc::Initialize"));

	return TRUE;
}

void CEcsDoc::RunServers()
{
	DEBUGER_ASSERT_VALID(m_pConfig != NULL);
	DEBUGER_ASSERT_VALID(m_pHostListener == NULL);
//	DEBUGER_ASSERT_VALID(m_pMonitorListener == NULL);
	CString strLog;

	//////////////////////////////////////////////////////////////////////////////////////////

	m_pHostListener = new CListenSk(this);
	DEBUGER_ASSERT_VALID(m_pHostListener != NULL);

	if (!m_pHostListener->Create(m_pConfig->m_nEcsPort))
	{
		strLog.Format(_T("ECS HOST SERVER 소켓생성 실패! [%s]"), CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CEcsDoc::RunServers"), NULL, TRUE);
	}
	else
	{
		m_pHostListener->Listen();
		strLog.Format(_T("ECS HOST SERVER 실행! [PORT=%d]"), m_pConfig->m_nEcsPort);
		WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, strLog, _T("CEcsDoc::RunServers"));
	}

	//////////////////////////////////////////////////////////////////////////////////////////

//	m_pMonitorListener = new CMonitorListener(this);
//	DEBUGER_ASSERT_VALID(m_pMonitorListener != NULL);
//
//	if (!m_pMonitorListener->Create(m_pConfig->m_nMonPort))
//	{
//		strLog.Format("ECS MONITOR SERVER 소켓생성 실패! [%s]", CLib::GetSystemErrMsg());
//		WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CEcsDoc::RunServers"), NULL, TRUE);
//	}
//	else
//	{
//		m_pMonitorListener->Listen();
//		strLog.Format("ECS MONITOR SERVER 실행! [PORT=%d]", m_pConfig->m_nMonPort);
//		WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CEcsDoc::RunServers"));
//	}
}

void CEcsDoc::ConnectHostServer()
{
	DEBUGER_ASSERT_VALID(m_pConfig != NULL);

	if (m_pHostCl != NULL)
	{
		switch (m_pHostCl->m_enCommStatus)
		{
		case CHostCl::enStatusConnectOK:
			WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, _T("HOST CLIENT 이미 연결되어 있습니다."), _T("CEcsDoc::ConnectHostServer"), NULL, TRUE);
			break;

		case CHostCl::enStatusConnecting:
			WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, _T("HOST CLIENT 연결 요청중입니다."), _T("CEcsDoc::ConnectHostServer"), NULL, TRUE);
			break;
		}
		return;
	}

	m_pHostCl = new CHostCl(this);
	DEBUGER_ASSERT_VALID(m_pHostCl != NULL);

//	if (m_pHostCl->InitializeXmlDom() == FALSE)
//	{
//		delete m_pHostCl;
//		m_pHostCl = NULL;
//
//		WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, _T("XML 초기화 실패!"), _T("CEcsDoc::ConnectHostServer"), NULL, TRUE);
//		return;
//	}

	if (m_pHostCl->Create() == FALSE)
	{
		delete m_pHostCl;
		m_pHostCl = NULL;

		CString strLog;
		strLog.Format(_T("HOST CLIENT 소켓생성 실패! [%s]"), CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CEcsDoc::ConnectHostServer"), NULL, TRUE);
		return;
	}

	m_pHostCl->m_enCommStatus = CHostCl::enStatusConnecting;
	m_pHostCl->Connect(m_pConfig->m_portHost.m_strIP, m_pConfig->m_portHost.m_nPort);
}

void CEcsDoc::ReconnectServer()
{
	if ((m_pHostCl == NULL) && (m_pConfig->m_portHost.m_bRetry == TRUE))
		ConnectHostServer();

	 m_pEquipments.ReconnectServers();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEcsDoc::StatusDisplay()
{
	if (m_MsgList.IsEmpty())
		return;

//	CEcsView* pView = (CEcsView*)GetViewObject();
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();

	if (pWnd == NULL )
		return;

	CStatusBarMsg* pMsg = (CStatusBarMsg*)m_MsgList.GetHead();
//	DEBUGER_ASSERT_VALID(pMsg != NULL);

	if (pMsg == NULL )
		return;

	pWnd->DisplayMessage(pMsg->m_strMsg);

	if (m_pConfig != NULL && m_pConfig->m_bBeep && !pMsg->m_bNormalStatus)
		Beep(800, 300);

	m_MsgList.RemoveHead();
	delete pMsg;
}

void CEcsDoc::StatusDisplay(CString strMsg, BOOL bNormalStatus)
{
	if (m_MsgList.GetCount() > 5)
		return;

	if (strMsg.IsEmpty())
		return;

	if (m_MsgList.GetCount() > 0)
	{
		CStatusBarMsg* pLastMsg = (CStatusBarMsg*)m_MsgList.GetTail();
		if ((pLastMsg != NULL) && (pLastMsg->m_strMsg.Compare(strMsg) == 0))
			return;
	}

	TRY
	{
		m_MsgList.AddTail(new CStatusBarMsg(strMsg, bNormalStatus));
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
	}
	END_CATCH

//	if (m_pMonitorListener)
//	{
//		strMsg.Replace('<', '{');
//		strMsg.Replace('>', '}');
//		
//		CString strSend;
//		strSend.Format("%c<ECS><MESSAGE><STATUS MSG='%s' BNS='%d'/></MESSAGE></ECS>%c", 
//			CMonitorServer::enSTX, strMsg, bNormalStatus, CMonitorServer::enETX);
//		m_pMonitorListener->SendToClients(strSend);
//	}
}

void CEcsDoc::GetViewHandle()
{
	CView* pView = GetViewObject();
	ASSERT(pView != NULL);

	m_hWndView = pView->m_hWnd;
}

CView* CEcsDoc::GetViewObject()
{
	POSITION pos = GetFirstViewPosition();
	ASSERT(pos != NULL);

	if (pos == NULL)
		return NULL;

	return GetNextView(pos);
}

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc commands
/*
void CEcsDoc::WriteCVPLCWord(int nPLCNum, int nAddress, WORD wData)
{
	CCv* pCv = (CCv*) GetEquipment(CEquipment::enCV, nPLCNum);
	DEBUGER_ASSERT_VALID(pCv != NULL);

	pCv->WriteWordValue(nAddress, wData);
}
*/

void CEcsDoc::ExcuteClient(int nEquipKind, int nEquipNum)
{
	CEquipment* pEquipment = GetEquipment(nEquipKind, nEquipNum);
	DEBUGER_ASSERT_VALID(pEquipment != NULL);

	if (pEquipment->IsConnect())
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 중단하시겠습니까?"), MB_YESNO) != IDYES)
			return;

		pEquipment->Disconnect();
	}
	else
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 연결하시겠습니까?"), MB_YESNO) != IDYES)
			return;

		pEquipment->ConnectServer();
	}
}

void CEcsDoc::ExcuteServer(int nEquipKind, int nEquipNum)
{
	CEquipment* pEquipment = GetEquipment(nEquipKind, nEquipNum);
	DEBUGER_ASSERT_VALID(pEquipment != NULL);

	if (pEquipment->IsConnect())
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 중단하시겠습니까?"), MB_YESNO) != IDYES)
			return;

		pEquipment->Disconnect();
	}
	else
	{
		if (pEquipment->m_pListener)
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 종료하시겠습니까?"), MB_YESNO) != IDYES)
				return;

			pEquipment->m_pListener->ShutDown();
			delete pEquipment->m_pListener;
			pEquipment->m_pListener = NULL;

			CString strLog;
			strLog.Format(_T("%s 서버 강제종료"), pEquipment->GetDeviceName());
			WriteLog(LOG_TYPE_EVENT, LOG_POS_RGV, strLog, _T("CEcsDoc::ExcuteServer"));
		}
		else
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 실행하시겠습니까?"), MB_YESNO) != IDYES)
				return;

			pEquipment->RunServer();
		}
	}
}

BOOL CEcsDoc::OnComStatusHost(UINT nID)
{
	CString strID, strMsg;
	strID.Format(_T("%d"), nID);
	strMsg.LoadString(nID);
	AfxMessageBox(strMsg);
	return TRUE;
}

BOOL CEcsDoc::OnComStatusClient(UINT nID)
{
	CString strID, strMsg;
	strID.Format(_T("%d"), nID);
	if ((strID.GetLength() != 5) || (strID[0] != '1'))
	{
		DEBUGER_ASSERT_VALID_LOG(FALSE, strID);
		return FALSE;
	}

	int nEquipKind = _ttoi(strID.Mid(1,2));
	int nEquipNum = _ttoi(strID.Mid(3,2));
	CEquipment* pEquipment = GetEquipment(nEquipKind, nEquipNum);
	if (pEquipment == NULL)
	{
		strMsg.Format(_T("해당 설비객체가 존재하지 않습니다! [ID=%d]"), nID);
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if (pEquipment->IsConnect())
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 중단하시겠습니까?"), MB_YESNO) != IDYES)
			return TRUE;

		pEquipment->Disconnect();
	}
	else
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 연결하시겠습니까?"), MB_YESNO) != IDYES)
			return TRUE;

		pEquipment->ConnectServer();
	}

	return TRUE;
}

BOOL CEcsDoc::OnComStatusServer(UINT nID)
{
	CString strID, strMsg;
	strID.Format(_T("%d"), nID);
	if ((strID.GetLength() != 5) || (strID[0] != '1'))
	{
		DEBUGER_ASSERT_VALID_LOG(FALSE, strID);
		return FALSE;
	}

	int nEquipKind = _ttoi(strID.Mid(1,2));
	int nEquipNum = _ttoi(strID.Mid(3,2));
	CEquipment* pEquipment = GetEquipment(nEquipKind, nEquipNum);
	if (pEquipment == NULL)
	{
		strMsg.Format(_T("해당 설비객체가 존재하지 않습니다! [ID=%d]"), nID);
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if (pEquipment->IsConnect())
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 중단하시겠습니까?"), MB_YESNO) != IDYES)
			return TRUE;

		pEquipment->Disconnect();
	}
	else
	{
		if (pEquipment->m_pListener)
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 종료하시겠습니까?"), MB_YESNO) != IDYES)
				return TRUE;

			pEquipment->m_pListener->ShutDown();
			delete pEquipment->m_pListener;
			pEquipment->m_pListener = NULL;

			CString strLog;
			strLog.Format(_T("%s 서버 강제종료"), pEquipment->GetDeviceName());
			WriteLog(LOG_TYPE_EVENT, LOG_POS_LGV, strLog, _T("CEcsDoc::ExcuteServer"));
		}
		else
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 실행하시겠습니까?"), MB_YESNO) != IDYES)
				return TRUE;

			pEquipment->RunServer();
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CEcsDoc::CheckPassword()
{
	CPasswordDlg dlg;

	dlg.m_pDoc = this;
	if (dlg.DoModal() != IDOK) return FALSE;

	if (dlg.m_strPassword != m_pConfig->m_strPassword)
	{
		AfxMessageBox(_T("패스워드가 일치하지 않습니다. 재입력해 주시기 바랍니다."));
		return FALSE;
	}
	return TRUE;
}

void CEcsDoc::OnSystemConfiguration1() 
{
//	if (CheckPassword() == FALSE) return;
	CConfigPptSheet ConfigSheet(this, m_pConfig, _T("환경설정"), NULL, 0);
	ConfigSheet.DoModal();
}

void CEcsDoc::OnSystemConfiguration2() 
{
//	if (CheckPassword() == FALSE) return;
	CConfigPptSheet ConfigSheet(this, m_pConfig, _T("환경설정"), NULL, 1);
	ConfigSheet.DoModal();
}

void CEcsDoc::OnSystemConfiguration3() 
{
//	if (CheckPassword() == FALSE) return;
	CConfigPptSheet ConfigSheet(this, m_pConfig, _T("환경설정"), NULL, 2);
	ConfigSheet.DoModal();
}

void CEcsDoc::OnSystemConfiguration4() 
{
//	if (CheckPassword() == FALSE) return;
	CConfigPptSheet ConfigSheet(this, m_pConfig, _T("환경설정"), NULL, 3);
	
	if (AfxMessageBox(_T("랙 정보 전체를 조회하기 때문에 시간이 걸립니다. 조회하시겠습니까?"), MB_YESNO) == IDYES)
	ConfigSheet.DoModal();
}

void CEcsDoc::OnJobOffline() 
{
	CJobOfflineDlg dlg(this);
	dlg.DoModal();	
}

void CEcsDoc::GenerateSemiMoveJob(int nStratStnNo, int nDestStnNo)
{
	/*
	CString strLog;
	// 입고작업을 생성한다. - 작업번호를 생성한다.
	int nLuggNum = m_pJob->GenerateOfflineLuggNum();
	if (nLuggNum == 0)
	{
		strLog.Format("%d 단축키용 이동 작업  작업번호 생성 실패!", nLuggNum);
		WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CEcsDoc::OnJobTest"));
		m_bTesting = FALSE;
		return;
	}

	CJobItem oJobItem(this);
	CJobItem* pJobItem = NULL;

	if (nStratStnNo == nDestStnNo)
	{
		AfxMessageBox(_T("출발지와 도착지의 Station 번호가 같습니다. 확인해 주세요!"));
		return;
	}

	oJobItem.m_nLuggNum		= nLuggNum;
	oJobItem.m_nJobType		= enJobTypeSemiMove;
	oJobItem.m_nStartWH		= 1;
	oJobItem.m_strStartStn	= CConvert::ToString(nStratStnNo);
	oJobItem.m_nDestWH		= 1;
	oJobItem.m_strDestStn	= CConvert::ToString(nDestStnNo);


	CStationInfo* pStartStation = GetStationInfo(oJobItem.m_strStartStn);
	if (pStartStation == NULL)
	{
		AfxMessageBox(_T("출발위치가 유효하지 않은 STATION입니다. 확인해주세요!"));
		return;
	}

	CStationInfo* pDestStation = GetStationInfo(oJobItem.m_strDestStn);
	if (pDestStation == NULL)
	{
		AfxMessageBox(_T("도착위치가 유효하지 않은 STATION입니다. 확인해주세요!"));
		return;
	}

	strLog.Format(_T("[%s]에서 [%s]로 \n[단축키 이동 작업]을 생성하시겠습니까?"), pStartStation->m_strName, pDestStation->m_strName);
	if (AfxMessageBox(strLog, MB_OKCANCEL) != IDOK)
		return;

	if ((pJobItem = m_pJob->Add(oJobItem)) == NULL)
	{
		AfxMessageBox(oJobItem.GetJobTypeString() + _T(" 작업생성 실패!"));
		return;
	}

	strLog.Format(_T("%s 작업생성"), oJobItem.GetJobTypeString());
	WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, strLog, _T("CJobOfflineDlg::OnButtonCreate"), pJobItem);

	// 단축키로 작업을 생성할때는 Invoke 하지 않는다. - '신규'상태로 둔다. 
//	if (m_pJob->Invoke(pJobItem) == FALSE)
//	{
//		AfxMessageBox(oJobItem.GetJobTypeString() + _T(" 작업지시 실패!"));
//	}

	// 작업복사 
	m_oCvDlg.m_nLuggNum		= pJobItem->m_nLuggNum;
	m_oCvDlg.m_nJobType		= pJobItem->m_nJobType;
	m_oCvDlg.m_nDestPos		= pDestStation->GetTrackDestination();
	m_oCvDlg.m_ucGenCode	= pJobItem->m_ucGenCode;
//	m_oCvDlg.m_nSize		= pJobItem->m_nSize;
	m_oCvDlg.m_bClone		= TRUE;

	m_oCvDlg.m_btnPaste.EnableWindow(TRUE);
	//*/
}

void CEcsDoc::OnJobOffline1() 
{
	GenerateSemiMoveJob(113, 111);
}

void CEcsDoc::OnJobOffline2() 
{
	GenerateSemiMoveJob(113, 209);
}

// 함수사용안함 
void CEcsDoc::OnJobTestStart() 
{
	/*
	CJobTestDlg	dlg(this);

	int nTemp = dlg.DoModal();

	if (nTemp == IDCANCEL)
	{
//		if (dlg.m_strLocation.IsEmpty() == TRUE)
//		{
//			AfxMessageBox("Location을 입력하세요");
//			return;
//		}

		
		CString strLog;
		// 입고작업을 생성한다. - 작업번호를 생성한다.
		int nLuggNum = m_pJob->GenerateOfflineLuggNum();
		if (nLuggNum == 0)
		{
			strLog.Format("%d TEST 입고 작업번호 생성 실패!", nLuggNum);
			WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CEcsDoc::OnJobTest"));
			m_bTesting = FALSE;
			return;
		}

		// 출고작업을 생성한다.
		CJobItem oJobItem(this);
		CJobItem* pJobItem = NULL;
		CStationInfo* pStartStation = NULL;
		CStationInfo* pDestStation = NULL;

		CString strTemp;
		static int nBank = 0;
		static int nBay = 0;
		static int nLevel = 0;

		m_strTestLocation = "";

		for (int i = nBank ; i < m_pConfig->m_nBank ; i++)
		{
			for (int j = nBay ; j < m_pConfig->m_nBay ; j++)
			{
				for (int k = nLevel ; k < m_pConfig->m_nLevel ; k++)
				{
					strTemp.Format("%02d-%03d-%02d", i + 1, j + 1, k + 1);
					
					if (m_strTestLocation == "")
					{
						for (int l = 0 ; l < m_pConfig->m_strDisableRacks.GetSize() ; l++)
						{
							if (strTemp != m_pConfig->m_strDisableRacks[l])
							{
								nBank	= i;
								nBay	= j;
								nLevel	= k;
								m_strTestLocation.Format("%02d%03d%02d", i + 1, j + 1, k + 1);
								break;
							}
						}
					}
				}
			}	
		}
		

//		m_strTestLocation = dlg.m_strLocation;
		m_strTestStation = dlg.m_strStation;

		pStartStation = GetScStationInfo(1, m_strTestLocation);
		DEBUGER_ASSERT_VALID(pStartStation != NULL);

		oJobItem.m_nLuggNum		= nLuggNum;
		oJobItem.m_nJobType		= enJobTypeSemiRet;
		oJobItem.m_nStartWH		= 1;
		oJobItem.m_strStartStn	= pStartStation->m_strID;
		oJobItem.m_strStartLoc	= m_strTestLocation;
		oJobItem.m_nDestWH		= 1;
		oJobItem.m_strDestStn	= m_strTestStation;
		oJobItem.m_ucGenCode	= 1;

	//	// 동일한 작업이 존재하는지 체크함!
	//	if (pJobItem = m_pJob->IsEqualAll(&oJobItem) == NULL)
	//	{
	//		strLog.Format(_T("동일한 작업이 존재... 작업정보 확인 [%d]"), oJobItem.m_nLuggNum);
	//		WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CEcsDoc::OnJobTest"), pJobItem);
	//		return;
	//	}

		if ((pJobItem = m_pJob->Add(oJobItem)) == NULL)
		{
			strLog.Format("%s TEST 입고 작업생성 실패!", oJobItem.GetJobTypeString());
			WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CEcsDoc::OnJobTest"), pJobItem);
			m_bTesting = FALSE;
			return;
		}

		strLog.Format(_T("%s TEST 입고 작업생성"), oJobItem.GetJobTypeString());
		WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CEcsDoc::OnJobTest"), pJobItem);

		if (m_pJob->Invoke(pJobItem) == FALSE)
		{
			strLog.Format("%s 작업지시 실패!", oJobItem.GetJobTypeString());
			WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CEcsDoc::OnJobTest"), pJobItem);
			return;
		}
		m_bTesting = TRUE;
	}
	//*/
}

// 함수사용안함 
void CEcsDoc::OnJobTestStop() 
{
	/*
	// 출고 작업이 있는지 Check 
	CJobItem * pRetJobItem = m_pJob->FetchByTestJob(enJobPatternRet);
	CJobItem * pStoJobItem = m_pJob->FetchByTestJob(enJobPatternSto);

	if(pRetJobItem == NULL)
	{
		// 없으면 그냥 종료
		m_bTesting = FALSE;
		return;
	}
	else
	{
		// 있으면 입고 중인지 확인할 것!  
		if(pStoJobItem == NULL)
		{
			// 입고 중인 작업이 없으면 입고작업이 생성되면 출고작업을 삭제 하라는 메세지를 띄우고 종료
			if(pRetJobItem->m_nJobStatus == enJobStatusScInvoke)
			{
				AfxMessageBox("현재 출고 중입니다. \n출고 중 일때는 중지할 수 없습니다. \n출고 HS에서 입고 작업이 실행되면... \n그때 다시 한번 중지해 주시시오");
				return;
			}
//				m_bTesting = FALSE;
		}
		else
		{
			AfxMessageBox("현재 입고 중인 작업이 완료되면 TEST모드가 끝납니다. \n기다려 주세요 \n출고 작업은 자동으로 삭제 됩니다.");
			m_pJob->Remove(pRetJobItem);
			m_bTesting = FALSE;
			return;
		}
	}

	m_bTesting = FALSE;
	return;
	//*/
}

void CEcsDoc::OnJobManager() 
{
	if (m_pJobDlg != NULL)
	{
		m_pJobDlg->ShowWindow(SW_SHOWNORMAL);
		m_pJobDlg->BringWindowToTop();
	}
	else
	{
		m_pJobDlg = new CJobDlg(this);
		m_pJobDlg->Create(IDD_JOB);
		m_pJobDlg->ShowWindow(SW_SHOW);
	}
}

void CEcsDoc::OnLogAll() 
{
	/*
	if (m_pLogDlg != NULL)
	{
		m_pLogDlg->ShowWindow(SW_SHOWNORMAL);
		m_pLogDlg->BringWindowToTop();
	}
	else
	{
		m_pLogDlg = new CLogDlg(this, m_pLog);
		
		m_pLogDlg->Create(IDD_LOG);
		m_pLogDlg->ShowWindow(SW_SHOW);
	}
	*/

	ShellExecute(NULL, _T("open"), _T(".\\EcsLog.exe"), NULL, NULL, SW_SHOW);
//	CString strIpAddress = _T("127.0.0.1");
//	CString strCommandLine = _T("/k ping -t ") + strIpAddress;
//	::ShellExecute(NULL, _T("open"), _T("cmd.exe"), strCommandLine, NULL, SW_SHOW);
}

void CEcsDoc::OnWarning() 
{
	if (m_pWarningDlg != NULL)
	{
		m_pWarningDlg->ShowWindow(SW_SHOW);
		if (m_pWarningDlg->GetSafeHwnd() != NULL)
			::PostMessage(m_pWarningDlg->GetSafeHwnd(), WM_USER_REFRESH_NOTIFY, 0, 0);
	}
	else
	{
		m_pWarningDlg = new CWarningDlg(this);
		m_pWarningDlg->Create(CWarningDlg::IDD);
		m_pWarningDlg->ShowWindow(TRUE);	
	}
}

void CEcsDoc::WriteLog(int nLogType, int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszPos, LPCTSTR lpszProdID /* = NULL */, BOOL bAlarm /* = FALSE */)
{
	DEBUGER_ASSERT_VALID(m_pLog != NULL);
	DEBUGER_ASSERT_VALID(m_pConfig != NULL);

	CString strLog;
	if (lpszPos == NULL) 
		strLog.Format(_T("%s"), lpszContent);
	else 
		strLog.Format(_T("%s... %s"), lpszPos, lpszContent);

	if (m_pConfig->m_bLogTrace) 
		DEBUGER_TRACE(strLog, lpszPos);

	if (strLog.GetLength() > _MAX_PATH)
		strLog = strLog.Left(_MAX_PATH);

	switch (nLogType)
	{
	case LOG_TYPE_JOB:		if (m_pLog) m_pLog->Job(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);		break;
	case LOG_TYPE_EVENT:	if (m_pLog) m_pLog->Event(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);	break;
	case LOG_TYPE_ERROR:	if (m_pLog) m_pLog->Error(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);	break;
	case LOG_TYPE_ALARM:	if (m_pLog) m_pLog->Alarm(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);	break;
	case LOG_TYPE_DEBUG:	if (m_pLog) m_pLog->Debug(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);	break;
	}
}

void CEcsDoc::WriteLog(int nLogType, int nLogPos, LPCTSTR lpszContent, LPCTSTR lpszPos, CJobItem* pJobItem /* = NULL */, BOOL bAlarm /* = FALSE */)
{
	if (pJobItem == NULL) 
		WriteLog(nLogType, nLogPos, LOG_SYSTEM, lpszContent, lpszPos, NULL, bAlarm);
	else 
		WriteLog(nLogType, nLogPos, pJobItem->m_nLuggNum, lpszContent, lpszPos, pJobItem->m_strBarcode, bAlarm);
}

void CEcsDoc::Alarm(int nLogPos, LPCTSTR lpszLog, CJobItem* pJobItem /* = NULL */)
{
	if (m_pLog == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	if (pJobItem == NULL) m_pLog->ShowAlarm(nLogPos, LOG_SYSTEM, lpszLog);
	else m_pLog->ShowAlarm(nLogPos, pJobItem->m_nLuggNum, lpszLog, pJobItem->m_strBarcode);
}

void CEcsDoc::MulticastServerInfo(CMonitorServer* pMonitorSv, int nCommStatus)
{
	CString strSend;
	strSend.Format(_T("%c<ECS><HOST><SERVER>%d</SERVER></HOST></ECS>%c"), 
		CMonitorServer::enSTX, nCommStatus, CMonitorServer::enETX);

//	if (pMonitorSv)
//	{
//		pMonitorSv->SendData(strSend);
//	}
//	else
//	{
//		if (m_pMonitorListener)
//			m_pMonitorListener->SendToClients(strSend);
//	}
}

void CEcsDoc::MulticastClientInfo(CMonitorServer* pMonitorSv, int nCommStatus)
{
	CString strSend;
	strSend.Format(_T("%c<ECS><HOST><CLIENT>%d</CLIENT></HOST></ECS>%c"), 
		CMonitorServer::enSTX, nCommStatus, CMonitorServer::enETX);

//	if (pMonitorSv)
//	{
//		pMonitorSv->SendData(strSend);
//	}
//	else
//	{
//		if (m_pMonitorListener)
//			m_pMonitorListener->SendToClients(strSend);
//	}
}

//////////////////////////////////////////////////////////////////////////
CDciControl* CEcsDoc::GetDciControl(CString& strCID)	
{
	CEcsView* pView = (CEcsView*)GetViewObject();
	DEBUGER_ASSERT_VALID(pView != NULL);

	if (pView->m_tabLayout.GetSafeHwnd())
	{
		switch(pView->m_tabLayout.GetCurSel()) {
		case 1-1:		return m_layout.GetDciControl(strCID);			
		case 2-1:		return m_layout2.GetDciControl(strCID);			
		}
	}

	return m_layout.GetDciControl(strCID);;
}

void CEcsDoc::OnRackConfig() 
{
	// TODO: Add your command handler code here
//	CJobOfflineDlg dlg(this);
//	dlg.DoModal();	

}

void CEcsDoc::OnSysUserManager() 
{
	// TODO: Add your command handler code here
	//CUserManagerDlg dlg;
	//dlg.DoModal();
}
