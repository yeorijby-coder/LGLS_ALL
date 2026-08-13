// EcsView.cpp : implementation of the CEcsView class
//

#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "EcsView.h"
#include "MainFrm.h"

//#include "ScDlg.h"
//#include "ScPairDlg.h"
//#include "ScHalfPairDlg.h"
#include "LgvDlg.h"
#include "LgvcDlg.h"
//#include "LifterDlg.h"
//#include "LgvDlg.h"
//#include "RfidDlg.h"
//#include "BcrDlg.h"
//#include "DisplayDlg.h"
#include "DataSettingDlg.h"
#include "MessageDisplayDlg.h"

#include "ScDongJinDlg.h"


//TEST
//#include "Dictionary.h"
#include <iostream>

#define MAIN_TEXT_GAP_Y	18

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEcsView

IMPLEMENT_DYNCREATE(CEcsView, CFormView)

BEGIN_MESSAGE_MAP(CEcsView, CFormView)
	ON_MESSAGE(WM_USER_HOST_NOTIFY, OnHostNotify)
	ON_MESSAGE(WM_USER_CV_NOTIFY, OnCvNotify)
	ON_MESSAGE(WM_USER_SC_NOTIFY, OnScNotify)
	ON_MESSAGE(WM_USER_RTV_NOTIFY, OnRtvNotify)
//	ON_MESSAGE(WM_USER_LGV_NOTIFY, OnLgvNotify)
//	ON_MESSAGE(WM_USER_LIFTER_NOTIFY, OnLifterNotify)
//	ON_MESSAGE(WM_USER_RFID_NOTIFY, OnRfidNotify)
//	ON_MESSAGE(WM_USER_BCR_NOTIFY, OnBcrNotify)
//	ON_MESSAGE(WM_USER_DISPLAY_NOTIFY, OnDisplayNotify)
	ON_MESSAGE(WM_USER_CONTROL_CLICK, OnControlClick)
	ON_MESSAGE(WM_USER_ALARM_NOTIFY, OnAlarmNotify)
	//{{AFX_MSG_MAP(CEcsView)		
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_VIEW_LAYOUT_INFO, OnViewLayoutInfo)
	ON_COMMAND(ID_UNSET_ALL_SUSPEND, OnUnSetAllSuspend)
	ON_COMMAND(ID_SET_ALL_SUSPEND, OnSetAllSuspend)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LAYOUT_INFO, OnUpdateViewLayoutInfo)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_NOTIFY(TCN_SELCHANGE, IDC_LAYOUT_TAB, OnSelchangeLayoutTab)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_CHECK_LAMP, OnCheckLamp)
	ON_COMMAND(ID_INPUT_MESSAGE, OnInputMessage)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BTN_RESET1, OnBtnReset1)
	ON_BN_CLICKED(IDC_BTN_RESET8, OnBtnReset2)
	ON_BN_CLICKED(IDC_CHK_TEST_START1, OnChkTestStart1)
	ON_BN_CLICKED(IDC_CHK_TEST_START2, OnChkTestStart2)
	ON_BN_CLICKED(IDC_BTN_ALTER_LOC, OnBtnAlterLoc)
	ON_BN_CLICKED(IDC_BTN_RTR_ORDER, OnBtnRtrOrder)
	ON_BN_CLICKED(IDC_BTN_ATA_ORDER, OnBtnAtaOrder)
	ON_BN_CLICKED(IDC_BTN_SITE_ORDER, OnBtnSiteOrder)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BTN_START, &CEcsView::OnBnClickedBtnStart)
	ON_CBN_SELCHANGE(IDC_LOGIC_GROUP, &CEcsView::OnCbnSelchangeLogicGroup)
	ON_BN_CLICKED(IDC_BTN_END, &CEcsView::OnBnClickedBtnEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEcsView construction/destruction

CEcsView::CEcsView()
	: CFormView(CEcsView::IDD)
{
	//{{AFX_DATA_INIT(CEcsView)
	m_bOn = FALSE;
	m_bTesting1 = FALSE;
	m_bTesting2 = FALSE;
	//}}AFX_DATA_INIT
	// TODO: add construction code here
}

CEcsView::~CEcsView()
{

}

void CEcsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEcsView)
	DDX_Control(pDX, IDC_LIST_HOST_SV, m_lstHostSv);
	DDX_Control(pDX, IDC_LIST_HOST_CL, m_lstHostCl);
	DDX_Control(pDX, IDC_LIST_ALL_JOB, m_lstJobList);
	DDX_Control(pDX, IDC_REQUST_LAMP, m_cbxLamp);
	DDX_Control(pDX, IDC_LAYOUT_TAB, m_tabLayout);
	DDX_Control(pDX, IDC_REQUST_STATION, m_cbxStation);
	DDX_Check(pDX, IDC_CHECK_LAMP, m_bOn);
	DDX_Check(pDX, IDC_CHK_TEST_START1, m_bTesting1);
	DDX_Check(pDX, IDC_CHK_TEST_START2, m_bTesting2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LOGIC_GROUP, m_cbxLogicGroup);
	DDX_Control(pDX, IDC_LIST_THIS_LOGIC_JOB, m_lstThisLogicJob);
}

BOOL CEcsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

#define		ID_MAIN_TIMER		100
void CEcsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	if (pDoc == NULL)
		return;

	CWinApp* pApp = AfxGetApp();

	pDoc->RunServers();

	if (pDoc->m_pConfig->m_portHost.m_bConnect)
		pDoc->ConnectHostServer();

	
	CEquipment* pEquipment = NULL;
	int j = 0;
	int nScCount = 0;		// SC 대수
	int nRtvCount = 0;		// RTV 대수
	for (int i = 0; i < pDoc->m_pEquipments.GetSize(); ++i)
	{
		pEquipment = pDoc->m_pEquipments[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment == NULL)
			continue;

		pEquipment->UpdateCommStatus(NOTIFY_ERROR);
		// 수정요망
		if (pEquipment->m_enKind == CEquipment::enSC)
		{
			CScDongJin* pSc = (CScDongJin*)pEquipment;

			if (pSc == NULL)
				continue;

			if (pEquipment->m_nNumber == 1)
			{
				j = 1;								// 1
				nScCount = SC1_PLC_CNT;				// 3	
			}
			else
			{
				j = SC1_PLC_CNT + 1;				// 4
				nScCount = SC_PLC_TOT_CNT;			// 6	
			}

			for (;j <= nScCount; j++)
			{

				CScInfo* pInfo = (CScInfo*)pSc->GetInfo(j, TRUE);

				if (pInfo == NULL)
					continue;

				pInfo->Restore(j);
			}
		}
		else
		{
			DEBUGER_ASSERT_VALID(pEquipment->GetInfo() != NULL);
			if (pEquipment->GetInfo() == NULL)
				continue;

			pEquipment->GetInfo()->Restore();
		}
		pEquipment->RunEquipment();
	}
		//*/

	m_tabLayout.InsertItem(0, _T("1단"), 0);
	m_tabLayout.InsertItem(1, _T("2단"), 1);

	ENABLE_WND(IDC_STC_WORK_LUGG1, FALSE);
	ENABLE_WND(IDC_WORKING_LUGG1, FALSE);
	ENABLE_WND(IDC_BTN_RESET1, FALSE);
	ENABLE_WND(IDC_BTN_ALTER_LOC, FALSE);
	ENABLE_WND(IDC_DUAL_STO_LOC, FALSE);
	ENABLE_WND(IDC_STC_DUAL_STO_LOC, FALSE);

	// 시작/종료 버튼 : 프로그램 시작시 모두 비활성 -> 첫 상태전문 수신시 시작버튼만 활성화(OnTimer에서 처리)
	ENABLE_WND(IDC_BTN_START, FALSE);
	ENABLE_WND(IDC_BTN_END, FALSE);

	if (pDoc != NULL && pDoc->m_pConfig != NULL && pDoc->m_pConfig->m_nLogicGroupCnt > 0)
	{
		CString strTemp;
		for (int i = 1; i <= pDoc->m_pConfig->m_nLogicGroupCnt; ++i)
		{
			strTemp.Format(_T("LOGIC_GROUP%02d"), i);
			m_cbxLogicGroup.AddString(strTemp);
			m_cbxLogicGroup.SetItemData(m_cbxLogicGroup.GetCount() - 1, i);
		}
	}

	/*
//	DDX_Control(pDX, IDC_LIST_THIS_LOGIC_JOB, m_lstThisLogicJob);
	int i = 0;
	//const int size = 5;
	UINT	mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	int		fmt = LVCFMT_LEFT;
	int		cx[] = { 80, 100, 160, 160, 160 };
	LPTSTR	pszText[] = { _T("작업번호"), _T("작업구분"), _T("출발위치"), _T("도착위치"), _T("작업지시시간") };
	int nCol = sizeof(pszText) / sizeof(pszText[0]);
	for (i = 0; i < nCol; ++i)
		m_lstThisLogicJob.InsertColumn(i, (LPCTSTR)pszText[i], fmt, cx[i]);
	m_lstThisLogicJob.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_lstThisLogicJob.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//*/
	CImageList imageList;
	//CListCtrl* pListCtrl1 = (CListCtrl*)GetDlgItem(IDC_LIST_COMM1);

	// Ethernet Port List Column
	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/, 1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BOARD));
	m_lstThisLogicJob.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lstThisLogicJob.SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt = LVCFMT_LEFT;
		int		cx[] = { 80, 100, 80, 80, 160, 200 };
		LPTSTR	pszText[] = { _T("작업번호"), _T("작업구분"), _T("출발위치"), _T("도착위치"), _T("작업지시시간"), _T("비고")};
		int nCol = sizeof(pszText) / sizeof(pszText[0]);

		for (int i = 0; i < nCol; ++i)
			m_lstThisLogicJob.InsertColumn(i, pszText[i], nFmt, cx[i]);
	}

	m_nMainTimerID = SetTimer(ID_MAIN_TIMER, 1000, NULL);
	
}

