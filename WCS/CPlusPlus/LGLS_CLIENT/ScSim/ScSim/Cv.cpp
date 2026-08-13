// Cv.cpp: implementation of the CCv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Cv.h"
#include "ByteArrayEx.h"

#include "MainFrm.h"
#include "EcsView.h"

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
	m_bStoreRequesting = FALSE;
	m_bRetrieveRequesting = FALSE;
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	m_nPlcNum = nNumber;
	
}

CCv::~CCv()
{
	delete m_pInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCv::AutoRunProc(int nConnNum)
{
//	UpdateCommStatus(NOTIFY_SEND);

	if(m_pSocket[nConnNum] == NULL)
		return;
		
	if (ReadStatus(nConnNum))
	{
		CTrackInfo* pTrack = NULL;
		CTrackHS* pTrackHS = NULL;
		CStationInfo* pStation = NULL;

		for (int i = 0; i < m_pInfo->m_pTracks.GetSize(); ++i)
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
		}

		::Sleep(50);

		UpdateCommStatus(NOTIFY_REFRESH);
		CString strLog;
		BOOL bTemp = FALSE;

		for(int j = 0; j<25; j++)
		{
			if(m_pDoc->m_nCurrUnLoadingPlcNum[j] != m_pInfo->m_pEquipment->m_nNumber && 
				m_pDoc->m_nCurrLoadingPlcNum[j] != m_pInfo->m_pEquipment->m_nNumber)
				continue;

			if (m_pDoc->m_bLoadingTrackDelete2[j] ==  TRUE || m_pDoc->m_bLoadingTrackDelete[j] == TRUE)
			{
				if (m_pDoc->m_bLoadingTrackDelete2[j] == TRUE && 
					m_pDoc->m_nCurrLoadingPlcNum[j] != 0 &&
					m_pDoc->m_nCurrLoadingPlcNum[j] == m_pInfo->m_pEquipment->m_nNumber &&		//
					m_pDoc->m_nCurrLoadingTrackNum2[j] != 0 )
				{
					int nDeleteWordCnt = 4;		// Data 지우는 Word 갯수 
					int nTrackWordSize = 10;
					int nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;

					int nDeleteWordAddr = nTrackWordSize + (m_pDoc->m_nCurrLoadingTrackNum2[j] - nStartTrackNum) * nTrackWordSize;
					int nProdWordAddr = nDeleteWordAddr + 8;		// 화물감지 지우는 Addr

					if (m_pDoc->m_nCurrLoadingPlcNum[j] == 1)
					{
						nTrackWordSize = 2;
						nProdWordAddr = m_pDoc->m_nCurrLoadingTrackNum2[j] + 100;
						nDeleteWordCnt = 2;
						nDeleteWordAddr = nTrackWordSize + (m_pDoc->m_nCurrLoadingTrackNum2[j] - nStartTrackNum) * nTrackWordSize;
					}			

					// SC.xml 입고 H/S 주소영역 적용 (DEL_ADDR/DEL_CNT/PROD_ADDR)
					ApplyHsAddrOverride(m_pDoc->m_nCurrLoadingTrackNum2[j], 1, nDeleteWordAddr, nDeleteWordCnt, nProdWordAddr);
					
					if (WriteWordValue(nProdWordAddr, 0) == TRUE)
					{
						strLog.Format(_T("TR #%d H/S Fork #1 화물감지 삭제!"), m_pDoc->m_nCurrLoadingTrackNum2[j]);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::AutoRunProc"));
					
						if (DeleteWordAtCnt(nDeleteWordAddr, nDeleteWordCnt))
						{
							m_pDoc->m_bLoadingTrackDelete2[j] = FALSE;
							m_pDoc->m_nCurrLoadingTrackNum2[j] = 0;
							bTemp = TRUE;

							strLog.Format(_T("%s H/S Fork #2 DATA 삭제 성공!"), pTrack->GetTrackString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::AutoRunProc"));
						}
						else
						{
							//nTrackWordSize = 10;
							//nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;

							//nStartWordAddr = 10 + (m_pDoc->m_nCurrLoadingTrackNum2[j] - nStartTrackNum) * nTrackWordSize;
							//nStartWordAddr += 8;

							if (WriteWordValue(nProdWordAddr, 1) == TRUE)
							{
								strLog.Format(_T("TR #%d H/S Fork #1 화물감지 기록!"), m_pDoc->m_nCurrLoadingTrackNum2[j]);
								WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::AutoRunProc"));
							}

							strLog.Format(_T("%s H/S Fork #2 DATA 삭제 실패! - %s"), 
								pTrack->GetTrackString(), m_pSocket[nConnNum] ? m_pSocket[nConnNum]->m_strLog : m_strLog);
							WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::AutoRunProc"));
						}
					}
				}

				if (m_pDoc->m_bLoadingTrackDelete[j] == TRUE && 
					m_pDoc->m_nCurrLoadingPlcNum[j] != 0 &&
					m_pDoc->m_nCurrLoadingPlcNum[j] == m_pInfo->m_pEquipment->m_nNumber &&		//
					m_pDoc->m_nCurrLoadingTrackNum[j] != 0 )	
				{
					int nDeleteWordCnt = 4;		// Data 지우는 Word 갯수 
					int nTrackWordSize = 10;
					int nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;

					int nDeleteWordAddr = nTrackWordSize + (m_pDoc->m_nCurrLoadingTrackNum[j] - nStartTrackNum) * nTrackWordSize;
					int nProdWordAddr = nDeleteWordAddr + 8;		// 화물감지 지우는 Addr

					if (m_pDoc->m_nCurrLoadingPlcNum[j] == 1)
					{
						nTrackWordSize = 2;
						nProdWordAddr = m_pDoc->m_nCurrLoadingTrackNum[j] + 100;
						nDeleteWordCnt = 2;
						nDeleteWordAddr = nTrackWordSize + (m_pDoc->m_nCurrLoadingTrackNum[j] - nStartTrackNum) * nTrackWordSize;
					}

					// SC.xml 입고 H/S 주소영역 적용 (DEL_ADDR/DEL_CNT/PROD_ADDR)
					ApplyHsAddrOverride(m_pDoc->m_nCurrLoadingTrackNum[j], 1, nDeleteWordAddr, nDeleteWordCnt, nProdWordAddr);

					if (WriteWordValue(nProdWordAddr, 0) == TRUE)
					{				
						//if (WriteTrackInfo(m_pDoc->m_nCurrLoadingTrackNum[j], 0, 0, 0, 0) == TRUE)
						if (DeleteWordAtCnt(nDeleteWordAddr, nDeleteWordCnt))
						{
							m_pDoc->m_bLoadingTrackDelete[j] = FALSE;
							m_pDoc->m_nCurrLoadingTrackNum[j] = 0;
							bTemp = TRUE;

							strLog.Format(_T("%s H/S Fork #1 DATA 삭제 성공!"), pTrack->GetTrackString());
							WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::AutoRunProc"));
						}
						else
						{
							//nTrackWordSize = 10;
							//nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;

							//nStartWordAddr = 10 + (m_pDoc->m_nCurrLoadingTrackNum[j] - nStartTrackNum) * nTrackWordSize;
							//nStartWordAddr += 8;

							if (WriteWordValue(nProdWordAddr, 1) == TRUE)
							{
								strLog.Format(_T("TR #%d H/S Fork #1 화물감지 기록!"), m_pDoc->m_nCurrLoadingTrackNum[j]);
								WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::AutoRunProc"));
							}

							strLog.Format(_T("%s H/S Fork #1 DATA 삭제 실패! - %s"), 
								pTrack->GetTrackString(), m_pSocket[nConnNum] ? m_pSocket[nConnNum]->m_strLog : m_strLog);
							WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::AutoRunProc"));
						}

					}
				}
			}
			if (bTemp == TRUE)
			{
				m_pDoc->m_nCurrLoadingPlcNum[j] = 0;
				m_pDoc->m_nCurrLoadingTrackNum[j] = 0;
				m_pDoc->m_nCurrLoadingTrackNum2[j] = 0;
				bTemp = FALSE;
			}

			// 출고 완료시 화물 감지를 적어주는 부분 
			if (m_pDoc->m_bUnLoadComplete[j] == TRUE)
			{

				TRACE("1포크 삭제가능 여부 : %d, 트랙번호 : %d, PLC번호 : %d\n", m_pDoc->m_bUnLoadingTrackDelete[j], m_pDoc->m_nCurrUnLoadingTrackNum[j], m_pDoc->m_nCurrUnLoadingPlcNum[j]);
				TRACE("2포크 삭제가능 여부 : %d, 트랙번호 : %d, PLC번호 : %d\n", m_pDoc->m_bUnLoadingTrackDelete2[j], m_pDoc->m_nCurrUnLoadingTrackNum2[j], m_pDoc->m_nCurrUnLoadingPlcNum[j]);
				int nStartTrackNum = 0;
				if (m_pDoc->m_bUnLoadingTrackDelete[j] == TRUE && 
					m_pDoc->m_nCurrUnLoadingTrackNum[j] != 0  && 
					m_pDoc->m_nCurrUnLoadingPlcNum[j] != 0 &&
					m_pDoc->m_nCurrUnLoadingPlcNum[j] == m_pInfo->m_pEquipment->m_nNumber)
				{
					//int nDeleteWordCnt = 4;		// Data 지우는 Word 갯수 
					int nTrackWordSize = 10;
					nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;

					int nDeleteWordAddr = nTrackWordSize + (m_pDoc->m_nCurrUnLoadingTrackNum[j] - nStartTrackNum) * nTrackWordSize;
					int nProdWordAddr = nDeleteWordAddr + 8;		// 화물감지 지우는 Addr

					if (m_pDoc->m_nCurrUnLoadingPlcNum[j] == 1)
					{
						nTrackWordSize = 2;
						nProdWordAddr = m_pDoc->m_nCurrUnLoadingTrackNum[j] + 100;
						//nDeleteWordCnt = 2;
						//nDeleteWordAddr = nTrackWordSize + (m_pDoc->m_nCurrUnLoadingTrackNum[j] - nStartTrackNum) * nTrackWordSize;
					}

					// SC.xml 출고 H/S 화물감지 주소 적용 (PROD_ADDR)
					int nDummyCnt = 0;
					ApplyHsAddrOverride(m_pDoc->m_nCurrUnLoadingTrackNum[j], 2, nDeleteWordAddr, nDummyCnt, nProdWordAddr);

					if (WriteWordValue(nProdWordAddr, 1) == TRUE)
					{
						m_pDoc->m_nCurrUnLoadingTrackNum[j] = 0;
						m_pDoc->m_bUnLoadingTrackDelete[j] = FALSE;
						strLog.Format(_T("TR #%d H/S Fork #1 화물감지 기록!"), m_pDoc->m_nCurrUnLoadingTrackNum[j]);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::AutoRunProc"));
					}
				}

				if (m_pDoc->m_bUnLoadingTrackDelete2[j] == TRUE && 
					m_pDoc->m_nCurrUnLoadingTrackNum2[j] != 0 && 
					m_pDoc->m_nCurrUnLoadingPlcNum[j] != 0 &&
					m_pDoc->m_nCurrUnLoadingPlcNum[j] == m_pInfo->m_pEquipment->m_nNumber)
				{
					//int nDeleteWordCnt = 4;		// Data 지우는 Word 갯수 
					int nTrackWordSize = 10;
					nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;

					int nDeleteWordAddr = nTrackWordSize + (m_pDoc->m_nCurrUnLoadingTrackNum2[j] - nStartTrackNum) * nTrackWordSize;
					int nProdWordAddr = nDeleteWordAddr + 8;		// 화물감지 지우는 Addr

					if (m_pDoc->m_nCurrUnLoadingPlcNum[j] == 1)
					{
						nTrackWordSize = 2;
						nProdWordAddr = m_pDoc->m_nCurrUnLoadingTrackNum2[j] + 100;
						//nDeleteWordCnt = 2;
						//nDeleteWordAddr = nTrackWordSize + (m_pDoc->m_nCurrUnLoadingTrackNum2[j] - nStartTrackNum) * nTrackWordSize;
					}

					// SC.xml 출고 H/S 화물감지 주소 적용 (PROD_ADDR)
					int nDummyCnt2 = 0;
					ApplyHsAddrOverride(m_pDoc->m_nCurrUnLoadingTrackNum2[j], 2, nDeleteWordAddr, nDummyCnt2, nProdWordAddr);

					if (WriteWordValue(nProdWordAddr, 1) == TRUE)
					{
						m_pDoc->m_nCurrUnLoadingTrackNum2[j] = 0;
						m_pDoc->m_bUnLoadingTrackDelete2[j] = FALSE;
						strLog.Format(_T("TR #%d H/S Fork #2 화물감지 기록!"), m_pDoc->m_nCurrUnLoadingTrackNum2[j]);
						WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::AutoRunProc"));
					}
				}

				if (nStartTrackNum != 0)
				{
					m_pDoc->m_nCurrUnLoadingPlcNum[j] = 0;

					m_pDoc->m_bUnLoadComplete[j] = FALSE;
				}
			}
		}
	}
	else
	{
		UpdateCommStatus(NOTIFY_ERROR);
	}

	::Sleep(50);
}

//=============================================================================
//	Desc	: 물류흐름제어 모드 업데이트
//			  물류흐름이 원활하지 않은 경우 상온 Aging, 충방전에서 출고된 
//			  Tray를 물류 Loop line으로 투입하지 않음.
//	Date	: 
//	Update	: 
//=============================================================================
void CCv::CommandProc()
{
	CString strLog;
	CCmdMsg* pCmdMsg = NULL;

	for (POSITION pos=m_cmdList.GetHeadPosition(); pos!=NULL; )
	{
		pCmdMsg = (CCmdMsg*)m_cmdList.GetNext(pos);
		CStringArray& strValues = pCmdMsg->m_strValues;
		DEBUGER_ASSERT_VALID(pCmdMsg != NULL);

//		int nTemp1 = strValues.GetSize();
//		int	nTemp2 = _ttoi(strValues[0]);

		switch (pCmdMsg->m_nCommand)
		{
		//case CCmdMsg::enCmdTrackSuspend:
		//{
		//	DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeTrackSuspend);
		//	CTrackInfo* pTrack = m_pDoc->GetTrackInfo(_ttoi(strValues[0]), m_nPlcNum);
		//	int bSuspend = _ttoi(strValues[1]);
		//	if (WriteSuspend(_ttoi(strValues[0]), bSuspend))
		//	{
		//		strLog.Format(_T("%s %s"), pTrack->GetTrackString(), (bSuspend == TRUE) ? "작업정지" : "작업정지해제");
		//		WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
		//	}
		//	else
		//	{
		//		strLog.Format(_T("%s %s 실패!"), pTrack->GetTrackString(), (bSuspend == TRUE) ? "작업정지" : "작업정지해제");
		//		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
		//	}
		//	break;
		//}
		//case CCmdMsg::enCmdTrackDeadLock:
		//{
		//	DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeTrackDeadLock);
		//	CTrackInfo* pTrack = m_pDoc->GetTrackInfo(_ttoi(strValues[0]), m_nPlcNum);
		//	int bDeadLock = _ttoi(strValues[1]);
		//	if (WriteDeadLock(_ttoi(strValues[0]), bDeadLock))
		//	{
		//		strLog.Format(_T("%s %s"), pTrack->GetTrackString(), (bDeadLock == TRUE) ? "DeadLock" : "DeadLock해제");
		//		WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
		//	}
		//	else
		//	{
		//		strLog.Format(_T("%s %s 실패!"), pTrack->GetTrackString(), (bDeadLock == TRUE) ? "DeadLock" : "DeadLock해제");
		//		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
		//	}
		//	break;
		//}
		//case CCmdMsg::enCmdTrackDeadLockCheck:
		//{
		//	DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeTrackDeadLockCheck);
		//	CTrackInfo* pTrack = m_pDoc->GetTrackInfo(_ttoi(strValues[0]), m_nPlcNum);
		//	int bDeadLock = _ttoi(strValues[1]);
		//	if (WriteDeadLockCheck(_ttoi(strValues[0]), bDeadLock))
		//	{
		//		strLog.Format(_T("%s %s"), pTrack->GetTrackString(), (bDeadLock == TRUE) ? "DeadLock" : "DeadLock해제");
		//		WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
		//	}
		//	else
		//	{
		//		strLog.Format(_T("%s %s 실패!"), pTrack->GetTrackString(), (bDeadLock == TRUE) ? "DeadLock" : "DeadLock해제");
		//		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
		//	}
		//	break;
		//}
		case CCmdMsg::enCmdWriteWord:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeWriteWord);
			if (WriteWordValue(_ttoi(strValues[0]), _ttoi(strValues[1])))
			{
				strLog.Format(_T("%s TR%s WriteWordValue [SET=%s]"), DEVICE, strValues[0], strValues[1]);
				WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
			}
			else
			{
				strLog.Format(_T("%s TR%s WriteWordValue 실패! [SET=%s]"), DEVICE, strValues[0], strValues[1]);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
			}
			break;
		case CCmdMsg::enCmdTrackWrite:
			DEBUGER_ASSERT_VALID(strValues.GetSize() == CCmdMsg::enSizeTrackWrite);
			if (WriteTrackInfo(_ttoi(strValues[0]), _ttoi(strValues[1]), _ttoi(strValues[2]), _ttoi(strValues[3]), _ttoi(strValues[4]), _ttoi(strValues[5])))
			{
				int nTemp1 = _ttoi(strValues[0]);
				int nTemp2 = _ttoi(strValues[1]);
				int nTemp3 = _ttoi(strValues[2]);
				int nTemp4 = _ttoi(strValues[3]);
				int nTemp5 = _ttoi(strValues[4]);
				int nTemp6 = _ttoi(strValues[5]);

				// 삭제
				if (_ttoi(strValues[0]) == 0 && _ttoi(strValues[1]) == 0 && _ttoi(strValues[2]) == 0 && 
					_ttoi(strValues[3]) == 0 && _ttoi(strValues[4]) == 0 && _ttoi(strValues[5]) == 0)
				{
					strLog.Format(_T("%s TRACK DATA 수동 삭제 ( TRACK#%04d )"), DEVICE);
					WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[1]), strLog, _T("CCv::CommandProc"));
				}
				// 쓰기
				else
				{
					CJobItem* pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValues[1]));
//					CTrackInfo* pTrack = m_pDoc->GetTrackInfo(_ttoi(strValues[0]));
					CTrackInfo* pTrack = m_pDoc->GetTrackInfo(_ttoi(strValues[0]), m_nPlcNum);
					if (pJobItem != NULL && pTrack != NULL)
					{
						if ((pTrack->GetStoStation() || pTrack->GetArvStation()) && pTrack->m_nDestPos != _ttoi(strValues[3]))
						{
							pJobItem->SetJobStatus(enJobStatusCvInvoke);
							m_pDoc->m_bBackupChk = TRUE;
							strLog.Format(_T("%s TRACK DATA 수동 쓰기 도착대에서 도착위치 수정 ( TRACK#%04d : 작업번호=%d, TRAY=%s )"), 
								DEVICE, _ttoi(strValues[0]), pJobItem->m_nLuggNum, pJobItem->m_strBarcode);
							WriteLog(LOG_TYPE_EVENT, pJobItem->m_nLuggNum, strLog, _T("CCv::CommandProc"));
						}
					}

					if (pTrack != NULL)
					{
						strLog.Format(_T("%s TRACK DATA 수동 쓰기 ( TRACK#%04d : 이전작업번호=%d, 이전작업종류=%d, 이전목적지=%d, 신규작업번호=%s, 신규작업종류=%s, 신규목적지=%s )"), 
								DEVICE, _ttoi(strValues[0]), pTrack->m_nLuggNum, pTrack->m_nJobType, pTrack->m_nDestPos, 
								strValues[1], strValues[2], strValues[3]);
						WriteLog(LOG_TYPE_EVENT, _ttoi(strValues[1]), strLog, _T("CCv::CommandProc"));
					}
				}
			}
			else
			{
				CJobItem* pJobItem = m_pDoc->m_pJob->Find(_ttoi(strValues[1]));
				if (pJobItem != NULL)
				{
					pJobItem->SetJobStatus(enJobStatusError);
					m_pDoc->m_bBackupChk = TRUE;
				}
		
				strLog.Format(_T("%s TRACK DATA 수동 쓰기 실패! ( TRACK#%04d: 작업번호=%s, 작업종류=%s, 목적지=%s ) [%s]"), 
					DEVICE, _ttoi(strValues[0]), strValues[1], strValues[2], strValues[3], m_pSocket[0] ? m_pSocket[0]->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ALARM, _ttoi(strValues[1]), strLog, _T("CCv::CommandProc"));
			}
