// DciMaster.cpp: implementation of the CDciMaster class.
//

#include "StdAfx.h"
#include "DciMaster.h"


// CDciMaster
CDciMaster::CDciMaster(void)
{
	m_rcLayoutL.SetRect(1,1,1,1);
	m_rcLayoutS.SetRect(1,1,1,1);	
	m_rcLayoutLTemp.SetRect(1,1,1,1);
	m_rcLayoutCenter.SetRect(1,1,1,1);
	m_ptOrginL.x = m_ptOrginS.x = 0;	
	m_ptOrginL.y = m_ptOrginS.y = 0;	
	m_nScale = 1;

	m_dciCreator.Add(RUNTIME_CLASS(CDciControl));
	m_dciCreator.Add(RUNTIME_CLASS(CDciGridCtrl));
	m_dciCreator.Add(RUNTIME_CLASS(CDciButtonCtrl));
	m_dciCreator.Add(RUNTIME_CLASS(CDciRackCtrl));
	m_dciCreator.Add(RUNTIME_CLASS(CDciTrackCtrl));
	m_dciCreator.Add(RUNTIME_CLASS(CDciRvCtrl));
	m_dciCreator.Add(RUNTIME_CLASS(CDciOpboxCtrl));
	m_dciCreator.Add(RUNTIME_CLASS(CDciStaticCtrl));
	m_dciCreator.Add(RUNTIME_CLASS(CDciStatusCtrl));
	m_dciCreator.Add(RUNTIME_CLASS(CDciStatisticCtrl));
}

CDciMaster::~CDciMaster(void)
{
}

// CDciMaster ¸â¹ö ÇÔ¼ö
CDciControl* CDciMaster::CreateDciObject(int nIndex, const CDciControl* pDciCtrlSrc /* = NULL */)
{
	if ((nIndex < 0) || (nIndex >= m_dciCreator.GetSize()) || (m_dciCreator[nIndex] == NULL))
		return NULL;

	CDciControl* pDciControl = NULL;
	if (pDciControl = (CDciControl*)m_dciCreator[nIndex]->CreateObject())
	{
		pDciControl->InitControl(this);
		ClonDciObject(pDciCtrlSrc, pDciControl);
	}
	
	return pDciControl;
}

CDciControl* CDciMaster::CreateDciObject(const CString& strClassName, const CDciControl* pDciCtrlSrc /* = NULL */)
{
	if (strClassName.IsEmpty())
		return NULL;

	for (int i=0; i<m_dciCreator.GetSize(); ++i)
	{
		if (strClassName == m_dciCreator[i]->m_lpszClassName)
			return CreateDciObject(i, pDciCtrlSrc);
	}

	return NULL;	
}

/*
CDciControl* CDciMaster::CreateDciObject(const LPCSTR& strClassName, const CDciControl* pDciCtrlSrc )
{
	if (strClassName == _T(""))
		return NULL;

	for (int i=0; i<m_dciCreator.GetSize(); ++i)
	{
		if (strClassName == m_dciCreator[i]->m_lpszClassName)
			return CreateDciObject(i, pDciCtrlSrc);
	}

	return NULL;	
}
//*/

void CDciMaster::ClonDciObject(const CDciControl* pDciCtrlSrc, CDciControl* pDciCtrlTar)
{
	if ((pDciCtrlSrc == NULL) || (pDciCtrlTar == NULL))
		return;

	CDciPropertyArray properties;
	((CDciControl*)pDciCtrlSrc)->UpdatePropNames(properties);
	((CDciControl*)pDciCtrlSrc)->UpdatePropValues(properties, FALSE);
	pDciCtrlTar->UpdatePropValues(properties);

	((CDciControl*)pDciCtrlSrc)->UpdateItemNames(properties);
	for (int i=0; i<((CDciControl*)pDciCtrlSrc)->GetItemSize(); ++i)
	{
		((CDciControl*)pDciCtrlSrc)->UpdateItemValues(i, properties, FALSE);
		pDciCtrlTar->UpdateItemValues(i, properties);
	}
}

CRuntimeClass* CDciMaster::GetDciClass(int nIndex)
{
	if ((nIndex < 0) || (nIndex >= m_dciCreator.GetSize()))
		return NULL;

	return m_dciCreator[nIndex];
}

CRuntimeClass* CDciMaster::GetDciClass(const CString& strClassName)
{
	if (strClassName.IsEmpty())
		return NULL;

	for (int i=0; i<m_dciCreator.GetSize(); ++i)
	{
		if (strClassName == m_dciCreator[i]->m_lpszClassName)
			return m_dciCreator[i];
	}

	return NULL;
}

int CDciMaster::GetDciClassIndex(CRuntimeClass* pClass)
{
	if (pClass == NULL)
		return -1;

	for (int i=0; i<m_dciCreator.GetSize(); ++i)
	{
		if (pClass == m_dciCreator[i])
			return i;
	}
	
	return -1;
}

int CDciMaster::GetDciClassSize()
{
	return m_dciCreator.GetSize();
}

