// DciRvCtrl.cpp: implementation of the CDciRvCtrl class.
//

#include "StdAfx.h"
#include "DciRvCtrl.h"
#include <afxmt.h>		// [LGLS 2026-08-22] CCriticalSection / CSingleLock


IMPLEMENT_SERIAL(CDciRvCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

// CDciRvCtrl
// [LGLS 2026-08-22] 표시 문자 보호락 (CDciTrackCtrl 과 동일 규격)
static CCriticalSection g_csRvExtraText;

void CDciRvCtrl::SetExtraTextSafe(LPCTSTR s, COLORREF c)
{
	CSingleLock lk(&g_csRvExtraText, TRUE);
	m_strExtraText = s;
	m_clrExtraText = c;
}

CString CDciRvCtrl::GetExtraTextSafe(COLORREF* pColor)
{
	CSingleLock lk(&g_csRvExtraText, TRUE);
	if (pColor) *pColor = m_clrExtraText;
	return CString((LPCTSTR)m_strExtraText);
}

CDciRvCtrl::CDciRvCtrl(void)
{
	m_clrExtraText = RGB(0, 0, 0);
}


CDciRvCtrl::~CDciRvCtrl(void)
{
}

// CDciRvCtrl 멤버 함수
void CDciRvCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);
	m_nBgMode = TRANSPARENT;
	m_clrBgColor = ::GetSysColor(COLOR_BTNFACE);
	
	m_nType = enL2R;
	m_nForkType = enSingle;
	m_nForkPos = 1;
	m_clrFork = ::GetSysColor(COLOR_BTNFACE);
	m_clrFork1 = ::GetSysColor(COLOR_BTNFACE);
	m_clrFork2 = ::GetSysColor(COLOR_BTNFACE);
	m_clrWing = ::GetSysColor(COLOR_BTNFACE);
	m_clrRail = RGB(0,0,0);
	m_nIncrease = 0;
//	m_nCol		= 1;
//	m_nRow		= 1;
	m_nProd		= 0;
//	m_nFontSize = 10;
	m_rcForkS.SetRect(0, 0, 0, 0);
	m_rcForkT.SetRect(0, 0, 0, 0);
	m_rcForkD.SetRect(0, 0, 0, 0);
}

int CDciRvCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enRvCtrlPropSize);
//	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("col"));
//	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("row"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("prod"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("forktype"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("type"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("forkpos"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("forkcolor"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("wingcolor"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("railcolor"));
//	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("fontsize"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("Increase"));
	ASSERT(properties.GetSize() == i);
	
	return i;
}

int CDciRvCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
//		m_nCol		= CConvert::ToInt(properties[i++].m_strValue);
//		m_nRow		= CConvert::ToInt(properties[i++].m_strValue);
		m_nProd		= CConvert::ToInt(properties[i++].m_strValue);
		m_nForkType	= CConvert::ToInt(properties[i++].m_strValue);
		m_nType		= CConvert::ToInt(properties[i++].m_strValue);
		m_nForkPos	= CConvert::ToInt(properties[i++].m_strValue);
		m_clrFork	= CConvert::ToColor(properties[i++].m_strValue);
		m_clrWing	= CConvert::ToColor(properties[i++].m_strValue);
		m_clrRail	= CConvert::ToColor(properties[i++].m_strValue);
