// Lib.cpp: implementation of the CLib class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Lib.h"
#include "MainFrm.h"
#include "EcsView.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CString CLib::GetTupleKeyEQP_ECD_MST(CString pstrEQP_TYP, CString pstrERROR_CODE)
{
   CString strTemp = _T("");
   strTemp.Format(_T("%s#%04s"), pstrEQP_TYP, pstrERROR_CODE);
   return strTemp;
}


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

      if(i == 0)      byDest <<= 4;
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

   return   0;
}

//==============================================================================
//   Desc   : 입력된 16진수값을 문자값으로 반환
//   Date    : 2009.12.16
//   Update   : 
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

CString CLib::ConvertCTimeToOracleDateTimeString(CTime pDate, CTime pTime)
{
   CString strTemp = ConvertCTimeToOracleDateString(pDate) + ConvertCTimeToOracleTimeString(pTime);
   return strTemp;
}

CString CLib::ConvertCTimeToOracleDateString(CTime t)
{
   //CString strTemp = t.Format("%m/%d/%Y %H:%M:%S");
   CString strTemp = t.Format("%Y%m%d");
   return strTemp;
}

CString CLib::ConvertCTimeToOracleTimeString(CTime t)
{
   //CString strTemp = t.Format("%m/%d/%Y %H:%M:%S");
   CString strTemp = t.Format("%H%M%S");
   return strTemp;
}

// [LGLS] MSSQL paging helper: insert " TOP n" after the first SELECT (and optional DISTINCT)
void CLib::ApplyTopN(CString& strSql, CString strN)
{
   int nPos = strSql.Find(_T("SELECT"));
   if (nPos < 0) { return; }
   int nIns = nPos + 6;
   int nDist = strSql.Find(_T("DISTINCT"), nIns);
   if (nDist >= 0)
   {
      CString strBetween = strSql.Mid(nIns, nDist - nIns);
      strBetween.Trim();
      if (strBetween.IsEmpty()) { nIns = nDist + 8; }
   }
   strSql.Insert(nIns, _T(" TOP ") + strN);
}


CTime CLib::ConvertCStringToCTime(CString pString)
{
   //2019-11-06 14:08:52
   int nYear = _wtoi(pString.Left(4));
   int nMon = _wtoi(pString.Mid(5,2));
   int nDay = _wtoi(pString.Mid(8,2));
   int nHour = _wtoi(pString.Mid(11,2));
   int nMin = _wtoi(pString.Mid(14,2));
   int nSec = _wtoi(pString.Mid(17,2));

   CTime tTime(nYear, nMon, nDay, nHour, nMin, nSec);
   
   return tTime;
}


COleDateTime CLib::ConvertCStringToCOleDateTime(CString pString)
{
   //2019-11-06 14:08:52
   int nYear = _wtoi(pString.Left(4));
   int nMon = _wtoi(pString.Mid(5,2));
   int nDay = _wtoi(pString.Mid(8,2));
   int nHour = _wtoi(pString.Mid(11,2));
   int nMin = _wtoi(pString.Mid(14,2));
   int nSec = _wtoi(pString.Mid(17,2));

   COleDateTime tTime(nYear, nMon, nDay, nHour, nMin, nSec);
   
   return tTime;
}

CTime CLib::ConvertCStringToCTime_Time(CString pString)
{
   //2019-11-06 14:08:52
   int nYear = _wtoi(pString.Left(4));
   int nMon = _wtoi(pString.Mid(5,2));
   int nDay = _wtoi(pString.Mid(8,2));
   int nHour = _wtoi(pString.Mid(11,2));
   int nMin = _wtoi(pString.Mid(14,2));
   int nSec = _wtoi(pString.Mid(17,2));

   CTime tTime(nHour, nMin, nSec);
   
   return tTime;
}

CTime CLib::ConvertCStringToCTime_Date(CString pString)
{
   //2019-11-06 14:08:52
   int nYear = _wtoi(pString.Left(4));
   int nMon = _wtoi(pString.Mid(5,2));
   int nDay = _wtoi(pString.Mid(8,2));
   int nHour = _wtoi(pString.Mid(11,2));
   int nMin = _wtoi(pString.Mid(14,2));
   int nSec = _wtoi(pString.Mid(17,2));

   CTime tTime(nYear, nMon, nDay);
   
   return tTime;
}


//==============================================================================
//   Desc   : 입력된 문자값을 10진수값으로 반환
//   Date    : 2009.12.16
//   Update   : 
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

HICON CLib::HICONFromPATH(CString pstrPath)
{

   CImage image;
   image.Load(pstrPath);
   CBitmap bitmap;
   bitmap.Attach(image.Detach());
   BITMAP bmp;
   bitmap.GetBitmap(&bmp);


   HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL), 
      bmp.bmWidth, bmp.bmHeight);

   ICONINFO ii = {0};
   ii.fIcon    = TRUE;
   ii.hbmColor = bitmap;
   ii.hbmMask  = hbmMask;

   HICON hIcon = ::CreateIconIndirect(&ii);
   ::DeleteObject(hbmMask);

   return hIcon;
}

BOOL CLib::IsValidControlID(CString& strCID)
{
   if (strCID.GetLength() != enLengthControlID_8)
   {
      return FALSE;
   }

   return TRUE;
}

BOOL CLib::IsValidControlMCNO(CString& strMC_NO)
{
   if (strMC_NO.GetLength() != enLengthControlMCNO)
   {
      return FALSE;
   }

   return TRUE;
}

int CLib::GetWarehouse(CString& strCID)
{
   DEBUGER_ASSERT_VALID(strCID.GetLength() == enLengthControlID);
   return CConvert::ToInt(strCID.Mid(1, 1));
}

int CLib::GetFloor(CString& strCID)
{
   DEBUGER_ASSERT_VALID(strCID.GetLength() == enLengthControlID);
   return CConvert::ToInt(strCID.Mid(2, 1));
}

int CLib::GetEquipment(CString& strCID)
{
   DEBUGER_ASSERT_VALID(strCID.GetLength() == enLengthControlID);
   return CConvert::ToInt(strCID.Mid(3, 2));
}

