#pragma once
#include "afx.h"
#include "JOB_MST.h"

class CJobCollection
{
public:
	CJobCollection(CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* pMap);
	CJobCollection(CEcsDoc* pDoc);
	~CJobCollection(void);

public:
	CEcsDoc* m_pDoc;
	CJOB_MST* m_pCopyJob;

public:
	BOOL m_bBlock;
	CCriticalSection m_csBlock;
	void EnterBlcokingSection();
	void LeaveBlcokingSection();

public:
	CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* m_pMap;

public:
	CJobCollection* GetJobCollection();
	BOOL ChgJobCollection(CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* pMapChgJOB_MST);
	CJOB_MST* GetJOB_MST( CString pstrLUGG_NO );
	CJOB_MST* GetCopyJob();

private: 
	BOOL DelJobCollection();
};

