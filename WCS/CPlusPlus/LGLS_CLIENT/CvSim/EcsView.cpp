// EcsView.cpp : implementation of the CEcsView class
//

#include "stdafx.h"
#include "Ecs.h"

#include "ByteArrayEx.h"

#include "EcsDoc.h"
#include "EcsView.h"
#include "MainFrm.h"

#include "ScDlg.h"
//#include "BcrDlg.h"

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
	ON_COMMAND(ID_UNSET_ALL_SUSPEND, OnUnSetAllSuspend)
	ON_COMMAND(ID_SET_ALL_SUSPEND, OnSetAllSuspend)
	ON_NOTIFY(TCN_SELCHANGE, IDC_LAYOUT_TAB, OnSelchangeLayoutTab)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_NOTIFY(NM_CLICK, IDC_LIST_REG_DATA, OnClickListRegData)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_CBN_SELCHANGE(IDC_COMBO_CV_KIND, OnSelchangeComboCvKind)
	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
	ON_BN_CLICKED(IDC_CHECK_NEXT_NOR, OnCheckNextNor)
	ON_BN_CLICKED(IDC_CHECK_CENTER_NEXT, OnCheckCenterNext)
	ON_BN_CLICKED(IDC_CHECK_CENTER_DEST, OnCheckCenterDest)
	ON_BN_CLICKED(IDC_CHECK_CROSS_TRACK, OnCheckCrossTrack)
	ON_BN_CLICKED(IDC_CHECK_CENTER, OnCheckCenter)
	ON_BN_CLICKED(IDC_CHECK_CROSS, OnCheckCross)
	ON_BN_CLICKED(IDC_BUTTON23, OnButton23)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BTN_ALLAUTO, OnBtnAllauto)
	ON_BN_CLICKED(IDC_BTN_ALLAUTO2, OnBtnAllSuspend)
	ON_COMMAND(ID_SYS_CONTROL, OnSysControl)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()
//void CEcsView::OnMouseMove(UINT nFlags, CPoint point)
/////////////////////////////////////////////////////////////////////////////
// CEcsView construction/destruction

CEcsView::CEcsView()
	: CFormView(CEcsView::IDD)
{
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
	DDX_Control(pDX, IDC_COMBO_CV_KIND, m_cbxCvKind);
	DDX_Control(pDX, IDC_LIST_REG_DATA, m_lstRegList);
	DDX_Check(pDX, IDC_CHECK1, m_bBit01);
	DDX_Check(pDX, IDC_CHECK2, m_bBit02);
	DDX_Check(pDX, IDC_CHECK3, m_bBit03);
	DDX_Check(pDX, IDC_CHECK4, m_bBit04);
	DDX_Check(pDX, IDC_CHECK5, m_bBit05);
	DDX_Check(pDX, IDC_CHECK6, m_bBit06);
	DDX_Check(pDX, IDC_CHECK7, m_bBit07);
	DDX_Check(pDX, IDC_CHECK8, m_bBit08);
	DDX_Check(pDX, IDC_CHECK9, m_bBit09);
	DDX_Check(pDX, IDC_CHECK10, m_bBit10);
	DDX_Check(pDX, IDC_CHECK11, m_bBit11);
	DDX_Check(pDX, IDC_CHECK12, m_bBit12);
	DDX_Check(pDX, IDC_CHECK13, m_bBit13);
	DDX_Check(pDX, IDC_CHECK14, m_bBit14);
	DDX_Check(pDX, IDC_CHECK15, m_bBit15);
	DDX_Check(pDX, IDC_CHECK16, m_bBit16);

	DDX_Check(pDX, IDC_CHECK17, m_bBit17);
	DDX_Check(pDX, IDC_CHECK18, m_bBit18);
	DDX_Check(pDX, IDC_CHECK19, m_bBit19);
	DDX_Check(pDX, IDC_CHECK20, m_bBit20);
	DDX_Check(pDX, IDC_CHECK21, m_bBit21);
	DDX_Check(pDX, IDC_CHECK22, m_bBit22);
	DDX_Check(pDX, IDC_CHECK23, m_bBit23);
	DDX_Check(pDX, IDC_CHECK24, m_bBit24);
	DDX_Check(pDX, IDC_CHECK25, m_bBit25);
	DDX_Check(pDX, IDC_CHECK26, m_bBit26);
	DDX_Check(pDX, IDC_CHECK27, m_bBit27);
	DDX_Check(pDX, IDC_CHECK28, m_bBit28);
	DDX_Check(pDX, IDC_CHECK29, m_bBit29);
	DDX_Check(pDX, IDC_CHECK30, m_bBit30);
	DDX_Check(pDX, IDC_CHECK31, m_bBit31);
	DDX_Check(pDX, IDC_CHECK32, m_bBit32);

	DDX_Check(pDX, IDC_CHECK33, m_bBit33);
	DDX_Check(pDX, IDC_CHECK34, m_bBit34);
	DDX_Check(pDX, IDC_CHECK35, m_bBit35);
	DDX_Check(pDX, IDC_CHECK36, m_bBit36);
	DDX_Check(pDX, IDC_CHECK37, m_bBit37);
	DDX_Check(pDX, IDC_CHECK38, m_bBit38);
	DDX_Check(pDX, IDC_CHECK39, m_bBit39);
	DDX_Check(pDX, IDC_CHECK40, m_bBit40);
	DDX_Check(pDX, IDC_CHECK41, m_bBit41);
	DDX_Check(pDX, IDC_CHECK42, m_bBit42);
	DDX_Check(pDX, IDC_CHECK43, m_bBit43);
	DDX_Check(pDX, IDC_CHECK44, m_bBit44);
	DDX_Check(pDX, IDC_CHECK45, m_bBit45);
	DDX_Check(pDX, IDC_CHECK46, m_bBit46);
	DDX_Check(pDX, IDC_CHECK47, m_bBit47);
	DDX_Check(pDX, IDC_CHECK48, m_bBit48);

	DDX_Check(pDX, IDC_CHECK_NEXT_NOR, m_bNextNor);
	DDX_Check(pDX, IDC_CHECK_CENTER, m_bCenter);
	DDX_Check(pDX, IDC_CHECK_CENTER_NEXT, m_bCenterNext);
	DDX_Check(pDX, IDC_CHECK_CENTER_DEST, m_bCenterDest);
	DDX_Check(pDX, IDC_CHECK_CROSS, m_bCross);
	DDX_Check(pDX, IDC_CHECK_CROSS_TRACK, m_bCrossTrack);

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

	int i = 0;
	CEquipment* pEquipment = NULL;
	for (i = 0; i < pDoc->m_pEquipments.GetSize(); ++i)
	{
		pEquipment = pDoc->m_pEquipments[i];

		if (pEquipment == NULL)
			continue;

		//DEBUGER_ASSERT_VALID(pEquipment != NULL);
		//DEBUGER_ASSERT_VALID(pEquipment->GetInfo() != NULL);

		if (pEquipment->m_enKind == CEquipment::enCV)
		{
			pEquipment->RunThread();
			for (int i = 0; i < PLC_CONN_PORT_CNT; i++)
			{
				pEquipment->RunServer(i);
			}
		}
	}

	// 레이아웃당 1개의 탭을 구성할 수 있다.  - 레이아웃에 여러대의 SC를 넣을 수 있다. 
	int nPlcCount = 0;
	int nLayoutCnt = pDoc->m_pEcsLayOuts.GetSize();
	for (i = 0; i < nLayoutCnt; ++i)
	{
		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[i];

		if (pEcsLayout == NULL)
			continue;

		m_tabLayout.InsertItem(nPlcCount, pEcsLayout->m_strText, nPlcCount);
		m_tabLayout.HighlightItem(nPlcCount++, FALSE);
		m_tabLayout.SetItemSize(CSize(200, 20));				// ?
	}

	UINT uTimerID = 0;

	m_nMainTimerID = SetTimer(ID_MAIN_TIMER, 100, NULL);

	m_cbxPlcNo.AddString(_T("01"));
	m_cbxPlcNo.AddString(_T("02"));
	m_cbxPlcNo.AddString(_T("03"));
	m_cbxPlcNo.AddString(_T("04"));
	m_cbxPlcNo.AddString(_T("05"));
	m_cbxPlcNo.AddString(_T("06"));
	m_cbxPlcNo.AddString(_T("07"));
	m_cbxPlcNo.AddString(_T("08"));
	m_cbxPlcNo.AddString(_T("09"));
	m_cbxPlcNo.AddString(_T("10"));
	m_cbxPlcNo.AddString(_T("11"));
	m_cbxPlcNo.AddString(_T("12"));
	m_cbxPlcNo.AddString(_T("13"));
	m_cbxPlcNo.AddString(_T("14"));
	m_cbxPlcNo.AddString(_T("15"));
	m_cbxPlcNo.AddString(_T("16"));
	m_cbxPlcNo.AddString(_T("17"));
	m_cbxPlcNo.AddString(_T("18"));
	m_cbxPlcNo.AddString(_T("19"));
	m_cbxPlcNo.AddString(_T("20"));
	m_cbxPlcNo.SetCurSel(0);

	m_cbxDevice.AddString(_T("D"));
	m_cbxDevice.SetCurSel(0);

	m_cbxCvKind.AddString(_T("0:일반  -N"));
	m_cbxCvKind.AddString(_T("1:디버터-D"));
	m_cbxPlcNo.SetCurSel(0);

	CImageList imageList;
	CListCtrl* pListCtrl1 = (CListCtrl*)GetDlgItem(IDC_LIST_REG_DATA);

	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/, 1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BOARD));
	pListCtrl1->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl1->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt = LVCFMT_LEFT;
		int cx[] = { 50, 50, 50, 50 };
		LPCTSTR lpszCol[] = { _T("PlcNO"), _T("RegNO"), _T("DEC"), _T("HEX") };
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i = 0; i < nCol; ++i)
			pListCtrl1->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	pDoc->m_oControlDlg.Create(IDD_ECS_CONTROL);
	pDoc->m_oControlDlg.InitialRefresh();
}

#define		IS_BIT_ON(xArg, yArg)	(((WORD)xArg & yArg) == yArg)
#define		IS_BIT_OFF(xArg, yArg)	(((WORD)xArg & yArg) != yArg)

#define		IS_BIT_KEY_CURR(xArg, bOn)						IsBitOnOffByKeyWord(m_nNumber - 1, nDevNum, xArg, bOn)
#define		IS_BIT_KEY_EXTRA(num, dev, xArg, bOn, bNext)	IsBitOnOffByKeyWord(num, dev, xArg, bOn, bNext)

#define		SET_REG_INFO(name, data, option)					pDoc->SetAddrByName(m_nNumber - 1, nDevNum, name, data, option)
#define		GET_REG_INFO(name)									pDoc->GetAddrByName(m_nNumber - 1, nDevNum, name)

#define		GET_REG_INFO_EXTRA(num, dev, name)					pDoc->GetAddrByName(num, dev, name)
#define		SET_REG_INFO_EXTRA(num, dev, name, data, option)	pDoc->SetAddrByName(num, dev, name, data, option)

BOOL CEcsView::IsBitOnOffByKeyWord(int m_nNumber, int nDevNum, CString strKeyWord, BOOL bOn, BOOL bNext = TRUE)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nParent;

	int nTrackNo = int(nDevNum / pDoc->m_nWordCnt) + ((m_nNumber + 1) * 100);
	//if (m_nNumber == 0)						// 하드코딩
	//	nTrackNo = nDevNum / 2;

	CTrackProperty* pTrackProperty = NULL;
	if ((pTrackProperty = pDoc->GetTrackPropertyByKeyword(m_nNumber, nTrackNo, strKeyWord)) == NULL)
		return 0;

	if (pTrackProperty->m_strType != "b")
		return FALSE;

	CString strType = pTrackProperty->m_strType;
	CString strAddr = pTrackProperty->m_strAddr;
	CString strName = pTrackProperty->m_strName;

	//WORD wParent = 0x0000;
	int nRealDevNum;
	int nWordOrder = _ttoi(strAddr.Right(1));
	if (pTrackProperty->m_bSeparatelyETC == TRUE)
	{
		nRealDevNum = _ttoi(strAddr);
		nParent = pDoc->m_arrRegData[m_nNumber].GetWord(nRealDevNum);
	}
	else
	{
		nRealDevNum = nDevNum + nWordOrder;
		nParent = pDoc->m_arrRegData[m_nNumber].GetWord(nRealDevNum);
	}

	WORD wData = 0x0000;
	switch (pTrackProperty->m_nInOrder)
	{
	case  1:		wData = enBit11;		break;
	case  2:		wData = enBit12;		break;
	case  3:		wData = enBit13;		break;
	case  4:		wData = enBit14;		break;
	case  5:		wData = enBit15;		break;
	case  6:		wData = enBit16;		break;
	case  7:		wData = enBit17;		break;
	case  8:		wData = enBit18;		break;
	case  9:		wData = enBit21;		break;
	case 10:		wData = enBit22;		break;
	case 11:		wData = enBit23;		break;
	case 12:		wData = enBit24;		break;
	case 13:		wData = enBit25;		break;
	case 14:		wData = enBit26;		break;
	case 15:		wData = enBit27;		break;
	case 16:		wData = enBit28;		break;
	}

	if (bOn == TRUE)
		return IS_BIT_ON(nParent, wData);
	else
		return IS_BIT_OFF(nParent, wData);
}

void CEcsView::InvokeArvStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 2;

	int nSimMode = pDoc->m_arrRegData[m_nNumber - 1][1];			// 시뮬레이터 모드일때

	int nDevNum = (pTrack->m_nNumber-pCv->m_nStTrNum+1)* pDoc->m_nWordCnt;

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

	if (pTrack->m_nNumber == 103)
		int aaaaaaaaaa = 0;

	#pragma region 도착대의 경우 처리
	//if (nSimMode == 1)
	//{
	//	#pragma region 시뮬레이션 모드일때 - 주석처리
	//	//// 화물감지후 5초가 지나면 화물 들어내기(지게차가 들어냈음)
	//	//if ((nDestNo == pTrack->m_nNumber) && ((nSensor & enBit11) == enBit11))
	//	//{
	//	//	CTimeSpan tElapseTime = CTime::GetCurrentTime() - pTrack->m_tTime;
	//	//	if (tElapseTime.GetTotalSeconds() > 5)
	//	//	{
	//	//		// 5초뒤에 화물감지 없애기 
	//	//		pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] ^= enBit11;					
	//	//	}
	//	//}
	//	#pragma endregion
	//}
	//else 
	//{
		// 작업번호 있고, 목적지가 900 이상이며, 자동이며, 화물감지가 ON 일때 (nSensor,
//		if ((nLuggNo != 0) && (nDestNo > 900) && ((nStatus & enBit11) == enBit11) && ((nSensor & enBit11) == enBit11)  )
		//if ((nLuggNo != 0) && (nDestNo > 900) && (IS_BIT_ON(nStatus, enBit11)) && (IS_BIT_ON(nSensor, enBit11)))
	if ((nLuggNo != 0) && (nDestNo > 900) && (IS_BIT_KEY_CURR(_T("Auto"), TRUE)) && (IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE)))
	{
		// 출고대 ON
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] |=  enBit13;
		//pDoc->m_arrRegData[m_nNumber - 1].SetBit(nDevNum + 7, enBit13, TRUE);
		//SET_REG_INFO(_T("StatusData"), (nStatus | enBit13), 0);		// ON
		SET_REG_INFO(_T("RetStation"), nStatus, 1);					// ON
	}
	// 작업번호 없고, 목적지가 0이며, 수동이며, 화물감지가 OFF 이고, 출고대가 ON일때 