int CLib::GetEquipNum(CString& strCID)
{
   DEBUGER_ASSERT_VALID(strCID.GetLength() == enLengthControlID);
   return CConvert::ToInt(strCID.Right(5));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//================================================================================= 
// DESC   : 콤보박스에서 인덱스 찾아오기
// DATE   : 2016 - 06 - 18
// UPDATE : 
//=================================================================================
int CLib::GetSpreadRowValueToComboBoxIndex(CString strComboBoxEnum, CString strSpreadValue)
{
   int nIdx = 0;
   int nIdx2 = 0;
   CString strBuf = _T("");
   int nRt = 0;
   int nTmp = 0;
   BOOL bLast = FALSE;
   CString strCopy = strComboBoxEnum;

   while(TRUE)
   {
      nIdx = strCopy.Find(_T("\t"));
      nIdx2 = strCopy.Find(_T("\t"), nIdx+1);
      if (nIdx2 == -1)
      {
         nIdx2 = strCopy.Find(_T("\n"), nIdx+1);
         bLast = TRUE;
      }

      strBuf = strCopy.Mid(nIdx+1, nIdx2-1);
      if (bLast == FALSE)
      {
         if (strBuf == strSpreadValue)
            return nRt + 1;
      }
      else
      {
         if (strBuf == strSpreadValue)
            return nRt + 1;
      }
         
      nRt++;
      if (strCopy.GetLength() == 0)
         return 0;

      strCopy = strCopy.Mid(strBuf.GetLength()+1, strComboBoxEnum.GetLength() - strBuf.GetLength());
      if (nRt > 30)
         break;
   }

   return 0;
}

int gnJobType[] = {
   enJobTypeAutoSto, enJobTypeAutoRet, enJobTypeAutoPR, enJobTypeAutoR2R, enJobTypeAutoA2A, enJobTypeAutoMove, 
   enJobTypeSemiSto, enJobTypeSemiRet, enJobTypeSemiPR, enJobTypeSemiR2R, /*enJobTypeSemiW2W */ enJobTypeSemiMove, 
};


//==============================================================================
//   Desc   : 입력된 문자열을 항목별로 분리하여 문자열 배열로 반환
//   Date   : 2013.09
//   Update   :
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
      if(nPos == 0){break;}
   }
}

