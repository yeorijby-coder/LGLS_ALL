// ScInfo.cpp: implementation of the CScInfo class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "ScInfo.h"
#include "RecordSetWrap.h"		// [LGLS 2026-08-22] 제품정보 조회

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 
//
IMPLEMENT_SERIAL(CScInfo, CInfo, 0)

CScInfo::CScInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
	m_pControl = NULL;
	m_pControl2 = NULL;
	m_pControl3 = NULL;
	m_pControl4 = NULL;
	m_pControl5 = NULL;
	m_pSC_DATA = NULL;
}

CScInfo::~CScInfo()
{
	//@@@@
	if(m_pSC_DATA != NULL)
		delete m_pSC_DATA;

}



void CScInfo::Serialize(CArchive& ar)
{
}

COLORREF CScInfo::GetForkColor1()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

 	if (m_pSC_DATA->V_ERR_CODE_RD != _T("0000") && m_pSC_DATA->V_ERR_CODE_RD != _T("0") && m_pSC_DATA->V_ERR_CODE_RD != _T(""))
 		return pConfig->m_clrUSER_COLOR_ERROR;
 
 	if (m_pSC_DATA->V_ERR_STA_FK1_RD != _T("0"))
 		return pConfig->m_clrUSER_COLOR_ERROR;
 
 	if (m_pSC_DATA->V_ONLINE_MODE_RD != _T("1") || m_pSC_DATA->V_AUTO_MODE_RD != _T("1") || m_pSC_DATA->V_ACTIVE_MODE_RD != _T("1"))
 		return DARK_GRAY;
 
 	// [LGLS 2026-08-23] 크레인이 작업을 받아 화물을 뜨러 가는 동안에는 설비에 작업번호가
	//   아직 실리지 않아(ITN_LUGG_FK1=0) 여기서 회색으로 빠졌다 - "색 없이 움직인다" 던 증상.
	//   설비 데이터가 비어 있어도 그 호기가 작업정보상 작업을 물고 있으면 그 작업색을 낸다.
	int nJobTypTmp = CConvert::ToInt(m_pSC_DATA->V_JOB_TYP_RD);

	// 설비에 작업번호가 실리지 않았어도, 작업정보상 이 호기가 작업을 물고 있으면 회색으로 빠지지 않는다.
	// [LGLS 2026-08-31] ★번호와 색이 서로 다른 필드를 봐서 어긋났다★ (사용자 지적)
	//   번호(CalcScText)는 차상 PALLET_ON_VEHICLE_RD(=V_LUGG_NO_FK1_RD)를 쓰는데
	//   색은 포크 ITN_LUGG_FK1 만 봤다. 크레인이 포크를 비운 뒤에도 차상 값은 잠시 남으므로
	//   ★번호는 그려지는데 색만 회색★ 이 됐다.
	//   번호와 같은 기준으로 판단한다 : 차상 → 포크 → 작업정보 캐시 순.
	CString strHeld = m_pSC_DATA->V_LUGG_NO_FK1_RD;   // 차상(실적재)
	strHeld.Trim();
	if (strHeld.IsEmpty() || strHeld == _T("0") || strHeld == _T("0000"))
	{
		strHeld = m_pSC_DATA->V_ITN_LUGG_FK1;           // 포크
		strHeld.Trim();
	}
	if (strHeld.IsEmpty() || strHeld == _T("0") || strHeld == _T("0000"))
	{
		strHeld = m_pEquipment->m_pDoc->GetVehicleJobNo(m_pSC_DATA->K_SC_NO);   // 작업정보 캐시
		strHeld.Trim();
	}
	if (strHeld.IsEmpty() || strHeld == _T("0") || strHeld == _T("0000"))
		return LIGHT_GRAY;

	// 작업 구분(JOB_TYP_RD)은 실경로에서 늘 채워지지는 않는다. 비어 있으면 작업정보에서 가져온다.
	//   (구분을 모르면 아래 switch 가 통째로 빠져 색 없이 움직이는 것처럼 보였다)
	if (nJobTypTmp == 0)
		nJobTypTmp = CConvert::ToInt(m_pEquipment->m_pDoc->GetVehicleJobTyp(m_pSC_DATA->K_SC_NO));
	// [LGLS 2026-08-31] 캐시에도 없으면 크레인이 든 작업번호로 직접 찾는다(RtvInfo 와 같은 방식).
	if (nJobTypTmp == 0)
		nJobTypTmp = CConvert::ToInt(m_pEquipment->m_pDoc->GetJobTypOfLugg(strHeld));

	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoA2A			: return pConfig->m_clrUSER_COLOR_ATA;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_SEMI_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_SEMI_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_SEMI_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_SEMI_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_SEMI_PR;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
 	}

	return BLACK;
}