//		if ((nLuggNo == 0) && (nDestNo == 0) &&	((nStatus & enBit11) != enBit11) &&	((nSensor & enBit11) != enBit11) && ((nStatus & enBit13) == enBit13))
		//if ((nLuggNo == 0) && (nDestNo == 0) && (IS_BIT_OFF(nStatus, enBit11)) && (IS_BIT_OFF(nSensor, enBit11)) && (IS_BIT_ON(nStatus, enBit13)))
	if ((nLuggNo == 0) && (nDestNo == 0) && (IS_BIT_KEY_CURR(_T("Auto"), FALSE)) && (IS_BIT_KEY_CURR(_T("ProductSensor"), FALSE)) && (IS_BIT_KEY_CURR(_T("RetStation"), TRUE)))
	{
		// 출고대를 토글해준다.
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^= enBit13;
		//pDoc->m_arrRegData[m_nNumber - 1].InverseBit(nDevNum + 7, enBit13);
		//SET_REG_INFO(_T("StatusData"), (nStatus ^ enBit13), 0);	// 반전
		SET_REG_INFO(_T("RetStation"), nStatus, 3);				// 반전
	}
	// 작업번호 있고, 목적지가 자기자신이며, 자동이며, 화물감지가 ON 일때 
//		if ((nLuggNo != 0) && (nDestNo == pTrack->m_nNumber) &&	((nStatus & enBit11) == enBit11) && ((nSensor & enBit11) == enBit11) )
		//if ((nLuggNo != 0) && (nDestNo == pTrack->m_nNumber) && (IS_BIT_ON(nStatus, enBit11)) && (IS_BIT_ON(nSensor, enBit11)))
	if ((nLuggNo != 0) && (nDestNo == pTrack->m_nNumber) && (IS_BIT_KEY_CURR(_T("Auto"), TRUE)) && (IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE)))
	{
		// 출고대 ON 
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] |=  enBit13;
		//pDoc->m_arrRegData[m_nNumber - 1].SetBit(nDevNum + 7, enBit13, TRUE);
		//SET_REG_INFO(_T("StatusData"), (nStatus | enBit13), 0);		// ON
		SET_REG_INFO(_T("RetStation"), nStatus, 1);					// ON
	}

#pragma region 작업번호가9999일때하는작업들_주석

	////------------------------------------------작업번호가 9999일때 화물감지 2단 on -------------------------------------------------
	//if (pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 9999)
	//{
	//	//도착지 = 9999
	//	//op 자동
	//	if (pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 9999 &&
	//	   (pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) == enBit11)
	//	{
	//		pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] |= enBit11;
	//		pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] |= enBit12;
	//	}
	//}
	//	//도착지 != 9999
	//	//op 수동
	//if ( pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] != 9999 ||
	//	(pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) != enBit11)
	//{
	//	if (((pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11) &&
	//		 (pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == enBit11)
	//	{
	//		pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] ^= enBit11;
	//		pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] ^= enBit12;
	//	}
	//}
	////------------------------------------------------------------------------------------------------------------
#pragma endregion
//}
#pragma endregion
}

void CEcsView::InvokeRetStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CStationInfo* pTempStation = pTrack->GetStoStation();
	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 2;

	int nSimMode = pDoc->m_arrRegData[m_nNumber - 1][1];			// 시뮬레이터 모드일때

	int nDevNum = (pTrack->m_nNumber-pCv->m_nStTrNum+1)* pDoc->m_nWordCnt;

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

	if (pTrack->m_nNumber == 15)
		int aaaaaaaaa = 0;

#pragma region 출고대의 경우 처리
	// 목적지가 자기자신의 트랙번호와 같으면
	if (nSimMode == 1)
	{
#pragma region 시뮬레이션 모드일때 (주석처리)
		/*
		// 화물 감지 후 5초가 지나면 화물 들어내기(지게차가 들어냈음)
		if ((nDestNo == pTrack->m_nNumber) &&
		//	((nSensor & enBit11) == enBit11) &&
		//	((nStatus & enBit13) == enBit13) &&
			(IS_BIT_ON(nSensor, enBit11)) &&
			(IS_BIT_ON(nStatus, enBit13)) &&
			(pTempStation != NULL))						// 출고대 이면서 입고대인 경우
		{
			CTimeSpan tElapseTime = CTime::GetCurrentTime() - pTrack->m_tTime;
			if (tElapseTime.GetTotalSeconds() > 5)
			{
				//int aaaaa = pDoc->m_arrRegData[m_nNumber-1][nDevNum+7];

				// 5초뒤에 DATA 없애기
				//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = (enBit11 | enBit12);

				//pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 0;
				//pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 0;
				//pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] = 0;
				//pDoc->m_arrRegData[m_nNumber-1][nDevNum+3] = 0;
				//pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] = 0;

				pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 7, (enBit11 | enBit12));

				pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 0, 0x0000);
				pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 1, 0x0000);
				pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 2, 0x0000);
				pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 3, 0x0000);
				pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 4, 0x0000);
			}
		}
		//*/
#pragma endregion
	}
	//	else 
	//	{
#pragma region 시뮬레이션 모드 아닐때 
//	if ((nDestNo == pTrack->m_nNumber) && ((nStatus & enBit13) != enBit13))
	//if ((nDestNo == pTrack->m_nNumber) && (IS_BIT_OFF(nStatus, enBit13)))
	if ((nDestNo == pTrack->m_nNumber) && (IS_BIT_KEY_CURR(_T("RetStation"), FALSE)))
	{
		// 출고대 신호 ON
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = nStatus | enBit13;
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = nSensor | enBit11;
		// 
		//SET_REG_INFO(_T("StatusData"), (nStatus | enBit13), 0);		// ON
		//SET_REG_INFO(_T("SensorData"), (nSensor | enBit11), 0);		// ON

		SET_REG_INFO(_T("RetStation"), nStatus, 1);					// ON
		SET_REG_INFO(_T("ProductSensor"), nSensor, 1);				// ON

		pTrack->m_bStartFlag = TRUE;
		pTrack->m_tTime = COleDateTime::GetCurrentTime();
	}
	//else if ((nDestNo != pTrack->m_nNumber) && (IS_BIT_ON(nStatus, enBit13)))
	else if ((nDestNo != pTrack->m_nNumber) && (IS_BIT_KEY_CURR(_T("RetStation"), TRUE)))
	{
		// 출고대 신호 OFF

		SET_REG_INFO(_T("RetStation"), nStatus, 3);					// 반전

		//pTrack->m_bStartFlag = TRUE;
		//pTrack->m_tTime = CTime::GetCurrentTime();
	}

	// 작업번호와 목적지가 0이고, 화물감지가 ON 이며, 출고대가 ON인 경우 - SPL에 쓰던건데 무슨 경우인지 모르겠음!
//	else if ((nDestNo == 0) && (nLuggNo == 0) && ((nSensor & enBit11) == enBit11) && ((nStatus & enBit13) == enBit13) )
	//else if ((nDestNo == 0) && (nLuggNo == 0) && (IS_BIT_ON(nSensor, enBit11)) && (IS_BIT_ON(nStatus, enBit13)))
	//{
	//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 7, nStatus & enBit11);
	//	//SET_REG_INFO(_T("StatusData"), (nStatus & enBit11), 0);		// 특정비트만 남기고 나머지는 다 OFF
	//	SET_REG_INFO(_T("Auto"), nStatus, 2);							// 특정비트만 남기고 나머지는 다 OFF
	//
	//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 8, 0x0000);
	//	SET_REG_INFO(_T("SensorData"), 0x0000, 0);						// WORD RESET
	//
	//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 0, 0x0000);
	//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 1, 0x0000);
	//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 2, 0x0000);
	//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 3, 0x0000);
	//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 4, 0x0000);
	//	SET_REG_INFO(_T("LuggNum"), 0x0000, 0);						// WORD RESET
	//	SET_REG_INFO(_T("DestPos"), 0x0000, 0);						// WORD RESET
	//	SET_REG_INFO(_T("D10n+2"),  0x0000, 0);						// WORD RESET
	//	SET_REG_INFO(_T("D10n+3"),  0x0000, 0);						// WORD RESET
	//	SET_REG_INFO(_T("D10n+4"),  0x0000, 0);						// WORD RESET
	//
	//}
	//*/

	// 작업번호가 0이 아니고, 목적지가 자기자신이며, 화물감지가 OFF 인경우 
//	else if ((nDestNo == pTrack->m_nNumber) && (nLuggNo != 0) && ((nSensor & enBit11) != enBit11)  )
	//else if ((nDestNo == pTrack->m_nNumber) && (nLuggNo != 0) && (IS_BIT_OFF(nSensor, enBit11)))
	else if ((nDestNo == pTrack->m_nNumber) && (nLuggNo != 0) && (IS_BIT_KEY_CURR(_T("ProductSensor"), FALSE)))
	{
		// TRACK DATA 삭제
		//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 7, nStatus & enBit11);
		//SET_REG_INFO(_T("StatusData"), (nStatus & enBit11), 0);		// 특정비트만 남기고 나머지는 다 OFF
		SET_REG_INFO(_T("Auto"), nStatus, 2);							// 특정비트만 남기고 나머지는 다 OFF

		//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 0, 0x0000);
		//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 1, 0x0000);
		//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 2, 0x0000);
		//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 3, 0x0000);
		//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 4, 0x0000);

		SET_REG_INFO(_T("LuggNum"), 0x0000, 0);						// WORD RESET
		SET_REG_INFO(_T("DestPos"), 0x0000, 0);						// WORD RESET
		SET_REG_INFO(_T("D10n+2"), 0x0000, 0);						// WORD RESET
		SET_REG_INFO(_T("D10n+3"), 0x0000, 0);						// WORD RESET
		SET_REG_INFO(_T("D10n+4"), 0x0000, 0);						// WORD RESET
	}
#pragma endregion
	//	}
#pragma endregion
}

void CEcsView::InvokeStoStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CStationInfo* pTempStation = pTrack->GetArvStation();
	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 2;

	int nSimMode = pDoc->m_arrRegData[m_nNumber - 1][1];			// 시뮬레이터 모드일때

	int nDevNum = (pTrack->m_nNumber-pCv->m_nStTrNum+1)* pDoc->m_nWordCnt;

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

#pragma region 입고대의 경우 처리
	// 작업번호가 없고, 입고대가 ON이 아닐경우 
	//if ((nLuggNo == 0) && ((nStatus & enBit12) != enBit12))
	//if ((nLuggNo == 0) && (IS_BIT_OFF(nStatus, enBit12)))
	if ((nLuggNo == 0) && (IS_BIT_KEY_CURR(_T("StoStation"), FALSE)))
	{
		if (nSimMode != 1)
		{
			if (pTrack->m_nNumber == 103)
				int aaaaaaaaaa = 0;

			// 입고대 신호 ON
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = nStatus | enBit12;
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = nSensor | enBit11;
			// 
			//pDoc->m_arrRegData[m_nNumber - 1].SetBit(nDevNum + 7, enBit12, TRUE);
			//pDoc->m_arrRegData[m_nNumber - 1].SetBit(nDevNum + 8, enBit11, TRUE);

			//SET_REG_INFO(_T("StatusData"), (nStatus | enBit12), 0);		// ON
			//SET_REG_INFO(_T("SensorData"), (nSensor | enBit11), 0);		// ON

			SET_REG_INFO(_T("StoStation"), nStatus, 1);					// ON
			SET_REG_INFO(_T("ProductSensor"), nSensor, 1);				// ON
		}
		//else
		//{
		//	CTimeSpan tElapseTime = CTime::GetCurrentTime() - pTrack->m_tTime;
		//	if (tElapseTime.GetTotalSeconds() > 5)
		//	{
		//		// 5초뒤에 입고대 신호 ON
		//		pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = nStatus | enBit12;
		//		pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = nSensor | enBit11;
		//	}		
		//}
	}
	// 작업 번호가 있고, 입고대가 ON 일 경우				
//	else if ((nLuggNo != 0) && ((nStatus & enBit12) == enBit12))
	//else if ((nLuggNo != 0) && (IS_BIT_ON(nStatus, enBit12)))
	else if ((nLuggNo != 0) && (IS_BIT_KEY_CURR(_T("StoStation"), TRUE)))
	{
		// 입고대 신호 OFF
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = nStatus ^ enBit12;
		//pDoc->m_arrRegData[m_nNumber - 1].InverseBit(nDevNum + 7, enBit12);
		//SET_REG_INFO(_T("StatusData"), (nStatus ^ enBit12), 0);	// 반전
		SET_REG_INFO(_T("StoStation"), nStatus, 3);				// 반전
	}
	// 입고대 이면서 출고대인경우
	else if (pStation->m_enKind == CStationInfo::enRetStation)
	{
		// 목적지가 자기자신의 트랙번호와 같으면
		if (nDestNo == pTrack->m_nNumber)
		{
			// 출고대 신호 ON
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = nStatus | enBit13;
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = nSensor | enBit11;
			// 
			//pDoc->m_arrRegData[m_nNumber - 1].SetBit(nDevNum + 7, enBit13, TRUE);
			//pDoc->m_arrRegData[m_nNumber - 1].SetBit(nDevNum + 8, enBit11, TRUE);

			//SET_REG_INFO(_T("StatusData"), (nStatus | enBit12), 0);		// ON
			//SET_REG_INFO(_T("SensorData"), (nSensor | enBit11), 0);		// ON

			SET_REG_INFO(_T("StoStation"), nStatus, 1);					// ON
			SET_REG_INFO(_T("ProductSensor"), nSensor, 1);				// ON
			pTrack->m_bStartFlag = TRUE;
			pTrack->m_tTime = COleDateTime::GetCurrentTime();
		}
		// 화물감지후 5초가 지나면 화물 들어내기(지게차가 들어냈음)
		else if (nDestNo == pTrack->m_nNumber && pTrack->m_bStartFlag == TRUE)
		{
			COleDateTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;
			if (tElapseTime.GetTotalSeconds() > 5)
			{
				// 5초뒤에 화물감지 없애기 
				//pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = 0;	
				//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum+8, 0x0000);
				SET_REG_INFO(_T("SensorData"), 0x0000, 0);					// WORD RESET
			}
		}
		// 작업번호와 목적지가 0이고, 화물감지가 ON 이며, 출고대가 ON인 경우 - SPL에 쓰던건데 무슨 경우인지 모르겠음!
//		else if ((nDestNo == 0) && (nLuggNo == 0) && ((nSensor & enBit11) == enBit11) && ((nStatus & enBit13) == enBit13) )
		//else if ((nDestNo == 0) && (nLuggNo == 0) && (IS_BIT_ON(nSensor, enBit11)) && (IS_BIT_ON(nStatus, enBit13)))
		//{
		//	//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] &= enBit11;
		//
		//	//pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = 0;
		//
		//	//pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 0;
		//	//pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 0;
		//	//pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] = 0;
		//	//pDoc->m_arrRegData[m_nNumber-1][nDevNum+3] = 0;
		//	//pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] = 0;	
		//
		//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 7, nStatus & enBit11);
		//	//SET_REG_INFO(nDevNum, _T("StatusData"), (nStatus & enBit11), 0);		// 특정비트만 남기고 나머지는 다 OFF
		//	SET_REG_INFO(_T("Auto"), nStatus, 2);							// 특정비트만 남기고 나머지는 다 OFF
		//
		//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 8, 0x0000);
		//	SET_REG_INFO(_T("SensorData"), 0x0000, 0);						// WORD RESET
		//
		//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 0, 0x0000);
		//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 1, 0x0000);
		//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 2, 0x0000);
		//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 3, 0x0000);
		//	//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 4, 0x0000);
		//	SET_REG_INFO(_T("LuggNum"), 0x0000, 0);						// WORD RESET
		//	SET_REG_INFO(_T("DestPos"), 0x0000, 0);						// WORD RESET
		//	SET_REG_INFO(_T("D10n+2"),  0x0000, 0);						// WORD RESET	
		//	SET_REG_INFO(_T("D10n+3"),  0x0000, 0);						// WORD RESET	
		//	SET_REG_INFO(_T("D10n+4"),  0x0000, 0);						// WORD RESET	
		//}

		// 작업번호가 0이 아니고, 목적지가 자기자신이며, 화물감지가 OFF 인경우 
		else if ((nDestNo == pTrack->m_nNumber) && (nLuggNo != 0) && ((nSensor & enBit11) != enBit11))
		{
			// TRACK DATA 삭제 
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] &= enBit11;

			//pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 0;
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 0;
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] = 0;
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+3] = 0;
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] = 0;	

			//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 7, nStatus & enBit11);
			//SET_REG_INFO(_T("StatusData"), (nStatus& enBit11), 0);		// 특정비트만 남기고 나머지는 다 OFF
			SET_REG_INFO(_T("Auto"), nStatus, 2);							// 특정비트만 남기고 나머지는 다 OFF

			//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 0, 0x0000);
			//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 1, 0x0000);
			//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 2, 0x0000);
			//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 3, 0x0000);
			//pDoc->m_arrRegData[m_nNumber - 1].SetWord(nDevNum + 4, 0x0000);
			SET_REG_INFO(_T("LuggNum"), 0x0000, 0);						// WORD RESET
			SET_REG_INFO(_T("DestPos"), 0x0000, 0);						// WORD RESET
			SET_REG_INFO(_T("D10n+2"), 0x0000, 0);						// WORD RESET	
			SET_REG_INFO(_T("D10n+3"), 0x0000, 0);						// WORD RESET	
			SET_REG_INFO(_T("D10n+4"), 0x0000, 0);						// WORD RESET	
		}
	}
