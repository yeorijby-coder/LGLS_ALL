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
#define		COPY_BUFFER_SIZE		99999
void CJob::FileCopy(CString strSourceFileName, CString strTargetFileName)
{
	CString strLog;
	UINT unReadByte;
	UINT64 unFileSize, unTotalCopyByte=0;
	LPBYTE pucSourceFileBuffer;
	LPBYTE pucTargetFileBuffer;
	CFile ReadFile;
	CFile WriteFile;
	pucSourceFileBuffer = new BYTE[COPY_BUFFER_SIZE];
	pucTargetFileBuffer = new BYTE[COPY_BUFFER_SIZE];

	if (!ReadFile.Open(strSourceFileName, CFile::modeRead | CFile::shareDenyNone))
	{
		strLog.Format(_T(" Source File을 열 수 없습니다.[%s]"), strSourceFileName);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::FileCopy"));
		delete pucSourceFileBuffer;
		delete pucTargetFileBuffer;
		return;
	}

	if (!WriteFile.Open(strTargetFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone))
	{
		strLog.Format(_T(" Target File을 열 수 없습니다.[%s]"), strTargetFileName);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::FileCopy"));
		delete pucSourceFileBuffer;
		delete pucTargetFileBuffer;
		return;
	}

	unFileSize = ReadFile.GetLength();
	while(unFileSize > unTotalCopyByte)
	{
		unReadByte = ReadFile.Read(pucSourceFileBuffer, COPY_BUFFER_SIZE);
		if(unReadByte > 0)
		{
			WriteFile.Write(pucSourceFileBuffer, unReadByte);
		}
		unTotalCopyByte += unReadByte;
	}

	if(ReadFile.m_hFile != CFile::hFileNull)
	{
		ReadFile.Close();
	}

	if(WriteFile.m_hFile != CFile::hFileNull)
	{
		WriteFile.Close();
	}

	if(m_pDoc->m_bLog == TRUE)
	{
		strLog.Format(_T("Copy 했습니다.! [Source File=%s][Target File=%s]"), strSourceFileName, strTargetFileName);
		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_SYSTEM, strLog, _T("CJob::FileCopy"));
	}

	delete pucSourceFileBuffer;
	delete pucTargetFileBuffer;
}