void CEcsView::OnTimer(UINT_PTR  nIDEvent)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (pDoc == NULL) 
	{
		return;
	}

	static int nCount = 1;

	//TEST
//	static int nTemp = 0;
//
//	const int nTempTemp = ++nTemp;
//	CString strTemp;
//	Dictionary<std::string, int> dict("key0", 1);
//
//	dict.PushKey("key1", 1);
//	dict.PushKey("key2", 1);
//
//	DictionaryUnit<std::string, int> unit = dict.GetValueFromKey("key");
//
//	for (int k=0;k<unit.KeyCount();k++)
//		strTemp.Format(_T("%d"), unit.GetKey(k));


	// 2초마다 할일
	if (nCount % 2 == 0)
	{
		// 설비 재접속
		pDoc->ReconnectServer();
//		pDoc->m_strAlterLocation = "1000102";
//		pDoc->m_nPrevLuggNum = 1004;

		SET(IDC_RECEIVE_STATUS, pDoc->m_bReceiveStatus ? _T("OK") : _T("미수신"));

		// 시작/종료 버튼 상태 갱신
		// - 첫 상태전문 수신 전 : 둘다 비활성
		// - 첫 상태전문 수신 후 : 선택된 로직그룹의 m_bStart에 따름
		if (pDoc->m_bReceiveStatus == FALSE)
		{
			ENABLE_WND(IDC_BTN_START, FALSE);
			ENABLE_WND(IDC_BTN_END, FALSE);
		}
		else if (pDoc->m_pConfig != NULL && pDoc->m_pConfig->m_nLogicGroupCnt > 0)
		{
			int nSelLogicGroup = (int)m_cbxLogicGroup.GetItemData(m_cbxLogicGroup.GetCurSel());
			if (nSelLogicGroup >= 1 && nSelLogicGroup <= pDoc->m_pConfig->m_nLogicGroupCnt)
			{
				BOOL bGroupStart = pDoc->m_pLogicGorupInfos[nSelLogicGroup - 1]->m_bStart;
				ENABLE_WND(IDC_BTN_START, !bGroupStart);
				ENABLE_WND(IDC_BTN_END, bGroupStart);
			}
		}

		if (pDoc->m_bReceiveStatus == TRUE)
		{
			if (pDoc != NULL && pDoc->m_pConfig != NULL && pDoc->m_pConfig->m_nLogicGroupCnt > 0)
			{
				SJobInvokeInfo* pJobInvokeInfo = NULL;
				CString strTemp;
				for (int i = 0; i < pDoc->m_pConfig->m_nLogicGroupCnt; i++)
				{
					//pJobInvokeInfo = pDoc->m_pJobInvokeInfos.GetAt(i);
					//pDoc->m_pJobInvokeInfos[i]->m_bStart;

					if (pDoc->m_pLogicGorupInfos[i]->m_bStart)
					{
						if (pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect())
						{

							/*	// 이거는 KCTC 버전인듯

							int nCnt = 0;//m_nScJobCnt;		// SC 당 작업 개수 

							// SC 별로 루핑한다. 
							int nStationNum = 0;
							CStationInfo* pStation = NULL;
							int nStnCnt = pDoc->m_pJobInvokeInfos[i]->m_strStoStations.GetCount();
							for (int i = 0; i < nStnCnt; ++i)
							{
								pStation = pDoc->m_pStationInfos[i];
								if (pStation == NULL)
									continue;

								if (pStation->m_enKind != CStationInfo::enScStation)
									continue;

								nStationNum = atoi(pStation->m_strID);

								// 각 SC 별 작업 정보 갯수를 가져온다. 
								nCnt = pDoc->m_pJob->GetCountByScNum(nStationNum);

								// SC당 작업 갯수 보다 작을 경우...
								if (m_nScJobCnt > nCnt)
								{
									if (m_nMode == 0)
									{
										////// 회수모드 => 이동(KCTC:56) 작업 생성 
										if (pDoc->m_pHostCl->JobOrder(56, nStationNum, strEquipKind) > 0)
										{
											m_lstHostCl.InsertString(0, pDoc->m_pHostCl->m_JobOrderMsg);
										}
									}
									else
									{
										////// 불출모드 => 출고(KCTC:52) 작업 생성 
										if (pDoc->m_pHostCl->JobOrder(52, nStationNum, strEquipKind) > 0)
										{
											m_lstHostCl.InsertString(0, pDoc->m_pHostCl->m_JobOrderMsg);
										}
									}
								}
								else
								{
									CJobItem* pJobItem = NULL;
									// 진행중인 작업구분이 끝난 것이 있는지 체크하여 끝났으면 다음 작업구분을 생성한다.  - Check
									if (m_nMode == 0)
									{
										////// 회수모드 => 이동(KCTC:56) 작업 완료 체크 - 완료시 이동(KCTC:57) 작업 생성   
										pJobItem = pDoc->m_pJob->GetJobCompleteInStep(nStationNum, 1, m_nMode + 1);
										if (pJobItem != NULL)
										{
											pJobItem->m_ucCrateKind = 2;										// 완료 차수 초기화 
											pDoc->m_nPrevLuggNum = pJobItem->m_nLuggNum;						// LuggNum을 미리 입력 
											if (pDoc->m_pHostCl->JobOrder(57, nStationNum, strEquipKind) > 0)
											{
												m_lstHostCl.InsertString(0, pDoc->m_pHostCl->m_JobOrderMsg);
											}
										}

										////// 회수모드 => 이동(KCTC:57) 작업 완료 체크 - 완료시 입고(KCTC:51) 작업 생성   
										pJobItem = pDoc->m_pJob->GetJobCompleteInStep(nStationNum, 3, m_nMode + 1);
										if (pJobItem != NULL)
										{
											pJobItem->m_ucCrateKind = 0;										// 완료 차수 초기화 
											pDoc->m_nPrevLuggNum = pJobItem->m_nLuggNum;						// LuggNum을 미리 입력 
											if (pDoc->m_pHostCl->JobOrder(51, nStationNum, strEquipKind) > 0)
											{
												m_lstHostCl.InsertString(0, pDoc->m_pHostCl->m_JobOrderMsg);
											}
										}
									}
									else
									{
										////// 불출모드 => 출고(KCTC:52) 작업 완료 체크 - 완료시 이동(KCTC:58) 작업 생성  
										pJobItem = pDoc->m_pJob->GetJobCompleteInStep(nStationNum, 1, m_nMode + 1);

										if (pJobItem != NULL)
										{
											pJobItem->m_ucCrateKind = 0;										// 완료 차수 초기화 
											pDoc->m_nPrevLuggNum = pJobItem->m_nLuggNum;						// LuggNum을 미리 입력 

											if (pDoc->m_pHostCl->JobOrder(58, nStationNum, strEquipKind) > 0)
											{
												pJobItem->SetJobStatus(enJobStatusCvInvoke);
												m_lstHostCl.InsertString(0, pDoc->m_pHostCl->m_JobOrderMsg);
											}

											int a = 0;
										}
									}
								}
							}
							//*/
							int a = 0;
							int nStoStnCnt = pDoc->m_pLogicGorupInfos[i]->m_nStoStnCnt;
							for (int j = 0; j < nStoStnCnt; j++)
							{
								CString strStoStnNum = pDoc->m_pLogicGorupInfos[i]->m_strStoStations.GetAt(j);
								CString strViaStnNum = pDoc->m_pLogicGorupInfos[i]->m_strViaStations.GetAt(j);
								CString strRetStnNum = pDoc->m_pLogicGorupInfos[i]->m_strRetStations.GetAt(j);


								int nCurrWorkingLuggNum = pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum;

								// 이거는 유유제약 버전인듯
								if (pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum == 0)
								{
									int nJobType = 6;
									int nLuggNum = pDoc->m_pHostCl->JobOrder(6, _ttoi(strStoStnNum), _ttoi(strViaStnNum));
									CString strTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));

									LPCTSTR strTempTemp = (LPCTSTR)pDoc->m_pHostCl->m_JobOrderMsg;
									m_lstHostCl.InsertString(0, pDoc->m_pHostCl->m_JobOrderMsg);


									int nLogicGroup = (int)m_cbxLogicGroup.GetItemData(m_cbxLogicGroup.GetCurSel());

									if (nLogicGroup - 1 == i)
									{
										//SET(IDC_DUAL_STO_LOC, pDoc->m_pLogicGorupInfos[i]->m_strAlterLocation);
										////SET_INT(IDC_DUAL_STO_LUGG, pDoc->m_pJobInvokeInfos[i]->m_nPrevLuggNum);

										//if (pDoc->m_pLogicGorupInfos[i]->m_strAlterLocation != "")
										//{
										//	ENABLE_WND(IDC_BTN_ALTER_LOC, TRUE);
										//	ENABLE_WND(IDC_DUAL_STO_LOC, TRUE);
										//	ENABLE_WND(IDC_STC_DUAL_STO_LOC, TRUE);
										//}

										// 이동 작업 생성 - 작업중인 작업번호 SETTING 
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum = nLuggNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingJobType = nJobType;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strFromPos = strStoStnNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strToPos = strViaStnNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strTime = strTime;

										// 리스트에 작업 추가 - 완료될때 리스트에 작업이 삭제 되는것도 처리해야함!
										UpdateList(m_lstThisLogicJob, __FUNCTION__, pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]);

										//SET_INT(IDC_WORKING_LUGG1, pDoc->m_pJobInvokeInfos[i]->m_nWorkingLuggNum);
									}

									else
									{
										// 선택된 그룹뿐 아니라 모든 로직그룹의 작업상태를 저장해야 동시 구동이 가능함
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum = nLuggNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingJobType = nJobType;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strFromPos = strStoStnNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strToPos = strViaStnNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strTime = strTime;
									}
								}
								else
								{
									BOOL bComplete = FALSE;
									// 이동작업이 완료 되었으면 
									if(pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_bCompleteMove == TRUE)
									{
										int nJobType = 1;
										// 입고 랙 뱅크 : 이 로직그룹의 ScNum 목록 내에서만 선정
										int nLuggNum = pDoc->m_pHostCl->JobOrder(nJobType, _ttoi(strViaStnNum), 0, FALSE, NULL, &pDoc->m_pLogicGorupInfos[i]->m_strScs);
										CString strTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));

										// 입고 작업 생성 - 작업중인 작업번호 UPDATE 
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum = nLuggNum;

										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_bCompleteMove = FALSE;
										LPCTSTR strTempTemp = (LPCTSTR)pDoc->m_pHostCl->m_JobOrderMsg;
										m_lstHostCl.InsertString(0, strTempTemp);


										// 입고 작업 생성 - 작업중인 작업번호 SETTING 
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum = nLuggNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingJobType = nJobType;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strFromPos = strViaStnNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strToPos = pDoc->m_strStoLocation[nLuggNum];
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strTime = strTime;

										// 리스트에 작업 추가 - 완료될때 리스트에 작업이 삭제 되는것도 처리해야함!
										UpdateList(m_lstThisLogicJob, __FUNCTION__, pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]);
										bComplete = TRUE;

									}

									if (pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_bCompleteStore == TRUE)
									{
										int nJobType = 2;
										// 출고 출발지 : 입고 완료된 작업의 랙 위치(m_strToPos)
										int nLuggNum = pDoc->m_pHostCl->JobOrder(nJobType, _ttoi(strRetStnNum), 0, FALSE, pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strToPos, NULL);
										CString strTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));

										// 출고 작업 생성 - 작업중인 작업번호 UPDATE 
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum = nLuggNum;

										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_bCompleteStore = FALSE;
										LPCTSTR strTempTemp = (LPCTSTR)pDoc->m_pHostCl->m_JobOrderMsg;
										m_lstHostCl.InsertString(0, strTempTemp);

										// 출고 작업 생성 - 작업중인 작업번호 SETTING 
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum = nLuggNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_nWorkingJobType = nJobType;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strFromPos = pDoc->m_strRetLocation[nLuggNum];
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strToPos = strRetStnNum;
										pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]->m_strTime = strTime;

										// 리스트에 작업 추가 - 완료될때 리스트에 작업이 삭제 되는것도 처리해야함!
										UpdateList(m_lstThisLogicJob, __FUNCTION__, pDoc->m_pLogicGorupInfos[i]->m_pJobInvokeInfos[j]);
										bComplete = TRUE;

									}

									//// 종료버튼 누를때 있는 리스트도 다 지워야 함!
									//if (bComplete == FALSE)
									//{
									//	// 완료 처리를 못했으면 리스트 갯수를 확인하여 리스트가 없으면 만들어낼 것!
									//}
								}
							}
							//	////// 회수모드 => 이동(KCTC:56) 작업 생성 
							//	if (pDoc->m_pHostCl->JobOrder(56, nStationNum) > 0)
							//	{
							//		//m_lstHostCl.InsertString(0, pDoc->m_pHostCl->m_JobOrderMsg);
							//	}
							//
							//	// 입고 작업 생성 - 작업중인 작업번호 SETTING 
							//	pDoc->m_pJobInvokeInfos[i]->m_nWorkingLuggNum = pDoc->m_pHostCl->JobOrder(1);
							//}
							//else
							//{
							//	//pDoc->m_bCompleteStore1 = TRUE;
							//	if (pDoc->m_pJobInvokeInfos[i]->m_bCompleteStore == TRUE)
							//	{
							//		// 출고 작업 생성 - 작업중인 작업번호 UPDATE 
							//		pDoc->m_pJobInvokeInfos[i]->m_nWorkingLuggNum = pDoc->m_pHostCl->JobOrder(2);
							//
							//		pDoc->m_pJobInvokeInfos[i]->m_bCompleteStore = FALSE;
							//	}
							//}
							//*/
						}
					}

				}
			}
		}
		/*
		if (m_bTesting1 == TRUE)
		{
			SET(IDC_DUAL_STO_LOC, pDoc->m_strAlterLocation);
			SET_INT(IDC_DUAL_STO_LUGG, pDoc->m_nPrevLuggNum);

			if (pDoc->m_strAlterLocation != "")
			{
				ENABLE_WND(IDC_BTN_ALTER_LOC, TRUE);
				ENABLE_WND(IDC_DUAL_STO_LOC, TRUE);
				ENABLE_WND(IDC_STC_DUAL_STO_LOC, TRUE);

			}

			SET_INT(IDC_WORKING_LUGG1, pDoc->m_nWorkingLuggNum1);
			
			//SET(IDC_RECEIVE_STATUS, pDoc->m_bReceiveStatus ? "OK" : "미수신");

			if (pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()) 
			{
				//int nJobCnt = m_pJob->GetCount();
				
				if (pDoc->m_nWorkingLuggNum1 == 0)
				{
					// 입고 작업 생성 - 작업중인 작업번호 SETTING 
					//BOOL CHostCl::JobOrder(int nJobType, CString strLocation, CString strStation, BOOL bManual)
					pDoc->m_nWorkingLuggNum1 = pDoc->m_pHostCl->JobOrder(1);
				}
				else 
				{
					//pDoc->m_bCompleteStore1 = TRUE;
					if (pDoc->m_bCompleteStore1 == TRUE)
					{
						// 출고 작업 생성 - 작업중인 작업번호 UPDATE 
						pDoc->m_nWorkingLuggNum1 = pDoc->m_pHostCl->JobOrder(2);

						pDoc->m_bCompleteStore1 = FALSE;
					}
				}
			}
		}

		if (m_bTesting2 == TRUE)
		{
			SET(IDC_DUAL_STO_LOC, pDoc->m_strAlterLocation);
			SET_INT(IDC_DUAL_STO_LUGG, pDoc->m_nPrevLuggNum);

			if (pDoc->m_strAlterLocation != "")
			{
				ENABLE_WND(IDC_BTN_ALTER_LOC, TRUE);
				ENABLE_WND(IDC_DUAL_STO_LOC, TRUE);
				ENABLE_WND(IDC_STC_DUAL_STO_LOC, TRUE);

			}

			SET_INT(IDC_WORKING_LUGG1, pDoc->m_nWorkingLuggNum2);
			
			//SET(IDC_RECEIVE_STATUS, pDoc->m_bReceiveStatus ? "OK" : "미수신");

			if (pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()) 
			{
				//int nJobCnt = m_pJob->GetCount();
				
				if (pDoc->m_nWorkingLuggNum2 == 0)
				{
					// 입고 작업 생성 - 작업중인 작업번호 SETTING 
					//BOOL CHostCl::JobOrder(int nJobType, CString strLocation, CString strStation, BOOL bManual)
					pDoc->m_nWorkingLuggNum2 = pDoc->m_pHostCl->JobOrder(1);
				}
				else 
				{
					//pDoc->m_bCompleteStore1 = TRUE;
					if (pDoc->m_bCompleteStore2 == TRUE)
					{
						// 출고 작업 생성 - 작업중인 작업번호 UPDATE 
						pDoc->m_nWorkingLuggNum2 = pDoc->m_pHostCl->JobOrder(2);

						pDoc->m_bCompleteStore2 = FALSE;
					}
				}
			}
		}
		//*/
	}

	if (++nCount % 3600 == 0)		// 한시간마다 타이머 리셋
	{
		nCount = 0;
	}


	CFormView::OnTimer(nIDEvent);
}