#pragma endregion
}

void CEcsView::InvokeScStoHs(CCv* pCv, CTrackInfo* pTrack)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CStationInfo* pTempStation = pTrack->GetArvStation();
	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 2;

	int nSimMode = pDoc->m_arrRegData[m_nNumber - 1][1];			// 시뮬레이터 모드일때

	int nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * pDoc->m_nWordCnt;

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

#pragma region 입고HS 의 경우 처리
	if ((nLuggNo != 0) &&
		(nDestNo > 900) &&
		//(nStatus & enBit11) == enBit11 &&
		//(nSensor & enBit11) == enBit11)
		//(IS_BIT_ON(nStatus, enBit11)) &&
		//(IS_BIT_ON(nSensor, enBit11)))
		(IS_BIT_KEY_CURR(_T("Auto"), TRUE)) &&
		(IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE)))
	{
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = nStatus | enBit14;
		// 
		//pDoc->m_arrRegData[m_nNumber - 1].SetBit(nDevNum + 7, enBit14, TRUE);

		//SET_REG_INFO(_T("StatusData"), (nStatus | enBit14), 0);		// ON
		SET_REG_INFO(_T("ScStoHS"), nStatus, 1);						// ON
	}

	if ((nLuggNo == 0) ||
		(nDestNo < 900) ||
		//(nStatus & enBit11) != enBit11 ||
		//(nSensor & enBit11) != enBit11)
		//(IS_BIT_OFF(nStatus, enBit11)) ||
		//(IS_BIT_OFF(nSensor, enBit11)))
		(IS_BIT_KEY_CURR(_T("Auto"), FALSE)) ||
		(IS_BIT_KEY_CURR(_T("ProductSensor"), FALSE)))
	{
		//if ((nStatus & enBit14) == enBit14)
		//if (IS_BIT_ON(nStatus, enBit14))
		if (IS_BIT_KEY_CURR(_T("ScStoHS"), TRUE))
		{
			//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = nStatus ^ enBit14;
			// 
			//pDoc->m_arrRegData[m_nNumber - 1].InverseBit(nDevNum + 7, enBit14);

			//SET_REG_INFO(_T("StatusData"), (nStatus ^ enBit14), 0);	// 반전
			SET_REG_INFO(_T("ScStoHS"), nStatus, 3);					// 반전
		}
	}
	//입고처리시 : 화물감지 ON, 입고 H/S ON 일때 처리
	// 출고대, 화물감지 off
	if ((nLuggNo == 0) &&
		(nDestNo == 0) &&
		//(nStatus & enBit13) == enBit13 &&						
		//(nStatus & enBit11) == enBit11 &&
		//(nSensor & enBit11) == enBit11)
		//(IS_BIT_ON(nStatus, enBit13)) &&
		//(IS_BIT_ON(nStatus, enBit11)) &&
		//(IS_BIT_ON(nSensor, enBit11)))
		(IS_BIT_KEY_CURR(_T("RetStation"), TRUE)) &&
		(IS_BIT_KEY_CURR(_T("Auto"), TRUE)) &&
		(IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE)))
	{
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = nStatus ^ enBit13;
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = nSensor ^ enBit11;
		// 
		//pDoc->m_arrRegData[m_nNumber - 1].InverseBit(nDevNum + 7, enBit13);
		//pDoc->m_arrRegData[m_nNumber - 1].InverseBit(nDevNum + 8, enBit11);

		//SET_REG_INFO(_T("StatusData"), (nStatus ^ enBit13), 0);	// 반전
		//SET_REG_INFO(_T("SensorData"), (nSensor ^ enBit11), 0);	// 반전

		SET_REG_INFO(_T("RetStation"), nStatus, 3);				// 반전
		SET_REG_INFO(_T("ProductSensor"), nSensor, 3);			// 반전
	}

	if ((nLuggNo == 0) &&
		(nDestNo == 0) &&
		//(nStatus & enBit14) == enBit14 &&
		//(nSensor & enBit11) == enBit11)
		//(IS_BIT_ON(nStatus, enBit14)) &&
		//(IS_BIT_ON(nSensor, enBit11)))
		(IS_BIT_KEY_CURR(_T("ScStoHS"), TRUE)) &&
		(IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE)))
	{
		//pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = nSensor ^ enBit11;
		// 
		//pDoc->m_arrRegData[m_nNumber - 1].InverseBit(nDevNum + 8, enBit11);

		//SET_REG_INFO(_T("SensorData"), (nSensor ^ enBit11), 0);	// 반전
		SET_REG_INFO(_T("ProductSensor"), nSensor, 3);			// 반전
	}
#pragma endregion
}

void CEcsView::InvokeScRetHs(CCv* pCv, CTrackInfo* pTrack)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CStationInfo* pTempStation = pTrack->GetArvStation();
	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 2;

	int nSimMode = pDoc->m_arrRegData[m_nNumber - 1][1];			// 시뮬레이터 모드일때

	int nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * pDoc->m_nWordCnt;

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

	int nResult = -1;
#pragma region 출고HS 의 경우 처리
	if ((nLuggNo == 0) &&
		(IS_BIT_KEY_CURR(_T("Auto"), TRUE)) &&
		(IS_BIT_KEY_CURR(_T("ScRetHS"), FALSE)) &&
		(IS_BIT_KEY_CURR(_T("ProductSensor"), FALSE)))
	{
		SET_REG_INFO(_T("ScRetHS"), nStatus, 1);									// ON
	}

	if ((nLuggNo == 0) &&
		(IS_BIT_KEY_CURR(_T("ScRetHS"), TRUE)) &&
		(IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE)))
	{
		SET_REG_INFO(_T("ScRetHS"), nStatus, 3);									// 반전
	}

	if ((nLuggNo != 0) ||
		(IS_BIT_KEY_CURR(_T("Auto"), FALSE)))
	{
		if (IS_BIT_KEY_CURR(_T("ScRetHS"), TRUE))
		{
			SET_REG_INFO(_T("ScRetHS"), nStatus, 3);								// 반전
		}
	}
#pragma endregion
}

BOOL CEcsView::InvokeLogic(CCv* pCv, CTrackInfo* pTrack, CLogicArray* pLogicArray)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	if (pTrack == NULL
		|| pLogicArray == NULL
		)
		return FALSE;

	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 5;

	int nSimMode = pDoc->m_arrRegData[m_nNumber - 1][1];			// 시뮬레이터 모드일때

	int nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * pDoc->m_nWordCnt;

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

	if (m_nNumber == 1)
		int nStoStation = GET_REG_INFO(_T("StoStation"));

	int nLogicSize = pLogicArray->GetSize();
	CString strName = pLogicArray->m_strName;

	CLogic* pLogic = NULL;
	for (int l = 0; l < nLogicSize; l++)
	{
		pLogic = &pLogicArray->GetAt(l);

		if (pLogic == NULL)
			continue;

		BOOL bTemp = FALSE;
		BOOL bConditionResult = FALSE;
		BOOL bActionResult = FALSE;
		CCondition oCondition;
		CAction oAction;

		int nId = pLogic->m_nId;
		CString strLogicDesc = pLogic->m_strDesc;
		CString strDongJak = strLogicDesc;

		pLogic->m_arrConditions.GetSize();

		int nConditionCnt = pLogic->m_arrConditions.GetSize();
		int nActionCnt = pLogic->m_arrActions.GetSize();

		CString strConditionDesc = pLogic->m_arrConditions.m_strDesc;
		CString strActionDesc = pLogic->m_arrActions.m_strDesc;

		CString strParent = pDoc->GetParentWord(strName);
		int nParent = GET_REG_INFO(strParent);

		for (int j = 0; j < nConditionCnt; j++)
		{
			bConditionResult = TRUE;
			oCondition = pLogic->m_arrConditions.GetAt(j);

			CString strKeyWord = oCondition.m_strKeyWord;
			int nGetData = GET_REG_INFO(strKeyWord);

			// BT_SIGN_TIME_ELAPSE의 경우를 위해서 
			COleDateTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;

			if (nId == 1 && strName == _T("ScRetHS") && strKeyWord == _T("DestPos") && pTrack->m_nNumber == 12)
			//if (pTrack->m_nNumber == 11)
				int a = 0;

			int nSec = _ttoi(strKeyWord);
			int nTotalSec = tElapseTime.GetTotalSeconds();
			BOOL bIsDestSelf = pDoc->IsDestination(strKeyWord, nGetData, pTrack->m_nNumber, 1);
			BOOL bIsScDest = pDoc->IsDestination(strKeyWord, nGetData, 0, 2);
			BOOL bIsStation = pDoc->IsStationKind(pTrack, strKeyWord);
			BOOL bTimeElapse = (pTrack->m_bStartFlag) && (nTotalSec > nSec);

			// Condition Check
			switch (oCondition.m_enType)
			{
			case BT_SIGN_BIT_OFF:			bTemp = (nGetData == FALSE);	break;		// OFF
			case BT_SIGN_BIT_ON:			bTemp = (nGetData == TRUE);		break;		// ON
			case BT_SIGN_NOT_ZERO:			bTemp = (nGetData != 0);		break;		// NOT 0
			case BT_SIGN_IS_ZERO:			bTemp = (nGetData == 0);		break;		// IS 0
			//case BT_SIGN_NO:				bTemp = (nGetData );			break;		// NO 
			//case BT_SIGN_EXIST:			bTemp = (nGetData );			break;		// EXIST 
			case BT_SIGN_IS_STATION:		bTemp = bIsStation;				break;		// IsStation 
			//case BT_SIGN_LESS:			bTemp = (nGetData );			break;		// LESS <
			//case BT_SIGN_GREATER:			bTemp = (nGetData );			break;		// GREATER > 
			//case BT_SIGN_LTOE:			bTemp = (nGetData );			break;		// LTOE <=
			//case BT_SIGN_GTOE:			bTemp = (nGetData );			break;		// GTOE >=
			case BT_SIGN_EQUAL_SELF:		bTemp = bIsDestSelf;			break;		// EQUAL SELF
			case BT_SIGN_NOT_EQUAL_SELF:	bTemp = !bIsDestSelf;			break;		// NOT EQUAL SELF
			case BT_SIGN_IS_SC_DEST:		bTemp = bIsScDest;				break;		// IS SC DEST NUM
			//case BT_SIGN_EXCEPT:			bTemp = (nGetData );			break;		// EXCEPT
			case BT_SIGN_TIME_ELAPSE:		bTemp = bTimeElapse;			break;		// TIME ELAPSE(5초)
			case BT_SIGN_NOT_SC_DEST:		bTemp = !bIsScDest;				break;		// NOT SC DEST NUM
			default:						bTemp = FALSE;					break;		// 정의되지 않으면 무조건 FALSE
			}
			bConditionResult = bConditionResult & bTemp;

			if (bConditionResult == FALSE)
				break;
		}

		BOOL bActionRunOK = FALSE;
		if (bConditionResult == TRUE)
		{
			nStatus = GET_REG_INFO(_T("StatusData"));
			for (int k = 0; k < nActionCnt; k++)
			{
				oAction = pLogic->m_arrActions.GetAt(k);

				BYTE bTemp = 0x0A;

				BYTE bTempTemp = ~bTemp;

				nStatus = GET_REG_INFO(_T("StatusData"));

				COleDateTime time = COleDateTime::GetCurrentTime();

				int nGetData = GET_REG_INFO(oAction.m_strKeyWord);

				strParent = pDoc->GetParentWord(oAction.m_strKeyWord);

				nParent = GET_REG_INFO(strParent);

				int nDeviceNo = 0;
				if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
				{
					// 신호가 분리되어 있는 경우에 Parent 값을 가져오는 방법 이같이 사용해야할것
					strParent = oAction.m_strKeyWord + _T("s");
					nParent = pDoc->GetAddrByName(m_nNumber - 1, nDevNum, strParent, TRUE);	// s가 붙었음!
				}

				if (strName == _T("ScRetHS"))
				{
					if (oAction.m_nType == 52 || oAction.m_nType == 53 || oAction.m_nType == 57 || oAction.m_nType == 58)
						int a = 0;
				}

				// Action Run
				switch (oAction.m_enType)
				{
				case BT_ACTION_BIT_ON:		SET_REG_INFO(oAction.m_strKeyWord, nParent, 1);			bActionRunOK = TRUE;	 break;		//
				case BT_ACTION_BIT_OFF:		SET_REG_INFO(oAction.m_strKeyWord, nParent, 2);			bActionRunOK = TRUE;	 break;		// 특정비트만 남기고 나머지는 다 OFF
				case BT_ACTION_INVERSE:		SET_REG_INFO(oAction.m_strKeyWord, nParent, 3);			bActionRunOK = TRUE;	 break;		// 
				case BT_ACTION_SF_ON:		pTrack->m_bStartFlag = TRUE; pTrack->m_tTime = time;	bActionRunOK = TRUE;	 break;		// Start Flag
				case BT_ACTION_SF_OFF:		pTrack->m_bStartFlag = FALSE;							bActionRunOK = TRUE;	 break;		// Start Flag
				case BT_ACTION_TIME_RESET:	pTrack->m_tTime = time;									bActionRunOK = TRUE;	 break;		// 
				case BT_ACTION_WORD_RESET:	SET_REG_INFO(oAction.m_strKeyWord, 0x0000, 0);			bActionRunOK = TRUE;	 break;		// 
				case BT_ACTION_BIT_F_OFF:	SET_REG_INFO(oAction.m_strKeyWord, nParent, 4);			bActionRunOK = TRUE;	 break;		// 
				default:					continue;
				}

				nGetData = GET_REG_INFO(oAction.m_strKeyWord);

				if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
				{
					// 신호가 분리되어 있는 경우에 Parent 값을 가져오는 방법 이같이 사용해야할것
					//pDoc->m_bParent = TRUE;
					strParent = oAction.m_strKeyWord + _T("s");
					nParent = pDoc->GetAddrByName(m_nNumber - 1, nDevNum, strParent, TRUE);	// s가 붙었음!
				}


				int aaa = 0;
			}

			// 로그 남기기 
			if (bActionRunOK)
			{
				if (pTrack->m_nNumber == 104 /*&& strName != _T("ScRetHS")*/)
					int aaaaaaaaaa = 0;

				CString strLog;
				nStatus = GET_REG_INFO(_T("StatusData"));
				strLog.Format(_T("[TrackNo : %d][LogicGroup : %s] [Logic ID : %d] [Logic Desc : %s]"), pTrack->m_nNumber, strName, nId, strLogicDesc);
				pDoc->StatusDisplay(strLog, FALSE);
			}
		}
	}

	return TRUE;
}

