#include "StdAfx.h"
#include "SockThread.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "MonitorServer.h"

IMPLEMENT_DYNCREATE(CSockThread, CWinThread)

CSockThread::CSockThread()
{
}
CSockThread::CSockThread(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
}


CSockThread::~CSockThread()
{
}
BOOL CSockThread::InitInstance()
{	
	if(!AfxSocketInit())
	{
		return FALSE;
	}
	m_CMonitorSv = new CMonitorServer(m_pDoc);
	m_CMonitorSv->Attach(m_hSocket,(LONG)0);

	m_pDoc->m_pMonitorListener->m_pSocketList.AddTail(m_CMonitorSv);
	return TRUE;
}
BOOL CSockThread::Run()
{
	return TRUE;
}
BOOL CSockThread::ExitInstance()
{
	return TRUE;
}

void CSockThread::SendData(CString strSend)
{
	m_CMonitorSv->SendData(strSend);
}