BOOL CJob::Backup()
{
	m_csSyncBackup.Lock();

	CString strLog;
	CString strJobFile, strBackupFile, strExternalBackupFile, strExternalJobFile;
	strJobFile.Format(_T("%s\\JOB.ECS"), m_pDoc->m_pConfig->m_strJobPath);
	strBackupFile.Format(_T("%s\\JOB.BKP"), m_pDoc->m_pConfig->m_strJobPath);
	strExternalJobFile = _T("E:\\JOB.ECS");
	strExternalBackupFile = _T("E:\\JOB.BKP");

	// Backup 파일이 있는지 확인할 것!
	TRY
	{
		// 있으면 지움 
		if (::GetFileAttributes(strBackupFile) != 0xFFFFFFFF) 
			CFile::Remove(strBackupFile);

//		CFile::Rename(strJobFile, strBackupFile);

	}
	CATCH (CException, e)
	{
		DEBUGER_TRACE(CLib::GetExceptionString(e), _T("CJob::Backup"));
		strLog.Format(_T("정상 작업정보 백업(Backup 파일 삭제) 실패! [%s]"), CLib::GetExceptionString(e));		//
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Backup"));						//
	}
	END_CATCH

	// Backup 파일이 있는지 확인할 것!
	TRY
	{
		// 있으면 지움 
//		if (::GetFileAttributes(strBackupFile) != 0xFFFFFFFF) 
//			CFile::Remove(strBackupFile);
		
		CFile::Rename(strJobFile, strBackupFile);
		
	}
	CATCH (CException, e)
	{
		DEBUGER_TRACE(CLib::GetExceptionString(e), _T("CJob::Backup"));
		strLog.Format(_T("정상 작업정보 백업(파일명 변경) 실패! [%s]"), CLib::GetExceptionString(e));		//
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Backup"));						//
	}
	END_CATCH


	//// 메모리 스틱에 Backup 파일이 있는지 확인할 것!
	//if (m_pDoc->m_pConfig->m_bNotBackupNetwork == FALSE)
	//{
	//	TRY
	//	{
	//		// 있으면 지움 
	//		if (::GetFileAttributes(strExternalBackupFile) != 0xFFFFFFFF) 
	//			CFile::Remove(strExternalBackupFile);

	////		CFile::Rename(strExternalJobFile, strExternalBackupFile);

	//	}
	//	CATCH (CException, e)
	//	{
	//		DEBUGER_TRACE(CLib::GetExceptionString(e), _T("CJob::Backup"));
	//		strLog.Format(_T("원격지 작업정보 백업(Backup 파일 삭제) 실패! [%s]"), CLib::GetExceptionString(e));		//
	//		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Backup"));						//
	//	}
	//	END_CATCH

	//	// 메모리 스틱에 Backup 파일이 있는지 확인할 것!
	//	TRY
	//	{
	//		// 있으면 지움 
	////		if (::GetFileAttributes(strExternalBackupFile) != 0xFFFFFFFF) 
	////			CFile::Remove(strExternalBackupFile);
	//		
	//		CFile::Rename(strExternalJobFile, strExternalBackupFile);
	//		
	//	}
	//	CATCH (CException, e)
	//	{
	//		DEBUGER_TRACE(CLib::GetExceptionString(e), _T("CJob::Backup"));
	//		strLog.Format(_T("원격지 작업정보 백업(파일명 변경) 실패! [%s]"), CLib::GetExceptionString(e));		//
	//		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Backup"));						//
	//	}
	//	END_CATCH
	//}

	// 파일 Serialize
	TRY
	{
		CFile f(strJobFile, CFile::modeCreate | CFile::modeWrite);

		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();

		f.Close();

		if (m_pDoc->m_pConfig->m_bNotBackupNetwork == FALSE)
		{
			// 있으면 
			if (::GetFileAttributes(strExternalJobFile) != 0xFFFFFFFF) 
			{
				CFile::Remove(strExternalJobFile);
				FileCopy(strJobFile, strExternalJobFile);
			}
			// 없으면 
			else // if (::GetFileAttributes(strExternalJobFile) == 0xFFFFFFFF) 
			{
				FileCopy(strJobFile, strExternalJobFile);
				
//				if(m_pDoc->m_bLog == TRUE)
//				{
//					strLog.Format(_T("정상적으로 백업했습니다.!(Rename) [Job File=%s][External File=%s]"), strBackupFile, strExternalJobFile);
//					m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_SYSTEM, strLog, _T("CJob::FileCopy"));
//				}
			}
		}

//		if(m_pDoc->m_bLog == TRUE)
//		{
//			strLog.Format(_T("정상적으로 백업했습니다.! [Job File=%s][External File=%s]"), strJobFile, strExternalJobFile);
//			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_SYSTEM, strLog, _T("CJob::FileCopy"));
//		}

	}
	CATCH (CException, e)
	{
		CString strLog;
		strLog.Format(_T("작업정보 백업 실패! [%s]"), CLib::GetExceptionString(e));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Backup"));
		m_csSyncBackup.Unlock();
		return FALSE;
	}
	END_CATCH

	m_csSyncBackup.Unlock();
	
	return TRUE;
}
/*
BOOL CJob::Restore()
{
	CString strFileName;
	strFileName.Format(_T("%s\\JOB.ECS"), m_pDoc->m_pConfig->m_strJobPath);

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
		strLog.Format(_T("작업정보 복원 실패! [%s]"), CLib::GetExceptionString(e));
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

BOOL CJob::BackupRestore()
{
	CString strOldFileName, strNewFileName;
	strOldFileName.Format(_T("%s\\JOB.BKP"), m_pDoc->m_pConfig->m_strJobPath);
	strNewFileName.Format(_T("%s\\JOB.ECS"), m_pDoc->m_pConfig->m_strJobPath);

//	CFile::Rename( _T("OldName"), _T("NewName") );
	CFile::Rename(strOldFileName, strNewFileName );

	TRY
	{
		CFile::Remove(strNewFileName);
//		::MoveFile(strOldFileName, strNewFileName )
		CFile::Rename(strOldFileName, strNewFileName );
	}
	CATCH(CFileException, e)
	{
		CString strLog;
		strLog.Format(_T("작업정보 복원 실패! [%s]"), CLib::GetExceptionString(e));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::BackupRestore"));
		return FALSE;
	}
	END_CATCH
 
	return Restore();
}
*/

