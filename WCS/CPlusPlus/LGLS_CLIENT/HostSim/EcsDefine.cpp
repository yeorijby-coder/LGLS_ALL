// EcsDefine.cpp: implementation of the CEcsDefine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "EcsDefine.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MYTRACE

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEcsDefine::CEcsDefine(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
}

CEcsDefine::~CEcsDefine()
{
		
}

CEquipment* CEcsDefine::CreateEquipment(CString& strClassName, int nIndex, int nNumber, LPCTSTR lpszDevice)
{
	if (strClassName.IsEmpty())
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return NULL;
	}

		 if (strClassName == RUNTIME_CLASS(CCv)->m_lpszClassName)			return new CCv(m_pDoc, nIndex, nNumber, lpszDevice);
//	else if (strClassName == RUNTIME_CLASS(CSc)->m_lpszClassName)			return new CSc(m_pDoc, nIndex, nNumber, lpszDevice);
//	else if (strClassName == RUNTIME_CLASS(CScPair)->m_lpszClassName)		return new CScPair(m_pDoc, nIndex, nNumber, lpszDevice);
//	else if (strClassName == RUNTIME_CLASS(CScHalfPair)->m_lpszClassName)	return new CScHalfPair(m_pDoc, nIndex, nNumber, lpszDevice);
	else if (strClassName == RUNTIME_CLASS(CCvFx)->m_lpszClassName)			return new CCvFx(m_pDoc, nIndex, nNumber, lpszDevice);			//
	else if (strClassName == RUNTIME_CLASS(CScDongJin)->m_lpszClassName)	return new CScDongJin(m_pDoc, nIndex, nNumber, lpszDevice);
//	else if (strClassName == RUNTIME_CLASS(CRtv)->m_lpszClassName)			return new CRtv(m_pDoc, nIndex, nNumber, lpszDevice);
//	else if (strClassName == RUNTIME_CLASS(CLifter)->m_lpszClassName)		return new CLifter(m_pDoc, nIndex, nNumber, lpszDevice);
	else if (strClassName == RUNTIME_CLASS(CLgv)->m_lpszClassName)			return new CLgv(m_pDoc, nIndex, nNumber, lpszDevice);
//	else if (strClassName == RUNTIME_CLASS(CRfid)->m_lpszClassName)			return new CRfid(m_pDoc, nIndex, nNumber, lpszDevice);
//	else if (strClassName == RUNTIME_CLASS(CBcr)->m_lpszClassName)			return new CBcr(m_pDoc, nIndex, nNumber, lpszDevice);
//	else if (strClassName == RUNTIME_CLASS(CDisplay)->m_lpszClassName)		return new CDisplay(m_pDoc, nIndex, nNumber, lpszDevice);
	else 
		DEBUGER_ASSERT_VALID_LOG(FALSE, strClassName);

	return NULL;	
}

BOOL CEcsDefine::ParseXml()
{
	if (!CXmlDom::InitializeXmlDom())
		return FALSE;

	if (!CXmlDom::LoadXmlFile(ECS_DEFINE_FILE))
		return FALSE;

	CEquipment* pEquipment = NULL;
	CString strClassName, strDevice, strCID, strValue, strForkType;
	int i, nCount = GetChildElmtCount();
	m_pDoc->m_pEquipments.SetSize(nCount);

	try
	{
		for (i=0; i<nCount; ++i)
		{
			MoveRoot();
			MoveChild(i);

			GetElmtName(strValue);
			DEBUGER_ASSERT_VALID(strValue == _T("Equipment"));

			GetAttrValue(_T("class"), strClassName);
			GetAttrValue(_T("number"), strValue);
			GetAttrValue(_T("device"), strDevice);
			int nDeviceNumber = _ttoi(strValue);

			m_pDoc->m_pEquipments[i] = pEquipment = CreateEquipment(strClassName, i, nDeviceNumber, strDevice);

			if (pEquipment == NULL)
				continue;

			DEBUGER_ASSERT_VALID(pEquipment != NULL);


			switch (pEquipment->m_enKind)
			{
			case CEquipment::enCV:	
				if (nDeviceNumber == 12)
				{
					if (!ParseCvFx(pEquipment))
					{
						DEBUGER_TRACE(_T("Fail to ParseCvFx!"), _T("CEcsDefine::ParseXml"));
						return FALSE;
					}				
				
				}
				else
				{
					if (!ParseCv(pEquipment))
					{
						DEBUGER_TRACE(_T("Fail to ParseCv!"), _T("CEcsDefine::ParseXml"));
						return FALSE;
					}				
				}
				break;

			case CEquipment::enSC:
//				GetAttrValue(4, strForkType);			// 인덱스로 가져옴! XML 파일에서 반드시 순서를 지켜야함!			// JBY 0829
				if (!ParseSc(pEquipment, _ttoi(strForkType)))
				{
					DEBUGER_TRACE(_T("Fail to ParseSc!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;

			case CEquipment::enRGV:
				if (!ParseRgv(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseRgv!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;
/*
			case CEquipment::enRTV:
				if (!ParseRtvZaion(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseRtv!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;

			case CEquipment::enLIFTER:
				if (!ParseLifter(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseLifter!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;

			case CEquipment::enRFID:
				if (!ParseRfid(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseRfid!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;

			case CEquipment::enBCR:
				if (!ParseBcr(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseBcr!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;

			case CEquipment::enDISPLAY:
				if (!ParseDisplay(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseDisplay!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;
*/
			default:
				DEBUGER_TRACE(_T("등록되지 않은 설비구분!"), _T("CEcsDefine::ParseXml"));
				return FALSE;
			}
		}

		if (!ParseStation())
		{
			DEBUGER_TRACE(_T("Fail to ParseStation!"), _T("CEcsDefine::ParseXml"));
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		m_strErrorMessage = GetErrorMessage(e);
		DEBUGER_TRACE(m_strErrorMessage, _T("CEcsDefine::ParseXml"));
		return FALSE;
	}

	return TRUE;
}

BOOL CEcsDefine::ParseTracks(CEquipment* pEquipment, CTrackInfoArray& pTracks)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);

	if (MoveXPath(_T("./Tracks"), FALSE))	//Equipment/Tracks
	{
		CString strValue;
		CString strCID, strTrackCID, strPID;
		GetAttrValue(_T("cid"), strCID);
		DEBUGER_ASSERT_VALID_LOG(CLib::IsValidControlID(strCID), strCID);

		CTrackInfo* pTrack = NULL;
		int i, j, nCount = GetChildElmtCount();
		pTracks.SetSize(nCount);

		for (i=0; i<nCount; ++i)
		{
			MoveChild(i);	//Equipment/Tracks/Track

			pTracks[i] = pTrack = new CTrackInfo(pEquipment);
			DEBUGER_ASSERT_VALID(pTrack != NULL);

			GetAttrValue(_T("number"), strValue);
			pTrack->m_nNumber = _ttoi(strValue);

			GetAttrValue(_T("virtual"), strValue);
			pTrack->m_bVirtual = _ttoi(strValue);

			strTrackCID.Format(_T("%s%02d"), strCID.Left(7), pTrack->m_nNumber % 100);
			pTrack->m_pTrackCtrl = (CDciTrackCtrl*)m_pDoc->GetDciControl(strTrackCID);
			DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strTrackCID));
//			DEBUGER_ASSERT_VALID(pTrack->m_pTrackCtrl != NULL);





			strPID.Format(_T("%s%02d%02d%02d"), strCID.Left(3), 37, int(pTrack->m_nNumber / 1000), 1);
			CDciStaticCtrl* pPrdInfoStaticCtrl1 = (CDciStaticCtrl*)m_pDoc->GetDciControl(strPID);

			strPID.Format(_T("%s%02d%02d%02d"), strCID.Left(3), 37, int(pTrack->m_nNumber / 1000), 2);
			CDciStaticCtrl* pPrdInfoStaticCtrl2 = (CDciStaticCtrl*)m_pDoc->GetDciControl(strPID);


//			if (pPrdInfoStaticCtrl1 != NULL || pPrdInfoStaticCtrl2 != NULL)
//			{
//				m_pDoc->m_pStoDataChk1.Add(pPrdInfoStaticCtrl1);
//				m_pDoc->m_pStoDataChk2.Add(pPrdInfoStaticCtrl2);
//			}


			if (MoveXPath(_T("./Status"), FALSE))	//Equipment/Tracks/Track/Status
			{
				for (j=0; j<GetChildElmtCount(); ++j)
				{
					GetChildElmtName(j, strValue);

					WORD wStatus = 0;
					if (strValue == _T("StoStation")) wStatus = CTrackInfo::enStatusStoSTReady;
					else if (strValue == _T("RetStation")) wStatus = CTrackInfo::enStatusRetSTReady;
					else if (strValue == _T("ArvStation")) wStatus = CTrackInfo::enStatusArvSTReady;
					else if (strValue == _T("ScStoHS")) wStatus = CTrackInfo::enStatusScStoHSReady;
					else if (strValue == _T("ScRetHS")) wStatus = CTrackInfo::enStatusScRetHSReady;
					else if (strValue == _T("RtvDepartHS")) wStatus = CTrackInfo::enStatusRtvDepartHSReady;
					else if (strValue == _T("RtvArriveHS")) wStatus = CTrackInfo::enStatusRtvArriveHSReady;
//					else if (strValue == _T("LgvDepartHS")) wStatus = CTrackInfo::enStatusLgvDepartHSReady;
//					else if (strValue == _T("LgvArriveHS")) wStatus = CTrackInfo::enStatusLgvArriveHSReady;
//					else if (strValue == _T("LfDepartHS")) wStatus = CTrackInfo::enStatusLfDepartHSReady;
//					else if (strValue == _T("LfArriveHS")) wStatus = CTrackInfo::enStatusLfArriveHSReady;
//					else if (strValue == _T("CvCheckPos")) wStatus = CTrackInfo::enStatusCvCheckReady;
					else DEBUGER_ASSERT_VALID_LOG(FALSE, strValue);

					if (pTrack->m_nNumber==7)
						int a = 0;

					pTrack->m_mapStatusInfos.SetAt(wStatus, NULL);
					pTrack->m_arrayStatus.Add(wStatus);
				}

				MoveParent();	//Equipment/Tracks/Track
			}

			if (MoveXPath(_T("./Destination"), FALSE))	//Equipment/Tracks/Track/Destination
			{
				GetAttrValue(_T("tid"), strValue);
				pTrack->m_nDestination = _ttoi(strValue);

				MoveParent();
			}


			MoveParent();	//Equipment/Tracks
		}

		MoveParent();	//Equipment
	}

	if (pTracks.GetSize())
		m_pDoc->m_pTrackInfos.Append(pTracks);

	return TRUE;
}

BOOL CEcsDefine::ParseCv(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CCv)));

	CString strValue, strCID, strVirtual;
	CCv* pCv = (CCv*)pEquipment;
	CCvInfo* pInfo = pCv->m_pInfo;
	DEBUGER_ASSERT_VALID(pCv != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID_LOG(strValue == pEquipment->GetDeviceName(), strValue);

//	GetAttrValue(_T("virtual"), strVirtual);
//	pEquipment->m_bVirTrack = _ttoi(strVirtual);

	if (MoveXPath(_T("./Doors"), FALSE))	//Equipment/Doors
	{
		int nCount = GetChildElmtCount();
		pInfo->m_pDoorCtrls.SetSize(nCount);

		for (int i=0; i<nCount; ++i)
		{
			MoveChild(i);	//Equipment/Doors/Door
			GetAttrValue(_T("cid"), strCID);
			pInfo->m_pDoorCtrls[i] = (CDciStaticCtrl*)m_pDoc->GetDciControl(strCID);
			DEBUGER_ASSERT_VALID_LOG(CLib::IsValidControlID(strCID), strCID);
			DEBUGER_ASSERT_VALID(pInfo->m_pDoorCtrls[i] != NULL);
			MoveParent();	//Equipment/Doors
		}

		MoveParent();	//Equipment
	}

/*
	if (MoveXPath(_T("./Lamps"), FALSE))	//Equipment/Lamps
	{
		int nCount = GetChildElmtCount();
		pInfo->m_pLampCtrls.SetSize(nCount);

		for (int i=0; i<nCount; ++i)
		{
			MoveChild(i);	//Equipment/Lamps/Lamp
			GetAttrValue(_T("cid"), strCID);
			pInfo->m_pLampCtrls[i] = (CDciStaticCtrl*)m_pDoc->GetDciControl(strCID);
			DEBUGER_ASSERT_VALID_LOG(CLib::IsValidControlID(strCID), strCID);
			DEBUGER_ASSERT_VALID(pInfo->m_pLampCtrls[i] != NULL);
			MoveParent();	//Equipment/Lamps
		}

		MoveParent();	//Equipment
	}
*/
	return ParseTracks(pEquipment, pInfo->m_pTracks);
}

