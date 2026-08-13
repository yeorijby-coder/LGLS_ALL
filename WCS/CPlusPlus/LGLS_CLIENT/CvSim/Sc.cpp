// Sc.cpp: implementation of the CSc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "Sc.h"
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

IMPLEMENT_DYNAMIC(CSc, CEquipment)

CSc::CSc(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice) : CEquipment(pDoc, nIndex, nNumber, lpszDevice)
{
	m_enKind = CEquipment::enSC;
	m_pInfo = new CScInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	m_bReset = FALSE;
}

CSc::~CSc()
{
	delete m_pInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSc::MulticastInfo(CMonitorServer* pMonitorSv)
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

	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSc::AutoRunProc(int nConnNum)
{
	UpdateCommStatus(NOTIFY_SEND);

	if (ReadStatus())
	{
		if (m_pInfo->IsErrorStatus())
		{
			ErrorCheck();
			ErrorRoutine();
		}
		else
		{
			if ((m_pInfo->m_nErrorCode == 0) && 
				(m_pInfo->m_nPrevErrorCode != 0))
			{
				m_pInfo->m_nPrevErrorCode = 0;
				m_pInfo->Backup();
			}

			if (m_pInfo->m_ScInfo[0].m_bInvoke)
				CompleteCheck();
			else
				InvokeCheck();

			// 화재발생 확인
			CString strLog;
//			strLog.Format(_T("%s @@@@@ 화재발생! - Alarm=%d Reset=%d"), DEVICE, m_pInfo->m_ucFireAlarm, m_bReset);
//			WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::AutoRunProc"));
			if (m_pInfo->m_ScInfo[0].m_ucFireAlarm == 1 && m_bReset == FALSE)
			{
				strLog.Format(_T("%s @@@@@ 리셋! - Alarm=%d Reset=%d"), DEVICE, m_pInfo->m_ScInfo[0].m_ucFireAlarm, m_bReset);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::AutoRunProc"));
				if (!Reset())
				{
					strLog.Format(_T("%s 화재발생 처리 후 리셋 실패 [%s]"), DEVICE, m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::AutoRunProc"));
				}
				m_bReset = TRUE;
//				CWnd::SetDlgItemText(IDC_EDIT_JOB_PROD_INFO2, "Reset OK!");
			}
			else if (m_pInfo->m_ScInfo[0].m_ucFireAlarm == 1 && m_bReset == TRUE)
			{
				strLog.Format(_T("%s @@@@@ 액티브! - Alarm=%d Reset=%d"), DEVICE, m_pInfo->m_ScInfo[0].m_ucFireAlarm, m_bReset);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::AutoRunProc"));
				if (Active())
				{
					strLog.Format(_T("%s 화재발생 처리완료"), DEVICE);
					WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::AutoRunProc"));
					m_pInfo->m_ScInfo[0].m_ucFireAlarm = 0;
					FireAlarmOff();
					m_bReset = FALSE;
				}
				else
				{
					strLog.Format(_T("%s 화재발생 처리 후 액티브 실패! [%s]"), DEVICE, m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::AutoRunProc"));
				}
//				CWnd::SetDlgItemText(IDC_EDIT_JOB_PROD_INFO2, "Home OK!");
			}
		}
		UpdateCommStatus(NOTIFY_REFRESH);
	}
	else
	{
		ErrorCheck();
		UpdateCommStatus(NOTIFY_ERROR);
	}

	SafeCheck();

	::Sleep(100);
}

void CSc::CommandProc()
{
	CString strLog;
	CCmdMsg* pCmdMsg = NULL;
	for (POSITION pos=m_cmdList.GetHeadPosition(); pos!=NULL; )
	{
		pCmdMsg = (CCmdMsg*)m_cmdList.GetNext(pos);
		CStringArray& strValues = pCmdMsg->m_strValues;
		DEBUGER_ASSERT_VALID(pCmdMsg != NULL);

		switch (pCmdMsg->m_nCommand)
		{
		case CScCmdMsg::enCmdStoSuspend:
			{
				DEBUGER_ASSERT_VALID(strValues.GetSize() == 1);
				m_pInfo->m_bStoSuspend = _ttoi(strValues[0]);
				m_pInfo->m_bModified = TRUE;
				m_pInfo->InvokeControl();
				m_pInfo->StatusReport();

				strLog.Format(_T("%s 입고 일시정지"), DEVICE);
				//m_pDoc->StatusDisplay(strLog, FALSE);
			}
			break;

		case CScCmdMsg::enCmdRetSuspend:
			{
				DEBUGER_ASSERT_VALID(strValues.GetSize() == 1);
				m_pInfo->m_bRetSuspend = _ttoi(strValues[0]);
				m_pInfo->m_bModified = TRUE;
				m_pInfo->InvokeControl();
				m_pInfo->StatusReport();

				strLog.Format(_T("%s 출고 일시정지"), DEVICE);
				//m_pDoc->StatusDisplay(strLog, FALSE);
			}
			break;

		case CScCmdMsg::enCmdManualSto:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			if (Store(_ttoi(strValues[0]), strValues[1], _ttoi(strValues[2])))
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
				m_pInfo->m_nInternalJobType = enJobTypeManual;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				strLog.Format(_T("%s 입고지시 [작업번호=%s, 출발위치=%s, 도착위치=%s]"), DEVICE, strValues[0], strValues[2], strValues[1]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 입고지시 실패! [작업번호=%s, 출발위치=%s, 도착위치=%s] [%s]"), DEVICE, strValues[0], strValues[2], strValues[1], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CSc::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdManualRet:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			if (Retrieve(_ttoi(strValues[0]), strValues[1], _ttoi(strValues[2])))
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
				m_pInfo->m_nInternalJobType = enJobTypeManual;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				strLog.Format(_T("%s 출고지시 [작업번호=%s, 출발위치=%s, 도착위치=%s]"), DEVICE, strValues[0], strValues[1], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 출고지시 실패! [작업번호=%s, 출발위치=%s, 도착위치=%s] [%s]"), DEVICE, strValues[0], strValues[1], strValues[2], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CSc::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdManualHs2Hs:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			if (HsToHs(_ttoi(strValues[0]), _ttoi(strValues[1]), _ttoi(strValues[2])))
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
				m_pInfo->m_nInternalJobType = enJobTypeManual;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				strLog.Format(_T("%s 직출고지시 [작업번호=%s, 출발위치=%s, 도착위치=%s]"), DEVICE, strValues[0], strValues[1], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 직출고지시 실패! [작업번호=%s, 출발위치=%s, 도착위치=%s] [%s]"), DEVICE, strValues[0], strValues[1], strValues[2], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CSc::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdManualR2R:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			if (RackToRack(_ttoi(strValues[0]), strValues[1], strValues[2]))
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
				m_pInfo->m_nInternalJobType = enJobTypeManual;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				strLog.Format(_T("%s 랙투랙지시 [작업번호=%s, 출발위치=%s, 도착위치=%s]"), DEVICE, strValues[0], strValues[1], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 랙투랙지시 실패! [작업번호=%s, 출발위치=%s, 도착위치=%s] [%s]"), DEVICE, strValues[0], strValues[1], strValues[2], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CSc::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdHomeReturn:
			if (CallToHome())
			{
// 					m_pInfo->m_bInvoke = TRUE;
// 					m_pInfo->m_nInternalLuggNum = LOG_SYSTEM;
// 					m_pInfo->m_nInternalJobType = enJobTypeManual;
// 					m_pInfo->m_bModified = TRUE;
// 					m_pInfo->Backup();

				strLog.Format(_T("%s 홈복귀지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 홈복귀지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdNewLocation:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			{
				CJobItem* pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValues[0]));
				if (pJobItem != NULL)
				{
					strLog.Format(_T("%s 이중입고 재지정 지시 [LOC=%s]"), DEVICE, strValues[1]);
					WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CommandProc"), pJobItem);

					pJobItem->m_strDestLoc = strValues[1];
					pJobItem->SetJobStatus(enJobStatusDualStoreRetry);
					// m_pDoc->m_pJob->Backup();		// @@
					m_pDoc->m_bBackupChk = TRUE;
					ErrorRoutine();
				}
				else
				{
					strLog.Format(_T("%s 이중입고 재지정 지시 실패! [LOC=%s]"), DEVICE, strValues[1]);
					WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CSc::CommandProc"));
				} 
			}
			break;


		case CScCmdMsg::enCmdComplete:
			{
				strLog.Format(_T("%s 강제완료"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CommandProc"));

				m_pInfo->m_ScInfo[0].m_nLuggNum = m_pInfo->m_ScInfo[0].m_nInternalLuggNum;
				m_pInfo->m_ucJobStatus = CScInfo::enComplete;
				m_pInfo->m_ucPrevStatus  = CScInfo::enRunning;
				CompleteCheck();
			}
			break;

		case CScCmdMsg::enCmdEmergency:
			if (Emergency())
			{
				strLog.Format(_T("%s EMERGENCY지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s EMERGENCY지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdActive:
			if (Active())
			{
				strLog.Format(_T("%s ACTIVE지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s ACTIVE지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdStop:
			if (Stop())
			{
				strLog.Format(_T("%s STOP지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s STOP지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdReset:
			if (Reset())
			{
				strLog.Format(_T("%s RESET지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s RESET지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdCancel:
 			if (Delete())
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = FALSE;
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();
				
				strLog.Format(_T("%s FORK1 DATA DELETE지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s FORK1 DATA DELETE지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::CommandProc"));
			}
			break;
		// 화재발생
		case CScCmdMsg::enCmdFireAlarmOn:
			if (FireAlarmOn())
			{
				strLog.Format(_T("%s 화재발생 지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 화재발생 지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::CommandProc"));
			}
			break;

		default:
			m_strLog.Format(_T("%s INVALID COMMAND!"), DEVICE);
			WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CSc::CommandProc"));
		}

		delete pCmdMsg;
		pCmdMsg = NULL;
	}

	m_cmdList.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CSc::ReadStatus()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	m_pSocket[0]->m_bWriteLog = FALSE;

	CByteArray arrRxData;
	int nReadWordSize = 36;
	static int nCount = 0;
	CString strTemp;
	if (!m_pSocket[0]->ReadWord(arrRxData, 95, nReadWordSize))
	{
		if (++nCount >= 10)
		{
			m_pInfo->m_nErrorCode = enScErrorCommunication;
			m_pInfo->m_ucScStatus = CScInfo::enError;
			m_pInfo->m_bModified = TRUE;
		}

		return FALSE;
	}
	nCount = 0;

	m_pSocket[0]->m_bWriteLog = m_port.m_bWriteLog;

	///////////////////////////////////////////////////////////////////////////////////
	
	if (arrRxData.GetSize() < nReadWordSize*2)
		return FALSE;

	CString strDepartInfo = "";
	CString strArriveInfo = "";

	// 출발지와 목적지 정보 추출하기(D113 ~ D120)
	for(int i = 0 ; i < 8 ; i+=2)									// COUNT: 8
	{
//		strDepartInfo += strTemp.Format(_T("%c", arrRxData[36 + i]);	// START:36
		strTemp.Format(_T("%02d"), CLib::SwapToWord(arrRxData.GetData() + 36 + i));					// START:36
		strDepartInfo = strDepartInfo + strTemp;

//		strArriveInfo += strTemp.Format(_T("%c", arrRxData[44 + i]);	// START:44
		strTemp.Format(_T("%02d"), CLib::SwapToWord(arrRxData.GetData() + 44 + i));					// START:44
		strArriveInfo = strArriveInfo + strTemp;
	}

	BOOL bReport = FALSE;
	if (m_pInfo->SetRcMode((BYTE)arrRxData[0]))		bReport = TRUE;				// D 95
	m_pInfo->SetFireAlarm((BYTE)arrRxData[6]);									// D 98
	if (m_pInfo->SetScMode((BYTE)arrRxData[10]))	bReport = TRUE;				// D100 - L
	m_pInfo->SetProdLoad((BYTE)arrRxData[11]);									// D100 - H
	if (m_pInfo->SetScStatus((BYTE)arrRxData[12]))	bReport = TRUE;				// D101
	m_pInfo->SetHorizontalPos((BYTE)arrRxData[14]);								// D102
	m_pInfo->SetVerticalPos((BYTE)arrRxData[16]);								// D103
	m_pInfo->SetForkPos((BYTE)arrRxData[18]);									// D104
	m_pInfo->SetErrorCode((BYTE)arrRxData[20]);									// D105			- Trouble보고 할것인지 결정할것
	m_pInfo->SetForkStatus((BYTE)arrRxData[22]);								// D106
	
//	WORD wWord = CLib::SwapToWord(arrRxData.GetData() + 24);					// D107
//	
//	switch(m_pInfo->m_pEquipment->m_nNumber)
//	{
//	case 1:		(m_pInfo->m_pEquipment->m_nNumber == 0) ? nTrackNum = 1		: nTrackNum = 6;		break;
//	case 2:		(m_pInfo->m_pEquipment->m_nNumber == 0) ? nTrackNum = 7		: nTrackNum = 8;		break;
//	case 3:		(m_pInfo->m_pEquipment->m_nNumber == 0) ? nTrackNum = 9		: nTrackNum = 10;		break;
//	}

//	WORD wRetWord = CLib::SwapToWord(arrRxData.GetData() + 26);					// D108

//	int nBit=0;			// 작업대 번호
//	for(int i=0; i<16; i++)
//	{
//		if(CLib::IsSet(wWord, i) == 1)
//			nBit = i;
//	}

	
	/*
	// 트랙번호 찾아오기
	int nTrackNum;
	switch(nBit)
	{
	case 0:		(m_pInfo->m_pEquipment->m_nNumber == 0) ? nTrackNum = 3		: nTrackNum = 5;		break;
	case 1:		(m_pInfo->m_pEquipment->m_nNumber == 0) ? nTrackNum = 1		: nTrackNum = 6;		break;
	case 2:		(m_pInfo->m_pEquipment->m_nNumber == 0) ? nTrackNum = 7		: nTrackNum = 8;		break;
	case 3:		(m_pInfo->m_pEquipment->m_nNumber == 0) ? nTrackNum = 9		: nTrackNum = 10;		break;
	case 4:		(m_pInfo->m_pEquipment->m_nNumber == 0) ? nTrackNum = 106	: nTrackNum = 112;		break;
	case 5:		(m_pInfo->m_pEquipment->m_nNumber == 0) ? nTrackNum = 107	: nTrackNum = 109;		break;
	}
	*/


//	uchar	TxBuff[256];
//
//	// 출고시는 포킹허가 요청과 출고 HS만 올라오면 무조건 포킹 허가함!!
//	if (m_pDoc->m_ConveyorTrackInfo[nTrackNum-1].m_bRetHomeStandReady && wRetWord !=0)
//	{
//		memset(TxBuff, 0x00, sizeof(TxBuff));
//		TxBuff[0] = 0x01;										
//		m_pSocket->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  200, 1);
//	}
//
//	// 입고시는 포킹허가 요청과 입고 HS와 CRANE에 화물이 없을때 포킹 허가함!!
//	if (m_pDoc->m_ConveyorTrackInfo[nTrackNum-1].m_bStoHomeStandReady && wStoWord !=0 && ucProdLoad == '0')
//	{
//		memset(TxBuff, 0x00, sizeof(TxBuff));
//		TxBuff[0] = 0x01;										// K0011 
//		Write(TxBuff, 0x4B, 1, SC_BASE_ADDR_HIGH);				// 
//	}


	if (m_pInfo->SetRcStatus((BYTE)arrRxData[28]))	bReport = TRUE;			// D109
	m_pInfo->SetJobStatus((BYTE)arrRxData[30]);								// D110
	m_pInfo->SetCmdType((BYTE)arrRxData[32]);								// D111		
	m_pInfo->SetLuggNum(CLib::SwapToWord(arrRxData.GetData() + 34));		// D112
	m_pInfo->SetDepartInfo(strDepartInfo);									// D113 ~ D116
	m_pInfo->SetArriveInfo(strArriveInfo);									// D117 ~ D120

	if (bReport == TRUE)	m_pInfo->StatusReport();
	m_pInfo->InvokeControl();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
CString CSc::MiddleData(const CByteArray arrData, int nFirst, int nCount)
{
	CString strTemp, strReturnData = "";

	for(int i = 0 ; i < nCount ; i++)
		strReturnData += strTemp.Format(_T("%c"), arrData.GetData() + nFirst);

	return strReturnData;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int CSc::Store(int nLuggNum, CString strLocation, int nHsNum, int nSize /* = 0 */, BOOL bStoHS /* = FALSE */, BOOL bRetHS /* = FALSE */)
int CSc::Store(int nLuggNum, CString strLocation, int nHsNum, int nSize /* = 0 */)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X"), 
							1,							// 작업구분 = 입고					// D171
							nLuggNum,					// 작업번호 = 9999		(포크 1)	// D172
							0, 0, 0,					// 출발위치		= 열행단(포크 1)	// D173~D175
							nHsNum,						// 출발작업대	= H/S	(포크 1)	// D176
							CLib::GetSide(strLocation),	// 도착위치		= 열	(포크 1)	// D177
							CLib::GetBay(strLocation),	// 도착위치		= 행	(포크 1)	// D178
							CLib::GetLevel(strLocation),// 도착위치		= 단	(포크 1)	// D179	
							0,							// 도착작업대	= H/S	(포크 1)	// D180
							0,							// 2포크 사용 여부					// D181
							0,							// 작업번호 = 0			(포크 2)	// D182
							0, 0, 0,					// 출발위치		= 열행단(포크 2)	// D183~D185
							0,							// 출발작업대	= H/S	(포크 2)	// D186
							0, 0, 0,					// 도착위치		= 열행단(포크 2)	// D187~D189
							0,							// 도착작업대	= H/S	(포크 2)	// D190
							1);							// 기록완료							// D191

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 171, 21);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
//	int nUseFork;

	TxBuff[0] = 1;									// D171 - Store - DualStore

	if(nLuggNum != 0)
	{
		TxBuff[2] = nLuggNum & 0xFF;				// D172 LOW 
		TxBuff[3] = (nLuggNum >> 8) & 0xFF;		// D172 HIGH

		TxBuff[10] = (BYTE)nHsNum;					// D176 Start Home Stand  LOW

		TxBuff[12] = CLib::GetSide(strLocation);			// D177 Bank   LOW
		TxBuff[14] = CLib::GetBay(strLocation);			// D178 Bay    LOW
		TxBuff[16] = CLib::GetLevel(strLocation);		// D179 Level  LOW
	}

	TxBuff[20] = 0;							// D181 (0 - USE FORK#1) (1 - USE FORK#1, #2) (2 - USE FORK#2) --- LOW

	TxBuff[40] = 1;									// D191 WRITE COMPLETE DEFAULT SETTING  -HIGH
//	TxBuff[42] = nSize;						        // D192 Gen   - HIGH
	TxBuff[42] = (BYTE)nSize;						// D192 Gen   - HIGH
//	TxBuff[44] = (BYTE)bStoHS;						// D193 StoHS - HIGH
//	TxBuff[46] = (BYTE)bRetHS;						// D194 RetHS - HIGH



	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 22);
}

//int CSc::Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nSize /* = 0 */, BOOL bStoHS /* = FALSE */, BOOL bRetHS /* = FALSE */)
int CSc::Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nSize /* = 0 */)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X"), 
							2,							// 작업구분 = 출고					// D171
							nLuggNum,					// 작업번호 = 9999		(포크 1)	// D172
							CLib::GetSide(strLocation),	// 출발위치		= 열	(포크 1)	// D173
							CLib::GetBay(strLocation),	// 출발위치		= 행	(포크 1)	// D174
							CLib::GetLevel(strLocation),// 출발위치		= 단	(포크 1)	// D175	
							0,							// 출발작업대	= H/S	(포크 1)	// D176
							0, 0, 0,					// 도착위치		= 열행단(포크 1)	// D177~D179	
							nHsNum,						// 도착작업대	= H/S	(포크 1)	// D180
							0,							// 2포크 사용 여부					// D181
							0,							// 작업번호 = 0			(포크 2)	// D182
							0, 0, 0,					// 출발위치		= 열행단(포크 2)	// D183~D185
							0,							// 출발작업대	= H/S	(포크 2)	// D186
							0, 0, 0,					// 도착위치		= 열행단(포크 2)	// D187~D189
							0,							// 도착작업대	= H/S	(포크 2)	// D190
							1);							// 기록완료							// D191

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 171, 21);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
//	int nUseFork;

	TxBuff[0] = 2;									// D171 - Retrieve

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

	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 22);

}

//int CSc::HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nSize /* = 0 */, BOOL bStoHS /* = FALSE */, BOOL bRetHS /* = FALSE */)
int CSc::HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nSize /* = 0 */)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X"), 
							3,				// 작업구분 = 홈복귀				// D171
							nLuggNum,		// 작업번호 = 9999		(포크 1)	// D172
							0, 0, 0,		// 출발위치		= 열행단(포크 1)	// D173~D175
							nHsNumFrom,		// 출발작업대	= H/S	(포크 1)	// D176
							0, 0, 0,		// 도착위치		= 열행단(포크 1)	// D177~D179	
							nHsNumTo,		// 도착작업대	= H/S	(포크 1)	// D180
							0,				// 2포크 사용 여부					// D181
							0,				// 작업번호 = 0			(포크 2)	// D182
							0, 0, 0,		// 출발위치		= 열행단(포크 2)	// D183~D185
							0,				// 출발작업대	= H/S	(포크 2)	// D186
							0, 0, 0,		// 도착위치		= 열행단(포크 2)	// D187~D189
							0,				// 도착작업대	= H/S	(포크 2)	// D190
							1);				// 기록완료							// D191

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 171, 21);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
//	int nUseFork;

	TxBuff[0] = 3;								    // D171 - Store - DualStore

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
	
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 22);

}

BOOL CSc::RackToRack(int nLuggNum, CString strLocFrom, CString strLocTo, int nSize /* = 0 */)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X"), 
							4,							// 작업구분 = 랙투랙				// D171
							nLuggNum,					// 작업번호 = 9999		(포크 1)	// D172
							CLib::GetSide(strLocFrom),	// 출발위치		= 열	(포크 1)	// D173
							CLib::GetBay(strLocFrom),	// 출발위치		= 행	(포크 1)	// D174
							CLib::GetLevel(strLocFrom),	// 출발위치		= 단	(포크 1)	// D175	
							0,							// 출발작업대	= H/S	(포크 1)	// D176
							CLib::GetSide(strLocTo),	// 도착위치		= 열	(포크 1)	// D177
							CLib::GetBay(strLocTo),		// 도착위치		= 행	(포크 1)	// D178
							CLib::GetLevel(strLocTo),	// 도착위치		= 단	(포크 1)	// D179	
							0,							// 도착작업대	= H/S	(포크 1)	// D180
							0,							// 2포크 사용 여부					// D181
							0,							// 작업번호 = 0			(포크 2)	// D182
							0, 0, 0,					// 출발위치		= 열행단(포크 2)	// D183~D185
							0,							// 출발작업대	= H/S	(포크 2)	// D186
							0, 0, 0,					// 도착위치		= 열행단(포크 2)	// D187~D189
							0,							// 도착작업대	= H/S	(포크 2)	// D190
							1);							// 기록완료							// D191

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 171, 21);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
//	int nUseFork;

	TxBuff[0] = 4;                             // D171 - Rack To Rack

	if(nLuggNum != 0)
	{
		TxBuff[2] = nLuggNum & 0xFF;		       // D172 LOW 
		TxBuff[3] = (nLuggNum >> 8) & 0xFF;       // D172 HIGH

		TxBuff[4] = CLib::GetSide(strLocFrom);          // D173 Bank   LOW
		TxBuff[6] = CLib::GetBay(strLocFrom);           // D174 Bay    LOW
		TxBuff[8] = CLib::GetLevel(strLocFrom);         // D175 Level  LOW

		TxBuff[12] = CLib::GetSide(strLocTo);           // D177 Bank   LOW
		TxBuff[14] = CLib::GetBay(strLocTo);            // D178 Bay    LOW
		TxBuff[16] = CLib::GetLevel(strLocTo);		   // D179 Level  LOW
	}

//	TxBuff[20] = nUseFork;						  // D181 (0 - USE FORK#1) (1 - USE FORK#1, #2) (2 - USE FORK#2) --- LO

	TxBuff[40] = 1;						          // D191 WRITE COMPLETE DEFAULT SETTING  -LOW
//	TxBuff[42] = nSize;						      // D192 Gen   - LOW
	TxBuff[42] = (BYTE)nSize;                     // D192 Gen   - LOW

	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 22);
	
}

BOOL CSc::CallToHome()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X%.4X"), 
							5,				// 작업구분 = 홈복귀				// D171
							LOG_SYSTEM,		// 작업번호 = 9999		(포크 1)	// D172
							0, 0, 0,		// 출발위치		= 열행단(포크 1)	// D173~D175
							1,				// 출발작업대	= H/S	(포크 1)	// D176
							0, 0, 0,		// 도착위치		= 열행단(포크 1)	// D177~D179	
							1,				// 도착작업대	= H/S	(포크 1)	// D180
							0,				// 2포크 사용 여부					// D181
							0,				// 작업번호 = 0			(포크 2)	// D182
							0, 0, 0,		// 출발위치		= 열행단(포크 2)	// D183~D185
							0,				// 출발작업대	= H/S	(포크 2)	// D186
							0, 0, 0,		// 도착위치		= 열행단(포크 2)	// D187~D189
							0,				// 도착작업대	= H/S	(포크 2)	// D190
							1);				// 기록완료							// D191

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 171, 21);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 5;								// D171 - CallToHome
	TxBuff[40] = 1;								// D191 WRITE COMPLETE DEFAULT SETTING  -LOW

	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 21);
}

