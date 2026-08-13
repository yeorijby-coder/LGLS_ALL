// ScPair.cpp: implementation of the CScPair class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "ScPair.h"
#include "EcsDoc.h"
#include "EcsView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEVICE m_port.m_strDevice

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CScPair, CSc)

CScPair::CScPair(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice) : CSc(pDoc, nIndex, nNumber, lpszDevice)
{
	m_enKind = CEquipment::enSC;
//	m_pInfo = new CScPairInfo(this);
	m_pInfo = new CScInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
}

CScPair::~CScPair()
{
	delete m_pInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CScPair::MulticastInfo(CMonitorServer* pMonitorSv)
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

void CScPair::AutoRunProc(int nConnNum)
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

			if (m_pInfo->IsInvoked())
				CompleteCheck();
			else
				InvokeCheck();
		}

		UpdateCommStatus(NOTIFY_REFRESH);
	}
	else
	{
		ErrorCheck();

		UpdateCommStatus(NOTIFY_ERROR);
	}

	SafeCheck();
}

void CScPair::CommandProc()
{
	CString strLog;

	CEcsView * pView = (CEcsView *)((CMainFrame *)AfxGetApp()->GetMainWnd())->GetActiveView();
	DEBUGER_ASSERT_VALID(pView != NULL);

	int nSelectedTab = 0;
	if (pView->m_tabLayout.GetSafeHwnd())
	{
		nSelectedTab = pView->m_tabLayout.GetCurSel();
	}
	
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
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScPairCommand);
//int CScPair::Store(int nLuggNum, CString strLocation, int nHsNum, int nLuggNum2, CString strLocation2, char bUseFork, int nSize /* = 0 */)
			if (Store(_ttoi(strValues[0]), strValues[1], _ttoi(strValues[2]), _ttoi(strValues[3]), strValues[4]))
			{

				if(_ttoi(strValues[0]) > 0)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
					m_pInfo->m_nInternalJobType = enJobTypeManual;
				}

				if(_ttoi(strValues[3]) > 0)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = _ttoi(strValues[3]);
					m_pInfo->m_nInternalJobType = enJobTypeManual;
				}

//				m_pInfo->m_bInvoke = TRUE;
//				m_pInfo->m_nInternalLuggNum = _ttoi(strValues[0]);
//				m_pInfo->m_nInternalJobType = enJobTypeManual;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				strLog.Format(_T("%s 입고지시 [작업번호1=%s, 출발위치1=%s, 도착위치1=%s] [작업번호2=%s, 출발위치2=%s, 도착위치2=%s]"), 
					DEVICE, strValues[0], strValues[2], strValues[1], strValues[3], strValues[2], strValues[4]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 입고지시 실패! [작업번호1=%s, 출발위치1=%s, 도착위치1=%s] [작업번호2=%s, 출발위치2=%s, 도착위치2=%s] [%s]"), 
					DEVICE, strValues[0], strValues[2], strValues[1], strValues[3], strValues[2], strValues[4], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScPair::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdManualRet:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScPairCommand);
			if (Retrieve(_ttoi(strValues[0]), strValues[1], _ttoi(strValues[2]), _ttoi(strValues[3]), strValues[4]))
			{
				if(_ttoi(strValues[0]) > 0)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
					m_pInfo->m_nInternalJobType = enJobTypeManual;
				}

				if(_ttoi(strValues[3]) > 0)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = _ttoi(strValues[3]);
					m_pInfo->m_nInternalJobType = enJobTypeManual;
				}

//				m_pInfo->m_bInvoke = TRUE;
//				m_pInfo->m_nInternalLuggNum = _ttoi(strValues[0]);
//				m_pInfo->m_nInternalJobType = enJobTypeManual;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				strLog.Format(_T("%s 출고지시 [작업번호1=%s, 출발위치1=%s, 도착위치1=%s] [작업번호2=%s, 출발위치2=%s, 도착위치2=%s]"), 
					DEVICE, strValues[0], strValues[1], strValues[2], strValues[3], strValues[4], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 출고지시 실패! [작업번호1=%s, 출발위치1=%s, 도착위치1=%s] [작업번호2=%s, 출발위치2=%s, 도착위치2=%s] [%s]"), 
					DEVICE, strValues[0], strValues[1], strValues[2], strValues[3], strValues[4], strValues[2], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScPair::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdManualHs2Hs:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScPairCommand);
			if (HsToHs(_ttoi(strValues[0]), _ttoi(strValues[1]), _ttoi(strValues[2]), _ttoi(strValues[3])))
			{
				if(_ttoi(strValues[0]) > 0)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
					m_pInfo->m_nInternalJobType = enJobTypeManual;
				}

				if(_ttoi(strValues[3]) > 0)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = _ttoi(strValues[3]);
					m_pInfo->m_nInternalJobType = enJobTypeManual;
				}
				
//				m_pInfo->m_bInvoke = TRUE;
//				m_pInfo->m_nInternalLuggNum = _ttoi(strValues[0]);
//				m_pInfo->m_nInternalJobType = enJobTypeManual;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				strLog.Format(_T("%s 직출고지시 [작업번호1=%s, 출발위치1=%s, 도착위치1=%s] [작업번호2=%s, 출발위치2=%s, 도착위치2=%s]"), 
					DEVICE, strValues[0], strValues[1], strValues[2], strValues[3], strValues[1], strValues[2]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 직출고지시 실패! [작업번호1=%s, 출발위치1=%s, 도착위치1=%s] [작업번호2=%s, 출발위치2=%s, 도착위치2=%s] [%s]"), 
					DEVICE, strValues[0], strValues[1], strValues[2], strValues[3], strValues[1], strValues[2], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScPair::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdManualR2R:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScCommand);
			if (RackToRack(_ttoi(strValues[0]), strValues[1], strValues[2], _ttoi(strValues[3]), strValues[4], strValues[5]))
			{
				if(_ttoi(strValues[0]) > 0)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = _ttoi(strValues[0]);
					m_pInfo->m_nInternalJobType = enJobTypeManual;
				}

				if(_ttoi(strValues[3]) > 0)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = _ttoi(strValues[3]);
					m_pInfo->m_nInternalJobType = enJobTypeManual;
				}

