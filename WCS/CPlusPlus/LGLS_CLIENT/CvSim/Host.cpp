// Host.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"
#include "Host.h"

#include "EcsDoc.h"
#include "MainFrm.h"
#include "EcsView.h"
#include "Lib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CListenSk, CAsyncSocketEx)
IMPLEMENT_DYNAMIC(CHostSv, CAsyncSocketEx)
IMPLEMENT_DYNAMIC(CHostCl, CAsyncSocketEx)

//////////////////////////////////////
// CListenSk

CListenSk::CListenSk()
{
}

CListenSk::CListenSk(CEcsDoc * pDoc)
{
	m_pDoc = pDoc;
	m_bConnected = FALSE;
	m_bInkJetConnected = FALSE;
}

//CListenSk::CListenSk(LPCTSTR lpszName, BOOL bWriteLog)
//{
//	m_strName = lpszName;
//	m_bWriteLog = bWriteLog;
//	m_enCommStatus = enStatusClose;
//}

CListenSk::~CListenSk()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSk, CAsyncSocketEx)
//{{AFX_MSG_MAP(CListenSk)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////
// CListenSk member functions

#define		LOG_ERROR		m_pDoc->m_pLog->Error
#define		LOG_EVENT		m_pDoc->m_pLog->Event
#define     LOG_JOB			m_pDoc->m_pLog->Job
#define		LOG_DEBUG		m_pDoc->m_pLog->Debug
#define     JOB				m_pDoc->m_pJob

void CListenSk::OnAccept(int nErrorCode) 
{
	CString strLocalAddr;
	UINT    nLocalPort;
	
	if (GetSockName(strLocalAddr, nLocalPort))
	{
		if (m_pDoc->m_pConfig->m_nEcsPort == (int)nLocalPort)
		{
			if (m_bConnected) 
			{
				m_pDoc->m_pHostSv->ShutDown();
				delete m_pDoc->m_pHostSv;
				m_pDoc->m_pHostSv = NULL;
				
				m_pDoc->m_pHostSv = new CHostSv(m_pDoc);
				m_pDoc->m_pHostSv->UpdateCommSv(NOTIFY_RECEIVE);
				
				LOG_EVENT(LOG_POS_SYSTEM, LOG_SYSTEM, "FMS 재연결 성공");
				m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectOK;
			}
			else 
			{
				m_pDoc->m_pHostSv = new CHostSv(m_pDoc);
				m_pDoc->m_pHostSv->UpdateCommSv(NOTIFY_RECEIVE);
				m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectFail;
			}
			
			if (!Accept(*m_pDoc->m_pHostSv))
			{
				m_pDoc->m_pHostSv->ShutDown();
				delete m_pDoc->m_pHostSv;
				m_pDoc->m_pHostSv = NULL;
				
				m_bConnected = FALSE;
				m_pDoc->m_pHostSv->UpdateCommSv(NOTIFY_ERROR);
				m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectFail;
			}
			else
			{
				m_bConnected = TRUE;
				m_pDoc->m_pHostSv->UpdateCommSv(NOTIFY_RECEIVE);
				m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectOK;
				m_pDoc->m_dtHeartBit = COleDateTime::GetCurrentTime();
				LOG_EVENT(LOG_POS_SYSTEM, LOG_SYSTEM, "FMS 연결 성공");
			}
		}
	}
	else
	{
		CString strLog;
		strLog.Format("[CListenSk::OnAccept] FMS 연결 실패! ( %s )", CLib::GetSystemErrMsg());
		m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectFail;
		LOG_ERROR(LOG_POS_SYSTEM, LOG_SYSTEM, strLog);
	}
	
	CAsyncSocketEx::OnAccept(nErrorCode);
}

/////////////////////////////////
// CHostSv

CHostSv::CHostSv(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	m_nHostSeqNum = 0;
//	m_bArriveReport = FALSE;
//	m_saStation.RemoveAll();
//	m_saBcr.RemoveAll();
}

//CHostSv::CHostSv(LPCTSTR lpszName, BOOL bWriteLog)
//{
//	m_strName = lpszName;
//	m_bWriteLog = bWriteLog;
//	m_enCommStatus = enStatusClose;
//}

CHostSv::~CHostSv()
{
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CHostSv, CAsyncSocketEx)
//{{AFX_MSG_MAP(CHostSv)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////
// CHostSv member functions

void CHostSv::OnClose(int nErrorCode) 
{
	UpdateCommSv(NOTIFY_ERROR);
	
	CString strLog;
	strLog.Format("[CHostSv::OnClose] FMS 연결 해제 ( %s )", CLib::GetSystemErrMsg(nErrorCode));
	LOG_EVENT(LOG_POS_SYSTEM, LOG_SYSTEM, strLog);

	m_pDoc->m_pHostListener->m_bConnected = FALSE;
	m_pDoc->m_pHostSv = NULL;
	
	ShutDown();
	delete this;
	
	CAsyncSocketEx::OnClose(nErrorCode);
}

void CHostSv::OnReceive(int nErrorCode) 
{
	CAsyncSocketEx::OnReceive(nErrorCode);
	
	if (nErrorCode)
		AfxMessageBox("The Windows Sockets implementation detected that the network subsystem failed.");
	
	int nMsgLen = 0;
	char RxBuff[4096];
	CString strLog;
	
	DWORD dwLen = 0;
	DWORD nRealLen = 0;
	
	if (IOCtl(FIONREAD, &dwLen) == FALSE)
	{
		strLog.Format("[CHostSv::OnReceive] FMS Message Receive Error! ( %s )", CLib::GetSystemErrMsg());
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
		return;
	}
	
	memset(RxBuff, 0x00, sizeof(RxBuff));
	if ((nRealLen = Receive(RxBuff, dwLen)) != dwLen)
	{
		strLog.Format("[CHostSv::OnReceive] FMS Message Length Error! (Len=%d, RealLen=%d, Msg=%s)", dwLen, nRealLen, RxBuff);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
		return;
	}
	strLog = RxBuff;
//	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::OnReceive"));

	if (RxBuff[0] != '@')
	{
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, "[CHostSv::OnReceive] FMS Message Start Error!");
		return;
	}
	
	int nFrameLen = dwLen -1;
	if (RxBuff[nFrameLen - 1] != '*' || RxBuff[nFrameLen] != ';')
	{
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, "[CHostSv::OnReceive] FMS Message End Error!");
		return;
	}

	// HeartBit 시간설정
	m_pDoc->m_dtHeartBit = COleDateTime::GetCurrentTime();
	m_pDoc->m_bHeartBit = FALSE;

	TokenRxFrame(RxBuff);
	UpdateCommSv(NOTIFY_RECEIVE);
}

void CHostSv::WriteData()
{
//	if (m_bArriveReport == FALSE)
//	{
//		CString strStation = m_saStation.GetAt(0);
//		CString strBcr = m_saBcr.GetAt(0);
//		if (BarcodeData(strStation, strBcr) == TRUE)
//		{
//			m_saStation.RemoveAt(0);
//			m_saBcr.RemoveAt(0);
//		}
//	}
}

void CHostSv::TokenRxFrame(char *pFrame)
{
	CString strRecv, strRxBuff;
	strRecv = pFrame;
	
	for (int i = 0; i < strRecv.GetLength(); ++i)
	{
		if (strRecv[i] == '@')
		{
			strRxBuff = "";
			strRxBuff += strRecv[i];
		}
		else if (strRecv[i] == ';')
		{
			strRxBuff += strRecv[i];
			Parsing(strRxBuff);
			strRxBuff = "";
		}
		else
		{
			strRxBuff += strRecv[i];
		}
	}
} 

void CHostSv::Parsing(CString pFrame)
{
	CString strLog, str;
	int nResultCode = 0;
	CString strFrame = pFrame;
	CString strCmd = strFrame.Mid(21, 4);

	// HeartBit 통지/응답
	if (strCmd == _T("0001"))
	{
		nResultCode = atoi(strFrame.Mid(39, 4));

		if (nResultCode == 0)
		{
//			strLog.Format(_T("[RECV HeartBit] %s"), strFrame);
//			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
			strLog.Format(_T("FMS HeartBit 통지 수신"));
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

			char TxBuff[MSG_LENGTH_HEADER + 1];
			memset(TxBuff, 0x0, sizeof(TxBuff));
			memcpy(TxBuff, pFrame, sizeof(TxBuff));
			strncpy(TxBuff + 5, "eECS3F01", 8);
			strncpy(TxBuff + 13, "IMS     ", 8);
			strncpy(TxBuff + 21, "1001", 4);
			strncpy(TxBuff + 39, "   1", 4);

//			strLog.Format(_T("[SEND HeartBit] %s"), TxBuff);
//			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

			if (SendMsg(TxBuff, MSG_LENGTH_HEADER) == TRUE)
			{
				strLog.Format(_T("FMS HeartBit 응답 송신"));
				m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
			}
			else
			{
				strLog.Format(_T("[CHostSv::Parsing] FMS HeartBit 응답 송신 실패!"));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
				return;
			}
		}
		else
		{
//			strLog.Format(_T("[RECV HeartBit] %s"), strFrame);
//			m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
			strLog.Format(_T("FMS HeartBit 응답 수신"));
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
		}
	}
	// 시간 설정 요구
	else if (strCmd == _T("0002"))
	{
//		strLog.Format(_T("[RECV Time] %s"), strFrame);
//		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
		strLog.Format(_T("FMS 시간 설정 요구 수신"));
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		// 시스템 시간 설정
		CString strDate = strFrame.Mid(25, 14);

		char TxBuff[MSG_LENGTH_HEADER + 1];
		memset(TxBuff, 0x0, sizeof(TxBuff));
		memcpy(TxBuff, pFrame, sizeof(TxBuff));
		strncpy(TxBuff + 5, "eECS3F01", 8);
		strncpy(TxBuff + 13, "IMS     ", 8);
		strncpy(TxBuff + 21, "1002", 4);

//		strLog.Format(_T("[SEND Time] %s"), TxBuff);
//		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		if (SendMsg(TxBuff, MSG_LENGTH_HEADER) == TRUE)
		{
			strLog.Format(_T("FMS 시간 설정 응답 송신"));
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
		}
		else
		{
			strLog.Format(_T("[CHostSv::Parsing] FMS 시간 설정 응답 송신 실패!"));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
			return;
		}

		COleDateTime odt;
		SYSTEMTIME st;
		odt.SetDateTime(atoi(strFrame.Mid(25, 4)), atoi(strFrame.Mid(29, 2)), atoi(strFrame.Mid(31, 2)), 
						atoi(strFrame.Mid(33, 2)), atoi(strFrame.Mid(35, 2)), atoi(strFrame.Mid(37, 2)));
		odt.GetAsSystemTime(st);
		::SetLocalTime(&st);
	}
	// 도착보고 응답
	else if (strCmd == _T("0501"))
	{
//		strLog.Format(_T("[RECV Arrive] %s"), strFrame);
//		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		CString strTray = strFrame.Mid(54, 6);
		CJobItem* pJobItem = m_pDoc->m_pJob->Find(strTray);
		nResultCode = atoi(strFrame.Mid(39, 4));

//		if (pJobItem == NULL)
//		{
//			strLog = _T("도착보고 응답 이상 - 작업정보가 존재하지 않음");
//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, atoi(strTray.Right(5)), strLog, _T("CHostSv::Parsing"));
//		}
		
		if (nResultCode != enHostErrorNone)
		{
			if (pJobItem != NULL)
			{
				if ((pJobItem->m_strDestStn == _T("651") && nResultCode == enHostError9026) ||
					(pJobItem->m_strDestStn == _T("648") && nResultCode == enHostError9026) ||
					(pJobItem->m_strDestStn == _T("618") && nResultCode == enHostError9022) ||
					(pJobItem->m_strDestStn == _T("605") && nResultCode == enHostError9022))
				{
					strLog.Format(_T("FMS 도착보고 응답 이상인데 정상! 도착지(%s) ( %d=%s )"), pJobItem->m_strDestStn, nResultCode, CLib::GetHostResultSting(nResultCode));
					m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), pJobItem, strTray);
					m_pDoc->m_pJob->Remove(pJobItem);
					return;
				}
				else
				{
					strLog.Format(_T("FMS 도착보고 응답 이상! 도착지(%s) ( %d=%s )"), pJobItem->m_strDestStn, nResultCode, CLib::GetHostResultSting(nResultCode));
					m_pDoc->Alarm(LOG_POS_HOST, strLog, pJobItem);
					m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), pJobItem, strTray);
					m_pDoc->m_pJob->Remove(pJobItem);
					return;
				}
			}
			else
			{
				strLog.Format(_T("FMS 도착보고 응답 이상! 도착지(???) ( %d=%s )"), nResultCode, CLib::GetHostResultSting(nResultCode));
				m_pDoc->Alarm(LOG_POS_HOST, strLog, pJobItem, strTray);
				m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), pJobItem, strTray);
				return;
			}
		}
		
		strLog = _T("FMS 도착보고 응답 수신");
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), pJobItem, strTray);
		
		if (pJobItem != NULL && (pJobItem->m_nJobStatus == enJobStatusArrived || pJobItem->m_nJobStatus == enJobStatusCvInvoke || 
			pJobItem->m_nJobStatus == enJobStatusError))
		{
// Update : 2014.06.12 - Q >> Aging 입고 HS에 도착한 Tray의 작업정보를 Crane이 작업을 진행한 후 삭제할 수 있도록 요청
//						 A >> 도착보고 응답을 받은 후 삭제하지 않고 Crane이 Tray를 떠간 후 삭제
			if (pJobItem->m_strDestStn == _T("101") || pJobItem->m_strDestStn == _T("102") || pJobItem->m_strDestStn == _T("103") ||
				pJobItem->m_strDestStn == _T("104") || pJobItem->m_strDestStn == _T("105") || pJobItem->m_strDestStn == _T("106") ||
				pJobItem->m_strDestStn == _T("107"))
				return;
//-----------------------------------------------------------------------------------------------------------------------------------

			m_pDoc->m_pJob->Remove(pJobItem);
		}
	}
	// 반송지시
	else if (strCmd == _T("0502"))
	{
		CString strTray = strFrame.Mid(71, 6);
		strLog.Format(_T("[RECV Job] %s"), strFrame);
		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), NULL, strTray);

		nResultCode = atoi(strFrame.Mid(39, 4));

		if ((nResultCode != enHostErrorNone))
		{
			strLog.Format(_T("FMS 반송지시 이상! ( %d=%s )"), nResultCode, CLib::GetHostResultSting(nResultCode));
			m_pDoc->Alarm(LOG_POS_HOST, strLog, NULL, strTray);
			m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), NULL, strTray);
			return;
		}

		CJobItem oJobItem(m_pDoc);
		oJobItem.m_nLuggNum		= m_pDoc->m_pJob->GenerateOnlineLuggNum();
		oJobItem.m_nJobType		= atoi(strFrame.Mid(50, 1));
		oJobItem.m_nStartWH		= '1';
		oJobItem.m_nDestWH		= '1';
		oJobItem.m_strStartStn	= strFrame.Mid(51, 3);
		if (strFrame.Mid(57, 1) != _T(" "))
			str.Format(_T("%02s%03s%02s"), strFrame.Mid(57, 1), strFrame.Mid(60, 2), strFrame.Mid(62, 2));
		else str = _T("");
		oJobItem.m_strStartLoc	= str;
		oJobItem.m_strDestStn	= strFrame.Mid(54, 3);
		if (strFrame.Mid(64, 1) != _T(" "))
			str.Format(_T("%02s%03s%02s"), strFrame.Mid(64, 1), strFrame.Mid(67, 2), strFrame.Mid(69, 2));
		else str = _T("");
		oJobItem.m_strDestLoc	= str;
		str = strFrame.Mid(98, 1);
		oJobItem.m_nSize		= (int)str[0];	// Tray Type
		// TRAY ID가 "M"으로 시작하는 Tray는 enTrayTypeZ Type으로 변경
		if (strFrame.Mid(71, 1) == "M")	oJobItem.m_nSize = 90;
		oJobItem.m_strBarcode = oJobItem.m_strProductID = strFrame.Mid(71, 6);
