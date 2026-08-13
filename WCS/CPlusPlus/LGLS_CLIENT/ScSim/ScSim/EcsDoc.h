// EcsDoc.h : interface of the CEcsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECSDOC_H__B97BBA17_6F50_487A_8719_FDFAA52355F4__INCLUDED_)
#define AFX_ECSDOC_H__B97BBA17_6F50_487A_8719_FDFAA52355F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
#include "Sc.h"
#include "ScPair.h"
//#include "Bcr.h"
//#include "Rtv.h"

#include "JobDlg.h"
#include "LogDlg.h"
#include "WarningDlg.h"
#include "CvDlg.h"
#include "LogPlayback.h"
#include "EcsControl.h"

struct SClReadThreadInfo
{
	CEcsDoc* m_pDoc;
		
	HANDLE m_hEventKillClReadThread;
	HANDLE m_hEventClReadThreadKilled;
};
UINT ClReadThreadProc(LPVOID pParam);

struct SSvReadThreadInfo
{
	CEcsDoc* m_pDoc;
		
	HANDLE m_hEventKillSvReadThread;
	HANDLE m_hEventSvReadThreadKilled;
};
UINT SvReadThreadProc(LPVOID pParam);

typedef struct SScPlcDataGroup
{
	int				m_nStartAddress;
	int				m_nWordLenth;
	int				m_nPointer;
	CWordArray		m_arrData;
	CStringArray	m_arrName;
} SScPlcDataGroup;

class CStatusBarMsg
{
public:
	CStatusBarMsg(CString strMsg, BOOL bNormalStatus) : m_strMsg(strMsg), m_bNormalStatus(bNormalStatus) {}
	
	CString m_strMsg;
	BOOL m_bNormalStatus;
};

class CEcsDoc : public CDocument
{
protected: // create from serialization only
	CEcsDoc();
	DECLARE_DYNCREATE(CEcsDoc)

// Attributes
public:
	//<<== 15.2.9 cwb - 탭 추가
	CEcsLayout		m_layout1;		// 1F
	CEcsLayout		m_layout2;		// 2F
	CEcsLayout		m_layoutAssemble; // Assemble  

	CArray<CEcsLayout*, CEcsLayout*> m_pEcsLayOuts;
	//==>
	CConfig*		m_pConfig;		// 환경설정
	CLog*			m_pLog;			// 로그관리
//	CLogPlayback*	m_pLogPlayback;	// Play Back
	CJob*			m_pJob;			// 작업관리
	CPtrList		m_MsgList;		// 메시지
	CEcsControl		m_oControlDlg;

public:
	//CMonitorListener*	m_pMonitorListener;
	//CListenSk*			m_pHostListener;
	//CHostSv*			m_pHostSv;
	//CHostCl*			m_pHostCl;

public:
	CEquipmentArray		m_pEquipments;
	CTrackInfoArray		m_pTrackInfos;
	CStationInfoArray	m_pStationInfos;
	CMapStringToString	m_mapRetLimitInfos;
	CMap<int, int, CString, CString> m_mapTempVals;	// 온도값 관리
	CMap<int, int, CString, CString> m_mapHumVals;	// 습도값 관리
	CMap<int, int, int, int> m_mapCEquipStatus;		// 중앙설비상태 관리
	CWordArray			m_arrRegData[101];
	CTrackHSArray		m_pStosHS;
	CTrackHSArray		m_pRetsHS;

	SScPlcDataGroup		m_spdgPlc2Ecs;
	SScPlcDataGroup		m_spdgEcs2Plc;
	SScPlcDataGroup		m_spdgInterPlc;

public:
	HWND			m_hWndView;
	CJobDlg*		m_pJobDlg;
//	CLogDlg*		m_pLogDlg;
	CWarningDlg*	m_pWarningDlg;
	CCvDlg			m_oCvDlg;

public:
	BOOL			m_bTesting;
	CString			m_strTestLocation;
	CString			m_strTestStation;
	BOOL			m_bLogPlayback;
	COleDateTime	m_dtHeartBit;
	BOOL			m_bHeartBit;
	CString			m_strCEquipInfo;	// 중앙설비상태 클라이언트 송신용 정보
	int				m_nLoopTrayCount;	// Update : 2014.06.08 - Loop track 실시간 물류 TRAY 수량
	int				m_nCurCapacity;		// Update : 2014.06.08 - 현재 가동률
	BOOL			m_bBackupChk;
	BOOL			m_bUnLoadComplete[100];
	int				m_nCompleteFork[100];
	int				m_nCurrLoadingHsNum[100];
	int				m_nCurrLoadingTrackNum[100];
	int				m_nCurrLoadingTrackNum2[100];
	int				m_nCurrLoadingPlcNum[100];
	int				m_nCurrUnLoadingTrackNum[100];
	int				m_nCurrUnLoadingTrackNum2[100];
	int				m_nCurrUnLoadingPlcNum[100];
	BOOL			m_bLoadingTrackDelete[100];
	BOOL			m_bLoadingTrackDelete2[100];
	BOOL			m_bUnLoadingTrackDelete[100];
	BOOL			m_bUnLoadingTrackDelete2[100];
	BOOL			m_bNextDualStore;
	BOOL			m_bNextEmptyRetrieve;

