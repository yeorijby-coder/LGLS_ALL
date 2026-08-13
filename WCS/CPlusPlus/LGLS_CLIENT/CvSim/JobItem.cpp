// JobItem.cpp: implementation of the CJobItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "JobItem.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CJobItem, CObject, 0)

CJobItem::CJobItem()
{
	m_pDoc = NULL;

	m_nLuggNum = 0;
	m_nJobType = enJobTypeNone;
	m_nStartWH = 0;
	m_strStartStn.Empty();
	m_strStartLoc.Empty();
	m_nDestWH = 0;
	m_strDestStn.Empty();
	m_strDestLoc.Empty();
	m_ucGenCode = 0;
	m_ucCrateKind = 0;
	m_ucCrateStatus = 0;
	m_bBcrRead = FALSE;

	m_nProdCount= 0;
	m_nSeqNo = 0;

	m_nJobStatus = enJobStatusNew;
	m_nResultCode = enHostErrorNone;
	m_nDepartTrackNum = 0;
	m_nArriveTrackNum = 0;
	m_bRecvAckDualSto = FALSE;
	m_tTime = COleDateTime::GetCurrentTime();
	m_nPriority = 0;
	m_strAgingStn = _T("");
	m_nCvPlcNum = 0;
	m_nPairLuggNo = 0;
}

CJobItem::CJobItem(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	m_nLuggNum = 0;
	m_nJobType = enJobTypeNone;
	m_nStartWH = 0;
	m_strStartStn.Empty();
	m_strStartLoc.Empty();
	m_nDestWH = 0;
	m_strDestStn.Empty();
	m_strDestLoc.Empty();
	m_ucGenCode = 0;
	m_ucCrateKind = 0;
	m_ucCrateStatus = 0;
	m_bBcrRead = FALSE;

	m_nProdCount= 0;
	m_nSeqNo = 0;

	m_nJobStatus = enJobStatusNew;
	m_nResultCode = enHostErrorNone;
	m_nDepartTrackNum = 0;
	m_nArriveTrackNum = 0;
	m_bRecvAckDualSto = FALSE;
	m_tTime = COleDateTime::GetCurrentTime();
	m_nPriority = 0;
	m_strAgingStn = _T("");
	m_nPairLuggNo = 0;
}

CJobItem::CJobItem(CEcsDoc* pDoc, const CJobItem& rJobItem)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	m_nLuggNum		= rJobItem.m_nLuggNum;
	m_nJobType		= rJobItem.m_nJobType;
	m_nStartWH		= rJobItem.m_nStartWH;
	m_strStartStn	= rJobItem.m_strStartStn;
	m_strStartLoc	= rJobItem.m_strStartLoc;
	m_nDestWH		= rJobItem.m_nDestWH;
	m_strDestStn	= rJobItem.m_strDestStn;
	m_strDestLoc	= rJobItem.m_strDestLoc;
	m_ucGenCode		= rJobItem.m_ucGenCode;
	m_ucCrateKind	= rJobItem.m_ucCrateKind;
	m_ucCrateStatus = rJobItem.m_ucCrateStatus;
	m_strBarcode	= rJobItem.m_strBarcode;
	m_strProductID	= rJobItem.m_strProductID;
	m_strPalletNo	= rJobItem.m_strPalletNo;
	m_strCompany	= rJobItem.m_strCompany;
	m_bBcrRead		= rJobItem.m_bBcrRead;
	m_nSize			= rJobItem.m_nSize;
	m_nProdCount	= rJobItem.m_nProdCount;
	m_nSeqNo		= rJobItem.m_nSeqNo;

	m_nJobStatus		= rJobItem.m_nJobStatus;
	m_nResultCode		= rJobItem.m_nResultCode;
	m_nDepartTrackNum	= rJobItem.m_nDepartTrackNum;
	m_nArriveTrackNum	= rJobItem.m_nArriveTrackNum;
	m_bRecvAckDualSto	= rJobItem.m_bRecvAckDualSto;
	m_tTime				= rJobItem.m_tTime;
	m_nPriority			= rJobItem.m_nPriority;
}

