//Cv.cpp:implementation of the CCv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "Cv.h"
#include "EcsDoc.h"
#include "ByteArrayEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEVICE m_port.m_strDevice

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CCv, CEquipment)

CCv::CCv(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice) : CEquipment(pDoc, nIndex, nNumber, lpszDevice)
{
	m_enKind = CEquipment::enCV;
	m_pInfo = new CCvInfo(this);

	int i = 0 ;
	for (i = 0 ; i < 5 ; i++) 
	{
		m_bStoreRequesting[i] = FALSE;
	}
	
	for (i = 0 ; i < 3 ; i++) 
	{
		m_bRetrieveRequesting[i] = FALSE;
	}

	for (i = 0 ; i < 7 ; i++) 
	{
		m_bStorePLTRequesting[i] = FALSE;
	}

	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
}

CCv::~CCv()
{
	delete m_pInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCv::MulticastInfo(CMonitorServer* pMonitorSv)
{
	CString strSend;
	strSend.Format(_T("%c<ECS><EQUIP DEVICE='%s'>%s</EQUIP></ECS>%c"), 
		CMonitorServer::enSTX, m_port.m_strDevice, m_pInfo->GetXmlString(), CMonitorServer::enETX
	);

//==========================================================================================================	
//	// PlayBack을 위해서 
//==========================================================================================================
//	m_pInfo->m_pEquipment->m_pDoc->m_pLogPlayback->Log(strSend);
//----------------------------------------------------------------------------------------------------------

//	if (pMonitorSv)
//	{
//		pMonitorSv->SendData(strSend);
//	}
//	else
//	{
//		if (m_pDoc->m_pMonitorListener && m_pDoc->m_pMonitorListener->GetClientCount())
//			m_pDoc->m_pMonitorListener->SendToClients(strSend);
//	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCv::AutoRunProc()
{
	UpdateCommStatus(NOTIFY_SEND);
	if (ReadStatus())
	{
		CTrackInfo* pTrack = NULL;
		CTrackHS* pTrackHS = NULL;
		CStationInfo* pStation = NULL;

		// 생산 팔레트 정보를 강제로 적는 부분 
		if (m_pDoc->m_pConfig->m_bViewProductInfoInputer == TRUE)
		{
			if (m_pDoc->m_bProdInfo == TRUE && m_pDoc->m_nProdPlc == m_pInfo->m_pEquipment->m_nNumber)
			{
				BOOL bTemp1 = WriteWordValue(m_pDoc->m_nProdAddr,		m_pDoc->m_nProdDate);
				BOOL bTemp2 = WriteWordValue(m_pDoc->m_nProdAddr + 1,	m_pDoc->m_nProdCount);

				if (bTemp1 == TRUE && bTemp2 == TRUE)
				{
					m_pDoc->m_nProdAddr = 0;
					m_pDoc->m_nProdDate = 0;
					m_pDoc->m_nProdCount = 0;
					m_pDoc->m_bProdInfo = FALSE;
					m_pDoc->m_nProdPlc = 0;
				}
			}
		}


		for (int i=0; i<m_pInfo->m_pTracks.GetSize(); ++i)
		{
			pTrack = m_pInfo->m_pTracks[i];
			DEBUGER_ASSERT_VALID(pTrack != NULL);
			
			if (!pTrack->IsAutoMode() || pTrack->IsNotInvokeTrack() || pTrack->m_bSuspend)
				continue;

			if ((pStation = pTrack->GetStoStation()) || (pStation = pTrack->GetArvStation()))
				InvokeStoStation(pTrack, pStation);

			if ((pStation = pTrack->GetRetStation()) || (pStation = pTrack->GetArvStation()))
				InvokeRetStation(pTrack, pStation);

			if (pTrackHS = pTrack->GetScRetHS())
				InvokeScRetHS(pTrack, pTrackHS);

			if (pTrackHS = pTrack->GetRtvArriveHS())
				InvokeRtvArriveHS(pTrack, pTrackHS);

			if (pTrack->m_nNumber == 3117 || pTrack->m_nNumber == 3217 || pTrack->m_nNumber == 3317 )
			{
				InvokeWeight(pTrack);	
			}
			
			// 도착완료 보내는 작업
			if (pTrack->m_nNumber == 3111 || 
				pTrack->m_nNumber == 3112 || 
				pTrack->m_nNumber == 3211 || 
				pTrack->m_nNumber == 3212 || 
				pTrack->m_nNumber == 3311 || 
				pTrack->m_nNumber == 3312 || 
				pTrack->m_nNumber == 3404 || 
				pTrack->m_nNumber == 3409 || 
				pTrack->m_nNumber == 3502 || 
				pTrack->m_nNumber == 1205 )
			{
				InvokeArriveReport(pTrack);				
			}
		}

		UpdateCommStatus(NOTIFY_REFRESH);		//NOTIFY_RECEIVE
	}
	else
	{
		UpdateCommStatus(NOTIFY_ERROR);
	}


	::Sleep(100);
}

void CCv::CommandProc()
{
	CString strLog;
	CCmdMsg* pCmdMsg = NULL;
	for (POSITION pos=m_cmdList.GetHeadPosition(); pos!=NULL; )
	{
		pCmdMsg = (CCmdMsg*)m_cmdList.GetNext(pos);
		CStringArray& strValues = pCmdMsg->m_strValues;
		DEBUGER_ASSERT_VALID(pCmdMsg != NULL);

		int nTemp1 = strValues.GetSize();
		int	nTemp2 = _ttoi(strValues[0]);

		switch (pCmdMsg->m_nCommand)
		{
		case CCmdMsg::enCmdTrackSuspend:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeTrackSuspend);
			if (WriteSuspend(_ttoi(strValues[0]), _ttoi(strValues[1])))
			{
				strLog.Format(_T("%s TR%s 일시정지 [SET=%s]"), DEVICE, strValues[0], strValues[1]);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s TR%s 일시정지 실패! [SET=%s]"), DEVICE, strValues[0], strValues[1]);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;

		case CCmdMsg::enCmdTrackDeadLock:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeTrackDeadLock);
			if (WriteDeadLock(_ttoi(strValues[0]), _ttoi(strValues[1])))
			{
				strLog.Format(_T("%s TR%s 무게 측정 요청 [SET=%s]"), DEVICE, strValues[0], strValues[1]);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s TR%s 무게 측정 요청 실패! [SET=%s]"), DEVICE, strValues[0], strValues[1]);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;

		case CCmdMsg::enCmdTrackDeadLockCheck:
			{
				DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeTrackDeadLockCheck);
				
				int nError = (_ttoi(strValues[1]) > 0) ? 6 : 0;
				
				if (WriteError(_ttoi(strValues[0]), nError))
				{
					if (_ttoi(strValues[0]) == 3503)
						strLog.Format(_T("%s TR%s SIZE CHECKER ERROR 해제 요청 [SET=%s]"), DEVICE, strValues[0], strValues[1]);
					else
						strLog.Format(_T("%s TR%s 무게 에러 요청 [SET=%s]"), DEVICE, strValues[0], strValues[1]);
					WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
				}
				else
				{
					if (_ttoi(strValues[0]) == 3503)
						strLog.Format(_T("%s TR%s SIZE CHECKER ERROR 해제 요청 [SET=%s]"), DEVICE, strValues[0], strValues[1]);
					else
						strLog.Format(_T("%s TR%s 무게 에러 요청 실패! [SET=%s]"), DEVICE, strValues[0], strValues[1]);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
				}
			}
			break;

		case CCmdMsg::enCmdTrackWrite:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeTrackWrite);
			if (WriteTrackInfo(_ttoi(strValues[0]), _ttoi(strValues[1]), _ttoi(strValues[2]), _ttoi(strValues[3]), _ttoi(strValues[4]), _ttoi(strValues[5])))
			{
				CString strProductID = "";
				CJobItem* pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValues[1]));
				if (pJobItem != NULL)
				{
					pJobItem->SetJobStatus(enJobStatusCvInvoke);
					m_pDoc->m_pJob->Backup();
					strProductID = pJobItem->m_strProductID;
				}

				CTrackInfo* pTrack = m_pDoc->GetTrackInfo(_ttoi(strValues[0]));
				if (pTrack != NULL && (pTrack->m_nLuggNum != 0 || pTrack->m_nJobType != 0 || pTrack->m_nDestPos != 0))
				{
					strLog.Format(_T("%s 기존 데이터 [TR%03d: LuggNum=%d, JobType=%d, DestPos=%d] 있는데 데이터 기록함!"), 
						DEVICE, pTrack->m_nNumber, pTrack->m_nLuggNum, pTrack->m_nJobType, pTrack->m_nDestPos);
					WriteLog(LOG_TYPE_EVENT, pTrack->m_nLuggNum, strLog, _T("CCv::CommandProc"), strProductID);
					
				}
				
				strLog.Format(_T("%s TRACK DATA WRITE [TR%03d: LuggNum=%s, JobType=%s, DestPos=%s, Size=%s]"), 
					DEVICE, _ttoi(strValues[0]), strValues[1], strValues[2], strValues[3], strValues[5]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[1]), strLog, _T("CCv::CommandProc"), strProductID);
			}
			else
			{
				strLog.Format(_T("%s TRACK DATA WRITE 실패! [TR%03d: LuggNum=%s, JobType=%s, DestPos=%s] [%s]"), 
					DEVICE, _ttoi(strValues[0]), strValues[1], strValues[2], strValues[3], m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ALARM, _ttoi(strValues[1]), strLog, _T("CCv::CommandProc"));
			}
			break;

		case CCvCmdMsg::enCmdDoorOpenAccept:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
			if (WriteDoorOpenAccept(_ttoi(strValues[0])))
			{
				strLog.Format(_T("%s DOOR OPEN ACCEPT [DOOR%d]"), DEVICE, _ttoi(strValues[0])+1);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s DOOR OPEN ACCEPT 실패! [DOOR%d] [%s]"), DEVICE, _ttoi(strValues[0])+1, m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;
		case CCvCmdMsg::enCmdDoorOpenReq:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
			if (WriteDoorOpenReq(_ttoi(strValues[0])))
			{
				strLog.Format(_T("%s DOOR OPEN REQUEST [DOOR%d]"), DEVICE, _ttoi(strValues[0])+1);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s DOOR OPEN REQUEST 실패! [DOOR%d] [%s]"), DEVICE, _ttoi(strValues[0])+1, m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;
/*
		case CCvCmdMsg::enCmdTurnPermissionRequest:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
			if (WriteInPermissionRequest(_ttoi(strValues[0])))
			{
				strLog.Format(_T("%s 턴테이블 진입요청 [DOOR%d]"), DEVICE, _ttoi(strValues[0])+1);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 턴테이블 진입요청 [DOOR%d] [%s]"), DEVICE, _ttoi(strValues[0])+1, m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;
*/
		case CCvCmdMsg::enCmdDoorClose:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
			if (WriteDoorClose(_ttoi(strValues[0])))
			{
				strLog.Format(_T("%s DOOR CLOSE [DOOR%d]"), DEVICE, _ttoi(strValues[0])+1);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s DOOR CLOSE 실패! [DOOR%d] [%s]"), DEVICE, _ttoi(strValues[0])+1, m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;

		case CCvCmdMsg::enCmdDoorStatus:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
			if (WriteDoorStatus(_ttoi(strValues[0])))
			{
				strLog.Format(_T("%s DOOR STATUS 전송 [STATUS=%x]"), DEVICE, _ttoi(strValues[0]));
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s DOOR STATUS 전송 실패! [STATUS=%x] [%s]"), DEVICE, _ttoi(strValues[0]), m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;
/*
		case CCvCmdMsg::enCmdTurnPermission:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
			if (WriteInPermission(_ttoi(strValues[0])))
			{
				strLog.Format(_T("%s 턴테이블 동작 허가 전송 [STATUS=%x]"), DEVICE, _ttoi(strValues[0]));
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 턴테이블 동작 허가 전송 실패! [STATUS=%x] [%s]"), DEVICE, _ttoi(strValues[0]), m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;
*/
		default:
			strLog.Format(_T("%s INVLID COMMAND!"), DEVICE);
			WriteLog(LOG_TYPE_ALARM, LOG_SYSTEM, strLog, _T("CCv::CommandProc"));
		}

//		if(pCmdMsg->m_nCommand >0)
//		{
			delete pCmdMsg;
			pCmdMsg = NULL;
//		}
	}

//	delete pCmdMsg;
//	pCmdMsg = NULL;

	m_cmdList.RemoveAll();
}

BOOL CCv::WriteSuspend(int nTrackNum, BOOL bSuspend)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

//	int nTrackWordSize = 10, nWriteWordSize = 1;
//	int nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;
//	int nStartWordAddr = 15 + (nTrackNum - nStartTrackNum) * nTrackWordSize;
//	DEBUGER_ASSERT_VALID(m_pInfo->m_pTracks[0] != NULL);
//	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	int nTrackWordSize = 10, nWriteWordSize = 1;
	int nStartTrackNum = 0;		// m_pInfo->m_pTracks[0]->m_nNumber;
	int nStartWordAddr = 0;		// 10 + (nTrackNum - nStartTrackNum) * nTrackWordSize;
	int nRealTrackNum = (nTrackNum % 1000);
	nStartWordAddr = (nRealTrackNum * nTrackWordSize) + 5;
	
	CTrackInfo* pTrack = m_pDoc->GetTrackInfo(nTrackNum);
	DEBUGER_ASSERT_VALID(pTrack != NULL);

	CByteArrayEx arrBuffer(nStartWordAddr, nWriteWordSize);
//	arrBuffer.SetNibble(nStartWordAddr, enNibble1, (BYTE)bSuspend);
//	arrBuffer.SetNibble(nStartWordAddr, enNibble2, 0x00);
//	arrBuffer.SetNibble(nStartWordAddr, enNibble3, 0x00);
//	arrBuffer.SetNibble(nStartWordAddr, enNibble4, (BYTE)pTrack->m_bDeadLock);

	arrBuffer.SetBit(nStartWordAddr, enBit11, bSuspend);
	arrBuffer.SetBit(nStartWordAddr, enBit12, pTrack->m_bDeadLock);
	
//	WORD wTemp = 0x0000;
//	CLib::SetBit(wTemp, 0, bSuspend);
//	CLib::SetBit(wTemp, 1, pTrack->m_bDeadLock);

	return m_pSocket->WriteWord(arrBuffer, nStartWordAddr, nWriteWordSize, enDeviceCodeR);
//	return WriteWordValue(nStartWordAddr, wTemp);
}