//===========================================================================
//   Desc    : CRC16 값 가져오기
//   Date   : 
//   Update   : 
//===========================================================================
unsigned short CLib::GetCRC16(unsigned char* uszData, unsigned short nSize)
{
   unsigned short   nCRC = 0xFFFF;

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

// 스프레드 보류999
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void CLib::SetTrayInfoToSpread(CSpreadSheet* pSpread, CJobItem* pJobItem1, CJobItem* pJobItem2)
// {
//    if (pSpread == NULL || pJobItem1 == NULL)
//       return;
// 
//    pSpread->SetCurrentRow();         
//    pSpread->SetData(1, _T("트레이정보"));                   pSpread->SetData(2, _T("하단 트레이"));               pSpread->SetData(3, _T("상단 트레이"));         
//    if (pJobItem2 != NULL)
//    {
//       pSpread->SetData(4, _T("하단 트레이"));               pSpread->SetData(5, _T("상단 트레이"));
//    }
//    CString strTrayID1_1 = pJobItem1 ? pJobItem1->m_TrayInfo[0].m_strBarcode : _T("");
//    CString strTrayID1_2 = pJobItem1 ? pJobItem1->m_TrayInfo[1].m_strBarcode : _T("");
//    CString strProdID1_1 = pJobItem1 ? pJobItem1->m_TrayInfo[0].m_strProductID : _T("");
//    CString strProdID1_2 = pJobItem1 ? pJobItem1->m_TrayInfo[1].m_strProductID : _T("");
//    CString strProdName1_1 = pJobItem1 ? pJobItem1->m_TrayInfo[0].m_strProductName : _T("");
//    CString strProdName1_2 = pJobItem1 ? pJobItem1->m_TrayInfo[1].m_strProductName : _T("");
// 
//    CString strTrayID2_1 = pJobItem2 ? pJobItem2->m_TrayInfo[0].m_strBarcode : _T("");
//    CString strTrayID2_2 = pJobItem2 ? pJobItem2->m_TrayInfo[1].m_strBarcode : _T("");
//    CString strProdID2_1 = pJobItem2 ? pJobItem2->m_TrayInfo[0].m_strProductID : _T("");
//    CString strProdID2_2 = pJobItem2 ? pJobItem2->m_TrayInfo[1].m_strProductID : _T("");
//    CString strProdName2_1 = pJobItem2 ? pJobItem2->m_TrayInfo[0].m_strProductName : _T("");
//    CString strProdName2_2 = pJobItem2 ? pJobItem2->m_TrayInfo[1].m_strProductName : _T("");
// 
//    pSpread->SetCurrentRow();   pSpread->SetData(1, _T("Tray ID"));      pSpread->SetData(2, strTrayID1_1);      pSpread->SetData(3, strTrayID1_2);      pSpread->SetData(4, strTrayID2_1);      pSpread->SetData(5, strTrayID2_2);
//    pSpread->SetCurrentRow();   pSpread->SetData(1, _T("품목코드"));   pSpread->SetData(2, strProdID1_1);      pSpread->SetData(3, strProdID1_2);      pSpread->SetData(4, strProdID2_1);      pSpread->SetData(5, strProdID2_2);
//    pSpread->SetCurrentRow();   pSpread->SetData(1, _T("품목명"));      pSpread->SetData(2, strProdName1_1);   pSpread->SetData(3, strProdName1_2);   pSpread->SetData(4, strProdName2_1);   pSpread->SetData(5, strProdName2_2);
// 
//    pSpread->SetCurrentRow();
// }

/*
void CLib::SetTogleButton(CSpreadSheet* pSpread, BOOL bCondition, CString strTogle1, CString strTogle2, int nCol, int nTagNum)
{
   if (pSpread==NULL)
      return;

   CString strValue = _T("");

    strValue = (bCondition) ? strTogle1 : strTogle2;
   pSpread->SetButton(nCol, strValue);
   pSpread->SetUserCellTag(nCol, CConvert::ToString(nTagNum));
   pSpread->SetColor(nCol, SPREAD_COLOR_NONE, RED);      
}

void CLib::SetSpreadDefaultOneRow(CSpreadSheet* pSpread, CString strSubject, CString strData, int& nRow, BOOL bCellSpan, int nSpanCol, int nSpanColNum, int nMidIndex)
{
   // 스프레드 객체가 널이면 행을 표시하지 않음!
   if (pSpread == NULL)
      return;

   // 제목이 없으면 행을 표시하지 않음!
   if (strSubject == _T(""))
      return;

   CString strValue = _T("");

   // 행추가
   if (nMidIndex == 0)
      pSpread->SetCurrentRow();

   // 제목
   pSpread->SetData(1+nMidIndex, strSubject);   

   // 값
   pSpread->SetData(2+nMidIndex, strData);   

   // 셀병합
   if (bCellSpan == TRUE && nSpanCol != 0 && nSpanColNum != 0)
      pSpread->SetCellSpan(nSpanCol, nSpanColNum);

   nRow = pSpread->m_nCurrentRow;
}

void CLib::SetSpreadDefaultOneRow(CSpreadSheet* pSpread, CString strSubject, int nData, int& nRow, BOOL bCellSpan, int nSpanCol, int nSpanColNum, int nMidIndex)
{
   // 스프레드 객체가 널이면 행을 표시하지 않음!
   if (pSpread == NULL)
      return;

   // 제목이 없으면 행을 표시하지 않음!
   if (strSubject == _T(""))
      return;

   CString strValue = _T("");

   // 행추가
   if (nMidIndex == 0)
      pSpread->SetCurrentRow();

   // 제목
   pSpread->SetData(1+nMidIndex, strSubject);   

   // 값
   pSpread->SetData(2+nMidIndex, nData);   

   // 셀병합
   if (bCellSpan == TRUE && nSpanCol != 0 && nSpanColNum != 0)
      pSpread->SetCellSpan(nSpanCol, nSpanColNum);

   nRow = pSpread->m_nCurrentRow;
}
//*/
BOOL CLib::GetStoStation(int nBcrNum)
{
   switch(nBcrNum)
   {
   case    BCR_NO58 :
   case    BCR_NO62 :
   case    BCR_NO69 : 
   case    BCR_NO17 :
   case    BCR_NO28 : 
   case    BCR_NO42 :      return TRUE;
   default:            break;

   }
   return FALSE;
}

CString CLib::Quot(CString pstrValue)
{
   CString strTemp = _T("");
   strTemp.Format(_T("%c%s%c"), '\'', pstrValue, '\'');
   return strTemp;
}

CString CLib::Concat(CString pstrValue1, CString pstrValue2)
{
   CString strTemp = _T("");
   strTemp.Format(_T("%s%s"), pstrValue1, pstrValue2);
   return strTemp;
}
/*
void CLib::SetSpreadColSize(CSpreadSheet* pSpread, int pColArgs...)
{
   va_list argList;
   va_start(argList, pColArgs);

   for(int nIdxArgs = 0; nIdxArgs < pColArgs; nIdxArgs++)
   {
      int nColSize = va_arg(argList, int);
      pSpread->SetColWidth(nIdxArgs, nColSize);
   }

   va_end(argList);
}
//*/
WORD CLib::CreateStatusData( int bitParm0...)
{
   WORD rtWORD = 0;
   va_list argList;
   va_start(argList, bitParm0);
   

   for(int nIdxArgs = 0; nIdxArgs < bitParm0; nIdxArgs++)
   {
      int nParm = va_arg(argList, int);
      CLib::SetBit(rtWORD,nIdxArgs, (BOOL)nParm);//나중에수정하자
      //rtWORD = rtWORD | (bitParm0 << nIdxArgs);
   }

   va_end(argList);

   return rtWORD;
}

CString CLib::GetIniStringWH_TYP(CString pstrDefault)
{
   CString strReturn = _T("");
   TCHAR szTemp[_MAX_PATH] = {0};
   ::GetPrivateProfileString(_T("WH"), _T("WHTYP"), pstrDefault, szTemp, _MAX_PATH, ECS_INI_FILE);
   strReturn.Format(_T("%s"), szTemp);

   return strReturn;
}

CString CLib::GetIniStringFromPath(CString pstrPath, CString pstrKey, int penLang)
{

   TCHAR szTemp[_MAX_PATH] = {0};
   CString strSection = _T("");
   if(penLang == 0){ strSection = _T("kor");}
   else if(penLang == 1){strSection = _T("eng");}
   else if(penLang == 2){strSection = _T("hun");}
   else if(penLang == 3){strSection = _T("chin");}
   else{strSection = _T("eng");}

   CString strReturn = _T("");
   ::GetPrivateProfileString(strSection, pstrKey, _T(""), szTemp, _MAX_PATH, pstrPath);
   strReturn.Format(_T("%s"), szTemp);
   return strReturn;
}

CString CLib::GetIniStringFromPath_FONT_LOAD(CString pstrPath, CString pstrKey, int penLang)
{
   TCHAR szTemp[_MAX_PATH] = {0};
   CString strSection = _T("");
   if(penLang == 0){ strSection = _T("kor");}
   else if(penLang == 1){strSection = _T("eng");}
   else if(penLang == 2){strSection = _T("hun");}
   else if(penLang == 3){strSection = _T("chin");}
   else{strSection = _T("eng");}

   CString strReturn = _T("");
   CString strReversePath = pstrPath.Left(pstrPath.ReverseFind('\\'));
   ::GetPrivateProfileString(strSection, pstrKey, _T(""), szTemp, _MAX_PATH, pstrPath);
   strReturn.Format(_T("%s\\%s\\%s"), strReversePath, strSection, szTemp);
   return strReturn;
}
CString CLib::GetCommonCode(CString& p_strSql, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG)
{
   if(pDoc   == NULL)                     return FALSE;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CStringList strList;
   CString strSql = _T("");
   int nRowCnt = 0, j=0;
   CString strMessage;
   strSql.Format(_T(" SELECT CCD_CD            \n")
              _T("      , CCD_NM_KOR         \n")
              _T("      , CCD_NM_ENG         \n")
              _T("      , CCD_NM_HUN         \n")
              _T("      , CCD_NM_CHIN         \n")
              _T("    FROM COMMON_CODE         \n")
              _T("   WHERE CCD_CD_YN = 'Y'      \n")
              _T("    AND CDX_CD = '%s'           "), strCDX_CD);

   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 

   delete pRsw;
   return strSql;
}

CString CLib::GetEng(CString &strLang, CEcsDoc* pDoc, int nLang)
{
   int nRowCnt = -1;
   CString strSql = _T("");
   CString strTemp, strMessage;
    CString CRLF = _T("\r\n");

   if(pDoc == NULL)            return _T("");

   if (nLang != 1)
   {
      strSql += CRLF + _T("         SELECT CL.TO_LANG_E        \n");
      strSql += CRLF + _T("           FROM CHG_LANG CL         \n");
      if(nLang == 0)
         strSql += CRLF + _T("          WHERE CL.FROM_LANG = '") + strLang + _T("'\n");
      else if(nLang == 2)
         strSql += CRLF + _T("          WHERE CL.TO_LANG_H = '") + strLang + _T("'\n");
      else if(nLang == 3)
         strSql += CRLF + _T("          WHERE CL.TO_LANG_C = '") + strLang + _T("'\n");
   }
   else
      return strLang;
   _RecordsetPtr pRspEqpData = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRswEqpDataWrap = new CRecordSetWrap(pRspEqpData); 


   if(nRowCnt < 1)
   {   
      if(pRswEqpDataWrap != NULL)
      {
         nRowCnt = 0;
         delete pRswEqpDataWrap;   
      }
      return _T("");
   }
   pRswEqpDataWrap->MoveFirst();

   
   strLang = pRswEqpDataWrap->GetItem(_T("TO_LANG_E"));

   delete pRswEqpDataWrap;

   return strLang;
}

bool CLib::BindCombo(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, BOOL pbIsAll)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString strSql;
   CString strCCD_CD, strCCD_NM_VALUE;
   int nRowCnt = 0, j=0;
   CString strMessage;
   cbx.ResetContent();

   strSql.Format(_T(" SELECT CCD_CD, ") + pDoc->NVL + _T("(CCD_NM_KOR, ' ') AS CCD_NM_VALUE      \n")
                _T("    FROM COMMON_CODE										    \n")
                _T("   WHERE CCD_CD_YN = 'Y'										\n")
                _T("    AND CDX_CD = '%s'											\n")
                _T("    AND WH_TYP LIKE '%%%s%%'									\n")
                _T("   ORDER BY CCD_EPR_ORD											\n"), strCDX_CD, pDoc->m_WH_TYP);

   strSql = CLib::GetCommonCodeLang(strSql, (int)pDoc->m_enLang);
   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 
   if(pbIsAll == TRUE)
   {
      cbx.SetItemDataEx(0, _T("ALL"));
      cbx.AddString(_T("ALL"));
   }
   int nAll = (int)pbIsAll;

   CString strTemp = _T("");
   for(int i = 0; i < nRowCnt; i++)
   {
      strCCD_CD = pRsw->GetItem(_T("CCD_CD"));
      strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_VALUE"));

	  strTemp.Format(_T("%s:%s"), strCCD_NM_VALUE, strCCD_CD);
      cbx.SetItemDataEx(i + nAll, strCCD_CD);
      cbx.AddString(strCCD_NM_VALUE);
      //cbx.AddString(strTemp);

      pRsw->MoveNext();
   }

   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}

