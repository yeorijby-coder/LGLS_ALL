#include "stdafx.h"
#include "CV_DATA.h"


// CCV_DATA

CCV_DATA::CCV_DATA(void)
{
}

CCV_DATA::CCV_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrTRACK_NO) : CInfo(pstrWH_TYP, pstrPLC_NO, pstrTRACK_NO)
{
	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
	K_TRACK_NO = pstrTRACK_NO;
}

CCV_DATA::CCV_DATA(CString pstrWH_TYP, CString pstrPLC_NO) : CInfo(pstrWH_TYP, pstrPLC_NO)
{
	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
}


CCV_DATA::~CCV_DATA(void)
{
}

void CCV_DATA::SetWH_TYP(CString pstrWH_TYP)
{
	K_WH_TYP = pstrWH_TYP;
}

void CCV_DATA::SetPLC_NO(CString pstrPLC_NO)
{
	K_PLC_NO = pstrPLC_NO;
}

void CCV_DATA::SetTRACK_NO(CString pstrTRACK_NO)
{
	K_TRACK_NO = pstrTRACK_NO;
}


void CCV_DATA::SetLUGG_NO_RD(CString pLUGG_NO_RD)
{
	if(V_LUGG_NO_RD == pLUGG_NO_RD)
		return;



	V_LUGG_NO_RD = pLUGG_NO_RD;
	m_bModified = true;
}

void CCV_DATA::SetLUGG_NO_OD(CString pLUGG_NO_OD)
{
	if(V_LUGG_NO_OD == pLUGG_NO_OD)
		return;



	V_LUGG_NO_OD = pLUGG_NO_OD;
	m_bModified = true;
}

void CCV_DATA::SetDEST_POS_RD(CString pDEST_POS_RD)
{
	if(V_DEST_POS_RD == pDEST_POS_RD)
		return;



	V_DEST_POS_RD = pDEST_POS_RD;
	m_bModified = true;
}

void CCV_DATA::SetDEST_POS_OD(CString pDEST_POS_OD)
{
	if(V_DEST_POS_OD == pDEST_POS_OD)
		return;



	V_DEST_POS_OD = pDEST_POS_OD;
	m_bModified = true;
}

void CCV_DATA::SetJOB_TYP_RD(CString pJOB_TYP_RD)
{
	if(V_JOB_TYP_RD == pJOB_TYP_RD)
		return;



	V_JOB_TYP_RD = pJOB_TYP_RD;
	m_bModified = true;
}

void CCV_DATA::SetJOB_TYP_OD(CString pJOB_TYP_OD)
{
	if(V_JOB_TYP_OD == pJOB_TYP_OD)
		return;



	V_JOB_TYP_OD = pJOB_TYP_OD;
	m_bModified = true;
}

void CCV_DATA::SetPULP_SENSOR_RD(CString pPULP_SENSOR_RD)
{
	if(V_PULP_SENSOR_RD == pPULP_SENSOR_RD)
		return;



	V_PULP_SENSOR_RD = pPULP_SENSOR_RD;
	m_bModified = true;
}

void CCV_DATA::SetPULP_SENSOR_OD(CString pPULP_SENSOR_OD)
{
	if(V_PULP_SENSOR_OD == pPULP_SENSOR_OD)
		return;



	V_PULP_SENSOR_OD = pPULP_SENSOR_OD;
	m_bModified = true;
}

void CCV_DATA::SetTRAY_LEV_RD(CString pTRAY_LEV_RD)
{
	if(V_TRAY_LEV_RD == pTRAY_LEV_RD)
		return;



	V_TRAY_LEV_RD = pTRAY_LEV_RD;
	m_bModified = true;
}

void CCV_DATA::SetMES_ERROR_CD(CString pMES_ERROR_CD)
{
	if(V_MES_ERROR_CD== pMES_ERROR_CD)
		return;



	V_MES_ERROR_CD = pMES_ERROR_CD;
	m_bModified = true;
}

void CCV_DATA::SetTRAY_LEV_OD(CString pTRAY_LEV_OD)
{
	if(V_TRAY_LEV_OD == pTRAY_LEV_OD)
		return;



	V_TRAY_LEV_OD = pTRAY_LEV_OD;
	m_bModified = true;
}

