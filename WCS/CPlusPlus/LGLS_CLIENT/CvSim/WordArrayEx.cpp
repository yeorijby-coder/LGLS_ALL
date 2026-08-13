// WordArrayEx.cpp: implementation of the CByteArrayEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "WordArrayEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWordArrayEx::CWordArrayEx()
{
	m_nStartWordAddr = 0;
	m_nTotalWordSize = 0;

	CWordArray::SetSize(m_nTotalWordSize);
}

CWordArrayEx::CWordArrayEx(int nStartWordAddr, int nTotalWordSize)
{
	m_nStartWordAddr = nStartWordAddr;
	m_nTotalWordSize = nTotalWordSize;

	CWordArray::SetSize(m_nTotalWordSize);
}

CWordArrayEx::~CWordArrayEx()
{

}

void CWordArrayEx::SetArray(int nStartWordAddr, int nTotalWordSize)
{
//	DEBUGER_ASSERT_VALID(nTotalWordSize*2 == m_nSize);

	m_nStartWordAddr = nStartWordAddr;
	m_nTotalWordSize = nTotalWordSize;

	CWordArray::SetSize(m_nTotalWordSize);
}

int CWordArrayEx::GetIndex(int nWordAddr)
{
	//DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(m_nSize != 0);
	int nIndex = nWordAddr - m_nStartWordAddr;
//	DEBUGER_ASSERT_RANGE(nIndex, m_nSize);
	return nIndex;
}

WORD CWordArrayEx::GetWord(int nWordAddr)
{
	int nIndex = GetIndex(nWordAddr);
	WORD wWord = WORD(m_pData[nIndex]);
	return wWord;	
}

BYTE CWordArrayEx::GetByte(int nWordAddr, EN_BYTE enByte)
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

BYTE CWordArrayEx::GetNibble(int nWordAddr, EN_NIBBLE enNibble)
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

BOOL CWordArrayEx::GetBit(int nWordAddr, EN_BIT enBit)
{
	WORD wWord = GetWord(nWordAddr);
	return (wWord & enBit) ? TRUE : FALSE;
}

void CWordArrayEx::SetWord(int nWordAddr, WORD wWord)
{
	int nIndex = GetIndex(nWordAddr);
	m_pData[nIndex] = wWord;
}

void CWordArrayEx::SetByte(int nWordAddr, EN_BYTE enByte, WORD wByte)
{
	BYTE ucShift = 0;
	int nIndex = GetIndex(nWordAddr);
	if (enByte == enByteH) ucShift = 8;

	WORD wApplyData		= (wByte << ucShift);
	WORD wNotApplyData	= m_pData[nIndex] & ~enByte;

	m_pData[nIndex] = wApplyData | wNotApplyData;

//	m_pData[nIndex] = (m_pData[nIndex] & enByte) | ((wByte & enByte) << ucShift);
}

void CWordArrayEx::SetNibble(int nWordAddr, EN_NIBBLE enNibble, WORD wNibble)
{
	BYTE ucShift = 0;
	int nIndex = GetIndex(nWordAddr);
//	if (enNibble == enNibble3 || enNibble == enNibble4) ++nIndex;
//	if (enNibble == enNibble2 || enNibble == enNibble4) ucShift = 4;
	switch (enNibble)
	{
	case enNibble1:		 ucShift =  0;	break;
	case enNibble2:		 ucShift =  4;	break;
	case enNibble3:		 ucShift =  8;	break;
	case enNibble4:		 ucShift = 12;	break;
	}
	//m_pData[nIndex] |= (wNibble << ucShift);
	WORD wApplyData = (wNibble << ucShift);
	WORD wNotApplyData = m_pData[nIndex] & ~enNibble;

	m_pData[nIndex] = wApplyData | wNotApplyData;
}

void CWordArrayEx::SetBit(int nWordAddr, EN_BIT enBit, BOOL bBit)
{
	int nIndex = GetIndex(nWordAddr);
	if (bBit) m_pData[nIndex] |= enBit;
	else m_pData[nIndex] &= ~enBit;
}

void CWordArrayEx::InverseBit(int nWordAddr, EN_BIT enBit)
{
	int nIndex = GetIndex(nWordAddr);
	m_pData[nIndex] ^= enBit;
}

void CWordArrayEx::WriteDump(LPCTSTR lpszFileName)
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
