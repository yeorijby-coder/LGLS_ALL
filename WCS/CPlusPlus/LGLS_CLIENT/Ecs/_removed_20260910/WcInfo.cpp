#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "WcInfo.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBcrInfo
//
IMPLEMENT_SERIAL(CWcInfo, CInfo, 0)

CWcInfo::CWcInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
	m_pControl = NULL;
	m_pWC_DATA = NULL;
	m_bModified = TRUE;
}


CWcInfo::~CWcInfo(void)
{
	if (m_pWC_DATA) {delete m_pWC_DATA;};
}

CWC_DATA* CWcInfo::CreateWC_DATA( CString pstrPLC_NO, CString pstrWC_MC_NO)
{
	CWC_DATA* pWC_DATA = new CWC_DATA(m_pEquipment->m_WH_TYP,  GetStringWC_MC_NO(pstrWC_MC_NO), GetStringWC_MC_NO(pstrWC_MC_NO));
	m_pWC_DATA = pWC_DATA;
	return pWC_DATA;
}

CString CWcInfo::GetStringWC_MC_NO(CString pstrWC_MC_NO)
{
	CString strWC_MC_NO;
	strWC_MC_NO.Format(_T("%03s"), pstrWC_MC_NO);
	return strWC_MC_NO;
}