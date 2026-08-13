#pragma once
#include "afx.h"

#include "DciRvCtrl.h"
#include "DciButtonCtrl.h"

class CSC_DATA : public CObject
{
public:
	CSC_DATA(void);
	CSC_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrSC_NO);

	~CSC_DATA(void);

public:
	BOOL m_bModified;

public:
	CDciRvCtrl* m_pControl;
	CDciRvCtrl* m_pControl2;
	CDciRvCtrl* m_pControl3;
	CDciRvCtrl* m_pControl4;
	CDciRvCtrl* m_pControl5;

public:
	//key
	CString K_WH_TYP;
	CString K_PLC_NO;
	CString K_SC_NO;

public:
	void SetWH_TYP(CString K_WH_TYP);
	void SetPLC_NO(CString K_PLC_NO);
	void SetSC_NO(CString pSC_NO);

public:

	//data
	CString V_SC_VIEW_NM;
	CString V_SC_GRP_NO;
	CString V_ONLINE_MODE_RD;
	CString V_AUTO_MODE_RD;
	CString V_SENSOR_FK_RD;
	CString V_UCSTATUS_RD;
	CString V_POS_H_RD;
	CString V_POS_V_RD;
	CString V_ERR_CODE_RD;
	CString V_ERR_STA_FK1_RD;
	CString V_ERR_STA_FK2_RD;
	CString V_ACTIVE_MODE_RD;
	CString V_COMPLETE_RD;
	CString V_JOB_TYP_RD;
	CString V_LUGG_NO_FK1_RD;
	CString V_START_BANK_FK1_RD;
	CString V_START_HSPOS_FK1_RD;
	CString V_DEST_BANK_FK1_RD;
	CString V_DEST_HSPOS_FK1_RD;
	CString V_USE_FK_RD;
	CString V_LUGG_NO_FK2_RD;
	CString V_START_BANK_FK2_RD;
	CString V_START_HSPOS_FK2_RD;
	CString V_DEST_BANK_FK2_RD;
	CString V_DEST_HSPOS_FK2_RD;
	CString V_JOB_TYP_OD;
	CString V_LUGG_NO_FK1_OD;
	CString V_START_BANK_FK1_OD;
	CString V_START_HSPOS_FK1_OD;
	CString V_DEST_BANK_FK1_OD;
	CString V_DEST_HSPOS_FK1_OD;
	CString V_USE_FK_OD;
	CString V_LUGG_NO_FK2_OD;
	CString V_START_BANK_FK2_OD;
	CString V_START_HSPOS_FK2_OD;
	CString V_DEST_BANK_FK2_OD;
	CString V_DEST_HSPOS_FK2_OD;
	CString V_WRITE_CONTINUE_OD;
	CString V_WRITE_FLAG_OD;
	CString V_USER_COMMAND_OD;
	CString V_OD_RQ_YN;
	CTime V_READ_UPD_DT;
	CTime V_WRITE_UPD_DT;
	CString V_OD_USER_ID;
	CTime V_OD_UPD_DT;
	CString V_START_BAY_FK1_RD;
	CString V_START_LEVEL_FK1_RD;
	CString V_START_BAY_FK2_RD;
	CString V_START_LEVEL_FK2_RD;
	CString V_START_BAY_FK1_OD;
	CString V_START_LEVEL_FK1_OD;
	CString V_START_BAY_FK2_OD;
	CString V_START_LEVEL_FK2_OD;
	CString V_DEST_BAY_FK1_RD;
	CString V_DEST_LEVEL_FK1_RD;
	CString V_DEST_BAY_FK2_RD;
	CString V_DEST_LEVEL_FK2_RD;
	CString V_DEST_BAY_FK1_OD;
	CString V_DEST_LEVEL_FK1_OD;
	CString V_DEST_BAY_FK2_OD;
	CString V_DEST_LEVEL_FK2_OD;
	CString V_FORKPOS_FK1_RD;
	CString V_FORKPOS_FK2_RD;
	CString V_CV_WORKBENCH_RD;
	CString V_CV_WORKBENCH_SUB_RD;
	CString V_SC_TYP;
	CString V_SUSPEND;	
	CString V_ITN_LUGG_FK1;
	CString V_ITN_LUGG_FK2;
	CString V_PLT_INFO_RD;
	CString V_OD_RQ_FLAG;
	CString V_CMD_RQ_ID;
	CString V_CMD_RQ_YN;
	CString V_MES_SEND_YN;
	CString V_MES_ERR_SEND_YN;
	CString V_MC_NO;
	CString V_MC_NO_NM;

	CString V_EQP_TIME;
	CString V_EQP_CONNECTED_YN;
	CString V_EQP_COLOR;


