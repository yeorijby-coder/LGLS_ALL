// ScInfo.cpp: implementation of the CScInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "ScInfo.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CScInfo, CInfo, 0)

CScInfo::CScInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
//	for (int i=0; i<enFloorSize; ++i)
	m_pControl = NULL;
	m_pControl1 = NULL;
	m_pControl2 = NULL;
	m_pControl3 = NULL;
	m_pControl4 = NULL;

	m_pStation = NULL;
	m_nErrorCode = enScErrorCommunication;		

//	m_bInvoke = FALSE;
	m_nPrevErrorCode = 0;
//	m_nInternalLuggNum = 0;
	m_nInternalJobType = 0;
//	m_nDepartTrackNum = 0;
//	m_nArriveTrackNum = 0;
	m_nPrevRoutine = enRoutineStore;
	m_bStoSuspend = FALSE;
	m_bRetSuspend = FALSE;
	m_ucPrevStatus = enWaiting;

	m_tOperStart = CTime(0);	
	m_tOperEnd = CTime(0);	
	m_tErrorStart = CTime(0);	
	m_tErrorEnd = CTime(0);	
	m_tStatusReport = CTime::GetCurrentTime();

	m_nForkType = enSingle;
	m_nDirectionType = enL2R;

	m_nSelectedTab = 0;

	m_nStartLevel = 0;
	m_nEndLevel = 0;

	for (int i = 0 ; i < 2 ; i++)
	{
		m_ScInfo[i].m_bInvoke			= FALSE;
		m_ScInfo[i].m_nLuggNum			= 0;			
		m_ScInfo[i].m_nInternalLuggNum	= 0;
//		m_ScInfo[i].m_ucCmdType			= '0';		
		m_ScInfo[i].m_nInternalJobType	= 0;
		m_ScInfo[i].m_strDepartInfo		= "";	
		m_ScInfo[i].m_strArriveInfo		= "";	
		m_ScInfo[i].m_nDepartTrackNum	= 0;
		m_ScInfo[i].m_nArriveTrackNum	= 0;
	}
	m_bRtv = FALSE;
}

CScInfo::~CScInfo()
{

}

void CScInfo::Initialize()
{
	m_ucRcStatus = enStop;		
	m_ucRcMode = enOff;
	m_ucScMode = enManual;			
	m_ucScStatus = enError;		
	m_wHorizontalPos = 0;	
	m_wVerticalPos = 0;	
	m_ucProdLoad = enProdEmpty;		
	m_ucForkPos1 = enForkPosCenter;		
	m_ucForkPos2 = enForkPosCenter;		
	m_ucForkStatus = enForkReady;		
	m_nErrorCode = enScErrorCommunication;		
//	m_nLuggNum = 0;			
//	m_ucCmdType = '0';		
	m_ucJobStatus = '0';
	m_bModified = TRUE;
}

void CScInfo::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
//		ar	<< m_bInvoke; 
		ar	<< m_bStoSuspend; 
		ar	<< m_bRetSuspend;
		ar	<< m_nPrevErrorCode;
		ar	<< m_nErrorCode;
//		ar	<< m_nInternalLuggNum;
		ar	<< m_nInternalJobType;
