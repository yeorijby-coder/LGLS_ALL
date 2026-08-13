// Lgv.cpp: implementation of the CLgv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "Lgv.h"
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

IMPLEMENT_DYNAMIC(CLgv, CEquipment)

CLgv::CLgv(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice) : CEquipment(pDoc, nIndex, nNumber, lpszDevice)
{
	m_enKind = CEquipment::enRGV;
	m_pInfo = new CLgvInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
//	m_nTemp1 = 0;
//	m_nTemp2 = 0;
	InitializeXmlDom();
}

CLgv::~CLgv()
{
	delete m_pInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLgv::MulticastInfo(CMonitorServer* pMonitorSv)
{
//	if (pMonitorSv == NULL)
//		return;

//	m_pInfo->MulticastDoor(pMonitorSv);
	m_pInfo->MulticastVehicleAll(pMonitorSv);
//	m_pInfo->MulticastAddAllJob(pMonitorSv);
//	Sleep(100);
	m_pInfo->MulticastCommStatus(pMonitorSv);
/*
//==========================================================================================================	
//	// PlayBack을 위해서 
//==========================================================================================================
//	m_pEquipment->m_pDoc->m_pLogPlayback->Log(strTxBuff);	
//----------------------------------------------------------------------------------------------------------
*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLgv::ReadProcess()
{
	CString strValue, strName, strLog;
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

	CByteArray arrBuffer;
	arrBuffer.SetSize(4);

	//////////////////////////////////////////////////////////////////////////////////////
	if (m_pThreadSocket->RecvData(arrBuffer) == FALSE)
	{
		strLog = m_pThreadSocket->m_strLog;
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////////////////
	int nCarNum = 0;
	int nLuggNo = 0;
	int nPosition = 0;

	CString strRecv(arrBuffer.GetData());
	int nRecvLen = strRecv.GetLength();

	m_pThreadSocket->m_strRecv = strRecv;
	m_pInfo->m_pEquipment->RefreshDialog();

	m_pThreadSocket->TokenRxFrame(strRecv, CAsyncSocketEx::enSTX, CAsyncSocketEx::enETX);
	for (POSITION pos=m_pThreadSocket->m_strRxList.GetHeadPosition(); pos!=NULL; )
	{
		CString& strRxFrame = m_pThreadSocket->m_strRxList.GetNext(pos);
		int nLen = strRxFrame.GetLength();

		if (strRxFrame[0] != CAsyncSocketEx::enSTX)
		{
			strLog.Format(_T("수신데이터 이상! [NO STX]"));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_RGV, m_strLog, _T("CLgv::ReadProcess"));
			continue;
		}

		if (strRxFrame[nLen-1] != CAsyncSocketEx::enETX)
		{
			strLog.Format(_T("수신데이터 이상! [NO ETX]"));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_RGV, m_strLog, _T("CLgv::ReadProcess"));
			continue;
		}

		if (m_pXml->LoadXmlString(strRxFrame.Mid(1, nLen-2)) == FALSE)
		{
			strLog.Format(_T("LoadXmlString 실패! [%s]"), strRxFrame.Mid(1, nLen-2)/*, m_pXml->GetErrorMessage()*/);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_RGV, strLog, _T("CLgv::ReadProcess"));
			continue;
		}

		if (ParseMessageStop() == FALSE && ParseMessageTransfer() == FALSE)
		{
			if (m_pXml->MoveXPath(_T("./Message")))	
			{
				m_pXml->GetAttrValue(_T("Name"), strName);
				int nActivity = 0;
				int nProdLoad = 0;
				int m_nTempDepartTrackNum = 0;
				int m_nTempArriveTrackNum = 0;
				CTrackHS* pTrackHS = NULL;

				if (strName == _T("MoveCancel") || strName == _T("MoveStart") || strName == _T("LoadingCmp") || strName == _T("UnloadingCmp"))
				{
					m_pXml->MoveChild(0);

					BOOL bVisible = TRUE;

					m_pXml->GetAttrValue(_T("No"), strValue);			nCarNum		= _ttoi(strValue);
					m_pXml->GetAttrValue(_T("LuggNo"), strValue);		nLuggNo		= _ttoi(strValue);
					m_pXml->GetAttrValue(_T("Position"), strValue);		nPosition	= _ttoi(strValue);

					CLgvVehicle* pVehicle = (CLgvVehicle*)m_pInfo->GetLgvVehicle(nCarNum);

					if (pVehicle == NULL)
						continue;

					CJobItem* pJobItem = NULL;
					if (strName == _T("UnloadingCmp"))
					{
						// 완료 응답을 올려준다.
						CString strSendXmlMessage;
						strSendXmlMessage.Format(_T("%c<Message Name='UnloadingCmp_Reply'><Car No='%02d' Position='%02d' LuggNo='%04d' ReturnCode='%02d'/></Message>%c"),
							CInterfaceThreadSk::enSTX,	nCarNum, nPosition, nLuggNo, 0, CInterfaceThreadSk::enETX);

						m_pThreadSocket->SendData(strSendXmlMessage, FALSE);
						

						strLog.Format(_T("%s %s Message 수신 응답함![%s]"), DEVICE, strName, strSendXmlMessage);
						m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_RGV, nLuggNo, strLog, _T("CLgv::ReadProcess"));

						pJobItem = m_pDoc->m_pJob->Find(nLuggNo);
						if (pJobItem == NULL)
						{
							m_strLog.Format(_T("%s CAR %d 작업을 작업정보에서 찾을 수 없습니다. [LUGG=%d]"), DEVICE, nCarNum, nLuggNo);
							m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_RGV, nLuggNo, strLog, _T("CLgv::ReadProcess"));

							continue;
						}

						pVehicle->m_nLuggNum = nLuggNo;
						pVehicle->m_nCurPosition = nPosition;
						pVehicle->m_bInvoke = FALSE;


						// 작업정보를 완료해준다.
						if (pJobItem != NULL)
						{
							int nDepartTrack = 0;
							int nArriveTrack = 0;
								
							nActivity = CJobItem::enActivityDropComplete;

							CLgvVehicle* pLgvVehicle = NULL;
							if (pJobItem->m_nVehicleID != 0)
								pLgvVehicle = (CLgvVehicle*) m_pInfo->GetLgvVehicle(pJobItem->m_nVehicleID);
							
							if (pLgvVehicle != NULL && pLgvVehicle->m_nLuggNum != 0) 
							{
								pLgvVehicle->m_ucLoadStatus = CLgvVehicle::enUnload - 0x30;
								pLgvVehicle->m_nLuggNum = 0;
								pJobItem->m_nVehicleID = 0;
							}
							pTrackHS = (CTrackHS*)m_pInfo->m_pArrivesHS.FindByPos(nPosition);
							pJobItem->m_nTempArriveTrackNum = (pTrackHS != NULL && pTrackHS->m_pTrack != NULL) ? pTrackHS->m_pTrack->m_nNumber : 0;

							pJobItem->m_nDepartTrackNum = pJobItem->m_nTempDepartTrackNum;		//pLgvJobItem->m_nDepartTrackNum;
							pJobItem->m_nArriveTrackNum = pJobItem->m_nTempArriveTrackNum;		//pLgvJobItem->m_nArriveTrackNum;
							pJobItem->SetJobStatus(enJobStatusRtvComplete);
							pJobItem->SetRgvcJobStatus(nActivity, 0, nCarNum);
							m_pDoc->m_pJob->Backup();
						}

						// RGV 작업정보도 완료해준다.
						m_pInfo->m_bModified = TRUE;
						m_pInfo->InvokeControl(nCarNum);
						m_pInfo->InvokeControl();
						m_pInfo->Backup();
						
						m_strLog.Format(_T("%s CAR %d 작업완료 [LUGG=%d]"), DEVICE, nCarNum, nLuggNo);
						m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_RGV, nLuggNo, strLog, _T("CLgv::ReadProcess"));

						m_pXml->MoveParent();
						continue;
					}
					// 차에 할당될 때 해당 차에서 작업번호가 있을 경우 그 작업번호를 RGV 작업정보에서 찾아서 있을경우 지운다.
					else if(strName == "MoveStart")
					{
		
						pJobItem = m_pDoc->m_pJob->Find(nLuggNo);
						if (pJobItem == NULL)
						{
							m_strLog.Format(_T("%s CAR %d 작업을 작업정보에서 찾을 수 없습니다. [LUGG=%d]"), DEVICE, nCarNum, nLuggNo);
							m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_RGV, nLuggNo, strLog, _T("CLgv::ReadProcess"));

							continue;
						}

						pTrackHS = (CTrackHS*)m_pInfo->m_pDepartsHS.FindByPos(nPosition);
						pJobItem->m_nTempDepartTrackNum = (pTrackHS != NULL && pTrackHS->m_pTrack != NULL) ? pTrackHS->m_pTrack->m_nNumber : 0;

						nActivity = CJobItem::enActivityVehicleAssigne;
						nProdLoad = CLgvVehicle::enUnload;
						m_pInfo->m_bInvoke = TRUE;
						m_pInfo->MulticastCommStatus();
					}
					else if(strName == "LoadingCmp")
					{
						pJobItem = m_pDoc->m_pJob->Find(nLuggNo);
						if (pJobItem == NULL)
						{
							m_strLog.Format(_T("%s CAR %d 작업을 작업정보에서 찾을 수 없습니다. [LUGG=%d]"), DEVICE, nCarNum, nLuggNo);
							m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_RGV, nLuggNo, strLog, _T("CLgv::ReadProcess"));

							continue;
						}

						pTrackHS = (CTrackHS*)m_pInfo->m_pDepartsHS.FindByPos(pVehicle->m_nCurPosition);
						pJobItem->m_nTempDepartTrackNum = (pTrackHS != NULL && pTrackHS->m_pTrack != NULL) ? pTrackHS->m_pTrack->m_nNumber : 0;

						nActivity = CJobItem::enActivityPickComplete;
						nProdLoad = CLgvVehicle::enLoad;

					}
					else if(strName == "MoveCancel")
					{
						pJobItem = m_pDoc->m_pJob->Find(nLuggNo);
						if (pJobItem == NULL)
						{
							m_strLog.Format(_T("%s CAR %d 작업을 작업정보에서 찾을 수 없습니다. [LUGG=%d]"), DEVICE, nCarNum, nLuggNo);
							m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_RGV, nLuggNo, strLog, _T("CLgv::ReadProcess"));

							continue;
						}

						nActivity = CJobItem::enActivityQueue;

						pJobItem->m_nActivity = nActivity;
						pJobItem->m_nVehicleID = 0;
						m_pDoc->m_pJob->Backup();

						pVehicle->m_nLuggNum = 0;
						pVehicle->m_nCurPosition = nPosition;
						nProdLoad = CLgvVehicle::enUnload;

						m_pInfo->m_bModified = TRUE;

						m_pInfo->InvokeControl(nCarNum);
						m_pInfo->Backup();

						m_pXml->MoveParent();
						continue;
					}

					pJobItem->SetRgvcJobStatus(nActivity, m_pInfo->m_pEquipment->m_nNumber, nCarNum);
					m_pDoc->m_pJob->Backup();

					pVehicle->m_ucLoadStatus = nProdLoad - 0x30;
					pVehicle->m_nLuggNum = nLuggNo;
					pVehicle->m_nCurPosition = nPosition;

					pVehicle->m_pControl->m_bVisible = bVisible;
					m_pInfo->m_bModified = TRUE;

					m_pInfo->InvokeControl(nCarNum);
					m_pInfo->Backup();

					m_pXml->MoveParent();
					
				}
				else if (strName == "State")
				{
					int nMode, nRun, nContain, nECode;
					int nCount = m_pXml->GetChildElmtCount();
					for (int i = 0 ; i < nCount ; i++)
					{
						m_pXml->MoveChild(i);

						m_pXml->GetAttrValue(_T("No"), strValue);			nCarNum		= _ttoi(strValue);
						m_pXml->GetAttrValue(_T("Mode"), strValue);			nMode		= _ttoi(strValue);
						m_pXml->GetAttrValue(_T("Run"), strValue);			nRun		= _ttoi(strValue);
						m_pXml->GetAttrValue(_T("Contain"), strValue);		nContain	= _ttoi(strValue);
						m_pXml->GetAttrValue(_T("Position"), strValue);		nPosition	= _ttoi(strValue);
						m_pXml->GetAttrValue(_T("ECode"), strValue);		nECode		= _ttoi(strValue);
						m_pXml->GetAttrValue(_T("LuggNo"), strValue);		nLuggNo		= _ttoi(strValue);
					
						CLgvVehicle* pVehicle = (CLgvVehicle*)m_pInfo->GetLgvVehicle(nCarNum);

						if (pVehicle == NULL)
							continue;

						// - 화면에 표시하는 용도임!
						pVehicle->SetMode(nMode);														// RGV 동작모드		- {0:Manual, 1:Local, 2:Auto}					
						pVehicle->SetStatus(nRun);														// RGV 동작상태		- {0:대기, 1:이동, 2:로딩, 3:언로딩, 4:에러}	
						pVehicle->SetProdLoad(nContain);												// RGV 화물감지상태 - {0:화물무, 1:화물유}						 	
						pVehicle->SetHorizontalPos(nPosition);											// RGV 위치
						pVehicle->SetErrorCode(nECode, m_pInfo->m_pCvInfo, m_pInfo->m_pEquipment->m_nNumber);		// RGV 에러코드
						pVehicle->SetLuggNum(nLuggNo);													// RGV 작업번호

						m_pInfo->m_bModified = pVehicle->m_bModified;

						m_pInfo->InvokeControl(pVehicle->m_nVehicleID);
						m_pInfo->Backup();

						m_pXml->MoveParent();
					}
				}
				else
				{
					strLog.Format(_T("ParsingXmlString 실패! [Invalid Message Name][%s]"), strRxFrame);
					m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_RGV, strLog, _T("CLgv::ReadProcess"));
					continue;
				}
			}
			else if (m_pXml->MoveXPath(_T("./HeartBit")))	
			{
				UpdateCommStatus(NOTIFY_REFRESH);
				continue;				
			}
			else
			{
				strLog.Format(_T("ParsingXmlString 실패! [Message를 Parsing하지 못함!]"));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_LGV, strLog, _T("CLgv::ReadProcess"));
				continue;
			}
		}
	}
	
	return TRUE;
}

