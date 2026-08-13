// ScDongJin.cpp: implementation of the CSc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "ScDongJin.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEVICE m_port.m_strDevice

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CScDongJin, CEquipment)

CScDongJin::CScDongJin(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice) : CEquipment(pDoc, nIndex, nNumber, lpszDevice)
{
	m_enKind = CEquipment::enSC;
//	m_pInfo = new CScInfo(this);
//	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
//	m_nNumber = nNumber
	m_pInfo.RemoveAll();

}

CScDongJin::~CScDongJin()
{
//	delete m_pInfo;
	for (int n = 0; n <= m_pInfo.GetUpperBound(); n++)
	{
		delete m_pInfo[n];
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CScDongJin::IsIdleJob(int nLuggNum) 
{
	BOOL bTemp = TRUE;
	for (int n = 0; n <= m_pInfo.GetUpperBound(); n++)
	{
		CScInfo* pInfo = (CScInfo*) GetInfo(n + 1, FALSE);
		
		if (pInfo == NULL)
			continue;

		int nInternalLuggNum = pInfo->m_ScInfo[0].m_nInternalLuggNum;
		if ((nInternalLuggNum != 0) && (nInternalLuggNum == nLuggNum))
		{
			bTemp = FALSE;
			break;
		}
		else 
		{
			bTemp = TRUE;
		}

	}
	return bTemp;
}

void CScDongJin::MulticastInfo(CMonitorServer* pMonitorSv)
{
//==========================================================================================================	
//	// PlayBack을 위해서 
//==========================================================================================================
//	m_pInfo->m_pEquipment->m_pDoc->m_pLogPlayback->Log(strSend);	
//----------------------------------------------------------------------------------------------------------

	CString strSend;
	for (int n = 0; n <= m_pInfo.GetUpperBound(); n++)
	{
		strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'>%s</EQUIP></ECS>%c"), 
			CMonitorServer::enSTX, m_port.m_strDevice, m_pInfo[n]->GetXmlString(), CMonitorServer::enETX);

//		if (pMonitorSv)
//		{
//			pMonitorSv->SendData(strSend);
//		}
//		else
//		{
//			if (m_pDoc->m_pMonitorListener && m_pDoc->m_pMonitorListener->GetClientCount())
//				m_pDoc->m_pMonitorListener->SendToClients(strSend);
//		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CScDongJin::AutoRunProc()
{
	UpdateCommStatus(NOTIFY_SEND);

	int nCnt = m_pInfo.GetSize();
//	for (int n = 0; n <= m_pInfo.GetUpperBound(); n++)		// Connection 별로 반복
//	{
		int n = m_nNumber - 1;
		for (int i = 0 ; i < nCnt ; i++)						// SC 호기 별로 반복
		{
			int nEquipNum = (n * nCnt) + (i + 1);

			CScInfo* pInfo = (CScInfo *)GetInfo(nEquipNum, FALSE);

			if (pInfo == NULL)
				continue;
			
			if (ReadStatus(pInfo))
			{
				if (pInfo->IsScJobStatusError())
				{
					ErrorCheck(pInfo);
					ErrorRoutine(pInfo);
				}
				else
				{
					if ((pInfo->m_nErrorCode == 0) && 
						(pInfo->m_nPrevErrorCode != 0))
					{
						pInfo->m_nPrevErrorCode = 0;
						pInfo->Backup();
					}

					if (pInfo->m_ScInfo[0].m_bInvoke)
					{
						CompleteCheck(pInfo);
//						ErrorResetCheck(pInfo);
					}
					else
						InvokeCheck(pInfo);
				}

				UpdateCommStatus(NOTIFY_REFRESH);
			}
			else
			{
				ErrorCheck(pInfo);
				UpdateCommStatus(NOTIFY_ERROR);
			}
			SafeCheck(pInfo);
		}
//	}
}

void CScDongJin::CommandProc()
{
	CString strLog, strTempDepart, strTempArrive;
	CCmdMsg* pCmdMsg = NULL;
	for (POSITION pos=m_cmdList.GetHeadPosition(); pos!=NULL; )
	{
		pCmdMsg = (CCmdMsg*)m_cmdList.GetNext(pos);
		CStringArray& strValues = pCmdMsg->m_strValues;
		DEBUGER_ASSERT_VALID(pCmdMsg != NULL);

		int n = (int)pCmdMsg->m_nSubCmd;

		CScInfo* pInfo = (CScInfo*) GetInfo(n, FALSE);
		
		if (pInfo == NULL)
			continue;
		
//		pCmdMsg->m_nCommand++;
		switch (pCmdMsg->m_nCommand)
		{
		case CScCmdMsg::enCmdStoSuspend:
			{
				DEBUGER_ASSERT_VALID(strValues.GetSize() == 1);
				pInfo->m_bStoSuspend = _ttoi(strValues[0]);
				pInfo->m_bModified = TRUE;
				pInfo->InvokeControl();
				pInfo->StatusReport();

				strLog.Format(_T("SC #%d 입고 일시정지"), n);
				m_pDoc->StatusDisplay(strLog, FALSE);
			}
			break;

		case CScCmdMsg::enCmdRetSuspend:
			{
				DEBUGER_ASSERT_VALID(strValues.GetSize() == 1);
				pInfo->m_bRetSuspend = _ttoi(strValues[0]);
				pInfo->m_bModified = TRUE;
				pInfo->InvokeControl();
				pInfo->StatusReport();

				strLog.Format(_T("SC #%d 출고 일시정지"), n);
				m_pDoc->StatusDisplay(strLog, FALSE);
			}
			break;
/*
		case CScCmdMsg::enCmdR2RSuspend:
			{
				DEBUGER_ASSERT_VALID(strValues.GetSize() == 1);
				pInfo->m_bR2RSuspend = _ttoi(strValues[0]);
				pInfo->m_bModified = TRUE;
				pInfo->InvokeControl();
				pInfo->StatusReport();

				strLog.Format(_T("SC #%d  랙투랙 일시정지"), n);
				m_pDoc->StatusDisplay(strLog, FALSE);
			}
			break;
*/
		case CScCmdMsg::enCmdManualSto:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			if (Store(_ttoi(strValues[0]), strValues[2], _ttoi(strValues[1]), pInfo->m_nEquipNum))
			{
				//@@
				pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
				pInfo->m_ScInfo[0].m_nInternalJobType = (_ttoi(strValues[3]) == 1) ? enJobTypeAutoSto : enJobTypeManual;
				pInfo->m_bModified = TRUE;
				pInfo->Backup();

				strLog.Format(_T("SC #%d  입고지시 [작업번호=%s, 출발위치=%s, 도착위치=%s]"), n, strValues[0], strValues[2], strValues[1]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  입고지시 실패! [작업번호=%s, 출발위치=%s, 도착위치=%s] [%s]"), n, strValues[0], strValues[2], strValues[1], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScDongJin::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdManualRet:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);

			{
				CString strTemp = strValues[0];
				strTemp = strValues[1];
				strTemp = strValues[2];
				strTemp = strValues[3];
//				strTemp = strValues[4];
			}
			if (Retrieve(_ttoi(strValues[0]), strValues[1], _ttoi(strValues[2]), pInfo->m_nEquipNum))
			{
				pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
				pInfo->m_ScInfo[0].m_nInternalJobType = (_ttoi(strValues[3]) == 1) ? enJobTypeAutoRet : enJobTypeManual;
				pInfo->m_bModified = TRUE;
				pInfo->Backup();

				strLog.Format(_T("SC #%d  출고지시 [작업번호=%s, 출발위치=%s, 도착위치=%s]"), n, strValues[0], strValues[1], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  출고지시 실패! [작업번호=%s, 출발위치=%s, 도착위치=%s] [%s]"), n, strValues[0], strValues[1], strValues[2], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScDongJin::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdManualHs2Hs:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			if (HsToHs(_ttoi(strValues[0]), _ttoi(strValues[1]), _ttoi(strValues[2]), pInfo->m_nEquipNum))
			{
				pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
				pInfo->m_ScInfo[0].m_nInternalJobType = (_ttoi(strValues[3]) == 1) ? enJobTypeAutoMove : enJobTypeManual;
				pInfo->m_bModified = TRUE;
				pInfo->Backup();

				strLog.Format(_T("SC #%d  직출고지시 [작업번호=%s, 출발위치=%s, 도착위치=%s]"), n, strValues[0], strValues[1], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  직출고지시 실패! [작업번호=%s, 출발위치=%s, 도착위치=%s] [%s]"), n, strValues[0], strValues[1], strValues[2], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScDongJin::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdManualR2R:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			if (RackToRack(_ttoi(strValues[0]), strValues[1], strValues[2], pInfo->m_nEquipNum))
			{
				pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
				pInfo->m_ScInfo[0].m_nInternalJobType = (_ttoi(strValues[3]) == 1) ? enJobTypeAutoR2R : enJobTypeManual;
				pInfo->m_bModified = TRUE;
				pInfo->Backup();

				strLog.Format(_T("SC #%d  랙투랙지시 [작업번호=%s, 출발위치=%s, 도착위치=%s]"), n, strValues[0], strValues[1], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  랙투랙지시 실패! [작업번호=%s, 출발위치=%s, 도착위치=%s] [%s]"), n, strValues[0], strValues[1], strValues[2], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScDongJin::CommandProc"));
			}
			break;
		case CScCmdMsg::enCmdHomeReturn:
			if (CallToHome(n))
			{
// 					pInfo->m_ScInfo[0].m_bInvoke = TRUE;
// 					pInfo->m_ScInfo[0].m_nInternalLuggNum = LOG_SYSTEM;
// 					pInfo->m_ScInfo[0].m_nInternalJobType = enJobTypeManual;
// 					pInfo->m_bModified = TRUE;
// 					pInfo->Backup();

				strLog.Format(_T("SC #%d  홈복귀지시"), n);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  홈복귀지시 실패 [%s]"), n, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::CommandProc"));
			}
			break;

/*
		case CScCmdMsg::enCmdNewLocation:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			{
				CJobItem* pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValues[0]));
				if (pJobItem != NULL)
				{
					strLog.Format(_T("SC #%d  이중입고 재지정 지시 [LOC=%s]"), n, strValues[1]);
					WriteLog(LOG_TYPE_EVENT, strLog, _T("CScDongJin::CommandProc"), pJobItem);

					pJobItem->m_strDestLoc = strValues[1];
					pJobItem->SetJobStatus(enJobStatusDualStoreRetry);
					m_pDoc->m_pJob->Backup();
					ErrorRoutine(pInfo);
				}
				else
				{
					strLog.Format(_T("SC #%d  이중입고 재지정 지시 실패! [LOC=%s]"), n, strValues[1]);
					WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScDongJin::CommandProc"));
				} 
			}
			break;
*/

		case CScCmdMsg::enCmdComplete:
			{
				strLog.Format(_T("SC #%d  강제완료"), n);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScDongJin::CommandProc"));

				pInfo->SetScJobStatus(	CScInfo::enScJobStatusJobComplete | 
										CScInfo::enScJobStatusIdle | 
//										CScInfo::enScJobStatusSensorProduct | 
										CScInfo::enScJobStatusForkCenter);
				CompleteCheck(pInfo);

			}
			break;

		case CScCmdMsg::enCmdEmergency:
			if (Emergency(n))
			{
				strLog.Format(_T("SC #%d  EMERGENCY지시"), n);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  EMERGENCY지시 실패 [%s]"), n, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdActive:
			if (Active(pInfo))
			{
				strLog.Format(_T("SC #%d  ACTIVE지시"), n);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  ACTIVE지시 실패 [%s]"), n, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdStop:
			if (Stop(n))
			{
				strLog.Format(_T("SC #%d  STOP지시"), n);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  STOP지시 실패 [%s]"), n, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdReset:
			if (Reset(n))
			{
				strLog.Format(_T("SC #%d  RESET지시"), n);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  RESET지시 실패 [%s]"), n, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdCancel:
 			if (Delete(n))
			{
				pInfo->m_ScInfo[0].m_bInvoke = FALSE;
				pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
				pInfo->m_ScInfo[0].m_nInternalJobType = enJobTypeNone;
				pInfo->m_bModified = TRUE;
				pInfo->Backup();
				
				strLog.Format(_T("SC #%d  FORK1 DATA DELETE지시"), n);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScDongJin::CommandProc"));
			}
			else
			{
				strLog.Format(_T("SC #%d  FORK1 DATA DELETE지시 실패 [%s]"), n, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::CommandProc"));
			}
			break;

		default:
			m_strLog.Format(_T("SC #%d  INVALID COMMAND!"), n);
			WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CScDongJin::CommandProc"));
		}

		delete pCmdMsg;		// 죽으면 주석!
		pCmdMsg = NULL;		// 죽으면 주석!
	}

	m_cmdList.RemoveAll();	// 죽으면 주석!
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CScDongJin::GetHsNum(BYTE ucCmdType, int nNumber, int nBank, int nBay, int nLevel)
{
	int nTemp = nNumber * 2;
	switch(ucCmdType) 
	{
	case CScInfo::enStore:
		if (nLevel == 2 )
		{
			if (nBay == 1 && nBank == --nTemp )			return 1;
			if (nBay == 24 && nBank == nTemp)			return 3;
		}
		break;
	case CScInfo::enRetrieve:
		if (nLevel == 2)
		{
			if (nBay == 1 && nBank == nTemp)			return 2;
			if (nBay == 24 && nBank == --nTemp)			return 4;
		}
		break;
/*	case CScInfo::enRack2Rack:
		if (nLevel == 2 && nBank == --nTemp)
		{
			if (nBay == 1)			return 2;
			if (nBay == 24)			return 4;
		}
		break;
*/
	}
	return 0;
}

BOOL CScDongJin::ReadStatus(CScInfo* pInfo)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), pInfo->m_nEquipNum);
		return FALSE;
	}

	if (pInfo == NULL)
	{
		m_strLog.Format(_T("SC #%d  SC 정보를 가져오지 못했습니다."), pInfo->m_nEquipNum);
		return FALSE;		
	}

	m_pSocket->m_bWriteLog = FALSE;

	CByteArray arrRxData;
	CByteArray arrBufferRxData;
	int nReadWordSize = 12;		//36;
	int nReadBufferWordSize = 6;		//36;
	static int nCount = 0;
	CString strTemp;

	int nAddress, nBufferAddress;
	switch(pInfo->m_nEquipNum) 
	{
	case 1:		nAddress = 100;		nBufferAddress = 10;		break;
	case 2:		nAddress = 120;		nBufferAddress = 20;		break;
	case 3:		nAddress = 140;		nBufferAddress = 30;		break;
	case 4:		nAddress = 100;		nBufferAddress = 10;		break;
	case 5:		nAddress = 120;		nBufferAddress = 20;		break;
	case 6:		nAddress = 140;		nBufferAddress = 30;		break;
	}

	// 구문 변경 요망
	if (!m_pSocket->ReadWord(arrRxData, nAddress, nReadWordSize))
	{
		if (++nCount >= 10)
		{
			pInfo->m_nErrorCode = enScErrorCommunication;
			pInfo->SetScJobStatus(CScInfo::enScJobStatusError);
			pInfo->m_bModified = TRUE;
		}

		return FALSE;
	}
	nCount = 0;

	m_pSocket->m_bWriteLog = m_port.m_bWriteLog;

	///////////////////////////////////////////////////////////////////////////////////
	
	if (arrRxData.GetSize() < nReadWordSize*2)
		return FALSE;

	CString strDepartInfo = "";
	CString strArriveInfo = "";
	int nFromBank, nFromBay, nFromLevel;
	int nToBank, nToBay, nToLevel;
	
	pInfo->SetScJobStatus(CLib::SwapToWord(arrRxData.GetData() + 0));			// D100, D120, D140
	pInfo->SetErrorCode(CLib::SwapToWord(arrRxData.GetData() + 2));				// D101, D121, D141
	pInfo->SetHorizontalPos(CLib::SwapToWord(arrRxData.GetData() + 4));			// D102, D122, D142
	pInfo->SetVerticalPos(CLib::SwapToWord(arrRxData.GetData() + 6));			// D103, D123, D143
	pInfo->SetCmdType((BYTE)arrRxData[8]);										// D104, D124, D144

	nFromBank	= (int)CLib::SwapToWord(arrRxData.GetData() + 10);				// D105, D125, D145
	nFromBay	= (int)CLib::SwapToWord(arrRxData.GetData() + 12);				// D106, D126, D146
	nFromLevel	= (int)CLib::SwapToWord(arrRxData.GetData() + 14);				// D107, D127, D147
	nToBank		= (int)CLib::SwapToWord(arrRxData.GetData() + 16);				// D108, D128, D148
	nToBay		= (int)CLib::SwapToWord(arrRxData.GetData() + 18);				// D109, D129, D149
	nToLevel	= (int)CLib::SwapToWord(arrRxData.GetData() + 20);				// D110, D130, D150

	pInfo->SetScDoorStatus(CLib::SwapToWord(arrRxData.GetData() + 22));			// D111, D131, D151

	if(nFromBank == 0 && nFromBay ==0 && nFromLevel == 0 && pInfo->m_ucCmdType == 0 && nToBank == 0 && nToBay == 0 && nToLevel ==0 )
		pInfo->m_bBufferEmpty = TRUE;
	else
		pInfo->m_bBufferEmpty = FALSE;

	strDepartInfo.Format(_T("%02d%02d%02d00"), nFromBank, nFromBay, nFromLevel);
	strArriveInfo.Format(_T("%02d%02d%02d00"), nToBank, nToBay, nToLevel);

	CString strTempDepartInfo = strDepartInfo.Mid(0, 6);
	CString strTempArriveInfo = strArriveInfo.Mid(0, 6);

	// HS를 표현하지 않음??
	int	nFromHsNum	= GetHsNum(pInfo->m_ucCmdType, pInfo->m_nEquipNum, nFromBank, nFromBay, nFromLevel);
	int nToHsNum	= GetHsNum(pInfo->m_ucCmdType, pInfo->m_nEquipNum, nToBank, nToBay, nToLevel);
	
	if (nFromHsNum != 0)		strDepartInfo.Format(_T("%s%02d"), strTempDepartInfo, nFromHsNum);
	if (nToHsNum != 0)			strArriveInfo.Format(_T("%s%02d"), strTempArriveInfo, nToHsNum);

	pInfo->SetDepartInfo(strDepartInfo, 1-1);						// D105 ~ D107
	pInfo->SetArriveInfo(strArriveInfo, 1-1);						// D108 ~ D110

//	int nBufferJobType		= (int)CLib::SwapToWord(arrBufferRxData.GetData() +  0);				// 
//	int nBufferFromBank		= (int)CLib::SwapToWord(arrBufferRxData.GetData() +  2);				// 
//	int nBufferFromBay		= (int)CLib::SwapToWord(arrBufferRxData.GetData() +  4);				// 
//	int nBufferFromLevel	= (int)CLib::SwapToWord(arrBufferRxData.GetData() +  6);				// 
//	int nBufferToBank		= (int)CLib::SwapToWord(arrBufferRxData.GetData() +  8);				// 
//	int nBufferToBay		= (int)CLib::SwapToWord(arrBufferRxData.GetData() + 10);				// 
//	int nBufferToLevel		= (int)CLib::SwapToWord(arrBufferRxData.GetData() + 12);				// 

	pInfo->InvokeControl();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
CString CSc::MiddleData(const CByteArray arrData, int nFirst, int nCount)
{
	CString strTemp, strReturnData = "";

	for(int i = 0 ; i < nCount ; i++)
		strReturnData += strTemp.Format("%c", arrData.GetData() + nFirst);

	return strReturnData;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int CScDongJin::ManualJob()
//{
//	GetHsNum(BYTE ucCmdType, int nNumber, int nBank, int nBay, int nLevel)
//}

//int CSc::Store(int nLuggNum, CString strLocation, int nHsNum, int nSize /* = 0 */, BOOL bStoHS /* = FALSE */, BOOL bRetHS /* = FALSE */)
int CScDongJin::Store(int nLuggNum, CString strLocation, int nHsNum, int n, int nSize /* = 0 */)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
//	int nUseFork;

	int nAddress, nBank;
	switch(n) 
	{
	case 1:		nAddress = 10;		nBank = (nHsNum == 1) ?  1 :  2;			break;
	case 2:		nAddress = 20;		nBank = (nHsNum == 1) ?  3 :  4;			break;
	case 3:		nAddress = 30;		nBank = (nHsNum == 1) ?  5 :  6;			break;
	case 4:		nAddress = 10;		nBank = (nHsNum == 1) ?  7 :  8;			break;
	case 5:		nAddress = 20;		nBank = (nHsNum == 1) ?  9 : 10;			break;
	case 6:		nAddress = 30;		nBank = 11;/*(nHsNum == 1) ? 11 : 12;*/		break;
	}


	TxBuff[0] = 1;									// D10, D20, D30 - Store - DualStore

	TxBuff[2]  = nBank;								// D11, D21, D31 - From Bank
	TxBuff[4]  = (nHsNum == 1) ? 1 : 24;			// D12, D22, D32 - From Bay
	TxBuff[6]  = 2;									// D13, D23, D33 - From Level

	TxBuff[8]  = CLib::GetBank(strLocation);		// D14, D24, D34 - TO Bank
	TxBuff[10] = CLib::GetBay(strLocation);			// D15, D25, D35 - TO Bay
	TxBuff[12] = CLib::GetLevel(strLocation);		// D16, D26, D36 - TO Level



	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 7);
}

//int CSc::Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nSize /* = 0 */, BOOL bStoHS /* = FALSE */, BOOL bRetHS /* = FALSE */)
int CScDongJin::Retrieve(int nLuggNum, CString strLocation, int nHsNum, int n, int nSize /* = 0 */)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
//	int nUseFork;

	int nAddress, nBank;
	switch(n) 
	{
	case 1:		nAddress = 10;		nBank = (nHsNum == 2) ?  2 :  1;		break;
	case 2:		nAddress = 20;		nBank = (nHsNum == 2) ?  4 :  3;		break;
	case 3:		nAddress = 30;		nBank = (nHsNum == 2) ?  6 :  5;		break;
	case 4:		nAddress = 10;		nBank = 8;								break;
	case 5:		nAddress = 20;		nBank = 10;								break;
	case 6:		nAddress = 30;		nBank = 12;								break;
	}

	TxBuff[0] = 2;									// D10, D20, D30 - Retrieve

	// 작업번호 관리는 안하는 것인지?
	/*

	if(nLuggNum != 0)
	{
		TxBuff[2] = nLuggNum & 0xFF;				// D172 LOW 
		TxBuff[3] = (nLuggNum >> 8) & 0xFF;		// D172 HIGH

		TxBuff[4] = CLib::GetSide(strLocation);			// D173 Bank   LOW
		TxBuff[6] = CLib::GetBay(strLocation);			// D174 Bay    LOW
		TxBuff[8] = CLib::GetLevel(strLocation);			// D175 Level  LOW

		TxBuff[18] = (BYTE)nHsNum;					// D176 Dest Home Stand  LOW
	}

//	TxBuff[20] = 0;							// D181 (0 - USE FORK#1) (1 - USE FORK#1, #2) (2 - USE FORK#2) --- LO

	TxBuff[40] = 1;									// D191 WRITE COMPLETE DEFAULT SETTING  -LOW
//	TxBuff[42] = nSize;						        // D192 Gen   - LOW
	TxBuff[42] = (BYTE)nSize;						// D192 Gen   - LOW
//	TxBuff[44] = (BYTE)bStoHS;						// D193 StoHS - HIGH
//	TxBuff[46] = (BYTE)bRetHS;						// D194 RetHS - HIGH
	*/



	TxBuff[2]  = CLib::GetBank(strLocation);		// D11, D21, D31 - From Bank
	TxBuff[4]  = CLib::GetBay(strLocation);			// D12, D22, D32 - From Bay
	TxBuff[6]  = CLib::GetLevel(strLocation);		// D13, D23, D33 - From Level

	TxBuff[8]  = nBank;								// D14, D24, D34 - TO Bank
	TxBuff[10] = (nHsNum == 2) ? 1 : 24;			// D15, D25, D35 - TO Bay
	TxBuff[12] = 2;									// D16, D26, D36 - TO Level

	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 7);

}

//int CSc::HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nSize /* = 0 */, BOOL bStoHS /* = FALSE */, BOOL bRetHS /* = FALSE */)
int CScDongJin::HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int n, int nSize /* = 0 */)			// 카길에서는 사용하지 않는듯!
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
//	int nUseFork;

	int nAddress, nFromBank, nToBank;

	switch(n) 
	{
	case 1:		nAddress = 10;		nFromBank = (nHsNumFrom == 1) ?  1 :  2;	nToBank = (nHsNumTo == 2) ?  2 :  1;		break;
	case 2:		nAddress = 20;		nFromBank = (nHsNumFrom == 1) ?  3 :  4;	nToBank = (nHsNumTo == 2) ?  4 :  3;		break;
	case 3:		nAddress = 30;		nFromBank = (nHsNumFrom == 1) ?  5 :  6;	nToBank = (nHsNumTo == 2) ?  6 :  5;		break;
	case 4:		nAddress = 10;		nFromBank = (nHsNumFrom == 1) ?  7 :  8;	nToBank = 8;								break;
	case 5:		nAddress = 20;		nFromBank = (nHsNumFrom == 1) ?  9 : 10;	nToBank = 10;								break;
	case 6:		nAddress = 30;		nFromBank = (nHsNumFrom == 1) ? 11 : 12;	nToBank = 12;								break;
	}

	TxBuff[0] = 5;								    // D171 - Store - DualStore
	TxBuff[2]  = nFromBank;		// D11, D21, D31 - From Bank
	TxBuff[4]  = (nHsNumFrom == 1) ? 1 : 24;;			// D12, D22, D32 - From Bay
	TxBuff[6]  = 2;		// D13, D23, D33 - From Level

	TxBuff[8]  = nToBank;								// D14, D24, D34 - TO Bank
	TxBuff[10] = (nHsNumTo == 2) ? 1 : 24;			// D15, D25, D35 - TO Bay
	TxBuff[12] = 2;									// D16, D26, D36 - TO Level
/*
	if(nLuggNum != 0)
	{
		TxBuff[2] = nLuggNum & 0xFF;				// D172 LOW 
		TxBuff[3] = (nLuggNum >> 8) & 0xFF;		// D172 HIGH

		TxBuff[10] = (BYTE)nHsNumFrom;				// D176 Start Home Stand  LOW
		TxBuff[18] = (BYTE)nHsNumTo;				// D176 Dest Home Stand  LOW
	}

//	TxBuff[20] = nUseFork;							// D181 (0 - USE FORK#1) (1 - USE FORK#1, #2) (2 - USE FORK#2) --- LO

	TxBuff[40] = 1;									// D191 WRITE COMPLETE DEFAULT SETTING  -LOW
//	TxBuff[42] = nSize;						        // D192 Gen   - LOW
	TxBuff[42] = (BYTE)nSize;						// D192 Gen   - LOW
//	TxBuff[44] = (BYTE)bStoHS;						// D193 StoHS - HIGH
//	TxBuff[46] = (BYTE)bRetHS;						// D194 RetHS - HIGH
*/	
//	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 7);
	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 7);

}

BOOL CScDongJin::RackToRack(int nLuggNum, CString strLocFrom, CString strLocTo, int n, int nSize /* = 0 */)		// 카길에서는 사용하지 않는듯!
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
//	int nUseFork;

	int nAddress;
//	nFromBank = _ttoi(strLocFrom.Mid(2,1));
//	nToBank = _ttoi(strLocTo.Mid(2,1));
	
//======================== 2014.7.10 수정=================================================//
// 크레인번호에 따라 D주소 지정 후 Bank, Bay, Level 쓰기
//========================================================================================//
	switch(n) 
	{
	case 1:		nAddress = 10;			break;
	case 2:		nAddress = 20;			break;
	case 3:		nAddress = 30;			break;
	case 4:		nAddress = 10;			break;
	case 5:		nAddress = 20;			break;
	case 6:		nAddress = 30;			break;
	}

	if(nLuggNum != 0)
	{
		TxBuff[0] = 3;								 // D10 -1, 4 | D20 - 2, 5 | D30 - 3, 6
		TxBuff[2]  = _ttoi(strLocFrom.Mid(0,2));		//From Bank
		TxBuff[4]  = _ttoi(strLocFrom.Mid(2,3));		//From Bay
		TxBuff[6]  = _ttoi(strLocFrom.Mid(5,2));		//From Level

		TxBuff[8]  = _ttoi(strLocTo.Mid(0,2));		//To Bank
		TxBuff[10] = _ttoi(strLocTo.Mid(2,3));		//To Bay
		TxBuff[12] = _ttoi(strLocTo.Mid(5,2));		//To Level
	}
	
	/*
	if(nLuggNum != 0)
	{
		TxBuff[2] = nLuggNum & 0xFF;		       // D172 LOW  //작업번호 관리 안함
		TxBuff[3] = (nLuggNum >> 8) & 0xFF;       // D172 HIGH

		TxBuff[4] = CLib::GetSide(strLocFrom);          // D105 Bank   LOW
		TxBuff[6] = CLib::GetBay(strLocFrom);           // D106 Bay    LOW
		TxBuff[8] = CLib::GetLevel(strLocFrom);         // D107 Level  LOW

		TxBuff[12] = CLib::GetSide(strLocTo);           // D108 Bank   LOW
		TxBuff[14] = CLib::GetBay(strLocTo);            // D109 Bay    LOW
		TxBuff[16] = CLib::GetLevel(strLocTo);		   // D110 Level  LOW
	}

  */

//	TxBuff[20] = nUseFork;						  // D181 (0 - USE FORK#1) (1 - USE FORK#1, #2) (2 - USE FORK#2) --- LO

//	TxBuff[40] = 1;						          // D191 WRITE COMPLETE DEFAULT SETTING  -LOW
//	TxBuff[42] = nSize;						      // D192 Gen   - LOW
//	TxBuff[42] = (BYTE)nSize;                     // D192 Gen   - LOW

												  
												  
	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 7);
	
}


BOOL CScDongJin::CallToHome(int n)			
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	int nAddress, nBank, nBay = 1, nLevel = 1;
	switch(n) 
	{
	case 1:		nAddress = 10;		nBank = 1;		break;
	case 2:		nAddress = 20;		nBank = 3;		break;
	case 3:		nAddress = 30;		nBank = 5;		break;
	case 4:		nAddress = 10;		nBank = 7;		break;
	case 5:		nAddress = 20;		nBank = 9;		break;
	case 6:		nAddress = 30;		nBank = 11;		break;
	}

	TxBuff[0] = 5;									// D10, D20, D30 - 이동 - CallToHome

//	TxBuff[2]  = nBank;								// D11, D21, D31 - From Bank
//	TxBuff[4]  = nBay;									// D12, D22, D32 - From Bay
//	TxBuff[6]  = nLevel;									// D13, D23, D33 - From Level

//	TxBuff[8]  = nBank;									// D14, D24, D34 - TO Bank
//	TxBuff[10] = nBay;									// D15, D25, D35 - TO Bay
//	TxBuff[12] = nLevel;									// D16, D26, D36 - TO Level

	
//	TxBuff[40] = 1;								// D191 WRITE COMPLETE DEFAULT SETTING  -LOW

	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 7);
}