CJobItem::~CJobItem()
{

}

CString CJobItem::GetString()
{
	CString strJobItem;
	strJobItem.Format(_T("%04d %02d %1d %5s %6s %1d %5s %6s %02d %1d %4d \r\n"),
		m_nLuggNum, m_nJobType,
		m_nStartWH, m_strStartStn, m_strStartLoc,
		m_nDestWH, m_strDestStn, m_strDestLoc,

		m_nJobStatus, m_bBcrRead, m_nSeqNo
	);
	return strJobItem;
}

void CJobItem::SetString(CString& strJobItem)
{
	if (strJobItem.GetLength() < 42)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	m_nLuggNum		= _ttoi(strJobItem.Left(4));
	m_nJobType		= _ttoi(strJobItem.Mid(4,2));
	m_nStartWH		= _ttoi(strJobItem.Mid(7,1));
	m_strStartStn	= strJobItem.Mid(9,5);
	m_strStartLoc	= strJobItem.Mid(15,6);
	m_nDestWH		= _ttoi(strJobItem.Mid(22,1));
	m_strDestStn	= strJobItem.Mid(24,5);
	m_strDestLoc	= strJobItem.Mid(30,6);
	m_nJobStatus	= _ttoi(strJobItem.Mid(37,2));
	m_bBcrRead		= _ttoi(strJobItem.Mid(40,1));
	m_nSeqNo		= _ttoi(strJobItem.Mid(41,4));
}

void CJobItem::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nLuggNum << m_nJobType;
		ar << m_nStartWH << m_strStartStn << m_strStartLoc;
		ar << m_nDestWH << m_strDestStn << m_strDestLoc;
		ar << m_ucGenCode << m_ucCrateKind << m_ucCrateStatus << m_strBarcode << m_strProductID << m_strPalletNo << m_strCompany;
		ar << m_nJobStatus << m_nResultCode << m_nDepartTrackNum << m_nArriveTrackNum;
		ar << m_bRecvAckDualSto << m_tTime << m_bBcrRead << m_nProdCount << m_nSeqNo << m_nSize << m_strAgingStn;
	}
	else
	{
		ar >> m_nLuggNum >> m_nJobType;
		ar >> m_nStartWH >> m_strStartStn >> m_strStartLoc;
		ar >> m_nDestWH >> m_strDestStn >> m_strDestLoc;
		ar >> m_ucGenCode >> m_ucCrateKind >> m_ucCrateStatus >> m_strBarcode >> m_strProductID >> m_strPalletNo >> m_strCompany;
		ar >> m_nJobStatus >> m_nResultCode >> m_nDepartTrackNum >> m_nArriveTrackNum;
		ar >> m_bRecvAckDualSto >> m_tTime >> m_bBcrRead >> m_nProdCount >> m_nSeqNo >> m_nSize >> m_strAgingStn;
	}
}

void CJobItem::SetJobStatus(int nJobStatus)
{
	m_nJobStatus = nJobStatus;
	m_tTime = COleDateTime::GetCurrentTime();
	MulticastEditJob();
}

void CJobItem::SetResultCode(int nResultCode)
{
	m_nResultCode = nResultCode;
	m_tTime = COleDateTime::GetCurrentTime();
	MulticastEditJob();
}

CString CJobItem::GetXmlString()
{
	CString strXml;
	strXml.Format(_T("<ITEM NO='%d' JT='%d' SWH='%d' SSTN='%s' SLOC='%s' DWH='%d' DSTN='%s' DLOC='%s' GEN='%d' CRTK='%d' CRTS='%d' BCD='%s' PROD='%s' PLTNO='%s' CMPN='%s' CNT='%d' STS='%d' RES='%d' DTR='%d' ATR='%d' BBR='%d' SEQNO='%d' SIZE='%d' TIME='%d'/>"),
		m_nLuggNum, m_nJobType, m_nStartWH, m_strStartStn, m_strStartLoc, m_nDestWH, m_strDestStn, m_strDestLoc,
		m_ucGenCode, m_ucCrateKind, m_ucCrateStatus, m_strBarcode, m_strProductID, m_strPalletNo, m_strCompany, m_nProdCount, m_nJobStatus, m_nResultCode, 
		//m_nDepartTrackNum, m_nArriveTrackNum, m_bBcrRead, m_nSeqNo, m_nSize, m_tTime.GetTime()
		m_nDepartTrackNum, m_nArriveTrackNum, m_bBcrRead, m_nSeqNo, m_nSize, m_tTime.m_dt
	);
	return strXml;
}

