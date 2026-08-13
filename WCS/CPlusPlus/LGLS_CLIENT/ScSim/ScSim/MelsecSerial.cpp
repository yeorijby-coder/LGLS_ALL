// MelsecSerialSk.cpp: implementation of the CMelsecSerialSk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "MelsecSerial.h"
#include "Equipment.h"
#include "ByteArrayEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_DYNAMIC(CMelsecSerial, CSerial)

CMelsecSerial::CMelsecSerial(CEquipment* pEquipment, int nIndex) : CSerial(pEquipment, nIndex)
{

}

CMelsecSerial::~CMelsecSerial()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMelsecSerial::Connect(CString strComName, DWORD dwReadTimeOut,
	DWORD dwBaudRate,
	BYTE byByteSize,
	BYTE byParity,
	BYTE byStopBits,
	BYTE byFlowCtrl)
{
	//InitSerial();
	SetConfig(strComName, dwBaudRate, byByteSize, byParity, byStopBits, byFlowCtrl, dwReadTimeOut);
	return Open();
}

void CMelsecSerial::CheckSum(BYTE* pBuff, int nLen, BYTE* pSumA, BYTE* pSumB)
{
	int     SumA, SumB, Check = 0;

	for (int i = 1; i < nLen; i++) Check += pBuff[i] & 0x00ff;

	SumA = Check & 0x00ff;
	SumA = SumA >> 4;
	SumB = Check & 0x000f;

	if (SumA <= 0x09)
		SumA = SumA + 0x30;
	else {
		SumA = SumA - 0x09;
		SumA = SumA + 0x40;
	}
	if (SumB <= 0x09)
		SumB = SumB + 0x30;
	else {
		SumB = SumB - 0x09;
		SumB = SumB + 0x40;
	}

	*pSumA = (BYTE)SumA;
	*pSumB = (BYTE)SumB;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMelsecSerial::CheckRequest(CByteArray& arrRxData)
{
	CByteArray arrBuffer;
	int nWordCnt = 16;

	int nByteCnt = 23;	// 19;	// 읽기는 19인데 쓰기는 23임 => 하지만 LF까지 무조건 읽어오기 때문에 이렇게 해도 될듯
	BYTE pRxBuff[1024];
	memset(pRxBuff, 0x00, 1024);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//BOOL CMelsecSerial::RecvData(BYTE * pRxData, CString & strData, int& nReadCnt)
	CString strFrame;
	BOOL bTemp = RecvData(pRxBuff, strFrame, nByteCnt);		// nByteCnt : 함수안에서 다시 세팅됨 - 읽은 Count로 ...
	if (bTemp == FALSE)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int nRecvLen = nByteCnt; // arrBuffer.GetSize();
	int nTotalLen = 19;

	strFrame = CString(pRxBuff);

	//nRecvLen = strFrame.GetLength();
	CString strTemp = arrBuffer.GetData();
	if (nRecvLen < nTotalLen)
	{
		m_strLog.Format(_T("응답 프레임 길이 이상! [예상=%d, 실제=%d]"), nTotalLen, nRecvLen);
		return FALSE;
	}

	if (pRxBuff[0] != enENQ)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 응답 요구 이상"));
		return FALSE;
	}

	if (pRxBuff[1] != '0' ||
		pRxBuff[2] != '0' ||
		pRxBuff[3] != 'F' ||
		pRxBuff[4] != 'F')
	{
		m_strLog.Format(_T("응답 프레임 Header 내용 이상1"));
		return FALSE;
	}

	BYTE bTemp1  = pRxBuff[1];	// arrBuffer[1];
	BYTE bTemp2  = pRxBuff[2];	// arrBuffer[2];
	BYTE bTemp3  = pRxBuff[3];	// arrBuffer[3];
	BYTE bTemp4  = pRxBuff[4];	// arrBuffer[4];
	BYTE bTemp5  = pRxBuff[5];	// arrBuffer[5];
	BYTE bTemp6  = pRxBuff[6];	// arrBuffer[6];
	BYTE bTemp7  = pRxBuff[7];	// arrBuffer[7];
	BYTE bTemp8  = pRxBuff[8];	// arrBuffer[8];
	BYTE bTemp9  = pRxBuff[9];	// arrBuffer[9];
	BYTE bTemp10 = pRxBuff[10];	// arrBuffer[10];
	BYTE bTemp11 = pRxBuff[11];	// arrBuffer[11];
	BYTE bTemp12 = pRxBuff[12];	// arrBuffer[12];
	BYTE bTemp13 = pRxBuff[13];	// arrBuffer[13];
	BYTE bTemp14 = pRxBuff[14];	// arrBuffer[14];
	BYTE bTemp15 = pRxBuff[15];	// arrBuffer[15];
	BYTE bTemp16 = pRxBuff[16];	// arrBuffer[16];
	BYTE bTemp17 = pRxBuff[17];	// arrBuffer[17];
	BYTE bTemp18 = pRxBuff[18];	// arrBuffer[18];

	int bStartaddress1 = ((bTemp9 - 0x30) * 1000);
	int bStartaddress2 = ((bTemp10 - 0x30) * 100);
	int bStartaddress3 = ((bTemp11 - 0x30) * 10);
	int bStartaddress4 = ((bTemp12 - 0x30) * 1);

	int bLen11 = (CLib::CharToDec(bTemp13) * 16);
	int bLen12 = (CLib::CharToDec(bTemp14) * 1);

	int nStartaddress = bStartaddress1 + bStartaddress2 + bStartaddress3 + bStartaddress4;
	int nWordLen = bLen11 + bLen12;


	BYTE ucSumA, ucSumB;
	ucSumA = 0x00;
	ucSumB = 0x00;
	if (pRxBuff[5] == 'W' && pRxBuff[6] == 'R')
	{
		if (pRxBuff[5] != 'W' ||
			pRxBuff[6] != 'R' ||
			pRxBuff[7] != '0' ||
			pRxBuff[8] != 'D')
		{
			m_strLog.Format(_T("응답 프레임 Header 내용 이상2"));
			return FALSE;
		}

		CheckSum(pRxBuff, 15, &ucSumA, &ucSumB);
		//void CMelsecSerial::CheckSum(BYTE * pBuff, int nLen, BYTE * pSumA, int * pSumB)

		//if ((arrBuffer[15] != ucSumA) ||
		//	(arrBuffer[16] != ucSumB) ||
		//	(arrBuffer[17] != enCR)   ||
		//	(arrBuffer[18] != enLF)   )
		if ((bTemp15 != ucSumA) ||
			(bTemp16 != ucSumB) ||
			(bTemp17 != enCR) ||
			(bTemp18 != enLF))
		{
			BOOL boolTemp1 = (bTemp15 != ucSumA);
			BOOL boolTemp2 = (bTemp16 != ucSumB);
			BOOL boolTemp3 = (bTemp17 != enCR);
			BOOL boolTemp4 = (bTemp18 != enLF);

			m_strLog.Format(_T("Check Sum 이상"));
			return FALSE;
		}

		arrRxData.SetSize(nRecvLen);
		int nSize = arrRxData.GetSize();
		for (int i = 0; i < nSize; ++i)
			arrRxData[i] = pRxBuff[i];

	}
	else //if (pRxBuff[5] == 'W' && pRxBuff[6] == 'W')
	{
		if (pRxBuff[5] != 'W' ||
			pRxBuff[6] != 'W' ||
			pRxBuff[7] != '0' ||
			pRxBuff[8] != 'D')
		{
			m_strLog.Format(_T("응답 프레임 Header 내용 이상2"));
			return FALSE;
		}

		//int nLen = (nWordLen * 4) + 10; // SumA+SumB+ETX+CR+LF
		int nByteCnt2 = nWordLen * 4;	// 일단 이걸로 정함!
		BYTE pRxBuffBuff[1024];
		memset(pRxBuffBuff, 0x00, nByteCnt2);

		if (nRecvLen < 23)
		{
			nByteCnt2 = 23 - nRecvLen;
			BOOL bTempTemp = RecvData(pRxBuffBuff, strFrame, nByteCnt2);		// nByteCnt : 함수안에서 다시 세팅됨 - 읽은 Count로 ...
			if (bTempTemp == FALSE)
			{
				return FALSE;
			}
			memcpy(pRxBuff + nTotalLen, pRxBuffBuff, nWordLen * 4);

		}


		CheckSum(pRxBuff, 15 + (nWordLen * 4), &ucSumA, &ucSumB);

		int nCnt = 15 + 0 + (nWordLen * 4);
		bTemp15 = pRxBuff[nCnt + 0];	// arrBuffer[15];
		bTemp16 = pRxBuff[nCnt + 1];	// arrBuffer[16];
		bTemp17 = pRxBuff[nCnt + 2];	// arrBuffer[17];
		bTemp18 = pRxBuff[nCnt + 3];	// arrBuffer[18];

		if ((bTemp15 != ucSumA) ||
			(bTemp16 != ucSumB) ||
			(bTemp17 != enCR)	||
			(bTemp18 != enLF)   )
		{
			BOOL boolTemp1 = (bTemp15 != ucSumA);
			BOOL boolTemp2 = (bTemp16 != ucSumB);
			BOOL boolTemp3 = (bTemp17 != enCR);
			BOOL boolTemp4 = (bTemp18 != enLF);

			m_strLog.Format(_T("Check Sum 이상"));
			return FALSE;
		}

		
		arrRxData.SetSize(nTotalLen + (nWordLen * 4));		

		////// 실제 값에 입력하기 => 밖에서 
		int nSize = arrRxData.GetSize();
		for (int i = 0; i < nSize; ++i)
			arrRxData[i] = pRxBuff[i];
	}


	return TRUE;
}

