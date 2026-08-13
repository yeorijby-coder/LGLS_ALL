// DciButtonCtrl.cpp: implementation of the CDciButtonCtrl class.
//

#include "StdAfx.h"
#include "DciButtonCtrl.h"


IMPLEMENT_SERIAL(CDciButtonCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

// CDciButtonCtrl
CDciButtonCtrl::CDciButtonCtrl(void)
{
}

CDciButtonCtrl::~CDciButtonCtrl(void)
{
}

// CDciButtonCtrl ¸â¹ö ÇÔ¼ö
void CDciButtonCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);
	
	m_clrBgColor = ::GetSysColor(COLOR_BTNFACE);
//	m_nFontSize = 1;
	m_nShape = enBcsRectangle;
}

int CDciButtonCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enButtonCtrlPropSize);
//	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("fontsize"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("shape"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciButtonCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
//		m_nFontSize	= CConvert::ToInt(properties[i++].m_strValue);
		m_nShape = CConvert::ToInt(properties[i++].m_strValue);
	}
	else
	{
//		properties[i++].m_strValue.Format(_T("%d"), m_nFontSize);
		properties[i++].m_strValue.Format(_T("%d"), m_nShape);
	}

	return i;
}

void CDciButtonCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	try
	{
		switch (m_nShape)
		{
		case enBcsRectangle:									// 0
			m_pDCI->DrawButton(pDC, m_rcControlL, m_clrBgColor, m_bClick);
			break;

		case enBcsEllipse:										// 1
			{
		
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				int nGap = (rcControlS.Width() < rcControlS.Height()) ? rcControlS.Width()/10 : rcControlS.Height()/10;
			
				CBrush bgBrush(m_clrBgColor);
				CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
				CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

				rcControlS.DeflateRect(nGap, nGap, nGap, nGap);
				rcControlS.DeflateRect(0, 0, 1, 1);
				pDC->SelectStockObject(m_bClick ? BLACK_BRUSH : WHITE_BRUSH);
				pDC->Ellipse(rcControlS);
				rcControlS.InflateRect(0, 0, 1, 1);

				rcControlS.DeflateRect(1, 1, 0, 0);
				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : BLACK_BRUSH);
				pDC->Ellipse(rcControlS);
				rcControlS.InflateRect(1, 1, 0, 0);

				rcControlS.DeflateRect(1, 1, 1, 1);
				pDC->SelectObject(&bgBrush);
				pDC->Ellipse(rcControlS);

				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pOldPen);
	
			}
			break;

		case enBcsRoundRect:									// 2
			{
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				int nGap = (rcControlS.Width() < rcControlS.Height()) ? rcControlS.Width()/10 : rcControlS.Height()/10;
				CPoint ptRound(nGap*5, nGap*5);

				CBrush bgBrush(m_clrBgColor);
				CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
				CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

				rcControlS.DeflateRect(nGap, nGap, nGap, nGap);

				rcControlS.DeflateRect(0, 0, 1, 1);
				pDC->SelectStockObject(m_bClick ? BLACK_BRUSH : WHITE_BRUSH);
				pDC->RoundRect(rcControlS, ptRound);
				rcControlS.InflateRect(0, 0, 1, 1);

				rcControlS.DeflateRect(1, 1, 0, 0);
				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : BLACK_BRUSH);
				pDC->RoundRect(rcControlS, ptRound);
				rcControlS.InflateRect(1, 1, 0, 0);

				rcControlS.DeflateRect(1, 1, 1, 1);
				pDC->SelectObject(&bgBrush);
				pDC->RoundRect(rcControlS, ptRound);

				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pOldPen);
			}
			break;
		
		//==> »ï°¢Çü 4¹æÇâ Ãß°¡ -  ÃÖ¿øºó ¼öÁ¤
		case enBcsRectangleToLeft:								// 3
			{
				CPoint Pt[3];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[1] = CPoint(rcControlS.left+2,  rcControlS.top+2);
				Pt[2] = CPoint(rcControlS.left+2,  rcControlS.bottom-2);
				//Å×µÎ¸® »ö»ó ÁöÁ¤ÇÒ ½Ã..
//				CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);
				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,3);