void CEcsView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	KillTimer(enTimerStatusDisplay);
	KillTimer(enTimerReconnect);
	KillTimer(enTimerStatusReport);
	KillTimer(enTimerCheckIdleJob);
}

/////////////////////////////////////////////////////////////////////////////
// CEcsView printing

BOOL CEcsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEcsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEcsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CEcsView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
}

void CEcsView::OnDraw(CDC* pDC) 
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	CEcsLayout layout;

	switch (m_tabLayout.GetCurSel())
	{
	case en1Floor:			pDoc->m_layout.OnDraw(this, pDC);			break;
	case en2Floor:			pDoc->m_layout2.OnDraw(this, pDC);			break;
	default:				DEBUGER_ASSERT_VALID(FALSE);				break;
	}

//	layout.OnDraw(this, pDC);

	CString strText;
	CRect rcRectArray[2];
	CTrackInfo* pTrack = NULL;
	CDciMaster* pDCI = layout.GetDciMaster();
	if (pDCI == NULL)
		return;

	int i = 0;
	for ( i=0; i<2; ++i)
	{
		pTrack = pDoc->GetTrackInfo(1307+i);
		if ((pTrack == NULL) || (pTrack->m_pTrackCtrl == NULL))
			continue;

		strText.Format(_T("%d"), pTrack->GetLgvTaskPriority());
		rcRectArray[i] = pTrack->m_pTrackCtrl->m_rcControlL;
		rcRectArray[i].bottom += 1;
		rcRectArray[i].top += 1;

		pDCI->DrawText(pDC, rcRectArray[i], strText, RGB(0,0,0));		
	}

	for (i=0; i<2; ++i)
	{
		pTrack = pDoc->GetTrackInfo(1357+i);
		if ((pTrack == NULL) || (pTrack->m_pTrackCtrl == NULL))
			continue;

		strText.Format(_T("%d"), pTrack->GetLgvTaskPriority());
		rcRectArray[i] = pTrack->m_pTrackCtrl->m_rcControlL;
		rcRectArray[i].bottom -= 1;
		rcRectArray[i].top -= 1;

		pDCI->DrawText(pDC, rcRectArray[i], strText, RGB(0,0,0));		
	}
}

