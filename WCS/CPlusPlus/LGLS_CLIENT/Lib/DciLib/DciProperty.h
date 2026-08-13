// DciProperty.h: interface for the CDciProperty class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include <afxtempl.h>


class AFX_EXT_CLASS CDciProperty 
{
public:
	CDciProperty(void);
	virtual ~CDciProperty(void);

public:
	enum EN_PROPERTY_TYPE { 
		PT_VOID		= 0x0000, 
		PT_DEC		= 0x0001,	// DECIMAL INTEGER
		PT_HEX		= 0x0002,	// HEXADECIAML INTEGER
		PT_OCT		= 0x0003,	// OCTAL INTEGER
		PT_BIN		= 0x0004,	// BINARY INTEGER
		PT_UDEC		= 0x0005,	// UNSIGNED DECIMAL INTEGER
		PT_BOOL		= 0x0006,	// BOOLEAN
		PT_FLOAT	= 0x0007,	// FLOAT
		PT_DOUBLE	= 0x0008,	// DOUBLE
		PT_CHAR		= 0x0009,	// CHARACTER
		PT_STRING	= 0x000A,	// STRING
		PT_COLOR	= 0x000B,	// COLORREF
		PT_PERCENT	= 0x000C
	};

public:
	int		m_nType;
	CString m_strName;
	CString m_strValue;

public:
	void SetProperty(int nType, LPCTSTR lpszName = NULL, LPCTSTR lpszValue = NULL);
};

// class AFX_EXT_CLASS CDciPropertyList : public CList<CDciProperty, CDciProperty>
// {
// public:
// 	CDciPropertyList() {}
// 	virtual ~CDciPropertyList() {}
// };

class AFX_EXT_CLASS CDciPropertyArray : public CArray<CDciProperty, CDciProperty>
{
public:
	CDciPropertyArray() {}
	virtual ~CDciPropertyArray() {}
};

//#undef AFX_DATA
//#define AFX_DATA
