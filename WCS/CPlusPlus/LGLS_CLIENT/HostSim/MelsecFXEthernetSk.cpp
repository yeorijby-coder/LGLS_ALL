// MelsecFXEthernetSk.cpp: implementation of the CMelsecEthernetSk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "MelsecFXEthernetSk.h"
#include "Equipment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMelsecFXEthernetSk, CInterfaceSk)

CMelsecFXEthernetSk::CMelsecFXEthernetSk(CEquipment* pEquipment) : CInterfaceSk(pEquipment)
{
	m_bReceiving = FALSE;

}

CMelsecFXEthernetSk::~CMelsecFXEthernetSk()
{
	m_bReceiving = FALSE;

}
//	enum {	enDeviceCodeD, enDeviceCodeR, 
//			enDeviceCodeTN, enDeviceCodeTS, 
//			enDeviceCodeCN, enDeviceCodeCS, 
//			enDeviceCodeX, enDeviceCodeY, 
//			enDeviceCodeM, enDeviceCodeS, 
//			enDeviceCodeSize };
//

//	nBatchRW - 1 : BatchRead,		nBatchRW - 2 : BatchWrite,		nBatchRW - 3 : Test, 
BYTE CMelsecFXEthernetSk::GetSubHeader(BYTE bDeviceCode, BYTE & bDeviceCode1, BYTE & bDeviceCode2, BOOL bBitUnit, int nBatchRW, BOOL bCommand)
{
	BYTE bSubHeader = 0x00;
	switch(nBatchRW)
	{
	case 1:		bSubHeader = (bBitUnit == TRUE) ? 0x00 : 0x01;		break;
	case 2:		bSubHeader = (bBitUnit == TRUE) ? 0x02 : 0x03;		break;
	case 3:		bSubHeader = (bBitUnit == TRUE) ? 0x04 : 0x05;		break;
	}

	switch(int(bDeviceCode))
	{
	case CMelsecFXEthernetSk::enDeviceCodeD:			bDeviceCode1 = 0x20;			bDeviceCode2 = 0x44;			break;
	case CMelsecFXEthernetSk::enDeviceCodeR:			bDeviceCode1 = 0x20;			bDeviceCode2 = 0x52;			break;
	case CMelsecFXEthernetSk::enDeviceCodeTN:			bDeviceCode1 = 0x4E;			bDeviceCode2 = 0x54;			break;
	case CMelsecFXEthernetSk::enDeviceCodeTS:			bDeviceCode1 = 0x53;			bDeviceCode2 = 0x54;			break;
	case CMelsecFXEthernetSk::enDeviceCodeCN:			bDeviceCode1 = 0x4E;			bDeviceCode2 = 0x43;			break;
	case CMelsecFXEthernetSk::enDeviceCodeCS:			bDeviceCode1 = 0x53;			bDeviceCode2 = 0x43;			break;
	case CMelsecFXEthernetSk::enDeviceCodeX:			bDeviceCode1 = 0x20;			bDeviceCode2 = 0x58;			break;
	case CMelsecFXEthernetSk::enDeviceCodeY:			bDeviceCode1 = 0x20;			bDeviceCode2 = 0x59;			break;
	case CMelsecFXEthernetSk::enDeviceCodeM:			bDeviceCode1 = 0x20;			bDeviceCode2 = 0x4D;			break;
	case CMelsecFXEthernetSk::enDeviceCodeS:			bDeviceCode1 = 0x20;			bDeviceCode2 = 0x53;			break;
	}
	
	if (bCommand == FALSE)
		bSubHeader |= 0x80; 

	return bSubHeader;
}

