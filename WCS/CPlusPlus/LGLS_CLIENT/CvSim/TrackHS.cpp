// TrackHS.cpp: implementation of the CTrackHS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "TrackHS.h"
#include "TrackInfo.h"
#include "Equipment.h"
#include "StationInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrackHS::CTrackHS(CEquipment* pEquipment)
{
	m_pEquipment = pEquipment;
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	m_nPos = 0;
	m_nKind = 0;
	m_pTrack = NULL;
	m_bDeadLockTrack = FALSE;
	m_bBufferTrack = FALSE;
	m_nBufferMatchingTrack = 0;
	m_nPair =0;
}

CTrackHS::~CTrackHS()
{

}

void CTrackHS::AddDestStationID(CString& strFrom, CString& strTo)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strFrom), strFrom);
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strTo), strTo);

	CString strStationID;
	int from = _ttoi(strFrom);
	int to = _ttoi(strTo);

	for (int i=from; i<=to; ++i)
	{
		switch(g_nLenStationID)
		{
		case 1:		strStationID.Format(_T("%01d"), i);		break;
		case 2:		strStationID.Format(_T("%02d"), i);		break;
		case 3:		strStationID.Format(_T("%03d"), i);		break;
		case 4:		strStationID.Format(_T("%04d"), i);		break;
		case 5:		strStationID.Format(_T("%05d"), i);		break;
		case 6:		strStationID.Format(_T("%06d"), i);		break;
		case 7:		strStationID.Format(_T("%07d"), i);		break;
		case 8:		strStationID.Format(_T("%08d"), i);		break;
		case 9:		strStationID.Format(_T("%09d"), i);		break;
		default:	strStationID.Format(_T("%d"), i);		break;
		}
		
		m_strDestStations.Add(strStationID);
		DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strStationID), strStationID);
	}
}

BOOL CTrackHS::IsDestStationID(CString& strDestStationID)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strDestStationID), strDestStationID);

	for (int i=0; i<m_strDestStations.GetSize(); ++i)
	{
		if (m_strDestStations[i] == strDestStationID)
			return TRUE;
	}

	return FALSE;
}

CString CTrackHS::ToString()
{
	CString strHS;
	strHS.Format(_T("[%d] TR%04d"), m_nPos, m_pTrack->m_nNumber);
	return strHS;
}

//////////////////////////////////////////////////////////////////////////

void CTrackHSArray::ClearArray()
{
	for (int i=0; i<m_nSize; ++i)
		delete m_pData[i];
	RemoveAll();
}

void CTrackHSArray::SetArraySize(int nSize)
{
	DEBUGER_ASSERT_VALID(nSize > 0);

	SetSize(nSize);
	m_nIndex = 0;
}

CTrackHS* CTrackHSArray::NextTrackHS()
{
	DEBUGER_ASSERT_VALID(m_nSize > 0);
	DEBUGER_ASSERT_RANGE(m_nIndex, m_nSize);
	m_nIndex = (m_nIndex + 1) % m_nSize;
	return m_pData[m_nIndex];
}

CTrackHS* CTrackHSArray::FirstTrackHS()
{
	DEBUGER_ASSERT_VALID(m_nSize > 0);
	m_nIndex = 0;
	return m_pData[m_nIndex];
}

CTrackHS* CTrackHSArray::LastTrackHS()
{
	DEBUGER_ASSERT_VALID(m_nSize > 0);
	m_nIndex = m_nSize - 1;
	return m_pData[m_nIndex];
}

CTrackHS* CTrackHSArray::CurrentTrackHS()
{
	DEBUGER_ASSERT_VALID(m_nSize > 0);
	DEBUGER_ASSERT_RANGE(m_nIndex, m_nSize);
	return m_pData[m_nIndex];
}

CTrackHS* CTrackHSArray::GetNextTrackHS()
{
	DEBUGER_ASSERT_VALID(m_nSize > 0);
	DEBUGER_ASSERT_RANGE(m_nIndex, m_nSize);
	int nIndex = m_nIndex;
	m_nIndex = (m_nIndex + 1) % m_nSize;
	return m_pData[nIndex];
}

CTrackHS* CTrackHSArray::FindTrackHS(CString& strDestStationID)
{
	CTrackHS* pTrackHS = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrackHS = m_pData[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);

		if (pTrackHS->IsDestStationID(strDestStationID))
			return pTrackHS;
	}

	return NULL;
}

CTrackHS* CTrackHSArray::FindTrackHS(int nDepartPos, CString& strDestStationID)
{
	CTrackHS* pTrackHS = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrackHS = m_pData[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);

		if (pTrackHS->m_nPos == nDepartPos)
			continue;

		if (pTrackHS->IsDestStationID(strDestStationID))
			return pTrackHS;
	}

	return NULL;
}

CTrackHS* CTrackHSArray::FindByTrack(int nTrackNum)
{
	CTrackHS* pTrackHS = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrackHS = m_pData[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);
		DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);

		if (pTrackHS->m_pTrack->m_nNumber == nTrackNum)
			return pTrackHS;
	}

	return NULL;
}

CTrackHS* CTrackHSArray::FindByPos(int nPos)
{
	CTrackHS* pTrackHS = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pTrackHS = m_pData[i];
		DEBUGER_ASSERT_VALID(pTrackHS != NULL);

		if (pTrackHS->m_nPos == nPos)
			return pTrackHS;
	}

	return NULL;
}
