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
//	m_nCycleFlowIndex = 0;
	m_pControl = NULL;
	m_pStation = NULL;

	m_nPrevErrorCode = 0;
	m_wPrevErrCode = 0;
	m_wErrCode = 0;
	m_nPrevRoutine = enRoutineStore;
	m_bStoSuspend = FALSE;
	m_bRetSuspend = FALSE;
	m_bR2RSuspend = FALSE;
	m_ucPrevStatus = enWaiting;

	m_tOperStart = CTime(0);	
	m_tOperEnd = CTime(0);	
	m_tErrorStart = CTime(0);	
	m_tErrorEnd = CTime(0);	
	m_tStatusReport = CTime::GetCurrentTime();

	m_nForkType = enSingle;
//	m_nCycleType = enFullCycle;
//	m_nCycleStep = enHome;

//	m_bEmergencyStoped = FALSE;
//	m_bBeltMoved = FALSE;
//	m_nCycleFlowIndex = 0;
//	m_bActive = FALSE;
//	m_bCycleInvoke = FALSE;

	m_nEquipNum = 0;

	m_bFirstDoorCheck = TRUE;

	for (int i = 0 ; i < 2 ; i++)
	{
		m_ScInfo[i].m_bInvoke			= FALSE;
		m_ScInfo[i].m_nLuggNum			= 0;			
		m_ScInfo[i].m_nInternalLuggNum	= 0;
		m_ScInfo[i].m_nInternalJobType	= 0;
		m_ScInfo[i].m_strDepartInfo		= "";	
		m_ScInfo[i].m_strArriveInfo		= "";	
		m_ScInfo[i].m_nDepartTrackNum	= 0;
		m_ScInfo[i].m_nArriveTrackNum	= 0;
		m_ScInfo[i].m_nFromHsNum		= 0;
		m_ScInfo[i].m_nToHsNum			= 0;
	}
}

CScInfo::~CScInfo()
{

}

void CScInfo::Initialize()
{
//	m_ucRcStatus = enStop;		
//	m_ucRcMode = enOff;
//	m_ucScMode = enManual;			
//	m_ucScStatus = enError;		
	m_wHorizontalPos = 0;	
	m_wVerticalPos = 0;	
	m_ucProdLoad = enProdEmpty;		
	m_ucForkPos = enForkPosCenter;		
//	m_ucForkStatus = enForkReady;		
	m_nErrorCode = enScErrorCommunication;		
//	m_ucJobStatus = '0';
	m_bModified = TRUE;
//	m_bActive = FALSE;
}

