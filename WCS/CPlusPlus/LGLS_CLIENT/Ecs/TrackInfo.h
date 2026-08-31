// TrackInfo.h: interface for the CTrackInfo class.
//

#pragma once

#include <afxcoll.h>
#include <afxtempl.h>
#include "DciTrackCtrl.h"
#include "CV_DATA.h"

class CEquipment;



// CTrackInfo
//
class CTrackInfo : public CObject  
{
public:
	DWORD m_dwJobVerSeen = 0;	// [LGLS 2026-08-31] 마지막으로 반영한 작업정보 캐시 버전

	CTrackInfo(CEquipment* pEquipment);

	// [LGLS 2026-07-19] 입고대 ON 지연(데이터 없는 화물 감지 2초) 판정용
	DWORD m_dwStoOnTick;
	BOOL m_bLastStoOn;   // [LGLS] 입고대 지연 ON 직전 상태(전이 감지용)
	BOOL IsStoReadyOn();
	BOOL ApplyRetCntDisplay(CDciTrackCtrl* pTrackCtrl);
	void ApplyTrackTextMode(CDciTrackCtrl* pTrackCtrl);   // [LGLS] 작업번호/제품정보 표시 모드
	CDWordArray m_arrOrigItemColors;   // [LGLS] 작업대 화살표 원색(일시정지 해제 복원용)
	CString m_strProdLugg;
	CString m_strProdVal;   // [LGLS] SC 출고 잔여 수 표시
	virtual ~CTrackInfo();

public:
	enum EN_ERROR {
		enErrorNone					= 0x0000,
		enErrorCommunication		= 0xFFFF
	};
	enum EN_STATUS {
		enStatusMode				= 0x0001,		// 0
		enStatusStoSTReady			= 0x0002,		// 1
		enStatusRetSTReady			= 0x0004,		// 2
		enStatusArvSTReady			= 0x0006,		// ----
		enStatusScStoHSReady		= 0x0008,		// 3
		enStatusScRetHSReady		= 0x0010,		// 4
		enStatusRtvDepartHSReady	= 0x0020,		// 5
		enStatusRtvArriveHSReady	= 0x0040,		// 6
		enStatusLgvDepartHSReady	= 0x0080,		// 7
		enStatusLgvArriveHSReady	= 0x0100,		// 8
		enStatusLfDepartHSReady		= 0x0200,		// 9
		enStatusLfArriveHSReady		= 0x0400,		// 10
		enStatusLgvInRequest		= 0x0800,		// 11
		enStatusBcrReadRequest		= 0x1000,		// 12		//enStatusBcrReadRequest
		enStatusCvCheckReady		= 0x2000,		// 13
		enStatusDispensorLow		= 0x4000,		// 14
		enStatusDispensorHigh		= 0x8000		// 15

	};
	enum EN_SENSOR {
		enSensorProduct1L			= 0x0001,
		enSensorProduct2L			= 0x0002,
		enSensorProduct3L			= 0x0004,
		enSensorTrackDelete			= 0x0008,
		enSensorRearOver			= 0x0010,
		enSensorClear				= 0x0020,
		enSensorHomeStandUp			= 0x0040,
		enSensorHomeStandDown		= 0x0080,
		enSensorDeverterUp			= 0x0100,
		enSensorDeverterDown		= 0x0200,
		enSensorScInterlock			= 0x0400,
		enSensorRtvInterlock		= 0x0800,
		enSensorLgvInterlock		= 0x1000,
		enSensorLfInterlock			= 0x2000,
		enSensorStoreMode			= 0x8000
	};
	enum EN_MOTOR {
		enMotorStop					= 0x0001,
		enMotorForward				= 0x0002,
		enMotorBackward				= 0x0004,
		enMotorSizeCheck1			= 0x0008,
		enMotorSizeCheck2			= 0x0010,
		enMotorSizeCheck3			= 0x0020,
		enMotorSizeCheck4			= 0x0040,
		enMotorStopperOn1			= 0x0080,
		enMotorStopperOff1			= 0x0100,
		enMotorStopperOn2			= 0x0200,
		enMotorStopperOff2			= 0x0400,
		enMotorStopperOn3			= 0x0800,
		enMotorStopperOff3			= 0x1000
	};

public:
	CCV_DATA* m_pCV_DATA;
	CString m_strPLC_NO;

	CEquipment*		m_pEquipment;
	int				m_nNumber;		
	CString			m_pPlcno;
	CDciTrackCtrl*	m_pTrackCtrl;		
	CMapWordToOb	m_mapStatusInfos;	
	CWordArray		m_arrayStatus;		
	BOOL	m_bDlgAutoRefresh;				

public:
	CCV_DATA* CreateCCV_DATA();
	CString GetStringTRACK_NO();
	CString GetStringPLC_NO();

public:
	CString GetCid();


public:
	void InvokeControl(CDciTrackCtrl* pTrackCtrl = NULL);
	void SetSeachColor(CDciTrackCtrl* pTrackCtrl, BOOL pBasic);

public:
	COLORREF GetCvColor();
	// [LGLS 2026-08-30] 에러코드 표기 흔들림(0/00/0000/빈 값) 흡수 - 정상 판정
	static BOOL IsNoError(const CString& strErrCode);
	WORD GetMapStatusKey(int nIndex);
	CObject* GetMapStatusValue(WORD wStatus);

public:
	void SetDataField( CStringArray& strTrackData );
	bool SetCtrl( CDciTrackCtrl* pCtrl);
};


// CTrackInfoArray
//
class CTrackInfoArray : public CArray<CTrackInfo*, CTrackInfo*>
{
public:
	CTrackInfoArray() {}
	virtual ~CTrackInfoArray();

public:
	CTrackInfo* GetTrackInfo(int nNumber);//, int nCvPlcNum = 0);
	void InvokeControl(BOOL bManual);
	void Initialize();
};
