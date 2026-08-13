
// EcsView.cpp : CEcsView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Ecs.h"
#endif

#include "MainFrm.h"
#include "EcsDoc.h"
#include "EcsView.h"
#include "SystemLoginDlg.h"
#include "CvSkinDlg.h"
#include "RevSkinDlg.h"
#include "RollSkinDlg.h"
#include "ScSkinDlg.h"
#include "RtvSkinDlg.h"
#include "BCRSkinDlg.h"
#include "WcSkinDlg.h"
#include "ViewUsageRackDlg.h"
#include "FireMessageDlg.h"
#include "ViewJobListDlg.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEcsView
//
IMPLEMENT_DYNCREATE(CEcsView, CFormView)

BEGIN_MESSAGE_MAP(CEcsView, CFormView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SEARCH, &CEcsView::OnSearch)
	ON_MESSAGE(WM_USER_REFRESH_LAYOUT, &CEcsView::OnViewLayoutInfo)
	ON_BN_CLICKED(IDC_BTN_RESET1, OnBtnReset1)
	ON_BN_CLICKED(IDC_BTN_RESET8, OnBtnReset2)
	ON_BN_CLICKED(IDC_CHK_TEST_START1, OnChkTestStart1)
	ON_BN_CLICKED(IDC_CHK_TEST_START2, OnChkTestStart2)
	ON_BN_CLICKED(IDC_BTN_ALTER_LOC, OnBtnAlterLoc)
	ON_BN_CLICKED(IDC_BTN_RTR_ORDER, OnBtnRtrOrder)
	ON_BN_CLICKED(IDC_BTN_ATA_ORDER, OnBtnAtaOrder)
	ON_BN_CLICKED(IDC_BTN_SITE_ORDER, OnBtnSiteOrder)
	ON_MESSAGE(WM_USER_CONTROL_CLICK, &CEcsView::OnControlClick)
	ON_MESSAGE(WM_USER_REFRESH_DIALOG, &CEcsView::OnRefreshDialog)
	ON_BN_CLICKED(IDC_CHK_BCR_AUTO_UPDATE, &CEcsView::OnBnClickedChkBcrAutoUpdate)
	ON_BN_CLICKED(IDC_CHK_BCR_AUTO_UPDATE2, &CEcsView::OnBnClickedChkBcrAutoUpdate2)
END_MESSAGE_MAP()



CEcsView::CEcsView()
	: CFormView(CEcsView::IDD)
	, m_bIsBcrRead(FALSE)
{
	m_nSearchType = 0;
	m_bSearchFlag = FALSE;
	m_nSearchCount = 0;
	m_bTest = false;
}

CEcsView::~CEcsView()
{
}

void CEcsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAYOUT_TAB, m_tabLayout);
	DDX_Check(pDX, IDC_CHK_TEST_START1, m_bTesting1);
	DDX_Check(pDX, IDC_CHK_TEST_START2, m_bTesting2);
	DDX_Check(pDX, IDC_CHK_BCR_AUTO_UPDATE, m_bBcrAutoUpdate1);
	DDX_Check(pDX, IDC_CHK_BCR_AUTO_UPDATE2, m_bIsBcrRead);
}

BOOL CEcsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}
#define			ID_MAIN_TIMER		100

void CEcsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();

	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	//pDoc->m_layoutHUN_GF.OnInitialUpdate(this);
	//pDoc->m_layoutHUN_GFDECK.OnInitialUpdate(this);
	//pDoc->m_layoutHUN_1F.OnInitialUpdate(this);
	//pDoc->m_layoutHUN_1FDECK.OnInitialUpdate(this);
	//pDoc->m_layoutHUN_ASSEMBLE.OnInitialUpdate(this);

	//pDoc->m_pViewJobListDlg = new CViewJobListDlg(pDoc);
	//pDoc->m_pViewJobListDlg->Create(IDD_VIEW_JOBLIST);
	//pDoc->m_pViewJobListDlg->ShowWindow(SW_HIDE);//SW_HIDE

	int nMonitoringId = 35030 + pDoc->m_pConfig->m_nUSER_LAST_TAB_INDEX;
	pDoc->OnCommandRangeMainFrameMONITORING(nMonitoringId);
	CRect rect;
	int nMakeSizeX, nMakeSizeY, nWindowSizeX, nWindowSizeY;
	nMakeSizeX = MAKE_WINDOW_SIZE_X - 4;
	nMakeSizeY = MAKE_WINDOW_SIZE_Y - 200;

	GetDesktopWindow()->GetWindowRect(&rect);
	nWindowSizeX = rect.Width() - 4;
	nWindowSizeY = rect.Height() - 200;
	pDoc->m_pMaxSizeX = (double)nWindowSizeX/nMakeSizeX;
	pDoc->m_pMaxSizeY = (double)nWindowSizeY/nMakeSizeY;
	switch(pDoc->m_pConfig->m_nUSER_LAST_LANG)
	{
	case ID_LANGUAGE_KOR:
		{
			pDoc->m_enLang = EN_KOR;
			break;
		}
	case ID_LANGUAGE_ENGLISH:
		{
			pDoc->m_enLang = EN_ENG;
			break;
		}
	case ID_LANGUAGE_CHIN:
		{
			pDoc->m_enLang = EN_CHIN;
			break;
		}
	case ID_LANGUAGE_HUNGARIAN:
		{
			pDoc->m_enLang = EN_HUN;
			break;
		}    
	}

	// 레이아웃당 1개의 탭을 구성할 수 있다.  - 레이아웃에 여러대의 SC를 넣을 수 있다. 
	int i = 0;
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

		// 탭이 1개일때는 표시하지 않는다. 
	//	if (i==0)
	//		m_tabLayout.ShowWindow(SW_HIDE);
	}



	pDoc->UpdateRibbonLang();
	::SetTimer(this->m_hWnd, 1000, NULL, NULL);
	Invalidate(TRUE);
}

void CEcsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDI_NON_COMP, point.x, point.y, this, TRUE);
#endif
}


// CEcsView 진단
//
#ifdef _DEBUG
void CEcsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CEcsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEcsDoc* CEcsView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEcsDoc)));
	return (CEcsDoc*)m_pDocument;
}
#endif //_DEBUG