void CEcsView::OnTimer(UINT nIDEvent)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	static int nCount = 1;

	if (pDoc->m_bReLoading == TRUE)
		return;

	CString strLog, strDevice, strKeyWordCnt, strKeyWord, strValue, strTemp;
	int j = 1, k = 1, nTemp;

	pDoc->StatusDisplay();

	for (int i = 0; i < pDoc->m_pEquipments.GetSize(); ++i)
	{
#pragma region CCv객체가져오기
		nTemp = j;
		CCv* pCv = NULL;
		for (; j <= CV_PLC_CNT; j++)
		{
			strDevice.Format(_T("CV_E%0.2d"), j);
			CCv* pCvTemp = (CCv*)pDoc->m_pEquipments.GetEquipment(strDevice);
			if (pCvTemp == NULL)
			{
				strDevice.Format(_T("CV_V%0.2d"), k);
				pCvTemp = (CCv*)pDoc->m_pEquipments.GetEquipment(strDevice);

				if (pCvTemp == NULL)
					continue;
				else
				{
					k += 1;
					pCv = pCvTemp;
					break;
				}
			}

			else
			{
				j += 1;
				pCv = pCvTemp;
				break;
			}
		}

		if (pCv == NULL)
			continue;

#pragma endregion 
#pragma region 가져온CCv객체에따라트랙별체크
		CTrackInfo* pTrack = NULL;
		CStationInfo* pStation = NULL;
		CTrackHS* pTrackHS = NULL;

		CByteArrayEx arrBuffer(0, 0);

		int nTrackCnt = pCv->m_pInfo->m_pTracks.GetSize();

		for (int j = 0; j < nTrackCnt; ++j)
		{
			pTrack = pCv->m_pInfo->m_pTracks[j];
			if (pTrack == NULL)
				continue;

			// 존재하지 않는 트랙은 할 필요가 없음!
			if (pTrack->m_nTabNum == 0)
				continue;

			int m_nNumber = pCv->m_nNumber;

			int nSimMode = (int)pDoc->m_arrRegData[m_nNumber - 1].GetWord(1);			// 시뮬레이터 모드일때

			int nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * 10;

			int nLuggNo = GET_REG_INFO(_T("LuggNum"));
			int nDestNo = GET_REG_INFO(_T("DestPos"));
			int nStatus = GET_REG_INFO(_T("StatusData"));
			int nSensor = GET_REG_INFO(_T("SensorData"));

			if (pTrack->m_nNumber == 104)
				int aaaaaaaaaa = 0;

			CString strCheckKeyWord = _T("Autos");

			pDoc->m_bParent = TRUE;
			int nTemp1 = GET_REG_INFO(strCheckKeyWord);

#pragma region Logic 확인
			// 로직 그룹 만큼 루프하기 
			int nLogicGroupCnt = pDoc->m_arrLogicGroups.GetSize();

			CLogicArray* pLogicArray = NULL;
			for (int n = 0; n < nLogicGroupCnt; n++)
			{
				pLogicArray = &pDoc->m_arrLogicGroups[n];

				if (pLogicArray == NULL)
					continue;

				CActionArray* pActions = NULL;
				CConditionArray* pConditions = NULL;

				if (pTrack->IsNotInvokeTrack())
					continue;

				InvokeLogic(pCv, pTrack, pLogicArray);
			}
#pragma endregion 
			pDoc->m_bParent = TRUE;
			int nTemp2 = GET_REG_INFO(strCheckKeyWord);

			/////////////////////////////////////////////////////////////////////////////////////////////
#pragma region 일반트랙일때하는작업
			if (pTrack->m_nKind == 0)
			{
				MoveNextTrackForKindNormal_1(pCv, pTrack, pStation);
				MoveNextTrackForKindNormal_2(pCv, pTrack, pStation);
			}
#pragma endregion 
#pragma region 디버터일때하는작업
			if (pTrack->m_nKind == 1)
			{
				MoveNextTrackForKindDiverter_1(pCv, pTrack, pStation);
				MoveNextTrackForKindDiverter_2(pCv, pTrack, pStation);
			}
#pragma endregion 
			int nWordAddr = 0;
			int nPlcNum = pTrack->m_nCvPlcNum;

			if (nDevNum == 220)
			{
				int a = 0;
			}
#pragma region 트랙정보세팅

			// 디바이스 맵에 해당 필요 정보들을 입력해놓고 그것을 키워드 별로 불러올 수 있는 방법 필요
			// 지금은 그냥 하드코딩으로 진행함!
			if (pDoc->m_pDeviceMaps[nPlcNum - 1]->m_bUseSeparatelyETC == TRUE)
			{
				nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * pDoc->m_nWordCnt;
				
				pTrack->SetLuggNum(pDoc->m_arrRegData[nPlcNum - 1][nDevNum]);
				pTrack->SetDestPos(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 1] & 0x000F);
				pTrack->SetJobType((pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 1] >> 12) & 0x000F);
				WORD wTemp = 0x0000;
				BOOL bTemp = FALSE;
				BOOL bErrEM = FALSE;
				BOOL bErrOT = FALSE;
				BOOL bErrOL = FALSE;

				CString strKeyWord;// = _T("Auto");

				int nSeparatelyETCConunt = pDoc->m_strEtcKeyWords.GetSize();
				for (int i = 0; i < nSeparatelyETCConunt; i++)
				{
					CString strKeyWord = pDoc->m_strEtcKeyWords[i];
					bTemp = GET_REG_INFO(strKeyWord);

					if (strKeyWord == _T("ProductSensor") && (pTrack->m_nNumber == 7))
						int aaaaaa = 0;

					BOOL bUpdateSignalRegResult = UpdateSignalReg(pTrack, nPlcNum - 1, nDevNum, strKeyWord, bTemp);

					if (bUpdateSignalRegResult == TRUE)
					{
						pTrack->m_bModified = TRUE;

						if (strKeyWord == _T("ProductSensor") && (bTemp == TRUE) && (pTrack->m_nNumber == 7))
							int aaaaaaaaaaaa = 0;

						if (strKeyWord == _T("ProductSensor") && (bTemp == FALSE) && (pTrack->m_nNumber == 7))
							int bbbbbbbbbbbb = 0;

						// 하드코딩
						if (strKeyWord == _T("Emergency"))
						{
							pTrack->SetErrorCode(bTemp ? 1 : pTrack->m_wErrorCode);
							bErrEM = bTemp;
						}
						if (strKeyWord == _T("OverTime"))
						{
							pTrack->SetErrorCode(bTemp ? 10 : pTrack->m_wErrorCode);
							bErrOT = bTemp;
						}
						if (strKeyWord == _T("OverLoad"))
						{
							pTrack->SetErrorCode(bTemp ? 2 : pTrack->m_wErrorCode);
							bErrOL = bTemp;
						}
					}
				}

				if (!bErrEM && !bErrOT && !bErrOL)
				{
					pTrack->SetErrorCode(0);		// 정상으로 변경 
				}
			}
			else
			{
				pTrack->SetLuggNum(pDoc->m_arrRegData[nPlcNum - 1][nDevNum]);
				pTrack->SetDestPos(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 1]);
				pTrack->SetJobType(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 2] & 0x000f);
				pTrack->SetGenCode((pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 2] >> 8) & 0x00ff);
				pTrack->SetSize(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 3] & 0x00ff);
				pTrack->SetTurn((pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 3] >> 8) & 0x00ff);
				pTrack->SetSuspend(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 5]);
				pTrack->SetDeadLock(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 5]);
				pTrack->SetErrorCode(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 6]);
				pTrack->SetStatusData(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 7]);
				pTrack->SetSensorData(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 8]);
				pTrack->SetMotorData(pDoc->m_arrRegData[nPlcNum - 1][nDevNum + 9]);
			}

			pTrack->InvokeControl(pTrack->m_pTrackCtrl);
#pragma endregion 
		}
#pragma endregion 
	}

	//----------------------------------------------------------
	if (++nCount % 600 == 0)
	{
		nCount = 0;
	}

	CFormView::OnTimer(nIDEvent);
}

BOOL CEcsView::UpdateSignalReg(CTrackInfo* pTrack, int nPlcNo, int nDeviceNo, CString strKeyWord, BOOL bOn)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nSelIndex = pDoc->GetSignalIndex(strKeyWord);

	if (nSelIndex == -1)
		return FALSE;

	int nTemp = pTrack->m_sSeparatelyETC[nSelIndex];

	pTrack->m_sSeparatelyETC[nSelIndex] = bOn;

	pDoc->m_bParent = TRUE;
	int nParent = pDoc->GetAddrByName(nPlcNo, nDeviceNo, strKeyWord + _T("s"));
	pDoc->SetAddrByName(nPlcNo, nDeviceNo, strKeyWord, nParent, bOn ? 1 : 4);

	return TRUE;
}

void CEcsView::OnDestroy()
{
	CFormView::OnDestroy();

	CEcsDoc* pDoc = GetDocument();

	CEquipment* pEquipment = NULL;
	for (int i = 0; i < pDoc->m_pEquipments.GetSize(); ++i)
	{
		pEquipment = pDoc->m_pEquipments[i];
		if (pEquipment == NULL)
			continue;

		DEBUGER_ASSERT_VALID(pEquipment != NULL);
		DEBUGER_ASSERT_VALID(pEquipment->GetInfo() != NULL);

		if (pEquipment->m_enKind == CEquipment::enCV)
		{
			pEquipment->KillThread();
		}
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
		int nTemp = m_tabLayout.GetCurSel();
		if (nTemp < 0)
			return;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		int nLayoutCnt = pDoc->m_pEcsLayOuts.GetSize();
		for (int i = 0; i < nLayoutCnt; i++)
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
	pDoc->m_tChecktime = COleDateTime::GetCurrentTime();

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

	CString strCID, strEquipNum;
	strCID.Format(_T("%u"), wParam);
	if (CLib::IsValidControlID(strCID) == FALSE)
		return 0;

	int nPlcNum = _ttoi(strCID.Mid(5, 1));

	CDciControl* pDciControl = pDoc->GetDciControl(strCID);

	if (pDciControl == NULL)	return 0;
	if (!pDciControl->m_bEnable)	return 0;

	int nWarehouse = CLib::GetWarehouse(strCID);
	int nEquipKind = CLib::GetEquipment(strCID);
	int nEquipNum = CLib::GetEquipNum(strCID);

	//클릭 컨트롤이 SC일때는 nEquipNum를 두자리만 가져오기.
	if (nEquipKind == 20)
	{
		nEquipNum = _ttoi(strCID.Right(5));
		nEquipNum = (nEquipNum % 1000) + (nEquipNum / 1000 * 100 - 100);

		if (nEquipNum % 100 > 10)
			nEquipNum = nEquipNum - 10;

		// test
		nEquipNum = _ttoi(strCID.Right(2));
	}

	int nTrackNum = nEquipNum;// + (nWarehouse-1)*1000;

	//int nCvPlcNum = nEquipKind - 10;
	int nCurrPlcNum = nPlcNum;

	//PLC별로 10~ 15 번일 경우 컨베이어로 씀
	//if(nEquipKind/10 == 1)
	//	nEquipKind = 10;
	switch (nEquipKind)
	{
	case CEquipment::enCV:
	{
		//pDoc->m_oCvDlg.m_pDoc = pDoc;
		//pDoc->m_oCvDlg.m_pTrack = pDoc->GetTrackInfo(nTrackNum, 0);
		//DEBUGER_ASSERT_VALID(pDoc->m_oCvDlg.m_pTrack != NULL);

		pDoc->m_oControlDlg.m_pDoc = pDoc;
		pDoc->m_oControlDlg.m_nCurrPlcNum = nCurrPlcNum;
		pDoc->m_oControlDlg.m_nTrackNum = nTrackNum;

		CTrackInfo* pTrack = pDoc->GetTrackInfo(nTrackNum, nCurrPlcNum);
		if (pTrack != NULL)
		{
			CStationInfo* pStation = pTrack->GetArvStation();
			if (pStation != NULL)
			{
				pDoc->m_oControlDlg.m_strID = pStation->m_strID;

			}
		}

		UpdateData(TRUE);
		if (pDoc->m_oControlDlg.m_hWnd == NULL)
		{
			pDoc->m_oControlDlg.Create(IDD_ECS_CONTROL);

			CRect MainRect;
			CRect Rect;
			CRect PosRect;

			::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);
			pDoc->m_oControlDlg.GetWindowRect(&Rect);

			PosRect.left = ((MainRect.right - MainRect.left) - Rect.Width()) / 2;
			PosRect.top = ((MainRect.bottom - MainRect.top) - Rect.Height()) / 2;
			pDoc->m_oControlDlg.SetWindowPos(&wndTop, PosRect.left, PosRect.top,
				Rect.Width(), Rect.Height(),
				SWP_SHOWWINDOW);

			pDoc->m_oControlDlg.Refresh();
		}
		else
		{
			if (!pDoc->m_oControlDlg.IsWindowVisible())
				pDoc->m_oControlDlg.ShowWindow(SW_SHOWNORMAL);

			pDoc->m_oControlDlg.SendMessage(WM_INITDIALOG);
			pDoc->m_oControlDlg.Refresh();
		}
	}
	break;
	case CEquipment::enEQUIP:
	{
		pDoc->m_pJobDlg->SET(IDC_EDT_DEST_POS, strCID.Right(4));
	}
	break;
	case CEquipment::enCV1:
	{
		pDoc->m_oCvDlg.m_pDoc = pDoc;
		pDoc->m_oCvDlg.m_pTrack = pDoc->GetTrackInfo(nTrackNum, 0);
		DEBUGER_ASSERT_VALID(pDoc->m_oCvDlg.m_pTrack != NULL);
		pDoc->m_oCvDlg.DoModal();
	}
	break;
	case CEquipment::enSC:
	{
		//디버터 설정 창이 떠 있을때만 적용
		if (pDoc->m_pJobDlg != NULL)
		{
			CString strScNo;
			strScNo.Format(_T("%05d"), _ttoi(strCID.Right(2)));

			if (pDoc->m_pJobDlg->m_bDestPos == TRUE)
			{
				pDoc->m_pJobDlg->SET(IDC_EDT_DEST_POS, strScNo);
			}
		}

		if (nEquipNum / 100 != 3)
		{
			CScPair* pScPair = (CScPair*)pDoc->GetEquipment(nEquipKind, nEquipNum);
			if (pScPair != NULL && pScPair->m_pInfo != NULL && pScPair->m_pInfo->m_pStation != NULL)
			{
				if (pDoc->m_pJobDlg != NULL)
				{
					if (pDoc->m_pJobDlg->m_bDestPos == TRUE)
					{
						//CString strTemp;
						//strTemp.Format(_T("%05d", nTrackNum);
						pDoc->m_pJobDlg->SET(IDC_EDT_DEST_POS, pScPair->m_pInfo->m_pStation->m_strID);
					}
				}
			}
		}
		else
		{
			CSc* pSc = (CSc*)pDoc->GetEquipment(nEquipKind, nEquipNum);
			if (pSc != NULL && pSc->m_pInfo != NULL && pSc->m_pInfo->m_pStation != NULL)
			{
				if (pDoc->m_pJobDlg != NULL)
				{
					if (pDoc->m_pJobDlg->m_bDestPos == TRUE)
					{
						//CString strTemp;
						//strTemp.Format(_T("%05d", nTrackNum);
						pDoc->m_pJobDlg->SET(IDC_EDT_DEST_POS, pSc->m_pInfo->m_pStation->m_strID);
					}
				}
			}
		}
	}
	break;

	case CEquipment::enBCR:
	{
		//CEquipment* pEquipment = pDoc->GetEquipment(nEquipKind, nEquipNum);
		//DEBUGER_ASSERT_VALID(pEquipment != NULL);
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
	GET(IDC_COMBO_PLC, strPlcNo);
	nPlcNo = _ttoi(strPlcNo);
	GET(IDC_TRACK_NO2, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	nDeviceNo = _ttoi(strDeviceNo.Mid(2, 5));
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	UpdateList(nPlcNo, nDeviceNo % 100 * 10);
	UpdateBits(nPlcNo, nDeviceNo % 100 * 10);
	UpdateTrack(nPlcNo, nDeviceNo);
}

void CEcsView::UpdateList(int nPlcNo, int nDeviceNo)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_REG_DATA);
	LV_ITEM lvitem;
	TCHAR szBuff[256];
	int nLine = 0;

	CString strPlcNo, strDeviceNo;
	strPlcNo.Format(_T("%d"), nPlcNo);
	SET(IDC_COMBO_PLC, strPlcNo);
	strDeviceNo.Format(_T("%d"), nDeviceNo);
	SET(IDC_EDIT_REGNUM, strDeviceNo);

	if (nDeviceNo == 0)
		return;
	pListCtrl->DeleteAllItems();

	for (int i = nDeviceNo; i < nDeviceNo + 10; i++)
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
		wsprintf(szBuff, _T("%04d"), pDoc->m_arrRegData[nPlcNo - 1][i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%02X %02X"), CLib::GetByteH(pDoc->m_arrRegData[nPlcNo - 1][i]), CLib::GetByteL(pDoc->m_arrRegData[nPlcNo - 1][i]));
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);
	}
}

void CEcsView::UpdateBits(int nPlcNo, int nDeviceNo)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nLine = 0;

	CString strRegName;
	strRegName.Format(_T("D%05d"), nDeviceNo);
	SET(IDC_REG_NAME, strRegName);

	SET_INT(IDC_EDIT_REGNUM, nDeviceNo);

	if (nDeviceNo == 0)
		return;
	m_bBit01 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 0);
	m_bBit02 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 1);
	m_bBit03 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 2);
	m_bBit04 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 3);
	m_bBit05 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 4);
	m_bBit06 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 5);
	m_bBit07 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 6);
	m_bBit08 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 7);
	m_bBit09 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 8);
	m_bBit10 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 9);
	m_bBit11 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 10);
	m_bBit12 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 11);
	m_bBit13 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 12);
	m_bBit14 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 13);
	m_bBit15 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 14);
	m_bBit16 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 15);

	strRegName.Format(_T("%05d"), pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo]);
	SET(IDC_DECIMAL, strRegName);
	strRegName.Format(_T("%04X"), pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo]);
	SET(IDC_HEXA, strRegName);

	UpdateData(FALSE);
}

