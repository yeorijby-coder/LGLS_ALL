// ScInfo.h: interface for the CScInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCINFO_H__CC7DC245_F32B_4FC3_BDD5_2E6836872D0C__INCLUDED_)
#define AFX_SCINFO_H__CC7DC245_F32B_4FC3_BDD5_2E6836872D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"
#include "TrackHS.h"
#include "DciRvCtrl.h"
#include "ByteArrayEx.h"

#define WORD_SIZE_OF_ECS2PLC	3
#define WORD_SIZE_OF_PLC2ECS	4

typedef struct ScJobInfo
{
	int  m_nLuggNum;			// 작업번호
	int	 m_nInternalLuggNum;
//	BYTE m_ucCmdType;			// 작업구분 {없음=0, 입고=1, 출고=2, 직출고=3, 재배치=4, 홈복귀=5, 이동=6, 이중입고재지정=7}
	int  m_nInternalJobType;
	BOOL m_bInvoke;				// SC 작업지시 검사플래그
	CString m_strDepartInfo;	// 출발지(4x4) {열, 행, 단, HS}
	CString m_strArriveInfo;	// 도착지(4x4) {열, 행, 단, HS}
	int	 m_nDepartTrackNum;
	int	 m_nArriveTrackNum;
	int  m_nFromHsNum;
	int  m_nToHsNum;

} ScJobInfo;


class CEquipment;
class CStationInfo;
//class CByteArrayEx;
class CScInfo : public CInfo  
{
	DECLARE_SERIAL(CScInfo)
		
public:
	CScInfo() {}
	CScInfo(CEquipment* pEquipment);
	virtual ~CScInfo();

public:
	enum EN_TYPE { enL2R, enR2L, enT2B, enB2T, enTypeSize };
	enum EN_FORK_TYPE { enSingle, enTwin, enDouble, enMulti, enForkTypeSize };
	enum EN_CYCLE_TYPE { enFullCycle, enHalfCycle, enCycleSize };

	enum EN_CYCLE { enHome=1, enPickup=2, enUnload=3, enBeltMove=5 };
	enum EN_RC_MODE { enOff=0, enOnline=1, enRemote=2 };																			// D 95
	enum EN_SC_MODE { enAuto=1, enManual=0 };																						// D100-L
	enum EN_PROD_LOAD { enProdEmpty=0, enProdExistFork1=1, enProdExistFork2=2, enProdExistFork12=3 };								// D100-H
	enum EN_SC_STATUS { enIdle=0, enWaiting=1, enRunning=2, enError=4  };															// D101
	enum EN_FORK_POS { enForkPosCenter= 0 , enForkPosBank1=1, enForkPosBank2=2 };													// D104
	enum EN_FORK_STATUS { enForkReady=0, enForkDualStoreError=1, enForkEmptyRetrieveError=2 };										// D106
	enum EN_RC_STATUS { enStop=0, enActive=1 };																						// D109
	enum EN_JOB_STATUS { enMoving='0', en1ForkComplete='1', en2ForkComplete='2', enComplete='3'  };									// D110
	enum EN_CMD_TYPE { enCmdNone=0, enStore=1, enRetrieve=2, enHs2Hs=3, enRack2Rack=4, enHomeReturn=5, enMove=6, enDualStore=7 };	// D111
	enum EN_ROUTINE { enRoutineStore, enRoutineRetrieve, enRoutineRackToRack };
	enum EN_FLOOR_POS { enFloor0, enFloor1, enFloor2, enFloor3, enFloorSize };			// enFloor0 : 지하  


