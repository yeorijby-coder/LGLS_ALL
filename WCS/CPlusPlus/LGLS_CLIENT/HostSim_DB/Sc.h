// Sc.h: interface for the CSc class.
//

#pragma once

#include "Equipment.h"
#include "ScInfo.h"
#include "SC_DATA.h"


class CSc : public CEquipment  
{
	DECLARE_DYNAMIC(CSc)

public:
	CSc(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex);
	virtual ~CSc();

public:
	CScInfo* m_pInfo;
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
