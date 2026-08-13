#if !defined(AFX_INTERFACETHREADSK_H__BF2499C5_67FA_445E_AA12_8950C04FD93C__INCLUDED_)
#define AFX_INTERFACETHREADSK_H__BF2499C5_67FA_445E_AA12_8950C04FD93C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterfaceThreadSk.h : header file
//

//#include "InterfaceSk.h"
#include "AsyncSocketEx.h"

/////////////////////////////////////////////////////////////////////////////
// CInterfaceThreadSk command target

class CEquipment;
class CInterfaceThreadSk : public CAsyncSocketEx
{
	DECLARE_DYNAMIC(CInterfaceThreadSk)
// Operations
public:
	CInterfaceThreadSk(CEquipment* pEquipment);
	virtual ~CInterfaceThreadSk();
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

public:
	virtual BOOL RecvMsg(int nType, CString& strMsg) { return TRUE; }
	virtual BOOL SendMsg(int nType, const CString& strMsg) { return TRUE; }

public:
	BOOL SendData(const CString& strData, BOOL bMonitor = TRUE);
	BOOL SendData(CByteArray& arrTxBuff);

public:
	virtual BOOL ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen, BYTE bDeviceCode = enDeviceCodeD) { return TRUE; }
	virtual BOOL WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen, BYTE bDeviceCode = enDeviceCodeD) { return TRUE; }
	virtual BOOL Read(int nUnitType, BYTE *pRxBuff, BYTE DeviceCode, int nStartAddr, WORD wReadLen) { return TRUE; }
	virtual BOOL Write(int nUnitType, BYTE *pTxBuff, BYTE DeviceCode, int nStartAddr, WORD wWriteLen) { return TRUE; }

public:
	BOOL RecvWait();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterfaceThreadSk)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CInterfaceThreadSk)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERFACETHREADSK_H__BF2499C5_67FA_445E_AA12_8950C04FD93C__INCLUDED_)
