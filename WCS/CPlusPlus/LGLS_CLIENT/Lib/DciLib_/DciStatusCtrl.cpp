// DciStatusCtrl.cpp: implementation of the CDciStatusCtrl class.
//

#include "StdAfx.h"
#include "DciStatusCtrl.h"


IMPLEMENT_SERIAL(CDciStatusCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

// CDciStatusCtrl
CDciStatusCtrl::CDciStatusCtrl(void)
{
}


CDciStatusCtrl::~CDciStatusCtrl(void)
{
}

// CDciStatusCtrl ¸â¹ö ÇÔ¼ö
void CDciStatusCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);

	m_nShape = enShpaeBasic;
	m_nStatus = 0;
}

int CDciStatusCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enStatusCtrlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("shape"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("ratio"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciStatusCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
		m_nShape = CConvert::ToInt(properties[i++].m_strValue);
		m_nRatio = CConvert::ToInt(properties[i++].m_strValue);
	}
	else	
	{
		properties[i++].m_strValue.Format(_T("%d"), m_nShape);
		properties[i++].m_strValue.Format(_T("%d"), m_nRatio);
	}

	return i;
}

void CDciStatusCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	TRY
	{
		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);

		CRect rcControlS, rcTextS, rcStatusS;
		rcControlS = m_pDCI->ConvertRectS(m_rcControlL);

		int w = abs(rcControlS.Width()), h = abs(rcControlS.Height());
		int n = 0, nn = 0;

		switch (m_nShape)
		{
		case enShpaeBasic:
			break;

		case enShapeHorizontal:
			break;

		case enShapeVertical:
			n = 1;//int (h / w);

			if (m_nRatio > -1 && m_nRatio < 100)
				nn = 100 - m_nRatio;

			rcControlS.left		+= w * n / 100;
			rcControlS.right	-= w * n / 100;
			rcControlS.top		+= h * nn / 100;
			//rcControlS.bottom	-= h * n / 100;
			rcControlS.InflateRect(0, 0, 1, 1);

			pDC->Rectangle(rcControlS);
			break;

		default:
			return;
		}

		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldBkMode);
	}
	CATCH (CException, e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciStatusCtrl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
	END_CATCH
}
