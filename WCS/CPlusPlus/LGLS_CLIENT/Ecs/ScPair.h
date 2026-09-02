// ScPair.h: interface for the CScPair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCPAIR_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
#define AFX_SCPAIR_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sc.h"
//#include "ScSocket.h"
#include "MelsecEthernetSk.h"

class CScPair : public CSc  
{
	DECLARE_DYNAMIC(CScPair)

public:
	CScPair(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex);
	virtual ~CScPair();

public:
	virtual CInfo* GetInfo() { return m_pInfo; }

protected:
	virtual void AutoRunProc();
	void RefreshScWaitCount();   // [LGLS 2026-09-02] 크레인별 출고 대기(20) 건수 표시
	virtual void CommandProc();

public:
	BOOL IsValidLocation(CString& strLocation);
};

#endif // !defined(AFX_SCPAIR_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
