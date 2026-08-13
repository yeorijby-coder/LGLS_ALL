#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
//#include "Thread.h"
#include "Log.h"

UINT ClReadThreadProc(LPVOID pParam)
{
	SClReadThreadInfo* pClReadInfo = (SClReadThreadInfo*)pParam;

    while (TRUE) 
	{
		if (WaitForSingleObject(pClReadInfo->m_hEventKillClReadThread, 500) // Kill notify
			== WAIT_OBJECT_0)
		{
			break; // Terminate this thread by existing the proc.
		}

		if (pClReadInfo->m_pDoc->m_pHostCl == NULL)
			continue;

		pClReadInfo->m_pDoc->m_pHostCl->ReadData();

	}

	SetEvent(pClReadInfo->m_hEventClReadThreadKilled);
	
	return 0;
	
}
