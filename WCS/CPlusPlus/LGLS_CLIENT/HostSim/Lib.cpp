
#include "stdafx.h"
#include "Ecs.h"
#include "Lib.h"
#include "EcsDoc.h"
#include "MainFrm.h"

BOOL CLib::IsSet(BYTE Byte, int nPos)
{
	DEBUGER_ASSERT_RANGE(nPos, 8);
    BYTE BitFormat = (BYTE)Power(2, nPos);
    return ((Byte & BitFormat) == BitFormat) ? TRUE : FALSE;
}

BOOL CLib::IsSet(WORD Word, int nPos)
{
	DEBUGER_ASSERT_RANGE(nPos, 16);
    WORD BitFormat = (WORD)Power(2, nPos);
    return ((Word & BitFormat) == BitFormat) ? TRUE : FALSE;
}

BOOL CLib::IsSet(DWORD dwWord, int nPos)
{
	DEBUGER_ASSERT_RANGE(nPos, 32);
    DWORD BitFormat = (DWORD)Power(2, nPos);
    return ((dwWord & BitFormat) == BitFormat) ? TRUE : FALSE;
}

BOOL CLib::IsSet(UINT Int, int nPos)
{
	DEBUGER_ASSERT_RANGE(nPos, 32);
    UINT BitFormat = (UINT)Power(2, nPos);
    return ((Int & BitFormat) == BitFormat) ? TRUE : FALSE;
}

void CLib::SetBit(WORD& wValue, int nPos, BOOL bSet)
{
	DEBUGER_ASSERT_RANGE(nPos, 16);
    WORD BitFormat = (WORD)Power(2, nPos);
	if (bSet) wValue |= BitFormat;
	else wValue &= ~BitFormat;
}

int CLib::Power(int x, int y)
{
    if (y == 0) 
		return 1;

	int z = x;
	for (int i=0; i<y-1; ++i)
		z *= x;

    return z;
}

WORD CLib::HexStrToWORD(BYTE* bySrc)
{
	WORD wDest = 0;
	for (int i=0; i<4; i++)
	{
		if (bySrc[i] >= '0' && bySrc[i] <= '9')
			wDest |= bySrc[i] - '0';
		else
			wDest |= bySrc[i] - 'A' + 10;

		if(i!=3) wDest <<= 4;
	}

	return wDest;
}

DWORD CLib::HexStrToDWORD(BYTE* bySrc)
{
	DWORD dwDest = 0;
	for (int i=0; i<8; i++)
	{
		if (bySrc[i] >= '0' && bySrc[i] <= '9')
			dwDest |= bySrc[i] - '0';
		else
			dwDest |= bySrc[i] - 'A' + 10;

		if(i!=7) dwDest <<= 4;
	}

	return dwDest;
}

BYTE CLib::HexStrToBYTE(BYTE* bySrc)
{
	BYTE byDest = 0;
	for(int i = 0; i < 2; i++)
	{
		if( bySrc[i] >= '0' && bySrc[i] <= '9')
			byDest |= bySrc[i] - '0';
		else
			byDest |= bySrc[i] - 'A' + 10;

		if(i == 0)		byDest <<= 4;
	}

	return byDest;
}

BYTE CLib::HexStrToBYTE(BYTE bySrc)
{
	if( bySrc >= '0' && bySrc <= '9' )
		return bySrc - '0';

	if( bySrc >= 'A' && bySrc <= 'F' )
		return bySrc - 'A' + 10;

	if( bySrc >= 'a' && bySrc <= 'f' )
		return bySrc - 'a' + 10;

	return	0;
}

WORD CLib::SwapToWord(BYTE *pSrc)
{
	return (WORD)(pSrc[1] << 8) | pSrc[0];
}

DWORD CLib::SwapToDWord(BYTE *pSrc)
{
	return (DWORD)(pSrc[3] << 24) | (pSrc[2] << 16) | (pSrc[1] << 8) | pSrc[0];
}

