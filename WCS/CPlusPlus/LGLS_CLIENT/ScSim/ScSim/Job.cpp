// Job.cpp: implementation of the CJob class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "Job.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static BOOL g_bBackup = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJob::CJob(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
}

CJob::~CJob()
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	for (POSITION pos=CObList::GetHeadPosition(); pos!=NULL; )
		delete (CJobItem*)CObList::GetNext(pos);
	CObList::RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CJob::Backup()
{
	if (g_bBackup == TRUE)	return FALSE;

	m_csSyncBackup.Lock();
	g_bBackup = TRUE;

	CString strJobFile, strBackupFile;
	strJobFile.Format(_T("%s\\JOB.ECS"), m_pDoc->m_pConfig->m_strJobPath);
	strBackupFile.Format(_T("%s\\JOB.BKP"), m_pDoc->m_pConfig->m_strJobPath);

	TRY
	{
		// 파일이 존재하는지 체크 
		if (::GetFileAttributes(strBackupFile) != 0xFFFFFFFF) 
			CFile::Remove(strBackupFile);

		// 파일이 존재하는지 체크
		if (::GetFileAttributes(strJobFile) != 0xFFFFFFFF) 
			CFile::Rename(strJobFile, strBackupFile);
	}
	CATCH (CException, e)
	{
		int aaaa= 0;
		//DEBUGER_TRACE(CLib::GetExceptionString(e), _T("CJob::Backup"));
	}
	END_CATCH

	TRY
	{
//		CFile f(strJobFile, CFile::modeCreate | CFile::modeWrite);
		CStdioFile f((LPCTSTR)strJobFile, CFile::modeCreate | CFile::modeWrite);

		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();

		f.Close();
	}
	CATCH (CException, e)
	{
		CString strLog;
		strLog.Format(_T("작업정보 백업 실패!"));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Backup"));
		m_csSyncBackup.Unlock();
		g_bBackup = FALSE;
		return FALSE;
	}
	END_CATCH

	m_csSyncBackup.Unlock();
	g_bBackup = FALSE;
	
	return TRUE;
}

