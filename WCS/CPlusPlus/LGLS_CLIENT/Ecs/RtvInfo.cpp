// RtvInfo.cpp: implementation of the CRtvInfo class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "RtvInfo.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CRtvInfo
//
IMPLEMENT_DYNAMIC(CRtvInfo, CRtv)

CRtvInfo::CRtvInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
	m_pControl = NULL;
	m_pRTV_DATA = NULL;
	m_bModified = TRUE;
}


CRtvInfo::~CRtvInfo()
{
}


void CRtvInfo::Serialize(CArchive& ar)
{
}



COLORREF CRtvInfo::GetForkColor()
{
// 	
// 	if (m_nErrorCode == enErrorCommunication)
// 		return pConfig->m_clrDisconnect;
// 
// 	if (m_ucStatus == enStatusError)
// 		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;
// 
// 	if ((IsAutoMode() == FALSE) || (IsActive() == FALSE))
// 		return DARK_GRAY;
// 	
// 	if (!IsInvoked())
// 		return LIGHT_GRAY;
// 	
// 	switch (m_RtvInfo[0].m_nInternalJobType)
// 	{
// 	case enJobTypeAutoSto:		return pConfig->m_clrAutoSto;
// 	case enJobTypeAutoPR:		
// 	case enJobTypeAutoRet:		return pConfig->m_clrAutoRet;
// 	case enJobTypeAutoR2R:		return pConfig->m_clrAutoR2R;
// 	case enJobTypeAutoMove:		return pConfig->m_clrAutoMove;
// 	case enJobTypeAutoW2W:		return pConfig->m_clrAutoW2W;
// 	case enJobTypeSemiSto:		return pConfig->m_clrSemiSto;
// 	case enJobTypeSemiPR:		
// 	case enJobTypeSemiRet:		return pConfig->m_clrSemiRet;
// 	case enJobTypeSemiR2R:		return pConfig->m_clrSemiR2R;
// 	case enJobTypeSemiMove:		return pConfig->m_clrSemiMove;
// 	case enJobTypeSemiW2W:		return pConfig->m_clrSemiW2W;
// 	case enJobTypeManual:		return pConfig->m_clrManual;
// 	}
// 	
	return LIGHT_GRAY;
}

COLORREF CRtvInfo::GetForkColor1()
{
// 	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
// 	if(pConfig==NULL)
// 		return LIGHT_GRAY;
// 	
// 	if (m_nErrorCode == enErrorCommunication)
// 		return pConfig->m_clrDisconnect;
// 	
// 	if (m_ucStatus == enStatusError)
// 		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;
// 	
// 	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActive() == FALSE))
// 		return DARK_GRAY;
// 	
// 	if (!IsInvoked())
// 		return LIGHT_GRAY;
// 	
// 	if (m_RtvInfo[0].m_nInternalLuggNum != 0)
// 	{
// 		switch (m_RtvInfo[0].m_nInternalJobType)
// 		{
// 		case enJobTypeAutoSto:		return pConfig->m_clrAutoSto;
// 		case enJobTypeAutoPR:		
// 		case enJobTypeAutoRet:		return pConfig->m_clrAutoRet;
// 		case enJobTypeAutoR2R:		return pConfig->m_clrAutoR2R;
// 		case enJobTypeAutoMove:		return pConfig->m_clrAutoMove;
// 		case enJobTypeAutoW2W:		return pConfig->m_clrAutoW2W;
// 		case enJobTypeSemiSto:		return pConfig->m_clrSemiSto;
// 		case enJobTypeSemiPR:		
// 		case enJobTypeSemiRet:		return pConfig->m_clrSemiRet;
// 		case enJobTypeSemiR2R:		return pConfig->m_clrSemiR2R;
// 		case enJobTypeSemiMove:		return pConfig->m_clrSemiMove;
// 		case enJobTypeSemiW2W:		return pConfig->m_clrSemiW2W;
// 		case enJobTypeManual:		return pConfig->m_clrManual;
// 		}
// 	}
// 	
// 	if (m_RtvInfo[1].m_nInternalJobType != 0)
// 		return LIGHT_GRAY;

	return LIGHT_GRAY;
	//return BLACK;
}

