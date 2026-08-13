// InterfaceSk.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "InterfaceSk.h"
#include "Equipment.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterfaceSk

IMPLEMENT_DYNAMIC(CInterfaceSk, CAsyncSocketEx)

CInterfaceSk::CInterfaceSk(CEquipment* pEquipment) : CAsyncSocketEx(pEquipment->m_port.m_strDevice, pEquipment->m_port.m_bWriteLog)
{
	m_pEquipment = pEquipment;
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	m_bRecvWaiting = FALSE;
	m_nRecvTimeoutCount = 0;

	m_hRecvEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

}

CInterfaceSk::~CInterfaceSk()
{
	::CloseHandle(m_hRecvEvent);
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CInterfaceSk, CAsyncSocket)
	//{{AFX_MSG_MAP(CInterfaceSk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CInterfaceSk member functions

void CInterfaceSk::OnConnect(int nErrorCode) 
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if(m_pEquipment->m_enKind == CEquipment::enRGV)
		return;

	m_pEquipment->OnConnectSocket(nErrorCode);

	if (nErrorCode) delete this;
}

void CInterfaceSk::OnClose(int nErrorCode) 
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if(m_pEquipment->m_enKind == CEquipment::enRGV)
		return;
	
	m_pEquipment->OnCloseSocket(nErrorCode);
	delete this;
}


void CInterfaceSk::OnReceive(int nErrorCode) 
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if(m_pEquipment->m_enKind == CEquipment::enRGV)
		return;
	
	if (nErrorCode)
	{
		m_strLog.Format(_T("%s %s"), m_strName, CLib::GetSystemErrMsg(nErrorCode));
		m_pEquipment->WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CInterfaceSk::OnReceive"));
		OnClose(nErrorCode);
		return;
	}

	if (!m_bRecvWaiting)
	{
		CString strRecv;
		RecvData(strRecv);
		m_strLog.Format(_T("%s 수신 대기시간 초과후 데이터 수신! [RECV=%s]"), m_strName, strRecv);
		DEBUGER_TRACE(m_strLog, _T("CInterfaceSk::OnReceive"));
		return;
	}

	::SetEvent(m_hRecvEvent);
}
//*/
/////////////////////////////////////////////////////////////////////////////

BOOL CInterfaceSk::RecvWait()
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if(m_pEquipment->m_enKind == CEquipment::enRGV)
		return FALSE;

	m_bRecvWaiting = TRUE;

 	if (::WaitForSingleObject(m_hRecvEvent, m_pEquipment->m_port.m_dwRecvTimeout) == WAIT_TIMEOUT)
	{
		m_bRecvWaiting = FALSE;
		m_nRecvTimeoutCount++;

		m_strLog.Format(_T("%s 수신 대기시간 초과! [TIMEOUT=%d] [COUNT=%d]"), m_strName, m_pEquipment->m_port.m_dwRecvTimeout, m_nRecvTimeoutCount);
		DEBUGER_TRACE(m_strLog, _T("CInterfaceSk::RecvWait"));
		return FALSE;
	}

	m_bRecvWaiting = FALSE;
	m_nRecvTimeoutCount = 0;

	return TRUE;
}