void CJobItem::MulticastAddJob(CMonitorServer* pMonitorSv /* = NULL */)
{
	CString strSend;
	strSend.Format(_T("%c<ECS><JOB><ADD>%s</ADD></JOB></ECS>%c"), STX, GetXmlString(), ETX);


}

void CJobItem::MulticastEditJob(CMonitorServer* pMonitorSv /* = NULL */)
{
	CString strSend;
	strSend.Format(_T("%c<ECS><JOB><EDIT>%s</EDIT></JOB></ECS>%c"), STX, GetXmlString(), ETX);

	
}

void CJobItem::MulticastRemoveJob(CMonitorServer* pMonitorSv /* = NULL */)
{
	CString strSend;
	strSend.Format(_T("%c<ECS><JOB><REMOVE>%s</REMOVE></JOB></ECS>%c"), STX, GetXmlString(), ETX);

	
}

// @@@@@
CString CJobItem::GetLogString()
{
	CString strLog;
	strLog.Format(_T("작업번호=%d, 작업종류=%s, 시작위치=%s, 완료위치=%s, TRAY=%s"), 
		m_nLuggNum, GetJobTypeString(), GetStartStationString(), GetDestStationString(), m_strBarcode);
	return strLog;
}

BOOL CJobItem::IsEqual(CJobItem* pJobItem)
{
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	if ((m_nJobType == pJobItem->m_nJobType) &&	(m_strBarcode == pJobItem->m_strBarcode) &&
		(m_nStartWH == pJobItem->m_nStartWH) && (m_strStartStn == pJobItem->m_strStartStn) && (m_strStartLoc == pJobItem->m_strStartLoc) &&
		(m_nDestWH == pJobItem->m_nDestWH) && (m_strDestStn == pJobItem->m_strDestStn) && (m_strDestLoc == pJobItem->m_strDestLoc))
		return TRUE;

	return FALSE;
}

int CJobItem::GetTrackDestination()
{
	CString strStation = _T("");
	if (m_strDestStn == _T("101") || m_strDestStn == _T("102") || m_strDestStn == _T("103") || m_strDestStn == _T("104") ||
		m_strDestStn == _T("105") || m_strDestStn == _T("106") || m_strDestStn == _T("107"))
		strStation = m_strAgingStn;
	else
		strStation = m_strDestStn;

	CStationInfo* pStation = m_pDoc->GetStationInfo(strStation);
	if (pStation == NULL)
	{
		//DEBUGER_ASSERT_VALID_LOG(FALSE, strStation);
		return 0;
	}

	return pStation->GetTrackDestination();
}

CString CJobItem::GetAgingStation()
{
	CString strStation = _T("");
	if (m_strDestStn == _T("101") || m_strDestStn == _T("102") || m_strDestStn == _T("103") || m_strDestStn == _T("104") ||
		m_strDestStn == _T("105") || m_strDestStn == _T("106") || m_strDestStn == _T("107"))
		strStation = m_strAgingStn;
	else
		strStation = _T("");

	return strStation;
}

//////////////////////////////////////////////////////////////////////////

BOOL CJobItem::IsOnlineLuggNum(int nLuggNum)
{
	return ((nLuggNum > 0) && (nLuggNum < 9000)) ? TRUE : FALSE;
}

BOOL CJobItem::IsOfflineLuggNum(int nLuggNum)
{
	return ((nLuggNum > 9000) && (nLuggNum < 9999)) ? TRUE : FALSE;
}