//				pDC->SelectObject(pOldPen);	
				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,3);
			}
			break;
		case enBcsRectangleToRight:								// 4
			{
				CPoint Pt[3];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[1] = CPoint(rcControlS.right-2, rcControlS.bottom-2);
				Pt[2] = CPoint(rcControlS.left+2,  rcControlS.top+2);

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,3);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,3);
			}
			break;
	
		case enBcsRectangleBoLeft:								// 5
			{
				CPoint Pt[3];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.right-2, rcControlS.bottom-2);
				Pt[1] = CPoint(rcControlS.left+2,  rcControlS.top+2);
				Pt[2] = CPoint(rcControlS.left+2,  rcControlS.bottom-2);

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,3);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,3);
			}
			break;
	
		case enBcsRectangleBoRight:								// 6
			{
				CPoint Pt[3];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[1] = CPoint(rcControlS.left+2,  rcControlS.bottom-2);
				Pt[2] = CPoint(rcControlS.right-2,  rcControlS.bottom-2);

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,3);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,3);
			}
			break;
//	enBcsIsoscelesTriangleToLeft, enBcsIsoscelesTriangleToRight, enBcsIsoscelesTriangleToTop, enBcsIsoscelesTriangleToBottom, 
		case enBcsIsoscelesTriangleToLeft:						// 7
			{
				CPoint Pt[3];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.left+2, rcControlS.top+2);
				Pt[1] = CPoint(rcControlS.left+2, rcControlS.bottom-2);
				Pt[2] = CPoint(rcControlS.right-2, rcControlS.top + int(rcControlS.Height() / 2 ));

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,3);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,3);
			}
			break;
		case enBcsIsoscelesTriangleToRight:						// 8
			{
				CPoint Pt[3];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[1] = CPoint(rcControlS.right-2, rcControlS.bottom-2);
				Pt[2] = CPoint(rcControlS.left+2, rcControlS.top + int(rcControlS.Height() / 2 ));

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,3);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,3);
			}
			break;
		case enBcsIsoscelesTriangleToTop:						// 9
			{
				CPoint Pt[3];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.left+2, rcControlS.bottom-2);
				Pt[1] = CPoint(rcControlS.right-2, rcControlS.bottom-2);
				Pt[2] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top+2);
					
//				CString strTemp1, strTemp2 = _T("");
//				strTemp1.Format(_T("[Point 1] X=%d, Y=%d \n"), Pt[0].x, Pt[0].y);		strTemp2 += strTemp1;
//				strTemp1.Format(_T("[Point 2] X=%d, Y=%d \n"), Pt[1].x, Pt[1].y);		strTemp2 += strTemp1;
//				strTemp1.Format(_T("[Point 3] X=%d, Y=%d \n"), Pt[2].x, Pt[2].y);		strTemp2 += strTemp1;
//
//				AfxMessageBox(strTemp2); 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,3);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,3);
			}
			break;
		case enBcsIsoscelesTriangleToBottom:					// 10
			{
				CPoint Pt[3];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.left+2, rcControlS.top+2);
				Pt[1] = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[2] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom-2);

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,3);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,3);
			}
			break;
//	enBcsDoubleIsoscelesTriangleToLeft, enBcsDoubleIsoscelesTriangleToRight, enBcsDoubleIsoscelesTriangleToTop, enBcsDoubleIsoscelesTriangleToBottom, 
		case enBcsDoubleIsoscelesTriangleToLeft:				// 11
			{
				CPoint Pt[7];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.left+2, rcControlS.top+2);
				Pt[1] = CPoint(rcControlS.left+2, rcControlS.bottom-2);
				Pt[2] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 2 )); // Áß¾ÓÁ¡ 
				Pt[3] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom-2);
				Pt[4] = CPoint(rcControlS.right-2, rcControlS.top + int(rcControlS.Height() / 2 ));
				Pt[5] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top+2);
				Pt[6] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 2 )); // Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,7);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,7);
			}
			break;
		case enBcsDoubleIsoscelesTriangleToRight:				// 12
			{
				CPoint Pt[7];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[1] = CPoint(rcControlS.right-2, rcControlS.bottom-2);
				Pt[2] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 2 )); // Áß¾ÓÁ¡ 
				Pt[3] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom-2);
				Pt[4] = CPoint(rcControlS.left+2, rcControlS.top + int(rcControlS.Height() / 2 ));
				Pt[5] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top+2);
				Pt[6] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 2 )); // Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,7);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,7);
			}
			break;
		case enBcsDoubleIsoscelesTriangleToTop:					// 13
			{
				CPoint Pt[7];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.left+2, rcControlS.bottom-2);
				Pt[1] = CPoint(rcControlS.right-2, rcControlS.bottom-2);
				Pt[2] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 2 )); // Áß¾ÓÁ¡ 
				Pt[3] = CPoint(rcControlS.right-2, rcControlS.top + int(rcControlS.Height() / 2 ));
				Pt[4] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top+2);
				Pt[5] = CPoint(rcControlS.left+2, rcControlS.top + int(rcControlS.Height() / 2 ));
				Pt[6] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 2 )); // Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,7);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,7);
			}
			break;
		case enBcsDoubleIsoscelesTriangleToBottom:				// 14	
			{
				CPoint Pt[7];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0] = CPoint(rcControlS.left+2, rcControlS.top+2);
				Pt[1] = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[2] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 2 )); // Áß¾ÓÁ¡ 
				Pt[3] = CPoint(rcControlS.right-2, rcControlS.top + int(rcControlS.Height() / 2 ));
				Pt[4] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom-2);
				Pt[5] = CPoint(rcControlS.left+2, rcControlS.top + int(rcControlS.Height() / 2 ));
				Pt[6] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 2 )); // Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,7);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,7);
			}
			break;