COLORREF CScInfo::GetForkColor1(CSC_DATA* pSC_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pSC_DATA->V_ERR_CODE_RD != _T("0000") && pSC_DATA->V_ERR_CODE_RD != _T("0") && pSC_DATA->V_ERR_CODE_RD != _T(""))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (pSC_DATA->V_ERR_STA_FK1_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (pSC_DATA->V_ONLINE_MODE_RD == _T("0") || 
		pSC_DATA->V_AUTO_MODE_RD   == _T("0") || 
		pSC_DATA->V_ACTIVE_MODE_RD == _T("0"))
		return DARK_GRAY;

	// [LGLS 2026-08-31] 이 경로도 설비의 JOB_TYP_RD 만 봐서, 번호는 그려지는데
	//   색만 회색으로 빠지는 창이 있었다. 크레인이 든 번호로 작업정보를 찾아 보완한다.
	CString strHeldP = pSC_DATA->V_LUGG_NO_FK1_RD;   // 차상
	strHeldP.Trim();
	if (strHeldP.IsEmpty() || strHeldP == _T("0") || strHeldP == _T("0000"))
	{ strHeldP = pSC_DATA->V_ITN_LUGG_FK1; strHeldP.Trim(); }
	if (strHeldP.IsEmpty() || strHeldP == _T("0") || strHeldP == _T("0000"))
	{ strHeldP = m_pEquipment->m_pDoc->GetVehicleJobNo(pSC_DATA->K_SC_NO); strHeldP.Trim(); }
	BOOL bHeldP = (!strHeldP.IsEmpty() && strHeldP != _T("0") && strHeldP != _T("0000"));

	int nJobTypTmp = CConvert::ToInt(pSC_DATA->V_JOB_TYP_RD);
	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoA2A			: return pConfig->m_clrUSER_COLOR_ATA;
	case enJobTypeAutoPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_SEMI_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_SEMI_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_SEMI_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_SEMI_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_SEMI_PR;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
	}

	// 설비 지시값이 비면 작업정보에서 작업구분을 찾는다.
	if (bHeldP)
	{
		int nTypJob = CConvert::ToInt(m_pEquipment->m_pDoc->GetVehicleJobTyp(pSC_DATA->K_SC_NO));
		if (nTypJob == 0) nTypJob = CConvert::ToInt(m_pEquipment->m_pDoc->GetJobTypOfLugg(strHeldP));
		switch (nTypJob)
		{
		case enJobTypeAutoSto		: return pConfig->m_clrUSER_COLOR_STO;
		case enJobTypeAutoRet		: return pConfig->m_clrUSER_COLOR_RET;
		case enJobTypeAutoR2R		: return pConfig->m_clrUSER_COLOR_RTR;
		case enJobTypeAutoMove		: return pConfig->m_clrUSER_COLOR_MOVE;
		case enJobTypeAutoA2A		: return pConfig->m_clrUSER_COLOR_ATA;
		case enJobTypeAutoPR		: return pConfig->m_clrUSER_COLOR_RET;
		case enJobTypeSemiSto		: return pConfig->m_clrUSER_COLOR_SEMI_STO;
		case enJobTypeSemiRet		: return pConfig->m_clrUSER_COLOR_SEMI_RET;
		case enJobTypeSemiR2R		: return pConfig->m_clrUSER_COLOR_SEMI_RTR;
		case enJobTypeSemiMove		: return pConfig->m_clrUSER_COLOR_SEMI_MOVE;
		case enJobTypeSemiPR		: return pConfig->m_clrUSER_COLOR_SEMI_PR;
		case enJobTypeManual		: return pConfig->m_clrUSER_COLOR_MANUAL;
		}
	}

	if (pSC_DATA->V_ONLINE_MODE_RD == _T("1") && 
		pSC_DATA->V_AUTO_MODE_RD   == _T("1") && 
		pSC_DATA->V_ACTIVE_MODE_RD == _T("1"))
		return LIGHT_GRAY;
	else
		return DARK_GRAY;

	return BLACK;
}