BOOL CJobItem::IsOnlineJobType(int nJobType)
{
	switch (nJobType)
	{
	case enJobTypeAutoSto:	
	case enJobTypeAutoRet:	
	case enJobTypeAutoR2R:	
	case enJobTypeAutoPR:	
	case enJobTypeAutoMove:	
	case enJobTypeAutoW2W:	
		return TRUE;
	}

	return FALSE;
}

BOOL CJobItem::IsOfflineJobType(int nJobType)
{
	switch (nJobType)
	{
	case enJobTypeSemiSto:	
	case enJobTypeSemiRet:	
	case enJobTypeSemiR2R:	
	case enJobTypeSemiPR:	
	case enJobTypeSemiMove:	
	case enJobTypeSemiW2W:	
		return TRUE;
	}

	return FALSE;
}

int CJobItem::ConvertJobTypeToPattern(int nJobType)
{
	switch (nJobType)
	{
	case enJobTypeAutoSto:
	case enJobTypeSemiSto:
		return enJobPatternSto;

	case enJobTypeAutoRet:
	case enJobTypeSemiRet:
		return enJobPatternRet;

	case enJobTypeAutoR2R:
	case enJobTypeSemiR2R:
		return enJobPatternR2R;

	case enJobTypeAutoMove:
	case enJobTypeSemiMove:
		return enJobPatternMove;

	case enJobTypeAutoW2W:
	case enJobTypeSemiW2W:
		return enJobPatternW2W;

	case enJobTypeAutoPR:
	case enJobTypeSemiPR:
		return enJobPatternPR;

	}

	return -1;
}

CString CJobItem::GetJobTypeString(int nJobType)
{
	switch (nJobType)
	{
	case enJobTypeNone:			return _T("없음");
	case enJobTypeAutoSto:		return _T("입고");
	case enJobTypeAutoRet:		return _T("출고");
	case enJobTypeAutoR2R:		return _T("랙이동");
	case enJobTypeAutoPR:		return _T("피킹출고");
	case enJobTypeAutoMove:		return _T("이동");
	case enJobTypeAutoW2W:		return _T("창고간이동");
	case enJobTypeSemiSto:		return _T("반자동 입고");
	case enJobTypeSemiRet:		return _T("반자동 출고");
	case enJobTypeSemiR2R:		return _T("반자동 랙이동");
	case enJobTypeSemiPR:		return _T("반자동 피킹출고");
	case enJobTypeSemiMove:		return _T("반자동 이동");
	case enJobTypeSemiW2W:		return _T("반자동 창고간이동");
	case enJobTypeManual:		return _T("수동작업");
	}

	return CConvert::ToString(nJobType);
}

CString CJobItem::GetJobStatusString(int nJobStatus)
{
	switch (nJobStatus)
	{
	case enJobStatusAll:					return _T("전체");
	case enJobStatusNew:					return _T("신규");

	case enJobStatusComplete:				return _T("완료");
	case enJobStatusArrived:				return _T("도착");
	case enJobStatusCancel:					return _T("취소");

	case enJobStatusErrorEmptyRetrieve:		return _T("공출고 에러");
	case enJobStatusErrorDualStore:			return _T("이중입고 에러");
	case enJobStatusDualStoreRetry:			return _T("이중입고 재지정");

	case enJobStatusCvNew:					return _T("C/V 구동 대기");
	case enJobStatusCvInvoke:				return _T("C/V 구동중");

	case enJobStatusScRequest:				return _T("S/C 구동 요구");
	case enJobStatusScInvoke:				return _T("S/C 구동중");
	case enJobStatusScComplete:				return _T("S/C 작업완료");

//	case enJobStatusRtvInvoke:				return _T("RTV 구동중");
//	case enJobStatusRtvComplete:			return _T("RTV 작업완료");

//	case enJobStatusLifterInvoke:			return _T("Lifter 구동중");
//	case enJobStatusLifterComplete:			return _T("Lifter 작업완료");
//	case enJobStatusLifterCvInvoke:			return _T("Lifter C/V 구동");

//	case enJobStatusLgvInvoke:				return _T("LGV 작업지시");
//	case enJobStatusLgvComplete:			return _T("LGV 작업완료");

	case enJobStatusCompleteRequest:		return _T("완료 요청");
	case enJobStatusRefuseComplete:			return _T("완료보고 실패");

	case enJobStatusError:					return _T("에러");
	case enJobStatusRefuseError:			return _T("에러 실패");

	case enJobStatusRefuseArrived:			return _T("도착보고 실패");

	case enJobStatusResend:					return _T("재전송");
	}

	CString strTemp;
	strTemp.Format(_T("등록되지 않은 작업상태 [%d]"), nJobStatus);
	return strTemp;
}