BOOL CCv::WriteDeadLock(int nTrackNum, BOOL bDeadLock)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

//	int nTrackWordSize = 10, nWriteWordSize = 1;
//	int nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;
//	int nStartWordAddr = 15 + (nTrackNum - nStartTrackNum) * nTrackWordSize;
//	DEBUGER_ASSERT_VALID(m_pInfo->m_pTracks[0] != NULL);
//	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	int nTrackWordSize = 10, nWriteWordSize = 1;
	int nStartTrackNum = 0;		// m_pInfo->m_pTracks[0]->m_nNumber;
	int nStartWordAddr = 0;		// 10 + (nTrackNum - nStartTrackNum) * nTrackWordSize;
	int nRealTrackNum = (nTrackNum % 1000);
	nStartWordAddr = (nRealTrackNum * nTrackWordSize) + 5;
	
	CTrackInfo* pTrack = m_pDoc->GetTrackInfo(nTrackNum);
	DEBUGER_ASSERT_VALID(pTrack != NULL);

	CByteArrayEx arrBuffer(nStartWordAddr, nWriteWordSize);
//	arrBuffer.SetWord(nStartWordAddr, (WORD)bSuspend);
	
//	arrBuffer.SetNibble(nStartWordAddr, enNibble1, (BYTE)pTrack->m_bSuspend);
//	arrBuffer.SetNibble(nStartWordAddr, enNibble2, 0x00);
//	arrBuffer.SetNibble(nStartWordAddr, enNibble3, 0x00);
//	arrBuffer.SetNibble(nStartWordAddr, enNibble4, (BYTE)bDeadLock);

	arrBuffer.SetBit(nStartWordAddr, enBit11, pTrack->m_bSuspend);
	arrBuffer.SetBit(nStartWordAddr, enBit12, bDeadLock);

	return m_pSocket->WriteWord(arrBuffer, nStartWordAddr, nWriteWordSize, enDeviceCodeR);
}

BOOL CCv::WriteError(int nTrackNum, int nError)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

//	int nTrackWordSize = 10, nWriteWordSize = 1;
//	int nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;
//	int nStartWordAddr = 12 + (nTrackNum - nStartTrackNum) * nTrackWordSize;
//	DEBUGER_ASSERT_VALID(m_pInfo->m_pTracks[0] != NULL);
//	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	int nTrackWordSize = 10, nWriteWordSize = 1;
	int nStartTrackNum = 0;		// m_pInfo->m_pTracks[0]->m_nNumber;
	int nStartWordAddr = 0;		// 10 + (nTrackNum - nStartTrackNum) * nTrackWordSize;
	int nRealTrackNum = (nTrackNum % 1000);
	nStartWordAddr = (nRealTrackNum * nTrackWordSize) + 3;
	
	CTrackInfo* pTrack = m_pDoc->GetTrackInfo(nTrackNum);
	DEBUGER_ASSERT_VALID(pTrack != NULL);

	CByteArrayEx arrBuffer(nStartWordAddr, nWriteWordSize);
//	arrBuffer.SetWord(nStartWordAddr, (WORD)bSuspend);
	
	arrBuffer.SetByte(nStartWordAddr, enByteH, 0);
	arrBuffer.SetByte(nStartWordAddr, enByteL, (BYTE)nError);

	return m_pSocket->WriteWord(arrBuffer, nStartWordAddr, nWriteWordSize, enDeviceCodeR);
}