bool CLib::BindSpreadCommonCode(CString pCDX_CD, CString pCCD_NM_XXX, CString& pCCD_CD, CEcsDoc *pDoc)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString strSql;
   CString strCCD_CD, strCCD_NM_VALUE;
   int nRowCnt = 0, j=0;
   CString strMessage;

   switch(pDoc->m_enLang)
   {
   case EN_KOR:
	   strSql.Format(_T(" SELECT CCD_CD											        \n")
					_T("    FROM COMMON_CODE										    \n")
					_T("   WHERE CCD_CD_YN = 'Y'										\n")
					_T("     AND CDX_CD = '%s'											\n")
					_T("     AND CCD_NM_KOR = '%s'										\n")
					_T("     AND WH_TYP = '%s'											\n"), pCDX_CD, pCCD_NM_XXX, pDoc->m_WH_TYP);
   break;
   case EN_ENG:
	   strSql.Format(_T(" SELECT CCD_CD											        \n")
					_T("    FROM COMMON_CODE										    \n")
					_T("   WHERE CCD_CD_YN = 'Y'										\n")
					_T("     AND CDX_CD = '%s'											\n")
					_T("     AND CCD_NM_ENG = '%s'										\n")
					_T("     AND WH_TYP = '%s'											\n"), pCDX_CD, pCCD_NM_XXX, pDoc->m_WH_TYP);
   break;
   case EN_HUN:
	   strSql.Format(_T(" SELECT CCD_CD											        \n")
					_T("    FROM COMMON_CODE										    \n")
					_T("   WHERE CCD_CD_YN = 'Y'										\n")
					_T("     AND CDX_CD = '%s'											\n")
					_T("     AND CCD_NM_HUN = '%s'										\n")
					_T("     AND WH_TYP = '%s'											\n"), pCDX_CD, pCCD_NM_XXX, pDoc->m_WH_TYP);
   break;
   case EN_CHIN:
	   strSql.Format(_T(" SELECT CCD_CD											        \n")
					_T("    FROM COMMON_CODE										    \n")
					_T("   WHERE CCD_CD_YN = 'Y'										\n")
					_T("     AND CDX_CD = '%s'											\n")
					_T("     AND CCD_NM_CHIN = '%s'										\n")
					_T("     AND WH_TYP = '%s'											\n"), pCDX_CD, pCCD_NM_XXX, pDoc->m_WH_TYP);
   break;
   }

   strSql = CLib::GetCommonCodeLang(strSql, (int)pDoc->m_enLang);
   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 

   pCCD_CD = pRsw->GetItem(_T("CCD_CD"));

   pRsw->MoveNext();

   delete pRsw;
   return true;
}


bool CLib::BindCombo_STN_KIND(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, BOOL pbIsAll)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString strSql;
   CString strCCD_CD, strCCD_NM_VALUE;
   int nRowCnt = 0, j=0;
   CString strMessage;
   cbx.ResetContent();

   strSql.Format(_T(" SELECT MC_NO, ") + pDoc->NVL + _T("(MC_NO_NM, '') AS MC_NO_NM		        \n")
                _T("    FROM CV_DATA											    \n")
                _T("   WHERE WH_TYP = '%s'											\n")
                _T("     AND MC_NO IN (%s)											\n")
                _T("   ORDER BY MC_NO    											\n"), pDoc->m_WH_TYP, strCDX_CD);

   strSql = CLib::GetCommonCodeLang(strSql, (int)pDoc->m_enLang);
   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 
   if(pbIsAll == TRUE)
   {
      cbx.SetItemDataEx(0, _T("ALL"));
      cbx.AddString(_T("ALL"));
   }
   int nAll = (int)pbIsAll;

   for(int i = 0; i < nRowCnt; i++)
   {
      strCCD_CD = pRsw->GetItem(_T("MC_NO"));
      strCCD_NM_VALUE = pRsw->GetItem(_T("MC_NO_NM"));

      cbx.SetItemDataEx(i + nAll, strCCD_CD);
	  if (strCCD_NM_VALUE == "")
	  {
		  cbx.AddString(strCCD_CD);
	  }
	  else
	  {
		  cbx.AddString(strCCD_CD + " - " + strCCD_NM_VALUE);
	  }
      

      pRsw->MoveNext();
   }

   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}