// Update : 2014.05 - 중앙 설비 IR/OCV의 진입불가 및 병목현상을 판단하여 목적지 분배 ("M" Tray 제외)
		if (oJobItem.m_strStartStn == _T("634") && oJobItem.m_nSize != enTrayTypeZ)
		{
			int n541 = 99, n542 = 99;
			m_pDoc->m_mapCEquipStatus.Lookup(541, n541);
			m_pDoc->m_mapCEquipStatus.Lookup(542, n542);
			// #541 진입불가시 #542로 투입
			if (!(n541 == 1 || n541 == 2 || n541 == 3) && (n542 == 1 || n542 == 2 || n542 == 3))
				oJobItem.m_strDestStn = _T("635");
			// #542 진입불가시 #541로 투입
			else if ((n541 == 1 || n541 == 2 || n541 == 3) && !(n542 == 1 || n542 == 2 || n542 == 3))
				oJobItem.m_strDestStn = _T("541");
			// 병목현상을 판단하여 목적지 분배
			else
			{
				CTrackInfo* pTrack = NULL;
				CTrackInfo* pTrack1 = NULL;
				int nCount = 0;

				pTrack1 = m_pDoc->GetTrackInfo(312, 0);
				for (int n = 313; n <= 315; n++)
				{
					pTrack = m_pDoc->GetTrackInfo(n, 0);
					if (pTrack->IsProductSensing() && pTrack->m_nDestPos > 0)	++nCount;
				}
				if (pTrack1->IsProductSensing() && pTrack1->m_nDestPos > 0 && nCount < 3)
					oJobItem.m_strDestStn = _T("542");
				else if (nCount > 1 && pTrack1->IsProductSensing() == FALSE && pTrack1->m_nDestPos == 0)
					oJobItem.m_strDestStn = _T("541");
			}
		}
//-------------------------------------------------------------------------------------------------------
		strLog = _T("FMS 반송지시 수신");
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), &oJobItem);

//		unsigned char ucMsgType='C';
		
//		if (oJobItem.IsOnlineLuggNum() == FALSE)
//		{
//			strLog.Format(_T("작업번호 이상! [%d]"), oJobItem.m_nLuggNum);
//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
//			Answer(ucMsgType, oJobItem.m_nLuggNum, oJobItem.m_nJobType, enHostErrorInternal);
//			return;
//		}
		
//		if (oJobItem.IsOnlineJobType() == FALSE)
//		{
//			strLog.Format(_T("작업구분 이상! [%d]"), oJobItem.m_nJobType);
//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
//			Answer(ucMsgType, oJobItem.m_nLuggNum, enJobTypeNone, enHostErrorUnknownJobType);
//			return;
//		}
		
//		if (oJobItem.IsValidStartStation() == FALSE)
//		{
//			strLog.Format(_T("출발지 이상! [%s]"), oJobItem.m_strStartStn);
//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
//			Answer(ucMsgType, oJobItem.m_nLuggNum, oJobItem.m_nJobType, enHostErrorInvalidStartStation);
//			return;
//		}
		
//		if (oJobItem.IsValidStartLocation() == FALSE)
//		{
//			strLog.Format(_T("출발LOCATION 이상! [%s]"), oJobItem.m_strStartLoc);
//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
//			Answer(enHostCommandOrder, oJobItem.m_nLuggNum, oJobItem.m_nJobType, enHostErrorInvalidStartLocation);
//			return;
//		}
		
//		if (oJobItem.IsValidDestStation() == FALSE)
//		{
//			strLog.Format(_T("도착지 이상! [%s]"), oJobItem.m_strDestStn);
//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
//			Answer(ucMsgType, oJobItem.m_nLuggNum, oJobItem.m_nJobType, enHostErrorInvalidDestStation);
//			return;
//		}
		
//		if (oJobItem.IsValidDestLocation() == FALSE)
//		{
//			strLog.Format(_T("도착LOCATION 이상! [%s]"), oJobItem.m_strDestLoc);
//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
//			Answer(enHostCommandOrder, oJobItem.m_nLuggNum, oJobItem.m_nJobType, enHostErrorInvalidDestLocation);
//			return;
//		}

//		CJobItem* pJobItem = m_pDoc->m_pJob->Find(oJobItem.m_strBarcode);
//		if (pJobItem != NULL)	m_pDoc->m_pJob->Remove(pJobItem);

		JobOrderDirective(oJobItem);
		
		/*
		//	int nJobRouting;																	// 사용안함???
		switch (ucMsgType)
		{
		case enHostCommandOrder:
			oJobItem.m_nSeqNo		= atoi(	strFrame.Mid(30, 4));
			oJobItem.m_strPalletNo	= strFrame.Mid(34, 7);
			oJobItem.m_nPriority	= atoi(	strFrame.Mid(41, 3));
			
			oJobItem.m_strBarcode	= oJobItem.m_strPalletNo	= strFrame.Mid(45, 20);		// ??
			
			JobOrderDirective(oJobItem);
			break;
			
		case enHostCommandRedirect:
			//		nJobRouting				= atoi(	strFrame.Mid(44, 1));							// 사용안함???
			RedirectLocDirective(oJobItem);
			break;
			
		case enHostCommandResendOrder:
			ResendOrderDirective(oJobItem);
			break;
			
		default:
			strLog.Format(_T("정의되지 않은 COMMAND! [CMD=%s]"), ucMsgType);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
			Answer(enHostCommandUnpredict, oJobItem.m_nLuggNum, oJobItem.m_nJobType, enHostErrorUndefinedCommandType);
		}
		*/

		char TxBuff[MSG_LENGTH_REQUEST_MSG + 1];
		memset(TxBuff, 0x0, sizeof(TxBuff));
		memcpy(TxBuff, pFrame, sizeof(TxBuff));
		strncpy(TxBuff + 5, "eECS3F01", 8);
		strncpy(TxBuff + 13, "IMS     ", 8);
		strncpy(TxBuff + 21, "1502", 4);

