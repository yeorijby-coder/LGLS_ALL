// InterfaceThreadSk.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "InterfaceThreadSk.h"
#include "Equipment.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterfaceThreadSk

IMPLEMENT_DYNAMIC(CInterfaceThreadSk, CAsyncSocketEx)

CInterfaceThreadSk::CInterfaceThreadSk(CEquipment* pEquipment) : CAsyncSocketEx(pEquipment->m_port.m_strDevice, pEquipment->m_port.m_bWriteLog)
{
	m_pEquipment = pEquipment;
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	m_bRecvWaiting = FALSE;
	m_nRecvTimeoutCount = 0;

	m_hRecvEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CInterfaceThreadSk::~CInterfaceThreadSk()
{
	::CloseHandle(m_hRecvEvent);
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CInterfaceThreadSk, CInterfaceSk)
	//{{AFX_MSG_MAP(CInterfaceThreadSk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CInterfaceThreadSk member functions

void CInterfaceThreadSk::OnConnect(int nErrorCode) 
{
//	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if (m_pEquipment == NULL)
		return;

	if(m_pEquipment->m_enKind != CEquipment::enRGV)
		return;

	m_pEquipment->OnConnectThreadSocket(nErrorCode);
	if (nErrorCode) delete this;
}

void CInterfaceThreadSk::OnClose(int nErrorCode) 
{
//	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if (m_pEquipment == NULL)
		return;

	if(m_pEquipment->m_enKind != CEquipment::enRGV)
		return;

	Sleep(100);

	m_pEquipment->OnCloseThreadSocket(nErrorCode);
	delete this;
}

/*
void CInterfaceThreadSk::OnReceive(int nErrorCode) 
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if(m_pEquipment->m_enKind != CEquipment::enRGV)
		return;

	if (nErrorCode)
	{
		m_strLog.Format(_T("%s %s"), m_strName, CLib::GetSystemErrMsg(nErrorCode));
		m_pEquipment->WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CInterfaceThreadSk::OnReceive"));
		OnClose(nErrorCode);
		return;
	}

	if (!m_bRecvWaiting)
	{
		CString strRecv;
		RecvData(strRecv);
		m_strLog.Format(_T("%s 수신 대기시간 초과후 데이터 수신! [RECV=%s]"), m_strName, strRecv);
		DEBUGER_TRACE(m_strLog, _T("CInterfaceThreadSk::OnReceive"));
		return;
	}

	::SetEvent(m_hRecvEvent);
}
*/

/////////////////////////////////////////////////////////////////////////////

BOOL CInterfaceThreadSk::RecvWait()
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if (m_pEquipment == NULL)
		return FALSE;

	if(m_pEquipment->m_enKind != CEquipment::enRGV)
		return FALSE;

	m_bRecvWaiting = TRUE;

	if(WaitForSingleObject(m_hRecvEvent, m_pEquipment->m_port.m_dwRecvTimeout) == WAIT_TIMEOUT)
	{
		if (::WaitForSingleObject(m_hRecvEvent, m_pEquipment->m_port.m_dwRecvTimeout) == WAIT_TIMEOUT)
		{
			m_bRecvWaiting = FALSE;

			m_nRecvTimeoutCount++;

			m_strLog.Format(_T("%s 수신 대기시간 초과! [TIMEOUT=%d] [COUNT=%d]"), m_strName, m_pEquipment->m_port.m_dwRecvTimeout, m_nRecvTimeoutCount);
			DEBUGER_TRACE(m_strLog, _T("CInterfaceThreadSk::RecvWait"));
			return FALSE;
		}
	}

	m_bRecvWaiting = FALSE;
	m_nRecvTimeoutCount = 0;

	return TRUE;
}

BOOL CInterfaceThreadSk::SendData(const CString& strData, BOOL bMonitor)
{
	if(m_pEquipment->m_enKind != CEquipment::enRGV || bMonitor == TRUE)
		return FALSE;

	m_strSend = strData;
	m_pEquipment->RefreshDialog();
	int nErrorCode = 0;
	int nDataLen = strData.GetLength();
	int nSendLen = Send(strData, nDataLen);

	if (nSendLen == SOCKET_ERROR)
	{
//		nErrorCode = CAsyncSocket::GetLastError();
///		if (nErrorCode == WSAEWOULDBLOCK)
//			m_strTxList.AddTail(strData);

		m_strLog.Format(_T("%s 데이터 전송 실패! [%s] [%s]"), m_strName, CLib::GetSystemErrMsg(nErrorCode), strData);
		return FALSE;
	}

	if (nSendLen != nDataLen)
	{
		m_strLog.Format(_T("%s 데이터 전송 실패! [전송데이터 길이 이상, DATA=%d, SEND=%d] [%s]"), m_strName, nDataLen, nSendLen, strData);
		return FALSE;
	}
	
	if (m_bWriteLog)
	{
		CString strSend;
		strSend.Format(_T("[%s][%d][SEND] %s \r\n"), CTime::GetCurrentTime().Format(_T("%H:%M:%S")), ::GetCurrentThreadId(), strData);
		WriteLog(strSend);
	}

	return TRUE;
}

