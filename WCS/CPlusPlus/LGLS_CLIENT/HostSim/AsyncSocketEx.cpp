// AsyncSocketEx.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "AsyncSocketEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAsyncSocketEx

IMPLEMENT_DYNAMIC(CAsyncSocketEx, CAsyncSocket)

CAsyncSocketEx::CAsyncSocketEx()
{
	m_strName = "";
	m_bWriteLog = FALSE;
	m_enCommStatus = enStatusClose;
}

CAsyncSocketEx::CAsyncSocketEx(LPCTSTR lpszName, BOOL bWriteLog)
{
	m_strName = lpszName;
	m_bWriteLog = bWriteLog;
	m_enCommStatus = enStatusClose;
}

CAsyncSocketEx::~CAsyncSocketEx()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CAsyncSocketEx, CAsyncSocket)
	//{{AFX_MSG_MAP(CAsyncSocketEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CAsyncSocketEx member functions

void CAsyncSocketEx::OnSend(int nErrorCode) 
{
	for (POSITION pos=m_strTxList.GetHeadPosition(); pos != NULL; )
	{
		if (!SendData(m_strTxList.GetNext(pos)))
			if (CAsyncSocket::GetLastError() == WSAEWOULDBLOCK)
				break;

		m_strTxList.RemoveHead();
	}
	
	CAsyncSocket::OnSend(nErrorCode);
}

/////////////////////////////////////////////////////////////////////////////

void CAsyncSocketEx::TokenRxFrame(CString& strRecv, TCHAR chSTX, TCHAR chETX, BOOL bIMS)
{
	CString strRxBuff;
	m_strRxList.RemoveAll();
	int nLength = strRecv.GetLength();
	for (int i=0; i<nLength; ++i)
	{
		if (((bIMS == TRUE) && (strRecv[i] == 'E') && (strRecv[i+1] == 'C') && (strRecv[i+2] == 'S')) || 
			((bIMS == FALSE) && (strRecv[i] == chSTX)) ) 
		{
			strRxBuff.Empty();						// 이전 데이터는 모두 버림
			strRxBuff += strRecv[i];
		}
		else if (strRecv[i] == chETX)
		{
			strRxBuff += strRecv[i];
			m_strRxList.AddTail(strRxBuff);			// CStringList에 추가함!
		}
		else
		{
			strRxBuff += strRecv[i];				// 나머지 값들은 모두 추가함!
		}
	}
}

BOOL CAsyncSocketEx::RecvData(CString& strData)
{
	DWORD dwLen = 0;
	if (!IOCtl(FIONREAD, &dwLen))
	{
		m_strLog.Format(_T("%s IOCtl Error! [LEN=%d] [MSG=%s]"), m_strName, dwLen, CLib::GetSystemErrMsg());
		return FALSE;
	}

	if (dwLen <= 0)
	{
		m_strLog.Format(_T("%s Invlid Data Length! [LEN=%d]"), m_strName, dwLen);
		return FALSE;
	}

	int nRecvLen = 0;
	CByteArray arrRxBuff;
	arrRxBuff.SetSize(dwLen+1);
	if ((nRecvLen = Receive(arrRxBuff.GetData(), dwLen)) != (int)dwLen)
	{
		m_strLog.Format(_T("%s 데이터 수신 실패! [MSG=%s] [DATA=%d, RECV=%d]"), m_strName, CLib::GetSystemErrMsg(), dwLen, nRecvLen);
		return FALSE;
	}

	strData.Format(_T("%s"), (LPCTSTR)arrRxBuff.GetData());

	if (m_bWriteLog)
	{
		CString strRecv;
		strRecv.Format(_T("[%s][%d][RECV] %s \r\n"), CTime::GetCurrentTime().Format(_T("%H:%M:%S")), ::GetCurrentThreadId(), strData);
		WriteLog(strRecv);
	}

	return TRUE;
}

BOOL CAsyncSocketEx::RecvData(CByteArray& arrRxBuff)
{
	DWORD dwLen = 0;
	if (!IOCtl(FIONREAD, &dwLen))
	{
		m_strLog.Format(_T("%s IOCtl Error! [LEN=%d] [MSG=%s]"), m_strName, dwLen, CLib::GetSystemErrMsg());
		return FALSE;
	}

	if (dwLen <= 0)
	{
		m_strLog.Format(_T("%s Invlid Data Length! [LEN=%d]"), m_strName, dwLen);
		return FALSE;
	}

	arrRxBuff.RemoveAll();
	arrRxBuff.SetSize(dwLen+1);
	int nRecvLen = Receive(arrRxBuff.GetData(), dwLen);
	if (nRecvLen != (int)dwLen)
	{
		m_strLog.Format(_T("%s 데이터 수신 실패! [MSG=%s] [DATA=%d, RECV=%d]"), m_strName, CLib::GetSystemErrMsg(), dwLen, nRecvLen);
		arrRxBuff.RemoveAll();
		return FALSE;
	}

	if (m_bWriteLog)
	{
		CString strRecv;
		strRecv.Format(_T("[%s][%d][RECV] %s \r\n"), CTime::GetCurrentTime().Format(_T("%H:%M:%S")), ::GetCurrentThreadId(), GetHexString(arrRxBuff));
		WriteLog(strRecv);
	}

	return TRUE;
}

