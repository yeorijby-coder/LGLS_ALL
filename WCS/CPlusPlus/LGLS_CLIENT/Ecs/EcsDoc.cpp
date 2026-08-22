
// EcsDoc.cpp : CEcsDoc 클래스의 구현
//

#include "stdafx.h"
#include <propkey.h>
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Ecs.h"
#endif

#include "MainFrm.h"
#include "EcsDoc.h"
#include "WarningDlg.h"
#include "EcsView.h"
#include "EcsDefine.h"

#include "SystemLoginDlg.h"
#include "SystemConfigDlg.h"
#include "UserCreateUserDlg.h"
#include "SystemScTimeDlg.h"
#include "UserGroupDlg.h"
#include "UserCreateGroupDlg.h"
#include "ViewJobListDlg.h"
#include "ViewHostEmptyPltDlg.h"
#include "RecordSetWrap.h"
#include "SemiTestDlg.h"	// [LGLS 2026-08-13] 반자동 TEST
#include "EqpSuspendDlg.h"
#include "ViewUsageRackDlg.h"
#include "LogIoSkinDlg.h"
#include "LogEqpSkinDlg.h"
#include "LogMesSkinDlg.h"
#include "LogClientSkinDlg.h"
#include "LogBcrSkinDlg.h"
#include "LogEqpErrHisSkinDlg.h"
#include "LogEqpJobHisSkinDlg.h"
#include "LogWcsLogPgr.h"
#include "UserUserDlg.h"
#include "FireMessageDlg.h"
#include "ConfigStatus.h"
#include "ConfigLogDelete.h"



#include "EQP_ECD_MST.h"


#define DEF_LOGIN_SKIP

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CCriticalSectionEx CEcsDoc::m_csLockSyncThread = CCriticalSectionEx();

int g_nTabCount;

int g_nPlcCount;


// CEcsDoc
//
IMPLEMENT_DYNCREATE(CEcsDoc, CDocument)

BEGIN_MESSAGE_MAP(CEcsDoc, CDocument)
	//신규추가
	//ON_COMMAND(ID_VIEW_RACK, &CEcsDoc::OnMenuClick_VIEW_RACK)
// 	ON_COMMAND(ID_USER_USER, &CEcsDoc::OnMenuClick_USER_USER)
// 	ON_COMMAND(ID_USER_CREATEUSER, &CEcsDoc::OnMenuClick_USER_CREATEUSER)
// 	ON_COMMAND(ID_USER_GROUP, &CEcsDoc::OnMenuClick_USER_GROUP)
// 	ON_COMMAND(ID_USER_CREATEGROUP, &CEcsDoc::OnMenuClick_USER_CREATEGROUP)
	//--신규추가
	
	
	//ON_COMMAND_RANGE(ID_SYSTEM_CONFIG, ID_SYSTEM_EQP_SUSPEND, &CEcsDoc::OnCommandRangeMainFrameCONFIGURATION)
	ON_COMMAND_RANGE(ID_SYSTEM_CONFIG, IDD_CONFIG_LOG_DELETE, &CEcsDoc::OnCommandRangeMainFrameCONFIGURATION)
	ON_COMMAND_RANGE(ID_SYSTEM_CONFIG, IDD_CONFIG_STATUS, &CEcsDoc::OnCommandRangeMainFrameCONFIGURATION)
	ON_COMMAND_RANGE(ID_VIEW_JOBLIST, ID_VIEW_SEARCH, &CEcsDoc::OnCommandRangeMainFrameVIEW)
	ON_COMMAND_RANGE(ID_MONITORING_1F, ID_MONITORING_ASSEMBLE, &CEcsDoc::OnCommandRangeMainFrameMONITORING)
	ON_COMMAND_RANGE(ID_LANGUAGE_KOR, ID_LANGUAGE_HUNGARIAN, &CEcsDoc::OnCommandRangeMainFrameNATION)
	ON_COMMAND_RANGE(ID_MANUAL_JOB, IDD_MANUAL_EMPTY, &CEcsDoc::OnCommandRangeMainFrameMANUAL)
	ON_COMMAND_RANGE(ID_LOG_IO, IDD_LOG_WCS_LOG_PGR, &CEcsDoc::OnCommandRangeMainFrameLOG)
	//ON_COMMAND_RANGE(ID_LOG_IO, ID_LOG_CLIENT, &CEcsDoc::OnCommandRangeMainFrameLOG)
	ON_COMMAND_RANGE(ID_USER_USER, ID_USER_GROUP, &CEcsDoc::OnCommandRangeMainFrameUSER)
	ON_COMMAND_RANGE(ID_STATUS_CV, ID_STATUS_WC1, &CEcsDoc::OnCommandRangeMainFrameSTATUS)
	ON_COMMAND_RANGE(ID_MONITORING_VIEW_JOBNO, ID_MONITORING_VIEW_PRODINFO, &CEcsDoc::OnCommandTrackTextMode)	// [LGLS 2026-07-20 재적용]
	ON_COMMAND(ID_ALARM_SHOW, &CEcsDoc::OnCommandAlarmShow)	// [LGLS 2026-08-22]
	ON_COMMAND(ID_MANUAL_SEMITEST, &CEcsDoc::OnCommandSemiTestOpen)	// [LGLS 2026-08-13]
	ON_COMMAND(ID_MANUAL_TESTCLEAR, &CEcsDoc::OnCommandSemiTestClear)	// [LGLS 2026-08-13]
END_MESSAGE_MAP()


// CEcsDoc 생성/소멸
//
CString g_strEcsPath;

CEcsDoc::CEcsDoc()
{
	m_bViewFirstLoad = FALSE;
	m_nTrackTextMode = 0;	// [LGLS 2026-07-20 재적용] 트랙 표시모드 기본=작업번호
	m_hWndViewRackDlg = NULL;
	m_bExit = false;

	TCHAR szPath[MAX_PATH];
	::GetCurrentDirectory(sizeof(szPath), szPath);
	g_strEcsPath = CString(szPath) + _T("\\");
	m_pCollectDB = NULL;
	m_pCollectRequest = NULL;
	m_CollectCellInfo.m_bRequest = FALSE;
	m_CollectCellInfo.m_strAGING_TYP = _T("0");
	m_CollectCellInfo.m_strBANK = _T("0");
	m_CollectCellInfo.m_strWH_TYP = _T("0");
	m_CollectJOB_MST.m_bRequest = FALSE;
	m_CollectJOB_MST.m_strWH_TYP = _T("10");
	m_pJob = new CJobCollection(this);


	//cconfig 생성
	m_pConfig = new CConfig(this);
	DEBUGER_ASSERT_VALID(m_pConfig != NULL);

	m_WH_TYP = m_pConfig->m_strWH_WH_TYP;
	m_CollectJOB_MST.m_strWH_TYP = m_WH_TYP;
	m_enSelectedLayout = (EN_LAYOUT)m_pConfig->m_nUSER_LAST_TAB_INDEX;
	if(m_enSelectedLayout < 0 || m_enSelectedLayout > EN_CHIN_LAST)
	{
		m_enSelectedLayout = EN_LFC_G;
	}
	m_pLang = NULL;
	m_strId = _T("");
	m_enLang = (EN_LANG)m_pConfig->m_nUSER_LAST_LANG;

	m_pUrmDBAccess = NULL;
	m_pDlgUrmDBAccess = NULL;

	m_pLoginDlg = NULL;
	m_pViewJobListDlg = NULL;
	m_pViewHostEmptyPltDlg = NULL;
	m_pViewUsageRackDlg = NULL;
	m_pEqpSuspendDlg = NULL;

	m_pManualRtv = NULL;
	m_pManualSc = NULL;
	m_pScManualRet = NULL;
	m_pManualJob = NULL;
	m_pManualEmpty = NULL;
	m_pManualLogin = NULL;
	m_pSemiTest = NULL;	// [LGLS 2026-08-13]

	m_pConfigStatus = NULL;

	m_pCvSkinDlg = NULL;
	m_pWarningDlg = NULL;
	m_pRevSkinDlg = NULL;
	m_pRollSkinDlg = NULL;
	m_pScSkinDlg = NULL;
	m_pRtvSkinDlg = NULL;
	m_pBcrSkinDlg = NULL;
	m_pWcSkinDlg = NULL;

	m_pLogIoSkinDlg = NULL;
	m_pLogEqpSkinDlg = NULL;
	m_pLogClientSkinDlg = NULL;
	m_pConfigLogDelete = NULL;
	m_pLogMesSkinDlg = NULL;
	m_pLogWcsSkinDlg = NULL;
	m_pLogBcrSkinDlg = NULL;
	m_pLogEqpErrHis=NULL;
	m_pLogJobHis=NULL;

	m_pErrorMst = NULL;
	m_pFireMessageDlg = NULL;
	m_pViewSearchDlg = NULL;
	
	m_pUserUserDlg = NULL;

	m_blConnectStatus = FALSE;
	m_blJobList = FALSE;
	m_blJobListDb = FALSE;
	//m_nFireCollect = TRUE;
	m_blManualLogin = FALSE;
	
	m_bDebugMode = FALSE;
	m_bDebugStart = FALSE;
	m_strBypassChkYn = _T("N");

	m_bMoveFlag = FALSE; //1


	//DB LANG 설정
	ConfigDbLang();
}

//DB에 따라서 변수 설정
void CEcsDoc::ConfigDbLang()
{
#if ORACLE
	SYSDATE = _T("SYSDATE");
	NVL = _T("NVL");
#elif  POSTGRESQL
	SYSDATE = _T("NOW()");
	NVL = _T("COALESCE");
#elif MSSQL
	SYSDATE = _T("GETDATE()");	// [LGLS]
	NVL = _T("COALESCE");
#endif
}

CString CEcsDoc::TO_NUMBER(CString pVALUE)
{
	CString RtnValue;

#if ORACLE
	RtnValue = _T("TO_NUMBER(") + pVALUE + _T(")");
#elif POSTGRESQL
	RtnValue = pVALUE + _T("::integer");
#elif MSSQL
	RtnValue = _T("CAST(") + pVALUE + _T(" AS INT)");	// [LGLS]
#endif

	return RtnValue;
}

