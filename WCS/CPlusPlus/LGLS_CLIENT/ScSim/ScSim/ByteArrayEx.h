// ByteArrayEx.h: interface for the CByteArrayEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BYTEARRAYEX_H__5E536D86_43A8_4AD5_B4AE_AC16115681AB__INCLUDED_)
#define AFX_BYTEARRAYEX_H__5E536D86_43A8_4AD5_B4AE_AC16115681AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>

enum EN_BYTE { 
	enByteL = 0x00FF, 
	enByteH = 0xFF00
};

enum EN_NIBBLE {
	enNibble1 = 0x000F,
	enNibble2 = 0x00F0,
	enNibble3 = 0x0F00,
	enNibble4 = 0xF000
};

enum EN_BIT { 
	enBit11 = 0x0001, enBit12 = 0x0002, enBit13 = 0x0004, enBit14 = 0x0008, 
	enBit15 = 0x0010, enBit16 = 0x0020, enBit17 = 0x0040, enBit18 = 0x0080,
	enBit21 = 0x0100, enBit22 = 0x0200, enBit23 = 0x0400, enBit24 = 0x0800,
	enBit25 = 0x1000, enBit26 = 0x2000, enBit27 = 0x4000, enBit28 = 0x8000
};

class CByteArrayEx : public CByteArray  
{
public:
	CByteArrayEx(int nStartWordAddr, int nTotalWordSize);
	virtual ~CByteArrayEx();

protected:
	int GetIndex(int nWordAddr);

public:
	void InitArray() { if (m_nSize) ::memset(m_pData, 0, m_nSize); }
	void SetArray(int nStartWordAddr, int nTotalWordSize);
	void WriteDump(LPCTSTR lpszFileName);

public:
	WORD GetWord(int nWordAddr);
	BYTE GetByte(int nWordAddr, EN_BYTE enByte);
	BYTE GetNibble(int nWordAddr, EN_NIBBLE enNibble);
	BOOL GetBit(int nWordAddr, EN_BIT enBit);
	int	GetStartWordAddr() { return m_nStartWordAddr; }
	int GetTotalWordSize() { return m_nTotalWordSize; }

public:
	void SetWord(int nWordAddr, WORD wWord);
	void SetByte(int nWordAddr, EN_BYTE enByte, BYTE ucByte);
	void SetNibble(int nWordAddr, EN_NIBBLE enNibble, BYTE ucNibble);
	void SetBit(int nWordAddr, EN_BIT enBit, BOOL bBit);
	
protected:
	int m_nStartWordAddr;
	int m_nTotalWordSize;
};

#endif // !defined(AFX_BYTEARRAYEX_H__5E536D86_43A8_4AD5_B4AE_AC16115681AB__INCLUDED_)