BOOL CCv::WriteTrackInfo(int nTrackNum, int nLuggNum, int nJobType, int nDestPos, int nDeadLockCheck, int nSize /* = 0 */)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	int nTrackWordSize = 10, nWriteWordSize = 5;
	int nStartTrackNum = 0;		// m_pInfo->m_pTracks[0]->m_nNumber;
	int nStartWordAddr = 0;		// 10 + (nTrackNum - nStartTrackNum) * nTrackWordSize;
	int nRealTrackNum = (nTrackNum % 1000);
	nStartWordAddr = nRealTrackNum * nTrackWordSize;

	CByteArrayEx arrBuffer(nStartWordAddr, nWriteWordSize);
	arrBuffer.SetWord(nStartWordAddr+0, nLuggNum);
	arrBuffer.SetWord(nStartWordAddr+1, nDestPos);
	arrBuffer.SetByte(nStartWordAddr+2, enByteL, nJobType);
	arrBuffer.SetByte(nStartWordAddr+2, enByteH, nDeadLockCheck);		///
	arrBuffer.SetWord(nStartWordAddr+3, nSize);

	return m_pSocket->WriteWord(arrBuffer, nStartWordAddr, nWriteWordSize, enDeviceCodeR);
}

BOOL CCv::WriteTrackInfo(CTrackInfo* pTrack, CJobItem* pJobItem, int nDestTrackNum /* = 0 */, int nDeadLockCheck /* = 0 */, int nSize /* = 0 */)
{
	CStationInfo* pDestStation = m_pDoc->GetStationInfo(pJobItem->m_strDestStn);
	int nTrackDest = (nDestTrackNum == 0) ? pDestStation->GetTrackDestination() : nDestTrackNum;
	DEBUGER_ASSERT_VALID(pDestStation != NULL);
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	return WriteTrackInfo(pTrack->m_nNumber, pJobItem->m_nLuggNum, pJobItem->m_nJobType, nTrackDest, nDeadLockCheck, nSize);
}

BOOL CCv::WriteDoorOpenReq(int nIndex)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

	WORD wDoorOpenReq = m_pInfo->m_wDoorOpenReq & 0x0001;
	CLib::SetBit(wDoorOpenReq, 0, TRUE);

	int nStartWorkAddr = 9902, nWriteWordSize = 2;
	CByteArrayEx arrBuffer(nStartWorkAddr, nWriteWordSize);
	arrBuffer.SetWord(nStartWorkAddr, wDoorOpenReq);

	CString strLog;
	strLog.Format(_T("%s DOOR OPEN REQUEST [D9901=%04X] [D9902: OLD=%04X, NEW=%04X]"), 
		DEVICE, m_pInfo->m_wDoorStatus, m_pInfo->m_wDoorOpenReq, wDoorOpenReq);
	WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::WriteDoorOpenReq"));

	return m_pSocket->WriteWord(arrBuffer, nStartWorkAddr, nWriteWordSize, enDeviceCodeR);
}

BOOL CCv::WriteDoorOpenAccept(int nIndex)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

	WORD wDoorOpenAccept = m_pInfo->m_wDoorOpenAccept & 0x0001;
	CLib::SetBit(wDoorOpenAccept, 0, TRUE);

	int nStartWorkAddr = 9903, nWriteWordSize = 2;
	CByteArrayEx arrBuffer(nStartWorkAddr, nWriteWordSize);
	arrBuffer.SetWord(nStartWorkAddr, wDoorOpenAccept);

	CString strLog;
	strLog.Format(_T("%s DOOR OPEN REQUEST [D9901=%04X] [D9903: OLD=%04X, NEW=%04X]"), 
		DEVICE, m_pInfo->m_wDoorStatus, m_pInfo->m_wDoorOpenAccept, wDoorOpenAccept);
	WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::WriteDoorOpenAccept"));

	return m_pSocket->WriteWord(arrBuffer, nStartWorkAddr, nWriteWordSize, enDeviceCodeR);
}

BOOL CCv::WriteDoorClose(int nIndex)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

	WORD wDoorClose = m_pInfo->m_wDoorClose & 0x0001;
	CLib::SetBit(wDoorClose, 0, FALSE);		// 완료의 개념이므로 FALSE

	int nStartWorkAddr = 9904, nWriteWordSize = 1;
	CByteArrayEx arrBuffer(nStartWorkAddr, nWriteWordSize);
	arrBuffer.SetWord(nStartWorkAddr, wDoorClose);

	CString strLog;
	strLog.Format(_T("%s DOOR CLOSE COMPLETE [D9901=%04X] [D9904: OLD=%04X, NEW=%04X]"), 
		DEVICE, m_pInfo->m_wDoorStatus, m_pInfo->m_wDoorClose, wDoorClose);
	WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::WriteDoorClose"));

	return m_pSocket->WriteWord(arrBuffer, nStartWorkAddr, nWriteWordSize, enDeviceCodeR);
}

BOOL CCv::WriteRgvStatus(BOOL bStop)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}

	WORD wRgvStatus = m_pInfo->m_wRgvStatus & 0x0001;

	CLib::SetBit(wRgvStatus, 0, bStop);		

	int nStartWorkAddr = 9905, nWriteWordSize = 1;
	CByteArrayEx arrBuffer(nStartWorkAddr, nWriteWordSize);
	arrBuffer.SetWord(nStartWorkAddr, wRgvStatus);

	CString strLog;
	strLog.Format(_T("%s Rgv Status Updated! [D9905: OLD=%04X, NEW=%04X]"), 
		DEVICE, m_pInfo->m_wRgvStatus, wRgvStatus);
	WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::WriteRgvStatus"));

	return m_pSocket->WriteWord(arrBuffer, nStartWorkAddr, nWriteWordSize, enDeviceCodeR);
}

BOOL CCv::WriteWordValue(int nAddress, WORD wWord, BOOL bOtherWay/* = FALSE*/)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}
	
	CByteArrayEx arrBuffer(nAddress, 1);
	arrBuffer.SetWord(nAddress, wWord);

	BYTE	TxBuff[4098];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = (BYTE)wWord;									
	
	if (bOtherWay == TRUE)
		return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 1);
	else
		return m_pSocket->WriteWord(arrBuffer, nAddress, 1, enDeviceCodeR);
}

/*
BOOL CCv::WritePLCWordValue(int nPlcNum, int nAddress, WORD wWord)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}
	
	CByteArrayEx arrBuffer(nAddress, 1);
	arrBuffer.SetWord(nAddress, wWord);
	
	return m_pSocket->WriteWord(arrBuffer, nAddress, 1);
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CCv::WritePlayBack()
{
	if(m_pDoc->m_bLogPlayback == FALSE)
		return;

	CString strTxBuff;

	for(POSITION pos = m_listCrane.GetHeadPosition(); pos != NULL; )
	{
		CString strTxData;

		CCrane* pCrane = (CCrane *)m_listCrane.GetNext(pos);

		strTxData += pCrane->GetXmlStringAll();
		strTxBuff.Format("%c<ECS><CRANE>%s</CRANE></ECS>%c", STX, strTxData, ETX);
		CSystem::m_pDoc->m_pLogPlayback->Log(strTxBuff);
	}
}
*/