//		strLog.Format(_T("[SEND Job] %s"), TxBuff);
//		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		if (SendMsg(TxBuff, MSG_LENGTH_REQUEST_MSG) == TRUE)
		{
			strLog = _T("FMS 반송지시 응답 송신");
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), &oJobItem);
		}
		else
		{
			strLog = _T("[CHostSv::Parsing] FMS 반송지시 응답 송신 실패!");
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), &oJobItem);
			return;
		}
	}
	// 완료보고 응답
	else if (strCmd == _T("0503"))
	{
//		strLog.Format(_T("[RECV Complete] %s"), strFrame);
//		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		CString strTray = strFrame.Mid(71, 6);
		CJobItem* pJobItem = m_pDoc->m_pJob->Find(strTray);
		nResultCode = atoi(strFrame.Mid(39, 4));

//		if (pJobItem == NULL)
//		{
//			strLog.Format(_T("완료보고 응답 이상 [TrayID=%s - 작업정보 존재하지 않음]"), strTray);
//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), NULL);
//			return;
//		}
		
		if ((nResultCode != enHostErrorNone))
		{
			strLog.Format(_T("FMS 완료보고 응답 이상! ( %d=%s )"), nResultCode, CLib::GetHostResultSting(nResultCode));
			m_pDoc->Alarm(LOG_POS_HOST, strLog, pJobItem, strTray);
			m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), pJobItem, strTray);
			return;
		}
		
		strLog = _T("FMS 완료보고 응답 수신");
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), pJobItem, strTray);
		
		if (pJobItem != NULL && pJobItem->m_nJobStatus == enJobStatusCompleteRequest)
			m_pDoc->m_pJob->Remove(pJobItem);
	}
	// 화재발생 강제이동
	else if (strCmd == _T("0504"))
	{
//		strLog.Format(_T("[RECV Fire] %s"), strFrame);
//		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		nResultCode = atoi(strFrame.Mid(39, 4));
		int nStage = atoi(strFrame.Mid(53, 3));
		CString strStartLoc = (nStage == 301) ? "0100101" : CLib::GetLocation(nStage);

		if ((nResultCode != enHostErrorNone))
		{
			strLog.Format(_T("FMS 화재발생 강제이동 수신 이상! ( STAGE=%d, LOC=%s, %d=%s )"), 
				nStage, strStartLoc, nResultCode, CLib::GetHostResultSting(nResultCode));
//			m_pDoc->Alarm(LOG_POS_HOST, strLog);
			m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
			return;
		}
		
		int nErrorCode = atoi(strFrame.Mid(56, 2));
		if (nErrorCode == 22)	str = _T("Smoke Error");
		else if (nErrorCode == 23)	str = _T("Temperature Alarm");

		strLog.Format(_T("FMS 화재발생 강제이동! ( STAGE=%d, LOC=%s, %d=%s )"), nStage, strStartLoc, nErrorCode, str);
		m_pDoc->Alarm(LOG_POS_HOST, strLog);
		strLog.Format(_T("FMS 화재발생 강제이동 수신 ( STAGE=%d, LOC=%s, %d=%s )"), nStage, strStartLoc, nErrorCode, str);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		CJobItem oJobItem(m_pDoc);
		CJobItem* pJobItem = NULL;
		CStationInfo* pStartStation = m_pDoc->GetScStationInfo(1, strStartLoc);
		int nLuggNum = m_pDoc->m_pJob->GenerateOfflineLuggNum();

		oJobItem.m_nLuggNum		= (nLuggNum == 0) ? 9999 : nLuggNum;
		oJobItem.m_nJobType		= enJobTypeSemiRet;
		oJobItem.m_nStartWH		= 1;
		oJobItem.m_strStartStn	= pStartStation->m_strID;
		oJobItem.m_strStartLoc	= strStartLoc;
		oJobItem.m_nDestWH		= 1;
		oJobItem.m_strDestStn	= (nStage == 301 || nStage < 200) ? "633" : "648";
		oJobItem.m_nPriority	= 119;

		// TRACK#457 - 화재발생 알람 On 
		CTrackInfo* pTrack = m_pDoc->GetTrackInfo(457, 0);
		if (pTrack->m_pEquipment->IsConnect() == TRUE)
		{
			CCmdMsg* pCmdMsg = new CCmdMsg;
			pCmdMsg->m_nCommand = CCvCmdMsg::enCmdFireAlarm;
			pCmdMsg->m_strValues.SetSize(CCvCmdMsg::enSizeFireAlarm);
			pCmdMsg->m_strValues[0].Format(_T("%d"), 457);
			pCmdMsg->m_strValues[1].Format(_T("%d"), 1);
			pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);

			strLog = _T("화재발생 강제이동 자동 알람 ON");
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), &oJobItem);
		}

		if ((pJobItem = m_pDoc->m_pJob->Add(oJobItem)) == NULL)
		{
			strLog.Format(_T("[CHostSv::Parsing] 화재발생 강제이동 작업생성 실패! ( STAGE=%d, LOC=%s )"), nStage, strStartLoc);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), &oJobItem);
			return;
		}

		strLog.Format(_T("화재발생 강제이동 작업생성 ( STAGE=%d, LOC=%s )"), nStage, strStartLoc);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), pJobItem);

		if (m_pDoc->m_pJob->Invoke(pJobItem) == FALSE)
		{
			strLog.Format(_T("[CHostSv::Parsing] 화재발생 강제이동 작업지시 실패! ( STAGE=%d, LOC=%s )"), nStage, strStartLoc);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"), pJobItem);
			return;
		}
	}
	// 설비상태통지
	else if (strCmd == _T("0505"))
	{
//		strLog.Format(_T("[RECV Status] %s"), strFrame);
//		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		nResultCode = atoi(strFrame.Mid(39, 4));
		if (nResultCode != enHostErrorNone)
		{
			strLog.Format(_T("FMS 설비상태통지 수신 이상! ( %d=%s )"), nResultCode, CLib::GetHostResultSting(nResultCode));
//			m_pDoc->Alarm(LOG_POS_HOST, strLog);
			m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
			return;
		}
		strLog.Format(_T("FMS 설비상태통지 수신"));
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		// 화면 업데이트
		m_pDoc->InvokeCenterEquipCtrl(strFrame.Mid(50, 100));
		// Client 전송
		m_pDoc->m_strCEquipInfo.Format(_T("%c<ECS><EQUIP DEVICE='CENTER_EQUIP' VALUE='%s'></EQUIP></ECS>%c"), 
			CMonitorServer::enSTX, strFrame.Mid(50, 100), CMonitorServer::enETX);

		// 설비상태통지 응답
		char TxBuff[MSG_LENGTH_EQUIPSTATUS_MSG + 1];
		memset(TxBuff, 0x0, sizeof(TxBuff));
		memcpy(TxBuff, pFrame, sizeof(TxBuff));
		strncpy(TxBuff + 5, "eECS3F01", 8);
		strncpy(TxBuff + 13, "IMS     ", 8);
		strncpy(TxBuff + 21, "1505", 4);

		strLog.Format(_T("[SEND Status] %s"), TxBuff);
		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));

		if (SendMsg(TxBuff, MSG_LENGTH_EQUIPSTATUS_MSG) == TRUE)
		{
			strLog.Format(_T("FMS 설비상태통지 응답 송신"));
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
		}
		else
		{
			strLog.Format(_T("[CHostSv::Parsing] FMS 설비상태통지 응답 송신 실패!"));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
			return;
		}
	}
}

void CHostSv::JobOrderDirective(CJobItem& rJobItem)
{
	CString strLog;
//	CJobItem* pJobItem = m_pDoc->m_pJob->Find(rJobItem.m_nLuggNum);
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(rJobItem.m_strBarcode);
	if (pJobItem != NULL)
	{
		if (pJobItem->IsEqual(&rJobItem))
		{
			strLog.Format(_T("이미 동일한 반송지시의 작업이 진행중입니다. ( 이전작업번호=%d, 신규작업번호=%d, TRAY=%s )"), 
				pJobItem->m_nLuggNum, rJobItem.m_nLuggNum, rJobItem.m_strBarcode);
			m_pDoc->Alarm(LOG_POS_HOST, strLog, pJobItem);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::JobOrderDirective"), &rJobItem);
			return;
		}
		else
		{
			if (pJobItem->m_strDestStn == _T("520") || pJobItem->m_strDestStn == _T("521") || 
				pJobItem->m_strDestStn == _T("531") || pJobItem->m_strDestStn == _T("532") || pJobItem->m_strDestStn == _T("536") ||
				pJobItem->m_strDestStn == _T("541") || pJobItem->m_strDestStn == _T("542") ||
				pJobItem->m_strDestStn == _T("550") || pJobItem->m_strDestStn == _T("551") || pJobItem->m_strDestStn == _T("552") ||
				pJobItem->m_strDestStn == _T("561") || pJobItem->m_strDestStn == _T("562") ||
				pJobItem->m_strDestStn == _T("571") || pJobItem->m_strDestStn == _T("572") ||
				pJobItem->m_strDestStn == _T("573") || pJobItem->m_strDestStn == _T("574") ||
				pJobItem->m_strDestStn == _T("580") || pJobItem->m_strDestStn == _T("581") || pJobItem->m_strDestStn == _T("582"))
			{
				m_pDoc->m_pJob->Remove(pJobItem);
			}
			else
			{
				Sleep(300);
				pJobItem = m_pDoc->m_pJob->Find(rJobItem.m_strBarcode);
				if (pJobItem != NULL)
				{
					strLog.Format(_T("이미 동일한 Tray ID로 작업이 진행중입니다. ( 이전작업번호=%d, 신규작업번호=%d, TRAY=%s )"), 
						pJobItem->m_nLuggNum, rJobItem.m_nLuggNum, rJobItem.m_strBarcode);
//					m_pDoc->Alarm(LOG_POS_HOST, strLog, pJobItem);
					m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::JobOrderDirective"), &rJobItem);
// Update : 2014.06.21 - Tray 밀림현상 처리
//						 해당 시작위치에 이미 예약된 작업이 있을 경우 작업상태를 에러로 변경 후 계속 진행
					pJobItem->SetJobStatus(enJobStatusError);
				}
			}
//-------------------------------------------------------------------------------------------------------
		}
	}
	
	switch (rJobItem.m_nJobType)
	{
	case enJobTypeAutoSto:
	case enJobTypeAutoMove:
		{
			CStationInfo* pStation = m_pDoc->GetStationInfo(rJobItem.m_strStartStn);
			DEBUGER_ASSERT_VALID(pStation != NULL);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack != NULL);
			
			pJobItem = m_pDoc->m_pJob->FetchReservedJobByStartStation(pStation->m_strID);
			if (pJobItem != NULL && pStation != NULL)
			{
				strLog.Format(_T("이미 BCR#%s에서 %s Tray로 예약된 작업이 존재합니다! ( 시작위치=%s, 이전작업번호=%d, 신규작업번호=%d, 이전 TRAY=%s, 신규 TRAY=%s )"), 
					pStation->m_strID, pJobItem->m_strBarcode, pStation->m_strID, pJobItem->m_nLuggNum, rJobItem.m_nLuggNum, pJobItem->m_strBarcode, rJobItem.m_strBarcode);
				m_pDoc->Alarm(LOG_POS_HOST, strLog, pJobItem);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::JobOrderDirective"), pJobItem, pJobItem->m_strBarcode);
// Update : 2014.06.21 - Tray 밀림현상 처리
//						 해당 시작위치에 이미 예약된 작업이 있을 경우 작업상태를 에러로 변경 후 계속 진행
//				return;
				pJobItem->SetJobStatus(enJobStatusError);
//-------------------------------------------------------------------------------------------------------
			}
		}
		break;
		
	case enJobTypeAutoRet:
	case enJobTypeAutoPR:
		break;

//	case enJobTypeAutoR2R:
//	{
//		if (CLib::GetStackerNum(rJobItem.m_nStartWH, rJobItem.m_strStartLoc) != CLib::GetStackerNum(rJobItem.m_nDestWH, rJobItem.m_strDestLoc))
//		{
//			strLog.Format(_T("S/C 동일호기 작업이 아님! [랙이동 LOCATION 이상] [출발LOC=%s] [도착LOC=%s]"), rJobItem.m_strStartLoc, rJobItem.m_strDestLoc);
//			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::JobOrderDirective"));
//			return;
//		}
//	}
//	break;
		
//	  case enJobTypeAutoW2W:
//	  {
//		  if (CLib::GetStackerNum(rJobItem.m_nStartWH, rJobItem.m_strStartLoc) == CLib::GetStackerNum(rJobItem.m_nDestWH, rJobItem.m_strDestLoc))
//		  {
//			  strLog.Format(_T("S/C 동일호기 작업! [창고간이동 LOCATION 이상] [출발LOC=%s] [도착LOC=%s]"), rJobItem.m_strStartLoc, rJobItem.m_strDestLoc);
//			  m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::JobOrderDirective"));
//			  return;
//		  }
//	  }
//	  break;

	default:
		strLog.Format(_T("[CHostSv::JobOrderDirective] 정의되지 않은 작업종류입니다 ( 작업종류=%d )"), rJobItem.m_nJobType);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::JobOrderDirective"), &rJobItem);
		return;
	}
	
	if ((pJobItem = m_pDoc->m_pJob->Add(rJobItem)) == NULL)
	{
		strLog.Format(_T("[CHostSv::JobOrderDirective] 작업등록 실패! ( %s )"), rJobItem.GetLogString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::JobOrderDirective"), &rJobItem);
		return;
	}
	
	strLog.Format(_T("작업등록 ( %s )"), pJobItem->GetLogString());
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostServer::JobOrderDirective"), pJobItem);
	
	m_pDoc->m_pJob->Invoke(pJobItem->m_nLuggNum);
}

void CHostSv::RedirectLocDirective(CJobItem& rJobItem)
{/*
	CString strLog;
	CJobItem* pJobItem = NULL;
	
	if ((pJobItem = m_pDoc->m_pJob->Find(rJobItem.m_nLuggNum)) == NULL)
	{
		strLog.Format(_T("이중입고에러 작업정보가 존재하지 않음! [%d]"), rJobItem.m_nLuggNum);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::RedirectLocDirective"));
//		Answer(enHostCommandRedirect, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorNotExistJob);
		return;
	}
	
	if (pJobItem->m_nJobType != rJobItem.m_nJobType)
	{
		strLog.Format(_T("작업구분 불일치! [OLD=%s] [NEW=%s]"), pJobItem->GetJobStatusString(), rJobItem.GetJobTypeString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::RedirectLocDirective"), pJobItem);
//		Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorUnknownJobType);
		return;
	}
	
	if (pJobItem->m_nJobStatus != enJobStatusErrorDualStore)
	{
		strLog.Format(_T("이중입고에러 상태가 아님! [%s]"), pJobItem->GetJobStatusString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::RedirectLocDirective"), pJobItem);
//		Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorNotExistJob);
		return;
	}
	
	switch (pJobItem->m_nJobType)
	{
	case enJobTypeAutoSto:
	case enJobTypeAutoR2R:
	case enJobTypeAutoW2W:
		if (CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc) != CLib::GetStackerNum(rJobItem.m_nDestWH, rJobItem.m_strDestLoc))
		{
			strLog.Format(_T("S/C 동일호기 %s 작업이 아님! [재지정 LOCATION 이상] [OLD=%s] [NEW=%s]"), rJobItem.GetJobTypeString(), pJobItem->m_strDestLoc, rJobItem.m_strDestLoc);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::RedirectLocDirective"), pJobItem);
//			Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorInvalidDestLocation);
			return;
		}
		break;
		
	default:
		strLog.Format(_T("유효하지 않은 작업구분! [%s]"), pJobItem->GetJobStatusString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::RedirectLocDirective"), pJobItem);
//		Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorUnknownJobType);
		return;
	}
	
	pJobItem->m_strDestLoc = rJobItem.m_strDestLoc;
	pJobItem->SetJobStatus(enJobStatusDualStoreRetry);
	m_pDoc->m_pJob->Backup();
	
	strLog.Format(_T("%s 이중입고 재지정 [LOC=%s]"), pJobItem->GetJobTypeString(), pJobItem->m_strDestLoc);
	m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, strLog, _T("CHostServer::RedirectLocDirective"), pJobItem);
//	Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorNone);
*/}