COLORREF CScInfo::GetForkColor2()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_pSC_DATA->V_ERR_CODE_RD != _T("0000") && m_pSC_DATA->V_ERR_CODE_RD != _T("0") && m_pSC_DATA->V_ERR_CODE_RD != _T(""))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (m_pSC_DATA->V_ERR_STA_FK2_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (m_pSC_DATA->V_ONLINE_MODE_RD != _T("1") || m_pSC_DATA->V_AUTO_MODE_RD != _T("1") || m_pSC_DATA->V_ACTIVE_MODE_RD != _T("1"))
		return DARK_GRAY;

	if (m_pSC_DATA->V_ITN_LUGG_FK2 == _T("0") || m_pSC_DATA->V_ITN_LUGG_FK2 == _T("0000"))
		return LIGHT_GRAY;

	int nJobTypTmp = CConvert::ToInt(m_pSC_DATA->V_JOB_TYP_RD);
	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoA2A			: return pConfig->m_clrUSER_COLOR_ATA;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_SEMI_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_SEMI_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_SEMI_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_SEMI_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_SEMI_PR;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
	}

	return BLACK;
}

COLORREF CScInfo::GetForkColor2(CSC_DATA* pSC_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pSC_DATA->V_ERR_CODE_RD != _T("0000") && pSC_DATA->V_ERR_CODE_RD != _T("0") && pSC_DATA->V_ERR_CODE_RD != _T(""))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (pSC_DATA->V_ERR_STA_FK1_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (pSC_DATA->V_ONLINE_MODE_RD == _T("0") || 
		pSC_DATA->V_AUTO_MODE_RD   == _T("0") || 
		pSC_DATA->V_ACTIVE_MODE_RD == _T("0"))
		return DARK_GRAY;

	int nJobTypTmp = CConvert::ToInt(pSC_DATA->V_JOB_TYP_RD);
	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoA2A			: return pConfig->m_clrUSER_COLOR_ATA;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_SEMI_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_SEMI_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_SEMI_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_SEMI_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_SEMI_PR;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
	}

	if (pSC_DATA->V_ONLINE_MODE_RD == _T("1") && 
		pSC_DATA->V_AUTO_MODE_RD   == _T("1") && 
		pSC_DATA->V_ACTIVE_MODE_RD == _T("1"))
		return LIGHT_GRAY;
	else
		return DARK_GRAY;

	return BLACK;
}


