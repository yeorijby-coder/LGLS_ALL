
#include "StdAfx.h"
#include "stdafx.h"
#include "CollectDB.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Equipment.h"
#include "RecordSetWrap.h"

#define CRLF _T("\n")

extern "C" __declspec(dllimport) int  Usb_Qu_Getstate(); 

CCollectDB::CCollectDB(CEcsDoc* pDoc)
{
	m_bThreadDoWork = FALSE;
	m_pDoc = pDoc;
	m_pThread = NULL;
	m_pDB_ACCESS = NULL;
}


CCollectDB::~CCollectDB(void)
{
	if(m_pDB_ACCESS != NULL){ delete m_pDB_ACCESS; }
	m_pDB_ACCESS = NULL;

}

BOOL CCollectDB::IsDB_POSSIBLE()
{
	if(m_pDB_ACCESS == NULL || m_pDB_ACCESS->m_pAdoDB == NULL)
	{
		if(m_pDB_ACCESS != NULL)
		{
			if(m_pDB_ACCESS->m_pAdoDB == NULL)
			{
				delete m_pDB_ACCESS->m_pAdoDB;
				m_pDB_ACCESS->m_pAdoDB = NULL;
			}
			delete m_pDB_ACCESS;
			m_pDB_ACCESS = NULL;
		}

		m_pDB_ACCESS = new CURMDBAccess(m_pDoc, new CAdoDB(m_pDoc));
		return m_pDB_ACCESS->m_pAdoDB->ConnectDB();
	}

	if(m_pDB_ACCESS->m_pAdoDB->m_bConnected == FALSE)
	{
		delete m_pDB_ACCESS->m_pAdoDB;
		delete m_pDB_ACCESS;
		m_pDB_ACCESS->m_pAdoDB = NULL;
		m_pDB_ACCESS = NULL;
		return FALSE;
	}

	return m_pDB_ACCESS->m_pAdoDB->m_bConnected;
}

BOOL CCollectDB::StartDoWork()
{
	if(m_bThreadDoWork == TRUE)
	{
		return FALSE;
	}

	
	m_bThreadDoWork = TRUE;
	m_pThread = ::AfxBeginThread(DoWork, (LPVOID)this);
	if(m_pThread == NULL)
	{
		m_bThreadDoWork = FALSE;
		return FALSE;
	}

	return IsAllive();
}

BOOL CCollectDB::IsAllive()
{
	return m_bThreadDoWork;
}

BOOL CCollectDB::StopDoWork()
{
	m_bThreadDoWork = FALSE;
	::WaitForSingleObject(m_pThread, INFINITE);
	return TRUE;
}

UINT CCollectDB::DoWork(LPVOID pParm)
{
	CEquipment *pEquipment = NULL;
	CCollectDB* pThis = (CCollectDB*)pParm;
	while(pThis->m_bThreadDoWork)
	{			
		CEcsDoc* pDoc = pThis->m_pDoc;
		if(pDoc == NULL)
		{
			::Sleep(500);
			break;
		}
		int nEqpCount = pDoc->m_pEquipments.GetCount();
		for(int nIdxEqp = 0; nIdxEqp < nEqpCount; nIdxEqp++)
		{
			if(pDoc->m_bExit == true)
			{
				pThis->StopDoWork();
				break;
			}

			if(pThis->IsDB_POSSIBLE() == FALSE)
			{
				::Sleep(500);
				break;
			}
			pEquipment = pDoc->m_pEquipments[nIdxEqp];

			//설비들 값
			if(pEquipment == NULL || pEquipment->m_pRsw == NULL)
			{
				pThis->Collect_EQUIPMENT(pEquipment);
				::Sleep(50); //추가
			}
		}	

		if(pDoc->m_blConnectStatus == TRUE)
		{
			int HostCnt = 1; //2 (나중에 수정할 부분)
			for(int nIdxHost = 0; nIdxHost < HostCnt; nIdxHost++)
			{
				CString strHostNum = _T("HOST2");  //_T("HOST") + CConvert::ToString(nIdxHost+1); 
				pThis->ConnectStatus(pDoc->m_pConnectStatus, strHostNum);
				::Sleep(50); //추가
			}
		}
		::Sleep(1000); //1000
	}
	pThis->m_bThreadDoWork = FALSE;
	return 0;
}


void CCollectDB::Collect_EQUIPMENT(CEquipment* pEquipment)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strErrMsg = _T("");

	strSql = pEquipment->GetSelectQry();
	if(strSql == _T(""))
	{
		::Sleep(500);
		return;
	}

	_RecordsetPtr pRsptr = m_pDB_ACCESS->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strErrMsg);
	if(nRowCnt <= 0)
	{
		return;
	}
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	pEquipment->SetVar(pRsw);

	//::Sleep(50); //추가


	//@@@@
	//delete pRsw;
}
void CCollectDB::ConnectStatus(CConnectStatus* pConnectStatus, CString strHostNum)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strErrMsg = _T("");

	pConnectStatus->m_HOST_NUM = strHostNum;
	strSql = pConnectStatus->GetSelectQry();

	if(strSql == _T(""))
	{
		::Sleep(500);
		return;
	}
	//_RecordsetPtr pRsptr = NULL;
	//if (m_pDB_ACCESS != NULL && m_pDB_ACCESS->m_pAdoDB != NULL)
	_RecordsetPtr pRsptr = m_pDB_ACCESS->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strErrMsg);

	if(nRowCnt <= 0)
	{
		return;
	}

	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	pConnectStatus->SetVar(pRsw);
	delete pConnectStatus->m_pRsw; 
	pConnectStatus->m_pRsw = NULL;
}

