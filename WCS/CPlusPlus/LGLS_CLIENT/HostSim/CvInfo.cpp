// CvInfo.cpp: implementation of the CCvInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "CvInfo.h"
#include "EcsDoc.h"

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
	m_wDoorOpenReq = 0;
	m_wDoorOpenAccept = 0;
	m_wRgvStatus = 0;
	m_wPrevLampStatus = 0;
	m_wCurrLampStatus = 0;
	m_bFirstDoorCheck = TRUE;
	m_wRgvcErrorStatus = 0;
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
//	if (m_wDoorStatus == wDoorStatus)		// 항상 에러상태를 체크해서 경광등을 올려야 하므로...
//		return;

	// 에러가 처음 발생했을 경우
	if ((m_wDoorStatus == 0) && (wDoorStatus != 0))
	{
		m_wDoorStatus = wDoorStatus;
		m_bModified = TRUE;
		SetLampInfoDoor(TRUE);
	}

	// 에러상태에서 다른 에러가 발생했을 경우
//	else 
	if ((m_wDoorStatus != 0) && (wDoorStatus != 0) && (m_wDoorStatus != wDoorStatus)) 
	{
		m_wDoorStatus = wDoorStatus;
		m_bModified = TRUE;
	}

	// 정상으로 돌아왔을경우 
//	else 
	if ((m_wDoorStatus != 0) && (wDoorStatus == 0))
	{
		SetLampInfoDoor(FALSE);
	}

	// 프로그램이 처음 실행 되었을 때 
	if (m_bFirstDoorCheck == TRUE)
	{
		SetLampInfoDoor(BOOL(wDoorStatus));
		m_bFirstDoorCheck = FALSE;
	}

	if (m_wDoorStatus == wDoorStatus)		
		return;

	m_wDoorStatus = wDoorStatus;
	m_bModified = TRUE;
}

//=====================================================================================================
//	// 경광등 관련
//=====================================================================================================	
void CCvInfo::SetLampInfoDoor(BOOL bOn)
{
	CCvFx* pCv = (CCvFx*)m_pEquipment->m_pDoc->GetEquipment(CEquipment::enCV, 12);	// 12번이 경광등 임
	if (pCv == NULL)
		return;

	int nIndex = 0;
	switch(m_pEquipment->m_nNumber) 
	{
	case 1:
	case 2:	
	case 3:
	case 4:
	case 5:		nIndex = 8;		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:	nIndex = 9;		break;
	}

	CLib::SetBit(pCv->m_pInfo->m_wCurrLampStatus, nIndex, bOn);
}
//-----------------------------------------------------------------------------------------------------
//=====================================================================================================
//	// 경광등 관련
//=====================================================================================================	
void CCvInfo::SetLampInfo(BOOL bOn)
{
	CCvFx* pCv = (CCvFx*)m_pEquipment->m_pDoc->GetEquipment(CEquipment::enCV, 12);	// 12번이 경광등 임
	if (pCv == NULL)
		return;

	int nIndex = 0;
	switch(m_pEquipment->m_nNumber) 
	{
	case 1:
	case 2:		nIndex = 3;		break;
	case 3:
	case 4:
	case 5:		nIndex = 4;		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:	nIndex = 5;		break;
	}

	if (m_pEquipment->m_nNumber != 12)
		CLib::SetBit(pCv->m_pInfo->m_wCurrLampStatus, nIndex, bOn);
}
//-----------------------------------------------------------------------------------------------------

void CCvInfo::SetDoorOpenReq(WORD wDoorOpenReq)
{
	if (m_wDoorOpenReq == wDoorOpenReq)
		return;

	m_wDoorOpenReq = wDoorOpenReq;
	m_bModified = TRUE;

	// RGV 객체를 가져와야 함!
	int nRgvcNum=0;
	switch(m_pEquipment->m_nNumber)
	{
	case 4:		nRgvcNum = 1;	break;
	case 10:	nRgvcNum = 2;	break;
	default:					return;
	}

	CEquipment* pEquipment = m_pEquipment->m_pDoc->GetEquipment(CEquipment::enRGV, nRgvcNum);
	if (pEquipment == NULL)
		return;

	pEquipment->RefreshDialog(1);
}

void CCvInfo::SetDoorOpenAccept(WORD wDoorOpenAccept)
{
	if (m_wDoorOpenAccept == wDoorOpenAccept)
		return;

	m_wDoorOpenAccept = wDoorOpenAccept;
	m_bModified = TRUE;

	// RGV 객체를 가져와야 함!
	int nRgvcNum=0;
	switch(m_pEquipment->m_nNumber)
	{
	case 4:		nRgvcNum = 1;	break;
	case 10:	nRgvcNum = 2;	break;
	default:					return;
	}

	CEquipment* pEquipment = m_pEquipment->m_pDoc->GetEquipment(CEquipment::enRGV, nRgvcNum);
	if (pEquipment == NULL)
		return;

	pEquipment->RefreshDialog(1);
}

void CCvInfo::SetDoorClose(WORD wDoorClose)
{
	if (m_wDoorClose == wDoorClose)
		return;

	m_wDoorClose = wDoorClose;
	m_bModified = TRUE;


	// RGV 객체를 가져와야 함!
	int nRgvcNum=0;
	switch(m_pEquipment->m_nNumber)
	{
	case 4:		nRgvcNum = 1;	break;
	case 10:	nRgvcNum = 2;	break;
	default:					return;
	}

	CEquipment* pEquipment = m_pEquipment->m_pDoc->GetEquipment(CEquipment::enRGV, nRgvcNum);
	if (pEquipment == NULL)
		return;

	pEquipment->RefreshDialog(1);
}

void CCvInfo::SetRgvStatus(WORD wRgvStatus)
{
	if (m_wRgvStatus == wRgvStatus)
		return;

	m_wRgvStatus = wRgvStatus;
	m_bModified = TRUE;
}

CString CCvInfo::GetXmlString()
{
	CString strXml;
	strXml.Format(_T("<INFO><DOOR STS='%d' REQ='%d'/>"), m_wDoorStatus, m_wDoorOpenReq);
	strXml += m_pTracks.GetXmlString();
	strXml += _T("</INFO>");
	return strXml;
}

void CCvInfo::InvokeControl()
{
	if (m_bModified == FALSE)
		return;

	int i = 0;
	CDciStaticCtrl* pDoorCtrl = NULL;
	int nDoorCtrlCnt = m_pDoorCtrls.GetSize();
	for (i=0; i<nDoorCtrlCnt; ++i)
	{
		pDoorCtrl = m_pDoorCtrls[i];
		DEBUGER_ASSERT_VALID(pDoorCtrl != NULL);

		pDoorCtrl->m_bVisible = !IsDoorOpen(0);			
		pDoorCtrl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, TRUE);
	}

	CDciStaticCtrl* pLampCtrl = NULL;
	int nLampCtrlCnt = m_pLampCtrls.GetSize();
	for (i=0; i<nLampCtrlCnt; ++i)
	{
		pLampCtrl = m_pLampCtrls[i];
		DEBUGER_ASSERT_VALID(pLampCtrl != NULL);

		pLampCtrl->m_clrBgColor = RED;	
		pLampCtrl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, TRUE);
	}
	m_pTracks.InvokeControl(FALSE);
	m_pEquipment->MulticastInfo(NULL);

	m_bModified = FALSE;
}
