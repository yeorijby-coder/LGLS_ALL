
#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "EcsView.h"

#include "URMDBAccess.h"

IMPLEMENT_DYNCREATE(CURMDBAccess, CObject)

CURMDBAccess::CURMDBAccess()
{
}

CURMDBAccess::CURMDBAccess(CEcsDoc* pDoc, CAdoDB* pAdoDB)
{
	m_pDoc			= pDoc;
	m_pAdoDB		= pAdoDB;

	m_strDBErrMsg	= "";
}

CURMDBAccess::~CURMDBAccess()
{

}


int CURMDBAccess::SelectQryCnt(CString p_strSql)
{
	if ( m_pAdoDB == NULL || m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	CString		strSql = p_strSql;
	int nRowCnt = -1;
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, nRowCnt, m_strDBErrMsg))
	{
		return -1;
	}

	return nRowCnt;
}

////////////////////////////////////////////////////////////////////////////////////
// STKCIM Database Access
// 재고 내 CST ID 위치 찾기
BOOL CURMDBAccess::FindCSTID(CString strCSTID, CString &strLoc, CString &strZoneName, CString &strSHIPID, CString &strFOSBID)
{

	/*
	CStringList strTempList;

	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	strLoc		= "";
	strZoneName	= "";
	strSHIPID	= "";
	strFOSBID	= "";

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select Loc	, WhZone	, SHIPID	, FOSBID	,						"
				  "        (Select V_ZoneName From ZoneInfo Where ZoneNo = WhZone) ZoneName	"
				  "   From LocMast			"
				  "  Where CSTID = '%s'		", strCSTID);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return FALSE;
	}

	CString strTaskStatus;

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "LOC")		strLoc = strValue;
		if (StrFieldName == "SHIPID")	strSHIPID = strValue;
		if (StrFieldName == "FOSBID")	strFOSBID = strValue;
		if (StrFieldName == "ZONENAME")	strZoneName = strValue;
	}

	// Duplicate CST ID Location 은 공랙 처리한다.
	CString strUpdate;

	strUpdate.Format(_T(" Update LocMast					"
					 "    Set RackStat		= '0',		"
					 "        OldRackStat	= '0',		"
					 "        WmsJob		= '',		"
					 "        InDateTime	= NULL,		"
					 "        JobjisiDate	= NULL,		"
					 "        JobNo			= '0000',	"
					 "        RackGbn		= '1',		"
					 "        StockGbn		= '1',		"
					 "        JobReason		= '',		"
					 "        JobGubun		= '',		"
					 "        Qty			= 0,		"
					 "        CSTID			= '',		"
					 "        SHIPID		= '',		"
					 "        FOSBID		= '',		"
					 "        LotID			= '',		"
					 "        LotPriority	= '',		"
					 "        LotType		= '',		"
					 "        UserLotID		= '',		"
					 "        MSCODE		= '',		"
					 "        Descript1		= '',		"
					 "        Descript2		= '',		"
					 "        CSTType		= '',		"
					 "        CSTStat		= '',		"
					 "        CarStat		= '',		"
					 "        EmptyFlag		= '',		"
					 "        Cleaned		= '',		"
					 "        Remark		= '',		"
					 "        UDate			= now(),	"
					 "        UUser			= '%s'		"		
					 "  Where Loc			= '%s'		"), m_pDoc->m_strUserID, strLoc);

	if ( !m_pAdoDB->ExecuteSqlForMainPGM( strUpdate ) )
	{
		CString strLog;
		strLog.Format("Location[%s] 정보 변경.. Database Update 오류..", strLoc);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog, m_pDoc->m_strUserID);
		return FALSE;
	}
	*/
	return TRUE;
}

