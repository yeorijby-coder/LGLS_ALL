// MelsecEthernetSk.h: interface for the CMelsecEthernetSk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MELSECETHERNETSK_H__75A1A516_64D6_47BB_944F_03256EC97FFE__INCLUDED_)
#define AFX_MELSECETHERNETSK_H__75A1A516_64D6_47BB_944F_03256EC97FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InterfaceSk.h"

#define CMD_WORD_UNIT		0x00
#define CMD_BIT_UNIT		0x01

#define	DEVICE_CODE_SM		0x91
#define	DEVICE_CODE_SD		0xA9
#define	DEVICE_CODE_X		0x9C
#define	DEVICE_CODE_Y		0x9D
#define	DEVICE_CODE_M		0x90
#define	DEVICE_CODE_L		0x92
#define	DEVICE_CODE_F		0x93
#define	DEVICE_CODE_V		0x94
#define	DEVICE_CODE_B		0xA0
#define	DEVICE_CODE_D		0xA8
#define	DEVICE_CODE_W		0xB4
#define	DEVICE_CODE_TS		0xC1
#define	DEVICE_CODE_TC		0xC0
#define	DEVICE_CODE_TN		0xC2
#define	DEVICE_CODE_SS		0xC7
#define	DEVICE_CODE_SC		0xC6
#define	DEVICE_CODE_SN		0xC8
#define	DEVICE_CODE_CS		0xC4
#define	DEVICE_CODE_CC		0xC3
#define	DEVICE_CODE_CN		0xC5
#define	DEVICE_CODE_SB		0xA1
#define	DEVICE_CODE_SW		0xB5
#define	DEVICE_CODE_S		0x98
#define	DEVICE_CODE_DX		0xA2
#define	DEVICE_CODE_DY		0xA3
#define	DEVICE_CODE_Z		0xCC
#define	DEVICE_CODE_R		0xAF
#define	DEVICE_CODE_ZR		0xB0

#define BINARY_HEADER_LEN	11


class CEquipment;
class CMelsecEthernetSk : public CInterfaceSk  
{
	DECLARE_DYNAMIC(CMelsecEthernetSk)

public:
	CMelsecEthernetSk(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enDeviceCodeR = 0xAF };
	enum { enBinaryHeaderLen = 11 };

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen, BYTE bDeviceCode = enDeviceCodeD);
	virtual BOOL WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen, BYTE bDeviceCode = enDeviceCodeD);

//	BOOL Write(int nUnitType, BYTE *pTxBuff, BYTE DeviceCode, int nStartAddr, WORD wWriteLen, BOOL bIsAscii = 0)
//	BOOL Read(int nUnitType, BYTE *pRxBuff, BYTE DeviceCode, int nStartAddr, WORD wReadLen, BOOL bIsAscii = 0)

// for RTV
public:
	BOOL Read(int nUnitType, BYTE *pRxBuff, BYTE DeviceCode, int nStartAddr, WORD wReadLen);
	BOOL Write(int nUnitType, BYTE *pTxBuff, BYTE DeviceCode, int nStartAddr, WORD wWriteLen);

	BOOL RecvReadAck(int nUnitType, BYTE *pRxBuff, WORD wReadCnt);
	BOOL RecvWriteAck();

	int RecvFrame(BYTE *pRxBuff);

	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

#endif // !defined(AFX_MELSECETHERNETSK_H__75A1A516_64D6_47BB_944F_03256EC97FFE__INCLUDED_)
