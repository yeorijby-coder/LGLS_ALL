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
	// [LGLS 2026-09-10] 자기 자신을 기다리지 않으려고 스레드 id 를 들고 있는다.
	//   종전 StopDoWork 은 CWinThread* 를 HANDLE 자리에 넘겼다(잘못된 핸들).
	DWORD m_dwThreadId;

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

