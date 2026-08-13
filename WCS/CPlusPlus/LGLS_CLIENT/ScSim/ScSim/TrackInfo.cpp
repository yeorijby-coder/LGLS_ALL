// TrackInfo.cpp: implementation of the CTrackInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "TrackInfo.h"
#include "Equipment.h"
#include "EcsDoc.h"

#include "MainFrm.h"
#include "EcsView.h"

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

	m_nTabNum = 0;
	m_nCvPlcNum = 0;
	m_nNumber = 0;
	m_nDestination = 0;
	m_bReportTrack	= FALSE;
	m_pTrackCtrl = NULL;
	m_pTrackCtrl2 = NULL;
	m_pTrackCtrl3 = NULL;
	m_pTrackCtrl4 = NULL;
	m_pTrackCtrl5 = NULL;
	m_bModified		= TRUE;
	m_bSuspend		= FALSE;
	m_bDeadLock		= FALSE;
	m_bStoRequest	= FALSE;
	m_bRetRequest	= FALSE;
	m_nGoTrack      = -1;
	m_nCrossDest    = 0;
	m_nTurn			= 0;
	m_tOperStart = COleDateTime::GetCurrentTime();
	m_tStatusReport = COleDateTime::GetCurrentTime();
	m_nMGTrack		= 0;
	m_nMGPlcNum		= 0;
	m_nDispenser = 0;
	m_nDPPlcNum = 0;
	m_nHSTrack = 0;
	m_nTrayPicker = 0;
	m_bStartFlag = FALSE;
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
	m_strBarcode	= _T("");
	//m_nKind         = 0;
	//m_nNextCv       = 0;

}

// 5번째 워드(Not Tracking을 사용할 때)
BOOL CTrackInfo::CommandDeadLock(BOOL bDeadLock /* = FALSE*/)
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	if (m_pEquipment->IsConnect(0) == FALSE)
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

	if (m_pEquipment->IsConnect(0) == FALSE)
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

	if (m_pEquipment->IsConnect(0) == FALSE)
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

// 	COleDateTimeSpan tElapse = COleDateTime::GetCurrentTime() - m_tStatusReport;
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
		nStatusGbn = (m_pEquipment->IsConnect(0) && IsStoStationReady());
	}
	else if (pStation = GetRetStation())
	{
		nEquipGbn = enEquipRetStn;
		nStatusGbn = (m_pEquipment->IsConnect(0) && IsRetStationReady());
	}
	else if (pStation = GetArvStation())
	{
		nEquipGbn = enEquipStoStn;
		nStatusGbn = (m_pEquipment->IsConnect(0) && IsStoStationReady());
	}
	else
	{
		return;
	}

}

void CTrackInfo::ErrorReport(BOOL bManual /* = FALSE */)
{
	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
	DEBUGER_ASSERT_VALID(pDoc != NULL);


}

void CTrackInfo::ReleaseReport(BOOL bManual /* = FALSE */)
{
	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
	DEBUGER_ASSERT_VALID(pDoc != NULL);


}

//void CTrackInfo::TroubleReport(BOOL bManual /* = FALSE */)
//{
//	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
//	DEBUGER_ASSERT_VALID(pDoc != NULL);
//
//	if (pDoc->m_pHostSv && pDoc->m_pHostSv->IsConnect())
//		pDoc->m_pHostSv->TroubleReport(1, m_nNumber, m_tErrorStart, m_tErrorEnd, m_wErrorCode, m_nLuggNum, GetWarehouseNo());
//}
//
//void CTrackInfo::LeadTimeReport(BOOL bManual /* = FALSE */)
//{
//	CEcsDoc* pDoc = m_pEquipment->m_pDoc;
//	DEBUGER_ASSERT_VALID(pDoc != NULL);
//
//	if (pDoc->m_pHostSv && pDoc->m_pHostSv->IsConnect())
//		pDoc->m_pHostSv->LeadTimeReport(1, m_nNumber, m_tOperStart, m_tOperEnd, m_nLuggNum, GetWarehouseNo());
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