CString CJobItem::GetTrayTypeString(int nSize)
{
	switch (nSize)
	{
	case enTrayType1:		return _T("C70_27.5Ah 320H");
	case enTrayType2:		return _T("50Ah 320H, 16Ch");
	case enTrayType3:		return _T("AMG(20Ah) 320H");
	case enTrayType4:		return _T("50Ah 320H, 32Ch");
	case enTrayType5:		return _T("H100 320H, 32Ch");
	case enTrayType6:		return _T("TAM 320H, 밀착 16CH");
	case enTrayType7:		return _T("E250, 320H, 32Ch");
	case enTrayType8:		return _T("PSEV 37.5Ah, 320, 16Ch");
	case enTrayType9:		return _T("C33 320H");
	case enTrayTypeA:		return _T("C70_27.5Ah 455H");
	case enTrayTypeB:		return _T("50Ah 455H, 16Ch");
	case enTrayTypeC:		return _T("AMG(20Ah) 455H");
	case enTrayTypeD:		return _T("50Ah 455H, 32Ch");
	case enTrayTypeE:		return _T("H100 455H 32Ch");
	case enTrayTypeF:		return _T("TAM 455H, 밀착 16CH");
	case enTrayTypeG:		return _T("E250, 455H, 32Ch");
	case enTrayTypeH:		return _T("PSEV 320H, 16채널 공트레이");
	case enTrayTypeI:		return _T("C33 455H");
	case enTrayTypeZ:		return _T("PSEV 37.5Ah, 320, 16Ch M");
	}

	return _T("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CJobItem::IsValidWarehouse(int nWarehouse)
{
	switch (nWarehouse)
	{
	case enWarehouse1:
	case enWarehouse2:
	case enWarehouse3:
		return TRUE;
	}

	return FALSE;
}

BOOL CJobItem::IsValidStartStation()
{
	return m_pDoc->m_pStationInfos.IsValidStartStation(GetJobPattern(), m_strStartStn);
}

BOOL CJobItem::IsValidDestStation()
{
	return m_pDoc->m_pStationInfos.IsValidDestStation(GetJobPattern(), m_strDestStn);
}

BOOL CJobItem::IsValidStartLocation()
{
	switch (GetJobPattern())
	{
	case enJobPatternSto:
	case enJobPatternMove:
		return TRUE;
	}

	if (CLib::IsValidLocation(m_nStartWH, m_strStartLoc) == FALSE)
		return FALSE;

	if (CLib::IsDisableLocation(m_nStartWH, m_strStartLoc))
		return FALSE;

	return TRUE;
}

BOOL CJobItem::IsValidDestLocation()
{
	switch (GetJobPattern())
	{
	case enJobPatternRet:
	case enJobPatternMove:
	case enJobPatternPR:
		return TRUE;
	}

	if (CLib::IsValidLocation(m_nDestWH, m_strDestLoc) == FALSE)
		return FALSE;

	if (CLib::IsDisableLocation(m_nDestWH, m_strDestLoc))
		return FALSE;

	return TRUE;
}
// @@@@@
CString CJobItem::GetStartStationString()
{
	CStationInfo* pStation = m_pDoc->GetStationInfo(m_strStartStn);
	if (pStation == NULL) return m_strStartStn;

	CString strStartInfo;
	if (pStation->m_enKind != CStationInfo::enScStation) strStartInfo = pStation->m_strName;
	else strStartInfo.Format(_T("%s [%s]"), pStation->m_strName, m_strStartLoc);
	return strStartInfo;
}
// @@@@@
CString CJobItem::GetDestStationString()
{
	CStationInfo* pStation = m_pDoc->GetStationInfo(m_strDestStn);
	if (pStation == NULL) return m_strDestStn;

	CString strDestInfo;
	if (pStation->m_enKind != CStationInfo::enScStation) 
		strDestInfo = pStation->m_strName;
	else 
		strDestInfo.Format(_T("%s [%s]"), pStation->m_strName, m_strDestLoc);
	return strDestInfo;
}

CString CJobItem::GetDepartTrackString()
{
	CTrackInfo* pTrack = m_pDoc->GetTrackInfo(m_nDepartTrackNum, m_nCvPlcNum);
	return (pTrack != NULL) ? pTrack->GetTrackString() : CConvert::ToString(m_nDepartTrackNum);
}

CString CJobItem::GetArriveTrackString()
{
	CTrackInfo* pTrack = m_pDoc->GetTrackInfo(m_nArriveTrackNum, m_nCvPlcNum);
	return (pTrack != NULL) ? pTrack->GetTrackString() : CConvert::ToString(m_nArriveTrackNum);
}

CString CJobItem::GetCrateKindString(BYTE ucCrateKind)
{
	switch (ucCrateKind)
	{
	case enCrateBIDP:		return _T("BIDP");
	case enCrateIDP:		return _T("IDP");
	}

	return CConvert::ToString((int)ucCrateKind);
}

CString CJobItem::GetCrateStatusString(BYTE ucCrateStatus)
{
	switch (ucCrateStatus)
	{
	case 0:		return _T("없음");
	case 1:		return _T("사용가능");
	case 2:		return _T("세정대상");
	case 3:		return _T("보수대상");
	}

	return CConvert::ToString((int)ucCrateStatus);
}

CString CJobItem::GetGenString(BYTE ucGenCode)
{
	switch (ucGenCode)
	{
	case 11:	return _T("GEN7-IDP");
	case 12:	return _T("GEN7-BIDP");
	case 13:	return _T("GEN7.5-IDP");
	case 14:	return _T("GEN7.5-BIDP");
	case 15:	return _T("GEN8-IDP");
	case 16:	return _T("GEN8-BIDP");
	case 17:	return _T("GEN9-IDP");
	case 18:	return _T("GEN9-BIDP");
	case 19:	return _T("GEN10-IDP");
	case 20:	return _T("GEN10-BIDP");
	}

	return CConvert::ToString((int)ucGenCode);
}

CString CJobItem::GetWarehouseString(int nWarehouse)
{
	switch (nWarehouse)
	{
	case enWarehouse1:	return _T("PSEV");
	}

	return CConvert::ToString(nWarehouse);
}

int CJobItem::ConvertGenCode(CString& strGen, BYTE ucCrateKind)
{
	TRACE(strGen);
	if (strGen.GetLength() < 4)
		return 0;

	switch (ucCrateKind)
	{
	case enCrateIDP:
		switch (_ttoi(strGen.Mid(1,3)))
		{
		case 700:	return 11;
		case 750:	return 13;
		case 800:	return 15;
		case 900:	return 17;
		case 100:	return 19;
		}
		break;

	case enCrateBIDP:
		switch (_ttoi(strGen.Mid(1,3)))
		{
		case 700:	return 12;
		case 750:	return 14;
		case 800:	return 16;
		case 900:	return 18;
		case 100:	return 20;
		}
		break;
	}

	return 0;
}

BOOL CJobItem::IsNewTask()
{
	switch (m_nJobStatus)
	{
	case enJobStatusNew:
	case enJobStatusCvNew:
	case enJobStatusScRequest:
		return TRUE;
	}

	return FALSE;
}

CString CJobItem::GetDateTime()
{
	return m_tTime.Format(_T("%Y-%m-%d %H:%M:%S"));
}
