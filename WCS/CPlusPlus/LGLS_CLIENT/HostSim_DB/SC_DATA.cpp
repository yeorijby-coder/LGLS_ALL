#include "StdAfx.h"
#include "SC_DATA.h"


CSC_DATA::CSC_DATA(void)
{
	m_pControl = NULL;
	//m_pControl2 = NULL;
	//m_pControl3 = NULL;
	//m_pControl4 = NULL;
	//m_pControl5 = NULL;
}


CSC_DATA::CSC_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrSC_NO)
{
	m_pControl = NULL;
	//m_pControl2 = NULL;
	//m_pControl3 = NULL;
	//m_pControl4 = NULL;
	//m_pControl5 = NULL;

	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
	K_SC_NO = pstrSC_NO;
}

CSC_DATA::~CSC_DATA(void)
{
}

void CSC_DATA::SetSC_NO(CString pstrSC_NO)
{
	K_SC_NO = pstrSC_NO;
}

void CSC_DATA::SetWH_TYP(CString pstrWH_TYP)
{
	K_WH_TYP = pstrWH_TYP;
}

void CSC_DATA::SetPLC_NO(CString pstrPLC_NO)
{
	K_PLC_NO = pstrPLC_NO;
}

void CSC_DATA::SetSC_GRP_NO(CString pSC_GRP_NO)
{
	if(V_SC_GRP_NO == pSC_GRP_NO)
		return;



	V_SC_GRP_NO = pSC_GRP_NO;
	m_bModified = true;
}

void CSC_DATA::SetONLINE_MODE_RD(CString pONLINE_MODE_RD)
{
	if(V_ONLINE_MODE_RD == pONLINE_MODE_RD)
		return;



	V_ONLINE_MODE_RD = pONLINE_MODE_RD;
	m_bModified = true;
}

void CSC_DATA::SetAUTO_MODE_RD(CString pAUTO_MODE_RD)
{
	if(V_AUTO_MODE_RD == pAUTO_MODE_RD)
		return;



	V_AUTO_MODE_RD = pAUTO_MODE_RD;
	m_bModified = true;
}

void CSC_DATA::SetSENSOR_FK_RD(CString pSENSOR_FK_RD)
{
	if(V_SENSOR_FK_RD == pSENSOR_FK_RD)
		return;



	V_SENSOR_FK_RD = pSENSOR_FK_RD;
	m_bModified = true;
}

void CSC_DATA::SetUCSTATUS_RD(CString pUCSTATUS_RD)
{
	if(V_UCSTATUS_RD == pUCSTATUS_RD)
		return;



	V_UCSTATUS_RD = pUCSTATUS_RD;
	m_bModified = true;
}

void CSC_DATA::SetPOS_H_RD(CString pPOS_H_RD)
{
	if(V_POS_H_RD == pPOS_H_RD)
		return;



	V_POS_H_RD = pPOS_H_RD;
	m_bModified = true;
}

void CSC_DATA::SetPOS_V_RD(CString pPOS_V_RD)
{
	if(V_POS_V_RD == pPOS_V_RD)
		return;



	V_POS_V_RD = pPOS_V_RD;
	m_bModified = true;
}

void CSC_DATA::SetERR_CODE_RD(CString pERR_CODE_RD)
{
	if(V_ERR_CODE_RD == pERR_CODE_RD)
		return;



	V_ERR_CODE_RD = pERR_CODE_RD;
	m_bModified = true;
}

void CSC_DATA::SetERR_STA_FK1_RD(CString pERR_STA_FK1_RD)
{
	if(V_ERR_STA_FK1_RD == pERR_STA_FK1_RD)
		return;



	V_ERR_STA_FK1_RD = pERR_STA_FK1_RD;
	m_bModified = true;
}

void CSC_DATA::SetERR_STA_FK2_RD(CString pERR_STA_FK2_RD)
{
	if(V_ERR_STA_FK2_RD == pERR_STA_FK2_RD)
		return;



	V_ERR_STA_FK2_RD = pERR_STA_FK2_RD;
	m_bModified = true;
}

