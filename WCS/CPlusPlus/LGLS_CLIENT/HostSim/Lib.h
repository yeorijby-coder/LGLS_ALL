// Global Library Header

#ifndef __LIB_H__
#define __LIB_H__

class CConfig;
class CJobItem;
class CLib
{
public:
	CConfig* m_pConfig;

	static BOOL IsSet(BYTE Byte, int nPos);
	static BOOL IsSet(WORD Word, int nPos);
	static BOOL IsSet(DWORD dwWord, int nPos);
	static BOOL IsSet(UINT Int, int nPos);
	static void SetBit(WORD& wValue, int nPos, BOOL bSet);

	static int Power(int x, int y);

	static DWORD HexStrToDWORD(BYTE* bySrc);
	static WORD HexStrToWORD(BYTE* bySrc);
	static BYTE HexStrToBYTE(BYTE* bySrc);
	static BYTE HexStrToBYTE(BYTE bySrc);

	static BYTE GetByteL(WORD wValue) { return (BYTE)(wValue & 0x00FF); }
	static BYTE GetByteH(WORD wValue) { return (BYTE)((wValue & 0xFF00) >> 8); }
	static WORD GetWord(BYTE ucHigh, BYTE ucLow) { return (WORD)((ucHigh << 8) | ucLow); }

	static BOOL IsOutSideForDouble(CString& strLocation) { return ((((_ttoi(strLocation.Mid(0, 2)) + 2) % 4) / 2) + 1) == 2; }
	static BOOL IsInSideForDouble(CString& strLocation) { return ((((_ttoi(strLocation.Mid(0, 2)) + 2) % 4) / 2) + 1) == 1; }
	static BOOL IsNeedBeltMove(CString& strLocation1, CString& strLocation2);
	static BOOL IsNeedBeltMove(CString& strLocation, int nForkNum, int nPattern);
	static BOOL PairJobCheck(CJobItem* pInPutJobItem1, CJobItem* pInPutJobItem2, int& nSelectJobItem);
	static BOOL PairJobCheckR2R(CJobItem* pInPutJobItem1, CJobItem* pInPutJobItem2, int& nSelectJobItem, CString& strLocation1, CString& strLocation2, BOOL bFromLoc);

	static DWORD	SwapToDWord(BYTE *pSrc);
	static WORD		SwapToWord(BYTE *pSrc);

	static CString  makeStringFromBin(BYTE *pData, UINT nLen);

	static BOOL		GetLocationFromPortNum(int nPortNum, int& nBank, int& nBay, int& nLevel);

public:
	static CString ToString(int nValue);
	static CString ToString(UINT uValue);

public:
//	static CString ConvertReasonToString(int nType);

public:
	static CString GetCycleStepName(BYTE ucCycleStep);

public:
	static CString GetSystemErrMsg(int nErrNo=0);
	static CString GetExceptionString(CException* e);
	static CString GetHostResultSting(int nResultCode);

public:
	enum { enLengthIDPID = 11, enLengthBIDPID = 12, enLengthBarcode = 31 };
	static BOOL IsValidIDPID(CString& strIDPID);
	static BOOL IsValidBIDPID(CString& strBIDPID);
	static BOOL IsValidBarcode(CString& strBarcode);

public:
	enum { enLengthLocation = 7 };
	static BOOL IsValidLocation(int nWarehouse, CString& strLocation);
	static BOOL IsDisableLocation(int nWarehouse, CString& strLocation);
	static int	GetStackerNum(int nWarehouse, CString& strLocation, BOOL bDouble = FALSE);
	static int	GetBank(CString& strLocation);
	static int	GetBay(CString& strLocation);
	static int	GetLevel(CString& strLocation);
	static int	GetSide(CString& strLocation);
	static int	GetDoubleSide(CString& strLocation);

public:
	enum { enLengthControlID = 9 };
	static BOOL IsValidControlID(CString& strCID);
	static int	GetWarehouse(CString& strCID);
	static int	GetFloor(CString& strCID);
	static int  GetEquipment(CString& strCID);
	static int  GetEquipNum(CString& strCID);
	static int	GetJobTypeFromInv(char chType);

public:
	static void SetComboBoxWarehouse(CComboBox& rComboBox);
	static void SetComboBoxJobType(CComboBox& rComboBox);
	static void SetComboBoxGenCode(CComboBox& rComboBox);
	static void SetComboBoxQty(CComboBox& rComboBox);

public:
	static int GetStationNumByTrackNum(int nTrackNum);

public:
	// 입력된 문자열을 항목별로 분리하여 문자열 배열로 반환
	static void Split(CString strSrc, CStringArray& strDest, LPCTSTR lpszInd);
	// CRC16 구하기
	static unsigned short GetCRC16(unsigned char* uszData, unsigned short nSize);
};

#endif // __LIB_H__