bool CLib::BindComboPtr(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;

   CStringList strList;
   CString strSql;
   CString strCCD_CD, strCCD_NM_KOR, strCCD_NM_ENG, strCCD_NM_HUN, strCCD_NM_CHIN;
   int nRowCnt = 0, j=0;
   CString strMessage;
   strSql.Format(_T(" SELECT CCD_CD, CCD_NM_KOR, ") + pDoc->NVL + _T("(CCD_NM_ENG,CCD_NM_KOR) AS CCD_NM_ENG               \n")
      _T("    , ") + pDoc->NVL + _T("(CCD_NM_HUN,CCD_NM_KOR) AS CCD_NM_HUN, ") + pDoc->NVL + _T("(CCD_NM_CHIN,CCD_NM_KOR) AS CCD_NM_CHIN      \n")
      _T("    FROM COMMON_CODE         \n")
      _T("   WHERE CCD_CD_YN = 'Y'      \n")
      _T("    AND CDX_CD = '%s'         \n")
      _T("    AND WH_TYP LIKE '%%%s%%'   \n")
      _T("   ORDER BY CCD_EPR_ORD            \n"), strCDX_CD, pDoc->m_WH_TYP);

   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 


   wchar_t *szCCD_CD;

   for(int i = 0; i < nRowCnt; i++)
   {
      szCCD_CD = new wchar_t;

      strCCD_CD = pRsw->GetItem(_T("CCD_CD"));
      strCCD_NM_KOR = pRsw->GetItem(_T("CCD_NM_KOR"));
      strCCD_NM_ENG = pRsw->GetItem(_T("CCD_NM_ENG"));
      strCCD_NM_HUN = pRsw->GetItem(_T("CCD_NM_HUN"));
      strCCD_NM_CHIN= pRsw->GetItem(_T("CCD_NM_CHIN"));

      _tcscpy(szCCD_CD, strCCD_CD);

      if (nEN_LANG == 0)
         cbx.AddString(strCCD_NM_KOR);
      else if (nEN_LANG == 1)
         cbx.AddString(strCCD_NM_ENG);
      else if (nEN_LANG == 2)
         cbx.AddString(strCCD_NM_HUN);
      else if (nEN_LANG == 3)
         cbx.AddString(strCCD_NM_CHIN);
         
      cbx.SetItemDataPtr(j++, szCCD_CD);

      pRsw->MoveNext();
   }

   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}


bool CLib::BindCombo_ViewUsageRackDlg(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG, CString strAGING_TYP)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;

   CStringList strList;
   CString strSql;
   CString strCCD_CD, strCCD_NM_KOR, strCCD_NM_ENG, strCCD_NM_HUN, strCCD_NM_CHIN;
   CString strBank;
   int nRowCnt = 0;
   CString strMessage;
   cbx.ResetContent();
   
   strSql.Format(_T("       SELECT *                                                \n")
            _T("           FROM (                                                    \n")     
            _T("                 SELECT BANK, SUBSTRING(AGING_TYP,1,3) AS AGING_TYP            \n") // [LGLS] SUBSTR -> SUBSTRING
            _T("                   FROM CELL_MST                                     \n")
            _T("                  WHERE WH_TYP = '%s'                                 \n")
            _T("                ) CM                                             \n")
            _T("          WHERE CM.AGING_TYP = '%s'                                    \n")
            _T("       GROUP BY CM.AGING_TYP, CM.BANK                                 \n")
            _T("       ORDER BY CM.BANK                                             \n")  , pDoc->m_WH_TYP, strAGING_TYP);
   //, strCDX_CD, _T("%"), pDoc->m_WH_TYP, _T("%")
   
   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 

   for(int i = 0; i < nRowCnt; i++)
   {
      /*strCCD_CD = pRsw->GetItem(_T("CCD_CD"));
      strCCD_NM_KOR = pRsw->GetItem(_T("CCD_NM_KOR"));
      strCCD_NM_ENG = pRsw->GetItem(_T("CCD_NM_ENG"));
      strCCD_NM_HUN = pRsw->GetItem(_T("CCD_NM_HUN"));
      strCCD_NM_CHIN= pRsw->GetItem(_T("CCD_NM_CHIN"));

      if (nEN_LANG == 0)
         cbx.AddString(strCCD_NM_KOR);
      else if (nEN_LANG == 1)
         cbx.AddString(strCCD_NM_ENG);
      else if (nEN_LANG == 2)
         cbx.AddString(strCCD_NM_HUN);
      else if (nEN_LANG == 3)
         cbx.AddString(strCCD_NM_CHIN);*/
      strBank = pRsw->GetItem(_T("BANK"));
      cbx.AddString(strBank);
      cbx.SetItemDataEx(i, strBank);

      pRsw->MoveNext();
   }
   
   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}


void CLib::GetComBoBoxData(CComboBoxWrapper& cbx, CString& strVal, int nlength)
{
   int nCurSel = cbx.GetCurSel();
   if (nCurSel == -1)
   {
      cbx.GetWindowText(strVal);
      return;
   }
   
   DWORD bItemDate = cbx.GetItemData(nCurSel);
   TCHAR szBuf[10]= {0};
   switch(nlength)
   {
   case 1:
      wsprintf(szBuf,_T("%ld"), bItemDate);
   case 5:
      wsprintf(szBuf,_T("%5d"), bItemDate);
   default :
      wsprintf(szBuf,_T("%d"), bItemDate);
   }
   strVal = (LPCTSTR)szBuf;
   strVal = strVal.Trim();
  }


bool CLib::BindCombo_SC_NO(CComboBox& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, CString strAGING_TYP)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString srtAAAa;
   srtAAAa.Format(_T("%%")); 
   CString CRLF = _T("\r\n");
   CStringList strList;
   CString strSql;
   CString strCCD_CD, strCCD_NM_VALUE;
   int nRowCnt = 0, j=0;
   CString strMessage;
   cbx.ResetContent();

   /*strSql += CRLF + _T(" SELECT CM.AGING_TYP AS TAIL, CCD_CD, CCD_NM_KOR, COALESCE(CCD_NM_ENG,CCD_NM_KOR) AS CCD_NM_ENG ");
   strSql += CRLF + _T("      , COALESCE(CCD_NM_HUN,CCD_NM_KOR) AS CCD_NM_HUN, COALESCE(CCD_NM_CHIN,CCD_NM_KOR) AS CCD_NM_CHIN ");
   strSql += CRLF + _T("   FROM COMMON_CODE CC JOIN (SELECT AGING_TYP FROM CELL_MST GROUP BY AGING_TYP) CM ");
   strSql += CRLF + _T("                    ON  SUBSTR(CC.CCD_CD,3,1) = SUBSTR(CM.AGING_TYP,5,1) ");
   strSql += CRLF + _T("  WHERE CC.CCD_CD_YN = 'Y' ");
   strSql += CRLF + _T("    AND CC.CDX_CD = ") + CLib::Quot(strCDX_CD);
   strSql += CRLF + _T("    AND CC.WH_TYP LIKE ") + CLib::QuotLikeLR(pDoc->m_WH_TYP);
   if(strAGING_TYP != _T("ALL"))
   {
      strSql += CRLF + _T("    AND CM.AGING_TYP LIKE ") + CLib::QuotLikeR(strAGING_TYP);
   }
   strSql += CRLF + _T("  ORDER BY CC.CCD_EPR_ORD ");*/
   strSql += CRLF + _T(" SELECT CCD_CD, CCD_NM_KOR, ") + pDoc->NVL + _T("(CCD_NM_ENG,CCD_NM_KOR) AS CCD_NM_ENG ");
   strSql += CRLF + _T("      , ") + pDoc->NVL + _T("(CCD_NM_HUN,CCD_NM_KOR) AS CCD_NM_HUN, ") + pDoc->NVL + _T("(CCD_NM_CHIN,CCD_NM_KOR) AS CCD_NM_CHIN ");
   strSql += CRLF + _T("   FROM COMMON_CODE ");
   strSql += CRLF + _T("  WHERE CCD_CD_YN = 'Y' ");
   strSql += CRLF + _T("    AND CDX_CD = ") + CLib::Quot(strCDX_CD);
   strSql += CRLF + _T("    AND WH_TYP LIKE ") + CLib::QuotLikeLR(pDoc->m_WH_TYP);
   if(strAGING_TYP != _T("ALL"))
   {
      strSql += CRLF + _T("    AND CCD_CD LIKE ") + CLib::QuotLikeR(strAGING_TYP);
   }
   strSql += CRLF + _T("  ORDER BY CCD_EPR_ORD ");


   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 

   for(int i = 0; i < nRowCnt; i++)
   {
      strCCD_CD = pRsw->GetItem(_T("CCD_CD"));

      if (nEN_LANG == 0)
      {
         strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_KOR"));
      //   strCCD_NM_VALUE += pRsw->GetItem(_T("TAIL"));
         cbx.AddString(strCCD_NM_VALUE);
      }
      else if (nEN_LANG == 1)
      {
         strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_ENG"));
      //   strCCD_NM_VALUE += pRsw->GetItem(_T("TAIL"));
         cbx.AddString(strCCD_NM_VALUE);
      }
      else if (nEN_LANG == 2)
      {
         strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_HUN"));
      //   strCCD_NM_VALUE += pRsw->GetItem(_T("TAIL"));
         cbx.AddString(strCCD_NM_VALUE);
      }
      else if (nEN_LANG == 3)
      {
         strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_CHIN"));
      //   strCCD_NM_VALUE += pRsw->GetItem(_T("TAIL"));
         cbx.AddString(strCCD_NM_VALUE);
      }

      cbx.SetItemData(j++, CConvert::ToInt(strCCD_CD));
   
      pRsw->MoveNext();
   }
   
   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}


