
#include "stdafx.h"
#include "Ecs.h"
#include "Lib.h"
#include "EcsDoc.h"


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

WORD CLib::HexStrToWORD(TCHAR* bySrc)
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

//==============================================================================
//	Desc	: 입력된 16진수값을 문자값으로 반환
//	Date    : 2009.12.16
//	Update	: 
//==============================================================================
char CLib::DecToChar(int nSrc)
{
	switch (nSrc)
	{
		case 0 : return ('0');
		case 1 : return ('1');
		case 2 : return ('2');
		case 3 : return ('3');
		case 4 : return ('4');
		case 5 : return ('5');
		case 6 : return ('6');
		case 7 : return ('7');
		case 8 : return ('8');
		case 9 : return ('9');
		case 10 : return ('A');
		case 11 : return ('B');
		case 12 : return ('C');
		case 13 : return ('D');
		case 14 : return ('E');
		case 15 : return ('F');
		default   : return ('0');
	}
}

//==============================================================================
//	Desc	: 입력된 문자값을 10진수값으로 반환
//	Date    : 2009.12.16
//	Update	: 
//==============================================================================
int CLib::CharToDec(char szSrc)
{
	switch (szSrc)
	{
		case '0' : return (0);
		case '1' : return (1);
		case '2' : return (2);
		case '3' : return (3);
		case '4' : return (4);
		case '5' : return (5);
		case '6' : return (6);
		case '7' : return (7);
		case '8' : return (8);
		case '9' : return (9);
		case 'A' : return (10);
		case 'B' : return (11);
		case 'C' : return (12);
		case 'D' : return (13);
		case 'E' : return (14);
		case 'F' : return (15);
		default  : return (0);
	}
}

WORD CLib::SwapToWord(BYTE *pSrc)
{
	return (WORD)(pSrc[1] << 8) | pSrc[0];
}

