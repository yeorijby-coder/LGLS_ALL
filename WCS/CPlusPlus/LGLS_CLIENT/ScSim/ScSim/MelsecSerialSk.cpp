// MelsecSerialSk.cpp: implementation of the CMelsecSerialSk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "MelsecSerialSk.h"
#include "Equipment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMelsecSerialSk, CInterfaceSk)

CMelsecSerialSk::CMelsecSerialSk(CEquipment* pEquipment, int nIndex) : CInterfaceSk(pEquipment, nIndex)
{

}

CMelsecSerialSk::~CMelsecSerialSk()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CMelsecSerialSk::ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen)
{
	CByteArray arrBuffer;
	arrBuffer.SetSize(19);
	arrBuffer[0] = enENQ;

	int i=0;
	CString strBuffer;
	strBuffer.Format(_T("00FFWR0D%.4d%.2X"), nStartWord, nWordLen);
	for (i=0; i<strBuffer.GetLength(); ++i)
		arrBuffer[1+i] = (BYTE)strBuffer[i];

	BYTE ucSumA, ucSumB;
	CheckSum(arrBuffer, 15, ucSumA, ucSumB);
	arrBuffer[15] = ucSumA;
	arrBuffer[16] = ucSumB;
	arrBuffer[17] = enCR;
	arrBuffer[18] = enLF;

	////////////////////////////////////////////////////////////////////////////////////

	if (!SendData(arrBuffer))
		return FALSE;

	if (!RecvWait(m_nIndex))
		return FALSE;

	if (!RecvData(arrBuffer))
		return FALSE;

	////////////////////////////////////////////////////////////////////////////////////

//	MELSEC PLC 책자 참조 2002. 2. 27
	int  nLen = (nWordLen * 4) + 5 + 5; // STX+4+WordLen*4+ETX+SumA+SumB+CR+LF
//	int  nLen = (nWordCnt * 4) + 8;  // '2'
//	int  nLen = (nWordCnt * 4) + 6;  // '1' CheckSum 확인 안하는 옵션

	// SCP : 실제 데이터 수신시 1 Byte 더 수신됨 (RxFrame + 0x00)

	if (arrBuffer.GetSize() < nLen)
	{
		m_strLog.Format(_T("응답메시지 길이 이상! [예상=%d, 실제=%d]"), nLen, arrBuffer.GetSize());
		ResponseNak();
		return FALSE;
	}

	CheckSum(arrBuffer, nLen-4, ucSumA, ucSumB);

	if ((arrBuffer[0] != enSTX) || 
		(arrBuffer[nLen-1] != enLF) || 
		(arrBuffer[nLen-2] != enCR) ||
		(arrBuffer[nLen-3] != ucSumB) || 
		(arrBuffer[nLen-4] != ucSumA) || 
		(arrBuffer[nLen-5] != enETX))
	{
		m_strLog.Format(_T("응답메시지 데이터 이상! [예상=%d, 실제=%d]"), nLen, arrBuffer.GetSize());
		ResponseNak();
		return FALSE;
	}

	arrRxData.SetSize(nWordLen * 4);
	for (i=0; i<arrRxData.GetSize(); ++i)
		arrRxData[i] = arrBuffer[5+i];

	ResponseAck();

	return TRUE;
}

