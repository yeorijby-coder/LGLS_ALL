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

	m_nIndex = nIndex;
	m_nNumber = nNumber;
	m_enKind = enNone;
	m_port.m_strDevice = lpszDevice;

	m_pEquipDlg = NULL;

	for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
	{
		m_bThreadOut[i] = FALSE;

		m_pSocket[i] = NULL;
		m_pThread[i] = NULL;
		m_pListener[i] = NULL;
		m_pfThreadProc[i] = CEquipment::ThreadProc;
		m_hEventArray[i][enEventKill] = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		m_hEventArray[i][enEventSend] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	}		// 
	m_hEventSerialArray[enEventKill] = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventSerialArray[enEventSend] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_pSerialComm = NULL;
	m_pSerialThread = NULL;
	m_pfThreadSerialProc = CEquipment::ThreadSerialProc;
}

CEquipment::~CEquipment()
{
	for (POSITION pos=m_cmdList.GetHeadPosition(); pos!=NULL; )
		delete m_cmdList.GetNext(pos);
	m_cmdList.RemoveAll();

	for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
	{
		::CloseHandle(m_hEventArray[i][enEventSend]);	// ??
		::CloseHandle(m_hEventArray[i][enEventKill]);

		if (m_pListener[i] != NULL)
			delete m_pListener[i];

		if (m_pSocket[i] != NULL)
			delete m_pSocket[i];
	}

	::CloseHandle(m_hEventSerialArray[enEventSend]);	// ??
	::CloseHandle(m_hEventSerialArray[enEventKill]);

	if (m_pSerialComm != NULL)
		delete m_pSerialComm;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEquipment::SetCmdMsg(CCmdMsg* pCmdMsg)
{
	if (pCmdMsg == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	TRY
	{
		if (m_enKind == enCV)
			int a=0;

		if (m_enKind == enSC)
			int b=0;

		m_cmdList.AddTail(pCmdMsg);
	}
	CATCH (CException, e)
	{
		m_strLog.Format(_T("%s [%s]"), DEVICE, CLib::GetExceptionString(e));
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::SetCmdMsg"));
		return;
	}
	END_CATCH

	//::SetEvent(m_hEventArray[enEventSend]);
}

void CEquipment::UpdateCommStatus(int nStatus, int nNum)
{
	UINT uMsg;
	switch (m_enKind)
	{
	case enCV:		uMsg = WM_USER_CV_NOTIFY;		break;
	case enSC:		uMsg = WM_USER_SC_NOTIFY;		break;
	case enRTV:		uMsg = WM_USER_RTV_NOTIFY;		break;
	case enLGV:		uMsg = WM_USER_LGV_NOTIFY;		break;
	case enLIFTER:	uMsg = WM_USER_LIFTER_NOTIFY;	break;
	case enRFID:	uMsg = WM_USER_RFID_NOTIFY;		break;
	case enBCR:		uMsg = WM_USER_BCR_NOTIFY;		break;
	case enDISPLAY:	uMsg = WM_USER_DISPLAY_NOTIFY;	break;
	case enTHS:		uMsg = WM_USER_THS_NOTIFY;		break;
	default:		return;
	}

//	if (m_pDoc && m_pDoc->m_hWndView)
//		::PostMessage(m_pDoc->m_hWndView, uMsg, m_nNumber-1, nStatus);
}

void CEquipment::WriteLog(int nLogType, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszPos, LPCTSTR lpszProdID /* = NULL */, BOOL bAlarm /* = FALSE */)
{
	int nLogPos;
	switch (m_enKind)
	{
	case enCV:		nLogPos = LOG_POS_CV;		break;
	case enSC:		nLogPos = LOG_POS_SC;		break;
	case enRTV:		nLogPos = LOG_POS_RTV;		break;
	case enLIFTER:	nLogPos = LOG_POS_LIFTER;	break;
	case enLGV:		nLogPos = LOG_POS_LGV;		break;
	case enRFID:	nLogPos = LOG_POS_RFID;		break;
	case enBCR:		nLogPos = LOG_POS_BCR;		break;
	case enDISPLAY:	nLogPos = LOG_POS_DISPLAY;	break;
	case enTHS:		nLogPos = LOG_POS_THS;		break;
	default:		nLogPos = LOG_POS_SYSTEM;	break;
	}

	m_pDoc->WriteLog(nLogType, nLogPos, nLuggNum, lpszContent, lpszPos, lpszProdID, bAlarm);
}

void CEquipment::WriteLog(int nLogType, LPCTSTR lpszContent, LPCTSTR lpszPos, CJobItem* pJobItem/* = NULL*/, LPCTSTR lpszProdID/* = NULL*/, BOOL bAlarm/* = FALSE*/)
{
	if (pJobItem == NULL) 
		WriteLog(nLogType, LOG_SYSTEM, lpszContent, lpszPos, lpszProdID, bAlarm);
	else 
		WriteLog(nLogType, pJobItem->m_nLuggNum, lpszContent, lpszPos, pJobItem->m_strBarcode, bAlarm);
}

void CEquipment::RefreshDialog(WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	if (m_pEquipDlg && m_pEquipDlg->GetSafeHwnd())
		m_pEquipDlg->PostMessage(WM_USER_REFRESH_NOTIFY, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CEquipment::IsConnect(int nConnNum)
{
	if ((m_pSocket[nConnNum] == NULL) || (m_pSocket[nConnNum]->m_enCommStatus != CInterfaceSk::enStatusConnectOK))
		return FALSE;

	return TRUE;
}

BOOL CEquipment::ConnectServer(int nIndex)
{
	if (m_pSocket[nIndex])
	{
		switch (m_pSocket[nIndex]->m_enCommStatus)
		{
		case CInterfaceSk::enStatusConnectOK:
			m_strLog.Format(_T("%s 이미 연결되어 있습니다."), DEVICE);
			break;

		case CInterfaceSk::enStatusConnecting:
			m_strLog.Format(_T("%s 연결 요청중입니다."), DEVICE);
			break;
		}

		m_pDoc->StatusDisplay(m_strLog, FALSE);
		return FALSE;
	}

	if ((m_pSocket[nIndex] = CreateSocket(nIndex)) == NULL)
	{
		m_strLog.Format(_T("%s SOCKET 생성 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::ConnectServer"), NULL, TRUE);
		return FALSE;
	}

	if (!m_pSocket[nIndex]->Create())
	{
		m_strLog.Format(_T("%s 연결요청 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::ConnectServer"), NULL, TRUE);

		delete m_pSocket[nIndex];
		m_pSocket[nIndex] = NULL;

		return FALSE;
	}

	m_pSocket[nIndex]->m_enCommStatus = CInterfaceSk::enStatusConnecting;
	m_pSocket[nIndex]->Connect(m_port.m_strIP, m_port.m_nPort[nIndex]);

	return TRUE;
}

void CEquipment::OnConnectSocket(int nErrorCode, int nIndex)
{
	CInfo* pInfo = GetInfo();
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	if (nErrorCode)
	{
		m_strLog.Format(_T("%s 접속 실패 [%s]"), DEVICE, CLib::GetSystemErrMsg(nErrorCode));
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"), NULL, TRUE);

		UpdateCommStatus(NOTIFY_ERROR);
		m_pSocket[nIndex]->m_enCommStatus = CInterfaceSk::enStatusConnectFail;
		m_pSocket[nIndex]->ShutDown();
		m_pSocket[nIndex] = NULL;
//		delete m_pSocket[nIndex];

		pInfo->Initialize();
	}
	else
	{
		m_bThreadOut[nIndex] = FALSE;
		m_strLog.Format(_T("%s 접속 성공 [%s]"), DEVICE, m_pSocket[nIndex]->GetPeerInfo());
		WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnConnectSocket"));

		UpdateCommStatus(NOTIFY_SEND);
		m_pSocket[nIndex]->m_enCommStatus = CInterfaceSk::enStatusConnectOK;

		::ResetEvent(m_hEventArray[nIndex][enEventKill]);
		m_pThread[nIndex] = ::AfxBeginThread(m_pfThreadProc[nIndex], this);
		DEBUGER_ASSERT_VALID(m_pThread != NULL);
	}

	pInfo->m_bModified = TRUE;
	pInfo->InvokeControl();
}

BOOL CEquipment::RunServer(int nIndex)
{
	DEBUGER_ASSERT_VALID(m_port.IsKindOf(RUNTIME_CLASS(CPortEthernet)));

	CInfo* pInfo = GetInfo();
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	pInfo->m_bModified = TRUE;
	pInfo->InvokeControl();

	if (m_pListener[nIndex])
	{
		m_strLog.Format(_T("%s 리스너가 이미 동작하고 있습니다."), DEVICE);
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::RunServer"), NULL, TRUE);
		return FALSE;
	}

	m_pListener[nIndex] = new CListenerSk(this, nIndex);
	DEBUGER_ASSERT_VALID(m_pListener[nIndex] != NULL);

	if (m_pListener[nIndex] == NULL || !m_pListener[nIndex]->Create(m_port.m_nPort[nIndex]))
	{
		m_strLog.Format(_T("%s 리스너 SOCKET 생성 실패! [%s][%d]"), DEVICE, CLib::GetSystemErrMsg(), m_port.m_nPort[nIndex]);
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::RunServer"), NULL, TRUE);
		return FALSE;
	}

	if (!m_pListener[nIndex]->Listen())
	{
		m_strLog.Format(_T("%s 리스너 LISTEN 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::RunServer"), NULL, TRUE);
		return FALSE;
	}

	m_strLog.Format(_T("%s 서버 실행! [PORT=%d]"), DEVICE, m_port.m_nPort[nIndex]);
	WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::RunServer"));

	return TRUE;
}

void CEquipment::RunThread()
{
	for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
	{
		::ResetEvent(m_hEventArray[i][enEventKill]);
		m_pThread[i] = ::AfxBeginThread(m_pfThreadProc[i], this);
	}

	DEBUGER_ASSERT_VALID(m_pThread != NULL);
}

void CEquipment::RunSerialThread()
{
	::ResetEvent(m_hEventSerialArray[enEventKill]);
	m_pSerialThread = ::AfxBeginThread(m_pfThreadSerialProc, this);

	DEBUGER_ASSERT_VALID(m_pSerialThread != NULL);
}

void CEquipment::KillThread()
{
	for (int i = 0; i < PLC_CONN_PORT_CNT ; i++)
	{
		if (m_pThread[i])
		{
			DWORD dwExitCode = 0;
			if (::GetExitCodeThread(m_pThread[i]->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
			{
				m_pThread[i]->m_bAutoDelete = FALSE;

				::SetEvent(m_hEventArray[i][enEventKill]);
				//::SetEvent(m_pSocket1->m_hRecvEvent);
			
				if (::WaitForSingleObject(m_pThread[i]->m_hThread, m_pDoc->m_pConfig->m_dwExitTimeout) == WAIT_TIMEOUT)
				{
					if (::TerminateThread(m_pThread[i]->m_hThread, dwExitCode))
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

				delete m_pThread[i];
				m_pThread[i] = NULL;
			}
		}
	}
}

void CEquipment::KillSerialThread()
{
	if (m_pSerialThread)
	{
		DWORD dwExitCode = 0;
		if (::GetExitCodeThread(m_pSerialThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
		{
			m_pSerialThread->m_bAutoDelete = FALSE;

			::SetEvent(m_hEventSerialArray[enEventKill]);

			if (::WaitForSingleObject(m_pSerialThread->m_hThread, m_pDoc->m_pConfig->m_dwExitTimeout) == WAIT_TIMEOUT)
			{
				if (::TerminateThread(m_pSerialThread->m_hThread, dwExitCode))
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

			delete m_pSerialThread;
			m_pSerialThread = NULL;
		}
	}
}

void CEquipment::OnAcceptSocket(int nErrorCode, int nIndex)
{
	if (nErrorCode)
	{
		UpdateCommStatus(NOTIFY_ERROR, nIndex);
		m_strLog.Format(_T("%s SOCKET ERROR [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"), NULL, TRUE);
		return;
	}

	if (m_pSocket[nIndex] != NULL)
	{
		m_pSocket[nIndex]->OnClose(0);
		DEBUGER_ASSERT_VALID(m_pSocket[nIndex] == NULL);

		m_strLog.Format(_T("%s-01 신규 연결요청으로 인하여 현재 연결된 CLIENT 접속을 종료"), DEVICE);
		WriteLog(LOG_TYPE_DEBUG, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"), NULL, TRUE);
	}

	if ((m_pSocket[nIndex] = CreateSocket(nIndex)) == NULL)
	{
		UpdateCommStatus(NOTIFY_ERROR, nIndex);
		m_strLog.Format(_T("%s SOCKET 생성 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg());
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"), NULL, TRUE);
		return;
	}

	if (!m_pListener[nIndex]->Accept(*m_pSocket[nIndex]))
	{
		delete m_pSocket[nIndex];
		m_pSocket[nIndex] = NULL;

		UpdateCommStatus(NOTIFY_ERROR, nIndex);
		m_strLog.Format(_T("%s ACCEPT 실패! [%s]"), DEVICE, CLib::GetSystemErrMsg(nErrorCode));
		WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"), NULL, TRUE);
		return;
	}

	CString strPeerIp = m_pSocket[nIndex]->GetPeerIP();
	int nPeerPort = m_pSocket[nIndex]->GetPeerPort();

	m_pSocket[nIndex]->m_enCommStatus = CInterfaceSk::enStatusConnectOK;
	UpdateCommStatus(NOTIFY_SEND, nIndex);

	DEBUGER_ASSERT_VALID(GetInfo() != NULL);
	GetInfo()->m_bModified = TRUE;
	GetInfo()->InvokeControl();

	m_strLog.Format(_T("%s CLIENT 접속 성공! [%s]"), DEVICE, m_pSocket[nIndex]->GetPeerInfo());
	WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnAcceptSocket"));

	m_bThreadOut[nIndex] =  FALSE;
}

void CEquipment::Disconnect(int nIndex)
{
	// default 값이거나 인덱스가 소켓보다 많을 경우는 모든 소켓 통신종료
	if (nIndex == -1 || nIndex >= PLC_CONN_PORT_CNT)
	{
		for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
		{
			if (m_pSocket[i])
				m_pSocket[i]->OnClose(0);	
		}
	}
	else 
	{
		if (m_pSocket[nIndex])
			m_pSocket[nIndex]->OnClose(0);	
	}
}

void CEquipment::OnCloseSocket(int nErrorCode, int nIndex)
{
	DEBUGER_ASSERT_VALID(m_pSocket[nIndex] != NULL);
	
	UpdateCommStatus(NOTIFY_ERROR, 0);

	if (m_pThread[nIndex])
	{
		m_bThreadOut[nIndex] =  TRUE;
		DWORD dwExitCode = 0;
		if (::GetExitCodeThread(m_pThread[nIndex]->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
		{
			m_pThread[nIndex]->m_bAutoDelete = FALSE;

			::SetEvent(m_hEventArray[nIndex][enEventKill]);
			::SetEvent(m_pSocket[nIndex]->m_hRecvEvent);
			
			if (::WaitForSingleObject(m_pThread[nIndex]->m_hThread, m_pDoc->m_pConfig->m_dwExitTimeout) == WAIT_TIMEOUT)
			{
				if (::TerminateThread(m_pThread[nIndex]->m_hThread, dwExitCode))
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
				::SetEvent(m_hEventArray[nIndex][enEventKill]);
				m_strLog.Format(_T("%s THREAD 정상종료"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseSocket"));
			}

			delete m_pThread[nIndex];
			m_pThread[nIndex] = NULL;
		}

		if (m_enKind == CEquipment::enSC)
		{
			RunThread();
		}
	}

	m_strLog.Format(_T("%s 연결해제 [%s]"), DEVICE, CLib::GetSystemErrMsg(nErrorCode));
	WriteLog(LOG_TYPE_EVENT, LOG_SYSTEM, m_strLog, _T("CEquipment::OnCloseSocket"));
	
	m_pSocket[nIndex]->m_enCommStatus = CInterfaceSk::enStatusClose;
	m_pSocket[nIndex]->ShutDown();
	m_pSocket[nIndex] = NULL;

	DEBUGER_ASSERT_VALID(GetInfo() != NULL);
	GetInfo()->Initialize();
	GetInfo()->StatusReport();
	GetInfo()->InvokeControl();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINT CEquipment::ThreadProc(LPVOID pParam)
{
	CEquipment* pThis = (CEquipment*)pParam;
	DEBUGER_ASSERT_VALID(pThis != NULL);

	CString strLog;
	strLog.Format(_T("%s THREAD 시작"), pThis->m_port.m_strDevice);
	DEBUGER_TRACE(strLog, _T("CEquipment::ThreadProc"));

	while (TRUE)
	{
		for(int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
		{			
			switch (::WaitForMultipleObjects(enEventSize, pThis->m_hEventArray[i], FALSE, 400))
			{
			case enEventKill:
				strLog.Format(_T("\r\n %s THREAD 종료 (EVENT) \r\n"), pThis->m_port.m_strDevice);
				TRACE(strLog);
				return 0;

			case WAIT_TIMEOUT:
				if (pThis->m_pSocket[i] != NULL)
					pThis->AutoRunProc(i);		//i
				break;

			default:
				//pThis->AutoRunProc(i);
				break;
			}
		}

		::Sleep(100);
	}

	strLog.Format(_T("\r\n %s THREAD 종료 (END) \r\n"), pThis->m_port.m_strDevice);
	TRACE(strLog);

	return 0;	
}

UINT CEquipment::ThreadSerialProc(LPVOID pParam)
{
	CEquipment* pThis = (CEquipment*)pParam;
	DEBUGER_ASSERT_VALID(pThis != NULL);

	CString strLog;
	strLog.Format(_T("%s THREAD 시작"), pThis->m_port.m_strDevice);
	DEBUGER_TRACE(strLog, _T("CEquipment::ThreadSerialProc"));

	while (TRUE)
	{
	//	for (int i = 0; i < PLC_CONN_PORT_CNT; i++)
	//	{
			switch (::WaitForMultipleObjects(enEventSize, pThis->m_hEventSerialArray, FALSE, 400))
			{
			case enEventKill:
				strLog.Format(_T("\r\n %s THREAD 종료 (EVENT) \r\n"), pThis->m_port.m_strDevice);
				TRACE(strLog);
				return 0;

			case WAIT_TIMEOUT:
				//if (pThis->m_pSocket[i] != NULL)
					pThis->AutoRunProc(0);		//i
				break;

			default:
				//pThis->AutoRunProc(i);
				break;
			}
	//	}

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

		if (pEquipment->m_port.m_strDevice == strDevice)
			return pEquipment;
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

		if(pEquipment->m_enKind != nEquipKind)
			continue;

		if ((pEquipment->m_enKind == nEquipKind) && (pEquipment->m_nNumber == nEquipNum))
			return pEquipment;
	}

	return NULL;
}
/*
CTrackInfo* CEquipmentArray::GetDeadLockTrack(int nTrackNum)
{
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

//		if ((pEquipment->m_enKind == nEquipKind) && (pEquipment->m_nNumber == nEquipNum))
//			return pEquipment;

		CCv*		pCv			= NULL;
		CSc*		pSc			= NULL;
		CTrackHS*	pArriveHS	= NULL;
		int			nHSCount	= 0;
		int			i			= 0;

		switch(pEquipment->m_enKind) {
		// 트랙 정보를 가져오는 것이므로 CV는 할 수 있는 것이 없음!
		case CEquipment::enCV:
			break;
		case CEquipment::enSC:
			pSc = (CSc*) pEquipment;
			DEBUGER_ASSERT_VALID(pSc != NULL);
			nHSCount = pSc->m_pInfo->m_pRetsHS.GetSize();

			for (i=0; i<pSc->m_pInfo->m_pRetsHS.GetSize(); ++i)
			{
				pArriveHS = pSc->m_pInfo->m_pRetsHS.GetNextTrackHS();
				DEBUGER_ASSERT_VALID(pArriveHS != NULL);
				DEBUGER_ASSERT_VALID(pArriveHS->m_pTrack != NULL);

				if(nTrackNum == pArriveHS->m_pTrack->m_nNumber)
				{
					if(pArriveHS->m_bDeadLockTrack == TRUE)
					{
						return pArriveHS->m_pTrack;
					}
				}
			}
			break;
		// 그외의 설비들은 DeadLock에 관여하지 않음
		default:
			continue;
		}
	}

	return NULL;
}
//*/

void CEquipmentArray::SetAllSetSusspend()
{
	CEquipment* pEquipment = NULL;
	int j = 0;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		CCv*		pCv			= NULL;
//		CBcr*		pBcr		= NULL;
		CSc*		pSc			= NULL;
		
		CTrackHS*	pArriveHS	= NULL;
		CTrackInfo*	pTrack		= NULL;
		int			nHSCount	= 0;

		switch(pEquipment->m_enKind) 
		{
		//보편적으로 CV는 작업정지 하지 않음!
		case CEquipment::enCV:
			pCv = (CCv*) pEquipment;
			DEBUGER_ASSERT_VALID(pCv != NULL);
			
			// 트랙을 가져와야 하는데...
			if (pCv->IsConnect(0) == TRUE) 
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
//					pCvCmdMsg->m_strValues[2].Format(_T("%d"), pTrack->m_nCvPlcNum);
					pTrack->m_pEquipment->SetCmdMsg(pCvCmdMsg);
				}
			}

			break;

		//case CEquipment::enBCR:
		//	pBcr = (CBcr*) pEquipment;
		//	DEBUGER_ASSERT_VALID(pBcr != NULL);
		//	if (pBcr->IsConnect(0) == TRUE) 
		//	{
		//		for (j = 0; j <= pBcr->m_pInfo.GetUpperBound(); ++j)
		//		{
		//			CBcrCmdMsg* pCmdMsg = new CBcrCmdMsg;
		//			DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
		//			pCmdMsg->m_nCommand = CBcrCmdMsg::enCmdSuspend;
		//			pCmdMsg->m_nSubCmd = j;
		//			pCmdMsg->m_strValues.SetSize(1);
		//			pCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
		//			pBcr->SetCmdMsg(pCmdMsg);
		//		}
		//	}
		//	break;

		case CEquipment::enSC:
			pSc = (CSc*) pEquipment;
			DEBUGER_ASSERT_VALID(pSc != NULL);

			if (pSc->IsConnect(0) == TRUE) 
			{
				CScCmdMsg*		pScRetCmdMsg			= new CScCmdMsg;
				DEBUGER_ASSERT_VALID(pScRetCmdMsg		!= NULL);

				pScRetCmdMsg->m_nCommand = CScCmdMsg::enCmdRetSuspend;
				pScRetCmdMsg->m_strValues.SetSize(1);
				pScRetCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
				pSc->SetCmdMsg(pScRetCmdMsg);


				
				CScCmdMsg*		pScStoCmdMsg			= new CScCmdMsg;
				DEBUGER_ASSERT_VALID(pScStoCmdMsg		!= NULL);

				pScStoCmdMsg->m_nCommand = CScCmdMsg::enCmdStoSuspend;
				pScStoCmdMsg->m_strValues.SetSize(1);
				pScStoCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
				pSc->SetCmdMsg(pScStoCmdMsg);
			}

			break;

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

		CCv*		pCv			= NULL;
//		CBcr*		pBcr		= NULL;
		CSc*		pSc			= NULL;
		
		CTrackHS*	pArriveHS	= NULL;
		CTrackInfo*	pTrack		= NULL;
		int			nHSCount	= 0;

		switch(pEquipment->m_enKind) 
		{
		//보편적으로 CV는 작업정지 하지 않음!
		case CEquipment::enCV:
			pCv = (CCv*) pEquipment;
			DEBUGER_ASSERT_VALID(pCv != NULL);
			
			// 트랙을 가져와야 하는데...
			if (pCv->IsConnect(0) == TRUE) 
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
//					pCvCmdMsg->m_strValues[2].Format(_T("%d"), pTrack->m_nCvPlcNum);
					pTrack->m_pEquipment->SetCmdMsg(pCvCmdMsg);
				}
			}

			break;

		//case CEquipment::enBCR:
		//	pBcr = (CBcr*) pEquipment;
		//	DEBUGER_ASSERT_VALID(pBcr != NULL);
		//	if (pBcr->IsConnect(0) == TRUE) 
		//	{
		//		for (j = 0; j <= pBcr->m_pInfo.GetUpperBound(); ++j)
		//		{
		//			CBcrCmdMsg* pCmdMsg = new CBcrCmdMsg;
		//			DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
		//			pCmdMsg->m_nCommand = CBcrCmdMsg::enCmdSuspend;
		//			pCmdMsg->m_nSubCmd = j;
		//			pCmdMsg->m_strValues.SetSize(1);
		//			pCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
		//			pBcr->SetCmdMsg(pCmdMsg);
		//		}
		//	}
		//	break;

		case CEquipment::enSC:
			pSc = (CSc*) pEquipment;
			DEBUGER_ASSERT_VALID(pSc != NULL);

			if (pSc->IsConnect(0) == TRUE) 
			{
				CScCmdMsg*		pScRetCmdMsg			= new CScCmdMsg;
				DEBUGER_ASSERT_VALID(pScRetCmdMsg		!= NULL);

				pScRetCmdMsg->m_nCommand = CScCmdMsg::enCmdRetSuspend;
				pScRetCmdMsg->m_strValues.SetSize(1);
				pScRetCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
				pSc->SetCmdMsg(pScRetCmdMsg);


				
				CScCmdMsg*		pScStoCmdMsg			= new CScCmdMsg;
				DEBUGER_ASSERT_VALID(pScStoCmdMsg		!= NULL);

				pScStoCmdMsg->m_nCommand = CScCmdMsg::enCmdStoSuspend;
				pScStoCmdMsg->m_strValues.SetSize(1);
				pScStoCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
				pSc->SetCmdMsg(pScStoCmdMsg);
			}

			break;

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

		if (pEquipment->m_enKind == CEquipment::enCV)
		{
			pEquipment->ReconnectServer();
			//pEquipment->m_pSocket1->m_enCommStatus = CAsyncSocketEx::enStatusClose;
			//pEquipment->ConnectServer();
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
		DEBUGER_ASSERT_VALID(pEquipment->GetInfo() != NULL);

		pEquipment->GetInfo()->StatusReport();
	}
}

BOOL CEquipmentArray::IsIdleJob(int nLuggNum)
{
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment->IsIdleJob(nLuggNum) == FALSE)
			return FALSE;
	}

	return TRUE;
}

void CEquipmentArray::InvokeControl(BOOL bManual)
{
	CInfo* pInfo = NULL;
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		pInfo = pEquipment->GetInfo();
		DEBUGER_ASSERT_VALID(pInfo != NULL);

		if (bManual)
			pInfo->m_bModified = TRUE;
		pInfo->InvokeControl();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEquipmentArray::Load(CReportCtrl* pReportCtrl)
{
	CWaitCursor SomeWait;
	pReportCtrl->DeleteAllItems();
	CString strTemp;

	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];

		if (pEquipment == NULL)
			continue;

		// 하나의 설비(PLC)당 소켓이 여러개이다. 
		for (int j=0;j<PLC_CONN_PORT_CNT;j++)
		{
			

			CStringList listTemp;
			listTemp.AddTail(pEquipment->GetDeviceName());
			listTemp.AddTail("");
			listTemp.AddTail(pEquipment->m_port.m_strIP);
			strTemp.Format(_T("%d"), j + 1);
			listTemp.AddTail(strTemp);
			strTemp.Format(_T("%d"),pEquipment->m_port.m_nPort[j]);
			listTemp.AddTail(strTemp);

			BOOL bTemp = (pEquipment->m_pSocket[j] != NULL && pEquipment->m_pSocket[j]->m_enCommStatus == CInterfaceSk::enStatusConnectOK);		

			listTemp.AddTail( bTemp ? "O" : "X");

			pReportCtrl->AddItem(0, 0, &listTemp);
			pReportCtrl->SetItemData(i, (LPARAM)pEquipment);
			listTemp.RemoveAll();
		}
	}

	pReportCtrl->ResizeColumn();
}

