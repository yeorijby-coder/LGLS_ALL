#include "StdAfx.h"
#include "stdafx.h"
#include "ErrorMst.h"

CErrorMst::CErrorMst(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	m_Map = new CMap<CString, LPCTSTR, CEQP_ECD_MST*, CEQP_ECD_MST*>();
}


CErrorMst::~CErrorMst()
{
	if (m_Map) {delete m_Map;};
}


CString CErrorMst::GetQrySelect()
{
	CString strSql = _T("");

	//포인터 오류
	strSql.Format(_T(" SELECT COALESCE(EQP_TYP,'0') AS EQP_TYP			        \r\n")
				  _T("      , COALESCE(EQP_ERR_CD,'0') AS EQP_ERR_CD			\r\n")
				  _T("      , COALESCE(MSG_KOR,'0') AS MSG_KOR					\r\n")
				  _T("      , COALESCE(MSG_CHIN,'0') AS MSG_CHIN				\r\n")
				  _T("      , COALESCE(MSG_ENG,'0') AS MSG_ENG					\r\n")
				  _T("      , COALESCE(MSG_HUN,'0') AS MSG_HUN					\r\n")
				  _T("      , COALESCE(ACTION_KOR,'0') AS ACTION_KOR			\r\n")
				  _T("      , COALESCE(ACTION_CHIN,'0') AS ACTION_CHIN			\r\n")
				  _T("      , COALESCE(ACTION_ENG,'0') AS ACTION_ENG			\r\n")
				  _T("      , COALESCE(ACTION_HUN,'0') AS ACTION_HUN			\r\n")
				  _T("      , COALESCE(ERR_LEV,'0') AS ERR_LEV					\r\n")
				  _T("   FROM EQP_ECD_MST		\r\n"));
	return strSql;
}

CString CErrorMst::GetACTION(EN_LANG pEN_LANG, CString pstrTUPLE_KEY)
{
	if(m_Map == NULL){ return _T(""); };
	CEQP_ECD_MST* p = NULL;
	p = (*m_Map)[pstrTUPLE_KEY];
	if(p == NULL)
	{
		return _T("NON DEFINED KEY VALUE : ") + pstrTUPLE_KEY;
	}

	return p->m_ACTION[pEN_LANG];
}

CString CErrorMst::GetMSG(EN_LANG pEN_LANG, CString pstrTUPLE_KEY)
{
	if(m_Map == NULL){ return _T(""); };
	CEQP_ECD_MST* p = NULL;
	p = (*m_Map)[pstrTUPLE_KEY];
	if(p == NULL)
	{
		return _T("NON DEFINED KEY VALUE : ") + pstrTUPLE_KEY;
	}

	return  p->m_MSG[pEN_LANG];
}