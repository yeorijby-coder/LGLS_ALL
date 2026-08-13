// TrackInfo.cpp: implementation of the CTrackInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "TrackInfo.h"
#include "Equipment.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrackInfo::CTrackInfo(CEquipment* pEquipment)
{
	m_pEquipment = pEquipment;
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	m_nNumber = 0;
	m_nDestination = 0;

	m_nJobQty = 0;

	m_bReportTrack	= FALSE;
	m_pTrackCtrl = NULL;

	m_bModified		= TRUE;
	m_bSuspend		= FALSE;
	
	m_bDeadLock		= FALSE;

	m_bStoRequest	= FALSE;
	m_bRetRequest	= FALSE;
	
	m_bProductPLTRequest = FALSE;

	m_bFirstDoorCheck = TRUE;

	m_tStatusReport = CTime::GetCurrentTime();
	m_strEtcValue		= "";

	m_bVirtual = FALSE;

	Initialize();
}

CTrackInfo::~CTrackInfo()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CTrackInfo::Initialize()
{
	m_nLuggNum		= 0;
	m_nJobType		= 0;
	m_nDestPos		= 0;
	m_ucGenCode		= 0;
	m_wReserved3	= 0;
	m_wErrorCode	= enErrorCommunication;
	m_wStatusData	= 0;
	m_wSensorData	= 0;
	m_wMotorData	= 0;
	m_bModified		= TRUE;
}

// 임의의 Word를 Write 하려 할때 
BOOL CTrackInfo::CommandWriteWord(int nAddress, WORD wWord)
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if (m_pEquipment->IsConnect() == FALSE)
		return FALSE;

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCmdMsg::enCmdWriteWord;
	pCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeWriteWord);
	pCmdMsg->m_strValues[0].Format(_T("%d"), nAddress);
	pCmdMsg->m_strValues[1].Format(_T("%d"), wWord);
	m_pEquipment->SetCmdMsg(pCmdMsg);

	return TRUE;
}

// 5번째 워드(Not Tracking을 사용할 때)
BOOL CTrackInfo::CommandDeadLock(BOOL bDeadLock /* = FALSE*/)
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if (m_pEquipment->IsConnect() == FALSE)
		return FALSE;

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCmdMsg::enCmdTrackDeadLock;
	pCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeTrackDeadLock);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_nNumber);
	pCmdMsg->m_strValues[1].Format(_T("%d"), bDeadLock);
	m_pEquipment->SetCmdMsg(pCmdMsg);

	return TRUE;
}

// 2번째 워드(Tracking을 사용할 때)
BOOL CTrackInfo::CommandDeadLockCheck(BOOL bDeadLock /* = FALSE*/)
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if (m_pEquipment->IsConnect() == FALSE)
		return FALSE;

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCmdMsg::enCmdTrackDeadLockCheck;
	pCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeTrackDeadLockCheck);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_nNumber);
	pCmdMsg->m_strValues[1].Format(_T("%d"), bDeadLock);
	m_pEquipment->SetCmdMsg(pCmdMsg);

	return TRUE;
}


BOOL CTrackInfo::CommandTrack(int nLuggNum, int nJobType, int nDestPos, BYTE ucGenCode)
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if (m_pEquipment->IsConnect() == FALSE)
		return FALSE;

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCmdMsg::enCmdTrackWrite;
	pCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeTrackWrite);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_nNumber);
	pCmdMsg->m_strValues[1].Format(_T("%d"), nLuggNum);
	pCmdMsg->m_strValues[2].Format(_T("%d"), nJobType);
	pCmdMsg->m_strValues[3].Format(_T("%d"), nDestPos);
	pCmdMsg->m_strValues[4].Format(_T("%d"), ucGenCode);
	m_pEquipment->SetCmdMsg(pCmdMsg);

	return TRUE;
}