BOOL CMelsecSerialSk::WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen)
{
	DEBUGER_ASSERT_VALID(arrTxData.GetSize() == nWordLen*4);

	CByteArray arrBuffer;
	int nLen = 15 + arrTxData.GetSize() + 4;
	arrBuffer.SetSize(nLen);
	arrBuffer[0] = enENQ;

	int i=0;
	CString strBuffer;
	strBuffer.Format(_T("00FFWW0D%.4d%.2X"), nStartWord, nWordLen);
	for (i=0; i<strBuffer.GetLength(); ++i)
		arrBuffer[1+i] = (BYTE)strBuffer[i];

	for (i=0; i<arrTxData.GetSize(); ++i)
		arrBuffer[15+i] = arrTxData[i];

	BYTE ucSumA, ucSumB;
	CheckSum(arrBuffer, nLen-4, ucSumA, ucSumB);
	arrBuffer[nLen-4] = ucSumA;
	arrBuffer[nLen-3] = ucSumB;
	arrBuffer[nLen-2] = enCR;
	arrBuffer[nLen-1] = enLF;

	////////////////////////////////////////////////////////////////////////////////////

	if (!SendData(arrBuffer))
		return FALSE;

	if (!RecvWait(m_nIndex))
		return FALSE;

	if (!RecvData(arrBuffer))
		return FALSE;

	////////////////////////////////////////////////////////////////////////////////////

	// 성공 = ACK + 00FF + CR + LF (HEX: 06 30 30 46 46 0d 0a)
	// 실패 = NAK + 00FF + CR + LF (HEX: 15 30 30 46 46 0d 0a)

	if (arrBuffer.GetSize() < 7)
	{
		m_strLog.Format(_T("수신 데이터 길이 이상! [%s]"), (LPCTSTR)arrBuffer.GetData());
		return FALSE;
	}

	if (arrBuffer[0] == enNAK)
	{
		m_strLog.Format(_T("수신 데이터 NAK 응답! [%s]"), (LPCTSTR)arrBuffer.GetData());
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMelsecSerialSk::CheckRequest(CByteArray& arrRxData)
{
	CByteArray arrBuffer;

	if (!RecvData(arrBuffer))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int nRecvLen = arrBuffer.GetSize();
	int nTotalLen = 20;
	if (nRecvLen < nTotalLen)
	{
		m_strLog.Format(_T("응답 프레임 길이 이상! [예상=%d, 실제=%d]"), nTotalLen, nRecvLen);
		return FALSE;
	}

	if (arrBuffer[0] != enENQ)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 응답 요구 이상"));
		return FALSE;
	}

	BYTE bTemp1  = arrBuffer[1];
	BYTE bTemp2  = arrBuffer[2];
	BYTE bTemp3  = arrBuffer[3];
	BYTE bTemp4  = arrBuffer[4];
	BYTE bTemp5  = arrBuffer[5];
	BYTE bTemp6  = arrBuffer[6];
	BYTE bTemp7  = arrBuffer[7];
	BYTE bTemp8  = arrBuffer[8];
	BYTE bTemp9  = arrBuffer[9];
	BYTE bTemp10 = arrBuffer[10];
	BYTE bTemp11 = arrBuffer[11];
	BYTE bTemp12 = arrBuffer[12];
	BYTE bTemp13 = arrBuffer[13];
	BYTE bTemp14 = arrBuffer[14];
	BYTE bTemp15 = arrBuffer[15];
	BYTE bTemp16 = arrBuffer[16];
	BYTE bTemp17 = arrBuffer[17];
	BYTE bTemp18 = arrBuffer[18];

	if (arrBuffer[1] != '0' ||
		arrBuffer[2] != '0' ||
		arrBuffer[3] != 'F' ||
		arrBuffer[4] != 'F' ||
		arrBuffer[5] != 'W' ||
		arrBuffer[6] != 'R' ||
		arrBuffer[7] != '0' ||
		arrBuffer[8] != 'D' )
	{
		m_strLog.Format(_T("응답 프레임 Header 내용 이상"));
		return FALSE;
	}

	BYTE ucSumA, ucSumB;
	CheckSum(arrBuffer, 15, ucSumA, ucSumB);

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
	for (int i = 0; i < arrRxData.GetSize(); ++i)
		arrRxData[i] = arrBuffer[i];

	return TRUE;
}

BOOL CMelsecSerialSk::ResponseAck()
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

BOOL CMelsecSerialSk::ResponseNak()
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

void CMelsecSerialSk::CheckSum(CByteArray& arrData, int nLen, BYTE& rSumA, BYTE& rSumB)
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

BOOL CMelsecSerialSk::ResponseReadWord(CByteArray& arrTxData, int nLen)
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
		bTemp = CLib::DecToChar((arrTxData[i]) & 0x0F);
		arrBuffer[5 + (i * 2) + 0] = bTemp;				// 5 + (i * 2) + 0	
		strTemp1.Format(_T("%c"), bTemp);		
		strTemp += strTemp1;


		bTemp = CLib::DecToChar((arrTxData[i] >> 4) & 0x0F);
		arrBuffer[5 + (i * 2) + 1] = bTemp;				// 5 + (i * 2) + 1
		strTemp1.Format(_T("%c"), bTemp);
		strTemp += strTemp1;
	}
	BYTE * pArray = arrBuffer.GetData();

	strTemp2 = CString(pArray);		bTemp = arrBuffer.GetAt(nSize + 5);

	arrBuffer[nSize + 5] = enETX;	strTemp2 = CString(pArray);		bTemp = arrBuffer.GetAt(nSize + 6);

	BYTE ucSumA, ucSumB;
	CheckSum(arrBuffer, nSize + 5, ucSumA, ucSumB);
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

BOOL CMelsecSerialSk::ResponseWriteWord(CByteArray& arrTxData, int nLen)
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//arrBuffer.Append(arrTxData);
	if (!SendData(arrBuffer))
		return FALSE;

	return TRUE;
}
