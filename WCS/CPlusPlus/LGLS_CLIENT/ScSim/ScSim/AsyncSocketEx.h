#if !defined(AFX_ASYNCSOCKETEX_H__4AF52E4D_DDC8_4D08_8E3E_713DA7A51910__INCLUDED_)
#define AFX_ASYNCSOCKETEX_H__4AF52E4D_DDC8_4D08_8E3E_713DA7A51910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AsyncSocketEx.h : header file
//

//#define	MSG_LENGTH_HEADER				15

#define CMD_ORDER						'O'
#define CMD_ORDER_ANSWER				'o'
#define CMD_PALLET						'N'
#define CMD_PALLET_ANSWER				'n'
#define CMD_CANCEL						'D'
#define CMD_CANCEL_ANSWER				'd'
#define CMD_ALT_LOC						'R'
#define CMD_ALT_LOC_ANSWER				'r'
#define CMD_STATUS						'S'
#define CMD_STATUS_ANSWER				's'
#define CMD_ERROR						'E' 
#define CMD_ERROR_ANSWER				'e'
#define CMD_COMPLETE					'F' 
#define CMD_COMPLETE_ANSWER				'f'
#define CMD_REQ_STO						'T'
#define CMD_REQ_STO_ANSWER				't'
#define CMD_BARCODE						'B'
#define CMD_BARCODE_ANSWER				'b'
#define	CMD_DIRECT_BYPASS				'Q'
#define	CMD_DIRECT_BYPASS_ANSWER		'q'
#define	CMD_ROBOT_ADDJOB				'K'
#define	CMD_ROBOT_ADDJOB_ANSWER			'k'
#define CMD_RET_COMP					'C'
#define CMD_RET_COMP_ANSWER				'c'
#define	CMD_LOAD_ARRV					'A'
#define	CMD_LOAD_ARRV_ANSWER			'a'
#define CMD_CHANGE						'H'
#define CMD_CHANGE_ANSWER				'h'
#define	CMD_REQ_PAUSE					'P'
#define	CMD_REQ_PAUSE_ANSWER			'p'
#define	CMD_TIME_SET					'Y'
#define	CMD_TIME_SET_ANSWER				'y'
#define	CMD_OFFLINE_JOB					'J'
#define	CMD_OFFLINE_JOB_ANSWER			'j'
#define CMD_IN_OUT_REQUEST			    'L'
#define CMD_IN_OUT_REQUEST_ANSWER	    'l'
#define CMD_STORE_REQUEST			    'N'
#define CMD_STORE_REQUEST_ANSWER	    'n'
#define	CMD_UNPREDICT					'X'

#define	RECEIVE_OK						0
#define	MSG_NO_ERROR					0
#define	MSG_NO_STX						1
#define	MSG_NO_ETX						2
#define	MSG_LUGG_NO_DUPLICATED			3
#define	MSG_NO_EXIST_JOB				4
#define	MSG_ECS_BUFFER_FULL				5
#define	MSG_INVALID_HEADER_LENG			6
#define	MSG_INVALID_MSG_LENG			7
#define	MSG_INVALID_LOC					8
#define	MSG_INVALID_STN_NO				9
#define	MSG_INVALID_LUGG_NO				10
#define	MSG_UNKNOWN_JOB_TYPE			11
#define	MSG_INHIBITED_LOC				12
#define	MSG_INVALID_MAILBOX_NAME		13
#define	MSG_ALREADY_INVOKED_JOB			14
#define	MSG_INVALID_CONTENT				15	
#define	MSG_UNKNOWN_MSG_TYPE			16	
#define	MSG_IMPROPER_HANDSHAKE			17
#define	MSG_NO_RESPONSE					18
#define	MSG_SOCKET_ERROR				19
#define	MSG_INTERNAL_ERROR				99

