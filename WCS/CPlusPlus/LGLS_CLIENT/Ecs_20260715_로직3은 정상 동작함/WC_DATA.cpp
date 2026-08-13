#include "StdAfx.h"
#include "WC_DATA.h"


CWC_DATA::CWC_DATA(void)
{
}

CWC_DATA::CWC_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrWC_MC_NO)
{
	m_pControl = NULL;
	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
	K_WC_MC_NO = pstrWC_MC_NO;
}

CWC_DATA::~CWC_DATA(void)
{
}

void CWC_DATA::SetWH_TYP(CString pstrWH_TYP)
{
	K_WH_TYP = pstrWH_TYP;
}

void CWC_DATA::SetPLC_NO(CString pstrPLC_NO)
{
	K_PLC_NO = pstrPLC_NO;
}

void CWC_DATA::SetWC_NO(CString pstrWC_NO)
{
	K_WC_NO = pstrWC_NO;
}

void CWC_DATA::SetWC_MC_NO(CString pstrWC_MC_NO)
{
	K_WC_MC_NO = pstrWC_MC_NO;
}

void CWC_DATA::SetWEIGHT_READ_STA(CString pstrWEIGHT_READ_STA)
{
	if(V_WEIGHT_READ_STA == pstrWEIGHT_READ_STA)
		return;
	
	V_WEIGHT_READ_STA = pstrWEIGHT_READ_STA;
	m_bModified = true;
}

void CWC_DATA::SetWEIGHT_READ_STA_NM(CString pstrWEIGHT_READ_STA_NM)
{
	if(V_WEIGHT_READ_STA_NM == pstrWEIGHT_READ_STA_NM)
		return;
	
	V_WEIGHT_READ_STA_NM = pstrWEIGHT_READ_STA_NM;
	m_bModified = true;
}

void CWC_DATA::SetWEIGHT_RCV_VAL(CString pstrWEIGHT_RCV_VAL)
{
	if(V_WEIGHT_RCV_VAL == pstrWEIGHT_RCV_VAL)
		return;

	V_WEIGHT_RCV_VAL = pstrWEIGHT_RCV_VAL;
	m_bModified = true;
}

void CWC_DATA::SetSUSPEND(CString pstrSUSPEND)
{
	if(V_SUSPEND == pstrSUSPEND)
		return;

	V_SUSPEND = pstrSUSPEND;
	m_bModified = true;
}

void CWC_DATA::SetSUSPEND_NM(CString pstrSUSPEND_NM)
{
	if(V_SUSPEND_NM == pstrSUSPEND_NM)
		return;

	V_SUSPEND_NM = pstrSUSPEND_NM;
	m_bModified = true;
}

void CWC_DATA::SetCMD_RQ_ID(CString pstrCMD_RQ_ID)
{
	if(V_CMD_RQ_ID == pstrCMD_RQ_ID)
		return;

	V_CMD_RQ_ID = pstrCMD_RQ_ID;
	m_bModified = true;
}

void CWC_DATA::SetCMD_RQ_YN(CString pstrCMD_RQ_YN)
{
	if(V_CMD_RQ_YN == pstrCMD_RQ_YN)
		return;

	V_CMD_RQ_YN = pstrCMD_RQ_YN;
	m_bModified = true;
}

void CWC_DATA::SetEQP_TIME(CString pstrEQP_TIME)
{
	if(V_EQP_TIME == pstrEQP_TIME)
		return;

	V_EQP_TIME = pstrEQP_TIME;
	m_bModified = true;
}

void CWC_DATA::SetEQP_COLOR(CString pstrEQP_COLOR)
{
	if(V_EQP_COLOR == pstrEQP_COLOR)
		return;

	V_EQP_COLOR = pstrEQP_COLOR;
	m_bModified = true;
}

void CWC_DATA::SetEQP_CONNECTED_YN(CString pstrEQP_CONNECTED_YN)
{
	if(V_EQP_CONNECTED_YN == pstrEQP_CONNECTED_YN)
		return;

	V_EQP_CONNECTED_YN = pstrEQP_CONNECTED_YN;
	m_bModified = true;
}

void CWC_DATA::SetCHK_BYPASS_YN(CString pstrCHK_BYPASS_YN)
{
	if(V_CHK_BYPASS_YN == pstrCHK_BYPASS_YN)
		return;

	V_CHK_BYPASS_YN = pstrCHK_BYPASS_YN;
	m_bModified = true;
}

void CWC_DATA::SetCHK_BYPASS_YN_NM(CString pstrCHK_BYPASS_YN_NM)
{
	if(V_CHK_BYPASS_YN_NM == pstrCHK_BYPASS_YN_NM)
		return;

	V_CHK_BYPASS_YN_NM = pstrCHK_BYPASS_YN_NM;
	m_bModified = true;
}

CString CWC_DATA::GetCid()
{
	CString strCID;
	strCID.Format(_T("11171%03s"), K_WC_MC_NO);
	return strCID;
}