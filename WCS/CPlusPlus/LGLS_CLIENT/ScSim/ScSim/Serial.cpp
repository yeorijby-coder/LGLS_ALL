// EcsSrial.cpp : implementation of the CEcsSrial class
//

#include "stdafx.h"
#include "Serial.h"
#include "Lib.h"
#include "EcsEnv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
// CSerial

CSerial::CSerial(CEquipment* pEquipment, int nIndex)
{
    m_pEquipment = pEquipment;
    m_strCommName = _T("");
    m_dwBaudRate = 9600;
    m_byByteSize = 8;
    m_byParity = 0;
    m_byStopBits = 0;
    m_byFlowCtrl = 0;
    m_hCommDev = INVALID_HANDLE_VALUE;
    m_Line = Dead;
    m_dwInQue = 256;
    m_dwOutQue = 256;
    m_dwErrCode = TRUE;
    m_strErrMsg = "";

    ZeroMemory(&m_osWrite, sizeof(OVERLAPPED));
    ZeroMemory(&m_osRead, sizeof(OVERLAPPED));
    ZeroMemory(&m_Comstat, sizeof(COMSTAT));
    ZeroMemory(&m_CommTimeOuts, sizeof(COMMTIMEOUTS));
}


CSerial::~CSerial()
{
    if (m_hCommDev != INVALID_HANDLE_VALUE) Close();
}

void CSerial::InitSerial()
{
    m_byFlowCtrl = 1;
    m_byByteSize = 1;
    m_byParity = 1;
    m_byStopBits = 1;
    m_dwInQue = 1;
    m_dwOutQue = 1;
    m_dwErrCode = 1;
    m_dwBaudRate = 1;
    m_strLog = "";
    m_strErrMsg = "";
    m_strCommName = "";
}

void CSerial::SetConfig(CString strComName, DWORD dwBaudRate, BYTE byByteSize, BYTE byParity, BYTE byStopBits, BYTE byFlowCtrl, DWORD ReadTotalTimeoutConstant)
{
    m_strCommName = strComName;
    m_dwBaudRate = dwBaudRate;
    m_byByteSize = byByteSize;
    m_byParity = byParity;
    m_byStopBits = byStopBits;
    m_byFlowCtrl = byFlowCtrl;
}

BOOL CSerial::Open(void)
{
    m_hCommDev = CreateFile(m_strCommName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (m_hCommDev == INVALID_HANDLE_VALUE) {
        m_strErrMsg = _T("Device file create fail");
        return FALSE;
    }
    m_Line = Live;
    return TRUE;
}

void CSerial::Close(void)
{
    if (m_hCommDev != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hCommDev);
        m_hCommDev = INVALID_HANDLE_VALUE;
    }
    m_Line = Dead;
}

BOOL CSerial::Send(BYTE* pTxBuff, DWORD nLen)
{
    if (m_hCommDev == INVALID_HANDLE_VALUE || nLen <= 0) return FALSE;
    DWORD dwWritten = 0;
    if (!WriteFile(m_hCommDev, pTxBuff, nLen, &dwWritten, NULL)) {
        m_strErrMsg = _T("WriteFile failed");
        return FALSE;
    }
    return (dwWritten == nLen);
}


BOOL CSerial::Recv(BYTE* pRxBuff, DWORD nLen)
{
    if (m_hCommDev == INVALID_HANDLE_VALUE || nLen <= 0) return FALSE;
    DWORD dwRead = 0;
    DWORD dwTotalRead = 0;
    int retry = 0;
    const int MAX_RETRY = 500; // 충분한 재시도
    while (dwTotalRead < nLen && retry < MAX_RETRY)
    {
        BOOL bResult = ReadFile(m_hCommDev, pRxBuff + dwTotalRead, nLen - dwTotalRead, &dwRead, NULL);
        if (!bResult)
        {
            DWORD dwErr = GetLastError();
            if (dwErr == ERROR_IO_PENDING || dwErr == ERROR_IO_INCOMPLETE)
            {
                Sleep(20);
                ++retry;
                continue;
            }
            m_strErrMsg.Format(_T("ReadFile failed: %d"), dwErr);
            return FALSE;
        }
        if (dwRead == 0)
        {
            Sleep(20);
            ++retry;
            continue;
        }
        dwTotalRead += dwRead;
    }
    if (dwTotalRead < nLen)
    {
        m_strErrMsg.Format(_T("Recv timeout: %lu/%lu bytes received"), dwTotalRead, nLen);
        return FALSE;
    }
    return TRUE;
}

BOOL CSerial::RecvUntilLF(BYTE* pRxBuff, DWORD nMaxLen, DWORD* pActualLen)
{
    if (m_hCommDev == INVALID_HANDLE_VALUE || nMaxLen == 0) return FALSE;
    DWORD dwRead = 0, dwTotalRead = 0;
    int retry = 0;
    const int MAX_RETRY = 500;
    BOOL foundLF = FALSE;
    while (dwTotalRead < nMaxLen && retry < MAX_RETRY)
    {
        BOOL bResult = ReadFile(m_hCommDev, pRxBuff + dwTotalRead, 1, &dwRead, NULL); // 1바이트씩 읽기
        if (!bResult)
        {
            DWORD dwErr = GetLastError();
            if (dwErr == ERROR_IO_PENDING || dwErr == ERROR_IO_INCOMPLETE)
            {
                Sleep(10);
                ++retry;
                continue;
            }
            m_strErrMsg.Format(_T("ReadFile failed: %d"), dwErr);
            return FALSE;
        }
        if (dwRead == 0)
        {
            Sleep(10);
            ++retry;
            continue;
        }
        if (pRxBuff[dwTotalRead] == 0x0A) // LF
        {
            foundLF = TRUE;
            ++dwTotalRead;
            break;
        }
        ++dwTotalRead;
    }

    if (pRxBuff[5] == 'W' &&
        pRxBuff[6] == 'W' &&
        pRxBuff[7] == '0' &&
        pRxBuff[8] == 'D')
    {
        if (pActualLen) *pActualLen = dwTotalRead;
        return TRUE;
    }

    if (pActualLen) *pActualLen = dwTotalRead;
    return foundLF;
}

BOOL CSerial::SendUntilAck(const BYTE* pTxBuff, DWORD nLen, BYTE ackByte, DWORD timeoutMs)
{
    if (m_hCommDev == INVALID_HANDLE_VALUE || nLen == 0) return FALSE;
    DWORD dwWritten = 0;
    if (!WriteFile(m_hCommDev, pTxBuff, nLen, &dwWritten, NULL)) {
        m_strErrMsg = _T("WriteFile failed");
        return FALSE;
    }
    if (dwWritten != nLen) {
        m_strErrMsg = _T("WriteFile incomplete");
        return FALSE;
    }
    // ACK 수신 대기
    BYTE rxBuff[8] = {0};
    DWORD dwRead = 0;
    DWORD elapsed = 0;
    const DWORD pollInterval = 10;
    while (elapsed < timeoutMs) {
        BOOL bResult = ReadFile(m_hCommDev, rxBuff, 1, &dwRead, NULL);
        if (bResult && dwRead == 1) {
            if (rxBuff[0] == ackByte) 
                return TRUE;
        }
        Sleep(pollInterval);
        elapsed += pollInterval;
    }
    m_strErrMsg = _T("ACK not received");
    return FALSE;
}