BOOL CSc::Emergency()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X"), 0x01);

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 199, 1);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 1; 
//	TxBuff[0] = '1';  
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);
	
}

BOOL CSc::Active()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X"), 0x02);

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 199, 1);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));


	TxBuff[0] = 2;    
//	TxBuff[0] = '1';  
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);

}

BOOL CSc::Stop()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X"), 0x04);

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 199, 1);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 4; 
//	TxBuff[0] = '1';  
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);

}

BOOL CSc::Reset()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X"), 0x08);

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 199, 1);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 8;                     
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);
}

BOOL CSc::Delete()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	/*
	CString strBuffer;
	strBuffer.Format(_T("%.4X"), 0x10);

	CByteArray arrBuffer;
	arrBuffer.SetSize(strBuffer.GetLength());
	for (int i=0; i<arrBuffer.GetSize(); ++i)
		arrBuffer[i] = strBuffer[i];

	DEBUGER_ASSERT_VALID(m_pSocket != NULL);
	return m_pSocket->WriteWord(arrBuffer, 199, 1);
	//*/
	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

//  TxBuff[0] = 1;
	TxBuff[0] = 16;//bFirst;							// 16 포크#1,  32 포크#2,   64 포크#1, #2 ALL DELETE
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);
}

//=============================================================================
//	Desc	: 화재발생 강제이동 On 신호 송신
//	Date	:
//	Update	: 
//=============================================================================
BOOL CSc::FireAlarmOn()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	CString strLog;
	strLog.Format(_T("%s 화재발생 강제이동 On 신호 송신"), DEVICE);
	WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::FireAlarmOn"), NULL, NULL, TRUE);

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[1] = 224; 
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);
}