void CEcsView::UpdateTrack(int nPlcNo, int nDeviceNo)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nLine = 0;

	int nTrackNum;
	/*
	if (nPlcNo == 1)
		nTrackNum = (nDeviceNo/10);
	else
		nTrackNum = (nDeviceNo/10)+(nPlcNo*1000);
*/
	nTrackNum = nDeviceNo;

	nDeviceNo = nTrackNum % 1000 * 10;
	CString strTrNum, strRegName;
	strTrNum.Format(_T("%05d"), nTrackNum);
	SET(IDC_TRACK_NO, strTrNum);
	strTrNum.Format(_T("TR%05d"), nTrackNum);
	SET(IDC_TRACK_NO2, strTrNum);

	CTrackInfo* pTrack = NULL;
	pTrack = pDoc->GetTrackInfo(nTrackNum, nPlcNo);
	if (pTrack != NULL)
	{
		m_cbxCvKind.SetCurSel(pTrack->m_nKind);

		if (pTrack->m_nKind == 0)
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
			strTrNum.Format(_T("%05d"), pTrack->m_nNextCv);
			SET(IDC_EDIT_NEXT_CV, strTrNum);
			strTrNum.Format(_T("%05d"), pTrack->m_nCenterNext);
			SET(IDC_EDIT_CENTER_NEXT, strTrNum);
			strTrNum.Format(_T("%05d"), pTrack->m_nCenterDest);
			SET(IDC_EDIT_CENTER_DEST, strTrNum);
			strTrNum.Format(_T("%05d"), pTrack->m_nCrossTrack);
			SET(IDC_EDIT_CROSS_TRACK, strTrNum);
			m_bCenter = pTrack->m_bCenter;
			m_bCross = pTrack->m_bCross;
			pDoc->m_nNextPlcNum = pTrack->m_nNextPlcNum;
			pDoc->m_nCenterNextPlcNum = pTrack->m_nCenterNextPlcNum;
			pDoc->m_nCrossPlcNum = pTrack->m_nCrossPlcNum;
		}
		else if (pTrack->m_nKind == 1)
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
			strTrNum.Format(_T("%05d"), 0);
			SET(IDC_EDIT_NEXT_CV, strTrNum);
			SET(IDC_EDIT_CENTER_NEXT, strTrNum);
			SET(IDC_EDIT_CENTER_DEST, strTrNum);
			SET(IDC_EDIT_CROSS_TRACK, strTrNum);
			m_bCenter = FALSE;
			m_bCross = FALSE;
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
			strTrNum.Format(_T("%05d"), 0);
			SET(IDC_EDIT_NEXT_CV, strTrNum);
			SET(IDC_EDIT_CENTER_NEXT, strTrNum);
			SET(IDC_EDIT_CENTER_DEST, strTrNum);
			SET(IDC_EDIT_CROSS_TRACK, strTrNum);
			m_bCenter = FALSE;
			m_bCross = FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("해당 트랙이 존재하지 않습니다."));
		return;
	}

	CStationInfo* pStation = NULL;
	CTrackHS* pTrackHS = NULL;

	if (pStation = pTrack->GetStoStation())
		SET(IDC_EDIT_STATION, _T("입고대"));
	else if (pStation = pTrack->GetArvStation())
	{
		if (pStation->m_enKind == CStationInfo::enArvStation)
			SET(IDC_EDIT_STATION, _T("도착대"));
		if (pStation->m_enKind == CStationInfo::enArvStation2)
			SET(IDC_EDIT_STATION, _T("도착대2"));
	}
	else if (pStation = pTrack->GetRetStation())
		SET(IDC_EDIT_STATION, _T("출고대"));

	else
		SET(IDC_EDIT_STATION, _T(" "));

	if (pTrackHS = pTrack->GetScRetHS())
		SET(IDC_EDIT_HOME_ST, _T("출고HS"));
	else if (pTrackHS = pTrack->GetScStoHS())
		SET(IDC_EDIT_HOME_ST, _T("입고HS"));
	else if (pTrackHS = pTrack->GetRtvDepartHS())
		SET(IDC_EDIT_HOME_ST, _T("RTV출발 HS"));
	else if (pTrackHS = pTrack->GetRtvArriveHS())
		SET(IDC_EDIT_HOME_ST, _T("RTV도착 HS"));
	else if (pTrackHS = pTrack->GetLgvDepartHS())
		SET(IDC_EDIT_HOME_ST, _T("LGV출발 HS"));
	else if (pTrackHS = pTrack->GetLgvArriveHS())
		SET(IDC_EDIT_HOME_ST, _T("LGV도착 HS"));
	else
		SET(IDC_EDIT_HOME_ST, _T(" "));

	WORD asas = pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 5];

	//SK이노베이션용
/*	strRegName.Format(_T("%04d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo]);
	SET(IDC_LUGG_NUM,strRegName);
	strRegName.Format(_T("%04d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+1]);
	SET(IDC_DEST_POS,strRegName);
	strRegName.Format(_T("%02d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2]%100);
	SET(IDC_JOB_TYPE,strRegName);
	strRegName.Format(_T("%04d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+3]);
	SET(IDC_TRAY_TYPE,strRegName);
	strRegName.Format(_T("%01d"),(pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+5] & 0X000F));
	SET(IDC_SUSPEND,strRegName);
	strRegName.Format(_T("%04d"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+6]);
	SET(IDC_ERR_CODE,strRegName);
*/

//SKI 시
	strRegName.Format(_T("%05d"), pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo]);
	SET(IDC_LUGG_NUM, strRegName);

	strRegName.Format(_T("%05d"), pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 1]);
	SET(IDC_DEST_POS, strRegName);

	strRegName.Format(_T("%02d"), (pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] >> 8) & 0x00FF);
	SET(IDC_TRAY_TYPE, strRegName);

	strRegName.Format(_T("%05d"), pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] & 0x000F);
	SET(IDC_JOB_TYPE, strRegName);

	strRegName.Format(_T("%04d"), pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 3] & 0x000F);
	SET(IDC_EDIT_SIZE, strRegName);
	strRegName.Format(_T("%04d"), (pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] >> 4) & 0x000F);
	SET(IDC_EDIT_TURN, strRegName);

	strRegName.Format(_T("%01d"), (pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 5] & 0X000F));
	SET(IDC_SUSPEND, strRegName);
	strRegName.Format(_T("%05d"), pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 6]);
	SET(IDC_ERR_CODE, strRegName);

	strRegName.Format(_T("%01d"), pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 4]);
	SET(IDC_TRAY_ID_2, strRegName);

	strRegName.Format(_T("%01d"), pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 9]);
	SET(IDC_EDIT_LGV_AGREE, strRegName);

	WORD wWord = pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] / 100;
	WORD wWord2 = wWord & 0x01;
	WORD wWord3 = wWord >> 1;
	DWORD wdWord = wWord2 << 16;
	DWORD wdWord2 = wdWord | pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 4];

	WORD wWord233 = pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7];
	//	strRegName.Format(_T("%02X"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2]/100);
	//	SET(IDC_EDIT_SIZE,strRegName);
	//	strRegName.Format(_T("%04X"),pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4]);
	//	SET(IDC_TRAY_ID_2,strRegName);

	//	strRegName.Format(_T("%c%04d"),wWord3,wdWord2);
	//	SET(IDC_TRAY_ID,strRegName);

	m_bBit17 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 0);
	m_bBit18 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 1);
	m_bBit19 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 2);
	m_bBit20 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 3);
	m_bBit21 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 4);
	m_bBit22 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 5);
	m_bBit23 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 6);
	m_bBit24 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 7);
	m_bBit25 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 8);
	m_bBit26 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 9);
	m_bBit27 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 10);
	m_bBit28 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 11);
	m_bBit29 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 12);
	m_bBit30 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 13);
	m_bBit31 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 14);
	m_bBit32 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 15);

	m_bBit33 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 0);
	m_bBit34 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 1);
	m_bBit35 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 2);
	m_bBit36 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 3);
	m_bBit37 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 4);
	m_bBit38 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 5);
	m_bBit39 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 6);
	m_bBit40 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 7);
	m_bBit41 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 8);
	m_bBit42 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 9);
	m_bBit43 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 10);
	m_bBit44 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 11);
	m_bBit45 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 12);
	m_bBit46 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 13);
	m_bBit47 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 14);
	m_bBit48 = CLib::IsSet(pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 15);

	UpdateData(FALSE);
}

void CEcsView::OnButton18()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	int nPlcNo, nDeviceNo, nTrackNum;
	GET(IDC_COMBO_PLC, strPlcNo);
	nPlcNo = _ttoi(strPlcNo);

	GET(IDC_TRACK_NO2, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	nTrackNum = _ttoi(strDeviceNo.Mid(2, 5));
	nDeviceNo = nTrackNum % 1000 * 10;
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	CString strDecimal;

	int nDecimal;
	GET(IDC_DECIMAL, strDecimal);
	nDecimal = _ttoi(strDecimal);
	if (nDecimal < 0 || nDecimal > 65535)	return;

	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo] = nDecimal;

	GET(IDC_EDIT_REGNUM, strDeviceNo);

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
	UpdateTrack(nPlcNo, nTrackNum);
}

void CEcsView::OnButton13()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	int nPlcNo, nDeviceNo, nTrackNum;
	GET(IDC_COMBO_PLC, strPlcNo);
	nPlcNo = _ttoi(strPlcNo);

	//	GET(IDC_REG_NAME,strDeviceNo);	
	//	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_TRACK_NO2, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	nTrackNum = _ttoi(strDeviceNo.Mid(2, 5));
	nDeviceNo = nTrackNum % 1000 * 10;
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	CString strHexa;

	GET(IDC_HEXA, strHexa);

	unsigned long nHexa;		//변경할 HEX값
	unsigned long nStartHexa;   //비교할 HEX값 : 0
	unsigned long nEndHexa;		//비교할 HEX값 : FFFF
	nHexa = _tcstoul(strHexa, NULL, 16);
	nStartHexa = _tcstoul(_T("0000"), NULL, 16);
	nEndHexa = _tcstoul(_T("FFFF"), NULL, 16);

	if (nHexa < nStartHexa || nHexa > nEndHexa)
		return;

	TCHAR szHexa[5];
	wsprintf(szHexa, _T("%4s"), strHexa);

	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo] = CLib::HexStrToWORD((TCHAR*)szHexa);

	GET(IDC_EDIT_REGNUM, strDeviceNo);

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
	UpdateTrack(nPlcNo, nTrackNum);
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
	if (m_bBit11)
		nTemp += 0x400;
	if (m_bBit12)
		nTemp += 0x800;
	if (m_bBit13)
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
	GET(IDC_COMBO_PLC, strPlcNo);
	nPlcNo = _ttoi(strPlcNo);

	//	GET(IDC_REG_NAME,strDeviceNo);	
	//	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_TRACK_NO2, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	strDeviceNo = strDeviceNo.Mid(2, 5);
	nDeviceNo = GET_INT(IDC_EDIT_REGNUM);
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo] = nTemp;

	//	UpdateList(nPlcNo, (nDeviceNo/10)*10);
	//	UpdateTrack(nPlcNo, (nDeviceNo/10)*10);
	//	UpdateList(nPlcNo, (nDeviceNo%1000)*10);
	//	UpdateTrack(nPlcNo, nDeviceNo);	
	UpdateList(nPlcNo, nDeviceNo);
	UpdateTrack(nPlcNo, _ttoi(strDeviceNo));
}

void CEcsView::OnButton17()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	//	GET(IDC_COMBO_PLC,strPlcNo);	
	//	if (strPlcNo.IsEmpty()) return;

	GET(IDC_TRACK_NO, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;

	nPlcNo = _ttoi(strDeviceNo.Mid(0, 2));

	//	nPlcNo = _ttoi(strPlcNo);
	nDeviceNo = _ttoi(strDeviceNo);
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

	//	nPlcNo = nDeviceNo / 1000;
	//	nDeviceNo = (nDeviceNo % 1000) * 10;
	//	nDeviceNo = (nDeviceNo % 100)*10;

	//	UpdateList(nPlcNo, (nDeviceNo/10)*10);
	//	UpdateBits(nPlcNo, (nDeviceNo/10)*10);
	//	UpdateTrack(nPlcNo, (nDeviceNo/10)*10);
	UpdateList(nPlcNo, (nDeviceNo % 100) * 10);
	UpdateBits(nPlcNo, (nDeviceNo % 100) * 10);
	UpdateTrack(nPlcNo, nDeviceNo);
}

void CEcsView::OnButton14()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = nDeviceNo % 100 * 10;
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

	//	nPlcNo = nDeviceNo / 1000;
	//	if (nPlcNo == 0)
	//		nPlcNo++;
	nPlcNo = _ttoi(strPlcNo);
	//	nDeviceNo = (nDeviceNo % 1000) * 10;
	//	nDeviceNo = nDeviceNo%1000*10;

	CString strLuggNum, strDestPos, strDeadLock, strJobType, strTrayType, strTrayId, strSuspend, strErrCode, strRgv, strPlc, strTurn, strSize;

	GET(IDC_LUGG_NUM, strLuggNum);
	GET(IDC_DEST_POS, strDestPos);
	GET(IDC_EDIT_SIZE, strSize);
	GET(IDC_JOB_TYPE, strJobType);
	GET(IDC_TRAY_TYPE, strTrayType);
	GET(IDC_TRAY_ID_2, strTrayId);
	GET(IDC_TRAY_ID, strTrayId);
	GET(IDC_EDIT_TURN, strTurn);
	GET(IDC_SUSPEND, strSuspend);
	GET(IDC_ERR_CODE, strErrCode);
	GET(IDC_TRAY_ID_2, strRgv);
	GET(IDC_EDIT_LGV_AGREE, strPlc);

	/*
		pDoc->m_arrRegData[nPlcNo-1][nDeviceNo] = _ttoi(strLuggNum);
		pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+1] = _ttoi(strDestPos);
		pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+3] = _ttoi(strTrayType);
		pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] = pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] | _ttoi(strSuspend);
		pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+6] = _ttoi(strErrCode);
	*/
	//	strRegName.Format(_T("%02d",pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+1]%100);
	//	SET(IDC_JOB_TYPE,strRegName);

	//	strJobType, strTrayId

	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo] = _ttoi(strLuggNum);

	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 1] = _ttoi(strDestPos);
	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] = (_ttoi(strTrayType) << 8) + (_ttoi(strTurn) << 4) + _ttoi(strJobType);
	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 3] = _ttoi(strSize);
	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 4] = (_ttoi(strTrayId) << 4) + _ttoi(strSuspend);
	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 6] = _ttoi(strErrCode);
	//	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] = _ttoi(strRgv);
	//	pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+9] = _ttoi(strPlc);

	int kkk = pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 1];

	/*	int nLen = strTrayId.GetLength();

		if (nLen == 6)
		{
			TCHAR strTemp1[2];
			wsprintf(strTemp1, _T("%s"), strTrayId.Mid(0,1));
			CString strTemp2 = strTrayId.Mid(1,5);
			unsigned long nTemp = atol(strTemp2);

			BYTE byByte = nTemp >> 16;
			WORD wWord  = nTemp & 0xFFFF;
			WORD wWord2 = (strTemp1[0]) << 1;
			wWord2 = wWord2 | byByte;
			wWord2 = wWord2*100 + _ttoi(strDestPos);
			pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+2] = wWord2;
			pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+4] = wWord;
		}
	*/
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

	GET(IDC_TRACK_NO, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = (nDeviceNo % 100) * 10;
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

	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7] = nTemp;

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
}

