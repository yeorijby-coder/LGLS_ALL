#pragma once

#include <afxcoll.h>
#include <afxtempl.h>
#include "URMDBAccess.h"
#include "AdoDB.h"
#include "CellInfo.h"
#include "JOB_MST.h"

class CRecordSetWrap;
class CEcsDoc;
class CCollectDataList
{
public:
	CCollectDataList(CEcsDoc* pDoc);
	CCollectDataList(void);
	~CCollectDataList(void);


private:
	CURMDBAccess*		m_pDB_ACCESS;

public:
	BOOL IsDB_POSSIBLE();

private:
	CEcsDoc* m_pDoc;

private:
	CWinThread* m_pThread;
	BOOL m_bThreadDoWork;

private:
	void ThreadDoWork();

public:
	BOOL IsAllive();

private:
	static UINT DoWork(LPVOID pParm);

public:
	BOOL StartDoWork();
	BOOL StopDoWork();

public:
	bool IsCollectCellInfo();
	void CallBackCellInfo();

	bool IsCollectJOB_MST();
	void CallBackJOB_MST();

public:
	CString GetSelQuery_CELL_MST();
	CMap<CString, LPCTSTR, CCellInfo*, CCellInfo*>* CreateCellInfo( CRecordSetWrap* pRsw, int nRowCnt);

	CString GetSelQuery_JOB_MST();
	CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* CreateJOB_MST( CRecordSetWrap* pRsw, int nRowCnt );
};