//-------------------------------------------------------------------------------------------------------
			break;
		//case CCvCmdMsg::enCmdDoorOpen:
		//	DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
		//	if (WriteDoorOpen(_ttoi(strValues[0])))
		//	{
		//		strLog.Format(_T("%s DOOR OPEN [DOOR%d]"), DEVICE, _ttoi(strValues[0])+1);
		//		WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
		//	}
		//	else
		//	{
		//		strLog.Format(_T("%s DOOR OPEN 실패! [DOOR%d] [%s]"), DEVICE, _ttoi(strValues[0])+1, m_pSocket[0] ? m_pSocket[0]->m_strLog : m_strLog);
		//		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
		//	}
		//	break;
		//case CCvCmdMsg::enCmdTurnPermissionRequest:
		//	DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
		//	if (WriteInPermissionRequest(_ttoi(strValues[0])))
		//	{
		//		strLog.Format(_T("%s 턴테이블 진입요청 [DOOR%d]"), DEVICE, _ttoi(strValues[0])+1);
		//		WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
		//	}
		//	else
		//	{
		//		strLog.Format(_T("%s 턴테이블 진입요청 [DOOR%d] [%s]"), DEVICE, _ttoi(strValues[0])+1, m_pSocket[0] ? m_pSocket[0]->m_strLog : m_strLog);
		//		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
		//	}
		//	break;
		//case CCvCmdMsg::enCmdDoorClose:
		//	DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
		//	if (WriteDoorClose(_ttoi(strValues[0])))
		//	{
		//		strLog.Format(_T("%s DOOR CLOSE [DOOR%d]"), DEVICE, _ttoi(strValues[0])+1);
		//		WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
		//	}
		//	else
		//	{
		//		strLog.Format(_T("%s DOOR CLOSE 실패! [DOOR%d] [%s]"), DEVICE, _ttoi(strValues[0])+1, m_pSocket[0] ? m_pSocket[0]->m_strLog : m_strLog);
		//		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
		//	}
		//	break;
		//case CCvCmdMsg::enCmdDoorStatus:
		//	DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
		//	if (WriteDoorStatus(_ttoi(strValues[0])))
		//	{
		//		strLog.Format(_T("%s DOOR STATUS 전송 [STATUS=%x]"), DEVICE, _ttoi(strValues[0]));
		//		WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
		//	}
		//	else
		//	{
		//		strLog.Format(_T("%s DOOR STATUS 전송 실패! [STATUS=%x] [%s]"), DEVICE, _ttoi(strValues[0]), m_pSocket[0] ? m_pSocket[0]->m_strLog : m_strLog);
		//		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
		//	}
		//	break;
		//case CCvCmdMsg::enCmdTurnPermission:
		//	DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeDoor);
		//	if (WriteInPermission(_ttoi(strValues[0])))
		//	{
		//		strLog.Format(_T("%s 턴테이블 동작 허가 전송 [STATUS=%x]"), DEVICE, _ttoi(strValues[0]));
		//		WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
		//	}
		//	else
		//	{
		//		strLog.Format(_T("%s 턴테이블 동작 허가 전송 실패! [STATUS=%x] [%s]"), DEVICE, _ttoi(strValues[0]), m_pSocket[0] ? m_pSocket[0]->m_strLog : m_strLog);
		//		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
		//	}
		//	break;
		//case CCvCmdMsg::enCmdFireAlarm:		// 화재발생 수동 On/Off
		//	DEBUGER_ASSERT_VALID(strValues.GetSize() == CCvCmdMsg::enSizeFireAlarm);
		//	if (WriteFireAlarm(_ttoi(strValues[0]), _ttoi(strValues[1])))
		//	{
		//		strLog = _T("화재발생 강제이동 수동 알람 리셋");
		//		WriteLog(LOG_TYPE_EVENT, strLog, _T("CCv::CommandProc"));
		//	}
		//	else
		//	{
		//		strLog = _T("화재발생 강제이동 수동 알람 리셋 실패!");
		//		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::CommandProc"));
		//	}
		//	break;
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

	m_cmdList.RemoveAll();
}

