#pragma once

#include "info.h"
#include "DciButtonCtrl.h"
#include "WC_DATA.h"


class CWcItem;
class CWcInfo : public CInfo  
{
	DECLARE_SERIAL(CWcInfo)

public:
	CWcInfo() {}
	CWcInfo(CEquipment* pEquipment);
	virtual ~CWcInfo();

public:
	BOOL			m_bModified;
	CDciButtonCtrl* m_pControl;
	CMap<CString, LPCTSTR, CWC_DATA*, CWC_DATA*> m_MapWC_DATA;
	CWC_DATA*       m_pWC_DATA;

	CWC_DATA* CreateWC_DATA( CString pstrPLC_NO, CString pstrWC_MC_NO );
	CString GetStringWC_MC_NO(CString pstrWC_MC_NO);
};

