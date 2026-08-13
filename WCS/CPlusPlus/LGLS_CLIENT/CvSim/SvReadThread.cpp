#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
//#include "Thread.h"
#include "Log.h"

UINT SvReadThreadProc(LPVOID pParam)
{
	SSvReadThreadInfo* pSvReadInfo = (SSvReadThreadInfo*)pParam;

    while (TRUE) 
	{
		if (WaitForSingleObject(pSvReadInfo->m_hEventKillSvReadThread, 100)	== WAIT_OBJECT_0)
			break; // Terminate this thread by existing the proc.

		if (pSvReadInfo->m_pDoc->m_pHostSv == NULL)
			continue;

		pSvReadInfo->m_pDoc->m_pHostSv->WriteData();
	}
	SetEvent(pSvReadInfo->m_hEventSvReadThreadKilled);
	
	return 0;
}
