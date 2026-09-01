
#pragma once

#include "EnCarrier.h"
#include "AdoDB.h"

class CEcsDoc;

class CURMDBAccess : public CObject
{
protected:
	DECLARE_DYNCREATE(CURMDBAccess)

public:
	CEcsDoc*	m_pDoc;
	CAdoDB*		m_pAdoDB;

public:
	CString		m_strDBErrMsg;

public:
	// CST Stat
	enum { CSTSTAT_NORMAL,
		   CSTSTAT_WAITIN,
		   CSTSTAT_TRANSFERRING,
		   CSTSTAT_COMPLETED,
		   CSTSTAT_ALTERNATE,
		   CSTSTAT_WAITOUT,
		   CSTSTAT_BLOCK,
		   CSTSTAT_RESERVED = 9};

	enum { MAKE_CODE = 1,
		   MAKE_NAME = 2,
		   MAKE_CODENAME = 3,
		   MAKE_NAMECODE = 4,
		   MAKE_CODEBACKNAME = 5,
		   MAKE_NAMEBACKCODE = 6};

public:
	CURMDBAccess();
	CURMDBAccess(CEcsDoc* pDoc, CAdoDB* pAdoDB);
	~CURMDBAccess();

public:
	int CURMDBAccess::SelectQryCnt(CString p_strSql);

public:
	BOOL FindCSTID(CString strCSTID, CString &strLoc, CString &strZoneName, CString &strSHIPID, CString &strFOSBID);
	BOOL FindJobInfo(CString strCSTID);
	BOOL TaskStatus(CString strTaskName);
	BOOL PortStatus(int nPortNo, CString strPortType, CString &strPortState, CString &strJisiGbn);
	BOOL ChangePortStatus(int nPortNo, CString strPortType);
	BOOL CheckScStatus(int nScNo, int nJobType);

	CString GetZoneNo(CString strZoneName);
	CString GetZoneName(CString strZoneNo);
	CString GetLocationZoneName(CString strLoc);
	CString GetCodeName(CString strCodeGroup, CString strCodeNo);
	CString GetWhName(CString strWhNo);
	int GetPortConfirm(CString strPortNo);
	int GetUserConfirm(CString strUserID);
	int GetPgmConfirm(CString strPgmID);
	int GetCodeGroupConfirm(CString strCodeGroup);
	int GetZoneConfirm(CString strZoneNo);
	int GetZoneUseConfirm(CString strZoneNo);
	BOOL GetOutJobOrderGroup(CString &strJobGroup, CString &strJobNo);
	BOOL NotFindJobNo(CString strJobNo);
	int GetInputJobCnt();

	void MakeComboWithZone(CComboBoxWrapper *comboBox, CString strZoneType, CString strAll, int nMakeType, CString strIndexType = _T("ASC"));
	void MakeComboWithCode(CComboBoxWrapper *comboBox, CString strCodeGroup, CString strAll, int nMakeType, CString strIndexType = _T("ASC"));
	void MakeComboWithPort(CComboBoxWrapper *comboBox, CString strPortGbn, CString strPortType, int nMakeType);
	void FindCombo(CComboBoxWrapper *comboBox, CString strCode, int nGetType, CString strDivisionChar);
	CString GetComboData(CString strComboText, int nGetType, CString strDivisionChar);
	CString EjectChar(CString strSourceText, CString strEjectChar);
	BOOL GetSysDate(CString &strSysDate, CString &strSysMilliSecond);

public:
	BOOL ConfirmJobInfo(CString strCSTID);
	BOOL ConfirmMoveLocMast(CString strFrLoc, CString strToLoc, CString &strFrZone, CString &strToZone);
	BOOL ConfirmVacantJobInfo(int &nVacantCnt, int &nWaitTrayCnt, int &nStockVacantCnt);
	BOOL MakeCommandID(CString strIOGbn, CString strCSTID, int nSeq, CString &strCommandID, CString &strCmdTime);
	BOOL UrmOfflineInOrder(CString strProdID, CString strPortNo, CString strCSTID, int &nRetCd, CString &strReturnMsg);
	BOOL UrmVacantInOrder(CString strProdID, CString strPortNo, int &nRetCd, CString &strReturnMsg);
	BOOL UrmOutputOrder(CString strWhNo		, CString strScNo		, CString strLoc		, CString strZone		,
						CString strPortNo	, CString strCSTID		, CString strCommandID	, CString strCmdTime	,
						CString strWMSJob	, CString strJobJisiDate, CString strHostSendGbn, CString strManualJobGbn,
						CString strUUser	, int &nRetCd			, CString &strReturnMsg	);
	BOOL UrmVacantOutOrder(CString strWhNo	, CString strScNo	, CString strLoc		, CString strZone		,
						   CString strPortNo, CString strWMSJob	, CString strHostSendGbn, CString strManualJobGbn,
						   CString strUUser	, int &nRetCd		, CString &strReturnMsg	);
	BOOL UrmShelfToShelfOrder(CString strWhNo		, CString strScNo	, CString strFrLoc			, CString strToLoc	,
							  CString strFrZone		, CString strToZone	, CString strManualJobGbn	, CString strUUser	,
							  int &nRetCd			, CString &strReturnMsg	);

	BOOL UrmManualProcCvJob(CString strIOGbn		, CString strJobNo		, CString strJobType	, CString strUUser	,
							int &nRetCd				, CString &strReturnMsg	);
	BOOL UrmManualVacantDelivery(CString strFrWhNo	, CString strFrLoc	, CString strJobNo		, CString strIOGbn	,
								 CString strUUser	, int &nRetCd		, CString &strReturnMsg	);
	BOOL UrmManualDoubleWareHousing(CString strFrWhNo	, CString strScNo	, CString strFrLoc		, CString strJobNo		,
									CString strIOGbn	, CString strUUser	, int &nRetCd			, CString &strReturnMsg	);
	BOOL UrmManualDelete(CString strJobNo, CString strIPAddress, CString strUser, int &nRetCd	, CString &strReturnMsg);

	BOOL UrmLogging(CString strLogGbn	, CString strLogPgmID	, CString strLogCode	, CString strLogDesc	,
					CString strLoginIP	, CString strUUser		, int &nRetCd			, CString &strReturnMsg	);

	BOOL UrmRackUpdateProc(CString strJobGbn	, CString strWhNo	, CString strLoc		,
						   CString strUUser		, int &nRetCd		, CString &strReturnMsg	);

	BOOL ChangeCSTState(int nJobNo, int nCSTStat);
	BOOL CellStateChange(CString strLoc, CString strState);
	BOOL GetEmptyLocation(CString strLoc, CString strZone, CString &strNewLoc, int nForkNo = 1);
	BOOL DoubleStorageProcess(CJobItem* pJobItem, CString strState, CString &strNewCSTID, int nForkNo);
	BOOL DuplicateCSTIDProcess(CString strCSTID, CString strLoc, CString strState, CString &strNewCSTID);
	BOOL EmptyOutputJobDelete(int nJobNo);

	// 동시작업 관련
	BOOL UpdatePairFlag(int nLuggNum, CString strWaitCheck);
	CString	GetIOMode(int nHour);

	int InsertSQL(CString p_strSql, CStringList& pStrList);


};

