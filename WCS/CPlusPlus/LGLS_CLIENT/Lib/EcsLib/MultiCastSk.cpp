
#include "stdafx.h"

#include <afxsock.h>
#include "MulticastSk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	STX				0x02
#define	ETX				0x03

//////////////////////////////////////////
// CMultiCastSk

CMultiCastSk::CMultiCastSk()
{
	m_strLocalIP = _T("");
	m_nLocalPort = 0;
	m_strLocalHost = _T("");

	m_strSendersIP = _T("");
	m_nSendersPort = 0;

	memset(&m_saHostGroup, 0, sizeof(m_saHostGroup));
}

CMultiCastSk::~CMultiCastSk()
{

}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMultiCastSk, CAsyncSocket)
	//{{AFX_MSG_MAP(CMultiCastSk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

//////////////////////////////////////////
// CMultiCastSk member functions
void CMultiCastSk::OnReceive(int nErrorCode)
{
	CAsyncSocket::OnReceive(nErrorCode); 

	if (nErrorCode)		return;

	DWORD dwLen = 0;
	if(IOCtl( FIONREAD, &dwLen ) == FALSE)
		return;

	BYTE *pRxBuff = new BYTE[dwLen+1];
	ZeroMemory(pRxBuff, dwLen+1);

	int nRecvLen = ReceiveFrom(pRxBuff, dwLen, m_strSendersIP, m_nSendersPort);

	if(nRecvLen == SOCKET_ERROR || nRecvLen == 0)
	{
		delete pRxBuff;
		return;
	}

	CStringList strRxBuffList;
	TokenRxFrame(strRxBuffList, pRxBuff, dwLen);
	delete pRxBuff;

	for (POSITION pos = strRxBuffList.GetHeadPosition(); pos != NULL;)
	{
		CString strRxTemp = strRxBuffList.GetNext(pos);

		if (strRxTemp[0] != STX)
		{
			continue;
		}

		if (strRxTemp[strRxTemp.GetLength()-1] != ETX)
		{
			continue;
		}

		CString strRxBuff = strRxTemp.Mid(1, strRxTemp.GetLength()-2);
		Parsing(strRxBuff);
	}
}

BOOL CMultiCastSk::SendMsg(CString strTxBuff, int nTxLen)
{
	if(SendTo(strTxBuff, nTxLen, (SOCKADDR*)&m_saHostGroup, sizeof(SOCKADDR), 0) == SOCKET_ERROR)
		return FALSE;

	return TRUE;
}

BOOL CMultiCastSk::RecverJoinGroup(CString strGroupIP, UINT nGroupPort, int nRecvSize)
{
	if(!Create(nGroupPort, SOCK_DGRAM, FD_READ))	return FALSE;

	// Join the multicast group
	m_mrMReq.imr_multiaddr.s_addr = inet_addr(CT2A(strGroupIP));	// group addr 
	m_mrMReq.imr_interface.s_addr = htons(INADDR_ANY);		// use default
	if(setsockopt(m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR *)&m_mrMReq, sizeof(m_mrMReq)) == SOCKET_ERROR )
		return FALSE;

	if ( nRecvSize != 0 )
		SetSocketBufferSize(SO_RCVBUF, &nRecvSize);

	GetLocalPortIP();

	return TRUE;
}

BOOL CMultiCastSk::RecverLeaveGroup()
{
	if(setsockopt (m_hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char FAR *)&m_mrMReq, sizeof(m_mrMReq)) < 0)
		return FALSE;

	Close();
	return TRUE;
}

BOOL CMultiCastSk::SenderSocket(CString strGroupIP, UINT nGroupPort, UINT nTTL, BOOL bLoopBack, int nSendSize)
{
	if ( !Create(0, SOCK_DGRAM, 0) )		return FALSE;

	if ( SetTTL(nTTL) == FALSE )			return FALSE;

	SetLoopBack(bLoopBack);

	SetGroupPortIP(strGroupIP, nGroupPort);

	if ( nSendSize != 0 )
		SetSocketBufferSize(SO_SNDBUF, &nSendSize);

	GetLocalPortIP();

	return TRUE;
}

BOOL CMultiCastSk::GetSocketBufferSize(int nOptionName, int *pOptionValue)
{
	int nOptionLen = sizeof(int);
	return GetSockOpt(nOptionName, pOptionValue, &nOptionLen);
}

BOOL CMultiCastSk::SetSocketBufferSize(int nOptionName, int *pOptionValue)
{
	return SetSockOpt(nOptionName, pOptionValue, sizeof(int));
}

BOOL CMultiCastSk::SetTTL(UINT nTTL)
{
	return SetSockOpt(IP_MULTICAST_TTL, &nTTL, sizeof(int), IPPROTO_IP);
}

void CMultiCastSk::SetLoopBack(BOOL bLoop)
{
	int nLoopBack = (int)bLoop;
	SetSockOpt(IP_MULTICAST_LOOP, &nLoopBack, sizeof(int), IPPROTO_IP);
}

void CMultiCastSk::GetLocalPortIP()
{
	char szLocalHost[512];
	if ( gethostname(szLocalHost, 511) == SOCKET_ERROR  )	return;
	m_strLocalHost = szLocalHost;

	struct hostent *host = gethostbyname(szLocalHost);
	if ( host == NULL )		return;

	m_strLocalIP = inet_ntoa (*(struct in_addr*)*host->h_addr_list);

	CString strTemp;
	GetSockName(strTemp, m_nLocalPort);		/* Get Port Number for Sending Port */
}

void CMultiCastSk::SetGroupPortIP(CString strGroupIP, UINT nGroupPort)
{
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
	// Multicast IP BAND 224.2.0.0 ... 239.255.255.255 //
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
	m_saHostGroup.sin_family = AF_INET;
	m_saHostGroup.sin_addr.s_addr = inet_addr(CT2A(strGroupIP));
	m_saHostGroup.sin_port = htons((USHORT)nGroupPort);
}

void CMultiCastSk::TokenRxFrame(CStringList &strRxBuffList, BYTE *pFrame, int dwLen)
{
	CString strTemp;
	for( int i = 0; i < dwLen; i++ )
	{
		if ( pFrame[i] == NULL )	pFrame[i] = 0x30;
		strTemp += pFrame[i];

		if ( pFrame[i] == ETX )
		{
			strRxBuffList.AddTail(strTemp);
			strTemp = _T("");
		}
	}

	// 마지막이 ETX가 아닌 경우
	if ( strTemp.IsEmpty() == FALSE )
		strRxBuffList.AddTail(strTemp);
}