BOOL CScDongJin::Emergency(int n)			// 동진에서는 사용안함!
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 1; 
//	TxBuff[0] = '1';  
	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);
	
}

BOOL CScDongJin::Active(CScInfo* pInfo)
{
//======================================================================================================================================================
//	// 재기동지시로 변경
//======================================================================================================================================================
//	// 원본 소스
//	BYTE	TxBuff[256];
//	memset(TxBuff, 0x00, sizeof(TxBuff));
//
//
//	TxBuff[0] = 2;    
//	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);
//
//	// 수정본 소스
	if (pInfo == NULL)
		return FALSE;

	CString strLog;
	int nHsNum = 0;
	switch(pInfo->m_ScInfo[0].m_nInternalJobType) 
	{
	case CScInfo::enStore:
		if (Store(pInfo->m_ScInfo[0].m_nInternalLuggNum, pInfo->m_ScInfo[0].m_strArriveInfo, pInfo->m_ScInfo[0].m_nFromHsNum, pInfo->m_nEquipNum) == TRUE)
		{
			pInfo->m_ScInfo[0].m_bInvoke = TRUE;
			pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
			pInfo->m_tOperStart = CTime::GetCurrentTime();
			pInfo->m_bModified = TRUE;
			pInfo->Backup();

			strLog.Format(_T("SC #%d  입고지시 [HS#%d -> %s]"), pInfo->m_nEquipNum, pInfo->m_ScInfo[0].m_nFromHsNum, pInfo->m_ScInfo[0].m_strArriveInfo);
			WriteLog(LOG_TYPE_JOB, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::Active"));
			return TRUE;
		}
		else
		{
			strLog.Format(_T("SC #%d  입고지시 실패! [HS#%d -> %s] [%s]"), pInfo->m_nEquipNum, pInfo->m_ScInfo[0].m_nFromHsNum, pInfo->m_ScInfo[0].m_strArriveInfo, m_strLog);
			WriteLog(LOG_TYPE_ERROR, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::Active"));
			return FALSE;
		}
		break;
	case CScInfo::enRetrieve:
		if (Retrieve(pInfo->m_ScInfo[0].m_nInternalLuggNum, pInfo->m_ScInfo[0].m_strDepartInfo, pInfo->m_ScInfo[0].m_nToHsNum, pInfo->m_nEquipNum) == TRUE)
		{
			pInfo->m_ScInfo[0].m_bInvoke = TRUE;
			pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
			pInfo->m_tOperStart = CTime::GetCurrentTime();
			pInfo->m_bModified = TRUE;
			pInfo->Backup();

			strLog.Format(_T("SC #%d  출고지시 [HS#%d -> %s]"), pInfo->m_nEquipNum, pInfo->m_ScInfo[0].m_nToHsNum, pInfo->m_ScInfo[0].m_strDepartInfo);
			WriteLog(LOG_TYPE_JOB, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::Active"));
			return TRUE;
		}
		else
		{
			strLog.Format(_T("SC #%d  출고지시 실패! [HS#%d -> %s] [%s]"), pInfo->m_nEquipNum, pInfo->m_ScInfo[0].m_nToHsNum, pInfo->m_ScInfo[0].m_strDepartInfo, m_strLog);
			WriteLog(LOG_TYPE_ERROR, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::Active"));
			return FALSE;
		}
		break;
	case CScInfo::enRack2Rack:
		if (RackToRack(pInfo->m_ScInfo[0].m_nInternalLuggNum, pInfo->m_ScInfo[0].m_strDepartInfo, pInfo->m_ScInfo[0].m_strArriveInfo, pInfo->m_nEquipNum))
		{
			pInfo->m_ScInfo[0].m_bInvoke = TRUE;
			pInfo->m_nPrevRoutine = CScInfo::enRoutineRackToRack;
			pInfo->m_tOperStart = CTime::GetCurrentTime();
			pInfo->m_bModified = TRUE;
			pInfo->Backup();

			strLog.Format(_T("SC #%d  랙투랙지시 [%s -> %s]"), pInfo->m_nEquipNum, pInfo->m_ScInfo[0].m_strDepartInfo, pInfo->m_ScInfo[0].m_strArriveInfo);
			WriteLog(LOG_TYPE_JOB, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::Active"));
			return TRUE;
		}
		else
		{
			strLog.Format(_T("SC #%d  랙투랙지시 실패! [%s -> %s] [%s]"), pInfo->m_nEquipNum, pInfo->m_ScInfo[0].m_strDepartInfo, pInfo->m_ScInfo[0].m_strArriveInfo, m_strLog);
			WriteLog(LOG_TYPE_ERROR, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::Active"));
			return FALSE;
		}
		break;
//	default:
	}
//------------------------------------------------------------------------------------------------------------------------------------------------------
	return FALSE;
}

BOOL CScDongJin::Stop(int n)				// 동진에서는 사용안함!
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 4; 
//	TxBuff[0] = '1';  
	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);

}

BOOL CScDongJin::Reset(int n)
{

	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	int nAddress;
	switch(n) 
	{
	case 1:
	case 4:		nAddress = 10;		break;
	case 2:
	case 5:		nAddress = 20;		break;
	case 3:
	case 6:		nAddress = 30;		break;
	}

	TxBuff[0] = 9;			// D10, D20, D30  
	TxBuff[2] = 8;			// D11, D21, D31
	
	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 2);
}

BOOL CScDongJin::CompleteReset(int n)
{

	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	int nAddress;
	switch(n) 
	{
	case 1:
	case 4:		nAddress = 10;		break;
	case 2:
	case 5:		nAddress = 20;		break;
	case 3:
	case 6:		nAddress = 30;		break;
	}

	TxBuff[0] = 9;			// D10, D20, D30  
	TxBuff[2] = 9;			// D11, D21, D31
	
	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 2);

}


BOOL CScDongJin::Delete(int n)				
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("SC #%d  통신연결 않됨"), n);
		return FALSE;
	}

	// 작업삭제가 정의되지 않음!
	return TRUE;