//=============================================================================
//	Desc	: 화재발생 강제이동 Off 신호 송신
//	Date	:
//	Update	: 
//=============================================================================
BOOL CSc::FireAlarmOff()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	CString strLog;
	strLog.Format(_T("%s 화재발생 강제이동 Off 신호 송신"), DEVICE);
	WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::FireAlarmOn"));

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[1] = 0; 
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);
	//*/
}

BOOL CSc::IsValidLocation(CString& strLocation)
{
	if (CLib::IsValidLocation(m_pInfo->m_pStation->GetWarehouseNo(), strLocation) == FALSE)
	{
		m_strLog.Format(_T("%s 유효하지 않은 LOCATION! [LOC=%s]"), DEVICE, strLocation);
		return FALSE;
	}

	if (CLib::GetStackerNum(m_pInfo->m_pStation->GetWarehouseNo(), strLocation) != m_nNumber)
	{
		m_strLog.Format(_T("%s 유효하지 않은 LOCATION! [S/C 번호 불일치] [SC=%d, LOC=%s]"), DEVICE, m_nNumber, strLocation);
		return FALSE;
	}

	if (CLib::IsDisableLocation(m_pInfo->m_pStation->GetWarehouseNo(), strLocation))
	{
		m_strLog.Format(_T("%s 금지된 LOCATION! [LOC=%s]"), DEVICE, strLocation);
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSc::InvokeCheck()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if ((m_pInfo->m_ScInfo[0].m_bInvoke != FALSE) || 
		(m_pInfo->IsWaitStatus() == FALSE) || 
		(m_pInfo->IsProductEmpty() == FALSE) ||
		(m_pInfo->IsOnlineMode() == FALSE) || 
		(m_pInfo->IsAutoMode() == FALSE) || 
		(m_pInfo->IsActive() == FALSE))
		return;

//	if (m_pInfo->m_ucJobCount > 0)
//	{
//		CString strLog;
//		strLog.Format(_T("%s 지상반에 %d개의 작업이 존재합니다. 삭제후 진행해주세요!"), DEVICE, m_pInfo->m_ucJobCount);
//		m_pDoc->Alarm(LOG_POS_SC, strLog);
//		return;
//	}

	switch (m_pInfo->m_nPrevRoutine)
	{
	case CScInfo::enRoutineStore:
		{
			if (RetrieveRoutine())
				break;

			if (RackToRackRoutine())
				break;

			if (StoreRoutine())
				break;
		}
		break;

	case CScInfo::enRoutineRetrieve:
		{
			if (RackToRackRoutine())
				break;

			if (StoreRoutine())
				break;

			if (RetrieveRoutine())
				break;
		}
		break;

	case CScInfo::enRoutineRackToRack:
		{
			if (StoreRoutine())
				break;

			if (RetrieveRoutine())
				break;

			if (RackToRackRoutine())
				break;
		}
		break;
	}
}

void CSc::CompleteCheck()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if( (m_pInfo->m_ScInfo[0].m_bInvoke == FALSE) || 
		(m_pInfo->m_ScInfo[0].m_nLuggNum != m_pInfo->m_ScInfo[0].m_nInternalLuggNum) ||
		(m_pInfo->IsCompleteStatus() == FALSE) || 
		(m_pInfo->IsWaitStatus() == FALSE) || 
	   ((m_pInfo->m_ucPrevStatus != CScInfo::enRunning) && (m_pInfo->m_ucPrevStatus != CScInfo::enError)) ||
		(m_pInfo->IsProductEmpty() == FALSE) )
		return;

	m_pInfo->m_ScInfo[0].m_bInvoke = FALSE;
	m_pInfo->m_bModified = TRUE;

	if (m_pInfo->m_nInternalJobType == enJobTypeManual)
	{
		m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
		m_pInfo->m_ucPrevStatus = CScInfo::enWaiting;
		m_pInfo->Backup();
		return;
	}

	m_pInfo->m_tOperEnd = CTime::GetCurrentTime();

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[0].m_nInternalLuggNum);
	if (pJobItem == NULL)
	{
		strLog.Format(_T("%s 완료처리 실패! [%d번 작업정보 없음!]"), DEVICE, m_pInfo->m_ScInfo[0].m_nInternalLuggNum);
		WriteLog(LOG_TYPE_ERROR, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CRtv::CompleteCheck"), NULL, TRUE);
		return;
	}

	BYTE ucScCmdType = CScInfo::enCmdNone;
	switch (CJobItem::ConvertJobTypeToPattern(m_pInfo->m_ScInfo[0].m_nInternalJobType))
	{
	case enJobPatternSto:
	case enJobPatternR2R:
		if (m_pInfo->m_pStation->m_strID == pJobItem->m_strDestStn)
		{
			ucScCmdType = CScInfo::enStore;
		}
		break;

	case enJobPatternRet:
	case enJobPatternPR:
		if (m_pInfo->m_pStation->m_strID == pJobItem->m_strStartStn)
		{
			ucScCmdType = CScInfo::enRetrieve;
			pJobItem->m_nArriveTrackNum = m_pInfo->m_ScInfo[0].m_nArriveTrackNum;
//			pJobItem->m_nJobStatus = enJobStatusCvNew ;
			// 화재발생시 Fire Alarm Off
			if (pJobItem->m_nPriority == 119)
				FireAlarmOff();
		}
		break;
	case enJobPatternMove:
		break;
	default:
		strLog.Format(_T("%s 완료처리 불가 작업구분 이상 [작업구분=%s]"), DEVICE, CJobItem::GetJobTypeString(m_pInfo->m_ScInfo[0].m_nInternalJobType));
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::CompleteCheck"), pJobItem);
		return;
	}

	strLog.Format(_T("%s %s 작업 완료"), DEVICE, pJobItem->GetJobTypeString());
	WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CompleteCheck"), pJobItem);

	m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
	m_pInfo->m_ucPrevStatus = CScInfo::enWaiting;
	m_pInfo->Backup();
	m_pInfo->StatusReport();

	pJobItem->SetJobStatus(enJobStatusScComplete);

	if (ucScCmdType != CScInfo::enCmdNone) 
		m_pDoc->m_pJob->Complete(pJobItem, ucScCmdType);
	else 
		pJobItem->m_nArriveTrackNum = m_pInfo->m_ScInfo[0].m_nArriveTrackNum;

	// m_pDoc->m_pJob->Backup();		// @@
	m_pDoc->m_bBackupChk = TRUE;

}

