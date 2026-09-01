#pragma once

#include <afxcoll.h>
#include <afxtempl.h>
#include "URMDBAccess.h"
#include "AdoDB.h"
#include "Equipment.h"
#include "ConnectStatus.h"

class CEcsDoc;
class CCollectDB
{
public:
	CCollectDB(CEcsDoc* pDoc);
	~CCollectDB(void);

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
	BOOL m_blConnectStatus;


private:
	void Collect_EQUIPMENT(CEquipment* pEquipent);
	void ConnectStatus(CConnectStatus* pConnectStatus, CString strHostNum);
};

