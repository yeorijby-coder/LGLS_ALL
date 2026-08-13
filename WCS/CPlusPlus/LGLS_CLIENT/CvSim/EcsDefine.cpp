// EcsDefine.cpp: implementation of the CEcsDefine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "EcsDefine.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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

	if (strClassName == RUNTIME_CLASS(CCv)->m_lpszClassName) return new CCv(m_pDoc, nIndex, nNumber, lpszDevice);
	else if (strClassName == _T("CCvTableLifter")) return NULL;
	else if (strClassName == RUNTIME_CLASS(CSc)->m_lpszClassName || strClassName == RUNTIME_CLASS(CScPair)->m_lpszClassName)
	{
		strClassName = RUNTIME_CLASS(CScPair)->m_lpszClassName;
		return new CScPair(m_pDoc, nIndex, nNumber, lpszDevice);
	}
	else if (strClassName == _T("CWc")) return NULL;
	else if (strClassName == _T("CBcr")) return NULL;//RUNTIME_CLASS(CBcr)->m_lpszClassName) return new CBcr(m_pDoc, nIndex, nNumber, lpszDevice);
	else if (strClassName == _T("CThs")) return NULL;//RUNTIME_CLASS(CThs)->m_lpszClassName) return new CThs(m_pDoc, nIndex, nNumber, lpszDevice);
	else if (strClassName == _T("CRtv")) return NULL;//RUNTIME_CLASS(CRtv)->m_lpszClassName) return new CRtv(m_pDoc, nIndex, nNumber, lpszDevice);
	else if (strClassName == _T("CLifter")) return NULL;//RUNTIME_CLASS(CLifter)->m_lpszClassName) return new CLifter(m_pDoc, nIndex, nNumber, lpszDevice);
	else if (strClassName == _T("CLgv")) return NULL;//RUNTIME_CLASS(CLgv)->m_lpszClassName) return new CLgv(m_pDoc, nIndex, nNumber, lpszDevice);
	else if (strClassName == _T("CRfid")) return NULL;//RUNTIME_CLASS(CRfid)->m_lpszClassName) return new CRfid(m_pDoc, nIndex, nNumber, lpszDevice);
	else if (strClassName == _T("CDisplay")) return NULL;//RUNTIME_CLASS(CDisplay)->m_lpszClassName) return new CDisplay(m_pDoc, nIndex, nNumber, lpszDevice);
	else DEBUGER_ASSERT_VALID_LOG(FALSE, strClassName);

	return NULL;
}

