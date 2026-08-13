// Lgv.h: interface for the CLgv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LGV_H__149898A0_73F7_449A_835A_5B63A5F7D8B0__INCLUDED_)
#define AFX_LGV_H__149898A0_73F7_449A_835A_5B63A5F7D8B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Equipment.h"
//#include "LgvSocket.h"
#include "LgvInfo.h"

class CLgv : public CEquipment  
{
	DECLARE_DYNAMIC(CLgv)

public:
	CLgv(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CLgv();

public:
	CLgvInfo* m_pInfo;
//	int m_nTemp1;
//	int m_nTemp2;
	virtual CInfo* GetInfo(int nEquipNum = 0) { return m_pInfo; }
	virtual void MulticastInfo(CMonitorServer* pMonitorSv = NULL);		//

public:
//	virtual void RunEquipment() { if (m_port.m_bConnect) CEquipment::RunServer(); }
//	virtual void ReconnectServer() { if (!m_pListener && m_port.m_bRetry) CEquipment::RunServer(); }
	virtual BOOL IsIdleJob(int nLuggNum) { return m_pInfo->m_oLgvVehicles.IsIdleJob(nLuggNum); }

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();
	virtual CInterfaceSk* CreateSocket() {
		return new CInterfaceSk(this);
	}
	virtual CInterfaceThreadSk* CreateThreadSocket() {
		return new CInterfaceThreadSk(this);
	}

protected:
	void InvokeDoorOpenRequest(int nDoorNum = 0);
	void InvokeDoorCloseRequest(int nDoorNum = 0);
	void InvokeCheck();


protected:
	BOOL RequestStop(int nStop = 0);
	BOOL InvokeJob(int nLuggNum, int nSource, int nDest, CString strProdInfo="");
	BOOL ReadProcess();
	BOOL ParseMessageStop();
	BOOL ParseMessageTransfer();

//	BOOL RequestTransport(int nLuggNum, int nCrateSize, int nPriority, int nPickPos, int nDropPos, BOOL bResent = FALSE);	// TRR
//	BOOL RequestModifyTransport(int nLuggNum, int nCrateSize, int nPriority, int nPickPos, int nDropPos, BOOL bResent = FALSE);	// MOD
//	BOOL RequestDeleteTransport(int nLuggNum, BOOL bResent = FALSE);	// DEL
//	BOOL RequestStatusTransport(int nLuggNum, BOOL bResent = FALSE);	// TRQ
//	BOOL RequestStatusVehicle(int nVehicleID, BOOL bResent = FALSE);	// AGQ
//	BOOL RequestExpectedTransport(int nPriority, int nMovePos, int nExpectedTime, BOOL bResent = FALSE);	// TEX
//	BOOL ReportStatusDoor(int nDoorID, BOOL bOpen, BOOL bResent = FALSE);
};

#endif // !defined(AFX_LGV_H__149898A0_73F7_449A_835A_5B63A5F7D8B0__INCLUDED_)
