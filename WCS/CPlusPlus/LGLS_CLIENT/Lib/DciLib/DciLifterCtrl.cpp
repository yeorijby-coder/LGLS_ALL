// DciLifterCtrl.cpp: implementation of the CDciLifterCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DciLifterCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDciLifterCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

CDciLifterCtrl::CDciLifterCtrl()
{

}

CDciLifterCtrl::~CDciLifterCtrl()
{

}

void CDciLifterCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);
	m_nBgMode = TRANSPARENT;

	m_nSize = 0;
}

int CDciLifterCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enLifterCtrlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("size"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciLifterCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
		m_nSize = atoi(properties[i++].m_strValue);
		m_items.SetSize(m_nSize);
	}
	else
	{
		properties[i++].m_strValue.Format(_T("%d"), m_nSize);
	}

	return i;
}

int CDciLifterCtrl::UpdateItemNames(CDciPropertyArray& properties)
{
	int i = 0;

	properties.SetSize(6);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("id"));
	properties[i++].SetProperty(CDciProperty::PT_PERCENT, _T("l"));
	properties[i++].SetProperty(CDciProperty::PT_PERCENT, _T("b"));
	properties[i++].SetProperty(CDciProperty::PT_PERCENT, _T("r"));
	properties[i++].SetProperty(CDciProperty::PT_PERCENT, _T("t"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("color"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciLifterCtrl::UpdateItemValues(int nIndex, CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	if ((nIndex < 0) || (nIndex >= m_items.GetSize()))
		return 0;

	int i = 0;

	if (bSaveObject)
	{
		m_items[nIndex].m_nLifterID			= atoi(properties[i++].m_strValue);
		m_items[nIndex].m_rcLifterL.left	= atoi(properties[i++].m_strValue);
		m_items[nIndex].m_rcLifterL.bottom	= atoi(properties[i++].m_strValue);
		m_items[nIndex].m_rcLifterL.right	= atoi(properties[i++].m_strValue);
		m_items[nIndex].m_rcLifterL.top		= atoi(properties[i++].m_strValue);
		m_items[nIndex].m_clrLifter			= CConvert::ToColor(properties[i++].m_strValue);
	}
	else
	{
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_nLifterID);
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_rcLifterL.left);
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_rcLifterL.bottom);
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_rcLifterL.right);
		properties[i++].m_strValue.Format(_T("%d"), m_items[nIndex].m_rcLifterL.top);
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_items[nIndex].m_clrLifter));
	}

	return i;
}

void CDciLifterCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	for (int i=0; i<m_items.GetSize(); ++i)
	{
		CDciLifterItem& item = m_items[i];
		m_pDCI->DrawButton(pDC, item.m_rcLifterL, item.m_clrLifter, m_bClick);
		m_pDCI->DrawText(pDC, item.m_rcLifterL, m_strText, m_clrFgColor);
	}
}

BOOL CDciLifterCtrl::SetClick(CWnd* pWnd, const CPoint& ptClickS)
{
	for (int i=0; i<m_items.GetSize(); ++i)
	{
		CDciLifterItem& item = m_items[i];
		if (m_bClick = m_pDCI->ConvertRectS(item.m_rcLifterL).PtInRect(ptClickS))
		{
			InvalidateControl(pWnd, FALSE);
			break;
		}
	}

	return m_bClick;
}