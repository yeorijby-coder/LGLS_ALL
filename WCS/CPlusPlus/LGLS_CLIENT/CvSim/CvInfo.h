// CvInfo.h: interface for the CCvInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CVINFO_H__71CE1B79_BA0A_4867_AC05_3484986AFC24__INCLUDED_)
#define AFX_CVINFO_H__71CE1B79_BA0A_4867_AC05_3484986AFC24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"
#include "TrackInfo.h"
#include "DciStaticCtrl.h"


class CEquipment;
class CCvInfo : public CInfo  
{
	DECLARE_SERIAL(CCvInfo)

public:
	CCvInfo() {}
	CCvInfo(CEquipment* pEquipment);
	virtual ~CCvInfo();

public:
	class CDciStaticCtrlArray : public CArray<CDciStaticCtrl*, CDciStaticCtrl*> {};
	CDciStaticCtrlArray m_pDoorCtrls;
	WORD m_wDoorStatus;		// DOOR 상태 (READ)
	WORD m_wDoorClose;		// DOOR CLOSE 지시 (WRITE)
	WORD m_wDoorOpen;		// DOOR OPEN 지시 (WRITE)
	WORD w_wInPermission;	// 진입허가(READ)		// 추가
	
public:
	CTrackInfoArray m_pTracks;
	BIT_FIELD m_tagBits;

public:
	void SetDoorStatus(WORD wDoorStatus);
	void SetInPermission(WORD wInPermission);
	BOOL IsDoorOpen(int nIndex) { return CLib::IsSet(m_wDoorStatus, nIndex); }
	BOOL IsDoorClose(int nIndex) { return !CLib::IsSet(m_wDoorStatus, nIndex); }
	int  GetDoorCount() { return m_pDoorCtrls.GetSize(); }
	CString GetXmlString();

public:
	virtual void InvokeControl();
	virtual void Serialize(CArchive& ar);
	virtual void Initialize() { m_bModified = TRUE; m_pTracks.Initialize(); }
	virtual void StatusReport(BOOL bManual = FALSE) { m_pTracks.StatusReport(); }
};

#endif // !defined(AFX_CVINFO_H__71CE1B79_BA0A_4867_AC05_3484986AFC24__INCLUDED_)