BOOL CEcsDefine::ParseCvFx(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CCvFx)));

	CString strValue, strCID, strVirtual;
	CCvFx* pCv = (CCvFx*)pEquipment;
	CCvInfo* pInfo = pCv->m_pInfo;
	DEBUGER_ASSERT_VALID(pCv != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID_LOG(strValue == pEquipment->GetDeviceName(), strValue);

	if (MoveXPath(_T("./Lamps"), FALSE))	//Equipment/Lamps
	{
		int nCount = GetChildElmtCount();
		pInfo->m_pLampCtrls.SetSize(nCount);

		for (int i=0; i<nCount; ++i)
		{
			MoveChild(i);	//Equipment/Lamps/Lamp
			GetAttrValue(_T("cid"), strCID);
			pInfo->m_pLampCtrls[i] = (CDciStaticCtrl*)m_pDoc->GetDciControl(strCID);
			DEBUGER_ASSERT_VALID_LOG(CLib::IsValidControlID(strCID), strCID);
			DEBUGER_ASSERT_VALID(pInfo->m_pLampCtrls[i] != NULL);
			MoveParent();	//Equipment/Lamps
		}

		MoveParent();	//Equipment
	}

	return TRUE;
}

BOOL CEcsDefine::ParseSc(CEquipment* pEquipment, int nForkType)
{
//	if(nForkType != 0)
//		return ParseScPair(pEquipment);
//	else
		return ParseScDongJinSingle(pEquipment);

}
/*

// 일반 SFA Single Crane
BOOL CEcsDefine::ParseScSingle(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CSc)));
	MYTRACE(_T("\n\n CEcsDefine::ParseScSingle(%s)"), pEquipment->GetDeviceName());

	CString strValue, strCID, strFloor;
 
	CSc*	 pSc = (CSc*)pEquipment;
	CScInfo* pInfo = pSc->m_pInfo;
	DEBUGER_ASSERT_VALID(pSc != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("class"), strValue);						// JBY 0829
	GetAttrValue(_T("number"), strValue);				// JBY 0829
	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("cid"), strCID);

	pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);

	pEquipment->m_nForkType = CDciRvCtrl::enSingle;

	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);			// @@
	MYTRACE(_T("\n\t <%s cid='%s'>"), pEquipment->GetDeviceName(), pInfo->m_pControl->m_strCID);

	if (MoveXPath(_T("./StoHS"), FALSE))	//Equipment/StoHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();
		pInfo->m_pStosHS.SetArraySize(nCount);

		for (i=0; i<nCount; ++i)
		{
			pInfo->m_pStosHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/StoHS/HS

				GetAttrValue(_T("pos"), strValue);
				pTrackHS->m_nPos = _ttoi(strValue);

				GetAttrValue(_T("tid"), strValue);
				pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

				j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
				pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScStoHSReady, pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScStoHSReady] == pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

				MYTRACE(_T("\n\t\t <StoHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

			MoveParent();	//Equipment/StoHS
		}

		MoveParent();	//Equipment
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}

	if (MoveXPath(_T("./RetHS"), FALSE))	//Equipment/RetHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();
		pInfo->m_pRetsHS.SetArraySize(nCount);

		for (i=0; i<nCount; ++i)
		{
			pInfo->m_pRetsHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/RetHS/HS

				GetAttrValue(_T("pos"), strValue);
				pTrackHS->m_nPos = _ttoi(strValue);

				GetAttrValue(_T("tid"), strValue);
				pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

				GetAttrValue(_T("deadlock"), strValue);
				int nDeadlock = _ttoi(strValue);
				pTrackHS->m_bDeadLockTrack = nDeadlock;

				j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
				pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScRetHSReady, pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScRetHSReady] == pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

				MYTRACE(_T("\n\t\t <RetHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

				CString strFrom, strTo;
				int nDestStationSize = GetChildElmtCount();
				for (j=0; j<nDestStationSize; ++j)
				{
					MoveChild(j);	//Equipment/RetHS/HS/DestST

						GetAttrValue(_T("from"), strFrom);
						GetAttrValue(_T("to"), strTo);
						ParseDestStation(_ttoi(strFrom), _ttoi(strTo), pTrackHS->m_strDestStations);
						pTrackHS->AddDestStationID(strFrom, strTo);

					MoveParent();	//Equipment/RetHS/HS
				}

			MoveParent();	//Equipment/RetHS
		}

		MoveParent();	//Equipment
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}

	MYTRACE(_T("\n\t </%s> \n"), pEquipment->GetDeviceName());

	return TRUE;

}

BOOL CEcsDefine::ParseScPair(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CSc)));
	MYTRACE(_T("\n\n CEcsDefine::ParseScPair(%s)"), pEquipment->GetDeviceName());

	CString strValue, strCID, strFloor, strForkType, strCycleType;
 
	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("cid"), strCID);

	GetAttrValue(_T("cycletype"), strCycleType);

	CScInfo* pInfo = NULL;

	if (_ttoi(strCycleType) == CScInfo::enFullCycle)
	{
		CScPair* pSc = (CScPair*)pEquipment;
		pInfo = pSc->m_pInfo;
		pInfo->m_nCycleType = CScInfo::enFullCycle;
		DEBUGER_ASSERT_VALID(pSc != NULL);		//CScInfo
		DEBUGER_ASSERT_VALID(pInfo != NULL);
	}
	else //if (_ttoi(strCycleType) == CScInfo::enHalfCycle)
	{
		CScHalfPair* pSc = (CScHalfPair*)pEquipment;
		pInfo = pSc->m_pInfo;
		pInfo->m_nCycleType = CScInfo::enHalfCycle;
		DEBUGER_ASSERT_VALID(pSc != NULL);		//CScInfo
		DEBUGER_ASSERT_VALID(pInfo != NULL);
	}

	pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);

	GetAttrValue(_T("forktype"), strForkType);

	if (_ttoi(strForkType) == CDciRvCtrl::enTwin || 
		_ttoi(strForkType) == CDciRvCtrl::enDouble )
	{
		pInfo->m_nForkType = _ttoi(strForkType);
//		pEquipment->m_nForkType = _ttoi(strForkType);
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;	
	}

	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);			// @@
	MYTRACE(_T("\n\t <%s cid='%s'>"), pEquipment->GetDeviceName(), pInfo->m_pControl->m_strCID);

	if (MoveXPath(_T("./StoHS"), FALSE))	//Equipment/StoHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();
		pInfo->m_pStosHS.SetArraySize(nCount);

		for (i=0; i<nCount; ++i)
		{
			pInfo->m_pStosHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/StoHS/HS

				GetAttrValue(_T("pos"), strValue);
				pTrackHS->m_nPos = _ttoi(strValue);

				GetAttrValue(_T("tid"), strValue);
				pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

				GetAttrValue(_T("pair"), strValue);
				pTrackHS->m_nPair = _ttoi(strValue);

				j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
				pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScStoHSReady, pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScStoHSReady] == pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

				MYTRACE(_T("\n\t\t <StoHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

			MoveParent();	//Equipment/StoHS
		}

		MoveParent();	//Equipment
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}

	if (MoveXPath(_T("./RetHS"), FALSE))	//Equipment/RetHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();
		pInfo->m_pRetsHS.SetArraySize(nCount);

		for (i=0; i<nCount; ++i)
		{
			pInfo->m_pRetsHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/RetHS/HS

				GetAttrValue(_T("pos"), strValue);
				pTrackHS->m_nPos = _ttoi(strValue);

				GetAttrValue(_T("tid"), strValue);
				pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

				GetAttrValue(_T("pair"), strValue);
				pTrackHS->m_nPair = _ttoi(strValue);

				GetAttrValue(_T("deadlock"), strValue);
				int nDeadlock = _ttoi(strValue);
				pTrackHS->m_bDeadLockTrack = nDeadlock;

				j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
				pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScRetHSReady, pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScRetHSReady] == pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

				MYTRACE(_T("\n\t\t <RetHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

				CString strFrom, strTo;
				int nDestStationSize = GetChildElmtCount();
				for (j=0; j<nDestStationSize; ++j)
				{
					MoveChild(j);	//Equipment/RetHS/HS/DestST

						GetAttrValue(_T("from"), strFrom);
						GetAttrValue(_T("to"), strTo);
						ParseDestStation(_ttoi(strFrom), _ttoi(strTo), pTrackHS->m_strDestStations);
						pTrackHS->AddDestStationID(strFrom, strTo);

					MoveParent();	//Equipment/RetHS/HS
				}

			MoveParent();	//Equipment/RetHS
		}

		MoveParent();	//Equipment
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}
	
	MYTRACE(_T("\n\t </%s> \n"), pEquipment->GetDeviceName());

	return TRUE;

}
BOOL CEcsDefine::ParseLifter(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CLifter)));
	MYTRACE(_T("\n\n CEcsDefine::ParseLifter(%s)"), pEquipment->GetDeviceName());

	CString strValue, strCID;
	CLifter* pLifter = (CLifter*)pEquipment;
	CLifterInfo* pInfo = pLifter->m_pInfo;
	DEBUGER_ASSERT_VALID(pLifter != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("cid"), strCID);
	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));

	if (ParseTracks(pEquipment, pInfo->m_pTracks) == FALSE)
		return FALSE;

	if (MoveXPath(_T("./Floors"), FALSE))	// Equipment/Floors
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();

		for (i=0; i<nCount; ++i)
		{
			MoveChild(i);	//Equipment/Floors/Floor

				GetAttrValue(_T("fid"), strValue);
				DEBUGER_ASSERT_RANGE(_ttoi(strValue), CLifterInfo::enFloorSize);

				GetAttrValue(_T("cid"), strCID);	
				pInfo->m_pControl[_ttoi(strValue)] = (CDciTrackCtrl*)m_pDoc->GetDciControl(strCID);
				DEBUGER_ASSERT_VALID(pInfo->m_pControl[_ttoi(strValue)] != NULL);

				MYTRACE(_T("\n\t <Floor fid='%s' tid='%s'/>"), strValue, strCID);

				if (MoveXPath(_T("./DepartHS"), FALSE))		//Equipment/Floors/Floor/DepartHS
				{
					pTrackHS = new CTrackHS(pEquipment);
					pInfo->m_pDepartsHS.Add(pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS != NULL);

					GetAttrValue(_T("pos"), strValue);
					pTrackHS->m_nPos = _ttoi(strValue);

					GetAttrValue(_T("tid"), strValue);
					pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

					j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
					pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusLfDepartHSReady, pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusLfDepartHSReady] == pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

					MYTRACE(_T("\n\t\t <DepartHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

					MoveParent();	//Equipment/Floors/Floor
				}
				else
				{
					DEBUGER_ASSERT_VALID(FALSE);
					return FALSE;
				}

				if (MoveXPath(_T("./ArriveHS"), FALSE))		//Equipment/Floors/Floor/ArriveHS
				{
					pTrackHS = new CTrackHS(pEquipment);
//					pInfo->m_pArrivesHS.Add(pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS != NULL);

					GetAttrValue(_T("pos"), strValue);
					pTrackHS->m_nPos = _ttoi(strValue);

					GetAttrValue(_T("tid"), strValue);
					pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

					j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
					pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusLfArriveHSReady, pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusLfArriveHSReady] == pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

					MYTRACE(_T("\n\t\t <ArriveHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

					CString strFrom, strTo;
					int nDestStationSize = GetChildElmtCount();
					for (j=0; j<nDestStationSize; ++j)
					{
						MoveChild(j);	//Equipment/Floors/Floor/ArriveHS/DestST

							GetAttrValue(_T("from"), strFrom);
							GetAttrValue(_T("to"), strTo);
							ParseDestStation(_ttoi(strFrom), _ttoi(strTo), pTrackHS->m_strDestStations);
//							pTrackHS->AddDestStationID(strFrom, strTo);

						MoveParent();	//Equipment/Floors/Floor/ArriveHS
					}

					MoveParent();	//Equipment/Floors/Floor
				}
				else
				{
					DEBUGER_ASSERT_VALID(FALSE);
					return FALSE;
				}

			MYTRACE(_T("\n\t </Floor>"));

			MoveParent();	//Equipment/Floors
		}

		MoveParent();	//Equipment
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}

	return TRUE;
}

BOOL CEcsDefine::ParseLgv(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CLgv)));
	MYTRACE(_T("\n\n CEcsDefine::ParseLgv(%s)"), pEquipment->GetDeviceName());

	CString strValue, strCID;
	CLgv* pLgv = (CLgv*)pEquipment;
	CLgvInfo* pInfo = pLgv->m_pInfo;
	DEBUGER_ASSERT_VALID(pLgv != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("cid"), strCID);
	pInfo->m_pControl = (CDciTrackCtrl*)m_pDoc->GetDciControl(strCID);
	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);
	MYTRACE(_T("\n\t <%s cid='%s'>"), pEquipment->GetDeviceName(), pInfo->m_pControl->m_strCID);

	if (MoveXPath(_T("./Vehicles"), FALSE))
	{
		CLgvVehicle* pLgvVehicle = NULL;
		int i, nCount = GetChildElmtCount();
		pInfo->m_oLgvVehicles.SetSize(nCount);

		for (i=0; i<nCount; ++i)
		{
			MoveChild(i);
			GetAttrValue(_T("vid"), strValue);

			pInfo->m_oLgvVehicles[i] = pLgvVehicle = new CLgvVehicle(_ttoi(strValue));
			DEBUGER_ASSERT_VALID(pLgvVehicle != NULL);
			MYTRACE(_T("\n\t\t <AGV vid='%s'/>"), strValue);

			MoveParent();
		}

		MoveParent();
	}

	if (MoveXPath(_T("./HomeStand"), FALSE))
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();
		pInfo->m_pTrackHsArray.SetArraySize(nCount);
		
		for (i=0; i<nCount; ++i)
		{
			pInfo->m_pTrackHsArray[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment[@class='CLgv']/HomeStand/HS

				GetAttrValue(_T("pos"), strValue);
				pTrackHS->m_nPos = _ttoi(strValue);

				if(pTrackHS->m_nPos == 101)
					int a=0;

				GetAttrValue(_T("tid"), strValue);
				pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
				DEBUGER_ASSERT_VALID_LOG(pTrackHS->m_pTrack != NULL, strValue);
				MYTRACE(_T("\n\t\t <HS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

				if (MoveXPath(_T("./Name"), FALSE))
				{
					GetElmtValue(pTrackHS->m_strName);
					MYTRACE(_T("\n\t\t\t <Name=%s/>"), pTrackHS->m_strName);
					MoveParent();
				}

				if (MoveXPath(_T("./DepartHS"), FALSE))
				{
					pInfo->m_pDepartsHS.Add(pTrackHS);

					j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
					pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusLgvDepartHSReady, pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusLgvDepartHSReady] == pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);
					MYTRACE(_T("\n\t\t\t <DepartHS/>"));

					MoveParent();
				}

				if (MoveXPath(_T("./ArriveHS"), FALSE))
				{
//					pInfo->m_pArrivesHS.Add(pTrackHS);

					j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
					pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusLgvArriveHSReady, pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusLgvArriveHSReady] == pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);
					MYTRACE(_T("\n\t\t\t <ArriveHS/>"));

					CString strFrom, strTo;
					int nDestStationSize = GetChildElmtCount();
					for (j=0; j<nDestStationSize; ++j)
					{
						MoveChild(j);
						
							GetAttrValue(_T("from"), strFrom);
							GetAttrValue(_T("to"), strTo);
							ParseDestStation(_ttoi(strFrom), _ttoi(strTo), pTrackHS->m_strDestStations);
//							pTrackHS->AddDestStationID(strFrom, strTo);
						
						MoveParent();
					}

					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

				if (MoveXPath(_T("./BidpBufferStoST"), FALSE))
				{
					pInfo->m_pBufferSto.Add(pTrackHS);
					pTrackHS->m_nKind |= CLgvInfo::enBidpBufferStoST;
					MYTRACE(_T("\n\t\t\t <BidpBufferStoST/>"));
					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

				if (MoveXPath(_T("./BidpEmptyRetST"), FALSE))
				{
					pInfo->m_pBufferRet.Add(pTrackHS);
					pTrackHS->m_nKind |= CLgvInfo::enBidpEmptyRetST;
					MYTRACE(_T("\n\t\t\t <BidpEmptyRetST/>"));
					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

				if (MoveXPath(_T("./BidpExistRetST"), FALSE))
				{
					pInfo->m_pBufferRet.Add(pTrackHS);
					pTrackHS->m_nKind |= CLgvInfo::enBidpExistRetST;
					MYTRACE(_T("\n\t\t\t <BidpExistRetST/>"));
					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

				if (MoveXPath(_T("./BidpEmptyArriveST"), FALSE))
				{
					pInfo->m_pSemiStoRet.Add(pTrackHS);
					pTrackHS->m_nKind |= CLgvInfo::enBidpEmptyArriveST;
					MYTRACE(_T("\n\t\t\t <BidpEmptyArriveST/>"));
					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

				if (MoveXPath(_T("./BidpExistArriveST"), FALSE))
				{
					pInfo->m_pSemiStoRet.Add(pTrackHS);
					pTrackHS->m_nKind |= CLgvInfo::enBidpExistArriveST;
					MYTRACE(_T("\n\t\t\t <BidpExistArriveST/>"));
					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

				if (MoveXPath(_T("./IdpStoST"), FALSE))
				{
					pTrackHS->m_nKind |= CLgvInfo::enIdpStoST;
					MYTRACE(_T("\n\t\t\t <IdpStoST/>"));
					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

				if (MoveXPath(_T("./IdpRetST"), FALSE))
				{
					pTrackHS->m_nKind |= CLgvInfo::enIdpRetST;
					MYTRACE(_T("\n\t\t\t <IdpRetST/>"));
					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

				if (MoveXPath(_T("./IdpDepartST"), FALSE))
				{
					pTrackHS->m_nKind |= CLgvInfo::enIdpDepartST;
					MYTRACE(_T("\n\t\t\t <IdpDepartST/>"));
					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

				if (MoveXPath(_T("./IdpArriveST"), FALSE))
				{
					pTrackHS->m_nKind |= CLgvInfo::enIdpArriveST;
					MYTRACE(_T("\n\t\t\t <IdpArriveST/>"));
					MoveParent();	//Equipment[@class='CLgv']/HomeStand/HS
				}

			MoveParent();	//Equipment[@class='CLgv']/HomeStand
		}

		if (pTrackHS)
		{
			pInfo->m_pCvInfo = (CCvInfo*)pTrackHS->m_pTrack->m_pEquipment->GetInfo();
			DEBUGER_ASSERT_VALID_LOG(pTrackHS->m_pTrack->m_pEquipment != NULL, pTrackHS->ToString());
			DEBUGER_ASSERT_VALID_LOG(pTrackHS->m_pTrack->m_pEquipment->IsKindOf(RUNTIME_CLASS(CCv)), pTrackHS->ToString());
			DEBUGER_ASSERT_VALID_LOG(pInfo->m_pCvInfo != NULL, pTrackHS->ToString());
		}
		else DEBUGER_ASSERT_VALID(FALSE);

		MoveParent();	//Equipment[@class='CLgv']
	}
	
	return TRUE;
}

BOOL CEcsDefine::ParseRfid(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CRfid)));

	CString strValue, strCID;
	CRfid* pRfid = (CRfid*)pEquipment;
	CRfidInfo* pInfo = pRfid->m_pInfo;
	DEBUGER_ASSERT_VALID(pRfid != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("cid"), strCID);
	pInfo->m_pControl = (CDciButtonCtrl*)m_pDoc->GetDciControl(strCID);
	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);

	DEBUGER_ASSERT_VALID(GetChildElmtCount() == 1);
	MoveChildFirst();
	GetAttrValue(_T("tid"), strValue);
	pInfo->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
	DEBUGER_ASSERT_VALID(pInfo->m_pTrack != NULL);

	return TRUE;
}

BOOL CEcsDefine::ParseBcr(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CBcr)));

	CString strValue, strCID;
	CBcr* pBcr = (CBcr*)pEquipment;
	CBcrInfo* pInfo = pBcr->m_pInfo;
	DEBUGER_ASSERT_VALID(pBcr != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("cid"), strCID);
	pInfo->m_pControl = (CDciButtonCtrl*)m_pDoc->GetDciControl(strCID);
	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);

	DEBUGER_ASSERT_VALID(GetChildElmtCount() == 1);
	MoveChildFirst();
	GetAttrValue(_T("tid"), strValue);
	pInfo->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
	DEBUGER_ASSERT_VALID(pInfo->m_pTrack != NULL);

	return TRUE;
}

BOOL CEcsDefine::ParseDisplay(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CDisplay)));

	CString strValue, strCID;
	CDisplay* pDisplay = (CDisplay*)pEquipment;
	CDisplayInfo* pInfo = pDisplay->m_pInfo;
	DEBUGER_ASSERT_VALID(pDisplay != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("cid"), strCID);
	pInfo->m_pControl = (CDciButtonCtrl*)m_pDoc->GetDciControl(strCID);
	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);

	CTrackInfo* pTrack = NULL;
	CDisplayBoard* pDisplayBoard = NULL;
	int i, j, nCount = GetChildElmtCount();
	pInfo->m_pDisplayBoards.SetSize(nCount);

	for (i=0; i<nCount; ++i)
	{
		MoveChild(i);	//Equipment/Board

			pInfo->m_pDisplayBoards[i] = pDisplayBoard = new CDisplayBoard(i+1);
			DEBUGER_ASSERT_VALID(pDisplayBoard != NULL);

			pDisplayBoard->m_pTracks.SetSize(GetChildElmtCount());
			for (j=0; j<pDisplayBoard->m_pTracks.GetSize(); ++j)
			{
				MoveChild(j);	//Equipment/Board/Line
					
					GetAttrValue(_T("tid"), strValue);
					pDisplayBoard->m_pTracks[j] = pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
					DEBUGER_ASSERT_VALID(pTrack != NULL);

				MoveParent();	//Equipment/Board
			}

		MoveParent();	//Equipment
	}

	return TRUE;
}

*/

