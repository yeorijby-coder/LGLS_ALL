// MonitorClient.h: interface for the CMonitorClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORCLIENT_H__261CC3E0_91B1_470C_96C7_48DAAFB6E5A3__INCLUDED_)
#define AFX_MONITORCLIENT_H__261CC3E0_91B1_470C_96C7_48DAAFB6E5A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AsyncSocketEx.h"

class CEcsDoc;
class CXmlDom;
class CMonitorClient : public CAsyncSocketEx  
{
public:
	CMonitorClient(CEcsDoc* pDoc);
	virtual ~CMonitorClient();

public:
	enum { enSTX = 0x02, enETX = 0x03 };
	enum { enSendBufferMaxSize = 7800 };

protected:
	CEcsDoc* m_pDoc;
	CXmlDom* m_pXml;

public:
	BOOL m_bValidate;

public:
	BOOL InitializeXmlDom();

public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);	
};

#endif // !defined(AFX_MONITORCLIENT_H__261CC3E0_91B1_470C_96C7_48DAAFB6E5A3__INCLUDED_)