void CSc::ErrorCheck()
{
	if (m_pInfo->IsErrorStatus() == FALSE)
	{
		m_pInfo->m_nPrevErrorCode = 0;
		m_pInfo->Backup();
		return;
	}

	if (m_pInfo->m_nPrevErrorCode == m_pInfo->m_nErrorCode)
		return;

	m_pInfo->m_nPrevErrorCode = m_pInfo->m_nErrorCode;
	m_pInfo->Backup();

	if (m_pInfo->m_nErrorCode == 0)
		return;

	CString strLog;
	int nLogType = m_pInfo->IsErrorCode() ? LOG_TYPE_ERROR : LOG_TYPE_ALARM;
	strLog.Format(_T("%s %s [%d=%s]"), DEVICE, m_pInfo->IsErrorCode() ? _T("ERROR") : _T("ALARM"), m_pInfo->m_nErrorCode, m_pInfo->GetErrorString());

	CJobItem* pJobItem = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[0].m_nInternalLuggNum);
	if ((m_pInfo->m_ScInfo[0].m_nInternalLuggNum == 0) || (pJobItem == NULL))
	{
		WriteLog(nLogType, strLog, _T("CSc::ErrorCheck"), NULL, NULL, TRUE);
		return;
	}

	switch (m_pInfo->m_nErrorCode)
	{
	case enScErrorEmptyRetrieve:
		{
			if (pJobItem->GetJobPattern() == enJobPatternSto)
			{
				strLog.Format(_T("%s 출고중 공출고에러.. SC 체크"), DEVICE);
				WriteLog(nLogType, strLog, _T("CSc::ErrorCheck"), pJobItem, NULL, TRUE);
				return;
			}

			strLog.Format(_T("%s 공출고에러 [LOCATION=%s]"), DEVICE, pJobItem->GetStartStationString());
			WriteLog(nLogType, strLog, _T("CSc::ErrorCheck"), pJobItem, NULL, TRUE);

			pJobItem->SetJobStatus(enJobStatusErrorEmptyRetrieve);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;

			if (pJobItem->IsOfflineJobType())
			{
				m_pDoc->m_pJob->Remove(pJobItem);
				return;
			}

		}
		break;

	case enScErrorDualStore:
		{
			if (pJobItem->GetJobPattern() == enJobPatternRet || 
				pJobItem->GetJobPattern() == enJobPatternPR)
			{
				strLog.Format(_T("%s 이중입고에러.. SC 체크"), DEVICE);
				WriteLog(nLogType, strLog, _T("CSc::ErrorCheck"), pJobItem, NULL, TRUE);
				return;
			}

			strLog.Format(_T("%s 이중입고 에러! [LOCATION=%s]"), DEVICE, pJobItem->GetDestStationString());
			WriteLog(nLogType, strLog, _T("CSc::ErrorCheck"), pJobItem, NULL, TRUE);

			pJobItem->SetJobStatus(enJobStatusErrorDualStore);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;

			if (pJobItem->IsOfflineJobType())
				return;

		}
		break;

	default:
		WriteLog(nLogType, strLog, _T("CSc::ErrorCheck"), pJobItem, NULL, TRUE);
		break;
	}
}

