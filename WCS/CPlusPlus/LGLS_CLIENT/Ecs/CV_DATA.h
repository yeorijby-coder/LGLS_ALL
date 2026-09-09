#pragma once
#include "afx.h"
#include "Info.h"
#include "DciTrackCtrl.h"
// CCV_DATA 명령 대상입니다.

class CCV_DATA : public CInfo
{

public:
	CCV_DATA(void);
	//트랙전용
	CCV_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrTRACK_NO);

	//설비전용 parent
	CCV_DATA(CString pstrWH_TYP, CString pstrPLC_NO);
	~CCV_DATA(void);

public:
	CMap<CString, LPCTSTR, CCV_DATA*, CCV_DATA*> m_MapTracks;		
	CDciTrackCtrl*	m_pTrackCtrl;	

public:
	BOOL m_bModified;

public:
	//key
	CString K_WH_TYP;
	CString K_PLC_NO;
	CString K_TRACK_NO;

public:
	void SetWH_TYP(CString pWH_TYP);
	void SetPLC_NO(CString pPLC_NO);
	void SetTRACK_NO(CString pTRACK_NO);

public:
	//data
	CString V_WH_TYP;
	CString V_PLC_NO;
	CString V_TRACK_NO;
	CString V_LUGG_NO_RD;
	CString V_LUGG_NO_OD;
	CString V_DEST_POS_RD;
	CString V_DEST_POS_OD;
	CString V_JOB_TYP_RD;
	CString V_JOB_TYP_OD;
	CString V_PULP_SENSOR_RD;
	CString V_PULP_SENSOR_OD;
	CString V_TRAY_LEV_RD;
	CString V_TRAY_LEV_OD;
	CString V_TRAY_TYP_RD;
	CString V_TRAY_TYP_OD;
	CString V_ERR_RQ_RD;
	CString V_ERR_RQ_OD;
	CString V_AUTO_MODE_RD;
	CString V_STO_READY_RD;
	CString V_RET_READY_RD;
	CString V_STOHS_READY_RD;
	CString V_RETHS_READY_RD;
	CString V_RTV_DEPARTHS_READY_RD;
	CString V_RTV_ARRIVEHS_READY_RD;
	CString V_LGV_DEPARTHS_READY_RD;
	CString V_LGV_ARRIVEHS_READY_RD;
	CString V_SENSOR0_DATA_RD;
	CString V_SENSOR1_DATA_RD;
	CString V_SENSOR2_DATA_RD;
	CString V_SENSOR3_DATA_RD;
	CString V_SENSOR4_DATA_RD;
	CString V_RTV_IN_SIGN;
	CString V_RTV_OUT_SIGN;
	CString V_RTV_LOCK_SIGN;
	CString V_MES_ERROR_CD;
	CString V_WAIT_SC_RET_JOB_RD;
	CString V_WAIT_SC_RET_JOB_RD_NM;
	CString V_WAIT_SC_RET_JOB_OD;


	CString V_ERROR_CODE;
	CString V_OD_RQ_YN;
	CTime V_READ_UPD_DT;
	CTime V_WRITE_UPD_DT;
	CString V_OD_USER_ID;
	CTime V_OD_UPD_DT;
	CString V_FLOOR_NO;
	CString V_BCR_BOTTOM;
	CString V_BCR_TOP;
	CString V_USE_YN;
	CString V_RET_HS_YN;
	CString V_SUSPEND;
	CString V_OD_RQ_FLAG;
	CString V_LIFTER_HS_YN;
	CString V_DP_HS_YN;
	CString V_MG_HS_YN;
	CString V_LOOP_NM;
	CString V_DESTINATION_YN;
	CString V_CMD_RQ_ID;
	CString V_CMD_RQ_YN;
	CString V_COMP_MG;
	CString V_CV_BCR_BOTTOM; //사용되지 않음
	CString V_CV_BCR_TOP;  //사용되지 않음
	CString V_COMP_DP;
	CString V_MES_SEND_YN;
	CString V_MES_ERR_SEND_YN;
	CString V_WAIT_TIME_RD;

	CString V_MC_NO;
	CString V_MC_NO_NM;
	CString V_TR_PAUSE_RD;
	CString V_TR_PAUSE_RD_NM;
	CString V_TR_PAUSE_OD;

	CString V_STOCK_MODE;
	CString V_REMOTE_CONTROL;
	CString V_PULP_SENSOR_RD_NM;

	CString V_EQP_TIME;
	CString V_EQP_CONNECTED_YN;
	CString V_EQP_COLOR;

	CString V_CHK_BYPASS_YN;
	CString V_ITEM_NO;


	CString V_SC_PLT_JOB_TYP;