	enum EN_STATUS {
		enScJobStatusIdle			= 0x0001,		// D100.0,		D120.0,		D140.0, 
		enScJobStatusError			= 0x0002,		// D100.1,		D120.1,		D140.1, 
		enScJobStatusRegPos			= 0x0004,		// D100.2,		D120.2,		D140.2, 
		enScJobStatusHoistUp		= 0x0008,		// D100.3,		D120.3,		D140.3, 
		enScJobStatusHoistDown		= 0x0010,		// D100.4,		D120.4,		D140.4, 
		enScJobStatusForkCenter		= 0x0020,		// D100.5,		D120.5,		D140.5, 
		enScJobStatusForkRight		= 0x0040,		// D100.6,		D120.6,		D140.6, 
		enScJobStatusForkLeft		= 0x0080,		// D100.7,		D120.7,		D140.7, 
		enScJobStatusSensorProduct	= 0x0100,		// D100.8,		D120.8,		D140.8, 
		enScJobStatusScManualMode	= 0x0200,		// D100.9,		D120.9,		D140.9, 
		enScJobStatusScAutoMode		= 0x0400,		// D100.A,		D120.A,		D140.A, 
		enScJobStatusScRemoteMode	= 0x0800,		// D100.B,		D120.B,		D140.B, 
		enScJobStatusRcManualMode	= 0x1000,		// D100.C,		D120.C,		D140.C, 
		enScJobStatusRcAutoMode		= 0x2000,		// D100.D,		D120.D,		D140.D, 
		enScJobStatusRcRemoteMode	= 0x4000,		// D100.E,		D120.E,		D140.E, 
		enScJobStatusJobComplete	= 0x8000		// D100.F,		D120.F,		D140.F, 
	};
	enum EN_DOOR_STATUS {
		enScDoorStatusOpen			= 0x0000		// D111.0,		D131.0,		D151.0, 
	};

public:
	CDciRvCtrl*		m_pControl;
	CStationInfo*	m_pStation;
	CTrackHSArray	m_pStosHS;
	CTrackHSArray	m_pRetsHS;
	int				m_nEquipNum;

public:
//	BYTE m_ucRcStatus;			// 지상반 동작상태	{ enStop='0', enActive='1' }
//	BYTE m_ucRcMode;			// 지상반 동작모드	{ enOnline='2', enRemote='1' }
//	BYTE m_ucScMode;			// 기상반 동작모드	{ enAuto='1', enManual='0' }
//	BYTE m_ucScStatus;			// 기상반 동작상태	{ enIdle=0, enWaiting='1', enRunning='2', enError='4' }
//	BYTE m_ucJobStatus;			// 작업상태			{ enMoving='0', enComplete='1' }
	WORD m_wHorizontalPos;		// 수평주행위치
	WORD m_wVerticalPos;		// 수직주행위치
	BYTE m_ucProdLoad;			// 화물유무			{ enProdEmpty='0', enProdExist='1' }
	BYTE m_ucForkPos;			// 포크위치			{ enForkPosCenter='0', enForkPosBank1='1', enForkPosBank2='2' }
//	BYTE m_ucForkStatus;		// 포크상태			{ enForkReady=0, enForkDualStoreError=1, enForkEmptyRetrieveError=2 }
//	BYTE m_ucForkStatus2;		// 포크상태			{ enForkReady=0, enForkDualStoreError=1, enForkEmptyRetrieveError=2 }
	int	 m_nErrorCode;			// 에러코드
	BYTE m_ucCmdType;			// 작업구분			{없음=0, 입고=1, 출고=2, 직출고=3, 재배치=4, 홈복귀=5, 이동=6, 이중입고재지정=7}
//	BYTE m_nCycleStep;			// CycleStep		{홈복귀=1, PickUp=2, Unload=3, Move=4(미사용), BeltMove=5, 학습=7}
	CString m_strDepartInfo;	// 출발지(4x4)		{열, 행, 단, HS}
	CString m_strArriveInfo;	// 도착지(4x4)		{열, 행, 단, HS}
//	BOOL m_bCycleInvoke;		// CycleInvoke

