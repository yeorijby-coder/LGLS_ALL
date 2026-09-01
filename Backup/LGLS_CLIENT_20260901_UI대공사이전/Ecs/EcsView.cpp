
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
#include "WarningDlg.h"
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
	ON_MESSAGE(WM_USER_CONTROL_CLICK, &CEcsView::OnControlClick)
	ON_MESSAGE(WM_USER_REFRESH_DIALOG, &CEcsView::OnRefreshDialog)
END_MESSAGE_MAP()



CEcsView::CEcsView()
	: CFormView(CEcsView::IDD)
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

	// [LGLS 2026-08-22] 작업 체류 경고창을 미리 만들어 숨겨 둔다.
	//   TASK 로그는 서버에만 남아 운전자가 보지 못하므로 Client 에서 창으로 알린다.
	//   창 자신이 10초 주기로 JOB_MST 를 살펴 체류 작업이 생기면 스스로 나타난다.
	if (pDoc->m_pWarningDlg == NULL)
	{
		CWarningDlg* pWarn = new CWarningDlg(pDoc);
		if (pWarn->Create(IDD_WARNING_DLG, this))
		{
			pWarn->ShowWindow(SW_HIDE);
			pDoc->m_pWarningDlg = pWarn;
		}
		else
		{
			delete pWarn;
		}
	}

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
	if(pDoc == NULL)
	{	
		return FALSE;	
	}

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
				pDoc->m_pRtvSkinDlg->Create(IDD_SKIN_RTV_CTRL1);
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
	strSql += CRLF + _T("SELECT TOP 1 (SELECT STOCK_MODE FROM CV_DATA WHERE MC_NO = '149') AS STOCK_MODE			");	// [LGLS] TOP 1 instead of LIMIT 1
	strSql += CRLF + _T("      ,(SELECT ROLL_MODE FROM CV_DATA WHERE MC_NO = '154') AS ROLL_MODE			");
	strSql += CRLF + _T("  FROM CV_DATA  ");
	strSql += CRLF + _T(" WHERE WH_TYP = ") + CLib::Quot(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND PLC_NO = ") + CLib::Quot(pCV_DATA->K_PLC_NO);	// [LGLS] LIMIT removed (TOP 1 above)
	
	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 


	pSTOCK_MODE = pRsw->GetItem(_T("STOCK_MODE"));
	pREMOTE_CONTROL = _T("");
	pROLL_MODE = pRsw->GetItem(_T("ROLL_MODE"));

	delete pRsw;
}
