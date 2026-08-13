// Rtv.cpp: implementation of the CRtv class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Rtv.h"
#include "ByteArrayEx.h"
#include "RecordSetWrap.h"
#include "RtvSkinDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEVICE m_port.m_strDevice


// CRtv
//
IMPLEMENT_DYNAMIC(CRtv, CEquipment)

CRtv::CRtv(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex) : CEquipment(pDoc, pstrThreadNo, nIndex)
{
	m_enKind = CEquipment::enRTV;
	m_pInfo = new CRtvInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	m_pDoc = pDoc;
	m_pRsw = NULL;
}

CRtv::~CRtv()
{
	delete m_pInfo;
}


void CRtv::AutoRunProc()
{
	CString strRTV_NO;
	if(m_pInfo == NULL){ return; };
	if(m_pRsw == NULL){	return;	}
	if(m_pRsw->m_pRecordSet == NULL){ return;}
	
	int nRtvCnt = m_pRsw->m_pRecordSet->RecordCount;
	if(nRtvCnt < 1)
	{
		return;
	}
	m_pRsw->MoveFirst();
	
	CRtvInfo * pRtvInfo = m_pInfo;
	if(pRtvInfo == NULL){ return; }

	m_pRsw->MoveFirst(); 
	for(int nIdxFor = 0; nIdxFor < nRtvCnt; nIdxFor++)
	{
		strRTV_NO = m_pRsw->GetItem(_T("RTV_NO"));
		CRTV_DATA* pRTV_DATA = m_pInfo->m_MapRTV_DATA[strRTV_NO];
		if(pRTV_DATA == NULL)
		{ 
			m_pRsw->MoveNext();
			continue; 
		};
		pRTV_DATA->SetAUTO_MODE_RD(m_pRsw->GetItem( _T("AUTO_MODE_RD")));
		pRTV_DATA->SetSENSOR_FK_RD(m_pRsw->GetItem( _T("SENSOR_FK_RD")));
		pRTV_DATA->SetUCSTATUS_RD(m_pRsw->GetItem( _T("UCSTATUS_RD")));
		pRTV_DATA->SetPOS_H_RD(m_pRsw->GetItem( _T("POS_H_RD")));
		pRTV_DATA->SetFORKPOS_FK1_RD(m_pRsw->GetItem(_T("FORKPOS_FK1_RD")));
		pRTV_DATA->SetFORKPOS_FK2_RD(m_pRsw->GetItem(_T("FORKPOS_FK2_RD")));
		pRTV_DATA->SetERR_CODE_RD(m_pRsw->GetItem( _T("ERR_CODE_RD")));
		pRTV_DATA->SetACTIVE_MODE_RD(m_pRsw->GetItem( _T("ACTIVE_MODE_RD")));
		pRTV_DATA->SetCOMPLETE_RD(m_pRsw->GetItem( _T("COMPLETE_RD")));
		pRTV_DATA->SetJOB_TYP_RD(m_pRsw->GetItem( _T("JOB_TYP_RD")));
		pRTV_DATA->SetLUGG_NO_FK1_RD(m_pRsw->GetItem( _T("LUGG_NO_FK1_RD")));
		pRTV_DATA->SetDEPARTHS_NO_FK1_RD(m_pRsw->GetItem( _T("DEPARTHS_NO_FK1_RD")));
		pRTV_DATA->SetARRIVEHS_NO_FK1_RD(m_pRsw->GetItem( _T("ARRIVEHS_NO_FK1_RD")));
		pRTV_DATA->SetUSE_FORK_RD(m_pRsw->GetItem( _T("USE_FORK_RD")));
		pRTV_DATA->SetLUGG_NO_FK2_RD(m_pRsw->GetItem( _T("LUGG_NO_FK2_RD")));
		pRTV_DATA->SetDEPARTHS_NO_FK2_RD(m_pRsw->GetItem( _T("DEPARTHS_NO_FK2_RD")));
		pRTV_DATA->SetARRIVEHS_NO_FK2_RD(m_pRsw->GetItem( _T("ARRIVEHS_NO_FK2_RD")));
		pRTV_DATA->SetSUSPEND(m_pRsw->GetItem( _T("SUSPEND")));
		pRTV_DATA->SetITN_LUGG_FK1(m_pRsw->GetItem( _T("ITN_LUGG_FK1")));
		pRTV_DATA->SetITN_LUGG_FK2(m_pRsw->GetItem( _T("ITN_LUGG_FK2")));

		if(GetModified() == TRUE)
		{
			//::PostMessage(m_pDoc->m_hWndView, WM_USER_REFRESH_DIALOG, 0, m_enKind);					
		}
		pRtvInfo->InvokeControl(pRTV_DATA);
		m_pRsw->MoveNext();
	}
	


}
void CRtv::CommandProc()
{
}



