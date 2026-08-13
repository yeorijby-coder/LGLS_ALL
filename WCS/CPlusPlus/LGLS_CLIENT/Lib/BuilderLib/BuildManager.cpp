// BuildManager.cpp: implementation of the CBuildManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuildManager.h"

#include "ControlManagerDlg.h"
#include "PropertyManagerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuildManager::CBuildManager()
{
	::CoInitialize(NULL);

	m_pDCI = new CDciMaster;
	m_pDciGridCtrl = (CDciGridCtrl*)m_pDCI->CreateDciObject(_T("CDciGridCtrl"));
	m_pSelectCtrls = new CDciControlList;

	m_bFirst = FALSE;
	m_nZoomCount = 0;
	m_nZoomLimit = 0;

	m_ctrlGroups.SetSize(1);
}

CBuildManager::~CBuildManager()
{
	m_pSelectCtrls->RemoveAll();
	delete m_pSelectCtrls;
	delete m_pDciGridCtrl;
	delete m_pDCI;

	::CoUninitialize();
}

void CBuildManager::OnInitialUpdate(CWnd* pWnd)
{
	if ((pWnd == NULL) || (pWnd->GetSafeHwnd() == NULL))
		return;

	CRect rcClient;
	pWnd->GetClientRect(rcClient);

	m_pDCI->SetLayoutS(rcClient);
	m_pDCI->SetLayoutL(m_pDciGridCtrl->m_rcControlL);
	m_pDCI->RecalcLayout();

	pWnd->Invalidate();
}

void CBuildManager::OnDraw(CWnd* pWnd, CDC* pDC)
{
	CFont font;
	if (!font.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 1, 0, 0, 0, 0, _T("Arial")))
		return;

	CFont* pOldFont = pDC->SelectObject(&font);

	m_pDciGridCtrl->UpdateControl(pDC);

	for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
	{
		CControlGroup& group = m_ctrlGroups[i];
		if (group.m_bVisible == FALSE)
			continue;
		
		for (POSITION pos=group.m_pGroupCtrls->GetHeadPosition(); pos!= NULL; )
			group.m_pGroupCtrls->GetNext(pos)->UpdateControl(pDC);
	}

	for (POSITION pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
	{
		CRectTracker tracker;
		tracker.m_rect = m_pDCI->ConvertRectS(m_pSelectCtrls->GetNext(pos)->m_rcControlL);
		tracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::hatchInside;
		tracker.Draw(pDC);
	}

	pDC->SelectObject(pOldFont);
}

void CBuildManager::OnSize(CWnd* pWnd, UINT nType, int cx, int cy)
{
	if ((m_pDCI == NULL) || (m_pDciGridCtrl == NULL))
		return;

	m_pDCI->SetLayoutS(0, 0, cx, cy);
	m_pDCI->RecalcLayout();
}

void CBuildManager::OnMouseWheel(CWnd* pWnd, UINT nFlags, short zDelta, CPoint pt)
{
	if (m_pDCI == NULL)
		return;

	CRect rcLayoutSTemp = m_pDCI->GetLayoutS();	// 고정 값 
	CRect rcLayoutL = m_pDCI->GetLayoutL();
	CRect rcLayoutS = m_pDCI->ConvertRectS(rcLayoutL);
	int nWidthS = rcLayoutS.Width();
	int nHeightS = rcLayoutS.Height();

	if (pt.x < rcLayoutS.left  || 
		pt.x > rcLayoutS.right || 
		pt.y < rcLayoutS.top  || 
		pt.y > rcLayoutS.bottom  ) 
		return;

	float fScale = 0.0;
	// 마우스 휠 다운 - 확대 
	if (zDelta < 0)
		OnZoomIn(pWnd);
	// 마우스 휠 업 - 축소
	else 
		OnZoomOut(pWnd);
}

void CBuildManager::OnOriginalSize(CWnd* pWnd)
{
	CDciControl* pDciControl = NULL;
//	DEBUGER_ASSERT_VALID(pWnd != NULL);
//	DEBUGER_ASSERT_VALID(m_pDCI != NULL);
	if ((m_pDCI == NULL) || (pWnd == NULL))
		return;

	CRect rect;

	m_nZoomCount = 0;
	m_pDCI->SetLayoutL(m_pDCI->m_rcLayoutLTemp);
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();

	for (int i=m_ctrlGroups.GetUpperBound(); i>=0; --i)
	{
		CControlGroup& group = m_ctrlGroups[i];
		for (POSITION pos=group.m_pGroupCtrls->GetTailPosition(); pos!=NULL; )
		{
			pDciControl = group.m_pGroupCtrls->GetPrev(pos);

			if (pDciControl == NULL)
				continue;

			//DEBUGER_ASSERT_VALID(pDciControl != NULL);
				
			CString strClassName = pDciControl->GetRuntimeClass()->m_lpszClassName;
			if (strClassName == "CDciStaticCtrl")
			{
				CDciStaticCtrl *pControl = (CDciStaticCtrl*)pDciControl;
				if (pControl != NULL)
					pControl->m_nFontSize = pControl->m_nFontSizeTemp ;
			}
		}
	}
}

