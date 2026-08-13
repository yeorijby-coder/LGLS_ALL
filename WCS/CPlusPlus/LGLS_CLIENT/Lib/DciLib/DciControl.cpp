// DciControl.cpp: implementation of the CDciControl class.
//

#include "stdafx.h"
#include "DciControl.h"


IMPLEMENT_SERIAL(CDciControl, CObject, DCI_SIRIALIZE_SCHEMA)

// CDciControl
CDciControl::CDciControl()
{
}

CDciControl::~CDciControl()
{
}

// CDciControl ¸â¹ö ÇÔ¼ö
void CDciControl::InitControl(CDciMaster* pDCI)
{
	m_pDCI = pDCI;
	ASSERT(m_pDCI != NULL);

	m_strCID.Empty();
	m_strText.Empty();
	m_rcControlL.SetRectEmpty();

	m_nBgMode = OPAQUE;
	m_nFgMode = TRANSPARENT;

	m_clrBgColor = RGB(255,255,255);
	m_clrFgColor = RGB(0,0,0);

	m_bEnable = TRUE;
	m_bVisible = TRUE;
	
	m_bClick = FALSE;
	m_nFontSize = 10;
}

int CDciControl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = 0;

	properties.SetSize(enControlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("id"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("l"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("b"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("r"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("t"));
	properties[i++].SetProperty(CDciProperty::PT_STRING, _T("text"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("bgmode"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("fgmode"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("bgcolor"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("fgcolor"));
	properties[i++].SetProperty(CDciProperty::PT_BOOL, _T("enable"));
	properties[i++].SetProperty(CDciProperty::PT_BOOL, _T("visible"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("fontsize"));
//	m_nFontSize
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciControl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = 0;
	
	if (bSaveObject)
	{
		m_strCID			= properties[i++].m_strValue;
		m_rcControlL.left	= (LONG)CConvert::ToInt(properties[i++].m_strValue);
		m_rcControlL.bottom = (LONG)CConvert::ToInt(properties[i++].m_strValue);
		m_rcControlL.right	= (LONG)CConvert::ToInt(properties[i++].m_strValue);
		m_rcControlL.top	= (LONG)CConvert::ToInt(properties[i++].m_strValue);
		m_strText			= properties[i++].m_strValue;
		m_nBgMode			= CConvert::ToInt(properties[i++].m_strValue);
		m_nFgMode			= CConvert::ToInt(properties[i++].m_strValue);
		m_clrBgColor		= CConvert::ToColor(properties[i++].m_strValue);
		m_clrFgColor		= CConvert::ToColor(properties[i++].m_strValue);
		m_bEnable			= (BOOL)CConvert::ToInt(properties[i++].m_strValue);
		m_bVisible			= (BOOL)CConvert::ToInt(properties[i++].m_strValue);
		m_nFontSize			= (LONG)CConvert::ToInt(properties[i++].m_strValue);
	}
	else	
	{
		properties[i++].m_strValue.Format(_T("%s"), m_strCID);
		properties[i++].m_strValue.Format(_T("%d"), m_rcControlL.left);
		properties[i++].m_strValue.Format(_T("%d"), m_rcControlL.bottom);
		properties[i++].m_strValue.Format(_T("%d"), m_rcControlL.right);
		properties[i++].m_strValue.Format(_T("%d"), m_rcControlL.top);
		properties[i++].m_strValue.Format(_T("%s"), m_strText);
		properties[i++].m_strValue.Format(_T("%d"), m_nBgMode);
		properties[i++].m_strValue.Format(_T("%d"), m_nFgMode);
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_clrBgColor));
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_clrFgColor));
		properties[i++].m_strValue.Format(_T("%d"), m_bEnable);
		properties[i++].m_strValue.Format(_T("%d"), m_bVisible);
		properties[i++].m_strValue.Format(_T("%d"), m_nFontSize);
	}

	return i;
}

void CDciControl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	// prepare locals so catch block can restore state
	CBrush* pOldBrush = NULL;
	CFont* pOldFont = NULL;
	int nOldMode = OPAQUE;
	COLORREF nOldFgColor = RGB(0,0,0);

	try
	{
		CBrush bgBrush(m_clrBgColor);

		// save previous modes (callers may pass invalid pDC but we assume pDC valid here)
		nOldMode = pDC->GetBkMode();
		nOldFgColor = pDC->GetTextColor();

		nOldMode = pDC->SetBkMode(m_nFgMode);
		nOldFgColor = pDC->SetTextColor(m_clrFgColor);

		if (m_nBgMode == TRANSPARENT)
			pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		else
			pOldBrush = pDC->SelectObject(&bgBrush);

		CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
		rcControlS.InflateRect(0, 0, 1, 1);
		pDC->Rectangle(rcControlS);

		//==================================================================================================
		//==================================================================================================
		LOGFONT m_logfont;
		CFont m_font;

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
			nSize = nSize / 2;
		else
			nSize = nSize / nStrCnt;

		memset(&m_logfont, 0, sizeof(LOGFONT));
		m_logfont.lfQuality = PROOF_QUALITY;
		m_logfont.lfHeight = m_nFontSize;
		m_logfont.lfWeight = FW_BOLD;
		lstrcpy(m_logfont.lfFaceName, _T("Arial"));

		m_font.CreateFontIndirect(&m_logfont);

		// select font and remember previous one
		pOldFont = (CFont*)pDC->SelectObject(&m_font);

		m_pDCI->DrawText(pDC, m_rcControlL, m_strText, m_clrFgColor);

		// restore DC state
		if (pOldBrush != NULL)
			pDC->SelectObject(pOldBrush);
		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldMode);

		if (pOldFont != NULL)
			pDC->SelectObject(pOldFont);

	}
	catch (CException* e)
	{
		// Attempt to restore DC state to avoid leaving DC with deleted GDI objects
		if (pOldBrush != NULL && pDC != NULL)
			pDC->SelectObject(pOldBrush);
		if (pOldFont != NULL && pDC != NULL)
			pDC->SelectObject(pOldFont);
		if (pDC != NULL)
		{
			pDC->SetTextColor(nOldFgColor);
			pDC->SetBkMode(nOldMode);
		}

		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciControl::UpdateControl... CException! [%s] \n"), szMessage);
		e->Delete();
	}
}

void CDciControl::DrawFontText(CDC* pDC, CString strText, CRect* pRect, int nOldBkMode, int nOldFgColor, CBrush* pOldBrush, CPen* pOldPen)
{
//	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
//	int nOldFgColor = pDC->SetTextColor(m_clrFgColor);
//	CBrush* pOldBrush = NULL;
//	CPen* pOldPen = NULL;




	CRect rcControlS = NULL;
		
	if (pRect == NULL)
		rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
	else
		rcControlS = *pRect;

	CString strRealText = _T("");

	if (strText == _T(""))
		strRealText = m_strText;
	else
		strRealText = strText;


	LOGFONT m_logfont;
	CFont m_font;
	CFont* pOldFont;

	int nSize = 0;
	int nHeight = rcControlS.Height();
	int nWidth = rcControlS.Width();
	nSize = nHeight;
	if (nHeight > nWidth)
		nSize = nWidth;

	int nStrCnt = 0;
	//if (m_strText.GetLength() < 5)
	if (strRealText.GetLength() < 5)
		nStrCnt = 1;
	else
		nStrCnt = (strRealText.GetLength() / 5) + 1;
	//	nStrCnt = (m_strText.GetLength() / 5) + 1;

	//if (m_strText.GetLength() > 20)
	if (strRealText.GetLength() > 20)
		nSize = nSize / 2;
	else
		nSize = nSize / nStrCnt;

	memset(&m_logfont, 0, sizeof(LOGFONT));
	m_logfont.lfQuality = PROOF_QUALITY;
	//			m_logfont.lfHeight = r;
	m_logfont.lfHeight = m_nFontSize;
	m_logfont.lfWeight = FW_BOLD;
	lstrcpy(m_logfont.lfFaceName, _T("Arial"));

	m_font.CreateFontIndirect(&m_logfont);

	pOldFont = pDC->SelectObject(&m_font);
	
	//pDC->DrawText(m_strText, rcControlS, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->DrawText(strRealText, rcControlS, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (pOldFont != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldFont != NULL)
		pDC->SelectObject(pOldBrush);

	pDC->SetTextColor(nOldFgColor);
	pDC->SetBkMode(nOldBkMode);

	// restore previous font to avoid leaving the DC with a pointer to a temporary font
	if (pOldFont != NULL)
		pDC->SelectObject(pOldFont);
}


void CDciControl::InvalidateControl(CWnd* pWnd, BOOL bErase)
{
	if ((pWnd == NULL) || (pWnd->GetSafeHwnd() == NULL))
		return;

	pWnd->InvalidateRect(m_pDCI->ConvertRectS(m_rcControlL), bErase);
}

void CDciControl::InvalidateControl(HWND hWnd, BOOL bErase)
{
	if (hWnd == NULL)
		return;

	::InvalidateRect(hWnd, m_pDCI->ConvertRectS(m_rcControlL), bErase);	
}

BOOL CDciControl::SetClick(CWnd* pWnd, const CPoint& ptClickS)
{
	if (m_bClick = m_pDCI->ConvertRectS(m_rcControlL).PtInRect(ptClickS))
		InvalidateControl(pWnd, FALSE);

	return m_bClick;
}

// CDciControlArray ¸â¹ö ÇÔ¼ö
CDciControlArray::CDciControlArray()
{

}

CDciControlArray::~CDciControlArray()
{
	Clear();
}

void CDciControlArray::Clear()
{
	for (int i=0; i<m_nSize; ++i)
		delete m_pData[i];
	RemoveAll();
}

// CDciControlList ¸â¹ö ÇÔ¼ö
CDciControlList::CDciControlList()
{

}

CDciControlList::~CDciControlList()
{
	Clear();
}

void CDciControlList::Clear()
{
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
		delete GetNext(pos);
	RemoveAll();
}