BOOL CCv::WriteTrackInfo(int nTrackNum, int nLuggNum, int nJobType, int nDestPos, int nDeadLockCheck, int nSize /* = 0 */)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s과(와) 통신연결이 끊어졌습니다!"), DEVICE);
		return FALSE;
	}
	int nTrackWordSize = 10, nWriteWordSize = 4;
	int nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;

//	int nRWSTemp = 0;
//	if((nStartTrackNum / 50) >= 1)
//		m_nRWSTemp = 500;

	int nStartWordAddr = 10 + (nTrackNum - nStartTrackNum) * nTrackWordSize;
//	if((nStartTrackNum / 50) >= 1)

	//nStartWordAddr += m_nRWSTemp;

	DEBUGER_ASSERT_VALID(m_pInfo->m_pTracks[0] != NULL);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	CByteArrayEx arrBuffer(nStartWordAddr, nWriteWordSize);
	arrBuffer.SetWord(nStartWordAddr+0, nLuggNum);
	arrBuffer.SetWord(nStartWordAddr+1, nDestPos);
	arrBuffer.SetByte(nStartWordAddr+2, enByteL, nJobType);
	arrBuffer.SetByte(nStartWordAddr+2, enByteH, nDeadLockCheck);
	arrBuffer.SetWord(nStartWordAddr+3, nSize);

	return m_pSocket[0]->WriteWord(arrBuffer, nStartWordAddr, nWriteWordSize);
}