//		ar	<< m_nDepartTrackNum;
//		ar	<< m_nArriveTrackNum;
		ar	<< m_tErrorStart;
		ar	<< m_tErrorEnd;
		ar	<< m_tOperStart;
		ar	<< m_tOperEnd;
		ar	<< m_tStatusReport;
		ar	<< m_ucPrevStatus;

		for (int i = 0 ; i < 2 ; i++)
		{
			ar	<< m_ScInfo[i].m_bInvoke			;
			ar	<< m_ScInfo[i].m_nLuggNum			;	
			ar	<< m_ScInfo[i].m_nInternalLuggNum	;
//			ar	<< m_ScInfo[i].m_ucCmdType			;
//			ar	<< m_ScInfo[i].m_nInternalJobType	;
			ar	<< m_ScInfo[i].m_strDepartInfo		;
			ar	<< m_ScInfo[i].m_strArriveInfo		;
			ar	<< m_ScInfo[i].m_nDepartTrackNum	;
			ar	<< m_ScInfo[i].m_nArriveTrackNum	;
		}
	}
	else
	{
//		ar	>> m_bInvoke;
		ar	>> m_bStoSuspend;
		ar	>> m_bRetSuspend;
		ar	>> m_nPrevErrorCode;
		ar	>> m_nErrorCode;
//		ar	>> m_nInternalLuggNum;
		ar	>> m_nInternalJobType;
//		ar	>> m_nDepartTrackNum;
//		ar	>> m_nArriveTrackNum;
		ar	>> m_tErrorStart;
		ar	>> m_tErrorEnd;
		ar	>> m_tOperStart;
		ar	>> m_tOperEnd;
		ar	>> m_tStatusReport;
		ar	>> m_ucPrevStatus;

		for (int i = 0 ; i < 2 ; i++)
		{
			ar	>> m_ScInfo[i].m_bInvoke			;
			ar	>> m_ScInfo[i].m_nLuggNum			;	
			ar	>> m_ScInfo[i].m_nInternalLuggNum	;
//			ar	>> m_ScInfo[i].m_ucCmdType			;
//			ar	>> m_ScInfo[i].m_nInternalJobType	;
			ar	>> m_ScInfo[i].m_strDepartInfo		;
			ar	>> m_ScInfo[i].m_strArriveInfo		;
			ar	>> m_ScInfo[i].m_nDepartTrackNum	;
			ar	>> m_ScInfo[i].m_nArriveTrackNum	;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CScInfo::StatusReport(BOOL bManual /* = FALSE */)
{
	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	int nStatusGbn = 1;
	if (m_pEquipment->IsConnect(0) && IsOnlineMode() && IsAutoMode() && IsActive() && !IsErrorStatus())
	{
		if (m_bStoSuspend && m_bRetSuspend) nStatusGbn = 1;			// 작업불가
		else if (m_bStoSuspend && !m_bRetSuspend) nStatusGbn = 2;	// 입고불가
		else if (m_bRetSuspend && !m_bStoSuspend) nStatusGbn = 3;	// 출고불가
		else nStatusGbn = 0;
	}

}

void CScInfo::ErrorReport(BOOL bManual /* = FALSE */)
{
	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	// 이중입고 공출고는 다르게 보고함
	// 입고장애 및 출고장애는 삼성정밀화학에서는 사용되지 않음 !!(더블 랙에서 사용됨)
	int nErrorKind = 0;
	switch(m_nErrorCode) 
	{
		case enScErrorDualStore:		nErrorKind  = 1;	break;
		case enScErrorEmptyRetrieve:	nErrorKind  = 3; 	break;
	}
	
	// Location 구하기 
	CString strLocation = "";
	if (m_ScInfo[0].m_strDepartInfo.GetLength() >= 6)
		strLocation = m_ScInfo[0].m_strDepartInfo;
		
	if (m_ScInfo[0].m_strArriveInfo.GetLength() >= 6)
		strLocation = m_ScInfo[0].m_strArriveInfo;

	//@@
//	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
//		pDoc->m_pHostCl->Error(1, m_pEquipment->m_nNumber, nErrorKind, m_nErrorCode, m_ScInfo[1].m_nLuggNum, strLocation);
}

//void CScInfo::TroubleReport(BOOL bManual /* = FALSE */)
//{
//	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
//	DEBUGER_ASSERT_VALID(pDoc != NULL);
//
//	if (pDoc->m_pHostSv && pDoc->m_pHostSv->IsConnect())
//		pDoc->m_pHostSv->TroubleReport(2, m_pEquipment->m_nNumber, m_tErrorStart, m_tErrorEnd, m_nErrorCode, m_nInternalLuggNum, GetWarehouseNo());
//}
//
//void CScInfo::LeadTimeReport(BOOL bManual /* = FALSE */)
//{
//	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
//	DEBUGER_ASSERT_VALID(pDoc != NULL);
//
//	if (pDoc->m_pHostSv && pDoc->m_pHostSv->IsConnect())
//		pDoc->m_pHostSv->LeadTimeReport(2, m_pEquipment->m_nNumber, m_tOperStart, m_tOperEnd, m_nInternalLuggNum, GetWarehouseNo());
//}

int CScInfo::GetWarehouseNo()
{
	return m_pStation ? m_pStation->GetWarehouseNo() : 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CScInfo::SetRcStatus(BYTE ucRcStatus)
{
	if (m_ucRcStatus == ucRcStatus)
		return FALSE;

	m_ucRcStatus = ucRcStatus;
	StatusReport();
	m_bModified = TRUE;
//	StatusReport();
	return TRUE;
}

BOOL CScInfo::SetRcMode(BYTE ucRcMode)
{
	if (m_ucRcMode == ucRcMode)
		return FALSE;

	m_ucRcMode = ucRcMode;
	StatusReport();
	m_bModified = TRUE;
//	StatusReport();
	return TRUE;
}

BOOL CScInfo::SetScMode(BYTE ucScMode)
{
	if (m_ucScMode == ucScMode)
		return FALSE;

	m_ucScMode = ucScMode;
	StatusReport();
	m_bModified = TRUE;
//	StatusReport();
	return TRUE;
}

BOOL CScInfo::SetScStatus(BYTE ucScStatus)
{
	if (m_ucScStatus == ucScStatus)
		return FALSE;

	m_ucPrevStatus = m_ucScStatus;
	m_ucScStatus = ucScStatus;
	StatusReport();
	m_bModified = TRUE;
//	StatusReport();
	return TRUE;
}

void CScInfo::SetHorizontalPos(WORD wHorizontalPos)
{
	if(m_nForkType == enTwin)
		wHorizontalPos =	(wHorizontalPos == 1) ? 1 : wHorizontalPos / 2 ;
	//	wHorizontalPos = wHorizontalPos / 2;

	if (m_wHorizontalPos == wHorizontalPos)
		return;

	m_wHorizontalPos = wHorizontalPos;
	m_bModified = TRUE;
}

void CScInfo::SetVerticalPos(WORD wVerticalPos)
{
	if (m_wVerticalPos == wVerticalPos)
		return;

	m_wVerticalPos = wVerticalPos;
	m_bModified = TRUE;
}

void CScInfo::SetProdLoad(BYTE ucProdLoad)
{
	if (m_ucProdLoad == ucProdLoad)
		return;

	m_ucProdLoad = ucProdLoad;
	m_bModified = TRUE;
}

void CScInfo::SetForkPos1(BYTE ucForkPos)
{
	if (m_ucForkPos1 == ucForkPos)
		return;

	m_ucForkPos1 = ucForkPos;
	m_bModified = TRUE;
}

void CScInfo::SetForkPos2(BYTE ucForkPos)
{
	if (m_ucForkPos2 == ucForkPos)
		return;

	m_ucForkPos2 = ucForkPos;
	m_bModified = TRUE;
}

void CScInfo::SetForkStatus(BYTE ucForkStatus)
{
	if (m_ucForkStatus == ucForkStatus)
		return;

	m_ucForkStatus = ucForkStatus;
	m_bModified = TRUE;
}

void CScInfo::SetErrorCode(WORD wErrorCode)
{
	if (m_nErrorCode == wErrorCode)
		return;

	if (wErrorCode == 0 && m_nErrorCode == enScErrorCommunication)
	{
		m_nErrorCode = wErrorCode;
		m_bModified = TRUE;
		return;
	}

	// 에러가 처음 발생했을 경우 
	if ((m_nErrorCode == 0) && (wErrorCode != 0))
	{
		m_tErrorStart = CTime::GetCurrentTime();
		m_tErrorEnd = m_tErrorStart;
		m_nErrorCode = wErrorCode;
		ErrorReport();
	}
	// 에러상태에서 다른 에러가 발생했을 경우(에러코드 변경)
	if ((m_nErrorCode != 0) && (wErrorCode != 0) && (m_nErrorCode != wErrorCode)) 
	{
		m_nErrorCode = wErrorCode;
		ErrorReport();
	}
	//
//	if ((m_nErrorCode != 0) && (wErrorCode == 0) && (m_tErrorEnd == m_tErrorStart))
//	{
//		m_tErrorEnd = COleDateTime::GetCurrentTime();
//		if (IsErrorCode(m_nErrorCode))		// 알람까지 보고하기로함 (2011.05.06)
//			TroubleReport();
//	}
	// 에러 해제시
	if ((m_nErrorCode != 0) && (wErrorCode == 0)) 
	{
//		ReleaseReport();
	}

	m_nErrorCode = wErrorCode;
	m_bModified = TRUE;
}


void CScInfo::SetLuggNum(WORD wLuggNum, int nFork)
{
	if (m_ScInfo[nFork].m_nLuggNum == wLuggNum)
		return;

	m_ScInfo[nFork].m_nLuggNum = wLuggNum;
	m_bModified = TRUE;
}

void CScInfo::SetCmdType(BYTE ucCmdType)
{
	if (m_ucCmdType == ucCmdType)
		return;

	m_ucCmdType = ucCmdType;
	m_bModified = TRUE;
}

void CScInfo::SetDepartInfo(CString& strDepartInfo, int nFork)
{
	if (m_ScInfo[nFork].m_strDepartInfo == strDepartInfo)
		return;

	m_ScInfo[nFork].m_strDepartInfo = strDepartInfo;
	m_bModified = TRUE;
}

void CScInfo::SetArriveInfo(CString& strArriveInfo, int nFork)
{
	if (m_ScInfo[nFork].m_strArriveInfo == strArriveInfo)
		return;

	m_ScInfo[nFork].m_strArriveInfo = strArriveInfo;
	m_bModified = TRUE;
}

void CScInfo::SetJobStatus(BYTE ucJobStatus)
{
	if (m_ucJobStatus == ucJobStatus)
		return;

	m_ucJobStatus = ucJobStatus;
	m_bModified = TRUE;
}
/*
void CScInfo::SetJobCount(BYTE ucJobCount)
{
	if (m_ucJobCount == ucJobCount)
		return;

	m_ucJobCount = ucJobCount;
	m_bModified = TRUE;
}
void CScInfo::SetCompleteNum(int nCompleteNum)
{
	if (m_nCompleteNum == nCompleteNum)
		return;

	m_nCompleteNum = nCompleteNum;
	m_bModified = TRUE;
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CString CScInfo::GetRcStatus()
{
	switch (m_ucRcStatus)
	{
	case enStop:	return _T("STOP");
	case enActive:	return _T("ACTIVE");
	}

	return _T("INVALID");
}

CString CScInfo::GetRcMode()
{
	switch (m_ucRcMode)
	{
	case enOff:				return _T("OFF");
	case enOnline:			return _T("ONLINE");
	case enRemote:			return _T("REMOTE");
//	case enOnlineRemote:	return _T("HP ONLINE, OP REMOTE");
//	case enRemoteOnline:	return _T("HP REMOTE, OP ONLINE");
//	case enOnlineOff:		return _T("HP ONLINE, OP OFF");
//	case enOffOnline:		return _T("HP OFF, OP ONLINE");
//	case enRemoteOff:		return _T("HP REMOTE, OP OFF");
//	case enOffRemote:		return _T("HP OFF, OP REMOTE");
	}

	return _T("INVALID");
}

CString CScInfo::GetScMode()
{
	switch (m_ucScMode)
	{
	case enAuto:	return _T("자동");
	case enManual:	return _T("수동");
	}

	return _T("INVALID");
}

CString CScInfo::GetScStatus()
{
	switch (m_ucScStatus)
	{
	case enIdle:		
	case enWaiting:		return _T("대기");
	case enRunning:		return _T("동작");
	case enError:		return _T("에러");
	}

	return _T("INVALID");
}

CString CScInfo::GetForkPos1()
{
	switch (m_ucForkPos1)
	{
	case enForkPosCenter:	return _T("CENTER");
	case enForkPosBank1:	return _T("BANK1");
	case enForkPosBank2:	return _T("BANK2");
	}

	return _T("INVALID");
}

CString CScInfo::GetForkPos2()
{
	switch (m_ucForkPos2)
	{
	case enForkPosCenter:	return _T("CENTER");
	case enForkPosBank1:	return _T("BANK1");
	case enForkPosBank2:	return _T("BANK2");
	}

	return _T("INVALID");
}

CString CScInfo::GetForkStatus()
{
	switch (m_ucForkStatus)
	{
	case enForkReady:				return _T("정상");
	case enForkDualStoreError:		return _T("이중입고");
	case enForkEmptyRetrieveError:	return _T("공출고");
	}

	return _T("INVALID");
}

CString CScInfo::GetProdLoad()
{
	switch (m_ucProdLoad)
	{
	case enProdEmpty:		return _T("없음");
	case enProdExistFork1:	
		if (m_nForkType==0) return _T("있음");
		else				return _T("Frok #1 화물유");
	case enProdExistFork2:	return _T("Frok #2 화물유");
	case enProdExistFork12:	return _T("Frok #1,#2화물유");
	}

	return _T("INVALID");
}

CString CScInfo::GetCmdType()
{
	switch (m_ucCmdType)
	{
	case enCmdNone:		return _T("없음");
	case enStore:		return _T("입고");
	case enRetrieve:	return _T("출고");
	case enHs2Hs:		return _T("직출고");
	case enRack2Rack:	return _T("재배치");
	case enHomeReturn:	return _T("홈복귀");
	case enMove:		return _T("이동");
	case enDualStore:	return _T("이중입고재지정");
	}

	return _T("INVALID");
}

CString CScInfo::GetJobStatus()
{
	switch (m_ucJobStatus)
	{
//	case '6':
//	case '7':
//	case '9':				return _T("삭제");
//	case enReady:			return _T("준비");
	case enMoving:			return _T("");
//	case enLoad:			return _T("화물로딩");
//	case enMoveDest:		return _T("도착지 이동");
//	case enUnload:			return _T("화물언로딩");
	case en1ForkComplete:	return _T("Fork#1 완료");
	case en2ForkComplete:	return _T("Fork#2 완료");
	case enComplete:		return _T("#1, #2 완료");
	}

	return _T("INVALID");
}

CString CScInfo::GetPrevStatus()
{
	switch (m_ucPrevStatus)
	{
	case enIdle:		
	case enWaiting:		return _T("대기");
	case enRunning:		return _T("동작");
	case enError:		return _T("에러");
	}

	return _T("INVALID");
}
CString CScInfo::GetDepartInfoString()
{
	CString strDepartInfo;
	switch (m_ucCmdType)
	{
	case enCmdNone:
		return _T("");

	case enStore:
	case enHs2Hs:
		strDepartInfo.Format(_T("HS#%d"), GetDepartHS1());
		return strDepartInfo;

	case enRetrieve:
	case enRack2Rack:
		return GetDepartLoc1();
	}

	return m_ScInfo[0].m_strDepartInfo;
}

CString CScInfo::GetDepartInfoString2()
{
	CString strDepartInfo;
	switch (m_ucCmdType)
	{
	case enCmdNone:
		return _T("");

	case enStore:
	case enHs2Hs:
		strDepartInfo.Format(_T("HS#%d"), GetDepartHS2());
		return strDepartInfo;

	case enRetrieve:
	case enRack2Rack:
		return GetDepartLoc2();
	}

	return m_ScInfo[1].m_strDepartInfo;
}

CString CScInfo::GetArriveInfoString()
{
	CString strArriveInfo;
	switch (m_ucCmdType)
	{
	case enCmdNone:
		return _T("");

	case enStore:
	case enRack2Rack:
		return GetArriveLoc1();

	case enHs2Hs:
	case enRetrieve:
		strArriveInfo.Format(_T("HS#%d"), GetArriveHS1());
		return strArriveInfo;
	}

	return m_ScInfo[0].m_strArriveInfo;
}

CString CScInfo::GetArriveInfoString2()
{
	CString strArriveInfo;
	switch (m_ucCmdType)
	{
	case enCmdNone:
		return _T("");

	case enStore:
	case enRack2Rack:
		return GetArriveLoc2();

	case enHs2Hs:
	case enRetrieve:
		strArriveInfo.Format(_T("HS#%d"), GetArriveHS2());
		return strArriveInfo;
	}

	return m_ScInfo[1].m_strArriveInfo;
}

CString CScInfo::GetXmlString()
{
	CString strXml;
	strXml.Format(_T("<INFO FKTP='%d' RCS='%d' RCM='%d' SCM='%d' SCS='%d' HPOS='%d' VPOS='%d' PSTS='%d' FPOS1='%d' FPOS2='%d' FSTS='%d' ERR='%d' LNO1='%d' LNO2='%d' JT='%d' DI1='%s' DI2='%s' AI1='%s' AI2='%s' JSTS='%d' INVK1='%d' INVK2='%d' ILNO1='%d' ILNO2='%d' IJT1='%d' IJT2='%d' STOS='%d' RETS='%d'/>"), 
		m_nForkType, m_ucRcStatus, m_ucRcMode, m_ucScMode, m_ucScStatus, m_wHorizontalPos, m_wVerticalPos, m_ucProdLoad, m_ucForkPos1, m_ucForkPos2, m_ucForkStatus,
		m_nErrorCode, m_ScInfo[0].m_nLuggNum, m_ScInfo[1].m_nLuggNum, m_ucCmdType, m_ScInfo[0].m_strDepartInfo, m_ScInfo[1].m_strDepartInfo, 
		m_ScInfo[0].m_strArriveInfo, m_ScInfo[1].m_strArriveInfo, m_ucJobStatus, m_ScInfo[0].m_bInvoke, m_ScInfo[1].m_bInvoke, 
		m_ScInfo[0].m_nInternalLuggNum, m_ScInfo[1].m_nInternalLuggNum, m_ScInfo[0].m_nInternalJobType, m_ScInfo[1].m_nInternalJobType, m_bStoSuspend, m_bRetSuspend
	);
	return strXml;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
COLORREF CScInfo::GetForkColor1()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_nErrorCode == enScErrorCommunication)
		return pConfig->m_clrDisconnect;

	if (IsErrorStatus())
		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;

	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActive() == FALSE))
		return DARK_GRAY;

	if (m_ucCmdType == enHomeReturn)
		return pConfig->m_clrManual;

//	if (IsInvoked() == FALSE)
//		return LIGHT_GRAY;

	switch (m_ucCmdType)		
	{
	case enJobTypeAutoSto:		return pConfig->m_clrAutoSto;
	case enJobTypeAutoRet:		return pConfig->m_clrAutoRet;
	case enJobTypeAutoR2R:		return pConfig->m_clrAutoR2R;
	case enJobTypeAutoMove:		return pConfig->m_clrAutoMove;
	case enJobTypeAutoPR:		return pConfig->m_clrAutoRet;//pConfig->m_clrAutoW2W;
	case enJobTypeSemiSto:		return pConfig->m_clrSemiSto;
	case enJobTypeSemiRet:		return pConfig->m_clrSemiRet;
	case enJobTypeSemiR2R:		return pConfig->m_clrSemiR2R;
	case enJobTypeSemiMove:		return pConfig->m_clrSemiMove;
	case enJobTypeSemiPR:		return pConfig->m_clrSemiRet;//pConfig->m_clrSemiW2W;
	case enJobTypeManual:		return pConfig->m_clrManual;
	}

	if (m_ScInfo[1].m_nInternalJobType == 0)
		return LIGHT_GRAY;

	return BLACK;
}

COLORREF CScInfo::GetForkColor2()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_nErrorCode == enScErrorCommunication)
		return pConfig->m_clrDisconnect;

	if (IsErrorStatus())
		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;

	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActive() == FALSE))
		return DARK_GRAY;

	if (m_ucCmdType == enHomeReturn)
		return pConfig->m_clrManual;

//	if (IsInvoked() == FALSE)
//		return LIGHT_GRAY;

	switch (m_ucCmdType)		
	{
	case enJobTypeAutoSto:		return pConfig->m_clrAutoSto;
	case enJobTypeAutoRet:		return pConfig->m_clrAutoRet;
	case enJobTypeAutoR2R:		return pConfig->m_clrAutoR2R;
	case enJobTypeAutoMove:		return pConfig->m_clrAutoMove;
	case enJobTypeAutoPR:		return pConfig->m_clrAutoRet;//pConfig->m_clrAutoW2W;
	case enJobTypeSemiSto:		return pConfig->m_clrSemiSto;
	case enJobTypeSemiRet:		return pConfig->m_clrSemiRet;
	case enJobTypeSemiR2R:		return pConfig->m_clrSemiR2R;
	case enJobTypeSemiMove:		return pConfig->m_clrSemiMove;
	case enJobTypeSemiPR:		return pConfig->m_clrSemiRet;//pConfig->m_clrSemiW2W;
	case enJobTypeManual:		return pConfig->m_clrManual;
	}

	if (m_ScInfo[0].m_nInternalJobType != 0)
		return LIGHT_GRAY;

	return BLACK;
}


COLORREF CScInfo::GetForkColor()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_nErrorCode == enScErrorCommunication)
		return pConfig->m_clrDisconnect;

	if (IsErrorStatus())
		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;

	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActive() == FALSE))
		return DARK_GRAY;

	if (m_ucCmdType == enHomeReturn)
		return pConfig->m_clrManual;

//	if (IsInvoked() == FALSE)
//		return LIGHT_GRAY;

	switch (m_ucCmdType)		// 2번 포크는 생각하지 않음!!
	{
	case enJobTypeAutoSto:		return pConfig->m_clrAutoSto;
	case enJobTypeAutoRet:		return pConfig->m_clrAutoRet;
	case enJobTypeAutoR2R:		return pConfig->m_clrAutoR2R;
	case enJobTypeAutoMove:		return pConfig->m_clrAutoMove;
	case enJobTypeAutoPR:		return pConfig->m_clrAutoRet;//pConfig->m_clrAutoW2W;
	case enJobTypeSemiSto:		return pConfig->m_clrSemiSto;
	case enJobTypeSemiRet:		return pConfig->m_clrSemiRet;
	case enJobTypeSemiR2R:		return pConfig->m_clrSemiR2R;
	case enJobTypeSemiMove:		return pConfig->m_clrSemiMove;
	case enJobTypeSemiPR:		return pConfig->m_clrSemiRet;//pConfig->m_clrSemiW2W;
	case enJobTypeManual:		return pConfig->m_clrManual;
	}

	return BLACK;
}

COLORREF CScInfo::GetRailColor()
{
	if (m_bStoSuspend || m_bRetSuspend)
		return m_pEquipment->m_pDoc->m_pConfig->m_clrSuspend;

	return IsInvoked() ? BLUE : BLACK;
}

COLORREF CScInfo::GetPostColor()
{
	if (m_nForkType == 1 || m_nForkType == 2)
	{
		return (m_ucProdLoad != enProdEmpty) ? BLUE : DARK_GRAY;	
	}			
	else
		return (m_ucProdLoad == enProdExistFork1) ? BLUE : DARK_GRAY;
//*/
}

void CScInfo::InvokeControl()
{
	if (m_bModified == FALSE)
		return;

	CString strTemp;
	DEBUGER_ASSERT_VALID(m_pControl != NULL);
	m_wHorizontalPos;
	BOOL bErase = (m_pControl->m_nForkPos != m_wHorizontalPos);
	m_pControl->m_nForkPos = (m_wHorizontalPos >= 254 || m_wHorizontalPos < 1) ? 0 : m_wHorizontalPos;

	if (m_nForkType == enTwin  || m_nForkType == enDouble)
	{
		// 원본
		m_pControl->m_clrFork2 = m_pControl->m_clrFork = GetForkColor1();

		if (m_ucProdLoad == 1)		
			m_ucProdLoad = 2;
		else if (m_ucProdLoad == 2)	
			m_ucProdLoad = 1;
	}
	else
	{
		m_pControl->m_clrFork = GetForkColor1();
	}

	m_pControl->m_clrRail = GetRailColor();

	m_pControl->m_nProd = m_ucProdLoad;
	m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);

	m_pEquipment->MulticastInfo(NULL);
	m_pEquipment->RefreshDialog();

	m_bModified = FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CScInfo::GetDepartHS(CString strDepartInfo)
{
	return _ttoi(strDepartInfo.Right(2));
}

int CScInfo::GetArriveHS(CString strArriveInfo)
{
	return _ttoi(strArriveInfo.Right(2));
}

CString CScInfo::GetDepartLoc(CString strDepartInfo)
{
	if (strDepartInfo.GetLength() != 8)
	{
		//DEBUGER_ASSERT_VALID(FALSE);
		return _T("000000");
	}

	CString strLoc;
	strLoc.Format(_T("%02d%03d%02d"), _ttoi(strDepartInfo.Left(2)), _ttoi(strDepartInfo.Mid(2,2)), _ttoi(strDepartInfo.Mid(4,2)));
	return strLoc;
}

CString CScInfo::GetArriveLoc(CString strArriveInfo)
{
	if (strArriveInfo.GetLength() != 8)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return _T("000000");
	}

	CString strLoc;
	strLoc.Format(_T("%02d%03d%02d"), _ttoi(strArriveInfo.Left(2)), _ttoi(strArriveInfo.Mid(2,2)), _ttoi(strArriveInfo.Mid(4,2)));
	return strLoc;
}

CString CScInfo::GetErrorString()
{
	CString strErrorCode, strErrorMessage;
	strErrorCode.Format(_T("%d"), m_nErrorCode);
	strErrorMessage.Format(_T("등록되지 않은 에러코드 [CODE=%d]"), m_nErrorCode);

	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("SC_ERROR_CODE"), strErrorCode, (LPCWSTR)strErrorMessage, szTemp, _MAX_PATH, ECS_CODE_FILE);
	strErrorMessage.Format(_T("%s"), szTemp);

	return strErrorMessage;
}

BOOL CScInfo::IsAlarmCode(int nErrorCode)
{
	CString strErrorCode;
	strErrorCode.Format(_T("%d"), nErrorCode);
	return (BOOL)::GetPrivateProfileInt(_T("SC_ALARM_CODE"), strErrorCode, 0, ECS_CODE_FILE);
}

BOOL CScInfo::IsErrorCode(int nErrorCode)
{
	return ((nErrorCode != 0) && (IsAlarmCode(nErrorCode) == FALSE));
}
