#include "StdAfx.h"
#include "Lang.h"



IMPLEMENT_DYNCREATE(CLang, CObject)

CLang::CLang()
{ 

}


CLang::~CLang()
{
}


BOOL CLang::InitializeLangs(int p_nHashSize)
{
	try
	{
		m_Langs.InitHashTable(p_nHashSize);
	}
	catch (CException* e)
	{
		TCHAR szTemp[512] = {0};
		e->GetErrorMessage(szTemp, 512);
		AfxMessageBox(szTemp);
		return FALSE;
	}
	return TRUE;
}

BOOL CLang::AddLangs(LPWSTR p_lpwstrKey, ST_LANG* p_STValue)
{
	LPWSTR key = p_lpwstrKey;
	ST_LANG* value = p_STValue;
	ST_LANG* prev_Value;

	if(m_Langs.Lookup(key, prev_Value))
	{
		return FALSE;
	}

	try
	{
		m_Langs.SetAt(key, value);
	}
	catch (CException* e)
	{
		TCHAR szTemp[512] = {0};
		e->GetErrorMessage(szTemp, 512);
		AfxMessageBox(szTemp);
		return FALSE;
	}

	return TRUE;
}


ST_LANG* CLang::GetST_LANG(LPWSTR p_lpwstrKey, ST_LANG* pST_LANG)
{
	LPWSTR key = p_lpwstrKey;

	if(m_Langs.Lookup(key, pST_LANG ))
	{
		//존재하는 값
		return pST_LANG;
	}

	return pST_LANG;

}

ST_LANG* CLang::GetST_LANG(CString p_cstrKey, ST_LANG* pST_LANG, bool bTrash)
{
	LPWSTR key = (LPWSTR)(LPCWSTR)p_cstrKey;

	if(m_Langs.Lookup(key, pST_LANG ))
	{
		//존재하는 값
		return pST_LANG;
	}

	return pST_LANG;
}

CString CLang::GetQrySelect()
{
	CString strSql = _T("");

	//나중에
	strSql.Format(_T(" SELECT COALESCE(FROM_LANG, FROM_LANG) AS FROM_LANG			        \r\n")
				  _T("      , COALESCE(TO_LANG_E, FROM_LANG) AS TO_LANG_E		 		    \r\n")
				  _T("      , COALESCE(TO_LANG_H,FROM_LANG) AS TO_LANG_H		 		    \r\n")
				  _T("      , COALESCE(TO_LANG_C,FROM_LANG) AS TO_LANG_C		 		    \r\n")
				  _T("      , COALESCE(CTL_TYPE,'0') AS CTL_TYPE		 				\r\n")
				  _T("      , COALESCE(LANG_TYPE,'0') AS LANG_TYPE		 		    \r\n")
				  _T("      , COALESCE(USE_YN,'0') AS USE_YN		 				\r\n")
				  _T("		, COALESCE(INS_DT,now()) AS INS_DT					\r\n")
				  _T("		, COALESCE(INS_USER,'0') AS INS_USER					\r\n")
				  _T("		, COALESCE(UPD_DT,now()) AS UPD_DT					\r\n")
				  _T("		, COALESCE(UPD_USER,'0') AS UPD_USR					\r\n")
				  _T("   FROM CHG_LANG		    \r\n"));
	return strSql;
}

BOOL CLang::AddLangs(CString p_cstrKey, ST_LANG* p_STValue)
{
	LPWSTR key = (LPWSTR)(LPCWSTR)p_cstrKey;
	ST_LANG* value = p_STValue;
	ST_LANG* prev_Value;

	if(m_Langs.Lookup(key, prev_Value ))
	{
		//존재하는 값
		return FALSE;
	}

	try
	{
		m_Langs.SetAt(key, value);
	}
	catch (CException* e)
	{
		TCHAR szTemp[512] = {0};
		e->GetErrorMessage(szTemp, 512);
		AfxMessageBox(szTemp);
		return FALSE;
	}

	return TRUE;
}


CString CLang::GetLangValue(LPWSTR p_lpwstrKey, EN_LANG nLang)
{
	LPWSTR key = p_lpwstrKey;
	ST_LANG* stValue;
	if(m_Langs.Lookup(key, stValue))
	{
		return stValue->m_ArrLang[nLang];
	}

	return (CString)p_lpwstrKey;
}

LPWSTR CLang::GetLangValue(LPWSTR p_lpwstrKey, EN_LANG nLang, bool isCString)
{
	LPWSTR key = p_lpwstrKey;
	ST_LANG* stValue;
	if(m_Langs.Lookup(key, stValue))
	{
		return (stValue->m_ArrLang[nLang]);
	}

	return _T("");
}

BOOL CLang::MallocLPWSTR(CString strMultiByte, LPWSTR& str)
{
	WCHAR *p = new WCHAR[1024];
	StrCpyW(p, CT2W(strMultiByte));
	str =  (LPWSTR)malloc(sizeof(p));
	str = (LPWSTR)p;
	return TRUE;
}


CString CLang::GetLangValue(CString p_cstrKey, EN_LANG nLang)
{
	LPWSTR key = (LPWSTR)(LPCWSTR)p_cstrKey;
	ST_LANG* stValue;
	if(m_Langs.Lookup(key, stValue))
	{
		LPWSTR a = stValue->m_ArrLang[nLang];
		return (stValue->m_ArrLang[nLang]);
	}

	return p_cstrKey;
}

LPWSTR CLang::GetLangValue(CString p_cstrKey, EN_LANG nLang, bool isCString)
{
	LPWSTR key = (LPWSTR)(LPCWSTR)p_cstrKey;
	ST_LANG* stValue;
	if(m_Langs.Lookup(key, stValue))
	{
		return (stValue->m_ArrLang[nLang]);
	}

	return _T("");
}