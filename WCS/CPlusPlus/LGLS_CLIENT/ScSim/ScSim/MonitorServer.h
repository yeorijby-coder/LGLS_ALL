// MonitorServer.h: interface for the CMonitorServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORSERVER_H__4C4922DE_CD81_4CC5_BFC5_50744724DB73__INCLUDED_)
#define AFX_MONITORSERVER_H__4C4922DE_CD81_4CC5_BFC5_50744724DB73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AsyncSocketEx.h"

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
	COleDateTime	m_tAccept;
	CString			m_strPeerName;
	CString			m_strPeerIP;
	int				m_nPeerPort;
	BOOL			m_bValidate;

public:
	BOOL InitializeXmlDom();
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

public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

#endif // !defined(AFX_MONITORSERVER_H__4C4922DE_CD81_4CC5_BFC5_50744724DB73__INCLUDED_)
