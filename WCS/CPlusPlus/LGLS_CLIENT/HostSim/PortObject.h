#if !defined(AFX_PORTOBJECT_H__3E4B1AFA_BEF7_4FED_8CFF_778DA8CE8D8A__INCLUDED_)
#define AFX_PORTOBJECT_H__3E4B1AFA_BEF7_4FED_8CFF_778DA8CE8D8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxtempl.h>

class CPortObject : public CObject
{
	DECLARE_DYNCREATE(CPortObject)

public:
	CPortObject();
	virtual ~CPortObject();

public:
	CString	m_strDevice;
	BOOL	m_bConnect;
	BOOL	m_bRetry;
};

class CPortEthernet : public CPortObject
{
	DECLARE_DYNCREATE(CPortEthernet)

public:
	CPortEthernet();
	virtual ~CPortEthernet();

public:
	CString m_strIP;
	int     m_nPort;
	BOOL	m_bWriteLog;
	DWORD	m_dwRecvTimeout;
};

class CPortSerial : public CPortObject
{
	DECLARE_DYNCREATE(CPortSerial)

public:
	CPortSerial();
	virtual ~CPortSerial();

public:
	CString	m_strComName;
	int		m_nBaudRate;
	int		m_nParity;
	int		m_nStopBits;
	int		m_nBitsPerChar;
	DWORD	m_dwReadTimeOut;
};

typedef CArray<CPortEthernet, CPortEthernet>	CPortArray;

#endif // !defined(AFX_PORTOBJECT_H__3E4B1AFA_BEF7_4FED_8CFF_778DA8CE8D8A__INCLUDED_)