BOOL CEcsDefine::ParseScDongJinSingle(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CScDongJin)));
	MYTRACE(_T("\n\n CEcsDefine::ParseScDongJinSingle(%s)"), pEquipment->GetDeviceName());

	CString strValue, strCID, strFloor;
 
	CScDongJin*	 pSc = (CScDongJin*)pEquipment;
	DEBUGER_ASSERT_VALID(pSc != NULL);

	GetAttrValue(_T("class"), strValue);						// JBY 0829
	GetAttrValue(_T("number"), strValue);				// JBY 0829
	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	int nCount = GetChildElmtCount();
	pEquipment->m_nForkType = CDciRvCtrl::enSingle;

//	static int nScCount = 0;

	for (int i = 0 ; i < nCount ; i++) 
	{
		MoveChild(i);	//Equipment/Sc
		GetAttrValue(_T("no"), strValue);	
		int nEquipNum = _ttoi(strValue);
//		CScInfo* pInfo = pSc->m_pInfo[nEquipNum];
		CScInfo* pInfo = new CScInfo(pEquipment);
		DEBUGER_ASSERT_VALID(pInfo != NULL);
		
		if (pInfo == NULL)
			continue;

		GetAttrValue(_T("cid"), strCID);
		pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);

		pInfo->m_nEquipNum = _ttoi(strCID.Right(2));

		CString strFID, strRID;//
		strFID.Format(_T("%s%04d%02d"), strCID.Left(3), 3601, pInfo->m_nEquipNum);
		CDciStaticCtrl* pScRetFrontCntCtrl = (CDciStaticCtrl*)m_pDoc->GetDciControl(strFID);

		strRID.Format(_T("%s%04d%02d"), strCID.Left(3), 3602, pInfo->m_nEquipNum);
		CDciStaticCtrl* pScRetRearCntCtrl = (CDciStaticCtrl*)m_pDoc->GetDciControl(strRID);

		m_pDoc->m_pScRetFronts.Add(pScRetFrontCntCtrl);
		m_pDoc->m_pScRetRears.Add(pScRetRearCntCtrl);

		DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
		DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);			// @@
		MYTRACE(_T("\n\t <%s cid='%s'>"), pEquipment->GetDeviceName(), pInfo->m_pControl->m_strCID);

		if (MoveXPath(_T("./StoHS"), FALSE))	//Equipment/Sc/StoHS
		{
			CTrackHS* pTrackHS = NULL;
			int i, j, nCount = GetChildElmtCount();
			pInfo->m_pStosHS.SetArraySize(nCount);

			for (i=0; i<nCount; ++i)
			{
				pInfo->m_pStosHS[i] = pTrackHS = new CTrackHS(pEquipment);
				DEBUGER_ASSERT_VALID(pTrackHS != NULL);

				MoveChild(i);	//Equipment/StoHS/HS

					GetAttrValue(_T("pos"), strValue);
					pTrackHS->m_nPos = _ttoi(strValue);

					GetAttrValue(_T("tid"), strValue);
					pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

					j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
					pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScStoHSReady, pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScStoHSReady] == pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

					MYTRACE(_T("\n\t\t <StoHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

				MoveParent();	//Equipment/StoHS
			}

			MoveParent();	//Equipment

		}
		else
		{
			DEBUGER_ASSERT_VALID(FALSE);
			return FALSE;
		}

		if (MoveXPath(_T("./RetHS"), FALSE))	//Equipment/RetHS
		{
			CTrackHS* pTrackHS = NULL;
			int i, j, nCount = GetChildElmtCount();
			pInfo->m_pRetsHS.SetArraySize(nCount);

			for (i=0; i<nCount; ++i)
			{
				pInfo->m_pRetsHS[i] = pTrackHS = new CTrackHS(pEquipment);
				DEBUGER_ASSERT_VALID(pTrackHS != NULL);

				MoveChild(i);	//Equipment/RetHS/HS

					GetAttrValue(_T("pos"), strValue);
					pTrackHS->m_nPos = _ttoi(strValue);

					GetAttrValue(_T("tid"), strValue);
					pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

					GetAttrValue(_T("deadlock"), strValue);
					int nDeadlock = _ttoi(strValue);
					pTrackHS->m_bDeadLockTrack = nDeadlock;

					pTrackHS->m_nDestStationPos = 0;

					j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
					pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScRetHSReady, pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScRetHSReady] == pTrackHS);
					DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

					MYTRACE(_T("\n\t\t <RetHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

					CString strFrom, strTo;
					int nDestStationSize = GetChildElmtCount();
					for (j=0; j<nDestStationSize; ++j)
					{
						MoveChild(j);	//Equipment/RetHS/HS/DestST

							GetAttrValue(_T("from"), strFrom);
							GetAttrValue(_T("to"), strTo);
//							ParseDestStation(_ttoi(strFrom), _ttoi(strTo), pTrackHS->m_strDestStations);
							pTrackHS->AddDestStationID(strFrom, strTo);

						MoveParent();	//Equipment/RetHS/HS
					}

				MoveParent();	//Equipment/RetHS
			}

			MoveParent();	//Equipment
		}
		else
		{
			DEBUGER_ASSERT_VALID(FALSE);
			return FALSE;
		}

		pInfo->Initialize();
		pSc->m_pInfo.Add(pInfo);

		MoveParent();	//Equipment/Sc
	}


	MYTRACE(_T("\n\t </%s> \n"), pEquipment->GetDeviceName());

	return TRUE;

}

