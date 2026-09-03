// Lib.h : Global Library Header

#pragma once

class CEcsDoc;
class CTrackInfo;
class CJobItem;
class CComboBoxWrapper;
class CLib
{


public:
	static CString GetTupleKeyEQP_ECD_MST(CString pstrEQP_TYP, CString pstrERROR_CODE);


public:
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
	static char DecToChar(int nSrc);
	static int CharToDec(char szSrc);

	static BYTE GetByteL(WORD wValue) { return (BYTE)(wValue & 0x00FF); }
	static BYTE GetByteH(WORD wValue) { return (BYTE)((wValue & 0xFF00) >> 8); }
	static WORD GetWord(BYTE ucHigh, BYTE ucLow) { return (WORD)((ucHigh << 8) | ucLow); }

	static DWORD	SwapToDWord(BYTE *pSrc);
	static WORD		SwapToWord(BYTE *pSrc);

public:
	static CString ToString(int nValue);
	static CString ToString(UINT uValue);
	static CString Quot(CString strValue);
	static CString Concat(CString pstrValue1, CString pstrValue2);

public:
	static CString ConvertCTimeToOracleDateTimeString(CTime pDate, CTime pTime);
	static void ApplyTopN(CString& strSql, CString strN);	// [LGLS] insert TOP n after first SELECT (MSSQL paging)
	static CString ConvertCTimeToOracleTimeString(CTime t);
	static CString ConvertCTimeToOracleDateString(CTime t);
	static CString ConvertReasonToString(int nType);
	static CString ConvertScStationToImsScNo(CString strScStation);
	static CString ConvertImsScNoToScStation(CString strImsScNo);
	static CString ConvertEcsScNoToImsScNo(CString strEcsScNo);

public:
	static HICON HICONFromPATH(CString pstrPath);

public:
	static CString GetSystemErrMsg(int nErrNo=0);
	static CString GetExceptionString(CException* e);
	static CString GetHostResultSting(int nResultCode);

public:
	enum { enLengthIDPID = 11, enLengthBIDPID = 12, enLengthBarcode = 6 };
	static BOOL IsValidIDPID(CString& strIDPID);
	static BOOL IsValidBIDPID(CString& strBIDPID);
	static BOOL IsValidBarcode(CString& strBarcode);
	static BOOL IsValidLine(int nLine);
	static BOOL	IsDestStn2F(CString strDestStn); //조립동 2층으로 가는 작업인지를 체크

public:
	static BOOL ParsingStackerID(CString& strStackerCrane, CString& strStackerStation, CString& strLog, int& nScNum);

public:
	enum {enLengthStn = 4, enLengthLocation = 6, enLengthSKILocation = 10 };
	static BOOL IsValidLocation(int nWarehouse, CString& strLocation);
	static BOOL IsValidLocation(int nWarehouse, CString& strLocation, CString& strStn);
	static BOOL IsValidSKILocation(CString& strSKILocation);
	static BOOL IsDisableLocation(int nWarehouse, CString& strLocation, CString& strStn);
	static BOOL IsDisableSKILocation(CString& strLocation);
	static int	GetStackerNum(int nWarehouse, CString& strLocation);						//&& 추가
	static int	GetBank(CString& strLocation);
	static int	GetBay(CString& strLocation);
	static int	GetLevel(CString& strLocation);
	static CString GetStageLocation(int nStage);
	static BOOL ReverseBank(CString& strLocation);

public:
	enum { enLengthControlID = 10 };
	enum { enLengthControlID_8 = 8 };
	enum { enLengthControlMCNO = 3 };
	static BOOL IsValidControlID(CString& strCID);
	static BOOL IsValidControlMCNO(CString& strMC_NO);
	static int	GetWarehouse(CString& strCID);
	static int	GetFloor(CString& strCID);
	static int  GetEquipment(CString& strCID);
	static int  GetEquipNum(CString& strCID);
	static int	GetJobTypeFromInv(char chType);
	static int	GetJobTypeFromInv(int nMesJobType);
	static BOOL GetStoStation(int nBcrNum);

public:
	static void SetComboBoxWarehouse(CComboBox& rComboBox);
	static void SetComboBoxJobType(CComboBox& rComboBox);
//	static void SetComboBoxGenCode(CComboBox& rComboBox);
	static void SetComboBoxFloor(CComboBox& rComboBox);

public:
	static CString SetSpreadComboBoxJobType();
	static int GetSpreadRowValueToComboBoxIndex(CString strComboBoxEnum, CString strSpreadValue);

public:
	static int GetStationNumByTrackNum(int nTrackNum);
	static int GetTrackNumByFloor(int nTrackNum);
	static int CenterEquipFloor(int nCenterEquipNum);
	
public:
	// 입력된 문자열을 항목별로 분리하여 문자열 배열로 반환
	static void Split(CString strSrc, CStringArray& strDest, LPCTSTR lpszInd);
	// CRC16 구하기
	static unsigned short GetCRC16(unsigned char* uszData, unsigned short nSize);
	
public:
//	static void SetTogleButton(CSpreadSheet* pSpread, BOOL bCondition, CString strTogle1, CString strTogle2, int nCol, int nTagNum);
//	static void SetSpreadDefaultOneRow(CSpreadSheet* pSpread, CString strSubject, CString strData	, int& nRow, BOOL bCellSpan=FALSE, int nSpanCol=0, int nSpanColNum=0, int nMidIndex=0);
//	static void SetSpreadDefaultOneRow(CSpreadSheet* pSpread, CString strSubject, int nData			, int& nRow, BOOL bCellSpan=FALSE, int nSpanCol=0, int nSpanColNum=0, int nMidIndex=0);
//	static void SetTrayInfoToSpread(CSpreadSheet* pSpread, CJobItem* pJobItem1, CJobItem* pJobItem2 = NULL);


public:
//	static void SetSpreadColSize(CSpreadSheet* pSpread, int pColArgs...);
	static WORD CreateStatusData( int bitParm0...);

public:
	static CString GetIniStringFromPath(CString pstrPath, CString pstrKey, int penLang = 0);
	static CString GetIniStringFromPath_FONT_LOAD(CString pstrPath, CString pstrKey, int penLang = 0);
	
