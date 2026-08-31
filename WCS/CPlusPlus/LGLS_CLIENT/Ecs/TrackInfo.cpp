// TrackInfo.cpp: implementation of the CTrackInfo class.
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "TrackInfo.h"
#include "Equipment.h"
#include "RecordSetWrap.h"

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
	m_dwStoOnTick = 0;   // [LGLS]
	m_bLastStoOn = FALSE;
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

// [LGLS 2026-07-19] 입고대 ON 규칙: 데이터 없이 화물이 감지된 지 2초 경과 시 ON
BOOL CTrackInfo::IsStoReadyOn()
{
	BOOL bNoData = (m_pCV_DATA->V_LUGG_NO_RD == _T("0") || m_pCV_DATA->V_LUGG_NO_RD == _T("0000") || m_pCV_DATA->V_LUGG_NO_RD.IsEmpty());
	if (m_pCV_DATA->V_SENSOR0_DATA_RD == _T("1") && bNoData)
	{
		if (m_dwStoOnTick == 0) m_dwStoOnTick = ::GetTickCount();
		return ((::GetTickCount() - m_dwStoOnTick) >= 2000);
	}
	m_dwStoOnTick = 0;
	return FALSE;
}

// [LGLS 2026-07-19] 리본 '창고 모니터링' 표시 모드 반영.
//   1=작업번호(LUGG_NO), 2=제품정보(JOB_MST.BCR_BOTTOM, 작업번호별 1회 조회 캐시), 0=트랙번호(기본, 확장 텍스트 해제)
void CTrackInfo::ApplyTrackTextMode(CDciTrackCtrl* pTrackCtrl)
{
	int nMode = m_pEquipment->m_pDoc->m_nTrackTextMode;
	// [LGLS 2026-08-24] 작업번호는 **화물이 실제로 있는 트랙**에만 표시한다.
	//   CvThread 는 CV 지시를 낼 때 도착지 트랙에 트래킹(작업번호)을 미리 기록한다(도착 예약).
	//   그 값만 보고 표시하면, 화물이 아직 통로에 있는데도 출고대에 번호가 먼저 찍힌다
	//   (아직 완료되지 않은 작업이 완료 자리에 보이는 것으로 오인된다).
	BOOL bLuggVal = (m_pCV_DATA->V_LUGG_NO_RD != _T("0") && m_pCV_DATA->V_LUGG_NO_RD != _T("0000") && !m_pCV_DATA->V_LUGG_NO_RD.IsEmpty());
	// [LGLS 2026-08-31] 색과 같은 기준으로 맞춘다 - 작업정보에 살아 있는 작업번호면 센서와 무관하게 보인다.
	//   (색만 나오고 번호는 안 보이면 어느 작업인지 알 수 없다)
	BOOL bJobAlive = (bLuggVal && m_pEquipment->m_pDoc->IsJobInJobMst(m_pCV_DATA->V_LUGG_NO_RD));
	BOOL bHasJob  = (bLuggVal && (m_pCV_DATA->V_SENSOR0_DATA_RD == _T("1") || bJobAlive));

	// [LGLS 2026-08-22] 컨베이어 위 작업번호 글자색:
	//   작업정보(JOB_MST)에 실재하는 작업이면 검정, 실물만 남고 작업이 없는 잔재면 흰색.
	//   (2초 캐시 조회 - CEcsDoc::IsJobInJobMst)
	COLORREF clrJob = RGB(0, 0, 0);
	if (bHasJob && !bJobAlive)
		clrJob = RGB(255, 255, 255);

	if (nMode == 1)
	{
		// [LGLS 2026-07-22] 수집 스레드가 갱신 중인 CString 을 참조 공유 없이 깊은 복사(레이스 방어)
		pTrackCtrl->SetExtraTextSafe(bHasJob ? CString((LPCTSTR)m_pCV_DATA->V_LUGG_NO_RD) : CString(_T(" ")), clrJob);
	}
	else if (nMode == 2)
	{
		CString strVal = _T(" ");
		if (bHasJob)
		{
			if (m_strProdLugg != m_pCV_DATA->V_LUGG_NO_RD)
			{
				m_strProdLugg = CString((LPCTSTR)m_pCV_DATA->V_LUGG_NO_RD);   // [LGLS 2026-07-22] 깊은 복사(레이스 방어)
				m_strProdVal = _T("");
				CString strSql;
				strSql.Format(_T(" SELECT TOP 1 ISNULL(BCR_BOTTOM, ' ') AS BCR FROM JOB_MST WHERE LUGG_NO = '%s' ORDER BY INS_DT DESC "), m_strProdLugg);
				int nRowCnt = 0;
				CString strMsg = _T("");
				_RecordsetPtr pRs = m_pEquipment->m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMsg);
				if (nRowCnt > 0)
				{
					CRecordSetWrap* pRsw = new CRecordSetWrap(pRs);
					pRsw->MoveFirst();
					m_strProdVal = pRsw->GetItem(_T("BCR"));
					delete pRsw;
				}
			}
			m_strProdVal.TrimRight();
			strVal = m_strProdVal.IsEmpty() ? _T(" ") : m_strProdVal;
		}
		else
			m_strProdLugg = _T("");
		pTrackCtrl->SetExtraTextSafe(strVal, RGB(0,0,0));
	}
	else
	{
		// 트랙번호 모드: 레이아웃 토글 상태와 무관하게 트랙번호(끝 2자리)를 명시 표시
		CString strTr = m_pCV_DATA->K_TRACK_NO;
		if (strTr.GetLength() > 2) strTr = strTr.Right(2);
		pTrackCtrl->SetExtraTextSafe(strTr, RGB(0,0,0));
	}
}
// [LGLS 2026-07-19] [ETC] ViewRetCnt=1 : SC별 출고 잔여 작업 수를 레이아웃의 전용 CDciStaticCtrl(17199901~05)에 표시.
//   (사용자 피드백: 트랙 번호를 바꿔치기하지 말고 CV#X 라벨과 같은 별도 static으로 표시)
//   0=검정, 1 이상=파랑. 2초 주기 캐시로 JOB_MST 집계. 호출 트리거는 출고 라인 트랙(101/105/109/113/117)의 InvokeControl.
static DWORD s_dwRetCntTick = 0;
static int s_nRetCnt[5] = {0,0,0,0,0};
BOOL CTrackInfo::ApplyRetCntDisplay(CDciTrackCtrl* pTrackCtrl)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	int nTrack = CConvert::ToInt(m_pCV_DATA->V_MC_NO);
	int idx = (nTrack == 101) ? 0 : (nTrack == 105) ? 1 : (nTrack == 109) ? 2 : (nTrack == 113) ? 3 : (nTrack == 117) ? 4 : -1;
	if (idx < 0) return FALSE;

	static CDciControl* s_pRetCntStatic[5] = {NULL, NULL, NULL, NULL, NULL};
	if (s_pRetCntStatic[idx] == NULL)
	{
		CString strCID;
		strCID.Format(_T("171999%02d"), idx + 1);
		s_pRetCntStatic[idx] = m_pEquipment->m_pDoc->GetDciControl(strCID);
	}
	CDciControl* pStatic = s_pRetCntStatic[idx];
	if (pStatic == NULL) return FALSE;

	CString strNew = _T("");
	COLORREF clrNew = RGB(0,0,0);
	if (pConfig != NULL && pConfig->m_nETC_ViewRetCnt == 1)
	{
		DWORD dwNow = ::GetTickCount();
		if (s_dwRetCntTick == 0 || (dwNow - s_dwRetCntTick) >= 2000)
		{
			s_dwRetCntTick = dwNow;
			for (int k = 0; k < 5; k++) s_nRetCnt[k] = 0;
			CString strSql = _T("");
			strSql += _T(" SELECT START_POS, COUNT(*) AS CNT FROM JOB_MST \n");
			strSql += _T("  WHERE JOB_TYP IN ('2','12') AND START_POS IN ('901','902','903','904','905') \n");
			strSql += _T("    AND JOB_STATUS BETWEEN '20' AND '29' AND (DEL_YN IS NULL OR DEL_YN <> 'Y') \n");   // [LGLS 2026-07-22] SC 처리 구간(20~29)의 출고만 카운트
			strSql += _T("  GROUP BY START_POS ");
			int nRowCnt = 0;
			CString strMsg = _T("");
			_RecordsetPtr pRs = m_pEquipment->m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMsg);
			if (nRowCnt > 0)
			{
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRs);
				pRsw->MoveFirst();
				for (int r = 0; r < nRowCnt; r++)
				{
					int sc = CConvert::ToInt(pRsw->GetItem(_T("START_POS"))) - 901;
					if (sc >= 0 && sc < 5) s_nRetCnt[sc] = CConvert::ToInt(pRsw->GetItem(_T("CNT")));
					pRsw->MoveNext();
				}
				delete pRsw;
			}
		}
		strNew.Format(_T("%d개"), s_nRetCnt[idx]);   // [LGLS] "N개" 형식
		clrNew = (s_nRetCnt[idx] > 0) ? RGB(0,0,255) : RGB(0,0,0);
	}

	if (pStatic->m_strText != strNew || pStatic->m_clrFgColor != clrNew)
	{
		pStatic->m_strText = strNew;
		pStatic->m_clrFgColor = clrNew;
		pStatic->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
	}
	return FALSE;
}