BOOL CJob::Restore(int nMode)
{
	//CString strFileName;
	CString strJobFile, strBackupFile;
	CString strJobFile2, strBackupFile2;
	strJobFile.Format(_T("%s\\JOB.ECS"), m_pDoc->m_pConfig->m_strJobPath);
	strBackupFile.Format(_T("%s\\JOB.BKP"), m_pDoc->m_pConfig->m_strJobPath);
	strJobFile2.Format(_T("%s\\JOB2.ECS"), m_pDoc->m_pConfig->m_strJobPath);
	strBackupFile2.Format(_T("%s\\JOB2.BKP"), m_pDoc->m_pConfig->m_strJobPath);

	TRY
	{
		if ((::GetFileAttributes(strJobFile) != 0xFFFFFFFF)) 
		{
			if (::GetFileAttributes(strJobFile2) != 0xFFFFFFFF)
				CFile::Remove(strJobFile2);

			CopyFile(strJobFile, strJobFile2, TRUE);
		}

		if (::GetFileAttributes(strBackupFile) != 0xFFFFFFFF) 
		{
			if (::GetFileAttributes(strBackupFile2) != 0xFFFFFFFF)
				CFile::Remove(strBackupFile2);
	
			CopyFile(strBackupFile, strBackupFile2, TRUE);
		}

		if (nMode == 1)
		{
			if ((::GetFileAttributes(strJobFile) != 0xFFFFFFFF) && (::GetFileAttributes(strBackupFile) != 0xFFFFFFFF)) 
			{
				CFile::Remove(strJobFile);
				CFile::Rename(strBackupFile, strJobFile);
			}
		}

//		CFile f(strJobFile, CFile::modeRead);
		CStdioFile f((LPCTSTR)strJobFile, CFile::modeRead);

		CArchive ar(&f, CArchive::load);
		Serialize(ar);
		ar.Close();

		CSingleLock SingleLock(&m_csSyncJobList);
		SingleLock.Lock();

		CJobItem* pJobItem = NULL;
		for (POSITION pos=GetHeadPosition(); pos!=NULL; )
		{
			pJobItem = (CJobItem*)GetNext(pos);

			if (pJobItem == NULL)
				continue;

			if (pJobItem != NULL)
				pJobItem->m_pDoc = m_pDoc;
		}

		f.Close();
	}
	CATCH (CArchiveException, e)
	{
		//CString strLog;
		//strLog.Format(_T("작업정보 복원 실패! ( ArchiveException = %s )"), szMessage CLib::GetExceptionString(e));
		//strLog = _T("작업정보 복원 실패! (ArchiveException)");
		//m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Restore"));
		return FALSE;
	}
	CATCH (CFileException, e)
	{
		//CString strLog;
		//strLog.Format(_T("작업정보 복원 실패! ( FileException = %s )"), CString(szMessage) CLib::GetExceptionString(e));
		//strLog = _T("작업정보 복원 실패! (FileException)");
		//m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Restore"));
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

void CJob::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		for (int i=0; i<10; i++)
		{
			for (int j=0; j<10000; j++)
				ar << m_pDoc->m_arrRegData[i][j];
		}
	}
	else
	{
		for (int i=0; i<10; i++)
		{
			for (int j=0; j<10000; j++)
				ar >> m_pDoc->m_arrRegData[i][j];
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CJobItem* CJob::Add(CJobItem& rJobItem)
{
	CJobItem* pJobItem = new CJobItem(m_pDoc, rJobItem);
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	TRY
	{
		CObList::AddTail(pJobItem);
	}
	CATCH (CMemoryException, e)
	{
		DEBUGER_TRACE(CLib::GetExceptionString(e), _T("CJob::Add"));
		delete pJobItem;
		return NULL;
	}
	END_CATCH

	return pJobItem;
}

BOOL CJob::Remove(CJobItem* pJobItem)
{
	if (pJobItem == NULL)
		return FALSE;

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	POSITION pos = CObList::Find(pJobItem);
	if (pos == NULL)
	{
		return FALSE;
	}

	pJobItem->MulticastRemoveJob();

	CObList::RemoveAt(pos);
	delete pJobItem;

	// Backup();		// @@
	m_pDoc->m_bBackupChk = TRUE;

	return TRUE;
}

void CJob::CheckIdleJob()
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos = GetHeadPosition(); pos != NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if (pJobItem->IsNewTask())
			continue;

		switch (pJobItem->m_nJobStatus)
		{
		case enJobStatusArrived:
			{
				CTimeSpan tElapseTime = CTime::GetCurrentTime() - pJobItem->m_tTime;
				if (tElapseTime.GetTotalMinutes() > 3)
				{
					// 설비에서 존재하는 작업인지 체크
					if (m_pDoc->IsIdleJob(pJobItem->m_nLuggNum) == FALSE)
						continue;

					Remove(pJobItem);
					continue;
				}
//----------------------------------------------------------------------------------------
			}
			break;
		case enJobStatusCancel:
		case enJobStatusErrorDualStore:
		case enJobStatusErrorEmptyRetrieve:
			{
				CTimeSpan tElapseTime = CTime::GetCurrentTime() - pJobItem->m_tTime;
				if (tElapseTime.GetTotalMinutes() < 1)
					continue;

				if (pJobItem->IsOfflineJobType())
				{
					if (pJobItem->m_nJobStatus != enJobStatusErrorDualStore)
					{
						m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_SYSTEM, _T("작업삭제 (OFFLINE)"), _T("CJob::CheckIdleJob"), pJobItem);
						Remove(pJobItem);
					}
					continue;
				}


			}
			break;

		default:
			{
				CTimeSpan tElapseTime = CTime::GetCurrentTime() - pJobItem->m_tTime;

				if (pJobItem->GetJobPattern() == enJobPatternMove || pJobItem->m_nJobStatus == enJobStatusError)
				{
// Update : 2014.06.15 - Degassing #1 구간은 입출고 도착보고 바코드가 BCR#613으로 동일하고 입출고 처리시간이 짧아 
//						 입고시 생성한 Degassing 이동 작업정보가 출고시에도 남아 있어 작업이 진행되지 않는 문제 발생
					if (tElapseTime.GetTotalMinutes() > 3)
					{
						// 설비에서 존재하는 작업인지 체크
						if (m_pDoc->IsIdleJob(pJobItem->m_nLuggNum) == FALSE)
							continue;

						Remove(pJobItem);
						continue;
					}
				}
/*
				if (tElapseTime.GetTotalMinutes() < 5)
					continue;

				// 중앙설비 및 Aging HS 구간, 공트레이 반납 구간은 이상작업 발생 제외
				if (pJobItem->m_strDestStn == _T("101") || pJobItem->m_strDestStn == _T("102") ||
					pJobItem->m_strDestStn == _T("103") || pJobItem->m_strDestStn == _T("104") ||
					pJobItem->m_strDestStn == _T("105") || pJobItem->m_strDestStn == _T("106") ||
					pJobItem->m_strDestStn == _T("107") || pJobItem->m_strDestStn == _T("520") ||
					pJobItem->m_strDestStn == _T("521") || pJobItem->m_strDestStn == _T("531") ||
					pJobItem->m_strDestStn == _T("532") || pJobItem->m_strDestStn == _T("536") ||
					pJobItem->m_strDestStn == _T("541") || pJobItem->m_strDestStn == _T("542") ||
					pJobItem->m_strDestStn == _T("550") || pJobItem->m_strDestStn == _T("551") ||
					pJobItem->m_strDestStn == _T("552") || pJobItem->m_strDestStn == _T("561") ||
					pJobItem->m_strDestStn == _T("562") || pJobItem->m_strDestStn == _T("571") ||
					pJobItem->m_strDestStn == _T("572") || pJobItem->m_strDestStn == _T("573") ||
					pJobItem->m_strDestStn == _T("574") || pJobItem->m_strDestStn == _T("580") ||
					pJobItem->m_strDestStn == _T("581") || pJobItem->m_strDestStn == _T("582") ||
					pJobItem->m_strDestStn == _T("654"))
					continue;

				// 만일 1시간 이상 누적된 작업이 있을 경우 이상작업 발생 제외
				if (tElapseTime.GetTotalHours() > 1)		continue;

				// 설비에서 존재하는 작업인지 체크
				if (m_pDoc->IsIdleJob(pJobItem->m_nLuggNum) == FALSE)
					continue;

				CString strLog;
				strLog.Format(_T("이상작업 발생! 확인요망! [%s]"), pJobItem->GetLogString());
				m_pDoc->Alarm(LOG_POS_SYSTEM, strLog, pJobItem);
*/			}
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CJob::Invoke(int nLuggNum)
{
	CJobItem* pJobItem = NULL;

	if (nLuggNum == 0)
	{
		pJobItem = FetchByStatus(enJobStatusNew);
		if (pJobItem == NULL)	return FALSE;
	}
	else
	{
		pJobItem = Find(nLuggNum);
		if (pJobItem == NULL)
		{
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, _T("INVOKE 실패! [작업정보 없음]"), _T("CJob::Invoke"));
			return FALSE;
		}
	}

	return Invoke(pJobItem);
}

BOOL CJob::Invoke(CJobItem* pJobItem)
{
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	CString strLog;
	switch (pJobItem->GetJobPattern())
	{
	case enJobPatternSto:
	case enJobPatternMove:
		{
			pJobItem->m_nJobStatus = enJobStatusCvNew;
			pJobItem->MulticastAddJob();
			// Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;
//			strLog.Format(_T("CV 구동요구 ( 작업구분=%s, 출발지=%s, 도착지=%s )"), 
//				pJobItem->GetJobTypeString(), pJobItem->GetStartStationString(), pJobItem->GetDestStationString());
//			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Invoke"), pJobItem);
		}
		return TRUE;

	case enJobPatternRet:
	case enJobPatternR2R:
	case enJobPatternW2W:
	case enJobPatternPR:
		{
			pJobItem->m_nJobStatus = enJobStatusScRequest;
			pJobItem->MulticastAddJob();
			// Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;

			strLog.Format(_T("SC 구동요구 [작업구분=%s] [출발지=%s] [도착지=%s]"), 
				pJobItem->GetJobTypeString(), pJobItem->GetDestStationString(), pJobItem->GetDestStationString());
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Invoke"), pJobItem);
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CJob::Complete(CJobItem* pJobItem, BYTE ucScJobType, BOOL bManual /* = FALSE */)
{
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	if (CObList::Find(pJobItem) == NULL)	return FALSE;

	CString strLog;
	if (pJobItem->GetJobPattern() == enJobPatternMove)
	{
		strLog.Format(_T("작업완료 작업구분 이상! (%s)"), pJobItem->GetJobTypeString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);
		return FALSE;
	}

	strLog.Format(_T("작업완료 (%s)"), pJobItem->GetLogString());
	m_pDoc->WriteLog(bManual ? LOG_TYPE_EVENT : LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);

	// TRACK#457 - 화재발생 알람 OFF
	CTrackInfo* pTrack = m_pDoc->GetTrackInfo(457, 0);
	if (pTrack != NULL)
	{
		if (pJobItem->m_nPriority == 119 && pTrack->m_pEquipment->IsConnect(0) == TRUE)
		{
			CCmdMsg* pCmdMsg = new CCmdMsg;
			pCmdMsg->m_nCommand = CCvCmdMsg::enCmdFireAlarm;
			pCmdMsg->m_strValues.SetSize(CCvCmdMsg::enSizeFireAlarm);
			pCmdMsg->m_strValues[0].Format(_T("%d"), 457);
			pCmdMsg->m_strValues[1].Format(_T("%d"), 0);
			pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);

			strLog = _T("화재발생 강제이동 자동 알람 OFF");
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);
		}
	}

/*
	if (pJobItem->IsOfflineJobType())
	{
		// 입고일때
		if (ucScJobType == CScInfo::enStore)
		{
			// 현재 작업을 삭제한다.
			Remove(pJobItem);

//			m_pDoc->m_bTesting = TRUE;
			// 현재 Test 모드일경우 출고작업을 다시 지시한다.
			if(m_pDoc->m_bTesting == TRUE)
			{
				// 현재 출고 중인 작업이 없을 경우에는 Test모드를 중지한다.
				CJobItem* pRetJobItem = FetchByTestRetJob(m_pDoc->m_strTestLocation);
				if(pRetJobItem == NULL)
				{
					m_pDoc->m_bTesting = FALSE;
					return TRUE;
				}

				// 크래인에게 출고작업을 지시한다.
				pRetJobItem->SetJobStatus(enJobStatusScRequest);
			}
		}
		else if(ucScJobType == CScInfo::enRetrieve)
		{
//			enJobStatusScComplete
			// 현재 Test 모드일경우 입고작업을 생성하고, CCv::InvokeScRetHS에서 입고작업을 적는다.
			if(m_pDoc->m_bTesting == TRUE)
			{
				// 현재 작업이 크래인 완료상태이고, 작업정보의 출발 Location이 Test모드의 Location과 같을경우
				if (pJobItem->m_nJobStatus == enJobStatusScComplete && pJobItem->m_strStartLoc == m_pDoc->m_strTestLocation)
				{
					// 입고작업을 생성한다. - 작업번호를 생성한다.
					int nLuggNum = m_pDoc->m_pJob->GenerateOfflineLuggNum();
					if (nLuggNum == 0)
					{
						// 로그로 대체할것!
//							AfxMessageBox(_T("작업번호 생성 실패!"));
						strLog.Format(_T("%d TEST 입고 작업번호 생성 실패!", nLuggNum);
						m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);
						m_pDoc->m_bTesting = FALSE;
						return TRUE;
					}

					// 입고작업을 생성한다.
					CJobItem oJobItem(m_pDoc);
					CJobItem* pJobItem = NULL;
					CStationInfo* pStartStation = NULL;
					CStationInfo* pDestStation = NULL;
					
//					pStartStation = (CStationInfo*)m_cbxStartStn.GetItemDataPtr(m_cbxStartStn.GetCurSel());
					pDestStation = m_pDoc->GetScStationInfo(1, m_pDoc->m_strTestLocation);
//					DEBUGER_ASSERT_VALID(pStartStation != NULL);
					DEBUGER_ASSERT_VALID(pDestStation != NULL);

					oJobItem.m_nLuggNum		= nLuggNum;
					oJobItem.m_nJobType		= enJobTypeSemiSto;
					oJobItem.m_nStartWH		= 1;
					oJobItem.m_strStartStn	= m_pDoc->m_strTestStation; //"61101";//pStartStation->m_strID;
					oJobItem.m_nDestWH		= 1;
					oJobItem.m_strDestStn	= pDestStation->m_strID;
					oJobItem.m_strDestLoc	= m_pDoc->m_strTestLocation;
					oJobItem.m_ucGenCode	= 1;

//					// 동일한 작업이 존재하는지 체크함!
//					if (pJobItem = pDoc->m_pJob->IsEqualAll(&oJobItem) != NULL)
//					{
//						// 동일한 작업이 존재 할경우 그 작업이 CV 구동중이며, 트정 트랙에 있는 것과 같은지 확인할것! 
//						CTrackInfo * pTrackInfo = pDoc->GetTrackInfo(7);
//						if (pJobItem->m_nStatus == enJobStatusCvInvoke && pJobItem == pDoc->m_pJob->Find(pTrackInfo->m_nLuggNum))
//							// 같을 경우에 크래인에게 작업지시함
//							pJobItem->SetJobStatus(enJobStatusScRequest);
//			//			else
//
//							return;
//
//			//			strLog.Format(_T("동일한 작업이 존재... FMS 확인 [%d]"), oJobItem.m_nLuggNum);
//			//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CJob::Complete"), &oJobItem);
//			//			Answer(enHostCommandOrder, oJobItem.m_nLuggNum, oJobItem.m_nJobType, enHostErrorAlreadyJob);
//					}

					if ((pJobItem = m_pDoc->m_pJob->Add(oJobItem)) == NULL)
					{
						strLog.Format(_T("%s TEST 입고 작업생성 실패!", oJobItem.GetJobTypeString());
						m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);
						m_pDoc->m_bTesting = FALSE;
						return TRUE;
					}

					strLog.Format(_T("%s TEST 입고 작업생성"), oJobItem.GetJobTypeString());
					m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);

//					if (m_pDoc->m_pJob->Invoke(pJobItem) == FALSE)
//					{
//						strLog.Format(_T("%s 작업지시 실패!", oJobItem.GetJobTypeString());
//						m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);
//						return;
//					}
				}
			}
		}

		return TRUE;
	}
*/
	if (ucScJobType == CScInfo::enStore)
		pJobItem->SetJobStatus(enJobStatusComplete);

	return TRUE;
}

//=============================================================================
//	Desc	: 출고대 도착완료 후 삭제
//	Date	:
//	Update	: 
//=============================================================================
BOOL CJob::Arrive(CJobItem* pJobItem, BOOL bManual /* = FALSE */)
{
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	if (CObList::Find(pJobItem) == NULL)
		return FALSE;

	CString strLog;
	switch (pJobItem->GetJobPattern())
	{
	case enJobPatternRet:
	case enJobPatternPR:
	case enJobPatternMove:
		break;

	default:
		//strLog.Format(_T("도착완료 작업구분 이상! [%s]"), pJobItem->GetJobTypeString());
		//m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Arrive"), pJobItem);
		return FALSE;
	}

	strLog.Format(_T("도착완료 ( %s )"), pJobItem->GetLogString());
	m_pDoc->WriteLog(bManual ? LOG_TYPE_EVENT : LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Arrive"), pJobItem);

	if (pJobItem->IsOfflineJobType())
	{
		Remove(pJobItem);
		return TRUE;
	}

	pJobItem->SetJobStatus(enJobStatusArrived);

	return TRUE;
}

//=============================================================================
//	Desc	: 작업대 도착 보고 응답 후 삭제
//	Date	:
//	Update	: 
//=============================================================================
BOOL CJob::Arrive2(CJobItem* pJobItem, BOOL bManual /* = FALSE */)
{
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	if (CObList::Find(pJobItem) == NULL)
		return FALSE;

	CString strLog;
	switch (pJobItem->GetJobPattern())
	{
	case enJobPatternRet:
	case enJobPatternPR:
	case enJobPatternMove:
		break;

	default:
		//strLog.Format(_T("도착완료 작업구분 이상! [%s]"), pJobItem->GetJobTypeString());
		//m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Arrive2"), pJobItem);
		return FALSE;
	}

	strLog.Format(_T("도착완료 ( %s )"), pJobItem->GetLogString());
	m_pDoc->WriteLog(bManual ? LOG_TYPE_EVENT : LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Arrive2"), pJobItem);

	if (pJobItem->IsOfflineJobType())
	{
		Remove(pJobItem);
		return TRUE;
	}

	pJobItem->SetJobStatus(enJobStatusArrived);

	return TRUE;
}

//=============================================================================
//	Desc	: Aging 입고대 도착 보고
//	Date	:
//	Update	: 
//=============================================================================
BOOL CJob::Arrive3(CJobItem* pJobItem, CStationInfo* pStation, BOOL bManual /* = FALSE */)
{
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	if (CObList::Find(pJobItem) == NULL)
		return FALSE;

	CString strLog;
//	switch (pJobItem->GetJobPattern())
//	{
//	case enJobPatternRet:
//	case enJobPatternPR:
//	case enJobPatternMove:
//		break;
//
//	default:
//		strLog.Format(_T("도착완료 작업구분 이상! [%s]"), pJobItem->GetJobTypeString());
//		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Arrive3"), pJobItem);
//		return FALSE;
//	}

	strLog.Format(_T("도착완료 ( %s )"), pJobItem->GetLogString());
	m_pDoc->WriteLog(bManual ? LOG_TYPE_EVENT : LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Arrive3"), pJobItem);

	if (pJobItem->IsOfflineJobType())
	{
		Remove(pJobItem);
		return TRUE;
	}

	pJobItem->SetJobStatus(enJobStatusArrived);

	return TRUE;
}

BOOL CJob::Cancel(CJobItem* pJobItem, BOOL bManual /* = FALSE */)
{
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	if (CObList::Find(pJobItem) == NULL)
		return FALSE;

	CString strLog;
	strLog.Format(_T("작업취소 [%s]"), pJobItem->GetLogString());
	m_pDoc->WriteLog(bManual ? LOG_TYPE_EVENT : LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Cancel"), pJobItem);

	if (pJobItem->IsOfflineJobType())
	{
		Remove(pJobItem);
		return TRUE;
	}

	pJobItem->SetJobStatus(enJobStatusCancel);

//	if (m_pDoc->m_pHostSv && m_pDoc->m_pHostSv->IsConnect())
//		m_pDoc->m_pHostSv->CancelReport(pJobItem, bManual);

	return TRUE;
}

BOOL CJob::Request(CString& strStationID, CString& strBarcode, BOOL bManual /* = FALSE */)
{
	CString strLog;
	strLog.Format(_T("작업요청(BCR 보고) [입고대=%s, BARCODE=%s]"), m_pDoc->GetStationName(strStationID), strBarcode);
	m_pDoc->WriteLog(bManual ? LOG_TYPE_EVENT : LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Request"));

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CJobItem* CJob::Find(int nLuggNum)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if (pJobItem->m_nLuggNum == nLuggNum)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::Find(CString strBcr)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if (pJobItem->m_nJobStatus == enJobStatusError)
			continue;

		if (pJobItem->m_strBarcode == strBcr)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::IsEqualAll(CJobItem* rJobItem)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if ((rJobItem->m_nJobType		== pJobItem->m_nJobType) && 
			(rJobItem->m_strBarcode		== pJobItem->m_strBarcode) &&
			(rJobItem->m_nStartWH		== pJobItem->m_nStartWH) && 
			(rJobItem->m_strStartStn	== pJobItem->m_strStartStn) && 
			(rJobItem->m_strStartLoc	== pJobItem->m_strStartLoc) &&
			(rJobItem->m_nDestWH		== pJobItem->m_nDestWH) && 
			(rJobItem->m_strDestStn		== pJobItem->m_strDestStn) && 
			(rJobItem->m_strDestLoc		== pJobItem->m_strDestLoc) )
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchByStatus(int nJobStatus)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if (pJobItem->m_nJobStatus == nJobStatus)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchByRetDestination(CString& strDestStationID)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strDestStationID), strDestStationID);

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if ((pJobItem->GetJobPattern() != enJobPatternRet) &&
			(pJobItem->GetJobPattern() != enJobPatternMove))
			continue;

		if (pJobItem->m_strDestStn == strDestStationID)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchByTestStoJob(CString strLocation)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;
		
		if (pJobItem->GetJobPattern() != enJobPatternSto)
			continue;

		if (pJobItem->m_nJobStatus != enJobStatusNew)
			continue;

		if (pJobItem->IsOfflineLuggNum() == FALSE)
			continue;
		
		if (pJobItem->m_ucGenCode != 1)
			continue;

		if (pJobItem->m_strDestLoc == strLocation)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchByTestRetJob(CString strLocation)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;
		
		if (pJobItem->GetJobPattern() != enJobPatternRet)
			continue;

		if (pJobItem->m_nJobStatus != enJobStatusCvInvoke)
			continue;

		if (pJobItem->IsOfflineLuggNum() == FALSE)
			continue;
		
		if (pJobItem->m_ucGenCode != 1)
			continue;

		if (pJobItem->m_strStartLoc == strLocation)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchByTestJob(int nJobPattern)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;
		
//		if (pJobItem->GetJobPattern() != enJobPatternRet)
//			continue;

//		if (pJobItem->m_nJobStatus != enJobStatusCvInvoke)
//			continue;

		if (pJobItem->IsOfflineLuggNum() == FALSE)
			continue;
		
		if (pJobItem->m_ucGenCode != 1)
			continue;

		if (pJobItem->GetJobPattern() == nJobPattern)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchDualStoreJobBySc(int nScNum)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if ((pJobItem->m_nJobStatus == enJobStatusDualStoreRetry) && 
			(CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc) == nScNum))
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchStoreJobByCv(CString& strStoStationID, CString& strTrayId)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strStoStationID), strStoStationID);

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if ((pJobItem->GetJobPattern() != enJobPatternSto) &&
			(pJobItem->GetJobPattern() != enJobPatternMove))
			continue;

		if ((pJobItem->m_nJobStatus == enJobStatusCvNew) && 
			(pJobItem->m_strStartStn == strStoStationID) &&
			(pJobItem->m_strBarcode == strTrayId))
			return pJobItem;
	}

	return NULL;
}

//=============================================================================
//	Desc	: 
//	Date	:
//	Update	: 2013.09
//=============================================================================
CJobItem* CJob::FetchRetrieveJobBySc(int nScNum, CString& strDestStationID)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strDestStationID), strDestStationID);

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	POSITION pos = NULL;
	// 화재발생시 긴급 출고
	for (pos = GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		if (pJobItem == NULL)	continue;
		if ((pJobItem->m_nPriority == 119) &&
			(pJobItem->m_nJobStatus == enJobStatusScRequest) && 
			(pJobItem->m_strDestStn == strDestStationID) && 
			(CLib::GetStackerNum(pJobItem->m_nStartWH, pJobItem->m_strStartLoc) == nScNum))
		{
			return pJobItem;
		}
	}

	//pos = NULL;
	for (pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if ((pJobItem->GetJobPattern() != enJobPatternRet) && 
			(pJobItem->GetJobPattern() != enJobPatternPR) && 
			(pJobItem->GetJobPattern() != enJobPatternW2W))
			continue;

		if ((pJobItem->m_nJobStatus == enJobStatusScRequest) && 
			(pJobItem->m_strDestStn == strDestStationID) && 
			(CLib::GetStackerNum(pJobItem->m_nStartWH, pJobItem->m_strStartLoc) == nScNum))
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchRackToRackJobBySc(int nScNum, int nExceptLuggNum)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if ((pJobItem->GetJobPattern() == enJobPatternR2R) &&
			(pJobItem->m_nJobStatus == enJobStatusScRequest) && 
			(CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc) == nScNum) &&
			(CLib::GetStackerNum(pJobItem->m_nStartWH, pJobItem->m_strStartLoc) == nScNum))
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchCompleteJob(int nJobStatus, int nArriveTrackNum)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if ((pJobItem->m_nJobStatus == nJobStatus) && 
			(pJobItem->m_nArriveTrackNum == nArriveTrackNum))
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchLgvSemiMoveJob(CString& strStartStn, CString& strDestStn)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if (pJobItem->m_nJobType != enJobTypeSemiMove)
			continue;
		
		if ((pJobItem->m_strStartStn == strStartStn) ||
			(pJobItem->m_strDestStn == strDestStn))
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchReservedJobByStartStation(CString& strStationID)
{
	DEBUGER_ASSERT_VALID(CStationInfo::IsValidID(strStationID));

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if (pJobItem->m_strStartStn != strStationID)
			continue;

		if ((pJobItem->GetJobPattern() != enJobPatternSto) &&
			(pJobItem->GetJobPattern() != enJobPatternMove))
			continue;

		if ((pJobItem->m_nJobStatus == enJobStatusNew) || 
			(pJobItem->m_nJobStatus == enJobStatusCvNew))
			return pJobItem;
	}

	return NULL;
}

