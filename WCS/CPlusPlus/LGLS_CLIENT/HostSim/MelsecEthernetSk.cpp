// MelsecEthernetSk.cpp: implementation of the CMelsecEthernetSk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "MelsecEthernetSk.h"
#include "Equipment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMelsecEthernetSk, CInterfaceSk)

CMelsecEthernetSk::CMelsecEthernetSk(CEquipment* pEquipment) : CInterfaceSk(pEquipment)
{
	m_bReceiving = FALSE;

}

CMelsecEthernetSk::~CMelsecEthernetSk()
{
	m_bReceiving = FALSE;

}
/*
BOOL CMelsecEthernetSk::Read(int nUnitType, BYTE *pRxBuff, BYTE DeviceCode, int nStartAddr, WORD wReadLen, BOOL bIsAscii)
{
	PostMsgToOwner(NOTIFY_SEND);

	BYTE TxBuff[1024];
	memset(TxBuff, 0x00, sizeof(TxBuff));

    TxBuff[0]  = 0x50;  
	TxBuff[1]  = 0x00;	// Subheader	

	TxBuff[2]  = 0x00;	// Network No. (Self station)ㅇ
	TxBuff[3]  = 0xFF;	// PLC No.

    TxBuff[4]  = 0xFF;	// 요구 상대 모듈 I/O 번호
	TxBuff[5]  = 0x03;	//
	
	TxBuff[6]  = 0x00;	// 요구 상대 모듈 국번호

	TxBuff[7]  = 0x0C;
	TxBuff[8]  = 0x00;	// Request data length

	TxBuff[9]  = 0x08;  //
	TxBuff[10] = 0x00;	// CPU monitoring timer (unit is 250ms)

	TxBuff[11] = 0x01;
	TxBuff[12] = 0x04;  // Command
	TxBuff[13] = nUnitType;
	TxBuff[14] = 0x00; 	// Subcommand

	TxBuff[15] = (nStartAddr & 0xFF);
	TxBuff[16] = (nStartAddr >> 8) & 0xFF;
	TxBuff[17] = (nStartAddr >> 16) & 0xFF;		// Head device (Start Address)

	TxBuff[18] = DeviceCode;					// Device code (Memory Type)

	TxBuff[19] = wReadLen & 0x00FF;				// WORD단위:읽을 워드수, BIT단위:읽을 비트수
	TxBuff[20] = (wReadLen >> 8) & 0x00FF;		// Number of device points

	CString strBuf = CCommonFunc::makeStringFromBin(TxBuff, 21);
	
	if(strBuf == " ")
		return FALSE;

	if(bIsAscii)
	{
		strBuf.Format("500000FF03FF000018000804010000D*%06d%04X", nStartAddr, wReadLen);
		if( Send(strBuf, strBuf.GetLength()) == SOCKET_ERROR )
		{
			PostMsgToOwner(NOTIFY_SEND_ERROR);
			m_strErrMsg.Format("Read.. 송신 에러 [%s]", CCommonFunc::GetSystemErrMsg(CAsyncSocket::GetLastError()));
			TRACE("CMelsec::Read.Send ASCII Send에러 (%s)\n", m_strErrMsg);
			return FALSE;
		}
	}
	else
	{
		if( Send(TxBuff, 21) == SOCKET_ERROR )
		{
			PostMsgToOwner(NOTIFY_SEND_ERROR);
			m_strErrMsg.Format("Read.. 송신 에러 [%s]", CCommonFunc::GetSystemErrMsg(CAsyncSocket::GetLastError()));
			TRACE("CMelsec::Read.Send Binary Send에러 (%s)\n", m_strErrMsg);
			return FALSE;
		}
	}

	BOOL bRet = RecvReadAck(nUnitType, pRxBuff, wReadLen, bIsAscii);
	if(bRet)				PostMsgToOwner(NOTIFY_SEND_END);
	else					PostMsgToOwner(NOTIFY_SEND_ERROR);

	return bRet;
}

BOOL CMelsecEthernetSk::Write(int nUnitType, BYTE *pTxBuff, BYTE DeviceCode, int nStartAddr, WORD wWriteLen, BOOL bIsAscii)
{
	PostMsgToOwner(NOTIFY_RECEIVE);

	m_strErrMsg = "";

	BYTE TxBuff[2048];
	memset(TxBuff, 0x00, sizeof(TxBuff));
	WORD wReqLen = 12 + GetDataLength(nUnitType, wWriteLen);

    TxBuff[0]  = 0x50;  
	TxBuff[1]  = 0x00;	// Subheader	

	TxBuff[2]  = 0x00;	// Network No. (Self station)
	TxBuff[3]  = 0xFF;	// PC No.

    TxBuff[4]  = 0xFF;	
	TxBuff[5]  = 0x03;	// Specific value
	TxBuff[6]  = 0x00;	// Specific value 2

	TxBuff[7]  = wReqLen & 0x00FF;
	TxBuff[8]  = (wReqLen >> 8) & 0x00FF;  	// Request data length

	TxBuff[9]  = 0x08;  // 0x04;
	TxBuff[10] = 0x00;	// CPU monitoring timer (unit is 250ms)

	TxBuff[11] = 0x01;
	TxBuff[12] = 0x14;  //Command
	TxBuff[13] = nUnitType;
	TxBuff[14] = 0x00; 	// Subcommand

	TxBuff[15] = (nStartAddr & 0xFF);
	TxBuff[16] = (nStartAddr >> 8) & 0xFF;
	TxBuff[17] = (nStartAddr >> 16) & 0xFF;		// Head device (Start Address)

	TxBuff[18] = DeviceCode;					// Device code (Memory Type)

	TxBuff[19] = wWriteLen & 0x00FF;            //WORD단위:쓸 워드수, BIT단위:쓸 비트수
	TxBuff[20] = (wWriteLen >> 8) & 0x00FF;		// Number of device points

	memcpy(TxBuff+21, pTxBuff, GetDataLength(nUnitType, wWriteLen));
	int nLen = 21 + GetDataLength(nUnitType, wWriteLen);

	CString strBuf = CCommonFunc::makeStringFromBin(TxBuff, 21);

	if(strBuf == " ")
		return FALSE;

	if(bIsAscii)
	{
		CString strTemp="", strHigh, strLow;
		for(int i=0; i<wWriteLen; i++)
		{
			strLow.Format("%02X", pTxBuff[i*2]);
			strHigh.Format("%02X", pTxBuff[i*2 + 1]);
			strTemp += (strHigh + strLow);
		}
		strBuf.Format("500000FF03FF00%04X000A14010000D*%06d%04X%s", 
			24+strTemp.GetLength(), nStartAddr, wWriteLen, strTemp);

		if( Send(strBuf, strBuf.GetLength()) == SOCKET_ERROR )
		{
			PostMsgToOwner(NOTIFY_RECEIVE_ERROR);
			TRACE("CMelsec::Write Ascii.. Send에러 시스템 메세지=%s\n", CCommonFunc::GetSystemErrMsg());
			return FALSE;
		}
	}
	else
	{
		if( Send(TxBuff, nLen) == SOCKET_ERROR )
		{
			PostMsgToOwner(NOTIFY_RECEIVE_ERROR);
			TRACE("CMelsec::Write Binary.. Send에러 시스템 메세지=%s\n", CCommonFunc::GetSystemErrMsg());
			return FALSE;
		}
	}

	BOOL bRet = RecvWriteAck(bIsAscii);

	if(bRet)			PostMsgToOwner(NOTIFY_RECEIVE_END);
	else				PostMsgToOwner(NOTIFY_RECEIVE_ERROR);

	return bRet;
}
*/
BOOL CMelsecEthernetSk::ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen, BYTE bDeviceCode)
{
	CByteArray arrBuffer;
	arrBuffer.SetSize(21);

    arrBuffer[0]  = 0x50;  
	arrBuffer[1]  = 0x00;							// Subheader	

	arrBuffer[2]  = 0x00;							// Network No. (Self station)
	arrBuffer[3]  = 0xFF;							// PC No.

    arrBuffer[4]  = 0xFF;	
	arrBuffer[5]  = 0x03;							// Specific value
	arrBuffer[6]  = 0x00;							// Specific value 2

	arrBuffer[7]  = 0x0C;
	arrBuffer[8]  = 0x00;							// Request data length

	arrBuffer[9]  = 0x08;							// 0x04;
	arrBuffer[10] = 0x00;							// CPU monitoring timer (unit is 250ms)

	arrBuffer[11] = 0x01;
	arrBuffer[12] = 0x04;							// Command
	arrBuffer[13] = enCmdWordUnit;
	arrBuffer[14] = 0x00; 							// Subcommand

	arrBuffer[15] = (nStartWord & 0xFF);
	arrBuffer[16] = (nStartWord >> 8) & 0xFF;
	arrBuffer[17] = (nStartWord >> 16) & 0xFF;		// Head device (Start Address)

	arrBuffer[18] = bDeviceCode;					// Device code (Memory Type)

	arrBuffer[19] = nWordLen & 0x00FF;				// WORD단위:읽을 워드수, BIT단위:읽을 비트수
	arrBuffer[20] = (nWordLen >> 8) & 0x00FF;		// Number of device points

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (!SendData(arrBuffer))
		return FALSE;

	if (!RecvWait())
		return FALSE;

	if (!RecvData(arrBuffer))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int nRecvLen = arrBuffer.GetSize();
	int nTotalLen = enBinaryHeaderLen + nWordLen*2;
	if (nRecvLen < nTotalLen)
	{
		m_strLog.Format(_T("응답 프레임 길이 이상! [예상=%d, 실제=%d]"), nTotalLen, nRecvLen);
		return FALSE;
	}

	if (arrBuffer[0] != 0xD0 || arrBuffer[1] != 0x00)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 SUB HEADER 이상!"));
		return FALSE;
	}

	if (arrBuffer[2] != 0x00 || arrBuffer[3] != 0xFF)		// Network NO, PC NO
	{
		m_strLog.Format(_T("응답 프레임 NETWORK OR PC NO 이상!"));
		return FALSE;
	}

	if (arrBuffer[4] != 0xFF || arrBuffer[5] != 0x03 || arrBuffer[6] != 0x00)   // Specific value
	{
		m_strLog.Format(_T("응답 프레임 SPECIFIC VALUE 이상!"));
		return FALSE;
	}

	WORD wErrorCode = (WORD)((arrBuffer[10] << 8) | arrBuffer[9]);
	if (wErrorCode)
	{
		m_strLog.Format(_T("응답 프레임 에러! [ERROR=%d]"), wErrorCode);
		return FALSE;
	}

	arrRxData.SetSize(nWordLen*2);
	for (int i=0; i<arrRxData.GetSize(); ++i)
		arrRxData[i] = arrBuffer[enBinaryHeaderLen+i];

	return TRUE;
}