COLORREF CScInfo::GetForkColor()
{
// 	if (m_nErrorCode == enScErrorCommunication)
// 		return pConfig->m_clrDisconnect;
// 
// 	if (IsErrorStatus())
// 		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;
// 
// 	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActive() == FALSE))
// 		return DARK_GRAY;
// 
// 	if (IsInvoked() == FALSE)
// 		return LIGHT_GRAY;
// 
// 	switch (m_nInternalJobType)
// 	{
// 	case enJobTypeAutoSto			: return pConfig->m_clrAutoSto;
// 	case enJobTypeAutoRet			: return pConfig->m_clrAutoRet;
// //	case enJobTypeAutoPriorityRet	: return pConfig->m_clrAutoPriorityRet;
// 	case enJobTypeAutoR2R			: return pConfig->m_clrAutoR2R;
// 	case enJobTypeAutoMove			: return pConfig->m_clrAutoMove;
// 	case enJobTypeAutoPR			: return pConfig->m_clrAutoW2W;
// 	case enJobTypeSemiSto			: return pConfig->m_clrSemiSto;
// 	case enJobTypeSemiRet			: return pConfig->m_clrSemiRet;
// 	case enJobTypeSemiR2R			: return pConfig->m_clrSemiR2R;
// 	case enJobTypeSemiMove			: return pConfig->m_clrSemiMove;
// 	case enJobTypeSemiPR			: return pConfig->m_clrSemiW2W;
// 	case enJobTypeManual			: return pConfig->m_clrManual;
// 	}

	return BLACK;
}

COLORREF CScInfo::GetRailColor()
{
	if(m_pSC_DATA == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
	}

	// [LGLS] fix: any suspend(in/out/all stop) -> RED; idle no-longer always-colored.
	if (m_pSC_DATA->V_SUSPEND == _T("1") || m_pSC_DATA->V_SUSPEND == _T("2") || m_pSC_DATA->V_SUSPEND == _T("3"))
		return RED;

	// carrying luggage -> BLUE, otherwise idle -> BLACK (was tautology "!=0 || !=0000" == always true)
	if (m_pSC_DATA->V_ERR_CODE_RD != _T("0000") && m_pSC_DATA->V_ERR_CODE_RD != _T("0") && m_pSC_DATA->V_ERR_CODE_RD != _T(""))
		return RED;

	// [LGLS 2026-07-22] m_bInvoke: JOB_TYP_RD(지시 21~완료 25 구간, 스케줄러 유지)=지시 중이면 BLUE, 아니면 BLACK
	return (m_pSC_DATA->V_JOB_TYP_RD != _T("0") && m_pSC_DATA->V_JOB_TYP_RD != _T("")) ? BLUE : BLACK;
	//RAIL 색상
}

COLORREF CScInfo::GetRailColor(CSC_DATA* pSC_DATA)
{
	if(pSC_DATA == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
	}

	// [LGLS] fix: any suspend(in/out/all stop) -> RED
	if (pSC_DATA->V_SUSPEND == _T("1") || pSC_DATA->V_SUSPEND == _T("2") || pSC_DATA->V_SUSPEND == _T("3"))
		return RED;

	// error code stored as "0" (1-char) or "0000" -> treat both as no-error
	if (pSC_DATA->V_ERR_CODE_RD != _T("0000") && pSC_DATA->V_ERR_CODE_RD != _T("0") && pSC_DATA->V_ERR_CODE_RD != _T(""))
	{
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RAIL_ERROR;
	}

	// [LGLS 2026-07-22] m_bInvoke: JOB_TYP_RD(지시 21~완료 25 구간, 스케줄러 유지)=지시 중이면 BLUE, 아니면 BLACK
	return (pSC_DATA->V_JOB_TYP_RD != _T("0") && pSC_DATA->V_JOB_TYP_RD != _T("")) ? BLUE : BLACK;
}

COLORREF CScInfo::GetPostColor()
{
 	/*if (m_nForkType == 1 || m_nForkType == 2)
 	{
 		return (m_ucProdLoad != enProdEmpty) ? BLUE : DARK_GRAY;	
 	}			
 	else
 		return (m_ucProdLoad == enProdExistFork1) ? BLUE : DARK_GRAY;*/

	return BLUE;
}