bool CLib::BindCombo_CELL_SC_NO(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, CString strAGING_TYP, CString strSC_NO)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString srtAAAa;
   srtAAAa.Format(_T("%%")); 
   CStringList strList;
   CString strSql;
   CString strCCD_CD, strCCD_NM_VALUE;
   int nRowCnt = 0, j=0;
   CString strMessage;
   cbx.ResetContent();


   strSql.Format(_T("    SELECT CCD_CD, CCD_NM_KOR, ") + pDoc->NVL + _T("(CCD_NM_ENG,CCD_NM_KOR) AS CCD_NM_ENG                         \n")
      _T("                 , ") + pDoc->NVL + _T("(CCD_NM_HUN,CCD_NM_KOR) AS CCD_NM_HUN, ") + pDoc->NVL + _T("(CCD_NM_CHIN,CCD_NM_KOR) AS CCD_NM_CHIN               \n")
      _T("              FROM COMMON_CODE CC                                                                    \n")
      _T("             WHERE CC.CCD_CD_YN = 'Y'                                                               \n")
      _T("               AND CC.CDX_CD = '%s'                                                                  \n")
      _T("               AND CC.WH_TYP LIKE '%s%%'                                                               \n")
      _T("               AND CC.CCD_CD LIKE '%s%%'                                                           \n")
      _T("           ORDER BY CC.CCD_EPR_ORD                                                                         \n")
      , strCDX_CD, pDoc->m_WH_TYP, strAGING_TYP);

   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 
   cbx.SetItemDataEx(0, 0);
   cbx.AddString(_T("ALL"));
   for(int i = 0; i < nRowCnt; i++)
   {
      strCCD_CD = pRsw->GetItem(_T("CCD_CD"));

      if (nEN_LANG == 0)
         strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_KOR"));
      else if (nEN_LANG == 1)
         strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_ENG"));
      else if (nEN_LANG == 2)
         strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_HUN"));
      else if (nEN_LANG == 3)
         strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_CHIN"));

      cbx.AddString(strCCD_NM_VALUE);
      cbx.SetItemDataEx(i + 1, strCCD_CD);

      pRsw->MoveNext();
   }

   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}

// DWORD CLib::GetCStringToDWORD(CString pStr)
// {
//    DWORD var;
//    sscanf(pStr, _T("%u"), &var);
//    return var;
// }
// 
// CString CLib::GetCStringToDWORD_PTR(CString pStr)
// {
// 
// }

bool CLib::BindCombo_SC_HS_DEF(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG, CString strSC_NO, CString strJOB_TYP)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString strSql;
   CString strHS_NO;
   CString strHS_MC_NO;
   CString strMC_NO_NM;
   CString strPOS;
   int nRowCnt = 0, j = 0;
   CString strMessage;
   cbx.ResetContent();
   
    strSql.Format(_T(" SELECT SHD.HS_NO	AS HS_NO																				 \n")
		_T("                 ,SHD.HS_MC_NO	AS HS_MC_NO																			 \n")
		_T("                 ,CD.MC_NO_NM AS MC_NO_NM																			 \n")
       _T("           FROM SC_HS_DEF SHD																						 \n")
	   _T("           LEFT OUTER JOIN CV_DATA CD																				 \n")
	   _T("                        ON SHD.HS_MC_NO = CD.MC_NO																	 \n")
       _T("          WHERE SHD.WH_TYP = '%s'																					 \n")
        _T("            AND SHD.SC_NO = '%s'																					 \n")
		_T("            AND CASE WHEN '%s' = '1' THEN SHD.HS_NO IN ('01','03')													 \n")
		_T("            		 WHEN '%s' = '2' THEN SHD.HS_NO IN ('02','04','05','06','07','08','09','10','11')                \n")
		_T("            						  ELSE 1 = 1 END																 \n")
        _T("         ORDER BY SHD.HS_NO          \n"), pDoc->m_WH_TYP, strSC_NO, strJOB_TYP, strJOB_TYP);

   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 

   for(int i = 0; i < nRowCnt; i++)
   {
      strHS_NO = pRsw->GetItem(_T("HS_NO"));
	  strHS_MC_NO = pRsw->GetItem(_T("HS_MC_NO"));
	  strMC_NO_NM = pRsw->GetItem(_T("MC_NO_NM"));
	  strPOS = strHS_MC_NO + _T(" (") + strMC_NO_NM + _T(")");
      
      cbx.AddString(strPOS);
	  cbx.SetItemDataEx(j++, strHS_NO);

      pRsw->MoveNext();
   }
   
   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}