BOOL CEcsDefine::ParseRgv(CEquipment* pEquipment)
{
	
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CLgv)));
	MYTRACE(_T("\n\n CEcsDefine::ParseRgv(%s)"), pEquipment->GetDeviceName());

	CString strValue, strCID, strName, strJID, strPID;
	CLgv* pRgv = (CLgv*)pEquipment;
	CLgvInfo* pInfo = pRgv->m_pInfo;
	DEBUGER_ASSERT_VALID(pRgv != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("cid"), strCID);
	pInfo->m_pControl = (CDciTrackCtrl*)m_pDoc->GetDciControl(strCID);
	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);
	MYTRACE(_T("\n\t <%s cid='%s'>"), pEquipment->GetDeviceName(), pInfo->m_pControl->m_strCID);

	if (MoveXPath(_T("./Cars"), FALSE))
	{
		CLgvVehicle* pRgvVehicle = NULL;
		int i, nCount = GetChildElmtCount();
		pInfo->m_oLgvVehicles.SetSize(nCount);

		for (i=0; i<nCount; ++i)
		{
			MoveChild(i);
			GetAttrValue(_T("no"), strValue);

			pRgvVehicle = new CLgvVehicle(_ttoi(strValue));
			pInfo->m_oLgvVehicles.SetAt(i, pRgvVehicle);

			DEBUGER_ASSERT_VALID(pRgvVehicle != NULL);
			MYTRACE(_T("\n\t\t <RGV vid='%s'/>"), strValue);

			GetAttrValue(_T("cid"), strCID);
			pRgvVehicle->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);
			DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
			DEBUGER_ASSERT_VALID(pRgvVehicle->m_pControl != NULL);
			MYTRACE(_T("\n\t <%s cid='%s'>"), pEquipment->GetDeviceName(), pRgvVehicle->m_pControl->m_strCID);

			GetAttrValue(_T("name"), strValue);
//			DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());
			
			strJID.Format(_T("%s%02d%02d%02d"), strCID.Left(3), 33, pRgv->m_nNumber, pRgvVehicle->m_nVehicleID);
			CDciStaticCtrl* pJobStaticCtrl = (CDciStaticCtrl*)m_pDoc->GetDciControl(strJID);

			strPID.Format(_T("%s%02d%02d%02d"), strCID.Left(3), 34, pRgv->m_nNumber, pRgvVehicle->m_nVehicleID);
			CDciStaticCtrl* pPathStaticCtrl = (CDciStaticCtrl*)m_pDoc->GetDciControl(strPID);
			
			if (pJobStaticCtrl == NULL || pPathStaticCtrl == NULL)
			{
				DEBUGER_ASSERT_VALID(FALSE);
				return FALSE;
			}

			if (pInfo->m_pEquipment->m_nNumber == 1)
			{
				m_pDoc->m_pRtvJobNums1.Add(pJobStaticCtrl);
				m_pDoc->m_pRtvSrcDsts1.Add(pPathStaticCtrl);
			}
			else
			{
				m_pDoc->m_pRtvJobNums2.Add(pJobStaticCtrl);
				m_pDoc->m_pRtvSrcDsts2.Add(pPathStaticCtrl);
			}
			MoveParent();
		}

		MoveParent();
	}


	if (MoveXPath(_T("./DepartHS"), FALSE))	//->Equipment/DepartHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nDepartHSCount = GetChildElmtCount();