//	if ((m_nLuggNum == 0) && (nLuggNum != 0))
//	{
//		m_tOperStart = COleDateTime::GetCurrentTime();
//		m_tOperEnd = m_tOperStart;
//	}
//
//	if ((m_nLuggNum != 0) && (nLuggNum == 0) && (m_tOperEnd == m_tOperStart) && (CJobItem::IsOnlineLuggNum(m_nLuggNum) == TRUE))
//	{
//		m_tOperEnd = COleDateTime::GetCurrentTime();
//		LeadTimeReport();
//	}

	m_nLuggNum = nLuggNum;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1,m_nNumber);
}

void CTrackInfo::SetJobType(int nJobType)
{
	if (m_nJobType == nJobType)
		return;

	m_nJobType = nJobType;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1,m_nNumber);
}

void CTrackInfo::SetDestPos(int nDestPos)
{
	if (m_nDestPos == nDestPos)
		return;

	if (nDestPos == 0)
	{
		CStationInfo* pStation = NULL;
		if (pStation = GetStoStation())
		{
			pStation->m_strTrayId = "";
		}
		else if (pStation = GetArvStation())
		{
			pStation->m_strTrayId = "";
		}
	}

	m_nDestPos = nDestPos;
	m_tOperStart = COleDateTime::GetCurrentTime();
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1,m_nNumber);
}

void CTrackInfo::SetGenCode(BYTE ucGenCode)
{
	if (m_ucGenCode == ucGenCode)
		return;

	m_ucGenCode = ucGenCode;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1,m_nNumber);
}