BOOL CTrackInfo::CommandTrack(CJobItem* pJobItem, int nDestTrackNum /* = 0 */)
{
	CStationInfo* pDestStation = m_pEquipment->m_pDoc->GetStationInfo(pJobItem->m_strDestStn);
	int nTrackDest = (nDestTrackNum == 0) ? pDestStation->GetTrackDestination() : nDestTrackNum;
	DEBUGER_ASSERT_VALID(pDestStation != NULL);
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	return CommandTrack(pJobItem ? pJobItem->m_nLuggNum : 0, pJobItem ? pJobItem->m_nJobType : 0, nTrackDest, pJobItem ? pJobItem->m_ucGenCode : 0);
}

void CTrackInfo::StatusReport(BOOL bManual /* = FALSE */)
{
	if (m_bReportTrack == FALSE)
		return;

// 	CTimeSpan tElapse = CTime::GetCurrentTime() - m_tStatusReport;
// 	if ((bManual == FALSE) || (tElapse.GetTotalSeconds() < 30))
// 		return;

	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	CStationInfo* pStation = NULL;
	int nEquipGbn = 0, nStatusGbn = 0;
	if (pStation = GetStoStation())
	{
		nEquipGbn = enEquipStoStn;
		nStatusGbn = (m_pEquipment->IsConnect() && IsStoStationReady());
	}
	else if (pStation = GetRetStation())
	{
		nEquipGbn = enEquipRetStn;
		nStatusGbn = (m_pEquipment->IsConnect() && IsRetStationReady());
	}
	else if (pStation = GetArvStation())
	{
		nEquipGbn = enEquipStoStn;
		nStatusGbn = (m_pEquipment->IsConnect() && IsStoStationReady());
	}
	else
	{
		return;
	}

//	BOOL bConnected = pDoc->m_pHostCl->IsConnect();

	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
		pDoc->m_pHostCl->Status();
}

void CTrackInfo::ErrorReport(BOOL bManual /* = FALSE */)
{
	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	int nPlcNum = m_nNumber / 1000;

	int nErrorCode = (nPlcNum * 1000) + m_wErrorCode;
	
	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
	{
//		pDoc->m_pHostCl->Error(2, m_nNumber, 0, m_wErrorCode, m_nLuggNum);
		// 중량이상 에러코드 101
		if ((nPlcNum == 3 && (m_wErrorCode == 101 || m_wErrorCode == 239)) || 
			(nPlcNum == 1 && m_wErrorCode == 85))		
			pDoc->m_pHostCl->Error(2, m_nNumber, 5, nErrorCode, m_nLuggNum, "", m_strEtcValue);		//  중량이상 
		else
			pDoc->m_pHostCl->Error(2, m_nNumber, 0, nErrorCode, m_nLuggNum);
	}
}

//void CTrackInfo::TroubleReport(BOOL bManual /* = FALSE */)
//{
//	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
//	DEBUGER_ASSERT_VALID(pDoc != NULL);
//
//	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
//		pDoc->m_pHostCl->TroubleReport(1, m_nNumber, m_tErrorStart, m_tErrorEnd, m_wErrorCode, m_nLuggNum, GetWarehouseNo());
//}
//
//void CTrackInfo::LeadTimeReport(BOOL bManual /* = FALSE */)
//{
//	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
//	DEBUGER_ASSERT_VALID(pDoc != NULL);
//
//	if (pDoc->m_pHostCl && pDoc->m_pHostCl->IsConnect())
//		pDoc->m_pHostCl->LeadTimeReport(1, m_nNumber, m_tOperStart, m_tOperEnd, m_nLuggNum, GetWarehouseNo());
//}