//		pInfo->m_pDepartsHS.SetArraySize(nDepartHSCount);
	//	pTrackHS = new CTrackHS(pEquipment);
		for (i=0; i<nDepartHSCount; i++)
		{
			pTrackHS = new CTrackHS(pEquipment);
//			pTrackHS = (CTrackHS*) pEquipment;
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);
//			pInfo->m_pDepartsHS[i] = pTrackHS;
			pInfo->m_pDepartsHS.Add(pTrackHS);
			
			MoveChild(i);	//->Equipment/Rtv/DepartHS/HS
			
			GetAttrValue(_T("pos"), strValue);
			pTrackHS->m_nPos = _ttoi(strValue);
			
			GetAttrValue(_T("tid"), strValue);
			pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

			j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
			pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusRtvDepartHSReady, pTrackHS);
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusRtvDepartHSReady] == pTrackHS);
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);		// 임시JBY
			
			MYTRACE(_T("\n\t\t <DepartHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);
			
			MoveParent();	//->Equipment/DepartHS

			if (pTrackHS )
			{
				if ((pRgv->m_pInfo->m_pEquipment->m_nNumber == 1 && pTrackHS->m_pTrack->m_pEquipment->m_nNumber == 4) || 
					(pRgv->m_pInfo->m_pEquipment->m_nNumber == 2 && pTrackHS->m_pTrack->m_pEquipment->m_nNumber == 10) )
				{
					pInfo->m_pCvInfo = (CCvInfo*)pTrackHS->m_pTrack->m_pEquipment->GetInfo();
					DEBUGER_ASSERT_VALID_LOG(pTrackHS->m_pTrack->m_pEquipment != NULL, pTrackHS->ToString());
					DEBUGER_ASSERT_VALID_LOG(pTrackHS->m_pTrack->m_pEquipment->IsKindOf(RUNTIME_CLASS(CCv)), pTrackHS->ToString());
					DEBUGER_ASSERT_VALID_LOG(pInfo->m_pCvInfo != NULL, pTrackHS->ToString());
				}
			}
			else 
				DEBUGER_ASSERT_VALID(FALSE);

//			delete pTrackHS;
//			CTrackHS* pTrackHS = NULL;
		}
		

		MoveParent();	//->Equipment/
//		delete pTrackHS;
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}	
	
	if (MoveXPath(_T("./ArriveHS"), FALSE))	//->Equipment/ArriveHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nArriveHSCount = GetChildElmtCount();
