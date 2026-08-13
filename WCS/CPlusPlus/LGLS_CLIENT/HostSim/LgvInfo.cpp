// LgvInfo.cpp: implementation of the CLgvInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "LgvInfo.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLgvInfo, CInfo, 0)

CLgvInfo::CLgvInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
	m_pControl = NULL;
	m_pCvInfo = NULL;

	m_wErrorCode = 0;
	m_bSuspend = FALSE;
	m_bStop = FALSE;
	m_bStopRequest = FALSE;
	m_nRequestLuggNum = 0;
	m_bInvoke = FALSE;
}

CLgvInfo::~CLgvInfo()
{
	int i=0;
	int nHsNum = m_pDepartsHS.GetSize();
	for(i = 0 ; i < nHsNum ; i++)
	{
		if (m_pDepartsHS[i] != NULL)
			delete m_pDepartsHS[i];
	}

	nHsNum = m_pArrivesHS.GetSize();
	for(i = 0 ; i < nHsNum ; i++)
	{
		if (m_pArrivesHS[i] != NULL)
			delete m_pArrivesHS[i];
	}

	m_pDepartsHS.RemoveAll();
	m_pArrivesHS.RemoveAll();
	
	m_pTrackHsArray.ClearArray();
}

void CLgvInfo::Initialize()
{
	m_wErrorCode = 0xFFFF;
	m_bModified = TRUE;
	m_bStop = FALSE;
	m_bStopRequest = FALSE;
	m_nRequestLuggNum = 0;
}

void CLgvInfo::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
		ar << m_bSuspend << m_bStop << m_bStopRequest << m_nRequestLuggNum;
	else
		ar >> m_bSuspend >> m_bStop >> m_bStopRequest >> m_nRequestLuggNum;
	
//	m_oLgvJobList.Serialize(ar);

	for (int i=0;i<m_oLgvVehicles.GetSize();i++) 
	{
		CLgvVehicle* pVehicle = m_oLgvVehicles.GetAt(i);
		DEBUGER_ASSERT_VALID(pVehicle != NULL);

		pVehicle->Serialize(ar);
	}
}

COLORREF CLgvInfo::GetColor(int nCarNum)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
//	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pConfig == NULL)
		return LIGHT_GRAY;

	if (nCarNum == 0)
	{
		if (m_wErrorCode == 0xFFFF)
			return pConfig->m_clrDisconnect;

		// LgvCar 중에 에러상태인것을 찾아야 하리라.
//		if (m_oLgvJobList.IsErrorStatus())
//			return pConfig->m_clrError;

		if (m_bSuspend)
			return pConfig->m_clrSuspend;

		if (m_bStop)
			return pConfig->m_clrAlarm;

		// Lgv 중에 작업지시 된 부분을 찾아야 하리라.
		if (m_bInvoke == TRUE)
			return pConfig->m_clrManual;
	}
	else
	{
		CLgvVehicle* pRgv = (CLgvVehicle*)GetLgvVehicle(nCarNum);
//		DEBUGER_ASSERT_VALID(pRgv != NULL);

		if (pRgv == NULL)
			return DARK_GRAY;

		if (pRgv->IsErrorStatus())
			return pConfig->m_clrError;

		if (pRgv->IsAutoMode() == FALSE)
			return DARK_GRAY;
		
		if (pRgv->IsInvoked())
		{
//			return pConfig->m_clrAutoSto;
			CJobItem* pJobItem = (CJobItem*) m_pEquipment->m_pDoc->m_pJob->Find(pRgv->m_nLuggNum);
//			DEBUGER_ASSERT_VALID(pJobItem != NULL);

			if (pJobItem != NULL)
			{
				switch(pJobItem->m_nActivity) 
				{
	//			case CLgvJobItem::enActivityQueue:			break;	// return pConfig->m_clrAutoSto;
				case CJobItem::enActivityVehicleAssigne:	return pConfig->m_clrAutoSto;
				case CJobItem::enActivityPickComplete:		return pConfig->m_clrAutoRet;
				case CJobItem::enActivityDropComplete:		return LIGHT_GRAY;
				}
			}
		}
	}
	return LIGHT_GRAY;	
}

