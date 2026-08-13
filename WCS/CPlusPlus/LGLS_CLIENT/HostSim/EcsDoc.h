// EcsDoc.h : interface of the CEcsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECSDOC_H__B97BBA17_6F50_487A_8719_FDFAA52355F4__INCLUDED_)
#define AFX_ECSDOC_H__B97BBA17_6F50_487A_8719_FDFAA52355F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "Config.h"
#include "Log.h"
#include "Job.h"


//#include "HostListener.h"
//#include "HostServer.h"
//#include "HostClient.h"

#include "Host.h"
//#include "HostServer.h"
//#include "HostClient.h"

#include "MonitorListener.h"
#include "MonitorServer.h"

#include "EcsDefine.h"
#include "EcsLayout.h"
#include "StationInfo.h"
#include "TrackInfo.h"

#include "Equipment.h"
#include "Cv.h"
#include "CvFx.h"
//#include "Sc.h"
//#include "ScPair.h"
#include "ScDongJin.h"
//#include "ScHalfPair.h"
//#include "Rtv.h"
//#include "Lifter.h"
#include "Lgv.h"
//#include "Rfid.h"
//#include "Bcr.h"
//#include "Display.h"

#include "JobDlg.h"
#include "LogDlg.h"
#include "WarningDlg.h"
#include "CvDlg.h"
#include "ScDongJinDlg.h"
#include "LgvcDlg.h"
#include "LgvDlg.h"

class CLgvDlg;
class CStatusBarMsg
{
public:
	CStatusBarMsg(CString strMsg, BOOL bNormalStatus)
	:m_strMsg(strMsg), m_bNormalStatus(bNormalStatus)
	{}
	
	CString m_strMsg;
	BOOL m_bNormalStatus;
};

typedef struct SJobInvokeInfo
{
public:
	CString m_strStoStation;
	CString m_strRetStation;

	CString m_strLocation;

	BOOL	m_bStart;			// TEST 시작 
	int		m_nWorkingLuggNum;	// 현재 작업중인 작업번호 
	BOOL	m_bCompleteStore;	// 입고작업완료 
	BOOL	m_bCompleteMove;	// 이동작업완료 
	int		m_nPrevLuggNum;		// PRE 작업번호 
	int		m_nDualStoScNo;
	CString	m_strFromPos;		// 기타 작업할 포지션 
	CString	m_strToPos;			// 기타 작업할 포지션 
	int		m_nWorkingJobType;	// 현재 작업중인 작업구분 
	CString m_strTime;			// 작업 시간


//	int		m_nScCount;			// 해당 로직에서 작업중인 크레인 수

	SJobInvokeInfo& operator=(const SJobInvokeInfo&) = default;
} SJobInvokeInfo;

typedef struct SLogicGorupInfo
{
public:
	BOOL	m_bStart;			// TEST 시작 
	CString	m_strAlterLocation;	// (이중입고 에러시)재지정 할 Location;
	int		m_nStoStnCnt;			// 해당 로직에서 작업중인 입고스테이션 수

	CArray<SJobInvokeInfo*, SJobInvokeInfo*> m_pJobInvokeInfos;

	CStringArray m_strStoStations;		// 해당 로직에서 작업중인 입고스테이션들
	CStringArray m_strViaStations;		// 해당 로직에서 작업중인 경유스테이션들
	CStringArray m_strRetStations;		// 해당 로직에서 작업중인 출고스테이션들
	CStringArray m_strScs;				// 해당 로직에서 작업중인 SC들

	SLogicGorupInfo& operator=(const SLogicGorupInfo&) = default;
} SLogicGorupInfo;



class CEcsDoc : public CDocument
{
protected: // create from serialization only
	BOOL CheckPassword();
	CEcsDoc();
	DECLARE_DYNCREATE(CEcsDoc)

// Attributes
public:
	CEcsLayout		m_layout;
	CEcsLayout		m_layout1;		// 1층
	CEcsLayout		m_layout2;		// 2층
	CConfig*		m_pConfig;		// 환경설정
	CLog*			m_pLog;			// 로그관리
//	CLogPlayback*	m_pLogPlayback;	// Play Back
	CJob*			m_pJob;			// 작업관리
	CPtrList		m_MsgList;		// 메시지
	
public:
	CArray<CDciStaticCtrl*, CDciStaticCtrl*> m_pRtvJobNums1;
	CArray<CDciStaticCtrl*, CDciStaticCtrl*> m_pRtvSrcDsts1;
	CArray<CDciStaticCtrl*, CDciStaticCtrl*> m_pRtvJobNums2;
	CArray<CDciStaticCtrl*, CDciStaticCtrl*> m_pRtvSrcDsts2;
	CArray<CDciStaticCtrl*, CDciStaticCtrl*> m_pScRetFronts;
	CArray<CDciStaticCtrl*, CDciStaticCtrl*> m_pScRetRears;
//	CArray<CDciStaticCtrl*, CDciStaticCtrl*> m_pStoDataChk1;
//	CArray<CDciStaticCtrl*, CDciStaticCtrl*> m_pStoDataChk2;
	CArray<SLogicGorupInfo*, SLogicGorupInfo*> m_pLogicGorupInfos;