#define	MSG_LENGTH_HEADER				52
// 반송지시
#define	MSG_LENGTH_REQUEST_DATA			49
#define	MSG_LENGTH_REQUEST_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_REQUEST_DATA)
#define	MSG_LENGTH_RESPONSE_DATA		10
#define	MSG_LENGTH_RESPONSE_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_RESPONSE_DATA)
// 완료보고
#define	MSG_LENGTH_COMPLETE_DATA		47
#define	MSG_LENGTH_COMPLETE_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_COMPLETE_DATA)
// 도착보고
#define	MSG_LENGTH_ARRIVAL_DATA			10
#define	MSG_LENGTH_ARRIVAL_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_ARRIVAL_DATA)
// 설비상태 에러보고
#define	MSG_LENGTH_ERROR_DATA			67
#define	MSG_LENGTH_ERROR_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_ERROR_DATA)
// 설비상태통지
#define	MSG_LENGTH_EQUIPSTATUS_DATA		100
#define	MSG_LENGTH_EQUIPSTATUS_MSG		(MSG_LENGTH_HEADER + MSG_LENGTH_EQUIPSTATUS_DATA)
#define	MSG_LENGTH_BARCODE_DATA			38
#define	MSG_LENGTH_BARCODE_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_BARCODE_DATA)
#define	MSG_LENGTH_PALLET_DATA			8
#define	MSG_LENGTH_PALLET_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_PALLET_DATA)
#define	MSG_LENGTH_CANCEL_DATA			9
#define	MSG_LENGTH_CANCEL_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_CANCEL_DATA)
#define	MSG_LENGTH_STO_REQ_DATA			22
#define	MSG_LENGTH_STO_REQ_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_STO_REQ_DATA)
// 스태커 상태보고
#define	MSG_LENGTH_STATUS_DATA			50
#define	MSG_LENGTH_STATUS_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_STATUS_DATA)
// 온습도 정보 보고
#define	MSG_LENGTH_THS_DATA				312
#define	MSG_LENGTH_THS_MSG				(MSG_LENGTH_HEADER + MSG_LENGTH_THS_DATA)
// 동간 TRAY 대기 수량 보고
#define	MSG_LENGTH_EMPTY_TRAY_DATA		10
#define	MSG_LENGTH_EMPTY_TRAY_MSG		(MSG_LENGTH_HEADER + MSG_LENGTH_EMPTY_TRAY_DATA)
// DCIR TRAY 대기 수량 보고
#define	MSG_LENGTH_DCIR_TRAY_DATA		10
#define	MSG_LENGTH_DCIR_TRAY_MSG		(MSG_LENGTH_HEADER + MSG_LENGTH_DCIR_TRAY_DATA)
//  STX(1) + MsgType(1) + Date(8) + Time(2) +(20 * SC_CNT) + ETX(1)
#define	MSG_LENGTH_THROUGHPUT_DATA		33
#define	MSG_LENGTH_THROUGHPUT_MSG		(MSG_LENGTH_HEADER + MSG_LENGTH_THROUGHPUT_DATA)
//	Lugg(4) + JobClass(1) + Loc1(7) + Loc2(7) + Product(20) + Qty(15) + Date(8) + Time(6) + TxFlag(2) + CRLF(2)
#define	MSG_LENGTH_OFFJOB_DATA			72
#define	MSG_LENGTH_OFFJOB_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_OFFJOB_DATA)



/////////////////////////////////////////////////////////////////////////////
// CAsyncSocketEx command target

class CAsyncSocketEx : public CAsyncSocket
{
	DECLARE_DYNAMIC(CAsyncSocketEx)
// Operations
public:
	CAsyncSocketEx();
	CAsyncSocketEx(LPCTSTR lpszName, BOOL bWriteLog);
	virtual ~CAsyncSocketEx();

public:
	enum EN_STATUS { enStatusClose, enStatusConnecting, enStatusConnectOK, enStatusConnectFail, enStatusSize };
	EN_STATUS m_enCommStatus;
	CString m_strName;
	CString m_strLog;
	BOOL m_bWriteLog;

	CCriticalSection m_csSyncSendList;

protected:
	CStringList	m_strTxList;
	CStringList m_strRxList;

public:
	BOOL RecvData(CString& strData);
	BOOL RecvData(CByteArray& arrRxBuff);
	BOOL SendData(const CString& strData);
	BOOL SendData(CByteArray& arrTxBuff);
	void TokenRxFrame(CString& strRecv, TCHAR chSTX, TCHAR chETX);

protected:
	CCriticalSection m_csSyncSocketLog;
	void WriteLog(LPCTSTR lpszLog);

public:
	CString GetHexString(CByteArray& arrBuffer);
	CString GetPeerInfo();
	CString GetPeerIP();
	int GetPeerPort();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsyncSocketEx)
	public:
	virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CAsyncSocketEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASYNCSOCKETEX_H__4AF52E4D_DDC8_4D08_8E3E_713DA7A51910__INCLUDED_)