void CHostSv::ResendOrderDirective(CJobItem& rJobItem)
{/*
	CString strLog;
	CJobItem* pJobItem = NULL;
	
	if (pJobItem = m_pDoc->m_pJob->Find(rJobItem.m_nLuggNum))
	{
		strLog.Format(_T("해당 작업정보 존재! [%s]"), rJobItem.GetLogString());
		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, rJobItem.m_nLuggNum, strLog, _T("CHostServer::ResendOrderDirective"));
//		Answer(enHostCommandResendOrder, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorNone);
		return;
	}
	
	if ((pJobItem = m_pDoc->m_pJob->Add(rJobItem)) == NULL)
	{
		strLog.Format(_T("작업추가 실패! [%s]"), rJobItem.GetLogString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostServer::ResendOrderDirective"));
//		Answer(enHostCommandResendOrder, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorBufferFull);
		return;
	}
	
	pJobItem->m_nJobStatus = enJobStatusResend;
	m_pDoc->Alarm(LOG_POS_HOST, strLog, pJobItem);
	
	strLog.Format(_T("재송신작업 수신 및 추가 [%s]"), pJobItem->GetLogString());
	m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, strLog, _T("CHostServer::ResendOrderDirective"), pJobItem);
//	Answer(enHostCommandResendOrder, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorNone);
*/}

void CHostSv::Answer(TCHAR chType, int nLuggNum, int nJobType, int nResultCode)
{
	Answer(chType, nLuggNum, nResultCode);
}

void CHostSv::Answer(BYTE ucMsgType, int nLuggNum, int nReasonCode)
{
	CString strProductID ="";
	char TxBuff[64];
	char AckNak = (nReasonCode == MSG_NO_ERROR) ? 'A':'N';
	ucMsgType += (ucMsgType == CMD_UNPREDICT) ? 0 : 32; // make lower case character
	
	memset(TxBuff, 0x20, sizeof(TxBuff));
	sprintf(TxBuff, "ECS_MBX   %04d ", MSG_LENGTH_RESPONSE_DATA);  //MSG_LENGTH_RESPONSE_DATA - 10
	sprintf(TxBuff + MSG_LENGTH_HEADER, "%c%c%c%02d%04d%c",
		STX, ucMsgType, AckNak, nReasonCode, nLuggNum, ETX);
	
	UpdateCommSv(NOTIFY_SEND);
	Send(TxBuff, MSG_LENGTH_RESPONSE_MSG);
}

void CHostSv::UpdateCommSv(char Status)
{
	::PostMessage(m_pDoc->m_hWndView, WM_USER_HOST_NOTIFY, 0, Status);
}

CString CHostSv::GetStartPos(int nStartPos)
{
	CString strTemp;
	
	if(nStartPos == ECS_STN_POS_101)
		strTemp.Format("입고대");
	
	return strTemp;	
}

CString CHostSv::GetDestPos(int nDestPos)
{
	CString strTemp;
	
	if(nDestPos == ECS_STN_POS_101)
		strTemp.Format("출고대");
	
	return strTemp;
}

//=============================================================================
//	Desc	: 완료 보고
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::Complete()
{
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchByStatus(enJobStatusCompleteRequest);
	if (pJobItem == NULL)	return FALSE;
	
	return Complete(pJobItem->m_nLuggNum);
}

//=============================================================================
//	Desc	: 완료 보고 재전송
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::ResendCompleteReport()
{
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchByStatus(enJobStatusRefuseComplete);
	if (pJobItem == NULL)	return FALSE;
	
	return Complete(pJobItem->m_nLuggNum, pJobItem->GetJobPattern());
}

//=============================================================================
//	Desc	: 완료 보고
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::Complete(int nLuggNum, int nScJobType, int nStation, BOOL bDelete, BOOL bManual)
{
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
	CString strLog, str = _T("");
	if (pJobItem == NULL)
	{
		if (nScJobType == CScInfo::enStore)			str = _T("입고");
		else if (nScJobType == CScInfo::enRetrieve)	str = _T("출고");

		strLog.Format(_T("[CHostSv::Complete] 작업정보 검색 실패! ( 작업번호=%d, 작업종류=%s )"), nLuggNum, str);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Error"), pJobItem);
		return FALSE;
	}

	//반자동작업 시 완료 보고 하지 않고 작업만 삭제...
	if(pJobItem->m_nLuggNum >= 9000)
	{
		m_pDoc->m_pJob->Remove(pJobItem);
		return TRUE;
	}

	int nStep = 1;				// C/V 완료
	
	switch (CJobItem::ConvertJobTypeToPattern(pJobItem->m_nJobType))
	{
	case enJobPatternSto:
		nScJobType = 1;		// 입고완료
		nStep = 0;
		break;
	case enJobPatternRet:
	case enJobPatternPR:
		nScJobType = 2;		// 출고완료
		if (pJobItem->m_nJobStatus == enJobStatusScComplete)
			nStep = 0;		// S/C 완료
		break;
	case enJobPatternMove:
		nScJobType = 6;		// 이동완료
		break;
	}
	
	if (nStation == 0)
		nStation = atoi(pJobItem->m_strDestStn);

	CString strStartLoc = _T("       "), strDestLoc = _T("       ");

	// Stacket Crane
	if (nStep == 0)
	{
		if (nScJobType == 2)	// 출고
		{
			strStartLoc.Format(_T("%2d%1d%02d%02d"), CLib::GetStackerNum(1, pJobItem->m_strStartLoc) * 10, 
				1, CLib::GetBay(pJobItem->m_strStartLoc), CLib::GetLevel(pJobItem->m_strStartLoc));
		}
		if (nScJobType == 1)	// 입고
		{
			strDestLoc.Format(_T("%2d%1d%02d%02d"), CLib::GetStackerNum(1, pJobItem->m_strDestLoc) * 10, 
				1, CLib::GetBay(pJobItem->m_strDestLoc), CLib::GetLevel(pJobItem->m_strDestLoc));
		}
	}

	COleDateTime t = COleDateTime::GetCurrentTime();
	char TxBuff[MSG_LENGTH_COMPLETE_MSG + 1];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1503%04d%02d%02d%02d%02d%02d%4d%06d:%d%03s%03s%07s%07s%-6s%20s*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			0, MSG_LENGTH_COMPLETE_DATA, nScJobType, pJobItem->m_strStartStn, pJobItem->m_strDestStn, 
			strStartLoc, strDestLoc, pJobItem->m_strProductID, " ");

	strLog.Format(_T("[SEND Complete] %s"), TxBuff);
	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Complete"), pJobItem);

	if (SendMsg(TxBuff, MSG_LENGTH_COMPLETE_MSG) == TRUE)
	{
		strLog.Format("%s 완료보고 ( 작업종류=%s, 시작위치=%s, 완료위치=%s )", 
			(bManual) ? "수동" : "", pJobItem->GetJobTypeString(), 
			(nScJobType == 1) ? pJobItem->m_strStartStn : pJobItem->m_strStartLoc,
			(nScJobType == 1) ? pJobItem->m_strDestLoc : pJobItem->m_strDestStn);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Complete"), pJobItem);
		
		if (pJobItem->m_nJobType == enJobTypeAutoSto ||
			pJobItem->m_nJobType == enJobTypeAutoMove ||
			pJobItem->m_nJobType == enJobTypeAutoR2R ||
			(pJobItem->m_nJobType == enJobTypeAutoPR && nStep == 1) ||
			(pJobItem->m_nJobType == enJobTypeAutoRet && nStep == 1))
		{
			if (bDelete == TRUE)
				pJobItem->m_nJobStatus = enJobStatusCompleteRequest;
		}
	}
	else
	{
		strLog.Format("%s 완료보고 실패! ( 작업종류=%s, 시작위치=%s, 완료위치=%s, MSG=%s )", 
			(bManual) ? "수동" : "", pJobItem->GetJobTypeString(), 
			(nScJobType == 1) ? pJobItem->m_strStartStn : pJobItem->m_strStartLoc,
			(nScJobType == 1) ? pJobItem->m_strDestLoc : pJobItem->m_strDestStn, CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Complete"), pJobItem);
		pJobItem->m_nJobStatus = enJobStatusRefuseComplete;
		return FALSE;
	}
	
	return TRUE;
}

//=============================================================================
//	Desc	: S/C 상태 에러 보고
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::ErrorScReport(CJobItem* pJobItem, EN_ERROR_GBN enErrorGbn, CString strErrMsg)
{
	CString strLog;

	if (pJobItem == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}
	
	int nScNum = CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc);
	if (IsConnect() == FALSE)
	{
		strLog.Format(_T("[CHostSv::ErrorScReport] S/C %d호기와 통신연결이 끊어졌습니다!"), nScNum);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::ErrorScReport"), pJobItem);
		return FALSE;
	}
	
	int nErrorCode = (enErrorGbn == enErrorDualStore)? enScErrorDualStore : enScErrorEmptyRetrieve;
	int nErrorKind = (enErrorGbn == enErrorDualStore)? 1 : 3;
	
	Error(1, 0, nScNum, nErrorKind, nErrorCode, strErrMsg, pJobItem->m_nLuggNum, pJobItem->m_strDestLoc);
	
	return TRUE;
}

//=============================================================================
//	Desc	: 설비상태 에러 보고
//	Date	:
//	Update	: 
//=============================================================================
void CHostSv::Error(int nDevKind, int nRelease, int nDevNo, int nErrKind, WORD wErrCode, CString strErrMsg, int nLuggNum, CString strLocation, BOOL bManual)
{
	CJobItem *pJobItem = NULL;
	CString strLog;

	if (nLuggNum && nErrKind)
	{
		pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
		if (pJobItem == NULL)
		{
			strLog.Format(_T("[CHostSv::Error] 설비상태보고 중 작업번호 검색 실패! ( 작업번호=%d, S/C#%d, LOC=%s )"),
				nLuggNum, nDevNo, strLocation);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Error"), pJobItem);
			return;
		}
		else
			nLuggNum = pJobItem->m_nLuggNum;

	}
	else
		nLuggNum = 0;
	
	/*
	if (pJobItem->m_strDestLoc == NULL)
		strLocation = pJobItem->m_strStartLoc;
	else
		strLocation = pJobItem->m_strDestLoc;
	*/

	if (nDevKind == 1)
	{
		if (nLuggNum >= 9000 || (nLuggNum == 0 && (wErrCode == 58 || wErrCode == 54)))
		{
			return;
		}
	
		if (nDevNo == 0)
		{
			return;
		}
	}

//	int	nBank, nBay, nLevel;
//	if (strLocation.IsEmpty())
//	{
//		nBank = nBay = nLevel = 0;
//	}
//	else
//	{
//		nBank  = CLib::GetBank(strLocation);
//		nBay   = CLib::GetBay(strLocation);
//		nLevel = CLib::GetLevel(strLocation);
//	}
	
	COleDateTime t = COleDateTime::GetCurrentTime();
	char TxBuff[MSG_LENGTH_ERROR_MSG + 1];
	memset(TxBuff, 0x00, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1508%04d%02d%02d%02d%02d%02d%4d%06d:%04d%-2s%03d       %-50s%d*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			(pJobItem == NULL) ? 0 : pJobItem->m_nResultCode, MSG_LENGTH_ERROR_DATA, nDevNo, 
			(nDevKind == 1) ? "SC" : "CV", wErrCode, strErrMsg, nRelease);

//	strLog.Format(_T("[SEND Error] %s"), TxBuff);
//	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Error"));

	if (SendMsg(TxBuff, MSG_LENGTH_ERROR_MSG) == TRUE)
	{
		if (nDevKind == 1)	// SC
		{
			strLog.Format("%s 설비상태 %s보고 ( S/C#%d, LOC=%s, %d=%s )",
				(bManual) ? "수동" : "", (nRelease == 0) ? "에러" : "에러조치", nDevNo, strLocation, wErrCode, strErrMsg);
		}
		else				// CV
		{
			strLog.Format("%s 설비상태 %s보고 ( TRACK#%03d, %d=%s )",
				(bManual) ? "수동" : "", (nRelease == 0) ? "에러" : "에러조치", nDevNo, wErrCode, strErrMsg);
		}
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Error"), pJobItem);
	}
	else
	{
		if (nDevKind == 1)	// SC
		{
			strLog.Format("%s 설비상태 %s보고 실패! ( S/C#%d, LOC=%s, %d=%s, SYSERR=%s )",
				(bManual) ? "수동" : "", (nRelease == 0) ? "에러" : "에러조치", 
				nDevNo, strLocation, wErrCode, strErrMsg, CLib::GetSystemErrMsg());
		}
		else				// CV
		{
			strLog.Format("%s 설비상태 %s보고 실패! ( TRACK#%03d, %d=%s, SYSERR=%s )",
				(bManual) ? "수동" : "", (nRelease == 0) ? "에러" : "에러조치", 
				nDevNo, wErrCode, strErrMsg, CLib::GetSystemErrMsg());
		}
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Error"), pJobItem);
		
		if (nLuggNum && nErrKind && pJobItem)
			pJobItem->m_nJobStatus = enJobStatusRefuseError;
	}
}