int CJob::GenerateOfflineLuggNum()
{
	int nLuggNum = 9000;
	CJobItem* pJobItem = NULL;

	for (POSITION pos=GetTailPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetPrev(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);
		if (pJobItem == NULL)	continue;

		if (pJobItem->IsOfflineLuggNum())
		{
			nLuggNum = pJobItem->m_nLuggNum;
			break;
		}
	}

	for (int i = 0; i < 990; ++i)
	{
		nLuggNum = (nLuggNum > 9990) ? 9001 : nLuggNum + 1;
		pJobItem = Find(nLuggNum);

		if ((pJobItem == NULL) &&
			(m_pDoc->m_pEquipments.IsIdleJob(nLuggNum) == TRUE))
			return nLuggNum;
	}

	return 0;
}

int CJob::GenerateOnlineLuggNum()
{
	static int nLuggNum = 0;
	CJobItem* pJobItem = NULL;

	/*
	for (POSITION pos=GetTailPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetPrev(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if (pJobItem->IsOfflineLuggNum())
		{
			nLuggNum = pJobItem->m_nLuggNum;
			break;
		}
	}
	*/

	// Q >> 자동 작업번호 변수를 static으로 변경하면서 생성된 작업번호가 9000번을 넘어가는 문제 발생
	// A >> 자동 작업번호를 8990번으로 제한
	for (int i=0; i < 8990; ++i)
	{
		nLuggNum = (nLuggNum > 8990) ? 1 : nLuggNum + 1;
		pJobItem = Find(nLuggNum);

		if ((pJobItem == NULL) && (m_pDoc->m_pEquipments.IsIdleJob(nLuggNum) == TRUE))
			return nLuggNum;
	}

	return 0;
}