DWORD CLib::SwapToDWord(BYTE *pSrc)
{
	return (DWORD)(pSrc[3] << 24) | (pSrc[2] << 16) | (pSrc[1] << 8) | pSrc[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CString CLib::GetSystemErrMsg(int nErrNo)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,
				  nErrNo ? nErrNo : ::GetLastError(),
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				  (LPTSTR) &lpMsgBuf,
				  0,
				  NULL);

	CString str;
	str.Format(_T("%s"), (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	str.TrimRight();

	return str; 
}

CString CLib::GetExceptionString(CException* e)
{
	CString strLog="", strClassName = "";
	TCHAR szMessage[512] = {0};
	e->GetErrorMessage(szMessage, sizeof(szMessage));

//	CRuntimeClass * pRTClass = e->GetRuntimeClass();
//	strClassName = pRTClass->m_lpszClassName;

//	strLog.Format(_T("[%s]"), szMessage);
	return strLog;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CString CLib::GetHostResultSting(int nResultCode)
{
	switch (nResultCode)
	{
	case enHostErrorNone:	return _T("정상");
	case enHostError9001:	return _T("ECS에서 작업이 진행중입니다.");
	case enHostError9002:	return _T("재고와 Tray공정이 불일치합니다.");
	case enHostError9003:	return _T("재고와 TrayType이 불일치합니다.");
	case enHostError9004:	return _T("미정의된 포트입니다.");
	case enHostError9005:	return _T("출고 금지된 포트입니다.");
	case enHostError9006:	return _T("작업(물류) 진행중인 Tray입니다.");
	case enHostError9007:	return _T("다음 진행가능한 경로를 찾을 수 없습니다.");
	case enHostError9008:	return _T("최대 작업수 상태입니다.");
	case enHostError9009:	return _T("ECS에서 작업 진행중입니다,");
	case enHostError9010:	return _T("재고와 Tray 공정이 불일치합니다.");
	case enHostError9011:	return _T("재고와 Tray Type이 불일치합니다.");
	case enHostError9012:	return _T("작업 가능한 설비가 없습니다.");
	case enHostError9013:	return _T("출고 금지 상태인 포트입니다.");
	case enHostError9014:	return _T("다음 경로를 찾을수 없습니다.");
	case enHostError9015:	return _T("최대 작업수입니다.");
	case enHostError9016:	return _T("ECS에서 진행중인 Tray입니다.");
	case enHostError9017:	return _T("다른 위치에 존재하는 Tray입니다.");
	case enHostError9018:	return _T("Type이 종료된 Tray 입니다.");
	case enHostError9019:	return _T("현재 공정을 찾을 수 없습니다.");
	case enHostError9020:	return _T("경로를 찾을 수 없습니다.");
	case enHostError9021:	return _T("사용금지된 포트 번호입니다.");
	case enHostError9022:	return _T("최대 작업수 입니다.");
	case enHostError9023:	return _T("사용금지된 포트 번호입니다.");
	case enHostError9024:	return _T("다른 위치에 존재하는 Tray입니다.");
	case enHostError9025:	return _T("입고가능한 로케이션이 없습니다.");
	case enHostError9026:	return _T("해당설비에서 응답이 오기를 기다리고 있습니다.");
	case enHostError9027:	return _T("다른위치에 제공이 존재하는 Tray입니다.");
	case enHostError9029:	return _T("입고 가능한 위치를 검색할 수 없습니다.");
	case enHostError9030:	return _T("충방전기 예약 처리중.");
	case enHostError9031:	return _T("충방전기 예약 처리중.");
	case enHostError9032:	return _T("작업데이터를 찾을 수 없습니다.");
	case enHostError9033:	return _T("등록되지 않은 Tray입니다.");
	case enHostError9034:	return _T("잘못된 저장 위치입니다.");
	case enHostError9035:	return _T("잘못된 저장 위치입니다.");
	case enHostError9036:	return _T("해당위치의 Tray 정보가 상이합니다.");
	case enHostError9037:	return _T("공정을 찾을수 없습니다.");
	case enHostError9038:	return _T("공정 미완료");
	case enHostError9039:	return _T("공정 불일치");
	case enHostError9200:	return _T("select statement error (DB 이상)");
	case enHostError9203:	return _T("작업이 진행중인 Tray ID입니다.");
	case enHostError9207:	return _T("등록된 Tray ID가 아닙니다.");
	case enHostError9209:	return _T("공정번호가 없습니다.");
	case enHostError9210:	return _T("공정이 진행중 입니다.(1차)");
	case enHostError9211:	return _T("공정이 진행중 입니다.(2차)");
	case enHostError9212:	return _T("공정이 진행중 입니다.");
	case enHostError9217:	return _T("Tray내의 Channel이 중복 되어 있습니다.");
	case enHostError9240:	return _T("공 Tray가 아닙니다.");
	case enHostError9241:	return _T("입고할 수 없는 Tray 형식입니다.");
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
	if (strBarcode.GetLength() != g_nLenBarcode)
		return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CLib::IsValidLocation(int nWarehouse, CString& strLocation)
{
	if (strLocation.GetLength() != g_nLenLocation)
	{
		DEBUGER_TRACE(strLocation, _T("CLib::IsValidLocation"));
		return FALSE;
	}

	int nBank = GetBank(strLocation);
	int nBay = GetBay(strLocation);
	int nLevel = GetLevel(strLocation);

	switch (nWarehouse)
	{
	case CJobItem::enWarehouse1:
		{
			if ((nBank < 1) || (nBank > 2))		
			{
				DEBUGER_TRACE(strLocation, _T("CLib::IsValidLocation"));
				return FALSE;
			}
			if (nBank == 1)
			{
				if ((nBay < 1) || (nBay > 7))
				{
					DEBUGER_TRACE(strLocation, _T("CLib::IsValidLocation"));
					return FALSE;
				}
			}
			else if (nBank == 2)
			{
				if ((nBay < 1) || (nBay > 6))
				{
					DEBUGER_TRACE(strLocation, _T("CLib::IsValidLocation"));
					return FALSE;
				}
			}
			if ((nLevel < 1) || (nLevel > 8))
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

	switch (nWarehouse)
	{
	case CJobItem::enWarehouse1:
		{
			if ((nBank == 1) && (nBay == 1) && ((nLevel == 2) || (nLevel == 7) || (nLevel == 8)))
				return TRUE;
		}
		break;
		
	default:
		DEBUGER_TRACE(CConvert::ToString(nWarehouse), _T("CLib::IsDisableLocation"));
		return FALSE;
	}

	return FALSE;
}

CString CLib::ConvertReasonToString(int nType)
{
// @@@@@
/*	switch (nType)
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
		strTemp.Format(_T("Unknown Reason(%d)"), nType);
		return strTemp;
	}
*/
	return "";
}


int CLib::GetJobTypeFromInv(char chType)
{
    switch(chType)
	{
	case '1':			return enJobTypeAutoSto;
	case '2':           return enJobTypeAutoRet;
	case '3':			return enJobTypeAutoPR;
//	case '3':			return enJobTypeAutoMove;
//	case '4':           return enJobTypeAutoR2R;
//	case '5':           return enJobTypeAutoW2W;
	case '6':			return enJobTypeAutoMove;
	}
	return 0;
}

int CLib::GetStackerNum(int nWarehouse, CString& strLocation)
{
	if (strLocation.GetLength() != g_nLenLocation)
		return 0;

	int nScNum = GetBank(strLocation);

	switch (nWarehouse)
	{
	case CJobItem::enWarehouse1:
		break;
	}

	return nScNum;
}
// @@@@@
int CLib::GetSide(CString& strLocation)
{
	if (strLocation.GetLength() != g_nLenLocation)
		return 0;

//	return ((_ttoi(strLocation.Mid(0, 2)) + 1) % 2) + 1;

	int n = GetBank(strLocation);
	if (n == 1) return 2;
	else if (n == 2) return 1;

	return 0;
}

int CLib::GetBank(CString& strLocation)
{
	if (strLocation.GetLength() != g_nLenLocation)
		return 0;

	return _ttoi(strLocation.Left(2));
}

int CLib::GetBay(CString& strLocation)
{
	if (strLocation.GetLength() != g_nLenLocation)
		return 0;

	return _ttoi(strLocation.Mid(2,3));
}

int CLib::GetLevel(CString& strLocation)
{
	if (strLocation.GetLength() != g_nLenLocation)
		return 0;

	return _ttoi(strLocation.Right(2));
}

CString CLib::GetLocation(int nStage)
{
	CString strLoc = _T("");
	int nTemp = nStage % 100;
	int nMod = nTemp % 8;

	strLoc.Format(_T("%02d%03d%02d"), 
				nStage / 100, 
				(nTemp / 8) + ((nMod > 0) ? 1 : 0),
				(nMod == 0) ? 8 : nMod);

	return strLoc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CLib::IsValidControlID(CString& strCID)
{
	if (strCID.GetLength() != g_nLenControlID)
		return FALSE;

	return TRUE;
}

int CLib::GetWarehouse(CString& strCID)
{
	DEBUGER_ASSERT_VALID(strCID.GetLength() == g_nLenControlID);
	return _ttoi(strCID.Mid(0, 1));
}

int CLib::GetFloor(CString& strCID)
{
	DEBUGER_ASSERT_VALID(strCID.GetLength() == g_nLenControlID);
	return _ttoi(strCID.Mid(1, 1));
}

int CLib::GetEquipment(CString& strCID)
{
	DEBUGER_ASSERT_VALID(strCID.GetLength() == g_nLenControlID);
	return _ttoi(strCID.Mid(3, 2));
}

int CLib::GetEquipNum(CString& strCID)
{
	DEBUGER_ASSERT_VALID(strCID.GetLength() == g_nLenControlID);
//	return _ttoi(strCID.Right(3));
	return _ttoi(strCID.Right(3));

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
		enJobTypeAutoSto, enJobTypeAutoRet, /*enJobTypeAutoPR, enJobTypeAutoR2R, enJobTypeAutoW2W,*/ enJobTypeAutoMove, 
		enJobTypeSemiSto, enJobTypeSemiRet, /*enJobTypeSemiPR, enJobTypeSemiR2R, enJobTypeSemiW2W */ enJobTypeSemiMove, 
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
