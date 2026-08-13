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
	int	m_nBank;
	int	m_nBay;
	int	m_nLevel;
	int	m_nCvPlcNo;
	int	m_nDelAddr;		// 입고 H/S : 로딩 완료후 지울 CV 트랙 데이터 시작주소 (-1 : 미지정 -> 기존 계산식 사용)
	int	m_nDelCnt;		// 입고 H/S : 지울 Word 개수 ( 0 : 미지정 -> 기존 계산식 사용)
	int	m_nProdAddr;	// 입고/출고 H/S : 화물감지 Word 주소 (-1 : 미지정 -> 기존 계산식 사용)
	int	m_nDelAddr2;	// PAIR 트랙(포크#2) 삭제 시작주소 (-1 : 미지정 -> 기존 계산식 사용)
	int	m_nDelCnt2;		// PAIR 트랙(포크#2) 지울 Word 개수 ( 0 : 미지정 -> 기존 계산식 사용)
	int	m_nProdAddr2;	// PAIR 트랙(포크#2) 화물감지 Word 주소 (-1 : 미지정 -> 기존 계산식 사용)

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
