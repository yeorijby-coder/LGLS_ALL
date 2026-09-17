//operator delete
// EcsDefine.cpp: implementation of the CEcsDefine class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "EcsDefine.h"

#define MYTRACE

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEcsDefine
//
CEcsDefine::CEcsDefine(CEcsDoc* pDoc)		
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
}

CEcsDefine::~CEcsDefine()
{
}


// CEcsDefine 멤버 함수
//
CEquipment* CEcsDefine::CreateEquipment(CString& strClassName, CString strThreadNo, int nIndex)
{
	if (strClassName.IsEmpty())
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return NULL;
	}

	if (strClassName == RUNTIME_CLASS(CCv)->m_lpszClassName) return new CCv(m_pDoc, strThreadNo, nIndex);
	else if(strClassName == RUNTIME_CLASS(CSc)->m_lpszClassName || strClassName == RUNTIME_CLASS(CScPair)->m_lpszClassName)
	{
		strClassName = RUNTIME_CLASS(CScPair)->m_lpszClassName;
		return new CScPair(m_pDoc, strThreadNo, nIndex);
	}
	else if (strClassName == RUNTIME_CLASS(CRtv)->m_lpszClassName) return new CRtv(m_pDoc, strThreadNo, nIndex);
	else DEBUGER_ASSERT_VALID_LOG(FALSE, strClassName);

	return NULL;	
}

BOOL CEcsDefine::ParseXml()
{
	if(m_pDoc == NULL)
	{	DEBUGER_ASSERT_VALID(FALSE); return FALSE; };

	if (!CXmlDom::InitializeXmlDom())
	{	return FALSE;	};

	if (!CXmlDom::LoadXmlFile(m_pDoc->GetDefineXmlPATH()))
	{	return FALSE;	};

	CEquipment* pEquipment = NULL;
	CString strClassName, strCID, strValue;
	CString strThreadNo;
	CString strCollectLayout;
	int nCount = GetChildElmtCount(); //epqs enum cnt
	m_pDoc->m_pEquipments.SetSize(nCount); 
	m_pDoc->m_MapEqps.InitHashTable(nCount);

	try
	{
		for(int nIdxEquipment = 0; nIdxEquipment < nCount; nIdxEquipment++)
		{
			MoveRoot();
			MoveChild(nIdxEquipment);		
			GetElmtName(strValue);
			if(!(strValue == _T("Equipment")))
			{ DEBUGER_ASSERT_VALID(strValue == _T("Equipment"));};
			GetAttrValue(_T("class"), strClassName);
			GetAttrValue(_T("thread"), strThreadNo);

			m_pDoc->m_pEquipments[nIdxEquipment] = pEquipment = CreateEquipment(strClassName, strThreadNo, nIdxEquipment);
			m_pDoc->m_MapEqps.SetAt(pEquipment->GetEquipmentKey(), pEquipment);
			DEBUGER_ASSERT_VALID(pEquipment != NULL);
			
			switch (pEquipment->m_enKind)
			{
			case CEquipment::enCV:	
				if (!ParseCv(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseCv!"), __WFUNCTION__);
					return FALSE;
				}
				break;
			case CEquipment::enRTV:
				if (!ParseRtv(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseRtv!"), __WFUNCTION__);
					return FALSE;
				}
				break;

			case CEquipment::enSC: //무조건 페어로 생성
				if (!ParseScPair(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseScPair!"), __WFUNCTION__);
					return FALSE;
				}
				break;
				break;
				break;
			}
		}
	}
	catch (_com_error& e)
	{
		m_strErrorMessage = GetErrorMessage(e);
		DEBUGER_TRACE(m_strErrorMessage, __WFUNCTION__);
		return FALSE;
	}
	return TRUE;
}


BOOL CEcsDefine::ParseCv(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CCv)));

	CString strValue, strCID;
	CCv* pCv = (CCv*)pEquipment;
	
	CCvInfo* pInfo = pCv->m_pInfo;
	DEBUGER_ASSERT_VALID(pCv != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);
	pCv->m_WH_TYP = m_pDoc->m_WH_TYP;
	pCv->m_strInPlc = _T("");


	return ParseTracks(pCv);
}


