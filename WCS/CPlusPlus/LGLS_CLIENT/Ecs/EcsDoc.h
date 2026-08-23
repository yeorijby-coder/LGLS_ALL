// EcsDoc.h : CEcsDoc 클래스의 인터페이스
//

//#import "CIM.Hsms.dll.tlb" no_namespace named_guids

#pragma once

#include "Config.h"

#include "EcsDefine.h"
#include "EcsLayout.h"
#include "StationInfo.h"
#include "TrackInfo.h"

#include "Equipment.h"
#include "Cv.h"
#include "Sc.h"
#include "Bcr.h"
#include "ScPair.h"
#include "Rtv.h"
#include "Wc.h"
#include "ScManualRet.h"

#include "AdoDB.h"
#include "URMDBAccess.h"

#include "Lang.h"
#include "Permission.h"
#include "MsgBoxLang.h"


#include "ViewJobListDlg.h"
#include "ViewHostEmptyPltDlg.h"
#include "ViewSearchDlg.h"
#include "EqpSuspendDlg.h"
#include "UserUserDlg.h"


#include "ManualRtv.h"
#include "ManualSc.h"
#include "ManualJob.h"
#include "ManualEmpty.h"
#include "ManualLogin.h"

#include "CollectDataList.h"
#include "JobCollection.h"

#include "ErrorMst.h"
#include "CollectDB.h"

#include "EQP_ECD_MST.h"
#include "FireParms.h"

#include "MainFrm.h"
#include "ConnectStatus.h"
#include "ConfigStatus.h"
#include "ConfigLogDelete.h"



class CCriticalSectionEx : public CCriticalSection
{
public:
	CCriticalSectionEx() {};
	CCriticalSectionEx(const CCriticalSectionEx& cs)
	{

	};
};

typedef struct STCollectDataList_CellInfo_Request
{
	bool m_bRequest;	
	CString m_strWH_TYP;		
	CString m_strBANK;
	CString m_strAGING_TYP;

} STCollectDataList_CellInfo_Request;

typedef struct STCollectDataList_JOB_MST_Request
{
	bool m_bRequest;	
	CString m_strWH_TYP;	
} STCollectDataList_JOB_MST_Request;

typedef struct STFIRE_EQP_MESSAGE
{
	CString m_strKey;
	BOOL m_bStop;
	CString strWH_TYP;
	CString strPLC_NO;
	CString strTRACK_NO;
	CString strLOCATION_NO;
	CString strMESSAGE;
	CString strLEVEL;

} STFIRE_EQP_MESSAGE;

// CEcsDoc
//
class CBcr;
class CFireParms;
class CSystemLoginDlg;
class CViewUsageRackDlg;
class CEcsDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CEcsDoc();
	DECLARE_DYNCREATE(CEcsDoc)

public:

#define CONST_WH_TYP = "10";

public:
	afx_msg void OnCommandRangeMainFrameCONFIGURATION(UINT nID);
	afx_msg void OnCommandRangeMainFrameVIEW(UINT nID);
	afx_msg void OnCommandRangeMainFrameMONITORING(UINT nID);
	afx_msg void OnCommandRangeMainFrameNATION(UINT nID);
	afx_msg void OnCommandRangeMainFrameMANUAL(UINT nID);
	afx_msg void OnCommandTrackTextMode(UINT nID);
	afx_msg void OnUpdateTrackTextMode(CCmdUI* pCmdUI);	// [LGLS 2026-08-22] 현재 선택된 보기 표시
	afx_msg void OnCommandSemiTestOpen();	// [LGLS 2026-08-13] 반자동 TEST 창
	afx_msg void OnCommandSemiTestClear();	// [LGLS 2026-08-13] TEST 초기화	// [LGLS 2026-07-20 재적용] 트랙 표시모드(작업번호/트랙번호/제품정보)
	afx_msg void OnCommandRangeMainFrameLOG(UINT nID);
	afx_msg void OnCommandRangeMainFrameUSER(UINT nID);
	afx_msg void OnCommandRangeMainFrameSTATUS(UINT nID);
	afx_msg void OnMenuClick_VIEW_RACK();

	