void CLgv::AutoRunProc()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if (m_pInfo->m_bSuspend)
		return;

	ReadProcess();
	
	if (m_pInfo->m_wErrorCode == 0xFFFF)
	{
		m_pInfo->m_wErrorCode = 0;
		m_pInfo->InvokeControl();
	}
	
	InvokeCheck();

	InvokeDoorCloseRequest(0);
	InvokeDoorOpenRequest(0);

	Sleep(100);
}


void CLgv::InvokeDoorOpenRequest(int nDoorNum /* = 0*/)
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pInfo->m_pCvInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pInfo->m_pCvInfo->m_pEquipment != NULL);

	CString strLog;

	if (m_pInfo->m_pCvInfo->IsDoorOpenReq(nDoorNum) == FALSE)
		return;

	if (m_pInfo->m_pCvInfo->m_pEquipment->IsConnect() == FALSE)
	{
		strLog.Format(_T("%s FANCE 상태를 알 수 없습니다."), DEVICE);
		return;
	}

	if (m_pInfo->m_pCvInfo->IsDoorOpen(nDoorNum) == TRUE)
	{
		strLog.Format(_T("%s FANCE가 이미 열려 있습니다."), DEVICE);
		return;
	}

	// RGV 상태가 Stop 상태인지 확인한다.
	if (m_pInfo->m_bStop == FALSE)
	{
		strLog.Format(_T("%s RGV가 구동중입니다."), DEVICE);
		return;
	}

	m_pInfo->m_bModified = TRUE;
	m_pInfo->InvokeControl();
	m_pInfo->Backup();

	strLog.Format(_T("%s DOOR OPEN ACCEPT"), DEVICE);