bool CLib::BindCombo_SC_HS_DEF_DEST_SPREAD(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG, CString strSC_NO)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString strSql;
   CString strHS_NO;
   int nFind = 0;
   int nRowCnt = 0, j = 0;
   CString strMessage;
   CString CRLF = _T("\r\n");
   cbx.ResetContent();

   strSql = _T("");
   strSql += CRLF + _T("");
   strSql += CRLF + _T(" SELECT ") + pDoc->NVL + _T("(DEST_DEF_DAT, '0,') AS DEST_DEF_DAT");
   strSql += CRLF + _T("   FROM SC_HS_DEF SHD ");
   strSql += CRLF + _T("  WHERE WH_TYP = ") + CLib::Quot(pDoc->m_WH_TYP);
   strSql += CRLF + _T("    AND SC_NO = ( SELECT CCD_CD FROM COMMON_CODE WHERE CDX_CD = 'CELL_SC_NO' AND WH_TYP LIKE '%' + ");	// [LGLS] || -> +
   strSql += CRLF + _T("                                                                                                ") + CLib::Quot(pDoc->m_WH_TYP);
   strSql += CRLF + _T("                                                                                                           + '%'");	// [LGLS] || -> +
   strSql += CRLF + _T("                                                                             AND CCD_NM_KOR = ") + CLib::Quot(strSC_NO);
#if ORACLE
   strSql += CRLF + _T("                                                                             AND ROWNUM = 1 )");
#endif
#if POSTGRESQL
   strSql += CRLF + _T("                                                                             LIMIT 1 )");
#elif MSSQL
   strSql += CRLF + _T("                                                                             )");	// [LGLS]
   strSql.Replace(_T("( SELECT CCD_CD FROM COMMON_CODE"), _T("( SELECT TOP 1 CCD_CD FROM COMMON_CODE"));	// [LGLS] TOP 1 instead of LIMIT 1
#endif
   strSql += CRLF + _T("    AND HS_NO IN ('03', '06')");

   strSql = CLib::GetCommonCodeLang(strSql, (int)pDoc->m_enLang);

   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 

   for(int i = 0; i < nRowCnt; i++)
   {
      strHS_NO = pRsw->GetItem(_T("DEST_DEF_DAT"));
      int nFind = strHS_NO.Replace(_T(","), _T(""));
      for(int nIdx = 0; nIdx < nFind; nIdx++)
      {
         cbx.AddString(strHS_NO.Mid(nIdx * 5, 5));
         cbx.SetItemDataEx(nIdx, strHS_NO.Mid(nIdx * 5, 5));
      }
      pRsw->MoveNext();
   }

   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}


bool CLib::BindCombo_SC_HS_DEF_DEST(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG, CString strSC_NO)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString strSql;
   CString strHS_NO;
   int nFind = 0;
   int nRowCnt = 0, j = 0;
   CString strMessage;
   CString strHS_NO_SQL;
   cbx.ResetContent();
   

   if (pDoc->m_WH_TYP == _T("10"))
   {
      strHS_NO_SQL = _T("'03', '06'");
   }
   else
   {
      strHS_NO_SQL = _T("'02', '04'");
   }
    strSql.Format(_T(" SELECT *                  \n")
        _T("           FROM SC_HS_DEF            \n")
        _T("          WHERE WH_TYP = '%s'         \n")
        _T("            AND SC_NO = '%s'              \n")
        _T("            AND HS_NO IN (%s)              \n")
        _T("         ORDER BY SC_NO                 \n"), pDoc->m_WH_TYP, strSC_NO, strHS_NO_SQL);


   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

   pRsw->MoveFirst(); 

   for(int i = 0; i < nRowCnt; i++)
   {
      strHS_NO = pRsw->GetItem(_T("DEST_DEF_DAT"));
      int nFind = strHS_NO.Replace(_T(","), _T(""));
      for(int nIdx = 0; nIdx < nFind; nIdx++)
      {
         cbx.AddString(strHS_NO.Mid(nIdx * 5, 5));
         cbx.SetItemDataEx(nIdx, strHS_NO.Mid(nIdx * 5, 5));
      }
      pRsw->MoveNext();
   }
   
   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}

void CLib::GetColumnName(CStringArray& pArrColName, CString pString)
{
   int nTempS,nTempE;
   CString strSql, strTemp;
   strSql = pString;
   strSql.Replace(_T("\t"), _T(""));
   
   nTempS = strSql.Find(_T("SELECT"));
   nTempE = strSql.Find(_T("FROM"));
   strSql = strSql.Mid(nTempS+6,nTempE-nTempS-6); //조회조건만 남기고 자르기
      
   while (true)
   {
      nTempS = strSql.Find(_T(" AS "));
      if (nTempS < 0)
      {
         break;
      }
      strSql = strSql.Right(strSql.GetLength()-nTempS-4);
      nTempE = strSql.Find(_T(","));
      if (nTempE < 0)
         nTempE = strSql.GetLength();
      strTemp = strSql.Left(nTempE); //조회조건 자르기
      strSql = strSql.Right(strSql.GetLength()-nTempE-1); //자른 조회조건 버리기 남기고 자르기

      strTemp = strTemp.Trim();
      pArrColName.Add(strTemp);//1

   }
}

CString CLib::QuotLikeLR( CString pStr )
{
   CString strTemp = _T("");
   strTemp.Format(_T("%c%%%s%%%c"), '\'', pStr, '\'');
   return strTemp;
}

CString CLib::QuotLikeL( CString pStr )
{
   CString strTemp = _T("");
   strTemp.Format(_T("%c%%%s%c"), '\'', pStr, '\'');
   return strTemp;
}

CString CLib::QuotLikeR( CString pStr )
{
   CString strTemp = _T("");
   strTemp.Format(_T("%c%s%%%c"), '\'', pStr, '\'');
   return strTemp;
}

void CLib::CellMasking( CString& pstrCELL_NO_TO )
{
   if(pstrCELL_NO_TO.GetLength() < 7)
      return;

   pstrCELL_NO_TO.Format(_T("%02s-%03s-%02s"), pstrCELL_NO_TO.Left(2), pstrCELL_NO_TO.Mid(2,3), pstrCELL_NO_TO.Right(2));
}

CString CLib::GetCommonCodeLang( CString pstrSql, int pnLang )
{
   if(pnLang == 0)
   {
      return pstrSql;
   }

   if(pnLang == 1)
   {
      pstrSql.Replace( _T("CCD_NM_KOR"), _T("CCD_NM_ENG") ) ;
      pstrSql.Replace( _T("MSG_KOR"), _T("MSG_ENG") ) ;
      return pstrSql;
   }

   if(pnLang == 2)
   {
      pstrSql.Replace( _T("CCD_NM_KOR"), _T("CCD_NM_HUN") ) ;
      pstrSql.Replace( _T("MSG_KOR"), _T("MSG_HUN") ) ;
      return pstrSql;
   }

   if(pnLang == 3)
   {
      pstrSql.Replace( _T("CCD_NM_KOR"), _T("CCD_NM_CHIN") ) ;
      pstrSql.Replace( _T("MSG_KOR"), _T("MSG_CHIN") ) ;
      return pstrSql;
   }

   return pstrSql;
}

