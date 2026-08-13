// ScDongJin.h: interface for the CScDongJin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCDONGJIN_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
#define AFX_SCDONGJIN_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Equipment.h"
//#include "ScSocket.h"
#include "MelsecEthernetSk.h"
#include "ScInfo.h"

class CScDongJin : public CEquipment  
{
	DECLARE_DYNAMIC(CScDongJin)

public:
	CScDongJin(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CScDongJin();

public:
//	Update : 2014.01
//	CScInfo* m_pInfo;
	CArray<CScInfo*, CScInfo*> m_pInfo;
//	virtual CInfo* GetInfo() { return m_pInfo; }


	virtual CInfo* GetInfo(int nEuipNum, BOOL bFirst) 
	{ 
		CScInfo* pInfo = NULL;
		int nConnection = 0;
		int nCount = m_pInfo.GetSize();
		for (int i = 0 ; i < nCount ; i++) 
		{
			pInfo = m_pInfo.GetAt(i);

			if (pInfo == NULL)
				continue;

			if (bFirst == FALSE)
				nConnection = (i + 1) + ((pInfo->m_pEquipment->m_nNumber-1) * 3);
			else
				nConnection = pInfo->m_nEquipNum;

			if (nConnection == nEuipNum)
				return pInfo;
		}

		return	NULL;		//m_pInfo[0];		// ¿øº»
	}
//*/

public:
	virtual void MulticastInfo(CMonitorServer* pMonitorSv);
	/*
	virtual BOOL IsIdleJob(int nLuggNum, int nEquipNum) 
	{
//		GetInfoIndex(nEquipNum);
		CScInfo* pInfo = (CScInfo*) GetInfo(nEquipNum, FALSE);
		
		if (pInfo == NULL)
			return FALSE;

		return ((pInfo->m_ScInfo[0].m_nInternalLuggNum == 0) || (pInfo->m_ScInfo[0].m_nInternalLuggNum != nLuggNum)); 
	}
	*/
	BOOL IsIdleJob(int nLuggNum); 


	int GetInfoIndex(int nNumber);
	void StartUpRequest(CScInfo* pInfo);

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();
	virtual CInterfaceSk* CreateSocket() { 
		return new CMelsecEthernetSk(this); 
	}
	virtual CInterfaceThreadSk* CreateThreadSocket() {
		return new CInterfaceThreadSk(this);
	}

protected:
	void InvokeCheck(CScInfo* pInfo = NULL);
	void CompleteCheck(CScInfo* pInfo = NULL);
	void ErrorCheck(CScInfo* pInfo = NULL);
	void SafeCheck(CScInfo* pInfo = NULL);
	void ErrorResetCheck(CScInfo* pInfo);

protected:
	BOOL ErrorRoutine(CScInfo* pInfo = NULL);
	BOOL StoreRoutine(CScInfo* pInfo = NULL);
	BOOL RetrieveRoutine(CScInfo* pInfo = NULL);
	BOOL RackToRackRoutine(CScInfo* pInfo = NULL);
	BOOL HsToHsRoutine(CJobItem* pJobItem, CTrackHS* pStoHS, CScInfo* pInfo = NULL);

protected:
	BOOL ReadStatus(CScInfo* pInfo = NULL);
//	BOOL Store(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0, BOOL bStoHS = FALSE, BOOL bRetHS = FALSE);
//	BOOL Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0, BOOL bStoHS = FALSE, BOOL bRetHS = FALSE);
//	BOOL HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nSize = 0, BOOL bStoHS = FALSE, BOOL bRetHS = FALSE);
	BOOL Store(int nLuggNum, CString strLocation, int nHsNum, int n, int nSize = 0);
	BOOL Retrieve(int nLuggNum, CString strLocation, int nHsNum, int n, int nSize = 0);
	BOOL HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int n, int nSize = 0);
	BOOL RackToRack(int nLuggNum, CString strLocFrom, CString strLocTo, int n, int nSize = 0);
	BOOL CallToHome(int n);

	int GetHsNum(BYTE ucCmdType, int nNumber, int nBank, int nBay, int nLevel);
	int GetHsNum(BYTE ucCmdType, int nNumber, CString strLocation) 
	{ return GetHsNum(ucCmdType, nNumber, _ttoi(strLocation.Left(2)), _ttoi(strLocation.Mid(2,3)), _ttoi(strLocation.Right(2))); }

protected:
	BOOL Emergency(int n);
	BOOL Active(CScInfo* pInfo = NULL);
	BOOL Stop(int n);
	BOOL Reset(int n);
	BOOL Delete(int n);
	BOOL CompleteReset(int n);

public:
	BOOL IsValidLocation(CString& strLocation);
};

#endif // !defined(AFX_SCDONGJIN_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
