#include "StdAfx.h"
#include "EQP_ECD_MST.h"
#include "Lang.h"


CEQP_ECD_MST::CEQP_ECD_MST(void)
{
}

CEQP_ECD_MST::CEQP_ECD_MST(CString pstrEQP_TYP, CString pstrERROR_CODE)
{
	EQP_TYP = pstrEQP_TYP;
	ERROR_CODE = pstrERROR_CODE;
	m_MSG.SetSize(4);
	m_ACTION.SetSize(4);
}

void CEQP_ECD_MST::SetMSG_KOR(CString pstrValue)
{
	m_MSG[EN_KOR] = pstrValue;
}
void CEQP_ECD_MST::SetMSG_ENG(CString pstrValue)
{
	m_MSG[EN_ENG] = pstrValue;
}
void CEQP_ECD_MST::SetMSG_CHIN(CString pstrValue)
{
	m_MSG[EN_CHIN] = pstrValue;
}
void CEQP_ECD_MST::SetMSG_HUN(CString pstrValue)
{
	m_MSG[EN_HUN] = pstrValue;
}

void CEQP_ECD_MST::SetACTION_KOR(CString pstrValue)
{
	m_ACTION[EN_KOR] = pstrValue;
}
void CEQP_ECD_MST::SetACTION_ENG(CString pstrValue)
{
	m_ACTION[EN_ENG] = pstrValue;
}
void CEQP_ECD_MST::SetACTION_CHIN(CString pstrValue)
{
	m_ACTION[EN_CHIN] = pstrValue;
}
void CEQP_ECD_MST::SetACTION_HUN(CString pstrValue)
{
	m_ACTION[EN_HUN] = pstrValue;
}


CString CEQP_ECD_MST::GetTupleKeyEQP_ECD_MST()
{
	return CLib::GetTupleKeyEQP_ECD_MST(EQP_TYP, ERROR_CODE);
}

void CEQP_ECD_MST::SetValues_MSG(CString strKOR, CString strENG, CString strCHIN, CString strHUN)
{
	SetMSG_KOR(strKOR);
	SetMSG_ENG(strENG);
	SetMSG_CHIN(strCHIN);
	SetMSG_HUN(strHUN);
}

void CEQP_ECD_MST::SetValues_ACTION(CString strKOR, CString strENG, CString strCHIN, CString strHUN)
{
	SetACTION_KOR(strKOR);
	SetACTION_ENG(strENG);
	SetACTION_CHIN(strCHIN);
	SetACTION_HUN(strHUN);
}


CEQP_ECD_MST::~CEQP_ECD_MST(void)
{
}

