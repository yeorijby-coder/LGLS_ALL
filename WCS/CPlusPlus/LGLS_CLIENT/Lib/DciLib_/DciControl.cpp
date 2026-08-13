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
	}

	return i;
}

void CDciControl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	try
	{
		CBrush* pOldBrush = NULL;
		CBrush bgBrush(m_clrBgColor);
		int nOldMode = pDC->SetBkMode(m_nFgMode);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);

		if (m_nBgMode == TRANSPARENT)
			pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		else
			pOldBrush = pDC->SelectObject(&bgBrush);

		CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
		rcControlS.InflateRect(0, 0, 1, 1);
		pDC->Rectangle(rcControlS);

		m_pDCI->DrawText(pDC, m_rcControlL, m_strText, m_clrFgColor);

		pDC->SelectObject(pOldBrush);
		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldMode);
	}
	catch (CException* e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciControl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
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
	TRACE(_T("InvalidateControl : \n"));

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
