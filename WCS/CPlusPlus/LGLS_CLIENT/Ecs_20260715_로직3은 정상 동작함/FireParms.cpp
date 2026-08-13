
#include "stdafx.h"
#include "FireParms.h"

IMPLEMENT_DYNCREATE(CFireParms, CObject)

CFireParms::CFireParms()
{
	m_bStop = FALSE;
}

CFireParms::~CFireParms()
{
}

BOOL CFireParms::InitializeThis()
{
	if(CreateKey() == TRUE)
	{
		m_bInitialize = TRUE;
		return TRUE;
	}
	return FALSE;
}

void CFireParms::SetSTR(CString pstrWH_TYP
	, CString pstrWH_TYP_NM
	, CString pstrPLC_NO
	, CString pstrEQP_NO
	, CString pstrCELL_NO
	, CString pstrMESSAGE
	, CString pstrLEVEL
	, int pnLang)
{
	strWH_TYP = pstrWH_TYP;
	strWH_TYP_NM = pstrWH_TYP_NM;
	strPLC_NO = pstrPLC_NO;
	strEQP_NO = pstrEQP_NO;
	strCELL_NO = pstrCELL_NO;
	strMESSAGE = pstrMESSAGE;
	strLEVEL = pstrLEVEL;
	nLang = pnLang;
}

CString CFireParms::GetKey()
{
	return m_strKey;
}

BOOL CFireParms::CreateKey()
{
	if(strWH_TYP == _T("")){ return FALSE; };
	if(strPLC_NO == _T("")){ return FALSE;  };
	if(strEQP_NO == _T("") && strCELL_NO == _T("00-000-00")){ return FALSE;  };
	CString strReturn = _T("");
	strReturn.Format(_T("%02s%02s%05s%09s%02d"), strWH_TYP, strPLC_NO, strEQP_NO, strCELL_NO, nLang);
	m_strKey = strReturn;
	return TRUE;
}

BOOL CFireParms::Stop()
{
	m_bStop = TRUE;
	return TRUE;
}

BOOL CFireParms::IsStop()
{
	return m_bStop;
}

BOOL CFireParms::Start()
{
	m_bStop = FALSE;
	return TRUE;
}