// 작업 정보 찾기
BOOL CURMDBAccess::FindJobInfo(CString strCSTID)
{
	/*
	CStringList strTempList;

	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select Count(*) JOBCNT	"
				  "   From JobInfo			"
				  "  Where CSTID = '%s'		", strCSTID);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	if (strTempList.IsEmpty())
	{
		return FALSE;
	}
	
	int nCnt;

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "JOBCNT")		nCnt = atoi(strValue);
	}
	
	if ( nCnt == 0 )
		return TRUE;

	// Duplicate CST ID 작업은 삭제 처리한다.
	CString strDelete;

	strDelete.Format(" Delete From JobInfo	"
					 "  Where CSTID = '%s'	"
					 "    And IOGbn <> '01'	", strCSTID);

	if ( !m_pAdoDB->ExecuteSqlForMainPGM( strDelete ) )
	{
		CString strLog;
		strLog.Format("CST ID[%s] 작업정보 삭제.. Database Update 오류..", strCSTID);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog, m_pDoc->m_strUserID);
		return FALSE;
	}

	// Job Information
	CStringList strTempList2;
	strSql.Format(" Select ToLoc		"
				  "   From JobInfo		"
				  "  Where CSTID = '%s'	"
				  "    And IOGbn = '01'	", strCSTID);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList2, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	if (strTempList2.IsEmpty())
	{
		return TRUE;
	}
	
	CString	strToLoc;

	for (pos = strTempList2.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList2.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "TOLOC")		strToLoc = strValue;
	}

	if ( strToLoc != "000000" )
	{
		// Duplicate CST ID Location 은 공랙 처리한다.
		CString strUpdate;

		strUpdate.Format(" Update LocMast					"
						 "    Set RackStat		= '0',		"
						 "        OldRackStat	= '0',		"
						 "        WmsJob		= '',		"
						 "        InDateTime	= NULL,		"
						 "        JobjisiDate	= NULL,		"
						 "        JobNo			= '0000',	"
						 "        RackGbn		= '1',		"
						 "        StockGbn		= '1',		"
						 "        JobReason		= '',		"
						 "        JobGubun		= '',		"
						 "        Qty			= 0,		"
						 "        CSTID			= '',		"
						 "        SHIPID		= '',		"
						 "        FOSBID		= '',		"
						 "        LotID			= '',		"
						 "        LotPriority	= '',		"
						 "        LotType		= '',		"
						 "        UserLotID		= '',		"
						 "        MSCODE		= '',		"
						 "        Descript1		= '',		"
						 "        Descript2		= '',		"
						 "        CSTType		= '',		"
						 "        CSTStat		= '',		"
						 "        CarStat		= '',		"
						 "        EmptyFlag		= '',		"
						 "        Cleaned		= '',		"
						 "        Remark		= '',		"
						 "        UDate			= now(),	"
						 "        UUser			= '%s'		"		
						 "  Where Loc			= '%s'		", m_pDoc->m_strUserID, strToLoc);

		if ( !m_pAdoDB->ExecuteSqlForMainPGM( strUpdate ) )
		{
			CString strLog;
			strLog.Format("Location[%s] 정보 변경.. Database Update 오류..", strToLoc);
			LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog, m_pDoc->m_strUserID);
			return FALSE;
		}

		// Duplicate CST ID 작업은 삭제 처리한다.
		strDelete.Format(" Delete From JobInfo	"
						 "  Where CSTID = '%s'	", strCSTID);

		if ( !m_pAdoDB->ExecuteSqlForMainPGM( strDelete ) )
		{
			CString strLog;
			strLog.Format("압고 CST ID[%s] 작업정보 삭제.. Database Update 오류..", strCSTID);
			LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog, m_pDoc->m_strUserID);
			return FALSE;
		}
	}
	*/
	return TRUE;
}

// Task Information
BOOL CURMDBAccess::TaskStatus(CString strTaskName)
{
	/*
	CStringList strTempList;

	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select TaskStatus		"
				  "   From TaskInfo			"
				  "  Where TaskID = '%s'	", strTaskName);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	// Task 정보 없음
	if (strTempList.IsEmpty())
	{
		return FALSE;
	}

	CString strTaskStatus;

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "TASKSTATUS")	strTaskStatus = strValue;
	}
	
	if ( strTaskStatus != "1" )
		return FALSE;
		*/
	return TRUE;
}

// Port Information
BOOL CURMDBAccess::PortStatus(int nPortNo, CString strPortType, CString &strPortState, CString &strJisiGbn)
{
	/*
	CStringList strTempList;

	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	CString		strSql;
	int			nRowCnt;

	strSql.Format(" Select PortState, JisiGbn 	"
				  "   From PortInfo				"
				  "  Where PortNo = '%d'		"
				  "    And PortType = '%s'		", nPortNo, strPortType);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	// Task 정보 없음
	if (strTempList.IsEmpty())
	{
		strPortState	= "";
		strJisiGbn		= "";

		return FALSE;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "PORTSTATE")	strPortState	= strValue;
		if (StrFieldName == "JISIGBN")		strJisiGbn		= strValue;
	}
	*/
	return TRUE;
}

BOOL CURMDBAccess::ChangePortStatus(int nPortNo, CString strPortType)
{
	/*
	CString strSql;

	strSql.Format(" Update PortInfo			"
				  "    Set PortType = '%s'	, UDate = Sysdate	, UUser = '%s'	"
				  "  Where PortNo = '%04d'	", strPortType, m_pDoc->m_strUserID, nPortNo);

	if ( !m_pAdoDB->ExecuteSqlForMainPGM( strSql ) )
	{
		CString strLog;
		strLog.Format("Port 상태 변경.. Database Update 오류.. PortNo=[%04d]", nPortNo);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog, m_pDoc->m_strUserID);
		return FALSE;
	}
	*/
	return TRUE;
}