void CEcsView::OnButton16()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = (nDeviceNo % 100) * 10;
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

	//	nPlcNo = nDeviceNo / 1000;
	nPlcNo = _ttoi(strPlcNo);
	/*	if (nPlcNo == 0)
			nPlcNo++;
		nDeviceNo = (nDeviceNo % 1000) * 10;
	*/

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

	pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8] = nTemp;

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

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;
	GET(IDC_COMBO_CV_KIND, strCvKind);
	if (strCvKind.IsEmpty()) return;
	GET(IDC_EDIT_NEXT_CV, strNextCv);
	int nNextCv = _ttoi(strNextCv);
	if (nNextCv <= 0) return;

	UpdateData(TRUE);

	int nCenterNext, nCenterDest, nCrossTrack, nCenterNextPlcNum, nCrossPlcNum;

	if (m_bCenter)
	{
		GET(IDC_EDIT_CENTER_NEXT, strCenterNext);
		nCenterNext = _ttoi(strCenterNext);
		if (nCenterNext <= 0) return;
		GET(IDC_EDIT_CENTER_DEST, strCenterDest);
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
		GET(IDC_EDIT_CROSS_TRACK, strCrossTrack);
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
		pTrack->m_nKind = _ttoi(strCvKind.Mid(0, 1));
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

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	CTrackInfo* pTrack = NULL;
	pTrack = pDoc->GetTrackInfo(nTrNum, nPlcNo);

	if (pTrack == NULL)
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

void CEcsView::OnButton22()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strTrNum, strCvKind, strNextCv;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	//nPlcNo = 7;
	CCv* pCv = (CCv*)pDoc->GetEquipment(CEquipment::enCV, nPlcNo);

	CTrackInfo* pTrack = NULL;

	if (pCv == NULL)
		return;

	for (int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
	{
		pTrack = pCv->m_pInfo->m_pTracks[i];

		int nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * 10;
		pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7] = pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7] | enBit11;
	}
}

void CEcsView::OnSelchangeComboCvKind()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CString strPlcNo, strTrNum, strCvKind;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	GET(IDC_COMBO_CV_KIND, strCvKind);

	int nKind = _ttoi(strCvKind.Mid(0, 1));
	int nSel = m_cbxCvKind.GetCurSel();

	CTrackInfo* pTrack = NULL;
	pTrack = pDoc->GetTrackInfo(nTrNum, nPlcNo);

	if (pTrack == NULL)
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
		strTrNum.Format(_T("%05d"), pTrack->m_nNextCv);
		SET(IDC_EDIT_NEXT_CV, strTrNum);
		strTrNum.Format(_T("%05d"), pTrack->m_nCenterNext);
		SET(IDC_EDIT_CENTER_NEXT, strTrNum);
		strTrNum.Format(_T("%05d"), pTrack->m_nCenterDest);
		SET(IDC_EDIT_CENTER_DEST, strTrNum);
		strTrNum.Format(_T("%05d"), pTrack->m_nCrossTrack);
		SET(IDC_EDIT_CROSS_TRACK, strTrNum);
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
		strTrNum.Format(_T("%05d"), 0);
		SET(IDC_EDIT_NEXT_CV, strTrNum);
		SET(IDC_EDIT_CENTER_NEXT, strTrNum);
		SET(IDC_EDIT_CENTER_DEST, strTrNum);
		SET(IDC_EDIT_CROSS_TRACK, strTrNum);
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
		strTrNum.Format(_T("%05d"), 0);
		SET(IDC_EDIT_NEXT_CV, strTrNum);
		SET(IDC_EDIT_CENTER_NEXT, strTrNum);
		SET(IDC_EDIT_CENTER_DEST, strTrNum);
		SET(IDC_EDIT_CROSS_TRACK, strTrNum);
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
	int nDeviceNo = _ttoi(strDevNo.Mid(1, 4));

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
	for (int i = 0; i < pDoc->m_pStationInfos.GetSize(); ++i)
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

void CEcsView::InvokeStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	/*
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);

	CTrackHS* pTrackHS = NULL;
	if (pTrack == NULL || pStation == NULL)	return;
	int nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * 10;

	int nAAA, nBBB, nCCC;
	int m_nNumber = pCv->m_nNumber;

	//	if( m_nNumber > 9)
	//		nDevNum = (pTrack->m_nNumber%10-pCv->m_nStTrNum+1)*10;

	if (pTrack->m_nNumber == 1126)
		int aaa = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit15;

	if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit11) != enBit11)
	{
		if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] != 0)
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = 0;
		//if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] != 0)
		//	m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = 0;

		return;
	}

	switch (pStation->m_enKind)
	{
	case CStationInfo::enStoStation:

		nAAA = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum];
		nBBB = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit11;
		nCCC = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit12;

		if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0 &&
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] != pTrack->m_nNumber &&
			(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) != enBit13)
		{
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] = 999;
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] = pTrack->m_nNumber;
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] | enBit13;
			if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) != enBit11)
				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] | enBit11;
		}
		else if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] != 0 &&
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] != pTrack->m_nNumber &&
			(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) == enBit13)
		{
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit13;
			nBBB = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7];
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
				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) != enBit11 &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] != 1) &&
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 0)
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit11;
				}
			}
			else if (pTrackHS = pTrack->GetScRetHS())
			{
			}
			else if (pTrackHS = pTrack->GetRtvArriveHS())
			{
				int aaa = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum];
				if (aaa = 1111)
					int a = 0;
				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11)
				{
					if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit17) == enBit17)
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit17;
				}
				else if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] != 0) &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11)
				{
					if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit17) == enBit17)
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit17;
				}
			}

			else
			{
				if (pTrack->m_nNumber == 1469 || pTrack->m_nNumber == 2469)
				{
					if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] / 1000 != 8 && (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11)
					{
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] = 8331;
					}

				}

				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) != enBit13 &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) != enBit11)
					return;

				WORD bbb = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum];
				WORD ccc = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13;
				WORD ddd = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11;

				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0) &&
					((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) == enBit13 ||
						(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11))
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit13;
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit26;
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] = 0;
				}
				else if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == pTrack->m_nNumber &&
					((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) != enBit13 &&
						(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11) &&
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] != 9998)
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] | enBit13;

					if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit12) != enBit12 &&
						(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 9998))
						//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit12;

						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] | enBit26;
					//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
				}
				else if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] != pTrack->m_nNumber &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) == enBit13)
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit13;
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit26;
					//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
				}
				if (pTrack->m_nDispenser != 0)
				{
					if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 9998) &&
						(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] != 9998))
					{
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] = 9998;
						//							if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12)!= enBit12)
														//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] |= enBit12;
					}
				}
			}
		}
		else if (pTrack->m_bCenter == 1)
		{
			if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 0 &&
				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0 &&
				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] == 0)
			{
				pTrack->m_bStartFlag = FALSE;
			}
			if (pTrack->m_nDispenser != 0)
			{
				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 9998) &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] != 9998))
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] = 9998;
					if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit12) != enBit12)
						//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] |= enBit12;
						if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) == enBit13)
							m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^= enBit13;
				}
			}
			if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == pTrack->m_nNumber)
			{
				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) != enBit13)
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] | enBit13;
				//				if((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12) != enBit12)
				//					m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit12;
			}
			//		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0 || m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] != pTrack->m_nNumber)
			//			return;
			if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 0 || m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] / 1000 != 8)
				return;
			//		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber && pTrack->m_bStartFlag != TRUE)
			//		{
			//			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;
			//			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
			//			pTrack->m_bStartFlag = TRUE;
			//			pTrack->m_tTime = CTime::GetCurrentTime();
			//		}

			if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] / 1000 == 8 && pTrack->m_bStartFlag != TRUE)
			{
				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) != enBit13)
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] | enBit13;

				//m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
				pTrack->m_bStartFlag = TRUE;
				pTrack->m_tTime = CTime::GetCurrentTime();
			}
			//		else if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber && pTrack->m_bStartFlag == TRUE)
			else if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] / 1000 == 8 && pTrack->m_bStartFlag == TRUE && (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] >> 12) == 1)
			{
				CTimeSpan tElapseTime = CTime::GetCurrentTime() - pTrack->m_tTime;
				if (tElapseTime.GetTotalSeconds() > 10)
				{
					int nNextPlcNum = pTrack->m_nCenterNextPlcNum - 1;
					int nNextDevNum = (pTrack->m_nCenterNext - m_pDoc->m_nStTrNum[nNextPlcNum] + 1) * 10;

					if (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum] == 0)
					{
						pTrack->m_bStartFlag = FALSE;

						if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit11) == enBit11)
						{
							m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = 0;
							m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = enBit11;
						}
						else
							m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = 0;

						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] = 0;

						int nLuggNum = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum];
						int nJobType = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 2] << 12;
						int nBarcode = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4];
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 2] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 3] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] = 0;

						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum] = nLuggNum;
						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum + 1] = pTrack->m_nCenterDest;
						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum + 2] = nJobType * 100 + 16;
						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum + 3] = 0;
						//	m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum+4] = nBarcode;

						m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum + 8] = m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum + 8] | enBit11;
					}
				}
			}
		}
		else
		{
			if (pTrackHS = pTrack->GetScRetHS())
			{
				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] | enBit15;
			}
			else if (pTrackHS = pTrack->GetRtvDepartHS())
			{
				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] | enBit17;
			}
			else
			{
				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 0) ||
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit13) == enBit13)
					return;

				if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == pTrack->m_nNumber)
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] | enBit13;
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] | enBit11;
				}
			}
		}

		break;

	case CStationInfo::enRetStation:
		if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 0)
			return;

		if (nDevNum == 260)
		{
			int aa = 0;
			aa = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 2];

		}
		if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == pTrack->m_nNumber && pTrack->m_bStartFlag != TRUE)
		{
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] | enBit13;
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] | enBit11;
			pTrack->m_bStartFlag = TRUE;
			pTrack->m_tTime = CTime::GetCurrentTime();
		}
		else if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == pTrack->m_nNumber && pTrack->m_bStartFlag == TRUE)
		{
			CTimeSpan tElapseTime = CTime::GetCurrentTime() - pTrack->m_tTime;
			if (tElapseTime.GetTotalSeconds() > 20)
			{
				pTrack->m_bStartFlag = FALSE;

				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit11) == enBit11)
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = enBit11;
				else
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = 0;

				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] = 0;

				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] = 0;
				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] = 0;
				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 2] = 0;
				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 3] = 0;
				m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] = 0;
			}
		}
		break;
	}
	//*/
}

void CEcsView::LgvStation(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	/*
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);

	CTrackHS* pTrackHS = NULL;
	if (pTrack == NULL || pStation == NULL)	return;
	int nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * 10;

	int m_nNumber = pCv->m_nNumber;

	//수동상태이거나, 자동상태이면서 신호요청이 들어온 경우..(들어왔다 꺼졌을 때도)

	if (pTrack->m_bCenter == 0)
	{
		if (pTrackHS = pTrack->GetLgvDepartHS())
		{
			if (!pTrack->m_bCross)
			{
				int ii = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4];
				//작업이 존재하고 LGV에서 신호가 들어왔을때...
				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] != 0) &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] != 0))
				{
					//화물감지 유무를 판단하여 화물이 있으면 정상 허가 신호 1  화물감지가 안되면 3
					if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11)
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 1;
					else
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 3;
				}
				else if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] != 0 &&
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] == 0)
				{
					if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] != 0)
					{
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 2] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 3] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 5] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 5] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 6] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] = 0;
						m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 0;
					}
				}
			}
		}

		if (pTrackHS = pTrack->GetLgvArriveHS())
		{
			int ii = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4];

			//정상적인 상황. 화물감지가 되어있지 않고 데이터가 존재하지 않을때.. D10n+9 = 0
			if (((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) != enBit11) &&
				(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0) &&
				(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] == 0))
			{
				if (m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] != 0 &&
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] != 4 &&
					((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) != enBit11))
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] |= enBit11;
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 0;
				}
				else if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit11) != enBit11)
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 0;
				else if (((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) != enBit11) &&
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0 &&
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] == 0 &&
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] == 4)
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 0;
			}
			//화물감지가 되어 있지 않고 LGV에서 트랙으로 도착 신호 받았을 때.. 
			else if (((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) != enBit11) &&
				(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] != 0))
			{
				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] != 0) ||
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] != 0) ||
					((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11))
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 4;
				}
				else if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 0) &&
					(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] == 2))
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 2;
				}
			}
			else if (((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11) &&
				((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] & enBit12) == enBit12) &&
				(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0) &&
				(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1] == 0))
			{
				if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 4] == 2))
				{
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 2;
				}
				else
					m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 9] = 0;
			}
		}
	}
	//*/
}

void CEcsView::InvokeHS(CCv* pCv, CTrackInfo* pTrack)
{
	/*
	CEcsDoc* m_pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	
	CTrackHS* pTrackHS = NULL;
	if (pTrack == NULL)	return;

	int nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * 10;

	int m_nNumber = pCv->m_nNumber;

	if (pTrack->GetRtvArriveHsChk() == TRUE)
	{
		if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11)
		{
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit17;
		}
	}

	if (pTrack->GetRtvDepartHSChk() == TRUE)
	{
		if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11)
		{
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit16;
		}
	}

	if ((pTrack->GetRtvArriveHsChk() == TRUE) && (pTrack->GetRtvDepartHSChk() == TRUE))
	{
		if ((m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum] == 0) &&
			(m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 8] & enBit11) == enBit11)
		{
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit16;
			m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] = m_pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 7] ^ enBit17;
		}
	}
	//*/
}

void CEcsView::MoveNextTrackForKindNormal_1(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	CTrackInfo* pNextTrack = NULL;

#pragma region 기본 정보 설정
	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 2;
	int nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * pDoc->m_nWordCnt;

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

	if (pTrack->m_bTwinCheck == TRUE)
	{
		int nHSDev = (pTrack->m_nHSTrack - pDoc->m_nStTrNum[m_nNumber - 1] + 1) * pDoc->m_nWordCnt;

		int nHsDevStatus = GET_REG_INFO_EXTRA(m_nNumber - 1, nHSDev, _T("StatusData"));

		if ((nLuggNo != 0) &&
			(nDestNo == pTrack->m_nHSTrack) &&
			//((nSensor & enBit11) == enBit11))
			//IS_BIT_ON(nSensor, enBit11))
			IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE))
		{
			//if ((nHsDevStatus & enBit13) == enBit13 ||
			//	(nHsDevStatus & enBit14) == enBit14)
			//if ((IS_BIT_ON(nHsDevStatus, enBit13)) ||
			//	(IS_BIT_ON(nHsDevStatus, enBit14)))
			if ((IS_BIT_KEY_EXTRA(m_nNumber - 1, nHSDev, _T("RetStation"), TRUE, FALSE)) ||
				(IS_BIT_KEY_EXTRA(m_nNumber - 1, nHSDev, _T("ScStoHS"), TRUE, FALSE)))
				return;
		}
	}
