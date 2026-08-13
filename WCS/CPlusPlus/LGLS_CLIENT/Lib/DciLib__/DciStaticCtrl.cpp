// DciStaticCtrl.cpp: implementation of the CDciStaticCtrl class.
//

#include "StdAfx.h"
#include "DciStaticCtrl.h"


IMPLEMENT_SERIAL(CDciStaticCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

// CDciStaticCtrl
CDciStaticCtrl::CDciStaticCtrl(void)
{
}

CDciStaticCtrl::~CDciStaticCtrl(void)
{
}

// CDciStaticCtrl ¸â¹ö ÇÔ¼ö
void CDciStaticCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);

	m_nBorder = enBorderNone;
	m_nPenStyle = PS_SOLID;
	m_nPenWidth = 1;
	m_clrPen = RGB(0,0,0);
	m_rcGap.SetRectEmpty();
	m_nFontSize = 1;
	m_nFontSizeTemp = 0;
}

int CDciStaticCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enStaticCtrlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("fontsize"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("border"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("pstyle"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("pwidth"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("pcolor"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("gl"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("gt"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("gr"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("gb"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciStaticCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
		m_nFontSize		= CConvert::ToInt(properties[i++].m_strValue);
		m_nBorder		= CConvert::ToInt(properties[i++].m_strValue);
		m_nPenStyle		= CConvert::ToInt(properties[i++].m_strValue);
		m_nPenWidth		= CConvert::ToInt(properties[i++].m_strValue);
		m_clrPen		= CConvert::ToColor(properties[i++].m_strValue);
		m_rcGap.left	= CConvert::ToInt(properties[i++].m_strValue);
		m_rcGap.top		= CConvert::ToInt(properties[i++].m_strValue);
		m_rcGap.right	= CConvert::ToInt(properties[i++].m_strValue);
		m_rcGap.bottom	= CConvert::ToInt(properties[i++].m_strValue);
	}
	else	
	{
		properties[i++].m_strValue.Format(_T("%d"), m_nFontSize);
		properties[i++].m_strValue.Format(_T("%d"), m_nBorder);
		properties[i++].m_strValue.Format(_T("%d"), m_nPenStyle);
		properties[i++].m_strValue.Format(_T("%d"), m_nPenWidth);
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_clrPen));
		properties[i++].m_strValue.Format(_T("%d"), m_rcGap.left);
		properties[i++].m_strValue.Format(_T("%d"), m_rcGap.top);
		properties[i++].m_strValue.Format(_T("%d"), m_rcGap.right);
		properties[i++].m_strValue.Format(_T("%d"), m_rcGap.bottom);
	}

	return i;
}

void CDciStaticCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	try
	{
		CBrush brush(m_clrBgColor);
		CPen pen(m_nPenStyle, m_nPenWidth, m_clrPen);
		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);
		CBrush* pOldBrush = NULL;
		CPen* pOldPen = NULL;
		
		if (m_nBgMode == TRANSPARENT)
			pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		else
			pOldBrush = pDC->SelectObject(&brush);

		if (m_nBorder == enBorderNone)
			pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);
		else
			pOldPen = pDC->SelectObject(&pen);

		CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
		int w = abs(rcControlS.Width()), h = abs(rcControlS.Height());
		rcControlS.left		+= w * m_rcGap.left / 100;
		rcControlS.right	-= w * m_rcGap.right / 100;
		rcControlS.top		+= h * m_rcGap.top / 100;
		rcControlS.bottom	-= h * m_rcGap.bottom / 100;
		rcControlS.InflateRect(0, 0, 1, 1);

		int r = abs(rcControlS.Width()) < abs(rcControlS.Height()) ? abs(rcControlS.Width())/2 : abs(rcControlS.Height())/2;

		switch (m_nBorder)
		{
		case enBorderNone:
		case enBorderRectangle:
		case enBorderLargeFont:
			pDC->Rectangle(rcControlS);
			break;

		case enBorderRoundRect:
			pDC->RoundRect(rcControlS, CPoint(r, r));
			break;

		case enBorderEllipse:
			pDC->Ellipse(rcControlS);
			break;
		}

		LOGFONT m_logfont;
		CFont m_font ;
		CFont * pOldFont;

		int nSize = 0;
		int nHeight = rcControlS.Height();
		int nWidth = rcControlS.Width();
		nSize = nHeight;
		if (nHeight > nWidth)
			nSize = nWidth;

		int nStrCnt = 0;
		if (m_strText.GetLength() < 5)
			nStrCnt = 1;
		else
			nStrCnt = (m_strText.GetLength() / 5) + 1;


/*		if (m_strText == "Degassing" || m_strText == "ISP" || m_strText == "RANK" || (m_strText.GetLength() == 4 && m_strText.Left(1) == _T("9")))
		{
			nSize = nSize / 3 ;
		}
		else*/ 

		//if (m_strText.GetLength() > 20)
		//	nSize = nSize / 2 ;
		//else
		//	

		nSize = nSize / 2 ;
		//nSize = nSize / nStrCnt ;

//		if (m_nBorder == enBorderLargeFont)
//		{
			memset(&m_logfont,0,sizeof(LOGFONT));
			m_logfont.lfQuality = PROOF_QUALITY;
//			m_logfont.lfHeight = r;
			m_logfont.lfHeight = nSize;
			//m_logfont.lfHeight = nSize;//long((-MulDiv(m_nFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 )) / m_pDCI->GetScale());
			m_logfont.lfWeight = FW_BOLD;
			lstrcpy(m_logfont.lfFaceName, _T("Arial") );

			m_font.CreateFontIndirect(&m_logfont);

			pOldFont = pDC->SelectObject(&m_font);
//		}

		pDC->DrawText(m_strText, rcControlS, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldBkMode);

//		if (m_nBorder == enBorderLargeFont)
			pDC->SelectObject(pOldFont);
//		m_font.DeleteObject();
	}
	catch (CException* e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciStaticCtrl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
}