// Port Information
BOOL CURMDBAccess::CheckScStatus(int nScNo, int nJobType)
{
	/*
	CStringList strTempList;

	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	CString		strSql;
	int			nRowCnt;

	strSql.Format(" Select ScStatus	, ScCut	"
				  "   From ScInfo			"
				  "  Where ScNo = %d		", nScNo);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	if (strTempList.IsEmpty())
	{
		return FALSE;
	}

	int	nScStatus;
	int nScCut;

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "SCSTATUS")		nScStatus	= atoi(strValue);
		if (StrFieldName == "SCCUT")		nScCut		= atoi(strValue);
	}

	switch ( nJobType )
	{
		case 1 :							// 입고
			if ( (nScStatus == 1 || nScStatus == 2) || (nScCut == 1 || nScCut == 3) )
				return FALSE;

			break;

		case 2 :							// 출고
			if ( (nScStatus == 1 || nScStatus == 3) || (nScCut == 2 || nScCut == 3) )
				return FALSE;

			break;

		case 3 :							// 이동
			if ( (nScStatus != 0) || (nScCut != 0) )
				return FALSE;

			break;

		default :
			return FALSE;
			break;
	}
	*/
	return TRUE;
}

// Zone No 구하기
CString CURMDBAccess::GetZoneNo(CString strZoneName)
{
	
	CStringList strTempList;
	CString		strZoneNo = _T("");
	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return strZoneNo;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select ZoneNo			"
				  "   From ZoneInfo			"
				  "  Where ZoneName = '%s'	", strZoneName);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return strZoneNo;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return strZoneNo;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "ZONENO")		strZoneNo	= strValue;
	}
	*/
	return strZoneNo;
}

// ZoneName 구하기
CString CURMDBAccess::GetZoneName(CString strZoneNo)
{
	
	CStringList strTempList;
	CString		strZoneName = _T("");
	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return strZoneName;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select ZoneName			"
				  "   From ZoneInfo			"
				  "  Where ZoneNo = '%s'	", strZoneNo);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return strZoneName;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return strZoneName;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "ZONENAME")		strZoneName	= strValue;
	}
	*/
	return strZoneName;
}

// Location 으로 ZoneName 구하기
CString CURMDBAccess::GetLocationZoneName(CString strLoc)
{
	CStringList strTempList;
	CString		strZoneName = _T("");
	CString		strLocation;
	/*
	strLoc.TrimLeft();
	strLoc.TrimRight();

	if ( strLoc.GetLength() > 6 )
		strLocation.Format("%2s%4s", strLoc.Left(2), strLoc.Right(4));
	else
		strLocation = strLoc;

	if ( m_pAdoDB->m_bConnected == FALSE )
		return strZoneName;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select V_ZoneName		"
				  "   From ZoneInfo			"
				  "  Where ZoneNo = (Select WhZone From LocMast Where Loc = '%s') ", strLocation);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return strZoneName;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return strZoneName;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "V_ZONENAME")		strZoneName	= strValue;
	}
	*/
	return strZoneName;
}

// 코드 이름 구하기
CString CURMDBAccess::GetCodeName(CString strCodeGroup, CString strCodeNo)
{
	CStringList strTempList;
	CString		strCodeName = _T("");
	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return strCodeName;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select Code_Name 	"
				  "   From Code			"
				  "  Where Code_Group = '%s'	"
				  "    And Code_No = '%s'		", strCodeGroup, strCodeNo);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return strCodeName;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return strCodeName;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "CODE_NAME")	strCodeName	= strValue;
	}
	*/
	return strCodeName;
}

// 창고 이름 구하기
CString CURMDBAccess::GetWhName(CString strWhNo)
{
	CStringList strTempList;
	CString		strWhName = _T("");
	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return strWhName;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select WhDesc 		"
				  "   From WhInfo		"
				  "  Where WhNo = '%s'	", strWhNo);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return strWhName;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return strWhName;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "WHDESC")	strWhName	= strValue;
	}
	*/
	return strWhName;
}

// Port 등록 확인
int CURMDBAccess::GetPortConfirm(CString strPortNo)
{
	CStringList strTempList;
	CString		strGetNo = _T("");

	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return 0;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select PortNo 		"
				  "   From PortInfo		"
				  "  Where PortNo = '%s'	", strPortNo);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return 0;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return 0;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "PORTNO")	strGetNo	= strValue;
	}
	*/
	return strGetNo.GetLength();
}