void CEcsView::GenerateStoRet(int nIndex, int nLuggNum)
{
	int nSelCnt = 0;
	CEcsDoc* pDoc = GetDocument();
	if (pDoc == NULL || nIndex < 0)
	{
		return;
	}

	SET_CHECK(IDC_CHK_BCR_AUTO_UPDATE2, pDoc->m_bReportedBCD[nIndex]);

	if (pDoc->m_bReportedBCD[nIndex] == TRUE && nLuggNum <= 0)
	{
		// 입고 작업 생성 - 작업중인 작업번호 SETTING 
		//BOOL CHostCl::JobOrder(int nJobType, CString strLocation, CString strStation, BOOL bManual)
		pDoc->m_nWorkingLuggNum1 = JobOrder(1);
	}
	else
	{
		SelectIF_LUGG_STA();

		//pDoc->m_bCompleteStore1 = TRUE;
		if (pDoc->m_bCompleteStore1 == TRUE)
		{
			// 출고 작업 생성 - 작업중인 작업번호 UPDATE 
			pDoc->m_nWorkingLuggNum1 = JobOrder(2);

			pDoc->m_bCompleteStore1 = FALSE;
		}
	}
	//pDoc->m_nWorkingLuggNum1 = 0;
	if (nLuggNum > 0)
		pDoc->m_bReportedBCD[nIndex] = FALSE;

}
// CEcsView 메시지 처리기
//
void CEcsView::OnTimer(UINT_PTR nIDEvent) 
{
	static int nCount = 1;	 
	int nSelCnt = 0;
	CEcsDoc* pDoc = GetDocument();
	if(pDoc == NULL)
	{
		CFormView::OnTimer(nIDEvent);
		return;
	}
 	if (nCount % 100 == 0) //HEART BEAT COLLECT DB
 	{
		if(pDoc->IsAlliveCollectDB() == FALSE)
		{
			pDoc->AlliveCollectDB(); //죽으나 마나 타이머에서 계속 생성시킬거임
		}

		//if(pDoc->IsAlliveCollectRequest() == FALSE)
		//{
		//	pDoc->AlliveCollectRequest(); //죽으나 마나 타이머에서 계속 생성시킬거임
		//}
 	}

	//HOST 상태확인을 위해 최초 1회만 탐
	if(pDoc->m_blConnectStatus == FALSE)
	{
		pDoc->EquipStatusCheck();
	}
	
	// 2초마다 할일
	if (nCount % 2 == 0)
	{
		// 설비 재접속
		//pDoc->ReconnectServer();
//		pDoc->m_strAlterLocation = "1000102";
//		pDoc->m_nPrevLuggNum = 1004;

		SET(IDC_RECEIVE_STATUS, pDoc->m_bReceiveStatus ? _T("OK") : _T("미수신"));

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
			//========================================================================================================================
			//  한국단자 1F 버전 - 출고작업 정의된 갯수만큼 생성, 입고대 준비되어 있을때 마구 출발해야하는 버전 
			//========================================================================================================================
			// 
			// 
			//------------------------------------------------------------------------------------------------------------------------

			//========================================================================================================================
			//  휴온스 버전 - 바코드 Reading ->  입고 작업 생성 -> 입고작업 완료 -> 출고작업생성 -> 출고작업 완료 -> 입고작업 생성	
			//  한국단자의 3F 버전으로 사용해도 될듯
			//========================================================================================================================
			/*
			SET_CHECK(IDC_CHK_BCR_AUTO_UPDATE2, pDoc->m_bReportedBCD[0]);

			if (pDoc->m_bReportedBCD[0] == TRUE && pDoc->m_nWorkingLuggNum1 <= 0)
			{
				// 입고 작업 생성 - 작업중인 작업번호 SETTING 
				//BOOL CHostCl::JobOrder(int nJobType, CString strLocation, CString strStation, BOOL bManual)
				pDoc->m_nWorkingLuggNum1 = JobOrder(1);
			}
			else 
			{
				SelectIF_LUGG_STA();

				//pDoc->m_bCompleteStore1 = TRUE;
				if (pDoc->m_bCompleteStore1 == TRUE)
				{
					// 출고 작업 생성 - 작업중인 작업번호 UPDATE 
					pDoc->m_nWorkingLuggNum1 = JobOrder(2);

					pDoc->m_bCompleteStore1 = FALSE;
				}
			}
			//pDoc->m_nWorkingLuggNum1 = 0;
			if (pDoc->m_nWorkingLuggNum1 > 0)
				pDoc->m_bReportedBCD[0] = FALSE;
			//*/
			//------------------------------------------------------------------------------------------------------------------------

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

			//========================================================================================================================
			//  휴온스 버전 - 바코드 Reading ->  입고 작업 생성 -> 입고작업 완료 -> 출고작업생성 -> 출고작업 완료 -> 입고작업 생성	
			//========================================================================================================================
			/*
			SET_INT(IDC_WORKING_LUGG1, pDoc->m_nWorkingLuggNum2);
			
			if (pDoc->m_bReportedBCD[1] == TRUE && pDoc->m_nWorkingLuggNum2 == 0)
			{
				// 입고 작업 생성 - 작업중인 작업번호 SETTING 
				//BOOL CHostCl::JobOrder(int nJobType, CString strLocation, CString strStation, BOOL bManual)
				pDoc->m_nWorkingLuggNum2 = JobOrder(1);
			}
			else 
			{
				SelectIF_LUGG_STA();

				//pDoc->m_bCompleteStore1 = TRUE;
				if (pDoc->m_bCompleteStore2 == TRUE)
				{
					// 출고 작업 생성 - 작업중인 작업번호 UPDATE 
					pDoc->m_nWorkingLuggNum2 = JobOrder(2);

					pDoc->m_bCompleteStore2 = FALSE;
				}
			}

			if (pDoc->m_nWorkingLuggNum2 > 0)
				pDoc->m_bReportedBCD[1] = FALSE;

			//*/
			//------------------------------------------------------------------------------------------------------------------------
		}
	}
	
	//TEXT 컨트롤 값에 따라서 보여주는 값 변화
	if (nCount % 100 == 0)
	{
		CDciControl* pDciControl = NULL; //보류4
		int nLayoutCnt = pDoc->m_pEcsLayOuts.GetSize();
	
		for(int nIdxLayout = 0; nIdxLayout < nLayoutCnt; nIdxLayout++)
		{
			//CTrackInfo* pTrackInfo101 = pDoc->GetTrackInfoNew(_T("101"));
			CTrackInfo* pTrackInfo149 = pDoc->GetTrackInfoNew(_T("148"));
			CTrackInfo* pTrackInfo154 = pDoc->GetTrackInfoNew(_T("154"));
	
			if(pTrackInfo149 == NULL || pTrackInfo154 == NULL){ break; };
			
			
			CString strSTOCK_MODE, strREMOTE_CONTROL, strROLL_MODE;
			GetQrySelectStatusAll(pTrackInfo149->m_pCV_DATA, strSTOCK_MODE, strREMOTE_CONTROL, strROLL_MODE);

			CString strSTOCK_MODE_CID, strREMOTE_CONTROL_CID, strROLL_MODE_CID;
	
			//strREMOTE_CONTROL_CID = _T("17111101");
			//pDciControl = pDoc->GetLayout_PARM((EN_LAYOUT)nIdxLayout)->GetDciControl(strREMOTE_CONTROL_CID);

			//if(pDciControl != NULL)
			//{
			//	if (strREMOTE_CONTROL == _T("0"))
			//	{
			//		pDciControl->m_strText = _T("리모컨 OFF");
			//		//pDciControl->m_clrFgColor = RED;
			//	}
			//	else
			//	{
			//		pDciControl->m_strText = _T("리모컨 ON");
			//	}

			//	
			//	pDciControl->InvalidateControl(pDoc->m_hWndView, FALSE);
			//}
	
			strSTOCK_MODE_CID = _T("17111149");
			pDciControl = pDoc->GetLayout_PARM((EN_LAYOUT)nIdxLayout)->GetDciControl(strSTOCK_MODE_CID);
	
			if(pDciControl != NULL)
			{
				if (strSTOCK_MODE == _T("0"))
				{
					pDciControl->m_strText = _T("출고모드");
				}
				else
				{
					pDciControl->m_strText = _T("입고모드");
				}
				pDciControl->InvalidateControl(pDoc->m_hWndView, FALSE);
			}
	
			strROLL_MODE_CID = _T("17111154");
			pDciControl = pDoc->GetLayout_PARM((EN_LAYOUT)nIdxLayout)->GetDciControl(strROLL_MODE_CID);
	
			if(pDciControl != NULL)
			{
				if (strROLL_MODE == _T("0"))
				{
					pDciControl->m_strText = _T("정상모드");
				}
				else
				{
					pDciControl->m_strText = _T("배출모드");
				}				
				pDciControl->InvalidateControl(pDoc->m_hWndView, FALSE);
			}
	
			continue;
		}
	}

	// 상태값 읽어오기
	ForCvSelectIF_MC_STA();
	ForScSelectIF_MC_STA();
	ForBcrSelectIF_MC_STA();

	nCount++;

	if(nCount >= 10000)
	{
		nCount = 0;
	}

	CFormView::OnTimer(nIDEvent);
}

void CEcsView::OnDestroy() 
{
	CFormView::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CEcsView printing

void CEcsView::OnDraw(CDC* pDC) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	CBitmap* pOldBitmap, bitmap;

	memDC.CreateCompatibleDC(pDC);

	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);

	DrawSelectedLayout(&memDC, pDoc);

	pDC->BitBlt(0,0, rect.Width(), rect.Height(), &memDC, 0,0,SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	CEcsLayout layout;
	int nTemp = -1;
	if (m_tabLayout.GetSafeHwnd())
	{
		nTemp = m_tabLayout.GetCurSel();
	}

	if (nTemp < 0)
		return;

	CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
	if (pEcsLayout == NULL)
		return;

	pEcsLayout->OnDraw(this, pDC, nTemp + 1);

}

BOOL CEcsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	WheelSelectedLayout(pDoc, nFlags, zDelta, pt);	
	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CEcsView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	RButtonUpSelectedLayout(pDoc);
	CFormView::OnRButtonUp(nFlags, point);
}