	int m_nScRetFrontRemainJobCnt[SC1_PLC_CNT];
	int m_nScRetRearRemainJobCnt[SC_PLC_TOT_CNT];

//	BOOL m_bScRetFrontCountChanged[SC1_PLC_CNT];
//	BOOL m_bScRetRearCountChanged[SC_PLC_TOT_CNT];

public:
//	CMonitorListener*	m_pMonitorListener;
	CListenSk*			m_pHostListener;
	CHostSv*			m_pHostSv;
	CHostCl*			m_pHostCl;

public:
	CEquipmentArray		m_pEquipments;
	CTrackInfoArray		m_pTrackInfos;
	CStationInfoArray	m_pStationInfos;
	CMapStringToString	m_mapRetLimitInfos;

public:
	HWND			m_hWndView;
	CJobDlg*		m_pJobDlg;
	CLogDlg*		m_pLogDlg;
	CWarningDlg*	m_pWarningDlg;
	CCvDlg			m_oCvDlg;
	CScDongJinDlg	m_oScDlg;
	CLgvcDlg		m_oRgvcDlg;
	CLgvDlg			m_oRgvDlg;

public:
	BOOL			m_bTesting;
	CString			m_strTestLocation;
	CString			m_strTestStation;
	BOOL			m_bLogPlayback;

	BOOL			m_bLog;

public:
	int				m_nBank;
	int				m_nBay;
	int				m_nLevel;

public:
	int				m_nAutoState;

public:
	int				m_nPairLuggNum;

public:
	BOOL			m_bCommStatus;

public:
	BOOL			m_bProdInfo;
	int 			m_nProdAddr;
	int 			m_nProdDate;
	int 			m_nProdCount;
	int 			m_nProdPlc;

public:
	CString			m_strDepartPos[10000];
	CString			m_strArrivePos[10000];
	CString			m_strWeightVal[10000];
	CString			m_strProductID[10000];

	CString			m_strStoLocation[10000];
	CString			m_strRetLocation[10000];



public:
	//2개의 크래인에서 3개의 PLC
	WORD m_wSc1Status910;
	WORD m_wSc1Status911;
	WORD m_wSc1Status912;
	WORD m_wSc2Status910;
	WORD m_wSc2Status911;
	WORD m_wSc1Status920;
	WORD m_wSc2Status921;
	WORD m_wSc3Status922;

public:
	CString m_strStoStation;
	CString m_strRetStation;

	CString m_strLocation;

//	CString m_strStoLocation;
//	CString m_strRetLocation;
	int		m_nMaxBank;
	int		m_nMaxBay;
	int		m_nMaxLevel;

	int		m_nWorkingLuggNum1;	// 현재 작업중인 작업번호 
	int		m_nWorkingLuggNum2;	// 현재 작업중인 작업번호 
	BOOL	m_bCompleteStore1;	// 입고작업완료 
	BOOL	m_bCompleteStore2;	// 입고작업완료 
	int		m_nPrevLuggNum;	// PRE 작업번호 
	int		m_nPrevLuggNum2;	// PRE 작업번호 
	CString	m_strAlterLocation;	// (이중입고 에러시)재지정 할 Location;
	int		m_nDualStoScNo;
	CString	m_strFromPos;		// 기타 작업할 포지션 
	CString	m_strToPos;			// 기타 작업할 포지션 

