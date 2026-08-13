#pragma once
#include "afx.h"
#include "DciRvCtrl.h"

// RTV_DATA 명령 대상입니다.

class CRTV_DATA : public CObject
{
public:
	CRTV_DATA(void);
	CRTV_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrRTV_NO);

	~CRTV_DATA(void);

public:
	BOOL m_bModified;
	CDciRvCtrl* m_pControl;

public:
	CMap<CString, LPCTSTR, int, int> m_MapRtvPosition;

public:
	//key
	CString K_WH_TYP;
	CString K_PLC_NO;
	CString K_RTV_NO;

public:
	void SetWH_TYP(CString pWH_TYP);
	void SetPLC_NO(CString pPLC_NO);
	void SetRTV_NO(CString pRTV_NO);

public:

	//data
	CString V_AUTO_MODE_RD;
	CString V_SENSOR_FK_RD;
	CString V_UCSTATUS_RD;
	CString V_POS_H_RD;
	CString V_POS_V_RD;
	CString V_FORKPOS_FK1_RD;
	CString V_FORKPOS_FK2_RD;
	CString V_ERR_CODE_RD;
	CString V_ACTIVE_MODE_RD;
	CString V_COMPLETE_RD;
	CString V_JOB_TYP_RD;
	CString V_LUGG_NO_FK1_RD;
	CString V_DEPARTHS_NO_FK1_RD;
	CString V_ARRIVEHS_NO_FK1_RD;
	CString V_USE_FORK_RD;
	CString V_LUGG_NO_FK2_RD;
	CString V_DEPARTHS_NO_FK2_RD;
	CString V_ARRIVEHS_NO_FK2_RD;
	CString V_JOB_TYP_OD;
	CString V_LUGG_NO_FK1_OD;
	CString V_DEPARTHS_NO_FK1_OD;
	CString V_ARRIVEHS_NO_FK1_OD;
	CString V_USE_FK_OD;
	CString V_LUGG_NO_FK2_OD;
	CString V_DEPARTHS_NO_FK2_OD;
	CString V_ARRIVEHS_NO_FK2_OD;
	CString V_USER_COMMAND_OD;
	CString V_OD_RQ_YN;
	CTime V_READ_UPD_DT;
	CTime V_WRITE_UPD_DT;
	CString V_OD_USER_ID;
	CTime V_OD_UPD_DT;
	CString V_ITN_LUGG_FK1;
	CString V_ITN_LUGG_FK2;
	CString V_ITN_ARR_HS_MC_NO_FK1;
	CString V_ITN_ARR_HS_MC_NO_FK2;
	CString V_CMD_RQ_ID;
	CString V_CMD_RQ_YN;
	CString V_MES_SEND_YN;
	CString V_MES_ERR_SEND_YN;
	CString V_SUSPEND;


public:
	void SetAUTO_MODE_RD(CString pAUTO_MODE_RD);
	void SetSENSOR_FK_RD(CString pSENSOR_FK_RD);
	void SetUCSTATUS_RD(CString pUCSTATUS_RD);
	void SetPOS_H_RD(CString pPOS_H_RD);
	void SetPOS_V_RD(CString pPOS_V_RD);
	void SetFORKPOS_FK1_RD(CString pFORKPOS_FK1_RD);
	void SetFORKPOS_FK2_RD(CString pFORKPOS_FK2_RD);
	void SetERR_CODE_RD(CString pERR_CODE_RD);
	void SetACTIVE_MODE_RD(CString pACTIVE_MODE_RD);
	void SetCOMPLETE_RD(CString pCOMPLETE_RD);
	void SetJOB_TYP_RD(CString pJOB_TYP_RD);
	void SetLUGG_NO_FK1_RD(CString pLUGG_NO_FK1_RD);
	void SetDEPARTHS_NO_FK1_RD(CString pDEPARTHS_NO_FK1_RD);
	void SetARRIVEHS_NO_FK1_RD(CString pARRIVEHS_NO_FK1_RD);
	void SetUSE_FORK_RD(CString pUSE_FORK_RD);
	void SetLUGG_NO_FK2_RD(CString pLUGG_NO_FK2_RD);
	void SetDEPARTHS_NO_FK2_RD(CString pDEPARTHS_NO_FK2_RD);
	void SetARRIVEHS_NO_FK2_RD(CString pARRIVEHS_NO_FK2_RD);
	void SetJOB_TYP_OD(CString pJOB_TYP_OD);
	void SetLUGG_NO_FK1_OD(CString pLUGG_NO_FK1_OD);
	void SetDEPARTHS_NO_FK1_OD(CString pDEPARTHS_NO_FK1_OD);
	void SetARRIVEHS_NO_FK1_OD(CString pARRIVEHS_NO_FK1_OD);
	void SetUSE_FK_OD(CString pUSE_FK_OD);
	void SetLUGG_NO_FK2_OD(CString pLUGG_NO_FK2_OD);
	void SetDEPARTHS_NO_FK2_OD(CString pDEPARTHS_NO_FK2_OD);
	void SetARRIVEHS_NO_FK2_OD(CString pARRIVEHS_NO_FK2_OD);
	void SetUSER_COMMAND_OD(CString pUSER_COMMAND_OD);
	void SetOD_RQ_YN(CString pOD_RQ_YN);
	void SetREAD_UPD_DT(CTime pREAD_UPD_DT);
	void SetWRITE_UPD_DT(CTime pWRITE_UPD_DT);
	void SetOD_USER_ID(CString pOD_USER_ID);
	void SetOD_UPD_DT(CTime pOD_UPD_DT);
	void SetITN_LUGG_FK1(CString pITN_LUGG_FK1);
	void SetITN_LUGG_FK2(CString pITN_LUGG_FK2);
	void SetITN_ARR_HS_MC_NO_FK1(CString pITN_ARR_HS_MC_NO_FK1);
	void SetITN_ARR_HS_MC_NO_FK2(CString pITN_ARR_HS_MC_NO_FK2);
	void SetCMD_RQ_ID(CString pCMD_RQ_ID);
	void SetCMD_RQ_YN(CString pCMD_RQ_YN);
	void SetMES_SEND_YN(CString pMES_SEND_YN);
	void SetMES_ERR_SEND_YN(CString pMES_ERR_SEND_YN);
	void SetSUSPEND(CString pSUSPEND);

public:
	CString GetCid();
};



