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
	CDciStaticCtrlArray m_pLampCtrls;
	WORD m_wDoorStatus;		// DOOR 상태 (READ)
	WORD m_wDoorClose;		// DOOR CLOSE 지시 (WRITE)
	WORD m_wDoorOpenReq;	// DOOR OPEN 요청 (WRITE)
	WORD m_wDoorOpenAccept;	// DOOR OPEN 승인 (WRITE)
	WORD m_wRgvStatus;		// RGV 상태
	WORD m_wPrevLampStatus;		// Lamp 상태 (READ)
	WORD m_wCurrLampStatus;		// Lamp 상태 (READ)

public:
	BOOL m_bFirstDoorCheck;		// 처음 도어의 상태
	WORD m_wRgvcErrorStatus;	// RGVC의 에러 상태

public:
	CTrackInfoArray m_pTracks;

public:
	void SetDoorStatus(WORD wDoorStatus);
	void SetDoorOpenReq(WORD wDoorOpenReq);
	void SetDoorClose(WORD wDoorClose);
	void SetDoorOpenAccept(WORD wDoorOpenAccept);
	void SetRgvStatus(WORD m_wRgvStatus);
	void SetLampInfoDoor(BOOL bOn);
	void SetLampInfo(BOOL bOn);

	BOOL IsDoorOpen(int nIndex) { return CLib::IsSet(m_wDoorStatus, nIndex); }
	BOOL IsDoorClose(int nIndex) { return !CLib::IsSet(m_wDoorStatus, nIndex); }
	BOOL IsDoorOpenReq(int nIndex) { return CLib::IsSet(m_wDoorOpenReq, nIndex); }
	BOOL IsDoorOpenAccept(int nIndex) { return CLib::IsSet(m_wDoorOpenAccept, nIndex); }
	BOOL IsDoorCloseResponse(int nIndex) { return CLib::IsSet(m_wDoorClose, nIndex); }
	BOOL IsRgvStatusRun(int nIndex) { return CLib::IsSet(m_wRgvStatus, nIndex); }
	BOOL IsRgvStatusStop(int nIndex) { return !CLib::IsSet(m_wRgvStatus, nIndex); }
	int  GetDoorCount() { return m_pDoorCtrls.GetSize(); }
	int  GetLampCount() { return m_pLampCtrls.GetSize(); }
//	BOOL IsLampOn(int nIndex) { return CLib::IsSet(m_wDoorStatus, nIndex); }
	CString GetXmlString();

public:
	virtual void InvokeControl();
	virtual void Serialize(CArchive& ar);
	virtual void Initialize() { m_bModified = TRUE; m_pTracks.Initialize(); }
	virtual void StatusReport(BOOL bManual = FALSE) { m_pTracks.StatusReport(); }
};

#endif // !defined(AFX_CVINFO_H__71CE1B79_BA0A_4867_AC05_3484986AFC24__INCLUDED_)