void CEcsView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	
//	MoveSearchWnd(cx, cy, pDoc->m_bOnSizeReady); 
//	OnSizeSelectedLayout(pDoc, nType, cx, cy);		// 원본

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
		for (int i = 0 ; i < nLayoutCnt ; i++)
		{
			if (nTemp == i)
			{
				// Tab 선택에 맞게 PLC 번호가 조정이 되어야 한다. 
				m_tabLayout.HighlightItem(nTemp, TRUE);	
				//SET_INT(IDC_COMBO_PLC, pEcsLayout->m_nRepresentPlc + 1);		// @@@
				//OnButton5();
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

// 	pDoc->m_bMoveFlag = TRUE;
// 	pDoc->m_PrevPoint = point;
// 	pDoc->m_tChecktime = CTime::GetCurrentTime();
	//OnLButtonDownSelectedLayout(pDoc, nFlags, point);		// 원본
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

//	pDoc->m_bMoveFlag = FALSE;
	//OnButtonUpSelectedLayout(pDoc, nFlags, point);
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

	//OnKeyDownSelectedLayout(pDoc, nChar, nRepCnt, nFlags);
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


LRESULT CEcsView::OnRefreshDialog(WPARAM wParam, LPARAM lParam)
{
	CEquipment::EN_KIND enEquipKind = CEquipment::EN_KIND(lParam);
	CEcsDoc* pDoc = GetDocument();
	if(pDoc == NULL)
	{
		return 0;
	}

	switch(enEquipKind)
	{
	case CEquipment::enCV:
		{

			if (!pDoc->Permission(_T("CCvSkinDlg"), SEL_YN))
			{
				AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
				return 0;
			}

			if (pDoc->m_pCvSkinDlg == NULL)
			{
				pDoc->m_pCvSkinDlg = new CCvSkinDlg(pDoc);
				pDoc->m_pCvSkinDlg->Create(IDD_SKIN_CV_CTRL1);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pCvSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_pCvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);
			}
			::SetWindowPos(pDoc->m_pCvSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pCvSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pCvSkinDlg->m_hWnd, SW_SHOW);
			::SendMessage(pDoc->m_pCvSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, NULL, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
			break;
		}
	case CEquipment::enSC:
		{

			if (!pDoc->Permission(_T("CScSkinDlg"), SEL_YN))
			{
				AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
				return 0;
			}

			if (pDoc->m_pScSkinDlg == NULL)
			{
				pDoc->m_pScSkinDlg = new CScSkinDlg(pDoc);
				pDoc->m_pScSkinDlg->Create(IDD_SKIN_SC_CTRL);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pScSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_pScSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

				if(pDoc->m_pScSkinDlg == NULL)
					break;
			}
			
			::SetWindowPos(pDoc->m_pScSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pScSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pScSkinDlg->m_hWnd, SW_SHOW);
			::SendMessage(pDoc->m_pScSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)NULL, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
		}
	case CEquipment::enRTV:
		{

			if (!pDoc->Permission(_T("CRTVSkinDlg"), SEL_YN))
			{
				AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
				return 0;
			}

			if (pDoc->m_pRtvSkinDlg == NULL)
			{
				pDoc->m_pRtvSkinDlg = new CRtvSkinDlg(pDoc);
				pDoc->m_pRtvSkinDlg->Create(IDD_SKIN_RTV_CTRL);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pRtvSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_pRtvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

				if(pDoc->m_pRtvSkinDlg == NULL)
					break;
			}
			
			::SetWindowPos(pDoc->m_pRtvSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pRtvSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pRtvSkinDlg->m_hWnd, SW_SHOW);
			::SendMessage(pDoc->m_pRtvSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)NULL, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
		}
	case CEquipment::enBCR:
		{

			if (!pDoc->Permission(_T("CBCRSkinDlg"), SEL_YN))
			{
				AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
				return 0;
			}

			if (pDoc->m_pBcrSkinDlg == NULL)
			{
				pDoc->m_pBcrSkinDlg = new CBCRSkinDlg(pDoc);
				pDoc->m_pBcrSkinDlg->Create(IDD_SKIN_RTV_CTRL);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pBcrSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_pRtvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

				if(pDoc->m_pBcrSkinDlg == NULL)
					break;
			}
			
			::SetWindowPos(pDoc->m_pBcrSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pBcrSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pBcrSkinDlg->m_hWnd, SW_SHOW);
			::SendMessage(pDoc->m_pBcrSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)NULL, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
		}
	case CEquipment::enWC:
		{

			if (!pDoc->Permission(_T("CWcSkinDlg"), SEL_YN))
			{
				AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
				return 0;
			}

			if (pDoc->m_pWcSkinDlg == NULL)
			{
				pDoc->m_pWcSkinDlg = new CWcSkinDlg(pDoc);
				pDoc->m_pWcSkinDlg->Create(IDD_SKIN_WC_CTRL);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pWcSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_pWcSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

				if(pDoc->m_pWcSkinDlg == NULL)
					break;
			}
			
			::SetWindowPos(pDoc->m_pWcSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pWcSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pWcSkinDlg->m_hWnd, SW_SHOW);
			::SendMessage(pDoc->m_pWcSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)NULL, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
		}
	}
	return 0;
}

LRESULT CEcsView::OnControlClick(WPARAM wParam, LPARAM lParam)
{
	CEcsDoc* pDoc = GetDocument();	
	if(pDoc == NULL){	return FALSE;	}

	CString strCID;
	CString strMC_NO;
	strCID.Format(_T("%u"), wParam);
	if (CLib::IsValidControlID(strCID) == FALSE)
		return 0;

	//if (CLib::IsValidControlMCNO(strMC_NO) == FALSE)
	//	return 0;

	CString strEqpKey, strWH_TYP, strEN_EQP_TYP; 
	int nEN_EQP_TYP;
	int nIntTemp = 0;

	//strEqpKey = strCID.Right(5);
	strEqpKey = strCID.Right(3);
	//strEqpKey = strMC_NO;
	strWH_TYP = pDoc->m_WH_TYP;
	strEN_EQP_TYP = strCID.Mid(3, 2);
	nIntTemp = CConvert::ToInt(strEN_EQP_TYP);
	nEN_EQP_TYP = nIntTemp; //(nIntTemp / 10) * 10;


	switch (nEN_EQP_TYP)
	{
	case CEquipment::enCV:
		{			
			if (pDoc->m_strSeachTrack == strEqpKey)
			{
				CString strTrack;
				strTrack = pDoc->m_strSeachTrack;
				CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(strTrack);
				pTrackInfo->SetSeachColor(pTrackInfo->m_pTrackCtrl, true);
				pDoc->m_strSeachTrack = _T("");
			}

			CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(strEqpKey);
			if(pTrackInfo == NULL){ return 0; };
			if (pDoc->m_pCvSkinDlg == NULL)
			{

				if (!pDoc->Permission(_T("CCvSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
					return 0;
				}

				pDoc->m_pCvSkinDlg = new CCvSkinDlg(pDoc);
				pDoc->m_pCvSkinDlg->Create(IDD_SKIN_CV_CTRL1);


				//pDoc->m_pCvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
				//	Rect.Width(), Rect.Height(), 
				//	SWP_SHOWWINDOW);


			}

			CRect MainRect;
			CRect Rect;
			CRect PosRect;
			::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
			pDoc->m_pCvSkinDlg->GetWindowRect(&Rect); 

			PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
			PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 

			pDoc->m_pCvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
				Rect.Width(), Rect.Height(), 
				SWP_SHOWWINDOW);

			::SetWindowPos(pDoc->m_pCvSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pCvSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pCvSkinDlg->m_hWnd, SW_SHOWNORMAL);
			::SendMessage(pDoc->m_pCvSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pTrackInfo, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람

			
			break;
		}

	case CEquipment::enSC:
		{
			CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(strEqpKey);
			if(pSC_DATA == NULL){ return 0; };

			if (pDoc->m_pScSkinDlg == NULL)
			{
				if (!pDoc->Permission(_T("CScSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->m_pLang->GetLangValue(_T("권한이 없습니다"), pDoc->m_enLang));
					return 0;
				}

				pDoc->m_pScSkinDlg = new CScSkinDlg(pDoc);
				pDoc->m_pScSkinDlg->Create(IDD_SKIN_SC_CTRL);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pScSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_pScSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);
			}
			::SetWindowPos(pDoc->m_pScSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pScSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pScSkinDlg->m_hWnd, SW_SHOWNORMAL); 
			::SendMessage(pDoc->m_pScSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pSC_DATA, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
			break;
		}

 
 	case CEquipment::enRTV:
 		{
			CRTV_DATA* pRTV_DATA = pDoc->GetRTV_DATA(strEqpKey);
			if(pRTV_DATA == NULL){ return 0; };

			if (pDoc->m_pRtvSkinDlg == NULL)
			{
				if (!pDoc->Permission(_T("CRtvSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->m_pLang->GetLangValue(_T("권한이 없습니다"), pDoc->m_enLang));
					return 0;
				}

				pDoc->m_pRtvSkinDlg = new CRtvSkinDlg(pDoc);
				pDoc->m_pRtvSkinDlg->Create(IDD_SKIN_RTV_CTRL);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pRtvSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				pDoc->m_pRtvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);
			}
			::SetWindowPos(pDoc->m_pRtvSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pRtvSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pRtvSkinDlg->m_hWnd, SW_SHOWNORMAL); 
			::SendMessage(pDoc->m_pRtvSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pRTV_DATA, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
			break;
		}
 
 	case CEquipment::enBCR:
 		{
			CBCR_MST* pBCR_MST = pDoc->GetBCR_MST(strEqpKey);
			if(pBCR_MST == NULL){ return 0; };

 			if (pDoc->m_pBcrSkinDlg == NULL)
			{
				if (!pDoc->Permission(_T("CBCRSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->m_pLang->GetLangValue(_T("권한이 없습니다"), pDoc->m_enLang));
					return 0;
				}

 				pDoc->m_pBcrSkinDlg = new CBCRSkinDlg(pDoc);
 				pDoc->m_pBcrSkinDlg->Create(IDD_SKIN_BCR_CRTL);
 				CRect MainRect;
 				CRect Rect;
 				CRect PosRect;
 				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
 				pDoc->m_pBcrSkinDlg->GetWindowRect(&Rect); 
 
 				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
 				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
 				pDoc->m_pBcrSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
 					Rect.Width(), Rect.Height(), 
 					SWP_SHOWWINDOW);
 			}
			::SetWindowPos(pDoc->m_pBcrSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pBcrSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pBcrSkinDlg->m_hWnd, SW_SHOWNORMAL); 
			::SendMessage(pDoc->m_pBcrSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pBCR_MST, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람


			break;
		}
		case CEquipment::enWC:
 		{
			CWC_DATA* pWC_DATA = pDoc->GetWC_DATA(strEqpKey);
			if(pWC_DATA == NULL){ return 0; };

 			if (pDoc->m_pWcSkinDlg == NULL)
			{
				if (!pDoc->Permission(_T("CWcSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->m_pLang->GetLangValue(_T("권한이 없습니다"), pDoc->m_enLang));
					return 0;
				}

 				pDoc->m_pWcSkinDlg = new CWcSkinDlg(pDoc);
 				pDoc->m_pWcSkinDlg->Create(IDD_SKIN_WC_CTRL);
 				CRect MainRect;
 				CRect Rect;
 				CRect PosRect;
 				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
 				pDoc->m_pWcSkinDlg->GetWindowRect(&Rect); 
 
 				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
 				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
 				pDoc->m_pWcSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
 					Rect.Width(), Rect.Height(), 
 					SWP_SHOWWINDOW);
 			}
			::SetWindowPos(pDoc->m_pWcSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pWcSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pWcSkinDlg->m_hWnd, SW_SHOWNORMAL); 
			::SendMessage(pDoc->m_pWcSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pWC_DATA, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
			break;
		}
		case CEquipment::enREV:
		{			
			CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(strEqpKey);
			if(pTrackInfo == NULL){ return 0; };
			if (pDoc->m_pRevSkinDlg == NULL)
			{

				if (!pDoc->Permission(_T("CRevSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
					return 0;
				}

				pDoc->m_pRevSkinDlg = new CRevSkinDlg(pDoc);
				pDoc->m_pRevSkinDlg->Create(IDD_SKIN_REV_CTRL);

			}

			CRect MainRect;
			CRect Rect;
			CRect PosRect;
			::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
			pDoc->m_pRevSkinDlg->GetWindowRect(&Rect); 

			PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
			PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 

			pDoc->m_pRevSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
				Rect.Width(), Rect.Height(), 
				SWP_SHOWWINDOW);

			::SetWindowPos(pDoc->m_pRevSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pRevSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pRevSkinDlg->m_hWnd, SW_SHOWNORMAL);
			::SendMessage(pDoc->m_pRevSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pTrackInfo, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람

			
			break;
		}
		case CEquipment::enROLL:
		{			
			CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(strEqpKey);
			if(pTrackInfo == NULL){ return 0; };
			if (pDoc->m_pRollSkinDlg == NULL)
			{

				if (!pDoc->Permission(_T("CRollSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
					return 0;
				}

				pDoc->m_pRollSkinDlg = new CRollSkinDlg(pDoc);
				pDoc->m_pRollSkinDlg->Create(IDD_SKIN_ROLL_CTRL);

			}

			CRect MainRect;
			CRect Rect;
			CRect PosRect;
			::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
			pDoc->m_pRollSkinDlg->GetWindowRect(&Rect); 

			PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
			PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 

			pDoc->m_pRollSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
				Rect.Width(), Rect.Height(), 
				SWP_SHOWWINDOW);

			::SetWindowPos(pDoc->m_pRollSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pRollSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pRollSkinDlg->m_hWnd, SW_SHOWNORMAL);
			::SendMessage(pDoc->m_pRollSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pTrackInfo, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람

			
			break;
		}
	}
	return 0;
		
}

LRESULT CEcsView::OnViewLayoutInfo(WPARAM wParam, LPARAM lParam)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return 0;

	RECT rc;
	GetClientRect(&rc);
	SendMessage(WM_SIZE,(WPARAM)SIZE_RESTORED, MAKELPARAM(rc.right-rc.left, rc.bottom-rc.top));

	//pLayout->OnViewLayoutInfo(this);
	
	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();

		if (nTemp < 0)
			return 0;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return 0;

		pEcsLayout->OnViewLayoutInfo(this);
	}

	pDoc->m_bViewFirstLoad = TRUE;
	return 0;
}

//=============================================================================
//	Desc	: '검색기능' 창을 화면 조정에 따라 우측 하단에 배치
//	Author	: YJY
//	Date	:
//	Update	: 
//=============================================================================
void CEcsView::MoveSearchWnd(int cx, int cy, BOOL bStatus)
{
	// 호출 시점 : 프로그램 시작 시, 윈도우 크기 변경 시, LayOut 1-3단 클릭 시
// 	CRect rcRect;
// 	CEcsDoc *pDoc = GetDocument();
// 	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
// 	if(pDoc == NULL || pLayout == NULL)
// 		return;
// 
// 	if (FALSE == bStatus || NULL == pDoc) 
// 		return; 
// 
// 	// ## 위치 조정 시 ## 이 변수 2개만 변경하세요.
// 	int nWOffset = 185;		// 값이 크면 왼쪽으로~
// 	int nHOffset = 100;		// 값이 크면 위쪽으로~
// 
// 	// SET SCALE FACTOR
// 	int nScale;
// 	CRect rc = pLayout->GetDciMaster()->GetLayoutL();
// 
// 	if ((rc.right / rc.top) > (cx / cy))
// 		nScale = cx / rc.right;
// 	else 
// 		nScale = cy / rc.top;
// 
// 	while ((cx < nScale * rc.right) || (cy < nScale * rc.top))
// 		--nScale;
// 
// 	nScale = (nScale > 0) ? nScale : 1;
// 
// 	// LayOut에 맞춘 오프셋 구하기
// 	int nLayOutWOffset = abs(cx - nScale * rc.right) / 2;
// 	int nLayOutHOffset = cy - abs(cy - nScale * rc.top) / 2;
// 
// 	// 컨트롤 배치 위치
// 	int nPosX =  cx - nWOffset - nLayOutWOffset; 
// 	int nPosY =  nLayOutHOffset - nHOffset;
// 
// 	for (int i = 0; i < 7; i++)
// 	{
// 		rcRect = pDoc->m_rcSerachCtrl[i];
// 		GetDlgItem(pDoc->m_rgCtrlID[i])->SetWindowPos(NULL, nPosX + rcRect.left, nPosY + rcRect.top, 0, 0, SWP_NOSIZE);
// 	}
}

//=============================================================================
//	Desc	: '검색기능' 컨트롤 ID 와 위치 저장
//	Author	: YJY
//	Date	:
//	Update	: 
//=============================================================================
void CEcsView::CalcSerachControlPos()
{
	CEcsDoc *pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTop = 0;
	int nLeft = 0;

	// 컨트롤 아이디 배열 저장
// 	pDoc->m_rgCtrlID[0] = IDC_STC_SEARCH;
// 	pDoc->m_rgCtrlID[1] = IDC_CHECK_LUGGNUM_SEARCH;
// 	pDoc->m_rgCtrlID[2] = IDC_CHECK_TRAYID_SEARCH;
// 	pDoc->m_rgCtrlID[3] = IDC_CHECK_STATION_SEARCH;
// 	pDoc->m_rgCtrlID[4] = IDC_CHECK_TRACKNUM_SEARCH;
// 	pDoc->m_rgCtrlID[5] = IDC_EDIT_LUGGNUM_SEARCH;
// 	pDoc->m_rgCtrlID[6] = IDC_BUTTON_TRACK_SEARCH;
// 
// 	// 재실행 할때마다 Control Position 값이 바뀌는데 이유를 모르겠음. 프레임->Recalclayout()과 관계 되는지?
// 	int i = 0;
// 	for (i = 0; i < 7; i++)
// 	{
// 		GetDlgItem(pDoc->m_rgCtrlID[i])->GetWindowRect(&pDoc->m_rcSerachCtrl[i]);
// 		
// 		if ((pDoc->m_rcSerachCtrl[i].left < nLeft) || (0 == nLeft))
// 			nLeft = pDoc->m_rcSerachCtrl[i].left;
// 
// 		if ((pDoc->m_rcSerachCtrl[i].top < nTop) || (0 == nTop))
// 			nTop = pDoc->m_rcSerachCtrl[i].top;
// 	}
// 
// 	// 위치값이 실행시 마다 바뀌므로, 가장 작은값 기준으로 계산.
// 	for (i = 0; i < 7; i++)
// 	{
// 		pDoc->m_rcSerachCtrl[i].left = pDoc->m_rcSerachCtrl[i].left - nLeft;
// 		pDoc->m_rcSerachCtrl[i].top = pDoc->m_rcSerachCtrl[i].top - nTop;
// 	}
}

BOOL CEcsView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CEcsDoc *pDoc = (CEcsDoc*)GetDocument();
	ASSERT(pDoc != NULL);

	if (pMsg->message == WM_KEYDOWN) 
	{
		PreTranslateMessageSelectedLayout(pDoc, pMsg->wParam, 0, 0);
	}

	return CFormView::PreTranslateMessage(pMsg);
}

int CEcsView::AlterLocation(BOOL bManual)
{
	CEcsDoc* pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return -1;
	}

	int nUserData = 0;
	CString strLog, strTempMsg, strLocation;
	
	//memset(m_AlterLocMsg, 0x0, sizeof(m_AlterLocMsg));

	int nLuggNum = pDoc->m_nPrevLuggNum;

	CString strJobType = _T("");

	strLocation = pDoc->m_strLocation;

	//nLuggNum = 8583;
	nLuggNum = GET_INT(IDC_DUAL_STO_LUGG);

	if (bManual == TRUE)
		strLocation = pDoc->m_strAlterLocation;

	if (UpdateIF_LUGG_STA(CConvert::ToString(nLuggNum)	// pLUGGNO
		, strLocation									// pTO_AREA = _T("")
		, _T("98")										// pWORK_STA = _T("00")
		, _T("N")										// pIF_STATUS = _T("N")
		, _T("00")										// pERRCODE	= _T("00")
		, _T("100")										// pPRIORITY = _T("100")
		, FALSE											// bPopUpMsgDlg	= FALSE
	) == TRUE)
	{
		pDoc->m_nPrevLuggNum = nLuggNum;
	}
		 
	//m_pDoc->m_nPrevLuggNum = nLuggNum;

	//m_nAlterLocSize = strTempMsg.GetLength() + 3;
	//
	//sprintf(m_AlterLocMsg,						"WMS_MBX   %04d ",	m_nAlterLocSize);
	//sprintf(m_AlterLocMsg + MSG_LENGTH_HEADER, "%c%c%s%c",	STX, CMD_ALT_LOC, strTempMsg, ETX);
	//
	//int nLenth =  sizeof(m_AlterLocMsg);
	
	// 실제로 보내는거 해야함!
	//if(	SendMsg(m_AlterLocMsg, m_nAlterLocSize + MSG_LENGTH_HEADER) == TRUE )
	//{
	//	strLog.Format(_T("이중입고 재지정 %s 지시.. Location=[%s], ScNo=[%d]"), (bManual) ? _T("수동") : _T(""), strLocation, nScNo);
	//	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::AlterLocation"));
	//}
	//else
	//{
	//	strLog.Format(_T("이중입고 재지정 %s 지시 실패.. Location=[%s], ScNo=[%d]"), (bManual) ? _T("수동") : _T(""), strLocation, nScNo);
	//	m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::AlterLocation"));
	//	return 0;
	//}
	
	return nLuggNum;
}

BOOL CEcsView::CalcurateLocation(BOOL bSto)
{
	CEcsDoc* pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return FALSE;
	}

	BOOL bCurrentScOK = FALSE;
	BOOL bNextScOK = FALSE;

	int nCurrentScMinBank = 0;
	int nNextScMinBank = 0;
	
	int nCurrentScMaxBank = 0;
	int nNextScMaxBank = 0;
	
	//m_pDoc->m_strLocation = "";

	int nPrevBank = 0;
	if (pDoc->m_strLocation.GetLength() != 7)
	{
		nPrevBank	= 1;
		m_nCurBay	= 1;
		m_nCurLevel	= 1;
		//return FALSE;
	}
	else
	{
		nPrevBank	= _ttoi(pDoc->m_strLocation.Mid(0,2));
		m_nCurBay	= _ttoi(pDoc->m_strLocation.Mid(2,3));
		m_nCurLevel	= _ttoi(pDoc->m_strLocation.Mid(5,2));
	}

	if (nPrevBank == 10)
		int aaaaaaa = 0;

//	if (nPrevBank == 7)
//		nPrevBank = 10;


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

	CStringList strUseAbleBanks;
	CString strBank;
	int nCurBank = 0;
	// 작업 가능한 SC 호기를 체크한다. 
	for (int i = 0 ; i < SC_CNT ; i++)
	{
		// SC 호기 구하기 
		int nScNum = i + 1;
		int nStatus = pDoc->m_nScStatus[i];

		if (nStatus == 4 || nStatus == 5 || nStatus == 8)
		{
			continue;
		}

		if (bSto == TRUE)
		{
			if (nStatus == 6)
			{
				continue;
			}
		}
		else
		{
			if (nStatus == 7)
			{
				continue;
			}
		}

		// 호기에 맞는 Bank 값 구하기 
		int nMinBank = (nScNum * 2) - 1;
		int nMaxBank = (nScNum * 2);
		
		strBank = CConvert::ToString(nMinBank);		strUseAbleBanks.AddTail(strBank);
		strBank = CConvert::ToString(nMaxBank);		strUseAbleBanks.AddTail(strBank);
	}
	
	if (strUseAbleBanks.GetCount() == 0)
		return FALSE;

	int nUseable1stBank = _ttoi(strUseAbleBanks.GetHead());
	for (POSITION pos=strUseAbleBanks.GetHeadPosition(); pos!=NULL; )
	{
		CString& strUseableBank = strUseAbleBanks.GetNext(pos);
		int nLen = strUseableBank.GetLength();		///
		
		int nUseableBank = _ttoi(strUseableBank);

		if (nPrevBank < nUseableBank)
		{
			nCurBank = nUseableBank;
			break;
		}
	}

	if (nCurBank == 0)
	{
		nCurBank = nUseable1stBank;

		++m_nCurBay;
		if (pDoc->m_nMaxBay < m_nCurBay )
		{
			m_nCurBay = 1;
			++m_nCurLevel;

			if (pDoc->m_nMaxLevel < m_nCurLevel)
			{
				//m_nCurBank = 1;
				m_nCurBay = 1;
				m_nCurLevel = 1;
			}
		}
	}

	// 이번에 지시할 Location 저장 
	pDoc->m_strLocation.Format(_T("%02d%03d%02d"), nCurBank, m_nCurBay, m_nCurLevel);
	return TRUE;
}

int CEcsView::JobOrder(int nJobType, BOOL bManual)
{
	CEcsDoc* pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return -1;
	}

	int nUserData = 0;
	int nIndex = 0;
	CString strLog, strTempMsg, strProductId, strStation, strPltName, strFromHsTrNo, strToHsTrNo;
	CString strHS_MC_NO, strSENSOR0_DATA_RD;
	
	//memset(m_JobOrderMsg, 0x0, sizeof(m_JobOrderMsg));


	//static int nLuggNum = 1000;

	//nLuggNum++;

	//if (nLuggNum > 8999 || nLuggNum < 1000)
	//	nLuggNum = 1000;


	// 작업 번호 생성하기
	int nRowCnt;
	CString strSql, strMessage, strNextVal;
	strSql.Format(_T("  SELECT NEXTVAL('ONLINE_LUGGNO') "));

	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
		return FALSE;

	pRsw->MoveFirst();
	//seq에서 작업에 사용할 번호 가져오기
	strNextVal = pRsw->GetItem(_T("nextval"));

	int nLuggNum = _tstoi(strNextVal);

	//strNextVal.Format(_T("%d"), nTemp);

	CString strJobType = _T("");
	int nScNum  = 0;
	int nScNum1 = 0;
	int nScNum2 = 0;

	switch (nJobType)
	{
	case 1:		//			   1 2   3 4 5  6  7  8  9  10 11 
		if (CalcurateLocation() == FALSE)
			return 0;

		// 첫 상태를 받지 않았을때 
		if (pDoc->m_bReceiveStatus == FALSE)
			return 0;

		strStation = pDoc->m_strStoStation;

		strJobType = _T("입고");

		strProductId = _T("입고하는 제품 정보임!");
		nScNum = CLib::GetStackerNum(1, pDoc->m_strLocation, TRUE);

		if (strStation == _T("102"))
			nIndex = 0;
		else //if (strStation == _T("105"))
			nIndex = 1;

		strPltName = pDoc->m_strStoBcrData[nIndex];

		strHS_MC_NO = _T("0");
		if (!CLib::HsDataSelect(pDoc, TRUE, _T("90") + CConvert::ToString(nScNum), strHS_MC_NO, strSENSOR0_DATA_RD))
		{
			return 0;
		}

		if (InsertIF_LUGG_STA(CConvert::ToString(nLuggNum)	// pLUGGNO
			, CConvert::ToString(nJobType)					// pJOB_KIND
			, strPltName									// pLD_CDN_NO		// 입고 PLT 번호
			, strStation									// pFROM_CV_NO		// 출발 작업대
			, strHS_MC_NO									// pTO_CV_NO		// 입고 HS
			, _T("")										// pFROM_SC_NO		// 
			, _T("90") + CConvert::ToString(nScNum)					// pTO_SC_NO		// 도착 SC
			, _T("")										// pFROM_AREA		// 
			, pDoc->m_strLocation							// pTO_AREA			// 도착 CELL
			, _T("00")										// pWORK_STA
			, CConvert::ToString(0)							// pISHIGH
			, CConvert::ToString(0)							// pERRCODE
			, CConvert::ToString(100)						// pPRIORITY
			, strProductId									// pPRDCT_NM
		) == TRUE)
		{
			pDoc->m_nPrevLuggNum = nLuggNum;
			//pDoc->m_nWorkingLuggNum1 = nLuggNum;
		}

		break;
	case 2:		
	case 3:		
		strStation = pDoc->m_strRetStation;

		strJobType = _T("출고");
		strProductId = _T("출고하는 제품 정보임!");

		strPltName = (nJobType == 2) ? "출고 PLT 번호" : "PICKING PLT 번호";

		nScNum = CLib::GetStackerNum(1, pDoc->m_strLocation, TRUE);

		strHS_MC_NO = _T("0");
		if (!CLib::HsDataSelect(pDoc, FALSE, _T("90") + CConvert::ToString(nScNum), strHS_MC_NO, strSENSOR0_DATA_RD))
		{
			return 0;
		}


		if (InsertIF_LUGG_STA(CConvert::ToString(nLuggNum)	// pLUGGNO
			, CConvert::ToString(nJobType)					// pJOB_KIND
			, strPltName									// pLD_CDN_NO		// 출고 PLT 번호
			, strHS_MC_NO									// pFROM_CV_NO		// 출발 HS
			, strStation									// pTO_CV_NO		// 도착 CV
			, _T("90") + CConvert::ToString(nScNum)					// pFROM_SC_NO		// 출발 SC
			, _T("")										// pTO_SC_NO		// 
			, pDoc->m_strLocation							// pFROM_AREA		// 출발 CELL
			, _T("")										// pTO_AREA			// 
			, _T("00")										// pWORK_STA
			, CConvert::ToString(0)							// pISHIGH
			, CConvert::ToString(0)							// pERRCODE
			, CConvert::ToString(100)						// pPRIORITY
			, strProductId									// pPRDCT_NM
		) == TRUE)
		{
			pDoc->m_nPrevLuggNum = nLuggNum;
			//pDoc->m_nWorkingLuggNum1 = nLuggNum;
		}
		break;
	case 4:		
	case 5:		
		switch(nJobType)
		{
		case 4:		strJobType = _T("랙투랙");			strPltName = "RACK PLT 번호";		break;
		case 5:		strJobType = _T("호기간 이동");		strPltName = "호기간 PLT 번호";		break;
		}

		strProductId = strJobType + _T("하는 제품 정보임!");

		nScNum1 = CLib::GetStackerNum(1, pDoc->m_strFromPos, TRUE);
		nScNum2 = CLib::GetStackerNum(1, pDoc->m_strToPos, TRUE);

		//strFromHsTrNo, strToHsTrNo;
		strHS_MC_NO = _T("0");
		CLib::HsDataSelect(pDoc, FALSE, _T("90") + CConvert::ToString(nScNum1), strHS_MC_NO, strSENSOR0_DATA_RD);		strFromHsTrNo = strHS_MC_NO;
		CLib::HsDataSelect(pDoc, FALSE, _T("90") + CConvert::ToString(nScNum2), strHS_MC_NO, strSENSOR0_DATA_RD);		strToHsTrNo = strHS_MC_NO;

		if (InsertIF_LUGG_STA(CConvert::ToString(nLuggNum)	// pLUGGNO
			, CConvert::ToString(nJobType)					// pJOB_KIND
			, strPltName									// pLD_CDN_NO		// 출고 PLT 번호
			, (nJobType == 4) ? _T("") : strFromHsTrNo		// pFROM_CV_NO		// 출발 HS
			, (nJobType == 4) ? _T("") : strToHsTrNo		// pTO_CV_NO		// 도착 CV
			, _T("90") + CConvert::ToString(nScNum1)					// pFROM_SC_NO		// 출발 SC
			, _T("90") + CConvert::ToString(nScNum2)					// pTO_SC_NO		// 도착 SC
			, pDoc->m_strFromPos							// pFROM_AREA		// 출발 CELL
			, pDoc->m_strToPos								// pTO_AREA			// 도착 CELL
			, _T("00")										// pWORK_STA
			, CConvert::ToString(0)							// pISHIGH
			, CConvert::ToString(0)							// pERRCODE
			, CConvert::ToString(100)						// pPRIORITY
			, strProductId									// pPRDCT_NM
		) == TRUE)
		{
			pDoc->m_nPrevLuggNum = nLuggNum;
			//pDoc->m_nWorkingLuggNum1 = nLuggNum;
		}
		break;
	case 6:		
		strJobType = _T("이동");

		strPltName = "이동 PLT 번호";
		strProductId = _T("이동하는 제품 정보임!");

		nScNum = CLib::GetStackerNum(1, pDoc->m_strLocation, TRUE);

		strHS_MC_NO = _T("0");
		CLib::HsDataSelect(pDoc, TRUE, _T("90") + CConvert::ToString(nScNum), strHS_MC_NO, strSENSOR0_DATA_RD);

		if (InsertIF_LUGG_STA(CConvert::ToString(nLuggNum)	// pLUGGNO
			, CConvert::ToString(nJobType)					// pJOB_KIND
			, strPltName									// pLD_CDN_NO		// 입고 PLT 번호
			, pDoc->m_strFromPos							// pFROM_CV_NO		// 출발 작업대
			, pDoc->m_strToPos								// pTO_CV_NO		// 도착 작업대
			, _T("")										// pFROM_SC_NO		// 
			, _T("")										// pTO_SC_NO		// 
			, _T("")										// pFROM_AREA		// 
			, _T("")										// pTO_AREA			// 
			, _T("00")										// pWORK_STA
			, CConvert::ToString(0)							// pISHIGH
			, CConvert::ToString(0)							// pERRCODE
			, CConvert::ToString(100)						// pPRIORITY
			, strProductId									// pPRDCT_NM
		) == TRUE)
		{
			pDoc->m_nPrevLuggNum = nLuggNum;
			//pDoc->m_nWorkingLuggNum1 = nLuggNum;
		}
		break;
	}

	//pDoc->m_nPrevLuggNum = nLuggNum;

	//m_nJobOrderSize = strTempMsg.GetLength() + 3;
	//
	//sprintf(m_JobOrderMsg,						"WMS_MBX   %04d ",	m_nJobOrderSize);
	//sprintf(m_JobOrderMsg + MSG_LENGTH_HEADER, "%c%c%s%c",	STX, CMD_ORDER, strTempMsg, ETX);
	//
	//int nLenth =  sizeof(m_JobOrderMsg);
	
	// 실제로 보내는거 해야 함!
	//if(	SendMsg(m_JobOrderMsg, m_nJobOrderSize + MSG_LENGTH_HEADER) == TRUE )
	//{
	//	strLog.Format(_T("신규 %s 작업 %s 지시.. Location=[%s], Station=[%s]"), strJobType, (bManual) ? _T("수동") : _T(""), m_pDoc->m_strLocation, strStation);
	//	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::JobOrder"));
	//}
	//else
	//{
	//	strLog.Format(_T("신규 %s 작업 %s 지시 실패.. Location=[%s], Station=[%s]"), strJobType, (bManual) ? _T("수동") : _T(""), m_pDoc->m_strLocation, strStation);
	//	m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::JobOrder"));
	//	return 0;
	//}
	
	return nLuggNum;
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

	if (pDoc != NULL /*&& pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()*/) 
	{
		GET(IDC_DUAL_STO_LOC, pDoc->m_strAlterLocation);

		if (pDoc->m_strAlterLocation.GetLength() != 7)
		{
			AfxMessageBox(_T("이중입고 재지정 작업 지시\nLocation을 바르게 입력(7자리)해주세요!"));
			return;
		}

		// 재작업 지시 
		if ((pDoc->m_nWorkingLuggNum1 = AlterLocation(TRUE)) != 0)
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

	if (pDoc != NULL /*&& pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()*/) 
	{
		GET(IDC_EDT_FROM_LOC, pDoc->m_strFromPos);
		GET(IDC_EDT_TO_LOC, pDoc->m_strToPos);

		if (pDoc->m_strFromPos.GetLength() != 7 || pDoc->m_strToPos.GetLength() != 7 )
		{
			AfxMessageBox(_T("랙투랙 작업 지시\nLocation을 바르게 입력(7자리)해주세요!"));
			return; 
		}

		// 랙투랙 작업 지시 
		if ((pDoc->m_nWorkingLuggNum1 = JobOrder(4)) != 0)
		{
			CString strTemp = _T("");
			GET(IDC_EDT_FROM_LOC, strTemp);
			GET(IDC_EDT_TO_LOC, strTemp);
		}
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

	if (pDoc != NULL /*&& pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()*/) 
	{
		GET(IDC_EDT_FROM_LOC, pDoc->m_strFromPos);
		GET(IDC_EDT_TO_LOC, pDoc->m_strToPos);

		if (pDoc->m_strFromPos.GetLength() != 7 || pDoc->m_strToPos.GetLength() != 7 )
		{
			AfxMessageBox(_T("호기간 이동 지시\nLocation을 바르게 입력(7자리)해주세요!"));
			return; 
		}

		// 호기간 이동 작업 지시 
		if ((pDoc->m_nWorkingLuggNum1 = JobOrder(5)) != 0)
		{
			CString strTemp = _T("");
			GET(IDC_EDT_FROM_LOC, strTemp);
			GET(IDC_EDT_TO_LOC, strTemp);
		}
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

	if (pDoc != NULL /*&& pDoc->m_pHostCl != NULL && pDoc->m_pHostCl->IsConnect()*/) 
	{
		GET(IDC_EDT_FROM_LOC, pDoc->m_strFromPos);
		GET(IDC_EDT_TO_LOC, pDoc->m_strToPos);

		if (pDoc->m_strFromPos.GetLength() != 3 || pDoc->m_strToPos.GetLength() != 3 )
		{
			AfxMessageBox(_T("이동지시\nStation을 바르게 입력(3자리)해주세요!"));
			return; 
		}

		// 이동 작업 지시 
		if ((pDoc->m_nWorkingLuggNum1 = JobOrder(6)) != 0)
		{
			CString strTemp = _T("");
			GET(IDC_EDT_FROM_LOC, strTemp);
			GET(IDC_EDT_TO_LOC, strTemp);
		}
	}		
	else
	{
		AfxMessageBox(_T("Host가 연결되지 않았습니다."));
	}
		
}

void CEcsView::OnSearch()
{
	// TODO: Add your command handler code here
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

// 	if (pDoc->m_pSearchDlg != NULL)
// 	{
// 		pDoc->m_pSearchDlg->ShowWindow(SW_SHOWNORMAL);
// 		pDoc->m_pSearchDlg->BringWindowToTop();
// 	}
// 	else
// 	{
// 		pDoc->m_pSearchDlg = new CSearchDlg(this);
// 		pDoc->m_pSearchDlg->Create(IDD_SEARCH_DLG);
// 		pDoc->m_pSearchDlg->ShowWindow(SW_SHOW);
// 		
// 	}	
}


void CEcsView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	OnMouseMoveSelectedLayout(pDoc, nFlags, point);
	
	CFormView::OnMouseMove(nFlags, point);
}

BOOL CEcsView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBrush backBrush(RGB(255, 255, 255));               // 파랑색. 원하는 컬러를 너주면 된다...

	if(pDC == NULL)
		return TRUE;

	CBrush* pOldBrush = pDC->SelectObject(&backBrush); 
	CRect rect; pDC->GetClipBox(&rect); 
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush); 

	return TRUE;      

	return CFormView::OnEraseBkgnd(pDC);
}



void CEcsView::DrawSelectedLayout( CDC* pDC, CEcsDoc* pDoc )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnDraw(this, pDC, 0); 
}

void CEcsView::WheelSelectedLayout( CEcsDoc* pDoc, UINT nFlags, short zDelta, CPoint pt )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnMouseWheel(this, nFlags, zDelta, pt); 
}

void CEcsView::RButtonUpSelectedLayout( CEcsDoc* pDoc)
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnOriginalSize(this);
}

void CEcsView::OnSizeSelectedLayout(CEcsDoc* pDoc, UINT nType, int cx, int cy )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnSize(this, nType, cx, cy);
}

void CEcsView::OnLButtonDownSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnButtonDown(this, nFlags, point);

}

void CEcsView::OnButtonUpSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnButtonUp(this, nFlags, point);
}

void CEcsView::OnKeyDownSelectedLayout( CEcsDoc* pDoc, UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnKeyDown(this, nChar, nRepCnt, nFlags);
}

void CEcsView::PreTranslateMessageSelectedLayout( CEcsDoc * pDoc, WPARAM wParam, int param3, int param4 )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnKeyDown(this, wParam, 0, 0);
}

void CEcsView::OnMouseMoveSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point )
{
 	//CTimeSpan tTimeSpan;
 	//tTimeSpan = CTime::GetCurrentTime() - pDoc->m_tChecktime;
 	//if(tTimeSpan.GetTotalSeconds() < 1)
 	//	return;
 
 	//CEcsLayout* pLayout = (CEcsLayout*)pDoc->GetSelectedLayout();
 	//if(pLayout == NULL)
 	//	return;
 
 	//if(pDoc->m_bMoveFlag == TRUE)
 	//{
 	//	if(tTimeSpan.GetTotalSeconds() < 1)
 	//		return;
 	//	if(pDoc->m_PrevPoint != point)
 	//	{
 	//		if(pDoc->m_PrevPoint.x > point.x)
 	//		{
 	//			if(pDoc->m_PrevPoint.x - point.x > 3)
 	//			{
 	//				pLayout->OnKeyDown(this, VK_RIGHT, 0, 0);	
 
 	//				pDoc->m_PrevPoint.x = point.x;
 	//			}
 	//		}
 	//		else if(pDoc->m_PrevPoint.x < point.x)
 	//		{
 	//			if(point.x - pDoc->m_PrevPoint.x > 3)
 	//			{
 	//				pLayout->OnKeyDown(this, VK_LEFT, 0, 0);
 
 	//				pDoc->m_PrevPoint.x = point.x;
 	//			}
 	//		}
 	//		if(pDoc->m_PrevPoint.y > point.y)
 	//		{
 	//			if(pDoc->m_PrevPoint.y - point.y > 5)
 	//			{
 	//				pLayout->OnKeyDown(this, VK_DOWN, 0, 0);
 
 	//				pDoc->m_PrevPoint.y = point.y;
 	//			}
 	//		}
 	//		else if(pDoc->m_PrevPoint.y < point.y)
 	//		{
 	//			if(point.y - pDoc->m_PrevPoint.y > 5)
 	//			{
 	//				pLayout->OnKeyDown(this, VK_UP, 0, 0);
 
 	//				pDoc->m_PrevPoint.y = point.y;
 	//			}
 	//		}
 	//	}
 	//}
}

void CEcsView::GetQrySelectStatusAll( CCV_DATA* pCV_DATA, CString& pSTOCK_MODE, CString& pREMOTE_CONTROL, CString& pROLL_MODE)
{
	CEcsDoc* pDoc = GetDocument();
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	//strSql += CRLF + _T("SELECT (SELECT REMOTE_CONTROL FROM CV_DATA WHERE MC_NO = '101') AS REMOTE_CONTROL  ");
	strSql += CRLF + _T("SELECT (SELECT STOCK_MODE FROM CV_DATA WHERE MC_NO = '149') AS STOCK_MODE			");
	strSql += CRLF + _T("      ,(SELECT ROLL_MODE FROM CV_DATA WHERE MC_NO = '154') AS ROLL_MODE			");
	strSql += CRLF + _T("  FROM CV_DATA  ");
	strSql += CRLF + _T(" WHERE WH_TYP = ") + CLib::Quot(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND PLC_NO = ") + CLib::Quot(pCV_DATA->K_PLC_NO);
	strSql += CRLF + _T("   LIMIT 1 ");
	
	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 


	pSTOCK_MODE = pRsw->GetItem(_T("STOCK_MODE"));
	pREMOTE_CONTROL = _T("");
	pROLL_MODE = pRsw->GetItem(_T("ROLL_MODE"));

	delete pRsw;
}

BOOL CEcsView::UpdateIF_MC_STA(CString strMC_TYP, CString strMC_NO, CString strIF_STATUS, BOOL bPopUpMsgDlg)
{
	CEcsDoc* pDoc = GetDocument();

	CString pMC_STA = _T("");
	CString pMC_NO = _T("");

	int nRowCnt = 0;
	CString strSql = _T("");
	CString strMessage = _T("");

	strSql.Format(_T(" UPDATE IF_MC_STA			\n")
		_T("			  SET IF_STATUS = '%s'	\n")
		_T("			    , UPD_DT = %s		\n")
		_T("		    WHERE IF_STATUS = 'N'	\n")
		_T("			  AND MC_TYP = '%s'		\n")
		_T("			  AND MC_NO = '%s'		\n"), strIF_STATUS, pDoc->SYSDATE, strMC_TYP, strMC_NO);

	BOOL isSuccess = pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE)
	{
		if (bPopUpMsgDlg == TRUE)
			AfxMessageBox(pDoc->GetMsgLangDef(_T("실패")));
		CString strTemp = pDoc->GetDlgUrmDBAccessErrorString();
		pDoc->RollbackTrans_DLG();
		return FALSE;
	}

	pDoc->CommitTrans_DLG();
	if (bPopUpMsgDlg == TRUE)
		AfxMessageBox(pDoc->GetMsgLangDef(_T("성공")));

	// 아몰랑
	return TRUE;
}


BOOL CEcsView::ForCvSelectIF_MC_STA(BOOL bPopUpMsgDlg)
{
	CEcsDoc* pDoc = GetDocument();

	CString pMC_STA = _T("");
	CString pMC_NO = _T("");

	int nRowCnt = 0;
	CString strSql = _T("");
	CString strMessage = _T("");

	strSql.Format(_T(" SELECT *					")
		_T("		     FROM IF_MC_STA			")
		_T("		    WHERE MC_USE_DEF = 'Y'	")
		_T("			  AND MC_TYP = 'CV'		")
		_T("			  AND IF_STATUS = 'N'	"));


	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt > 0)
	{
		pMC_NO = pRsw->GetItem(_T("MC_NO"));

		pRsw->MoveFirst();
		for (int nIdx = 0; nIdx < nRowCnt; nIdx++)
		{
			pMC_STA = pRsw->GetItem(_T("MC_STA"));
			pDoc->m_bStoStation[nIdx] = _ttoi(pMC_STA);

			return UpdateIF_MC_STA(_T("CV"), pMC_NO);;
			//pRsw->MoveNext();	// 한개만 하고 빠져나갈것임!
		}
		return TRUE;
	}
	delete pRsw;

	// 아몰랑
	return FALSE;
}

BOOL CEcsView::ForScSelectIF_MC_STA(BOOL bPopUpMsgDlg)
{
	CEcsDoc* pDoc = GetDocument();

	CString pSC_STA = _T("");
	CString pMC_NO = _T("");

	int nRowCnt = 0;
	CString strSql = _T("");
	CString strMessage = _T("");

	strSql.Format(_T(" SELECT *					")
		_T("		     FROM IF_MC_STA			")
		_T("		    WHERE MC_USE_DEF = 'Y'	")
		_T("			  AND MC_TYP = 'SC'		")
		_T("			  AND IF_STATUS = 'N'	"));


	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt > 0)
	{
		pMC_NO = pRsw->GetItem(_T("MC_NO"));

		pRsw->MoveFirst();
		for (int nIdx = 0; nIdx < nRowCnt; nIdx++)
		{
			pSC_STA = pRsw->GetItem(_T("SC_STA"));
			pDoc->m_nScStatus[nIdx] = _ttoi(pSC_STA);

			return UpdateIF_MC_STA(_T("SC"), pMC_NO);;
			//pRsw->MoveNext();	// 한개만 하고 빠져나갈것임!
		}
		return TRUE;
	}
	delete pRsw;

	// 아몰랑
	return FALSE;
}

