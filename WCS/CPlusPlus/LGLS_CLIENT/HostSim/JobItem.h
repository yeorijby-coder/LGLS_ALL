// JobItem.h: interface for the CJobItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOBITEM_H__90A232C3_7A57_44B7_AD5F_9B95CF83A8A1__INCLUDED_)
#define AFX_JOBITEM_H__90A232C3_7A57_44B7_AD5F_9B95CF83A8A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Lib.h"
#include "StationInfo.h"

class CEcsDoc;
class CMonitorServer;
class CJobItem : public CObject
{
	DECLARE_SERIAL(CJobItem)

public:
	CJobItem();
	CJobItem(CEcsDoc* pDoc);
	CJobItem(CEcsDoc* pDoc, const CJobItem& rJobItem);
	virtual ~CJobItem();

public:
	enum EN_ACTIVITY { 
		enActivityQueue,				// Queued job list
		enActivityVehicleAssigne,		// Moving to pick position
		enActivityPickComplete,			// Moving to drop position
		enActivityDropComplete,			// Transport completed
		enActivitySize
	};
//	enum EN_CRATE_KIND { enCrateUndefine, enCrateBIDP, enCrateIDP };

public:
	CEcsDoc* m_pDoc;

public:
	int		m_nLuggNum;
	int		m_nJobType;
	int		m_nStartWH;
	CString	m_strStartStn;
	CString	m_strStartLoc;
	int		m_nRouteWH;
	CString m_strRouteStn;
	CString m_strRouteLoc;
	int		m_nDestWH;
	CString m_strDestStn;
	CString	m_strDestLoc;
	BYTE	m_ucGenCode;		// ★삼성정밀화학에서는 Test용으로 사용함!!
	CString m_strBarcode;		// IDP, BIDP, PRODUCT
	CString m_strProductID;		// 품목코드
	CString m_strPalletNo;		// PalletNo
	CString m_strWeight;		// ★카길에서는 중량으로 사용함!

	int		m_nPriority;		// 우선순위
	int		m_nSeqNo;			// Seq. No
	int		m_nSize;			// 제품 크기

	int		m_nPairLuggNo;		// 짝이 되는 작업번호

public:
	int		m_nJobStatus;
	int		m_nResultCode;
	int		m_nDepartTrackNum;
	int		m_nArriveTrackNum;
	int		m_nTempPickPos;				//
	int		m_nTempDropPos;				//
	int		m_nTempDepartTrackNum;		//
	int		m_nTempArriveTrackNum;		//
	int		m_nActivity;				//
	int		m_nVehicleID;				//
	int		m_nRgvcNum;					//
	BOOL	m_bRecvAckDualSto;
	CTime	m_tTime;

public:
	BOOL	IsNewTask();
	BOOL	IsEqual(CJobItem* pJobItem);
	void	SetJobStatus(int nJobStatus);
	void	SetRgvcJobStatus(int nRgvJobStatus, int nRgvcNum, int nVehicleID);
	void	SetResultCode(int nResultCode);
	int		GetTrackDestination();

public:
	CString GetLogString();
	CString GetXmlString();
	void	MulticastAddJob(CMonitorServer* pMonitorSv = NULL);
	void	MulticastEditJob(CMonitorServer* pMonitorSv = NULL);
	void	MulticastRemoveJob(CMonitorServer* pMonitorSv = NULL);
	
public:
//	CString GetString();
//	void	SetString(CString& strJobItem);
	virtual void Serialize(CArchive& ar);

public:
	static BOOL IsOnlineLuggNum(int nLuggNum);
	BOOL		IsOnlineLuggNum() { return IsOnlineLuggNum(m_nLuggNum); }
	static BOOL IsOfflineLuggNum(int nLuggNum);
	BOOL		IsOfflineLuggNum() { return IsOfflineLuggNum(m_nLuggNum); }
	static BOOL IsOnlineJobType(int nJobType);
	BOOL		IsOnlineJobType() { return IsOnlineJobType(m_nJobType); }
	static BOOL IsOfflineJobType(int nJobType);
	BOOL		IsOfflineJobType() { return IsOfflineJobType(m_nJobType); }

public:
	enum EN_WAREHOUSE { enWarehouse1=1, enWarehouse2=2, enWarehouse3=3 };
	static BOOL IsValidWarehouse(int nWarehouse);
	BOOL IsValidStartWarehouse() { return IsValidWarehouse(m_nStartWH); }
	BOOL IsValidDestWarehouse() { return IsValidWarehouse(m_nDestWH); }
	BOOL IsValidStartLocation();
	BOOL IsValidDestLocation();
	BOOL IsValidStartStation();
	BOOL IsValidDestStation();

public:
	BOOL IsValidJobType();
	BOOL IsValidJobStatus();
	BOOL IsValidJobItem();

public:
	static int ConvertJobTypeToPattern(int nJobType);
//	static int ConvertGenCode(CString& strGen, BYTE ucCrateKind);
	int	GetJobPattern() { return ConvertJobTypeToPattern(m_nJobType); }

public:
	static CString GetGenString(BYTE ucGenCode);
	static CString GetJobTypeString(int nJobType);
	static CString GetJobStatusString(int nJobStatus);
	static CString GetWarehouseString(int nWarehouse);	
	static CString GetRgvStatusString(int nActivity);
	CString GetRgvStatusString() { return GetRgvStatusString(m_nActivity); }
	CString GetGenString() { return GetGenString(m_ucGenCode); }
	CString GetJobTypeString() { return GetJobTypeString(m_nJobType); }
	CString GetJobStatusString() { return GetJobStatusString(m_nJobStatus); }
	CString GetStartWarehouseString() { return GetWarehouseString(m_nStartWH); }
	CString GetDestWarehouseString() { return GetWarehouseString(m_nDestWH); }
	CString GetStartStationString();
	CString GetDestStationString();
	CString GetDepartTrackString();
	CString GetArriveTrackString();

	CString GetDateTime();
};

#endif // !defined(AFX_JOBITEM_H__90A232C3_7A57_44B7_AD5F_9B95CF83A8A1__INCLUDED_)
