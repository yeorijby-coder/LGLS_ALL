#include "stdafx.h"
#include "RTV_DATA.h"


// RTV_DATA

CRTV_DATA::CRTV_DATA(void)
{
	m_pControl = NULL;
}
CRTV_DATA::CRTV_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrRTV_NO)
{
	m_pControl = NULL;
	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
	K_RTV_NO = pstrRTV_NO;
}

CRTV_DATA::~CRTV_DATA()
{
}

void CRTV_DATA::SetWH_TYP(CString pstrWH_TYP)
{
	K_WH_TYP = pstrWH_TYP;
}

void CRTV_DATA::SetPLC_NO(CString pstrPLC_NO)
{
	K_PLC_NO = pstrPLC_NO;
}

void CRTV_DATA::SetRTV_NO(CString pstrRTV_NO)
{
	K_RTV_NO = pstrRTV_NO;
}

void CRTV_DATA::SetAUTO_MODE_RD(CString pAUTO_MODE_RD)
{
	if(V_AUTO_MODE_RD == pAUTO_MODE_RD)
		return;



	V_AUTO_MODE_RD = pAUTO_MODE_RD;
	m_bModified = true;
}

void CRTV_DATA::SetSENSOR_FK_RD(CString pSENSOR_FK_RD)
{
	if(V_SENSOR_FK_RD == pSENSOR_FK_RD)
		return;



	V_SENSOR_FK_RD = pSENSOR_FK_RD;
	m_bModified = true;
}

void CRTV_DATA::SetUCSTATUS_RD(CString pUCSTATUS_RD)
{
	if(V_UCSTATUS_RD == pUCSTATUS_RD)
		return;



	V_UCSTATUS_RD = pUCSTATUS_RD;
	m_bModified = true;
}

void CRTV_DATA::SetPOS_H_RD(CString pPOS_H_RD)
{
	if(V_POS_H_RD == pPOS_H_RD)
		return;



	V_POS_H_RD = pPOS_H_RD;
	m_bModified = true;
}

void CRTV_DATA::SetPOS_V_RD(CString pPOS_V_RD)
{
	if(V_POS_V_RD == pPOS_V_RD)
		return;



	V_POS_V_RD = pPOS_V_RD;
	m_bModified = true;
}

void CRTV_DATA::SetFORKPOS_FK1_RD(CString pFORKPOS_FK1_RD)
{
	if(V_FORKPOS_FK1_RD == pFORKPOS_FK1_RD)
		return;



	V_FORKPOS_FK1_RD = pFORKPOS_FK1_RD;
	m_bModified = true;
}

void CRTV_DATA::SetFORKPOS_FK2_RD(CString pFORKPOS_FK2_RD)
{
	if(V_FORKPOS_FK2_RD == pFORKPOS_FK2_RD)
		return;



	V_FORKPOS_FK2_RD = pFORKPOS_FK2_RD;
	m_bModified = true;
}

void CRTV_DATA::SetERR_CODE_RD(CString pERR_CODE_RD)
{
	if(V_ERR_CODE_RD == pERR_CODE_RD)
		return;



	V_ERR_CODE_RD = pERR_CODE_RD;
	m_bModified = true;
}

void CRTV_DATA::SetACTIVE_MODE_RD(CString pACTIVE_MODE_RD)
{
	if(V_ACTIVE_MODE_RD == pACTIVE_MODE_RD)
		return;



	V_ACTIVE_MODE_RD = pACTIVE_MODE_RD;
	m_bModified = true;
}

void CRTV_DATA::SetCOMPLETE_RD(CString pCOMPLETE_RD)
{
	if(V_COMPLETE_RD == pCOMPLETE_RD)
		return;



	V_COMPLETE_RD = pCOMPLETE_RD;
	m_bModified = true;
}

void CRTV_DATA::SetJOB_TYP_RD(CString pJOB_TYP_RD)
{
	if(V_JOB_TYP_RD == pJOB_TYP_RD)
		return;



	V_JOB_TYP_RD = pJOB_TYP_RD;
	m_bModified = true;
}

void CRTV_DATA::SetLUGG_NO_FK1_RD(CString pLUGG_NO_FK1_RD)
{
	if(V_LUGG_NO_FK1_RD == pLUGG_NO_FK1_RD)
		return;



	V_LUGG_NO_FK1_RD = pLUGG_NO_FK1_RD;
	m_bModified = true;
}

void CRTV_DATA::SetDEPARTHS_NO_FK1_RD(CString pDEPARTHS_NO_FK1_RD)
{
	if(V_DEPARTHS_NO_FK1_RD == pDEPARTHS_NO_FK1_RD)
		return;



	V_DEPARTHS_NO_FK1_RD = pDEPARTHS_NO_FK1_RD;
	m_bModified = true;
}

