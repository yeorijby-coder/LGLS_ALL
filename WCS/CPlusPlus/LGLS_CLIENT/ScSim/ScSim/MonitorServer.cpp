// MonitorServer.cpp: implementation of the CMonitorServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "MonitorServer.h"
#include "ByteArrayEx.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonitorServer::CMonitorServer(CEcsDoc* pDoc) : CAsyncSocketEx(_T("MonitorServer"), pDoc->m_pConfig->m_bMonLog)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	m_pXml = new CXmlDom;
	DEBUGER_ASSERT_VALID(m_pXml != NULL);

	m_nPeerPort = 0;
	m_bValidate = FALSE;
}

CMonitorServer::~CMonitorServer()
{
	delete m_pXml;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CMonitorServer::InitializeXmlDom()
{
	DEBUGER_ASSERT_VALID(m_pXml != NULL);
	return m_pXml->InitializeXmlDom();
}

void CMonitorServer::OnClose(int nErrorCode)
{
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pDoc->m_pMonitorListener != NULL);

	CString strLog;
	strLog.Format(_T("ECS MONITOR CLIENT 연결 해제! [%s] [%s]"), GetPeerInfo(), CLib::GetSystemErrMsg(nErrorCode));
	m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnClose"));

	m_pDoc->m_pMonitorListener->RemoveClient(this);
	CAsyncSocket::ShutDown();
	delete this;
}

void CMonitorServer::OnReceive(int nErrorCode)
{
	CString strLog;
	if (nErrorCode)
	{
		OnClose(nErrorCode);
		return;
	}
	
	CByteArray arrBuffer;
//	CByteArrayEx arrBuffer(nStartWordAddr, nReadWordSize);
//	arrBuffer.SetArray(nStartWordAddr, nRWSTemp);

	
	if (!RecvData(arrBuffer))
	{
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, m_strLog, _T("CMonitorServer::OnReceive"));
		return;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int nRecvLen = arrBuffer.GetSize();

	//CString& strRecvData = arrBuffer.GetData();
	
	/*
	WORD wErrorCode = (WORD)((arrBuffer[10] << 8) | arrBuffer[9]);
	if (wErrorCode)
	{
		m_strLog.Format(_T("응답 프레임 에러! [ERROR=%d]"), wErrorCode);
		return FALSE;
	}
	
	arrRxData.SetSize(nWordLen*2);
	for (int i=0; i<arrRxData.GetSize(); ++i)
		arrRxData[i] = arrBuffer[enBinaryHeaderLen+i];
	*/

	
	if (arrBuffer[0] != 0x50 || arrBuffer[1] != 0x00)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 SUB HEADER 이상!"));
		return;
	}

	if (arrBuffer[2] != 0x00 || arrBuffer[3] != 0xFF)		// Network NO, PC NO
	{
		m_strLog.Format(_T("응답 프레임 NETWORK OR PC NO 이상!"));
		return;
	}

	if (arrBuffer[4] != 0xFF || arrBuffer[5] != 0x03 || arrBuffer[6] != 0x00)   // Specific value
	{
		m_strLog.Format(_T("응답 프레임 SPECIFIC VALUE 이상!"));
		return;
	}

	if (arrBuffer[7] != 0x0C || arrBuffer[8] != 0x00)   // Specific value
	{
		m_strLog.Format(_T("응답 프레임 SPECIFIC VALUE 이상!"));
		return;
	}

	int bbb1 = arrBuffer[19];
	int bbb2 = arrBuffer[20];

	/*
	BYTE TxBuff[1024];
	memset(TxBuff, 0x00, sizeof(TxBuff));
//	WORD wReqLen = 12 + GetDataLength(nUnitType, wWriteLen);

    TxBuff[0]  = 0xD0;  
	TxBuff[1]  = 0x00;	// Subheader	

	TxBuff[2]  = 0x00;	// Network No. (Self station)
	TxBuff[3]  = 0xFF;	// PC No.

    TxBuff[4]  = 0xFF;	
	TxBuff[5]  = 0x03;	// Specific value
	TxBuff[6]  = 0x00;	// Specific value 2

	TxBuff[7]  = wReqLen & 0x00FF;
	TxBuff[8]  = (wReqLen >> 8) & 0x00FF;  	// Request data length

	TxBuff[9]  = 0x08;  // 0x04;
	TxBuff[10] = 0x00;	// CPU monitoring timer (unit is 250ms)

	TxBuff[11] = 0x01;
	TxBuff[12] = 0x14;  //Command
	TxBuff[13] = nUnitType;
	TxBuff[14] = 0x00; 	// Subcommand

	TxBuff[15] = (nStartAddr & 0xFF);
	TxBuff[16] = (nStartAddr >> 8) & 0xFF;
	TxBuff[17] = (nStartAddr >> 16) & 0xFF;		// Head device (Start Address)

	TxBuff[18] = DeviceCode;					// Device code (Memory Type)

	TxBuff[19] = wWriteLen & 0x00FF;            //WORD단위:쓸 워드수, BIT단위:쓸 비트수
	TxBuff[20] = (wWriteLen >> 8) & 0x00FF;		// Number of device points

	memcpy(TxBuff+21, pTxBuff, GetDataLength(nUnitType, wWriteLen));
	int nLen = 21 + GetDataLength(nUnitType, wWriteLen);

	if( Send(TxBuff, nLen) == SOCKET_ERROR )
	{
		m_strErrMsg.Format("Write.. 송신 에러 [%s]", CLib::GetSystemErrMsg());
		return FALSE;
	}
*/

