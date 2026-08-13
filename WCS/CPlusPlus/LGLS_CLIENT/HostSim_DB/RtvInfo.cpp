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

	if (pRTV_DATA->V_ERR_CODE_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	//if (pRTV_DATA->V_ERR_STA_FK2_RD != _T("0"))
	//	return pConfig->m_clrUSER_COLOR_ERROR;

	//if (pRTV_DATA->V_SENSOR_FK_RD  != _T("0") || 
	//	pRTV_DATA->V_AUTO_MODE_RD != _T("0") || 
	//	pRTV_DATA->V_ACTIVE_MODE_RD != _T("0"))
	//	return DARK_GRAY;

	if (pRTV_DATA->V_AUTO_MODE_RD != _T("1") ||
		pRTV_DATA->V_ACTIVE_MODE_RD != _T("1"))
		return DARK_GRAY;

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

	if (pRTV_DATA->V_ERR_CODE_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	//if (pRTV_DATA->V_ERR_STA_FK2_RD != _T("0"))
	//	return pConfig->m_clrUSER_COLOR_ERROR;

	//if (pRTV_DATA->V_SENSOR_FK_RD  != _T("0") || 
	//	pRTV_DATA->V_AUTO_MODE_RD != _T("0") || 
	//	pRTV_DATA->V_ACTIVE_MODE_RD != _T("0"))
	//	return DARK_GRAY;


	if (pRTV_DATA->V_AUTO_MODE_RD != _T("1") ||
		pRTV_DATA->V_ACTIVE_MODE_RD != _T("1"))

		return DARK_GRAY;

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

	if (m_pRTV_DATA->V_SUSPEND == _T("3"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND;

	if (m_pRTV_DATA->V_SUSPEND == _T("2"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RET_SUSPEND;

	if (m_pRTV_DATA->V_SUSPEND == _T("1"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_STO_SUSPEND;


	return ( m_pRTV_DATA->V_ITN_LUGG_FK1 != _T("0") || m_pRTV_DATA->V_ITN_LUGG_FK2 != _T("0")) ? BLUE : BLACK;
}

COLORREF CRtvInfo::GetRailColor(CRTV_DATA* pRTV_DATA)
{
	if(pRTV_DATA == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
	}

	if (pRTV_DATA->V_SUSPEND == _T("3"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND;

	if (pRTV_DATA->V_SUSPEND == _T("2"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RET_SUSPEND;

	if (pRTV_DATA->V_SUSPEND == _T("1"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_STO_SUSPEND;


	return ( pRTV_DATA->V_ITN_LUGG_FK1 != _T("0") || pRTV_DATA->V_ITN_LUGG_FK2 != _T("0")) ? BLUE : BLACK;
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
	int nPos = pRTV_DATA->m_MapRtvPosition[pRTV_DATA->V_POS_H_RD];
	if (pRTV_DATA->m_pControl)				
	{
		bErase = TRUE;
		pRTV_DATA->m_pControl->m_nForkPos = (nPos < 0 || nPos > 15) ? 0 : nPos;
	}


	CString strSENSOR_FK_RD = m_pRTV_DATA->V_SENSOR_FK_RD;//0;
	int nProd = 0;
	if (strSENSOR_FK_RD == _T("1"))
		nProd  = 2;
	else if (strSENSOR_FK_RD == _T("2"))
		nProd  = 1;
	else if (strSENSOR_FK_RD == _T("3"))
		nProd  = 3;

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
	CString strPLC_NO;
	strPLC_NO.Format(_T("%05s"), pstrEQP_NO);
	return strPLC_NO;
}