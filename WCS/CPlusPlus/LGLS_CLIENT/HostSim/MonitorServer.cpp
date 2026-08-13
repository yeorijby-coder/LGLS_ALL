// MonitorServer.cpp: implementation of the CMonitorServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "MonitorServer.h"
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
//	DEBUGER_ASSERT_VALID(m_pDoc->m_pMonitorListener != NULL);

	CString strLog;
	strLog.Format(_T("ECS MONITOR CLIENT 연결 해제! [%s] [%s]"), GetPeerInfo(), CLib::GetSystemErrMsg(nErrorCode));
	m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnClose"));

//	m_pDoc->m_pMonitorListener->RemoveClient(this);
//	CAsyncSocket::ShutDown();
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

	CString strRecv;
	if (!RecvData(strRecv))
	{
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, m_strLog, _T("CMonitorServer::OnReceive"));
		return;
	}
	
	DEBUGER_ASSERT_VALID(m_pXml != NULL);
	CAsyncSocketEx::TokenRxFrame(strRecv, enSTX, enETX);
	for (POSITION pos=m_strRxList.GetHeadPosition(); pos!=NULL; )
	{
		CString& strRxFrame = m_strRxList.GetNext(pos);
		int nLen = strRxFrame.GetLength();

		if (nLen < 12)
		{
			strLog.Format(_T("수신데이터 길이 이상! [LEN=%d]"), nLen);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnReceive"));
			continue;
		}

		if (strRxFrame[0] != enSTX)
		{
			strLog.Format(_T("수신데이터 이상! [NO STX]"));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnReceive"));
			continue;
		}

		if (strRxFrame[nLen-1] != enETX)
		{
			strLog.Format(_T("수신데이터 이상! [NO ETX]"));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnReceive"));
			continue;
		}

		if (m_pXml->LoadXmlString(strRxFrame.Mid(1, nLen-2)) == FALSE)
		{
			strLog.Format(_T("LoadXmlString 실패! [%s]"), m_pXml->GetErrorMessage());
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::OnReceive"));
			continue;
		}

		if (m_pXml->MoveXPath(_T("/ECS/SYS")))
			ParseSys();

		if (m_pXml->MoveXPath(_T("/ECS/JOB")))
			ParseJob();

		if (m_pXml->MoveXPath(_T("/ECS/EQUIP")))
			ParseEquip();
	}

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
				m_pDoc->Alarm(LOG_POS_MONITOR, strLog);

				strSend.Format(_T("%c<ECS><SYS><VALIDATION><REJECT MSG='%s'/></VALIDATION></SYS></ECS>%c"), enSTX, strLog, enETX);
				CAsyncSocketEx::SendData(strSend);
				m_bValidate = FALSE;
				return FALSE;
			}

			m_pXml->GetChildElmtValue(_T("VERSION"), strValue);
			if (strValue != ECS_CLIENT_VERSION)
			{
				strLog.Format(_T("%s Version 불일치! [SERVER=%s, CLIENT=%s]"), GetClientName(), ECS_CLIENT_VERSION, strValue);
				m_pDoc->Alarm(LOG_POS_MONITOR, strLog);

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

//			BOOL bTemp = m_pDoc->m_pHostSv->IsConnect();

			m_pDoc->MulticastServerInfo(this, (m_pDoc->m_pHostSv && m_pDoc->m_pHostSv->IsConnect()) ? NOTIFY_SEND : NOTIFY_ERROR);
			m_pDoc->MulticastClientInfo(this, (m_pDoc->m_pHostCl && m_pDoc->m_pHostCl->IsConnect()) ? NOTIFY_SEND : NOTIFY_ERROR);
			m_pDoc->m_pJob->MulticastAllJobPerClient(this);
			m_pDoc->m_pEquipments.MulticastInfo(this);
		}
		else if (m_pXml->MoveXPath(_T("./CONNECT_STATUS"), FALSE))
		{
			m_pXml->GetChildElmtValue(_T("EQUIPMENT_KIND"), strValue);		int nEquipKind = _ttoi(strValue);
			m_pXml->GetChildElmtValue(_T("EQUIPMENT_NUM"), strValue);		int nEquipNum = _ttoi(strValue);

			CEquipment* pEquipment = m_pDoc->GetEquipment(nEquipKind, nEquipNum);
			if (pEquipment == NULL)
				return FALSE;

			pEquipment->MulticastInfo(this);
			
			strLog.Format(_T("%s CLIENT Multicast 요청 받음!"), GetClientName());
			m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseSys"));
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
			m_pXml->GetAttrValue(_T("NO"), strValue);		oJobItem.m_nLuggNum = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("JT"), strValue);		oJobItem.m_nJobType = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("SWH"), strValue);		oJobItem.m_nStartWH = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("SSTN"), strValue);		oJobItem.m_strStartStn = strValue;
			m_pXml->GetAttrValue(_T("SLOC"), strValue);		oJobItem.m_strStartLoc = strValue;
			m_pXml->GetAttrValue(_T("DWH"), strValue);		oJobItem.m_nDestWH = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("DSTN"), strValue);		oJobItem.m_strDestStn = strValue;
			m_pXml->GetAttrValue(_T("DLOC"), strValue);		oJobItem.m_strDestLoc = strValue;
			m_pXml->GetAttrValue(_T("RWH"), strValue);		oJobItem.m_nRouteWH = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("RSTN"), strValue);		oJobItem.m_strRouteStn = strValue;
			m_pXml->GetAttrValue(_T("RLOC"), strValue);		oJobItem.m_strRouteLoc = strValue;
			m_pXml->GetAttrValue(_T("GEN"), strValue);		oJobItem.m_ucGenCode = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("BCD"), strValue);		oJobItem.m_strBarcode = strValue;
			m_pXml->GetAttrValue(_T("PROD"), strValue);		oJobItem.m_strProductID = strValue;
			m_pXml->GetAttrValue(_T("PLT"), strValue);		oJobItem.m_strPalletNo = strValue;
			m_pXml->GetAttrValue(_T("WC"), strValue);		oJobItem.m_strWeight = strValue;
			m_pXml->GetAttrValue(_T("STS"), strValue);		oJobItem.m_nJobStatus = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("RES"), strValue);		oJobItem.m_nResultCode = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("DTR"), strValue);		oJobItem.m_nDepartTrackNum = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("ATR"), strValue);		oJobItem.m_nArriveTrackNum = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("ACT"), strValue);		oJobItem.m_nActivity = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("VID"), strValue);		oJobItem.m_nVehicleID = _ttoi(strValue);
			m_pXml->GetAttrValue(_T("RN"), strValue);		oJobItem.m_nRgvcNum = _ttoi(strValue);	
			m_pXml->GetAttrValue(_T("TIME"), strValue);		oJobItem.m_tTime = CTime((time_t)_ttoi(strValue));

			strLog.Format(_T("%s 작업추가 요청 [%s]"), GetClientName(), oJobItem.GetLogString());
 			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			if ((pJobItem = m_pDoc->m_pJob->Add(oJobItem)) == NULL)
			{
				strLog.Format(_T("%s 작업추가 실패 [%s]"), GetClientName(), oJobItem.GetLogString());
 				m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem, TRUE);
				return FALSE;
			}

			m_pDoc->m_pJob->Invoke(pJobItem);
		}
		else if (m_pXml->MoveXPath(_T("./EDIT/ITEM"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 수정할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->Alarm(LOG_POS_MONITOR, strLog);

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			m_pXml->GetAttrValue(_T("STS"), strValue);
			int nJobStatus = _ttoi(strValue);

			strLog.Format(_T("%s 작업수정 요청 [%s -> %s]"), GetClientName(), pJobItem->GetJobStatusString(), CJobItem::GetJobStatusString(nJobStatus));
 			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			pJobItem->SetJobStatus(nJobStatus);
			m_pDoc->m_pJob->Backup();
		}
		else if (m_pXml->MoveXPath(_T("./REMOVE/ITEM"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 삭제할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->Alarm(LOG_POS_MONITOR, strLog);

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

			if ((pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 재전송할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->Alarm(LOG_POS_MONITOR, strLog);

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

			if ((pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 완료보고할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->Alarm(LOG_POS_MONITOR, strLog);

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			strLog.Format(_T("%s 작업완료 요청 [%s]"), GetClientName(), pJobItem->GetJobStatusString());
			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			m_pXml->GetAttrValue(_T("TYPE"), strValue);
			m_pDoc->m_pJob->Complete(pJobItem, (BYTE)_ttoi(strValue), TRUE);
			m_pDoc->m_pJob->Backup();
		}
		else if (m_pXml->MoveXPath(_T("./ARRIVE"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 도착보고할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->Alarm(LOG_POS_MONITOR, strLog);

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			strLog.Format(_T("%s 도착완료 요청 [%s]"), GetClientName(), pJobItem->GetJobStatusString());
			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			m_pDoc->m_pJob->Arrive(pJobItem, TRUE);
			m_pDoc->m_pJob->Backup();
		}
		else if (m_pXml->MoveXPath(_T("./CANCEL"), FALSE))
		{
			m_pXml->GetAttrValue(_T("NO"), strValue);

			if ((pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValue))) == NULL)
			{
				strLog.Format(_T("%s 최소보고할 작업정보가 존재하지 않습니다. [작업번호=%s]"), GetClientName(), strValue);
				m_pDoc->StatusDisplay(strLog);

				m_pDoc->m_pJob->MulticastAllJobPerClient(this);
				return FALSE;
			}

			strLog.Format(_T("%s 작업취소 요청 [%s]"), GetClientName(), pJobItem->GetJobStatusString());
			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseJob"), pJobItem);

			m_pDoc->m_pJob->Cancel(pJobItem, TRUE);
			m_pDoc->m_pJob->Backup();
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
			m_pDoc->Alarm(LOG_POS_MONITOR, strLog);
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
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_MONITOR, strLog, _T("CMonitorServer::ParseEquip"), NULL, TRUE);
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
				pCmdMsg->m_nCommand = _ttoi(strValue);

				m_pXml->GetAttrValue(_T("SUB"), strValue);
				pCmdMsg->m_nSubCmd = _ttoi(strValue);

				pCmdMsg->m_strValues.SetSize(m_pXml->GetChildElmtCount());
				for (int j=0; j<pCmdMsg->m_strValues.GetSize(); ++j)
					m_pXml->GetChildElmtValue(j, pCmdMsg->m_strValues[j]);

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