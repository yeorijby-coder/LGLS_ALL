// Queue.h : header file
//

#ifndef	__QUEUE_H__
#define	__QUEUE_H__


#define	QUEUE_SIZE		8192

class CQueue
{
private:
	BYTE m_Buffer[QUEUE_SIZE];
	int	m_nSize;

public:
	CQueue();
	~CQueue();

	int			GetSize();								// Gets the number of elements in this array
	int			Add(BYTE pData);						// Adds an element to the end of the array
	int			Add(char pData);						// Adds an element to the end of the array
	int			Append(BYTE* pData, int nSize);			// Appends another array to the array
	int			Append(char* pData, int nSize);			// Appends another array to the array
	BYTE		GetAt(int nIndex);						// Returns the value at a given index
	const char	GetCharAt(int nIndex);					// Returns the character value at a given index
	BYTE*		GetData(int nIndex, int nSize);			// Allows access to elements in the array
	const char*	GetCharData(int nIndex, int nSize);		// Allows access to elements in the array
	BYTE*		GetData();								// Allows access to elements in the array
	const char*	GetCharData();							// Allows access to elements in the array
	void		RemoveAt(int nIndex, int nCount = 1);	// Removes an element at a specific index
	void		RemoveAll();							// Removes all the elements from this array
	int			Find(char pData);						// Find a character in this array
};

#endif	// __QUEUE_H__