	static CString GetIniStringWH_TYP(CString pstrDefault = _T("10"));

public:
	static void GetComBoBoxData(CComboBoxWrapper& cbx, CString& strVal, int nlength = 0);
	static bool BindCombo(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, BOOL pbIsAll = FALSE);
	// [LGLS 2026-08-23] 조회 콤보에서 선택 키 얻기.
	//   조회 콤보는 CBS_DROPDOWN(직접 입력 가능)이라 목록에서 고르지 않으면 GetCurSel()이 -1 이 되고,
	//   그러면 조건이 통째로 빠져 필터가 걸리지 않는다(송수신자 W2E 를 골라도 전부 나오던 문제).
	//   -1 이면 표시 문자열로 항목을 되찾아 키를 돌려준다.
	static CString GetComboKey(CComboBoxWrapper& cbx);
	static bool BindCombo_STN_KIND(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, BOOL pbIsAll = FALSE);
	static bool BindComboPtr(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG);
	static bool BindCombo_ViewUsageRackDlg(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG, CString strAGING_TYP);
	static bool BindCombo_SC_NO(CComboBox& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, CString strAGING_TYP);
	static bool BindCombo_CELL_SC_NO(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, CString strAGING_TYP, CString strSC_NO);
	static bool BindCombo_SC_HS_DEF(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG, CString strSC_NO, CString strJOB_TYP);
	static bool BindCombo_SC_HS_DEF_DEST(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG, CString strSC_NO);
	static bool BindCombo_SC_HS_DEF_DEST_SPREAD(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG, CString strSC_NO);
	static BOOL BindCombo_SC_NO_MANUAL(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, CString strAGING_TYP, CString strSC_NO);
	// [LGLS 2026-09-03] RGV 정차 트랙(출발지/도착지) - 트랙번호로 표시
	static bool BindCombo_RTV_TRACK(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG);
	// [LGLS 2026-09-03] 크레인 N호기의 H/S 트랙(해당 S/C측 C/V 의 두 트랙) - CV_DATA 이름 기준
	static bool BindCombo_SC_HS_TRACK(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG, int nScIdx);
	static BOOL BindCombo_RTV_NO(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG);
	static bool SetBindCombo_DEST_POS_DEF(CComboBoxWrapper& cbx, CEcsDoc *pDoc);
	static bool BindSpreadCommonCode(CString pCDX_CD, CString pCCD_NM_XXX, CString& pCCD_CD, CEcsDoc *pDoc);

public:
	static void CellMasking( CString& pstrCELL_NO_TO );
	static CString GetCommonCode(CString& strSql, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG);
	static CString GetEng(CString& strLang, CEcsDoc* pDoc, int nLang);
	static CString GetCommonCodeLang( CString pstrSql, int pnLang );
	static CTime ConvertCStringToCTime(CString pString);
	static CTime ConvertCStringToCTime_Time(CString pString);
	static CTime ConvertCStringToCTime_Date(CString pString);
	static COleDateTime ConvertCStringToCOleDateTime(CString pString);
	static void GetColumnName(CStringArray& pArrColName, CString pString);

	// [LGLS 2026-08-01] 기준 행(라벨/값) 바로 아래에 두 행을 런타임 생성하고 담고 있는 그룹박스·창을 그만큼 늘린다.
	//   nLblAbove : 행 간격 산출용(기준 행 바로 위 라벨). pGrpIds : 함께 늘릴 그룹박스 ID 배열.

	// [LGLS 2026-08-01] 조회 조건 입력칸(라벨+편집) 1쌍을 런타임 생성. 스타일=nRef*, 위치=nPos*(숨긴 예비칸 활용).
	// [LGLS 2026-08-05] observables.tsv(설비 PLC 주소표)에서 실주소를 조회 (예: D0106/M0310/R0300)
	static CString GetObsAddr(CString pstrOwner, CString pstrName);
	static void AddFilterField(CWnd* pDlg, int nLblAboveRow, int nLblRow, int nValRow,
	                           LPCTSTR szText, int nLblId, int nEdtId, CPtrArray& arKeep);
	static void AddTwoRowsBelow(CWnd* pDlg, int nLblAbove, int nLbl, int nVal,
	                            LPCTSTR szText1, int nLbl1, int nVal1,
	                            LPCTSTR szText2, int nLbl2, int nVal2,
	                            const int* pGrpIds, int nGrpCnt, CPtrArray& arKeep);
	static void RenameColumn(CStringArray& pColArray, int pLang, CStringArray& pColReName);
	static CString QuotLikeLR( CString pStr );
	static CString QuotLikeL( CString pStr );
	static CString QuotLikeR( CString pStr );

};