// [LGLS 2026-08-30] 설비 에러코드가 "에러 없음"인지 판정.
//   표기 흔들림(0 / 00 / 0000 / 빈 값)을 모두 정상으로 본다. 숫자 0 이면 정상.
BOOL CTrackInfo::IsNoError(const CString& strErrCode)
{
	CString s = strErrCode;
	s.Trim();
	if (s.IsEmpty())
		return TRUE;
	for (int i = 0; i < s.GetLength(); ++i)
	{
		if (s[i] != _T('0'))
			return FALSE;
	}
	return TRUE;		// 전부 '0' 이면 에러 없음
}

// [LGLS 2026-08-31] 작업구분 → 색. 종전에는 GetCvColor 안에만 있었는데,
//   "트랙의 작업번호가 작업정보에 있으면 그 작업구분으로 칠한다" 규칙(사용자 지시)에서도
//   같은 표를 써야 하므로 함수로 뽑았다. 0 = 해당 없음.
static COLORREF ColorOfJobTyp(CConfig* pConfig, int nJobTyp)
{
	switch (nJobTyp)
	{
	case enJobTypeAutoSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoA2A			: return pConfig->m_clrUSER_COLOR_ATA;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_SEMI_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_SEMI_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_SEMI_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_SEMI_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_SEMI_PR;
	case enJobTypeSemiW2W			: return pConfig->m_clrUSER_COLOR_SEMI_ATA;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
	}
	return CLR_INVALID;
}