CEcsDoc::~CEcsDoc()
{
	if(m_pCollectDB != NULL){ delete m_pCollectDB;}
	if(m_pCollectRequest != NULL){ delete m_pCollectRequest;}
	if(m_pLang != NULL){ delete m_pLang;}
	if(m_pUrmDBAccess != NULL){ delete m_pUrmDBAccess;}
	if(m_pDlgUrmDBAccess != NULL){ delete m_pDlgUrmDBAccess;}
	if(m_pViewJobListDlg != NULL){ delete m_pViewJobListDlg;}
	if(m_pViewHostEmptyPltDlg != NULL){ delete m_pViewHostEmptyPltDlg;}
	if(m_pCvSkinDlg != NULL){ delete m_pCvSkinDlg;}
	if(m_pRevSkinDlg != NULL){ delete m_pRevSkinDlg;}
	if(m_pRollSkinDlg != NULL){ delete m_pRollSkinDlg;}
	if(m_pScSkinDlg != NULL){ delete m_pScSkinDlg;}
	if(m_pRtvSkinDlg != NULL){ delete m_pRtvSkinDlg;}
	if(m_pBcrSkinDlg != NULL){ delete m_pBcrSkinDlg;}
	if(m_pWcSkinDlg != NULL){ delete m_pWcSkinDlg;}
	if(m_pViewUsageRackDlg != NULL) { delete m_pViewUsageRackDlg;}
	if(m_pManualRtv != NULL) { delete m_pManualRtv;}
	if(m_pManualSc != NULL) { delete m_pManualSc;} 
	if(m_pScManualRet != NULL) { delete m_pScManualRet;} 
	if(m_pManualJob != NULL) { delete m_pManualJob;}
	if(m_pSemiTest != NULL){ if(::IsWindow(m_pSemiTest->m_hWnd)) m_pSemiTest->DestroyWindow(); delete m_pSemiTest; m_pSemiTest = NULL; }	// [LGLS 2026-08-13]
	if(m_pConfigStatus != NULL) { delete m_pConfigStatus;}
	if(m_pManualEmpty != NULL) { delete m_pManualEmpty;}
	if(m_pManualLogin != NULL) { delete m_pManualLogin;}
	if(m_pLogIoSkinDlg != NULL) { delete m_pLogIoSkinDlg;}
	if(m_pLogBcrSkinDlg != NULL) { delete m_pLogBcrSkinDlg;}
	if(m_pLogEqpSkinDlg != NULL) { delete m_pLogEqpSkinDlg;}
	if(m_pLogClientSkinDlg != NULL) { delete m_pLogClientSkinDlg;}
	if(m_pConfigLogDelete != NULL) { delete m_pConfigLogDelete;} 
	if(m_pLogMesSkinDlg != NULL) { delete m_pLogMesSkinDlg;}
	if(m_pLogWcsSkinDlg != NULL) { delete m_pLogWcsSkinDlg;}
	if(m_pErrorMst != NULL) {delete m_pErrorMst;};
	if(m_pLogEqpErrHis != NULL) {delete m_pLogEqpErrHis;};
	if(m_pLogJobHis != NULL) {delete m_pLogJobHis;};
	if(m_pFireMessageDlg != NULL) {delete m_pFireMessageDlg;};
	if(m_pViewSearchDlg != NULL) {delete m_pViewSearchDlg;};
	if(m_pUserUserDlg != NULL) {delete m_pUserUserDlg;};
	if(m_pEqpSuspendDlg != NULL) {delete m_pEqpSuspendDlg;};
	if(m_pConfig != NULL) {delete m_pConfig;};
	if(m_pJob != NULL) {delete m_pJob;};
	if(m_pLoginDlg != NULL) {delete m_pLoginDlg;};

	CTrackInfo* pTrack = NULL;
	for (int i= 0; i<m_pTrackInfos.GetSize(); ++i)
	{
		//@@@@
		pTrack = m_pTrackInfos[i];
		if(pTrack != NULL && pTrack->m_pCV_DATA != NULL)
		{
			delete pTrack->m_pCV_DATA;
		}
		if(pTrack != NULL)
		{
			delete pTrack;
			pTrack = NULL;
		}
	}
	m_pTrackInfos.RemoveAll();

	int i =0; 
	int nEqiupCnt = m_pEquipments.GetSize();
	for(i = 0; i<m_pEquipments.GetSize(); ++i)
		delete m_pEquipments[i];

	for (i = 0; i<m_pEcsLayOuts.GetSize(); ++i)
		delete m_pEcsLayOuts[i];
	m_pEcsLayOuts.RemoveAll();
	
	//SK:) 죽
	m_pEquipments.RemoveAll();

}

