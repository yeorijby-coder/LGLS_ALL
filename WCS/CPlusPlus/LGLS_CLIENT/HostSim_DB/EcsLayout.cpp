// EcsLayout.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsLayout.h"
#include "XmlDom.h"
#include "DciStaticCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// CEcsLayout
//
CEcsLayout::CEcsLayout()
{
	m_pDCI = new CDciMaster;
	m_pDciCtrls = new CDciControlList;

	m_bFirst = TRUE;

	m_nZoomCount = 0;
	m_nZoomLimit = 30;		// 사용안함
}

CEcsLayout::~CEcsLayout()
{
	delete m_pDciCtrls;
	delete m_pDCI;
}


// CEcsLayout 멤버 함수
//
void CEcsLayout::OnInitialUpdate(CWnd* pWnd)
{
	if ((pWnd == NULL) || (pWnd->GetSafeHwnd() == NULL))
		return;
	
	m_nFontSize = ::GetPrivateProfileInt(_T("LAYOUT"), _T("FontSize"), 10, ECS_INI_FILE);

	CString strFontFace = _T("돋움");
	TRY
	{
		//여기
		if (!m_oFont.CreateFont(10, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, 0, 0, strFontFace))
		{
			CString strTemp;
			strTemp.Format(_T("%s  폰트생성 실패!"),__WFUNCTION__);
			DEBUGER_ASSERT_VALID_LOG(FALSE, strTemp);
			return;
		}

		CRect rcClient;
		pWnd->GetClientRect(rcClient);
		m_pDCI->SetLayoutS(rcClient);
		m_pDCI->RecalcLayout();
		pWnd->Invalidate();
	}
	CATCH (CException, e)
	{
		CString strTemp;
		strTemp.Format(_T("%s"),e->GetRuntimeClass());
		e->ReportError();
	}
	END_CATCH
}

void CEcsLayout::OnDraw(CWnd* pWnd, CDC* pDC, int nTabType)
{
	CFont* pOldFont = pDC->SelectObject(&m_oFont);

	CDciControl* pDciControl = NULL;
	for (POSITION pos=m_pDciCtrls->GetHeadPosition(); pos!= NULL; )
	{
		pDciControl = m_pDciCtrls->GetNext(pos);	
		DEBUGER_ASSERT_VALID(pDciControl != NULL);

		pDciControl->UpdateControl(pDC);
	}
	pDC->SelectObject(pOldFont);
}

