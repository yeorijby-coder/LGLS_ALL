// ByteArrayEx.cpp: implementation of the CByteArrayEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "ByteArrayEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CByteArrayEx::CByteArrayEx(int nStartWordAddr, int nTotalWordSize, BOOL bFxForSeries)
{
	m_nStartWordAddr = nStartWordAddr;
	m_nTotalWordSize = nTotalWordSize;

//	CByteArray::SetSize(m_nTotalWordSize*2);

	int nTemp = m_nTotalWordSize;
	if (bFxForSeries == FALSE)
		nTemp = m_nTotalWordSize * 2;

	CByteArray::SetSize(nTemp);
}

CByteArrayEx::~CByteArrayEx()
{

}

void CByteArrayEx::SetArray(int nStartWordAddr, int nTotalWordSize)
{
//	DEBUGER_ASSERT_VALID(nTotalWordSize*2 == m_nSize);

	m_nStartWordAddr = nStartWordAddr;
	m_nTotalWordSize = nTotalWordSize;
}

int CByteArrayEx::GetIndex(int nWordAddr)
{
	int nIndex = (nWordAddr - m_nStartWordAddr) * 2;
//	DEBUGER_ASSERT_RANGE(nIndex, m_nSize);
	return nIndex;
}

WORD CByteArrayEx::GetWord(int nWordAddr)
{
	int nIndex = GetIndex(nWordAddr);
	WORD wWord = WORD(m_pData[nIndex+1] << 8) | WORD(m_pData[nIndex]);
	return wWord;	
}

BYTE CByteArrayEx::GetByte(int nWordAddr, EN_BYTE enByte)
{
	BYTE ucShift = 0;
	WORD wWord = GetWord(nWordAddr);
	switch (enByte)
	{
	case enByteL:		ucShift = 0;	break;
	case enByteH:		ucShift = 8;	break;
	}
	return (wWord & enByte) >> ucShift;
}

BYTE CByteArrayEx::GetNibble(int nWordAddr, EN_NIBBLE enNibble)
{
	BYTE ucShift = 0;
	WORD wWord = GetWord(nWordAddr);
	switch (enNibble)
	{
	case enNibble1:		ucShift = 0;	break;
	case enNibble2:		ucShift = 4;	break;
	case enNibble3:		ucShift = 8;	break;
	case enNibble4:		ucShift = 12;	break;
	}

	return (wWord & enNibble) >> ucShift;
}

BOOL CByteArrayEx::GetBit(int nWordAddr, EN_BIT enBit)
{
	WORD wWord = GetWord(nWordAddr);
	return (wWord & enBit) ? TRUE : FALSE;
}

void CByteArrayEx::SetWord(int nWordAddr, WORD wWord)
{
	int nIndex = GetIndex(nWordAddr);
	m_pData[nIndex+0] = BYTE(wWord & enByteL);
	m_pData[nIndex+1] = BYTE((wWord & enByteH) >> 8);
}

void CByteArrayEx::SetByte(int nWordAddr, EN_BYTE enByte, BYTE ucByte)
{
	int nIndex = GetIndex(nWordAddr);
	if (enByte == enByteH) ++nIndex;
	m_pData[nIndex] = ucByte;
}

void CByteArrayEx::SetNibble(int nWordAddr, EN_NIBBLE enNibble, BYTE ucNibble)
{
	BYTE ucShift = 0;
	int nIndex = GetIndex(nWordAddr);
	if (enNibble == enNibble3 || enNibble == enNibble4) ++nIndex;
	if (enNibble == enNibble2 || enNibble == enNibble4) ucShift = 4;
	m_pData[nIndex] |= (ucNibble << ucShift);
}

void CByteArrayEx::SetBit(int nWordAddr, EN_BIT enBit, BOOL bBit)
{
	int nIndex = GetIndex(nWordAddr);
	if (enBit & 0xFF00) ++nIndex;
	if (bBit) m_pData[nIndex] |= enBit;
	else m_pData[nIndex] &= ~enBit;
}

void CByteArrayEx::WriteDump(LPCTSTR lpszFileName)
{
	CString strKey, strValue;
	int i, nWordAddr = m_nStartWordAddr;
	for (i=0; i<m_nTotalWordSize; ++i)
	{
		strKey.Format(_T("D%.3d"), nWordAddr+i);
		strValue.Format(_T("%X"), GetWord(nWordAddr+i));
		::WritePrivateProfileString(_T("DeviceMap"), strKey, strValue, lpszFileName);
	}
}