//				m_pInfo->m_bInvoke = TRUE;
//				m_pInfo->m_nInternalLuggNum = _ttoi(strValues[0]);
//				m_pInfo->m_nInternalJobType = enJobTypeManual;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				strLog.Format(_T("%s 랙투랙지시 [작업번호1=%s, 출발위치1=%s, 도착위치1=%s] [작업번호2=%s, 출발위치2=%s, 도착위치2=%s]"), 
					DEVICE, strValues[0], strValues[1], strValues[2], strValues[3], strValues[4], strValues[5]);
				WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[0]), strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 랙투랙지시 실패! [작업번호1=%s, 출발위치1=%s, 도착위치1=%s] [작업번호2=%s, 출발위치2=%s, 도착위치2=%s] [%s]"), 
					DEVICE, strValues[0], strValues[1], strValues[2], strValues[3], strValues[4], strValues[5], m_strLog);
				WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScPair::CommandProc"));
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
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s 홈복귀지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdNewLocation:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CScCmdMsg::enSizeScPairCommand);
			{
				CJobItem* pJobItem1 = m_pDoc->m_pJob->Find(_ttoi(strValues[0]));
				CJobItem* pJobItem2 = m_pDoc->m_pJob->Find(_ttoi(strValues[2]));
				if (pJobItem1 != NULL)
				{
					strLog.Format(_T("%s 이중입고 재지정 지시 Fork #1 [LOC=%s]"), 
						DEVICE, strValues[1]);
					WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"), pJobItem1);

					pJobItem1->m_strDestLoc = strValues[1];
					pJobItem1->SetJobStatus(enJobStatusDualStoreRetry);
					m_pDoc->m_pJob->Backup();
					ErrorRoutine();
				}
				else
				{
					strLog.Format(_T("%s 이중입고 재지정 지시 Fork #1 실패!  [LOC=%s]"), 
						DEVICE, strValues[1]);
					WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[0]), strLog, _T("CScPair::CommandProc"));
				} 

				if (pJobItem2 != NULL)
				{
					strLog.Format(_T("%s 이중입고 재지정 지시 Fork #2 [LOC=%s]"), 
						DEVICE, strValues[3]);
					WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"), pJobItem2);

					pJobItem2->m_strDestLoc = strValues[3];
					pJobItem2->SetJobStatus(enJobStatusDualStoreRetry);
					m_pDoc->m_pJob->Backup();
					ErrorRoutine();
				}
				else
				{
					strLog.Format(_T("%s 이중입고 재지정 지시 Fork #2 실패!  [LOC=%s]"), 
						DEVICE, strValues[3]);
					WriteLog(LOG_TYPE_ERROR, _ttoi(strValues[2]), strLog, _T("CScPair::CommandProc"));
				} 			}
			break;


		case CScCmdMsg::enCmdComplete:
			{
				strLog.Format(_T("%s 강제완료"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"));

				if (m_pInfo->m_ScInfo[0].m_bInvoke && m_pInfo->m_ScInfo[0].m_nLuggNum == 0)
					m_pInfo->m_ScInfo[0].m_nLuggNum = m_pInfo->m_ScInfo[0].m_nInternalLuggNum;

				if (m_pInfo->m_ScInfo[1].m_bInvoke && m_pInfo->m_ScInfo[1].m_nLuggNum == 0)
					m_pInfo->m_ScInfo[1].m_nLuggNum = m_pInfo->m_ScInfo[1].m_nInternalLuggNum;

				m_pInfo->m_ucJobStatus = CScInfo::enComplete;
				m_pInfo->m_ucPrevStatus = CScInfo::enRunning;
				CompleteCheck();
			}
			break;

		case CScCmdMsg::enCmdEmergency:
			if (Emergency())
			{
				strLog.Format(_T("%s EMERGENCY지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s EMERGENCY지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdActive:
			if (Active())
			{
				strLog.Format(_T("%s ACTIVE지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s ACTIVE지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdStop:
			if (Stop())
			{
				strLog.Format(_T("%s STOP지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s STOP지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdReset:
			if (Reset())
			{
				strLog.Format(_T("%s RESET지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s RESET지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdCancel:
 			if (Delete(16))
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = FALSE;
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
				m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;		// JBY 0829

				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();
								
				strLog.Format(_T("%s FORK#1 DATA DELETE지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s FORK#1 DATA DELETE지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CommandProc"));
			}
			break;

		case CScCmdMsg::enCmdCancel1:
 			if (Delete(32))
			{
				m_pInfo->m_ScInfo[1].m_bInvoke = FALSE;
				m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;
				m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;			// JBY 0829

				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();
								
				strLog.Format(_T("%s FORK#2 DATA DELETE지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s FORK#2 DATA DELETE지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CommandProc"));
			}
			break;
		case CScCmdMsg::enCmdCancel2:
 			if (Delete(64))
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = FALSE;
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
				m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;			// JBY 0829

				m_pInfo->m_ScInfo[1].m_bInvoke = FALSE;
				m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;
				m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;			// JBY 0829

				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();
								
				strLog.Format(_T("%s FORK#1,2 DATA DELETE지시"), DEVICE);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s FORK#1,2 DATA DELETE지시 실패 [%s]"), DEVICE, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CommandProc"));
			}
			break;

		default:
			m_strLog.Format(_T("%s INVALID COMMAND!"), DEVICE);
			WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, m_strLog, _T("CScPair::CommandProc"));
		}

		delete pCmdMsg;
		pCmdMsg = NULL;
	}

	m_cmdList.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CScPair::ReadStatus()
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
		if (++nCount >= 15)
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

	CEcsView * pView = (CEcsView *)((CMainFrame *)AfxGetApp()->GetMainWnd())->GetActiveView();
	DEBUGER_ASSERT_VALID(pView != NULL);

	int nSelectedTab = 0;
	if (pView->m_tabLayout.GetSafeHwnd())
	{
		nSelectedTab = pView->m_tabLayout.GetCurSel();
	}

	if (arrRxData.GetSize() < nReadWordSize*2)
		return FALSE;

	CString strDepartInfo1 = "";
	CString strArriveInfo1 = "";
	CString strDepartInfo2 = "";
	CString strArriveInfo2 = "";

	// 출발지와 목적지 정보 추출하기(D113 ~ D120)
	for(int i = 0 ; i < 8 ; i+=2)									// COUNT: 8
	{
		strTemp.Format(_T("%02d"), CLib::SwapToWord(arrRxData.GetData() + 36 + i));					// START:36
		strDepartInfo1 = strDepartInfo1 + strTemp;

		strTemp.Format(_T("%02d"), CLib::SwapToWord(arrRxData.GetData() + 44 + i));					// START:44
		strArriveInfo1 = strArriveInfo1 + strTemp;

		
		strTemp.Format(_T("%02d"), CLib::SwapToWord(arrRxData.GetData() + 56 + i));					// START:36
		strDepartInfo2 = strDepartInfo2 + strTemp;

		strTemp.Format(_T("%02d"), CLib::SwapToWord(arrRxData.GetData() + 64 + i));					// START:44
		strArriveInfo2 = strArriveInfo2 + strTemp;
	}

	m_pInfo->SetRcMode((BYTE)arrRxData[0]);								// D 95
	m_pInfo->SetScMode((BYTE)arrRxData[10]);								// D100 - L
	m_pInfo->SetProdLoad((BYTE)arrRxData[11]);							// D100 - H
	m_pInfo->SetScStatus((BYTE)arrRxData[12]);							// D101
	m_pInfo->SetHorizontalPos((BYTE)arrRxData[14]);						// D102
	m_pInfo->SetVerticalPos((BYTE)arrRxData[16]);							// D103
	m_pInfo->SetForkPos((BYTE)arrRxData[18]);								// D104
	m_pInfo->SetErrorCode((BYTE)arrRxData[20]);							// D105			- Trouble보고 할것인지 결정할것
	m_pInfo->SetForkStatus((BYTE)arrRxData[22]);							// D106
	m_pInfo->SetRcStatus((BYTE)arrRxData[28]);							// D109
	m_pInfo->SetJobStatus((BYTE)arrRxData[30]);							// D110
	m_pInfo->SetCmdType((BYTE)arrRxData[32]);								// D111		
	m_pInfo->SetLuggNum(CLib::SwapToWord(arrRxData.GetData() + 34), 1-1);	// D112
	m_pInfo->SetDepartInfo(strDepartInfo1,  0);							// D113 ~ D116		// JBY 0829
	m_pInfo->SetArriveInfo(strArriveInfo1, 0);							// D117 ~ D120		// JBY 0829
	m_pInfo->SetLuggNum(CLib::SwapToWord(arrRxData.GetData() + 54),  2-1);	// D122
	m_pInfo->SetDepartInfo(strDepartInfo2,  1);							// D113 ~ D116		// JBY 0829
	m_pInfo->SetArriveInfo(strArriveInfo2,  1);							// D117 ~ D120		// JBY 0829


//	m_pDoc->m_pEquipments.MulticastInfo(NULL);

	m_pInfo->InvokeControl();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
CString CScPair::MiddleData(const CByteArray arrData, int nFirst, int nCount)
{
	CString strTemp, strReturnData = "";

	for(int i = 0 ; i < nCount ; i++)
		strReturnData += strTemp.Format(_T("%c"), arrData.GetData() + nFirst);

	return strReturnData;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int CScPair::Store(int nLuggNum, CString strLocation, int nHsNum, int nSize /* = 0 */, BOOL bStoHS /* = FALSE */, BOOL bRetHS /* = FALSE */)
int CScPair::Store(int nLuggNum, CString strLocation, int nHsNum, int nLuggNum2, CString strLocation2, char bUseFork, int nSize /* = 0 */)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
	int nUseFork;

	TxBuff[0] = 1;									// D171 - Store - DualStore

	if(nLuggNum != 0)
	{
		TxBuff[2] = nLuggNum & 0xFF;				// D172 LOW 
		TxBuff[3] = (nLuggNum >> 8) & 0xFF;			// D172 HIGH

		TxBuff[10] = (BYTE)nHsNum;					// D176 Start Home Stand  LOW

		// Double 일경우
		if(m_pInfo->m_nForkType == 2)
			TxBuff[12] = ((CLib::GetBank(strLocation) % 4) ==0 ) ? 4 : CLib::GetBank(strLocation) % 4;// D177 Bank   LOW
		else
			TxBuff[12] = CLib::GetSide(strLocation);// D177 Bank   LOW
		TxBuff[14] = CLib::GetBay(strLocation);		// D178 Bay    LOW
		TxBuff[16] = CLib::GetLevel(strLocation);	// D179 Level  LOW
	}

	switch(bUseFork)
	{
	case	'0':
		nUseFork = 0;
		break;
	case	'1':
		nUseFork = 1;
		break;
	case	'2':
		nUseFork = 2;
		break;
	}

	TxBuff[20] = nUseFork;							// D181 (0 - USE FORK#1) (1 - USE FORK#1, #2) (2 - USE FORK#2) --- LOW

	if (nLuggNum2 != 0)
	{
		TxBuff[22] = nLuggNum2 & 0xFF;				// D182 LOW 
		TxBuff[23] = (nLuggNum2 >> 8) & 0xFF;		// D182 HIGH

	    TxBuff[30] = (BYTE)nHsNum;					// D186 Start Home Stand  LOW

		// Double 일경우
		if(m_pInfo->m_nForkType == 2)
			TxBuff[32] = ((CLib::GetBank(strLocation2) % 4) ==0 ) ? 4 : CLib::GetBank(strLocation2) % 4;			// D187 Bank   LOW
		else
			TxBuff[32] = CLib::GetSide(strLocation2);// D187 Bank   LOW
		TxBuff[34] = CLib::GetBay(strLocation2);			// D188 Bay    LOW
		TxBuff[36] = CLib::GetLevel(strLocation2);		// D189 Level  LOW
	}

	TxBuff[40] = 1;									// D191 WRITE COMPLETE DEFAULT SETTING  -HIGH
//	TxBuff[42] = nSize;						        // D192 Gen   - HIGH
	TxBuff[42] = (BYTE)nSize;						// D192 Gen   - HIGH
//	TxBuff[44] = (BYTE)bStoHS;						// D193 StoHS - HIGH
//	TxBuff[46] = (BYTE)bRetHS;						// D194 RetHS - HIGH



	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 22);
}

//int CScPair::Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nSize /* = 0 */, BOOL bStoHS /* = FALSE */, BOOL bRetHS /* = FALSE */)
int CScPair::Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nLuggNum2, CString strLocation2, char bUseFork, int nSize /* = 0 */)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
	int nUseFork;

	TxBuff[0] = 2;									// D171 - Retrieve

	if(nLuggNum != 0)
	{
		TxBuff[2] = nLuggNum & 0xFF;				// D172 LOW 
		TxBuff[3] = (nLuggNum >> 8) & 0xFF;		// D172 HIGH

		// Double 일경우
		if(m_pInfo->m_nForkType == 2)
			TxBuff[4] = ((CLib::GetBank(strLocation) % 4) ==0 ) ? 4 : CLib::GetBank(strLocation) % 4;			// D173 Bank   LOW
		else
			TxBuff[4] = CLib::GetSide(strLocation);			// D173 Bank   LOW
		TxBuff[6] = CLib::GetBay(strLocation);			// D174 Bay    LOW
		TxBuff[8] = CLib::GetLevel(strLocation);			// D175 Level  LOW

		TxBuff[18] = (BYTE)nHsNum;					// D176 Dest Home Stand  LOW
	}

	switch(bUseFork)
	{
	case	'0':
		nUseFork = 0;
		break;
	case	'1':
		nUseFork = 1;
		break;
	case	'2':
		nUseFork = 2;
		break;
	}

	TxBuff[20] = nUseFork;							// D181 (0 - USE FORK#1) (1 - USE FORK#1, #2) (2 - USE FORK#2) --- LO

	if (nLuggNum2 != 0)
	{
		TxBuff[22] = nLuggNum2 & 0xFF;				// D182 LOW 
		TxBuff[23] = (nLuggNum2 >> 8) & 0xFF;		// D182 HIGH

		// Double 일경우
		if(m_pInfo->m_nForkType == 2)
			TxBuff[24] = ((CLib::GetBank(strLocation2) % 4) ==0 ) ? 4 : CLib::GetBank(strLocation2) % 4;			// D183 Bank   LOW
		else
			TxBuff[24] = CLib::GetSide(strLocation2);			// D183 Bank   LOW
			
		TxBuff[26] = CLib::GetBay(strLocation2);			// D184 Bay    LOW
		TxBuff[28] = CLib::GetLevel(strLocation2);		// D185 Level  LOW

	    TxBuff[38] = (BYTE)nHsNum;					// D190 Dest Home Stand  LOW
	}

	TxBuff[40] = 1;									// D191 WRITE COMPLETE DEFAULT SETTING  -LOW
//	TxBuff[42] = nSize;						        // D192 Gen   - LOW
	TxBuff[42] = (BYTE)nSize;						// D192 Gen   - LOW
//	TxBuff[44] = (BYTE)bStoHS;						// D193 StoHS - HIGH
//	TxBuff[46] = (BYTE)bRetHS;						// D194 RetHS - HIGH

	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 22);

}

//int CScPair::HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nSize /* = 0 */, BOOL bStoHS /* = FALSE */, BOOL bRetHS /* = FALSE */)
int CScPair::HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nLuggNum2, char bUseFork, int nSize /* = 0 */)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
	int nUseFork;

	TxBuff[0] = 3;								    // D171 - Store - DualStore

	if(nLuggNum != 0)
	{
		TxBuff[2] = nLuggNum & 0xFF;				// D172 LOW 
		TxBuff[3] = (nLuggNum >> 8) & 0xFF;		// D172 HIGH

		TxBuff[10] = (BYTE)nHsNumFrom;				// D176 Start Home Stand  LOW
		TxBuff[18] = (BYTE)nHsNumTo;				// D176 Dest Home Stand  LOW
	}

	switch(bUseFork)
	{
	case	'0':
		nUseFork = 0;
		break;
	case	'1':
		nUseFork = 1;
		break;
	case	'2':
		nUseFork = 2;
		break;
	}

	TxBuff[20] = nUseFork;							// D181 (0 - USE FORK#1) (1 - USE FORK#1, #2) (2 - USE FORK#2) --- LO

	if (nLuggNum2 != 0)
	{
		TxBuff[22] = nLuggNum2 & 0xFF;				// D182 LOW 
		TxBuff[23] = (nLuggNum2 >> 8) & 0xFF;		// D182 HIGH

		TxBuff[30] = (BYTE)nHsNumFrom;				// D186 Start Home Stand  LOW
		TxBuff[38] = (BYTE)nHsNumTo;				// D190 Dest Home Stand  LOW
	}

	TxBuff[40] = 1;									// D191 WRITE COMPLETE DEFAULT SETTING  -LOW
//	TxBuff[42] = nSize;						        // D192 Gen   - LOW
	TxBuff[42] = (BYTE)nSize;						// D192 Gen   - LOW
//	TxBuff[44] = (BYTE)bStoHS;						// D193 StoHS - HIGH
//	TxBuff[46] = (BYTE)bRetHS;						// D194 RetHS - HIGH
	
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 22);

}

BOOL CScPair::RackToRack(int nLuggNum, CString strLocFrom, CString strLocTo, int nLuggNum2, CString strLocFrom2, CString strLocTo2, char bUseFork, int nSize /* = 0 */)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));
	int nUseFork;

	TxBuff[0] = 4;                             // D171 - Rack To Rack

	if(nLuggNum != 0)
	{
		TxBuff[2] = nLuggNum & 0xFF;		       // D172 LOW 
		TxBuff[3] = (nLuggNum >> 8) & 0xFF;       // D172 HIGH

		// Double 일경우
		if(m_pInfo->m_nForkType == 2)
			TxBuff[4] = ((CLib::GetBank(strLocFrom) % 4) ==0 ) ? 4 : CLib::GetBank(strLocFrom) % 4;			// D183 Bank   LOW
		else
			TxBuff[4] = CLib::GetSide(strLocFrom);          // D173 Bank   LOW
		TxBuff[6] = CLib::GetBay(strLocFrom);           // D174 Bay    LOW
		TxBuff[8] = CLib::GetLevel(strLocFrom);         // D175 Level  LOW

		// Double 일경우
		if(m_pInfo->m_nForkType == 2)
			TxBuff[12] = ((CLib::GetBank(strLocTo) % 4) ==0 ) ? 4 : CLib::GetBank(strLocTo) % 4;			// D183 Bank   LOW
		else
			TxBuff[12] = CLib::GetSide(strLocTo);           // D177 Bank   LOW
		TxBuff[14] = CLib::GetBay(strLocTo);            // D178 Bay    LOW
		TxBuff[16] = CLib::GetLevel(strLocTo);		   // D179 Level  LOW
	}

	switch(bUseFork)
	{
	case	'0':
		nUseFork = 0;
		break;
	case	'1':
		nUseFork = 1;
		break;
	case	'2':
		nUseFork = 2;
		break;
	}

	TxBuff[20] = nUseFork;						  // D181 (0 - USE FORK#1) (1 - USE FORK#1, #2) (2 - USE FORK#2) --- LO

	if (nLuggNum2 != 0)
	{
		TxBuff[22] = nLuggNum2 & 0xFF;			  // D182 LOW 
		TxBuff[23] = (nLuggNum2 >> 8) & 0xFF;     // D182 HIGH

		// Double 일경우
		if(m_pInfo->m_nForkType == 2)
			TxBuff[24] = ((CLib::GetBank(strLocFrom2) % 4) ==0 ) ? 4 : CLib::GetBank(strLocFrom2) % 4;			// D183 Bank   LOW
		else
			TxBuff[24] = CLib::GetSide(strLocFrom2);          // D173 Bank   LOW
		TxBuff[26] = CLib::GetBay(strLocFrom2);         // D184 Bay    LOW
		TxBuff[28] = CLib::GetLevel(strLocFrom2);       // D185 Level  LOW

		// Double 일경우
		if(m_pInfo->m_nForkType == 2)
			TxBuff[32] = ((CLib::GetBank(strLocTo2) % 4) ==0 ) ? 4 : CLib::GetBank(strLocTo2) % 4;			// D183 Bank   LOW
		else
			TxBuff[32] = CLib::GetSide(strLocTo2);          // D173 Bank   LOW
		TxBuff[34] = CLib::GetBay(strLocTo2);           // D188 Bay    LOW
		TxBuff[36] = CLib::GetLevel(strLocTo2);         // D189 Level  LOW
	}


	TxBuff[40] = 1;						          // D191 WRITE COMPLETE DEFAULT SETTING  -LOW
//	TxBuff[42] = nSize;						      // D192 Gen   - LOW
	TxBuff[42] = (BYTE)nSize;                     // D192 Gen   - LOW

	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 22);
}

BOOL CScPair::CallToHome()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 5;								// D171 - CallToHome
	TxBuff[40] = 1;								// D191 WRITE COMPLETE DEFAULT SETTING  -LOW

	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  171, 21);
}

BOOL CScPair::Emergency()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 1; 
//	TxBuff[0] = '1';  
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);
	
}

BOOL CScPair::Active()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));


	TxBuff[0] = 2;    
//	TxBuff[0] = '1';  
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);

}

BOOL CScPair::Stop()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 4; 
//	TxBuff[0] = '1';  
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);

}

BOOL CScPair::Reset()
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 8;                     
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);

}

BOOL CScPair::Delete(int nForkValue)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s 통신연결 않됨"), DEVICE);
		return FALSE;
	}

	BYTE	TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

//  TxBuff[0] = 1;
	TxBuff[0] = nForkValue;							// 16 포크#1,  32 포크#2,   64 포크#1, #2 ALL DELETE
	return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  199, 1);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CScPair::IsValidLocation(CString& strLocation)
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

void CScPair::InvokeCheck()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if ((m_pInfo->IsInvoked() == TRUE) || 
		(m_pInfo->IsWaitStatus() == FALSE) || 
		(m_pInfo->IsProductEmpty() == FALSE) ||
		(m_pInfo->IsOnlineMode() == FALSE) || 
		(m_pInfo->IsAutoMode() == FALSE) || 
		(m_pInfo->IsActive() == FALSE))
		return;

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