void CRTV_DATA::SetARRIVEHS_NO_FK1_RD(CString pARRIVEHS_NO_FK1_RD)
{
	if(V_ARRIVEHS_NO_FK1_RD == pARRIVEHS_NO_FK1_RD)
		return;



	V_ARRIVEHS_NO_FK1_RD = pARRIVEHS_NO_FK1_RD;
	m_bModified = true;
}

void CRTV_DATA::SetUSE_FORK_RD(CString pUSE_FORK_RD)
{
	if(V_USE_FORK_RD == pUSE_FORK_RD)
		return;



	V_USE_FORK_RD = pUSE_FORK_RD;
	m_bModified = true;
}

void CRTV_DATA::SetLUGG_NO_FK2_RD(CString pLUGG_NO_FK2_RD)
{
	if(V_LUGG_NO_FK2_RD == pLUGG_NO_FK2_RD)
		return;



	V_LUGG_NO_FK2_RD = pLUGG_NO_FK2_RD;
	m_bModified = true;
}

void CRTV_DATA::SetDEPARTHS_NO_FK2_RD(CString pDEPARTHS_NO_FK2_RD)
{
	if(V_DEPARTHS_NO_FK2_RD == pDEPARTHS_NO_FK2_RD)
		return;



	V_DEPARTHS_NO_FK2_RD = pDEPARTHS_NO_FK2_RD;
	m_bModified = true;
}

void CRTV_DATA::SetARRIVEHS_NO_FK2_RD(CString pARRIVEHS_NO_FK2_RD)
{
	if(V_ARRIVEHS_NO_FK2_RD == pARRIVEHS_NO_FK2_RD)
		return;



	V_ARRIVEHS_NO_FK2_RD = pARRIVEHS_NO_FK2_RD;
	m_bModified = true;
}

void CRTV_DATA::SetJOB_TYP_OD(CString pJOB_TYP_OD)
{
	if(V_JOB_TYP_OD == pJOB_TYP_OD)
		return;



	V_JOB_TYP_OD = pJOB_TYP_OD;
	m_bModified = true;
}

void CRTV_DATA::SetLUGG_NO_FK1_OD(CString pLUGG_NO_FK1_OD)
{
	if(V_LUGG_NO_FK1_OD == pLUGG_NO_FK1_OD)
		return;



	V_LUGG_NO_FK1_OD = pLUGG_NO_FK1_OD;
	m_bModified = true;
}

void CRTV_DATA::SetDEPARTHS_NO_FK1_OD(CString pDEPARTHS_NO_FK1_OD)
{
	if(V_DEPARTHS_NO_FK1_OD == pDEPARTHS_NO_FK1_OD)
		return;



	V_DEPARTHS_NO_FK1_OD = pDEPARTHS_NO_FK1_OD;
	m_bModified = true;
}

void CRTV_DATA::SetARRIVEHS_NO_FK1_OD(CString pARRIVEHS_NO_FK1_OD)
{
	if(V_ARRIVEHS_NO_FK1_OD == pARRIVEHS_NO_FK1_OD)
		return;



	V_ARRIVEHS_NO_FK1_OD = pARRIVEHS_NO_FK1_OD;
	m_bModified = true;
}

void CRTV_DATA::SetUSE_FK_OD(CString pUSE_FK_OD)
{
	if(V_USE_FK_OD == pUSE_FK_OD)
		return;



	V_USE_FK_OD = pUSE_FK_OD;
	m_bModified = true;
}

void CRTV_DATA::SetLUGG_NO_FK2_OD(CString pLUGG_NO_FK2_OD)
{
	if(V_LUGG_NO_FK2_OD == pLUGG_NO_FK2_OD)
		return;



	V_LUGG_NO_FK2_OD = pLUGG_NO_FK2_OD;
	m_bModified = true;
}

void CRTV_DATA::SetDEPARTHS_NO_FK2_OD(CString pDEPARTHS_NO_FK2_OD)
{
	if(V_DEPARTHS_NO_FK2_OD == pDEPARTHS_NO_FK2_OD)
		return;



	V_DEPARTHS_NO_FK2_OD = pDEPARTHS_NO_FK2_OD;
	m_bModified = true;
}

void CRTV_DATA::SetARRIVEHS_NO_FK2_OD(CString pARRIVEHS_NO_FK2_OD)
{
	if(V_ARRIVEHS_NO_FK2_OD == pARRIVEHS_NO_FK2_OD)
		return;



	V_ARRIVEHS_NO_FK2_OD = pARRIVEHS_NO_FK2_OD;
	m_bModified = true;
}

