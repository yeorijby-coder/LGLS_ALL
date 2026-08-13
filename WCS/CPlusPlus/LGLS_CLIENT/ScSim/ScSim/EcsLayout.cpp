// EcsLayout.cpp: implementation of the CEcsLayout class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "EcsLayout.h"
#include "XmlDom.h"
#include "DciStaticCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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

void CEcsLayout::OnInitialUpdate(CWnd* pWnd)
{
	if ((pWnd == NULL) || (pWnd->GetSafeHwnd() == NULL))
		return;

	m_nFontSize = ::GetPrivateProfileInt(_T("LAYOUT"), _T("FontSize"), 10, ECS_INI_FILE);
	if (!m_oFont.CreateFont(m_nFontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 1, 0, 0, 0, 0, _T("Arial")))
	{
		DEBUGER_ASSERT_VALID_LOG(FALSE, _T("CEcsLayout::OnInitialUpdate... 폰트생성 실패!"));
		return;
	}

	CRect rcClient;
	pWnd->GetClientRect(rcClient);
	m_pDCI->SetLayoutS(rcClient);
//	m_pDCI->SetLayoutL(9, 30, 29, 0);
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();
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

void CEcsLayout::OnSize(CWnd* pWnd, UINT nType, int cx, int cy)
{
	if ((m_pDCI == NULL) || (m_pDciCtrls == NULL))
		return;

	m_pDCI->SetLayoutS(0, 0, cx, cy);
	m_pDCI->RecalcLayout();
	pWnd->Invalidate();
}

void CEcsLayout::OnButtonDown(CWnd* pWnd, UINT nFlag, const CPoint& point)
{
	CDciControl* pDciControl = NULL;
	for (POSITION pos=m_pDciCtrls->GetTailPosition(); pos!= NULL; )
	{
		pDciControl = m_pDciCtrls->GetPrev(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);

		if (pDciControl->m_bVisible && pDciControl->SetClick(pWnd, point))
			return;
	}
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
				pWnd->PostMessage(WM_USER_CONTROL_CLICK, _ttoi(pDciControl->m_strCID));
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
				
		CString strClassName = (pDciControl->GetRuntimeClass()->m_lpszClassName);
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
	CRect rect;

//	m_nZoomCount = 0;
//	m_nZoomLimit = 30;

	CString strTemp = _T("이 이상은 확대할 수 없습니다.");

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
			//strTemp.Format(_T("이 이상은 확대할 수 없습니다.[Limit=%d][Count=%d]"), m_nZoomLimit, m_nZoomCount);
			AfxMessageBox(strTemp);
			return;
		}
	}
	else 
	{
		if (m_nZoomCount++ >= r)
		{
			m_nZoomLimit = m_nZoomCount;
			//strTemp.Format(_T("이 이상은 확대할 수 없습니다.[Limit=%d][Count=%d]"), m_nZoomLimit, m_nZoomCount);
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

	for (POSITION pos=m_pDciCtrls->GetTailPosition(); pos!=NULL; )
	{
		pDciControl = m_pDciCtrls->GetPrev(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);
				
		CString strClassName = (pDciControl->GetRuntimeClass()->m_lpszClassName);
		if(strClassName == _T("CDciStaticCtrl"))
		{
			CDciStaticCtrl *pControl = (CDciStaticCtrl*) pDciControl;
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

	for (POSITION pos=m_pDciCtrls->GetTailPosition(); pos!=NULL; )
	{
		pDciControl = m_pDciCtrls->GetPrev(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);

		CString strClassName = (pDciControl->GetRuntimeClass()->m_lpszClassName);
		if(strClassName == _T("CDciStaticCtrl"))
		{
			CDciStaticCtrl *pControl = (CDciStaticCtrl*) pDciControl;
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
	CRect rect;

	m_bFirst = TRUE;
	//m_pDCI->SetLayoutL(0, 118, 180, 0);
	rect = m_pDCI->GetLayoutL();
	int l = rect.left;		
	int b = rect.bottom;	
	int r = rect.right;		
	int t = rect.top;		
//	m_pDCI->RecalcLayout();
	pWnd->Invalidate();

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
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;
	// 오른쪽으로 이동 
	case VK_RIGHT:
		{
			rect = m_pDCI->GetLayoutL();
			l = rect.left+3;	
			b = rect.bottom;	
			r = rect.right+3;	
			t = rect.top;		
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;
	// 왼쪽으로 이동 
	case VK_LEFT:
		{
			rect = m_pDCI->GetLayoutL();
			l = rect.left-3;	
			b = rect.bottom;	
			r = rect.right-3;	
			t = rect.top;		
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


//	case VK_F10:
//		if (pDciControl = this->GetDciControl(_T("02500000")))
//		{
//			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
//			m_pDCI->RecalcLayout();
//			pWnd->Invalidate();
//		}
//		break;
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

		if (_ttoi(pDciControl->m_strCID.Mid(3,2)) == enCV)
		{
			if (pDciControl->m_strText.IsEmpty())
			{
				pDciControl->m_strText.Format(_T("%d"), 1000*(_ttoi(pDciControl->m_strCID.Mid(1,1))-1) + _ttoi(pDciControl->m_strCID.Right(3)));
			}
			else if (_ttoi(pDciControl->m_strText) != 0)
			{
				pDciControl->m_strText.Empty();
			}
		}

		if (_ttoi(pDciControl->m_strCID.Left(1)) == 2)
		{
		
		}
		//	pDciControl->m_bVisible = !pDciControl->m_bVisible;
	}

	pWnd->Invalidate();
}

BOOL CEcsLayout::LoadXml(LPCTSTR lpszFullPath)
{
	CXmlDom dom;

	CString strTemp1, strTemp2;
	try
	{
		if (!dom.InitializeXmlDom())
		{
			m_strLog.Format(_T("[CEcsLayout::LoadXml] XML DOM 초기화 실패!"));
			return FALSE;
		}
		
		if (!dom.LoadXmlFile(lpszFullPath))
		{
			m_strLog.Format(_T("[CEcsLayout::LoadXml] LAYOUT 정의파일 로드 실패! [%s]"), lpszFullPath);
			return FALSE;
		}
		
		CRect rcLayout;
		CString strValue;
		dom.GetAttrValue(_T("l"), strValue);	rcLayout.left = _ttoi(strValue);
		dom.GetAttrValue(_T("b"), strValue);	rcLayout.bottom = _ttoi(strValue);
		dom.GetAttrValue(_T("r"), strValue);	rcLayout.right = _ttoi(strValue);
		dom.GetAttrValue(_T("t"), strValue);	rcLayout.top = _ttoi(strValue);
		dom.GetAttrValue(_T("text"), strValue);	m_strText = strValue;
		dom.GetAttrValue(_T("id"), strValue);	int nPlcCount = _ttoi(strValue);
		
		// m_nRepresentPlc가 제일 처음 Layout에는 0이어야 하고
		m_nRepresentPlc = g_nPlcCount;

		if (nPlcCount < 1)
		{
			m_strLog.Format(_T("[CEcsLayout::LoadXml] Layout 당 1개의 SC PLC는 있어야 합니다.\n<Builder> Tag의 id항목을 확인하세요^^\n파일명 : %s"), lpszFullPath);
			AfxMessageBox(m_strLog);
			return FALSE;
		}

		// m_nRepresentPlcr가 두번째 부터가 n*x가 되어야 하므로
		g_nPlcCount += nPlcCount;

		m_pDCI->SetLayoutL(rcLayout);

		m_pDCI->m_rcLayoutLTemp = m_pDCI->GetLayoutL();	

		if (dom.SelectElements(_T("//Control")))
		{
			int i, j, k;
			//LPCSTR LPCTSTRClassName;
			CString strClassName; 
			CString strShow;
			CDciPropertyArray properties;
			CDciControl* pDciControl = NULL;

			int cnt = dom.GetSelectCount();
			int a=0;

			for (i=0; i<cnt; ++i)
			{
				dom.MoveSelect(i);
				
				dom.MoveParent();
				dom.GetElmtName(strClassName);
				
				dom.MoveParent();
				dom.GetAttrValue(_T("show"), strShow);
				if (_ttoi(strShow) == 0)
					continue;

				dom.MoveSelect(i);

				if ((pDciControl = m_pDCI->CreateDciObject(strClassName)) == NULL)
				{
					m_strLog.Format(_T("%s 객체 생성 실패!"), strClassName);
					DEBUGER_TRACE(m_strLog, _T("CEcsLayout::LoadXml"));
					continue;
				}

				if (strClassName == _T("CDciRvCtrl"))
					int aaaaaa = 0;

				pDciControl->UpdatePropNames(properties);
				int nPropertieSize = properties.GetSize();
				for (j=0; j<nPropertieSize; ++j)
				{
					try
					{
						dom.GetAttrValue(properties[j].m_strName, properties[j].m_strValue);

						strTemp1 = properties[j].m_strName;
						strTemp2 = properties[j].m_strValue;

						if (strTemp1 == _T("railcolor"))
							int aaaaaa = 0;

					}
					catch (_com_error& e)
					{
						m_strLog.Format(_T("%s::%s=%s (%s)"), (LPCWSTR)pDciControl->GetRuntimeClass()->m_lpszClassName, 
							properties[j].m_strName, properties[j].m_strValue, e.ErrorMessage());
						DEBUGER_TRACE(m_strLog, _T("CEcsLayout::LoadXml"));
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
								DEBUGER_TRACE(m_strLog, _T("CEcsLayout::LoadXml"));
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
		m_strLog.Format(_T("[CEcsLayout::LoadXml] _com_error (%s)"), e.ErrorMessage());
		return FALSE;
	}

	return TRUE;
}
CDciControl* CEcsLayout::GetDciControl(const CString& strCID)
{
	if (strCID.GetLength() != CLib::enLengthControlID)
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