BOOL CSc::ErrorRoutine()
{
	if ((m_pInfo->m_ScInfo[0].m_bInvoke == FALSE) || 
		(m_pInfo->IsErrorStatus() == FALSE) || 
		(m_pInfo->IsDualStoreError() == FALSE))
		return FALSE;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchDualStoreJobBySc(m_nNumber);
	if (pJobItem == NULL)
	{
		strLog.Format(_T("%s 이중입고 재지정 작업을 찾을 수 없습니다."), DEVICE);
		//m_pDoc->StatusDisplay(strLog);
		return FALSE;
	}

	switch (pJobItem->GetJobPattern())
	{
	case enJobPatternSto:
	case enJobPatternW2W:
	case enJobPatternR2R:
		{
			int nStoHS = m_pInfo->GetDepartHS1();
			DEBUGER_ASSERT_VALID(nStoHS != 0);
			
			if (Store(pJobItem->m_nLuggNum, pJobItem->m_strDestLoc,  nStoHS))
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
				m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
				m_pInfo->m_ucJobStatus = CScInfo::enMoving;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				pJobItem->SetJobStatus(enJobStatusScInvoke);

				// m_pDoc->m_pJob->Backup();		// @@
				m_pDoc->m_bBackupChk = TRUE;

				strLog.Format(_T("%s %s작업 이중입고 재지정"), DEVICE, pJobItem->GetJobTypeString());
				WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::ErrorRoutine"), pJobItem);
			}
			else
			{
				strLog.Format(_T("%s %s작업 이중입고 재지정 실패! [%s]"), DEVICE, pJobItem->GetJobTypeString(), m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::ErrorRoutine"), pJobItem, NULL, TRUE);
				return FALSE;
// Update : 2014.06.12
/*				// 실패가 가짜일수도 있다.
				::Sleep(3000);
				ReadStatus();

				strLog.Format(_T("%s %s작업 이중입고 재지정 전송 실패 후 작업번호 [%04d]"), DEVICE, m_pInfo->m_nLuggNum);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::ErrorRoutine"), pJobItem);

				// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
				// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
				// pJobItem->m_nLuggNum : 작업정보의 작업번호 
				if (m_pInfo->m_nLuggNum == pJobItem->m_nLuggNum)
				{
					m_pInfo->m_bInvoke = TRUE;
					m_pInfo->m_nInternalLuggNum = pJobItem->m_nLuggNum;
					m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;
					m_pInfo->m_bModified = TRUE;
					m_pInfo->Backup();

					pJobItem->SetJobStatus(enJobStatusScInvoke);

					// m_pDoc->m_pJob->Backup();		// @@
					m_pDoc->m_bBackupChk = TRUE;

					strLog.Format(_T("%s %s작업 이중입고 재지정"), DEVICE, pJobItem->GetJobTypeString());
					WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::ErrorRoutine"), pJobItem);
				}*/
//----------------------------------------------------------------------------------
			}
		}
		break;
/*
	case enJobPatternR2R:
		{
			if (RackToRack(pJobItem->m_nLuggNum, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc))
			{
				m_pInfo->m_bInvoke = TRUE;
				m_pInfo->m_nInternalLuggNum = pJobItem->m_nLuggNum;
				m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				pJobItem->SetJobStatus(enJobStatusScInvoke);
				m_pDoc->m_pJob->Backup();

				strLog.Format(_T("%s %s작업 이중입고 재지정"), DEVICE, pJobItem->GetJobTypeString());
				WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::ErrorRoutine"), pJobItem);
			}
			else
			{
				strLog.Format(_T("%s %s작업 이중입고 재지정 실패! [%s]"), DEVICE, pJobItem->GetJobTypeString(), m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::ErrorRoutine"), pJobItem, TRUE);
				return FALSE;
			}
		}
		break;
*/
	default:
		strLog.Format(_T("%s 이중입고 재지정 실패! [유효하지 않은 작업구분] [%s]"), DEVICE, pJobItem->GetJobTypeString());
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::ErrorRoutine"), pJobItem, NULL, TRUE);
		return FALSE;
	}

	return TRUE;
}

