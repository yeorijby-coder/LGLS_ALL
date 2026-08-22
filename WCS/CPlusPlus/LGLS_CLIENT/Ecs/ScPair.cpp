// ScPair.cpp: implementation of the CScPair class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "ScPair.h"
#include "EcsDoc.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CScPair, CSc)
	//ï¿½ï¿½ï¿½ï¿½ X
CScPair::CScPair(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex)
	: CSc(pDoc,pstrThreadNo, nIndex)
{
	m_pDoc = pDoc;
	m_enKind = CEquipment::enSC;
	m_pInfo = new CScInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
}

CScPair::~CScPair()
{
	delete m_pInfo;
}


void CScPair::AutoRunProc() 
{
	CString strSC_NO;
	if(m_pInfo == NULL){ return; };
	if(m_pRsw == NULL){ return; };
	if(m_pRsw->m_pRecordSet == NULL){ return;}

	int nScCnt = (int)m_pRsw->m_pRecordSet->RecordCount;

	// [LGLS 2026-07-24] ¼³ºñ µ¥ÀÌÅÍ CString ¾²±â ±¸°£
	if(m_pDoc == NULL) return;
	CSingleLock _lockSc(&m_pDoc->m_csEqpData, TRUE);

	m_pRsw->MoveFirst();
	for(int nIdxFor = 0; nIdxFor < nScCnt; nIdxFor++)
	{
		//strSC_NO = m_pRsw->GetItem(_T("SC_NO"));
		strSC_NO = m_pRsw->GetItem(_T("MC_NO"));
		CSC_DATA* pSC_DATA = m_pInfo->m_MapSC_DATA[strSC_NO];
		if(pSC_DATA == NULL)
		{ 
			m_pRsw->MoveNext();
			continue; 
		};

		pSC_DATA->SetSC_VIEW_NM(m_pRsw->GetItem( _T("SC_VIEW_NM")));
		pSC_DATA->SetONLINE_MODE_RD(m_pRsw->GetItem( _T("ONLINE_MODE_RD")));
		pSC_DATA->SetAUTO_MODE_RD(m_pRsw->GetItem( _T("AUTO_MODE_RD")));
		pSC_DATA->SetSENSOR_FK_RD(m_pRsw->GetItem( _T("SENSOR_FK_RD")));
		pSC_DATA->SetUCSTATUS_RD(m_pRsw->GetItem( _T("UCSTATUS_RD")));
		pSC_DATA->SetPOS_H_RD(m_pRsw->GetItem( _T("POS_H_RD")));
		pSC_DATA->SetPOS_V_RD(m_pRsw->GetItem( _T("POS_V_RD")));
		pSC_DATA->SetERR_CODE_RD(m_pRsw->GetItem( _T("ERR_CODE_RD")));
		pSC_DATA->SetERR_STA_FK1_RD( m_pRsw->GetItem( _T("ERR_STA_FK1_RD")));
		pSC_DATA->SetERR_STA_FK2_RD(m_pRsw->GetItem( _T("ERR_STA_FK2_RD")));
		pSC_DATA->SetACTIVE_MODE_RD(m_pRsw->GetItem( _T("ACTIVE_MODE_RD")));
		pSC_DATA->SetCOMPLETE_RD(m_pRsw->GetItem( _T("COMPLETE_RD")));
		pSC_DATA->SetJOB_TYP_RD(m_pRsw->GetItem( _T("JOB_TYP_RD")));
		pSC_DATA->SetLUGG_NO_FK1_RD(m_pRsw->GetItem( _T("LUGG_NO_FK1_RD")));
		pSC_DATA->SetLUGG_NO_FK1_OD(m_pRsw->GetItem( _T("LUGG_NO_FK1_OD")));	// [LGLS 2026-08-22] Áö½Ã°ª(¼³ºñ ¹Ý¿µ Àü ±¸°£ Ç¥½Ã¿ë)
		pSC_DATA->SetSTART_BANK_FK1_RD(m_pRsw->GetItem( _T("START_BANK_FK1_RD")));
		pSC_DATA->SetSTART_HSPOS_FK1_RD(m_pRsw->GetItem( _T("START_HSPOS_FK1_RD")));
		pSC_DATA->SetDEST_BANK_FK1_RD(m_pRsw->GetItem( _T("DEST_BANK_FK1_RD")));
		pSC_DATA->SetDEST_HSPOS_FK1_RD(m_pRsw->GetItem( _T("DEST_HSPOS_FK1_RD")));
		pSC_DATA->SetUSE_FK_RD(m_pRsw->GetItem( _T("USE_FK_RD")));
		pSC_DATA->SetLUGG_NO_FK2_RD(m_pRsw->GetItem( _T("LUGG_NO_FK2_RD")));
		pSC_DATA->SetSTART_BANK_FK2_RD(m_pRsw->GetItem( _T("START_BANK_FK2_RD")));
		pSC_DATA->SetSTART_HSPOS_FK2_RD(m_pRsw->GetItem(_T("START_HSPOS_FK2_RD")));
		pSC_DATA->SetDEST_BANK_FK2_RD(m_pRsw->GetItem(_T("DEST_BANK_FK2_RD")));
		pSC_DATA->SetDEST_HSPOS_FK2_RD(m_pRsw->GetItem(_T("DEST_HSPOS_FK2_RD")));
		pSC_DATA->SetSTART_BAY_FK1_RD(m_pRsw->GetItem(_T("START_BAY_FK1_RD")));
		pSC_DATA->SetSTART_LEVEL_FK1_RD(m_pRsw->GetItem(_T("START_LEVEL_FK1_RD")));
		pSC_DATA->SetSTART_BAY_FK2_RD(m_pRsw->GetItem(_T("START_BAY_FK2_RD")));
		pSC_DATA->SetSTART_LEVEL_FK2_RD(m_pRsw->GetItem(_T("START_LEVEL_FK2_RD")));
		pSC_DATA->SetDEST_BAY_FK1_RD(m_pRsw->GetItem(_T("DEST_BAY_FK1_RD")));
		pSC_DATA->SetDEST_LEVEL_FK1_RD(m_pRsw->GetItem(_T("DEST_LEVEL_FK1_RD")));
		pSC_DATA->SetDEST_BAY_FK2_RD(m_pRsw->GetItem(_T("DEST_BAY_FK2_RD")));
		pSC_DATA->SetDEST_LEVEL_FK2_RD(m_pRsw->GetItem(_T("DEST_LEVEL_FK2_RD")));
		pSC_DATA->SetFORKPOS_FK1_RD(m_pRsw->GetItem(_T("FORKPOS_FK1_RD")));
		pSC_DATA->SetFORKPOS_FK2_RD(m_pRsw->GetItem(_T("FORKPOS_FK2_RD")));
		pSC_DATA->SetCV_WORKBENCH_RD(m_pRsw->GetItem(_T("CV_WORKBENCH_RD")));
		pSC_DATA->SetCV_WORKBENCH_SUB_RD(m_pRsw->GetItem(_T("CV_WORKBENCH_SUB_RD")));
		pSC_DATA->SetPLT_INFO_RD(m_pRsw->GetItem(_T("PLT_INFO_RD")));
		pSC_DATA->SetITN_LUGG_FK1(m_pRsw->GetItem(_T("ITN_LUGG_FK1")));
		pSC_DATA->SetITN_LUGG_FK2(m_pRsw->GetItem(_T("ITN_LUGG_FK2")));
		pSC_DATA->SetSUSPEND(m_pRsw->GetItem(_T("SUSPEND")));
		pSC_DATA->SetMC_NO(m_pRsw->GetItem(_T("MC_NO")));
		pSC_DATA->SetMC_NO_NM(m_pRsw->GetItem(_T("MC_NO_NM")));
		pSC_DATA->SetEQP_TIME(m_pRsw->GetItem( _T("EQP_TIME")));
		pSC_DATA->SetEQP_CONNECTED_YN(m_pRsw->GetItem( _T("CONNECTED_YN")));

		if(GetModified() == TRUE)
		{
			//::PostMessage(m_pDoc->m_hWndView, WM_USER_REFRESH_DIALOG, 0, m_enKind);					
		}
		m_pInfo->InvokeControl(pSC_DATA);
		//m_pInfo->InvokeControl(pSC_DATA->m_pControl);
		m_pRsw->MoveNext();	
	}
}

void CScPair::CommandProc()
{
	
}