/*--
void CScPair::CompleteCheck()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
//======================================================================================================================================
//	// 두개의 포크가 모두 작업지시가 되지 않았거나..
//	// 두개의 포크의 작업번호가 내부 작업번호와 다르거나 ....
//	// 완료(포크1 or 2 or 모두) 상태가 아니거나 
//	// 대기 상태가 아니거나 
//	// 이전 상태가 러닝이 아니면서 에러도 아니거나 
//	// 화물이 비어있는 상태가 아니라면 (화물이 있다면)
//
//	// 완료처리를 하지 않는다. 
//======================================================================================================================================
	if(((m_pInfo->m_ScInfo[0].m_bInvoke == FALSE) && (m_pInfo->m_ScInfo[1].m_bInvoke == FALSE)) || 
	   ((m_pInfo->m_ScInfo[0].m_nLuggNum != m_pInfo->m_ScInfo[0].m_nInternalLuggNum) && 
	    (m_pInfo->m_ScInfo[1].m_nLuggNum != m_pInfo->m_ScInfo[1].m_nInternalLuggNum)) ||
		(m_pInfo->IsCompleteStatus() == FALSE) || 
		(m_pInfo->IsWaitStatus() == FALSE) || 
	   ((m_pInfo->m_ucPrevStatus != CScInfo::enRunning) && (m_pInfo->m_ucPrevStatus != CScInfo::enError)) ||
		(m_pInfo->IsProductEmpty() == FALSE) )
		return;

	if (m_pInfo->m_nInternalJobType == enJobTypeManual )
	{
		m_pInfo->m_ScInfo[0].m_bInvoke = FALSE;
		m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
		m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;

		m_pInfo->m_ScInfo[1].m_bInvoke = FALSE;
		m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;
		m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;

		m_pInfo->m_nInternalJobType = 0;
		m_pInfo->m_ucPrevStatus = CScInfo::enWaiting;

		m_pInfo->m_bModified = TRUE;
		m_pInfo->Backup();
		return;
	}

	//if (m_pInfo->m_ScInfo[1].m_nInternalJobType == enJobTypeManual )
	//{
	//	m_pInfo->m_ScInfo[1].m_bInvoke = FALSE;
	//	m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;
	//	m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;
	//	m_pInfo->m_ucPrevStatus = CScInfo::enWaiting;

	//	m_pInfo->m_bModified = TRUE;
	//	m_pInfo->Backup();
	//	return;
	//}

	m_pInfo->m_tOperEnd = CTime::GetCurrentTime();

	CString strLog;
	CJobItem* pJobItem2 = NULL;
	CJobItem* pJobItem = NULL;

	if(m_pInfo->IsCompleteStatusFork3() == TRUE && m_pInfo->m_ScInfo[0].m_nInternalLuggNum != 0 && m_pInfo->m_ScInfo[1].m_nInternalLuggNum != 0)
	{
		pJobItem = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[0].m_nInternalLuggNum);
		pJobItem2 = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[1].m_nInternalLuggNum);
		if (pJobItem == NULL || pJobItem2 == NULL)
		{
			m_pInfo->m_ScInfo[0].m_nInternalJobType = enJobTypeNone;
			m_pInfo->m_ScInfo[1].m_nInternalJobType = enJobTypeNone;

			strLog.Format(_T("%s 완료처리 실패! [%d번 작업정보 없음!]"), DEVICE, m_pInfo->m_ScInfo[0].m_nInternalLuggNum);
			WriteLog(LOG_TYPE_ERROR, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScPair::CompleteCheck"), NULL, TRUE);

			strLog.Format(_T("%s 완료처리 실패! [%d번 작업정보 없음!]"), DEVICE, m_pInfo->m_ScInfo[1].m_nInternalLuggNum);
			WriteLog(LOG_TYPE_ERROR, m_pInfo->m_ScInfo[1].m_nInternalLuggNum, strLog, _T("CScPair::CompleteCheck"), NULL, TRUE);
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
			// @@
			if (m_pInfo->m_pStation->m_strID == pJobItem->m_strStartStn ||
				m_pInfo->m_pStation->m_strID == pJobItem2->m_strStartStn )
			{
				ucScCmdType = CScInfo::enRetrieve;
				pJobItem->m_nArriveTrackNum = m_pInfo->m_ScInfo[0].m_nArriveTrackNum;
				pJobItem2->m_nArriveTrackNum = m_pInfo->m_ScInfo[1].m_nArriveTrackNum;
	//			pJobItem->m_nJobStatus = enJobStatusCvNew ;
			}
			break;
		case enJobPatternMove:
			break;
		default:
			strLog.Format(_T("%s 완료처리 불가 작업구분 이상 [작업구분=%s]"), DEVICE, CJobItem::GetJobTypeString(m_pInfo->m_ScInfo[0].m_nInternalJobType));
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CompleteCheck"), pJobItem);

			strLog.Format(_T("%s 완료처리 불가 작업구분 이상 [작업구분=%s]"), DEVICE, CJobItem::GetJobTypeString(m_pInfo->m_ScInfo[1].m_nInternalJobType));
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CompleteCheck"), pJobItem2);

			m_pInfo->m_ScInfo[0].m_nInternalJobType =  enJobTypeNone;
			m_pInfo->m_ScInfo[1].m_nInternalJobType =  enJobTypeNone;
			return;
		}

		strLog.Format(_T("%s %s 작업 완료"), DEVICE, pJobItem->GetJobTypeString());
		WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CompleteCheck"), pJobItem);

		strLog.Format(_T("%s %s 작업 완료"), DEVICE, pJobItem2->GetJobTypeString());
		WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CompleteCheck"), pJobItem2);

		m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
		m_pInfo->m_ScInfo[0].m_bInvoke = FALSE;

		m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;
		m_pInfo->m_ScInfo[1].m_bInvoke = FALSE;

		m_pInfo->m_ucPrevStatus = CScInfo::enWaiting;
		m_pInfo->m_ScInfo[0].m_nInternalJobType = enJobTypeNone;
		m_pInfo->m_ScInfo[1].m_nInternalJobType = enJobTypeNone;
		m_pInfo->Backup();

		pJobItem->SetJobStatus(enJobStatusScComplete);
		pJobItem2->SetJobStatus(enJobStatusScComplete);

		if (ucScCmdType != CScInfo::enCmdNone) 
		{
			m_pDoc->m_pJob->Complete(pJobItem, ucScCmdType);
			m_pDoc->m_pJob->Complete(pJobItem2, ucScCmdType);
		}
		else 
		{
			pJobItem->m_nArriveTrackNum = m_pInfo->m_ScInfo[0].m_nArriveTrackNum;
			pJobItem2->m_nArriveTrackNum = m_pInfo->m_ScInfo[1].m_nArriveTrackNum;
		}

//		m_pDoc->m_pJob->Backup();
	}
	else if (m_pInfo->IsCompleteStatusFork2() == TRUE && m_pInfo->m_ScInfo[1].m_nInternalLuggNum != 0)
	{
		pJobItem2 = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[1].m_nInternalLuggNum);
		if (pJobItem2 == NULL)
		{
			strLog.Format(_T("%s FORK #2 완료처리 실패! [%d번 작업정보 없음!]"), DEVICE, m_pInfo->m_ScInfo[1].m_nInternalLuggNum);
			WriteLog(LOG_TYPE_ERROR, m_pInfo->m_ScInfo[1].m_nInternalLuggNum, strLog, _T("CScPair::CompleteCheck"), NULL, TRUE);
			return;
		}

		BYTE ucScCmdType2 = CScInfo::enCmdNone;
		switch (CJobItem::ConvertJobTypeToPattern(m_pInfo->m_ScInfo[1].m_nInternalJobType))
		{
		case enJobPatternSto:
		case enJobPatternR2R:
			if (m_pInfo->m_pStation->m_strID == pJobItem2->m_strDestStn)
			{
				ucScCmdType2 = CScInfo::enStore;
			}
			break;

		case enJobPatternRet:
		case enJobPatternPR:
			if (m_pInfo->m_pStation->m_strID == pJobItem2->m_strStartStn)
			{
				ucScCmdType2 = CScInfo::enRetrieve;
				pJobItem2->m_nArriveTrackNum = m_pInfo->m_ScInfo[1].m_nArriveTrackNum;
	//			pJobItem2->m_nJobStatus = enJobStatusCvNew ;
			}
			break;
		case enJobPatternMove:
			break;
		default:
			strLog.Format(_T("%s FORK #2 완료처리 불가 작업구분 이상 [작업구분=%s]"), DEVICE, CJobItem::GetJobTypeString(m_pInfo->m_nInternalJobType));
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CompleteCheck"), pJobItem2);
			return;
		}

		strLog.Format(_T("%s %s 작업 완료"), DEVICE, pJobItem2->GetJobTypeString());
		WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CompleteCheck"), pJobItem2);

		m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;
		m_pInfo->m_ScInfo[1].m_bInvoke = FALSE;
		m_pInfo->m_ucPrevStatus = CScInfo::enWaiting;
		m_pInfo->m_ScInfo[1].m_nInternalJobType = enJobTypeNone;
		m_pInfo->Backup();

		pJobItem2->SetJobStatus(enJobStatusScComplete);

		if (ucScCmdType2 != CScInfo::enCmdNone) 
			m_pDoc->m_pJob->Complete(pJobItem2, ucScCmdType2);
		else 
			pJobItem2->m_nArriveTrackNum = m_pInfo->m_ScInfo[1].m_nArriveTrackNum;

//		m_pDoc->m_pJob->Backup();
	}
	else if(m_pInfo->IsCompleteStatusFork1() == TRUE &&m_pInfo->m_ScInfo[0].m_nInternalLuggNum != 0)
	{
		pJobItem = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[0].m_nInternalLuggNum);
		if (pJobItem == NULL)
		{
			m_pInfo->m_nInternalJobType = enJobTypeNone;
			strLog.Format(_T("%s 완료처리 실패! [%d번 작업정보 없음!]"), DEVICE, m_pInfo->m_ScInfo[0].m_nInternalLuggNum);
			WriteLog(LOG_TYPE_ERROR, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScPair::CompleteCheck"), NULL, TRUE);
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
			}
			break;
		case enJobPatternMove:
			break;
		default:
			strLog.Format(_T("%s 완료처리 불가 작업구분 이상 [작업구분=%s]"), DEVICE, CJobItem::GetJobTypeString(m_pInfo->m_nInternalJobType));
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::CompleteCheck"), pJobItem);
			m_pInfo->m_nInternalJobType = enJobTypeNone;
			return;
		}

		strLog.Format(_T("%s %s 작업 완료"), DEVICE, pJobItem->GetJobTypeString());
		WriteLog(LOG_TYPE_EVENT, strLog, _T("CScPair::CompleteCheck"), pJobItem);

		m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;
		m_pInfo->m_ScInfo[0].m_bInvoke = FALSE;
		m_pInfo->m_ucPrevStatus = CScInfo::enWaiting;
		m_pInfo->m_ScInfo[0].m_nInternalJobType = enJobTypeNone;
		m_pInfo->Backup();

		pJobItem->SetJobStatus(enJobStatusScComplete);

		if (ucScCmdType != CScInfo::enCmdNone) 
			m_pDoc->m_pJob->Complete(pJobItem, ucScCmdType);
		else 
			pJobItem->m_nArriveTrackNum = m_pInfo->m_ScInfo[0].m_nArriveTrackNum;

	}

	m_pDoc->m_pJob->Backup();
	m_pInfo->m_bModified = TRUE;

}
--*/
void CScPair::CompleteCheck()
{
	if (m_pInfo->m_ScInfo[0].m_bInvoke == FALSE && m_pInfo->m_ScInfo[1].m_bInvoke == FALSE)		return;
	if (m_pInfo->IsCompleteStatus() == FALSE)													return;
	if (m_pInfo->IsWaitStatus() == FALSE)														return;
	if (m_pInfo->IsProductEmpty() == FALSE)														return;
	if ((m_pInfo->m_ucPrevStatus != CScInfo::enRunning) && (m_pInfo->m_ucPrevStatus != CScInfo::enError))	return;
//	   ((m_pInfo->m_ScInfo[0].m_nLuggNum != m_pInfo->m_ScInfo[0].m_nInternalLuggNum) && 
//	    (m_pInfo->m_ScInfo[1].m_nLuggNum != m_pInfo->m_ScInfo[1].m_nInternalLuggNum)) ||

	CompleteCheck1();
	CompleteCheck2();
}

void CScPair::CompleteCheck1()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if (!m_pInfo->m_ScInfo[0].m_bInvoke)																return;		// JBY 0829 -
//	if (m_pInfo->m_ScInfo[0].m_nLuggNum != m_pInfo->m_ScInfo[0].m_nInternalLuggNum)						return;		// JBY 0829 -
	if (!m_pInfo->IsCompleteStatusFork1() && !m_pInfo->IsCompleteStatusFork3())							return;
//	if (m_pInfo->IsWaitStatus() == FALSE)																return;		// JBY 0829 -
//	if (m_pInfo->IsProductEmpty() == FALSE)																return;		// JBY 0829 -
//	if (m_pInfo->m_ucPrevStatus != CScInfo::enRunning && m_pInfo->m_ucPrevStatus != CScInfo::enError)	return;		// JBY 0829 -

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
		WriteLog(LOG_TYPE_ERROR, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScPair::CompleteCheck"), NULL, TRUE);
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

	pJobItem->SetJobStatus(enJobStatusScComplete);

	if (ucScCmdType != CScInfo::enCmdNone) 
		m_pDoc->m_pJob->Complete(pJobItem, ucScCmdType);
	else 
		pJobItem->m_nArriveTrackNum = m_pInfo->m_ScInfo[0].m_nArriveTrackNum;

	m_pDoc->m_pJob->Backup();

}

void CScPair::CompleteCheck2()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if (!m_pInfo->m_ScInfo[1].m_bInvoke)																return;		// JBY 0829 -
//	if (m_pInfo->m_ScInfo[1].m_nLuggNum != m_pInfo->m_ScInfo[1].m_nInternalLuggNum)						return;		// JBY 0829 -
	if (!m_pInfo->IsCompleteStatusFork2() && !m_pInfo->IsCompleteStatusFork3())							return;
//	if (m_pInfo->IsWaitStatus() == FALSE)																return;		// JBY 0829 -
//	if (m_pInfo->IsProductEmpty() == FALSE)																return;		// JBY 0829 -
//	if (m_pInfo->m_ucPrevStatus != CScInfo::enRunning && m_pInfo->m_ucPrevStatus != CScInfo::enError)	return;		// JBY 0829 -


	m_pInfo->m_ScInfo[1].m_bInvoke = FALSE;
	m_pInfo->m_bModified = TRUE;

	if (m_pInfo->m_nInternalJobType == enJobTypeManual)
	{
		m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;
		m_pInfo->m_ucPrevStatus = CScInfo::enWaiting;
		m_pInfo->Backup();
		return;
	}

	m_pInfo->m_tOperEnd = CTime::GetCurrentTime();

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[1].m_nInternalLuggNum);
	if (pJobItem == NULL)
	{
		strLog.Format(_T("%s 완료처리 실패! [%d번 작업정보 없음!]"), DEVICE, m_pInfo->m_ScInfo[1].m_nInternalLuggNum);
		WriteLog(LOG_TYPE_ERROR, m_pInfo->m_ScInfo[1].m_nInternalLuggNum, strLog, _T("CScPair::CompleteCheck"), NULL, TRUE);
		return;
	}
	

	BYTE ucScCmdType = CScInfo::enCmdNone;
	switch (CJobItem::ConvertJobTypeToPattern(m_pInfo->m_ScInfo[1].m_nInternalJobType))
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
			pJobItem->m_nArriveTrackNum = m_pInfo->m_ScInfo[1].m_nArriveTrackNum;
