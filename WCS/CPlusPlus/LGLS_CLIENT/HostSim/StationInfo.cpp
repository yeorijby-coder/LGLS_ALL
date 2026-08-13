// StationInfo.cpp: implementation of the CStationInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "StationInfo.h"
#include "Equipment.h"
#include "TrackInfo.h"
#include "ScInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CStationInfo, CObject)

CStationInfo::CStationInfo(EN_KIND enKind, CEquipment* pEquipment, CTrackInfo* pTrack, int nEquipNum)
{
	m_enKind = enKind;
	m_pEquipment = pEquipment;
	m_pTrack = pTrack;

	if (nEquipNum != 0)
	{
		m_nEquipNum = nEquipNum;
	}

	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);
}

CStationInfo::~CStationInfo()
{

}

BOOL CStationInfo::IsValidID(CString& strStationID)
{
	if (strStationID.GetLength() == 1 || strStationID.GetLength() == enLengthID)
		return TRUE;

	return FALSE;
//	return (strStationID.GetLength() == enLengthID);
}

int CStationInfo::GetWarehouseNo(CString& strStationID)
{
//	return (strStationID.GetLength() == 5) ? _ttoi(strStationID.Mid(1,1)) : 0;
	return 1;
}

int CStationInfo::GetTrackDestination()
{
	if (m_pTrack == NULL)
	{
		DEBUGER_ASSERT_VALID(m_pEquipment != NULL);
		DEBUGER_ASSERT_VALID(m_pEquipment->m_enKind == CEquipment::enSC);
		DEBUGER_ASSERT_VALID(m_strID.GetLength() == enLengthID);

		int nTemp = (_ttoi(m_strID.Right(4)) - 1000);

		if (m_strID.GetLength() == 3)
			nTemp = _ttoi(m_strID);			

		return nTemp;
	}

	return m_pTrack->m_nNumber;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CStationInfoArray::IsValidStartStation(int nJobPattern, CString& strStationID)
{
	if (CStationInfo::IsValidID(strStationID) == FALSE)
		return FALSE;

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		switch (nJobPattern)
		{
		case enJobPatternSto:
		case enJobPatternMove:
			if ((pStation->m_enKind != CStationInfo::enStoStation) &&
				(pStation->m_enKind != CStationInfo::enArvStation))
				continue;
			break;

		case enJobPatternRet:
		case enJobPatternPR:
		case enJobPatternR2R:
		case enJobPatternW2W:
			if (pStation->m_enKind != CStationInfo::enScStation)
				continue;
			break;

		default:
			DEBUGER_ASSERT_VALID_LOG(FALSE, CConvert::ToString(nJobPattern));
			continue;
		}

		if (pStation->m_strID == strStationID)
			return TRUE;
	}

	return FALSE;
}

BOOL CStationInfoArray::IsValidDestStation(int nJobPattern, CString& strStationID)
{
	if (CStationInfo::IsValidID(strStationID) == FALSE)
		return FALSE;

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		switch (nJobPattern)
		{
		case enJobPatternSto:
		case enJobPatternW2W:
		case enJobPatternR2R:
			if (pStation->m_enKind != CStationInfo::enScStation)
				continue;
			break;

		case enJobPatternRet:
		case enJobPatternPR:
			if ((pStation->m_enKind != CStationInfo::enArvStation) && 
				(pStation->m_enKind != CStationInfo::enRetStation))
				continue;
			break;

		case enJobPatternMove:
			if ((pStation->m_enKind != CStationInfo::enArvStation) && 
				(pStation->m_enKind != CStationInfo::enRetStation))
				continue;
			break;
	
		default:
			DEBUGER_ASSERT_VALID_LOG(FALSE, CConvert::ToString(nJobPattern));
			continue;
		}

		if (pStation->m_strID == strStationID)
			return TRUE;
	}

	return FALSE;
}

CStationInfo* CStationInfoArray::GetStationInfo(CString& strStationID)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strStationID), strStationID);

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		if (pStation->m_strID == strStationID)
			return pStation;
	}

	return NULL;
}

CStationInfo* CStationInfoArray::GetScStationInfo(int nWarehouse, CString& strLocation)
{
	DEBUGER_ASSERT_VALID_LOG(CLib::IsValidLocation(nWarehouse, strLocation), strLocation);

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
//		DEBUGER_ASSERT_VALID(pStation != NULL);
//		DEBUGER_ASSERT_VALID(pStation->m_pEquipment != NULL);

		if (pStation == NULL)
			continue;

		if (pStation->m_pEquipment == NULL)
			continue;

		if (pStation->m_enKind != CStationInfo::enScStation)
			continue;

		if (pStation->GetWarehouseNo() != nWarehouse)
			continue;

		if (pStation->m_pEquipment->m_enKind != CEquipment::enSC)
			continue;
//		DEBUGER_ASSERT_VALID(pStation->m_pEquipment->m_enKind == CEquipment::enSC);

//		CScInfo *pInfo  = (CScInfo *)pStation->m_pEquipment->GetInfo();
		BOOL bDouble = FALSE;		// (pInfo->m_nForkType == CScInfo::enDouble) ? TRUE : FALSE;

		if (pStation->m_nEquipNum == CLib::GetStackerNum(nWarehouse, strLocation, bDouble))
			return pStation;
	}

	return NULL;
}


CString CStationInfoArray::GetStationName(CString& strStationID)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strStationID), strStationID);

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		if (pStation->m_strID == strStationID)
			return pStation->m_strName;
	}

	return _T("");
}