COLORREF CTrackInfo::GetCvColor()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	// [LGLS 2026-08-30] 정상(무에러) 표기가 한 가지가 아니다.
	//   설비에서 올라오는 실제 값은 CvThread 가 ToString("0000") 로 쓰는 4자리 '0000' 이고,
	//   구 경로/초기값으로 '0', '00', 빈 값, NULL 도 함께 존재한다.
	//   종전에는 "0" 하나만 정상으로 보아, 정상인 트랙이 전부 에러색(빨강)으로 그려졌다.
	if (!IsNoError(m_pCV_DATA->V_ERROR_CODE))
		return pConfig->m_clrUSER_COLOR_ERROR;


	//104번 BYPASS 활성화 시 색상
	if (m_pEquipment->m_pDoc->m_strBypassChkYn == _T("Y"))
	{
		if (m_pCV_DATA->V_MC_NO == _T("104"))
			return pConfig->m_clrUSER_COLOR_WC_PASS;
	}
		
	// [LGLS] 화물(작업) 없는 트랙은 중립 회색(11/12처럼). 잔여 JOB_TYP/DEST_POS 색 무시, 화물 있을 때만 작업색.

	// [LGLS 2026-08-31] ★트랙에 작업번호가 있고 그 번호가 작업정보에 있으면
	//   작업정보의 작업구분으로 칠한다★ (사용자 지시)
	//   설비의 JOB_TYP_RD 는 지시 구간에서만 채워지고 그 밖에는 비어 있어서,
	//   화물이 실려 있어도 색이 나오지 않던 자리가 있었다(C/V#4).
	//   "작업정보에 있는가" 가 살아있는 작업과 잔재를 가르는 기준이 된다 -
	//   잔재(작업정보에 없는 번호)는 종전대로 아래 센서 규칙을 따른다.
	{
		CString strLuggTrk = m_pCV_DATA->V_LUGG_NO_RD;
		strLuggTrk.Trim();
		if (!strLuggTrk.IsEmpty() && strLuggTrk != _T("0") && strLuggTrk != _T("0000"))
		{
			int nTypJob = CConvert::ToInt(m_pEquipment->m_pDoc->GetJobTypOfLugg(strLuggTrk));
			COLORREF clrJobTyp = ColorOfJobTyp(pConfig, nTypJob);
			if (clrJobTyp != CLR_INVALID)
				return clrJobTyp;
		}
	}

	if (m_pCV_DATA->V_SENSOR0_DATA_RD != _T("1"))
		return LIGHT_GRAY;

	int nJobTypTmp = CConvert::ToInt(m_pCV_DATA->V_JOB_TYP_RD);
	{
		COLORREF clrRd = ColorOfJobTyp(pConfig, nJobTypTmp);
		if (clrRd != CLR_INVALID) return clrRd;
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
	// [LGLS 2026-07-19] SC 출고 잔여 수(전용 static)는 트랙 데이터 변경과 무관하게 갱신 (static 자체 무효화)
	if (pTrackCtrl != NULL)
		ApplyRetCntDisplay(pTrackCtrl);

	// [LGLS 2026-07-19] 입고대 2초 지연 ON은 데이터 변경 없이 시간 경과로 상태가 바뀌므로 전이 시 강제 갱신
	BOOL bStoOnNow = IsStoReadyOn();
	if (bStoOnNow != m_bLastStoOn)
	{
		m_bLastStoOn = bStoOnNow;
		m_pCV_DATA->m_bModified = TRUE;
	}

	// [LGLS 2026-08-31] ★작업정보가 바뀌면 설비값이 그대로라도 다시 그린다★ (사용자 지적)
	//   아래 m_bModified 검사 때문에 설비 데이터가 안 바뀌면 화면을 갱신하지 않는데,
	//   번호·색은 작업정보(JOB_MST)에서도 온다. 그래서 작업이 삭제된 뒤에도
	//   화면에는 번호가 그대로 남았다(9010 이 계속 보이던 현상).
	//   (TrackInfo 의 입고대 2초 지연 처리가 쓰던 것과 같은 방식이다)
	if (m_pEquipment != NULL && m_pEquipment->m_pDoc != NULL &&
		m_dwJobVerSeen != m_pEquipment->m_pDoc->m_dwJobCacheVer)
	{
		m_dwJobVerSeen = m_pEquipment->m_pDoc->m_dwJobCacheVer;
		m_pCV_DATA->m_bModified = TRUE;
	}

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

	// [LGLS 2026-08-22] 핸드셰이크 4종은 EQP_TASK 가 트랙 역할(크레인/RGV × 싣는곳/내리는곳)까지
	//   반영해 CV_DATA 에 직접 기록한다. 컬럼이 채워져 있으면 그것을 점등 기준으로 쓰고,
	//   아직 비어 있는(구버전 DB) 경우에만 종전의 화물·데이터 유무 판정으로 되돌아간다.
	#define LGLS_HS_ON(col, fallback)  ((col) == _T("1") ? TRUE : ((col) == _T("0") ? FALSE : (fallback)))
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
			pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : (IsStoReadyOn() ? RGB(255,120,200) : clrStatusNone);  // [LGLS] 입고대: 화물감지 시 핑크
			break;

		//case enStatusRetSTReady:
		//	//m_pCV_DATA->V_RET_READY_RD == "1" ? pTrackCtrl->m_items[i].m_clrItem = pConfig->m_clrUSER_COLOR_RET : pTrackCtrl->m_items[i].m_clrItem = clrStatusNone;
		//	pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : (m_pCV_DATA->V_RET_READY_RD == "1" ? pConfig->m_clrUSER_COLOR_RET : clrStatusNone);	
		//	break;

		case enStatusRetSTReady:
		case enStatusCvCheckReady:
		case enStatusArvSTReady:
			//m_pCV_DATA->V_RET_READY_RD == "1" ? pTrackCtrl->m_items[i].m_clrItem = pConfig->m_clrUSER_COLOR_RET : pTrackCtrl->m_items[i].m_clrItem = clrStatusNone;
			pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : (m_pCV_DATA->V_SENSOR0_DATA_RD == "1" && m_pCV_DATA->V_LUGG_NO_RD != "0" && m_pCV_DATA->V_LUGG_NO_RD != "0000" ? RGB(120,200,255) : clrStatusNone);  // [LGLS] 출고대: 화물+데이터+목적지자신 시 하늘색	

			//if (m_pCV_DATA->V_STO_READY_RD == "1" && m_pCV_DATA->V_RET_READY_RD == "0")
			//	pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : (bHasData && m_pCV_DATA->V_STO_READY_RD == "1" ? pConfig->m_clrUSER_COLOR_STN_STO : clrStatusNone);
			//else
			//	pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : (bHasData && m_pCV_DATA->V_RET_READY_RD == "1" ? pConfig->m_clrUSER_COLOR_STN_RET : clrStatusNone);	
			break;

		case enStatusScStoHSReady:
		//case enStatusRtvDepartHSReady:
		//case enStatusLgvDepartHSReady:
		//case enStatusLfDepartHSReady:
			{
				// [LGLS 2026-08-04] 입고 H/S = 설비에 화물을 "싣는 곳"(CV->설비) : 화물·데이터가 있으면 점등
				pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : (LGLS_HS_ON(m_pCV_DATA->V_STOHS_READY_RD, bHasData) ? pConfig->m_clrUSER_COLOR_HS_STO : clrStatusNone);
				//if(m_pCV_DATA->V_STOHS_READY_RD == "1")
				//{
				//	pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : pConfig->m_clrUSER_COLOR_HS_STO;
				//}
				//else
				//{
				//	pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : clrStatusNone;
				//}
			}
			break;

		case enStatusScRetHSReady:
		//case enStatusRtvArriveHSReady:
		//case enStatusLgvArriveHSReady:
		//case enStatusLfArriveHSReady:
			{
				// [LGLS 2026-08-04] 출고 H/S = 설비가 화물을 "내려놓는 곳"(설비->CV) : 화물·데이터가 없으면 점등
				pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : (LGLS_HS_ON(m_pCV_DATA->V_RETHS_READY_RD, !bHasData) ? pConfig->m_clrUSER_COLOR_HS_RET: clrStatusNone);
				//if (m_pCV_DATA->V_RETHS_READY_RD == "1")
				//{
				//	pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : pConfig->m_clrUSER_COLOR_HS_RET;
				//}
				//else
				//{
				//	pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : clrStatusNone;
				//}
			}
			break;

		// [LGLS 2026-08-04] RTV 인수인계 지점도 입고/출고 H/S 신호로 표시한다.
		//   RtvArriveHS = RTV 가 화물을 '내려놓는' 입고 드롭 트랙 -> 화물/데이터가 없어야 받을 수 있다
		//     색은 '내려놓는 곳' 공통(크레인 출고 하역과 동일한 HS_RET)
		case enStatusRtvArriveHSReady:
			pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : (LGLS_HS_ON(m_pCV_DATA->V_RTV_ARRIVEHS_READY_RD, !bHasData) ? pConfig->m_clrUSER_COLOR_HS_RET : clrStatusNone);
			break;

		//   RtvDepartHS = RTV 가 화물을 '싣는' 출고 픽업 트랙 -> 화물/데이터가 있어야 실어갈 수 있다
		//     색은 '싣는 곳' 공통(크레인 입고 픽업과 동일한 HS_STO)
		case enStatusRtvDepartHSReady:
			pTrackCtrl->m_items[i].m_clrItem = (m_pCV_DATA->V_TR_PAUSE_RD == "1" || m_pCV_DATA->V_TR_PAUSE_OD == "1") ? pConfig->m_clrUSER_COLOR_SUSPEND : (LGLS_HS_ON(m_pCV_DATA->V_RTV_DEPARTHS_READY_RD, bHasData) ? pConfig->m_clrUSER_COLOR_HS_STO : clrStatusNone);
			break;

		default:
			DEBUGER_ASSERT_VALID(FALSE);
		}
	}

	//if (m_pCV_DATA->V_MES_ERROR_CD != _T("0")) {pTrackCtrl->m_clrBgColor = pConfig->m_clrMES_ERROR_CD;};

 	//2018.07.10 화면이 변해야 되는 경우만 화면에 뿌려줌. 화면감지, 입,출고대... 2단감지 등 // 강용규
 	//if(m_bRefreshCheck == TRUE)
 		
 	// [LGLS 2026-07-19] 상태 정의가 없는 설비 작업대(화살표) 트랙 일시정지 표시:
 	//   일시정지 시 화살표 아이템을 범례 일시정지 색으로, 해제 시 원래 색으로 복원
 	if (m_arrayStatus.GetSize() == 0 && nTrackItemCnt > 0)
 	{
 		BOOL bPausedWs = (m_pCV_DATA->V_TR_PAUSE_RD == _T("1") || m_pCV_DATA->V_TR_PAUSE_OD == _T("1"));
 		if (m_arrOrigItemColors.GetSize() != nTrackItemCnt)
 		{
 			m_arrOrigItemColors.SetSize(nTrackItemCnt);
 			for (int k = 0; k < nTrackItemCnt; ++k) m_arrOrigItemColors[k] = pTrackCtrl->m_items[k].m_clrItem;
 		}
 		for (int k = 0; k < nTrackItemCnt; ++k)
 			pTrackCtrl->m_items[k].m_clrItem = bPausedWs ? pConfig->m_clrUSER_COLOR_SUSPEND : (COLORREF)m_arrOrigItemColors[k];
 	}

 	// [LGLS 2026-07-19] 리본 표시 모드(작업번호/제품정보) 반영
 	ApplyTrackTextMode(pTrackCtrl);
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