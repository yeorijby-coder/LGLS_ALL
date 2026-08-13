#pragma once
#include "afx.h"
#include <afxcoll.h>
#include "EQP_ECD_MST.h"
#include "Lang.h"

class CErrorMst
{
public:
	CErrorMst(CEcsDoc* pDoc);
	~CErrorMst(void);

public:
	CEcsDoc* m_pDoc;

public:
	CMap<CString, LPCTSTR, CEQP_ECD_MST*, CEQP_ECD_MST*>* m_Map;

public:
	CString GetQrySelect();

public:
	CString GetMSG(EN_LANG pEN_LANG, CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetMSG(pEN_LANG, CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };
	CString GetMSG(EN_LANG pEN_LANG, CString pstrTUPLE_KEY);

	CString GetMSG_KOR(CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetMSG_KOR(CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };
	CString GetMSG_ENG(CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetMSG_ENG(CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };
	CString GetMSG_CHIN(CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetMSG_CHIN(CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };
	CString GetMSG_HUN(CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetMSG_HUN(CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };

	CString GetMSG_KOR(CString pstrTUPLE_KEY)  { return GetMSG(EN_KOR, pstrTUPLE_KEY); };
	CString GetMSG_ENG(CString pstrTUPLE_KEY)  { return GetMSG(EN_ENG, pstrTUPLE_KEY); };
	CString GetMSG_CHIN(CString pstrTUPLE_KEY) { return GetMSG(EN_CHIN, pstrTUPLE_KEY); };
	CString GetMSG_HUN(CString pstrTUPLE_KEY)  { return GetMSG(EN_HUN, pstrTUPLE_KEY); };


	CString GetACTION(EN_LANG pEN_LANG, CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetACTION(pEN_LANG, CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };
	CString GetACTION(EN_LANG pEN_LANG, CString pstrTUPLE_KEY);

	CString GetACTION_KOR(CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetACTION_KOR(CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };
	CString GetACTION_ENG(CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetACTION_ENG(CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };
	CString GetACTION_CHIN(CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetACTION_CHIN(CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };
	CString GetACTION_HUN(CString pstrEQP_TYP, CString pstrERROR_CODE){ return GetACTION_HUN(CLib::GetTupleKeyEQP_ECD_MST(pstrEQP_TYP, pstrERROR_CODE)); };

	CString GetACTION_KOR(CString pstrTUPLE_KEY)   { return GetACTION(EN_KOR, pstrTUPLE_KEY); };
	CString GetACTION_ENG(CString pstrTUPLE_KEY)   { return GetACTION(EN_ENG, pstrTUPLE_KEY); };
	CString GetACTION_CHIN(CString pstrTUPLE_KEY)  { return GetACTION(EN_CHIN, pstrTUPLE_KEY); };
	CString GetACTION_HUN(CString pstrTUPLE_KEY)   { return GetACTION(EN_HUN, pstrTUPLE_KEY); };
};

