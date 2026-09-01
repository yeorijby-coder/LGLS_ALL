// MonitorListener.h : header file
//

#pragma once

class CEcsDoc;
class CMonitorServer;


// CMonitorListener
//
class CMonitorListener : public CAsyncSocket
{
public:
	CMonitorListener(CEcsDoc* pDoc);
	virtual ~CMonitorListener();

protected:
	CEcsDoc* m_pDoc;
	CObList m_pSocketList;
	CCriticalSection m_csSyncSend;

public:
	CMonitorServer* GetClient(CString& strIP);
	void RemoveClient(CMonitorServer* pMonitorSv);
	void SendToClients(CString& strSend);
	void Load(CReportCtrl* pReportCtrl);
	int GetClientCount() { return m_pSocketList.GetCount();	}

// Overrides
public:
	virtual void OnAccept(int nErrorCode);

// Implementation
protected:
};