BOOL CSc::StoreRoutine()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pInfo->m_pStation != NULL);

	if (m_pInfo->m_bStoSuspend)
		return FALSE;

	CString strLog;
	CTrackHS* pStoHS = NULL;
	CJobItem* pJobItem = NULL;

	for (int i=0; i<m_pInfo->m_pStosHS.GetSize(); ++i)
	{
		pStoHS = m_pInfo->m_pStosHS.GetNextTrackHS();
		DEBUGER_ASSERT_VALID(pStoHS != NULL);
		DEBUGER_ASSERT_VALID(pStoHS->m_pTrack != NULL);

		if ((pStoHS->m_pTrack->m_bSuspend == TRUE) ||
			(pStoHS->m_pTrack->m_nLuggNum == 0) || 
			(pStoHS->m_pTrack->IsScStoHsReady() == FALSE))
			continue;

		if ((pJobItem = m_pDoc->m_pJob->Find(pStoHS->m_pTrack->m_nLuggNum)) == NULL)
		{
			strLog.Format(_T("%s 작업정보가 존재하지 않습니다. [%s]"), DEVICE, pStoHS->ToString());
			//m_pDoc->StatusDisplay(strLog, FALSE);
			continue;
		}

		if (pJobItem->m_strDestStn != m_pInfo->m_pStation->m_strID)
			return HsToHsRoutine(pJobItem, pStoHS);

		if ((pJobItem->GetJobPattern() != enJobPatternSto) && (pJobItem->GetJobPattern() != enJobPatternW2W))
		{
			strLog.Format(_T("%s 입고작업을 할 수 없습니다. [작업구분(%s) 이상] [%s]"), DEVICE, pJobItem->GetJobTypeString(), pStoHS->ToString());
			//m_pDoc->StatusDisplay(strLog, FALSE);
			continue;
		}

		if (Store(pJobItem->m_nLuggNum, pJobItem->m_strDestLoc, pStoHS->m_nPos))
		{
			m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
			m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
			m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
			m_pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS->m_pTrack->m_nNumber;
			m_pInfo->m_ucJobStatus = CScInfo::enMoving;
			m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
			m_pInfo->m_tOperStart = CTime::GetCurrentTime();
			m_pInfo->m_bModified = TRUE;
			m_pInfo->Backup();

			pJobItem->SetJobStatus(enJobStatusScInvoke);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;

			strLog.Format(_T("%s 입고지시 [HS#%d -> %s]"), DEVICE, pStoHS->m_nPos, pJobItem->m_strDestLoc);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::StoreRoutine"), pJobItem);
			return TRUE;
		}
		else
		{
			strLog.Format(_T("%s 입고지시 실패! [HS#%d -> %s] [%s]"), DEVICE, pStoHS->m_nPos, pJobItem->m_strDestLoc, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::StoreRoutine"), pJobItem);
			continue;

// Update : 2014.06.12
/*			// 실패가 가짜일수도 있다.
			::Sleep(3000);
			ReadStatus();

			strLog.Format(_T("%s 입고지시 전송 실패 후 작업번호 [%04d]"), DEVICE, m_pInfo->m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::StoreRoutine"), pJobItem);

			// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
			// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
			// pJobItem->m_nLuggNum : 작업정보의 작업번호 
			if (m_pInfo->m_nLuggNum == pJobItem->m_nLuggNum)
			{
				m_pInfo->m_bInvoke = TRUE;
				m_pInfo->m_nInternalLuggNum = pJobItem->m_nLuggNum;
				m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;
				m_pInfo->m_nDepartTrackNum = pStoHS->m_pTrack->m_nNumber;
				m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
				m_pInfo->m_tOperStart = CTime::GetCurrentTime();
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				pJobItem->SetJobStatus(enJobStatusScInvoke);
				// m_pDoc->m_pJob->Backup();		// @@
				m_pDoc->m_bBackupChk = TRUE;

				strLog.Format(_T("%s 입고지시 [HS#%d -> %s]"), DEVICE, pStoHS->m_nPos, pJobItem->m_strDestLoc);
				WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::StoreRoutine"), pJobItem);
				return TRUE;
			}*/
//------------------------------------------------------------------------------------------
		}
	}

	return FALSE;
}

BOOL CSc::HsToHsRoutine(CJobItem* pJobItem, CTrackHS* pStoHS)
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if ((pJobItem == NULL) || (pStoHS == NULL))
		return FALSE;

	CString strLog;
	BOOL bResult = FALSE;
	CTrackHS* pRetHS = NULL;
	CJobItem* pJobItemPrev = NULL;

//	// 원본
//	if ((pRetHS = m_pInfo->m_pRetsHS.FindTrackHS(pStoHS->m_nPos, pJobItem->m_strDestStn)) == NULL)
//		return FALSE;
		int j=0;
		for (j=0; j<m_pInfo->m_pRetsHS.GetSize(); ++j)
		{
			pRetHS = m_pInfo->m_pRetsHS.GetNextTrackHS();
			DEBUGER_ASSERT_VALID(pRetHS != NULL);
			DEBUGER_ASSERT_VALID(pRetHS->m_pTrack != NULL);

//			if ((pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE) &&
//				(pRetHS->m_pTrack->m_bSuspend == FALSE) &&
//				(pRetHS->m_pTrack->m_nLuggNum == 0) &&
//				(pRetHS->m_pTrack->IsRtvArriveHsReady() == TRUE))
//				break;

			if ((pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE) &&
				(pRetHS->m_pTrack->m_bSuspend == FALSE) && 
				(pRetHS->m_pTrack->m_nLuggNum == 0) && 
				(pRetHS->m_pTrack->IsScRetHsReady() == TRUE))
				break;

//----------------------------------------------------------------------------------------------------------------
		}

		int nTemp = m_pInfo->m_pRetsHS.GetSize();

		if ((j == m_pInfo->m_pRetsHS.GetSize()) || (pRetHS == NULL))
			return FALSE;


