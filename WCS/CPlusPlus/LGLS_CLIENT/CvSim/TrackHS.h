// TrackHS.h: interface for the CTrackHS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKHS_H__CC48611A_E3D9_43B3_B2C8_8E6952549C80__INCLUDED_)
#define AFX_TRACKHS_H__CC48611A_E3D9_43B3_B2C8_8E6952549C80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>
//#include <afxtempl.h>

class CEquipment;
class CTrackInfo;
class CTrackHS : public CObject
{
public:
	CTrackHS(CEquipment* pEquipment);
	virtual ~CTrackHS();

public:
	int	m_nPos;
	int m_nKind;
	BOOL m_bDeadLockTrack;
	BOOL m_bBufferTrack;
	int m_nBufferMatchingTrack;
	CString m_strName;
	CTrackInfo* m_pTrack;
	CEquipment* m_pEquipment;
	CStringArray m_strDestStations;
	int	m_nPair;

public:
	void AddDestStationID(CString& strFrom, CString& strTo);
	BOOL IsDestStationID(CString& strDestStationID);
	CString ToString();
};

class CTrackHSArray : public CArray<CTrackHS*, CTrackHS*>
{
public:
	CTrackHSArray() : m_nIndex(0) {}
	virtual ~CTrackHSArray() { ClearArray(); }

public:
	void ClearArray();
	void SetArraySize(int nSize);
	int GetIndex() { return m_nIndex; }

public:
	CTrackHS* NextTrackHS();
	CTrackHS* FirstTrackHS();
	CTrackHS* LastTrackHS();
	CTrackHS* CurrentTrackHS();
	CTrackHS* GetNextTrackHS();
	CTrackHS* FindTrackHS(CString& strDestStationID);
	CTrackHS* FindTrackHS(int nDepartPos, CString& strDestStationID);
	CTrackHS* FindByTrack(int nTrackNum);
	CTrackHS* FindByPos(int nPos);

protected:
	int m_nIndex;
};

#endif // !defined(AFX_TRACKHS_H__CC48611A_E3D9_43B3_B2C8_8E6952549C80__INCLUDED_)