BOOL CMelsecSerial::ResponseAck()
{
	CByteArray arrBuffer;
	arrBuffer.SetSize(7);
	arrBuffer[0] = enACK;
	arrBuffer[1] = '0';
	arrBuffer[2] = '0';
	arrBuffer[3] = 'F';
	arrBuffer[4] = 'F';
	arrBuffer[5] = enCR;
	arrBuffer[6] = enLF;

	if (!SendData(arrBuffer))
		return FALSE;

	return TRUE;
}

BOOL CMelsecSerial::ResponseNak()
{
	CByteArray arrBuffer;
	arrBuffer.SetSize(7);
	arrBuffer[0] = enNAK;
	arrBuffer[1] = '0';
	arrBuffer[2] = '0';
	arrBuffer[3] = 'F';
	arrBuffer[4] = 'F';
	arrBuffer[5] = enCR;
	arrBuffer[6] = enLF;
	
	if (!SendData(arrBuffer))
		return FALSE;

//	if (!RecvData(arrBuffer))
//		return FALSE;

	return TRUE;
}

void CMelsecSerial::CheckSum(CByteArray& arrData, int nLen, BYTE& rSumA, BYTE& rSumB)
{
	int nCheck = 0;
	for (int i=1; i<nLen; ++i)			// 1부터 시작!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		nCheck += arrData[i] & 0x00FF;

	int nSumA, nSumB;
	nSumA = nCheck & 0x00FF;
	nSumA = nSumA >> 4;
	nSumB = nCheck & 0x000F;

    if (nSumA <= 0x09)
	{
        nSumA = nSumA + 0x30;
	}
    else 
	{
        nSumA = nSumA - 0x09;
        nSumA = nSumA + 0x40;
    }

    if (nSumB <= 0x09)
	{
        nSumB = nSumB + 0x30;
	}
    else 
	{
        nSumB = nSumB - 0x09;
        nSumB = nSumB + 0x40;
    }

	rSumA = (BYTE)nSumA;
	rSumB = (BYTE)nSumB;
}

