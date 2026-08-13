#pragma once
#include "EcsDoc.h"
// CTrayPicker 명령 대상입니다.

class CTrayPicker : public CObject
{
public:
	CTrayPicker();
	virtual ~CTrayPicker();

public:
//	CObList				m_listTrack;
	CList<int, int>		m_listTrack;
	CCriticalSection	m_syncTrayPicker;
};