//=============================================================================
//	Desc	: 출고대 도착 보고
//	Date	:
//	Update	: 
//=============================================================================
void CHostSv::Arrived(int nLuggNum, BOOL bManual)
{
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
	CString strLog;
	if (pJobItem == NULL)
	{
		strLog.Format(_T("[CHostSv::Arrived] 출고대 도착 처리중 작업번호 검색 실패! ( 작업번호=%d )"), nLuggNum);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Arrived"), pJobItem);
		return;
	}

	if (pJobItem->m_nJobStatus == enJobStatusArrived)
		m_pDoc->m_pJob->Remove(pJobItem);
}

//=============================================================================
//	Desc	: 도착지 도착 보고
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::BarcodeData(CString strStation, CString strBcr, BOOL bManual)
{
// Update : 2014.06.13 - Tray 밀림현상 처리
//						 해당 Tray에 대해 동일한 작업이 있는 경우 Skip
	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(strBcr);

	if (pJobItem != NULL && pJobItem->m_strStartStn == strStation)
	{
		strLog.Format(_T("이미 BCR#%s에서 %s Tray의 작업이 진행중입니다. ( 시작위치=%s, 작업번호=%d, TRAY=%s )"), 
			strStation, strBcr, strStation, pJobItem->m_nLuggNum, strBcr);
		m_pDoc->Alarm(LOG_POS_HOST, strLog, pJobItem);
		m_pDoc->WriteLog(LOG_TYPE_ALARM, LOG_POS_HOST, strLog, _T("CHostSv::BarcodeData"), pJobItem, strBcr);
		return FALSE;
	}
//-------------------------------------------------------------------------

	//	m_bArriveReport = TRUE;
	int nResultCode = 0, nCount = 0;
	CTrackInfo* pTrack = NULL;
// Update : 2014.01
// BCR#603 - Lifter 출고대 이동 Tray 대기 수량이 20개이면 진입금지
//			 FMS로 도착보고시 ResultCode에 진입여부 전송 
// TRACK#041 ~ TRACK#060
	if (strStation == _T("603"))
	{
		for (int n = 41; n <= 60; n++)
		{
			pTrack = m_pDoc->GetTrackInfo(n, 0);
			if (pTrack == NULL)	continue;
			if (pTrack->IsProductSensing() && pTrack->m_nDestPos > 0)	++nCount;
		}
		nResultCode = (nCount == 20) ? 1 : 0;
	}
//------------------------------------------------------------------
// Update : 2014.07.03 - Degassing에서 배출한 공Tray를 BCR#613에서 다시 Degassing으로 이동하도록 처리
//						 FMS로 도착보고시 ResultCode에 현재 공Tray 대기 수량 전송
// TRACK#173 ~ TRACK#175
	if (strStation == _T("613"))
	{
		nResultCode = 0, nCount = 0;
		for (int n = 173; n <= 175; n++)
		{
			pTrack = m_pDoc->GetTrackInfo(n, 0);
			if (pTrack == NULL)	continue;
			if (pTrack->IsProductSensing() && pTrack->m_nDestPos > 0)	++nCount;
		}
		nResultCode = nCount;
	}
//----------------------------------------------------------------------------------------------------
// Update : 2014.08 - DCIR TRAY 대기 버퍼 추가 (대기 버퍼의 TRAY 수량 전송)
// TRACK#478 ~ TRACK#484
	if (strStation == _T("646"))
	{
		nResultCode = 0, nCount = 0;
		for (int n = 478; n <= 484; n++)
		{
			pTrack = m_pDoc->GetTrackInfo(n, 0);
			if (pTrack == NULL)	continue;
			if (pTrack->IsProductSensing() && pTrack->m_nDestPos > 0)	++nCount;
		}
		nResultCode = nCount;	// BCR#646 TRAY 포함
	}
//----------------------------------------------------------------------------------------------------
	COleDateTime t = COleDateTime::GetCurrentTime();
	CStationInfo * pStation = m_pDoc->GetStationInfo(strStation);
	if (pStation == NULL)	return FALSE;
	char TxBuff[MSG_LENGTH_ARRIVAL_MSG + 1];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1501%04d%02d%02d%02d%02d%02d%4d%06d:%d%03s%-6s*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			nResultCode, MSG_LENGTH_ARRIVAL_DATA, pStation->m_enKind, pStation->m_strID, strBcr);
	
	strLog.Format(_T("[SEND Arrive] %s"), TxBuff);
	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::BarcodeData"), NULL, strBcr);

	if (SendMsg(TxBuff, MSG_LENGTH_ARRIVAL_MSG) == TRUE)
	{
		strLog.Format("%s 도착지 도착보고 ( TRAY=%s, %s=%s )", 
			(bManual) ? "수동" : "", strBcr, strStation, pStation->m_strName);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::BarcodeData"), NULL, strBcr);
//		pJobItem->m_nJobStatus = enJobStatusArrived;
		pStation->m_strTrayId = strBcr;
	}
	else
	{
		strLog.Format("%s 도착지 도착보고 실패! ( TRAY=%s, %s=%s )", 
			(bManual) ? "수동" : "", strBcr, strStation, pStation->m_strName);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::BarcodeData"), NULL, strBcr);
//		pJobItem->m_nJobStatus = enJobStatusRefuseArrived;
//		m_bArriveReport = FALSE;
		return FALSE;
	}
	
//	m_bArriveReport = FALSE;
	return TRUE;
}

//=============================================================================
//	Desc	: 도착 보고 재전송
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::ResendArrivedReport()
{
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchByStatus(enJobStatusRefuseArrived);
	if (pJobItem == NULL)	return FALSE;
	
	BOOL bResult = FALSE;
//	if (pJobItem->GetJobPattern() == enJobPatternSto)		// Aging 입고 도착보고
//	{
		if (pJobItem->GetAgingStation().IsEmpty())	return FALSE;
		bResult = AgingStoArrived(pJobItem->m_nLuggNum, pJobItem->GetAgingStation());
//	}
//	else if (pJobItem->GetJobPattern() == enJobPatternMove)	// 이동작업 도착보고
//	{
//		bResult = BarcodeData(pJobItem->m_strStartStn, pJobItem->m_strBarcode);
//	}

	return bResult;
}

//=============================================================================
//	Desc	: Aging 입고대 도착 보고
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::AgingStoArrived(int nLuggNum, CString strStation, BOOL bManual)
{
	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);

	if (strStation.IsEmpty())
	{
		strLog.Format(_T("[CHostSv::AgingStoArrived] Aging 입고대 도착보고 중 스테이션정보 검색 실패1! ( 작업번호=%d, %s )"), 
			nLuggNum, strStation);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::AgingStoArrived"), pJobItem);
		return FALSE;
	}

	CStationInfo * pStation = m_pDoc->GetStationInfo(strStation);

	if (pJobItem == NULL)
	{
		strLog.Format(_T("[CHostSv::AgingStoArrived] Aging 입고대 도착보고 중 작업번호 검색 실패! ( 작업번호=%d, %s=%s )"), 
			nLuggNum, strStation, pStation->m_strName);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::AgingStoArrived"), pJobItem);
		return FALSE;
	}

	if (pStation == NULL)
	{
		strLog.Format(_T("[CHostSv::AgingStoArrived] Aging 입고대 도착보고 중 스테이션정보 검색 실패2! ( 작업번호=%d, %s )"), 
			nLuggNum, strStation);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::AgingStoArrived"), pJobItem);
		return FALSE;
	}

	COleDateTime t = COleDateTime::GetCurrentTime();
	char TxBuff[MSG_LENGTH_ARRIVAL_MSG + 1];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1501%04d%02d%02d%02d%02d%02d%4d%06d:%d%03s%-6s*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			pJobItem->m_nResultCode, MSG_LENGTH_ARRIVAL_DATA, pJobItem->m_nJobType, strStation, pJobItem->m_strBarcode);

	strLog.Format(_T("[SEND Arrive] %s"), TxBuff);
	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::AgingStoArrived"), pJobItem);

	if (SendMsg(TxBuff, MSG_LENGTH_ARRIVAL_MSG) == TRUE)
	{
		strLog.Format("%s Aging 입고대 도착보고 ( 작업종류=%s, %s=%s )",  
			(bManual) ? "수동" : "", pJobItem->GetJobTypeString(), strStation, pStation->m_strName);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::AgingStoArrived"), pJobItem);
		pJobItem->m_nJobStatus = enJobStatusArrived;
	}
	else
	{
		strLog.Format("%s Aging 입고대 도착보고 실패! ( 작업종류=%s, %s=%s )",  
			(bManual) ? "수동" : "", pJobItem->GetJobTypeString(), strStation, pStation->m_strName);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::AgingStoArrived"), pJobItem);
		pJobItem->m_nJobStatus = enJobStatusRefuseArrived;
		return FALSE;
	}
	return TRUE;
}

//=============================================================================
//	Desc	: 온습도 정보 보고
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::SendThsInfo(BOOL bManual)
{
	COleDateTime t = COleDateTime::GetCurrentTime();
	char TxBuff[MSG_LENGTH_THS_MSG + 1];
	CString str = "", strTemp = "";

	// 온도정보
	int nCount = m_pDoc->m_mapTempVals.GetCount();

	for (int n = 0; n < nCount; n++)
	{
		if (!m_pDoc->m_mapTempVals.Lookup(n + 1, str))
			strTemp += "000";
		else
			strTemp += str;
	}

	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1509%04d%02d%02d%02d%02d%02d%4d%06d:%s*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			0, MSG_LENGTH_THS_DATA, strTemp);
	
	CString strLog;
//	strLog.Format(_T("[SEND Temp] %s"), TxBuff);
//	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::SendThsInfo"));

	if (SendMsg(TxBuff, MSG_LENGTH_THS_MSG) == TRUE)
	{
		strLog.Format("%s 온도정보 보고",  (bManual) ? "수동" : "");
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::SendThsInfo"));
	}
	else
	{
		strLog.Format("%s 온도정보 보고 실패!", (bManual) ? "수동" : "");
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::SendThsInfo"));
		return FALSE;
	}

	::Sleep(50);

	// 습도정보
	nCount = m_pDoc->m_mapHumVals.GetCount();
	str = "", strTemp = "";

	for (n = 0; n < nCount; n++)
	{
		if (!m_pDoc->m_mapHumVals.Lookup(n + 1, str))
			strTemp += "000";
		else
			strTemp += str;
	}

	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1510%04d%02d%02d%02d%02d%02d%4d%06d:%s*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			0, MSG_LENGTH_THS_DATA, strTemp);
	