BOOL CMelsecSerial::ResponseReadWord(CByteArray& arrTxData, int nLen)
{
	int nSize = nLen * 4;
	CByteArray arrBuffer;
	arrBuffer.SetSize(10 + nSize);		

	arrBuffer[0] = enSTX;
	arrBuffer[1] = '0';
	arrBuffer[2] = '0';
	arrBuffer[3] = 'F';
	arrBuffer[4] = 'F';

	CString strTemp1, strTemp2, strTemp3, strTemp4;		// 확인용

	CString strTemp = CString(arrBuffer.GetData());
	strTemp3 = CString(arrTxData.GetData());
	BYTE bTemp = 0x00;
	//for (int i = 0, j = 0 ; i < nSize ; i = i + 2, j++)		// 60
	for (int i = 0; i < nLen * 2; ++i)							// 30
	{
		int nIndex = 5 + (i * 2) + 0;

		bTemp = CLib::DecToChar((arrTxData[i]) & 0x0F);
		arrBuffer[nIndex + 1] = bTemp;				// 5 + (i * 2) + 0	
		//strTemp1.Format(_T("%c"), bTemp);
		//strTemp += strTemp1;
		strTemp = CString(arrBuffer.GetData());

		bTemp = CLib::DecToChar((arrTxData[i] >> 4) & 0x0F);
		arrBuffer[nIndex] = bTemp;				// 5 + (i * 2) + 1
		//strTemp1.Format(_T("%c"), bTemp);
		//strTemp += strTemp1;
		strTemp = CString(arrBuffer.GetData());
	}
	BYTE * pArray = arrBuffer.GetData();

	strTemp2 = CString(pArray);		bTemp = arrBuffer.GetAt(nSize + 5);

	arrBuffer[nSize + 5] = enETX;	strTemp2 = CString(pArray);		bTemp = arrBuffer.GetAt(nSize + 6);

	BYTE ucSumA, ucSumB;
	CheckSum(arrBuffer, nSize + 6, ucSumA, ucSumB);
	arrBuffer[nSize + 6] = ucSumA;	strTemp2 = CString(pArray);
	arrBuffer[nSize + 7] = ucSumB;	strTemp2 = CString(pArray);
	arrBuffer[nSize + 8] = enCR;	strTemp2 = CString(pArray);
	arrBuffer[nSize + 9] = enLF;	strTemp2 = CString(pArray);

	pArray = arrBuffer.GetData();

	strTemp2 = CString(arrBuffer.GetData());

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//arrBuffer.Append(arrTxData);
	if (!SendData(arrBuffer))
		return FALSE;

	return TRUE;
}

