#pragma once

#include "Equipment.h"

#include "AdoDB.h"
#include "URMDBAccess.h"

class CConnectStatus
{
	//DECLARE_DYNAMIC(CConnectStatus)

public:
	CConnectStatus(CEcsDoc* pDoc);
	~CConnectStatus(void);

public:
	CEcsDoc* m_pDoc;
	CRecordSetWrap* m_pRsw;

public:
	//CMap<CString, LPCTSTR, CConnectStatus*, CConnectStatus*> m_MapCONNECT;
	void SetVar(CRecordSetWrap* pRsw);
	CString GetSelectQry();
	BOOL m_bModified;

	CString V_EQP_TIME1;
	CString V_EQP_TIME2;
	CString V_EQP_COLOR1;
	CString V_EQP_COLOR2;

	void SetEQP_TIME1(CString V_EQP_TIME1);
	void SetEQP_TIME2(CString V_EQP_TIME2);
	void SetEQP_COLOR1(CString V_EQP_COLOR1);
	void SetEQP_COLOR2(CString V_EQP_COLOR2);

	CString m_WH_TYP;
	CString m_HOST_NUM;
};

