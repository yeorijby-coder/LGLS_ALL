// Debuger.cpp: implementation of the CDebuger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "Debuger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCriticalSection CDebuger::m_csSyncWriteLog;
BOOL CDebuger::m_bAssert = ::GetPrivateProfileInt(_T("COMMON"), _T("DebugerAssert"), FALSE, ECS_INI_FILE);
BOOL CDebuger::m_bTrace = ::GetPrivateProfileInt(_T("COMMON"), _T("DebugerTrace"), FALSE, ECS_INI_FILE);

CDebuger::CDebuger()
{
}

CDebuger::~CDebuger()
{

}

void CDebuger::WriteLog(LPCTSTR lpszLog, LPCTSTR lpszFileName)
{
	m_csSyncWriteLog.Lock();

	TRY
	{
		CFile f(lpszFileName, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);
		f.SeekToEnd();

		CArchive ar(&f, CArchive::store);
		ar.WriteString(lpszLog);
		ar.Close();

		f.Close();
	}
	CATCH (CException, e)
	{
		CString strLog;
		TCHAR szMessage[512] = {0};
		e->GetErrorMessage(szMessage, sizeof(szMessage));
		strLog.Format(_T("%s [%s] [%s]"), e->GetRuntimeClass()->m_lpszClassName, szMessage, lpszLog);
		TRACE(_T("\n [CDebuger::WriteLog] %s \n"), strLog);
	}
	END_CATCH

	m_csSyncWriteLog.Unlock();
}

void CDebuger::Assert(BOOL bValid, LPCTSTR lpszLog, int nLine, LPCSTR lpszFile)
{
#ifdef _DEBUG
	if (!bValid) 
		::AfxAssertFailedLine(lpszFile, nLine);
#endif

	if (bValid)
		return;

	if (!m_bAssert)
		return;

	CString strLog;
	if (lpszLog == NULL) strLog.Format(_T("[%s][%s][%d] ASSERT(FALSE)! \r\n"), CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), lpszFile, nLine);
	else strLog.Format(_T("[%s][%s][%d] ASSERT(FALSE)! %s \r\n"), CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), lpszFile, nLine, lpszLog);
	WriteLog(strLog, _T("ASSERT.LOG"));	
}

void CDebuger::Assert(int nIndex, int nSize, LPCTSTR lpszLog, int nLine, LPCSTR lpszFile)
{
#ifdef _DEBUG
	if ((nIndex < 0) || (nIndex >= nSize)) 
		::AfxAssertFailedLine(lpszFile, nLine);
#endif

	if ((nIndex >= 0) && (nIndex < nSize))
		return;

	if (!m_bAssert)
		return;

	CString strLog;
	if (lpszLog == NULL) strLog.Format(_T("[%s][%s][%d] ASSERT(Invalid Array Index)! (INDEX=%d, SIZE=%d) \r\n"), CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), lpszFile, nLine, nIndex, nSize);
	else strLog.Format(_T("[%s][%s][%d] ASSERT(Invalid Array Index)! (INDEX=%d, SIZE=%d) %s \r\n"), CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), lpszFile, nLine, nIndex, nSize, lpszLog);
	WriteLog(strLog, _T("ASSERT.LOG"));	
}

void CDebuger::Trace(LPCTSTR lpszLog, LPCTSTR lpszPos, int nLine, LPCSTR lpszFile)
{
	CString strLog;
//	strLog.Format(_T("[%s][%s][%d][%s] %s \r\n"), CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), lpszFile, nLine, lpszPos, lpszLog);
	strLog.Format(_T("[%s][%d][%s] %s \r\n"), CTime::GetCurrentTime().Format(_T("%H:%M:%S")), ::GetCurrentThreadId(), lpszPos, lpszLog);
	TRACE(_T("\n %s \n"), strLog.GetLength() >= 500 ? strLog.Left(500) : strLog);

	if (!m_bTrace)
		return;

	CString strFileName;
	strFileName.Format(_T("%sTRACE.LOG"), CTime::GetCurrentTime().Format(_T("%Y%m%d")));
	WriteLog(strLog, strFileName);
}