BOOL CCv::WriteTrackInfo(CTrackInfo* pTrack, CJobItem* pJobItem, int nDestTrackNum /* = 0 */, int nDeadLockCheck /* = 0 */, int nSize /* = 0 */)
{
	CStationInfo* pDestStation = m_pDoc->GetStationInfo(pJobItem->m_strDestStn);

	int nTrackDest = (nDestTrackNum == 0) ? pDestStation->GetTrackDestination() : nDestTrackNum;
	int nJobSize = (nSize == 0) ? pJobItem->m_nSize : nSize;
	DEBUGER_ASSERT_VALID(pDestStation != NULL);
	DEBUGER_ASSERT_VALID(pJobItem != NULL);

	CString strLog;
	strLog.Format(_T("%s, Dest=%d 구동지시 ( %s )"), pTrack->GetTrackString(), nTrackDest, pJobItem->GetLogString());
	WriteLog(LOG_TYPE_JOB, strLog, _T("CCv::InvokeStoStation"), pJobItem);

	return WriteTrackInfo(pTrack->m_nNumber, pJobItem->m_nLuggNum, pJobItem->m_nJobType, nTrackDest, nDeadLockCheck, nJobSize);
}

BOOL CCv::WriteWordValue(int nAddress, WORD wWord, BOOL bOtherWay/* = FALSE*/)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s과(와) 통신연결이 끊어졌습니다!"), DEVICE);
		return FALSE;
	}
	
	CByteArrayEx arrBuffer(nAddress, 1);
	arrBuffer.SetWord(nAddress, wWord);

	BYTE	TxBuff[4098];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = (BYTE)wWord;									
	
	if (bOtherWay == TRUE)
		return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D,  nAddress, 1);
	else
		return m_pSocket[0]->WriteWord(arrBuffer, nAddress, 1);
}