#pragma endregion
#pragma region 내부 변수 선언
	int nNextPlcNum = pTrack->m_nNextPlcNum - 1;

	int nNextDevNum = (pTrack->m_nNextCv- pDoc->m_nStTrNum[pTrack->m_nNextPlcNum-1]+1)* pDoc->m_nWordCnt;

	if (nNextPlcNum == 1)
		int aa = 0;

	if (nNextDevNum > 10000)
		return;

	int nCurrAuto = GET_REG_INFO_EXTRA(m_nNumber - 1, nDevNum, _T("Auto"));
	int nNextAuto = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("Auto"));
	int nCurrSensor = GET_REG_INFO_EXTRA(m_nNumber - 1, nDevNum, _T("ProductSensor"));
	int nNextSensor = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("ProductSensor"));

	int nNextTrLuggNo = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("LuggNum"));
	int nNextTrDestNo = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("DestPos"));
	int nNextTrStatus = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("StatusData"));
	int nNextTrSensor = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("SensorData"));

	if (nNextDevNum > 10000)
		return;
#pragma endregion

#pragma region 화물 옮기기 위한 조건 체크
	BOOL Temp3 = IS_BIT_ON(nStatus, enBit11);
	BOOL Temp4 = IS_BIT_ON(nNextTrStatus, enBit11);
	BOOL Temp = IS_BIT_KEY_CURR(_T("Auto"), TRUE);
	BOOL Temp2 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("Auto"), TRUE, FALSE);

	if (pTrack->m_nNumber == 235)
		int aa = 0;

	if (!(Temp && Temp2))
		return;

	// 목적지가 자기 자신이면
	if (nDestNo == pTrack->m_nDestCode)
		return;

	// 목적지가 0이면 
	if (nDestNo == 0)
		return;

	BOOL bTemp1 = IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE);
	BOOL bTemp2 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("ProductSensor"), TRUE, FALSE);

	if (bTemp1 && bTemp2)
		return;

	BOOL bTemp3 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("ProductSensor"), FALSE, FALSE);
	if (!((nNextTrLuggNo == 0 && bTemp3) &&
		(nLuggNo != 0 && bTemp1)))
		return;

	BOOL bTemp4 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("ScStoHS"), TRUE, FALSE);
	if (bTemp4)
		return;
#pragma endregion

	if (pTrack->m_bCross)
	{
		int nCrossPlcNum = pTrack->m_nCrossPlcNum;
		int nCrossDevNum = (pTrack->m_nCrossTrack- pDoc->m_nStTrNum[pTrack->m_nCrossPlcNum-1]+1)* pDoc->m_nWordCnt;

		int nCrossTrLuggNo = GET_REG_INFO_EXTRA(nCrossPlcNum, nCrossDevNum, _T("LuggNum"));
		int nCrossTrDestNo = GET_REG_INFO_EXTRA(nCrossPlcNum, nCrossDevNum, _T("DestPos"));
		int nCrossTrStatus = GET_REG_INFO_EXTRA(nCrossPlcNum, nCrossDevNum, _T("StatusData"));
		int nCrossTrSensor = GET_REG_INFO_EXTRA(nCrossPlcNum, nCrossDevNum, _T("SensorData"));

		CTrackHS* pTrackHS = NULL;
		if (pTrackHS = pTrack->GetScStoHS())
		{
			//if (IS_BIT_ON(nCrossTrStatus, enBit14))
			if (IS_BIT_KEY_EXTRA(nCrossPlcNum, nCrossDevNum, _T("ScStoHS"), TRUE, FALSE))
			{
				return;
			}
		}
		else
		{
			if (nCrossTrLuggNo != 0)
			{
				pNextTrack = pDoc->GetTrackInfo(pTrack->m_nNextCv, nNextPlcNum);
				if (pNextTrack->m_nGoTrack == pTrack->m_nNumber)
					return;
			}
		}
	}

	if (bTemp3 && pTrack->m_bStartFlag != TRUE)
	{
		pTrack->m_bStartFlag = TRUE;
		pTrack->m_tTime = COleDateTime::GetCurrentTime();
	}
	else if (bTemp3 && pTrack->m_bStartFlag == TRUE)
	{
		
		//CTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;
		COleDateTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;
		int nElapse = tElapseTime.GetTotalSeconds();
		if (nElapse > 2)
		{
			pTrack->m_bStartFlag = FALSE;
			int nResultAddr = SET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("ProductSensor"), nNextTrSensor, 1);						// ON

			if (IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE))
			{
				nResultAddr = SET_REG_INFO(_T("ProductSensor"), nSensor, 4);	// OFF
			}

			if (IS_BIT_KEY_CURR(_T("Auto"), TRUE))
			{
				nResultAddr = SET_REG_INFO(_T("Auto"), nStatus, 1);				// ON
				nResultAddr = SET_REG_INFO(_T("StoStation"), nStatus, 4);		// OFF
				nResultAddr = SET_REG_INFO(_T("RetStation"), nStatus, 4);		// OFF
				nResultAddr = SET_REG_INFO(_T("ScStoHS"), nStatus, 4);			// OFF
				nResultAddr = SET_REG_INFO(_T("ScRetHS"), nStatus, 4);			// OFF
				nResultAddr = SET_REG_INFO(_T("RtvDepartHS"), nStatus, 4);		// OFF
				nResultAddr = SET_REG_INFO(_T("RtvArriveHS"), nStatus, 4);		// OFF
				nResultAddr = SET_REG_INFO(_T("LgvDepartHS"), nStatus, 4);		// OFF
				nResultAddr = SET_REG_INFO(_T("LgvArriveHS"), nStatus, 4);		// OFF
				nResultAddr = SET_REG_INFO(_T("LifterDepartHS"), nStatus, 4);	// OFF
				nResultAddr = SET_REG_INFO(_T("LifterArriveHS"), nStatus, 4);	// OFF
				nResultAddr = SET_REG_INFO(_T("LgvInRequest"), nStatus, 4);		// OFF
				nResultAddr = SET_REG_INFO(_T("LgvOutRequest"), nStatus, 4);	// OFF
				nResultAddr = SET_REG_INFO(_T("CvCheckPos"), nStatus, 4);		// OFF
			}
			else
				nResultAddr = SET_REG_INFO(_T("StatusData"), 0x0000, 0);					// WORD RESET

			if (pNextTrack != NULL)
				pNextTrack->m_nGoTrack = pTrack->m_nNumber;
		}
	}
}

void CEcsView::MoveNextTrackForKindDiverter_1(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	CTrackInfo* pNextTrack = NULL;

	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 2;

	int nStTrNum = pCv->m_nStTrNum;
	int nDevNum = (pTrack->m_nNumber - nStTrNum + 1) * pDoc->m_nWordCnt;

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

	if (pTrack->m_nNumber == 112)
		int kk = 0;
	int nLen = pTrack->m_nStationArray.GetSize();

	for (int i = 0; i < nLen; i++)
	{
		int nStation = pTrack->m_nStationArray[i];
		CStationInfo* pStationInfo = pDoc->m_pStationInfos[i];
		int nDestCode = (pStationInfo != NULL && pStationInfo->m_pTrack != NULL) ? pStationInfo->m_pTrack->m_nDestCode : 0;

		//int a = pDoc->m_arrRegData[m_nNumber - 1][nDevNum];
		//a = pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1];
		if ((nStation != 0 && nStation == nDestNo) || 
			((pStationInfo != NULL) && (_ttoi(pStationInfo->m_strID) == nDestNo)) ||
			(nDestCode == nDestNo))
		{
			int nNextTrNum = pTrack->m_nNextTrArray[i];
			if (nNextTrNum == 0)
				continue;

			int nNextPlcNum = pTrack->m_nNextPlcArray[i] - 1;
			//if (pDoc->m_pDeviceMaps[nNextPlcNum]->m_bUseSeparatelyETC == TRUE)	// 배열에서 -1을 하지 않음! 
			//	n1TrackAtWord = 2;
			//else 
			//	n1TrackAtWord = 10;

			int nNextDevNum = (nNextTrNum - pDoc->m_nStTrNum[nNextPlcNum] + 1) * pDoc->m_nWordCnt;

			int nNextTrLuggNo = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("LuggNum"));
			int nNextTrDestNo = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("DestPos"));
			int nNextTrStatus = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("StatusData"));
			int nNextTrSensor = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("SensorData"));

			int nCurrAuto = (nStatus & enBit11);
			int nNextAuto = (nNextTrStatus & enBit11);
			int nCurrSensor = (nSensor & enBit11);
			int nNextSensor = (nNextTrSensor & enBit11);

			//2015.8.26 10g 양품 36개 트랙만 10개 쌓음
			int nFlag = 0;
			//int nCurrTrNum[37] = { 10357, 10354, 10351, 10348, 10345, 10342, 10339, 10336, 10333, 10330,
			//					  10327, 10324, 10321, 10318, 10315, 10312, 10309, 10306,
			//					  10302, 10299, 10295, 10293, 10290, 10287, 10284, 10281, 10278, 10275,
			//					  10272, 10269, 10266, 10263, 10260, 10257, 10254, 10251 };

			BOOL bOn1 = IS_BIT_KEY_CURR(_T("Auto"), TRUE);
			BOOL bOn2 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("Auto"), TRUE, FALSE);

			if (!(bOn1 && bOn2))
				return;

			if (pTrack->m_nNumber == 9006)
				int aa = 0;

			if (IS_BIT_KEY_CURR(_T("ProductSensor"), FALSE))
				return;

			BOOL bOn3 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("ProductSensor"), FALSE, FALSE);
			BOOL bOn4 = IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE);

			if (!((nNextTrLuggNo == 0 && bOn3) && bOn4))
				return;

			if (pTrack->m_bCross)
			{
				int nCrossPlcNum = pTrack->m_nCrossPlcNum;
				int nCrossDevNum = (pTrack->m_nCrossTrack - pDoc->m_nStTrNum[pTrack->m_nCrossPlcNum] + 1) * pDoc->m_nWordCnt;

				int nCrossTrLuggNo = GET_REG_INFO_EXTRA(nCrossPlcNum, nCrossDevNum, _T("LuggNum"));
				int nCrossTrDestNo = GET_REG_INFO_EXTRA(nCrossPlcNum, nCrossDevNum, _T("DestPos"));
				int nCrossTrStatus = GET_REG_INFO_EXTRA(nCrossPlcNum, nCrossDevNum, _T("StatusData"));
				int nCrossTrSensor = GET_REG_INFO_EXTRA(nCrossPlcNum, nCrossDevNum, _T("SensorData"));

				if (nNextTrNum == pTrack->m_nCrossDest && nCrossTrLuggNo != 0)
				{
					pNextTrack = pDoc->GetTrackInfo(nNextTrNum, nNextPlcNum);
					if (pNextTrack->m_nGoTrack == pTrack->m_nNumber)
						return;
				}
			}

			bOn1 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("ProductSensor"), FALSE, FALSE);
			//bOn2 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("Auto"), TRUE, FALSE);

			if (bOn1 && pTrack->m_bStartFlag != TRUE)
			{
				pTrack->m_bStartFlag = TRUE;
				pTrack->m_tTime = COleDateTime::GetCurrentTime();
			}
			else if (bOn1 && pTrack->m_bStartFlag == TRUE)
			{
				COleDateTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;
				if (tElapseTime.GetTotalSeconds() > 2)
				{
					pTrack->m_bStartFlag = FALSE;

					int nResultAddr = SET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("ProductSensor"), nNextTrSensor, 1);	// ON

					//BOOL bSignal = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("ProductSensor"), TRUE, TRUE);

					if (IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE))
					{
						nResultAddr = SET_REG_INFO(_T("ProductSensor"), nCurrSensor, 4);	// OFF
						//bSignal = IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE);
					}

					if (IS_BIT_KEY_CURR(_T("Auto"), TRUE))
					{
						nResultAddr = SET_REG_INFO(_T("Auto"), nStatus, 1);				// ON
						nResultAddr = SET_REG_INFO(_T("StoStation"), nStatus, 4);		// OFF
						nResultAddr = SET_REG_INFO(_T("RetStation"), nStatus, 4);		// OFF
						nResultAddr = SET_REG_INFO(_T("ScStoHS"), nStatus, 4);			// OFF
						nResultAddr = SET_REG_INFO(_T("ScRetHS"), nStatus, 4);			// OFF
						nResultAddr = SET_REG_INFO(_T("RtvDepartHS"), nStatus, 4);		// OFF
						nResultAddr = SET_REG_INFO(_T("RtvArriveHS"), nStatus, 4);		// OFF
						nResultAddr = SET_REG_INFO(_T("LgvDepartHS"), nStatus, 4);		// OFF
						nResultAddr = SET_REG_INFO(_T("LgvArriveHS"), nStatus, 4);		// OFF
						nResultAddr = SET_REG_INFO(_T("LifterDepartHS"), nStatus, 4);	// OFF
						nResultAddr = SET_REG_INFO(_T("LifterArriveHS"), nStatus, 4);	// OFF
						nResultAddr = SET_REG_INFO(_T("LgvInRequest"), nStatus, 4);		// OFF
						nResultAddr = SET_REG_INFO(_T("LgvOutRequest"), nStatus, 4);	// OFF
						nResultAddr = SET_REG_INFO(_T("CvCheckPos"), nStatus, 4);		// OFF
					}

					if (nNextTrNum == pTrack->m_nCrossDest && pNextTrack != NULL)
						pNextTrack->m_nGoTrack = pTrack->m_nNumber;

					CString strLog;
					strLog.Format(_T(""));
					pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, _T(""), _T("CEcsView::MoveNextTrackForKindDiverter_1"));
				}
			}
		}
	}
}

void CEcsView::MoveNextTrackForKindNormal_2(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

#pragma region 내부 변수 선언
	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 2;
	int nDevNum = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * pDoc->m_nWordCnt;

	int nNextPlcNum = pTrack->m_nNextPlcNum - 1;
	int nNextDevNum = (pTrack->m_nNextCv - pDoc->m_nStTrNum[nNextPlcNum] + 1) * pDoc->m_nWordCnt;

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nJobtype = GET_REG_INFO(_T("D10n+2"));
	int nSize = GET_REG_INFO(_T("D10n+3"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

	int nNextTrLuggNo = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("LuggNum"));
	int nNextTrDestNo = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("DestPos"));
	int nNextTrJobtype = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("D10n+2"));
	int nNextTrSize = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("D10n+3"));
	int nNextTrStatus = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("StatusData"));
	int nNextTrSensor = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, _T("SensorData"));

	if (nNextDevNum > 10000)
		return;

	int nCurrAuto = (nStatus & enBit11);
	int nNextAuto = (nNextTrStatus & enBit11);
	int nCurrSensor = (nSensor & enBit11);
	int nNextSensor = (nNextTrSensor & enBit11);
