#pragma once


#define	MSG_LENGTH_HEADER				15

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

#define	MSG_LENGTH_HEADER				15
#define	MSG_LENGTH_RESPONSE_DATA		10
#define	MSG_LENGTH_RESPONSE_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_RESPONSE_DATA)
#define	MSG_LENGTH_COMPLETE_DATA		18		// 13
#define	MSG_LENGTH_COMPLETE_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_COMPLETE_DATA)
#define	MSG_LENGTH_ARRIVAL_DATA			34		// 12
#define	MSG_LENGTH_ARRIVAL_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_ARRIVAL_DATA)
#define	MSG_LENGTH_REQSTORE_DATA		17
#define	MSG_LENGTH_REQSTORE_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_REQSTORE_DATA)
#define	MSG_LENGTH_ERROR_DATA			32		// 23
#define	MSG_LENGTH_ERROR_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_ERROR_DATA)
#define	MSG_LENGTH_BARCODE_DATA			34		// 27
#define	MSG_LENGTH_BARCODE_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_BARCODE_DATA)
#define	MSG_LENGTH_PALLET_DATA			34		// 8
#define	MSG_LENGTH_PALLET_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_PALLET_DATA)
#define	MSG_LENGTH_CANCEL_DATA			9
#define	MSG_LENGTH_CANCEL_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_CANCEL_DATA)
#define	MSG_LENGTH_STO_REQ_DATA			22
#define	MSG_LENGTH_STO_REQ_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_STO_REQ_DATA)
#define	MSG_LENGTH_STATUS_DATA			48		// 35
#define	MSG_LENGTH_STATUS_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_STATUS_DATA)
//  STX(1) + MsgType(1) + Date(8) + Time(2) +(20 * SC_CNT) + ETX(1)
#define	MSG_LENGTH_THROUGHPUT_DATA		33
#define	MSG_LENGTH_THROUGHPUT_MSG		(MSG_LENGTH_HEADER + MSG_LENGTH_THROUGHPUT_DATA)
//	Lugg(4) + JobClass(1) + Loc1(7) + Loc2(7) + Product(20) + Qty(15) + Date(8) + Time(6) + TxFlag(2) + CRLF(2)
#define	MSG_LENGTH_OFFJOB_DATA			72
#define	MSG_LENGTH_OFFJOB_MSG			(MSG_LENGTH_HEADER + MSG_LENGTH_OFFJOB_DATA)


// CAsyncSocketEx 명령 대상입니다.
//
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


public:
	CStringList	m_strTxList;
	CStringList m_strRxList;

// Overrides
public:
	virtual void OnSend(int nErrorCode);

// Implementation
public:
	BOOL RecvData(CString& strData);
	BOOL RecvData(CByteArray& arrRxBuff);
	BOOL SendData(const CString& strData);
	BOOL SendData(CByteArray& arrTxBuff);
	void TokenRxFrame(CString& strRecv, TCHAR chSTX, TCHAR chETX);

protected:
	CCriticalSection m_csSyncWriteLog;
	void WriteLog(LPCTSTR lpszLog);

public:
	CString GetHexString(CByteArray& arrBuffer);
	CString GetPeerInfo();
	CString GetPeerIP();
	int GetPeerPort();
};