//		pInfo->m_pArrivesHS.SetArraySize(nArriveHSCount);
	//	pTrackHS = new CTrackHS(pEquipment);
		for (i=0; i<nArriveHSCount; i++)
		{
			pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);
//			pInfo->m_pArrivesHS[i] = pTrackHS;
			pInfo->m_pArrivesHS.Add(pTrackHS);

			
			MoveChild(i);	//->Equipment/ArriveHS/HS
			
			GetAttrValue(_T("pos"), strValue);
			pTrackHS->m_nPos = _ttoi(strValue);
			
			GetAttrValue(_T("tid"), strValue);
			pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);
			
		
			j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
			pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusRtvArriveHSReady, pTrackHS);
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusRtvArriveHSReady] == pTrackHS);
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);		// 임시JBY
			
			MYTRACE(_T("\n\t\t <ArriveHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);
			
			CString strFrom, strTo;
			int nDestStationSize = GetChildElmtCount();
			for (j=0; j<nDestStationSize; ++j)
			{
				MoveChild(j);	//->Equipment/ArriveHS/HS/DestST
				
				GetAttrValue(_T("from"), strFrom);
				GetAttrValue(_T("to"), strTo);
				ParseDestStation(_ttoi(strFrom), _ttoi(strTo), pTrackHS->m_strDestStations);
				pTrackHS->AddDestStationID(strFrom, strTo);
				
				MoveParent();	//->Equipment/ArriveHS/HS
			}
			MoveParent();	//->Equipment/ArriveHS
			
//			delete pTrackHS;
		}
		
//		if (pTrackHS)
//		{
//			pInfo->m_pCvInfo = (CCvInfo*)pTrackHS->m_pTrack->m_pEquipment->GetInfo();
//			DEBUGER_ASSERT_VALID_LOG(pTrackHS->m_pTrack->m_pEquipment != NULL, pTrackHS->ToString());
//			DEBUGER_ASSERT_VALID_LOG(pTrackHS->m_pTrack->m_pEquipment->IsKindOf(RUNTIME_CLASS(CCv)), pTrackHS->ToString());
//			DEBUGER_ASSERT_VALID_LOG(pInfo->m_pCvInfo != NULL, pTrackHS->ToString());
//		}
//		else 
//			DEBUGER_ASSERT_VALID(FALSE);

		MoveParent();	//->Equipment/
//		delete pTrackHS;
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}


	return TRUE;
}