//		m_nFontSize	= CConvert::ToInt(properties[i++].m_strValue);
		m_nIncrease = CConvert::ToInt(properties[i++].m_strValue);
		m_clrFork2	= m_clrFork;
		
	}
	else
	{
//		properties[i++].m_strValue.Format(_T("%d"), m_nCol);
//		properties[i++].m_strValue.Format(_T("%d"), m_nRow);
		properties[i++].m_strValue.Format(_T("%d"), m_nProd);
		properties[i++].m_strValue.Format(_T("%d"), m_nForkType);
		properties[i++].m_strValue.Format(_T("%d"), m_nType);
		properties[i++].m_strValue.Format(_T("%d"), m_nForkPos);
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_clrFork));
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_clrWing));
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_clrRail));
//		properties[i++].m_strValue.Format(_T("%d"), m_nFontSize);
		properties[i++].m_strValue.Format(_T("%d"), m_nIncrease);
	}
	
	return i;
}
//===============================================================================================================================================
// 포크 컨트롤 수정 :	- 세로타입일경우 - 
//===============================================================================================================================================
void CDciRvCtrl::UpdateControlHorizental(int nType , 
										 CPoint & ptRailS1, CPoint & ptRailS2, 
										 CRect & rcRailS1, CRect & rcRailS2, 
										 CRect & rcWingS1, CRect & rcWingS2, 
										 CRect & rcForkL1, CRect & rcForkL2,
										 CRect & rcForkS, CRect & rcForkT, CRect & rcForkD,
										 int nForkType /*= 0*/)
{
	if ( nType == enT2B || nType == enB2T)
		return;

	int nForkPos = 0, nForkSize = 1, nRailEndS = 4, nWingGapRatio = 10, nWingScaleRatio = 30;

//	if (nForkType == enDouble)
//		nRailEndS *= 2;

	rcRailS1.SetRect(ptRailS1.x, ptRailS1.y - nRailEndS/2, ptRailS1.x + nRailEndS, ptRailS1.y + nRailEndS/2);
	rcRailS2.SetRect(ptRailS2.x, ptRailS2.y - nRailEndS/2, ptRailS2.x - nRailEndS, ptRailS2.y + nRailEndS/2);

	nForkSize = abs(m_rcControlL.Height()) ? abs(m_rcControlL.Height()) : 1;
//	if (nForkType == enGap2)
//		nForkSize = abs(m_rcControlL.Height()) ? abs(m_rcControlL.Height())*m_nLen : m_nLen;

	if (m_nForkPos <= 0) nForkPos = 0;
	else if (m_nForkPos >= abs(m_rcControlL.Width()/nForkSize)) nForkPos = abs(m_rcControlL.Width())/nForkSize - 1;
	else nForkPos = m_nForkPos;

	if (nType == enL2R)
	{
		rcForkL1.left = m_rcControlL.left + nForkPos*nForkSize;
		rcForkL1.top = m_rcControlL.top;
		rcForkL1.right = m_rcControlL.left + (nForkPos+1)*nForkSize;
		rcForkL1.bottom = m_rcControlL.bottom;
		m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

		rcWingS1.left = rcForkS.left - rcForkS.Width()*nWingGapRatio/100 - rcForkS.Width()*nWingScaleRatio/100;
		rcWingS1.top = rcForkS.top + rcForkS.Height()/2 - rcForkS.Height()*nWingScaleRatio/100;
		rcWingS1.right = rcForkS.left - rcForkS.Width()*nWingGapRatio/100;
		rcWingS1.bottom = rcForkS.top + rcForkS.Height()/2 + rcForkS.Height()*nWingScaleRatio/100;

		if (nForkType == enSingle)
		{
			rcWingS2.left = rcForkS.right + rcForkS.Width()*nWingGapRatio/100;
			rcWingS2.top = rcForkS.top + rcForkS.Height()/2 - rcForkS.Height()*nWingScaleRatio/100;
			rcWingS2.right = rcForkS.right + rcForkS.Width()*nWingGapRatio/100 + rcForkS.Width()*nWingScaleRatio/100;
			rcWingS2.bottom = rcForkS.top + rcForkS.Height()/2 + rcForkS.Height()*nWingScaleRatio/100;
		}
		else if (nForkType == enTwin)
		{
			rcForkL2.left = m_rcControlL.left + (nForkPos+1)*nForkSize;
			rcForkL2.top = m_rcControlL.top;
			rcForkL2.right = m_rcControlL.left + (nForkPos+2)*nForkSize;
			rcForkL2.bottom = m_rcControlL.bottom;
			m_rcForkT = rcForkT = m_pDCI->ConvertRectS(rcForkL2);

			rcWingS2.left = rcForkT.right + rcForkT.Width()*nWingGapRatio/100;
			rcWingS2.top = rcForkT.top + rcForkT.Height()/2 - rcForkT.Height()*nWingScaleRatio/100;
			rcWingS2.right = rcForkT.right + rcForkT.Width()*nWingGapRatio/100 + rcForkT.Width()*nWingScaleRatio/100;
			rcWingS2.bottom = rcForkT.top + rcForkT.Height()/2 + rcForkT.Height()*nWingScaleRatio/100;

			if (m_nIncrease != 0)
			{
				rcWingS2.left = rcForkS.right + rcForkS.Width()*nWingGapRatio/100;
				rcWingS2.top = rcForkS.top + rcForkS.Height()/m_nIncrease - rcForkS.Height()*nWingScaleRatio/100;
				rcWingS2.right = rcForkS.right + rcForkS.Width()*nWingGapRatio/100 + rcForkS.Width()*nWingScaleRatio/100;
				rcWingS2.bottom = rcForkS.top + rcForkS.Height()/m_nIncrease + rcForkS.Height()*nWingScaleRatio/100;
			}
		}
		else if (nForkType == enDouble)
		{
			// 포크사이즈가 반으로 줄어야 함!!
			nForkSize = nForkSize/2;
			
			// 레일을 반으로 줄여야 함! 
			nRailEndS /= 2;
			rcRailS1.SetRect(ptRailS1.x, ptRailS1.y - nRailEndS/2, ptRailS1.x + nRailEndS, ptRailS1.y + nRailEndS/2);
			rcRailS2.SetRect(ptRailS2.x, ptRailS2.y - nRailEndS/2, ptRailS2.x - nRailEndS, ptRailS2.y + nRailEndS/2);

			// 포크 #1 세팅
			rcForkL1.left	= m_rcControlL.left			+  (nForkPos		* nForkSize);
			rcForkL1.top	= m_rcControlL.top;
			rcForkL1.right	= m_rcControlL.left			+ ((nForkPos + 1)	* nForkSize);
			rcForkL1.bottom = m_rcControlL.top								- nForkSize;
			m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

			// 윙 #1 세팅
			rcWingS1.left	= rcForkS.left		- rcForkS.Width()*nWingGapRatio/100 - rcForkS.Width()*nWingScaleRatio/100;
			rcWingS1.top	= rcForkS.bottom	+ rcForkS.Height()/2				+ rcForkS.Height()*nWingScaleRatio/100;
			rcWingS1.right	= rcForkS.left		- rcForkS.Width()*nWingGapRatio/100;
			rcWingS1.bottom = rcForkS.bottom	- rcForkS.Height()/2				- rcForkS.Height()*nWingScaleRatio/100;

			// 윙 #2 세팅
			rcWingS2.left	= rcForkS.right		+ rcForkS.Width()*nWingGapRatio/100;
			rcWingS2.top	= rcForkS.bottom	+ rcForkS.Height()/2				+ rcForkS.Height()*nWingScaleRatio/100;
			rcWingS2.right	= rcForkS.right		+ rcForkS.Width()*nWingGapRatio/100 + rcForkS.Width()*nWingScaleRatio/100;
			rcWingS2.bottom = rcForkS.bottom	- rcForkS.Height()/2				- rcForkS.Height()*nWingScaleRatio/100;

			// 포크 #2 세팅
			rcForkL2.left	= m_rcControlL.left		+  (nForkPos		* nForkSize);
			rcForkL2.top	= m_rcControlL.bottom						+ nForkSize;
			rcForkL2.right	= m_rcControlL.left		+ ((nForkPos + 1)	* nForkSize);
			rcForkL2.bottom = m_rcControlL.bottom;	
			m_rcForkD = rcForkD = m_pDCI->ConvertRectS(rcForkL2);		
		}
		// m_nIncrease 사용함으로 enGap2은 사용하지 않음!
//		else if(nForkType == enGap2)
//		{
//		//	nForkSize *=2;
//	
//			rcWingS2.left = rcForkS.right + rcForkS.Width()*nWingGapRatio/100;
//			rcWingS2.top = rcForkS.top + rcForkS.Height()/2 - rcForkS.Height()*nWingScaleRatio/100;
//			rcWingS2.right = rcForkS.right + rcForkS.Width()*nWingGapRatio/100 + rcForkS.Width()*nWingScaleRatio/100;
//			rcWingS2.bottom = rcForkS.top + rcForkS.Height()/2 + rcForkS.Height()*nWingScaleRatio/100;
//		
//	//		if (m_nForkPos/2 <= 0) nForkPos = 0;
//	//		else if (m_nForkPos/2 >= abs(m_rcControlL.Width()/nForkSize)) nForkPos = (abs(m_rcControlL.Width())/nForkSize - 1)/2;
//	//		else nForkPos = m_nForkPos/2;
//		}
	}
	else //if (nType == enR2L)
	{
		rcForkL1.left = m_rcControlL.right - (nForkPos+1)*nForkSize;		
		rcForkL1.top = m_rcControlL.top;									
		rcForkL1.right = m_rcControlL.right - nForkPos*nForkSize;			
		rcForkL1.bottom = m_rcControlL.bottom;								
		m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);				

		rcWingS1.left = rcForkS.left - rcForkS.Width()*nWingGapRatio/100 - rcForkS.Width()*nWingScaleRatio/100;
		rcWingS1.top = rcForkS.top + rcForkS.Height()/2 - rcForkS.Height()*nWingScaleRatio/100;
		rcWingS1.right = rcForkS.left - rcForkS.Width()*nWingGapRatio/100;
		rcWingS1.bottom = rcForkS.top + rcForkS.Height()/2 + rcForkS.Height()*nWingScaleRatio/100;

		if (nForkType == enSingle)
		{
			rcWingS2.left = rcForkS.right + rcForkS.Width()*nWingGapRatio/100;
			rcWingS2.top = rcForkS.top + rcForkS.Height()/2 - rcForkS.Height()*nWingScaleRatio/100;
			rcWingS2.right = rcForkS.right + rcForkS.Width()*nWingGapRatio/100 + rcForkS.Width()*nWingScaleRatio/100;
			rcWingS2.bottom = rcForkS.top + rcForkS.Height()/2 + rcForkS.Height()*nWingScaleRatio/100;
		}
		else if (nForkType == enTwin)
		{
			rcForkL2.left = m_rcControlL.right - (nForkPos+2)*nForkSize;	
			rcForkL2.top = m_rcControlL.top;								
			rcForkL2.right = m_rcControlL.right - (nForkPos+1)*nForkSize;	
			rcForkL2.bottom = m_rcControlL.bottom;							
			m_rcForkT = rcForkT = m_pDCI->ConvertRectS(rcForkL2);			

			rcWingS1.left = rcForkT.left - rcForkT.Width()*nWingGapRatio/100 - rcForkT.Width()*nWingScaleRatio/100;
			rcWingS1.top = rcForkT.top + rcForkT.Height()/2 - rcForkT.Height()*nWingScaleRatio/100;
			rcWingS1.right = rcForkT.left - rcForkT.Width()*nWingGapRatio/100;
			rcWingS1.bottom = rcForkT.top + rcForkT.Height()/2 + rcForkT.Height()*nWingScaleRatio/100;
		}
		else if (nForkType == enDouble)
		{
			// 포크사이즈가 반으로 줄어야 함!!
			nForkSize = nForkSize/2;

			// 레일을 반으로 줄여야 함! 
			nRailEndS /= 2;
			rcRailS1.SetRect(ptRailS1.x, ptRailS1.y - nRailEndS/2, ptRailS1.x + nRailEndS, ptRailS1.y + nRailEndS/2);
			rcRailS2.SetRect(ptRailS2.x, ptRailS2.y - nRailEndS/2, ptRailS2.x - nRailEndS, ptRailS2.y + nRailEndS/2);

	
//			// 포크 #1 세팅
//			rcForkL1.left	= m_rcControlL.right		- (nForkPos + 1)	* nForkSize;
//			rcForkL1.top	= m_rcControlL.top;
//			rcForkL1.right	= m_rcControlL.right		-  nForkPos			* nForkSize;
//			rcForkL1.bottom = m_rcControlL.top								- nForkSize;
//			m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);
//			// 포크 #1 세팅
			rcForkL1.left	= m_rcControlL.right	- (nForkPos + 1)	* nForkSize;
			rcForkL1.top	= m_rcControlL.bottom						+ nForkSize;
			rcForkL1.right	= m_rcControlL.right	-  nForkPos			* nForkSize;
			rcForkL1.bottom = m_rcControlL.bottom;	
			m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

//			// 윙 #1 세팅
//			rcWingS1.left	= rcForkS.left		- rcForkS.Width()*nWingGapRatio/100 - rcForkS.Width()*nWingScaleRatio/100;
//			rcWingS1.top	= rcForkS.bottom	+ rcForkS.Height()/2				+ rcForkS.Height()*nWingScaleRatio/100;
//			rcWingS1.right	= rcForkS.left		- rcForkS.Width()*nWingGapRatio/100;
//			rcWingS1.bottom = rcForkS.bottom	- rcForkS.Height()/2				- rcForkS.Height()*nWingScaleRatio/100;
//
//			// 윙 #2 세팅
//			rcWingS2.left	= rcForkS.right		+ rcForkS.Width()*nWingGapRatio/100;
//			rcWingS2.top	= rcForkS.bottom	+ rcForkS.Height()/2				+ rcForkS.Height()*nWingScaleRatio/100;
//			rcWingS2.right	= rcForkS.right		+ rcForkS.Width()*nWingGapRatio/100 + rcForkS.Width()*nWingScaleRatio/100;
//			rcWingS2.bottom = rcForkS.bottom	- rcForkS.Height()/2				- rcForkS.Height()*nWingScaleRatio/100;
//
//			// 포크 #2 세팅
//			rcForkL2.left	= m_rcControlL.right	- (nForkPos + 1)	* nForkSize;
//			rcForkL2.top	= m_rcControlL.bottom						+ nForkSize;
//			rcForkL2.right	= m_rcControlL.right	-  nForkPos			* nForkSize;
//			rcForkL2.bottom = m_rcControlL.bottom;	
//			m_rcForkD = rcForkD = m_pDCI->ConvertRectS(rcForkL2);		
//			// 포크 #2 세팅
			rcForkL2.left	= m_rcControlL.right		- (nForkPos + 1)	* nForkSize;
			rcForkL2.top	= m_rcControlL.top;
			rcForkL2.right	= m_rcControlL.right		-  nForkPos			* nForkSize;
			rcForkL2.bottom = m_rcControlL.top								- nForkSize;
			m_rcForkD = rcForkD = m_pDCI->ConvertRectS(rcForkL2);		

			// 윙 #1 세팅
			rcWingS1.left	= rcForkD.left		- rcForkD.Width()*nWingGapRatio/100 - rcForkD.Width()*nWingScaleRatio/100;
			rcWingS1.top	= rcForkD.bottom	+ rcForkD.Height()/2				+ rcForkD.Height()*nWingScaleRatio/100;
			rcWingS1.right	= rcForkD.left		- rcForkD.Width()*nWingGapRatio/100;
			rcWingS1.bottom = rcForkD.bottom	- rcForkD.Height()/2				- rcForkD.Height()*nWingScaleRatio/100;

			// 윙 #2 세팅
			rcWingS2.left	= rcForkD.right		+ rcForkD.Width()*nWingGapRatio/100;
			rcWingS2.top	= rcForkD.bottom	+ rcForkD.Height()/2				+ rcForkD.Height()*nWingScaleRatio/100;
			rcWingS2.right	= rcForkD.right		+ rcForkD.Width()*nWingGapRatio/100 + rcForkD.Width()*nWingScaleRatio/100;
			rcWingS2.bottom = rcForkD.bottom	- rcForkD.Height()/2				- rcForkD.Height()*nWingScaleRatio/100;

		}
		// m_nIncrease 사용함으로 enGap2은 사용하지 않음!
//		else if(nForkType == enGap2)
//		{
//
//		//	nForkSize *=2;
//
//			rcWingS2.left = rcForkS.right + rcForkS.Width()*nWingGapRatio/100;
//			rcWingS2.top = rcForkS.top + rcForkS.Height()/2 - rcForkS.Height()*nWingScaleRatio/100;
//			rcWingS2.right = rcForkS.right + rcForkS.Width()*nWingGapRatio/100 + rcForkS.Width()*nWingScaleRatio/100;
//			rcWingS2.bottom = rcForkS.top + rcForkS.Height()/2 + rcForkS.Height()*nWingScaleRatio/100;
//					
//	//		if (m_nForkPos/2 <= 0) nForkPos = 0;
//	//		else if (m_nForkPos/2 >= abs(m_rcControlL.Width()/nForkSize)) nForkPos = (abs(m_rcControlL.Width())/nForkSize - 1)/2;
//	//		else nForkPos = m_nForkPos/2;
//		}
	}
}