//	WriteLog(LOG_TYPE_JOB, strLog, _T("CLgv::InvokeDoorOpenRequest"));
}

void CLgv::InvokeDoorCloseRequest(int nDoorNum /* = 0*/)
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pInfo->m_pCvInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pInfo->m_pCvInfo->m_pEquipment != NULL);

	CString strLog;

	if (m_pInfo->m_pCvInfo->IsDoorOpenReq(nDoorNum) == FALSE)
		return;

	if (m_pInfo->m_pCvInfo->m_pEquipment->IsConnect() == FALSE)
	{
		strLog.Format(_T("%s FANCE 상태를 알 수 없습니다."), DEVICE);
		return;
	}

	if (m_pInfo->m_pCvInfo->IsDoorOpen(nDoorNum) == TRUE)
	{
		strLog.Format(_T("%s FANCE가 아직 열려 있습니다."), DEVICE);
		return;
	}

	// RGV 상태가 Stop 상태인지 확인한다.
	if (m_pInfo->m_bStop == TRUE)
	{
		strLog.Format(_T("%s RGV가 정지중입니다."), DEVICE);
		return;
	}

	m_pInfo->m_bModified = TRUE;
	m_pInfo->InvokeControl();
	m_pInfo->Backup();

}

void CLgv::InvokeCheck()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	CString strLog;
	CTrackHS* pDepartHS = NULL;
	CTrackHS* pArriveHS = NULL;
	CJobItem* pJobItem = NULL;
	CJobItem* pJobItemPrev = NULL;

