// LogPlayback.cpp : implementation of the CLogPlayback class
//

#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "LogPlayback.h"


CLogPlayback::CLogPlayback(CEcsDoc* pDoc) : CLog(pDoc)
{

	COleDateTime theTime = COleDateTime::GetCurrentTime();
//	m_strFileName.Format(_T("%s%d%.2d%.2d.log", m_strFileTitle, theTime.GetYear(), theTime.GetMonth(), theTime.GetDay());
	m_strFileName.Format(_T("%s%.2d%.2d.log"), m_strFileTitle, theTime.GetMonth(), theTime.GetDay());

	m_strLogDir = m_pDoc->m_pConfig->m_strPlaybackPath;
	m_strFileTitle = m_strLogDir + "\\PB";

	m_pFileOut = NULL;
}

CLogPlayback::~CLogPlayback()
{

}

void CLogPlayback::Log(CString strData)
{
	if(strData.IsEmpty())
		return;

	CString strWrite = strData;
	
	strWrite.TrimLeft(STX);
	strWrite.TrimRight(ETX);

	
	SYSTEMTIME  sSystemTime  = {0};
	GetLocalTime(&sSystemTime);

	strData = GetFormatTime(sSystemTime) + strWrite;
	strWrite.Format(_T("%s\r\n"), strData);


	Write(strWrite);
}

////////////////////////////////////////////
//
CString CLogPlayback::GetFormatTime(SYSTEMTIME time)
{
	CString strTime;
	strTime.Format(_T("[%04d-%02d-%02d %02d:%02d:%02d.%03d]"), 
					time.wYear, time.wMonth, time.wDay, 
					time.wHour, time.wMinute, time.wSecond, 
					time.wMilliseconds);
	return strTime;
}

CString CLogPlayback::GetXmlString(CString strData)
{
	CString strResult;
	strResult.Format(_T("%c<ECS>%s</ECS>%c"), STX, strData, ETX);

	return strResult;
}

////////////////////////////////////////////
//
void CLogPlayback::Write(CString strData)
{
	m_syncLogThread.Lock();

	WriteFileOpen();

	if ( m_pFileOut )
		m_pFileOut->Write(strData.GetBuffer(0), strData.GetLength());

	m_syncLogThread.Unlock();
}

void CLogPlayback::WriteFileOpen()
{
	SYSTEMTIME  sSystemTime  = {0};
	GetLocalTime(&sSystemTime);

	CString strNewFile;
	int nIntervalHour = 2;
	int nHour = (sSystemTime.wHour/nIntervalHour) * nIntervalHour;
//	strNewFile.Format(_T("%s%d%.2d%.2d%.2d.log", m_strFileTitle, sSystemTime.wYear, sSystemTime.wMonth, sSystemTime.wDay, nHour);
	strNewFile.Format(_T("%s%.2d%.2d%.2d.log"), m_strFileTitle, sSystemTime.wMonth, sSystemTime.wDay, nHour);
	if ( strNewFile != m_strFileName )
	{
		m_strFileName = strNewFile;
		
		if ( m_pFileOut )
		{
			delete m_pFileOut;
			m_pFileOut = NULL;
		}

		ExpiredFileRemove(90);
	}

	TRY
	{
		if ( m_pFileOut == NULL )
		{
			m_pFileOut = new CFile(m_strFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite);
			m_pFileOut->SeekToEnd();

			Log( GetXmlString(_T("<STARTINIT/>")) );
			Log( GetXmlString(_T("<ENDINIT/>")) );
		}
	}
	CATCH( CFileException, e )
	{
		CString strMsg;
		strMsg.Format(_T("로그파일 열기 실패[%s]"), CLib::GetSystemErrMsg(e->m_cause));

		if ( m_pFileOut )
		{
			delete m_pFileOut;
			m_pFileOut = NULL;
		}
	}
	END_CATCH

}

void CLogPlayback::ExpiredFileRemove(LONG nBackDays)
{
	CString strFindFile;
	strFindFile.Format(_T("%s\\*.*"), m_strLogDir);

	WIN32_FIND_DATA FileData;
	HANDLE hFindFile = ::FindFirstFile(strFindFile, &FileData);

	if ( hFindFile == INVALID_HANDLE_VALUE )
	{
		CString strMsg = CLib::GetSystemErrMsg();
		return;
	}

	COleDateTimeSpan timeSpan(nBackDays, 0, 0, 0);
	COleDateTime timeBack = COleDateTime::GetCurrentTime()-timeSpan;

	for ( ; ::FindNextFile(hFindFile, &FileData); )
	{
		if ( FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			continue;

		COleDateTime timeFile = FileData.ftLastWriteTime;
		if ( timeBack > timeFile )
		{
			CString strTemp;
			strTemp.Format(_T("%s\\%s"), m_strLogDir, FileData.cFileName);
			TRY
			{
				CFile::Remove(strTemp);
			}
			CATCH( CFileException, e )
			{
				CString strMsg;
				strMsg.Format(_T("로그파일[%s] 삭제 실패[%s]"), strTemp, CLib::GetSystemErrMsg(e->m_cause));
				//m_pDoc->StatusDisplay(strMsg);
			}
			END_CATCH
		}
	}

	FindClose(hFindFile);
}