void CCV_DATA::SetTRAY_TYP_RD(CString pTRAY_TYP_RD)
{
	if(V_TRAY_TYP_RD == pTRAY_TYP_RD)
		return;



	V_TRAY_TYP_RD = pTRAY_TYP_RD;
	m_bModified = true;
}

void CCV_DATA::SetTRAY_TYP_OD(CString pTRAY_TYP_OD)
{
	if(V_TRAY_TYP_OD == pTRAY_TYP_OD)
		return;



	V_TRAY_TYP_OD = pTRAY_TYP_OD;
	m_bModified = true;
}


void CCV_DATA::SetERR_RQ_RD(CString pERR_RQ_RD)
{
	if(V_ERR_RQ_RD == pERR_RQ_RD)
		return;



	V_ERR_RQ_RD = pERR_RQ_RD;
	m_bModified = true;
}

void CCV_DATA::SetERR_RQ_OD(CString pERR_RQ_OD)
{
	if(V_ERR_RQ_OD == pERR_RQ_OD)
		return;



	V_ERR_RQ_OD = pERR_RQ_OD;
	m_bModified = true;
}

void CCV_DATA::SetAUTO_MODE_RD(CString pAUTO_MODE_RD)
{
	if(V_AUTO_MODE_RD == pAUTO_MODE_RD)
		return;



	V_AUTO_MODE_RD = pAUTO_MODE_RD;
	m_bModified = true;
}

void CCV_DATA::SetSTO_READY_RD(CString pSTO_READY_RD)
{
	if(V_STO_READY_RD == pSTO_READY_RD)
		return;



	V_STO_READY_RD = pSTO_READY_RD;
	m_bModified = true;
}