BOOL CEcsView::ForBcrSelectIF_MC_STA(BOOL bPopUpMsgDlg)
{
	CEcsDoc* pDoc = GetDocument();

	CString pBCR_DATA = _T("");
	CString pMC_NO = _T("");

	int nRowCnt = 0;
	CString strSql = _T("");
	CString strMessage = _T("");

	strSql.Format(_T(" SELECT *					")
		_T("		     FROM IF_MC_STA			")
		_T("		    WHERE MC_USE_DEF = 'Y'	")
		_T("			  AND MC_TYP = 'BCR'	")
		_T("			  AND IF_STATUS = 'N'	"));


	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt > 0)
	{
		pMC_NO = pRsw->GetItem(_T("MC_NO"));

		int nMC_NO = _ttoi(pMC_NO);
		pRsw->MoveFirst();
		for (int nIdx = 0; nIdx < nRowCnt; nIdx++)
		{
			pBCR_DATA = pRsw->GetItem(_T("BCR_DATA"));
			pDoc->m_strStoBcrData[nMC_NO-1] = pBCR_DATA;
			pDoc->m_bReportedBCD[nMC_NO-1] = TRUE;

			//pDoc->CommitTrans_DLG();
			//if (bPopUpMsgDlg == TRUE)
			//	AfxMessageBox(pDoc->GetMsgLangDef(_T("성공")));

			return UpdateIF_MC_STA(_T("BCR"), pMC_NO);
			//pRsw->MoveNext();	// 한개만 하고 빠져나갈것임!
		}
		return TRUE;
	}
	delete pRsw;

	// 아몰랑
	return FALSE;
}