void CBuildManager::OnZoomIn(CWnd* pWnd)
{
	CDciControl* pDciControl = NULL;
//	DEBUGER_ASSERT_VALID(pWnd != NULL);
//	DEBUGER_ASSERT_VALID(m_pDCI != NULL);
	if ((m_pDCI == NULL) || (pWnd == NULL))
		return;

	CRect rect;

//	m_nZoomCount = 0;
//	m_nZoomLimit = 30;

	CString strTemp = "이 이상은 확대할 수 없습니다.";

//	m_nZoomCount++;
	rect = m_pDCI->GetLayoutL();
	int l = rect.left+1;	
	int b = rect.bottom+1;	
	int r = rect.right-1;	
	int t = rect.top-1;		


	// 세로(Bottom)값과 가로(Right)값 중에 작은 값보다 크거나 같을때 
	if (t < r)
	{
		if (m_nZoomCount++ >= t)
		{
			m_nZoomLimit = m_nZoomCount;
			//strTemp.Format("이 이상은 확대할 수 없습니다.[Limit=%d][Count=%d]", m_nZoomLimit, m_nZoomCount);
			AfxMessageBox(strTemp);
			return;
		}
	}
	else 
	{
		if (m_nZoomCount++ >= r)
		{
			m_nZoomLimit = m_nZoomCount;
			//strTemp.Format("이 이상은 확대할 수 없습니다.[Limit=%d][Count=%d]", m_nZoomLimit, m_nZoomCount);
			AfxMessageBox(strTemp);
			return;	
		}
	}

	m_pDCI->SetLayoutL(l, t, r, b);
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();

//	CString strTemp;
	strTemp.Format(_T("\n\nZoom Count = %d\nLeft = %d\nBottom = %d\nRight = %d\nTop = %d"), m_nZoomCount, l, t, r, b);
//	AfxMessageBox(strTemp);

	for (int i=m_ctrlGroups.GetUpperBound(); i>=0; --i)
	{
		CControlGroup& group = m_ctrlGroups[i];
		for (POSITION pos=group.m_pGroupCtrls->GetTailPosition(); pos!=NULL; )
		{
			pDciControl = group.m_pGroupCtrls->GetPrev(pos);
			if (pDciControl == NULL)
				continue;

			//DEBUGER_ASSERT_VALID(pDciControl != NULL);
				
			CString strClassName = pDciControl->GetRuntimeClass()->m_lpszClassName;
			if(strClassName == _T("CDciStaticCtrl"))
			{
				CDciStaticCtrl *pControl = (CDciStaticCtrl*)pDciControl;
				if(pControl != NULL)
					pControl->m_nFontSize += 1  ;

			}
		}
	}
}

void CBuildManager::OnZoomOut(CWnd* pWnd)
{
	CDciControl* pDciControl = NULL;
//	DEBUGER_ASSERT_VALID(pWnd != NULL);
//	DEBUGER_ASSERT_VALID(m_pDCI != NULL);

	if ((m_pDCI == NULL) || (pWnd == NULL))
		return;

	CRect rect;

	rect = m_pDCI->GetLayoutL();
	int l = rect.left-1;	
	int b = rect.bottom-1;	
	int r = rect.right+1;	
	int t = rect.top+1;		

	CString strTemp;
	strTemp.Format(_T("Zoom Count = %d\nZoom Limit= %d\nLeft = %d\nBottom = %d\nRight = %d\nTop = %d"), m_nZoomCount, m_nZoomLimit, l, t, r, b);
	if (--m_nZoomCount < 0)
	{
		m_nZoomCount = 0;
//		strTemp = "이 이상은 축소할 수 없습니다\n\n" + strTemp;
//		AfxMessageBox(strTemp);
		AfxMessageBox(_T("이 이상은 축소할 수 없습니다"));
		return;
	}

//	AfxMessageBox(strTemp);

	m_pDCI->SetLayoutL(l, t, r, b);
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();

	for (int i=m_ctrlGroups.GetUpperBound(); i>=0; --i)
	{
		CControlGroup& group = m_ctrlGroups[i];
		for (POSITION pos=group.m_pGroupCtrls->GetTailPosition(); pos!=NULL; )
		{
			pDciControl = group.m_pGroupCtrls->GetPrev(pos);

			if (pDciControl == NULL)
				continue;

			//DEBUGER_ASSERT_VALID(pDciControl != NULL);

			CString strClassName = pDciControl->GetRuntimeClass()->m_lpszClassName;
			if (strClassName == _T("CDciStaticCtrl"))
			{
				CDciStaticCtrl *pControl = (CDciStaticCtrl*)pDciControl;
				if (pControl != NULL)
					pControl->m_nFontSize -= 1  ;
			}
		}
	}
}

