// ScSocket.h: interface for the CScSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCSOCKET_H__B977E6F8_362A_4F5F_95D8_FF9AE5290C6D__INCLUDED_)
#define AFX_SCSOCKET_H__B977E6F8_362A_4F5F_95D8_FF9AE5290C6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InterfaceSk.h"

class CEquipment;
class CScSocket : public CInterfaceSk 
{
	DECLARE_DYNAMIC(CScSocket)

public:
	CScSocket(CEquipment* pEquipment, int nIndex);
	virtual ~CScSocket();

public:
	enum { enSTX = 0x02, enETX = 0x03 };

public:
	virtual void OnReceive(int nErrorCode);
	virtual BOOL SendMsg(int nType, const CString& strMsg);
};

#endif // !defined(AFX_SCSOCKET_H__B977E6F8_362A_4F5F_95D8_FF9AE5290C6D__INCLUDED_)