BOOL CCv::ReadStatus()
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	CString strLog;
	CJobItem * pJobItem = NULL;
	CStationInfo* pStation = NULL;
	int nStartWordAddr = 10;
	int nReadWordSize = m_pInfo->m_pTracks.GetSize() * 10;
	CByteArrayEx arrBuffer(nStartWordAddr, nReadWordSize);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	switch(m_pInfo->m_pEquipment->m_nNumber) 
	{
	case 1:				nStartWordAddr = 1010;							break;
	case 2:				nStartWordAddr = 2010;							break;

	case 3:				nStartWordAddr = 1010;							break;
	case 4:				nStartWordAddr = 2010;							break;
	case 5:				nStartWordAddr = 9010;							break;

	case 6:				nStartWordAddr = 1010;							break;
	case 7:				nStartWordAddr = 2010;							break;
	case 8:				nStartWordAddr = 3010;							break;
	case 9:				nStartWordAddr = 4010;							break;
	case 10:			nStartWordAddr = 5010;							break;
	case 11:			nStartWordAddr = 9010;							break;

	}

	m_pSocket->m_bWriteLog = FALSE;
	arrBuffer.SetArray(nStartWordAddr, nReadWordSize);
	if (m_pSocket->ReadWord(arrBuffer, nStartWordAddr, nReadWordSize, enDeviceCodeR) == FALSE)
	{
		strLog.Format(_T("%s 트랙정보 읽기 실패! [%s]"), DEVICE, m_pSocket ? m_pSocket->m_strLog : m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::ReadStatus"));
		return FALSE;
	}
	m_pSocket->m_bWriteLog = m_port.m_bWriteLog;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

//	if(m_pInfo->m_pEquipment->m_nNumber != 1)
//		return TRUE;

	CTrackInfo* pTrack = NULL;
//===========================================================================================================
//	// 생산파레트 정보 
//===========================================================================================================
	
	BOOL bWeight = FALSE;
	BOOL bProductPLT = FALSE;
	BOOL bAddProdPLT = FALSE;
	WORD wTemp1 = 0;
	WORD wTemp2 = 0;
	int nEtcValue = 0;	
	int nTrackNum = 0;
	int nEtcStartWordAddr = 0, nEtcReadWordSize = 2;
	int nAddEtcStartWordAddr = 0, nAddEtcReadWordSize = 2;
	int nTempEtcReadWordSize = 0;
	if (m_pInfo->m_pEquipment->m_nNumber == 1 || 
		m_pInfo->m_pEquipment->m_nNumber == 3 || 
		m_pInfo->m_pEquipment->m_nNumber == 4 || 
		m_pInfo->m_pEquipment->m_nNumber == 6 || 
		m_pInfo->m_pEquipment->m_nNumber == 7 || 
		m_pInfo->m_pEquipment->m_nNumber == 8)
	{
		switch(m_pInfo->m_pEquipment->m_nNumber) 
		{
		case 6:		bWeight = TRUE;			nEtcStartWordAddr = 9921;		nTrackNum = 3117;			break;
		case 7:		bWeight = TRUE;			nEtcStartWordAddr = 9923;		nTrackNum = 3217;			break;
		case 8:		bWeight = TRUE;			nEtcStartWordAddr = 9925;		nTrackNum = 3317;			break;
		case 3:		bProductPLT = TRUE;		nEtcStartWordAddr = 9957;		nEtcReadWordSize = 4;		break;
		case 1:		bProductPLT = TRUE;		nEtcStartWordAddr = 9961;		nEtcReadWordSize = 2;		break;
		case 4:		bProductPLT = TRUE;		nEtcStartWordAddr = 9951;		nEtcReadWordSize = 6;	
					bAddProdPLT = TRUE;		nAddEtcStartWordAddr = 9963;	nAddEtcReadWordSize = 2;	break;
		}
		
		//CByteArrayEx arrBufferWeight(nEtcStartWordAddr, nEtcReadWordSize);
		
		if (nEtcStartWordAddr != 0)
		{
			CByteArrayEx arrBufferWeight(nEtcStartWordAddr, nEtcReadWordSize);

			m_pSocket->m_bWriteLog = FALSE;
			if (m_pSocket->ReadWord(arrBufferWeight, nEtcStartWordAddr, nEtcReadWordSize, enDeviceCodeR) == FALSE)
			{
				strLog.Format(_T("%s 중량 및 생산팔레트 정보 읽기 실패! [%s]"), DEVICE, m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::ReadStatus"));
				return FALSE;
			}
			m_pSocket->m_bWriteLog = m_port.m_bWriteLog;

			if (bWeight == TRUE)
			{

				wTemp1 = arrBufferWeight.GetWord(nEtcStartWordAddr);
				wTemp2 = arrBufferWeight.GetWord(nEtcStartWordAddr+1);

				pTrack = m_pDoc->GetTrackInfo(nTrackNum);
				DEBUGER_ASSERT_VALID(pTrack != NULL);

				nEtcValue = (wTemp1 * 1000) + wTemp2;
				pTrack->SetEtcValue(CConvert::ToString(nEtcValue));
			}
			else if (bProductPLT == TRUE)
			{
				int nTrackNums[3];
				//int nIndex = 0;
				if (nEtcReadWordSize == 4)
				{
					//nIndex = 3;
					nTrackNums[0] = 2119;
					nTrackNums[1] = 2118;
				}
				else if (nEtcReadWordSize == 6)
				{
					//nIndex = 0;
					nTrackNums[0] = 2210;
					nTrackNums[1] = 2229;
					nTrackNums[2] = 2236;
				}
				else //if (nEtcReadWordSize == 2)
				{
					//nIndex = 5;
					nTrackNums[0] = 1103;	
				}

				for (int i = 0, j = 0 ; i < nEtcReadWordSize ; i += 2, j++) 
				{
					wTemp1 = arrBufferWeight.GetWord(nEtcStartWordAddr + i);
					wTemp2 = arrBufferWeight.GetWord(nEtcStartWordAddr + i + 1);

					pTrack = m_pDoc->GetTrackInfo(nTrackNums[j]);
					DEBUGER_ASSERT_VALID(pTrack != NULL);

					nEtcValue = (wTemp1 * 10000) + wTemp2;
					CString strProductPLT;
					strProductPLT.Format(_T("%06d"), nEtcValue);
					pTrack->SetEtcValue(strProductPLT);
				}
			}
			
			bWeight = FALSE;
			bProductPLT = FALSE;
			nEtcStartWordAddr = 0;
		}

		if (nAddEtcStartWordAddr != 0)
		{
			CByteArrayEx arrBufferWeight(nAddEtcStartWordAddr, nAddEtcReadWordSize);

			m_pSocket->m_bWriteLog = FALSE;
			if (m_pSocket->ReadWord(arrBufferWeight, nAddEtcStartWordAddr, nAddEtcReadWordSize, enDeviceCodeR) == FALSE)
			{
				strLog.Format(_T("%s 중량 및 생산팔레트 정보 읽기 실패! 2[%s]"), DEVICE, m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::ReadStatus"));
				return FALSE;
			}
			m_pSocket->m_bWriteLog = m_port.m_bWriteLog;

			if (bAddProdPLT == TRUE)
			{
				int nTrackNums[1];
				//int nIndex = 6;
				nTrackNums[0] = 2203;	

				for (int i = 0, j = 0 ; i < nAddEtcReadWordSize ; i += 2, j++) 
				{
					wTemp1 = arrBufferWeight.GetWord(nAddEtcStartWordAddr + i);
					wTemp2 = arrBufferWeight.GetWord(nAddEtcStartWordAddr + i + 1);

					pTrack = m_pDoc->GetTrackInfo(nTrackNums[j]);
					DEBUGER_ASSERT_VALID(pTrack != NULL);

					nEtcValue = (wTemp1 * 10000) + wTemp2;
					CString strProductPLT;
					strProductPLT.Format(_T("%06d"), nEtcValue);
					pTrack->SetEtcValue(strProductPLT);
				}
			}
			
			//bWeight = FALSE;
			bAddProdPLT = FALSE;
			nAddEtcStartWordAddr = 0;
		}

		
	}
	//*/
//-----------------------------------------------------------------------------------------------------------

	int nWordAddr = 0;

	for (int i=0; i<m_pInfo->m_pTracks.GetSize(); ++i)
	{
		pTrack = m_pInfo->m_pTracks[i];
		DEBUGER_ASSERT_VALID(pTrack != NULL);
	
		nWordAddr = nStartWordAddr + i*10;
		pTrack->SetLuggNum(arrBuffer.GetWord(nWordAddr+0));
		pTrack->SetDestPos(arrBuffer.GetWord(nWordAddr+1));
		pTrack->SetJobType(arrBuffer.GetByte(nWordAddr+2, enByteL));
		pTrack->SetGenCode(arrBuffer.GetByte(nWordAddr+2, enByteH));
		pTrack->m_wReserved3 = arrBuffer.GetWord(nWordAddr+3);
		WORD wTemp = arrBuffer.GetWord(nWordAddr+5);
		BOOL bStatus1 = CLib::IsSet(arrBuffer.GetWord(nWordAddr+5), 0);
		pTrack->SetSuspend(CLib::IsSet(arrBuffer.GetWord(nWordAddr+5), 0));
		pTrack->SetDeadLock(CLib::IsSet(arrBuffer.GetWord(nWordAddr+5), 1));
		pTrack->SetErrorCode(arrBuffer.GetWord(nWordAddr+6));
	
		if (pTrack->m_nLuggNum != 0)
			int j=0;

//=============================================================================================================================
//		// 생산 파레트 요청을 한번만 요청하기
//=============================================================================================================================
		int	 nIndex = -1;
		BOOL bStatus = CLib::IsSet(arrBuffer.GetWord(nWordAddr+7), 1);
//		BOOL bStatus = CLib::IsSet(arrBuffer.GetWord(nWordAddr+8), 0);

		if (bStatus ^ pTrack->IsStoStationReady())
//		if (bStatus ^ pTrack->IsProductSensing())
//		if (pTrack->IsStoStationReady() == TRUE)
		{
			//strLog.Format(_T("%s 입고 Station Check0 [%s]"), DEVICE, pTrack->GetTrackString());
			//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));

			switch(pTrack->m_nNumber)
			{
			case	2210:		nIndex = 0;				break;	
			case	2229:		nIndex = 1;				break;
			case	2236:		nIndex = 2;				break;
			case	2119:		nIndex = 3;				break;
			case	2118:		nIndex = 4;				break;
			case	1103:		nIndex = 5;				break;
			case	2203:		nIndex = 6;				break;
			}
			
			if (nIndex >= 0 && nIndex <= 6)
			{
				//strLog.Format(_T("%s 입고 Station Check1 [%s]"), DEVICE, pTrack->GetTrackString());
				//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));

				if (bStatus == FALSE )
				{
					//strLog.Format(_T("%s 입고 Station Check2-1 [%s]"), DEVICE, pTrack->GetTrackString());
					//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));

					if (pTrack->m_nNumber == 2210 || 
						pTrack->m_nNumber == 2229 || 
						pTrack->m_nNumber == 2236 || 
						pTrack->m_nNumber == 2119 ||
						pTrack->m_nNumber == 2118 ||
						pTrack->m_nNumber == 1103 ||
						pTrack->m_nNumber == 2203 )
					{
						m_bStorePLTRequesting[nIndex] = FALSE;

						//if(m_pDoc->m_bLog == TRUE)
						//{
						//	strLog.Format(_T("%s 입고 요청 중 상태리셋! [%s]"), DEVICE, pTrack->GetTrackString());
						//	WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
						//}
					}					
				}
				else
				{
					//strLog.Format(_T("%s 입고 Station Check2-2 [%s]"), DEVICE, pTrack->GetTrackString());
					//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
					
					// 요청중이면 한번만 요청함
					if (m_bStorePLTRequesting[nIndex] == FALSE)
					{
						pTrack->m_bProductPLTRequest = TRUE;

						//if(m_pDoc->m_bLog == TRUE)
						//{
						//	strLog.Format(_T("%s 입고 요청 가능상태! [%s]"), DEVICE, pTrack->GetTrackString());
						//	WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
						//}
					}

					nEtcValue = _ttoi(pTrack->m_strEtcValue);

					//strLog.Format(_T("%s 입고 Station Check3 [%s]"), DEVICE, pTrack->GetTrackString());
					//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));

					// (생산정보가 올라올때) 한번만 요청하기
					if (m_pDoc->m_pHostCl != NULL && 
						m_pDoc->m_pHostCl->IsConnect() &&							// 호스트가 연결되어있고, 
						m_bStorePLTRequesting[nIndex] == FALSE &&					// 출고 요청 중이 아니며
						pTrack->m_bProductPLTRequest == TRUE )						// 출고 요청 신호가 올라오면
					{
						//strLog.Format(_T("%s 입고 Station Check4 [%s]"), DEVICE, pTrack->GetTrackString());
						//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));

						CStationInfo* pStation = pTrack->GetStoStation();
						
						if (pStation == NULL)
						{
							//if(m_pDoc->m_bLog ==TRUE)
							//{
							//	strLog.Format(_T("%s 입고 스테이션 정보 없음! [%s]"), DEVICE, pTrack->GetTrackString());
							//	WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
							//}

							continue;
						}
						
						//strLog.Format(_T("%s 입고 Station Check5 [%s]"), DEVICE, pTrack->GetTrackString());
						//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));

						m_pDoc->m_pHostCl->StoJobRequest(_ttoi(pStation->m_strID), 0, pTrack->m_strEtcValue);
						
						//if(m_pDoc->m_bLog ==TRUE)
						//{
						//	strLog.Format(_T("%s 입고 요청 완료! [%s]"), DEVICE, pTrack->GetTrackString());
						//	WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
						//}
						
						m_bStorePLTRequesting[nIndex] = TRUE;
						pTrack->m_bProductPLTRequest = FALSE;
					}
					//strLog.Format(_T("%s 입고 Station Check6 [%s]"), DEVICE, pTrack->GetTrackString());
					//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
				}		// if (bStatus == FALSE )			else

				/*
				// 요청중이면 한번만 요청함
				if (m_bStorePLTRequesting[nIndex] == FALSE)
				{
				pTrack->m_bProductPLTRequest = TRUE;
				}

				
				// (생산정보가 올라올때) 한번만 요청하기
				if (bStatus == TRUE &&	
					m_pDoc->m_pHostCl != NULL && 
					m_pDoc->m_pHostCl->IsConnect() &&							// 호스트가 연결되어있고, 
					m_bStorePLTRequesting[nIndex] == FALSE &&					// 출고 요청 중이 아니며
					pTrack->m_bProductPLTRequest == TRUE )						// 출고 요청 신호가 올라오면
				{
					CStationInfo* pStation = pTrack->GetStoStation();
//					DEBUGER_ASSERT_VALID(pStation != NULL);

					if (pStation == NULL)
						continue;

					m_pDoc->m_pHostCl->StoJobRequest(_ttoi(pStation->m_strID), 0, pTrack->m_strEtcValue);

					m_bStorePLTRequesting[nIndex] = TRUE;
					pTrack->m_bProductPLTRequest = FALSE;

				}

				if (bStatus == FALSE )
				{
					if (pTrack->m_nNumber == 2210 || 
						pTrack->m_nNumber == 2229 || 
						pTrack->m_nNumber == 2236 || 
						pTrack->m_nNumber == 2118 ||
						pTrack->m_nNumber == 2119 ||
						pTrack->m_nNumber == 1103 )
					{
						m_bStorePLTRequesting[nIndex] = FALSE;
					}					
				}
				*/
			}		//	if (nIndex >= 0 && nIndex <= 5)



		}			//	if (bStatus ^ pTrack->IsStoStationReady())

//-----------------------------------------------------------------------------------------------------------------------------

//=============================================================================================================================
//		// 입고 요청을 한번만 요청하기 - Magagin
//=============================================================================================================================
		if (bStatus ^ pTrack->IsRetStationReady())
		{
			nIndex = -1;
			switch(pTrack->m_nNumber)
			{
			case	3119:		nIndex = 0;				break;	
			case	3219:		nIndex = 1;				break;
			case	3319:		nIndex = 2;				break;
			case	3411:		nIndex = 3;				break;
			}

			if (nIndex >= 0 && nIndex <= 3)
			{
				//입고 요청 중이 아니면 입고 요청신호를 올림
				if (m_bStoreRequesting[nIndex] == FALSE)
				{
					pTrack->m_bStoRequest = TRUE;
				}

				// (입고대 상태가 켜질때) 한번만 요청하기
				if (bStatus == TRUE &&																	// 입고대상태가 ON이며
					((m_pDoc->m_pHostCl != NULL) && (m_pDoc->m_pHostCl->IsConnect())) && 				// 호스트가 연결되어있고, 
					m_bStoreRequesting[nIndex] == FALSE &&												// 입고 요청 중이 아니며
					pTrack->m_bStoRequest == TRUE )														// 입고 요청 신호가 올라오면
				{
					CStationInfo* pStation = pTrack->GetArvStation();
//					DEBUGER_ASSERT_VALID(pStation != NULL);

					if (pStation == NULL)
						continue;

					m_pDoc->m_pHostCl->JobRequest(_ttoi(pStation->m_strID), 1);

					m_bStoreRequesting[nIndex] = TRUE;
					pTrack->m_bStoRequest = FALSE;
				}

				if (bStatus == 0 )
				{
					if (pTrack->m_nNumber == 3119 || 
						pTrack->m_nNumber == 3219 || 
						pTrack->m_nNumber == 3319 || 
						pTrack->m_nNumber == 3411 )
					{
						m_bStoreRequesting[nIndex] = FALSE;
					}					
				}
			}
		}
//-----------------------------------------------------------------------------------------------------------------------------


//=============================================================================================================================
//		// 출고 요청을 한번만 요청하기
//=============================================================================================================================
		int nStation = 0;
		nIndex = -1;
		bStatus = CLib::IsSet(arrBuffer.GetWord(nWordAddr+7), 6);

		//if (bStatus ^ pTrack->IsRtvArriveHsReady())
		if (pTrack->IsRtvArriveHsReady() ==  TRUE)
		{
			switch(pTrack->m_nNumber)
			{
			case	2101:		nIndex = 0;			nStation = 110;				break;
			//case	2201:		nIndex = 1;			nStation = 108;				break;
			case	2217:		nIndex = 2;			nStation = 109;				break;
			}
			
			//strLog.Format(_T("%s 출고 Station Check0 [%s]"), DEVICE, pTrack->GetTrackString());
			//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));

			if (nIndex >= 0 && nIndex <= 2)
			{
				
				//strLog.Format(_T("%s 출고 Station Check2 [%s]"), DEVICE, pTrack->GetTrackString());
				//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));

				// 출고 요청 중이 아니면 출고 요청신호를 올림
				if (m_bRetrieveRequesting[nIndex] == FALSE)
				{
					//strLog.Format(_T("%s 출고 Station Check3 [%s]"), DEVICE, pTrack->GetTrackString());
					//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
					pTrack->m_bRetRequest = TRUE;
				}

				// (RTV 도착지 상태가 꺼질때) 한번만 요청하기
				if (bStatus == TRUE &&																	// RTV 도착지 상태가 OFF이며
					((m_pDoc->m_pHostCl != NULL) && (m_pDoc->m_pHostCl->IsConnect())) && 				// 호스트가 연결되어있고, 
					m_bRetrieveRequesting[nIndex] == FALSE &&													// 출고 요청 중이 아니며
					pTrack->m_bRetRequest == TRUE )														// 출고 요청 신호가 올라오면
				{
					//strLog.Format(_T("%s 출고 Station Check4 [%s]"), DEVICE, pTrack->GetTrackString());
					//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
					
					pStation = m_pDoc->GetStationInfo(CConvert::ToString(nStation));
//					DEBUGER_ASSERT_VALID(pStation != NULL);

					if (pStation == NULL)
					{
						//strLog.Format(_T("%s 출고 Station Check5 [%s]"), DEVICE, pTrack->GetTrackString());
						//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
						continue;
					}

					pJobItem = m_pDoc->m_pJob->FetchByRetDestination(pStation->m_strID);

					// 이쪽으로 출고하는 작업이 있는지 체크하는 것이므로 위와 다르게 NULL이 아니면 경고창 내도록 함!
					if (pJobItem != NULL)
					{
						//strLog.Format(_T("%s 출고 Station Check6 [%s]"), DEVICE, pTrack->GetTrackString());
						//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
						continue;
					}

					//strLog.Format(_T("%s 출고 Station Check7 [%s]"), DEVICE, pTrack->GetTrackString());
					//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));

					m_pDoc->m_pHostCl->JobRequest(nStation, 2);

					m_bRetrieveRequesting[nIndex] = TRUE;
					pTrack->m_bRetRequest = FALSE;
				}
				//else
				//{
					//strLog.Format(_T("%s 출고 Station Check8 [%s] 인덱스[%d]"), DEVICE, pTrack->GetTrackString(), nIndex);
					//WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
				//}
			}
			//else
			//{
			//	strLog.Format(_T("%s 출고 Station Check1 [%s] 인덱스[%d]"), DEVICE, pTrack->GetTrackString(), nIndex);
			//	WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::ReadStatus"));
			//}
		}
		else
		{
			if (bStatus == FALSE )
			{
				switch(pTrack->m_nNumber)
				{
				case	2101:		nIndex = 0;			nStation = 110;				break;
				//case	2201:		nIndex = 1;			nStation = 108;				break;
				case	2217:		nIndex = 2;			nStation = 109;				break;
				}
				
				if (pTrack->m_nNumber == 2101 || 
					//pTrack->m_nNumber == 2201 || 
					pTrack->m_nNumber == 2217 )
				{
					m_bRetrieveRequesting[nIndex] = FALSE;
				}
			}		
		}
//-----------------------------------------------------------------------------------------------------------------------------

		pTrack->SetStatusData(arrBuffer.GetWord(nWordAddr+7));
		pTrack->SetSensorData(arrBuffer.GetWord(nWordAddr+8));
		pTrack->SetMotorData(arrBuffer.GetWord(nWordAddr+9));

		pTrack->InvokeControl();
	}


	if (m_pInfo->m_pEquipment->m_nNumber == 4 || 
		m_pInfo->m_pEquipment->m_nNumber == 10 )
	{
//	if (m_pInfo->m_pDoorCtrls.GetSize())
//	{
		nStartWordAddr = 9901, nReadWordSize = 10;
		CByteArrayEx arrBufferEx(nStartWordAddr, nReadWordSize);

		m_pSocket->m_bWriteLog = FALSE;
		if (m_pSocket->ReadWord(arrBufferEx, nStartWordAddr, nReadWordSize, enDeviceCodeR) == FALSE)
		{
			strLog.Format(_T("%s DOOR정보 읽기 실패! [%s]"), DEVICE, m_pSocket ? m_pSocket->m_strLog : m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::ReadStatus"));
			return FALSE;
		}
		m_pSocket->m_bWriteLog = m_port.m_bWriteLog;

		WORD wTemp11 = arrBufferEx.GetWord(9901);
		WORD wTemp12 = arrBufferEx.GetWord(9902);
		WORD wTemp13 = arrBufferEx.GetWord(9903);
		WORD wTemp14 = arrBufferEx.GetWord(9904);
		WORD wTemp15 = arrBufferEx.GetWord(9905);

		if (wTemp15 != 0)
			int abc = 0;

		m_pInfo->SetDoorStatus(arrBufferEx.GetWord(9901));
		m_pInfo->SetDoorOpenReq(arrBufferEx.GetWord(9902));
		m_pInfo->SetDoorOpenAccept(arrBufferEx.GetWord(9903));
		m_pInfo->SetDoorClose(arrBufferEx.GetWord(9904));
		m_pInfo->SetRgvStatus(arrBufferEx.GetWord(9905));
		m_pInfo->InvokeControl();
	}
//*/
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCv::InvokeWeight(CTrackInfo* pTrack)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
//	DEBUGER_ASSERT_VALID(pStation != NULL);

	if ((pTrack->m_nLuggNum == 0) ||
		(pTrack->m_nDestPos != pTrack->m_nNumber) ||
		(pTrack->IsRetStationReady() == FALSE))
		return;

	CString strLog;

	if (pTrack->m_bDeadLock == FALSE)
	{
		if(WriteDeadLock(pTrack->m_nNumber, TRUE))
		{
			strLog.Format(_T("%s %s Weight 측정 지시 "), DEVICE, pTrack->GetTrackString());
			WriteLog(LOG_TYPE_JOB, pTrack->m_nLuggNum, strLog, _T("CCv::InvokeStoStation"));
		}
		else
		{
			strLog.Format(_T("%s %s Weight 측정 지시 실패! [%s]"), DEVICE, pTrack->GetTrackString(), m_pSocket ? m_pSocket->m_strLog : m_strLog);
			WriteLog(LOG_TYPE_ERROR, pTrack->m_nLuggNum, strLog, _T("CCv::InvokeStoStation"));
		}
	}
	else
	{
		if (pTrack->m_wReserved3 == 0)
		{
			if (_ttof(pTrack->m_strEtcValue) == 0.0)
			{
	//			strLog.Format(_T("%s %s 중량값 없음 "), DEVICE, pTrack->GetTrackString());
	//			WriteLog(LOG_TYPE_ERROR, pTrack->m_nLuggNum, strLog, _T("CCv::InvokeStoStation"));
				return;
			}
			else 
			{
				// 작업정보 가져오기
				CJobItem* pJobItem = (CJobItem *) m_pDoc->m_pJob->Find(pTrack->m_nLuggNum);
//				DEBUGER_ASSERT_VALID(pJobItem != NULL);

				if (pJobItem == NULL)
					return;

				int nJobWeight = (int) _ttof(pJobItem->m_strWeight) * 10;
				int nTrackWeight = _ttoi(pTrack->m_strEtcValue);

				int nJobNTrack = nJobWeight - nTrackWeight;
				int nTrackNJob = nTrackWeight - nJobWeight;
				int nConfigWeight = m_pDoc->m_pConfig->m_nWeightValue * 10;

				// 작업정보의 중량값도 가져오기
				if ((nJobNTrack > nConfigWeight) || (nTrackNJob > nConfigWeight))
				{
					// 에러상황
					WriteError(pTrack->m_nNumber, 6);

					pJobItem->SetJobStatus(enJobStatusWCError);
					m_pDoc->m_pJob->Backup();			
				}
				else
				{
					if (WriteTrackInfo(pTrack, pJobItem))
					{
						pJobItem->SetJobStatus(enJobStatusCvInvoke);

						strLog.Format(_T("%s %s 구동지시 [%s]"), DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString());
						WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeWeight"), pJobItem);
					}
					else
					{
						pJobItem->SetJobStatus(enJobStatusInvokeError);

						strLog.Format(_T("%s %s 구동지시 실패! [%s] [%s]"), DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString(), m_pSocket ? m_pSocket->m_strLog : m_strLog);
						WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::InvokeWeight"), pJobItem);
					}

					m_pDoc->m_pJob->Backup();					
				}
			}
		}
	}
}

void CCv::InvokeStoStation(CTrackInfo* pTrack, CStationInfo* pStation)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);

	switch (pStation->m_enKind)
	{
	case CStationInfo::enStoStation:
		if ((pTrack->m_nLuggNum != 0) ||
			(pTrack->IsStoStationReady() == FALSE))
			return;
		break;

		// WC에서 출발 신호 확인
	case CStationInfo::enArvStation:
		if (pTrack->m_nNumber == 3119 || pTrack->m_nNumber == 3219 || pTrack->m_nNumber == 3319 || pTrack->m_nNumber == 3411 )
		{
			if ((pTrack->m_nLuggNum != 0) ||
//				(pTrack->m_nDestPos != pTrack->m_nNumber) ||
				(pTrack->IsStoStationReady() == FALSE))
				return;
		}
		else if(pTrack->m_nNumber == 3111 || 
				pTrack->m_nNumber == 3112 || 
				pTrack->m_nNumber == 3211 || 
				pTrack->m_nNumber == 3212 || 
				pTrack->m_nNumber == 3311 || 
				pTrack->m_nNumber == 3312 || 
				pTrack->m_nNumber == 3404 || 
				pTrack->m_nNumber == 3409 || 
				pTrack->m_nNumber == 3502 )
		{
			if (//(pTrack->m_nLuggNum != 0) ||
				(pTrack->m_nDestPos != pTrack->m_nNumber) ||
				(pTrack->IsRetStationReady() == FALSE))
				return;
		}
		else if(pTrack->m_nNumber == 3117 || 
				pTrack->m_nNumber == 3217 || 
				pTrack->m_nNumber == 3317  )
		{
			if ((pTrack->m_nLuggNum == 0) ||
				(pTrack->m_nDestPos != pTrack->m_nNumber) ||
				(pTrack->IsRetStationReady() == FALSE))
				return;
		}
		else if(pTrack->m_nNumber == 3503 )
		{
			if ((pTrack->m_nLuggNum == 0) ||
//				(pTrack->m_nDestPos != pTrack->m_nNumber) ||
				(pTrack->IsRetStationReady() == FALSE))				
				return;
		}
		else
		{
			if ((pTrack->m_nLuggNum == 0) ||
				(pTrack->m_nDestPos != pTrack->m_nNumber) ||
				(pTrack->IsRetStationReady() == FALSE))
				return;
		}
		break;
	}

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchStoreJobByCv(pStation->m_strID);
	if (pJobItem == NULL)
		return;

	int nDestination = pTrack->m_nDestination;

//===================================================================================================================================================
//	// WC을 해야 하는 것이면 도착지를 바꿔줘야 함!
//===================================================================================================================================================
	// 작업패턴이 입고이며, 
	if (pJobItem->GetJobPattern() == enJobPatternSto || 
		pJobItem->GetJobPattern() == enJobPatternMove )
	{
		// 경유작업대가 있으면 그 경유 작업대로 가야함
		int nRouteStn = _ttoi(pJobItem->m_strRouteStn);
		switch(nRouteStn) 
		{
//		case 0:											break;
		case 215:		nDestination = 3117;			break;
		case 216:		nDestination = 3217;			break;
		case 217:		nDestination = 3317;			break;
		default:										break;
		} 
	}
//---------------------------------------------------------------------------------------------------------------------------------------------------

//===================================================================================================================================================
//	// WC에서는 Weight Checking Request 값을 리셋시켜줘야함!
//===================================================================================================================================================
	if ((pTrack->m_nNumber == 3117 || pTrack->m_nNumber == 3217 || pTrack->m_nNumber == 3317 ) && 
		(pStation->m_pTrack->m_nNumber != pTrack->m_nNumber))
	{
		WriteDeadLock(pTrack->m_nNumber, FALSE);
		return;
	}
//---------------------------------------------------------------------------------------------------------------------------------------------------

//===================================================================================================================================================
//	// SizeChecker에서는 에러리셋 값을 리셋시켜줘야함!
//===================================================================================================================================================
	int nSize = 0;
	if (pTrack->m_nNumber == 3503 )
	{
		nSize = 6;
//		return;
	}
//---------------------------------------------------------------------------------------------------------------------------------------------------
	
	if (WriteTrackInfo(pTrack, pJobItem, nDestination, 0 , nSize))
	{
		pJobItem->SetJobStatus(enJobStatusCvInvoke);

		strLog.Format(_T("%s %s 구동지시 [%s]"), DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString());
		WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeStoStation"), pJobItem);
	}
	else
	{
		pJobItem->SetJobStatus(enJobStatusInvokeError);

		strLog.Format(_T("%s %s 구동지시 실패! [%s] [%s]"), DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString(), m_pSocket ? m_pSocket->m_strLog : m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::InvokeStoStation"), pJobItem);
	}

	/*
//===================================================================================================================================================
//	// 이고 입고대에서는 생산 정보 표현해줘야 하리라. 
//===================================================================================================================================================
	CDciStaticCtrl* pPrdInfo1 = NULL;
	CDciStaticCtrl* pPrdInfo2 = NULL;

	// 전면부 표시 이고입고대 생산 정보 표현 
	if (pTrack->m_nNumber == 3504)
	{
		pPrdInfo1 = m_pDoc->m_pStoDataChk1[0];		// 이고 입고대는 0번 다음에 추가시에 다음 인덱스 사용 요망 
		pPrdInfo2 = m_pDoc->m_pStoDataChk2[0];		// 이고 입고대는 0번 다음에 추가시에 다음 인덱스 사용 요망 
		if (pJobItem != NULL && pPrdInfo2 != NULL && pPrdInfo2 != NULL)
		{
			CString strProductID= pJobItem->m_strProductID;
			strProductID.TrimRight();
			pPrdInfo1->m_strText = strProductID;
			pPrdInfo1->InvalidateControl(m_pDoc->m_hWndView, FALSE);

			int nCount = 1;
			if (strProductID.GetLength() > 2)
			{
				CString strProductKind;
				strProductKind = strProductID.Right(2);

				int nProdKind = _ttoi(strProductKind);
				if (nProdKind > 0)
				{
					// 무게값 *10
					//int nJobWeight = (int) atof(pJobItem->m_strWeight) * 10;		// 원본
					int nJobWeight = (int)(atof(pJobItem->m_strWeight)) * 10;

					int nPrdWeight = nJobWeight - 430;
					
					nCount = int(nPrdWeight / nProdKind) / 10 ;
				}
			}

			pPrdInfo2->m_strText.Format("%02d개", nCount);
			pPrdInfo2->InvalidateControl(m_pDoc->m_hWndView, FALSE);
		}
	}
//---------------------------------------------------------------------------------------------------------------------------------------------------
	//*/
	m_pDoc->m_pJob->Backup();
}

void CCv::InvokeRetStation(CTrackInfo* pTrack, CStationInfo* pStation)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);

	if ((pTrack->m_nLuggNum == 0) ||
		(pTrack->IsRetStationReady() == FALSE) ||
		(pTrack->m_nDestPos != pTrack->m_nNumber))
		return;

	if (pTrack->m_nNumber == 3117 || 
		pTrack->m_nNumber == 3217 || 
		pTrack->m_nNumber == 3317 )
		return;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(pTrack->m_nLuggNum);

	if ((pJobItem == NULL) ||
		(pJobItem->m_strDestStn != pStation->m_strID) ||
		(pJobItem->m_nJobStatus == enJobStatusArrived))
		return;

	if ((pJobItem->GetJobPattern() != enJobPatternRet) && 
		(pJobItem->GetJobPattern() != enJobPatternPR) && 
		(pJobItem->GetJobPattern() != enJobPatternMove))
	{
		strLog.Format(_T("%s %s 출고대 작업구분 이상! [출고대=%s, 작업구분=%s]"), 
			DEVICE, pTrack->GetTrackString(), pStation->m_strName, pJobItem->GetJobTypeString());
		m_pDoc->Alarm(LOG_POS_CV, strLog, pJobItem);
		return;
	}

	if (pJobItem->m_strDestStn != pStation->m_strID)
	{
		strLog.Format(_T("%s %s 출고대 도착지 이상! [출고대=%s, 도착지=%s]"), 
			DEVICE, pTrack->GetTrackString(), pStation->m_strID, pJobItem->m_strDestStn);
		m_pDoc->Alarm(LOG_POS_CV, strLog, pJobItem);
		return;
	}

	// RGV 작업완료시 작업이 완료되므로... 이 메세지가 출력될 수 밖에 없다. - 그래서 이 메세지를 출력하지 않도록 설정함!
	if(pJobItem->m_nJobStatus != enJobStatusCvInvoke)
	{
		strLog.Format(_T("%s %s 작업 상태 이상!  [예상 = %s, 실제 = %s]"), 
			DEVICE, pTrack->GetTrackString(), CJobItem::GetJobStatusString(enJobStatusCvInvoke), pJobItem->GetJobStatusString());
//		m_pDoc->Alarm(LOG_POS_CV, strLog, pJobItem);
		return;
	}

	strLog.Format(_T("%s 도착완료"), DEVICE);
	WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeRetStation"), pJobItem);

	m_pDoc->m_pJob->Arrive(pJobItem);
	m_pDoc->m_pJob->Backup();
}

