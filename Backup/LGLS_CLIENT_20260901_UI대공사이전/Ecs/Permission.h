#pragma once

enum EN_PERM
{
	SEL_YN = 0,
	UPD_YN,
	DEL_YN,
	INS_YN,
	PRT_YN,
	EXE_YN,
	DOWN_YN,
	WIN_VIEW_YN,
};

class CPermission : public CObject
{
protected:
	DECLARE_DYNCREATE(CPermission)

public:
	CPermission(void);
	~CPermission(void);


public:
	BOOL m_ArrPerm[8];
};