void CEcsView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	int width = cx;
	int height = cy;

	CMainFrame * pMainFrm = (CMainFrame*)AfxGetMainWnd();

	if (pMainFrm != NULL)
		pMainFrm->DockControlBarRight(&pMainFrm->m_wndToolBar[2], &pMainFrm->m_wndToolBar[1]);

	if (m_tabLayout.GetSafeHwnd())
	{
		m_tabLayout.MoveWindow(0, 0, cx, 19);

		switch (m_tabLayout.GetCurSel())
		{
		case en1Floor:			pDoc->m_layout.OnSize(this, nType, cx, cy);;			break;
		case en2Floor:			pDoc->m_layout2.OnSize(this, nType, cx, cy);;			break;
		}
	}
}

void CEcsView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	if (m_tabLayout.GetSafeHwnd())
	{
		switch (m_tabLayout.GetCurSel())
		{
		case en1Floor:			pDoc->m_layout.OnButtonDown(this, nFlags, point);			break;
		case en2Floor:			pDoc->m_layout2.OnButtonDown(this, nFlags, point);			break;
		default:				DEBUGER_ASSERT_VALID(FALSE);		break;
		}
	}
	
	CFormView::OnLButtonDown(nFlags, point);
}

void CEcsView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	if (m_tabLayout.GetSafeHwnd())
	{
		switch (m_tabLayout.GetCurSel())
		{
		case en1Floor:			pDoc->m_layout.OnButtonUp(this, nFlags, point);			break;
		case en2Floor:			pDoc->m_layout2.OnButtonUp(this, nFlags, point);			break;
		}
	}
	
	CFormView::OnLButtonUp(nFlags, point);
}

void CEcsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	if (m_tabLayout.GetSafeHwnd())
	{
		switch (m_tabLayout.GetCurSel())
		{
		case en1Floor:			pDoc->m_layout.OnKeyDown(this, nChar, nRepCnt, nFlags);			break;
		case en2Floor:			pDoc->m_layout2.OnKeyDown(this, nChar, nRepCnt, nFlags);			break;
		}
	}

	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// CEcsView diagnostics

#ifdef _DEBUG
void CEcsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CEcsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEcsDoc* CEcsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEcsDoc)));
	return (CEcsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEcsView message handlers

char CEcsView::ConvertNotifyToStatus(int nNotify) 
{
	switch (nNotify)
	{   
	case	NOTIFY_SEND:				// 2
		return COMM_STA_SEND;			// 2
	case	NOTIFY_RECEIVE:				// 3
	case	NOTIFY_REFRESH:				// 1
		return COMM_STA_RECEIVE;		// 3
	case	NOTIFY_ERROR:				// 4
	default:
		return COMM_STA_STOP;			// 1
	}
}

LRESULT CEcsView::OnHostNotify(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
//	ASSERT(pWnd != NULL);

	if (pWnd == NULL)
		return 0;

	if (lParam != 1 && lParam != 2 && lParam != 3)
		int a=0;

	pWnd->UpdateCommStatus(1, ID_COM_HOST_SERVER+wParam, ConvertNotifyToStatus(lParam));

	return 0;
}

LRESULT CEcsView::OnCvNotify(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
//	ASSERT(pWnd != NULL);

	if (pWnd == NULL)
		return 0;

	pWnd->UpdateCommStatus(1, ID_COM_CV01+wParam, ConvertNotifyToStatus(lParam));

	return 0;
}

LRESULT CEcsView::OnScNotify(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
//	ASSERT(pWnd != NULL);

	if (pWnd == NULL)
		return 0;

	pWnd->UpdateCommStatus(1, ID_COM_SC1_W1+wParam, ConvertNotifyToStatus(lParam));

	return 0;
}

LRESULT CEcsView::OnRtvNotify(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
//	ASSERT(pWnd != NULL);

	if (pWnd == NULL)
		return 0;

	pWnd->UpdateCommStatus(1, ID_COM_RTV1+wParam, ConvertNotifyToStatus(lParam));

	return 0;
}

LRESULT CEcsView::OnBcrNotify(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
//	ASSERT(pWnd != NULL);

	if (pWnd == NULL)
		return 0;

	pWnd->UpdateCommStatus(1, ID_COM_BCR1+wParam, ConvertNotifyToStatus(lParam));

	return 0;
}

LRESULT CEcsView::OnControlClick(WPARAM wParam, LPARAM lParam)
{
	CString strCID;
	strCID.Format(_T("%u"), wParam);
	if (CLib::IsValidControlID(strCID) == FALSE)
		return 0;

	int nWarehouse = CLib::GetWarehouse(strCID);
	int nEquipKind = CLib::GetEquipment(strCID);
	int nEquipNum = CLib::GetEquipNum(strCID);
	int nTrackNum = nEquipNum;// + (nWarehouse-1)*1000;

	int nSCsNum = 0;
	int nRTVsNum = 0;
	int nPlcNum = int(nTrackNum / 1000);

	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return 0;

	switch (nEquipKind)
	{
	case CEquipment::enCV:
		{
			
//			pDoc->m_oCvDlg.m_pDoc = pDoc;
//			pDoc->m_oCvDlg.m_pTrack = pDoc->GetTrackInfo(nTrackNum);
//			DEBUGER_ASSERT_VALID(pDoc->m_oCvDlg.m_pTrack != NULL);
//			pDoc->m_oCvDlg.DoModal();

			// TEST
//			pDoc = NULL;
			
			// 경광등을 클릭하면 CV 창을 띄우면 안됨!
			if (nPlcNum != 4)
			{
				pDoc->m_oCvDlg.m_pDoc = GetDocument();
				pDoc->m_oCvDlg.m_pTrack = pDoc->GetTrackInfo(nTrackNum);

				if (pDoc->m_oCvDlg.m_hWnd == NULL) 
				{
					pDoc->m_oCvDlg.Create(IDD_CV_CTRL1);
				 
					CRect MainRect;
					CRect Rect;
					CRect PosRect;

					::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
					pDoc->m_oCvDlg.GetWindowRect(&Rect); 

					PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
					PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
					pDoc->m_oCvDlg.SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
												Rect.Width(), Rect.Height(), 
												SWP_SHOWWINDOW);

				} 
				else 
				{
					if (!pDoc->m_oCvDlg.IsWindowVisible()) 
						pDoc->m_oCvDlg.ShowWindow(SW_SHOWNORMAL);

					pDoc->m_oCvDlg.SendMessage(WM_INITDIALOG);   
				}
			}
			else
			{
				
			}
		}
		break;

	case CEquipment::enSC:
		{
			switch(nEquipNum) 
			{
			case 1:				
			case 2:				
			case 3:				nSCsNum = 1;				break;
			case 4:				
			case 5:				
			case 6:				nSCsNum = 2;				break;
			default:										break;
			}
			CEquipment* pEquipment = pDoc->GetEquipment(nEquipKind, nSCsNum);
//			DEBUGER_ASSERT_VALID(pEquipment != NULL);

			if (pEquipment == NULL)
				return 0;

			// 다시볼것!!
			CScDongJin* pSc = (CScDongJin*)pEquipment;
			CScInfo* pInfo = (CScInfo *)pSc->GetInfo(nEquipNum, FALSE);
//			DEBUGER_ASSERT_VALID(pInfo != NULL);
			
			if (pInfo == NULL)
				return 0;

//			CScDongJinDlg dlg((CScDongJin*)pEquipment, nEquipNum, this);
//			dlg.DoModal();
			pDoc->m_oScDlg.m_pSc = (CScDongJin*)pEquipment;
			pDoc->m_oScDlg.m_nEquipNum = pDoc->m_oScDlg.m_pSc->m_nNumber;
			pDoc->m_oScDlg.m_pScInfo = pInfo;
			pDoc->m_oScDlg.m_bRetSuspend = pInfo->m_bRetSuspend;
			pDoc->m_oScDlg.m_bStoSuspend = pInfo->m_bStoSuspend;
			pDoc->m_oScDlg.m_bR2RSuspend = pInfo->m_bR2RSuspend;

			if (pDoc->m_oScDlg.m_hWnd == NULL) 
			{
				pDoc->m_oScDlg.Create(IDD_SC_CTRL1);
			 
				CRect MainRect;
				CRect Rect;
				CRect PosRect;

				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_oScDlg.GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_oScDlg.SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
											Rect.Width(), Rect.Height(), 
											SWP_SHOWWINDOW);

			} 
			else 
			{
				if (!pDoc->m_oScDlg.IsWindowVisible()) 
					pDoc->m_oScDlg.ShowWindow(SW_SHOWNORMAL);

				pDoc->m_oScDlg.SendMessage(WM_INITDIALOG);   
			}

		}
		break;

	case CEquipment::enRTV:
		{
			int nRgvcNum = nEquipNum / 100;
			int nCarNum = nEquipNum % 100;
			CEquipment* pEquipment = pDoc->GetEquipment(CEquipment::enRGV, nRgvcNum);

			if (pEquipment == NULL)
				return 0;

//			CLgvDlg dlg((CLgv*)pEquipment, nCarNum, this);
//			dlg.DoModal();
			pDoc->m_oRgvDlg.m_pLgv = (CLgv*)pEquipment;
			pDoc->m_oRgvDlg.m_nSelectCar = nCarNum;
			pDoc->m_oRgvDlg.m_bSuspend = pDoc->m_oRgvDlg.m_pLgv->m_pInfo->m_bSuspend;

			if (pDoc->m_oRgvDlg.m_hWnd == NULL) 
			{
				pDoc->m_oRgvDlg.Create(IDD_LGV_CTRL1);
			 
				CRect MainRect;
				CRect Rect;
				CRect PosRect;

				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_oRgvDlg.GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_oRgvDlg.SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
											Rect.Width(), Rect.Height(), 
											SWP_SHOWWINDOW);

			} 
			else 
			{
				if (!pDoc->m_oRgvDlg.IsWindowVisible()) 
					pDoc->m_oRgvDlg.ShowWindow(SW_SHOWNORMAL);

				pDoc->m_oRgvDlg.SendMessage(WM_INITDIALOG);   
			}

		}
		break;

	case CEquipment::enRGV: // LGV
		{
			int nRgvcNum = nEquipNum / 10;
			CEquipment* pEquipment = pDoc->GetEquipment(nEquipKind, nEquipNum);

			if (pEquipment == NULL)
				return 0;

//			CLgvcDlg dlg((CLgv*)pEquipment, nEquipNum, this);
//			dlg.DoModal();

			pDoc->m_oRgvcDlg.m_pLgv = (CLgv*)pEquipment;
			pDoc->m_oRgvcDlg.m_nSelectLgv = nEquipNum;

			if (pDoc->m_oRgvcDlg.m_hWnd == NULL) 
			{
				pDoc->m_oRgvcDlg.Create(IDD_LGVC_CTRL);
			 
				CRect MainRect;
				CRect Rect;
				CRect PosRect;

				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_oRgvcDlg.GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_oRgvcDlg.SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
											Rect.Width(), Rect.Height(), 
											SWP_SHOWWINDOW);

			} 
			else 
			{
				if (!pDoc->m_oRgvcDlg.IsWindowVisible()) 
					pDoc->m_oRgvcDlg.ShowWindow(SW_SHOWNORMAL);

				pDoc->m_oRgvcDlg.SendMessage(WM_INITDIALOG);   
			}
		}
		break;
	case CEquipment::enLampButton:
		{
			CCvFx* pCv = (CCvFx*)pDoc->GetEquipment(CEquipment::enCV, 12);
			if (pCv == NULL || pCv->m_pInfo == NULL)
				return 0;

			BOOL bBit = !CLib::IsSet(pCv->m_pInfo->m_wCurrLampStatus, nEquipNum-1);
			CLib::SetBit(pCv->m_pInfo->m_wCurrLampStatus, nEquipNum-1, bBit);
		}
		break;
	}

	return 0;
}

