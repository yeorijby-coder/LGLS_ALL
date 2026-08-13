// Info.h: interface for the CInfo class.
//

#pragma once

class CEquipment;

// CInfo 명령 대상입니다.
//
class CInfo : public CObject
{
	DECLARE_SERIAL(CInfo)
		
public:
	CInfo(): m_pEquipment(NULL), m_bModified(TRUE) {}
	CInfo(CEquipment* pEquipment);
	CInfo(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrEQP_NO);
	CInfo(CString pstrWH_TYP, CString pstrPLC_NO);
	virtual ~CInfo();

public:
	CEquipment* m_pEquipment;
	BOOL		m_bModified;
	BOOL		m_bManualModified;

public:
	int m_nPlcNo;
	int m_nNumber;
	CString m_strWH_TYP;

public:
	virtual void Initialize() {}
	virtual void InvokeControl() {}
	virtual CString GetStringWH_TYP() {return _T("");};
	virtual CString GetStringPLC_NO() {return _T("");};
	virtual CString GetStringEQP_NO() {return _T("");};
	virtual CString GetMapKey() {return _T("");};
	virtual CString GetMapKeyCV() { return _T("");};
	virtual CString GetMapKeyTRACK() { return _T("");};
	virtual BOOL SetMapKey(){return TRUE;};
};