int CTrackInfo::GetWarehouseNo()
{
	int nWarehouseNo = 0;

	if ((m_nNumber > 1000) && (m_nNumber < 2000)) nWarehouseNo = CJobItem::enWarehouse2;
	else if ((m_nNumber > 0) && (m_nNumber < 1000)) nWarehouseNo = CJobItem::enWarehouse1;
	else nWarehouseNo = 0;

	return nWarehouseNo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CTrackInfo::SetLuggNum(int nLuggNum)
{
	if (m_nLuggNum == nLuggNum)
		return;

	if ((m_nLuggNum == 0) && (nLuggNum != 0))
	{
		m_tOperStart = CTime::GetCurrentTime();
		m_tOperEnd = m_tOperStart;
	}

	if ((m_nLuggNum != 0) && (nLuggNum == 0) && (m_tOperEnd == m_tOperStart) && (CJobItem::IsOnlineLuggNum(m_nLuggNum) == TRUE))
	{
		m_tOperEnd = CTime::GetCurrentTime();
//		LeadTimeReport();
	}

	m_nLuggNum = nLuggNum;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1);
}

void CTrackInfo::SetJobType(int nJobType)
{
	if (m_nJobType == nJobType)
		return;

	m_nJobType = nJobType;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1);
}

void CTrackInfo::SetDestPos(int nDestPos)
{
	if (m_nDestPos == nDestPos)
		return;

	m_nDestPos = nDestPos;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1);
}

void CTrackInfo::SetEtcValue(CString strEtcValue)
{
	if (m_strEtcValue == strEtcValue)
		return;

	m_strEtcValue = strEtcValue;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1);
}

void CTrackInfo::SetGenCode(BYTE ucGenCode)
{
	if (m_ucGenCode == ucGenCode)
		return;

	m_ucGenCode = ucGenCode;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1);
}

void CTrackInfo::SetSize(int nSize)
{
	if (m_wReserved3 == nSize)
		return;

	m_wReserved3 = nSize;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1);
}

void CTrackInfo::SetSuspend(BOOL bSuspend)
{
	if (m_bSuspend == bSuspend)
		return;

	m_bSuspend = bSuspend;
	m_bModified = TRUE;
}

void CTrackInfo::SetDeadLock(BOOL bDeadLock)
{
	if (m_bDeadLock == bDeadLock)
		return;

	m_bDeadLock = bDeadLock;
	m_bModified = TRUE;
}

//=====================================================================================================
//	// 경광등 관련
//=====================================================================================================	
void CTrackInfo::SetLampInfo(BOOL bOn)
{
	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
	if (pDoc == NULL)
		return;

	CCvFx* pCv = (CCvFx*)pDoc->GetEquipment(CEquipment::enCV, 12);	// 12번이 경광등 임
	if (pCv == NULL)
		return;

	int nIndex = 0;
	switch(m_pEquipment->m_nNumber) 
	{
	case 1:
	case 2:		nIndex = 3;		break;
	case 3:
	case 4:
	case 5:		nIndex = 4;		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:	nIndex = 5;		break;
	}

	CLib::SetBit(pCv->m_pInfo->m_wCurrLampStatus, nIndex, bOn);
}
//-----------------------------------------------------------------------------------------------------

