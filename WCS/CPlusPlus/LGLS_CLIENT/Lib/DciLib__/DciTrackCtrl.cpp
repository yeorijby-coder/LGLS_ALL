// DciTrackCtrl.cpp: implementation of the CDciTrackCtrl class.
//

#include "StdAfx.h"
#include "DciTrackCtrl.h"


IMPLEMENT_SERIAL(CDciTrackCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

CDciTrackCtrl::CDciTrackCtrl(void)
{
//	m_bExist = FALSE;
}

CDciTrackCtrl::~CDciTrackCtrl(void)
{
}

// CDciTrackCtrl 멤버 함수
void CDciTrackCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);
	m_clrBgColor = ::GetSysColor(COLOR_BTNFACE);
	
	m_nFontSize = 1;
	m_nRow = 0;
	m_nCol = 0;
	m_bDoubleExist = 0;
}

int CDciTrackCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enTrackCtrlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("fontsize"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("prod"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("prod2"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("row"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("col"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciTrackCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
		//m_nFontSize	= CConvert::ToInt(properties[i++].m_strValue);
		m_nFontSize	= CConvert::ToInt(properties[i++].m_strValue);
		m_bExist = CConvert::ToInt(properties[i++].m_strValue);
		m_bDoubleExist = CConvert::ToInt(properties[i++].m_strValue);
		m_nRow = CConvert::ToInt(properties[i++].m_strValue);
		m_nCol = CConvert::ToInt(properties[i++].m_strValue);
		m_items.SetSize(m_nRow * m_nCol);
	}
	else
	{
		properties[i++].m_strValue.Format(_T("%d"), m_nFontSize);
		properties[i++].m_strValue.Format(_T("%d"), m_bExist);
		properties[i++].m_strValue.Format(_T("%d"), m_bDoubleExist);
		properties[i++].m_strValue.Format(_T("%d"), m_nRow);
		properties[i++].m_strValue.Format(_T("%d"), m_nCol);
	}

	return i;
}

int CDciTrackCtrl::UpdateItemNames(CDciPropertyArray& properties)
{
	int i = 0;

	properties.SetSize(6);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("shape"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("color"));
	properties[i++].SetProperty(CDciProperty::PT_PERCENT, _T("gl"));
	properties[i++].SetProperty(CDciProperty::PT_PERCENT, _T("gt"));
	properties[i++].SetProperty(CDciProperty::PT_PERCENT, _T("gr"));
	properties[i++].SetProperty(CDciProperty::PT_PERCENT, _T("gb"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciTrackCtrl::UpdateItemValues(int nIndex, CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	if ((nIndex < 0) || (nIndex >= m_items.GetSize()))
		return 0;

	int i = 0;

	if (bSaveObject)
	{
		m_items[nIndex].m_nShape		= CConvert::ToInt(properties[i++].m_strValue);
		m_items[nIndex].m_clrItem		= CConvert::ToColor(properties[i++].m_strValue);
		m_items[nIndex].m_rcGap.left	= CConvert::ToInt(properties[i++].m_strValue);
		m_items[nIndex].m_rcGap.top		= CConvert::ToInt(properties[i++].m_strValue);
		m_items[nIndex].m_rcGap.right	= CConvert::ToInt(properties[i++].m_strValue);
		m_items[nIndex].m_rcGap.bottom	= CConvert::ToInt(properties[i++].m_strValue);
	}
	else
	{
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_nShape);
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_items[nIndex].m_clrItem));
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_rcGap.left);
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_rcGap.top);
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_rcGap.right);
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_rcGap.bottom);
	}

	return i;
}

void CDciTrackCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	try
	{
		int g = 2, s = 4;
		CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
		m_pDCI->DrawButton(pDC, m_rcControlL, m_clrBgColor, m_bClick);

		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);

		LOGFONT m_logfont;
		CFont m_font ;
		CFont * pOldFont;

		int nSize = rcControlS.Height();
		if (rcControlS.Height() > rcControlS.Width())
			nSize = rcControlS.Width();

		nSize = (nSize / 2) -  (nSize / 12);

		HDC hDC;
		hDC = pDC->m_hDC;
		hDC = pDC->GetSafeHdc();
		memset(&m_logfont, 0, sizeof(LOGFONT));
		m_logfont.lfQuality = PROOF_QUALITY;