//	enBcsTripleIsoscelesTriangleToLeft, enBcsTripleIsoscelesTriangleToRight, enBcsTripleIsoscelesTriangleToTop, enBcsTripleIsoscelesTriangleToBottom, 
		case enBcsTripleIsoscelesTriangleToLeft:			// 15
			{
				CPoint Pt[11];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0]  = CPoint(rcControlS.left+2, rcControlS.top+2);
				Pt[1]  = CPoint(rcControlS.left+2, rcControlS.bottom-2);
				Pt[2]  = CPoint(rcControlS.left+2 + int(rcControlS.Width() / 3 ), rcControlS.top + int(rcControlS.Height() / 2 ));			// 1 Áß¾ÓÁ¡ 
				Pt[3]  = CPoint(rcControlS.left+2 + int(rcControlS.Width() / 3 ), rcControlS.bottom-2);

				Pt[4]  = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 3 ) * 2), rcControlS.top + int(rcControlS.Height() / 2 ));	// 2 Áß¾ÓÁ¡ 
				Pt[5]  = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 3 ) * 2), rcControlS.bottom-2);

				Pt[6]  = CPoint(rcControlS.right-2, rcControlS.top + int(rcControlS.Height() / 2 ));										// ³¡Á¡ 

				Pt[7]  = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 3 ) * 2), rcControlS.top+2);
				Pt[8]  = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 3 ) * 2), rcControlS.top + int(rcControlS.Height() / 2 ));	// 2 Áß¾ÓÁ¡ 

				Pt[9]  = CPoint(rcControlS.left+2 + int(rcControlS.Width() / 3 ), rcControlS.top+2);
				Pt[10] = CPoint(rcControlS.left+2 + int(rcControlS.Width() / 3 ), rcControlS.top + int(rcControlS.Height() / 2 ));			// 1 Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,11);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,11);
			}
			break;
		case enBcsTripleIsoscelesTriangleToRight:			// 16
			{
				CPoint Pt[11];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0]  = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[1]  = CPoint(rcControlS.right-2, rcControlS.bottom-2);

				Pt[2]  = CPoint(rcControlS.right-2 - int(rcControlS.Width() / 3 ), rcControlS.top + int(rcControlS.Height() / 2 ));			// 1 Áß¾ÓÁ¡ 
				Pt[3]  = CPoint(rcControlS.right-2 - int(rcControlS.Width() / 3 ), rcControlS.bottom-2);

				Pt[4]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 3 ) * 2), rcControlS.top + int(rcControlS.Height() / 2 ));	// 2 Áß¾ÓÁ¡ 
				Pt[5]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 3 ) * 2), rcControlS.bottom-2);

				Pt[6]  = CPoint(rcControlS.left+2, rcControlS.top + int(rcControlS.Height() / 2 ));											// ³¡Á¡					

				Pt[7]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 3 ) * 2), rcControlS.top+2);
				Pt[8]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 3 ) * 2), rcControlS.top + int(rcControlS.Height() / 2 ));	// 2 Áß¾ÓÁ¡ 

				Pt[9]  = CPoint(rcControlS.right-2 - int(rcControlS.Width() / 3 ), rcControlS.top+2);
				Pt[10] = CPoint(rcControlS.right-2 - int(rcControlS.Width() / 3 ), rcControlS.top + int(rcControlS.Height() / 2 ));			// 1 Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,11);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,11);
			}
			break;
		case enBcsTripleIsoscelesTriangleToTop:				// 17
			{
				CPoint Pt[11];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0]  = CPoint(rcControlS.left+2, rcControlS.bottom-2);
				Pt[1]  = CPoint(rcControlS.right-2, rcControlS.bottom-2);

				Pt[2]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - int(rcControlS.Height() / 3));			// 1 Áß¾ÓÁ¡ 
				Pt[3]  = CPoint(rcControlS.right-2, rcControlS.bottom - int(rcControlS.Height() / 3));

				Pt[4]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - (int(rcControlS.Height() / 3) * 2));	// 2 Áß¾ÓÁ¡ 
				Pt[5]  = CPoint(rcControlS.right-2, rcControlS.bottom - (int(rcControlS.Height() / 3) * 2));

				Pt[6]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top+2);										// ³¡Á¡ 

				Pt[7]  = CPoint(rcControlS.left+2, rcControlS.bottom - (int(rcControlS.Height() / 3) * 2));
				Pt[8]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - (int(rcControlS.Height() / 3) * 2));	// 2 Áß¾ÓÁ¡ 

				Pt[9]  = CPoint(rcControlS.left+2, rcControlS.bottom - int(rcControlS.Height() / 3));
				Pt[10] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - int(rcControlS.Height() / 3));			// 1 Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,11);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,11);
			}
			break;
		case enBcsTripleIsoscelesTriangleToBottom:			// 18
			{
				CPoint Pt[11];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0]  = CPoint(rcControlS.left+2, rcControlS.top+2);
				Pt[1]  = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[2]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 3 ));		// 1 Áß¾ÓÁ¡ 
				Pt[3]  = CPoint(rcControlS.right-2, rcControlS.top + int(rcControlS.Height() / 3 ));

				Pt[4]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + (int(rcControlS.Height() / 3 ) * 2));	// 2 Áß¾ÓÁ¡ 
				Pt[5]  = CPoint(rcControlS.right-2, rcControlS.top + (int(rcControlS.Height() / 3 ) * 2));

				Pt[6]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom-2);									// ³¡Á¡ 

				Pt[7]  = CPoint(rcControlS.left+2, rcControlS.top + (int(rcControlS.Height() / 3 ) * 2));
				Pt[8]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + (int(rcControlS.Height() / 3 ) * 2));	// 2 Áß¾ÓÁ¡ 

				Pt[9]  = CPoint(rcControlS.left+2, rcControlS.top + int(rcControlS.Height() / 3 ));
				Pt[10] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 3 ));		// 1 Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,11);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,11);
			}
			break;