BOOL CEcsDefine::ParseTracks(CCv* pCv)
{
	CCvInfo* pCvInfo = pCv->m_pInfo;

	CString strValue;
	CString strPLCNO;
	CString strCID;
	CString strTRACK_NO;
	CString strTemp;

//	if (MoveXPath(_T("./Tracks"), FALSE))	//Equipment/Tracks
//	{
		int nPlcCnt = GetChildElmtCount();	
		int nTrackCnt = 0;
		for(int nIdxPlc = 0; nIdxPlc < nPlcCnt; nIdxPlc++)
		{
			MoveChild(nIdxPlc);		
			nTrackCnt += GetChildElmtCount();
			MoveParent();
		}
		pCvInfo->m_MapTrackInfo.InitHashTable(nPlcCnt);

		for(int nIdxPlc = 0; nIdxPlc < nPlcCnt; nIdxPlc++)
		{
			MoveChild(nIdxPlc);		
			//GetAttrValue(_T("plcno"), strPLCNO);

			GetAttrValue(_T("FirstTrack"), strValue);
			int nFirstTr = _ttoi(strValue);

			pCv->m_nStTrNum = nFirstTr;

			GetAttrValue(_T("plcno"), strPLCNO);
			int index = _ttoi(strPLCNO);
			m_pDoc->m_nStTrNum[index-1] = pCv->m_nStTrNum;


			if(pCv->m_strInPlc == _T(""))
			{
				strTemp.Format(_T("\'%s\'"), strPLCNO);
				pCv->m_strInPlc = strTemp;
			}
			else
			{
				strTemp.Format(_T("\'%s\'"), strPLCNO);
				pCv->m_strInPlc = pCv->m_strInPlc + _T(", ") + strTemp;
			}

			pCv->m_nNumber = CConvert::ToInt(strPLCNO); //m_nNumber 사용되지 않음
			nTrackCnt = GetChildElmtCount();
			for(int nIdxTrack = 0; nIdxTrack < nTrackCnt; nIdxTrack++)
			{
				MoveChild(nIdxTrack);
				CTrackInfo* pTrackInfo = new CTrackInfo(pCv);
				pTrackInfo->m_strPLC_NO = strPLCNO;
				GetAttrValue(_T("number"), strTRACK_NO);
				pTrackInfo->m_nNumber = CConvert::ToInt(strTRACK_NO);	
				pTrackInfo->CreateCCV_DATA();
				pCvInfo->m_MapTrackInfo.SetAt(pTrackInfo->m_pCV_DATA->K_TRACK_NO, pTrackInfo);
				strCID = pTrackInfo->GetCid();
				if(pTrackInfo->m_nNumber == 16105 || 
					pTrackInfo->m_nNumber == 10047 ||
					pTrackInfo->m_nNumber == 11003 ||
					pTrackInfo->m_nNumber == 9390 ||
					pTrackInfo->m_nNumber == 14063 ||
					pTrackInfo->m_nNumber == 10390  ||
					pTrackInfo->m_nNumber == 9031 ||
					pTrackInfo->m_nNumber == 8049 ||
					pTrackInfo->m_nNumber == 8030 ||
					pTrackInfo->m_nNumber == 10301 )
				{
					int a= 0;
				}

				if(pTrackInfo->SetCtrl((CDciTrackCtrl*)m_pDoc->GetDciControl_FindAllLayout(strCID)) == FALSE)
				{
					//TRACE(strCID + _T("\r\n"));
					MoveParent();
					continue;
				}
				if (MoveXPath(_T("./Status"), FALSE))	//Equipment/Tracks/Track/Status
				{
					for (int nIdxStatus = 0; nIdxStatus < GetChildElmtCount(); ++nIdxStatus)
					{
						GetChildElmtName(nIdxStatus, strValue); // strValue : status name
						WORD wStatus = 0;
						if (strValue == _T("StoStation"))	{wStatus = CTrackInfo::enStatusStoSTReady;}
						else if (strValue == _T("RetStation"))	{wStatus = CTrackInfo::enStatusRetSTReady;}
						else if (strValue == _T("ArvStation"))	{wStatus = CTrackInfo::enStatusArvSTReady;}
						else if (strValue == _T("ScStoHS"))		{wStatus = CTrackInfo::enStatusScStoHSReady;}
						else if (strValue == _T("ScRetHS"))		{wStatus = CTrackInfo::enStatusScRetHSReady;}
						else if (strValue == _T("RtvDepartHS")) {wStatus = CTrackInfo::enStatusRtvDepartHSReady;}
						else if (strValue == _T("RtvArriveHS")) {wStatus = CTrackInfo::enStatusRtvArriveHSReady;}
						else if (strValue == _T("LgvDepartHS")) {wStatus = CTrackInfo::enStatusLgvDepartHSReady;}
						else if (strValue == _T("LgvArriveHS")) {wStatus = CTrackInfo::enStatusLgvArriveHSReady;}
						else DEBUGER_ASSERT_VALID_LOG(FALSE, strValue);

						pTrackInfo->m_mapStatusInfos.SetAt(wStatus, NULL);
						pTrackInfo->m_arrayStatus.Add(wStatus);
						if(wStatus == 0)
							int aaa =0;
					}
					MoveParent();	
				} 
				MoveParent();
				m_pDoc->m_pTrackInfos.Add(pTrackInfo);
			}
			MoveParent();
		}
//	}
	return TRUE;
}