void CCv::InvokeScRetHS(CTrackInfo* pTrack, CTrackHS* pTrackHS)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pTrackHS != NULL);

	if ((pTrack->m_nLuggNum != 0) ||
		(pTrack->IsScRetHsReady() != FALSE))
		return;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchScCompleteJob(pTrack->m_nNumber);
	if (pJobItem == NULL)
		return;

	CJobItem* pPairJobItem = NULL;
	if (pJobItem->m_nPairLuggNo > 0)
	{
		pPairJobItem = m_pDoc->m_pJob->FetchJobByPair(enJobPatternRet, enJobStatusScComplete, pJobItem->m_nPairLuggNo);

		if (pPairJobItem == NULL)
			return;

		CTrackInfo* pPairTrack = m_pDoc->GetTrackInfo(pPairJobItem->m_nArriveTrackNum);
		if (pPairTrack != NULL)
		{
			if (WriteTrackInfo(pPairTrack, pPairJobItem))
			{
				pPairJobItem->m_nDepartTrackNum = pPairJobItem->m_nArriveTrackNum = 0;
				pPairJobItem->SetJobStatus(enJobStatusCvInvoke);
				m_pDoc->m_pJob->Backup();

				strLog.Format(_T("%s %s 구동지시 [%s]"), DEVICE, pPairTrack->GetTrackString(), pPairJobItem->GetLogString());
				WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeScRetHS"), pPairJobItem);
			}
			else
			{
				strLog.Format(_T("%s %s 구동지시 실패! [%s] [%s]"), 
					DEVICE, pPairTrack->GetTrackString(), pPairJobItem->GetLogString(), m_pSocket ? m_pSocket->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::InvokeScRetHS"), pPairJobItem);
			}
		}
		else
		{
			strLog.Format(_T("%s %s 구동지시 실패! - Pair Track을 찾을 수가 없습니다. [%s] [%s]"), 
				DEVICE, pPairTrack->GetTrackString(), pPairJobItem->GetLogString(), m_pSocket ? m_pSocket->m_strLog : m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::InvokeScRetHS"), pPairJobItem);			
		}
	}

//================================================================================================================================================
//	// Test 모드일때 입고작업을 출고 HS에 기록한다.
//================================================================================================================================================
	/*
	// TEST 모드일 때
	if (m_pDoc->m_bTesting == TRUE)
	{
		pJobItem->SetJobStatus(enJobStatusCvInvoke);
		CJobItem* pTestJobItem = m_pDoc->m_pJob->FetchByTestStoJob(m_pDoc->m_strTestLocation);
		if (pTestJobItem == NULL)
		{
			m_pDoc->m_bTesting = FALSE;
			return;
		}

		pJobItem = pTestJobItem;
	}
	//*/
//------------------------------------------------------------------------------------------------------------------------------------------------
//=================================================================================================================
//	// Dead Lock 을 체크하기 위해서 (추가)
//=================================================================================================================
//	// DeadLock Checking 중일때  
//	int nDeadLock = 0;
//	if (pTrack->m_ucGenCode != 0)
//	{
//		nDeadLock = 1;
//	}
//-----------------------------------------------------------------------------------------------------------------

	if (WriteTrackInfo(pTrack, pJobItem))
	{
		pJobItem->m_nDepartTrackNum = pJobItem->m_nArriveTrackNum = 0;
		pJobItem->SetJobStatus(enJobStatusCvInvoke);
		m_pDoc->m_pJob->Backup();

		strLog.Format(_T("%s %s 구동지시 [%s]"), DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString());
		WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeScRetHS"), pJobItem);
	}
	else
	{
		strLog.Format(_T("%s %s 구동지시 실패! [%s] [%s]"), 
			DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString(), m_pSocket ? m_pSocket->m_strLog : m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::InvokeScRetHS"), pJobItem);
	}
}

void CCv::InvokeRtvArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pTrackHS != NULL);

	if ((pTrack->m_nLuggNum != 0) ||
		(pTrack->IsRtvArriveHsReady() != FALSE))
		return;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchRtvCompleteJob(pTrack->m_nNumber);
	if (pJobItem == NULL)
		return;

	if (WriteTrackInfo(pTrack, pJobItem))
	{
		pJobItem->m_nDepartTrackNum = pJobItem->m_nArriveTrackNum = 0;
		pJobItem->SetJobStatus(enJobStatusCvInvoke);
		m_pDoc->m_pJob->Backup();

		strLog.Format(_T("%s %s 구동지시 [%s]"), DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString());
		WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeRtvArriveHS"), pJobItem);

//=========================================================================================================================================================
//		// 목적지가 출하장인경우 RTV 도착지에서 도착보고		- 류정열 이사님께서 메일로 요청함! (2014.04.02)
//=========================================================================================================================================================
		CStationInfo* pStation = m_pDoc->GetStationInfo(pJobItem->m_strDestStn);
		if (	(pStation != NULL) 
			&&	(pJobItem->m_nJobType == enJobTypeAutoRet) 
//			&&	(pTrack->m_nDestination != 0) 
//			&&	(_ttoi(pStation->m_strID) > 200) 
	//		&&	(pTrack->m_nDestination == pStation->GetTrackDestination())
			)
		{
			if ((m_pDoc->m_pHostCl == NULL) ||
				(m_pDoc->m_pHostCl->IsConnect() == FALSE) ||
				(m_pDoc->m_pJob->Arrive(pJobItem) == FALSE))
			{
				strLog.Format(_T("%s %s 도착보고 실패! IMS에서 출고완료처리를 해주세요! [IMS 통신오류]"), DEVICE, pTrack->GetTrackString());
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::InvokeRtvArriveHS"), pJobItem, TRUE);
			}
			else
			{
				strLog.Format(_T("%s 도착완료"), DEVICE);
				WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeRtvArriveHS"), pJobItem);

//				pJobItem->m_nJobType = enJobTypeSemiRet;
//				pJobItem->MulticastEditJob();
//				m_pDoc->m_pJob->Backup();
			}
		}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
	}
	else
	{
		strLog.Format(_T("%s %s 구동지시 실패! [%s] [%s]"), 
			DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString(), m_pSocket ? m_pSocket->m_strLog : m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::InvokeRtvArriveHS"), pJobItem);
	}
}

void CCv::InvokeLifterArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pTrackHS != NULL);

	if ((pTrack->m_nLuggNum != 0) ||
		(pTrack->IsLfArriveHsReady() != FALSE))
		return;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchLifterCompleteJob(pTrack->m_nNumber);
	if (pJobItem == NULL)
		return;

	if (WriteTrackInfo(pTrack, pJobItem, pTrack->m_nDestination))
	{
		pJobItem->m_nDepartTrackNum = pJobItem->m_nArriveTrackNum = 0;
		pJobItem->SetJobStatus(enJobStatusCvInvoke);
		m_pDoc->m_pJob->Backup();

		strLog.Format(_T("%s %s 구동지시 [%s]"), DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString());
		WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeLifterArriveHS"), pJobItem);
	}
	else
	{
		strLog.Format(_T("%s %s 구동지시 실패! [%s] [%s]"), 
			DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString(), m_pSocket ? m_pSocket->m_strLog : m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::InvokeLifterArriveHS"), pJobItem);
	}
}

