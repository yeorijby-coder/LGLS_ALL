// EcsView.cpp : implementation of the CEcsView class
//

#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "EcsView.h"
#include "MainFrm.h"

#include "ScDlg.h"
#include "ScPairDlg.h"
//#include "BcrDlg.h"
#include "ByteArrayEx.h"

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
	ON_MESSAGE(WM_USER_LGV_NOTIFY, OnLgvNotify)
	ON_MESSAGE(WM_USER_LIFTER_NOTIFY, OnLifterNotify)
	ON_MESSAGE(WM_USER_RFID_NOTIFY, OnRfidNotify)
	ON_MESSAGE(WM_USER_BCR_NOTIFY, OnBcrNotify)
	ON_MESSAGE(WM_USER_DISPLAY_NOTIFY, OnDisplayNotify)
	ON_MESSAGE(WM_USER_THS_NOTIFY, OnThsNotify)
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
//	ON_COMMAND(ID_UNSET_ALL_SUSPEND, OnUnSetAllSuspend)
//	ON_COMMAND(ID_SET_ALL_SUSPEND, OnSetAllSuspend)
	ON_BN_CLICKED(IDC_BTN_RUN_MODE, OnBtnRunMode)
	ON_BN_CLICKED(IDC_BTN_SC_MODE, OnBtnScMode)
	ON_BN_CLICKED(IDC_BTN_PROD, OnBtnProd)
	ON_BN_CLICKED(IDC_BTN_RUN_STATUS, OnBtnRunStatus)
	ON_BN_CLICKED(IDC_BTN_ACTIVE, OnBtnActive)
	ON_BN_CLICKED(IDC_BTN_JOB_COMPLETE, OnBtnJobComplete)
	ON_BN_CLICKED(IDC_BTN_DUAL_STORE,	OnBtnDualStore)
	ON_BN_CLICKED(IDC_BTN_EMPTY_RETIEVE, OnBtnEmptyRetrieve)
	ON_BN_CLICKED(IDC_BTN_FORK2_POS, OnBtnFork2Pos)
	ON_BN_CLICKED(IDC_BTN_FORK1_POS, OnBtnFork1Pos)
	ON_BN_CLICKED(IDC_BTN_ERR_CODE, OnBtnError)
	ON_BN_CLICKED(IDC_BTN_VIRTICAL, OnBtnVirtical)
	ON_BN_CLICKED(IDC_BTN_HORIZONTAL, OnBtnHorizontal)
	ON_NOTIFY(TCN_SELCHANGE, IDC_LAYOUT_TAB, OnSelchangeLayoutTab)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_NOTIFY(NM_CLICK, IDC_LIST_REG_DATA, OnClickListRegData)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
//	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
//	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
//	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
//	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
//	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
//	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
//	ON_CBN_SELCHANGE(IDC_COMBO_PLC, OnSelchangeComboPlc)
//	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
//	ON_BN_CLICKED(IDC_CHECK_NEXT_NOR, OnCheckNextNor)
//	ON_BN_CLICKED(IDC_CHECK_CENTER_NEXT, OnCheckCenterNext)
//	ON_BN_CLICKED(IDC_CHECK_CENTER_DEST, OnCheckCenterDest)
//	ON_BN_CLICKED(IDC_CHECK_CROSS_TRACK, OnCheckCrossTrack)
//	ON_BN_CLICKED(IDC_CHECK_CENTER, OnCheckCenter)
//	ON_BN_CLICKED(IDC_CHECK_CROSS, OnCheckCross)
//	ON_BN_CLICKED(IDC_BUTTON23, OnButton23)
//	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
//	ON_BN_CLICKED(IDC_BTN_ALLAUTO, OnBtnAllauto)
//	ON_BN_CLICKED(IDC_BTN_ALLAUTO2, OnBtnAllSuspend)
//	ON_COMMAND(ID_SYS_CONTROL, OnSysControl)
//	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT, &CEcsView::OnBnClickedBtnSerialConnect)
END_MESSAGE_MAP()
//void CEcsView::OnMouseMove(UINT nFlags, CPoint point)
/////////////////////////////////////////////////////////////////////////////
// CEcsView construction/destruction

CEcsView::CEcsView()
	: CFormView(CEcsView::IDD)
{
	for(int i =0; i < 100; i++)
	{
		m_bUnLoad[i] = FALSE;
		m_bLoad[i] = TRUE;
		m_bJob1[i] = FALSE;
		m_bJob2[i] = FALSE;
		m_bFirstComplete[i] = FALSE;
		m_bOldFirstComplete[i] = FALSE;
		m_bEmptyRetrieved[i] = FALSE;
		m_bLoadPairJob[i] = FALSE;
		m_bUnLoadPairJob[i] = FALSE;
	}
}

CEcsView::~CEcsView()
{

}

void CEcsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEcsView)
	DDX_Control(pDX, IDC_LAYOUT_TAB, m_tabLayout);
	DDX_Control(pDX, IDC_COMBO_PLC, m_cbxPlcNo);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_cbxDevice);
	DDX_Control(pDX, IDC_LIST_REG_DATA, m_lstRegList);
	DDX_Check(pDX, IDC_CHECK1, m_bBit01); //bit0
	DDX_Check(pDX, IDC_CHECK2, m_bBit02); //bit1
	DDX_Check(pDX, IDC_CHECK3, m_bBit03);
	DDX_Check(pDX, IDC_CHECK4, m_bBit04);
	DDX_Check(pDX, IDC_CHECK5, m_bBit05);
	DDX_Check(pDX, IDC_CHECK6, m_bBit06);
	DDX_Check(pDX, IDC_CHECK49, m_bBit07);
	DDX_Check(pDX, IDC_CHECK50, m_bBit08);
	DDX_Check(pDX, IDC_CHECK51, m_bBit09);
	DDX_Check(pDX, IDC_CHECK52, m_bBit10);
	DDX_Check(pDX, IDC_CHECK53, m_bBit11);
	DDX_Check(pDX, IDC_CHECK54, m_bBit12);
	DDX_Check(pDX, IDC_CHECK15, m_bBit13);
	DDX_Check(pDX, IDC_CHECK16, m_bBit14);
	DDX_Check(pDX, IDC_CHECK55, m_bBit15);
	DDX_Check(pDX, IDC_CHECK18, m_bBit16);


	/*DDX_Check(pDX, IDC_CHECK6, m_bBit06);
	DDX_Check(pDX, IDC_CHECK7, m_bBit07);
	DDX_Check(pDX, IDC_CHECK8, m_bBit08);
	DDX_Check(pDX, IDC_CHECK9, m_bBit09);
	DDX_Check(pDX, IDC_CHECK10, m_bBit10);
	DDX_Check(pDX, IDC_CHECK11, m_bBit11);
	DDX_Check(pDX, IDC_CHECK12, m_bBit12);
	DDX_Check(pDX, IDC_CHECK13, m_bBit13);
	DDX_Check(pDX, IDC_CHECK14, m_bBit14);
	DDX_Check(pDX, IDC_CHECK15, m_bBit15);
	DDX_Check(pDX, IDC_CHECK16, m_bBit16);*/

	DDX_Check(pDX, IDC_CHK_RUN_MODE0,		m_bRemote);
	DDX_Check(pDX, IDC_CHK_RUN_MODE2,		m_bOnLine);
	DDX_Check(pDX, IDC_CHK_RUN_MODE1,		m_bOffLine);

	DDX_Check(pDX, IDC_CHK_SC_MODE0,		m_bManual);
	DDX_Check(pDX, IDC_CHK_SC_MODE1,		m_bAuto);

	DDX_Check(pDX, IDC_CHK_PROD0,			m_bEmpty);
	DDX_Check(pDX, IDC_CHK_PROD1,			m_bProd1);
	DDX_Check(pDX, IDC_CHK_PROD2,			m_bProd2);
	
	DDX_Check(pDX, IDC_CHK_RUN_STATUS1,		m_bWait);
	DDX_Check(pDX, IDC_CHK_RUN_STATUS2,		m_bMoving);
	DDX_Check(pDX, IDC_CHK_RUN_STATUS3,		m_bError);

	DDX_Check(pDX, IDC_CHK_ACTIVE0,			m_bStop);
	DDX_Check(pDX, IDC_CHK_ACTIVE1,			m_bActive);

	DDX_Check(pDX, IDC_CHK_JOB_COMPLETE0,	m_bRunning);
	DDX_Check(pDX, IDC_CHK_JOB_COMPLETE1,	m_bComplete1);
	DDX_Check(pDX, IDC_CHK_JOB_COMPLETE2,	m_bComplete2);
	DDX_Check(pDX, IDC_CHK_JOB_COMPLETE3,	m_bComplete3);

	DDX_Check(pDX, IDC_CHK_ERR_FORK1,		m_bErrFork1);
	DDX_Check(pDX, IDC_CHK_ERR_FORK2,		m_bErrFork2);

	DDX_Check(pDX, IDC_CHK_NEXT_DUAL_STO,	m_bNextDualSto );
	DDX_Check(pDX, IDC_CHK_NEXT_EMPTY_RET,	m_bNextEmptyRet);
	DDX_Check(pDX, IDC_CHK_NEXT_DUAL_STO2,	m_bNextDualSto2 );
	DDX_Check(pDX, IDC_CHK_NEXT_EMPTY_RET2,	m_bNextEmptyRet2);
	DDX_Check(pDX, IDC_CHK_NEXT_OVER_SIZE,	m_bNextOverSize);
	DDX_Check(pDX, IDC_CHK_NEXT_FALL_DOWN,	m_bNextFallDown);

	//}}AFX_DATA_MAP
}

BOOL CEcsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

#define			ID_MAIN_TIMER		100

void CEcsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	CWinApp * pApp = AfxGetApp();
	
//	pDoc->RunServers();

	CString strTemp;
	int i=0;
	CEquipment* pEquipment = NULL;
	for (i=0; i<pDoc->m_pEquipments.GetSize(); ++i)
	{
		pEquipment = pDoc->m_pEquipments[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);
		DEBUGER_ASSERT_VALID(pEquipment->GetInfo() != NULL);	
		
		if (pEquipment->m_enKind == CEquipment::enSC)
		{
			CSc* pSc = (CSc*)pEquipment;
			CScInfo* pScInfo = NULL;
			BOOL bSerial = FALSE;
			if (pSc != NULL)
			{
				pScInfo = (CScInfo*)pSc->GetInfo();

				bSerial = (pScInfo != NULL && pScInfo->m_bEtherNet == FALSE);
			}

			if (bSerial == TRUE)
			{
				// 작업하고 Continue 해야함!
				SerialConnect(pEquipment, pEquipment->m_nNumber);
				// 작업하고 Continue 해야함!
				continue;
			}
			pEquipment->RunThread();

			for (int j = 0 ; j < PLC_CONN_PORT_CNT ; j++)
			{
				pEquipment->RunServer(j);
			}

			strTemp.Format(_T("%02d"), pEquipment->m_nNumber);
			m_cbxPlcNo.AddString(strTemp);
		}
	}

	// 레이아웃당 1개의 탭을 구성할 수 있다.  - 레이아웃에 여러대의 SC를 넣을 수 있다. 
	int nPlcCount = 0;
	int nLayoutCnt = pDoc->m_pEcsLayOuts.GetSize();
	for (i = 0 ; i < nLayoutCnt ; ++i)
	{
		CEcsLayout * pEcsLayout = pDoc->m_pEcsLayOuts[i];

		if (pEcsLayout == NULL)
			continue;

		m_tabLayout.InsertItem(nPlcCount, pEcsLayout->m_strText, nPlcCount);	
		m_tabLayout.HighlightItem(nPlcCount++, FALSE);
		m_tabLayout.SetItemSize(CSize(200, 20));				// ?
	}

	UINT uTimerID = 0;
	
	m_nMainTimerID = SetTimer(ID_MAIN_TIMER, 1000, NULL);

	m_cbxPlcNo.SetCurSel(0);

	m_cbxDevice.AddString(_T("D"));
	m_cbxDevice.AddString(_T("M"));
	m_cbxDevice.SetCurSel(0);

	CImageList imageList;
	CListCtrl* pListCtrl1 = (CListCtrl*)GetDlgItem(IDC_LIST_REG_DATA);

	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/,	1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BOARD));
	pListCtrl1->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl1->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask			= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt			= LVCFMT_LEFT;
		int cx[]			= {50, 50, 50, 50};
		LPCTSTR lpszCol[]	= {_T("PlcNO"), _T("RegNO"), _T("DEC"), _T("HEX")};
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i=0; i<nCol; ++i)
			pListCtrl1->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	CListCtrl* pListCtrl2 = (CListCtrl*)GetDlgItem(IDC_LIST_SC_STATUS);

	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/,	1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BOARD));
	pListCtrl2->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl2->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask			= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt			= LVCFMT_LEFT;
		int cx[]			= {200, 50, 100};
		LPCTSTR lpszCol[]	= {_T("Info"), _T("Value"), _T("Address")};
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i=0; i<nCol; ++i)
			pListCtrl2->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	CListCtrl* pListCtrl3 = (CListCtrl*)GetDlgItem(IDC_LIST_SC_RECEIVE);

	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/,	1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BOARD));
	pListCtrl3->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl3->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask			= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt			= LVCFMT_LEFT;
		int cx[]			= {200, 50, 100};
		LPCTSTR lpszCol[]	= {_T("Info"), _T("Value"), _T("Address")};
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i=0; i<nCol; ++i)
			pListCtrl3->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	m_bOnLine = TRUE;
	m_bAuto = TRUE;
	m_bWait = TRUE;
	m_bActive = TRUE;

	UpdateData(FALSE);
	
	OnAllRunMode();
	OnAllScMode();
	OnAllRunStatus();
	OnAllActive();

	CString strDeviceNo;
	SET(IDC_EDIT_REGNUM, _T("95"));
	GET(IDC_EDIT_REGNUM, strDeviceNo);

	UpdateList(1, _ttoi(strDeviceNo));

//	UpdateStatusList();
//	UpdateReadCommandList();
}

void CEcsView::OnTimer(UINT nIDEvent) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	static int nCount = 1;
	CString strLog;
    
	//int nResultCode = _ttoi(strLog.Mid(39, 4));

	if (nCount % 1 == 0)
	{
//		UpdateScInfo();

		if (pDoc->m_pJob != NULL)
		{
			if (pDoc->m_bBackupChk == TRUE)
			{
				pDoc->m_pJob->Backup();
				pDoc->m_bBackupChk = FALSE;
			}
		}

		CEquipment* pEquipment = NULL;
		for (int i = 0, nScIndex = -1; i<pDoc->m_pEquipments.GetSize(); ++i)
		{
			pEquipment = pDoc->m_pEquipments[i];
			DEBUGER_ASSERT_VALID(pEquipment != NULL);
			DEBUGER_ASSERT_VALID(pEquipment->GetInfo() != NULL);
			
			if (pEquipment->m_enKind != CEquipment::enSC)
				continue;

			nScIndex++;
			
			CScInfo* pInfo = (CScInfo*)pEquipment->GetInfo();
			if (pInfo != NULL && pInfo->m_bEtherNet == TRUE)
			{
				if (pEquipment->IsConnect(0) == FALSE && pEquipment->IsConnect(1) == FALSE)
					continue;
			}

			if (pEquipment->m_enKind == CEquipment::enSC)
			{
				// ecs에서 작업 요청이 있는지 체크 
				CheckRequest(nScIndex);

				// 진행할 작업이 있는지 체크 
				//CheckRuningJob(nScIndex++);
				CheckRuningJob(nScIndex);
			}
		}
	}

	if (nCount % 10 == 0)
	{
		pDoc->ReconnectServer();
	}

//----------------------------------------------------------
	if (++nCount % 600 == 0)
	{
		nCount = 0;
	}

	CFormView::OnTimer(nIDEvent);
}

BOOL CEcsView::SerialConnect(CEquipment* pEquipment, int nIndex)
{
	CString strLog;

	if (pEquipment == NULL)
		return FALSE;

	CSc* pSc = (CSc*)pEquipment;
	CScInfo* pScInfo = NULL;

	pSc->m_pSerialComm = new CMelsecSerial(pEquipment, nIndex);
	LPCTSTR lpstrCommName = (LPCTSTR)pSc->m_portSerial.m_strComName;
	//pSc->m_portSerial.m_strComName = _T("\\.\COM6");
	//strcpy(lpstrCommName, );
	if (pSc->m_pSerialComm->Connect(pSc->m_portSerial.m_strComName) == FALSE)
	{
		return FALSE;
	}

	pSc->RunSerialThread();
	return TRUE;
}