void CTrackInfo::SetErrorCode(WORD wErrorCode)
{
//	if (m_wErrorCode == wErrorCode)			// 항상 에러상태를 체크해서 경광등을 올려야 하므로...
//		return;

	// 에러가 처음 발생했을 경우
	if ((m_wErrorCode == 0) && (wErrorCode != 0))
	{
		m_tErrorStart = CTime::GetCurrentTime();
		m_tErrorEnd = m_tErrorStart;
		m_wErrorCode = wErrorCode;
		m_bModified = TRUE;
		ErrorReport();

		if (m_bVirtual == FALSE)
			SetLampInfo(TRUE);
	}

	// 에러상태에서 다른 에러가 발생했을 경우
	if ((m_wErrorCode != 0) && (wErrorCode != 0) && (m_wErrorCode != wErrorCode)) 
	{
		m_wErrorCode = wErrorCode;
		m_bModified = TRUE;
		ErrorReport();
	}

	// 정상으로 돌아왔을경우 
	if ((m_wErrorCode != 0) && (wErrorCode == 0) && (m_tErrorEnd == m_tErrorStart))
	{
		m_tErrorEnd = CTime::GetCurrentTime();
		SetLampInfo(FALSE);
	}

	// 프로그램이 처음 실행 되었을 때 
	if (m_bFirstDoorCheck == TRUE)
	{
		SetLampInfo(BOOL(wErrorCode));
		m_bFirstDoorCheck = FALSE;
	}

	if (m_wErrorCode == wErrorCode)
		return;

	m_wErrorCode = wErrorCode;
	m_bModified = TRUE;

	if (m_wErrorCode != 0)
	{
		CString strLog;
		strLog.Format(_T("%s %s %s [%d=%s]"), m_pEquipment->GetDeviceName(), GetTrackString(), IsErrorCode(m_wErrorCode) ? _T("ERROR") : _T("ALARM"), m_wErrorCode, GetErrorString());
		m_pEquipment->WriteLog(IsErrorCode(m_wErrorCode) ? LOG_TYPE_ERROR : LOG_TYPE_ALARM, m_nLuggNum, strLog, NULL, NULL, TRUE);
	}
}

void CTrackInfo::SetStatusData(WORD wStatusData)
{
	if (m_wStatusData == wStatusData)
		return;

	m_wStatusData = wStatusData;
	m_bModified = TRUE;
	StatusReport();
}

void CTrackInfo::SetSensorData(WORD wSensorData)
{
	// 수정하기 직전의 화물 감지 상태 정보 저장 
	BOOL bPrevProdSensor = IsProductSensing();

	if (m_wSensorData == wSensorData)
		return;

	m_wSensorData = wSensorData;

/*
//===================================================================================================================================================
//	// 이고 입고대에서 화물감지가 꺼질때 생산 정보 지워줘야 하리라. 
//===================================================================================================================================================
	CDciStaticCtrl* pPrdInfo1 = NULL;
	CDciStaticCtrl* pPrdInfo2 = NULL;

	// 전면부 표시 이고입고대 생산 정보 지우기 
	if (m_nNumber == 3504 && bPrevProdSensor == TRUE && IsProductSensing() == FALSE)
	{
		pPrdInfo1 = m_pEquipment->m_pDoc->m_pStoDataChk1[0];		// 이고 입고대는 0번 다음에 추가시에 다음 인덱스 사용 요망 
		pPrdInfo2 = m_pEquipment->m_pDoc->m_pStoDataChk2[0];		// 이고 입고대는 0번 다음에 추가시에 다음 인덱스 사용 요망 
		if (pPrdInfo2 != NULL && pPrdInfo2 != NULL)
		{
			pPrdInfo1->m_strText = "";
			pPrdInfo1->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);

			pPrdInfo2->m_strText = "0개";
			pPrdInfo2->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
		}
	}
//---------------------------------------------------------------------------------------------------------------------------------------------------
//*/

	m_bModified = TRUE;
}

void CTrackInfo::SetMotorData(WORD wMotorData)
{
	if (m_wMotorData == wMotorData)
		return;

	m_wMotorData = wMotorData;
	m_bModified = TRUE;
}

void CTrackInfo::SetStatusDataBit(EN_STATUS enStatus, BOOL bValue)
{
	if (bValue) m_wStatusData |= enStatus;
	else m_wStatusData &= ~enStatus;
}