void CDciRvCtrl::UpdateControlVertical(	int nType ,
									    CPoint & ptRailS1, CPoint & ptRailS2, 
										CRect & rcRailS1, CRect & rcRailS2, 
										CRect & rcWingS1, CRect & rcWingS2, 
										CRect & rcForkL1, CRect & rcForkL2,
										CRect & rcForkS, CRect & rcForkT, CRect & rcForkD,
										int nForkType /*= 0*/)
{

	if ( nType == enL2R || nType == enR2L)
		return;

	int nForkPos = 0, nForkSize = 1, nRailEndS = 4, nWingGapRatio = 10, nWingScaleRatio = 30;

	rcRailS1.SetRect(ptRailS1.x - nRailEndS/2, ptRailS1.y, ptRailS1.x + nRailEndS/2, ptRailS1.y + nRailEndS);
	rcRailS2.SetRect(ptRailS2.x - nRailEndS/2, ptRailS2.y - nRailEndS, ptRailS2.x + nRailEndS/2, ptRailS2.y);

	nForkSize = abs(m_rcControlL.Width()) ? abs(m_rcControlL.Width()) : 1;

//	if (nForkType == enGap2)
//		nForkSize = abs(m_rcControlL.Width()) ? abs(m_rcControlL.Width())*m_nLen : m_nLen;

//	nForkSize = abs(m_rcControlL.Width()) ? abs(m_rcControlL.Width())*m_nLen : m_nLen;

	if (m_nForkPos <= 0) nForkPos = 0;
	else if (m_nForkPos >= abs(m_rcControlL.Height()/nForkSize)) nForkPos = abs(m_rcControlL.Height())/nForkSize - 1;
	else nForkPos = m_nForkPos;

	if (nType == enT2B)
	{
		rcForkL1.left = m_rcControlL.left;
		rcForkL1.top = m_rcControlL.top - nForkPos*nForkSize;
		rcForkL1.right = m_rcControlL.right;
		rcForkL1.bottom = m_rcControlL.top - (nForkPos+1)*nForkSize;
		m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

		rcWingS1.left = rcForkS.left + rcForkS.Width()/2 - rcForkS.Width()*nWingScaleRatio/100;
		rcWingS1.top = rcForkS.top - rcForkS.Height()*nWingGapRatio/100 - rcForkS.Height()*nWingScaleRatio/100;
		rcWingS1.right = rcForkS.left + rcForkS.Width()/2 + rcForkS.Width()*nWingScaleRatio/100;
		rcWingS1.bottom = rcForkS.top - rcForkS.Height()*nWingGapRatio/100;

		rcWingS2.left = rcForkS.left + rcForkS.Width()/2 - rcForkS.Width()*nWingScaleRatio/100;
		rcWingS2.top = rcForkS.bottom + rcForkS.Height()*nWingGapRatio/100;
		rcWingS2.right = rcForkS.left + rcForkS.Width()/2 + rcForkS.Width()*nWingScaleRatio/100;
		rcWingS2.bottom = rcForkS.bottom + rcForkS.Height()*nWingGapRatio/100 + rcForkS.Height()*nWingScaleRatio/100;

		if (nForkType == enTwin)
		{
			rcForkL2.left = m_rcControlL.left;
			rcForkL2.top = m_rcControlL.top - (nForkPos+1)*nForkSize;
			rcForkL2.right = m_rcControlL.right;
			rcForkL2.bottom = m_rcControlL.top - (nForkPos+2)*nForkSize;
			m_rcForkT = rcForkT = m_pDCI->ConvertRectS(rcForkL2);

			rcWingS2.left = rcForkT.left + rcForkT.Width()/2 - rcForkT.Width()*nWingScaleRatio/100;
			rcWingS2.top = rcForkT.bottom + rcForkT.Height()*nWingGapRatio/100;
			rcWingS2.right = rcForkT.left + rcForkT.Width()/2 + rcForkT.Width()*nWingScaleRatio/100;
			rcWingS2.bottom = rcForkT.bottom + rcForkT.Height()*nWingGapRatio/100 + rcForkT.Height()*nWingScaleRatio/100;

		}
		else if (nForkType == enDouble)
		{
			// 포크사이즈가 반으로 줄어야 함!!
			nForkSize = nForkSize/2;

			// 레일을 반으로 줄여야 함! 
			nRailEndS /= 2;
			rcRailS1.SetRect(ptRailS1.x - nRailEndS/2, ptRailS1.y, ptRailS1.x + nRailEndS/2, ptRailS1.y + nRailEndS);
			rcRailS2.SetRect(ptRailS2.x - nRailEndS/2, ptRailS2.y - nRailEndS, ptRailS2.x + nRailEndS/2, ptRailS2.y);
	
			// 포크 #1 세팅
			rcForkL1.left	= m_rcControlL.left;
			rcForkL1.top	= m_rcControlL.top			- (nForkPos + 1)	* nForkSize;
			rcForkL1.right	= m_rcControlL.right							- nForkSize;
			rcForkL1.bottom = m_rcControlL.top			-  nForkPos			* nForkSize;
			m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

			// 윙 #1 세팅
			rcWingS1.left	= rcForkS.right		- rcForkS.Width()/2						- rcForkS.Width()*nWingScaleRatio/100;
			rcWingS1.top	= rcForkS.top		- rcForkS.Height()*nWingGapRatio/100	- rcForkS.Height()*nWingScaleRatio/100;
			rcWingS1.right	= rcForkS.right		+ rcForkS.Width()/2						+ rcForkS.Width()*nWingScaleRatio/100;
			rcWingS1.bottom = rcForkS.top		- rcForkS.Height()*nWingGapRatio/100;
			
			// 윙 #2 세팅
			rcWingS2.left	= rcForkS.right		- rcForkS.Width()/2						- rcForkS.Width()*nWingScaleRatio/100;
			rcWingS2.top	= rcForkS.bottom	+ rcForkS.Height()*nWingGapRatio/100;
			rcWingS2.right	= rcForkS.right		+ rcForkS.Width()/2						+ rcForkS.Width()*nWingScaleRatio/100;
			rcWingS2.bottom = rcForkS.bottom	+ rcForkS.Height()*nWingGapRatio/100	+ rcForkS.Height()*nWingScaleRatio/100;

			// 포크 #2 세팅
			rcForkL2.left	= m_rcControlL.left							+ nForkSize;
			rcForkL2.top	= m_rcControlL.top		- (nForkPos + 1)	* nForkSize;
			rcForkL2.right	= m_rcControlL.right;
			rcForkL2.bottom = m_rcControlL.top		-  nForkPos			* nForkSize;	
			m_rcForkD = rcForkD = m_pDCI->ConvertRectS(rcForkL2);		
		}
		// m_nIncrease 사용함으로 enGap2은 사용하지 않음!
//		else if (nForkType == enGap2)
//		{
//
//	//		rcWingS2.left = rcForkS.left + 2;
//	//		rcWingS2.top = rcForkS.bottom + 2;
//	///		rcWingS2.right = rcForkS.left + 2;
//	//		rcWingS2.bottom = rcForkS.bottom + 2;
//		
//	//			rcWingS2.left = rcForkS.right + rcForkS.Width()*nWingGapRatio/100;
//	//			rcWingS2.top = rcForkS.top + rcForkS.Height()/2 - rcForkS.Height()*nWingScaleRatio/100;
//	//			rcWingS2.right = rcForkS.right + rcForkS.Width()*nWingGapRatio/100 + rcForkS.Width()*nWingScaleRatio/100;
//	//			rcWingS2.bottom = rcForkS.top + rcForkS.Height()/2 + rcForkS.Height()*nWingScaleRatio/100;		
//		}
	}
	else //if (nType == enB2T)
	{
		rcForkL1.left = m_rcControlL.left;
		rcForkL1.top = m_rcControlL.bottom + (nForkPos+1)*nForkSize;
		rcForkL1.right = m_rcControlL.right;
		rcForkL1.bottom = m_rcControlL.bottom + nForkPos*nForkSize;
		m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

		rcWingS1.left = rcForkS.left + rcForkS.Width()/2 - rcForkS.Width()*nWingScaleRatio/100;
		rcWingS1.top = rcForkS.top - rcForkS.Height()*nWingGapRatio/100 - rcForkS.Height()*nWingScaleRatio/100;
		rcWingS1.right = rcForkS.left + rcForkS.Width()/2 + rcForkS.Width()*nWingScaleRatio/100;
		rcWingS1.bottom = rcForkS.top - rcForkS.Height()*nWingGapRatio/100;

		rcWingS2.left = rcForkS.left + rcForkS.Width()/2 - rcForkS.Width()*nWingScaleRatio/100;
		rcWingS2.top = rcForkS.bottom + rcForkS.Height()*nWingGapRatio/100;
		rcWingS2.right = rcForkS.left + rcForkS.Width()/2 + rcForkS.Width()*nWingScaleRatio/100;
		rcWingS2.bottom = rcForkS.bottom + rcForkS.Height()*nWingGapRatio/100 + rcForkS.Height()*nWingScaleRatio/100;

		if (nForkType == enTwin)
		{
			rcForkL2.left = m_rcControlL.left;
			rcForkL2.top = m_rcControlL.bottom + (nForkPos+2)*nForkSize;
			rcForkL2.right = m_rcControlL.right;
			rcForkL2.bottom = m_rcControlL.bottom + (nForkPos+1)*nForkSize;
			m_rcForkT = rcForkT = m_pDCI->ConvertRectS(rcForkL2);

			rcWingS1.left = rcForkT.left + rcForkT.Width()/2 - rcForkT.Width()*nWingScaleRatio/100;
			rcWingS1.top = rcForkT.top - rcForkT.Height()*nWingGapRatio/100 - rcForkT.Height()*nWingScaleRatio/100;
			rcWingS1.right = rcForkT.left + rcForkT.Width()/2 + rcForkT.Width()*nWingScaleRatio/100;
			rcWingS1.bottom = rcForkT.top - rcForkT.Height()*nWingGapRatio/100;

			if (m_nIncrease != 0)
			{
				if (nForkPos <= (m_nIncrease-1))
				{
					// 홈스탠드 일때 
					nForkPos = 1;
					rcForkL1.left = m_rcControlL.left;
					rcForkL1.top = m_rcControlL.bottom + (nForkPos + 1) * nForkSize;
					rcForkL1.right = m_rcControlL.right;
					rcForkL1.bottom = m_rcControlL.bottom + nForkPos * nForkSize;
					m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

					rcWingS1.left = rcForkS.left + rcForkS.Width() / 2 - rcForkS.Width() * nWingScaleRatio / 100;
					rcWingS1.top = rcForkS.top - rcForkS.Height() * nWingGapRatio / 100 - rcForkS.Height() * nWingScaleRatio / 100;
					rcWingS1.right = rcForkS.left + rcForkS.Width() / 2 + rcForkS.Width() * nWingScaleRatio / 100;
					rcWingS1.bottom = rcForkS.top - rcForkS.Height() * nWingGapRatio / 100;

					rcWingS2.left = rcForkS.left + rcForkS.Width() / 2 - rcForkS.Width() * nWingScaleRatio / 100;
					rcWingS2.top = rcForkS.bottom + rcForkS.Height() * nWingGapRatio / 100;
					rcWingS2.right = rcForkS.left + rcForkS.Width() / 2 + rcForkS.Width() * nWingScaleRatio / 100;
					rcWingS2.bottom = rcForkS.bottom + rcForkS.Height() * nWingGapRatio / 100 + rcForkS.Height() * nWingScaleRatio / 100;

					rcForkL2.left = m_rcControlL.left;
					rcForkL2.top = m_rcControlL.bottom + (nForkPos + 2) * nForkSize;
					rcForkL2.right = m_rcControlL.right;
					rcForkL2.bottom = m_rcControlL.bottom + (nForkPos + 1) * nForkSize;
					m_rcForkT = rcForkT = m_pDCI->ConvertRectS(rcForkL2);

					rcWingS1.left = rcForkT.left + rcForkT.Width() / 2 - rcForkT.Width() * nWingScaleRatio / 100;
					rcWingS1.top = rcForkT.top - rcForkT.Height() * nWingGapRatio / 100 - rcForkT.Height() * nWingScaleRatio / 100;
					rcWingS1.right = rcForkT.left + rcForkT.Width() / 2 + rcForkT.Width() * nWingScaleRatio / 100;
					rcWingS1.bottom = rcForkT.top - rcForkT.Height() * nWingGapRatio / 100;
				}	
				else
				{
					// 랙 일때 
					BOOL bForkReduce = FALSE;		// FALSE : Increase 칸씩 건너뛸 때,			TRUE : 포크를 줄일때 

					if (bForkReduce == FALSE)
					{
						//==================================================================================================================================
						// Increase 칸씩 건너뛸 때
						//==================================================================================================================================
						//nForkPos += (m_nIncrease * 2) + (m_nIncrease / 2);
						nForkPos += (m_nIncrease * 2) + (m_nIncrease / 2);
						rcForkL1.left = m_rcControlL.left;
						rcForkL1.top = m_rcControlL.bottom + ((nForkPos / m_nIncrease) + 1) * nForkSize;
						rcForkL1.right = m_rcControlL.right;
						rcForkL1.bottom = m_rcControlL.bottom + (nForkPos / m_nIncrease) * nForkSize;
						m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

						rcWingS2.left = rcForkS.left + rcForkS.Width() / 2 - rcForkS.Width() * nWingScaleRatio / 100;
						rcWingS2.top = rcForkS.bottom + rcForkS.Height() * nWingGapRatio / 100;
						rcWingS2.right = rcForkS.left + rcForkS.Width() / 2 + rcForkS.Width() * nWingScaleRatio / 100;
						rcWingS2.bottom = rcForkS.bottom + rcForkS.Height() * nWingGapRatio / 100 + rcForkS.Height() * nWingScaleRatio / 100;

						rcForkL2.left = m_rcControlL.left;
						rcForkL2.top = m_rcControlL.bottom + ((nForkPos / m_nIncrease) + 2) * nForkSize;
						rcForkL2.right = m_rcControlL.right;
						rcForkL2.bottom = m_rcControlL.bottom + ((nForkPos / m_nIncrease) + 1) * nForkSize;
						m_rcForkT = rcForkT = m_pDCI->ConvertRectS(rcForkL2);

						rcWingS1.left = rcForkT.left + rcForkT.Width() / 2 - rcForkT.Width() * nWingScaleRatio / 100;
						rcWingS1.top = rcForkT.top - rcForkT.Height() * nWingGapRatio / 100 - rcForkT.Height() * nWingScaleRatio / 100;
						rcWingS1.right = rcForkT.left + rcForkT.Width() / 2 + rcForkT.Width() * nWingScaleRatio / 100;
						rcWingS1.bottom = rcForkT.top - rcForkT.Height() * nWingGapRatio / 100;
						//----------------------------------------------------------------------------------------------------------------------------------
					}
					else
					{
						//==================================================================================================================================
						// 포크를 줄여서 보여줄 때 - 현재는 구현되지 않음! 
						//==================================================================================================================================
						nForkPos += (m_nIncrease * 2) + (m_nIncrease / 2);
						rcForkL1.left = m_rcControlL.left;
						rcForkL1.top = m_rcControlL.bottom + ((nForkPos / m_nIncrease) + 1) * nForkSize;
						rcForkL1.right = m_rcControlL.right;
						rcForkL1.bottom = m_rcControlL.bottom + (nForkPos / m_nIncrease) * nForkSize;
						m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

						rcWingS2.left = rcForkS.left + rcForkS.Width() / 2 - rcForkS.Width() * nWingScaleRatio / 100;
						rcWingS2.top = rcForkS.bottom + rcForkS.Height() * nWingGapRatio / 100;
						rcWingS2.right = rcForkS.left + rcForkS.Width() / 2 + rcForkS.Width() * nWingScaleRatio / 100;
						rcWingS2.bottom = rcForkS.bottom + rcForkS.Height() * nWingGapRatio / 100 + rcForkS.Height() * nWingScaleRatio / 100;

						rcForkL2.left = m_rcControlL.left;
						rcForkL2.top = m_rcControlL.bottom + ((nForkPos / m_nIncrease) + 2) * nForkSize;
						rcForkL2.right = m_rcControlL.right;
						rcForkL2.bottom = m_rcControlL.bottom + ((nForkPos / m_nIncrease) + 1) * nForkSize;
						m_rcForkT = rcForkT = m_pDCI->ConvertRectS(rcForkL2);

						rcWingS1.left = rcForkT.left + rcForkT.Width() / 2 - rcForkT.Width() * nWingScaleRatio / 100;
						rcWingS1.top = rcForkT.top - rcForkT.Height() * nWingGapRatio / 100 - rcForkT.Height() * nWingScaleRatio / 100;
						rcWingS1.right = rcForkT.left + rcForkT.Width() / 2 + rcForkT.Width() * nWingScaleRatio / 100;
						rcWingS1.bottom = rcForkT.top - rcForkT.Height() * nWingGapRatio / 100;
						//----------------------------------------------------------------------------------------------------------------------------------
					}
				}
			}
		}
		else if (nForkType == enDouble)
		{
			// 포크사이즈가 반으로 줄어야 함!!
			nForkSize = nForkSize/2;

			// 레일을 반으로 줄여야 함! 
			nRailEndS /= 2;
			rcRailS1.SetRect(ptRailS1.x - nRailEndS/2, ptRailS1.y, ptRailS1.x + nRailEndS/2, ptRailS1.y + nRailEndS);
			rcRailS2.SetRect(ptRailS2.x - nRailEndS/2, ptRailS2.y - nRailEndS, ptRailS2.x + nRailEndS/2, ptRailS2.y);
	
			// 포크 #1 세팅
			rcForkL1.left = m_rcControlL.left;
			rcForkL1.top = m_rcControlL.bottom + (nForkPos+1)*nForkSize;
			rcForkL1.right = m_rcControlL.right - nForkSize;
			rcForkL1.bottom = m_rcControlL.bottom + nForkPos*nForkSize;
			m_rcForkS = rcForkS = m_pDCI->ConvertRectS(rcForkL1);

			// 윙 #1 세팅
			rcWingS1.left	= rcForkS.right		- rcForkS.Width()/2						- rcForkS.Width()*nWingScaleRatio/100;
			rcWingS1.top	= rcForkS.top		- rcForkS.Height()*nWingGapRatio/100	- rcForkS.Height()*nWingScaleRatio/100;
			rcWingS1.right	= rcForkS.right		+ rcForkS.Width()/2						+ rcForkS.Width()*nWingScaleRatio/100;
			rcWingS1.bottom = rcForkS.top		- rcForkS.Height()*nWingGapRatio/100;
			
			// 윙 #2 세팅
			rcWingS2.left	= rcForkS.right		- rcForkS.Width()/2						- rcForkS.Width()*nWingScaleRatio/100;
			rcWingS2.top	= rcForkS.bottom	+ rcForkS.Height()*nWingGapRatio/100;
			rcWingS2.right	= rcForkS.right		+ rcForkS.Width()/2						+ rcForkS.Width()*nWingScaleRatio/100;
			rcWingS2.bottom = rcForkS.bottom	+ rcForkS.Height()*nWingGapRatio/100	+ rcForkS.Height()*nWingScaleRatio/100;

			// 포크 #2 세팅
			rcForkL2.left	= m_rcControlL.left  + nForkSize;
			rcForkL2.top	= m_rcControlL.bottom	+ (nForkPos+1)	* nForkSize;
			rcForkL2.right	= m_rcControlL.right;//	+ nForkSize;
			rcForkL2.bottom = m_rcControlL.bottom	+ nForkPos		* nForkSize;	
			m_rcForkD = rcForkD = m_pDCI->ConvertRectS(rcForkL2);		
		}
	}
}
void CDciRvCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	try
	{
		// [LGLS 2026-08-22] 창고 모니터링 보기 문자. 비어 있으면 종전처럼 m_strText(호기 번호)를 쓴다.
		//   여기서 한 번 정해 두고 아래 포크 텍스트 출력에 그대로 넘긴다.
		//   종전에 별도로 덧그렸더니 호기 번호 위에 작업번호가 겹쳐 찍혔다.
		COLORREF clrExDisp;
		CString  strExDisp = GetExtraTextSafe(&clrExDisp);
		strExDisp.Trim();
		CString  strDispText = strExDisp.IsEmpty() ? m_strText : strExDisp;
		COLORREF clrDispText = strExDisp.IsEmpty() ? m_clrFgColor : clrExDisp;

		CPoint ptRailS1, ptRailS2;					// 레일끝의 포인트???
		CRect rcControlS;							// 컨트롤 헨들러??
		CRect rcRailS1, rcRailS2;					// 레일 헨들러???		// S1:좌측(아래), S2:우측(위)
		CRect rcWingS1, rcWingS2;					// 윙 헨들러???			// S1:좌측(아래), S2:우측(위)
		CRect rcForkL1, rcForkL2;					// 포크 임시 헨들러???	// L1:Single용,   L2:Twin, Double용		
		CRect rcForkS, rcForkT, rcForkD;			// 포크 헨들러???		// S :Single용,   T :Twin용		D :Double용
		CRect rcIntersect;

//		int nForkPos = 0, nForkSize = 1, nRailEndS = 4, nWingGapRatio = 10, nWingScaleRatio = 30;
		rcControlS = m_pDCI->ConvertRectS(m_rcControlL);

		switch (m_nForkType)
		{
		case enSingle:
			{
				switch (m_nType)
				{
				case enL2R:
				case enR2L:
					{
						// 레일끝에 점 표시하기
						ptRailS1.x = rcControlS.left;
						ptRailS1.y = rcControlS.top + abs(rcControlS.Height()/2);
						ptRailS2.x = rcControlS.right;
						ptRailS2.y = rcControlS.top + abs(rcControlS.Height()/2);

						UpdateControlHorizental(m_nType, 
												ptRailS1, ptRailS2, 
												rcRailS1, rcRailS2, 
												rcWingS1, rcWingS2, 
												rcForkL1, rcForkL2,
												rcForkS, rcForkT, rcForkD);
					}
					break;
				case enT2B:
				case enB2T:
					{
						// 레일끝에 점 표시하기
						ptRailS1.x = rcControlS.left + abs(rcControlS.Width()/2);
						ptRailS1.y = rcControlS.top;
						ptRailS2.x = rcControlS.left + abs(rcControlS.Width()/2);
						ptRailS2.y = rcControlS.bottom;

						UpdateControlVertical(	m_nType, 
												ptRailS1, ptRailS2, 
												rcRailS1, rcRailS2, 
												rcWingS1, rcWingS2, 
												rcForkL1, rcForkL2,
												rcForkS, rcForkT, rcForkD);
					}
					break;
				case enTypeSize : 
					{
						// 레일끝에 점 표시하기
						ptRailS1.x = rcControlS.left;
						ptRailS1.y = rcControlS.top + abs(rcControlS.Height()/2);
						ptRailS2.x = rcControlS.right;
						ptRailS2.y = rcControlS.top + abs(rcControlS.Height()/2);

						UpdateControlHorizental(m_nType, 
												ptRailS1, ptRailS2, 
												rcRailS1, rcRailS2, 
												rcWingS1, rcWingS2, 
												rcForkL1, rcForkL2,
												rcForkS, rcForkT, rcForkD);					
					
					}
					break;				
				
				}
			}
			break;
		case enTwin:
			{
				switch (m_nType)
				{
				case enL2R:
				case enR2L:
					{
						// 레일끝에 점 표시하기
						ptRailS1.x = rcControlS.left;
						ptRailS1.y = rcControlS.top + abs(rcControlS.Height()/2);
						ptRailS2.x = rcControlS.right;
						ptRailS2.y = rcControlS.top + abs(rcControlS.Height()/2);

						UpdateControlHorizental(m_nType, 
												ptRailS1, ptRailS2, 
												rcRailS1, rcRailS2, 
												rcWingS1, rcWingS2, 
												rcForkL1, rcForkL2,
												rcForkS, rcForkT, rcForkD, 
												enTwin);
					}
					break;
				case enT2B:
				case enB2T:
					{
						// 레일끝에 점 표시하기
						ptRailS1.x = rcControlS.left + abs(rcControlS.Width()/2);
						ptRailS1.y = rcControlS.top;
						ptRailS2.x = rcControlS.left + abs(rcControlS.Width()/2);
						ptRailS2.y = rcControlS.bottom;

						UpdateControlVertical(	m_nType, 
												ptRailS1, ptRailS2, 
												rcRailS1, rcRailS2, 
												rcWingS1, rcWingS2, 
												rcForkL1, rcForkL2,
												rcForkS, rcForkT, rcForkD, 
												enTwin);
					}
				case enTypeSize:
					{
						// 레일끝에 점 표시하기
						ptRailS1.x = rcControlS.left + abs(rcControlS.Width()/2);
						ptRailS1.y = rcControlS.top;
						ptRailS2.x = rcControlS.left + abs(rcControlS.Width()/2);
						ptRailS2.y = rcControlS.bottom;

						UpdateControlVertical(	m_nType, 
												ptRailS1, ptRailS2, 
												rcRailS1, rcRailS2, 
												rcWingS1, rcWingS2, 
												rcForkL1, rcForkL2,
												rcForkS, rcForkT, rcForkD, 
												enTwin);						




					}
					break;
				}
			}
			break;
		case enDouble:
			{
				switch (m_nType)
				{
				case enL2R:
				case enR2L:
					{
						// 가로 타입일때는 Top을 늘려야 함!		m_rcControlL
//						if (m_nIncrease == 0)
//						{
//							int nForkSize = abs(m_rcControlL.Height()) ? abs(m_rcControlL.Height()) : 1;
//							m_rcControlL.top = abs(m_rcControlL.top) + abs(rcControlS.Height()/2);
//
//							m_nIncrease++;
//						}

						CString strTemp;
						strTemp.Format(_T("Bottom = %d \nTop = %d \nLeft = %d \nRight = %d "), 
							m_rcControlL.bottom, m_rcControlL.top, m_rcControlL.left, m_rcControlL.right);
//						AfxMessageBox(strTemp);

						// 레일끝에 점 표시하기
						ptRailS1.x = rcControlS.left;
						ptRailS1.y = rcControlS.top + abs(rcControlS.Height()/2);		// 레일위치 이동
						ptRailS2.x = rcControlS.right;
						ptRailS2.y = rcControlS.top + abs(rcControlS.Height()/2);		// 레일위치 이동

						UpdateControlHorizental(m_nType, 
												ptRailS1, ptRailS2, 
												rcRailS1, rcRailS2, 
												rcWingS1, rcWingS2, 
												rcForkL1, rcForkL2,
												rcForkS, rcForkT, rcForkD, 
												enDouble);
					}
					break;
				case enT2B:
				case enB2T:
					{
						// 세로 타입일때는 Right를 늘려야 함!
//						if (m_nIncrease == 0)
//						{
//							int nForkSize = abs(m_rcControlL.Width()) ? abs(m_rcControlL.Width()) : 1;
//							m_rcControlL.right = m_rcControlL.right + rcControlS.Width();
//
//							m_nIncrease++;
//						}

						// 레일끝에 점 표시하기
						ptRailS1.x = rcControlS.left + abs(rcControlS.Width()/2);		// 레일위치 이동
						ptRailS1.y = rcControlS.top;
						ptRailS2.x = rcControlS.left + abs(rcControlS.Width()/2);		// 레일위치 이동
						ptRailS2.y = rcControlS.bottom;

						UpdateControlVertical(	m_nType, 
												ptRailS1, ptRailS2, 
												rcRailS1, rcRailS2, 
												rcWingS1, rcWingS2, 
												rcForkL1, rcForkL2,
												rcForkS, rcForkT, rcForkD, 
												enDouble);
					}
					break;
				}
			}
			break;
	
		// Sc 크기 직사각형으로 늘림
		//case enGap2:
		//	{
		//		switch (m_nType)
		//		{
		//		case enL2R:
		//		case enR2L:
		//			{
		//				// 레일끝에 점 표시하기
		//				ptRailS1.x = rcControlS.left;
		//				ptRailS1.y = rcControlS.top + abs(rcControlS.Height()/2);
		//				ptRailS2.x = rcControlS.right;
		//				ptRailS2.y = rcControlS.top + abs(rcControlS.Height()/2);

		//				UpdateControlHorizental(m_nType, 
		//										ptRailS1, ptRailS2, 
		//										rcRailS1, rcRailS2, 
		//										rcWingS1, rcWingS2, 
		//										rcForkL1, rcForkL2,
		//										rcForkS, rcForkT, rcForkD, 
		//										enGap2);
		//			}
		//			break;
		//		case enT2B:
		//		case enB2T:
		//			{
		//				// 레일끝에 점 표시하기
		//				ptRailS1.x = rcControlS.left + abs(rcControlS.Width()/2);
		//				ptRailS1.y = rcControlS.top;
		//				ptRailS2.x = rcControlS.left + abs(rcControlS.Width()/2);
		//				ptRailS2.y = rcControlS.bottom;

		//				UpdateControlVertical(	m_nType, 
		//										ptRailS1, ptRailS2, 
		//										rcRailS1, rcRailS2, 
		//										rcWingS1, rcWingS2, 
		//										rcForkL1, rcForkL2,
		//										rcForkS, rcForkT, rcForkD, 
		//										enGap2);
		//			}
		//		case enTypeSize:
		//			{
		//				// 레일끝에 점 표시하기
		//				ptRailS1.x = rcControlS.left + abs(rcControlS.Width()/2);
		//				ptRailS1.y = rcControlS.top;
		//				ptRailS2.x = rcControlS.left + abs(rcControlS.Width()/2);
		//				ptRailS2.y = rcControlS.bottom;

		//				UpdateControlVertical(	m_nType, 
		//										ptRailS1, ptRailS2, 
		//										rcRailS1, rcRailS2, 
		//										rcWingS1, rcWingS2, 
		//										rcForkL1, rcForkL2,
		//										rcForkS, rcForkT, rcForkD, 
		//										enGap2);						




		//			}
		//			break;
		//		}
		//	}
		//	break;
		}

		CPen* pOldPen = NULL;
		CPen penRail(PS_SOLID, 1, m_clrRail);

		CBrush* pOldBrush = NULL;
		CBrush bgBrush(m_clrBgColor), wingBrush(m_clrWing), railBrush(m_clrRail);

		int nOldMode = pDC->SetBkMode(m_nFgMode);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);

		/////////////////////////////////////////////////////////////////////////////////////////

		if (m_nBgMode == TRANSPARENT)
			pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		else
			pOldBrush = pDC->SelectObject(&bgBrush);
		pOldPen = (CPen*) pDC->SelectStockObject(NULL_PEN);
		pDC->Rectangle(rcControlS);

		/////////////////////////////////////////////////////////////////////////////////////////

		pDC->SelectObject(&penRail);
		pDC->MoveTo(ptRailS1);
		pDC->LineTo(ptRailS2);

		pDC->SelectObject(&railBrush);
		pDC->Ellipse(rcRailS1);
		pDC->Ellipse(rcRailS2);

		/////////////////////////////////////////////////////////////////////////////////////////

		m_pDCI->DrawButton(pDC, rcForkL1, m_clrFork, m_bClick);

		LOGFONT m_logfont;
		CFont m_font ;
		CFont * pOldFont;

		int nProductionValue = 8;
		int nSize;
		int nSizeH = nSize = rcForkL1.top - rcForkL1.bottom;
		int nSizeW = rcForkL1.right - rcForkL1.left;
		if (nSizeH > nSizeW)
			nSize = nSizeW;

		if (nSize == 2)
			nSize = (nSize * nProductionValue) ;
		else if (nSize == 1)
			nSize = (nSize * nProductionValue * 2) ;


		HDC hDC;
		hDC = pDC->m_hDC;
		hDC = pDC->GetSafeHdc();
		memset(&m_logfont, 0, sizeof(LOGFONT));
		m_logfont.lfQuality = PROOF_QUALITY;
		m_logfont.lfHeight = m_nFontSize;