// 사용자 ID 등록 확인
int CURMDBAccess::GetUserConfirm(CString strUserID)
{
	CStringList strTempList;
	CString		strGetID = _T("");

	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return 0;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select LoginID 		"
				  "   From Users		"
				  "  Where LoginID = '%s'	", strUserID);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return 0;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return 0;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "LOGINID")	strGetID	= strValue;
	}
	*/
	return strGetID.GetLength();
}

// 프로그램 ID 등록 확인
int CURMDBAccess::GetPgmConfirm(CString strPgmID)
{
	CStringList strTempList;
	CString		strGetID = _T("");

	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return 0;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select PgmID 		"
				  "   From PgmMast		"
				  "  Where PgmID = '%s'	", strPgmID);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return 0;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return 0;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "PGMID")	strGetID	= strValue;
	}
	*/
	return strGetID.GetLength();
}

// Code Group 등록 확인
int CURMDBAccess::GetCodeGroupConfirm(CString strCodeGroup)
{
	CStringList strTempList;
	CString		strGetCodeGroup = _T("");

	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return 0;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select Code_Group 			"
				  "   From CodeGroup			"
				  "  Where Code_Group = '%s'	", strCodeGroup);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return 0;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return 0;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "CODE_GROUP")	strGetCodeGroup	= strValue;
	}
	*/
	return strGetCodeGroup.GetLength();
}

// Zone No 등록 확인
int CURMDBAccess::GetZoneConfirm(CString strZoneNo)
{
	CStringList strTempList;
	CString		strGetZoneNo = _T("");

	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return 0;

	CString		strSql;
	int			nRowCnt;

	// Job Information
	strSql.Format(" Select ZoneNo 			"
				  "   From ZoneInfo			"
				  "  Where ZoneNo = '%s'	", strZoneNo);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return 0;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return 0;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "ZONENO")	strGetZoneNo	= strValue;
	}
	*/
	return strGetZoneNo.GetLength();
}

// Zone No 사용 확인
int CURMDBAccess::GetZoneUseConfirm(CString strZoneNo)
{
	CStringList strTempList;
	CString		strZoneCnt = _T("");

	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return 0;

	CString		strSql;
	int			nRowCnt;

	strSql.Format(" Select Count(*) ZoneCnt	"
				  "   From LocMast			"
				  "  Where WhZone = '%s'	", strZoneNo);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return 0;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return 0;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "ZONECNT")	strZoneCnt	= strValue;
	}
	*/
	return CConvert::ToInt(strZoneCnt);
}

// 제품 출고 작업 번호 확인
BOOL CURMDBAccess::GetOutJobOrderGroup(CString &strJobGroup, CString &strJobNo)
{
	CStringList strTempList;
	/*
	strJobGroup	= "0";
	strJobNo	= "0";

	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	CString		strSql;
	int			nRowCnt;

	strSql.Format(" Select Min(JobOrderTime) ORDERTIME, Min(JobNo) ORDERJOBNO	"
				  "   From JobInfo			"
				  "  Where IOGbn = '02'		"
				  "    And SendGbn = '4'	"
				  "    And ToPortNo = '%d'	"
				  "    And WmsJob = '21'	"
				  "    And ECSErrCode = 'N'	"
				  "  Order By JobOrderTime	", ECS_STN_POS_12910);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return FALSE;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "ORDERTIME")		strJobGroup	= strValue;
		if (StrFieldName == "ORDERJOBNO")		strJobNo	= strValue;
	}
	*/
	return TRUE;
}

// 동시입고 작업 확인
int CURMDBAccess::GetInputJobCnt()
{
	CStringList strTempList;
	CString strJobCnt = _T("0");

	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return 0;

	CString		strSql;
	int			nRowCnt;

	strSql.Format(" Select Count(*) INPUTCNT"
				  "   From JobInfo			"
				  "  Where IOGbn = '01'		"
				  "    And SendGbn In ('1', '2', '3', '4')	"
				  "    And WmsJob = '11'	"
				  "    And WaitCheck <> 'Y'	"
				  "    And ECSErrCode = 'N'	");
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return 0;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return 0;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "INPUTCNT")		strJobCnt	= strValue;
	}
*/
	return CConvert::ToInt(strJobCnt);
}