CString CTrackInfo::GetTrackString()
{
	CString strTrack, strTemp;
	
	strTemp.Format(_T("TR%04d"), m_nNumber);


	int nPart = _ttoi(strTemp.Mid(3,1));
	int nScNum = _ttoi(strTemp.Mid(4,1));
	int nTrackNum = _ttoi(strTemp.Mid(4,2));
	switch(nPart) 
	{
	case 9:
		nTrackNum = _ttoi(strTemp.Mid(5,1));
		switch(nScNum) 
		{
		case 1:
			switch(nTrackNum) 
			{
			case 1:
			case 2:
			case 3:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 1, nTrackNum, strTemp);				break;
			case 4:
			case 5:
			case 6:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 2, nTrackNum, strTemp);				break;
			}
			break;
		case 2:
			switch(nTrackNum) 
			{
			case 1:
			case 2:
			case 3:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 3, nTrackNum, strTemp);				break;
			case 4:
			case 5:
			case 6:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 4, nTrackNum, strTemp);				break;
			}
			break;
		case 3:
			switch(nTrackNum) 
			{
			case 1:
			case 2:
			case 3:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 5, nTrackNum, strTemp);				break;
			case 4:
			case 5:
			case 6:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 6, nTrackNum, strTemp);				break;
			}
			break;
		case 4:
			switch(nTrackNum) 
			{
			case 1:
			case 2:
			case 3:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 7, nTrackNum, strTemp);				break;
			case 4:
			case 5:
			case 6:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 8, nTrackNum, strTemp);				break;
			}
			break;
		case 5:
			switch(nTrackNum) 
			{
			case 1:
			case 2:
			case 3:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 9, nTrackNum, strTemp);				break;
			case 4:
			case 5:
			case 6:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 10, nTrackNum, strTemp);			break;
			}
			break;
		case 6:
			switch(nTrackNum) 
			{
			case 1:
			case 2:
			case 3:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 11, nTrackNum, strTemp);			break;
			case 4:
			case 5:
			case 6:				strTrack.Format(_T("G%1s-RC%d-%02d [%s]"),strTemp.Mid(2,1), 12, nTrackNum, strTemp);			break;
			}
			break;
		}

		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:						strTrack.Format(_T("G%1s-P%d-%02d [%s]"),strTemp.Mid(2,1), nPart, nTrackNum, strTemp);			break;
	}

	return strTrack;
}

CString CTrackInfo::GetJobTypeString()
{
	return CJobItem::GetJobTypeString(m_nJobType);
}

CString CTrackInfo::GetGenString()
{
	CString strGEN;
	strGEN.Format(_T("GEN%d"), m_ucGenCode);
	return strGEN;
}

WORD CTrackInfo::GetMapStatusKey(int nIndex)
{
	WORD wStatusKey = 0;
	CObject* pStatusValue = NULL;
	int i = 0;

	for (POSITION pos=m_mapStatusInfos.GetStartPosition(); pos!=NULL; ++i)
	{
		m_mapStatusInfos.GetNextAssoc(pos, wStatusKey, pStatusValue);
//		DEBUGER_ASSERT_VALID(pStatusObject != NULL);

		if (i == nIndex)
			return wStatusKey;
	}

	return 0;
}

CObject* CTrackInfo::GetMapStatusValue(WORD wStatus)
{
	CObject* pStatusValue = NULL;
	if (m_mapStatusInfos.Lookup(wStatus, pStatusValue) == FALSE)
		return NULL;

	return pStatusValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

COLORREF CTrackInfo::GetCvColor()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_wErrorCode == enErrorCommunication)
		return pConfig->m_clrDisconnect;

	if (m_wErrorCode != 0)
		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;

	switch (m_nJobType)
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

	if (IsAutoMode() == FALSE)
		return DARK_GRAY;

	return LIGHT_GRAY;
}