CString CLib::makeStringFromBin(BYTE *pData, UINT nLen)
{
	CString strRet;

	try
	{
		CString strBuf;
		for(UINT i=0; i<nLen; i++)
		{
			strBuf.Format(_T("%0.2X"), pData[i]);
			strRet += strBuf;
		}
	}
	catch (...)
	{
		return " ";
	}

	return strRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CString CLib::GetSystemErrMsg(int nErrNo)
{
	LPVOID lpMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	    NULL,
		nErrNo ? nErrNo : ::GetLastError(),
	    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
	    0,
		NULL 
	);

	CString str;
	str.Format(_T("%s"), (LPCTSTR)lpMsgBuf);
	LocalFree( lpMsgBuf );

	str.TrimRight();

	return str; 
}

CString CLib::GetExceptionString(CException* e)
{
	CString strLog;
	TCHAR szMessage[512] = {0};
	e->GetErrorMessage(szMessage, sizeof(szMessage));
	strLog.Format(_T("%s! [%s]"), e->GetRuntimeClass()->m_lpszClassName, szMessage);
	return strLog;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CString CLib::GetHostResultSting(int nResultCode)
{
	switch (nResultCode)
	{
	case enHostErrorNone:					return _T("정상");
	case enHostErrorNoSTX:					return _T("NO STX");
	case enHostErrorNoETX:					return _T("NO ETX");
	case enHostErrorDuplicatedLuggNum:		return _T("작업번호 중복");
	case enHostErrorNotExistJob:			return _T("존재하지 않는 작업정보");
	case enHostErrorBufferFull:				return _T("BUFFER FULL");
	case enHostErrorInvalidHeaderLength:	return _T("HEADER LENGTH 이상");
	case enHostErrorInvalidMsgLength:		return _T("MESSAGE LENGTH 이상");
	case enHostErrorInvalidLocation:		return _T("LOCATION 이상");
	case enHostErrorInvalidStation:			return _T("STATION 이상");
	case enHostErrorInvalidLugg:			return _T("잘못된 Lugg Number");
	case enHostErrorUndefinedCommandType:	return _T("정의되지 않은 COMMAND");
	case enHostErrorInhibitedLoc:			return _T("금지된 LOCATION");
	case enHostErrorInvalidMailboxName:		return _T("MAILBOX NAME 이상");
	case enHostErrorAlreadyJob:				return _T("이미 존재하는 작업정보");
	case enHostErrorInvalidContent:			return _T("CONTENT 이상");
	case enHostErrorUnknownJobType:			return _T("작업구분 이상");
	case enHostErrorImproperHandshake:		return _T("IMPROPER HANDSHAKE");
	case enHostErrorNoResponse:				return _T("상대의 응답이 없음");
	case enHostErrorSocket:					return _T("SOCKET ERROR");
	case enHostErrorInternal:				return _T("내부처리 ERROR");
	}

	CString str;
	str.Format(_T("등록되지 않은 코드 [CODE=%d]"), nResultCode);
	return str;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CLib::IsValidIDPID(CString& strIDPID)
{
	if (strIDPID.GetLength() != enLengthIDPID)
		return FALSE;

	return TRUE;
}

BOOL CLib::IsValidBIDPID(CString& strBIDPID)
{
	if (strBIDPID.GetLength() != enLengthBIDPID)
		return FALSE;

	if (strBIDPID[0] != 'B')
		return FALSE;

	return TRUE;
}

BOOL CLib::IsValidBarcode(CString& strBarcode)
{
	if (strBarcode.GetLength() != enLengthBarcode)
		return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLib::PairJobCheckR2R(CJobItem* pInPutJobItem1, CJobItem* pInPutJobItem2, int& nSelectJobItem, CString& strLocation1, CString& strLocation2, BOOL bFromLoc)
{
	BOOL bPairJob = FALSE;
//	CString strLocation1, strLocation2;
	if (bFromLoc == TRUE)
	{
		strLocation1 = pInPutJobItem1->m_strStartLoc;
		strLocation2 = pInPutJobItem2->m_strStartLoc;
	}
	else
	{
		strLocation1 = pInPutJobItem1->m_strDestLoc;
		strLocation2 = pInPutJobItem2->m_strDestLoc;
	}

	if ((strLocation1.Mid(2,5) == strLocation2.Mid(2,5)) &&
		(GetDoubleSide(strLocation1) == GetDoubleSide(strLocation2)) )
	{
		bPairJob = TRUE;
		if (GetDoubleSide(strLocation1) == 1)
			nSelectJobItem = (strLocation1.Mid(0,2) < strLocation2.Mid(0,2)) ? 1 : 2;
		else
			nSelectJobItem = (strLocation1.Mid(0,2) < strLocation2.Mid(0,2)) ? 1 : 2;
	}
	
	return bPairJob;
}

BOOL CLib::PairJobCheck(CJobItem* pInPutJobItem1, CJobItem* pInPutJobItem2, int& nSelectJobItem)
{
	BOOL bPairJob = FALSE;
	CString strLocation1, strLocation2;
	switch(pInPutJobItem1->GetJobPattern()) 
	{
	case enJobPatternSto:
	case enJobPatternR2R:
		strLocation1 = pInPutJobItem1->m_strDestLoc;
		strLocation2 = pInPutJobItem2->m_strDestLoc;
		break;
	case enJobPatternRet:
		strLocation1 = pInPutJobItem1->m_strStartLoc;
		strLocation2 = pInPutJobItem2->m_strStartLoc;
		break;
	case enJobPatternMove:
		nSelectJobItem = 1;
		return TRUE;
	}

	if ((strLocation1.Mid(2,5) == strLocation2.Mid(2,5)) &&
		(GetDoubleSide(strLocation1) == GetDoubleSide(strLocation2)) )
	{
		bPairJob = TRUE;
		if (GetDoubleSide(strLocation1) == 1)
			nSelectJobItem = (strLocation1.Mid(0,2) < strLocation2.Mid(0,2)) ? 1 : 2;
		else
			nSelectJobItem = (strLocation1.Mid(0,2) < strLocation2.Mid(0,2)) ? 1 : 2;
	}
	
	return bPairJob;
}

BOOL CLib::IsNeedBeltMove(CString& strLocation1, CString& strLocation2)
{
	if (GetDoubleSide(strLocation1) != GetDoubleSide(strLocation2))
	{
		return FALSE;
	}
	else
	{
		if( (IsOutSideForDouble(strLocation1) && IsInSideForDouble(strLocation2)) || 
			(IsOutSideForDouble(strLocation2) && IsInSideForDouble(strLocation1)) ) 
			return TRUE;
	}
	if (IsOutSideForDouble(strLocation1))
		return FALSE;

	if (IsOutSideForDouble(strLocation2))
		return FALSE;


	int nBank1	= _ttoi(strLocation1.Mid(0,2));
	int nBay1	= _ttoi(strLocation1.Mid(2,3));
	int nLevel1 = _ttoi(strLocation1.Mid(5,2));

	int nBank2	= _ttoi(strLocation2.Mid(0,2));
	int nBay2	= _ttoi(strLocation2.Mid(2,3));
	int nLevel2 = _ttoi(strLocation2.Mid(5,2));
//	static BOOL IsOutSideForDouble(CString& strLocation) { return ((((_ttoi(strLocation.Mid(0, 2)) - 1) % 4) / 2) + 1) == 2; }
//	static BOOL IsInSideForDouble(CString& strLocation) { return ((((_ttoi(strLocation.Mid(0, 2)) - 1) % 4) / 2) + 1) == 1; }

	if (nBank1 != nBank2)
		return FALSE;

	if (nBay1 == nBay2)
	{
		if (nLevel1 != nLevel2)
			return TRUE;
		
		return FALSE;
	}

	return TRUE;
}

BOOL CLib::IsNeedBeltMove(CString& strLocation, int nForkNum, int nPattern)
{
	switch(nPattern) 
	{
	case enJobPatternSto:		 // 입고일때 
		// 같은 방향이면 벨트이동을 할 필요가 없다.
		if (GetDoubleSide(strLocation) == nForkNum)
			return FALSE;
		else
			return TRUE;
		break;
	case enJobPatternRet:		// 출고일때 
		// H/S가 같은 방향이면 벨트이동을 할필요가 없다. 
		if (GetDoubleSide(strLocation) == nForkNum)
			return FALSE;
		else
			return TRUE;
		break;
	case enJobPatternR2R:		// 랙투랙일때 - 화물 1개로만 작업
		if (GetDoubleSide(strLocation) == nForkNum)
			return FALSE;
		else
			return TRUE;
		break;

	}

	// 일단 그외는 모두 필요 없는 경우로 한정!!
	return FALSE;
}

BOOL CLib::IsValidLocation(int nWarehouse, CString& strLocation)
{
	if (strLocation.GetLength() != enLengthLocation)
	{
		DEBUGER_TRACE(strLocation, _T("CLib::IsValidLocation"));
		return FALSE;
	}

	int nBank = GetBank(strLocation);
	int nBay = GetBay(strLocation);
	int nLevel = GetLevel(strLocation);

//	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();
//	CEcsDoc * pDoc = (CEcsDoc*) pFrame->GetActiveDocument();


	CEcsApp* pApp = (CEcsApp*) AfxGetApp();
	if (pApp == NULL)
		return FALSE;

	CMainFrame* pWnd = (CMainFrame*) pApp->GetMainWnd();
	if (pWnd == NULL)
		return FALSE;

	CEcsDoc * pDoc = (CEcsDoc*) pWnd->GetActiveDocument();
	if (pDoc == NULL)
		return FALSE;

	switch (nWarehouse)
	{
	case CJobItem::enWarehouse1:
		{
			if ((nBank < 1) || (nBank > pDoc->m_pConfig->m_nBank))		
			{
				DEBUGER_TRACE(strLocation, _T("CLib::IsValidLocation"));
				return FALSE;
			}

			if ((nBay < 1) || (nBay > pDoc->m_pConfig->m_nBay))
			{
				DEBUGER_TRACE(strLocation, _T("CLib::IsValidLocation"));
				return FALSE;
			}

			if ((nLevel < 1) || (nLevel > pDoc->m_pConfig->m_nLevel))
			{
				DEBUGER_TRACE(strLocation, _T("CLib::IsValidLocation"));
				return FALSE;
			}
		}
		break;

	default:
		DEBUGER_TRACE(CConvert::ToString(nWarehouse), _T("CLib::IsValidLocation"));
		return FALSE;
	}

	return TRUE;
}

BOOL CLib::IsDisableLocation(int nWarehouse, CString& strLocation)
{
	if (IsValidLocation(nWarehouse, strLocation) == FALSE)
	{
		DEBUGER_TRACE(strLocation, _T("CLib::IsDisableLocation"));
		return FALSE;
	}

	int nBank = GetBank(strLocation);
	int nBay = GetBay(strLocation);
	int nLevel = GetLevel(strLocation);

	// 수정본 소스 
//	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();
//	CEcsDoc * pDoc = (CEcsDoc*) pFrame->GetActiveDocument();
	CEcsApp* pApp = (CEcsApp*) AfxGetApp();
	if (pApp == NULL)
		return FALSE;

	CMainFrame* pWnd = (CMainFrame*) pApp->GetMainWnd();
	if (pWnd == NULL)
		return FALSE;

	CEcsDoc * pDoc = (CEcsDoc*) pWnd->GetActiveDocument();
	if (pDoc == NULL)
		return FALSE;


	CString strTemp;
	strTemp.Format(_T("%02d-%03d-%02d"), nBank, nBay, nLevel);

	for(int i = 0 ; i < pDoc->m_pConfig->m_strDisableRacks.GetSize() ; i++)
	{
		if (strTemp ==	pDoc->m_pConfig->m_strDisableRacks[i])
			return TRUE;
	}

	/*
	// 원본 소스
	switch (nWarehouse)
	{
	case CJobItem::enWarehouse1:
		{
			if ((nBay == 1) && ((nLevel == 1) || (nLevel == 2)))
				return TRUE;

			if ((nBay == 29) && ((nLevel >= 1) && (nLevel <= 7)))
				return TRUE;

			switch(nBank)
			{
			case 2:	
			case 3:	
			case 4:	
			case 5:	
				{
					if ((nBay == 2) && ((nLevel == 1) || (nLevel == 2)))
						return TRUE;
				}
				break;
			case 6:
				{
					if ( ( (nBay	== 24) || (nBay		== 25) ) && 
						 ( (nLevel	==  4) || (nLevel	==  5) ) )
						return TRUE;					
				}
				break;
			}			
		}
		break;
		
	default:
		DEBUGER_TRACE(CConvert::ToString(nWarehouse), _T("CLib::IsDisableLocation"));
		return FALSE;
	}
	*/

	return FALSE;
}
/*
CString CLib::ConvertReasonToString(int nType)
{
	switch (nType)
	{

	case	enHostErrorNone:					return "정상수신";
	case	enHostErrorNoSTX:					return "NO STX";
	case	enHostErrorNoETX:					return "NO ETX";
	case	enHostErrorDuplicatedLuggNum:		return "Luggage 번호 중복";
	case	enHostErrorNotExistJob:				return "NO Exist Job";
	case	enHostErrorBufferFull:				return "Buffer Full";
	case	enHostErrorUnknownJobType:			return "Unknown JobType";
	case	enHostErrorInternal:				return "Internal Error";
	case	enHostErrorSocket:					return "Socket Error";
	case	enHostErrorAlreadyJob:				return "Already Invoked Job";
	case	enHostErrorInvalidStartLocation:	return "Invalid Start Location";
//	case	enHostErrorInvalidDestLocation:		return "Invalid Dest Location";
	case	enHostErrorInvalidStartStation:		return "Invalid Start Stn. No";
	case	enHostErrorInvalidDestStation:		return "Invalid Dest Stn. No";
	case	enHostErrorInvalidStartWarehouse:	return "Invalid Start Warehouse";
	case	enHostErrorInvalidDestWarehouse:	return "Invalid Dest Warehouse";
	case	enHostErrorRefusedEcsJob:			return "Refused Ecs Job";
	case	enHostErrorProtocolLength:			return "Protocol Length Error";
	case	enHostErrorUndefinedCommandType:	return "Undefinded Command Type";
	case	enHostErrorJobReport:				return "Job Report Error";
	case	enHostErrorInvalidHeaderLength:		return "Invalid Header Length";
	case	enHostErrorInvalidMsgLength:		return "Invalid Msg Length";
	case	enHostErrorInvalidLuggNo:			return "Invalid Luggage No";
	case	enHostErrorInvalidMailboxName:		return "Invalid MailBox Name";
	case	enHostErrorInvalidContent:			return "Invalid Contents";
	case	enHostErrorInhibitedLoc:			return "Inhibited Location";
//	case	enHostErrorUnknownMsgType:			return "Unknown Msg Type";
	case	enHostErrorImproperHandshake:		return "Improper Handshake";
	case	enHostErrorNoResponse:				return "No Response from WMS";


//	case	0:
	default:
		CString strTemp;
		strTemp.Format("Unknown Reason(%d)", nType);
		return strTemp;
	}
}

*/
int CLib::GetJobTypeFromInv(char chType)
{
    switch(chType)
	{
	case '1':			return enJobTypeAutoSto;
	case '2':           return enJobTypeAutoRet;
	case '3':			return enJobTypeAutoPR;
//	case '3':			return enJobTypeAutoMove;
	case '4':           return enJobTypeAutoR2R;
	case '5':           return enJobTypeAutoW2W;
	case '6':			return enJobTypeAutoMove;
	}
	return 0;
}

int CLib::GetStackerNum(int nWarehouse, CString& strLocation, BOOL bDouble)
{
	if (strLocation.GetLength() != enLengthLocation)
		return 0;

	int nScNum = (GetBank(strLocation) + 1) / 2;

	if (bDouble == TRUE)
		nScNum = (GetBank(strLocation) + 3) / 4;

	switch (nWarehouse)
	{
	case CJobItem::enWarehouse1:
		break;
	}

	return nScNum;
}

int CLib::GetDoubleSide(CString& strLocation)
{
	if (strLocation.GetLength() != enLengthLocation)
		return 0;

	return (((_ttoi(strLocation.Mid(0, 2)) - 1) % 4) / 2) + 1;
}


int CLib::GetSide(CString& strLocation)
{
	if (strLocation.GetLength() != enLengthLocation)
		return 0;

	return ((_ttoi(strLocation.Mid(0, 2)) + 1) % 2) + 1;
}


int CLib::GetBank(CString& strLocation)
{
	if (strLocation.GetLength() != enLengthLocation)
		return 0;

	return _ttoi(strLocation.Left(2));
}

int CLib::GetBay(CString& strLocation)
{
	if (strLocation.GetLength() != enLengthLocation)
		return 0;

	return _ttoi(strLocation.Mid(2,3));
}

int CLib::GetLevel(CString& strLocation)
{
	if (strLocation.GetLength() != enLengthLocation)
		return 0;

	return _ttoi(strLocation.Right(2));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CLib::IsValidControlID(CString& strCID)
{
	if (strCID.GetLength() != enLengthControlID)
		return FALSE;

	return TRUE;
}

int CLib::GetWarehouse(CString& strCID)
{
	DEBUGER_ASSERT_VALID(strCID.GetLength() == enLengthControlID);
	return _ttoi(strCID.Mid(1, 1));
}

int CLib::GetFloor(CString& strCID)
{
	DEBUGER_ASSERT_VALID(strCID.GetLength() == enLengthControlID);
	return _ttoi(strCID.Mid(2, 1));
}

int CLib::GetEquipment(CString& strCID)
{
	DEBUGER_ASSERT_VALID(strCID.GetLength() == enLengthControlID);
	return _ttoi(strCID.Mid(3, 2));
}

int CLib::GetEquipNum(CString& strCID)
{
	DEBUGER_ASSERT_VALID(strCID.GetLength() == enLengthControlID);
	return _ttoi(strCID.Right(4));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLib::SetComboBoxWarehouse(CComboBox& rComboBox)
{
	rComboBox.AddString(CJobItem::GetWarehouseString(CJobItem::enWarehouse1));
	rComboBox.SetItemData(rComboBox.GetCount()-1, CJobItem::enWarehouse1);
//	rComboBox.AddString(CJobItem::GetWarehouseString(CJobItem::enWarehouse2));
//	rComboBox.SetItemData(rComboBox.GetCount()-1, CJobItem::enWarehouse2);
//	rComboBox.AddString(CJobItem::GetWarehouseString(CJobItem::enWarehouse3));
//	rComboBox.SetItemData(rComboBox.GetCount()-1, CJobItem::enWarehouse3);
}

void CLib::SetComboBoxJobType(CComboBox& rComboBox)
{
	int nJobType[] = {
		enJobTypeAutoSto, enJobTypeAutoRet, enJobTypeAutoPR, enJobTypeAutoR2R, enJobTypeAutoW2W, enJobTypeAutoMove, 
		enJobTypeSemiSto, enJobTypeSemiRet, enJobTypeSemiPR, enJobTypeSemiR2R, enJobTypeSemiW2W, enJobTypeSemiMove 
	};
	int nJobTypeSize = sizeof(nJobType) / sizeof(nJobType[0]);
	for (int i=0; i<nJobTypeSize; ++i)
	{
		rComboBox.AddString(CJobItem::GetJobTypeString(nJobType[i]));
		rComboBox.SetItemData(rComboBox.GetCount()-1, nJobType[i]);
	}
}

void CLib::SetComboBoxGenCode(CComboBox& rComboBox)
{
	for (int i=11; i<=20; ++i)
	{
		rComboBox.AddString(CJobItem::GetGenString((BYTE)i));
		rComboBox.SetItemData(rComboBox.GetCount()-1, i);	
	}
}

void CLib::SetComboBoxQty(CComboBox& rComboBox)
{
	CString strTemp;
	for (int i=1; i<=20; ++i)
	{
		strTemp.Format(_T("%d개"), i);
		rComboBox.AddString(strTemp);
		int nCount = rComboBox.GetCount();
		rComboBox.SetItemData(nCount-1, i);	
	}
}

int CLib::GetStationNumByTrackNum(int nTrackNum)
{
	int nStationNum = 0;

	switch ( nTrackNum )
	{
	case	111:		nStationNum = 101;		break;		// 1층 전면 입고대
	case	110:		nStationNum = 102;		break;		// 1층 전면 출고대
	case	113:		nStationNum = 111;		break;		// 1층 검체 채취실 피킹대 #1
	case	116:		nStationNum = 112;		break;		// 1층 검체 채취실 피킹대 #2
	case	119:		nStationNum = 131;		break;		// 1층 생산 입출고대
	case	214:		nStationNum = 251;		break;		// 2층 후면 입출고대 #1
	case	211:		nStationNum = 252;		break;		// 2층 후면 입출고대 #2
	case	215:		nStationNum = 253;		break;		// 2층 후면 P/M
	case	314:		nStationNum = 351;		break;		// 3층 후면 입출고대 #1
	case	311:		nStationNum = 352;		break;		// 3층 후면 입출고대 #2
	case	315:		nStationNum = 353;		break;		// 3층 후면 P/M
	}

	return nStationNum;
}

CString CLib::GetCycleStepName(BYTE ucCycleStep)
{
	switch(ucCycleStep)
	{
	case 0				:	return _T("스텝 없음(0)");
	case SC_CYCLE_HOME	:	return _T("홈복귀");
	case SC_CYCLE_PICKUP:	return _T("Pickup");
	case SC_CYCLE_UNLOAD:	return _T("Unload");
	case SC_CYCLE_MOVE	:	return _T("Move");
	case SC_CYCLE_BMOVE	:	return _T("Belt Move");
	case SC_CYCLE_LEARN	:	return _T("학습");
	}

	CString strRet;
	strRet.Format(_T("알수 없는 step(%d)"), ucCycleStep);
	return strRet;
}

BOOL CLib::GetLocationFromPortNum(int nPortNum, int& nBank, int& nBay, int& nLevel)
{
	nBank = 1;
	switch(nPortNum)
	{
	case ECS_STN_POS_PORT_1:
//		nBank = BANK_1;
		nBay = 3;
		nLevel = 8;
		return TRUE;

	case ECS_STN_POS_PORT_2:
//		nBank = BANK_1;
		nBay = 5;
		nLevel = 8;
		return TRUE;

	case ECS_STN_POS_PORT_3:
//		nBank = BANK_1;
		nBay = 5;
		nLevel = 7;
		return TRUE;

	case ECS_STN_POS_PORT_4:
//		nBank = BANK_1;
		nBay = 5;
		nLevel = 6;
		return TRUE;

	}

	return FALSE;
}
//==============================================================================
//	Desc	: 입력된 문자열을 항목별로 분리하여 문자열 배열로 반환
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CLib::Split(CString strSrc, CStringArray& strDest, LPCTSTR lpszInd)
{
	int nPos = 0;
	strDest.RemoveAll();
	while (TRUE)
	{
		// End
		if (nPos > 0 && strSrc.Find(lpszInd, nPos) == -1)
		{
			strDest.Add(strSrc.Mid(nPos, strSrc.GetLength() - nPos));
			break;
		}
		strDest.Add(strSrc.Mid(nPos, strSrc.Find(lpszInd, nPos) - nPos));
		nPos = strSrc.Find(lpszInd, nPos) + 1;
	}
}

//===========================================================================
//	Desc    : CRC16 값 가져오기
//	Date	: 
//	Update	: 
//===========================================================================
unsigned short CLib::GetCRC16(unsigned char* uszData, unsigned short nSize)
{
	unsigned short	nCRC = 0xFFFF;

	for (unsigned short nIndex = 0; nIndex < nSize; nIndex++)
	{
		nCRC = (nCRC & 0xFF00) | (nCRC ^ (unsigned short)(uszData[nIndex] & 0xFF));

		for (unsigned char bit = 0; bit < 8; bit++)
		{
			if (nCRC & 0x0001)
				nCRC = (nCRC >> 1) ^ 0xA001;
			else
				nCRC >>= 1;
		}
	}

	return(nCRC);
}