BOOL CEcsDefine::ParseXml()
{
	if (!CXmlDom::InitializeXmlDom())
		return FALSE;

	if (!CXmlDom::LoadXmlFile(ECS_DEFINE_FILE))
		return FALSE;

	//전용설비 Station 추가
	//강용규 나중에 수정하기 
	//ParseEquip();

	CEquipment* pEquipment = NULL;
	CString strClassName, strDevice, strCID, strValue, strForkType, strThreadNo;
	int i, nScCnt = 0, nCount = GetChildElmtCount();
	m_pDoc->m_pEquipments.SetSize(nCount);

	try
	{
		for (i = 0; i < nCount; ++i)
		{
			MoveRoot();
			MoveChild(i);

			GetElmtName(strValue);
			DEBUGER_ASSERT_VALID(strValue == _T("Equipment"));

			GetAttrValue(_T("class"), strClassName);
			GetAttrValue(_T("thread"), strThreadNo);
			GetAttrValue(_T("number"), strValue);
			GetAttrValue(_T("device"), strDevice);

			if (strClassName.Left(3) == "CSc")
			{
				CString strValue, strCID;
				CString strPositionPLC_READ, strPositionWCS_VIEW;
				CString strPLC_NO, strEQP_NO;
				CString strPlcPos, strViewPos;

				CEquipment* pRealEquipment = NULL;

				CScPair* pSc = NULL;// (CScPair*)pEquipment;
				CScInfo* pInfo = NULL;// pSc->m_pInfo;
				MoveXPath(_T("./Scs"), FALSE);

				int nEquipmentCnt = m_pDoc->m_pEquipments.GetSize();
				nScCnt = GetChildElmtCount();

				m_pDoc->m_pEquipments.SetSize(nEquipmentCnt + nScCnt - 1); // 기존에 가상으로 1개를 만들었으니까 없애야함!

				nEquipmentCnt = m_pDoc->m_pEquipments.GetSize();

				for (int nIdxSc = 0; nIdxSc < nScCnt; nIdxSc++)
				{
					MoveChild(nIdxSc);
					GetAttrValue(_T("plcno"), strPLC_NO);
					GetAttrValue(_T("number"), strEQP_NO);

					m_pDoc->m_pEquipments[i + nIdxSc] = pEquipment = CreateEquipment(strClassName, nIdxSc, CConvert::ToInt(strPLC_NO), strDevice);

					pSc = (CScPair*)pEquipment;
					DEBUGER_ASSERT_VALID(pSc != NULL);
					//pSc->m_nNumber = CConvert::ToInt(strPLC_NO);//m_nNumber 사용되지 않음
					pInfo = pSc->m_pInfo;
					DEBUGER_ASSERT_VALID(pInfo != NULL);

					strCID.Format(_T("17120%03s"), strEQP_NO);
					pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);

					MoveParent();
				}

				--nScCnt;// 기존에 가상으로 1개를 만들었으니까(define.xml에서 Sc를 1개로 묶어버렸으니까) 없애야함!

				continue;
			}
			else
				m_pDoc->m_pEquipments[i + nScCnt] = pEquipment = CreateEquipment(strClassName, i + nScCnt, _ttoi(strValue) + nScCnt, strDevice);

			if (strClassName == "CSc")
			{
				CString a = "";
			}

			if (pEquipment == NULL)
				continue;

			//DEBUGER_ASSERT_VALID(pEquipment != NULL);

			switch (pEquipment->m_enKind)
			{
			case CEquipment::enCV:
				if (!ParseCv(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseCv!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;

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
		CString strCID, strTrackCID, strTabNum;

		GetAttrValue(_T("cid"), strCID);
		DEBUGER_ASSERT_VALID_LOG(CLib::IsValidControlID(strCID), strCID);

		GetAttrValue(_T("FirstTrack"), strValue);
		int nFirstTr = _ttoi(strValue);

		pEquipment->m_nStTrNum = nFirstTr;

		GetAttrValue(_T("plcno"), strValue);
		int index = _ttoi(strValue);
		m_pDoc->m_nStTrNum[index - 1] = pEquipment->m_nStTrNum;

		CTrackInfo* pTrack = NULL;
		int i, j, nCount = GetChildElmtCount();
		pTracks.SetSize(nCount);

		// CID 10자리 기준으로 구성되었음!
		// 롯데정밀화학 G동 8자리 기준
		for (i = 0; i < nCount; ++i)
		{
			MoveChild(i);	//Equipment/Tracks/Track

			pTracks[i] = pTrack = new CTrackInfo(pEquipment);
			DEBUGER_ASSERT_VALID(pTrack != NULL);

			GetAttrValue(_T("number"), strValue);
			pTrack->m_nCvPlcNum = _ttoi(strCID.Mid(5, 1)); //17'1'10001		

			pTrack->m_nNumber = _ttoi(strValue);

			GetAttrValue(_T("tabnumber"), strTabNum);
			pTrack->m_nTabNum = _ttoi(strTabNum);

			strTrackCID.Format(_T("%s%03d"), strCID.Left(5), pTrack->m_nNumber);

			if (strTrackCID == "17210202")
				int babo = 0;

			int nTemp = 0;

			pTrack->m_pTrackCtrl = (CDciTrackCtrl*)m_pDoc->GetDciControl(strTrackCID, _ttoi(strTabNum));
			DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strTrackCID));

			// 이거는 Status 보다 앞에 있어야 한다.
			if (MoveXPath(_T("./ForkLift"), FALSE))	//Equipment/Tracks/Track/ForkLift
			{
				//GetAttrValue(_T("tid"), strValue);
				//pTrack->m_nDestination = _ttoi(strValue);
				m_pDoc->m_sEcsDefineLoad[pTrack->m_nNumber][0] = _T("ForkLift");
				nTemp = 1;

				MoveParent();
			}

			if (MoveXPath(_T("./Status"), FALSE))	//Equipment/Tracks/Track/Status
			{
				for (j = 0; j < GetChildElmtCount(); ++j)
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
					else if (strValue == _T("LGVDepartHS")) wStatus = CTrackInfo::enStatusLgvDepartHSReady;
					else if (strValue == _T("LGVArriveHS")) wStatus = CTrackInfo::enStatusLgvArriveHSReady;
					else if (strValue == _T("LfDepartHS")) wStatus = CTrackInfo::enStatusLfDepartHSReady;
					else if (strValue == _T("LfArriveHS")) wStatus = CTrackInfo::enStatusLfArriveHSReady;
					else if (strValue == _T("CvCheckPos")) wStatus = CTrackInfo::enStatusCvCheckReady;
					else if (strValue == _T("ArvStation2")) wStatus = CTrackInfo::enStatusArvSTReady;
					else if (strValue == _T("FullChecker")) wStatus = CTrackInfo::enStatusFullReady;
					else DEBUGER_ASSERT_VALID_LOG(FALSE, strValue);

					pTrack->m_mapStatusInfos.SetAt(wStatus, NULL);
					pTrack->m_arrayStatus.Add(wStatus);

					m_pDoc->m_sEcsDefineLoad[pTrack->m_nNumber][j + nTemp] = strValue;
				}

				MoveParent();	//Equipment/Tracks/Track
			}

			if (MoveXPath(_T("./Destination"), FALSE))	//Equipment/Tracks/Track/Destination
			{
				GetAttrValue(_T("tid"), strValue);
				pTrack->m_nDestination = _ttoi(strValue);

				MoveParent();
			}
			//if (MoveXPath(_T("./Dispenser"), FALSE))	//Equipment/Tracks/Track/Destination
			//{
			//	GetAttrValue(_T("uptid"), strValue);
			//	pTrack->m_nDispenser = _ttoi(strValue);
			//	pTrack->m_nDPPlcNum = _ttoi(strValue) / 1000;
			//	MoveParent();
			//}
			//if (MoveXPath(_T("./TrayPickerStartStation"), FALSE))	//Equipment/Tracks/Track/Destination
			//{
			//	pTrack->m_nTrayPicker = _ttoi(strValue);

			//	MoveParent();
			//}
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

	CString strValue, strCID;
	CCv* pCv = (CCv*)pEquipment;
	CCvInfo* pInfo = pCv->m_pInfo;
	DEBUGER_ASSERT_VALID(pCv != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	strValue = pEquipment->m_port.m_strDevice;
	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID_LOG(strValue == pEquipment->GetDeviceName(), strValue);

	return ParseTracks(pEquipment, pInfo->m_pTracks);
}

BOOL CEcsDefine::ParseSc(CEquipment* pEquipment, int nForkType/* = 0*/)
{
	if (nForkType != 0)
		return ParseScPair(pEquipment);
	else
		return ParseScSingle(pEquipment);

}

BOOL CEcsDefine::ParseScSingle(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CSc)));
	MYTRACE(_T("\n\n CEcsDefine::ParseSc(%s)"), pEquipment->GetDeviceName());

	CString strValue, strCID, strFloor, strCID2, strCID3;

	CSc* pSc = (CSc*)pEquipment;
	CScInfo* pInfo = pSc->m_pInfo;
	DEBUGER_ASSERT_VALID(pSc != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("floorcount"), strFloor);

	GetAttrValue(_T("cid"), strCID);
	pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);
	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);
	if (pInfo->m_pControl == NULL)
		return FALSE;

	if (strFloor == "2")
	{
		strCID2.Format(_T("%d"), (_ttoi(strCID) + 10));
		pInfo->m_pControl2 = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID2);
		DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID2));
		DEBUGER_ASSERT_VALID(pInfo->m_pControl2 != NULL);
	}
	else if (strFloor == "3")
	{
		GetAttrValue(_T("cid2"), strCID2);
		pInfo->m_pControl2 = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID2, _ttoi(strCID2.Mid(6, 1)));
		DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID2));
		DEBUGER_ASSERT_VALID(pInfo->m_pControl2 != NULL);

		GetAttrValue(_T("cid3"), strCID3);
		pInfo->m_pControl3 = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID3, _ttoi(strCID3.Mid(6, 1)));
		DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID3));
		DEBUGER_ASSERT_VALID(pInfo->m_pControl3 != NULL);
	}

	MYTRACE(_T("\n\t <%s cid='%s'>"), pEquipment->GetDeviceName(), pInfo->m_pControl->m_strCID);

	if (MoveXPath(_T("./StoHS"), FALSE))	//Equipment/StoHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();
		pInfo->m_pStosHS.SetArraySize(nCount);

		for (i = 0; i < nCount; ++i)
		{
			pInfo->m_pStosHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/StoHS/HS

			GetAttrValue(_T("pos"), strValue);
			pTrackHS->m_nPos = _ttoi(strValue);

			GetAttrValue(_T("tid"), strValue);
			pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
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

		for (i = 0; i < nCount; ++i)
		{
			pInfo->m_pRetsHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/RetHS/HS

			GetAttrValue(_T("pos"), strValue);
			pTrackHS->m_nPos = _ttoi(strValue);

			GetAttrValue(_T("tid"), strValue);
			pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
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
			for (j = 0; j < nDestStationSize; ++j)
			{
				MoveChild(j);	//Equipment/RetHS/HS/DestST

				GetAttrValue(_T("from"), strFrom);
				GetAttrValue(_T("to"), strTo);

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

BOOL CEcsDefine::ParseScPairNew(CEquipment* pEquipment, int& nScCnt)
{
	/*
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CScPair)));

	CString strValue, strCID;
	CString strPositionPLC_READ, strPositionWCS_VIEW;
	CString strPLC_NO, strEQP_NO;
	CString strPlcPos, strViewPos;
	CString strDevice, strClassName;
	//int nScCnt;

	CEquipment* pRealEquipment = NULL;

	CScPair* pSc = NULL;// (CScPair*)pEquipment;
	//pSc->m_WH_TYP = m_pDoc->m_WH_TYP;
	CScInfo* pInfo = NULL;// pSc->m_pInfo;
	MoveXPath(_T("./Scs"), FALSE);

	int nEquipmentCnt = m_pDoc->m_pEquipments.GetSize();
	nScCnt = GetChildElmtCount();

	m_pDoc->m_pEquipments.SetSize(nEquipmentCnt + nScCnt - 1); // 기존에 가상으로 1개를 만들었으니까 없애야함!

	//pInfo->m_MapSC_DATA.InitHashTable(nScCnt);
	for(int nIdxSc = 0; nIdxSc < nScCnt; nIdxSc++)
	{
		MoveChild(nIdxSc);
		GetAttrValue(_T("plcno"), strPLC_NO);
		GetAttrValue(_T("number"), strEQP_NO);

		strClassName = pEquipment->GetRuntimeClass()->m_lpszClassName;
		strDevice.Format(_T("SC_E%02d"), CConvert::ToInt(strPLC_NO));
		m_pDoc->m_pEquipments[pEquipment->m_nIndex + nIdxSc] = pRealEquipment = CreateEquipment(strClassName, nIdxSc, CConvert::ToInt(strPLC_NO), strDevice);

		pSc = (CScPair*)pRealEquipment;
		DEBUGER_ASSERT_VALID(pSc != NULL);
		//pSc->m_nNumber = CConvert::ToInt(strPLC_NO);//m_nNumber 사용되지 않음
		pInfo = pSc->m_pInfo;
		DEBUGER_ASSERT_VALID(pInfo != NULL);

		strCID.Format(_T("17120%03s"), strEQP_NO);
		pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);
		//CSC_DATA* pSC_DATA = pInfo->CreateSC_DATA(strEQP_NO);
		//pSc->m_pInfo->m_MapSC_DATA.SetAt(pSC_DATA->K_SC_NO, pSC_DATA);
		//pSC_DATA->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid());
		//pSC_DATA->m_pControl2 = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid(_T("1")));
		//pSC_DATA->m_pControl3 = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid(_T("2")));
		//pSC_DATA->m_pControl4 = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid(_T("3")));
		//pSC_DATA->m_pControl5 = (CDciRvCtrl*)m_pDoc->GetDciControl_FindAllLayout(pSC_DATA->GetCid(_T("4")));
		MoveParent();
	}

	--nScCnt;		// 기존에 가상으로 1개를 만들었으니까 없애야함! - 최종 리턴값을 수정해줌!
	//*/
	return TRUE;
}

BOOL CEcsDefine::ParseScPair(CEquipment* pEquipment)
{
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CScPair)));
	MYTRACE(_T("\n\n CEcsDefine::ParseScPair(%s)"), pEquipment->GetDeviceName());

	CString strValue, strCID, strFloor, strForkType, strCID2, strCID3;

	CScPair* pSc = (CScPair*)pEquipment;

	CScInfo* pInfo = pSc->m_pInfo;
	DEBUGER_ASSERT_VALID(pSc != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("class"), strValue);			// JBY 0829

	GetAttrValue(_T("number"), strValue);			// JBY 0829

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("floorcount"), strFloor);

	GetAttrValue(_T("cid"), strCID);
	pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);
	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);

	if (MoveXPath(_T("./StoHS"), FALSE))	//Equipment/StoHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();
		pInfo->m_pStosHS.SetArraySize(nCount);

		for (i = 0; i < nCount; ++i)
		{
			pInfo->m_pStosHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/StoHS/HS

			GetAttrValue(_T("pos"), strValue);
			pTrackHS->m_nPos = _ttoi(strValue);

			GetAttrValue(_T("tid"), strValue);
			pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
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

		for (i = 0; i < nCount; ++i)
		{
			pInfo->m_pRetsHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/RetHS/HS

			GetAttrValue(_T("pos"), strValue);
			pTrackHS->m_nPos = _ttoi(strValue);

			GetAttrValue(_T("tid"), strValue);
			pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
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
			for (j = 0; j < nDestStationSize; ++j)
			{
				MoveChild(j);	//Equipment/RetHS/HS/DestST

				GetAttrValue(_T("from"), strFrom);
				GetAttrValue(_T("to"), strTo);

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

BOOL CEcsDefine::ParseRtv(CEquipment* pEquipment)
{
	/*
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CRtv)));
	MYTRACE(_T("\n\n CEcsDefine::ParseRtv(%s)"), pEquipment->GetDeviceName());

	int nPlcNum =0;
	CString strValue, strCID;
	CRtv* pRtv = (CRtv*)pEquipment;
	CRtvInfo* pInfo = pRtv->m_pInfo;
	DEBUGER_ASSERT_VALID(pRtv != NULL);
	DEBUGER_ASSERT_VALID(pInfo != NULL);

	GetAttrValue(_T("device"), strValue);
	DEBUGER_ASSERT_VALID(strValue == pEquipment->GetDeviceName());

	GetAttrValue(_T("cid"), strCID);
	pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);
	DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);
	MYTRACE(_T("\n\t <%s cid='%s'>"), pEquipment->GetDeviceName(), pInfo->m_pControl->m_strCID);

	if (MoveXPath(_T("./DepartHS"), FALSE))		//Equipment/DepartHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();
		pInfo->m_pDepartsHS.SetArraySize(nCount);

		for (i=0; i<nCount; ++i)
		{
			pInfo->m_pDepartsHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/DepartHS/HS

				GetAttrValue(_T("pos"), strValue);
				pTrackHS->m_nPos = _ttoi(strValue);

//				GetAttrValue(_T("plc"), strValue);
//				nPlcNum = _ttoi(strValue);

				GetAttrValue(_T("tid"), strValue);
				pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), nPlcNum);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

				j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
				pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusRtvDepartHSReady, pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusRtvDepartHSReady] == pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

				MYTRACE(_T("\n\t\t <DepartHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

			MoveParent();	//Equipment/DepartHS
		}

		MoveParent();	//Equipment
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}

	if (MoveXPath(_T("./ArriveHS"), FALSE))		//Equipment/ArriveHS
	{
		CTrackHS* pTrackHS = NULL;
		int i, j, nCount = GetChildElmtCount();
		pInfo->m_pArrivesHS.SetArraySize(nCount);

		for (i=0; i<nCount; ++i)
		{
			pInfo->m_pArrivesHS[i] = pTrackHS = new CTrackHS(pEquipment);
			DEBUGER_ASSERT_VALID(pTrackHS != NULL);

			MoveChild(i);	//Equipment/ArriveHS/HS

				GetAttrValue(_T("pos"), strValue);
				pTrackHS->m_nPos = _ttoi(strValue);

//				GetAttrValue(_T("plc"), strValue);
//				nPlcNum = _ttoi(strValue);

				GetAttrValue(_T("tid"), strValue);
				pTrackHS->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), nPlcNum);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

//				GetAttrValue(_T("deadlock"), strValue);
//				pTrackHS->m_bDeadLockTrack = _ttoi(strValue);
//				int nDeadlock = _ttoi(strValue);
//				if (nDeadlock == 2)
//				{
//					pTrackHS->m_bBufferTrack = TRUE;
//					GetAttrValue(_T("matchingtrack"), strValue);
//					pTrackHS->m_nBufferMatchingTrack = _ttoi(strValue);
//				}
//				else
//				{
//					pTrackHS->m_bDeadLockTrack = nDeadlock;
//				}

				j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
				pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusRtvArriveHSReady, pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusRtvArriveHSReady] == pTrackHS);
				DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);

				MYTRACE(_T("\n\t\t <ArriveHS pos='%d' tid='%d'/>"), pTrackHS->m_nPos, pTrackHS->m_pTrack->m_nNumber);

				CString strFrom, strTo;
				int nDestStationSize = GetChildElmtCount();
				for (j=0; j<nDestStationSize; ++j)
				{
					MoveChild(j);	//Equipment/ArriveHS/HS/DestST

						GetAttrValue(_T("from"), strFrom);
						GetAttrValue(_T("to"), strTo);
//						ParseDestStation(_ttoi(strFrom), _ttoi(strTo), pTrackHS->m_strDestStations);
						pTrackHS->AddDestStationID(strFrom, strTo);

					MoveParent();	//Equipment/ArriveHS/HS
				}

			MoveParent();	//Equipment/ArriveHS
		}

		MoveParent();	//Equipment
	}
	else
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}

	MYTRACE(_T("\n\t </%s> \n"), pEquipment->GetDeviceName());
	//*/
	return TRUE;
}

BOOL CEcsDefine::ParseLifter(CEquipment* pEquipment)
{
	/*	DEBUGER_ASSERT_VALID(pEquipment != NULL);
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
	//	pInfo->m_pControl = (CDciRvCtrl*)m_pDoc->GetDciControl(strCID);
		DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
	//	DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);

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
						pInfo->m_pArrivesHS.Add(pTrackHS);
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
	*/
	return TRUE;
}

BOOL CEcsDefine::ParseLgv(CEquipment* pEquipment)
{
	/*	DEBUGER_ASSERT_VALID(pEquipment != NULL);
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
						pInfo->m_pArrivesHS.Add(pTrackHS);

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
	*/
	return TRUE;
}

BOOL CEcsDefine::ParseRfid(CEquipment* pEquipment)
{
	/*	DEBUGER_ASSERT_VALID(pEquipment != NULL);
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
	*/
	return TRUE;
}

BOOL CEcsDefine::ParseBcr(CEquipment* pEquipment)
{
	/*
	DEBUGER_ASSERT_VALID(pEquipment != NULL);
	DEBUGER_ASSERT_VALID(pEquipment->IsKindOf(RUNTIME_CLASS(CBcr)));

	CString strValue, strCID, strTab, strPLC, strType, strDeviceName;
	int nUpper = 0;
	int nBcrKind = 0;

	CBcr* pBcr = (CBcr*)pEquipment;
	CBcrInfo* pInfo = NULL;

	DEBUGER_ASSERT_VALID(pBcr != NULL);

	GetAttrValue(_T("type"), strType);
	if (strType == "S")
	{
		nBcrKind = CBcrInfo::enBcrKindSingle;
	}
	else if (strType == "D")
	{
		nBcrKind = CBcrInfo::enBcrKindTwin;
	}
	else if (strType == "T")
	{
		nBcrKind = CBcrInfo::enBcrKindTriple;
	}

	GetAttrValue(_T("device"), strDeviceName);
	DEBUGER_ASSERT_VALID(strDeviceName == pEquipment->GetDeviceName());
//	GetAttrValue(_T("tabnumber"), strTab);
	GetAttrValue(_T("cid"), strCID);
	DEBUGER_ASSERT_VALID(GetChildElmtCount() == 1);
	MoveChildFirst();
	GetAttrValue(_T("tid"), strValue);

	for(int i =0; i < nBcrKind; i++)
	{
		pInfo = new CBcrInfo(pEquipment);

//		pInfo->m_nStationKind = (CLib::GetStoStation(_ttoi(strDeviceName.Right(2))) == TRUE)   ?  CBcrInfo::enStationKindSto : CBcrInfo::enStationKindArv;
//		pInfo->m_nBcrKind = nBcrKind;
		pInfo->m_nTabNum = _ttoi(strTab);
//		pInfo->m_nPlcNum = _ttoi(strType);
		pInfo->m_pControl = (CDciButtonCtrl*)m_pDoc->GetDciControl(strCID);
		DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strCID));
		DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);

		pInfo->m_pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
		DEBUGER_ASSERT_VALID(pInfo->m_pTrack != NULL);

		pBcr->m_pInfo.Add(pInfo);
	}

	if (pBcr->m_nNumber == 66)
		int b = 0;

//	if (pBcr->m_pInfo.GetCount() != nBcrKind)
//	{
//		int a=0;
//	}
//*/
	return TRUE;
}

BOOL CEcsDefine::ParseEquip()
{
	for (int i = 100; i < 400; i++)
	{
		CString strCID, strID;
		strCID.Format(_T("530158%3d"), i);
		strID.Format(_T("%s"), strCID.Right(4));
		if ((CDciStaticCtrl*)m_pDoc->GetDciControl(strCID) != NULL)
		{
			CStationInfo* pStation = NULL;
			pStation = new CStationInfo(CStationInfo::enEquipStation, NULL, NULL);
			//			DEBUGER_ASSERT_VALID(pStation != NULL);
			pStation->m_strID = strID;
			pStation->m_strName = "전용설비";
			m_pDoc->m_pStationInfos.Add(pStation);
		}
	}

	return TRUE;
}
BOOL CEcsDefine::ParseDisplay(CEquipment* pEquipment)
{
	/*	DEBUGER_ASSERT_VALID(pEquipment != NULL);
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
	*/
	return TRUE;
}

//==============================================================================
//	Desc	: 온습도 센서
//	Date	: 2013.09
//	Update	:
//==============================================================================
BOOL CEcsDefine::ParseThs(CEquipment* pEquipment)
{
	return TRUE;
}

BOOL CEcsDefine::ParseStation()
{
	CString strValue;
	CString strID, strName, strCID, strSt, strDest, strDestCode;
	CStationInfo* pStation = NULL;
	CEquipment* pEquipment = NULL;
	CTrackInfo* pTrack = NULL;
	int i, j, nTabNum, nCount = 0;

	if (SelectElements(_T("//ScStation")))
	{
		nCount = GetSelectCount();
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	//Equipment/ScStation
			MoveParent();	//Equipment

			GetAttrValue(_T("plcno"), strValue);
			int nNumber = _ttoi(strValue);
			pEquipment = m_pDoc->GetEquipment(CEquipment::enSC, nNumber);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////
			GetAttrValue(_T("number"), strID);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			//GetAttrValue(_T("id"), strID);	
			GetAttrValue(_T("name"), strName);
			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enScStation, pEquipment);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			CScInfo* pInfo = (CScInfo*)pEquipment->GetInfo();
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
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	//Equipment/Tracks/Track/Status/StoStation
			//GetAttrValue(_T("id"), strCID);
			GetAttrValue(_T("DestCode"), strDestCode);
			MoveParent();	//Equipment/Tracks/Track/Status
			MoveParent();	//Equipment/Tracks/Track			

			GetAttrValue(_T("number"), strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	//Equipment/Tracks
			MoveParent();	//Equipment

			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);
			pTrack->m_bReportTrack = TRUE;
			pTrack->m_nDestCode = _ttoi(strDestCode);

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			GetAttrValue(_T("name"), strName);
			//			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);

			//			if (pTrack && CStationInfo::IsValidID(strID))
			//				pTrack->m_bReportTrack = (strID[2] == '4') ? FALSE : TRUE;

			pStation = new CStationInfo(CStationInfo::enStoStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			pStation->m_strID = CConvert::ToString(pTrack->m_nNumber);
			pStation->m_strName = strName;
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
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/RetStation
			//GetAttrValue(_T("id"), strCID);
			GetAttrValue(_T("DestCode"), strDestCode);

			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
			//pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);
			pTrack->m_nDestCode = _ttoi(strDestCode);

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			GetAttrValue(_T("name"), strName);
			//			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);

			//			if (pTrack && CStationInfo::IsValidID(strID))
			//				pTrack->m_bReportTrack = (strID[2] == '4') ? FALSE : TRUE;

			pStation = new CStationInfo(CStationInfo::enRetStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			pStation->m_strID = CConvert::ToString(pTrack->m_nNumber);		//strID;
			pStation->m_strName = strName;
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
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/ArvStation
			GetAttrValue(_T("DestCode"), strDestCode);
			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);
			pTrack->m_bReportTrack = TRUE;
			pTrack->m_nDestCode = _ttoi(strDestCode);
			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

		//	GetAttrValue(_T("cid"), strCID);
		//	pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue));
		//	pTrack->m_bReportTrack = TRUE;
		//	DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);
/*
			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);
*/
////////////////////////////////////////////////////////////////

			MoveSelect(i);
			GetAttrValue(_T("name"), strName);
			//			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);

			//			GetAttrValue(_T("id"), strID);
						//DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enArvStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusArvSTReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusArvSTReady] == pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);

			pStation->m_strID = CConvert::ToString(pTrack->m_nNumber);		//strID;
			pStation->m_strName = strName;
			m_pDoc->m_pStationInfos.Add(pStation);
			MYTRACE(_T("\n <ArvStation name='%s' id='%s'/>"), pStation->m_strName, pStation->m_strID);
		}
	}

	if (SelectElements(_T("//FullChecker")))
	{
		nCount = GetSelectCount();
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/FullChecker
			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

			GetAttrValue(_T("cid"), strCID);
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
			pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			//			GetAttrValue(_T("id"), strID);
			//			GetAttrValue(_T("name"), strName);
			//			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			//			DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enFullStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusFullReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusFullReady] == pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);

			pStation->m_strID = CConvert::ToString(pTrack->m_nNumber);		//strID;
			pStation->m_strName = strName;
			m_pDoc->m_pStationInfos.Add(pStation);
			MYTRACE(_T("\n <FullStation id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}
	if (SelectElements(_T("//ScRetHS")))
	{
		nCount = GetSelectCount();
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/FullChecker
			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

		//GetAttrValue(_T("cid"), strCID);
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
			pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			//			GetAttrValue(_T("id"), strID);
			//			GetAttrValue(_T("name"), strName);
			//			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			//			DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enRetHSStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScRetHSReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScRetHSReady] == pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);

			pStation->m_strID = CConvert::ToString(pTrack->m_nNumber);		//strID;
			pStation->m_strName = strName;
			m_pDoc->m_pStationInfos.Add(pStation);
			MYTRACE(_T("\n <enRetHSStation id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}
	if (SelectElements(_T("//ScStoHS")))
	{
		nCount = GetSelectCount();
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/FullChecker
			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

			//GetAttrValue(_T("cid"), strCID);
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
			pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			//			GetAttrValue(_T("id"), strID);
			//			GetAttrValue(_T("name"), strName);
			//			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			//			DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enStoHSStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScStoHSReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScStoHSReady] == pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);

			pStation->m_strID = CConvert::ToString(pTrack->m_nNumber);		//strID;
			pStation->m_strName = strName;
			m_pDoc->m_pStationInfos.Add(pStation);
			MYTRACE(_T("\n <enStoHSStation id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}
	if (SelectElements(_T("//LGVArriveHS")))
	{
		nCount = GetSelectCount();
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/FullChecker
			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

			GetAttrValue(_T("cid"), strCID);
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
			pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			//			GetAttrValue(_T("id"), strID);
			//			GetAttrValue(_T("name"), strName);
			//			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			//			DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enLgvStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusLgvArriveHSReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusLgvArriveHSReady] == pStation);
			int k = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);

			pStation->m_strID = CConvert::ToString(pTrack->m_nNumber);		//strID;
			pStation->m_strName = strName;
			m_pDoc->m_pStationInfos.Add(pStation);
			MYTRACE(_T("\n <LGVArriveHS id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}
	if (SelectElements(_T("//LGVDepartHS")))
	{
		nCount = GetSelectCount();
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/LGVDepartHS
			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

			GetAttrValue(_T("cid"), strCID);
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
			pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			//			GetAttrValue(_T("id"), strID);
			//			GetAttrValue(_T("name"), strName);
			//			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			//			DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enLgvStation, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusLgvDepartHSReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusLgvDepartHSReady] == pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);

			pStation->m_strID = CConvert::ToString(pTrack->m_nNumber);		//strID;
			pStation->m_strName = strName;
			m_pDoc->m_pStationInfos.Add(pStation);
			MYTRACE(_T("\n <LGVDepartHS id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}
	if (SelectElements(_T("//ArvStation2")))
	{
		nCount = GetSelectCount();
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	///ECS/Equipment/Tracks/Track/Status/ArvStation
			MoveParent();	///ECS/Equipment/Tracks/Track/Status
			MoveParent();	///ECS/Equipment/Tracks/Track

			GetAttrValue(_T("number"), strValue);

			////////////////////////////////////////////////////////////////

			MoveParent();	///ECS/Equipment/Tracks
			MoveParent();	///ECS/Equipment

			GetAttrValue(_T("cid"), strCID);
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), 0);
			pTrack->m_bReportTrack = TRUE;
			DEBUGER_ASSERT_VALID_LOG(pTrack != NULL, strValue);

			GetAttrValue(_T("device"), strValue);
			pEquipment = m_pDoc->GetEquipment(strValue);
			DEBUGER_ASSERT_VALID_LOG(pEquipment != NULL, strValue);

			////////////////////////////////////////////////////////////////

			MoveSelect(i);
			GetAttrValue(_T("id"), strID);
			GetAttrValue(_T("name"), strName);
			GetAttrValue(_T("st"), strSt);
			GetAttrValue(_T("dest"), strDest);
			DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strID), strID);
			//			DEBUGER_ASSERT_VALID(m_pDoc->GetStationInfo(strID) == NULL);

			pStation = new CStationInfo(CStationInfo::enArvStation2, pEquipment, pTrack);
			DEBUGER_ASSERT_VALID(pStation != NULL);

			j = pStation->m_pTrack->m_mapStatusInfos.GetCount();
			pStation->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusArvSTReady, pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusArvSTReady] == pStation);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack->m_mapStatusInfos.GetCount() == j);

			pStation->m_strID = CConvert::ToString(pTrack->m_nNumber);		//strID;
			pStation->m_strName = strName;
			pStation->m_nSt = _ttoi(strSt);
			pStation->m_nDest = _ttoi(strDest);
			m_pDoc->m_pStationInfos.Add(pStation);
			MYTRACE(_T("\n <ArvStation id='%s' name='%s'/>"), pStation->m_strID, pStation->m_strName);
		}
	}

	if (SelectElements(_T("//RetLimitInfo")))
	{
		nCount = GetSelectCount();
		for (i = 0; i < nCount; ++i)
		{
			MoveSelect(i);	//Track/RetLimitInfo
			MoveParent();

			//			GetAttrValue(_T("tabnumber"), strValue);
			nTabNum = _ttoi(strValue);

			GetAttrValue(_T("number"), strValue);
			pTrack = m_pDoc->GetTrackInfo(_ttoi(strValue), nTabNum);
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
	for (int nSID = nFrom; nSID <= nTo; ++nSID)
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