void CDciMaster::RecalcLayout()
{
	int nWidthS	= abs(m_rcLayoutS.Width()) ? abs(m_rcLayoutS.Width()) : 1;
	int nHeightS = abs(m_rcLayoutS.Height()) ? abs(m_rcLayoutS.Height()) : 1;
	int nWidthL = abs(m_rcLayoutL.Width()) ? abs(m_rcLayoutL.Width()) : 1;
	int nHeightL = abs(m_rcLayoutL.Height()) ? abs(m_rcLayoutL.Height()) : 1;

	// SET SCALE FACTOR
	if (nWidthL / nHeightL > nWidthS / nHeightS)
		m_nScale = nWidthS / nWidthL;
	else
		m_nScale = nHeightS / nHeightL;

	while ((nWidthS < m_nScale*nWidthL) || (nHeightS < m_nScale*nHeightL))
		--m_nScale;
	m_nScale = (m_nScale > 0) ? m_nScale : 1;

	// SET ORGIN POINT
	m_ptOrginL.x = m_rcLayoutL.left;
	m_ptOrginL.y = m_rcLayoutL.bottom;
	m_ptOrginS.x = m_rcLayoutS.left + abs(nWidthS - m_nScale * nWidthL) / 2;
	m_ptOrginS.y = m_rcLayoutS.bottom - abs(nHeightS - m_nScale * nHeightL) / 2;
}

CPoint CDciMaster::ConvertPointS(int x, int y)
{
	CPoint ptScreen;
// 	ptScreen.x = m_ptOrginS.x + m_nScale * x;
// 	ptScreen.y = m_ptOrginS.y - m_nScale * y;
	ptScreen.x = m_ptOrginS.x + m_nScale * (x - m_ptOrginL.x);
	ptScreen.y = m_ptOrginS.y - m_nScale * (y - m_ptOrginL.y);
	return ptScreen;
}

CPoint CDciMaster::ConvertPointL(int x, int y)
{
	CPoint ptLogical;
//	ptLogical.x = (x - m_ptOrginS.x) / m_nScale;
//	ptLogical.y = (m_ptOrginS.y - y) / m_nScale;
	ptLogical.x = m_ptOrginL.x + (x - m_ptOrginS.x) / m_nScale;
	ptLogical.y = m_ptOrginL.y + (m_ptOrginS.y - y) / m_nScale;
	return ptLogical;
}

CRect CDciMaster::ConvertRectS(int l, int t, int r, int b)
{
	CRect rcRectS;
// 	rcRectS.left	= m_ptOrginS.x + m_nScale * l;
// 	rcRectS.top		= m_ptOrginS.y - m_nScale * t;
// 	rcRectS.right	= m_ptOrginS.x + m_nScale * r;
// 	rcRectS.bottom	= m_ptOrginS.y - m_nScale * b;
	rcRectS.left	= m_ptOrginS.x + m_nScale * (l - m_ptOrginL.x);
	rcRectS.top		= m_ptOrginS.y - m_nScale * (t - m_ptOrginL.y);
	rcRectS.right	= m_ptOrginS.x + m_nScale * (r - m_ptOrginL.x);
	rcRectS.bottom	= m_ptOrginS.y - m_nScale * (b - m_ptOrginL.y);
	return rcRectS;
}

CRect CDciMaster::ConvertRectL(int l, int t, int r, int b)
{
	CRect rcRectL;
// 	rcRectL.left	= (l - m_ptOrginS.x) / m_nScale;
// 	rcRectL.top		= (m_ptOrginS.y - t) / m_nScale;
// 	rcRectL.right	= (r - m_ptOrginS.x) / m_nScale;
// 	rcRectL.bottom	= (m_ptOrginS.y - b) / m_nScale;
	rcRectL.left	= m_ptOrginL.x + (l - m_ptOrginS.x) / m_nScale;
	rcRectL.top		= m_ptOrginL.y + (m_ptOrginS.y - t) / m_nScale;
	rcRectL.right	= m_ptOrginL.x + (r - m_ptOrginS.x) / m_nScale;
	rcRectL.bottom	= m_ptOrginL.y + (m_ptOrginS.y - b) / m_nScale;
	return rcRectL;
}

void CDciMaster::DrawText(CDC* pDC, const CRect& rcRectL, const CString& strText, COLORREF clrText)
{
	if (pDC == NULL || rcRectL.IsRectNull() || strText.IsEmpty())
		return;

	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	int nOldTextColor = pDC->SetTextColor(clrText);

	pDC->DrawText(strText, ConvertRectS(rcRectL), DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	pDC->SetTextColor(nOldTextColor);
	pDC->SetBkMode(nOldBkMode);
}

void CDciMaster::Draw3dSolidRect(CDC* pDC, const CRect& rcRectL, COLORREF clrBrush, COLORREF clrLeftTop, COLORREF clrRightBottom)
{
	if (pDC == NULL || rcRectL.IsRectNull())
		return;

	CRect rcRectS = ConvertRectS(rcRectL);
	pDC->FillSolidRect(rcRectS, clrBrush);
	pDC->Draw3dRect(rcRectS, clrLeftTop, clrRightBottom);
}

void CDciMaster::DrawButton(CDC* pDC, const CRect& rcRectL, COLORREF clrBrush, BOOL bClick)
{
	if (pDC == NULL || rcRectL.IsRectNull())
		return;

// 	COLORREF clrBlack = RGB(0,0,0);
// 	COLORREF clrWhite = RGB(255,255,255);
	CRect rcRectS = ConvertRectS(rcRectL);
	pDC->FillSolidRect(rcRectS, clrBrush);

	if (bClick)
	{
		pDC->Draw3dRect(rcRectS, RGB(0,0,0), RGB(255,255,255));
// 		rcRectS.DeflateRect(1, 1, 0, 0);
// 		pDC->Draw3dRect(rcRectS, RGB(192,192,192), RGB(255,255,255));
	}
	else
	{
		pDC->Draw3dRect(rcRectS, RGB(255,255,255), RGB(0,0,0));
// 		rcRectS.DeflateRect(0, 0, 1, 1);
// 		pDC->Draw3dRect(rcRectS, RGB(255,255,255), RGB(192,192,192));
	}
}
