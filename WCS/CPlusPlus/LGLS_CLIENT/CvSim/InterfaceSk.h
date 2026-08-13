#if !defined(AFX_INTERFACESK_H__BF2499C5_67FA_445E_AA12_8950C04FD93C__INCLUDED_)
#define AFX_INTERFACESK_H__BF2499C5_67FA_445E_AA12_8950C04FD93C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterfaceSk.h : header file
//

#include "AsyncSocketEx.h"
/////////////////////////////////////////////////////////////////////////////
// CInterfaceSk command target

class CEquipment;
class CInterfaceSk : public CAsyncSocketEx
{
	DECLARE_DYNAMIC(CInterfaceSk)
// Operations
public:
	CInterfaceSk(CEquipment* pEquipment, int nIndex);
	virtual ~CInterfaceSk();

protected:
	CEquipment* m_pEquipment;

public:
	HANDLE m_hRecvEvent;	
	BOOL m_bRecvWaiting;	
	int m_nRecvTimeoutCount;
	int m_nIndex;

public:
	CString m_strRecv;
	CString m_strSend;

public:
	virtual BOOL RecvMsg(int nType, CString& strMsg) { return TRUE; }
	virtual BOOL SendMsg(int nType, const CString& strMsg) { return TRUE; }

public:
	virtual BOOL CheckRequest(CByteArray& arrRxData) { return TRUE; }
	virtual BOOL ResponseReadWord(CByteArray& arrTxData, int nLen) { return TRUE; }
	virtual BOOL ResponseWriteWord(CByteArray& arrTxData, int nLen) { return TRUE; }
	virtual BOOL ReadWord(CByteArray& arrRxData, int nStartWord, int nWordLen) { return TRUE; }
	virtual BOOL WriteWord(CByteArray& arrTxData, int nStartWord, int nWordLen) { return TRUE; }
	virtual BOOL Read(int nUnitType, BYTE *pRxBuff, BYTE DeviceCode, int nStartAddr, WORD wReadLen) { return TRUE; }
	virtual BOOL Write(int nUnitType, BYTE *pTxBuff, BYTE DeviceCode, int nStartAddr, WORD wWriteLen) { return TRUE; }

public:
	BOOL RecvWait();
	CString GetPeerIP();
	int GetPeerPort();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterfaceSk)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CInterfaceSk)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERFACESK_H__BF2499C5_67FA_445E_AA12_8950C04FD93C__INCLUDED_)
