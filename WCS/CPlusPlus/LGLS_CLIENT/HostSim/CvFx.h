// CvFx.h: interface for the CCv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CVFX_H__9EFBEB19_E3E6_4CAE_A649_ADE083D07600__INCLUDED_)
#define AFX_CVFX_H__9EFBEB19_E3E6_4CAE_A649_ADE083D07600__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Equipment.h"
#include "MelsecFXEthernetSk.h"
#include "CvInfo.h"

class CCvFx : public CEquipment  
{
	DECLARE_DYNAMIC(CCvFx)

public:
	CCvFx(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CCvFx();

	enum { enDeviceCodeR = 0xAF };

public:
	CCvInfo* m_pInfo;
	BOOL	m_bStoreRequesting[4];
	BOOL	m_bRetrieveRequesting[3];
	BOOL	m_bStorePLTRequesting[5];
	CCriticalSection m_csSyncPLC;

	virtual CInfo* GetInfo(int nEquipNum = 0) { return m_pInfo; }
	virtual void MulticastInfo(CMonitorServer* pMonitorSv);
	virtual BOOL IsIdleJob(int nLuggNum) { return m_pInfo->m_pTracks.IsIdleJob(nLuggNum); }

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();
	virtual CInterfaceSk* CreateSocket() { 
		return new CMelsecFXEthernetSk(this); 
	}
	virtual CInterfaceThreadSk* CreateThreadSocket() {
		return new CInterfaceThreadSk(this);
	}

private:
	BOOL ReadStatus();
public:
	BOOL WriteWordValue(int nAddress, WORD wWord, BOOL bOtherWay = FALSE);
	BOOL WriteLampStatus(int nIndex, BOOL bOn) { return WriteWordValue(nIndex + 1, (WORD)0x0001 & bOn); }
	BOOL WriteLampCommStatus(BOOL bOn) { return WriteWordValue(0, (WORD)0x0001 & bOn); }

};

#endif // !defined(AFX_CVFX_H__9EFBEB19_E3E6_4CAE_A649_ADE083D07600__INCLUDED_)