void CEcsView::CheckRuningJob(int nPlcNum) 
{	
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

//	BOOL bTemp = m_bFirstDelete;
	// 작업구분 체크 
	if (pDoc->m_arrRegData[nPlcNum][111] != 0x00 )
	{
		// 작업구분이 1~4일때는 작업번호가 있어야만 함!
		if ((pDoc->m_arrRegData[nPlcNum][111] == 1 || 
			 pDoc->m_arrRegData[nPlcNum][111] == 2 || 
			 pDoc->m_arrRegData[nPlcNum][111] == 3 || 
			 pDoc->m_arrRegData[nPlcNum][111] == 4 ||
			 pDoc->m_arrRegData[nPlcNum][111] == 5) &&
			(pDoc->m_arrRegData[nPlcNum][112] == 0))
		{
			// 에러상황
			pDoc->m_arrRegData[nPlcNum][101] = 4;
			pDoc->m_arrRegData[nPlcNum][105] = 141;		// 수신데이터 이상
			return;
		}

		#pragma region 작업진행 하지 않는 경우

		// 에러일때는 작업을 진행하지 않음! 
		if (pDoc->m_arrRegData[nPlcNum][101] == 4)		// SC 상태 '이상'
			return;

		// 에러일때는 작업을 진행하지 않음! 
		if (pDoc->m_arrRegData[nPlcNum][105] != 0)		// 에러코드 있음
			return;

		// 정지일때는 작업을 진행하지 않음! 
		if (pDoc->m_arrRegData[nPlcNum][109] == 0)		// INACTIVE
			return;

		#pragma endregion

		// 작업 구분에 따라서 해야할 일이 달라진다.
		switch(pDoc->m_arrRegData[nPlcNum][111])
		{
			#pragma region 입고
			case 1:		// 입고
			{
				// 포크에 따라 구분해서 사용한다.
				switch(pDoc->m_arrRegData[nPlcNum][121])
				{
				case 0:			CheckJobRun(nPlcNum, 1, 1);		break;		// 1포크	
				case 1:			CheckJobRun(nPlcNum, 3, 1);		break;		// 1,2 포크
				case 2:			CheckJobRun(nPlcNum, 2, 1);		break;		// 2포크
				}
			}
			break;
			#pragma endregion

			#pragma region 출고
			case 2:		// 출고
			{
				// 포크에 따라 구분해서 사용한다.
				switch(pDoc->m_arrRegData[nPlcNum][121])
				{
				case 0:			CheckJobRun(nPlcNum, 1, 2);		break;		// 1포크	
				case 1:			CheckJobRun(nPlcNum, 3, 2);		break;		// 1,2 포크
				case 2:			CheckJobRun(nPlcNum, 2, 2);		break;		// 2포크
				}
			}
			break;
			#pragma endregion

			#pragma region 직출고
			case 3:		// 직출고
			{
				// 포크에 따라 구분해서 사용한다.
				switch(pDoc->m_arrRegData[nPlcNum][121])
				{
				case 0:			CheckJobRun(nPlcNum, 1, 3);		break;		// 1포크	
				case 1:			CheckJobRun(nPlcNum, 3, 3);		break;		// 1,2 포크
				case 2:			CheckJobRun(nPlcNum, 2, 3);		break;		// 2포크
				}
			}
			break;
			#pragma endregion

			#pragma region 재배치
			case 4:		// 재배치 
			{
				// 포크에 따라 구분해서 사용한다.
				switch(pDoc->m_arrRegData[nPlcNum][121])
				{
				case 0:			CheckJobRun(nPlcNum, 1, 4);		break;		// 1포크	
				case 1:			CheckJobRun(nPlcNum, 3, 4);		break;		// 1,2 포크
				case 2:			CheckJobRun(nPlcNum, 2, 4);		break;		// 2포크
				}
			}
			break;
			#pragma endregion

			#pragma region 홉복귀(임시 호기간이동)
			case 5:		// 홈복귀(임시 호기간이동)  
			{
				// 포크에 따라 구분해서 사용한다.
				switch(pDoc->m_arrRegData[nPlcNum][121])
				{
				case 0:			CheckJobRun(nPlcNum, 1, 2);		break;		// 1포크	
				case 1:			CheckJobRun(nPlcNum, 3, 2);		break;		// 1,2 포크
				case 2:			CheckJobRun(nPlcNum, 2, 2);		break;		// 2포크
				}
				//// 포크에 따라 구분해서 사용한다.
				//switch(pDoc->m_arrRegData[nPlcNum][121])
				//{
				//case 0:			CheckJobRun(nPlcNum, 1, 5);		break;		// 1포크	
				//case 1:			CheckJobRun(nPlcNum, 3, 5);		break;		// 1,2 포크
				//case 2:			CheckJobRun(nPlcNum, 2, 5);		break;		// 2포크
				//}
			}
			break;
			#pragma endregion
		}
	}	
}

void CEcsView::CheckForkJobRun(int nPlcNum, int nFork, int nJobType, int nForkJobType) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nBank = 0;
	int nBay = 0;
	int nLevel = 0;
	int nHsNum = 0;

	// ===================================================================
	// MCS 에서 Level값 조정 여부 & Double Deep 인지 체크 
	// ===================================================================
	BOOL bDoubleDeep = FALSE;
	BOOL bRtv = FALSE;
	CScInfo* pInfo = NULL;
	// Double Deep 이거나 MCS 일경우에는 1~4 Bank 가 지시된다  
	CSc* pSc = (CSc*)pDoc->GetEquipment(CEquipment::enSC, nPlcNum + 1);
	if (pSc != NULL)
	{
		pInfo = (CScInfo*)pSc->GetInfo();

		if (pInfo != NULL)
		{
			if (pInfo->m_nForkType == CScInfo::enMCS)
			{
				bDoubleDeep = TRUE;

				// Level 값을 조정 해준다. 
			//	nLevel -= pInfo->m_nStartLevel;
			//	nLevel = 10;
			}
			else if (pInfo->m_nForkType == CScInfo::enSingleForkDoubleDeep)
			{
				bDoubleDeep = TRUE;
			}

			bRtv = pInfo->m_bRtv;
		}
	}
	// -------------------------------------------------------------------

	switch(nForkJobType)
	{
	// 로딩작업  
	case 1:
		switch(nJobType)
		{
		// 입고패턴 
		case 1:
			nHsNum = 116;

			if (bRtv == TRUE)
				nHsNum = 113;

			if (nFork == 2)
			{
				nHsNum = 126;

				if (bRtv == TRUE)
					nHsNum = 123;
			}

			if (pDoc->GetLocationByHs(nPlcNum, 1, pDoc->m_arrRegData[nPlcNum][nHsNum], nBank, nBay, nLevel, pDoc->m_nCurrLoadingTrackNum[nPlcNum], pDoc->m_nCurrLoadingTrackNum2[nPlcNum], pDoc->m_nCurrLoadingPlcNum[nPlcNum]) == FALSE)
				return;

			break;

		// 출고패턴 
		case 2:
			nBank	= pDoc->m_arrRegData[nPlcNum][113];//113;
			nBay	= pDoc->m_arrRegData[nPlcNum][114];//114;
			nLevel	= pDoc->m_arrRegData[nPlcNum][115];//115;
			if (nFork == 2)
			{
				nBank	= pDoc->m_arrRegData[nPlcNum][123];//123;
				nBay	= pDoc->m_arrRegData[nPlcNum][124];//124;
				nLevel	= pDoc->m_arrRegData[nPlcNum][125];//125;
			}
			break;
		}
		break;
	// 언로딩작업  
	case 2:
		switch(nJobType)
		{
		// 입고패턴 
		case 1:
			nBank	= pDoc->m_arrRegData[nPlcNum][117];//117;
			nBay	= pDoc->m_arrRegData[nPlcNum][118];//118;
			nLevel	= pDoc->m_arrRegData[nPlcNum][119];//119;
			if (nFork == 2)
			{
				nBank	= pDoc->m_arrRegData[nPlcNum][127];//127;
				nBay	= pDoc->m_arrRegData[nPlcNum][128];//128;
				nLevel	= pDoc->m_arrRegData[nPlcNum][129];//129;
			}
			break;

		// 출고패턴 
		case 2:
			nHsNum = 120;

			if (bRtv == TRUE)
				nHsNum = 114;

			if (nFork == 2)
			{
				nHsNum = 130;

				if (bRtv == TRUE)
					nHsNum = 124;
			}

			if (pDoc->GetLocationByHs(nPlcNum, 2, pDoc->m_arrRegData[nPlcNum][nHsNum], nBank, nBay, nLevel, pDoc->m_nCurrUnLoadingTrackNum[nPlcNum], pDoc->m_nCurrUnLoadingTrackNum2[nPlcNum], pDoc->m_nCurrUnLoadingPlcNum[nPlcNum]) == FALSE)
				return;

			if(nFork == 1)
				pDoc->m_nCurrUnLoadingTrackNum2[nPlcNum] = 0;

			break;
		}
		break;
	// 홈복귀 작업  
	case 3:
		nBay = 0;
		nLevel = 0;

		break;
	}

	//SC 컨트롤을 해당 위치로 이동시킴
	if (pDoc->m_arrRegData[nPlcNum][102] > nBay)
	{
		--pDoc->m_arrRegData[nPlcNum][102];

		if (m_bOldFirstComplete[nPlcNum] != m_bFirstComplete[nPlcNum] && m_bFirstComplete[nPlcNum] == TRUE)
		{
			pDoc->m_arrRegData[nPlcNum][101] = 2;					// SC 상태 이동
		}
	}
	else if (pDoc->m_arrRegData[nPlcNum][102] < nBay)
	{
		++pDoc->m_arrRegData[nPlcNum][102];

		if (m_bOldFirstComplete[nPlcNum] != m_bFirstComplete[nPlcNum] && m_bFirstComplete[nPlcNum] == TRUE)
		{
			pDoc->m_arrRegData[nPlcNum][101] = 2;					// SC 상태 이동 
		}
	}

	if (pDoc->m_arrRegData[nPlcNum][103] > nLevel)
	{
		--pDoc->m_arrRegData[nPlcNum][103];

		if (m_bOldFirstComplete[nPlcNum] != m_bFirstComplete[nPlcNum] && m_bFirstComplete[nPlcNum] == TRUE)
		{
			pDoc->m_arrRegData[nPlcNum][101] = 2;					// SC 상태 이동
		}
	}
	else if (pDoc->m_arrRegData[nPlcNum][103] < nLevel)
	{
 		++pDoc->m_arrRegData[nPlcNum][103];

		if (m_bOldFirstComplete[nPlcNum] != m_bFirstComplete[nPlcNum] && m_bFirstComplete[nPlcNum] == TRUE)
		{
			pDoc->m_arrRegData[nPlcNum][101] = 2;					// SC 상태 이동
		}
	}


	// 포킹함!
	int nHori = pDoc->m_arrRegData[nPlcNum][102];
	int nVirt = pDoc->m_arrRegData[nPlcNum][103];
	if (pDoc->m_arrRegData[nPlcNum][102] == nBay && pDoc->m_arrRegData[nPlcNum][103] == nLevel)
	{
		// 홈복귀 일경우 ...
		if (nForkJobType == 3)
		{
			pDoc->m_arrRegData[nPlcNum][110] = 3;								// 작업완료
			pDoc->DeleteData(nPlcNum, nFork);									// SC DATA 삭제
			pDoc->m_arrRegData[nPlcNum][101] = 1;								// SC 상태 대기	
			m_bLoad[nPlcNum] = TRUE;
			return;
		}

		// 이중 입고가 체크되어 있다면... 
		UpdateData(TRUE);
		if ((m_bNextDualSto == TRUE || m_bNextDualSto2 == TRUE) && nForkJobType == 2)
		{
			CheckDaulSto(nPlcNum, nFork);
			return;
		}

		int nTemp = 0;
		switch(nFork)
		{
		case 1:		
			nTemp = nBank;	

			// ===================================================================
			// Double Deep 일때 Bank 값 조정 하기 1~2 -> 1~4
			// ===================================================================
			if (bDoubleDeep == TRUE)
				nTemp = int((nBank + 1) / 2);
			// -------------------------------------------------------------------

			/*
			// Double Deep 이거나 MCS 일경우에는 1~4 Bank 가 지시된다  
			pSc = (CSc*)pDoc->GetEquipment(CEquipment::enSC, nPlcNum + 1);
			
			if (pSc != NULL)
			{
				pInfo = (CScInfo*) pSc->GetInfo();
				
				if (pInfo != NULL)
				{
					if (pInfo->m_nForkType == enMCS || 
						pInfo->m_nForkType == enSingleForkDoubleDeep)
					{
						nTemp = int((nBank + 1) / 2);
//						switch(nBank)
//						{
//						case 1: 
//						case 2:		nTemp = 1;		break;
//						case 3: 
//						case 4:		nTemp = 2;		break;
//						}
					}
				}
			}
			//*/
			
			break;
		case 2:		nTemp = (nBank << 8);			break;
		case 3:		nTemp = nBank + (nBank << 8);	break;
		}

		if(pDoc->m_arrRegData[nPlcNum][110] != 3)
			pDoc->m_arrRegData[nPlcNum][104] = nTemp;			// 포크 좌출 우출 
		
		int nCurrProd = CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][100]);

		// 두번째 포킹할때 문제가 발생함!
		int nProd = 0x0000 ;		// 언로딩일때 화물감지는 이때 없어지게 됨! - 로딩일때는 원래 없음!
		if (nForkJobType == 2)
		{
			nProd = (nCurrProd - nFork) << 8;
		}
		else if (nForkJobType == 1 && m_bFirstComplete[nPlcNum] == TRUE)
		{
			nProd = nCurrProd << 8;
//			nProd = CLib::GetByteH(pDoc->m_arrRegData[0][100]);
		}

		nCurrProd = CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][100]);

		pDoc->m_arrRegData[nPlcNum][100] = nProd + CLib::GetByteL(pDoc->m_arrRegData[nPlcNum][100]);	// 화물감지 
	}		
}

void CEcsView::CheckOneJobRun(int nPlcNum, int nFork, int nJobType, int nForkJobType/* = 0*/) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (nForkJobType == 1)
	{
//		pDoc->m_bUnLoadComplete = FALSE;
		pDoc->m_nCompleteFork[nPlcNum] = 0;
		// 화물이 없을때 로딩일 경우 - 작업받은 로케이션과 현재 위치를 파악하여 현재 위치를 수정해준다.
		switch(nJobType)
		{
		case 1:		CheckForkJobRun(nPlcNum, nFork, 1, 1);		break;
		case 2:		CheckForkJobRun(nPlcNum, nFork, 2, 1);		break;
		case 3:		CheckForkJobRun(nPlcNum, nFork, 1, 1);		break;
		case 4:		CheckForkJobRun(nPlcNum, nFork, 2, 1);		break;
		case 5:		CheckForkJobRun(nPlcNum, nFork, 0, 3);		break;
		}
	}
	else if(nForkJobType == 2)
	{
		m_bUnLoad[nPlcNum] = TRUE;		
		switch(nJobType)
		{
		case 1:		CheckForkJobRun(nPlcNum, nFork, 1, 2);		break;
		case 2:		CheckForkJobRun(nPlcNum, nFork, 2, 2);		break;
		case 3:		CheckForkJobRun(nPlcNum, nFork, 2, 2);		break;
		case 4:		CheckForkJobRun(nPlcNum, nFork, 1, 2);		break;
		case 5:		CheckForkJobRun(nPlcNum, nFork, 0, 3);		break;
		}
	}
	// 기본값 
	else // if(nForkJobType == 0)
	{
		// 로드 
		if (CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][100]) != nFork)	// 화물감지
		{
			pDoc->m_nCompleteFork[nPlcNum] = 0;

			// 화물이 없을때 로딩일 경우 - 작업받은 로케이션과 현재 위치를 파악하여 현재 위치를 수정해준다.
			switch(nJobType)
			{
			case 1:		CheckForkJobRun(nPlcNum, nFork, 1, 1);		break;
			case 2:		CheckForkJobRun(nPlcNum, nFork, 2, 1);		break;
			case 3:		CheckForkJobRun(nPlcNum, nFork, 1, 1);		break;
			case 4:		CheckForkJobRun(nPlcNum, nFork, 2, 1);		break;
			case 5:		CheckForkJobRun(nPlcNum, nFork, 0, 3);		break;
			}
		}

		// 언로드 
		else if(CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][100]) == nFork)
		{
			m_bUnLoad[nPlcNum] = TRUE;		
			switch(nJobType)
			{
			case 1:		CheckForkJobRun(nPlcNum, nFork, 1, 2);		break;
			case 2:		CheckForkJobRun(nPlcNum, nFork, 2, 2);		break;
			case 3:		CheckForkJobRun(nPlcNum, nFork, 2, 2);		break;
			case 4:		CheckForkJobRun(nPlcNum, nFork, 1, 2);		break;
			case 5:		CheckForkJobRun(nPlcNum, nFork, 0, 3);		break;
			}
		}
		else
		{
			AfxMessageBox(_T("이건 뭐야!! 이러면 안되는데... 3"));
		}
	}
}

void CEcsView::CheckJobRun(int nPlcNum, int nFork, int nJobType) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	static int nForkCount = 0;

	// 포크 위치와 화물감지를 통해 현재 무엇을 해야 하는 상황인지 판단한다.
	if (pDoc->m_arrRegData[nPlcNum][104] == 0)		// 포크센터
	{	
		#pragma region 포크 센터 일때
		// 포크가 센터일 경우 - 화물 감지를 통해서 로딩과 언로딩을 판단한다.
		// 쌍작업 
		if (nFork == 3)
		{
			#pragma region 동시포크 작업
			//작업구분에 따른 bank,bay,lev 가져오기
			m_bLoadPairJob[nPlcNum]		= CheckCanPrePairJob(nPlcNum, nJobType, 1);
			m_bUnLoadPairJob[nPlcNum]	= CheckCanPrePairJob(nPlcNum, nJobType, 2);

			// 로드 
//			m_bLoad = TRUE;
			int nProdTemp = CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][100]);			// 화물감지
