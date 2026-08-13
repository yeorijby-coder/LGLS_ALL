// TrackInfo.cpp: implementation of the CTrackInfo class.
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "TrackInfo.h"
#include "Equipment.h"

#include "MainFrm.h"
#include "EcsView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTrackInfo
//
CTrackInfo::CTrackInfo(CEquipment* pEquipment)
{
	m_pTrackCtrl = NULL;
	m_pCV_DATA = NULL;
	m_pEquipment = pEquipment;
}

CTrackInfo::~CTrackInfo()
{
	
}

CString CTrackInfo::GetStringPLC_NO()
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), m_pEquipment->m_nNumber);
	return strPLC_NO;
}

CString CTrackInfo::GetStringTRACK_NO()
{
	CString strTRACK_NO;
	strTRACK_NO.Format(_T("%03d"), m_nNumber);
	//strTRACK_NO.Format(_T("%03d"), m_nNumber);
	return strTRACK_NO;
}

CString CTrackInfo::GetCid()
{
	CString strCID;
	strCID.Format(_T("17110%03s"), m_pCV_DATA->K_TRACK_NO);
	//strCID.Format(_T("17110%02s%03d"), m_strPLC_NO, m_pCV_DATA->K_TRACK_NO);
	return strCID;
}

CCV_DATA* CTrackInfo::CreateCCV_DATA()
{
	CCV_DATA* pCV_DATA = new CCV_DATA(m_pEquipment->m_WH_TYP,GetStringPLC_NO(), GetStringTRACK_NO());
	m_pCV_DATA = pCV_DATA;
	return pCV_DATA;
}

COLORREF CTrackInfo::GetCvColor()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_pCV_DATA->V_ERROR_CODE != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;


	//104번 BYPASS 활성화 시 색상
	if (m_pEquipment->m_pDoc->m_strBypassChkYn == _T("Y"))
	{
		if (m_pCV_DATA->V_MC_NO == _T("104"))
			return pConfig->m_clrUSER_COLOR_WC_PASS;
	}
		
	// [LGLS] 화물(작업) 없는 트랙은 중립 회색(11/12처럼). 잔여 JOB_TYP/DEST_POS 색 무시, 화물 있을 때만 작업색.
	if (m_pCV_DATA->V_SENSOR0_DATA_RD != _T("1"))
		return LIGHT_GRAY;

	int nJobTypTmp = CConvert::ToInt(m_pCV_DATA->V_JOB_TYP_RD);
	switch (nJobTypTmp)
	{
	//case 1:return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoSto			: 
		if (m_pCV_DATA->V_TRAY_LEV_RD == _T("1"))
		{
			return pConfig->m_clrUSER_COLOR_STO;
		}
			return LIGHT_MINT;

	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoA2A			: return pConfig->m_clrUSER_COLOR_ATA;	
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_RET;
 	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
 	}

	if (m_pCV_DATA->V_TRAY_LEV_RD == _T("1"))
		return SKY_BLUE;
	if (m_pCV_DATA->V_TRAY_LEV_RD == _T("2"))
		return LIGHT_BLUE;

	if (m_pCV_DATA->V_AUTO_MODE_RD == _T("0"))
 		return DARK_GRAY;
	
	//화물D
 	if (m_pCV_DATA->V_JOB_TYP_RD == _T("0") && m_pCV_DATA->V_SENSOR0_DATA_RD == _T("1"))
 		return WHEAT;
 
 	if (m_pCV_DATA->V_DEST_POS_RD != _T("000"))
 		return MINT;

	

	return LIGHT_GRAY;
}


WORD CTrackInfo::GetMapStatusKey(int nIndex) 
{
	WORD wStatusKey = 0;
	CObject* pStatusValue = NULL;
	int i = 0;

	for (POSITION pos=m_mapStatusInfos.GetStartPosition(); pos!=NULL; ++i)
	{
		m_mapStatusInfos.GetNextAssoc(pos, wStatusKey, pStatusValue);
		//		DEBUGER_ASSERT_VALID(pStatusObject != NULL);

		if (i == nIndex)
			return wStatusKey;
	}

	return 0;
}

