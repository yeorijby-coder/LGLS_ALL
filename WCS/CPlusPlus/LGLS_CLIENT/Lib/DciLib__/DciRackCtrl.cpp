// DciRackCtrl.cpp: implementation of the CDciRackCtrl class.
//

#include "StdAfx.h"
#include "DciRackCtrl.h"


IMPLEMENT_SERIAL(CDciRackCtrl, CDciControl, DCI_SIRIALIZE_SCHEMA)

// CDciRackCtrl
CDciRackCtrl::CDciRackCtrl(void)
{
}


CDciRackCtrl::~CDciRackCtrl(void)
{
}

// CDciRackCtrl 멤버 함수
void CDciRackCtrl::InitControl(CDciMaster* pDCI)
{
	CDciControl::InitControl(pDCI);
	m_clrBgColor = RGB(0,111,111);

	m_nType = enL2R;
	m_nUnitLenL = 1;
	m_nStartPos = 0;
}

int CDciRackCtrl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = CDciControl::UpdatePropNames(properties);

	properties.SetSize(enRackCtrlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("start"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("type"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("len"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciRackCtrl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = CDciControl::UpdatePropValues(properties, bSaveObject);

	if (bSaveObject)
	{
		m_nStartPos	= CConvert::ToInt(properties[i++].m_strValue);
		m_nType		= CConvert::ToInt(properties[i++].m_strValue);
		m_nUnitLenL = CConvert::ToInt(properties[i++].m_strValue);
	}
	else	
	{
		properties[i++].m_strValue.Format(_T("%d"), m_nStartPos);
		properties[i++].m_strValue.Format(_T("%d"), m_nType);
		properties[i++].m_strValue.Format(_T("%d"), m_nUnitLenL);
	}

	return i;
}

void CDciRackCtrl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	try
	{
		int nOldMode = pDC->SetBkMode(m_nFgMode);
		int nOldFgColor = pDC->SetTextColor(m_clrFgColor);
		int nBay2 = 1;
		CString str;
		CRect rcUnitL;
		int nLen = m_nUnitLenL;
//		int nRow = abs(m_rcControlL.Height()) / m_nUnitLenL;
//		int nCol = abs(m_rcControlL.Width()) / m_nUnitLenL;
		int nRow = abs(m_rcControlL.Height()) / m_nUnitLenL;
		int nCol = abs(m_rcControlL.Width()) / m_nUnitLenL;

//=========================================================================================================================
//		// 랙 컨트롤을 수정해야 하리라.
//=========================================================================================================================
/*
		// 원본
		for (int i=0; i<nRow; ++i)
		{
			for (int j=0; j<nCol; ++j)
			{
				rcUnitL.left	= m_rcControlL.left+j*m_nUnitLenL;
				rcUnitL.top		= m_rcControlL.bottom+(i+1)*m_nUnitLenL;
				rcUnitL.right	= m_rcControlL.left+(j+1)*m_nUnitLenL;
				rcUnitL.bottom	= m_rcControlL.bottom+i*m_nUnitLenL;
				m_pDCI->DrawButton(pDC, rcUnitL, m_clrBgColor, m_bClick);

				if (m_bClick)
				{
					str.Format("%d", i*nCol+j+1);
					m_pDCI->DrawText(pDC, rcUnitL, str, m_clrFgColor);
				}
			}
		}
*/

		// 수정본
		if (m_nType == enL2R || m_nType == enR2L)
		{
			nRow = 1;
			for (int i=0; i<nRow; ++i)
			{
				for (int j=0; j<nCol; ++j)
				{
					rcUnitL.left	= m_rcControlL.left+j*m_nUnitLenL;
					rcUnitL.top		= m_rcControlL.bottom+(i+1)*m_nUnitLenL;
					rcUnitL.right	= m_rcControlL.left+(j+1)*m_nUnitLenL;
					rcUnitL.bottom	= m_rcControlL.bottom+i*m_nUnitLenL;
					m_pDCI->DrawButton(pDC, rcUnitL, m_clrBgColor, m_bClick);

					if (m_bClick)
					{
						int nBay;
						if(m_nType == enL2R)	
						{
							nBay = i*nCol+j+1;
							if(m_nStartPos != 0)
								nBay += m_nStartPos;

							str.Format(_T("%d"), nBay);
						}
						else //if (m_nType == enR2L)
						{
							nBay = i*nCol+nCol-j;
							if(m_nStartPos != 0)
								nBay += m_nStartPos;
							str.Format(_T("%d"), nBay);
						}

						m_pDCI->DrawText(pDC, rcUnitL, str, m_clrFgColor);
					}
				}
			}
		}

		else if(m_nType == enT2B || m_nType == enB2T)
		{
			nCol = 1;
			for (int i=0; i<nRow; ++i)
			{
				for (int j=0; j<nCol; ++j)
				{
					rcUnitL.left	= m_rcControlL.left+j*m_nUnitLenL;
					rcUnitL.top		= m_rcControlL.bottom+(i+1)*m_nUnitLenL;
					rcUnitL.right	= m_rcControlL.left+(j+1)*m_nUnitLenL;
					rcUnitL.bottom	= m_rcControlL.bottom+i*m_nUnitLenL;

					m_pDCI->DrawButton(pDC, rcUnitL, m_clrBgColor, m_bClick);

					if (m_bClick)
					{
						int nBay;
						if(m_nType == enT2B)	
						{
							nBay = j*nRow+nRow-i;
							if(m_nStartPos != 0)
								nBay += m_nStartPos;

							str.Format(_T("%d"), nBay);

//							str.Format("%d", i*nRow+j+1);
						}
						else //if (m_nType == enR2L)
						{
							nBay = j*nRow+i+1;
							if(m_nStartPos != 0)
								nBay += m_nStartPos;
							str.Format(_T("%d"), nBay);
//							str.Format("%d", i*nRow+nRow-j);
						}

						m_pDCI->DrawText(pDC, rcUnitL, str, m_clrFgColor);
					}
				}
			}
		}
		//==> 큰 랙일 때 2씩 증가 - 최원빈 추가
		else //if (m_nType == enTypeSize)
		{
			for (int i=0; i<nRow; ++i)
			{
				for (int j=0; j<nCol; ++j)
				{
					rcUnitL.left	= m_rcControlL.left+j*m_nUnitLenL;
					rcUnitL.top		= m_rcControlL.bottom+(i+1)*m_nUnitLenL;
					rcUnitL.right	= m_rcControlL.left+(j+1)*m_nUnitLenL;
					rcUnitL.bottom	= m_rcControlL.bottom+i*m_nUnitLenL;
					m_pDCI->DrawButton(pDC, rcUnitL, m_clrBgColor, m_bClick);

					if (m_bClick)
					{
						str.Format(_T("%d"), nBay2);
						m_pDCI->DrawText(pDC, rcUnitL, str, m_clrFgColor);
						nBay2 += 2 ;
						
						/*
						if(m_nType == enTypeSize)	
						{
							nBay = i*nCol+j+1;
							if(m_nStartPos != 0)
								nBay += m_nStartPos;

							str.Format("%d", nBay);
						}
						else //if (m_nType == enR2L)
						{
							nBay = i*nCol+nCol-j;
							if(m_nStartPos != 0)
								nBay += m_nStartPos;
							str.Format("%d", nBay);
						}

						m_pDCI->DrawText(pDC, rcUnitL, str, m_clrFgColor);
						//*/
					}
				}
			}
		}
		//*/
//-------------------------------------------------------------------------------------------------------------------------

		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldMode);
	}
	catch (CException* e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciRackCtrl::UpdateControl... CException! [%s] \n"), szMessage);	
	}
}