BOOL CEcsView::DeleteIF_LUGG_STA(CString pLUGGNO, BOOL bPopUpMsgDlg)
{
	CEcsDoc* pDoc = GetDocument();
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	// strProductSize = _T("");

	// 인터페이스 정보가 있는지 확인
	strSql.Format(_T("SELECT *						")
		_T("	        FROM IF_LUGG_STA			")
		_T("           WHERE LUGGNO = '%s'			"), pLUGGNO);

	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	//CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt == 1)
	{
		// 해당 인터페이스 정보 삭제
		strSql.Format(_T("DELETE FROM IF_LUGG_STA 	 \n")
			_T(" WHERE LUGGNO = '%s'	 \n"), pLUGGNO);

		int isSuccess = pDoc->ExcuteQueryString_DLG(strSql);

		if (isSuccess == TRUE)
		{
			pDoc->CommitTrans_DLG();
			if (bPopUpMsgDlg == TRUE)
				AfxMessageBox(pDoc->GetMsgLangDef(_T("성공")));
			return TRUE;
		}
		if (bPopUpMsgDlg == TRUE)
			AfxMessageBox(pDoc->GetMsgLangDef(_T("실패")));
		CString strTemp = pDoc->GetDlgUrmDBAccessErrorString();
		pDoc->RollbackTrans_DLG();
		return FALSE;
	}

	if (bPopUpMsgDlg == TRUE)
		AfxMessageBox(pDoc->GetMsgLangDef(_T("작업이 없어서 삭제할 수 없습니다.")));
	return FALSE;

}