//		m_logfont.lfHeight = nSize;//long((-MulDiv(m_nFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 )) / m_pDCI->GetScale());
		m_logfont.lfWeight = FW_BOLD;
		lstrcpy(m_logfont.lfFaceName, _T("Arial"));
		m_font.CreateFontIndirect(&m_logfont);
		pOldFont = pDC->SelectObject(&m_font);

		//pDC->DrawText(m_strText, rcForkL1, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		m_pDCI->DrawText(pDC, rcForkL1, strDispText, clrDispText);

		if (m_nForkType == enTwin)
//		if (m_nForkType != enSingle)
		{
			m_pDCI->DrawButton(pDC, rcForkL2, m_clrFork2, m_bClick);
			m_pDCI->DrawText(pDC, rcForkL2, strDispText, clrDispText);
		}

//		if (m_nForkType == enDouble)
//		{
//			m_pDCI->DrawButton(pDC, rcForkL3, m_clrFork, m_bClick);
//			m_pDCI->DrawText(pDC, rcForkL3, m_strText, m_clrFgColor);
//		}

		rcIntersect.IntersectRect(rcWingS1, rcControlS);
		if (rcWingS1.EqualRect(rcIntersect))
		{
			//pDC->FillSolidRect(rcWingS1, m_clrWing);
			pDC->FillSolidRect(rcWingS1, RGB(255, 255, 255));
			pDC->Draw3dRect(rcWingS1, RGB(255,255,255), RGB(0,0,0));
		}
		
		rcIntersect.IntersectRect(rcWingS2, rcControlS);
		if (rcWingS2.EqualRect(rcIntersect))
		{
			pDC->FillSolidRect(rcWingS2, m_clrWing);
			pDC->Draw3dRect(rcWingS2, RGB(255,255,255), RGB(0,0,0));
		}

		// 필요시 이부분을 주석 처리해야할수도 있음!
		if (/*m_nForkType == enGap2 || */m_nForkType == enSingle)
		{
			m_pDCI->DrawButton(pDC, rcForkL1, m_clrFork, m_bClick);
			m_pDCI->DrawText(pDC, rcForkL1, strDispText, clrDispText);
		}

		if (m_nForkType != enSingle/* && m_nForkType != enGap2*/)
		{
			m_pDCI->DrawButton(pDC, rcForkL2, m_clrFork2, m_bClick);
			m_pDCI->DrawText(pDC, rcForkL2, strDispText, clrDispText);
		}
	
		// 더블 포크 일때는 하나의 그리드를 넘어가니깐... IntersectRect와 EqualRect를 체크하지 않는다.
		// 아래의 문장이 없으면 더블 포크일때 윙이 표현되지 않음!!
		if (m_nForkType == enDouble)
		{
			pDC->FillSolidRect(rcWingS1, m_clrWing);
			pDC->Draw3dRect(rcWingS1, RGB(255,255,255), RGB(0,0,0));
		
			pDC->FillSolidRect(rcWingS2, m_clrWing);
			pDC->Draw3dRect(rcWingS2, RGB(255,255,255), RGB(0,0,0));
		}

		/////////////////////////////////////////////////////////////////////////////////////////
