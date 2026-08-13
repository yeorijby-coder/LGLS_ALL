// BcrInfo.cpp: implementation of the CBcrInfo class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "BcrInfo.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBcrInfo
//
IMPLEMENT_SERIAL(CBcrInfo, CInfo, 0)

CBcrInfo::CBcrInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
	m_pControl = NULL;
	m_pBCR_MST = NULL;
	m_bModified = TRUE;
}

CBcrInfo::~CBcrInfo()
{
	if (m_pBCR_MST) {delete m_pBCR_MST;};
}



void CBcrInfo::Serialize(CArchive& ar)
{
	
}

COLORREF CBcrInfo::GetColor()
{
// 	if (m_nErrorCode == enErrorCommunication)
// 		return pConfig->m_clrDisconnect;
//  
// 	if (m_nErrorCode != enErrorNone)
// 		return pConfig->m_clrError;
// 
// 	if (m_bSuspend)
// 		return pConfig->m_clrSuspend;
// 
// 	if(m_pTrack->m_bBcrNotUsed == TRUE)
// 		return BLACK;
// 
// 	if(m_nStationKind == enBcrStoStation)
// 		return DARK_ORANGE;

	return LIGHT_GRAY;
}

void CBcrInfo::InvokeControl()
{
	if (m_bModified == FALSE)
		return;

	DEBUGER_ASSERT_VALID(m_pControl != NULL);
	m_pControl->m_clrBgColor = GetColor();
	m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
	m_bModified = FALSE;
}

CBCR_MST* CBcrInfo::CreateBCR_MST( CString pstrPLC_NO, CString pstrBCR_MC_NO)
{
	CBCR_MST* pBCR_MST = new CBCR_MST(m_pEquipment->m_WH_TYP,  GetStringBCR_MC_NO(pstrBCR_MC_NO), GetStringPLC_NO());
	m_pBCR_MST = pBCR_MST;
	return pBCR_MST;
}

void CBcrInfo::InvokeControl(CBCR_MST* pBCR_MST, CCV_DATA* pCV_DATA)
{
	if (pBCR_MST->m_bModified == FALSE)
		return;

	DEBUGER_ASSERT_VALID(pBCR_MST != NULL);
	if (pBCR_MST->K_BCR_NO == _T("22"))
	{
		int a =0;
	}
	pBCR_MST->m_pControl->m_clrBgColor = GetColor(pBCR_MST, pCV_DATA);
	pBCR_MST->m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
	pBCR_MST->m_bModified = TRUE;
	m_bModified = FALSE;
}

COLORREF CBcrInfo::GetColor(CBCR_MST* pBCR_MST, CCV_DATA* pCV_DATA)
{
	// 	if (m_nErrorCode == enErrorCommunication)
	// 		return pConfig->m_clrDisconnect;
	// 
	// 	if (m_nErrorCode != enErrorNone)
	// 		return pConfig->m_clrError;
	// 
	// 	if (m_bSuspend)
	// 		return pConfig->m_clrSuspend;
	// 
	 	if(pBCR_MST->V_SUSPEND == _T("1"))
	 		return BLACK;
		
		if (pBCR_MST->V_NOREAD_CNT > _T("0"))
			return RED;

	 	if(pCV_DATA->V_RET_READY_RD == _T("1"))
	 		return DARK_ORANGE;
	

	return LIGHT_GRAY;
}

CString CBcrInfo::GetStringPLC_NO()
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), m_pEquipment->m_nNumber);
	return strPLC_NO;
}

CString CBcrInfo::GetStringPLC_NO(int pnPLC_NO)
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), pnPLC_NO);
	return strPLC_NO;
}

CString CBcrInfo::GetStringBCR_MC_NO(CString pstrEQP_NO)
{
	CString strEQP_NO;
	strEQP_NO.Format(_T("%05s"), pstrEQP_NO);
	return strEQP_NO;
}