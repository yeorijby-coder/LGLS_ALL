// Bcr.h: interface for the CBcr class.
//

#pragma once

#include "Equipment.h"
#include "BcrInfo.h"


class CBcr : public CEquipment
{
	DECLARE_DYNAMIC(CBcr)

public:
	CBcr(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex);
	virtual ~CBcr();

public:
	CBcrInfo* m_pInfo;
	virtual CInfo* GetInfo() { return m_pInfo; }

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();
	
public:
	CString GetSelectQry();
	void SetVar(CRecordSetWrap* pRsw);
};