//==========================================================================================================================================================
//		// 화물 감지 표시 - CScInfo::InvokeControl(), CScInfo::InvokeControl()에서 
//		//					m_pControl->m_clrWing = GetPostColor(); 이 부분 주석 처리할것!!
//==========================================================================================================================================================
		int g = 2, s = 4;
		CRect rcTemp;

		// 1번 포크
		if (m_nProd == 1)
		{
			rcTemp = m_pDCI->ConvertRectS(rcForkL1);
			IndicateProdSensor(pDC, rcTemp, g, s, 0x000000);
		}
		// 2번 포크
		else if (m_nProd == 2)
		{
			rcTemp = m_pDCI->ConvertRectS(rcForkL2);
			IndicateProdSensor(pDC, rcTemp, g, s, 0x000000);
		}
		// 화물 2개 동시감지
		else if (m_nProd == 3)
		{
			// 1번 포크
			rcTemp = m_pDCI->ConvertRectS(rcForkL1);
			IndicateProdSensor(pDC, rcTemp, g, s, 0x000000);

			// 2번 포크
			rcTemp = m_pDCI->ConvertRectS(rcForkL2);
			IndicateProdSensor(pDC, rcTemp, g, s, 0x000000);
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------------

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldMode);

		//pDC->SelectObject(pOldFont);
		
	}
	catch (CException* e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciRvCtrl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
}