void CBuildManager::OnButtonDown(CWnd* pWnd, UINT nFlag, const CPoint& point)
{
	if (nFlag & MK_LBUTTON)
	{
		for (int i=m_ctrlGroups.GetUpperBound(); i>=0; --i)
		{
			CControlGroup& group = m_ctrlGroups[i];
			for (POSITION pos=group.m_pGroupCtrls->GetTailPosition(); pos!= NULL; )
			{
				CDciControl* pDciControl = group.m_pGroupCtrls->GetPrev(pos);
				ASSERT(pDciControl != NULL);

				if (pDciControl->m_bVisible && pDciControl->SetClick(pWnd, point))
					break;
			}
		}
	}
}

void CBuildManager::OnButtonUp(CWnd* pWnd, UINT nFlag, const CPoint& point)
{
	if (nFlag == 0)
	{
	}
	else if (nFlag & MK_CONTROL)
	{
		pWnd->InvalidateRect(m_pDCI->ConvertRectS(m_pDciGridCtrl->m_rcSelectL));

		m_ptSelect1st = point;
		m_pDciGridCtrl->SetSelectRect(point);

		CDciControlList* pCtrlList = new CDciControlList;
		for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
			pCtrlList->AddTail(m_ctrlGroups[i].m_pGroupCtrls);

		CDciControl* pDciControl = NULL;
		for (POSITION pos=pCtrlList->GetTailPosition(); pos!=NULL; )
		{
			pDciControl = pCtrlList->GetPrev(pos);
			ASSERT(pDciControl != NULL);

			if (m_pDCI->ConvertRectS(pDciControl->m_rcControlL).PtInRect(point))
			{
				if ((pDciControl->m_bVisible == TRUE) && (m_pSelectCtrls->Find(pDciControl) == NULL))
					m_pSelectCtrls->AddTail(pDciControl);
				break;
			}
		}

		pCtrlList->RemoveAll();
		delete pCtrlList;

		pWnd->InvalidateRect(m_pDCI->ConvertRectS(m_pDciGridCtrl->m_rcSelectL));
	}
	else if (nFlag & MK_SHIFT)
	{
		pWnd->InvalidateRect(m_pDCI->ConvertRectS(m_pDciGridCtrl->m_rcSelectL));

		m_ptSelect2nd = point;
		m_pDciGridCtrl->SetSelectRect(m_ptSelect1st, m_ptSelect2nd);

		for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
		{
			CDciControl* pDciControl = NULL;
			CControlGroup& group = m_ctrlGroups[i];
			for (POSITION pos=group.m_pGroupCtrls->GetHeadPosition(); pos!=NULL; )
			{
				pDciControl = group.m_pGroupCtrls->GetNext(pos);
				ASSERT(pDciControl != NULL);

				if ((m_pSelectCtrls->Find(pDciControl) == NULL) &&
					(m_pDciGridCtrl->m_rcSelectL.left <= pDciControl->m_rcControlL.left) &&
					(m_pDciGridCtrl->m_rcSelectL.right >= pDciControl->m_rcControlL.right) &&
					(m_pDciGridCtrl->m_rcSelectL.top >= pDciControl->m_rcControlL.top) &&
					(m_pDciGridCtrl->m_rcSelectL.bottom <= pDciControl->m_rcControlL.bottom))
					m_pSelectCtrls->AddTail(pDciControl);
			}
		}

		pWnd->InvalidateRect(m_pDCI->ConvertRectS(m_pDciGridCtrl->m_rcSelectL));
	}

	for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
	{
		CDciControl* pDciControl = NULL;
		CControlGroup& group = m_ctrlGroups[i];
		for (POSITION pos=group.m_pGroupCtrls->GetHeadPosition(); pos!=NULL; )
		{
			pDciControl = group.m_pGroupCtrls->GetNext(pos);
			ASSERT(pDciControl != NULL);

			if (pDciControl && pDciControl->m_bClick)
			{
				pDciControl->m_bClick = FALSE;
				pDciControl->InvalidateControl(pWnd, FALSE);
			}
		}
	}

}