void CScInfo::Serialize(CArchive& ar)
{
//		m_nEquipNum, m_wHorizontalPos, m_wVerticalPos, m_ucProdLoad, m_ucForkPos, m_wScJobStatus, m_wScDoorStatus,
//		m_nErrorCode, m_ScInfo[0].m_nLuggNum, m_ScInfo[1].m_nLuggNum, m_ucCmdType, m_ScInfo[0].m_strDepartInfo, m_ScInfo[1].m_strDepartInfo, 
//		m_ScInfo[0].m_strArriveInfo, m_ScInfo[1].m_strArriveInfo, m_ScInfo[0].m_bInvoke, m_ScInfo[1].m_bInvoke, 
//		m_ScInfo[0].m_nInternalLuggNum, m_ScInfo[1].m_nInternalLuggNum, m_ScInfo[0].m_nInternalJobType, m_ScInfo[1].m_nInternalJobType, 
//		m_bStoSuspend, m_bRetSuspend, m_bR2RSuspend

	CTrackHS* pRetHS = NULL;
	int nSize = m_pRetsHS.GetSize();
	int i = 0;

	if (ar.IsStoring())
	{
		ar	<< m_bStoSuspend; 
		ar	<< m_bRetSuspend;
		ar	<< m_bR2RSuspend;
		ar	<< m_nPrevErrorCode;
		ar	<< m_nErrorCode;
		ar	<< m_ucPrevStatus;

		ar	<< m_ucCmdType;
		ar	<< m_wScJobStatus;
		ar	<< m_wScDoorStatus;
		ar	<< m_ucForkPos;
		ar	<< m_ucProdLoad;
		ar	<< m_wVerticalPos;
		ar	<< m_wHorizontalPos;
		ar	<< m_nEquipNum;
		for (i = 0 ; i < 2 ; i++)
		{
			ar	<< m_ScInfo[i].m_bInvoke;
			ar	<< m_ScInfo[i].m_nLuggNum;	
			ar	<< m_ScInfo[i].m_nInternalLuggNum;
			ar	<< m_ScInfo[i].m_nInternalJobType;
			ar	<< m_ScInfo[i].m_strDepartInfo;
			ar	<< m_ScInfo[i].m_strArriveInfo;
			ar	<< m_ScInfo[i].m_nDepartTrackNum;
			ar	<< m_ScInfo[i].m_nArriveTrackNum;
			ar	<< m_ScInfo[i].m_nFromHsNum;
			ar	<< m_ScInfo[i].m_nToHsNum;
		}

		for (i = 0 ; i < nSize ; i++)
		{
			pRetHS = m_pRetsHS.GetAt(i);
			if (pRetHS == NULL)
				continue;

			ar	<< pRetHS->m_nDestStationPos;
		}
		
	}
	else		
	{
		ar	>> m_bStoSuspend;
		ar	>> m_bRetSuspend;
		ar	>> m_bR2RSuspend;
		ar	>> m_nPrevErrorCode;
		ar	>> m_nErrorCode;
		ar	>> m_ucPrevStatus;

		ar	>> m_ucCmdType;
		ar	>> m_wScJobStatus;
		ar	>> m_wScDoorStatus;
		ar	>> m_ucForkPos;
		ar	>> m_ucProdLoad;
		ar	>> m_wVerticalPos;
		ar	>> m_wHorizontalPos;
		ar	>> m_nEquipNum;

		for (i = 0 ; i < 2 ; i++)
		{
			ar	>> m_ScInfo[i].m_bInvoke;
			ar	>> m_ScInfo[i].m_nLuggNum;	
			ar	>> m_ScInfo[i].m_nInternalLuggNum;
			ar	>> m_ScInfo[i].m_nInternalJobType;
			ar	>> m_ScInfo[i].m_strDepartInfo;
			ar	>> m_ScInfo[i].m_strArriveInfo;
			ar	>> m_ScInfo[i].m_nDepartTrackNum;
			ar	>> m_ScInfo[i].m_nArriveTrackNum;
			ar	>> m_ScInfo[i].m_nFromHsNum;
			ar	>> m_ScInfo[i].m_nToHsNum;
		}

		for (i = 0 ; i < nSize ; i++)
		{
			pRetHS = m_pRetsHS.GetAt(i);
			if (pRetHS == NULL)
				continue;

			ar	>> pRetHS->m_nDestStationPos;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CScInfo::StatusReport(BOOL bManual /* = FALSE */)
{
	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	int nStatusGbn = 1;
	if (m_pEquipment->IsConnect() && IsScJobStatusScRemoteMode() && IsScJobStatusRcRemoteMode() &&  !IsScJobStatusError())
	{

		// @.@ -> RackToRack 금지도 쓸것인지?
		if (m_bStoSuspend && m_bRetSuspend ) nStatusGbn = 1;		// 작업불가
		else if (m_bStoSuspend && !m_bRetSuspend) nStatusGbn = 2;	// 입고불가
		else if (m_bRetSuspend && !m_bStoSuspend) nStatusGbn = 3;	// 출고불가
		else nStatusGbn = 0;
	}

	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
		pDoc->m_pHostCl->Status();
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
	case enScErrorDualStore:			nErrorKind  = 1; 			break;
	case enScErrorEmptyRetrieve:		nErrorKind  = 3; 			break;
	}

	// Location 구하기 
	CString strLocation = "";
	if (_ttoi(m_ScInfo[0].m_strDepartInfo.Right(2)) == 0 )
		strLocation = m_ScInfo[0].m_strDepartInfo.Left(6);
		
	if (_ttoi(m_ScInfo[0].m_strArriveInfo.Right(2)) == 0 )
		strLocation = m_ScInfo[0].m_strArriveInfo.Left(6);

	/*
//=====================================================================================================
//	// 경광등 관련
//=====================================================================================================	
//	BOOL WriteLampStatus(int nIndex, BOOL bOn) { return WriteWordValue(nIndex + 1, (WORD)0x0001 & bOn); }
	CCvFx* pCv = (CCvFx*)pDoc->GetEquipment(CEquipment::enCV, 12);	// 12번이 경광등 임
	if (pCv == NULL)
		return;

	switch(m_nEquipNum) 
	{
	case 1:
	case 2:		pCv->WriteLampStatus(0, (m_nErrorCode!=0));		break;
	case 3:
	case 4:		pCv->WriteLampStatus(1, (m_nErrorCode!=0));		break;
	case 5:
	case 6:		pCv->WriteLampStatus(2, (m_nErrorCode!=0));		break;
	}
//-----------------------------------------------------------------------------------------------------
*/
	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
		pDoc->m_pHostCl->Error(1, m_nEquipNum, nErrorKind, m_nErrorCode, m_ScInfo[0].m_nInternalLuggNum, strLocation);
}


//void CScInfo::TroubleReport(BOOL bManual /* = FALSE */)
//{
//	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
//	DEBUGER_ASSERT_VALID(pDoc != NULL);
//
//	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
//		pDoc->m_pHostCl->TroubleReport(2, m_pEquipment->m_nNumber, m_tErrorStart, m_tErrorEnd, m_nErrorCode, m_nInternalLuggNum, GetWarehouseNo());
//}
//
//void CScInfo::LeadTimeReport(BOOL bManual /* = FALSE */)
//{
//	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
//	DEBUGER_ASSERT_VALID(pDoc != NULL);
//
//	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
//		pDoc->m_pHostCl->LeadTimeReport(2, m_pEquipment->m_nNumber, m_tOperStart, m_tOperEnd, m_nInternalLuggNum, GetWarehouseNo());
//}

int CScInfo::GetWarehouseNo()
{
	return m_pStation ? m_pStation->GetWarehouseNo() : 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
BOOL CScInfo::SetInvoked()
{
	BOOL bOld = m_bInvoke;
	m_bInvoke = TRUE;

	if(bOld == FALSE)	m_bModified = TRUE;

	return bOld;
}
BOOL CScInfo::ResetInvoke()
{
	BOOL bOld = m_bInvoke;
	m_bInvoke = FALSE;

	if(bOld)	m_bModified = TRUE;

	return bOld;
}

BOOL CScInfo::SetHorizontalPos(BYTE cPos)
{
	if (cPos == m_ucHorizontalPos)
		return false;

	m_ucHorizontalPos = cPos;
	m_bModified = TRUE;

	return true;
}

BOOL CScInfo::SetVerticalPos(BYTE cPos)
{
	if (cPos == m_ucVerticalPos)
		return false;

	m_ucVerticalPos = cPos;
	m_bModified = TRUE;

	return true;
}

BOOL CScInfo::SetFirstForkPos(BYTE cPos)
{
	if (cPos == m_ucFirstForkPos)
		return false;

	m_ucFirstForkPos = cPos;
	m_bModified = TRUE;
	return true;
}

void CScInfo::SetRcStatus(BYTE ucRcStatus)
{
	if (m_ucRcStatus == ucRcStatus)
		return;

	m_ucRcStatus = ucRcStatus;
	m_bModified = TRUE;
	StatusReport();
}
void CScInfo::SetRcMode(BYTE ucRcMode)
{
	if (m_ucRcMode == ucRcMode)
		return;

	m_ucRcMode = ucRcMode;
	m_bModified = TRUE;
	StatusReport();
}

void CScInfo::SetScMode(BYTE ucScMode)
{
	if (m_ucScMode == ucScMode)
		return;

	m_ucScMode = ucScMode;
	m_bModified = TRUE;
	StatusReport();
}

void CScInfo::SetScStatus(BYTE ucScStatus)
{
	if (m_ucScStatus == ucScStatus)
		return;

	m_ucPrevStatus = m_ucScStatus;
	m_ucScStatus = ucScStatus;
	m_bModified = TRUE;
	StatusReport();
}


*/


void CScInfo::SetHorizontalPos(WORD wHorizontalPos)
{
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

void CScInfo::SetForkPos(BYTE ucForkPos)
{
	if (m_ucForkPos == ucForkPos)
		return;

	m_ucForkPos = ucForkPos;
	m_bModified = TRUE;
}

//=====================================================================================================
//	// 경광등 관련
//=====================================================================================================	
void CScInfo::SetLampInfo(BOOL bOn)
{
	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
	if (pDoc == NULL)
		return;

	CCvFx* pCv = (CCvFx*)pDoc->GetEquipment(CEquipment::enCV, 12);	// 12번이 경광등 임
	if (pCv == NULL)
		return;

	int nIndex = 0;

	switch(m_nEquipNum) 
	{
	case 1:
	case 2:		nIndex = 0;		break;
	case 3:
	case 4:		nIndex = 1;		break;
	case 5:
	case 6:		nIndex = 2;		break;
	}

	CLib::SetBit(pCv->m_pInfo->m_wCurrLampStatus, nIndex, bOn);
}
//-----------------------------------------------------------------------------------------------------

void CScInfo::SetErrorCode(WORD wErrorCode)
{
//	if (m_nErrorCode == wErrorCode)			// 항상 에러상태를 체크해서 경광등을 올려야 하므로...
//		return;

	// 에러가 처음 발생했을 경우 
	if ((m_nErrorCode == 0) && (wErrorCode != 0))
	{
		m_tErrorStart = CTime::GetCurrentTime();
		m_tErrorEnd = m_tErrorStart;
		m_nErrorCode = wErrorCode;
		m_bModified = TRUE;
		ErrorReport();
		SetLampInfo(TRUE);
		return;
	}

	// 에러상태에서 다른 에러가 발생했을 경우(에러코드 변경)
//	else 
	if ((m_nErrorCode != 0) && (wErrorCode != 0) && (m_nErrorCode != wErrorCode)) 
	{
		m_nErrorCode = wErrorCode;
		m_bModified = TRUE;
		ErrorReport();
		return;
	}

	// 에러 해제 되었을경우
//	else 
	if ((m_nErrorCode != 0) && (wErrorCode == 0) && (m_tErrorEnd == m_tErrorStart))
	{
		m_tErrorEnd = CTime::GetCurrentTime();
		SetLampInfo(FALSE);
	}
//	else
//	{
//		SetLampInfo(BOOL(wErrorCode));
//	}
	// 프로그램이 처음 실행 되었을 때 
	if (m_bFirstDoorCheck == TRUE)
	{
		SetLampInfo(BOOL(wErrorCode));
		m_bFirstDoorCheck = FALSE;
	}

	if (m_nErrorCode == wErrorCode)			
		return;

	SetPrevErrorCode(m_nErrorCode);
	m_nErrorCode = wErrorCode;
	m_bModified = TRUE;
}

void CScInfo::SetScJobStatus(WORD wScJobStatus)
{
	if (m_wScJobStatus == wScJobStatus)
		return;

	m_wScJobStatus = wScJobStatus;
	m_bModified = TRUE;
}

void CScInfo::SetScDoorStatus(WORD wScDoorStatus)
{
	if (m_wScDoorStatus == wScDoorStatus)
		return;

	m_wScDoorStatus = wScDoorStatus;
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
/*
void CScInfo::SetForkStatus(BYTE ucForkStatus)
{
	if (m_ucForkStatus == ucForkStatus)
		return;

	m_ucForkStatus = ucForkStatus;
	m_bModified = TRUE;
}

void CScInfo::SetForkStatus2(BYTE ucForkStatus2)
{
	if (m_ucForkStatus2 == ucForkStatus2)
		return;

	m_ucForkStatus2 = ucForkStatus2;
	m_bModified = TRUE;
}


void CScInfo::SetJobStatus(BYTE ucJobStatus)
{
	if (m_ucJobStatus == ucJobStatus)
		return;

	m_ucJobStatus = ucJobStatus;
	m_bModified = TRUE;
}

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

CString CScInfo::GetJobStatus()
{
	switch (m_ucJobStatus)
	{
//	case '6':
//	case '7':
//	case '9':			return _T("삭제");
//	case enReady:		return _T("준비");
	case enMoving:		return _T("");
//	case enLoad:		return _T("화물로딩");
//	case enMoveDest:	return _T("도착지 이동");
//	case enUnload:		return _T("화물언로딩");
	case enComplete:	return _T("완료");
	}

	return _T("INVALID");
}
CString CScInfo::GetForkStatus()
{
//	if (m_nCycleType == enHalfCycle)
//	{
//		if (m_ucForkStatus == enForkReady) 
//			return _T("정상") ;
//		else
//			return _T("에러");
//	}

	switch (m_ucForkStatus)
	{
	case enForkReady:				return _T("정상");
	case enForkDualStoreError:		return _T("에러");		//return _T("이중입고");
//	case enForkEmptyRetrieveError:	return _T("공출고");
	}


	return _T("INVALID");
}

CString CScInfo::GetForkStatus2()
{
//	if (m_nCycleType == enHalfCycle)
//	{
//		if (m_ucForkStatus2 == enForkReady) 
//			return _T("정상") ;
//		else
//			return _T("에러");
//	}

	switch (m_ucForkStatus2)
	{
	case enForkReady:				return _T("정상");
	case enForkDualStoreError:		return _T("에러");		//return _T("이중입고");
//	case enForkEmptyRetrieveError:	return _T("공출고");
	}


	return _T("INVALID");
}


CString CScInfo::GetForkPos()
{
	switch (m_ucForkPos)
	{
	case enForkPosCenter:	return _T("CENTER");
	case enForkPosBank1:	return _T("BANK1");
	case enForkPosBank2:	return _T("BANK2");
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
	case enProdExistFork12:	return _T("Frok #12 화물유");
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


*/

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


CString CScInfo::GetDepartInfoString()
{
	CString strDepartInfo;
	switch (m_ucCmdType)
	{
	case enCmdNone:
		return _T("");

	case enStore:
	case enHs2Hs:
		strDepartInfo.Format(_T("%s [HS#%d]"), GetDepartLoc1(), GetDepartHS1());
		return strDepartInfo;

	case enRetrieve:
	case enRack2Rack:
	case enHomeReturn:
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
		strDepartInfo.Format(_T("%s [HS#%d]"), GetDepartLoc2(), GetDepartHS2());
		return strDepartInfo;

	case enRetrieve:
	case enRack2Rack:
	case enHomeReturn:
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
	case enHomeReturn:
		return GetArriveLoc1();

	case enHs2Hs:
	case enRetrieve:
		strArriveInfo.Format(_T("%s [HS#%d]"), GetArriveLoc1(), GetArriveHS1());
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
	case enHomeReturn:
		return GetArriveLoc2();

	case enHs2Hs:
	case enRetrieve:
		strArriveInfo.Format(_T("%s [HS#%d]"), GetArriveLoc2(), GetArriveHS2());
		return strArriveInfo;
	}

	return m_ScInfo[1].m_strArriveInfo;
}

CString CScInfo::GetXmlString()
{
	CString strXml;
	strXml.Format(_T("<INFO NUM='%d' HPOS='%d' VPOS='%d' PSTS='%d' FPOS='%d' SJS='%d' SDS='%d'"
					   " ERR='%d' LNO1='%d' LNO2='%d' JT='%d' DI1='%s' DI2='%s'" 
					   " AI1='%s' AI2='%s' INVK1='%d' INVK2='%d' "
					   " ILNO1='%d' ILNO2='%d' IJT1='%d' IJT2='%d'"
					   " STOS='%d' RETS='%d' R2RS='%d'/>"), 
		m_nEquipNum, m_wHorizontalPos, m_wVerticalPos, m_ucProdLoad, m_ucForkPos, m_wScJobStatus, m_wScDoorStatus,
		m_nErrorCode, m_ScInfo[0].m_nLuggNum, m_ScInfo[1].m_nLuggNum, m_ucCmdType, m_ScInfo[0].m_strDepartInfo, m_ScInfo[1].m_strDepartInfo, 
		m_ScInfo[0].m_strArriveInfo, m_ScInfo[1].m_strArriveInfo, m_ScInfo[0].m_bInvoke, m_ScInfo[1].m_bInvoke, 
		m_ScInfo[0].m_nInternalLuggNum, m_ScInfo[1].m_nInternalLuggNum, m_ScInfo[0].m_nInternalJobType, m_ScInfo[1].m_nInternalJobType, 
		m_bStoSuspend, m_bRetSuspend, m_bR2RSuspend
	);
	return strXml;

//	WORD m_wScJobStatus;		// SCJobStatus		D100, D120, D140

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
COLORREF CScInfo::GetForkColor1()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_nErrorCode == enScErrorCommunication)
		return pConfig->m_clrDisconnect;

	if (IsScJobStatusError())
	{
		int nPattern = CJobItem::ConvertJobTypeToPattern(m_ScInfo[0].m_nInternalJobType);
		switch(nPattern)
		{
		case enJobPatternSto:	return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrScStoError;	
		case enJobPatternRet:	return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrScRetError;	
		}

		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;
	}

	if (m_nErrorCode != 0)
		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;

//	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActiveMode() == FALSE))
//		return DARK_GRAY;

	// 동진물류 S/C
	if ((IsScJobStatusScManualMode() == TRUE) || (IsScJobStatusScAutoMode() == TRUE) ||(IsScJobStatusScRemoteMode() == FALSE) || 
		(IsScJobStatusRcManualMode() == TRUE) || (IsScJobStatusRcAutoMode() == TRUE) ||(IsScJobStatusRcRemoteMode() == FALSE))
		return DARK_GRAY;


	if (IsInvoked() == FALSE)
	{
		if (IsScJobStatusIdle() == FALSE)
			return DARK_GRAY;

		return LIGHT_GRAY;
	}

	switch (m_ScInfo[0].m_nInternalJobType)		
	{
	case enJobTypeAutoSto:		return pConfig->m_clrAutoSto;
	case enJobTypeAutoRet:		return pConfig->m_clrAutoRet;
	case enJobTypeAutoR2R:		return pConfig->m_clrAutoR2R;
	case enJobTypeAutoMove:		return pConfig->m_clrAutoMove;
	case enJobTypeAutoW2W:		return pConfig->m_clrAutoW2W;
	case enJobTypeSemiSto:		return pConfig->m_clrSemiSto;
	case enJobTypeSemiRet:		return pConfig->m_clrSemiRet;
	case enJobTypeSemiR2R:		return pConfig->m_clrSemiR2R;
	case enJobTypeSemiMove:		return pConfig->m_clrSemiMove;
	case enJobTypeSemiW2W:		return pConfig->m_clrSemiW2W;
	case enJobTypeManual:		return pConfig->m_clrManual;
	}

	if (m_ScInfo[1].m_nInternalJobType != 0)
	{
		if (IsScJobStatusIdle() == FALSE)
			return DARK_GRAY;

		return LIGHT_GRAY;
	}

	return BLACK;
}

COLORREF CScInfo::GetForkColor2()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_nErrorCode == enScErrorCommunication)
		return pConfig->m_clrDisconnect;

	if (IsScJobStatusError())
//		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;
	{
		int nPattern = CJobItem::ConvertJobTypeToPattern(m_ScInfo[1].m_nInternalJobType);
		switch(nPattern)
		{
		case enJobPatternSto:	return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrScStoError;	
		case enJobPatternRet:	return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrScRetError;	
		}

		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;
	}

	if (m_nErrorCode != 0)
		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;

//	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActiveMode() == FALSE))
//		return DARK_GRAY;

	// 동진물류 S/C
	if ((IsScJobStatusScManualMode() == TRUE) || (IsScJobStatusScAutoMode() == TRUE) ||(IsScJobStatusScRemoteMode() == FALSE) || 
		(IsScJobStatusRcManualMode() == TRUE) || (IsScJobStatusRcAutoMode() == TRUE) ||(IsScJobStatusRcRemoteMode() == FALSE))
		return DARK_GRAY;

	if (IsScJobStatusIdle() == FALSE)
		return DARK_GRAY;

	if (IsInvoked() == FALSE)
		return LIGHT_GRAY;

	switch (m_ScInfo[1].m_nInternalJobType)		
	{
	case enJobTypeAutoSto:		return pConfig->m_clrAutoSto;
	case enJobTypeAutoRet:		return pConfig->m_clrAutoRet;
	case enJobTypeAutoR2R:		return pConfig->m_clrAutoR2R;
	case enJobTypeAutoMove:		return pConfig->m_clrAutoMove;
	case enJobTypeAutoW2W:		return pConfig->m_clrAutoW2W;
	case enJobTypeSemiSto:		return pConfig->m_clrSemiSto;
	case enJobTypeSemiRet:		return pConfig->m_clrSemiRet;
	case enJobTypeSemiR2R:		return pConfig->m_clrSemiR2R;
	case enJobTypeSemiMove:		return pConfig->m_clrSemiMove;
	case enJobTypeSemiW2W:		return pConfig->m_clrSemiW2W;
	case enJobTypeManual:		return pConfig->m_clrManual;
	}

	if (IsScJobStatusIdle() == FALSE)
		return DARK_GRAY;

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

	if (IsScJobStatusError())
		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;

//	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActiveMode() == FALSE))
//		return DARK_GRAY;
	// 동진물류 S/C
	if ((IsScJobStatusScManualMode() == TRUE) || (IsScJobStatusScAutoMode() == TRUE) ||(IsScJobStatusScRemoteMode() == FALSE) || 
		(IsScJobStatusRcManualMode() == TRUE) || (IsScJobStatusRcAutoMode() == TRUE) ||(IsScJobStatusRcRemoteMode() == FALSE))
		return DARK_GRAY;

	if (IsInvoked() == FALSE)
		return LIGHT_GRAY;

	int nJobType = 0;
	int nJobType1 = m_ScInfo[0].m_nInternalJobType;
	int nJobType2 = m_ScInfo[1].m_nInternalJobType;

	if (nJobType1 > 0 || nJobType2 > 0)
	{
		if (nJobType1 > 0)
			nJobType = nJobType1;
		else if(nJobType2 > 0)
			nJobType = nJobType2;
	}

	switch (m_ScInfo[1].m_nInternalJobType)		
	{
	case enJobTypeAutoSto:		return pConfig->m_clrAutoSto;
	case enJobTypeAutoRet:		return pConfig->m_clrAutoRet;
	case enJobTypeAutoR2R:		return pConfig->m_clrAutoR2R;
	case enJobTypeAutoMove:		return pConfig->m_clrAutoMove;
	case enJobTypeAutoW2W:		return pConfig->m_clrAutoW2W;
	case enJobTypeSemiSto:		return pConfig->m_clrSemiSto;
	case enJobTypeSemiRet:		return pConfig->m_clrSemiRet;
	case enJobTypeSemiR2R:		return pConfig->m_clrSemiR2R;
	case enJobTypeSemiMove:		return pConfig->m_clrSemiMove;
	case enJobTypeSemiW2W:		return pConfig->m_clrSemiW2W;
	case enJobTypeManual:		return pConfig->m_clrManual;
	}

	return BLACK;
}

COLORREF CScInfo::GetRailColor()
{
	if (m_bStoSuspend || m_bRetSuspend || m_bR2RSuspend)
		return m_pEquipment->m_pDoc->m_pConfig->m_clrSuspend;

//	m_ScInfo[0].m_bInvoke = FALSE;
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
}
void CScInfo::InvokeControl()
{
	if (m_bModified == FALSE)
		return;

	DEBUGER_ASSERT_VALID(m_pControl != NULL);
//	BOOL bErase = (m_pControl->m_nForkPos != m_wHorizontalPos);
	BOOL bErase = (m_pControl->m_nForkPos != m_wHorizontalPos-1);
	m_pControl->m_nForkPos = (m_wHorizontalPos >= 254) ? 1 : (m_wHorizontalPos - 1);		// 출고갯수 표현하려면
	// R2L은 반대로 해야 함!!
	m_pControl->m_clrFork = GetForkColor1();
	m_pControl->m_clrFork2 = GetForkColor2();
	m_pControl->m_clrRail = GetRailColor();

	//이해담 sc
	//if(m_pControl->m_clrRail == m_pEquipment->m_pDoc->m_pConfig->m_clrSuspend)
	if(m_pControl->m_clrRail == BLUE || (m_pControl->m_clrRail == RED && m_ScInfo[0].m_nInternalLuggNum != 0))
	{
		m_pControl->m_nFontSize = 9;
		m_pControl->m_strText.Format(_T("%d"), m_ScInfo[0].m_nInternalLuggNum);
	}
	else
	{
		m_pControl->m_nFontSize = m_pEquipment->m_pDoc->m_pConfig->m_nFontSize;
		m_pControl->m_strText.Format(_T("%d"), _ttoi(m_pControl->m_strCID.Right(3)));
	}

//	m_pControl->m_clrWing = GetPostColor();
	m_pControl->m_nProd = IsScJobStatusSensorProduct();
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
		DEBUGER_ASSERT_VALID(FALSE);
		return _T("000000");
	}

	CString strLoc;
	strLoc.Format(_T("%02d-%03d-%02d"), _ttoi(strDepartInfo.Left(2)), _ttoi(strDepartInfo.Mid(2,2)), _ttoi(strDepartInfo.Mid(4,2)));
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
	strLoc.Format(_T("%02d-%03d-%02d"), _ttoi(strArriveInfo.Left(2)), _ttoi(strArriveInfo.Mid(2,2)), _ttoi(strArriveInfo.Mid(4,2)));
	return strLoc;
}

CString CScInfo::GetErrorString()
{
	CString strErrorCode, strErrorMessage;
	strErrorCode.Format(_T("%d"), m_nErrorCode);
//	strErrorMessage.Format(_T("등록되지 않은 에러코드 [CODE=%d]"), m_nErrorCode);
	strErrorMessage = "등록되지 않은 에러코드";

	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("SC_ERROR_CODE"), strErrorCode, (LPCTSTR)strErrorMessage, szTemp, _MAX_PATH, ECS_CODE_FILE);
	CString strTemp, strTempErrorCode;
	strTemp.Format(_T("[CODE=%d]"), m_nErrorCode);
	strTempErrorCode = (m_nErrorCode != 0) ? strTemp : "" ;
	strErrorMessage.Format(_T("%s %s"), szTemp, strTempErrorCode);

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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
BYTE CScInfo::GetCycleLevel()
{
	return m_ucECS2PLCData[5];
}

void CScInfo::SetCycleLevel(BOOL ucCycleLevel)
{
	m_ucECS2PLCData[5] = ucCycleLevel;

	m_bModified = TRUE;
}

BYTE CScInfo::GetCycleBay()
{
	return m_ucECS2PLCData[4];
}

void CScInfo::SetCycleBay(BYTE ucCycleBay)
{
	m_ucECS2PLCData[4] = ucCycleBay;

	m_bModified = TRUE;
}
void CScInfo::SetMainPowerOn				(BOOL bMainPowerOn)
{
	if(m_bMainPowerOn == bMainPowerOn)	return;

	m_bMainPowerOn = bMainPowerOn;
	m_bModified = TRUE;

//	enum EN_RC_STATUS { enStop=0, enActive=1 };																						// D109
//	(m_bMainPowerOn) ? SetRcStatus(enActive)	: SetRcStatus(enStop);
}
void CScInfo::SetMainPower				(BOOL bMainPower)
{
	if(m_bMainPower == bMainPower)	return;

	m_bMainPower = bMainPower;
	m_bModified = TRUE;
}
void CScInfo::SetActiveMode(BOOL bActive)
{
	if(bActive == m_bActive)	return;

	if(m_bActive && IsBitOn(SIG_STARTUP_REQ))	
		SetBit(SIG_STARTUP_REQ, FALSE);

	m_bActive = bActive;
	m_bModified = TRUE;

	(bActive) ? SetRcStatus(enActive)	: SetRcStatus(enStop);
}

void CScInfo::SetCycleOK					(BOOL bCycleOK)
{
	if(bCycleOK == m_bCycleOK)	return;

	if(bCycleOK && IsBitOn(SIG_STARTUP_REQ))	
		SetBit(SIG_STARTUP_REQ, FALSE);

	m_bCycleOK = bCycleOK;
	m_bModified = TRUE;
}

void CScInfo::SetCycleComplete				(BOOL bCycleComplete)
{
	if(IsOnBusy() == FALSE)
	{
		if(m_bCycleComplete == FALSE && bCycleComplete //&& IsBufferEmpty() 
			&& IsBitOn(SIG_COMPLETE_WORK_RECEIVE) == FALSE)
			SetBit(SIG_COMPLETE_WORK_RECEIVE, TRUE);

		if(m_bCycleComplete && bCycleComplete == FALSE) 
			SetBit(SIG_COMPLETE_WORK_RECEIVE, FALSE);
	}

	(bCycleComplete) ? SetJobStatus(enComplete) : SetJobStatus(enMoving);

	if(m_bCycleComplete == bCycleComplete)	
		return;


	m_bCycleComplete = bCycleComplete;
	m_bModified = TRUE;

	
}

void CScInfo::SetError(BOOL bError)
{
	if(bError)
		SetScStatus(enError);
	else
		SetScStatus(enWaiting);


	if(m_bError == bError)
		return;

	if(IsBitOn(SIG_ERROR_RESET))
		SetBit(SIG_ERROR_RESET, FALSE);

	if(bError == FALSE && IsBitOn(SIG_ERROR_RESET))
		SetBit(SIG_ERROR_RESET, FALSE);

	if(bError)
	{
		SetBit(SIG_STARTUP_REQ, FALSE);
	}


	
	m_bError = bError;
	m_bModified = TRUE;
}
void CScInfo::SetForkingEnablePosition	(BOOL bForkingEnablePosition)
{
	if(m_bForkingEnablePosition == bForkingEnablePosition)	return;

	if(IsBitOnPLC(SIG_PLC_ON_WORKING) && bForkingEnablePosition)
	{
		if(IsBitOn(SIG_SELECT_UP_OR_DOWN))
			SetBit(SIG_UNLOAD_OK, TRUE);
		else
			SetBit(SIG_PICKUP_OK, TRUE);
	}

	m_bForkingEnablePosition = bForkingEnablePosition;
	m_bModified = TRUE;
}
void CScInfo::SetFork1OnRight				(BOOL bFork1OnRight)
{
	if(m_bFork1OnRight == bFork1OnRight)	return;

	m_bFork1OnRight = bFork1OnRight;
	m_bModified = TRUE;

	(bFork1OnRight) ? SetForkPos(enForkPosBank1) : SetForkPos(enForkPosCenter);
}

void CScInfo::SetFork1OnLeft				(BOOL bFork1OnLeft)
{
	if(m_bFork1OnLeft == bFork1OnLeft)	return;

	m_bFork1OnLeft = bFork1OnLeft;
	m_bModified = TRUE;

	(bFork1OnLeft) ? SetForkPos(enForkPosBank2) : SetForkPos(enForkPosCenter);
}

void CScInfo::SetFork2OnRight				(BOOL bFork2OnRight)
{
	if(m_bFork2OnRight == bFork2OnRight)	return;

	m_bFork2OnRight = bFork2OnRight;
	m_bModified = TRUE;

	(bFork2OnRight) ? SetForkPos(enForkPosBank1) : SetForkPos(enForkPosCenter);
}

void CScInfo::SetFork2OnLeft				(BOOL bFork2OnLeft)
{
	if(m_bFork2OnLeft == bFork2OnLeft)	return;

	m_bFork2OnLeft = bFork2OnLeft;
	m_bModified = TRUE;

	(bFork2OnLeft) ? SetForkPos(enForkPosBank2) : SetForkPos(enForkPosCenter);
}

void CScInfo::SetErrorOnFork1				(BOOL bErrorOnFork1)
{
	if(m_bErrorOnFork1 == bErrorOnFork1)	return;

	m_bErrorOnFork1 = bErrorOnFork1;
	m_bModified = TRUE;

	(m_bErrorOnFork1)	? SetForkStatus (enForkDualStoreError)	: SetForkStatus (enForkReady);

}

void CScInfo::SetErrorOnFork2				(BOOL bErrorOnFork2)
{
	if(m_bErrorOnFork2 == bErrorOnFork2)	return;

	m_bErrorOnFork2 = bErrorOnFork2;
	m_bModified = TRUE;

	(m_bErrorOnFork2)	? SetForkStatus2(enForkDualStoreError)	: SetForkStatus2(enForkReady);
}

void CScInfo::SetOnLForkingFork1		(BOOL bOnLForkingFork1)
{
	if(bOnLForkingFork1 == m_bOnLForkingFork1) return;

	if(bOnLForkingFork1)
	{
		PermitLoadUnload(TRUE, FALSE);
		PermitLoadUnload(FALSE, FALSE);
	}

	m_bOnLForkingFork1 = bOnLForkingFork1;
	m_bModified = TRUE;
}

void CScInfo::SetOnRForkingFork1		(BOOL bOnRForkingFork1)
{
	if(bOnRForkingFork1 == m_bOnRForkingFork1) return;

	if(bOnRForkingFork1)
	{
		PermitLoadUnload(TRUE, FALSE);
		PermitLoadUnload(FALSE, FALSE);
	}

	m_bOnRForkingFork1 = bOnRForkingFork1;
	m_bModified = TRUE;
}

void CScInfo::SetOnBusy					(BOOL bOnBusy)
{
	if(m_bOnBusy == bOnBusy)	return;

	m_bOnBusy = bOnBusy;
	m_bModified = TRUE;

	(m_bOnBusy)	? SetScStatus(enRunning) : SetScStatus(enWaiting);

	if(bOnBusy == FALSE && GetCycleStep() == SC_CYCLE_UNLOAD)
	{
		CJobItem* pJobItem1 = m_pEquipment->m_pDoc->m_pJob->Find(m_ScInfo[0].m_nInternalLuggNum);
		CJobItem* pJobItem2 = m_pEquipment->m_pDoc->m_pJob->Find(m_ScInfo[1].m_nInternalLuggNum);
		CString strCommandId = "";
		if(pJobItem1 != NULL)	
		{
			strCommandId = pJobItem1->GetCommandId();

			if(m_pEquipment->m_pDoc->m_pHostCl)
			{
				m_pEquipment->m_pDoc->m_pHostCl->ReportSCStatus(SC_STATUS_IDLE, 0, strCommandId);
			}
		}
		else if(pJobItem2 != NULL)
		{
			strCommandId = pJobItem2->GetCommandId();

			if(m_pEquipment->m_pDoc->m_pHostCl)
			{
				m_pEquipment->m_pDoc->m_pHostCl->ReportSCStatus(SC_STATUS_IDLE, 0, strCommandId);
			}
		}
	}
}

void CScInfo::SetProdLoadOnFork(BYTE ucProdLoad)
{
	if(m_ucProdLoad == ucProdLoad)	
		return;

	m_ucProdLoad = ucProdLoad;
	m_bModified = TRUE;

//	CJobItem* pJobItem1 = m_pEquipment->m_pDoc->m_pJob->Find(m_ScInfo[0].m_nInternalLuggNum);
//	if(bProdLoadOnFork1)
//	{
//		if(pJobItem1)
//		{
//			CString strCommandId = "", strCarrierId = "", strSource = "", strDest = "";
//			strCommandId = pJobItem1->GetCommandId();
//			strCarrierId = pJobItem1->GetContainerID();
//			strSource = pJobItem1->GetHostSource();
//			strDest = pJobItem1->GetHostDest();
//
//			if(m_pEquipment->m_pDoc->m_pHostCl)
//				m_pEquipment->m_pDoc->m_pHostCl->ReportCarrierCommandStatus(COMMAND_STATUS_CARRIERTRANSFERING,
//										strCommandId, 
//										strCarrierId,
//										strSource, 
//										CLib::GetHostLocFromStationNum(MCS_STN_POS_HS_01), 
//										strDest);
//		}
//	}
//	else
//	{
//		if(GetCycleStep() == SC_CYCLE_UNLOAD && IsArrivedPosition(SC_CLEANING_PORTNUM))
//		{
//			pJobItem->SetCleaned(TRUE);
//			m_pDoc->m_pJob->Backup();
//		}
//	}
}
void CScInfo::SetProdLoadOnFork2			(BOOL bProdLoadOnFork2)
{
	if(m_bProdLoadOnFork2 == bProdLoadOnFork2)	return;

	m_bProdLoadOnFork2 = bProdLoadOnFork2;
	m_bModified = TRUE;

//	CJobItem* pJobItem2 = m_pEquipment->m_pDoc->m_pJob->Find(m_ScInfo[1].m_nInternalLuggNum);
//	if(bProdLoadOnFork2)
//	{
//		if(pJobItem2)
//		{
//			CString strCommandId = "", strCarrierId = "", strSource = "", strDest = "";
//			strCommandId = pJobItem2->GetCommandId();
//			strCarrierId = pJobItem2->GetContainerID();
//			strSource = pJobItem2->GetHostSource();
//			strDest = pJobItem2->GetHostDest();
//
//			if(m_pEquipment->m_pDoc->m_pHostCl)
//				m_pEquipment->m_pDoc->m_pHostCl->ReportCarrierCommandStatus(COMMAND_STATUS_CARRIERTRANSFERING,
//										strCommandId, 
//										strCarrierId,
//										strSource, 
//										CLib::GetHostLocFromStationNum(MCS_STN_POS_HS_01), 
//										strDest);
//		}
//	}
//
}
void CScInfo::SetProdOnLeftShelf			(BOOL bProdOnLeftShelf)
{
	if(m_bProdOnLeftShelf == bProdOnLeftShelf)	return;

	m_bProdOnLeftShelf = bProdOnLeftShelf;
	m_bModified = TRUE;
}

void CScInfo::SetProdOnRightShelf			(BOOL bProdOnRightShelf)
{
	if(m_bProdOnRightShelf == bProdOnRightShelf)	return;

	m_bProdOnRightShelf = bProdOnRightShelf;
	m_bModified = TRUE;
}

void CScInfo::SetBufferEmpty				(BOOL bBufferEmpty)
{
	if(m_bBufferEmpty == bBufferEmpty)	return;

	m_bBufferEmpty = bBufferEmpty;
	m_bModified = TRUE;

}

void CScInfo::SetAutoMode				(BOOL bAutoMode)
{
	if(m_bAutoMode == bAutoMode)	
		return;

	m_bAutoMode = bAutoMode;
	m_bModified = TRUE;

	(m_bAutoMode)? SetScMode(enAuto): SetScMode(enManual);	

}

void CScInfo::SetOnLineMode				(BOOL bOnLineMode)
{
	if(m_bOnLineMode == bOnLineMode)	return;

	m_bOnLineMode = bOnLineMode;
	m_bModified = TRUE;

	(m_bOnLineMode)	? SetRcMode(enOnline)	: SetRcMode(enRemote);	
}

void CScInfo::SetCarrierID(CString strCarrierID)
{
	if(m_strCarrierID == strCarrierID)	return;

	m_strCarrierID = strCarrierID;
	m_bModified = TRUE;

    if(strCarrierID == "ERROR")
	{
		CString strId = "ERROR";
		CJobItem* pJobItem = NULL;
		if(m_ScInfo[0].m_nInternalLuggNum)
		{
			pJobItem = m_pDoc->m_pJob->Find(m_ScInfo[0].m_nInternalLuggNum);
			if(pJobItem)
				strId = pJobItem->GetContainerID();
		}
		if(m_ScInfo[1].m_nInternalLuggNum)
		{
			pJobItem = m_pDoc->m_pJob->Find(m_ScInfo[1].m_nInternalLuggNum);
			if(pJobItem)
				strId = pJobItem->GetContainerID();
		}

		LOG_ERROR(LOG_POS_SC, m_nInternalLuggNum, "ID Read fail", strId);
	}
}

BOOL CScInfo::cmdExecute(int nCmd)
{
//	if(!m_pSc)	return FALSE;

//	if(m_pSc->IsConnect() == FALSE)	return FALSE;

	m_ScMsg.SetEvent(TRUE);

	m_ScMsg.SetCommand(nCmd);
	m_pSc->ExecuteCommand();

	return TRUE;
}
void CScInfo::CheckPermit()
{
 	if(IsOnBusy() == FALSE)	return;

	if(m_ScInfo[0].m_nInternalLuggNum == 0)	
		return;

	CJobItem* pJobItem = m_pDoc->m_pJob->Find(m_ScInfo[0].m_nInternalLuggNum);
	if(pJobItem == NULL)	return;

	CTrackInfo* pPortInfo = NULL;
	if(GetCycleStep() == SC_CYCLE_PICKUP)
	{
		if(IsProdLoadOnFork1())	return;
		
		switch(pJobItem->GetJobPattern())
		{
			case enJobPatternSto	:
				if(IsArrivedPosition(pJobItem->m_nStartPos))
				{
					pPortInfo = m_pDoc->GetTrackInfo(pJobItem->m_nDepartTrackNum);
					if(pPortInfo == NULL)	return;
					if(pPortInfo->IsScStoHsReady())
					{
						PermitLoadUnload(TRUE, TRUE);
						PermitLoadUnload(FALSE, FALSE);
					}
					else	// 입고대 준비 안되어 있을 때 처리
					{

					}
				}
				break;

			case enJobPatternR2R	:
			case enJobPatternRet	:
				if(IsArrivedPosition(pJobItem->m_strStartLoc) == FALSE)
					return;

				if(IsShelfSensor() == TRUE)
				{
					PermitLoadUnload(TRUE, TRUE);
					PermitLoadUnload(FALSE, FALSE);
				}
				else
				{
					pJobItem->m_nJobStatus =enJobStatusScRequest;
//					pJobItem->SetStatus(JOB_STA_CV_OPER_REQUEST);
					m_ScInfo[0].m_nInternalLuggNum = 0;
					m_ScInfo[0].m_nInternalJobType = enJobTypeNone;
					m_ScInfo[1].m_nInternalLuggNum = 0;
					m_ScInfo[1].m_nInternalJobType = enJobTypeNone;
//					SetInternalLuggNum(0);
//					SetInternalJobType(enJobTypeNone);
					ResetInvoke();

					JOB->Backup();
					// 공출고시 처리
				}
				break;

			case enJobPatternMove	:
				return;
		}
	}
	else if(GetCycleStep() == SC_CYCLE_UNLOAD)
	{
		if(IsProdLoadOnFork1() == FALSE)	return;

		switch(pJobItem->GetJobPattern())
		{
			case enJobPatternSto	:
			case enJobPatternR2R	:
				if(IsArrivedPosition(pJobItem->m_strDestLoc) == FALSE)
					return;

				if(IsShelfSensor() == FALSE)
				{
					PermitLoadUnload(FALSE, TRUE);
					PermitLoadUnload(TRUE, FALSE);
				}
				else	// 이중 입고 처리
				{

				}
				break;

			case enJobPatternRet	:
 				if(IsArrivedPosition(pJobItem->m_nDestPos))
				{
					pPortInfo = m_pDoc->GetTrackInfo(pJobItem->m_nArriveTrackNum);
//					pPortInfo = m_pDoc->GetPortInfo(pJobItem->m_nDestPos);
					if(pPortInfo == NULL)	return;
					if(pPortInfo->IsScRetHsReady())
					{
						PermitLoadUnload(FALSE, TRUE);
						PermitLoadUnload(TRUE, FALSE);
					}
					else // 포트 상태 준비 안되어 있을 경우 처리
					{

					}
				}
				break;
//			case JOB_PATTERN_SCAN	:
			case enJobPatternMove	:
				return;
		}
	}
	else	return;
}
BOOL CScInfo::IsArrivedPosition(int nPortNum)
{
	int nBank, nBay, nLevel;

	if(CLib::GetLocationFromPortNum(nPortNum, nBank, nBay, nLevel) == FALSE)
		return FALSE;

	if(IsBitOn(SIG_FORK1_RIGHT_BANK) == nBank-1 && 
		nBay == GetCycleBay() && 
		nLevel == GetCycleLevel())
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CScInfo::IsArrivedPosition(CString strPosition)
{
	if(strPosition.GetLength() != CLib::enLengthLocation)	
		return FALSE;

	int nBank = _ttoi(strPosition.Mid(0, m_pEquipment->m_pDoc->m_pConfig->m_nBank));
	int nBay = _ttoi(strPosition.Mid(m_pEquipment->m_pDoc->m_pConfig->m_nBank, m_pEquipment->m_pDoc->m_pConfig->m_nBay));
	int nLevel = _ttoi(strPosition.Mid(m_pEquipment->m_pDoc->m_pConfig->m_nBank + m_pEquipment->m_pDoc->m_pConfig->m_nBay, m_pEquipment->m_pDoc->m_pConfig->m_nLevel));

	if(IsBitOn(SIG_FORK1_RIGHT_BANK) == nBank-1 && 
		nBay == GetCycleBay() && 
		nLevel == GetCycleLevel())
	{
		return TRUE;
	}

	return FALSE;
}

void CScInfo::PermitLoadUnload(BOOL bLoad, BOOL bOn)
{
	if(bLoad)
	{
		SetBit(SIG_PERMIT_PICKUP,	bOn);
		SetBit(SIG_PICKUP_OK,		bOn);
	}
	else
	{
		SetBit(SIG_PERMIT_UNLOAD,	bOn);
		SetBit(SIG_UNLOAD_OK,		bOn);
	}
}

BOOL CScInfo::IsShelfSensor()
{
	if(IsBitOn(SIG_FORK1_RIGHT_BANK))
		return IsProdOnRightShelf();
	else
		return IsProdOnLeftShelf();
}

void CScInfo::ClearInternalData()
{
	m_ScInfo[0].m_nInternalLuggNum	= 0;
	m_ScInfo[0].m_nInternalJobType	= enJobTypeNone;
	m_ScInfo[1].m_bInvoke			= FALSE;
	m_ScInfo[1].m_nInternalLuggNum	= 0;
	m_ScInfo[1].m_nInternalJobType	= enJobTypeNone;
	m_ScInfo[1].m_bInvoke			= FALSE;
	Backup();
//	SendMsgToView(NOTIFY_REFRESH);		// 신호등 표시되지 않을수도...
}

void CScInfo::ClearInternalData(int nFork)
{
	m_ScInfo[nFork].m_nInternalLuggNum	= 0;
	m_ScInfo[nFork].m_nInternalJobType	= enJobTypeNone;
	m_ScInfo[nFork].m_bInvoke			= FALSE;
	Backup();
//	SendMsgToView(NOTIFY_REFRESH);		// 신호등 표시되지 않을수도...
}

void CScInfo::SetCycleStoped(BOOL bData)
{
	if(m_bCycleStoped == bData)	return;

	m_bCycleStoped = bData;
	SetModified(TRUE);

}
void CScInfo::SetEmergencyStoped(BOOL bData)
{
	if(m_bEmergencyStoped == bData)	return;

	m_bEmergencyStoped = bData;
	SetModified(TRUE);
}
void CScInfo::SetOriginalData(BYTE *pData)
{
	memcpy(m_ucOriginalData, pData, WORD_SIZE_TO_READ*2);
}

BYTE* CScInfo::GetOriginalData()
{
	return m_ucOriginalData;
}

DWORD CScInfo::GetVerticalAimError()
{
	return CLib::SwapToDWord(m_ucOriginalData + OFFSET_OF_VERT_ERROR*2);
}

DWORD CScInfo::GetHorizontalAimError()
{
	return CLib::SwapToDWord(m_ucOriginalData + OFFSET_OF_HORI_ERROR*2);
}

DWORD CScInfo::GetForkAimError()
{
	return CLib::SwapToDWord(m_ucOriginalData + OFFSET_OF_FORK_AIM_ERROR*2);
}

void CScInfo::SetBit(int nSigName, BOOL bOn)
{
	if(bOn)
		m_ucECS2PLCData[nSigName/8] |= (BYTE)(1<<(nSigName%8));
	else
		m_ucECS2PLCData[nSigName/8] &= (BYTE)(~(1<<nSigName%8));

	m_bModified = TRUE;
}

BOOL CScInfo::IsBitOn(int nSigName)
{
	return (m_ucECS2PLCData[nSigName/8] & (1<<nSigName%8)) ? TRUE: FALSE;	// (1<<nSigName%8) ☜ nSigName : time of shift 
}

BOOL CScInfo::IsBitOnPLC(int nSigName)
{
	return (m_ucPLC2ECSData[nSigName/8] & (1<<nSigName%8)) ? TRUE: FALSE;
}

BOOL CScInfo::SetModified(BOOL bModified)
{
	BOOL bOld = m_bModified;
	m_bModified = bModified;

	return bOld;
}

void CScInfo::SetECS2PLCData(BYTE* pucData)
{
	memcpy(m_ucECS2PLCData, pucData, WORD_SIZE_OF_ECS2PLC*2);
}

void CScInfo::SetPLC2ECSData(BYTE* pucData)
{
	memcpy(m_ucPLC2ECSData, pucData, WORD_SIZE_OF_PLC2ECS*2);
}

void CScInfo::SetCycleStep			(BYTE ucCycleStep)
{
	m_ucECS2PLCData[OFFSET_OF_CYCLE_STEP] = (m_ucECS2PLCData[OFFSET_OF_CYCLE_STEP] & 0xF8) | ucCycleStep;
}

BOOL CScInfo::IsLoadedProduct()
{
	if(IsProdLoadOnFork1())	return TRUE;

	return FALSE;
}

//	enum EN_FORK_TYPE { enSingle, enTwin, enDouble, enForkTypeSize };

BOOL CScInfo::IsTypeOfTwinFork()
{
	if(m_nForkType == enTwin)
		return TRUE;
	
	return FALSE;
}

BOOL CScInfo::IsTypeOfDoubleFork()
{
	if(m_nForkType == enDouble)
		return TRUE;
	
	return FALSE;
}

BOOL CScInfo::IsFinishTheWork()
{
	if(!IsMainPowereOn() || !IsAutoMode() || m_wErrCode)
		return FALSE;

	if( !IsOnBusy() && 
		(!IsProdLoadOnFork1() || !IsProdLoadOnFork2()))
		 return TRUE;

	return FALSE;
}

BOOL CScInfo::IsReadyToWork()
{
	if(	m_ScInfo[0].m_bInvoke || m_ScInfo[1].m_bInvoke || 
		!IsMainPowereOn() || 
		!IsAutoMode() || 
		!IsActiveMode() || 
		m_wErrCode || 
		IsProdLoadOnFork1() || 
		IsProdLoadOnFork2() )
		return FALSE;

	return TRUE;
}

CString CScInfo::GetHorizontalPosString()
{
	CString strTemp;
	strTemp.Format("%d", m_ucHorizontalPos);
	return strTemp;
}

CString CScInfo::GetVerticalPosString()
{
	CString strTemp;
	strTemp.Format("%d", m_ucVerticalPos);
	return strTemp;
}


void CScInfo::SendMsgToView(int nStatus)
{
	if(IsWindow(m_pWndView->m_hWnd))
		m_pWndView->PostMessage(m_nMsgToView, (WPARAM)this, nStatus);
}



//*/