void CDciRvCtrl::IndicateProdSensor(CDC * pDC, CRect rect, int ng, int ns, COLORREF nColor)
{
	pDC->FillSolidRect(rect.left+ng,		rect.top+ng,		ns, ns, nColor);
	pDC->FillSolidRect(rect.right-ng-ns,	rect.top+ng,		ns, ns, nColor);
	pDC->FillSolidRect(rect.left+ng,		rect.bottom-ng-ns,	ns, ns, nColor);
	pDC->FillSolidRect(rect.right-ng-ns,	rect.bottom-ng-ns,	ns, ns, nColor);	
}

BOOL CDciRvCtrl::SetClick(CWnd* pWnd, const CPoint& ptClickS)
{

	CString strTemp;

	switch (m_nForkType)
	{
	case enSingle:
		if (m_bClick = (m_rcForkS.PtInRect(ptClickS)))
			InvalidateControl(pWnd, FALSE);

//		strTemp.Format(_T("Bottom = %d \nTop = %d \nLeft = %d \nRight = %d \n클릭여부 = %d"), m_rcForkS.bottom, m_rcForkS.top, m_rcForkS.left, m_rcForkS.right, m_bClick);
//		AfxMessageBox(strTemp);
		break;
	case enTwin:
		if (m_bClick = (m_rcForkS.PtInRect(ptClickS) || m_rcForkT.PtInRect(ptClickS)) )
			InvalidateControl(pWnd, FALSE);

//		strTemp.Format(_T("Bottom = %d \nTop = %d \nLeft = %d \nRight = %d \n클릭여부 = %d"), m_rcForkT.bottom, m_rcForkT.top, m_rcForkT.left, m_rcForkT.right, m_bClick);
//		AfxMessageBox(strTemp);
		break;
	case enDouble:
		if (m_bClick = (m_rcForkS.PtInRect(ptClickS) || m_rcForkD.PtInRect(ptClickS)) )
			InvalidateControl(pWnd, FALSE);
		
//		strTemp.Format(_T("Bottom = %d \nTop = %d \nLeft = %d \nRight = %d \n클릭여부 = %d"), m_rcForkD.bottom, m_rcForkD.top, m_rcForkD.left, m_rcForkD.right, m_bClick);
//		AfxMessageBox(strTemp);
		break;
//	case enGap2:
//		if (m_bClick = (m_rcForkS.PtInRect(ptClickS) || m_rcForkD.PtInRect(ptClickS)) )
//			InvalidateControl(pWnd, FALSE);
//		break;
	}

//	if (m_bClick = (m_rcForkS.PtInRect(ptClickS) || m_rcForkT.PtInRect(ptClickS) || m_rcForkD.PtInRect(ptClickS)) )
//		InvalidateControl(pWnd, FALSE);

	return m_bClick;
}