BOOL CEcsView::UpdateIF_LUGG_STA(CString pLUGGNO
							   , CString pTO_AREA		//= _T("")
							   , CString pWORK_STA		//= _T("00")
							   , CString pIF_STATUS		//= _T("N")
							   , CString pERRCODE		//= _T("00")
							   , CString pPRIORITY		//= _T("100")
							   , BOOL	 bPopUpMsgDlg	//= FALSE
)
{
	CEcsDoc* pDoc = GetDocument();
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	pDoc->BeginTrans_DLG();

	strSql = _T("");

	strSql.Format(_T(" INSERT INTO IF_LUGG_STA_HIS				\n")
		_T("           ( CRT_DATE								\n")	// 01	// CRT_DATE	
		_T("           , CRT_TIME								\n")	// 02	// CRT_TIME	
		_T("           , LUGGNO									\n")	// 03	// LUGGNO		
		_T("           , JOB_KIND								\n")	// 04	// JOB_KIND	
		_T("           , LD_CTN_NO								\n")	// 05	// LD_CTN_NO	
		_T("           , FROM_CV_NO								\n")	// 06	// FROM_CV_NO	
		_T("           , TO_CV_NO								\n")	// 07	// TO_CV_NO	
		_T("           , FROM_SC_NO								\n")	// 08	// FROM_SC_NO	
		_T("           , TO_SC_NO								\n")	// 09	// TO_SC_NO	
		_T("           , FROM_AREA								\n")	// 10	// FROM_AREA	
		_T("           , TO_AREA								\n")	// 11	// TO_AREA	
		_T("           , WORK_STA								\n")	// 12	// WORK_STA	
		_T("           , ST_ISHIGH								\n")	// 13	// ST_ISHIGH		
		_T("           , ERRCODE								\n")	// 14	// ERRCODE	
		_T("           , PRIORITY								\n")	// 15	// PRIORITY	
		_T("           , IF_STATUS								\n")	// 16	// IF_STATUS	
		_T("           , UPD_DT									\n")	// 17	// UPD_DT		
		_T("           , UPD_USER_ID							\n")	// 18	// UPD_USER_ID
		_T("           , PRDCT_NM								\n")	// 19	// PRDCT_NM
		_T("           )										\n")
		_T(" SELECT TO_CHAR(") + pDoc->SYSDATE + _T(", 'YYYYMMDD')					\n")
		_T("      , TO_CHAR(") + pDoc->SYSDATE + _T(", 'HH24MISS')					\n")
		_T("      , LUGGNO, JOB_KIND, LD_CTN_NO                                     \n")
		_T("      , FROM_CV_NO, TO_CV_NO, FROM_SC_NO, TO_SC_NO, FROM_AREA, TO_AREA  \n")
		_T("      , WORK_STA, ST_ISHIGH, ERRCODE, PRIORITY, IF_STATUS               \n")
		_T("      , ") + pDoc->SYSDATE + _T(", 'HOST_SIM', PRDCT_NM					\n")
		_T("   FROM IF_LUGG_STA                                                     \n")
		_T("  WHERE LUGGNO      = '%s'												\n"), pLUGGNO
	);

	BOOL isSuccess = pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE)
	{
		if (bPopUpMsgDlg == TRUE)
			AfxMessageBox(pDoc->GetMsgLangDef(_T("실패")));
		CString strTemp = pDoc->GetDlgUrmDBAccessErrorString();
		pDoc->RollbackTrans_DLG();
		return FALSE;
	}


	strSql = _T("");

	strSql.Format(_T(" UPDATE IF_LUGG_STA				\n")
		_T("    SET UPD_USER_ID = 'HOST_SIM_DB'		    \n")
		_T("      , UPD_DT = ") + pDoc->SYSDATE + _T("	\n"));

	if (pTO_AREA != _T(""))
		strSql += _T("	  , TO_AREA = '") + pTO_AREA + _T("'	\n");
	//if (pWORK_STA != _T("00"))
		strSql += _T("	  , WORK_STA = '") + pWORK_STA + _T("'	\n");
	//if (pIF_STATUS != _T("N"))
		strSql += _T("	  , IF_STATUS = '") + pIF_STATUS + _T("'	\n");
	//if (pERRCODE != _T("00"))
		strSql += _T("	  , ERRCODE = '") + pERRCODE + _T("'	\n");
	//if (pPRIORITY != _T("100"))
		strSql += _T("	  , PRIORITY = '") + pPRIORITY + _T("'	\n");

	strSql += _T("  WHERE LUGGNO = '") + pLUGGNO + _T("'	\n");

	isSuccess = pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE)
	{
		if (bPopUpMsgDlg == TRUE)
			AfxMessageBox(pDoc->GetMsgLangDef(_T("실패")));
		CString strTemp = pDoc->GetDlgUrmDBAccessErrorString();
		pDoc->RollbackTrans_DLG();
		return FALSE;
	}

	pDoc->CommitTrans_DLG();
	if (bPopUpMsgDlg == TRUE)
		AfxMessageBox(pDoc->GetMsgLangDef(_T("성공")));

	return TRUE;
}