void CTrackInfo::InvokeControl()
{
	if (m_bModified == FALSE)
		return;

	if (m_pTrackCtrl == NULL) 
		return; 

	if ((m_pTrackCtrl->GetItemSize() != m_arrayStatus.GetSize()) ||
		(m_pTrackCtrl->GetItemSize() != m_mapStatusInfos.GetCount()))
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	COLORREF clrStatusNone = 0x606060;
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	m_pTrackCtrl->m_clrBgColor = GetCvColor();

	
	//이해담 cv
	if(m_pEquipment->m_pDoc->m_layout.m_bShowText == TRUE)
	{
		if(m_nLuggNum != 0 &&
		  ((m_pTrackCtrl->m_clrBgColor != LIGHT_GRAY) && (m_pTrackCtrl->m_clrBgColor != pConfig->m_clrDisconnect)))
		{
			m_pTrackCtrl->m_strText.Format(_T("%d"), m_nLuggNum);
			CDciTrackCtrl* pDciTrackCtrl = (CDciTrackCtrl*) m_pTrackCtrl;
			
			if (pDciTrackCtrl == NULL)
				return;
			
			pDciTrackCtrl->m_nFontSize = m_pEquipment->m_pDoc->m_pConfig->m_nFontSize;
		}
		else
		{
			m_pTrackCtrl->m_strText.Format(_T("%d"), m_nNumber);
			CDciTrackCtrl* pDciTrackCtrl = (CDciTrackCtrl*) m_pTrackCtrl;
			
			if (pDciTrackCtrl == NULL)
				return;
			
			pDciTrackCtrl->m_nFontSize = m_pEquipment->m_pDoc->m_pConfig->m_nFontSize;
		}
	}

	m_pTrackCtrl->m_bExist = IsProductSensing();

	for (int i=0; i<m_pTrackCtrl->GetItemSize(); ++i)
	{
		WORD wStatus = m_arrayStatus[i];
		DEBUGER_ASSERT_VALID(wStatus != 0);
		
		int nTrackNum = _ttoi(m_pTrackCtrl->m_strText);
		CTrackInfo* pTrack = m_pEquipment->m_pDoc->GetTrackInfo(nTrackNum);
		switch (wStatus)
		{
		case enStatusStoSTReady:
			m_pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrStoST : clrStatusNone);

			break;

		case enStatusRetSTReady:
			m_pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrRetST : clrStatusNone);
			
			break;

		case enStatusCvCheckReady:
		case enStatusArvSTReady:
			m_pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrArvST: clrStatusNone);
			
			break;

		case enStatusScStoHSReady:
		case enStatusRtvDepartHSReady:
		case enStatusLgvDepartHSReady:
		case enStatusLfDepartHSReady:
			m_pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrDepart: clrStatusNone);  
		
			break;

		case enStatusScRetHSReady:
		case enStatusRtvArriveHSReady:
		case enStatusLgvArriveHSReady:
		case enStatusLfArriveHSReady:
			m_pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrArrive : clrStatusNone);
			break;

		default:
			DEBUGER_ASSERT_VALID(FALSE);
		}
	}

	m_pTrackCtrl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
	m_pEquipment->RefreshDialog();
	MulticastInfo();

	m_bModified = FALSE;
}

void CTrackInfo::MulticastInfo()
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);
	DEBUGER_ASSERT_VALID(m_pEquipment->m_pDoc != NULL);

	CString strSend;
	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO>%s</INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_pEquipment->GetDeviceName(), GetXmlString(), CMonitorServer::enETX
	);

//==========================================================================================================	
//	// PlayBack을 위해서 
//==========================================================================================================
//	m_pEquipment->m_pDoc->m_pLogPlayback->Log(strSend);	
//----------------------------------------------------------------------------------------------------------

//	CMonitorListener* pMonitor = m_pEquipment->m_pDoc->m_pMonitorListener;
//	if (pMonitor && pMonitor->GetClientCount())
//		pMonitor->SendToClients(strSend);
}

CString CTrackInfo::GetXmlString()
{
	CString strXml;
	strXml.Format(_T("<TRACK TID='%d' LNO='%d' JT='%d' DST='%d' ERR='%d' STATUS='%d' SENSOR='%d' MOTOR='%d' SUS='%d' ETCV='%s'/>"),
		m_nNumber, m_nLuggNum, m_nJobType, m_nDestPos, m_wErrorCode, m_wStatusData, m_wSensorData, m_wMotorData, m_bSuspend, m_strEtcValue
	);
	return strXml;
}

