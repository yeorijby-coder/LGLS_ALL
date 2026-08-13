// MelsecSerial.h: interface for the CMelsecSerialSk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MELSECSERIAL_H__CC12C514_47C5_4901_BA0A_35E4DCDD884A__INCLUDED_)
#define AFX_MELSECSERIAL_H__CC12C514_47C5_4901_BA0A_35E4DCDD884A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "InterfaceSk.h"
#include "Serial.h"

class CEquipment;
class CMelsecSerial : public CSerial
{
//	DECLARE_DYNAMIC(CMelsecSerial)

public:
	CMelsecSerial(CEquipment* pEquipment, int nIndex);
	virtual ~CMelsecSerial();

public:
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

public:
	BOOL Connect(CString strCommName,
		DWORD dwReadTimeOut = 500,
		DWORD dwBaudRate = (DWORD)9600,
		BYTE byByteSize = (BYTE)8,
		BYTE byParity = NOPARITY,
		BYTE byStopBits = ONESTOPBIT,
		BYTE byFlowCtrl = 0);

public:
//	virtual BOOL ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen);
//	virtual BOOL WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen);
	virtual BOOL CheckRequest(CByteArray& arrRxData);

protected:
	BOOL ResponseAck();
	BOOL ResponseNak();

public:
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

protected:
	static void CheckSum(BYTE* pBuff, int nLen, BYTE* pSumA, BYTE* pSumB);
	void CheckSum(CByteArray& arrData, int nLen, BYTE& rSumA, BYTE& rSumB);

public:
	BOOL	RecvData(BYTE* pRxData, CString& strData, int& nReadCnt);
	BOOL	RecvData(CString& strData);
	BOOL	RecvData(CByteArray& arrRxBuff);
	BOOL	SendData(const CString& strData);
	BOOL	SendData(CByteArray& arrTxBuff);
	void	WriteLog(LPCTSTR lpszLog);
	CString GetHexString(CByteArray& arrBuffer);
	BOOL Write(const BYTE* pTxBuff, int BaseAddr, int nWordCnt, int Type);

};

#endif // !defined(AFX_MELSECSERIAL_H__CC12C514_47C5_4901_BA0A_35E4DCDD884A__INCLUDED_)