//		enBcsQuadrupleIsoscelesTriangleToLeft, enBcsQuadrupleIsoscelesTriangleToRight, enBcsQuadrupleIsoscelesTriangleToTop, enBcsQuadrupleIsoscelesTriangleToBottom,	// 19, 20, 21, 22
		case enBcsQuadrupleIsoscelesTriangleToLeft:			// 19
			{
				CPoint Pt[15];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0]  = CPoint(rcControlS.left+2, rcControlS.top+2);
				Pt[1]  = CPoint(rcControlS.left+2, rcControlS.bottom-2);
				Pt[2]  = CPoint(rcControlS.left+2 + int(rcControlS.Width() / 4 ), rcControlS.top + int(rcControlS.Height() / 2 ));			// 1 Áß¾ÓÁ¡ 
				Pt[3]  = CPoint(rcControlS.left+2 + int(rcControlS.Width() / 4 ), rcControlS.bottom-2);										// 1 ¾Æ·¡ ³¡Á¡

				Pt[4]  = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 4 ) * 2), rcControlS.top + int(rcControlS.Height() / 2 ));	// 2 Áß¾ÓÁ¡ 
				Pt[5]  = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 4 ) * 2), rcControlS.bottom-2);								// 2 ¾Æ·¡ ³¡Á¡

				Pt[6]  = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 4 ) * 3), rcControlS.top + int(rcControlS.Height() / 2 ));	// 3 Áß¾ÓÁ¡ 
				Pt[7]  = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 4 ) * 3), rcControlS.bottom-2);								// 3 ¾Æ·¡ ³¡Á¡

				Pt[8]  = CPoint(rcControlS.right-2, rcControlS.top + int(rcControlS.Height() / 2 ));										// ³¡Á¡(°¡·Î:¿À¸¥ÂÊ ³¡ & ¼¼·Î:Áß°£)

				Pt[9]  = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 4 ) * 3), rcControlS.top+2);									// 3 À§ ³¡Á¡
				Pt[10] = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 4 ) * 3), rcControlS.top + int(rcControlS.Height() / 2 ));	// 3 Áß¾ÓÁ¡ 

				Pt[11] = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 4 ) * 2), rcControlS.top+2);
				Pt[12] = CPoint(rcControlS.left+2 + (int(rcControlS.Width() / 4 ) * 2), rcControlS.top + int(rcControlS.Height() / 2 ));	// 2 Áß¾ÓÁ¡ 

				Pt[13] = CPoint(rcControlS.left+2 + int(rcControlS.Width() / 4 ), rcControlS.top+2);
				Pt[14] = CPoint(rcControlS.left+2 + int(rcControlS.Width() / 4 ), rcControlS.top + int(rcControlS.Height() / 2 ));			// 1 Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,15);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,15);
			}
			break;
		case enBcsQuadrupleIsoscelesTriangleToRight:			// 20
			{
				CPoint Pt[15];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0]  = CPoint(rcControlS.right-2, rcControlS.top+2);
				Pt[1]  = CPoint(rcControlS.right-2, rcControlS.bottom-2);

				Pt[2]  = CPoint(rcControlS.right-2 - int(rcControlS.Width() / 4 ), rcControlS.top + int(rcControlS.Height() / 2 ));			// 1 Áß¾ÓÁ¡ 
				Pt[3]  = CPoint(rcControlS.right-2 - int(rcControlS.Width() / 4 ), rcControlS.bottom-2);

				Pt[4]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 4 ) * 2), rcControlS.top + int(rcControlS.Height() / 2 ));	// 2 Áß¾ÓÁ¡ 
				Pt[5]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 4 ) * 2), rcControlS.bottom-2);

				Pt[6]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 4 ) * 3), rcControlS.top + int(rcControlS.Height() / 2 ));	// 3 Áß¾ÓÁ¡ 
				Pt[7]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 4 ) * 3), rcControlS.bottom-2);

				Pt[8]  = CPoint(rcControlS.left+2, rcControlS.top + int(rcControlS.Height() / 2 ));											// ³¡Á¡					

				Pt[9]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 4 ) * 3), rcControlS.top+2);
				Pt[10]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 4 ) * 3), rcControlS.top + int(rcControlS.Height() / 2 ));	// 3 Áß¾ÓÁ¡ 

				Pt[11]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 4 ) * 2), rcControlS.top+2);
				Pt[12]  = CPoint(rcControlS.right-2 - (int(rcControlS.Width() / 4 ) * 2), rcControlS.top + int(rcControlS.Height() / 2 ));	// 2 Áß¾ÓÁ¡ 

				Pt[13]  = CPoint(rcControlS.right-2 - int(rcControlS.Width() / 4 ), rcControlS.top+2);
				Pt[14] = CPoint(rcControlS.right-2 - int(rcControlS.Width() / 4 ), rcControlS.top + int(rcControlS.Height() / 2 ));			// 1 Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,15);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,15);
			}
			break;
		case enBcsQuadrupleIsoscelesTriangleToTop:				// 21
			{
				CPoint Pt[15];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0]  = CPoint(rcControlS.left+2, rcControlS.bottom-2);
				Pt[1]  = CPoint(rcControlS.right-2, rcControlS.bottom-2);

				Pt[2]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - int(rcControlS.Height() / 4));			// 1 Áß¾ÓÁ¡ 
				Pt[3]  = CPoint(rcControlS.right-2, rcControlS.bottom - int(rcControlS.Height() / 4));

				Pt[4]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - (int(rcControlS.Height() / 4) * 2));	// 2 Áß¾ÓÁ¡ 
				Pt[5]  = CPoint(rcControlS.right-2, rcControlS.bottom - (int(rcControlS.Height() / 4) * 2));

				Pt[6]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - (int(rcControlS.Height() / 4) * 3));	// 3 Áß¾ÓÁ¡ 
				Pt[7]  = CPoint(rcControlS.right-2, rcControlS.bottom - (int(rcControlS.Height() / 4) * 3));

				Pt[8]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top+2);											// ³¡Á¡ 

				Pt[9]  = CPoint(rcControlS.left+2, rcControlS.bottom - (int(rcControlS.Height() / 4) * 3));
				Pt[10]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - (int(rcControlS.Height() / 4) * 3));	// 3 Áß¾ÓÁ¡ 

				Pt[11]  = CPoint(rcControlS.left+2, rcControlS.bottom - (int(rcControlS.Height() / 4) * 2));
				Pt[12]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - (int(rcControlS.Height() / 4) * 2));	// 2 Áß¾ÓÁ¡ 

				Pt[13]  = CPoint(rcControlS.left+2, rcControlS.bottom - int(rcControlS.Height() / 4));
				Pt[14] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom - int(rcControlS.Height() / 4));			// 1 Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,15);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,15);
			}
			break;
		case enBcsQuadrupleIsoscelesTriangleToBottom:			// 22
			{
				CPoint Pt[15];
				CBrush bgBrush(m_clrBgColor);
				CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
				Pt[0]  = CPoint(rcControlS.left+2, rcControlS.top+2);
				Pt[1]  = CPoint(rcControlS.right-2, rcControlS.top+2);

				Pt[2]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 4 ));		// 1 Áß¾ÓÁ¡ 
				Pt[3]  = CPoint(rcControlS.right-2, rcControlS.top + int(rcControlS.Height() / 4 ));

				Pt[4]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + (int(rcControlS.Height() / 4 ) * 2));	// 2 Áß¾ÓÁ¡ 
				Pt[5]  = CPoint(rcControlS.right-2, rcControlS.top + (int(rcControlS.Height() / 4 ) * 2));

				Pt[6]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + (int(rcControlS.Height() / 4 ) * 3));	// 3 Áß¾ÓÁ¡ 
				Pt[7]  = CPoint(rcControlS.right-2, rcControlS.top + (int(rcControlS.Height() / 4 ) * 3));

				Pt[8]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.bottom-2);									// ³¡Á¡ 

				Pt[9]  = CPoint(rcControlS.left+2, rcControlS.top + (int(rcControlS.Height() / 4 ) * 3));
				Pt[10]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + (int(rcControlS.Height() / 4 ) * 3));	// 3 Áß¾ÓÁ¡ 

				Pt[11]  = CPoint(rcControlS.left+2, rcControlS.top + (int(rcControlS.Height() / 4 ) * 2));
				Pt[12]  = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + (int(rcControlS.Height() / 4 ) * 2));	// 2 Áß¾ÓÁ¡ 

				Pt[13]  = CPoint(rcControlS.left+2, rcControlS.top + int(rcControlS.Height() / 4 ));
				Pt[14] = CPoint(rcControlS.left + int(rcControlS.Width() / 2 ), rcControlS.top + int(rcControlS.Height() / 4 ));		// 1 Áß¾ÓÁ¡ 

				pDC->SelectStockObject(m_bClick ? WHITE_BRUSH : 1);
				pDC->Polygon(Pt,15);

				pDC->SelectObject(&bgBrush);
				pDC->Polygon(Pt,15);
			}
			break;
		}
