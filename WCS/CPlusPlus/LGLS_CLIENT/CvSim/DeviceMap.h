// DeviceMap.h: interface for the CDeviceMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVICEMAP_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_)
#define AFX_DEVICEMAP_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XmlDom.h"
#include "TrackInfo.h"
#include "Dictionary.h"

enum EN_PROPERTY_TYPE {
	PT_VOID = 0x0000,
	PT_DEC = 0x0001,	// DECIMAL INTEGER
	PT_HEX = 0x0002,	// HEXADECIAML INTEGER
	PT_OCT = 0x0003,	// OCTAL INTEGER
	PT_BIN = 0x0004,	// BINARY INTEGER
	PT_UDEC = 0x0005,	// UNSIGNED DECIMAL INTEGER
	PT_BOOL = 0x0006,	// BOOLEAN
	PT_FLOAT = 0x0007,	// FLOAT
	PT_DOUBLE = 0x0008,	// DOUBLE
	PT_CHAR = 0x0009,	// CHARACTER
	PT_STRING = 0x000A,	// STRING
	PT_COLOR = 0x000B,	// COLORREF
	PT_PERCENT = 0x000C
};


class CTrackProperty
{
public:
	CTrackProperty(void) {}
	virtual ~CTrackProperty(void) {}

public:
	CString				m_strType;
	CString				m_strName;
	CString				m_strAddr;
	CString				m_strParent;
	int					m_nInOrder;				// Type이 워드가 아닐때 - 워드내의 순서  (D10n.? => ?에 해당 되는 부분)
	BOOL				m_bSeparatelyETC;
	BOOL				m_bKeywordETC;			// ETC 신호인지를 체크하는 변수 
	int					m_nTrackNo;
	CArray<BOOL, BOOL>	m_bEtcInfo;				// 0:Emergency, 1:Auto, 2:OverTime, 3:OverLoad, 4:ProductSensor 

public:
	void SetProperty(CString strType, CString strName, CString strAddr, int nTrackNo = 0, int nInOrder = 1);
	void InitProperty();
};

class CTrackPropertyArray : public CArray<CTrackProperty*, CTrackProperty*>
{
public:
	CTrackPropertyArray() {}
	virtual ~CTrackPropertyArray() {}

	CTrackProperty* GetTrackProperty(CString strKeyword, int nTrackNo = 0, BOOL bParent = FALSE);

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CFlantProperty
{
public:
	CFlantProperty(void) {}
	virtual ~CFlantProperty(void) {}

public:
	CString				m_strType;
	CString				m_strName;
	CString				m_strAddr;
	CString				m_strParent;
	int					m_nInOrder;				// Type이 워드가 아닐때 - 워드내의 순서  (D10n.? => ?에 해당 되는 부분)
	BOOL				m_bSeparatelyETC;
	BOOL				m_bKeywordETC;			// ETC 신호인지를 체크하는 변수 
	int					m_nTrackNo;
	CArray<BOOL, BOOL>	m_bEtcInfo;				// 0:Emergency, 1:Auto, 2:OverTime, 3:OverLoad, 4:ProductSensor 

public:
	void SetProperty(CString strType, CString strName, CString strAddr, int nTrackNo = 0, int nInOrder = 1);
	void InitProperty();
};

class CFlantPropertyArray : public CArray<CFlantProperty*, CFlantProperty*>
{
public:
	CFlantPropertyArray() {}
	virtual ~CFlantPropertyArray() {}

	CFlantProperty* GetFlantProperty(CString strKeyword, int nTrackNo = 0, BOOL bParent = FALSE);

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CDestination
{
public:
	CDestination(void) {}
	virtual ~CDestination(void) {}

public:
	int					m_nPos;
	int					m_nTrackNo;
	BOOL				m_bIsScDest;
};

class CDestinationArray : public CArray<CDestination, CDestination>
{
public:
	CDestinationArray() { m_bUse = FALSE; }
	virtual ~CDestinationArray() {}

public:
	CString m_strDesc;
	BOOL	m_bUse;
	CString m_strKeyWord;

public:
//	CDestination* GetDestinationByPos(int nPos);
//	CDestination* GetDestinationByTrack(int nTrack);
//	CDestination* GetDestinationByDest(int nDest);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*/

class CEcsDoc;
class CEquipment;
class CDeviceMap : public CXmlDom  
{
public:
	CDeviceMap(CEcsDoc* pDoc);
	virtual ~CDeviceMap();

protected:
	CEcsDoc*			m_pDoc;

public:
	EN_COMM_METHOD		m_enCommMethod;
	EN_CPU_KIND			m_enCpuKind;
	EN_ADDRESS_AREA		m_enAddressArea;
	BOOL				m_bUseDestTable;
	BOOL				m_bUseScDest;
	BOOL				m_bUseSeparatelyETC;
	CString				m_strLog;
//	CDestinationArray	m_oDestTable;
	int					m_nPlc;
//	int					m_nId;
	enum { enDefaultPropSize = 4 };	// type, addr, name, InOrder  <= 추후 1개가 Append 됨

public:
//	BOOL GetUseByTrack(int nTrack);
	BOOL ParseXml(CString strFilePath, int nId);

};

#endif // !defined(AFX_DEVICEMAP_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_)