LRESULT CEcsView::OnAlarmNotify(WPARAM wParam, LPARAM lParam) 
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return 0;

	if ((pDoc->m_pWarningDlg != NULL) && (pDoc->m_pWarningDlg->m_bShow == FALSE))
		return 0;

	pDoc->OnWarning();

	return 0;
}

void CEcsView::OnViewLayoutInfo() 
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	pDoc->m_layout.OnViewLayoutInfo(this, pDoc->m_pConfig->m_nFontSize);
//	if (m_tabLayout.GetSafeHwnd())
//	{
//		switch (m_tabLayout.GetCurSel())
//		{
//		case en1Floor:			pDoc->m_layout.OnViewLayoutInfo(this);			break;
//		case en2Floor:			pDoc->m_layout2.OnViewLayoutInfo(this);			break;
//		}
//	}
}
void CEcsView::OnInputMessage() 
{
//	CEcsDoc* pDoc = GetDocument();
//
//	if (pDoc == NULL)
//		return;
//
//	pDoc->SetAllSetSusspend();

//	CDataSettingDlg::CDataSettingDlg(CString strData, CString strCaption, CWnd* pParent /*=NULL*/)
	CDataSettingDlg DSDlg(_T("메세지를 입력해주세요."), _T("메세지 입력"));
	if (DSDlg.DoModal() != IDOK)
		return;
	
	CMessageDisplayDlg MDDlg(_T(""));
	MDDlg.m_strMessage =  DSDlg.m_strData;	
	MDDlg.DoModal();
}

void CEcsView::OnSetAllSuspend() 
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	pDoc->SetAllSetSusspend();
}

void CEcsView::OnUnSetAllSuspend() 
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	pDoc->SetAllUnSetSusspend();
}

void CEcsView::UpdateRtvInfo(int nCarNum) 
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

//	pDoc->SetAllUnSetSusspend();

	if (pDoc == NULL)
		return;

	int nRTVsNum = 1;
	if (nCarNum > 5)
		nRTVsNum = 2;

	CEquipment* pEquipment = pDoc->GetEquipment(CEquipment::enRGV, nRTVsNum);
//	DEBUGER_ASSERT_VALID(pEquipment != NULL);

	if (pEquipment == NULL)
		return;

	CLgv* pRtv = (CLgv*)pEquipment;
	CLgvInfo* pInfo = (CLgvInfo *)pRtv->GetInfo();
//	DEBUGER_ASSERT_VALID(pInfo != NULL);
	
	if (pInfo == NULL)
		return;

//	pRtv->m_pInfo->m_bInvoke = FALSE;

	CDciStaticCtrl* pJobStaticCtrl = NULL;
	CDciStaticCtrl* pPathStaticCtrl = NULL;
	CLgvVehicle* pRgvVehicle = NULL;
//	CLgvJobItem* pRgvJobItem = NULL;
	CJobItem* pJobItem = NULL;
	int nVehicleNum = 0;
	int nCarCnt = pInfo->m_oLgvVehicles.GetSize();
	for (int i=0;i<nCarCnt;i++)
	{
		nVehicleNum = i;
//		if (nCarNum > 5)
//			nVehicleNum += 5;
			
//		pRgvVehicle = pInfo->m_oLgvVehicles.GetAt(i);
		pRgvVehicle = pInfo->GetLgvVehicle(nVehicleNum + 1);

		if (pRgvVehicle == NULL)
			continue;

		if(nRTVsNum == 1)
		{
			pJobStaticCtrl = pDoc->m_pRtvJobNums1[nVehicleNum];
			pPathStaticCtrl = pDoc->m_pRtvSrcDsts1[nVehicleNum];
		}
		else
		{
			pJobStaticCtrl = pDoc->m_pRtvJobNums2[nVehicleNum];
			pPathStaticCtrl = pDoc->m_pRtvSrcDsts2[nVehicleNum];
		}

			
		pJobStaticCtrl->m_strText.Format(_T("%d"), pRgvVehicle->m_nLuggNum);
//		pRgvJobItem = pInfo->GetLgvJobItem(pRgvVehicle->m_nLuggNum);
		pJobItem = (CJobItem*)pDoc->m_pJob->Find(pRgvVehicle->m_nLuggNum); 

		if (pJobItem != NULL)
		{
			pPathStaticCtrl->m_strText.Format(_T("TR#%d / TR#%d"),
				pJobItem->m_nTempDepartTrackNum, 
				pJobItem->m_nTempArriveTrackNum);

			pRtv->m_pInfo->m_bInvoke = TRUE;

		}
		else
		{
			pPathStaticCtrl->m_strText = _T("0 / 0");
		}

		pJobStaticCtrl->InvalidateControl(pDoc->m_hWndView, FALSE);
		pPathStaticCtrl->InvalidateControl(pDoc->m_hWndView, FALSE);
	}

	if (pDoc->m_pJob->FetchByStatusNRgvNum(enJobStatusRtvInvoke, nRTVsNum) == NULL)
	{
		if (pRtv->m_pInfo->m_bInvoke == TRUE)
			pRtv->m_pInfo->m_bModified = TRUE;

		pRtv->m_pInfo->m_bInvoke = FALSE;	
	}
	else
	{
		if (pRtv->m_pInfo->m_bInvoke == FALSE)
			pRtv->m_pInfo->m_bModified = TRUE;

		pRtv->m_pInfo->m_bInvoke = TRUE;	
	}

	pRtv->m_pInfo->InvokeControl();

	pRtv->MulticastInfo(NULL);