BOOL CEcsDefine::ParseRtvZaion(CEquipment* pEquipment)
{
/*	
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CLgv)));
	MYTRACE(_T("\n\n CEcsDefine::ParseRtvZaion(%s)"), pEquipment->GetDeviceName());
	
	CString strValue, strCID, strFloor, strHID, strJID, strPID;
	
	CString	strElmtText, strElmtName, strElmtValue;
	
	CLgv*	 pRtv = (CLgv*)pEquipment;
	DEBUGER_ASSERT_VALID(pRtv != NULL);
	
	GetAttrValue(_T("class"), strValue);				// JBY 0829
	GetAttrValue(_T("number"), strValue);				// JBY 0829
	pEquipment->m_nNumber = _ttoi(strValue);
	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());
	
	pEquipment->m_nForkType = CDciRvCtrl::enSingle;
	CLgvInfo* pInfo = NULL;
	int nCarCount = 0;

	if (MoveXPath(_T("./Cars"), FALSE))	//->Equipment
	{
		nCarCount = GetChildElmtCount();
		
		for (int i = 0 ; i < nCarCount ; i++) 
		{
			MoveChild(i);	//->Equipment/Cars/Rtv

			GetAttrValue(_T("no"), strValue);	
			int nEquipNum = _ttoi(strValue);
			pInfo = new CLgvInfo(pEquipment);
			DEBUGER_ASSERT_VALID(pInfo != NULL);
			
			if (pInfo == NULL)
			{
				DEBUGER_ASSERT_VALID(FALSE);
				return FALSE;
			}
			
			GetAttrValue(_T("cid"), strCID);
			pInfo->m_pControl = (CDciCtrlCtrl*)m_pDoc->GetDciControl(strCID);
			
			pInfo->m_nEquipNum = _ttoi(strCID.Right(2));
			
			DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
			DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);			// @@
			MYTRACE(_T("\n\t <%s cid='%s'>"), pEquipment->GetDeviceName(), pInfo->m_pControl->m_strCID);

			strJID.Format("%s%02d%02d%02d", strCID.Left(3), 31, pEquipment->m_nNumber, pInfo->m_nEquipNum);
			CDciStaticCtrl* pJobStaticCtrl = (CDciStaticCtrl*)m_pDoc->GetDciControl(strJID);

			strPID.Format("%s%02d%02d%02d", strCID.Left(3), 32, pEquipment->m_nNumber, pInfo->m_nEquipNum);
			CDciStaticCtrl* pPathStaticCtrl = (CDciStaticCtrl*)m_pDoc->GetDciControl(strPID);
			
			if (pJobStaticCtrl == NULL || pPathStaticCtrl == NULL)
			{
				DEBUGER_ASSERT_VALID(FALSE);
				return FALSE;
			}

			m_pDoc->m_pRtvJobNums.Add(pJobStaticCtrl);
			m_pDoc->m_pRtvSrcDsts.Add(pPathStaticCtrl);

			pInfo->Initialize();
//			pRtv->m_pInfo.Add(pInfo);

			MoveParent();	//->Equipment/Cars
		}

		MoveParent();	//->Equipment
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}
	
//	CArray<CTrackHS*, CTrackHS*> pDepartsHS;
	if (MoveXPath(_T("./DepartHS"), FALSE))	//->Equipment/DepartHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nDepartHSCount = GetChildElmtCount();
//		pInfo->m_pDepartsHS.SetArraySize(nDepartHSCount);
//		pRtv->m_pCtrlPositions.SetSize(nCount);
		
		for (i=0; i<nDepartHSCount; ++i)
		{
			pTrackHS = new CTrackHS(pEquipment);
			pInfo->m_pDepartsHS[i] = pTrackHS;

			DEBUGER_ASSERT_VALID(pTrackHS != NULL);
			
			MoveChild(i);	//->Equipment/Rtv/DepartHS/HS
			
			GetAttrValue(_T("pos"), strValue);
			pTrackHS->m_nPos = _ttoi(strValue);
			
			GetAttrValue(_T("tid"), strValue);
			pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);
			
			strHID.Format("%s%02d%02d%02d", strCID.Left(3), 35, pEquipment->m_nNumber, pTrackHS->m_nPos);
			CDciStaticCtrl* pStaticCtrl = (CDciStaticCtrl*)m_pDoc->GetDciControl(strHID);
			if (pStaticCtrl == NULL)
			{
				DEBUGER_ASSERT_VALID(FALSE);
//				break;
				return FALSE;
			}
//			pRtv->m_pCtrlPositions.Add(pStaticCtrl);

			j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
			pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusRtvDepartHSReady, pTrackHS);
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusRtvDepartHSReady] == pTrackHS);
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);		// 임시JBY
			
//			pDepartsHS.Add(pTrackHS);
			MYTRACE(_T("\n\t\t <DepartHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);
			
			MoveParent();	//->Equipment/DepartHS
		}
		
		MoveParent();	//->Equipment/
		delete pTrackHS;
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}	
	
	CArray<CTrackHS*, CTrackHS*> pArrivesHS;
	if (MoveXPath(_T("./ArriveHS"), FALSE))	//->Equipment/ArriveHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nArriveHSCount = GetChildElmtCount();
//		pInfo->m_pArrivesHS.SetArraySize(nArriveHSCount);
//		pRtv->m_pCtrlPositions.SetSize(nCount);
	
		for (i=0; i<nArriveHSCount; ++i)
		{
			pTrackHS = new CTrackHS(pEquipment);
//			pInfo->m_pArrivesHS[i] = pTrackHS;
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);
			
			MoveChild(i);	//->Equipment/ArriveHS/HS
			
			GetAttrValue(_T("pos"), strValue);
			pTrackHS->m_nPos = _ttoi(strValue);
			
			GetAttrValue(_T("tid"), strValue);
			pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);
			
			strHID.Format("%s%02d%02d%02d", strCID.Left(3), 35, pEquipment->m_nNumber, pTrackHS->m_nPos);
			CDciStaticCtrl* pStaticCtrl = (CDciStaticCtrl*)m_pDoc->GetDciControl(strHID);
			if (pStaticCtrl == NULL)
			{
				DEBUGER_ASSERT_VALID(FALSE);
//				break;
				return FALSE;
			}
//			pRtv->m_pCtrlPositions.Add(pStaticCtrl);
			
			j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
			pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusRtvArriveHSReady, pTrackHS);
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusRtvArriveHSReady] == pTrackHS);
			DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);		// 임시JBY
			
			MYTRACE(_T("\n\t\t <ArriveHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);
			
			CString strFrom, strTo;
			int nDestStationSize = GetChildElmtCount();
			for (j=0; j<nDestStationSize; ++j)
			{
				MoveChild(j);	//->Equipment/ArriveHS/HS/DestST
				
				GetAttrValue(_T("from"), strFrom);
				GetAttrValue(_T("to"), strTo);
				ParseDestStation(_ttoi(strFrom), _ttoi(strTo), pTrackHS->m_strDestStations);
				pTrackHS->AddDestStationID(strFrom, strTo);
				
				MoveParent();	//->Equipment/ArriveHS/HS
			}
//			pArrivesHS.Add(pTrackHS);
			MoveParent();	//->Equipment/ArriveHS
		}
		
		MoveParent();	//->Equipment/
		delete pTrackHS;
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}
/*	
	CTrackHS* pTrackHS = NULL;
	for (int n = 0 ; n < nCarCount ; n++) 
	{
		int nEquipNumber = n + 1;
		if (pEquipment->m_nNumber == 2)
			nEquipNumber += 5;

		CRtvInfo* pCarInfo = (CRtvInfo *)pRtv->GetInfo(nEquipNumber, FALSE);

		if(pCarInfo == NULL)
			continue;

		pTrackHS = pArrivesHS[n];
//		pCarInfo->m_pArrivesHS.Add(pTrackHS);

		pTrackHS = pDepartsHS[n];
		pCarInfo->m_pDepartsHS.Add(pTrackHS);
	}
*/	
//	MoveParent();	//->Equipment
	
	
//	MYTRACE(_T("\n\t </%s> \n"), pEquipment->GetDeviceName());
	
	return TRUE;
	
}