// 콤보박스 만들기 With Zone
void CURMDBAccess::MakeComboWithZone(CComboBoxWrapper *comboBox, CString strZoneType, CString strAll, int nMakeType, CString strIndexType)
{
	CStringList strTempList;
	CString		strZone	= _T("");
	CString		strName	= _T("");


	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return;

	CString		strSql;
	int			nRowCnt;

	comboBox->ResetContent();

	if ( strAll == "전체" )
		comboBox->AddString("전체");

	// Job Information
	strSql.Format(" Select ZoneNo, ZoneName "
				  "   From ZoneInfo			"
				  "  Where ZoneType = '%s'	"
				  "    And ZoneUseGbn = 'Y'	"
				  "  Order By ZoneNo %s		", strZoneType, strIndexType);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		comboBox->SetCurSel(0);
		return;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		comboBox->SetCurSel(0);
		return;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "ZONENO")		strZone	= strValue;
		if (StrFieldName == "ZONENAME")
		{
			CString strMakeCombo;

			strName	= strValue;

			switch ( nMakeType )
			{
				case MAKE_CODE :
					comboBox->AddString((LPCTSTR)strZone);
					break;

				case MAKE_NAME :
					comboBox->AddString((LPCTSTR)strName);
					break;

				case MAKE_CODENAME :
					strMakeCombo.Format("%s : %s", strZone, strName);
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;

				case MAKE_NAMECODE :
					strMakeCombo.Format("%s : %s", strName, strZone);
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;

				case MAKE_CODEBACKNAME :
					strMakeCombo.Format("%-60s : %s", strZone, strName);
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;

				case MAKE_NAMEBACKCODE :
					strMakeCombo.Format("%-60s : %s", strName, strZone);
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;
			}
		}
	}

	comboBox->SetCurSel(0);
	*/
	return;
}

// 콤보박스 만들기 With Code
void CURMDBAccess::MakeComboWithCode(CComboBoxWrapper *comboBox, CString strCodeGroup, CString strAll, int nMakeType, CString strIndexType)
{
	CStringList strTempList;
	CString		strCode	= _T("");
	CString		strName	= _T("");

	if ( m_pAdoDB->m_bConnected == FALSE )
		return;

	CString		strSql;
	int			nRowCnt;

	comboBox->ResetContent();

	if ( strAll == _T("전체") )
		comboBox->AddString(_T("전체"));

	// Job Information
	strSql.Format(_T(" SELECT CDX_CD, CCD_KOR 	")
				  _T("   FROM COMMON_CODE		")
				  _T("  WHERE CCD_CD_YN = 'Y'	")
				  _T("  ORDER BY CCD_CD "), strCodeGroup, strIndexType);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		comboBox->SetCurSel(0);
		return;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		comboBox->SetCurSel(0);
		return;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "CDX_CD")		strCode	= strValue;
		if (StrFieldName == "CCD_KOR")
		{
			CString strMakeCombo;

			strName	= strValue;

			switch ( nMakeType )
			{
				case MAKE_CODE :
					comboBox->AddString((LPCTSTR)strCode);
					break;

				case MAKE_NAME :
					comboBox->AddString((LPCTSTR)strName);
					break;

				case MAKE_CODENAME :
					strMakeCombo.Format(_T("%s : %s", strCode, strName));
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;

				case MAKE_NAMECODE :
					strMakeCombo.Format(_T("%s : %s", strName, strCode));
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;

				case MAKE_CODEBACKNAME :
					strMakeCombo.Format(_T("%-60s : %s", strCode, strName));
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;

				case MAKE_NAMEBACKCODE :
					strMakeCombo.Format(_T("%-60s : %s", strName, strCode));
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;
			}
		}
	}

	comboBox->SetCurSel(0);
	
	return;
}

// 콤보박스 만들기 With Port
void CURMDBAccess::MakeComboWithPort(CComboBoxWrapper *comboBox, CString strPortGbn, CString strPortType, int nMakeType)
{
	CStringList strTempList;
	CString		strCode	= _T("");
	CString		strName	= _T("");

	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return;

	CString		strSql;
	int			nRowCnt;

	comboBox->ResetContent();

	// Job Information
	strSql.Format(" Select PortNo, PortDesc "
				  "   From PortInfo			"
				  "  Where PortGbn = '%s'	"
				  "    And PortType = '%s'	"
				  "  Order By PortNo		", strPortGbn.MakeUpper, strPortType);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		comboBox->SetCurSel(0);
		return;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		comboBox->SetCurSel(0);
		return;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "PORTNO")		strCode	= strValue;
		if (StrFieldName == "PORTDESC")
		{
			CString strMakeCombo;

			strName	= strValue;

			switch ( nMakeType )
			{
				case MAKE_CODE :
					comboBox->AddString((LPCTSTR)strCode);
					break;

				case MAKE_NAME :
					comboBox->AddString((LPCTSTR)strName);
					break;

				case MAKE_CODENAME :
					strMakeCombo.Format("%s : %s", strCode, strName);
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;

				case MAKE_NAMECODE :
					strMakeCombo.Format("%s : %s", strName, strCode);
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;

				case MAKE_CODEBACKNAME :
					strMakeCombo.Format("%-60s : %s", strCode, strName);
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;

				case MAKE_NAMEBACKCODE :
					strMakeCombo.Format("%-60s : %s", strName, strCode);
					comboBox->AddString((LPCTSTR)strMakeCombo);
					break;
			}
		}
	}

	comboBox->SetCurSel(0);
	*/
	return;
}