//==========================================================================================================
//	DOOR 상태 표시
//==========================================================================================================
//	int nCvNum = 0;	
//	int nPlcGroup = GET_INT(IDC_PLC_GROUP);
//
//	switch(nPlcGroup) 
//	{
//	case 2:		nCvNum = 4;		break;
//	case 3:		nCvNum = 10;	break;
//	}
//
	CString strTemp;
	CCv* pCv2 = (CCv*) pDoc->GetEquipment(CEquipment::enCV, 4);
	if (pCv2 == NULL)
		return;

	SET_INT(IDC_DOOR_STATUS,	pCv2->m_pInfo->m_wDoorStatus		);
	SET_INT(IDC_OPEN_REQUEST,	pCv2->m_pInfo->m_wDoorOpenReq		);
	SET_INT(IDC_OPEN_ACCEPT,	pCv2->m_pInfo->m_wDoorOpenAccept	);
	SET_INT(IDC_CLOSE_REPORT,	pCv2->m_pInfo->m_wDoorClose			);
	SET_INT(IDC_RGV_STATUS,		pCv2->m_pInfo->m_wRgvStatus			);

	CCv* pCv3 = (CCv*) pDoc->GetEquipment(CEquipment::enCV, 10);
	if (pCv3 == NULL)
		return;

	SET_INT(IDC_DOOR_STATUS2,	pCv3->m_pInfo->m_wDoorStatus		);
	SET_INT(IDC_OPEN_REQUEST2,	pCv3->m_pInfo->m_wDoorOpenReq		);
	SET_INT(IDC_OPEN_ACCEPT2,	pCv3->m_pInfo->m_wDoorOpenAccept	);
	SET_INT(IDC_CLOSE_REPORT2,	pCv3->m_pInfo->m_wDoorClose			);
	SET_INT(IDC_RGV_STATUS2,	pCv3->m_pInfo->m_wRgvStatus			);

	GetDlgItem(IDC_STATIC_LAMP)->ShowWindow(pDoc->m_pConfig->m_bViewLampControler);
	GetDlgItem(IDC_REQUST_LAMP)->ShowWindow(pDoc->m_pConfig->m_bViewLampControler);
	GetDlgItem(IDC_CHECK_LAMP)->ShowWindow(pDoc->m_pConfig->m_bViewLampControler);

	GetDlgItem(IDC_STATIC_PRODUCT_INFO)->ShowWindow(pDoc->m_pConfig->m_bViewProductInfoInputer);
	GetDlgItem(IDC_REQUST_STATION)->ShowWindow(pDoc->m_pConfig->m_bViewProductInfoInputer);
	GetDlgItem(IDC_BUTTON9)->ShowWindow(pDoc->m_pConfig->m_bViewProductInfoInputer);
	GetDlgItem(IDC_BUTTON10)->ShowWindow(pDoc->m_pConfig->m_bViewProductInfoInputer);
//----------------------------------------------------------------------------------------------------------

}

void CEcsView::UpdateScRetCountInfo(int nScNum) 
{
	CEcsDoc* pDoc = GetDocument();
	if (pDoc == NULL)
		return;
	
	int SCS_PER_SC = 3;
	int nSCsNum = 1;
	if (nScNum > SCS_PER_SC)
		nSCsNum = 2;

	CEquipment* pEquipment = pDoc->GetEquipment(CEquipment::enSC, nSCsNum);
	if (pEquipment == NULL)
		return;

	CScDongJin* pSc = (CScDongJin*)pEquipment;
	if (pSc == NULL)
		return;

	CScInfo* pInfo;

	pInfo = (CScInfo *)pSc->GetInfo(nScNum, FALSE);
	if (pInfo == NULL)
		return;

	CDciStaticCtrl* pCountCtrl = NULL;

	// 전면부 표시 
	if (nSCsNum == 1)
	{
		pCountCtrl = pDoc->m_pScRetFronts[nScNum-1];
		if (pCountCtrl == NULL)
			return;

		pCountCtrl->m_clrFgColor = pDoc->m_pConfig->m_clrAutoR2R;		// 
		pCountCtrl->m_clrBgColor = pDoc->m_pConfig->m_clrAutoW2W;		// 
		pCountCtrl->m_strText.Format(_T("%02d개"), pDoc->m_nScRetFrontRemainJobCnt[nScNum-1]);
		pCountCtrl->InvalidateControl(pDoc->m_hWndView, FALSE);
	}

	// 후면부 표시 
	pCountCtrl = pDoc->m_pScRetRears[nScNum-1];
	if (pCountCtrl == NULL)
		return;

	pCountCtrl->m_clrFgColor = pDoc->m_pConfig->m_clrAutoR2R;		// 
	pCountCtrl->m_clrBgColor = pDoc->m_pConfig->m_clrAutoW2W;		// 
	pCountCtrl->m_strText.Format(_T("%02d개"), pDoc->m_nScRetRearRemainJobCnt[nScNum-1]);
	pCountCtrl->InvalidateControl(pDoc->m_hWndView, FALSE);
}

void CEcsView::OnUpdateViewLayoutInfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CEcsView::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	for (int i=0; i<10; ++i)
		strTemp.Format(_T("%d"),i);
	int a=1;
}

void CEcsView::OnButton2() 
{
	// TODO: Add your control notification handler code here
	
}

void CEcsView::OnButton3() 
{
	// TODO: Add your control notification handler code here
	
}

void CEcsView::OnButton4() 
{
	// TODO: Add your control notification handler code here
	
}


// 함수 사용안함 

void CEcsView::OnButton5() 
{
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	//pDoc->OnJobTestStart();
}

// 함수 사용안함 
void CEcsView::OnButton6() 
{
	
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;


	//	pDoc->OnJobTestStop();
	
}

void CEcsView::OnButton7() 
{
	// TODO: Add your control notification handler code here
	CString strDeviceKind, strDeviceNo, strErrorKind, strErrorCode, strLuggNo, strLocation, strWeight;

	int nTemp;
	GET(IDC_DEVICE_KIND,strDeviceKind);			nTemp = strDeviceKind.GetLength();			if (nTemp > 1)	return;	
	GET(IDC_DEVICE_NO,	strDeviceNo);			nTemp = strDeviceNo.GetLength();			if (nTemp > 4)	return;	
	GET(IDC_ERROR_KIND,	strErrorKind);			nTemp = strErrorKind.GetLength();			if (nTemp > 1)	return;	
	GET(IDC_ERROR_CODE,	strErrorCode);			nTemp = strErrorCode.GetLength();			if (nTemp > 4)	return;	
	GET(IDC_LUGG_NUM,	strLuggNo);				nTemp = strLuggNo.GetLength();				if (nTemp > 4)	return;	
	GET(IDC_LOCATION,	strLocation);			nTemp = strLocation.GetLength();			if (nTemp > 7)	return;	
	GET(IDC_WEIGHT,		strWeight);				nTemp = strWeight.GetLength();				if (nTemp > 20)	return;	

	if (strDeviceKind	== _T("") ||
		strDeviceNo		== _T("") ||
		strErrorKind	== _T("") ||
		strErrorCode	== _T("") ||
		strLuggNo		== _T("") ||
		strLocation		== _T("") ||
		strWeight		== _T("") )
	{
		AfxMessageBox(_T("입력 항목에 공백이 있습니다. \n입력 항목을 체크해주세요^^"));
		return;
	}


	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
	{
		pDoc->m_pHostCl->Error(_ttoi(strDeviceKind),_ttoi(strDeviceNo),_ttoi(strErrorKind),_ttoi(strErrorCode),_ttoi(strLuggNo), strLocation, strWeight);
	}

	
}