void CEcsDoc::OnCommandRangeMainFrameUSER(UINT nID)
{
	switch(nID)
	{
	case ID_USER_USER:
		{
			if (!Permission(_T("CUserUserDlg"), SEL_YN))
			{
				AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
				return;
			}

			if (m_pUserUserDlg == NULL)
			{
				m_pUserUserDlg = new CUserUserDlg(this);
				this->m_pUserUserDlg->Create(IDD_USER_USER);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pUserUserDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pUserUserDlg->SetWindowPos(&m_pUserUserDlg->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

			}
			::SetWindowPos(m_pUserUserDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pUserUserDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pUserUserDlg->m_hWnd, SW_SHOWNORMAL);

			break;
		}
	}
}

// [LGLS 2026-08-22] 리본 [알람] 버튼 - 작업 체류 경고창을 강제로 띄운다.
//   창은 CEcsView::OnInitialUpdate 에서 미리 만들어 숨겨 두므로 여기서는 표시만 한다.
void CEcsDoc::OnCommandAlarmShow()
{
	if (m_pWarningDlg == NULL || !::IsWindow(m_pWarningDlg->GetSafeHwnd()))
	{
		AfxMessageBox(GetMsgLangDef(_T("알람 창이 준비되지 않았습니다")));
		return;
	}
	m_pWarningDlg->ShowWindow(SW_SHOW);
	m_pWarningDlg->SetForegroundWindow();
}

void CEcsDoc::OnCommandRangeMainFrameLOG(UINT nID)
{
	switch(nID)
	{
		case ID_LOG_IO:
			{
				if (!Permission(_T("CLogIoSkinDlg"), SEL_YN))
				{
					AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
					return;
				}

				if (m_pLogIoSkinDlg == NULL)
				{
					m_pLogIoSkinDlg = new CLogIoSkinDlg(this);
					this->m_pLogIoSkinDlg->Create(IDD_LOG_IO);
					CRect MainRect;
					CRect Rect;
					CRect PosRect;
					::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
					this->m_pLogIoSkinDlg->GetWindowRect(&Rect); 

					PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
					PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
					this->m_pLogIoSkinDlg->SetWindowPos(&m_pLogIoSkinDlg->wndTop, PosRect.left, PosRect.top, 
						Rect.Width(), Rect.Height(), 
						SWP_SHOWWINDOW);

				}
				::SetWindowPos(m_pLogIoSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_pLogIoSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				ShowWindow(m_pLogIoSkinDlg->m_hWnd, SW_SHOWNORMAL);
				break;
			}
		case ID_LOG_EQP_HIS:
			{
				if (!Permission(_T("CLogEqpErrHisSkinDlg"), SEL_YN))
				{
					AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
					return;
				}

				if (m_pLogEqpSkinDlg == NULL)
				{
					m_pLogEqpSkinDlg = new CLogEqpErrHisSkinDlg(this);
					this->m_pLogEqpSkinDlg->Create(IDD_LOG_EQP_ERR_HIS);
					CRect MainRect;
					CRect Rect;
					CRect PosRect;
					::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
					this->m_pLogEqpSkinDlg->GetWindowRect(&Rect); 

					PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
					PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
					this->m_pLogEqpSkinDlg->SetWindowPos(&m_pLogEqpSkinDlg->wndTop, PosRect.left, PosRect.top, 
						Rect.Width(), Rect.Height(), 
						SWP_SHOWWINDOW);

				}
				::SetWindowPos(m_pLogEqpSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_pLogEqpSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				ShowWindow(m_pLogEqpSkinDlg->m_hWnd, SW_SHOWNORMAL);
				break;
			}
		case ID_LOG_CLIENT:
			{
				if (!Permission(_T("CLogClientSkinDlg"), SEL_YN))
				{
					AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
					return;
				}

				if (m_pLogClientSkinDlg == NULL)
				{
					m_pLogClientSkinDlg = new CLogClientSkinDlg(this);
					this->m_pLogClientSkinDlg->Create(IDD_LOG_CLIENT);
					CRect MainRect;
					CRect Rect;
					CRect PosRect;
					::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
					this->m_pLogClientSkinDlg->GetWindowRect(&Rect); 

					PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
					PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
					this->m_pLogClientSkinDlg->SetWindowPos(&m_pLogClientSkinDlg->wndTop, PosRect.left, PosRect.top, 
						Rect.Width(), Rect.Height(), 
						SWP_SHOWWINDOW);

				}
				::SetWindowPos(m_pLogClientSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_pLogClientSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				ShowWindow(m_pLogClientSkinDlg->m_hWnd, SW_SHOWNORMAL);
				break;
			}
		case ID_LOG_MES:
			{
				if (!Permission(_T("CLogMesSkinDlg"), SEL_YN))
				{
					AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
					return;
				}

				if (m_pLogMesSkinDlg == NULL)
				{
					m_pLogMesSkinDlg = new CLogMesSkinDlg(this);
					this->m_pLogMesSkinDlg->Create(IDD_LOG_MES);
					CRect MainRect;
					CRect Rect;
					CRect PosRect;
					::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
					this->m_pLogMesSkinDlg->GetWindowRect(&Rect); 

					PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
					PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
					this->m_pLogMesSkinDlg->SetWindowPos(&m_pLogMesSkinDlg->wndTop, PosRect.left, PosRect.top, 
						Rect.Width(), Rect.Height(), 
						SWP_SHOWWINDOW);

				}
				::SetWindowPos(m_pLogMesSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_pLogMesSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				ShowWindow(m_pLogMesSkinDlg->m_hWnd, SW_SHOWNORMAL);
				break;
			}
		case ID_LOG_BCR:
			{
				if (!Permission(_T("CLogBcrSkinDlg"), SEL_YN))
				{
					AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
					return;
				}

				if (m_pLogBcrSkinDlg == NULL)
				{
					m_pLogBcrSkinDlg = new CLogBcrSkinDlg(this);
					this->m_pLogBcrSkinDlg->Create(IDD_LOG_BCR);
					CRect MainRect;
					CRect Rect;
					CRect PosRect;
					::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
					this->m_pLogBcrSkinDlg->GetWindowRect(&Rect); 

					PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
					PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
					this->m_pLogBcrSkinDlg->SetWindowPos(&m_pLogBcrSkinDlg->wndTop, PosRect.left, PosRect.top, 
						Rect.Width(), Rect.Height(), 
						SWP_SHOWWINDOW);

				}
				::SetWindowPos(m_pLogBcrSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_pLogBcrSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				ShowWindow(m_pLogBcrSkinDlg->m_hWnd, SW_SHOWNORMAL);
				break;
			}
		case ID_LOG_JOB_HIS:
			{
				if (!Permission(_T("CLogEqpJobHisSkinDlg"), SEL_YN))
				{
					AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
					return;
				}

				if (m_pLogJobHis == NULL)
				{
					m_pLogJobHis = new CLogEqpJobHisSkinDlg(this);
					this->m_pLogJobHis->Create(IDD_LOG_EQP_JOB_HIS);
					CRect MainRect;
					CRect Rect;
					CRect PosRect;
					::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
					this->m_pLogJobHis->GetWindowRect(&Rect); 

					PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
					PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
					this->m_pLogJobHis->SetWindowPos(&m_pLogJobHis->wndTop, PosRect.left, PosRect.top, 
						Rect.Width(), Rect.Height(), 
						SWP_SHOWWINDOW);

				}
				::SetWindowPos(m_pLogJobHis->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_pLogJobHis->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				ShowWindow(m_pLogJobHis->m_hWnd, SW_SHOWNORMAL);
				break;
			}
			case IDD_LOG_WCS_LOG_PGR:
			{

				if (m_pLogWcsSkinDlg == NULL)
				{
					m_pLogWcsSkinDlg = new CLogWcsLogPgr(this);
					this->m_pLogWcsSkinDlg->Create(IDD_LOG_WCS_LOG_PGR);
					CRect MainRect;
					CRect Rect;
					CRect PosRect;
					::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
					this->m_pLogWcsSkinDlg->GetWindowRect(&Rect); 

					PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
					PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
					this->m_pLogWcsSkinDlg->SetWindowPos(&m_pLogWcsSkinDlg->wndTop, PosRect.left, PosRect.top, 
						Rect.Width(), Rect.Height(), 
						SWP_SHOWWINDOW);

				}
				::SetWindowPos(m_pLogWcsSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_pLogWcsSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
				ShowWindow(m_pLogWcsSkinDlg->m_hWnd, SW_SHOWNORMAL);
				break;
			}
	}
}
	

void CEcsDoc::OnCommandRangeMainFrameMANUAL(UINT nID)
{
	switch(nID)
	{
	case ID_MANUAL_JOB:
		{
			//if (!Permission(_T("CManualJob"), INS_YN))
			//{
			//	AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
			//	return;
			//}

			if(m_pManualJob == NULL)
			{
				// [LGLS 재적용] 반자동 작업 비밀번호(ManualLogin) 제거 - 바로 ManualJob 오픈
				m_blManualLogin = TRUE;
				m_pManualJob = new CManualJob(this, m_pDlgUrmDBAccess);
				this->m_pManualJob->Create(IDD_MANAUL_JOB);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);
				this->m_pManualJob->GetWindowRect(&Rect);
				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2;
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2;
				this->m_pManualJob->SetWindowPos(&m_pManualJob->wndTop, PosRect.left, PosRect.top,
					Rect.Width(), Rect.Height(),
					SWP_SHOWWINDOW);
			}
			::SetWindowPos(m_pManualJob->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pManualJob->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pManualJob->m_hWnd, SW_SHOWNORMAL);

			/*if (m_pManualJob == NULL)
			{
				m_pManualJob = new CManualJob(this, m_pDlgUrmDBAccess);
				this->m_pManualJob->Create(IDD_MANAUL_JOB);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pManualJob->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pManualJob->SetWindowPos(&m_pManualJob->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

			}
			::SetWindowPos(m_pManualJob->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pManualJob->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pManualJob->m_hWnd, SW_SHOWNORMAL);*/
			break;
			

		}
	case ID_MANUAL_RTV:
		{
			if (false && !Permission(_T("CManualRtv"), INS_YN)) // [LGLS 재적용] 반자동 RTV 권한체크 해제(JOB/SC와 일관)
			{
				AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
				return;
			}

			if (m_pManualRtv == NULL)
			{
				m_pManualRtv = new CManualRtv(this, m_pDlgUrmDBAccess);
				this->m_pManualRtv->Create(IDD_MANAUL_RTV);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pManualRtv->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pManualRtv->SetWindowPos(&m_pManualRtv->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

			}
			::SetWindowPos(m_pManualRtv->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pManualRtv->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pManualRtv->m_hWnd, SW_SHOWNORMAL);
			break;
		}
	case ID_MANUAL_SC:
		{
			//if (!Permission(_T("CManualSc"), INS_YN))
			//{
			//	AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
			//	return;
			//}

			if (m_pManualSc == NULL)
			{
				m_pManualSc = new CManualSc(this, m_pDlgUrmDBAccess);
				this->m_pManualSc->Create(IDD_MANAUL_SC);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pManualSc->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pManualSc->SetWindowPos(&m_pManualSc->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

			}
			::SetWindowPos(m_pManualSc->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pManualSc->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pManualSc->m_hWnd, SW_SHOWNORMAL);
			break;
		}
	case IDD_MANUAL_EMPTY:
		{

			//if (!Permission(_T("CManualEmpty"), INS_YN))
			//{
			//	AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
			//	return;
			//}

			if (m_pManualEmpty == NULL)
			{
				m_pManualEmpty = new CManualEmpty(this, m_pDlgUrmDBAccess);
				this->m_pManualEmpty->Create(IDD_MANUAL_EMPTY);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pManualEmpty->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pManualEmpty->SetWindowPos(&m_pManualEmpty->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

			}
			::SetWindowPos(m_pManualEmpty->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pManualEmpty->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pManualEmpty->m_hWnd, SW_SHOWNORMAL);
			break;
		}
	}
}


void CEcsDoc::OnCommandRangeMainFrameVIEW(UINT nID)
{
	switch(nID)
	{
	case ID_VIEW_JOBLIST:
		{
			if (!Permission(_T("CViewJobListDlg"), SEL_YN))
			{
				AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
				return;
			}

			if (m_pViewJobListDlg == NULL)
			{
				m_pViewJobListDlg = new CViewJobListDlg(this);
				this->m_pViewJobListDlg->Create(IDD_VIEW_JOBLIST1);
			
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pViewJobListDlg->GetWindowRect(&Rect); 

				double dWidth = Rect.Width() * m_pMaxSizeX;
				double dHeight = Rect.Height() * m_pMaxSizeX;


				//PosRect.left = ((MainRect.right  - MainRect.left) - dWidth)  / 2; 
				//PosRect.top  = ((MainRect.bottom - MainRect.top)  - dHeight) / 2; 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pViewJobListDlg->SetWindowPos(&m_pViewJobListDlg->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);
				
			}
			::SetWindowPos(m_pViewJobListDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pViewJobListDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pViewJobListDlg->m_hWnd, SW_SHOWNORMAL);

			break;
		}
	case ID_VIEW_HOST_EMPTY_PLT:
		{
			if (m_pViewHostEmptyPltDlg == NULL)
			{
				m_pViewHostEmptyPltDlg = new CViewHostEmptyPltDlg(this);
				this->m_pViewHostEmptyPltDlg->Create(IDD_VIEW_HOST_EMPTY_PLT);
			
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pViewHostEmptyPltDlg->GetWindowRect(&Rect); 

				double dWidth = Rect.Width() * m_pMaxSizeX;
				double dHeight = Rect.Height() * m_pMaxSizeX;

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pViewHostEmptyPltDlg->SetWindowPos(&m_pViewHostEmptyPltDlg->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);
				
			}
			::SetWindowPos(m_pViewHostEmptyPltDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pViewHostEmptyPltDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pViewHostEmptyPltDlg->m_hWnd, SW_SHOWNORMAL);

			break;
		}
	case ID_VIEW_USAGE:
		{
			if (!Permission(_T("CViewUsageRackDlg"), SEL_YN))
			{
				AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
				return;
			}

			if (m_pViewUsageRackDlg == NULL)
			{
				m_pViewUsageRackDlg = new CViewUsageRackDlg(this);
				this->m_pViewUsageRackDlg->Create(IDD_VIEW_USAGE_RACK);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pViewUsageRackDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pViewUsageRackDlg->SetWindowPos(&m_pViewUsageRackDlg->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

			}
			::SetWindowPos(m_pViewUsageRackDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pViewUsageRackDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pViewUsageRackDlg->m_hWnd, SW_SHOWNORMAL);
			break;
		}
	case ID_VIEW_SEARCH:
		{
			if (!Permission(_T("CViewSearchDlg"), SEL_YN))
			{
				AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
				return;
			}

			if (m_pViewSearchDlg == NULL)
			{
				m_pViewSearchDlg = new CViewSearchDlg(this);
				this->m_pViewSearchDlg->Create(IDD_VIEW_SEARCH);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pViewSearchDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pViewSearchDlg->SetWindowPos(&m_pViewSearchDlg->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

			}
			::SetWindowPos(m_pViewSearchDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pViewSearchDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pViewSearchDlg->m_hWnd, SW_SHOWNORMAL);
			break;
		}
	}
}

void CEcsDoc::OnCommandRangeMainFrameSTATUS(UINT nID)
{
}

void CEcsDoc::OnCommandRangeMainFrameMONITORING(UINT nID)
{
	m_enSelectedLayout = (EN_LAYOUT)(nID - ID_MONITORING_NON);
	::PostMessage(m_hWndView, WM_USER_REFRESH_LAYOUT, (WPARAM)this, (LPARAM)m_enSelectedLayout);
	m_pConfig->m_nUSER_LAST_TAB_INDEX = m_enSelectedLayout;
	m_pConfig->SaveConfigMONITOR();
}

void CEcsDoc::OnCommandRangeMainFrameNATION(UINT nID)
{
	switch(nID)
	{
	case ID_LANGUAGE_KOR:
		{
			m_enLang = EN_KOR;
			break;
		}
	case ID_LANGUAGE_ENGLISH:
		{
			m_enLang = EN_ENG;
			break;
		}
	case ID_LANGUAGE_CHIN:
		{
			m_enLang = EN_CHIN;
			break;
		}
	case ID_LANGUAGE_HUNGARIAN:
		{
			m_enLang = EN_HUN;
			break;
		}    
	}
	//구현1
	//DLG 띄어잇는 곳에 REFRESH 메시지
	//
	SetMsgLangDef();
	UpdateRibbonLang();
	m_pConfig->m_nUSER_LAST_LANG = m_enLang;
	m_pConfig->SaveConfigLANG();
}

void CEcsDoc::OnCommandRangeMainFrameCONFIGURATION(UINT nID)
{
	switch(nID)
	{
	case ID_SYSTEM_CONFIG:
		{
			break;
		}
	case ID_SYSTEM_LOGIN:
		{
			m_pLoginDlg->InitializeUser();
			break;
		}
	case ID_SYSTEM_INVENTORY:
		{
			// [LGLS 2026-08-05 재적용] 재고정보(ViewRackDlg)는 삭제됨 - 신규 WCS 는 재고 관리 제외
			break;
		}
	case ID_SYSTEM_EQP_SUSPEND:
		{
			if (!Permission(_T("CEqpSuspend"), SEL_YN))
			{
				AfxMessageBox(GetMsgLangDef(_T("권한이 없습니다")));
				return;
			}

			if (m_pEqpSuspendDlg == NULL)
			{
				m_pEqpSuspendDlg = new CEqpSuspendDlg(this);
				this->m_pEqpSuspendDlg->Create(IDD_EQP_SUSPEND);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pEqpSuspendDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pEqpSuspendDlg->SetWindowPos(&m_pEqpSuspendDlg->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

			}
			::SetWindowPos(m_pEqpSuspendDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pEqpSuspendDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pEqpSuspendDlg->m_hWnd, SW_SHOWNORMAL);
			break;
		}    
	case IDD_CONFIG_LOG_DELETE:
		{

			if (m_pConfigLogDelete == NULL)
			{
				m_pConfigLogDelete = new CConfigLogDelete(this);
				this->m_pConfigLogDelete->Create(IDD_CONFIG_LOG_DELETE);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pConfigLogDelete->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pConfigLogDelete->SetWindowPos(&m_pConfigLogDelete->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

			}
			::SetWindowPos(m_pConfigLogDelete->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pConfigLogDelete->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pConfigLogDelete->m_hWnd, SW_SHOWNORMAL);
			break;
		}    
	case IDD_CONFIG_STATUS:
		{
			if (m_pConfigStatus == NULL)
			{
				m_pConfigStatus = new CConfigStatus(this);
				this->m_pConfigStatus->Create(IDD_CONFIG_STATUS);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				this->m_pConfigStatus->GetWindowRect(&Rect); 
			
				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				this->m_pConfigStatus->SetWindowPos(&m_pConfigStatus->wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);
			
			}
			::SetWindowPos(m_pConfigStatus->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_pConfigStatus->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(m_pConfigStatus->m_hWnd, SW_SHOWNORMAL);
			break;
		}
	}
}

BOOL CEcsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return Initialize();
}

BOOL CEcsDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	if (m_strId !=_T(""))
	{
		if (AfxMessageBox(GetMsgLangDef(_T("종료하시겠습니까?")), MB_OKCANCEL) == IDCANCEL)
			return FALSE;
	}
	for(int i=0; i< m_pEquipments.GetSize(); ++i)
	{
		::SetEvent(m_pEquipments[i]->m_hEventArray[CEquipment::enEventKill]);
	}


	m_bExit = true;
	Sleep(5000);
	
	return CDocument::CanCloseFrame(pFrame);
}

void CEcsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())	{	}
	else{	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CEcsDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CEcsDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CEcsDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CEcsDoc 진단
//
#ifdef _DEBUG
void CEcsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEcsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL CEcsDoc::InitializeDB() //생성자 초기화
{
	m_pUrmDBAccess = new CURMDBAccess(this, new CAdoDB(this));
	m_pDlgUrmDBAccess = new CURMDBAccess(this, new CAdoDB(this));
	if(IsConnectDB(m_pUrmDBAccess) == FALSE){ return FALSE; };
	if(IsConnectDB(m_pDlgUrmDBAccess) == FALSE){ return FALSE; };
	return TRUE;
}

BOOL CEcsDoc::IsConnectDB(CURMDBAccess* pDbAccess)
{
	if(pDbAccess == NULL){ return FALSE; };
	if(pDbAccess->m_pAdoDB == NULL){ return FALSE; };
	if(pDbAccess->m_pAdoDB->m_bConnected == FALSE)
	{ 
		if (m_blJobListDb == FALSE)
		{
			m_blJobListDb = pDbAccess->m_pAdoDB->ConnectDB();
		}
		return pDbAccess->m_pAdoDB->ConnectDB(); 
	};
	return pDbAccess->m_pAdoDB->m_bConnected;
}


BOOL CEcsDoc::IsConnectDB()
{
	return IsConnectDB(m_pUrmDBAccess);
}

BOOL CEcsDoc::IsConnectDB_DLG()
{
	return IsConnectDB(m_pDlgUrmDBAccess);
}



// CEcsDoc 명령
//
BOOL CEcsDoc::Initialize()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	GetViewHandle();
	while(InitializeDB() == FALSE)
	{
		if(AfxMessageBox(GetMsgLangDef(_T("DB 접속에 실패했습니다. 재시도 하시겠습니까?")), MB_OKCANCEL) == IDOK)
		{
			continue;
		}
		return FALSE;
	}

	//if(InitializeDB() == FALSE){	return FALSE;	}

	////수정 헝가리->중국
	//if(m_WH_TYP == _T("10"))
	//{
	//	if (!m_layout.LoadXml(ECS_LAYOUT_FILE, m_strKioskNo))	{return FALSE;}
	//	m_layout.m_enLayout = EN_LFC_G;
	//}

	int i = 0;
//	char szPath[_MAX_PATH] = {0};
	TCHAR szPath[_MAX_PATH] = {0};
	::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);

	g_strEcsPath = szPath;

	int nTabCount = ::GetPrivateProfileInt(_T("COMMON"), _T("TabCount"), 1, ECS_INI_FILE);
	g_nPlcCount = 0;
	g_nTabCount = nTabCount;

	for (i = 0 ; i < nTabCount ; i++)
	{
		CEcsLayout* pEcsLayout = new CEcsLayout();

		CString strLayOutFilePath;
		strLayOutFilePath.Format(_T(".\\EcsLayout%d.xml"), i + 1);

		if (!pEcsLayout->LoadXml(strLayOutFilePath, m_strKioskNo))
		{
			AfxMessageBox(pEcsLayout->m_strLog);
			continue;
		}

		m_pEcsLayOuts.Add(pEcsLayout);
	}

	
	CEcsDefine define(this);
	if (!define.ParseXml())
	{
		AfxMessageBox(define.GetErrorMessage());
		return FALSE;
	}

	//kdh
	Global.LoadBitmaps();
	Global.LoadIcons();
	Global.SetRGB(IDX_RGB_MASK, RGB(255,0,255));
	Global.SetRGB(IDX_RGB_BACKGROUND, RGB(255,255,255));
	Global.SetRGB(IDX_RGB_FONT, RGB(51,51,51));
	Global.SetRGB(IDX_RGB_FONT_CAPTION, RGB(0,0,0));
	Global.SetRGB(IDX_RGB_FONT_CAPTION_INACTIVE, RGB(178,178,178));
	Global.SetFont(IDX_FONT_SMALL, _T("Arial"), 8);
	Global.SetFont(IDX_FONT_MEDIUM, _T("Arial"), 9);
	Global.SetFont(IDX_FONT_LARGE, _T("Arial"), 11, TRUE);
	AddWindowFontRegistry();
	Global.InitializeIcon();
	
	
	m_pLoginDlg = new CSystemLoginDlg(this);
	while(m_pLoginDlg->InitializeUser() == FALSE)
	{
		if (AfxMessageBox(GetMsgLangDef(_T("종료하시겠습니까?")), MB_OKCANCEL) == IDOK)
			return FALSE;
	}

	m_pLang = new CLang();

	InitializeLang();
	InitializeErrorMst();
	m_nFireCollect = 1;
	m_bTrans = false;

	return TRUE;
}

CSC_DATA* CEcsDoc::GetSC_DATA(CString strSC_NO)
{
	CString strScKey;
	CString strValue;
	CEquipment* pEqp = NULL;
	for(POSITION pPosSc = m_MapEqps.GetStartPosition(); pPosSc != NULL; )
	{
		CScPair* pSc = NULL;
		m_MapEqps.GetNextAssoc(pPosSc, strScKey, pEqp);
		if(pEqp->m_enKind != CEquipment::enSC){continue;};
		pSc = (CScPair*)pEqp;	
		CSC_DATA* pSC_DATA = pSc->m_pInfo->m_MapSC_DATA[strSC_NO];
		if(pSC_DATA != NULL)
			return pSC_DATA;
	}
	return NULL;	
}

CSC_DATA* CEcsDoc::GetSC_DATA(int nSC_NO)
{	
	CString strSC_NO;
	strSC_NO.Format(_T("%05s"), nSC_NO);
	CString strScKey;
	CString strValue;
	CEquipment* pEqp = NULL;
	for(POSITION pPosSc = m_MapEqps.GetStartPosition(); pPosSc != NULL; )
	{
		CScPair* pSc = NULL;
		m_MapEqps.GetNextAssoc(pPosSc, strScKey, pEqp);
		if(pEqp->m_enKind != CEquipment::enSC){continue;};
		pSc = (CScPair*)pEqp;	
		CSC_DATA* pSC_DATA = pSc->m_pInfo->m_MapSC_DATA[strSC_NO];
		if(pSC_DATA != NULL)
			return pSC_DATA;
	}
	return NULL;	
}

CRTV_DATA* CEcsDoc::GetRTV_DATA(CString strRTV_NO)
{	
	CString strRtvKey;
	CString strValue;
	CEquipment* pEqp = NULL;
	for(POSITION pPosRtv = m_MapEqps.GetStartPosition(); pPosRtv != NULL; )
	{
		CRtv* pRtv = NULL;
		m_MapEqps.GetNextAssoc(pPosRtv, strRtvKey, pEqp);
		if(pEqp->m_enKind != CEquipment::enRTV){continue;};
		pRtv = (CRtv*)pEqp;	
		CRTV_DATA* pRtv_DATA = pRtv->m_pInfo->m_MapRTV_DATA[strRTV_NO];
		if(pRtv_DATA != NULL)
			return pRtv_DATA;
	}
	return NULL;	
}

CRTV_DATA* CEcsDoc::GetRTV_DATA(int nRTV_NO)
{	
	CString strRTV_NO;
	strRTV_NO.Format(_T("%05s"), nRTV_NO);
	CString strRtvKey;
	CString strValue;
	CEquipment* pEqp = NULL;
	for(POSITION pPosRtv = m_MapEqps.GetStartPosition(); pPosRtv != NULL; )
	{
		CRtv* pRtv = NULL;
		m_MapEqps.GetNextAssoc(pPosRtv, strRtvKey, pEqp);
		if(pEqp->m_enKind != CEquipment::enRTV){continue;};
		pRtv = (CRtv*)pEqp;	
		CRTV_DATA* pRtv_DATA = pRtv->m_pInfo->m_MapRTV_DATA[strRTV_NO];
		if(pRtv_DATA != NULL)
			return pRtv_DATA;
	}
	return NULL;	
}

CBCR_MST* CEcsDoc::GetBCR_MST(CString pstrBCR_NO)
{
	CString strBCR_NO;
	if(pstrBCR_NO == _T("")){ return NULL; };
	if(pstrBCR_NO.GetLength() < 2){ return NULL;};
	strBCR_NO = pstrBCR_NO.Right(2);

	CString strBcrKey;
	CString strValue;
	CEquipment* pEqp = NULL;
	for(POSITION pPosBcr = m_MapEqps.GetStartPosition(); pPosBcr != NULL; )
	{
		CBcr* pBcr = NULL;
		m_MapEqps.GetNextAssoc(pPosBcr, strBcrKey, pEqp);
		if(pEqp == NULL){ continue;};
		if(pEqp->m_enKind != CEquipment::enBCR){continue;};
		pBcr = (CBcr*)pEqp;	
		CBCR_MST* pBCR_MST = pBcr->m_pInfo->m_MapBCR_MST[strBCR_NO];
		if(pBCR_MST != NULL)
			return pBCR_MST;
	}
	return NULL;	
}

CBCR_MST* CEcsDoc::GetBCR_MST(int nBCR_NO)
{	
	CString strBCR_NO;
	strBCR_NO.Format(_T("%05d"), nBCR_NO);
	CString strBcrKey;
	CString strValue;
	CEquipment* pEqp = NULL;
	for(POSITION pPosBcr = m_MapEqps.GetStartPosition(); pPosBcr != NULL; )
	{
		CBcr* pBcr = NULL;
		m_MapEqps.GetNextAssoc(pPosBcr, strBcrKey, pEqp);
		if(pEqp->m_enKind != CEquipment::enBCR){continue;};
		pBcr = (CBcr*)pEqp;	
		CBCR_MST* pBCR_MST = pBcr->m_pInfo->m_MapBCR_MST[strBCR_NO];
		if(pBCR_MST != NULL)
			return pBCR_MST;
	}
	return NULL;	
}

CWC_DATA* CEcsDoc::GetWC_DATA(CString pstrWC_MC_NO)
{
	CString strWC_MC_NO;
	if(pstrWC_MC_NO == _T("")){ return NULL; };
	if(pstrWC_MC_NO.GetLength() < 2){ return NULL;};
	strWC_MC_NO = pstrWC_MC_NO.Right(3);

	CString strWcKey;
	CString strValue;
	CEquipment* pEqp = NULL;
	for(POSITION pPosWc = m_MapEqps.GetStartPosition(); pPosWc != NULL; )
	{
		CWc* pWc = NULL;
		m_MapEqps.GetNextAssoc(pPosWc, strWcKey, pEqp);
		if(pEqp == NULL){ continue;};
		if(pEqp->m_enKind != CEquipment::enWC){continue;};
		pWc = (CWc*)pEqp;	
		CWC_DATA* pWC_DATA = pWc->m_pInfo->m_MapWC_DATA[strWC_MC_NO];
		if(pWC_DATA != NULL)
			return pWC_DATA;
	}
	return NULL;	
}

CTrackInfo* CEcsDoc::GetTrackInfoNew(CString strTrackNo)
{
	// pEN_KIND, m_strThreadNo
	CString strCvKey;
	CString strValue;
	CTrackInfo* pTrackInfo = NULL;
	CEquipment* pEqp = NULL;
	CCv* pCv = NULL;
	for(POSITION pPosCv = m_MapEqps.GetStartPosition(); pPosCv != NULL; )
	{
		m_MapEqps.GetNextAssoc(pPosCv, strCvKey, pEqp);
		if(pEqp->m_enKind != CEquipment::enCV){continue;};
		pCv = (CCv*)pEqp;	
		pTrackInfo = pCv->m_pInfo->m_MapTrackInfo[strTrackNo];
		if(pTrackInfo != NULL)
			return pTrackInfo;
	}
	return NULL;	
}

CTrackInfo* CEcsDoc::GetTrackInfoNew(int nTrackNo)
{
	// pEN_KIND, m_strThreadNo
	CString strTrackNo;
	strTrackNo.Format(_T("%05d"), nTrackNo);
	CString strCvKey;
	CString strValue;
	CTrackInfo* pTrackInfo = NULL;
	CEquipment* pEqp = NULL;
	CCv* pCv = NULL;
	for(POSITION pPosCv = m_MapEqps.GetStartPosition(); pPosCv != NULL; )
	{
		m_MapEqps.GetNextAssoc(pPosCv, strCvKey, pEqp);
		if(pEqp->m_enKind != CEquipment::enCV){continue;};
		pCv = (CCv*)pEqp;	
		pTrackInfo = pCv->m_pInfo->m_MapTrackInfo[strTrackNo];
		if(pTrackInfo != NULL)
			return pTrackInfo;
	}
	return NULL;	
}

void CEcsDoc::AddWindowFontRegistry()  //보류4 font path 지정할것
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtensionCHIN = _T(".ttc");
	CString strExtension = _T(".ttf");
	CString strExtensionINI = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\font\\"), _T("font"), strExtensionINI);
	CString strValue = CLib::GetIniStringFromPath_FONT_LOAD(strFullPath, _T("fontname"), (int)EN_KOR);
	strValue += strExtension;
	Global.RegistryFont(strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\font\\"), _T("font"), strExtensionINI);
	strValue = CLib::GetIniStringFromPath_FONT_LOAD(strFullPath, _T("fontname"), (int)EN_ENG);
	strValue += strExtension;
	Global.RegistryFont(strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\font\\"), _T("font"), strExtensionINI);
	strValue = CLib::GetIniStringFromPath_FONT_LOAD(strFullPath, _T("fontname"), (int)EN_HUN);
	strValue += strExtension;
	Global.RegistryFont(strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\font\\"), _T("font"), strExtensionINI);
	strValue = CLib::GetIniStringFromPath_FONT_LOAD(strFullPath, _T("fontname"), (int)EN_CHIN);
	strValue += strExtensionCHIN;
	Global.RegistryFont(strValue);
}

BOOL CEcsDoc::FreeUserInfo()
{
	if(IsLogin() == TRUE && m_strId != _T("LFC"))//ONLY_VIEW
	{
		CString strValue = _T("");
		strValue.Format(_T("ID : %s LOGIN ALREADY. GOING TO LOGOUT?"), m_strId);
		if(AfxMessageBox(GetMsgLangDef(strValue), MB_YESNO) == IDYES)
		{
			m_strId = _T("");
		}
		else{ return FALSE; };
	}

	m_strId = _T("");
	if(m_pUserInfo.GetCount() >= 1)
	{
		for( POSITION pos = m_pUserInfo.GetStartPosition(); pos != NULL; )
		{
			CString strKey;
			CPermission* pPermission = NULL;
			m_pUserInfo.GetNextAssoc(pos, strKey, pPermission);
			if(pPermission != NULL){ delete pPermission; }
		}
		m_pUserInfo.RemoveAll();
	}

	return TRUE;
}

BOOL CEcsDoc::IsLogin()
{
	CString strValue = _T("");
	if(m_strId != _T(""))
	{
		return TRUE;
	}
	return FALSE;
}

void CEcsDoc::InitilizeUserInfo(CString pstrId, int &pnRowCnt)
{
	if(FreeUserInfo() == FALSE)
	{
		return;
	}

	CString strId = pstrId;
	CString strSql;
	CString strMessage = _T("");
	int nRowCnt = -1;

	strSql.Format(_T("  SELECT UG.GRP_CD, UGW.WIN_ID, UGW.SEL_YN, UGW.UPD_YN, DEL_YN, INS_YN, PRT_YN, EXE_YN, DOWN_YN, WIN_VIEW_YN  \n")
				  _T("    FROM USER_GRP_WIN UGW JOIN USER_GRP UG \n")
				  _T("      ON UGW.GRP_CD = UG.GRP_CD			 \n")
				  _T(" 	  JOIN USER_MST UM						 \n")
				  _T("      ON UG.GRP_LEVEL = UM.GRP_CD			 \n")
				  _T("   WHERE UM.USER_ID = '%s'	order by UGW.WIN_ID		 \n"), strId);

	_RecordsetPtr pRsptUser = GetSelectQryRecordsetPtr(strSql, nRowCnt, strMessage);  NULL;
	CRecordSetWrap* pRswUser = new CRecordSetWrap(pRsptUser); 
	CString test;
	pnRowCnt = nRowCnt;
	if(nRowCnt > 1)
	{
		m_pUserInfo.InitHashTable(nRowCnt);
		pRswUser->MoveFirst();
		for(int nIdxForUser = 0; nIdxForUser < nRowCnt; nIdxForUser++)
		{
			test += pRswUser->GetItem(_T("WIN_ID")) + _T("\n");

			CPermission* pPermission = new CPermission();

			m_pUserInfo.SetAt(pRswUser->GetItem(_T("WIN_ID")), pPermission);
			
			pPermission->m_ArrPerm[EN_PERM::SEL_YN] =        (pRswUser->GetItem(_T("SEL_YN")) == _T("Y")) ? TRUE : FALSE;
			pPermission->m_ArrPerm[EN_PERM::UPD_YN] =                (pRswUser->GetItem(_T("UPD_YN")) == _T("Y")) ? TRUE : FALSE;
			pPermission->m_ArrPerm[EN_PERM::DEL_YN] =                  (pRswUser->GetItem(_T("DEL_YN")) == _T("Y"))  ? TRUE : FALSE;
			pPermission->m_ArrPerm[EN_PERM::INS_YN] =                  (pRswUser->GetItem(_T("INS_YN")) == _T("Y"))  ? TRUE : FALSE;
			pPermission->m_ArrPerm[EN_PERM::PRT_YN] =                  (pRswUser->GetItem(_T("PRT_YN")) == _T("Y"))  ? TRUE : FALSE;
			pPermission->m_ArrPerm[EN_PERM::EXE_YN] =                  (pRswUser->GetItem(_T("EXE_YN")) == _T("Y"))  ? TRUE : FALSE;
			pPermission->m_ArrPerm[EN_PERM::DOWN_YN] =                        (pRswUser->GetItem(_T("DOWN_YN")) == _T("Y"))  ? TRUE : FALSE;
			pPermission->m_ArrPerm[EN_PERM::WIN_VIEW_YN] =  (pRswUser->GetItem(_T("WIN_VIEW_YN")) == _T("Y"))  ? TRUE : FALSE;
		
			pRswUser->MoveNext();

			// [LGLS] (버그 수정) 맵에 저장한 권한 객체를 여기서 delete 하면 맵 전체가
			//        dangling 포인터가 되어 설비 클릭(Permission 조회) 시 AV 발생.
			//        해제는 FreeUserInfo() 가 일괄 수행한다.
		}
	}
	delete pRswUser;
}

_RecordsetPtr CEcsDoc::GetSelectQryRecordsetPtr(CString pStrSql, int &pnRowCnt, CString &pStrMessage)
{
	if(IsConnectDB(m_pUrmDBAccess) == FALSE){ return FALSE; };
	return m_pUrmDBAccess->m_pAdoDB->SelectSqlForThread_RecordSet(pStrSql, pnRowCnt, pStrMessage);
}

_RecordsetPtr CEcsDoc::GetSelectQryRecordsetPtr_DLG(CString pStrSql, int &pnRowCnt, CString &pStrMessage)
{
	if(IsConnectDB(m_pDlgUrmDBAccess) == FALSE){ return FALSE; };
	return m_pDlgUrmDBAccess->m_pAdoDB->SelectSqlForThread_RecordSet(pStrSql, pnRowCnt, pStrMessage);
}


void CEcsDoc::InitializeLang()
{
	int nRowCnt = -1;
	CString strSql = m_pLang->GetQrySelect();
	CString strMessage = _T("");
	CStringList strList;
	_RecordsetPtr pRsptr = m_pUrmDBAccess->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strMessage);
	if(nRowCnt <= 0){	return;	 }
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	CString strFROM_LANG, strTO_LANG_E, strTO_LANG_H, strTO_LANG_C;
	m_pLang->InitializeLangs(nRowCnt);
	pRsw->MoveFirst(); 
	for(int nIdxFor = 0; nIdxFor < nRowCnt; nIdxFor++)
	{
		strFROM_LANG = pRsw->GetItem(_T("FROM_LANG"));
		strTO_LANG_E = pRsw->GetItem(_T("TO_LANG_E"));
		strTO_LANG_H = pRsw->GetItem(_T("TO_LANG_H"));
		strTO_LANG_C = pRsw->GetItem(_T("TO_LANG_C"));
		if(strFROM_LANG == _T("작업구분"))
		{

			int a = 0;
		}
		ST_LANG* st_LANG = new ST_LANG();
		m_pLang->MallocLPWSTR(strFROM_LANG, st_LANG->m_Key);
		m_pLang->MallocLPWSTR(strFROM_LANG, st_LANG->m_ArrLang[EN_LANG::EN_KOR]);
		m_pLang->MallocLPWSTR(strTO_LANG_E, st_LANG->m_ArrLang[EN_LANG::EN_ENG]);
		m_pLang->MallocLPWSTR(strTO_LANG_H, st_LANG->m_ArrLang[EN_LANG::EN_HUN]);
		m_pLang->MallocLPWSTR(strTO_LANG_C, st_LANG->m_ArrLang[EN_LANG::EN_CHIN]);
		m_pLang->AddLangs(strFROM_LANG, st_LANG);
		pRsw->MoveNext();

		//delete st_LANG;
	}
 	delete pRsw;
}

void CEcsDoc::GetViewHandle()
{
	CView* pView = GetViewObject();
	ASSERT(pView != NULL);
	if (pView == NULL)	return;

	m_hWndView = pView->m_hWnd;
}

CView* CEcsDoc::GetViewObject()
{
	POSITION pos = GetFirstViewPosition();
	ASSERT(pos != NULL);

	if(pos == NULL)
		return NULL;

	return GetNextView(pos);
}

// 
// void CEcsDoc::OnMenuClick_LANGUAGE_KOR()
// {
// 	m_enLang = EN_LANG::EN_KOR;
// 	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
// 	if(pWnd == NULL)
// 		return;
// 	pWnd->RenameRibbonText(m_enLang);
// }

// void CEcsDoc::OnMenuClick_SYSTEM_LOGOUT()
// {
// 	CString m_strId = _T("");
// 	CMap<CString, LPCTSTR, CPermission*, CPermission*> m_pUserInfo;
// 
// 	CString		strJunk;
// 	CPermission* posPerm;
// 	POSITION pos = m_pUserInfo.GetStartPosition();
// 	for (;;)
// 	{
// 		if (pos == NULL) break;
// 		m_pUserInfo.GetNextAssoc(pos, strJunk, posPerm);
// 		if(posPerm != NULL)
// 		{
// 			delete posPerm;
// 		}
// 	}
// }
// 
// void CEcsDoc::OnMenuClick_SYSTEM_SCTIME()
// {
// 	if(IsPermissionDlg(_T("CSystemScTimeDlg")) == FALSE)
// 	{
// 		CString strKey = _T("권한 Permission없음");
// 		CString aaa = m_pLang->GetLangValue(strKey, m_enLang);
// 		AfxMessageBox(aaa);
// 		return;
// 	}
// 
// 	CSystemScTimeDlg* pDlg = new CSystemScTimeDlg(this);
// 	pDlg->Create(IDD_SYSTEM_SCTIME);
// 	pDlg->ShowWindow(SW_SHOW);
// }

void CEcsDoc::OnMenuClick_VIEW_RACK()
{
	// [LGLS 2026-08-05 재적용] 재고정보(ViewRackDlg) 삭제 - 무동작
}

BOOL CEcsDoc::IsPermissionDlg(CString pStr)
{
// 	CPermission* pPermission = NULL;
// 	m_pUserInfo.Lookup(pStr, pPermission);
// 	if(pPermission == NULL || pPermission->m_ArrPerm[EN_PERM::SEL_YN] == FALSE)
// 	{	
// 		return FALSE;
// 	}
	
	return TRUE;
}

CDciControl* CEcsDoc::GetDciControl_FindAllLayout(CString& strCID)
{
	CEcsView* pView = (CEcsView*)GetViewObject();
	if(pView == NULL){ return NULL;};

	CDciControl* pDciControl = NULL; //보류4

	int nLast = 2;

	if(m_WH_TYP == _T("10"))
		nLast = EN_CHIN_LAST;
		
	for(int nIdxLayout = (EN_CHIN_INIT + 1); nIdxLayout < nLast; nIdxLayout++)
	{
		pDciControl = GetLayout_PARM((EN_LAYOUT)nIdxLayout)->GetDciControl(strCID);
		if(pDciControl != NULL){break;}
	}
	//pDciControl = GetSelectedLayout()->GetDciControl(strCID);
	return pDciControl;
}

CDciControl* CEcsDoc::GetDciControl_FindAllLayout(CString& strCID, int& nLayoutNo)
{
	CEcsView* pView = (CEcsView*)GetViewObject();
	if(pView == NULL){ return NULL;};

	CDciControl* pDciControl = NULL; //보류4

	int nLast = 2;

	if(m_WH_TYP == _T("10"))
		nLast = EN_CHIN_LAST;

	for(int nIdxLayout = (EN_CHIN_INIT + 1); nIdxLayout < nLast; nIdxLayout++)
	{
		pDciControl = GetLayout_PARM((EN_LAYOUT)nIdxLayout)->GetDciControl(strCID);
		if(pDciControl != NULL)
		{
			nLayoutNo = nIdxLayout;
			break;
		}
	}
	//pDciControl = GetSelectedLayout()->GetDciControl(strCID);
	return pDciControl;
}

CDciControl* CEcsDoc::GetDciControl(CString& strCID)
{
	CEcsView* pView = (CEcsView*)GetViewObject();
	if(pView == NULL){ return NULL;};

	CDciControl* pDciControl = NULL; //보류4
//	pDciControl = GetDciControl_FindAllLayout(strCID);

//	return pDciControl;

	
	int nLayoutCnt = m_pEcsLayOuts.GetSize();
	for (int i=0;i<nLayoutCnt;i++)
	{
		CEcsLayout * pEcsLayout = m_pEcsLayOuts[i];

		if (pEcsLayout == NULL)
			continue;

		if (pEcsLayout->GetDciControl(strCID) != NULL)
			return pEcsLayout->GetDciControl(strCID);
	}

	return NULL;
	//*/

}
// 
// BOOL CEcsDoc::GetDBObject( CAdoDB** pDB, CURMDBAccess** pDbAccess )
// {
// 	TRY
// 	{
// 		if((*pDB) == NULL || (*pDbAccess) == NULL)
// 		{
// 			(*pDB) = new CAdoDB();
// 			(*pDbAccess) = new CURMDBAccess(this, (*pDB));
// 			return TRUE;
// 		}
// 	}
// 	CATCH(CException, e)
// 	{
// 		(*pDB) = NULL;
// 		(*pDbAccess) = NULL;
// 		return FALSE;
// 	}
// 	END_CATCH
// 
// 	return TRUE;
// }
// 
// BOOL CEcsDoc::IsConnectMainDB()
// {
// 	if(m_pUrmDB == NULL || m_pUrmDB->m_bConnected == FALSE)
// 	{
// 		m_pUrmDB = NULL;
// 		m_pUrmDBAccess = NULL;
// 		m_pUrmDB = new CAdoDB(this);
// 		m_pUrmDBAccess = new CURMDBAccess(this, m_pUrmDB);
// 		if(m_pUrmDB->ConnectDB() == FALSE)
// 		{
// 			delete m_pUrmDB;
// 			delete m_pUrmDBAccess;
// 			return FALSE;
// 		}
// 	}
// 	return TRUE;
// }

void CEcsDoc::EnterBlcokingSection()
{
	CEcsDoc::m_csLockSyncThread.Lock();
}

void CEcsDoc::LeaveBlcokingSection()
{
	CEcsDoc::m_csLockSyncThread.Unlock();
}


void CEcsDoc::UpdateRibbonLang()
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	if(pWnd == NULL)
		return;
	::PostMessage(pWnd->m_hWnd, WM_USER_LANG_NOTIFY, 0, (LPARAM)this);
}


CEcsLayout* CEcsDoc::GetLayout_PARM(EN_LAYOUT penLAYOUT)
{
	//if(penLAYOUT == EN_LFC_G && m_WH_TYP == _T("10"))
	//{ 
	//	return &m_layout;
	//}

	//CArray<CEcsLayout*, CEcsLayout*> m_pEcsLayOuts;

	int nLayoutCnt = m_pEcsLayOuts.GetSize();
	for (int i=0;i<nLayoutCnt;i++)
	{
		CEcsLayout * pEcsLayout = m_pEcsLayOuts[i];

		if (pEcsLayout == NULL)
			continue;

		return pEcsLayout;

		//if (pEcsLayout->GetDciControl(strCID) != NULL)
		//	return pEcsLayout->GetDciControl(strCID);
	}

	return NULL;
}


CEcsLayout* CEcsDoc::GetSelectedLayout()
{
	////LFC G동
	//if(m_enSelectedLayout == EN_LFC_G && m_WH_TYP == _T("10"))
	//{
	//	return &m_layout;
	//}

	int nLayoutCnt = m_pEcsLayOuts.GetSize();
	for (int i=0;i<nLayoutCnt;i++)
	{
		CEcsLayout * pEcsLayout = m_pEcsLayOuts[i];

		if (pEcsLayout == NULL)
			continue;

		return pEcsLayout;

		//if (pEcsLayout->GetDciControl(strCID) != NULL)
		//	return pEcsLayout->GetDciControl(strCID);
	}

	return NULL;

	//return &m_layout;
}

CString CEcsDoc::GetDefineXmlPATH()
{
	if(m_WH_TYP == "10")
	{ 
		return ECS_DEFINE_FILE; 
	}

	/*else if(m_WH_TYP == "20"){ return ECS_DEFINE_FILE_HUN_CMPROD; }
	else if(m_WH_TYP == "30"){ return ECS_DEFINE_FILE_CHIN_HWASUNG; }
	else if(m_WH_TYP == "40"){ return ECS_DEFINE_FILE_CHIN_CMPROD; }*/

	return ECS_DEFINE_FILE;
}

BOOL CEcsDoc::IsAlliveCollectDB()
{
	if(m_pCollectDB == NULL){ m_pCollectDB = new CCollectDB(this);};
	return m_pCollectDB->IsAllive();
}

void CEcsDoc::AlliveCollectDB()
{
	m_pCollectDB->StartDoWork();
}

BOOL CEcsDoc::IsAlliveCollectRequest()
{
	if(m_pCollectRequest == NULL){ m_pCollectRequest = new CCollectDataList(this);};
	return m_pCollectRequest->IsAllive();
}

void CEcsDoc::AlliveCollectRequest()
{
	m_pCollectRequest->StartDoWork();
}

void CEcsDoc::UpdateJOB_MST()
{
	if(m_CollectJOB_MST.m_bRequest == FALSE){	m_CollectJOB_MST.m_bRequest = TRUE;	}else{	return;	};
	m_CollectJOB_MST.m_strWH_TYP = m_WH_TYP;
}

void CEcsDoc::InitializeErrorMst()
{
	m_pErrorMst = new CErrorMst(this);
	//if(m_pUrmDB == NULL){ return; };
	
	CString strMessage;
	int nRowCnt = -1;
	CString strSql = m_pErrorMst->GetQrySelect();
	_RecordsetPtr pRspError = m_pUrmDBAccess->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRswError = new CRecordSetWrap(pRspError); 
	if(nRowCnt > 1)
	{
		m_pErrorMst->m_Map->InitHashTable(nRowCnt);
		pRswError->MoveFirst();
		for(int nIdxForError = 0; nIdxForError < nRowCnt; nIdxForError++)
		{
			CString strEQP_TYP, strERROR_CODE;
			CString strACTION_KOR, strACTION_ENG, strACTION_CHIN, strACTION_HUN;
			CString strMSG_KOR, strMSG_ENG, strMSG_CHIN, strMSG_HUN;
			strEQP_TYP = pRswError->GetItem(_T("EQP_TYP"));
			// [LGLS] CString 자기참조 Format 은 VS2019 UCRT 에서 해제된 버퍼 참조로 AV -> 임시변수로 분리
			CString strCodeTmp = pRswError->GetItem(_T("EQP_ERR_CD"));
			strERROR_CODE.Format(_T("%04s"), (LPCTSTR)strCodeTmp);
			CEQP_ECD_MST* pMapItem = new CEQP_ECD_MST(strEQP_TYP, strERROR_CODE);
			strACTION_KOR = pRswError->GetItem(_T("ACTION_KOR"));
			strACTION_ENG = pRswError->GetItem(_T("ACTION_ENG"));
			strACTION_CHIN = pRswError->GetItem(_T("ACTION_CHIN"));
			strACTION_HUN = pRswError->GetItem(_T("ACTION_HUN"));

			strMSG_KOR = pRswError->GetItem(_T("MSG_KOR"));
			strMSG_ENG = pRswError->GetItem(_T("MSG_ENG"));
			strMSG_CHIN = pRswError->GetItem(_T("MSG_CHIN"));
			strMSG_HUN = pRswError->GetItem(_T("MSG_HUN"));
			pMapItem->SetValues_ACTION(strACTION_KOR, strACTION_ENG, strACTION_CHIN, strACTION_HUN);
			pMapItem->SetValues_MSG(strMSG_KOR, strMSG_ENG, strMSG_CHIN, strMSG_HUN);
			m_pErrorMst->m_Map->SetAt(pMapItem->GetTupleKeyEQP_ECD_MST(), pMapItem);
			pRswError->MoveNext();
		}
	}
}


CString CEcsDoc::GetQrySelectFIRE()
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	strSql += CRLF + _T("SELECT CM.WH_TYP AS WH_TYP ");
	strSql += CRLF + _T("	   ,") + NVL + _T("(CCD_WH_TYP.CCD_NM_KOR,'30') AS WH_TYP_NM ");
	strSql += CRLF + _T("	   ,'0' AS PLC_NO ");
	strSql += CRLF + _T("	   ,") + NVL + _T("(CCD_EQP_NO.CCD_NM_KOR,'0') AS EQP_NO ");
	strSql += CRLF + _T("	   ,") + NVL + _T("(CM.CELL_NO,'00-000-00') AS CELL_NO ");
	strSql += CRLF + _T("	   ,") + NVL + _T("(CCD_FIRE.CCD_NM_KOR,'N') AS FIRE_OD_RQ_YN ");
	strSql += CRLF + _T("  FROM CELL_MST CM");
	strSql += CRLF + _T("					 LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP ");
	strSql += CRLF + _T("					         ON CCD_WH_TYP.CDX_CD = 'WH_TYP' ");
	strSql += CRLF + _T("					        AND CCD_WH_TYP.CCD_CD = CM.WH_TYP ");
	strSql += CRLF + _T("					        AND CCD_WH_TYP.WH_TYP LIKE ") + CLib::QuotLikeLR(m_WH_TYP);
	strSql += CRLF + _T("					 LEFT OUTER JOIN COMMON_CODE CCD_EQP_NO ");
	strSql += CRLF + _T("					         ON CCD_EQP_NO.CDX_CD = 'SC_NO' ");
	strSql += CRLF + _T("					        AND CCD_EQP_NO.CCD_CD = CM.CELL_SC_NO ");
	strSql += CRLF + _T("					        AND CCD_EQP_NO.WH_TYP LIKE ") + CLib::QuotLikeLR(m_WH_TYP);
	strSql += CRLF + _T("					 LEFT OUTER JOIN COMMON_CODE CCD_FIRE ");
	strSql += CRLF + _T("					         ON CCD_FIRE.CDX_CD = 'FIRE_OD_RQ_YN' ");
	strSql += CRLF + _T("					        AND CCD_FIRE.CCD_CD = CM.FIRE_OD_RQ_YN ");
	strSql += CRLF + _T("					        AND CCD_FIRE.WH_TYP LIKE ") + CLib::QuotLikeLR(m_WH_TYP);
	strSql += CRLF + _T(" WHERE CM.FIRE_OD_RQ_YN = 'Y' ");
	
	
	return CLib::GetCommonCodeLang(strSql, (int)m_enLang);
}

CFireParms* CEcsDoc::CreateFireParm(CRecordSetWrap* pRsw)
{
	CFireParms* pParm = new CFireParms();
	CString strWH_TYP = pRsw->GetItem(_T("WH_TYP"));
	CString strWH_TYP_NM = pRsw->GetItem(_T("WH_TYP_NM"));
	CString strPLC_NO = pRsw->GetItem(_T("PLC_NO"));
	CString strEQP_NO = pRsw->GetItem(_T("EQP_NO"));
	CString strCELL_NO = pRsw->GetItem(_T("CELL_NO"));
	pParm->SetSTR(strWH_TYP, strWH_TYP_NM, strPLC_NO, strEQP_NO, strCELL_NO, _T(" "), _T("100"), (int)m_enLang);
	if(pParm->InitializeThis() == FALSE)
	{
		return NULL;
	}
	return pParm;
}

int CEcsDoc::GetFireMessage()
{
	m_pErrorMst = new CErrorMst(this);
	//if(m_pUrmDB == NULL){ return -1; };
	
	CString strSql;
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql.Format(_T("SELECT ") + NVL + _T("(FIRE_OD_RQ_YN,'N') AS FIRE_OD_RQ_YN \n")
			  _T("  FROM CELL_MST \n")
			  _T(" WHERE FIRE_OD_RQ_YN = 'N' \n"));
	
	_RecordsetPtr rtrsPtr = GetSelectQryRecordsetPtr(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(rtrsPtr);
	delete pRsw;

	return nRowCnt;
}

int CEcsDoc::GetSelectQryCnt(CString pStrSql)
{
	if(IsConnectDB(m_pUrmDBAccess) == FALSE){ return FALSE; };
	return m_pUrmDBAccess->SelectQryCnt(pStrSql);
}

int CEcsDoc::GetSelectQryCnt_DLG(CString pStrSql)
{
	if(IsConnectDB(m_pDlgUrmDBAccess) == FALSE){ return FALSE; };
	return m_pDlgUrmDBAccess->SelectQryCnt(pStrSql);
}

int CEcsDoc::ExcuteQueryString_DLG( CString pstrSql )
{
	if(IsConnectDB(m_pDlgUrmDBAccess) == FALSE){ return FALSE; };
	return m_pDlgUrmDBAccess->m_pAdoDB->ExecuteQueryString(pstrSql);
}

int CEcsDoc::ExcuteQueryString( CString pstrSql )
{
	if(IsConnectDB(m_pUrmDBAccess) == FALSE){ return FALSE; };
	return m_pUrmDBAccess->m_pAdoDB->ExecuteQueryString(pstrSql);
}

long CEcsDoc::BeginTrans()
{
	return 1;
}


long CEcsDoc::RollbackTrans()
{
	return 1;
}


long CEcsDoc::BeginTrans_DLG()
{
	if(IsConnectDB(m_pDlgUrmDBAccess) == FALSE){ return FALSE; };
	
	if (m_bTrans == false)
	{
		m_pDlgUrmDBAccess->m_pAdoDB->BeginTrans();
		m_bTrans = true;
	}
	

	return 1;
}

long CEcsDoc::RollbackTrans_DLG()
{
	if(IsConnectDB(m_pDlgUrmDBAccess) == FALSE){ return FALSE; };
	
	if (m_bTrans == true)
	{
		m_pDlgUrmDBAccess->m_pAdoDB->RollbackTrans();
	}
	
	m_bTrans = false;

	return 1;
}

long CEcsDoc::CommitTrans_DLG()
{	
	if(IsConnectDB(m_pDlgUrmDBAccess) == FALSE){ return FALSE; };

	if (m_bTrans == true)
	{
		m_pDlgUrmDBAccess->m_pAdoDB->CommitTrans();
	}
	
	m_bTrans = false;

	return 1;
}

long CEcsDoc::CommitTrans()
{
	return 1;
}

BOOL CEcsDoc::Permission(CString pWID_ID, int pEN_PERM)
{
	// [LGLS] (버그 수정) 기존: new CPermission() 누수 + 룩업 성공 시 dangling 포인터 참조.
	//        룩업 실패(권한 미정의 화면)는 허용으로 처리해 기존 기본 동작을 유지한다.
	CPermission* pPermission = NULL;
	if (m_pUserInfo.Lookup(pWID_ID, pPermission) == FALSE || pPermission == NULL)
	{
		return TRUE;
	}

	if (pEN_PERM < 0 || pEN_PERM >= 8)
	{
		return FALSE;
	}

	return pPermission->m_ArrPerm[pEN_PERM];
}

BOOL CEcsDoc::GetQueryInsertClientLog(CString pWIN_ID, CString pLUGG_NO, CString pBOTTOM_TRAY, CString pTOP_TRAY, CString pMESSAGE, BOOL bAlarm /* = FALSE */)
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	
	strSql += CRLF + _T("			INSERT INTO WCS_CLIENT_LOG			");
	strSql += CRLF + _T("				( WH_TYP  						");
	strSql += CRLF + _T("				, INS_DT  						");
	strSql += CRLF + _T("				, IP      						");
	strSql += CRLF + _T("				, USER_ID 						");
	strSql += CRLF + _T("				, WIN_ID  						");
	strSql += CRLF + _T("				, LUGG_NO 						");
	strSql += CRLF + _T("				, BOTTOM_TRAY  					");
	strSql += CRLF + _T("				, TOP_TRAY  					");
	strSql += CRLF + _T("				, MESSAGE 						");
	strSql += CRLF + _T("				, ALARM_YN )					");
	strSql += CRLF + _T("				VALUES 							");
	strSql += CRLF + _T("				( '") + m_WH_TYP + _T("' 		");
	strSql += CRLF + _T("				, ")  + SYSDATE + _T("			");
	strSql += CRLF + _T("				, '") + m_PC_IP + _T("'			");
	strSql += CRLF + _T("				, '") + m_strId + _T("'			");
	strSql += CRLF + _T("				, '") + pWIN_ID + _T("'			");
	strSql += CRLF + _T("				, '") + pLUGG_NO + _T("'		");
	strSql += CRLF + _T("				, '") + pBOTTOM_TRAY + _T("'	");
	strSql += CRLF + _T("				, '") + pTOP_TRAY + _T("'		");
	strSql += CRLF + _T("				, '") + pMESSAGE + _T("'			");
	strSql += CRLF + _T("				, '") + CString(bAlarm ? _T("Y") : _T("N")) + _T("')	");

 	BOOL isSuccess = ExcuteQueryString_DLG(strSql);
	if(isSuccess == FALSE)
	{
		AfxMessageBox(GetMsgLangDef(_T("CLIENT LOG 추가 실패")));
	}
	// [LGLS 2026-08-22] 구 CLog::Write 의 bAlarm 처리(PumpupAlarm) 대응 -
	//   로그를 남기면서 이 PC 의 경고창에 즉시 띄운다. 다른 Client 는 ALARM_YN 을
	//   보고 자기 주기에 가져간다.
	if (bAlarm && m_pWarningDlg != NULL && ::IsWindow(m_pWarningDlg->GetSafeHwnd()))
	{
		((CWarningDlg*)m_pWarningDlg)->PumpupAlarm(pWIN_ID, pLUGG_NO, pMESSAGE);
	}
	
	return isSuccess;	

}

void CEcsDoc::SetMsgLangDef()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strCnt = _T("");
	CString strFor = _T("");
	int nCnt = 0;
	int iFor = 0;
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");
	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\msg_box\\"), _T("msg_box"), strExtension);
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("CNT"), _T("CNT"), _T("0"), szTemp, _MAX_PATH, strFullPath);
	strCnt.Format(_T("%s"), szTemp); 
	nCnt = CConvert::ToInt(strCnt);

	CString strKey, strLang = _T("");
	int nFind = 0;

	for (iFor = 1; iFor <= nCnt; iFor++)
	{
		strFor = CConvert::ToString(iFor);
		CString strValue = CLib::GetIniStringFromPath(strFullPath, strFor, (int)m_enLang);
		nFind = strValue.Find(_T("$"));
		strKey = strValue.Left(nFind);
		strLang = strValue.Right(strValue.GetLength() - nFind -1);

		CMsgBoxLang* pMsgBoxLang = new CMsgBoxLang();

		m_pMsgBoxLang.SetAt(strKey, pMsgBoxLang);

		if (m_enLang == EN_MSG_LANG::kor) {pMsgBoxLang->m_ArrPerm[EN_MSG_LANG::kor] = strLang;}
		else if (m_enLang == EN_MSG_LANG::chin) {pMsgBoxLang->m_ArrPerm[EN_MSG_LANG::chin] = strLang;}

	}
}

CString CEcsDoc::GetMsgLangDef(CString pKey)
{
	CMsgBoxLang* pMsgBoxLang = new CMsgBoxLang();

	m_pMsgBoxLang.Lookup(pKey , pMsgBoxLang);
	CString strReturn = pMsgBoxLang->m_ArrPerm[m_enLang];

	if (strReturn == _T("")) {strReturn = pKey;}

	if (pMsgBoxLang != NULL) {delete pMsgBoxLang;};

	return strReturn;
}

BOOL CEcsDoc::IsFireDlg()
{
	if(m_pFireMessageDlg == NULL){ return FALSE;};
	return TRUE;
}

void CEcsDoc::CreateFireMessageDlg()
{
	m_pFireMessageDlg = new CFireMessageDlg(this);
	m_pFireMessageDlg->Create(IDD_FIRE_MESSAGE);
}

void CEcsDoc::OnCreateScManualRet(CString pScNo)
{
	if (m_pScManualRet == NULL)
	{
		m_pScManualRet = new CScManualRet(this);
		this->m_pScManualRet->Create(IDD_SC_MANUAL_RET);
		CRect MainRect;
		CRect Rect;
		CRect PosRect;
		::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
		this->m_pScManualRet->GetWindowRect(&Rect); 

		PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
		PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
		this->m_pScManualRet->SetWindowPos(&m_pScManualRet->wndTop, PosRect.left, PosRect.top, 
			Rect.Width(), Rect.Height(), 
			SWP_SHOWWINDOW);

	}
	CSC_DATA* pSC_DATA = GetSC_DATA(pScNo);
	m_pScManualRet->m_pSC_DATA = pSC_DATA;
	m_pScManualRet->m_strScNo = pScNo;
	::SetWindowPos(m_pScManualRet->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	::SetWindowPos(m_pScManualRet->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(m_pScManualRet->m_hWnd, SW_SHOW);
}

BOOL CEcsDoc::EquipStatusCheck()
{
	CConnectStatus* pConnectStatus = new CConnectStatus(this);
	m_pConnectStatus = pConnectStatus;

	pConnectStatus->m_WH_TYP = m_WH_TYP;

	//HOST 통신 상태 확인 준비 완료
	m_blConnectStatus = TRUE;
	return TRUE;
}

// [LGLS 2026-07-20 재적용] 트랙 표시 모드 전환 (작업번호/트랙번호/제품정보)
void CEcsDoc::OnCommandTrackTextMode(UINT nID)
{
	m_nTrackTextMode = (int)(nID - ID_MONITORING_VIEW_JOBNO);	// 0=작업번호 1=트랙번호 2=제품정보
	UpdateAllViews(NULL);
}

// [LGLS 2026-08-13] 리본 MANUAL > 반자동 TEST : 반자동 TEST 창 열기 (모덜리스, 문서가 소유)
void CEcsDoc::OnCommandSemiTestOpen()
{
	if (m_pSemiTest != NULL && !::IsWindow(m_pSemiTest->m_hWnd))
	{
		delete m_pSemiTest;
		m_pSemiTest = NULL;
	}
	if (m_pSemiTest == NULL)
	{
		m_pSemiTest = new CSemiTestDlg(this);
		m_pSemiTest->Create(IDD_LGLS_SEMI_TEST);
		CRect MainRect;
		CRect Rect;
		::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);
		m_pSemiTest->GetWindowRect(&Rect);
		m_pSemiTest->SetWindowPos(&CWnd::wndTop,
			((MainRect.right  - MainRect.left) - Rect.Width())  / 2,
			((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2,
			Rect.Width(), Rect.Height(), SWP_SHOWWINDOW);
	}
	m_pSemiTest->ShowWindow(SW_SHOW);
	m_pSemiTest->SetForegroundWindow();
}

// [LGLS 2026-08-13] 리본 MANUAL > TEST 초기화 :
//   CV/SC/RTV 설비 데이터의 작업 관련 값과 JOB_MST 작업정보를 모두 초기화한다. (반자동 TEST 클린 스타트용)
void CEcsDoc::OnCommandSemiTestClear()
{
	if (AfxMessageBox(GetMsgLangDef(_T("CV/SC/RTV 데이터와 모든 작업정보를 초기화하시겠습니까?")), MB_YESNO | MB_ICONWARNING) != IDYES)
		return;

	CStringArray arSql;
	arSql.Add(_T("DELETE FROM JOB_MST WHERE WH_TYP = '") + m_WH_TYP + _T("'"));
	arSql.Add(_T("UPDATE CV_DATA SET LUGG_NO_RD='0000', JOB_TYP_RD='0', DEST_POS_RD='000', ERROR_CODE='0' WHERE WH_TYP='") + m_WH_TYP + _T("'"));
	arSql.Add(_T("UPDATE SC_DATA_LGLS SET ITN_LUGG_FK1='0', ITN_LUGG_FK2='0', LUGG_NO_FK1_OD='0', JOB_TYP_OD='0', CMD_RQ_YN='N', HOST_SEND_YN='N', HOST_ERR_SEND_YN='N' WHERE WH_TYP='") + m_WH_TYP + _T("'"));
	arSql.Add(_T("UPDATE RTV_DATA_LGLS SET LUGG_OD='0', JOB_TYP_OD='0', CMD_RQ_YN='N' WHERE WH_TYP='") + m_WH_TYP + _T("'"));

	BeginTrans_DLG();
	for (int i = 0; i < arSql.GetSize(); i++)
	{
		if (!ExcuteQueryString_DLG(arSql[i]))
		{
			RollbackTrans_DLG();
			AfxMessageBox(GetMsgLangDef(_T("TEST 초기화 실패")));
			return;
		}
	}
	CommitTrans_DLG();
	AfxMessageBox(GetMsgLangDef(_T("TEST 초기화 완료")));
}