void CLgvInfo::InvokeControl(int nCarNum)
{
	if (m_bModified == FALSE)
		return;

	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;

	if (pConfig == NULL)
		return;

	if (nCarNum != 0)
	{
		CLgvVehicle* pRgv = (CLgvVehicle*)GetLgvVehicle(nCarNum);
//		DEBUGER_ASSERT_VALID(pRgv != NULL);

		if (pRgv == NULL)
			return;
		
		int nMaxPosition = -1;
		if (m_pEquipment->m_nNumber == 1)
		{
			nMaxPosition = 35;
		}
		else if (m_pEquipment->m_nNumber == 2)
		{
			nMaxPosition = 34;			
		}

		if (nMaxPosition > 0 && pRgv->m_nCurPosition > nMaxPosition)
			return;

		//pRgv->m_nCurPosition = 5;  //이해담 test용 지워야함
		CDciStaticCtrl* pRgvPosCtrl = (CDciStaticCtrl*)m_pEquipment->m_pDoc->m_layout.GetRgvPosition(m_pEquipment->m_nNumber, pRgv->m_nCurPosition);
//		DEBUGER_ASSERT_VALID(pRgvPosCtrl != NULL);

		if (pRgvPosCtrl == NULL)
			return;

		pRgv->m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, TRUE);

		pRgv->m_pControl->m_rcControlL.CopyRect(pRgvPosCtrl->m_rcControlL);

		pRgv->m_bInvoke = TRUE;

		pRgv->m_pControl->m_clrFork = GetColor(pRgv->m_nVehicleID);
		
		//이해담 rgv
		//if(m_bSuspend ==TRUE) //test 용
//		if (pRgv->m_pControl->m_clrFork == pConfig->m_clrAutoSto || 
//		    pRgv->m_pControl->m_clrFork == pConfig->m_clrAutoRet ||
//		    ((pRgv->IsErrorStatus() == pConfig->m_clrError || pRgv->IsAutoMode() == FALSE) && pRgv->m_nLuggNum != 0))
//		{
//			pRgv->m_pControl->m_nFontSize = 9;
//			pRgv->m_pControl->m_strText.Format(_T("%d"), pRgv->m_nLuggNum);
//		}
//		else
//		{
			pRgv->m_pControl->m_nFontSize = m_pEquipment->m_pDoc->m_pConfig->m_nFontSize;
			pRgv->m_pControl->m_strText.Format(_T("%d"), _ttoi(pRgv->m_pControl->m_strCID.Right(2)));
//		}
		
		pRgv->m_pControl->m_nProd = pRgv->m_ucLoadStatus;
		
		pRgv->m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
		MulticastVehicle(pRgv);		
	}
	else
	{
		m_pControl->m_clrBgColor = GetColor();
		m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
		MulticastVehicleAll(NULL);	
	}

	m_pEquipment->RefreshDialog();
	MulticastCommStatus(NULL);		//@.@
//	MulticastVehicleAll(NULL);	
	m_bModified = FALSE;
}

int CLgvInfo::ConvertLgvPosToTrackNum(int nLgvPos)
{
	CTrackHS* pTrackHS = NULL;

	if (pTrackHS = m_pDepartsHS.FindByPos(nLgvPos))
		return pTrackHS->m_pTrack->m_nNumber;

	if (pTrackHS = m_pArrivesHS.FindByPos(nLgvPos))
		return pTrackHS->m_pTrack->m_nNumber;

	return 0;
}

int CLgvInfo::ConvertTrackNumToLgvPos(int nTrackNum)
{
	CTrackHS* pTrackHS = NULL;

	if (pTrackHS = m_pDepartsHS.FindByTrack(nTrackNum))
		return pTrackHS->m_nPos;

	if (pTrackHS = m_pArrivesHS.FindByTrack(nTrackNum))
		return pTrackHS->m_nPos;

	return 0;
}

void CLgvInfo::MulticastVehicle(CLgvVehicle* pLgvVehicle)
{
	if (pLgvVehicle == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	CString strSend;
	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO>%s</INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_pEquipment->GetDeviceName(), pLgvVehicle->GetXmlString(), CMonitorServer::enETX
	);

//	CMonitorListener* pMonitor = m_pEquipment->m_pDoc->m_pMonitorListener;
//	if (pMonitor && pMonitor->GetClientCount())
//		pMonitor->SendToClients(strSend);
}

void CLgvInfo::MulticastVehicleAll(CMonitorServer* pMonitorSv)
{

	CString strSend;
	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO>%s</INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_pEquipment->GetDeviceName(), m_oLgvVehicles.GetXmlString(), CMonitorServer::enETX
	);
	
//	if (pMonitorSv)
//	{
//		pMonitorSv->SendData(strSend);
//	}
//	else
//	{
//		CMonitorListener* pMonitorAll = m_pEquipment->m_pDoc->m_pMonitorListener;
//		if (pMonitorAll && pMonitorAll->GetClientCount())
//			pMonitorAll->SendToClients(strSend);
//	}
}

