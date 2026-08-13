// MonitorListener.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "MonitorListener.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitorListener

CMonitorListener::CMonitorListener(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
}

CMonitorListener::~CMonitorListener()
{
	for (POSITION pos=m_pSocketList.GetHeadPosition(); pos!=NULL; )
		delete (CMonitorServer*)m_pSocketList.GetNext(pos);
	m_pSocketList.RemoveAll();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMonitorListener, CAsyncSocket)
	//{{AFX_MSG_MAP(CMonitorListener)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMonitorListener member functions

void CMonitorListener::OnAccept(int nErrorCode) 
{
	CString strLog;
	CMonitorServer* pMonitorSv = new CMonitorServer(m_pDoc);
	DEBUGER_ASSERT_VALID(pMonitorSv != NULL);

	if (pMonitorSv->InitializeXmlDom() == FALSE)
	{
		strLog.Format(_T("XML 초기화 실패"));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorListener::OnAccept"));
		delete pMonitorSv;
		return;
	}

	if (!Accept(*pMonitorSv))
	{
		strLog.Format(_T("Accept 에러 [%s]"), CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorListener::OnAccept"));
		delete pMonitorSv;
		return;
	}

	pMonitorSv->m_tAccept = CTime::GetCurrentTime();
	pMonitorSv->m_strPeerIP = pMonitorSv->GetPeerIP();
	pMonitorSv->m_nPeerPort = pMonitorSv->GetPeerPort();

	if (GetClient(pMonitorSv->m_strPeerIP) != NULL)
	{
		strLog.Format(_T("해당 CLIENT는 이미 접속되어 있습니다. [%s]"), pMonitorSv->m_strPeerIP);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorListener::OnAccept"), NULL, TRUE);
		delete pMonitorSv;
		return;
	}

	TRY
	{
		m_pSocketList.AddTail(pMonitorSv);
	}
	CATCH (CException, e)
	{
		strLog.Format(_T("CLIENT 추가 실패! [%s]"), CLib::GetExceptionString(e));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorListener::OnAccept"), NULL, TRUE);
		delete pMonitorSv;
	}
	END_CATCH
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMonitorServer* CMonitorListener::GetClient(CString& strIP)
{
	DEBUGER_ASSERT_VALID(strIP.GetLength() > 0);

	CMonitorServer* pMonitorSv = NULL;
	for (POSITION pos=m_pSocketList.GetHeadPosition(); pos!=NULL; )
	{
		pMonitorSv = (CMonitorServer*)m_pSocketList.GetNext(pos);
		DEBUGER_ASSERT_VALID(pMonitorSv != NULL);

		if (pMonitorSv->m_strPeerIP == strIP)
			return pMonitorSv;
	}

	return NULL;
}

void CMonitorListener::RemoveClient(CMonitorServer* pMonitorSv)
{
	DEBUGER_ASSERT_VALID(pMonitorSv != NULL);

	POSITION pos = m_pSocketList.Find(pMonitorSv);
	if (pos == NULL)
	{
		CString strLog;
		strLog.Format(_T("해당 CLIENT가 존재하지 않습니다. [IP=%s, PORT=%d]"), pMonitorSv->m_strPeerIP, pMonitorSv->m_nPeerPort);
		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorListener::RemoveClient"));
		return;
	}

	m_pSocketList.RemoveAt(pos);
}

void CMonitorListener::SendToClients(CString& strSend)
{
	m_csSyncSend.Lock();

	CMonitorServer* pMonitorSv = NULL;
	for (POSITION pos=m_pSocketList.GetHeadPosition(); pos!=NULL; )
	{
		pMonitorSv = (CMonitorServer*)m_pSocketList.GetNext(pos);
		DEBUGER_ASSERT_VALID(pMonitorSv != NULL);

		if (pMonitorSv->m_bValidate)
			pMonitorSv->SendData(strSend);
	}

	m_csSyncSend.Unlock();
}

void CMonitorListener::Load(CReportCtrl* pReportCtrl)
{
	CWaitCursor SomeWait;
	CMonitorServer* pMoniterSv = NULL;

	pReportCtrl->DeleteAllItems();

	for (POSITION pos = m_pSocketList.GetHeadPosition(); pos != NULL; )
	{
		pMoniterSv = (CMonitorServer*)m_pSocketList.GetNext(pos);
		if (pMoniterSv == NULL) 
			continue;

		CStringList listTemp;
		listTemp.AddTail(pMoniterSv->m_strPeerName);
		listTemp.AddTail(pMoniterSv->m_tAccept.Format(_T("%Y-%m-%d %H:%M:%S")));
		listTemp.AddTail(pMoniterSv->m_strPeerIP);
		listTemp.AddTail(CConvert::ToString(pMoniterSv->m_nPeerPort));
		listTemp.AddTail(pMoniterSv->m_bValidate ? "O" : "X");
		pReportCtrl->AddItem(0, 0, &listTemp);
		listTemp.RemoveAll();
	}

	pReportCtrl->ResizeColumn();
}
