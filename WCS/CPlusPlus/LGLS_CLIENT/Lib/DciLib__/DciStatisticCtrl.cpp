// DciStatisticCtrl.cpp: implementation of the CDciStatusCtrl class.
//

#include "StdAfx.h"
#include "DciStatisticCtrl.h"


IMPLEMENT_SERIAL(CDciStatisticCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

// CDciStatisticCtrl
CDciStatisticCtrl::CDciStatisticCtrl(void)
{
}


CDciStatisticCtrl::~CDciStatisticCtrl(void)
{
	for (int i = 0 ; i < m_pStatuCtrls.GetCount() ; i++)
		delete m_pStatuCtrls[i];

	m_pStatuCtrls.RemoveAll();
}

// CDciStatisticCtrl 멤버 함수
void CDciStatisticCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);

	m_nShape = enShpaeBasic;
	m_nStatus = 0;
//	m_rcGap.SetRectEmpty();
//	m_pStatusCtrl = NULL;
}

int CDciStatisticCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enStatisticCtrlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("shape"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("count"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciStatisticCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
		m_nShape = CConvert::ToInt(properties[i++].m_strValue);
		m_nCount = CConvert::ToInt(properties[i++].m_strValue);
	}
	else	
	{
		properties[i++].m_strValue.Format(_T("%d"), m_nShape);
		properties[i++].m_strValue.Format(_T("%d"), m_nCount);
	}

	return i;
}

void CDciStatisticCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	if (m_nCount <= 0)
	{
		return;
	}
	else
	{
		for (int i = 0 ; i < m_nCount ; i++)
		{
			int nRatio = 85;			// 기본값은 100임!
			CDciStatusCtrl* pStatusCtrl = new CDciStatusCtrl();
			pStatusCtrl->m_nShape = m_nShape;
			pStatusCtrl->m_nRatio = nRatio;
			m_pStatuCtrls.Add(pStatusCtrl);	
			m_nRatios.Add(nRatio);
		}
	}

	TRY
	{
		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);

		CRect rcControlS, rcTextS, rcStatusS;
		rcControlS = m_pDCI->ConvertRectS(m_rcControlL);

		int w = abs(rcControlS.Width()), h = abs(rcControlS.Height());
		int n = 0;

		switch (m_nShape)
		{
		case enShpaeBasic:
			break;

		case enShapeHorizontal:
			break;

		case enShapeVertical:
			//n = int (h / w) ;
			//rcControlS.left		+= w * n / 100;
			//rcControlS.right	-= w * n / 100;
			//rcControlS.top		+= h * n / 100;
			////rcControlS.bottom	-= h * n / 100;
			//rcControlS.InflateRect(0, 0, 1, 1);

			for (int i = 0 ; i < m_pStatuCtrls.GetCount() ; i++)
			{
				m_pStatuCtrls[i]->m_nRatio = m_nRatios[i];
				m_pStatuCtrls[i]->SetDciMaster(m_pDCI);
				m_pStatuCtrls[i]->UpdateControl(pDC);
			}
			
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
		TRACE(_T("\n CDciStatisticCtrl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
	END_CATCH
}
