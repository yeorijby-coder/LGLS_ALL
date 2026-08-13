// DciOpboxCtrl.cpp: implementation of the CDciOpboxCtrl class.
//

#include "StdAfx.h"
#include "DciOpboxCtrl.h"


IMPLEMENT_SERIAL(CDciOpboxCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

// CDciOpboxCtrl
CDciOpboxCtrl::CDciOpboxCtrl(void)
{
}


CDciOpboxCtrl::~CDciOpboxCtrl(void)
{
}

// CDciOpboxCtrl ¸â¹ö ÇÔ¼ö
void CDciOpboxCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);

	m_nMode = enModeNone;
	m_nGapW = 0;
	m_nGapH = 0;
}

int CDciOpboxCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enOpboxCtrlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("mode"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("gapw"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("gaph"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciOpboxCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
		m_nMode = CConvert::ToInt(properties[i++].m_strValue);
		m_nGapW = CConvert::ToInt(properties[i++].m_strValue);
		m_nGapH = CConvert::ToInt(properties[i++].m_strValue);
	}
	else	
	{
		properties[i++].m_strValue.Format(_T("%d"), m_nMode);
		properties[i++].m_strValue.Format(_T("%d"), m_nGapW);
		properties[i++].m_strValue.Format(_T("%d"), m_nGapH);
	}

	return i;
}

void CDciOpboxCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	try
	{
		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);

		int nMode = ((m_nMode > 0) && (m_nMode < enModeSize)) ? m_nMode : enModeNone;
		COLORREF clrMode[enModeSize][3] = { 
			{0x808080, 0x808080, 0x808080},	// NONE
			{0x808080, 0xff0000, 0x808080},	// AUTO
			{0x808080, 0x808080, 0x00ffff},	// MANUAL
			{0x0000ff, 0x808080, 0x00ffff},	// EMERGENCY
			{0x0000ff, 0xff0000, 0x00ffff}	// ALL
		};

		CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
		int w = abs(rcControlS.Width()), h = abs(rcControlS.Height());

		rcControlS.left		+= w * m_nGapW / 100;
		rcControlS.right	-= w * m_nGapW / 100;
		rcControlS.top		+= h * m_nGapH / 100;
		rcControlS.bottom	-= h * m_nGapH / 100;
		
		int nUnitHeight = rcControlS.Height() / 3;
		CRect rcUnitS(rcControlS);

		for (int i=0; i<3; ++i)
		{
			rcUnitS.top		= rcControlS.top + (i) * nUnitHeight;
			rcUnitS.bottom	= rcControlS.top + (i+1) * nUnitHeight;
			
			pDC->FillSolidRect(rcUnitS, clrMode[nMode][i]);
			if (m_bClick) pDC->Draw3dRect(rcUnitS, 0x000000, 0xffffff);
			else pDC->Draw3dRect(rcUnitS, 0xffffff, 0x000000);
		}

		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldBkMode);
	}
	catch (CException* e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciOpboxCtrl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
}