BOOL CEcsDefine::ParseRtv(CEquipment* pEquipment)
{

	/*
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CRtv)));

	CRtv* pRtv = (CRtv*)pEquipment;
	CString strValue, strCID;
	int nElmtCount = GetChildElmtCount();

	for (int n = 0; n < nElmtCount; n++)
	{
		CRtvInfo* pInfo = new CRtvInfo(pEquipment);
		DEBUGER_ASSERT_VALID(pRtv != NULL);
		DEBUGER_ASSERT_VALID(pInfo != NULL);

//		GetAttrValue(_T("device"), strValue);
//		DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

		MoveChild(n);

		GetAttrValue(_T("number"), strValue);
		pInfo->m_nNumber = _ttoi(strValue);
		DEBUGER_ASSERT_VALID(pInfo->m_nNumber != NULL);

		GetAttrValue(_T("cid"), strCID);
		pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);
		DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
		DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);

		pInfo->Initialize();
		pRtv->m_pInfo.Add(pInfo);

		MoveParent();
	}
	*/

	return TRUE;
}


BOOL CEcsDefine::ParseStation()
{
	CString strValue;
	CString strID, strName, strDest;
	CStationInfo* pStation = NULL;
	CEquipment* pEquipment = NULL;
	CTrackInfo* pTrack = NULL;
	int i, j, nCount = 0;

	if (SelectElements(_T("//ScStation")))
	{
		nCount = GetSelectCount();
		for (i=0; i<nCount; ++i)
		{
			MoveSelect(i);	//Equipment/Sc/ScStation/
			MoveParent();	//Equipment/Sc

			GetAttrValue(_T("no"), strValue);
			int nEquipNum = _ttoi(strValue);

			MoveParent();	//Equipment

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);
			
			GetAttrValue(_T("number"), strValue);
			int nEquipGroupNum = _ttoi(strValue);

			////////////////////////////////////////////////////////////////

//			GetAttrValue(_T("cid"), strValue);
//			int nEquipNum = _ttoi(strValue);

			MoveSelect(i);	//Equipment/Sc/ScStation/

			GetAttrValue(_T("id"), strID);	
			GetAttrValue(_T("name"), strName);
			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enScStation, pEquipment, NULL, nEquipNum);
			DEBUGER_ASSERT_VALID(pStation != NULL);

//			CScInfo* pInfo = (CScInfo*)pEquipment->GetInfo(nEquipNum);
			CScDongJin* pSc = (CScDongJin*) pEquipment;
			CScInfo* pInfo =  (CScInfo*)pSc->GetInfo(nEquipNum, TRUE);

			DEBUGER_ASSERT_VALID(pInfo != NULL);
			pInfo->m_pStation = pStation;

			pStation->m_strID = strID;	
			pStation->m_strName = strName;
			m_pDoc->m_pStationInfos.Add(pStation);
			MYTRACE(_T("\n <ScStation id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}

	if (SelectElements(_T("//StoStation")))
	{
		nCount = GetSelectCount();
		for (i=0; i<nCount; ++i)
		{
			MoveSelect(i);	//Equipment/Tracks/Track/Status/StoStation
			MoveParent();	//Equipment/Tracks/Track/Status
			MoveParent();	//Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);
			strID = strValue;
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
			pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	//Equipment/Tracks
			MoveParent();	//Equipment

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			GetAttrValue(_T("id"), strID);	
			GetAttrValue(_T("name"), strName);
			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			if (_ttoi(strDest) != 0 )
				pTrack->m_nDestination = _ttoi(strDest);

			if (pTrack && CStationInfo::IsValidID(strID))
				pTrack->m_bReportTrack = (strID[2] == '4') ? FALSE : TRUE;

			pStation = new CStationInfo(CStationInfo::enStoStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			pStation->m_strID = strID;
			pStation->m_strName = strName;
//			pStation->m_bVirTrack = pEquipment->m_bVirTrack;
			m_pDoc->m_pStationInfos.Add(pStation);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusStoSTReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusStoSTReady] == pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);
			MYTRACE(_T("\n <StoStation id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}

	if (SelectElements(_T("//RetStation")))
	{
		nCount = GetSelectCount();
		for (i=0; i<nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/RetStation
			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);
			strID = strValue;
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
			//pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			GetAttrValue(_T("id"), strID);	
			GetAttrValue(_T("name"), strName);
			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);

			if (pTrack && CStationInfo::IsValidID(strID))
				pTrack->m_bReportTrack = (strID[2] == '4') ? FALSE : TRUE;

			pStation = new CStationInfo(CStationInfo::enRetStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			pStation->m_strID = strID;
			pStation->m_strName = strName;
//			pStation->m_bVirTrack = pEquipment->m_bVirTrack;
			m_pDoc->m_pStationInfos.Add(pStation);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusRetSTReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusRetSTReady] == pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);
			MYTRACE(_T("\n <RetStation id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}

	if (SelectElements(_T("//ArvStation")))
	{
		nCount = GetSelectCount();
		for (i=0; i<nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/ArvStation
			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);
			strID = strValue;
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
			pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			GetAttrValue(_T("id"), strID);	
			GetAttrValue(_T("name"), strName);
			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enArvStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusArvSTReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusArvSTReady] == pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);

			pStation->m_strID = strID;
			pStation->m_strName = strName;
//			pStation->m_bVirTrack = pEquipment->m_bVirTrack;
			m_pDoc->m_pStationInfos.Add(pStation);
			MYTRACE(_T("\n <ArvStation id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}

	if (SelectElements(_T("//RetLimitInfo")))
	{
		nCount = GetSelectCount();
		for (i=0; i<nCount; ++i)
		{
			MoveSelect(i);	//Track/RetLimitInfo
			MoveParent();

			GetAttrValue(_T("number"), strValue);
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			CStationInfo* pRetStation = pTrack->GetRetStation();
			if (pRetStation == NULL)
				pRetStation = pTrack->GetArvStation();

			DEBUGER_ASSERT_VALID_LOG(pRetStation != NULL, pTrack->GetTrackString());

			MoveSelect(i);
			GetAttrValue(_T("count"), strValue);
			m_pDoc->m_mapRetLimitInfos.SetAt(pRetStation->m_strID, strValue);
			MYTRACE(_T("\n <RetLimitInfo sid='%s' cnt='%s' desc='%s'/>"), pRetStation->m_strID, strValue, pRetStation->m_strName);
		}
	}

	return TRUE;
}

void CEcsDefine::ParseDestStation(int nFrom, int nTo, CStringArray& strDestStations)
{
	CString strSID, strXPath;
	for (int nSID=nFrom; nSID<=nTo; ++nSID)
	{
		strSID.Format(_T("%d"), nSID);

		strXPath.Format(_T("//RetStation[@id=%d]"), nSID);
		if (GetNodeCount(strXPath) > 0)
		{
			MYTRACE(_T("\n\t\t\t <DestStation sid='%s'/>"), strSID);
			strDestStations.Add(strSID);
			continue;
		}

		strXPath.Format(_T("//ScStation[@id=%d]"), nSID);
		if (GetNodeCount(strXPath) > 0)
		{
			MYTRACE(_T("\n\t\t\t <DestStation sid='%s'/>"), strSID);
			strDestStations.Add(strSID);
			continue;
		}

		strXPath.Format(_T("//StoStation[@id=%d]"), nSID);
		if (GetNodeCount(strXPath) > 0)
		{
			MYTRACE(_T("\n\t\t\t <DestStation sid='%s'/>"), strSID);
			strDestStations.Add(strSID);
			continue;
		}

		strXPath.Format(_T("//ArvStation[@id=%d]"), nSID);
		if (GetNodeCount(strXPath) > 0)
		{
			MYTRACE(_T("\n\t\t\t <DestStation sid='%s'/>"), strSID);
			strDestStations.Add(strSID);
			continue;
		}
	}
}
