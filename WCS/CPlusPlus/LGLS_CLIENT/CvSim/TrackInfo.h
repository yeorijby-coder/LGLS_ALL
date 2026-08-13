// TrackInfo.h: interface for the CTrackInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKINFO_H__300832FF_C6BD_49CE_B6E8_3A8EF6109305__INCLUDED_)
#define AFX_TRACKINFO_H__300832FF_C6BD_49CE_B6E8_3A8EF6109305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>
//#include <afxtempl.h>

#include "DciTrackCtrl.h"
//#include "DciOpboxCtrl.h"

class CEquipment;
class CStationInfo;
class CTrackHS;
class CJobItem;
class CTrackInfo : public CObject  
{
public:
	CTrackInfo(CEquipment* pEquipment);
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
		enStatusLgvArriveHSReady	= 0x0090,		// 8  0x0100 사용 안함 
		enStatusFullReady			= 0x0100,		// 8
		enStatusLfDepartHSReady		= 0x0200,		// 9
		enStatusLfArriveHSReady		= 0x0400,		// 10
		enStatusLgvInRequest		= 0x0800,		// 11
		enStatusLgvOutRequest		= 0x1000,		// 12
		enStatusCvCheckReady		= 0x2000,		// 13
		enStatusMDInitReady			= 0x8000		// 15
	};


	enum EN_SENSOR {
		enSensorProduct1			= 0x0001,
		enSensorProduct2			= 0x0002,
		enSensorLowVelocity			= 0x0004,
		enSensorFrontOver			= 0x0008,
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
	CEquipment*		m_pEquipment;
	int				m_nNumber;				// TRACK NUMBER (트랙식별자이므로 중복불가)
	int				m_nDestination;			// 사용자 정의 목적지 (해당 값이 0이아닌경우 사용)
	bool			m_bReportTrack;			// 상태보고 트랙 (입고대, 출고대)
	CMapWordToOb	m_mapStatusInfos;		// 해당 트랙상태에 대한 객체 포인터
	CWordArray		m_arrayStatus;			// 위 맵데이터의 순서 정의 (컨트롤에 상태표시 위치 정의)
	CDciTrackCtrl*	m_pTrackCtrl;			// 트랙컨트롤 객체 포인터
	CDciTrackCtrl*	m_pTrackCtrl2;
	CDciTrackCtrl*	m_pTrackCtrl3;
	CDciTrackCtrl*	m_pTrackCtrl4;
	CDciTrackCtrl*	m_pTrackCtrl5;
	int				m_nCvPlcNum;
	int				m_strPlcno;
	int				m_nTabNum;

	int				m_nKind;
	int				m_nNextPlcNum;
	int				m_nNextPlcNum2;
	int				m_nNextCv;
	BOOL			m_bCenter;
	int				m_nCenterNextPlcNum;
	int				m_nCenterNext;
	int				m_nCenterDest;
	BOOL			m_bCross;
	int				m_nCrossTrack;
	int				m_nCrossDest;
	int				m_nCrossPlcNum;
	int				m_nGoTrack;
	BOOL			m_bTwinCheck;
	int				m_nHSTrack;

	int				m_nMGTrack;
	int				m_nMGPlcNum;
	int				m_nDispenser;
	int				m_nDPPlcNum;
	int				m_nTrayPicker;

	CWordArray		m_nStationArray;
	CWordArray		m_nNextTrArray;
	CWordArray		m_nNextPlcArray;
	CWordArray      m_nFloorArray;

	CWordArray		m_nDestPosArray; //20.03.20 jhs추가

	BOOL			m_bStartFlag;
	COleDateTime	m_tTime;
	int				m_nDestCode;		// 목적지 코드

public:
	int		m_nLuggNum;						// D10n + 0
	int		m_nJobType;						// D10n + 2 (LWORD)
	int		m_nDestPos;						// D10n + 1
	BYTE	m_ucGenCode;					// D10n + 2 (HWORD)
	WORD	m_wReserved3;					// D10n + 3
	WORD	m_wErrorCode;					// D10n + 6
	WORD	m_wStatusData;					// D10n + 7
	WORD	m_wSensorData;					// D10n + 8
	WORD	m_wMotorData;					// D10n + 9
	CString m_strBarcode;
	int		m_nTurn;						//Turn Flag
public:
	BOOL	m_bModified;
	BOOL	m_bStoRequest;
	BOOL	m_bRetRequest;
	BOOL	m_bSuspend;		
	BOOL	m_bDeadLock;		
	CString m_strDisplay;

public:
	COleDateTime m_tOperStart;
	COleDateTime m_tOperEnd;
	COleDateTime m_tErrorStart;
	COleDateTime m_tErrorEnd;
	COleDateTime m_tStatusReport;

public:
	CArray <BOOL, BOOL> m_sSeparatelyETC;	// 서로 내부의 갯수가 같아야함! - m_strEtcKeyWords
//	CStringArray m_strEtcKeyWords;			// 서로 내부의 갯수가 같아야함! - m_sSeparatelyETC => DOC 밑으로 옮김 
	int m_nEtcCount;

public:
	BOOL CommandTrack(int nLuggNum, int nJobType, int nDestPos, BYTE ucGenCode);
	BOOL CommandTrack(CJobItem* pJobItem, int nDestTrackNum = 0);
	BOOL CommandDeadLock(BOOL bDeadLock = FALSE);
	BOOL CommandDeadLockCheck(BOOL bDeadLock = FALSE);

public:
	void Initialize();
//	void InvokeControl(int i);
	void InvokeControl(CDciTrackCtrl*	pTrackCtrl = NULL);
	BOOL LoadXML();
	BOOL SaveXML();
	void LoadStation();
	void SetStation(int nStation, int nNextPlc, int nNextTrack, int nDestPos);

public:
	void StatusReport(BOOL bManual = FALSE);
	void ErrorReport(BOOL bManual = FALSE);
	void ReleaseReport(BOOL bManual = FALSE);
//	void TroubleReport(BOOL bManual = FALSE);
//	void LeadTimeReport(BOOL bManual = FALSE);
	int GetWarehouseNo();

public:
	static BOOL IsAlarmCode(WORD wErrorCode);
	BOOL IsAlarmCode() { return IsAlarmCode(m_wErrorCode); }
	static BOOL IsErrorCode(WORD wErrorCode);
	BOOL IsErrorCode() { return IsErrorCode(m_wErrorCode); }

public:
	void SetLuggNum(int nLuggNum);
	void SetJobType(int nJobType);
	void SetStartPos(int nStartPos);
	void SetDestPos(int nDestPos);
	void SetGenCode(BYTE ucGenCode);
	void SetSize(int nSize);
	void SetSuspend(BOOL bSuspend);
	void SetDeadLock(BOOL bDeadLock);
	void SetErrorCode(WORD wErrorCode);
	void SetStatusData(WORD wStatusData);
	void SetSensorData(WORD wSensorData);
	void SetMotorData(WORD wMotorData);
	void SetTurn(int nTurn);

public:
	CString GetTrackString();
	CString GetJobTypeString();
	CString GetGenString();
	CString GetErrorString();
	CString GetXmlString();
	COLORREF GetCvColor();

public:
	WORD GetLgvTaskPriority() { return m_wReserved3; }
	BOOL GetStatusData(WORD wStatus) { return (m_wStatusData & wStatus) ? TRUE : FALSE; }
	BOOL GetStatusData(EN_STATUS enStatus) { return (m_wStatusData & enStatus) ? TRUE : FALSE; }
	BOOL GetSensorData(WORD wSensor) { return (m_wSensorData & wSensor) ? TRUE : FALSE; }
	BOOL GetSensorData(EN_SENSOR enSensor) { return (m_wSensorData & enSensor) ? TRUE : FALSE; }
	BOOL GetMotorData(WORD wMotor) { return (m_wMotorData & wMotor) ? TRUE : FALSE; }
	BOOL GetMotorData(EN_MOTOR enMotor) { return (m_wMotorData & enMotor) ? TRUE : FALSE; }
	BOOL GetLGVArriveData(WORD wMotor) { return (m_wMotorData & wMotor) ? FALSE : TRUE; }
//	int	GetJobPattern() { return ConvertJobTypeToPattern(m_nJobType); }

public:
	BOOL IsAutoMode() { return GetStatusData(enStatusMode); }
	BOOL IsProductSensing() { return GetSensorData(enSensorProduct1); }
	BOOL IsSensorProductSensing() { return GetSensorData(enSensorProduct2); }
	BOOL IsSensorScInterlock() { return GetSensorData(enSensorScInterlock); }
	BOOL IsSensorRtvInterlock() { return GetSensorData(enSensorRtvInterlock); }
	BOOL IsSensorLgvInterlock() { return GetSensorData(enSensorLgvInterlock); }
	BOOL IsSensorLfInterlock() { return GetSensorData(enSensorLfInterlock); }
	BOOL IsSensorStoreMode() { return GetSensorData(enSensorStoreMode); }
	BOOL IsEmergency() { return (m_wErrorCode == 1) || (m_wErrorCode == 3); }
	BOOL IsStoStationReady() { return GetStatusData(enStatusStoSTReady); }
	BOOL IsRetStationReady() { return GetStatusData(enStatusRetSTReady); }
	BOOL IsScStoHsReady() { return GetStatusData(enStatusScStoHSReady); }
	BOOL IsScRetHsReady() { return GetStatusData(enStatusScRetHSReady); }
	BOOL IsRtvDepartHsReady() { return GetStatusData(enStatusRtvDepartHSReady); }
	BOOL IsRtvArriveHsReady() { return GetStatusData(enStatusRtvArriveHSReady); }
	BOOL IsLgvDepartHsReady() { return GetStatusData(enStatusLgvDepartHSReady); }
	BOOL IsLgvArriveHsReady() { return GetStatusData(enStatusLgvArriveHSReady); }
	BOOL IsLfDepartHsReady() { return GetStatusData(enStatusLfDepartHSReady); }
	BOOL IsLfArriveHsReady() { return GetStatusData(enStatusLfArriveHSReady); }
	BOOL IsLgvInRequest() { return GetStatusData(enStatusLgvInRequest); }
	BOOL IsLgvOutRequest() { return GetStatusData(enStatusLgvOutRequest); }
	BOOL IsCvCheckPosReady() { return GetStatusData(enStatusCvCheckReady); }
	BOOL IsMDInitReady() { return GetStatusData(enStatusMDInitReady); }
	BOOL IsNotInvokeTrack() { return (m_mapStatusInfos.GetCount() == 0); }
	BOOL IsLgvMode() { return (GetSensorData(0x4000) == 0); }
	//RTV 
 	BOOL GetRtvArriveHsChk() { return GetMapCVValue(enStatusRtvArriveHSReady); }
 	BOOL GetRtvDepartHSChk() { return GetMapCVValue(enStatusRtvDepartHSReady); }
 	
// 	//SC
 	BOOL GetScRetHSChk() { return GetMapCVValue(enStatusScRetHSReady);}
 	BOOL GetScStoHSChk() { return GetMapCVValue(enStatusScStoHSReady);}
	
public:
	WORD GetMapStatusKey(int nIndex);
	CObject* GetMapStatusValue(WORD wStatus);
	
	BOOL GetMapCVValue(WORD wStatus);

	CStationInfo* GetStoHSStation() { return (CStationInfo*)GetMapStatusValue(enStatusScStoHSReady); }
	CStationInfo* GetRetHSStation() { return (CStationInfo*)GetMapStatusValue(enStatusScRetHSReady); }
	CStationInfo* GetStoStation() { return (CStationInfo*)GetMapStatusValue(enStatusStoSTReady); }
	CStationInfo* GetRetStation() { return (CStationInfo*)GetMapStatusValue(enStatusRetSTReady); }
	CStationInfo* GetArvStation() { return (CStationInfo*)GetMapStatusValue(enStatusArvSTReady); }
	CStationInfo* GetScRetST() { return (CStationInfo*)GetMapStatusValue(enStatusScRetHSReady); }
	CStationInfo* GetFullStation() { return (CStationInfo*)GetMapStatusValue(enStatusFullReady); }
	CStationInfo* GetLgvDepartStation() { return (CStationInfo*)GetMapStatusValue(enStatusLgvDepartHSReady); }
	CStationInfo* GetLgvArriveStation() { return (CStationInfo*)GetMapStatusValue(enStatusLgvArriveHSReady); }
	CTrackHS* GetScStoHS() { return (CTrackHS*)GetMapStatusValue(enStatusScStoHSReady); }
	CTrackHS* GetScRetHS() { return (CTrackHS*)GetMapStatusValue(enStatusScRetHSReady); }
	CTrackHS* GetRtvDepartHS() { return (CTrackHS*)GetMapStatusValue(enStatusRtvDepartHSReady); }
	CTrackHS* GetRtvArriveHS() { return (CTrackHS*)GetMapStatusValue(enStatusRtvArriveHSReady); }
	CTrackHS* GetLifterDepartHS() { return (CTrackHS*)GetMapStatusValue(enStatusLfDepartHSReady); }
	CTrackHS* GetLifterArriveHS() { return (CTrackHS*)GetMapStatusValue(enStatusLfArriveHSReady); }
	CTrackHS* GetLgvDepartHS() { return (CTrackHS*)GetMapStatusValue(enStatusLgvDepartHSReady); }
	CTrackHS* GetLgvArriveHS() { return (CTrackHS*)GetMapStatusValue(enStatusLgvArriveHSReady); }

public:
	void SetStatusDataBit(EN_STATUS enStatus, BOOL bValue);
	void SetScRetHsReady(BOOL bValue) { SetStatusDataBit(enStatusScRetHSReady, bValue); }
	void SetRtvArriveHsReady(BOOL bValue) { SetStatusDataBit(enStatusRtvArriveHSReady, bValue); }
	void SetLifterArriveHsReady(BOOL bValue) { SetStatusDataBit(enStatusLfArriveHSReady, bValue); }
	void SetLgvArriveHsReady(BOOL bValue) { SetStatusDataBit(enStatusLgvArriveHSReady, bValue); }
};

class CTrackInfoArray : public CArray<CTrackInfo*, CTrackInfo*>
{
public:
	CTrackInfoArray() {}
	virtual ~CTrackInfoArray() {}

public:
	CString GetXmlString();
	CTrackInfo* GetTrackInfo(int nNumber, int nCvPlcNum);
	BOOL IsIdleJob(int nLuggNum);
	void InvokeControl(BOOL bManual);
	void StatusReport();
	void Initialize();
};

#endif // !defined(AFX_TRACKINFO_H__300832FF_C6BD_49CE_B6E8_3A8EF6109305__INCLUDED_)