//		m_logfont.lfHeight = m_nFontSize;
		m_logfont.lfHeight = nSize;//long((-MulDiv(m_nFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 )) / m_pDCI->GetScale());
		m_logfont.lfWeight = FW_HEAVY;
		lstrcpy(m_logfont.lfFaceName, _T("Arial"));
		m_font.CreateFontIndirect(&m_logfont);
		pOldFont = pDC->SelectObject(&m_font);

		/*
		// nFontSize            : 일반 크기(pt)
		// LOGFONT::lfHeight    : 논리 크기
 
		// 일반 크기(pt) 를 논리 크기로 변환
		LOGFONT::lfHeight = -MulDiv( nFontSize, GetDeviceCaps( pDC, LOGPIXELSY ), 72 );
		// 논리크기 = -((일반크기 * GetDeviceCaps(pDC, LOGPIXELSY)) / 72)
 
		//논리 크기를 일반 크기로 변환
		nFontSize = -MulDiv( LOGFONT::lfHeight, 72, GetDeviceCaps( pDC, LOGPIXELSY ) );
		// 일반크기 = -((논리크기 * 72) / GetDeviceCaps(pDC, LOGPIXELSY))
		//*/

		pDC->DrawText(m_strText, rcControlS, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		if (m_bExist)
		{
			pDC->FillSolidRect(rcControlS.left+g, rcControlS.top+g, s, s, 0x000000);
			pDC->FillSolidRect(rcControlS.right-g-s, rcControlS.top+g, s, s, 0x000000);
			pDC->FillSolidRect(rcControlS.left+g, rcControlS.bottom-g-s, s, s, 0x000000);
			pDC->FillSolidRect(rcControlS.right-g-s, rcControlS.bottom-g-s, s, s, 0x000000);
		}


		int nSelectValue = abs(rcControlS.Height());
		if (abs(rcControlS.Width()) < abs(rcControlS.Height()))
			nSelectValue = abs(rcControlS.Width());

		float nRatio = 25.0;
		float fTemp = nRatio / 100.0;
		int nMinusRatioValue = int(fTemp * float(nSelectValue));
		int nL = rcControlS.left;
		int nR = rcControlS.right;
		int nT = rcControlS.top;
		int nB = rcControlS.bottom;
		int nX = rcControlS.left + int(abs(rcControlS.Width()) / 2) - nMinusRatioValue;
		int nY = rcControlS.top + int(abs(rcControlS.Height()) / 2) - nMinusRatioValue;

		CString strTemp11, strTemp22 = _T("");
		strTemp11.Format(_T("[Control W & H]   W=%d,   H=%d \n"), abs(rcControlS.Width()), abs(rcControlS.Height()));	strTemp22 += strTemp11;
		strTemp11.Format(_T("[Control R & L & T & B] R=%d, L=%d T=%d, B=%d \n"), nR, nL, nT, nB);						strTemp22 += strTemp11;
//		strTemp11.Format(_T("[Control MRV] MRV=%d \n"), nMinusRatioValue);												strTemp22 += strTemp11;
		strTemp11.Format(_T("[Control] Sel=%d X=%d, Y=%d MRV=%d \n "), nSelectValue, nX, nY, nMinusRatioValue);			strTemp22 += strTemp11;

		if (m_bDoubleExist)
		{
//			AfxMessageBox(strTemp22); 
			
  			pDC->FillSolidRect(nX, nY, nMinusRatioValue * 2, nMinusRatioValue * 2, 0x000000);
		}

		if ((m_nRow * m_nCol == 0) || (m_items.GetSize() == 0))
			return;

		CRect rcUnitS;
		int nUnitX = abs(rcControlS.Width()) / m_nCol;
		int nUnitY = abs(rcControlS.Height()) / m_nRow;
		
		for (int i=0; i<m_nRow; ++i)
		{
			for (int j=0; j<m_nCol; ++j)
			{
				rcUnitS.left = rcControlS.left + (j) * nUnitX;
				rcUnitS.top = rcControlS.top + (i) * nUnitY;
				rcUnitS.right = rcControlS.left + (j+1) * nUnitX;
				rcUnitS.bottom = rcControlS.top + (i+1) * nUnitY;

				CDciTrackItem& item = m_items[i * m_nCol + j];
				CBrush brush(item.m_clrItem);

				rcUnitS.left += nUnitX * item.m_rcGap.left / 100;
				rcUnitS.top += nUnitY * item.m_rcGap.top / 100;
				rcUnitS.right -= nUnitX * item.m_rcGap.right / 100;
				rcUnitS.bottom -= nUnitY * item.m_rcGap.bottom / 100;
				rcUnitS.DeflateRect(0, 0, 1, 1);

				switch (item.m_nShape)
				{
				case CDciTrackItem::enShapeRectangle:
					{
						pDC->FillRect(rcUnitS, &brush);
//						pDC->Draw3dRect(rcUnitS, RGB(0,0,0), RGB(255,255,255));
					}
					break;

				case CDciTrackItem::enShapeToLeft:
					{
						CPoint ptBound[3];
						ptBound[0].x = rcUnitS.right;
						ptBound[0].y = rcUnitS.top;
						ptBound[1].x = rcUnitS.right;
						ptBound[1].y = rcUnitS.bottom;
						ptBound[2].x = rcUnitS.left;
						ptBound[2].y = rcUnitS.top + abs(rcUnitS.Height()) / 2;

						CRgn rgnBound;
						rgnBound.CreatePolygonRgn(ptBound, sizeof(ptBound)/sizeof(ptBound[0]), WINDING);
						pDC->FillRgn(&rgnBound, &brush);
					}
					break;

				case CDciTrackItem::enShapeToRight:
					{
						CPoint ptBound[3];
						ptBound[0].x = rcUnitS.left;
						ptBound[0].y = rcUnitS.top;
						ptBound[1].x = rcUnitS.left;
						ptBound[1].y = rcUnitS.bottom;
						ptBound[2].x = rcUnitS.right;
						ptBound[2].y = rcUnitS.top + abs(rcUnitS.Height()) / 2;

						CRgn rgnBound;
						rgnBound.CreatePolygonRgn(ptBound, sizeof(ptBound)/sizeof(ptBound[0]), WINDING);
						pDC->FillRgn(&rgnBound, &brush);
					}
					break;

				case CDciTrackItem::enShapeToUp:
					{
						CPoint ptBound[3];
						ptBound[0].x = rcUnitS.left;
						ptBound[0].y = rcUnitS.bottom;
						ptBound[1].x = rcUnitS.right;
						ptBound[1].y = rcUnitS.bottom;
						ptBound[2].x = rcUnitS.left + abs(rcUnitS.Width()) / 2;
						ptBound[2].y = rcUnitS.top;

						CRgn rgnBound;
						rgnBound.CreatePolygonRgn(ptBound, sizeof(ptBound)/sizeof(ptBound[0]), WINDING);
						pDC->FillRgn(&rgnBound, &brush);
					}
					break;
					
				case CDciTrackItem::enShapeToDown:
					{
						CPoint ptBound[3];
						ptBound[0].x = rcUnitS.left;
						ptBound[0].y = rcUnitS.top;
						ptBound[1].x = rcUnitS.right;
						ptBound[1].y = rcUnitS.top;
						ptBound[2].x = rcUnitS.left + abs(rcUnitS.Width()) / 2;
						ptBound[2].y = rcUnitS.bottom;

						CRgn rgnBound;
						rgnBound.CreatePolygonRgn(ptBound, sizeof(ptBound)/sizeof(ptBound[0]), WINDING);
						pDC->FillRgn(&rgnBound, &brush);
					}
					break;
				}
			}
		}

		if (m_bExist)
		{
			pDC->FillSolidRect(rcControlS.left+g, rcControlS.top+g, s, s, 0x000000);
			pDC->FillSolidRect(rcControlS.right-g-s, rcControlS.top+g, s, s, 0x000000);
			pDC->FillSolidRect(rcControlS.left+g, rcControlS.bottom-g-s, s, s, 0x000000);
			pDC->FillSolidRect(rcControlS.right-g-s, rcControlS.bottom-g-s, s, s, 0x000000);
		}


		nSelectValue = abs(rcControlS.Height());
		if (abs(rcControlS.Width()) < abs(rcControlS.Height()))
			nSelectValue = abs(rcControlS.Width());

		nRatio = 25.0;
		fTemp = nRatio / 100.0;
		nMinusRatioValue = int(fTemp * float(nSelectValue));
		nL = rcControlS.left;
		nR = rcControlS.right;
		nT = rcControlS.top;
		nB = rcControlS.bottom;
		nX = rcControlS.left + int(abs(rcControlS.Width()) / 2) - nMinusRatioValue;
		nY = rcControlS.top + int(abs(rcControlS.Height()) / 2) - nMinusRatioValue;

		strTemp11 = _T("");
		strTemp22 = _T("");
		strTemp11.Format(_T("[Control W & H]   W=%d,   H=%d \n"), abs(rcControlS.Width()), abs(rcControlS.Height()));	strTemp22 += strTemp11;
		strTemp11.Format(_T("[Control R & L & T & B] R=%d, L=%d T=%d, B=%d \n"), nR, nL, nT, nB);						strTemp22 += strTemp11;
//		strTemp11.Format(_T("[Control MRV] MRV=%d \n"), nMinusRatioValue);												strTemp22 += strTemp11;
		strTemp11.Format(_T("[Control] Sel=%d X=%d, Y=%d MRV=%d \n "), nSelectValue, nX, nY, nMinusRatioValue);			strTemp22 += strTemp11;

		if (m_bDoubleExist)
		{
			//AfxMessageBox(strTemp22); 
  			pDC->FillSolidRect(nX, nY, nMinusRatioValue * 2, nMinusRatioValue * 2, 0x000000);
		}

		pDC->DrawText(m_strText, rcControlS, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldBkMode);
		pDC->SelectObject(pOldFont);
	}
	catch (CException* e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciControl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
}