void CTrackInfo::SetSize(int nSize)
{
	if (m_wReserved3 == nSize)
		return;

	m_wReserved3 = nSize;
	m_bModified = TRUE;

	m_pEquipment->RefreshDialog(1,m_nNumber);
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

void CTrackInfo::SetErrorCode(WORD wErrorCode)
{
	if (m_wErrorCode == wErrorCode)
		return;

	if (wErrorCode == 0 && m_wErrorCode == enErrorCommunication)
	{
		m_wErrorCode = wErrorCode;
		m_bModified = TRUE;
		return;
	}

	// 에러가 처음 발생했을 경우
	if ((m_wErrorCode == 0) && (wErrorCode != 0))
	{
		m_tErrorStart = COleDateTime::GetCurrentTime();
		m_tErrorEnd = m_tErrorStart;
		m_wErrorCode = wErrorCode;
		ErrorReport();
	}
	// 에러상태에서 다른 에러가 발생했을 경우
	if ((m_wErrorCode != 0) && (wErrorCode != 0) && (m_wErrorCode != wErrorCode)) 
	{
		m_wErrorCode = wErrorCode;
		ErrorReport();
	}
	//
//	if ((m_wErrorCode != 0) && (wErrorCode == 0) && (m_tErrorEnd == m_tErrorStart))
//	{
//		m_tErrorEnd = COleDateTime::GetCurrentTime();
//		if (IsErrorCode(m_wErrorCode)) 		// 알람까지 보고하기로함 (2011.05.06)
//			TroubleReport();
//	}
	// 에러 해제시 FMS로 Report 송신
	if ((m_wErrorCode != 0) && (wErrorCode == 0))
	{
		ReleaseReport();
	}

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
	if (m_wSensorData == wSensorData)
		return;

	m_wSensorData = wSensorData;
	// 필히 "완료위치" 설정 후 처리
	if ((m_nDestPos != 0) && (IsProductSensing() == TRUE))
		m_tOperStart = COleDateTime::GetCurrentTime();
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
	CString strTrack;
//	strTrack.Format(_T("TRACK#%03d"), m_nNumber + ((m_nCvPlcNum ==0) ? 1*1000 : m_nCvPlcNum*1000));
	strTrack.Format(_T("TRACK#%05d"), m_nNumber);
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

	if (IsAutoMode() == FALSE)
		return DARK_GRAY;

	if (m_nJobType == enJobTypeNone && IsProductSensing())
		return WHEAT;

	switch (m_nJobType)
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

	return LIGHT_GRAY;
}

void CTrackInfo::InvokeControl(CDciTrackCtrl*	pTrackCtrl)
{
	if (m_bModified == FALSE)
		return;

	if (pTrackCtrl == NULL) 
		return;                                                                                                                                                                                                                                                  

	if ((pTrackCtrl->GetItemSize() != m_arrayStatus.GetSize()) ||
		(pTrackCtrl->GetItemSize() != m_mapStatusInfos.GetCount()))
	{

		return; //이거 수정해야함 ! 강용규qq
		/*int k = m_nNumber;
		int j =pTrackCtrl->GetItemSize();
		int jj = m_arrayStatus.GetSize();
		CString aa= pTrackCtrl->m_strCID;
		DEBUGER_ASSERT_VALID(FALSE);
		return;*/

	}

	COLORREF clrStatusNone = 0x606060;
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);
	CTrackHS* pTrackHS = NULL;
	pTrackCtrl->m_clrBgColor = GetCvColor();
	pTrackCtrl->m_bExist = IsProductSensing();

	for (int i=0; i<pTrackCtrl->GetItemSize(); ++i)
	{
		WORD wStatus = m_arrayStatus[i];

		DEBUGER_ASSERT_VALID(wStatus != 0);
		//pTrackCtrl->m_strCID.Right(5);
		int nTrackNum = _ttoi(pTrackCtrl->m_strCID.Right(5));
		CTrackInfo* pTrack = m_pEquipment->m_pDoc->GetTrackInfo(nTrackNum, m_nCvPlcNum);
		switch (wStatus)
		{
		case enStatusStoSTReady:
			pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrStoST : clrStatusNone);
			break;

		case enStatusRetSTReady:
			pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrRetST : clrStatusNone);			
			break;

		case enStatusCvCheckReady:
		case enStatusArvSTReady:
			pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrArvST: clrStatusNone);
			break;

		case enStatusScStoHSReady:
		case enStatusRtvDepartHSReady:
		case enStatusLfDepartHSReady:
			pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrDepart: clrStatusNone);  	
			break;

		case enStatusScRetHSReady:
		case enStatusRtvArriveHSReady:
		case enStatusLfArriveHSReady:
		case enStatusFullReady:
			pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetStatusData(wStatus) ? pConfig->m_clrArrive : clrStatusNone);
			break;
		case enStatusLgvDepartHSReady:
			if(pTrack != NULL)
			if (pTrack->GetStatusData(1))
			{

/*				if((pTrackHS = pTrack->GetLgvArriveHS()) != NULL)
				{
					if(i == 0)
					{
						int kkk =pTrackCtrl->GetItemSize();
						if (pTrack->GetStatusData(1))
						{
					
							if(pTrack->GetSensorData(1) == TRUE || pTrack->m_nLuggNum != 0)
								pTrackCtrl->m_items[i+1].m_clrItem = clrStatusNone;
							else
								pTrackCtrl->m_items[i+1].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetLGVArriveData(0) ? pConfig->m_clrArrive : clrStatusNone);
							//	pTrackCtrl->m_items[i].m_clrItem = pConfig->m_clrArrive;
						}
						else
							pTrackCtrl->m_items[i-1].m_clrItem = clrStatusNone;
						break;
					}
				}
*/
				if(pTrack->GetSensorData(1) == FALSE)
					pTrackCtrl->m_items[i].m_clrItem = clrStatusNone;
				else
					pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetLGVArriveData(0) ? pConfig->m_clrDepart : clrStatusNone);
				//	pTrackCtrl->m_items[i].m_clrItem = pConfig->m_clrArrive;
			}
			else
				pTrackCtrl->m_items[i].m_clrItem = clrStatusNone;		
			break;			
		case enStatusLgvArriveHSReady:
			if(pTrack != NULL)
			if (pTrack->GetStatusData(1))
			{
				if(pTrack->GetSensorData(1) == TRUE || pTrack->m_nLuggNum != 0)
					pTrackCtrl->m_items[i].m_clrItem = clrStatusNone;
				else
					pTrackCtrl->m_items[i].m_clrItem = m_bSuspend ? pConfig->m_clrSuspend : (GetLGVArriveData(0) ? pConfig->m_clrArrive : clrStatusNone);
				//	pTrackCtrl->m_items[i].m_clrItem = pConfig->m_clrArrive;
			}
			else
				pTrackCtrl->m_items[i].m_clrItem = clrStatusNone;
			break;

		default:
			DEBUGER_ASSERT_VALID(FALSE);
		}
	}

	pTrackCtrl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
	m_pEquipment->RefreshDialog();

	m_bModified = FALSE;
}

