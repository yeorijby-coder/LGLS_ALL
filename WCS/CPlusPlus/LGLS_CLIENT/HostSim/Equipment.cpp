// Equipment.cpp: implementation of the CEquipment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "Equipment.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEVICE m_port.m_strDevice

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CEquipment, CObject)

CEquipment::CEquipment(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	m_pXml = new CXmlDom;
	DEBUGER_ASSERT_VALID(m_pXml != NULL);
	
	m_nIndex = nIndex;
	m_nNumber = nNumber;
	m_enKind = enNone;
	m_port.m_strDevice = lpszDevice;

	m_pSocket = NULL;
	m_pThreadSocket = NULL;
	m_pListener = NULL;

	m_pThread = NULL;
	m_pfThreadProc = CEquipment::ThreadProc;

	m_pEquipDlg = NULL;

	m_nForkType = -1;
	m_nTotalEquipCount = 0;

	m_hEventArray[enEventKill] = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventArray[enEventSend] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CEquipment::~CEquipment()
{
	::CloseHandle(m_hEventArray[enEventSend]);
	::CloseHandle(m_hEventArray[enEventKill]);


	for (POSITION pos=m_cmdList.GetHeadPosition(); pos!=NULL; )
		delete m_cmdList.GetNext(pos);
	m_cmdList.RemoveAll();

	if (m_pListener != NULL)
		delete m_pListener;
	
	if (m_pSocket != NULL)
		delete m_pSocket;

	if (m_pThreadSocket != NULL)
		delete m_pThreadSocket;

	delete m_pXml;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEquipment::InitializeXmlDom()
{
	DEBUGER_ASSERT_VALID(m_pXml != NULL);
	return m_pXml->InitializeXmlDom();
}


BOOL CEquipment::SetCmdMsg(CCmdMsg* pCmdMsg)
{
	if (pCmdMsg == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}

	TRY
	{
		m_cmdList.AddTail(pCmdMsg);
	}
	CATCH (CException, e)
	{
		m_strLog.Format(_T("%s [%s]"), DEVICE, CLib::GetExceptionString(e));
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::SetCmdMsg"));
		return FALSE;
	}
	END_CATCH

	::SetEvent(m_hEventArray[enEventSend]);
	return TRUE;
}

void CEquipment::UpdateCommStatus(int nStatus)
{
	UINT uMsg;
	switch (m_enKind)
	{
	case enCV:		uMsg = WM_USER_CV_NOTIFY;		break;
	case enSC:		uMsg = WM_USER_SC_NOTIFY;		break;
	case enRGV:		uMsg = WM_USER_RTV_NOTIFY;		break;
//	case enLGV:		uMsg = WM_USER_LGV_NOTIFY;		break;
//	case enLIFTER:	uMsg = WM_USER_LIFTER_NOTIFY;	break;
//	case enRFID:	uMsg = WM_USER_RFID_NOTIFY;		break;
//	case enBCR:		uMsg = WM_USER_BCR_NOTIFY;		break;
//	case enDISPLAY:	uMsg = WM_USER_DISPLAY_NOTIFY;	break;
	default:		return;
	}

	if (m_pDoc && m_pDoc->m_hWndView)
		::PostMessage(m_pDoc->m_hWndView, uMsg, m_nNumber-1, nStatus);
}

void CEquipment::WriteLog(int nLogType, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszPos, LPCTSTR lpszProdID /* = NULL */, BOOL bAlarm /* = FALSE */)
{
	int nLogPos;
	switch (m_enKind)
	{
	case enCV:		nLogPos = LOG_POS_CV;		break;
	case enSC:		nLogPos = LOG_POS_SC;		break;
	case enRTV:		nLogPos = LOG_POS_RGV;		break;
	case enRGV:		nLogPos = LOG_POS_RGV;		break;
//	case enLIFTER:	nLogPos = LOG_POS_LIFTER;	break;
//	case enLGV:		nLogPos = LOG_POS_LGV;		break;
//	case enRFID:	nLogPos = LOG_POS_RFID;		break;
//	case enBCR:		nLogPos = LOG_POS_BCR;		break;
//	case enDISPLAY:	nLogPos = LOG_POS_DISPLAY;	break;
	default:		nLogPos = LOG_POS_SYSTEM;	break;
	}

	m_pDoc->WriteLog(nLogType, nLogPos, nLuggNum, lpszContent, lpszPos, lpszProdID, bAlarm);
}

void CEquipment::WriteLog(int nLogType, LPCTSTR lpszContent, LPCTSTR lpszPos, CJobItem* pJobItem /* = NULL */, BOOL bAlarm /* = FALSE */)
{
	if (pJobItem == NULL) 
		WriteLog(nLogType, LOG_SYSTEM, lpszContent, lpszPos, NULL, bAlarm);
	else 
		WriteLog(nLogType, pJobItem->m_nLuggNum, lpszContent, lpszPos, pJobItem->m_strBarcode, bAlarm);
}

void CEquipment::RefreshDialog(WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	if (m_pEquipDlg && m_pEquipDlg->GetSafeHwnd())
		m_pEquipDlg->PostMessage(WM_USER_REFRESH_NOTIFY, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CEquipment::IsConnect()
{
	if (m_enKind == enRGV)
//	if (m_enKind == enCV && m_nNumber == 1) 
	{
		if ((m_pThreadSocket == NULL) || (m_pThreadSocket->m_enCommStatus != CAsyncSocketEx::enStatusConnectOK))
		{
			return FALSE;
		}
	}
	else
	{
		if ((m_pSocket == NULL) || (m_pSocket->m_enCommStatus != CAsyncSocketEx::enStatusConnectOK))
		{
			return FALSE;
		}
	}
	
//	UpdateCommStatus(NOTIFY_SEND);
	return TRUE;
}

BOOL CEquipment::ConnectServer()
{
	if (IsConnect() == TRUE)
		return FALSE;

	if (m_enKind == enRGV)
//	if (m_enKind == enCV && m_nNumber == 1)
	{
		if (m_pThreadSocket)
		{
			switch (m_pThreadSocket->m_enCommStatus)
			{
			case CAsyncSocketEx::enStatusConnectOK:
				m_strLog.Format(_T("%s 이미 연결되어 있습니다."), DEVICE);
				break;

			case CAsyncSocketEx::enStatusConnecting:
				m_strLog.Format(_T("%s 연결 요청중입니다."), DEVICE);
				break;
			}

			m_pDoc->StatusDisplay(m_strLog, FALSE);
			return FALSE;
		}

		if ((m_pThreadSocket = CreateThreadSocket()) == NULL)
		{
			m_strLog.Format(_T("%s THREAD SOCKET 생성 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
			WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::ConnectServer"), NULL, TRUE);
			return FALSE;
		}

		if (!m_pThreadSocket->Create())
		{
			m_strLog.Format(_T("%s 연결요청 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
			WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::ConnectServer"), NULL, TRUE);

			delete m_pThreadSocket;
			m_pThreadSocket = NULL;

			return FALSE;
		}

		m_pThreadSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnecting;
		m_pThreadSocket->Connect(m_port.m_strIP, m_port.m_nPort);

		return TRUE;
	}
//*/
	if (m_pSocket)
	{
		switch (m_pSocket->m_enCommStatus)
		{
		case CAsyncSocketEx::enStatusConnectOK:
			m_strLog.Format(_T("%s 이미 연결되어 있습니다."), DEVICE);
			break;

		case CAsyncSocketEx::enStatusConnecting:
			m_strLog.Format(_T("%s 연결 요청중입니다."), DEVICE);
			break;
		}

		m_pDoc->StatusDisplay(m_strLog, FALSE);
		return FALSE;
	}

	if ((m_pSocket = CreateSocket()) == NULL)
	{
		m_strLog.Format(_T("%s SOCKET 생성 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::ConnectServer"), NULL, TRUE);
		return FALSE;
	}

	if (!m_pSocket->Create())
	{
		m_strLog.Format(_T("%s 연결요청 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::ConnectServer"), NULL, TRUE);

		delete m_pSocket;
		m_pSocket = NULL;

		return FALSE;
	}

	m_pSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnecting;
	m_pSocket->Connect(m_port.m_strIP, m_port.m_nPort);

	return TRUE;
}

void CEquipment::OnConnectSocket(int nErrorCode)
{
	// SC와 RTV 는 다르게 pInfo를 가져와야함!
	int j = 0;
	int nScCount = 0;
	int nRtvCount = 0;
	if (m_enKind == CEquipment::enSC )
	{
		CScDongJin* pSc = (CScDongJin *) this;
		if (pSc == NULL)
			return;

		j = 1 + (SC1_PLC_CNT * (m_nNumber - 1));					// 1	4
		nScCount = SC1_PLC_CNT * (m_nNumber - 1) + SC1_PLC_CNT;		// 3	6
			
		if (nErrorCode)
		{
			m_strLog.Format(_T("%s 접속 실패 [%s][%s]"), DEVICE, m_pSocket->GetPeerInfo(), CLib::GetSystemErrMsg(nErrorCode));
			WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"), NULL, TRUE);

			UpdateCommStatus(NOTIFY_ERROR);
			m_pSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectFail;

			m_pSocket->ShutDown();
			m_pSocket = NULL;

			for ( ;j <= nScCount ; j++)
			{
				CScInfo* pInfo =  (CScInfo*)pSc->GetInfo(j, FALSE);

				if (pInfo == NULL)
					continue;

				pInfo->Initialize();
				pInfo->m_bModified = TRUE;
				pInfo->InvokeControl();
//				pInfo->SetLampInfo(FALSE);
			}
			return;
		}
		else
		{
			m_strLog.Format(_T("%s 접속 성공 [%s]"), DEVICE, m_pSocket->GetPeerInfo());
			WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"));

			UpdateCommStatus(NOTIFY_SEND);
			m_pSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectOK;

			::ResetEvent(m_hEventArray[enEventKill]);
			m_pThread = ::AfxBeginThread(m_pfThreadProc, this);
			DEBUGER_ASSERT_VALID(m_pThread != NULL);
			if (m_pThread == NULL)
				return;

			for ( ;j <= nScCount ; j++)
			{
				CScInfo* pInfo =  (CScInfo*)pSc->GetInfo(j, FALSE);

				if (pInfo == NULL)
					continue;

				pInfo->m_bModified = TRUE;
				pInfo->InvokeControl();
//				pInfo->SetLampInfo(TRUE);
			}
			return;
		}
	}
	else
	{
		CInfo* pInfo = GetInfo();

		if (pInfo == NULL)
			return;

		if (nErrorCode)
		{
			m_strLog.Format(_T("%s 접속 실패 [%s][%s]"), DEVICE, m_pSocket->GetPeerInfo(), CLib::GetSystemErrMsg(nErrorCode));
			WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"), NULL, TRUE);

			UpdateCommStatus(NOTIFY_ERROR);
			m_pSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectFail;

			m_pSocket->ShutDown();
			m_pSocket = NULL;

			pInfo->Initialize();
//			pInfo->SetLampInfo(FALSE);
		}
		else
		{
			m_strLog.Format(_T("%s 접속 성공 [%s]"), DEVICE, m_pSocket->GetPeerInfo());
			WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"));

			UpdateCommStatus(NOTIFY_SEND);
			m_pSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectOK;

			::ResetEvent(m_hEventArray[enEventKill]);
			m_pThread = ::AfxBeginThread(m_pfThreadProc, this);

			if (m_pThread == NULL)
				return;

			if (m_enKind == CEquipment::enCV )
			{
				CCv* pCv = (CCv *) this;
				if (pCv == NULL)
					return;

				CCvInfo* pCvInfo = (CCvInfo*)pCv->GetInfo();

				if (pCvInfo == NULL)
					return;

//				pCvInfo->SetLampInfo(TRUE);
			}
		}

		pInfo->m_bModified = TRUE;
		pInfo->InvokeControl();
	}
}

void CEquipment::OnConnectThreadSocket(int nErrorCode)
{
	int j = 0;
	int nRtvCount = 0;
	if (m_enKind == CEquipment::enRGV )
	{
		CLgv* pRtv = (CLgv *) this;
		if (pRtv == NULL)
			return;

		if (m_nNumber == 1)
		{
			j = 1 ;								// 1
			nRtvCount = RTV1_PLC_CNT;			// 5	

			if (nErrorCode)
			{
				m_strLog.Format(_T("%s 접속 실패 [%s][%s]"), DEVICE, m_pThreadSocket->GetPeerInfo(), CLib::GetSystemErrMsg(nErrorCode));
				WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"), NULL, TRUE);

				UpdateCommStatus(NOTIFY_ERROR);
				m_pThreadSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectFail;

				m_pThreadSocket->ShutDown();
				m_pThreadSocket = NULL;

				CLgvInfo* pInfo =  (CLgvInfo*)pRtv->GetInfo();

				if (pInfo == NULL)
					return;

				pInfo->Initialize();
				pInfo->m_bModified = TRUE;
				pInfo->InvokeControl();
//				pInfo->SetLampInfo(FALSE, pInfo->m_pCvInfo, m_nNumber);

				return;
			}
			else
			{
				m_strLog.Format(_T("%s 접속 성공 [%s]"), DEVICE, m_pThreadSocket->GetPeerInfo());
				WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"));

				UpdateCommStatus(NOTIFY_SEND);
				m_pThreadSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectOK;

				::ResetEvent(m_hEventArray[enEventKill]);
				m_pThread = ::AfxBeginThread(m_pfThreadProc, this);
				if (m_pThread == NULL)
					return;

				CLgvInfo* pInfo =  (CLgvInfo*)pRtv->GetInfo();

				if (pInfo == NULL)
					return;

				pInfo->m_wErrorCode = 0;
				pInfo->m_bModified = TRUE;
				pInfo->InvokeControl();

				int nCarCnt = pInfo->m_oLgvVehicles.GetSize();
				CLgvVehicle* pVehicle = NULL;
				for (int k = 0 ; k < nCarCnt ; k++) 
				{
					pVehicle = pInfo->m_oLgvVehicles[k];

					if (pVehicle == NULL)
						continue;

					pInfo->m_bModified = TRUE;
					pInfo->InvokeControl(pVehicle->m_nVehicleID);
//					pVehicle->SetLampInfo(TRUE, pInfo->m_pCvInfo, m_nNumber);
				}

				return;
			}
		}
		else
		{
			j = RTV1_PLC_CNT + 1 ;				// 6
			nRtvCount = RTV_PLC_TOT_CNT;		// 13	

			if (nErrorCode)
			{
				m_strLog.Format(_T("%s 접속 실패 [%s][%s]"), DEVICE, m_pThreadSocket->GetPeerInfo(), CLib::GetSystemErrMsg(nErrorCode));
				WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"), NULL, TRUE);

				UpdateCommStatus(NOTIFY_ERROR);
				m_pThreadSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectFail;

				m_pThreadSocket->ShutDown();
				m_pThreadSocket = NULL;

				CLgvInfo* pInfo =  (CLgvInfo*)pRtv->GetInfo();

				if (pInfo == NULL)
					return;

				pInfo->Initialize();
				pInfo->m_bModified = TRUE;
				pInfo->InvokeControl();

				return;
			}
			else
			{
				m_strLog.Format(_T("%s 접속 성공 [%s]"), DEVICE, m_pThreadSocket->GetPeerInfo());
				WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"));

				UpdateCommStatus(NOTIFY_SEND);
				m_pThreadSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectOK;

				::ResetEvent(m_hEventArray[enEventKill]);
				m_pThread = ::AfxBeginThread(m_pfThreadProc, this);

				if (m_pThread == NULL)
					return;

				CLgvInfo* pInfo =  (CLgvInfo*)pRtv->GetInfo();

				if (pInfo == NULL)
					return;

				pInfo->m_wErrorCode = 0;
				pInfo->m_bModified = TRUE;
				pInfo->InvokeControl();

				int nCarCnt = pInfo->m_oLgvVehicles.GetSize();
				CLgvVehicle* pVehicle = NULL;
				for (int k=0;k<nCarCnt;k++) 
				{
					pVehicle = pInfo->m_oLgvVehicles[k];

					if (pVehicle == NULL)
						continue;

					pInfo->m_bModified = TRUE;
					pInfo->InvokeControl(pVehicle->m_nVehicleID);
//					pVehicle->SetLampInfo(TRUE, pInfo->m_pCvInfo, m_nNumber);
				}

				return;
			}
		}
	}
	else
	{
		CInfo* pInfo = GetInfo();

		if (pInfo == NULL)
			return;

		if (nErrorCode)
		{
			m_strLog.Format(_T("%s 접속 실패 [%s][%s]"), DEVICE, m_pThreadSocket->GetPeerInfo(), CLib::GetSystemErrMsg(nErrorCode));
			WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectThreadSocket"), NULL, TRUE);

			UpdateCommStatus(NOTIFY_ERROR);
			m_pThreadSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectFail;

			m_pThreadSocket->ShutDown();
			m_pThreadSocket = NULL;

			pInfo->Initialize();

		}
		else
		{
			m_strLog.Format(_T("%s 접속 성공 [%s]"), DEVICE, m_pThreadSocket->GetPeerInfo());
			WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectThreadSocket"));

			int nNum = m_nNumber;
			int nKind = m_enKind;
			
			UpdateCommStatus(NOTIFY_SEND);
			m_pThreadSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectOK;

			::ResetEvent(m_hEventArray[enEventKill]);
			m_pThread = ::AfxBeginThread(m_pfThreadProc, this);

			if (m_pThread == NULL)
				return;
		}

		pInfo->m_bModified = TRUE;
		pInfo->InvokeControl();
	}
}


BOOL CEquipment::RunServer()
{
	DEBUGER_ASSERT_VALID(m_port.IsKindOf(RUNTIME_CLASS(CPortEthernet)));

	if (m_pListener)
	{
		m_strLog.Format(_T("%s 리스너가 이미 동작하고 있습니다."), DEVICE);
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::RunServer"), NULL, TRUE);
		return FALSE;
	}

	m_pListener = new CListenerSk(this);

	if (m_pListener == NULL)
		return FALSE;

	if (!m_pListener->Create(m_port.m_nPort))
	{
		m_strLog.Format(_T("%s 리스너 SOCKET 생성 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::RunServer"), NULL, TRUE);
		return FALSE;
	}

	if (!m_pListener->Listen())
	{
		m_strLog.Format(_T("%s 리스너 LISTEN 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::RunServer"), NULL, TRUE);
		return FALSE;
	}

	m_strLog.Format(_T("%s 서버 실행! [PORT=%d]"), DEVICE, m_port.m_nPort);
	WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::RunServer"));

	return TRUE;
}

void CEquipment::OnAcceptSocket(int nErrorCode)
{
	if (nErrorCode)
	{
		UpdateCommStatus(NOTIFY_ERROR);
		m_strLog.Format(_T("%s SOCKET ERROR [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"), NULL, TRUE);
		return;
	}

	if (m_pSocket != NULL)
	{
		m_pSocket->OnClose(0);

		if (m_pSocket != NULL)
			return;

		m_strLog.Format(_T("%s 신규 연결요청으로 인하여 현재 연결된 CLIENT 접속을 종료"), DEVICE);
		WriteLog(LOG_TYPE_DEBUG, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"), NULL, TRUE);
	}

	if ((m_pSocket = CreateSocket()) == NULL)
	{
		UpdateCommStatus(NOTIFY_ERROR);
		m_strLog.Format(_T("%s SOCKET 생성 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"), NULL, TRUE);
		return;
	}

	if (!m_pListener->Accept(*m_pSocket))
	{
		delete m_pSocket;
		m_pSocket = NULL;

		UpdateCommStatus(NOTIFY_ERROR);
		m_strLog.Format(_T("%s ACCEPT 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg(nErrorCode));
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"), NULL, TRUE);
		return;
	}

	m_pSocket->m_enCommStatus = CAsyncSocketEx::enStatusConnectOK;
	UpdateCommStatus(NOTIFY_SEND);

	::ResetEvent(m_hEventArray[enEventKill]);
	m_pThread = ::AfxBeginThread(m_pfThreadProc, this);

	if(m_pThread == NULL)
		return;

	if(GetInfo() == NULL)
		return;

	GetInfo()->m_bModified = TRUE;
	GetInfo()->InvokeControl();

	m_strLog.Format(_T("%s CLIENT 접속 성공! [%s]"), DEVICE, m_pSocket->GetPeerInfo());
	WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"));
}

void CEquipment::Disconnect()
{
	if (m_enKind == enRGV)// || m_enKind == enCV)
//	if (m_enKind == enCV && m_nNumber == 1)
	{
		if (m_pThreadSocket)
			m_pThreadSocket->OnClose(0);
	}
	else
	{
		if (m_pSocket)
			m_pSocket->OnClose(0);
	}
}

void CEquipment::OnCloseSocket(int nErrorCode)
{
	if(m_pSocket == NULL)
		return;

	UpdateCommStatus(NOTIFY_ERROR);

	if (m_pThread)
	{
		DWORD dwExitCode = 0;
		if (::GetExitCodeThread(m_pThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
		{
			m_pThread->m_bAutoDelete = FALSE;

			::SetEvent(m_hEventArray[enEventKill]);
			::SetEvent(m_pSocket->m_hRecvEvent);
			
			if (::WaitForSingleObject(m_pThread->m_hThread, m_pDoc->m_pConfig->m_dwExitTimeout) == WAIT_TIMEOUT)
			{
				if (::TerminateThread(m_pThread->m_hThread, dwExitCode))
				{
					m_strLog.Format(_T("%s THREAD 강제종료"), DEVICE);
					WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseSocket"));
				}
				else
				{
					m_strLog.Format(_T("%s THREAD 강제종료 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
					WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseSocket"));
				}
			}
			else
			{
				m_strLog.Format(_T("%s THREAD 정상종료"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseSocket"));
			}

			delete m_pThread;
			m_pThread = NULL;
		}
	}

	m_strLog.Format(_T("%s 연결해제 [%s]"), DEVICE, CLib::GetSystemErrMsg(nErrorCode));
	WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseSocket"));
	
	m_pSocket->m_enCommStatus = CAsyncSocketEx::enStatusClose;
	m_pSocket->ShutDown();
	m_pSocket = NULL;

	int j, nScCount;
	if (m_enKind == CEquipment::enSC )
	{
		j = 1 + (SC1_PLC_CNT * (m_nNumber - 1));					// 1	4
		nScCount = SC1_PLC_CNT * (m_nNumber - 1) + SC1_PLC_CNT;		// 3	6
		
		CScDongJin* pSc = (CScDongJin *) this;
		if (pSc == NULL)
			return;
		
		for ( ;j <= nScCount ; j++)
		{
			CScInfo* pInfo =  (CScInfo*)pSc->GetInfo(j, FALSE);

			if (pInfo == NULL)
				continue;

			pInfo->Initialize();
			pInfo->StatusReport();
			pInfo->InvokeControl();
//			pInfo->SetLampInfo(FALSE);
		}
	}
	else
	{
		if(GetInfo() == NULL)
			return;

		GetInfo()->Initialize();
		GetInfo()->StatusReport();
		GetInfo()->InvokeControl();
//		GetInfo()->SetLampInfo(FALSE);
	}
}

void CEquipment::OnCloseThreadSocket(int nErrorCode)
{
//	DEBUGER_ASSERT_VALID(m_pThreadSocket != NULL);
	if(m_pThreadSocket == NULL)
		return;

	UpdateCommStatus(NOTIFY_ERROR);

	if (m_pThread)
	{
		DWORD dwExitCode = 0;
		if (::GetExitCodeThread(m_pThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
		{
			m_pThread->m_bAutoDelete = FALSE;

			::SetEvent(m_hEventArray[enEventKill]);
			::SetEvent(m_pThreadSocket->m_hRecvEvent);
			
			if (::WaitForSingleObject(m_pThread->m_hThread, m_pDoc->m_pConfig->m_dwExitTimeout) == WAIT_TIMEOUT)
			{
				if (::TerminateThread(m_pThread->m_hThread, dwExitCode))
				{
					m_strLog.Format(_T("%s THREAD 강제종료"), DEVICE);
					WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseThreadSocket"));
				}
				else
				{
					m_strLog.Format(_T("%s THREAD 강제종료 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
					WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseThreadSocket"));
				}
			}
			else
			{
				m_strLog.Format(_T("%s THREAD 정상종료"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseThreadSocket"));
			}

			delete m_pThread;
			m_pThread = NULL;
		}
	}

	m_strLog.Format(_T("%s 연결해제 [%s]"), DEVICE, CLib::GetSystemErrMsg(nErrorCode));
	WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseThreadSocket"));
	
	m_pThreadSocket->m_enCommStatus = CAsyncSocketEx::enStatusClose;
	m_pThreadSocket->ShutDown();
	m_pThreadSocket = NULL;
//	delete m_pThreadSocket;

	int j = 0;
	int nRtvCount = 0;
	if (m_enKind == CEquipment::enRGV )
	{
		CLgv* pRtv = (CLgv *) this;
		if (pRtv == NULL)
			return;

		if (m_nNumber == 1)
		{
			j = 1 ;								// 1
			nRtvCount = RTV1_PLC_CNT;			// 5	

			for ( ;j <= nRtvCount ; j++)
			{
				CLgvInfo* pInfo =  (CLgvInfo*)pRtv->GetInfo();

				if (pInfo == NULL)
					continue;

				pInfo->Initialize();
				pInfo->StatusReport();
				pInfo->InvokeControl();

//				CLgvVehicle* pVehicle = (CLgvVehicle*)pInfo->GetLgvVehicle(j);
//
//				if (pVehicle == NULL)
//					continue;
//
//				pVehicle->SetLampInfo(FALSE, pInfo->m_pCvInfo, m_nNumber);
			}
			return;
		}
		else
		{
			j = RTV1_PLC_CNT + 1 ;				// 6
			nRtvCount = RTV_PLC_TOT_CNT;		// 13	

			for ( ;j <= nRtvCount ; j++)
			{
				CLgvInfo* pInfo =  (CLgvInfo*)pRtv->GetInfo();

				if (pInfo == NULL)
					continue;

				pInfo->Initialize();
				pInfo->StatusReport();
				pInfo->InvokeControl();

//				CLgvVehicle* pVehicle = (CLgvVehicle*)pInfo->GetLgvVehicle(j-RTV1_PLC_CNT);
//
//				if (pVehicle == NULL)
//					continue;
//
//				pVehicle->SetLampInfo(FALSE, pInfo->m_pCvInfo, m_nNumber);
			}
			return;
		}
	}
	else
	{
		if (GetInfo() == NULL)
			return;

		GetInfo()->Initialize();
		GetInfo()->StatusReport();
		GetInfo()->InvokeControl();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINT CEquipment::ThreadProc(LPVOID pParam)
{
	CEquipment* pThis = (CEquipment*)pParam;

//	if (pThis->m_enKind != CEquipment::enRGV)
//		pThis->UpdateCommStatus(NOTIFY_ERROR);

//	pThis->UpdateCommStatus(NOTIFY_ERROR);
//	DEBUGER_ASSERT_VALID(pThis != NULL);

	CString strLog;
	strLog.Format(_T("%s THREAD 시작"), pThis->m_port.m_strDevice);
	DEBUGER_TRACE(strLog, _T("CEquipment::ThreadProc"));

	while (pThis->IsConnect())
	{
		switch (::WaitForMultipleObjects(enEventSize, pThis->m_hEventArray, FALSE, 400))
		{
		case enEventKill:
			strLog.Format(_T("\r\n %s THREAD 종료 (EVENT) \r\n"), pThis->m_port.m_strDevice);
			TRACE(strLog);
			return 0;

		case enEventSend:
			pThis->CommandProc();
			break;

		case WAIT_TIMEOUT:
			pThis->AutoRunProc();
			break;
		}

		::Sleep(100);
	}

	strLog.Format(_T("\r\n %s THREAD 종료 (END) \r\n"), pThis->m_port.m_strDevice);
	TRACE(strLog);

	return 0;	
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEquipment* CEquipmentArray::GetEquipment(CString& strDevice)
{
	DEBUGER_ASSERT_VALID_LOG(strDevice.GetLength() > 0, strDevice);

	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment == NULL)
			continue;

		if (pEquipment->m_port.m_strDevice == strDevice)
			return pEquipment;
	}

	return NULL;
}

CInfo* CEquipmentArray::GetInfo(int nEquipKind, int nEquipNum)
{
	CInfo* pInfo = NULL;
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment == NULL)
			continue;

		if (pEquipment->m_enKind != nEquipKind)
			continue;

		pInfo = pEquipment->GetInfo();

		if (pInfo == NULL)
			continue;
	}

	return NULL;
}

CEquipment* CEquipmentArray::GetEquipment(int nEquipKind, int nEquipNum)
{
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

//		if ((pEquipment->m_enKind == nEquipKind) && (pEquipment->m_nNumber == nEquipNum))
//			return pEquipment;

		if (pEquipment == NULL)
			continue;

//		if (nEquipKind == CEquipment::enRTV)
//		{
//			if (pEquipment->m_enKind == nEquipKind && ((CRtv*)pEquipment)->GetInfoIndex(nEquipNum) >= 0)
//				return pEquipment;
//		}
//		else
//		{
			if ((pEquipment->m_enKind == nEquipKind) && (pEquipment->m_nNumber == nEquipNum))
				return pEquipment;
//		}
	}

	return NULL;
}

void CEquipmentArray::SetAllSetSusspend()
{
	CEquipment* pEquipment = NULL;
	int j = 0;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment == NULL)
			continue;

		CCv*			pCv					= NULL;
		CScDongJin*		pSc					= NULL;
//		CRtv*			pRtv				= NULL;
//		CLifter*		pLifter				= NULL;
		CLgv*			pRtv				= NULL;
		
		CTrackHS*		pArriveHS			= NULL;
		CTrackInfo*		pTrack				= NULL;
		int				nHSCount			= 0;

		int j = 0;
		int nScCount = 0;
		int nRtvCount = 0;

		switch(pEquipment->m_enKind) 
		{
//		//보편적으로 CV는 작업정지 하지 않음!
/*
		case CEquipment::enCV:
			pCv = (CCv*) pEquipment;
			DEBUGER_ASSERT_VALID(pCv != NULL);
			
			// 트랙을 가져와야 하는데...
			if (pCv->IsConnect() == TRUE) 
			{
				int nTemp = pCv->m_pInfo->m_pTracks.GetSize();

				for (j=0; j<pCv->m_pInfo->m_pTracks.GetSize(); ++j)
				{
					pTrack = pCv->m_pInfo->m_pTracks[j];
					DEBUGER_ASSERT_VALID(pTrack != NULL);

					CCmdMsg*		pCvCmdMsg			= new CCmdMsg;						
					DEBUGER_ASSERT_VALID(pCvCmdMsg		!= NULL);
					
					pCvCmdMsg->m_nCommand = CCmdMsg::enCmdTrackSuspend;
					pCvCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeTrackSuspend);
					pCvCmdMsg->m_strValues[0].Format(_T("%d"), pTrack->m_nNumber);
					pCvCmdMsg->m_strValues[1].Format(_T("%d"), TRUE);
					pTrack->m_pEquipment->SetCmdMsg(pCvCmdMsg);
				}
			}

			break;
*/
		case CEquipment::enSC:
			pSc = (CScDongJin*) pEquipment;
			DEBUGER_ASSERT_VALID(pSc != NULL);

			if (pSc->IsConnect() == TRUE) 
			{
				j = 1 + (SC1_PLC_CNT * (pSc->m_nNumber - 1));					// 1	4
				nScCount = SC1_PLC_CNT * (pSc->m_nNumber - 1) + SC1_PLC_CNT;		// 3	6

				for ( ;j <= nScCount ; j++)
				{
//					CScInfo* pInfo =  (CScInfo*)pSc->GetInfo(j, FALSE);
//
//					if (pInfo == NULL)
//						continue;
//
					CScCmdMsg*		pScRetCmdMsg			= new CScCmdMsg;
					DEBUGER_ASSERT_VALID(pScRetCmdMsg		!= NULL);

					pScRetCmdMsg->m_nCommand = CScCmdMsg::enCmdRetSuspend;
					pScRetCmdMsg->m_nSubCmd = j;
					pScRetCmdMsg->m_strValues.SetSize(1);
					pScRetCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
					pSc->SetCmdMsg(pScRetCmdMsg);

					
					CScCmdMsg*		pScStoCmdMsg			= new CScCmdMsg;
					DEBUGER_ASSERT_VALID(pScStoCmdMsg		!= NULL);

					pScStoCmdMsg->m_nCommand = CScCmdMsg::enCmdStoSuspend;
					pScStoCmdMsg->m_nSubCmd = j;
					pScStoCmdMsg->m_strValues.SetSize(1);
					pScStoCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
					pSc->SetCmdMsg(pScStoCmdMsg);		
				}
			}

			break;
		case CEquipment::enRGV:
			pRtv = (CLgv*) pEquipment;
			DEBUGER_ASSERT_VALID(pRtv != NULL);

			if (pRtv->IsConnect() == TRUE) 
			{
				if (pRtv->m_nNumber == 1)
				{
					j = 1 ;								// 1
					nRtvCount = RTV1_PLC_CNT;			// 5	

					for ( ;j <= nRtvCount ; j++)
					{
						CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
						DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
						pCmdMsg->m_nCommand = CLgvCmdMsg::enCmdSuspend;
						pCmdMsg->m_strValues.SetSize(1);
						pCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
						pRtv->SetCmdMsg(pCmdMsg);
					}
				}
				else if (pRtv->m_nNumber == 2)
				{
					j = RTV1_PLC_CNT + 1 ;				// 6
					nRtvCount = RTV_PLC_TOT_CNT;		// 13	

					for ( ;j <= nRtvCount ; j++)
					{
						CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
						DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
						pCmdMsg->m_nCommand = CLgvCmdMsg::enCmdSuspend;
						pCmdMsg->m_strValues.SetSize(1);
						pCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
						pRtv->SetCmdMsg(pCmdMsg);
					}
				}
			}

			break;
			/*
		case CEquipment::enLIFTER:
			pLifter = (CLifter*) pEquipment;
			DEBUGER_ASSERT_VALID(pLifter != NULL);

			if (pLifter->IsConnect() == TRUE) 
			{
				CLifterCmdMsg*	pLifterCmdMsg		= new CLifterCmdMsg;
				DEBUGER_ASSERT_VALID(pLifterCmdMsg	!= NULL);

				pLifterCmdMsg->m_nCommand = CLifterCmdMsg::enCmdSuspend;
				pLifterCmdMsg->m_strValues.SetSize(1);
				pLifterCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
				pLifter->SetCmdMsg(pLifterCmdMsg);
			}

			break;
		case CEquipment::enLGV:
			pLgv = (CLgv*) pEquipment;
			DEBUGER_ASSERT_VALID(pLgv != NULL);

			if (pLgv->IsConnect() == TRUE) 
			{
				CLgvCmdMsg*		pLgvCmdMsg			= new CLgvCmdMsg;
				DEBUGER_ASSERT_VALID(pLgvCmdMsg		!= NULL);

				pLgvCmdMsg->m_nCommand = CLgvCmdMsg::enCmdSuspend;
				pLgvCmdMsg->m_strValues.SetSize(1);
				pLgvCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
				pLgv->SetCmdMsg(pLgvCmdMsg);
			}

			break;
			*/
		default:
			continue;
		}
	}
}

void CEquipmentArray::SetAllUnSetSusspend()
{
	CEquipment* pEquipment = NULL;
	int j = 0;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment == NULL)
			continue;

		CCv*			pCv					= NULL;
		CScDongJin*		pSc					= NULL;
//		CRtv*			pRtv				= NULL;
//		CLifter*		pLifter				= NULL;
		CLgv*			pRtv				= NULL;
		
		CTrackHS*		pArriveHS			= NULL;
		CTrackInfo*		pTrack				= NULL;
		int				nHSCount			= 0;

		int j = 0;
		int nScCount = 0;
		int nRtvCount = 0;

		switch(pEquipment->m_enKind) 
		{
//		//보편적으로 CV는 작업정지 하지 않음!
/*
		case CEquipment::enCV:
			pCv = (CCv*) pEquipment;
			DEBUGER_ASSERT_VALID(pCv != NULL);
			
			// 트랙을 가져와야 하는데...
			if (pCv->IsConnect() == TRUE) 
			{
				int nTemp = pCv->m_pInfo->m_pTracks.GetSize();

				for (j=0; j<pCv->m_pInfo->m_pTracks.GetSize(); ++j)
				{
					pTrack = pCv->m_pInfo->m_pTracks[j];
					DEBUGER_ASSERT_VALID(pTrack != NULL);

					CCmdMsg*		pCvCmdMsg			= new CCmdMsg;						
					DEBUGER_ASSERT_VALID(pCvCmdMsg		!= NULL);
					
					pCvCmdMsg->m_nCommand = CCmdMsg::enCmdTrackSuspend;
					pCvCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeTrackSuspend);
					pCvCmdMsg->m_strValues[0].Format(_T("%d"), pTrack->m_nNumber);
					pCvCmdMsg->m_strValues[1].Format(_T("%d"), FALSE);
					pTrack->m_pEquipment->SetCmdMsg(pCvCmdMsg);
				}
			}

			break;
*/
		case CEquipment::enSC:
			pSc = (CScDongJin*) pEquipment;
			DEBUGER_ASSERT_VALID(pSc != NULL);

			if (pSc->IsConnect() == TRUE) 
			{
				j = 1 + (SC1_PLC_CNT * (pSc->m_nNumber - 1));					// 1	4
				nScCount = SC1_PLC_CNT * (pSc->m_nNumber - 1) + SC1_PLC_CNT;		// 3	6

				for ( ;j <= nScCount ; j++)
				{
//					CScInfo* pInfo =  (CScInfo*)pSc->GetInfo(j, FALSE);
//
//					if (pInfo == NULL)
//						continue;
//
					CScCmdMsg*		pScRetCmdMsg			= new CScCmdMsg;
					DEBUGER_ASSERT_VALID(pScRetCmdMsg		!= NULL);

					pScRetCmdMsg->m_nCommand = CScCmdMsg::enCmdRetSuspend;
					pScRetCmdMsg->m_nSubCmd = j;
					pScRetCmdMsg->m_strValues.SetSize(1);
					pScRetCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
					pSc->SetCmdMsg(pScRetCmdMsg);

					
					CScCmdMsg*		pScStoCmdMsg			= new CScCmdMsg;
					DEBUGER_ASSERT_VALID(pScStoCmdMsg		!= NULL);

					pScStoCmdMsg->m_nCommand = CScCmdMsg::enCmdStoSuspend;
					pScStoCmdMsg->m_nSubCmd = j;
					pScStoCmdMsg->m_strValues.SetSize(1);
					pScStoCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
					pSc->SetCmdMsg(pScStoCmdMsg);		
				}
			}

			break;

		case CEquipment::enRGV:
			pRtv = (CLgv*) pEquipment;
			DEBUGER_ASSERT_VALID(pRtv != NULL);

			if (pRtv->IsConnect() == TRUE) 
			{
				if (pRtv->m_nNumber == 1)
				{
					j = 1 ;								// 1
					nRtvCount = RTV1_PLC_CNT;			// 5	

					for ( ;j <= nRtvCount ; j++)
					{
						CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
						DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
						pCmdMsg->m_nCommand = CLgvCmdMsg::enCmdSuspend;
						pCmdMsg->m_strValues.SetSize(1);
						pCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
						pRtv->SetCmdMsg(pCmdMsg);
					}
				}
				else if (pRtv->m_nNumber == 2)
				{
					j = RTV1_PLC_CNT + 1 ;				// 6
					nRtvCount = RTV_PLC_TOT_CNT;		// 13	

					for ( ;j <= nRtvCount ; j++)
					{
						CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
						DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
						pCmdMsg->m_nCommand = CLgvCmdMsg::enCmdSuspend;
						pCmdMsg->m_strValues.SetSize(1);
						pCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
						pRtv->SetCmdMsg(pCmdMsg);
					}
				}
			}

			break;

			/*
		case CEquipment::enLIFTER:
			pLifter = (CLifter*) pEquipment;
			DEBUGER_ASSERT_VALID(pLifter != NULL);

			if (pLifter->IsConnect() == TRUE) 
			{
				CLifterCmdMsg*	pLifterCmdMsg		= new CLifterCmdMsg;
				DEBUGER_ASSERT_VALID(pLifterCmdMsg	!= NULL);

				pLifterCmdMsg->m_nCommand = CLifterCmdMsg::enCmdSuspend;
				pLifterCmdMsg->m_strValues.SetSize(1);
				pLifterCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
				pLifter->SetCmdMsg(pLifterCmdMsg);
			}

			break;
		case CEquipment::enLGV:
			pLgv = (CLgv*) pEquipment;
			DEBUGER_ASSERT_VALID(pLgv != NULL);

			if (pLgv->IsConnect() == TRUE) 
			{
				CLgvCmdMsg*		pLgvCmdMsg			= new CLgvCmdMsg;
				DEBUGER_ASSERT_VALID(pLgvCmdMsg		!= NULL);

				pLgvCmdMsg->m_nCommand = CLgvCmdMsg::enCmdSuspend;
				pLgvCmdMsg->m_strValues.SetSize(1);
				pLgvCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
				pLgv->SetCmdMsg(pLgvCmdMsg);
			}

			break;
			*/
		default:
			continue;
		}
	}
}

void CEquipmentArray::ReconnectServers()
{
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment == NULL)
			continue;
	
		if (pEquipment->m_enKind == CEquipment::enRGV)	
//		if (pEquipment->m_enKind == CEquipment::enCV && pEquipment->m_nNumber == 1)	
		{
			// (!m_pThreadSocket && m_port.m_bRetry)
			if (pEquipment->IsConnect() == FALSE && pEquipment->m_port.m_bRetry == TRUE)	
				pEquipment->ConnectServer();
		}
		else
		{
			if (pEquipment->IsConnect() == FALSE)	
				pEquipment->ReconnectServer();
		}
	}
}

void CEquipmentArray::StatusReport()
{
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);
//		DEBUGER_ASSERT_VALID(pEquipment->GetInfo() != NULL);

		if (pEquipment == NULL)
			continue;

		if (pEquipment->m_enKind == CEquipment::enSC  )
		{
			CScDongJin* pSc = (CScDongJin*)pEquipment;

			if (pSc==NULL)
				continue;

			for (int i=0;i<SC1_PLC_CNT;i++) 
			{
				
				CScInfo* pInfo = (CScInfo*)pSc->GetInfo(i, FALSE);
				
				if (pInfo==NULL)
					continue;

				pInfo->StatusReport();
			}
		}
		/*
		// RGV는 상태를 보고할 필요없음!
		else if (pEquipment->m_enKind == CEquipment::enRGV)
		{
			CLgv* pRgv = (CLgv*) pEquipment;

			if (pRgv == NULL)
				continue;

			CLgvInfo* pInfo = (CLgvInfo*)pSc->GetInfo();

			for (int i=0;i<SC1_PLC_CNT;i++) 
			{
				
				CLgvInfo* pInfo = (CLgvInfo*)pSc->GetInfo(i, FALSE);
				
				if (pInfo==NULL)
					continue;

				pInfo->StatusReport();
			}
		}
		*/
		else
		{
			pEquipment->GetInfo()->StatusReport();
		}
	}
}

BOOL CEquipmentArray::IsIdleJob(int nLuggNum)
{
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment == NULL)
			continue;

		if (pEquipment->IsIdleJob(nLuggNum) == FALSE)
			return FALSE;
	}

	return TRUE;
}

void CEquipmentArray::MulticastInfo(CMonitorServer* pMonitorSv)
{
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment == NULL)
			continue;

		pEquipment->MulticastInfo(pMonitorSv);
	}
}

void CEquipmentArray::InvokeControl(BOOL bManual)
{
	CInfo* pInfo = NULL;
	CEquipment* pEquipment = NULL;
	int j=0;
	int nScCount=0;
	int nRtvCount=0;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment == NULL)
			continue;

		if (pEquipment->m_enKind == CEquipment::enSC )
		{
			if (pEquipment->m_nNumber == 1)
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
				CScInfo* pInfo =  (CScInfo*)pEquipment->GetInfo();

				if (pInfo == NULL)
					continue;

				if (bManual)
					pInfo->m_bModified = TRUE;

				pInfo->InvokeControl();
			}
		}
		else if (pEquipment->m_enKind == CEquipment::enRGV )
		{
			if (pEquipment->m_nNumber == 1)
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
				CLgvInfo* pInfo =  (CLgvInfo*)pEquipment->GetInfo();

				if (pInfo == NULL)
					continue;

				if (bManual)
					pInfo->m_bModified = TRUE;

				pInfo->InvokeControl();
			}
		}
		else
		{
			pInfo = pEquipment->GetInfo();
		//	DEBUGER_ASSERT_VALID(pInfo != NULL);

			if (pInfo == NULL)
				continue;

			if (bManual)
				pInfo->m_bModified = TRUE;

			pInfo->InvokeControl();
		}

	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CEquipmentArray::WritePlayBack()
{
	CString strTxBuff;

	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

//		if(pEquipment->m_pDoc->m_pLogPlayback == NULL)
//			return;

		CString strTxData;

		if (pEquipment->m_enKind == CEquipment::enCV)
		{
			CCv*			pCv					= NULL;
			CTrackInfo*		pTrack				= NULL;
		
			pCv = (CCv*) pEquipment;
			DEBUGER_ASSERT_VALID(pCv != NULL);
			pCv->MulticastInfo(NULL);

			int nTemp = pCv->m_pInfo->m_pTracks.GetSize();

			for (int j=0; j<pCv->m_pInfo->m_pTracks.GetSize(); ++j)
			{
				pTrack = pCv->m_pInfo->m_pTracks[j];
				DEBUGER_ASSERT_VALID(pTrack != NULL);

				pTrack->MulticastInfo();
			}

		}
		else
		{
			pEquipment->MulticastInfo(NULL);		
		}

		CSc*			pSc					= NULL;
		CRtv*			pRtv				= NULL;
		CLifter*		pLifter				= NULL;
		CLgv*			pLgv				= NULL;
		CBcr*			pBcr				= NULL;
		CRfid*			pRfid				= NULL;
		CDisplay*		pDisplay			= NULL;
		

		switch(pEquipment->m_enKind) 
		{
		case CEquipment::enCV:
			pCv = (CCv*) pEquipment;
			DEBUGER_ASSERT_VALID(pCv != NULL);
			
			// 트랙을 가져와야 하는데...
			if (pCv->IsConnect() == TRUE) 
			{
				CString strTemp;
				strTemp.Format("%c<ECS><CV>", STX);
				int nTemp = pCv->m_pInfo->m_pTracks.GetSize();

				for (int j=0; j<pCv->m_pInfo->m_pTracks.GetSize(); ++j)
				{
					pTrack = pCv->m_pInfo->m_pTracks[j];
					DEBUGER_ASSERT_VALID(pTrack != NULL);

					strTemp += pTrack->GetXmlString();
				}
				strTxBuff.Format("%s</CV></ECS>%c", strTemp, ETX);
				pEquipment->m_pDoc->m_pLogPlayback->Log(strTxBuff);
			}

			break;
		case CEquipment::enSC:
			pSc = (CSc*) pEquipment;
			DEBUGER_ASSERT_VALID(pSc != NULL);

			if (pSc->IsConnect() == TRUE) 
			{
				strTxData += pSc->m_pInfo->GetXmlString();
//				strTxBuff.Format("%c<ECS><SC>%s</SC></ECS>%c", STX, strTxData, ETX);
				strTxBuff.Format(_T("%c<ECS><EQUIP DEVICE='%s'>%s</EQUIP></ECS>%c"), 
					CMonitorServer::enSTX, pEquipment->m_port.m_strDevice, pSc->m_pInfo->GetXmlString(), CMonitorServer::enETX);
				pEquipment->m_pDoc->m_pLogPlayback->Log(strTxBuff);
			}

			break;
		case CEquipment::enRTV:
			pRtv = (CRtv*) pEquipment;
			DEBUGER_ASSERT_VALID(pRtv != NULL);

			if (pRtv->IsConnect() == TRUE) 
			{
				strTxData += pRtv->m_pInfo->GetXmlString();
				strTxBuff.Format("%c<ECS><RTV>%s</RTV></ECS>%c", STX, strTxData, ETX);
				pEquipment->m_pDoc->m_pLogPlayback->Log(strTxBuff);
			}

			break;
		case CEquipment::enLIFTER:
			pLifter = (CLifter*) pEquipment;
			DEBUGER_ASSERT_VALID(pLifter != NULL);

			if (pLifter->IsConnect() == TRUE) 
			{
				strTxData += pLifter->m_pInfo->GetXmlString();
				strTxBuff.Format("%c<ECS><LIFTER>%s</LIFTER></ECS>%c", STX, strTxData, ETX);
				pEquipment->m_pDoc->m_pLogPlayback->Log(strTxBuff);
			}

			break;
			
//		case CEquipment::enLGV:
//			pLgv = (CLgv*) pEquipment;
//			DEBUGER_ASSERT_VALID(pLgv != NULL);
//
//			if (pLgv->IsConnect() == TRUE) 
//			{
//				strTxData += pLgv->m_pInfo->GetXmlString();
//				strTxBuff.Format("%c<ECS><LGV>%s</LGV></ECS>%c", STX, strTxData, ETX);
//				pEquipment->m_pDoc->m_pLogPlayback->Log(strTxBuff);
//			}
//
//			break;
//			

		case CEquipment::enBCR:
			pBcr = (CBcr*) pEquipment;
			DEBUGER_ASSERT_VALID(pBcr != NULL);

			if (pBcr->IsConnect() == TRUE) 
			{
				strTxData += pBcr->m_pInfo->GetXmlString();
				strTxBuff.Format("%c<ECS><BCR>%s</BCR></ECS>%c", STX, strTxData, ETX);
				pEquipment->m_pDoc->m_pLogPlayback->Log(strTxBuff);
			}

			break;
		case CEquipment::enRFID:
			pRfid = (CRfid*) pEquipment;
			DEBUGER_ASSERT_VALID(pRfid != NULL);

			if (pRfid->IsConnect() == TRUE) 
			{
				strTxData += pRfid->m_pInfo->GetXmlString();
				strTxBuff.Format("%c<ECS><RFID>%s</RFID></ECS>%c", STX, strTxData, ETX);
				pEquipment->m_pDoc->m_pLogPlayback->Log(strTxBuff);
			}

			break;
		case CEquipment::enDISPLAY:
			pDisplay = (CDisplay*) pEquipment;
			DEBUGER_ASSERT_VALID(pDisplay != NULL);

			if (pDisplay->IsConnect() == TRUE) 
			{
				strTxData += pDisplay->m_pInfo->GetXmlString();
				strTxBuff.Format("%c<ECS><DISPLAY>%s</DISPLAY></ECS>%c", STX, strTxData, ETX);
				pEquipment->m_pDoc->m_pLogPlayback->Log(strTxBuff);
			}

			break;
		default:
			continue;
		}
	}
}
*/