/* @@
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
	BYTE	TxBuff2[256];
	memset(TxBuff2, 0x00, sizeof(TxBuff2));
	int nAddress;
	switch(n) 
	{
	case 1:		nAddress = 10;			break;
	case 2:		nAddress = 20;			break;
	case 3:		nAddress = 30;			break;
	case 4:		nAddress = 10;			break;
	case 5:		nAddress = 20;			break;
	case 6:		nAddress = 30;			break;
	}

//  TxBuff[0] = 1;
//	TxBuff[0] = 16;//bFirst;							// 16 포크#1,  32 포크#2,   64 포크#1, #2 ALL DELETE
	TxBuff[0] = 0;
	TxBuff[2] = 0;
	TxBuff[4] = 0;
	TxBuff[6] = 0;
	TxBuff[8] = 0;
	TxBuff[10] = 0;
	TxBuff[12] = 0;
	TxBuff2[0] = 1;
	*/
//	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);
//	m_pSocket->Write(CMD_WORD_UNIT, TxBuff2, DEVICE_CODE_D, 100, 1);
//	return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 7);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CScDongJin::IsValidLocation(CString& strLocation)
{
	int n = CLib::GetStackerNum(1, strLocation);
	if (CLib::IsValidLocation(1, strLocation) == FALSE)
	{
		m_strLog.Format(_T("SC #%d  유효하지 않은 LOCATION! [LOC=%s]"), n, strLocation);
		return FALSE;
	}

	if (CLib::GetStackerNum(1, strLocation) != n)
	{
		m_strLog.Format(_T("SC #%d  유효하지 않은 LOCATION! [S/C 번호 불일치] [SC=%d, LOC=%s]"), n, n, strLocation);
		return FALSE;
	}

	if (CLib::IsDisableLocation(1, strLocation))
	{
		m_strLog.Format(_T("SC #%d  금지된 LOCATION! [LOC=%s]"), n, strLocation);
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CScDongJin::InvokeCheck(CScInfo* pInfo)
{
	DEBUGER_ASSERT_VALID(pInfo != NULL);
	if (pInfo == NULL)
		return;

/*	if ((pInfo->m_ScInfo[0].m_bInvoke != FALSE) || 
		(pInfo->IsWaitStatus() == FALSE) || 
		(pInfo->IsProductEmpty() == FALSE) ||
		(pInfo->IsOnlineMode() == FALSE) || 
		(pInfo->IsAutoMode() == FALSE) || 
		(pInfo->IsActiveMode() == FALSE))
		return;

*/
	if (pInfo->m_bStoSuspend == TRUE && pInfo->m_bRetSuspend == TRUE )
		return;

	if ((pInfo->m_ScInfo[0].m_bInvoke != FALSE) || 
		(pInfo->IsScJobStatusIdle() == FALSE) || 
		(pInfo->IsScJobStatusForkCenter() == FALSE) || 
		(pInfo->IsScJobStatusSensorProduct() == TRUE) ||
		(pInfo->IsScJobStatusScRemoteMode() == FALSE) || 
		(pInfo->IsScJobStatusRcRemoteMode() == FALSE))
		return;

//	if (pInfo->m_ucJobCount > 0)
//	{
//		CString strLog;
//		strLog.Format(_T("%s 지상반에 %d개의 작업이 존재합니다. 삭제후 진행해주세요!"), DEVICE, pInfo->m_ucJobCount);
//		m_pDoc->Alarm(LOG_POS_SC, strLog);
//		return;
//	}

	switch (pInfo->m_nPrevRoutine)
	{
	case CScInfo::enRoutineStore:
		{
			if (RetrieveRoutine(pInfo))
				break;

			if (RackToRackRoutine(pInfo))
				break;

			if (StoreRoutine(pInfo))
				break;
		}
		break;

	case CScInfo::enRoutineRetrieve:
		{
			if (RackToRackRoutine(pInfo))
				break;

			if (StoreRoutine(pInfo))
				break;

			if (RetrieveRoutine(pInfo))
				break;
		}
		break;

	case CScInfo::enRoutineRackToRack:
		{
			if (StoreRoutine(pInfo))
				break;

			if (RetrieveRoutine(pInfo))
				break;

			if (RackToRackRoutine(pInfo))
				break;
		}
		break;
	}
}

void CScDongJin::ErrorResetCheck(CScInfo* pInfo)
{
	DEBUGER_ASSERT_VALID(pInfo != NULL);
	if (pInfo == NULL)
		return;

	if( (pInfo->m_ScInfo[0].m_bInvoke == FALSE) || 
		(pInfo->IsScJobStatusIdle() == FALSE) || 
		(pInfo->m_wPrevErrCode == 0) ||
		(pInfo->m_nErrorCode != 0) ||
		(pInfo->IsScJobStatusForkCenter() == FALSE) || 
		(pInfo->IsScJobStatusScRemoteMode() == FALSE) ||
		(pInfo->IsScJobStatusJobComplete() == TRUE) ||
		(pInfo->IsScJobStatusIdle() == FALSE) ||
		(pInfo->m_bBufferEmpty == 0) ||
		(pInfo->IsScJobStatusRcRemoteMode() == FALSE))
		return;

	StartUpRequest(pInfo);
}

void CScDongJin::StartUpRequest(CScInfo* pInfo)
{
	if (pInfo == NULL)
		return;

	int nPattern, nSubCommand;
	CString strLog, strTempDepart, strTempArrive, strDepartLoc, strArriveLoc;

	if (pInfo->m_ScInfo[0].m_nInternalJobType == 0)
		return;

//	pInfo->m_ScInfo[0].m_nInternalJobType = 2;
	nPattern = CJobItem::ConvertJobTypeToPattern(pInfo->m_ScInfo[0].m_nInternalJobType);
	
//	nPattern = 2;
	int	nFromHsNum	= 0;//GetHsNum(pInfo->m_ScInfo[0].m_nInternalJobType, pInfo->m_nEquipNum, nFromBank, nFromBay, nFromLevel);
	int nToHsNum	= 0;//GetHsNum(pInfo->m_ScInfo[0].m_nInternalJobType, pInfo->m_nEquipNum, nToBank, nToBay, nToLevel);

//	int nFromBank, nFromBay, nFromLevel;
//	int nToBank, nToBay, nToLevel;

//	pInfo->m_ScInfo[0].m_strDepartInfo = "10241000";
//	pInfo->m_ScInfo[0].m_strArriveInfo = "10010202";

//	strTempDepart.Format("%s0%s%s",
//		pInfo->m_ScInfo[0].m_strDepartInfo.Mid(0,2),
//		pInfo->m_ScInfo[0].m_strDepartInfo.Mid(2,2),
//		pInfo->m_ScInfo[0].m_strDepartInfo.Mid(4,2));
//	strTempArrive.Format("%s0%s%s",
//		pInfo->m_ScInfo[0].m_strArriveInfo.Mid(0,2),
//		pInfo->m_ScInfo[0].m_strArriveInfo.Mid(2,2),
//		pInfo->m_ScInfo[0].m_strArriveInfo.Mid(4,2));

	// 작업정보가 없으면 할수없음???
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(pInfo->m_ScInfo[0].m_nInternalLuggNum);
	if (pJobItem == NULL)
	{
		AfxMessageBox(_T("작업정보가 없습니다. 작업정보 없이는 재지시를 할수가 없습니다."));
		return;
	}

	strTempDepart = strDepartLoc = pJobItem->m_strStartLoc;
	strTempArrive = strArriveLoc = pJobItem->m_strDestLoc;

//	strTempDepart.Format("%s0%s%s",
//		strDepartLoc.Mid(0,2),
//		strDepartLoc.Mid(3,2),
//		strDepartLoc.Mid(5,2));
//
//	strTempArrive.Format("%s0%s%s",
//		strArriveLoc.Mid(0,2),
//		strArriveLoc.Mid(3,2),
//		strArriveLoc.Mid(5,2));
	
	switch(nPattern) 
	{
	case enJobPatternSto:
//		nFromBank	= _ttoi(pInfo->m_ScInfo[0].m_strDepartInfo.Mid(0,2));
//		nFromBay	= _ttoi(pInfo->m_ScInfo[0].m_strDepartInfo.Mid(2,2));
//		nFromLevel	= _ttoi(pInfo->m_ScInfo[0].m_strDepartInfo.Mid(4,2));
//		nFromHsNum	= GetHsNum(nPattern, pInfo->m_nEquipNum, nFromBank, nFromBay, nFromLevel);

		if (_ttoi(pJobItem->m_strStartStn) > 200 )
			nFromHsNum = 1;
		else
			nFromHsNum = 3;

		strTempDepart = CConvert::ToString(nFromHsNum);

		nSubCommand = CScCmdMsg::enCmdManualSto;

		break;
	case enJobPatternRet:

//		nToBank		= _ttoi(pInfo->m_ScInfo[0].m_strArriveInfo.Mid(0,2));
//		nToBay		= _ttoi(pInfo->m_ScInfo[0].m_strArriveInfo.Mid(2,2));
//		nToLevel	= _ttoi(pInfo->m_ScInfo[0].m_strArriveInfo.Mid(4,2));
//		nToHsNum	= GetHsNum(nPattern, pInfo->m_nEquipNum, nToBank, nToBay, nToLevel);

		if (_ttoi(pJobItem->m_strDestStn) > 200 )
			nToHsNum = 2;
		else
			nToHsNum = 4;
		strTempArrive = CConvert::ToString(nToHsNum);

		nSubCommand = CScCmdMsg::enCmdManualRet;
		break;
	case enJobPatternR2R:
		nSubCommand = CScCmdMsg::enCmdManualR2R;
		break;
	case enJobPatternW2W:
		
		// 출발호기
		if (CLib::GetStackerNum(1, strDepartLoc) == pInfo->m_nEquipNum)
		{
//			nPattern = 2;

			strTempDepart.Format(_T("%s0%s%s"),
				strDepartLoc.Mid(0,2),
				strDepartLoc.Mid(3,2),
				strDepartLoc.Mid(5,2));
			
			strTempArrive = CConvert::ToString(2);		// 후면부로만 호기이동함!

			nSubCommand = CScCmdMsg::enCmdManualRet;

		}
		else if (CLib::GetStackerNum(1, strArriveLoc) == pInfo->m_nEquipNum)
		{
//			nPattern = 1;

			strTempDepart = CConvert::ToString(1);		// 후면부로만 호기이동함!

			strTempArrive.Format(_T("%s0%s%s"),
				strArriveLoc.Mid(0,2),
				strArriveLoc.Mid(3,2),
				strArriveLoc.Mid(5,2));

			nSubCommand = CScCmdMsg::enCmdManualSto;
		}
		else
		{
			strTempDepart = _T("0000000");
			strTempArrive = _T("0000000");
		}
			
		break;
	case enJobPatternMove:

//		nFromBank	= _ttoi(pInfo->m_ScInfo[0].m_strDepartInfo.Mid(0,2));
//		nFromBay	= _ttoi(pInfo->m_ScInfo[0].m_strDepartInfo.Mid(2,2));
//		nFromLevel	= _ttoi(pInfo->m_ScInfo[0].m_strDepartInfo.Mid(4,2));
//		nFromHsNum	= GetHsNum(1, pInfo->m_nEquipNum, nFromBank, nFromBay, nFromLevel);
		if (_ttoi(pJobItem->m_strStartStn) > 200 )
			nFromHsNum = 1;
		else
			nFromHsNum = 3;

		strTempDepart = CConvert::ToString(nFromHsNum);

//		nToBank		= _ttoi(pInfo->m_ScInfo[0].m_strArriveInfo.Mid(0,2));
//		nToBay		= _ttoi(pInfo->m_ScInfo[0].m_strArriveInfo.Mid(2,2));
//		nToLevel	= _ttoi(pInfo->m_ScInfo[0].m_strArriveInfo.Mid(4,2));
//		nToHsNum	= GetHsNum(2, pInfo->m_nEquipNum, nToBank, nToBay, nToLevel);
		if (_ttoi(pJobItem->m_strDestStn) > 200 )
			nToHsNum = 2;
		else
			nToHsNum = 4;
		strTempArrive = CConvert::ToString(nToHsNum);

		nSubCommand = CScCmdMsg::enCmdManualHs2Hs;
		break;
	default:
		return;
	}

	if (strTempDepart == _T("0000000") || strTempArrive == _T("0000000"))
		return;

	CScCmdMsg* pScCmdMsg = NULL;
	pScCmdMsg = new CScCmdMsg;
	DEBUGER_ASSERT_VALID(pScCmdMsg != NULL);
	pScCmdMsg->m_nCommand = nSubCommand;		// nPattern + CScCmdMsg::enCmdR2RSuspend;		//CScCmdMsg::enCmdManualRet;
	pScCmdMsg->m_nSubCmd = pInfo->m_nEquipNum;
	pScCmdMsg->m_strValues.SetSize(CScCmdMsg::enSizeScCommand);
	pScCmdMsg->m_strValues[0].Format(_T("%d"), pInfo->m_ScInfo[0].m_nInternalLuggNum);
	pScCmdMsg->m_strValues[1].Format(_T("%s"), strTempDepart);
	pScCmdMsg->m_strValues[2].Format(_T("%s"), strTempArrive);
	pScCmdMsg->m_strValues[3].Format(_T("%d"), TRUE);
	SetCmdMsg(pScCmdMsg);
}

void CScDongJin::CompleteCheck(CScInfo* pInfo)
{
	DEBUGER_ASSERT_VALID(pInfo != NULL);
	if (pInfo == NULL)
		return;

	if( //(pInfo->m_ScInfo[0].m_bInvoke == FALSE) || 
//		(pInfo->m_ScInfo[0].m_nLuggNum != pInfo->m_ScInfo[0].m_nInternalLuggNum) ||
		(pInfo->IsScJobStatusJobComplete() == FALSE) || 
//		(pInfo->IsScJobStatusIdle() == FALSE) || 
		(pInfo->IsScJobStatusForkCenter() == FALSE) || 
//	   ((pInfo->m_ucPrevStatus != CScInfo::enRunning) && (pInfo->m_ucPrevStatus != CScInfo::enError)) ||
		(pInfo->IsScJobStatusSensorProduct() == TRUE) )
		return;

	CString strLog;
	if (!CompleteReset(pInfo->m_nEquipNum))
	{
		strLog.Format(_T("SC %d 완료확인 실패!"), pInfo->m_nEquipNum);
		m_pDoc->Alarm(LOG_POS_SC, strLog);
//		WriteLog(LOG_TYPE_EVENT, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::CompleteCheck"));
		return;
	}

	strLog.Format(_T("SC %d 완료확인 보고"), pInfo->m_nEquipNum);
	WriteLog(LOG_TYPE_EVENT, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::CompleteCheck"));

	pInfo->m_ScInfo[0].m_bInvoke = FALSE;
	pInfo->m_bModified = TRUE;

	if (pInfo->m_ScInfo[0].m_nInternalJobType == enJobTypeManual)
	{
		pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
		pInfo->m_ucPrevStatus = CScInfo::enWaiting;
		pInfo->Backup();
		return;
	}

	pInfo->m_tOperEnd = CTime::GetCurrentTime();

	CJobItem* pJobItem = m_pDoc->m_pJob->Find(pInfo->m_ScInfo[0].m_nInternalLuggNum);
	if (pJobItem == NULL)
	{
		strLog.Format(_T("SC #%d 완료처리 실패! [%d번 작업정보 없음!]"), pInfo->m_nEquipNum, pInfo->m_ScInfo[0].m_nInternalLuggNum);
		WriteLog(LOG_TYPE_ERROR, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::CompleteCheck"), NULL, TRUE);
		return;
	}
	

	BYTE ucScCmdType = CScInfo::enCmdNone;
	switch (CJobItem::ConvertJobTypeToPattern(pInfo->m_ScInfo[0].m_nInternalJobType))
	{
	case enJobPatternSto:
	case enJobPatternR2R:
		if (pInfo->m_pStation->m_strID == pJobItem->m_strDestStn)
		{
			ucScCmdType = CScInfo::enStore;
		}
		break;

	case enJobPatternRet:
	case enJobPatternPR:
		if (pInfo->m_pStation->m_strID == pJobItem->m_strStartStn)
		{
			ucScCmdType = CScInfo::enRetrieve;
			pJobItem->m_nArriveTrackNum = pInfo->m_ScInfo[0].m_nArriveTrackNum;
//			pJobItem->m_nJobStatus = enJobStatusCvNew ;
		}
		break;
	case enJobPatternMove:
		break;
	case enJobPatternW2W:
		if (pInfo->m_pStation->m_strID == pJobItem->m_strStartStn)
		{
			ucScCmdType = CScInfo::enRetrieve;
			pJobItem->m_nArriveTrackNum = pInfo->m_ScInfo[0].m_nArriveTrackNum;
//			pJobItem->m_nJobStatus = enJobStatusCvNew ;
		}
		else
		{
			ucScCmdType = CScInfo::enStore;
		}
		break;
	default:
		strLog.Format(_T("SC #%d 완료처리 불가 작업구분 이상 [작업구분=%s]"), pInfo->m_nEquipNum, CJobItem::GetJobTypeString(pInfo->m_ScInfo[0].m_nInternalJobType));
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::CompleteCheck"), pJobItem);
		return;
	}

	strLog.Format(_T("SC #%d %s 작업 완료"), pInfo->m_nEquipNum, pJobItem->GetJobTypeString());
	WriteLog(LOG_TYPE_EVENT, strLog, _T("CScDongJin::CompleteCheck"), pJobItem);

	pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
	pInfo->m_ucPrevStatus = CScInfo::enWaiting;
	pInfo->Backup();

	pJobItem->SetJobStatus(enJobStatusScComplete);

	if (ucScCmdType != CScInfo::enCmdNone) 
		m_pDoc->m_pJob->Complete(pJobItem, ucScCmdType);
	else 
		pJobItem->m_nArriveTrackNum = pInfo->m_ScInfo[0].m_nArriveTrackNum;

	m_pDoc->m_pJob->Backup();

}

void CScDongJin::ErrorCheck(CScInfo* pInfo)
{
	if (pInfo == NULL)
		return;

	if (pInfo->IsScJobStatusError() == FALSE)
	{
		pInfo->m_nPrevErrorCode = 0;
		pInfo->Backup();
		return;
	}

	if (pInfo->m_nPrevErrorCode == pInfo->m_nErrorCode)
		return;

	pInfo->m_nPrevErrorCode = pInfo->m_nErrorCode;
	pInfo->Backup();

	if (pInfo->m_nErrorCode == 0)
		return;

	CString strLog;
	int nLogType = pInfo->IsErrorCode() ? LOG_TYPE_ERROR : LOG_TYPE_ALARM;
	strLog.Format(_T("SC #%d %s [%d=%s]"), pInfo->m_nEquipNum, pInfo->IsErrorCode() ? _T("ERROR") : _T("ALARM"), pInfo->m_nErrorCode, pInfo->GetErrorString());

	CJobItem* pJobItem = m_pDoc->m_pJob->Find(pInfo->m_ScInfo[0].m_nInternalLuggNum);
	if ((pInfo->m_ScInfo[0].m_nInternalLuggNum == 0) || (pJobItem == NULL))
	{
		WriteLog(nLogType, strLog, _T("CScDongJin::ErrorCheck"), NULL, TRUE);
		return;
	}

	switch (pInfo->m_nErrorCode)
	{
	case enScErrorEmptyRetrieve:
		{
			if (pJobItem->GetJobPattern() == enJobPatternSto)
			{
				strLog.Format(_T("SC #%d 입고중 공출고에러.. SC 체크"), pInfo->m_nEquipNum);
				WriteLog(nLogType, strLog, _T("CScDongJin::ErrorCheck"), pJobItem, TRUE);
				return;
			}

			strLog.Format(_T("SC #%d 공출고에러 [LOCATION=%s]"), pInfo->m_nEquipNum, pJobItem->GetStartStationString());
			WriteLog(nLogType, strLog, _T("CScDongJin::ErrorCheck"), pJobItem, TRUE);

			pJobItem->SetJobStatus(enJobStatusErrorEmptyRetrieve);
			m_pDoc->m_pJob->Backup();

			if (pJobItem->IsOfflineJobType())
			{
				m_pDoc->m_pJob->Remove(pJobItem);
				return;
			}

			// CScInfo::SetErrorCode에서 상위로 보고함!
//			if (m_pDoc->m_pHostCl)
//				m_pDoc->m_pHostCl->ErrorScReport(pJobItem, enErrorEmptyRetrieve);
		}
		break;

	case enScErrorDualStore:
		{
			if (pJobItem->GetJobPattern() == enJobPatternRet || 
				pJobItem->GetJobPattern() == enJobPatternPR)
			{
				strLog.Format(_T("SC #%d 출고 및 피킹출고 중 이중입고에러.. SC 체크"), pInfo->m_nEquipNum);
				WriteLog(nLogType, strLog, _T("CScDongJin::ErrorCheck"), pJobItem, TRUE);
				return;
			}

			strLog.Format(_T("SC #%d 이중입고 에러! [LOCATION=%s]"), pInfo->m_nEquipNum, pJobItem->GetDestStationString());
			WriteLog(nLogType, strLog, _T("CScDongJin::ErrorCheck"), pJobItem, TRUE);

			pJobItem->SetJobStatus(enJobStatusErrorDualStore);
			m_pDoc->m_pJob->Backup();

			if (pJobItem->IsOfflineJobType())
				return;

			// CScInfo::SetErrorCode에서 상위로 보고함!
//			if (m_pDoc->m_pHostCl)
//				m_pDoc->m_pHostCl->ErrorScReport(pJobItem, enErrorDualStore);
		}
		break;

	default:
		WriteLog(nLogType, strLog, _T("CScDongJin::ErrorCheck"), pJobItem, TRUE);
		break;
	}
}

BOOL CScDongJin::ErrorRoutine(CScInfo* pInfo)
{
	if (pInfo == NULL)
		return FALSE;

	if ((pInfo->m_ScInfo[0].m_bInvoke == FALSE) || 
		(pInfo->IsScJobStatusError() == FALSE) || 
		(pInfo->IsDualStoreError() == FALSE))
		return FALSE;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchDualStoreJobBySc(pInfo->m_nEquipNum);
	if (pJobItem == NULL)
	{
		strLog.Format(_T("SC #%d 이중입고 재지정 작업을 찾을 수 없습니다."), pInfo->m_nEquipNum);
		m_pDoc->StatusDisplay(strLog);
		return FALSE;
	}

	switch (pJobItem->GetJobPattern())
	{
	case enJobPatternSto:
	case enJobPatternW2W:
	case enJobPatternR2R:
		{
			int nStoHS = pInfo->GetDepartHS1();
//			DEBUGER_ASSERT_VALID(nStoHS != 0);

			Reset(pInfo->m_nEquipNum);

			if (Store(pJobItem->m_nLuggNum, pJobItem->m_strDestLoc,  nStoHS, pInfo->m_nEquipNum))
			{
				pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
				pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
				pInfo->m_ScInfo[0].m_nFromHsNum = nStoHS;
				pInfo->m_bModified = TRUE;
				pInfo->Backup();

				pJobItem->SetJobStatus(enJobStatusScInvoke);

				m_pDoc->m_pJob->Backup();

				strLog.Format(_T("SC #%d %s작업 이중입고 재지정"), pInfo->m_nEquipNum, pJobItem->GetJobTypeString());
				WriteLog(LOG_TYPE_JOB, strLog, _T("CScDongJin::ErrorRoutine"), pJobItem);
			}
			else
			{
				strLog.Format(_T("SC #%d %s작업 이중입고 재지정 실패! [%s]"), pInfo->m_nEquipNum, pJobItem->GetJobTypeString(), m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::ErrorRoutine"), pJobItem, TRUE);
				return FALSE;
			}
		}
		break;
	default:
		strLog.Format(_T("SC #%d 이중입고 재지정 실패! [유효하지 않은 작업구분] [%s]"), pInfo->m_nEquipNum, pJobItem->GetJobTypeString());
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::ErrorRoutine"), pJobItem, TRUE);
		return FALSE;
	}

	return TRUE;
}

BOOL CScDongJin::StoreRoutine(CScInfo* pInfo)
{
	DEBUGER_ASSERT_VALID(pInfo != NULL);
	DEBUGER_ASSERT_VALID(pInfo->m_pStation != NULL);

	if (pInfo == NULL)
		return FALSE;

	if (pInfo->m_pStation == NULL)
		return FALSE;
	
	if (pInfo->m_bStoSuspend == TRUE)
		return FALSE;

	CString strLog;
	CTrackHS* pStoHS = NULL;
	CJobItem* pJobItem = NULL;

	for (int i=0; i<pInfo->m_pStosHS.GetSize(); ++i)
	{
		pStoHS = pInfo->m_pStosHS.GetNextTrackHS();
		DEBUGER_ASSERT_VALID(pStoHS != NULL);
		DEBUGER_ASSERT_VALID(pStoHS->m_pTrack != NULL);

		if ((pStoHS->m_pTrack->m_bSuspend == TRUE) ||
			(pStoHS->m_pTrack->m_nLuggNum == 0) || 
			(pStoHS->m_pTrack->IsScStoHsReady() == FALSE))
			continue;

		if ((pJobItem = m_pDoc->m_pJob->Find(pStoHS->m_pTrack->m_nLuggNum)) == NULL)
		{
			strLog.Format(_T("SC #%d 작업정보가 존재하지 않습니다. [%s]"), pInfo->m_nEquipNum, pStoHS->ToString());
			m_pDoc->StatusDisplay(strLog, FALSE);
			continue;
		}

//		if (pJobItem->m_strDestStn != pInfo->m_pStation->m_strID)
//			return HsToHsRoutine(pJobItem, pStoHS, pInfo);

		if ((pJobItem->GetJobPattern() != enJobPatternSto) && (pJobItem->GetJobPattern() != enJobPatternW2W))
		{
			strLog.Format(_T("SC #%d 입고작업을 할 수 없습니다. [작업구분(%s) 이상] [%s]"), pInfo->m_nEquipNum, pJobItem->GetJobTypeString(), pStoHS->ToString());
			m_pDoc->StatusDisplay(strLog, FALSE);
			continue;
		}

		if (Store(pJobItem->m_nLuggNum, pJobItem->m_strDestLoc, pStoHS->m_nPos, pInfo->m_nEquipNum))
		{
			pInfo->m_ScInfo[0].m_bInvoke = TRUE;
			pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
			pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
			pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS->m_pTrack->m_nNumber;
			pInfo->m_ScInfo[0].m_nFromHsNum = pStoHS->m_nPos;
			pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
			pInfo->m_tOperStart = CTime::GetCurrentTime();
			pInfo->m_bModified = TRUE;
			pInfo->Backup();

			pJobItem->SetJobStatus(enJobStatusScInvoke);
			m_pDoc->m_pJob->Backup();

			strLog.Format(_T("SC #%d 입고지시 [HS#%d -> %s]"), pInfo->m_nEquipNum, pStoHS->m_nPos, pJobItem->m_strDestLoc);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CScDongJin::StoreRoutine"), pJobItem);
			return TRUE;
		}
		else
		{
			strLog.Format(_T("SC #%d 입고지시 실패! [HS#%d -> %s] [%s]"), pInfo->m_nEquipNum, pStoHS->m_nPos, pJobItem->m_strDestLoc, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::StoreRoutine"), pJobItem);
			continue;
		}
	}

	return FALSE;
}

BOOL CScDongJin::HsToHsRoutine(CJobItem* pJobItem, CTrackHS* pStoHS, CScInfo* pInfo)
{
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	if (pInfo == NULL)
		return FALSE;

	if ((pJobItem == NULL) || (pStoHS == NULL))
		return FALSE;

	CString strLog;
	BOOL bResult = FALSE;
	CTrackHS* pRetHS = NULL;
	CJobItem* pJobItemPrev = NULL;

	int j = 0;
		for (j=0; j<pInfo->m_pRetsHS.GetSize(); ++j)
		{
			pRetHS = pInfo->m_pRetsHS.GetNextTrackHS();
			DEBUGER_ASSERT_VALID(pRetHS != NULL);
			DEBUGER_ASSERT_VALID(pRetHS->m_pTrack != NULL);

			if ((pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE) &&
				(pRetHS->m_pTrack->m_bSuspend == FALSE) && 
				(pRetHS->m_pTrack->m_nLuggNum == 0) && 
				(pRetHS->m_pTrack->IsScRetHsReady() == TRUE))
				break;
		}

		int nTemp = pInfo->m_pRetsHS.GetSize();

		if ((j == pInfo->m_pRetsHS.GetSize()) || (pRetHS == NULL))
			return FALSE;
	
	if ((pRetHS->m_pTrack->m_bSuspend == TRUE) || 
		(pRetHS->m_pTrack->m_nLuggNum > 0) || 
//		(pRetHS->m_pTrack->m_bDeadLock == TRUE) || 
		(pRetHS->m_pTrack->IsScRetHsReady() == FALSE))
		return FALSE;

	if (pJobItemPrev = m_pDoc->m_pJob->FetchScCompleteJob(pRetHS->m_pTrack->m_nNumber))
	{
		strLog.Format(_T("SC #%d 완료되지 않은 이전작업이 존재합니다. 확인해주세요. [작업번호=%d] [출고HS=TR%d] [작업상태=%s]"), 
			pInfo->m_nEquipNum, pJobItemPrev->m_nLuggNum, pJobItemPrev->m_nArriveTrackNum, pJobItemPrev->GetJobStatusString());
		m_pDoc->Alarm(LOG_POS_SC, strLog, pJobItemPrev);
		return FALSE;
	}

	if (bResult = HsToHs(pJobItem->m_nLuggNum, pStoHS->m_nPos, pRetHS->m_nPos, pInfo->m_nEquipNum))
	{
		pInfo->m_ScInfo[0].m_bInvoke = TRUE;
		pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
		pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
		pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS->m_pTrack->m_nNumber;
		pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;
		pInfo->m_ScInfo[0].m_nFromHsNum = pStoHS->m_nPos;
		pInfo->m_ScInfo[0].m_nToHsNum = pRetHS->m_nPos;
		pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
		pInfo->m_tOperStart = CTime::GetCurrentTime();
		pInfo->m_bModified = TRUE;
		pInfo->Backup();

		pJobItem->SetJobStatus(enJobStatusScInvoke);
		m_pDoc->m_pJob->Backup();

		strLog.Format(_T("SC #%d 작업대이동 지시 [HS#%d ->HS#%d]"), pInfo->m_nEquipNum, pStoHS->m_nPos, pRetHS->m_nPos);
		WriteLog(LOG_TYPE_JOB, strLog, _T("CScDongJin::HsToHsRoutine"), pJobItem);
	}
	else
	{
		strLog.Format(_T("SC #%d 작업대이동 지시 실패! [HS#%d -> HS#%d] [%s]"), pInfo->m_nEquipNum, pStoHS->m_nPos, pRetHS->m_nPos, m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::HsToHsRoutine"), pJobItem);
	}

	return bResult;
}


BOOL CScDongJin::RetrieveRoutine(CScInfo* pInfo)
{
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	if (pInfo == NULL)
		return FALSE;

	if (pInfo->m_bRetSuspend == TRUE)
		return FALSE;

	CString strLog;
	CTrackHS* pRetHS = NULL;
	CJobItem* pJobItem = NULL;
	CJobItem* pJobItemPrev = NULL;

	for (int i=0; i<pInfo->m_pRetsHS.GetSize(); ++i)
	{
		pRetHS = pInfo->m_pRetsHS.GetNextTrackHS();
		DEBUGER_ASSERT_VALID(pRetHS != NULL);
		DEBUGER_ASSERT_VALID(pRetHS->m_pTrack != NULL);

		// 원본 
//		if (//(pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE) &&
//			(pRetHS->m_pTrack->m_bSuspend == TRUE) || 
//			(pRetHS->m_pTrack->m_nLuggNum != 0) || 
//			(pRetHS->m_pTrack->IsScRetHsReady() == FALSE))
//			continue;
//
//		// 각 호기 별로 출고 HS를 보지 않도록 설정 요청!
//		// 수정본
//		if ((m_pDoc->m_pConfig->m_bNoCheckScRetHs == TRUE) &&
//			(pRetHS->m_pTrack->m_nNumber == 3916 || 
//			 pRetHS->m_pTrack->m_nNumber == 3926 || 
//			 pRetHS->m_pTrack->m_nNumber == 3936 || 
//			 pRetHS->m_pTrack->m_nNumber == 3946 || 
//			 pRetHS->m_pTrack->m_nNumber == 3956 || 
//			 pRetHS->m_pTrack->m_nNumber == 3966 ) )
//		{
//			if (pRetHS->m_pTrack->m_bSuspend == TRUE) 
//				continue;
//			
//			int nRetBuffer[6][3] = {	
//				{3914, 3915, 3916},
//				{3924, 3925, 3926},
//				{3934, 3935, 3936},
//				{3944, 3945, 3946},
//				{3954, 3955, 3956},
//				{3964, 3965, 3966} };
//			
//			int nBufferCnt = 0;
//			CTrackInfo* pBufferTrack = NULL;
//			int nRetBufferCnt = 3;	//	m_pDoc->m_pConfig->m_nRetBufferCount;
//			for(int k=0;k<nRetBufferCnt;k++)
//			{
//				pBufferTrack = m_pDoc->GetTrackInfo(nRetBuffer[pInfo->m_nEquipNum-1][k]);
//				
//				if (pBufferTrack == NULL)
//					continue;
//				
//				if (pBufferTrack->IsProductSensing() == TRUE)
//					nBufferCnt++;
//			}
//			
//			if (nBufferCnt >= nRetBufferCnt - 1)
//				continue;
//		}
//		// 진짜 수정본 - 2017.06.23

		BOOL bNotCheckRetFrontHs = FALSE;

		if (m_pDoc->m_pConfig->m_bNoCheckScRetHs == TRUE)
		{
			if (pRetHS->m_pTrack->m_bSuspend == TRUE) 
				continue;
			
			BOOL bNotCheckHs = FALSE;


			// 트랙번호별로 체크가 되어있는지 확인한다. 
			switch(pRetHS->m_pTrack->m_nNumber)
			{
			case 3916:	bNotCheckHs = m_pDoc->m_pConfig->m_bNotCheckRetHsSc1;	break;
			case 3926:	bNotCheckHs = m_pDoc->m_pConfig->m_bNotCheckRetHsSc2;	break;
			case 3936:	bNotCheckHs = m_pDoc->m_pConfig->m_bNotCheckRetHsSc3;	break;
			case 3946:	bNotCheckHs = m_pDoc->m_pConfig->m_bNotCheckRetHsSc4;	break;
			case 3956:	bNotCheckHs = m_pDoc->m_pConfig->m_bNotCheckRetHsSc5;	break;
			case 3966:	bNotCheckHs = m_pDoc->m_pConfig->m_bNotCheckRetHsSc6;	break;
			default :
				{
					bNotCheckHs = FALSE;
					bNotCheckRetFrontHs = TRUE;							
					break;;
				}
			}
			

			if (bNotCheckHs == TRUE)
			{
				// 체크가 되어있으면 출고시에 HS 신호를 체크하지 않음!
				int nRetBufferCnt = 3;//= m_pDoc->m_pConfig->m_nRetBufferCount;
			
				int nBufferCnt = 0;
				CTrackInfo* pBufferTrack = NULL;

				int nFrontTrackNum = pRetHS->m_pTrack->m_nNumber;

				for(int k=0;k<nRetBufferCnt;k++)
				{
					pBufferTrack = m_pDoc->GetTrackInfo(nFrontTrackNum);
					nFrontTrackNum--;
					
					if (pBufferTrack == NULL)
						continue;
					
					if (pBufferTrack->IsProductSensing() == TRUE || pBufferTrack->m_nLuggNum != 0)
						nBufferCnt++;
				}
				
				if (nBufferCnt >= nRetBufferCnt - 1)
					continue;
			}
			else 
			{
				// 체크가 되어있지 않으면 출고시에 출고 HS신호를 체크함!
				if (//(pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE) &&
					(pRetHS->m_pTrack->m_bSuspend == TRUE) || 
					(pRetHS->m_pTrack->m_nLuggNum != 0) || 
					(pRetHS->m_pTrack->IsScRetHsReady() == FALSE))
					continue;
			}
		}
		else
		{
			if (//(pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE) &&
				(pRetHS->m_pTrack->m_bSuspend == TRUE) || 
				(pRetHS->m_pTrack->m_nLuggNum != 0) || 
				(pRetHS->m_pTrack->IsScRetHsReady() == FALSE))
				continue;
		}

		if (pJobItemPrev = m_pDoc->m_pJob->FetchScCompleteJob(pRetHS->m_pTrack->m_nNumber))
		{
			CTimeSpan tElapseTime = CTime::GetCurrentTime() - pJobItemPrev->m_tTime;
			if (tElapseTime.GetTotalSeconds() < m_pDoc->m_pConfig->m_nCompleteCnt)		// 10
				continue;

			strLog.Format(_T("SC #%d 완료되지 않은 이전작업이 존재합니다. 확인해주세요. [작업번호=%d] [출고HS=TR%d] [작업상태=%s]"), 
				pInfo->m_nEquipNum, pJobItemPrev->m_nLuggNum, pJobItemPrev->m_nArriveTrackNum, pJobItemPrev->GetJobStatusString());
			m_pDoc->Alarm(LOG_POS_SC, strLog, pJobItemPrev);
			continue;
		}
//===================================================================================================================================================================================
//		// 출고 로직 변경 (이전 작업대를 기억하고 있다가 출고하는 방법)
//===================================================================================================================================================================================
//		// 원본소스
//		for (int j=0; j<pRetHS->m_strDestStations.GetSize(); ++j)
//		{
//			if (pJobItem = m_pDoc->m_pJob->FetchRetrieveJobBySc(pInfo->m_nEquipNum, pRetHS->m_strDestStations[j]))
//			{
//				// 현재 작업에 목적지가 2F BIDP 출고대이면 현재 진행중인 작업개수를 확인
//				if (m_pDoc->m_pJob->IsValidScRetrieveTask(pJobItem->m_strDestStn) == FALSE)
//				{
//					pJobItem = NULL;
//					continue;
//				}
//
//				break;
//			}
//		}
//
//		if (pJobItem == NULL)
//			continue;
		

		// 수정본 소스 
		int nIndex = 0;
		int nStationCnt = pRetHS->m_strDestStations.GetSize();
		for (int j=0; j<nStationCnt; ++j)
		{
			nIndex = (pRetHS->m_nDestStationPos >= nStationCnt) ? 0 : pRetHS->m_nDestStationPos;
			if (pJobItem = m_pDoc->m_pJob->FetchRetrieveJobBySc(pInfo->m_nEquipNum, pRetHS->m_strDestStations[nIndex]))
			{
				pRetHS->m_nDestStationPos = ++nIndex;
				break;
			}
			pRetHS->m_nDestStationPos = ++nIndex;
		}

		if (pJobItem == NULL)
			continue;
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		if (Retrieve(pJobItem->m_nLuggNum, pJobItem->m_strStartLoc, pRetHS->m_nPos, pInfo->m_nEquipNum))
		{
			pInfo->m_ScInfo[0].m_bInvoke = TRUE;
			pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
			pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
			pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;
			pInfo->m_ScInfo[0].m_nToHsNum = pRetHS->m_nPos;
			pInfo->m_nPrevRoutine = CScInfo::enRoutineRetrieve;
			pInfo->m_tOperStart = CTime::GetCurrentTime();
			pInfo->m_bModified = TRUE;
			pInfo->Backup();

			pJobItem->SetJobStatus(enJobStatusScInvoke);
			m_pDoc->m_pJob->Backup();

			strLog.Format(_T("SC #%d 출고지시 [%s -> HS#%d]"), pInfo->m_nEquipNum, pJobItem->m_strStartLoc, pRetHS->m_nPos);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CScDongJin::RetrieveRoutine"), pJobItem);
			
			return TRUE;
		}
		else
		{
			strLog.Format(_T("SC #%d 출고지시 실패! [%s -> HS#%d] [%s]"), pInfo->m_nEquipNum, pJobItem->m_strStartLoc, pRetHS->m_nPos, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::RetrieveRoutine"), pJobItem);
			continue;
		}
	}

	return FALSE;
}

BOOL CScDongJin::RackToRackRoutine(CScInfo* pInfo)
{
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	if (pInfo == NULL)
		return FALSE;

	if (pInfo->m_bR2RSuspend == TRUE)
		return FALSE;

//	if (pInfo->m_bStoSuspend == TRUE)
//		return FALSE;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchRackToRackJobBySc(pInfo->m_nEquipNum);
	if (pJobItem == NULL)
		return FALSE;

	if (RackToRack(pJobItem->m_nLuggNum, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc, pInfo->m_nEquipNum))
	{
		pInfo->m_ScInfo[0].m_bInvoke = TRUE;
		pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
		pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
		pInfo->m_nPrevRoutine = CScInfo::enRoutineRackToRack;
		pInfo->m_tOperStart = CTime::GetCurrentTime();
		pInfo->m_bModified = TRUE;
		pInfo->Backup();

		pJobItem->SetJobStatus(enJobStatusScInvoke);
		m_pDoc->m_pJob->Backup();

		strLog.Format(_T("SC #%d 랙투랙지시 [%s -> %s]"), pInfo->m_nEquipNum, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc);
		WriteLog(LOG_TYPE_JOB, strLog, _T("CScDongJin::RackToRackRoutine"), pJobItem);
		return TRUE;
	}
	else
	{
		strLog.Format(_T("SC #%d 랙투랙지시 실패! [%s -> %s] [%s]"), pInfo->m_nEquipNum, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc, m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CScDongJin::RackToRackRoutine"), pJobItem);
	}

	return FALSE;
}

void CScDongJin::SafeCheck(CScInfo* pInfo)
{
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	if (pInfo == NULL)
		return;

//	if ((pInfo->IsAutoMode() == FALSE) || (pInfo->IsErrorStatus() == TRUE))
//		return;

	CString strLog;
	CTrackHS* pTrackHS = NULL;

	for (int i=0; i<pInfo->m_pStosHS.GetSize(); ++i)
	{
		pTrackHS = pInfo->m_pStosHS[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

		if (pTrackHS->m_pTrack->IsSensorScInterlock() == TRUE)
		{
			if (Stop(pInfo->m_nEquipNum))
			{
				strLog.Format(_T("SC #%d EMERGENCY [%s SC INTERLOCK ON]"), pInfo->m_nEquipNum, pTrackHS->ToString());
				WriteLog(LOG_TYPE_EVENT, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::SafeCheck"), NULL, TRUE);
				return;
			}
			else
			{
				strLog.Format(_T("SC #%d EMERGENCY 실패! [%s SC INTERLOCK ON]"), pInfo->m_nEquipNum, pTrackHS->ToString());
				WriteLog(LOG_TYPE_ALARM, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::SafeCheck"), NULL, TRUE);
				continue;
			}
		}
	}

	for (int j=0; j<pInfo->m_pRetsHS.GetSize(); ++j)
	{
		pTrackHS = pInfo->m_pRetsHS[j];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

		if (pTrackHS->m_pTrack->IsSensorScInterlock() == TRUE)
		{
			if (Stop(pInfo->m_nEquipNum))
			{
				strLog.Format(_T("SC #%d EMERGENCY [%s SC INTERLOCK ON]"), pInfo->m_nEquipNum, pTrackHS->ToString());
				WriteLog(LOG_TYPE_EVENT, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::SafeCheck"), NULL, TRUE);
				return;
			}
			else
			{
				strLog.Format(_T("SC #%d EMERGENCY 실패! [%s SC INTERLOCK ON]"), pInfo->m_nEquipNum, pTrackHS->ToString());
				WriteLog(LOG_TYPE_ALARM, pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScDongJin::SafeCheck"), NULL, TRUE);
				continue;
			}
		}
	}
}

int CScDongJin::GetInfoIndex(int nNumber)
{
	for (int n = 0; n <= m_pInfo.GetUpperBound(); n++)
	{
		int nNo = m_pInfo[n]->m_pEquipment->m_nNumber;
		if (nNumber == nNo)
			return n;
	}

	return -1;
}