//	strLog.Format(_T("[SEND Hum] %s"), TxBuff);
//	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::SendThsInfo"));

	if (SendMsg(TxBuff, MSG_LENGTH_THS_MSG) == TRUE)
	{
		strLog.Format("%s 습도정보 보고",  (bManual) ? "수동" : "");
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::SendThsInfo"));
	}
	else
	{
		strLog.Format("%s 습도정보 보고 실패!", (bManual) ? "수동" : "");
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::SendThsInfo"));
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
//	Desc	: 스태커 상태보고
//	Date	:
//	Update	: 
//=============================================================================
void CHostSv::Status(int nRegularCycle)
{
#define	SC_STATUS_NORMAL_WAIT			0
#define	SC_STATUS_STORING				1
#define	SC_STATUS_RETRIEVING			2
#define	SC_STATUS_RACK_TO_RACK			3
#define	SC_STATUS_NO_ONLINE				4
#define	SC_STATUS_ERROR					5
#define	SC_STATUS_SUSPEND_STO			6
#define	SC_STATUS_SUSPEND_RET			7
#define	SC_STATUS_SUSPEND_ALL			8
	
	int nCvStatus = 1, nScStatus = 1, nStoHSStatus = 1, nRetHSStatus = 1;

	COleDateTime t = COleDateTime::GetCurrentTime();
	char TxBuff[MSG_LENGTH_STATUS_MSG + 1];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1512%04d%02d%02d%02d%02d%02d%4d%06d:", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			0, MSG_LENGTH_STATUS_DATA);

	for (int i = 0; i < SC_PLC_CNT; i++)
	{
		CSc* pSc = (CSc*) m_pDoc->GetEquipment(CEquipment::enSC, i + 1);
		DEBUGER_ASSERT_VALID(pSc != NULL);
		
		CScInfo *pScInfo = (CScInfo *) pSc->GetInfo();
		DEBUGER_ASSERT_VALID(pScInfo != NULL);
		
		if (!pScInfo->m_bInvoke && pScInfo->m_ucRcStatus == CScInfo::enActive)
			nScStatus = SC_STATUS_NORMAL_WAIT;
		
		if (pScInfo->m_bInvoke)
		{
			if (pScInfo->m_nInternalJobType == CScInfo::enRetrieve)
				nScStatus = SC_STATUS_RETRIEVING;
			else if (pScInfo->m_nInternalJobType == CScInfo::enStore)
				nScStatus = SC_STATUS_STORING;
			else if (pScInfo->m_nInternalJobType == CScInfo::enRack2Rack)
				nScStatus = SC_STATUS_RACK_TO_RACK;
		}

		if (pScInfo->m_ucRcMode != CScInfo::enOnline)
			nScStatus = SC_STATUS_NO_ONLINE;
		if (pScInfo->m_ucScMode != CScInfo::enAuto)
			nScStatus = SC_STATUS_NO_ONLINE;
//		if (pScInfo->m_ucScMode != CScInfo::enActive)
//			nScStatus = SC_STATUS_NO_ONLINE;
		if (pScInfo->m_ucScStatus == CScInfo::enError)
			nScStatus = SC_STATUS_ERROR;
		if (pScInfo->m_bStoSuspend)
			nScStatus = SC_STATUS_SUSPEND_STO;
		if (pScInfo->m_bRetSuspend)
			nScStatus = SC_STATUS_SUSPEND_RET;
		if (pScInfo->m_bStoSuspend && pScInfo->m_bRetSuspend)
			nScStatus = SC_STATUS_SUSPEND_ALL;
		
		int nLugg = pScInfo->m_nInternalLuggNum;
		sprintf(TxBuff + MSG_LENGTH_HEADER - 2 + (25 * i), "%d%04d%04d%02d%04d%-6s%d%d%d%d",
				nScStatus, nLugg, 0, (m_pDoc->m_pJob->Find(nLugg) != NULL) ? 1 : 0, 
				pScInfo->m_nErrorCode, "00-000", pScInfo->m_ucForkStatus, pScInfo->m_ucProdLoad, 0, 0);
	}
	sprintf(TxBuff + MSG_LENGTH_HEADER - 2 + 50, "%s", "*;");
	
	CString strLog;
//	strLog.Format(_T("[SEND SC] %s"), TxBuff);
//	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::Status"));

	if (SendMsg(TxBuff, MSG_LENGTH_STATUS_MSG) == FALSE)
	{
		strLog.Format("스태커 상태보고 실패! ( %s )", CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Status"));
	}
	else 
	{
		strLog = _T("스태커 상태보고");
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::Status"));
	}
}

//=============================================================================
//	Desc	: HeartBit 통지
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::SendHeartBit(BOOL bManual)
{
	COleDateTime t = COleDateTime::GetCurrentTime();
	char TxBuff[MSG_LENGTH_HEADER + 1];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1001%04d%02d%02d%02d%02d%02d%4d%06d:*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), 0, 0);
	
	CString strLog;
//	strLog.Format(_T("[SEND HeartBit] %s"), TxBuff);
//	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::SendHeatBit"));

	if (SendMsg(TxBuff, MSG_LENGTH_HEADER) == TRUE)
	{
		m_pDoc->m_bHeartBit = TRUE;
		strLog.Format(_T("HeartBit 통지"));
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::SendHeatBit"));
	}
	else
	{
		strLog.Format(_T("HeartBit 통지 실패!"));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::SendHeatBit"));
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
//	Desc	: 동간 TRAY 대기 수량 보고
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::SendEmptyTrayWaitCount()
{
	COleDateTime t = COleDateTime::GetCurrentTime();
	char TxBuff[MSG_LENGTH_EMPTY_TRAY_MSG + 1];
	int nCount = 0;
	CTrackInfo* pTrack = NULL;

	for (int n = 41; n <= 60; n++)
	{
		pTrack = m_pDoc->GetTrackInfo(n, 0);
		if (pTrack == NULL)	continue;
		if (pTrack->IsProductSensing() && pTrack->m_nDestPos > 0)	++nCount;
	}

	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1511%04d%02d%02d%02d%02d%02d%4d%06d:%-10d*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			0, MSG_LENGTH_EMPTY_TRAY_DATA, nCount);
	
	CString strLog;
//	strLog.Format(_T("[SEND EmptyTray] %s"), TxBuff);
//	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::SendEmptyTrayWaitCount"));

	if (SendMsg(TxBuff, MSG_LENGTH_EMPTY_TRAY_MSG) == TRUE)
	{
		strLog.Format(_T("동간 TRAY 대기 수량 보고 ( 수량=%d )"), nCount);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::SendEmptyTrayWaitCount"));
	}
	else
	{
		strLog.Format(_T("동간 TRAY 대기 수량 보고 실패! ( 수량=%d )"), nCount);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::SendEmptyTrayWaitCount"));
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
//	Desc	: DCIR TRAY 대기 버퍼 추가 (대기 버퍼의 TRAY 수량 전송)
//	Date	:
//	Update	: 
//=============================================================================
BOOL CHostSv::SendDcirTrayWaitCount()
{
	COleDateTime t = COleDateTime::GetCurrentTime();
	char TxBuff[MSG_LENGTH_DCIR_TRAY_MSG + 1];
	int nCount = 0;
	CTrackInfo* pTrack = NULL;

	for (int n = 478; n <= 484; n++)
	{
		pTrack = m_pDoc->GetTrackInfo(n, 0);
		if (pTrack == NULL)	continue;
		if (pTrack->IsProductSensing() && pTrack->m_nDestPos > 0)	++nCount;
	}

	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS3F01IMS     1506%04d%02d%02d%02d%02d%02d%4d%06d:%-10d*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			0, MSG_LENGTH_DCIR_TRAY_DATA, nCount);
	
	CString strLog;
//	strLog.Format(_T("[SEND EmptyTray] %s"), TxBuff);
//	m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostSv::SendDcirTrayWaitCount"));

	if (SendMsg(TxBuff, MSG_LENGTH_DCIR_TRAY_MSG) == TRUE)
	{
		strLog.Format(_T("DCIR TRAY 대기 수량 보고 ( 수량=%d )"), nCount);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::SendDcirTrayWaitCount"));
	}
	else
	{
		strLog.Format(_T("DCIR TRAY 대기 수량 보고 실패! ( 수량=%d )"), nCount);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::SendDcirTrayWaitCount"));
		return FALSE;
	}

	return TRUE;
}

void CHostSv::JobRequest(int nStationNo, int nJobKind, BOOL bManual)
{
	if (nJobKind != 1 && nJobKind != 2)
		return;

	char TxBuff[256];
	memset(TxBuff, 0x20, sizeof(TxBuff));
	CString strLog;
	
	sprintf(TxBuff, "ECS_MBX   %04d ", MSG_LENGTH_PALLET_DATA);//64
	sprintf(TxBuff + MSG_LENGTH_HEADER, "%c%c%d%03d%c%c", STX, CMD_STORE_REQUEST, nJobKind, nStationNo, 0x20, ETX);
	
	if (SendMsg(TxBuff, MSG_LENGTH_PALLET_MSG) == FALSE)
	{
		strLog.Format("%s 공파레트 %s 요청 송신 실패.. 에러=[%s]", 
			(bManual) ? "수동" : "", 
			(nJobKind == 1) ? "입고" : ((nJobKind == 2) ? "출고" : "이동"), 
			CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobRequest"));
		return;
	}
	
	strLog.Format("%s 공파레트 %s 요청 송신.. Station=[%d] [%s] ", 
		(bManual) ? "수동" : "", 
		(nJobKind == 1) ? "입고" : ((nJobKind == 2) ? "출고" : "이동"), 
		nStationNo, 
		TxBuff);
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::JobRequest"));
}

BOOL CHostSv::SendMsg(char* pTxBuff, int nSndMsgLen)
{
	if (m_pDoc->m_pHostSv != NULL)
	{
		UpdateCommSv(NOTIFY_SEND);
		int nTemp = Send(pTxBuff, nSndMsgLen);
		BOOL bTemp = (nTemp == nSndMsgLen);
//		return (nTemp == nSndMsgLen) ? TRUE : FALSE;
		return bTemp;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// CHostCl

CHostCl::CHostCl(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	
//	nHogi = 0;
	nStackerStatus = 0;
	nLuggNum = 0;
//	nSto101 = 0;
//	nSto102 = 0;
//	nRet201 = 0;
//	nRet202 = 0;
	nStoRetMode =0;
}

//CHostCl::CHostCl(LPCTSTR lpszName, BOOL bWriteLog)
//{
//	m_strName = lpszName;
//	m_bWriteLog = bWriteLog;
//	m_enCommStatus = enStatusClose;
//}

CHostCl::~CHostCl()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CHostCl, CAsyncSocketEx)
//{{AFX_MSG_MAP(CHostCl)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

////////////////////////////////////
// CHostCl member functions

void CHostCl::OnConnect(int nErrorCode) 
{
	CString strLog;
	if (nErrorCode)
	{
		// 프로그램 종료시 CEcsView::OnTimer로인해 CEcsDoc 객체소멸후 OnConnect 호출되면 에러
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if (pMainFrame != NULL)
		{
			CEcsDoc* pDoc = (CEcsDoc*)pMainFrame->GetActiveDocument();
			if (pDoc != NULL)
			{
				UpdateCommCl(NOTIFY_ERROR);
				
				strLog.Format(_T("ECS HOST CLIENT와 FMS SERVER 연결 실패! [%s]"), CLib::GetSystemErrMsg(nErrorCode));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostClient::OnConnect"), NULL, NULL, TRUE);
				
				CAsyncSocket::ShutDown();
				m_enCommStatus = enStatusConnectFail;
				pDoc->m_pHostCl = NULL;
			}
			m_enCommStatus = enStatusConnectFail;
		}
		m_enCommStatus = enStatusConnectFail;
		
		delete this;
	}
	else
	{
		UpdateCommCl(NOTIFY_SEND);
		
		strLog.Format(_T("ECS HOST CLIENT와 FMS SERVER 연결 성공!"));
		m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, strLog, _T("CHostClient::OnConnect"));
		
		m_enCommStatus = enStatusConnectOK;
		m_pDoc->m_pHostCl = this;
		m_pDoc->StatusReport();
	}
}

void CHostCl::OnClose(int nErrorCode) 
{
	UpdateCommCl(NOTIFY_ERROR);
	
	CString strLog;
	strLog.Format("ECS클라이언트와 FMS 서버 연결 해제..! [%s]", CLib::GetSystemErrMsg(nErrorCode));
	
	LOG_EVENT(LOG_POS_SYSTEM, LOG_SYSTEM, strLog);
	m_pDoc->m_pHostCl = NULL;
	ShutDown();
	delete this;
	
	CAsyncSocketEx::OnClose(nErrorCode);
}

void CHostCl::OnReceive(int nErrorCode) 
{
	CAsyncSocketEx::OnReceive(nErrorCode);
	
	if (nErrorCode) {
		AfxMessageBox("The Windows Sockets implementation (HostCl) detected that the network subsystem failed.");
	}
	
	int nMsgLen = 0;
	char RxBuff[1024];
	CString strLog;
	
	DWORD dwLen = 0;
	DWORD nRealLen = 0;
	
	if(IOCtl( FIONREAD, &dwLen ) == FALSE)
	{
		strLog.Format("CHostCl::OnReceive.. 메시지 수신 에러 [%s]", CLib::GetSystemErrMsg());
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
		UpdateCommCl(NOTIFY_ERROR);
		return;
	}
	
	memset(RxBuff, 0x00, sizeof(RxBuff));
	if ((nRealLen = Receive(RxBuff, dwLen)) != dwLen)
	{
		strLog.Format("ECS클라이언트 메시지 수신.. 수신 메시지 길이 이상.. LEN[%d]  REAL_LEN[%d]", dwLen, nRealLen);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
		UpdateCommCl(NOTIFY_ERROR);
		return;
	}
	
	if (RxBuff[0] != '@')
	{
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, "ECS클라이언트 데이터 수신.. No STX 에러..!");
		UpdateCommCl(NOTIFY_ERROR);
		return;
	}
	
	int nFrameLen = dwLen -1;
	if (RxBuff[nFrameLen - 1] != '*' || RxBuff[nFrameLen] != ';')
	{
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, "ECS클라이언트 데이터 수신.. No ETX 에러..!");
		UpdateCommCl(NOTIFY_ERROR);
		return;
	}
	
	Parsing(RxBuff);
	UpdateCommCl(NOTIFY_RECEIVE);
	
}

CString CHostCl::GetStringReason(int nReasonCode)
{
	return CLib::ConvertReasonToString(nReasonCode);
}

void CHostCl::Parsing(char *pFrame)
{
/*	CString strLog;
	
	CString strFrame = pFrame;
	BYTE ucMsgType   = pFrame[1];
	BYTE ucAckNak    = pFrame[2];
	int nResultCode  = atoi(strFrame.Mid(3, 2));
	int nLuggNum     = atoi(strFrame.Mid(5, 4));
	
	switch (ucMsgType)
	{
	case	CMD_COMPLETE_ANSWER:
		{	
			CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
			if (pJobItem == NULL)
			{
				strLog.Format(_T("완료보고 응답 이상 [%d번 작업정보가 존재하지 않음]"), nLuggNum);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, nLuggNum, strLog, _T("CHostCl::Parsing"));
				return;
			}
			
			if ((nResultCode != enHostErrorNone) || (ucAckNak != 'A'))
			{
				pJobItem->SetResultCode(nResultCode);
				
				strLog.Format(_T("완료보고 응답 이상 [%d=%s]"), nResultCode, CLib::GetHostResultSting(nResultCode));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"), pJobItem);
				return;
			}
			
			strLog.Format(_T("완료보고 응답 수신"));
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"), pJobItem);
			
			if (pJobItem->m_nJobStatus == enJobStatusCompleteRequest)
				m_pDoc->m_pJob->Remove(pJobItem);
			
		}
		break;
		
	case	CMD_LOAD_ARRV_ANSWER:
		{
			
			CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
			if (pJobItem == NULL)
			{
				strLog.Format(_T("도착보고 응답 이상 [%d번 작업정보가 존재하지 않음]"), nLuggNum);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, nLuggNum, strLog, _T("CHostCl::Parsing"));
				return;
			}
			
			if ((nResultCode != enHostErrorNone) || (ucAckNak != 'A'))
			{
				pJobItem->SetResultCode(nResultCode);
				
				strLog.Format(_T("도착보고 응답 이상 [%d=%s]"), nResultCode, CLib::GetHostResultSting(nResultCode));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"), pJobItem);
				return;
			}
			
			strLog.Format(_T("도착보고 응답 수신"));
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"), pJobItem);
			
			if (pJobItem->m_nJobStatus == enJobStatusArrived)
				m_pDoc->m_pJob->Remove(pJobItem);
			
		}
		break;
		
	case	CMD_ERROR_ANSWER:
		{
			CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
			if (pJobItem == NULL)
			{
				strLog.Format(_T("에러보고 응답 이상 [%d번 작업정보가 존재하지 않음]"), nLuggNum);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, nLuggNum, strLog, _T("CHostCl::Parsing"));
				return;
			}
			
			if ((nResultCode != enHostErrorNone) || (ucAckNak != 'A'))
			{
				pJobItem->SetResultCode(nResultCode);
				
				strLog.Format(_T("에러보고 응답 이상 [%d=%s]"), nResultCode, CLib::GetHostResultSting(nResultCode));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"), pJobItem);
				return;
			}
			
			switch (pJobItem->m_nJobStatus)
			{
			case enJobStatusErrorEmptyRetrieve:
				strLog.Format(_T("공출고 에러보고 응답 수신"));
				m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"), pJobItem);
				m_pDoc->m_pJob->Remove(pJobItem);
				break;
				
			case enJobStatusErrorDualStore:
				strLog.Format(_T("이중입고 에러보고 응답 수신"));
				m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"), pJobItem);
				pJobItem->m_bRecvAckDualSto = TRUE;
				break;
			}
			
		}
		
		break;
		
	case	CMD_STATUS_ANSWER:
		{
			CString strLog;
			if ((nResultCode != enHostErrorNone) || (ucAckNak != 'A'))
			{
				strLog.Format(_T("상태보고 응답 이상 [%d=%s]"), nResultCode, CLib::GetHostResultSting(nResultCode));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, nLuggNum, strLog, _T("CHostCl::Parsing"));
				return;
			}
			
			strLog.Format(_T("상태보고 응답"));
			m_pDoc->StatusDisplay(strLog);
		}
		break;
		
	case	CMD_BARCODE_ANSWER:
		{
			if ((nResultCode != enHostErrorNone) || (ucAckNak != 'A'))
			{
				strLog.Format(_T("바코드보고 응답 이상 [%d=%s]"), nResultCode, CLib::GetHostResultSting(nResultCode));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
				return;
			}
			
			strLog.Format(_T("바코드보고 응답 수신 "));
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
		}
		break;
		
	case	CMD_STORE_REQUEST_ANSWER:
		{
			if ((nResultCode != enHostErrorNone) || (ucAckNak != 'A'))
			{
				strLog.Format(_T("작업요청 응답 이상 [%d=%s]"), nResultCode, CLib::GetHostResultSting(nResultCode));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
				return;
			}
			
			strLog.Format(_T("작업요청 응답 수신 [작업번호=%d]"), nLuggNum);
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
		}
		break;
		
	default:
		{
			CString strLog;
			strLog.Format("에러.. Unknown MsgType=[%c]", ucMsgType);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
			//			LOG_ERROR(LOG_POS_HOST, nLuggNum, strLog);
		}
		break;
		
	}
*/
	
}

void CHostCl::UpdateCommCl(char Status)
{
	::PostMessage(m_pDoc->m_hWndView, WM_USER_HOST_NOTIFY, 1, Status);
}

BOOL CHostCl::Complete()
{
	
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchByStatus(enJobStatusCompleteRequest);
	if (pJobItem == NULL) 
		return FALSE;
	
	return Complete(pJobItem->m_nLuggNum);
}

BOOL CHostCl::CompleteAuto()
{
	
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchByStatus(enJobStatusRefuseComplete);
	if (pJobItem == NULL) 
		return FALSE;
	
	return Complete(pJobItem->m_nLuggNum);
}

BOOL CHostCl::Complete(int nLuggNum, int nScJobType, int nStation, BOOL bDelete, BOOL bManual)
{
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
	CString strLog;
	if (pJobItem == NULL)
	{
		strLog.Format(_T("작업번호를 찾을수 없슴.."));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Error"), pJobItem);
		return FALSE;
	}
	
	int nStep = 1;				// C/V 완료
	
	char TxBuff[256];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	
	switch (CJobItem::ConvertJobTypeToPattern(pJobItem->m_nJobType))
	{
	case	enJobPatternSto:
		nScJobType = 1;		// 입고완료
		nStep = 0;
		break;
	case	enJobPatternRet:
	case	enJobPatternPR:
		nScJobType = 2;		// 출고완료
		if (pJobItem->m_nJobStatus == enJobStatusScComplete)
			nStep = 0;		// S/C 완료
		break;
	case	enJobPatternMove:
		nScJobType = 3;		// 이동완료
		break;
	}
	
	if(nStation == 0)
		nStation = atoi(pJobItem->m_strDestStn);
	
	sprintf(TxBuff, "ECS_MBX   %04d ", MSG_LENGTH_COMPLETE_DATA);
	
	sprintf(TxBuff + MSG_LENGTH_HEADER, "%c%c%d%04d%d%d%03d%c",	
		STX, enHostCommandComplete, pJobItem->m_nJobType, nLuggNum, nScJobType, nStep, nStation, ETX);
	
	if(	SendMsg(TxBuff, MSG_LENGTH_COMPLETE_MSG) == TRUE )
	{
		strLog.Format("%s 작업 완료 보고 .. JobType=[%s]  ScJobType=[%d] StepCount=[%d]",  (bManual) ? "수동" : "", pJobItem->GetJobTypeString(), nScJobType, nStep);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Complete"), pJobItem);
		
		if ( pJobItem->m_nJobType == enJobTypeAutoSto ||
			pJobItem->m_nJobType == enJobTypeAutoMove ||
			pJobItem->m_nJobType == enJobTypeAutoR2R ||
			(pJobItem->m_nJobType == enJobTypeAutoPR && nStep == 1) ||
			(pJobItem->m_nJobType == enJobTypeAutoRet && nStep == 1) )
		{
			if (bDelete == TRUE)
				pJobItem->m_nJobStatus = enJobStatusCompleteRequest;
		}
	}
	else
	{
		strLog.Format("%s 작업 완료 보고 실패.. 에러=[%s]",  (bManual) ? "수동" : "", CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Complete"), pJobItem);
		pJobItem->m_nJobStatus = enJobStatusRefuseComplete;
		return FALSE;
	}
	
	return TRUE;
}

BOOL CHostCl::ErrorScReport(CJobItem* pJobItem, EN_ERROR_GBN enErrorGbn)
{
	if (pJobItem == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}
	
	if (IsConnect() == FALSE)
	{
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, _T("통신연결 않됨"), _T("CHostCl::ErrorScReport"), pJobItem);
		return FALSE;		
	}
	
	int nScNum = CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc);
	int nErrorCode = (enErrorGbn == enErrorDualStore)? enScErrorDualStore : enScErrorEmptyRetrieve;
	int nErrorKind = (enErrorGbn == enErrorDualStore)? 1 : 3;
	
	Error(1, nScNum, nErrorKind, nErrorCode, pJobItem->m_nLuggNum, pJobItem->m_strDestLoc );
	
	CString strLog;
	strLog.Format(_T("에러보고 [%s]"), (enErrorGbn == enErrorDualStore) ? _T("이중입고") : _T("공출고"));
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::ErrorScReport"), pJobItem);
	
	return TRUE;
}


