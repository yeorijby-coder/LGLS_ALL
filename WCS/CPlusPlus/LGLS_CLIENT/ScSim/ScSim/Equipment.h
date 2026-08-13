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
#include "ListenerSk.h"
#include "PortObject.h"
#include "CmdMsg.h"
#include "Info.h"
#include "TrackInfo.h"
#include "MelsecSerial.h"

class CEcsDoc;
class CJobItem;
class CMonitorServer;

//class CStationInfo;
//class CTrackHS;

class CEquipment : public CObject  
{
	DECLARE_DYNAMIC(CEquipment)

public:
	CEquipment(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CEquipment();

public:
	enum EN_KIND { 
		enNone = 0, 
		enCV = 10, 
		enCV1 = 11, 
		enEQUIP = 15, 
		enSC = 20, 
		enRTV= 30, 
		enLIFTER = 40, 
		enLGV = 50, 
		enRFID = 60, 
		enBCR = 70, 
		enDISPLAY = 80, 
		enTHS = 90 };

public:
	CEcsDoc* m_pDoc;
	int m_nIndex;		// CEquipmentArray Index
	int m_nNumber;		// Equipment Number
	int m_nStTrNum;

	EN_KIND m_enKind;	// Equipment Type
	CString m_strLog;
	int m_nForkType;
	BOOL m_bThreadOut[PLC_CONN_PORT_CNT];
	CMelsecSerial* m_pSerialComm;

public:
	enum EN_EVENT { enEventSend, 
		// enEventSend1, enEventSend2, enEventSend3, 
		// enEventSend4, enEventSend5, enEventSend6, 
		// enEventSend7, enEventSend8, enEventSend9, 
		// enEventSend10, enEventSend11, enEventSend12, 
		// enEventSend13, enEventSend14, enEventSend15, 
		// enEventSend16, enEventSend17, enEventSend18, 
		enEventKill, 
		enEventSize };
	HANDLE m_hEventArray[PLC_CONN_PORT_CNT][enEventSize];
	HANDLE m_hEventSerialArray[enEventSize];
	CPortEthernet m_port;
	CPortSerial m_portSerial;
	CWnd* m_pEquipDlg;

public:
	CInterfaceSk*	m_pSocket[PLC_CONN_PORT_CNT];
	CListenerSk*	m_pListener[PLC_CONN_PORT_CNT];
	CWinThread*		m_pThread[PLC_CONN_PORT_CNT];
	CWinThread*		m_pSerialThread;
	CObList			m_cmdList;
	CObList			m_pSocketList;

protected:
	CCriticalSection m_csSyncThread;

	static UINT ThreadProc(LPVOID pParam);
	typedef UINT (*FPThreadProc)(LPVOID);
	FPThreadProc m_pfThreadProc[PLC_CONN_PORT_CNT];

	static UINT ThreadSerialProc(LPVOID pParam);
	typedef UINT(*FPThreadSerialProc)(LPVOID);
	FPThreadSerialProc m_pfThreadSerialProc;

protected:
	friend class CListenerSk;
	friend class CInterfaceSk;
	virtual void OnCloseSocket(int nErrorCode, int nIndex);
	virtual void OnConnectSocket(int nErrorCode, int nIndex);
	virtual void OnAcceptSocket(int nErrorCode, int nIndex);
	virtual CInterfaceSk* CreateSocket(int nIndex) = 0;	//' = 0 ' 의 경우 재정의를 무조건 해줘야함

protected:
	virtual void AutoRunProc(int nConnNum) = 0;

public:
	virtual void RunEquipment() 
	{ 
		for(int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
		{
			if (m_port.m_bConnect) 
				ConnectServer(i); 
		}
	}
	virtual void ReconnectServer() 
	{ 
		for(int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
		{
			if (!m_pSocket[i] && m_port.m_bRetry) 
				ConnectServer(i);
			else if (m_pSocket[i] != NULL && m_pSocket[i]->m_enCommStatus != CAsyncSocketEx::enStatusConnectOK)
				ConnectServer(i);
		}
	}
	virtual void MulticastInfo(CMonitorServer* pMonitorSv) {}
	virtual BOOL IsIdleJob(int nLuggNum) { return TRUE; }
	virtual CInfo* GetInfo() { return NULL; }
	virtual void SetCmdMsg(CCmdMsg* pCmdMsg);
	virtual BOOL IsConnect(int nConnNum = 0);

public:
	BOOL RunServer(int nIndex);
	void RunThread();
	void RunSerialThread();
	void KillThread();
	void KillSerialThread();
	BOOL ConnectServer(int nIndex);
	void Disconnect(int nIndex = -1);
	void RefreshDialog(WPARAM wParam = 0, LPARAM lParam = 0);

public:
	void UpdateCommStatus(int nStatus, int nNum = 0);
	void WriteLog(int nLogType, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszPos, LPCTSTR lpszProdID = NULL, BOOL bAlarm = FALSE);
	void WriteLog(int nLogType, LPCTSTR lpszContent, LPCTSTR lpszPos, CJobItem* pJobItem = NULL, LPCTSTR lpszProdID = NULL, BOOL bAlarm = FALSE);
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
//	CTrackInfo* GetDeadLockTrack(int nTrackNum);
	CEquipment* GetEquipment(int nEquipKind, int nEquipNum);

public:
	void		SetAllUnSetSusspend();
	void		SetAllSetSusspend();

public:
	void		StatusReport();
	void		InvokeControl(BOOL bManual);
	void		ReconnectServers();
	BOOL		IsIdleJob(int nLuggNum);
	void		Load(CReportCtrl* pReportCtrl);
};

#endif // !defined(AFX_EQUIPMENT_H__EB7797B8_286B_4DE9_B0A3_10A424B42936__INCLUDED_)