//				pDoc->m_arrRegData[0][100] = 0x0100 + CLib::GetByteL(pDoc->m_arrRegData[0][100]);	// 화물감지 
			if (m_bLoad[nPlcNum] == TRUE) 
			{
//				BOOL bPairJob = CheckCanPrePairJob(nJobType, 1);				// 동시작업 가능한지 체크 
				#pragma region 로딩 작업

				if (m_bLoadPairJob[nPlcNum] == TRUE)
				{
					CheckOneJobRun(nPlcNum, nFork, nJobType);							
				}
				else
				{
					if (nProdTemp == 0 || nProdTemp == 2)			// 화물감지
					{
						m_bJob1[nPlcNum] = TRUE;
						m_bJob2[nPlcNum] = FALSE;
						CheckOneJobRun(nPlcNum, 1, nJobType, 1);
					}
					else if(nProdTemp == 0 || nProdTemp == 1)
					{
						// 2번 포크에 작업 구분에 따라 로딩 하는 작업 지시  
						m_bJob2[nPlcNum] = TRUE;
						m_bJob1[nPlcNum] = FALSE;
						CheckOneJobRun(nPlcNum, 2, nJobType, 1);
					}
				}
				#pragma endregion 
			}
			// 언로드 
			else
			{
//				BOOL bPairJob = CheckCanPrePairJob(nJobType, 2);				// 동시작업 가능한지 체크 
				#pragma region 언로딩 작업

				if (m_bUnLoadPairJob[nPlcNum] == TRUE)
				{
					CheckOneJobRun(nPlcNum, nFork, nJobType);					
				}
				else
				{
					if (nProdTemp == 3 || nProdTemp == 1)	// 화물감지
					{
						m_bJob1[nPlcNum] = TRUE;
						m_bJob2[nPlcNum] = FALSE;
						CheckOneJobRun(nPlcNum, 1, nJobType, 2);						
					}
					else if(nProdTemp == 3 || nProdTemp == 2)
					{
						// 2번 포크에 작업 구분에 따라 로딩 하는 작업 지시  
						m_bJob2[nPlcNum] = TRUE;
						m_bJob1[nPlcNum] = FALSE;
						CheckOneJobRun(nPlcNum, 2, nJobType, 2);
					}
				}
				#pragma endregion 
			}					
			#pragma endregion 
		}
		// 싱글작업
		else
		{
			#pragma region 싱글포크 작업
			m_bLoadPairJob[nPlcNum]		= FALSE;
			m_bUnLoadPairJob[nPlcNum]	= FALSE;
			CheckOneJobRun(nPlcNum, nFork, nJobType);
			#pragma endregion 
		}
		#pragma endregion 
	}
	else if (nForkCount == 10)
	{
		#pragma region 포킹완료
		nForkCount = 0;

		// 포킹 완료
		pDoc->m_arrRegData[nPlcNum][104] = 0;	// 포크센터	
		
		int nMode = CLib::GetByteL(pDoc->m_arrRegData[nPlcNum][100]);		// 자동, 수동 
		UpdateData(TRUE);
		if (m_bNextEmptyRet || m_bNextEmptyRet2)
		{
			CheckEmptyRet(nPlcNum, nFork);
			return;		
		}

		int nProd = 0; //(0x0000 + nFork) << 8;
		int nCurrPord = CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][100]);

		if (m_bUnLoad[nPlcNum] == TRUE )
		{
			#pragma region 언로드
//			if ((CheckCanPrePairJob(nJobType, 2) && nFork == 3) || 
			if ((m_bUnLoadPairJob[nPlcNum]  && nFork == 3) || 
				nFork == 2 || nFork == 1 || m_bFirstComplete[nPlcNum] == TRUE || nJobType == 2 || nJobType == 3)
			{
				nProd = 0x0000;
				m_bUnLoad[nPlcNum] = FALSE;
				pDoc->m_nCompleteFork[nPlcNum] = 3;
				pDoc->m_arrRegData[nPlcNum][100] = nProd + nMode;	// 화물감지 
				pDoc->m_arrRegData[nPlcNum][110] = 3;			// 작업완료
				pDoc->DeleteData(nPlcNum, nFork);					// SC DATA 삭제
				pDoc->m_arrRegData[nPlcNum][101] = 1;				// SC 상태 대기
				pDoc->m_arrRegData[nPlcNum][111] = 0;				// 작업구분 삭제 
				m_bLoad[nPlcNum] = TRUE;
				m_bJob1[nPlcNum] = FALSE;
				m_bJob2[nPlcNum] = FALSE;
				m_bFirstComplete[nPlcNum] = FALSE;
				pDoc->m_bUnLoadingTrackDelete[nPlcNum] = TRUE;
				pDoc->m_bUnLoadingTrackDelete2[nPlcNum] = TRUE;
				if (nJobType == 2 || nJobType == 3)
				{
					pDoc->m_bUnLoadComplete[nPlcNum] = TRUE;
//					if (nFork == 3)
					if (nFork == 1)
					{
						pDoc->m_nCurrLoadingTrackNum2[nPlcNum] = 0;
					}
					if (nFork == 2)
					{
						pDoc->m_nCurrLoadingTrackNum[nPlcNum] = 0;
					}
				}
				return;
			}
//			else if (nFork == 3 && CheckCanPrePairJob(nJobType, 2) == FALSE)
			else if (nFork == 3 && m_bUnLoadPairJob[nPlcNum] == FALSE)
			{
				if (m_bJob1[nPlcNum] == TRUE)
				{
					pDoc->m_arrRegData[nPlcNum][100] = 0x0200 + nMode;	// 화물감지 - 해당 포크의 화물이 없어져야함  
					pDoc->m_arrRegData[nPlcNum][110] = 3;					// 작업완료
					pDoc->m_arrRegData[nPlcNum][101] = 1;					// SC 상태 대기
					pDoc->DeleteData1(nPlcNum);							// SC DATA 삭제
					m_bJob1[nPlcNum] = FALSE;

					if (m_bFirstComplete[nPlcNum] == FALSE)
					{
						m_bOldFirstComplete[nPlcNum] = m_bFirstComplete[nPlcNum];
						m_bFirstComplete[nPlcNum] = TRUE;
					}
					pDoc->m_bUnLoadingTrackDelete[nPlcNum] = TRUE;
					return;
				}
				else if (m_bJob2[nPlcNum] == TRUE)
				{
					pDoc->m_arrRegData[nPlcNum][100] = 0x0100 + nMode;	// 화물감지  - 해당 포크의 화물이 없어져야함
					pDoc->m_arrRegData[nPlcNum][110] = 3;					// 작업완료
					pDoc->m_arrRegData[nPlcNum][101] = 1;					// SC 상태 대기
					pDoc->DeleteData2(nPlcNum);							// SC DATA 삭제
					m_bJob2[nPlcNum] = FALSE;

					if (m_bFirstComplete[nPlcNum] == FALSE)
					{
						m_bOldFirstComplete[nPlcNum] = m_bFirstComplete[nPlcNum];
						m_bFirstComplete[nPlcNum] = TRUE;
					}
					pDoc->m_bUnLoadingTrackDelete2[nPlcNum] = TRUE;
					return;
				}
			}
			#pragma endregion
		}
		else if(nJobType == 1 || nJobType == 3)
		{
			#pragma region 입고,직출고
			// m_bUnLoad 가 FALSE(로딩) 이고, 입고작업이므로 입고 로딩 완료시에 CV DATA 삭제!
			pDoc->m_bLoadingTrackDelete[nPlcNum] = TRUE;
			if (nFork == 2 || nFork == 3)
				pDoc->m_bLoadingTrackDelete2[nPlcNum] = TRUE;
			#pragma endregion
		}
		else if (nJobType == 2 || nJobType == 4)
		{	
			#pragma region 출고, 랙투랙
//			if (nFork == 3 && CheckCanPrePairJob(nJobType, 1) == FALSE)
			if (nFork == 3 && m_bLoadPairJob[nPlcNum] == FALSE)
			{
				int nTempProd = CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][100]);
				int nMode = CLib::GetByteL(pDoc->m_arrRegData[nPlcNum][100]);
				if (m_bJob1[nPlcNum] == TRUE)
				{
//					pDoc->m_arrRegData[nPlcNum][110] = 1;					// 작업완료
					pDoc->m_arrRegData[nPlcNum][101] = 1;					// SC 상태 대기
//					pDoc->DeleteData2();							// SC DATA 삭제
					m_bJob1[nPlcNum] = FALSE;

					m_bOldFirstComplete[nPlcNum] = m_bFirstComplete[nPlcNum];
					if (m_bFirstComplete[nPlcNum] == FALSE)
					{
						pDoc->m_arrRegData[nPlcNum][100] = 0x0100 + nMode;	// 화물감지  - 해당 포크의 화물이 생겨야함
						m_bFirstComplete[nPlcNum] = TRUE;
					}
					else if (m_bFirstComplete[nPlcNum] == TRUE)
					{
						m_bFirstComplete[nPlcNum] = FALSE;
						nTempProd = 0x0300;
						pDoc->m_arrRegData[nPlcNum][100] = nTempProd + nMode;	// 화물감지  - 해당 포크의 화물이 생겨야함
						m_bLoad[nPlcNum] = FALSE;
					}
					return;
				}
				else if (m_bJob2[nPlcNum] == TRUE)
				{
//					pDoc->m_arrRegData[nPlcNum][110] = 2;					// 작업완료
					pDoc->m_arrRegData[nPlcNum][101] = 1;					// SC 상태 대기
//					pDoc->DeleteData2();							// SC DATA 삭제
					m_bJob2[nPlcNum] = FALSE;

					m_bOldFirstComplete[nPlcNum] = m_bFirstComplete[nPlcNum];
					if (m_bFirstComplete[nPlcNum] == FALSE)
					{
						pDoc->m_arrRegData[nPlcNum][100] = 0x0200 + nMode;	// 화물감지  - 해당 포크의 화물이 생겨야함
						m_bFirstComplete[nPlcNum] = TRUE;
					}
					else if (m_bFirstComplete[nPlcNum] == TRUE)
					{
						m_bFirstComplete[nPlcNum] = FALSE;
						m_bLoad[nPlcNum] = FALSE;
						nTempProd = 0x0300;
						pDoc->m_arrRegData[nPlcNum][100] = nTempProd + nMode;	// 화물감지  - 해당 포크의 화물이 생겨야함
					}
					return;
				}
			}
			#pragma endregion
		}

		nProd = (0x0000 + nFork) << 8;

		pDoc->m_arrRegData[nPlcNum][100] = nProd + nMode;								

		nCurrPord = CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][100]);			// 화물감지
		
		if (m_bLoad[nPlcNum] == TRUE && nCurrPord == nFork && nFork == 3)
			m_bLoad[nPlcNum] = FALSE;
		#pragma endregion 
	}
	else
	{
		#pragma region 포크 좌출 우출 중
		nForkCount++;
		#pragma endregion 
	}
}

void CEcsView::CheckDaulSto(int nPlcNum, int nFork)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	pDoc->m_arrRegData[nPlcNum][101] = 4;		// 에러 
	pDoc->m_arrRegData[nPlcNum][109] = 0;		// Stop
	if(m_bNextDualSto && (nFork == 1 || nFork == 3))
	{
		pDoc->m_arrRegData[nPlcNum][105] = 54;		// 이중입고  
	}
	else if (m_bNextDualSto2 && (nFork == 2 || nFork == 3))
	{
		pDoc->m_arrRegData[nPlcNum][105] = 55;		// 이중입고 
	}
	else
	{
		m_bNextDualSto = FALSE;
		m_bNextDualSto2 = FALSE;
		AfxMessageBox(_T("이중입고 에러 설정한 포크와 작업하는 포크 값이 다릅니다."));
		UpdateData(FALSE);
		return;
	}

	m_bNextDualSto = FALSE;
	m_bNextDualSto2 = FALSE;

	UpdateData(FALSE);
}


void CEcsView::CheckEmptyRet(int nPlcNum, int nFork)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	pDoc->m_arrRegData[nPlcNum][101] = 4;		// 에러 
	pDoc->m_arrRegData[nPlcNum][109] = 0;		// Stop
	if(m_bNextEmptyRet && (nFork == 1 || nFork == 3))
	{
		pDoc->m_arrRegData[nPlcNum][105] = 58;		// 공출고 
	}
	else if (m_bNextEmptyRet2 && (nFork == 2 || nFork == 3))
	{
		pDoc->m_arrRegData[nPlcNum][105] = 59;		// 공출고 
	}
	else
	{
		m_bNextEmptyRet = FALSE;
		m_bNextEmptyRet2 = FALSE;
		AfxMessageBox(_T("공출고 에러 설정한 포크와 작업하는 포크 값이 다릅니다."));
		UpdateData(FALSE);
		return;
	}

	m_bNextEmptyRet = FALSE;
	m_bNextEmptyRet2 = FALSE;

	int nProd = 0x0000;

	int nMode = CLib::GetByteL(pDoc->m_arrRegData[nPlcNum][100]);		// 자동, 수동 
	int nCurrProd = CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][100]);	// 화물감지 

	if (nFork == 3)
	{
		// 로딩시 동시작업인가?
		if (m_bLoadPairJob[nPlcNum])
		{
			if (pDoc->m_arrRegData[nPlcNum][105] == 58)
			{
				nProd = 0x0200;			
				pDoc->m_arrRegData[nPlcNum][121] = 2;					// 2번 포크에만 작업이 있는 것으로 수정함!
				pDoc->DeleteData(nPlcNum, 1);
			}
			else //if (pDoc->m_arrRegData[0][105] == 59)
			{
				nProd = 0x0100;		
				pDoc->m_arrRegData[nPlcNum][121] = 1;					// 1번 포크에만 작업이 있는 것으로 수정함!
				pDoc->DeleteData(nPlcNum, 2);
			}

			pDoc->m_arrRegData[nPlcNum][100] = nProd + nMode;		// 화물감지  - 해당 포크의 화물이 생겨야함
			m_bFirstComplete[nPlcNum] = TRUE;
			m_bLoad[nPlcNum] = FALSE;								// 화물을 하나만 뜬게 되면서 언로딩으로 바뀌어야 한다.
		}
		// 따로 따로 포킹하는 작업 
		else
		{
			if (pDoc->m_arrRegData[nPlcNum][105] == 59)
			{
				pDoc->DeleteData(nPlcNum, 2);							// 1번포크 작업후 2번 포크를 작업하게 되니까..
				m_bLoad[nPlcNum] = FALSE;
				pDoc->m_arrRegData[nPlcNum][100] = nCurrProd + nMode;		// 화물감지
			}
			else if (pDoc->m_arrRegData[nPlcNum][105] == 58)
			{
				nProd = 0x0000;									// 처음이니까 무조건 화물이 없음!
				pDoc->m_arrRegData[nPlcNum][100] = nProd + nMode;		// 화물감지
				pDoc->DeleteData(nPlcNum, 1);							// 1번포크 작업후 2번 포크를 작업하게 되니까..
				pDoc->m_arrRegData[nPlcNum][121] = 2;					// 2번 포크에만 작업이 있는 것으로 수정함!
			}
			m_bFirstComplete[nPlcNum] = TRUE;
		}
	}
	else
	{
		// 싱글작업 - 데이터 삭제하고 
		pDoc->DeleteData(nPlcNum, 3);
		nProd = 0x0000;											// 처음이니까 무조건 화물이 없음!
		pDoc->m_arrRegData[nPlcNum][100] = nProd + nMode;				// 화물감지
	}
	UpdateData(FALSE);
}

BOOL CEcsView::CheckCanPrePairJob(int nPlcNum, int nJobType, int nForkJobType) 
{
	switch(nJobType) //작업구분
	{
	case 1:  //입고
		switch(nForkJobType)
		{
		case 1:			return TRUE;								// 로딩작업 
		case 2: 		return CheckCanPairJob(nPlcNum, nJobType);	// 언로딩작업 
		}
		break;
	case 2: //출고
		switch(nForkJobType)
		{
		case 1:			return CheckCanPairJob(nPlcNum, nJobType);	// 로딩작업 
		case 2: 		return TRUE;								// 언로딩작업 
		}
		break;
	case 3: //직출고
		switch(nForkJobType)
		{
		case 1:			return TRUE;								// 로딩작업 
		case 2: 		return TRUE;								// 언로딩작업 
		}
		break;
	case 4: //재배치
		switch(nForkJobType)
		{
		case 1:			return CheckCanPairJob(nPlcNum, nJobType, TRUE);	// 로딩작업 
		case 2: 		return CheckCanPairJob(nPlcNum, nJobType, FALSE);	// 언로딩작업 
		}
		break;
	}

	return FALSE;
}