// 콤보박스 데이타 찾기
void CURMDBAccess::FindCombo(CComboBoxWrapper *comboBox, CString strCode, int nGetType, CString strDivisionChar)
{
	return;
}

CString CURMDBAccess::GetComboData(CString strComboText, int nGetType, CString strDivisionChar)
{
	int		nPos;

	// 선택된 문자열에서 구분자를 찾는다.
	// 찾는 문자열이 없을경우 문자열 전체를 리턴한다. (Ex. 전체)
	
	nPos = strComboText.FindOneOf(strDivisionChar);
	if ( nPos < 0 )
		return strComboText;

	CString strReturn;

	/*
	switch ( nGetType )
	{
		case GET_FRONT :
			// 구분자 이전 문자 리턴
			strReturn = strComboText.Left(nPos - 1);
			strReturn.TrimRight();
			strReturn.TrimLeft();

			break;

		case GET_REAR :
			// 구분자 이후 문자 리턴
			strReturn = strComboText.Right(strComboText.GetLength() - nPos - 1);
			strReturn.TrimRight();
			strReturn.TrimLeft();
			break;

		default :
			strReturn = strComboText;
			break;
	}
	*/
	return strReturn;
}

// 문자열 기호 제거
CString CURMDBAccess::EjectChar(CString strSourceText, CString strEjectChar)
{
	int		nCnt;
	CString strTempSourceText;
	CString strTempEjectChar;
	CString strReturnText = _T("");

	strTempSourceText.Format(_T("%s"), (strSourceText));
	strTempEjectChar.Format(_T("%s"), (strEjectChar));

	for ( nCnt = 0; nCnt < strTempSourceText.GetLength(); nCnt++)
	{
		if ( strTempSourceText.Mid(nCnt, 1) != strTempEjectChar )
			strReturnText = strReturnText + strTempSourceText.Mid(nCnt, 1);
	}

	return strReturnText;
}