BOOL CEcsDefine::ParseSc(CEquipment* pEquipment, int nForkType/* = 0*/)
{
	//if(nForkType != 0)
		return ParseScPair(pEquipment);
	//else
	//	return ParseScSingle(pEquipment);

}

BOOL CEcsDefine::ParseScSingle(CEquipment* pEquipment)
{

	return TRUE;
}


BOOL CEcsDefine::ParseScPair(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CScPair)));

	CString strValue;
	CString strPositionPLC_READ, strPositionWCS_VIEW;
	CString strPLC_NO, strEQP_NO;
	CString strPlcPos, strViewPos;
	int nScCnt;


	CScPair* pSc = (CScPair*)pEquipment;
	pSc->m_WH_TYP = m_pDoc->m_WH_TYP;
	CScInfo* pInfo = pSc->m_pInfo;
	DEBUGER_ASSERT_VALID(pSc != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);
	MoveXPath(_T("./Scs"), FALSE);
	nScCnt = GetChildElmtCount();
	pInfo->m_MapSC_DATA.InitHashTable(nScCnt);
	for(int nIdxSc = 0; nIdxSc < nScCnt; nIdxSc++)
	{
		MoveChild(nIdxSc);		
		GetAttrValue(_T("plcno"), strPLC_NO);
		GetAttrValue(_T("number"), strEQP_NO); 
		pSc->m_nNumber = CConvert::ToInt(strPLC_NO);//m_nNumber 사용되지 않음
		CSC_DATA* pSC_DATA = pInfo->CreateSC_DATA(strEQP_NO);
		pSc->m_pInfo->m_MapSC_DATA.SetAt(pSC_DATA->K_SC_NO, pSC_DATA);
		pSC_DATA->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid());
		pSC_DATA->m_pControl2 = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid(_T("1")));
		pSC_DATA->m_pControl3 = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid(_T("2")));
		pSC_DATA->m_pControl4 = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid(_T("3")));
		pSC_DATA->m_pControl5 = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid(_T("4")));
		MoveParent();
	}
	return TRUE;

}

BOOL CEcsDefine::ParseRtv(CEquipment* pEquipment)
{
 	DEBUGER_ASSERT_VALID(pEquipment != NULL);
 	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CRtv)));
 
 	CString strValue;
	CString strPositionPLC_READ, strPositionWCS_VIEW;
	CString strPLC_NO, strEQP_NO;
	CString strPlcPos, strViewPos;
	int nPositionCnt, nRtvCnt;


 	CRtv* pRtv = (CRtv*)pEquipment;
	pRtv->m_WH_TYP = m_pDoc->m_WH_TYP;
 	CRtvInfo* pInfo = pRtv->m_pInfo;
 	DEBUGER_ASSERT_VALID(pRtv != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);
	MoveXPath(_T("./Rtvs"), FALSE);
	nRtvCnt = GetChildElmtCount();
	pInfo->m_MapRTV_DATA.InitHashTable(nRtvCnt);
	for(int nIdxRtv = 0; nIdxRtv < nRtvCnt; nIdxRtv++)
	{
		MoveChild(nIdxRtv);		
		GetAttrValue(_T("plcno"), strPLC_NO);
		GetAttrValue(_T("number"), strEQP_NO); 
		pRtv->m_nNumber = CConvert::ToInt(strPLC_NO);//m_nNumber 사용되지 않음
		CRTV_DATA* pRTV_DATA = pInfo->CreateRTV_DATA(strEQP_NO);
		pRtv->m_pInfo->m_MapRTV_DATA.SetAt(pRTV_DATA->K_RTV_NO, pRTV_DATA);
		MoveXPath(_T("./Position"), FALSE);
		nPositionCnt = GetChildElmtCount();
		pRTV_DATA->m_MapRtvPosition.InitHashTable(nPositionCnt);
		pRTV_DATA->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pRTV_DATA->GetCid());
		for(int nIdxPosition = 0; nIdxPosition < nPositionCnt; nIdxPosition++)
		{
			MoveChild(nIdxPosition);
			GetAttrValue(_T("plc"), strPositionPLC_READ);
			GetAttrValue(_T("view"), strPositionWCS_VIEW);
			pRTV_DATA->m_MapRtvPosition.SetAt(strPositionPLC_READ, CConvert::ToInt(strPositionWCS_VIEW));
			MoveParent();
		}
		MoveParent();
		MoveParent();
		GetElmtName(strValue);
	}






 

	return TRUE;
}