BOOL CEcsView::CheckCanPairJob(int nPlcNum, int nJobType, BOOL bR2RSto) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nBank1, nBay1, nLevel1;
	int nBank2, nBay2, nLevel2;
	switch(nJobType)
	{
	case 1: 
		nBank1 = 117; //FK1 도착지 열
		break;
	case 2: 
		nBank1 = 113; //FK1 출발지 열
		break;
	case 3: 
		return TRUE;
	case 4: 
		nBank1 = (bR2RSto == TRUE) ? 117 : 113 ;
		break;
	}

	nBay1	= nBank1 + 1;
	nLevel1 = nBank1 + 2;

	nBank2	= nBank1 + 10;

	nBay2	= nBank2 + 1;
	nLevel2 = nBank2 + 2;
	
	int nBank1Value = pDoc->m_arrRegData[nPlcNum][nBank1];
	int nBay1Value = pDoc->m_arrRegData[nPlcNum][nBay1];
	int nLevel1Value = pDoc->m_arrRegData[nPlcNum][nLevel1];
	int nBank2Value = pDoc->m_arrRegData[nPlcNum][nBank2];
	int nBay2Value = pDoc->m_arrRegData[nPlcNum][nBay2];
	int nLevel2Value = pDoc->m_arrRegData[nPlcNum][nLevel2];

	if(nBank1Value != 0 && nBank2Value !=0)
		return TRUE;

	return FALSE;
}

void CEcsView::CheckRequest(int nPlcNum) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	BYTE bTemp2 = pDoc->m_arrRegData[nPlcNum][191];
	BYTE bTemp3 = pDoc->m_arrRegData[nPlcNum][199];

	// ECS에서 요청 온 작업이 있는지 체크  
	if (pDoc->m_arrRegData[nPlcNum][191] == 0x01 ) //191 : 작업 DATA FLAG (1:WRITE 완료, 0:RESET)
	{
		#pragma region ECS에서 요청 온 작업일 때
		// 작업구분 체크 
		if (pDoc->m_arrRegData[nPlcNum][171] != 0x00 ) // (171:작업구분, 172:작업번호)
		{
			#pragma region 작업구분이 있을 때
			// 작업구분이 1~4일때는 작업번호가 있어야만 함! 
			if ((pDoc->m_arrRegData[nPlcNum][171] == 1 || 
				 pDoc->m_arrRegData[nPlcNum][171] == 2 || 
				 pDoc->m_arrRegData[nPlcNum][171] == 3 || 
				 pDoc->m_arrRegData[nPlcNum][171] == 4 ||
				 pDoc->m_arrRegData[nPlcNum][171] == 5) &&
				(pDoc->m_arrRegData[nPlcNum][172] == 0))
			{
				// 에러상황
//				pDoc->m_arrRegData[nPlcNum][101] = 4;
//				pDoc->m_arrRegData[nPlcNum][105] = 141;		// 수신데이터 이상
			}
			else
			{
				// 정상 상황
				pDoc->m_arrRegData[nPlcNum][191] = 0x00;		// 작업데이터 수신 여부를 지운다.
				pDoc->m_arrRegData[nPlcNum][101] = 0x02;		// SC 상태를 '동작중'으로 바꾼다.
				pDoc->m_arrRegData[nPlcNum][110] = 0x00;		// 작업 완료 상태를 작업중으로 바꾼다.

				// 데이터 옮김
				for (int i = 0 ; i < 20 ; i++)
				{
					pDoc->m_arrRegData[nPlcNum][111+i] = pDoc->m_arrRegData[nPlcNum][171+i]; // D111~130 -> D171~190
					pDoc->m_arrRegData[nPlcNum][171+i] = 0; //쓰기영역 0
				}
				return;
			}
			#pragma endregion
		}
		else
		{
			#pragma region 작업구분이 없을 때
			// 에러상황 (작업구분이 0인경우)
			pDoc->m_arrRegData[nPlcNum][101] = 4;		// SC 상태 '이상'
			pDoc->m_arrRegData[nPlcNum][105] = 141;		// 수신데이터 이상			
			#pragma endregion
		}
		#pragma endregion
	}
	// 리모트 컨트롤 정보
	else if (pDoc->m_arrRegData[nPlcNum][199] != 0x00)
	{
		#pragma region REMOTE CONTROL에 데이터가 들어온 경우
		switch(pDoc->m_arrRegData[nPlcNum][199])
		{
		case 0x0001:	pDoc->m_arrRegData[nPlcNum][101] = 4;				pDoc->m_arrRegData[nPlcNum][105] = 3;		break;	// 비상정지
		case 0x0002:	pDoc->m_arrRegData[nPlcNum][109] = 1;															break;	// ACTIVE
		case 0x0004:	pDoc->m_arrRegData[nPlcNum][109] = 0;															break;	// 정지
		case 0x0008:	pDoc->m_arrRegData[nPlcNum][101] = 1;				pDoc->m_arrRegData[nPlcNum][105] = 0;		break;	// ERROR RESET
		case 0x0010:	pDoc->DeleteData(nPlcNum, 1);						pDoc->m_arrRegData[nPlcNum][109] = 0;		break;	// FORK #1 삭제
		case 0x0020:	pDoc->DeleteData(nPlcNum, 2);						pDoc->m_arrRegData[nPlcNum][109] = 0;		break;	// FORK #2 삭제
		case 0x0040:																						
			pDoc->DeleteData(nPlcNum, 3);	
//			m_bLoad = TRUE;		 
//			m_bFirstComplete = FALSE;

			m_bUnLoad[nPlcNum] = FALSE;
			m_bLoad[nPlcNum] = TRUE;
			m_bJob1[nPlcNum] = FALSE;
			m_bJob2[nPlcNum] = FALSE;
			m_bFirstComplete[nPlcNum] = FALSE;
			m_bOldFirstComplete[nPlcNum] = FALSE;

			pDoc->m_arrRegData[nPlcNum][109] = 0;		
			break;	// FORK #1,2 삭제
		}

		pDoc->m_arrRegData[nPlcNum][199] = 0; //REMOTE CONTROL 0으로 초기화

		return;
		#pragma endregion
	}
}

void CEcsView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	CEcsDoc* pDoc = GetDocument();

	CEquipment* pEquipment = NULL;
	for (int i=0; i<pDoc->m_pEquipments.GetSize(); ++i)
	{
		pEquipment = pDoc->m_pEquipments[i];
		if (pEquipment == NULL)
			continue;

		DEBUGER_ASSERT_VALID(pEquipment != NULL);
		DEBUGER_ASSERT_VALID(pEquipment->GetInfo() != NULL);

//		if (pEquipment->m_enKind == CEquipment::enCV)
//		{
			pEquipment->KillThread();
//		}
			pEquipment->KillSerialThread();
	}

	KillTimer(enTimerStatusDisplay);
	KillTimer(enTimerReconnect);
	KillTimer(enTimerCheckIdleJob);
	KillTimer(enTimerThsInfo);
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
	ASSERT(pDoc != NULL);

	CEcsLayout layout;
	int nTemp = m_tabLayout.GetCurSel();

	if (nTemp < 0)
		return;

	CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
	if (pEcsLayout == NULL)
		return;

	pEcsLayout->OnDraw(this, pDC, nTemp + 1);

}

void CEcsView::OnSize(UINT nType, int cx, int cy)  //qwerqwer
{
	CFormView::OnSize(nType, cx, cy);
	
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	int width = cx;
	int height = cy;

	if (m_tabLayout.GetSafeHwnd())
	{
		m_tabLayout.MoveWindow(0, 0, cx, 19);
		

		int nTemp = m_tabLayout.GetCurSel();
		if (nTemp < 0)
			return;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		int nLayoutCnt = pDoc->m_pEcsLayOuts.GetSize();
		for (int i = 0 ; i < nLayoutCnt ; i++)
		{
			if (nTemp == i)
			{
				// Tab 선택에 맞게 PLC 번호가 조정이 되어야 한다. 
				m_tabLayout.HighlightItem(nTemp, TRUE);	
				SET_INT(IDC_COMBO_PLC, pEcsLayout->m_nRepresentPlc + 1);		// @@@
				OnButton5();
			}
			else
			{
				m_tabLayout.HighlightItem(i, FALSE);
			}
		}

		pEcsLayout->OnSize(this, nType, cx, cy);
	}
}

void CEcsView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	pDoc->m_bMoveFlag = TRUE; //1
	pDoc->m_PrevPoint = point;
	pDoc->m_tChecktime = CTime::GetCurrentTime();

	int a = m_tabLayout.GetCurSel();
	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();
		if (nTemp < 0)
			return;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		pEcsLayout->OnButtonDown(this, nFlags, point);
	}
	
	CFormView::OnLButtonDown(nFlags, point);
}

void CEcsView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	pDoc->m_bMoveFlag = FALSE; //3

	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();
		if (nTemp < 0)
			return;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		pEcsLayout->OnButtonUp(this, nFlags, point);
	}

	CFormView::OnLButtonUp(nFlags, point);
}

void CEcsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();

		if (nTemp < 0)
			return;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		pEcsLayout->OnKeyDown(this, nChar, nRepCnt, nFlags);
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
	return 0;
}

LRESULT CEcsView::OnCvNotify(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pWnd != NULL);

//	if (wParam < 16)
//		pWnd->UpdateCommStatus(0, ID_COM_CV01_01+wParam, ConvertNotifyToStatus(lParam));
//	else if (wParam < 32)
//		pWnd->UpdateCommStatus(0, ID_COM_CV02_01+wParam-16, ConvertNotifyToStatus(lParam));
//	else if (wParam < 48)
//		pWnd->UpdateCommStatus(1, ID_COM_CV03_01+wParam-32, ConvertNotifyToStatus(lParam));
//	else if (wParam < 64)
//		pWnd->UpdateCommStatus(1, ID_COM_CV04_01+wParam-48, ConvertNotifyToStatus(lParam));
//	else if (wParam < 80)
//		pWnd->UpdateCommStatus(2, ID_COM_CV05_01+wParam-64, ConvertNotifyToStatus(lParam));
//	else if (wParam < 96)
//		pWnd->UpdateCommStatus(2, ID_COM_CV06_01+wParam-80, ConvertNotifyToStatus(lParam));
	
	return 0;
}

LRESULT CEcsView::OnScNotify(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pWnd != NULL);

	return 0;
}

LRESULT CEcsView::OnRtvNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CEcsView::OnLgvNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CEcsView::OnLifterNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CEcsView::OnRfidNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CEcsView::OnBcrNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CEcsView::OnThsNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CEcsView::OnDisplayNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CEcsView::OnControlClick(WPARAM wParam, LPARAM lParam)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	
	CString strCID;
	strCID.Format(_T("%u"), wParam);
	if (CLib::IsValidControlID(strCID) == FALSE)
		return 0;

	CTrackInfo* pTrack = NULL;

	int nWarehouse = CLib::GetWarehouse(strCID);
	int nPlcNum = CLib::GetFloor(strCID);
	int nEquipKind = CLib::GetEquipKind(strCID);
	int nEquipNum = CLib::GetEquipNum(strCID);
	int nTrackNum = nEquipNum;// + (nWarehouse-1)*1000;

	switch (nEquipKind)
	{
	case CEquipment::enCV:
		{
//			int nPlcNum=_ttoi(strCID.Mid(3, 1));
			int nCvPlcNum = nPlcNum;
 			pDoc->m_oCvDlg.m_pDoc = pDoc;
			pDoc->m_oCvDlg.m_pTrack = pDoc->GetTrackInfo(nTrackNum, nPlcNum);
			if (pDoc->m_oCvDlg.m_pTrack == NULL)
				return 0;
//			DEBUGER_ASSERT_VALID(pDoc->m_oCvDlg.m_pTrack != NULL);
//			nTrackNum = (nTrackNum % 1000) * 10;
			pDoc->m_oCvDlg.DoModal();
		}
		break;
	case CEquipment::enSC:
		{
			CEquipment* pEquipment = pDoc->GetEquipment(nEquipKind, nEquipNum);
			DEBUGER_ASSERT_VALID(pEquipment != NULL);

			CScInfo* pInfo = (CScInfo*) pEquipment->GetInfo();

			if (pInfo == NULL)
				return 0;

			SET_INT(IDC_COMBO_PLC, nEquipNum);		// @@@

			OnButton5();

			if (pInfo->m_nForkType == CScInfo::enSingle || 
				pInfo->m_nForkType == CScInfo::enSingleForkDoubleDeep || 
				pInfo->m_nForkType == CScInfo::enMCS)
			{
				CScDlg dlg((CSc*)pEquipment, this);
				dlg.DoModal();
			}
			else
			{
				CScPairDlg dlg((CScPair*)pEquipment, this);
				dlg.DoModal();
			}
		}
		break;

	case CEquipment::enBCR:
		{
			CEquipment* pEquipment = pDoc->GetEquipment(nEquipKind, nEquipNum);
			DEBUGER_ASSERT_VALID(pEquipment != NULL);
//			CBcrDlg dlg((CBcr*)pEquipment, nEquipNum, this);
//			dlg.DoModal();
		}
		break;

	}


	return 0;
}

LRESULT CEcsView::OnAlarmNotify(WPARAM wParam, LPARAM lParam) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if ((pDoc->m_pWarningDlg != NULL) && (pDoc->m_pWarningDlg->m_bShow == FALSE))
		return 0;

	pDoc->OnWarning();

	return 0;
}

void CEcsView::OnViewLayoutInfo() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();

		if (nTemp < 0)
			return;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		pEcsLayout->OnViewLayoutInfo(this);
	}
//	pDoc->m_layout1.OnViewLayoutInfo(this);
}

void CEcsView::OnSetAllSuspend() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	pDoc->SetAllSetSusspend();
}

void CEcsView::OnUnSetAllSuspend() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	pDoc->SetAllUnSetSusspend();
}

void CEcsView::OnButton5() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;


	int nPlcNo, nDeviceNo;
	GET(IDC_COMBO_PLC,strPlcNo);			
	nPlcNo = _ttoi(strPlcNo);
	//GET(IDC_TRACK_NO2,strDeviceNo);	
	GET(IDC_EDIT_REGNUM,strDeviceNo);	
	if (strDeviceNo.IsEmpty()) return;

	nDeviceNo = _ttoi(strDeviceNo);
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
	//UpdateTrack(nPlcNo, nDeviceNo);
}

void CEcsView::UpdateList(int nPlcNo, int nDeviceNo) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_REG_DATA);
	LV_ITEM lvitem;
	TCHAR szBuff[256];
	int nLine=0;
	
	CString strPlcNo, strDeviceNo;
	strPlcNo.Format(_T("%d"), nPlcNo);
	SET(IDC_COMBO_PLC,strPlcNo);
	strDeviceNo.Format(_T("%d"), nDeviceNo);
	SET(IDC_EDIT_REGNUM,strDeviceNo);

	if(nDeviceNo == 0)
		return;
	pListCtrl->DeleteAllItems();

	for (int i=nDeviceNo; i<nDeviceNo+150; i++)
	{
		int nCol = 0;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem = nLine++;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%.2d"), nPlcNo);
		lvitem.pszText = (LPWSTR)szBuff;
		lvitem.iImage = 0;
		pListCtrl->InsertItem(&lvitem);

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("D%04d"), i);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%04d"), pDoc->m_arrRegData[nPlcNo-1][i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%02X %02X"), CLib::GetByteH(pDoc->m_arrRegData[nPlcNo-1][i]), CLib::GetByteL(pDoc->m_arrRegData[nPlcNo-1][i]));
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

	}
}

