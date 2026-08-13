// MelsecEthernetSk.h: interface for the CMelsecEthernetSk class.
//
//////////////////////////////////////////////////////////////////////

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
	CMelsecEthernetSk(CEquipment* pEquipment, int nIndex);
	virtual ~CMelsecEthernetSk();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen);
	virtual BOOL WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen);

	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

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
/*
class CMelsecEthernetSk2 : public CInterfaceSk2
{
	DECLARE_DYNAMIC(CMelsecEthernetSk2)

public:
	CMelsecEthernetSk2(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk2();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	
	int RecvFrame(BYTE *pRxBuff);

	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk3 : public CInterfaceSk3
{
	DECLARE_DYNAMIC(CMelsecEthernetSk3)

public:
	CMelsecEthernetSk3(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk3();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk4 : public CInterfaceSk4
{
	DECLARE_DYNAMIC(CMelsecEthernetSk4)

public:
	CMelsecEthernetSk4(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk4();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk5 : public CInterfaceSk5
{
	DECLARE_DYNAMIC(CMelsecEthernetSk5)

public:
	CMelsecEthernetSk5(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk5();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk6 : public CInterfaceSk6
{
	DECLARE_DYNAMIC(CMelsecEthernetSk6)

public:
	CMelsecEthernetSk6(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk6();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk7 : public CInterfaceSk7
{
	DECLARE_DYNAMIC(CMelsecEthernetSk7)

public:
	CMelsecEthernetSk7(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk7();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk8 : public CInterfaceSk8
{
	DECLARE_DYNAMIC(CMelsecEthernetSk8)

public:
	CMelsecEthernetSk8(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk8();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk9 : public CInterfaceSk9
{
	DECLARE_DYNAMIC(CMelsecEthernetSk9)

public:
	CMelsecEthernetSk9(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk9();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk10 : public CInterfaceSk10
{
	DECLARE_DYNAMIC(CMelsecEthernetSk10)

public:
	CMelsecEthernetSk10(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk10();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk11 : public CInterfaceSk11
{
	DECLARE_DYNAMIC(CMelsecEthernetSk11)

public:
	CMelsecEthernetSk11(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk11();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk12 : public CInterfaceSk12
{
	DECLARE_DYNAMIC(CMelsecEthernetSk12)

public:
	CMelsecEthernetSk12(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk12();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk13 : public CInterfaceSk13
{
	DECLARE_DYNAMIC(CMelsecEthernetSk13)

public:
	CMelsecEthernetSk13(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk13();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk14 : public CInterfaceSk14
{
	DECLARE_DYNAMIC(CMelsecEthernetSk14)

public:
	CMelsecEthernetSk14(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk14();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk15 : public CInterfaceSk15
{
	DECLARE_DYNAMIC(CMelsecEthernetSk15)

public:
	CMelsecEthernetSk15(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk15();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk16 : public CInterfaceSk16
{
	DECLARE_DYNAMIC(CMelsecEthernetSk16)

public:
	CMelsecEthernetSk16(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk16();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk17 : public CInterfaceSk17
{
	DECLARE_DYNAMIC(CMelsecEthernetSk17)

public:
	CMelsecEthernetSk17(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk17();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};

class CMelsecEthernetSk18 : public CInterfaceSk18
{
	DECLARE_DYNAMIC(CMelsecEthernetSk18)

public:
	CMelsecEthernetSk18(CEquipment* pEquipment);
	virtual ~CMelsecEthernetSk18();

public:
	enum { enCmdWordUnit = 0x00, enCmdBitUnit = 0x01 };
	enum { enDeviceCodeD = 0xA8 };
	enum { enBinaryHeaderLen = 11 };
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

	BOOL m_bReceiving;
	CString m_strErrMsg;
public:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

// for RTV
public:
	WORD SwapToWord(BYTE *pSrc);
	DWORD SwapToDWord(BYTE *pSrc);

	int GetDataLength(int nUnitType, WORD wLen);

};
//*/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//#endif // !defined(AFX_MELSECETHERNETSK_H__75A1A516_64D6_47BB_944F_03256EC97FFE__INCLUDED_)