void CSC_DATA::SetACTIVE_MODE_RD(CString pACTIVE_MODE_RD)
{
	if(V_ACTIVE_MODE_RD == pACTIVE_MODE_RD)
		return;



	V_ACTIVE_MODE_RD = pACTIVE_MODE_RD;
	m_bModified = true;
}

void CSC_DATA::SetCOMPLETE_RD(CString pCOMPLETE_RD)
{
	if(V_COMPLETE_RD == pCOMPLETE_RD)
		return;



	V_COMPLETE_RD = pCOMPLETE_RD;
	m_bModified = true;
}

void CSC_DATA::SetJOB_TYP_RD(CString pJOB_TYP)
{
	if(V_JOB_TYP_RD == pJOB_TYP)
		return;

	

	V_JOB_TYP_RD = pJOB_TYP;
	m_bModified = true;
}

void CSC_DATA::SetLUGG_NO_FK1_RD(CString pLUGG_NO_FK1_RD)
{
	if(V_LUGG_NO_FK1_RD == pLUGG_NO_FK1_RD)
		return;



	V_LUGG_NO_FK1_RD = pLUGG_NO_FK1_RD;
	m_bModified = true;
}

void CSC_DATA::SetSTART_BANK_FK1_RD(CString pSTART_BANK_FK1_RD)
{
	if(V_START_BANK_FK1_RD == pSTART_BANK_FK1_RD)
		return;



	V_START_BANK_FK1_RD = pSTART_BANK_FK1_RD;
	m_bModified = true;
}

void CSC_DATA::SetSTART_HSPOS_FK1_RD(CString pSTART_HSPOS_FK1_RD)
{
	if(V_START_HSPOS_FK1_RD == pSTART_HSPOS_FK1_RD)
		return;



	V_START_HSPOS_FK1_RD = pSTART_HSPOS_FK1_RD;
	m_bModified = true;
}


void CSC_DATA::SetDEST_BANK_FK1_RD(CString pDEST_BANK_FK1_RD)
{
	if(V_DEST_BANK_FK1_RD == pDEST_BANK_FK1_RD)
		return;



	V_DEST_BANK_FK1_RD = pDEST_BANK_FK1_RD;
	m_bModified = true;
}

void CSC_DATA::SetDEST_HSPOS_FK1_RD(CString pDEST_HSPOS_FK1_RD)
{
	if(V_DEST_HSPOS_FK1_RD == pDEST_HSPOS_FK1_RD)
		return;



	V_DEST_HSPOS_FK1_RD = pDEST_HSPOS_FK1_RD;
	m_bModified = true;
}

void CSC_DATA::SetUSE_FK_RD(CString pUSE_FK_RD)
{;
	if(V_USE_FK_RD == pUSE_FK_RD)
		return;

	

	V_USE_FK_RD = pUSE_FK_RD;
	m_bModified = true;
}

void CSC_DATA::SetLUGG_NO_FK2_RD(CString pLUGG_NO_FK2_RD)
{
	if(V_LUGG_NO_FK2_RD == pLUGG_NO_FK2_RD)
		return;



	V_LUGG_NO_FK2_RD = pLUGG_NO_FK2_RD;
	m_bModified = true;
}

void CSC_DATA::SetSTART_BANK_FK2_RD(CString pSTART_BANK_FK2_RD)
{
	if(V_START_BANK_FK2_RD == pSTART_BANK_FK2_RD)
		return;



	V_START_BANK_FK2_RD = pSTART_BANK_FK2_RD;
	m_bModified = true;
}

void CSC_DATA::SetSTART_HSPOS_FK2_RD(CString pSTART_HSPOS_FK2_RD)
{
	if(V_START_HSPOS_FK2_RD == pSTART_HSPOS_FK2_RD)
		return;



	V_START_HSPOS_FK2_RD = pSTART_HSPOS_FK2_RD;
	m_bModified = true;
}

void CSC_DATA::SetDEST_BANK_FK2_RD(CString pDEST_BANK_FK2_RD)
{
	if(V_DEST_BANK_FK2_RD == pDEST_BANK_FK2_RD)
		return;



	V_DEST_BANK_FK2_RD = pDEST_BANK_FK2_RD;
	m_bModified = true;
}