CObject* CTrackInfo::GetMapStatusValue(WORD wStatus)
{
	CObject* pStatusValue = NULL;
	if (m_mapStatusInfos.Lookup(wStatus, pStatusValue) == FALSE)
		return NULL;

	return pStatusValue;
}
//모니터링 cv 색상표시
void CTrackInfo::InvokeControl(CDciTrackCtrl*	pTrackCtrl)
{
	if (m_pCV_DATA->m_bModified == FALSE)
		return;

	m_pCV_DATA->m_bModified = FALSE;

	if (pTrackCtrl == NULL) 
	{
		return; 
	}

	// [LGLS] 상태(Status)수와 레이아웃 아이템수 불일치(라인트랙=상태0개)여도 화물/배경/재그리기는 실행. 상태 색상 루프만 건너뜀.
	BOOL bStatusMatch = ((pTrackCtrl->GetItemSize() == m_arrayStatus.GetSize()) &&
		(pTrackCtrl->GetItemSize() == m_mapStatusInfos.GetCount()));

	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);

	static CString strTemp = _T("");
	CString strTime;
	strTime.Format(_T("읽기 - Control 표시전[%02d:%02d:%02d.%03d]\n"),cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
	strTemp = _T("");
	strTemp += strTime;

 	COLORREF clrStatusNone = 0x606060;
 	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
 	DEBUGER_ASSERT_VALID(pConfig != NULL);
 
 	pTrackCtrl->m_clrBgColor = GetCvColor();
 	pTrackCtrl->m_bExist = (m_pCV_DATA->V_SENSOR0_DATA_RD == _T("1")) ? TRUE : FALSE; 
	// [LGLS] 실화이 없으면(sensor0=0 또는 lugg 0/0000) 작업색을 표시하지 않음
	BOOL bHasData = (m_pCV_DATA->V_SENSOR0_DATA_RD == _T("1") && m_pCV_DATA->V_LUGG_NO_RD != _T("0") && m_pCV_DATA->V_LUGG_NO_RD != _T("0000"));
 	//pTrackCtrl->m_bDoubleExist = IsWriten2Level();
 
	int nTrackItemCnt = pTrackCtrl->GetItemSize();
	if (bStatusMatch)   // [LGLS] 상태 아이템 있을 때만(라인트랙은 m_arrayStatus 비어 OOB 방지)
	for (int i=0; i<nTrackItemCnt; ++i)
	{
		WORD wStatus = m_arrayStatus[i];
		DEBUGER_ASSERT_VALID(wStatus != 0);

		int nTrackNum = CConvert::ToInt(pTrackCtrl->m_strText);

		CTrackInfo* pTrack = m_pEquipment->m_pDoc->GetTrackInfo(nTrackNum);

		//if (pTrack == NULL)
		//	DEBUGER_ASSERT_VALID(FALSE);

		switch (wStatus)
		{
		case enStatusStoSTReady:
			//m_pCV_DATA->V_STO_READY_RD == "1" ? pTrackCtrl->m_items[i].m_clrItem = pConfig->m_clrUSER_COLOR_STO : pTrackCtrl->m_items[i].m_clrItem = clrStatusNone;
			pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : (m_pCV_DATA->V_SENSOR0_DATA_RD == "1" ? RGB(255,120,200) : clrStatusNone);  // [LGLS] 입고대: 화물감지 시 핑크
			break;

		//case enStatusRetSTReady:
		//	//m_pCV_DATA->V_RET_READY_RD == "1" ? pTrackCtrl->m_items[i].m_clrItem = pConfig->m_clrUSER_COLOR_RET : pTrackCtrl->m_items[i].m_clrItem = clrStatusNone;
		//	pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : (m_pCV_DATA->V_RET_READY_RD == "1" ? pConfig->m_clrUSER_COLOR_RET : clrStatusNone);	
		//	break;

		case enStatusRetSTReady:
		case enStatusCvCheckReady:
		case enStatusArvSTReady:
			//m_pCV_DATA->V_RET_READY_RD == "1" ? pTrackCtrl->m_items[i].m_clrItem = pConfig->m_clrUSER_COLOR_RET : pTrackCtrl->m_items[i].m_clrItem = clrStatusNone;
			pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : (m_pCV_DATA->V_SENSOR0_DATA_RD == "1" && m_pCV_DATA->V_LUGG_NO_RD != "0" && m_pCV_DATA->V_LUGG_NO_RD != "0000" && (CConvert::ToInt(m_pCV_DATA->V_DEST_POS_RD) == nTrackNum || m_pCV_DATA->V_JOB_TYP_RD == "2") ? RGB(120,200,255) : clrStatusNone);  // [LGLS] 출고대: 화물+데이터+목적지자신 시 하늘색	

			//if (m_pCV_DATA->V_STO_READY_RD == "1" && m_pCV_DATA->V_RET_READY_RD == "0")
			//	pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : (bHasData && m_pCV_DATA->V_STO_READY_RD == "1" ? pConfig->m_clrUSER_COLOR_STN_STO : clrStatusNone);
			//else
			//	pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : (bHasData && m_pCV_DATA->V_RET_READY_RD == "1" ? pConfig->m_clrUSER_COLOR_STN_RET : clrStatusNone);	
			break;

		case enStatusScStoHSReady:
		//case enStatusRtvDepartHSReady:
		//case enStatusLgvDepartHSReady:
		//case enStatusLfDepartHSReady:
			{
				pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : (bHasData && m_pCV_DATA->V_STOHS_READY_RD == "1" ? pConfig->m_clrUSER_COLOR_HS_STO : clrStatusNone);
				//if(m_pCV_DATA->V_STOHS_READY_RD == "1")
				//{
				//	pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : pConfig->m_clrUSER_COLOR_HS_STO;
				//}
				//else
				//{
				//	pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : clrStatusNone;
				//}
			}
			break;

		case enStatusScRetHSReady:
		//case enStatusRtvArriveHSReady:
		//case enStatusLgvArriveHSReady:
		//case enStatusLfArriveHSReady:
			{
				pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : (bHasData && m_pCV_DATA->V_RETHS_READY_RD == "1" ? pConfig->m_clrUSER_COLOR_HS_RET: clrStatusNone);
				//if (m_pCV_DATA->V_RETHS_READY_RD == "1")
				//{
				//	pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : pConfig->m_clrUSER_COLOR_HS_RET;
				//}
				//else
				//{
				//	pTrackCtrl->m_items[i].m_clrItem = m_pCV_DATA->V_TR_PAUSE_RD == "1" ? pConfig->m_clrUSER_COLOR_ALL_SUSPEND : clrStatusNone;
				//}
			}
			break;

		default:
			DEBUGER_ASSERT_VALID(FALSE);
		}
	}

	//if (m_pCV_DATA->V_MES_ERROR_CD != _T("0")) {pTrackCtrl->m_clrBgColor = pConfig->m_clrMES_ERROR_CD;};

 	//2018.07.10 화면이 변해야 되는 경우만 화면에 뿌려줌. 화면감지, 입,출고대... 2단감지 등 // 강용규
 	//if(m_bRefreshCheck == TRUE)
 		
 	pTrackCtrl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);

	//SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);

	//static CString strTemp = _T("");
