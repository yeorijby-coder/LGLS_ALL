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

#include "Logic.h"
#include "KeyWord.h"
#include "DeviceMap.h"

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
#include "WordArrayEx.h"
#include "TrackDataDlg.h"
#include "LogicValidationDlg.h"
#include "DataSettingDlg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <afxtempl.h>

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

class CStatusBarMsg
{
public:
	CStatusBarMsg(CString strMsg, BOOL bNormalStatus) : m_strMsg(strMsg), m_bNormalStatus(bNormalStatus) {}
	
	CString m_strMsg;
	BOOL m_bNormalStatus;
};
//class CDeviceMap;
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
	CArray<CDeviceMap*, CDeviceMap*> m_pDeviceMaps;			// 
//	CArray<CStringList*, CStringList*> m_strlistOneTrack;


	//==>
	CConfig*		m_pConfig;		// 환경설정
	CLog*			m_pLog;			// 로그관리
	CJob*			m_pJob;			// 작업관리
	CPtrList		m_MsgList;		// 메시지
	CEcsControl		m_oControlDlg;

public:
	CEquipmentArray		m_pEquipments;
	CTrackInfoArray		m_pTrackInfos;
	CStationInfoArray	m_pStationInfos;
	CMapStringToString	m_mapRetLimitInfos;
	CMap<int, int, CString, CString> m_mapTempVals;	// 온도값 관리
	CMap<int, int, CString, CString> m_mapHumVals;	// 습도값 관리
	CMap<int, int, int, int> m_mapCEquipStatus;		// 중앙설비상태 관리

public:
	CWordArrayEx		m_arrRegData[CV_PLC_CNT];

public:
	CArray	<CLogicArray, CLogicArray>	m_arrLogicGroups;			// LogicGroups  중요
	//int					m_n1TrackWord;
	int					m_n1TrackKeyWordCnt;
	//CMapStringToString	m_pTrackProperties[CV_PLC_CNT];
	CTrackPropertyArray m_pTrackProperties[CV_PLC_CNT];
	CFlantPropertyArray m_pFlantProperties[CV_PLC_CNT];
	CMapStringToString	m_pKeyWords;								// Parent KeyWord 찾을때만 사용됨 
//	CStringArray		m_strKeyWords;
	CArray	<CStringArray*, CStringArray*> m_strTrackingAreas;		// CEcsView::MoveNextTrackForKindNormal, CEcsView::MoveNextTrackForKindDeverter,

public:
	CStringArray m_strEtcKeyWords;

public:
	HWND			 m_hWndView;
	CJobDlg*		 m_pJobDlg;
	//	CLogDlg*		m_pLogDlg;
	CWarningDlg*	 m_pWarningDlg;
	CTrackDataDlg* 	 m_pTrackDataDlg;
	CDataSettingDlg* m_pDataSettingDlg;

	CLogicValidationDlg* m_pLogicValidationDlg;

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
	BOOL			m_bScInterLock;
	int				m_nStTrNum[CV_PLC_CNT];
	int				m_nNextPlcNum;
	int				m_nNextPlcNum2;
	int				m_nCenterNextPlcNum;
	int				m_nCrossPlcNum;
	BOOL			m_bParent;
	int				m_nPlcCnt;
	
//	CString			m_sTwoKeywordName[100];
//	CString			m_sTenKeywordName[100];
//	CString			m_sFiveKeywordName[100];

	CString			m_sEcsDefineLoad[1000][16];

	int				m_nTempPlcNum;
	int				m_nTempDevNum;

	int				m_nWordCnt;		// 기본값 : 10 - Track별 Word 개수 

public:
	BOOL			m_bReLoading;
	
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
	COleDateTime	m_tChecktime;

public:
	//
	EN_LAYOUT		m_layoutSelected;

	// 
public:
	int			m_nScDestFrom;
	int			m_nScDestTo;
	CString		m_strDestList;					// ',' 구분함
	CStringList m_strTrackingKeywordList;		// ',' 구분함

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
	BOOL IsIdleJob(int nLuggNum) { return m_pEquipments.IsIdleJob(nLuggNum); }
	// 실시간 물류 가동률 상태 업데이트
	void UpdateOperationRate();

public:
	void MulticastServerInfo(CMonitorServer* pMonitorSv, int nCommStatus);
	void MulticastClientInfo(CMonitorServer* pMonitorSv, int nCommStatus);

public:
	int GetAddrByName(int nNumber, int nDevNum, CString strArgName, BOOL bParent = FALSE);
	int SetAddrByName(int nNumber, int nDevNum, CString strArgName, WORD wData, int nOption = 0);
	int GetBitInOrderByWord(int nNumber, int nDevNum, CString strArgName);
//	int	UpdateRegDataByName(CString strName, BOOL bSaveObject = TRUE) { return UpdateRegDataByName(m_pTrackProperties, strName, bSaveObject); }
//	int	GetRegDataByName(CString strName) { return UpdateRegDataByName(m_pTrackProperties, strName, TRUE); }
//	int	SetRegDataByName(CString strName) { return UpdateRegDataByName(m_pTrackProperties, strName, FALSE); }
	CString GetParentWord(CString strKeyWord);
//	BOOL IsScDest(int nValue) { return (nValue > m_nScDestFrom && nValue < m_nScDestTo); }
	BOOL IsStationKind(CTrackInfo* pTrack, CString strKeyWord);
	BOOL IsDestination(CString strArgName, int nValue, int nTrNo, int nMethod);
	BOOL LookupKeywordInfo(CString strKeyWord, CString& strType, CString& strAddr, CString& strInOrder);
	int GetSignalIndex(CString strKeyWord);

public:
	CTrackProperty* GetTrackPropertyByKeyword(int nPlcNum, int nTrackNo, CString strKeyWord, BOOL bPrent = FALSE)
	{ return m_pTrackProperties[nPlcNum].GetTrackProperty(strKeyWord, nTrackNo, bPrent); }

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
	BOOL			GetUseByPos(int nPos);
//	BOOL			GetUseByTrack(int nTrackNo);
	CDeviceMap*		GetDeviceMapByTrack(int nTrack);
//	CDestinationArray*	GetDestinationsByDestNTrack(int nTrack, int nDest);

public:
	void WriteCVPLCWord(int nPLCNum, int nAddress, WORD wData);

public:
	void LoadKeyWord(int nCount);
	void LoadDeviceMap(int nCount);
	void LoadLogic(int nCount);

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
	afx_msg void OnSystemConfig3();
	afx_msg void OnSysUserManager();
	afx_msg void OnJobManager();
	afx_msg void OnJobOffline();
	afx_msg void OnLogAll();
	afx_msg void OnWarning();
	afx_msg void TrackData(CString Type, CString Addr, CString Name, CString Value);
	afx_msg void TrackConditionData(CString Value);
	afx_msg void OnButton11();
	afx_msg void OnButton22();
	afx_msg void OnButton33();
	afx_msg void OnLogicValid();

	afx_msg void OnReLoadKeyWord();
	afx_msg void OnReLoadDeviceMap();
	afx_msg void OnReLoadLogic();
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