BOOL CEcsView::SelectIF_LUGG_STA(BOOL bPopUpMsgDlg)
{
	CEcsDoc* pDoc = GetDocument();

	
	CString pLUGGNO		= _T("");
	CString pJOB_KIND	= _T("");
	CString pLD_CDN_NO	= _T("");
	CString pFROM_CV_NO	= _T("");
	CString pTO_CV_NO	= _T("");
	CString pFROM_SC_NO	= _T("");
	CString pTO_SC_NO	= _T("");
	CString pFROM_AREA	= _T("");
	CString pTO_AREA	= _T("");
	CString pWORK_STA	= _T("");	
	CString pISHIGH		= _T("");
	CString pERRCODE	= _T("");
	CString pPRIORITY	= _T("");
	CString pPRDCT_NM	= _T("");

	int nRowCnt = 0;
	CString strSql = _T("");
	CString strMessage = _T("");

	strSql.Format(_T("  SELECT *		\n")
		_T("	  FROM IF_LUGG_STA		\n")
		_T("	 WHERE IF_STATUS = 'N'	\n") );

//	if (pLUGGNO != _T("0"))	
//		strSql = strSql + _T("	 AND LUGGNO = '") + pLUGGNO + _T("' \n ORDER BY CRT_DATE DESC, CRT_TIME DESC");

	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	static int nRetryCnt = 0;

	if (nRowCnt > 0)
	{
		////strProductSize = pRsw->GetItem(_T("PRODUCT_SIZE"));

		pRsw->MoveFirst();
		for (int nIdx = 0; nIdx < nRowCnt; nIdx++)
		{
			pLUGGNO	= pRsw->GetItem(_T("LUGGNO"));
			pJOB_KIND = pRsw->GetItem(_T("JOB_KIND"));
			pLD_CDN_NO = pRsw->GetItem(_T("LD_CTN_NO"));
			pFROM_CV_NO = pRsw->GetItem(_T("FROM_CV_NO"));
			pTO_CV_NO = pRsw->GetItem(_T("TO_CV_NO"));
			pFROM_SC_NO = pRsw->GetItem(_T("FROM_SC_NO"));
			pTO_SC_NO = pRsw->GetItem(_T("TO_SC_NO"));
			pFROM_AREA = pRsw->GetItem(_T("FROM_AREA"));
			pTO_AREA = pRsw->GetItem(_T("TO_AREA"));
			pWORK_STA = pRsw->GetItem(_T("WORK_STA"));
			pISHIGH = pRsw->GetItem(_T("ST_ISHIGH"));
			pERRCODE = pRsw->GetItem(_T("ERRCODE"));
			pPRIORITY = pRsw->GetItem(_T("PRIORITY"));
			pPRDCT_NM = pRsw->GetItem(_T("PRDCT_NM"));
			
			if (pWORK_STA == _T("90")) // 91,96 미사용
			{
				pDoc->m_nWorkingLuggNum1 = 0;
				if (pJOB_KIND == _T("1"))
					pDoc->m_bCompleteStore1 = TRUE;
				else if (pJOB_KIND == _T("2") && pDoc->m_bBcrAutoUpdate1 == TRUE)
				{
					if (UpdateIF_MC_STA(_T("BCR"), _T("001"), _T("N")) == TRUE)
						pDoc->m_bReportedBCD[0] = TRUE;
				}


				// 지울것 - 91:강제완료(WCS->WMS), 96:작업삭제(WCS->WMS), 90:정상완료보고
				return DeleteIF_LUGG_STA(pLUGGNO);
			}
			else if (pWORK_STA == _T("99"))
			{
				pDoc->m_strAlterLocation = pTO_AREA;
				pDoc->m_nPrevLuggNum = _ttoi(pLUGGNO);
				int nErrorCode = _ttoi(pERRCODE);

				// 이중입고일때만... 새로운 로케이션 구하기 - 함수안에서 pDoc->m_strLocation 저장!!!
				if (nErrorCode == 60)
				{
					// 이중입고 일때 - 3번 재지정 후 에러 표시 
					if (nRetryCnt++ <= 3)
					{
						if (CalcurateLocation() == FALSE)
							return FALSE;

						// 자동 재지정		// 재작업 지시 - 에러코드는 던지지 않음! 바뀌지 않을 꺼니까
						if ((pDoc->m_nWorkingLuggNum1 = AlterLocation()) != 0)
						{
							// 재작업 로케이션 지우기 
							pDoc->m_strAlterLocation = _T("");
							return TRUE;
						}
						return FALSE;
					}
				}
				else
				{
					// 공출고 일때도 보내기는 해야함  - 에러코드는 던지지 않음! 바뀌지 않을 꺼니까
					if ((pDoc->m_nWorkingLuggNum1 = AlterLocation()) != 0)
					{
						// 재작업 로케이션 지우기 
						pDoc->m_strAlterLocation = _T("");
						return TRUE;
					}
					return FALSE;
				}

				nRetryCnt = 0;
				// Update 할 것!
				return UpdateIF_LUGG_STA(pLUGGNO, pTO_AREA, pWORK_STA, _T("Y"), _T("00"), _T("100"), FALSE);
			}
			pRsw->MoveNext();
		}
	}
	delete pRsw;

	//*/
	// 아몰랑
	return FALSE;
}