BOOL CCv::DeleteWordAtCnt(int nAddress, int nCnt)
{
	if (!IsConnect(0))
	{
		m_strLog.Format(_T("%s과(와) 통신연결이 끊어졌습니다!"), DEVICE);
		return FALSE;
	}

//	CByteArrayEx arrBuffer(nAddress, 1);
//	arrBuffer.SetWord(nAddress, wWord);

	BYTE	TxBuff[4098];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	CByteArrayEx arrBuffer(nAddress, nCnt);
	for (int iii = 0; iii < nCnt; iii++)
	{
		arrBuffer.SetWord(nAddress + iii, 0);
	}

//	if (bOtherWay == TRUE)
//		return m_pSocket[0]->Write(CMD_WORD_UNIT, TxBuff, DEVICE_CODE_D, nAddress, 1);
//	else
		return m_pSocket[0]->WriteWord(arrBuffer, nAddress, nCnt);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================================
//	Desc	: SC.xml <HS>의 DEL_ADDR/DEL_CNT/PROD_ADDR 주소 적용
//			  - 입고HS(nType=1) : 로딩 완료 -> CV 작업대 데이터 삭제 주소
//			  - 출고HS(nType=2) : 언로딩 완료 -> CV 화물감지 기록 주소
//			  - 속성이 없으면 기존 계산식 값 유지
//			  - PAIR 트랙 일치시 DEL_ADDR2/DEL_CNT2/PROD_ADDR2 사용
//=============================================================================
void CCv::ApplyHsAddrOverride(int nTrackNum, int nType, int& nDeleteWordAddr, int& nDeleteWordCnt, int& nProdWordAddr)
{
	CTrackHS* pTrackHS = m_pDoc->FindHsByCvTrack(m_pInfo->m_pEquipment->m_nNumber, nTrackNum, nType);
	if (pTrackHS == NULL)
		return;

	if (pTrackHS->m_pTrack != NULL && pTrackHS->m_pTrack->m_nNumber == nTrackNum)
	{
		// 메인 트랙(TID)
		if (pTrackHS->m_nDelAddr >= 0)		nDeleteWordAddr = pTrackHS->m_nDelAddr;
		if (pTrackHS->m_nDelCnt > 0)		nDeleteWordCnt = pTrackHS->m_nDelCnt;
		if (pTrackHS->m_nProdAddr >= 0)		nProdWordAddr = pTrackHS->m_nProdAddr;
	}
	else if (pTrackHS->m_nPair == nTrackNum)
	{
		// PAIR 트랙(포크#2)
		if (pTrackHS->m_nDelAddr2 >= 0)		nDeleteWordAddr = pTrackHS->m_nDelAddr2;
		if (pTrackHS->m_nDelCnt2 > 0)		nDeleteWordCnt = pTrackHS->m_nDelCnt2;
		if (pTrackHS->m_nProdAddr2 >= 0)	nProdWordAddr = pTrackHS->m_nProdAddr2;
	}
}

BOOL CCv::CheckRequest(int nConnNum)
{
	if (!IsConnect(nConnNum))
	{
		m_strLog.Format(_T("%s과(와) 통신연결이 끊어졌습니다!"), DEVICE);
		return FALSE;
	}
		
	CByteArray arrBuffer;
	
	if (m_pSocket[nConnNum]->CheckRequest(arrBuffer) == FALSE)
	{
		return FALSE;
	}

	int nHi = arrBuffer[12];
	int nLow = arrBuffer[11];
	int nnLen = (arrBuffer[8] << 8 ) | arrBuffer[7];
	int nCommand = (arrBuffer[12] << 8) | arrBuffer[11];
	int nStartaddress = (arrBuffer[17] << 16) | (arrBuffer[16] << 8) | (arrBuffer[15]);
	int nDevice = arrBuffer[18];
	int nLen = (arrBuffer[20] << 8) | arrBuffer[19];
	int aa = arrBuffer[19];
	if (nStartaddress > 10000)
		return FALSE;

	arrBuffer[9] = nLen;

	m_pSocket[nConnNum]->m_bWriteLog = m_port.m_bWriteLog;

	if (nCommand == 0x0401)
	{
		arrBuffer.SetSize(nLen*2);
		for (int i=0; i<nLen; ++i)
		{
			arrBuffer[(i*2)+1] = CLib::GetByteH(m_pDoc->m_arrRegData[m_nPlcNum-1][nStartaddress+i]);
			arrBuffer[i*2]     = CLib::GetByteL(m_pDoc->m_arrRegData[m_nPlcNum-1][nStartaddress+i]);

		}
 
		if (m_pSocket[nConnNum]->ResponseReadWord(arrBuffer, nLen) == FALSE)
		{
			return FALSE;
		}
	}
	else if (nCommand == 0x1401)
	{
		for (int i=0; i<nLen; ++i)
		{
			int aaa = arrBuffer[22+i];
			int bbb = arrBuffer[21+i];
			m_pDoc->m_arrRegData[m_nPlcNum-1][nStartaddress+i] = (arrBuffer[22+(i*2)] << 8) | arrBuffer[21+(i*2)];
			int nLuggNum = m_pDoc->m_arrRegData[m_nPlcNum-1][nStartaddress+i];

			if(i ==  2 ||i ==  3)
				int aaaaa= 0;
		}

		if (m_pSocket[nConnNum]->ResponseWriteWord(arrBuffer, nLen) == FALSE)
		{
			return FALSE;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}
// 트랙의 시작을 무조건 1 or 11 or 21 or 31 or 41 or 51... 로 한다. 
BOOL CCv::ReadStatus(int nConnNum)
{
	if (!IsConnect(nConnNum))
	{
		m_strLog.Format(_T("%s과(와) 통신연결이 끊어졌습니다!"), DEVICE);
		return FALSE;
	}
	CString strLog;
	int nStartWordAddr = 10;
	
	int nStartTrackNum = m_pInfo->m_pTracks[0]->m_nNumber;
	int nReadWordSize = m_pInfo->m_pTracks.GetSize() * 10;
	

	int nTrackCnt = m_pInfo->m_pTracks.GetSize();
	
	//	int nStart10	= (nStartTrackNum % 100) / 10;
//	nStartWordAddr = (nStartTrackNum * 10) - nStartWordAddr;	// 원래 주석 
	m_nRWSTemp = 500;
/*
	CByteArrayEx arrBuffer(nStartWordAddr + m_nRWSTemp, nReadWordSize);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	
	m_pSocket1->m_bWriteLog = FALSE;
	
	arrBuffer.SetArray(nStartWordAddr + m_nRWSTemp, nReadWordSize);
	
	if (m_pSocket1->ReadWord(arrBuffer, nStartWordAddr + m_nRWSTemp, nReadWordSize) == FALSE)
	{
		strLog.Format(_T("%s 시작번지 %d 트랙정보 읽기 실패! ( %s )"), 
			DEVICE, nStartWordAddr, m_pSocket1 ? m_pSocket1->m_strLog : m_strLog);
		WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::ReadStatus"));
		return FALSE;
	}
*/
	
	m_pSocket[nConnNum]->m_bWriteLog = FALSE;

	CByteArrayEx arrBuffer(nStartWordAddr, 0);
	arrBuffer.SetArray(nStartWordAddr, nReadWordSize);

	//PLC 별로 트랙 갯수만큼 Read함
	for(int j=0; j < 10; j++)
	{
		CByteArrayEx arrBuffer2(nStartWordAddr, nReadWordSize);
		if( j == 10-1 )
		{
			int nRWSTemp = 0;
			if( nReadWordSize % m_nRWSTemp == 0)
				nRWSTemp = 500;
			else
				nRWSTemp = nReadWordSize % m_nRWSTemp;
			arrBuffer2.SetArray(nStartWordAddr + (m_nRWSTemp * j), nRWSTemp);   
			if (m_pSocket[nConnNum]->ReadWord(arrBuffer2, nStartWordAddr + (m_nRWSTemp * j), nRWSTemp) == FALSE)
			{
				strLog.Format(_T("%s 시작번지 %d 트랙정보 읽기 실패! ( %s )"), 
					DEVICE, nStartWordAddr + (m_nRWSTemp * j), m_pSocket[nConnNum] ? m_pSocket[0]->m_strLog : m_strLog);
				WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::ReadStatus"));
				return FALSE;
			}
			
			arrBuffer.Append(arrBuffer2);
			
			break;
		}
		
		arrBuffer2.SetArray(nStartWordAddr + (m_nRWSTemp * j), m_nRWSTemp);
		if (m_pSocket[nConnNum]->ReadWord(arrBuffer2, nStartWordAddr + (m_nRWSTemp * j), m_nRWSTemp) == FALSE)
		{
			strLog.Format(_T("%s 시작번지 %d 트랙정보 읽기 실패! ( %s )"), 
				DEVICE, nStartWordAddr + (m_nRWSTemp * j), m_pSocket[nConnNum] ? m_pSocket[0]->m_strLog : m_strLog);
			WriteLog(LOG_TYPE_ERROR, strLog, _T("CCv::ReadStatus"));
			return FALSE;
		}
		
		arrBuffer.Append(arrBuffer2);
		
   }
	m_pSocket[nConnNum]->m_bWriteLog = m_port.m_bWriteLog;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nWordAddr = 0;
	CTrackInfo* pTrack = NULL;
	for (int i = 0; i < m_pInfo->m_pTracks.GetSize(); ++i)
	{
		pTrack = m_pInfo->m_pTracks[i];
		DEBUGER_ASSERT_VALID(pTrack != NULL);
		
		if(pTrack->m_nNumber == 25)
			int aaa =0;
		nWordAddr = nStartWordAddr + i*10;
		pTrack->SetLuggNum(arrBuffer.GetWord(nWordAddr+0));
		pTrack->SetDestPos(arrBuffer.GetWord(nWordAddr+1));
		pTrack->SetJobType(arrBuffer.GetByte(nWordAddr+2, enByteL));
		pTrack->SetGenCode(arrBuffer.GetByte(nWordAddr+2, enByteH));
		pTrack->SetSize(arrBuffer.GetWord(nWordAddr+3));
		pTrack->SetSuspend(arrBuffer.GetNibble(nWordAddr+5, enNibble1));
		pTrack->SetDeadLock(arrBuffer.GetNibble(nWordAddr+5, enNibble4)); // 안씀
		pTrack->SetErrorCode(arrBuffer.GetWord(nWordAddr+6));
		pTrack->SetStatusData(arrBuffer.GetWord(nWordAddr+7));
		pTrack->SetSensorData(arrBuffer.GetWord(nWordAddr+8));
		pTrack->SetMotorData(arrBuffer.GetWord(nWordAddr+9));			// 안씀
		
		int a = pTrack->m_nNumber;
		pTrack->InvokeControl(pTrack->m_pTrackCtrl); 
	}
	
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCv::InvokeStoStation(CTrackInfo* pTrack, CStationInfo* pStation)
{

}

void CCv::InvokeRetStation(CTrackInfo* pTrack, CStationInfo* pStation)
{

}

void CCv::InvokeScRetHS(CTrackInfo* pTrack, CTrackHS* pTrackHS)
{
}

void CCv::InvokeRtvArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS)
{
}

void CCv::InvokeLifterArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS)
{
}

void CCv::InvokeLgvArriveHS(CTrackInfo* pTrack, CTrackHS* pTrackHS)
{
}

void CCv::InvokeArriveReport(CTrackInfo* pTrack)
{
}