	BOOL			m_bScInterLock;
	int				m_nStTrNum[30];
	int				m_nNextPlcNum;
	int				m_nNextPlcNum2;
	int				m_nCenterNextPlcNum;
	int				m_nCrossPlcNum;
public:
	enum EN_LAYOUT
	{
			EN_1F,
			EN_2F,
			EN_Assemble
	};

public:
	BOOL			m_bMoveFlag;
	CPoint			m_PrevPoint;
	CTime			m_tChecktime;

public:
	//
	EN_LAYOUT		m_layoutSelected;



public:
	// 2개의 크래인에서 3개의 PLC
	// ?????
//	WORD m_wSc1Status910;
//	WORD m_wSc1Status911;
//	WORD m_wSc1Status912;
//	WORD m_wSc2Status910;
//	WORD m_wSc2Status911;
//	WORD m_wSc1Status920;
//	WORD m_wSc2Status921;
//	WORD m_wSc3Status922;

private:
	BOOL Initialize();

public:
	void RunServers();
	void ReStartHostListener();
	void ReconnectServer();
	void ExcuteClient(int nEquipKind, int nEquipNum);
	void ExcuteServer(int nEquipKind, int nEquipNum);
	void StatusReport() { m_pEquipments.StatusReport(); }
	void InvokeControl() { m_pEquipments.InvokeControl(TRUE); m_pTrackInfos.InvokeControl(TRUE); }
	void InvokeCenterEquipCtrl(CString strFrame);
	BOOL IsIdleJob(int nLuggNum) { return m_pEquipments.IsIdleJob(nLuggNum); }	//읽어보자 LJM
	// 실시간 물류 가동률 상태 업데이트
	void UpdateOperationRate();

