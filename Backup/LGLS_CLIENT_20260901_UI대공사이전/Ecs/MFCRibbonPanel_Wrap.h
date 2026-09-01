#pragma once
#include "afxribbonpanel.h"
class CMFCRibbonPanel_Wrap :
	public CMFCRibbonPanel
{
public:
	CMFCRibbonPanel_Wrap(void);
	~CMFCRibbonPanel_Wrap(void);
	CMFCRibbonPanel_Wrap(LPCTSTR lpszName = NULL, HICON hIcon = NULL ) : CMFCRibbonPanel(lpszName, hIcon) {};

public:
	void SetName(CString& name ) { m_strName = name; };
};