//		m_pDCI->DrawText(pDC, m_rcControlL, m_strText, m_clrFgColor);
		//==>

		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);




		CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
		LOGFONT m_logfont;
		CFont m_font ;
		CFont * pOldFont;

		int nSize = rcControlS.Height();
		if (rcControlS.Height() > rcControlS.Width())
			nSize = rcControlS.Width();

		nSize = nSize / 2 ;

		HDC hDC;
		hDC = pDC->m_hDC;
		hDC = pDC->GetSafeHdc();
		memset(&m_logfont, 0, sizeof(LOGFONT));
		m_logfont.lfQuality = PROOF_QUALITY;
//		m_logfont.lfHeight = m_nFontSize;
		m_logfont.lfHeight = m_nFontSize;//long((-MulDiv(m_nFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 )) / m_pDCI->GetScale());
		m_logfont.lfWeight = FW_BOLD;
		lstrcpy(m_logfont.lfFaceName, _T("Arial"));
		m_font.CreateFontIndirect(&m_logfont);
		pOldFont = pDC->SelectObject(&m_font);

		pDC->DrawText(m_strText, rcControlS, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		/*
		LOGFONT m_logfont;
		CFont m_font ;
		CFont * pOldFont;

		memset(&m_logfont, 0, sizeof(LOGFONT));
		m_logfont.lfQuality = PROOF_QUALITY;
		m_logfont.lfHeight = m_nFontSize;
		m_logfont.lfWeight = FW_BOLD;
		lstrcpy(m_logfont.lfFaceName, _T("Arial"));
		m_font.CreateFontIndirect(&m_logfont);
		pOldFont = pDC->SelectObject(&m_font);

		pDC->DrawText(m_strText, m_pDCI->ConvertRectS(m_rcControlL), DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		*/
		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldBkMode);
		pDC->SelectObject(pOldFont);
	}
	catch (CException* e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciButtonCtrl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
}