void CRTV_DATA::SetUSER_COMMAND_OD(CString pUSER_COMMAND_OD)
{
	if(V_USER_COMMAND_OD == pUSER_COMMAND_OD)
		return;



	V_USER_COMMAND_OD = pUSER_COMMAND_OD;
	m_bModified = true;
}

void CRTV_DATA::SetOD_RQ_YN(CString pOD_RQ_YN)
{
	if(V_OD_RQ_YN == pOD_RQ_YN)
		return;



	V_OD_RQ_YN = pOD_RQ_YN;
	m_bModified = true;
}

void CRTV_DATA::SetREAD_UPD_DT(CTime pREAD_UPD_DT)
{
	if(V_READ_UPD_DT == pREAD_UPD_DT)
		return;



	V_READ_UPD_DT = pREAD_UPD_DT;
	m_bModified = true;
}

void CRTV_DATA::SetWRITE_UPD_DT(CTime pWRITE_UPD_DT)
{
	if(V_WRITE_UPD_DT == pWRITE_UPD_DT)
		return;



	V_WRITE_UPD_DT = pWRITE_UPD_DT;
	m_bModified = true;
}

void CRTV_DATA::SetOD_USER_ID(CString pOD_USER_ID)
{
	if(V_OD_USER_ID == pOD_USER_ID)
		return;



	V_OD_USER_ID = pOD_USER_ID;
	m_bModified = true;
}

void CRTV_DATA::SetOD_UPD_DT(CTime pOD_UPD_DT)
{
	if(V_OD_UPD_DT == pOD_UPD_DT)
		return;



	V_OD_UPD_DT = pOD_UPD_DT;
	m_bModified = true;
}

void CRTV_DATA::SetITN_LUGG_FK1(CString pITN_LUGG_FK1)
{
	if(V_ITN_LUGG_FK1 == pITN_LUGG_FK1)
		return;



	V_ITN_LUGG_FK1 = pITN_LUGG_FK1;
	m_bModified = true;
}

void CRTV_DATA::SetITN_LUGG_FK2(CString pITN_LUGG_FK2)
{
	if(V_ITN_LUGG_FK2 == pITN_LUGG_FK2)
		return;



	V_ITN_LUGG_FK2 = pITN_LUGG_FK2;
	m_bModified = true;
}

void CRTV_DATA::SetITN_ARR_HS_MC_NO_FK1(CString pITN_ARR_HS_MC_NO_FK1)
{
	if(V_ITN_ARR_HS_MC_NO_FK1 == pITN_ARR_HS_MC_NO_FK1)
		return;



	V_ITN_ARR_HS_MC_NO_FK1 = pITN_ARR_HS_MC_NO_FK1;
	m_bModified = true;
}

void CRTV_DATA::SetITN_ARR_HS_MC_NO_FK2(CString pITN_ARR_HS_MC_NO_FK2)
{
	if(V_ITN_ARR_HS_MC_NO_FK2 == pITN_ARR_HS_MC_NO_FK2)
		return;



	V_ITN_ARR_HS_MC_NO_FK2 = pITN_ARR_HS_MC_NO_FK2;
	m_bModified = true;
}

void CRTV_DATA::SetCMD_RQ_ID(CString pCMD_RQ_ID)
{
	if(V_CMD_RQ_ID == pCMD_RQ_ID)
		return;



	V_CMD_RQ_ID = pCMD_RQ_ID;
	m_bModified = true;
}

void CRTV_DATA::SetCMD_RQ_YN(CString pCMD_RQ_YN)
{
	if(V_CMD_RQ_YN == pCMD_RQ_YN)
		return;



	V_CMD_RQ_YN = pCMD_RQ_YN;
	m_bModified = true;
}

void CRTV_DATA::SetMES_SEND_YN(CString pMES_SEND_YN)
{
	if(V_MES_SEND_YN == pMES_SEND_YN)
		return;



	V_MES_SEND_YN = pMES_SEND_YN;
	m_bModified = true;
}

void CRTV_DATA::SetMES_ERR_SEND_YN(CString pMES_ERR_SEND_YN)
{
	if(V_MES_ERR_SEND_YN == pMES_ERR_SEND_YN)
		return;



	V_MES_ERR_SEND_YN = pMES_ERR_SEND_YN;
	m_bModified = true;
}

void CRTV_DATA::SetSUSPEND(CString pSUSPEND)
{
	if(V_SUSPEND == pSUSPEND)
		return;



	V_SUSPEND = pSUSPEND;
	m_bModified = true;
}

CString CRTV_DATA::GetCid()
{
	CString strCID;
	strCID.Format(_T("17130%03s"), K_RTV_NO);	// [LGLS] fix: prefix 11130->17130, %05s->%03s to bind layout widget 17130801
	return strCID;
}