BOOL CMelsecFXEthernetSk::ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen, BYTE bDeviceCode)
{

	CByteArray arrBuffer;
	arrBuffer.SetSize(12);

	BYTE bDeviceCode1, bDeviceCode2;
	BYTE bRequestSubHeader = GetSubHeader(bDeviceCode, bDeviceCode1, bDeviceCode2, FALSE, 1, TRUE);

	arrBuffer[0] = bRequestSubHeader;				// Subheader (BATCH READ - WORD UNIT -'D','R','T','C')	

	arrBuffer[1] = 0xFF;							// PC No.	(FIXED VALUE)

    arrBuffer[2] = 0x0A;							// Monitoring Timer 1. 
	arrBuffer[3] = 0x00;							// Monitoring Timer 2.

	arrBuffer[4] = (nStartWord & 0xFF);				// Head device (Start Address)
	arrBuffer[5] = (nStartWord >> 8) & 0xFF;		// Head device (Start Address)
	arrBuffer[6] = (nStartWord >> 16) & 0xFF;		// Head device (Start Address)
	arrBuffer[7] = (nStartWord >> 24) & 0xFF;		// Head device (Start Address)

	arrBuffer[8]  = bDeviceCode1;					// DEVIDE NAME
	arrBuffer[9]  = bDeviceCode2;					// DEVIDE NAME

	arrBuffer[10] = (nWordLen & 0xFF);				// CPU monitoring timer (unit is 250ms)
	arrBuffer[11] = 0x00;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BYTE pBuffer[12];
	memset(pBuffer, 0x00, sizeof(pBuffer));

	if (!SendData(arrBuffer))
		return FALSE;

	if (!RecvWait())
		return FALSE;

	if (!RecvData(arrBuffer))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int nRecvLen = arrBuffer.GetSize();
	CString strRecv;
	strRecv.Format(_T("%s"),arrBuffer.GetData());

	memset(pBuffer, 0x00, sizeof(pBuffer));

	BYTE bRealResponseSubHeader = arrBuffer[0];

	BYTE bResponseSubHeader = GetSubHeader(bDeviceCode, bDeviceCode1, bDeviceCode2, FALSE, 1, FALSE);
	
	if (arrBuffer[0] != bResponseSubHeader)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 SUB HEADER 이상!"));
		return FALSE;
	}

	WORD wErrorCode = 0x0000;
	if (arrBuffer[1] != 0x00)					// Complete Code
	{
		m_strLog.Format(_T("응답 프레임 Complete Code 이상! [SubHeader = %x] [Complete Code = %x] [AbNormal Coe = %x]"), 
			arrBuffer[0], arrBuffer[1], arrBuffer[2]);
		wErrorCode = arrBuffer[2];
		return FALSE;
	}

	arrRxData.SetSize(nWordLen);
//	arrRxData.SetSize(nWordLen*2);
	BYTE bTemp = 0x00;
	int nTemp = nRecvLen;
	for (int i=0; i<arrRxData.GetSize(); ++i)
	{
		bTemp = arrBuffer[enBinaryHeaderLen+i];
		arrRxData[i] = bTemp;
	}


	return TRUE;

//*/
/*
	BYTE TxBuff[256];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	TxBuff[0] = 0x01;

	TxBuff[1] = 0xFF;

	TxBuff[2] = 0x0A;
	TxBuff[3] = 0x00;

	TxBuff[4] = 0x01;
	TxBuff[5] = 0x00;
	TxBuff[6] = 0x00;
	TxBuff[7] = 0x00;

	TxBuff[8] = 0x20;
	TxBuff[9] = 0x52;

	TxBuff[10] = (nWordLen & 0xFF);

	TxBuff[11] = 0x00;

	if (Send(TxBuff, nWordLen) == SOCKET_ERROR)
	{
		m_strErrMsg.Format("Read.. 송신에러[%s]",CLib::GetSystemErrMsg());
		return FALSE;
	}

//	CByteArray& arrRxData
	BYTE RxBuff[256];
	memset(RxBuff, 0x00, sizeof(TxBuff));
	BOOL bResult = RecvReadAck(0, RxBuff, nWordLen, bDeviceCode);

	for(int k=0;k<nWordLen;k++)
	{	
		arrRxData[k] = RxBuff[k];
	}

	return bResult;
	*/
}

