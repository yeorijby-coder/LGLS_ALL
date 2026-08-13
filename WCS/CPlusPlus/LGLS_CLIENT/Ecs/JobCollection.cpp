#include "StdAfx.h"
#include "stdafx.h"
#include "JobCollection.h"


CJobCollection::CJobCollection(CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* pMap)
{
	m_pMap = pMap;
}

CJobCollection::CJobCollection(CEcsDoc* pDoc)
{
	m_bBlock = FALSE;
	m_pMap = new CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>();
	m_pMap->InitHashTable(1);
	m_pDoc = pDoc;
	m_pCopyJob = NULL;
}


CJobCollection::~CJobCollection(void)
{
	if(m_pMap != NULL)
	{
		for(POSITION pos = m_pMap->GetStartPosition(); pos != NULL;)
		{
			CString strLuggNo;
			CJOB_MST* pJOB_MST = NULL;
			m_pMap->GetNextAssoc(pos, strLuggNo, pJOB_MST);
			if(pJOB_MST != NULL){ delete pJOB_MST; pJOB_MST = NULL; };
		}
		m_pMap->RemoveAll();
		delete m_pMap;
	}
}

BOOL CJobCollection::DelJobCollection()
{
	if(m_pMap != NULL)
	{
		int nCount = m_pMap->GetCount();
		for(POSITION pos = m_pMap->GetStartPosition(); pos != NULL;)
		{
			CString strLuggNo;
			CJOB_MST* pJOB_MST = NULL;
			m_pMap->GetNextAssoc(pos, strLuggNo, pJOB_MST);
			if(pJOB_MST != NULL){ delete pJOB_MST; pJOB_MST = NULL;};
		}
		m_pMap->RemoveAll();
		delete m_pMap;
	}
	return TRUE;
}

BOOL CJobCollection::ChgJobCollection(CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* pMapChgJOB_MST)
{
	if(m_bBlock == TRUE){ return FALSE; };
	m_bBlock = TRUE;
	EnterBlcokingSection();

	DelJobCollection();
	m_pMap = pMapChgJOB_MST;

	m_bBlock = FALSE;
	LeaveBlcokingSection();

	return TRUE;
}

CJobCollection* CJobCollection::GetJobCollection()
{
	if(m_bBlock == TRUE){ return NULL; };
	EnterBlcokingSection();
	m_bBlock = TRUE;

	CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* pMapCloneJOB_MST = new CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>();
	
	int nJobCount = m_pMap->GetCount();
	//pMapCloneJOB_MST->InitHashTable(nJobCount);
	for(POSITION pos = m_pMap->GetStartPosition(); pos != NULL;)
	{
		if(pos == NULL){ continue; };
		CString strLuggNo;
		CJOB_MST* pJOB_MST = NULL;
		m_pMap->GetNextAssoc(pos, strLuggNo, pJOB_MST);
		CJOB_MST* pCloneJOB_MST = new CJOB_MST(pJOB_MST);
		pMapCloneJOB_MST->SetAt(pCloneJOB_MST->LUGG_NO, pCloneJOB_MST);
	}

	m_bBlock = FALSE;
	LeaveBlcokingSection();

	return new CJobCollection(pMapCloneJOB_MST);
}

void CJobCollection::EnterBlcokingSection()
{
	m_csBlock.Lock();
}

void CJobCollection::LeaveBlcokingSection()
{
	m_csBlock.Unlock();
}

CJOB_MST* CJobCollection::GetJOB_MST( CString pstrLUGG_NO )
{
	CJOB_MST* pJOB_MST = (*m_pMap)[pstrLUGG_NO];
	if(pJOB_MST == NULL){ return NULL; };
	return new CJOB_MST(pJOB_MST);
}


CJOB_MST* CJobCollection::GetCopyJob()
{ 
	if(m_pCopyJob == NULL){ m_pCopyJob = new CJOB_MST(); }
	
	return m_pCopyJob;
}