public:
	void SetSC_GRP_NO(CString pSC_GRP_NO);
	void SetONLINE_MODE_RD(CString pONLINE_MODE_RD);
	void SetAUTO_MODE_RD(CString pAUTO_MODE_RD);
	void SetSENSOR_FK_RD(CString pSENSOR_FK_RD);
	void SetUCSTATUS_RD(CString pUCSTATUS_RD);
	void SetPOS_H_RD(CString pPOS_H_RD);
	void SetPOS_V_RD(CString pPOS_pRD);
	void SetERR_CODE_RD(CString pERR_CODE_RD);
	void SetERR_STA_FK1_RD(CString pERR_STA_FK1_RD);
	void SetERR_STA_FK2_RD(CString pERR_STA_FK2_RD);
	void SetACTIVE_MODE_RD(CString pACTIVE_MODE_RD);
	void SetCOMPLETE_RD(CString pCOMPLETE_RD);
	void SetJOB_TYP_RD(CString pJOB_TYP);
	void SetLUGG_NO_FK1_RD(CString pLUGG_NO_FK1_RD);
	void SetSTART_BANK_FK1_RD(CString pSTART_BANK_FK1_RD);
	void SetSTART_HSPOS_FK1_RD(CString pSTART_HSPOS_FK1_RD);
	void SetDEST_BANK_FK1_RD(CString pDEST_BANK_FK1_RD);
	void SetDEST_HSPOS_FK1_RD(CString pDEST_HSPOS_FK1_RD);
	void SetUSE_FK_RD(CString pUSE_FK_RD);
	void SetLUGG_NO_FK2_RD(CString pLUGG_NO_FK2_RD);
	void SetSTART_BANK_FK2_RD(CString pSTART_BANK_FK2_RD);
	void SetSTART_HSPOS_FK2_RD(CString pSTART_HSPOS_FK2_RD);
	void SetDEST_BANK_FK2_RD(CString pDEST_BANK_FK2_RD);
	void SetDEST_HSPOS_FK2_RD(CString pDEST_HSPOS_FK2_RD);
	void SetJOB_TYP_OD(CString pJOB_TYP_OD);
	void SetLUGG_NO_FK1_OD(CString pLUGG_NO_FK1_OD);
	void SetSTART_BANK_FK1_OD(CString pSTART_BANK_FK1_OD);
	void SetSTART_HSPOS_FK1_OD(CString pSTART_HSPOS_FK1_OD);
	void SetDEST_BANK_FK1_OD(CString pDEST_BANK_FK1_OD);
	void SetDEST_HSPOS_FK1_OD(CString pDEST_HSPOS_FK1_OD);
	void SetUSE_FK_OD(CString pUSE_FK_OD);
	void SetLUGG_NO_FK2_OD(CString pLUGG_NO_FK2_OD);
	void SetSTART_BANK_FK2_OD(CString pSTART_BANK_FK2_OD);
	void SetSTART_HSPOS_FK2_OD(CString pSTART_HSPOS_FK2_OD);
	void SetDEST_BANK_FK2_OD(CString pDEST_BANK_FK2_OD);
	void SetDEST_HSPOS_FK2_OD(CString pDEST_HSPOS_FK2_OD);
	void SetWRITE_CONTINUE_OD(CString pWRITE_CONTINUE_OD);
	void SetWRITE_FLAG_OD(CString pWRITE_FLAG_OD);
	void SetUSER_COMMAND_OD(CString pUSER_COMMAND_OD);
	void SetOD_RQ_YN(CString pOD_RQ_YN);
	void SetREAD_UPD_DT(CTime pREAD_UPD_DT);
	void SetWRITE_UPD_DT(CTime pWRITE_UPD_DT);
	void SetOD_USER_ID(CString pOD_USER_ID);
	void SetOD_UPD_DT(CTime pOD_UPD_DT);


	void SetSTART_BAY_FK1_RD(CString pSTART_BAY_FK1_RD);
	void SetSTART_LEVEL_FK1_RD(CString pSTART_LEVEL_FK1_RD);
	void SetSTART_BAY_FK2_RD(CString pSTART_BAY_FK2_RD);
	void SetSTART_LEVEL_FK2_RD(CString pSTART_LEVEL_FK2_RD);

	void SetSTART_BAY_FK1_OD(CString pSTART_BAY_FK1_OD);
	void SetSTART_LEVEL_FK1_OD(CString pSTART_LEVEL_FK1_OD);
	void SetSTART_BAY_FK2_OD(CString pSTART_BAY_FK2_OD);
	void SetSTART_LEVEL_FK2_OD(CString pSTART_LEVEL_FK2_OD);

	void SetDEST_BAY_FK1_RD(CString pDEST_BAY_FK1_RD);
	void SetDEST_LEVEL_FK1_RD(CString pDEST_LEVEL_FK1_RD);
	void SetDEST_BAY_FK2_RD(CString pDEST_BAY_FK2_RD);
	void SetDEST_LEVEL_FK2_RD(CString pDEST_LEVEL_FK2_RD);
	
	void SetDEST_BAY_FK1_OD(CString pDEST_BAY_FK1_OD);
	void SetDEST_LEVEL_FK1_OD(CString pDEST_LEVEL_FK1_OD);
	void SetDEST_BAY_FK2_OD(CString pDEST_BAY_FK2_OD);
	void SetDEST_LEVEL_FK2_OD(CString pDEST_LEVEL_FK2_OD);

	void SetFORKPOS_FK1_RD(CString pFORKPOS_FK1_RD);
	void SetFORKPOS_FK2_RD(CString pFORKPOS_FK2_RD);

	void SetCV_WORKBENCH_RD(CString pCV_WORKBENCH_RD);
	void SetCV_WORKBENCH_SUB_RD(CString pCV_WORKBENCH_SUB_RD);
	void SetSC_TYP(CString pSC_TYP);
	void SetSUSPEND(CString pSUSPEND);
	void SetITN_LUGG_FK1(CString pITN_LUGG_FK1);
	void SetITN_LUGG_FK2(CString pITN_LUGG_FK2);
	void SetPLT_INFO_RD(CString pPLT_INFO_RD);
	void SetOD_RQ_FLAG(CString pOD_RQ_FLAG);
	void SetCMD_RQ_ID(CString pCMD_RQ_ID);
	void SetCMD_RQ_YN(CString pCMD_RQ_YN);
	void SetMES_SEND_YN(CString pMES_SEND_YN);
	void SetMES_ERR_SEND_YN(CString pMES_SEND_YN);
	void SetSC_VIEW_NM(CString pSC_VIEW_NM);

	void SetMC_NO(CString pMC_NO);
	void SetMC_NO_NM(CString pMC_NO_NM);

	void SetEQP_TIME(CString V_EQP_TIME);
	void SetEQP_CONNECTED_YN(CString V_EQP_CONNECTED_YN);
	void SetEQP_COLOR(CString V_EQP_COLOR);

public:
	CString GetCid();
	CString GetCid(CString pnChar4);
};