//	int strSubHeader = arrBuffer;
    int strnNetworkNo = arrBuffer[2];
    int strnPCNo = arrBuffer[3];
    //CString strnRequestIONo = (arrBuffer[5] * &H100) + arrBuffer[4];
    int strnRequestKookNo = arrBuffer[6];
    //CString strnLen = (arrBuffer[8] * &H100) + arrBuffer[7];

	if (m_bValidate == FALSE)
		OnClose(0);
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CMonitorServer::ParseSys()
{
	DEBUGER_ASSERT_VALID(m_pXml != NULL);

	CString strLog;
	CString strValue, strSend;

	try
	{
		m_pXml->GetElmtName(strValue);
		DEBUGER_ASSERT_VALID(strValue == _T("SYS"));

		if (m_pXml->MoveXPath(_T("./VALIDATION"), FALSE))
		{
			if (m_pXml->MoveXPath(_T("./NAME"), FALSE))
			{
				m_pXml->GetElmtValue(m_strPeerName);
				m_pXml->MoveParent();
			}

			m_pXml->GetChildElmtValue(_T("SERVER"), strValue);
			if (strValue != ECS_SERVER_ID)
			{
				strLog.Format(_T("%s 유효하지 않은 ECS SERVER ID [SERVER=%s, CLIENT=%s]"), GetClientName(), ECS_SERVER_ID, strValue);
				m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseSys"));

				strSend.Format(_T("%c<ECS><SYS><VALIDATION><REJECT MSG='%s'/></VALIDATION></SYS></ECS>%c"), enSTX, strLog, enETX);
				CAsyncSocketEx::SendData(strSend);
				m_bValidate = FALSE;
				return FALSE;
			}

			m_pXml->GetChildElmtValue(_T("VERSION"), strValue);
			if (strValue != ECS_CLIENT_VERSION)
			{
				strLog.Format(_T("%s Version 불일치! [SERVER=%s, CLIENT=%s]"), GetClientName(), ECS_CLIENT_VERSION, strValue);
				m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseSys"));

				strSend.Format(_T("%c<ECS><SYS><VALIDATION><REJECT MSG='%s'/></VALIDATION></SYS></ECS>%c"), enSTX, strLog, enETX);
				CAsyncSocketEx::SendData(strSend);
				m_bValidate = FALSE;
				return FALSE;
			}

			strLog.Format(_T("%s CLIENT 인증 성공"), GetClientName());
			m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseSys"));

			strSend.Format(_T("%c<ECS><SYS><VALIDATION><ACCEPT/></VALIDATION></SYS></ECS>%c"), enSTX, enETX);
			CAsyncSocketEx::SendData(strSend);
			m_bValidate = TRUE;

			m_pDoc->MulticastServerInfo(this, (m_pDoc->m_pHostSv && m_pDoc->m_pHostSv->IsConnect()) ? NOTIFY_SEND : NOTIFY_ERROR);
			m_pDoc->m_pJob->MulticastAllJobPerClient(this);
			m_pDoc->m_pEquipments.MulticastInfo(this);
		}
	}
	catch (_com_error& e)
	{
		strLog.Format(_T("%s XML PARSING 실패! [%s]"), GetClientName(), m_pXml->GetErrorMessage(e));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseSys"));
		m_bValidate = FALSE;
		return FALSE;
	}

	return TRUE;
}

