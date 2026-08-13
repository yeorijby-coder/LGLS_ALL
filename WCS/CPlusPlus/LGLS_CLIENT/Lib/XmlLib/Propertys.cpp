// Propertys.cpp: implementation of the CPropertys class.
//

#include "stdafx.h"
#include "Propertys.h"


// CPropertys
CPropertys::CPropertys()
{
}

CPropertys::~CPropertys()
{
}

// CPropertys ¸â¹ö ÇÔ¼ö
void CPropertys::SetProperty(int nType, LPCTSTR lpszName /* = NULL */, LPCTSTR lpszValue /* = NULL */)
{
	m_nType = nType;
	m_strName.Format(_T("%s"), lpszName);
	m_strValue.Format(_T("%s"), lpszValue);
}