BOOL CJob::Restore(int nMode)
{
	CString strJobFile, strBackupFile;
	CString strJobFile2, strBackupFile2;
	strJobFile.Format(_T("%s\\JOB.ECS"),		m_pDoc->m_pConfig->m_strJobPath);
	strBackupFile.Format(_T("%s\\JOB.BKP"),		m_pDoc->m_pConfig->m_strJobPath);
	strJobFile2.Format(_T("%s\\JOB2.ECS"),		m_pDoc->m_pConfig->m_strJobPath);
	strBackupFile2.Format(_T("%s\\JOB2.BKP"),	m_pDoc->m_pConfig->m_strJobPath);

	TRY
	{
		if(nMode == 1)
		{
			if ((::GetFileAttributes(strJobFile) != 0xFFFFFFFF) && 
				(::GetFileAttributes(strBackupFile) != 0xFFFFFFFF))
			{
				if(::GetFileAttributes(strJobFile2) != 0xFFFFFFFF)
				{
					CFile::Remove(strJobFile2);
				}

				CFile::Rename(strJobFile, strJobFile2);
			}

			if (::GetFileAttributes(strBackupFile) != 0xFFFFFFFF)
			{
				CopyFile(strBackupFile, strBackupFile2, TRUE);
				CFile::Rename(strBackupFile, strJobFile);
			}
		}

		CFile f(strJobFile, CFile::modeRead);

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

			if (pJobItem != NULL)
				pJobItem->m_pDoc = m_pDoc;
		}

		f.Close();
	}
	CATCH (CArchiveException, e)
	{
		CString strLog;
		strLog.Format(_T("작업정보 복원 실패! [%s]"), 
			CLib::GetExceptionString(e));

		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Restore"));
		return FALSE;
	}
	CATCH (CFileException, e)
	{
		DEBUGER_TRACE(CLib::GetExceptionString(e), _T("CJob::Restore"));
		return FALSE;
	}
	END_CATCH

	return TRUE;
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

	m_csSyncRemove.Lock();

	POSITION pos = CObList::Find(pJobItem);
	if (pos == NULL)
	{
		m_csSyncRemove.Unlock();
		return FALSE;
	}

	pJobItem->MulticastRemoveJob();

	CObList::RemoveAt(pos);
	delete pJobItem;
	Backup();

	m_csSyncRemove.Unlock();

	return TRUE;	
}

