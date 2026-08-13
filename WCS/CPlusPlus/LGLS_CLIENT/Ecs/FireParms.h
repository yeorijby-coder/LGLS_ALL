#pragma once

class CFireParms : public CObject
{
protected:
	DECLARE_DYNCREATE(CFireParms)

public:
	CFireParms();
	CFireParms(CEcsDoc* pDoc);
	~CFireParms();

private:
	BOOL m_bInitialize;

private:
	CString m_strKey;
	BOOL	m_bStop;

public:
	CString strWH_TYP;
	CString strWH_TYP_NM;
	CString strPLC_NO;
	CString strEQP_NO;
	CString strCELL_NO;
	CString strMESSAGE;
	CString strLEVEL;
	int nLang;


public:
	BOOL InitializeThis();

private:
	BOOL CreateKey();

public:
	CString GetKey();

public:
	BOOL Stop();
	BOOL Start();

public:
	BOOL IsStop();

public:
	void SetSTR(CString pstrWH_TYP
		, CString pstrWH_TYP_NM
		, CString pstrPLC_NO
		, CString pstrTRACK_NO
		, CString pstrLOCATION_NO
		, CString pstrMESSAGE
		, CString pstrLEVEL
		, int pnLang);
};

