
#ifndef __ADODB_H__
#define __ADODB_H__



//ado 개체를 사용 하기 위해서 필히 선언 해야 하는 것이다
//==>jhkim0629
#import "C:\Program Files\Common Files\System\ado\msado20.tlb" no_namespace rename ("EOF", "adoEOF")
//#import "C:\Program Files (x86)\Common Files\System\ado\msado20.tlb" no_namespace rename ("EOF", "adoEOF")
//<==jhkim0629

#include "AdoDbIO.h"

class CEcsDoc;

class CAdoDB : public CObject
{
protected:
	DECLARE_DYNCREATE(CAdoDB)

public:
	CAdoDB();
	CAdoDB(CEcsDoc* pDoc);
	~CAdoDB();

public:
	CEcsDoc* m_pDoc;
	BOOL     m_bConnected;
	CString  m_strErrMsg;

public:
	_ConnectionPtr	m_pWmsDb;

public:
	BOOL	ConnectDB();			

public:
	BOOL ExecuteStoredProc(CAdoDbIO *pAdoDbIO, int nScNum);
	BOOL ExecuteQueryString(CString strSql);

	void CrateInputParameter(_CommandPtr pCmd, CObList *plistIn);
	void CrateOutputParameter(_CommandPtr pCmd, CObList *plistOut);
	void ExtractOutputParameter(_CommandPtr pCmd, CObList *plistOut);

public:
	BOOL SelectSqlForThread(CString strSql, CStringList &strTempList, int nRtRecord, int &nRowCnt, CString &strMsg);
	BOOL SelectSqlForThread(CString strSql, int &nRowCnt, CString &strMsg);
	BOOL ExecuteSqlForMainPGM(CString strSql);

public:
	BOOL ExecuteProcForInputOrder(CString strStorProc, CStringList &strTempList, int &nRetCd, CString &strRetMsg);
	BOOL ExecuteProcForOutputOrder(CString strStorProc, CStringList &strTempList, int &nRetCd, CString &strRetMsg, CString &strJobNo);
	BOOL ExecuteProcForManualOrder(CString strStorProc, CStringList &strTempList, int &nRetCd, CString &strRetMsg);
	void UTF8toANSI (LPSTR src, CString & dst);
	_RecordsetPtr SelectSqlForThread_RecordSet(CString strSql, int &nRowCnt, CString &strMsg);

public:
	BOOL BeginTrans();
	BOOL CommitTrans();
	BOOL RollbackTrans();

};

#endif // __ADODB_H__