// [LGLS 2026-08-22] 창고 모니터링 보기를 크레인에도 적용한다(컨베이어와 같은 규약).
//   1=작업번호(싣고 있는 LUGG) / 0=호기(크레인은 트랙이 없으므로 S/C 번호) / 2=제품정보
//   글자색 : 작업정보(JOB_MST)에 실재하는 작업이면 검정, 실물만 남은 잔재면 흰색.
void CScInfo::CalcScText(CSC_DATA* pData, CString& strOut, COLORREF& clrOut)
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
		CString strJob = m_pEquipment->m_pDoc->GetVehicleJobNo(pData->K_SC_NO);
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

	// [LGLS 2026-08-31] ★크레인에 색 없이 번호만 남으면 안 된다★ (사용자 지시)
	//   색(GetForkColor1)은 작업정보의 작업구분으로 낸다. 작업정보에 없는 번호는
	//   낼 색이 없으므로 번호도 그리지 않는다 - 그래야 번호와 색이 항상 함께 있다.
	//   (설비 차상값은 다음 작업 전까지 이전 번호를 들고 있어 잔재가 생긴다.
	//    그 잔재는 S/C 상태창에서 설비값 그대로 볼 수 있다.)
	if (m_pEquipment->m_pDoc->IsJobInJobMst(strLugg) == FALSE)
		return;

	if (nMode == 1)
	{
		strOut = strLugg;
	}
	else if (nMode == 2)
	{
		if (m_strScProdLugg != strLugg)
		{
			m_strScProdLugg = strLugg;
			m_strScProdVal = _T("");
			CString strSql;
			strSql.Format(_T(" SELECT TOP 1 ISNULL(BCR_BOTTOM, ' ') AS BCR FROM JOB_MST WHERE LUGG_NO = '%s' ORDER BY INS_DT DESC "), m_strScProdLugg);
			int nRowCnt = 0;
			CString strMsg = _T("");
			_RecordsetPtr pRs = m_pEquipment->m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMsg);
			if (nRowCnt > 0)
			{
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRs);
				pRsw->MoveFirst();
				m_strScProdVal = pRsw->GetItem(_T("BCR"));
				delete pRsw;
			}
		}
		m_strScProdVal.TrimRight();
		if (!m_strScProdVal.IsEmpty()) strOut = m_strScProdVal;		// 제품정보가 없으면 호기 유지
	}
}

void CScInfo::ApplyScTextMode(CSC_DATA* pData, CDciRvCtrl* c1, CDciRvCtrl* c2, CDciRvCtrl* c3, CDciRvCtrl* c4, CDciRvCtrl* c5)
{
	CString strVal; COLORREF clrJob;
	CalcScText(pData, strVal, clrJob);
	if (c1) c1->SetExtraTextSafe(strVal, clrJob);
	if (c2) c2->SetExtraTextSafe(strVal, clrJob);
	if (c3) c3->SetExtraTextSafe(strVal, clrJob);
	if (c4) c4->SetExtraTextSafe(strVal, clrJob);
	if (c5) c5->SetExtraTextSafe(strVal, clrJob);
}