BOOL CMelsecEthernetSk::WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen, BYTE bDeviceCode)
{
	WORD wReqLen = 12 + nWordLen * 2;
	CByteArray arrBuffer;
	arrBuffer.SetSize(21);

    arrBuffer[0]  = 0x50;  
	arrBuffer[1]  = 0x00;						// Subheader	

	arrBuffer[2]  = 0x00;						// Network No. (Self station)
	arrBuffer[3]  = 0xFF;						// PC No.

    arrBuffer[4]  = 0xFF;	
	arrBuffer[5]  = 0x03;						// Specific value
	arrBuffer[6]  = 0x00;						// Specific value 2

	arrBuffer[7]  = wReqLen & 0x00FF;
	arrBuffer[8]  = (wReqLen >> 8) & 0x00FF;  	// Request data length

	arrBuffer[9]  = 0x08;						// 0x04;
	arrBuffer[10] = 0x00;						// CPU monitoring timer (unit is 250ms)

	arrBuffer[11] = 0x01;
	arrBuffer[12] = 0x14;						// Command
	arrBuffer[13] = enCmdWordUnit;
	arrBuffer[14] = 0x00; 						// Subcommand

	arrBuffer[15] = (nStartWord & 0xFF);
	arrBuffer[16] = (nStartWord >> 8) & 0xFF;
	arrBuffer[17] = (nStartWord >> 16) & 0xFF;	// Head device (Start Address)

	arrBuffer[18] = bDeviceCode;				// Device code (Memory Type)

	arrBuffer[19] = nWordLen & 0x00FF;			// WORD단위:쓸 워드수, BIT단위:쓸 비트수
	arrBuffer[20] = (nWordLen >> 8) & 0x00FF;	// Number of device points

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

	if (arrBuffer.GetSize() < enBinaryHeaderLen)
	{
		m_strLog.Format(_T("응답 프레임 길이 이상! [LEN=%d]"), arrBuffer.GetSize());
		return FALSE;
	}

	if (arrBuffer[0] != 0xD0 || arrBuffer[1] != 0x00)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 SUB HEADER 이상!"));
		return FALSE;
	}

	if (arrBuffer[2] != 0x00 || arrBuffer[3] != 0xFF)		// Network NO, PC NO
	{
		m_strLog.Format(_T("응답 프레임 NETWORK OR PC NO 이상!"));
		return FALSE;
	}

	if (arrBuffer[4] != 0xFF || arrBuffer[5] != 0x03 || arrBuffer[6] != 0x00)   // Specific value
	{
		m_strLog.Format(_T("응답 프레임 SPECIFIC VALUE 이상!"));
		return FALSE;
	}

	WORD wErrorCode = (WORD)((arrBuffer[10] << 8) | arrBuffer[9]);
	if (wErrorCode)
	{
		m_strLog.Format(_T("응답 프레임 에러! [ERROR=%d]"), wErrorCode);
		return FALSE;
	}

	return TRUE;
}




