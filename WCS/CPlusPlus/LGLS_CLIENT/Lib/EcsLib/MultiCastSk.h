
#ifndef __MULTICASTSK_H__
#define __MULTICASTSK_H__

#define SENDER_MULTICAST	1
#define RECVER_MULTICAST	2

class AFX_EXT_CLASS CMultiCastSk : public CAsyncSocket
{
public:
	CMultiCastSk();
	virtual ~CMultiCastSk();

protected:
	SOCKADDR_IN m_saHostGroup;
	ip_mreq m_mrMReq;

public:
	CString m_strSendersIP;
	UINT m_nSendersPort;

	CString m_strLocalIP;
	UINT m_nLocalPort;
	CString m_strLocalHost;

public:
	BOOL RecverJoinGroup(CString strGroupIP, UINT nGroupPort, int nRecvSize);
	BOOL RecverLeaveGroup();
	BOOL SenderSocket(CString strGroupIP, UINT nGroupPort, UINT nTTL, BOOL bLoopBack, int nSendSize);

	void SetLoopBack(BOOL);
	BOOL SetTTL(UINT nTTL);

	void GetLocalPortIP();
	BOOL SendMsg(CString strTxBuff, int nTxLen);

	void TokenRxFrame(CStringList &strRxBuffList, BYTE *pFrame, int dwLen);
	BOOL GetSocketBufferSize(int nOptionName, int *pOptionValue);
	BOOL SetSocketBufferSize(int nOptionName, int *pOptionValue);

protected:
	void SetGroupPortIP(CString strGroupIP, UINT nGroupPort);

public:
	virtual void Parsing(CString strRxBuff) {};

public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiCastSk)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMultiCastSk)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

};

#endif
