//CvFx.cpp:implementation of the CCv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "CvFx.h"
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

IMPLEMENT_DYNAMIC(CCvFx, CEquipment)

CCvFx::CCvFx(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice) : CEquipment(pDoc, nIndex, nNumber, lpszDevice)
{
	m_enKind = CEquipment::enCV;
	m_pInfo = new CCvInfo(this);

	int i = 0 ;
	for (i = 0 ; i < 4 ; i++) 
	{
		m_bStoreRequesting[i] = FALSE;
	}
	
	for (i = 0 ; i < 3 ; i++) 
	{
		m_bRetrieveRequesting[i] = FALSE;
	}

	for (i = 0 ; i < 5 ; i++) 
	{
		m_bStorePLTRequesting[i] = FALSE;
	}

	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
}

CCvFx::~CCvFx()
{
	delete m_pInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCvFx::MulticastInfo(CMonitorServer* pMonitorSv)
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

void CCvFx::AutoRunProc()
{
	UpdateCommStatus(NOTIFY_SEND);

	if (m_pInfo->m_wPrevLampStatus != m_pInfo->m_wCurrLampStatus)
	{
		for (int i = 0 ; i < 10 ; i++)
		{
			// 이전 램프상태
			BOOL bPrevLamp = CLib::IsSet(m_pInfo->m_wPrevLampStatus, i);
			BOOL bCurrLamp = CLib::IsSet(m_pInfo->m_wCurrLampStatus, i);

			if (bPrevLamp != bCurrLamp)
			{
				WriteLampStatus(i, bCurrLamp);

				CString strLog;

				strLog.Format(_T("%s %d번 경광등 설정 [SET=%s]"), DEVICE, i + 1, bCurrLamp ? "ON" : "OFF");
				WriteLog(LOG_TYPE_DEBUG, strLog, _T("CCv::CommandProc"));
			}
		}

		m_pInfo->m_wPrevLampStatus = m_pInfo->m_wCurrLampStatus;
	}

	if (ReadStatus())
	{
		UpdateCommStatus(NOTIFY_REFRESH);		//NOTIFY_RECEIVE
	}
	else
	{
		UpdateCommStatus(NOTIFY_ERROR);
	}

	// 통신상태 쓰기
	m_pDoc->m_bCommStatus = !m_pDoc->m_bCommStatus;

	WriteLampCommStatus(m_pDoc->m_bCommStatus);

	::Sleep(500);

	// 에러상태 쓰기
}

void CCvFx::CommandProc()
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
		case CCmdMsg::enCmdWriteLamp:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeWriteLamp);
			if (WriteLampStatus(_ttoi(strValues[0]), _ttoi(strValues[1])))
			{
				strLog.Format(_T("%s %d번 경광등 설정 [SET=%s]"), DEVICE, strValues[0], strValues[1]);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s %d번 경광등 설정 실패! [SET=%s]"), DEVICE, strValues[0], strValues[1]);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;

		default:
			strLog.Format(_T("%s INVLID COMMAND!"), DEVICE);
			WriteLog(LOG_TYPE_ALARM, LOG_SYSTEM, strLog, _T("CCv::CommandProc"));
		}

		delete pCmdMsg;
		pCmdMsg = NULL;
	}

	m_cmdList.RemoveAll();
}
BOOL CCvFx::WriteWordValue(int nAddress, WORD wWord, BOOL bOtherWay/* = FALSE*/)
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
BOOL CCvFx::WriteLoopTest(WORD wWord, BOOL bOtherWay)
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("통신연결 않됨"));
		return FALSE;
	}
	
	int nAddress = 0;
	CByteArrayEx arrBuffer(nAddress, 1);
	arrBuffer.SetWord(nAddress, wWord);

	BYTE	TxBuff[4098];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = (BYTE)wWord;									
	
	if (bOtherWay == TRUE)
		return m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 1);
	else
		return m_pSocket->LoopBackTest(arrBuffer, 1, enDeviceCodeR);
}
*/
BOOL CCvFx::ReadStatus()
{
	if (!IsConnect())
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	CString strLog;
	CJobItem * pJobItem = NULL;
	CStationInfo* pStation = NULL;
	int nStartWordAddr = 0;
	int nReadWordSize = 11 * 2;//m_pInfo->m_pTracks.GetSize() * 10;
	CByteArrayEx arrBuffer(nStartWordAddr, nReadWordSize);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	BYTE pBuffer[24];
	memset(pBuffer, 0x00, sizeof(pBuffer));

	m_pSocket->m_bWriteLog = FALSE;
	if (m_pSocket->ReadWord(arrBuffer, nStartWordAddr, nReadWordSize, enDeviceCodeR) == FALSE)
	{
		strLog.Format(_T("%s 경광등 정보 읽기 실패! [%s]"), DEVICE, m_pSocket ? m_pSocket->m_strLog : m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::ReadStatus"));
		return FALSE;
	}

	m_pSocket->m_bWriteLog = m_port.m_bWriteLog;
	for(int i = 0 ; i < nReadWordSize ; i++)
	{
		pBuffer[i] = arrBuffer[i];
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

//===========================================================================================================
//	// 경광등
//===========================================================================================================
	WORD wLamp = 0x0000;
	BOOL bRealLamp = FALSE;
	BOOL bOldLamp = FALSE;
	nReadWordSize = 11;
//	if (m_pInfo->m_wPrevLampStatus != m_pInfo->m_wCurrLampStatus)
//	{
//		m_pInfo->m_wPrevLampStatus = m_pInfo->m_wCurrLampStatus;

		for(int m = 0 ; m < (nReadWordSize-1) ; m++)
		{
			int nAddress = (m * 2) + 2;
			wLamp = arrBuffer.GetWord(nAddress);
			wLamp = arrBuffer[nAddress];
			bRealLamp = CLib::IsSet(wLamp, 0);

//			wLamp = arrBuffer.GetWord(nAddress-1);

//			bRealLamp = arrBuffer.GetBit(nAddress, enBit11);


			bOldLamp = CLib::IsSet(m_pInfo->m_wPrevLampStatus, m);
	//		if (bOldLamp != bRealLamp/* && m_pInfo->m_wPrevLampStatus != m_pInfo->m_wCurrLampStatus*/)
	//		{
	//			CLib::SetBit(m_pInfo->m_wCurrLampStatus, m, bRealLamp);
	//		}

			m_pInfo->m_pLampCtrls[m]->m_clrBgColor = (bRealLamp == TRUE) ? RED : GREEN;
			m_pInfo->m_pLampCtrls[m]->InvalidateControl(m_pDoc->m_hWndView, FALSE);

			CLib::SetBit(m_pInfo->m_wCurrLampStatus, m, bRealLamp);
		}
//	}
//-----------------------------------------------------------------------------------------------------------
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
