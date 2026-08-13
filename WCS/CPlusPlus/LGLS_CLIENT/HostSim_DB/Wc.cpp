#include "StdAfx.h"
#include "Ecs.h"
#include "Wc.h"
#include "EcsDoc.h"
#include "RecordSetWrap.h"

#define DEVICE m_port.m_strDevice

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CWc, CEquipment)

CWc::CWc(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex)
	: CEquipment(pDoc, pstrThreadNo,  nIndex)
{
	m_pDoc = pDoc;
	m_pInfo = new CWcInfo(this);
	m_enKind = CEquipment::enWC;
	m_pRsw = NULL;
}

CWc::~CWc(void)
{
	if (m_pInfo) {delete m_pInfo;};
}

void CWc::AutoRunProc()
{
	CString strWC_NO, strWC_MC_NO, strCHK_BYPASS_YN;
	if(m_pInfo == NULL){ return; };
	if(m_pRsw == NULL){	return;	}
	if(m_pRsw->m_pRecordSet == NULL){ return;}

	int nWcCnt = m_pRsw->m_pRecordSet->RecordCount;
	if(nWcCnt < 1)
	{
		return;
	}
	CWcInfo* pWcInfo = m_pInfo;
	if(pWcInfo == NULL) {	return; };

	m_pRsw->MoveFirst();
	for(int nIdxFor = 0; nIdxFor < nWcCnt; nIdxFor++)
	{
		strWC_NO = m_pRsw->GetItem(_T("WC_NO"));
		strWC_MC_NO = m_pRsw->GetItem(_T("WC_MC_NO"));
		CWC_DATA* pWC_DATA = m_pInfo->m_MapWC_DATA[strWC_MC_NO];
		if(pWC_DATA == NULL)
		{ 
			m_pRsw->MoveNext();
			continue; 
		};

		pWC_DATA->SetWH_TYP(m_pRsw->GetItem( _T("WH_TYP")));
		pWC_DATA->SetWC_NO(m_pRsw->GetItem( _T("WC_NO")));
		pWC_DATA->SetWC_MC_NO(m_pRsw->GetItem( _T("WC_MC_NO")));
		pWC_DATA->SetPLC_NO(m_pRsw->GetItem( _T("PLC_NO")));

		pWC_DATA->SetWEIGHT_RCV_VAL(m_pRsw->GetItem( _T("WEIGHT_RCV_VAL")));
		pWC_DATA->SetWEIGHT_READ_STA(m_pRsw->GetItem( _T("WEIGHT_READ_STA")));
		pWC_DATA->SetCMD_RQ_ID(m_pRsw->GetItem( _T("CMD_RQ_ID")));
		pWC_DATA->SetCMD_RQ_YN(m_pRsw->GetItem( _T("CMD_RQ_YN")));
		pWC_DATA->SetSUSPEND(m_pRsw->GetItem( _T("SUSPEND")));
		pWC_DATA->SetEQP_TIME(m_pRsw->GetItem( _T("EQP_TIME")));
		pWC_DATA->SetEQP_CONNECTED_YN(m_pRsw->GetItem( _T("CONNECTED_YN")));
		pWC_DATA->SetCHK_BYPASS_YN(m_pRsw->GetItem( _T("CHK_BYPASS_YN")));

		//
		m_pDoc->m_strBypassChkYn = pWC_DATA->V_CHK_BYPASS_YN;

		m_pRsw->MoveNext();
	}
}

void CWc::CommandProc()
{
}

CString CWc::GetSelectQry() 
{
	CString strSql = _T("");

	strSql.Format(_T("SELECT  WD.WH_TYP																									\n")
				  _T("       ,WD.WC_NO																									\n")
				  _T("       ,WD.WC_MC_NO																								\n")
				  _T("       ,WD.PLC_NO																									\n")
				  _T("       ,WD.WEIGHT_READ_STA																						\n")
				  _T("       ,WD.WEIGHT_RCV_VAL																							\n")
				  _T("       ,WD.CMD_RQ_ID																								\n")
				  _T("       ,WD.CMD_RQ_YN																								\n")
				  _T("       ,WD.SUSPEND																								\n")
				  _T("	     ,WD.CHK_BYPASS_YN																							\n")
				  _T("	     ,CASE WHEN TO_CHAR(NOW() - EM.UPD_DT, 'YYYYMMDDHH24MISS')::INTEGER > 5 THEN 6 ELSE 0 END AS EQP_TIME		\n")
				  _T("	     ,EM.CONNECTED_YN															  								\n")
				  _T(" FROM WC_DATA WD 																									\n")
				  _T(" INNER JOIN (SELECT PLC_NO, CONNECTED_YN, UPD_DT FROM EQP_MST WHERE WH_TYP = '%02s' AND EQP_TYP = 'WC') EM		\n")
				  _T("         ON WD.PLC_NO = EM.PLC_NO																					\n")
				  _T("WHERE WD.WH_TYP = '%02s'					\n")
				  _T("ORDER BY WD.WC_NO							\n"), m_WH_TYP, m_WH_TYP);
	
	return strSql;
}

void CWc::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}
