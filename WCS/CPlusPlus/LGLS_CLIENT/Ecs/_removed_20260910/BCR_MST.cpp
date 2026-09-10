
#include "stdafx.h"
#include "BCR_MST.h"


CBCR_MST::CBCR_MST(void)
{
	m_pControl = NULL;
}


CBCR_MST::CBCR_MST(CString pstrWH_TYP, CString pstrBCR_MC_NO, CString pstrBCR_NO)
{
	m_pControl = NULL;
	K_WH_TYP = pstrWH_TYP;
	K_BCR_NO = pstrBCR_NO;
	m_strBCR_MC_NO = pstrBCR_MC_NO;
}

CBCR_MST::~CBCR_MST(void)
{
}

void CBCR_MST::SetWH_TYP(CString pstrWH_TYP)
{
	K_WH_TYP = pstrWH_TYP;
}

void CBCR_MST::SetBCR_NO(CString pstrBCR_NO)
{
	K_BCR_NO = pstrBCR_NO;
}

void CBCR_MST::SetBCR_MC_NO(CString pBCR_MC_NO)
{
	if(V_BCR_MC_NO == pBCR_MC_NO)
		return;



	V_BCR_MC_NO = pBCR_MC_NO;
	m_bModified = true;
}

void CBCR_MST::SetUPD_DT(CString pUPD_DT)
{
	if(V_UPD_DT == pUPD_DT)
		return;



	V_UPD_DT = pUPD_DT;
	m_bModified = true;
}

void CBCR_MST::SetUPD_USER_ID(CString pUPD_USER_ID)
{
	if(V_UPD_USER_ID == pUPD_USER_ID)
		return;



	V_UPD_USER_ID = pUPD_USER_ID;
	m_bModified = true;
}

void CBCR_MST::SetUSE_YN(CString pUSE_YN)
{
	if(V_USE_YN == pUSE_YN)
		return;



	V_USE_YN = pUSE_YN;
	m_bModified = true;
}


void CBCR_MST::SetPLC_IP(CString pPLC_IP)
{
	if(V_PLC_IP == pPLC_IP)
		return;



	V_PLC_IP = pPLC_IP;
	m_bModified = true;
}


void CBCR_MST::SetPLC_PORT(CString pPLC_PORT)
{
	if(V_PLC_PORT == pPLC_PORT)
		return;



	V_PLC_PORT = pPLC_PORT;
	m_bModified = true;
}

void CBCR_MST::SetSOCK_TIME_OUT(CString pSOCK_TIME_OUT)
{
	if(V_SOCK_TIMEOUT == pSOCK_TIME_OUT)
		return;



	V_SOCK_TIMEOUT = pSOCK_TIME_OUT;
	m_bModified = true;
}

void CBCR_MST::SetRETRY_YN(CString pRETRY_YN)
{
	if(V_RETRY_YN == pRETRY_YN)
		return;



	V_RETRY_YN = pRETRY_YN;
	m_bModified = true;
}


void CBCR_MST::SetCONNECTED_YN(CString pCONNECTED_YN)
{
	if(V_CONNECTED_YN == pCONNECTED_YN)
		return;



	V_CONNECTED_YN = pCONNECTED_YN;
	m_bModified = true;
}


void CBCR_MST::SetBCR_STA(CString pBCR_STA)
{
	if(V_BCR_STA == pBCR_STA)
		return;



	V_BCR_STA = pBCR_STA;
	m_bModified = true;
}


void CBCR_MST::SetBCR_LEV(CString pBCR_LEV)
{
	if(V_BCR_LEV == pBCR_LEV)
		return;



	V_BCR_LEV = pBCR_LEV;
	m_bModified = true;
}


void CBCR_MST::SetNOREAD_CNT(CString pNOREAD_CNT)
{
	if(V_NOREAD_CNT == pNOREAD_CNT)
		return;



	V_NOREAD_CNT = pNOREAD_CNT;
	m_bModified = true;
}


void CBCR_MST::SetCMD_RQ_ID(CString pCMD_RQ_ID)
{
	if(V_CMD_RQ_ID == pCMD_RQ_ID)
		return;



	V_CMD_RQ_ID = pCMD_RQ_ID;
	m_bModified = true;
}

void CBCR_MST::SetCMD_RQ_YN(CString pCMD_RQ_YN)
{
	if(V_CMD_RQ_YN == pCMD_RQ_YN)
		return;



	V_CMD_RQ_YN = pCMD_RQ_YN;
	m_bModified = true;
}

void CBCR_MST::SetSUSPEND(CString pSUSPEND)
{
	if(V_SUSPEND == pSUSPEND)
		return;



	V_SUSPEND = pSUSPEND;
	m_bModified = true;
}

CString CBCR_MST::GetCid()
{
	CString strCID;
	strCID.Format(_T("11170%05s"), K_BCR_NO);
	return strCID;
}

CString CBCR_MST::GetCid(CString nChar4)
{
	CString strCID;
	strCID.Format(_T("1117%01s%05s"), nChar4, K_BCR_NO);
	return strCID;
}
