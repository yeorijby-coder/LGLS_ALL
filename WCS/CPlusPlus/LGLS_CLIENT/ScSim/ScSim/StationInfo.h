// StationInfo.h: interface for the CStationInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATIONINFO_H__029086BC_58C0_458D_BBE6_4B60F706B572__INCLUDED_)
#define AFX_STATIONINFO_H__029086BC_58C0_458D_BBE6_4B60F706B572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxtempl.h>

class CEquipment;
class CTrackInfo;
class CStationInfo : public CObject
{
	DECLARE_DYNAMIC(CStationInfo)

public:
	enum EN_KIND { enScStation, enStoStation, enArvStation, enArvStation2, enRetStation, enFullStation, enLgvStation, enEquipStation, enStoHSStation, enRetHSStation  };
	CStationInfo(EN_KIND enKind, CEquipment* pEquipment, CTrackInfo* pTrack = NULL);
	virtual ~CStationInfo();

public:
	EN_KIND m_enKind;
	CEquipment* m_pEquipment;
	CTrackInfo* m_pTrack;		// enScStationÀÎ°æ¿ì NULL

public:
	CString m_strID;
	CString m_strName;
	CString m_strTrayId;
	int m_nSt;
	int m_nDest;

public:
	enum { enLengthID = 5 };
	static BOOL IsValidID(CString& strStationID);
	static int GetWarehouseNo(CString& strStationID);
	int GetWarehouseNo() { return GetWarehouseNo(m_strID); } 
	int GetTrackDestination();
};

class CStationInfoArray : public CArray<CStationInfo*, CStationInfo*>
{
public:
	CStationInfoArray() {}
	virtual ~CStationInfoArray() {}

public:
	BOOL IsValidStartStation(int nJobPattern, CString& strStationID);
	BOOL IsValidDestStation(int nJobPattern, CString& strStationID);
	CStationInfo* GetStationInfo(CString& strStationID);
	CStationInfo* GetScStationInfo(int nWarehouse, CString& strLocation);
	CString GetStationName(CString& strStationID);
};

#endif // !defined(AFX_STATIONINFO_H__029086BC_58C0_458D_BBE6_4B60F706B572__INCLUDED_)