void CEcsView::UpdateBits(int nPlcNo, int nDeviceNo) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nLine=0;

	CString strRegName;
	strRegName.Format(_T("D%05d"),nDeviceNo);
	SET(IDC_REG_NAME,strRegName);

	//SET_INT(IDC_EDIT_REGNUM, nDeviceNo);

	if(nDeviceNo == 0)
		return;
	m_bBit01=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 0);
	m_bBit02=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 1);
	m_bBit03=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 2);
	m_bBit04=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 3);
	m_bBit05=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 4);
	m_bBit06=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 5);
	m_bBit07=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 6);
	m_bBit08=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 7);
	m_bBit09=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 8);
	m_bBit10=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 9);
	m_bBit11=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 10);
	m_bBit12=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 11);
	m_bBit13=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 12);
	m_bBit14=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 13);
	m_bBit15=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 14);
	m_bBit16=CLib::IsSet(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo], 15);

	strRegName.Format(_T("%05d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo]);
	SET(IDC_DECIMAL,strRegName);
	strRegName.Format(_T("%04X"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo]);
	SET(IDC_HEXA,strRegName);

	UpdateData(FALSE);

}

void CEcsView::UpdateTrack(int nPlcNo, int nDeviceNo) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nLine=0;

	int nTrackNum;
	/*
	if (nPlcNo == 1)
		nTrackNum = (nDeviceNo/10);
	else
		nTrackNum = (nDeviceNo/10)+(nPlcNo*1000);
*/

	nTrackNum = nDeviceNo;
	
	
	nDeviceNo = nTrackNum%1000*10;
	CString strTrNum, strRegName;
	strTrNum.Format(_T("%05d"),nTrackNum);
	SET(IDC_TRACK_NO,strTrNum);
	strTrNum.Format(_T("TR%05d"),nTrackNum);
	SET(IDC_TRACK_NO2,strTrNum);
	strRegName.Format(_T("%04d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo]);
	SET(IDC_LUGG_NUM,strRegName);
	strRegName.Format(_T("%05d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+1]);
	SET(IDC_DEST_POS,strRegName);
	strRegName.Format(_T("%02d"),(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2] >> 8) & 0x00FF);
	SET(IDC_TRAY_TYPE,strRegName);	
	
	strRegName.Format(_T("%05d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2] & 0x000F);
	SET(IDC_JOB_TYPE,strRegName);

	strRegName.Format(_T("%04d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+3] & 0x000F);
	SET(IDC_EDIT_SIZE,strRegName);
	strRegName.Format(_T("%04d"),(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2] >> 4) & 0x000F);
	SET(IDC_EDIT_TURN,strRegName);

	strRegName.Format(_T("%01d"),(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+5] & 0X000F));
	SET(IDC_SUSPEND,strRegName);
	strRegName.Format(_T("%05d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+6]);
	SET(IDC_ERR_CODE,strRegName);

	UpdateData(FALSE);

}

void CEcsView::OnButton18() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	int nPlcNo, nDeviceNo;// , nTrackNum;
	GET(IDC_COMBO_PLC,strPlcNo);			
	nPlcNo = _ttoi(strPlcNo);

	GET(IDC_REG_NAME, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	nDeviceNo = _ttoi(strDeviceNo.Mid(2,5)); //주소값

	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	CString strDecimal;

	int nDecimal;
	GET(IDC_DECIMAL,strDecimal); //적용하고자 하는 값
	nDecimal = _ttoi(strDecimal);
	if (nDecimal < 0 || nDecimal > 65535)	return;

	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo] = nDecimal;

	GET(IDC_EDIT_REGNUM,strDeviceNo); //선택한 주소값

	UpdateList(nPlcNo, _ttoi(strDeviceNo)); //선택한 주소값부터 리스트
	UpdateBits(nPlcNo, nDeviceNo); // 해당 주소값에 비트 적용
//	UpdateTrack(nPlcNo, nTrackNum);
	
}

void CEcsView::OnButton13() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	int nPlcNo, nDeviceNo;
	GET(IDC_COMBO_PLC,strPlcNo);			
	nPlcNo = _ttoi(strPlcNo);

	GET(IDC_REG_NAME,strDeviceNo);	
	if (strDeviceNo.IsEmpty()) return;

	nDeviceNo = _ttoi(strDeviceNo.Mid(2,5));
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	CString strHexa;

	GET(IDC_HEXA,strHexa);

	unsigned long nHexa;		//변경할 HEX값
	unsigned long nStartHexa;   //비교할 HEX값 : 0
	unsigned long nEndHexa;		//비교할 HEX값 : FFFF
	nHexa = _tcstoul(strHexa, NULL, 16);
	nStartHexa = _tcstoul(_T("0000"), NULL, 16);
	nEndHexa = _tcstoul(_T("FFFF"), NULL, 16);

	if (nHexa < nStartHexa || nHexa > nEndHexa)			
		return;

	TCHAR szHexa[5];
	//wsprintf(szHexa, _T("%04d"), _ttoi(strHexa));
	wsprintf((LPWSTR)szHexa, _T("%04s"), strHexa);

	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo] = CLib::HexStrToWORD(szHexa);


	GET(IDC_EDIT_REGNUM,strDeviceNo);

	UpdateList(nPlcNo, _ttoi(strDeviceNo));
	UpdateBits(nPlcNo, nDeviceNo);
	//UpdateTrack(nPlcNo, nTrackNum);
	
}

CScInfo* CEcsView::GetSimScInfo()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CEquipment* pEquipment = pDoc->GetEquipment(CEquipment::enSC, 1);
	
	if (pEquipment == NULL)
		return NULL;

	CRuntimeClass* pRc = pEquipment->GetRuntimeClass();

	if (pRc == NULL)
		return NULL;

	CScInfo* pScInfo = NULL;
	if (pRc->m_lpszClassName == "CSc")
	{
		CSc* pSc = NULL;
		(CSc*) pSc = (CSc*) pEquipment;

		if (pSc == NULL)
			return NULL;

		pScInfo = (CScInfo*) pSc->GetInfo();
	}
	else if (pRc->m_lpszClassName == "CScPair")
	{
		CScPair* pSc = NULL;
		(CScPair*) pSc = (CScPair*) pEquipment;

		if (pSc == NULL)
			return NULL;

		pScInfo = (CScInfo*) pSc->GetInfo();	
	}
	else
	{
		return NULL;
	}

	return pScInfo;
}

void CEcsView::OnBtnRunMode() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

//	CScInfo* pScInfo = GetSimScInfo();
//	if (pScInfo == NULL)
//		return;
	
		 if (m_bOffLine)	nTemp = 0x00;	
	else if (m_bOnLine)		nTemp = 0x01;
	else if (m_bRemote)		nTemp = 0x02;

//	pScInfo->SetRcMode((BYTE)nTemp);

	int nPlcNum = GET_INT(IDC_COMBO_PLC);
	int nIndex = 0;
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][99] = nTemp;
}

void CEcsView::OnBtnScMode() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

	int nIndex = 5;

	int nTempHigh = 0;//pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] >> 8;

	if (m_bEmpty)		nTempHigh += 0x0000;	
	if (m_bProd1)		nTempHigh += 0x0100;
	if (m_bProd2)		nTempHigh += 0x0200;

	if (m_bManual)		nTemp = nTempHigh + 0x0000;	
	else if (m_bAuto)	nTemp = nTempHigh + 0x0001;

	int nPlcNum = GET_INT(IDC_COMBO_PLC);
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][100] = nTemp;
}

void CEcsView::OnBtnProd() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

	int nIndex = 5;

	int nTempLow = 0;//(BYTE)pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] & 0xFF;

	if (m_bManual)		nTempLow = 0x0000;	
	else if (m_bAuto)	nTempLow = 0x0001;

	if (m_bEmpty)		nTemp += 0x0000 + nTempLow;	
	if (m_bProd1)		nTemp += 0x0100 + nTempLow;
	if (m_bProd2)		nTemp += 0x0200 + nTempLow;

	int nPlcNum = GET_INT(IDC_COMBO_PLC);
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][100] = nTemp;
	
}

void CEcsView::OnBtnRunStatus() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

//	CScInfo* pScInfo = GetSimScInfo();
//	if (pScInfo == NULL)
//		return;
	
		 if (m_bWait)		nTemp = 0x01;	
	else if (m_bMoving)		nTemp = 0x02;
	else if (m_bError)		nTemp = 0x04;

//	pScInfo->SetRcMode((BYTE)nTemp);
//	DDX_Check(pDX, IDC_CHK_RUN_STATUS1,		m_bWait);
//	DDX_Check(pDX, IDC_CHK_RUN_STATUS2,		m_bMoving);
//	DDX_Check(pDX, IDC_CHK_RUN_STATUS3,		m_bError);

	int nPlcNum = GET_INT(IDC_COMBO_PLC);
	int nIndex = 6;
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][101] = nTemp;
}

void CEcsView::OnBtnActive() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

//	CScInfo* pScInfo = GetSimScInfo();
//	if (pScInfo == NULL)
//		return;
	
		 if (m_bStop)		nTemp = 0x00;	
	else if (m_bActive)		nTemp = 0x01;

//	pScInfo->SetRcMode((BYTE)nTemp);
//	DDX_Check(pDX, IDC_CHK_ACTIVE0,			m_bStop);
//	DDX_Check(pDX, IDC_CHK_ACTIVE1,			m_bActive);

	int nPlcNum = GET_INT(IDC_COMBO_PLC);

	int nIndex = 14;
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][109] = nTemp;
}

void CEcsView::OnAllActive() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	UpdateData(TRUE);
	
	int nPlcNum = 0;
	for(int i =0; i < pDoc->m_pEquipments.GetSize(); i++)
	{
		if(pDoc->m_pEquipments[i]->m_enKind == CEquipment::enSC)
		{
	
			pDoc->m_arrRegData[nPlcNum++][109] = 0x01;
		}
	}

}

void CEcsView::OnAllRunStatus()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);
	
	int nPlcNum = 0;
	for(int i =0; i < pDoc->m_pEquipments.GetSize(); i++)
	{
		if(pDoc->m_pEquipments[i]->m_enKind == CEquipment::enSC)
		{
			pDoc->m_arrRegData[nPlcNum++][109] = 0x01;
		}
	}
}

void CEcsView::OnAllScMode()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);
	
	int nPlcNum = 0;
	for(int i =0; i < pDoc->m_pEquipments.GetSize(); i++)
	{
		if(pDoc->m_pEquipments[i]->m_enKind == CEquipment::enSC)
		{
			pDoc->m_arrRegData[nPlcNum++][100] = 0x0001;
		}
	}
}

void CEcsView::OnAllRunMode()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);
	
	int nPlcNum = 0;
	for(int i =0; i < pDoc->m_pEquipments.GetSize(); i++)
	{
		if(pDoc->m_pEquipments[i]->m_enKind == CEquipment::enSC)
		{
			pDoc->m_arrRegData[nPlcNum++][99] = 0x01;
		}
	}
}

void CEcsView::OnBtnJobComplete() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

		 if (m_bRunning)		nTemp = 0x00;	
	else if (m_bComplete1)		nTemp = 0x01;
	else if (m_bComplete2)		nTemp = 0x02;
	else if (m_bComplete3)		nTemp = 0x03;

//	DDX_Check(pDX, IDC_CHK_JOB_COMPLETE0,	m_bRunning);
//	DDX_Check(pDX, IDC_CHK_JOB_COMPLETE1,	m_bComplete1);
//	DDX_Check(pDX, IDC_CHK_JOB_COMPLETE2,	m_bComplete2);
//	DDX_Check(pDX, IDC_CHK_JOB_COMPLETE3,	m_bComplete3);

	int nPlcNum = GET_INT(IDC_COMBO_PLC);

	int nIndex = 15;
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][110] = nTemp;
}

void CEcsView::OnBtnDualStore() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

	if (m_bErrFork1)		nTemp += 0x0001;	
	if (m_bErrFork2)		nTemp += 0x0100;

	int nPlcNum = GET_INT(IDC_COMBO_PLC);

	int nIndex = 11;
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][106] = nTemp;
}

void CEcsView::OnBtnEmptyRetrieve() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

	if (m_bErrFork1)		nTemp += 0x0002;	
	if (m_bErrFork2)		nTemp += 0x0200;

	int nPlcNum = GET_INT(IDC_COMBO_PLC);
	int nIndex = 11;
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][106] = nTemp;
}

void CEcsView::OnBtnHorizontal() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

	CString strValue;

	GET(IDC_EDT_HORIZONTAL, strValue);	
	if (strValue.IsEmpty()) 
		return;

	nTemp = _ttoi(strValue);
	if (nTemp < 0 || nTemp > 99)	
		return;

	int nPlcNum = GET_INT(IDC_COMBO_PLC);
	int nIndex = 7;
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][102] = nTemp;
}

void CEcsView::OnBtnVirtical() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

	CString strValue;

	GET(IDC_EDT_VIRTICAL, strValue);	
	if (strValue.IsEmpty()) 
		return;

	nTemp = _ttoi(strValue);
	if (nTemp < 0 || nTemp > 99)	
		return;

	int nPlcNum = GET_INT(IDC_COMBO_PLC);
	int nIndex = 8;
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][103] = nTemp;
}

void CEcsView::OnBtnError() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

	CString strValue;

	GET(IDC_EDT_ERROR_CODE, strValue);	
	if (strValue.IsEmpty()) 
		return;

	nTemp = _ttoi(strValue);
	if (nTemp < 0 || nTemp > 999)	
		return;

	int nPlcNum = GET_INT(IDC_COMBO_PLC);
	int nIndex = 10;
//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	pDoc->m_arrRegData[nPlcNum-1][105] = nTemp;
}

void CEcsView::OnBtnFork1Pos() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

	CString strValue;

	GET(IDC_EDT_FORK1_POS, strValue);	
	if (strValue.IsEmpty()) 
		return;

	int nPlcNum = GET_INT(IDC_COMBO_PLC);

	int nTempFork = _ttoi(strValue);
	if (nTempFork < 0 || nTempFork > 2)	
		return;

	int nIndex = 9;
	int nTempHigh = CLib::GetByteH(pDoc->m_arrRegData[nPlcNum][104]);//pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] >> 8;

//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)nTemp + (nTempHigh << 8);
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	nTemp = (WORD)nTempFork + (nTempHigh << 8);
	pDoc->m_arrRegData[nPlcNum-1][104] = nTemp;
}

void CEcsView::OnBtnFork2Pos() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTemp = 0;

	UpdateData(TRUE);

	CString strValue;

	GET(IDC_EDT_FORK2_POS, strValue);	
	if (strValue.IsEmpty()) 
		return;

	int nTempFork = _ttoi(strValue);
	if (nTempFork < 0 || nTempFork > 2)	
		return;

	int nPlcNum = GET_INT(IDC_COMBO_PLC);

	int nIndex = 9;
	int nTempLow = CLib::GetByteL(pDoc->m_arrRegData[nPlcNum][104]);//pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] & 0xFF;

//	pDoc->m_spdgPlc2Ecs.m_arrData[nIndex] = (WORD)(nTemp << 8) + nTempLow;
//	pDoc->m_arrRegData[0][pDoc->m_spdgPlc2Ecs.m_nStartAddress + nIndex] = nTemp;
	nTemp = (WORD)(nTempFork << 8) + nTempLow;
	pDoc->m_arrRegData[nPlcNum-1][104] = nTemp;
}

void CEcsView::OnButton12() 
{
	int nTemp = 0;

	UpdateData(TRUE);

	if (m_bBit01)
		nTemp += 0x01;
	if (m_bBit02)
		nTemp += 0x02;
	if (m_bBit03)
		nTemp += 0x04;
	if (m_bBit04)
		nTemp += 0x08;
	if (m_bBit05)
		nTemp += 0x10;
	if (m_bBit06)
		nTemp += 0x20;
	if (m_bBit07)
		nTemp += 0x40;
	if (m_bBit08)
		nTemp += 0x80;
	if (m_bBit09)
		nTemp += 0x100;
	if (m_bBit10)
		nTemp += 0x200;
	if (m_bBit11) //m_bBit11
		nTemp += 0x400;
	if (m_bBit12)
		nTemp += 0x800;
	if (m_bBit13) //m_bBit13
		nTemp += 0x1000;
	if (m_bBit14)
		nTemp += 0x2000;
	if (m_bBit15)
		nTemp += 0x4000;
	if (m_bBit16)
		nTemp += 0x8000;

	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	int nPlcNo, nDeviceNo;
	GET(IDC_COMBO_PLC,strPlcNo);	// ex) 1
	nPlcNo = _ttoi(strPlcNo);

	GET(IDC_REG_NAME,strDeviceNo);// ex) D00095	
	if (strDeviceNo.IsEmpty()) return;

	//GET(IDC_TRACK_NO2 ,strDeviceNo);	
	//if (strDeviceNo.IsEmpty()) return;


	//strDeviceNo=  strDeviceNo.Mid(2, 5);
	nDeviceNo = _ttoi(strDeviceNo.Mid(2,5)); // ex) 95, 100
	//nDeviceNo=GET_INT(IDC_EDIT_REGNUM);
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo] = nTemp;

	UpdateList(nPlcNo, nDeviceNo); // 해당 번지수부터 리스트 만들기
	UpdateBits(nPlcNo, nDeviceNo); // 선택한 비트값으로 적용하기
//	UpdateList(nPlcNo, (nDeviceNo/10)*10);
//	UpdateTrack(nPlcNo, (nDeviceNo/10)*10);
//	UpdateList(nPlcNo, (nDeviceNo%1000)*10);
//	UpdateTrack(nPlcNo, nDeviceNo);	
	
	//UpdateTrack(nPlcNo, _ttoi(strDeviceNo));
}

void CEcsView::OnButton22() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strTrNum, strCvKind, strNextCv;
	
	GET(IDC_COMBO_PLC,strPlcNo);	
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO,strTrNum);	
	if (strTrNum.IsEmpty()) return;
	
	int nPlcNo, nTrNum;
	
	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);
	

	//nPlcNo = 7;
 	CCv* pCv = (CCv*) pDoc->GetEquipment(CEquipment::enCV, nPlcNo);

	CTrackInfo* pTrack = NULL;

	if(pCv == NULL)
		return;


	for ( int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
	{
		pTrack = pCv->m_pInfo->m_pTracks[i];

		int nDeviceNo = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;
		pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+7] = pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+7] | enBit11;
	}

}

void CEcsView::OnClickListRegData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_lstRegList.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nIndex = m_lstRegList.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex >= m_lstRegList.GetItemCount()))
		return;

	int nPlcNo = _ttoi(m_lstRegList.GetItemText(nIndex, 0));
	CString strDevNo = m_lstRegList.GetItemText(nIndex, 1);
	int nDeviceNo = _ttoi(strDevNo.Mid(1,4));

	UpdateBits(nPlcNo, nDeviceNo);
	
	*pResult = 0;
}

void CEcsView::UpdateStationInfo(int nWarehouse, CComboBox& cbxStation)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (!CJobItem::IsValidWarehouse(nWarehouse))
	{
		AfxMessageBox(_T("유효하지 않은 창고번호입니다!"));
		return;
	}

	cbxStation.ResetContent();
	CStationInfo* pStation = NULL;
	for (int i=0; i<pDoc->m_pStationInfos.GetSize(); ++i)
	{
		pStation = pDoc->m_pStationInfos[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		if (pStation->GetWarehouseNo() != nWarehouse)
			continue;

		int nTemp = cbxStation.GetDlgCtrlID();


	}
}

