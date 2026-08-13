// Convert.h: interface for the CConvert class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

class AFX_EXT_CLASS CConvert
{
public:
	CConvert(void);
	virtual ~CConvert(void);

public:
	static CString ToString(int nValue);
	static CString ToString(COLORREF color);

public:
	static int ToInt(CString& strValue);
	static COLORREF	ToColor(CString& strValue);
};

//#undef AFX_DATA
//#define AFX_DATA