//	CString strTime;
	strTime.Format(_T("읽기 - Contorl 표시후[%02d:%02d:%02d.%03d]"),cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);

	strTemp += strTime;

	if (m_pEquipment->m_pDoc->m_bDebugMode/* && m_pEquipment->m_pDoc->m_bDebugStart*/)
	{
		AfxMessageBox(strTemp);
		strTemp = _T("");
		m_pEquipment->m_pDoc->m_bDebugStart = FALSE;
	}
 
 	//TRACE(_T("InvokeControl : %d\n"), this->m_nNumber);

}

bool CTrackInfo::SetCtrl( CDciTrackCtrl* pCtrl )
{
	if(pCtrl == NULL)
		return FALSE;

	m_pTrackCtrl = pCtrl;
	m_pCV_DATA->m_pTrackCtrl = pCtrl;

	return TRUE;
}

CTrackInfoArray::~CTrackInfoArray()
{
	//if (m_pCV_DATA)		delete m_pCV_DATA;

	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = GetAt(i);
		DEBUGER_ASSERT_VALID(pTrack != NULL);

		if (pTrack->m_pCV_DATA)
			delete pTrack->m_pCV_DATA;
	}
}

CTrackInfo* CTrackInfoArray::GetTrackInfo(int nNumber)
{
	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = GetAt(i);
		DEBUGER_ASSERT_VALID(pTrack != NULL);

		if (pTrack->m_nNumber == nNumber)
			return pTrack;
	}

	return NULL;
}

void CTrackInfoArray::InvokeControl(BOOL bManual)
{
 	//CTrackInfo* pTrack = NULL;
 	//for (int i=0; i<m_nSize; ++i)
 	//{
 	//	pTrack = GetAt(i);
 	//	DEBUGER_ASSERT_VALID(pTrack != NULL);
 
 	//	if (bManual)
 	//		pTrack->m_pCV_DATA->m_bModified = TRUE;

 	//	pTrack->InvokeControl(pTrack->m_pTrackCtrl);
 	//}
}

void CTrackInfoArray::Initialize()
{
	CTrackInfo* pTrack = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrack = GetAt(i);
		DEBUGER_ASSERT_VALID(pTrack != NULL);
	}
}


void CTrackInfo::SetSeachColor(CDciTrackCtrl* pTrackCtrl, BOOL pBasic)
{
	if(pBasic == TRUE)
	{	
	 	pTrackCtrl->m_clrBgColor = GetCvColor();
 		pTrackCtrl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
		return; 
	}

	if (pTrackCtrl == NULL) 
	{
		return; 
	}

 	COLORREF clrStatusNone = 0x606060;
 	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
 	DEBUGER_ASSERT_VALID(pConfig != NULL);
 
	pTrackCtrl->m_clrBgColor = pConfig->m_clrUSER_COLOR_CV_SEARCH;

 	pTrackCtrl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
 
 	//TRACE(_T("InvokeControl : %d\n"), this->m_nNumber);

}