BOOL CMelsecFXEthernetSk::RecvReadAck(int nUnitType, BYTE *pRxBuff, WORD wReadLen, BYTE bDeviceCode)
{
	int nRecvLen = 0;
	BYTE RxBuff[8192];
	memset(RxBuff, 0x00, sizeof(RxBuff));

	if((nRecvLen = RecvFrame(RxBuff)) == FALSE)		
		return FALSE;

	BYTE bDeviceCode1, bDeviceCode2;
	BYTE bResponseSubHeader = GetSubHeader(bDeviceCode, bDeviceCode1, bDeviceCode2, FALSE, 1, FALSE);
	
	WORD wErrorCode = 0x0000;
	if (RxBuff[1] != 0x00)					// Complete Code
	{
		m_strLog.Format(_T("응답 프레임 Complete Code 이상! [Complete Code = %x] [AbNormal Coe = %x]"), RxBuff[1], RxBuff[2]);
		wErrorCode = RxBuff[2];
		return FALSE;
	}

	if (RxBuff[0] != bResponseSubHeader)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 SUB HEADER 이상!"));
		return FALSE;
	}

	if (RxBuff[2] != (wReadLen & 0xFF))					// Complete Code
	{
		m_strLog.Format(_T("응답 프레임 길이 이상! "), RxBuff[1], RxBuff[2]);
		wErrorCode = RxBuff[2];
		return FALSE;
	}

	/*
	if(RxBuff[0] != 0xD0 || RxBuff[1] != 0x00)                //Subheader
	{
		m_strErrMsg.Format("RecvReadAck.. Subheader 이상..");
		return FALSE;
	}
	if(RxBuff[2] != 0x00 || RxBuff[3] != 0xFF)                // Network NO, PC NO
	{
		m_strErrMsg.Format("RecvReadAck.. Network or PC NO 이상..");
		return FALSE;
	}
	if(RxBuff[4] != 0xFF || RxBuff[5] != 0x03 || RxBuff[6] != 0x00)   // Specific value
	{
		m_strErrMsg.Format("RecvReadAck.. Specific Value 이상..");
		return FALSE;
	}

	WORD wErrNo = SwapToWord(RxBuff+9);
	if(wErrNo)
	{
		m_strErrMsg.Format("RecvReadAck.. 응답 수신.. 오류코드[%d]", wErrNo);
		return FALSE;
	}

	int nTotalLen = enBinaryHeaderLen + GetDataLength(nUnitType, wReadLen);
	if(nTotalLen != nRecvLen)
	{
		m_strErrMsg.Format("RecvReadAck.. 응답 프레임 길이 이상[전체%d - 수신%d]", nTotalLen, nRecvLen);
		return FALSE;
	}
	*/

	memcpy(pRxBuff, RxBuff+enBinaryHeaderLen, GetDataLength(nUnitType, wReadLen));

	return TRUE;
}


BOOL CMelsecFXEthernetSk::WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen, BYTE bDeviceCode)
{
	WORD wReqLen = 12 + nWordLen * 2;
	CByteArray arrBuffer;
	arrBuffer.SetSize(12);

//	nBatchRW - 1 : BatchRead,		nBatchRW - 2 : BatchWrite,		nBatchRW - 3 : Test, 
//	BYTE CMelsecFXEthernetSk::GetSubHeader(BYTE bDeviceCode, BYTE & bDeviceCode1, BYTE & bDeviceCode2, BOOL bBitUnit, int nBatchRW, BOOL bCommand)

	BYTE bDeviceCode1, bDeviceCode2;
	BYTE bRequestSubHeader = GetSubHeader(bDeviceCode, bDeviceCode1, bDeviceCode2, FALSE, 2, TRUE);

	arrBuffer[0] = bRequestSubHeader;				// Subheader (BATCH READ - WORD UNIT -'D','R','T','C')	

	arrBuffer[1] = 0xFF;							// PC No.	(FIXED VALUE)

    arrBuffer[2] = 0x0A;							// Monitoring Timer 1. 
	arrBuffer[3] = 0x00;							// Monitoring Timer 2.

	arrBuffer[4] = (nStartWord & 0xFF);				// Head device (Start Address)
	arrBuffer[5] = (nStartWord >> 8) & 0xFF;		// Head device (Start Address)
	arrBuffer[6] = (nStartWord >> 16) & 0xFF;		// Head device (Start Address)
	arrBuffer[7] = (nStartWord >> 24) & 0xFF;		// Head device (Start Address)

	arrBuffer[8]  = bDeviceCode1;					// DEVIDE NAME
	arrBuffer[9]  = bDeviceCode2;					// DEVIDE NAME

	arrBuffer[10] = (nWordLen & 0xFF);				// CPU monitoring timer (unit is 250ms)
	arrBuffer[11] = 0x00;

	arrBuffer.Append(arrTxData);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	CString strTemp(arrBuffer.GetData());

	BYTE TxBuff[12];
	memset(TxBuff, 0x00, sizeof(TxBuff));

	int nRecvLength = arrBuffer.GetSize();

	for (int i = 0 ; i < arrBuffer.GetSize() ; i++)
		TxBuff[i] = arrBuffer[i];

	if (!SendData(arrBuffer))
		return FALSE;

	if (!RecvWait())
		return FALSE;

	if (!RecvData(arrBuffer))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int nRecvLen = arrBuffer.GetSize();
	CString strRecv;
	strRecv.Format(_T("%s"),arrBuffer.GetData());
	BYTE bRealResponseSubHeader = arrBuffer[0];

	BYTE bResponseSubHeader = GetSubHeader(bDeviceCode, bDeviceCode1, bDeviceCode2, FALSE, 2, FALSE);
	
	WORD wErrorCode = 0x0000;
	if (arrBuffer[1] != 0x00)					// Complete Code
	{
		m_strLog.Format(_T("응답 프레임 Complete Code 이상! [Complete Code = %x] [AbNormal Coe = %x]"), arrBuffer[1], arrBuffer[2]);
		wErrorCode = arrBuffer[2];
		return FALSE;
	}

	if (arrBuffer[0] != bResponseSubHeader)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 SUB HEADER 이상!"));
		return FALSE;
	}

	return TRUE;
}