void CJob::CheckIdleJob()
{
	m_csSyncRemoveIdleJob.Lock();

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if (pJobItem->IsNewTask())
			continue;

		switch (pJobItem->m_nJobStatus)
		{
		case enJobStatusComplete:
		case enJobStatusCompleteRequest:
//		case enJobStatusArrived:
		case enJobStatusResend:
		case enJobStatusScResend:
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

				if ((m_pDoc->m_pHostCl == NULL) || (m_pDoc->m_pHostCl->IsConnect() == FALSE))
					continue;

				switch (pJobItem->m_nJobStatus)
				{
//				case enJobStatusCancel:				m_pDoc->m_pHostCl->Cancel(pJobItem); break;
//				case enJobStatusArrived:			m_pDoc->m_pHostCl->Arrive(pJobItem->m_nLuggNum); break;
				case enJobStatusCompleteRequest:
				case enJobStatusComplete:			m_pDoc->m_pHostCl->Complete(pJobItem->m_nLuggNum, CScInfo::enStore); break;
				case enJobStatusErrorEmptyRetrieve:	m_pDoc->m_pHostCl->ErrorScReport(pJobItem, enErrorEmptyRetrieve); break;
				case enJobStatusErrorDualStore:		if (!pJobItem->m_bRecvAckDualSto) m_pDoc->m_pHostCl->ErrorScReport(pJobItem, enErrorDualStore); break;
				}
			}
			break;

		default:
			{
				if (m_pDoc->IsIdleJob(pJobItem->m_nLuggNum) == FALSE)
					continue;

				CTimeSpan tElapseTime = CTime::GetCurrentTime() - pJobItem->m_tTime;
				if (tElapseTime.GetTotalMinutes() < 5)
					continue;

				if ((m_pDoc->m_pHostCl == NULL) || (m_pDoc->m_pHostCl->IsConnect() == FALSE))
					continue;

				CString strLog;
				strLog.Format(_T("이상작업 발생! 확인요망! [%s]"), pJobItem->GetLogString());
				m_pDoc->Alarm(LOG_POS_SYSTEM, strLog, pJobItem);
			}
			break;
		}

	}

	m_csSyncRemoveIdleJob.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CJob::Invoke(int nLuggNum)
{
	CJobItem* pJobItem = NULL;

	if (nLuggNum == 0)
	{
		pJobItem = FetchByStatus(enJobStatusNew);
		if (pJobItem == NULL)
			return FALSE;
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
			Backup();

			strLog.Format(_T("CV 구동요구 [작업구분=%s] [출발지=%s] [도착지=%s]"), pJobItem->GetJobTypeString(), pJobItem->GetStartStationString(), pJobItem->GetDestStationString());
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Invoke"), pJobItem);
		}
		return TRUE;

	case enJobPatternRet:
	case enJobPatternR2R:
	case enJobPatternW2W:
	case enJobPatternPR:
		{
			pJobItem->m_nJobStatus = enJobStatusScRequest;
			pJobItem->MulticastAddJob();
			Backup();

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

	if (CObList::Find(pJobItem) == NULL)
		return FALSE;

	CString strLog;
	if (pJobItem->GetJobPattern() == enJobPatternMove)
	{
		strLog.Format(_T("작업완료 작업구분 이상! [%s]"), pJobItem->GetJobTypeString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);
		return FALSE;
	}

	strLog.Format(_T("작업완료 [%s]"), pJobItem->GetLogString());
	m_pDoc->WriteLog(bManual ? LOG_TYPE_EVENT : LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);

	if (pJobItem->IsOfflineJobType())
	{
		/*
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
						strLog.Format("%d TEST 입고 작업번호 생성 실패!", nLuggNum);
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
//			//			strLog.Format(_T("동일한 작업이 존재... IMS 확인 [%d]"), oJobItem.m_nLuggNum);
//			//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CJob::Complete"), &oJobItem);
//			//			Answer(enHostCommandOrder, oJobItem.m_nLuggNum, oJobItem.m_nJobType, enHostErrorAlreadyJob);
//					}

					if ((pJobItem = m_pDoc->m_pJob->Add(oJobItem)) == NULL)
					{
						strLog.Format("%s TEST 입고 작업생성 실패!", oJobItem.GetJobTypeString());
						m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);
						m_pDoc->m_bTesting = FALSE;
						return TRUE;
					}

					strLog.Format(_T("%s TEST 입고 작업생성"), oJobItem.GetJobTypeString());
					m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);

//					if (m_pDoc->m_pJob->Invoke(pJobItem) == FALSE)
//					{
//						strLog.Format("%s 작업지시 실패!", oJobItem.GetJobTypeString());
//						m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_SYSTEM, strLog, _T("CJob::Complete"), pJobItem);
//						return;
//					}
				}
			}
		}
		//*/
		return TRUE;
	}

	if (ucScJobType == CScInfo::enStore)
		pJobItem->SetJobStatus(enJobStatusComplete);

	if (m_pDoc->m_pHostCl && m_pDoc->m_pHostCl->IsConnect())
		m_pDoc->m_pHostCl->Complete(pJobItem->m_nLuggNum);

	return TRUE;
}

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
		strLog.Format(_T("도착완료 작업구분 이상! [%s]"), pJobItem->GetJobTypeString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, _T("CJob::Arrive"), pJobItem);
		return FALSE;
	}

	strLog.Format(_T("도착완료 [%s]"), pJobItem->GetLogString());
	m_pDoc->WriteLog(bManual ? LOG_TYPE_EVENT : LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Arrive"), pJobItem);

	if (pJobItem->IsOfflineJobType())
	{
		Remove(pJobItem);
		return TRUE;
	}

//	pJobItem->SetJobStatus(enJobStatusArrived);

//Arrived(int nLuggNum, int nStation, BOOL bManual)

	if (m_pDoc->m_pHostCl && m_pDoc->m_pHostCl->IsConnect())
	{
//		int nDestStation = _ttoi(pJobItem->m_strDestStn);
//		if (nDestStation > 200 && nDestStation <= 208)
//			m_pDoc->m_pHostCl->Arrived(pJobItem->m_nLuggNum, nDestStation);

		m_pDoc->m_pHostCl->Complete(pJobItem->m_nLuggNum);
	}

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

//	if (m_pDoc->m_pHostCl && m_pDoc->m_pHostCl->IsConnect())
//		m_pDoc->m_pHostCl->CancelReport(pJobItem, bManual);

	return TRUE;
}