COLORREF CRtvInfo::GetForkColor1(CRTV_DATA* pRTV_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pRTV_DATA->V_ERR_CODE_RD != _T("0") && pRTV_DATA->V_ERR_CODE_RD != _T("0000") && pRTV_DATA->V_ERR_CODE_RD != _T(""))
		return pConfig->m_clrUSER_COLOR_ERROR;

	//if (pRTV_DATA->V_ERR_STA_FK2_RD != _T("0"))
	//	return pConfig->m_clrUSER_COLOR_ERROR;

	//if (pRTV_DATA->V_SENSOR_FK_RD  != _T("0") || 
	//	pRTV_DATA->V_AUTO_MODE_RD != _T("0") || 
	//	pRTV_DATA->V_ACTIVE_MODE_RD != _T("0"))
	//	return DARK_GRAY;

	if (pRTV_DATA->V_AUTO_MODE_RD != _T("1") ||
		pRTV_DATA->V_ACTIVE_MODE_RD != _T("1"))
		return LIGHT_GRAY;	// [LGLS] 평상시 RTV도 SC 처럼 밝은 회색

	int nJobTypTmp = CConvert::ToInt(pRTV_DATA->V_JOB_TYP_RD);
	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
	}



	return LIGHT_GRAY;
}

COLORREF CRtvInfo::GetForkColor2()
{
// 	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
// 	if(pConfig==NULL)
// 		return LIGHT_GRAY;	
// 	if (m_nErrorCode == enErrorCommunication)
// 		return pConfig->m_clrDisconnect;
// 	
// 	if (m_ucStatus == enStatusError)
// 		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;
// 	
// 	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActive() == FALSE))
// 		return DARK_GRAY;
// 	
// 	if (!IsInvoked())
// 		return LIGHT_GRAY;
// 	
// 	if (m_RtvInfo[1].m_nInternalLuggNum != 0)
// 	{
// 		switch (m_RtvInfo[1].m_nInternalJobType)
// 		{
// 		case enJobTypeAutoSto:		return pConfig->m_clrAutoSto;
// 		case enJobTypeAutoPR:		return pConfig->m_clrAutoW2W;		// %%%%%%
// 		case enJobTypeAutoRet:		return pConfig->m_clrAutoRet;
// 		case enJobTypeAutoR2R:		return pConfig->m_clrAutoR2R;
// 		case enJobTypeAutoMove:		return pConfig->m_clrAutoMove;
// 		case enJobTypeAutoW2W:		return pConfig->m_clrAutoW2W;
// 		case enJobTypeSemiSto:		return pConfig->m_clrSemiSto;
// 		case enJobTypeSemiPR:		return pConfig->m_clrSemiW2W;		// %%%%%%
// 		case enJobTypeSemiRet:		return pConfig->m_clrSemiRet;
// 		case enJobTypeSemiR2R:		return pConfig->m_clrSemiR2R;
// 		case enJobTypeSemiMove:		return pConfig->m_clrSemiMove;
// 		case enJobTypeSemiW2W:		return pConfig->m_clrSemiW2W;		
// 		case enJobTypeManual:		return pConfig->m_clrManual;
// 		}
// 	}
// 	
// 	if (m_RtvInfo[0].m_nInternalJobType != 0)
// 		return LIGHT_GRAY;
// 	
	return LIGHT_GRAY;
	//return BLACK;
}

COLORREF CRtvInfo::GetForkColor2(CRTV_DATA* pRTV_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pRTV_DATA->V_ERR_CODE_RD != _T("0") && pRTV_DATA->V_ERR_CODE_RD != _T("0000") && pRTV_DATA->V_ERR_CODE_RD != _T(""))
		return pConfig->m_clrUSER_COLOR_ERROR;

	//if (pRTV_DATA->V_ERR_STA_FK2_RD != _T("0"))
	//	return pConfig->m_clrUSER_COLOR_ERROR;

	//if (pRTV_DATA->V_SENSOR_FK_RD  != _T("0") || 
	//	pRTV_DATA->V_AUTO_MODE_RD != _T("0") || 
	//	pRTV_DATA->V_ACTIVE_MODE_RD != _T("0"))
	//	return DARK_GRAY;


	if (pRTV_DATA->V_AUTO_MODE_RD != _T("1") ||
		pRTV_DATA->V_ACTIVE_MODE_RD != _T("1"))

		return LIGHT_GRAY;	// [LGLS] 평상시 RTV 밝은 회색(보이는 포크=GetForkColor2)

	int nJobTypTmp = CConvert::ToInt(pRTV_DATA->V_JOB_TYP_RD);
	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
	}



	return LIGHT_GRAY;
}


