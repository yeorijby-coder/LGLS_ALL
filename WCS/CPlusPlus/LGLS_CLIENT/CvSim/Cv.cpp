// Cv.cpp: implementation of the CCv class.
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

	CheckRequest(nConnNum);

	::Sleep(100);
}

//=============================================================================
//	Desc	: 물류흐름제어 모드 업데이트
//			  물류흐름이 원활하지 않은 경우 상온 Aging, 충방전에서 출고된 
//			  Tray를 물류 Loop line으로 투입하지 않음.
//	Date	:
//	Update	: 
//=============================================================================

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CCv::CheckRequest(int nConnNum)
{
	if (!IsConnect(nConnNum))
	{
		m_strLog.Format(_T("%s과(와) 통신연결이 끊어졌습니다!"), DEVICE);
		return FALSE;
	}
		
	CByteArray arrBuffer;
	//CWordArray arrBuffer;

	
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
			WORD wTemp = m_pDoc->m_arrRegData[m_nPlcNum - 1][nStartaddress + i];
			BYTE bTemp1 = CLib::GetByteH(wTemp);
			BYTE bTemp2 = CLib::GetByteL(wTemp);

			arrBuffer[(i*2)+1]	= bTemp1;
			arrBuffer[i*2]		= bTemp2;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCv::InvokeStation(CTrackInfo* pTrack, CStationInfo* pStation)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);
	if (pTrack == NULL || pStation == NULL)	return;
	int nDevNum = (pTrack->m_nNumber-m_nStTrNum+1)*10;

	int nAAA, nBBB, nCCC;

	switch (pStation->m_enKind)
	{
	case CStationInfo::enStoStation:

		nAAA = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
		nBBB = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7];
		nCCC = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12;

		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0 &&
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12) != enBit12)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit12;
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
		}
		else if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0 &&
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12) == enBit12)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit12;
			nBBB = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7];
		}
		break;

	case CStationInfo::enArvStation:
	case CStationInfo::enRetStation:
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0) ||
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) == enBit13)
			return;

		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;

		break;
	}
}

void CCv::MoveNextTrackForKindNormal(CTrackInfo* pTrack)
{
	int nDevNum = (pTrack->m_nNumber-m_nStTrNum+1)*10;
	int nNextDevNum = (pTrack->m_nNextCv-m_nStTrNum+1)*10;

	BOOL bAaa,bBbb;

	if (!(m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum] == 0 && m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0))
		return;

	if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
		return;
	
	if (!(((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == TRUE) && ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) == TRUE)))
		return;

	if (((m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == TRUE) && ((m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE))
	{
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum]   = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+1] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1];
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+2] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2];
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+3] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3];
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+4] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4];

		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 0;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 0;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] = 0;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3] = 0;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] = 0;

		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] ^ enBit11;
	}
	else if ((((bAaa = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == FALSE) && ((bBbb = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE)) &&
		pTrack->m_bStartFlag != TRUE)
	{
		pTrack->m_bStartFlag = TRUE;
		pTrack->m_tTime = COleDateTime::GetCurrentTime();
	}
	else if ((((bAaa = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == FALSE) && ((bBbb = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE)) &&
		pTrack->m_bStartFlag == TRUE)
	{
		COleDateTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;
		if (tElapseTime.GetTotalSeconds() > 3)
		{
			pTrack->m_bStartFlag = FALSE;
			m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] | enBit11;
		}
	}
}

void CCv::MoveNextTrackForKindDiverter(CTrackInfo* pTrack)
{
	int nDevNum = (pTrack->m_nNumber-m_nStTrNum+1)*10;
	int nNextDevNum = (pTrack->m_nNextCv-m_nStTrNum+1)*10;

	BOOL bAaa,bBbb;

	int nLen = pTrack->m_nStationArray.GetSize();
	
	for (int i=0; i<nLen; i++)
	{
		int nStation = pTrack->m_nStationArray[i];
		int nDest = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1];
		int nNextDevNum = (pTrack->m_nNextCv-m_nStTrNum+1)*10;

		if (pTrack->m_nStationArray[i] == m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1])
		{
			int nNextTrNum = pTrack->m_nNextTrArray[i];
			int nNextDevNum = (nNextTrNum-m_nStTrNum+1)*10;
			if (!(m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum] == 0 && m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0))
				return;
		
			if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
				return;

			if (!(((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == TRUE) && ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) == TRUE)))
				return;

			if (((m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == TRUE) && ((m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE))
			{
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum]   = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+1] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1];
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+2] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2];
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+3] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3];
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+4] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4];

				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] = 0;

				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] ^ enBit11;
			}
			else if ((((bAaa = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == FALSE) && ((bBbb = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE)) &&
				pTrack->m_bStartFlag != TRUE)
			{
				pTrack->m_bStartFlag = TRUE;
				pTrack->m_tTime = COleDateTime::GetCurrentTime();
			}
			else if ((((bAaa = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == FALSE) && ((bBbb = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE)) &&
				pTrack->m_bStartFlag == TRUE)
			{
				COleDateTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;
				if (tElapseTime.GetTotalSeconds() > 3)
				{
					pTrack->m_bStartFlag = FALSE;
					m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] | enBit11;
				}
			}
		}
	}
}

void CCv::InvokeRetStation(CTrackInfo* pTrack, CStationInfo* pStation)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);
	if (pTrack == NULL || pStation == NULL)	return;
	int nDevNum = (pTrack->m_nNumber-m_nStTrNum+1)*10;

	switch (pStation->m_enKind)
	{
	case CStationInfo::enRetStation:
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0) ||
			((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) == TRUE))
			return;

		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit12;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
		break;

	case CStationInfo::enArvStation:
		/*
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0) ||
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13 == TRUE))
			return;

		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;
		*/
		break;
	}
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

//================================================================================================================================================
//	// Test 모드일때 입고작업을 출고 HS에 기록한다.
//================================================================================================================================================
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
}