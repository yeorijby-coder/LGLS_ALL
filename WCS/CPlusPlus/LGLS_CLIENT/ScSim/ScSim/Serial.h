// Serial.h : Defines the class behaviors for the application.

#ifndef __SERIAL_H__
#define __SERIAL_H__

#define FC_DTRDSR       0x01
#define FC_RTSCTS_H     0x02
#define FC_RTSCTS_T     0x04
#define FC_XONXOFF      0x08

#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

class CEquipment;
class CSerial
{
public:
	CSerial(CEquipment* pEquipment, int nIndex);
	~CSerial();
	
protected:
	CEquipment* m_pEquipment;
	CCriticalSection m_csSerialIO; // 동기화 객체 추가

public:
	enum {Live, Dead, Init} m_Line;	

	HANDLE		m_hCommDev;			// File handle for communication port
	CString		m_strCommName;		// Device Name
	DWORD		m_dwBaudRate;
	BYTE		m_byFlowCtrl;
	BYTE		m_byByteSize;
	BYTE		m_byParity;
	BYTE 		m_byStopBits;
	COMSTAT		m_Comstat;
	DWORD		m_CommError;
	DWORD		m_dwInQue, m_dwOutQue;
	OVERLAPPED	m_osWrite, m_osRead;

	COMMTIMEOUTS	m_CommTimeOuts;

public:
	CString		m_strErrMsg;
	DWORD		m_dwErrCode;
	CString		m_strLog;

public:
	void SetConfig(CString strComName,
			   DWORD dwBaudRate = (DWORD)9600,
		       BYTE byByteSize = (BYTE)8,
		       BYTE byParity = NOPARITY,
		       BYTE byStopBits = ONESTOPBIT,
		       BYTE byFlowCtrl = 0,
			   DWORD ReadTotalTimeoutConstant=500);

	void InitSerial();
	BOOL Setup(void);
	BOOL Open(void);
	void Close(void);
	BOOL PurgeComm(DWORD dwAction = PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	BOOL IsLive() { return (m_Line == Live) ? TRUE : FALSE; };
	BOOL ClearCommError(void);
	BOOL GetOverlappedResult(LPOVERLAPPED lplap, LPDWORD lpNumOfBytes,BOOL bWait=TRUE);
	BOOL SetCommMask(DWORD dwEvtMask);
	BOOL GetCommMask(LPDWORD lpdwEvtMask);
	BOOL EscapeCommFunction(DWORD dwFunc);
	BOOL WaitCommEvent(LPDWORD lpfdwEvtMask, LPOVERLAPPED lpo);

	HANDLE GetCommDev() { return m_hCommDev; }

public:
	virtual BOOL Recv(BYTE* pRxBuff, DWORD nLen);
	virtual BOOL Send(BYTE* pTxBuff, DWORD nLen);

	BOOL RecvUntilLF(BYTE* pRxBuff, DWORD nMaxLen, DWORD* pActualLen);
	BOOL SendUntilAck(const BYTE* pTxBuff, DWORD nLen, BYTE ackByte, DWORD timeoutMs);

};

#endif // __SERIAL_H__