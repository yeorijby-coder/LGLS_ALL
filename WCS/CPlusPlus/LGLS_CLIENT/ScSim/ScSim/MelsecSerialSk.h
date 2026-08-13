// MelsecSerialSk.h: interface for the CMelsecSerialSk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MELSECSERIALSK_H__CC12C514_47C5_4901_BA0A_35E4DCDD884A__INCLUDED_)
#define AFX_MELSECSERIALSK_H__CC12C514_47C5_4901_BA0A_35E4DCDD884A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InterfaceSk.h"

class CEquipment;
class CMelsecSerialSk : public CInterfaceSk
{
	DECLARE_DYNAMIC(CMelsecSerialSk)

public:
	CMelsecSerialSk(CEquipment* pEquipment, int nIndex);
	virtual ~CMelsecSerialSk();

public:
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

public:
	virtual BOOL ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen);
	virtual BOOL WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen);

protected:
	virtual BOOL CheckRequest(CByteArray& arrRxData);
	BOOL ResponseAck();
	BOOL ResponseNak();

public:
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen);
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen);

protected:
	void CheckSum(CByteArray& arrData, int nLen, BYTE& rSumA, BYTE& rSumB);
};

#endif // !defined(AFX_MELSECSERIALSK_H__CC12C514_47C5_4901_BA0A_35E4DCDD884A__INCLUDED_)