void CSC_DATA::SetDEST_HSPOS_FK2_RD(CString pDEST_HSPOS_FK2_RD)
{
	if(V_DEST_HSPOS_FK2_RD == pDEST_HSPOS_FK2_RD)
		return;



	V_DEST_HSPOS_FK2_RD = pDEST_HSPOS_FK2_RD;
	m_bModified = true;
}

void CSC_DATA::SetJOB_TYP_OD(CString pJOB_TYP_OD)
{
	if(V_JOB_TYP_OD == pJOB_TYP_OD)
		return;



	V_JOB_TYP_OD = pJOB_TYP_OD;
	m_bModified = true;
}

void CSC_DATA::SetLUGG_NO_FK1_OD(CString pLUGG_NO_FK1_OD)
{
	if(V_LUGG_NO_FK1_OD == pLUGG_NO_FK1_OD)
		return;



	V_LUGG_NO_FK1_OD = pLUGG_NO_FK1_OD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_BANK_FK1_OD(CString pSTART_BANK_FK1_OD)
{
	if(V_START_BANK_FK1_OD == pSTART_BANK_FK1_OD)
		return;



	V_START_BANK_FK1_OD = pSTART_BANK_FK1_OD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_HSPOS_FK1_OD(CString pSTART_HSPOS_FK1_OD)
{
	if(V_START_HSPOS_FK1_OD == pSTART_HSPOS_FK1_OD)
		return;



	V_START_HSPOS_FK1_OD = pSTART_HSPOS_FK1_OD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_BANK_FK1_OD(CString pDEST_BANK_FK1_OD)
{
	if(V_DEST_BANK_FK1_OD == pDEST_BANK_FK1_OD)
		return;



	V_DEST_BANK_FK1_OD = pDEST_BANK_FK1_OD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_HSPOS_FK1_OD(CString pDEST_HSPOS_FK1_OD)
{
	if(V_DEST_HSPOS_FK1_OD == pDEST_HSPOS_FK1_OD)
		return;



	V_DEST_HSPOS_FK1_OD = pDEST_HSPOS_FK1_OD;
	m_bModified = true;

}

void CSC_DATA::SetUSE_FK_OD(CString pUSE_FK_OD)
{
	if(V_USE_FK_OD == pUSE_FK_OD)
		return;



	V_USE_FK_OD = pUSE_FK_OD;
	m_bModified = true;

}

void CSC_DATA::SetLUGG_NO_FK2_OD(CString pLUGG_NO_FK2_OD)
{
	if(V_LUGG_NO_FK2_OD == pLUGG_NO_FK2_OD)
		return;



	V_LUGG_NO_FK2_OD = pLUGG_NO_FK2_OD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_BANK_FK2_OD(CString pSTART_BANK_FK2_OD)
{
	if(V_START_BANK_FK2_OD == pSTART_BANK_FK2_OD)
		return;



	V_START_BANK_FK2_OD = pSTART_BANK_FK2_OD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_HSPOS_FK2_OD(CString pSTART_HSPOS_FK2_OD)
{
	if(V_START_HSPOS_FK2_OD == pSTART_HSPOS_FK2_OD)
		return;



	V_START_HSPOS_FK2_OD = pSTART_HSPOS_FK2_OD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_BANK_FK2_OD(CString pDEST_BANK_FK2_OD)
{
	if(V_DEST_BANK_FK2_OD == pDEST_BANK_FK2_OD)
		return;



	V_DEST_BANK_FK2_OD = pDEST_BANK_FK2_OD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_HSPOS_FK2_OD(CString pDEST_HSPOS_FK2_OD)
{
	if(V_DEST_HSPOS_FK2_OD == pDEST_HSPOS_FK2_OD)
		return;



	V_DEST_HSPOS_FK2_OD = pDEST_HSPOS_FK2_OD;
	m_bModified = true;

}

void CSC_DATA::SetWRITE_CONTINUE_OD(CString pWRITE_CONTINUE_OD)
{
	if(V_WRITE_CONTINUE_OD == pWRITE_CONTINUE_OD)
		return;



	V_WRITE_CONTINUE_OD = pWRITE_CONTINUE_OD;
	m_bModified = true;

}

void CSC_DATA::SetWRITE_FLAG_OD(CString pWRITE_FLAG_OD)
{
	if(V_WRITE_FLAG_OD == pWRITE_FLAG_OD)
		return;



	V_WRITE_FLAG_OD = pWRITE_FLAG_OD;
	m_bModified = true;

}

void CSC_DATA::SetUSER_COMMAND_OD(CString pUSER_COMMAND_OD)
{
	if(V_USER_COMMAND_OD == pUSER_COMMAND_OD)
		return;



	V_USER_COMMAND_OD = pUSER_COMMAND_OD;
	m_bModified = true;

}

void CSC_DATA::SetOD_RQ_YN(CString pOD_RQ_YN)
{
	if(V_OD_RQ_YN == pOD_RQ_YN)
		return;



	V_OD_RQ_YN = pOD_RQ_YN;
	m_bModified = true;

}

void CSC_DATA::SetREAD_UPD_DT(CTime pREAD_UPD_DT)
{
	if(V_READ_UPD_DT == pREAD_UPD_DT)
		return;



	V_READ_UPD_DT = pREAD_UPD_DT;
	m_bModified = true;

}

void CSC_DATA::SetWRITE_UPD_DT(CTime pWRITE_UPD_DT)
{
	if(V_WRITE_UPD_DT == pWRITE_UPD_DT)
		return;



	V_WRITE_UPD_DT = pWRITE_UPD_DT;
	m_bModified = true;

}

void CSC_DATA::SetOD_USER_ID(CString pOD_USER_ID)
{
	if(V_OD_USER_ID == pOD_USER_ID)
		return;



	V_OD_USER_ID = pOD_USER_ID;
	m_bModified = true;

}

void CSC_DATA::SetOD_UPD_DT(CTime pOD_UPD_DT)
{
	if(V_OD_UPD_DT == pOD_UPD_DT)
		return;



	V_OD_UPD_DT = pOD_UPD_DT;
	m_bModified = true;

}

void CSC_DATA::SetSTART_BAY_FK1_RD(CString pSTART_BAY_FK1_RD)
{
	if(V_START_BAY_FK1_RD == pSTART_BAY_FK1_RD)
		return;



	V_START_BAY_FK1_RD = pSTART_BAY_FK1_RD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_LEVEL_FK1_RD(CString pSTART_LEVEL_FK1_RD)
{
	if(V_START_LEVEL_FK1_RD == pSTART_LEVEL_FK1_RD)
		return;



	V_START_LEVEL_FK1_RD = pSTART_LEVEL_FK1_RD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_BAY_FK2_RD(CString pSTART_BAY_FK2_RD)
{
	if(V_START_BAY_FK2_RD == pSTART_BAY_FK2_RD)
		return;



	V_START_BAY_FK2_RD = pSTART_BAY_FK2_RD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_LEVEL_FK2_RD(CString pSTART_LEVEL_FK2_RD)
{
	if(V_START_LEVEL_FK2_RD == pSTART_LEVEL_FK2_RD)
		return;



	V_START_LEVEL_FK2_RD = pSTART_LEVEL_FK2_RD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_BAY_FK1_OD(CString pSTART_BAY_FK1_OD)
{
	if(V_START_BAY_FK1_OD == pSTART_BAY_FK1_OD)
		return;



	V_START_BAY_FK1_OD = pSTART_BAY_FK1_OD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_LEVEL_FK1_OD(CString pSTART_LEVEL_FK1_OD)
{
	if(V_START_LEVEL_FK1_OD == pSTART_LEVEL_FK1_OD)
		return;



	V_START_LEVEL_FK1_OD = pSTART_LEVEL_FK1_OD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_BAY_FK2_OD(CString pSTART_BAY_FK2_OD)
{
	if(V_START_BAY_FK2_OD == pSTART_BAY_FK2_OD)
		return;



	V_START_BAY_FK2_OD = pSTART_BAY_FK2_OD;
	m_bModified = true;

}

void CSC_DATA::SetSTART_LEVEL_FK2_OD(CString pSTART_LEVEL_FK2_OD)
{
	if(V_START_LEVEL_FK2_OD == pSTART_LEVEL_FK2_OD)
		return;



	V_START_LEVEL_FK2_OD = pSTART_LEVEL_FK2_OD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_BAY_FK1_RD(CString pDEST_BAY_FK1_RD)
{
	if(V_DEST_BAY_FK1_RD == pDEST_BAY_FK1_RD)
		return;



	V_DEST_BAY_FK1_RD = pDEST_BAY_FK1_RD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_LEVEL_FK1_RD(CString pDEST_LEVEL_FK1_RD)
{
	if(V_DEST_LEVEL_FK1_RD == pDEST_LEVEL_FK1_RD)
		return;



	V_DEST_LEVEL_FK1_RD = pDEST_LEVEL_FK1_RD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_BAY_FK2_RD(CString pDEST_BAY_FK2_RD)
{
	if(V_DEST_BAY_FK2_RD == pDEST_BAY_FK2_RD)
		return;



	V_DEST_BAY_FK2_RD = pDEST_BAY_FK2_RD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_LEVEL_FK2_RD(CString pDEST_LEVEL_FK2_RD)
{
	if(V_DEST_LEVEL_FK2_RD == pDEST_LEVEL_FK2_RD)
		return;



	V_DEST_LEVEL_FK2_RD = pDEST_LEVEL_FK2_RD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_BAY_FK1_OD(CString pDEST_BAY_FK1_OD)
{
	if(V_DEST_BAY_FK1_OD == pDEST_BAY_FK1_OD)
		return;



	V_DEST_BAY_FK1_OD = pDEST_BAY_FK1_OD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_LEVEL_FK1_OD(CString pDEST_LEVEL_FK1_OD)
{
	if(V_DEST_LEVEL_FK1_OD == pDEST_LEVEL_FK1_OD)
		return;



	V_DEST_LEVEL_FK1_OD = pDEST_LEVEL_FK1_OD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_BAY_FK2_OD(CString pDEST_BAY_FK2_OD)
{
	if(V_DEST_BAY_FK2_OD == pDEST_BAY_FK2_OD)
		return;



	V_DEST_BAY_FK2_OD = pDEST_BAY_FK2_OD;
	m_bModified = true;

}

void CSC_DATA::SetDEST_LEVEL_FK2_OD(CString pDEST_LEVEL_FK2_OD)
{
	if(V_DEST_LEVEL_FK2_OD == pDEST_LEVEL_FK2_OD)
		return;



	V_DEST_LEVEL_FK2_OD = pDEST_LEVEL_FK2_OD;
	m_bModified = true;

}

void CSC_DATA::SetFORKPOS_FK1_RD(CString pFORKPOS_FK1_RD)
{
	if(V_FORKPOS_FK1_RD == pFORKPOS_FK1_RD)
		return;



	V_FORKPOS_FK1_RD = pFORKPOS_FK1_RD;
	m_bModified = true;

}

void CSC_DATA::SetFORKPOS_FK2_RD(CString pFORKPOS_FK2_RD)
{
	if(V_FORKPOS_FK2_RD == pFORKPOS_FK2_RD)
		return;



	V_FORKPOS_FK2_RD = pFORKPOS_FK2_RD;
	m_bModified = true;

}

void CSC_DATA::SetCV_WORKBENCH_RD(CString pCV_WORKBENCH_RD)
{
	if(V_CV_WORKBENCH_RD == pCV_WORKBENCH_RD)
		return;



	V_CV_WORKBENCH_RD = pCV_WORKBENCH_RD;
	m_bModified = true;

}

void CSC_DATA::SetCV_WORKBENCH_SUB_RD(CString pCV_WORKBENCH_SUB_RD)
{
	if(V_CV_WORKBENCH_SUB_RD == pCV_WORKBENCH_SUB_RD)
		return;



	V_CV_WORKBENCH_SUB_RD = pCV_WORKBENCH_SUB_RD;
	m_bModified = true;

}

void CSC_DATA::SetSC_TYP(CString pSC_TYP)
{
	if(V_SC_TYP == pSC_TYP)
		return;



	V_SC_TYP = pSC_TYP;
	m_bModified = true;

}

void CSC_DATA::SetSUSPEND(CString pSUSPEND)
{
	if(V_SUSPEND == pSUSPEND)
		return;



	V_SUSPEND = pSUSPEND;
	m_bModified = true;

}

void CSC_DATA::SetITN_LUGG_FK1(CString pITN_LUGG_FK1)
{
	if(V_ITN_LUGG_FK1 == pITN_LUGG_FK1)
		return;



	V_ITN_LUGG_FK1 = pITN_LUGG_FK1;
	m_bModified = true;

}

void CSC_DATA::SetITN_LUGG_FK2(CString pITN_LUGG_FK2)
{
	if(V_ITN_LUGG_FK2 == pITN_LUGG_FK2)
		return;



	V_ITN_LUGG_FK2 = pITN_LUGG_FK2;
	m_bModified = true;

}

void CSC_DATA::SetPLT_INFO_RD(CString pPLT_INFO_RD)
{
	if(V_PLT_INFO_RD == pPLT_INFO_RD)
		return;



	V_PLT_INFO_RD = pPLT_INFO_RD;
	m_bModified = true;

}

void CSC_DATA::SetOD_RQ_FLAG(CString pOD_RQ_FLAG)
{
	if(V_OD_RQ_FLAG == pOD_RQ_FLAG)
		return;



	V_OD_RQ_FLAG = pOD_RQ_FLAG;
	m_bModified = true;

}

void CSC_DATA::SetCMD_RQ_ID(CString pCMD_RQ_ID)
{
	if(V_CMD_RQ_ID == pCMD_RQ_ID)
		return;



	V_CMD_RQ_ID = pCMD_RQ_ID;
	m_bModified = true;

}

void CSC_DATA::SetCMD_RQ_YN(CString pCMD_RQ_YN)
{
	if(V_CMD_RQ_YN == pCMD_RQ_YN)
		return;



	V_CMD_RQ_YN = pCMD_RQ_YN;
	m_bModified = true;

}

void CSC_DATA::SetMES_SEND_YN(CString pMES_SEND_YN)
{
	if(V_MES_SEND_YN == pMES_SEND_YN)
		return;



	V_MES_SEND_YN = pMES_SEND_YN;
	m_bModified = true;

}

void CSC_DATA::SetMES_ERR_SEND_YN(CString pMES_SEND_YN)
{
	if(V_MES_SEND_YN == pMES_SEND_YN)
		return;



	V_MES_SEND_YN = pMES_SEND_YN;
	m_bModified = true;

}

void CSC_DATA::SetSC_VIEW_NM(CString pSC_VIEW_NM)
{
	if(V_SC_VIEW_NM == pSC_VIEW_NM)
		return;

	V_SC_VIEW_NM = pSC_VIEW_NM;
	m_bModified = true;

}

void CSC_DATA::SetMC_NO(CString pMC_NO)
{
	if(V_MC_NO == pMC_NO)
		return;

	V_MC_NO = pMC_NO;
	m_bModified = true;

}

void CSC_DATA::SetMC_NO_NM(CString pMC_NO_NM)
{
	if(V_MC_NO_NM == pMC_NO_NM)
		return;

	V_MC_NO_NM = pMC_NO_NM;
	m_bModified = true;

}

void CSC_DATA::SetEQP_TIME(CString pstrEQP_TIME)
{
	if(V_EQP_TIME == pstrEQP_TIME)
		return;

	V_EQP_TIME = pstrEQP_TIME;
	m_bModified = true;
}

void CSC_DATA::SetEQP_COLOR(CString pstrEQP_COLOR)
{
	if(V_EQP_COLOR == pstrEQP_COLOR)
		return;

	V_EQP_COLOR = pstrEQP_COLOR;
	m_bModified = true;
}

void CSC_DATA::SetEQP_CONNECTED_YN(CString pstrEQP_CONNECTED_YN)
{
	if(V_EQP_CONNECTED_YN == pstrEQP_CONNECTED_YN)
		return;

	V_EQP_CONNECTED_YN = pstrEQP_CONNECTED_YN;
	m_bModified = true;
}

CString CSC_DATA::GetCid()
{
	CString strCID;
	strCID.Format(_T("17120%03s"), K_SC_NO);
	return strCID;
}

CString CSC_DATA::GetCid(CString nChar4)
{
	CString strCID;
	strCID.Format(_T("17120%03s"), K_SC_NO);
	return strCID;
}