void CHostCl::Error(int nDevKind, int nDevNo, int nErrKind, WORD wErrCode, int nLuggNum, CString strLocation, BOOL bManual)
{
	CJobItem *pJobItem = NULL;
	CString strLog;
	if (nLuggNum && nErrKind)
	{
		pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
		if (pJobItem == NULL)
		{
			strLog.Format(_T("작업번호를 찾을 수 없습니다."));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Error"), pJobItem);
			return;
		}
	}
	
	int	nBank, nBay, nLevel;
	if ( strLocation.IsEmpty() )
	{
		nBank = nBay = nLevel = 0;
	}
	else
	{
		nBank  = CLib::GetBank(strLocation);
		nBay   = CLib::GetBay(strLocation);
		nLevel = CLib::GetLevel(strLocation);
	}
	
	char TxBuff[256];
	int nErrorCode = wErrCode;
//	if (nErrorCode >= 0 && nErrorCode < 1000 )
//		nErrorCode = wErrCode;
//	else
//		nErrorCode = 0;
	
	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "ECS_MBX   %04d ", MSG_LENGTH_ERROR_DATA);	//23
	sprintf(TxBuff + MSG_LENGTH_HEADER, "%c%c%d%03d%d%04d%04d%02d%03d%02d%c",
		STX, enHostCommandError, nDevKind, nDevNo, nErrKind, nErrorCode, nLuggNum, nBank, nBay, nLevel, ETX);

	if ((nLuggNum > 0) && (nLuggNum < 9000))
	{
		if(	SendMsg(TxBuff, MSG_LENGTH_ERROR_MSG) == TRUE )
		{
			strLog.Format("%s 에러 보고 송신.. DevKind=[%d] DevNo=[%d] ErrKind=[%d] LuggNum=[%d] ErrorCode=[%d]",
				(bManual) ? "수동" : "", nDevKind, nDevNo, nErrKind, nLuggNum, nErrorCode);
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Error"), pJobItem);
		}
		else
		{
			strLog.Format("%s 에러 보고 송신 실패.. 에러=[%s]",  (bManual) ? "수동" : "", CLib::GetSystemErrMsg());
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Error"), pJobItem);
			
			if (nLuggNum && nErrKind && pJobItem)
				pJobItem->m_nJobStatus = enJobStatusRefuseError;
		}	
	}
}

