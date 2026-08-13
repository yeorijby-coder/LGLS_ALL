// Debuger.h: interface for the CDebuger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGER_H__2D4A7A3E_24A0_4532_85AD_CFF797997983__INCLUDED_)
#define AFX_DEBUGER_H__2D4A7A3E_24A0_4532_85AD_CFF797997983__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDebuger : public CObject  
{
public:
	CDebuger();
	virtual ~CDebuger();

protected:
	static CCriticalSection m_csSyncWriteLog;
	
public:
	static BOOL m_bAssert;
	static BOOL m_bTrace;

public:
	static void WriteLog(LPCTSTR lpszLog, LPCTSTR lpszFileName);
	static void Assert(BOOL bValid, LPCTSTR lpszLog, int nLine, LPCSTR lpszFile);
	static void Assert(int nIndex, int nSize, LPCTSTR lpszLog, int nLine, LPCSTR lpszFile);
	static void Trace(LPCTSTR lpszLog, LPCTSTR lpszPos, int nLine, LPCSTR lpszFile);
};

#define DEBUGER_ASSERT_VALID(valid)					CDebuger::Assert(valid, NULL, __LINE__, __FILE__)
#define DEBUGER_ASSERT_VALID_LOG(valid,log)			CDebuger::Assert(valid, log, __LINE__, __FILE__)
#define DEBUGER_ASSERT_RANGE(index,size)			CDebuger::Assert(index, size, NULL, __LINE__, __FILE__)
#define DEBUGER_ASSERT_RANGE_LOG(index,size,log)	CDebuger::Assert(index, size, log, __LINE__, __FILE__)
#define DEBUGER_TRACE(log,pos)						CDebuger::Trace(log, pos, __LINE__, __FILE__)

#endif // !defined(AFX_DEBUGER_H__2D4A7A3E_24A0_4532_85AD_CFF797997983__INCLUDED_)