void CCv::InvokeLgvArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pTrackHS != NULL);

	if ((pTrack->m_nLuggNum != 0) ||
		(pTrack->IsLgvArriveHsReady() != FALSE))
		return;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchLgvCompleteJob(pTrack->m_nNumber);
	if (pJobItem == NULL)
		return;

	if (WriteTrackInfo(pTrack, pJobItem, pTrack->m_nDestination))
	{
		pJobItem->m_nDepartTrackNum = pJobItem->m_nArriveTrackNum = 0;
		pJobItem->SetJobStatus(enJobStatusCvInvoke);
		m_pDoc->m_pJob->Backup();

		strLog.Format(_T("%s %s 구동지시 [%s]"), DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString());
		WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeLgvArriveHS"), pJobItem);
	}
	else
	{
		strLog.Format(_T("%s %s 구동지시 실패! [%s] [%s]"), 
			DEVICE, pTrack->GetTrackString(), pJobItem->GetLogString(), m_pSocket ? m_pSocket->m_strLog : m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::InvokeLgvArriveHS"), pJobItem);
	}
}

void CCv::InvokeArriveReport(CTrackInfo* pTrack)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);

	if ((pTrack->m_nLuggNum == 0) ||
		(pTrack->m_ucGenCode != 0) ||
		(pTrack->m_nDestPos != pTrack->m_nNumber))
		return;

	if (pTrack->IsRetStationReady() == FALSE)
		return;

	CStationInfo* pStation = pTrack->GetRetStation();		// NULL; 
	if (pStation == NULL)
		pStation = pTrack->GetArvStation();

	if (pStation == NULL)
		return;

	CString strLog;