	WORD m_wScJobStatus;		// SCJobStatus		D100, D120, D140
	WORD m_wScDoorStatus;		// SCDoorStatus		D111, D131, D151

public:
	int  m_nPrevErrorCode;		// 직전 에러코드
	int	 m_nPrevRoutine;
	BOOL m_bStoSuspend;
	BOOL m_bRetSuspend;
	BOOL m_bR2RSuspend;
	BYTE m_ucPrevStatus;

public:
	WORD	m_wPrevErrCode;
	WORD	m_wErrCode;
	BOOL	m_bBufferEmpty;
	int  m_nForkType;

public:
	BOOL	m_bFirstDoorCheck;

public:
	ScJobInfo	m_ScInfo[2];

public:
	CTime m_tOperStart;
	CTime m_tOperEnd;
	CTime m_tErrorStart;
	CTime m_tErrorEnd;
	CTime m_tStatusReport;

public:
	virtual void Initialize();
	virtual void InvokeControl();
	virtual void Serialize(CArchive& ar);
	virtual void StatusReport(BOOL bManual = FALSE);
	virtual void ErrorReport(BOOL bManual = FALSE );

public:
//	void TroubleReport(BOOL bManual = FALSE);
//	void LeadTimeReport(BOOL bManual = FALSE);
	int GetWarehouseNo();

public:
	static BOOL IsAlarmCode(int nErrorCode);
	BOOL IsAlarmCode() { return IsAlarmCode(m_nErrorCode); }
	static BOOL IsErrorCode(int nErrorCode);
	BOOL IsErrorCode() { return IsErrorCode(m_nErrorCode); }

public:
	void SetScJobStatus(WORD wScJobStatus);
	void SetScDoorStatus(WORD wScDoorStatus);
//	void SetRcStatus(BYTE ucRcStatus);
//	void SetRcMode(BYTE ucRcMode);
	void SetJobCount(BYTE ucJobCount);
//	void SetScMode(BYTE ucScMode);
//	void SetScStatus(BYTE ucScStatus);		//
	void SetHorizontalPos(WORD wHorizontalPos);
	void SetVerticalPos(WORD wVerticalPos);
	void SetProdLoad(BYTE ucProdLoad);
	void SetForkPos(BYTE ucForkPos);
//	void SetForkStatus(BYTE ucForkStatus);
//	void SetForkStatus2(BYTE ucForkStatus2);
	void SetErrorCode(WORD wErrorCode);
	void SetCompleteNum(int nCompleteNum);
	void SetLuggNum(WORD nLuggNum, int nFork);
	void SetCmdType(BYTE ucCmdType);
	void SetDepartInfo(CString& strDepartInfo, int nFork);
	void SetArriveInfo(CString& strArriveInfo, int nFork);
	void SetJobStatus(BYTE ucJobStatus);

	void SetLampInfo(BOOL bOn);

//	void SetActiveMode(BOOL bActive);

public:
//	CString GetRcStatus();
//	CString GetRcMode();
//	CString GetScMode();
//	CString GetScStatus();
//	CString GetForkPos();
//	CString GetForkStatus();
//	CString GetForkStatus2();
//	CString GetProdLoad();
	CString GetCmdType();
//	CString GetJobStatus();
//	CString GetPrevStatus();
	CString GetDepartInfoString();
	CString GetArriveInfoString();
	CString GetDepartInfoString2();
	CString GetArriveInfoString2();
	CString GetErrorString();
	CString GetXmlString();
	
public:
	BOOL GetScJobStatusData(WORD wStatus) { return (m_wScJobStatus & wStatus) ? TRUE : FALSE; }
	BOOL GetScJobStatusData(EN_STATUS enStatus) { return (m_wScJobStatus & enStatus) ? TRUE : FALSE; }
	BOOL GetScDoorStatusData(WORD wStatus) { return (m_wScDoorStatus & wStatus) ? TRUE : FALSE; }
	BOOL GetScDoorStatusData(EN_DOOR_STATUS enStatus) { return (m_wScDoorStatus & enStatus) ? TRUE : FALSE; }

public:
	BOOL IsScJobStatusIdle()			{ return GetScJobStatusData(enScJobStatusIdle); }
	BOOL IsScJobStatusError()			{ return GetScJobStatusData(enScJobStatusError); }
	BOOL IsScJobStatusRegPos()			{ return GetScJobStatusData(enScJobStatusRegPos); }
	BOOL IsScJobStatusHoistUp()			{ return GetScJobStatusData(enScJobStatusHoistUp); }
	BOOL IsScJobStatusHoistDown()		{ return GetScJobStatusData(enScJobStatusHoistDown); }
	BOOL IsScJobStatusForkCenter()		{ return GetScJobStatusData(enScJobStatusForkCenter); }
	BOOL IsScJobStatusForkRight()		{ return GetScJobStatusData(enScJobStatusForkRight); }
	BOOL IsScJobStatusForkLeft()		{ return GetScJobStatusData(enScJobStatusForkLeft); }
	BOOL IsScJobStatusSensorProduct()	{ return GetScJobStatusData(enScJobStatusSensorProduct); }
	BOOL IsScJobStatusScManualMode()	{ return GetScJobStatusData(enScJobStatusScManualMode); }
	BOOL IsScJobStatusScAutoMode()		{ return GetScJobStatusData(enScJobStatusScAutoMode); }
	BOOL IsScJobStatusScRemoteMode()	{ return GetScJobStatusData(enScJobStatusScRemoteMode); }
	BOOL IsScJobStatusRcManualMode()	{ return GetScJobStatusData(enScJobStatusRcManualMode); }
	BOOL IsScJobStatusRcAutoMode()		{ return GetScJobStatusData(enScJobStatusRcAutoMode); }
	BOOL IsScJobStatusRcRemoteMode()	{ return GetScJobStatusData(enScJobStatusRcRemoteMode); }
	BOOL IsScJobStatusJobComplete()		{ return GetScJobStatusData(enScJobStatusJobComplete); }

