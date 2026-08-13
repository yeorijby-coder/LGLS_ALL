// Log.cpp : implementation of the CLog class
//

#include "stdafx.h"
#include "Ecs.h"
#include "Log.h"

#include "EcsDoc.h"
#include "StartupTip.h"


CLog::CLog(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

// 	m_strLogDir = m_pDoc->m_pConfig->m_strLogPath;
 	m_strLogName = "LOG";

	m_posAlarm = NULL;
}

CLog::~CLog()
{
	for (POSITION pos=m_listAlarm.GetHeadPosition(); pos!=NULL; )
		delete m_listAlarm.GetNext(pos);
	m_listAlarm.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLog::Job(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID/* =NULL */, BYTE bAlarm/* =0 */)
{
	if (!m_pDoc->m_pConfig->m_bJob) return;

	Write(LOG_TYPE_JOB, nLogPos, nLuggNum, lpszContent, lpszProdID, bAlarm);
}

void CLog::Event(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID/* =NULL */, BYTE bAlarm/* =0 */)
{
	if (!m_pDoc->m_pConfig->m_bEvent) return;

	Write(LOG_TYPE_EVENT, nLogPos, nLuggNum, lpszContent, lpszProdID, bAlarm);
}

void CLog::Error(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID/* =NULL */, BYTE bAlarm/* =0 */)
{
	if (!m_pDoc->m_pConfig->m_bError) return;

	Write(LOG_TYPE_ERROR, nLogPos, nLuggNum, lpszContent, lpszProdID, bAlarm);
}

void CLog::Alarm(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID/* =NULL */, BYTE bAlarm/* =0 */)
{
	if (!m_pDoc->m_pConfig->m_bAlarm) return;

	Write(LOG_TYPE_ALARM, nLogPos, nLuggNum, lpszContent, lpszProdID, bAlarm);
}

void CLog::Debug(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID/* =NULL */, BYTE bAlarm/* =0 */)
{
	if (!m_pDoc->m_pConfig->m_bDebug) return;

	Write(LOG_TYPE_DEBUG, nLogPos, nLuggNum, lpszContent, lpszProdID, bAlarm);
}

void CLog::ShowAlarm(int nLogPos, int nLuggNum, CString strContent, CString strProdID)
{
	CLogEvent* pLogEvent = new CLogEvent(nLogPos, LOG_TYPE_DEBUG, nLuggNum, strContent, strProdID);
	PumpupAlarm(pLogEvent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLog::Write(int nType, int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID/* =NULL */, BYTE bAlarm/* =0 */, int nErrCode/* =0 */)
{
	m_syncLogThread.Lock();

	CTime theTime = CTime::GetCurrentTime();
	CTimeSpan theTimeSpan(1, 0, 0, 0);

	CString strExpiredLogFile, strTemp;
	strTemp.Format(_T("%.2d"), theTime.GetDay());

	CString strLogFile;
	strLogFile.Format(_T("%s\\%s%s.elg"), m_pDoc->m_pConfig->m_strLogPath, m_strLogName, strTemp);

	for (int i=0; i<2; ++i)
	{
		theTime += theTimeSpan;
		strTemp.Format(_T("%.2d"), theTime.GetDay());
		strExpiredLogFile.Format(_T("%s\\%s%s.elg"), m_pDoc->m_pConfig->m_strLogPath, m_strLogName, strTemp);

		TRY
		{
			if (::GetFileAttributes(strExpiredLogFile) != 0xFFFFFFFF)
				CFile::Remove(strExpiredLogFile);
		}
		CATCH (CFileException, e)
		{
			strTemp.Format(_T("만기된 로그파일 삭제 실패! [%s] [CODE=%d]"), strExpiredLogFile, e->m_cause);
		}
		END_CATCH
	}

	TRY
	{
		CFile f(strLogFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		f.SeekToEnd();

		CArchive ar(&f, CArchive::store);
		CLogEvent* pLogEvent = new CLogEvent(nLogPos, nType, nLuggNum, lpszContent, lpszProdID, nErrCode);
//		pLogEvent->Serialize(ar);
		ar.WriteString(pLogEvent->GetString());

		if (bAlarm)
			PumpupAlarm(pLogEvent);
		else
			delete pLogEvent;

		ar.Close();
		f.Close();
	}
	CATCH (CException, e)
	{
		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, sizeof(szMessage));
		TRACE(_T("\n %s! %s \n"), e->GetRuntimeClass()->m_lpszClassName, szMessage);
		m_syncLogThread.Unlock();
		return;
	}
	END_CATCH

	strTemp.Format(_T("\n %s \n"), strTemp);
	TRACE(strTemp);

//	m_pDoc->StatusDisplay(lpszContent);

	m_syncLogThread.Unlock();
}

BOOL CLog::Load(CReportCtrl* pReportCtrl, CSortContext* pContext)
{
	CWaitCursor someWait;

	TRY 
	{
		CFile f(pContext->m_strFileName, CFile::modeRead);
		CArchive ar(&f, CArchive::load);

		pReportCtrl->DeleteAllItems();

		CString strLogEvent;
		while (ar.ReadString(strLogEvent))
		{
			CLogEvent LogEvent;
			LogEvent.SetString(strLogEvent);

			if (!pContext->Sorting(&LogEvent))
				continue;

			CStringList listTemp;
			listTemp.AddTail(LogEvent.GetLogType());
			listTemp.AddTail(LogEvent.GetTime());
			listTemp.AddTail(LogEvent.GetLuggNum());
			listTemp.AddTail(LogEvent.GetPosition());
			listTemp.AddTail(LogEvent.m_strProdID);
			listTemp.AddTail(LogEvent.m_strContent);

			pReportCtrl->AddItem(LogEvent.m_nType, LogEvent.GetImageIndex(), &listTemp);
		}

		pReportCtrl->ResizeColumn();

		ar.Close();
		f.Close();
	}
	CATCH (CException, e)
	{
		AfxMessageBox(CLib::GetExceptionString(e));
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

// BOOL CLog::Load(CReportCtrl* pReportCtrl, CSortContext* pContext)
// {
// 	CWaitCursor SomeWait;
// 	CFile* pFileIn = NULL;
// 
// 	TRY
// 	{
// 		pFileIn = new CFile(pContext->m_strFileName, CFile::modeRead);
// 	}
// 	CATCH (CException, e)
// 	{
// 		TCHAR szMessage[_MAX_PATH];
// 		e->GetErrorMessage(szMessage, sizeof(szMessage));
// 		if (pFileIn) delete pFileIn;
// 		AfxMessageBox(szMessage);
// 		return FALSE;
// 	}
// 	END_CATCH
// 
// 	CArchive* pArchiveIn = new CArchive(pFileIn, CArchive::load);
// 	DWORD dwFileLen = pFileIn->GetLength();
// 	DWORD dwCurrentPos = 0;
// 
// 	pReportCtrl->DeleteAllItems();
// 
// 	do
// 	{
// 		CLogEvent LogEvent;
// 
// 		TRY 
// 		{
// 			LogEvent.Serialize(*pArchiveIn);
// 		}
// 		CATCH (CException, e)
// 		{
// 			CString strTemp;
// 			TCHAR szTemp[_MAX_PATH] = {0};
// 			e->GetErrorMessage(szTemp, sizeof(szTemp));
// 			strTemp.Format("로그파일 시리얼 라이징중 이상 발생! [%s] [MSG=%s]", e->GetRuntimeClass()->m_lpszClassName, szTemp);
// 			if (pArchiveIn) delete pArchiveIn;
// 			if (pFileIn) delete pFileIn;
// 			AfxMessageBox(strTemp);
// 			return FALSE;
// 		}
// 		END_CATCH
// 
// 		dwCurrentPos += LogEvent.GetReadSize();
// 
// 		if (!pContext->Sorting(&LogEvent))
// 			continue;
// 
// 		CStringList listTemp;
// 		listTemp.AddTail(LogEvent.GetLogType());
// 		listTemp.AddTail(LogEvent.GetTime());
// 		listTemp.AddTail(LogEvent.GetLuggNum());
// 		listTemp.AddTail(LogEvent.GetPosition());
// 		listTemp.AddTail(LogEvent.m_strProdID);
// 		listTemp.AddTail(LogEvent.m_strContent);
// 
// 		pReportCtrl->AddItem(LogEvent.m_nType, LogEvent.GetImageIndex(), &listTemp);
// 
// 		listTemp.RemoveAll();
// 
// 		if (pReportCtrl->GetItemCount() > LOG_DISP_MAX) break;
// 	}
// 	while ( dwFileLen > dwCurrentPos+10 );
// 
// 	delete pArchiveIn;
// 	delete pFileIn;
// 
// 	pReportCtrl->ResizeColumn();
// 
// 	return TRUE;
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLog::AssertValid() const
{
	CObject::AssertValid();
}

void CLog::PumpupAlarm(CLogEvent* pLogEvent)
{
	TRY
	{
		m_listAlarm.AddTail(pLogEvent);
	}
	CATCH(CMemoryException, e)
	{
		delete pLogEvent;

		char szErrMsg[1024];
		e->GetErrorMessage((LPTSTR)szErrMsg, sizeof(szErrMsg));
		return;
	}
	END_CATCH

	MultiCastPerLog(pLogEvent);

	DeleteMaxOverAlarm();

	::PostMessage(m_pDoc->m_hWndView, WM_USER_ALARM_NOTIFY, 0, 0);
}

void CLog::DeleteMaxOverAlarm()
{
	m_syncAlarmLoading.Lock();

	while (m_listAlarm.GetCount() > 10)
		delete (CLogEvent*)m_listAlarm.RemoveHead();

	m_syncAlarmLoading.Unlock();
}

void CLog::DeleteAllAlarm()
{
	m_syncAlarmLoading.Lock();

	m_posAlarm = NULL;

	for (POSITION pos=m_listAlarm.GetHeadPosition(); pos!=NULL; )
		delete (CLogEvent*)m_listAlarm.GetNext(pos);

	m_listAlarm.RemoveAll();
	
	m_syncAlarmLoading.Unlock();
}

void CLog::LoadAlarmFirst(CStartupTip* pTip) 
{
	m_syncAlarmLoading.Lock();

	m_posAlarm = m_listAlarm.GetHeadPosition();
	if (m_posAlarm != NULL)
	{
		CLogEvent* pLogEvent = (CLogEvent*)m_listAlarm.GetNext(m_posAlarm);
		CString strHead = pLogEvent->GetTipHead();
		CString strContent = pLogEvent->GetTipContent();
		pTip->SetTipText(strHead, strContent);
	}
	
	m_syncAlarmLoading.Unlock();	
}

void CLog::LoadAlarmLast(CStartupTip* pTip) 
{
	m_syncAlarmLoading.Lock();

	m_posAlarm = m_listAlarm.GetTailPosition();
	if (m_posAlarm != NULL)
	{
		CLogEvent* pLogEvent = (CLogEvent*)m_listAlarm.GetPrev(m_posAlarm);
		CString strHead = pLogEvent->GetTipHead();
		CString strContent = pLogEvent->GetTipContent();
		pTip->SetTipText(strHead, strContent);
	}

	m_syncAlarmLoading.Unlock();
}

void CLog::LoadAlarmPrev(CStartupTip* pTip) 
{
	m_syncAlarmLoading.Lock();

	if (m_posAlarm == NULL)
	{
		LoadAlarmLast(pTip);
	}
	else
	{
		CLogEvent* pLogEvent = (CLogEvent*)m_listAlarm.GetPrev(m_posAlarm);
		CString strHead = pLogEvent->GetTipHead();
		CString strContent = pLogEvent->GetTipContent();
		pTip->SetTipText(strHead, strContent);
	}
	
	m_syncAlarmLoading.Unlock();		
}

void CLog::LoadAlarmNext(CStartupTip* pTip) 
{
	m_syncAlarmLoading.Lock();

	if (m_posAlarm == NULL)
	{
		LoadAlarmFirst(pTip);
	}
	else
	{
		CLogEvent* pLogEvent = (CLogEvent*)m_listAlarm.GetNext(m_posAlarm);
		CString strHead = pLogEvent->GetTipHead();
		CString strContent = pLogEvent->GetTipContent();
		pTip->SetTipText(strHead, strContent);
	}

	m_syncAlarmLoading.Unlock();	
}

BOOL CLog::LoadAlarmList(CReportCtrl* pReportCtrl)
{
	CWaitCursor SomeWait;

	m_syncAlarmLoading.Lock();

	for (POSITION pos = m_listAlarm.GetHeadPosition(); pos != NULL; )
	{
		CLogEvent* pLogEvent = (CLogEvent*)m_listAlarm.GetNext(pos);
		if ( pLogEvent->m_bLoaded )		continue;
		pLogEvent->m_bLoaded = true;

		CStringList listTemp;
		listTemp.AddTail(pLogEvent->GetLuggNum());
		listTemp.AddTail(pLogEvent->m_strProdID);
		listTemp.AddTail(pLogEvent->GetPosition());
		listTemp.AddTail(pLogEvent->m_strContent);

		pReportCtrl->AddItem(pLogEvent->m_nType, pLogEvent->GetImageIndex(), &listTemp);

		listTemp.RemoveAll();
	}

	while( pReportCtrl->GetItemCount() > 10 )
		pReportCtrl->DeleteItem(0);

	pReportCtrl->ResizeColumn();

	m_syncAlarmLoading.Unlock();

	return TRUE;
}

void CLog::MultiCastPerLog(CLogEvent* pLogEvent)
{
//	if (m_pDoc->m_pMonitorListener == NULL)
//		return;
//
//	CString strSend;
//	strSend.Format("%c<ECS><MESSAGE>%s</MESSAGE></ECS>%c", CMonitorServer::enSTX, pLogEvent->GetXmlString(), CMonitorServer::enETX);
//	m_pDoc->m_pMonitorListener->SendToClients(strSend);
}

// LogEvent Class
/////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLogEvent, CObject, 0)

CLogEvent::CLogEvent(BYTE nLogPos, BYTE nType, int nLuggNum, CString strContent, CString strProdID, int nErrCode)
{
	m_tTime = CTime::GetCurrentTime();

	m_nPos       = nLogPos;
	m_nType      = nType;
	m_nLuggNum   = nLuggNum;
	m_strContent = strContent;
	m_strProdID  = strProdID;
	m_nErrCode   = nErrCode;

	m_bLoaded = false;
}

CString CLogEvent::GetString()
{
	CString strLogEvent;
	strLogEvent.Format(_T("%14s %02d %02d %04d %-13s %s\r\n"),
		m_tTime.Format(_T("%Y%m%d%H%M%S")),
		m_nType,
		m_nPos,
		m_nLuggNum,
		m_strProdID,
		m_strContent
	);
	return strLogEvent;
}

void CLogEvent::SetString(CString& strLogEvent)
{
	if (strLogEvent.GetLength() < 50)
		return;

	int nYear = _ttoi(strLogEvent.Left(4));
	int nMonth = _ttoi(strLogEvent.Mid(4,2));
	int nDay = _ttoi(strLogEvent.Mid(6,2));

	if (nYear < 1900) nYear = 2000;
	if ((nMonth < 1) || (nMonth > 12)) nMonth = 1;
	if ((nDay < 1) || (nDay > 31)) nDay = 1;

	m_tTime = CTime(nYear, nMonth, nDay, _ttoi(strLogEvent.Mid(8,2)), _ttoi(strLogEvent.Mid(10,2)), _ttoi(strLogEvent.Mid(12,2)));
	m_nType = _ttoi(strLogEvent.Mid(15,2));
	m_nPos = _ttoi(strLogEvent.Mid(18,2));
	m_nLuggNum = _ttoi(strLogEvent.Mid(21,4));
	m_strProdID = strLogEvent.Mid(26,13);
	m_strContent = strLogEvent.Mid(40);

	m_strProdID.TrimRight(' ');
	m_strContent.TrimLeft(' ');
}

void CLogEvent::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_tTime.GetTime();

		ar << m_nPos;
		ar << m_nType;
		ar << m_nLuggNum;
		ar << m_strContent;
		ar << m_strProdID;
		ar << m_nErrCode;
	}
	else
	{
		ar >> m_tTime;

		ar >> m_nPos;
		ar >> m_nType;
		ar >> m_nLuggNum;
		ar >> m_strContent;
		ar >> m_strProdID;
		ar >> m_nErrCode;
	}
}

DWORD CLogEvent::GetReadSize()
{
	DWORD dwSize = 0;
	dwSize += sizeof(m_tTime) + sizeof(m_nPos) + sizeof(m_nType) + sizeof(m_nLuggNum);
	dwSize += m_strContent.GetLength()+1 + m_strProdID.GetLength()+1 + sizeof(m_nErrCode);
	return dwSize;
}

CString CLogEvent::GetTime()
{
	return m_tTime.Format(_T("%H:%M:%S"));
}

CString CLogEvent::GetLogType()
{
	switch( m_nType )
	{
	case	LOG_TYPE_JOB:			return _T("Job");
	case	LOG_TYPE_EVENT:			return _T("Event");
	case	LOG_TYPE_ERROR:			return _T("Error");
	case	LOG_TYPE_ALARM:			return _T("Alarm");
	case	LOG_TYPE_DEBUG:			return _T("Debug");
	}

	return "";
}

CString CLogEvent::GetLuggNum()
{
	CString strLuggNum;
	if (m_nLuggNum != LOG_SYSTEM && m_nLuggNum != 0)
		strLuggNum.Format(_T("%d"), m_nLuggNum);

	return strLuggNum;
}

CString CLogEvent::GetErrorCode()
{
	if (m_nErrCode == 0)
		return "";

	CString strTemp;
	strTemp.Format(_T("%d"), m_nErrCode);
	return strTemp;
}

int CLogEvent::GetImageIndex()
{
	switch( m_nType )
	{
	case	LOG_TYPE_JOB:			return 0;
	case	LOG_TYPE_EVENT:			return 1;
	case	LOG_TYPE_ERROR:			return 2;
	case	LOG_TYPE_ALARM:			return 3;
	case	LOG_TYPE_DEBUG:			return 4;
	}

	return 0;
}

CString CLogEvent::GetPosition()
{
	return GetPosition(m_nPos);
}

CString CLogEvent::GetPosition(int nPos)
{
	switch (nPos)
	{
	case	LOG_POS_ALL:		return CString("ALL");
	case	LOG_POS_SYSTEM:		return CString("SYS");
	case	LOG_POS_HOST:		return CString("HOST");
	case	LOG_POS_CV:			return CString("CV");	
	case	LOG_POS_SC:			return CString("SC");
	case	LOG_POS_RGV:		return CString("RGV");
//	case	LOG_POS_LGV:		return CString("LGV");
//	case	LOG_POS_LIFTER:		return CString("LIFTER");
//	case	LOG_POS_RFID:		return CString("RFID");
//	case	LOG_POS_BCR:		return CString("BCR");
//	case	LOG_POS_DISPLAY:	return CString("DISPLAY");
	case	LOG_POS_MONITOR:	return CString("MONITOR");
	default:					return CString("DEFAULT");
	}
}

CString CLogEvent::GetTipHead()
{
	CString strTemp;
	strTemp.Format(_T("TIME : %s\\작업번호 : %s\\바코드ID : %s\\구분 : %s"),
					m_tTime.Format(_T("%Y년 %m월 %d일 %H:%M:%S")),
					GetLuggNum(),
					m_strProdID,
					GetPosition()
	);
	return strTemp;
}

CString CLogEvent::GetTipContent()
{
	return m_strContent;
}

CString CLogEvent::GetXmlString()
{
	CString strTextNode;
	strTextNode.Format(_T("<ALARM TM='%d' TP='%d' POS='%d' LNO='%d' PID='%s' MSG='%s'/>"),
					  m_tTime.GetTime(),
					  m_nType,
					  m_nPos,
					  m_nLuggNum,
					  m_strProdID,
					  m_strContent
	);
	return strTextNode;
}

// CSortContext Class
/////////////////////////////////////////////////////////////////////////

CSortContext::CSortContext()
{
	m_strFileName = _T("");

	m_nType = LOG_TYPE_ALL;		
	m_nPos = LOG_POS_ALL;
	m_nSort = LOG_SORT_NONE;

	m_strContext = _T("");
	m_nFrom = 0;
	m_nTo = 0;

	m_pFont = NULL;
}

BOOL CSortContext::IsValidContext()
{
	if (m_nFrom < 0 || m_nFrom > 23)
	{
		MessageBox(NULL, _T("작업기록 파일 검색 범위 이상!, 검색 시간을 확인 하세요.."), _T("검색범위"), MB_OK|MB_ICONWARNING);
		return false;
	}

	if (m_nTo < 1 || m_nTo > 24)
	{
		MessageBox(NULL, _T("작업기록 파일 검색 범위 이상!, 검색 시간을 확인 하세요.."), _T("검색범위"), MB_OK|MB_ICONWARNING);
		return false;
	}

	if (m_nFrom >= m_nTo)
	{
		MessageBox(NULL, _T("작업기록 파일 검색 범위 이상!, 검색 시간을 확인 하세요.."), _T("검색범위"), MB_OK|MB_ICONWARNING);
		return false;
	}

	if (m_strFileName.IsEmpty())
	{
		MessageBox(NULL, _T("작업기록 파일을 선택 하세요"), _T("파일선택"), MB_OK|MB_ICONWARNING);
		return false;
	}

	if ((m_nSort != LOG_SORT_NONE) && m_strContext.IsEmpty())
	{
		MessageBox(NULL, _T("검색내용을 입력 하세요"), _T("검색내용"), MB_OK|MB_ICONWARNING);
		return false;
	}

	return true;
}

BOOL CSortContext::Sorting(CLogEvent* pLogEvent)
{
	if (m_nType != LOG_TYPE_ALL && m_nType != pLogEvent->m_nType)
		return false;

	if (m_nPos != LOG_POS_ALL && m_nPos != pLogEvent->m_nPos)	
		return false;

	int nHour = pLogEvent->m_tTime.GetHour();
	if (nHour < m_nFrom || nHour >= m_nTo)
		return false;

	if ( !CheckLuggNum(pLogEvent->m_nLuggNum) )
		return false;

	if ( !CheckProdID(pLogEvent->m_strProdID) )
		return false;

	if ( !CheckContents(pLogEvent->m_strContent) )
		return false;

	return true;
}

BOOL CSortContext::CheckLuggNum(int nLuggNum)
{
	if ( m_nSort == LOG_SORT_LUGGID )
	{
		if ( _ttoi(m_strContext) == nLuggNum )
			return true;
		else
			return false;
	}

	return true;
}

BOOL CSortContext::CheckProdID(CString CheckProdID)
{
	if ( m_nSort == LOG_SORT_PRODID )
	{
		if( CheckProdID.Find(m_strContext) == -1)
			return false;
		else
			return true;
	}

	return true;
}

BOOL CSortContext::CheckContents(CString strContent)
{
	if ( m_nSort == LOG_SORT_STR )
	{
		if( strContent.Find(m_strContext) == -1)
			return false;
		else
			return true;
	}

	return true;
}