//	if ((pStation == NULL) || (pStation->GetTrackDestination() == pTrack->m_nNumber))
//		return;
	
	if (pTrack->m_nJobType == enJobTypeAutoRet ||
		pTrack->m_nJobType == enJobTypeAutoMove)
	{
		if ((m_pDoc->m_pHostCl == NULL) ||
			(m_pDoc->m_pHostCl->IsConnect() == FALSE) )
		{
			strLog.Format(_T("%s %s 도착보고 실패! IMS에서 출고완료처리를 해주세요! [IMS 통신오류]"), DEVICE, pTrack->GetTrackString());
			WriteLog(LOG_TYPE_ERROR, pTrack->m_nLuggNum, strLog, _T("CCv::InvokeArriveReport"));
			return;
		}

		m_pDoc->m_pHostCl->RetJobRequest(_ttoi(pStation->m_strID), pTrack->m_nLuggNum);

		strLog.Format(_T("%s %s 도착보고"), DEVICE, pTrack->GetTrackString());
		WriteLog(LOG_TYPE_JOB, pTrack->m_nLuggNum, strLog, _T("CCv::InvokeArriveReport"));

		pTrack->CommandTrack(pTrack->m_nLuggNum, pTrack->m_nJobType, pTrack->m_nDestPos, 1);
	}
}