//	static int nCompleteCount = 0;
	int nDepartsHSCount = m_pInfo->m_pDepartsHS.GetSize();

	for (int i=0; i<nDepartsHSCount; ++i)
	{
		pDepartHS = m_pInfo->m_pDepartsHS.GetNextTrackHS();
		DEBUGER_ASSERT_VALID(pDepartHS != NULL);
		DEBUGER_ASSERT_VALID(pDepartHS->m_pTrack != NULL);

		if ((pDepartHS->m_pTrack->m_bSuspend == TRUE) ||
			(pDepartHS->m_pTrack->m_nLuggNum == 0) ||
			(pDepartHS->m_pTrack->IsAutoMode() == FALSE) ||
			(pDepartHS->m_pTrack->m_nLuggNum == LOG_SYSTEM) ||
			(pDepartHS->m_pTrack->IsRtvDepartHsReady() == FALSE) ||
			(pDepartHS->m_pTrack->m_nDestPos == pDepartHS->m_pTrack->m_nNumber))
			continue;

		if ((pJobItem = m_pDoc->m_pJob->Find(pDepartHS->m_pTrack->m_nLuggNum)) == NULL)
		{
			continue;
		}

		if ((pArriveHS = m_pInfo->m_pArrivesHS.FindTrackHS(pDepartHS->m_nPos, pJobItem->m_strDestStn)) == NULL)
			continue;

		DEBUGER_ASSERT_VALID(pArriveHS->m_pTrack != NULL);

		// 도착지 상태는 작업정지 상태만 확인함! 
		if (pArriveHS->m_pTrack->m_bSuspend == TRUE)
			continue;

		// 한번 Rgv에게 작업 지시한 작업은 다시 작업 지시하지 않음!
		if (pJobItem->m_nJobStatus != enJobStatusCvInvoke && pJobItem->m_nJobStatus != enJobStatusResend)
			continue;

		if (pDepartHS->m_pTrack->m_wErrorCode != 0)
			continue;

		if (pJobItemPrev = m_pDoc->m_pJob->FetchRtvCompleteJob(pArriveHS->m_pTrack->m_nNumber))
		{
			// 원복했음!					
			// - SC DLG의 랙투랙 정지를 Disable 해야 하리라. 
			// 하려면 SC에도 적용해야 함! 

//			if (nCompleteCount > m_pDoc->m_pConfig->m_nCompleteCnt)
//			{
				CTimeSpan tElapseTime = CTime::GetCurrentTime() - pJobItemPrev->m_tTime;
				if (tElapseTime.GetTotalSeconds() < m_pDoc->m_pConfig->m_nCompleteCnt)		// 10
					continue;

				strLog.Format(_T("%s 완료되지 않은 이전작업이 존재합니다. 확인해주세요. [작업번호=%d] [도착위지=TR%d] [작업상태=%s]"), 
					DEVICE, pJobItemPrev->m_nLuggNum, pJobItemPrev->m_nArriveTrackNum, pJobItemPrev->GetJobStatusString());
				m_pDoc->Alarm(LOG_POS_RGV, strLog, pJobItemPrev);
//			}
			continue;
		}
		
//		nCompleteCount = 0;

		pJobItem->m_nTempPickPos = pDepartHS->m_nPos;
		pJobItem->m_nTempDropPos = pArriveHS->m_nPos;

		pJobItem->m_nTempDepartTrackNum = pDepartHS->m_pTrack->m_nNumber;
		pJobItem->m_nTempArriveTrackNum = pArriveHS->m_pTrack->m_nNumber;

		if (InvokeJob(pJobItem->m_nLuggNum, pDepartHS->m_nPos, pArriveHS->m_nPos, pJobItem->m_strProductID))
		{
			strLog.Format(_T("%s 작업 지시 요청 [ST%03d -> ST%03d] [LUGG=%d]"), 
				DEVICE, pDepartHS->m_nPos, pArriveHS->m_nPos, pJobItem->m_nLuggNum);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CLgv::InvokeCheck"), pJobItem);
		}
		else
		{
			strLog.Format(_T("%s 작업지시요청 실패! [ST%03d -> ST%03d] [LUGG=%d] [%s]"), 
				DEVICE, pDepartHS->m_nPos, pArriveHS->m_nPos, pJobItem->m_nLuggNum, m_pThreadSocket ? m_pThreadSocket->m_strLog : m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::InvokeCheck"), pJobItem);
		}
	}
}