BOOL CAsyncSocketEx::SendData(const CString& strData)
{
	int nErrorCode = 0;
	int nDataLen = strData.GetLength();
	int nSendLen = Send(strData, nDataLen);

	if (nSendLen == SOCKET_ERROR)
	{
		nErrorCode = CAsyncSocket::GetLastError();
//		if (nErrorCode == WSAEWOULDBLOCK)
//			m_strTxList.AddTail(strData);

		m_strLog.Format(_T("%s 데이터 전송 실패! [%s] [%s]"), m_strName, CLib::GetSystemErrMsg(nErrorCode), strData);
		return FALSE;
	}

	if (nSendLen != nDataLen)
	{
		m_strLog.Format(_T("%s 데이터 전송 실패! [전송데이터 길이 이상, DATA=%d, SEND=%d] [%s]"), m_strName, nDataLen, nSendLen, strData);
		return FALSE;
	}
	
	if (m_bWriteLog)
	{
		CString strSend;
		strSend.Format(_T("[%s][%d][SEND] %s \r\n"), CTime::GetCurrentTime().Format(_T("%H:%M:%S")), ::GetCurrentThreadId(), strData);
		WriteLog(strSend);
	}

	return TRUE;
}

BOOL CAsyncSocketEx::SendData(CByteArray& arrTxBuff)
{
	int nDataLen = arrTxBuff.GetSize();
	int nSendLen = Send(arrTxBuff.GetData(), nDataLen);

	if (nSendLen != nDataLen)
	{
		m_strLog.Format(_T("%s 데이터 전송 실패! [전송데이터 길이 이상, DATA=%d, SEND=%d] [%s]"), m_strName, nDataLen, nSendLen, (LPCTSTR)arrTxBuff.GetData());
		return FALSE;
	}

	if (m_bWriteLog)
	{
		CString strSend;
		strSend.Format(_T("[%s][%d][SEND] %s \r\n"), CTime::GetCurrentTime().Format(_T("%H:%M:%S")), ::GetCurrentThreadId(), GetHexString(arrTxBuff));
		WriteLog(strSend);
	}

	return TRUE;
}

void CAsyncSocketEx::WriteLog(LPCTSTR lpszLog)
{
	if (!m_bWriteLog)
		return;

	m_csSyncWriteLog.Lock();

	CString strFileName;
	strFileName.Format(_T("%s%s.LOG"), CTime::GetCurrentTime().Format(_T("%Y%m%d")), m_strName);

	TRY 
	{
		CFile f(strFileName, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);
		f.SeekToEnd();

		CArchive ar(&f, CArchive::store);
		ar.WriteString(lpszLog);
		ar.Close();

		f.Close();
	}
	CATCH (CException, e)
	{
		TCHAR szTemp[512] = {0};
		e->GetErrorMessage(szTemp, 512);
		m_strLog.Format(_T("%s %s [%s] [%s]"), m_strName, e->GetRuntimeClass()->m_lpszClassName, szTemp, lpszLog);
		DEBUGER_TRACE(m_strLog, _T("CAsyncSocketEx::WriteLog"));
	}
	END_CATCH

	m_csSyncWriteLog.Unlock();
}

CString CAsyncSocketEx::GetHexString(CByteArray& arrBuffer)
{
	CString strBuffer, strHex;
	for (int i=0; i<arrBuffer.GetSize(); ++i)
	{
		strHex.Format(_T("%02X "), arrBuffer[i]);
		strBuffer += strHex;
	}

	return strBuffer;
}

CString CAsyncSocketEx::GetPeerInfo()
{
	UINT uPort;
	CString strIP, strInfo;
	GetPeerName(strIP, uPort);
	strInfo.Format(_T("IP=%s, PORT=%d"), strIP, uPort);
	return strInfo;
}

CString CAsyncSocketEx::GetPeerIP()
{
	UINT uPort;
	CString strIP;
	GetPeerName(strIP, uPort);
	return strIP;
}

int CAsyncSocketEx::GetPeerPort()
{
	UINT uPort;
	CString strIP;
	GetPeerName(strIP, uPort);
	return uPort;
}