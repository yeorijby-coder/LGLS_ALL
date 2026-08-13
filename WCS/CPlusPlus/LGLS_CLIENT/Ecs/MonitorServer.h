// MonitorServer.h: interface for the CMonitorServer class.
//

#pragma once

#include "AsyncSocketEx.h"
#include "CmdMsg.h"

class CEcsDoc;
class CXmlDom;


class CMonitorServer : public CAsyncSocketEx  
{
public:
	CMonitorServer(CEcsDoc* pDoc);
	virtual ~CMonitorServer();

public:
	enum { enSTX = 0x02, enETX = 0x03 };
	enum { enSendBufferMaxSize = 7800 };

protected:
	CEcsDoc* m_pDoc;
	CXmlDom* m_pXml;

public:
	CTime	m_tAccept;
	CString m_strPeerName;
	CString	m_strPeerIP;
	int		m_nPeerPort;
	BOOL	m_bValidate;
	BOOL	m_bThreadOut;
	BOOL	m_bFirstConnect;
	CCriticalSection m_csSyncSend;
//SV<->TASK 부하 스레드용 doc->equipments->multicastinfo 20180106
public:
	enum EN_EVENT { enEventKill, enEventSend, enEventFirst, enEventSize};
	HANDLE m_hEventArray[enEventSize];

	CObList			m_cmdList;
public:
	CTime m_tThreadTime;
public:
	CWinThread* m_pEquipThread; 

protected:
CCriticalSection m_csSyncThread;
static UINT ThreadMultiCast(LPVOID pParam);
typedef UINT (*FPThreadProc)(LPVOID);
FPThreadProc m_pfThreadProc;

//------------------------------------------------------

public:
	void SetCmdMsg(CCmdMsg* pCmdMsg);
	BOOL InitializeXmlDom();
	void SendDataCmd(CString &strSend);
	BOOL IsConnect() { return (m_enCommStatus == enStatusConnectOK); }
	CString GetClientName() { 
		CString strName;
		strName.Format(_T("%s:%s"), m_strPeerIP, m_strPeerName);
		return strName;
	}
	

protected:
	BOOL ParseSys();
	BOOL ParseJob();
	BOOL ParseEquip();
	void CommandProc();

public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

////SV<->TASK 부하 스레드용 doc->equipments->multicastinfo 20180106
//public:
//	BOOL	IsEquipThreadUse();
////---------------------------------------------------------------
//
};
