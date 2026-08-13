// ScPair.h: interface for the CScPair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCPAIR_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
#define AFX_SCPAIR_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sc.h"
#include "ScSocket.h"
#include "MelsecEthernetSk.h"

class CScPair : public CSc  
{
	DECLARE_DYNAMIC(CScPair)

public:
	CScPair(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CScPair();

public:
//	CScPairInfo* m_pInfo;
	int m_nNumber;
	CScInfo* m_pInfo;
	virtual CInfo* GetInfo() { return m_pInfo; }

public:
	virtual void MulticastInfo(CMonitorServer* pMonitorSv);
	virtual BOOL IsIdleJob(int nLuggNum) { 
		return ((m_pInfo->m_ScInfo[0].m_nInternalLuggNum == 0) || (m_pInfo->m_ScInfo[0].m_nInternalLuggNum != nLuggNum) ||
				(m_pInfo->m_ScInfo[1].m_nInternalLuggNum == 0) || (m_pInfo->m_ScInfo[1].m_nInternalLuggNum != nLuggNum) ); 
	}

	virtual void AutoRunProc(int nConnNum);
protected:
	virtual void CommandProc();
	virtual CInterfaceSk* CreateSocket(int nIndex) { return new CMelsecEthernetSk(this, nIndex); }

protected:
	void InvokeCheck();
	void CompleteCheck();
	void CompleteCheck1();
	void CompleteCheck2();
	void ErrorCheck();
	void SafeCheck();

	BOOL CheckRequest(int nConnNum);
	void UpdateScInfo(); 

//	void SwapJobItem(CJobItem* pJobItem, CJobItem* pJobItem1, CJobItem* pJobItem2);
	BOOL ErrorDualStorProcess(int nFork);
	BOOL ErrorEmptyRetrieveProcess(int nFork);

protected:
	BOOL ErrorRoutine();
	BOOL StoreRoutine();
	BOOL RetrieveRoutine();
	BOOL RackToRackRoutine();
	BOOL HsToHsRoutine(CJobItem* pJobItem, CTrackHS* pStoHS);

protected:
	BOOL ReadStatus();
//	BOOL Store(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0, BOOL bStoHS = FALSE, BOOL bRetHS = FALSE);
//	BOOL Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0, BOOL bStoHS = FALSE, BOOL bRetHS = FALSE);
//	BOOL HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nSize = 0, BOOL bStoHS = FALSE, BOOL bRetHS = FALSE);
	BOOL Store(int nLuggNum, CString strLocation, int nHsNum, int nLuggNum2 = 0, CString strLocation2 = "0000000", char bUseFork = '0', int nSize = 0);
	BOOL Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nLuggNum2 = 0, CString strLocation2 = "0000000", char bUseFork = '0', int nSize = 0);
	BOOL HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nLuggNum2 = 0, char bUseFork = '0', int nSize = 0);
	BOOL RackToRack(int nLuggNum, CString strLocFrom, CString strLocTo, int nLuggNum2, CString strLocFrom2 = "0000000", CString strLocTo2 = "0000000", char bUseFork = '0', int nSize = 0);
	BOOL CallToHome();

protected:
	BOOL Emergency();
	BOOL Active();
	BOOL Stop();
	BOOL Reset();
	BOOL Delete(int nForkValue);

public:
	BOOL IsValidLocation(CString& strLocation);
};

#endif // !defined(AFX_SCPAIR_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