void CBuildManager::OnKeyDown(CWnd* pWnd, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CRect rect;

	m_bFirst = TRUE;

	rect = m_pDCI->GetLayoutL();
	int l = rect.left;		
	int b = rect.bottom;	
	int r = rect.right;		
	int t = rect.top;		
	m_pDCI->RecalcLayout();
	//pWnd->Invalidate();

	int nAddValue = 0;

	switch (nChar)
	{
	case VK_ESCAPE:
		{
			m_pSelectCtrls->RemoveAll();
			m_pDciGridCtrl->m_rcSelectL.SetRectEmpty();
			m_ptSelect1st = m_ptSelect2nd = CPoint(0,0);

			pWnd->Invalidate();
		}
		break;

	case VK_LEFT:
		// 선택되었을때 - 선택된 컨트롤 이동  
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CDciControl* pDciControl = NULL;
			for (POSITION pos = m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				if (pDciControl = m_pSelectCtrls->GetNext(pos))
				{
					if ((::GetKeyState(VK_SHIFT) < 0))	// CTRL KEY DOWN
					{
						// Shift Key 눌려졌을때 - 왼쪽이 늘어남
						pDciControl->m_rcControlL.right--;
					}
					else
					{
						// Shift Key 눌려지지 않았을때 - 왼쪽으로 이동함!
						pDciControl->m_rcControlL.left--;
						pDciControl->m_rcControlL.right--;
					}
				}
			}
			pWnd->Invalidate();
		}
		// 선택되지 않았을때 - 전체가 왼쪽으로 이동(3칸씩)
		else 
		{
			nAddValue = 3;
			rect = m_pDCI->GetLayoutL();
			l = rect.left + nAddValue;	
			b = rect.bottom;	
			r = rect.right + nAddValue;	
			t = rect.top;		
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_RIGHT:
		// 선택되었을때 - 선택된 컨트롤 이동  
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CDciControl* pDciControl = NULL;
			for (POSITION pos = m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				if (pDciControl = m_pSelectCtrls->GetNext(pos))
				{
					if ((::GetKeyState(VK_SHIFT) < 0))	// CTRL KEY DOWN
					{
						// Shift Key 눌려졌을때 - 오른쪽이 늘어남
						pDciControl->m_rcControlL.right++;
					}
					else
					{
						// Shift Key 눌려지지 않았을때 - 오른쪽으로 이동함!
						pDciControl->m_rcControlL.left++;
						pDciControl->m_rcControlL.right++;
					}
				}
			}
			pWnd->Invalidate();
		}
		// 선택되지 않았을때 - 전체가 오른쪽으로 이동(3칸씩)
		else 
		{
			nAddValue = 3;
			rect = m_pDCI->GetLayoutL();
			l = rect.left - nAddValue;	
			b = rect.bottom;	
			r = rect.right - nAddValue;	
			t = rect.top;		
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_UP:
		// 선택되었을때 - 선택된 컨트롤 이동  
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CDciControl* pDciControl = NULL;
			for (POSITION pos = m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				if (pDciControl = m_pSelectCtrls->GetNext(pos))
				{
					if ((::GetKeyState(VK_SHIFT) < 0))	// CTRL KEY DOWN
					{
						// Shift Key 눌려졌을때 - 위쪽이 늘어남
						pDciControl->m_rcControlL.top++;
					}
					else
					{
						// Shift Key 눌려지지 않았을때 - 위쪽으로 이동함!
						pDciControl->m_rcControlL.top++;
						pDciControl->m_rcControlL.bottom++;
					}
				}
			}
			pWnd->Invalidate();
		}
		// 선택되지 않았을때 - 전체가 위로 이동(2칸씩)
		else 
		{
			nAddValue = 2;
			rect = m_pDCI->GetLayoutL();
			l = rect.left;		
			b = rect.bottom - nAddValue;
			r = rect.right;		
			t = rect.top - nAddValue;		
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_DOWN:
		// 선택되었을때 - 선택된 컨트롤 이동  
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CDciControl* pDciControl = NULL;
			for (POSITION pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				if (pDciControl = m_pSelectCtrls->GetNext(pos))
				{
					if ((::GetKeyState(VK_SHIFT) < 0))	// CTRL KEY DOWN
					{
						// Shift Key 눌려졌을때 - 아래쪽이 늘어남
						pDciControl->m_rcControlL.top--;
					}
					else
					{
						// Shift Key 눌려지지 않았을때 - 아래쪽으로 이동함!
						pDciControl->m_rcControlL.top--;
						pDciControl->m_rcControlL.bottom--;
					}
				}
			}
			pWnd->Invalidate();
		}
		// 선택되지 않았을때 - 전체가 아래로 이동(2칸씩)
		else 
		{
 			nAddValue = 2;
			rect = m_pDCI->GetLayoutL();
			l = rect.left;		
			b = rect.bottom + nAddValue;	
			r = rect.right;		
			t = rect.top + nAddValue;		
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();			
		}
		break;

	case VK_ADD:		// [+]
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CDciControl* pDciControl = NULL;
			for (POSITION pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				if (pDciControl = m_pSelectCtrls->GetNext(pos))
				{
					pDciControl->m_rcControlL.left--;
					pDciControl->m_rcControlL.bottom--;
					pDciControl->m_rcControlL.right++;
					pDciControl->m_rcControlL.top++;
				}
			}
			pWnd->Invalidate();
		}
		break;

	case VK_SUBTRACT:	// [-]
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CDciControl* pDciControl = NULL;
			for (POSITION pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				if (pDciControl = m_pSelectCtrls->GetNext(pos))
				{
					pDciControl->m_rcControlL.left++;
					pDciControl->m_rcControlL.bottom++;
					pDciControl->m_rcControlL.right--;
					pDciControl->m_rcControlL.top--;
				}
			}
			pWnd->Invalidate();
		}
		break;

	case VK_MULTIPLY:	// [*]
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CDciControl* pDciControl = NULL;
			for (POSITION pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				if (pDciControl = m_pSelectCtrls->GetNext(pos))
				{
					pDciControl->m_rcControlL.left *= 2;
					pDciControl->m_rcControlL.top *= 2;
					pDciControl->m_rcControlL.right *= 2;
					pDciControl->m_rcControlL.bottom *= 2;
				}
			}
			pWnd->Invalidate();
		}
		break;

	case VK_DIVIDE:		// [/]
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CDciControl* pDciControl = NULL;
			for (POSITION pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				if (pDciControl = m_pSelectCtrls->GetNext(pos))
				{
					pDciControl->m_rcControlL.left /= 2;
					pDciControl->m_rcControlL.top /= 2;
					pDciControl->m_rcControlL.right /= 2;
					pDciControl->m_rcControlL.bottom /= 2;
				}
			}
			pWnd->Invalidate();
		}
		break;

	case 'A':
		{
			if ((::GetKeyState(VK_CONTROL) < 0) && (::GetKeyState(VK_SHIFT) >= 0))	// CTRL + A
			{
				m_pSelectCtrls->RemoveAll();

				for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
					m_pSelectCtrls->AddTail(m_ctrlGroups[i].m_pGroupCtrls);

				pWnd->Invalidate();
			}
			else
			{
				CControlManagerDlg dlg(this, pWnd);
				dlg.DoModal();

				/*
				if (m_oControlManagerDlg.m_hWnd == NULL)
				{
					m_oControlManagerDlg.Create(IDD_ECS_CONTROL);

					CRect MainRect;
					CRect Rect;
					CRect PosRect;

					::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);
					m_oControlManagerDlg.GetWindowRect(&Rect);

					PosRect.left = ((MainRect.right - MainRect.left) - Rect.Width()) / 2;
					PosRect.top = ((MainRect.bottom - MainRect.top) - Rect.Height()) / 2;
					m_oControlManagerDlg.SetWindowPos(&wndTop, PosRect.left, PosRect.top,
						Rect.Width(), Rect.Height(),
						SWP_SHOWWINDOW);

					m_oControlManagerDlg.Refresh();
				}
				else
				{
					if (!m_oControlManagerDlg.IsWindowVisible())
						pDoc->m_oControlDlg.ShowWindow(SW_SHOWNORMAL);

					m_oControlManagerDlg.SendMessage(WM_INITDIALOG);
					m_oControlManagerDlg.Refresh();
				}
				//*/
			}
		}
		break;

	case 'C':
	case VK_INSERT:
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CControlGroup* pGroup = NULL;
			CDciControl* pDciCtrlSrc = NULL;
			CDciControl* pDciCtrlTar = NULL;
			CDciControlList* pSelectCtrls = new CDciControlList;

			for (POSITION pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				pDciCtrlSrc = m_pSelectCtrls->GetNext(pos);
				pDciCtrlTar = m_pDCI->CreateDciObject(pDciCtrlSrc->GetRuntimeClass()->m_lpszClassName, pDciCtrlSrc);
				ASSERT((pDciCtrlSrc != NULL) && (pDciCtrlTar != NULL));

				pGroup = m_ctrlGroups.GetControlGroup(pDciCtrlSrc);
				pGroup->m_pGroupCtrls->AddTail(pDciCtrlTar);
				pSelectCtrls->AddTail(pDciCtrlTar);				
				ASSERT(pGroup != NULL);
			}

			m_pSelectCtrls->RemoveAll();
			m_pSelectCtrls->AddTail(pSelectCtrls);

			pSelectCtrls->RemoveAll();
			delete pSelectCtrls;
			
			pWnd->Invalidate();
		}
		break;

	case 'D':
	case VK_DELETE:
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CControlGroup* pGroup = NULL;
			CDciControl* pDciControl = NULL;
			for (POSITION pos1=m_pSelectCtrls->GetHeadPosition(); pos1!=NULL; )
			{
				pDciControl = m_pSelectCtrls->GetNext(pos1);
				ASSERT(pDciControl != NULL);

				pGroup = m_ctrlGroups.GetControlGroup(pDciControl);
				ASSERT(pGroup != NULL);

				for (POSITION pos2=pGroup->m_pGroupCtrls->GetHeadPosition(); pos2!=NULL; )
				{
					if (pDciControl == pGroup->m_pGroupCtrls->GetNext(pos2))
					{
						if (pos2 == NULL)
							pos2 = pGroup->m_pGroupCtrls->GetTailPosition();
						else
							pGroup->m_pGroupCtrls->GetPrev(pos2);

						pGroup->m_pGroupCtrls->RemoveAt(pos2);
						delete pDciControl;
						pDciControl = NULL;
						break;
					}
				}
			}
			m_pSelectCtrls->RemoveAll();

			pWnd->Invalidate();
		}
		break;

	case 'Z':
		{
			CPropertyManagerDlg dlg(this);
			if (dlg.DoModal() != IDOK)
				return;

// 			for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
// 			{
// 				CControlGroup& group = m_ctrlGroups[i];
// 				for (POSITION pos=group.m_pGroupCtrls->GetHeadPosition(); pos!=NULL; )
// 					group.m_pGroupCtrls->GetNext(pos)->m_bVisible = TRUE;
//			}
			m_pDCI->SetLayoutL(m_pDciGridCtrl->m_rcControlL);
			m_pDCI->RecalcLayout();
			
			pWnd->Invalidate();
		}
		break;

	case 'L':
		{
// 			for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
// 			{
// 				CControlGroup& group = m_ctrlGroups[i];
// 				for (POSITION pos=group.m_pGroupCtrls->GetHeadPosition(); pos!=NULL; )
// 					group.m_pGroupCtrls->GetNext(pos)->m_bVisible = FALSE;
// 
// 				for (pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
// 					group.m_pGroupCtrls->GetNext(pos)->m_bVisible = TRUE;
// 			}

			if (m_pSelectCtrls->GetCount() < 1)
			{
				AfxMessageBox(_T("선택된 Control이 없습니다."));
				return;
			}

			if (m_pSelectCtrls->GetHead() == NULL)
			{
				AfxMessageBox(_T("NULL POINTER"));
				return;
			}

			CDciControl* pDciControl = NULL;
			CRect rcRectL = m_pSelectCtrls->GetHead()->m_rcControlL;
			for (POSITION pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				pDciControl = m_pSelectCtrls->GetNext(pos);
				ASSERT(pDciControl != NULL);

				rcRectL.left	= (rcRectL.left < pDciControl->m_rcControlL.left) ? rcRectL.left : pDciControl->m_rcControlL.left;
				rcRectL.bottom	= (rcRectL.bottom < pDciControl->m_rcControlL.bottom) ? rcRectL.bottom : pDciControl->m_rcControlL.bottom;
				rcRectL.right	= (rcRectL.right > pDciControl->m_rcControlL.right) ? rcRectL.right : pDciControl->m_rcControlL.right;
				rcRectL.top		= (rcRectL.top > pDciControl->m_rcControlL.top) ? rcRectL.top : pDciControl->m_rcControlL.top;
			}

			m_pDCI->SetLayoutL(rcRectL);
			m_pDCI->RecalcLayout();

			pWnd->Invalidate();
		}
		break;

	case 'S':
		{
			m_pDCI->SetLayoutS(m_pDCI->ConvertRectS(m_pDciGridCtrl->m_rcSelectL));
			m_pDCI->RecalcLayout();

			pWnd->Invalidate();
		}
		break;

	case 'V':
		if (m_pSelectCtrls && m_pSelectCtrls->GetCount())
		{
			CDciControl* pDciControl = NULL;
			for (POSITION pos=m_pSelectCtrls->GetHeadPosition(); pos!=NULL; )
			{
				pDciControl = m_pSelectCtrls->GetNext(pos);
				pDciControl->m_bVisible = !(pDciControl->m_bVisible);
			}

			pWnd->Invalidate();
		}
		break;

	case 'G':
		if (m_pDciGridCtrl)
		{
			m_pDciGridCtrl->m_bVisible = !(m_pDciGridCtrl->m_bVisible);

			pWnd->Invalidate();
		}
		break;

	case 'N':
		{
			for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
			{
				CDciControl* pDciControl = NULL;
				CControlGroup& group = m_ctrlGroups[i];
				for (POSITION pos=group.m_pGroupCtrls->GetHeadPosition(); pos!=NULL; )
				{
					pDciControl = group.m_pGroupCtrls->GetNext(pos);
					ASSERT(pDciControl != NULL);

					int nTrackLetter = 0;
					int nCIdStrCount = pDciControl->m_strCID.GetLength();
					if (nCIdStrCount >= 8 && pDciControl->m_strCID.Mid(3,2) == "10")
					{
						nTrackLetter = nCIdStrCount - 5;
						pDciControl->m_strText.Format(_T("%d"), CConvert::ToInt(pDciControl->m_strCID.Right(nTrackLetter)));
					}
				}
			}

			pWnd->Invalidate();
		}
		break;

	case 'M':
		{
			for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
			{
				CDciControl* pDciControl = NULL;
				CControlGroup& group = m_ctrlGroups[i];
				for (POSITION pos=group.m_pGroupCtrls->GetHeadPosition(); pos!=NULL; )
				{
					pDciControl = group.m_pGroupCtrls->GetNext(pos);
					ASSERT(pDciControl != NULL);

					int nTrackLetter = 0;
					int nCIdStrCount = pDciControl->m_strCID.GetLength();
					if (pDciControl->m_strText.IsEmpty() && nCIdStrCount >= 8 && pDciControl->m_strCID.Mid(3,2) == "10")
						pDciControl->m_strText.Format(_T("%d"), CConvert::ToInt(pDciControl->m_strCID.Right(nCIdStrCount - 5)));
					else
						pDciControl->m_strText.Empty();
				}
			}

			pWnd->Invalidate();
		}
		break;
	}
}