BOOL CEcsView::InsertIF_LUGG_STA(CString pLUGGNO				
								 , CString pJOB_KIND			
								 , CString pLD_CTN_NO			
								 , CString pFROM_CV_NO			
								 , CString pTO_CV_NO			
								 , CString pFROM_SC_NO			
								 , CString pTO_SC_NO			
								 , CString pFROM_AREA			
								 , CString pTO_AREA			
								 , CString pWORK_STA			
								 , CString pISHIGH			
								 , CString pERRCODE			
								 , CString pPRIORITY	
								 , CString pPRDCT_NM
								 , BOOL bPopUpMsgDlg
)	
{
	CEcsDoc* pDoc = GetDocument();
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	pDoc->BeginTrans_DLG();

	strSql = _T("");

//	CString strFromArea = CLib::CheckModifyLocation(pFROM_AREA);
//	CString strToArea = CLib::CheckModifyLocation(pTO_AREA);
	//CString CURRENT_DATE_NATURAL;
	//CString CURRENT_TIME_NATURAL;


	strSql.Format(_T(" INSERT INTO IF_LUGG_STA					\n")  
		_T("           ( CRT_DATE								\n")	// 01	// CRT_DATE	
		_T("           , CRT_TIME								\n")	// 02	// CRT_TIME	
		_T("           , LUGGNO									\n")	// 03	// LUGGNO		
		_T("           , JOB_KIND								\n")	// 04	// JOB_KIND	
		_T("           , LD_CTN_NO								\n")	// 05	// LD_CTN_NO	
		_T("           , FROM_CV_NO								\n")	// 06	// FROM_CV_NO	
		_T("           , TO_CV_NO								\n")	// 07	// TO_CV_NO	
		_T("           , FROM_SC_NO								\n")	// 08	// FROM_SC_NO	
		_T("           , TO_SC_NO								\n")	// 09	// TO_SC_NO	
		_T("           , FROM_AREA								\n")	// 10	// FROM_AREA	
		_T("           , TO_AREA								\n")	// 11	// TO_AREA	
		_T("           , WORK_STA								\n")	// 12	// WORK_STA	
		_T("           , ST_ISHIGH								\n")	// 13	// ST_ISHIGH		
		_T("           , ERRCODE								\n")	// 14	// ERRCODE	
		_T("           , PRIORITY								\n")	// 15	// PRIORITY	
		_T("           , IF_STATUS								\n")	// 16	// IF_STATUS	
		_T("           , UPD_DT									\n")	// 17	// UPD_DT		
		_T("           , UPD_USER_ID							\n")	// 18	// UPD_USER_ID
		_T("           , PRDCT_NM								\n")	// 19	// PRDCT_NM
		_T("           , WH_TYP									\n")	// 20	// WH_TYP
		_T("           )										\n")
		_T("      VALUES										\n")
		_T("           ( ") + pDoc->CURRENT_DATE_NATURAL + _T("	\n")	// 01	// CRT_DATE	
		_T("           , ") + pDoc->CURRENT_TIME_NATURAL + _T("	\n")	// 02	// CRT_TIME	
		_T("           , '") + pLUGGNO + _T("'					\n")	// 03	// LUGGNO		
		_T("           , '") + pJOB_KIND + _T("'				\n")	// 04	// JOB_KIND	 
		_T("           , '") + pLD_CTN_NO + _T("'				\n")	// 05	// LD_CTN_NO	 
		_T("           , '") + pFROM_CV_NO + _T("'				\n")	// 06	// FROM_CV_NO	 
		_T("           , '") + pTO_CV_NO + _T("'				\n")	// 07	// TO_CV_NO	
		_T("           , '") + pFROM_SC_NO + _T("'				\n")	// 08	// FROM_SC_NO	 
		_T("           , '") + pTO_SC_NO + _T("'				\n")	// 09	// TO_SC_NO	
		_T("           , '") + pFROM_AREA + _T("'				\n")	// 10	// FROM_AREA	 
		_T("           , '") + pTO_AREA + _T("'					\n")	// 11	// TO_AREA	
		_T("           , '") + pWORK_STA + _T("'				\n")	// 12	// WORK_STA	
		_T("           , '") + pISHIGH + _T("'					\n")	// 13	// ST_ISHIGH		 
		_T("           , '") + pERRCODE + _T("'					\n")	// 14	// ERRCODE	
		_T("           , '") + pPRIORITY + _T("'				\n")	// 15	// PRIORITY	
		_T("           , 'N'									\n")	// 16	// IF_STATUS	 
		_T("           , ") + pDoc->SYSDATE + _T("				\n")	// 17	// UPD_DT		 
		_T("           , 'HOST_DB_SIM'							\n")	// 18	// UPD_USER_ID
		_T("           , '") + pPRDCT_NM + _T("'				\n")	// 19	// PRDCT_NM	
		_T("           , '10'									\n")	// 20	// WH_TYP	
		_T("           )										\n")
		);

	BOOL isSuccess = pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE)
	{
		if (bPopUpMsgDlg == TRUE)
			AfxMessageBox(pDoc->GetMsgLangDef(_T("실패")));
		CString strTemp = pDoc->GetDlgUrmDBAccessErrorString();
		pDoc->RollbackTrans_DLG();
		return FALSE;
	}

	pDoc->CommitTrans_DLG();
	if (bPopUpMsgDlg == TRUE)
		AfxMessageBox(pDoc->GetMsgLangDef(_T("성공")));

	return TRUE;
}



void CEcsView::OnBnClickedChkBcrAutoUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CEcsDoc* pDoc = GetDocument();

	pDoc->m_bBcrAutoUpdate1 = m_bBcrAutoUpdate1;
}


void CEcsView::OnBnClickedChkBcrAutoUpdate2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEcsDoc* pDoc = GetDocument();

	//UpdateData(TRUE);
	pDoc->m_bReportedBCD[0] = !pDoc->m_bReportedBCD[0];

	//if (m_bIsBcrRead == TRUE)
	//{
	//	//pDoc->m_strStoBcrData[0] = pBCR_DATA;
	//	
	//}
	//else
	//{
	//	pDoc->m_bReportedBCD[0] = TRUE;
	//}
}