BOOL CMelsecEthernetSk::Read(int nUnitType, BYTE *pRxBuff, BYTE DeviceCode, int nStartAddr, WORD wReadLen)
{
	m_strErrMsg = "";

	BYTE TxBuff[1024];
	memset(TxBuff, 0x00, sizeof(TxBuff));

    TxBuff[0]  = 0x50;  
	TxBuff[1]  = 0x00;	// Subheader	

	TxBuff[2]  = 0x00;	// Network No. (Self station)
	TxBuff[3]  = 0xFF;	// PC No.

    TxBuff[4]  = 0xFF;	
	TxBuff[5]  = 0x03;	// Specific value
	TxBuff[6]  = 0x00;	// Specific value 2

	TxBuff[7]  = 0x0C;
	TxBuff[8]  = 0x00;	// Request data length

	TxBuff[9]  = 0x08;  // 0x04;
	TxBuff[10] = 0x00;	// CPU monitoring timer (unit is 250ms)

	TxBuff[11] = 0x01;
	TxBuff[12] = 0x04;  //Command
	TxBuff[13] = nUnitType;
	TxBuff[14] = 0x00; 	// Subcommand

	TxBuff[15] = (nStartAddr & 0xFF);
	TxBuff[16] = (nStartAddr >> 8) & 0xFF;
	TxBuff[17] = (nStartAddr >> 16) & 0xFF;		// Head device (Start Address)

	TxBuff[18] = DeviceCode;					// Device code (Memory Type)

	TxBuff[19] = wReadLen & 0x00FF;				//WORD단위:읽을 워드수, BIT단위:읽을 비트수
	TxBuff[20] = (wReadLen >> 8) & 0x00FF;		// Number of device points

	Sleep(30);

//	if (!SendData(arrBuffer))
//		return FALSE;

	if(Send(TxBuff, 21) == SOCKET_ERROR )
	{
		m_strErrMsg.Format(_T("Read.. 송신 에러 [%s]"), CLib::GetSystemErrMsg());
		return FALSE;
	}

	return RecvReadAck(nUnitType, pRxBuff, wReadLen);
}