//=================================================================================================================
//	// DeadLock구간일 경우 작업대 이동 작업을 진행하지 않음!!
//=================================================================================================================
//	// Dead Lock Check Track일때  
//		// Dead Lock Check Track일때  
		if (pRetHS->m_bDeadLockTrack == TRUE)
		{
			// 데드락 이후의 트랙들을 가져와서 화물이 하나라도 있으면 작업대 이동 작업을 하지 않음!!
			CTrackInfo* pArriveTrack	 = m_pDoc->GetTrackInfo(pRetHS->m_pTrack->m_nNumber, pRetHS->m_pTrack->m_nCvPlcNum);	// 도착트랙
			DEBUGER_ASSERT_VALID(pArriveTrack		!= NULL);
			
			CTrackInfo* pArriveNextTrack = NULL;

			switch(pRetHS->m_pTrack->m_nNumber)
			{
			case 237:
				{
					pArriveNextTrack = m_pDoc->GetTrackInfo(236, 0); 
				}
				break;
			case 238:
				{
					pArriveNextTrack = m_pDoc->GetTrackInfo(239, 0); 				
				}
				break;
			}
			DEBUGER_ASSERT_VALID(pArriveNextTrack	!= NULL);

			if( pArriveTrack->IsProductSensing()		== TRUE || 
				pArriveNextTrack->IsProductSensing()	== TRUE ||
				pArriveTrack->m_nDestPos				> 0	|| 
				pArriveNextTrack->m_nDestPos			> 0	)
			{
				return FALSE;
			}
		}
		
//-----------------------------------------------------------------------------------------------------------------
	
	if ((pRetHS->m_pTrack->m_bSuspend == TRUE) || 
		(pRetHS->m_pTrack->m_nLuggNum > 0) || 
//		(pRetHS->m_pTrack->m_bDeadLock == TRUE) || 
		(pRetHS->m_pTrack->IsScRetHsReady() == FALSE))
		return FALSE;

	if (pJobItemPrev = m_pDoc->m_pJob->FetchScCompleteJob(pRetHS->m_pTrack->m_nNumber))
	{
		strLog.Format(_T("%s 완료되지 않은 이전작업이 존재합니다. 확인해주세요. ( 작업번호=%d, 출고HS=TRACK#%d, 작업상태=%s )"), 
			DEVICE, pJobItemPrev->m_nLuggNum, pJobItemPrev->m_nArriveTrackNum, pJobItemPrev->GetJobStatusString());
		m_pDoc->Alarm(LOG_POS_SC, strLog, pJobItemPrev);
		return FALSE;
	}

	if (bResult = HsToHs(pJobItem->m_nLuggNum, pStoHS->m_nPos, pRetHS->m_nPos))
	{
		m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
		m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
		m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
		m_pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS->m_pTrack->m_nNumber;
		m_pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;
		m_pInfo->m_ucJobStatus = CScInfo::enMoving;
		m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
		m_pInfo->m_tOperStart = CTime::GetCurrentTime();
		m_pInfo->m_bModified = TRUE;
		m_pInfo->Backup();

		pJobItem->SetJobStatus(enJobStatusScInvoke);
		// m_pDoc->m_pJob->Backup();		// @@
		m_pDoc->m_bBackupChk = TRUE;

		strLog.Format(_T("%s 작업대이동 지시 [HS#%d ->HS#%d]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos);
		WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::HsToHsRoutine"), pJobItem);

		if (pRetHS->m_bDeadLockTrack == TRUE)
		{
			if (pRetHS->m_pTrack->CommandDeadLockCheck(TRUE) == FALSE)
			{
				strLog.Format(_T("%s %s Dead Lock Check 기록 실패! [%s]"), DEVICE, pRetHS->ToString(), pJobItem->GetGenString());
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::HsToHsRoutine"), pJobItem, NULL, TRUE);
			}
		}
	}
	else
	{
		strLog.Format(_T("%s 작업대이동 지시 실패! [HS#%d -> HS#%d] [%s]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos, m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::HsToHsRoutine"), pJobItem);

// Update : 2014.06.12
/*		// 실패가 가짜일수도 있다.
		Sleep(3000);
		ReadStatus();

		strLog.Format(_T("%s 작업대이동 전송 실패 후 작업번호 [%04d]"), DEVICE, m_pInfo->m_nLuggNum);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::HsToHsRoutine"), pJobItem);

		// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
		// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
		// pJobItem->m_nLuggNum : 작업정보의 작업번호 
		if (m_pInfo->m_nLuggNum == pJobItem->m_nLuggNum)
		{
			m_pInfo->m_bInvoke = TRUE;
			m_pInfo->m_nInternalLuggNum = pJobItem->m_nLuggNum;
			m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;
			m_pInfo->m_nDepartTrackNum = pStoHS->m_pTrack->m_nNumber;
			m_pInfo->m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;
			m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
			m_pInfo->m_tOperStart = CTime::GetCurrentTime();
			m_pInfo->m_bModified = TRUE;
			m_pInfo->Backup();

			pJobItem->SetJobStatus(enJobStatusScInvoke);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;

			strLog.Format(_T("%s 작업대이동 지시 [HS#%d ->HS#%d]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::HsToHsRoutine"), pJobItem);

			if (pRetHS->m_bDeadLockTrack == TRUE)
			{
				if (pRetHS->m_pTrack->CommandDeadLockCheck(TRUE) == FALSE)
				{
					strLog.Format(_T("%s %s Dead Lock Check 기록 실패! [%s]"), DEVICE, pRetHS->ToString(), pJobItem->GetGenString());
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::HsToHsRoutine"), pJobItem, TRUE);
				}
			}
		}*/
//-------------------------------------------------------
	}

	return bResult;
}

BOOL CSc::RetrieveRoutine()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if (m_pInfo->m_bRetSuspend)
		return FALSE;

	CString strLog;
	CTrackHS* pRetHS = NULL;
	CJobItem* pJobItem = NULL;
	CJobItem* pJobItemPrev = NULL;

	for (int i=0; i<m_pInfo->m_pRetsHS.GetSize(); ++i)
	{
		pRetHS = m_pInfo->m_pRetsHS.GetNextTrackHS();
		DEBUGER_ASSERT_VALID(pRetHS != NULL);
		DEBUGER_ASSERT_VALID(pRetHS->m_pTrack != NULL);

		if (//(pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE) &&
			(pRetHS->m_pTrack->m_bSuspend == TRUE) || 
			(pRetHS->m_pTrack->m_nLuggNum != 0) || 
//			(pRetHS->m_pTrack->m_bDeadLock == TRUE) || 
			(pRetHS->m_pTrack->IsScRetHsReady() == FALSE))
			continue;

		if (pJobItemPrev = m_pDoc->m_pJob->FetchScCompleteJob(pRetHS->m_pTrack->m_nNumber))
		{
			strLog.Format(_T("%s 완료되지 않은 이전작업이 존재합니다. 확인해주세요. ( 작업번호=%d, 출고HS=TRACK#%d, 작업상태=%s )"), 
				DEVICE, pJobItemPrev->m_nLuggNum, pJobItemPrev->m_nArriveTrackNum, pJobItemPrev->GetJobStatusString());
			m_pDoc->Alarm(LOG_POS_SC, strLog, pJobItemPrev);
			continue;
		}

		for (int j=0; j<pRetHS->m_strDestStations.GetSize(); ++j)
		{
			if (pJobItem = m_pDoc->m_pJob->FetchRetrieveJobBySc(m_nNumber, pRetHS->m_strDestStations[j]))
			{
				// 현재 작업에 목적지가 2F BIDP 출고대이면 현재 진행중인 작업개수를 확인
				if (m_pDoc->m_pJob->IsValidScRetrieveTask(pJobItem->m_strDestStn) == FALSE)
				{
					pJobItem = NULL;
					continue;
				}

				break;
			}
		}

		if (pJobItem == NULL)
			continue;
		
		if (Retrieve(pJobItem->m_nLuggNum, pJobItem->m_strStartLoc, pRetHS->m_nPos))
		{
			m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
			m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
			m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
			m_pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;
			m_pInfo->m_ucJobStatus = CScInfo::enMoving;
			m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRetrieve;
			m_pInfo->m_tOperStart = CTime::GetCurrentTime();
			m_pInfo->m_bModified = TRUE;
			m_pInfo->Backup();

			pJobItem->SetJobStatus(enJobStatusScInvoke);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;

			strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem->m_strStartLoc, pRetHS->m_nPos);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::RetrieveRoutine"), pJobItem);

			if (pRetHS->m_bDeadLockTrack == TRUE)
			{
				if (pRetHS->m_pTrack->CommandDeadLockCheck(TRUE) == FALSE)
				{
					strLog.Format(_T("%s %s Dead Lock Check 기록 실패! [%s]"), DEVICE, pRetHS->ToString(), pJobItem->GetGenString());
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::RetrieveRoutine"), pJobItem, NULL, TRUE);
				}
			}
			
			// Fire Alarm On
			if (pJobItem->m_nPriority == 119)
				FireAlarmOn();
			else
				FireAlarmOff();

			return TRUE;
		}
		else
		{
			strLog.Format(_T("%s 출고지시 실패! [%s -> HS#%d] [%s]"), DEVICE, pJobItem->m_strStartLoc, pRetHS->m_nPos, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::RetrieveRoutine"), pJobItem);
			continue;

// Update : 2014.06.12
/*			// 실패가 가짜일수도 있다.
			::Sleep(3000);
			ReadStatus();

			strLog.Format(_T("%s 출고지시 전송 실패 후 작업번호 [%04d]"), DEVICE, m_pInfo->m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::RetrieveRoutine"), pJobItem);

			// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
			// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
			// pJobItem->m_nLuggNum : 작업정보의 작업번호 
			if (m_pInfo->m_nLuggNum == pJobItem->m_nLuggNum)
			{
				m_pInfo->m_bInvoke = TRUE;
				m_pInfo->m_nInternalLuggNum = pJobItem->m_nLuggNum;
				m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;
				m_pInfo->m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;
				m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRetrieve;
				m_pInfo->m_tOperStart = CTime::GetCurrentTime();
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				pJobItem->SetJobStatus(enJobStatusScInvoke);
				// m_pDoc->m_pJob->Backup();		// @@
				m_pDoc->m_bBackupChk = TRUE;

				strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem->m_strStartLoc, pRetHS->m_nPos);
				WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::RetrieveRoutine"), pJobItem);

				if (pRetHS->m_bDeadLockTrack == TRUE)
				{
					if (pRetHS->m_pTrack->CommandDeadLockCheck(TRUE) == FALSE)
					{
						strLog.Format(_T("%s %s Dead Lock Check 기록 실패! [%s]"), DEVICE, pRetHS->ToString(), pJobItem->GetGenString());
						WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::RetrieveRoutine"), pJobItem, TRUE);
					}
				}
				
				// Fire Alarm On
				if (pJobItem->m_nPriority == 119)
					FireAlarmOn();
				else
					FireAlarmOff();

				return TRUE;
			}*/
//-------------------------------------------------------
		}
	}

	return FALSE;
}

