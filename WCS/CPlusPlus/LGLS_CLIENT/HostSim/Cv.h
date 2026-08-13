// Cv.h: interface for the CCv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CV_H__9EFBEB19_E3E6_4CAE_A649_ADE083D07600__INCLUDED_)
#define AFX_CV_H__9EFBEB19_E3E6_4CAE_A649_ADE083D07600__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Equipment.h"
#include "MelsecEthernetSk.h"
#include "MelsecEthernetThreadSk.h"
#include "CvInfo.h"

class CCv : public CEquipment  
{
	DECLARE_DYNAMIC(CCv)

public:
	CCv(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CCv();

	enum { enDeviceCodeR = 0xAF };

public:
	CCvInfo* m_pInfo;
	BOOL	m_bStoreRequesting[4];
	BOOL	m_bRetrieveRequesting[3];
	BOOL	m_bStorePLTRequesting[7];
	CCriticalSection m_csSyncPLC;

	virtual CInfo* GetInfo(int nEquipNum = 0) { return m_pInfo; }
	virtual void MulticastInfo(CMonitorServer* pMonitorSv);
	virtual BOOL IsIdleJob(int nLuggNum) { return m_pInfo->m_pTracks.IsIdleJob(nLuggNum); }

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();
	virtual CInterfaceSk* CreateSocket() { 
		return new CMelsecEthernetSk(this); 
	}
	virtual CInterfaceThreadSk* CreateThreadSocket() {
		return new CInterfaceThreadSk(this);
	}

private:
	BOOL ReadStatus();
	BOOL WriteSuspend(int nTrackNum, BOOL bSuspend);
	BOOL WriteDeadLock(int nTrackNum, BOOL bDeadLock);
//	BOOL WriteDeadLockCheck(int nTrackNum, BOOL bDeadLock);
	BOOL WriteError(int nTrackNum, int nError);
	BOOL WriteTrackInfo(int nTrackNum, int nLuggNum, int nJobType, int nDestPos, int nDeadLockCheck = 0, int nSize = 0 );
	BOOL WriteTrackInfo(CTrackInfo* pTrack, CJobItem* pJobItem, int nDestTrackNum = 0, int nDeadLockCheck = 0, int nSize = 0);
	BOOL WriteDoorStatus(WORD wDoorStatus) { return WriteWordValue(1001, wDoorStatus); }
//	BOOL WriteInPermission(WORD wTTStatus) { return WriteWordValue(1004, wTTStatus); }
//	BOOL WriteInPermissionRequest(WORD wTTStatus) { return WriteWordValue(1004, wTTStatus); }
//	BOOL WriteDoorOpen(int nIndex);
	BOOL WriteDoorOpenAccept(int nIndex);
	BOOL WriteDoorOpenReq(int nIndex);
	BOOL WriteDoorClose(int nIndex);

public:
	BOOL WriteWordValue(int nAddress, WORD wWord, BOOL bOtherWay = FALSE);
	BOOL WriteRgvStatus(BOOL bStop);
//	BOOL WritePLCWordValue(int nPlcNum, int nAddress, WORD wWord);
//	BOOL WriteLampStatus(int nIndex, BOOL bOn) { return WriteWordValue(nIndex + 1, (WORD)0x0001 & bOn); }
//	void SetLampInfoDoor(BOOL bOn);

private:
	void InvokeWeight(CTrackInfo* pTrack);
	void InvokeStoStation(CTrackInfo* pTrack, CStationInfo* pStation);
	void InvokeRetStation(CTrackInfo* pTrack, CStationInfo* pStation);
	void InvokeScRetHS(CTrackInfo* pTrack, CTrackHS* pTrackHS);
	void InvokeRtvArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS);
	void InvokeLifterArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS);
	void InvokeLgvArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS);
	void InvokeArriveReport(CTrackInfo* pTrack);
};

#endif // !defined(AFX_CV_H__9EFBEB19_E3E6_4CAE_A649_ADE083D07600__INCLUDED_)
