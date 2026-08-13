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

	if (!RecvWait())
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

	if (!RecvWait())
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
	for (int i=1; i<nLen; ++i)
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