BOOL CSc::RackToRackRoutine()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if (m_pInfo->m_bStoSuspend)
		return FALSE;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchRackToRackJobBySc(m_nNumber);
	if (pJobItem == NULL)
		return FALSE;

	if (RackToRack(pJobItem->m_nLuggNum, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc))
	{
		m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
		m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
		m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
		m_pInfo->m_ucJobStatus = CScInfo::enMoving;
		m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRackToRack;
		m_pInfo->m_tOperStart = CTime::GetCurrentTime();
		m_pInfo->m_bModified = TRUE;
		m_pInfo->Backup();

		pJobItem->SetJobStatus(enJobStatusScInvoke);
		// m_pDoc->m_pJob->Backup();		// @@
		m_pDoc->m_bBackupChk = TRUE;

		strLog.Format(_T("%s 랙투랙지시 [%s -> %s]"), DEVICE, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc);
		WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::RackToRackRoutine"), pJobItem);
		return TRUE;
	}
	else
	{
		strLog.Format(_T("%s 랙투랙지시 실패! [%s -> %s] [%s]"), DEVICE, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc, m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::RackToRackRoutine"), pJobItem);

// Update : 2014.06.12
/*		// 실패가 가짜일수도 있다.
		::Sleep(3000);
		ReadStatus();

		strLog.Format(_T("%s 랙투랙지시 전송 실패 후 작업번호 [%04d]"), DEVICE, m_pInfo->m_nLuggNum);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::RackToRackRoutine"), pJobItem);

		// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
		// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
		// pJobItem->m_nLuggNum : 작업정보의 작업번호 
		if (m_pInfo->m_nLuggNum == pJobItem->m_nLuggNum)
		{
			m_pInfo->m_bInvoke = TRUE;
			m_pInfo->m_nInternalLuggNum = pJobItem->m_nLuggNum;
			m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;
			m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRackToRack;
			m_pInfo->m_tOperStart = CTime::GetCurrentTime();
			m_pInfo->m_bModified = TRUE;
			m_pInfo->Backup();

			pJobItem->SetJobStatus(enJobStatusScInvoke);
			// m_pDoc->m_pJob->Backup();		// @@
			m_pDoc->m_bBackupChk = TRUE;

			strLog.Format(_T("%s 랙투랙지시 [%s -> %s]"), DEVICE, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CSc::RackToRackRoutine"), pJobItem);
			return TRUE;
		}*/
//-------------------------------------------------------
	}

	return FALSE;
}

void CSc::SafeCheck()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if ((m_pInfo->IsAutoMode() == FALSE) || (m_pInfo->IsErrorStatus() == TRUE))
		return;

	CString strLog;
	CTrackHS* pTrackHS = NULL;

	for (int i=0; i<m_pInfo->m_pStosHS.GetSize(); ++i)
	{
		pTrackHS = m_pInfo->m_pStosHS[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

		if (pTrackHS->m_pTrack->IsSensorScInterlock() == TRUE)
//		if (m_pDoc->m_bScInterLock == TRUE)
		{
			if (Stop())
			{
				strLog.Format(_T("%s EMERGENCY [%s SC INTERLOCK ON]"), DEVICE, pTrackHS->ToString());
				WriteLog(LOG_TYPE_EVENT, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CSc::SafeCheck"), NULL, TRUE);
				return;
			}
			else
			{
				strLog.Format(_T("%s EMERGENCY 실패! [%s SC INTERLOCK ON]"), DEVICE, pTrackHS->ToString());
				WriteLog(LOG_TYPE_ALARM, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CSc::SafeCheck"), NULL, TRUE);
				continue;
			}
		}
	}

	for (int j=0; j<m_pInfo->m_pRetsHS.GetSize(); ++j)
	{
		pTrackHS = m_pInfo->m_pRetsHS[j];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

		if (pTrackHS->m_pTrack->IsSensorScInterlock() == TRUE)
//		if (m_pDoc->m_ScInfo[0].m_bScInterLock == TRUE)
		{
			if (Stop())
			{
				strLog.Format(_T("%s EMERGENCY [%s SC INTERLOCK ON]"), DEVICE, pTrackHS->ToString());
				WriteLog(LOG_TYPE_EVENT, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CSc::SafeCheck"), NULL, TRUE);
				return;
			}
			else
			{
				strLog.Format(_T("%s EMERGENCY 실패! [%s SC INTERLOCK ON]"), DEVICE, pTrackHS->ToString());
				WriteLog(LOG_TYPE_ALARM, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CSc::SafeCheck"), NULL, TRUE);
				continue;
			}
		}
	}
}