BOOL CJob::IsValidScRetrieveTask(CString& strDestStn)
{
	if (CStationInfo::IsValidID(strDestStn) == FALSE)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}

	CString strRetLimitCount;		// 제한된 갯수
	if (m_pDoc->m_mapRetLimitInfos.Lookup(strDestStn, strRetLimitCount) == FALSE)
		return TRUE;

	CStationInfo* pStation = m_pDoc->GetStationInfo(strDestStn);
	DEBUGER_ASSERT_VALID(pStation != NULL);
	DEBUGER_ASSERT_VALID(pStation->m_pTrack != NULL);
	DEBUGER_ASSERT_VALID((pStation->m_enKind == CStationInfo::enRetStation)||(pStation->m_enKind == CStationInfo::enArvStation));

	int nRetLimitCount=0;
	int nJobCount=0;
	nRetLimitCount = _ttoi(strRetLimitCount);
	if ((pStation == NULL) ||
		(pStation->m_pTrack->IsRetStationReady() == TRUE) ||
		(pStation->m_pTrack->IsProductSensing() == TRUE) ||
		(pStation->m_pTrack->m_nLuggNum != 0))
	{
		nRetLimitCount++;
		nJobCount++;
//		strRetLimitCount.Format(_T("%d", nRetLimitCount);
	}

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CString strJobCount;
	CJobItem* pJobItem = NULL;
	CMapStringToString mapJobCount;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if ((pJobItem->IsNewTask() == FALSE) &&
			(m_pDoc->m_mapRetLimitInfos.Lookup(pJobItem->m_strDestStn, strJobCount) == TRUE))
		{
			nJobCount++;
//			if (!mapJobCount.Lookup(pJobItem->m_strDestStn, strJobCount)) 
//				mapJobCount.SetAt(pJobItem->m_strDestStn, _T("1"));
//			else 
//				mapJobCount.SetAt(pJobItem->m_strDestStn, CConvert::ToString(_ttoi(strJobCount)+1));
		}
	}

