// ScInfo.cpp: implementation of the CScInfo class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "ScInfo.h"

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

 	if (m_pSC_DATA->V_ERR_CODE_RD != _T("0000"))
 		return pConfig->m_clrUSER_COLOR_ERROR;
 
 	if (m_pSC_DATA->V_ERR_STA_FK1_RD != _T("0"))
 		return pConfig->m_clrUSER_COLOR_ERROR;
 
 	if (m_pSC_DATA->V_ONLINE_MODE_RD != _T("0") || m_pSC_DATA->V_AUTO_MODE_RD != _T("0") || m_pSC_DATA->V_ACTIVE_MODE_RD != _T("0"))
 		return DARK_GRAY;
 
 	if (m_pSC_DATA->V_ITN_LUGG_FK1 == _T("0") || m_pSC_DATA->V_ITN_LUGG_FK1 == _T("0000"))
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
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
 	}

	return BLACK;
}

COLORREF CScInfo::GetForkColor1(CSC_DATA* pSC_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pSC_DATA->V_ERR_CODE_RD != _T("0000"))
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
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoA2A			: return pConfig->m_clrUSER_COLOR_ATA;
	case enJobTypeAutoPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_RET;
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

COLORREF CScInfo::GetForkColor2()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_pSC_DATA->V_ERR_CODE_RD != _T("0000"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (m_pSC_DATA->V_ERR_STA_FK2_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (m_pSC_DATA->V_ONLINE_MODE_RD != _T("0") || m_pSC_DATA->V_AUTO_MODE_RD != _T("0") || m_pSC_DATA->V_ACTIVE_MODE_RD != _T("0"))
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
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
	}

	return BLACK;
}

COLORREF CScInfo::GetForkColor2(CSC_DATA* pSC_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pSC_DATA->V_ERR_CODE_RD != _T("0000"))
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
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_RET;
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

	if (m_pSC_DATA->V_SUSPEND == _T("3"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND;

	if (m_pSC_DATA->V_SUSPEND == _T("2"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RET_SUSPEND;

	if (m_pSC_DATA->V_SUSPEND == _T("1"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_STO_SUSPEND;

 
 	return ( m_pSC_DATA->V_ITN_LUGG_FK1 != _T("0") || m_pSC_DATA->V_ITN_LUGG_FK1 != _T("0000") || m_pSC_DATA->V_ITN_LUGG_FK2 != _T("0") || m_pSC_DATA->V_ITN_LUGG_FK2 != _T("0000")) ? BLUE : BLACK;
	//RAIL 색상
}

COLORREF CScInfo::GetRailColor(CSC_DATA* pSC_DATA)
{
	if(pSC_DATA == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
	}

	if (pSC_DATA->V_SUSPEND == _T("3"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND;

	if (pSC_DATA->V_SUSPEND == _T("2"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RET_SUSPEND;

	if (pSC_DATA->V_SUSPEND == _T("1"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_STO_SUSPEND;

	if (pSC_DATA->V_ERR_CODE_RD != _T("0000"))
	{
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RAIL_ERROR;
	}


	return ( pSC_DATA->V_ITN_LUGG_FK1 != _T("0") || pSC_DATA->V_ITN_LUGG_FK2 != _T("0")) ? m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_SC_INVK : BLACK;
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

void CScInfo::InvokeControl()
{
	if(m_pSC_DATA == NULL)
	{  
		return;
	};

 	if (m_pSC_DATA->m_bModified == FALSE)
 		return;
 
 	//DEBUGER_ASSERT_VALID(m_pControl != NULL);
 	BOOL bErase = FALSE;	// (m_pControl->m_nForkPos != m_wHorizontalPos);
 
 	// SC 행위치 표현하지 않음!
 	if (m_pControl)						
		m_pControl->m_nForkPos = 1;//(m_wHorizontalPos >= 254) ? 1 : m_wHorizontalPos;
 	if (m_pControl2)
 		m_pControl2->m_nForkPos = 1;//(m_wHorizontalPos >= 254) ? 1 : m_wHorizontalPos;
 	if (m_pControl3)
 		m_pControl3->m_nForkPos = 1;//(m_wHorizontalPos >= 254) ? 1 : m_wHorizontalPos;
	if (m_pControl4)
		m_pControl4->m_nForkPos = 1;//(m_wHorizontalPos >= 254) ? 1 : m_wHorizontalPos;
	if (m_pControl5)
		m_pControl5->m_nForkPos = 1;//(m_wHorizontalPos >= 254) ? 1 : m_wHorizontalPos;
 
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
 
	if (m_pControl)
	{
 		m_pControl->m_clrFork = GetForkColor2();		
 		m_pControl->m_clrFork2 = GetForkColor1();		
	}
 	if (m_pControl2)
 	{
 		m_pControl2->m_clrFork = GetForkColor2();	
 		m_pControl2->m_clrFork2 = GetForkColor1();	
 	}
 
 	if (m_pControl3)
 	{
 		m_pControl3->m_clrFork = GetForkColor2();	
 		m_pControl3->m_clrFork2 = GetForkColor1();	
 	}
	if (m_pControl4)
	{
		m_pControl4->m_clrFork = GetForkColor2();	
		m_pControl4->m_clrFork2 = GetForkColor1();	
	}
	if (m_pControl5)
	{
		m_pControl5->m_clrFork = GetForkColor2();	
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