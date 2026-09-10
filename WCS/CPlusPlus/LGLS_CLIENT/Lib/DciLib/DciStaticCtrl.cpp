// DciStaticCtrl.cpp: implementation of the CDciStaticCtrl class.
//

#include "StdAfx.h"
#include "DciStaticCtrl.h"



// [LGLS 2026-09-09] 컨트롤 칸에 맞춰 글자 크기를 정해 그린다.
//   기준 크기 = XML 의 fontsize x 화면 배율(CDciMaster::GetScale).
//   그 크기가 칸을 넘치면 들어갈 때까지 줄인다. 한 줄 가운데 정렬은 종전과 같다.
static void DrawTextFitToRect(CDC* pDC, const CString& strText, const CRect& rc, int nBaseFontSize, int nScale)
{
	if (strText.IsEmpty())
		return;

	int nW = abs(rc.Width());
	int nH = abs(rc.Height());
	if (nW < 2 || nH < 2)
		return;

	if (nScale < 1) nScale = 1;
	int nFont = nBaseFontSize * nScale;
	if (nFont < 1) nFont = 1;

	// 칸 높이를 넘지 않게 (위아래 약간의 여백을 둔다)
	int nMaxH = nH - 2;
	if (nMaxH < 3) nMaxH = 3;
	if (nFont > nMaxH) nFont = nMaxH;

	LOGFONT lf;
	CFont font;
	CSize sz;
	CFont* pOldFont = NULL;
	int nMaxW = nW - 2;
	if (nMaxW < 3) nMaxW = 3;

	// 한 번 재보고 폭 비율로 한 번에 줄인 뒤, 몇 번만 다듬는다(매번 폰트를 만드는 비용을 아낀다)
	for (int nTry = 0; nTry < 8; nTry++)
	{
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfQuality = PROOF_QUALITY;
		lf.lfHeight  = nFont;
		lf.lfWeight  = FW_BOLD;
		lstrcpy(lf.lfFaceName, _T("Arial"));

		font.DeleteObject();
		if (!font.CreateFontIndirect(&lf))
			return;

		pOldFont = pDC->SelectObject(&font);
		sz = pDC->GetTextExtent(strText);

		if (sz.cx <= nMaxW || nFont <= 3)
			break;

		pDC->SelectObject(pOldFont);
		pOldFont = NULL;

		int nNext = (int)((__int64)nFont * nMaxW / (sz.cx > 0 ? sz.cx : 1));
		if (nNext >= nFont) nNext = nFont - 1;
		if (nNext < 3)      nNext = 3;
		nFont = nNext;
	}

	CRect rcDraw(rc);
	pDC->DrawText(strText, rcDraw, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (pOldFont != NULL)
		pDC->SelectObject(pOldFont);
}

IMPLEMENT_SERIAL(CDciStaticCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

// CDciStaticCtrl
CDciStaticCtrl::CDciStaticCtrl(void)
{
}

CDciStaticCtrl::~CDciStaticCtrl(void)
{
}

// CDciStaticCtrl 멤버 함수
void CDciStaticCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);

	m_nBorder = enBorderNone;
	m_nPenStyle = PS_SOLID;
	m_nPenWidth = 1;
	m_clrPen = RGB(0,0,0);
	m_rcGap.SetRectEmpty();
//	m_nFontSize = 1;
	m_nFontSizeTemp = 0;
}

int CDciStaticCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enStaticCtrlPropSize);
//	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("fontsize"));
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
//		m_nFontSize		= CConvert::ToInt(properties[i++].m_strValue);
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
//		properties[i++].m_strValue.Format(_T("%d"), m_nFontSize);
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

		/*
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

		if (m_strText.GetLength() > 20)
			nSize = nSize / 2 ;
		else
			nSize = nSize / nStrCnt ;

			memset(&m_logfont,0,sizeof(LOGFONT));
			m_logfont.lfQuality = PROOF_QUALITY;
//			m_logfont.lfHeight = r;
			m_logfont.lfHeight = m_nFontSize;
			m_logfont.lfWeight = FW_BOLD;
			lstrcpy(m_logfont.lfFaceName, _T("Arial") );

			m_font.CreateFontIndirect(&m_logfont);

			pOldFont = pDC->SelectObject(&m_font);

		pDC->DrawText(m_strText, rcControlS, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldBkMode);
		//*/

		// [LGLS 2026-09-09] 글자를 칸 크기에 맞춰 그린다(종전 DrawFontText 는 fontsize 고정).
		// [LGLS 2026-09-10] 글자는 락 안에서 한 벌 떠 온 뒤 그 사본으로 그린다.
		//   재는 동안(GetTextExtent + 폰트 생성) 수집 스레드가 m_strText 를 바꾸면
		//   버퍼가 사라져 DrawText 안에서 죽는다.
		CString strDraw = GetTextSafe();
		DrawTextFitToRect(pDC, strDraw, rcControlS, m_nFontSize, m_pDCI->GetScale());

		if (pOldPen != NULL)	pDC->SelectObject(pOldPen);
		if (pOldBrush != NULL)	pDC->SelectObject(pOldBrush);
		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldBkMode);
	}
	catch (CException* e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciStaticCtrl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
}
