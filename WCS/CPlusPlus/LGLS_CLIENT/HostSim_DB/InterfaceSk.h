// InterfaceSk.h : header file
//

#pragma once

#include "AsyncSocketEx.h"

class CEquipment;

// CInterfaceSk
//
class CInterfaceSk : public CAsyncSocketEx
{
	DECLARE_DYNAMIC(CInterfaceSk)
// Operations
public:
	CInterfaceSk(CEquipment* pEquipment);
	virtual ~CInterfaceSk();
	enum { enDeviceCodeD = 0xA8 };

protected:
	CEquipment* m_pEquipment;

public:
	HANDLE m_hRecvEvent;	
	BOOL m_bRecvWaiting;	
	int m_nRecvTimeoutCount;

public:
	CString m_strRecv;
	CString m_strSend;

// Overrides
public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	virtual BOOL RecvMsg(int nType, CString& strMsg) { return TRUE; }
	virtual BOOL SendMsg(int nType, const CString& strMsg) { return TRUE; }

public:
	virtual BOOL ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen, BYTE bDeviceCode = enDeviceCodeD) { return TRUE; }
	virtual BOOL WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen, BYTE bDeviceCode = enDeviceCodeD) { return TRUE; }
	virtual BOOL Read(int nUnitType, BYTE *pRxBuff, BYTE DeviceCode, int nStartAddr, WORD wReadLen) { return TRUE; }
	virtual BOOL Write(int nUnitType, BYTE *pTxBuff, BYTE DeviceCode, int nStartAddr, WORD wWriteLen) { return TRUE; }

// Implementation
public:
	BOOL RecvWait();
};
