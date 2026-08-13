#pragma once

typedef struct ST_LANG
{
	LPWSTR m_Key; //LPCSTR 'L
	LPWSTR m_ArrLang[4];

}ST_LANG;

enum EN_LANG
{
	EN_KOR = 0,
	EN_ENG,
	EN_HUN,
	EN_CHIN
};


class CLang : public CObject
{

protected:
	DECLARE_DYNCREATE(CLang)

public:
	CLang();
	~CLang();
	
public:
	CMap<CString, LPCTSTR, ST_LANG*, ST_LANG*> m_Langs;

public:
	CString GetQrySelect();

public:
	BOOL InitializeLangs(int p_nHashSize);
	BOOL AddLangs(LPWSTR p_lpwstrKey, ST_LANG* pST_LANG);
	BOOL AddLangs(CString p_cstrKey, ST_LANG* pST_LANG);
	ST_LANG* GetST_LANG(LPWSTR p_lpwstrKey, ST_LANG* pST_LANG);
	ST_LANG* GetST_LANG(CString p_cstrKey, ST_LANG* pST_LANG, bool bTrash); // 2번째 파라미터는 단순 리턴형 구분변수입니다. 사용되지 않습니다.
	CString GetLangValue(LPWSTR p_lpwstrKey, EN_LANG nLang);
	LPWSTR GetLangValue(LPWSTR p_lpwstrKey, EN_LANG nLang, bool pTrash); // 2번째 파라미터는 단순 리턴형 구분변수입니다. 사용되지 않습니다.
	CString GetLangValue(CString p_cstrKey, EN_LANG nLang);
	LPWSTR GetLangValue(CString p_cstrKey, EN_LANG nLang, bool pTrash); // 2번째 파라미터는 단순 리턴형 구분변수입니다. 사용되지 않습니다.

public:
	BOOL MallocLPWSTR(CString strMultiByte, LPWSTR& str);

};

