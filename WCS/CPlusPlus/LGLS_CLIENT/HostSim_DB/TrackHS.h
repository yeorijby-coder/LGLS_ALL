// TrackHS.h: interface for the CTrackHS class.
//

#pragma once

#include <afxcoll.h>
#include <afxtempl.h>

class CEquipment;
class CTrackInfo;


// CTrackHS
//
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
	int m_nDestStationPos;
	int m_nDestPos;
	int m_nRetDestPos;
	int m_nStair;	// S/C 입출고 단
	int m_nReject;
	int m_nByPassTrack;
	int m_nMainTrack;
	int m_nNotUseFork;

public:
	void AddDestStationID(CString& strFrom, CString& strTo);
	BOOL IsDestStationID(CString& strDestStationID);
	CString ToString();
};


// CTrackHSArray
//
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
	CTrackHS* GetPrevTrackHS();
	CTrackHS* GetNextTrackHS();
	CTrackHS* GetNextTrackHSByTime(CTrackHS* pTrackHS);
	CTrackHS* FindTrackHS(CString& strDestStationID);
	CTrackHS* FindTrackHS(int nDepartPos, CString& strDestStationID);
	CTrackHS* FindByTrack(int nTrackNum);
	CTrackHS* FindByPos(int nPos);
	CTrackHS* FindByPosAndSetCurrentTrackHS(int nPos);

protected:
	int m_nIndex;
};
