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

	BYTE		m_ucFireAlarm;

} ScJobInfo;


class CEquipment;
class CStationInfo;
class CScInfo : public CInfo  
{
	DECLARE_SERIAL(CScInfo)
		
public:
	CScInfo() {}
	CScInfo(CEquipment* pEquipment);
	virtual ~CScInfo();

public:
	enum EN_TYPE { enL2R, enR2L, enT2B, enB2T, enTypeSize };
	enum EN_FORK_TYPE { enSingle, enTwin, enDouble, enSingleForkDoubleDeep, enMCS, enForkTypeSize };

	enum EN_RC_MODE { enOff=0, enOnline=1, enRemote=2 };																			// D 95
	enum EN_SC_MODE { enAuto=1, enManual=0 };																						// D100-L
	enum EN_PROD_LOAD { enProdEmpty=0, enProdExistFork1=1, enProdExistFork2=2, enProdExistFork12=3 };								// D100-H
	enum EN_SC_STATUS { enIdle=0, enWaiting=1, enRunning=2, enError=4  };															// D101
	enum EN_FORK_POS { enForkPosCenter= 0 , enForkPosBank1=1, enForkPosBank2=2 };													// D104
	enum EN_FORK_STATUS { enForkReady=0, enForkDualStoreError=1, enForkEmptyRetrieveError=2 };										// D106
	enum EN_RC_STATUS { enStop=0, enActive=1 };																						// D109
	enum EN_JOB_STATUS { enMoving=0, en1ForkComplete=1, en2ForkComplete=2, enComplete=3  };									// D110
	enum EN_CMD_TYPE { enCmdNone=0, enStore=1, enRetrieve=2, enHs2Hs=3, enRack2Rack=4, enHomeReturn=5, enMove=6, enDualStore=7 };	// D111
	enum EN_ROUTINE { enRoutineStore, enRoutineRetrieve, enRoutineRackToRack };
	enum EN_FLOOR_POS { enFloor0, enFloor1, enFloor2, enFloor3, enFloorSize };		// enFloor0 : 지하  

public:
	CDciRvCtrl* m_pControl;
	CDciRvCtrl* m_pControl1;
	CDciRvCtrl* m_pControl2;
	CDciRvCtrl* m_pControl3;
	CDciRvCtrl* m_pControl4;
	CStationInfo* m_pStation;
	CTrackHSArray m_pStosHS;
	CTrackHSArray m_pRetsHS;
	
public:
	BYTE m_ucRcStatus;			// 지상반 동작상태	{ enStop='0', enActive='1' }
	BYTE m_ucRcMode;			// 지상반 동작모드	{ enOnline='2', enRemote='1' }
	BYTE m_ucScMode;			// 기상반 동작모드	{ enAuto='1', enManual='0' }
	BYTE m_ucScStatus;			// 기상반 동작상태	{ enIdle=0, enWaiting='1', enRunning='2', enError='4' }
	BYTE m_ucJobStatus;			// 작업상태			{ enMoving='0', enComplete='1' }
	WORD m_wHorizontalPos;		// 수평주행위치
	WORD m_wVerticalPos;		// 수직주행위치
	BYTE m_ucProdLoad;			// 화물유무			{ enProdEmpty='0', enProdExist='1' }
	BYTE m_ucForkPos1;			// 포크위치			{ enForkPosCenter='0', enForkPosBank1='1', enForkPosBank2='2' }
	BYTE m_ucForkPos2;			// 포크위치			{ enForkPosCenter='0', enForkPosBank1='1', enForkPosBank2='2' }
	BYTE m_ucForkStatus;		// 포크상태			{ enForkReady=0, enForkDualStoreError=1, enForkEmptyRetrieveError=2 }
	int	 m_nErrorCode;			// 에러코드
//	int  m_nLuggNum;			// 작업번호
	BYTE m_ucCmdType;			// 작업구분 {없음=0, 입고=1, 출고=2, 직출고=3, 재배치=4, 홈복귀=5, 이동=6, 이중입고재지정=7}
//	CString m_strDepartInfo;	// 출발지(4x4) {열, 행, 단, HS}
//	CString m_strArriveInfo;	// 도착지(4x4) {열, 행, 단, HS}
	

//	BYTE m_ucJobCount;			// SRC가 보유한 작업개수
//	int	 m_nCompleteNum;		// 완료작업번호 (완료작업이 없으면 0 있으면 해당 작업번호)

public:
//	BOOL m_bInvoke;				// SC 작업지시 검사플래그
	int  m_nPrevErrorCode;		// 직전 에러코드
//	int	 m_nInternalLuggNum;
	int  m_nInternalJobType;
//	int	 m_nDepartTrackNum;
//	int	 m_nArriveTrackNum;
	int	 m_nPrevRoutine;
	BOOL m_bStoSuspend;
	BOOL m_bRetSuspend;
	BYTE m_ucPrevStatus;

public:
	int  m_nForkType;
	int  m_nDirectionType;		// 방향 타입
//	int  m_nLuggNum2;			// 작업번호
//	int	 m_nInternalLuggNum2;
//	BYTE m_ucCmdType2;			// 작업구분 {없음=0, 입고=1, 출고=2, 직출고=3, 재배치=4, 홈복귀=5, 이동=6, 이중입고재지정=7}
//	int  m_nInternalJobType2;
//	BOOL m_bInvoke2;				// SC 작업지시 검사플래그
//	CString m_strDepartInfo2;	// 출발지(4x4) {열, 행, 단, HS}
//	CString m_strArriveInfo2;	// 도착지(4x4) {열, 행, 단, HS}
//	int	 m_nDepartTrackNum2;
//	int	 m_nArriveTrackNum2;

public:
	int  m_nStartLevel;
	int  m_nEndLevel;