BOOL CLib::BindCombo_SC_NO_MANUAL(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, CString strSC_GRP_NO, CString strSC_NO)
{
   if(pDoc   == NULL)                     return false;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString srtAAAa;
   srtAAAa.Format(_T("%%")); 
   CStringList strList;
   CString strSql;
   CString CRLF = _T("\r\n");
   CString strCCD_CD, strCCD_NM_VALUE;
   int nRowCnt = 0, j=0;
   CString strMessage;
   cbx.ResetContent();

   strSql += CRLF + _T(" SELECT ") + pDoc->NVL + _T("(CCD.CCD_NM_KOR, '') AS CELL_SC_NO ");
   strSql += CRLF + _T("   FROM SC_DATA SD LEFT OUTER JOIN COMMON_CODE CCD ");
   strSql += CRLF + _T("                                ON CCD.CDX_CD = 'CELL_SC_NO' ");
   strSql += CRLF + _T("                               AND CCD.CCD_CD = SD.SC_NO ");
   strSql += CRLF + _T("                               AND CCD.WH_TYP LIKE '30%' ");
   strSql += CRLF + _T("  WHERE SD.WH_TYP = ") + CLib::Quot(pDoc->m_WH_TYP);
   strSql += CRLF + _T("    AND SD.SC_GRP_NO = ") + CLib::Quot(strSC_GRP_NO);
   strSql = CLib::GetCommonCodeLang(strSql, (int)pDoc->m_enLang);

   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
   if(nRowCnt < 0)
   {
      cbx.SetCurSel(0);
      delete pRsw;
      return FALSE;
   }

   pRsw->MoveFirst(); 
   cbx.SetItemDataEx(0, 0);
   cbx.AddString(_T("ALL"));
   for(int i = 0; i < nRowCnt; i++)
   {
      strCCD_NM_VALUE = pRsw->GetItem(_T("CELL_SC_NO"));
      cbx.AddString(strCCD_NM_VALUE);
      cbx.SetItemDataEx(i + 1, strCCD_NM_VALUE);

      pRsw->MoveNext();
   }

   cbx.SetCurSel(0);
   delete pRsw;
   return true;
}

BOOL CLib::BindCombo_RTV_NO(CComboBoxWrapper& cbx, CEcsDoc *pDoc, int nEN_LANG)
{
   if(pDoc   == NULL)                     return FALSE;
   if(pDoc->IsConnectDB() == FALSE) return FALSE;
   CString srtAAAa;
   srtAAAa.Format(_T("%%")); 
   CStringList strList;
   CString strSql;
   CString CRLF = _T("\r\n");
   CString strRTV_NO, strCCD_NM_VALUE;
   int nRowCnt = 0, j=0;
   CString strMessage;
   cbx.ResetContent();

   strSql = CRLF + _T("  SELECT RTV_NO, ") + pDoc->NVL + _T("(CCD.CCD_NM_CHIN, '') AS RTV_NM");
   strSql += CRLF + _T("   FROM RTV_DATA RD LEFT OUTER JOIN COMMON_CODE CCD ");
   strSql += CRLF + _T("                                ON CCD.CDX_CD = 'RTV_NO' ");
   strSql += CRLF + _T("                               AND CCD.CCD_CD = RD.RTV_NO ");
   strSql += CRLF + _T("                               AND CCD.WH_TYP LIKE '%") + pDoc->m_WH_TYP + _T("%'");
   strSql += CRLF + _T("  WHERE RD.WH_TYP = ") + CLib::Quot(pDoc->m_WH_TYP);
   strSql = CLib::GetCommonCodeLang(strSql, (int)pDoc->m_enLang);

   _RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
   CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
   if(nRowCnt < 0)
   {
      cbx.SetCurSel(0);
      delete pRsw;
      return FALSE;
   }

   pRsw->MoveFirst(); 
   //cbx.SetItemDataEx(0, 0);
   //cbx.AddString(_T("ALL"));
   for(int i = 0; i < nRowCnt; i++)
   {
      strCCD_NM_VALUE = pRsw->GetItem(_T("RTV_NM"));
      strRTV_NO = pRsw->GetItem(_T("RTV_NO"));
      cbx.AddString(strCCD_NM_VALUE);
      cbx.SetItemDataEx(i, strRTV_NO);

      pRsw->MoveNext();
   }

   cbx.SetCurSel(0);
   delete pRsw;
   return TRUE;
}

void CLib::RenameColumn(CStringArray& pColArray, int pLang, CStringArray& pColReName)
{
   int nFor = 0;
   TCHAR chrFileName[500];
   GetModuleFileName(NULL, chrFileName, MAX_PATH);
   CString strAppPath = _T("");
   CString strTemp;
   strAppPath.Format(_T("%s"),chrFileName);
   CString strExtension = _T(".ini");

   CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\column_name\\"), _T("column_name"), strExtension);
   
   for (nFor = 0; nFor < pColArray.GetSize(); nFor++)
   {
      strTemp = CLib::GetIniStringFromPath(strFullPath, pColArray[nFor], pLang);
      
      if (strTemp == _T(""))
      {
         pColReName.Add(pColArray[nFor]);
      }
      else
      {
         pColReName.Add(strTemp);
      }
   }
}

bool CLib::SetBindCombo_DEST_POS_DEF(CComboBoxWrapper& cbx, CEcsDoc *pDoc)
{
	if (pDoc   == NULL)                     
		return false;

    if (pDoc->IsConnectDB() == FALSE) 
		return FALSE;

	CStringList strList;
	CString strSql;
	CString strTRACK_NO, strREMARKS, strMC_NO;
	int nRowCnt = 0, j=0;
	CString strDEST_POS;
	CString strMessage;
	cbx.ResetContent();
	strSql.Format(_T("  SELECT TRACK_NO						")
				  _T("       , REMARKS						") 
				  _T("       , GROUP_NO						")
				  _T("       , MC_NO						")
				  _T("	  FROM DEST_POS_DEF					")
				  _T("ORDER BY GROUP_NO, MC_NO				"));

	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	
	cbx.SetItemDataEx(0, _T("ALL"));
	cbx.AddString(_T("ALL"));

	pRsw->MoveFirst(); 
	
	for(int i = 1; i <= nRowCnt; i++)
	{
		strTRACK_NO = pRsw->GetItem(_T("MC_NO")) + _T(" (") + pRsw->GetItem(_T("REMARKS")) + _T(")");
		strMC_NO = pRsw->GetItem(_T("MC_NO"));
		cbx.SetItemDataEx(i, strMC_NO);
		cbx.AddString(strTRACK_NO);

		pRsw->MoveNext();
	}
	cbx.SetCurSel(0);
	delete pRsw;
	return true;
}