BOOL CMelsecEthernetSk::Write(int nUnitType, BYTE *pTxBuff, BYTE DeviceCode, int nStartAddr, WORD wWriteLen)
{
	m_strErrMsg = "";

	BYTE TxBuff[1024];
	memset(TxBuff, 0x00, sizeof(TxBuff));
	WORD wReqLen = 12 + GetDataLength(nUnitType, wWriteLen);

    TxBuff[0]  = 0x50;  
	TxBuff[1]  = 0x00;	// Subheader	

	TxBuff[2]  = 0x00;	// Network No. (Self station)
	TxBuff[3]  = 0xFF;	// PC No.

    TxBuff[4]  = 0xFF;	
	TxBuff[5]  = 0x03;	// Specific value
	TxBuff[6]  = 0x00;	// Specific value 2

	TxBuff[7]  = wReqLen & 0x00FF;
	TxBuff[8]  = (wReqLen >> 8) & 0x00FF;  	// Request data length

	TxBuff[9]  = 0x08;  // 0x04;
	TxBuff[10] = 0x00;	// CPU monitoring timer (unit is 250ms)

	TxBuff[11] = 0x01;
	TxBuff[12] = 0x14;  //Command
	TxBuff[13] = nUnitType;
	TxBuff[14] = 0x00; 	// Subcommand

	TxBuff[15] = (nStartAddr & 0xFF);
	TxBuff[16] = (nStartAddr >> 8) & 0xFF;
	TxBuff[17] = (nStartAddr >> 16) & 0xFF;		// Head device (Start Address)

	TxBuff[18] = DeviceCode;					// Device code (Memory Type)

	TxBuff[19] = wWriteLen & 0x00FF;            //WORD단위:쓸 워드수, BIT단위:쓸 비트수
	TxBuff[20] = (wWriteLen >> 8) & 0x00FF;		// Number of device points

	memcpy(TxBuff+21, pTxBuff, GetDataLength(nUnitType, wWriteLen));
	int nLen = 21 + GetDataLength(nUnitType, wWriteLen);

	if( Send(TxBuff, nLen) == SOCKET_ERROR )
	{
		m_strErrMsg.Format(_T("Write.. 송신 에러 [%s]"), CLib::GetSystemErrMsg());
		return FALSE;
	}

//	return RecvWriteAck();
	if (!RecvWait())
		return FALSE;

	CByteArray arrBuffer;
	arrBuffer.SetSize(21);

	if (!RecvData(arrBuffer))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (arrBuffer.GetSize() < enBinaryHeaderLen)
	{
		m_strLog.Format(_T("응답 프레임 길이 이상! [LEN=%d]"), arrBuffer.GetSize());
		return FALSE;
	}

	if (arrBuffer[0] != 0xD0 || arrBuffer[1] != 0x00)		// SUB HEADER
	{
		m_strLog.Format(_T("응답 프레임 SUB HEADER 이상!"));
		return FALSE;
	}

	if (arrBuffer[2] != 0x00 || arrBuffer[3] != 0xFF)		// Network NO, PC NO
	{
		m_strLog.Format(_T("응답 프레임 NETWORK OR PC NO 이상!"));
		return FALSE;
	}

	if (arrBuffer[4] != 0xFF || arrBuffer[5] != 0x03 || arrBuffer[6] != 0x00)   // Specific value
	{
		m_strLog.Format(_T("응답 프레임 SPECIFIC VALUE 이상!"));
		return FALSE;
	}

	WORD wErrorCode = (WORD)((arrBuffer[10] << 8) | arrBuffer[9]);
	if (wErrorCode)
	{
		m_strLog.Format(_T("응답 프레임 에러! [ERROR=%d]"), wErrorCode);
		return FALSE;
	}
	return TRUE;
}

