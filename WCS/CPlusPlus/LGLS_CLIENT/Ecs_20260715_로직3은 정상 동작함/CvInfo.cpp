// CvInfo.cpp: implementation of the CCvInfo class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "CvInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CCvInfo
//
IMPLEMENT_SERIAL(CCvInfo, CInfo, 0)

CCvInfo::CCvInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
}

CCvInfo::~CCvInfo()
{
	//m_MapTrackInfo.RemoveAll();
}


// CInfo ¸â¹ö ÇÔ¼ö
//
void CCvInfo::Serialize(CArchive& ar)
{
}

void CCvInfo::InvokeControl()
{
	if (m_bModified == FALSE)
		return;
	
	BOOL bManual = FALSE;

	if (m_bManualModified == TRUE)
		bManual = TRUE;

	m_pTracks.InvokeControl(bManual);
	m_bModified = FALSE;
}
