// Sc.h: interface for the CSc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SC_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
#define AFX_SC_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Equipment.h"
#include "ScSocket.h"
#include "MelsecEthernetSk.h"
#include "MelsecSerialSk.h"
#include "ScInfo.h"

class CSc : public CEquipment  
{
	DECLARE_DYNAMIC(CSc)

public:
	CSc(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CSc();

public:
	int m_nNumber;
	CScInfo* m_pInfo;
	BOOL m_bReset;
	BOOL m_bUnloadingComplete;
	virtual CInfo* GetInfo() { return m_pInfo; }

public:
	virtual void MulticastInfo(CMonitorServer* pMonitorSv);
	virtual BOOL IsIdleJob(int nLuggNum) { 
		return ((m_pInfo->m_ScInfo[0].m_nInternalLuggNum == 0) || (m_pInfo->m_ScInfo[0].m_nInternalLuggNum != nLuggNum)); 
	}

protected:
	virtual void AutoRunProc(int nConnNum);
	virtual void CommandProc();
	virtual CInterfaceSk* CreateSocket(int nIndex) 
	{ 
		if (m_pInfo->m_bEtherNet == TRUE)
			return new CMelsecEthernetSk(this, nIndex); 
		else
			return new CMelsecSerialSk(this, nIndex);
	}

protected:
	void InvokeCheck();
	void CompleteCheck();
	void ErrorCheck();
	void SafeCheck();
	BOOL CheckRequest(int nConnNum);
	BOOL CheckRequestSerial(int nConnNum);

protected:
	BOOL ErrorRoutine();
	BOOL StoreRoutine();
	BOOL RetrieveRoutine();
	BOOL RackToRackRoutine();
	BOOL HsToHsRoutine(CJobItem* pJobItem, CTrackHS* pStoHS);

protected:
	BOOL ReadStatus();
	BOOL Store(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0);
	BOOL Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0);
	BOOL HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nSize = 0);
	BOOL RackToRack(int nLuggNum, CString strLocFrom, CString strLocTo, int nSize = 0);
	BOOL CallToHome();

protected:
	BOOL Emergency();
	BOOL Active();
	BOOL Stop();
	BOOL Reset();
	BOOL Delete();

public:
	// 화재발생시 강제이동 명령
	BOOL FireAlarmOn();
	BOOL FireAlarmOff();

public:
	void UpdateScInfo(); 

public:
	BOOL IsValidLocation(CString& strLocation);
};

#endif // !defined(AFX_SC_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