BOOL CMelsecEthernetSk::RecvReadAck(int nUnitType, BYTE *pRxBuff, WORD wReadLen)
{
	int nRecvLen = 0;
	BYTE RxBuff[8192];
	memset(RxBuff, 0x00, sizeof(RxBuff));

	if((nRecvLen = RecvFrame(RxBuff)) == FALSE)		
		return FALSE;

	if(RxBuff[0] != 0xD0 || RxBuff[1] != 0x00)                //Subheader
	{
		m_strErrMsg.Format(_T("RecvReadAck.. Subheader 이상.."));
		return FALSE;
	}
	if(RxBuff[2] != 0x00 || RxBuff[3] != 0xFF)                // Network NO, PC NO
	{
		m_strErrMsg.Format(_T("RecvReadAck.. Network or PC NO 이상.."));
		return FALSE;
	}
	if(RxBuff[4] != 0xFF || RxBuff[5] != 0x03 || RxBuff[6] != 0x00)   // Specific value
	{
		m_strErrMsg.Format(_T("RecvReadAck.. Specific Value 이상.."));
		return FALSE;
	}

	WORD wErrNo = SwapToWord(RxBuff+9);
	if(wErrNo)
	{
		m_strErrMsg.Format(_T("RecvReadAck.. 응답 수신.. 오류코드[%d]"), wErrNo);
		return FALSE;
	}

	int nTotalLen = BINARY_HEADER_LEN + GetDataLength(nUnitType, wReadLen);
	if(nTotalLen != nRecvLen)
	{
		m_strErrMsg.Format(_T("RecvReadAck.. 응답 프레임 길이 이상[전체%d - 수신%d]"), nTotalLen, nRecvLen);
		return FALSE;
	}

	memcpy(pRxBuff, RxBuff+BINARY_HEADER_LEN, GetDataLength(nUnitType, wReadLen));

	return TRUE;
}