	BOOL IsScDoorStatusOpen()			{ return GetScDoorStatusData(enScDoorStatusOpen); }

	void			SetPrevErrorCode(WORD wErrorCode)	{	m_wPrevErrCode = wErrorCode;}

public:
	void			ClearInternalData();
	void			ClearInternalData(int nFork);
	void			PermitLoadUnload(BOOL bLoad, BOOL bOn);
	void			ResetModified();
	void			SendMsgToView(int nStatus);

/*
public:
	BOOL			IsShelfSensor();
	BOOL			IsArrivedPosition(int nPortNum);
	BOOL			IsArrivedPosition(CString strPosition);
	BOOL			IsConnected();
	BOOL			IsBitOn(int nSigName);
	BOOL			IsBitOnPLC(int nSigName);
	BOOL			IsTypeOfTwinFork();
	BOOL			IsTypeOfDoubleFork();
	BOOL			IsLoadedProduct();
	BOOL			IsModified();
	BOOL			IsReadyToWork();
	BOOL			IsFinishTheWork();
	BOOL			IsActive();
	BOOL			IsIdleStatus();
	BOOL			IsEvent()					{ return (m_bEvent == TRUE); }	
//	BOOL			IsMainPowereOn()			{	return m_bMainPowerOn;}
//	BOOL			IsMainPower()				{	return m_bMainPower;}
//	BOOL			IsCycleOK()					{	return m_bCycleOK;}
	BOOL			IsCycleNG()					{	return m_bCycleNG;}
	BOOL			IsCycleComplete()			{	return m_bCycleComplete;}
	BOOL			IsError()					{	return m_bError;}
	BOOL			IsErrorOnFork1()			{	return m_bErrorOnFork1;}
	BOOL			IsErrorOnFork2()			{	return m_bErrorOnFork2;}
	BOOL			IsFork1OnLeft()				{	return m_bFork1OnLeft;}
	BOOL			IsFork1OnRight()			{	return m_bFork1OnRight;}
	BOOL			IsFork2OnRight()			{	return m_bFork2OnRight;}
	BOOL			IsFork2OnLeft()				{	return m_bFork2OnLeft;}
	BOOL			IsOnBusy()					{	return m_bOnBusy;}
	BOOL			IsOnLForkingFork1()			{	return m_bOnLForkingFork1;}
	BOOL			IsOnRForkingFork1()			{	return m_bOnRForkingFork1;}
	BOOL			IsProdLoadOnFork1()			{	return m_ucProdLoad == CScInfo::enProdExistFork1;}
	BOOL			IsProdLoadOnFork2()			{	return m_ucProdLoad == CScInfo::enProdExistFork2;}
	BOOL			IsProdLoadOnFork12()		{	return m_ucProdLoad == CScInfo::enProdExistFork12;}
	BOOL			IsProdOnLeftShelf()			{	return m_bProdOnLeftShelf;}
	BOOL			IsProdOnRightShelf()		{	return m_bProdOnRightShelf;}
	BOOL			IsBufferEmpty()				{	return m_bBufferEmpty;}
	BOOL			IsEmergencyStoped()			{	return m_bEmergencyStoped;}

public:
	void			SetOriginalData(BYTE* pData);
	void			SetBit(int nSigName, BOOL bOn);
	void			SetRetPrevRoutine(int nRetPrevRoutine);
	void			SetStoPrevRoutine(int nStoPrevRoutine);
	void			SetPrevRoutine(int nPrevRoutine);
	void			SetRackToRackSuspend(BOOL bRackToRackSuspend);
	void			SetRetrieveSuspend(BOOL bRetrieveSuspend);
	void			SetStoreSuspend(BOOL bStoreSuspend);
	BOOL			SetModified(BOOL bModified);
	void			SetECS2PLCData				(BYTE* pucData);
	void			SetPLC2ECSData				(BYTE* pucData);
	void			SetOnLineMode(BOOL bOnLineMode);
	void			SetCycleStep				(BYTE ucCycleStep);
	void			SetCycleNG					(BOOL bCycleNG);
	void			SetCycleComplete			(BOOL bCycleComplete);
	void			SetCycleOK					(BOOL bCycleOK);
//	void			SetMainPower				(BOOL bMainPower);
	void			SetErrorOnFork1				(BOOL bErrorOnFork1);
	void			SetErrorOnFork2				(BOOL bErrorOnFork2);
	void			SetError					(BOOL bError);
//	void			SetMainPowerOn				(BOOL bMainPowerOn);
	void			SetCycleBay					(BYTE ucCycleBay);
	void			SetCycleLevel				(BOOL ucCycleLevel);
	void			SetFork1OnRight				(BOOL bFork1OnRight);
	void			SetOnRForkingFork1			(BOOL bOnOnRForkingFork1);
	void			SetOnLForkingFork1			(BOOL bOnLForkingFork1);
	void			SetOnBusy					(BOOL bOnBusy);
	void			SetFork2OnLeft				(BOOL bFork2OnLeft);
	void			SetFork2OnRight				(BOOL bFork2OnRight);
	void			SetFork1OnLeft				(BOOL bFork1OnLeft);
	void			SetProdLoadOnFork			(BYTE ucProdLoad);
	void			SetProdOnRightShelf			(BOOL bProdOnRightShelf);
	void			SetProdOnLeftShelf			(BOOL bProdOnLeftShelf);
	void			SetBufferEmpty				(BOOL bBufferEmpty);
	void			SetAutoMode				(BOOL bAutoMode);
	BOOL			SetLuggNum(int nLuggNum);
	BOOL			SetJobType(BYTE cJobType);
	BOOL			SetLuggNumForkTwo(int nLuggNum);
	void			SetDestTrack(UINT nTrackNum);

public:
	DWORD			GetForkAimError();
	DWORD			GetHorizontalAimError();
	DWORD			GetVerticalAimError();
	BYTE*			GetOriginalData();
	DWORD			GetHorizontalError();
	UINT			GetResourceID();
	int				GetRetPrevRoutine();
	int				GetStoPrevRoutine();
	int				GetPrevRoutine();
	WORD			GetPrevErrorCode()					{	return m_wPrevErrCode; }
	BOOL			GetRackToRackSuspend();
	BOOL			GetRetrieveSuspend();
	BOOL			GetStoreSuspend();
	int				GetWareHouse();
	int				GetID();
	int				GetScStation();
	BYTE*			GetECS2PLCData()			{	return m_ucECS2PLCData;}
	BYTE*			GetPLC2ECSData()			{	return m_ucPLC2ECSData;}
	BYTE			GetCycleStep()				{ return m_ucECS2PLCData[2] & 0x07; }
	BYTE			GetCycleLevel()				;
	BYTE			GetCycleBay()				;
	WORD			GetErrorCode()			{	return m_wErrCode;	};
	BYTE			GetJobType()			{	return m_ScInfo[0].m_nInternalJobType;	};
	int				GetLuggNum()			{	return m_ScInfo[0].m_nInternalLuggNum;	};
	int				GetLuggNumForkTwo()		{	return m_ScInfo[1].m_nInternalLuggNum;	};
	CString			GetActiveString();
	CString			GetHorizontalPosString();
	CString			GetVerticalPosString();
	CString			GetProdLoadString();
	CString			GetCompleteString();
	UINT			GetDestTrack();
*/


public:
	int GetDepartHS(CString strDepartInfo);
	int GetArriveHS(CString strArriveInfo);
	int GetDepartHS1(){ return GetDepartHS(m_ScInfo[0].m_strDepartInfo); };
	int GetArriveHS1(){ return GetArriveHS(m_ScInfo[0].m_strArriveInfo); };
	int GetDepartHS2(){ return GetDepartHS(m_ScInfo[1].m_strDepartInfo); };
	int GetArriveHS2(){ return GetArriveHS(m_ScInfo[1].m_strArriveInfo); };
	CString GetDepartLoc(CString strDepartInfo);
	CString GetArriveLoc(CString strArriveInfo);
	CString GetDepartLoc1(){ return GetDepartLoc(m_ScInfo[0].m_strDepartInfo); };
	CString GetArriveLoc1(){ return GetArriveLoc(m_ScInfo[0].m_strArriveInfo); };
	CString GetDepartLoc2(){ return GetDepartLoc(m_ScInfo[1].m_strDepartInfo); };
	CString GetArriveLoc2(){ return GetArriveLoc(m_ScInfo[1].m_strArriveInfo); };
	COLORREF GetForkColor();
	COLORREF GetForkColor1();
	COLORREF GetForkColor2();
	COLORREF GetRailColor();
	COLORREF GetPostColor();



public:
//	BOOL IsOnlineMode() { return (m_ucRcMode == enOnline); }
//	BOOL IsAutoMode() { return (m_ucScMode == enAuto); }
//	BOOL IsActiveMode() { return (m_ucRcStatus == enActive); }
	BOOL IsInvoked() { return ((m_ScInfo[0].m_bInvoke == TRUE) || (m_ScInfo[1].m_bInvoke == TRUE)); }
//	BOOL IsWaitStatus() { return (m_ucScStatus == enWaiting || m_ucScStatus == enIdle); }
//	BOOL IsRunningStatus() { return (m_ucScStatus == enRunning); }
//	BOOL IsErrorStatus() { return (m_ucScStatus == enError); }
	BOOL IsDualStoreError() { return ((m_nErrorCode == enScErrorDualStore) ); }
	BOOL IsEmptyRetrieveError() { return ((m_nErrorCode == enScErrorEmptyRetrieve) ); }
//	BOOL IsStoreObstacle() { return ((m_nErrorCode == enScErrorStoreObstacle) || (m_nErrorCode == enScErrorStoreObstacle2)); }
//	BOOL IsRetrieveObstacle() { return ((m_nErrorCode == enScErrorRetrieveObstacle) || (m_nErrorCode == enScErrorRetrieveObstacle2)); }
	BOOL IsProductExist() { return (m_ucProdLoad != enProdEmpty); }		// PairType에도 사용가능
	BOOL IsProductEmpty() { return (m_ucProdLoad == enProdEmpty); }		// PairType에도 사용가능
//	BOOL IsCompleteStatusFork1() { return (m_ucJobStatus == en1ForkComplete); }
//	BOOL IsCompleteStatusFork2() { return (m_ucJobStatus == en2ForkComplete); }
//	BOOL IsCompleteStatusFork3() { return (m_ucJobStatus == enComplete); }
//	BOOL IsCompleteStatus() { return (IsCompleteStatusFork1() || IsCompleteStatusFork2() || IsCompleteStatusFork3()); }
};

#endif // !defined(AFX_SCINFO_H__CC7DC245_F32B_4FC3_BDD5_2E6836872D0C__INCLUDED_)