void CLgv::CommandProc()
{
	CString strLog;
	CCmdMsg* pCmdMsg = NULL;
//	CLgvJobItem* pLgvJobItem = NULL;
	for (POSITION pos=m_cmdList.GetHeadPosition(); pos!=NULL; )
	{
		pCmdMsg = (CCmdMsg*)m_cmdList.GetNext(pos);
		CStringArray& strValues = pCmdMsg->m_strValues;
		DEBUGER_ASSERT_VALID(pCmdMsg != NULL);

		switch (pCmdMsg->m_nCommand)
		{
		case CLgvCmdMsg::enCmdSuspend:
			{
				DEBUGER_ASSERT_VALID(strValues.GetSize() == 1);
				m_pInfo->m_bSuspend = _ttoi(strValues[0]);
				m_pInfo->m_bModified = TRUE;
//				m_pInfo->MulticastDoor(NULL);
				m_pInfo->InvokeControl();

				strLog.Format(_T("%s 일시정지"), DEVICE);
				m_pDoc->StatusDisplay(strLog, FALSE);
			}
			break;

		case CLgvCmdMsg::enRgvStop:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == 1);
			if (RequestStop(_ttoi(strValues[0])))
			{
				strLog.Format(_T("%s RGV %s 요청"), DEVICE, _ttoi(strValues[0]) ? "정지":"기동");
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CLgv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s RGV %s 요청 실패! [%s]"), DEVICE, _ttoi(strValues[0]) ? "정지":"기동", m_pThreadSocket ? m_pThreadSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::CommandProc"));
			}
			break;

		case CLgvCmdMsg::enTRR:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CLgvCmdMsg::enSizeTRR);
			if (InvokeJob(_ttoi(strValues[0]), _ttoi(strValues[1]), _ttoi(strValues[2]), strValues[3]))
			{
				m_pInfo->m_bModified = TRUE;
				m_pInfo->InvokeControl();
				m_pInfo->Backup();

				strLog.Format(_T("%s 작업지시요청 [ST%03d -> ST%03d] [LUGG=%s]"), 
					DEVICE, _ttoi(strValues[1]), _ttoi(strValues[2]), strValues[0]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CLgv::CommandProc"));

			}
			else
			{
				strLog.Format(_T("%s 작업지시요청 실패! [ST%03d -> ST%03d] [LUGG=%s] [%s]"), 
					DEVICE, _ttoi(strValues[1]), _ttoi(strValues[2]), strValues[0], m_pThreadSocket ? m_pThreadSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CLgv::CommandProc"));
			}
			break;
			/*
		case CLgvCmdMsg::enDEL:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CLgvCmdMsg::enSizeDEL);

			pLgvJobItem = (CLgvJobItem*) m_pInfo->GetLgvJobItem(_ttoi(strValues[0]));
			if (pLgvJobItem != NULL)
			{
				CLgvVehicle* pLgvVehicle = NULL;
				if (pLgvJobItem->m_nVehicleID != 0)
					pLgvVehicle = (CLgvVehicle*) m_pInfo->GetLgvVehicle(pLgvJobItem->m_nVehicleID);
				
				if (pLgvVehicle != NULL && pLgvVehicle->m_nLuggNum != 0) 
					pLgvVehicle->m_nLuggNum = 0;

				// RGV 작업정보도 완료해준다.
				m_pInfo->m_bModified = TRUE;
				m_pInfo->MulticastRemoveJob(pLgvJobItem);
				m_pInfo->RemoveLgvJobItem(_ttoi(strValues[0]));
				m_pInfo->InvokeControl();
				m_pInfo->Backup();


				strLog.Format(_T("%s 작업삭제 [LUGG=%s]"), DEVICE, strValues[0]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CLgv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 작업삭제 실패! [LUGG=%s] [RGV 작업정보 없음!] [%s]"), DEVICE, strValues[0], m_pThreadSocket ? m_pThreadSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CLgv::CommandProc"));
			}

			break;

		case CLgvCmdMsg::enMOD:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CLgvCmdMsg::enSizeMOD);
			if (RequestModifyTransport(_ttoi(strValues[0]), _ttoi(strValues[1]), _ttoi(strValues[2]), _ttoi(strValues[3]), _ttoi(strValues[4])))
			{
				strLog.Format(_T("%s 작업수정요청 [ST%03d -> ST%03d] [LUGG=%s, CRATE=%s, PRIORITY=%s]"), 
					DEVICE, _ttoi(strValues[3]), _ttoi(strValues[4]), strValues[0], strValues[1], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CLgv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 작업수정요청 실패! [ST%03d -> ST%03d] [LUGG=%s, CRATE=%s, PRIORITY=%s] [%s]"), 
					DEVICE, _ttoi(strValues[3]), _ttoi(strValues[4]), strValues[0], strValues[1], strValues[2], m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), m_strLog, _T("CLgv::CommandProc"));
			}
			break;

		case CLgvCmdMsg::enTRQ:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CLgvCmdMsg::enSizeTRQ);
			if (RequestStatusTransport(_ttoi(strValues[0])))
			{
				strLog.Format(_T("%s 작업상태요청 [LUGG=%s]"), DEVICE, strValues[0]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CLgv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 작업상태요청 실패! [LUGG=%s] [%s]"), DEVICE, strValues[0], m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CLgv::CommandProc"));
			}
			break;

		case CLgvCmdMsg::enAGQ:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CLgvCmdMsg::enSizeAGQ);
			if (RequestStatusVehicle(_ttoi(strValues[0])))
			{
				strLog.Format(_T("%s AGV 상태요청 [VID=%s]"), DEVICE, strValues[0]);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CLgv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s AGV 상태요청 실패! [VID=%s] [%s]"), DEVICE, strValues[0], m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::CommandProc"));
			}
			break;

		case CLgvCmdMsg::enTEX:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CLgvCmdMsg::enSizeTEX);
			if (RequestExpectedTransport(_ttoi(strValues[0]), _ttoi(strValues[1]), _ttoi(strValues[2])))
			{
				strLog.Format(_T("%s 이동요청 [PRIORITY=%s, POS=%s, TIME=%s]"), DEVICE, strValues[0], strValues[1], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CLgv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 이동요청 실패! [PRIORITY=%s, POS=%s, TIME=%s] [%s]"), DEVICE, strValues[0], strValues[1], strValues[2], m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::CommandProc"));
			}
			break;

		case CLgvCmdMsg::enHOS:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CLgvCmdMsg::enSizeHOS);
			if (ReportStatusDoor(_ttoi(strValues[0]), _ttoi(strValues[1])))
			{
				strLog.Format(_T("%s DOOR 상태요청 [DOOR=%s, STATUS=%s]"), DEVICE, strValues[0], strValues[1]);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CLgv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s DOOR 상태요청 실패! [DOOR=%s, STATUS=%s] [%s]"), DEVICE, strValues[0], strValues[1], m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::CommandProc"));
			}
			break;
*/		
		default:
			strLog.Format(_T("%s 유효하지않은 메시지형식! [CMD=%d]"), DEVICE, pCmdMsg->m_nCommand);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::CommandProc"));
		}

		delete pCmdMsg;
		pCmdMsg = NULL;
	}

	m_cmdList.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