BOOL CJob::Request(CString& strStationID, CString& strBarcode, BOOL bManual /* = FALSE */)
{
	CString strLog;
	strLog.Format(_T("작업요청(BCR 보고) [입고대=%s, BARCODE=%s]"), m_pDoc->GetStationName(strStationID), strBarcode);
	m_pDoc->WriteLog(bManual ? LOG_TYPE_EVENT : LOG_TYPE_JOB, LOG_POS_SYSTEM, strLog, _T("CJob::Request"));

	if (m_pDoc->m_pHostCl && m_pDoc->m_pHostCl->IsConnect())
		m_pDoc->m_pHostCl->BarcodeData(strStationID, strBarcode, bManual);

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

		if (pJobItem->m_nLuggNum == nLuggNum)
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

		if (pJobItem->m_nJobStatus == nJobStatus)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchByStatusNRgvNum(int nJobStatus, int nRgvNum)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if (pJobItem->m_nRgvcNum != nRgvNum)
			continue;

		if (pJobItem->m_nJobStatus == nJobStatus)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchOnTheRetHomeStandJob(int nTrackNum)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos = GetHeadPosition(); pos != NULL;)
	{
		pJobItem = (CJobItem *)GetNext(pos);

		if ( pJobItem->m_nJobStatus != enJobStatusCvNew )
			continue;

		if ( pJobItem->GetJobPattern() != enJobPatternRet )
			continue;

		if ( pJobItem->m_nArriveTrackNum == nTrackNum )
			return pJobItem;
	}

	return NULL;
}