void CEcsView::OnButton8() 
{
	// TODO: Add your control notification handler code here
	CString strJobKind;
	GET(IDC_IN_OUT_PUT,		strJobKind);
	int nJobKind = _ttoi(strJobKind);

	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

//	int nTemp = nJobKind.GetLength();

//	if (nJobKind != 1 && nJobKind != 2)
//	{
//		AfxMessageBox("작업구분을 확인해 주세요^^");
//		return;
//	}

	UpdateData(FALSE);
	CStationInfo* pStation = NULL;

	int nTemp = m_cbxStation.GetCurSel();
	if (nTemp == -1)
	{
		AfxMessageBox(_T("Station 정보를 선택해 주세요!"));
		return;
	}

	pStation = (CStationInfo *) m_cbxStation.GetItemDataPtr(m_cbxStation.GetCurSel());

	if (pStation == NULL)
	{
		AfxMessageBox(_T("Station 정보를 선택해 주세요!"));
		return;
	}

	int nStation = _ttoi(pStation->m_strID);
	
	switch(nStation) 
	{
	case 108:
	case 109:
	case 110:		nJobKind = 2;		break;
	case 210:
	case 211:
	case 212:
	case 213:		nJobKind = 1;		break;
	default:							return;
	}

	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect() && nStation != 0)
		pDoc->m_pHostCl->JobRequest(nStation, nJobKind);
}

void CEcsView::UpdateStationInfo(int nWarehouse, CComboBox& cbxStation)
{
	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	if (!CJobItem::IsValidWarehouse(nWarehouse))
	{
		AfxMessageBox(_T("유효하지 않은 창고번호입니다!"));
		return;
	}

	int nStations[] = {101, 102, 103, 104, 105, 106, 108};
	int nStnCnt = 7;
	cbxStation.ResetContent();
	CString strTemp;
	CStationInfo* pStation = NULL;
	for (int i = 0 ; i< nStnCnt ; i++)
	{
		pStation = pDoc->GetStationInfo(CConvert::ToString(nStations[i]));

		if (pStation->GetWarehouseNo() != nWarehouse)
			continue;

		int nTemp = cbxStation.GetDlgCtrlID();

		strTemp.Format(_T("[TR#%d] [%d]  %s"),
			pStation->GetTrackDestination(), 
			nStations[i], 
			pStation->m_strName);

		m_cbxStation.AddString(strTemp);
		m_cbxStation.SetItemDataPtr(m_cbxStation.GetCount()-1, (void*)pStation);
	}
}

void CEcsView::OnSelchangeLayoutTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	CEcsDoc* pDoc = GetDocument();
//	DEBUGER_ASSERT_VALID(pDoc != NULL);

	if (pDoc == NULL)
		return;

	CRect rect;

	GetWindowRect(&rect);

	int h = rect.Height();
	int w = rect.Width();

	this->Invalidate();

	SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(w, h)); 

	*pResult = 0;
}

void CEcsView::OnButton9() 
{
	// TODO: Add your control notification handler code here
	CString strPltInfo;
	GET(IDC_PLC_COMM,		strPltInfo);
	int nPltInfo = _ttoi(strPltInfo);

	CEcsDoc* pDoc = GetDocument();
//	ASSERT(pDoc != NULL);

	if (pDoc == NULL)
		return;

	UpdateData(FALSE);
	CStationInfo* pStation = NULL;

	int nTemp = m_cbxStation.GetCurSel();
	if (nTemp == -1)
	{
		AfxMessageBox(_T("Station 정보를 선택해 주세요!"));
		return;
	}

	int nPlcNum = 0;
	int nStation = 0;
	int nAddress = 0;

		switch(nTemp)
		{
		case 0:		nStation = 106;			nAddress = 9961;			nPlcNum = 1;	break;
		case 1:		nStation = 105;			nAddress = 9959;			nPlcNum = 3;	break;
		case 2:		nStation = 104;			nAddress = 9957;			nPlcNum = 3;	break;
		case 3:		nStation = 108;			nAddress = 9963;			nPlcNum = 4;	break;
		case 4:		nStation = 101;			nAddress = 9951;			nPlcNum = 4;	break;
		case 5:		nStation = 102;			nAddress = 9953;			nPlcNum = 4;	break;
		case 6:		nStation = 103;			nAddress = 9955;			nPlcNum = 4;	break;
		default:	AfxMessageBox(_T("Station 정보를 선택해 주세요!")); return;
		}

	if (nAddress ==0)
		return;

	int nPltInfo1 = nPltInfo / 10000;
	int nPltInfo2 = nPltInfo % 10000;

	CCv* pCv = (CCv*)pDoc->GetEquipment(CEquipment::enCV, nPlcNum);

	static int nCount = 1;

	if (pCv != NULL)
	{
//		pCv->WriteWordValue(nAddress, nPltInfo1);
//		pCv->WriteWordValue(nAddress+1, nPltInfo2);

		CTime tCurrent = CTime::GetCurrentTime();
		
		int nDay = tCurrent.GetDay();

		pDoc->m_bProdInfo = TRUE;
		pDoc->m_nProdAddr = nAddress;
		pDoc->m_nProdDate = nDay;
		pDoc->m_nProdCount = nCount++;
		pDoc->m_nProdPlc = nPlcNum;

		
//		pCv->WriteWordValue(nAddress, nDay);
//		pCv->WriteWordValue(nAddress+1, nCount++);

		if (nCount > 9999)
			nCount = 0;
	}

}

void CEcsView::OnCheckLamp() 
{
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();

	if (pDoc == NULL)
		return;

//	UpdateData(FALSE);

	int nIndex = m_cbxLamp.GetCurSel();
	if (nIndex == -1)
	{
		AfxMessageBox(_T("경광등을 선택해 주세요!"));
		return;
	}

	int nStation = 0;
	int nAddress = 0;


	CCvFx* pCv = (CCvFx*)pDoc->GetEquipment(CEquipment::enCV, 12);
	if (pCv == NULL)
		return;

	if (!UpdateData())
		return;

	CLib::SetBit(pCv->m_pInfo->m_wCurrLampStatus, nIndex, m_bOn);

//	pCv->WriteLampStatus(nIndex, m_bOn);
	int nStation1 = 0;

}



void CEcsView::OnButton10() 
{
	// TODO: Add your control notification handler code here
	CString strPltInfo;
	GET(IDC_PLC_COMM,		strPltInfo);
	int nPltInfo = _ttoi(strPltInfo);

	CEcsDoc* pDoc = GetDocument();

	if (pDoc == NULL)
		return;

	UpdateData(FALSE);
	CStationInfo* pStation = NULL;

	int nTemp = m_cbxStation.GetCurSel();
	if (nTemp == -1)
	{
		AfxMessageBox(_T("Station 정보를 선택해 주세요!"));
		return;
	}

	int nPlcNum = 0;
	int nStation = 0;
	int nAddress = 0;

	switch(nTemp)
	{
	case 0:		nStation = 106;			nAddress = 9961;			nPlcNum = 1;	break;
	case 1:		nStation = 105;			nAddress = 9959;			nPlcNum = 3;	break;
	case 2:		nStation = 104;			nAddress = 9957;			nPlcNum = 3;	break;
	case 3:		nStation = 108;			nAddress = 9963;			nPlcNum = 4;	break;
	case 4:		nStation = 101;			nAddress = 9951;			nPlcNum = 4;	break;
	case 5:		nStation = 102;			nAddress = 9953;			nPlcNum = 4;	break;
	case 6:		nStation = 103;			nAddress = 9955;			nPlcNum = 4;	break;
	default:	AfxMessageBox(_T("Station 정보를 선택해 주세요!"));					return;
	}

	if (nAddress ==0)
		return;

	int nPltInfo1 = nPltInfo / 10000;
	int nPltInfo2 = nPltInfo % 10000;

	CCv* pCv = (CCv*)pDoc->GetEquipment(CEquipment::enCV, nPlcNum);

	static int nCount = 1;

	if (pCv != NULL)
	{
		CTime tCurrent = CTime::GetCurrentTime();
		
		int nDay = tCurrent.GetDay();

		pDoc->m_bProdInfo = TRUE;
		pDoc->m_nProdAddr = nAddress;
		pDoc->m_nProdDate = 0;
		pDoc->m_nProdCount = 0;
		pDoc->m_nProdPlc = nPlcNum;
	}	
}

void CEcsView::OnBtnReset1() 
{
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (pDoc == NULL) 
	{
		return;
	}

	pDoc->m_nWorkingLuggNum1 = 0;
}
void CEcsView::OnBtnReset2() 
{
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (pDoc == NULL) 
	{
		return;
	}

	pDoc->m_nWorkingLuggNum2 = 0;
}

void CEcsView::OnChkTestStart1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ENABLE_WND(IDC_STC_WORK_LUGG1, m_bTesting1);
	ENABLE_WND(IDC_WORKING_LUGG1, m_bTesting1);
	ENABLE_WND(IDC_BTN_RESET1, m_bTesting1);

}


void CEcsView::OnChkTestStart2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ENABLE_WND(IDC_STC_WORK_LUGG2, m_bTesting2);
	ENABLE_WND(IDC_WORKING_LUGG2, m_bTesting2);
	ENABLE_WND(IDC_BTN_RESET2, m_bTesting2);

}

void CEcsView::OnBtnAlterLoc() 
{
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (pDoc != NULL && pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()) 
	{
		GET(IDC_DUAL_STO_LOC, pDoc->m_strAlterLocation);
		// 재작업 지시 
		if ((pDoc->m_nWorkingLuggNum1 = pDoc->m_pHostCl->AlterLocation()) != 0)
		{
			// 재작업 로케이션 지우기 
			pDoc->m_strAlterLocation = "";
		}
	}	
}