void CLgvInfo::MulticastCommStatus(CMonitorServer* pMonitorSv)
{
	CString strSend;
	CString strSendMsg = (m_pEquipment->m_pThreadSocket && m_pEquipment->m_pThreadSocket->m_strSend != _T("")) ? m_pEquipment->m_pThreadSocket->m_strSend.Mid(1, m_pEquipment->m_pThreadSocket->m_strSend.GetLength()-2) : _T("");
	CString strRecvMsg = (m_pEquipment->m_pThreadSocket && m_pEquipment->m_pThreadSocket->m_strRecv != _T("")) ? m_pEquipment->m_pThreadSocket->m_strRecv.Mid(1, m_pEquipment->m_pThreadSocket->m_strRecv.GetLength()-2) : _T("");


	strSendMsg.Replace('<', '{');	strSendMsg.Replace('>', '}');
	strRecvMsg.Replace('<', '{');	strRecvMsg.Replace('>', '}');

	strSendMsg.Replace(_T("'"), _T("|"));
	strRecvMsg.Replace(_T("'"), _T("|"));

	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO><COMM SUS='%d' SV='%s' CL='%s' CV='%s' SSN='%s' RSN='%s' STOP='%d' INV='%d'/></INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, 
		m_pEquipment->GetDeviceName(), m_bSuspend,
		m_pEquipment->m_pListener ? _T("구동 (CLIENT 접속대기)") : _T(""), 
		m_pEquipment->m_pThreadSocket ? ((CInterfaceThreadSk*)m_pEquipment->m_pThreadSocket)->GetPeerInfo() : _T(""), 
		m_pCvInfo->m_pEquipment->IsConnect() ? _T("통신연결") : _T(""),
		strSendMsg,		// m_pEquipment->m_pThreadSocket ? m_pEquipment->m_pThreadSocket->m_strSend : _T(""),
		strRecvMsg,		// m_pEquipment->m_pThreadSocket ? m_pEquipment->m_pThreadSocket->m_strRecv : _T(""),
		m_bStop,
		m_bInvoke,
		CMonitorServer::enETX
	);

//	if (pMonitorSv)
//	{
//		pMonitorSv->SendData(strSend);
//	}
//	else
//	{
//		CMonitorListener* pMonitorAll = m_pEquipment->m_pDoc->m_pMonitorListener;
//		if (pMonitorAll && pMonitorAll->GetClientCount())
//			pMonitorAll->SendToClients(strSend);
//	}
}
//=====================================================================================================
//	// 경광등 관련
//=====================================================================================================	
void CLgvInfo::SetLampInfo(BOOL bOn)
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
	case 1:		nIndex = 6;		break;
	case 2:		nIndex = 7;		break;
	}

	CLib::SetBit(pCv->m_pInfo->m_wCurrLampStatus, nIndex, bOn);
}
//-----------------------------------------------------------------------------------------------------
/*
void CLgvInfo::MulticastDoor(CMonitorServer* pMonitorSv)
{
	CString strSend;
	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO><DOOR OREQ='%d' CREQ='%d'/></INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_pEquipment->GetDeviceName(), m_wDoorOpenRequest, m_wDoorCloseRequest, CMonitorServer::enETX
	);

	if (pMonitorSv)
	{
		pMonitorSv->SendData(strSend);
	}
	else
	{
		CMonitorListener* pMonitorAll = m_pEquipment->m_pDoc->m_pMonitorListener;
		if (pMonitorAll && pMonitorAll->GetClientCount())
			pMonitorAll->SendToClients(strSend);
	}
}
void CLgvInfo::MulticastAddJob(CLgvJobItem* pLgvJobItem)
{
	if (pLgvJobItem == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	CString strSend;
	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO><ADD>%s</ADD></INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_pEquipment->GetDeviceName(), pLgvJobItem->GetXmlString(), CMonitorServer::enETX
	);

	CMonitorListener* pMonitor = m_pEquipment->m_pDoc->m_pMonitorListener;
	if (pMonitor && pMonitor->GetClientCount())
		pMonitor->SendToClients(strSend);
}

void CLgvInfo::MulticastEditJob(CLgvJobItem* pLgvJobItem)
{
	if (pLgvJobItem == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	CString strSend;
	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO><EDIT>%s</EDIT></INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_pEquipment->GetDeviceName(), pLgvJobItem->GetXmlString(), CMonitorServer::enETX
	);

	CMonitorListener* pMonitor = m_pEquipment->m_pDoc->m_pMonitorListener;
	if (pMonitor && pMonitor->GetClientCount())
		pMonitor->SendToClients(strSend);
}

void CLgvInfo::MulticastRemoveJob(CLgvJobItem* pLgvJobItem)
{
	if (pLgvJobItem == NULL)
	{
//		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	CString strSend;
	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO><REMOVE>%s</REMOVE></INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_pEquipment->GetDeviceName(), pLgvJobItem->GetXmlString(), CMonitorServer::enETX
	);

	CMonitorListener* pMonitor = m_pEquipment->m_pDoc->m_pMonitorListener;
	if (pMonitor && pMonitor->GetClientCount())
		pMonitor->SendToClients(strSend);
}

void CLgvInfo::MulticastAddAllJob(CMonitorServer* pMonitorSv)
{
	if (pMonitorSv == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	CString strSend;
	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO><REMOVEALL/></INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_pEquipment->GetDeviceName(), CMonitorServer::enETX
	);
	pMonitorSv->SendData(strSend);

	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'><INFO><ADD>%s</ADD></INFO></EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_pEquipment->GetDeviceName(), m_oLgvJobList.GetXmlString(), CMonitorServer::enETX
	);
	pMonitorSv->SendData(strSend);
}

*/