BOOL CMelsecEthernetSk::RecvWriteAck()
{
	BYTE RxBuff[2048];
	int nRecvLen;

	memset(RxBuff, 0x00, sizeof(RxBuff));
	if((nRecvLen = RecvFrame(RxBuff)) == FALSE)
		return FALSE;

	if(RxBuff[0] != 0xD0 || RxBuff[1] != 0x00)                //Subheader
	{
		m_strErrMsg.Format(_T("RecvReadAck.. Subheader 이상.."));
		return FALSE;
	}
	if(RxBuff[2] != 0x00 || RxBuff[3] != 0xFF)                // Network NO, PC NO
	{
		m_strErrMsg.Format(_T("RecvWriteAck.. Network or PC NO 이상.."));
		return FALSE;
	}
	if(RxBuff[4] != 0xFF || RxBuff[5] != 0x03 || RxBuff[6] != 0x00)   // Specific value
	{
		m_strErrMsg.Format(_T("RecvWriteAck.. Specific Value 이상.."));
		return FALSE;
	}

	WORD wErrNo = SwapToWord(RxBuff+9);
	if(wErrNo)
	{
		m_strErrMsg.Format(_T("RecvWriteAck.. 응답 수신.. 오류코드[%d]"), wErrNo);
		return FALSE;
	}

	return TRUE;
}

int CMelsecEthernetSk::RecvFrame(BYTE *pRxBuff)
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

	if (dwLen < BINARY_HEADER_LEN)
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

		if(nRealLen <= BINARY_HEADER_LEN)
		{
			m_strErrMsg.Format(_T("RecvFrame.. 수신 데이터 길이[%d] 이상"), nRealLen);
			return FALSE;
		}
	}

	m_bReceiving = FALSE;

	return nTotalLen;
}

WORD CMelsecEthernetSk::SwapToWord(BYTE *pSrc)
{
	return (WORD)(pSrc[1] << 8) | pSrc[0];
}

DWORD CMelsecEthernetSk::SwapToDWord(BYTE *pSrc)
{
	return (DWORD)(pSrc[3] << 24) | (pSrc[2] << 16) | (pSrc[1] << 8) | pSrc[0];
}


int CMelsecEthernetSk::GetDataLength(int nUnitType, WORD wLen)
{
	return (nUnitType == CMD_WORD_UNIT) ? (wLen*2) : ((wLen/2)+(wLen%2));
}
