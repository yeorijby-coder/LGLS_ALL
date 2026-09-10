#pragma once

#include "Equipment.h"
#include "WcInfo.h"


class CWc : public CEquipment
{
	DECLARE_DYNAMIC(CWc)

public:
	CWc(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex);
	virtual ~CWc();

public:
	CWcInfo* m_pInfo;
	virtual CInfo* GetInfo() { return m_pInfo; }

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();

public:
	CString GetSelectQry();
	void SetVar(CRecordSetWrap* pRsw);

public:
	CString m_WH_TYP;
	int m_nNumber;
};

