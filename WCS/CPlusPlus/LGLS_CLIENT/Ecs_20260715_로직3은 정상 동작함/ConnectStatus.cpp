#include "StdAfx.h"
#include "ConnectStatus.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CConnectStatus::CConnectStatus(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	m_pRsw = NULL;
}


CConnectStatus::~CConnectStatus(void)
{
}

CString CConnectStatus::GetSelectQry()
{
	CString strSql = _T("");

	strSql.Format(_T("SELECT CASE WHEN DATEDIFF(SECOND, UPD_DT, GETDATE()) > 120 THEN 121 ELSE 0 END AS EQP_TIME	\n")
				  _T(" FROM EQP_MST 																							    \n")
				  _T("WHERE WH_TYP = '%s' 	AND EQP_TYP = '%s'																		  \n"), m_WH_TYP, m_HOST_NUM);
	
	return strSql;
}

void CConnectStatus::SetVar(CRecordSetWrap* pRsw)
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	if(this->m_pRsw->m_pRecordSet == NULL){ return;}

	if (m_HOST_NUM == _T("HOST1"))
	{
		SetEQP_TIME1(m_pRsw->GetItem( _T("EQP_TIME")));
	}
	else
	{
		SetEQP_TIME2(m_pRsw->GetItem( _T("EQP_TIME")));
	}
}

void CConnectStatus::SetEQP_TIME1(CString pstrEQP_TIME1)
{
	if(V_EQP_TIME1 == pstrEQP_TIME1)
		return;

	V_EQP_TIME1 = pstrEQP_TIME1;
	m_bModified = true;
}

void CConnectStatus::SetEQP_TIME2(CString pstrEQP_TIME2)
{
	if(V_EQP_TIME2 == pstrEQP_TIME2)
		return;

	V_EQP_TIME2 = pstrEQP_TIME2;
	m_bModified = true;
}

void CConnectStatus::SetEQP_COLOR1(CString pstrEQP_COLOR1)
{
	if(V_EQP_COLOR1 == pstrEQP_COLOR1)
		return;

	V_EQP_COLOR1 = pstrEQP_COLOR1;
	m_bModified = true;
}

void CConnectStatus::SetEQP_COLOR2(CString pstrEQP_COLOR2)
{
	if(V_EQP_COLOR2 == pstrEQP_COLOR2)
		return;

	V_EQP_COLOR2 = pstrEQP_COLOR2;
	m_bModified = true;
}

