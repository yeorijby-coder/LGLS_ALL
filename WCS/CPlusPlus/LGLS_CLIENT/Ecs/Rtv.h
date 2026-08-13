// Rtv.h: interface for the CRtv class.
//

#pragma once

#include "Equipment.h"
#include "RTV_DATA.h"
#include "RtvInfo.h"


class CRtv : public CEquipment  
{
	DECLARE_DYNAMIC(CRtv)

public: //CEquipment(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice, LPCTSTR lpszDevName);
	CRtv(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex);
	virtual ~CRtv();

public:
	CRtvInfo* m_pInfo;
	CEcsDoc* m_pDoc;
	CEquipment* m_pEquipment;
	virtual CInfo* GetInfo() { return m_pInfo; }

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();
	
public:
	CString GetSelectQry();
	void SetVar(CRecordSetWrap* pRsw);
	BOOL GetModified();
};