//	if ((mapJobCount.Lookup(strDestStn, strJobCount) == TRUE) && (_ttoi(strJobCount) >= nRetLimitCount))
//		return FALSE;
	if (nJobCount >= nRetLimitCount)
		return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CJob::MulticastAllJobPerClient(CMonitorServer* pMonitorSv)
{
	if (pMonitorSv == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	CString strSend;
	strSend.Format(_T("%c<ECS><JOB><REMOVEALL/></JOB></ECS>%c"), CMonitorServer::enSTX, CMonitorServer::enETX);
	pMonitorSv->SendData(strSend);

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CString strJobItems;
	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		strJobItems += pJobItem->GetXmlString();
		if (strJobItems.GetLength() > CMonitorServer::enSendBufferMaxSize)
		{
			strSend.Format(_T("%c<ECS><JOB><ADD>%s</ADD></JOB></ECS>%c"), CMonitorServer::enSTX, strJobItems, CMonitorServer::enETX);
			pMonitorSv->SendData(strSend);
			strJobItems.Empty();
		}
	}

	if (strJobItems.IsEmpty() == FALSE)
	{
		strSend.Format(_T("%c<ECS><JOB><ADD>%s</ADD></JOB></ECS>%c"), CMonitorServer::enSTX, strJobItems, CMonitorServer::enETX);
		pMonitorSv->SendData(strSend);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CJob::WritePlayBack()
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CString strTxBuff;
	CString strTxData;

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		strTxData += pJobItem->GetXmlString();
		strTxBuff.Format(_T("%c<ECS><JOB>%s</JOB></ECS>%c"), STX, strTxData, ETX);
//		m_pDoc->m_pLogPlayback->Log(strTxBuff);
	}
}