//			pJobItem->m_nJobStatus = enJobStatusCvNew ;
		}
		break;
	case enJobPatternMove:
		break;
	default:
		strLog.Format(_T("%s 완료처리 불가 작업구분 이상 [작업구분=%s]"), DEVICE, CJobItem::GetJobTypeString(m_pInfo->m_ScInfo[1].m_nInternalJobType));
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CSc::CompleteCheck"), pJobItem);
		return;
	}

	strLog.Format(_T("%s %s 작업 완료"), DEVICE, pJobItem->GetJobTypeString());
	WriteLog(LOG_TYPE_EVENT, strLog, _T("CSc::CompleteCheck"), pJobItem);

	m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;
	m_pInfo->m_ucPrevStatus = CScInfo::enWaiting;
	m_pInfo->Backup();

	pJobItem->SetJobStatus(enJobStatusScComplete);

	if (ucScCmdType != CScInfo::enCmdNone) 
		m_pDoc->m_pJob->Complete(pJobItem, ucScCmdType);
	else 
		pJobItem->m_nArriveTrackNum = m_pInfo->m_ScInfo[1].m_nArriveTrackNum;

	m_pDoc->m_pJob->Backup();

}

void CScPair::ErrorCheck()
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
	if ((m_pInfo->m_ScInfo[0].m_nInternalLuggNum != 0) && (pJobItem == NULL))
	{
		WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), NULL, NULL,TRUE);
		return;
	}

	CJobItem* pJobItem2 = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[1].m_nInternalLuggNum);
	if ((m_pInfo->m_ScInfo[1].m_nInternalLuggNum != 0) && (pJobItem2 == NULL))
	{
		WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), NULL,NULL, TRUE);
		return;
	}

	if ((pJobItem == NULL) && (pJobItem2 == NULL))
	{
		WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), NULL,NULL, TRUE);
		return;
	}

	switch (m_pInfo->m_nErrorCode)
	{
	case enScErrorEmptyRetrieve:
		{
			if (pJobItem->GetJobPattern() == enJobPatternSto)
			{
				strLog.Format(_T("%s 입고중 공출고에러.. SC 체크"), DEVICE);
				WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), pJobItem, NULL, TRUE);
				return;
			}

			strLog.Format(_T("%s 공출고에러 [LOCATION=%s]"), DEVICE, pJobItem->GetStartStationString());
			WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), pJobItem,  NULL, TRUE);

			pJobItem->SetJobStatus(enJobStatusErrorEmptyRetrieve);
			m_pDoc->m_pJob->Backup();

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
				strLog.Format(_T("%s 출고 및 피킹출고 중 이중입고에러.. SC 체크"), DEVICE);
				WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), pJobItem,  NULL, TRUE);
				return;
			}

			strLog.Format(_T("%s 이중입고 에러! [LOCATION=%s]"), DEVICE, pJobItem->GetDestStationString());
			WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), pJobItem, NULL, TRUE);

			pJobItem->SetJobStatus(enJobStatusErrorDualStore);
			m_pDoc->m_pJob->Backup();

			if (pJobItem->IsOfflineJobType())
				return;

		}
		break;

	case enScErrorEmptyRetrieve2:
		{
			if (pJobItem2->GetJobPattern() == enJobPatternSto)
			{
				strLog.Format(_T("%s 입고중 공출고에러.. SC 체크 [Fork #2]"), DEVICE);
				WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), pJobItem2, NULL, TRUE);
				return;
			}

			strLog.Format(_T("%s Fork #2 공출고에러 [LOCATION=%s]"), DEVICE, pJobItem2->GetStartStationString());
			WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), pJobItem2,  NULL,TRUE);

			pJobItem2->SetJobStatus(enJobStatusErrorEmptyRetrieve2);
			m_pDoc->m_pJob->Backup();

			if (pJobItem2->IsOfflineJobType())
			{
				m_pDoc->m_pJob->Remove(pJobItem2);
				return;
			}

		}
		break;

	case enScErrorDualStore2:
		{
			if (pJobItem2->GetJobPattern() == enJobPatternRet || 
				pJobItem2->GetJobPattern() == enJobPatternPR)
			{
				strLog.Format(_T("%s 출고 및 피킹출고 중 이중입고에러.. SC 체크 [Fork #2]"), DEVICE);
				WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), pJobItem2, NULL, TRUE);
				return;
			}

			strLog.Format(_T("%s Fork #2 이중입고 에러! [LOCATION=%s]"), DEVICE, pJobItem2->GetDestStationString());
			WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), pJobItem2, NULL,TRUE);

			pJobItem2->SetJobStatus(enJobStatusErrorDualStore2);
			m_pDoc->m_pJob->Backup();

			if (pJobItem2->IsOfflineJobType())
				return;

		}
		break;

	default:
		WriteLog(nLogType, strLog, _T("CScPair::ErrorCheck"), pJobItem, NULL,TRUE);
		break;
	}
}

BOOL CScPair::ErrorRoutine()
{
	if(((m_pInfo->IsInvoked() == FALSE) || (m_pInfo->IsErrorStatus() == FALSE)) && 
	   ((m_pInfo->IsDualStoreError() == TRUE) ||
		(m_pInfo->IsEmptyRetrieveError() == TRUE) ||
		(m_pInfo->IsStoreObstacle() == TRUE) ||
		(m_pInfo->IsRetrieveObstacle() == TRUE) ))
		return FALSE;

	CString strLog;
	switch(m_pInfo->m_nErrorCode) 
	{
	case enScErrorDualStore:
		if (m_pInfo->m_ScInfo[0].m_bInvoke == FALSE)
			return FALSE;
		
		if (ErrorDualStorProcess(1) == FALSE)
		{
			// 작업지시 실패
			strLog.Format(_T("%s 이중입고 재지정 실패 \n다시 작업 지시 바랍니다."), DEVICE);
//			AfxMessageBox(strLog);			// TEST 용
			//m_pDoc->StatusDisplay(strLog);
			return FALSE;
		}
		break;
	case enScErrorDualStore2:
		if (m_pInfo->m_ScInfo[1].m_bInvoke == FALSE)
			return FALSE;
		
		if (ErrorDualStorProcess(2) == FALSE)
		{
			// 작업지시 실패
			strLog.Format(_T("%s 이중입고 재지정 실패 \n다시 작업 지시 바랍니다."), DEVICE);
//			AfxMessageBox(strLog);			// TEST 용
			//m_pDoc->StatusDisplay(strLog);
			return FALSE;
		}
		break;

	case enScErrorEmptyRetrieve:
		if (m_pInfo->m_ScInfo[0].m_bInvoke == FALSE)
			return FALSE;
		
		if (ErrorEmptyRetrieveProcess(1) == FALSE)
		{
			// 작업지시 실패
			strLog.Format(_T("%s 공출고 재지정 실패 \n다시 작업 지시 바랍니다."), DEVICE);
//			AfxMessageBox(strLog);			// TEST 용
			//m_pDoc->StatusDisplay(strLog);
			return FALSE;
		}
		break;

	case enScErrorEmptyRetrieve2:
		if (m_pInfo->m_ScInfo[1].m_bInvoke == FALSE)
			return FALSE;
		
		if (ErrorEmptyRetrieveProcess(2) == FALSE)
		{
			// 작업지시 실패
			strLog.Format(_T("%s 공출고 재지정 실패 \n다시 작업 지시 바랍니다."), DEVICE);
//			AfxMessageBox(strLog);			// TEST 용
			//m_pDoc->StatusDisplay(strLog);
			return FALSE;
		}
		break;
//	default:
	}

	return TRUE;
}

