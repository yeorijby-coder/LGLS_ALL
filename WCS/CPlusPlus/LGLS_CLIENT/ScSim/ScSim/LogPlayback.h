
#pragma once

#include "Log.h"

class CLogPlayback : public CLog
{
public:
	CLogPlayback(CEcsDoc* pDoc);
	virtual ~CLogPlayback();

public:
	CFile*			m_pFileOut;
	CString			m_strLogDir;
	CString			m_strFileTitle;
	CString			m_strFileName;

	void	Log(CString strData);

	CString	GetFormatTime(SYSTEMTIME time);

public:
	void	Write(CString strData);
	void	WriteFileOpen();

	static CString	GetXmlString(CString strData);
	void ExpiredFileRemove(LONG nBackDays);
};