// 특성입니다.
public:
	double m_pMaxSizeX;
	double m_pMaxSizeY;
	CString		    m_PC_IP;
	CString			m_WH_TYP;
	CString		    m_strKioskNo;
	CString			m_strSeachTrack;
	CString			m_strSeachTemp;
	CString			m_strDebug;

	EN_LAYOUT		m_enSelectedLayout;

	CEcsLayout		m_layoutHUN_GF;
	CEcsLayout		m_layoutHUN_GFDECK;	
	CEcsLayout		m_layoutHUN_1F;		
	CEcsLayout		m_layoutHUN_1FDECK;
	CEcsLayout		m_layoutHUN_ASSEMBLE;

	CEcsLayout		m_layoutCHIN_1F;
	CEcsLayout		m_layoutCHIN_2F;
	CEcsLayout		m_layoutCHIN_ASSEMBLE;
	CEcsLayout		m_layoutCHIN_COMMP;

	CEcsLayout		m_layout;

	CArray<CEcsLayout*, CEcsLayout*> m_pEcsLayOuts;

	BOOL			m_bViewFirstLoad;

	BOOL				m_bDebugMode;
	BOOL				m_bDebugStart;

	CString			m_strBypassChkYn;

	BOOL			m_bMoveFlag; //1
	CPoint			m_PrevPoint;
	CTime			m_tChecktime;

	int				m_nStTrNum[30];

private:
	CURMDBAccess*		m_pUrmDBAccess;
	CURMDBAccess*		m_pDlgUrmDBAccess;
	
	BOOL				m_bTrans;

public:
	int GetSelectQryCnt(CString pStrSql);
	int GetSelectQryCnt_DLG(CString pStrSql);
	_RecordsetPtr GetSelectQryRecordsetPtr(CString pStrSql, int &pnRowCnt, CString &pStrMessage);
	_RecordsetPtr GetSelectQryRecordsetPtr_DLG(CString pStrSql, int &pnRowCnt, CString &pStrMessage);
		
public: 
	CCollectDB* m_pCollectDB;

public:
	CJobCollection* m_pJob;					//사용되지 않음
	CCollectDataList* m_pCollectRequest;    //사용되지 않음
	STCollectDataList_CellInfo_Request m_CollectCellInfo; //사용되지 않음
	STCollectDataList_JOB_MST_Request m_CollectJOB_MST;	//사용되지 않음

public:
	CErrorMst* m_pErrorMst;	

public:
	CMap<CString, LPCTSTR, CEquipment*, CEquipment*> m_MapEqps;
	CTrackInfo* GetTrackInfoNew(CString strTrackNo);
	CTrackInfo* GetTrackInfoNew(int nTrackNo);
	CSC_DATA* GetSC_DATA(CString strSC_NO);
	CSC_DATA* GetSC_DATA(int nSC_NO);
	CRTV_DATA* GetRTV_DATA(CString strRTV_NO);
	CRTV_DATA* GetRTV_DATA(int nRTV_NO);
	CBCR_MST* GetBCR_MST(CString strBCR_NO);
	CBCR_MST* GetBCR_MST(int nBCR_NO);
	CWC_DATA* GetWC_DATA(CString strWC_MC_NO);
	

public:
	CEquipmentArray		m_pEquipments;

public:
	CConfig* m_pConfig;
	CLang* m_pLang;
	CString m_strId;//login된 id
	CString m_strSound;
	EN_LANG m_enLang;
	CMap<CString, LPCTSTR, CPermission*, CPermission*> m_pUserInfo;
	//폼명				권한
	CMap<CString, LPCTSTR, CMsgBoxLang*, CMsgBoxLang*> m_pMsgBoxLang;

public:
	bool m_bExit;

public:
	HWND			m_hWndView;
	// [LGLS 2026-08-05 재적용] 재고정보(ViewRackDlg) 대화상자는 삭제. HWND 는 다른 파일들이 참조해 유지.
	HWND            m_hWndViewRackDlg;
	CTrackInfoArray		m_pTrackInfos;