BOOL CScPair::ErrorEmptyRetrieveProcess(int nFork)
{
	// 작업정보를 가져온다. 
	CJobItem* pJobItem = NULL;
	CJobItem* pJobItem2 = NULL;
	pJobItem = m_pDoc->m_pJob->FetchDualStoreJobBySc(m_nNumber);

	CString strLog;
	if (pJobItem == NULL)
	{
		strLog.Format(_T("%s 이중입고 재지정 작업을 찾을 수 없습니다."), DEVICE);
		//m_pDoc->StatusDisplay(strLog);
		return FALSE;
	}

	char bUseFork = '0';

	
	// 2번 포크를 기본적으로 설정!
	int nForkNum = 2-1;

	// 1번 포크가 아니면...
	if (nFork != 1)
	{
		// 1번 포크로 설정!
		nForkNum = 1-1;
	}

	if ( m_pInfo->m_ScInfo[nForkNum].m_bInvoke && m_pInfo->m_ScInfo[nForkNum].m_nInternalLuggNum )
	{
		pJobItem2 = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[nForkNum].m_nInternalLuggNum);
		if (pJobItem2 != NULL)
		{
			bUseFork = '1';
		}
	}

	int nRetHS = 2;		//m_pInfo->GetArriveHS1();
	DEBUGER_ASSERT_VALID(nRetHS != 0);

	// 1번 포크가 아니면 작업정보를 바꿔준다..
	int nJobPattern = pJobItem->GetJobPattern();
	if(nFork != 1)
	{
		CJobItem* pTempJobItem = NULL;
		pTempJobItem = pJobItem2;
		pJobItem2 = pJobItem;
		pJobItem = pTempJobItem;
	}

	switch (nJobPattern)
	{
	case enJobPatternRet:
	case enJobPatternW2W:
	case enJobPatternPR:
		{
			// 작업 지시를 한다. 
			if (Retrieve(	(pJobItem == NULL) ?	0	: pJobItem->m_nLuggNum, 
							(pJobItem == NULL) ?	""	: pJobItem->m_strStartLoc, 
							nRetHS, 
							(pJobItem2 == NULL) ?	0	: pJobItem2->m_nLuggNum, 
							(pJobItem2 == NULL) ?	""	: pJobItem2->m_strStartLoc, 
							bUseFork))
			{
				if (pJobItem != NULL && pJobItem2 != NULL)				// JBY 0829	-
				{														// JBY 0829 -
					pJobItem->m_nPairLuggNo = pJobItem2->m_nLuggNum;	// JBY 0829 -
					pJobItem2->m_nPairLuggNo = pJobItem->m_nLuggNum;	// JBY 0829 -
				}														// JBY 0829 -

				if (pJobItem != NULL)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
					m_pInfo->m_ScInfo[0].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;

					pJobItem->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s %s작업 공출고 재지정 Fork #1"), DEVICE, pJobItem->GetJobTypeString());
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);
				}

				if (pJobItem2 != NULL)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
					m_pInfo->m_ScInfo[1].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem2->m_nJobType;

					pJobItem2->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s %s작업 공출고 재지정 Fork #2"), DEVICE, pJobItem2->GetJobTypeString());
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);
				}

				m_pInfo->m_ucJobStatus = CScInfo::enMoving;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				m_pDoc->m_pJob->Backup();
			}
			else
			{
				if (pJobItem != NULL && pJobItem2 != NULL)
				{
					strLog.Format(_T("%s %s작업 공출고 재지정 실패! Fork #1 [%s]"), DEVICE, pJobItem->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem, NULL, TRUE);

					strLog.Format(_T("%s %s작업 공출고 재지정 실패! Fork #2 [%s]"), DEVICE, pJobItem2->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2, NULL,  TRUE);

					// 실패가 가짜일수도 있다.
					Sleep(3000);
					ReadStatus();

					strLog.Format(_T("%s 공출고 재지정  전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);

					strLog.Format(_T("%s 공출고 재지정  전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);

					// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
					if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum && m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum )
					{
						if (pJobItem != NULL && pJobItem2 != NULL)				// JBY 0829	-
						{														// JBY 0829 -
							pJobItem->m_nPairLuggNo = pJobItem2->m_nLuggNum;	// JBY 0829 -
							pJobItem2->m_nPairLuggNo = pJobItem->m_nLuggNum;	// JBY 0829 -
						}														// JBY 0829 -

						if (pJobItem != NULL)
						{
							m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
							m_pInfo->m_ScInfo[0].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;

							pJobItem->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 공출고 재지정 Fork #1"), DEVICE, pJobItem->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);
						}

						if (pJobItem2 != NULL)
						{
							m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
							m_pInfo->m_ScInfo[1].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem2->m_nJobType;

							pJobItem2->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 공출고 재지정 Fork #2"), DEVICE, pJobItem2->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);
						}

						m_pInfo->m_ucJobStatus = CScInfo::enMoving;
						m_pInfo->m_bModified = TRUE;
						m_pInfo->Backup();

						m_pDoc->m_pJob->Backup();
						
						return TRUE;
					}
				}
				else if (pJobItem != NULL)
				{
					strLog.Format(_T("%s %s작업 공출고 재지정 실패! Fork #1 [%s]"), DEVICE, pJobItem->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem,NULL,  TRUE);

					// 실패가 가짜일수도 있다.
					Sleep(3000);
					ReadStatus();

					strLog.Format(_T("%s 공출고 재지정  전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);

					// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
					if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum)
					{
						if (pJobItem != NULL)
						{
							m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
							m_pInfo->m_ScInfo[0].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;

							pJobItem->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 공출고 재지정 Fork #1"), DEVICE, pJobItem->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);
						}

						m_pInfo->m_ucJobStatus = CScInfo::enMoving;
						m_pInfo->m_bModified = TRUE;
						m_pInfo->Backup();

						m_pDoc->m_pJob->Backup();
						
						return TRUE;
					}
				}

				else if (pJobItem2 != NULL)
				{
					strLog.Format(_T("%s %s작업 공출고 재지정 실패! Fork #2 [%s]"), DEVICE, pJobItem2->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2, NULL, TRUE);

					// 실패가 가짜일수도 있다.
					Sleep(3000);
					ReadStatus();

					strLog.Format(_T("%s 공출고 재지정  전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);

					// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
					if (m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum )
					{
						if (pJobItem2 != NULL)
						{
							m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
							m_pInfo->m_ScInfo[1].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem2->m_nJobType;

							pJobItem2->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 공출고 재지정 Fork #2"), DEVICE, pJobItem2->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);
						}

						m_pInfo->m_ucJobStatus = CScInfo::enMoving;
						m_pInfo->m_bModified = TRUE;
						m_pInfo->Backup();

						m_pDoc->m_pJob->Backup();
						
						return TRUE;
					}
				}
				return FALSE;
			}
		}
		break;
	case enJobPatternR2R:
		{
			if (RackToRack(	(pJobItem == NULL) ?	0	: pJobItem->m_nLuggNum, 
							(pJobItem == NULL) ?	""	: pJobItem->m_strStartLoc, 
							(pJobItem == NULL) ?	""	: pJobItem->m_strDestLoc,
							(pJobItem2 == NULL) ?	0	: pJobItem2->m_nLuggNum, 
							(pJobItem2 == NULL) ?	""	: pJobItem2->m_strStartLoc,
							(pJobItem2 == NULL) ?	""	: pJobItem2->m_strDestLoc))
			{
				if (pJobItem != NULL)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
					m_pInfo->m_ScInfo[0].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;

					pJobItem->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s %s작업 공출고 재지정 Fork #1"), DEVICE, pJobItem->GetJobTypeString());
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);
				}
				else
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;										// JBY 0829

				if (pJobItem2 != NULL)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
					m_pInfo->m_ScInfo[1].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem2->m_nJobType;

					pJobItem2->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s %s작업 공출고 재지정 Fork #2"), DEVICE, pJobItem2->GetJobTypeString());
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);
				}
				else
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;										// JBY 0829

				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				m_pDoc->m_pJob->Backup();

			}
			else
			{
				if (pJobItem != NULL && pJobItem2 != NULL)
				{
					strLog.Format(_T("%s %s작업 공출고 재지정 실패! Fork #1 [%s]"), DEVICE, pJobItem->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem, NULL, TRUE);

					strLog.Format(_T("%s %s작업 공출고 재지정 실패! Fork #2 [%s]"), DEVICE, pJobItem2->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2, NULL, TRUE);

					// 실패가 가짜일수도 있다.
					Sleep(3000);
					ReadStatus();

					strLog.Format(_T("%s 공출고 재지정  전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);

					strLog.Format(_T("%s 공출고 재지정  전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);

					// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
					if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum && m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum )
					{
						if (pJobItem != NULL)
						{
							m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
							m_pInfo->m_ScInfo[0].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;

							pJobItem->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 공출고 재지정 Fork #1"), DEVICE, pJobItem->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);
						}
						else
							m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;										// JBY 0829

						if (pJobItem2 != NULL)
						{
							m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
							m_pInfo->m_ScInfo[1].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem2->m_nJobType;

							pJobItem2->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 공출고 재지정 Fork #2"), DEVICE, pJobItem2->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);
						}
						else
							m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;										// JBY 0829

						m_pInfo->m_bModified = TRUE;
						m_pInfo->Backup();

						m_pDoc->m_pJob->Backup();
						
						return TRUE;
					}
				}
				if (pJobItem != NULL)
				{
					strLog.Format(_T("%s %s작업 공출고 재지정 실패! Fork #1 [%s]"), DEVICE, pJobItem->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem, NULL, TRUE);

					// 실패가 가짜일수도 있다.
					Sleep(3000);
					ReadStatus();

					strLog.Format(_T("%s 공출고 재지정  전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);

					// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
					if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum)
					{
						if (pJobItem != NULL)
						{
							m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
							m_pInfo->m_ScInfo[0].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;

							pJobItem->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 공출고 재지정 Fork #1"), DEVICE, pJobItem->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem);
						}
						else
							m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;										// JBY 0829

						m_pInfo->m_bModified = TRUE;
						m_pInfo->Backup();

						m_pDoc->m_pJob->Backup();
						
						return TRUE;
					}
				}

				if (pJobItem2 != NULL)
				{
					strLog.Format(_T("%s %s작업 공출고 재지정 실패! Fork #2 [%s]"), DEVICE, pJobItem2->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2, NULL, TRUE);

					// 실패가 가짜일수도 있다.
					Sleep(3000);
					ReadStatus();

					strLog.Format(_T("%s 공출고 재지정  전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);

					// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
					if (m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum )
					{
						if (pJobItem2 != NULL)
						{
							m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
							m_pInfo->m_ScInfo[1].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem2->m_nJobType;

							pJobItem2->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 공출고 재지정 Fork #2"), DEVICE, pJobItem2->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem2);
						}
						else
							m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;										// JBY 0829

						m_pInfo->m_bModified = TRUE;
						m_pInfo->Backup();

						m_pDoc->m_pJob->Backup();

						return TRUE;
					}
				}

				return FALSE;
			}
		}
		break;
	default:
		strLog.Format(_T("%s 공출고 재지정 실패! [유효하지 않은 작업구분] [%s]"), DEVICE, pJobItem->GetJobTypeString());
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorEmptyRetrieveProcess"), pJobItem, NULL, TRUE);
		return FALSE;
	}

	// 확인요망
	return TRUE;
}

BOOL CScPair::ErrorDualStorProcess(int nFork)
{
	// 작업정보를 가져온다. 
	CJobItem* pJobItem = NULL;
	CJobItem* pJobItem2 = NULL;
	pJobItem = m_pDoc->m_pJob->FetchDualStoreJobBySc(m_nNumber);

	CString strLog;
	if (pJobItem == NULL)
	{
		strLog.Format(_T("%s 이중입고 재지정 작업을 찾을 수 없습니다."), DEVICE);
		//m_pDoc->StatusDisplay(strLog);
		return FALSE;
	}

	char bUseFork = '0';

	// 2번 포크를 기본적으로 설정!
	int nForkNum = 2-1;

	// 1번 포크가 아니면...
	if (nFork != 1)
	{
		// 1번 포크로 설정!
		nForkNum = 1-1;
	}

	if ( m_pInfo->m_ScInfo[nForkNum].m_bInvoke && m_pInfo->m_ScInfo[nForkNum].m_nInternalLuggNum )
	{
		pJobItem2 = m_pDoc->m_pJob->Find(m_pInfo->m_ScInfo[nForkNum].m_nInternalLuggNum);
		if (pJobItem2 != NULL)
		{
			bUseFork = '1';
		}
	}

	int nStoHS = 1; //m_pInfo->GetDepartHS1();
	DEBUGER_ASSERT_VALID(nStoHS != 0);

	// 1번 포크가 아니면 작업정보를 바꿔준다..
	int nJobPattern = pJobItem->GetJobPattern();
	if(nFork != 1)
	{
		CJobItem* pTempJobItem = NULL;
		pTempJobItem = pJobItem2;
		pJobItem2 = pJobItem;
		pJobItem = pTempJobItem;
	}

	switch (nJobPattern)
	{
	case enJobPatternSto:
	case enJobPatternW2W:
	case enJobPatternR2R:
		{			
			// 작업 지시를 한다. 
			if (Store(	(pJobItem == NULL) ?	0	: pJobItem->m_nLuggNum, 
						(pJobItem == NULL) ?	""	: pJobItem->m_strDestLoc, 
						nStoHS, 
						(pJobItem2 == NULL) ?	0	: pJobItem2->m_nLuggNum, 
						(pJobItem2 == NULL) ?	""	: pJobItem2->m_strDestLoc, 
						bUseFork))
			{
				if (pJobItem != NULL)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
					m_pInfo->m_ScInfo[0].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;

					pJobItem->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s %s작업 이중입고 재지정 Fork #1"), DEVICE, pJobItem->GetJobTypeString());
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem);
				}

				if (pJobItem2 != NULL)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
					m_pInfo->m_ScInfo[1].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem2->m_nJobType;

					pJobItem2->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s %s작업 이중입고 재지정 Fork #2"), DEVICE, pJobItem2->GetJobTypeString());
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem2);
				}

				m_pInfo->m_ucJobStatus = CScInfo::enMoving;
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				m_pDoc->m_pJob->Backup();
				Sleep(500L);
			}
			else
			{
				if (pJobItem != NULL && pJobItem2 != NULL)
				{
					strLog.Format(_T("%s %s작업 이중입고 재지정 실패! Fork #1 [%s]"), DEVICE, pJobItem->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem, NULL, TRUE);

					strLog.Format(_T("%s %s작업 이중입고 재지정 실패! Fork #2 [%s]"), DEVICE, pJobItem2->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem2, NULL, TRUE);

					// 실패가 가짜일수도 있다.
					Sleep(3000);
					ReadStatus();

					strLog.Format(_T("%s 이중입고 재지정 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem);

					strLog.Format(_T("%s 이중입고 재지정 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem2);

					// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
					if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum && m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum)
					{
						if (pJobItem != NULL)
						{
							m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
							m_pInfo->m_ScInfo[0].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;

							pJobItem->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 이중입고 재지정 Fork #1"), DEVICE, pJobItem->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem);
						}

						if (pJobItem2 != NULL)
						{
							m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
							m_pInfo->m_ScInfo[1].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem2->m_nJobType;

							pJobItem2->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 이중입고 재지정 Fork #2"), DEVICE, pJobItem2->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem2);
						}

						m_pInfo->m_ucJobStatus = CScInfo::enMoving;
						m_pInfo->m_bModified = TRUE;
						m_pInfo->Backup();

						m_pDoc->m_pJob->Backup();
						Sleep(500L);

						return TRUE;
					}
				}

				else if (pJobItem != NULL)
				{
					strLog.Format(_T("%s %s작업 이중입고 재지정 실패! Fork #1 [%s]"), DEVICE, pJobItem->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem, NULL, TRUE);
					
					// 실패가 가짜일수도 있다.
					Sleep(3000);
					ReadStatus();

					strLog.Format(_T("%s 이중입고 재지정 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem);

					// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
					if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum)
					{
						if (pJobItem != NULL)
						{
							m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
							m_pInfo->m_ScInfo[0].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem->m_nJobType;

							pJobItem->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 이중입고 재지정 Fork #1"), DEVICE, pJobItem->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem);
						}

						m_pInfo->m_ucJobStatus = CScInfo::enMoving;
						m_pInfo->m_bModified = TRUE;
						m_pInfo->Backup();

						m_pDoc->m_pJob->Backup();
						Sleep(500L);

						return TRUE;
					}				
				}

				else if (pJobItem2 != NULL)
				{
					strLog.Format(_T("%s %s작업 이중입고 재지정 실패! Fork #2 [%s]"), DEVICE, pJobItem2->GetJobTypeString(), m_strLog);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem2, NULL, TRUE);
					
					// 실패가 가짜일수도 있다.
					Sleep(3000);
					ReadStatus();

					strLog.Format(_T("%s 이중입고 재지정 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
					WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem2);

					// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
					if (m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum)
					{
						if (pJobItem2 != NULL)
						{
							m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
							m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
							m_pInfo->m_ScInfo[1].m_nInternalJobType = m_pInfo->m_nInternalJobType = pJobItem2->m_nJobType;

							pJobItem2->SetJobStatus(enJobStatusScInvoke);

							strLog.Format(_T("%s %s작업 이중입고 재지정 Fork #2"), DEVICE, pJobItem2->GetJobTypeString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem2);
						}

						m_pInfo->m_ucJobStatus = CScInfo::enMoving;
						m_pInfo->m_bModified = TRUE;
						m_pInfo->Backup();

						m_pDoc->m_pJob->Backup();
						Sleep(500L);

						return TRUE;
					}				
				}
				return FALSE;
			}
		}
		break;
	default:
		strLog.Format(_T("%s 이중입고 재지정 실패! [유효하지 않은 작업구분] [%s]"), DEVICE, pJobItem->GetJobTypeString());
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::ErrorDualStorProcess"), pJobItem, NULL, TRUE);
		return FALSE;
	}

	// 확인요망
	return TRUE;
}

BOOL CScPair::StoreRoutine()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pInfo->m_pStation != NULL);

	if (m_pInfo->m_bStoSuspend)
		return FALSE;

	CString strLog;
	CTrackHS* pStoHS2 = NULL;
	CTrackInfo* pStoHS1 = NULL;
	CJobItem* pJobItem1 = NULL;
	CJobItem* pJobItem2 = NULL;

	for (int i=0; i<m_pInfo->m_pStosHS.GetSize(); ++i)
	{
		pStoHS2 = m_pInfo->m_pStosHS.GetNextTrackHS();
		DEBUGER_ASSERT_VALID(pStoHS2 != NULL);
		DEBUGER_ASSERT_VALID(pStoHS2->m_pTrack != NULL);

		if ((pStoHS2->m_pTrack->m_bSuspend == TRUE) ||
			(pStoHS2->m_pTrack->m_nLuggNum == 0) || 
			(pStoHS2->m_pTrack->IsScStoHsReady() == FALSE))
			continue;

		if ((pJobItem1 = m_pDoc->m_pJob->Find(pStoHS2->m_pTrack->m_nLuggNum)) == NULL)
		{
			strLog.Format(_T("%s 작업정보가 존재하지 않습니다. [%s]"), DEVICE, pStoHS2->ToString());
			//m_pDoc->StatusDisplay(strLog, FALSE);
			continue;
		}

		if (pJobItem1->m_strDestStn != m_pInfo->m_pStation->m_strID)
			return HsToHsRoutine(pJobItem1, pStoHS2);

		if ((pJobItem1->GetJobPattern() != enJobPatternSto) && (pJobItem1->GetJobPattern() != enJobPatternW2W))
		{
			strLog.Format(_T("%s 입고작업을 할 수 없습니다. [작업구분(%s) 이상] [%s]"), DEVICE, pJobItem1->GetJobTypeString(), pStoHS2->ToString());
			//m_pDoc->StatusDisplay(strLog, FALSE);
			continue;
		}

		// Twin 작업이 있는지 확인할 것!! - 트랙번호가 바로 이전 번호!
		pStoHS1 = m_pDoc->GetTrackInfo(pStoHS2->m_nPair, 0);	//m_pInfo->m_pStosHS.FetchByPair(pStoHS->m_nPair);	
		DEBUGER_ASSERT_VALID(pStoHS2 != NULL);

		for (int i=0; i<1; i++)		// 둘다 홈스탠드 상태는 ON 이나 Fork1 만 작업 가능한 경우 처리 위해  for loop 사용 ...
		{

			// 두번째 HS의 화물 감지 유무 확인!!
			if ((pStoHS1->m_bSuspend == TRUE) ||
				(pStoHS1->m_nLuggNum == 0) || 
				(pStoHS1->IsScStoHsReady() == FALSE))
			{
				// 준비안됨
				break;
			}
			else
			{
				if ((pJobItem2 = m_pDoc->m_pJob->Find(pStoHS1->m_nLuggNum)) == NULL)
				{
					strLog.Format(_T("%s 작업정보가 존재하지 않습니다. [TR %d]"), DEVICE, pStoHS1->m_nNumber);
					//m_pDoc->StatusDisplay(strLog, FALSE);
					break;
				}

				// 이런 경우는 있으면 안된다...
				if (pJobItem2->m_strDestStn != m_pInfo->m_pStation->m_strID)
	//				return HsToHsRoutine(pJobItem1, pStoHS);
					pJobItem2 = NULL;
					break;

				if ((pJobItem2->GetJobPattern() != enJobPatternSto) && (pJobItem2->GetJobPattern() != enJobPatternW2W))
				{
					strLog.Format(_T("%s 입고작업을 할 수 없습니다. [작업구분(%s) 이상] [TR %d]"), DEVICE, pJobItem2->GetJobTypeString(), pStoHS1->m_nNumber);
					//m_pDoc->StatusDisplay(strLog, FALSE);
					pJobItem2 = NULL;
					break;
				}
			}
		} // for loop


		// 입고시에 Fork2 작업에 마지막 Bay 입고 작업이 걸리면 pJobItem2는 NULL로 변경하는 로직 필요  by NJC

		if (pJobItem2 != NULL)
		{
			int nBay2 = CLib::GetBay(pJobItem2->m_strStartLoc);
			int nLastBay = (m_pInfo->m_pEquipment->m_nNumber == 7) ? 20 : 24;
			if (nBay2 >= nLastBay)		pJobItem2 = NULL;
		}

		char bUseFork = (pJobItem1 != NULL && pJobItem2 != NULL) ? '1': '0';

		if (Store(	(pJobItem1 == NULL) ?	  0		: pJobItem1->m_nLuggNum, 
					(pJobItem1 == NULL) ? "0000000" : pJobItem1->m_strDestLoc, 
					(pStoHS2 == NULL) ?		  0		: pStoHS2->m_nPos,
					(pJobItem2 == NULL) ?	  0		: pJobItem2->m_nLuggNum, 
					(pJobItem2 == NULL) ? "0000000" : pJobItem2->m_strDestLoc, bUseFork) )
		{
			if (pJobItem1 != NULL)
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;																	// JBY 0829
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem1->m_nLuggNum;										// JBY 0829
				m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem1->m_nJobType;			// JBY 0830
				m_pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS2->m_pTrack->m_nNumber;									// JBY 0829

				pJobItem1->SetJobStatus(enJobStatusScInvoke);

				strLog.Format(_T("%s 입고지시 [HS#%d -> %s]"), DEVICE, pStoHS2->m_nPos, pJobItem1->m_strDestLoc);
				WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::StoreRoutine"), pJobItem1);
			}
			else
				m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;										// JBY 0829


			if (pJobItem2 != NULL)
			{
				m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;																	// JBY 0829
				m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;										// JBY 0829
				m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;			// JBY 0830
				m_pInfo->m_ScInfo[1].m_nDepartTrackNum = pStoHS1->m_nNumber;											// JBY 0829

				pJobItem2->SetJobStatus(enJobStatusScInvoke);

				strLog.Format(_T("%s 입고지시 [HS#%d -> %s]"), DEVICE, pStoHS2->m_nPos, pJobItem2->m_strDestLoc);
				WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::StoreRoutine"), pJobItem2);
			}
			else
				m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;										// JBY 0829

			m_pInfo->m_ucJobStatus = CScInfo::enMoving;
			m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
			m_pInfo->m_tOperStart = CTime::GetCurrentTime();
			m_pInfo->m_bModified = TRUE;
			m_pInfo->Backup();

			m_pDoc->m_pJob->Backup();
			Sleep(500L);

			return TRUE;
		}
		else
		{
			if (pJobItem1 != NULL && pJobItem2 != NULL)
			{
				strLog.Format(_T("%s 입고지시 실패! Fork#1 [HS#%d -> %s] [%s]"), DEVICE, pStoHS2->m_nPos, pJobItem1->m_strDestLoc, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::StoreRoutine"), pJobItem1);

				strLog.Format(_T("%s 입고지시 실패! Fork#2 [HS#%d -> %s] [%s]"), DEVICE, pStoHS2->m_nPos, pJobItem2->m_strDestLoc, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::StoreRoutine"), pJobItem2);

				// 실패가 가짜일수도 있다.
				Sleep(3000);
				ReadStatus();

				strLog.Format(_T("%s 입고지시 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::StoreRoutine"), pJobItem1);

				strLog.Format(_T("%s 입고지시 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::StoreRoutine"), pJobItem2);

				// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
				// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
				// pJobItem->m_nLuggNum : 작업정보의 작업번호 
				if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem1->m_nLuggNum && m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum)
				{
					if (pJobItem1 != NULL)
					{
						m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;																	// JBY 0829
						m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem1->m_nLuggNum;										// JBY 0829
						m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem1->m_nJobType;										// JBY 0830
						m_pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS2->m_pTrack->m_nNumber;									// JBY 0829

						pJobItem1->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 입고지시 [HS#%d -> %s]"), DEVICE, pStoHS2->m_nPos, pJobItem1->m_strDestLoc);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::StoreRoutine"), pJobItem1);
					}
					else
						m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;															// JBY 0829


					if (pJobItem2 != NULL)
					{
						m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;																	// JBY 0829
						m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;										// JBY 0829
						m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;										// JBY 0830
						m_pInfo->m_ScInfo[1].m_nDepartTrackNum = pStoHS1->m_nNumber;											// JBY 0829

						pJobItem2->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 입고지시 [HS#%d -> %s]"), DEVICE, pStoHS2->m_nPos, pJobItem2->m_strDestLoc);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::StoreRoutine"), pJobItem2);
					}
					else
						m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;															// JBY 0829

					m_pInfo->m_ucJobStatus = CScInfo::enMoving;
					m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
					m_pInfo->m_tOperStart = CTime::GetCurrentTime();
					m_pInfo->m_bModified = TRUE;
					m_pInfo->Backup();

					m_pDoc->m_pJob->Backup();
					Sleep(500L);

					return TRUE;
				}
			}
			else if (pJobItem1 != NULL)
			{
				strLog.Format(_T("%s 입고지시 실패! Fork#1 [HS#%d -> %s] [%s]"), DEVICE, pStoHS2->m_nPos, pJobItem1->m_strDestLoc, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::StoreRoutine"), pJobItem1);

				// 실패가 가짜일수도 있다.
				Sleep(3000);
				ReadStatus();

				strLog.Format(_T("%s 입고지시 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::StoreRoutine"), pJobItem1);

				// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
				// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
				// pJobItem->m_nLuggNum : 작업정보의 작업번호 
				if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem1->m_nLuggNum)
				{
					if (pJobItem1 != NULL)
					{
						m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;																	// JBY 0829
						m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem1->m_nLuggNum;										// JBY 0829
						m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem1->m_nJobType;										// JBY 0830
						m_pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS2->m_pTrack->m_nNumber;									// JBY 0829

						pJobItem1->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 입고지시 [HS#%d -> %s]"), DEVICE, pStoHS2->m_nPos, pJobItem1->m_strDestLoc);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::StoreRoutine"), pJobItem1);
					}
					else
						m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;															// JBY 0829


					m_pInfo->m_ucJobStatus = CScInfo::enMoving;
					m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
					m_pInfo->m_tOperStart = CTime::GetCurrentTime();
					m_pInfo->m_bModified = TRUE;
					m_pInfo->Backup();

					m_pDoc->m_pJob->Backup();
					Sleep(500L);

					return TRUE;
				}
			}
			else if (pJobItem2 != NULL)
			{
				strLog.Format(_T("%s 입고지시 실패! Fork#2 [HS#%d -> %s] [%s]"), DEVICE, pStoHS2->m_nPos, pJobItem2->m_strDestLoc, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::StoreRoutine"), pJobItem2);

				// 실패가 가짜일수도 있다.
				Sleep(3000);
				ReadStatus();

				strLog.Format(_T("%s 입고지시 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::StoreRoutine"), pJobItem2);

				// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
				// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
				// pJobItem->m_nLuggNum : 작업정보의 작업번호 
				if (m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem1->m_nLuggNum)
				{
					if (pJobItem2 != NULL)
					{
						m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;																	// JBY 0829
						m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;										// JBY 0829
						m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;										// JBY 0830
						m_pInfo->m_ScInfo[1].m_nDepartTrackNum = pStoHS1->m_nNumber;											// JBY 0829

						pJobItem2->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 입고지시 [HS#%d -> %s]"), DEVICE, pStoHS2->m_nPos, pJobItem2->m_strDestLoc);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::StoreRoutine"), pJobItem2);
					}
					else
						m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;															// JBY 0829

					m_pInfo->m_ucJobStatus = CScInfo::enMoving;
					m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
					m_pInfo->m_tOperStart = CTime::GetCurrentTime();
					m_pInfo->m_bModified = TRUE;
					m_pInfo->Backup();

					m_pDoc->m_pJob->Backup();
					Sleep(500L);

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CScPair::HsToHsRoutine(CJobItem* pJobItem, CTrackHS* pStoHS)
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

		if ((pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE) &&
			(pRetHS->m_pTrack->m_bSuspend == FALSE) && 
			(pRetHS->m_pTrack->m_nLuggNum == 0) && 
			(pRetHS->m_pTrack->IsScRetHsReady() == TRUE))
		{
			break;
		}
	}

	int nTemp = m_pInfo->m_pRetsHS.GetSize();

	if ((j == m_pInfo->m_pRetsHS.GetSize()) || (pRetHS == NULL))
		return FALSE;
	
	if ((pRetHS->m_pTrack->m_bSuspend == TRUE) || 
		(pRetHS->m_pTrack->m_nLuggNum > 0) || 
		(pRetHS->m_pTrack->IsScRetHsReady() == FALSE))
		return FALSE;

	if (pJobItemPrev = m_pDoc->m_pJob->FetchScCompleteJob(pRetHS->m_pTrack->m_nNumber))
	{
		strLog.Format(_T("%s 완료되지 않은 이전작업이 존재합니다. 확인해주세요. [작업번호=%d] [출고HS=TR%d] [작업상태=%s]"), 
			DEVICE, pJobItemPrev->m_nLuggNum, pJobItemPrev->m_nArriveTrackNum, pJobItemPrev->GetJobStatusString());
		m_pDoc->Alarm(LOG_POS_SC, strLog, pJobItemPrev);
		return FALSE;
	}

	// 입고 H/S2에 작업정보가 있을 경우 출고 H/S2도 작업 가능한 상태인지 체크한다.
	CJobItem* pJobItemPrev2 = NULL;
	CJobItem* pJobItem2 = NULL;
	CTrackInfo* pRetHS2 = NULL;
	CTrackInfo* pStoHS2 = m_pDoc->GetTrackInfo(pStoHS->m_nPair, 0);	//m_pInfo->m_pStosHS.FetchByPair(pStoHS->m_nPair);
	DEBUGER_ASSERT_VALID(pStoHS2 != NULL);
	if ((pStoHS->m_pTrack->m_bSuspend == TRUE) ||
		(pStoHS->m_pTrack->m_nLuggNum == 0) || 
		(pStoHS->m_pTrack->IsScStoHsReady() == FALSE))
	{
		// 입고H/S2 가 준비 되지 않음!!
	}
	else
	{
		pRetHS2 = m_pDoc->GetTrackInfo(pRetHS->m_nPair, 0);	//m_pInfo->m_pRetsHS.FetchByPair(pRetHS->m_nPair);
		DEBUGER_ASSERT_VALID(pRetHS2 != NULL);

		if ((pRetHS2->m_bSuspend == TRUE) || 
			(pRetHS2->m_nLuggNum > 0) || 
			(pRetHS2->IsScRetHsReady() == FALSE))
			return FALSE;

		if ((pJobItem2 = m_pDoc->m_pJob->Find(pStoHS2->m_nLuggNum)) == NULL)
		{
			strLog.Format(_T("%s 작업정보가 존재하지 않습니다. [TR %d]"), DEVICE, pStoHS2->m_nNumber);
			//m_pDoc->StatusDisplay(strLog, FALSE);
			return FALSE;
		}

		if (pJobItemPrev2 = m_pDoc->m_pJob->FetchScCompleteJob(pRetHS2->m_nNumber))
		{
			strLog.Format(_T("%s 완료되지 않은 이전작업이 존재합니다. 확인해주세요. [작업번호=%d] [출고HS=TR%d] [작업상태=%s]"), 
				DEVICE, pJobItemPrev2->m_nLuggNum, pJobItemPrev2->m_nArriveTrackNum, pJobItemPrev2->GetJobStatusString());
			m_pDoc->Alarm(LOG_POS_SC, strLog, pJobItemPrev2);
			return FALSE;
		}

	}

	if (HsToHs ( (pJobItem == NULL) ?	0	: pJobItem->m_nLuggNum, 
				 (pStoHS == NULL) ?		0	: pStoHS->m_nPos, 
				 (pRetHS == NULL) ?		0	: pRetHS->m_nPos, 
				 (pJobItem2 == NULL) ?	0	: pJobItem2->m_nLuggNum))
	{
		if (pJobItem != NULL)
		{
			m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
			m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
			m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;						// JBY 0830
			m_pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS->m_pTrack->m_nNumber;
			m_pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;

			pJobItem->SetJobStatus(enJobStatusScInvoke);

			strLog.Format(_T("%s 작업대이동 지시 [HS#%d ->HS#%d]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::HsToHsRoutine"), pJobItem);
		}
		else
			m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;										// JBY 0829

		if (pJobItem2 != NULL)
		{
			m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
			m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
			m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;					// JBY 0830
			m_pInfo->m_ScInfo[1].m_nDepartTrackNum = pStoHS2->m_nNumber;
			m_pInfo->m_ScInfo[1].m_nArriveTrackNum = pRetHS2->m_nNumber;

			pJobItem2->SetJobStatus(enJobStatusScInvoke);

			strLog.Format(_T("%s 작업대이동 지시 [HS#%d ->HS#%d]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::HsToHsRoutine"), pJobItem2);
		}
		else
			m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;										// JBY 0829

		
		m_pInfo->m_ucJobStatus = CScInfo::enMoving;
		m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
		m_pInfo->m_tOperStart = CTime::GetCurrentTime();
		m_pInfo->m_bModified = TRUE;
		m_pInfo->Backup();

		m_pDoc->m_pJob->Backup();

		return TRUE;
	}
	else
	{
		if (pJobItem != NULL && pJobItem2 != NULL)
		{
			strLog.Format(_T("%s 작업대이동 지시 실패! [HS#%d -> HS#%d] [%s]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::HsToHsRoutine"), pJobItem);

			strLog.Format(_T("%s 작업대이동 지시 실패! [HS#%d -> HS#%d] [%s]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::HsToHsRoutine"), pJobItem2);

			// 실패가 가짜일수도 있다.
			Sleep(3000);
			ReadStatus();

			strLog.Format(_T("%s 작업대이동 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::HsToHsRoutine"), pJobItem);

			strLog.Format(_T("%s 작업대이동 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::HsToHsRoutine"), pJobItem2);

			// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
			// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
			// pJobItem->m_nLuggNum : 작업정보의 작업번호 
			if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum && m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum)
			{
				if (pJobItem != NULL)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
					m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;						// JBY 0830
					m_pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS->m_pTrack->m_nNumber;
					m_pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;

					pJobItem->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s 작업대이동 지시 [HS#%d ->HS#%d]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos);
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::HsToHsRoutine"), pJobItem);
				}
				else
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;										// JBY 0829

				if (pJobItem2 != NULL)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
					m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;					// JBY 0830
					m_pInfo->m_ScInfo[1].m_nDepartTrackNum = pStoHS2->m_nNumber;
					m_pInfo->m_ScInfo[1].m_nArriveTrackNum = pRetHS2->m_nNumber;

					pJobItem2->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s 작업대이동 지시 [HS#%d ->HS#%d]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos);
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::HsToHsRoutine"), pJobItem2);
				}
				else
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;										// JBY 0829

				
				m_pInfo->m_ucJobStatus = CScInfo::enMoving;
				m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
				m_pInfo->m_tOperStart = CTime::GetCurrentTime();
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				m_pDoc->m_pJob->Backup();

				return TRUE;
			}
		}
		else if (pJobItem != NULL)
		{
			strLog.Format(_T("%s 작업대이동 지시 실패! [HS#%d -> HS#%d] [%s]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::HsToHsRoutine"), pJobItem);
			// 실패가 가짜일수도 있다.
			Sleep(3000);
			ReadStatus();

			strLog.Format(_T("%s 작업대이동 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::HsToHsRoutine"), pJobItem);

			// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
			// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
			// pJobItem->m_nLuggNum : 작업정보의 작업번호 
			if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum)
			{
				if (pJobItem != NULL)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
					m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;						// JBY 0830
					m_pInfo->m_ScInfo[0].m_nDepartTrackNum = pStoHS->m_pTrack->m_nNumber;
					m_pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;

					pJobItem->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s 작업대이동 지시 [HS#%d ->HS#%d]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos);
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::HsToHsRoutine"), pJobItem);
				}
				else
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = 0;										// JBY 0829

				m_pInfo->m_ucJobStatus = CScInfo::enMoving;
				m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
				m_pInfo->m_tOperStart = CTime::GetCurrentTime();
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				m_pDoc->m_pJob->Backup();

				return TRUE;
			}
		}

		else if (pJobItem2 != NULL)
		{
			strLog.Format(_T("%s 작업대이동 지시 실패! [HS#%d -> HS#%d] [%s]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::HsToHsRoutine"), pJobItem2);
			// 실패가 가짜일수도 있다.
			Sleep(3000);
			ReadStatus();

			strLog.Format(_T("%s 작업대이동 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::HsToHsRoutine"), pJobItem2);

			// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
			// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
			// pJobItem->m_nLuggNum : 작업정보의 작업번호 
			if (m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum)
			{
				if (pJobItem2 != NULL)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
					m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;					// JBY 0830
					m_pInfo->m_ScInfo[1].m_nDepartTrackNum = pStoHS2->m_nNumber;
					m_pInfo->m_ScInfo[1].m_nArriveTrackNum = pRetHS2->m_nNumber;

					pJobItem2->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s 작업대이동 지시 [HS#%d ->HS#%d]"), DEVICE, pStoHS->m_nPos, pRetHS->m_nPos);
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::HsToHsRoutine"), pJobItem2);
				}
				else
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = 0;										// JBY 0829

				
				m_pInfo->m_ucJobStatus = CScInfo::enMoving;
				m_pInfo->m_nPrevRoutine = CScInfo::enRoutineStore;
				m_pInfo->m_tOperStart = CTime::GetCurrentTime();
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				m_pDoc->m_pJob->Backup();

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CScPair::RetrieveRoutine()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if (m_pInfo->m_bRetSuspend)
		return FALSE;

	CString strLog;
	CTrackHS* pRetHS = NULL;
	CJobItem* pJobItem = NULL;
	CJobItem* pJobItemPrev = NULL;
	int jj=0;

	for (int i=0; i<m_pInfo->m_pRetsHS.GetSize(); ++i)
	{
		pRetHS = m_pInfo->m_pRetsHS.GetNextTrackHS();
		DEBUGER_ASSERT_VALID(pRetHS != NULL);
		DEBUGER_ASSERT_VALID(pRetHS->m_pTrack != NULL);

		if (//(pRetHS->IsDestStationID(pJobItem->m_strDestStn) == TRUE) &&
			(pRetHS->m_pTrack->m_bSuspend == TRUE) || 
			(pRetHS->m_pTrack->m_nLuggNum != 0) || 
			(pRetHS->m_pTrack->IsScRetHsReady() == FALSE))
			continue;

		if (pJobItemPrev = m_pDoc->m_pJob->FetchScCompleteJob(pRetHS->m_pTrack->m_nNumber))
		{
			strLog.Format(_T("%s 완료되지 않은 이전작업이 존재합니다. 확인해주세요. [작업번호=%d] [출고HS=TR%d] [작업상태=%s]"), 
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
				jj = j;
				break;
			}
		}

		if (pJobItem == NULL)
			continue;
		
		// 입고 H/S2에 작업정보가 있을 경우 출고 H/S2도 작업 가능한 상태인지 체크한다.
		CJobItem* pJobItemPrev2 = NULL;
		CJobItem* pJobItem2 = NULL; 
		CTrackInfo* pRetHS2 = NULL;
//		if(pJobItem2->m_nPairLuggNo != 0)
		{
			pRetHS2 = m_pDoc->GetTrackInfo(pRetHS->m_nPair, 0);	//m_pInfo->m_pRetsHS.FetchByPair(pRetHS->m_nPair);
			DEBUGER_ASSERT_VALID(pRetHS2 != NULL);

			if ((pRetHS2->m_bSuspend == TRUE) || 
				(pRetHS2->m_nLuggNum != 0) || 
				(pRetHS2->IsScRetHsReady() == FALSE))
			{
				// 2번째 H/S에 화물이 감지가 되면 출고작업을 생성하지 않는다.
				continue;
			}
			else
			{
				if (pJobItemPrev2 = m_pDoc->m_pJob->FetchScCompleteJob(pRetHS2->m_nNumber))
				{
					strLog.Format(_T("%s 완료되지 않은 이전작업이 존재합니다. 확인해주세요. [작업번호=%d] [출고HS=TR%d] [작업상태=%s]"), 
						DEVICE, pJobItemPrev2->m_nLuggNum, pJobItemPrev2->m_nArriveTrackNum, pJobItemPrev2->GetJobStatusString());
					m_pDoc->Alarm(LOG_POS_SC, strLog, pJobItemPrev2);
					continue;
				}

				//  작업번호 2를 구할수있는 방법을 확인하라.
//				pJobItem2 = m_pDoc->m_pJob->FetchRetrieveJobBySc(m_nNumber, pRetHS->m_strDestStations[jj], pJobItem->m_nLuggNum);			//  Ver. JNC
				pJobItem2 = m_pDoc->m_pJob->FetchRetrieveJobByTwinSc(m_nNumber, pRetHS->m_strDestStations[jj], pJobItem->m_nLuggNum);		//	Ver. JBY
//				DEBUGER_ASSERT_VALID(pJobItem != NULL);
			}
		}

		{	// TWIN 타입 크레인 에서 동시 포킹 하기 위한 루틴
			// 마지막 Bay에 Fork2가 작업할 수 없으므로 두작업이 모두 마지막 Bay인 경우 Fork2 작업은 NULL로 변경하는 로직 필요함.  by NJC
			if (pJobItem != NULL && pJobItem2 != NULL)
			{
				int	nBay1 = CLib::GetBay(pJobItem->m_strStartLoc);
				int nBay2 = CLib::GetBay(pJobItem2->m_strStartLoc);
				if (nBay2 > nBay1)
				{
					CJobItem* pTemp = NULL;
					pTemp = pJobItem;
					pJobItem = pJobItem2;
					pJobItem2 = pTemp;
				}

				int nLastBay = (m_pInfo->m_pEquipment->m_nNumber == 7) ? 20 : 24;
				if (nBay2 >= nLastBay)		pJobItem2 = NULL;
			}
		}

		char bUseFork = (pJobItem != NULL && pJobItem2 != NULL) ? '1': '0';

		if (Retrieve((pJobItem == NULL) ?	  0		 : pJobItem->m_nLuggNum, 
					 (pJobItem == NULL) ? "0000000"  : pJobItem->m_strStartLoc, 
					 (pRetHS == NULL) ?		  0		 : pRetHS->m_nPos,
					 (pJobItem2 == NULL) ?	  0	  	 : pJobItem2->m_nLuggNum, 
					 (pJobItem2 == NULL) ? "0000000" : pJobItem2->m_strStartLoc, bUseFork))
		{
			// 동시 작업 일때만 PairLuggNum을 사용함!				- SC 완료후 CV에 DATA Write 관련하여...
			if (pJobItem != NULL && pJobItem2 != NULL)				// JBY 0829 -
			{														// JBY 0829	-	
				pJobItem->m_nPairLuggNo = pJobItem2->m_nLuggNum;	// JBY 0829	-
				pJobItem2->m_nPairLuggNo = pJobItem->m_nLuggNum;	// JBY 0829	-
			}														// JBY 0829 -

			if (pJobItem != NULL)
			{
				m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
				m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
				m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
				m_pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;

				pJobItem->SetJobStatus(enJobStatusScInvoke);

				strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem->m_strStartLoc, pRetHS->m_nPos);
				WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RetrieveRoutine"), pJobItem);
			}
			else
				m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;										// JBY 0829

			if (pJobItem2 != NULL)
			{
				m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
				m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
				m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;
				m_pInfo->m_ScInfo[1].m_nArriveTrackNum = pRetHS2->m_nNumber;

				pJobItem2->SetJobStatus(enJobStatusScInvoke);

				strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem2->m_strStartLoc, pRetHS->m_nPos);
				WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RetrieveRoutine"), pJobItem2);
			}
			else
				m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;										// JBY 0829

			m_pInfo->m_ucJobStatus = CScInfo::enMoving;
			m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRetrieve;
			m_pInfo->m_tOperStart = CTime::GetCurrentTime();
			m_pInfo->m_bModified = TRUE;
			m_pInfo->Backup();

			m_pDoc->m_pJob->Backup();
			Sleep(500L);
			
			return TRUE;
		}
		else
		{
			if (pJobItem != NULL && pJobItem2 != NULL)
			{
				strLog.Format(_T("%s 출고지시 실패! [%s -> HS#%d] [%s]"), DEVICE, pJobItem->m_strStartLoc, pRetHS->m_nPos, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RetrieveRoutine"), pJobItem);

				strLog.Format(_T("%s 출고지시 실패! [%s -> HS#%d] [%s]"), DEVICE, pJobItem2->m_strStartLoc, pRetHS->m_nPos, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RetrieveRoutine"), pJobItem2);

				// 실패가 가짜일수도 있다.
				Sleep(3000);
				ReadStatus();

				strLog.Format(_T("%s 출고지시 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RetrieveRoutine"), pJobItem);

				strLog.Format(_T("%s 출고지시 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RetrieveRoutine"), pJobItem2);

				// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
				// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
				// pJobItem->m_nLuggNum : 작업정보의 작업번호 
				if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum && m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum )
				{
					// 동시 작업 일때만 PairLuggNum을 사용함!				- SC 완료후 CV에 DATA Write 관련하여...
					if (pJobItem != NULL && pJobItem2 != NULL)				// JBY 0829 -
					{														// JBY 0829	-	
						pJobItem->m_nPairLuggNo = pJobItem2->m_nLuggNum;	// JBY 0829	-
						pJobItem2->m_nPairLuggNo = pJobItem->m_nLuggNum;	// JBY 0829	-
					}														// JBY 0829 -

					if (pJobItem != NULL)
					{
						m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
						m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
						m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
						m_pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;

						pJobItem->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem->m_strStartLoc, pRetHS->m_nPos);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RetrieveRoutine"), pJobItem);
					}
					else
						m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;										// JBY 0829

					if (pJobItem2 != NULL)
					{
						m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
						m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
						m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;
						m_pInfo->m_ScInfo[1].m_nArriveTrackNum = pRetHS2->m_nNumber;

						pJobItem2->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem2->m_strStartLoc, pRetHS->m_nPos);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RetrieveRoutine"), pJobItem2);
					}
					else
						m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;										// JBY 0829

					m_pInfo->m_ucJobStatus = CScInfo::enMoving;
					m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRetrieve;
					m_pInfo->m_tOperStart = CTime::GetCurrentTime();
					m_pInfo->m_bModified = TRUE;
					m_pInfo->Backup();

					m_pDoc->m_pJob->Backup();
					Sleep(500L);
					
					return TRUE;
				}
			}
			else if (pJobItem != NULL)
			{
				strLog.Format(_T("%s 출고지시 실패! [%s -> HS#%d] [%s]"), DEVICE, pJobItem->m_strStartLoc, pRetHS->m_nPos, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RetrieveRoutine"), pJobItem);

				// 실패가 가짜일수도 있다.
				Sleep(3000);
				ReadStatus();

				strLog.Format(_T("%s 출고지시 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RetrieveRoutine"), pJobItem);

				// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
				// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
				// pJobItem->m_nLuggNum : 작업정보의 작업번호 
				if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum)
				{
//					// 동시 작업 일때만 PairLuggNum을 사용함!				- SC 완료후 CV에 DATA Write 관련하여...
//					if (pJobItem != NULL && pJobItem2 != NULL)				// JBY 0829 -
//					{														// JBY 0829	-	
//						pJobItem->m_nPairLuggNo = pJobItem2->m_nLuggNum;	// JBY 0829	-
//						pJobItem2->m_nPairLuggNo = pJobItem->m_nLuggNum;	// JBY 0829	-
//					}														// JBY 0829 -

					if (pJobItem != NULL)
					{
						m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
						m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
						m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
						m_pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;

						pJobItem->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem->m_strStartLoc, pRetHS->m_nPos);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RetrieveRoutine"), pJobItem);
					}
					else
						m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;										// JBY 0829

					if (pJobItem2 != NULL)
					{
						m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
						m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
						m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;
						m_pInfo->m_ScInfo[1].m_nArriveTrackNum = pRetHS2->m_nNumber;

						pJobItem2->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem2->m_strStartLoc, pRetHS->m_nPos);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RetrieveRoutine"), pJobItem2);
					}
					else
						m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;										// JBY 0829

					m_pInfo->m_ucJobStatus = CScInfo::enMoving;
					m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRetrieve;
					m_pInfo->m_tOperStart = CTime::GetCurrentTime();
					m_pInfo->m_bModified = TRUE;
					m_pInfo->Backup();

					m_pDoc->m_pJob->Backup();
					Sleep(500L);
					
					return TRUE;
				}
			}

			else if (pJobItem2 != NULL)
			{
				strLog.Format(_T("%s 출고지시 실패! [%s -> HS#%d] [%s]"), DEVICE, pJobItem2->m_strStartLoc, pRetHS->m_nPos, m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RetrieveRoutine"), pJobItem2);

				// 실패가 가짜일수도 있다.
				Sleep(3000);
				ReadStatus();

				strLog.Format(_T("%s 출고지시 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RetrieveRoutine"), pJobItem2);

				// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
				// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
				// pJobItem->m_nLuggNum : 작업정보의 작업번호 
				if (m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem->m_nLuggNum)
				{
					// 동시 작업 일때만 PairLuggNum을 사용함!				- SC 완료후 CV에 DATA Write 관련하여...
					if (pJobItem != NULL && pJobItem2 != NULL)				// JBY 0829 -
					{														// JBY 0829	-	
						pJobItem->m_nPairLuggNo = pJobItem2->m_nLuggNum;	// JBY 0829	-
						pJobItem2->m_nPairLuggNo = pJobItem->m_nLuggNum;	// JBY 0829	-
					}														// JBY 0829 -

					if (pJobItem != NULL)
					{
						m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
						m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
						m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;
						m_pInfo->m_ScInfo[0].m_nArriveTrackNum = pRetHS->m_pTrack->m_nNumber;

						pJobItem->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem->m_strStartLoc, pRetHS->m_nPos);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RetrieveRoutine"), pJobItem);
					}
					else
						m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;										// JBY 0829

					if (pJobItem2 != NULL)
					{
						m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
						m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
						m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;
						m_pInfo->m_ScInfo[1].m_nArriveTrackNum = pRetHS2->m_nNumber;

						pJobItem2->SetJobStatus(enJobStatusScInvoke);

						strLog.Format(_T("%s 출고지시 [%s -> HS#%d]"), DEVICE, pJobItem2->m_strStartLoc, pRetHS->m_nPos);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RetrieveRoutine"), pJobItem2);
					}
					else
						m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;										// JBY 0829

					m_pInfo->m_ucJobStatus = CScInfo::enMoving;
					m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRetrieve;
					m_pInfo->m_tOperStart = CTime::GetCurrentTime();
					m_pInfo->m_bModified = TRUE;
					m_pInfo->Backup();

					m_pDoc->m_pJob->Backup();
					Sleep(500L);
					
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CScPair::RackToRackRoutine()
{
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	if (m_pInfo->m_bStoSuspend)
		return FALSE;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchRackToRackJobBySc(m_nNumber);
	if (pJobItem == NULL)
		return FALSE;

//	CJobItem* pJobItem1 = pJobItem;
	CJobItem* pJobItem2 = NULL;

	// Pair 작업 번호가 존재한다는 전제하에서...
	//if (pJobItem->m_nPairLuggNo != 0)
	{
		//CJobItem* pJobItem2 = m_pDoc->m_pJob->FetchScR2RJobByPair(pJobItem->m_nPairLuggNo);
		pJobItem2 = m_pDoc->m_pJob->FetchRackToRackJobBySc(m_nNumber, pJobItem->m_nLuggNum);
		//DEBUGER_ASSERT_VALID(pJobItem2 != NULL);
	}

	{	// TWIN 타입 크레인 에서 동시 포킹 하기 위한 루틴
		// 마지막 Bay에 Fork2가 작업할 수 없으므로 두작업이 모두 마지막 Bay인 경우 Fork2 작업은 NULL로 변경하는 로직 필요함.  by NJC
		// 랙투랙 작업이므로 Bay 체크시에 From 및 To 로케이션 모두 확인해야 함.
		if (pJobItem != NULL && pJobItem2 != NULL)
		{
			int	nBay1 = CLib::GetBay(pJobItem->m_strStartLoc);
			int nBay2 = CLib::GetBay(pJobItem2->m_strStartLoc);
			if (nBay2 > nBay1)
			{
				CJobItem* pTemp = NULL;
				pTemp = pJobItem;
				pJobItem = pJobItem2;
				pJobItem2 = pTemp;
			}
			int nLastBay = (m_pInfo->m_pEquipment->m_nNumber == 7) ? 20 : 24;
			if (nBay2 >= nLastBay)		pJobItem2 = NULL;
		}
	}

	char bUseFork = (pJobItem != NULL && pJobItem2 != NULL) ? '1': '0';

	if ( RackToRack((pJobItem == NULL)  ?	  0		: pJobItem->m_nLuggNum, 
					(pJobItem == NULL)  ? "0000000" : pJobItem->m_strStartLoc, 
					(pJobItem == NULL)  ? "0000000" : pJobItem->m_strDestLoc, 
					(pJobItem2 == NULL) ?	  0		: pJobItem2->m_nLuggNum, 
					(pJobItem2 == NULL) ? "0000000" : pJobItem2->m_strStartLoc, 
					(pJobItem2 == NULL) ? "0000000" : pJobItem2->m_strDestLoc, bUseFork) )
	{
		if (pJobItem != NULL)
		{
			m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
			m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
			m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;					// JBY 0830

			pJobItem->SetJobStatus(enJobStatusScInvoke);

			strLog.Format(_T("%s 랙투랙지시 [%s -> %s]"), DEVICE, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RackToRackRoutine"), pJobItem);
		}
		else
			m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;										// JBY 0829

		if (pJobItem2 != NULL)
		{
			m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
			m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
			m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;				// JBY 0830

			pJobItem2->SetJobStatus(enJobStatusScInvoke);

			strLog.Format(_T("%s 랙투랙지시 [%s -> %s]"), DEVICE, pJobItem2->m_strStartLoc, pJobItem2->m_strDestLoc);
			WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RackToRackRoutine"), pJobItem2);
		}
		else
			m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;										// JBY 0829

		m_pInfo->m_ucJobStatus = CScInfo::enMoving;
		m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRackToRack;
		m_pInfo->m_tOperStart = CTime::GetCurrentTime();
		m_pInfo->m_bModified = TRUE;
		m_pInfo->Backup();

		m_pDoc->m_pJob->Backup();
		Sleep(500L);

		return TRUE;
	}
	else
	{
		if (pJobItem != NULL && pJobItem2 != NULL)
		{
			strLog.Format(_T("%s 랙투랙지시 실패! [%s -> %s] [%s]"), DEVICE, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RackToRackRoutine"), pJobItem);

			strLog.Format(_T("%s 랙투랙지시 실패! [%s -> %s] [%s]"), DEVICE, pJobItem2->m_strStartLoc, pJobItem2->m_strDestLoc, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RackToRackRoutine"), pJobItem2);

			// 실패가 가짜일수도 있다.
			Sleep(3000);
			ReadStatus();

			strLog.Format(_T("%s 랙투랙지시 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RackToRackRoutine"), pJobItem);

			strLog.Format(_T("%s 랙투랙지시 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RackToRackRoutine"), pJobItem2);

			// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
			// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
			// pJobItem->m_nLuggNum : 작업정보의 작업번호 
			if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum && m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum)
			{
				if (pJobItem != NULL)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
					m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;					// JBY 0830

					pJobItem->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s 랙투랙지시 [%s -> %s]"), DEVICE, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc);
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RackToRackRoutine"), pJobItem);
				}
				else
					m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;										// JBY 0829

				if (pJobItem2 != NULL)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
					m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;				// JBY 0830

					pJobItem2->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s 랙투랙지시 [%s -> %s]"), DEVICE, pJobItem2->m_strStartLoc, pJobItem2->m_strDestLoc);
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RackToRackRoutine"), pJobItem2);
				}
				else
					m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;										// JBY 0829

				m_pInfo->m_ucJobStatus = CScInfo::enMoving;
				m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRackToRack;
				m_pInfo->m_tOperStart = CTime::GetCurrentTime();
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				m_pDoc->m_pJob->Backup();
				Sleep(500L);

				return TRUE;
			}
		}
		else if (pJobItem != NULL)
		{
			strLog.Format(_T("%s 랙투랙지시 실패! [%s -> %s] [%s]"), DEVICE, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RackToRackRoutine"), pJobItem);

			// 실패가 가짜일수도 있다.
			Sleep(3000);
			ReadStatus();

			strLog.Format(_T("%s 랙투랙지시 전송 실패 후 작업번호 Fork#1 [%04d]"), DEVICE, m_pInfo->m_ScInfo[0].m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RackToRackRoutine"), pJobItem);

			// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
			// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
			// pJobItem->m_nLuggNum : 작업정보의 작업번호 
			if (m_pInfo->m_ScInfo[0].m_nLuggNum == pJobItem->m_nLuggNum)
			{
				if (pJobItem != NULL)
				{
					m_pInfo->m_ScInfo[0].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[0].m_nInternalLuggNum = pJobItem->m_nLuggNum;
					m_pInfo->m_ScInfo[0].m_nInternalJobType = pJobItem->m_nJobType;					// JBY 0830

					pJobItem->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s 랙투랙지시 [%s -> %s]"), DEVICE, pJobItem->m_strStartLoc, pJobItem->m_strDestLoc);
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RackToRackRoutine"), pJobItem);
				}
				else
					m_pInfo->m_ScInfo[0].m_nInternalJobType = 0;										// JBY 0829

				m_pInfo->m_ucJobStatus = CScInfo::enMoving;
				m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRackToRack;
				m_pInfo->m_tOperStart = CTime::GetCurrentTime();
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				m_pDoc->m_pJob->Backup();
				Sleep(500L);

				return TRUE;
			}
		}

		else if (pJobItem2 != NULL)
		{
			strLog.Format(_T("%s 랙투랙지시 실패! [%s -> %s] [%s]"), DEVICE, pJobItem2->m_strStartLoc, pJobItem2->m_strDestLoc, m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RackToRackRoutine"), pJobItem2);
			// 실패가 가짜일수도 있다.
			Sleep(3000);
			ReadStatus();

			strLog.Format(_T("%s 랙투랙지시 전송 실패 후 작업번호 Fork#2 [%04d]"), DEVICE, m_pInfo->m_ScInfo[1].m_nLuggNum);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CScPair::RackToRackRoutine"), pJobItem2);

			// 실패가 가짜일수도 있다. - 만약 실패가 가짜라면....
			// m_pInfo->m_nLuggNum : S/C의 PLC에서 받아온 작업번호 (이값이 존재할경우 S/C는 구동함)
			// pJobItem->m_nLuggNum : 작업정보의 작업번호 
			if (m_pInfo->m_ScInfo[1].m_nLuggNum == pJobItem2->m_nLuggNum)
			{
				if (pJobItem2 != NULL)
				{
					m_pInfo->m_ScInfo[1].m_bInvoke = TRUE;
					m_pInfo->m_ScInfo[1].m_nInternalLuggNum = pJobItem2->m_nLuggNum;
					m_pInfo->m_ScInfo[1].m_nInternalJobType = pJobItem2->m_nJobType;				// JBY 0830

					pJobItem2->SetJobStatus(enJobStatusScInvoke);

					strLog.Format(_T("%s 랙투랙지시 [%s -> %s]"), DEVICE, pJobItem2->m_strStartLoc, pJobItem2->m_strDestLoc);
					WriteLog(LOG_TYPE_JOB, strLog, _T("CScPair::RackToRackRoutine"), pJobItem2);
				}
				else
					m_pInfo->m_ScInfo[1].m_nInternalJobType = 0;										// JBY 0829

				m_pInfo->m_ucJobStatus = CScInfo::enMoving;
				m_pInfo->m_nPrevRoutine = CScInfo::enRoutineRackToRack;
				m_pInfo->m_tOperStart = CTime::GetCurrentTime();
				m_pInfo->m_bModified = TRUE;
				m_pInfo->Backup();

				m_pDoc->m_pJob->Backup();
				Sleep(500L);

				return TRUE;
			}
		}
	}

	return FALSE;
}

/*
void CScPair::SwapJobItem(CJobItem* pJobItem, CJobItem* pJobItem1, CJobItem* pJobItem2)
{
	DEBUGER_ASSERT_VALID(pJobItem != NULL);
//	DEBUGER_ASSERT_VALID(pJobItem1 != NULL);
	DEBUGER_ASSERT_VALID(pJobItem2 != NULL);

	CString strLocation1, strLocation2;
	switch(pJobItem->GetJobPattern()) {
	case enJobPatternSto:
		strLocation1 = pJobItem->m_strDestLoc;
		strLocation2 = pJobItem2->m_strDestLoc;
		break;
	case enJobPatternRet:
		strLocation1 = pJobItem->m_strStartLoc;
		strLocation2 = pJobItem2->m_strStartLoc;
		break;
	case enJobPatternR2R:
	case enJobPatternW2W:
		// 조금더 고민해보라^^;;;
		strLocation1 = pJobItem->m_strStartLoc;
		strLocation2 = pJobItem2->m_strStartLoc;
		break;
	case enJobPatternMove:
		break;
	}

	// Twin일때 (L->R)
	if (m_pInfo->m_nForkType == CScInfo::enTwin)
	{
		// Bay를 가져올수있는 방법이 있어야 하리라.
		int nBay1 = CLib::GetBay(strLocation1);
		int nBay2 = CLib::GetBay(strLocation2);


		// L->R
		if (m_pInfo->m_nDirectionType == CScInfo::enL2R)
		{
			if (nBay1 < nBay2)
			{
				pJobItem1 = pJobItem;
			}
			else
			{
				pJobItem1 = pJobItem2;
				pJobItem2 = pJobItem;
			}
		}
		// R->L
		else if (m_pInfo->m_nDirectionType == CScInfo::enR2L)
		{
			if (nBay1 < nBay2)
			{
				pJobItem1 = pJobItem2;
				pJobItem2 = pJobItem;

			}
			else
			{
				pJobItem1 = pJobItem;
			}
		}
	}
	// Double 일때 
	else if(m_pInfo->m_nForkType == CScInfo::enDouble)
	{
		// 랙의 순번을 확인해야할 것이다.	
	}

}
*/
// 1번포크만 설정하였음!!
void CScPair::SafeCheck()
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

//		if (pTrackHS->m_pTrack->IsSensorScInterlock() == TRUE)
		if (m_pDoc->m_bScInterLock == TRUE)
		{
			if (Stop())
			{
				strLog.Format(_T("%s EMERGENCY [%s SC INTERLOCK ON]"), DEVICE, pTrackHS->ToString());
				WriteLog(LOG_TYPE_EVENT, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScPair::SafeCheck"), NULL, TRUE);
				return;
			}
			else
			{
				strLog.Format(_T("%s EMERGENCY 실패! [%s SC INTERLOCK ON]"), DEVICE, pTrackHS->ToString());
				WriteLog(LOG_TYPE_ALARM, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScPair::SafeCheck"), NULL, TRUE);
				continue;
			}
		}
	}

	for (int j=0; j<m_pInfo->m_pRetsHS.GetSize(); ++j)
	{
		pTrackHS = m_pInfo->m_pRetsHS[j];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

//		if (pTrackHS->m_pTrack->IsSensorScInterlock() == TRUE)
		if (m_pDoc->m_bScInterLock == TRUE)
		{
			if (Stop())
			{
				strLog.Format(_T("%s EMERGENCY [%s SC INTERLOCK ON]"), DEVICE, pTrackHS->ToString());
				WriteLog(LOG_TYPE_EVENT, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScPair::SafeCheck"), NULL, TRUE);
				return;
			}
			else
			{
				strLog.Format(_T("%s EMERGENCY 실패! [%s SC INTERLOCK ON]"), DEVICE, pTrackHS->ToString());
				WriteLog(LOG_TYPE_ALARM, m_pInfo->m_ScInfo[0].m_nInternalLuggNum, strLog, _T("CScPair::SafeCheck"), NULL, TRUE);
				continue;
			}
		}
	}
}