	BOOL LoadXML();
	BOOL SaveXML();
	void DeleteData(int nPlcNum, int nFork); 
	void DeleteData2(int nPlcNum); 
	void DeleteData1(int nPlcNum); 
	CEquipment* CreateEquipment(CString& strClassName, int nIndex, int nNumber, LPCTSTR lpszDevice);

public:
	void MulticastServerInfo(CMonitorServer* pMonitorSv, int nCommStatus);
	void MulticastClientInfo(CMonitorServer* pMonitorSv, int nCommStatus);

public:
	void StatusDisplay();
	void StatusDisplay(CString strMsg, BOOL bNormalStatus = TRUE);
	void WriteLog(int nLogType, int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszPos, LPCTSTR lpszProdID = NULL, BOOL bAlarm = FALSE);
	void WriteLog(int nLogType, int nLogPos, LPCTSTR lpszContent, LPCTSTR lpszPos, CJobItem* pJobItem = NULL, LPCTSTR lpszProdID = NULL, BOOL bAlarm = FALSE);
	void Alarm(int nLogPos, LPCTSTR lpszLog, CJobItem* pJobItem = NULL, LPCTSTR lpszBcr = _T(""));

public:
//	CDciControl*	GetDciControl(CString& strCID) { return m_layout.GetDciControl(strCID); }
	CDciControl*	GetDciControl(CString& strCID, int nTabNum =0 ) ;
	CEquipment*		GetEquipment(CString& strDevice) { return m_pEquipments.GetEquipment(strDevice); }
	CEquipment*		GetEquipment(int nEquipKind, int nEquipNum) { return m_pEquipments.GetEquipment(nEquipKind, nEquipNum); }
	CTrackInfo*		GetTrackInfo(int nTrackNum, int nCvPlcNum = 0) { return m_pTrackInfos.GetTrackInfo(nTrackNum, nCvPlcNum); }
	CStationInfo*	GetStationInfo(CString& strStationID) { return m_pStationInfos.GetStationInfo(strStationID); }
	CStationInfo*	GetScStationInfo(int nWarehouse, CString& strLocation) { return m_pStationInfos.GetScStationInfo(nWarehouse, strLocation); }
	CString			GetStationName(CString& strStationID) { return m_pStationInfos.GetStationName(strStationID); }
//	CTrackInfo*		GetDeadLockTrack(int nTrackNum) { return m_pEquipments.GetDeadLockTrack(nTrackNum); }
	COLORREF		GetColor(int nStatus);
	BOOL			GetLocationByHs(int nScPlcNum, int nType, int nHsNum, int& nBank, int& nBay, int& nLevel, int& nTrackNum, int& nPairTrackNum, int& nPlcNum);	
	CTrackHS*		FindHsByCvTrack(int nCvPlcNum, int nTrackNum, int nType);	// nType => 1:입고HS(STO_HS), 2:출고HS(RET_HS)

public:
	void WriteCVPLCWord(int nPLCNum, int nAddress, WORD wData);
	
public:
	void SetAllUnSetSusspend() { m_pEquipments.SetAllUnSetSusspend(); }
	void SetAllSetSusspend() { m_pEquipments.SetAllSetSusspend(); }

public:
	void GetViewHandle();
	CView* GetViewObject();
	
// Operations
public:
	CEcsLayout* GetSelectedLayout();
	CEcsLayout* GetSelectedLayout(EN_LAYOUT pEnLayout);

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

// Generated message map functions
public:
	//{{AFX_MSG(CEcsDoc)
	afx_msg BOOL OnComStatusHost(UINT nID);
	afx_msg BOOL OnComStatusClient(UINT nID);
	afx_msg BOOL OnComStatusServer(UINT nID);
	afx_msg void OnJobTestStart();
	afx_msg void OnJobTestStop();
	afx_msg void OnSystemConfig1();
	afx_msg void OnSystemConfig2();
	afx_msg void OnSysUserManager();
	afx_msg void OnJobManager();
	afx_msg void OnJobOffline();
	afx_msg void OnLogAll();
	afx_msg void OnWarning();
	afx_msg void OnButton11();
	afx_msg void OnButton22();
	afx_msg void OnButton33();
	//afx_msg void OnComCv0101();
	//afx_msg void OnComCv0102();
	//afx_msg void OnComCv0103();
	//afx_msg void OnComCv0104();
	//afx_msg void OnComCv0105();
	//afx_msg void OnComCv0106();
	//afx_msg void OnComCv0107();
	//afx_msg void OnComCv0108();
	//afx_msg void OnComCv0109();
	//afx_msg void OnComCv0110();
	//afx_msg void OnComCv0111();
	//afx_msg void OnComCv0112();
	//afx_msg void OnComCv0113();
	//afx_msg void OnComCv0114();
	//afx_msg void OnComCv0115();
	//afx_msg void OnComCv0116();
	//afx_msg void OnComCv0201();
	//afx_msg void OnComCv0202();
	//afx_msg void OnComCv0203();
	//afx_msg void OnComCv0204();
	//afx_msg void OnComCv0205();
	//afx_msg void OnComCv0206();
	//afx_msg void OnComCv0207();
	//afx_msg void OnComCv0208();
	//afx_msg void OnComCv0209();
	//afx_msg void OnComCv0210();
	//afx_msg void OnComCv0211();
	//afx_msg void OnComCv0212();
	//afx_msg void OnComCv0213();
	//afx_msg void OnComCv0214();
	//afx_msg void OnComCv0215();
	//afx_msg void OnComCv0216();
	//afx_msg void OnComCv0301();
	//afx_msg void OnComCv0302();
	//afx_msg void OnComCv0303();
	//afx_msg void OnComCv0304();
	//afx_msg void OnComCv0305();
	//afx_msg void OnComCv0306();
	//afx_msg void OnComCv0307();
	//afx_msg void OnComCv0308();
	//afx_msg void OnComCv0309();
	//afx_msg void OnComCv0310();
	//afx_msg void OnComCv0311();
	//afx_msg void OnComCv0312();
	//afx_msg void OnComCv0313();
	//afx_msg void OnComCv0314();
	//afx_msg void OnComCv0315();
	//afx_msg void OnComCv0316();
	//afx_msg void OnComCv0401();
	//afx_msg void OnComCv0402();
	//afx_msg void OnComCv0403();
	//afx_msg void OnComCv0404();
	//afx_msg void OnComCv0405();
	//afx_msg void OnComCv0406();
	//afx_msg void OnComCv0407();
	//afx_msg void OnComCv0408();
	//afx_msg void OnComCv0409();
	//afx_msg void OnComCv0410();
	//afx_msg void OnComCv0411();
	//afx_msg void OnComCv0412();
	//afx_msg void OnComCv0413();
	//afx_msg void OnComCv0414();
	//afx_msg void OnComCv0415();
	//afx_msg void OnComCv0416();
	//afx_msg void OnComCv0501();
	//afx_msg void OnComCv0502();
	//afx_msg void OnComCv0503();
	//afx_msg void OnComCv0504();
	//afx_msg void OnComCv0505();
	//afx_msg void OnComCv0506();
	//afx_msg void OnComCv0507();
	//afx_msg void OnComCv0508();
	//afx_msg void OnComCv0509();
	//afx_msg void OnComCv0510();
	//afx_msg void OnComCv0511();
	//afx_msg void OnComCv0512();
	//afx_msg void OnComCv0513();
	//afx_msg void OnComCv0514();
	//afx_msg void OnComCv0515();
	//afx_msg void OnComCv0516();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECSDOC_H__B97BBA17_6F50_487A_8719_FDFAA52355F4__INCLUDED_)