BOOL CMelsecFXEthernetSk::LoopBackTest(CByteArray& arrTxData, int nWordLen, BYTE bDeviceCode)
{
	WORD wReqLen = 12 + nWordLen * 2;
	CByteArray arrBuffer;
	arrBuffer.SetSize(5);

	BYTE bDeviceCode1, bDeviceCode2;
	BYTE bRequestSubHeader = GetSubHeader(bDeviceCode, bDeviceCode1, bDeviceCode2, FALSE, 1, TRUE);

	arrBuffer[0]  = 0x16;				// Subheader (BATCH READ - WORD UNIT -'D','R','T','C')	

	arrBuffer[1]  = 0xFF;							// PC No.	(FIXED VALUE)

    arrBuffer[2]  = 0x0A;							// Monitoring Timer 1. 
	arrBuffer[3]  = 0x00;							// Monitoring Timer 2.

	arrBuffer[4] = (nWordLen & 0xFF);				// CPU monitoring timer (unit is 250ms)

	arrBuffer.Append(arrTxData);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CString strTemp(arrBuffer.GetData());

	if (!SendData(arrBuffer))
		return FALSE;

	if (!RecvWait())
		return FALSE;

	if (!RecvData(arrBuffer))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int nRecvLen = arrBuffer.GetSize();
	CString strRecv;
	strRecv.Format(_T("%s"),arrBuffer.GetData());
	BYTE bRealResponseSubHeader = arrBuffer[0];

	BYTE bResponseSubHeader = GetSubHeader(bDeviceCode, bDeviceCode1, bDeviceCode2, FALSE, 1, FALSE);
	
	WORD wErrorCode = 0x0000;
	if (arrBuffer[1] != 0x00)					// Complete Code
	{
		m_strLog.Format(_T("응답 프레임 Complete Code 이상! [Complete Code = %x] [AbNormal Coe = %x]"), arrBuffer[1], arrBuffer[2]);
		wErrorCode = arrBuffer[2];
		return FALSE;
	}

	if (arrBuffer[0] != bResponseSubHeader)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 SUB HEADER 이상!"));
		return FALSE;
	}

	if ((arrBuffer[2] != (nWordLen & 0xFF)) || 
		(arrTxData.GetSize() != nRecvLen -3))					// Complete Code
	{
		m_strLog.Format(_T("응답 프레임 길이 이상! "), arrBuffer[1], arrBuffer[2]);
		wErrorCode = arrBuffer[2];
		return FALSE;
	}

	for (int i = 0 ; i < arrTxData.GetSize() ; i++)
	{
		if (arrTxData[i] != arrBuffer[2+i])
			return FALSE;
	}

	return TRUE;
}


BOOL CMelsecFXEthernetSk::RecvWriteAck(BYTE bDeviceCode)
{
	BYTE RxBuff[2048];
	int nRecvLen;

	memset(RxBuff, 0x00, sizeof(RxBuff));
	if((nRecvLen = RecvFrame(RxBuff)) == FALSE)
		return FALSE;

	BYTE bDeviceCode1, bDeviceCode2;
	BYTE bResponseSubHeader = GetSubHeader(bDeviceCode, bDeviceCode1, bDeviceCode2, FALSE, 1, FALSE);
	
	WORD wErrorCode = 0x0000;
	if (RxBuff[1] != 0x00)					// Complete Code
	{
		m_strLog.Format(_T("응답 프레임 Complete Code 이상! [Complete Code = %x] [AbNormal Coe = %x]"), RxBuff[1], RxBuff[2]);
		wErrorCode = RxBuff[2];
		return FALSE;
	}

	if (RxBuff[0] != bResponseSubHeader)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 SUB HEADER 이상!"));
		return FALSE;
	}

