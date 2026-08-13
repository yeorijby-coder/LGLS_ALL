#pragma once
#include "afx.h"
#include <afxcoll.h>

class CEQP_ECD_MST
{
public:
	CEQP_ECD_MST();
	CEQP_ECD_MST(CString EQP_TYP, CString ERROR_CODE);
	~CEQP_ECD_MST(void);

public:
	CString EQP_TYP;
	CString ERROR_CODE;
	CStringArray m_MSG;
	CStringArray m_ACTION;
	int ERR_LEV;

public:
	CString GetTupleKeyEQP_ECD_MST();
	void SetValues_MSG(CString strKOR, CString strENG, CString strCHIN, CString strHUN);
	void SetValues_ACTION(CString strKOR, CString strENG, CString strCHIN, CString strHUN);

public:
	void SetMSG_KOR(CString pstrValue);
	void SetMSG_ENG(CString pstrValue);
	void SetMSG_CHIN(CString pstrValue);
	void SetMSG_HUN(CString pstrValue);

	void SetACTION_KOR(CString pstrValue);
	void SetACTION_ENG(CString pstrValue);
	void SetACTION_CHIN(CString pstrValue);
	void SetACTION_HUN(CString pstrValue);
};