void CCV_DATA::SetRET_READY_RD(CString pRET_READY_RD)
{
	if(V_RET_READY_RD == pRET_READY_RD)
		return;



	V_RET_READY_RD = pRET_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetSTOHS_READY_RD(CString pSTOHS_READY_RD)
{
	if(V_STOHS_READY_RD == pSTOHS_READY_RD)
		return;



	V_STOHS_READY_RD = pSTOHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetRETHS_READY_RD(CString pRETHS_READY_RD)
{
	if(V_RETHS_READY_RD == pRETHS_READY_RD)
		return;



	V_RETHS_READY_RD = pRETHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetRTV_DEPARTHS_READY_RD(CString pRTV_DEPARTHS_READY_RD)
{
	if(V_RTV_DEPARTHS_READY_RD == pRTV_DEPARTHS_READY_RD)
		return;



	V_RTV_DEPARTHS_READY_RD = pRTV_DEPARTHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetRTV_ARRIVEHS_READY_RD(CString pRTV_ARRIVEHS_READY_RD)
{
	if(V_RTV_ARRIVEHS_READY_RD == pRTV_ARRIVEHS_READY_RD)
		return;



	V_RTV_ARRIVEHS_READY_RD = pRTV_ARRIVEHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetLGV_DEPARTHS_READY_RD(CString pLGV_DEPARTHS_READY_RD)
{
	if(V_LGV_DEPARTHS_READY_RD == pLGV_DEPARTHS_READY_RD)
		return;



	V_LGV_DEPARTHS_READY_RD = pLGV_DEPARTHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetLGV_ARRIVEHS_READY_RD(CString pLGV_ARRIVEHS_READY_RD)
{
	if(V_LGV_ARRIVEHS_READY_RD == pLGV_ARRIVEHS_READY_RD)
		return;



	V_LGV_ARRIVEHS_READY_RD = pLGV_ARRIVEHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetSENSOR0_DATA_RD(CString pSENSOR0_DATA_RD)
{
	if(V_SENSOR0_DATA_RD == pSENSOR0_DATA_RD)
		return;



	V_SENSOR0_DATA_RD = pSENSOR0_DATA_RD;
	m_bModified = true;
}

void CCV_DATA::SetSENSOR1_DATA_RD(CString pSENSOR1_DATA_RD)
{
	if(V_SENSOR1_DATA_RD == pSENSOR1_DATA_RD)
		return;



	V_SENSOR1_DATA_RD = pSENSOR1_DATA_RD;
	m_bModified = true;
}

void CCV_DATA::SetSENSOR2_DATA_RD(CString pSENSOR2_DATA_RD)
{
	if(V_SENSOR2_DATA_RD == pSENSOR2_DATA_RD)
		return;



	V_SENSOR2_DATA_RD = pSENSOR2_DATA_RD;
	m_bModified = true;
}

void CCV_DATA::SetSENSOR3_DATA_RD(CString pSENSOR3_DATA_RD)
{
	if(V_SENSOR3_DATA_RD == pSENSOR3_DATA_RD)
		return;



	V_SENSOR3_DATA_RD = pSENSOR3_DATA_RD;
	m_bModified = true;
}

void CCV_DATA::SetERROR_CODE(CString pERROR_CODE)
{
	if(V_ERROR_CODE == pERROR_CODE)
		return;



	V_ERROR_CODE = pERROR_CODE;
	m_bModified = true;
}

void CCV_DATA::SetOD_RQ_YN(CString pOD_RQ_YN)
{
	if(V_OD_RQ_YN == pOD_RQ_YN)
		return;



	V_OD_RQ_YN = pOD_RQ_YN;
	m_bModified = true;
}

void CCV_DATA::SetREAD_UPD_DT(CTime pREAD_UPD_DT)
{
	if(V_READ_UPD_DT == pREAD_UPD_DT)
		return;



	V_READ_UPD_DT = pREAD_UPD_DT;
	m_bModified = true;
}

void CCV_DATA::SetWRITE_UPD_DT(CTime pWRITE_UPD_DT)
{
	if(V_WRITE_UPD_DT == pWRITE_UPD_DT)
		return;



	V_WRITE_UPD_DT = pWRITE_UPD_DT;
	m_bModified = true;
}

void CCV_DATA::SetOD_USER_ID(CString pOD_USER_ID)
{
	if(V_OD_USER_ID == pOD_USER_ID)
		return;



	V_OD_USER_ID = pOD_USER_ID;
	m_bModified = true;
}

void CCV_DATA::SetOD_UPD_DT(CTime pOD_UPD_DT)
{
	if(V_OD_UPD_DT == pOD_UPD_DT)
		return;



	V_OD_UPD_DT = pOD_UPD_DT;
	m_bModified = true;
}

void CCV_DATA::SetFLOOR_NO(CString pFLOOR_NO)
{
	if(V_FLOOR_NO == pFLOOR_NO)
		return;



	V_FLOOR_NO = pFLOOR_NO;
	m_bModified = true;
}

void CCV_DATA::SetBCR_BOTTOM(CString pBCR_BOTTOM)
{
	if(V_BCR_BOTTOM == pBCR_BOTTOM)
		return;



	V_BCR_BOTTOM = pBCR_BOTTOM;
	m_bModified = true;
}

void CCV_DATA::SetBCR_TOP(CString pBCR_TOP)
{
	if(V_BCR_TOP == pBCR_TOP)
		return;



	V_BCR_TOP = pBCR_TOP;
	m_bModified = true;
}

void CCV_DATA::SetUSE_YN(CString pUSE_YN)
{
	if(V_USE_YN == pUSE_YN)
		return;



	V_USE_YN = pUSE_YN;
	m_bModified = true;
}


void CCV_DATA::SetRTV_IN_SIGN(CString pRTV_IN_SIGN)
{
	if(V_RTV_IN_SIGN == pRTV_IN_SIGN)
		return;



	V_RTV_IN_SIGN = pRTV_IN_SIGN;
	m_bModified = true;
}

void CCV_DATA::SetRTV_OUT_SIGN(CString pRTV_OUT_SIGN)
{
	if(V_RTV_OUT_SIGN == pRTV_OUT_SIGN)
		return;



	V_RTV_OUT_SIGN = pRTV_OUT_SIGN;
	m_bModified = true;
}

void CCV_DATA::SetRTV_LOCK_SIGN(CString pRTV_LOCK_SIGN)
{
	if(V_RTV_LOCK_SIGN == pRTV_LOCK_SIGN)
		return;



	V_RTV_LOCK_SIGN = pRTV_LOCK_SIGN;
	m_bModified = true;
}

void CCV_DATA::SetSENSOR4_DATA_RD(CString pSENSOR4_DATA_RD)
{
	if(V_SENSOR4_DATA_RD == pSENSOR4_DATA_RD)
		return;



	V_SENSOR4_DATA_RD = pSENSOR4_DATA_RD;
	m_bModified = true;
}

void CCV_DATA::SetRET_HS_YN(CString pRET_HS_YN)
{
	if(V_RET_HS_YN == pRET_HS_YN)
		return;



	V_RET_HS_YN = pRET_HS_YN;
	m_bModified = true;
}

void CCV_DATA::SetSUSPEND(CString pSUSPEND)
{
	if(V_SUSPEND == pSUSPEND)
		return;



	V_SUSPEND = pSUSPEND;
	m_bModified = true;
}

void CCV_DATA::SetOD_RQ_FLAG(CString pOD_RQ_FLAG)
{
	if(V_OD_RQ_FLAG == pOD_RQ_FLAG)
		return;



	V_OD_RQ_FLAG = pOD_RQ_FLAG;
	m_bModified = true;
}

void CCV_DATA::SetLIFTER_HS_YN(CString pLIFTER_HS_YN)
{
	if(V_LIFTER_HS_YN == pLIFTER_HS_YN)
		return;



	V_LIFTER_HS_YN = pLIFTER_HS_YN;
	m_bModified = true;
}

void CCV_DATA::SetDP_HS_YN(CString pDP_HS_YN)
{
	if(V_DP_HS_YN == pDP_HS_YN)
		return;



	V_DP_HS_YN = pDP_HS_YN;
	m_bModified = true;
}

void CCV_DATA::SetMG_HS_YN(CString pMG_HS_YN)
{
	if(V_MG_HS_YN == pMG_HS_YN)
		return;



	V_MG_HS_YN = pMG_HS_YN;
	m_bModified = true;
}

void CCV_DATA::SetLOOP_NM(CString pLOOP_NM)
{
	if(V_LOOP_NM == pLOOP_NM)
		return;



	V_LOOP_NM = pLOOP_NM;
	m_bModified = true;
}

void CCV_DATA::SetDESTINATION_YN(CString pDESTINATION_YN)
{
	if(V_DESTINATION_YN == pDESTINATION_YN)
		return;



	V_DESTINATION_YN = pDESTINATION_YN;
	m_bModified = true;
}

void CCV_DATA::SetCMD_RQ_ID(CString pCMD_RQ_ID)
{
	if(V_CMD_RQ_ID == pCMD_RQ_ID)
		return;



	V_CMD_RQ_ID = pCMD_RQ_ID;
	m_bModified = true;
}

void CCV_DATA::SetCMD_RQ_YN(CString pCMD_RQ_YN)
{
	if(V_CMD_RQ_YN == pCMD_RQ_YN)
		return;



	V_CMD_RQ_YN = pCMD_RQ_YN;
	m_bModified = true;
}

void CCV_DATA::SetCOMP_MG(CString pCOMP_MG)
{
	if(V_COMP_MG == pCOMP_MG)
		return;



	V_COMP_MG = pCOMP_MG;
	m_bModified = true;
}

void CCV_DATA::SetCV_BCR_BOTTOM(CString pCV_BCR_BOTTOM)
{
	if(V_CV_BCR_BOTTOM == pCV_BCR_BOTTOM)
		return;



	V_CV_BCR_BOTTOM = pCV_BCR_BOTTOM;
	m_bModified = true;
}

void CCV_DATA::SetCV_BCR_TOP(CString pCV_BCR_TOP)
{
	if(V_CV_BCR_TOP == pCV_BCR_TOP)
		return;



	V_CV_BCR_TOP = pCV_BCR_TOP;
	m_bModified = true;
}

void CCV_DATA::SetCOMP_DP(CString pCOMP_DP)
{
	if(V_COMP_DP == pCOMP_DP)
		return;



	V_COMP_DP = pCOMP_DP;
	m_bModified = true;
}

void CCV_DATA::SetMES_SEND_YN(CString pMES_SEND_YN)
{
	if(V_MES_SEND_YN == pMES_SEND_YN)
		return;



	V_MES_SEND_YN = pMES_SEND_YN;
	m_bModified = true;
}

void CCV_DATA::SetMES_ERR_SEND_YN(CString pMES_SEND_YN)
{
	if(V_MES_SEND_YN == pMES_SEND_YN)
		return;



	V_MES_SEND_YN = pMES_SEND_YN;
	m_bModified = true;
}

void CCV_DATA::SetWAIT_TIME_RD(CString pWAIT_TIME_RD)
{
	if (V_WAIT_TIME_RD == pWAIT_TIME_RD)
		return;

	V_WAIT_TIME_RD = pWAIT_TIME_RD;
	m_bModified = true;
}

void CCV_DATA::SetMC_NO(CString pMC_NO)
{
	if (V_MC_NO == pMC_NO)
		return;

	V_MC_NO = pMC_NO;
	m_bModified = true;
}

void CCV_DATA::SetMC_NO_NM(CString pMC_NO_NM)
{
	if (V_MC_NO_NM == pMC_NO_NM)
		return;

	V_MC_NO_NM = pMC_NO_NM;
	m_bModified = true;
}

void CCV_DATA::SetWAIT_SC_RET_JOB_RD(CString pWAIT_SC_RET_JOB_RD)
{
	if (V_WAIT_SC_RET_JOB_RD == pWAIT_SC_RET_JOB_RD)
		return;

	V_WAIT_SC_RET_JOB_RD = pWAIT_SC_RET_JOB_RD;
	m_bModified = true;
}

void CCV_DATA::SetWAIT_SC_RET_JOB_RD_NM(CString pWAIT_SC_RET_JOB_RD_NM)
{
	if (V_WAIT_SC_RET_JOB_RD_NM == pWAIT_SC_RET_JOB_RD_NM)
		return;

	V_WAIT_SC_RET_JOB_RD_NM = pWAIT_SC_RET_JOB_RD_NM;
	m_bModified = true;
}

void CCV_DATA::SetTR_PAUSE_RD(CString pTR_PAUSE_RD)
{
	if (V_TR_PAUSE_RD == pTR_PAUSE_RD)
		return;

	V_TR_PAUSE_RD = pTR_PAUSE_RD;
	m_bModified = true;
}

void CCV_DATA::SetTR_PAUSE_RD_NM(CString pTR_PAUSE_RD_NM)
{
	if (V_TR_PAUSE_RD_NM == pTR_PAUSE_RD_NM)
		return;

	V_TR_PAUSE_RD_NM = pTR_PAUSE_RD_NM;
	m_bModified = true;
}

void CCV_DATA::SetTR_PAUSE_OD(CString pTR_PAUSE_OD)
{
	if (V_TR_PAUSE_OD == pTR_PAUSE_OD)
		return;

	V_TR_PAUSE_OD = pTR_PAUSE_OD;
	m_bModified = true;
}

void CCV_DATA::SetSTOCK_MODE(CString pSTOCK_MODE)
{
	if (V_STOCK_MODE == pSTOCK_MODE)
		return;

	V_STOCK_MODE = pSTOCK_MODE;
	m_bModified = true;
}

void CCV_DATA::SetREMOTE_CONTROL(CString pREMOTE_CONTROL)
{
	if (V_REMOTE_CONTROL == pREMOTE_CONTROL)
		return;

	V_REMOTE_CONTROL = pREMOTE_CONTROL;
	m_bModified = true;
}

void CCV_DATA::SetROLL_MODE(CString pROLL_MODE)
{
	if (V_ROLL_MODE == pROLL_MODE)
		return;

	V_ROLL_MODE = pROLL_MODE;
	m_bModified = true;
}

void CCV_DATA::SetPULP_SENSOR_RD_NM(CString pPULP_SENSOR_RD_NM)
{
	if (V_PULP_SENSOR_RD_NM == pPULP_SENSOR_RD_NM)
		return;

	V_PULP_SENSOR_RD_NM = pPULP_SENSOR_RD_NM;
	m_bModified = true;
}

void CCV_DATA::SetEQP_TIME(CString pstrEQP_TIME)
{
	if(V_EQP_TIME == pstrEQP_TIME)
		return;

	V_EQP_TIME = pstrEQP_TIME;
	m_bModified = true;
}

void CCV_DATA::SetEQP_COLOR(CString pstrEQP_COLOR)
{
	if(V_EQP_COLOR == pstrEQP_COLOR)
		return;

	V_EQP_COLOR = pstrEQP_COLOR;
	m_bModified = true;
}

void CCV_DATA::SetEQP_CONNECTED_YN(CString pstrEQP_CONNECTED_YN)
{
	if(V_EQP_CONNECTED_YN == pstrEQP_CONNECTED_YN)
		return;

	V_EQP_CONNECTED_YN = pstrEQP_CONNECTED_YN;
	m_bModified = true;
}

void CCV_DATA::SetCHK_BYPASS_YN(CString pstrCHK_BYPASS_YN)
{
	if(V_CHK_BYPASS_YN == pstrCHK_BYPASS_YN)
		return;

	V_CHK_BYPASS_YN = pstrCHK_BYPASS_YN;
	m_bModified = true;
}

void CCV_DATA::SetITEM_NO(CString pITEM_NO)
{
	if(V_ITEM_NO == pITEM_NO)
		return;

	V_ITEM_NO = pITEM_NO;
	m_bModified = true;
}

void CCV_DATA::SetDRIV_PAPER_POS(CString pDRIV_PAPER_POS)
{
	if(V_DRIV_PAPER_POS == pDRIV_PAPER_POS)
		return;

	V_DRIV_PAPER_POS = pDRIV_PAPER_POS;
	m_bModified = true;
}

void CCV_DATA::SetELEV_ASC_ERR(CString pELEV_ASC_ERR)
{
	if(V_ELEV_ASC_ERR == pELEV_ASC_ERR)
		return;

	V_ELEV_ASC_ERR = pELEV_ASC_ERR;
	m_bModified = true;
}

void CCV_DATA::SetELEV_DESC_ERR(CString pELEV_DESC_ERR)
{
	if(V_ELEV_DESC_ERR == pELEV_DESC_ERR)
		return;

	V_ELEV_DESC_ERR = pELEV_DESC_ERR;
	m_bModified = true;
}

void CCV_DATA::SetCLAMP_FORWARD_ERR(CString pCLAMP_FORWARD_ERR)
{
	if(V_CLAMP_FORWARD_ERR == pCLAMP_FORWARD_ERR)
		return;

	V_CLAMP_FORWARD_ERR = pCLAMP_FORWARD_ERR;
	m_bModified = true;
}

void CCV_DATA::SetCLAMP_BACKWARD_ERR(CString pCLAMP_BACKWARD_ERR)
{
	if(V_CLAMP_BACKWARD_ERR == pCLAMP_BACKWARD_ERR)
		return;

	V_CLAMP_BACKWARD_ERR = pCLAMP_BACKWARD_ERR;
	m_bModified = true;
}

void CCV_DATA::SetDRIV_FORWARD_ERR(CString pDRIV_FORWARD_ERR)
{
	if(V_DRIV_FORWARD_ERR == pDRIV_FORWARD_ERR)
		return;

	V_DRIV_FORWARD_ERR = pDRIV_FORWARD_ERR;
	m_bModified = true;
}

void CCV_DATA::SetDRIV_BACKWARD_ERR(CString pDRIV_BACKWARD_ERR)
{
	if(V_DRIV_BACKWARD_ERR == pDRIV_BACKWARD_ERR)
		return;

	V_DRIV_BACKWARD_ERR = pDRIV_BACKWARD_ERR;
	m_bModified = true;
}

void CCV_DATA::SetPAPER_BLOCK_SENSOR1(CString pPAPER_BLOCK_SENSOR1)
{
	if(V_PAPER_BLOCK_SENSOR1 == pPAPER_BLOCK_SENSOR1)
		return;

	V_PAPER_BLOCK_SENSOR1 = pPAPER_BLOCK_SENSOR1;
	m_bModified = true;
}

void CCV_DATA::SetPAPER_BLOCK_SENSOR2(CString pPAPER_BLOCK_SENSOR2)
{
	if(V_PAPER_BLOCK_SENSOR2 == pPAPER_BLOCK_SENSOR2)
		return;

	V_PAPER_BLOCK_SENSOR2 = pPAPER_BLOCK_SENSOR2;
	m_bModified = true;
}

void CCV_DATA::SetPAPER_BLOCK_SENSOR3(CString pPAPER_BLOCK_SENSOR3)
{
	if(V_PAPER_BLOCK_SENSOR3 == pPAPER_BLOCK_SENSOR3)
		return;

	V_PAPER_BLOCK_SENSOR3 = pPAPER_BLOCK_SENSOR3;
	m_bModified = true;
}

void CCV_DATA::SetPAPER_BLOCK_SENSOR4(CString pPAPER_BLOCK_SENSOR4)
{
	if(V_PAPER_BLOCK_SENSOR4 == pPAPER_BLOCK_SENSOR4)
		return;

	V_PAPER_BLOCK_SENSOR4 = pPAPER_BLOCK_SENSOR4;
	m_bModified = true;
}

void CCV_DATA::SetPAPER_FULL_SENSOR(CString pPAPER_FULL_SENSOR)
{
	if(V_PAPER_FULL_SENSOR == pPAPER_FULL_SENSOR)
		return;

	V_PAPER_FULL_SENSOR = pPAPER_FULL_SENSOR;
	m_bModified = true;
}
void CCV_DATA::SetDRIV_FORWARD_POS(CString pDRIV_FORWARD_POS)
{
	if(V_DRIV_FORWARD_POS == pDRIV_FORWARD_POS)
		return;

	V_DRIV_FORWARD_POS = pDRIV_FORWARD_POS;
	m_bModified = true;
}

void CCV_DATA::SetDRIV_BACKWARD_POS(CString pDRIV_BACKWARD_POS)
{
	if(V_DRIV_BACKWARD_POS == pDRIV_BACKWARD_POS)
		return;

	V_DRIV_BACKWARD_POS = pDRIV_BACKWARD_POS;
	m_bModified = true;
}

void CCV_DATA::SetCRUSH_PAPER_SENSOR(CString pCRUSH_PAPER_SENSOR)
{
	if(V_CRUSH_PAPER_SENSOR == pCRUSH_PAPER_SENSOR)
		return;

	V_CRUSH_PAPER_SENSOR = pCRUSH_PAPER_SENSOR;
	m_bModified = true;
}

void CCV_DATA::SetCLAMP_FORWARD_SENSOR(CString pCLAMP_FORWARD_SENSOR)
{
	if(V_CLAMP_FORWARD_SENSOR == pCLAMP_FORWARD_SENSOR)
		return;

	V_CLAMP_FORWARD_SENSOR = pCLAMP_FORWARD_SENSOR;
	m_bModified = true;
}

void CCV_DATA::SetREV_ELEV_POS_DATA_ASC(CString pREV_ELEV_POS_DATA_ASC)
{
	if(V_REV_ELEV_POS_DATA_ASC == pREV_ELEV_POS_DATA_ASC)
		return;

	V_REV_ELEV_POS_DATA_ASC = pREV_ELEV_POS_DATA_ASC;
	m_bModified = true;
}

void CCV_DATA::SetREV_ELEV_POS_DATA_1(CString pREV_ELEV_POS_DATA_1)
{
	if(V_REV_ELEV_POS_DATA_1 == pREV_ELEV_POS_DATA_1)
		return;

	V_REV_ELEV_POS_DATA_1 = pREV_ELEV_POS_DATA_1;
	m_bModified = true;
}

void CCV_DATA::SetREV_ELEV_POS_DATA_2(CString pREV_ELEV_POS_DATA_2)
{
	if(V_REV_ELEV_POS_DATA_2 == pREV_ELEV_POS_DATA_2)
		return;

	V_REV_ELEV_POS_DATA_2 = pREV_ELEV_POS_DATA_2;
	m_bModified = true;
}

void CCV_DATA::SetREV_DRIV_POS_DATA_PICKUP(CString pREV_DRIV_POS_DATA_PICKUP)
{
	if(V_REV_DRIV_POS_DATA_PICKUP == pREV_DRIV_POS_DATA_PICKUP)
		return;

	V_REV_DRIV_POS_DATA_PICKUP = pREV_DRIV_POS_DATA_PICKUP;
	m_bModified = true;
}

void CCV_DATA::SetREV_DRIV_POS_DATA_REVERSE(CString pREV_DRIV_POS_DATA_REVERSE)
{
	if(V_REV_DRIV_POS_DATA_REVERSE == pREV_DRIV_POS_DATA_REVERSE)
		return;

	V_REV_DRIV_POS_DATA_REVERSE = pREV_DRIV_POS_DATA_REVERSE;
	m_bModified = true;
}

void CCV_DATA::SetREV_SENSOR_DATA_HEIGHT(CString pREV_SENSOR_DATA_HEIGHT)
{
	if(V_REV_SENSOR_DATA_HEIGHT == pREV_SENSOR_DATA_HEIGHT)
		return;

	V_REV_SENSOR_DATA_HEIGHT = pREV_SENSOR_DATA_HEIGHT;
	m_bModified = true;
}

void CCV_DATA::SetROLL_ELEV_POS_DATA_ASC(CString pROLL_ELEV_POS_DATA_ASC)
{
	if(V_ROLL_ELEV_POS_DATA_ASC == pROLL_ELEV_POS_DATA_ASC)
		return;

	V_ROLL_ELEV_POS_DATA_ASC = pROLL_ELEV_POS_DATA_ASC;
	m_bModified = true;
}

void CCV_DATA::SetROLL_ELEV_POS_DATA_PICKUP(CString pROLL_ELEV_POS_DATA_PICKUP)
{
	if(V_ROLL_ELEV_POS_DATA_PICKUP == pROLL_ELEV_POS_DATA_PICKUP)
		return;

	V_ROLL_ELEV_POS_DATA_PICKUP = pROLL_ELEV_POS_DATA_PICKUP;
	m_bModified = true;
}

void CCV_DATA::SetROLL_ELEV_POS_DATA_ROLLING(CString pROLL_ELEV_POS_DATA_ROLLING)
{
	if(V_ROLL_ELEV_POS_DATA_ROLLING == pROLL_ELEV_POS_DATA_ROLLING)
		return;

	V_ROLL_ELEV_POS_DATA_ROLLING = pROLL_ELEV_POS_DATA_ROLLING;
	m_bModified = true;
}

void CCV_DATA::SetROLL_CLAMP_POS_DATA_BACKWARD(CString pROLL_CLAMP_POS_DATA_BACKWARD)
{
	if(V_ROLL_CLAMP_POS_DATA_BACKWARD == pROLL_CLAMP_POS_DATA_BACKWARD)
		return;

	V_ROLL_CLAMP_POS_DATA_BACKWARD = pROLL_CLAMP_POS_DATA_BACKWARD;
	m_bModified = true;
}

void CCV_DATA::SetROLL_CLAMP_POS_DATA_FORWARD(CString pROLL_CLAMP_POS_DATA_FORWARD)
{
	if(V_ROLL_CLAMP_POS_DATA_FORWARD == pROLL_CLAMP_POS_DATA_FORWARD)
		return;

	V_ROLL_CLAMP_POS_DATA_FORWARD = pROLL_CLAMP_POS_DATA_FORWARD;
	m_bModified = true;
}

void CCV_DATA::SetROLL_SENSOR_DATA_HEIGHT(CString pROLL_SENSOR_DATA_HEIGHT)
{
	if(V_ROLL_SENSOR_DATA_HEIGHT == pROLL_SENSOR_DATA_HEIGHT)
		return;

	V_ROLL_SENSOR_DATA_HEIGHT = pROLL_SENSOR_DATA_HEIGHT;
	m_bModified = true;
}

void CCV_DATA::SetSC_PLT_JOB_TYP(CString pSC_PLT_JOB_TYP)
{
	if(V_SC_PLT_JOB_TYP == pSC_PLT_JOB_TYP)
		return;

	V_SC_PLT_JOB_TYP = pSC_PLT_JOB_TYP;
	m_bModified = true;
}

void CCV_DATA::SetCLAMP_BACKWARD_SENSOR(CString pCLAMP_BACKWARD_SENSOR)
{
	if(V_CLAMP_BACKWARD_SENSOR == pCLAMP_BACKWARD_SENSOR)
		return;

	V_CLAMP_BACKWARD_SENSOR = pCLAMP_BACKWARD_SENSOR;
	m_bModified = true;
}

CString CCV_DATA::GetMapKeyCV()
{
	CString strReturn;
	strReturn.Format(_T("%02s%02d"), m_strWH_TYP, m_nPlcNo);
	return strReturn;
}



CString CCV_DATA::GetMapKeyTRACK()
{
	CString strReturn;
	strReturn.Format(_T("%05d"), m_nNumber);
	return strReturn;
}