BOOL CMelsecSerial::ResponseWriteWord(CByteArray& arrTxData, int nLen)
{
	CByteArray arrBuffer;
	arrBuffer.SetSize(5);
	arrBuffer[0] = enACK;
	arrBuffer[1] = '0';
	arrBuffer[2] = '0';
//	arrBuffer[3] = 'F';
//	arrBuffer[4] = 'F';
	arrBuffer[3] = enCR;
	arrBuffer[4] = enLF;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//arrBuffer.Append(arrTxData);
	if (!SendData(arrBuffer))
		return FALSE;

	return TRUE;
}

BOOL CMelsecSerial::RecvData(CString& strData)
{
	return TRUE;
}

BOOL CMelsecSerial::RecvData(BYTE* pRxData, CString& strData, int &nReadCnt)
{
	int nWordCnt = 16;
	int    nLen = (nWordCnt * 4) + 10; // SumA+SumB+ETX+CR+LF

	int nByteCnt = nReadCnt;	// nLen;

	BYTE pRxBuff[1024];
	memset(pRxBuff, 0x00, 1024);
	int nCount = 0;
	DWORD* pActualLen = 0;
	//if (Recv(pRxBuff, nByteCnt) == FALSE)
	if (RecvUntilLF(pRxBuff, nByteCnt, pActualLen) == FALSE)
	{
		BYTE	DummyBuf[1024];

		memset(DummyBuf, 0x00, 1024);

		return FALSE;
	}

	strData = CString(pRxBuff);
	nCount = strData.GetLength();

	if (nCount > 0)
	{
		nReadCnt = nCount;
		memcpy(pRxData, pRxBuff, nByteCnt);

		return	TRUE;
	}
	return FALSE;
}


BOOL CMelsecSerial::RecvData(CByteArray& arrRxBuff)
{
	int nWordCnt = 16;
	int    nLen = (nWordCnt * 4) + 10; // SumA+SumB+ETX+CR+LF

	int nByteCnt = 19;	// nLen;
	//BYTE * 	pRxBuff;
	BYTE pRxBuff[19];
	memset(pRxBuff, 0x00, nByteCnt);

	if (Recv(arrRxBuff.GetData(), nByteCnt) == TRUE)
	{
		int nDummy = 0;

		return	TRUE;
	}

	//BYTE	DummyBuf[1024];
	////BYTE* DummyBuf;
	//memset(DummyBuf, 0x00, 1024);
	//Recv(DummyBuf, 1024);

	CByteArrayEx DummyBuf(99, 1024);
	BOOL bTemp = Recv(DummyBuf.GetData(), 1024);
	return FALSE;
}