	BOOL m_bRtv;

public:
	ScJobInfo	m_ScInfo[2];
	int			m_nSelectedTab;
	int			m_nTabNum;
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
	virtual void ErrorReport(BOOL bManual = FALSE);

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
	BOOL SetRcStatus(BYTE ucRcStatus);
	BOOL SetRcMode(BYTE ucRcMode);
	void SetJobCount(BYTE ucJobCount);
	BOOL SetScMode(BYTE ucScMode);
	BOOL SetScStatus(BYTE ucScStatus);
	void SetHorizontalPos(WORD wHorizontalPos);
	void SetVerticalPos(WORD wVerticalPos);
	void SetProdLoad(BYTE ucProdLoad);
	void SetForkPos1(BYTE ucForkPos);
	void SetForkPos2(BYTE ucForkPos);
	void SetForkStatus(BYTE ucForkStatus);
	void SetErrorCode(WORD wErrorCode);
	void SetCompleteNum(int nCompleteNum);
	void SetLuggNum(WORD nLuggNum, int nFork = 0);
	void SetCmdType(BYTE ucCmdType);
	void SetDepartInfo(CString& strDepartInfo, int nFork = 0);
	void SetArriveInfo(CString& strArriveInfo, int nFork = 0);
	void SetJobStatus(BYTE ucJobStatus);

public:
	CString GetRcStatus();
	CString GetRcMode();
	CString GetScMode();
	CString GetScStatus();
	CString GetForkPos1();
	CString GetForkPos2();
	CString GetForkStatus();
	CString GetProdLoad();
	CString GetCmdType();
	CString GetJobStatus();
	CString GetPrevStatus();
	CString GetDepartInfoString();
	CString GetArriveInfoString();
	CString GetDepartInfoString2();
	CString GetArriveInfoString2();
	CString GetErrorString();
	CString GetXmlString();

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
	BOOL IsOnlineMode() { return (m_ucRcMode == enOnline); }
	BOOL IsAutoMode() { return (m_ucScMode == enAuto); }
	BOOL IsActive() { return (m_ucRcStatus == enActive); }
	BOOL IsInvoked() { return ((m_ScInfo[0].m_bInvoke == TRUE) || (m_ScInfo[1].m_bInvoke == TRUE)); }
	BOOL IsWaitStatus() { return (m_ucScStatus == enWaiting || m_ucScStatus == enIdle); }
	BOOL IsErrorStatus() { return (m_ucScStatus == enError); }
	BOOL IsDualStoreError() { return ((m_nErrorCode == enScErrorDualStore) || (m_nErrorCode == enScErrorDualStore2)); }
	BOOL IsEmptyRetrieveError() { return ((m_nErrorCode == enScErrorEmptyRetrieve) || (m_nErrorCode == enScErrorEmptyRetrieve2)); }
	BOOL IsStoreObstacle() { return ((m_nErrorCode == enScErrorStoreObstacle) || (m_nErrorCode == enScErrorStoreObstacle2)); }
	BOOL IsRetrieveObstacle() { return ((m_nErrorCode == enScErrorRetrieveObstacle) || (m_nErrorCode == enScErrorRetrieveObstacle2)); }
	BOOL IsProductExist() { return (m_ucProdLoad != enProdEmpty); }		// PairType에도 사용가능
	BOOL IsProductEmpty() { return (m_ucProdLoad == enProdEmpty); }		// PairType에도 사용가능
	BOOL IsCompleteStatusFork1() { return (m_ucJobStatus == en1ForkComplete); }
	BOOL IsCompleteStatusFork2() { return (m_ucJobStatus == en2ForkComplete); }
	BOOL IsCompleteStatusFork3() { return (m_ucJobStatus == enComplete); }
	BOOL IsCompleteStatus() { return (IsCompleteStatusFork1() || IsCompleteStatusFork2() || IsCompleteStatusFork3()); }
};

#endif // !defined(AFX_SCINFO_H__CC7DC245_F32B_4FC3_BDD5_2E6836872D0C__INCLUDED_)
