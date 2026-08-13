#pragma once

class CMonitorServer;

class CSockThread :public CWinThread
{

protected:	
	DECLARE_DYNCREATE(CSockThread);
public:
	CMonitorServer* m_CMonitorSv;
	SOCKET m_hSocket;
	CEcsDoc* m_pDoc;

public:
	CSockThread(CEcsDoc* pDoc);
	CSockThread();
	~CSockThread();

public:
	virtual BOOL InitInstance();
	virtual int Run();
	virtual int ExitInstance();

public:
	void SendData(CString strSend);
};

