// KeyWord.h: interface for the CKeyWord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORD_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_)
#define AFX_KEYWORD_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XmlDom.h"
#include "TrackInfo.h"
#include "Dictionary.h"

class CEcsDoc;
class CEquipment;
class CKeyWord : public CXmlDom  
{
public:
	CKeyWord(CEcsDoc* pDoc);
	virtual ~CKeyWord();

protected:
	CEcsDoc* m_pDoc;

public:
	EN_COMM_METHOD m_enCommMethod;
	EN_CPU_KIND m_enCpuKind;
	EN_ADDRESS_AREA m_enAddressArea;
	BOOL m_bUseDestTable;
	BOOL m_bUseScDest;
	enum { enDefaultPropSize = 4 };	// type, addr, name, InOrder  <= ÃßÈÄ 1°³°¡ Append µÊ

public:
	BOOL ParseXml();
};

#endif // !defined(AFX_KEYWORD_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_)
