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
	//占쏙옙占쏙옙 X
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

	// [LGLS 2026-07-24] 설비 데이터 CString 쓰기 구간
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
		pSC_DATA->SetLUGG_NO_FK1_OD(m_pRsw->GetItem( _T("LUGG_NO_FK1_OD")));	// [LGLS 2026-08-22] 지시값(설비 반영 전 구간 표시용)
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

	// [LGLS 2026-09-02] 크레인별 출고 대기 작업 건수 표시(사용자 요청)
	//   레이아웃 컨트롤 id=90000901~90000905 (S/C #1~5 행, 랙 왼쪽 칸)
	//   대상 = 출고작업(JOB_TYP='2') 중 작업상태 [20] S/C 구동 대기, 크레인은 출발지(START_POS 901~905)로 판정
	RefreshScWaitCount();
}

void CScPair::RefreshScWaitCount()
{
	if (m_pDoc == NULL) return;

	CString strSql;
	strSql.Format(
		_T(" SELECT START_POS, COUNT(*) AS CNT ")
		_T("   FROM JOB_MST ")
		_T("  WHERE WH_TYP = '%s' ")
		_T("    AND JOB_TYP = '2' ")
		_T("    AND JOB_STATUS = '20' ")
		_T("    AND START_POS IN ('901','902','903','904','905') ")
		_T("  GROUP BY START_POS "),
		(LPCTSTR)m_pDoc->m_WH_TYP);

	int nRowCnt = -1;
	CString strMessage;
	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr(strSql, nRowCnt, strMessage);
	if (nRowCnt < 0) return;

	int nCnt[5] = { 0, 0, 0, 0, 0 };
	if (nRowCnt > 0)
	{
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
		pRsw->MoveFirst();
		for (int nRow = 0; nRow < nRowCnt; nRow++)
		{
			CString strPos = pRsw->GetItem(_T("START_POS"));
			strPos.Trim();
			int nSc = _ttoi(strPos) - 900;
			if (nSc >= 1 && nSc <= 5)
				nCnt[nSc - 1] = _ttoi(pRsw->GetItem(_T("CNT")));
			pRsw->MoveNext();
		}
		delete pRsw;
	}

	for (int nSc = 1; nSc <= 5; nSc++)
	{
		CString strCid;
		strCid.Format(_T("9000090%d"), nSc);
		CDciControl* pCtrl = m_pDoc->GetDciControl_FindAllLayout(strCid);
		if (pCtrl == NULL) continue;

		CString strTxt;
		if (nCnt[nSc - 1] > 0)
			strTxt.Format(_T("출고대기 %d"), nCnt[nSc - 1]);

		if (pCtrl->m_strText != strTxt)
		{
			pCtrl->m_strText = strTxt;
			pCtrl->m_clrFgColor = RGB(128, 0, 192);
			pCtrl->InvalidateControl(m_pDoc->m_hWndView, TRUE);
		}
	}
}

void CScPair::CommandProc()
{
	
}