void CHostCl::Arrived(int nLuggNum, BOOL bManual)
{
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
	CString strLog;
	if (pJobItem == NULL)
	{
		strLog.Format(_T("작업번호를 찾을 수 없습니다."));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Arrived"), pJobItem);
		return;
	}
	
	COleDateTime t = COleDateTime::GetCurrentTime();
	char TxBuff[MSG_LENGTH_ARRIVAL_MSG + 1];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS2F01FMS     1501%04d%02d%02d%02d%02d%02d%04d%06d:%d%03d%6s*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			pJobItem->m_nResultCode, MSG_LENGTH_ARRIVAL_DATA, pJobItem->m_nJobType, "000", pJobItem->m_strBarcode);
	
	if (SendMsg(TxBuff, MSG_LENGTH_ARRIVAL_MSG) == TRUE)
	{
		CString strLog;
		strLog.Format("%s 작업대 도착.. JobType=[%s] Station=[%s]",  
			(bManual) ? "수동" : "", pJobItem->GetJobTypeString(), pJobItem->m_strDestStn.Right(3));
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Arrived"), pJobItem);
		
//		if ( pJobItem->m_nJobType == enJobTypeAutoRet )
//		{
//			m_pDoc->m_pJob->m_nResultCount[pJobItem->GetStackerNum()]++;
//			::PostMessage(m_pDoc->m_hwndView, WM_USER_JOB_NOTIFY, JOB_RESULT_DRAW, 0);
//		}
		
		if (pJobItem->m_nJobType == enJobTypeAutoRet || 
			pJobItem->m_nJobType == enJobTypeAutoPR || 
			pJobItem->m_nJobType == enJobTypeAutoMove )
			m_pDoc->m_pJob->Remove(pJobItem);
	}
	else
	{
		CString strLog;
		strLog.Format("%s 작업대 도착 보고 실패",  (bManual) ? "수동" : "");
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Arrived"), pJobItem);
		pJobItem->m_nJobStatus = enJobStatusRefuseArrived;
	}
}

BOOL CHostCl::BarcodeData(CString strStation, CString strBarcodeData, BOOL bManual)
{
//	char TxBuff[1024];
	int nUserData = 0;
	CString strLog;
	
//	memset(TxBuff, 0x0, sizeof(TxBuff));
	
//	sprintf(TxBuff,						"ECS_MBX   %04d ",	MSG_LENGTH_BARCODE_DATA);
//	sprintf(TxBuff + MSG_LENGTH_HEADER, "%c%c%3s%-31s%d%c",	STX, enHostCommandBarcode, strStation.Mid(2, 3), strBarcodeData, nUserData, ETX);
	
//	int nLenth =  sizeof(TxBuff);

	COleDateTime t = COleDateTime::GetCurrentTime();
	CStationInfo * pStation = m_pDoc->GetStationInfo(strStation);
	char TxBuff[MSG_LENGTH_ARRIVAL_MSG + 1];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "@FM03eECS2F01FMS     1501%04d%02d%02d%02d%02d%02d%04d%06d:%d%3s%6s*;", 
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),
			0, MSG_LENGTH_ARRIVAL_DATA, pStation->m_enKind, pStation->m_strID, strBarcodeData);
	
	if (SendMsg(TxBuff, MSG_LENGTH_ARRIVAL_MSG) == TRUE)
	{
		strLog.Format("BarCode Data %s 보고.. 데이터=[%s], Station=[%s]",  (bManual) ? "수동" : "", strBarcodeData, strStation);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::BarcodeData"));
	}
	else
	{
		strLog.Format("BarCode Data %s 보고 실패.. 데이터=[%s], Station=[%s]", (bManual) ? "수동" : "", strBarcodeData, strStation);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::BarcodeData"));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CHostCl::ThsData(CString strStation, CString strThsData, BOOL bManual)
{
	char TxBuff[1024];
	int nUserData = 0;
	CString strLog;
	
	memset(TxBuff, 0x0, sizeof(TxBuff));
	
	sprintf(TxBuff,						"ECS_MBX   %04d ",	MSG_LENGTH_BARCODE_DATA);
	sprintf(TxBuff + MSG_LENGTH_HEADER, "%c%c%3s%-31s%d%c",	STX, enHostCommandBarcode, strStation.Mid(2, 3), strThsData, nUserData, ETX);
	
	int nLenth =  sizeof(TxBuff);
	
	if(	SendMsg(TxBuff, MSG_LENGTH_BARCODE_MSG) == TRUE )
	{
		strLog.Format("BarCode Data %s 보고.. 데이터=[%s], Station=[%s]",  (bManual) ? "수동" : "", strThsData, strStation);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::BarcodeData"));
	}
	else
	{
		strLog.Format("BarCode Data %s 보고 실패.. 데이터=[%s], Station=[%s]", (bManual) ? "수동" : "", strThsData, strStation);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::BarcodeData"));
		return FALSE;
	}
	
	return TRUE;
}

void CHostCl::Status(int nRegularCycle)
{
	
#define	SC_STATUS_NORMAL_WAIT			0
#define	SC_STATUS_STORING				1
#define	SC_STATUS_RETRIEVING			2
#define	SC_STATUS_RACK_TO_RACK			3
#define	SC_STATUS_NO_ONLINE				4
#define	SC_STATUS_ERROR					5
#define	SC_STATUS_SUSPEND_STO			6
#define	SC_STATUS_SUSPEND_RET			7
#define	SC_STATUS_SUSPEND_ALL			8
	
	int nCvStatus = 1, nScStatus = 1, nStoHSStatus = 1, nRetHSStatus = 1;
	
	CString strLog;
	char TxBuff[512];
	static	char	PrevBuff[512] = {0};
	memset(TxBuff, 0x0, sizeof(TxBuff));
	sprintf(TxBuff, "ECS_MBX   %04d ", MSG_LENGTH_STATUS_DATA);
	TxBuff[MSG_LENGTH_HEADER] = STX;
	TxBuff[MSG_LENGTH_HEADER+1] = enHostCommandStatus;
	
	return;

	for (int i = 0; i < SC_PLC_CNT; i++)
	{
		CSc* pSc = (CSc*) m_pDoc->GetEquipment(CEquipment::enSC, i + 1);	
		DEBUGER_ASSERT_VALID(pSc != NULL);
		
		CScInfo *pScInfo = (CScInfo *) pSc->GetInfo();
		DEBUGER_ASSERT_VALID(pScInfo != NULL);
		
		if (!pScInfo->m_bInvoke && pScInfo->m_ucRcStatus == CScInfo::enActive)
			nScStatus = SC_STATUS_NORMAL_WAIT;
		
		if (pScInfo->m_bInvoke)
		{
			if (pScInfo->m_nInternalJobType == CScInfo::enRetrieve)
				nScStatus = SC_STATUS_RETRIEVING;
			else if (pScInfo->m_nInternalJobType == CScInfo::enStore)
				nScStatus = SC_STATUS_STORING;
			else if (pScInfo->m_nInternalJobType == CScInfo::enRack2Rack)
				nScStatus = SC_STATUS_RACK_TO_RACK;
		}
		

		if (pScInfo->m_ucRcMode != CScInfo::enOnline)
			nScStatus = SC_STATUS_NO_ONLINE;
		if (pScInfo->m_ucScMode != CScInfo::enAuto)
			nScStatus = SC_STATUS_NO_ONLINE;
//		if (pScInfo->m_ucScMode != CScInfo::enActive)
//			nScStatus = SC_STATUS_NO_ONLINE;
		if (pScInfo->m_ucScStatus == CScInfo::enError)
			nScStatus = SC_STATUS_ERROR;
		if (pScInfo->m_bStoSuspend)
			nScStatus = SC_STATUS_SUSPEND_STO;
		if (pScInfo->m_bRetSuspend)
			nScStatus = SC_STATUS_SUSPEND_RET;
		if (pScInfo->m_bStoSuspend && pScInfo->m_bRetSuspend)
			nScStatus = SC_STATUS_SUSPEND_ALL;
		
		int nLugg = pScInfo->m_nInternalLuggNum;
//		nScStatus = 0;

//		// TEST
//		if(i % 2 == 0)
//		{
//			nScStatus = 3;
//			nLugg = 2222;
//		}
//		else
//		{
//			nScStatus = 2;
//			nLugg = 3333;		
//		}
		
		sprintf(TxBuff + MSG_LENGTH_HEADER + 2 + (7 * i), "%02d%d%04d",
			i+1, nScStatus, nLugg);
	}
	
	CStationInfo* pStation = NULL;
	int nStnCnt = 0;
	for (i=0; i<m_pDoc->m_pStationInfos.GetSize(); ++i)
	{
		pStation = m_pDoc->m_pStationInfos[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);
		
		if (pStation->m_enKind != CStationInfo::enScStation)
		{
			BOOL bStatus = FALSE;

			// 입고대 상태만을 올려주는 것으로 합의함!!			- 2012.09.18		- 이길문 대리 
			switch(pStation->m_enKind)
			{
			case CStationInfo::enStoStation: 	bStatus	= pStation->m_pTrack->IsStoStationReady();	break;	
//			case CStationInfo::enRetStation: 	bStatus	= pStation->m_pTrack->IsStoStationReady();	break;	
			case CStationInfo::enArvStation: 	bStatus	= pStation->m_pTrack->IsStoStationReady();	break;	
			}
			
			sprintf(TxBuff + MSG_LENGTH_HEADER + 2 + (7 * SC_PLC_CNT) + nStnCnt, "%d", bStatus);
			nStnCnt++;
		}
	}
	
	/*
	int StoStations[] = { 111, 110, 113, 116, 119, 214, 211, 215, 314, 311, 315 };
	BOOL bTemp;
	for (i = 0; i < IMS_STN_CNT; i++)
	{
		CTrackInfo* pTrack = m_pDoc->GetTrackInfo(StoStations[i]);
		DEBUGER_ASSERT_VALID(pTrack != NULL);
	
		bTemp = pTrack->IsStoStationReady();
	  
//		// TEST
//		if(i % 2 == 1)
//		{
//			bTemp = 1;
//		}

//		if(pTrack->m_nNumber == 111)
//			bTemp = 1;
		  
		sprintf(TxBuff + MSG_LENGTH_HEADER + 2 + (7 * SC_PLC_CNT) + i, "%d", bTemp);
	}
	*/

	TxBuff[MSG_LENGTH_HEADER + 2 + (7 * SC_PLC_CNT) + nStnCnt] = ETX;
	
	int rt = memcmp(PrevBuff, TxBuff, MSG_LENGTH_STATUS_MSG);
	if(	SendMsg(TxBuff, MSG_LENGTH_STATUS_MSG) == FALSE )
	{
		strLog.Format("상태 보고 송신 실패.. 에러=[%s]", CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Status"));
	}
	else 
	{
		strLog.Format("상태 보고 .. [%s][%s]", TxBuff, CLib::GetSystemErrMsg());
//		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostCl::Status"));
		memcpy(PrevBuff, TxBuff, MSG_LENGTH_STATUS_MSG);
	}
}

void CHostCl::JobRequest(int nStationNo, int nJobKind, BOOL bManual)
{
	if (nJobKind != 1 && nJobKind != 2)
		return; 

	char TxBuff[256];
	memset(TxBuff, 0x20, sizeof(TxBuff));
	CString strLog;
	
	sprintf(TxBuff, "ECS_MBX   %04d ", MSG_LENGTH_PALLET_DATA);//64
	sprintf(TxBuff + MSG_LENGTH_HEADER, "%c%c%d%03d%c%c",
		STX, CMD_STORE_REQUEST, nJobKind, nStationNo, 0x20, ETX);
	
	if(	SendMsg(TxBuff, MSG_LENGTH_PALLET_MSG) == FALSE )
	{
		strLog.Format("%s 공파레트 %s 요청 송신 실패.. 에러=[%s]", 
			(bManual) ? "수동" : "", 
			(nJobKind == 1) ? "입고" : ((nJobKind == 2) ? "출고" : "이동"), 
			CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::JobRequest"));
		return;
	}
	
	strLog.Format("%s 공파레트 %s 요청 송신.. Station=[%d] [%s] ", 
		(bManual) ? "수동" : "", 
		(nJobKind == 1) ? "입고" : ((nJobKind == 2) ? "출고" : "이동"), 
		nStationNo, 
		TxBuff);
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::JobRequest"));
}

int CHostCl::GetClientSeqNum()
{
	if (m_nClientSeqNum > 999999 || m_nClientSeqNum < 1)
		m_nClientSeqNum = 1;
	
	return  m_nClientSeqNum ++;
}

BOOL CHostCl::SendMsg(char* pTxBuff, int nSndMsgLen)
{
	
	if (m_pDoc->m_pHostCl != NULL)
	{
		UpdateCommCl(NOTIFY_SEND);
		int nTemp = Send(pTxBuff, nSndMsgLen);
		BOOL bTemp = (nTemp == nSndMsgLen);
//		return (nTemp == nSndMsgLen) ? TRUE : FALSE;
		return bTemp;
	}
	
	return FALSE;
}