public:
	CSystemLoginDlg* m_pLoginDlg;
	CViewJobListDlg* m_pViewJobListDlg;
	CViewHostEmptyPltDlg* m_pViewHostEmptyPltDlg;
	CViewUsageRackDlg* m_pViewUsageRackDlg;
	CEqpSuspendDlg* m_pEqpSuspendDlg;

	CManualRtv* m_pManualRtv;
	CManualSc* m_pManualSc;
	CScManualRet* m_pScManualRet;
	CManualJob* m_pManualJob;
	CManualEmpty* m_pManualEmpty;
	CManualLogin* m_pManualLogin;
	class CSemiTestDlg* m_pSemiTest;	// [LGLS 2026-08-13] 반자동 TEST 창(리본 MANUAL 에서 오픈)

	CDialog*			m_pConfigStatus;

	CDialog*			m_pCvSkinDlg;
	// [LGLS 2026-08-22] 작업 체류 경고창 (CWarningDlg) - 서버의 TASK 로그 대신 운전자에게 알린다
	CDialog*			m_pWarningDlg;
	afx_msg void OnCommandAlarmShow();   // [LGLS 2026-08-22] 리본 [알람] 버튼
	CDialog*			m_pRevSkinDlg;
	CDialog*			m_pRollSkinDlg;
	CDialog*			m_pScSkinDlg;
	CDialog*			m_pRtvSkinDlg;
	CDialog*			m_pBcrSkinDlg;
	CDialog*			m_pWcSkinDlg;
	
	CDialog*			m_pLogBcrSkinDlg;
	CDialog*			m_pLogIoSkinDlg;
	CDialog*		    m_pLogEqpSkinDlg;
	CDialog*		    m_pLogClientSkinDlg;
	CDialog*		    m_pConfigLogDelete;
	CDialog*		    m_pLogMesSkinDlg;
	CDialog*			m_pLogWcsSkinDlg;
	CDialog*			m_pLogEqpErrHis;
	CDialog*			m_pLogJobHis;	
	CDialog*			m_pViewSearchDlg;
	CDialog* m_pUserUserDlg;


public:
	CDialog* m_pFireMessageDlg;
	int m_nFireCollect;
	CFireParms* CreateFireParm(CRecordSetWrap* pRsw);
	CString GetQrySelectFIRE();
// 작업입니다.

public:
	BOOL GetDBObject( CAdoDB** pDB, CURMDBAccess** pDbAccess );
	BOOL InitializeDB();
	BOOL IsConnectDB(CURMDBAccess* pDbAccess);
	BOOL IsConnectDB();
	BOOL IsConnectDB_DLG();


public:
	BOOL IsPermissionDlg(CString pStr);

public:
	void InitilizeUserInfo(CString pStrId, int &pnRowCnt);
	BOOL FreeUserInfo();
	BOOL IsLogin();

private:
	BOOL Initialize();
	void InitializeLang();

public:
	CString GetDefineXmlPATH();

public:
	void AddWindowFontRegistry();
	void UpdateRibbonLang();


public:
	CEcsLayout* GetSelectedLayout();
	CEcsLayout* GetLayout_PARM(EN_LAYOUT penLAYOUT);
	CDciControl* GetDciControl_FindAllLayout(CString& strCID);
	CDciControl* GetDciControl_FindAllLayout(CString& strCID, int& nLayoutNo);
	void RefreshLayout();
	void ReloadLayout();
	void SetMsgLangDef();
	CString GetMsgLangDef(CString pKey);
	void InvokeControl();
	void InvokeCenterEquipCtrl(CString strEQPID, int nSTATE);

public:
	static CCriticalSectionEx m_csLockSyncThread;
	void EnterBlcokingSection();
	void LeaveBlcokingSection();