BOOL CMonitorServer::ParseJob()
{
	DEBUGER_ASSERT_VALID(m_pXml != NULL);

	CString strLog, strValue;
	CJobItem* pJobItem = NULL;

	try
	{
		m_pXml->GetElmtName(strValue);
		DEBUGER_ASSERT_VALID(strValue == _T("JOB"));
		DEBUGER_ASSERT_VALID(m_pXml->GetChildElmtCount() == 1);

		if (m_pXml->MoveXPath(_T("./ADD/ITEM"), FALSE))
		{
			CJobItem oJobItem(m_pDoc);
			m_pXml->GetAttrValue(_T("NO"), strValue);		oJobItem.m_nLuggNum = atoi(strValue);
			m_pXml->GetAttrValue(_T("JT"), strValue);		oJobItem.m_nJobType = atoi(strValue);
			m_pXml->GetAttrValue(_T("SWH"), strValue);		oJobItem.m_nStartWH = atoi(strValue);
			m_pXml->GetAttrValue(_T("SSTN"), strValue);		oJobItem.m_strStartStn = strValue;
			m_pXml->GetAttrValue(_T("SLOC"), strValue);		oJobItem.m_strStartLoc = strValue;
			m_pXml->GetAttrValue(_T("DWH"), strValue);		oJobItem.m_nDestWH = atoi(strValue);
			m_pXml->GetAttrValue(_T("DSTN"), strValue);		oJobItem.m_strDestStn = strValue;
			m_pXml->GetAttrValue(_T("DLOC"), strValue);		oJobItem.m_strDestLoc = strValue;
			m_pXml->GetAttrValue(_T("GEN"), strValue);		oJobItem.m_ucGenCode = atoi(strValue);
			m_pXml->GetAttrValue(_T("CRTK"), strValue);		oJobItem.m_ucCrateKind = atoi(strValue);
			m_pXml->GetAttrValue(_T("CRTS"), strValue);		oJobItem.m_ucCrateStatus = atoi(strValue);
			m_pXml->GetAttrValue(_T("BCD"), strValue);		oJobItem.m_strBarcode = strValue;
			m_pXml->GetAttrValue(_T("PROD"), strValue);		oJobItem.m_strProductID = strValue;
//			m_pXml->GetAttrValue(_T("LINE"), strValue);		oJobItem.m_strLineInfo = strValue;
			m_pXml->GetAttrValue(_T("PLTNO"), strValue);	oJobItem.m_strPalletNo = strValue;
			m_pXml->GetAttrValue(_T("CMPN"), strValue);		oJobItem.m_strCompany = strValue;
			m_pXml->GetAttrValue(_T("CNT"), strValue);		oJobItem.m_nProdCount = atoi(strValue);
			m_pXml->GetAttrValue(_T("STS"), strValue);		oJobItem.m_nJobStatus = atoi(strValue);
			m_pXml->GetAttrValue(_T("RES"), strValue);		oJobItem.m_nResultCode = atoi(strValue);
			m_pXml->GetAttrValue(_T("DTR"), strValue);		oJobItem.m_nDepartTrackNum = atoi(strValue);
			m_pXml->GetAttrValue(_T("ATR"), strValue);		oJobItem.m_nArriveTrackNum = atoi(strValue);
			m_pXml->GetAttrValue(_T("SIZE"), strValue);		oJobItem.m_nSize = atoi(strValue);
			m_pXml->GetAttrValue(_T("TIME"), strValue);		oJobItem.m_tTime = CTime((time_t)atoi(strValue));

			strLog.Format(_T("%s 작업추가 요청 [%s]"), GetClientName(), oJobItem.GetLogString());
 			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			if ((pJobItem = m_pDoc->m_pJob->Add(oJobItem)) == NULL)
			{
				strLog.Format(_T("%s 작업추가 실패 [%s]"), GetClientName(), oJobItem.GetLogString());
 				m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem, oJobItem.m_strProductID, TRUE);
				return FALSE;
			}

			m_pDoc->m_pJob->Invoke(pJobItem);
		}
		else if (m_pXml->MoveXPath(_T("./EDIT/ITEM"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(atoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 수정할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"));

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			m_pXml->GetAttrValue(_T("STS"), strValue);
			int nJobStatus = atoi(strValue);

			strLog.Format(_T("%s 작업수정 요청 [%s -> %s]"), GetClientName(), pJobItem->GetJobStatusString(), CJobItem::GetJobStatusString(nJobStatus));
 			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			pJobItem->SetJobStatus(nJobStatus);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;
		}
		else if (m_pXml->MoveXPath(_T("./REMOVE/ITEM"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(atoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 삭제할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"));

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			strLog.Format(_T("%s 작업삭제 요청 [%s]"), GetClientName(), pJobItem->GetLogString());
 			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			m_pDoc->m_pJob->Remove(pJobItem);
		}
		else if (m_pXml->MoveXPath(_T("./RESEND"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(atoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 재전송할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"));

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			strLog.Format(_T("%s 작업재전송 요청 [%s]"), GetClientName(), pJobItem->GetJobStatusString());
			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			pJobItem->MulticastAddJob(this);
		}
		else if (m_pXml->MoveXPath(_T("./COMPLETE"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(atoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 완료보고할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"));

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			strLog.Format(_T("%s 작업완료 요청 [%s]"), GetClientName(), pJobItem->GetJobStatusString());
			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			m_pXml->GetAttrValue(_T("TYPE"), strValue);
			m_pDoc->m_pJob->Complete(pJobItem, (BYTE)atoi(strValue), TRUE);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;
		}
		else if (m_pXml->MoveXPath(_T("./ARRIVE"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(atoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 도착보고할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"));

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			strLog.Format(_T("%s 도착완료 요청 [%s]"), GetClientName(), pJobItem->GetJobStatusString());
			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			m_pDoc->m_pJob->Arrive(pJobItem, TRUE);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;
		}
		else if (m_pXml->MoveXPath(_T("./CANCEL"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(atoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 최소보고할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->StatusDisplay(strLog);

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			strLog.Format(_T("%s 작업취소 요청 [%s]"), GetClientName(), pJobItem->GetJobStatusString());
			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			m_pDoc->m_pJob->Cancel(pJobItem, TRUE);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;
		}
		else if (m_pXml->MoveXPath(_T("./REQUEST"), FALSE))
		{
			CString strStationID, strBarcode;
			m_pXml->GetAttrValue(_T("BCD"), strBarcode);
			m_pXml->GetAttrValue(_T("SID"), strStationID);

			strLog.Format(_T("%s 작업요청 [입고대=%s, BARCODE=%s]"), GetClientName(), m_pDoc->GetStationName(strStationID), strBarcode);
 			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			m_pDoc->m_pJob->Request(strStationID, strBarcode);
		}
		else
		{
			strLog.Format(_T("%s 등록되지 않은 ELEMENT! [NAME=%s]"), GetClientName(), strValue);
			m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"));
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		strLog.Format(_T("%s XML PARSING 실패! [%s]"), GetClientName(), m_pXml->GetErrorMessage(e));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"));
		m_bValidate = FALSE;
		return FALSE;
	}

	return TRUE;
}

BOOL CMonitorServer::ParseEquip()
{
	DEBUGER_ASSERT_VALID(m_pXml != NULL);

	CString strLog, strValue;
	CEquipment* pEquipment = NULL;

	try
	{
		m_pXml->GetElmtName(strValue);
		DEBUGER_ASSERT_VALID(strValue == _T("EQUIP"));

		m_pXml->GetAttrValue(_T("DEVICE"), strValue);
		pEquipment = m_pDoc->GetEquipment(strValue);

		if (pEquipment == NULL)
		{
			strLog.Format(_T("%s 등록되지 않은 DEVICE명입니다. 확인해주세요! [DEVICE=%s]"), GetClientName(), strValue);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseEquip"), NULL, NULL, TRUE);
			return FALSE;
		}

		int nCount = m_pXml->GetChildElmtCount();
		for (int i=0; i<nCount; ++i)
		{
			m_pXml->MoveChild(i);
			m_pXml->GetElmtName(strValue);

			if (strValue == _T("CMD"))
			{
				CCmdMsg* pCmdMsg = new CCmdMsg;
				DEBUGER_ASSERT_VALID(pCmdMsg != NULL);

				m_pXml->GetAttrValue(_T("TYPE"), strValue);
				pCmdMsg->m_nCommand = atoi(strValue);

				m_pXml->GetAttrValue(_T("SUB"), strValue);
				pCmdMsg->m_nSubCmd = atoi(strValue);

				pCmdMsg->m_strValues.SetSize(m_pXml->GetChildElmtCount());
				for (int j=0; j<pCmdMsg->m_strValues.GetSize(); ++j)
				{
					m_pXml->GetChildElmtValue(j, pCmdMsg->m_strValues[j]);
				}
				pEquipment->SetCmdMsg(pCmdMsg);

				strLog.Format(_T("%s %s 작업 요청"), GetClientName(), pEquipment->GetDeviceName());
				m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseEquip"));
			}

			m_pXml->MoveParent();
		}
	}
	catch (_com_error& e)
	{
		strLog.Format(_T("%s XML PARSING 실패! [%s]"), GetClientName(), m_pXml->GetErrorMessage(e));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseEquip"));
		return FALSE;
	}

	return TRUE;
}