CString CTrackInfo::GetErrorString()
{
	CString strErrorCode, strErrorMessage, strTemp;
	strErrorCode.Format(_T("%d"), m_wErrorCode);
	strErrorMessage.Format(_T("등록되지 않은 에러코드 [CODE=%d]"), m_wErrorCode);

	TCHAR szTemp[_MAX_PATH] = {0};
	int nGroupNum =0;

	switch(m_pEquipment->m_nNumber) 
	{
	case 1:										
	case 2:				nGroupNum = 1;			break;
	case 3:										
	case 4:									
	case 5:				nGroupNum = 2;			break;
	case 6:									
	case 7:		
	case 8:		
	case 9:		
	case 10:	
	case 11:			nGroupNum = 3;			break;
	case 12:			nGroupNum = 4;			break;
	}
	
//	strErrorCode = "6";

	strTemp.Format(_T("%dGroup PLC"), nGroupNum);
	::GetPrivateProfileString(/*_T("CV_ERROR_CODE")*/strTemp, strErrorCode, (LPCTSTR)strErrorMessage, szTemp, _MAX_PATH, ECS_CODE_FILE);
	strErrorMessage.Format(_T("%s"), szTemp);

	return strErrorMessage;
}

BOOL CTrackInfo::IsAlarmCode(WORD wErrorCode)
{
	CString strErrorCode;
	strErrorCode.Format(_T("%d"), wErrorCode);
	return (BOOL)::GetPrivateProfileInt(_T("CV_ALARM_CODE"), strErrorCode, 0, ECS_CODE_FILE);
}

BOOL CTrackInfo::IsErrorCode(WORD wErrorCode)
{
	return ((wErrorCode != 0) && (IsAlarmCode(wErrorCode) == FALSE));
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrackInfo* CTrackInfoArray::GetTrackInfo(int nNumber)
{
	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = GetAt(i);
		DEBUGER_ASSERT_VALID(pTrack != NULL);

		if (pTrack->m_nNumber == nNumber)
			return pTrack;
	}

	return NULL;
}

CString CTrackInfoArray::GetXmlString()
{
	CString strXml;
	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = GetAt(i);
		DEBUGER_ASSERT_VALID(pTrack != NULL);
		strXml += pTrack->GetXmlString();
	}

	return strXml;
}

void CTrackInfoArray::StatusReport()
{
	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = GetAt(i);
		DEBUGER_ASSERT_VALID(pTrack != NULL);
		pTrack->StatusReport();
	}
}

void CTrackInfoArray::InvokeControl(BOOL bManual)
{
	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = GetAt(i);
		DEBUGER_ASSERT_VALID(pTrack != NULL);

		if (bManual)
			pTrack->m_bModified = TRUE;
		pTrack->InvokeControl();
	}
}

void CTrackInfoArray::Initialize()
{
	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = GetAt(i);
		DEBUGER_ASSERT_VALID(pTrack != NULL);
		pTrack->Initialize();
	}
}

BOOL CTrackInfoArray::IsIdleJob(int nLuggNum)
{
	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = m_pData[i];
		DEBUGER_ASSERT_VALID(pTrack != NULL);

		if (pTrack->m_nLuggNum == nLuggNum)
			return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CTrackInfoArray::WritePlayback()
{
	if(m_pDoc->m_pLogPlayback == NULL)
		return;

	CString strTxBuff;

	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = m_pData[i];
		DEBUGER_ASSERT_VALID(pTrack != NULL);

		CString strTxData;

		strTxData += pTrack->GetXmlString();
		strTxBuff.Format("%c<ECS><CRANE>%s</CRANE></ECS>%c", STX, strTxData, ETX);
		pTrack->m_pEquipment->WriteLog(IsErrorCode(m_wErrorCode) ? LOG_TYPE_ERROR : LOG_TYPE_ALARM, m_nLuggNum, strLog, NULL, NULL, TRUE);
	}

	return NULL;

}

*/