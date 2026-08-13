// Job.h: interface for the CJob class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOB_H__8C418FCE_6374_4605_9319_10979A1F64B1__INCLUDED_)
#define AFX_JOB_H__8C418FCE_6374_4605_9319_10979A1F64B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JobItem.h"
#include "TrackHS.h"

class CEcsDoc;
class CMonitorServer;
class CJob : public CObList
{
public:
	CJob(CEcsDoc* pDoc);
	virtual ~CJob();

public:
	CEcsDoc* m_pDoc;

protected:
	CCriticalSection m_csSyncBackup;
	CCriticalSection m_csSyncRemove;
	CCriticalSection m_csSyncRemoveIdleJob;
	CCriticalSection m_csSyncJobList;

public:
	BOOL Backup();
	BOOL Restore(int nMode);
//	BOOL BackupRestore();

public:
//	void WritePlayBack();

public:
	CJobItem* Add(CJobItem& rJobItem);
	BOOL Remove(CJobItem* pJobItem);
	BOOL Remove(int nLuggNum) { return Remove(Find(nLuggNum)); }
	void CheckIdleJob();
	void FileCopy(CString strSourceFileName, CString strTargetFileName);

public:
	BOOL Invoke(int nLuggNum);
	BOOL Invoke(CJobItem* pJobItem);
	BOOL Complete(CJobItem* pJobItem, BYTE ucScJobType, BOOL bManual = FALSE);
	BOOL Arrive(CJobItem* pJobItem, BOOL bManual = FALSE);
	BOOL Cancel(CJobItem* pJobItem, BOOL bManual = FALSE);
	BOOL Request(CString& strStationID, CString& strBarcode, BOOL bManual = FALSE);
	BOOL IsValidScRetrieveTask(CString& strDestStn);

public:
	CJobItem* Find(int nLuggNum);
	CJobItem* IsEqualAll(CJobItem* rJobItem);				//
//	CJobItem* FetchOnTheRetHomeStandJob(int nScNum, int nRank);
	CJobItem* FetchOnTheRetHomeStandJob(int nTrackNum);
	CJobItem* FetchByTestJob(int nJobPattern);				//
	CJobItem* FetchByTestRetJob(CString strLocation);		//
	CJobItem* FetchByTestStoJob(CString strLocation);		//
	CJobItem* FetchByStatus(int nJobStatus);
	CJobItem* FetchByStatusNRgvNum(int nJobStatus, int nRgvNum);
	CJobItem* FetchByRetDestination(CString& strDestStationID);
	CJobItem* FetchDualStoreJobBySc(int nScNum);
	CJobItem* FetchEmptyRetrieveJobBySc(int nScNum);		//
	CJobItem* FetchStoreJobByCv(CString& strStoStationID);
	CJobItem* FetchRetrieveJobBySc(int nScNum, CString& strDestStationID, BOOL bDouble = FALSE);
	CJobItem* FetchRackToRackJobBySc(int nScNum, BOOL bDouble = FALSE);
	CJobItem* FetchJobByPair(int nPattern, int nJobStatus, int nPairLuggNo);		//
	CJobItem* FetchScStoJobByPair(int nPairLuggNo) { return FetchJobByPair(enJobPatternSto, enJobStatusCvInvoke, nPairLuggNo); }		//
	CJobItem* FetchScRetJobByPair(int nPairLuggNo) { return FetchJobByPair(enJobPatternRet, enJobStatusScRequest, nPairLuggNo); }		//
	CJobItem* FetchScR2RJobByPair(int nPairLuggNo) { return FetchJobByPair(enJobPatternR2R, enJobStatusScRequest, nPairLuggNo); }		//
	CJobItem* FetchScH2HJobByPair(int nPairLuggNo) { return FetchJobByPair(enJobPatternMove, enJobStatusCvInvoke, nPairLuggNo); }		//
	CJobItem* FetchScW2WJobByPair(int nPairLuggNo) { return FetchJobByPair(enJobPatternW2W, enJobStatusCvInvoke, nPairLuggNo); }		//
	CJobItem* FetchCompleteJob(int nJobStatus, int nArriveTrackNum);
	CJobItem* FetchScCompleteJob(int nArriveTrackNum) { return FetchCompleteJob(enJobStatusScComplete, nArriveTrackNum); }
	CJobItem* FetchRtvCompleteJob(int nArriveTrackNum) { return FetchCompleteJob(enJobStatusRtvComplete, nArriveTrackNum); }
	CJobItem* FetchLifterCompleteJob(int nArriveTrackNum) { return FetchCompleteJob(enJobStatusLifterComplete, nArriveTrackNum); }
	CJobItem* FetchLgvCompleteJob(int nArriveTrackNum) { return FetchCompleteJob(enJobStatusLgvComplete, nArriveTrackNum); }
	CJobItem* FetchLgvSemiMoveJob(CString& strStartStn, CString& strDestStn);
	CJobItem* FetchReservedJobByStartStation(CString& strStationID);
//	CJobItem* FetchCompleteJobCv(int nJobStatus, int nArriveTrackNum);
//	CJobItem* FetchRtvCompleteJobCv(int nArriveTrackNum) { return FetchCompleteJobCv(enJobStatusRtvComplete, nArriveTrackNum); }
	int GenerateOfflineLuggNum();
	int GenerateOnlineLuggNum();
	BOOL CountRetrieveJobBySc(int nScNum);

public:
	void MulticastAllJobPerClient(CMonitorServer* pMonitorSv);
};

#endif // !defined(AFX_JOB_H__8C418FCE_6374_4605_9319_10979A1F64B1__INCLUDED_)
