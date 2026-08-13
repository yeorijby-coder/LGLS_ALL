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
	enum EN_CRATE_KIND { enCrateUndefine, enCrateBIDP, enCrateIDP };

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
	BYTE	m_ucCrateKind;		// CRATE KIND {BIDP=1, IDP=2}
	BYTE	m_ucCrateStatus;	// CRATE STATUS {없음=0, 사용가능=1, 세정대상=2, 보수대상=3}
	int		m_nProdCount;		// 제품수량
	CString m_strBarcode;		// IDP, BIDP, PRODUCT
	CString m_strProductID;		// 품목코드
	CString m_strPalletNo;		// PalletNo
	CString m_strCompany;		// 거래처 정보
	BOOL	m_bBcrRead;			// BCR 읽기유무 
	int		m_nPriority;		// 우선순위
	int		m_nSeqNo;			// Seq. No
	int		m_nSize;			// Tray Type
	int		m_nCvPlcNum;
public:
	int		m_nJobStatus;
	int		m_nResultCode;
	int		m_nDepartTrackNum;
	int		m_nArriveTrackNum;
	BOOL	m_bRecvAckDualSto;
	COleDateTime	m_tTime;
	CString m_strAgingStn;		// Aging Station 구분용

	int		m_nPairLuggNo;
public:
	BOOL IsNewTask();
	BOOL IsEqual(CJobItem* pJobItem);
	void SetJobStatus(int nJobStatus);
	void SetResultCode(int nResultCode);
	BOOL IsIDP() { return (m_ucCrateKind == enCrateIDP); }
	BOOL IsBIDP() { return (m_ucCrateKind == enCrateBIDP); }
	BOOL IsEmptyCrate() { return (m_ucCrateStatus != 0); }
	int  GetTrackDestination();
	CString GetAgingStation();
	
public:
	CString GetLogString();
	CString GetXmlString();
	void MulticastAddJob(CMonitorServer* pMonitorSv = NULL);
	void MulticastEditJob(CMonitorServer* pMonitorSv = NULL);
	void MulticastRemoveJob(CMonitorServer* pMonitorSv = NULL);
	
public:
	CString GetString();
	void SetString(CString& strJobItem);
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
	static int ConvertJobTypeToPattern(int nJobType);
	static int ConvertGenCode(CString& strGen, BYTE ucCrateKind);
	int	GetJobPattern() { return ConvertJobTypeToPattern(m_nJobType); }

public:
	static CString GetGenString(BYTE ucGenCode);
	static CString GetJobTypeString(int nJobType);
	static CString GetJobStatusString(int nJobStatus);
	static CString GetCrateKindString(BYTE ucCrateKind);
	static CString GetCrateStatusString(BYTE ucCrateStatus);
	static CString GetWarehouseString(int nWarehouse);
	static CString GetTrayTypeString(int nSize);

	CString GetGenString() { return GetGenString(m_ucGenCode); }
	CString GetJobTypeString() { return GetJobTypeString(m_nJobType); }
	CString GetJobStatusString() { return GetJobStatusString(m_nJobStatus); }
	CString GetCrateKindString() { return GetCrateKindString(m_ucCrateKind); }
	CString GetCrateStatusString() { return GetCrateStatusString(m_ucCrateStatus); }
	CString GetStartWarehouseString() { return GetWarehouseString(m_nStartWH); }
	CString GetDestWarehouseString() { return GetWarehouseString(m_nDestWH); }
	CString GetTrayTypeString() { return GetTrayTypeString(m_nSize); }
	CString GetStartStationString();
	CString GetDestStationString();
	CString GetDepartTrackString();
	CString GetArriveTrackString();

	CString GetDateTime();
};

#endif // !defined(AFX_JOBITEM_H__90A232C3_7A57_44B7_AD5F_9B95CF83A8A1__INCLUDED_)
