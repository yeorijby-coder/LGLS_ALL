// Queue.cpp : implementation file
//

#include "stdafx.h"
#include "Queue.h"


/////////////////////////////////////////////////////////////////////////////
// CQueue 

//==============================================================================
//	Desc	: Construction
//	Date	: 
//	Update	:
//==============================================================================
CQueue::CQueue()
{
	RemoveAll();
}

//==============================================================================
//	Desc	: Destruction
//	Date	: 
//	Update	:
//==============================================================================
CQueue::~CQueue()
{
}

//==============================================================================
//	Desc	: Gets the number of elements in this buffer
//	Date	: 
//	Update	:
//==============================================================================
int CQueue::GetSize()
{
	return m_nSize;
}

//==============================================================================
//	Desc	: Adds an element to the end of the buffer
//	Date	: 
//	Update	:
//==============================================================================
int CQueue::Add(BYTE pData)
{
	if (GetSize() == (QUEUE_SIZE - 1))
		return -1;

	m_Buffer[m_nSize++] = pData;

	return (m_nSize - 1);
}

//==============================================================================
//	Desc	: Adds an element to the end of the buffer
//	Date	: 
//	Update	:
//==============================================================================
int CQueue::Add(char pData)
{
	if (GetSize() == (QUEUE_SIZE - 1))
		return -1;

	m_Buffer[m_nSize++] = pData;

	return (m_nSize - 1);
}

//==============================================================================
//	Desc	: Appends another array to the buffer
//	Date	: 
//	Update	:
//==============================================================================
int CQueue::Append(BYTE* pData, int nSize)
{
	if (GetSize() == (QUEUE_SIZE - 1))
		return -1;

	::memcpy(&m_Buffer[m_nSize], pData, nSize);
	m_nSize += nSize;

	return (m_nSize - 1);
}

//==============================================================================
//	Desc	: Appends another array to the buffer
//	Date	: 
//	Update	:
//==============================================================================
int CQueue::Append(char* pData, int nSize)
{
	if (GetSize() == (QUEUE_SIZE - 1))
		return -1;

	::memcpy(&m_Buffer[m_nSize], pData, nSize);
	m_nSize += nSize;

	return (m_nSize - 1);
}

//==============================================================================
//	Desc	: Returns the value at a given index
//	Date	: 
//	Update	:
//==============================================================================
BYTE CQueue::GetAt(int nIndex)
{
	if (GetSize() == 0)
		return 0x00;

	return m_Buffer[nIndex];
}

//==============================================================================
//	Desc	: Returns the character value at a given index
//	Date	: 
//	Update	:
//==============================================================================
const char CQueue::GetCharAt(int nIndex)
{
	if (GetSize() == 0)
		return 0x00;

	char szBuffer;

	sprintf(&szBuffer, "%c", m_Buffer[nIndex]);

	return szBuffer;
}

//==============================================================================
//	Desc	: Allows access to elements in the buffer
//	Date	: 
//	Update	:
//==============================================================================
BYTE* CQueue::GetData(int nIndex, int nSize)
{
	if (GetSize() == 0)
		return 0x00;

	BYTE pBuffer[QUEUE_SIZE];

	::memset(pBuffer, 0x00, sizeof(pBuffer));
	::memcpy(pBuffer, &m_Buffer[nIndex], nSize);

	return pBuffer;
}

//==============================================================================
//	Desc	: Allows access to character elements in the buffer
//	Date	: 
//	Update	:
//==============================================================================
const char* CQueue::GetCharData(int nIndex, int nSize)
{
	if (GetSize() == 0)
		return 0x00;

	char szBuffer[QUEUE_SIZE];

	::memset(szBuffer, 0x00, sizeof(szBuffer));
	::memcpy(szBuffer, &m_Buffer[nIndex], nSize);

	return szBuffer;
}

//==============================================================================
//	Desc	: Allows access to elements in the buffer
//	Date	: 
//	Update	:
//==============================================================================
BYTE* CQueue::GetData()
{
	if (GetSize() == 0)
		return 0x00;

	return m_Buffer;
}

//==============================================================================
//	Desc	: Allows access to character elements in the buffer
//	Date	: 
//	Update	:
//==============================================================================
const char* CQueue::GetCharData()
{
	if (GetSize() == 0)
		return 0x00;

	char szBuffer[QUEUE_SIZE];
	
	sprintf(szBuffer, "%s", m_Buffer);

	return (szBuffer);
}

//==============================================================================
//	Desc	: Removes an element at a specific index
//	Date	: 
//	Update	:
//==============================================================================
void CQueue::RemoveAt(int nIndex, int nCount)
{
	if (GetSize() == 0)
		return;

	::memcpy(&m_Buffer[nIndex], &m_Buffer[nIndex + nCount], GetSize());
	m_nSize -= nCount;
}

//==============================================================================
//	Desc	: Removes all the elements from this buffer
//	Date	: 
//	Update	:
//==============================================================================
void CQueue::RemoveAll()
{
	m_nSize = 0;
	memset(m_Buffer, 0x00, QUEUE_SIZE);
}

//==============================================================================
//	Desc	: Find a character in this array
//	Date	: 
//	Update	:
//==============================================================================
int CQueue::Find(char pData)
{
	char szBuffer[QUEUE_SIZE];
	char* pDest;
	
	sprintf(szBuffer, "%s", m_Buffer);
	pDest = strchr(szBuffer, pData);

	if (pDest == NULL)	return -1;

	return (pDest - szBuffer + 1);
}
