// ListenerSk.h : header file
//

#pragma once

class CEquipment;


// CListenerSk 명령 대상입니다.
//
class CListenerSk : public CAsyncSocket
{
public:
	CListenerSk() {}
	CListenerSk(CEquipment* pEquipment);
	virtual ~CListenerSk();

// Variables
protected:
	CEquipment* m_pEquipment;

// Overrides
public:
	virtual void OnAccept(int nErrorCode);

// Implementation
protected:
};
