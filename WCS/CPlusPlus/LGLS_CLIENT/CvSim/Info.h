// Info.h: interface for the CInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFO_H__B1DE6482_A5DA_4C64_8E8D_1C97825E13D7__INCLUDED_)
#define AFX_INFO_H__B1DE6482_A5DA_4C64_8E8D_1C97825E13D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEquipment;
class CInfo : public CObject
{
	DECLARE_SERIAL(CInfo)
		
public:
	CInfo(): m_pEquipment(NULL), m_bModified(TRUE) {}
	CInfo(CEquipment* pEquipment);
	virtual ~CInfo();

public:
	CEquipment* m_pEquipment;
	BOOL		m_bModified;

protected:
	CCriticalSection m_csSyncBackup2;

public:
	BOOL Backup();
	BOOL Backup2(int nNo);
	BOOL Restore();
	BOOL Restore2(int nNo);

public:
	virtual void Initialize() {}
	virtual void InvokeControl() {}
	virtual void StatusReport(BOOL bManual = FALSE) {}
};

#endif // !defined(AFX_INFO_H__B1DE6482_A5DA_4C64_8E8D_1C97825E13D7__INCLUDED_)