BOOL CJob::BkpRestore()
{
	CString strFileName;
	strFileName.Format(_T("%s\\JOB.BKP"), m_pDoc->m_pConfig->m_strJobPath);

	TRY
	{
		CFile f(strFileName, CFile::modeRead);

		CArchive ar(&f, CArchive::load);
		Serialize(ar);
		ar.Close();

		CSingleLock SingleLock(&m_csSyncJobList);
		SingleLock.Lock();

		CJobItem* pJobItem = NULL;
		for (POSITION pos=GetHeadPosition(); pos!=NULL; )
		{
			pJobItem = (CJobItem*)GetNext(pos);
			DEBUGER_ASSERT_VALID(pJobItem != NULL);
			pJobItem->m_pDoc = m_pDoc;
		}

		f.Close();
	}
	CATCH (CArchiveException, e)
	{
		CString strLog;
		strLog.Format(_T("백업 작업정보 복원 실패! [%s]"), CLib::GetExceptionString(e));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Restore"));
		return FALSE;
	}
	CATCH (CFileException, e)
	{
		DEBUGER_TRACE(CLib::GetExceptionString(e), _T("CJob::Restore"));
	}
	END_CATCH

	return TRUE;
}


CJobItem* CJob::FetchRetrieveJobByTwinSc(int nScNum, CString& strDestStationID, int nExceptLuggNum)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strDestStationID), strDestStationID);

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	int nBank1 = 0, nBay1 = 0, nLevel1 = 0;
	int nBank2 = 0, nBay2 = 0, nLevel2 = 0;
	CJobItem* pJobItem = NULL;
	CJobItem* pExceptJobItem = NULL;

	CJobItem* pJobItem1 = NULL;		// 우선순위 제일 높음
	CJobItem* pJobItem2 = NULL;		// 우선순위 제일 중간
	CJobItem* pJobItem3 = NULL;		// 우선순위 제일 낮음