	//SJobInvokeInfo m_JobInvokeInfo[STO_STN_CNT];
	//////////////////////////////////
	// 0 : 정상						//
	// 1 : 출고작업 수행중			//
	// 2 : 출고작업 수행 중			//
	// 3 : Rack to Rack 수행 중		//
	// 4 : Online 모드가 아닌 상태	//
	// 5 : 에러발생 상태			//
	// 6 : 입고중지 상태			//
	// 7 : 출고중지 상태			//
	// 8 : 입출고 중지상태			//
	////////////////////////////////// 
	int		m_nScStatus[SC_CNT];
	BOOL	m_bStoStation[STO_STN_CNT];
	
	BOOL	m_bReceiveStatus;
private:
	BOOL Initialize();

public:
	void GenerateSemiMoveJob(int nStratStnNo, int nDestStnNo);
	void OnJobOffline1();
	void OnJobOffline2(); 

public:
	void RunServers();
	void ConnectHostServer();
	void ReconnectServer();
	void ExcuteClient(int nEquipKind, int nEquipNum);
	void ExcuteServer(int nEquipKind, int nEquipNum);
	void StatusReport() { m_pEquipments.StatusReport(); }
	void InvokeControl() { m_pEquipments.InvokeControl(TRUE); m_pTrackInfos.InvokeControl(TRUE); }
	BOOL IsIdleJob(int nLuggNum) { return m_pEquipments.IsIdleJob(nLuggNum); }

public:
	void MulticastServerInfo(CMonitorServer* pMonitorSv, int nCommStatus);
	void MulticastClientInfo(CMonitorServer* pMonitorSv, int nCommStatus);

public:
	void StatusDisplay();
	void StatusDisplay(CString strMsg, BOOL bNormalStatus = TRUE);
	void WriteLog(int nLogType, int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszPos, LPCTSTR lpszProdID = NULL, BOOL bAlarm = FALSE);
	void WriteLog(int nLogType, int nLogPos, LPCTSTR lpszContent, LPCTSTR lpszPos, CJobItem* pJobItem = NULL, BOOL bAlarm = FALSE);
	void Alarm(int nLogPos, LPCTSTR lpszLog, CJobItem* pJobItem = NULL);

public:
	CDciControl*	GetDciControl(CString& strCID) ;// { return m_layout.GetDciControl(strCID); }
	CEquipment*		GetEquipment(CString& strDevice) { return m_pEquipments.GetEquipment(strDevice); }
	CEquipment*		GetEquipment(int nEquipKind, int nEquipNum) { return m_pEquipments.GetEquipment(nEquipKind, nEquipNum); }
	CTrackInfo*		GetTrackInfo(int nTrackNum) { return m_pTrackInfos.GetTrackInfo(nTrackNum); }
	CStationInfo*	GetStationInfo(CString& strStationID) { return m_pStationInfos.GetStationInfo(strStationID); }
	CStationInfo*	GetScStationInfo(int nWarehouse, CString& strLocation) { return m_pStationInfos.GetScStationInfo(nWarehouse, strLocation); }
	CString			GetStationName(CString& strStationID) { return m_pStationInfos.GetStationName(strStationID); }
	int				GetAutoState() { return m_nAutoState;}
	CScInfo*		GetScInfo(int nScNum) { return (CScInfo*)m_pEquipments.GetInfo(CEquipment::enSC, nScNum); }
//	CRtvInfo*		GetRtvInfo(int nRtvNum) { return (CRtvInfo*)m_pEquipments.GetInfo(CEquipment::enRTV, nRtvNum); }

public:
	void SetAllUnSetSusspend() { m_pEquipments.SetAllUnSetSusspend(); }
	void SetAllSetSusspend() { m_pEquipments.SetAllSetSusspend(); }

public:
	void GetViewHandle();
	CView* GetViewObject();
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEcsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEcsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	afx_msg void OnWarning();

// Generated message map functions
public:
	//{{AFX_MSG(CEcsDoc)
	afx_msg BOOL OnComStatusHost(UINT nID);
	afx_msg BOOL OnComStatusClient(UINT nID);
	afx_msg BOOL OnComStatusServer(UINT nID);
	afx_msg void OnSystemConfiguration1();
	afx_msg void OnSystemConfiguration2();
	afx_msg void OnSystemConfiguration3();
	afx_msg void OnSystemConfiguration4();
	afx_msg void OnJobOffline();
	afx_msg void OnJobTestStart();
	afx_msg void OnJobTestStop();
	afx_msg void OnJobManager();
	afx_msg void OnLogAll();
	afx_msg void OnRackConfig();
	afx_msg void OnSysUserManager();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECSDOC_H__B97BBA17_6F50_487A_8719_FDFAA52355F4__INCLUDED_)
