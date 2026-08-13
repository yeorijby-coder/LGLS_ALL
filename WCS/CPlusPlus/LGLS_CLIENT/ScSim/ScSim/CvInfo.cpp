// CvInfo.cpp: implementation of the CCvInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "CvInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CCvInfo, CInfo, 0)

CCvInfo::CCvInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
	m_wDoorStatus = 0;
	m_wDoorClose = 0;
	m_wDoorOpen = 0;
	m_tagBits.ucBit = 0;
}

CCvInfo::~CCvInfo()
{
}

void CCvInfo::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_pTracks.GetSize();

		CTrackInfo* pTrack = NULL;
		for (int i=0; i<m_pTracks.GetSize(); ++i)
		{
			pTrack = m_pTracks[i];
			DEBUGER_ASSERT_VALID(pTrack != NULL);

			ar << pTrack->m_nNumber;
			ar << pTrack->m_bSuspend;
			ar << pTrack->m_nLuggNum;
			ar << pTrack->m_wErrorCode;
			ar << pTrack->m_tErrorStart << pTrack->m_tErrorEnd;
			ar << pTrack->m_tOperStart << pTrack->m_tOperEnd;
			ar << pTrack->m_tStatusReport;
			ar << pTrack->m_strDisplay;
		}
	}
	else
	{
		int nTrackSize = 0, nTrackNum = 0;

		ar >> nTrackSize;
		if (m_pTracks.GetSize() != nTrackSize)
			return;

		CTrackInfo* pTrack = NULL;
		for (int i=0; i<m_pTracks.GetSize(); ++i)
		{
			pTrack = m_pTracks[i];
			DEBUGER_ASSERT_VALID(pTrack != NULL);

			ar >> nTrackNum;
			if (pTrack->m_nNumber != nTrackNum)
				return;

			ar >> pTrack->m_bSuspend;
			ar >> pTrack->m_nLuggNum;
			ar >> pTrack->m_wErrorCode;
			ar >> pTrack->m_tErrorStart >> pTrack->m_tErrorEnd;
			ar >> pTrack->m_tOperStart >> pTrack->m_tOperEnd;
			ar >> pTrack->m_tStatusReport;
			ar >> pTrack->m_strDisplay;
		}
	}
}

void CCvInfo::SetDoorStatus(WORD wDoorStatus)
{
	if (m_wDoorStatus == wDoorStatus)
		return;

	m_wDoorStatus = wDoorStatus;
	m_bModified = TRUE;
}

void CCvInfo::SetInPermission(WORD wInPermission)
{
	if (w_wInPermission == wInPermission)
		return;

	w_wInPermission = wInPermission;
	m_bModified = TRUE;
}

CString CCvInfo::GetXmlString()
{
	/*
	CString strXml;
	strXml.Format(_T("<INFO><DOOR STS='%d'/>"), m_wDoorStatus);

	if (bSeperateTrack)
		strXml += m_pTracks.GetXmlString();

	strXml += _T("</INFO>");
	return strXml;
	*/
	CString strXml;
	strXml.Format(_T("<INFO><DOOR STS='%d'/>"), m_wDoorStatus);
	strXml += m_pTracks.GetXmlString();
	strXml += _T("</INFO>");
	return strXml;

}

void CCvInfo::InvokeControl()
{
	if (m_bModified == FALSE)
		return;

	CDciStaticCtrl* pDoorCtrl = NULL;
	for (int i=0; i<m_pDoorCtrls.GetSize(); ++i)
	{
		pDoorCtrl = m_pDoorCtrls[i];
		DEBUGER_ASSERT_VALID(pDoorCtrl != NULL);

		pDoorCtrl->m_bVisible = !IsDoorOpen(i);			
		pDoorCtrl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, TRUE);
	}

	m_pTracks.InvokeControl(FALSE);
	
	m_bModified = FALSE;
}
