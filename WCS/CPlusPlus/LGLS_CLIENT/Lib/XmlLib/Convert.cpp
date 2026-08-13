// Convert.cpp: implementation of the CConvert class.
//

#include "StdAfx.h"
#include "Convert.h"


// CConvert
CConvert::CConvert(void)
{
}

CConvert::~CConvert(void)
{
}

// CConvert ¸â¹ö ÇÔ¼ö
CString CConvert::ToString(int nValue)
{
	CString strValue;
	strValue.Format(_T("%d"), nValue);
	return strValue;
}

CString CConvert::ToString(COLORREF color)
{
	CString strColor;
	strColor.Format(_T("%.6x"), color);
	return strColor;
}

int CConvert::ToInt(CString& strValue)
{
	return _ttoi(strValue);
}

COLORREF CConvert::ToColor(CString& strValue)
{
	COLORREF color = 0;
	_stscanf_s(strValue, _T("%x"), &color);
	return color;
}
