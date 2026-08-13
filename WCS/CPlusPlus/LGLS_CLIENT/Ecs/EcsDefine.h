// EcsDefine.h: interface for the CEcsDefine class.
//

#pragma once

#include "XmlDom.h"
#include "TrackInfo.h"
#include "TrayPicker.h"
 
#include "Cv.h"

class CEcsDoc;
class CEquipment;
//class CTrackInfo;

class CEcsDefine : public CXmlDom  
{
public:
	CEcsDefine(CEcsDoc* pDoc);
	virtual ~CEcsDefine();

protected:
	CEcsDoc* m_pDoc;

protected:
	CEquipment* CreateEquipment(CString& strClassName, CString strThreadNo, int nIndex);

protected:

	BOOL ParseCv(CEquipment* pEquipment);
	BOOL ParseSc(CEquipment* pEquipment, int nForkType = 0);
	BOOL ParseRtv(CEquipment* pEquipment);
	BOOL ParseRfid(CEquipment* pEquipment);
	BOOL ParseBcr(CEquipment* pEquipment);
	BOOL ParseScPair(CEquipment* pEquipment);
	BOOL ParseScSingle(CEquipment* pEquipment);
	BOOL ParseWc(CEquipment* pEquipment);

private:
	BOOL ParseTracks(CCv* pEquipment);
	void ParseDestStation(int nFrom, int nTo, CStringArray& strDestStations);

public:
	BOOL ParseXml();
};
