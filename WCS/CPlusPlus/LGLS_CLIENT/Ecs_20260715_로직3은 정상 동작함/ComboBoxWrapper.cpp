#include "StdAfx.h"
#include "ComboBoxWrapper.h"


CComboBoxWrapper::CComboBoxWrapper(void)
{
}


CComboBoxWrapper::~CComboBoxWrapper(void)
{
}

void CComboBoxWrapper::SetItemDataEx(int nIndex, CString strKey)
{
	SetItemData(nIndex, CConvert::ToInt(strKey));
	m_Key.SetAt(nIndex, strKey);
}

CString CComboBoxWrapper::GetItemKey(int pIndex)
{
	return m_Key[pIndex];
}

CString CComboBoxWrapper::GetItemCCD(CString pstrText)
{
	CString strKeyTemp;
	int nIndex = 0;
	for(POSITION pPos = m_Key.GetStartPosition(); pPos != NULL; )
	{
		m_Key.GetNextAssoc(pPos, nIndex, strKeyTemp);
		GetLBText(nIndex, strKeyTemp);
		if(strKeyTemp == pstrText)
		{
			return m_Key[nIndex];
		}
		nIndex++;
	}
	
	return _T("9999");
}




void CComboBoxWrapper::SetCurSelEx(CString pstrKey)
{
	CString strKeyTemp;
	int nIndex = 0;
	for(POSITION pPos = m_Key.GetStartPosition(); pPos != NULL; )
	{
		m_Key.GetNextAssoc(pPos, nIndex, strKeyTemp);
		if(strKeyTemp == pstrKey)
		{
			SetCurSel(nIndex);
			return;
		}
	}
	return;
}


void CComboBoxWrapper::SetCurSelTextEx(CString pstrText)
{
	CString strTextTemp;
	int nIndex = 0;
	for(POSITION pPos = m_Key.GetStartPosition(); pPos != NULL; )
	{
		m_Key.GetNextAssoc(pPos, nIndex, strTextTemp);
		GetLBText(nIndex, strTextTemp);
		if(strTextTemp == pstrText)
		{
			SetCurSel(nIndex);
			UpdateData(TRUE);
			return;
		}
	}
	return;
}