COLORREF CRtvInfo::GetRailColor()
{
	if(m_pRTV_DATA == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
	}

	// [LGLS] any suspend -> RED (match SC/original ECS convention)
	if (m_pRTV_DATA->V_SUSPEND == _T("1") || m_pRTV_DATA->V_SUSPEND == _T("2") || m_pRTV_DATA->V_SUSPEND == _T("3"))
		return RED;

	if (m_pRTV_DATA->V_ERR_CODE_RD != _T("0") && m_pRTV_DATA->V_ERR_CODE_RD != _T("0000") && m_pRTV_DATA->V_ERR_CODE_RD != _T(""))
		return RED;

	// [LGLS 2026-07-22] m_bInvoke: JOB_TYP_RD(=RTV_DATA.JOB_TYP_OD)=지시 중이면 BLUE, 완료/삭제 클리어 시 BLACK
	return (m_pRTV_DATA->V_JOB_TYP_RD != _T("0") && m_pRTV_DATA->V_JOB_TYP_RD != _T("")) ? BLUE : BLACK;
}

COLORREF CRtvInfo::GetRailColor(CRTV_DATA* pRTV_DATA)
{
	if(pRTV_DATA == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
	}

	// [LGLS] any suspend -> RED
	if (pRTV_DATA->V_SUSPEND == _T("1") || pRTV_DATA->V_SUSPEND == _T("2") || pRTV_DATA->V_SUSPEND == _T("3"))
		return RED;

	if (pRTV_DATA->V_ERR_CODE_RD != _T("0") && pRTV_DATA->V_ERR_CODE_RD != _T("0000") && pRTV_DATA->V_ERR_CODE_RD != _T(""))
		return RED;

	// [LGLS 2026-07-22] m_bInvoke: JOB_TYP_RD(=RTV_DATA.JOB_TYP_OD)=지시 중이면 BLUE, 완료/삭제 클리어 시 BLACK
	return (pRTV_DATA->V_JOB_TYP_RD != _T("0") && pRTV_DATA->V_JOB_TYP_RD != _T("")) ? BLUE : BLACK;
}

COLORREF CRtvInfo::GetPostColor()
{
	//return (m_ucProdLoad != enProdEmpty) ? BLUE : DARK_GRAY;
	return DARK_GRAY;
}

int CRtvInfo::GetRtvPosition()
{
// 	int nCount = m_RtvPosInfo.GetSize();
// 	for (int i=0;i<nCount;i++)
// 	{
// 		if (m_RtvPosInfo[i].m_nDurgPos == m_ucHorizontalPos)
// 			return m_RtvPosInfo[i].m_nEcsPos;
// 	}

	return 0;
}