void CEcsLayout::OnMouseWheel(CWnd* pWnd, UINT nFlags, short zDelta, CPoint pt)
{
	if ((m_pDCI == NULL) || (m_pDciCtrls == NULL))
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


void CEcsLayout::OnSize(CWnd* pWnd, UINT nType, int cx, int cy)
{
	if ((m_pDCI == NULL) || (m_pDciCtrls == NULL))
		return;

	m_pDCI->SetLayoutS(0, 0, cx, cy);
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();
}

BOOL CEcsLayout::OnButtonDown(CWnd* pWnd, UINT nFlag, const CPoint& point)
{
	CDciControl* pDciControl = NULL;
	for (POSITION pos=m_pDciCtrls->GetTailPosition(); pos!= NULL; )
	{
		pDciControl = m_pDciCtrls->GetPrev(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);

		if (pDciControl->m_bVisible && pDciControl->SetClick(pWnd, point))
			return TRUE;
	}
	return FALSE;
}

void CEcsLayout::OnButtonUp(CWnd* pWnd, UINT nFlag, const CPoint& point)
{
	CDciControl* pDciControl = NULL;
	for (POSITION pos=m_pDciCtrls->GetTailPosition(); pos!=NULL; )
	{
		pDciControl = m_pDciCtrls->GetPrev(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);

		if (pDciControl->m_bClick)
		{
			pDciControl->m_bClick = FALSE;
			pDciControl->InvalidateControl(pWnd, FALSE);
			if (CLib::IsValidControlID(pDciControl->m_strCID))
				pWnd->PostMessage(WM_USER_CONTROL_CLICK, CConvert::ToInt(pDciControl->m_strCID));
			return;
		}
	}
}

void CEcsLayout::OnOriginalSize(CWnd* pWnd)
{
	CDciControl* pDciControl = NULL;
	DEBUGER_ASSERT_VALID(pWnd != NULL);
	DEBUGER_ASSERT_VALID(m_pDCI != NULL);
	CRect rect;

	m_nZoomCount = 0;
	m_pDCI->SetLayoutL(m_pDCI->m_rcLayoutLTemp);
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();

	for (POSITION pos=m_pDciCtrls->GetTailPosition(); pos!=NULL; )
	{
		pDciControl = m_pDciCtrls->GetPrev(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);
				
		CString strClassName = CA2CT(pDciControl->GetRuntimeClass()->m_lpszClassName);
		if(strClassName == _T("CDciStaticCtrl"))
		{
			CDciStaticCtrl *pControl = (CDciStaticCtrl*)pDciControl;
			if(pControl != NULL)
				pControl->m_nFontSize = pControl->m_nFontSizeTemp ;
		}
	}
}

void CEcsLayout::OnZoomIn(CWnd* pWnd)
{
	CDciControl* pDciControl = NULL;
	DEBUGER_ASSERT_VALID(pWnd != NULL);
	DEBUGER_ASSERT_VALID(m_pDCI != NULL);
	CRect rect, tempRect;

//	m_nZoomCount = 0;
//	m_nZoomLimit = 30;

	CString strTemp = _T("이 이상은 확대할 수 없습니다.");

//	m_nZoomCount++;
	int nZoomSize = 6;
	int nZoomMax = 5;

	rect = m_pDCI->GetLayoutL();
	int l = rect.left+nZoomSize;	
	int b = rect.bottom+nZoomSize;	
	int r = rect.right-nZoomSize;	
	int t = rect.top-nZoomSize;		


	tempRect = m_pDCI->m_rcLayoutLTemp;
	int tempL = tempRect.left;		
	int tempB = tempRect.bottom;	
	int tempR = tempRect.right;		
	int tempT = tempRect.top;

	
	/*if ((l > tempL -10) || (r > tempR + 10) || (t > tempT +10) || (b > tempB -10))*/
	if ((r - l < tempR - (nZoomSize * nZoomMax * 2)) || (t - b < tempT - (nZoomSize * nZoomMax * 2)))
	{
		AfxMessageBox(strTemp);
		return;
	}
	// 세로(Bottom)값과 가로(Right)값 중에 작은 값보다 크거나 같을때 
	if (t < r)
	{
		//m_nZoomCount += 6;
		//if (m_nZoomCount++ >= t)
		//{
		//	m_nZoomLimit = m_nZoomCount;
		//	//strTemp.Format("이 이상은 확대할 수 없습니다.[Limit=%d][Count=%d]", m_nZoomLimit, m_nZoomCount);
		//	AfxMessageBox(strTemp);
		//	return;
		//}
	}
	else 
	{
		//m_nZoomCount += 6;
		//if (m_nZoomCount++ >= r)
		//{
		//	m_nZoomLimit = m_nZoomCount;
		//	//strTemp.Format("이 이상은 확대할 수 없습니다.[Limit=%d][Count=%d]", m_nZoomLimit, m_nZoomCount);
		//	AfxMessageBox(strTemp);
		//	return;	
		//}
	}

	m_pDCI->SetLayoutL(l, t, r, b);
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();

//	CString strTemp;
	strTemp.Format(_T("\n\nZoom Count = %d\nLeft = %d\nBottom = %d\nRight = %d\nTop = %d"), m_nZoomCount, l, t, r, b);
//	AfxMessageBox(strTemp);

	for (POSITION pos=m_pDciCtrls->GetTailPosition(); pos!=NULL; )
	{
		pDciControl = m_pDciCtrls->GetPrev(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);
				
		CString strClassName = CA2CT(pDciControl->GetRuntimeClass()->m_lpszClassName);
		if(strClassName == _T("CDciStaticCtrl"))
		{
			CDciStaticCtrl *pControl = (CDciStaticCtrl*)pDciControl;
			if(pControl != NULL)
				pControl->m_nFontSize += 1  ;

		}
	}
}

void CEcsLayout::OnZoomOut(CWnd* pWnd)
{
	CDciControl* pDciControl = NULL;
	DEBUGER_ASSERT_VALID(pWnd != NULL);
	DEBUGER_ASSERT_VALID(m_pDCI != NULL);
	CRect rect, tempRect;

	CString strTemp;
	int nZoomSize = 6;
	int nZoomMax = 5;
	rect = m_pDCI->GetLayoutL();
	int l = rect.left-nZoomSize;	
	int b = rect.bottom-nZoomSize;	
	int r = rect.right+nZoomSize;	
	int t = rect.top+nZoomSize;		


	tempRect = m_pDCI->m_rcLayoutLTemp;
	int tempL = tempRect.left;		
	int tempB = tempRect.bottom;	
	int tempR = tempRect.right;		
	int tempT = tempRect.top;

	strTemp.Format(_T("이 이상은 축소할 수 없습니다"));
	/*if ((l > tempL -10) || (r > tempR + 10) || (t > tempT +10) || (b > tempB -10))*/
	if ((r - l > tempR) || (t - b > tempT))
	{
		AfxMessageBox(strTemp);
		return;
	}

	//strTemp.Format(_T("Zoom Count = %d\nZoom Limit= %d\nLeft = %d\nBottom = %d\nRight = %d\nTop = %d"), m_nZoomCount, m_nZoomLimit, l, t, r, b);
//	if (--m_nZoomCount < 0)
//	{
//		m_nZoomCount = 0;
////		strTemp = "이 이상은 축소할 수 없습니다\n\n" + strTemp;
////		AfxMessageBox(strTemp);
//		AfxMessageBox(_T("이 이상은 축소할 수 없습니다"));
//		return;
//	}

//	AfxMessageBox(strTemp);

	m_pDCI->SetLayoutL(l, t, r, b);
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();

	for (POSITION pos=m_pDciCtrls->GetTailPosition(); pos!=NULL; )
	{
		pDciControl = m_pDciCtrls->GetPrev(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);

		CString strClassName = CA2CT(pDciControl->GetRuntimeClass()->m_lpszClassName);
		if(strClassName == _T("CDciStaticCtrl"))
		{
			CDciStaticCtrl *pControl = (CDciStaticCtrl*)pDciControl;
			if(pControl != NULL)
				pControl->m_nFontSize -= 1  ;
		}
	}
}

void CEcsLayout::OnKeyDown(CWnd* pWnd, UINT nChar, UINT nRepCnt, UINT nFlags)
{
//	if ((::GetKeyState(VK_MENU) >= 0) ||
//		(::GetKeyState(VK_SHIFT) >= 0) ||
//		(::GetKeyState(VK_CONTROL) >= 0))
//		return;

	CDciControl* pDciControl = NULL;
	DEBUGER_ASSERT_VALID(pWnd != NULL);
	DEBUGER_ASSERT_VALID(m_pDCI != NULL);
	CRect rect, tempRect;

	m_bFirst = TRUE;

	rect = m_pDCI->GetLayoutL();
	int l = rect.left;		
	int b = rect.bottom;	
	int r = rect.right;		
	int t = rect.top;		
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();

	tempRect = m_pDCI->m_rcLayoutLTemp;
	int tempL = tempRect.left;		
	int tempB = tempRect.bottom;	
	int tempR = tempRect.right;		
	int tempT = tempRect.top;		

	switch (nChar)
	{
	// 아래쪽으로 이동 
	case VK_DOWN:
		{
 			rect = m_pDCI->GetLayoutL();
			l = rect.left;		
			b = m_bFirst ? rect.bottom-2 : rect.bottom+2;	
			r = rect.right;		
			t = m_bFirst ? rect.top-2 : rect.top+2;	
			if (tempB > b)
				return;	
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;
	// 위쪽으로 이동 
	case VK_UP:
		{
			rect = m_pDCI->GetLayoutL();
			l = rect.left;		
			b = m_bFirst ? rect.bottom+2 : rect.bottom-2;
			r = rect.right;		
			t = m_bFirst ? rect.top+2 : rect.top-2;		
			if (tempT < t)
				return;	
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;
	// 오른쪽으로 이동 
	case VK_RIGHT:
		{
			rect = m_pDCI->GetLayoutL();
			l = rect.left+2;	
			b = rect.bottom;	
			r = rect.right+2;	
			t = rect.top;
			if (tempR < r)
				return;
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;
	// 왼쪽으로 이동 
	case VK_LEFT:
		{
			rect = m_pDCI->GetLayoutL();
			l = rect.left-2;	
			b = rect.bottom;	
			r = rect.right-2;	
			t = rect.top;
			if (tempL > l)
				return;
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;
	// 원래 크기로
	case VK_F9:
		{
			OnOriginalSize(pWnd);
			m_bFirst = FALSE;

			//CDciStaticCtrl* pStaticControl = NULL;
		}
		break;
	// 확대
	case VK_F11:
		{
			OnZoomIn(pWnd);
		}
		break;
	// 축소 
	case VK_F12:
		{
			OnZoomOut(pWnd);
		}
		break;

	case VK_F1:
		if (pDciControl = this->GetDciControl(_T("01100000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_F2:
		if (pDciControl = this->GetDciControl(_T("01200000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_F3:
		if (pDciControl = this->GetDciControl(_T("01300000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_F4:
		if (pDciControl = this->GetDciControl(_T("01400000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_F5:
		if (pDciControl = this->GetDciControl(_T("01500000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_F6:
		if (pDciControl = this->GetDciControl(_T("02100000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_F7:
		if (pDciControl = this->GetDciControl(_T("02200000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_F8:
		if (pDciControl = this->GetDciControl(_T("02300000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;
	}

	CString strTemp;
	strTemp.Format(_T("Zoom Count = %d\nLeft = %d\nBottom = %d\nRight = %d\nTop = %d"), m_nZoomCount, l, t, r, b);
//	AfxMessageBox(strTemp);

}

void CEcsLayout::OnViewLayoutInfo(CWnd* pWnd)
{
	CDciControl* pDciControl = NULL;
	for (POSITION pos=m_pDciCtrls->GetHeadPosition(); pos!=NULL; )
	{
		pDciControl = m_pDciCtrls->GetNext(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);

		if (!CLib::IsValidControlID(pDciControl->m_strCID))
			continue;

		if (CConvert::ToInt(pDciControl->m_strCID.Mid(3,2)) == enCV && pDciControl->m_strCID.GetLength() > 1 && pDciControl->m_strCID.Left(1) == _T("1"))
		{
			if (pDciControl->m_strText.IsEmpty())
				pDciControl->m_strText.Format(_T("%d"), /*1000*(CConvert::ToInt(pDciControl->m_strCID.Mid(1,1))-1) + */CConvert::ToInt(pDciControl->m_strCID.Right(3)));
			else
				pDciControl->m_strText.Empty();
		}

		if (CConvert::ToInt(pDciControl->m_strCID.Left(1)) == 2)
			pDciControl->m_bVisible = !pDciControl->m_bVisible;
	}

	pWnd->Invalidate();
}

BOOL CEcsLayout::LoadXml(LPCTSTR lpszFullPath, CString strKioskNo)
{
	CXmlDom dom;

	CString strTemp1, strTemp2;
	try
	{
		if (!dom.InitializeXmlDom())
		{
			m_strLog.Format(_T("%s  XML DOM 초기화 실패!"), __WFUNCTION__);
			return FALSE;
		}

		if (!dom.LoadXmlFile(lpszFullPath))
		{
			m_strLog.Format(_T("%s LAYOUT 정의파일 로드 실패! [%s]"), __WFUNCTION__, lpszFullPath);
			return FALSE;
		}

		CRect rcLayout;
		CString strValue;
		dom.GetAttrValue(_T("l"), strValue);	rcLayout.left = CConvert::ToInt(strValue);
		dom.GetAttrValue(_T("b"), strValue);	rcLayout.bottom = CConvert::ToInt(strValue);
		dom.GetAttrValue(_T("r"), strValue);	rcLayout.right = CConvert::ToInt(strValue);
		dom.GetAttrValue(_T("t"), strValue);	rcLayout.top = CConvert::ToInt(strValue);
		m_pDCI->SetLayoutL(rcLayout);

		m_pDCI->m_rcLayoutLTemp = m_pDCI->GetLayoutL();	

		if (dom.SelectElements(_T("//Control")))
		{
			int i, j, k;
			LPCSTR LPCTSTRClassName;
			CString strClassName; 
			CString strShow;
			CDciPropertyArray properties;
			CDciControl* pDciControl = NULL;

			for (i=0; i<dom.GetSelectCount(); ++i)
			{
				dom.MoveSelect(i);
				
				dom.MoveParent();
				dom.GetElmtName(strClassName);

				dom.MoveParent();
				dom.GetAttrValue(_T("show"), strShow);
				if (CConvert::ToInt(strShow) == 0)
					continue;

				dom.MoveSelect(i);

				LPCTSTRClassName = CT2A(strClassName);
				if(strClassName == _T("CDciRvCtrl"))
				{
					int a = 0;
				}
				if ((pDciControl = m_pDCI->CreateDciObject(strClassName)) == NULL)
				{
					m_strLog.Format(_T("%s 객체 생성 실패!"), strClassName);
					DEBUGER_TRACE(m_strLog, __WFUNCTION__);
					continue;
				}

				pDciControl->UpdatePropNames(properties);
				int nPropertieSize = properties.GetSize();
				for (j=0; j<nPropertieSize; ++j)
				{
					try
					{
						//strTemp1 = properties[j].m_strName;

						if (strTemp1 == _T("railcolor"))
						{
							int aaaa = 0;
						}

						strTemp1 = properties[j].m_strName;
						//dom.GetAttrValue(properties[j].m_strName, properties[j].m_strValue);		// 원본
						dom.GetAttrValue(strTemp1, properties[j].m_strValue);						// 수정본 
						
						strTemp2 = properties[j].m_strValue;

						

						//if(strTemp2 == _T("1119900020"))
						//{
						//	int aa = 0;
						//}
						//properties[8].m_strValue = _T("000000"); // 8이 bgcolor 임
						//properties[1].m_strValue // l
						//properties[2].m_strValue // b
						//properties[3].m_strValue // r
						//properties[4].m_strValue // t
					}
					catch (_com_error& e)
					{
						m_strLog.Format(_T("%s::%s=%s (%s)"), pDciControl->GetRuntimeClass()->m_lpszClassName, 
							properties[j].m_strName, properties[j].m_strValue, e.ErrorMessage());
						DEBUGER_TRACE(m_strLog, __WFUNCTION__);
					}
				}

				if (properties[0].m_strValue.Mid(3,2) == _T("99"))
				{
					if (properties[5].m_strValue == strKioskNo)
					{
						properties[8].m_strValue = KIOSK_COLOR;
					}
				}

				pDciControl->UpdatePropValues(properties);

				if (pDciControl->GetItemSize())
				{
					pDciControl->UpdateItemNames(properties);

					for (j=0; j<pDciControl->GetItemSize(); ++j)
					{
						dom.MoveChild(j);
						
						for (k=0; k<properties.GetSize(); ++k)
						{
							try
							{

								dom.GetAttrValue(properties[k].m_strName, properties[k].m_strValue);
							}
							catch (_com_error& e)
							{
								m_strLog.Format(_T("%s::%s=%s (%s)"), pDciControl->GetRuntimeClass()->m_lpszClassName, 
									properties[k].m_strName, properties[k].m_strValue, e.ErrorMessage());
								DEBUGER_TRACE(m_strLog, __WFUNCTION__);
							}
						}
						pDciControl->UpdateItemValues(j, properties);

						dom.MoveParent();
					}
				}

				if(strClassName == _T("CDciStaticCtrl"))
				{
					CDciStaticCtrl *pControl = NULL;
					pControl = (CDciStaticCtrl*)pDciControl;
					int aa = pControl->m_nFontSize;
					if(aa != 1)
						aa = 0;
					pControl->m_nFontSizeTemp = pControl->m_nFontSize ;
				}

				m_pDciCtrls->AddTail(pDciControl);
			} // for (i=0; i<dom.GetSelectCount(); ++i)
		} // if (dom.SelectElements(_T("//Control")))
	}
	catch (_com_error& e)
	{
		m_strLog.Format(_T("%s  _com_error (%s)"), __WFUNCTION__, e.ErrorMessage());
		return FALSE;
	}

	return TRUE;
}

CDciControl* CEcsLayout::GetDciControl(const CString& strCID)
{
	if (strCID.GetLength() != CLib::enLengthControlID_8)
		return NULL;

	CDciControl* pDciControl = NULL;
	for (POSITION pos=m_pDciCtrls->GetHeadPosition(); pos!=NULL; )
	{
		pDciControl = m_pDciCtrls->GetNext(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);

		if (pDciControl->m_strCID == strCID)
			return pDciControl;
	}

	return NULL;
}

BOOL CEcsLayout::Test(CWnd* pWnd, int nLeft, int nRight, int nTop, int nBottom)
{
	CDciControl* pDciControl = NULL;
	//DEBUGER_ASSERT_VALID(pWnd != NULL);
	DEBUGER_ASSERT_VALID(m_pDCI != NULL);
	CRect rect;


	rect = m_pDCI->GetLayoutL();
	int l ;
	int b ;
	int r ;
	int t ;
	//m_pDCI->RecalcLayout();
	//pWnd->Invalidate();

	rect = m_pDCI->GetLayoutL();

	int nZoomSize = 15;
	l = nRight - nZoomSize;
	r = nLeft + nZoomSize;
	t = nBottom +  nZoomSize;
	b = nTop - nZoomSize;
	m_pDCI->SetLayoutL(l, t, r, b);
	m_pDCI->RecalcLayout();
	//pWnd->Invalidate();


	//OnZoomIn(pWnd);
	return true;
}