#pragma once

enum EN_MSG_LANG
{
	kor = 0,
	eng ,
	hun ,
	chin ,
};

//#include "afx.h"
class CMsgBoxLang :
	public CObject
{
protected:
	DECLARE_DYNCREATE(CMsgBoxLang)

public:
	CMsgBoxLang(void);
	~CMsgBoxLang(void);

public:
	CString m_ArrPerm[4];
};
