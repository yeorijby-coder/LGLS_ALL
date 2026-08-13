// MonitorClient.cpp: implementation of the CMonitorClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "MonitorClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonitorClient::CMonitorClient(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	m_pXml = new CXmlDom;
	DEBUGER_ASSERT_VALID(m_pXml != NULL);

	m_bValidate = FALSE;
}

CMonitorClient::~CMonitorClient()
{
	delete m_pXml;
}

BOOL CMonitorClient::InitializeXmlDom()
{
	DEBUGER_ASSERT_VALID(m_pXml != NULL);
	return m_pXml->InitializeXmlDom();
}

void CMonitorClient::OnClose(int nErrorCode)
{
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	CString strLog;
	strLog.Format(_T("CMonitorClient::OnClose... ECS MONITOR SERVER 연결 해제! [%s]"), CLib::GetSystemErrMsg(nErrorCode));
	m_pDoc->Alarm(LOG_POS_MONITOR, strLog);

	m_bValidate = FALSE;
//	m_pDoc->m_pMonitorClient = NULL;

	CAsyncSocket::ShutDown();
	delete this;	
}

void CMonitorClient::OnReceive(int nErrorCode)
{
	CString strLog;
	if (nErrorCode)
	{
		OnClose(nErrorCode);
		return;
	}

	CString strRecv;
	if (!RecvData(strRecv))
	{
		m_pDoc->Alarm(LOG_POS_MONITOR, _T("CMonitorServer::OnReceive... ") + m_strLog);
		return;
	}
	
	DEBUGER_ASSERT_VALID(m_pXml != NULL);
	CAsyncSocketEx::TokenRxFrame(strRecv, enSTX, enETX);
	for (POSITION pos=m_strRxList.GetHeadPosition(); pos!=NULL; )
	{
		CString& strRxFrame = m_strRxList.GetNext(pos);
		int nLen = strRxFrame.GetLength();

		if (nLen < 12)
		{
			strLog.Format(_T("수신데이터 길이 이상! [LEN=%d]"), nLen);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnReceive"));
			continue;
		}

		if (strRxFrame[0] != enSTX)
		{
			strLog.Format(_T("수신데이터 이상! [NO STX]"));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnReceive"));
			continue;
		}

		if (strRxFrame[nLen-1] != enETX)
		{
			strLog.Format(_T("수신데이터 이상! [NO ETX]"));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnReceive"));
			continue;
		}

		if (m_pXml->LoadXmlString(strRxFrame.Mid(1, nLen-2)) == FALSE)
		{
			strLog.Format(_T("LoadXmlString 실패! [%s]"), m_pXml->GetErrorMessage());
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnReceive"));
			continue;
		}

		if (m_pXml->MoveXPath(_T("/ECS/SYS")))
			ParseSys();

		if (m_pXml->MoveXPath(_T("/ECS/JOB")))
			ParseJob();

		if (m_pXml->MoveXPath(_T("/ECS/EQUIP")))
			ParseEquip();
	}

	if (m_bValidate == FALSE)
		OnClose(0);
}
