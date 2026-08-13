// Bcr.cpp: implementation of the CBcr class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "Bcr.h"
#include "EcsDoc.h"
#include "RecordSetWrap.h"

#define DEVICE m_port.m_strDevice

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBcr
//
IMPLEMENT_DYNAMIC(CBcr, CEquipment)

CBcr::CBcr(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex)
	: CEquipment(pDoc, pstrThreadNo,  nIndex)
{
	m_pInfo = new CBcrInfo(this);
	m_enKind = CEquipment::enBCR;
}

CBcr::~CBcr()
{
	if (m_pInfo) {delete m_pInfo;};
}

void CBcr::AutoRunProc()
{
	CString strBCR_NO, strBCR_MC_NO;
	if(m_pInfo == NULL){ return; };
	if(m_pRsw == NULL){	return;	}
	if(m_pRsw->m_pRecordSet == NULL){ return;}

	int nBcrCnt = m_pRsw->m_pRecordSet->RecordCount;
	if(nBcrCnt < 1)
	{
		return;
	}
	CBcrInfo* pBcrInfo = m_pInfo;
	if(pBcrInfo == NULL) {	return; };

	m_pRsw->MoveFirst();
	for(int nIdxFor = 0; nIdxFor < nBcrCnt; nIdxFor++)
	{
		strBCR_NO = m_pRsw->GetItem(_T("BCR_NO"));
		CBCR_MST* pBCR_MST = m_pInfo->m_MapBCR_MST[strBCR_NO];
		if(pBCR_MST == NULL)
		{ 
			m_pRsw->MoveNext();
			continue; 
		};

		CTrackInfo* pTrackInfo = (CTrackInfo*)(m_pDoc->GetTrackInfoNew(pBCR_MST->m_strBCR_MC_NO));
		if(pTrackInfo == NULL)
		{
			m_pRsw->MoveNext();
			continue; 
		}

		CCV_DATA* pCV_DATA = pTrackInfo->m_pCV_DATA;
		if(pCV_DATA == NULL)
		{
			m_pRsw->MoveNext();
			continue; 
		}


		pBCR_MST->SetUPD_DT(m_pRsw->GetItem( _T("UPD_DT")));
		pBCR_MST->SetUPD_USER_ID(m_pRsw->GetItem( _T("UPD_USER_ID")));
		pBCR_MST->SetUSE_YN(m_pRsw->GetItem( _T("USE_YN")));
		//pBCR_MST->SetPLC_IP(m_pRsw->GetItem( _T("PLC_IP")));
		//pBCR_MST->SetPLC_PORT(m_pRsw->GetItem( _T("PLC_PORT")));
		//pBCR_MST->SetSOCK_TIME_OUT(m_pRsw->GetItem( _T("SOCK_TIMEOUT")));
		//pBCR_MST->SetRETRY_YN(m_pRsw->GetItem( _T("RETRY_YN")));
		//pBCR_MST->SetCONNECTED_YN(m_pRsw->GetItem( _T("CONNECTED_YN")));
		pBCR_MST->SetBCR_STA(m_pRsw->GetItem( _T("BCR_STA")));
		pBCR_MST->SetBCR_LEV(m_pRsw->GetItem( _T("BCR_LEV")));
		pBCR_MST->SetNOREAD_CNT(m_pRsw->GetItem( _T("NOREAD_CNT")));
		pBCR_MST->SetSUSPEND(m_pRsw->GetItem(_T("SUSPEND")));
		pBcrInfo->InvokeControl(pBCR_MST, pCV_DATA);
		m_pRsw->MoveNext();
	}


}


void CBcr::CommandProc()
{
}


CString CBcr::GetSelectQry() //kdh20190521
{
	CString strSql = _T("");

	strSql.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(BM.BCR_NO,'0') AS BCR_NO, \n")
				  _T("		  ") + m_pDoc->NVL + _T("(BM.BCR_MC_NO,'0') AS BCR_MC_NO,  \n")
				  _T("        ") + m_pDoc->NVL + _T("(BM.UPD_DT,") + m_pDoc->SYSDATE + _T(") AS UPD_DT, ")
			      _T("        ") + m_pDoc->NVL + _T("(BM.UPD_USER_ID ,'0') AS UPD_USER_ID, \n")
			      _T("		  ") + m_pDoc->NVL + _T("(BM.USE_YN, '0') AS USE_YN, \n")
			      _T("        ") + m_pDoc->NVL + _T("(BM.BCR_LEV, '0') AS BCR_LEV, \n")
			      _T("        ") + m_pDoc->NVL + _T("(BM.BCR_STA, '0') AS BCR_STA, \n")
			      _T("        ") + m_pDoc->NVL + _T("(BM.NOREAD_CNT,'0') AS NOREAD_CNT, \n")
			      _T("		  ") + m_pDoc->NVL + _T("(BM.SUSPEND,'0') AS SUSPEND")
			      _T("  FROM BCR_MST BM \n")
			      _T(" WHERE BM.WH_TYP = '%02s' \n"), m_WH_TYP);
	
	
	return strSql;
}

void CBcr::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}