public:
	void SetLUGG_NO_RD(CString pLUGG_NO_RD);
	void SetLUGG_NO_OD(CString pLUGG_NO_OD);
	void SetDEST_POS_RD(CString pDEST_POS_RD);
	void SetDEST_POS_OD(CString pDEST_POS_OD);
	void SetIS_TURN_RD(CString pIS_TURN_RD);
	void SetIS_TURN_OD(CString pIS_TURN_OD);
	void SetJOB_TYP_RD(CString pJOB_TYP_RD);
	void SetJOB_TYP_OD(CString pJOB_TYP_OD);
	void SetPULP_SENSOR_RD(CString pPULP_SENSOR_RD);
	void SetPULP_SENSOR_OD(CString pPULP_SENSOR_OD);
	void SetTRAY_LEV_RD(CString pTRAY_LEV_RD);
	void SetTRAY_LEV_OD(CString pTRAY_LEV_OD);
	void SetTRAY_TYP_RD(CString pTRAY_TYP_RD);
	void SetTRAY_TYP_OD(CString pTRAY_TYP_OD);
	void SetFMS_RPT_RD(CString pFMS_RPT_RD);
	void SetFMS_PRT_OD(CString pFMS_RPT_OD);
	void SetTR_PAUSE_RD(CString pTR_PAUSE_RD);
	void SetTR_PAUSE_RD_NM(CString pTR_PAUSE_RD_NM);
	void SetTR_PAUSE_OD(CString pTR_PASUE_OD);
	void SetWAIT_TIME_RD(CString pWAIT_TIME_RD);
	void SetWAIT_TIME_OD(CString pWAIT_TIME_OD);
	void SetERR_RQ_RD(CString pERR_RQ_RD);
	void SetERR_RQ_OD(CString pERR_RQ_OD);
	void SetAUTO_MODE_RD(CString pAUTO_MODE_RD);
	void SetSTO_READY_RD(CString pSTO_READY_RD);
	void SetRET_READY_RD(CString pRET_READY_RD);
	void SetSTOHS_READY_RD(CString pSTOHS_READY_RD);
	void SetRETHS_READY_RD(CString pRETHS_READY_RD);
	void SetRTV_DEPARTHS_READY_RD(CString pRTV_DEPARTHS_READY_RD);
	void SetRTV_ARRIVEHS_READY_RD(CString pRTV_ARRIVEHS_READY_RD);
	void SetLGV_DEPARTHS_READY_RD(CString pLGV_DEPARTHS_READY_RD);
	void SetLGV_ARRIVEHS_READY_RD(CString pLGV_ARRIVEHS_READY_RD);
	void SetSENSOR0_DATA_RD(CString pSENSOR0_DATA_RD);
	void SetSENSOR1_DATA_RD(CString pSENSOR1_DATA_RD);
	void SetSENSOR2_DATA_RD(CString pSENSOR2_DATA_RD);
	void SetSENSOR3_DATA_RD(CString pSENSOR3_DATA_RD);
	void SetSENSOR4_DATA_RD(CString pSENSOR4_DATA_RD);
	void SetRTV_IN_SIGN(CString pRTV_IN_SIGN);
	void SetRTV_OUT_SIGN(CString pRTV_OUT_SIGN);
	void SetRTV_LOCK_SIGN(CString pRTV_LOCK_SIGN);
	void SetERROR_CODE(CString pERROR_CODE);
	void SetOD_RQ_YN(CString pOD_RQ_YN);
	void SetREAD_UPD_DT(CTime pREAD_UPD_DT);
	void SetWRITE_UPD_DT(CTime pWRITE_UPD_DT);
	void SetOD_USER_ID(CString pOD_USER_ID);
	void SetOD_UPD_DT(CTime pOD_UPD_DT);
	void SetFLOOR_NO(CString pFLOOR_NO);
	void SetBCR_BOTTOM(CString pBCR_BOTTOM);
	void SetBCR_TOP(CString pBCR_TOP);
	void SetUSE_YN(CString pUES_YN);
	void SetRET_HS_YN(CString pRET_HS_YN);
	void SetSUSPEND(CString pSUSPEND);
	void SetOD_RQ_FLAG(CString pOD_RQ_FLAG);
	void SetLIFTER_HS_YN(CString pLIFTER_HS_YN);
	void SetDP_HS_YN(CString pDP_HS_YN);
	void SetMG_HS_YN(CString pMG_HS_YN);
	void SetLOOP_NM(CString pLOOP_NM);
	void SetDESTINATION_YN(CString pDESTINATION_YN);
	void SetCMD_RQ_ID(CString pCMD_RQ_ID);
	void SetCMD_RQ_YN(CString pCMD_RQ_YN);
	void SetCOMP_MG(CString pCOMP_MG);
	void SetCV_BCR_BOTTOM(CString pCV_BCR_BOTTOM);
	void SetCV_BCR_TOP(CString pCV_BCR_TOP);
	void SetCOMP_DP(CString pCOMP_DP);
	void SetMES_SEND_YN(CString pMES_SEND_YN);
	void SetMES_ERR_SEND_YN(CString pMES_SEND_YN);
	void SetMES_ERROR_CD(CString pMES_ERROR_CD);

	void SetMC_NO(CString pMC_NO);
	void SetMC_NO_NM(CString pMC_NO_NM);
	void SetWAIT_SC_RET_JOB_RD(CString pWAIT_SC_RET_JOB_RD);
	void SetWAIT_SC_RET_JOB_RD_NM(CString pWAIT_SC_RET_JOB_RD_NM);

	void SetSTOCK_MODE(CString pSTOCK_MODE);
	void SetREMOTE_CONTROL(CString pREMOTE_CONTROL);
	void SetPULP_SENSOR_RD_NM(CString pPULP_SENSOR_RD_NM);

	void SetEQP_TIME(CString V_EQP_TIME);
	void SetEQP_CONNECTED_YN(CString V_EQP_CONNECTED_YN);
	void SetEQP_COLOR(CString V_EQP_COLOR);
	void SetCHK_BYPASS_YN(CString CHK_BYPASS_YN);
	void SetITEM_NO(CString ITEM_NO);



	void SetSC_PLT_JOB_TYP(CString V_SC_PLT_JOB_TYP);

public:
	virtual CString GetMapKeyCV();
	virtual CString GetMapKeyTRACK();
};