//	if (RxBuff[2] != (nRecvLen & 0xFF)) 					// Complete Code
//	{
//		m_strLog.Format(_T("응답 프레임 길이 이상! "), RxBuff[1], RxBuff[2]);
//		wErrorCode = RxBuff[2];
//		return FALSE;
//	}

	/*
	if(RxBuff[0] != 0xD0 || RxBuff[1] != 0x00)                //Subheader
	{
		m_strErrMsg.Format("RecvReadAck.. Subheader 이상..");
		return FALSE;
	}
	if(RxBuff[2] != 0x00 || RxBuff[3] != 0xFF)                // Network NO, PC NO
	{
		m_strErrMsg.Format("RecvWriteAck.. Network or PC NO 이상..");
		return FALSE;
	}
	if(RxBuff[4] != 0xFF || RxBuff[5] != 0x03 || RxBuff[6] != 0x00)   // Specific value
	{
		m_strErrMsg.Format("RecvWriteAck.. Specific Value 이상..");
		return FALSE;
	}

	WORD wErrNo = SwapToWord(RxBuff+9);
	if(wErrNo)
	{
		m_strErrMsg.Format("RecvWriteAck.. 응답 수신.. 오류코드[%d]", wErrNo);
		return FALSE;
	}
	*/

	return TRUE;
}

int CMelsecFXEthernetSk::RecvFrame(BYTE *pRxBuff)
{
	DWORD dwLen = 0, nRealLen, nTotalLen = 0;

	m_bReceiving = TRUE;

	if(WaitForSingleObject(m_hRecvEvent, 2000) == WAIT_TIMEOUT)
	{
		if(WaitForSingleObject(m_hRecvEvent, 2000) == WAIT_TIMEOUT)
		{
			m_strErrMsg.Format(_T("RecvFrame.. 수신 대기시간 초과 에러"));
			return FALSE;
		}
	}

	if(IOCtl( FIONREAD, &dwLen ) == FALSE)
	{
		m_strErrMsg.Format(_T("RecvFrame.. IOCtl 에러 [%s]"), CLib::GetSystemErrMsg());
		return FALSE;
	}

	if ((nRealLen = Receive(pRxBuff, dwLen)) != dwLen)
	{
		m_strErrMsg.Format(_T("RecvFrame.. Receive에러 [%s]"), CLib::GetSystemErrMsg());
		return FALSE;
	}

	nTotalLen = nRealLen;

	if (dwLen < enBinaryHeaderLen)
	{
		if(WaitForSingleObject(m_hRecvEvent, 2000) == WAIT_TIMEOUT)	// 2000
		{
			m_strErrMsg.Format(_T("RecvFrame.. 수신 대기시간 초과 에러#2 [dwLen=%d]"), dwLen);
			return FALSE;
		}

		if(IOCtl( FIONREAD, &dwLen ) == FALSE)
		{
			m_strErrMsg.Format(_T("RecvFrame.. IOCtl 에러#2 [%s]"), CLib::GetSystemErrMsg());
			return FALSE;
		}

		if ((nRealLen = Receive(pRxBuff+nRealLen, dwLen)) != dwLen)
		{
			m_strErrMsg.Format(_T("RecvFrame.. Receive에러#2 [%s]"), CLib::GetSystemErrMsg());
			return FALSE;
		}

		nTotalLen += nRealLen;

		if(nRealLen <= enBinaryHeaderLen)
		{
			m_strErrMsg.Format(_T("RecvFrame.. 수신 데이터 길이[%d] 이상"), nRealLen);
			return FALSE;
		}
	}

	m_bReceiving = FALSE;

	return nTotalLen;
}

WORD CMelsecFXEthernetSk::SwapToWord(BYTE *pSrc)
{
	return (WORD)(pSrc[1] << 8) | pSrc[0];
}

DWORD CMelsecFXEthernetSk::SwapToDWord(BYTE *pSrc)
{
	return (DWORD)(pSrc[3] << 24) | (pSrc[2] << 16) | (pSrc[1] << 8) | pSrc[0];
}


int CMelsecFXEthernetSk::GetDataLength(int nUnitType, WORD wLen)
{
	return (nUnitType == CMD_WORD_UNIT) ? (wLen*2) : ((wLen/2)+(wLen%2));
}