// [LGLS 2026-08-22] 창고 모니터링 보기를 RGV 에도 적용한다(S/C 와 같은 규약).
//   호기 번호가 평상시 표시값이고, 작업이 있을 때만 작업번호(1)/제품정보(2)로 바뀐다.
//   글자색 : 작업정보(JOB_MST)에 실재하면 검정, 실물만 남은 잔재면 흰색.
void CRtvInfo::CalcRtvText(CRTV_DATA* pData, CString& strOut, COLORREF& clrOut)
{
	strOut = _T(" ");
	clrOut = RGB(0, 0, 0);
	if (pData == NULL || m_pEquipment == NULL || m_pEquipment->m_pDoc == NULL) return;

	int nMode = m_pEquipment->m_pDoc->m_nTrackTextMode;
	CString strLugg = pData->V_LUGG_NO_FK1_RD;
	strLugg.Trim();
	BOOL bHasJob = (!strLugg.IsEmpty() && strLugg != _T("0") && strLugg != _T("0000"));

	// [LGLS 2026-08-22] 설비 데이터만으로는 표시가 끊긴다.
	//   지시 전에는 관측·지시값이 모두 비고, 지시 직후에는 작업색만 먼저 켜지며,
	//   완료 뒤에는 지시값이 이전 작업 번호로 남는다(5호기 입고에서 확인).
	//   그래서 작업번호가 비면 작업정보에서 이 호기에 물려 있는 진행 중 작업을 가져온다.
	if (!bHasJob)
	{
		CString strJob = m_pEquipment->m_pDoc->GetVehicleJobNo(pData->K_RTV_NO);
		strJob.Trim();
		if (!strJob.IsEmpty() && strJob != _T("0") && strJob != _T("0000"))
		{
			strLugg = strJob;
			bHasJob = TRUE;
		}
	}

	// [LGLS 2026-08-22] 호기 번호는 컨트롤이 이미 m_strText 로 포크 위에 그린다(레이아웃 text 속성).
	//   그래서 표시할 것이 없으면 빈 문자열을 돌려주고 컨트롤이 호기를 그대로 쓰게 둔다.
	//   (여기서 호기를 또 넣었더니 같은 자리에 두 번 찍혀 겹쳐 보였다.)
	strOut = _T("");
	if (!bHasJob) return;						// 작업 없음 -> 호기 표시

	if (m_pEquipment->m_pDoc->IsJobInJobMst(strLugg) == FALSE)
		clrOut = RGB(255, 255, 255);

	if (nMode == 1)
	{
		strOut = strLugg;
	}
	else if (nMode == 2)
	{
		if (m_strRtvProdLugg != strLugg)
		{
			m_strRtvProdLugg = strLugg;
			m_strRtvProdVal = _T("");
			CString strSql;
			strSql.Format(_T(" SELECT TOP 1 ISNULL(BCR_BOTTOM, ' ') AS BCR FROM JOB_MST WHERE LUGG_NO = '%s' ORDER BY INS_DT DESC "), m_strRtvProdLugg);
			int nRowCnt = 0;
			CString strMsg = _T("");
			_RecordsetPtr pRs = m_pEquipment->m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMsg);
			if (nRowCnt > 0)
			{
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRs);
				pRsw->MoveFirst();
				m_strRtvProdVal = pRsw->GetItem(_T("BCR"));
				delete pRsw;
			}
		}
		m_strRtvProdVal.TrimRight();
		if (!m_strRtvProdVal.IsEmpty()) strOut = m_strRtvProdVal;
	}
}

void CRtvInfo::ApplyRtvTextMode(CRTV_DATA* pData, CDciRvCtrl* c1)
{
	CString strVal; COLORREF clrJob;
	CalcRtvText(pData, strVal, clrJob);
	if (c1) c1->SetExtraTextSafe(strVal, clrJob);
}