BOOL CJob::CountRetrieveJobBySc(int nScNum)
{
	int SCS_PER_SC = 3;
	int nSCsNum = 1;
	if (nScNum > SCS_PER_SC)
		nSCsNum = 2;

	CEquipment* pEquipment = m_pDoc->GetEquipment(CEquipment::enSC, nSCsNum);
	if (pEquipment == NULL)
		return FALSE;

	CScDongJin* pSc = (CScDongJin*)pEquipment;
	if (pSc == NULL)
		return FALSE;

	CScInfo* pInfo;

	pInfo = (CScInfo *)pSc->GetInfo(nScNum, FALSE);
	if (pInfo == NULL)
		return FALSE;

	//한 개의 작업만 실행하고 대기
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	int nCount = 0;
	CJobItem* pJobItem = NULL;
	
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		//작업이 출고 피킹출고가 아니면 
		if ((pJobItem->GetJobPattern() != enJobPatternRet) && 
			(pJobItem->GetJobPattern() != enJobPatternPR) && 
			(pJobItem->GetJobPattern() != enJobPatternW2W))
			continue;

		if (pJobItem->m_nJobStatus != enJobStatusScRequest && 
			pJobItem->m_nJobStatus != enJobStatusScResend )
			continue;

		if (CLib::GetStackerNum(pJobItem->m_nStartWH, pJobItem->m_strStartLoc) == nScNum)
		{
			CTrackHS* pRetHS = NULL;

			pRetHS = pInfo->m_pRetsHS.FindByPos(2);				// 후면부의 SC HS Pos가 2임!
			if ((pRetHS != NULL) && (pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE))
			{
				++m_pDoc->m_nScRetRearRemainJobCnt[nScNum-1];
//				m_pDoc->m_bScRetRearCountChanged[nScNum-1] = TRUE;
			}



			pRetHS = pInfo->m_pRetsHS.FindByPos(4);				// 전면부의 SC HS Pos가 4임!
			if ((pRetHS != NULL) && (pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE))
			{
				++m_pDoc->m_nScRetFrontRemainJobCnt[nScNum-1];
//				m_pDoc->m_bScRetFrontCountChanged[nScNum-1] = TRUE;
			}
		}
	}

	return TRUE;
}
/*
CJobItem* CJob::FetchOnTheRetHomeStandJob(int nScNum, int nRank)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos = GetHeadPosition(); pos != NULL;)
	{
		pJobItem = (CJobItem *)GetNext(pos);

		if ( pJobItem->m_nJobStatus != JOB_STA_CV_OPER_REQUEST )
			continue;

		if ( pJobItem->GetStackerNum() != nScNum )
			continue;

		if ( pJobItem->GetRank() == nRank )
			return pJobItem;
	}

	return NULL;
}
*/
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

		if ((pJobItem->m_nJobStatus == enJobStatusDualStoreRetry) && 
			(CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc) == nScNum))
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchEmptyRetrieveJobBySc(int nScNum)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if ((pJobItem->m_nJobStatus == enJobStatusEmptyRetrieveRetry) && 
			(CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc) == nScNum))
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchStoreJobByCv(CString& strStoStationID)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strStoStationID), strStoStationID);

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if ((pJobItem->GetJobPattern() != enJobPatternSto) &&
			(pJobItem->GetJobPattern() != enJobPatternMove))
			continue;

		if ((pJobItem->m_nJobStatus == enJobStatusCvNew || pJobItem->m_nJobStatus == enJobStatusResend) && 
			(pJobItem->m_strStartStn == strStoStationID))
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchRetrieveJobBySc(int nScNum, CString& strDestStationID, BOOL bDouble)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strDestStationID), strDestStationID);

	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if ((pJobItem->GetJobPattern() != enJobPatternRet) && 
			(pJobItem->GetJobPattern() != enJobPatternPR) && 
			(pJobItem->GetJobPattern() != enJobPatternW2W))
			continue;

		if (pJobItem->m_nJobStatus != enJobStatusScRequest && 
			pJobItem->m_nJobStatus != enJobStatusScResend )
			continue;

		if (pJobItem->m_strDestStn != strDestStationID)
			continue;

		if (CLib::GetStackerNum(pJobItem->m_nStartWH, pJobItem->m_strStartLoc, bDouble) == nScNum)
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchRackToRackJobBySc(int nScNum, BOOL bDouble)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if ((pJobItem->GetJobPattern() == enJobPatternR2R) &&
			(pJobItem->m_nJobStatus == enJobStatusScRequest || pJobItem->m_nJobStatus == enJobStatusScResend) && 
			(CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc, bDouble) == nScNum) &&
			(CLib::GetStackerNum(pJobItem->m_nStartWH, pJobItem->m_strStartLoc,bDouble) == nScNum))
			return pJobItem;
	}

	return NULL;
}

