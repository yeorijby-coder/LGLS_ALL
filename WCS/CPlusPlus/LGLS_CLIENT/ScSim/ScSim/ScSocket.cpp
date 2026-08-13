// ScSocket.cpp: implementation of the CScSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "ScSocket.h"
#include "Sc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CScSocket, CInterfaceSk)

CScSocket::CScSocket(CEquipment* pEquipment, int nIndex) : CInterfaceSk(pEquipment, nIndex)
{

}

CScSocket::~CScSocket()
{

}

void CScSocket::OnReceive(int nErrorCode)
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	m_pEquipment->UpdateCommStatus(nErrorCode ? NOTIFY_ERROR : NOTIFY_RECEIVE);
	
	if (nErrorCode)
	{
		m_strLog.Format(_T("%s SOCKET ERROR [CODE=%d, MSG=%s]"), m_strName, nErrorCode, CLib::GetSystemErrMsg(nErrorCode));
		DEBUGER_TRACE(m_strLog, _T("CScSocket::OnReceive"));
		OnClose(nErrorCode);
		return;
	}

	CString strRecv;
	if (!CInterfaceSk::RecvData(strRecv))
	{
		m_pEquipment->WriteLog(LOG_TYPE_ERROR, m_strLog, _T("CLgvSocket::OnReceive"));
		return;
	}

	CAsyncSocketEx::TokenRxFrame(strRecv, enSTX, enETX);
	for (POSITION pos=m_strRxList.GetHeadPosition(); pos!=NULL; )
	{
		CString& strRxFrame = m_strRxList.GetNext(pos);
		int nLen = strRxFrame.GetLength();

		if (nLen < 9)
		{
			m_strRecv.Empty();
			m_strLog.Format(_T("%s 메시지 헤더 길이 이상! [LEN=%d] [DATA=%s]"), m_strName, nLen, strRxFrame);
//			m_pEquipment->WriteLog(LOG_TYPE_ERROR, m_strLog, _T("CScSocket::OnReceive"));
//			DEBUGER_TRACE(m_strLog, _T("CScSocket::OnReceive"));
			if (m_bRecvWaiting) ::SetEvent(m_hRecvEvent);
			continue;
		}

		if (strRxFrame[0] != enSTX)
		{
			m_strRecv.Empty();
			m_strLog.Format(_T("%s STX 이상! [DATA=%s]"), m_strName, strRxFrame);
//			m_pEquipment->WriteLog(LOG_TYPE_ERROR, m_strLog, _T("CScSocket::OnReceive"));
//			DEBUGER_TRACE(m_strLog, _T("CScSocket::OnReceive"));
			if (m_bRecvWaiting) ::SetEvent(m_hRecvEvent);
			continue;
		}

		if (strRxFrame[nLen-1] != enETX)
		{
			m_strRecv.Empty();
			m_strLog.Format(_T("%s ETX 이상! [DATA=%s]"), m_strName, strRxFrame);
//			m_pEquipment->WriteLog(LOG_TYPE_ERROR, m_strLog, _T("CScSocket::OnReceive"));
//			DEBUGER_TRACE(m_strLog, _T("CScSocket::OnReceive"));
			if (m_bRecvWaiting) ::SetEvent(m_hRecvEvent);
			continue;
		}

		if (_ttoi(strRxFrame.Mid(1, 4)) != nLen)
		{
			m_strRecv.Empty();
			m_strLog.Format(_T("%s 수신 메시지 길이 이상! [DATA=%s]"), m_strName, strRxFrame);
//			m_pEquipment->WriteLog(LOG_TYPE_ERROR, m_strLog, _T("CScSocket::OnReceive"));
//			DEBUGER_TRACE(m_strLog, _T("CScSocket::OnReceive"));
			if (m_bRecvWaiting) ::SetEvent(m_hRecvEvent);
			continue;
		}

		CString strCmd = strRxFrame.Mid(5, 3);	// 김봉순과장
		if ((strCmd != _T("OC2")) && (strCmd != _T("SQ2")) && (strCmd != _T("SQ3")) && (strCmd != _T("CC2")) && (strCmd != _T("OM2")))
		{
			m_strRecv.Empty();
			m_strLog.Format(_T("%s 유효하지않은 응답메시지! [DATA=%s]"), m_strName, strRxFrame);
//			m_pEquipment->WriteLog(LOG_TYPE_ERROR, m_strLog, _T("CScSocket::OnReceive"));
//			DEBUGER_TRACE(m_strLog, _T("CScSocket::OnReceive"));
			if (m_bRecvWaiting) ::SetEvent(m_hRecvEvent);
			continue;
		}
		if (strCmd == _T("SQ3"))
		{
			continue;
		}

		if (!m_bRecvWaiting)
		{
			m_strLog.Format(_T("%s 수신 대기시간 초과후 데이터 수신! [DATA=%s]"), m_strName, strRxFrame);
			m_pEquipment->WriteLog(LOG_TYPE_ERROR, m_strLog, _T("CScSocket::OnReceive"));
			continue;
		}

		m_strRecv = strRxFrame.Mid(5, nLen-5-1);
		::SetEvent(m_hRecvEvent);
	}
}

BOOL CScSocket::SendMsg(int nType, const CString& strMsg)
{
	m_strRecv.Empty();

	CString strData;
	strData.Format(_T("%c%04d%s%c"), enSTX, strMsg.GetLength()+6, strMsg, enETX);
	return CInterfaceSk::SendData(strData);
}