void CRtvInfo::InvokeControl(CRTV_DATA* pRTV_DATA)
{
	if(pRTV_DATA == NULL)
	{  
		return;
	};

	if (pRTV_DATA->m_bModified == FALSE)
		return; 

	//DEBUGER_ASSERT_VALID(m_pControl != NULL);

	BOOL bErase = FALSE;	// (m_pControl->m_nForkPos != m_wHorizontalPos);
	ApplyRtvTextMode(pRTV_DATA, pRTV_DATA->m_pControl);	// [LGLS 2026-08-22] 보기 모드 문자 반영

	int nPos = pRTV_DATA->m_MapRtvPosition[pRTV_DATA->V_POS_H_RD];
	if (pRTV_DATA->m_pControl)				
	{
		bErase = TRUE;
		pRTV_DATA->m_pControl->m_nForkPos = (nPos < 0 || nPos > 18) ? 0 : nPos;
	}


	// [LGLS] RTV cargo-detect indicator.
	//   Source column is RTV_DATA.SENSOR_RTV_RD (Rtv.cpp aliases it AS SENSOR_FK_RD): 0=empty, 1=loaded.
	//   DciRvCtrl draws the cargo marker from m_nProd: 1 -> rcForkL1, 2 -> rcForkL2, 3 -> both.
	//   rcForkL1 is the SINGLE-fork rect; rcForkL2 exists only for Twin/Double (see DciRvCtrl.cpp).
	//   The RTV control is forktype="0" (enSingle) in EcsLayout1.xml, so the old mapping (1 -> nProd 2)
	//   drew onto a second fork that does not exist -> cargo NEVER showed, only the data/colour did.
	//   ScInfo::InvokeControl(pSC_DATA) maps every loaded state to 1 for exactly this reason.
	CString strSENSOR_FK_RD = m_pRTV_DATA->V_SENSOR_FK_RD;//0;
	int nProd = 0;
	if (strSENSOR_FK_RD == _T("1"))
		nProd  = 1;
	else if (strSENSOR_FK_RD == _T("2"))
		nProd  = 1;
	else if (strSENSOR_FK_RD == _T("3"))
		nProd  = 1;

	if (pRTV_DATA->m_pControl)
	{
		pRTV_DATA->m_pControl->m_nProd = nProd;
		pRTV_DATA->m_pControl->m_clrRail = GetRailColor(pRTV_DATA);
		//	m_pControl->m_clrWing = GetPostColor();
		pRTV_DATA->m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}

	if (pRTV_DATA->m_pControl)
		pRTV_DATA->m_pControl->m_clrFork = GetForkColor1(pRTV_DATA);

	if (pRTV_DATA->m_pControl)
	{
		pRTV_DATA->m_pControl->m_clrFork = GetForkColor2(pRTV_DATA);		
		pRTV_DATA->m_pControl->m_clrFork2 = GetForkColor1(pRTV_DATA);		
	}
	pRTV_DATA->m_bModified = FALSE;
}
void CRtvInfo::InvokeControl()
{

	if(m_pRTV_DATA == NULL)
	{  
		return;
	};

	if (m_pRTV_DATA->m_bModified == FALSE)
		return;

	//DEBUGER_ASSERT_VALID(m_pControl != NULL);
	BOOL bErase = FALSE;	// (m_pControl->m_nForkPos != m_wHorizontalPos);

	ApplyRtvTextMode(m_pRTV_DATA, m_pControl);	// [LGLS 2026-08-22] 보기 모드 문자 반영

	if (m_pControl)						
		m_pControl->m_nForkPos = 1;//(m_wHorizontalPos >= 254) ? 1 : m_wHorizo

	if (m_pControl)
		m_pControl->m_clrFork = GetForkColor1();

	if (m_pControl)
	{
		m_pControl->m_clrFork = GetForkColor2();		
		m_pControl->m_clrFork2 = GetForkColor1();		
	}


	CString strSENSOR_FK_RD = m_pRTV_DATA->V_SENSOR_FK_RD;//0;
	int nProd = 0;
	if (strSENSOR_FK_RD == _T("1"))
		nProd  = 2;
	else if (strSENSOR_FK_RD == _T("2"))
		nProd  = 1;
	else if (strSENSOR_FK_RD == _T("3"))
		nProd  = 3;

	if (m_pControl)
	{
		m_pControl->m_nProd = nProd;
		m_pControl->m_clrRail = GetRailColor();
		//	m_pControl->m_clrWing = GetPostColor();
		m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}

	m_pRTV_DATA->m_bModified = FALSE;
}

CRTV_DATA* CRtvInfo::CreateRTV_DATA( CString pstrEQP_NO )
{
	CRTV_DATA* pRTV_DATA = new CRTV_DATA(m_pEquipment->m_WH_TYP, GetStringPLC_NO(), GetStringEQP_NO(pstrEQP_NO));
	m_pRTV_DATA = pRTV_DATA;
	return pRTV_DATA;
	
}

CString CRtvInfo::GetStringPLC_NO()
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), m_pEquipment->m_nNumber);
	return strPLC_NO;
}

CString CRtvInfo::GetStringPLC_NO(int pnPLC_NO)
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), pnPLC_NO);
	return strPLC_NO;
}

CString CRtvInfo::GetStringEQP_NO(CString pstrEQP_NO)
{
	// [LGLS] fix: %05s space-padding made map key "  801" != click/DB key "801"
	//        -> RTV dialog never opened and status never refreshed. Use value as-is.
	return pstrEQP_NO;
}