BOOL CMelsecSerial::SendData(const CString& strData)
{
	return TRUE;
}

BOOL CMelsecSerial::SendData(CByteArray& arrTxBuff)
{
	int nLen = arrTxBuff.GetSize();
	
	return Send(arrTxBuff.GetData(), nLen);
}

void CMelsecSerial::WriteLog(LPCTSTR lpszLog)
{
	/*
	if (!m_bWriteLog)
		return;

	m_csSyncSocketLog.Lock();

	CString strFileName;
	strFileName.Format(_T("%s%s.LOG"), COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")), m_strName);

	TRY
	{
		CFile f(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
		f.SeekToEnd();

		CArchive ar(&f, CArchive::store);
		ar.WriteString(lpszLog);
		ar.Close();

		f.Close();
	}
		CATCH(CException, e)
	{
		TCHAR szTemp[512] = { 0 };
		e->GetErrorMessage(szTemp, 512);
		m_strLog.Format(_T("%s %s [%s] [%s]"), m_strName, e->GetRuntimeClass()->m_lpszClassName, szTemp, lpszLog);
		DEBUGER_TRACE(m_strLog, _T("CAsyncSocketEx::WriteLog"));
		m_csSyncSocketLog.Unlock();
	}
	END_CATCH

		m_csSyncSocketLog.Unlock();
	//*/
}

CString CMelsecSerial::GetHexString(CByteArray& arrBuffer)
{
	CString strBuffer, strHex;
	for (int i = 0; i < arrBuffer.GetSize(); ++i)
	{
		strHex.Format(_T("%02X "), arrBuffer[i]);
		strBuffer += strHex;
	}

	return strBuffer;
}	
BOOL CMelsecSerial::Write(const BYTE* pTxBuff, int BaseAddr, int nWordCnt, int Type)
{
    BYTE TxBuff[256], SumA, SumB;
    char szStartAddr[6], szRange[6];
    m_strErrMsg = "";
    memset(TxBuff, 0x0, sizeof(TxBuff));

    if(Type == 'D')
        sprintf(szStartAddr, "D%.4d", BaseAddr);
    else
        sprintf(szStartAddr, "M%.4d", BaseAddr);

    sprintf(szRange, "%.2X", nWordCnt);

    TxBuff[0]  = enENQ;
    if (Type == 'D')
        memcpy(TxBuff+1, "00FFWW0", 7);
    else
        memcpy(TxBuff+1, "00FFBW0", 7);

    memcpy(TxBuff+8, szStartAddr, 5);
    memcpy(TxBuff+13, szRange, 2);

    if (Type == 'D')
    {
        memcpy(TxBuff+15, pTxBuff, nWordCnt * 4);
        CheckSum(TxBuff, 15+nWordCnt*4, &SumA, &SumB);
        TxBuff[15 + nWordCnt*4] = SumA;
        TxBuff[15 + 1 + nWordCnt*4] = SumB;
        TxBuff[15 + 2 + nWordCnt*4] = enCR;
        TxBuff[15 + 3 + nWordCnt*4] = enLF;
        // 송신 후 ACK(0x06) 수신까지 확인
        if (!SendUntilAck(TxBuff, 15 + 4 + nWordCnt*4, enACK, 1000))
        {
            m_strErrMsg.Format(_T("CMelsecSerial::Write 에러=[%s]"), m_strErrMsg);
            return FALSE;
        }
    }
    else
    {
        memcpy(TxBuff+15, pTxBuff, nWordCnt);
        CheckSum(TxBuff, 15+nWordCnt, &SumA, &SumB);
        TxBuff[15 + nWordCnt] = SumA;
        TxBuff[15 + 1 + nWordCnt] = SumB;
        TxBuff[15 + 2 + nWordCnt] = enCR;
        TxBuff[15 + 3 + nWordCnt] = enLF;
        if (!SendUntilAck(TxBuff, 15 + 4 + nWordCnt, enACK, 1000))
        {
            m_strErrMsg.Format(_T("CMelsecSerial::Write 에러=[%s]"), m_strErrMsg);
            return FALSE;
        }
    }
    return TRUE;
}
