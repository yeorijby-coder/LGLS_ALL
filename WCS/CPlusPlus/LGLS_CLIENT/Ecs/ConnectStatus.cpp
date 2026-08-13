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

	// [LGLS] SCH(IO_TASK) 는 하트비트를 받기 전까지 '미가동'으로 봐야 한다.
	//   빈 문자열로 두면 CConvert::ToInt("")==0 이 되어 '경과 0초'=정상(초록)으로 오판함.
	//   EQP_MST 에 SCH 행이 없거나 조회가 실패해도 계속 RED 로 남도록 큰 값/'N' 으로 초기화.
	V_EQP_TIME3 = _T("9999");
	V_EQP_CONN3 = _T("N");
}


CConnectStatus::~CConnectStatus(void)
{
}

CString CConnectStatus::GetSelectQry()
{
	CString strSql = _T("");

	// [LGLS] SCH(IO_TASK) 는 실제 경과초와 CONNECTED_YN 을 그대로 가져온다(임계값 판정은 MainFrm 에서).
	//   HOST 처럼 CASE 로 0/121 을 만들지 않는 이유: '미가동'과 '오류'를 구분해서 표시해야 하기 때문.
	if (m_HOST_NUM == _T("SCH"))
	{
		strSql.Format(_T("SELECT DATEDIFF(SECOND, UPD_DT, GETDATE()) AS EQP_TIME  \n")
					  _T("     , ISNULL(CONNECTED_YN, 'N') AS CONNECTED_YN        \n")
					  _T("  FROM EQP_MST                                          \n")
					  _T(" WHERE WH_TYP = '%s' AND EQP_TYP = 'SCH'                \n"), m_WH_TYP);

		return strSql;
	}

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

	if (m_HOST_NUM == _T("SCH"))
	{
		SetEQP_TIME3(m_pRsw->GetItem( _T("EQP_TIME")));
		SetEQP_CONN3(m_pRsw->GetItem( _T("CONNECTED_YN")));
	}
	else if (m_HOST_NUM == _T("HOST1"))
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
	CSingleLock lock(&m_csVar, TRUE);
	if(V_EQP_TIME1 == pstrEQP_TIME1)
		return;

	V_EQP_TIME1 = pstrEQP_TIME1;
	m_bModified = true;
}

void CConnectStatus::SetEQP_TIME2(CString pstrEQP_TIME2)
{
	CSingleLock lock(&m_csVar, TRUE);
	if(V_EQP_TIME2 == pstrEQP_TIME2)
		return;

	V_EQP_TIME2 = pstrEQP_TIME2;
	m_bModified = true;
}

void CConnectStatus::SetEQP_COLOR1(CString pstrEQP_COLOR1)
{
	CSingleLock lock(&m_csVar, TRUE);
	if(V_EQP_COLOR1 == pstrEQP_COLOR1)
		return;

	V_EQP_COLOR1 = pstrEQP_COLOR1;
	m_bModified = true;
}

void CConnectStatus::SetEQP_COLOR2(CString pstrEQP_COLOR2)
{
	CSingleLock lock(&m_csVar, TRUE);
	if(V_EQP_COLOR2 == pstrEQP_COLOR2)
		return;

	V_EQP_COLOR2 = pstrEQP_COLOR2;
	m_bModified = true;
}

void CConnectStatus::SetEQP_TIME3(CString pstrEQP_TIME3)
{
	CSingleLock lock(&m_csVar, TRUE);
	if(V_EQP_TIME3 == pstrEQP_TIME3)
		return;

	V_EQP_TIME3 = pstrEQP_TIME3;
	m_bModified = true;
}

void CConnectStatus::SetEQP_CONN3(CString pstrEQP_CONN3)
{
	CSingleLock lock(&m_csVar, TRUE);
	if(V_EQP_CONN3 == pstrEQP_CONN3)
		return;

	V_EQP_CONN3 = pstrEQP_CONN3;
	m_bModified = true;
}

void CConnectStatus::SetEQP_COLOR3(CString pstrEQP_COLOR3)
{
	CSingleLock lock(&m_csVar, TRUE);
	if(V_EQP_COLOR3 == pstrEQP_COLOR3)
		return;

	V_EQP_COLOR3 = pstrEQP_COLOR3;
	m_bModified = true;
}


CString CConnectStatus::GetTime2Safe(){ CSingleLock lock(&m_csVar, TRUE); return CString((LPCTSTR)V_EQP_TIME2); }
CString CConnectStatus::GetColor2Safe(){ CSingleLock lock(&m_csVar, TRUE); return CString((LPCTSTR)V_EQP_COLOR2); }
CString CConnectStatus::GetTime3Safe(){ CSingleLock lock(&m_csVar, TRUE); return CString((LPCTSTR)V_EQP_TIME3); }
CString CConnectStatus::GetConn3Safe(){ CSingleLock lock(&m_csVar, TRUE); return CString((LPCTSTR)V_EQP_CONN3); }
CString CConnectStatus::GetColor3Safe(){ CSingleLock lock(&m_csVar, TRUE); return CString((LPCTSTR)V_EQP_COLOR3); }