CString CTrackInfo::GetXmlString()
{
	CString strXml;
	CString strTime;

	strTime.Format(_T("%04d%02d%02d%02d%02d%02d"), 
		m_tOperStart.GetYear(), m_tOperStart.GetMonth(), m_tOperStart.GetDay(),
		m_tOperStart.GetHour(), m_tOperStart.GetMinute(), m_tOperStart.GetSecond());
	strXml.Format(_T("<TRACK TID='%d' LNO='%d' JT='%d' DST='%d' GEN='%d' ERR='%d' STATUS='%d' SENSOR='%d' MOTOR='%d' SUS='%d' SIZE='%d' TIME='%s'/>"),
		m_nNumber, m_nLuggNum, m_nJobType, m_nDestPos, m_ucGenCode, m_wErrorCode, m_wStatusData, m_wSensorData, m_wMotorData, m_bSuspend, m_wReserved3, strTime);

	return strXml;
}

CString CTrackInfo::GetErrorString()
{
	CString strErrorCode, strErrorMessage;
	strErrorCode.Format(_T("%d"), m_wErrorCode);
	strErrorMessage.Format(_T("등록되지 않은 에러코드 ( CODE=%d )"), m_wErrorCode);

	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("CV_ERROR_CODE"), strErrorCode, (LPCWSTR)strErrorMessage, szTemp, _MAX_PATH, ECS_CODE_FILE);
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

BOOL CTrackInfo::LoadXML()
{
	LoadStation();

	CString strXmlFile, strValue;
	int		nDestPos, nNextTrack, nNextPlc;
	strXmlFile.Format(_T("..\\TRACK\\TRACK#%05d.xml"), m_nNumber);

	CXmlDom dom;
	try
	{
		dom.InitializeXmlDom();
		if (dom.LoadXmlFile(strXmlFile) == FALSE)
		{
			return FALSE;
		}

		dom.MoveRoot();
		dom.GetChildElmtValue(_T("Kind"),	strValue);
		m_nKind = _ttoi(strValue);
		dom.GetChildElmtValue(_T("NextPlcNum"),	strValue);
		m_nNextPlcNum = _ttoi(strValue);
		dom.GetChildElmtValue(_T("NextTrack"),	strValue);
		m_nNextCv = _ttoi(strValue);

		dom.GetChildElmtValue(_T("Center"),	strValue);
		m_bCenter = _ttoi(strValue);
		dom.GetChildElmtValue(_T("CenterNextPlcNum"),	strValue);
		m_nCenterNextPlcNum = _ttoi(strValue);
		dom.GetChildElmtValue(_T("CenterNext"),	strValue);
		m_nCenterNext = _ttoi(strValue);
		dom.GetChildElmtValue(_T("CenterDest"),	strValue);
		m_nCenterDest = _ttoi(strValue);
		dom.GetChildElmtValue(_T("Cross"),	strValue);
		m_bCross = _ttoi(strValue);
		dom.GetChildElmtValue(_T("CrossTrack"),	strValue);
		m_nCrossTrack = _ttoi(strValue);
		dom.GetChildElmtValue(_T("CrossDest"),	strValue);
		m_nCrossDest = _ttoi(strValue);
		dom.GetChildElmtValue(_T("CrossPlcNum"),	strValue);
		m_nCrossPlcNum = _ttoi(strValue);
		dom.GetChildElmtValue(_T("TwinCheck"),	strValue);
		m_bTwinCheck = _ttoi(strValue);
		dom.GetChildElmtValue(_T("HSTrack"),	strValue);
		m_nHSTrack = _ttoi(strValue);

		if (dom.MoveXPath(_T("./Dests"), FALSE))	//Equipment/Tracks
		{
			int i, nCount = dom.GetChildElmtCount();
			
			for (i=0; i<nCount; ++i)
			{
				dom.MoveChild(i);	//Config/Dests/Dest

				dom.GetAttrValue(_T("DestPos"), strValue);
				nDestPos = _ttoi(strValue);
				dom.GetAttrValue(_T("NextPlcNum"), strValue);
				nNextPlc = _ttoi(strValue);
				dom.GetAttrValue(_T("NextTrack"), strValue);
				nNextTrack = _ttoi(strValue);
				SetStation(nDestPos, nNextPlc, nNextTrack);

				dom.MoveParent();	//Config/Dests
			}
		}

	}
	catch ( _com_error& e)
	{
		AfxMessageBox(dom.GetErrorMessage(e));
		return FALSE;
	}

	return TRUE;
}

