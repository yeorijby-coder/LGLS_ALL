#pragma once
#include "afx.h"
#include "CV_DATA.h"
#include "DciButtonCtrl.h"

// BCR_MST 명령 대상입니다.

class CBCR_MST : public CObject
{
public:
	CBCR_MST(void);
	CBCR_MST(CString pstrWH_TYP, CString strBCR_MC_NO, CString pstrBCR_NO);

	~CBCR_MST(void);

public:
	BOOL m_bModified;

public:
	CDciButtonCtrl* m_pControl;

public:
	//key
	CString K_WH_TYP;
	CString K_BCR_NO;

public:
	CString m_strBCR_MC_NO;

public:
	void SetBCR_NO(CString pBCR_NO);
	void SetWH_TYP(CString K_WH_TYP);

public:

	//data
	CString V_WH_TYP;
	CString V_BCR_NO;
	CString V_BCR_MC_NO;
	CString V_UPD_DT;
	CString V_UPD_USER_ID;
	CString V_USE_YN;
	CString V_PLC_IP;
	CString V_PLC_PORT;
	CString V_SOCK_TIMEOUT;
	CString V_RETRY_YN;
	CString V_CONNECTED_YN;
	CString V_BCR_STA;
	CString V_BCR_LEV;
	CString V_NOREAD_CNT;
	CString V_CMD_RQ_ID;
	CString V_CMD_RQ_YN;
	CString V_SUSPEND;

public:
	void SetBCR_MC_NO(CString pBCR_MC_NO);
	void SetUPD_DT(CString pUPD_DT);
	void SetUPD_USER_ID(CString pUPD_USER_ID);
	void SetUSE_YN(CString pUSE_YN);
	void SetPLC_IP(CString pPLC_IP);
	void SetPLC_PORT(CString pPLC_PORT);
	void SetSOCK_TIME_OUT(CString pSOCK_TIME_OUT);
	void SetRETRY_YN(CString pRETRY_YN);
	void SetCONNECTED_YN(CString pCONNECTED_YN);
	void SetBCR_STA(CString pBCR_STA);
	void SetBCR_LEV(CString pBCR_LEV);
	void SetNOREAD_CNT(CString pNOREAD_CNT);
	void SetCMD_RQ_ID(CString pCMD_RQ_ID);
	void SetCMD_RQ_YN(CString pCMD_RQ_YN);
	void SetSUSPEND(CString pSUSPEND);

public:
	CString GetCid();
	CString GetCid(CString pnChar4);
};


