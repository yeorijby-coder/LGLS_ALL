// Cv.h: interface for the CCv class.
//
#pragma once

#include "Equipment.h"
#include "MelsecEthernetSk.h"
#include "CvInfo.h"


class CCv : public CEquipment  
{
	DECLARE_DYNAMIC(CCv)

public:
	CCv(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex);
	virtual ~CCv();

public:
	CCvInfo* m_pInfo;
	CEcsDoc* m_pDoc;
	virtual CInfo* GetInfo() { return m_pInfo; }

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();
	
	bool ReadStatusDB();
	//	void InvokeTrayPickerUpNPass(CTrackInfo* pTrack, CStationInfo* pStation);
public:
	CString m_strInPlc;
	CString GetSelectQry();
	void SetVar(CRecordSetWrap* pRsw);
	BOOL GetModified();
	CString m_strBypassChkYn;

};
