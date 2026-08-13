// Info.cpp: implementation of the CInfo class.
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Equipment.h"
#include "Info.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CInfo
//
IMPLEMENT_SERIAL(CInfo, CObject, 0)

CInfo::CInfo(CEquipment* pEquipment)
{
	m_pEquipment = pEquipment;
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);
	m_bModified = TRUE;
}

CInfo::CInfo(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrEQP_NO)
{
	m_strWH_TYP = pstrWH_TYP;
	m_nNumber = CConvert::ToInt(pstrEQP_NO);
	m_nPlcNo = CConvert::ToInt(pstrPLC_NO);
}

CInfo::CInfo(CString pstrWH_TYP, CString pstrPLC_NO)
{
	m_strWH_TYP = pstrWH_TYP;
	m_nPlcNo = CConvert::ToInt(pstrPLC_NO);
}

CInfo::~CInfo()
{
}
