#pragma once
#include "afxwin.h"
class CComboBoxWrapper :
	public CComboBox
{
public:
	CComboBoxWrapper(void);
	~CComboBoxWrapper(void);


public:
	CMap<int, int, CString, LPCTSTR> m_Key;

public:
	void SetItemDataEx(int nIndex, CString strKey);
	void SetCurSelEx(CString pstrKey);
	void SetCurSelTextEx(CString pstrText);
	CString GetItemKey(int pIndex);
	CString GetItemCCD(CString pstrKey);
};