void CBuildManager::OnFileOpen(CWnd* pWnd)
{
	CFileDialog dlg(TRUE, _T("xml"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Xml Files (*.xml)|*.xml||"));
	if (dlg.DoModal() != IDOK)
		return;

	if (!LoadXml(dlg.GetPathName()))
	{
		AfxMessageBox(m_strMessage);
		return;
	}

	if (pWnd && pWnd->GetSafeHwnd())
		pWnd->Invalidate();

	//dlg.GetFileName();

	CString strTemp = dlg.GetFileName() + _T(" - Builder ");

	CWnd* pFrmWnd = AfxGetMainWnd();
	if (pFrmWnd)
		pFrmWnd->SetWindowText(strTemp);
}

void CBuildManager::OnFileSave(CWnd* pWnd /* = NULL */)
{
	CFileDialog dlg(FALSE, _T("xml"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Xml Files (*.xml)|*.xml||"));
	if (dlg.DoModal() != IDOK)
		return;

	if (!SaveXml(dlg.GetPathName()))
	{
		AfxMessageBox(m_strMessage);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CBuildManager::LoadXml(const CString& strFullPath)
{
	CXmlDom xml;

	if (!xml.InitializeXmlDom())
	{
		m_strMessage = _T("XML 초기화 실패!");
		return FALSE;
	}
		
	if (!xml.LoadXmlFile(strFullPath))
	{
		m_strMessage.Format(_T("%s 로딩 실패!"), strFullPath);
		return FALSE;
	}

	if (xml.MoveXPath(_T("//Builder")))
	{
		CDciPropertyArray properties;
		m_pDciGridCtrl->UpdatePropNames(properties);
		int nPropCount = properties.GetSize();
		for (int i=0; i< nPropCount; ++i)
		{ 
			if (i == 12)
				int a = 0;

			CString strName = properties[i].m_strName;
			CString strValue = properties[i].m_strValue;

			xml.GetAttrValue(properties[i].m_strName, properties[i].m_strValue);

			strName = properties[i].m_strName;
			strValue = properties[i].m_strValue;
		}
		m_pDciGridCtrl->UpdatePropValues(properties);
		
		m_pDCI->SetLayoutL(m_pDciGridCtrl->m_rcControlL);
		m_pDCI->m_rcLayoutLTemp = m_pDCI->GetLayoutL();	
		m_pDCI->RecalcLayout();
	}

	if (xml.GetNodeCount(_T("//Group")))
	{
		xml.MoveRoot();
		m_ctrlGroups.RemoveAll();
		m_ctrlGroups.SetSize(xml.GetChildElmtCount());

		for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
		{
			xml.MoveChild(i);

			CControlGroup& group = m_ctrlGroups[i];
			group.LoadXml(&xml, m_pDCI);

			xml.MoveRoot();
		}
	}

	return TRUE;
}

BOOL CBuildManager::SaveXml(const CString& strFullPath)
{
	CXmlDom xml;

	if (!xml.InitializeXmlDom())
	{
		m_strMessage = _T("XML 초기화 실패!");
		return FALSE;
	}

	xml.MoveDocument();
	xml.AddChildProcessingInstruction(_T("xml"), _T("version='1.0' encoding='euc-kr'"));
	xml.AddChildElement(_T("Builder"));
	xml.MoveRoot();

	CDciPropertyArray properties;
	m_pDciGridCtrl->UpdatePropNames(properties);
	m_pDciGridCtrl->UpdatePropValues(properties, FALSE);
	for (int i=0; i<properties.GetSize(); ++i)
		xml.AddChildAttribute(properties[i].m_strName, properties[i].m_strValue);

	xml.MoveParent();

	for (int i=0; i<m_ctrlGroups.GetSize(); ++i)
	{
		xml.AddChildElement(_T("Group"));
		xml.MoveChildLast();

		m_ctrlGroups[i].SaveXml(&xml);

		xml.MoveParent();
	}

	if (!xml.SaveXmlFile(strFullPath))
	{
		m_strMessage.Format(_T("%s 저장 실패!"), strFullPath);
		return FALSE;
	}

	return TRUE;
}

/*

BOOL CBuildManager::LoadXml(CXmlDom* pXML, CDciControl* pDciControl)
{
	if ((pXML == NULL) || (pDciControl == NULL))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CDciPropertyArray properties;
	pDciControl->UpdatePropNames(properties);
	ASSERT(properties.GetSize() == pXML->GetAttrCount());

	for (int i=0; i<properties.GetSize(); ++i)
		pXML->GetAttrValue(properties[i].m_strName, properties[i].m_strValue);
	pDciControl->UpdatePropValues(properties);

	if (pDciControl->GetItemSize())
	{
		pDciControl->UpdateItemNames(properties);

		for (int i=0; i<pDciControl->GetItemSize(); ++i)
		{
			pXML->MoveChild(i);
			
			for (int j=0; j<properties.GetSize(); ++j)
				pXML->GetAttrValue(properties[j].m_strName, properties[j].m_strValue);
			pDciControl->UpdateItemValues(i, properties);

			pXML->MoveParent();
		}
	}

	return TRUE;
}

BOOL CBuildManager::SaveXml(CXmlDom* pXML, CDciControl* pDciControl)
{
	if ((pXML == NULL) || (pDciControl == NULL))
	{
		ASSERT(FALSE);
		return FALSE;
	}

// 	CDciPropertyArray properties;
// 	pDciControl->UpdatePropNames(properties);
// 	pDciControl->UpdatePropValues(properties, FALSE);
// 	for (int i=0; i<properties.GetSize(); ++i)
// 		pXML->SetAttrValue(properties[i].m_strName, properties[i].m_strValue);

	return TRUE;
}

BOOL CBuildManager::CreateXml(CXmlDom* pXML, CDciControl* pDciControl)
{
	if ((pXML == NULL) || (pDciControl == NULL))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pXML->AddChildElement(pDciControl->GetRuntimeClass()->m_lpszClassName);
	pXML->MoveChildLast();

	CDciPropertyArray properties;
	pDciControl->UpdatePropNames(properties);
	pDciControl->UpdatePropValues(properties, FALSE);
	for (int i=0; i<properties.GetSize(); ++i)
		pXML->AddChildAttribute(properties[i].m_strName, properties[i].m_strValue);

	if (pDciControl->GetItemSize())
	{
		pDciControl->UpdateItemNames(properties);

		for (int i=0; i<pDciControl->GetItemSize(); ++i)
		{
			pDciControl->UpdateItemValues(i, properties, FALSE);

			pXML->AddChildElement(_T("Item"));
			pXML->MoveChildLast();

			for (int j=0; j<properties.GetSize(); ++j)
				pXML->AddChildAttribute(properties[j].m_strName, properties[j].m_strValue);

			pXML->MoveParent();
		}
	}

	pXML->MoveParent();

	return TRUE;
}
*/