CJobItem* CJob::FetchJobByPair(int nPattern, int nJobStatus, int nPairLuggNo)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if ((pJobItem->GetJobPattern() == nPattern) &&
			(pJobItem->m_nJobStatus == nJobStatus) && 
			(pJobItem->m_nLuggNum == nPairLuggNo) )
			return pJobItem;
	}

	return NULL;
}
/*
CJobItem* CJob::FetchCompleteJobCv(int nArriveTrackNum)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if ((pJobItem->m_nJobStatus == enJobStatusCvInvoke || pJobItem->m_nJobStatus == enJobStatusResend) && 
			(pJobItem->m_nArriveTrackNum == nArriveTrackNum))
			return pJobItem;
	}

	return NULL;
}
//*/

CJobItem* CJob::FetchCompleteJob(int nJobStatus, int nArriveTrackNum)
{
	CSingleLock SingleLock(&m_csSyncJobList);
	SingleLock.Lock();

	CJobItem* pJobItem = NULL;
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if ((pJobItem->m_nJobStatus == nJobStatus || pJobItem->m_nJobStatus == enJobStatusResend) && 
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

		if (pJobItem->IsOfflineLuggNum())
		{
			nLuggNum = pJobItem->m_nLuggNum;
			break;
		}
	}

	for (int i=0; i<900; ++i)
	{
		nLuggNum = (nLuggNum > 9900) ? 9001 : nLuggNum + 1;
		pJobItem = Find(nLuggNum);

		if ((pJobItem == NULL) &&
			(m_pDoc->m_pEquipments.IsIdleJob(nLuggNum) == TRUE))
			return nLuggNum;
	}

	return 0;
}

int CJob::GenerateOnlineLuggNum()
{
	int nLuggNum = 1000;
	CJobItem* pJobItem = NULL;

	for (POSITION pos=GetTailPosition(); pos!=NULL; )
	{
		pJobItem = (CJobItem*)GetPrev(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if (pJobItem->IsOnlineLuggNum())
		{
			nLuggNum = pJobItem->m_nLuggNum;
			break;
		}
	}

	for (int i=0; i<9000; ++i)
	{
		nLuggNum = (nLuggNum > 8999) ? 1 : nLuggNum + 1;
		pJobItem = Find(nLuggNum);

		if ((pJobItem == NULL) &&
			(m_pDoc->m_pEquipments.IsIdleJob(nLuggNum) == TRUE))
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

	CString strRetLimitCount;
	if (m_pDoc->m_mapRetLimitInfos.Lookup(strDestStn, strRetLimitCount) == FALSE)
		return TRUE;

	CStationInfo* pStation = m_pDoc->GetStationInfo(strDestStn);
	DEBUGER_ASSERT_VALID(pStation != NULL);
	DEBUGER_ASSERT_VALID(pStation->m_pTrack != NULL);
	DEBUGER_ASSERT_VALID((pStation->m_enKind == CStationInfo::enRetStation)||(pStation->m_enKind == CStationInfo::enArvStation));

	int nRetLimitCount=0;
	if ((pStation == NULL) ||
		(pStation->m_pTrack->IsRetStationReady() == TRUE) ||
		(pStation->m_pTrack->IsProductSensing() == TRUE) ||
		(pStation->m_pTrack->m_nLuggNum != 0))
	{
		nRetLimitCount = _ttoi(strRetLimitCount);
		nRetLimitCount++;
		strRetLimitCount.Format(_T("%d"), nRetLimitCount);
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
			if (!mapJobCount.Lookup(pJobItem->m_strDestStn, strJobCount)) 
				mapJobCount.SetAt(pJobItem->m_strDestStn, _T("1"));
			else 
				mapJobCount.SetAt(pJobItem->m_strDestStn, CConvert::ToString(_ttoi(strJobCount)+1));
		}
	}

	if ((mapJobCount.Lookup(strDestStn, strJobCount) == TRUE) && (_ttoi(strJobCount) >= _ttoi(strRetLimitCount)))
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
/*
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
		strTxBuff.Format("%c<ECS><JOB>%s</JOB></ECS>%c", STX, strTxData, ETX);
		m_pDoc->m_pLogPlayback->Log(strTxBuff);
	}
}
*/