BOOL CLgv::RequestJobInfo()
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

	CByteArray arrBuffer;
	arrBuffer.SetSize(4);

	CString strMsg, strLog;

	strMsg.Format(_T("%c<Message Name='JobInfoRequest'/>%c"), 
		CAsyncSocketEx::enSTX, nStop, CAsyncSocketEx::enETX);

	m_pInfo->m_bStopRequest = nStop;

	if (!m_pThreadSocket->SendData(strMsg, FALSE))
	{
		strLog = m_pThreadSocket->m_strLog;
		return FALSE;
	}

	return TRUE;
}
*/

BOOL CLgv::RequestStop(int nStop)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

	CByteArray arrBuffer;
	arrBuffer.SetSize(4);

	CString strMsg, strLog;

	strMsg.Format(_T("%c<Message Name='Stop'><RGV Stop='%d'/></Message>%c"), 
		CAsyncSocketEx::enSTX, nStop, CAsyncSocketEx::enETX);

	m_pInfo->m_bStopRequest = nStop;

	if (!m_pThreadSocket->SendData(strMsg, FALSE))
	{
		strLog = m_pThreadSocket->m_strLog;
		return FALSE;
	}

	return TRUE;
}

/*
BOOL CLgv::ParseMessageJobInfo()
{
	CString strValue, strLog;
	try
	{
		if (m_pXml->MoveXPath(_T("./Message")))	
		{
			m_pXml->GetAttrValue(_T("Name"), strValue);
			
			if (strValue == "JobInfoRequest_Reply")
			{
//				m_pXml->MoveChild(0);
//					m_pXml->GetAttrValue(_T("Stop"), strValue);	
//					
//					if (m_pInfo->m_bStopRequest == _ttoi(strValue))
//					{
//						m_pInfo->m_bStop = m_pInfo->m_bStopRequest;	
//						m_pInfo->m_bModified = TRUE;
//						m_pInfo->InvokeControl();
//						m_pInfo->m_bStopRequest = FALSE;
//
//						return TRUE;			// Stop 지시한 값과 같으면 
//					}

				int nMode, nRun, nContain, nECode;
				int nCount = m_pXml->GetChildElmtCount();
				for (int i = 0 ; i < nCount ; i++)
				{
					m_pXml->MoveChild(i);

					m_pXml->GetAttrValue(_T("No"), strValue);			nCarNum		= _ttoi(strValue);
					m_pXml->GetAttrValue(_T("Mode"), strValue);			nMode		= _ttoi(strValue);
					m_pXml->GetAttrValue(_T("Run"), strValue);			nRun		= _ttoi(strValue);
					m_pXml->GetAttrValue(_T("Contain"), strValue);		nContain	= _ttoi(strValue);
					m_pXml->GetAttrValue(_T("Position"), strValue);		nPosition	= _ttoi(strValue);
					m_pXml->GetAttrValue(_T("ECode"), strValue);		nECode		= _ttoi(strValue);
					m_pXml->GetAttrValue(_T("LuggNo"), strValue);		nLuggNo		= _ttoi(strValue);

					CLgvVehicle* pVehicle = (CLgvVehicle*)m_pInfo->GetLgvVehicle(nCarNum);

					if (pVehicle == NULL)
						continue;

					// - 화면에 표시하는 용도임!
					pVehicle->SetMode(nMode);					// RGV 동작모드		- {0:Manual, 1:Local, 2:Auto}					
					pVehicle->SetStatus(nRun);					// RGV 동작상태		- {0:대기, 1:이동, 2:로딩, 3:언로딩, 4:에러}	
					pVehicle->SetProdLoad(nContain);			// RGV 화물감지상태 - {0:화물무, 1:화물유}						 	
					pVehicle->SetHorizontalPos(nPosition);		// RGV 위치
					pVehicle->SetErrorCode(nECode);				// RGV 에러코드
					pVehicle->SetLuggNum(nLuggNo);				// RGV 작업번호
					m_pInfo->m_bModified = TRUE;

					m_pInfo->InvokeControl(pVehicle->m_nVehicleID);
					m_pInfo->Backup();

					m_pXml->MoveParent();
				}

			}
		}
	}
	catch (_com_error& e)
	{
		strLog.Format(_T("XML PARSING 실패! [%s]"), m_pXml->GetErrorMessage(e));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_RGV, strLog, _T("CLgv::ParseMessageStop"));
		return FALSE;
	}

	return FALSE;

}
*/
BOOL CLgv::ParseMessageStop()
{
	CString strValue, strLog;
	try
	{
		if (m_pXml->MoveXPath(_T("./Message")))	
		{
			m_pXml->GetAttrValue(_T("Name"), strValue);
			

			if (strValue == "Stop_Reply")
			{
				strLog.Format(_T("%s %s Message 수신!"), DEVICE, strValue);
				m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_RGV, strLog, _T("CLgv::ReadProcess"));

				m_pXml->MoveChild(0);
					m_pXml->GetAttrValue(_T("Stop"), strValue);	
					
						m_pInfo->m_bStop = BOOL(_ttoi(strValue));	
						m_pInfo->m_bModified = TRUE;
						m_pInfo->InvokeControl();
						m_pInfo->MulticastCommStatus();
						m_pInfo->m_bStopRequest = FALSE;

						return TRUE;			// Stop 지시한 값과 같으면 
			}
		}
	}
	catch (_com_error& e)
	{
		strLog.Format(_T("XML PARSING 실패! [%s]"), m_pXml->GetErrorMessage(e));
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::ParseMessageStop"));
		return FALSE;
	}

	return FALSE;

}