public:
	CDciControl*	GetDciControl(CString& strCID) ;
	CEquipment*		GetEquipment(int nEquipKind, int nEquipNum) { return m_pEquipments.GetEquipment(nEquipKind, nEquipNum); }
	CEquipment*		GetEquipmentSC(int nEquipKind, int nEquipNum) { return m_pEquipments.GetEquipmentSC(nEquipKind, nEquipNum); }
	CEquipment*		GetEquipmentRTV(int nEquipKind, int nEquipNum) { return m_pEquipments.GetEquipmentRTV(nEquipKind, nEquipNum); }
	CTrackInfo*		GetTrackInfo(int nTrackNum) { return m_pTrackInfos.GetTrackInfo(nTrackNum); }
	
public:
	void GetViewHandle();
	CView* GetViewObject();

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CEcsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	//신규추가

	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS	

public:
	BOOL IsAlliveCollectDB();
	void AlliveCollectDB();
	BOOL IsAlliveCollectRequest();
	void AlliveCollectRequest();
	void UpdateJOB_MST();
	void InitializeErrorMst();
	int GetFireMessage();

	long BeginTrans();
	long BeginTrans_DLG();
	long RollbackTrans();
	long RollbackTrans_DLG();
	long CommitTrans();
	long CommitTrans_DLG();

public:
	int ExcuteQueryString( CString pstrSql );	
	int ExcuteQueryString_DLG( CString pstrSql );

public:
	BOOL Permission(CString pWID_ID, int pEN_PERM);
	// [LGLS 2026-08-22] 구 EcsSv CEcsDoc::WriteLog 의 7번째 파라미터(BOOL bAlarm) 복원.
	//   파일 로그가 DB 로 옮겨오면서 사라졌던 "이 로그는 알람으로도 띄운다" 표시다.
	//   TRUE 면 WCS_CLIENT_LOG.ALARM_YN='Y' 로 남기고 경고창(CWarningDlg)에 즉시 띄운다.
	BOOL GetQueryInsertClientLog(CString pWIN_ID, CString pLUGG_NO, CString pBOTTOM_TRAY, CString pTOP_TRAY, CString pMESSAGE, BOOL bAlarm = FALSE);
	BOOL IsFireDlg();
	void CreateFireMessageDlg();

public:
	void ConfigDbLang();
	CString SYSDATE;
	CString NVL;
	virtual CString TO_NUMBER(CString pVALUE);
	void OnCreateScManualRet(CString pScNo);
	BOOL EquipStatusCheck();

	CConnectStatus* m_pConnectStatus;
	BOOL m_blConnectStatus;
	BOOL m_blJobList;//작업정보 관련
	BOOL m_blJobListDb;//작업정보 관련
	// [LGLS 2026-08-22] 작업정보(JOB_MST)에 실재하는 작업번호 집합 - 2초 캐시.
	//   창고 모니터링에서 컨베이어 위 작업번호 글자색을 정할 때 쓴다(있으면 검정 / 없으면 흰색).
	DWORD m_dwAliveJobTick;
	CMapStringToString m_mapAliveJob;
	CMapStringToString m_mapVehJob;		// 호기(901~905/801) -> 진행 중 작업번호
public:
	void RefreshJobCache();	// [LGLS 2026-08-23] 작업정보 2초 캐시 갱신
	BOOL IsJobInJobMst(LPCTSTR lpszLugg);
	CString GetVehicleJobNo(LPCTSTR lpszVehNo);
	CString GetVehicleJobTyp(LPCTSTR lpszVehNo);	// [LGLS 2026-08-23] 그 호기가 물고 있는 작업의 구분(1 입고 / 2 출고)	// [LGLS 2026-08-22] 그 호기에 물려 있는 진행 중 작업번호
public:
	int m_nTrackTextMode;				// [LGLS 2026-07-20 재적용] 0=작업번호 1=트랙번호 2=제품정보
	CCriticalSection m_csEqpData;		// [LGLS 재적용] 설비 데이터 CString 멀티스레드 경합 방지 락
	BOOL m_blManualLogin;//매뉴얼 로그인 관련
};
