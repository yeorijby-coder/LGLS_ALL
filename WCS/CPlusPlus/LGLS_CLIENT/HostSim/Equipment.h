// Equipment.h: interface for the CEquipment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQUIPMENT_H__EB7797B8_286B_4DE9_B0A3_10A424B42936__INCLUDED_)
#define AFX_EQUIPMENT_H__EB7797B8_286B_4DE9_B0A3_10A424B42936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>
//#include <afxtempl.h>

#include "InterfaceSk.h"
#include "InterfaceThreadSk.h"
#include "ListenerSk.h"
#include "PortObject.h"
#include "CmdMsg.h"
#include "Info.h"
#include "TrackInfo.h"

class CEcsDoc;
class CJobItem;
class CMonitorServer;
class CXmlDom;
//class CStationInfo;
//class CTrackHS;

class CEquipment : public CObject  
{
	DECLARE_DYNAMIC(CEquipment)

public:
	CEquipment(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CEquipment();

public:
	enum EN_KIND { enNone = 0, enCV = 10, enSC = 20, enRTV= 30, enRGV= 31, enRTVPOS = 35, enLIFTER = 40, enLGV = 50, enRFID = 60, enBCR = 70, enDISPLAY = 80, enLampButton = 99 };

public:
	CEcsDoc* m_pDoc;
	int m_nIndex;			// CEquipmentArray Index
	int m_nNumber;			// Equipment Number
	EN_KIND m_enKind;
	CString m_strLog;
	int m_nForkType;		// -1:미정의, 0:Single, 1:Twin, 2:Double
	int m_nTotalEquipCount;	// 다중 설비(RTVs SCs일때 설비의 전체 대수)
	CXmlDom*	m_pXml;

public:
	enum EN_EVENT { enEventKill, enEventSend, enEventSize };
	HANDLE m_hEventArray[enEventSize];
	CPortEthernet m_port;
	CWnd* m_pEquipDlg;

public:
	CInterfaceSk*	m_pSocket;			
	CListenerSk*	m_pListener;
	CWinThread*		m_pThread;
	CObList			m_cmdList;

public:
	CInterfaceThreadSk*	m_pThreadSocket;

protected:
	CCriticalSection m_csSyncThread;
	static UINT ThreadProc(LPVOID pParam);
	typedef UINT (*FPThreadProc)(LPVOID);
	FPThreadProc m_pfThreadProc;

protected:
	friend class CListenerSk;
	friend class CInterfaceSk;
	virtual void OnCloseSocket(int nErrorCode);
	virtual void OnConnectSocket(int nErrorCode);
	virtual void OnAcceptSocket(int nErrorCode);
	virtual CInterfaceSk* CreateSocket() = 0;

protected:
	friend class CInterfaceThreadSk;
	virtual void OnCloseThreadSocket(int nErrorCode);
	virtual void OnConnectThreadSocket(int nErrorCode);
	virtual CInterfaceThreadSk* CreateThreadSocket() = 0;

protected:
	virtual void AutoRunProc() = 0;
	virtual void CommandProc() = 0;

public:
	virtual void RunEquipment() { if (m_port.m_bConnect) ConnectServer(); }
	virtual void ReconnectServer() { if (!m_pSocket && m_port.m_bRetry) ConnectServer(); }
	virtual void ReconnectThreadServer() { if (!m_pThreadSocket && m_port.m_bRetry) ConnectServer(); }
	virtual void MulticastInfo(CMonitorServer* pMonitorSv) {}
	virtual BOOL IsIdleJob(int nLuggNum) { return TRUE; }
	virtual CInfo* GetInfo(int nEquipNum = 0) { return NULL; }
	virtual BOOL SetCmdMsg(CCmdMsg* pCmdMsg);
	virtual BOOL IsConnect();

public:
	BOOL RunServer();
	BOOL ConnectServer();
	void Disconnect();
	void RefreshDialog(WPARAM wParam = 0, LPARAM lParam = 0);

public:
	BOOL InitializeXmlDom();

public:
	void UpdateCommStatus(int nStatus);
	void WriteLog(int nLogType, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszPos, LPCTSTR lpszProdID = NULL, BOOL bAlarm = FALSE);
	void WriteLog(int nLogType, LPCTSTR lpszContent, LPCTSTR lpszPos, CJobItem* pJobItem = NULL, BOOL bAlarm = FALSE);
	CString GetDeviceName() { return m_port.m_strDevice; }
};

class CCmdMsg;		
class CScCmdMsg;	
class CLifterCmdMsg;	
class CRtvCmdMsg;		
class CLgvCmdMsg;		
class CEquipmentArray : public CArray<CEquipment*, CEquipment*>
{
public:
	CEquipmentArray() {}
	virtual ~CEquipmentArray() {}

public:
	CEquipment* GetEquipment(CString& strDevice);
	CEquipment* GetEquipment(int nEquipKind, int nEquipNum);
	CInfo*		GetInfo(int nEquipKind, int nEquipNum);

public:
	void		SetAllUnSetSusspend();
	void		SetAllSetSusspend();
//	void		WritePlayBack();

public:
	void		StatusReport();
	void		InvokeControl(BOOL bManual);
	void		ReconnectServers();
	void		MulticastInfo(CMonitorServer* pMonitorSv= NULL);
	BOOL		IsIdleJob(int nLuggNum);
};

#endif // !defined(AFX_EQUIPMENT_H__EB7797B8_286B_4DE9_B0A3_10A424B42936__INCLUDED_)