CString CRtv::GetSelectQry() //kdh20190521
{
	CString strSql = _T("");
	strSql.Format(_T(" SELECT RD.RTV_NO,										  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.ACTIVE_MODE_RD,'0') AS ACTIVE_MODE_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.AUTO_MODE_RD,'0') AS AUTO_MODE_RD, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.COMPLETE_RD,'0') AS COMPLETE_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.SENSOR_FK_RD,'0') AS SENSOR_FK_RD, \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.UCSTATUS_RD,'0') AS UCSTATUS_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.USE_FORK_RD,'0') AS USE_FORK_RD, \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.POS_H_RD,'0') AS POS_H_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.SENSOR_FK_RD,'0') AS SENSOR_FK_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.ARRIVEHS_NO_FK1_RD,'0') AS ARRIVEHS_NO_FK1_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(ARRIVEHS_NO_FK2_RD,'0') AS ARRIVEHS_NO_FK2_RD, \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.DEPARTHS_NO_FK1_RD,'0') AS DEPARTHS_NO_FK1_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.DEPARTHS_NO_FK2_RD,'0') AS DEPARTHS_NO_FK2_RD, \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.ERR_CODE_RD,'0') AS ERR_CODE_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.JOB_TYP_RD,'0') AS JOB_TYP_RD, \n ")
		_T("        ") + m_pDoc->NVL + _T("(RD.FORKPOS_FK1_RD,'0') AS FORKPOS_FK1_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.FORKPOS_FK2_RD,'0') AS FORKPOS_FK2_RD, \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.LUGG_NO_FK1_RD,'0') AS LUGG_NO_FK1_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.LUGG_NO_FK2_RD,'0') AS LUGG_NO_FK2_RD, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.SUSPEND,'0') AS SUSPEND, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.ITN_LUGG_FK1,'0') AS ITN_LUGG_FK1, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.ITN_LUGG_FK2,'0') AS ITN_LUGG_FK2, \n")
		_T("		EM.PLC_IP, EM.PLC_PORT_FROM, EM.CONNECTED_YN, EM.USE_YN \n")
		_T("  FROM RTV_DATA RD INNER JOIN EQP_MST EM \n")
		_T("                           ON EM.WH_TYP = RD.WH_TYP \n")
		_T("                          AND RD.PLC_NO = EM.PLC_NO \n")
		_T(" WHERE RD.WH_TYP = '%s' \n")
		_T("   AND EM.EQP_TYP = '%s' \n")
		, m_pDoc->m_WH_TYP, _T("RTV"));

	return strSql;
}

void CRtv::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}

BOOL CRtv::GetModified()
{
	if(m_pDoc->m_pRtvSkinDlg == NULL)
		return FALSE;

	CRTV_DATA* pRTV_DATA = ((CRtvSkinDlg*)m_pDoc->m_pRtvSkinDlg)->m_pRTV_DATA;
	if(pRTV_DATA == NULL)
		return FALSE;

	return  pRTV_DATA->m_bModified;
}