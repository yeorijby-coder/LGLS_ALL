// EcsDefine.h: interface for the CEcsDefine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECSDEFINE_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_)
#define AFX_ECSDEFINE_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XmlDom.h"
#include "TrackInfo.h"

class CEcsDoc;
class CEquipment;
class CEcsDefine : public CXmlDom  
{
public:
	CEcsDefine(CEcsDoc* pDoc);
	virtual ~CEcsDefine();

protected:
	CEcsDoc* m_pDoc;

protected:
	CEquipment* CreateEquipment(CString& strClassName, int nIndex, int nNumber, LPCTSTR lpszDevice);

protected:
	BOOL ParseCv(CEquipment* pEquipment);
	BOOL ParseCvFx(CEquipment* pEquipment);

	BOOL ParseSc(CEquipment* pEquipment, int nForkType = 0);
	BOOL ParseScPair(CEquipment* pEquipment);
	BOOL ParseScSingle(CEquipment* pEquipment);
	BOOL ParseScDongJinSingle(CEquipment* pEquipment);

	BOOL ParseRtv(CEquipment* pEquipment);
	BOOL ParseRtvZaion(CEquipment* pEquipment);

	BOOL ParseLifter(CEquipment* pEquipment);
	BOOL ParseLgv(CEquipment* pEquipment);
	BOOL ParseRgv(CEquipment* pEquipment);
	BOOL ParseRfid(CEquipment* pEquipment);
	BOOL ParseBcr(CEquipment* pEquipment);
	BOOL ParseDisplay(CEquipment* pEquipment);
	BOOL ParseStation();


private:
	BOOL ParseTracks(CEquipment* pEquipment, CTrackInfoArray& pTracks);
	void ParseDestStation(int nFrom, int nTo, CStringArray& strDestStations);

public:
	BOOL ParseXml();
};

#endif // !defined(AFX_ECSDEFINE_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_)