// 시스템 시간 구하기
BOOL CURMDBAccess::GetSysDate(CString &strSysDate, CString &strSysMilliSecond)
{
	CStringList strTempList;

	strSysDate = _T("");
	strSysMilliSecond = _T("");
	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	/*
	CString		strSql;
	int			nRowCnt;

	// SysDate Information
	strSql.Format(_" Select to_char(SysDate, 'yyyymmddhh24miss') SYSTEM_DATE,	"
				  "        to_char(SysTimeStamp, 'ff3') SYSTEM_MILLI			"
				  "   From Dual	");
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		return FALSE;
	}

	for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "SYSTEM_DATE")		strSysDate			= strValue;
		if (StrFieldName == "SYSTEM_MILLI")		strSysMilliSecond	= strValue;
	}
	*/
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// STKCIM Job Order
// 작업 지시 확인
BOOL CURMDBAccess::ConfirmJobInfo(CString strCSTID)
{
	CStringList strTempList;
	/*
	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	CString		strSql;
	int			nRowCnt;

	// SysDate Information
	strSql.Format(" Select JobNo		" 
				  "   From JobInfo		"
				  "  Where CSTID = '%s'	", strCSTID);
		
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	// 정보 없음
	// 작업을 생성해도 됨.
	if (strTempList.IsEmpty())
		return TRUE;

	// 해당 Carrier ID 는 이미 작업이 생성되어 있으므로 추가작업을 생성하지 않는다.
	*/
	return FALSE;
}

// 이동작업 작업정보 확인
BOOL CURMDBAccess::ConfirmMoveLocMast(CString strFrLoc, CString strToLoc, CString &strFrZone, CString &strToZone)
{
	return TRUE;
}

// 공 Tray 출고 작업 갯수 확인
BOOL CURMDBAccess::ConfirmVacantJobInfo(int &nVacantCnt, int &nWaitTrayCnt, int &nStockVacantCnt)
{
	CStringList strTempList;

	/*
	nVacantCnt = 0;
	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	CString		strSql;
	int			nRowCnt;

	// 공팔레트 재고 확인
	strSql.Format(" Select Count(*) STOCKVACANTCNT	"
				  "   from LocMast					"
				  "  where RackStat = '2'			"
				  "    And WMSJob In ('16', '31')	"
				  "    And EmptyFlag = '0'			");
				 
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		nVacantCnt		= 0;
		nWaitTrayCnt	= MAX_VACANT_CNT;
		nStockVacantCnt	= 0;

		return TRUE;
	}

	POSITION pos;
	for (pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "STOCKVACANTCNT")	nStockVacantCnt	= atoi(strValue);
	}

	if ( nStockVacantCnt <= 0 )
	{
		nVacantCnt		= 0;
		nWaitTrayCnt	= MAX_VACANT_CNT;
		nStockVacantCnt	= 0;

		return TRUE;
	}

	// 공팔레트 최대 출고 가능 갯수
	strSql.Format(" Select WaitTray			" 
				  "   From portInfo			"
				  "  Where PortNo = '%d'	", ECS_STN_POS_12910);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		nVacantCnt		= 0;
		nWaitTrayCnt	= 0;

		return TRUE;
	}

	for (pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "WAITTRAY")			nWaitTrayCnt	= atoi(strValue);
	}

	// 공팔레트 작업 정보
	strSql.Format(" Select Count(*) VACANTCNT	"
				  "   From JobInfo				"
				  "  Where ToPortNo = '%d'		"
				  "    And WmsJob = '%s'		"
				  "  Group By ToPortNo			", ECS_STN_POS_12910, WMSJOB_VACANT_OUTPUT);
	
	if (!m_pAdoDB->SelectSqlForThread(strSql, strTempList, 0, nRowCnt, m_strDBErrMsg))
	{
		return FALSE;
	}

	// 정보 없음
	if (strTempList.IsEmpty())
	{
		nVacantCnt		= 0;

		return TRUE;
	}

	for (pos = strTempList.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = strTempList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);
		
		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "VACANTCNT")		nVacantCnt		= atoi(strValue);
	}
	*/
	return TRUE;
}

// Command ID 생성
BOOL CURMDBAccess::MakeCommandID(CString strIOGbn, CString strCSTID, int nSeq, CString &strCommandID, CString &strCmdTime)
{
	CString strSysDateTime;
	CString strSysMilliSecond;

	/*
	if ( !GetSysDate(strSysDateTime, strSysMilliSecond) )
		return FALSE;

	strCommandID.Format("%sS_%-10s%-13s%02d%14s.%-5s", strIOGbn, MDLN, strCSTID, nSeq, strSysDateTime, strSysMilliSecond);
	strCmdTime.Format("%s%s", strSysDateTime, strSysMilliSecond.Left(2));
	*/
	return TRUE;
}


// 강제 삭제
BOOL CURMDBAccess::UrmManualDelete(CString strJobNo, CString strIPAddress, CString strUser, int &nRetCd, CString &strReturnMsg)
{
	CString strSql;

	/*
	strSql.Format(" Delete From JobInfo	"
				  "  Where JobNo = '%s'	", strJobNo);

	if ( !m_pAdoDB->ExecuteSqlForMainPGM( strSql ) )
	{
		nRetCd = -1;
		strReturnMsg.Format("이상시 처리 강제 삭제.. Database Delete 오류.. 작업번호=[%s]", strJobNo);
		return FALSE;
	}

	// 삭제 로그 생성
	CString	strMsg;
	strMsg.Format("삭제 : %s", strSql);
	UrmLogging("PGM"		, "SYS020"	, "0"		, strMsg		,
			   strIPAddress	, strUser	, nRetCd	, strReturnMsg	);

	nRetCd			= 0;
	strReturnMsg	= "";

	*/
	return TRUE;
}

// STKCIM 로그생성
// CREATE OR REPLACE PROCEDURE Sp_Logging( poRetCd          OUT NUMBER,
// 										poMsg            OUT VARCHAR2,
// 										poDBErr          OUT VARCHAR2,
// 										piLogGbn          IN VARCHAR2,
// 										piLogPgmID        IN VARCHAR2,
// 										piLogCode         IN VARCHAR2,
// 										piLogDesc         IN VARCHAR2,
// 										piLoginID         IN VARCHAR2,
// 										piLoginIP         IN VARCHAR2 ) AS
BOOL CURMDBAccess::UrmLogging(CString strLogGbn	, CString strLogPgmID	, CString strLogCode	, CString strLogDesc	,
							  CString strLoginIP, CString strUUser		, int &nRetCd			, CString &strReturnMsg	)
{

	return TRUE;
}

// Shelf 상태 변경
//CREATE OR REPLACE PROCEDURE Sp_RackUpdateProc(poRetCd           OUT NUMBER,
//											  poMsg             OUT VARCHAR2,
//											  poDBErr           OUT VARCHAR2,
//											  piJobGbn          IN VARCHAR2,
//											  piWhNo            IN VARCHAR2,
//											  piLoc             IN VARCHAR2,
//											  piUUser           IN VARCHAR2 ) IS
BOOL CURMDBAccess::UrmRackUpdateProc(CString strJobGbn	, CString strWhNo	, CString strLoc		,
									 CString strUUser	, int &nRetCd		, CString &strReturnMsg	)
{

	return TRUE;
}



// 금지랙 설정
BOOL CURMDBAccess::CellStateChange(CString strLoc, CString strState)
{
	CString strSql;

	/*
	strSql.Format(" Update LocMast			"
				  "    Set RackStat = '%s',	CSTStat = '%d'	"
				  "  Where Loc = '%s'		"
				  "    And RackGbn <> '9'	", strState, CSTSTAT_BLOCK, strLoc);

	if ( !m_pAdoDB->ExecuteSqlForMainPGM( strSql ) )
	{
		CString strLog;
		strLog.Format("Shelf 상태 변경.. Database Update 오류.. Location=[%s]", strLoc);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog, m_pDoc->m_strUserID);
		return FALSE;
	}
	*/
	return TRUE;
}

// 공랙 구하기
BOOL CURMDBAccess::GetEmptyLocation(CString strLoc, CString strZone, CString &strNewLoc, int nForkNo)
{
	return TRUE;
}

// 이중입고 랙 처리
BOOL CURMDBAccess::DoubleStorageProcess(CJobItem* pJobItem, CString strState, CString &strNewCSTID, int nForkNo)
{
	return TRUE;
}

// 공출고 작업 정보 삭제
BOOL CURMDBAccess::EmptyOutputJobDelete(int nJobNo)
{
	CString strSql;
	/*
	strSql.Format(" Delete From JobInfo	"
				  "  Where JobNo = '%d'	", nJobNo);

	if ( !m_pAdoDB->ExecuteSqlForMainPGM( strSql ) )
	{
		return FALSE;
	}
	*/
	return TRUE;
}

// Duplicate CST ID 처리
BOOL CURMDBAccess::DuplicateCSTIDProcess(CString strCSTID, CString strLoc, CString strState, CString &strNewCSTID)
{
	CStringList	strTempList;
	CString		strSql;

	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;
	/*
	// D + CTS ID (총 64 Byte)
	// => 동시에 해당 CST의 <DESCRIPT2> 정보에도 동일 CST ID (D+CST ID) 저장
	strNewCSTID.Format("D%s", strCSTID);

	// 이중입고 랙 금지및 CSTID 기록
	strSql.Format(" Update LocMast			"
				  "    Set RackStat = '%s'	, CSTID = '%s'	, Descript1 = '%s'	, Descript2 = '%s'	"
				  "  Where Loc = '%s'	", strState, strNewCSTID, strCSTID, strNewCSTID, strLoc);

	if ( !m_pAdoDB->ExecuteSqlForMainPGM( strSql ) )
	{
		CString strLog;
		strLog.Format("Duplicate CST ID.. Database Update 오류.. Location=[%s]", strLoc);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog, m_pDoc->m_strUserID);
		return FALSE;
	}
	*/
	return TRUE;
}

// 동시입고 플래그 수정
BOOL CURMDBAccess::UpdatePairFlag(int nLuggNum, CString strWaitCheck)
{
	CStringList	strTempList;
	CString		strSql;

	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;

	/*
	// 동시작업 플래그 수정
	strSql.Format(" Update JobInfo			"
				  "    Set WaitCheck = '%s'	"
				  "  Where JobNo = '%04d'	", strWaitCheck, nLuggNum);

	if ( !m_pAdoDB->ExecuteSqlForMainPGM( strSql ) )
	{
		CString strLog;
		strLog.Format("동시작업 플래그 Update Error.. 작업번호=[%04d]", nLuggNum);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog, m_pDoc->m_strUserID);
		return FALSE;
	}
	*/
	return TRUE;
}

CString	CURMDBAccess::GetIOMode(int nHour)
{
	
	return _T("");
}

//////////////////////////////////////////////////pcg common_code 추가
int CURMDBAccess::InsertSQL(CString p_strSql, CStringList& pStrList)
{
	if ( m_pAdoDB->m_bConnected == FALSE )
		return FALSE;


	CString		strSql = p_strSql;
	CString     strErrMsg;
	int			nRowCnt = -1;
	//	if (!m_pAdoDB->ExecuteStoredProc(strSql, pStrList, 0, nRowCnt, m_strDBErrMsg))
	if (!m_pAdoDB->ExecuteQueryString(strSql))
	{
		return -1;
	}


	return 1;
	//	return nRowCnt;
}
///////////////

