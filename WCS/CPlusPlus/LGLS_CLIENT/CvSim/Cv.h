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
#include "CvInfo.h"

class CCv : public CEquipment  
{
	DECLARE_DYNAMIC(CCv)

public:
	CCv(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CCv();

public:
	CCvInfo* m_pInfo;
	BOOL	m_bStoreRequesting;
	BOOL	m_bRetrieveRequesting;
	CCriticalSection m_csSyncPLC;
	int		m_nPlcNum;
	
	virtual CInfo* GetInfo() { return m_pInfo; }
	virtual BOOL IsIdleJob(int nLuggNum) { return m_pInfo->m_pTracks.IsIdleJob(nLuggNum); }

protected:
	virtual void AutoRunProc(int nConnNum);
	virtual CInterfaceSk* CreateSocket(int nIndex) { return new CMelsecEthernetSk(this, nIndex); }

	//virtual CInterfaceSk1* CreateSocket1() { return new CMelsecEthernetSk1(this); }
	//virtual CInterfaceSk2* CreateSocket2() { return new CMelsecEthernetSk2(this); }
	//virtual CInterfaceSk3* CreateSocket3() { return new CMelsecEthernetSk3(this); }
	//virtual CInterfaceSk4* CreateSocket4() { return new CMelsecEthernetSk4(this); }
	//virtual CInterfaceSk5* CreateSocket5() { return new CMelsecEthernetSk5(this); }
	//virtual CInterfaceSk6* CreateSocket6() { return new CMelsecEthernetSk6(this); }
	//virtual CInterfaceSk7* CreateSocket7() { return new CMelsecEthernetSk7(this); }
	//virtual CInterfaceSk8* CreateSocket8() { return new CMelsecEthernetSk8(this); }
	//virtual CInterfaceSk9* CreateSocket9() { return new CMelsecEthernetSk9(this); }
	//virtual CInterfaceSk10* CreateSocket10() { return new CMelsecEthernetSk10(this); }
	//virtual CInterfaceSk11* CreateSocket11() { return new CMelsecEthernetSk11(this); }
	//virtual CInterfaceSk12* CreateSocket12() { return new CMelsecEthernetSk12(this); }
	//virtual CInterfaceSk13* CreateSocket13() { return new CMelsecEthernetSk13(this); }
	//virtual CInterfaceSk14* CreateSocket14() { return new CMelsecEthernetSk14(this); }
	//virtual CInterfaceSk15* CreateSocket15() { return new CMelsecEthernetSk15(this); }
	//virtual CInterfaceSk16* CreateSocket16() { return new CMelsecEthernetSk16(this); }
	//virtual CInterfaceSk17* CreateSocket17() { return new CMelsecEthernetSk17(this); }
	//virtual CInterfaceSk18* CreateSocket18() { return new CMelsecEthernetSk18(this); }

private:
	BOOL CheckRequest(int nConnNum);
	BOOL ReadStatus();
	BOOL WriteSuspend(int nTrackNum, BOOL bSuspend);
	BOOL WriteDeadLock(int nTrackNum, BOOL bDeadLock);
	BOOL WriteDeadLockCheck(int nTrackNum, BOOL bDeadLock);
	BOOL WriteTrackInfo(int nTrackNum, int nLuggNum, int nJobType, int nDestPos, int nDeadLockCheck = 0, int nSize = 0 );
	BOOL WriteTrackInfo(CTrackInfo* pTrack, CJobItem* pJobItem, int nDestTrackNum = 0, int nDeadLockCheck = 0, int nSize = 0);
	BOOL WriteDoorOpen(int nIndex);
	BOOL WriteDoorClose(int nIndex);
	BOOL WriteFireAlarm(int nTrackNum, int nVal);
	// Update : 2014.06.08 - 물류흐름이 원활하지 않은 경우 상온 Aging, 충방전에서 출고된 Tray를 물류 Loop line으로 투입하지 않음.
	BOOL ReadCtrlMode();
	BOOL WriteCtrlMode(int nAging1, int nAging2, int nAging3, int nAging4, int nAging5, int nAging6, int nCde1, int nCde2);
	void UpdateOutCtrlMode();

private:
	void InvokeStation(CTrackInfo* pTrack, CStationInfo* pStation);
	void MoveNextTrackForKindNormal(CTrackInfo* pTrack);
	void MoveNextTrackForKindDiverter(CTrackInfo* pTrack);
	void InvokeRetStation(CTrackInfo* pTrack, CStationInfo* pStation);
	void InvokeScRetHS(CTrackInfo* pTrack, CTrackHS* pTrackHS);
};

#endif // !defined(AFX_CV_H__9EFBEB19_E3E6_4CAE_A649_ADE083D07600__INCLUDED_)
