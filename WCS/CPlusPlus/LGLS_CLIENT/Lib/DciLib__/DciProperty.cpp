// DciProperty.cpp: implementation of the CDciProperty class.
//

#include "StdAfx.h"
#include "DciProperty.h"


// CDciProperty
CDciProperty::CDciProperty(void)
{
}

CDciProperty::~CDciProperty(void)
{
}

// CDciProperty ¸â¹ö ÇÔ¼ö
void CDciProperty::SetProperty(int nType, LPCTSTR lpszName /* = NULL */, LPCTSTR lpszValue /* = NULL */)
{
	m_nType = nType;
	m_strName.Format(_T("%s"), lpszName);
	m_strValue.Format(_T("%s"), lpszValue);
}
