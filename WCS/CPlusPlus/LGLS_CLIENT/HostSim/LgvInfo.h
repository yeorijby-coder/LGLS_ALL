// LgvInfo.h: interface for the CLgvInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LGVINFO_H__5CAF271D_5E76_4003_8659_95532C33C4F0__INCLUDED_)
#define AFX_LGVINFO_H__5CAF271D_5E76_4003_8659_95532C33C4F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"
#include "LgvVehicle.h"
#include "DciTrackCtrl.h"
#include "TrackHS.h"
#include "CvInfo.h"

class CMonitorServer;
class CLgvInfo : public CInfo  
{
	DECLARE_SERIAL(CLgvInfo)

public:
	CLgvInfo() {}
	CLgvInfo(CEquipment* pEquipment);
	virtual ~CLgvInfo();

public:
	CDciTrackCtrl* m_pControl;
	CTrackHSArray m_pTrackHsArray;
	CCvInfo* m_pCvInfo;

public:
	CTrackHSArray m_pDepartsHS;
	CTrackHSArray m_pArrivesHS;

public:
	CLgvVehicles m_oLgvVehicles;
	WORD m_wErrorCode;
	BOOL m_bSuspend;
	BOOL m_bStop;
	BOOL m_bStopRequest;
	BOOL m_bInvoke;
	int m_nRequestLuggNum;

public:
	enum EN_TRACK_HS_KIND {
		enBidpEmptyArriveST	= 0x00000001,
		enBidpExistArriveST	= 0x00000002,
		enBidpEmptyRetST	= 0x00000004,
		enBidpExistRetST	= 0x00000008,
		enBidpBothRetST		= 0x0000000C,
		enBidpBufferStoST	= 0x00000010,
		enBidpBufferRetST	= 0x00000020,
		enIdpStoST			= 0x00000100,
		enIdpRetST			= 0x00000200,
		enIdpDepartST		= 0x00000400,	
		enIdpArriveST		= 0x00000800
	};

public:
	virtual void Initialize();
	virtual void InvokeControl(int nCarNum = 0);
	virtual void Serialize(CArchive& ar);

public:
	int ConvertLgvPosToTrackNum(int nLgvPos);
	int ConvertTrackNumToLgvPos(int nTrackNum);
	COLORREF GetColor(int nCarNum = 0);

public:
	void MulticastVehicleAll(CMonitorServer* pMonitorSv = NULL);
	void MulticastVehicle(CLgvVehicle* pLgvVehicle);
	void MulticastCommStatus(CMonitorServer* pMonitorSv = NULL);

public:
	CLgvVehicle* GetLgvVehicle(int nVehicleID) { return m_oLgvVehicles.Find(nVehicleID); }
	BOOL IsAllRgvStop() { return m_oLgvVehicles.IsAllRgvStop(); }
	BOOL IsAllRgvRestart() { return m_oLgvVehicles.IsAllRgvRestart(); }
	BOOL IsErrorStatus() { return m_oLgvVehicles.IsErrorStatus(); }

public:
	void SetLampInfo(BOOL bOn);
};

#endif // !defined(AFX_LGVINFO_H__5CAF271D_5E76_4003_8659_95532C33C4F0__INCLUDED_)
