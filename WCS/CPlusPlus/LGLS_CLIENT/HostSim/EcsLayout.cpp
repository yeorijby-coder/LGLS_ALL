// EcsLayout.cpp: implementation of the CEcsLayout class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "EcsLayout.h"
#include "XmlDom.h"

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
	m_bShowText = TRUE;
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

void CEcsLayout::OnDraw(CWnd* pWnd, CDC* pDC)
{
	CFont* pOldFont = pDC->SelectObject(&m_oFont);

	for (POSITION pos=m_pDciCtrls->GetHeadPosition(); pos!= NULL; )
		m_pDciCtrls->GetNext(pos)->UpdateControl(pDC);

	pDC->SelectObject(pOldFont);
}

void CEcsLayout::OnSize(CWnd* pWnd, UINT nType, int cx, int cy)
{
	if ((m_pDCI == NULL) || (m_pDciCtrls == NULL))
		return;

	m_pDCI->SetLayoutS(0, 0, cx, cy);
	m_pDCI->RecalcLayout();
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

void CEcsLayout::OnKeyDown(CWnd* pWnd, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((::GetKeyState(VK_MENU) >= 0) ||
		(::GetKeyState(VK_SHIFT) >= 0) ||
		(::GetKeyState(VK_CONTROL) >= 0))
		return;

	CDciControl* pDciControl = NULL;
	DEBUGER_ASSERT_VALID(pWnd != NULL);
	DEBUGER_ASSERT_VALID(m_pDCI != NULL);

	switch (nChar)
	{
	case VK_F12:
		{
			int l = ::GetPrivateProfileInt(_T("LAYOUT"), _T("L"), 0, ECS_INI_FILE);
			int b = ::GetPrivateProfileInt(_T("LAYOUT"), _T("B"), 0, ECS_INI_FILE);
			int r = ::GetPrivateProfileInt(_T("LAYOUT"), _T("R"), 120, ECS_INI_FILE);
			int t = ::GetPrivateProfileInt(_T("LAYOUT"), _T("T"), 60, ECS_INI_FILE);
			m_pDCI->SetLayoutL(l, t, r, b);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
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

	case VK_F9:
		if (pDciControl = this->GetDciControl(_T("02400000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_F10:
		if (pDciControl = this->GetDciControl(_T("02500000")))
		{
			m_pDCI->SetLayoutL(pDciControl->m_rcControlL);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;

	case VK_F11:
		if (pDciControl = this->GetDciControl(_T("00600000")))
		{
			CRect rcRect = pDciControl->m_rcControlL;
			rcRect.top -= 15;
			rcRect.bottom += 15;

			m_pDCI->SetLayoutL(rcRect);
			m_pDCI->RecalcLayout();
			pWnd->Invalidate();
		}
		break;
	}
}

CDciControl* CEcsLayout::GetRgvPosition(int nRgvNum, int nPos)
{
	CDciControl* pDciControl = NULL;
	for (POSITION pos=m_pDciCtrls->GetHeadPosition(); pos!=NULL; )
	{
		pDciControl = m_pDciCtrls->GetNext(pos);
		DEBUGER_ASSERT_VALID(pDciControl != NULL);

		if (!CLib::IsValidControlID(pDciControl->m_strCID))
			continue;

		if (_ttoi(pDciControl->m_strCID.Mid(3,2)) != enRTVPOS)
			continue;
		
		if (_ttoi(pDciControl->m_strCID.Mid(5,2)) == nRgvNum &&
			_ttoi(pDciControl->m_strCID.Mid(7,2)) == nPos)
		{
			return pDciControl;
		}
	}

	return NULL;
}


void CEcsLayout::OnViewLayoutInfo(CWnd* pWnd, int nFontSize)
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
			if (pDciControl->m_strText.IsEmpty() )
			{
				pDciControl->m_strText.Format(_T("%d"), _ttoi(pDciControl->m_strCID.Right(4)));
				CDciTrackCtrl* pDciTrackCtrl = (CDciTrackCtrl*) pDciControl;

				if (pDciTrackCtrl == NULL)
					continue;

				pDciTrackCtrl->m_nFontSize = nFontSize;

				m_bShowText = TRUE;
			}
			else
			{
				
				pDciControl->m_strText.Empty();
				m_bShowText = FALSE;
			}

			if (_ttoi(pDciControl->m_strCID.Mid(5,1)) == 4)
			{
				int nCID = _ttoi(pDciControl->m_strCID.Right(3));

				switch(nCID)
				{
				case 101:		pDciControl->m_strText = "SC #1#2";			break;
				case 102:		pDciControl->m_strText = "SC #3#4";			break;
				case 103:		pDciControl->m_strText = "SC #5#6";			break;
				case 104:		pDciControl->m_strText = "1G PLC";			break;
				case 105:		pDciControl->m_strText = "2G PLC";			break;
				case 106:		pDciControl->m_strText = "3G PLC";			break;
				case 107:		pDciControl->m_strText = "RGVC #1";			break;
				case 108:		pDciControl->m_strText = "RGVC #2";			break;
				case 109:		pDciControl->m_strText = "FENCE #1";		break;
				case 110:		pDciControl->m_strText = "FENCE #2";		break;
				}
			}
		}

		if (_ttoi(pDciControl->m_strCID.Left(1)) == 2)
			pDciControl->m_bVisible = !pDciControl->m_bVisible;
	}

	pWnd->Invalidate();
}

void CEcsLayout::OnViewLuggNum(CWnd* pWnd, int nFontSize, int LuggNum)
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

				pDciControl->m_strText.Format(_T("%d"), _ttoi(pDciControl->m_strCID.Right(4)));
				CDciTrackCtrl* pDciTrackCtrl = (CDciTrackCtrl*) pDciControl;

				if (pDciTrackCtrl == NULL)
					continue;

				pDciTrackCtrl->m_nFontSize = nFontSize;

		}
		
		if (_ttoi(pDciControl->m_strCID.Left(1)) == 2)
			pDciControl->m_bVisible = !pDciControl->m_bVisible;
	}
	
	pWnd->Invalidate();
}

BOOL CEcsLayout::LoadXml(LPCTSTR lpszFullPath)
{
	CXmlDom dom;

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
		m_pDCI->SetLayoutL(rcLayout);

		if (dom.SelectElements(_T("//Control")))
		{
			int i, j, k;
			CString strClassName, strShow, strGroupName;
			CDciPropertyArray properties;
			CDciControl* pDciControl = NULL;

			for (i=0; i<dom.GetSelectCount(); ++i)
			{
				dom.MoveSelect(i);
				
				dom.MoveParent();
				dom.GetElmtName(strClassName);
				
				dom.MoveParent();
				dom.GetAttrValue(_T("show"), strShow);
				if (_ttoi(strShow) == 0)
					continue;

				dom.GetAttrValue(_T("gname"), strGroupName);

				dom.MoveSelect(i);

				if ((pDciControl = m_pDCI->CreateDciObject(strClassName)) == NULL)
				{
					m_strLog.Format(_T("%s 객체 생성 실패!"), strClassName);
					DEBUGER_TRACE(m_strLog, _T("CEcsLayout::LoadXml"));
					continue;
				}

				pDciControl->UpdatePropNames(properties);
				for (j=0; j<properties.GetSize(); ++j)
				{
					try
					{
						dom.GetAttrValue(properties[j].m_strName, properties[j].m_strValue);
					}
					catch (_com_error& e)
					{
						m_strLog.Format(_T("%s::%s=%s (%s)"), pDciControl->GetRuntimeClass()->m_lpszClassName, 
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