void CEcsView::OnBtnRtrOrder() 
{
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (pDoc != NULL && pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()) 
	{
		GET(IDC_EDT_FROM_LOC, pDoc->m_strFromPos);
		GET(IDC_EDT_TO_LOC, pDoc->m_strToPos);

		if (pDoc->m_strFromPos.GetLength() != 7 || pDoc->m_strToPos.GetLength() != 7 )
		{
			AfxMessageBox(_T("랙투랙 작업 지시\nLocation을 바르게 입력(7자리)해주세요!"));
			return; 
		}

		AfxMessageBox(_T("일단 한국단자에서는 사용하지 말자!"));
		//// 랙투랙 작업 지시 - 일단 한국단자에서는 사용하지 말자
		//if ((pDoc->m_nWorkingLuggNum1 = pDoc->m_pHostCl->JobOrder(4)) != 0)
		//{
		//	CString strTemp = "";
		//	GET(IDC_EDT_FROM_LOC, strTemp);
		//	GET(IDC_EDT_TO_LOC, strTemp);
		//}
	}	
	else
	{
		AfxMessageBox(_T("Host가 연결되지 않았습니다."));
	}
}

void CEcsView::OnBtnAtaOrder() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (pDoc != NULL && pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()) 
	{
		GET(IDC_EDT_FROM_LOC, pDoc->m_strFromPos);
		GET(IDC_EDT_TO_LOC, pDoc->m_strToPos);

		if (pDoc->m_strFromPos.GetLength() != 7 || pDoc->m_strToPos.GetLength() != 7 )
		{
			AfxMessageBox(_T("호기간 이동 지시\nLocation을 바르게 입력(7자리)해주세요!"));
			return; 
		}

		AfxMessageBox(_T("일단 한국단자에서는 사용하지 말자!"));
		//// 호기간 이동 작업 지시 
		//if ((pDoc->m_nWorkingLuggNum1 = pDoc->m_pHostCl->JobOrder(5)) != 0)
		//{
		//	CString strTemp = "";
		//	GET(IDC_EDT_FROM_LOC, strTemp);
		//	GET(IDC_EDT_TO_LOC, strTemp);
		//}
	}		
	else
	{
		AfxMessageBox(_T("Host가 연결되지 않았습니다."));
	}
}

void CEcsView::OnBtnSiteOrder() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (pDoc != NULL && pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()) 
	{
		GET(IDC_EDT_FROM_LOC, pDoc->m_strFromPos);
		GET(IDC_EDT_TO_LOC, pDoc->m_strToPos);

		if (pDoc->m_strFromPos.GetLength() != 3 || pDoc->m_strToPos.GetLength() != 3 )
		{
			AfxMessageBox(_T("이동지시\nStation을 바르게 입력(3자리)해주세요!"));
			return; 
		}

		AfxMessageBox(_T("일단 한국단자에서는 사용하지 말자!"));
		//// 이동 작업 지시 
		//if ((pDoc->m_nWorkingLuggNum1 = pDoc->m_pHostCl->JobOrder(6)) != 0)
		//{
		//	CString strTemp = "";
		//	GET(IDC_EDT_FROM_LOC, strTemp);
		//	GET(IDC_EDT_TO_LOC, strTemp);
		//}
	}		
	else
	{
		AfxMessageBox(_T("Host가 연결되지 않았습니다."));
	}
		
}

void CEcsView::UpdateList(CListCtrl& oListCtrl, CString strFunction, SJobInvokeInfo* pJobInvokeInfo, BOOL bRemoveAll, BOOL bOnlyRemove)
{
	int i = 0, j = 0;
	if (bRemoveAll == TRUE)
	{
		oListCtrl.DeleteAllItems();
		if (bOnlyRemove == TRUE)
			return;
	}
	else
	{
		i = oListCtrl.GetItemCount();
	}

	if (pJobInvokeInfo == NULL)
		return;

	int nCnt = pJobInvokeInfo->m_strTime.IsEmpty();
	//CString strTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	UINT uMask = LVIF_TEXT | LVIF_IMAGE;
	oListCtrl.InsertItem(uMask, i, CConvert::ToString(pJobInvokeInfo->m_nWorkingLuggNum), 0, 0, 0, 0);
	oListCtrl.SetItem(i, ++j, uMask, CJobItem::GetJobTypeString(pJobInvokeInfo->m_nWorkingJobType), 0, 0, 0, 0);
	oListCtrl.SetItem(i, ++j, uMask, pJobInvokeInfo->m_strFromPos, 0, 0, 0, 0);
	oListCtrl.SetItem(i, ++j, uMask, pJobInvokeInfo->m_strToPos, 0, 0, 0, 0);
	oListCtrl.SetItem(i, ++j, uMask, pJobInvokeInfo->m_strTime, 0, 0, 0, 0);
	oListCtrl.SetItem(i, ++j, uMask, strFunction, 0, 0, 0, 0);
	//oListCtrl.SetItemData(i++, (LPARAM)pJobItem);
	oListCtrl.SetItemData(i++, NULL);
}


void CEcsView::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEcsDoc* pDoc = GetDocument();
	//ASSERT(pDoc != NULL);

	if (pDoc == NULL || pDoc->m_pConfig == NULL || pDoc->m_pConfig->m_nLogicGroupCnt <= 0)
		return;

	int nLogicGroup = (int)m_cbxLogicGroup.GetItemData(m_cbxLogicGroup.GetCurSel());

	if (nLogicGroup < 0)
		return;

	pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_bStart = TRUE;

	// 시작 클릭 -> 시작버튼 비활성, 종료버튼 활성
	ENABLE_WND(IDC_BTN_START, FALSE);
	ENABLE_WND(IDC_BTN_END, TRUE);

	// 어차피 지울 거니까 NULL이라도 상관없음! ㅡ,ㅡ;;;
	SJobInvokeInfo* pJobInvokeInfo = NULL;
	// 리스트에 작업 모두 삭제 
	//UpdateList(m_lstThisLogicJob, __FUNCTION__, pJobInvokeInfo, TRUE, TRUE);

	/*
	// 리스트에 작업 다시 표시 
	int a = 0;
	int nStoStnCnt = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_nStoStnCnt;
	for (int j = 0; j < nStoStnCnt; j++)
	{
		CString strStoStnNum = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_strStoStations.GetAt(j);
		CString strViaStnNum = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_strViaStations.GetAt(j);
		CString strRetStnNum = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_strRetStations.GetAt(j);

		if (pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum == 0)
		{
			//int nLuggNum = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[j]->m_nWorkingLuggNum;
			//int nJobType = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[j]->m_nWorkingJobType;
			//CString strFromStn = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[j]->m_strFromPos;
			//CString strToStn = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[j]->m_strToPos;
			//CString strTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));

			UpdateList(m_lstThisLogicJob, __FUNCTION__, pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[j]);
		}
	}
	//*/
}

void CEcsView::OnCbnSelchangeLogicGroup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEcsDoc* pDoc = GetDocument();
	//ASSERT(pDoc != NULL);

	if (pDoc == NULL || pDoc->m_pConfig == NULL || pDoc->m_pConfig->m_nLogicGroupCnt <= 0)
		return;

	int nLogicGroup = (int)m_cbxLogicGroup.GetItemData(m_cbxLogicGroup.GetCurSel());

	if (nLogicGroup < 0)
		return;

	BOOL bStart = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_bStart;
	ENABLE_WND(IDC_BTN_START, !bStart);
	ENABLE_WND(IDC_BTN_END, bStart);
}

void CEcsView::OnBnClickedBtnEnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEcsDoc* pDoc = GetDocument();
	//ASSERT(pDoc != NULL);

	if (pDoc == NULL || pDoc->m_pConfig == NULL || pDoc->m_pConfig->m_nLogicGroupCnt <= 0)
		return;

	int nLogicGroup = (int)m_cbxLogicGroup.GetItemData(m_cbxLogicGroup.GetCurSel());

	if (nLogicGroup < 0)
		return;

	pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_bStart = FALSE;

	// 종료 클릭 -> 시작버튼 활성, 종료버튼 비활성
	ENABLE_WND(IDC_BTN_START, TRUE);
	ENABLE_WND(IDC_BTN_END, FALSE);

	int nCount = pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos.GetCount();

	for (int i = 0; i < nCount; i++)
	{
		// 일단 이것만이라도 지우자
		pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[i]->m_bCompleteMove = FALSE;
		pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[i]->m_bCompleteStore = FALSE;
		pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[i]->m_nPrevLuggNum = 0;
		pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[i]->m_nWorkingJobType = 0;
		pDoc->m_pLogicGorupInfos[nLogicGroup - 1]->m_pJobInvokeInfos[i]->m_nWorkingLuggNum = 0;
	}

	// 어차피 지울 거니까 NULL이라도 상관없음! ㅡ,ㅡ;;;
	SJobInvokeInfo* pJobInvokeInfo = NULL;

	// 리스트에 작업 모두 삭제 
	UpdateList(m_lstThisLogicJob, __FUNCTION__, pJobInvokeInfo, TRUE, TRUE);
}