void CScInfo::InvokeControl()
{
	if(m_pSC_DATA == NULL)
	{  
		return;
	};

  	// [LGLS 2026-08-31] ★작업정보가 바뀌면 설비값이 그대로라도 다시 그린다★ (사용자 지적)
 	//   아래 m_bModified 검사 때문에 설비 데이터가 안 바뀌면 화면을 갱신하지 않는데,
 	//   번호·색은 작업정보(JOB_MST)에서도 온다. 그래서 작업이 삭제된 뒤에도
 	//   화면에는 번호가 그대로 남았다(9010 이 계속 보이던 현상).
 	//   (TrackInfo 의 입고대 2초 지연 처리가 쓰던 것과 같은 방식이다)
 	if (m_pEquipment != NULL && m_pEquipment->m_pDoc != NULL &&
 		m_dwJobVerSeen != m_pEquipment->m_pDoc->m_dwJobCacheVer)
 	{
 		m_dwJobVerSeen = m_pEquipment->m_pDoc->m_dwJobCacheVer;
 		m_pSC_DATA->m_bModified = TRUE;
 	}

 	if (m_pSC_DATA->m_bModified == FALSE)
 		return;
 
 	//DEBUGER_ASSERT_VALID(m_pControl != NULL);
 	BOOL bErase = FALSE;	// (m_pControl->m_nForkPos != m_wHorizontalPos);
 
 	// SC 행위치 표현하지 않음!
	// [LGLS] 현재위치(POS_H_RD, 원본 SUBSYSTEM_LOCATION bay) 를 rail 위치로 사상
	ApplyScTextMode(m_pSC_DATA, m_pControl, m_pControl2, m_pControl3, m_pControl4, m_pControl5);	// [LGLS 2026-08-22] 보기 모드 문자 반영

	int nScPos = CConvert::ToInt(m_pSC_DATA->V_POS_H_RD);
	if (nScPos < 0) nScPos = 0; else if (nScPos > 15) nScPos = 15;
 	if (m_pControl)						
		m_pControl->m_nForkPos = nScPos;// [LGLS] SC 현재위치(POS_H)
 	if (m_pControl2)
 		m_pControl2->m_nForkPos = nScPos;// [LGLS] SC 현재위치(POS_H)
 	if (m_pControl3)
 		m_pControl3->m_nForkPos = nScPos;// [LGLS] SC 현재위치(POS_H)
	if (m_pControl4)
		m_pControl4->m_nForkPos = nScPos;// [LGLS] SC 현재위치(POS_H)
	if (m_pControl5)
		m_pControl5->m_nForkPos = nScPos;// [LGLS] SC 현재위치(POS_H)
 
	 if (m_pControl)
 		m_pControl->m_clrFork = GetForkColor1();
 	if (m_pControl2)
 		m_pControl2->m_clrFork = GetForkColor1();
 	if (m_pControl3)
 		m_pControl3->m_clrFork = GetForkColor1();
	if (m_pControl4)
		m_pControl4->m_clrFork = GetForkColor1();
	if (m_pControl5)
		m_pControl5->m_clrFork = GetForkColor1();
 
	// [LGLS 2026-08-01] 싱글 포크 크레인 : Fork2 색으로 덮어쓰지 않는다.
	//   바로 위에서 m_clrFork = GetForkColor1()(작업구분 색)을 넣어도, 여기서 GetForkColor2()로
	//   덮어써 버려 화면에는 늘 회색만 보였다(GetForkColor2 는 ITN_LUGG_FK2 기준인데 단일 포크라 항상 '0').
	//   DciRvCtrl 은 forktype=enSingle 이면 m_clrFork2 를 그리지 않으므로 Fork1 색으로 통일한다.
	//   (RTV 는 같은 문제를 이미 Fork1 통일로 수정해 둔 상태였고, S/C 만 남아 있었다)
	if (m_pControl)
	{
		m_pControl->m_clrFork  = GetForkColor1();
		m_pControl->m_clrFork2 = GetForkColor1();
	}
	if (m_pControl2)
	{
		m_pControl2->m_clrFork  = GetForkColor1();
		m_pControl2->m_clrFork2 = GetForkColor1();
	}
	if (m_pControl3)
	{
		m_pControl3->m_clrFork  = GetForkColor1();
		m_pControl3->m_clrFork2 = GetForkColor1();
	}
	if (m_pControl4)
	{
		m_pControl4->m_clrFork  = GetForkColor1();
		m_pControl4->m_clrFork2 = GetForkColor1();
	}
	if (m_pControl5)
	{
		m_pControl5->m_clrFork  = GetForkColor1();
		m_pControl5->m_clrFork2 = GetForkColor1();
	}



 	CString strSENSOR_FK_RD = m_pSC_DATA->V_SENSOR_FK_RD;//0;
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
		//m_pControl->m_clrWing = GetPostColor();
		m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}
 	if (m_pControl2)
 	{
		m_pControl2->m_nProd = nProd;
 		m_pControl2->m_clrRail = GetRailColor();
 //		m_pControl2->m_clrWing = GetPostColor();
 		m_pControl2->m_nProd = nProd;
 		m_pControl2->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
 	}
 
 	if (m_pControl3)
 	{
		m_pControl3->m_nProd = nProd;
 		m_pControl3->m_clrRail = GetRailColor();
 //		m_pControl3->m_clrWing = GetPostColor();
 		m_pControl3->m_nProd = nProd;
 		m_pControl3->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
 	}

	if (m_pControl4)
	{
		m_pControl4->m_nProd = nProd;
		m_pControl4->m_clrRail = GetRailColor();
		//		m_pControl3->m_clrWing = GetPostColor();
		m_pControl4->m_nProd = nProd;
		m_pControl4->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}
		
	if (m_pControl5)
	{
		m_pControl5->m_nProd = nProd;
		m_pControl5->m_clrRail = GetRailColor();
		//		m_pControl3->m_clrWing = GetPostColor();
		m_pControl5->m_nProd = nProd;
		m_pControl5->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}

	m_pSC_DATA->m_bModified = FALSE;
}

