// StationInfo.h: interface for the CStationInfo class.
//

#pragma once

#include <afxtempl.h>

class CEquipment;
class CTrackInfo;


// CStationInfo
//
class CStationInfo : public CObject
{
	DECLARE_DYNAMIC(CStationInfo)

public:
	enum EN_KIND { enScStation, enStoStation, enRetStation, enArvStation };
	CStationInfo(EN_KIND enKind, CEquipment* pEquipment, CTrackInfo* pTrack = NULL);
	virtual ~CStationInfo();

public:
	EN_KIND m_enKind;
	CEquipment* m_pEquipment;
	CTrackInfo* m_pTrack;		// enScStation인경우 NULL

public:
	CString m_strID;
	CString m_strName;
	int		m_nBankPos;		// Crane 사용 Bank 위치( 1 : 1열, 2 : 2열, 3 : All )
	int		m_nBankCnt;		// Crane 전체 Bank 수량
	int		m_nBayCnt;		// Crane 전체 Bay 수량
	int		m_nLevelCnt;	// Crane 전체 Level 수량
//	CString m_strTrayId;
	int		m_nNextTrack;
	int		m_nStationUse;
public:
	enum { enLengthID = 5 };
	static BOOL IsValidID(CString& strStationID);
	static int GetWarehouseNo(CString& strStationID);
	int GetWarehouseNo() { return GetWarehouseNo(m_strID); } 
	int GetTrackDestination();
	// Crane 전체 Bank 수량 구하기
	int GetBankCount() { return (m_nBankCnt < 1) ? 0 : m_nBankCnt; }
	// Crane 전체 Bay 수량 구하기
	int GetBayCount() { return (m_nBayCnt < 1) ? 0 : m_nBayCnt; }
	// Crane 전체 Level 수량 구하기
	int GetLevelCount() { return (m_nLevelCnt < 1) ? 0 : m_nLevelCnt; }
	// Crane 사용 Bank 위치 구하기
	int GetBankPos() { return (m_nBankPos < 1) ? 0 : m_nBankPos; }
};


// CStationInfoArray
//
class CStationInfoArray : public CArray<CStationInfo*, CStationInfo*>
{
public:
	CStationInfoArray() {}
	virtual ~CStationInfoArray() {}

public:
	BOOL IsValidStartStation(int nJobPattern, CString& strStationID);
	BOOL IsValidDestStation(int nJobPattern, CString& strStationID);
	CStationInfo* GetStationInfo(CString& strStationID);
	CStationInfo* GetScStationInfo(int nWarehouse, int nScNo);
	CString GetStationName(CString& strStationID);
	CStationInfo* GetStationInfoByTrackNo(int nTrackNo);
};