//	CJobItem* pJobItem4 = NULL;
//	CJobItem* pJobItem5 = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);

		if(pJobItem == NULL)
			continue;

		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if ( nExceptLuggNum != 0 && pJobItem->m_nLuggNum == nExceptLuggNum )
		{
			nBank1  = CLib::GetBank(pJobItem->m_strStartLoc);
			nBay1   = CLib::GetBay(pJobItem->m_strStartLoc);
			nLevel1 = CLib::GetLevel(pJobItem->m_strStartLoc);
			
			pExceptJobItem = pJobItem;
			break;
		}
	}

	if (pExceptJobItem != NULL)
	{
		for (POSITION pos=GetHeadPosition(); pos!=NULL; )
		{
			pJobItem = (CJobItem*)GetNext(pos);
			if(pJobItem == NULL)
				continue;

			DEBUGER_ASSERT_VALID(pJobItem != NULL);

			if ((pJobItem->GetJobPattern() != enJobPatternRet) && 
				(pJobItem->GetJobPattern() != enJobPatternPR) && 
				(pJobItem->GetJobPattern() != enJobPatternW2W))
				continue;

			if (pJobItem->m_nJobStatus != enJobStatusScRequest)
				continue;

			if (CLib::GetStackerNum(pJobItem->m_nStartWH, pJobItem->m_strStartLoc) != nScNum)
				continue;

			if ( nExceptLuggNum != 0 && pJobItem->m_nLuggNum == nExceptLuggNum )
				continue;

			nBank2  = CLib::GetBank(pJobItem->m_strStartLoc);
			nBay2   = CLib::GetBay(pJobItem->m_strStartLoc);
			nLevel2 = CLib::GetLevel(pJobItem->m_strStartLoc);

			/*
			switch(nBank2)
			{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:		
				if (nBay2 == 24)		
					continue;
				break;
			case 13:
			case 14:		
				if (nBay2 == 20)		
					continue;
				break;
			default:
				continue;
			}
			*/

			// 크래인이 이동하지 않아도 되는가?
			if ((nBay1 == nBay2 - 1) && (nLevel1 == nLevel2))
			{
				// 한번에 포킹이가능한가?
				if (nBank1 == nBank2)
				{
					pJobItem1 = pJobItem;
				}
				else
				{
					pJobItem2 = pJobItem;
				}
			}
			else
			{
				pJobItem3 = pJobItem;
			}
		}
	}

	if (pJobItem1 != NULL)
		return pJobItem1;
	else if (pJobItem2 != NULL)
		return pJobItem2;
	else 
		return pJobItem3;
}
