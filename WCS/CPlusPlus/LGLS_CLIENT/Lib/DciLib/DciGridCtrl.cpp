// DciGridCtrl.cpp: implementation of the CDciGridCtrl class.
//

#include "StdAfx.h"
#include "DciGridCtrl.h"


IMPLEMENT_SERIAL(CDciGridCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

// CDciGridCtrl
CDciGridCtrl::CDciGridCtrl(void)
{
}


CDciGridCtrl::~CDciGridCtrl(void)
{
}

// CDciGridCtrl ¸â¹ö ÇÔ¼ö
void CDciGridCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);
	m_rcControlL.SetRect(0, 10, 10, 0);

	m_clrSelect = RGB(128,128,128);
	m_bShowLabel = TRUE;

	m_rcSelectL.SetRectEmpty();
}

int CDciGridCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enGridCtrlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("selcolor"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciGridCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
		m_clrSelect	= CConvert::ToColor(properties[i++].m_strValue);
	}
	else
	{
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_clrSelect));
	}

	return i;
}

void CDciGridCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	try
	{
		CBrush* pOldBrush = NULL;
		CBrush bgBrush(m_clrBgColor);
		CBrush selBrush(m_clrSelect);
		int nOldBkMode = pDC->SetBkMode(m_nFgMode);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);

		CString str;
		CRect rcRectS, rcRectL, rcIntersectL;
		int nDivX = abs(m_rcControlL.Width());
		int nDivY = abs(m_rcControlL.Height());

		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		
		for (int x=0; x<nDivX; ++x)
		{
			for (int y=0; y<nDivY; ++y)
			{
//				rcRectL.SetRect(x, y+1, x+1, y);
				rcRectL.SetRect(m_rcControlL.left+x, m_rcControlL.bottom+y+1, m_rcControlL.left+x+1, m_rcControlL.bottom+y);
				rcRectS = m_pDCI->ConvertRectS(rcRectL);
				rcRectS.right++;
				rcRectS.top--;

				if ((m_rcSelectL.left <= rcRectL.left) && (rcRectL.right <= m_rcSelectL.right) &&
					(m_rcSelectL.bottom <= rcRectL.bottom) && (rcRectL.top <= m_rcSelectL.top))
				{
					pDC->SelectObject(&selBrush);
				}
				else
				{
					if (m_nBgMode == TRANSPARENT)
						pDC->SelectStockObject(NULL_BRUSH);
					else
						pDC->SelectObject(&bgBrush);
				}
				pDC->Rectangle(rcRectS);

				if (m_bShowLabel && x*y == 0)
				{
					if (x == 0) str.Format(_T("%d"), y+1);
					if (y == 0) str.Format(_T("%d"), x+1);
					pDC->DrawText(str, rcRectS, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
				}
			}
		}

		pDC->SelectObject(pOldBrush);
		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldBkMode);
	}
	catch (CException* e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciGridCtrl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
}

CRect CDciGridCtrl::GetRectFromPoint(const CPoint& ptScreen)
{
	CRect rcUnitS, rcUnitL;
	int nDivX = abs(m_rcControlL.Width());
	int nDivY = abs(m_rcControlL.Height());

	for (int x=0; x<nDivX; ++x)
	{
		for (int y=0; y<nDivY; ++y)
		{
//			rcUnitL.SetRect(x, y+1, x+1, y);
			rcUnitL.SetRect(m_rcControlL.left+x, m_rcControlL.bottom+y+1, m_rcControlL.left+x+1, m_rcControlL.bottom+y);
			rcUnitS = m_pDCI->ConvertRectS(rcUnitL);
			if (rcUnitS.PtInRect(ptScreen))
				return rcUnitL;
		}
	}

	rcUnitL.SetRectEmpty();
	return rcUnitL;
}

CRect CDciGridCtrl::GetRectFromRect(const CRect& rcScreen)
{
	CRect rcIntersectS;
	CRect rcUnitS, rcUnitL;
	CRect rcSumL(0, 0, 0, 0);
	int nDivX = abs(m_rcControlL.Width());
	int nDivY = abs(m_rcControlL.Height());

	for (int x=0; x<nDivX; ++x)
	{
		for (int y=0; y<nDivY; ++y)
		{
//			rcUnitL.SetRect(x, y+1, x+1, y);
			rcUnitL.SetRect(m_rcControlL.left+x, m_rcControlL.bottom+y+1, m_rcControlL.left+x+1, m_rcControlL.bottom+y);
			rcUnitS = m_pDCI->ConvertRectS(rcUnitL);
			rcIntersectS.IntersectRect(rcUnitS, rcScreen);
			if (rcIntersectS.EqualRect(rcUnitS))
			{
				if (rcSumL.IsRectNull()) 
				{
					rcSumL = rcUnitL;
				}
				else
				{
					rcSumL.right = x+1;
					rcSumL.top = y+1;
				}
			}
		}
	}

	return rcSumL;
}

void CDciGridCtrl::SetSelectRect(const CPoint& ptScreen1, const CPoint& ptScreen2)
{
	CRect rcUnitL1 = GetRectFromPoint(ptScreen1);
	CRect rcUnitL2 = GetRectFromPoint(ptScreen2);

	m_rcSelectL.left	= (rcUnitL1.left < rcUnitL2.left) ? rcUnitL1.left : rcUnitL2.left;
	m_rcSelectL.bottom	= (rcUnitL1.bottom < rcUnitL2.bottom) ? rcUnitL1.bottom : rcUnitL2.bottom;
	m_rcSelectL.right	= (rcUnitL1.right > rcUnitL2.right) ? rcUnitL1.right : rcUnitL2.right;
	m_rcSelectL.top		= (rcUnitL1.top > rcUnitL2.top) ? rcUnitL1.top : rcUnitL2.top;
}