void CEcsView::OnSelchangeLayoutTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	CRect rect;

	GetWindowRect(&rect);

	int h = rect.Height();
	int w = rect.Width();

	this->Invalidate();

//	pDoc->AllModified(TRUE);

	SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(w, h)); 

	*pResult = 0;
}

// SC SIM 이므로 각 Layout 마다 SC의 대표 PLC 정보를 가지고 있다. 
// 그것을 활용하여 함수를 구성하였다. 
void CEcsView::OnBnClickedBtnSerialConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	int nTemp = m_tabLayout.GetCurSel();
	if (nTemp < 0)
		return;

	CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
	if (pEcsLayout == NULL)
		return;

	CEquipment* pEquipment = pDoc->GetEquipment(CEquipment::enSC, pEcsLayout->m_nRepresentPlc + 1);

	if (pEquipment == NULL)
		return;

	SerialConnect(pEquipment, pEcsLayout->m_nRepresentPlc + 1);
}
/*
void CEcsView::OnButton17()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

//	GET(IDC_COMBO_PLC,strPlcNo);
//	if (strPlcNo.IsEmpty()) return;

	GET(IDC_TRACK_NO,strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;

	nPlcNo = _ttoi(strDeviceNo.Mid(0,2));

//	nPlcNo = _ttoi(strPlcNo);
	nDeviceNo = _ttoi(strDeviceNo);
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

//	nPlcNo = nDeviceNo / 1000;
//	nDeviceNo = (nDeviceNo % 1000) * 10;
//	nDeviceNo = (nDeviceNo % 100)*10;

//	UpdateList(nPlcNo, (nDeviceNo/10)*10);
//	UpdateBits(nPlcNo, (nDeviceNo/10)*10);
//	UpdateTrack(nPlcNo, (nDeviceNo/10)*10);
	UpdateList(nPlcNo, (nDeviceNo%1000)*10);
	UpdateBits(nPlcNo, (nDeviceNo%1000)*10);
	UpdateTrack(nPlcNo, nDeviceNo);
}


void CEcsView::OnButton14()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO,strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC,strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = nDeviceNo%1000*10;
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

//	nPlcNo = nDeviceNo / 1000;
//	if (nPlcNo == 0)
//		nPlcNo++;
	nPlcNo = _ttoi(strPlcNo);
//	nDeviceNo = (nDeviceNo % 1000) * 10;
//	nDeviceNo = nDeviceNo%1000*10;

	CString strLuggNum,strDestPos,strDeadLock,strJobType,strTrayType,strTrayId,strSuspend,strErrCode, strRgv, strPlc, strTurn, strSize;

	GET(IDC_LUGG_NUM,strLuggNum);
	GET(IDC_DEST_POS,strDestPos);
	GET(IDC_EDIT_SIZE,strSize);
	GET(IDC_JOB_TYPE,strJobType);
	GET(IDC_TRAY_TYPE,strTrayType);
	GET(IDC_TRAY_ID_2,strTrayId);
	GET(IDC_TRAY_ID,strTrayId);
	GET(IDC_EDIT_TURN,strTurn);
	GET(IDC_SUSPEND,strSuspend);
	GET(IDC_ERR_CODE,strErrCode);
	GET(IDC_TRAY_ID_2,strRgv);
	GET(IDC_EDIT_LGV_AGREE,strPlc);


	//pDoc->m_arrRegData[nPlcNo-1][nDeviceNo] = _ttoi(strLuggNum);
	//pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+1] = _ttoi(strDestPos);
	//pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+3] = _ttoi(strTrayType);
	//pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] = pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] | _ttoi(strSuspend);
	//pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+6] = _ttoi(strErrCode);

//	strRegName.Format(_T("%02d",pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+1]%100);
//	SET(IDC_JOB_TYPE,strRegName);

//	strJobType, strTrayId

	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo] = _ttoi(strLuggNum);


	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+1] = _ttoi(strDestPos);
	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2] = (_ttoi(strTrayType) << 8) + (_ttoi(strTurn) << 4) + _ttoi(strJobType);
	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+3] = _ttoi(strSize);
	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] = (_ttoi(strTrayId) << 4) + _ttoi(strSuspend);
	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+6] = _ttoi(strErrCode);
//	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] = _ttoi(strRgv);
//	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+9] = _ttoi(strPlc);

	int kkk= pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+1];




	//int nLen = strTrayId.GetLength();

	//if (nLen == 6)
	//{
	//	TCHAR strTemp1[2];
	//	wsprintf(strTemp1, _T("%s"), strTrayId.Mid(0,1));
	//	CString strTemp2 = strTrayId.Mid(1,5);
	//	unsigned long nTemp = atol(strTemp2);
	//
	//	BYTE byByte = nTemp >> 16;
	//	WORD wWord  = nTemp & 0xFFFF;
	//	WORD wWord2 = (strTemp1[0]) << 1;
	//	wWord2 = wWord2 | byByte;
	//	wWord2 = wWord2*100 + _ttoi(strDestPos);
	//	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2] = wWord2;
	//	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] = wWord;
	//}

//	else
//	{
//		pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2] = _ttoi(strJobType);
//		pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2] = _ttoi(strDestPos);
//		pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] = 0;
//	}

//	UpdateList(nPlcNo, (nDeviceNo/10)*10);
	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);

}

void CEcsView::OnButton15()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO,strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC,strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = (nDeviceNo % 1000) * 10;
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

//	nPlcNo = nDeviceNo / 1000;
	nPlcNo = _ttoi(strPlcNo);
	if (nPlcNo == 0)
		nPlcNo++;
//	nDeviceNo = (nDeviceNo % 1000) * 10;

	int nTemp = 0;

	UpdateData(TRUE);

	if (m_bBit17)
		nTemp += 0x01;
	if (m_bBit18)
		nTemp += 0x02;
	if (m_bBit19)
		nTemp += 0x04;
	if (m_bBit20)
		nTemp += 0x08;
	if (m_bBit21)
		nTemp += 0x10;
	if (m_bBit22)
		nTemp += 0x20;
	if (m_bBit23)
		nTemp += 0x40;
	if (m_bBit24)
		nTemp += 0x80;
	if (m_bBit25)
		nTemp += 0x100;
	if (m_bBit26)
		nTemp += 0x200;
	if (m_bBit27)
		nTemp += 0x400;
	if (m_bBit28)
		nTemp += 0x800;
	if (m_bBit29)
		nTemp += 0x1000;
	if (m_bBit30)
		nTemp += 0x2000;
	if (m_bBit31)
		nTemp += 0x4000;
	if (m_bBit32)
		nTemp += 0x8000;

	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+7] = nTemp;

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
}

void CEcsView::OnButton16()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO,strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC,strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = (nDeviceNo % 1000) * 10;
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

//	nPlcNo = nDeviceNo / 1000;
	nPlcNo = _ttoi(strPlcNo);

	//if (nPlcNo == 0)
	//	nPlcNo++;
	//nDeviceNo = (nDeviceNo % 1000) * 10;


	int nTemp = 0;

	UpdateData(TRUE);

	if (m_bBit33)
		nTemp += 0x01;
	if (m_bBit34)
		nTemp += 0x02;
	if (m_bBit35)
		nTemp += 0x04;
	if (m_bBit36)
		nTemp += 0x08;
	if (m_bBit37)
		nTemp += 0x10;
	if (m_bBit38)
		nTemp += 0x20;
	if (m_bBit39)
		nTemp += 0x40;
	if (m_bBit40)
		nTemp += 0x80;
	if (m_bBit41)
		nTemp += 0x100;
	if (m_bBit42)
		nTemp += 0x200;
	if (m_bBit43)
		nTemp += 0x400;
	if (m_bBit44)
		nTemp += 0x800;
	if (m_bBit45)
		nTemp += 0x1000;
	if (m_bBit46)
		nTemp += 0x2000;
	if (m_bBit47)
		nTemp += 0x4000;
	if (m_bBit48)
		nTemp += 0x8000;

	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+8] = nTemp;

//	UpdateList(nPlcNo, (nDeviceNo/10)*10);
	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);

}

void CEcsView::OnButton19()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strTrNum, strCvKind, strNextCv;
	CString strCenterNext, strCenterDest, strCrossTrack;

	GET(IDC_COMBO_PLC,strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO,strTrNum);
	if (strTrNum.IsEmpty()) return;
	GET(IDC_COMBO_CV_KIND,strCvKind);
	if (strCvKind.IsEmpty()) return;
	GET(IDC_EDIT_NEXT_CV,strNextCv);
	int nNextCv = _ttoi(strNextCv);
	if (nNextCv <= 0) return;

	UpdateData(TRUE);

	int nCenterNext, nCenterDest, nCrossTrack, nCenterNextPlcNum, nCrossPlcNum;

	if (m_bCenter)
	{
		GET(IDC_EDIT_CENTER_NEXT,strCenterNext);
		nCenterNext = _ttoi(strCenterNext);
		if (nCenterNext <= 0) return;
		GET(IDC_EDIT_CENTER_DEST,strCenterDest);
		nCenterDest = _ttoi(strCenterDest);
		if (nCenterDest <= 0) return;
		nCenterNextPlcNum = pDoc->m_nCenterNextPlcNum;
	}
	else
	{
		nCenterNext = 0;
		nCenterDest = 0;
		nCenterNextPlcNum = 0;
	}

	if (m_bCross)
	{
		GET(IDC_EDIT_CROSS_TRACK,strCrossTrack);
		nCrossTrack = _ttoi(strCrossTrack);
		if (nCrossTrack < 0) return;
		nCrossPlcNum = pDoc->m_nCrossPlcNum;
	}
	else
	{
		nCrossTrack = 0;
		nCrossPlcNum = 0;
	}

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	CTrackInfo* pTrack = NULL;
	pTrack = pDoc->GetTrackInfo(nTrNum, nPlcNo);

	if (pTrack != NULL)
	{
		pTrack->m_nKind = _ttoi(strCvKind.Mid(0,1));
		pTrack->m_nNextCv = nNextCv;
		pTrack->m_nNextPlcNum = pDoc->m_nNextPlcNum;
		pTrack->m_bCenter = m_bCenter;
		pTrack->m_nCenterNextPlcNum = nCenterNextPlcNum;
		pTrack->m_nCenterNext = nCenterNext;
		pTrack->m_nCenterDest = nCenterDest;
		pTrack->m_bCross = m_bCross;
		pTrack->m_nCrossTrack = nCrossTrack;
		pTrack->m_nCrossPlcNum = nCrossPlcNum;

		pTrack->SaveXML();
		m_bNextNor = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		UpdateData(FALSE);
		pTrack->m_bModified = TRUE;
		pTrack->InvokeControl(pTrack->m_pTrackCtrl);
	}

}

void CEcsView::OnButton20()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strTrNum;

	GET(IDC_COMBO_PLC,strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO,strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	CTrackInfo* pTrack = NULL;
	pTrack = pDoc->GetTrackInfo(nTrNum, nPlcNo);

	if(pTrack == NULL)
		return;
	if (pDoc->m_pJobDlg != NULL)
	{
		pDoc->m_pJobDlg->ShowWindow(SW_SHOWNORMAL);
		pDoc->m_pJobDlg->BringWindowToTop();
	}
	else
	{
		pDoc->m_pJobDlg = new CJobDlg(pDoc, pTrack);
		pDoc->m_pJobDlg->Create(IDD_JOB);
		pDoc->m_pJobDlg->ShowWindow(SW_SHOW);
	}

}
void CEcsView::OnSelchangeComboCvKind()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strTrNum, strCvKind;

	GET(IDC_COMBO_PLC,strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO,strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	GET(IDC_COMBO_CV_KIND,strCvKind);

	int nKind = _ttoi(strCvKind.Mid(0,1));
	int nSel = m_cbxCvKind.GetCurSel();

	CTrackInfo* pTrack = NULL;
	pTrack = pDoc->GetTrackInfo(nTrNum, nPlcNo);

	if(pTrack == NULL)
		return;
	if (nSel == 0)
	{
		GetDlgItem(IDC_EDIT_NEXT_CV)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON19)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CENTER_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CENTER_DEST)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CROSS)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CROSS_TRACK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON20)->EnableWindow(FALSE);

		m_bNextNor = TRUE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;

		UpdateData(FALSE);
		strTrNum.Format(_T("%05d"),pTrack->m_nNextCv);
		SET(IDC_EDIT_NEXT_CV,strTrNum);
		strTrNum.Format(_T("%05d"),pTrack->m_nCenterNext);
		SET(IDC_EDIT_CENTER_NEXT,strTrNum);
		strTrNum.Format(_T("%05d"),pTrack->m_nCenterDest);
		SET(IDC_EDIT_CENTER_DEST,strTrNum);
		strTrNum.Format(_T("%05d"),pTrack->m_nCrossTrack);
		SET(IDC_EDIT_CROSS_TRACK,strTrNum);
	}
	else if (nSel == 1)
	{
		GetDlgItem(IDC_EDIT_NEXT_CV)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON19)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER_DEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CROSS_TRACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON20)->EnableWindow(TRUE);

		m_bNextNor = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;

		UpdateData(FALSE);
		strTrNum.Format(_T("%05d"),0);
		SET(IDC_EDIT_NEXT_CV,strTrNum);
		SET(IDC_EDIT_CENTER_NEXT,strTrNum);
		SET(IDC_EDIT_CENTER_DEST,strTrNum);
		SET(IDC_EDIT_CROSS_TRACK,strTrNum);
	}
	else
	{
		GetDlgItem(IDC_EDIT_NEXT_CV)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON19)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER_DEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CROSS_TRACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON20)->EnableWindow(FALSE);

		m_bNextNor = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;

		UpdateData(FALSE);
		strTrNum.Format(_T("%05d"),0);
		SET(IDC_EDIT_NEXT_CV,strTrNum);
		SET(IDC_EDIT_CENTER_NEXT,strTrNum);
		SET(IDC_EDIT_CENTER_DEST,strTrNum);
		SET(IDC_EDIT_CROSS_TRACK,strTrNum);
	}
}
void CEcsView::InvokeStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);

	CTrackHS* pTrackHS = NULL;
	if (pTrack == NULL || pStation == NULL)	return;
	int nDevNum = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;
	
	int nAAA, nBBB, nCCC;
	int m_nNumber = pCv->m_nNumber;
		
//	if( m_nNumber > 9)
//		nDevNum = (pTrack->m_nNumber%10-pCv->m_nStTrNum+1)*10;

	if (pTrack->m_nNumber == 1126)
		int aaa = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit15;

	if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) != enBit11)
	{
		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] != 0)
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = 0;
		//if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] != 0)
		//	m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = 0;

		return;
	}

	switch (pStation->m_enKind)
	{
	case CStationInfo::enStoStation:

		nAAA = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
		nBBB = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11;
		nCCC = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12;

		if(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0 &&
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] != pTrack->m_nNumber &&
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) != enBit13)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 999;
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = pTrack->m_nNumber;
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;
			if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) != enBit11)
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
		}
		else if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0 &&
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] != pTrack->m_nNumber &&
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) == enBit13)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit13;
			nBBB = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7];
		}
//		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 9998 &&
//			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12) != enBit12)
//		{
//			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit12;
//			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
//			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 999;
//			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = m_pTrack->m_nNumber;
//		}


		break;

	case CStationInfo::enArvStation:

		if (pTrack->m_bCenter == 0)
		{
			if (pTrackHS = pTrack->GetScStoHS())
			{
				if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) != enBit11 &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] != 1) &&
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0)
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11;
				}
			}
			else if (pTrackHS = pTrack->GetScRetHS())
			{
			}
			else if (pTrackHS = pTrack->GetRtvArriveHS())
			{
				int aaa=m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
				if(aaa= 1111)
					int a=0;
				if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11)
				{
					if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit17) == enBit17)
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit17;
				}
				else if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0) &&
						 (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11)
				{
					if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit17) == enBit17)
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit17;
				}
			}

			else
			{
				if(pTrack->m_nNumber == 1469 || pTrack->m_nNumber == 2469)
				{
					if(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] /1000 != 8 && (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11)
					{
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 8331;
					}

				}

				if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) != enBit13 &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) != enBit11)
					return;

				WORD bbb = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
				WORD ccc = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13;
				WORD ddd = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11;

				if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0) &&
					((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) == enBit13 ||
					 (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11))
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit13;
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit26;
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = 0;
				}
				else if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber &&
					((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) != enBit13 &&
					 (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11) &&
					 m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 9998)
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;
			
					if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12) != enBit12 &&
						(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 9998))
						//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit12;

					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit26;
					//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
				}
				else if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] != pTrack->m_nNumber &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) == enBit13)
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit13;
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit26;
					//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
				}
				if(pTrack->m_nDispenser != 0)
				{
					if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 9998) &&
						(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] != 9998 ))
						{
							m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 9998;
//							if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12)!= enBit12)
								//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] |= enBit12;
						}
				}
			}
		}
		else if (pTrack->m_bCenter == 1)
		{
			if(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0 && 
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0 && 
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] == 0)
			{
				pTrack->m_bStartFlag = FALSE;
			}
			if(pTrack->m_nDispenser != 0)
			{
				if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 9998) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] != 9998 ))
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 9998;
					if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12)!= enBit12)
						//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] |= enBit12;
					if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13)== enBit13)
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^= enBit13;
				}
			}
			if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
			{
				if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) != enBit13)
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;
//				if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12) != enBit12)
//					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit12;
			}

	//		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0 || m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] != pTrack->m_nNumber)
	//			return;
			if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0 || m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] /1000 != 8)
				return;
	//		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber && pTrack->m_bStartFlag != TRUE)
	//		{
	//			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;
	//			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
	//			pTrack->m_bStartFlag = TRUE;
	//			pTrack->m_tTime = CTime::GetCurrentTime();
	//		}

			if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1]/1000 == 8 && pTrack->m_bStartFlag != TRUE )
			{
				if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) != enBit13)
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;

				//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
				pTrack->m_bStartFlag = TRUE;
				pTrack->m_tTime = CTime::GetCurrentTime();
			}
	//		else if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber && pTrack->m_bStartFlag == TRUE)
			else if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1]/1000 == 8 && pTrack->m_bStartFlag == TRUE && (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] >> 12) == 1)
			{
				CTimeSpan tElapseTime = CTime::GetCurrentTime() - pTrack->m_tTime;
				if (tElapseTime.GetTotalSeconds() > 10)
				{
					int nNextPlcNum = pTrack->m_nCenterNextPlcNum-1;
					int nNextDevNum = (pTrack->m_nCenterNext-m_pDoc->m_nStTrNum[nNextPlcNum]+1)*10;

					if (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum] == 0)
					{
						pTrack->m_bStartFlag = FALSE;
				
						if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) == enBit11)
						{
							m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = 0;
							m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = enBit11;
						}
						else
							m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = 0;

						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = 0;

						int nLuggNum = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
						int nJobType = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] << 12;
						int nBarcode = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4];
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] = 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3] = 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] = 0;

						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum] = nLuggNum;
						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+1] = pTrack->m_nCenterDest;
						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+2] = nJobType *100 + 16;
						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+3] = 0;
					//	m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+4] = nBarcode;

						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] = m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] | enBit11;
					}
				}
			}
		}
		else
		{
			if (pTrackHS = pTrack->GetScRetHS())
			{
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit15;
			}
			else if (pTrackHS = pTrack->GetRtvDepartHS())
			{
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit17;
			}			
			else
			{
				if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0) ||
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) == enBit13)
					return;

				if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
				}
			}
		}
		
		break;

	case CStationInfo::enRetStation:
		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0)
			return;

		if(nDevNum == 260)
		{
			int aa = 0;
			aa = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2];

		}
		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber && pTrack->m_bStartFlag != TRUE)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
			pTrack->m_bStartFlag = TRUE;
			pTrack->m_tTime = CTime::GetCurrentTime();
		}
		else if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber && pTrack->m_bStartFlag == TRUE)
		{
			CTimeSpan tElapseTime = CTime::GetCurrentTime() - pTrack->m_tTime;
			if (tElapseTime.GetTotalSeconds() > 20)
			{
				pTrack->m_bStartFlag = FALSE;
				
				if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) == enBit11)
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = enBit11;
				else
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = 0;

				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = 0;

				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] = 0;
			}
		}

		break;

	}

}

void CEcsView::LgvStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);

	CTrackHS* pTrackHS = NULL;
	if (pTrack == NULL || pStation == NULL)	return;
	int nDevNum = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;

	int m_nNumber = pCv->m_nNumber;

	//수동상태이거나, 자동상태이면서 신호요청이 들어온 경우..(들어왔다 꺼졌을 때도)


	if (pTrack->m_bCenter == 0)
	{
		if (pTrackHS = pTrack->GetLgvDepartHS())
		{
			if (!pTrack->m_bCross)
			{
				int ii =m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4];
				//작업이 존재하고 LGV에서 신호가 들어왔을때...
				if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4]  != 0))
				{
					//화물감지 유무를 판단하여 화물이 있으면 정상 허가 신호 1  화물감지가 안되면 3
					if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11)
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] = 1;
					else
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] = 3;
				}
				else if(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0 &&
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] == 0)
				{
					if(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] != 0)
					{
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum]		= 0; 
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1]	= 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2]	= 0; 
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3]	= 0; 
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4]	= 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+5]	= 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+5]	= 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+6]	= 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8]	= 0;
						m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9]	= 0;
					}
				}				
				
			}
		}
		if (pTrackHS = pTrack->GetLgvArriveHS())
		{

			int ii =m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4];

			//정상적인 상황. 화물감지가 되어있지 않고 데이터가 존재하지 않을때.. D10n+9 = 0
			if (((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) != enBit11) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0)				  &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] == 0))
			{
				if(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] != 0 &&
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] != 4 &&
					((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) != enBit11))
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8]  |= enBit11;
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] = 0;
				}
				else if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) != enBit11)
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] = 0;
				else if(((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) != enBit11) && 
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0 &&
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] == 0 &&
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] == 4)
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] = 0;
 				
			}
			//화물감지가 되어 있지 않고 LGV에서 트랙으로 도착 신호 받았을 때.. 
			else if(((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) != enBit11) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] != 0))
			{
				if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0) ||
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] !=0) ||
					((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11))
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] = 4;
				}
				else if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] ==0) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] ==2))
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] = 2;
				}

			}
			else if(((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11) &&
					((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12) == enBit12) &&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] ==0)					&&
					(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] ==0))
			{
				if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] ==2))
				{
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] = 2;
				}
				else
					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+9] = 0;
				
			}
		}
	}

}

void CEcsView::InvokeHS(CCv* pCv, CTrackInfo* pTrack)
{
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	
	CTrackHS* pTrackHS = NULL;
	if (pTrack == NULL)	return;

	int nDevNum = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;
		
	int m_nNumber = pCv->m_nNumber;

	if (pTrack->GetRtvArriveHsChk() == TRUE)
	{
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit17;
		}
	}
	
	if (pTrack->GetRtvDepartHSChk() == TRUE )
	{
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit16;
		}
		
	}
	
	if ((pTrack->GetRtvArriveHsChk() == TRUE) && (pTrack->GetRtvDepartHSChk() == TRUE))
	{
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0) &&
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit16;
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit17;
		}
		
	}	

}

void CEcsView::MoveNextTrackForKindNormal_1(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	CTrackInfo* pNextTrack = NULL;

	
	if(pTrack->m_bTwinCheck == TRUE)
	{
		int m_nNumber = pCv->m_nNumber;
		int nDevNum = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;
		int nHSDev = (pTrack->m_nHSTrack-m_pDoc->m_nStTrNum[m_nNumber-1]+1)*10;			
		
		int a = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
		int b = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1];
		bool c = (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11;
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0) &&
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nHSTrack) &&
			((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11))
		{
			if ((m_pDoc->m_arrRegData[m_nNumber-1][nHSDev+7] & enBit13) == enBit13 || 
				(m_pDoc->m_arrRegData[m_nNumber-1][nHSDev+7] & enBit14) == enBit14)
				return;
		}
		
	}
	int nNumber = pCv->m_nNumber;

	int nNextPlcNum = pTrack->m_nNextPlcNum-1;
	int nDevNum = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;
//	if(nNumber > 9)
//		nDevNum = (pTrack->m_nNumber%10-pCv->m_nStTrNum+1)*10;

	int nNextDevNum = (pTrack->m_nNextCv-m_pDoc->m_nStTrNum[pTrack->m_nNextPlcNum-1]+1)*10;

	//if (nNextDevNum > 10000) return;

	if(nNextPlcNum==11)
		int aa =0;
	if(nNextDevNum > 10000)
		return;

	int nCurrAuto = (m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] & enBit11);
	int nNextAuto = (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+7] & enBit11);
	int nCurrSensor = (m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] & enBit11);
	int nNextSensor = (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] & enBit11);
	if(nNextDevNum > 10000)
		return;
	// 자동이 아니면
	if (!(nCurrAuto == enBit11 && nNextAuto == enBit11))
		return;


	if (m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] == pTrack->m_nNumber)
		return;
	
	if (nCurrSensor == enBit11 && nNextSensor == enBit11)
		return;



	if (!((m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum] == 0 && nNextSensor != enBit11) &&
		  (m_pDoc->m_arrRegData[nNumber-1][nDevNum] != 0 && nCurrSensor == enBit11)))
		return;

	if ((m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] & enBit17) == enBit17)
		return;

	if ((m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+7] & enBit14) == enBit14)
		return;

	if (pTrack->m_bCross)
	{
		int nCrossPlcNum = pTrack->m_nCrossPlcNum;
		int nCrossDevNum = (pTrack->m_nCrossTrack-m_pDoc->m_nStTrNum[pTrack->m_nCrossPlcNum-1]+1)*10;

		CTrackHS* pTrackHS = NULL;
		if (pTrackHS = pTrack->GetScStoHS())
		{
			if ((m_pDoc->m_arrRegData[nCrossPlcNum][nCrossDevNum+7] & enBit14) == enBit14)
			{
				return;
			}
		}
		else
		{
			if (m_pDoc->m_arrRegData[nCrossPlcNum][nCrossDevNum] != 0)
			{
				pNextTrack = m_pDoc->GetTrackInfo(pTrack->m_nNextCv, nNextPlcNum);
				if (pNextTrack->m_nGoTrack == pTrack->m_nNumber)
					return;
			}
		}
	}

	if (nNextSensor != enBit11 && pTrack->m_bStartFlag != TRUE)
	{
		pTrack->m_bStartFlag = TRUE;
		pTrack->m_tTime = CTime::GetCurrentTime();
	}
	else if (nNextSensor != enBit11 && pTrack->m_bStartFlag == TRUE)
	{
		CTimeSpan tElapseTime = CTime::GetCurrentTime() - pTrack->m_tTime;
		if (tElapseTime.GetTotalSeconds() > 2)
		{
			pTrack->m_bStartFlag = FALSE;
			m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] = m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] | enBit11;
			m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] = m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] | enBit12;
			if (nCurrSensor == enBit11)
			{
				m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] ^ enBit11;
				m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] ^ enBit12;
			}

			if (nCurrAuto == enBit11)
				m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] = enBit11;
			else
				m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] = 0;

			if (pNextTrack != NULL)
				pNextTrack->m_nGoTrack = pTrack->m_nNumber;
		}
	}
}

void CEcsView::MoveNextTrackForKindDiverter_1(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	CTrackInfo* pNextTrack = NULL;

	int nNumber = pCv->m_nNumber;

	int nStTrNum = pCv->m_nStTrNum;
	int nDevNum = (pTrack->m_nNumber-nStTrNum+1)*10;
//	if(nNumber > 9)
//		nDevNum = (pTrack->m_nNumber%10-nStTrNum+1)*10;

	if(pTrack->m_nNumber == 13044)
		int kk = 0;
	int nLen = pTrack->m_nStationArray.GetSize();
	
	for (int i=0; i<nLen; i++)
	{
		int nStation = pTrack->m_nStationArray[i];
		CStationInfo* pStationInfo = m_pDoc->m_pStationInfos[i];

		int a = m_pDoc->m_arrRegData[nNumber-1][nDevNum];
		a = m_pDoc->m_arrRegData[nNumber-1][nDevNum+1];
		if ((nStation != 0 && nStation == m_pDoc->m_arrRegData[nNumber-1][nDevNum+1]) || (pStationInfo!= NULL && _ttoi(pStationInfo->m_strID) ==  m_pDoc->m_arrRegData[nNumber-1][nDevNum+1]))
		{
			int nNextTrNum = pTrack->m_nNextTrArray[i];
			if (nNextTrNum == 0)
				continue;
			
			int nNextPlcNum = pTrack->m_nNextPlcArray[i]-1;
			int nNextDevNum = (nNextTrNum-m_pDoc->m_nStTrNum[nNextPlcNum]+1)*10;
			
			int nCurrAuto = (m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] & enBit11);
			int nNextAuto = (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+7] & enBit11);
			int nCurrSensor = (m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] & enBit11);
			int nNextSensor = (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] & enBit11);
			
			//2015.8.26 10g 양품 36개 트랙만 10개 쌓음
			int nFlag = 0;
			int nCurrTrNum[37] = {10357, 10354, 10351, 10348, 10345, 10342, 10339, 10336, 10333, 10330,
								  10327, 10324, 10321, 10318, 10315, 10312, 10309, 10306,
								  10302, 10299, 10295, 10293, 10290, 10287, 10284, 10281, 10278, 10275,
								  10272, 10269, 10266, 10263, 10260, 10257, 10254, 10251};


			if (!(nCurrAuto == enBit11 && nNextAuto == enBit11))
				return;

			if(pTrack->m_nNumber == 9006)
				int aa = 0;
			if ( pTrack->m_nDispenser != 0 && pTrack->m_nDestPos == 9999)
			{
				UnLoadDP(pCv, pTrack, nNextPlcNum, nNextTrNum);
			
				return;
			}	
			//2015.8.26 10G 양품 10개 쌓이면 출발 하기 위함...
			//for(int i =0; i < 36; i++)
			//{
			//	if(nCurrTrNum[i] == nNextTrNum)
			//	{
			//		nFlag =1;
			//		break;
			//	}
			//	else
			//		nFlag=0;
				
			//}
			if(nCurrSensor != enBit11)
				return;			

			if (nCurrSensor == enBit11 && nNextSensor == enBit11)
			{
				if ((pStation = pTrack->GetStoStation()) || (pStation = pTrack->GetArvStation()))
					LoadMG(pCv, pTrack, nNextPlcNum, nNextTrNum);	
				return;
			}
			if (!((m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum] == 0 && nNextSensor != enBit11) &&
				( nCurrSensor == enBit11)))
				return;
					
			if (pTrack->m_bCross)
			{
				int nCrossPlcNum = pTrack->m_nCrossPlcNum;
				int nCrossDevNum = (pTrack->m_nCrossTrack-m_pDoc->m_nStTrNum[pTrack->m_nCrossPlcNum]+1)*10;

				if (nNextTrNum == pTrack->m_nCrossDest && m_pDoc->m_arrRegData[nCrossPlcNum][nCrossDevNum] != 0)
				{
					pNextTrack = m_pDoc->GetTrackInfo(nNextTrNum, nNextPlcNum);
					if (pNextTrack->m_nGoTrack == pTrack->m_nNumber)
						return;
				}
			}
			
			if ((nNextSensor != enBit11 && pTrack->m_bStartFlag != TRUE) || (nFlag == 1 && pTrack->m_bStartFlag != TRUE))
			{
				pTrack->m_bStartFlag = TRUE;
				pTrack->m_tTime = CTime::GetCurrentTime();
			}
			else if ((nNextSensor != enBit11 && pTrack->m_bStartFlag == TRUE) || (nFlag == 1 && pTrack->m_bStartFlag == TRUE))
			{
				CTimeSpan tElapseTime = CTime::GetCurrentTime() - pTrack->m_tTime;
				if (tElapseTime.GetTotalSeconds() > 2)
				{
					int Ccc = m_pDoc->m_arrRegData[nNumber-1][nDevNum+8];

					pTrack->m_bStartFlag = FALSE;
					m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] = m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] | enBit11;
					m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] = m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] | enBit12;
					if (nCurrSensor == enBit11)
					{
						m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] ^ enBit11;
						m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] ^ enBit12;
					}

					if (nCurrAuto == enBit11)
						m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] = enBit11;
					//else
						//m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] = 0;

					if (nNextTrNum == pTrack->m_nCrossDest && pNextTrack != NULL)
						pNextTrack->m_nGoTrack = pTrack->m_nNumber;

					CString strLog;
					strLog.Format(_T(""));
					m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, _T(""), _T("CEcsView::MoveNextTrackForKindDiverter_1"));

				}

			}

		}	
	}

}

void CEcsView::MoveNextTrackForKindNormal_2(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	int nNumber = pCv->m_nNumber;
	int nDevNum = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;
//	if(nNumber > 9)
//		nDevNum = (pTrack->m_nNumber%10-pCv->m_nStTrNum+1)*10;
	int nNextPlcNum = pTrack->m_nNextPlcNum-1;
	int nNextDevNum = (pTrack->m_nNextCv-m_pDoc->m_nStTrNum[nNextPlcNum]+1)*10;

	//if (nNextDevNum > 10000) return;
	if(nNextDevNum > 10000)
		return;
	int nCurrAuto = (m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] & enBit11);
	int nNextAuto = (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+7] & enBit11);
	int nCurrSensor = (m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] & enBit11);
	int nNextSensor = (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] & enBit11);

	if(nNextDevNum > 10000)
		return;
	// 자동이 아니면
	if (!(nCurrAuto == enBit11 && nNextAuto == enBit11))
		return;

	if (m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] == pTrack->m_nNumber)
		return;
	
	if (nCurrSensor == enBit11 && nNextSensor == enBit11)
		return;

	int aa = m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum];
	aa = m_pDoc->m_arrRegData[nNumber-1][nDevNum];
	if (!((m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum] == 0 && nNextSensor == enBit11) &&
		  (m_pDoc->m_arrRegData[nNumber-1][nDevNum] != 0 && nCurrSensor != enBit11)))
		return;

	CTrackInfo* pTrackTemp = m_pDoc->GetTrackInfo((nNextDevNum/10)  + ((nNextPlcNum+1)*1000));
	CStationInfo* pStationTemp = NULL;
	
	if(pTrackTemp != NULL)
		pStationTemp = pTrackTemp->GetArvStation();
//	pTemp = pTemp->GetArvStation();
	if(pStationTemp != NULL)
	{
		m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+7] |= enBit25;
		Sleep(100);
	}
	
	m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum]   = m_pDoc->m_arrRegData[nNumber-1][nDevNum];
	m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+1] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+1];
	m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+2] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+2];
	m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+3] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+3];

	m_pDoc->m_arrRegData[nNumber-1][nDevNum] = 0;
	m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] = 0;
	m_pDoc->m_arrRegData[nNumber-1][nDevNum+2] = 0;
	m_pDoc->m_arrRegData[nNumber-1][nDevNum+3] = 0;


}

void CEcsView::MoveNextTrackForKindDiverter_2(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	int nNumber = pCv->m_nNumber;

	int nStTrNum = pCv->m_nStTrNum;
	int nDevNum = (pTrack->m_nNumber-nStTrNum+1)*10;
//	if(nNumber >9)
//		nDevNum = (pTrack->m_nNumber%10-nStTrNum+1)*10;
	int nLen = pTrack->m_nStationArray.GetSize();
	
	for (int i=0; i<nLen; i++)
	{
		int nStation = pTrack->m_nStationArray[i];
		int aa = m_pDoc->m_arrRegData[nNumber-1][nDevNum+1];
		CStationInfo* pStationInfo = m_pDoc->m_pStationInfos[i];

		if ((nStation != 0 && nStation == m_pDoc->m_arrRegData[nNumber-1][nDevNum+1]) || (pStationInfo!= NULL && _ttoi(pStationInfo->m_strID) ==  m_pDoc->m_arrRegData[nNumber-1][nDevNum+1]))
		{
			int nNextTrNum = pTrack->m_nNextTrArray[i];
			if(nNextTrNum == 0)
				continue;
			int nNextPlcNum = pTrack->m_nNextPlcArray[i]-1;
			int nNextDevNum = (nNextTrNum-m_pDoc->m_nStTrNum[nNextPlcNum]+1)*10;

			int nCurrAuto = (m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] & enBit11);
			int nNextAuto = (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+7] & enBit11);
			int nCurrSensor = (m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] & enBit11);
			int nNextSensor = (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+8] & enBit11);
		
			//2015.8.26 10g 양품 36개 트랙만 10개 쌓음
			int nFlag = 0;
			int nCurrTrNum[37] = {10357, 10354, 10351, 10348, 10345, 10342, 10339, 10336, 10333, 10330,
								  10327, 10324, 10321, 10318, 10315, 10312, 10309, 10306,
								  10302, 10299, 10295, 10293, 10290, 10287, 10284, 10281, 10278, 10275,
								  10272, 10269, 10266, 10263, 10260, 10257, 10254, 10251};

			if (!(nCurrAuto == enBit11 && nNextAuto == enBit11))
				return;

			if (m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] == pTrack->m_nNumber)
				return;

			//2015.8.26 10g 양품 수정
			for(int i =0; i < 36; i++)
			{
				if(nCurrTrNum[i] == nNextTrNum)
				{
					nFlag =1;
					break;

				}
				else
					nFlag =0;					
			}

			
			if(nFlag == 0)
			{
				if (nCurrSensor == enBit11 && nNextSensor == enBit11)
					return;

				if (!((m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum] == 0 && nNextSensor == enBit11) &&
					  ( nCurrSensor != enBit11)))
					  return;
				
			}
			else if(nFlag == 1)
			{
				if (!(nCurrSensor == 0 && nNextSensor == 1))
					return;			
						
			}
//			if (nCurrSensor == enBit11 && nNextSensor == enBit11)
//				return;

//			int aaa = m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum];
//				aa = m_pDoc->m_arrRegData[nNumber-1][nDevNum];
//			if (!((m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum] == 0 && nNextSensor == enBit11) &&
//				  (m_pDoc->m_arrRegData[nNumber-1][nDevNum] != 0 && nCurrSensor != enBit11)))
//				return;

			m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum]   = m_pDoc->m_arrRegData[nNumber-1][nDevNum];
			m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+1] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+1];
			m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+2] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+2];
			m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+3] = m_pDoc->m_arrRegData[nNumber-1][nDevNum+3];

			m_pDoc->m_arrRegData[nNumber-1][nDevNum] = 0;
			m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] = 0;
			m_pDoc->m_arrRegData[nNumber-1][nDevNum+2] = 0;
			m_pDoc->m_arrRegData[nNumber-1][nDevNum+3] = 0;
			m_pDoc->m_arrRegData[nNumber-1][nDevNum+4] = 0;

			if(pTrack->m_nMGTrack != 0)
			{
				int nMGDevNum  =  (pTrack->m_nMGTrack - m_pDoc->m_nStTrNum[pTrack->m_nMGPlcNum]+1)*10;
				m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum] = 0;
				m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum+1] = 0;
				m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum+2] = 0;
				m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum+3] = 0;
				m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum+4] = 0;
				m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum+6] = 0;
				if((m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum+7] & enBit11) == enBit11)
				{
					m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum+7] = 0;
					m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum+7] = enBit11;
				}
				m_pDoc->m_arrRegData[pTrack->m_nMGPlcNum][nMGDevNum+8] = 0;
				pTrack->m_nMGTrack = 0; 
				pTrack->m_nMGPlcNum = 0;
			}
		}
		
	}

}

void CEcsView::LoadMG(CCv* pCv, CTrackInfo* pTrack, int nNextPlcNum, int nNextTrNum)
{
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	CTrackInfo* pNextTrack = NULL;

	int nNumber = pCv->m_nNumber;

	int nStTrNum = pCv->m_nStTrNum;
	int nDevNum = (pTrack->m_nNumber-nStTrNum+1)*10;
	int nNextDevNum = (nNextTrNum-m_pDoc->m_nStTrNum[nNextPlcNum]+1)*10;

	if(m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+1] == 9999 && m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] == 9999)
	{
		m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] = 9998;
		m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] |= enBit12;
		pTrack->m_nMGTrack = nNextTrNum;
		pTrack->m_nMGPlcNum = nNextPlcNum;

		//if((m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] & enBit12) != enBit12)
			//m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] |= enBit12;
	}
	else if(m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+1] == 0 && m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] == 9999)
	{
		pTrack->m_nMGTrack = 0;
		pTrack->m_nMGPlcNum = 0;

	}
}


void CEcsView::UnLoadDP(CCv* pCv, CTrackInfo* pTrack, int nNextPlcNum, int nNextTrNum)
{
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	CTrackInfo* pNextTrack = NULL;

	int nNumber = pCv->m_nNumber;

	int nStTrNum = pCv->m_nStTrNum;
	int nDevNum = (pTrack->m_nNumber-nStTrNum+1)*10;
	int nNextDevNum = (nNextTrNum-m_pDoc->m_nStTrNum[nNextPlcNum]+1)*10;

	if(pTrack->m_nDispenser !=0)
	{
		int nDPPlcNum = pTrack->m_nDPPlcNum-1;
		int nDPDevNum = (pTrack->m_nDispenser-m_pDoc->m_nStTrNum[nDPPlcNum]+1)*10;

		if(m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] == 9999 )//&& 
		  // (m_pDoc->m_arrRegData[nNumber-1][nDevNum+4] >> 12) != 0	&& 
		  // (m_pDoc->m_arrRegData[nNumber-1][nDevNum+3] >> 4) ==  2	)
		{
			if(((m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] & enBit11) != enBit11 ||
			   (m_pDoc->m_arrRegData[nNumber-1][nDevNum+8] & enBit12) != enBit12 )||
			   (m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum+8] & enBit11) == enBit11)
			   return;
			
			m_pDoc->m_arrRegData[nNumber-1][nDevNum] = 9998;
			m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] = 9998;
			m_pDoc->m_arrRegData[nNumber-1][nDevNum+4] ^= (1 << 12); 
			//m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] |= enBit12;
			m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] ^= enBit13;

			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum] = 9998;
			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum+1] = pTrack->m_nNumber;
			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum+2] |= 6;
			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum+3] |= 0x0010;
			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum+8] ^= enBit11;

		}

		if(m_pDoc->m_arrRegData[nNumber-1][nDevNum+1] == pTrack->m_nNumber &&
			(m_pDoc->m_arrRegData[nNumber-1][nDevNum+3] & enBit15) == enBit15 &&
			(m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] & enBit12) != enBit12)
		{
			//m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] |= enBit12;
		}

	}

}

void CEcsView::OnCheckNextNor() 
{
	UpdateData(TRUE);
	
	if (m_bNextNor)
	{
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
	}

	UpdateData(FALSE);
	
}

void CEcsView::OnCheckCenterNext() 
{
	UpdateData(TRUE);
	
	if (m_bCenterNext)
	{
		m_bNextNor = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
	}

	UpdateData(FALSE);
	
}

void CEcsView::OnCheckCenterDest() 
{
	UpdateData(TRUE);
	
	if (m_bCenterDest)
	{
		m_bCenterNext = FALSE;
		m_bNextNor = FALSE;
		m_bCrossTrack = FALSE;
	}

	UpdateData(FALSE);
	
}

void CEcsView::OnCheckCrossTrack() 
{
	UpdateData(TRUE);
	
	if (m_bCrossTrack)
	{
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bNextNor = FALSE;
	}

	UpdateData(FALSE);
	
}

void CEcsView::OnCheckCenter() 
{
	UpdateData(TRUE);
	
	if (m_bCenter)
	{
		m_bNextNor = FALSE;
		m_bCenterNext = TRUE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
	}

	UpdateData(FALSE);
	
}

void CEcsView::OnCheckCross() 
{
	UpdateData(TRUE);
	
	if (m_bCross)
	{
		m_bNextNor = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = TRUE;
	}

	UpdateData(FALSE);
	
}

void CEcsView::OnButton23() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strTrNum, strCvKind, strNextCv;

	GET(IDC_COMBO_PLC,strPlcNo);	
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO,strTrNum);	
	if (strTrNum.IsEmpty()) return;
		
	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);
	CCv* pCv = (CCv*) pDoc->GetEquipment(CEquipment::enCV, nPlcNo);
	if(pCv == NULL)
		return;
	CTrackInfo* pTrack = NULL;
	for ( int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
	{
		pTrack = pCv->m_pInfo->m_pTracks[i];

		int nDeviceNo = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;

		if ((pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+7] & enBit11) == enBit11)
			pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+7] = pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+7] ^ enBit11;
	}
	
}

void CEcsView::OnButtonSave() 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	for (int i = 1; i < 25; ++i)

	{
		CCv* pCv = (CCv*) pDoc->GetEquipment(CEquipment::enCV, i);
	
		if(pCv == NULL)
			continue;
		CTrackInfo* pTrack = NULL;
		for ( int j = 0; j < pCv->m_pInfo->m_pTracks.GetSize(); ++j)
		{
			pTrack = pCv->m_pInfo->m_pTracks[j];

			pTrack->SaveXML();
		}
	}
	
}

void CEcsView::OnBtnAllauto() 
{
	// TODO: Add your control notification handler code here
	//nPlcNo = 7;

	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	for(int j = 0; j < 40; j++)
	{
 		CCv* pCv = (CCv*) pDoc->GetEquipment(CEquipment::enCV, j);

		CTrackInfo* pTrack = NULL;

		if(pCv == NULL)
			continue;
	

		for ( int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
		{
			pTrack = pCv->m_pInfo->m_pTracks[i];

			int nDeviceNo = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;
			pDoc->m_arrRegData[j-1][nDeviceNo+7] = pDoc->m_arrRegData[j-1][nDeviceNo+7] | enBit11;
		}
	}
	
}

void CEcsView::OnBtnAllSuspend() 
{
	// TODO: Add your control notification handler code here

	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	for(int j = 0; j < 40; j++)
	{
	
		CCv* pCv = (CCv*) pDoc->GetEquipment(CEquipment::enCV, j);
		if(pCv == NULL)
			continue;
		CTrackInfo* pTrack = NULL;

		for ( int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
		{
			pTrack = pCv->m_pInfo->m_pTracks[i];

			int nDeviceNo = (pTrack->m_nNumber-pCv->m_nStTrNum+1)*10;

			if ((pDoc->m_arrRegData[j-1][nDeviceNo+7] & enBit11) == enBit11)
				pDoc->m_arrRegData[j-1][nDeviceNo+7] = pDoc->m_arrRegData[j-1][nDeviceNo+7] ^ enBit11;
		}	
	}
}

void CEcsView::OnSysControl() 
{
	// TODO: Add your command handler code here
	
}



void CEcsView::OnMouseMoveSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point )
{
	CTimeSpan tTimeSpan;
	tTimeSpan = CTime::GetCurrentTime() - pDoc->m_tChecktime;
	if(tTimeSpan.GetTotalSeconds() < 1)
		return;
	
	CEcsLayout* pLayout = (CEcsLayout*)pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;
	
	if(pDoc->m_bMoveFlag == TRUE) //2
	{
		if(tTimeSpan.GetTotalSeconds() < 1)
			return;
		if(pDoc->m_PrevPoint != point)
		{
			if(pDoc->m_PrevPoint.x > point.x)
			{
				if(pDoc->m_PrevPoint.x - point.x > 3)
				{
					if (m_tabLayout.GetSafeHwnd())
					{
						//탭 별로 onkeydown 메세지를 던져준다.
						switch (m_tabLayout.GetCurSel())
						{	
						case EN_1F:					pDoc->m_layout1.OnKeyDown(this, VK_RIGHT, 0, 0);				break;
						case EN_2F :				pDoc->m_layout2.OnKeyDown(this, VK_RIGHT, 0, 0);				break;
						case EN_Assemble:			pDoc->m_layoutAssemble.OnKeyDown(this, VK_RIGHT, 0, 0);			break;
						}
					}
					//pLayout->OnKeyDown(this, VK_RIGHT, 0, 0);	
					
					pDoc->m_PrevPoint.x = point.x;
				}
			}
			else if(pDoc->m_PrevPoint.x < point.x)
			{
				if(point.x - pDoc->m_PrevPoint.x > 3)
				{
					//탭 별로 onkeydown 메세지를 던져준다.
					switch (m_tabLayout.GetCurSel())
					{	
						case EN_1F:							pDoc->m_layout1.OnKeyDown(this, VK_LEFT, 0, 0);			break;
						case EN_2F:							pDoc->m_layout2.OnKeyDown(this, VK_LEFT, 0, 0);			break;
						case EN_Assemble:					pDoc->m_layoutAssemble.OnKeyDown(this, VK_LEFT, 0, 0);	break;
					}

					//pLayout->OnKeyDown(this, VK_LEFT, 0, 0);
					
					pDoc->m_PrevPoint.x = point.x;
				}
			}
			if(pDoc->m_PrevPoint.y > point.y)
			{
				if(pDoc->m_PrevPoint.y - point.y > 5)
				{
					//탭 별로 onkeydown 메세지를 던져준다.
					switch (m_tabLayout.GetCurSel())
					{	
						case EN_1F:						pDoc->m_layout1.OnKeyDown(this, VK_DOWN, 0, 0);					break;
						case EN_2F:						pDoc->m_layout2.OnKeyDown(this, VK_DOWN, 0, 0);					break;
						case EN_Assemble:				pDoc->m_layoutAssemble.OnKeyDown(this, VK_DOWN, 0, 0);			break;
					}
					//pLayout->OnKeyDown(this, VK_DOWN, 0, 0);			
					pDoc->m_PrevPoint.y = point.y;
				}
			}
			else if(pDoc->m_PrevPoint.y < point.y)
			{
				if(point.y - pDoc->m_PrevPoint.y > 5)
				{
					//탭 별로 onkeydown 메세지를 던져준다.
					switch (m_tabLayout.GetCurSel())
					{	
						case EN_1F:							pDoc->m_layout1.OnKeyDown(this, VK_UP, 0, 0);			break;
						case EN_2F:							pDoc->m_layout2.OnKeyDown(this, VK_UP, 0, 0);			break;
						case EN_Assemble:					pDoc->m_layoutAssemble.OnKeyDown(this, VK_UP, 0, 0);	break;
					}
					//pLayout->OnKeyDown(this, VK_UP, 0, 0);
					
					pDoc->m_PrevPoint.y = point.y;
				}
			}
		}
	}
}



void CEcsView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	
	OnMouseMoveSelectedLayout(pDoc, nFlags, point);
	CFormView::OnMouseMove(nFlags, point);
}

void CEcsView::PreTranslateMessageSelectedLayout( CEcsDoc * pDoc, WPARAM wParam, int param3, int param4 )
{
	
	if (m_tabLayout.GetSafeHwnd())
	{
		//탭 별로 onkeydown 메세지를 던져준다.
		switch (m_tabLayout.GetCurSel())
		{	
		case EN_1F:						pDoc->m_layout1.OnKeyDown(this, wParam, 0, 0);			break;
        case EN_2F:						pDoc->m_layout2.OnKeyDown(this, wParam, 0, 0);			break;
		case EN_Assemble:				pDoc->m_layoutAssemble.OnKeyDown(this, wParam, 0, 0);	break;
		}
	}
}


BOOL CEcsView::PreTranslateMessage(MSG* pMsg) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);	

	
	if (pMsg->message == WM_KEYDOWN) 
	{
		PreTranslateMessageSelectedLayout(pDoc, pMsg->wParam, 0, 0);
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}
//*/