BOOL CTrackInfo::SaveXML()
{
	CXmlDom dom;
	CString str;

	try
	{
		if (dom.InitializeXmlDom() == FALSE)
		{
			AfxMessageBox(dom.GetErrorMessage());
			return FALSE;
		}

		dom.MoveDocument();
		dom.AddChildProcessingInstruction(_T("xml"), _T("version='1.0' encoding='euc-kr'"));
		dom.AddChildComment(_T("ECS Configuration (SCP2)"));
		dom.AddChildElement(_T("Config"));

		dom.MoveRoot();
		str.Format(_T("%d"), m_nKind);
		dom.AddChildElement(_T("Kind"), str);
		str.Format(_T("%d"), m_nNextPlcNum);
		dom.AddChildElement(_T("NextPlcNum"), str);
		str.Format(_T("%d"), m_nNextCv);
		dom.AddChildElement(_T("NextTrack"), str);
		str.Format(_T("%d"), m_bCenter);
		dom.AddChildElement(_T("Center"), str);
		str.Format(_T("%d"), m_nCenterNextPlcNum);
		dom.AddChildElement(_T("CenterNextPlcNum"), str);
		str.Format(_T("%d"), m_nCenterNext);
		dom.AddChildElement(_T("CenterNext"), str);
		str.Format(_T("%d"), m_nCenterDest);
		dom.AddChildElement(_T("CenterDest"), str);
		str.Format(_T("%d"), m_bCross);
		dom.AddChildElement(_T("Cross"), str);
		str.Format(_T("%d"), m_nCrossTrack);
		dom.AddChildElement(_T("CrossTrack"), str);
		str.Format(_T("%d"), m_nCrossDest);
		dom.AddChildElement(_T("CrossDest"), str);
		str.Format(_T("%d"), m_nCrossPlcNum);
		dom.AddChildElement(_T("CrossPlcNum"), str);
		str.Format(_T("%d"), m_bTwinCheck);
		dom.AddChildElement(_T("TwinCheck"), str);
		str.Format(_T("%d"), m_nHSTrack);
		dom.AddChildElement(_T("HSTrack"), str);

		dom.AddChildElement(_T("Dests"));
		
		if (dom.MoveXPath(_T("/Config/Dests")))
		{
			int nLen = m_nStationArray.GetSize();
			CString strStation, strNextTrack, strNextPlc;

			for (int i=0; i<nLen; ++i)
			{
				strStation.Format(_T("%d"), m_nStationArray[i]);
				strNextTrack.Format(_T("%d"), m_nNextTrArray[i]);
				strNextPlc.Format(_T("%d"), m_nNextPlcArray[i]);

				dom.AddChildElement(_T("Dest"));

				dom.MoveChildLast();
				dom.AddChildAttribute(_T("DestPos"),	strStation);
				dom.AddChildAttribute(_T("NextPlcNum"),	strNextPlc);
				dom.AddChildAttribute(_T("NextTrack"),	strNextTrack);
				dom.MoveParent();
			}
		}

		CString strXmlFile;
		strXmlFile.Format(_T("..\\TRACK\\TRACK#%05d.xml"), m_nNumber);

		if (dom.SaveXmlFile(strXmlFile) == FALSE)
		{
			AfxMessageBox(dom.GetErrorMessage());
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(dom.GetErrorMessage(e));
		return FALSE;
	}

	return TRUE;
}

void CTrackInfo::LoadStation()
{
	CEcsDoc* pDoc = m_pEquipment->m_pDoc;

	/*
	int nLen = m_nStationArray.GetSize();

	if (nLen > 0) return;
	*/

	int nLen = pDoc->m_pStationInfos.GetSize();
	m_nStationArray.SetSize(nLen);
	m_nFloorArray.SetSize(nLen);
	m_nNextPlcArray.SetSize(nLen);
	m_nNextTrArray.SetSize(nLen);
		
	CStationInfo* pStation = NULL;
	for (int i=0; i<pDoc->m_pStationInfos.GetSize(); ++i)
	{
		pStation = pDoc->m_pStationInfos[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		CString strTemp;
		int nTemp;
		
		switch (pStation->m_enKind)
		{
		case CStationInfo::enScStation:
			nTemp = pStation->GetTrackDestination();
			strTemp.Format(_T("[%04d] %s"), nTemp, pStation->m_strName);

			m_nStationArray[i] = nTemp;
			m_nFloorArray[i] = m_nTabNum;
			m_nNextPlcArray[i] = 0;
			m_nNextTrArray[i] = 0;
			//m_cbxDestTrack.AddString(strTemp);
			//m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, nTemp);
			break;

		case CStationInfo::enRetStation:
		case CStationInfo::enArvStation:
		case CStationInfo::enArvStation2:
		case CStationInfo::enStoStation:
			nTemp = pStation->m_pTrack->m_nNumber;
	//		nTemp = _ttoi(pStation->m_strID);
			if(nTemp == 0)
				nTemp = pStation->m_pTrack->m_nNumber;

			strTemp.Format(_T("[%04d] %s"), nTemp, pStation->m_strName);

			m_nStationArray[i] = nTemp;
			m_nFloorArray[i] = pStation->m_pTrack->m_nTabNum;
			m_nNextPlcArray[i] = 0;
			m_nNextTrArray[i] = 0;
			//m_cbxDestTrack.AddString(strTemp);
			//m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, nTemp);
			break;
		case CStationInfo::enFullStation:
			nTemp = pStation->m_pTrack->m_nNumber;
			strTemp.Format(_T("[%04d] %s"), nTemp, pStation->m_strName);

			m_nStationArray[i] = nTemp;
			m_nFloorArray[i] = pStation->m_pTrack->m_nTabNum;
			m_nNextPlcArray[i] = 0;
			m_nNextTrArray[i] = 0;
			break;
		case CStationInfo::enLgvStation:
			nTemp = pStation->m_pTrack->m_nNumber;
			strTemp.Format(_T("[%04d] %s"), nTemp, pStation->m_strName);

			m_nStationArray[i] = nTemp;
			m_nFloorArray[i] = pStation->m_pTrack->m_nTabNum;
			m_nNextPlcArray[i] = 0;
			m_nNextTrArray[i] = 0;
			break;
		case CStationInfo::enEquipStation:
			//nTemp = pStation->m_pTrack->m_nNumber;
			nTemp = _ttoi(pStation->m_strID);
			if(nTemp == 0)
				nTemp = pStation->m_pTrack->m_nNumber;

			strTemp.Format(_T("[%04d] %s"), nTemp, pStation->m_strName);

			m_nStationArray[i] = nTemp;
			m_nFloorArray[i] = 0;
			m_nNextPlcArray[i] = 0;
			m_nNextTrArray[i] = 0;
			//m_cbxDestTrack.AddString(strTemp);
			//m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, nTemp);
			break;
		}
	}
}

void CTrackInfo::SetStation(int nStation, int nNextPlc, int nNextTrack)
{
	int nLen1 = m_nStationArray.GetSize();
	int nLen2 = m_nNextTrArray.GetSize();

	if (nLen1 != nLen2)
		return;

	for (int i=0; i<nLen1; ++i)
	{
		int nnn = m_nStationArray[i];
		if (m_nStationArray[i] == nStation)
		{
			m_nNextPlcArray[i] = nNextPlc;
			m_nNextTrArray[i] = nNextTrack;
		}
	}

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrackInfo* CTrackInfoArray::GetTrackInfo(int nNumber, int nCvPlcNum)
{
	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = GetAt(i);
		DEBUGER_ASSERT_VALID(pTrack != NULL);

		if (nCvPlcNum == 0)
		{
			if (pTrack->m_nNumber == nNumber)
				return pTrack;
		}
		else
		{
			if (pTrack->m_nNumber == nNumber && pTrack->m_nCvPlcNum == nCvPlcNum)
				return pTrack;
		}
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
	
		pTrack->InvokeControl(pTrack->m_pTrackCtrl); 
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