void CScInfo::InvokeControl(CSC_DATA*	    pSC_DATA)
{
	if(pSC_DATA == NULL)
	{  
		return;
	};

	if (pSC_DATA->m_bModified == FALSE)
		return;

	//DEBUGER_ASSERT_VALID(m_pControl != NULL);
	BOOL bErase = FALSE;	// (m_pControl->m_nForkPos != m_wHorizontalPos);

	
	// SC 행위치 표현하지 않음!
	// [LGLS 2026-08-22] 보기 모드 문자 반영 (실제 갱신 경로 - SC_DATA 에 붙은 컨트롤)
	ApplyScTextMode(pSC_DATA, pSC_DATA->m_pControl, pSC_DATA->m_pControl2, pSC_DATA->m_pControl3, pSC_DATA->m_pControl4, pSC_DATA->m_pControl5);

	int nForkPos = (CConvert::ToInt(pSC_DATA->V_POS_H_RD) < 1) ? 0 : CConvert::ToInt(pSC_DATA->V_POS_H_RD);
	if (pSC_DATA->m_pControl)						
	{
		pSC_DATA->m_pControl->m_nForkPos =  nForkPos;
		bErase = TRUE;
	}
	if (pSC_DATA->m_pControl2)
	{
		pSC_DATA->m_pControl2->m_nForkPos = nForkPos;
		bErase = TRUE;
	}
	if (pSC_DATA->m_pControl3)
	{
		pSC_DATA->m_pControl3->m_nForkPos = nForkPos;
		bErase = TRUE;
	}
	if (pSC_DATA->m_pControl4)
	{
		pSC_DATA->m_pControl4->m_nForkPos =  nForkPos;
		bErase = TRUE;
	}
	if (pSC_DATA->m_pControl5)
	{
		pSC_DATA->m_pControl5->m_nForkPos =  nForkPos;
		bErase = TRUE;
	}

	if (pSC_DATA->m_pControl)
		pSC_DATA->m_pControl->m_clrFork = GetForkColor1(pSC_DATA);
	if (pSC_DATA->m_pControl2)
		pSC_DATA->m_pControl2->m_clrFork = GetForkColor1(pSC_DATA);
	if (pSC_DATA->m_pControl3)
		pSC_DATA->m_pControl3->m_clrFork = GetForkColor1(pSC_DATA);
	if (pSC_DATA->m_pControl4)
		pSC_DATA->m_pControl4->m_clrFork = GetForkColor1(pSC_DATA);
	if (pSC_DATA->m_pControl5)
		pSC_DATA->m_pControl5->m_clrFork = GetForkColor1(pSC_DATA);

	if (pSC_DATA->m_pControl)
	{
		pSC_DATA->m_pControl->m_clrFork = GetForkColor2(pSC_DATA);		
		pSC_DATA->m_pControl->m_clrFork2 = GetForkColor1(pSC_DATA);		
	}
	if (pSC_DATA->m_pControl2)
	{
		pSC_DATA->m_pControl2->m_clrFork = GetForkColor2(pSC_DATA);	
		pSC_DATA->m_pControl2->m_clrFork2 = GetForkColor1(pSC_DATA);	
	}

	if (pSC_DATA->m_pControl3)
	{
		pSC_DATA->m_pControl3->m_clrFork = GetForkColor2(pSC_DATA);	
		pSC_DATA->m_pControl3->m_clrFork2 = GetForkColor1(pSC_DATA);	
	}
	if (pSC_DATA->m_pControl4)
	{
		pSC_DATA->m_pControl4->m_clrFork = GetForkColor2(pSC_DATA);	
		pSC_DATA->m_pControl4->m_clrFork2 = GetForkColor1(pSC_DATA);	
	}
	if (pSC_DATA->m_pControl5)
	{
		pSC_DATA->m_pControl5->m_clrFork = GetForkColor2(pSC_DATA);	
		pSC_DATA->m_pControl5->m_clrFork2 = GetForkColor1(pSC_DATA);	
	}



	CString strSENSOR_FK_RD = pSC_DATA->V_SENSOR_FK_RD;//0;
	int nProd = 0;
	if (strSENSOR_FK_RD == _T("1"))
		nProd  = 1;
	else if (strSENSOR_FK_RD == _T("2"))
		nProd  = 1;
	else if (strSENSOR_FK_RD == _T("3"))
		nProd  = 1;

	if (pSC_DATA->m_pControl)
	{
		pSC_DATA->m_pControl->m_nProd = nProd;
		
		pSC_DATA->m_pControl->m_clrRail = GetRailColor(pSC_DATA);
		//	m_pControl->m_clrWing = GetPostColor();
		pSC_DATA->m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}
	if (pSC_DATA->m_pControl2)
	{
		pSC_DATA->m_pControl2->m_nProd = nProd;
		pSC_DATA->m_pControl2->m_clrRail = GetRailColor(pSC_DATA);
		//		m_pControl2->m_clrWing = GetPostColor();
		pSC_DATA->m_pControl2->m_nProd = nProd;
		pSC_DATA->m_pControl2->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}

	if (pSC_DATA->m_pControl3)
	{
		pSC_DATA->m_pControl3->m_nProd = nProd;
		pSC_DATA->m_pControl3->m_clrRail = GetRailColor(pSC_DATA);
		//		m_pControl3->m_clrWing = GetPostColor();
		pSC_DATA->m_pControl3->m_nProd = nProd;
		pSC_DATA->m_pControl3->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}

	if (pSC_DATA->m_pControl4)
	{
		pSC_DATA->m_pControl4->m_nProd = nProd;
		pSC_DATA->m_pControl4->m_clrRail = GetRailColor(pSC_DATA);
		//		m_pControl3->m_clrWing = GetPostColor();
		pSC_DATA->m_pControl4->m_nProd = nProd;
		pSC_DATA->m_pControl4->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}

	if (pSC_DATA->m_pControl5)
	{
		pSC_DATA->m_pControl5->m_nProd = nProd;
		pSC_DATA->m_pControl5->m_clrRail = GetRailColor(pSC_DATA);
		//		m_pControl3->m_clrWing = GetPostColor();
		pSC_DATA->m_pControl5->m_nProd = nProd;
		pSC_DATA->m_pControl5->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}

	pSC_DATA->m_bModified = FALSE;
}

CSC_DATA* CScInfo::CreateSC_DATA( CString pstrEQP_NO )
{
	CSC_DATA* pSC_DATA = new CSC_DATA(m_pEquipment->m_WH_TYP, GetStringPLC_NO(), GetStringEQP_NO(pstrEQP_NO));
	m_pSC_DATA = pSC_DATA;
	return pSC_DATA;
}

CString CScInfo::GetStringPLC_NO()
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), m_pEquipment->m_nNumber);
	return strPLC_NO;
}

CString CScInfo::GetStringPLC_NO(int pnPLC_NO)
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), pnPLC_NO);
	return strPLC_NO;
}

CString CScInfo::GetStringEQP_NO(CString pstrEQP_NO)
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%03s"), pstrEQP_NO);
	return strPLC_NO;
}