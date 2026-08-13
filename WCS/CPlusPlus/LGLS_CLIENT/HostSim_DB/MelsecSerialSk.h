// MelsecSerialSk.h: interface for the CMelsecSerialSk class.
//

#pragma once

#include "InterfaceSk.h"

class CEquipment;


// CMelsecSerialSk
//
class CMelsecSerialSk : public CInterfaceSk  
{
	DECLARE_DYNAMIC(CMelsecSerialSk)

public:
	CMelsecSerialSk(CEquipment* pEquipment);
	virtual ~CMelsecSerialSk();

	enum { enDeviceCodeD = 0xA8 };

public:
	enum { 
		enSOH = 0x01, enSTX = 0x02, enETX = 0x03, enEOT = 0x04, enENQ = 0x05, 
		enACK = 0x06, enNAK = 0x15,  enLF = 0x0A,  enCR = 0x0D,  enGS = 0x1D
	};

public:
	virtual BOOL ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen, BYTE bDeviceCode = enDeviceCodeD);
	virtual BOOL WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen, BYTE bDeviceCode = enDeviceCodeD);

protected:
	BOOL ResponseAck();
	BOOL ResponseNak();

protected:
	void CheckSum(CByteArray& arrData, int nLen, BYTE& rSumA, BYTE& rSumB);
};