#pragma endregion


	BOOL bTemp1 = IS_BIT_KEY_CURR(_T("Auto"), TRUE);
	BOOL bTemp2 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("Auto"), TRUE, FALSE);
	BOOL bTemp3 = IS_BIT_KEY_CURR(_T("ProductSensor"), TRUE);
	BOOL bTemp4 = IS_BIT_KEY_EXTRA(nNextPlcNum, nNextDevNum, _T("ProductSensor"), TRUE, FALSE);
	BOOL bTemp5 = IS_BIT_KEY_CURR(_T("ProductSensor"), FALSE);

	if (pTrack->m_nNumber == 10)
		int aaaaaaaaaaaaaaa = 0;

	// 현재트랙과 다음트랙이 모두 Auto상태이어야함
	if (!(bTemp1 && bTemp2))
		return;

	// 목적지가 자기자신이 아니어야 함
	if (//nDestNo == pTrack->m_nNumber || 
		nDestNo == pTrack->m_nDestCode )
		return;

	// 목적지가 0이면 
	if (nDestNo == 0)
		return;

	// 현재트랙과 다음트랙이 모두 화물 감지가 되면 화물을 이동시킬 필요가 없음!
	if (bTemp3 && bTemp4)
		return;

	// 현재 트랙의 작업번호가 있고 화물이 없으며 다음 트랙에 작업번호가 없고 화물이 있을 경우               가 아니면 Data 복사하지 않음
	if (!((nNextTrLuggNo == 0 && bTemp4) &&
		(nLuggNo != 0 && bTemp5)))
		return;

	// 출발지의 작업번호와 목적지가 없으면 Data복사하지 않음!
	if (nLuggNo == 0 || nDestNo == 0)
		return;

	CStringArray* strTrackingAreas;
	strTrackingAreas = pDoc->m_strTrackingAreas.GetAt(m_nNumber - 1);

	if (strTrackingAreas != NULL)
	{
		if (m_nNumber == nNextPlcNum)
		{
			int nTrackingAreaCnt = strTrackingAreas->GetSize();
			for (int nn = 0; nn < nTrackingAreaCnt; nn++)
			{
				CString strTrackingArea = strTrackingAreas->GetAt(nn);
				int nValue = GET_REG_INFO(strTrackingArea);
				SET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, strTrackingArea, nValue, 0);
				SET_REG_INFO(strTrackingArea, 0x0000, 0);					// WORD RESET

				int nAfterValueAtNextTrack = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, strTrackingArea);
				int nAfterValueAtCurrTrack = GET_REG_INFO(strTrackingArea);

				int aaa = 0;
			}
		}
		else
		{
			for (POSITION pos = pDoc->m_strTrackingKeywordList.GetHeadPosition(); pos != NULL; )
			{
				CString& strKeyWord = pDoc->m_strTrackingKeywordList.GetNext(pos);
				int nLen = strKeyWord.GetLength();

				// 1. 현재 트랙의 KeyWord 가져오기 
				int nValue = GET_REG_INFO(strKeyWord);

				// 2. 다음 트랙의 KeyWord로 복사하기 
				int nNextTrAddr = SET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, strKeyWord, nValue, 0);

				// 3. 현재 트랙의 Keyword 값 지우기
				int nCurrTrAddr = SET_REG_INFO(strKeyWord, 0x0000, 0);					// WORD RESET???

				int nAfterValueAtNextTrack = GET_REG_INFO_EXTRA(nNextPlcNum, nNextDevNum, strKeyWord);
				int nAfterValueAtCurrTrack = GET_REG_INFO(strKeyWord);

				int aaa = 0;
			}
		}
	}
}

void CEcsView::MoveNextTrackForKindDiverter_2(CCv* pCv, CTrackInfo* pTrack, CStationInfo* pStation)
{
	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	int m_nNumber = pCv->m_nNumber;
	//int n1TrackAtWord = 10;
	//if (pDoc->m_pDeviceMaps[m_nNumber - 1]->m_bUseSeparatelyETC == TRUE)
	//	n1TrackAtWord = 2;

	int nStTrNum = pCv->m_nStTrNum;
	int nDevNum = (pTrack->m_nNumber - nStTrNum + 1) * pDoc->m_nWordCnt;
	if (pTrack->m_nNumber == 230)
		int aaaaaaaa = 0;
	int nLen = pTrack->m_nStationArray.GetSize();

	int nLuggNo = GET_REG_INFO(_T("LuggNum"));
	int nDestNo = GET_REG_INFO(_T("DestPos"));
	int nJobtype = GET_REG_INFO(_T("nJobtype"));
	int nSize = GET_REG_INFO(_T("D10n+3"));
	int nStatus = GET_REG_INFO(_T("StatusData"));
	int nSensor = GET_REG_INFO(_T("SensorData"));

	for (int i = 0; i < nLen; i++)
	{
		int nStation = pTrack->m_nStationArray[i];
		//int aa = pDoc->m_arrRegData[m_nNumber - 1][nDevNum + 1];
		CStationInfo* pStationInfo = pDoc->m_pStationInfos[i];
		int nDestCode = (pStationInfo != NULL && pStationInfo->m_pTrack != NULL) ? pStationInfo->m_pTrack->m_nDestCode : 0;

		// nDestCode가 0이면서, 트랙정보가 없는 StationInfo 라면...  => StackerCrane이다 => Stacker Crane 목적지 사용
		if (nDestCode == 0 && pStationInfo != NULL && pStationInfo->m_pTrack == NULL)
		{
			int nStnCode = _ttoi(pStationInfo->m_strID);
			if (nStnCode < 100 && nStnCode > 0)
				nDestCode = nStnCode;
		}

		//if ((nStation != 0 && nStation == nDestNo) || (pStationInfo != NULL && _ttoi(pStationInfo->m_strID) == nDestNo))
		if ((nStation != 0 && nStation == nDestNo) ||
			((pStationInfo != NULL) && (_ttoi(pStationInfo->m_strID) == nDestNo)) ||
			(nDestCode == nDestNo))
		{
			int nNextTrNum = pTrack->m_nNextTrArray[i];
			if (nNextTrNum == 0)
				continue;
			int nNextPlcNum = pTrack->m_nNextPlcArray[i];
			//if (pDoc->m_pDeviceMaps[nNextPlcNum-1]->m_bUseSeparatelyETC == TRUE)	// 배열에서 -1을 하지 않음! 
			//	n1TrackAtWord = 2;
			//else
			//	n1TrackAtWord = 10;
			int nNextDevNum = (nNextTrNum - pDoc->m_nStTrNum[nNextPlcNum-1] + 1) * pDoc->m_nWordCnt;

			int nNextTrLuggNo = GET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, _T("LuggNum"));
			int nNextTrDestNo = GET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, _T("DestPos"));
			int nNextTrJobtype = GET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, _T("D10n+2"));
			int nNextTrSize = GET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, _T("D10n+3"));
			int nNextTrStatus = GET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, _T("StatusData"));
			int nNextTrSensor = GET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, _T("SensorData"));

			int nCurrAuto = (nStatus & enBit11);
			int nNextAuto = (nNextTrStatus & enBit11);
			int nCurrSensor = (nSensor & enBit11);
			int nNextSensor = (nNextTrSensor & enBit11);

			BOOL Temp = IS_BIT_KEY_CURR(_T("Auto"), TRUE);
			BOOL Temp2 = IS_BIT_KEY_EXTRA(nNextPlcNum-1, nNextDevNum, _T("Auto"), TRUE, FALSE);
			BOOL Temp3 = IS_BIT_ON(nStatus, enBit11);
			BOOL Temp4 = IS_BIT_ON(nNextTrStatus, enBit11);

			// 현재트랙과 다음트랙이 모두 Auto 상태이어야 함!
			if (!(Temp && Temp2))
				return;

			// 현재트랙의 목적지가 자기 자신이 아니어야 함!
			if (nDestNo == pTrack->m_nNumber || 
				nDestNo == pTrack->m_nDestCode)
				return;

			BOOL Temp5 = IS_BIT_KEY_CURR(_T("ProductSensor"), FALSE);
			BOOL Temp6 = IS_BIT_KEY_EXTRA(nNextPlcNum-1, nNextDevNum, _T("ProductSensor"), TRUE, FALSE);

			// 둘다 화물감지가 되고 있어야 함!
			if (!(Temp5 && Temp6))
				return;

			// 출발지의 작업번호와 목적지가 없으면 Data복사하지 않음!
			if (nLuggNo == 0 || nDestNo == 0)
				return;

			CStringArray* strTrackingAreas;
			strTrackingAreas = pDoc->m_strTrackingAreas.GetAt(m_nNumber - 1);

			if (strTrackingAreas != NULL)
			{
				if (m_nNumber == nNextPlcNum)
				{
					int nTrackingAreaCnt = strTrackingAreas->GetSize();
					for (int nn = 0; nn < nTrackingAreaCnt; nn++)
					{
						CString strTrackingArea = strTrackingAreas->GetAt(nn);
						int nValue = GET_REG_INFO(strTrackingArea);
						int nNextTrAddr = SET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, strTrackingArea, nValue, 0);
						int nCurrTrAddr = SET_REG_INFO(strTrackingArea, 0x0000, 0);					// WORD RESET

						int nAfterValueAtNextTrack = GET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, strTrackingArea);
						int nAfterValueAtCurrTrack = GET_REG_INFO(strTrackingArea);

						int aaa = 0;
					}
				}
				// PLC 가 다를 경우 
				else
				{
					for (POSITION pos = pDoc->m_strTrackingKeywordList.GetHeadPosition(); pos != NULL; )
					{
						CString& strKeyWord = pDoc->m_strTrackingKeywordList.GetNext(pos);
						int nLen = strKeyWord.GetLength();

						// 1. 현재 트랙의 KeyWord 가져오기 
						int nValue = GET_REG_INFO(strKeyWord);

						// 2. 다음 트랙의 KeyWord로 복사하기 
						int nNextTrAddr = SET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, strKeyWord, nValue, 0);

						// 3. 현재 트랙의 Keyword 값 지우기
						int nCurrTrAddr = SET_REG_INFO(strKeyWord, 0x0000, 0);					// WORD RESET???

						int nAfterValueAtNextTrack = GET_REG_INFO_EXTRA(nNextPlcNum-1, nNextDevNum, strKeyWord);
						int nAfterValueAtCurrTrack = GET_REG_INFO(strKeyWord);

						int aaa = 0;
					}
				}
				return;		// 목적지가 2군데 인데 다음트랙이 같을때 
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
	int nDevNum = (pTrack->m_nNumber - nStTrNum + 1) * 10;
	int nNextDevNum = (nNextTrNum - m_pDoc->m_nStTrNum[nNextPlcNum] + 1) * 10;

	if (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum + 1] == 9999 && m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 1] == 9999)
	{
		m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 1] = 9998;
		m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 7] |= enBit12;
		pTrack->m_nMGTrack = nNextTrNum;
		pTrack->m_nMGPlcNum = nNextPlcNum;

		//if((m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] & enBit12) != enBit12)
			//m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] |= enBit12;
	}
	else if (m_pDoc->m_arrRegData[nNextPlcNum][nNextDevNum + 1] == 0 && m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 1] == 9999)
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
	int nDevNum = (pTrack->m_nNumber - nStTrNum + 1) * 10;
	int nNextDevNum = (nNextTrNum - m_pDoc->m_nStTrNum[nNextPlcNum] + 1) * 10;

	if (pTrack->m_nDispenser != 0)
	{
		int nDPPlcNum = pTrack->m_nDPPlcNum - 1;
		int nDPDevNum = (pTrack->m_nDispenser - m_pDoc->m_nStTrNum[nDPPlcNum] + 1) * 10;

		if (m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 1] == 9999)//&& 
		  // (m_pDoc->m_arrRegData[nNumber-1][nDevNum+4] >> 12) != 0	&& 
		  // (m_pDoc->m_arrRegData[nNumber-1][nDevNum+3] >> 4) ==  2	)
		{
			if (((m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 8] & enBit11) != enBit11 ||
				(m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 8] & enBit12) != enBit12) ||
				(m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum + 8] & enBit11) == enBit11)
				return;

			m_pDoc->m_arrRegData[nNumber - 1][nDevNum] = 9998;
			m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 1] = 9998;
			m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 4] ^= (1 << 12);
			//m_pDoc->m_arrRegData[nNumber-1][nDevNum+7] |= enBit12;
			m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 7] ^= enBit13;

			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum] = 9998;
			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum + 1] = pTrack->m_nNumber;
			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum + 2] |= 6;
			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum + 3] |= 0x0010;
			m_pDoc->m_arrRegData[nDPPlcNum][nDPDevNum + 8] ^= enBit11;
		}

		if (m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 1] == pTrack->m_nNumber &&
			(m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 3] & enBit15) == enBit15 &&
			(m_pDoc->m_arrRegData[nNumber - 1][nDevNum + 7] & enBit12) != enBit12)
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

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);
	CCv* pCv = (CCv*)pDoc->GetEquipment(CEquipment::enCV, nPlcNo);
	if (pCv == NULL)
		return;
	CTrackInfo* pTrack = NULL;
	for (int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
	{
		pTrack = pCv->m_pInfo->m_pTracks[i];

		int nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * 10;

		if ((pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7] & enBit11) == enBit11)
			pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7] = pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7] ^ enBit11;
	}
}

void CEcsView::OnButtonSave()
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	for (int i = 1; i < 25; ++i)
	{
		CCv* pCv = (CCv*)pDoc->GetEquipment(CEquipment::enCV, i);

		if (pCv == NULL)
			continue;
		CTrackInfo* pTrack = NULL;
		for (int j = 0; j < pCv->m_pInfo->m_pTracks.GetSize(); ++j)
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

	for (int j = 0; j < 40; j++)
	{
		CCv* pCv = (CCv*)pDoc->GetEquipment(CEquipment::enCV, j);

		CTrackInfo* pTrack = NULL;

		if (pCv == NULL)
			continue;

		for (int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
		{
			pTrack = pCv->m_pInfo->m_pTracks[i];

			int nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * 10;
			pDoc->m_arrRegData[j - 1][nDeviceNo + 7] = pDoc->m_arrRegData[j - 1][nDeviceNo + 7] | enBit11;
		}
	}
}

void CEcsView::OnBtnAllSuspend()
{
	// TODO: Add your control notification handler code here
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	for (int j = 0; j < 40; j++)
	{
		CCv* pCv = (CCv*)pDoc->GetEquipment(CEquipment::enCV, j);
		if (pCv == NULL)
			continue;
		CTrackInfo* pTrack = NULL;

		for (int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
		{
			pTrack = pCv->m_pInfo->m_pTracks[i];

			int nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * 10;

			if ((pDoc->m_arrRegData[j - 1][nDeviceNo + 7] & enBit11) == enBit11)
				pDoc->m_arrRegData[j - 1][nDeviceNo + 7] = pDoc->m_arrRegData[j - 1][nDeviceNo + 7] ^ enBit11;
		}
	}
}

void CEcsView::OnSysControl()
{
	// TODO: Add your command handler code here

}

void CEcsView::OnMouseMoveSelectedLayout(CEcsDoc* pDoc, UINT nFlags, CPoint point)
{
	//COleDateTimeSpan tTimeSpan;
	COleDateTimeSpan tTimeSpan = COleDateTime::GetCurrentTime() - pDoc->m_tChecktime;
	if (tTimeSpan.GetTotalSeconds() < 1)
		return;

	CEcsLayout* pLayout = (CEcsLayout*)pDoc->GetSelectedLayout();
	if (pLayout == NULL)
		return;

	if (pDoc->m_bMoveFlag == TRUE) //2
	{
		if (tTimeSpan.GetTotalSeconds() < 1)
			return;
		if (pDoc->m_PrevPoint != point)
		{
			if (pDoc->m_PrevPoint.x > point.x)
			{
				if (pDoc->m_PrevPoint.x - point.x > 3)
				{
					if (m_tabLayout.GetSafeHwnd())
					{
						//탭 별로 onkeydown 메세지를 던져준다.
						switch (m_tabLayout.GetCurSel())
						{
						case EN_1F:					pDoc->m_layout1.OnKeyDown(this, VK_RIGHT, 0, 0);				break;
						case EN_2F:				pDoc->m_layout2.OnKeyDown(this, VK_RIGHT, 0, 0);				break;
						case EN_Assemble:			pDoc->m_layoutAssemble.OnKeyDown(this, VK_RIGHT, 0, 0);			break;
						}
					}
					//pLayout->OnKeyDown(this, VK_RIGHT, 0, 0);	

					pDoc->m_PrevPoint.x = point.x;
				}
			}
			else if (pDoc->m_PrevPoint.x < point.x)
			{
				if (point.x - pDoc->m_PrevPoint.x > 3)
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
			if (pDoc->m_PrevPoint.y > point.y)
			{
				if (pDoc->m_PrevPoint.y - point.y > 5)
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
			else if (pDoc->m_PrevPoint.y < point.y)
			{
				if (point.y - pDoc->m_PrevPoint.y > 5)
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

void CEcsView::PreTranslateMessageSelectedLayout(CEcsDoc* pDoc, WPARAM wParam, int param3, int param4)
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