BOOL CLgv::ParseMessageTransfer()
{
	CString strValue, strLog;
//	CLgvJobItem* pLgvJobItem = NULL;
	try
	{
		if (m_pXml->MoveXPath(_T("./Message")))	
		{
			m_pXml->GetAttrValue(_T("Name"), strValue);
			

			if (strValue == "Transfer_Reply")
			{
				strLog.Format(_T("%s %s Message 수신!"), DEVICE, strValue);
				m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_RGV, strLog, _T("CLgv::ReadProcess"));

				m_pXml->MoveChild(0);

				m_pXml->GetAttrValue(_T("LuggNo"), strValue);				int nLuggNo		= _ttoi(strValue);
				m_pXml->GetAttrValue(_T("ReturnCode"), strValue);			int nReturnCode = _ttoi(strValue);
				
				CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNo);
				if (nReturnCode == 0 && nLuggNo == m_pInfo->m_nRequestLuggNum)
				{
					// 작업정보를 완료해준다.
					if (pJobItem != NULL )
					{
						// 혹시 해당 작업이 RGV 작업정보에 존재할경우 삭제한다 
						//	- 지금부터 작업추가할 것인데 RGV 작업정보에 존재하는 작업을 추가할 수는 없기 때문에
//						if (pLgvJobItem != NULL)
//						{
							CLgvVehicle* pLgvVehicle = NULL;
							if (pJobItem->m_nVehicleID != 0)
								pLgvVehicle = (CLgvVehicle*) m_pInfo->GetLgvVehicle(pJobItem->m_nVehicleID);
							
							if (pLgvVehicle != NULL && pLgvVehicle->m_nLuggNum != 0) 
								pLgvVehicle->m_nLuggNum = 0;

							// 작업을 삭제한다?
							m_pInfo->m_bModified = TRUE;
							m_pInfo->m_bInvoke = TRUE;
							m_pInfo->MulticastCommStatus();
							m_pInfo->InvokeControl();
//						}
							m_pInfo->Backup();
//						pJobItem->m_nActivity = CJobItem::enActivityQueue;
						pJobItem->SetJobStatus(enJobStatusRtvInvoke);
//						pJobItem->m_nRgvcNum = m_pInfo->m_pEquipment->m_nNumber;
						pJobItem->SetRgvcJobStatus(CJobItem::enActivityQueue, m_pInfo->m_pEquipment->m_nNumber, pJobItem->m_nVehicleID);
						m_pDoc->m_pJob->Backup();

						m_pInfo->m_nRequestLuggNum = 0;

						return TRUE;
					}
					else
					{
						strLog.Format(_T("응답된 작업번호가 작업정보에 없음! [작업번호 = %d]"), nLuggNo);
						WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::ParseMessageTransfer"));
						return FALSE;
					}
				}
				else
				{
					strLog.Format(_T("응답코드가 정상이 아님! [응답 코드 = %d]"), nReturnCode);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::ParseMessageTransfer"));

					if (pJobItem != NULL)
					{
						CLgvVehicle* pLgvVehicle = NULL;
						if (pJobItem->m_nVehicleID != 0)
							pLgvVehicle = (CLgvVehicle*) m_pInfo->GetLgvVehicle(pJobItem->m_nVehicleID);
					
						if (pLgvVehicle != NULL && pLgvVehicle->m_nLuggNum != 0) 
							pLgvVehicle->m_nLuggNum = 0;

						// 작업을 삭제한다?
						m_pInfo->m_bModified = TRUE;
						m_pInfo->InvokeControl();
						m_pInfo->Backup();

						strLog.Format(_T("응답코드가 정상이 아닌데 작업정보가 있음! [작업번호 = %d][응답 코드 = %d]"), nLuggNo, nReturnCode);
						WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::ParseMessageTransfer"));
					}
					else
					{
						strLog.Format(_T("응답코드가 정상이 아닌데 작업정보도 존재하지 않음! [작업번호 = %d][응답 코드 = %d]"), nLuggNo, nReturnCode);
						WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::ParseMessageTransfer"));
					}
					return FALSE;
				}
			}
		}
	}
	catch (_com_error& e)
	{
		strLog.Format(_T("XML PARSING 실패! [%s]"), m_pXml->GetErrorMessage(e));
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CLgv::ParseMessageTransfer"));
		return FALSE;
	}

	return FALSE;

}

BOOL CLgv::InvokeJob(int nLuggNum, int nSource, int nDest, CString strProdInfo)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

	CByteArray arrBuffer;
	arrBuffer.SetSize(4);

	CString strMsg, strLog;
	strMsg.Format(_T("%c<Message Name='Transfer'><Transfer LuggNo='%d' Source='%d' Dest='%d' ProdInfo1='%s'/></Message>%c"), 
		CAsyncSocketEx::enSTX, nLuggNum, nSource, nDest, strProdInfo, CAsyncSocketEx::enETX);

	m_pInfo->m_nRequestLuggNum = nLuggNum;

	if (!m_pThreadSocket->SendData(strMsg, FALSE))
	{
		strLog = m_pThreadSocket->m_strLog;
		return FALSE;
	}

	return TRUE;
}

