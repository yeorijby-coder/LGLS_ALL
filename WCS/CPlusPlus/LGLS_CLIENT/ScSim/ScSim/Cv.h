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
#include "MelsecSerialSk.h"
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
	int		m_nRWSTemp;
	BOOL	m_bSeperator;

	virtual CInfo* GetInfo() { return m_pInfo; }
	virtual BOOL IsIdleJob(int nLuggNum) { return m_pInfo->m_pTracks.IsIdleJob(nLuggNum); }

protected:
	virtual void AutoRunProc(int nConnNum);
	virtual CInterfaceSk* CreateSocket(int nIndex) 
	{ 
	//	return new CMelsecEthernetSk(this, nIndex); 
		if (m_pInfo->m_bEtherNet == TRUE)
			return new CMelsecEthernetSk(this, nIndex);
		else
			return new CMelsecSerialSk(this, nIndex);
	}
	virtual void CommandProc();
private:
	BOOL CheckRequest(int nConnNum);
	BOOL ReadStatus(int nConnNum);
	BOOL WriteDoorOpen(int nIndex);
	BOOL WriteDoorClose(int nIndex);
	BOOL WriteFireAlarm(int nTrackNum, int nVal);
	BOOL DeleteWordAtCnt(int nAddress, int nCnt);	// 5워드까지만 제한(함수내에서) 
	void ApplyHsAddrOverride(int nTrackNum, int nType, int& nDeleteWordAddr, int& nDeleteWordCnt, int& nProdWordAddr);	// SC.xml HS의 DEL_ADDR/DEL_CNT/PROD_ADDR 적용 (nType => 1:입고HS, 2:출고HS)

public:
	BOOL WriteWordValue(int nAddress, WORD wWord, BOOL bOtherWay = FALSE);
	BOOL WriteTrackInfo(int nTrackNum, int nLuggNum, int nJobType, int nDestPos, int nDeadLockCheck = 0, int nSize = 0 );
	BOOL WriteTrackInfo(CTrackInfo* pTrack, CJobItem* pJobItem, int nDestTrackNum = 0, int nDeadLockCheck = 0, int nSize = 0);

private:
	void InvokeStoStation(CTrackInfo* pTrack, CStationInfo* pStation);
	void InvokeRetStation(CTrackInfo* pTrack, CStationInfo* pStation);
	void InvokeScRetHS(CTrackInfo* pTrack, CTrackHS* pTrackHS);
	void InvokeRtvArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS);
	void InvokeLifterArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS);
	void InvokeLgvArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS);
	void InvokeArriveReport(CTrackInfo* pTrack);
};

#endif // !defined(AFX_CV_H__9EFBEB19_E3E6_4CAE_A649_ADE083D07600__INCLUDED_)
